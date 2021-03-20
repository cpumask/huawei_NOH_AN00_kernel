﻿/*
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
#ifndef __BSP_ICC_H__
#define __BSP_ICC_H__

#include <osl_common.h>
#include <bsp_sram.h>
#include <bsp_shared_ddr.h>
#include <mdrv_icc.h>
#include <bsp_ipc.h>
#include <mdrv_public.h>

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

#define ICC_CHAN_NUM_MAX (32)
#define ICC_DYNAMIC_CHAN_NUM_MAX (8)

int BSP_ICC_Open(unsigned int u32ChanId, icc_chan_attr_s *pChanAttr);
int BSP_ICC_Write(unsigned int u32ChanId, unsigned char *pData, int s32Size);
int BSP_ICC_Read(unsigned int u32ChanId, unsigned char *pData, int s32Size);
s32 BSP_ICC_Debug_Register(u32 u32ChanId, funcptr_1 debug_routine, int param);
/* from drv_icc.h start */
#define ICC_BUSY (0x03 | NOTIFY_STOP_MASK)
#define ICC_OK (0)
#define ICC_ERR (-1)
#define ICC_CHANNEL_IS_FULL_FLAG (0xFF)

/*lint --e{488}*/
/* CPU ID 分配 */
enum CPU_ID {
    ICC_CPU_MIN = IPC_CORE_ACORE,
    ICC_CPU_APP = IPC_CORE_ACORE,
    ICC_CPU_MODEM = IPC_CORE_CCORE,
    ICC_CPU_MCU = IPC_CORE_MCORE,
    ICC_CPU_TLDSP = IPC_CORE_LDSP,
    ICC_CPU_HIFI = IPC_CORE_HiFi,
    ICC_CPU_NRCCPU = IPC_CORE_NRCCPU,
    ICC_CPU_APLUSB = IPC_CORE_NRCCPU, /* for a+b dummy coreid */
    ICC_CPU_MAX
};

/* 错误码定义 */
enum ICC_ERR_NO {
    ICC_CHN_INIT_FAIL = (0x80000000 + (0 << 16)),
    ICC_MALLOC_CHANNEL_FAIL,
    ICC_MALLOC_VECTOR_FAIL,
    ICC_CREATE_TASK_FAIL,
    ICC_DEBUG_INIT_FAIL,
    ICC_CREATE_SEM_FAIL,
    ICC_REGISTER_INT_FAIL,
    ICC_INVALID_PARA,
    ICC_WAIT_SEM_TIMEOUT,
    ICC_SEND_ERR,
    ICC_RECV_ERR,
    ICC_REGISTER_CB_FAIL,
    ICC_REGISTER_DPM_FAIL,
    ICC_MALLOC_MEM_FAIL,
    ICC_NULL_PTR,
    ICC_INIT_ADDR_TOO_BIG,
    ICC_INIT_SKIP,
    ICC_REGISTER_DYNAMIC_CB_FAIL,
#ifdef CONFIG_APLUSB_PCIE_ICC
    ICC_REGISTER_PCIE_MSI_CB_FAIL,
    ICC_PCIE_MSI_SEND_FAIL,
    ICC_ENABLE_PCIE_MSI_FAIL,
#endif
    ICC_DYNAMIC_SEND_FAIL,
    ICC_CHA_FIFO_SIZE_FAIL
};

/* 通道id分配 */
enum ICC_CHN_ID {
    ICC_CHN_ACORE_CCORE_MIN = 0, /* add for modem reset : A/C核通道开始标志 */
    ICC_CHN_IFC = 0,             /* acore与ccore之间的IFC的物理通道 */
    ICC_CHN_RFILE = 1,           /* acore与ccore之间的RFILE的物理通道 */
    ICC_CHN_NV = 2,              /* acore与ccore之间的NV的物理通道 */
    ICC_CHN_MLOADER = 3,         /* acore与ccore之间的MLOADER的物理通道 */
    ICC_CHN_GNSS,                /* acore与ccore之间的GPS的物理通道 */
    ICC_CHN_CUST,              /* acore与ccore之间的CUST的物理通道,预留给产品线使用，支撑产品线二次开发 */
    ICC_CHN_RESERVED2,         /* acore与ccore之间的RESERVED2的物理通道 */
    ICC_CHN_GUOM4,             /* acore与ccore之间的GUOM4的物理通道 */
    ICC_CHN_RESERVED3,         /* acore与ccore之间的RESERVED3的物理通道 */
    ICC_CHN_CSHELL,            /* acore与ccore之间的CSHELL的物理通道 */
    ICC_CHN_AC_PANRPC,         /* acore与ccore之间的PAN_RPC的物理通道 */
    ICC_CHN_ACORE_CCORE_MAX,   /* add for modem reset : A/C核通道结束标志 */
    ICC_CHN_MCORE_CCORE,       /* mcore与ccore之间的唯一的物理通道 */
    ICC_CHN_MCORE_ACORE,       /* mcore与acore之间的唯一的物理通道 */
    ICC_CHN_CCPU_HIFI_VOS_MSG, /* CCPU  和 HIFI 之间的核间通信通道 */
    ICC_CHN_HIFI_TPHY_MSG,     /* BBE16 和 HIFI 之间的核间通信代理通道 */
    ICC_CHN_VT_VOIP,           /* for LTE video phone use, ACORE<-->CCORE */
    ICC_CHN_SEC_IFC,           /* 共享IPC无任务直接回调处理函数，安全OS与ccore共享物理通道 */
    ICC_CHN_SEC_VSIM,          /* 私有IPC无任务直接回调处理函数，天际通安全OS与ccore物理通道 */
    ICC_CHN_SEC_RFILE,         /* RFILE安全OS与ccore物理通道 */
    ICC_CHN_IPC_EXTEND, /* 私有IPC无任务直接回调处理函数，扩展IPC 中断，acore 与ccore物理通道 */
    ICC_CHN_DYNAMIC,    /* 共享IPC无任务直接回调处理函数，ICC 动态建立专属通道 */
    ICC_CHN_IQI,
    ICC_CHN_NRIFC,
    ICC_CHN_NRCCPU_APCPU_OSA,
    ICC_CHN_NRCCPU_LRCCPU_OSA,
    ICC_CHN_APLUSB_IFC,
    ICC_CHN_APLUSB_PCIE,
    ICC_CHN_MCORE_NRCCPU,
    ICC_CHN_NRCCPU_APCPU_CSHELL,
    ICC_CHN_NRCCPU_APCPU_MLOADER,
    ICC_CHN_ACORE_HIFI, /* MBB 新增modem 单独复位使用通道 */
    ICC_STATIC_CHN_ID_MAX
};

