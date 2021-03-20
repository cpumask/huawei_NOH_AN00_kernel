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


#include "product_config.h"
#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
#include "fc.h"
#include "fc_interface.h"
#include "fc_cds_interface.h"
#include "fc_cst_interface.h"
#include "at_fc_interface.h"
#include "ads_fc_interface.h"
#include "acpuload_interface.h"
#include "imm_interface.h"
#include "nfext_interface.h"
#include "fc_intra_msg.h"
#include "PsTypeDef.h"
#include "ps_common_def.h"
#include "ps_lib.h"
#include "mdrv_nvim.h"
#include "fc_acore_c_reset.h"
#include "ttf_util.h"
#include "fc_acore.h"
#include "ttf_comm.h"
#include "gucttf_tag.h"
#include "securec.h"
#include "ps_trace_msg.h"

#define THIS_FILE_ID PS_FILE_ID_ACORE_FLOW_CTRL_C
#define THIS_MODU mod_fc

/* A核流控配置实体 */
FC_AcoreCfgNv g_fcAcoreCfg;

/* 网桥速率统计使用的变量 */
FC_BridgeRate g_fcBridgeRate;

/* CDS流控使用的全局变量，FC IdRAB映射关系 */
FC_RabMappingInfoSet g_fcRabMappingInfoSet[MODEM_ID_BUTT];

/* CPU流控使用，包括平滑次数，定时器 */
FC_CpuCtrl g_fcCpuaCtrl;

FC_CpuDrvAssemConfigPara g_cpuDriverAssePara =
    {
        6,
        10,
        5,
        0,
        {
            /* cpuload, PC驱动组包时延，UE驱动TX组包个数, UE驱动TX时延，UE驱动RX组包个数，UE驱动RX组包时延，保留域 */
            {
                85, { 20, 40, 10, 10, 10, 56, { 0, 0 }}
            },
            {
                70, { 15, 20, 10, 10, 10, 20, { 0, 0 }}
            },
            {
                60, { 10, 10, 10, 10, 10, 5, { 0, 0 }}
            },
            {
                40, { 5, 5, 10, 10, 10, 1, { 0, 0 }}
            },
            {
                0, { 0, 1, 1, 1, 1, 1, { 0, 0 }}
            }
        }
    };

/* 驱动组包参数实体结构 */
FC_CpuDrvAssemParaEntity g_drvAssemParaEntity = { 0, 0, 0, 0, 0, { 0, 0, 0, 0 }, { 100, { 0, 0, 0, 0, 0, 0, { 0, 0 }}},
    VOS_NULL_PTR, VOS_NULL_PTR };

VOS_SPINLOCK g_fcMemSpinLock;

STATIC VOS_VOID FC_BridgeResetRate(VOS_VOID);
#if (FEATURE_NFEXT == FEATURE_ON)
STATIC VOS_UINT32 FC_BridgeGetRate(VOS_VOID);
STATIC VOS_UINT32 FC_RmRateJudge(VOS_VOID);
#endif
STATIC VOS_VOID FC_DrvAssemInit(VOS_VOID);
STATIC VOS_VOID FC_JudgeAssemSmoothFactor(const FC_CpuDrvAssemPara *drvAssemPara);
STATIC FC_CpuDrvAssemPara* FC_GetCpuDrvAssemPara(VOS_UINT32 lev);
STATIC FC_CpuDrvAssemPara* FC_GetCurrentAssemPara(VOS_UINT32 assemLev);
STATIC VOS_VOID FC_JudgeDrvAssemAction(VOS_UINT32 assemLev);

STATIC FC_CpuDrvAssemPara* FC_SelectDrvAssemParaRule(VOS_UINT32 cpuLoad, VOS_UINT32 *assemLev);
STATIC VOS_VOID FC_JudgeDrvAssemPara(VOS_UINT32 cpuLoad);

STATIC VOS_UINT32 FC_PsRateJudge(VOS_VOID);
STATIC VOS_VOID FC_GetPsRate(VOS_UINT32 *ulRate, VOS_UINT32 *dlRate);
STATIC VOS_UINT32 FC_CpuaDownJudge(VOS_UINT32 cpuLoad, FC_CFG_CPU_STRU *fcCfgCpu, FC_Policy *fcPolicy);

VOS_UINT32 FC_CpuaStopFcAttempt(VOS_UINT32 param1, VOS_UINT32 param2);

STATIC VOS_UINT32 FC_CpuaUpProcess(VOS_VOID);
STATIC VOS_UINT32 FC_CpuaDownProcess(VOS_VOID);
STATIC VOS_UINT32 FC_CpuaStopFlowCtrl(VOS_VOID);

STATIC VOS_UINT32 FC_CpuaInit(VOS_VOID);

STATIC VOS_UINT32 FC_MEM_AdjustPriForDown(FC_PriTypeUint8 pointPri, FC_IdUint8 fcId);

STATIC VOS_UINT32 FC_MEM_SndUpToTargetPriIndMsg(FC_PriTypeUint8 targetPri, VOS_UINT16 memFreeCnt);
STATIC VOS_UINT32 FC_MEM_SndDownToTargetPriIndMsg(FC_PriTypeUint8 targetPri, VOS_UINT16 memFreeCnt);

STATIC VOS_UINT32 FC_CstUpProcess(VOS_UINT8 rabId);
STATIC VOS_UINT32 FC_CstDownProcess(VOS_UINT8 rabId);
STATIC FC_RabMappingInfo* FC_CdsGetFcInfo(VOS_UINT8 rabId, ModemIdUint16 modemId);

STATIC VOS_UINT32 FC_CdsInit(VOS_VOID);

STATIC VOS_VOID FC_UpProcess(VOS_RatModeUint32 rateMode);
STATIC VOS_VOID FC_DownProcess(VOS_RatModeUint32 rateMode);

STATIC VOS_UINT32 FC_RcvCstMsg(MsgBlock *msg);
STATIC VOS_UINT32 FC_RcvCdsMsg(MsgBlock *msg);
STATIC VOS_UINT32 FC_AcoreRcvTimerMsg(REL_TimerMsgBlock *timerMsg);
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
STATIC VOS_UINT32 FC_CdmaDownProcess(VOS_VOID);
STATIC VOS_UINT32 FC_CdmaUpProcess(VOS_VOID);
#endif
STATIC VOS_UINT32 FC_GprsDownProcess(VOS_VOID);

STATIC VOS_UINT32 FC_GprsUpProcess(VOS_VOID);

#if (FEATURE_NFEXT == FEATURE_ON)
#if (FEATURE_ACPU_STAT == FEATURE_ON)
STATIC VOS_VOID FC_BridgeCalcRate(VOS_UINT32 period)
{
    VOS_UINT32 currentByteCnt;
    VOS_UINT32 lastByteCnt;
    VOS_UINT32 rate;
    VOS_UINT32 deltaPacketCnt;

    if (period == 0) {
        g_fcBridgeRate.rate = 0;
        return;
    }

    lastByteCnt    = g_fcBridgeRate.lastByteCnt;
    currentByteCnt = NFEXT_GetBrBytesCnt();
    deltaPacketCnt = (currentByteCnt - lastByteCnt);

    /* 换算成bps,注意防止计算溢出 */
    if (deltaPacketCnt < period) {
        rate = (deltaPacketCnt * 1000 * 8) / period;
    } else {
        rate = ((deltaPacketCnt * 8) / period) * 1000;
    }

    g_fcBridgeRate.lastByteCnt = currentByteCnt;
    g_fcBridgeRate.rate        = rate;

    return;
}
#endif

/* 清除网桥速率统计 */
STATIC VOS_VOID FC_BridgeResetRate(VOS_VOID)
{
    g_fcBridgeRate.lastByteCnt = NFEXT_GetBrBytesCnt();
    g_fcBridgeRate.rate        = 0;
}

STATIC VOS_UINT32 FC_BridgeGetRate(VOS_VOID)
{
    return (g_fcBridgeRate.rate);
}

