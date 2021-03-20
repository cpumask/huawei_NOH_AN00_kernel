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

/* ���ز���ʵ�� */
FC_Policy g_fcPolicy[FC_PRIVATE_POLICY_ID_BUTT];

/* ���ص����ʵ�� */
FC_PointMgr g_fcPointMgr;

/* �����ڲ����Ա� */
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

/* �����ά�ɲ� */
VOS_VOID FC_MNTN_TraceEvent(VOS_VOID *msg)
{
    mdrv_diag_trace_report(msg);

    return;
}

/* ����ִ�п�ά�ɲ� */
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

/* ������ز��ԵĿ�ά�ɲ� */
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

/* �����ǰCPULoad */
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

/* �����������뺯�� */
VOS_VOID FC_SetFcEnableMask(VOS_UINT32 enableMask)
{
    g_fcEnableMask = enableMask;

    return;
}

/* �ж������ڲ������Ƿ�ʹ�� */
STATIC VOS_UINT32 FC_IsPolicyEnable(VOS_UINT32 pointPolicyMask, ModemIdUint16 modemId)
{
    VOS_UINT32 policyMask;

    /* ����Modem1,�ڲ��������ж�ʹ��ʱ����Ҫ����FC_POLICY_ID_BUTT������NV����ʹ��λ�Ƚ� */
    if (modemId == MODEM_ID_0) {
        policyMask = pointPolicyMask;
    } else {
        policyMask = (pointPolicyMask >> FC_POLICY_ID_BUTT);
    }

    return (policyMask & g_fcEnableMask);
}

/* �������ע�����ص�,���ⲿģ����� */
VOS_UINT32 FC_RegPoint(FC_RegPointPara *fcRegPoint)
{
    VOS_UINT32 result;

    /* ������� */
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

    /* ʹ�ܼ�� */
    if (FC_POLICY_MASK(fcRegPoint->policyId) != (FC_POLICY_MASK(fcRegPoint->policyId) & g_fcEnableMask)) {
        /* �ڴ�����δʹ�� */
        FC_LOG1(PS_PRINT_INFO, "FC_RegPoint, INFO, MEM FlowCtrl is disabled <1> \n", (VOS_INT32)g_fcEnableMask);
        return VOS_OK;
    }

    /* ע���ڴ���ԣ����ȼ�ֻ�����ڴ�����Լ���ķ�Χ֮�� */
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

/* ����ȥע�����ص� */
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

    /* ͨ�����ص�ID�ҵ����ص�ṹ */
    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        /* �����ز���δʹ�ܣ��򷵻�VOS_OK */
        FC_LOG1(PS_PRINT_WARNING, "FC_DeRegPoint, WARNING, This FCPonit = <1> didn't Reg!\n", (VOS_INT32)fcId);
        return VOS_OK;
    }

    /* ���ص�ɾ������Ҫ����Ӧ�����ز��Խ��д��� */
    pointPolicyMask = fcPoint->policyMask;

    if (FC_IsPolicyEnable(pointPolicyMask, modemId) != 0) {
        result = FC_SndDeRegPointMsg(fcId, modemId);

        return result;
    }

    return VOS_OK;
}

/* �������ص� */
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

    /* ʹ�ܼ�� */
    if (FC_POLICY_MASK(policyId) != (FC_POLICY_MASK(policyId) & g_fcEnableMask)) {
        /* �ڴ�����δʹ�� */
        FC_LOG1(PS_PRINT_INFO, "FC_ChangePoint, INFO, MEM FlowCtrl is disabled <1> \n", (VOS_INT32)g_fcEnableMask);
        return VOS_OK;
    }

    result = FC_SndChangePointMsg(fcId, policyId, pri, modemId);

    return result;
}

/* ��������Id����ȡ���ص� */
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

/* ������ص� */
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
            /* ���õ��µĲ��Լ��뵽ԭ���õ�Ĳ����� */
            point = &(g_fcPointMgr.fcPoint[fcPointLoop]);
            point->policyMask |= FC_POLICY_MASK(fcPrivatePolicyId);
            point->modemId = fcRegPoint->modemId;

            /* ��ӡһ����ʾ�������õ㱻���������� */
            FC_LOG1(
                PS_PRINT_NORMAL, "FC_PointAdd, NORMAL, The same point has added <1>!\n", (VOS_INT32)fcRegPoint->fcId);

            return VOS_OK;
        }
    }

    if (g_fcPointMgr.pointNum == FC_MAX_POINT_NUM) {
        FC_LOG(PS_PRINT_WARNING, "FC_PointAdd, WARNING, g_fcPointMgr.ulPointNum reaches the MAXNUM!\n");
        return VOS_ERR;
    }

    /* ���û���ҵ�����ô����һ���µ㣬��Ҫ���뵽����ṹ�� */
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