/* 说明: 接收回调函数ID,子通道必须放置在, "通道名称_xxx=0和通道名称_RECV_FUNC_ID_MAX之间，
 * 如: 要在IFC通道添加一个子通道，名称为IFC_RECV_ONLY_TEST，则需要:
    IFC_RECV_FUNC_RTC_SETTIME = 0,
    IFC_RECV_FUNC_BBE,
    IFC_RECV_ONLY_TEST,
    IFC_RECV_FUNC_ID_MAX
 */
enum ICC_RECV_FUNC_ID {
    IFC_RECV_FUNC_RTC_SETTIME = 0,
    IFC_RECV_FUNC_ONOFF,
    IFC_RECV_FUNC_EFUSE,
    IFC_RECV_FUNC_SIM0,
    IFC_RECV_FUNC_SIM1,
    IFC_RECV_FUNC_CSHELL,
    IFC_RECV_FUNC_UART,
    IFC_RECV_FUNC_LED,
    IFC_RECV_FUNC_RESET,
    FC_RECV_FUNC_GPSCLK,
    IFC_RECV_FUNC_WAKEUP,
    IFC_RECV_FUNC_PM_OM,
    IFC_RECV_FUNC_SOCP_DEBUG,
    IFC_RECV_FUNC_REMOTE_CLK,
    IFC_RECV_FUNC_PMU_OCP,
    IFC_RECV_FUNC_SYSTEM_HEATING,
    IFC_RECV_FUNC_SYS_BUS,
    IFC_RECV_FUNC_LOADPS,
    IFC_RECV_FUNC_MPERF,
    IFC_RECV_FUNC_IQI,
    IFC_RECV_FUNC_FREQ,
    IFC_RECV_FUNC_PPM_NV,
    IFC_RECV_FUNC_APB_SENSORHUB,
    IFC_RECV_FUNC_TRNG_SEED,
    IFC_RECV_FUNC_AVS_TEST,
    IFC_RECV_FUNC_HIFI_RESET,
    /* 若要在ICC_CHN_IFC物理通道上定义子通道,请在该注释行之前定义 */
    IFC_RECV_FUNC_TEST1,
    IFC_RECV_FUNC_ID_MAX,

    RFILE_RECV_FUNC_ID = 0,
    /* 若要在ICC_CHN_RFILE物理通道上定义子通道,请在该注释行之前定义 */
    RFILE_RECV_FUNC_ID_MAX,

    NV_RECV_FUNC_AC = 0,
    NV_RECV_FUNC_SC = 1,
    /* 若要在ICC_CHN_NV物理通道上定义子通道,请在该注释行之前定义 */
    NV_RECV_FUNC_ID_MAX,

    MLOADER_RECV_FUNC_ID = 0,
    /* 若要在ICC_CHN_MLOADER物理通道上定义子通道,请在该注释行之前定义 */
    MLOADER_RECV_FUNC_ID_MAX,

    GUOM4_TEST1 = 0,
    /* 若要在ICC_CHN_GUOM4物理通道上定义子通道,请在该注释行之前定义 */
    GUOM4_RECV_FUNC_ID_MAX,

    GUOM5_TEST1 = 0,
    /* 若要在ICC_CHN_GUOM5物理通道上定义子通道,请在该注释行之前定义 */
    GUOM5_RECV_FUNC_ID_MAX,

    CSHELL_TEST1 = 0,
    /* 若要在ICC_CHN_CSHELL物理通道上定义子通道,请在该注释行之前定义 */
    CSHELL_RECV_FUNC_ID_MAX,

    AC_PANRPC_ID = 0,
    ECDC_PANRPC_ID = 1,
    /* 若要在ICC_CHN_AC_PANRPC物理通道上定义子通道,请在该注释行之前定义 */
    AC_PANRPC_RECV_FUNC_ID_MAX,