/* Rm速率是否达到引发CPU流控的门限 */
STATIC VOS_UINT32 FC_RmRateJudge(VOS_VOID)
{
    /* 如果网桥速率超过门限，则认为是速率高引起CPU高 */
    if (g_fcBridgeRate.rate > g_fcAcoreCfg.fcCfgCpuA.rmRateThreshold) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
#else

/* 清除网桥速率统计 */
STATIC VOS_VOID FC_BridgeResetRate(VOS_VOID)
{
    return;
}

/* 空口速率是否达到引发CPU流控的门限 */
STATIC VOS_UINT32 FC_UmRateOverThreshold(VOS_VOID)
{
    VOS_UINT32 ulRate = 0;
    VOS_UINT32 dlRate = 0;

    /* 获取UM口速率, ADS获取速率接口被删除，后续启用该功能时寻找新接口 */

    /* 如果上行或下行速率超过门限，则认为是速率高引起CPU高 */
    if ((ulRate > g_fcAcoreCfg.fcCfgCpuA.umUlRateThreshold) || (dlRate > g_fcAcoreCfg.fcCfgCpuA.umDlRateThreshold)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#endif

/* 注册驱动组包参数调整回调函数 */
VOS_UINT32 FC_AcoreRegDrvAssemFunc(FC_AcoreDrvAssembleParaFunc fcDrvSetAssemParaFuncUe,
    FC_AcoreDrvAssembleParaFunc fcDrvSetAssemParaFuncPc)
{
    /* ADS接口已经删除，后续启用该功能时寻找适合的接口 */
    return VOS_OK;
}


/* 驱动组包参数初始化，从NV项中读出值 */
STATIC VOS_VOID FC_DrvAssemInit(VOS_VOID)
{
    FC_CpuDrvAssemParaNv cpuDrvAssemPara = {0};
    VOS_UINT32           rst;

    rst = GUCTTF_NV_Read(
        MODEM_ID_0, NV_ITEM_FC_ACPU_DRV_ASSEMBLE_PARA, &cpuDrvAssemPara, (VOS_UINT32)sizeof(FC_CpuDrvAssemParaNv));

    /* NV如果读取失败使用默认值 */
    if (rst != NV_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_DrvAssemInit Read NV fail!\n");

        return;
    }

    g_drvAssemParaEntity.curAssemPara.cpuLoad = 100;
    g_drvAssemParaEntity.curLev               = FC_ACPU_DRV_ASSEM_LEV_1;
    g_drvAssemParaEntity.setDrvFailCnt        = 0;

    /* NV的长度与结构体的长度定义不一样只拷NV的部分 */
    TTF_ACORE_SF_CHK(
        memcpy_s(&g_cpuDriverAssePara, sizeof(g_cpuDriverAssePara), &cpuDrvAssemPara, sizeof(cpuDrvAssemPara)));
}

/* 调整平滑系数 */
STATIC VOS_VOID FC_JudgeAssemSmoothFactor(const FC_CpuDrvAssemPara *drvAssemPara)
{
    /* 平滑系数计算，连续多次超过才做出调整 */
    if (g_drvAssemParaEntity.curAssemPara.cpuLoad > drvAssemPara->cpuLoad) {
        /* 下调档位平滑 */
        g_drvAssemParaEntity.smoothCntDown++;
        g_drvAssemParaEntity.smoothCntUp = 0;
    } else if (g_drvAssemParaEntity.curAssemPara.cpuLoad < drvAssemPara->cpuLoad) {
        /* 上调档位平滑 */
        g_drvAssemParaEntity.smoothCntUp++;
        g_drvAssemParaEntity.smoothCntDown = 0;
    } else {
        /* 如果需要调整档已经与当前相同，平滑系数清零 */
        g_drvAssemParaEntity.smoothCntUp   = 0;
        g_drvAssemParaEntity.smoothCntDown = 0;
    }
}

/* 根据当前档位获取参数 */
STATIC FC_CpuDrvAssemPara* FC_GetCpuDrvAssemPara(VOS_UINT32 lev)
{
    if (lev >= FC_ACPU_DRV_ASSEM_LEV_BUTT) {
        return VOS_NULL_PTR;
    }

    return &g_cpuDriverAssePara.cpuDrvAssemPara[lev];
}

/* 获取当前ASSEM参数 */
STATIC FC_CpuDrvAssemPara* FC_GetCurrentAssemPara(VOS_UINT32 assemLev)
{
    FC_CpuDrvAssemPara *drvAssemPara = VOS_NULL_PTR;

    /* 向上调整一步调到位 */
    if (g_drvAssemParaEntity.smoothCntUp >= g_cpuDriverAssePara.smoothCntUpLev) {
        g_drvAssemParaEntity.curLev = assemLev;
    } else if (g_drvAssemParaEntity.smoothCntDown >= g_cpuDriverAssePara.smoothCntDownLev) { /* 向下调整一个档位一个档位调整 */
        if (g_drvAssemParaEntity.curLev < (FC_ACPU_DRV_ASSEM_LEV_BUTT - 1)) {
            g_drvAssemParaEntity.curLev++;
        }
    } else {
        return VOS_NULL_PTR;
    }

    drvAssemPara = FC_GetCpuDrvAssemPara(g_drvAssemParaEntity.curLev);

    return drvAssemPara;
}

/* 输出当前调整组包方案 */
VOS_UINT32 FC_MNTN_TraceDrvAssemPara(FC_DrvAssemPara *drvAssenPara)
{
    FC_MntnDrvAssemPara fcMntnDrvAssemPara = {0};

    if (drvAssenPara == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    TTF_SET_MSG_SENDER_ID(&fcMntnDrvAssemPara, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_RECEIVER_ID(&fcMntnDrvAssemPara, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_LEN(&fcMntnDrvAssemPara, sizeof(FC_MntnDrvAssemPara) - VOS_MSG_HEAD_LENGTH);

    fcMntnDrvAssemPara.msgName = ID_FC_MNTN_DRV_ASSEM_PARA;
    (VOS_VOID)memcpy_s(
        &fcMntnDrvAssemPara.drvAssemPara, sizeof(FC_DrvAssemPara), drvAssenPara, sizeof(FC_DrvAssemPara));

    FC_MNTN_TraceEvent(&fcMntnDrvAssemPara);

    return VOS_OK;
}

/* 对驱动进行调整操作 */
STATIC VOS_VOID FC_DoJudgeDrvAssem(FC_CpuDrvAssemPara *drvAssemPara)
{
    /* 使能开关打开 */
    if ((FC_ACPU_DRV_ASSEM_PC_ON_MASK & g_cpuDriverAssePara.enableMask) == FC_ACPU_DRV_ASSEM_PC_ON_MASK) {
        if (g_drvAssemParaEntity.drvSetAssemParaFuncPc != VOS_NULL_PTR) {
            /* 调用回调函数调整驱动组包参数，这里调整PC侧组包方案 */
            if (g_drvAssemParaEntity.drvSetAssemParaFuncPc(&(drvAssemPara->drvAssemPara)) != VOS_OK) {
                FC_LOG(PS_PRINT_ERROR, "Set Driver Assemble parameter fail!\n");
                /* 设置失败计数 */
                g_drvAssemParaEntity.setDrvFailCnt++;

                return;
            }
        }
    }

    /* 组包系数清零 */
    g_drvAssemParaEntity.smoothCntUp   = 0;
    g_drvAssemParaEntity.smoothCntDown = 0;

    /* 设置失败次数清零 */
    g_drvAssemParaEntity.setDrvFailCnt = 0;

    /* 保存当前档位信息 */
    (VOS_VOID)memcpy_s(&g_drvAssemParaEntity.curAssemPara, sizeof(g_drvAssemParaEntity.curAssemPara), drvAssemPara,
        sizeof(FC_CpuDrvAssemPara));

    /* 可维可测 */
    FC_MNTN_TraceDrvAssemPara(&(drvAssemPara->drvAssemPara));
}

/* 对驱动进行调整 */
STATIC VOS_VOID FC_JudgeDrvAssemAction(VOS_UINT32 assemLev)
{
    FC_CpuDrvAssemPara *drvAssemPara = VOS_NULL_PTR;

    drvAssemPara = FC_GetCurrentAssemPara(assemLev);

    /* 超过平滑系数的水线之后再做出调整 */
    if (drvAssemPara != VOS_NULL_PTR) {
        FC_DoJudgeDrvAssem(drvAssemPara);
    }
}

/* 将驱动调整到最大档位 */
VOS_VOID FC_JudgeDrvToMaxPara(VOS_VOID)
{
    FC_CpuDrvAssemPara *drvAssemPara = VOS_NULL_PTR;

    /* 直接调整到最大档位 */
    g_drvAssemParaEntity.curLev = FC_ACPU_DRV_ASSEM_LEV_1;

    drvAssemPara = FC_GetCpuDrvAssemPara(FC_ACPU_DRV_ASSEM_LEV_1);

    FC_DoJudgeDrvAssem(drvAssemPara);
}

/* 通过CPULOAD的情况选择参数档位 */
STATIC FC_CpuDrvAssemPara* FC_SelectDrvAssemParaRule(VOS_UINT32 cpuLoad, VOS_UINT32 *assemLev)
{
    FC_CpuDrvAssemPara *cpuDrvAssemParaRst = VOS_NULL_PTR;
    FC_CpuDrvAssemPara *cpuDrvAssemPara    = VOS_NULL_PTR;
    VOS_INT             i;

    /* 选出当前需要调整档位 */
    for (i = 0; i < FC_ACPU_DRV_ASSEM_LEV_BUTT; i++) {
        cpuDrvAssemPara = &g_cpuDriverAssePara.cpuDrvAssemPara[i];
        if (cpuLoad >= cpuDrvAssemPara->cpuLoad) {
            cpuDrvAssemParaRst = cpuDrvAssemPara;
            *assemLev          = (VOS_UINT32)i;

            break;
        }
    }

    return cpuDrvAssemParaRst;
}

/* 通过CPULOAD的情况调整驱动组包参数数 */
STATIC VOS_VOID FC_JudgeDrvAssemPara(VOS_UINT32 cpuLoad)
{
    const FC_CpuDrvAssemPara *drvAssemPara = VOS_NULL_PTR;
    /* 默认最高档 */
    VOS_UINT32 assemLev = FC_ACPU_DRV_ASSEM_LEV_1;

    /* 使能开关未打开时，不再继续下面步骤 */
    if (g_cpuDriverAssePara.enableMask == 0) {
        return;
    }

    if ((g_drvAssemParaEntity.setDrvFailCnt % CPU_MAX_SET_DRV_FAIL_SMOOTH_CNT) != 0) {
        g_drvAssemParaEntity.setDrvFailCnt++;

        return;
    }

    /* 根据CPU负载情况选择对应档位 */
    drvAssemPara = FC_SelectDrvAssemParaRule(cpuLoad, &assemLev);

    if (drvAssemPara == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_JudgeDrvAssemPara pstDrvAssemPara is NULL!\n");

        return;
    }

    /* 根据当前档位计算平滑因子 */
    FC_JudgeAssemSmoothFactor(drvAssemPara);

    /* 对驱动做出调整 */
    FC_JudgeDrvAssemAction(assemLev);
}

/* 判断是否有数传 */
STATIC VOS_UINT32 FC_PsRateJudge(VOS_VOID)
{
    /* STICK形态判断空口速率E5形态判断网桥速率 */
#if (FEATURE_NFEXT == FEATURE_ON)
    return (FC_RmRateJudge());
#else
    return (FC_UmRateOverThreshold());
#endif
}

/* 获取数传速率 */
STATIC VOS_VOID FC_GetPsRate(VOS_UINT32 *ulRate, VOS_UINT32 *dlRate)
{
    /* E5形态下，获取网桥速率 */
#if (FEATURE_NFEXT == FEATURE_ON)
    /* 网桥上，上下行速率都赋值成网桥速率 */
    *ulRate = FC_BridgeGetRate();
    *dlRate = *ulRate;
#else
    /* STICK形态下，获取网桥速率 ADS获取速率接口被删除，后续启用该功能时寻找新接口 */
    *ulRate = 0;
    *dlRate = 0;
#endif
}

/* 是否触发CPU流控，第一次会做平滑处理 */
VOS_UINT32 FC_CpuaUpJudge(VOS_UINT32 cpuLoad, FC_CFG_CPU_STRU *fcCfgCpu, FC_Policy *fcPolicy)
{
    VOS_UINT32 result;

    if (cpuLoad < fcCfgCpu->cpuOverLoadVal) {
        g_fcCpuaCtrl.smoothTimerLen = 0;
        return VOS_FALSE;
    }

    /* 已经达到CPU流控的最高级别，不用再进一步流控 */
    if (fcPolicy->highestPri <= fcPolicy->donePri) {
        return VOS_FALSE;
    }

    /* 如果已经开始进行CPU流控，则继续进行流控，不再进行平滑和速率的判断 */
    if (fcPolicy->donePri != FC_PRI_NULL) {
        return VOS_TRUE;
    }

    if (g_fcCpuaCtrl.smoothTimerLen == 0) {
        FC_BridgeResetRate();
    }

    g_fcCpuaCtrl.smoothTimerLen++;

    if (g_fcCpuaCtrl.smoothTimerLen < fcCfgCpu->smoothTimerLen) {
        return VOS_FALSE;
    }

#if (FEATURE_ACPU_STAT == FEATURE_ON)
    /* A核ulSmoothTimerLen >= 2，详见FC_CFG_CheckParam函数内注释 */
    FC_BridgeCalcRate((fcCfgCpu->ulSmoothTimerLen - 1) * CPULOAD_GetRegularTimerLen());
#endif

    g_fcCpuaCtrl.smoothTimerLen = 0;

    /* 数传速率是否是CPU高的原因 */
    result = FC_PsRateJudge();
    if (result == VOS_FALSE) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/* 是否触发CPU解除流控 */
STATIC VOS_UINT32 FC_CpuaDownJudge(VOS_UINT32 cpuLoad, FC_CFG_CPU_STRU *fcCfgCpu, FC_Policy *fcPolicy)
{
    if ((cpuLoad < fcCfgCpu->cpuUnderLoadVal) && (fcPolicy->donePri > FC_PRI_NULL)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

/* A核流控模块提供的CPU负载处理接口 */
VOS_VOID FC_CpuaRcvCpuLoad(VOS_UINT32 cpuLoad)
{
    FC_CFG_CPU_STRU *fcCfgCpu = VOS_NULL_PTR;
    FC_Policy       *fcPolicy = VOS_NULL_PTR;
    VOS_UINT32       startCtrl;
    VOS_UINT32       ulRate;
    VOS_UINT32       dlRate;

    /* 通过CPU LOAD调整驱动组包参数，不受CPU流控开关控制 */
    FC_JudgeDrvAssemPara(cpuLoad);

    /* 参数检查 */
    if ((FC_POLICY_MASK_CPU_A & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CPU_A) {
        /* CPU流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CPU_ProcessLoad, INFO, CPU FlowCtrl is disabled <1>\n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return;
    }

    /* 输出可维可测 */
    FC_MNTN_TraceCpuLoad(ID_FC_MNTN_CPU_A_CPULOAD, cpuLoad);

    if (cpuLoad > 100) {
        /* 参数非法 */
        FC_LOG1(PS_PRINT_WARNING, "FC_CPU_ProcessLoad, WARNING, Invalid Cpu Load <1>\n", (VOS_INT32)cpuLoad);
        return;
    }

    fcCfgCpu = &(g_fcAcoreCfg.fcCfgCpuA);
    fcPolicy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);

    /* 是否要进行CPU流控 */
    startCtrl = FC_CpuaUpJudge(cpuLoad, fcCfgCpu, fcPolicy);
    if (startCtrl == VOS_TRUE) {
        FC_GetPsRate(&ulRate, &dlRate);
        FC_SndCpuMsg(ID_FC_CPU_A_OVERLOAD_IND, cpuLoad, ulRate, dlRate);
        return;
    }

    /* 进行CPU解流控判决 */
    startCtrl = FC_CpuaDownJudge(cpuLoad, fcCfgCpu, fcPolicy);
    if (startCtrl == VOS_TRUE) {
        FC_GetPsRate(&ulRate, &dlRate);
        FC_SndCpuMsg(ID_FC_CPU_A_UNDERLOAD_IND, cpuLoad, ulRate, dlRate);
        return;
    }

    return;
}

/* 尝试解除CPU流控 */
VOS_UINT32 FC_CpuaStopFcAttempt(VOS_UINT32 param1, VOS_UINT32 param2)
{
    FC_CFG_CPU_STRU *cfgCpu = VOS_NULL_PTR;

    cfgCpu = &(g_fcAcoreCfg.fcCfgCpuA);
    if (cfgCpu->stopAttemptTimerLen == 0) {
        return VOS_OK;
    }

#if (FEATURE_ACPU_STAT == FEATURE_ON)
    CPULOAD_ResetUserDefLoad();
#endif

    /* 如果定时器已经开启，则不需要再次启动 */
    if (g_fcCpuaCtrl.stopAttemptTHandle == VOS_NULL_PTR) {
        if (VOS_StartRelTimer(&g_fcCpuaCtrl.stopAttemptTHandle, UEPS_PID_FLOWCTRL, cfgCpu->stopAttemptTimerLen,
                FC_TIMER_STOP_CPU_ATTEMPT, 0, VOS_RELTIMER_NOLOOP, VOS_TIMER_PRECISION_0) != VOS_OK) {
            FC_LOG(PS_PRINT_WARNING, "FC_CpuaStopFcAttempt, VOS_StartRelTimer Fail!\n");
            return VOS_ERR;
        }
    }

    /* for lint 715 */
    (VOS_VOID)param1;
    (VOS_VOID)param2;

    return VOS_OK;
}

/* ACPU负载的流控处理 */
STATIC VOS_UINT32 FC_CpuaUpProcess(VOS_VOID)
{
    FC_Policy *fcPolicy = VOS_NULL_PTR;

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    fcPolicy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);
    FC_PolicyUp(fcPolicy);

    /* 已经是当前策略的最高级别了，执行回调函数 */
    if (fcPolicy->donePri == fcPolicy->highestPri) {
        if (fcPolicy->postFunc != VOS_NULL_PTR) {
            (VOS_VOID)fcPolicy->postFunc(FC_POLICY_ID_CPU_A, 0);
        }
    }

    return VOS_OK;
}

/* ACPU负载的解除流控处理 */
STATIC VOS_UINT32 FC_CpuaDownProcess(VOS_VOID)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);
    FC_PolicyDown(policy);

    return VOS_OK;
}

/* 根据CPU负载，判读是否可以提前解除CPU流控 */
STATIC VOS_UINT32 FC_CpuaStopFlowCtrl(VOS_VOID)
{
    FC_CFG_CPU_STRU *fcCfgCpu = VOS_NULL_PTR;
    FC_Policy       *fcPolicy = VOS_NULL_PTR;
    VOS_UINT32       cpuLoad  = 0;

    fcCfgCpu = &(g_fcAcoreCfg.fcCfgCpuA);
    fcPolicy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);

#if (FEATURE_ACPU_STAT == FEATURE_ON)
    cpuLoad = CPULOAD_GetUserDefLoad();
#endif

    /* 当前CPU小于解流控门限值,解流控,FEATURE_ACPU_STAT没打开ulCpuLoad为0所以第一个判断恒成立 */
    /*lint --e(685) */
    if ((cpuLoad <= fcCfgCpu->cpuUnderLoadVal) && (fcPolicy->donePri > FC_PRI_NULL)) {
        FC_CpuaDownProcess();
    }

    return VOS_OK;
}

/* ACPU负载流控的初始化 */
STATIC VOS_UINT32 FC_CpuaInit(VOS_VOID)
{
    /* 增加使用宏开关判断是否注册回调函数 */
#if (FEATURE_ACPU_STAT == FEATURE_ON)
    /* 向CPU监测模块注册回调函数 */
    if (CPULOAD_RegRptHook((CPULOAD_RptHookFunc)FC_CpuaRcvCpuLoad) != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_CpuaInit, ERROR, CPULOAD_RegRptHook return error!\n");
        return VOS_ERR;
    }

    /* A核CPU流控策略初始化 */
    g_fcPolicy[FC_POLICY_ID_CPU_A].postFunc = FC_CpuaStopFcAttempt;
#endif

    (VOS_VOID)memset_s(&g_fcBridgeRate, sizeof(FC_BridgeRate), 0, sizeof(FC_BridgeRate));
    (VOS_VOID)memset_s(&g_fcCpuaCtrl, sizeof(g_fcCpuaCtrl), 0, sizeof(g_fcCpuaCtrl));

    return VOS_OK;
}

/*
 * 内存递增时，根据内存占用量，计算出内存流控的目标流控级别，
 * 内存策略只有一个优先级时，按照Lev3处理
 */
FC_PriTypeUint8 FC_MEM_CalcUpTargetFcPri(FC_Policy *policy, VOS_UINT32 memValue)
{
    FC_PriTypeUint8 targetPri;
    FC_CfgMem      *memCfg = VOS_NULL_PTR;

    memCfg    = &(g_fcAcoreCfg.fcCfgMem);
    targetPri = policy->donePri;

    /* 内存策略只注册了一个优先级，按照Lev3处理 */
    if (policy->priCnt == 1) {
        /* 计算出目标流控优先级 */
        if (memValue <= memCfg->threshold[FC_MEM_THRESHOLD_LEV_3].setThreshold) {
            targetPri = policy->highestPri;
        }
    } else { /* 内存策略只注册了多个优先级 */
        /* 计算出目标流控优先级 */
        if (memValue <= memCfg->threshold[FC_MEM_THRESHOLD_LEV_4].setThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_4;
        } else if (memValue <= memCfg->threshold[FC_MEM_THRESHOLD_LEV_3].setThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_3;
        } else if (memValue <= memCfg->threshold[FC_MEM_THRESHOLD_LEV_2].setThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_2;
        } else if (memValue <= memCfg->threshold[FC_MEM_THRESHOLD_LEV_1].setThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_1;
        } else {
            ;
        }
    }

    return targetPri;
}

/*
 * 内存递减时，根据内存占用量，计算出内存流控的目标解流控级别，
 * 内存策略只有一个优先级时，按照Lev3处理
 */
FC_PriTypeUint8 FC_MEM_CalcDownTargetFcPri(FC_Policy *policy, VOS_UINT32 memValue)
{
    FC_PriTypeUint8 targetPri;
    FC_CfgMem      *memCfg = VOS_NULL_PTR;

    memCfg    = &(g_fcAcoreCfg.fcCfgMem);
    targetPri = policy->donePri;

    /* 内存策略只注册了一个优先级 */
    if (policy->priCnt == 1) {
        /* 计算出目标流控优先级 */
        if (memValue > memCfg->threshold[FC_MEM_THRESHOLD_LEV_3].stopThreshold) {
            targetPri = FC_PRI_NULL;
        }
    } else { /* 内存策略只注册了多个优先级 */
        /* 计算出目标流控优先级 */
        if (memValue > memCfg->threshold[FC_MEM_THRESHOLD_LEV_1].stopThreshold) {
            targetPri = FC_PRI_NULL;
        } else if (memValue > memCfg->threshold[FC_MEM_THRESHOLD_LEV_2].stopThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_1;
        } else if (memValue > memCfg->threshold[FC_MEM_THRESHOLD_LEV_3].stopThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_2;
        } else if (memValue > memCfg->threshold[FC_MEM_THRESHOLD_LEV_4].stopThreshold) {
            targetPri = FC_PRI_FOR_MEM_LEV_3;
        } else {
            ;
        }
    }

    return targetPri;
}

STATIC VOS_UINT32 FC_MEM_AdjustPriForUp(FC_PriTypeUint8 pointPri, FC_IdUint8 fcId)
{
    FC_Policy      *policy = VOS_NULL_PTR;
    FC_PriTypeUint8 targetPri;

    /* 获取内存流控策略和内存流控配置 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);

    /* 根据内存情况，重新刷新内存流控目标优先级 */
    targetPri = FC_MEM_CalcUpTargetFcPri(policy, 0);

    /* 目标优先级调整后，进行流控处理调整，符合新的流控优先级 */
    if (policy->donePri < targetPri) {
        FC_PolicyUpToTargetPri(policy, targetPri);
    } else if (policy->donePri > targetPri) {
        FC_PolicyDownToTargetPri(policy, targetPri);
    } else {
        ;
    }

    /* for lint 715 */
    (VOS_VOID)pointPri;
    (VOS_VOID)fcId;

    return VOS_OK;
}

STATIC VOS_UINT32 FC_MEM_AdjustPriForDown(FC_PriTypeUint8 pointPri, FC_IdUint8 fcId)
{
    FC_Policy      *policy = VOS_NULL_PTR;
    FC_PriTypeUint8 targetPri;

    /* 获取内存流控策略和内存流控配置 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);

    /* 根据内存情况，重新刷新内存流控目标优先级 */
    targetPri = FC_MEM_CalcDownTargetFcPri(policy, 0);

    /* 目标优先级调整后，进行流控处理调整，符合新的流控优先级 */
    if (policy->donePri < targetPri) {
        FC_PolicyUpToTargetPri(policy, targetPri);
    } else if (policy->donePri > targetPri) {
        FC_PolicyDownToTargetPri(policy, targetPri);
    } else {
        ;
    }

    /* for lint 715 */
    (VOS_VOID)pointPri;
    (VOS_VOID)fcId;

    return VOS_OK;
}

/* 发送内存流控到目标值的消息通知 */
STATIC VOS_UINT32 FC_MEM_SndUpToTargetPriIndMsg(FC_PriTypeUint8 targetPri, VOS_UINT16 memFreeCnt)
{
    FC_MemUpToTargetPriInd *msg = VOS_NULL_PTR;
    VOS_UINT32              result;

    msg = (FC_MemUpToTargetPriInd*)(VOS_UINT_PTR)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL, (VOS_UINT32)sizeof(FC_MemUpToTargetPriInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemUpToTargetPriIndMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName    = ID_FC_MEM_UP_TO_TARGET_PRI_IND;
    msg->targetPri  = targetPri;
    msg->memFreeCnt = memFreeCnt;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemUpToTargetPriIndMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 发送内存流控到目标值的消息通知 */
STATIC VOS_UINT32 FC_MEM_SndDownToTargetPriIndMsg(FC_PriTypeUint8 targetPri, VOS_UINT16 memFreeCnt)
{
    FC_MemDownToTargetPriInd *msg = VOS_NULL_PTR;
    VOS_UINT32                result;

    msg = (FC_MemDownToTargetPriInd*)(VOS_UINT_PTR)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL, (VOS_UINT32)sizeof(FC_MemDownToTargetPriInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemDownToTargetPriIndMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName    = ID_FC_MEM_DOWN_TO_TARGET_PRI_IND;
    msg->targetPri  = targetPri;
    msg->memFreeCnt = memFreeCnt;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemDownToTargetPriIndMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * 内存流控策略入口函数
 * 输入参数:ulMemValue:内存个数
 */
VOS_VOID FC_MEM_UpProcess(VOS_UINT32 memValue)
{
    FC_Policy      *policy = VOS_NULL_PTR;
    FC_PriTypeUint8 targetPri;
    VOS_ULONG       flags = 0UL;

    /* 参数检查 */
    if ((FC_POLICY_MASK_MEM & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_MEM) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_MEM_UpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return;
    }

    policy    = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);
    targetPri = FC_MEM_CalcUpTargetFcPri(policy, memValue);

    if ((policy->donePri < targetPri) && (policy->toPri < targetPri)) {
        /*lint --e(506,774,550,730) */
        /*lint --e{550,1072} */
        if (likely(preemptible())) { /* 如果在软中断这类不可打断的任务中，就不发流控消息 */
            VOS_SpinLockIntLock(&g_fcMemSpinLock, flags);
            policy->toPri = targetPri;
            VOS_SpinUnlockIntUnlock(&g_fcMemSpinLock, flags);

            /* 消息发送失败会单板复位，不再进行enToPri的恢复 */
            FC_MEM_SndUpToTargetPriIndMsg(targetPri, (VOS_UINT16)memValue);
        }
    }

    return;
}

/* 内存使用量减少，可能触发解除流控 */
VOS_VOID FC_MEM_DownProcess(VOS_UINT32 memValue)
{
    FC_Policy      *policy = VOS_NULL_PTR;
    FC_PriTypeUint8 targetPri;
    VOS_ULONG       flags = 0UL;

    /* 参数检查 */
    if ((FC_POLICY_MASK_MEM & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_MEM) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_MEM_UpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return;
    }

    policy    = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);
    targetPri = FC_MEM_CalcDownTargetFcPri(policy, memValue);

    if ((policy->donePri > targetPri) && (policy->toPri > targetPri)) {
        /*lint --e(506,774,550,730) */
        /*lint --e{550,1072} */
        if (likely(preemptible())) { /* 如果在软中断这类不可打断的任务中，就不发流控消息 */
            VOS_SpinLockIntLock(&g_fcMemSpinLock, flags);
            policy->toPri = targetPri;
            VOS_SpinUnlockIntUnlock(&g_fcMemSpinLock, flags);

            /* 消息发送失败会单板复位，不再进行enToPri的恢复 */
            FC_MEM_SndDownToTargetPriIndMsg(targetPri, (VOS_UINT16)memValue);
        }
    }

    return;
}

/* 内存流控初始化 */
STATIC VOS_UINT32 FC_MEM_Init(VOS_VOID)
{
    VOS_SpinLockInit(&g_fcMemSpinLock);

    /* 注册内存回调函数 */
    if ((FC_POLICY_MASK(FC_POLICY_ID_MEM) == (FC_POLICY_MASK(FC_POLICY_ID_MEM) & g_fcAcoreCfg.fcEnableMask))) {
        /* A核内存流控策略初始化 */
        g_fcPolicy[FC_POLICY_ID_MEM].adjustForUpFunc   = FC_MEM_AdjustPriForUp;
        g_fcPolicy[FC_POLICY_ID_MEM].adjustForDownFunc = FC_MEM_AdjustPriForDown;
    } else {
        /* A核内存流控策略初始化 */
        g_fcPolicy[FC_POLICY_ID_MEM].adjustForUpFunc   = VOS_NULL_PTR;
        g_fcPolicy[FC_POLICY_ID_MEM].adjustForDownFunc = VOS_NULL_PTR;
    }

    return VOS_OK;
}

/* CST流控策略 */
STATIC VOS_UINT32 FC_CstUpProcess(VOS_UINT8 rabId)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CST & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CST) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CstUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CST_MODEM_0);
    FC_PolicyUpToTargetPri(policy, policy->highestPri);

    /* for lint 715 */
    (VOS_VOID)rabId;

    return VOS_OK;
}

/* CST解除流控策略 */
STATIC VOS_UINT32 FC_CstDownProcess(VOS_UINT8 rabId)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CST & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CST) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CstDownProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CST_MODEM_0);
    FC_PolicyDownToTargetPri(policy, FC_PRI_NULL);

    /* for lint 715 */
    (VOS_VOID)rabId;

    return VOS_OK;
}

/* 获取Fc流控实体 */
STATIC FC_RabMappingInfo* FC_CdsGetFcInfo(VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_RabMappingInfoSet *fcRabMappingInfoSet = VOS_NULL_PTR;
    FC_RabMappingInfo    *fcRabMappingInfo    = VOS_NULL_PTR;
    FC_IdUint8            fcId;
    VOS_UINT32            rabMask;

    fcRabMappingInfoSet = &g_fcRabMappingInfoSet[modemId];
    rabMask             = (VOS_UINT32)(1UL << rabId);

    /* 遍历所有映射关系，如果找到包含指定RAB_ID的映射关系，则退出 */
    for (fcId = 0; fcId < fcRabMappingInfoSet->fcIdCnt; fcId++) {
        fcRabMappingInfo = &(fcRabMappingInfoSet->fcRabMappingInfo[fcId]);
        if ((rabMask & fcRabMappingInfo->includeRabMask) != 0) {
            return fcRabMappingInfo;
        }
    }

    return VOS_NULL_PTR;
}

/* 删除FCId对应的FC信息 */
VOS_UINT32 FC_CdsDelFcId(FC_IdUint8 fcId, ModemIdUint16 modemId)
{
    FC_RabMappingInfoSet *fcRabMappingInfoSet = VOS_NULL_PTR;
    FC_IdUint8            fcIdNum;
    FC_IdUint8            shiftFcId;

    fcRabMappingInfoSet = &g_fcRabMappingInfoSet[modemId];

    for (fcIdNum = 0; fcIdNum < fcRabMappingInfoSet->fcIdCnt; fcIdNum++) {
        if (fcId == fcRabMappingInfoSet->fcRabMappingInfo[fcIdNum].fcId) {
            break;
        }
    }

    if (fcIdNum >= fcRabMappingInfoSet->fcIdCnt) {
        FC_LOG1(PS_PRINT_WARNING, "FC_CdsDelFcId, can not find the Fc Id <1>!\n", (VOS_INT32)fcId);
        return VOS_ERR;
    }

    /* 将后面的映射关系拷贝过来，保持数组连续性 */
    for (shiftFcId = (FC_PriTypeUint8)(fcIdNum + 1); shiftFcId < fcRabMappingInfoSet->fcIdCnt; shiftFcId++) {
        fcRabMappingInfoSet->fcRabMappingInfo[shiftFcId - 1] = fcRabMappingInfoSet->fcRabMappingInfo[shiftFcId];
    }

    /* 最后一个映射关系无法被拷贝覆盖，所以需要手动清除 */
    (VOS_VOID)memset_s(&(fcRabMappingInfoSet->fcRabMappingInfo[shiftFcId - 1]), sizeof(FC_RabMappingInfo),
        0, sizeof(FC_RabMappingInfo));
    fcRabMappingInfoSet->fcIdCnt--;

    return VOS_OK;
}

/* 增加ClentId到RAB映射 */
VOS_UINT32 FC_CdsAddRab(FC_IdUint8 fcId, VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_RabMappingInfoSet *fcRabMappingInfoSet = VOS_NULL_PTR;
    FC_RabMappingInfo    *fcRabMappingInfo    = VOS_NULL_PTR;
    FC_IdUint8            fcIdNum;
    VOS_UINT32            rabMask;

    fcRabMappingInfoSet = &g_fcRabMappingInfoSet[modemId];
    rabMask             = (VOS_UINT32)(1UL << rabId);

    if (fcRabMappingInfoSet->fcIdCnt > FC_MAX_NUM) {
        FC_LOG1(PS_PRINT_ERROR, "FC_CdsAddRab, g_fcRabMappingInfoSet is exceed the ranger!\n",
            (VOS_INT32)fcRabMappingInfoSet->fcIdCnt);
        return VOS_ERR;
    }

    /* 遍历该FC的流控信息，如果找到包含指定RAB_ID的Fc Id，则退出 */
    for (fcIdNum = 0; fcIdNum < fcRabMappingInfoSet->fcIdCnt; fcIdNum++) {
        fcRabMappingInfo = &(fcRabMappingInfoSet->fcRabMappingInfo[fcIdNum]);
        if (fcId == fcRabMappingInfo->fcId) {
            if ((fcRabMappingInfo->includeRabMask & rabMask) == 0) {
                fcRabMappingInfo->includeRabMask |= rabMask;
                fcRabMappingInfo->noFcRabMask |= rabMask;
                (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK_CDS, fcId);
            }

            return VOS_OK;
        }
    }

    if (fcRabMappingInfoSet->fcIdCnt == FC_MAX_NUM) {
        FC_LOG(PS_PRINT_ERROR, "FC_CdsAddRab, AtClientCnt reaches the max num!\n");
        return VOS_ERR;
    }

    /* 如果是新的FC Id，则添加该Fc Id和RAB ID */
    fcRabMappingInfo                 = &(fcRabMappingInfoSet->fcRabMappingInfo[fcRabMappingInfoSet->fcIdCnt]);
    fcRabMappingInfo->fcId           = fcId;
    fcRabMappingInfo->includeRabMask = rabMask;
    fcRabMappingInfo->noFcRabMask    = rabMask;

    fcRabMappingInfoSet->fcIdCnt++;

    return VOS_OK;
}

/* 清除该RABID对应的流控信息 */
VOS_UINT32 FC_CdsDelRab(VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_RabMappingInfo *fcRabMappingInfo = VOS_NULL_PTR;
    VOS_UINT32         rabMask;

    /* 通过Rab Id来查找该Rab Id所在的FC实体 */
    fcRabMappingInfo = FC_CdsGetFcInfo(rabId, modemId);
    if (fcRabMappingInfo == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* 清除该RabId的信息，如果该Fc上已经不存在不需要流控的RabId，则进行流控 */
    rabMask = (VOS_UINT32)(1UL << rabId);

    fcRabMappingInfo->includeRabMask &= (~rabMask);

    if (fcRabMappingInfo->noFcRabMask != 0) {
        fcRabMappingInfo->noFcRabMask &= (~rabMask);
        if ((fcRabMappingInfo->noFcRabMask == 0) && (fcRabMappingInfo->includeRabMask != 0)) {
            (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK_CDS, fcRabMappingInfo->fcId);
        }
    }

    /* 如果删除了RabId后，该FC上没有其它信息，则删除该FC ID对应的信息 */
    if (fcRabMappingInfo->includeRabMask == 0) {
        FC_CdsDelFcId(fcRabMappingInfo->fcId, modemId);
    }

    return VOS_OK;
}

/* ATClient流控初始化 */
STATIC VOS_UINT32 FC_CdsInit(VOS_VOID)
{
    (VOS_VOID)memset_s(g_fcRabMappingInfoSet, sizeof(g_fcRabMappingInfoSet), 0, sizeof(g_fcRabMappingInfoSet));

    return VOS_OK;
}

/* CDS流控策略 */
VOS_UINT32 FC_CdsUpProcess(VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_RabMappingInfo *fcRabMappingInfo = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CDS & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CDS) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CstUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_WARNING, "FC_CdsUpProcess ModemId Is Invalid <1>\n", modemId);
        return VOS_ERR;
    }

    /* 通过RAB id来查找 Client流控实体 */
    fcRabMappingInfo = FC_CdsGetFcInfo(rabId, modemId);
    if (fcRabMappingInfo == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* 如果该Fc流控实体已经流控，则无需处理，直接返回 */
    if (fcRabMappingInfo->noFcRabMask == 0) {
        return VOS_OK;
    }

    /* 去除该FC对应的没有流控的RAB掩码，如果为0，表明所有RAB都要求流控，该FC进行流控 */
    fcRabMappingInfo->noFcRabMask &= (~((VOS_UINT32)1 << rabId));
    if (fcRabMappingInfo->noFcRabMask == 0) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK_CDS, fcRabMappingInfo->fcId);
    }

    return VOS_OK;
}

