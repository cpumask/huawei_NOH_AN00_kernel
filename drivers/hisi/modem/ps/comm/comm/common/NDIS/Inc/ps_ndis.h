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

#ifndef __PSNDIS_H__
#define __PSNDIS_H__

#include "vos.h"

#if (VOS_OS_VER != VOS_WIN32)
#include "ndis_drv.h"
#endif
#include "PsTypeDef.h"
#include "LUPQueue.h"
#include "ttf_comm.h"
#include "at_ndis_interface.h"
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
#include "mdrv_vcom.h"
#include "lps_om.h"
#include "cds_ndis_interface.h"
#include "TLPsPrintDict.h"
#else
#include "imm_interface.h"
#include "ads_dev_i.h"
#include "ads_device_interface.h"
#include "ads_ndis_interface.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/* DHCP选项中最大租约时间长度，单位小时，366天 */
#define TTF_NDIS_DHCP_MAX_LEASE_HOUR 8784

/* DHCP LEASE HOUR NV项默认取值 */
#define TTF_NDIS_DHCP_DEFAULT_LEASE_HOUR 144 /* 单位为小时 */

/* IPV6 MTU NV项默认取值 */
#define TTF_NDIS_IPV6_MTU_DEFAULT 1500 /* 单位为字节 */

#define Ndis_NvimItem_Read(ndisId, itemData, itemDataLen) \
    NDIS_NV_READX(MODEM_ID_0, ndisId, (VOS_VOID *)(itemData), itemDataLen)

/* NDIS上行任务事件 */
#define NDIS_UL_EVENT_RECIVE_USB_DATA 0x0001

/* NDIS下行任务事件 */
#define NDIS_DL_EVENT_RECIVE_ADS_DATA 0x0002

#define NDIS_ENTITY_RABNUM 2
#define NDIS_ENTITY_IPV4INDEX 0
#define NDIS_ENTITY_IPV6INDEX 1
#define NDIS_ENTITY_IPV4ADDRNUM 9

#define NDIS_ENTITY_IPV4 0x1 /* 该NDIS实体对应承载支持IPV4 */
#define NDIS_ENTITY_IPV6 0x2 /* 该NDIS实体对应承载支持IPV4 */

#define NDIS_SPE_CACHE_HDR_SIZE (IMM_MAC_HEADER_RES_LEN + sizeof(ETH_Ipfixhdr))

/* 单独复位名称以及优先级 */
#define PS_NDIS_FUNC_PROC_NAME  "PS_NDIS"       /* NDIS注册给底软的名称 */
#define RESET_PRIORITY_NDIS     22              /* NDIS单独复位优先级 */

#if (VOS_OS_VER != VOS_WIN32)
/* 单板PID,FID，待定义 */
#define NDIS_TASK_PID PS_PID_APP_NDIS
#define NDIS_TASK_FID PS_FID_APP_NDIS_PPP_DIPC
#define APP_AT_PID WUEPS_PID_AT
#else
#define NDIS_TASK_PID PS_PID_NDIS
#define NDIS_TASK_FID LUEPS_FID_APP_NDIS
#define APP_AT_PID LUEPS_PID_AT
#endif

#define NDIS_MODULE_UL UE_MODULE_NDIS_UL
#define NDIS_MODULE_DL UE_MODULE_NDIS_DL
#define NDIS_MODULE_COM UE_MODULE_NDIS_COM

#define NDIS_L4_AT 0xF030 /* MSP AT的模块ID */

/* Define IP Version */
#define IP_VERSION_V4 4
#define IP_VERSION_V6 6

#define NDIS_IFACE_UP   1
#define NDIS_IFACE_DOWN 0

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
/* 得到数据块使用的字节数。 返回数据块使用的字节数 */
#define NDIS_MemGetUsedLen(ndisMem) ((ndisMem)->size)
/* 得到数据块首地址。 返回数据块首地址 */
#define NDIS_MemGetDataPtr(ndisMem) ((ndisMem)->buf)
/* 底层MAA内存释放 */
#define NDIS_MemFree(ndisMem) mdrv_maa_free_lock(((ndisMem)->maa_addr), NDIS_TASK_PID)
#define NDIS_MAA_MEM_ALLOC(length) mdrv_maa_alloc_lock((length), NDIS_TASK_PID, MAA_ACESS)