    MCORE_CCORE_FUNC_TEST1 = 0,
    MCORE_CCORE_FUNC_TEST2,
    MCORE_CCORE_FUNC_HKADC = 2,  /* austin赋值，不能修改 */
    MCU_CCORE_CPUFREQ = 3,       /* austin赋值，不能修改 */
    MCU_CCORE_WSRC = 4,          /* austin赋值，不能修改 */
    MCORE_CCORE_FUNC_WAKEUP = 5, /* austin赋值，不能修改 */
    MCORE_CCORE_FUNC_REGULATOR,
    MCORE_CCORE_FUNC_SIM0,
    MCORE_CCORE_FUNC_SIM1,
    MCORE_CCORE_FUNC_SOCP = 9, /* austin赋值，不能修改 */
    MCORE_CCORE_FUNC_UART,
    MCORE_CCORE_FUNC_TSENSOR,
    MCORE_CCORE_FUNC_PDLOCK,
    MCORE_CCORE_FUNC_ADC_CONF = 13,
    MCORE_CCORE_FUNC_TEMP_PROTECT,
    /* 若要在ICC_CHN_MCORE_CCORE物理通道上定义子通道,请在该注释行之前定义 */
    MCORE_CCORE_FUNC_ID_MAX,

    MCORE_ACORE_FUNC_TEST1 = 0,
    MCORE_ACORE_FUNC_TEST2,
    MCORE_ACORE_FUNC_HKADC,
    MCU_ACORE_CPUFREQ,
    MCU_ACORE_WSRC,
    NV_RECV_FUNC_AM,
    MCORE_ACORE_FUNC_TEMPERATURE,
    MCU_ACORE_RFILE, /* for rfile */
    MCORE_ACORE_FUNC_WAKEUP,
    MCORE_ACORE_FUNC_DUMP,
    MCORE_ACORE_FUNC_RESET,
    MCORE_ACORE_FUNC_PM_OM,
    MCU_ACORE_DDRFREQ,
    MCORE_ACORE_FUNC_TSENSOR,
    /* 若要在ICC_CHN_MCORE_ACORE物理通道上定义子通道,请在该注释行之前定义 */
    MCORE_ACORE_FUNC_ID_MAX,

    /* BBE16  和 HIFI 之间的消息GU通道功能号  */
    HIFI_TPHY_MSG_NRM = 0,
    HIFI_TPHY_MSG_URG,
    HIFI_TPHY_MSG_MAX,

    /* BBE16  和 HIFI 之间的消息TDS通道功能号  */
    CCPU_HIFI_VOS_MSG_NRM = 0,
    CCPU_HIFI_VOS_MSG_URG,
    CCPU_HIFI_VOS_MSG_MAX,

    VT_VOIP_FUNC_VT_VOIP = 0,
    VT_VOIP_FUNC_RCS_SERV,
    VT_VOIP_FUNC_ID_MAX,

    SEC_IFC_RECV_FUNC_MODULE_VERIFY = 0,
    SEC_IFC_RECV_FUNK_TRNG_SEED_GET = 1,
    SEC_IFC_RECV_FUNK_EFUSE = 2,
    /* 若要在物理通道上定义子通道,请在该注释行之前定义 */
    SEC_IFC_RECV_FUNC_ID_MAX,

    SEC_VSIM_RECV_FUNC_TEST = 0,

    /* 若要在物理通道上定义子通道,请在该注释行之前定义 */
    SEC_VSIM_RECV_FUNC_ID_MAX,

    SEC_RFILE_RECV_FUNC_SUB0 = 0,

    /* 若要在物理通道上定义子通道,请在该注释行之前定义 */
    SEC_RFILE_RECV_FUNC_ID_MAX,

    DYNAMIC_ICC_FUNC_TEST = 0,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    DYNAMIC_ICC_FUNC_ID_MAX,

    IQI_ICC_FUNC_TEST = 0,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    IQI_ICC_FUNC_ID_MAX,
    IQI_LOGIC_CHAN_AC = 0,

    NRIFC_RECV_FUNC_PERFSTAT = 0,
    NRIFC_RECV_FUNC_ECDC = 1,
    NRIFC_RECV_FUNC_5G_MPERF = 2,
    NRIFC_RECV_FUNC_MLOADER = 3,
    NRIFC_RECV_FUNC_SYSBOOT = 4, /* 5G ONOFF */
    NRIFC_RECV_FUNC_WAKEUP_DEBUG = 5,
    NRIFC_RECV_FUNC_CSHELL = 6,
    NRIFC_RECV_FUNC_PM_OM = 7,
    NRIFC_RECV_FUNC_EFUSE = 8,
    NRIFC_RECV_FUNC_AVS_TEST = 9,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    NRIFC_ICC_FUNC_ID_MAX,

    NRCCPU_APCPU_OSA_FUNC_TEST = 0,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    NRCCPU_APCPU_OSA_FUNC_ID_MAX,

    NRCCPU_LRCCPU_OSA_FUNC_TEST = 0,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    NRCCPU_LRCCPU_OSA_FUNC_ID_MAX,

