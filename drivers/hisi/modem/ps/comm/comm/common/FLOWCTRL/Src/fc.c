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

#include "fc.h"
#include "fc_interface.h"
#include "PsTypeDef.h"
#include "ps_common_def.h"
#include "mdrv_nvim.h"
#include "ttf_comm.h"
#include "ttf_util.h"
#include "gucttf_tag.h"
#include "securec.h"
#include "ps_trace_msg.h"


/*lint -e534*/
#define THIS_FILE_ID PS_FILE_ID_FLOW_CTRL_C
#define THIS_MODU mod_fc

STATIC FC_Point* FC_PointGet(FC_IdUint8 fcId);
STATIC FC_PriTypeUint8 FC_PolicyGetPriWithFcId(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId);
STATIC VOS_UINT32 FC_SndRegPointMsg(FC_RegPointPara *fcRegPoint);
STATIC VOS_UINT32 FC_SndChangePointMsg(
    FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 pri, ModemIdUint16 modemId);
STATIC VOS_UINT32 FC_PolicyAddPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointPri);
STATIC VOS_UINT32 FC_PolicyDelPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId);
STATIC VOS_UINT32 FC_PolicyChangePoint(
    FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointOldPri, FC_PriTypeUint8 pointNewPri);
STATIC VOS_UINT32 FC_SndDeRegPointMsg(FC_IdUint8 fcId, ModemIdUint16 modemId);

/* 流控策略实体 */
FC_Policy g_fcPolicy[FC_PRIVATE_POLICY_ID_BUTT];

/* 流控点管理实体 */
FC_PointMgr g_fcPointMgr;

/* 流控内部策略表 */
FC_PrivatePolicyIdUint8 g_privatePolicyTbl[FC_MODEM_ID_NUM][FC_POLICY_ID_BUTT] = {
    { FC_PRIVATE_POLICY_ID_MEM_MODEM_0,
        FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0,
        FC_PRIVATE_POLICY_ID_CDS_MODEM_0,
        FC_PRIVATE_POLICY_ID_CST_MODEM_0,
        FC_PRIVATE_POLICY_ID_GPRS_MODEM_0,
        FC_PRIVATE_POLICY_ID_TMP_MODEM_0,
        FC_PRIVATE_POLICY_ID_CPU_C_MODEM_0,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        FC_PRIVATE_POLICY_ID_CDMA_MODEM_0
#endif
    },
    {
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
    }
};

VOS_UINT32 g_fcEnableMask = 0;

VOS_UINT32 g_fcDebugLevel = (VOS_UINT32)PS_PRINT_WARNING;

/*lint -save -e958 */

/* 输出可维可测 */
VOS_VOID FC_MNTN_TraceEvent(VOS_VOID *msg)
{
    mdrv_diag_trace_report(msg);

    return;
}