/* CDS解除流控策略 */
VOS_UINT32 FC_CdsDownProcess(VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_RabMappingInfo *fcRabMappingInfo = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CDS & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CDS) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CdsDownProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_WARNING, "FC_CdsDownProcess ModemId Is Invalid <1>\n", modemId);
        return VOS_ERR;
    }

    /* 通过RAB id来查找 FC实体 */
    fcRabMappingInfo = FC_CdsGetFcInfo(rabId, modemId);
    if (fcRabMappingInfo == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* 如果该FC实体已经解除流控，则无需处理，直接返回 */
    if (fcRabMappingInfo->noFcRabMask != 0) {
        fcRabMappingInfo->noFcRabMask |= ((VOS_UINT32)1 << rabId);
        return VOS_OK;
    }

    /* 增加v对应的没有流控的RAB掩码，如果为0，则该FC实体s需要进行解流控 */
    fcRabMappingInfo->noFcRabMask |= ((VOS_UINT32)1 << rabId);
    (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK_CDS, fcRabMappingInfo->fcId);

    return VOS_OK;
}

/* 处理流控请求 */
STATIC VOS_VOID FC_UpProcess(VOS_RatModeUint32 rateMode)
{
    switch (rateMode) {
        case VOS_RATMODE_GSM:

            FC_GprsUpProcess();

            break;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case VOS_RATMODE_1X:
        case VOS_RATMODE_HRPD:

            FC_CdmaUpProcess();

            break;
#endif
        default:

            FC_LOG1(PS_PRINT_WARNING, "FC_UpProcess RateMode Is Invalid <1>\n", (VOS_INT32)rateMode);

            break;
    }

    return;
}