    APLUSB_IFC_FUNC_TEST = 0,
    APLUSB_IFC_FUNC_PPM = 1,
    ACORE_ACORE_FUNC_HKADC = 2,
    APLUSB_IFC_FUNC_EFUSE = 3,
    APLUSB_IFC_FUNC_SIM0 = 4,
    APLUSB_IFC_FUNC_SIM1 = 5,
    APLUSB_IFC_FUNC_APB_SENSORHUB = 6,
    APLUSB_IFC_FUNC_SYSBOOT = 7,
    APLUSB_IFC_FUNC_PMOM_ON = 8,
    APLUSB_IFC_FUNC_PMOM_OFF = 9,
    APLUSB_IFC_FUNC_HOT_PATCH = 10,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    APLUSB_IFC_FUNC_FUNC_ID_MAX,

    APLUSB_PCIE_FUNC_TEST = 0,
    /* 该通道为A+B 方案上的专有通道，所有子通道在PCIE_ICC 中直接定义，请勿在此单独定义子通道 */
    APLUSB_PCIE_FUNC_FUNC_ID_MAX,

    MCORE_NRCCPU_FUNC_TEST = 0,
    MCORE_NRCCPU_TEMPERATURE_PROTECT,
    MCORE_NRCCPU_DDRFREQ,
    MCORE_NRCCPU_FUNC_WAKEUP,
    /* 该通道为A+B 方案上的专有通道，所有子通道在PCIE_ICC 中直接定义，请勿在此单独定义子通道 */
    MCORE_NRCCPU_FUNC_FUNC_ID_MAX,

    NRCCPU_APCPU_CSHELL_TEST1 = 0,
    /* 若要在ICC_CHN_NRCCPU_APCPU_CSHELL物理通道上定义子通道,请在该注释行之前定义 */
    NRCCPU_APCPU_CSHELL_RECV_FUNC_ID_MAX,

    NRCCPU_APCPU_MLOADER_TEST1 = 0,
    /* 若要在ICC_CHN_NRCCPU_APCPU_CSHELL物理通道上定义子通道,请在该注释行之前定义 */
    NRCCPU_APCPU_MLOADER_RECV_FUNC_ID_MAX,

    ACORE_HIFI_DUMP = 0,
    /* 若要在动态通道上定义子通道,请在该注释行之前定义 */
    ACORE_HIFI_FUNC_ID_MAX,
}; /* 枚举定义end */

struct icc_channel_packet {
    u32 channel_id;         /* 该数据包记录的通道id */
    u32 len;                /* 该数据包长度 */
    u32 src_cpu_id;         /* 该数据包的发送者的cpu id */
    u32 seq_num;            /* 该数据包的序列号: 同步发送的等待队列需要用到 */
    u32 need_responsed : 1; /* 该数据包是否需要回复给对方核标识bit位 */
    u32 is_responsed : 1;   /* 该数据包是否是对方核回复过来的标识bit位 */
    u32 reserved : 30;      /* 保留bit位 */
    s32 data;               /* 数据包头的context */
    u32 timestamp;          /* 该数据包的时间戳 */
    u32 task_id;            /* 该数据包的发送者的任务id */
};

struct icc_channel_compact_packet {
    u32 channel_id; /* 该数据包记录的通道id */
    u32 len;        /* 该数据包长度 */
    u32 timestamp;  /* 该数据包的时间戳 */
    u32 task_id;    /* 该数据包的发送者的任务id */
};

struct icc_channel_fifo {
    u32 magic;  /* fifo魔数，标识通道fifo的状态 */
    u32 size;   /* fifo大小 */
    u32 write;  /* fifo读指针 */
    u32 read;   /* fifo写指针 */
    u8 data[4]; /* fifo的context */
};

struct icc_dynamic_para {
    const char *name;
    u32 size;
    read_cb_func read_cb; /* 接收向量的读回调函数指针 */
    void *read_context;   /* 接收向量的读回调函数context */
};

#define ICC_CHN_ID_MAX (ICC_STATIC_CHN_ID_MAX + ICC_DYNAMIC_CHAN_NUM_MAX)
#define ICC_CHANNEL_PAYLOAD (sizeof(struct icc_channel_packet) + 4)
#define ICC_CHANNEL_ID_MAKEUP(channel_id, func_id) ((channel_id << 16) | (func_id))
#define SHM_ADDR_ICC (SHM_BASE_ADDR + SHM_OFFSET_ICC)
#define SHM_ADDR_SHA_SEC_ICC (SHM_BASE_ADDR + SHM_OFFSET_SHA_SEC_ICC)
#define SRAM_ADDR_ICC (SRAM_BASE_ADDR + SRAM_OFFSET_ICC)

typedef s32 (*write_cb_func)(u32 channel_id, void *context);
typedef void (*notify)(void);

#ifdef CONFIG_ICC
/* 对外接口声明start */
/* ****************************************************************************
* 函 数 名  : bsp_icc_get_first_package_lenth
* 功能描述  :获取icc通道里没有读走数据的第一包的长度
* 输入参数  : u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                               1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID的枚举值
                               2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
* 输出参数  : 无
* 返 回 值  : 没有读走数据的第一包的长度
**************************************************************************** */
s32 bsp_icc_get_first_package_lenth(u32 channel_id);
/* ****************************************************************************
* 函 数 名  : bsp_icc_send
* 功能描述  : icc异步发送数据接口
* 输入参数  : u32 cpuid       数据要发送的cpu的编号，使用约束: 要使用enum CPU_ID定义成员的枚举值
*             u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                               1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID的枚举值
                               2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             u8 *buffer      要发送的数据buffer的指针
*             u32 data_len    要发送的数据的大小, 使用约束: 最大值 < fifo_size - ICC_CHANNEL_PACKET_LEN
* 输出参数  : 无
* 返 回 值  : 正确:  实际写入fifo的大小;  错误: 负值
* 说    明  : 1) 公共通道：配套使用的注册在对方核的回调函数里必须要使用bsp_icc_read()函数把数据读走
*            2)
专用通道：支持在改通道使用模块的任务上下文中使用bsp_icc_read()函数把数据读走，但发送端必须有反压（通道数据发送缓慢）处理机制。

**************************************************************************** */
s32 bsp_icc_send(u32 cpuid, u32 channel_id, u8 *buffer, u32 data_len);