/* 流控执行可维可测 */
VOS_VOID FC_MNTN_TracePointFcEvent(
    FC_MntnEventTypeUint16 msgName, const FC_Point *fcPoint, VOS_UINT32 isFuncInvoked, VOS_UINT32 result)
{
    FC_MntnPointFc mntnPointFc;
    VOS_UINT_PTR   ptrAddr;
    VOS_UINT32    *octet = VOS_NULL_PTR;

    TTF_SET_MSG_SENDER_ID(&mntnPointFc, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_RECEIVER_ID(&mntnPointFc, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_LEN(&mntnPointFc, sizeof(FC_MntnPointFc) - VOS_MSG_HEAD_LENGTH);
    mntnPointFc.msgName       = msgName;
    mntnPointFc.isFuncInvoked = isFuncInvoked;
    mntnPointFc.result        = result;

    /* fetch FcPoint info */
    mntnPointFc.fcPoint.fcId       = fcPoint->fcId;
    mntnPointFc.fcPoint.modemId    = fcPoint->modemId;
    mntnPointFc.fcPoint.rsv[0]     = 0x0U;
    mntnPointFc.fcPoint.rsv[1]     = 0x0U;
    mntnPointFc.fcPoint.rsv[2]     = 0x0U;
    mntnPointFc.fcPoint.rsv[3]     = 0x0U;
    mntnPointFc.fcPoint.rsv[4]     = 0x0U;
    mntnPointFc.fcPoint.policyMask = fcPoint->policyMask;
    mntnPointFc.fcPoint.fcMask     = fcPoint->fcMask;
    mntnPointFc.fcPoint.param1     = fcPoint->param1;
    mntnPointFc.fcPoint.param2     = fcPoint->param2;

    ptrAddr                             = (VOS_UINT_PTR)(fcPoint->setFunc);
    mntnPointFc.fcPoint.pointSetAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                               = (VOS_UINT32*)(VOS_UINT_PTR)fcPoint->setFunc;
    mntnPointFc.fcPoint.pointSetAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    ptrAddr                             = (VOS_UINT_PTR)(fcPoint->clrFunc);
    mntnPointFc.fcPoint.pointClrAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                               = (VOS_UINT32*)(VOS_UINT_PTR)fcPoint->clrFunc;
    mntnPointFc.fcPoint.pointClrAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    ptrAddr                             = (VOS_UINT_PTR)(fcPoint->rstFunc);
    mntnPointFc.fcPoint.pointRstAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                               = (VOS_UINT32*)(VOS_UINT_PTR)fcPoint->rstFunc;
    mntnPointFc.fcPoint.pointRstAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    FC_MNTN_TraceEvent((VOS_VOID*)&mntnPointFc);

    return;
}

/* 输出流控策略的可维可测 */
STATIC VOS_UINT32 FC_MNTN_TracePolicy(FC_MntnEventTypeUint16 msgName, FC_Policy *policy)
{
    FC_MntnPolicy fcMntnPolicy = {0};
    VOS_UINT_PTR  ptrAddr;
    VOS_UINT32   *octet = VOS_NULL_PTR;

    TTF_SET_MSG_SENDER_ID(&fcMntnPolicy, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_RECEIVER_ID(&fcMntnPolicy, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_LEN(&fcMntnPolicy, sizeof(FC_MntnPolicy) - VOS_MSG_HEAD_LENGTH);
    fcMntnPolicy.msgName = msgName;

    fcMntnPolicy.policy.policyId   = policy->policyId;
    fcMntnPolicy.policy.priCnt     = policy->priCnt;
    fcMntnPolicy.policy.rsv[0]     = 0x0U;
    fcMntnPolicy.policy.rsv[1]     = 0x0U;
    fcMntnPolicy.policy.rsv[2]     = 0x0U;
    fcMntnPolicy.policy.highestPri = policy->highestPri;
    fcMntnPolicy.policy.donePri    = policy->donePri;
    fcMntnPolicy.policy.toPri      = policy->toPri;
    (VOS_VOID)memcpy_s(fcMntnPolicy.policy.fcPri, sizeof(fcMntnPolicy.policy.fcPri),
        policy->fcPri, sizeof(policy->fcPri));

    ptrAddr                             = (VOS_UINT_PTR)(policy->adjustForUpFunc);
    fcMntnPolicy.policy.policyUpAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                               = (VOS_UINT32*)(VOS_UINT_PTR)(policy->adjustForUpFunc);
    fcMntnPolicy.policy.policyUpAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    ptrAddr                               = (VOS_UINT_PTR)(policy->adjustForDownFunc);
    fcMntnPolicy.policy.policyDownAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                                 = (VOS_UINT32*)(VOS_UINT_PTR)(policy->adjustForDownFunc);
    fcMntnPolicy.policy.policyDownAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    ptrAddr                               = (VOS_UINT_PTR)(policy->postFunc);
    fcMntnPolicy.policy.policyPostAddr[0] = (VOS_UINT32)(ptrAddr & (~0U)); /* Low */
    octet                                 = (VOS_UINT32*)(VOS_UINT_PTR)(policy->adjustForDownFunc);
    fcMntnPolicy.policy.policyPostAddr[1] = (VOS_UINT32)(VOS_UINT_PTR)(octet + 1); /* High */

    FC_MNTN_TraceEvent(&fcMntnPolicy);

    return VOS_OK;
}

/* 输出当前CPULoad */
VOS_UINT32 FC_MNTN_TraceCpuLoad(FC_MntnEventTypeUint16 msgName, VOS_UINT32 cpuLoad)
{
    FC_MntnCpuLoad fcMntnCpuLoad = {0};

    TTF_SET_MSG_SENDER_ID(&fcMntnCpuLoad, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_RECEIVER_ID(&fcMntnCpuLoad, UEPS_PID_FLOWCTRL);
    TTF_SET_MSG_LEN(&fcMntnCpuLoad, sizeof(FC_MntnCpuLoad) - VOS_MSG_HEAD_LENGTH);
    fcMntnCpuLoad.msgName = msgName;
    fcMntnCpuLoad.cpuLoad = cpuLoad;

    FC_MNTN_TraceEvent(&fcMntnCpuLoad);

    return VOS_OK;
}

/* 设置流控掩码函数 */
VOS_VOID FC_SetFcEnableMask(VOS_UINT32 enableMask)
{
    g_fcEnableMask = enableMask;

    return;
}

/* 判断流控内部策略是否使能 */
STATIC VOS_UINT32 FC_IsPolicyEnable(VOS_UINT32 pointPolicyMask, ModemIdUint16 modemId)
{
    VOS_UINT32 policyMask;

    /* 对于Modem1,内部策略在判断使能时，需要右移FC_POLICY_ID_BUTT后再与NV项中使能位比较 */
    if (modemId == MODEM_ID_0) {
        policyMask = pointPolicyMask;
    } else {
        policyMask = (pointPolicyMask >> FC_POLICY_ID_BUTT);
    }

    return (policyMask & g_fcEnableMask);
}

/* 请求添加注册流控点,供外部模块调用 */
VOS_UINT32 FC_RegPoint(FC_RegPointPara *fcRegPoint)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (fcRegPoint == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, pstFcRegPoint is NULL!\n");
        return VOS_ERR;
    }

    if (fcRegPoint->modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enModemId err <1>!\n", (VOS_INT32)fcRegPoint->modemId);
        return VOS_ERR;
    }

    if (fcRegPoint->policyId >= FC_POLICY_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enPolicyId err <1>!\n", (VOS_INT32)fcRegPoint->policyId);
        return VOS_ERR;
    }

    if (fcRegPoint->fcId >= FC_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enFcId err <1>\n", (VOS_INT32)fcRegPoint->fcId);
        return VOS_ERR;
    }

    if ((fcRegPoint->fcPri < FC_PRI_LOWEST) || (fcRegPoint->fcPri > FC_PRI_HIGHEST)) {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid enPri <1>\n", (VOS_INT32)fcRegPoint->fcPri);
        return VOS_ERR;
    }

    /* 使能检查 */
    if (FC_POLICY_MASK(fcRegPoint->policyId) != (FC_POLICY_MASK(fcRegPoint->policyId) & g_fcEnableMask)) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_RegPoint, INFO, MEM FlowCtrl is disabled <1> \n", (VOS_INT32)g_fcEnableMask);
        return VOS_OK;
    }

    /* 注册内存策略，优先级只能在内存流控约定的范围之内 */
    if (fcRegPoint->policyId == FC_POLICY_ID_MEM) {
        if ((fcRegPoint->fcPri < FC_PRI_FOR_MEM_LEV_1) || (fcRegPoint->fcPri > FC_PRI_FOR_MEM_LEV_4)) {
            FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid enPri for MEM <1>\n", (VOS_INT32)fcRegPoint->fcPri);
            return VOS_ERR;
        }
    }

    if (fcRegPoint->setFunc == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid Fun pSetFunc\n");
        return VOS_ERR;
    }

    if (fcRegPoint->clrFunc == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid Fun pClrFunc\n");
        return VOS_ERR;
    }

    result = FC_SndRegPointMsg(fcRegPoint);

    return result;
}

/* 请求去注册流控点 */
VOS_UINT32 FC_DeRegPoint(FC_IdUint8 fcId, ModemIdUint16 modemId)
{
    VOS_UINT32 result;
    FC_Point  *fcPoint = VOS_NULL_PTR;
    VOS_UINT32 pointPolicyMask;

    if (fcId >= FC_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_DeRegPoint, ERROR, Invalid enFcId <1>\n", (VOS_INT32)fcId);
        return VOS_ERR;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_DeRegPoint, ERROR, enModemId err <1>!\n", (VOS_INT32)modemId);
        return VOS_ERR;
    }

    /* 通过流控点ID找到流控点结构 */
    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        /* 该流控策略未使能，则返回VOS_OK */
        FC_LOG1(PS_PRINT_WARNING, "FC_DeRegPoint, WARNING, This FCPonit = <1> didn't Reg!\n", (VOS_INT32)fcId);
        return VOS_OK;
    }

    /* 流控点删除，需要对相应的流控策略进行处理 */
    pointPolicyMask = fcPoint->policyMask;

    if (FC_IsPolicyEnable(pointPolicyMask, modemId) != 0) {
        result = FC_SndDeRegPointMsg(fcId, modemId);

        return result;
    }

    return VOS_OK;
}

/* 更改流控点 */
VOS_UINT32 FC_ChangePoint(FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 pri, ModemIdUint16 modemId)
{
    VOS_UINT32 result;

    if (fcId >= FC_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, Invalid enFcId <1>\n", (VOS_INT32)fcId);
        return VOS_ERR;
    }

    if ((pri < FC_PRI_LOWEST) || (pri > FC_PRI_HIGHEST)) {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, Invalid enPri <1>\n", (VOS_INT32)pri);
        return VOS_ERR;
    }

    if (policyId >= FC_POLICY_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, enPolicyId err <1>!\n", (VOS_INT32)policyId);
        return VOS_ERR;
    }

    if (modemId >= MODEM_ID_BUTT) {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, enModemId err <1>!\n", (VOS_INT32)modemId);
        return VOS_ERR;
    }

    /* 使能检查 */
    if (FC_POLICY_MASK(policyId) != (FC_POLICY_MASK(policyId) & g_fcEnableMask)) {
        /* 内存流控未使能 */
        FC_LOG1(PS_PRINT_INFO, "FC_ChangePoint, INFO, MEM FlowCtrl is disabled <1> \n", (VOS_INT32)g_fcEnableMask);
        return VOS_OK;
    }

    result = FC_SndChangePointMsg(fcId, policyId, pri, modemId);

    return result;
}

/* 根据流控Id来获取流控点 */
STATIC FC_Point* FC_PointGet(FC_IdUint8 fcId)
{
    VOS_UINT32 fcIdIdxLoop;

    if (g_fcPointMgr.pointNum > FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_WARNING, "FC_PointGet, ERROR, g_fcPointMgr.ulPointNum overflow!\n");

        return VOS_NULL_PTR;
    }

    for (fcIdIdxLoop = 0; fcIdIdxLoop < g_fcPointMgr.pointNum; fcIdIdxLoop++) {
        if (g_fcPointMgr.fcPoint[fcIdIdxLoop].fcId == fcId) {
            return (&(g_fcPointMgr.fcPoint[fcIdIdxLoop]));
        }
    }

    return VOS_NULL_PTR;
}

/* 添加流控点 */
STATIC VOS_UINT32 FC_PointAdd(FC_RegPointPara *fcRegPoint)
{
    FC_Point               *point = VOS_NULL_PTR;
    VOS_UINT32              fcPointLoop;
    FC_PrivatePolicyIdUint8 fcPrivatePolicyId;

    fcPrivatePolicyId = g_privatePolicyTbl[fcRegPoint->modemId][fcRegPoint->policyId];

    if (g_fcPointMgr.pointNum > FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_WARNING, "FC_PointAdd, WARNING, Reach maximun point number!\n");
        return VOS_ERR;
    }

    for (fcPointLoop = 0; fcPointLoop < g_fcPointMgr.pointNum; fcPointLoop++) {
        if (g_fcPointMgr.fcPoint[fcPointLoop].fcId == fcRegPoint->fcId) {
            /* 将该点新的策略加入到原来该点的策略中 */
            point = &(g_fcPointMgr.fcPoint[fcPointLoop]);
            point->policyMask |= FC_POLICY_MASK(fcPrivatePolicyId);
            point->modemId = fcRegPoint->modemId;

            /* 打印一个提示，表明该点被多个策略添加 */
            FC_LOG1(
                PS_PRINT_NORMAL, "FC_PointAdd, NORMAL, The same point has added <1>!\n", (VOS_INT32)fcRegPoint->fcId);

            return VOS_OK;
        }
    }

    if (g_fcPointMgr.pointNum == FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_WARNING, "FC_PointAdd, WARNING, g_fcPointMgr.ulPointNum reaches the MAXNUM!\n");
        return VOS_ERR;
    }

    /* 如果没有找到，那么就是一个新点，需要加入到管理结构中 */
    point = &(g_fcPointMgr.fcPoint[g_fcPointMgr.pointNum]);

    point->fcId       = fcRegPoint->fcId;
    point->policyMask = FC_POLICY_MASK(fcPrivatePolicyId);
    point->fcMask     = 0;
    point->param1     = fcRegPoint->param1;
    point->param2     = fcRegPoint->param2;
    point->setFunc    = fcRegPoint->setFunc;
    point->clrFunc    = fcRegPoint->clrFunc;
    point->rstFunc    = fcRegPoint->rstFunc;

    g_fcPointMgr.pointNum++;

    return VOS_OK;
}

