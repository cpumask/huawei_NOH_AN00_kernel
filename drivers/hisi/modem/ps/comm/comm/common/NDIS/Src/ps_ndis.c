/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Create: 2012-10-10
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
#include "ps_ndis.h"
#include "vos.h"
#include "mdrv.h"
#include "ps_tag.h"
#include "securec.h"
#include "mdrv_nvim.h"
#include "ps_common_def.h"
#include "nv_stru_gucnas.h"
#include "msp_at.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "msp_diag.h"
#endif
#include "LUPQueue.h"
#include "ps_iface_global_def.h"
#include "ip_nd_server.h"
#include "nv_stru_gucnas.h"
#include "ndis_drv.h"
#include "ipv4_dhcp_server.h"


#define THIS_MODU ps_ndis
/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
/*lint -e767*/
#define THIS_FILE_ID PS_FILE_ID_PSNDIS_C
/*lint +e767*/

/*
 * 2 Declare the Global Variable
 */
#define C2A_QUE_SIZE 512         /* 暂定512 */
#define USB_DATAREQ_QUE_SIZE 512 /* 暂定512 */

#define NDIS_PERIOD_ARP_TMRNAME 1
#define NDIS_ARP_REQ_TMRNAME 2

#define NDIS_ARP_FRAME_REV_OFFSET (((VOS_UINT64)(VOS_UINT_PTR)(&(((ETH_ArpFrame *)0)->rev[0]))) & 0xFFFFFFFF)


/* arp请求中间部分固定的值 */
const VOS_UINT8 g_arpReqFixVal[ETH_ARP_FIXED_MSG_LEN] = { 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01 };
/* arp响应中间部分固定的值 */
const VOS_UINT8 g_arpRspFixVal[ETH_ARP_FIXED_MSG_LEN] = { 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02 };
/* 广播地址，全1 */
const VOS_UINT8 g_broadCastAddr[ETH_MAC_ADDR_LEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

const VOS_UINT8 g_invalidAddr[IPV4_ADDR_LEN] = {0};

/* ARP周期 */
const VOS_UINT32 g_periodicArpCyc = 3000; /* 周期性ARP发送周期 */

/* 统计信息 */
NDIS_StatInfo g_ndisStatStru        = {0};

NDIS_Entity  g_ndisEntity[NAS_NDIS_MAX_ITEM] = {{0}};
NDIS_Entity *g_ndisEntityAddr                    = g_ndisEntity;

VOS_UINT32   g_nvMtu = 1500; /* IPV6 MTU默认取值 */

VOS_UINT32          g_ndisLomSwitch = 0;
SPE_MAC_EtherHeader g_speMacHeader  = {{ 0x58, 0x02, 0x03, 0x04, 0x05, 0x06 },
                                       { 0x00, 0x11, 0x09, 0x64, 0x01, 0x01 },
                                       0x00000000 }; /* mac地址初始化为固定值 */

/* 声明 */

/*
 * 功能描述: A核NDIS读取NV项的初始化函数
 * DHCP Lease Time, 设定范围为[1, 8784]小时
 * Vodafone    24小时
 * 其他        72小时
 * 时间经过DHCP Lease Time一半时，PC会主动发起续租，
 * 如果DHCP租约超期，则从PC通过NDIS通道发往单板的数据会出现目的不可达错误
 * 目前测试，当DHCP Lease Time小于等于4S时，对于数传影响较大，所以定义最小租约为1小时
 * 目前没有遇到超过8天的DHCP Lease Time，暂定上限为8784小时(366天)
 * 修改历史:
 *  1.日    期: 2012年4月24日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_NvItemInit(VOS_VOID)
{
    VOS_UINT32                       rtn;
    VOS_UINT32                       dhcpLeaseHour;
    VOS_UINT32                       ipv6Mtu;
    NDIS_NV_DhcpLeaseHour            ndisDhcpLeaseHour = {0};
    TAF_NDIS_NvIpv6RouterMtu         ndisIPv6Mtu = {0};

    /* 从NV读取流控配置信息 */
    /*lint -e718*/
    /*lint -e732*/
    /*lint -e746*/
    rtn = Ndis_NvimItem_Read(NV_ITEM_NDIS_DHCP_DEF_LEASE_TIME, &ndisDhcpLeaseHour,
                             sizeof(NDIS_NV_DhcpLeaseHour));
    if (rtn != PS_SUCC) {
        PS_PRINTF_ERR("Ndis_NvItemInit, Fail to read NV DHCP_LEASE_TIME: %d\n", rtn);
        ndisDhcpLeaseHour.dhcpLeaseHour = TTF_NDIS_DHCP_DEFAULT_LEASE_HOUR;
    }

    /* NV值合法性判断 */
    dhcpLeaseHour = ndisDhcpLeaseHour.dhcpLeaseHour;
    if ((dhcpLeaseHour > 0) && (dhcpLeaseHour <= TTF_NDIS_DHCP_MAX_LEASE_HOUR)) {
        /* 3600:hour -> second */
        g_leaseTime = dhcpLeaseHour * 3600;
    }

    /* 从NV读取IPV6 MTU信息 */
    rtn = Ndis_NvimItem_Read(NV_ITEM_IPV6_ROUTER_MTU, &ndisIPv6Mtu, sizeof(TAF_NDIS_NvIpv6RouterMtu));
    /*lint +e746*/
    /*lint +e732*/
    /*lint +e718*/
    if (rtn != PS_SUCC) {
        PS_PRINTF_ERR("Ndis_NvItemInit, Fail to read NV IPV6_MTU: %d\n!", rtn);
        ndisIPv6Mtu.ipv6RouterMtu = TTF_NDIS_IPV6_MTU_DEFAULT;
    }

    /* NV值合法性判断 */
    ipv6Mtu = ndisIPv6Mtu.ipv6RouterMtu;
    if (ipv6Mtu == 0) {
        g_nvMtu = TTF_NDIS_IPV6_MTU_DEFAULT;
    } else {
        g_nvMtu = ipv6Mtu;
    }

    return PS_SUCC;
}
/*
 * 功能描述: APP核NDIS功能的初始化函数
 * 修改历史:
 *  1.日    期: 2011年2月10日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_Init(VOS_VOID)
{
    VOS_UINT32                     loop;
    VOS_UINT8                     *macAddr        = VOS_NULL_PTR;
    VOS_UINT16                     payLoad;
    VOS_INT32                      rlt;
    VOS_UINT32                     len            = 2; /* ip payload为2个字节 */
    NDIS_ArpPeriodTimer           *arpPeriodTimer = VOS_NULL_PTR;

    /*lint -e746*/
    macAddr = (VOS_UINT8 *)Ndis_GetMacAddr(); /* 获得单板MAC地址 */
    /*lint -e746*/
    if (macAddr == VOS_NULL_PTR) {
        PS_PRINTF_ERR("Ndis_Init, Ndis_GetMacAddr Fail!\n");
        return PS_FAIL;
    }

    payLoad = IP_PAYLOAD;
    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        g_ndisEntity[loop].rabType    = NDIS_RAB_NULL;
        g_ndisEntity[loop].handle     = NDIS_INVALID_HANDLE;
        g_ndisEntity[loop].rabId      = NDIS_INVALID_RABID;
        g_ndisEntity[loop].used       = PS_FALSE;
        g_ndisEntity[loop].spePort     = NDIS_INVALID_SPEPORT;
        g_ndisEntity[loop].speIpfFlag = PS_FALSE;
        rlt = memcpy_s(g_ndisEntity[loop].ipV4Info.macFrmHdr + ETH_MAC_ADDR_LEN, ETH_MAC_ADDR_LEN,
                       macAddr, ETH_MAC_ADDR_LEN);
        NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

        rlt = memcpy_s(g_ndisEntity[loop].ipV4Info.macFrmHdr + (2 * ETH_MAC_ADDR_LEN), len, /* 2:addr */
                       (VOS_UINT8 *)(&payLoad), len);
        NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

        /* 周期性ARP定时器初始化 */
        arpPeriodTimer               = &(g_ndisEntity[loop].ipV4Info.arpPeriodTimer);
        arpPeriodTimer->tm          = VOS_NULL_PTR;
        arpPeriodTimer->name       = NDIS_PERIOD_ARP_TMRNAME;
        arpPeriodTimer->timerValue = g_periodicArpCyc;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        /* 创建下行IP包缓存队列 */
        if (NDIS_GET_DL_PKTQUE(loop) != VOS_NULL_PTR) {
            continue;
        }
        rlt = LUP_CreateQue(NDIS_TASK_PID, &(NDIS_GET_DL_PKTQUE(loop)),
                            NDIS_IPV4_WAIT_ADDR_RSLT_Q_LEN);
        if (rlt != PS_SUCC) {
            PS_PRINTF_ERR("Ndis_Init, LUP_CreateQue DlPktQue fail.\n");
            continue;
        }
#endif
    }

    if (Ndis_NvItemInit() != PS_SUCC) { /* NV项初始化 */
        PS_PRINTF_ERR("Ndis_Init, Ndis_NvItemInit Fail!\n");
        return PS_FAIL;
    }

    return PS_SUCC;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * 功能描述: C核单独复位时NDIS的回调处理函数
 * 修改历史:
 * 1.日    期: 2019年10月10日
 *   修改内容: 新生成函数
 */
STATIC VOS_INT Ndis_ModemResetCb(drv_reset_cb_moment_e param, VOS_INT userData)
{
    VOS_UINT32  result;
    VOS_UINT32  resetCbBefore;         /* 单独复位BEFORE统计 */
    VOS_UINT32  resetCbAfter;          /* 单独复位AFTER统计 */
    if (param == MDRV_RESET_CB_BEFORE) {
        PS_PRINTF_ERR("NDIS before reset: enter.");

        /* Ndis 初始化 */
        result = Ndis_Init();
        if (result != PS_SUCC) {
            PS_PRINTF_ERR("APP_Ndis_Pid_InitFunc, Ndis_Init fail!\n");
            return VOS_ERR;
        }

        /* 复位统计之外的统计数据清0 */
        resetCbBefore = g_ndisStatStru.resetCbBefore;
        resetCbAfter  = g_ndisStatStru.resetCbAfter;
        (VOS_VOID)memset_s(&g_ndisStatStru, sizeof(NDIS_StatInfo), 0x00,
                           sizeof(NDIS_StatInfo));

        /* 增加复位统计 */
        g_ndisStatStru.resetCbBefore = resetCbBefore;
        g_ndisStatStru.resetCbAfter  = resetCbAfter;
        NDIS_STAT_RESET_CB_BEFORE(1);

        PS_PRINTF_ERR("NDIS before reset: succ.");
        return VOS_OK;
    } else if (param == MDRV_RESET_CB_AFTER) {
        PS_PRINTF_ERR("NDIS after reset enter.");

        /* 增加复位统计 */
        NDIS_STAT_RESET_CB_AFTER(1);
        PS_PRINTF_ERR("NDIS after reset: succ.");
        return VOS_OK;
    } else {
        return VOS_ERROR;
    }
}

/*
 * Function Name: NDIS_SendDlPkt
 * Description: 发送下行缓存的IPV4数据包
 */
VOS_VOID NDIS_SendDlPkt(VOS_UINT32 addrIndex)
{
    IMM_Zc                 *immZc  = VOS_NULL_PTR;
    VOS_UINT8               rabId;
    ADS_PktTypeUint8        pktType;
    VOS_UINT16              app;
    VOS_INT32               lockKey;

    if (addrIndex >= NAS_NDIS_MAX_ITEM) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_SendDlPkt, addrIndex invalid!");
        return;
    }
    if (NDIS_GET_DL_PKTQUE(addrIndex) == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_SendDlPkt, que null!");
        return;
    }

    while (LUP_IsQueEmpty(NDIS_GET_DL_PKTQUE(addrIndex)) != PS_TRUE) {
        lockKey = VOS_SplIMP();
        if (LUP_DeQue(NDIS_GET_DL_PKTQUE(addrIndex), (VOS_VOID **)(&immZc)) != PS_SUCC) {
            VOS_Splx(lockKey);
            NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_SendDlPkt, LUP_DeQue return fail!");
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
            NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_SendDlPkt, Ndis_DlSendNcm fail!");
        }
        /*lint +e522*/
        NDIS_STAT_SEND_QUE_PKT_NUM(1);
    }

    return;
}

/*
 * 功能描述: 根据ExRabId查找NDIS实体的索引
 */
VOS_UINT32 NDIS_GetEntityIndex(VOS_UINT8 exRabId)
{
    VOS_UINT32 i = 0;

    /* 查询是否已存在相应Entity */
    do {
        if ((g_ndisEntity[i].used == PS_TRUE) && (exRabId == g_ndisEntity[i].rabId)) {
            /* 找到相应实体 */
            return i;
        }
    } while ((++i) < NAS_NDIS_MAX_ITEM);

    return NAS_NDIS_MAX_ITEM;
}
#endif

/*
 * 功能描述: 获取MAC地址
 * 修改历史:
 *  1.日    期: 2014年10月17日
 *    修改内容: 新生成函数
 */
VOS_UINT8 *Ndis_GetMacAddr(VOS_VOID)
{
    /* LTE协议栈MAC地址 */
    static VOS_UINT8 gUcMacAddressPstable[] = {
        0x4c, 0x54, 0x99, 0x45, 0xe5, 0xd5
    };

    return gUcMacAddressPstable;
}

/*
 * 功能描述: 发送Cnf消息到AT
 * 修改历史:
 *  1.日    期: 2011年3月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_SndMsgToAt(const VOS_UINT8 *buf, VOS_UINT16 msgLen, VOS_UINT32 msgId)
{
    MsgBlock        *msgBlock  = VOS_NULL_PTR;
    MSG_Header      *msgHeader = VOS_NULL_PTR;
    errno_t          rlt;

    /*lint -e516 -esym(516,*)*/
    msgBlock = (MsgBlock *)PS_ALLOC_MSG(NDIS_TASK_PID, msgLen - VOS_MSG_HEAD_LENGTH);
    /*lint -e516 +esym(516,*)*/
    if (msgBlock == VOS_NULL_PTR) {
        return PS_FAIL;
    }

    msgHeader = (MSG_Header *)(VOS_VOID *)msgBlock;

    rlt = memcpy_s(msgBlock->value, msgLen - VOS_MSG_HEAD_LENGTH, (buf + VOS_MSG_HEAD_LENGTH),
                   msgLen - VOS_MSG_HEAD_LENGTH);
    if (rlt != EOK) {
        PS_FREE_MSG(NDIS_TASK_PID, msgBlock);
        return PS_FAIL;
    }

    msgHeader->ulSenderPid   = NDIS_TASK_PID;
    msgHeader->ulReceiverPid = APP_AT_PID;
    msgHeader->msgName     = msgId;

    if (PS_SEND_MSG(NDIS_TASK_PID, msgBlock) != VOS_OK) {
        /* 异常打印 */
        return PS_FAIL;
    }

    return PS_SUCC;
}

#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
/*
 * 功能描述: Ndis检查ExRabId取值是否在合法范围内
 * 修改历史:
 *  1.日    期: 2018年08月21日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_ChkRabIdValid(VOS_UINT8 exRabId)
{
    /* B5000及以后的版本下，Ndis的索引为IFACE ID */
    if (exRabId >= PS_IFACE_ID_BUTT) {
        return PS_FAIL;
    }

    return PS_SUCC;
}
#else
/*
 * 功能描述: Ndis检查ExRabId取值是否在合法范围内
 * 修改历史:
 *  1.日    期: 2012年12月7日
 *    修改内容: 新生成函数
 *  2.日    期: 2013年1月15日
 *    修改内容: DSDA特性开发:对ModemID和RabId均做检查
 */