/* ****************************************************************************
* 函 数 名  : bsp_icc_send_sync
* 功能描述  : icc同步发送数据接口
* 输入参数  : u32 cpuid       数据要发送的cpu的编号，使用约束: 要使用enum CPU_ID定义成员的枚举值
*             u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                               1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID的枚举值
                               2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应子通道id的枚举值
*             u8 *buffer      要发送的数据buffer的指针
*             u32 data_len    要发送的数据的大小, 使用约束: 最大值 < fifo_size - ICC_CHANNEL_PACKET_LEN
*             s32 timeout     等回复超时间，使用约束: 要无限等需要使用宏 MAX_SCHEDULE_TIMEOUT
* 输出参数  : 无
* 返 回 值  : 正确: 接收核回调函数的返回值;  错误: 负值
* 说    明  : 1) 配套使用的注册在对方核的回调函数里必须要使用bsp_icc_read()函数把数据读走
*             2) 调用该接口会导致调用者任务睡眠直到超时时间到，或者对方核返回函数调用结果
*             3)
该接口适用于核间函数调用(IFC)的情形，核间函数调用只能返回系统自带的简单数据类型，不能返回结构体等用户自定义类型
*             4) 该接口不能在icc回调中使用，会引起死锁和icc任务延迟的严重后果

**************************************************************************** */
s32 bsp_icc_send_sync(u32 cpuid, u32 channel_id, u8 *data, u32 data_len, long timeout);

/* ****************************************************************************
* 函 数 名  : bsp_icc_read
* 功能描述  : icc从fifo读取数据接口
* 输入参数  : u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             u8 *buf        要读取fifo的数据buffer的指针
*             u32 buf_len    数据buffer的大小
* 输出参数  : 无
* 返 回 值  : 正确:  实际从通道读取数据大小;  错误: 负值
* 说    明  : 1)
可在回调函数中使用，或使用者自己任务上下文中使用，但发送端必须有反压（通道数据发送缓慢）处理机制，以保持核间通信数据流畅通
**************************************************************************** */
s32 bsp_icc_read(u32 channel_id, u8 *buf, u32 buf_len);

/* ****************************************************************************
* 函 数 名  : bsp_icc_event_register
* 功能描述  : 使用icc通道注册回调函数接口
* 输入参数  : u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             read_cb_func read_cb      读回调
*             void *read_context        传给接收任务的上下文信息，做扩展用
*             write_cb_func write_cb    读回调
*             void *write_context       传给接收任务的上下文信息，做扩展用
* 输出参数  : 无
* 返 回 值  : 正确:  0;  错误: 错误码
* 说    明  : 回调函数中不能有任何会引起任务阻塞、睡眠的函数调用，如：
*             1) taskDelay()
*             2) 信号量获取
*             3) printf()
*             4) malloc()
*             5) bsp_icc_send_sync()
**************************************************************************** */
s32 bsp_icc_event_register(u32 channel_id, read_cb_func read_cb, void *read_context, write_cb_func write_cb,
    void *write_context);

/* ****************************************************************************
* 函 数 名  : bsp_icc_event_unregister
* 功能描述  : 使用icc通道去注册回调函数接口
* 输入参数  : u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
* 输出参数  : 无
* 返 回 值  : 正确:  0;  错误: 负值
**************************************************************************** */
s32 bsp_icc_event_unregister(u32 channel_id);

/* ****************************************************************************
* 函 数 名  : bsp_icc_debug_register
* 功能描述  : 使用icc通道注册回调函数接口
* 输入参数  : u32 channel_id  channel_id = 通道id << 16 || function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             funcptr_1 debug_routine     带一个参数的低功耗唤醒回调
*             int param       传给回调函数的参数
* 输出参数  : 无
* 返 回 值  : 正确:  0;  错误: 错误码
* 说    明  : 回调函数中不能有任何会引起任务阻塞、睡眠的函数调用，如：
*             1) taskDelay()
*             2) 信号量获取
*             3) printf()
*             4) malloc()
*             5) bsp_icc_send_sync()
**************************************************************************** */
s32 bsp_icc_debug_register(u32 channel_id, funcptr_1 debug_routine, int param);
/* ****************************************************************************
 * 函 数 名  : pcie_icc_valid_register
 * 功能描述  : a+b 上提供的icc 通道注册接口
 * 输入参数  :
 * 输出参数  : 无
 * 返 回 值  : 无
 * 说    明  :
 * *************************************************************************** */
void pcie_icc_valid_register(notify func);