#else
/* skb内存，得到数据块使用的字节数。 返回数据块使用的字节数 */
#define NDIS_MemGetUsedLen(ndisMem) (IMM_ZcGetUsedLen(ndisMem))
/* 得到数据块首地址。 返回数据块首地址 */
#define NDIS_MemGetDataPtr(ndisMem) (IMM_ZcGetDataPtr(ndisMem))
#define NDIS_MemFree(ndisMem) (IMM_ZcFree(ndisMem))
#define NDIS_GET_DL_PKTQUE(index) (g_ndisEntity[index].dlPktQue)
/* 下行IP包缓存队列长度 */
#define NDIS_IPV4_WAIT_ADDR_RSLT_Q_LEN 10

#endif


/* A核监控-关键事件上报事件ID偏移,需保证与PS已有ID值不重复 */
#define NDIS_OM_KEY_EVENT_BASE_ID 0x11000

#define NDIS_OM_ID(moduleId, logType) DIAG_ID(moduleId, logType)

#define NDIS_ReportEventLog(moduleId, logType, eventID)
#define NDIS_LOG(moduleId, SubMod, level, prinString)
#define NDIS_LOG1(moduleId, SubMod, level, prinString, para1)
#define NDIS_LOG2(moduleId, SubMod, level, prinString, para1, para2)
#define NDIS_LOG3(moduleId, SubMod, level, prinString, para1, para2, para3)
#define NDIS_LOG4(moduleId, SubMod, level, prinString, para1, para2, para3, para4)

#define NDIS_INFO_LOG(moduleId, prinString)
#define NDIS_INFO_LOG1(moduleId, prinString, para1)
#define NDIS_INFO_LOG2(moduleId, prinString, para1, para2)
#define NDIS_INFO_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_INFO_LOG4(moduleId, prinString, para1, para2, para3, para4)

#define NDIS_WARNING_LOG(moduleId, prinString)
#define NDIS_WARNING_LOG1(moduleId, prinString, para1)
#define NDIS_WARNING_LOG2(moduleId, prinString, para1, para2)
#define NDIS_WARNING_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_WARNING_LOG4(moduleId, prinString, para1, para2, para3, para4)

#define NDIS_ERROR_LOG(moduleId, prinString)
#define NDIS_ERROR_LOG1(moduleId, prinString, para1)
#define NDIS_ERROR_LOG2(moduleId, prinString, para1, para2)
#define NDIS_ERROR_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_ERROR_LOG4(moduleId, prinString, para1, para2, para3, para4)

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON) && (VOS_OS_VER != VOS_WIN32)
#define NDIS_PRINT_INFO(enFunction, enProcFlag) TLPS_PRINT2LAYER_INFO((enFunction), (enProcFlag))
#define NDIS_PRINT_INFO1(enFunction, enProcFlag, ulPara1) \
    TLPS_PRINT2LAYER_INFO1((enFunction), (enProcFlag), (VOS_INT32)(ulPara1))
#define NDIS_PRINT_INFO2(enFunction, enProcFlag, ulPara1, ulPara2) \
    TLPS_PRINT2LAYER_INFO2((enFunction), (enProcFlag), (VOS_INT32)(ulPara1), (VOS_INT32)(ulPara2))

#define NDIS_PRINT_WARNING(enFunction, enProcFlag) TLPS_PRINT2LAYER_WARNING((enFunction), (enProcFlag))
#define NDIS_PRINT_WARNING1(enFunction, enProcFlag, ulPara1) \
    TLPS_PRINT2LAYER_WARNING1((enFunction), (enProcFlag), (VOS_INT32)(ulPara1))
#define NDIS_PRINT_WARNING2(enFunction, enProcFlag, ulPara1, ulPara2) \
    TLPS_PRINT2LAYER_WARNING2((enFunction), (enProcFlag), (VOS_INT32)(ulPara1), (VOS_INT32)(ulPara2))

#define NDIS_PRINT_ERROR(enFunction, enProcFlag) TLPS_PRINT2LAYER_ERROR((enFunction), (enProcFlag))
#define NDIS_PRINT_ERROR1(enFunction, enProcFlag, ulPara1) \
    TLPS_PRINT2LAYER_ERROR1((enFunction), (enProcFlag), (VOS_INT32)(ulPara1))