STATIC VOS_UINT32 Ndis_ChkRabIdValid(VOS_UINT8 exRabId)
{
    VOS_UINT16 modemId;
    VOS_UINT8  rabId;

    modemId = NDIS_GET_MODEMID_FROM_EXBID(exRabId);
    if (modemId >= MODEM_ID_BUTT) {
        return PS_FAIL;
    }

    rabId = NDIS_GET_BID_FROM_EXBID(exRabId);
    if ((rabId < MIN_VAL_EPSID) || (rabId > MAX_VAL_EPSID)) {
        return PS_FAIL;
    }

    return PS_SUCC;
}

#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#if (VOS_OS_VER != VOS_WIN32)
/*
 * 功能描述: 桩函数
 * 修改历史:
 *  1.日    期: 2018年08月21日
 *    修改内容: 新生成函数
 */
VOS_UINT32 ADS_DL_RegDlDataCallback(VOS_UINT8 rabId, RCV_DL_DATA_FUNC pFunc, VOS_UINT32 exParam)
{
    return PS_SUCC;
}
#endif
#else
#if (VOS_OS_VER != VOS_WIN32)
/*
 * 功能描述: 桩函数
 * 修改历史:
 *  1.日    期: 2018年08月21日
 *    修改内容: 新生成函数
 */
int ads_iface_register_rx_handler(VOS_UINT8 ifaceId, struct ads_iface_rx_handler_s *rxHandler)
{
    return PS_SUCC;
}
#endif
#endif
#endif
VOS_VOID Ndis_LomTraceRcvUlData(VOS_VOID)
{
    if (g_ndisLomSwitch == 1) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "User plane latency trace: NDIS rcv UL data");
    }
    return;
}

VOS_VOID Ndis_LomTraceRcvDlData(VOS_VOID)
{
    if (g_ndisLomSwitch == 1) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "User plane latency trace: NDIS rcv DL data");
    }
    return;
}
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * 功能描述: NDIS_ADS下行数据接收回调函数
 * 修改历史:
 *  1.日    期: 2011年12月9日
 *    修改内容: 新生成函数
 *  2.日    期: 2013年1月16日
 *    修改内容: DSDA特性开发，入参修改为扩展承载ID
 */
STATIC VOS_INT Ndis_DlAdsDataRcv(VOS_UINT8 exRabId, IMM_Zc *data, ADS_PktTypeUint8 pktType,
                                 VOS_UINT32 exParam)
{
    if (data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlAdsDataRcv, recv NULL PTR!");
        return PS_FAIL;
    }

    if ((Ndis_ChkRabIdValid(exRabId) != PS_SUCC) || (pktType >= ADS_PKT_TYPE_BUTT)) {
        /*lint -e522*/
        IMM_ZcFree(data);
        /*lint +e522*/
        NDIS_ERROR_LOG2(NDIS_TASK_PID, "Ndis_DlAdsDataRcv, recv RabId or PktType fail!", exRabId, pktType);
        NDIS_STAT_DL_DISCARD_ADSPKT(1);
        return PS_FAIL;
    }

    /* 增加从ADS接收到的数据包个数统计 */
    NDIS_STAT_DL_RECV_ADSPKT_SUCC(1);

    Ndis_LomTraceRcvDlData();
    if (Ndis_DlSendNcm(exRabId, pktType, data) != PS_SUCC) {
        /*lint -e522*/
        IMM_ZcFree(data);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlAdsDataRcv, Ndis_DlSendNcm fail!");
        return PS_FAIL;
    }

    return PS_SUCC;
}

/*
 * 功能描述: NDIS_ADS下行数据接收回调函数
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_INT Ndis_DlAdsDataRcvV2(unsigned long userData, IMM_Zc *data)
{
    ADS_PktTypeUint8 pktType;

    if (data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlAdsDataRcvV2, recv NULL PTR!");
        return PS_FAIL;
    }

    if (Ndis_ChkRabIdValid((VOS_UINT8)userData) != PS_SUCC) {
        /*lint -e522*/
        IMM_ZcFree(data);
        /*lint +e522*/
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_DlAdsDataRcvV2, recv RabId or PktType fail!", userData);
        NDIS_STAT_DL_DISCARD_ADSPKT(1);
        return PS_FAIL;
    }

    /* 增加从ADS接收到的数据包个数统计 */
    NDIS_STAT_DL_RECV_ADSPKT_SUCC(1);

    Ndis_LomTraceRcvDlData();
    if (IMM_ZcGetProtocol(data) == IP_PAYLOAD) {
        pktType = ADS_PKT_TYPE_IPV4;
    } else {
        pktType = ADS_PKT_TYPE_IPV6;
    }
    if (Ndis_DlSendNcm((VOS_UINT8)userData, pktType, data) != PS_SUCC) {
        /*lint -e522*/
        IMM_ZcFree(data);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlAdsDataRcvV2, Ndis_DlSendNcm fail!");
        return PS_FAIL;
    }

    return PS_SUCC;
}
#endif