/* 从流控点管理模块删除某一个流控点 */
VOS_UINT32 FC_PointDel(FC_IdUint8 fcId)
{
    VOS_UINT32 fcIdIdxLoop;
    VOS_UINT32 fcShiftId;
    FC_Point  *fcPoint = VOS_NULL_PTR;

    /* 防止全局变量被踩，导致后面数组访问越界 */
    if (g_fcPointMgr.pointNum > FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_WARNING, "FC_PointDel, WARNING, g_fcPointMgr.ulPointNum overflow!\n");

        return VOS_ERR;
    }

    for (fcIdIdxLoop = 0; fcIdIdxLoop < g_fcPointMgr.pointNum; fcIdIdxLoop++) {
        if (g_fcPointMgr.fcPoint[fcIdIdxLoop].fcId == fcId) {
            for (fcShiftId = fcIdIdxLoop + 1; fcShiftId < g_fcPointMgr.pointNum; fcShiftId++) {
                fcPoint = &(g_fcPointMgr.fcPoint[fcShiftId]);

                (VOS_VOID)memcpy_s((fcPoint - 1), sizeof(FC_Point), fcPoint, sizeof(FC_Point));
            }

            /* 最后一个需要手动清除 */
            (VOS_VOID)memset_s(&(g_fcPointMgr.fcPoint[fcShiftId - 1]), sizeof(FC_Point), 0, sizeof(FC_Point));

            g_fcPointMgr.pointNum--;

            return VOS_OK;
        }
    }

    return VOS_OK;
}

