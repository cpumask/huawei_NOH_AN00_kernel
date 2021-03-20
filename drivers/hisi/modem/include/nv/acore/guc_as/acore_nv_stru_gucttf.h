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

#ifndef __ACORE_NV_STRU_GUCTTF_H__
#define __ACORE_NV_STRU_GUCTTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "vos.h"
#include "acore_nv_id_gucttf.h"

#pragma pack(push, 4)

#define FC_ACPU_DRV_ASSEM_NV_LEV 4

enum FC_AcpuDrvAssemLev {
    FC_ACPU_DRV_ASSEM_LEV_1    = 0,
    FC_ACPU_DRV_ASSEM_LEV_2    = 1,
    FC_ACPU_DRV_ASSEM_LEV_3    = 2,
    FC_ACPU_DRV_ASSEM_LEV_4    = 3,
    FC_ACPU_DRV_ASSEM_LEV_5    = 4,
    FC_ACPU_DRV_ASSEM_LEV_BUTT = 5
};
typedef VOS_UINT32 FC_AcpuDrvAssemLevUint32;

enum FC_MemThresholdLev {
    FC_MEM_THRESHOLD_LEV_1    = 0,
    FC_MEM_THRESHOLD_LEV_2    = 1,
    FC_MEM_THRESHOLD_LEV_3    = 2,
    FC_MEM_THRESHOLD_LEV_4    = 3,
    FC_MEM_THRESHOLD_LEV_5    = 4,
    FC_MEM_THRESHOLD_LEV_6    = 5,
    FC_MEM_THRESHOLD_LEV_7    = 6,
    FC_MEM_THRESHOLD_LEV_8    = 7,
    FC_MEM_THRESHOLD_LEV_BUTT = 8
};
typedef VOS_UINT32 FC_MemThresholdLevUint32;

enum TTF_AcoreBool {
    TTF_ACORE_FALSE = 0,
    TTF_ACORE_TRUE  = 1,

    TTF_ACORE_BOOL_BUTT
};
typedef VOS_UINT8 TTF_AcoreBoolUint8;

/* 根据CPU LOAD动态高速驱动组包参数 */
typedef struct {
    VOS_UINT8 hostOutTimeout; /* PC驱动组包时间，取值范围0~50，单位us。 */
    VOS_UINT8 ethTxMinNum;    /* 单板下行组包个数，取值范围1~100。 */
    VOS_UINT8 ethTxTimeout;   /* 单板下行组包超时时间，取值范围1~20，单位ms。 */
    VOS_UINT8 ethRxMinNum;    /* 单板上行组包个数，取值范围1~20。 */
    VOS_UINT8 ethRxTimeout;   /* 单板上行组包超时时间，取值范围1~50，单位ms。 */
    VOS_UINT8 cdsGuDlThres;   /* 该参数已经废弃。 */
    VOS_UINT8 rsv[2];         /* 4字节对齐，预留。 */
} FC_DrvAssemPara;

/* 根据CPU LOAD动态高速驱动组包参数 */
typedef struct {
    VOS_UINT32      cpuLoad;      /* CPU负载，取值范围0~100。 */
    FC_DrvAssemPara drvAssemPara; /* 根据cpuLoad动态高速驱动组包参数 */
} FC_CpuDrvAssemPara;

/* FC_AcoreCfgCpu结构,CPU流控的门限和配置值 */
typedef struct {
    VOS_UINT32 cpuOverLoadVal;  /* CPU启流控水线，取值范围[1,100]，表示CPU占用率大小。 */
    VOS_UINT32 cpuUnderLoadVal; /* CPU停流控水线，取值范围[1,cpuOverLoadVal]，表示CPU占用率大小。 */
    VOS_UINT32 smoothTimerLen;  /* CPU流控启动平滑次数，取值范围[2,1000]，单位：CPU监控周期。 */
    VOS_UINT32 stopAttemptTimerLen; /* CPU流控达到最高级后，启动定时器尝试解流控，该定时器的时长，单位毫秒，取值范围大于等于0，0表示不使用。 */
    VOS_UINT32 umUlRateThreshold; /* 上行速率水线，上行速率超此水线才可以启CPU流控，单位bps，取值范围大于等于0。 */
    VOS_UINT32 umDlRateThreshold; /* 下行速率水线，下行速率超此水线才可以启CPU流控，单位bps，取值范围大于等于0。 */
    VOS_UINT32 rmRateThreshold;   /* 网桥速率水线，网桥速率超此水线才可以启CPU流控，单位bps，取值范围大于等于0。 */
} FC_AcoreCfgCpu;

/* FC_AcoreCfgMemThreshold结构,MEM流控的门限和配置值 */
typedef struct {
    VOS_UINT32 setThreshold;  /* 内存启流控水线。 */
    VOS_UINT32 stopThreshold; /* 内存停流控水线。 */
} FC_AcoreCfgMemThreshold;