VOS_UINT32 Ndis_UpdateMacAddr(VOS_UINT8 *ueMacAddr, VOS_UINT8 lenth, NDIS_Ipv4Info *arpV4Info)
{
    errno_t              rlt;
    if (lenth > ETH_MAC_ADDR_LEN) {
        return PS_FAIL;
    }
    rlt = memcpy_s(arpV4Info->ueMacAddr, ETH_MAC_ADDR_LEN, ueMacAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);
    rlt = memcpy_s(arpV4Info->macFrmHdr, ETH_MAC_HEADER_LEN, ueMacAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    arpV4Info->arpInitFlg = PS_TRUE;

    rlt = memcpy_s(g_speMacHeader.srcAddr, ETH_MAC_ADDR_LEN, (arpV4Info->macFrmHdr + ETH_MAC_ADDR_LEN),
                   ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    rlt = memcpy_s(g_speMacHeader.dstAddr, ETH_MAC_ADDR_LEN, ueMacAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#if (defined(CONFIG_BALONG_ESPE))
        mdrv_wan_set_ipfmac((VOS_VOID *)&g_speMacHeader);
#endif
#endif
    return PS_SUCC;
}

STATIC VOS_UINT32 Ndis_FillArpReplyInfo(ETH_ArpFrame *reqArp, NDIS_Ipv4Info *arpV4Info)
{
    errno_t              rlt;
    VOS_UINT32           tgtIpAddr   = reqArp->targetIP.ipAddr32bit;
    rlt = memcpy_s(reqArp->dstAddr, ETH_MAC_ADDR_LEN, reqArp->srcAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    rlt = memcpy_s(reqArp->targetAddr, ETH_MAC_ADDR_LEN, reqArp->srcAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    reqArp->targetIP.ipAddr32bit = reqArp->senderIP.ipAddr32bit;

    rlt = memcpy_s(reqArp->srcAddr, ETH_MAC_ADDR_LEN, (arpV4Info->macFrmHdr + ETH_MAC_ADDR_LEN),
                   ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    rlt = memcpy_s(reqArp->senderAddr, ETH_MAC_ADDR_LEN, (arpV4Info->macFrmHdr + ETH_MAC_ADDR_LEN),
                   ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    reqArp->senderIP.ipAddr32bit = tgtIpAddr;

    /* opcode */
    reqArp->opCode = ETH_ARP_RSP_TYPE;
    return PS_SUCC;
}

/*
 * 功能描述: 处理ARP Request 帧
 */
STATIC VOS_UINT32 Ndis_ProcReqArp(ETH_ArpFrame *reqArp, VOS_UINT8 rabId)
{
    NDIS_Entity         *ndisEntity = VOS_NULL_PTR;
    NDIS_Ipv4Info       *arpV4Info  = VOS_NULL_PTR;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    VOS_UINT32           addrIndex;
#endif
    ndisEntity = NDIS_GetEntityByRabId(rabId);
    if (ndisEntity == VOS_NULL_PTR) {
        return PS_FAIL;
    }

    arpV4Info = &ndisEntity->ipV4Info;

    if ((reqArp->senderIP.ipAddr32bit != 0) /* 兼容MAC OS 免费ARP类型,其Sender IP为0 */
        && (arpV4Info->ueIpInfo.ipAddr32bit != reqArp->senderIP.ipAddr32bit)) {
        /* 源UE IP与网侧配置不符，这种情况不处理 */
        NDIS_STAT_PROC_NOTUE_ARP(1);
        return PS_SUCC;
    }

    /* 更新PC MAC地址 */
    if (Ndis_UpdateMacAddr(reqArp->senderAddr, ETH_MAC_ADDR_LEN, arpV4Info) == PS_FAIL) {
        NDIS_PRINT_ERROR(Ndis_ProcReqArp_ENUM, IP_COM_RETURN_FAIL);
        return PS_FAIL;
    }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    addrIndex = NDIS_GetEntityIndex(rabId);
    /* 发送下行IP缓存队列中的数据包 */
    NDIS_SendDlPkt(addrIndex);
#endif


    /* 免费ARP不回复响应 */
    if ((reqArp->targetIP.ipAddr32bit == reqArp->senderIP.ipAddr32bit) || (reqArp->senderIP.ipAddr32bit == 0)) {
        NDIS_STAT_PROC_FREE_ARP(1);
        return PS_SUCC;
    }
    /* 发送响应 */
    if (Ndis_FillArpReplyInfo(reqArp, arpV4Info) == PS_FAIL) {
        NDIS_PRINT_ERROR1(Ndis_ProcReqArp_ENUM, IP_COM_RETURN_FAIL, PS_FAIL);
        return PS_FAIL;
    }
    /* 发送响应 */
    if (Ndis_SendMacFrm((VOS_UINT8 *)reqArp, sizeof(ETH_ArpFrame), rabId) == PS_FAIL) {
        NDIS_PRINT_ERROR(Ndis_ProcReqArp_ENUM, IP_COM_SEND_DATA_FAIL);
        return PS_FAIL;
    }
    NDIS_STAT_DL_SEND_ARP_REPLY(1);

    return PS_SUCC;
}
/*
 * 功能描述: 处理ARP Reply 帧,更新PC的MAC地址
 * 修改历史:
 *  1.日    期: 2009年12月31日
 *    修改内容: 新生成函数
 *  2.日    期: 2013年1月22日
 *    修改内容: DSDA
 */
STATIC VOS_UINT32 Ndis_ProcReplyArp(ETH_ArpFrame *rspArp, VOS_UINT8 rabId)
{
    NDIS_Ipv4Info *arpV4Info  = VOS_NULL_PTR;
    VOS_UINT32     targetIP   = rspArp->targetIP.ipAddr32bit;
    NDIS_Entity   *ndisEntity;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    VOS_UINT32     addrIndex;
#endif

    ndisEntity = NDIS_GetEntityByRabId(rabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_ProcReplyArp,  NDIS_GetEntityByRabId Error!", rabId);
        NDIS_PRINT_ERROR1(Ndis_ProcReplyArp_ENUM, IP_COM_CHECK_ERROR, rabId);
        return PS_FAIL;
    }

    arpV4Info = &ndisEntity->ipV4Info;

    if (targetIP == ndisEntity->ipV4Info.gwIpInfo.ipAddr32bit) {
        /* 更新PC MAC地址 */
        if (Ndis_UpdateMacAddr(rspArp->senderAddr, ETH_MAC_ADDR_LEN, arpV4Info) == PS_FAIL) {
            NDIS_PRINT_ERROR(Ndis_ProcReqArp_ENUM, IP_COM_RETURN_FAIL);
            return PS_FAIL;
        }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        addrIndex = NDIS_GetEntityIndex(rabId);
        /* 发送下行IP缓存队列中的数据包 */
        NDIS_SendDlPkt(addrIndex);
#endif

        arpV4Info->arpRepFlg = PS_TRUE;
        return PS_SUCC;
    }

    return PS_FAIL;
}
/*
 * 功能描述: 处理底软发送上来的ARP帧
 * 修改历史:
 *  1.日    期: 2009年12月31日
 *    修改内容: 新生成函数
 */
VOS_VOID Ndis_ProcArpMsg(ETH_ArpFrame *arpMsg, VOS_UINT8 rabId)
{
    /* Arp Request */
    if (VOS_MemCmp((VOS_UINT8 *)arpMsg + ETH_MAC_HEADER_LEN, g_arpReqFixVal, ETH_ARP_FIXED_MSG_LEN) == 0) {
        NDIS_STAT_UL_RECV_ARP_REQUEST(1);
        if (Ndis_ProcReqArp(arpMsg, rabId) == PS_FAIL) {
            NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_ProcArpMsg,  procreqarp fail!");
            NDIS_PRINT_INFO1(Ndis_ProcArpMsg_ENUM, IP_COM_PRINT_INFO, rabId);
        }
    } else if (VOS_MemCmp((VOS_UINT8 *)arpMsg + ETH_MAC_HEADER_LEN, g_arpRspFixVal, ETH_ARP_FIXED_MSG_LEN) == 0) {
        NDIS_STAT_DL_RECV_ARP_REPLY(1);
        if (Ndis_ProcReplyArp(arpMsg, rabId) == PS_FAIL) {
            NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_ProcArpMsg,  ProcReplyArp fail!");
            NDIS_PRINT_INFO(Ndis_ProcArpMsg_ENUM, IP_COM_PRINT_INFO);
        }
    } else {
        NDIS_PRINT_INFO(Ndis_ProcArpMsg_ENUM, IP_COM_CHECK_ERROR);
        NDIS_STAT_PROC_ARP_FAIL(1);
    }

    return;
}


STATIC VOS_VOID Ndis_UlNcmFrmProcArp(VOS_UINT8 exRabId, NDIS_Mem *pktNode, ETHFRM_Ipv4Pkt *ipPacket)
{
    VOS_UINT32            dataLen;
    /* 长度异常判断 */
    dataLen = NDIS_MemGetUsedLen(pktNode);
    /*lint -e413*/
    if (dataLen < NDIS_ARP_FRAME_REV_OFFSET) {
        /*lint -e522*/
        NDIS_MemFree(pktNode);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ulDataLen less than NDIS_ARP_FRAME_REV_OFFSET!");
        NDIS_PRINT_ERROR1(Ndis_UlNcmFrmProcArp_ENUM, IP_COM_MSGLEN_ERR, dataLen);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    /* ARP处理函数入参中增加RabId，后续以RabId作为Ndis实体遍历索引 */
    Ndis_ProcArpMsg((ETH_ArpFrame *)(VOS_VOID *)ipPacket, exRabId); /*lint !e527*/
    /* 处理完ARP后调用Imm_ZcFree释放ImmZc */
    /*lint -e522*/
    NDIS_MemFree(pktNode);
    /*lint +e522*/
    return;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
STATIC VOS_UINT32 Ndis_UlNcmFrmProcIpv6(VOS_UINT8 exRabId, IMM_Zc *immZc, ETHFRM_Ipv4Pkt *ipPacket)
{
    VOS_UINT32            dataLen;
    dataLen = IMM_ZcGetUsedLen(immZc);
    if ((dataLen - ETH_MAC_HEADER_LEN) > g_nvMtu) {
        NDIS_SPE_MEM_UNMAP(immZc, dataLen);
        IP_NDSERVER_ProcTooBigPkt(exRabId, ((VOS_UINT8 *)ipPacket + ETH_MAC_HEADER_LEN), g_nvMtu);
        NDIS_SPE_MEM_MAP(immZc, dataLen);

        /* 调用Imm_ZcFree释放ImmZc */
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        return IP_FAIL;
    }
    return IP_SUCC;
}
STATIC VOS_UINT32 Ndis_UlNcmFrmProcIpv4(IMM_Zc *immZc, VOS_UINT32 cacheLen, VOS_UINT32 dataLen)
{
    VOS_UINT8            *data     = VOS_NULL_PTR;
    ETH_Ipfixhdr         *ipFixHdr = VOS_NULL_PTR;
    VOS_UINT32            ipLen;
    data = IMM_ZcGetDataPtr(immZc);
    if (data == VOS_NULL_PTR) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        return IP_FAIL;
    }

    NDIS_SPE_MEM_UNMAP(immZc, cacheLen);

    /* 长度异常判断 */
    /*lint -e644*/
    if (dataLen < sizeof(ETH_Ipfixhdr)) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ulDataLen less than size of ETH_Ipfixhdr!");
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return IP_FAIL;
    }
    /*lint +e644*/
    ipFixHdr = (ETH_Ipfixhdr *)((VOS_VOID *)data);
    ipLen     = IP_NTOHS(ipFixHdr->totalLen);
    if (ipLen < immZc->len) {
        immZc->tail -= (immZc->len - ipLen);
        immZc->len = ipLen;
    }

    NDIS_SPE_MEM_MAP(immZc, cacheLen);
    return IP_SUCC;
}

STATIC VOS_UINT32 Ndis_UlSendAds(VOS_UINT8 exRabId, NDIS_Mem *pktNode, VOS_UINT16 frameType)
{
#if (FEATURE_OFF == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    if (ADS_UL_SendPacket(pktNode, exRabId) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ADS_UL_SendPacket fail!");
        return PS_FAIL;
    }
#else
    IMM_ZcSetProtocol(pktNode, frameType);
    if (ads_iface_tx(exRabId, pktNode) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ads_iface_tx fail!");
        return PS_FAIL;
    }
#endif
    return PS_SUCC;
}

#endif

STATIC VOS_UINT32 Ndis_UlNcmProcByFrameType(ETHFRM_Ipv4Pkt *ipPacket, VOS_UINT8 exRabId, NDIS_Mem *pktNode)
{
    VOS_UINT16            frameType = ipPacket->frameType;

    /* ARP处理 */
    if (frameType == ARP_PAYLOAD) {
        Ndis_UlNcmFrmProcArp(exRabId, pktNode, ipPacket);
        return PS_FAIL;
    }
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    /* IPV6超长包处理 */
    if (frameType == IPV6_PAYLOAD) {
        if (Ndis_UlNcmFrmProcIpv6(exRabId, pktNode, ipPacket) == IP_FAIL) {
            return PS_FAIL;
        }
    }
#endif
    return PS_SUCC;
}

/*
 * 功能描述: 对上行NCM帧的处理
 */
VOS_VOID Ndis_UlNcmFrmProc(VOS_UINT8 exRabId, NDIS_Mem *pktNode)
{
    ETHFRM_Ipv4Pkt       *ipPacket = VOS_NULL_PTR;
    VOS_UINT8            *data     = VOS_NULL_PTR;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    VOS_UINT32            dataLen = 0;
    VOS_UINT32            ipv4Flag = 0;
    VOS_UINT32            cacheLen = 0;
#endif

    data = NDIS_MemGetDataPtr(pktNode);
    if (data == VOS_NULL_PTR) {
        /*lint -e522*/
        NDIS_MemFree(pktNode);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, IMM_ZcGetDataPtr fail!");
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    ipPacket = (ETHFRM_Ipv4Pkt *)(VOS_VOID *)data;
    if (Ndis_UlNcmProcByFrameType(ipPacket, exRabId, pktNode) != PS_SUCC) {
        return;
    }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    if (ipPacket->frameType == IP_PAYLOAD) {
        ipv4Flag = 1;
        dataLen  = IMM_ZcGetUsedLen(pktNode);
        cacheLen = (dataLen < NDIS_SPE_CACHE_HDR_SIZE) ? dataLen : NDIS_SPE_CACHE_HDR_SIZE;
    }

    /* 经MAC层过滤后剩余的IP包发送，去掉MAC帧头后递交ADS */
    if (IMM_ZcRemoveMacHead(pktNode) != VOS_OK) {
        /*lint -e522*/
        IMM_ZcFree(pktNode);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, IMM_ZcRemoveMacHead fail!");
        return;
    }

    /* 检查IPV4包长度和实际Skb长度，如果不一致，则修改Skb长度为实际IP包长度 */
    if (ipv4Flag == 1) {
        if (Ndis_UlNcmFrmProcIpv4(pktNode, cacheLen, dataLen) == IP_FAIL) {
            return;
        }
    }
    if (Ndis_UlSendAds(exRabId, pktNode, ipPacket->frameType) != PS_SUCC) {
        return;
    }
    NDIS_STAT_UL_SEND_ADSPKT(1);
#endif
    return;
}

/*
 * 功能描述: 根据Handle查找RabId
 * 修改历史:
 *  1.日    期: 2011年12月9日
 *    修改内容: 新生成函数
 */
VOS_UINT8 Ndis_FindRabIdByHandle(VOS_INT32 rabHandle, VOS_UINT16 frameType)
{
    VOS_UINT32        loop;
    VOS_UINT8         tmpRabType;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;

    if ((frameType == ARP_PAYLOAD) || (frameType == IP_PAYLOAD)) {
        tmpRabType = NDIS_ENTITY_IPV4;
    } else if (frameType == IPV6_PAYLOAD) {
        tmpRabType = NDIS_ENTITY_IPV6;
    } else {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_FindRabIdByHandle, FrameType Error!", frameType);
        return NDIS_INVALID_RABID;
    }

    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        ndisEntity = &g_ndisEntity[loop];

        if ((rabHandle == ndisEntity->handle) &&
            (tmpRabType == (ndisEntity->rabType & tmpRabType))) { /* 数据包类型与承载类型一致 */
            return ndisEntity->rabId;
        }
    }

    return NDIS_INVALID_RABID;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
/*
 * 功能描述: 上行Vcom数据读取
 * 修改历史:
 *  1.日    期: 2020年4月6日
 *    修改内容: 新生成函数
 */
VOS_VOID NDIS_VcomReadCb(void *priv, struct vcom_rw_info *pktNode)
{
    VOS_UINT8             exRabId;
    VOS_INT32             handle;
    VOS_UINT16            frameType;
    ETHFRM_Ipv4Pkt       *ipPacket = VOS_NULL_PTR;
    VOS_UINT32            dataLen;

    if (pktNode == VOS_NULL_PTR || priv == VOS_NULL_PTR) {
        NDIS_PRINT_ERROR(NDIS_VcomReadCb_ENUM, IP_COM_NULL_PTR);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    if (pktNode->buf == VOS_NULL_PTR) {
        NDIS_MemFree(pktNode);
        NDIS_PRINT_ERROR(NDIS_VcomReadCb_ENUM, IP_COM_NULL_PTR);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    handle = *(VOS_INT32 *)priv;

    /* 长度异常判断 */
    dataLen = NDIS_MemGetUsedLen(pktNode);
    if (dataLen < ETH_MAC_HEADER_LEN) {
        NDIS_MemFree(pktNode);
        NDIS_PRINT_ERROR1(NDIS_VcomReadCb_ENUM, IP_COM_INPUT_PARA_ERROR, dataLen);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    ipPacket = (ETHFRM_Ipv4Pkt *)(VOS_VOID *)(pktNode->buf);
    frameType = ipPacket->frameType;

    /* NDIS下移到C核，只处理ARP报文 */
    if (frameType != ARP_PAYLOAD) {
        NDIS_MemFree(pktNode);
        NDIS_PRINT_ERROR1(NDIS_VcomReadCb_ENUM, IP_COM_DATA_TYPE_ERROR, frameType);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    /* 这里获取的是扩展RabId */
    exRabId = Ndis_FindRabIdByHandle(handle, frameType);
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_MemFree(pktNode);
        NDIS_PRINT_ERROR1(NDIS_VcomReadCb_ENUM, IP_COM_CHECK_ERROR, exRabId);
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return;
    }

    NDIS_STAT_UL_RECV_USBPKT_SUCC(1);

    Ndis_UlNcmFrmProc(exRabId, pktNode);

    return;
}

/*
 * 功能描述: 下行Vcom写数据回调
 * 修改历史:
 *  1.日    期: 2020年4月6日
 *    修改内容: 新生成函数
 */
VOS_VOID NDIS_VcomWriteCb(void *priv, struct vcom_rw_info *pktNode)
{
    if (pktNode == VOS_NULL_PTR) {
        NDIS_PRINT_ERROR(NDIS_VcomWriteCb_ENUM, IP_COM_NULL_PTR);
        return;
    }

    /* 释放内存 */
    if (pktNode->buf != VOS_NULL_PTR) {
        NDIS_MemFree(pktNode);
        pktNode->buf = VOS_NULL_PTR;
    }
    return;
}
#else
/*
 * Description: 当未获得PC MAC地址时，缓存下行IP包
 * History: VOS_UINT8
 */
VOS_VOID NDIS_MacAddrInvalidProc(IMM_Zc *immZc, VOS_UINT8 addrIndex)
{
    IMM_Zc      *queHead = VOS_NULL_PTR;

    NDIS_STAT_MAC_INVALID_PKT_NUM(1);
    if (addrIndex >= NAS_NDIS_MAX_ITEM) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_MacAddrInvalidProc, addrIndex invalid!");
        return;
    }

    if (NDIS_GET_DL_PKTQUE(addrIndex) == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_MacAddrInvalidProc, que null!");
        return;
    }

    /* **********************下行IPV4数据包缓存******************************** */
    if (LUP_IsQueFull(NDIS_GET_DL_PKTQUE(addrIndex)) == PS_TRUE) {
        if (LUP_DeQue(NDIS_GET_DL_PKTQUE(addrIndex), (VOS_VOID **)(&queHead)) != PS_SUCC) {
            /*lint -e522*/
            IMM_ZcFree(immZc);
            /*lint +e522*/
            NDIS_ERROR_LOG(NDIS_TASK_PID, "NdSer_MacAddrInvalidProc, LUP_DeQue return NULL");
            return;
        }
        /*lint -e522*/
        IMM_ZcFree(queHead); /* 释放最早的IP包 */
        /*lint +e522*/
    }

    if (LUP_EnQue(NDIS_GET_DL_PKTQUE(addrIndex), immZc) != PS_SUCC) { /* 插入最新的IP包 */
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NdSer_MacAddrInvalidProc, LUP_EnQue return NULL");
        return;
    }

    NDIS_STAT_ENQUE_PKT_NUM(1);

    return;
}

/*
 * 功能描述: App核间USB通道
 * 修改历史:
 *  1.日    期: 2011年1月31日
 *    修改内容: 新生成函数
 */
VOS_UINT32 AppNdis_UsbReadCb(VOS_INT32 rbIdHandle, VOS_VOID *pktNode)
{
    IMM_Zc *immZc = (IMM_Zc *)pktNode; /* 目前ImmZc和sk_buff完全一致，直接强转 */

    VOS_UINT8             exRabId;
    VOS_UINT16            frameType;
    ETHFRM_Ipv4Pkt       *ipPacket = VOS_NULL_PTR;
    VOS_UINT8            *data     = VOS_NULL_PTR;
    VOS_UINT32            dataLen;

    if (immZc == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "AppNdis_UsbReadCb read NULL PTR!");
        return PS_FAIL;
    }

    data = IMM_ZcGetDataPtr(immZc);
    if (data == VOS_NULL_PTR) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, IMM_ZcGetDataPtr fail!");
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return PS_FAIL;
    }

    /* 长度异常判断 */
    dataLen = IMM_ZcGetUsedLen(immZc);
    if (dataLen < ETH_MAC_HEADER_LEN) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ulDataLen less than ETH_MAC_HEADER_LEN!");
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return PS_FAIL;
    }

    ipPacket = (ETHFRM_Ipv4Pkt *)(VOS_VOID *)data;
    frameType = ipPacket->frameType;

    /* 这里获取的是扩展RabId */
    exRabId = Ndis_FindRabIdByHandle(rbIdHandle, frameType);
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, Ndis_ChkRabIdValid fail!");
        NDIS_STAT_UL_DISCARD_USBPKT(1);
        return PS_FAIL;
    }

    NDIS_STAT_UL_RECV_USBPKT_SUCC(1);

    Ndis_LomTraceRcvUlData();
    Ndis_UlNcmFrmProc(exRabId, immZc);

    return PS_SUCC;
}

/*
 * 功能描述: 根据SpePort查找RabId
 * 修改历史:
 *  1.日    期: 2011年12月9日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT8 Ndis_FindRabIdBySpePort(VOS_INT32 spePort, VOS_UINT16 frameType)
{
    VOS_UINT32        loop;
    VOS_UINT8         tmpRabType;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;

    switch (frameType) {
        case ARP_PAYLOAD:
            tmpRabType = NDIS_ENTITY_IPV4; /* ARP包也经过SPE */
            break;
        case IP_PAYLOAD:
            tmpRabType = NDIS_ENTITY_IPV4;
            break;
        case IPV6_PAYLOAD:
            tmpRabType = NDIS_ENTITY_IPV6;
            break;
        default:
            NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_FindRabIdByHandle, FrameType Error!", frameType);
            return NDIS_INVALID_RABID;
    }
    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        ndisEntity = &g_ndisEntity[loop];

        if ((spePort == ndisEntity->spePort) &&
            (tmpRabType == (ndisEntity->rabType & tmpRabType))) { /* 数据包类型与承载类型一致 */
            return ndisEntity->rabId;
        }
    }

    return NDIS_INVALID_RABID;
}

/*
 * 功能描述: App核间SPE通道
 * 修改历史:
 *  1.日    期: 2015年1月31日
 *    修改内容: 新生成函数
 */
VOS_UINT32 AppNdis_SpeReadCb(VOS_INT32 lSpePort, VOS_VOID *pktNode)
{
    IMM_Zc *immZc = (IMM_Zc *)pktNode; /* 目前ImmZc和sk_buff完全一致，直接强转 */

    VOS_UINT16            frameType;
    ETHFRM_Ipv4Pkt       *ipPacket = VOS_NULL_PTR;
    VOS_UINT8            *data     = VOS_NULL_PTR;
    VOS_UINT8             exRabId;
    VOS_UINT32            dataLen;

    if (immZc == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "AppNdis_UsbReadCb read NULL PTR!");
        return PS_FAIL;
    }

    data = IMM_ZcGetDataPtr(immZc);
    if (data == VOS_NULL_PTR) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, IMM_ZcGetDataPtr fail!");
        return PS_FAIL;
    }

    /* 长度异常判断 */
    dataLen = IMM_ZcGetUsedLen(immZc);
    if (dataLen < ETH_MAC_HEADER_LEN) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, ulDataLen less than ETH_MAC_HEADER_LEN!");
        return PS_FAIL;
    }

    ipPacket = (ETHFRM_Ipv4Pkt *)(VOS_VOID *)data;
    frameType = ipPacket->frameType;

    /* 这里获取的是扩展RabId */
    exRabId = Ndis_FindRabIdBySpePort(lSpePort, frameType);
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_UlNcmFrmProc, Ndis_ChkRabIdValid fail!");
        return PS_FAIL;
    }

    Ndis_LomTraceRcvUlData();
    Ndis_UlNcmFrmProc(exRabId, immZc);

    return PS_SUCC;
}

/*
 * 功能描述: 下行方向的IPV4 NCM数据的发送
 */
STATIC VOS_UINT32 Ndis_DlUsbIpv4Proc(VOS_UINT8 *addData, VOS_UINT8 exRabId, IMM_Zc *immZc)
{
    VOS_UINT8         index;
    NDIS_Entity       *ndisEntity = VOS_NULL_PTR;
    VOS_UINT16        app;
    VOS_UINT16        tmpApp;

    /* 使用ExRabId获取NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_DlUsbIpv4Proc, NDIS_GetEntityByRabId Error!", exRabId);
        return PS_FAIL;
    }

    if (ndisEntity->ipV4Info.arpInitFlg == PS_FALSE) {
        /* 将ucExRabId和数据包类型放入ImmZc的私有数据域中 */
        tmpApp = (ADS_PKT_TYPE_IPV4 & 0xFF);
        app    = ((VOS_UINT16)(tmpApp << 8)) | (exRabId); /* 8:bit */
        IMM_ZcSetUserApp(immZc, app);
        index = NDIS_GetEntityIndex(exRabId);
        NDIS_MacAddrInvalidProc(immZc, index);
        return PS_SUCC;
    }

    /* 填充MAC帧头，调用ImmZc接口将MAC帧头填入ImmZc中 */
    addData = ndisEntity->ipV4Info.macFrmHdr;
    return PS_RSLT_CODE_BUTT;
}

