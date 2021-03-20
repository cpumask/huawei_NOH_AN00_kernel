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

#ifndef __FC_H__
#define __FC_H__

#include "vos.h"
#include "ps_lib.h"
#include "fc_interface.h"
#include "fc_intra_msg.h"
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#include "acore_nv_stru_gucttf.h"
#else
#include "ccore_nv_stru_gucttf.h"
#endif
#include "ttf_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#define FC_MAX_POINT_NUM 22 /* C核CPU和温保会初始化最多2*FC_UL_RATE_MAX_LEV个 */
#define UEPS_FID_FLOWCTRL UEPS_FID_FLOWCTRL_C
#define UEPS_PID_FLOWCTRL UEPS_PID_FLOWCTRL_C

typedef FC_CcoreCfgCpu          FC_CFG_CPU_STRU;
typedef FC_CcoreCfgMemThreshold FC_CFG_MEM_THRESHOLD_STRU;
#else
#define FC_MAX_POINT_NUM 10 /* A核暂定值 */
#define UEPS_FID_FLOWCTRL UEPS_FID_FLOWCTRL_A
#define UEPS_PID_FLOWCTRL UEPS_PID_FLOWCTRL_A

typedef FC_AcoreCfgCpu          FC_CFG_CPU_STRU;
typedef FC_AcoreCfgMemThreshold FC_CFG_MEM_THRESHOLD_STRU;
#endif

#define FC_MEM_THRESHOLD_MAX_NUM 4

#define FC_PRI_LOWEST FC_PRI_1
#define FC_PRI_HIGHEST FC_PRI_9

#define FC_MAX_NUM 10
#define FC_RAB_MASK_ALL 0x0000FFFE /* RAB范围 1~15 */

#define FC_MAX_BRIDGE_BYTE_CNT 0x80000 /* 2^32 >> 10 >> 3 */

#define FC_GET_MSG_NAME16(rcvMsg) (*((VOS_UINT16*)((VOS_UINT8*)(rcvMsg) + VOS_MSG_HEAD_LENGTH)))
#define FC_GET_MSG_NAME32(rcvMsg) (*((VOS_UINT32*)((VOS_UINT8*)(rcvMsg) + VOS_MSG_HEAD_LENGTH)))

#define FC_POLICY_MASK(policyId) (((VOS_UINT32)1) << (policyId))

#define FC_POLICY_GET_ENABLE_MASK(policyId) ((FC_POLICY_MASK(policyId) & g_fcEnableMask))
#define FC_POLICY_GET(policyId) (&g_fcPolicy[policyId])

#define FC_DOWN_RATE_LIMIT_MASK 0xFFFF

#define FC_UE_MIN_RAB_ID 5
#define FC_UE_MAX_RAB_ID 15

#define FC_ACPU_DRV_ASSEM_UE_ON_MASK 1
#define FC_ACPU_DRV_ASSEM_PC_ON_MASK (1 << 1)
#define FC_ACPU_CDS_GU_ASSEM_ON_MASK (1 << 2)

#define CPU_MAX_SET_DRV_FAIL_SMOOTH_CNT 50 /* 配置驱动失败平滑系数 */
#define CPU_DRV_ASSEM_PARA_MAX_NV_LEV 4

#define FC_ADS_DL_RATE_UP_LEV 200
#define FC_ADS_DL_RATE_DOWN_LEV 20
#define FC_ADS_TIMER_LEN 50
#define FC_MODEM_ID_NUM 2

#define FC_CCPU_TRACE_CPULOAD_TIMELEN 1000 /* CCPU负载上报周期，1s */
#define TIMER_FC_CCPU_TRACE_CPULOAD 0x1001

#define FC_CCPU_PTR_OCTET_OCCUPIED 1 /* CCPU上指针占用1个U32 */
#define FC_ACPU_PTR_OCTET_OCCUPIED 2 /* ACPU上指针占用1个U32 */
#define FC_PTR_MAX_OCTET_OCCUPIED 2

