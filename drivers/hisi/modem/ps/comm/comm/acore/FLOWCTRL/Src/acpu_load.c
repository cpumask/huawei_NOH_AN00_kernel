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
#if (FEATURE_ACPU_STAT == FEATURE_ON)

#include "acpu_load.h"
#include "vos_config.h"
#include "ttf_util.h"
#include "acpuload_interface.h"

#include "mdrv_nvim.h"
#include "product_config.h"
#include "acore_nv_id_gucttf.h"
#include "acore_nv_stru_gucttf.h"

/*lint -e322 -e7*/
#include <linux/cpumask.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/irqnr.h>
#include <asm/cputime.h>
#include <linux/msa.h>
/*lint +e322 +e7*/

#define THIS_FILE_ID PS_FILE_ID_CPULOAD_C

/* 参考/fs/proc/stat.c实现 */
#define arch_idle_time(cpu) 0

/* 根据Timer定时进行的CPU负载统计 */
CPULOAD_StatInfo g_regularCpuLoad;

/* 实时响应用户进行的CPU负载统计 */
CPULOAD_StatInfo g_userDefCpuLoad;

/* 记录是否是第一次超时 */
VOS_INT32 g_firstTimeout = VOS_TRUE;

/* 回调函数指针数组 */
CPULOAD_RptHookFunc g_funRptHooks[CPULOAD_MAX_HOOK_NUM];

/* CPU统计周期定时器 */
HTIMER g_regularCpuLoadTmr;

/* CPU LOAD NV 配置信息 */
CPULOAD_CFG_STRU g_nvCfg;

/* 调用系统接口读取CPU使用情况数据 */
VOS_VOID CPULOAD_ReadCpuStat(CPULOAD_StatInfo *cpu)
{
    /* 获取空闲任务的运行时间和总的运行时间 */
    msa_getcpu_idle(&(cpu->currRecord.totalTime), &(cpu->currRecord.idleTime));

    return;
}

/* 将上次CPU数据更新为当前数据，用于下次计算 */
VOS_VOID CPULOAD_UpdateSavInfo(CPULOAD_StatInfo *cpu)
{
    /* 更新保存的结果 */
    cpu->prevRecord = cpu->currRecord;
}

/* 计算两次读取CPU使用情况期间的CPU占用率 */
VOS_UINT32 CPULOAD_CalLoad(CPULOAD_StatInfo *cpu)
{
    /* V9R1项目中使用宏开关进行打桩处理 */
    VOS_UINT32 idle;
    VOS_UINT32 total;
    VOS_UINT32 load;

    idle  = MOD_SUB((cpu->currRecord.idleTime), (cpu->prevRecord.idleTime), SLIENCE_MAX);
    total = MOD_SUB((cpu->currRecord.totalTime), (cpu->prevRecord.totalTime), SLIENCE_MAX);

    /* 异常保护，如果上报的tick值有问题，CPU load按照0处理，相当于把此功能off掉 */
    if (total == 0) {
        load = 0;

    } else {
        load = (100 * (total - idle)) / total;
    }

    /* 保存此次计算结果 */
    cpu->cpuLoad = load;

    return load;
}

/* 返回LINUX当前CPU占用率 */
VOS_UINT32 CPULOAD_GetCpuLoad(VOS_VOID)
{
    /* 使用宏开关判断是否需要打桩处理 */
    return g_regularCpuLoad.cpuLoad;
}

/* 执行注册的CPU贡载回调函数 */
VOS_VOID CPULOAD_InvokeRtpHooks(VOS_UINT32 load)
{
    VOS_UINT32 hookLoop;

    for (hookLoop = 0; hookLoop < CPULOAD_MAX_HOOK_NUM; hookLoop++) {
        if (g_funRptHooks[hookLoop] != VOS_NULL_PTR) {
            g_funRptHooks[hookLoop](load);
        }
    }

    return;
}

/* 计算定时时长内的CPU负载 */
VOS_VOID CPULOAD_RegularTimeoutProc(VOS_VOID)
{
    CPULOAD_StatInfo *cpu = &g_regularCpuLoad;
    VOS_UINT32        load;

    /* 读取当前的CPU统计数据 */
    CPULOAD_ReadCpuStat(cpu);

    load = CPULOAD_CalLoad(cpu);

    /* 保存此次数据，用于与下次计算 */
    CPULOAD_UpdateSavInfo(cpu);

    /* 第一次收到TimeOut消息，由于上次记录为空，计算不准 */
    if (g_firstTimeout == VOS_TRUE) {
        g_firstTimeout = VOS_FALSE;
        return;
    }

    CPULOAD_InvokeRtpHooks(load);

    return;
}

/* 定时器超时消息处理 */
VOS_VOID CPULOAD_RcvTimerExpireMsg(REL_TimerMsgBlock *timerMsg)
{
    switch (timerMsg->name) {
        case CPULOAD_REGULAR_TMR_NAME:
            CPULOAD_RegularTimeoutProc();
            break;
        default:
            break;
    }

    return;
}