/*
 * 功能描述: 下行方向的IPV6 NCM数据的发送
 */
STATIC VOS_UINT32 Ndis_DlUsbIpv6Proc(VOS_UINT8 *addData, VOS_UINT8 exRabId, IMM_Zc *immZc)
{
    VOS_UINT8         index;
    VOS_UINT8         teAddrState;
    VOS_UINT16        app;
    VOS_UINT16        tmpApp;

    /* 填充MAC帧头，调用ImmZc接口将MAC帧头填入ImmZc中 */
    index = (VOS_UINT8)IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (index >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlUsbIpv6Proc, IP_NDSERVER_GET_ADDR_INFO_INDEX fail!");
        return PS_FAIL;
    }
    teAddrState = IP_NDSERVER_TE_ADDR_BUTT;
    addData = NdSer_GetMacFrm(index, &teAddrState);
    if (addData == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlUsbIpv6Proc, NdSer_GetMacFrm fail!");
        NDIS_STAT_DL_GET_IPV6MAC_FAIL(1);
        return PS_FAIL;
    }

    if (teAddrState != IP_NDSERVER_TE_ADDR_REACHABLE) {
        /* 将ucExRabId和数据包类型放入ImmZc的私有数据域中 */
        tmpApp = (ADS_PKT_TYPE_IPV6 & 0xFF);
        app    = ((VOS_UINT16)(tmpApp << 8)) | (exRabId); /* 8:bit */
        IMM_ZcSetUserApp(immZc, app);

        NdSer_MacAddrInvalidProc(immZc, index);
        return PS_SUCC;
    }

    return PS_RSLT_CODE_BUTT;
}


/*
 * 功能描述: 下行方向的NCM数据的发送
 */
STATIC VOS_UINT32 Ndis_DlUsbSendNcm(VOS_UINT8 exRabId, ADS_PktTypeUint8 pktType, IMM_Zc *immZc)
{
    VOS_UINT8        *addData = VOS_NULL_PTR;
    VOS_UINT32        size;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;
    VOS_UINT32        result;

    /* 使用ExRabId获取NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_DlSendNcm, NDIS_GetEntityByRabId Error!", exRabId);
        return PS_FAIL;
    }

    /* 填充MAC帧头，调用ImmZc接口将MAC帧头填入ImmZc中 */
    if ((pktType == ADS_PKT_TYPE_IPV4) && ((ndisEntity->rabType & NDIS_ENTITY_IPV4) == NDIS_ENTITY_IPV4)) {
        result = Ndis_DlUsbIpv4Proc(addData, exRabId, immZc);
        if (result != PS_RSLT_CODE_BUTT) {
            return result;
        }
    } else if ((pktType == ADS_PKT_TYPE_IPV6) && ((ndisEntity->rabType & NDIS_ENTITY_IPV6) == NDIS_ENTITY_IPV6)) {
        result = Ndis_DlUsbIpv6Proc(addData, exRabId, immZc);
        if (result != PS_RSLT_CODE_BUTT) {
            return result;
        }
    } else { /* 数据包类型与承载支持类型不一致 */
        NDIS_ERROR_LOG2(NDIS_TASK_PID, "Ndis_DlSendNcm, Rab is different from PktType!", ndisEntity->rabType, pktType);
        NDIS_STAT_DL_PKT_DIFF_RAB_NUM(1);
        return PS_FAIL;
    }

    if (IMM_ZcAddMacHead(immZc, addData) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlSendNcm, IMM_ZcAddMacHead fail!");
        NDIS_STAT_DL_ADDMACFRM_FAIL(1);
        return PS_FAIL;
    }

    size = IMM_ZcGetUsedLen(immZc); /* 加上以太网帧头的长度 */
    /*lint -e718*/
    if (NDIS_UDI_WRITE(ndisEntity->handle, immZc, size) != 0) {
        NDIS_STAT_DL_SEND_USBPKT_FAIL(1);
        return PS_FAIL;
    }
    /*lint +e718*/

    NDIS_STAT_DL_SEND_USBPKT_SUCC(1);
    return PS_SUCC;
}

/*
 * 功能描述: 下行方向的NCM数据的发送
 * 修改历史:
 *  1.日    期: 2011年2月11日
 *    修改内容: 新生成函数
 *     2.日    期: 2015年2月11日
 *    修改内容: SPE
 */
VOS_UINT32 Ndis_DlSendNcm(VOS_UINT8 exRabId, ADS_PktTypeUint8 pktType, IMM_Zc *immZc)
{
    VOS_UINT32        result;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;

    /* 使用ExRabId获取NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == IP_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DlSendNcm, NDIS_GetEntityByRabId fail!");
        return PS_FAIL;
    }

    result = Ndis_DlUsbSendNcm(exRabId, pktType, immZc);
    return result;
}
#endif

/* NDIS ARP PROC Begin */
/*
 * 功能描述: 发送ARP Request 帧到Ethenet上
 * 修改历史:
 *  1.日    期: 2009年12月31日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_SendRequestArp(NDIS_Ipv4Info *arpInfoItem, VOS_UINT8 exRabId)
{
    ETH_ArpFrame arpReq;
    errno_t      rlt;
    /* 之前一次发送的Req尚未受到Reply反馈 */
    if (arpInfoItem->arpRepFlg == PS_FALSE) {
        NDIS_STAT_ARPREPLY_NOTRECV(1);
        /* 做一次告警日志 */
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)&arpReq, sizeof(ETH_ArpFrame), 0, sizeof(ETH_ArpFrame));

    /* 组ARP Request */
    rlt = memcpy_s(arpReq.dstAddr, ETH_MAC_ADDR_LEN, g_broadCastAddr, ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    rlt = memcpy_s(arpReq.srcAddr, ETH_MAC_ADDR_LEN, arpInfoItem->macFrmHdr + ETH_MAC_ADDR_LEN,
                   ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);
    arpReq.frameType = ARP_PAYLOAD;

    /* 请求的固定部分 */
    rlt = memcpy_s(((VOS_UINT8 *)&arpReq + ETH_MAC_HEADER_LEN), ETH_ARP_FIXED_MSG_LEN, g_arpReqFixVal,
                   ETH_ARP_FIXED_MSG_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    /* Payload部分的MAC地址设置 */
    (VOS_VOID)memset_s(arpReq.targetAddr, ETH_MAC_ADDR_LEN, 0, ETH_MAC_ADDR_LEN);
    rlt = memcpy_s(arpReq.senderAddr, ETH_MAC_ADDR_LEN, arpInfoItem->macFrmHdr + ETH_MAC_ADDR_LEN,
                   ETH_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_VAL((rlt != EOK), PS_FAIL);

    /* 单板IP */
    arpReq.senderIP.ipAddr32bit = arpInfoItem->gwIpInfo.ipAddr32bit;
    arpReq.targetIP.ipAddr32bit = arpInfoItem->ueIpInfo.ipAddr32bit;

    if (Ndis_SendMacFrm((VOS_UINT8 *)&arpReq, sizeof(ETH_ArpFrame), exRabId) != PS_SUCC) {
        arpInfoItem->arpRepFlg = PS_TRUE;
        NDIS_STAT_DL_SEND_ARP_REQUEST_FAIL(1);
        return PS_FAIL;
    }

    NDIS_STAT_DL_SEND_ARP_REQUEST_SUCC(1);

    arpInfoItem->arpRepFlg = PS_FALSE;

    return PS_SUCC;
}
/*
 * 功能描述: NDIS停止周期性ARP定时器
 * 修改历史:
 *  1.日    期: 2012年4月19日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_StopARPTimer(NDIS_ArpPeriodTimer *arpPeriodTimer)
{
    if (arpPeriodTimer->tm != VOS_NULL_PTR) {
        if (VOS_StopRelTimer(&(arpPeriodTimer->tm)) != VOS_OK) {
            NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_StopARPTimer, VOS_StopRelTimer fail!");
        }
        arpPeriodTimer->tm = VOS_NULL_PTR;
    }

    return;
}

/*
 * 功能描述: 处理TmerMsg
 * 1.日    期: 2012年4月28日
 *   修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_ProcARPTimerExp(VOS_VOID)
{
    VOS_UINT32                  loop;
    NDIS_Entity           *ndisEntity     = VOS_NULL_PTR;
    NDIS_Ipv4Info        *ipV4Info       = VOS_NULL_PTR;
    NDIS_ArpPeriodTimer *arpPeriodTimer = VOS_NULL_PTR;

    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        ndisEntity = &g_ndisEntity[loop];
        if ((ndisEntity->rabType & NDIS_ENTITY_IPV4) == NDIS_ENTITY_IPV4) {
            ipV4Info       = &(ndisEntity->ipV4Info);
            arpPeriodTimer = &(ipV4Info->arpPeriodTimer);

            if (ipV4Info->arpInitFlg == PS_TRUE) {
#if (VOS_OS_VER != VOS_WIN32)
                Ndis_StopARPTimer(arpPeriodTimer);
#endif
            } else {
                (VOS_VOID)Ndis_SendRequestArp(&(ndisEntity->ipV4Info), ndisEntity->rabId);
            }
        }
    }

    return;
}

/*
 * 功能描述: DHCP处理
 * 修改历史:
 *  1.日    期: 2011年2月11日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_DHCPPkt_Proc(const MsgBlock *msgBlock)
{
    VOS_UINT8              *data       = VOS_NULL_PTR;
    NDIS_Entity            *ndisEntity = VOS_NULL_PTR;
    VOS_UINT8               exRabId;
    VOS_UINT32              pktMemLen;

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
    CDS_NDIS_DataInd       *rcvMsg = (CDS_NDIS_DataInd *)(VOS_VOID *)msgBlock;
    data      = rcvMsg->data;
    pktMemLen = rcvMsg->len;
    exRabId   = rcvMsg->ifaceId;

#else
    ADS_NDIS_DataInd       *rcvMsg = (ADS_NDIS_DataInd *)(VOS_VOID *)msgBlock;

    data = IMM_ZcGetDataPtr(rcvMsg->data);
    if (data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DHCPPkt_Proc, IMM_ZcGetDataPtr fail!");
        return;
    }
    pktMemLen = IMM_ZcGetUsedLen(rcvMsg->data);
    exRabId = NDIS_FORM_EXBID(rcvMsg->modemId, rcvMsg->rabId);
#endif
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DHCPPkt_Proc, Ndis_ChkRabIdValid fail!");
        NDIS_PRINT_ERROR(Ndis_DHCPPkt_Proc_ENUM, IP_COM_CHECK_ERROR);
        return;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DHCPPkt_Proc, NDIS_GetEntityByRabId fail!");
        NDIS_PRINT_ERROR1(Ndis_DHCPPkt_Proc_ENUM, IP_COM_GET_ENTITY_FAIL, exRabId);
        return;
    }

    if ((ndisEntity->rabType & NDIS_ENTITY_IPV4) != NDIS_ENTITY_IPV4) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_DHCPPkt_Proc, Rab not support IPV4!");
        NDIS_PRINT_ERROR1(Ndis_DHCPPkt_Proc_ENUM, IP_COM_CHECK_ERROR, ndisEntity->rabType);
        return;
    }

    /* DHCP处理 */
    NDIS_STAT_UL_RECV_DHCPPKT(1);
    IPV4_DHCP_ProcDhcpPkt(data, exRabId, pktMemLen);

    return;
}

/*
 * 功能描述: 处理TmerMsg
 * 修改历史:
 *  1.日    期: 2012年4月28日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_ProcTmrMsg(const REL_TimerMsgBlock *rcvMsg)
{
    if (sizeof(REL_TimerMsgBlock) - VOS_MSG_HEAD_LENGTH > rcvMsg->ulLength) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_ProcTmrMsg, input msg length less than struc", rcvMsg->ulLength);
        return;
    }

    switch (rcvMsg->name) {
        case NDIS_PERIOD_ARP_TMRNAME:
            Ndis_ProcARPTimerExp();
            break;
        default:
            NDIS_INFO_LOG1(NDIS_TASK_PID, "Ndis_ProcTmrMsg, Recv other Timer", rcvMsg->name);
            break;
    }

    return;
}

/*
 * 功能描述: APP NDIS下行PID初始化函数
 * 修改历史:
 *  1.日    期: 2011年2月15日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 APP_Ndis_Pid_InitFunc(enum VOS_InitPhaseDefine phase)
{
    VOS_UINT32 result;

    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            result = Ndis_Init();
            if (result != PS_SUCC) {
                PS_PRINTF_ERR("APP_Ndis_Pid_InitFunc, Ndis_Init fail!\n");
                return VOS_ERR;
            }
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
            /* 给底软注册回调函数，用于C核单独复位的处理 */
            (VOS_VOID)mdrv_sysboot_register_reset_notify(PS_NDIS_FUNC_PROC_NAME, Ndis_ModemResetCb, 0,
                                                         RESET_PRIORITY_NDIS);
#endif
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

    return VOS_OK;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
/*
 * 功能描述: 处理底软发送上来的ARP帧
 * 修改历史:
 *  1.日    期: 2009年12月31日
 *    修改内容: 新生成函数
 */
VOS_UINT32 Ndis_SendMacFrm(const VOS_UINT8 *buf, VOS_UINT32 len, VOS_UINT8 exRabId)
{
    NDIS_Mem          pktNode = {0};
    VOS_UINT64        addr;
    VOS_INT32         lRtn;
    VOS_INT32         handle;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;
    errno_t           rlt;
    if (buf == VOS_NULL_PTR) {
        NDIS_PRINT_ERROR1(Ndis_SendMacFrm_ENUM, IP_COM_INPUT_PARA_ERROR, VOS_NULL_PTR);
        return PS_FAIL;
    }

    addr = NDIS_MAA_MEM_ALLOC(len);
    if (addr == VOS_NULL_PTR) {
        NDIS_PRINT_ERROR(Ndis_SendMacFrm_ENUM, IP_COM_ALLOC_MEM_FAIL);
        return PS_FAIL;
    }
    pktNode.maa_addr = addr;
    pktNode.size = len;
    rlt = memcpy_s((VOS_VOID *)(VOS_UINT_PTR)(pktNode.maa_addr & 0xFFFFFFFF), len, buf, len);
    if (rlt != EOK) {
        NDIS_MemFree(&pktNode);
        return PS_FAIL;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_MemFree(&pktNode);
        NDIS_PRINT_ERROR1(Ndis_SendMacFrm_ENUM, IP_COM_GET_ENTITY_FAIL, exRabId);
        return PS_FAIL;
    }

    handle = ndisEntity->handle;

    /* 数据发送 */
    lRtn = mdrv_vcom_ioctl(handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&pktNode);
    if (lRtn != 0) {
        NDIS_MemFree(&pktNode);
        NDIS_PRINT_ERROR2(Ndis_SendMacFrm_ENUM, IP_COM_SEND_DATA_FAIL, len, lRtn);
        NDIS_STAT_DL_SEND_ARPDHCPPKT_FAIL(1);
        return PS_FAIL;
    }
    return PS_SUCC;
}

#else
/*
 * 功能描述: 处理底软发送上来的ARP帧
 * 修改历史:
 *  1.日    期: 2009年12月31日
 *    修改内容: 新生成函数
 */
VOS_UINT32 Ndis_SendMacFrm(const VOS_UINT8 *buf, VOS_UINT32 len, VOS_UINT8 exRabId)
{
    IMM_Zc           *immZc = VOS_NULL_PTR;
    VOS_INT32         lRtn;
    VOS_INT32         handle;
    VOS_UINT8        *pdata       = VOS_NULL_PTR;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;
    errno_t           rlt;
    if (buf == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_SendMacFrm, pucBuf is NULL!");
        return PS_FAIL;
    }

    immZc = IMM_ZcStaticAlloc(len);
    if (immZc == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_SendMacFrm, IMM_ZcStaticAlloc fail!");
        return PS_FAIL;
    }

    pdata = (VOS_UINT8 *)IMM_ZcPut(immZc, len);
    if (pdata == VOS_NULL_PTR) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_SendMacFrm, IMM_ZcPut fail!");
        return PS_FAIL;
    }

    rlt = memcpy_s(pdata, len, buf, len);
    if (rlt != EOK) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        return PS_FAIL;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_SendMacFrm, NDIS_GetEntityByRabId fail!");
        return PS_FAIL;
    }

    handle = ndisEntity->handle;

    /* 数据发送 */
    lRtn = NDIS_UDI_WRITE(handle, immZc, len);
    if (lRtn != 0) {
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_SendMacFrm, udi_write fail!");
        NDIS_STAT_DL_SEND_ARPDHCPPKT_FAIL(1);
        return PS_FAIL;
    }
    return PS_SUCC;
}
#endif