#define NDIS_PRINT_ERROR2(enFunction, enProcFlag, ulPara1, ulPara2) \
    TLPS_PRINT2LAYER_ERROR2((enFunction), (enProcFlag), (VOS_INT32)(ulPara1), (VOS_INT32)(ulPara2))
#else
#define NDIS_PRINT_INFO(enFunction, enProcFlag)
#define NDIS_PRINT_INFO1(enFunction, enProcFlag, ulPara1)
#define NDIS_PRINT_INFO2(enFunction, enProcFlag, ulPara1, ulPara2)

#define NDIS_PRINT_WARNING(enFunction, enProcFlag)
#define NDIS_PRINT_WARNING1(enFunction, enProcFlag, ulPara1)
#define NDIS_PRINT_WARNING2(enFunction, enProcFlag, ulPara1, ulPara2)

#define NDIS_PRINT_ERROR(enFunction, enProcFlag)
#define NDIS_PRINT_ERROR1(enFunction, enProcFlag, ulPara1)
#define NDIS_PRINT_ERROR2(enFunction, enProcFlag, ulPara1, ulPara2)
#endif

/* 通过ModemId和BearerID组合成ExBearerID，高2bit为ModemId,低6bit为BearerID */
#define NDIS_FORM_EXBID(modemId, bearerId) ((VOS_UINT8)(((modemId) << 6) | (bearerId)))

/* 从扩展ExBearerID中获得ModemId */
#define NDIS_GET_MODEMID_FROM_EXBID(exBearerId) (VOS_UINT16)(((exBearerId)&0xC0) >> 6)

/* 从扩展ExBearerID中获得BearerID */
#define NDIS_GET_BID_FROM_EXBID(exBearerId) (VOS_UINT8)((exBearerId)&0x3F)

#define NDIS_SET_INITIALSETUP_NDISENTITY_INFO(ndisEntity) do { \
        ndisEntity->rabType    = NDIS_RAB_NULL; \
        ndisEntity->handle     = NDIS_INVALID_HANDLE; \
        ndisEntity->spePort     = NDIS_INVALID_SPEPORT; \
        ndisEntity->speIpfFlag = PS_FALSE; \
} while (0)

#define NDIS_SET_EXIST_NDIS_INFO(ndisEntity, exRabId, inHandle, inSpePort, inSpeIpfFlag) do { \
        ndisEntity->used       = PS_TRUE;   /* 设置该NDIS实体为使用状态 */ \
        ndisEntity->rabId      = exRabId; /* 将扩展RabId存到对应NDIS实体中 */ \
        ndisEntity->handle     = inHandle;  /* 保存Handle到NDIS实体中 */ \
        ndisEntity->spePort     = inSpePort; /* 保存SPE Port到NDIS实体中 */ \
        ndisEntity->speIpfFlag = inSpeIpfFlag; \
} while (0)


#if (defined(CONFIG_BALONG_SPE))
#define NDIS_SPE_MEM_CB(immZc) ((NDIS_SPE_MemCb *)&((immZc)->dma))

/*lint -emacro({717}, NDIS_SPE_MEM_MAP)*/
#define NDIS_SPE_MEM_MAP(immZc, memLen) do { \
    if (VOS_TRUE == NDIS_IsSpeMem(immZc)) {  \
        NDIS_SpeMemMapRequset(immZc, memLen); \
    }                                           \
} while (0)

/*lint -emacro({717}, NDIS_SPE_MEM_UNMAP)*/
#define NDIS_SPE_MEM_UNMAP(immZc, memLen) do { \
    if (VOS_TRUE == NDIS_IsSpeMem(immZc)) {    \
        NDIS_SpeMemUnmapRequset(immZc, memLen); \
    }                                             \
} while (0)
#else
#define NDIS_SPE_MEM_MAP(immZc, memLen)
#define NDIS_SPE_MEM_UNMAP(immZc, memLen)
#endif