/* �����ص����ģ��ɾ��ĳһ�����ص� */
VOS_UINT32 FC_PointDel(FC_IdUint8 fcId)
{
    VOS_UINT32 fcIdIdxLoop;
    VOS_UINT32 fcShiftId;
    FC_Point  *fcPoint = VOS_NULL_PTR;

    /* ��ֹȫ�ֱ������ȣ����º����������Խ�� */
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

            /* ���һ����Ҫ�ֶ���� */
            (VOS_VOID)memset_s(&(g_fcPointMgr.fcPoint[fcShiftId - 1]), sizeof(FC_Point), 0, sizeof(FC_Point));

            g_fcPointMgr.pointNum--;

            return VOS_OK;
        }
    }

    return VOS_OK;
}

/* �����ӦID�����ص��������� */
FC_PriOperUint32 FC_PointSetFc(VOS_UINT32 policyMask, FC_IdUint8 fcId)
{
    FC_Point  *fcPoint = VOS_NULL_PTR;
    VOS_UINT32 needSet;
    VOS_UINT32 result;

    /* �������ص�,�о��Ƿ���Ҫ�������ز��� */
    needSet = PS_FALSE;
    result  = VOS_OK;

    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /* �����0����0��������ת����Ҫִ�����ز���������˵��֮ǰ�����ص��Ѿ���������״̬ */
    if (fcPoint->fcMask == 0) {
        /* ִ�����غ��� */
        needSet = VOS_TRUE;
    }

    fcPoint->fcMask |= policyMask;

    /* ִ������ */
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

/* �����ӦID�����ص������� */
FC_PriOperUint32 FC_PointClrFc(VOS_UINT32 policyMask, FC_IdUint8 fcId)
{
    /*  ����ID�ҵ����ص�ʵ����ִ��ʵ�������ؽ������ */
    FC_Point  *fcPoint = VOS_NULL_PTR;
    VOS_UINT32 needSet;
    VOS_UINT32 result;

    /* �������ص�,�о��Ƿ���Ҫ���н�����ز��� */
    needSet = PS_FALSE;
    result  = VOS_OK;

    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /* ��û����Ϊ�������͵������أ����������� */
    if ((fcPoint->fcMask & policyMask) == 0) {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    fcPoint->fcMask &= ~policyMask;

    if (fcPoint->fcMask == 0) {
        /* ֻ���������������ˣ����ܽ������ */
        needSet = VOS_TRUE;
    }

    /* ִ�н�����صĲ��� */
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

/* ���ص�ע���ڲ������� */
VOS_UINT32 FC_PointReg(FC_RegPointPara *fcRegPoint)
{
    FC_PriTypeUint8         pri;
    FC_PrivatePolicyIdUint8 fcPrivatePolicyId;

    fcPrivatePolicyId = g_privatePolicyTbl[fcRegPoint->modemId][fcRegPoint->policyId];

    /* ������ص� */
    if (FC_PointAdd(fcRegPoint) != VOS_OK) {
        FC_LOG(PS_PRINT_ERROR, "FC_PointReg, ERROR, FC_PointAdd error!\n");
        return VOS_ERR;
    }

    /* ��������ز������Ѿ����ڸ�����ID��ֱ�ӷ��� */
    pri = FC_PolicyGetPriWithFcId(fcPrivatePolicyId, fcRegPoint->fcId);

    if (pri != FC_PRI_BUTT) {
        FC_LOG(PS_PRINT_ERROR, "FC_PointReg, ERROR, Policy has same FC Id\n");
        return VOS_ERR;
    }

    (VOS_VOID)FC_PolicyAddPoint(fcPrivatePolicyId, fcRegPoint->fcId, fcRegPoint->fcPri);

    return VOS_OK;
}

/* ȥע�����ص� */
VOS_UINT32 FC_PointDeReg(FC_IdUint8 fcId, ModemIdUint16 modemId)
{
    FC_Point               *fcPoint = VOS_NULL_PTR;
    FC_PrivatePolicyIdUint8 policyId;
    VOS_UINT32              pointPolicyMask;

    /* ͨ�����ص�ID�ҵ����ص�ṹ */
    fcPoint = FC_PointGet(fcId);
    if (fcPoint == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    /* ��ֹ��һ�ſ���� */
    if (modemId != fcPoint->modemId) {
        return VOS_ERR;
    }

    /* ���ص�ɾ������Ҫ����Ӧ�����ز��Խ��д��� */
    pointPolicyMask = fcPoint->policyMask;

    for (policyId = FC_PRIVATE_POLICY_ID_MEM_MODEM_0; policyId < FC_PRIVATE_POLICY_ID_BUTT; policyId++) {
        if ((FC_POLICY_MASK(policyId) & pointPolicyMask) != 0) {
            /* ʹ�ܼ�� */
            if (FC_IsPolicyEnable(FC_POLICY_MASK(policyId), modemId) != 0) {
                (VOS_VOID)FC_PolicyDelPoint(policyId, fcId);
            }
        }
    }

    /* �����ص����ɾ�������ص� */
    (VOS_VOID)FC_PointDel(fcId);

    return VOS_OK;
}

/* ����POINT���� */
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

/* ���ص�ģ���ʼ�� */
STATIC VOS_VOID FC_PointInit(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_fcPointMgr, sizeof(FC_PointMgr), 0, sizeof(FC_PointMgr));
}

/* ȥ�������ز��Ե�ǰ���ȼ������͵���Ч���ȼ� */
STATIC VOS_VOID FC_PolicyTrimInvalidPri(FC_Policy *fcPolicy)
{
    FC_Pri         *fcPri = VOS_NULL_PTR;
    FC_PriTypeUint8 priVal;

    /* �����ǰ���ȼ���Ч����ֱ�ӷ��أ����轵�� */
    if (fcPolicy->donePri == FC_PRI_NULL) {
        return;
    }

    /* �ҵ�һ����Ч�����ȼ���������ѭ�������ı䵱ǰ�������ȼ� */
    for (priVal = fcPolicy->donePri; priVal > FC_PRI_NULL; priVal--) {
        fcPri = &(fcPolicy->fcPri[priVal]);
        if (fcPri->valid == VOS_TRUE) {
            break;
        }
    }

    /* �����ǰ���ȼ��Լ����͵��������ȼ���Ϊ��Ч���򽫵�ǰ���ȼ�ֱ����Ϊ������ȼ� */
    fcPolicy->donePri = priVal;

    return;
}

/* ִ��һ�ε�ǰ���ȼ����������ص�����ز��� */
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

/* ִ��һ�ε�ǰ���ȼ����������ص�Ľ����ز��� */
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

/* ���ؼ������һ����Ч���� */
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

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ز��� */
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

/* ���ص�ָ�����ؼ��� */
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

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ز��� */
        if (fcPri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyUpWithOnePri(fcPolicy, fcPri);
        }

        fcPolicy->donePri++;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_UP, fcPolicy);

    return VOS_OK;
}

/* ���ؼ���ݼ�һ����Ч���� */
VOS_UINT32 FC_PolicyDown(FC_Policy *fcPolicy)
{
    FC_Pri *pri = VOS_NULL_PTR;

    if (fcPolicy->donePri == FC_PRI_NULL) {
        return VOS_OK;
    }

    while (fcPolicy->donePri > FC_PRI_NULL) {
        pri = &(fcPolicy->fcPri[fcPolicy->donePri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ȼ��Ľ����ز��� */
        if (pri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyDownWithOnePri(fcPolicy, pri);
            fcPolicy->donePri--;
            break;
        }

        fcPolicy->donePri--;
    }

    /* ȥ���������Ч���ȼ� */
    FC_PolicyTrimInvalidPri(fcPolicy);

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, fcPolicy);

    return VOS_OK;
}

/* ������ص�ָ�����ؼ��� */
VOS_UINT32 FC_PolicyDownToTargetPri(FC_Policy *fcPolicy, FC_PriTypeUint8 targetPri)
{
    FC_Pri *pri = VOS_NULL_PTR;

    if (fcPolicy->donePri <= targetPri) {
        return VOS_OK;
    }

    while (fcPolicy->donePri > targetPri) {
        pri = &(fcPolicy->fcPri[fcPolicy->donePri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ȼ��Ľ����ز��� */
        if (pri->valid == VOS_TRUE) {
            (VOS_VOID)FC_PolicyDownWithOnePri(fcPolicy, pri);
        }

        fcPolicy->donePri--;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, fcPolicy);

    return VOS_OK;
}

/* ���¸����ز��Ե�������ȼ� */
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

/* ������ص� */
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

    /* �����ǰ���ȼ��Ѵ��ڣ�����Ӹ����ص� */
    if (fcPri->valid == VOS_TRUE) {
        for (fcIdxLoop = 0; fcIdxLoop < fcPri->fcIdCnt; fcIdxLoop++) {
            if (fcId == fcPri->fcId[fcIdxLoop]) {
                /* �Ѿ�ע�����ͬID��ֱ�ӷ��� */
                FC_LOG1(PS_PRINT_WARNING, "FC_PolicyAddPointForPri, WARNING, add added FcId <1>\n", (VOS_INT32)fcId);
                return VOS_ERR;
            }
        }

        fcPri->fcId[fcPri->fcIdCnt] = fcId;
        fcPri->fcIdCnt++;
    }
    /* �����ǰ���ȼ������ڣ�������ȼ���Ϊ��Ч */
    else {
        fcPri->fcId[0] = fcId;
        fcPri->fcIdCnt = 1;
        fcPri->valid   = VOS_TRUE;
        policy->priCnt++;
    }

    return VOS_OK;
}

/* ɾ�����ص� */
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
            /* ɾ�����ص㣬����������λ */
            fcPri->fcId[fcIdIdxLoop] = 0;
            for (fcShiftId = (fcIdIdxLoop + 1); fcShiftId < fcPri->fcIdCnt; fcShiftId++) {
                fcPri->fcId[fcShiftId - 1] = fcPri->fcId[fcShiftId];
                fcPri->fcId[fcShiftId]     = 0;
            }

            fcPri->fcIdCnt--;

            /* �����ȼ�ɾ�������е����ص㣬��ɾ�������ȼ� */
            if (fcPri->fcIdCnt == 0) {
                fcPri->valid = PS_FALSE;
                policy->priCnt--;
            }

            return VOS_OK;
        }
    }

    return VOS_OK;
}