/* ****************************************************************************
 * 函 数 名  : bsp_icc_dpm_register
 * 功能描述  : 低功耗流程里icc提供的注册接口，用于用户区分是否在低功耗流程
 * 输入参数  :
 * 输出参数  : 无
 * 返 回 值  : 无
 * 说    明  :
 * *************************************************************************** */
s32 bsp_icc_dpm_register(u32 channel_id, notify icc_dpm_cb);

#else
static inline s32 bsp_icc_send(u32 cpuid, u32 channel_id, u8 *buffer, u32 data_len)
{
    return 0;
}
static inline s32 bsp_icc_read(u32 channel_id, u8 *buf, u32 buf_len)
{
    return 0;
}
static inline s32 bsp_icc_event_register(u32 channel_id, read_cb_func read_cb, void *read_context,
    write_cb_func write_cb, void *write_context)
{
    return 0;
}
static inline s32 bsp_icc_debug_register(u32 channel_id, funcptr_1 debug_routine, int param)
{
    return 0;
}
static inline s32 bsp_icc_event_unregister(u32 channel_id)
{
    return 0;
}
#endif


#if (defined(__CMSIS_RTOS) || defined(__M3_OS_RTOSCK__)) && !defined(CONFIG_ICC)
static inline s32 bsp_icc_init(void)
{
    return 0;
}
#else
s32 bsp_icc_init(void);
#endif
void bsp_icc_release(void);
s32 bsp_icc_suspend(void);
/* ICC通道配置，模块内部引用，非对外提供，其他模块不要引用 */
#define STRU_SIZE (sizeof(struct icc_channel_fifo))
#define ICC_IFC_SIZE (SZ_4K)
#ifdef CONFIG_NRICC
#define ICC_RFILE_SIZE (SZ_64K)
#else
#define ICC_RFILE_SIZE (SZ_4K)
#endif
#define ICC_NV_SIZE (SZ_4K)
#define ICC_GUOM0_SIZE (SZ_16K)
#define ICC_GUOM1_SIZE (0)
#define ICC_GUOM2_SIZE (0)
#define ICC_GUOM3_SIZE (0)
#define ICC_GUOM4_SIZE (SZ_128K)
#define ICC_GUOM5_SIZE (0)
#define ICC_CSHELL_SIZE (SZ_8K)
#define ICC_PANRPC_SIZE (SZ_4K)
#define ICC_MCCORE_SIZE (SZ_512)
#define ICC_MACORE_SIZE (SZ_512)
#define ICC_HIFI_VOS_SIZE (6 * 1024)
#define ICC_HIFI_TPHY_SIZE (6 * 1024)
#define ICC_VT_VOIP_SIZE (SZ_4K)
#define ICC_IPC_EXTEND_SIZE (0)
#define ICC_DYNAMIC_SIZE (0)
#define ICC_IQI_SIZE (SZ_4K)
#define ICC_NRIFC_SIZE (SZ_4K)
#define ICC_NRAPCPU_OSA_SIZE (SZ_128K)
#define ICC_NRLRCPU_OSA_SIZE (SZ_128K)
#define ICC_MNRCORE_SIZE (SZ_512)
#define ICC_ACORE2HIFI_SIZE (SZ_512)
#define ICC_RESERVED_STATIC_SIZE (SZ_512)


/* ddr共享内存通道地址定义, acore和ccore */
#define ICC_DBG_MSG_LEN_IN_DDR (0x20)
#define ICC_DBG_MSG_ADDR_IN_DDR (((SHM_ADDR_ICC) + 3) & ~3)
#define ADDR_IFC_SEND (ICC_DBG_MSG_ADDR_IN_DDR + ICC_DBG_MSG_LEN_IN_DDR)  /* C核发送，A核接收 */
#define ADDR_IFC_RECV (ADDR_IFC_SEND + STRU_SIZE + ICC_IFC_SIZE)          /* C核接收，A核发送 */
#define ADDR_RFILE_SEND (ADDR_IFC_RECV + STRU_SIZE + ICC_IFC_SIZE)        /* C核发送，A核接收 */
#define ADDR_RFILE_RECV (ADDR_RFILE_SEND + STRU_SIZE + ICC_RFILE_SIZE)    /* C核接收，A核发送 */
#define ADDR_NV_SEND (ADDR_RFILE_RECV + STRU_SIZE + ICC_RFILE_SIZE)       /* C核发送，A核接收 */
#define ADDR_NV_RECV (ADDR_NV_SEND + STRU_SIZE + ICC_NV_SIZE)             /* C核接收，A核发送 */
#define ADDR_GUOM0_SEND (ADDR_NV_RECV + STRU_SIZE + ICC_NV_SIZE)          /* C核发送，A核接收 */
#define ADDR_GUOM0_RECV (ADDR_GUOM0_SEND + STRU_SIZE + ICC_GUOM0_SIZE)    /* C核接收，A核发送 */
#define ADDR_GUOM1_SEND (ADDR_GUOM0_RECV + STRU_SIZE + ICC_GUOM0_SIZE)    /* C核发送，A核接收 */
#define ADDR_GUOM1_RECV (ADDR_GUOM1_SEND + STRU_SIZE + ICC_GUOM1_SIZE)    /* C核接收，A核发送 */
#define ADDR_GUOM2_SEND (ADDR_GUOM1_RECV + STRU_SIZE + ICC_GUOM1_SIZE)    /* C核发送，A核接收 */
#define ADDR_GUOM2_RECV (ADDR_GUOM2_SEND + STRU_SIZE + ICC_GUOM2_SIZE)    /* C核接收，A核发送 */
#define ADDR_GUOM3_SEND (ADDR_GUOM2_RECV + STRU_SIZE + ICC_GUOM2_SIZE)    /* C核发送，A核接收 */
#define ADDR_GUOM3_RECV (ADDR_GUOM3_SEND + STRU_SIZE + ICC_GUOM3_SIZE)    /* C核接收，A核发送 */
#define ADDR_GUOM4_SEND (ADDR_GUOM3_RECV + STRU_SIZE + ICC_GUOM3_SIZE)    /* C核发送，A核接收 */
#define ADDR_GUOM4_RECV (ADDR_GUOM4_SEND + STRU_SIZE + ICC_GUOM4_SIZE)    /* C核接收，A核发送 */
#define ADDR_GUOM5_SEND (ADDR_GUOM4_RECV + STRU_SIZE + ICC_GUOM4_SIZE)    /* C核发送，A核接收 */
#define ADDR_GUOM5_RECV (ADDR_GUOM5_SEND + STRU_SIZE + ICC_GUOM5_SIZE)    /* C核接收，A核发送 */
#define ADDR_CSHELL_SEND (ADDR_GUOM5_RECV + STRU_SIZE + ICC_GUOM5_SIZE)   /* C核发送，A核接收 */
#define ADDR_CSHELL_RECV (ADDR_CSHELL_SEND + STRU_SIZE + ICC_CSHELL_SIZE) /* C核接收，A核发送 */
#define ADDR_PANRPC_SEND (ADDR_CSHELL_RECV + STRU_SIZE + ICC_CSHELL_SIZE) /* C核发送，A核接收 */
#define ADDR_PANRPC_RECV (ADDR_PANRPC_SEND + STRU_SIZE + ICC_PANRPC_SIZE) /* C核接收，A核发送 */

