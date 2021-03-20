/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
#include "AcpuReset.h"
#include "AdsIntraMsg.h"
#include "AdsCtx.h"
#include "AdsMsgProc.h"
#include "AdsIfaceMgmt.h"
#include "AdsLog.h"
#include "ads_iface_debug.h"
#include "AdsMntn.h"
#if ((FEATURE_SHARE_APN == FEATURE_ON) || (FEATURE_SHARE_PDP == FEATURE_ON))
#include "ads_iface_filter.h"
#endif
#include "mn_comm_api.h"
#include "acore_nv_stru_gucnas.h"
#include "securec.h"
#include "ads_msg_chk.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_ADS_MAIN_C

#define ADS_V2_WAKE_LOCK_MAX 500

ADS_CONTEXT g_adsContext;
VOS_UINT32  g_adsTaskId        = 0;
VOS_UINT32  g_adsTaskReadyFlag = 0;

extern VOS_MsgHookFunc vos_MsgHook;
extern VOS_VOID        RNIC_ProcMsg(MsgBlock *msg);
extern VOS_UINT32      RNIC_PidInit(enum VOS_InitPhaseDefine phase);


VOS_SEM ADS_GetResetSem(VOS_VOID)
{
    return g_adsContext.resetSem;
}


VOS_UINT32 ADS_GetTaskReadyFlag(VOS_VOID)
{
    return g_adsTaskReadyFlag;
}


VOS_VOID ADS_SetTaskReadyFlag(VOS_UINT32 taskReadyFlag)
{
    g_adsTaskReadyFlag = taskReadyFlag;

    return;
}


VOS_UINT32 ADS_GetTaskId(VOS_VOID)
{
    return g_adsTaskId;
}


VOS_VOID ADS_SetTaskId(VOS_UINT32 taskId)
{
    g_adsTaskId = taskId;

    return;
}


ADS_CONTEXT* ADS_GetCtx(VOS_VOID)
{
    return &g_adsContext;
}


ADS_IFACE_Info* ADS_GetIfaceInfo(VOS_UINT8 ifaceId)
{
    /* ucIfaceId ��Ч���ɵ����߱�֤ */
    return &g_adsContext.ifaceInfo[ifaceId];
}


IMM_ZcHeader* ADS_GetDataQue(VOS_VOID)
{
    return &(g_adsContext.dataQue);
}


STATIC VOS_VOID ADS_ResetSemInit(VOS_VOID)
{
    g_adsContext.resetSem = VOS_NULL_PTR;

    /* ����������ź��� */
    if (VOS_OK != VOS_SmBCreate("ADS", 0, VOS_SEMA4_FIFO, &g_adsContext.resetSem)) {
        ADS_LOGH("Create ADS acpu sem failed!");
        return;
    }

    return;
}


STATIC VOS_VOID ADS_WakeLockInit(VOS_VOID)
{
    g_adsContext.wakeLockCfg.timeOut = ADS_V2_WAKE_LOCK_MAX;

    ads_iface_enable_wakelock(g_adsContext.wakeLockCfg.timeOut);

    return;
}


STATIC VOS_VOID ADS_TiCtxInit(VOS_VOID)
{
    VOS_UINT32 i;

    for (i = 0; i < ADS_MAX_TIMER_NUM; i++) {
        g_adsContext.timerCtx[i].timer = VOS_NULL_PTR;
    }

    return;
}


VOS_VOID ADS_IfaceInfoInit(VOS_UINT8 ifaceId)
{
    ADS_IFACE_Info *ifaceInfo = VOS_NULL_PTR;
    VOS_UINT32      pktType;

    ifaceInfo = ADS_GetIfaceInfo(ifaceId);

    (VOS_VOID)memset_s(ifaceInfo, sizeof(ADS_IFACE_Info), 0x00, sizeof(ADS_IFACE_Info));

    ifaceInfo->ifaceId   = ADS_INVAILD_IFACE_ID;
    ifaceInfo->stats     = ADS_IFACE_OP_TYPE_DOWN;
    ifaceInfo->transMode = ADS_TRANS_MODE_BUTT;
    ifaceInfo->modemId   = MODEM_ID_BUTT;

    for (pktType = 0; pktType < ADS_ADDR_BUTT; pktType++) {
        ifaceInfo->pduSessionTbl[pktType] = ADS_INVAILD_PDU_SESSION_ID;
    }

    return;
}