/* 处理GPRS流控请求 */
STATIC VOS_UINT32 FC_GprsUpProcess(VOS_VOID)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_GPRS & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_GPRS) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_GprsUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_GPRS_MODEM_0);
    FC_PolicyUpToTargetPri(policy, policy->highestPri);

    return VOS_OK;
}

/* 处理流控请求 */
STATIC VOS_VOID FC_DownProcess(VOS_RatModeUint32 rateMode)
{
    switch (rateMode) {
        case VOS_RATMODE_GSM:

            FC_GprsDownProcess();

            break;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case VOS_RATMODE_1X:
        case VOS_RATMODE_HRPD:

            FC_CdmaDownProcess();

            break;
#endif
        default:

            FC_LOG1(PS_PRINT_WARNING, "FC_DownProcess RateMode Is Invalid <1>\n", (VOS_INT32)rateMode);

            break;
    }

    return;
}

/* 处理GPRS解除流控请求 */
STATIC VOS_UINT32 FC_GprsDownProcess(VOS_VOID)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_GPRS & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_GPRS) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_GprsUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_GPRS_MODEM_0);
    FC_PolicyDownToTargetPri(policy, FC_PRI_NULL);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
/* 处理CDMA流控请求 */
STATIC VOS_UINT32 FC_CdmaUpProcess(VOS_VOID)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CDMA & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CDMA) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CdmaUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CDMA_MODEM_0);
    FC_PolicyUpToTargetPri(policy, policy->highestPri);

    return VOS_OK;
}