#define FC_LOG(level, str) TTF_LOG(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, level, str)
#define FC_LOG1(level, str, para1) TTF_LOG1(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, level, str, para1)
#define FC_LOG2(level, str, para1, para12) TTF_LOG2(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, level, str, para1, para12)
#define FC_LOG3(level, str, para1, para2, para3) \
    TTF_LOG3(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, level, str, para1, para2, para3)
#define FC_LOG4(level, str, para1, para2, para3, para4) \
    TTF_LOG4(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, level, str, para1, para2, para3, para4)

enum FC_PriOper {
    FC_PRI_CHANGE_AND_CONTINUE = 0, /* 1.Lev改变，继续下一级流控 */
    FC_PRI_CHANGE_AND_BREAK,        /* 2.Lev改变，退出当前操作 */
    FC_PRI_KEEP_AND_BREAK,          /* 3.维持Lev不变，退出当前操作 */
    FC_PRI_OPER_BUTT
};
typedef VOS_UINT32 FC_PriOperUint32;

enum FC_MntnEventType {
    ID_FC_MNTN_POINT_SET_FC          = 0x8001, /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_POINT_CLR_FC          = 0x8002, /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_POLICY_UP             = 0x8003, /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_DOWN           = 0x8004, /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_ADD_POINT      = 0x8005, /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_DEL_POINT      = 0x8006, /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_CHANGE_POINT   = 0x8007, /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_CPU_A_CPULOAD         = 0x8008, /* _H2ASN_MsgChoice FC_MNTN_CPULOAD_STRU */
    ID_FC_MNTN_CPU_C_CPULOAD         = 0x8009, /* _H2ASN_MsgChoice FC_MNTN_CPULOAD_STRU */
    ID_FC_MNTN_DRV_ASSEM_PARA        = 0x800A, /* _H2ASN_MsgChoice FC_MNTN_DRV_ASSEM_PARA_STRU */
    ID_FC_MNTN_ACORE_CRESET_START_FC = 0x800B, /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_ACORE_CRESET_END_FC   = 0x800C, /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */

    ID_FC_MNTN_EVENT_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT16 FC_MntnEventTypeUint16;

enum FC_TimerName {
    FC_TIMER_STOP_CPU_ATTEMPT = 0,
    FC_TIMER_NAME_BUTT
};
typedef VOS_UINT32 FC_TimerNameUint32;

/* FC内部流控策略ID */
enum FC_PrivatePolicyId {
    FC_PRIVATE_POLICY_ID_MEM_MODEM_0 = 0,
    FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0,
    FC_PRIVATE_POLICY_ID_CDS_MODEM_0,
    FC_PRIVATE_POLICY_ID_CST_MODEM_0,
    FC_PRIVATE_POLICY_ID_GPRS_MODEM_0,
    FC_PRIVATE_POLICY_ID_TMP_MODEM_0,
    FC_PRIVATE_POLICY_ID_CPU_C_MODEM_0,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    FC_PRIVATE_POLICY_ID_CDMA_MODEM_0,
#endif
    FC_PRIVATE_POLICY_ID_MEM_MODEM_1,
    FC_PRIVATE_POLICY_ID_CPU_A_MODEM_1,
    FC_PRIVATE_POLICY_ID_CDS_MODEM_1,
    FC_PRIVATE_POLICY_ID_CST_MODEM_1,
    FC_PRIVATE_POLICY_ID_GPRS_MODEM_1,
    FC_PRIVATE_POLICY_ID_TMP_MODEM_1,
    FC_PRIVATE_POLICY_ID_CPU_C_MODEM_1,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    FC_PRIVATE_POLICY_ID_CDMA_MODEM_1,
#endif
    FC_PRIVATE_POLICY_ID_BUTT
};
typedef VOS_UINT8 FC_PrivatePolicyIdUint8;

/* 流控优先级结构 * */
typedef struct {
    VOS_UINT8       valid;   /* PS_TRUE:有效, PS_FALSE:无效 */
    VOS_UINT8       fcIdCnt; /* 当前优先级对应的流控Id个数 */
    VOS_UINT8       fcIdIndex;
    FC_PriTypeUint8 pri;                    /* 当前优先级 */
    FC_IdUint8      fcId[FC_MAX_POINT_NUM]; /* 当前优先级对应的流控Id */
    VOS_UINT8       rsv[6];
} FC_Pri;

/* 流控通用策略结构 */
typedef VOS_UINT32 (*FC_PolicyPostProcess)(VOS_UINT32 param1, VOS_UINT32 param2);
typedef VOS_UINT32 (*FC_PolicyAdjustForUpFunc)(FC_PriTypeUint8 pointPri, FC_IdUint8 fcId);
typedef VOS_UINT32 (*FC_PolicyAdjustForDownFunc)(FC_PriTypeUint8 pointPri, FC_IdUint8 fcId);

typedef struct {
    FC_PrivatePolicyIdUint8    policyId;   /* 策略ID */
    VOS_UINT8                  priCnt;     /* 当前策略的优先级总数 */
    FC_PriTypeUint8            highestPri; /* 当前流控策略的最高优先级别 */
    FC_PriTypeUint8            donePri;    /* 当前处理过的优先级 */
    FC_PriTypeUint8            toPri;
    VOS_UINT8                  rsv[3];
    FC_Pri                     fcPri[FC_PRI_BUTT];         /* 某一个优先级流控对应的处理 */
    FC_PolicyAdjustForUpFunc   adjustForUpFunc;            /* _H2ASN_Replace VOS_UINT32  pAdjustForUpFunc; */
    FC_PolicyAdjustForDownFunc adjustForDownFunc;          /* _H2ASN_Replace VOS_UINT32  pAdjustForDownFunc; */
    FC_PolicyPostProcess postFunc; /* 该流控策略执行到最后一级的回调函数 */ /* _H2ASN_Replace VOS_UINT32  pPostFunc; */
} FC_Policy;

typedef struct {
    FC_IdUint8    fcId; /* 流控ID */
    VOS_UINT8     rsv[1];
    ModemIdUint16 modemId; /* ModemId */      /* _H2ASN_Replace VOS_UINT16  enModemId; */
    VOS_UINT32    policyMask;                 /* 该流控点关联的流控策略，便于查找核删除 */
    VOS_UINT32    fcMask;                     /* 流控投票管理 */
    VOS_UINT32    param1;                     /* 该流控点注册时，记录的参数值 */
    VOS_UINT32    param2;                     /* 该流控点注册时，记录的参数值 */
    FC_SetFunc    setFunc; /* 流控执行函数 */       /* _H2ASN_Replace VOS_UINT32  pSetFunc; */
    FC_ClrFunc    clrFunc; /* 流控解除函数 */       /* _H2ASN_Replace VOS_UINT32  pClrFunc; */
    FC_RstFunc    rstFunc; /* 新增的复位处理与恢复接口 */ /* _H2ASN_Replace VOS_UINT32  pRstFunc; */
} FC_Point;

/* 流控点管理结构 */
typedef struct {
    VOS_UINT32 pointNum;
    VOS_UINT8  rsv[4];
    FC_Point   fcPoint[FC_MAX_POINT_NUM];
} FC_PointMgr;

typedef struct {
    VOS_UINT32 lastByteCnt; /* 上次统计时的值 */
    VOS_UINT32 rate;        /* 网桥速率，单位:bps */
} FC_BridgeRate;

/* 一个FC上FC 和RAB之间的映射关系 */
typedef struct {
    FC_IdUint8 fcId;
    VOS_UINT8  rsv[7];
    VOS_UINT32 includeRabMask; /* 该Fc Id对应的所有RAB，用掩码表示 */
    VOS_UINT32 noFcRabMask;    /* 该Fc Id上不要求流控的RAB，没有RAB流控时等于ulIncludeRabMask */
} FC_RabMappingInfo;

typedef struct {
    FC_IdUint8        fcIdCnt; /* FC Id流控实体个数 */
    VOS_UINT8         rsv[3];
    FC_RabMappingInfo fcRabMappingInfo[FC_MAX_NUM]; /* FC 和RAB之间的映射关系集合 */
} FC_RabMappingInfoSet;

/* 流控点钩包结构 */
typedef struct {
    FC_IdUint8    fcId; /* 流控ID */
    VOS_UINT8     rsv[5];
    ModemIdUint16 modemId; /* ModemId */                   /* _H2ASN_Replace VOS_UINT16  enModemId; */
    VOS_UINT32    policyMask;                              /* 该流控点关联的流控策略，便于查找核删除 */
    VOS_UINT32    fcMask;                                  /* 流控投票管理 */
    VOS_UINT32    param1;                                  /* 该流控点注册时，记录的参数值 */
    VOS_UINT32    param2;                                  /* 该流控点注册时，记录的参数值 */
    VOS_UINT32    pointSetAddr[FC_PTR_MAX_OCTET_OCCUPIED]; /* 记录流控执行函数的地址 */
    VOS_UINT32    pointClrAddr[FC_PTR_MAX_OCTET_OCCUPIED]; /* 记录流控清除函数的地址 */
    VOS_UINT32    pointRstAddr[FC_PTR_MAX_OCTET_OCCUPIED]; /* 记录流控复位处理与恢复函数的地址 */
} FC_MntnPointInfo;

/* 流控点钩包TRACE结构 */
typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
        FC_MntnEventTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8                  rsv[2];
    FC_MntnPointInfo           fcPoint;
    VOS_UINT32                 isFuncInvoked;
    VOS_UINT32                 result;
} FC_MntnPointFc;