/* 请求对应ID的流控点启动流控 */
FC_PriOperUint32 FC_PointSetFc(VOS_UINT32 policyMask, FC_IdUint8 fcId)
{
    FC_Point  *fcPoint = VOS_NULL_PTR;
    VOS_UINT32 needSet;
    VOS_UINT32 result;

    /* 根据流控点,判决是否需要进行流控操作 */
    needSet = PS_FALSE;
    result  = VOS_OK;

    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /* 掩码从0到非0，发生翻转，需要执行流控操作，否则说明之前该流控点已经处于流控状态 */
    if (fcPoint->fcMask == 0) {
        /* 执行流控函数 */
        needSet = VOS_TRUE;
    }

    fcPoint->fcMask |= policyMask;

    /* 执行流控 */
    if (needSet == PS_FALSE) {
        FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_SET_FC, fcPoint, needSet, result);
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    if (fcPoint->setFunc != VOS_NULL_PTR) {
        result = fcPoint->setFunc(fcPoint->param1, fcPoint->param2);
    }

    FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_SET_FC, fcPoint, needSet, result);

    return FC_PRI_CHANGE_AND_BREAK;
}

/* 请求对应ID的流控点解除流控 */
FC_PriOperUint32 FC_PointClrFc(VOS_UINT32 policyMask, FC_IdUint8 fcId)
{
    /*  根据ID找到流控点实例，执行实例的流控解除函数 */
    FC_Point  *fcPoint = VOS_NULL_PTR;
    VOS_UINT32 needSet;
    VOS_UINT32 result;

    /* 根据流控点,判决是否需要进行解除流控操作 */
    needSet = PS_FALSE;
    result  = VOS_OK;

    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /* 并没有因为该种类型导致流控，无须解除流控 */
    if ((fcPoint->fcMask & policyMask) == 0) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    fcPoint->fcMask &= ~policyMask;

    if (fcPoint->fcMask == 0) {
        /* 只有所有情况都解除了，才能解除流控 */
        needSet = VOS_TRUE;
    }

    /* 执行解除流控的操作 */
    if (needSet == PS_FALSE) {
        FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_CLR_FC, fcPoint, needSet, result);
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    if (fcPoint->clrFunc != VOS_NULL_PTR) {
        result = fcPoint->clrFunc(fcPoint->param1, fcPoint->param2);
    }

    FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_CLR_FC, fcPoint, needSet, result);

    return FC_PRI_CHANGE_AND_BREAK;
}

/* 流控点注册内部处理函数 */
VOS_UINT32 FC_PointReg(FC_RegPointPara *fcRegPoint)
{
    FC_PriTypeUint8         pri;
    FC_PrivatePolicyIdUint8 fcPrivatePolicyId;

    fcPrivatePolicyId = g_privatePolicyTbl[fcRegPoint->modemId][fcRegPoint->policyId];

    /* 添加流控点 */
    if (FC_PointAdd(fcRegPoint) != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_PointReg, ERROR, FC_PointAdd error!\n");
        return VOS_ERR;
    }

    /* 如果该流控策略里已经存在该流控ID，直接返回 */
    pri = FC_PolicyGetPriWithFcId(fcPrivatePolicyId, fcRegPoint->fcId);

    if (pri != FC_PRI_BUTT) {
        FC_LOG(PS_PRINT_ERROR, "FC_PointReg, ERROR, Policy has same FC Id\n");
        return VOS_ERR;
    }

    (VOS_VOID)FC_PolicyAddPoint(fcPrivatePolicyId, fcRegPoint->fcId, fcRegPoint->fcPri);

    return VOS_OK;
}