/* FC_CfgMemThresholdCst结构,MEM流控的门限和配置值 */
typedef struct {
    VOS_UINT32 setThreshold;                     /* Range:[0,4096], 启动流控门限 单位字节 */
    VOS_UINT32 stopThreshold;                    /* Range:[0,4096], 停止流控门限 单位字节 */
} FC_CfgMemThresholdCst;

/*
 * 对应NV_ITEM_LINUX_CPU_MONITOR_TIMER_LEN项的结构,A核CPU占用率监测任务定时检测时长
 * 结构说明  : A核CPU占用率监测任务定时检测时长。
 * 向A核CPU流控上报CPU负载，可以根据A核流控模块CPU监测周期的需要，调整该时长。
 * 注意：
 * 本NV所有参数未经Balong同意，禁止修改！
 */
typedef struct {
    VOS_UINT32 monitorTimerLen; /* A核CPU占用率监测任务定时检测时长，单位：毫秒，取值范围100-1000。 */
} CPULOAD_Cfg;

/*
 * 对应NV_ITEM_FC_ACPU_DRV_ASSEMBLE_PARA项的结构，根据CPU LOAD动态高速驱动组包参数
 * 结构说明  : 根据CPU LOAD动态高速驱动组包参数。
 */
typedef struct {
    /*
     * 使能控制，值0表示不使能，值1表示使能。
     * Bit0：单板使能；
     * Bit1：PC使能。
     */
    VOS_UINT8          enableMask;
    VOS_UINT8          smoothCntUpLev;                            /* 向上调整平滑系数，值需要≥0。 */
    VOS_UINT8          smoothCntDownLev;                          /* 向下调整平滑系数，值需要≥0。 */
    VOS_UINT8          rsv;                                       /* 保留。 */
    FC_CpuDrvAssemPara cpuDrvAssemPara[FC_ACPU_DRV_ASSEM_NV_LEV]; /* CPU配置参数 */
} FC_CpuDrvAssemParaNv;

/*
 * 对应NV_ITEM_NETFILTER_HOOK_MASK项的结构,设置勾包点的NV项
 * 结构说明  : 设置勾包点的NV项，预留5种掩码组合：netfilterPara1-netfilterPara5，每个掩码取值范围为0-FFFFFFFF,
 * 其中掩码位为1则代表该掩码位对应的钩子函数可能会被注册到内核中
 */
typedef struct {
    VOS_UINT32 netfilterPara1; /* 钩子函数掩码参数1，对应的是网桥和ARP的钩子函数，当存在网桥时，掩码值设置为100，当没有网桥时，掩码值设置为96。 */
    VOS_UINT32 netfilterPara2; /* 钩子函数掩码参数2，对应的是IP协议栈入口处的钩子函数，取值为4224。 */
    VOS_UINT32 netfilterPara3; /* 钩子函数掩码参数3，对应的是IP协议栈出口处的钩子函数，取值为8448。 */

    VOS_UINT32 netfilterPara4; /* 钩子函数掩码参数4，对应的是网桥流控的钩子函数，当存在网桥流控时，掩码值设置为131072，当没有网桥流控时，掩码值设置为0。 */
    VOS_UINT32 netfilterPara5; /* 钩子函数掩码参数5，预留，掩码值设置为0。 */
} NfExtNv;

/* 结构说明  : CST流控门限 */
typedef struct {
    FC_AcoreCfgMemThreshold threshold; /* CST环形缓存队列流控水线，单位字节，总长4096。 */
} FC_CfgCst;

/* 结构说明  : MEM流控的门限和配置值 */
typedef struct {
    VOS_UINT32              thresholdCnt;                         /* A核内存流控档位个数，取值范围[0,8]。 */
    FC_AcoreCfgMemThreshold threshold[FC_MEM_THRESHOLD_LEV_BUTT]; /* CST环形缓存队列流控水线，单位字节，总长4096。 */
} FC_CfgMem;

/*
 * 对应NV_ITEM_ACORE_FLOW_CRTL_CONFIG项的结构
 * 结构说明  : 保存A核流控功能的配置信息。
 * 使用：可以根据需要打开或关闭流控功能，以及调整A核CPU、内存、CST的流控水线。
 */
typedef struct {
    /*
     * A核流控使能开关。每个比特位置0表示该项流控去使能，1表示使能。
     * Bit0：MEM流控；
     * Bit1：A核CPU流控；
     * Bit2：CDS流控；
     * Bit3：CST流控；
     */
    VOS_UINT32     fcEnableMask;
    FC_AcoreCfgCpu fcCfgCpuA; /* A核CPU流控水线配置。 */
    FC_CfgMem      fcCfgMem;  /* A核内存流控水线配置。 */
    FC_CfgCst      fcCfgCst;  /* CST业务流控水线配置 */
} FC_AcoreCfgNv;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