/* 鹰眼插桩 */
#ifdef __EAGLEYE__
extern VOS_VOID Coverity_Tainted_Set(VOS_VOID *pkt);
#define COVERITY_TAINTED_SET(pkt) do { \
        Coverity_Tainted_Set(pkt); \
} while (0)

#else
#define COVERITY_TAINTED_SET(pkt)
#endif


#ifdef _lint
#define ACPU_PID_ADS_UL 211
#endif
#define NDIS_CHK_SEC_RETURN_NO_VAL(x) \
    if (x) {                          \
        IPND_ERROR_LOG3(NDIS_NDSERVER_PID, "NDIS_CHK_SEC_RETURN_NO_VAL!", (VOS_UINT32)x, THIS_FILE_ID, __LINE__); \
        return;                       \
    }
#define NDIS_CHK_SEC_RETURN_VAL(x, y) \
    if (x) {                          \
        IPND_ERROR_LOG3(NDIS_NDSERVER_PID, "NDIS_CHK_SEC_RETURN_VAL!", (VOS_UINT32)x, THIS_FILE_ID, __LINE__); \
        return (y);                   \
    }

/*
 * 3 枚举定义
 */
typedef enum {
    ARP_PAYLOAD  = 0x0608,
    IP_PAYLOAD   = 0x0008,
    IPV6_PAYLOAD = 0xdd86,
    GMAC_PAYLOAD_BUTT
} GMAC_PayloadType;

typedef enum  {
    IP_A_CLASS = 0x0, /* A类型IP */
    IP_B_CLASS = 0x1, /* B类型IP */
    IP_C_CLASS = 0x2, /* C类型IP */
    IP_CLASS_BUTT
} IP_ClassType;
typedef VOS_UINT32 IP_ClassTypeUint32;

/*
 * 结构名: NDIS_RabType
 * 协议表格:
 * ASN.1描述:
 * 结构说明: NDIS承载类型枚举
 */
typedef enum {
    NDIS_RAB_NULL   = 0, /* 该承载未激活 */
    NDIS_RAB_IPV4   = 1, /* 该承载只支持IPV4 */
    NDIS_RAB_IPV6   = 2, /* 该承载只支持IPV6 */
    NDIS_RAB_IPV4V6 = 3, /* 该承载同时支持IPV4和IPV6 */
    NDIS_RAB_BUTT
} NDIS_RabType;
typedef VOS_UINT8 NDIS_RabTypeUint8;


/*
 * 6 消息定义
 */
#define NAS_NDIS_MAX_ITEM 11


#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
typedef struct vcom_rw_info NDIS_Mem;
#else
typedef struct sk_buff NDIS_Mem;
#endif

/*
 * 结构名: IPV4_CfgItem
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8  ueIpV4Addr[IPV4_ADDR_LEN];
    VOS_UINT8  gwIpV4Addr[IPV4_ADDR_LEN];
    VOS_UINT8  nmaskIpV4Addr[IPV4_ADDR_LEN];
    VOS_UINT32 dnsServerNum;
} IPV4_CfgItem;

/*
 * 结构名: IPV6_CfgItem
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8 ueIpV6Addr[IPV6_ADDR_LEN];
    VOS_UINT8 gwIpV6Addr[IPV6_ADDR_LEN];
    VOS_UINT8 nmaskIpV6Addr[IPV6_ADDR_LEN];
} IPV6_CfgItem;

/*
 * 结构名: NAS_NDIS_ArpcfgReq
 * 协议表格:
 * ASN.1描述:
 * 结构说明: NAS NDIS配置
 */
typedef struct {
    VOS_UINT32         ipV4ValidNum;
    IPV4_CfgItem ipV4Cfg[NAS_NDIS_MAX_ITEM];

    VOS_UINT32         ipV6ValidNum;
    IPV4_CfgItem ipV6Cfg[NAS_NDIS_MAX_ITEM];
} NAS_NDIS_ArpcfgReq;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef union {
    VOS_UINT8  ipAddr8bit[IPV4_ADDR_LEN];
    VOS_UINT32 ipAddr32bit;
} IPV4_AddrItem;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef union {
    VOS_UINT8  ipAddr8bit[IPV6_ADDR_LEN];
    VOS_UINT32 ipAddr32bit[4]; /* 4:addr */
} IPV6_AddrItem;