/* 处理CDMA解除流控请求 */
STATIC VOS_UINT32 FC_CdmaDownProcess(VOS_VOID)
{
    FC_Policy *policy = VOS_NULL_PTR;

    /* 使能检查 */
    if ((FC_POLICY_MASK_CDMA & g_fcAcoreCfg.fcEnableMask) != FC_POLICY_MASK_CDMA) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_CdmaUpProcess, INFO, MEM FlowCtrl is disabled <1> \n",
            (VOS_INT32)g_fcAcoreCfg.fcEnableMask);
        return VOS_OK;
    }

    /* 获取CPU流控策略实体，并调用通用流控策略 */
    policy = FC_POLICY_GET(FC_PRIVATE_POLICY_ID_CDMA_MODEM_0);
    FC_PolicyDownToTargetPri(policy, FC_PRI_NULL);

    return VOS_OK;
}
#endif
/* A核流控模块提供接口给AT配置通道与RABID映射关系 */
VOS_VOID FC_ChannelMapCreate(FC_IdUint8 fcId, VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_AddRabFcIdMapInd *msg = VOS_NULL_PTR;
    VOS_UINT32           result;

    /* 参数检查，RabId范围为[5,15] */
    if ((rabId < FC_UE_MIN_RAB_ID) || (rabId > FC_UE_MAX_RAB_ID)) {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapCreate RabId Is Invalid <1>\n", rabId);
        return;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapCreate ModemId Is Invalid <1>\n", modemId);
        return;
    }

    /* 发送消息 */
    msg = (FC_AddRabFcIdMapInd*)(VOS_UINT_PTR)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL_A, (VOS_UINT32)sizeof(FC_AddRabFcIdMapInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapCreate, Alloc Msg Fail\n");
        return;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName = ID_FC_ADD_RAB_FCID_MAP_IND;
    msg->fcId    = fcId;
    msg->rabId   = rabId;
    msg->modemId = modemId;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL_A, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapCreate, Send Msg Fail\n");
        return;
    }
}