/* ddr共享内存通道地址定义, 安全OS和ccore */
#ifdef SHM_SEC_BASE_ADDR
#define SHM_S_ADDR_ICC (SHM_SEC_BASE_ADDR + SHM_OFFSET_SEC_ICC)
#define SHM_S_SIZE_ICC (SHM_SIZE_SEC_ICC)
#else
#define SHM_S_ADDR_ICC 0
#define SHM_S_SIZE_ICC 0
#endif

#define ICC_DBG_MSG_LEN_IN_DDR_S (0x20)
#define ICC_DBG_MSG_ADDR_IN_DDR_S (((SHM_S_ADDR_ICC) + 3) & ~3)


#ifdef CONFIG_SEC_ICC /* 安全ICC */
/* 安全ICC通道地址定义, start */
#define ICC_DBG_MSG_LEN_IN_DDR_SEC (0x20)
#define ICC_DBG_MSG_ADDR_IN_DDR_SEC (((SHM_ADDR_SHA_SEC_ICC) + 3) & ~3)

#define ADDR_MCCORE_SEND (ICC_DBG_MSG_ADDR_IN_DDR_SEC + ICC_DBG_MSG_LEN_IN_DDR_SEC) /* M核发送，C核接收 Austin上使用SDDR */
#define ADDR_MCCORE_RECV (ADDR_MCCORE_SEND + STRU_SIZE + ICC_MCCORE_SIZE) /* M核接收，C核发送 */
#define ADDR_HIFI_VOS_MSG_SEND (ADDR_MCCORE_RECV + STRU_SIZE + ICC_MCCORE_SIZE)
#define ADDR_HIFI_VOS_MSG_RECV (ADDR_HIFI_VOS_MSG_SEND + STRU_SIZE + ICC_HIFI_VOS_SIZE)
#define ADDR_HIFI_TPHY_MSG_SEND (ADDR_HIFI_VOS_MSG_RECV + STRU_SIZE + ICC_HIFI_VOS_SIZE)
#define ADDR_HIFI_TPHY_MSG_RECV (ADDR_HIFI_TPHY_MSG_SEND + STRU_SIZE + ICC_HIFI_TPHY_SIZE)
#define ADDR_NRCPU_LRCPU_SEND (ADDR_HIFI_TPHY_MSG_RECV + STRU_SIZE + ICC_HIFI_TPHY_SIZE)
#define ADDR_NRCPU_LRCPU_RECV (ADDR_NRCPU_LRCPU_SEND + STRU_SIZE + ICC_NRLRCPU_OSA_SIZE)
#define ADDR_MNRCPU_SEND (ADDR_NRCPU_LRCPU_RECV + STRU_SIZE + ICC_NRLRCPU_OSA_SIZE)
#define ADDR_MNRCPU_RECV (ADDR_MNRCPU_SEND + STRU_SIZE + ICC_MNRCORE_SIZE)


#ifndef BSP_ICC_MCHANNEL_USE_LPM3TCM /* BALONGV7R2 */
/* SRAM(AXI)通道地址定义, mcore和ccore; mcore和acore */
#define ADDR_MACORE_SEND (SRAM_ADDR_ICC) /* M核发送，C核接收 */
#else /* K3V3 */
#define ADDR_MACORE_SEND (ADDR_PANRPC_RECV + STRU_SIZE + ICC_PANRPC_SIZE) /* M核发送，C核接收 Austin上使用SDDR */
#endif
#define ADDR_MACORE_RECV (ADDR_MACORE_SEND + STRU_SIZE + ICC_MACORE_SIZE) /* M核接收，A核发送 */