/*
 * 7 STRUCT定义
 */
#if (VOS_OS_VER != VOS_WIN32) /* 单字节对齐 */
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

/*lint -e958*/
/*lint -e959*/
/*
 * 结构名: ARP帧结构
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8  dstAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT8  srcAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT16 frameType;

    VOS_UINT16 hardwareType;
    VOS_UINT16 protoType;
    VOS_UINT8  hardwareLen;
    VOS_UINT8  protoLen;
    VOS_UINT16 opCode;

    VOS_UINT8         senderAddr[ETH_MAC_ADDR_LEN];
    IPV4_AddrItem senderIP;
    VOS_UINT8         targetAddr[ETH_MAC_ADDR_LEN];
    IPV4_AddrItem targetIP;

    VOS_UINT8 rev[18]; /* 18:rsv */
} ETH_ArpFrame;
/*
 * 结构名:  发给SPE的MAC头
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8  dstAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT8  srcAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT32 resv;
} SPE_MAC_EtherHeader;

/*
 * 结构名: ETH_Ipfixhdr
 * 协议表格:
 * ASN.1描述:
 * 结构说明: IP固定头
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
} ETH_Ipfixhdr;

/*
 * 结构名: ETH_Udphdr
 * 协议表格:
 * ASN.1描述:
 * 结构说明: UDP头
 */
typedef struct {
    VOS_UINT16 srcPort;  /* source port */
    VOS_UINT16 dstPort;  /* dest port */
    VOS_UINT16 len;      /* udp length */
    VOS_UINT16 checksum; /* udp check sum */
} ETH_Udphdr;

/*
 * 结构名: IPV4 UDP包结构
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8  dstAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT8  srcAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT16 frameType;

    ETH_Ipfixhdr ipHdt;
    VOS_UINT8         rev[4]; /* 4:rsv */
} ETHFRM_Ipv4Pkt;

/*
 * 结构名: IPV4 IP包结构
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    ETH_Ipfixhdr ipHdt;
    VOS_UINT8         rev[4]; /* 4:rsv */
} ETHFRM_Ipv4Ippkt;

/*lint -e959*/
/*lint -e958*/

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#if (defined(CONFIG_BALONG_SPE))
/*
 * 结构名: NDIS_SPE_MemCb
 * 协议表格:
 * ASN.1描述:
 * 结构说明: SPE MEM CB结构
 */
typedef struct {
    dma_addr_t dmaAddr;
} NDIS_SPE_MemCb;
#endif

/*
 * 结构名: NDIS_ArpPeriodTimer
 * 协议表格:
 * ASN.1描述:　
 * 结构说明: ARP周期定时器结构体
 */
typedef struct {
    HTIMER     tm;          /* 定时器指针 */
    VOS_UINT32 name;       /* 定时器名字 */
    VOS_UINT32 timerValue; /* 定时器时长 */
} NDIS_ArpPeriodTimer;

/*
 * 结构名: NDIS_IPV4_INFO_STRU包结构
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT32        arpInitFlg; /* 是否已获得UE的ARP映射 */
    VOS_UINT32        arpRepFlg;  /* ARP请求发送后受到Reply标志 */
    IPV4_AddrItem ueIpInfo;   /* UE IP,源于NAS配置 */
    IPV4_AddrItem gwIpInfo;   /* GW IP,源于NAS配置 */
    IPV4_AddrItem nmIpInfo;   /* NetMask,源于NAS配置 */
    VOS_UINT8         ueMacAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT8         macFrmHdr[ETH_MAC_HEADER_LEN]; /* 完整以太帧头，便于组包 */
    NDIS_ArpPeriodTimer arpPeriodTimer;        /* 为了同64位操作系统兼容，保持在8字节对齐位置 */

    /* DHCP Server Info */
    VOS_UINT32        ipAssignStatus;
    IPV4_AddrItem primDnsAddr;
    IPV4_AddrItem secDnsAddr;
    IPV4_AddrItem primWinsAddr;
    IPV4_AddrItem secWinsAddr;
} NDIS_Ipv4Info;