/* 执行初始化 */
VOS_UINT32 CPULOAD_Init(VOS_VOID)
{
    VOS_UINT32        hookLoop;
    VOS_UINT32        rtn;
    CPULOAD_CFG_STRU *nvCfg = &g_nvCfg;

    /* CPU ID目前只有一个，赋值为0 */
    (VOS_VOID)memset_s((VOS_VOID*)&g_regularCpuLoad, sizeof(CPULOAD_StatInfo), 0, sizeof(CPULOAD_StatInfo));
    (VOS_VOID)memset_s((VOS_VOID*)&g_userDefCpuLoad, sizeof(CPULOAD_StatInfo), 0, sizeof(CPULOAD_StatInfo));

    for (hookLoop = 0; hookLoop < CPULOAD_MAX_HOOK_NUM; hookLoop++) {
        g_funRptHooks[hookLoop] = VOS_NULL_PTR;
    }

    rtn = GUCTTF_NV_Read(MODEM_ID_0, NV_ITEM_LINUX_CPU_MONITOR_TIMER_LEN, nvCfg, sizeof(CPULOAD_CFG_STRU));

    /* 若读取NV_ITEM_LINUX_CPU_MONITOR_TIMER_LEN失败,或NV值非法，则使用默认值 */
    if ((rtn != NV_OK) || (nvCfg->ulMonitorTimerLen < CPULOAD_REGULAR_TMR_MIN_LEN) ||
        (nvCfg->ulMonitorTimerLen > CPULOAD_REGULAR_TMR_MAX_LEN)) {
        /* 如果异常，则将定时器初始化为默认值400ms */
        nvCfg->ulMonitorTimerLen = CPULOAD_REGULAR_TMR_DEF_LEN;
    }

    /* V9R1项目中使用宏开关判断是否需要启动定时器 */
    rtn = VOS_StartRelTimer(&g_regularCpuLoadTmr, ACPU_PID_CPULOAD, nvCfg->ulMonitorTimerLen, CPULOAD_REGULAR_TMR_NAME,
        0, VOS_RELTIMER_LOOP, VOS_TIMER_NO_PRECISION);
    if (rtn != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}

/* CPU监测模块消息处理 */
VOS_VOID CPULOAD_PidMsgProc(MsgBlock *rcvMsg)
{
    switch (VOS_GET_SENDER_ID(rcvMsg)) {
        case VOS_PID_TIMER:
            CPULOAD_RcvTimerExpireMsg((REL_TimerMsgBlock*)rcvMsg); /* 接收到TIMER超时消息时的处理 */
            break;

        default:
            break;
    }
}

/* CPU监测FID初始化函数 */
VOS_UINT32 CPULOAD_FidInit(enum VOS_InitPhaseDefine phase)
{
    VOS_UINT32 result = VOS_ERR;

    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            result = VOS_RegisterPIDInfo(ACPU_PID_CPULOAD, (InitFunType)VOS_NULL_PTR, (MsgFunType)CPULOAD_PidMsgProc);
            if (result != VOS_OK) {
                return VOS_ERR;
            }

            result = VOS_RegisterTaskPrio(ACPU_FID_CPULOAD, TTF_ACPULOAD_TASK_PRIO);

            if (result != VOS_OK) {
                return VOS_ERR;
            }

            result = CPULOAD_Init();

            if (result != VOS_OK) {
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
        case VOS_IP_BUTT:
            break;
        default:
            break;
    }

    return VOS_OK;
}

/* CPU负载的回调函数注册接口 */
VOS_UINT32 CPULOAD_RegRptHook(CPULOAD_RptHookFunc rptHook)
{
    VOS_UINT32 hookLoop;
    VOS_INT32  lockKey;

    lockKey = VOS_SplIMP();

    for (hookLoop = 0; hookLoop < CPULOAD_MAX_HOOK_NUM; hookLoop++) {
        if (g_funRptHooks[hookLoop] == VOS_NULL_PTR) {
            g_funRptHooks[hookLoop] = rptHook;
            VOS_Splx(lockKey);

            return VOS_OK;
        }
    }

    VOS_Splx(lockKey);

    return VOS_ERR;
}

/* 用户控制监测时长，初始化接口 */
VOS_VOID CPULOAD_ResetUserDefLoad()
{
    CPULOAD_StatInfo *cpu = &g_userDefCpuLoad;

    /* 读取当前的CPU统计数据 */
    CPULOAD_ReadCpuStat(cpu);

    /* 保存此次数据，用于与下次计算 */
    CPULOAD_UpdateSavInfo(cpu);

    return;
}

/* 用户控制监测时长，读取CPU负载接口 */
VOS_UINT32 CPULOAD_GetUserDefLoad()
{
    CPULOAD_StatInfo *cpu = &g_userDefCpuLoad;
    VOS_UINT32        load;

    /* 读取当前的CPU统计数据 */
    CPULOAD_ReadCpuStat(cpu);

    /* 与初始化时数据计算得到 */
    load = CPULOAD_CalLoad(cpu);

    /* 保存此次数据，用于与下次计算 */
    CPULOAD_UpdateSavInfo(cpu);

    return load;
}

/* 返回常规定时器时长，单位毫秒 */
VOS_UINT32 CPULOAD_GetRegularTimerLen()
{
    return g_nvCfg.ulMonitorTimerLen;
}

#endif