/* 去注册流控点 */
VOS_UINT32 FC_PointDeReg(FC_IdUint8 fcId, ModemIdUint16 modemId)
{
    FC_Point               *fcPoint = VOS_NULL_PTR;
    FC_PrivatePolicyIdUint8 policyId;
    VOS_UINT32              pointPolicyMask;

    /* 通过流控点ID找到流控点结构 */
    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    /* 防止另一张卡误调 */
    if (modemId != fcPoint->modemId) {
        return VOS_ERR;
    }

    /* 流控点删除，需要对相应的流控策略进行处理 */
    pointPolicyMask = fcPoint->policyMask;

    for (policyId = FC_PRIVATE_POLICY_ID_MEM_MODEM_0; policyId < FC_PRIVATE_POLICY_ID_BUTT; policyId++) {
        if ((FC_POLICY_MASK(policyId) & pointPolicyMask) != 0) {
            /* 使能检查 */
            if (FC_IsPolicyEnable(FC_POLICY_MASK(policyId), modemId) != 0) {
                (VOS_VOID)FC_PolicyDelPoint(policyId, fcId);
            }
        }
    }

    /* 从流控点池里删除该流控点 */
    (VOS_VOID)FC_PointDel(fcId);

    return VOS_OK;
}

/* 更改POINT属性 */
VOS_UINT32 FC_PointChange(FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 newPri, ModemIdUint16 modemId)
{
    FC_Point               *fcPoint = VOS_NULL_PTR;
    FC_PriTypeUint8         oldPri;
    FC_PrivatePolicyIdUint8 fcPrivatePolicyId;

    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    fcPrivatePolicyId = g_privatePolicyTbl[modemId][policyId];
    oldPri            = FC_PolicyGetPriWithFcId(fcPrivatePolicyId, fcId);

    if (oldPri >= FC_PRI_BUTT) {
        FC_LOG3(PS_PRINT_WARNING, "FC, FC_PointChange, WARNING, oldPri <1> err with PolicyId <2>, FcId <3>\n",
            (VOS_INT32)oldPri, (VOS_INT32)fcPrivatePolicyId, (VOS_INT32)fcId);
        return VOS_ERR;
    }

    if (newPri == oldPri) {
        FC_LOG3(PS_PRINT_WARNING, "FC, FC_PointChange, WARNING, NewPri <1> eqaul old with PolicyId <2>, FcId <3>\n",
            (VOS_INT32)newPri, (VOS_INT32)fcPrivatePolicyId, (VOS_INT32)fcId);
        return VOS_ERR;
    }

    (VOS_VOID)FC_PolicyChangePoint(fcPrivatePolicyId, fcId, oldPri, newPri);

    return VOS_OK;
}

/* 流控点模块初始化 */
STATIC VOS_VOID FC_PointInit(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_fcPointMgr, sizeof(FC_PointMgr), 0, sizeof(FC_PointMgr));
}

/* 去除该流控策略当前优先级及更低的无效优先级 */
STATIC VOS_VOID FC_PolicyTrimInvalidPri(FC_Policy *fcPolicy)
{
    FC_Pri         *fcPri = VOS_NULL_PTR;
    FC_PriTypeUint8 priVal;

    /* 如果当前优先级无效，则直接返回，无需降级 */
    if (fcPolicy->donePri == FC_PRI_NULL) {
        return;
    }

    /* 找到一个有效的优先级，则跳出循环，不改变当前流控优先级 */
    for (priVal = fcPolicy->donePri; priVal > FC_PRI_NULL; priVal--) {
        fcPri = &(fcPolicy->fcPri[priVal]);
        if (fcPri->valid == VOS_TRUE) {
            break;
        }
    }

    /* 如果当前优先级以及更低的所有优先级均为无效，则将当前优先级直接置为最低优先级 */
    fcPolicy->donePri = priVal;

    return;
}

/* 执行一次当前优先级的所有流控点的流控操作 */
STATIC FC_PriOperUint32 FC_PolicyUpWithOnePri(FC_Policy *fcPolicy, FC_Pri *pri)
{
    VOS_UINT32 loop;

    if (pri->fcIdCnt > FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_ERROR, "FC_PolicyUpWithOnePri, ERROR, FcIdCnt overflow!\n");

        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    for (loop = 0; loop < pri->fcIdCnt; loop++) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK(fcPolicy->policyId), pri->fcId[loop]);
    }

    return FC_PRI_CHANGE_AND_BREAK;
}

/* 执行一次当前优先级的所有流控点的解流控操作 */
STATIC FC_PriOperUint32 FC_PolicyDownWithOnePri(FC_Policy *policy, FC_Pri *pri)
{
    VOS_UINT32 loop;

    if (pri->fcIdCnt > FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_ERROR, "FC_PolicyDownWithOnePri, ERROR, FcIdCnt overflow!\n");

        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    for (loop = 0; loop < pri->fcIdCnt; loop++) {
        (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK(policy->policyId), pri->fcId[loop]);
    }

    return FC_PRI_CHANGE_AND_BREAK;
}