/* 当有一个RABID释放时，AT通过该接口删除流控模块映射关系 */
VOS_VOID FC_ChannelMapDelete(VOS_UINT8 rabId, ModemIdUint16 modemId)
{
    FC_DelRabFcIdMapInd *msg = VOS_NULL_PTR;
    VOS_UINT32           result;

    /* 参数检查，RabId范围为[5,15] */
    if ((rabId < FC_UE_MIN_RAB_ID) || (rabId > FC_UE_MAX_RAB_ID)) {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapDelete RabId Is Invalid <1>\n", rabId);
        return;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapDelete ModemId Is Invalid <1>\n", modemId);
        return;
    }

    /* 发送消息 */
    msg = (FC_DelRabFcIdMapInd*)(VOS_UINT_PTR)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL_A, (VOS_UINT32)sizeof(FC_DelRabFcIdMapInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapDelete, Alloc Msg Fail\n");
        return;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName = ID_FC_DEL_RAB_FCID_MAP_IND;
    msg->rabId   = rabId;
    msg->modemId = modemId;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL_A, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapDelete, Send Msg Fail\n");
        return;
    }
}

/* 接收CST消息 */
STATIC VOS_UINT32 FC_RcvCstMsg(MsgBlock *msg)
{
    /*lint --e(826) 屏蔽宏里面从UINT8指针转成UINT32指针时产生的告警 */
    switch (FC_GET_MSG_NAME32(msg)) {
        case ID_CST_FC_SET_FLOWCTRL_REQ:
            FC_CstUpProcess(((CST_FC_SetFlowctrlReq*)(VOS_UINT_PTR)msg)->rabId);
            break;

        case ID_CST_FC_STOP_FLOWCTRL_REQ:
            FC_CstDownProcess(((CST_FC_StopFlowctrlReq*)(VOS_UINT_PTR)msg)->rabId);
            break;

        default:
            break;
    }

    return VOS_OK;
}