/*
 * 功能描述: IPV4 PDN信息配置
 * 修改历史:
 *  1.日    期: 2011年3月15日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_Ipv4PdnInfoCfg(const AT_NDIS_Ipv4PdnInfo *nasNdisInfo, NDIS_Entity *ndisEntity)
{
    NDIS_Ipv4Info         *ipV4Info    = &(ndisEntity->ipV4Info);
    errno_t                rlt;

    ipV4Info->arpInitFlg = PS_FALSE;
    ipV4Info->arpRepFlg  = PS_FALSE;

    rlt = memcpy_s(ipV4Info->ueIpInfo.ipAddr8bit, IPV4_ADDR_LEN, nasNdisInfo->PdnAddrInfo.ipv4AddrU8,
                   IPV4_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    rlt = memcpy_s(ipV4Info->gwIpInfo.ipAddr8bit, IPV4_ADDR_LEN, nasNdisInfo->gateWayAddrInfo.ipv4AddrU8,
                   IPV4_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    rlt = memcpy_s(ipV4Info->nmIpInfo.ipAddr8bit, IPV4_ADDR_LEN, nasNdisInfo->subnetMask.ipv4AddrU8,
                   IPV4_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    /* 使能则配置DNS */
    if (nasNdisInfo->opDnsPrim == PS_TRUE) {
        rlt = memcpy_s(ipV4Info->primDnsAddr.ipAddr8bit, IPV4_ADDR_LEN,
                       nasNdisInfo->dnsPrimAddrInfo.ipv4AddrU8, IPV4_ADDR_LEN);
        NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    } else {
        ipV4Info->primDnsAddr.ipAddr32bit = 0;
    }

    /* 使能则配置辅DNS */
    if (nasNdisInfo->opDnsSec == PS_TRUE) {
        rlt = memcpy_s(ipV4Info->secDnsAddr.ipAddr8bit, IPV4_ADDR_LEN,
                       nasNdisInfo->dnsSecAddrInfo.ipv4AddrU8, IPV4_ADDR_LEN);
        NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    } else {
        ipV4Info->secDnsAddr.ipAddr32bit = 0;
    }

    /* 使能则配置主WINS */
    if (nasNdisInfo->opWinsPrim == PS_TRUE) {
        rlt = memcpy_s(ipV4Info->primWinsAddr.ipAddr8bit, IPV4_ADDR_LEN,
                       nasNdisInfo->winsPrimAddrInfo.ipv4AddrU8, IPV4_ADDR_LEN);
        NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    } else {
        ipV4Info->primWinsAddr.ipAddr32bit = 0;
    }

    /* 使能则配置辅WINS */
    if (nasNdisInfo->opWinsSec == PS_TRUE) {
        rlt = memcpy_s(ipV4Info->secWinsAddr.ipAddr8bit, IPV4_ADDR_LEN,
                       nasNdisInfo->winsSecAddrInfo.ipv4AddrU8, IPV4_ADDR_LEN);
        NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    } else {
        ipV4Info->secWinsAddr.ipAddr32bit = 0;
    }

    /* PCSCF暂时不操作,待需求描述 */
    ipV4Info->ipAssignStatus = IPV4_DHCP_ADDR_STATUS_FREE;

    return;
}

/*
 * 功能描述: NDIS启动周期性ARP定时器
 * 修改历史:
 *  1.日    期: 2012年4月19日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_StartARPTimer(NDIS_Entity *ndisEntity)
{
#if (VOS_OS_VER != VOS_WIN32)
    VOS_UINT32                  rtn;
    NDIS_ArpPeriodTimer        *arpPeriodTimer = VOS_NULL_PTR;

    /* 入参指针判断 */
    if (ndisEntity == VOS_NULL_PTR) {
        return PS_FAIL;
    }

    /* 检查是否需要启动ARP定时器 */
    if (((ndisEntity->rabType & NDIS_ENTITY_IPV4) != NDIS_ENTITY_IPV4) ||
        (ndisEntity->ipV4Info.arpInitFlg == PS_TRUE)) {
        return PS_SUCC;
    }

    arpPeriodTimer = &(ndisEntity->ipV4Info.arpPeriodTimer);

    /* 如果还在运行，则停掉 */
    if (arpPeriodTimer->tm != VOS_NULL_PTR) {
        Ndis_StopARPTimer(arpPeriodTimer);
    }

    rtn = VOS_StartRelTimer(&(arpPeriodTimer->tm), PS_PID_APP_NDIS, arpPeriodTimer->timerValue,
                            arpPeriodTimer->name, 0, VOS_RELTIMER_LOOP, VOS_TIMER_PRECISION_0);
    if (rtn != VOS_OK) {
        arpPeriodTimer->tm = VOS_NULL_PTR;
        return PS_FAIL;
    }
#endif

    return PS_SUCC;
}

/*
 * 功能描述: 根据ExRabId查找NDIS实体
 * 修改历史:
 *  1.日    期: 2013年1月15日
 *    修改内容: 新生成函数
 */
NDIS_Entity *NDIS_GetEntityByRabId(VOS_UINT8 exRabId)
{
    VOS_UINT16 i = 0;

    /* 查询是否已存在相应Entity */
    do {
        if ((g_ndisEntity[i].used == PS_TRUE) && (exRabId == g_ndisEntity[i].rabId)) {
            /* 找到相应实体 */
            return &g_ndisEntity[i];
        }
    } while ((++i) < NAS_NDIS_MAX_ITEM);

    return VOS_NULL_PTR;
}

/*
 * 功能描述: 分配一个空闲的NDIS实体
 * 修改历史:
 *  1.日    期: 2013年1月15日
 *    修改内容: 新生成函数
 */
STATIC NDIS_Entity *NDIS_AllocEntity(VOS_VOID)
{
    VOS_UINT16 i = 0;

    /* 返回第一个空闲的实体 */
    do {
        if (g_ndisEntity[i].used == PS_FALSE) {
            /* 找到空闲实体 */
            return &g_ndisEntity[i];
        }
    } while ((++i) < NAS_NDIS_MAX_ITEM);

    return VOS_NULL_PTR;
}

/*
 * 功能描述: PDN IPV4地址信息检查
 * 修改历史:
 *  1.日    期: 2011年12月11日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_CheckIpv4PdnInfo(const AT_NDIS_Ipv4PdnInfo *ipv4PdnInfo)
{
    if (ipv4PdnInfo->opPdnAddr == PS_FALSE) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_CheckIpv4PdnInfo,  pstIpv4PdnInfo->bitOpPdnAddr is false!");
        NDIS_PRINT_ERROR1(Ndis_CheckIpv4PdnInfo_ENUM, IP_COM_INPUT_PARA_ERROR, ipv4PdnInfo->opPdnAddr);
        return PS_FAIL;
    }

    /* PDN地址和网关地址如果为全0，则也失败 */
    if (VOS_MemCmp(ipv4PdnInfo->PdnAddrInfo.ipv4AddrU8, g_invalidAddr, IPV4_ADDR_LEN) == 0) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_CheckIpv4PdnInfo,  stPDNAddrInfo all zero!");
        NDIS_PRINT_ERROR(Ndis_CheckIpv4PdnInfo_ENUM, IP_COM_CHECK_ERROR);
        return PS_FAIL;
    }

    if (VOS_MemCmp(ipv4PdnInfo->gateWayAddrInfo.ipv4AddrU8, g_invalidAddr, IPV4_ADDR_LEN) == 0) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_CheckIpv4PdnInfo,  stGateWayAddrInfo all zero!");
        NDIS_PRINT_ERROR1(Ndis_CheckIpv4PdnInfo_ENUM, IP_COM_CHECK_ERROR, (VOS_INT32)(VOS_INT_PTR)g_invalidAddr);
        return PS_FAIL;
    }

    return PS_SUCC;
}

/*
 * 功能描述: 检查参数配置参数是否合法
 * 修改历史:
 *  1.日    期: 2011年12月23日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_PdnV4PdnCfg(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo, NDIS_Entity *ndisEntity)
{
    /* IPV4地址检查 */
    if (nasNdisInfo->opIpv4PdnInfo == PS_FALSE) { /* 原语指示IPV4信息无效 */
        NDIS_INFO_LOG(NDIS_TASK_PID, "Ndis_PdnV4PdnCfg,  bitOpIpv4PdnInfo is false!");
        NDIS_PRINT_INFO1(Ndis_PdnV4PdnCfg_ENUM, IP_COM_RECV_MSG_INFO, nasNdisInfo->opIpv4PdnInfo);
        return PS_FAIL;
    }

    if (Ndis_CheckIpv4PdnInfo(&(nasNdisInfo->ipv4PdnInfo)) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnV4PdnCfg,  Ndis_CheckIpv4PdnInfo fail!");
        NDIS_PRINT_ERROR(Ndis_PdnV4PdnCfg_ENUM, IP_COM_CHECK_ERROR);
        return PS_FAIL;
    }

    Ndis_Ipv4PdnInfoCfg(&(nasNdisInfo->ipv4PdnInfo), ndisEntity);

    /* 更新NDIS实体承载属性 */
    ndisEntity->rabType |= NDIS_ENTITY_IPV4;

    return PS_SUCC;
}

/*
 * 功能描述: IPV6 PDN信息配置
 * 修改历史:
 *  1.日    期: 2011年12月23日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT32 Ndis_PdnV6PdnCfg(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo, NDIS_Entity *ndisEntity)
{
    VOS_UINT8 exRabId;

    /* IPV6地址检查 */
    if (nasNdisInfo->opIpv6PdnInfo == PS_FALSE) { /* 原语指示IPV6信息无效 */
        NDIS_INFO_LOG(NDIS_TASK_PID, "Ndis_PdnV6PdnCfg,  bitOpIpv6PdnInfo is false!");
        NDIS_PRINT_INFO1(Ndis_PdnV6PdnCfg_ENUM, IP_COM_RECV_MSG_INFO, nasNdisInfo->opIpv6PdnInfo);
        return PS_FAIL;
    }

    exRabId = NDIS_FORM_EXBID(nasNdisInfo->modemId, nasNdisInfo->rabId);

    if (NdSer_CheckIpv6PdnInfo(&(nasNdisInfo->ipv6PdnInfo)) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnV6PdnCfg,  NdSer_CheckIpv6PdnInfo fail!");
        NDIS_PRINT_ERROR(Ndis_PdnV6PdnCfg_ENUM, IP_COM_CHECK_ERROR);
        return PS_FAIL;
    }

    /* 调ND SERVER API  配置IPV6地址信息给ND SERVER */
    NdSer_Ipv6PdnInfoCfg(exRabId, &(nasNdisInfo->ipv6PdnInfo));

    /* 更新NDIS实体属性 */
    ndisEntity->rabType |= NDIS_ENTITY_IPV6;

    return PS_SUCC;
}

/*
 * 功能描述: NDIS向AT返回的配置确认结果处理
 * 修改历史:
 *  1.日    期: 2012年4月25日
 *    修改内容: 新生成函数
 */
STATIC VOS_UINT8 Ndis_AtCnfResultProc(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo, VOS_UINT32 v4Ret,
                                      VOS_UINT32 v6Ret)
{
    VOS_UINT8 result;

    /* 根据配置结果向AT返回配置CNF原语 */
    if ((nasNdisInfo->opIpv4PdnInfo == PS_TRUE) && (nasNdisInfo->opIpv6PdnInfo == PS_TRUE)) {
        if ((v4Ret == PS_SUCC) && (v6Ret == PS_SUCC)) { /* IPV4和IPV6配置都成功 */
            result = AT_NDIS_PDNCFG_CNF_SUCC;
        } else if (v4Ret == PS_SUCC) { /* 只有IPV4配置成功 */
            result = AT_NDIS_PDNCFG_CNF_IPV4ONLY_SUCC;
        } else { /* 只有IPV6配置成功 */
            result = AT_NDIS_PDNCFG_CNF_IPV6ONLY_SUCC;
        }
    } else if (nasNdisInfo->opIpv4PdnInfo == PS_TRUE) { /* 只配置了IPV4 */
        result = AT_NDIS_PDNCFG_CNF_IPV4ONLY_SUCC;
    } else { /* 只配置了IPV6 */
        result = AT_NDIS_PDNCFG_CNF_IPV6ONLY_SUCC;
    }

    return result;
}
STATIC VOS_UINT32 Ndis_PdnInfoCfgSpeProc(NDIS_Entity *ndisEntity)
{
        ndisEntity->spePort = NDIS_INVALID_SPEPORT;
    return IP_SUCC;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
STATIC VOS_UINT32 Ndis_PdnJugeV4V6ConfigIndicate(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo, NDIS_Entity *ndisEntity,
                                                 VOS_UINT32 *v4Ret, VOS_UINT32 *v6Ret, AT_NDIS_PdnInfoCfgCnf cfgCnf)
{
    *v4Ret = Ndis_PdnV4PdnCfg(nasNdisInfo, ndisEntity);
    *v6Ret = Ndis_PdnV6PdnCfg(nasNdisInfo, ndisEntity);

    if ((*v6Ret == PS_FAIL) && (*v4Ret == PS_FAIL)) { /* 如果IPV4和IPV6配置指示信息都无效，也认为配置失败 */
        /* 向AT回复PDN配置失败 */
        (VOS_VOID)Ndis_SndMsgToAt((VOS_UINT8 *)&cfgCnf, sizeof(AT_NDIS_PdnInfoCfgCnf),
                                  ID_AT_NDIS_PDNINFO_CFG_CNF);
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnJugeV4V6ConfigIndicate,  Ipv4 and Ipv6 Cfg all fail!");
        return PS_FAIL;
    }
    return PS_SUCC;
}
STATIC VOS_UINT32 NdisCfgParaCheck(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo, VOS_UINT8 exRabId,
                                   AT_NDIS_PdnInfoCfgCnf cfgCnf)
{
    /* ExRabId参数范围有效性检查。若检查失败，则直接向AT回复配置失败 */
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        (VOS_VOID)Ndis_SndMsgToAt((VOS_UINT8 *)&cfgCnf, sizeof(AT_NDIS_PdnInfoCfgCnf), ID_AT_NDIS_PDNINFO_CFG_CNF);
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnInfoCfgProc,  Ndis_ChkRabIdValid fail!");
        return PS_FAIL;
    }
    return PS_SUCC;
}
/*
 * 功能描述: PDN地址信息参数配置
 */