/* 流控级别递增一个有效级别 */
VOS_UINT32 FC_PolicyUp(FC_Policy *policy)
{
    FC_Pri         *fcPri = VOS_NULL_PTR;
    FC_PriTypeUint8 priVal;

    if (policy->donePri >= policy->highestPri) {
        return VOS_OK;
    }

    while (policy->donePri < policy->highestPri) {
        priVal = (FC_PriTypeUint8)(policy->donePri + 1);
        fcPri  = &(policy->fcPri[priVal]);

        /* 当前优先级存在，执行当前流控操作 */
        if (fcPri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyUpWithOnePri(policy, fcPri);
            policy->donePri++;
            break;
        }

        policy->donePri++;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_UP, policy);

    return VOS_OK;
}

/* 流控到指定流控级别 */
VOS_UINT32 FC_PolicyUpToTargetPri(FC_Policy *fcPolicy, FC_PriTypeUint8 targetPri)
{
    FC_Pri         *fcPri = VOS_NULL_PTR;
    FC_PriTypeUint8 priVal;

    if (targetPri >= FC_PRI_BUTT) {
        return VOS_ERR;
    }

    if (fcPolicy->donePri >= targetPri) {
        return VOS_OK;
    }

    while (fcPolicy->donePri < targetPri) {
        priVal = (FC_PriTypeUint8)(fcPolicy->donePri + 1);
        fcPri  = &(fcPolicy->fcPri[priVal]);

        /* 当前优先级存在，执行当前流控操作 */
        if (fcPri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyUpWithOnePri(fcPolicy, fcPri);
        }

        fcPolicy->donePri++;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_UP, fcPolicy);

    return VOS_OK;
}

/* 流控级别递减一个有效级别 */
VOS_UINT32 FC_PolicyDown(FC_Policy *fcPolicy)
{
    FC_Pri *pri = VOS_NULL_PTR;

    if (fcPolicy->donePri == FC_PRI_NULL) {
        return VOS_OK;
    }

    while (fcPolicy->donePri > FC_PRI_NULL) {
        pri = &(fcPolicy->fcPri[fcPolicy->donePri]);

        /* 当前优先级存在，执行当前优先级的解流控操作 */
        if (pri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyDownWithOnePri(fcPolicy, pri);
            fcPolicy->donePri--;
            break;
        }

        fcPolicy->donePri--;
    }

    /* 去除多余的无效优先级 */
    FC_PolicyTrimInvalidPri(fcPolicy);

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, fcPolicy);

    return VOS_OK;
}

/* 解除流控到指定流控级别 */
VOS_UINT32 FC_PolicyDownToTargetPri(FC_Policy *fcPolicy, FC_PriTypeUint8 targetPri)
{
    FC_Pri *pri = VOS_NULL_PTR;

    if (fcPolicy->donePri <= targetPri) {
        return VOS_OK;
    }

    while (fcPolicy->donePri > targetPri) {
        pri = &(fcPolicy->fcPri[fcPolicy->donePri]);

        /* 当前优先级存在，执行当前优先级的解流控操作 */
        if (pri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyDownWithOnePri(fcPolicy, pri);
        }

        fcPolicy->donePri--;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, fcPolicy);

    return VOS_OK;
}

/* 更新该流控策略的最高优先级 */
STATIC VOS_VOID FC_PolicyUpdateHighestPri(FC_Policy *policy)
{
    FC_PriTypeUint8 pri;

    policy->highestPri = FC_PRI_NULL;

    for (pri = FC_PRI_1; pri < FC_PRI_BUTT; pri++) {
        if (policy->fcPri[pri].valid == VOS_TRUE) {
            policy->highestPri = pri;
        }
    }

    return;
}

/* 添加流控点 */
STATIC VOS_UINT32 FC_PolicyAddPointForPri(FC_Policy *policy, FC_IdUint8 fcId, FC_PriTypeUint8 pointPri)
{
    FC_Pri    *fcPri = VOS_NULL_PTR;
    VOS_UINT32 fcIdxLoop;

    if (pointPri >= FC_PRI_BUTT) {
        FC_LOG(PS_PRINT_ERROR, "FC_PolicyAddPointForPri, ERROR, enPointPri overflow!\n");

        return VOS_ERR;
    }

    fcPri = &(policy->fcPri[pointPri]);

    if (fcPri->fcIdCnt >= FC_MAX_POINT_NUM) {
        FC_LOG1(PS_PRINT_WARNING, "FC_PolicyAddPointForPri, WARNING, added FcId overflow\n", (VOS_INT32)fcId);
        return VOS_ERR;
    }

    /* 如果当前优先级已存在，则添加该流控点 */
    if (fcPri->valid == VOS_TRUE) {
        for (fcIdxLoop = 0; fcIdxLoop < fcPri->fcIdCnt; fcIdxLoop++) {
            if (fcId == fcPri->fcId[fcIdxLoop]) {
                /* 已经注册过相同ID，直接返回 */
                FC_LOG1(PS_PRINT_WARNING, "FC_PolicyAddPointForPri, WARNING, add added FcId <1>\n", (VOS_INT32)fcId);
                return VOS_ERR;
            }
        }

        fcPri->fcId[fcPri->fcIdCnt] = fcId;
        fcPri->fcIdCnt++;
    }
    /* 如果当前优先级不存在，则该优先级置为有效 */
    else {
        fcPri->fcId[0] = fcId;
        fcPri->fcIdCnt = 1;
        fcPri->valid   = VOS_TRUE;
        policy->priCnt++;
    }

    return VOS_OK;
}