/* ���ز����������ص� */
STATIC VOS_UINT32 FC_PolicyAddPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointPri)
{
    FC_Policy *policy = VOS_NULL_PTR;

    policy = FC_POLICY_GET(policyId);

    /* �����ز���������ص� */
    FC_PolicyAddPointForPri(policy, fcId, pointPri);

    /* ���¸����ز���������ȼ� */
    FC_PolicyUpdateHighestPri(policy);

    /* �������ز��ԣ��������ص㣬�����������޽������µ��� */
    if (policy->adjustForUpFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForUpFunc(pointPri, fcId);
    }

    /* �����ص�������о�����,���ע����������ȼ����ڵ�ǰ���ȼ�����ִ�и����ص������ */
    if (pointPri <= policy->donePri) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK(policyId), fcId);
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_ADD_POINT, policy);

    return VOS_OK;
}

/* ͨ��FcId����������ȼ� */
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
                /* �Ӹ����ز�����ɾ�������ص� */
                pointPri = priLoop;
                return pointPri;
            }
        }
    }

    return FC_PRI_BUTT;
}

/* ɾ�����ز��Ե�����ID */
STATIC VOS_UINT32 FC_PolicyDelPoint(FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId)
{
    FC_PriTypeUint8 pointPri;
    FC_Policy      *policy = VOS_NULL_PTR;

    /* ����Ҫɾ�������ص���н�����ش��� */
    (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK(policyId), fcId);

    /* ������������ز��Խ��е��� */
    pointPri = FC_PolicyGetPriWithFcId(policyId, fcId);
    if (pointPri == FC_PRI_BUTT) {
        return VOS_OK;
    }

    policy = FC_POLICY_GET(policyId);
    (VOS_VOID)FC_PolicyDelPointForPri(policy, fcId, pointPri);

    /* ���¸����ز���������ȼ� */
    FC_PolicyUpdateHighestPri(policy);

    /* ���ز��Ե��� */
    if (policy->adjustForDownFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForDownFunc(pointPri, fcId);
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DEL_POINT, policy);

    return VOS_OK;
}