#ifndef BSP_ICC_MCHANNEL_USE_LPM3TCM
#define ADDR_VT_VOIP_CCORE_RECV_ACORE_SEND (ADDR_PANRPC_RECV + STRU_SIZE + ICC_PANRPC_SIZE)
#else
#define ADDR_VT_VOIP_CCORE_RECV_ACORE_SEND (ADDR_MACORE_RECV + STRU_SIZE + ICC_MACORE_SIZE)
#endif
#define ADDR_VT_VOIP_CCORE_SEND_ACORE_RECV (ADDR_VT_VOIP_CCORE_RECV_ACORE_SEND + STRU_SIZE + ICC_VT_VOIP_SIZE)
/* 安全ICC通道地址定义, end */
#else

#ifndef BSP_ICC_MCHANNEL_USE_LPM3TCM /* BALONGV7R2 */
/* SRAM(AXI)通道地址定义, mcore和ccore; mcore和acore */
#define ADDR_MCCORE_SEND (SRAM_ADDR_ICC) /* M核发送，C核接收 */
#else /* K3V3 */
#define ADDR_MCCORE_SEND (ADDR_PANRPC_RECV + STRU_SIZE + ICC_PANRPC_SIZE) /* M核发送，C核接收 Austin上使用SDDR */
#endif
#define ADDR_MCCORE_RECV (ADDR_MCCORE_SEND + STRU_SIZE + ICC_MCCORE_SIZE) /* M核接收，C核发送 */
#define ADDR_MACORE_SEND (ADDR_MCCORE_RECV + STRU_SIZE + ICC_MCCORE_SIZE) /* M核发送，A核接收 */
#define ADDR_MACORE_RECV (ADDR_MACORE_SEND + STRU_SIZE + ICC_MACORE_SIZE) /* M核接收，A核发送 */

#ifndef BSP_ICC_MCHANNEL_USE_LPM3TCM
#define ADDR_HIFI_VOS_MSG_SEND (ADDR_PANRPC_RECV + STRU_SIZE + ICC_PANRPC_SIZE)
#else
#define ADDR_HIFI_VOS_MSG_SEND (ADDR_MACORE_RECV + STRU_SIZE + ICC_MACORE_SIZE)
#endif
#define ADDR_HIFI_VOS_MSG_RECV (ADDR_HIFI_VOS_MSG_SEND + STRU_SIZE + ICC_HIFI_VOS_SIZE)
#define ADDR_HIFI_TPHY_MSG_SEND (ADDR_HIFI_VOS_MSG_RECV + STRU_SIZE + ICC_HIFI_VOS_SIZE)
#define ADDR_HIFI_TPHY_MSG_RECV (ADDR_HIFI_TPHY_MSG_SEND + STRU_SIZE + ICC_HIFI_TPHY_SIZE)

#define ADDR_VT_VOIP_CCORE_RECV_ACORE_SEND (ADDR_HIFI_TPHY_MSG_RECV + STRU_SIZE + ICC_HIFI_TPHY_SIZE)
#define ADDR_VT_VOIP_CCORE_SEND_ACORE_RECV (ADDR_VT_VOIP_CCORE_RECV_ACORE_SEND + STRU_SIZE + ICC_VT_VOIP_SIZE)

#endif

#define ADDR_IPC_EXTEND_SEND (ADDR_VT_VOIP_CCORE_SEND_ACORE_RECV + STRU_SIZE + ICC_VT_VOIP_SIZE)
#define ADDR_IPC_EXTEND_RECV (ADDR_IPC_EXTEND_SEND + STRU_SIZE + ICC_IPC_EXTEND_SIZE)

#define ADDR_DYNAMIC_SEND (ADDR_IPC_EXTEND_RECV + STRU_SIZE + ICC_IPC_EXTEND_SIZE)
#define ADDR_DYNAMIC_RECV (ADDR_DYNAMIC_SEND + STRU_SIZE + ICC_DYNAMIC_SIZE)

#define ADDR_HIFI_ACORE_MSG_SEND (ADDR_DYNAMIC_RECV + STRU_SIZE + ICC_DYNAMIC_SIZE)
#define ADDR_HIFI_ACORE_MSG_RECV (ADDR_HIFI_ACORE_MSG_SEND + STRU_SIZE + ICC_ACORE2HIFI_SIZE)

#define ICC_SDDR_DYNAMIC_START_ADDR_ON_THIS_CORE (ADDR_HIFI_ACORE_MSG_RECV + STRU_SIZE + ICC_RESERVED_STATIC_SIZE)

#ifdef CONFIG_NRICC
#define NRSHM_ADDR_ICC (NRSHM_BASE_ADDR + NRSHM_OFFSET_NRICC)

/* nr ddr共享内存通道地址定义, acore和nrccore */
#define NRICC_DBG_MSG_LEN_IN_NRSDDR (0x20)
#define NRICC_DBG_MSG_ADDR_IN_NRSDDR (((NRSHM_ADDR_ICC) + 3) & ~3)
#endif


#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /*  __BSP_ICC_H__ */