/* 删除流控点 */
STATIC VOS_UINT32 FC_PolicyDelPointForPri(FC_Policy *policy, FC_IdUint8 fcId, FC_PriTypeUint8 pointPri)
{
    FC_Pri    *fcPri = VOS_NULL_PTR;
    VOS_UINT32 fcIdIdxLoop;
    VOS_UINT32 fcShiftId;

    if (pointPri >= FC_PRI_BUTT) {
        FC_LOG(PS_PRINT_ERROR, "FC_PolicyDelPointForPri, ERROR, enPointPri overflow!\n");
        return VOS_ERR;
    }

    fcPri = &(policy->fcPri[pointPri]);

    for (fcIdIdxLoop = 0; fcIdIdxLoop < fcPri->fcIdCnt; fcIdIdxLoop++) {
        if (fcId == fcPri->fcId[fcIdIdxLoop]) {
            /* 删除流控点，进行数组移位 */
            fcPri->fcId[fcIdIdxLoop] = 0;
            for (fcShiftId = (fcIdIdxLoop + 1); fcShiftId < fcPri->fcIdCnt; fcShiftId++) {
                fcPri->fcId[fcShiftId - 1] = fcPri->fcId[fcShiftId];
                fcPri->fcId[fcShiftId]     = 0;
            }

            fcPri->fcIdCnt--;

            /* 该优先级删除了所有的流控点，则删除该优先级 */
            if (fcPri->fcIdCnt == 0) {
                fcPri->valid = PS_FALSE;
                policy->priCnt--;
            }

            return VOS_OK;
        }
    }

    return VOS_OK;
}

/* 流控策略增加流控点 */
STATIC VOS_UINT32 FC_PolicyAddPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointPri)
{
    FC_Policy *policy = VOS_NULL_PTR;

    policy = FC_POLICY_GET(policyId);

    /* 向流控策略添加流控点 */
    FC_PolicyAddPointForPri(policy, fcId, pointPri);

    /* 更新该流控策略最高优先级 */
    FC_PolicyUpdateHighestPri(policy);

    /* 调整流控策略，增加流控点，按照流控门限进行重新调整 */
    if (policy->adjustForUpFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForUpFunc(pointPri, fcId);
    }

    /* 新流控点的流控判决处理,如果注册的流控优先级低于当前优先级，则执行该流控点的流控 */
    if (pointPri <= policy->donePri) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK(policyId), fcId);
    }

    /* 输出可维可测 */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_ADD_POINT, policy);

    return VOS_OK;
}

/* 通过FcId获得流控优先级 */
STATIC FC_PriTypeUint8 FC_PolicyGetPriWithFcId(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId)
{
    FC_Policy      *policy = VOS_NULL_PTR;
    FC_Pri         *pri    = VOS_NULL_PTR;
    FC_PriTypeUint8 priLoop;
    VOS_UINT32      fcIdIndexLoop;
    FC_PriTypeUint8 pointPri;

    policy = FC_POLICY_GET(policyId);

    for (priLoop = FC_PRI_1; priLoop < FC_PRI_BUTT; priLoop++) {
        pri = &(policy->fcPri[priLoop]);

        if (pri->fcIdCnt > FC_MAX_POINT_NUM) {
            FC_LOG(PS_PRINT_ERROR, "FC_PolicyGetPriWithFcId, ERROR, ucFcIdCnt overflow!\n");

            continue;
        }

        for (fcIdIndexLoop = 0; fcIdIndexLoop < pri->fcIdCnt; fcIdIndexLoop++) {
            if (pri->fcId[fcIdIndexLoop] == fcId) {
                /* 从该流控策略里删除该流控点 */
                pointPri = priLoop;
                return pointPri;
            }
        }
    }

    return FC_PRI_BUTT;
}

/* 删除流控策略的流控ID */
STATIC VOS_UINT32 FC_PolicyDelPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId)
{
    FC_PriTypeUint8 pointPri;
    FC_Policy      *policy = VOS_NULL_PTR;

    /* 对需要删除的流控点进行解除流控处理 */
    (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK(policyId), fcId);

    /* 对相关连的流控策略进行调整 */
    pointPri = FC_PolicyGetPriWithFcId(policyId, fcId);
    if (pointPri == FC_PRI_BUTT) {
        return VOS_OK;
    }

    policy = FC_POLICY_GET(policyId);
    (VOS_VOID)FC_PolicyDelPointForPri(policy, fcId, pointPri);

    /* 更新该流控策略最高优先级 */
    FC_PolicyUpdateHighestPri(policy);

    /* 流控策略调整 */
    if (policy->adjustForDownFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForDownFunc(pointPri, fcId);
    }

    /* 输出可维可测 */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DEL_POINT, policy);

    return VOS_OK;
}

/* 更改流控点的优先级 */
STATIC VOS_UINT32 FC_PolicyChangePoint(
    FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointOldPri, FC_PriTypeUint8 pointNewPri)
{
    FC_Policy *policy = VOS_NULL_PTR;

    policy = FC_POLICY_GET(policyId);

    /* 刷新该流控点在流控策略的流控点维护 */
    (VOS_VOID)FC_PolicyDelPointForPri(policy, fcId, pointOldPri);
    FC_PolicyAddPointForPri(policy, fcId, pointNewPri);

    /* 更新该流控策略最高优先级 */
    FC_PolicyUpdateHighestPri(policy);

    /* 调整流控策略，更改流控点，按照流控门限进行重新调整 */
    if (policy->adjustForUpFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForUpFunc(pointOldPri, fcId);
    }

    /* 针对优先级的更改进行流控点状态的调整,如果老的优先级低于当前优先级，新的优先级高于当前优先级，则解除该流控点的流控 */
    if ((pointOldPri < policy->donePri) && (pointNewPri > policy->donePri)) {
        (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK(policy->policyId), fcId);
    }

    /* 如果老的优先级高于当前优先级，新的优先级低于当前优先级，则对该流控点进行流控 */
    if ((pointOldPri > policy->donePri) && (pointNewPri < policy->donePri)) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK(policy->policyId), fcId);
    }

    /* 输出可维可测 */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_CHANGE_POINT, policy);

    return VOS_OK;
}