STATIC VOS_VOID Ndis_PdnInfoCfgProc(const AT_NDIS_PdnInfoCfgReq *nasNdisInfo)
{
    VOS_UINT8                   exRabId = NDIS_FORM_EXBID(nasNdisInfo->modemId, nasNdisInfo->rabId);
    NDIS_Entity                *ndisEntity = VOS_NULL_PTR;
    AT_NDIS_PdnInfoCfgCnf       cfgCnf     = {0};
    VOS_UINT32                  v4Ret;
    VOS_UINT32                  v6Ret;

    cfgCnf.result  = AT_NDIS_PDNCFG_CNF_FAIL;
    cfgCnf.rabId   = nasNdisInfo->rabId;
    cfgCnf.modemId = nasNdisInfo->modemId;
    if (NdisCfgParaCheck(nasNdisInfo, exRabId, cfgCnf) != PS_SUCC) {
        return;
    }

    /* 如果根据ExRabId查找不到NDIS实体，则分配一个空闲的NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        /* 如果分配不到空闲的NDIS实体，则返回 */
        ndisEntity = NDIS_AllocEntity();
        if (ndisEntity == VOS_NULL_PTR) {
            /* 向AT回复PDN配置失败 */
            (VOS_VOID)Ndis_SndMsgToAt((VOS_UINT8 *)&cfgCnf, sizeof(AT_NDIS_PdnInfoCfgCnf), ID_AT_NDIS_PDNINFO_CFG_CNF);
            NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnInfoCfgProc,  NDIS_AllocEntity failed!");
            return;
        }

        /* 该承载之前没有对应的NDIS实体，故填无效值 */
        NDIS_SET_INITIALSETUP_NDISENTITY_INFO(ndisEntity);
    }

    if (Ndis_PdnJugeV4V6ConfigIndicate(nasNdisInfo, ndisEntity, &v4Ret, &v6Ret, cfgCnf) == PS_FAIL) {
        return;
    }
    NDIS_SET_EXIST_NDIS_INFO(ndisEntity, exRabId, nasNdisInfo->handle, nasNdisInfo->iSpePort, nasNdisInfo->ipfFlag);

    cfgCnf.result  = Ndis_AtCnfResultProc(nasNdisInfo, v4Ret, v6Ret);
    cfgCnf.rabType = ndisEntity->rabType;

    /* 启动周期发送ARP的定时器 */
    if (Ndis_StartARPTimer(ndisEntity) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_ConfigArpInfo StartTmr Failed!");
        return;
    }

    if (Ndis_PdnInfoCfgSpeProc(ndisEntity) == IP_FAIL) {
        return;
    }

    /* 向ADS注册下行回调:只注册一次 */
    if (ADS_DL_RegDlDataCallback(exRabId, Ndis_DlAdsDataRcv, 0) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnInfoCfgProc, ADS_DL_RegDlDataCallback fail!");
        return;
    }

    /*lint -e718*/
    if (NDIS_UDI_IOCTL(ndisEntity->handle, NCM_IOCTL_REG_UPLINK_RX_FUNC, AppNdis_UsbReadCb) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnInfoCfgProc, regist AppNdis_UsbReadCb fail!");
        return;
    }
    /*lint +e718*/

    if (Ndis_SndMsgToAt((VOS_UINT8 *)&cfgCnf, sizeof(AT_NDIS_PdnInfoCfgCnf), ID_AT_NDIS_PDNINFO_CFG_CNF) == PS_FAIL) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnInfoCfgProc, send msg to at fail!");
    }

    return;
}

/*
 * 功能描述: PDN释放
 * 修改历史:
 *  1.日    期: 2011年12月7日
 *    修改内容: 用户面融合修改
 *  1.日    期: 2013年1月15日
 *    修改内容: DSDA特性开发:
 */
STATIC VOS_VOID Ndis_PdnRel(const AT_NDIS_PdnInfoRelReq *nasNdisRel)
{
    VOS_UINT8                    exRabId;
    NDIS_Entity                 *ndisEntity     = VOS_NULL_PTR;
    NDIS_ArpPeriodTimer         *arpPeriodTimer = VOS_NULL_PTR;
    AT_NDIS_PdnInfoRelCnf        relCnf;

    NDIS_INFO_LOG(NDIS_TASK_PID, "Ndis_PdnRel entered!");
    relCnf.result  = AT_NDIS_FAIL;
    relCnf.rabId   = nasNdisRel->rabId;
    relCnf.modemId = nasNdisRel->modemId;

    exRabId = NDIS_FORM_EXBID(nasNdisRel->modemId, nasNdisRel->rabId);
    if (Ndis_ChkRabIdValid(exRabId) == PS_FAIL) {
        (VOS_VOID)Ndis_SndMsgToAt((VOS_UINT8 *)&relCnf, sizeof(AT_NDIS_PdnInfoRelCnf),
                                  ID_AT_NDIS_PDNINFO_REL_CNF);
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnRel,  Ndis_ChkRabIdValid fail!");
        return;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        (VOS_VOID)Ndis_SndMsgToAt((VOS_UINT8 *)&relCnf, sizeof(AT_NDIS_PdnInfoRelCnf),
                                  ID_AT_NDIS_PDNINFO_REL_CNF);
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnRel,  NDIS_GetEntityByRabId failed!");
        return;
    }

    arpPeriodTimer = &(ndisEntity->ipV4Info.arpPeriodTimer);

    relCnf.rabType = ndisEntity->rabType;

    /* 如果周期性ARP定时器还在运行，则停掉 */
    Ndis_StopARPTimer(arpPeriodTimer);

    /* 调用ND SERVER API 释放该RabId对应ND SERVER实体 */
    if ((ndisEntity->rabType & NDIS_ENTITY_IPV6) == NDIS_ENTITY_IPV6) {
        NdSer_Ipv6PdnRel(exRabId);
    }

    /* 更新该RabId对应NDIS实体为空 */
    ndisEntity->rabType    = NDIS_RAB_NULL;
    ndisEntity->rabId      = NDIS_INVALID_RABID;
    ndisEntity->handle     = NDIS_INVALID_HANDLE;
    ndisEntity->used       = PS_FALSE;
    ndisEntity->spePort     = NDIS_INVALID_SPEPORT;
    ndisEntity->speIpfFlag = PS_FALSE;

    /* NDIS向AT回复释放确认原语 */
    relCnf.result = AT_NDIS_SUCC;
    if (Ndis_SndMsgToAt((VOS_UINT8 *)&relCnf, sizeof(AT_NDIS_PdnInfoRelCnf), ID_AT_NDIS_PDNINFO_REL_CNF) == PS_FAIL) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_PdnRel, ndis send msg to at fail!");
    }

    return;
}
#endif
/*
 * 功能描述: IFACE UP CFG消息内容转成PND INFO CFG消息内容
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新增
 */
STATIC VOS_VOID Ndis_ATIfaceUpCfgTransToPdnInfoCfg(const AT_NDIS_IfaceUpConfigInd *ndisIFaceInfo,
                                                   AT_NDIS_PdnInfoCfgReq *ndisPdnInfo)
{
    errno_t rlt;
    (VOS_VOID)memset_s((VOS_UINT8 *)ndisPdnInfo, sizeof(AT_NDIS_PdnInfoCfgReq), 0,
                       sizeof(AT_NDIS_PdnInfoCfgReq));

    ndisPdnInfo->msgId          = ndisIFaceInfo->msgId;
    ndisPdnInfo->opIpv4PdnInfo = ndisIFaceInfo->opIpv4PdnInfo;
    ndisPdnInfo->opIpv6PdnInfo = ndisIFaceInfo->opIpv6PdnInfo;
    ndisPdnInfo->modemId        = NDIS_GET_MODEMID_FROM_EXBID(ndisIFaceInfo->iFaceId); /* 将Iface ID转成ModemId+RabId */
    ndisPdnInfo->rabId = NDIS_GET_BID_FROM_EXBID(ndisIFaceInfo->iFaceId); /* 将Iface ID转成ModemId+RabId */
    ndisPdnInfo->handle = ndisIFaceInfo->handle;
    rlt = memcpy_s(&ndisPdnInfo->ipv4PdnInfo, sizeof(AT_NDIS_Ipv4PdnInfo),
                   &ndisIFaceInfo->ipv4PdnInfo, sizeof(AT_NDIS_Ipv4PdnInfo));
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    rlt = memcpy_s(&ndisPdnInfo->ipv6PdnInfo, sizeof(AT_NDIS_Ipv6PdnInfo),
                   &ndisIFaceInfo->ipv6PdnInfo, sizeof(AT_NDIS_Ipv6PdnInfo));
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    return;
}
STATIC VOS_UINT32 Ndis_IfaceJugeV4V6ConfigIndicate(AT_NDIS_PdnInfoCfgReq *pdnInfo, NDIS_Entity *ndisEntity)
{
    VOS_UINT32                    v4Ret;
    VOS_UINT32                    v6Ret;
    v4Ret = Ndis_PdnV4PdnCfg(pdnInfo, ndisEntity);
    v6Ret = Ndis_PdnV6PdnCfg(pdnInfo, ndisEntity);
    if ((v6Ret == PS_FAIL) && (v4Ret == PS_FAIL)) { /* 如果IPV4和IPV6配置指示信息都无效，也认为配置失败 */
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_IfaceJugeV4V6ConfigIndicate,  Ipv4 and Ipv6 Cfg all fail!");
        NDIS_PRINT_ERROR(Ndis_IfaceJugeV4V6ConfigIndicate_ENUM, IP_COM_ABNORMAL_SITUATION_ERROR);
        return PS_FAIL;
    }
    return PS_SUCC;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)

/*
 * 功能描述: 生成MODEM PDUSESSION VCOM配置信息
 */
STATIC VOS_VOID Ndis_TransVcomPdnInfoStru(const AT_NDIS_IfaceUpConfigInd *ndisIFaceInfo,
                                          struct pduinfo_cfg *vcomPdnInfo)
{
    VOS_UINT8     cnt = 0;

    (VOS_VOID)memset_s((VOS_UINT8 *)vcomPdnInfo, sizeof(struct pduinfo_cfg), 0, sizeof(struct pduinfo_cfg));
    vcomPdnInfo->handle  = (VOS_UINT32)ndisIFaceInfo->handle;
    vcomPdnInfo->iface_id = ndisIFaceInfo->iFaceId;
    vcomPdnInfo->operation_type = NDIS_IFACE_UP;
    if (ndisIFaceInfo->opIpv4PdnInfo == PS_TRUE) {
        vcomPdnInfo->pdu_list[cnt].ip_ver = IP_VERSION_V4;
        vcomPdnInfo->pdu_list[cnt].pdusession_id = ndisIFaceInfo->ipv4IfaceInfo.pduSessionId;
        vcomPdnInfo->modem_id = ndisIFaceInfo->ipv4IfaceInfo.modemId;
        cnt++;
    }

    if (ndisIFaceInfo->opIpv6PdnInfo == PS_TRUE) {
        vcomPdnInfo->pdu_list[cnt].ip_ver = IP_VERSION_V6;
        vcomPdnInfo->pdu_list[cnt].pdusession_id = ndisIFaceInfo->ipv6IfaceInfo.pduSessionId;
        vcomPdnInfo->modem_id = ndisIFaceInfo->ipv4IfaceInfo.modemId;
        cnt++;
    }

    vcomPdnInfo->pdu_num = cnt;
    return;
}

STATIC VOS_UINT32 Ndis_RegisterCbProc(const AT_NDIS_IfaceUpConfigInd *ifacInfo, NDIS_Entity *ndisEntity)
{
    struct pduinfo_cfg            vcomPdnInfo = {0};

    /* 向底软注册用户数据 */
    if (mdrv_vcom_ioctl(ndisEntity->handle, VCOM_IOCTL_SET_USER_DATA, (VOS_VOID *)&ndisEntity->handle) != VOS_OK) {
        NDIS_PRINT_ERROR1(Ndis_RegisterCbProc_ENUM, IP_COM_REGISTER_CALLBACK_FAIL, VCOM_IOCTL_SET_USER_DATA);
        return PS_FAIL;
    }

    /* 向底软注册上行回调 */
    if (mdrv_vcom_ioctl(ndisEntity->handle, VCOM_IOCTL_SET_READ_CB, NDIS_VcomReadCb) != VOS_OK) {
        NDIS_PRINT_ERROR1(Ndis_RegisterCbProc_ENUM, IP_COM_REGISTER_CALLBACK_FAIL, VCOM_IOCTL_SET_READ_CB);
        return PS_FAIL;
    }

    /* 向底软注册下行写回调 */
    if (mdrv_vcom_ioctl(ndisEntity->handle, VCOM_IOCTL_SET_WRITE_CB, NDIS_VcomWriteCb) != VOS_OK) {
        NDIS_PRINT_ERROR1(Ndis_RegisterCbProc_ENUM, IP_COM_REGISTER_CALLBACK_FAIL, VCOM_IOCTL_SET_WRITE_CB);
        return PS_FAIL;
    }

    /* 向底软注册MODEM PDUSESSION信息 */
    Ndis_TransVcomPdnInfoStru(ifacInfo, &vcomPdnInfo);
    if (mdrv_vcom_ioctl(ndisEntity->handle, VCOM_IOCTL_PDUINFO_CFG, (VOS_VOID *)(&vcomPdnInfo)) != VOS_OK) {
        NDIS_PRINT_ERROR1(Ndis_RegisterCbProc_ENUM, IP_COM_REGISTER_CALLBACK_FAIL, VCOM_IOCTL_PDUINFO_CFG);
        return PS_FAIL;
    }
    return PS_SUCC;
}

STATIC VOS_INT32 Ndis_FindHandleByIface(VOS_INT8 ifaceId)
{
    VOS_UINT32        loop;
    NDIS_Entity      *ndisEntity = VOS_NULL_PTR;

    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        ndisEntity = &g_ndisEntity[loop];

        if ((ndisEntity->rabId == ifaceId) &&
            (ndisEntity->used == PS_TRUE)) {
            return ndisEntity->handle;
        }
    }

    return NDIS_INVALID_HANDLE;
}

STATIC VOS_VOID Ndis_IfaceUpCfgProc(const AT_NDIS_IfaceUpConfigInd *ifacInfo)
{
    VOS_UINT8                     exRabId;
    VOS_INT32                     handle;
    NDIS_Entity                  *ndisEntity = VOS_NULL_PTR;
    AT_NDIS_PdnInfoCfgReq         pdnInfo;

    exRabId = ifacInfo->iFaceId;

    /* ExRabId参数范围有效性检查。若检查失败，则直接向AT回复配置失败 */
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_PRINT_ERROR1(Ndis_IfaceUpCfgProc_ENUM, IP_COM_INPUT_PARA_ERROR, exRabId);
        return;
    }

    /* 如果根据ExRabId查找不到NDIS实体，则分配一个空闲的NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        /* 如果分配不到空闲的NDIS实体，则返回 */
        ndisEntity = NDIS_AllocEntity();
        if (ndisEntity == VOS_NULL_PTR) {
            NDIS_PRINT_ERROR(Ndis_IfaceUpCfgProc_ENUM, IP_COM_NULL_PTR);
            return;
        }

        /* 该承载之前没有对应的NDIS实体，故填无效值 */
        NDIS_SET_INITIALSETUP_NDISENTITY_INFO(ndisEntity);
    }

    Ndis_ATIfaceUpCfgTransToPdnInfoCfg(ifacInfo, &pdnInfo);
    if (Ndis_IfaceJugeV4V6ConfigIndicate(&pdnInfo, ndisEntity) == PS_FAIL) {
        return;
    }

    handle  = ifacInfo->handle;
    NDIS_SET_EXIST_NDIS_INFO(ndisEntity, exRabId, handle, 0, 0);
    /* 启动周期发送ARP的定时器 */
    if (Ndis_StartARPTimer(ndisEntity) != PS_SUCC) {
        NDIS_PRINT_ERROR(Ndis_IfaceUpCfgProc_ENUM, IP_COM_START_TIMER_FAIL);
        return;
    }

    ndisEntity->spePort = NDIS_INVALID_SPEPORT;

    if (Ndis_RegisterCbProc(ifacInfo, ndisEntity) != PS_SUCC) {
        return;
    }

    /* 配置ESPE MAC信息 */
    mdrv_wan_set_ipfmac((VOS_VOID *)&g_speMacHeader);
    return;
}