VOS_VOID ADS_AllIfaceInfoInit(VOS_VOID)
{
    ADS_CONTEXT *adsCtx = VOS_NULL_PTR;
    VOS_UINT32   ifaceId;

    adsCtx                    = ADS_GetCtx();
    adsCtx->ifaceStatsBitMask = 0;

    for (ifaceId = 0; ifaceId < PS_IFACE_ID_BUTT; ifaceId++) {
        ADS_IfaceInfoInit((VOS_UINT8)ifaceId);
    }

    return;
}



VOS_VOID ADS_CtxInit(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_adsContext, sizeof(g_adsContext), 0, sizeof(g_adsContext));

    /* ��ʼ������ӿ������� */
    ADS_AllIfaceInfoInit();

    /* ��ʼ����������� */
    IMM_ZcQueueHeadInit(ADS_GetDataQue());

    /* ��ʼ����ʱ�������� */
    ADS_TiCtxInit();

    /* ��ʼ����λ�ź��� */
    ADS_ResetSemInit();

    mdrv_diag_conn_state_notify_fun_reg(ADS_MntnDiagConnStateNotifyCB);
    IPSMNTN_RegTraceCfgNotify(ADS_MntnTraceCfgNotifyCB);

    return;
}


VOS_INT ADS_ModemResetCB(drv_reset_cb_moment_e param, VOS_INT userData)
{
    ADS_CcpuResetInd *msg = VOS_NULL_PTR;

    if (MDRV_RESET_CB_BEFORE == param) {
        ADS_LOGH("before reset: enter.");

        /* ������Ϣ */
        msg = (ADS_CcpuResetInd *)ADS_ALLOC_MSG_WITH_HDR(sizeof(ADS_CcpuResetInd));
        if (VOS_NULL_PTR == msg) {
            ADS_LOGH("before reset: alloc msg failed.");
            return VOS_ERROR;
        }

        /* ��д��Ϣͷ */
        ADS_CFG_INTRA_MSG_HDR(msg, ID_ADS_CCPU_RESET_START_IND);

        /* ����Ϣ */
        if (VOS_OK != ADS_SendMsg(msg)) {
            ADS_LOGH("before reset: send msg failed.");
            return VOS_ERROR;
        }

        /* �ȴ��ظ��ź�����ʼΪ��״̬���ȴ���Ϣ��������ź��������� */
        if (VOS_OK != VOS_SmP(ADS_GetResetSem(), ADS_RESET_TIMEOUT_LEN)) {
            ADS_LOGH("before reset: VOS_SmP failed.");
            return VOS_ERROR;
        }

        ADS_LOGH("before reset: succ.");
        return VOS_OK;
    } else if (MDRV_RESET_CB_AFTER == param) {
        ADS_LOGH("after reset enter.");

        ADS_LOGH("after reset: succ.");
        return VOS_OK;
    } else {
        return VOS_ERROR;
    }
}


VOS_UINT32 ADS_PidInit(enum VOS_InitPhaseDefine phase)
{
    switch (phase) {
        case VOS_IP_LOAD_CONFIG:

            /* �����ĳ�ʼ�� */
            ADS_CtxInit();

            /* �����ӿڳ�ʼ�� */
            ads_iface_create();

            /* ��ʼ��WAKE LOCK */
            ADS_WakeLockInit();

            /* �����ӿڻص�ע�� */
            ADS_RegIfaceCB();

#if (FEATURE_PC5_DATA_CHANNEL == FEATURE_ON)
            /* LTEV�ӿڳ�ʼ�� */
            ADS_LtevIfaceUp();
#endif

#if ((FEATURE_SHARE_APN == FEATURE_ON) || (FEATURE_SHARE_PDP == FEATURE_ON))
            ads_iface_filter_ctx_init();
#endif
            /* ������ע��ص�����������C�˵�����λ�Ĵ��� */
            mdrv_sysboot_register_reset_notify(NAS_ADS_UL_FUNC_PROC_NAME, ADS_ModemResetCB, 0,
                                               ACPU_RESET_PRIORITY_ADS_UL);

            break;

        default:
            break;
    }

    return VOS_OK;
}