/* �������ص�����ȼ� */
STATIC VOS_UINT32 FC_PolicyChangePoint(
    FC_PrivatePolicyIdUint8 policyId, FC_IdUint8 fcId, FC_PriTypeUint8 pointOldPri, FC_PriTypeUint8 pointNewPri)
{
    FC_Policy *policy = VOS_NULL_PTR;

    policy = FC_POLICY_GET(policyId);

    /* ˢ�¸����ص������ز��Ե����ص�ά�� */
    (VOS_VOID)FC_PolicyDelPointForPri(policy, fcId, pointOldPri);
    FC_PolicyAddPointForPri(policy, fcId, pointNewPri);

    /* ���¸����ز���������ȼ� */
    FC_PolicyUpdateHighestPri(policy);

    /* �������ز��ԣ��������ص㣬�����������޽������µ��� */
    if (policy->adjustForUpFunc != VOS_NULL_PTR) {
        (VOS_VOID)policy->adjustForUpFunc(pointOldPri, fcId);
    }

    /* ������ȼ��ĸ��Ľ������ص�״̬�ĵ���,����ϵ����ȼ����ڵ�ǰ���ȼ����µ����ȼ����ڵ�ǰ���ȼ������������ص������ */
    if ((pointOldPri < policy->donePri) && (pointNewPri > policy->donePri)) {
        (VOS_VOID)FC_PointClrFc(FC_POLICY_MASK(policy->policyId), fcId);
    }

    /* ����ϵ����ȼ����ڵ�ǰ���ȼ����µ����ȼ����ڵ�ǰ���ȼ�����Ը����ص�������� */
    if ((pointOldPri > policy->donePri) && (pointNewPri < policy->donePri)) {
        (VOS_VOID)FC_PointSetFc(FC_POLICY_MASK(policy->policyId), fcId);
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_CHANGE_POINT, policy);

    return VOS_OK;
}