/*
 * 功能描述: IFACE DOWN消息处理
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_IfaceDownCfgProc(const AT_NDIS_IfaceDownConfigInd *ifacInfo)
{
    VOS_UINT8            exRabId;
    NDIS_Entity         *ndisEntity     = VOS_NULL_PTR;
    NDIS_ArpPeriodTimer *arpPeriodTimer = VOS_NULL_PTR;
    struct pduinfo_cfg   vcomPdnInfo = {0};
    VOS_INT32            handle;

    NDIS_INFO_LOG(NDIS_TASK_PID, "Ndis_IfaceDownCfgProc entered!");

    exRabId = ifacInfo->iFaceId;
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "pstIfacInfo,  Ndis_ChkRabIdValid fail!");
        return;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "pstIfacInfo,  NDIS_GetEntityByRabId failed!");
        return;
    }

    arpPeriodTimer = &(ndisEntity->ipV4Info.arpPeriodTimer);

    /* 如果周期性ARP定时器还在运行，则停掉 */
    Ndis_StopARPTimer(arpPeriodTimer);

    /* 调用ND SERVER API 释放该RabId对应ND SERVER实体 */
    if ((ndisEntity->rabType & NDIS_ENTITY_IPV6) == NDIS_ENTITY_IPV6) {
        NdSer_Ipv6PdnRel(exRabId);
    }

    /* 网卡关闭通知底软返回失败不影响正常业务进行 */
    handle = Ndis_FindHandleByIface((VOS_INT8)ifacInfo->iFaceId);
    if (handle != NDIS_INVALID_HANDLE) {
        vcomPdnInfo.handle   = (VOS_UINT32)handle;
        vcomPdnInfo.iface_id = ifacInfo->iFaceId;
        vcomPdnInfo.operation_type = NDIS_IFACE_DOWN;

        if (mdrv_vcom_ioctl(ndisEntity->handle, VCOM_IOCTL_PDUINFO_CFG, (VOS_VOID *)(&vcomPdnInfo)) != VOS_OK) {
            NDIS_PRINT_ERROR(Ndis_IfaceUpCfgProc_ENUM, IP_COM_REGISTER_CALLBACK_FAIL);
        }
    }

    /* 更新该RabId对应NDIS实体为空 */
    ndisEntity->rabType    = NDIS_RAB_NULL;
    ndisEntity->rabId      = NDIS_INVALID_RABID;
    ndisEntity->handle     = NDIS_INVALID_HANDLE;
    ndisEntity->used       = PS_FALSE;
    ndisEntity->spePort     = NDIS_INVALID_SPEPORT;
    ndisEntity->speIpfFlag = PS_FALSE;
    return;
}

#else

/*
 * Description: 释放下行缓存队列中的PKT
 * History: VOS_VOID
 */
STATIC VOS_VOID NDIS_ClearDlPktQue(VOS_UINT32 addrIndex)
{
    IMM_Zc      *immZc = VOS_NULL_PTR;
    if (addrIndex >= NAS_NDIS_MAX_ITEM) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_ClearDlPktQue, addrIndex invalid!");
        return;
    }
    if (NDIS_GET_DL_PKTQUE(addrIndex) == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NDIS_ClearDlPktQue, que null!");
        return;
    }

    while (LUP_IsQueEmpty(NDIS_GET_DL_PKTQUE(addrIndex)) != PS_TRUE) {
        if (LUP_DeQue(NDIS_GET_DL_PKTQUE(addrIndex), (VOS_VOID **)(&immZc)) != PS_SUCC) {
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NDIS_ClearDlPktQue, LUP_DeQue return fail!");
            return;
        }
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
    }

    return;
}

STATIC VOS_UINT32 Ndis_RegisterCbProc(NDIS_Entity *ndisEntity)
{
    struct ads_iface_rx_handler_s ifaceRxHandle;
    (VOS_VOID)memset_s(&ifaceRxHandle, sizeof(ifaceRxHandle), 0, sizeof(ifaceRxHandle));
    ifaceRxHandle.user_data     = ndisEntity->rabId;
    ifaceRxHandle.rx_func       = Ndis_DlAdsDataRcvV2;
    ifaceRxHandle.rx_cmplt_func = VOS_NULL_PTR;

    /* 向ADS注册下行回调 */
    if (ads_iface_register_rx_handler(ndisEntity->rabId, &ifaceRxHandle) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_RegisterCbProc, ADS_DL_RegDlDataCallback fail!");
        return PS_FAIL;
    }

    /*lint -e718*/
    if (NDIS_UDI_IOCTL(ndisEntity->handle, NCM_IOCTL_REG_UPLINK_RX_FUNC, AppNdis_UsbReadCb) != VOS_OK) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_RegisterCbProc, regist AppNdis_UsbReadCb fail!");
        return PS_FAIL;
    }
    /*lint +e718*/
    return PS_SUCC;
}
STATIC VOS_VOID Ndis_IfaceUpCfgProc(const AT_NDIS_IfaceUpConfigInd *ifacInfo)
{
    VOS_UINT8                     exRabId;
    VOS_INT32                     handle;
    NDIS_Entity                  *ndisEntity = VOS_NULL_PTR;
    AT_NDIS_PdnInfoCfgReq         pdnInfo;
    exRabId = ifacInfo->iFaceId;

    /* ExRabId参数范围有效性检查。若检查失败，则直接向AT回复配置失败 */
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_IfaceUpCfgProc,  Ndis_ChkRabIdValid fail!");
        return;
    }

    /* 如果根据ExRabId查找不到NDIS实体，则分配一个空闲的NDIS实体 */
    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        /* 如果分配不到空闲的NDIS实体，则返回 */
        ndisEntity = NDIS_AllocEntity();
        if (ndisEntity == VOS_NULL_PTR) {
            NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_IfaceUpCfgProc,  NDIS_AllocEntity failed!");
            return;
        }

        /* 该承载之前没有对应的NDIS实体，故填无效值 */
        NDIS_SET_INITIALSETUP_NDISENTITY_INFO(ndisEntity);
    }

    Ndis_ATIfaceUpCfgTransToPdnInfoCfg(ifacInfo, &pdnInfo);
    if (Ndis_IfaceJugeV4V6ConfigIndicate(&pdnInfo, ndisEntity) == PS_FAIL) {
        return;
    }
    handle  = ifacInfo->handle;
    NDIS_SET_EXIST_NDIS_INFO(ndisEntity, exRabId, handle, 0, 0);

    /* 启动周期发送ARP的定时器 */
    if (Ndis_StartARPTimer(ndisEntity) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_IfaceUpCfgProc StartTmr Failed!");
        return;
    }

    ndisEntity->spePort = NDIS_INVALID_SPEPORT;

    if (Ndis_RegisterCbProc(ndisEntity) != PS_SUCC) {
        return;
    }

#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#if (defined(CONFIG_BALONG_ESPE))
    mdrv_wan_set_ipfmac((struct ethhdr *)(VOS_VOID *)&g_speMacHeader);
#endif
#endif

    return;
}

/*
 * 功能描述: IFACE DOWN消息处理
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_IfaceDownCfgProc(const AT_NDIS_IfaceDownConfigInd *ifacInfo)

{
    VOS_UINT8            exRabId;
    NDIS_Entity         *ndisEntity     = VOS_NULL_PTR;
    NDIS_ArpPeriodTimer *arpPeriodTimer = VOS_NULL_PTR;
    VOS_UINT32           addrIndex;

    NDIS_INFO_LOG(NDIS_TASK_PID, "Ndis_IfaceDownCfgProc entered!");

    exRabId = ifacInfo->iFaceId;
    if (Ndis_ChkRabIdValid(exRabId) != PS_SUCC) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "pstIfacInfo,  Ndis_ChkRabIdValid fail!");
        return;
    }

    ndisEntity = NDIS_GetEntityByRabId(exRabId);
    if (ndisEntity == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "pstIfacInfo,  NDIS_GetEntityByRabId failed!");
        return;
    }

    arpPeriodTimer = &(ndisEntity->ipV4Info.arpPeriodTimer);

    /* 如果周期性ARP定时器还在运行，则停掉 */
    Ndis_StopARPTimer(arpPeriodTimer);

    /* 调用ND SERVER API 释放该RabId对应ND SERVER实体 */
    if ((ndisEntity->rabType & NDIS_ENTITY_IPV6) == NDIS_ENTITY_IPV6) {
        NdSer_Ipv6PdnRel(exRabId);
    }

    /* 更新该RabId对应NDIS实体为空 */
    ndisEntity->rabType    = NDIS_RAB_NULL;
    ndisEntity->rabId      = NDIS_INVALID_RABID;
    ndisEntity->handle     = NDIS_INVALID_HANDLE;
    ndisEntity->used       = PS_FALSE;
    ndisEntity->spePort     = NDIS_INVALID_SPEPORT;
    ndisEntity->speIpfFlag = PS_FALSE;

    /* 释放下行PKT缓存队列 */
    addrIndex = NDIS_GetEntityIndex(exRabId);
    NDIS_ClearDlPktQue(addrIndex);

    return;
}
#endif
STATIC VOS_UINT32 NDIS_RecvMsgParaCheck(VOS_UINT32 length, const NDIS_MsgTypeStruLen *msgStruLen,
                                       VOS_UINT32 msgIdNum, VOS_UINT32 msgId)
{
    VOS_UINT32  msgNumFloop;
    for (msgNumFloop = 0; msgNumFloop < msgIdNum; msgNumFloop++) {
        if (msgId == msgStruLen[msgNumFloop].msgId) {
            if (length < (msgStruLen[msgNumFloop].msTypeLenth - VOS_MSG_HEAD_LENGTH)) {
                return PS_FAIL;
            }
            return PS_SUCC;
        }
    }
    return PS_SUCC;
}

/*
 * 修改历史:
 * 1.日    期: 2011年3月16日
 *   修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_AtMsgProc(const MsgBlock *msgBlock)
{
    AT_NDIS_MsgIdUint32 msgId;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    NDIS_MsgTypeStruLen msgStruLen[] = {
        { ID_AT_NDIS_PDNINFO_CFG_REQ,  sizeof(AT_NDIS_PdnInfoCfgReq) },
        { ID_AT_NDIS_PDNINFO_REL_REQ, sizeof(AT_NDIS_PdnInfoRelReq) },
        { ID_AT_NDIS_IFACE_UP_CONFIG_IND, sizeof(AT_NDIS_IfaceUpConfigInd) },
        { ID_AT_NDIS_IFACE_DOWN_CONFIG_IND, sizeof(AT_NDIS_IfaceDownConfigInd) },
    };
#else
    NDIS_MsgTypeStruLen msgStruLen[] = {
        { ID_AT_NDIS_IFACE_UP_CONFIG_IND, sizeof(AT_NDIS_IfaceUpConfigInd) },
        { ID_AT_NDIS_IFACE_DOWN_CONFIG_IND, sizeof(AT_NDIS_IfaceDownConfigInd) },
    };
#endif
    VOS_UINT32 msgIdNum = sizeof(msgStruLen) / sizeof(NDIS_MsgTypeStruLen);
    COVERITY_TAINTED_SET(msgBlock->value); /* 鹰眼插桩 */
    
    /* 长度异常保护 */
    if (sizeof(MSG_Header) - VOS_MSG_HEAD_LENGTH > msgBlock->ulLength) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_AtMsgProc: input msg length less than struc MSG_Header",
                        msgBlock->ulLength);
        return;
    }

    msgId = ((MSG_Header *)(VOS_VOID *)msgBlock)->msgName;
    if (NDIS_RecvMsgParaCheck(msgBlock->ulLength, msgStruLen, msgIdNum, msgId) == PS_FAIL) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_AtMsgProc: input msg length less than struc MSG_Header",
                        msgId);
        return;
    }
    switch (msgId) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        case ID_AT_NDIS_PDNINFO_CFG_REQ: /* 根据消息的不同处理AT不同的请求 */
            Ndis_PdnInfoCfgProc((AT_NDIS_PdnInfoCfgReq *)(VOS_VOID *)msgBlock);
            break;

        case ID_AT_NDIS_PDNINFO_REL_REQ:
            Ndis_PdnRel((AT_NDIS_PdnInfoRelReq *)(VOS_VOID *)msgBlock);
            break;
#endif
        case ID_AT_NDIS_IFACE_UP_CONFIG_IND:
            Ndis_IfaceUpCfgProc((AT_NDIS_IfaceUpConfigInd *)(VOS_VOID *)msgBlock);
            break;

        case ID_AT_NDIS_IFACE_DOWN_CONFIG_IND:
            Ndis_IfaceDownCfgProc((AT_NDIS_IfaceDownConfigInd *)(VOS_VOID *)msgBlock);
            break;

        default:
            NDIS_WARNING_LOG(NDIS_TASK_PID, "Ndis_AtMsgProc:Error Msg!");
            break;
    }

    return;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
 /*
  * 功能描述: 对上行NCM帧的处理
  */
VOS_VOID Ndis_CdsIpv6OverLenProc(const MsgBlock *msgBlock)
{
    VOS_UINT8             ifaceId;
    CDS_NDIS_DataInd     *rcvMsg = (CDS_NDIS_DataInd *)(VOS_VOID *)msgBlock;
    VOS_UINT8            *data     = VOS_NULL_PTR;
    data      = rcvMsg->data;
    ifaceId   = rcvMsg->ifaceId;

    /* DATA已经是指向IP头起始位置 */
    IP_NDSERVER_ProcTooBigPkt(ifaceId, data, g_nvMtu);
    return;
}

/* 功能描述: NDIS接收CDS消息处理函数
 * 修改历史:
 *  1.日    期: 2020年04月08日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_CdsMsgProc(const MsgBlock *msgBlock)
{
    CDS_NDIS_DataInd        *cdsNdisMsg = (CDS_NDIS_DataInd *)(VOS_VOID *)msgBlock;

    if (sizeof(CDS_NDIS_DataInd) - VOS_MSG_HEAD_LENGTH > msgBlock->ulLength) {
        NDIS_PRINT_ERROR1(Ndis_CdsMsgProc_ENUM, IP_COM_MSGLEN_ERR, msgBlock->ulLength);
        return;
    }

    switch (cdsNdisMsg->ipPktType) {
        case CDS_NDIS_IP_PKT_TYPE_DHCPV4: /* DHCP包 */
            Ndis_DHCPPkt_Proc(msgBlock);
            break;
        case CDS_NDIS_IP_PKT_TYPE_DHCPV6: /* DHCPV6包 */
            NdSer_DhcpV6PktProc(msgBlock);
            break;
        case CDS_NDIS_IP_PKT_TYPE_ICMPV6: /* ND和ECHO REQUEST包 */
            NdSer_NdAndEchoPktProc(msgBlock);
            break;
        case CDS_NDIS_IP_PKT_TYPE_IPV6_OVERLEN: /* IPV6超长包处理 */
            Ndis_CdsIpv6OverLenProc(msgBlock);
            break;
        default:
            NDIS_PRINT_WARNING1(Ndis_CdsMsgProc_ENUM, IP_COM_RECV_MSG_INFO, cdsNdisMsg->ipPktType);
            break;
    }
    return;
}

/*
 * 功能描述: NDIS接收CDS消息处理函数,适用于AT下移之后的特殊包处理
 * 修改历史:
 *  1.日    期: 2020年04月08日
 *    修改内容: 新生成函数
*/
STATIC VOS_VOID Ndis_CdsMsgDispatch(const MsgBlock *msgBlock)
{
    CDS_NDIS_MsgIdUint32  msgId;

    /* 长度异常保护 */
    if (sizeof(MSG_Header) - VOS_MSG_HEAD_LENGTH > msgBlock->ulLength) {
        NDIS_PRINT_ERROR1(Ndis_CdsMsgDispatch_ENUM, IP_COM_MSGLEN_ERR, msgBlock->ulLength);
        return;
    }

    msgId = ((MSG_Header *)(VOS_VOID *)msgBlock)->msgName;

    switch (msgId) {
        case ID_CDS_NDIS_DATA_IND:
            Ndis_CdsMsgProc(msgBlock);
            break;

        default:
            NDIS_PRINT_WARNING1(Ndis_CdsMsgDispatch_ENUM, IP_COM_RECV_MSG_INFO, msgId);
            break;
    }

    return;
}
#else
/*
 * 功能描述: NDIS接收ADS消息处理函数
 * 修改历史:
 *  1.日    期: 2011年12月15日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_AdsMsgProc(const MsgBlock *msgBlock)
{
    ADS_NDIS_DataInd *adsNdisMsg = (ADS_NDIS_DataInd *)(VOS_VOID *)msgBlock;

    if (adsNdisMsg->data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_AdsMsgProc recv NULL PTR!");
        return;
    }

    if (adsNdisMsg->msgId != ID_ADS_NDIS_DATA_IND) {
        /*lint -e522*/
        IMM_ZcFree(adsNdisMsg->data);
        /*lint +e522*/
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_AdsMsgProc, MsgId error!");
        return;
    }

    switch (adsNdisMsg->ipPacketType) {
        case ADS_NDIS_IP_PACKET_TYPE_DHCPV4: /* DHCP包 */
            Ndis_DHCPPkt_Proc(msgBlock);
            break;
        case ADS_NDIS_IP_PACKET_TYPE_DHCPV6: /* DHCPV6包 */
            NdSer_DhcpV6PktProc(msgBlock);
            break;
        case ADS_NDIS_IP_PACKET_TYPE_ICMPV6: /* ND和ECHO REQUEST包 */
            NdSer_NdAndEchoPktProc(msgBlock);
            break;

        default:
            NDIS_WARNING_LOG1(NDIS_TASK_PID, "Ndis_AdsMsgProc:Other Msg!", adsNdisMsg->ipPacketType);
            break;
    }

    /* 处理完成后释放ImmZc */
    /*lint -e522*/
    IMM_ZcFree(adsNdisMsg->data);
    /*lint +e522*/

    return;
}