/* 流控策略实体初始化 */
STATIC VOS_UINT32 FC_PolicyInit(VOS_VOID)
{
    FC_PolicyIdUint8 policyId;
    VOS_UINT32       fcPrivatePolicyId;

    fcPrivatePolicyId = sizeof(FC_Policy) * FC_PRIVATE_POLICY_ID_BUTT;

    /* 通用流控策略初始化 */
    (VOS_VOID)memset_s(&g_fcPolicy[0], fcPrivatePolicyId, 0x00, fcPrivatePolicyId);

    for (policyId = 0; policyId < FC_PRIVATE_POLICY_ID_BUTT; policyId++) {
        g_fcPolicy[policyId].policyId = (VOS_UINT8)policyId;
    }

    return VOS_OK;
}

/* CPU流控NV参数合法性检查 */
VOS_UINT32 FC_CfgCheckCpuParam(FC_CFG_CPU_STRU *fcCfgCpu)
{
    if (fcCfgCpu->cpuOverLoadVal < fcCfgCpu->cpuUnderLoadVal) {
        FC_LOG2(PS_PRINT_WARNING, "FC, FC_CfgCheckCpuParam, WARNING, overLoad <1> less than underLoad <2>\n",
            (VOS_INT32)fcCfgCpu->cpuOverLoadVal, (VOS_INT32)fcCfgCpu->cpuUnderLoadVal);
        return VOS_ERR;
    }

    if ((fcCfgCpu->cpuOverLoadVal == 0) || (fcCfgCpu->cpuOverLoadVal > 100)) {
        FC_LOG2(PS_PRINT_WARNING, "FC, FC_CfgCheckCpuParam, WARNING, invalid value overLoad <1>, underLoad <2>\n",
            (VOS_INT32)fcCfgCpu->cpuOverLoadVal, (VOS_INT32)fcCfgCpu->cpuUnderLoadVal);
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 发送CPU流控消息 */
VOS_UINT32 FC_SndCpuMsg(FC_MsgTypeUint16 msgName, VOS_UINT32 cpuLoad, VOS_UINT32 ulRate, VOS_UINT32 dlRate)
{
    VOS_UINT32     result;
    FC_CpuLoadInd *msg = VOS_NULL_PTR;

    msg = (FC_CpuLoadInd*)VOS_AllocMsg(UEPS_PID_FLOWCTRL, sizeof(FC_CpuLoadInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        /* 该函数会在中断中被调用，故使用LogPrint */
        FC_LOG(PS_PRINT_ERROR, "FC_SndIntraCpuMsg, ERROR, Alloc Msg Fail\n");
        return VOS_ERR;
    }
    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName = msgName;
    msg->cpuLoad = cpuLoad;
    msg->ulRate  = ulRate;
    msg->dlRate  = dlRate;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        /* 该函数会在中断中被调用，故使用LogPrint */
        FC_LOG(PS_PRINT_ERROR, "FC_SndIntraCpuMsg, ERROR, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 发送注册流控点消息 */
STATIC VOS_UINT32 FC_SndRegPointMsg(FC_RegPointPara *fcRegPoint)
{
    VOS_UINT32      result;
    FC_RegPointInd *msg = VOS_NULL_PTR;

    msg = (FC_RegPointInd*)VOS_AllocMsg(UEPS_PID_FLOWCTRL, sizeof(FC_RegPointInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndRegPointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName = ID_FC_REG_POINT_IND;

    (VOS_VOID)memcpy_s(&(msg->fcPoint), sizeof(FC_RegPointPara), fcRegPoint, sizeof(FC_RegPointPara));

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndRegPointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 发送注册流控点消息 */
STATIC VOS_UINT32 FC_SndDeRegPointMsg(FC_IdUint8 fcId, ModemIdUint16 modemId)
{
    VOS_UINT32        result;
    FC_DeregPointInd *msg = VOS_NULL_PTR;

    msg = (FC_DeregPointInd*)VOS_AllocMsg(UEPS_PID_FLOWCTRL, sizeof(FC_DeregPointInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndDeRegPointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName = ID_FC_DEREG_POINT_IND;
    msg->fcId    = fcId;
    msg->modemId = modemId;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndDeRegPointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 流控点优先级改变消息 */
STATIC VOS_UINT32 FC_SndChangePointMsg(
    FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 pri, ModemIdUint16 modemId)
{
    VOS_UINT32         result;
    FC_ChangePointInd *msg = VOS_NULL_PTR;

    msg = (FC_ChangePointInd*)VOS_AllocMsg(UEPS_PID_FLOWCTRL, sizeof(FC_ChangePointInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndChangePointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL);
    msg->msgName  = ID_FC_CHANGE_POINT_IND;
    msg->fcId     = fcId;
    msg->policyId = policyId;
    msg->pri      = pri;
    msg->modemId  = modemId;

    result = PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    if (result != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_SndChangePointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 两核流控模块初始化 */
VOS_UINT32 FC_CommInit(VOS_VOID)
{
    /* 流控策略初始化 */
    FC_PolicyInit();

    /* 流控点初始化 */
    FC_PointInit();

    return VOS_OK;
}

/*lint -restore */
/*lint +e534*/