VOS_VOID ADS_FidTask(VOS_UINT32 queueID, VOS_UINT32 fidValue, VOS_UINT32 para1, VOS_UINT32 para2)
{
    MsgBlock  *msg         = VOS_NULL_PTR;
    VOS_UINT32 taskID      = 0;
    VOS_UINT32 expectEvent = 0;
    VOS_UINT32 eventMask   = 0;
    VOS_UINT32 event       = 0;
    VOS_UINT32 rtn;

    taskID = VOS_GetCurrentTaskID();
    if (VOS_NULL_LONG == taskID) {
        ADS_ERROR_LOG("ADS_FidTask: ERROR, TaskID is invalid.");
        return;
    }

    if (VOS_OK != VOS_CreateEvent(taskID)) {
        ADS_ERROR_LOG("ADS_FidTask: ERROR, create event fail.");
        return;
    }

    ADS_SetTaskId(taskID);
    ADS_SetTaskReadyFlag(VOS_TRUE);

    expectEvent = ADS_EVENT_SPECIAL_DATA_IND | VOS_MSG_SYNC_EVENT;
    eventMask   = VOS_EVENT_ANY | VOS_EVENT_WAIT;

    for (;;) {
        rtn = VOS_EventRead(expectEvent, eventMask, 0, &event);
        if (VOS_OK != rtn) {
            ADS_ERROR_LOG("ADS_FidTask: ERROR, read event error.");
            continue;
        }

        /* �¼����� */
        if (VOS_MSG_SYNC_EVENT != event) {
            ADS_EventProc(event);
            continue;
        }

        msg = (MsgBlock *)VOS_GetMsg(taskID);
        if (VOS_NULL_PTR != msg) {
            if (VOS_NULL_PTR != vos_MsgHook) {
                (VOS_VOID)(vos_MsgHook)((VOS_VOID *)(msg));
            }

            if (VOS_GET_RECEIVER_ID(msg) == ACPU_PID_ADS_UL) {
                ADS_ProcMsg(msg);
            } else if (VOS_GET_RECEIVER_ID(msg) == ACPU_PID_RNIC) {
                RNIC_ProcMsg(msg);
            } else {
                ;
            }

            PS_FREE_MSG(VOS_GET_RECEIVER_ID(msg), msg);  //lint !e449
        }
    }
}


VOS_UINT32 ADS_FidInit(enum VOS_InitPhaseDefine ip)
{
    VOS_UINT32 rslt;

    switch (ip) {
        case VOS_IP_LOAD_CONFIG:

            /* ע��ADS PID */
            rslt = VOS_RegisterPIDInfo(ACPU_PID_ADS_UL, (InitFunType)ADS_PidInit, (MsgFunType)ADS_ProcMsg);
            if (VOS_OK != rslt) {
                return VOS_ERR;
            }

            /* ע��RNIC PID */
            rslt = VOS_RegisterPIDInfo(ACPU_PID_RNIC, (InitFunType)RNIC_PidInit, (MsgFunType)RNIC_ProcMsg);
            if (VOS_OK != rslt) {
                return VOS_ERR;
            }

            /* ע��TASK Entry */
            rslt = VOS_RegisterMsgTaskEntry(ACPU_FID_ADS_UL, ADS_FidTask);
            if (VOS_OK != rslt) {
                return rslt;
            }

            /* �������ȼ� */
            rslt = VOS_RegisterTaskPrio(ACPU_FID_ADS_UL, ADS_TASK_PRIORITY);
            if (VOS_OK != rslt) {
                return VOS_ERR;
            }
            TAF_SortAdsUlFidChkMsgLenTbl();
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
    }

    return VOS_OK;
}

