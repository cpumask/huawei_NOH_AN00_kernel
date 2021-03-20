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

#include "mdrv.h"
#include "ps_tag.h"
#include "RnicCtx.h"
#include "RnicEntity.h"
#include "ps_common_def.h"
#include "RnicMsgProc.h"
#include "RnicIfaceCfg.h"
#include "RnicIfaceOndemand.h"
#include "RnicLog.h"
#include "RnicMntn.h"
#include "rnic_ondemand_i.h"
#include "rnic_dev_i.h"
#include "AcpuReset.h"
#include "AtRnicInterface.h"
#include "mn_comm_api.h"
#include "securec.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_RNIC_MAIN_C

/* RNIC CTX,���ڱ���RNIC��ȫ�ֱ��� */
RNIC_CTX g_rnicCtx;


VOS_INT RNIC_CCpuResetCallback(drv_reset_cb_moment_e param, VOS_INT iUserData)
{
    RNIC_CCPU_RESET_IND_STRU *msg = VOS_NULL_PTR;

    /* ��λǰ */
    if (MDRV_RESET_CB_BEFORE == param) {
        RNIC_PR_LOGI("before reset enter, %u", VOS_GetSlice());

        /* ������Ϣ */
        msg = (RNIC_CCPU_RESET_IND_STRU *)RNIC_ALLOC_MSG_WITH_HDR(sizeof(RNIC_CCPU_RESET_IND_STRU));
        if (VOS_NULL_PTR == msg) {
            RNIC_PR_LOGI("before reset alloc msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }

        /* ��д��Ϣͷ */
        RNIC_CFG_INTRA_MSG_HDR(msg, ID_RNIC_CCPU_RESET_START_IND);

        /* ����Ϣ */
        if (VOS_OK != RNIC_SendMsg(msg)) {
            RNIC_PR_LOGI("before reset send msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }

        /* �ȴ��ظ��ź�����ʼΪ��״̬���ȴ���Ϣ��������ź��������� */
        if (VOS_OK != VOS_SmP(RNIC_GET_RESET_SEM(), RNIC_RESET_TIMEOUT_LEN)) {
            RNIC_PR_LOGI("before reset Vos_SmP fail, %u", VOS_GetSlice());

            return VOS_ERROR;
        }

        return VOS_OK;
    }
    /* ��λ�� */
    else if (MDRV_RESET_CB_AFTER == param) {
        RNIC_PR_LOGI("After reset , %u", VOS_GetSlice());

        return VOS_OK;
    } else {
        return VOS_ERROR;
    }
}


VOS_VOID RNIC_InitCtx(VOS_VOID)
{
    RNIC_CTX            *rnicCtx = VOS_NULL_PTR;
    TAF_NV_RnicNapiCfg   napiCfg;
    TAF_NV_RnicNapiLbCfg napiLbCfg;
    VOS_UINT32           indexNum;

    (VOS_VOID)memset_s(&g_rnicCtx, sizeof(g_rnicCtx), 0x00, sizeof(g_rnicCtx));
    (VOS_VOID)memset_s(&napiCfg, sizeof(napiCfg), 0x00, sizeof(napiCfg));
    (VOS_VOID)memset_s(&napiLbCfg, sizeof(napiLbCfg), 0x00, sizeof(napiLbCfg));

    rnicCtx = RNIC_GET_RNIC_CTX_ADR();

#if(FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_OFF)
    g_rnicCtx.ipfMode = ADS_GetIpfMode();

    /* ����ǽ���������, �ر�NAPI��GRO */
    if (g_rnicCtx.ipfMode == ADS_RD_THRD) {
        napiCfg.napiEnable = RNIC_FEATURE_OFF;
        napiCfg.groEnable  = RNIC_FEATURE_OFF;
    }
#endif

    for (indexNum = 0; indexNum < RNIC_NET_ID_MAX_NUM; indexNum++) {
        rnicCtx->ifaceCtx[indexNum].rmNetId = (VOS_UINT8)indexNum;

        /* ��ʼ��RNIC PDN��Ϣ */
        RNIC_InitPdnInfo(&(rnicCtx->ifaceCtx[indexNum].psIfaceInfo));

    }

    /* ��ʼ��RNIC��ʱ�������� */
    RNIC_InitAllTimers();

#if (FEATURE_OFF == FEATURE_ACPU_PHONE_MODE)
    /* ��ʼ������ģʽ��Ϣ */
    RNIC_InitOnDemandDialInfo();
#endif

    /* ��ʼ����λ�ź��� */
    RNIC_InitResetSem();

    /* ��ʼ��Tethering��Ϣ */
    RNIC_InitTetherInfo();

    return;
}


VOS_VOID RNIC_InitPdnInfo(RNIC_PS_IfaceInfo *pdnInfo)
{
    RNIC_InitIpv4PdnInfo(pdnInfo);
    RNIC_InitIpv6PdnInfo(pdnInfo);
    RNIC_InitEthPdnInfo(pdnInfo);

    pdnInfo->modemId = MODEM_ID_BUTT;
    pdnInfo->ratType = RNIC_PS_RAT_TYPE_BUTT;

    IMM_ZcQueueHeadInit(&pdnInfo->imsQue);

    return;
}


VOS_VOID RNIC_InitNapiCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId, RNIC_NapiCfg *napiCfg)
{
#if (FEATURE_RNIC_NAPI_GRO == FEATURE_ON)
    /* 3GPP��7����������NV���ã����������ر�NAPI */
    if (rmNetId <= RNIC_3GPP_NET_ID_MAX_NUM) {
        RNIC_SET_NAPI_FEATURE(rmNetId, napiCfg->napiEnable);
        RNIC_SET_GRO_FEATURE(rmNetId, napiCfg->groEnable);
        RNIC_SET_NAPI_WEIGHT_ADJ_MODE(rmNetId, napiCfg->napiWeightAdjMode);
        RNIC_SET_NAPI_WEIGHT(rmNetId, napiCfg->napiPollWeight);
        RNIC_SET_NAPI_POLL_QUE_MAX_LEN(rmNetId, napiCfg->napiPollQueMaxLen);

        RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL1(rmNetId,
                                           napiCfg->napiWeightDynamicAdjCfg.dlPktNumPerSecLevel.dlPktNumPerSecLevel1);
        RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL2(rmNetId,
                                           napiCfg->napiWeightDynamicAdjCfg.dlPktNumPerSecLevel.dlPktNumPerSecLevel2);
        RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL3(rmNetId,
                                           napiCfg->napiWeightDynamicAdjCfg.dlPktNumPerSecLevel.dlPktNumPerSecLevel3);
        RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL4(rmNetId,
                                           napiCfg->napiWeightDynamicAdjCfg.dlPktNumPerSecLevel.dlPktNumPerSecLevel4);
        RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL5(rmNetId,
                                           napiCfg->napiWeightDynamicAdjCfg.dlPktNumPerSecLevel.dlPktNumPerSecLevel5);

        RNIC_SET_NAPI_WEIGHT_LEVEL1(rmNetId, napiCfg->napiWeightDynamicAdjCfg.napiWeightLevel.napiWeightLevel1);
        RNIC_SET_NAPI_WEIGHT_LEVEL2(rmNetId, napiCfg->napiWeightDynamicAdjCfg.napiWeightLevel.napiWeightLevel2);
        RNIC_SET_NAPI_WEIGHT_LEVEL3(rmNetId, napiCfg->napiWeightDynamicAdjCfg.napiWeightLevel.napiWeightLevel3);
        RNIC_SET_NAPI_WEIGHT_LEVEL4(rmNetId, napiCfg->napiWeightDynamicAdjCfg.napiWeightLevel.napiWeightLevel4);
        RNIC_SET_NAPI_WEIGHT_LEVEL5(rmNetId, napiCfg->napiWeightDynamicAdjCfg.napiWeightLevel.napiWeightLevel5);

    } else {
        RNIC_SET_NAPI_FEATURE(rmNetId, RNIC_FEATURE_OFF);
        RNIC_SET_GRO_FEATURE(rmNetId, RNIC_FEATURE_OFF);
    }
#else
    RNIC_SET_NAPI_FEATURE(rmNetId, RNIC_FEATURE_OFF);
    RNIC_SET_GRO_FEATURE(rmNetId, RNIC_FEATURE_OFF);
#endif
    return;
}


VOS_VOID RNIC_InitNapiLbCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId, RNIC_NapiLbCfg *napiLbCfg)
{
#if (FEATURE_RNIC_NAPI_GRO == FEATURE_ON)
    errno_t                  ret;
    /* 3GPP��7����������NV���ã����������ر�NAPI */
    if (rmNetId <= RNIC_3GPP_NET_ID_MAX_NUM) {
        RNIC_SET_NAPI_LB_FEATURE(rmNetId, napiLbCfg->napiLbEnable);
        RNIC_SET_NAPI_LB_CPUMASK(rmNetId, napiLbCfg->napiLbCpumask);
        /* Ĭ��Ϊ��λ0 */
        RNIC_SET_NAPI_LB_CUR_LEVEL(rmNetId, 0);
        ret = memcpy_s(g_rnicCtx.ifaceCtx[rmNetId].featureCfg.napiLbCfg.napiLbLevelCfg,
            sizeof(g_rnicCtx.ifaceCtx[rmNetId].featureCfg.napiLbCfg.napiLbLevelCfg),
            napiLbCfg->napiLbLevelCfg,sizeof(napiLbCfg->napiLbLevelCfg));
        if (ret != EOK) {
            PS_PRINTF_WARNING("RNIC_InitNapiLbCfg:memcpy_s is failed!\n");
        }
    }
    else {
        RNIC_SET_NAPI_LB_FEATURE(rmNetId, RNIC_FEATURE_OFF);
    }
#else
    RNIC_SET_NAPI_LB_FEATURE(rmNetId, RNIC_FEATURE_OFF);
#endif
    return;
}


VOS_VOID RNIC_InitOnDemandDialInfo(VOS_VOID)
{
    RNIC_SET_PROC_DIAL_MODE(RNIC_DIAL_MODE_MANUAL);
    RNIC_SET_PROC_IDLE_TIME(RNIC_DIAL_DEMA_IDLE_TIME);
    RNIC_SET_PROC_EVENT_REPORT(RNIC_FORBID_EVENT_REPORT);

    wakeup_source_init(&g_rnicCtx.onDemandDisconnWakeLock, "rnic_ondemand_dis_wake");

    return;
}


VOS_VOID RNIC_InitIpv4PdnInfo(RNIC_PS_IfaceInfo *pdnInfo)
{
    pdnInfo->opIpv4Act   = RNIC_BIT_OPT_FALSE;
    pdnInfo->ipv4RabId   = RNIC_RAB_ID_INVALID;
    pdnInfo->ipv4ExRabId = RNIC_RAB_ID_INVALID;
    pdnInfo->ipv4Addr    = 0;
}


VOS_VOID RNIC_InitIpv6PdnInfo(RNIC_PS_IfaceInfo *pdnInfo)
{
    pdnInfo->opIpv6Act   = RNIC_BIT_OPT_FALSE;
    pdnInfo->ipv6RabId   = RNIC_RAB_ID_INVALID;
    pdnInfo->ipv6ExRabId = RNIC_RAB_ID_INVALID;
    memset_s(pdnInfo->ipv6Addr, sizeof(pdnInfo->ipv6Addr), 0x00, sizeof(pdnInfo->ipv6Addr));
}


VOS_VOID RNIC_InitEthPdnInfo(RNIC_PS_IfaceInfo *pdnInfo)
{
    pdnInfo->opEthAct   = RNIC_BIT_OPT_FALSE;
    pdnInfo->ethRabId   = RNIC_RAB_ID_INVALID;
    pdnInfo->ethExRabId = RNIC_RAB_ID_INVALID;
}


VOS_VOID RNIC_InitResetSem(VOS_VOID)
{
    g_rnicCtx.resetSem = VOS_NULL_PTR;

    /* ����������ź��� */
    if (VOS_OK != VOS_SmBCreate("RNIC", 0, VOS_SEMA4_FIFO, &g_rnicCtx.resetSem)) {
        PS_PRINTF_WARNING("Create rnic acpu cnf sem failed!\n");

        return;
    }

    PS_PRINTF_INFO("Create rnic acpu cnf sem success!\n");

    return;
}


VOS_VOID RNIC_CheckNapiCfgValid(TAF_NV_RnicNapiCfg *napiCfg, TAF_NV_RnicNapiLbCfg *napiLbCfg)
{
    if (napiCfg->napiEnable >= RNIC_FEATURE_BUTT) {
        napiCfg->napiEnable = RNIC_FEATURE_OFF;
    }

    if (napiCfg->groEnable >= RNIC_FEATURE_BUTT) {
        napiCfg->groEnable = RNIC_FEATURE_OFF;
    }

    if ((napiCfg->napiPollWeight > RNIC_NAPI_POLL_MAX_WEIGHT) || (0 == napiCfg->napiPollWeight)) {
        napiCfg->napiPollWeight = RNIC_NAPI_POLL_DEFAULT_WEIGHT;
    }

    if ((napiCfg->napiPollQueMaxLen > RNIC_POLL_QUEUE_DEFAULT_MAX_LEN) || (0 == napiCfg->napiPollQueMaxLen)) {
        napiCfg->napiPollQueMaxLen = RNIC_POLL_QUEUE_DEFAULT_MAX_LEN;
    }

    if (napiCfg->napiWeightAdjMode >= NAPI_WEIGHT_ADJ_MODE_BUTT) {
        napiCfg->napiWeightAdjMode = NAPI_WEIGHT_ADJ_STATIC_MODE;
    }

    if (napiLbCfg->napiLbEnable >= RNIC_FEATURE_BUTT) {
        napiLbCfg->napiLbEnable = RNIC_FEATURE_OFF;
    }

    return;
}


VOS_VOID RNIC_InitTetherInfo(VOS_VOID)
{
    (VOS_VOID)memset_s(g_rnicCtx.tetherInfo.rmnetName, sizeof(g_rnicCtx.tetherInfo.rmnetName), 0x00,
                       sizeof(g_rnicCtx.tetherInfo.rmnetName));

    g_rnicCtx.tetherInfo.tetherConnStat = AT_RNIC_USB_TETHER_DISCONNECT;
    g_rnicCtx.tetherInfo.origGroEnable  = RNIC_FEATURE_OFF;

    return;
}



VOS_UINT32 RNIC_PidInit(enum VOS_InitPhaseDefine phase)
{
    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            /* ��ʼ��RNIC��������Ϣ */
            RNIC_InitCtx();

#if (FEATURE_OFF == FEATURE_ACPU_PHONE_MODE)
            /* �������貦�����proc�ڵ� */
            rnic_create_ondemand_procfs();

            /* ע�ᰴ�貦�Žڵ�����ص� */
            RNIC_IFACE_RegOnDemandOpsCB();
#endif
            /* ע���豸Ready notify�ص� */
            RNIC_IFACE_RegDevNotifyCB();

            /* ע��ص�����������C�˵�����λ�Ĵ��� */
            mdrv_sysboot_register_reset_notify(NAS_RNIC_FUNC_PROC_NAME, RNIC_CCpuResetCallback, 0,
                                               ACPU_RESET_PRIORITY_RNIC);
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