/*
 * 结构名: NDIS_Entity
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    NDIS_RabTypeUint8   rabType;
    VOS_UINT8           rabId; /* 保存的是ExRabId的值 */
    PS_BOOL_ENUM_UINT8  used;   /* PS_TRUE:该实体被使用，PS_FALSE:该实体空闲 */
    VOS_UINT8           rev;    /* 预留字节 */
    VOS_INT32           handle;
    NDIS_Ipv4Info       ipV4Info;
    VOS_INT32           spePort;
    VOS_UINT32          speIpfFlag;
    LUP_QUEUE_STRU     *dlPktQue; /* 下行包缓存队列 */
} NDIS_Entity;

/*
 * 结构名: NDIS_StatInfo
 * 协议表格:
 * ASN.1描述:
 * 结构说明: NDIS统计量结构
 */
typedef struct {
    VOS_UINT32 dicardUsbFrmNum;    /* 上行丢弃的USB数据包个数 */
    VOS_UINT32 recvUsbPktSuccNum;  /* 上行成功接收到USB的包的个数 */
    VOS_UINT32 sendPktToAdsSucNum; /* 上行发送到ADS成功的个数 */
    VOS_UINT32 dicardAdsPktNum;    /* 下行被丢弃的ADS 数据包NUM */
    VOS_UINT32 recvAdsPktSuccNum;  /* 下行从ADS收到包成功个数 */
    VOS_UINT32 getIpv6MacFailNum;  /* 下行发包时获取IPV6 MAC地址失败个数 */
    VOS_UINT32 dlPktDiffRabNum;    /* 下行数据包类型和承载类型不一致个数 */
    VOS_UINT32 addMacHdrFailNum;   /* 添加MAC头失败的统计 */
    VOS_UINT32 dlSendPktFailNum;   /* 发送包失败的统计量 */
    VOS_UINT32 dlSendPktSuccNum;   /* 下行成功发送数据包至底软的统计量 */

    VOS_UINT32 recvDhcpPktNum; /* 接收到的DHCP包 */

    /* ARP 统计信息 */
    VOS_UINT32 recvArpReq;           /* 收到的ARP Request包 */
    VOS_UINT32 recvArpReply;         /* 收到的ARP Reply包 */
    VOS_UINT32 procArpError;         /* 处理ARP失败个数 */
    VOS_UINT32 procNotUeArp;         /* 不是对应UE/PC地址的ARP报文个数 */
    VOS_UINT32 procFreeArp;          /* freeARP报文个数 */
    VOS_UINT32 sendArpReqSucc;       /* 发送ARP Request成功包 */
    VOS_UINT32 sendArpReqFail;       /* 发送ARP Request失败数 */
    VOS_UINT32 arpReplyNotRecv;      /* ARP请求没有收到Rely的计数 */
    VOS_UINT32 sendArpReply;         /* 发送ARP REPLY个数 */
    VOS_UINT32 sendArpDhcpNDFailNum; /* 发送ARP或DHCP或ND包失败个数 */

    VOS_UINT32 resetCbBefore;         /* 单独复位BEFORE统计 */
    VOS_UINT32 resetCbAfter;          /* 单独复位AFTER统计 */
    VOS_UINT32 macInvalidPktNum;      /* 下行IP包发送时PC MAC地址无效的统计量 */
    VOS_UINT32 enquePktNum;           /* 下行成功缓存的IP包个数 */
    VOS_UINT32 sendQuePktNum;         /* 下行成功发送缓存的IP包个数 */
} NDIS_StatInfo;
typedef struct {
    VOS_UINT32 msgId;        /* 消息ID */
    VOS_UINT32 msTypeLenth;  /* 消息类型长度 */
} NDIS_MsgTypeStruLen;