/* ���ز���ʵ���ʼ�� */
STATIC VOS_UINT32 FC_PolicyInit(VOS_VOID)
{
    FC_PolicyIdUint8 policyId;
    VOS_UINT32       fcPrivatePolicyId;

    fcPrivatePolicyId = sizeof(FC_Policy) * FC_PRIVATE_POLICY_ID_BUTT;

    /* ͨ�����ز��Գ�ʼ�� */
    (VOS_VOID)memset_s(&g_fcPolicy[0], fcPrivatePolicyId, 0x00, fcPrivatePolicyId);

    for (policyId = 0; policyId < FC_PRIVATE_POLICY_ID_BUTT; policyId++) {
        g_fcPolicy[policyId].policyId = (VOS_UINT8)policyId;
    }

    return VOS_OK;
}

/* CPU����NV�����Ϸ��Լ�� */
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

/* ����CPU������Ϣ */
VOS_UINT32 FC_SndCpuMsg(FC_MsgTypeUint16 msgName, VOS_UINT32 cpuLoad, VOS_UINT32 ulRate, VOS_UINT32 dlRate)
{
    VOS_UINT32     result;
    FC_CpuLoadInd *msg = VOS_NULL_PTR;

    msg = (FC_CpuLoadInd*)VOS_AllocMsg(UEPS_PID_FLOWCTRL, sizeof(FC_CpuLoadInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        /* �ú��������ж��б����ã���ʹ��LogPrint */
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
        /* �ú��������ж��б����ã���ʹ��LogPrint */
        FC_LOG(PS_PRINT_ERROR, "FC_SndIntraCpuMsg, ERROR, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* ����ע�����ص���Ϣ */
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

/* ����ע�����ص���Ϣ */
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

/* ���ص����ȼ��ı���Ϣ */
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

/* ��������ģ���ʼ�� */
VOS_UINT32 FC_CommInit(VOS_VOID)
{
    /* ���ز��Գ�ʼ�� */
    FC_PolicyInit();

    /* ���ص��ʼ�� */
    FC_PointInit();

    return VOS_OK;
}

/*lint -restore */
/*lint +e534*/