/* 流控策略钩包结构 */
typedef struct {
    FC_PrivatePolicyIdUint8 policyId;   /* 策略ID */
    VOS_UINT8               priCnt;     /* 当前策略的优先级总数 */
    FC_PriTypeUint8         highestPri; /* 当前流控策略的最高优先级别 */
    FC_PriTypeUint8         donePri;    /* 当前处理过的优先级 */
    FC_PriTypeUint8         toPri;
    VOS_UINT8               rsv[3];
    FC_Pri                  fcPri[FC_PRI_BUTT];                        /* 某一个优先级流控对应的处理 */
    VOS_UINT32              policyUpAddr[FC_PTR_MAX_OCTET_OCCUPIED];   /* 记录流控策略升执行函数的地址 */
    VOS_UINT32              policyDownAddr[FC_PTR_MAX_OCTET_OCCUPIED]; /* 记录流控策略降执行函数的地址 */
    VOS_UINT32              policyPostAddr[FC_PTR_MAX_OCTET_OCCUPIED]; /* 流控策略执行到最后一级的回调函数的地址 */
} FC_MntnPolicyInfo;

/* 流控策略钩包TRACE结构 */
typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
        FC_MntnEventTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8                  rsv[2];
    FC_MntnPolicyInfo          policy;
} FC_MntnPolicy;

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
        FC_MntnEventTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8                  rsv[2];
    VOS_UINT32                 cpuLoad;
} FC_MntnCpuLoad;

typedef struct {
    VOS_UINT32 smoothTimerLen; /* CPU流控的平滑次数，连续若干次CPU超标，才进行CPU流控 */
    VOS_UINT8  rsv[4];
    HTIMER     stopAttemptTHandle; /* CPU流控解除优化定时器 */
} FC_CpuCtrl;

typedef struct {
    const VOS_UINT32 traceCpuLoadTimerLen; /* 上报CCPU负载定时器时长 */ /* _H2ASN_Skip */
    VOS_UINT32       cpuLoadRecord;                            /* 记录底软上报的CPU负载 */
    HTIMER           traceCpuLoadTHandle;                      /* 上报CCPU负载定时器 */
} FC_TraceCpuload;

extern FC_Policy   g_fcPolicy[]; /* 流控策略实体 */
extern FC_PointMgr g_fcPointMgr; /* 流控点管理实体 */
extern VOS_UINT32  g_fcEnableMask;
/* 流控内部策略表 */
extern FC_PrivatePolicyIdUint8 g_privatePolicyTbl[FC_MODEM_ID_NUM][FC_POLICY_ID_BUTT];