/* 接收处理CDS消息 */
STATIC VOS_UINT32 FC_RcvCdsMsg(MsgBlock *msg)
{
    /*lint --e(826) 屏蔽宏里面从UINT8指针转成UINT32指针时产生的告警 */
    switch (FC_GET_MSG_NAME32(msg)) {
        case ID_CDS_FC_STOP_CHANNEL_IND:
            FC_CdsUpProcess(((CDS_FC_StopChannelInd*)(VOS_UINT_PTR)msg)->rabId,
                ((CDS_FC_StopChannelInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        case ID_CDS_FC_START_CHANNEL_IND:
            FC_CdsDownProcess(((CDS_FC_StartChannelInd*)(VOS_UINT_PTR)msg)->rabId,
                ((CDS_FC_StartChannelInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        default:
            break;
    }

    return VOS_OK;
}

/* 初始化时，注册一些固定的流控点 */
STATIC VOS_UINT32 FC_AcoreRegPoint(VOS_VOID)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_OFF)
/* V9R1项目中增加使用宏开关判断FC模块是否需要进行流控点注册 */
#if (FEATURE_NFEXT == FEATURE_ON)
    FC_RegPointPara fcRegPoint = {0};

    /* CPU负载流控点注册 */
    fcRegPoint.policyId = FC_POLICY_ID_CPU_A;
    fcRegPoint.fcPri    = FC_PRI_FOR_BRIDGE_FORWARD_DISCARD;
    fcRegPoint.fcId     = FC_ID_BRIDGE_FORWARD_DISACRD;
    fcRegPoint.param1   = 0;
    fcRegPoint.param2   = 0;
    fcRegPoint.setFunc  = NFEXT_BrSetFlowCtrl;
    fcRegPoint.clrFunc  = NFEXT_BrStopFlowCtrl;
    fcRegPoint.rstFunc  = VOS_NULL_PTR;
    FC_PointReg(&fcRegPoint);

    fcRegPoint.policyId = FC_POLICY_ID_CPU_A;
    fcRegPoint.fcPri    = FC_PRI_HIGHEST;
    fcRegPoint.fcId     = FC_ID_WIFI_ETH;
    fcRegPoint.param1   = 0;
    fcRegPoint.param2   = 0;
    fcRegPoint.setFunc  = DRV_WIFI_SET_RX_FCTL;
    fcRegPoint.clrFunc  = DRV_WIFI_CLR_RX_FCTL;
    fcRegPoint.rstFunc  = VOS_NULL_PTR;
    FC_PointReg(&fcRegPoint);

    /* 内存流控点注册 */
    fcRegPoint.policyId = FC_POLICY_ID_MEM;
    fcRegPoint.fcPri    = FC_PRI_FOR_BRIDGE_FORWARD_DISCARD;
    fcRegPoint.fcId     = FC_ID_BRIDGE_FORWARD_DISACRD;
    fcRegPoint.param1   = 0;
    fcRegPoint.param2   = 0;
    fcRegPoint.setFunc  = NFEXT_BrSetFlowCtrl;
    fcRegPoint.clrFunc  = NFEXT_BrStopFlowCtrl;
    fcRegPoint.rstFunc  = VOS_NULL_PTR;
    FC_PointReg(&fcRegPoint);

    fcRegPoint.policyId = FC_POLICY_ID_MEM;
    fcRegPoint.fcPri    = FC_PRI_FOR_MEM_LEV_4;
    fcRegPoint.fcId     = FC_ID_WIFI_ETH;
    fcRegPoint.param1   = 0;
    fcRegPoint.param2   = 0;
    fcRegPoint.setFunc  = DRV_WIFI_SET_RX_FCTL;
    fcRegPoint.clrFunc  = DRV_WIFI_CLR_RX_FCTL;
    fcRegPoint.rstFunc  = VOS_NULL_PTR;
    FC_PointReg(&fcRegPoint);

#endif
#endif

    return VOS_OK;
}

/* 接收处理定时器消息 */
STATIC VOS_UINT32 FC_AcoreRcvTimerMsg(REL_TimerMsgBlock *timerMsg)
{
    switch (timerMsg->name) {
        case FC_TIMER_STOP_CPU_ATTEMPT:
            FC_CpuaStopFlowCtrl();
            break;

        default:
            break;
    }

    return VOS_OK;
}

/* 流控流控消息处理函数 */
VOS_UINT32 FC_AcoreRcvIntraMsg(MsgBlock *msg)
{
    /*lint --e(826) 屏蔽宏里面从UINT8指针转成UINT16指针时产生的告警 */
    switch (FC_GET_MSG_NAME16(msg)) {
        case ID_FC_REG_POINT_IND:
            FC_PointReg(&(((FC_RegPointInd*)(VOS_UINT_PTR)msg)->fcPoint));
            break;

        case ID_FC_DEREG_POINT_IND:
            FC_PointDeReg(
                ((FC_DeregPointInd*)(VOS_UINT_PTR)msg)->fcId, ((FC_DeregPointInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        case ID_FC_CHANGE_POINT_IND:
            FC_PointChange(((FC_ChangePointInd*)(VOS_UINT_PTR)msg)->fcId,
                ((FC_ChangePointInd*)(VOS_UINT_PTR)msg)->policyId, ((FC_ChangePointInd*)(VOS_UINT_PTR)msg)->pri,
                ((FC_ChangePointInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        case ID_FC_CPU_A_OVERLOAD_IND:
            FC_CpuaUpProcess();
            break;

        case ID_FC_CPU_A_UNDERLOAD_IND:
            FC_CpuaDownProcess();
            break;

        case ID_FC_SET_FLOWCTRL_IND:
            FC_UpProcess(((FC_SetFlowCtrlInd*)(VOS_UINT_PTR)msg)->rateMode);
            break;

        case ID_FC_STOP_FLOWCTRL_IND:
            FC_DownProcess(((FC_SetFlowCtrlInd*)(VOS_UINT_PTR)msg)->rateMode);
            break;

        case ID_FC_ADD_RAB_FCID_MAP_IND:
            FC_CdsAddRab(((FC_AddRabFcIdMapInd*)(VOS_UINT_PTR)msg)->fcId,
                ((FC_AddRabFcIdMapInd*)(VOS_UINT_PTR)msg)->rabId, ((FC_AddRabFcIdMapInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        case ID_FC_DEL_RAB_FCID_MAP_IND:
            FC_CdsDelRab(
                ((FC_DelRabFcIdMapInd*)(VOS_UINT_PTR)msg)->rabId, ((FC_DelRabFcIdMapInd*)(VOS_UINT_PTR)msg)->modemId);
            break;

        case ID_FC_MEM_UP_TO_TARGET_PRI_IND:
            FC_PolicyUpToTargetPri(
                &(g_fcPolicy[FC_POLICY_ID_MEM]), ((FC_MemUpToTargetPriInd*)(VOS_UINT_PTR)msg)->targetPri);
            break;

        case ID_FC_MEM_DOWN_TO_TARGET_PRI_IND:
            FC_PolicyDownToTargetPri(
                &(g_fcPolicy[FC_POLICY_ID_MEM]), ((FC_MemDownToTargetPriInd*)(VOS_UINT_PTR)msg)->targetPri);
            break;

        case ID_FC_ACORE_CRESET_START_IND:
            FC_AcoreCResetProc(FC_ACORE_CRESET_BEFORE_RESET);
            break;

        case ID_FC_ACORE_CRESET_END_IND:
            FC_AcoreCResetProc(FC_ACORE_CRESET_AFTER_RESET);
            break;

        case ID_FC_ACORE_CRESET_START_RSP:
            FC_AcoreCResetRcvStartRsp();
            break;

        default:
            break;
    }

    return VOS_OK;
}

/* 流控消息处理函数 */
VOS_VOID FC_AcoreMsgProc(MsgBlock *msg)
{
    switch (TTF_GET_MSG_SENDER_ID(msg)) {
        case UEPS_PID_FLOWCTRL_A:
        case UEPS_PID_FLOWCTRL_C:
            FC_AcoreRcvIntraMsg(msg);
            break;

        case UEPS_PID_CST:
            FC_RcvCstMsg(msg);
            break;

        case UEPS_PID_CDS:
            FC_RcvCdsMsg(msg);
            break;

        case VOS_PID_TIMER:
            FC_AcoreRcvTimerMsg((REL_TimerMsgBlock*)(VOS_UINT_PTR)msg);
            break;

        default:
            break;
    }
}

/* 设置A核NV默认值 */
STATIC VOS_UINT32 FC_AcoreCfgSetDefaultValue(FC_AcoreCfgNv *acoreFcCfg)
{
    FC_LOG(PS_PRINT_WARNING, "FC_CFG_SetDefaultValue, Set Default NV Value.\n");

    (VOS_VOID)memset_s(acoreFcCfg, sizeof(FC_AcoreCfgNv), 0x0, sizeof(FC_AcoreCfgNv));
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_CDS;
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_CST;
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_GPRS;
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_TMP;
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_CPU_C;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    acoreFcCfg->fcEnableMask |= FC_POLICY_MASK_CDMA;
#endif
    FC_SetFcEnableMask(acoreFcCfg->fcEnableMask);

    acoreFcCfg->fcCfgCpuA.cpuOverLoadVal      = 95;
    acoreFcCfg->fcCfgCpuA.cpuUnderLoadVal     = 70;
    acoreFcCfg->fcCfgCpuA.smoothTimerLen      = 8;
    acoreFcCfg->fcCfgCpuA.stopAttemptTimerLen = 100;
    acoreFcCfg->fcCfgCpuA.umUlRateThreshold   = 1048576;
    acoreFcCfg->fcCfgCpuA.umDlRateThreshold   = 10485760;
    acoreFcCfg->fcCfgCpuA.rmRateThreshold     = 10485760;

    acoreFcCfg->fcCfgMem.thresholdCnt               = 4;
    acoreFcCfg->fcCfgMem.threshold[0].setThreshold  = 300;
    acoreFcCfg->fcCfgMem.threshold[0].stopThreshold = 350;
    acoreFcCfg->fcCfgMem.threshold[1].setThreshold  = 250;
    acoreFcCfg->fcCfgMem.threshold[1].stopThreshold = 300;
    acoreFcCfg->fcCfgMem.threshold[2].setThreshold  = 150;
    acoreFcCfg->fcCfgMem.threshold[2].stopThreshold = 200;
    acoreFcCfg->fcCfgMem.threshold[3].setThreshold  = 0;
    acoreFcCfg->fcCfgMem.threshold[3].stopThreshold = 20;

    acoreFcCfg->fcCfgCst.threshold.setThreshold  = 3072;
    acoreFcCfg->fcCfgCst.threshold.stopThreshold = 1024;

    return VOS_OK;
}

/* A核内存流控NV参数合法性检查 */
STATIC VOS_UINT32 FC_AcoreCfgCheckMemParam(FC_CfgMem *fcCfgMem)
{
    VOS_UINT32 thresholdLoop;

    if (fcCfgMem->thresholdCnt > FC_MEM_THRESHOLD_LEV_BUTT) {
        FC_LOG1(PS_PRINT_WARNING, "FC, FC_AcoreCfgCheckMemParam, WARNING, ulThresholdCnt <1>\n",
            (VOS_INT32)fcCfgMem->thresholdCnt);

        return VOS_ERR;
    }

    for (thresholdLoop = 0; thresholdLoop < fcCfgMem->thresholdCnt; thresholdLoop++) {
        /* 内存流控设置的是空闲值 */
        if (fcCfgMem->threshold[thresholdLoop].setThreshold > fcCfgMem->threshold[thresholdLoop].stopThreshold) {
            FC_LOG3(PS_PRINT_WARNING,
                "FC, FC_AcoreCfgCheckMemParam, WARNING, ulThresholdLoop <1> ulSetThreshold <2> less than ulStopThreshold <3>\n",
                (VOS_INT32)thresholdLoop, (VOS_INT32)fcCfgMem->threshold[thresholdLoop].setThreshold,
                (VOS_INT32)fcCfgMem->threshold[thresholdLoop].stopThreshold);
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/* A核NV参数合法性检查 */
VOS_UINT32 FC_AcoreCfgCheckParam(FC_AcoreCfgNv *acoreFcCfg)
{
    /* 设计约束，A核需要判断是否因为速传引起CPU占用率高，第一次CPU占用率超标时才启动计算速传速率，所以至少需要2次 */
    if (acoreFcCfg->fcCfgCpuA.smoothTimerLen < 2) {
        FC_LOG1(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, CPUA flow ctrl ulSmoothTimerLen is %d!\n",
            (VOS_INT32)acoreFcCfg->fcCfgCpuA.smoothTimerLen);
        return VOS_ERR;
    }

    if (FC_CfgCheckCpuParam(&(acoreFcCfg->fcCfgCpuA)) != VOS_OK) {
        FC_LOG(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, Check CPUA flow ctrl param failed!\n");
        return VOS_ERR;
    }

    if (FC_AcoreCfgCheckMemParam(&(acoreFcCfg->fcCfgMem)) != VOS_OK) {
        FC_LOG(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, Check mem flow ctrl param failed!\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* A核流控配置初始化 */
VOS_VOID FC_AcoreCfgInit(VOS_VOID)
{
    VOS_UINT32 result;
    VOS_UINT32 fcEnableMask;

    (VOS_VOID)memset_s(&g_fcAcoreCfg, sizeof(FC_AcoreCfgNv), 0, sizeof(FC_AcoreCfgNv));

    result = GUCTTF_NV_Read(MODEM_ID_0, NV_ITEM_ACORE_FLOW_CRTL_CONFIG, &g_fcAcoreCfg, sizeof(FC_AcoreCfgNv));

    /* 将A核FC NV项的开关状态映射到FC模块的总状态 */
    fcEnableMask              = g_fcAcoreCfg.fcEnableMask;
    g_fcAcoreCfg.fcEnableMask = 0;
    g_fcAcoreCfg.fcEnableMask |= ((1 == FC_ACORE_GetEnableMask(fcEnableMask, FC_ACORE_MEM_ENABLE_MASK)) ?
                                      (FC_POLICY_MASK(FC_POLICY_ID_MEM)) :
                                      (0));
    g_fcAcoreCfg.fcEnableMask |= ((1 == FC_ACORE_GetEnableMask(fcEnableMask, FC_ACORE_CPUA_ENABLE_MASK)) ?
                                      (FC_POLICY_MASK(FC_POLICY_ID_CPU_A)) :
                                      (0));
    g_fcAcoreCfg.fcEnableMask |= ((1 == FC_ACORE_GetEnableMask(fcEnableMask, FC_ACORE_CDS_ENABLE_MASK)) ?
                                      (FC_POLICY_MASK(FC_POLICY_ID_CDS)) :
                                      (0));
    g_fcAcoreCfg.fcEnableMask |= ((1 == FC_ACORE_GetEnableMask(fcEnableMask, FC_ACORE_CST_ENABLE_MASK)) ?
                                      (FC_POLICY_MASK(FC_POLICY_ID_CST)) :
                                      (0));

    /* C核独立使用的流控功能默认打开 */
    g_fcAcoreCfg.fcEnableMask |= FC_POLICY_MASK_GPRS;
    g_fcAcoreCfg.fcEnableMask |= FC_POLICY_MASK_TMP;
    g_fcAcoreCfg.fcEnableMask |= FC_POLICY_MASK_CPU_C;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    g_fcAcoreCfg.fcEnableMask |= FC_POLICY_MASK_CDMA;
#endif
    FC_SetFcEnableMask(g_fcAcoreCfg.fcEnableMask);

    if (result != NV_OK) {
        FC_LOG1(PS_PRINT_WARNING, "FC_AcoreCfgInit, WARNING, Fail to read NV, result <1>\n", (VOS_INT32)result);

        (VOS_VOID)FC_AcoreCfgSetDefaultValue(&g_fcAcoreCfg);

        return;
    }

    result = FC_AcoreCfgCheckParam(&g_fcAcoreCfg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_CommInit, ERROR, Check NV parameter fail!\n");

        /* 当前的NV值有错，设置默认值返回成功 */
        (VOS_VOID)FC_AcoreCfgSetDefaultValue(&g_fcAcoreCfg);
    }

    return;
}

/* 流控模块初始化 */
VOS_UINT32 FC_AcoreInit(VOS_VOID)
{
    VOS_UINT32 result;
    VOS_INT    iRet;
    VOS_CHAR   smName[] = "FcACoreCResetDoneSem";

    result = FC_CommInit();

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_Init, ERROR, FC_CommInit return error!\n");
        return VOS_ERR;
    }

    /* 流控配置项初始化 */
    FC_AcoreCfgInit();

    result = FC_CpuaInit();
    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreInit, ERROR, FC_CpuaInit return error!\n");
        return VOS_ERR;
    }

    FC_CdsInit();

    FC_MEM_Init();

    FC_AcoreRegPoint();

    FC_DrvAssemInit();

    /* 创建信号量，用于C核单独复位时，通知底软FcACore的回调事务已完成 */
    if (VOS_SmBCreate(smName, 0, VOS_SEMA4_FIFO, (VOS_SEM*)&g_fcaCorecResetDoneSem) != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreInit, Create Sem Fail\n");
        return VOS_ERR;
    }

    /* 注册回调函数到底软C核复位接口中 */
    iRet = mdrv_sysboot_register_reset_notify("TTF_FcACore", FC_AcoreCResetCallback, 0, FC_ACORE_CRESET_CALLBACK_PRIOR);

    if (iRet != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreInit, ERROR, DRV_CCORERESET_REGCBFUNC fail!\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 流控FID初始化函数 */
VOS_UINT32 FC_AcoreFidInit(enum VOS_InitPhaseDefine phase)
{
    VOS_UINT32 result = VOS_ERR;

    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            result = VOS_RegisterPIDInfo(UEPS_PID_FLOWCTRL, (InitFunType)VOS_NULL_PTR, (MsgFunType)FC_AcoreMsgProc);
            if (result != VOS_OK) {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, VOS_RegisterPIDInfo Fail\n");
                return VOS_ERR;
            }
            result = VOS_RegisterTaskPrio(UEPS_FID_FLOWCTRL, TTF_FLOW_CTRL_TASK_PRIO);
            if (result != VOS_OK) {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, OS_RegisterTaskPrio Fail\n");
                return VOS_ERR;
            }

            result = FC_AcoreInit();

            if (result != VOS_OK) {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, Call FC_Init return error!\n");
                return VOS_ERR;
            }

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
            break;
        default:
            break;
    }

    return VOS_OK;
}


#endif