/*
 * 功能描述: ADS V2消息内容转成V1格式
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_AdsV2MsgTransToV1Msg(const ADS_NDIS_DataIndV2 *v2Msg, ADS_NDIS_DataInd *v1Msg)
{
    (VOS_VOID)memset_s(v1Msg, sizeof(ADS_NDIS_DataInd), 0, sizeof(ADS_NDIS_DataInd));

    v1Msg->msgId        = ID_ADS_NDIS_DATA_IND;
    v1Msg->modemId      = NDIS_GET_MODEMID_FROM_EXBID(v2Msg->ifaceId);
    v1Msg->rabId        = NDIS_GET_BID_FROM_EXBID(v2Msg->ifaceId);
    v1Msg->ipPacketType = v2Msg->ipPacketType;
    v1Msg->data         = v2Msg->data;

    return;
}

/*
 * 功能描述: NDIS接收ADS消息处理函数
 * 修改历史:
 *  1.日    期: 2011年12月15日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_AdsMsgProcV2(const MsgBlock *msgBlock)
{
    ADS_NDIS_DataIndV2        *adsNdisMsg = (ADS_NDIS_DataIndV2 *)(VOS_VOID *)msgBlock;
    ADS_NDIS_DataInd           adsNdisV1Msg = {0};

    if (adsNdisMsg->data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "Ndis_AdsMsgProcV2 recv NULL PTR!");
        return;
    }

    Ndis_AdsV2MsgTransToV1Msg(adsNdisMsg, &adsNdisV1Msg);

    switch (adsNdisV1Msg.ipPacketType) {
        case ADS_NDIS_IP_PACKET_TYPE_DHCPV4: /* DHCP包 */
            Ndis_DHCPPkt_Proc((MsgBlock *)(&adsNdisV1Msg));
            break;
        case ADS_NDIS_IP_PACKET_TYPE_DHCPV6: /* DHCPV6包 */
            NdSer_DhcpV6PktProc((MsgBlock *)(&adsNdisV1Msg));
            break;
        case ADS_NDIS_IP_PACKET_TYPE_ICMPV6: /* ND和ECHO REQUEST包 */
            NdSer_NdAndEchoPktProc((MsgBlock *)(&adsNdisV1Msg));
            break;

        default:
            NDIS_WARNING_LOG1(NDIS_TASK_PID, "Ndis_AdsMsgProcV2:Other Msg!", adsNdisV1Msg.ipPacketType);
            break;
    }

    /* 处理完成后释放ImmZc */
    /*lint -e522*/
    IMM_ZcFree(adsNdisMsg->data);
    /*lint +e522*/

    return;
}

/*
 * 功能描述: NDIS接收ADS消息处理函数
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID Ndis_AdsMsgDispatch(const MsgBlock *msgBlock)
{
    AT_NDIS_MsgIdUint32 msgId;
    NDIS_MsgTypeStruLen msgStruLen[] = {
        { ID_ADS_NDIS_DATA_IND, sizeof(ADS_NDIS_DataInd) },
        { ID_ADS_NDIS_DATA_IND_V2, sizeof(ADS_NDIS_DataIndV2) },
    };
    VOS_UINT32 msgIdNum = sizeof(msgStruLen) / sizeof(NDIS_MsgTypeStruLen);
    COVERITY_TAINTED_SET(msgBlock->value); /* 鹰眼插桩 */

    /* 长度异常保护 */
    if (sizeof(MSG_Header) - VOS_MSG_HEAD_LENGTH > msgBlock->ulLength) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_AdsMsgDispatch: input msg length less than struc MSG_Header",
                        msgBlock->ulLength);
        return;
    }

    msgId = ((MSG_Header *)(VOS_VOID *)msgBlock)->msgName;
    if (NDIS_RecvMsgParaCheck(msgBlock->ulLength, msgStruLen, msgIdNum, msgId) == PS_FAIL) {
        NDIS_ERROR_LOG1(NDIS_TASK_PID, "Ndis_AdsMsgDispatch: input msg length less than struc MSG_Header",
                        msgBlock->ulLength);
        return;
    }
    switch (msgId) {
        case ID_ADS_NDIS_DATA_IND: /* 根据消息的不同处理AT不同的请求 */
            Ndis_AdsMsgProc(msgBlock);
            break;

        case ID_ADS_NDIS_DATA_IND_V2:
            Ndis_AdsMsgProcV2(msgBlock);
            break;

        default:
            NDIS_WARNING_LOG(NDIS_TASK_PID, "Ndis_AdsMsgDispatch:Error Msg!");
            break;
    }

    return;
}
#endif

/*
 * 功能描述: NDIS接收各模块消息处理函数
 * 修改历史:
 *  1.日    期: 2011年2月15日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID APP_Ndis_PidMsgProc(MsgBlock *msgBlock)
{
    if (msgBlock == VOS_NULL_PTR) {
        PS_PRINTF_INFO("Error:APP_Ndis_DLPidMsgProc Parameter pRcvMsg is NULL!");
        return;
    }

    switch (msgBlock->ulSenderPid) {
        case DOPRA_PID_TIMER:
            /*lint -e826*/
            Ndis_ProcTmrMsg((REL_TimerMsgBlock *)msgBlock);
            /*lint +e826*/
            break;

        case APP_AT_PID:
            Ndis_AtMsgProc(msgBlock);
            break;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
        case UEPS_PID_CDS:
            Ndis_CdsMsgDispatch(msgBlock);
            break;
#else
        case ACPU_PID_ADS_UL: /* ADS通过OSA消息发送DHCP和ND SERVER包给NDIS模块 */
            Ndis_AdsMsgDispatch(msgBlock);
            break;
#endif
        default:
            NDIS_WARNING_LOG(NDIS_TASK_PID, "Warning:APP_Ndis_PidMsgProc Recv not expected msg!");
            NDIS_PRINT_WARNING1(APP_Ndis_PidMsgProc_ENUM, IP_COM_RECV_MSG_INFO, msgBlock->ulSenderPid);
            break;
    }

    return;
}

/*lint -e40*/
/*
 * 功能描述: NDIS的FID初始化函数
 * 修改历史:
 *  1.日    期: 2008年9月17日
 *    修改内容: 新生成函数
 */
VOS_UINT32 APP_NDIS_FidInit(enum VOS_InitPhaseDefine phase)
{
    VOS_UINT32 result;

    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            /* 注册NDIS PID */
            result = VOS_RegisterPIDInfo(NDIS_TASK_PID, (InitFunType)APP_Ndis_Pid_InitFunc,
                                         (MsgFunType)APP_Ndis_PidMsgProc);
            if (result != VOS_OK) {
                PS_PRINTF_ERR("APP_NDIS_FidInit, register NDIS PID fail!\n");
                return VOS_ERR;
            }

            /* 注册ND SERVER PID */
            result = VOS_RegisterPIDInfo(NDIS_NDSERVER_PID, (InitFunType)APP_NdServer_Pid_InitFunc,
                                         (MsgFunType)APP_NdServer_PidMsgProc);
            if (result != VOS_OK) {
                PS_PRINTF_ERR("APP_NDIS_FidInit, register NDSERVER PID fail!\n");
                return VOS_ERR;
            }

            result = VOS_RegisterMsgTaskPrio(NDIS_TASK_FID, VOS_PRIORITY_P4);
            if (result != VOS_OK) {
                PS_PRINTF_ERR("APP_NDIS_FidInit, register priority fail!\n");
                return VOS_ERR;
            }
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
            if (VOS_RegisterFidTaskCoreBind(NDIS_TASK_FID, VOS_CORE_MASK_CORE0) != VOS_OK) {
                return VOS_ERR;
            }
#endif
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

    return PS_SUCC;
}
/*lint +e40*/

#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32 Ndis_MsgHook(VOS_UINT8 *data, VOS_UINT32 length, AT_NDIS_MsgIdUint32 msgId)
{
    mdrv_diag_trace_report((VOS_VOID *)data);
    return VOS_OK;
}
#endif
VOS_UINT32 g_ulGUNdisOMSwitch = PS_FALSE;
VOS_VOID   GU_NDIS_OM_SWITCH_ON(VOS_VOID)
{
    g_ulGUNdisOMSwitch = PS_TRUE;
    return;
}

VOS_VOID GU_NDIS_OM_SWITCH_OFF(VOS_VOID)
{
    g_ulGUNdisOMSwitch = PS_FALSE;
    return;
}

/*
 * 功能描述: 显示收发的业务数据信息
 *  修改历史:
 *   1.日    期: 2009年6月16日
 *     修改内容: 新生成函数
 * 修改历史:
 *   2.日    期: 2013年1月22日
 *     修改内容: DSDA
 */
VOS_VOID Ndis_ShowStat(VOS_VOID)
{
    PS_PRINTF_ERR("uplink discard packets num           %d\n", g_ndisStatStru.dicardUsbFrmNum);
    PS_PRINTF_ERR("uplink packets recevied from USB     %d\n", g_ndisStatStru.recvUsbPktSuccNum);
    PS_PRINTF_ERR("uplink packets sent to ADS           %d\n", g_ndisStatStru.sendPktToAdsSucNum);
    PS_PRINTF_ERR("downlink discarded ADS packets       %d\n", g_ndisStatStru.dicardAdsPktNum);
    PS_PRINTF_ERR("downlink received ADS succ           %d\n", g_ndisStatStru.recvAdsPktSuccNum);
    PS_PRINTF_ERR("downlink get IPV6 MAC fail           %d\n", g_ndisStatStru.getIpv6MacFailNum);
    PS_PRINTF_ERR("downlink packet diff with Rab        %d\n", g_ndisStatStru.dlPktDiffRabNum);
    PS_PRINTF_ERR("downlink add MAC head fail           %d\n", g_ndisStatStru.addMacHdrFailNum);
    PS_PRINTF_ERR("downlink send packet fail            %d\n", g_ndisStatStru.dlSendPktFailNum);
    PS_PRINTF_ERR("downlink send packet succ            %d\n", g_ndisStatStru.dlSendPktSuccNum);

    PS_PRINTF_ERR("\nrecv DHCP                          %d\n", g_ndisStatStru.recvDhcpPktNum);
    PS_PRINTF_ERR("recv ARP Request                     %d\n", g_ndisStatStru.recvArpReq);
    PS_PRINTF_ERR("recv ARP Reply                       %d\n", g_ndisStatStru.recvArpReply);
    PS_PRINTF_ERR("proc error ARP packets               %d\n", g_ndisStatStru.procArpError);
    PS_PRINTF_ERR("proc Not Ue Arp                      %d\n", g_ndisStatStru.procNotUeArp);
    PS_PRINTF_ERR("proc Free Arp                        %d\n", g_ndisStatStru.procFreeArp);
    PS_PRINTF_ERR("send ARP Req succ                    %d\n", g_ndisStatStru.sendArpReqSucc);
    PS_PRINTF_ERR("send ARP Req fail                    %d\n", g_ndisStatStru.sendArpReqFail);
    PS_PRINTF_ERR("send ARP Req No Reply                %d\n", g_ndisStatStru.arpReplyNotRecv);
    PS_PRINTF_ERR("send ARP Reply                       %d\n", g_ndisStatStru.sendArpReply);
    PS_PRINTF_ERR("send ARP or DHCP or ND fail          %d\n", g_ndisStatStru.sendArpDhcpNDFailNum);
    PS_PRINTF_ERR("MAC invalid pkt num                  %d\n", g_ndisStatStru.macInvalidPktNum);
    PS_PRINTF_ERR("dl en que pkt num                    %d\n", g_ndisStatStru.enquePktNum);
    PS_PRINTF_ERR("dl send que pkt num                  %d\n", g_ndisStatStru.sendQuePktNum);
    return;
}

/*
 * 功能描述: 显示有效的实体信息
 * 修改历史:
 *  1.日    期: 2011年3月16日
 *    修改内容: 新生成函数
 */
VOS_VOID Ndis_ShowValidEntity(VOS_UINT16 modemId, VOS_UINT8 rabId)
{
    NDIS_Entity      *entity = VOS_NULL_PTR;
    VOS_UINT8         exRabId;

    exRabId = NDIS_FORM_EXBID(modemId, rabId);
    entity = NDIS_GetEntityByRabId(exRabId);
    if (entity == VOS_NULL_PTR) {
        PS_PRINTF_ERR("             no right NDIS entity    \n");
        return;
    }

    PS_PRINTF_ERR("                 ModemID:  %d\n", NDIS_GET_MODEMID_FROM_EXBID(entity->rabId));
    PS_PRINTF_ERR("                 EPS bearer ID:  %d\n", NDIS_GET_BID_FROM_EXBID(entity->rabId));
    PS_PRINTF_ERR("             ARP got flag:  %d\n", entity->ipV4Info.arpInitFlg);
    PS_PRINTF_ERR(" ARP recv reply flag:  %d\n", entity->ipV4Info.arpRepFlg);

    PS_PRINTF_ERR("\n======================================================\n");
}

/*
 * 功能描述: 显示所有的实体信息
 * 修改历史:
 *  1.日    期: 2011年4月18日
 *    修改内容: 新生成函数
 *  2.日    期: 2013年1月16日
 *    修改内容: DSDA
 */
VOS_VOID Ndis_ShowAllEntity(VOS_VOID)
{
    VOS_UINT32   loop;
    NDIS_Entity *entity = VOS_NULL_PTR;

    for (loop = 0; loop < NAS_NDIS_MAX_ITEM; loop++) {
        entity = &g_ndisEntityAddr[loop];
        if (entity->used == PS_FALSE) {
            PS_PRINTF_ERR("                 ModemID:  %d\n", NDIS_GET_MODEMID_FROM_EXBID(entity->rabId));
            PS_PRINTF_ERR("                 EPS bearer ID %d inactive\n", NDIS_GET_BID_FROM_EXBID(entity->rabId));
            continue;
        }

        PS_PRINTF_ERR("                 ModemID:  %d\n", NDIS_GET_MODEMID_FROM_EXBID(entity->rabId));
        PS_PRINTF_ERR("                 EPS bearer ID:  %d\n", NDIS_GET_BID_FROM_EXBID(entity->rabId));
        PS_PRINTF_ERR("             ARP got flag:  %d\n", entity->ipV4Info.arpInitFlg);
        PS_PRINTF_ERR(" ARP recv reply flag:  %d\n", entity->ipV4Info.arpRepFlg);

        PS_PRINTF_ERR("\n======================================================\n");
    }
}

VOS_VOID NDIS_OpenLatency(VOS_VOID)
{
    g_ndisLomSwitch = 1;
    return;
}

VOS_VOID NDIS_CloseLatency(VOS_VOID)
{
    g_ndisLomSwitch = 0;
    return;
}