extern NDIS_StatInfo g_ndisStatStru;
extern const VOS_UINT8        g_broadCastAddr[];
extern NDIS_Entity      g_ndisEntity[];
extern VOS_UINT32 g_nvMtu;
#define NDIS_STAT_UL_DISCARD_USBPKT(n) (g_ndisStatStru.dicardUsbFrmNum += (n))
#define NDIS_STAT_UL_RECV_USBPKT_SUCC(n) (g_ndisStatStru.recvUsbPktSuccNum += (n))
#define NDIS_STAT_UL_SEND_ADSPKT(n) (g_ndisStatStru.sendPktToAdsSucNum += (n))
#define NDIS_STAT_DL_DISCARD_ADSPKT(n) (g_ndisStatStru.dicardAdsPktNum += (n))
#define NDIS_STAT_DL_RECV_ADSPKT_SUCC(n) (g_ndisStatStru.recvAdsPktSuccNum += (n))
#define NDIS_STAT_DL_GET_IPV6MAC_FAIL(n) (g_ndisStatStru.getIpv6MacFailNum += (n))
#define NDIS_STAT_DL_PKT_DIFF_RAB_NUM(n) (g_ndisStatStru.dlPktDiffRabNum += (n))
#define NDIS_STAT_DL_ADDMACFRM_FAIL(n) (g_ndisStatStru.addMacHdrFailNum += (n))
#define NDIS_STAT_DL_SEND_USBPKT_FAIL(n) (g_ndisStatStru.dlSendPktFailNum += (n))
#define NDIS_STAT_DL_SEND_USBPKT_SUCC(n) (g_ndisStatStru.dlSendPktSuccNum += (n))

#define NDIS_STAT_UL_RECV_DHCPPKT(n) (g_ndisStatStru.recvDhcpPktNum += (n))

#define NDIS_STAT_UL_RECV_ARP_REQUEST(n) (g_ndisStatStru.recvArpReq += (n))
#define NDIS_STAT_DL_RECV_ARP_REPLY(n) (g_ndisStatStru.recvArpReply += (n))
#define NDIS_STAT_PROC_ARP_FAIL(n) (g_ndisStatStru.procArpError += (n))
#define NDIS_STAT_DL_SEND_ARP_REQUEST_SUCC(n) (g_ndisStatStru.sendArpReqSucc += (n))
#define NDIS_STAT_DL_SEND_ARP_REQUEST_FAIL(n) (g_ndisStatStru.sendArpReqFail += (n))
#define NDIS_STAT_ARPREPLY_NOTRECV(n) (g_ndisStatStru.arpReplyNotRecv += (n))
#define NDIS_STAT_DL_SEND_ARP_REPLY(n) (g_ndisStatStru.sendArpReply += (n))
#define NDIS_STAT_DL_SEND_ARPDHCPPKT_FAIL(n) (g_ndisStatStru.sendArpDhcpNDFailNum += (n))
#define NDIS_STAT_RESET_CB_BEFORE(n) (g_ndisStatStru.resetCbBefore += (n))
#define NDIS_STAT_RESET_CB_AFTER(n) (g_ndisStatStru.resetCbAfter += (n))
#define NDIS_STAT_PROC_NOTUE_ARP(n) (g_ndisStatStru.procNotUeArp += (n))
#define NDIS_STAT_PROC_FREE_ARP(n) (g_ndisStatStru.procFreeArp += (n))
#define NDIS_STAT_MAC_INVALID_PKT_NUM(n) (g_ndisStatStru.macInvalidPktNum += (n))
#define NDIS_STAT_ENQUE_PKT_NUM(n) (g_ndisStatStru.enquePktNum += (n))
#define NDIS_STAT_SEND_QUE_PKT_NUM(n) (g_ndisStatStru.sendQuePktNum += (n))
/*
 * 10 函数声明
 */
NDIS_Entity *NDIS_GetEntityByRabId(VOS_UINT8 exRabId);
VOS_UINT32 Ndis_SendMacFrm(const VOS_UINT8 *buf, VOS_UINT32 len, VOS_UINT8 rabId);
VOS_UINT8 *Ndis_GetMacAddr(VOS_VOID);

/*
 * 全局变量声明
 */
extern SPE_MAC_EtherHeader g_speMacHeader;

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_UINT32   Ndis_DlSendNcm(VOS_UINT8 rabId, ADS_PktTypeUint8 pktType, IMM_Zc *immZc);
VOS_UINT32 Ndis_UpdateMacAddr(VOS_UINT8 *ueMacAddr, VOS_UINT8 lenth, NDIS_Ipv4Info *arpV4Info);
VOS_VOID NDIS_SendDlPkt(VOS_UINT32 addrIndex);
VOS_UINT32 NDIS_GetEntityIndex(VOS_UINT8 exRabId);
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

#endif /* end of LUPNdis.c */