typedef struct {
    FC_MntnEventTypeUint16 enMsgID; /* _H2ASN_MsgChoice_Export FC_MNTN_EVENT_TYPE_ENUM_UINT16 */

    VOS_UINT8 aucMsgBlock[2];
} FC_MSG_DATA;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    FC_MSG_DATA stMsgData;
} Fc_MSG;

VOS_VOID FC_SetFcEnableMask(VOS_UINT32 enableMask);

extern VOS_UINT32 FC_SndTemperatureMsg(FC_MsgTypeUint16 msgName);

VOS_UINT32 FC_CfgCheckCpuParam(FC_CFG_CPU_STRU *fcCfgCpu);

extern VOS_UINT32 FC_SndCpuMsg(FC_MsgTypeUint16 msgName, VOS_UINT32 cpuLoad, VOS_UINT32 ulRate, VOS_UINT32 dlRate);

extern VOS_UINT32 FC_PolicyUp(FC_Policy *policy);

extern VOS_UINT32 FC_PolicyDown(FC_Policy *fcPolicy);

extern VOS_UINT32 FC_PolicyUpToTargetPri(FC_Policy *fcPolicy, FC_PriTypeUint8 targetPri);

extern VOS_UINT32 FC_PolicyDownToTargetPri(FC_Policy *fcPolicy, FC_PriTypeUint8 targetPri);

extern FC_PriOperUint32 FC_PointSetFc(VOS_UINT32 policyMask, FC_IdUint8 fcId);

extern FC_PriOperUint32 FC_PointClrFc(VOS_UINT32 policyMask, FC_IdUint8 fcId);

extern VOS_UINT32 FC_PointReg(FC_RegPointPara *fcRegPoint);

extern VOS_UINT32 FC_PointDeReg(FC_IdUint8 fcId, ModemIdUint16 modemId);

extern VOS_UINT32 FC_PointChange(
    FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 newPri, ModemIdUint16 modemId);

extern VOS_UINT32 FC_CommInit(VOS_VOID);

VOS_VOID          FC_MNTN_TraceEvent(VOS_VOID *msg);
extern VOS_UINT32 FC_MNTN_TraceCpuLoad(FC_MntnEventTypeUint16 msgName, VOS_UINT32 cpuLoad);
extern VOS_VOID   FC_MNTN_TracePointFcEvent(
      FC_MntnEventTypeUint16 msgName, const FC_Point *fcPoint, VOS_UINT32 isFuncInvoked, VOS_UINT32 result);
extern VOS_UINT32 FC_CpuaUpJudge(VOS_UINT32 cpuLoad, FC_CFG_CPU_STRU *fcCfgCpu, FC_Policy *fcPolicy);

extern VOS_VOID        FC_CpuaRcvCpuLoad(VOS_UINT32 cpuLoad);
extern FC_PriTypeUint8 FC_MEM_CalcUpTargetFcPri(FC_Policy *policy, VOS_UINT32 memValue);
extern FC_PriTypeUint8 FC_MEM_CalcDownTargetFcPri(FC_Policy *policy, VOS_UINT32 memValue);

extern VOS_VOID FC_MEM_UpProcess(VOS_UINT32 memValue);
extern VOS_VOID FC_MEM_DownProcess(VOS_UINT32 memValue);

extern VOS_UINT32 FC_CdsDelFcId(FC_IdUint8 fcId, ModemIdUint16 modemId);
extern VOS_UINT32 FC_CdsAddRab(FC_IdUint8 fcId, VOS_UINT8 rabId, ModemIdUint16 modemId);
extern VOS_UINT32 FC_CdsDelRab(VOS_UINT8 rabId, ModemIdUint16 modemId);
extern VOS_UINT32 FC_CdsUpProcess(VOS_UINT8 rabId, ModemIdUint16 modemId);
extern VOS_UINT32 FC_CdsDownProcess(VOS_UINT8 rabId, ModemIdUint16 modemId);
extern VOS_UINT32 FC_AcoreRcvIntraMsg(MsgBlock *msg);
extern VOS_VOID   FC_AcoreMsgProc(MsgBlock *msg);
extern VOS_UINT32 FC_AcoreInit(VOS_VOID);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
