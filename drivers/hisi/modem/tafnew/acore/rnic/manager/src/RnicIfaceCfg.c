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

#include "ps_common_def.h"
#include "RnicMsgProc.h"
#include "RnicEntity.h"
#include "RnicMntn.h"
#include "RnicIfaceOndemand.h"
#include "RnicLog.h"
#include "bastet_rnic_interface.h"
#include "rnic_ondemand_i.h"
#include "rnic_policy_manage.h"
#include "securec.h"
#include "mn_comm_api.h"

/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_RNIC_IFACE_CFG_C


VOS_VOID RNIC_IFACE_SetNapiCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId)
{
    struct rnic_napi_config_s napiCfg;

    napiCfg.devid             = rmNetId;
    napiCfg.napi_enable       = RNIC_GET_NAPI_FEATURE(rmNetId);
    napiCfg.gro_enable        = RNIC_GET_GRO_FEATURE(rmNetId);
    napiCfg.napi_weight       = RNIC_GET_NAPI_WEIGHT(rmNetId);
    napiCfg.napi_queue_length = RNIC_GET_NAPI_POLL_QUE_MAX_LEN(rmNetId);

    rnic_set_napi_config(&napiCfg);
    return;
}


VOS_VOID RNIC_IFACE_SetNapiLbCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId)
{
    RNIC_NapiLbCfg          *rnicNapiLbCfg = RNIC_GET_NAPI_LB_CFG_ADR(rmNetId);
    struct rnic_lb_config_s  napiLbCfg;

    (VOS_VOID)memset_s(&napiLbCfg, sizeof(napiLbCfg), 0x00, sizeof(napiLbCfg));

    napiLbCfg.devid              = rmNetId;
    napiLbCfg.lb_enable          = rnicNapiLbCfg->napiLbEnable;
    napiLbCfg.cpu_bitmask        = RNIC_GET_NAPI_LB_CPUMASK(rmNetId);

    if (memcpy_s(napiLbCfg.level_cfg, sizeof(napiLbCfg.level_cfg),
                 rnicNapiLbCfg->napiLbLevelCfg,
                 sizeof(rnicNapiLbCfg->napiLbLevelCfg)) != EOK) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetNapiLbCfg: memcpy fail!");
    }

    rnic_set_lb_config(&napiLbCfg);
    return;
}


VOS_VOID RNIC_IFACE_SetNapiLbLevel(RNIC_DEV_ID_ENUM_UINT8 rmNetId, VOS_UINT8 lbLevel)
{
    if (lbLevel != RNIC_GET_NAPI_LB_CUR_LEVEL(rmNetId)) {
        if (!rnic_set_lb_level(rmNetId, lbLevel)) {
            RNIC_SET_NAPI_LB_CUR_LEVEL(rmNetId, lbLevel);
        }
    }
    return;
}


VOS_VOID RNIC_IFACE_SetNapiWeight(RNIC_DEV_ID_ENUM_UINT8 rmNetId, VOS_UINT8 weight)
{
    if (weight != RNIC_GET_NAPI_WEIGHT(rmNetId)) {
        rnic_set_napi_weight(rmNetId, weight);
        RNIC_SET_NAPI_WEIGHT(rmNetId, weight);
    }

    return;
}


VOS_VOID RNIC_IFACE_SetRhcCfg(VOS_VOID)
{
    RNIC_CTX                 *rnicCtx = VOS_NULL_PTR;
    struct rnic_rhc_config_s  rhcCfg;

    rnicCtx = RNIC_GET_RNIC_CTX_ADR();
    (VOS_VOID)memset_s(&rhcCfg, sizeof(struct rnic_rhc_config_s), 0x00,
                       sizeof(struct rnic_rhc_config_s));

    rhcCfg.rhc_feature        = rnicCtx->rhcCfg.feature;
    rhcCfg.qcc_enable         = rnicCtx->rhcCfg.qccEnable;
    rhcCfg.napi_wt_exp_enable = rnicCtx->rhcCfg.napiWtExpEnable;
    rhcCfg.ddr_req_enable     = rnicCtx->rhcCfg.ddrReqEnable;
    rhcCfg.qcc_timeout        = rnicCtx->rhcCfg.qccTimeout;
    rhcCfg.napi_poll_max      = rnicCtx->rhcCfg.napiPollMax;
    rhcCfg.ddr_mid_bd         = rnicCtx->rhcCfg.ddrMidBd;
    rhcCfg.ddr_high_bd        = rnicCtx->rhcCfg.ddrHighBd;

    if (memcpy_s(rhcCfg.level_cfg, sizeof(rhcCfg.level_cfg),
                 rnicCtx->rhcCfg.levelCfg, sizeof(rnicCtx->rhcCfg.levelCfg)) != EOK) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetRhcCfg: memcpy_s fail!");
        return;
    }

    rnic_set_rhc_config(&rhcCfg);
}


VOS_VOID RNIC_IFACE_ResetNapiConfig(RNIC_DEV_ID_ENUM_UINT8 rmNetId)
{
    VOS_UINT8 minWeight = RNIC_GET_NAPI_WEIGHT_LEVEL1(rmNetId);

    if ((RNIC_GET_NAPI_FEATURE(rmNetId) == RNIC_FEATURE_ON) &&
        (RNIC_GET_NAPI_WEIGHT_ADJ_MODE(rmNetId) == NAPI_WEIGHT_ADJ_DYNAMIC_MODE)) {
        if (0 != rnic_set_napi_weight(rmNetId, minWeight)) {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_ResetNapiConfig: set rnic_set_napi_weight fail!");
        }
        RNIC_SET_NAPI_WEIGHT(rmNetId, minWeight);
    }

    if (RNIC_FEATURE_ON == RNIC_GET_NAPI_LB_FEATURE(rmNetId)) {
        if (0 != rnic_set_lb_level(rmNetId, 0)) {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_ResetNapiConfig: set rnic_set_lb_level fail!");
        }
        RNIC_SET_NAPI_LB_CUR_LEVEL(rmNetId, 0);
    }

    return;
}


VOS_VOID RNIC_IFACE_ResetRhcLevel(VOS_VOID)
{
    if (g_rnicCtx.rhcCfg.feature == RNIC_FEATURE_ON) {
        rnic_set_rhc_level(0);
        g_rnicCtx.rhcCfg.rhcLevel = 0;
    }
}


VOS_UINT8 RNIC_IFACE_CalcNapiWeight(VOS_UINT8 rmNetId)
{
    VOS_UINT8  weight;
    VOS_UINT32 dlNapiRecvPktNum;

    weight           = RNIC_NAPI_POLL_DEFAULT_WEIGHT;
    dlNapiRecvPktNum = RNIC_GET_IFACE_PERIOD_RECV_PKT(rmNetId);

    /*
     * ��Ϊ5��:2/4/8/32/64
     * 150M bpsÿ���հ�����12300���ң���[0,12300]�����Χ�ڣ�weightֵ����Ϊ2.
     * 300M bpsÿ���հ�����24600���ң���[12300,24600]�����Χ�ڣ�weightֵ����Ϊ4.
     * 450M bpsÿ���հ�����38400���ң���[24600,38400]�����Χ�ڣ�weightֵ����Ϊ8.
     * 600M bpsÿ���հ�����49200���ң���[38400,49200]�����Χ�ڣ�weightֵ����Ϊ32.
     * ����600M��weightֵͳһ����Ϊ64.
     */

    if (dlNapiRecvPktNum <= RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL1(rmNetId)) {
        weight = RNIC_GET_NAPI_WEIGHT_LEVEL1(rmNetId);
    } else if ((dlNapiRecvPktNum > RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL1(rmNetId)) &&
               (dlNapiRecvPktNum <= RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL2(rmNetId))) {
        weight = RNIC_GET_NAPI_WEIGHT_LEVEL2(rmNetId);
    } else if ((dlNapiRecvPktNum > RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL2(rmNetId)) &&
               (dlNapiRecvPktNum <= RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL3(rmNetId))) {
        weight = RNIC_GET_NAPI_WEIGHT_LEVEL3(rmNetId);
    } else if ((dlNapiRecvPktNum > RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL3(rmNetId)) &&
               (dlNapiRecvPktNum <= RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL4(rmNetId))) {
        weight = RNIC_GET_NAPI_WEIGHT_LEVEL4(rmNetId);
    } else {
        weight = RNIC_GET_NAPI_WEIGHT_LEVEL5(rmNetId);
    }

    return weight;
}

VOS_VOID RNIC_IFACE_AdjustNapiWeight(VOS_UINT8 rmNetId)
{
    VOS_UINT8 weight;

    /* NAPI Weight��̬����ģʽ����ȥ����weightֵ���������������Ĭ�ϲ��þ�̬default weightֵ�� */
    if (NAPI_WEIGHT_ADJ_DYNAMIC_MODE == RNIC_GET_NAPI_WEIGHT_ADJ_MODE(rmNetId)) {
        weight = RNIC_IFACE_CalcNapiWeight(rmNetId);
        RNIC_IFACE_SetNapiWeight(rmNetId, weight);
    }

    return;
}


VOS_VOID RNIC_IFACE_AdjNapiLbLevel(VOS_UINT8 rmNetId)
{
    VOS_UINT32 lbLevel;
    VOS_UINT32 dlNapiRecvPktNum;

    /* NAPI���ؾ������Դ�ʱ���Ÿ�����������CPU��Ȩ�� */
    if (RNIC_FEATURE_ON == RNIC_GET_NAPI_LB_FEATURE(rmNetId)) {
        dlNapiRecvPktNum = RNIC_GET_IFACE_PERIOD_RECV_PKT(rmNetId);

        for (lbLevel = 0; lbLevel < (RNIC_NVIM_NAPI_LB_MAX_LEVEL - 1); lbLevel++) {
            if ((dlNapiRecvPktNum >= RNIC_GET_NAPI_LB_LEVEL_PPS(rmNetId, lbLevel)) &&
                (dlNapiRecvPktNum < RNIC_GET_NAPI_LB_LEVEL_PPS(rmNetId, lbLevel + 1UL)))
                break;
        }

        RNIC_IFACE_SetNapiLbLevel(rmNetId, (VOS_UINT8)lbLevel);
    }

    return;
}


VOS_UINT8 RNIC_IFACE_GetLevelByPps(VOS_UINT32 totalPps, VOS_UINT32 nonTcpPps)
{
    VOS_UINT32 lbLevel;

    for (lbLevel = 0; lbLevel < RNIC_NVIM_RHC_MAX_LEVEL; lbLevel++) {
        if ((totalPps >= g_rnicCtx.rhcCfg.levelCfg[lbLevel].totalPps) &&
            (nonTcpPps >= g_rnicCtx.rhcCfg.levelCfg[lbLevel].nonTcpPps)) {
            continue;
        }
        break;
    }
    lbLevel = (lbLevel == 0) ? 0 : (lbLevel - 1);

    return (VOS_UINT8)lbLevel;
}


VOS_VOID RNIC_IFACE_AdjRhcLevel(RNIC_DSFLOW_Stats *pdsFlowTotalStats)
{
    VOS_UINT8 rhcLevel = 0;

    if (g_rnicCtx.rhcCfg.feature == RNIC_FEATURE_OFF) {
        return;
    }

    rhcLevel = RNIC_IFACE_GetLevelByPps(pdsFlowTotalStats->periodRecvPktNum, pdsFlowTotalStats->periodRecvNonTcpPktNum);

    if ((rhcLevel != g_rnicCtx.rhcCfg.rhcLevel) || (g_rnicCtx.rhcCfg.levelCfg[rhcLevel].isolationDisable)) {
        rnic_set_rhc_level(rhcLevel);
        g_rnicCtx.rhcCfg.rhcLevel = rhcLevel;
    }

    return;
}

#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)

VOS_VOID RNIC_IFACE_NapiSchedule(VOS_ULONG usrData)
{
    rnic_rx_complete((VOS_UINT8)usrData);
    return;
}


VOS_INT RNIC_IFACE_RegRxHandler(RNIC_IFACE_Ctx *ifaceCtx)
{
    struct ads_iface_rx_handler_s rxHandler;

    (VOS_VOID)memset_s(&rxHandler, sizeof(rxHandler), 0x00, sizeof(rxHandler));

    rxHandler.user_data      = ifaceCtx->rmNetId;
    rxHandler.rx_dma_support = true;
    rxHandler.rx_func        = RNIC_DataRxProc;
    rxHandler.rx_cmplt_func  = RNIC_IFACE_NapiSchedule;

    return ads_iface_register_rx_handler(ifaceCtx->ifaceId, &rxHandler);
}


STATIC rnic_ps_iface_data_tx_func RNIC_IFACE_GetDataTxFunc(RNIC_PS_RatTypeUint8 ratType, VOS_UINT8 addrType)
{
    rnic_ps_iface_data_tx_func data_tx_func = NULL;

    if (addrType == RNIC_ETH_ADDR) {
        data_tx_func = RNIC_EthDataTxProc;
    } else {
        data_tx_func = (addrType == RNIC_IPV4_ADDR) ? RNIC_V4DataTxProc : RNIC_V6DataTxProc;
#if ((FEATURE_IMS == FEATURE_ON) && (FEATURE_ACPU_PHONE_MODE == FEATURE_ON))
        if (ratType == RNIC_PS_RAT_TYPE_IWLAN) {
            data_tx_func = RNIC_VoWifiDataTxProc;
        }
#endif
    }

    return data_tx_func;
}


VOS_VOID RNIC_IFACE_SetNetDevUp(RNIC_IFACE_Ctx *ifaceCtx, VOS_UINT8 addrType)
{
    struct rnic_ps_iface_config_s psIfaceCfg;

    (VOS_VOID)memset_s(&psIfaceCfg, sizeof(psIfaceCfg), 0x00, sizeof(psIfaceCfg));

    psIfaceCfg.devid       = ifaceCtx->rmNetId;
    psIfaceCfg.addr_family = addrType;

    psIfaceCfg.info.iface_id = ifaceCtx->ifaceId;
    psIfaceCfg.info.pdu_session_id = (addrType == RNIC_IPV4_ADDR) ? ifaceCtx->psIfaceInfo.ipv4ExRabId :
                                                                    ifaceCtx->psIfaceInfo.ipv6ExRabId;
    psIfaceCfg.info.fc_head  = RNIC_GET_FC_HEAD_BY_MODEMID(ifaceCtx->psIfaceInfo.modemId);
    psIfaceCfg.info.modem_id = (VOS_UINT8)ifaceCtx->psIfaceInfo.modemId;

    if (addrType == RNIC_ETH_ADDR) {
        psIfaceCfg.info.pdu_session_id = ifaceCtx->psIfaceInfo.ethExRabId;
        psIfaceCfg.info.fc_head        = EIPF_ETH_ULFC;
    }

    psIfaceCfg.data_tx_func = RNIC_IFACE_GetDataTxFunc(ifaceCtx->psIfaceInfo.ratType, addrType);
    rnic_set_ps_iface_up(&psIfaceCfg);

    return;
}


VOS_VOID RNIC_IFACE_SetNetDevDown(RNIC_IFACE_Ctx *ifaceCtx, VOS_UINT8 addrType)
{
    struct rnic_ps_iface_config_s psIfaceCfg;

    memset_s(&psIfaceCfg, sizeof(psIfaceCfg), 0x00, sizeof(psIfaceCfg));

    psIfaceCfg.devid               = ifaceCtx->rmNetId;
    psIfaceCfg.addr_family         = addrType;
    psIfaceCfg.info.iface_id       = RNIC_INVALID_IFACE_ID;
    psIfaceCfg.info.pdu_session_id = RNIC_INVALID_PDU_SESSION_ID;
    psIfaceCfg.info.fc_head        = RNIC_INVALID_FC_HEAD;
    psIfaceCfg.info.modem_id       = MODEM_ID_BUTT;
    psIfaceCfg.data_tx_func        = VOS_NULL_PTR;

    rnic_set_ps_iface_down(&psIfaceCfg);

    RNIC_IFACE_ResetNapiConfig(ifaceCtx->rmNetId);

    return;
}
#else

VOS_VOID RNIC_IFACE_NapiSchedule(VOS_UINT32 usrData)
{
    rnic_rx_complete((VOS_UINT8)usrData);
    return;
}


VOS_INT RNIC_IFACE_RegRxHandler(RNIC_IFACE_Ctx *ifaceCtx, VOS_UINT8 ipFamilyMask)
{
    VOS_UINT8 exRabId = RNIC_RAB_ID_INVALID;

    if (RNIC_BIT8_IS_SET(ipFamilyMask, RNIC_IPV4_ADDR)) {
        exRabId = ifaceCtx->psIfaceInfo.ipv4ExRabId;
    }

    if (RNIC_BIT8_IS_SET(ipFamilyMask, RNIC_IPV6_ADDR)) {
        exRabId = ifaceCtx->psIfaceInfo.ipv6ExRabId;
    }

    if (RNIC_PS_RAT_TYPE_3GPP == ifaceCtx->psIfaceInfo.ratType) {
        /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
        (VOS_VOID)ADS_DL_RegDlDataCallback(exRabId, RNIC_DataRxProc, ifaceCtx->rmNetId);

#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
        /* NAPI���Դ򿪲�ִ�� */
        if (RNIC_FEATURE_ON == RNIC_GET_NAPI_FEATURE(ifaceCtx->rmNetId)) {
            /* ע������NAPI����Ļص����� */
            (VOS_VOID)ADS_DL_RegNapiCallback(exRabId, RNIC_IFACE_NapiSchedule, ifaceCtx->rmNetId);
        }
#endif
    }

    return VOS_OK;
}


VOS_VOID RNIC_IFACE_SetNetDevUp(RNIC_IFACE_Ctx *ifaceCtx, VOS_UINT8 addrType)
{
    struct rnic_ps_iface_config_s psIfaceCfg;

    memset_s(&psIfaceCfg, sizeof(psIfaceCfg), 0x00, sizeof(psIfaceCfg));

    psIfaceCfg.devid       = ifaceCtx->rmNetId;
    psIfaceCfg.addr_family = addrType;

    /* �������������������հ��ص� */
#if ((FEATURE_ON == FEATURE_IMS) && (FEATURE_ON == FEATURE_ACPU_PHONE_MODE))
    if (RNIC_PS_RAT_TYPE_IWLAN == ifaceCtx->psIfaceInfo.ratType) {
        psIfaceCfg.data_tx_func = RNIC_VoWifiDataTxProc;
    } else
#endif
    {
        psIfaceCfg.data_tx_func = (addrType == RNIC_IPV4_ADDR) ? RNIC_V4DataTxProc : RNIC_V6DataTxProc;
    }

    if (0 != rnic_set_ps_iface_up(&psIfaceCfg)) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetDevUp: set rnic_set_ps_iface_up fail!");
    }

    return;
}


VOS_VOID RNIC_IFACE_SetNetDevDown(RNIC_IFACE_Ctx *ifaceCtx, VOS_UINT8 addrType)
{
    struct rnic_ps_iface_config_s psIfaceCfg;

    memset_s(&psIfaceCfg, sizeof(psIfaceCfg), 0x00, sizeof(psIfaceCfg));

    psIfaceCfg.devid       = ifaceCtx->rmNetId;
    psIfaceCfg.addr_family = addrType;
    psIfaceCfg.data_tx_func = VOS_NULL_PTR;

    if (0 != rnic_set_ps_iface_down(&psIfaceCfg)) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetDevDown: set rnic_set_ps_iface_down fail!");
    }

    RNIC_IFACE_ResetNapiConfig(ifaceCtx->rmNetId);

    return;
}
#endif /* FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM */


#if (defined(CONFIG_BALONG_ESPE))

VOS_VOID RNIC_IFACE_SetEspePortConfig(VOS_VOID)
{
    struct rnic_espe_port_config_s portConfig;

    portConfig.info.iface_id       = RNIC_INVALID_IFACE_ID;
    portConfig.info.pdu_session_id = RNIC_INVALID_PDU_SESSION_ID;
    portConfig.info.fc_head        = RNIC_INVALID_FC_HEAD;
    portConfig.info.modem_id       = MODEM_ID_BUTT;

    if (VOS_OK != rnic_set_espe_port_config(&portConfig)) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetEspePortConfig: Config ESPE failed!");
    }

    return;
}
#endif


VOS_VOID RNIC_IFACE_SetFeatureCfg(VOS_VOID)
{
    VOS_UINT32 indexNum;

    for (indexNum = 0; indexNum <= RNIC_3GPP_NET_ID_MAX_NUM; indexNum++) {
        /* ����NAPI���� */
        RNIC_IFACE_SetNapiCfg((VOS_UINT8)indexNum);
        RNIC_IFACE_SetNapiLbCfg((VOS_UINT8)indexNum);

#if (FEATURE_OFF == FEATURE_ACPU_PHONE_MODE)
        /* ע���豸���ж����ص� */
        RNIC_IFACE_RegTxDropCB((VOS_UINT8)indexNum);
#endif
    }

    RNIC_IFACE_SetRhcCfg();
    return;
}

VOS_VOID RNIC_IFACE_SetDsFlowStats(VOS_UINT8 rmNetId, RNIC_DSFLOW_Stats *dsFlowTotalStats)
{
    struct rnic_dsflow_stats_s *dsflowStats = VOS_NULL_PTR;

    /* ���������ӿڻ�ȡ������Ϣ */
    dsflowStats = rnic_get_dsflow_stats(rmNetId);

    if (VOS_NULL_PTR == dsflowStats) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_SetDsFlowStats fail, get dsflow stats failed !");
        return;
    }

    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.currentRecvRate  = dsflowStats->rx_bytes;
    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.currentSendRate  = dsflowStats->tx_bytes;
    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.periodRecvPktNum = dsflowStats->rx_packets;
    /* �ۼ����з��ͱ��ģ�����Ͽ���ʱ����ʱ���� */
    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.periodSendPktNum += dsflowStats->tx_packets;

    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.periodRecvTcpPktNum    = dsflowStats->rx_tcp_pkts;
    g_rnicCtx.ifaceCtx[rmNetId].dsFlowStats.periodRecvNonTcpPktNum = dsflowStats->rx_non_tcp_pkts;

    dsFlowTotalStats->currentRecvRate += dsflowStats->rx_bytes;
    dsFlowTotalStats->currentSendRate += dsflowStats->tx_bytes;

    dsFlowTotalStats->periodRecvPktNum += dsflowStats->rx_packets;
    dsFlowTotalStats->periodSendPktNum += dsflowStats->tx_packets;
    dsFlowTotalStats->periodRecvTcpPktNum += dsflowStats->rx_tcp_pkts;
    dsFlowTotalStats->periodRecvNonTcpPktNum += dsflowStats->rx_non_tcp_pkts;

    RNIC_NORMAL_LOG4(ACPU_PID_RNIC, "RNIC_IFACE_SetDsFlowStats: <rx_bytes, tx_bytes, rx_packets, tx_packets> ",
                     dsflowStats->rx_bytes, dsflowStats->tx_bytes, dsflowStats->rx_packets, dsflowStats->tx_packets);

    return;
}


VOS_VOID RNIC_IFACE_ClearDsFlowFlux(VOS_UINT8 rmNetId)
{
    rnic_clear_dsflow_stats(rmNetId);
    return;
}


VOS_VOID RNIC_IFACE_ClearDsFlowStats(VOS_UINT8 rmNetId)
{
    RNIC_IFACE_Ctx *ifaceCtx = VOS_NULL_PTR;

    ifaceCtx = RNIC_GET_IFACE_CTX_ADR(rmNetId);

    ifaceCtx->dsFlowStats.currentRecvRate  = 0;
    ifaceCtx->dsFlowStats.periodRecvPktNum = 0;

    ifaceCtx->dsFlowStats.currentSendRate        = 0;
    ifaceCtx->dsFlowStats.periodSendPktNum       = 0;
    ifaceCtx->dsFlowStats.periodRecvTcpPktNum    = 0;
    ifaceCtx->dsFlowStats.periodRecvNonTcpPktNum = 0;

    return;
}


VOS_VOID RNIC_IFACE_NetdevNotify(VOS_VOID)
{
    RNIC_DEV_READY_STRU *sndMsg;

    /* �ڴ���� */
    sndMsg = (RNIC_DEV_READY_STRU *)RNIC_ALLOC_MSG_WITH_HDR(sizeof(RNIC_DEV_READY_STRU));

    if (VOS_NULL_PTR == sndMsg) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendNetdevReadyInd: Malloc failed!");
        return;
    }

    /* ��д��Ϣ */
    RNIC_CFG_INTRA_MSG_HDR(sndMsg, ID_RNIC_NETDEV_READY_IND);

    /* ������Ϣ */
    if (RNIC_SendMsg(sndMsg) != VOS_OK) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendNetdevReadyInd: Send msg failed!");
    }

    return;
}


VOS_VOID RNIC_IFACE_RegDevNotifyCB(VOS_VOID)
{
    struct rnic_deivce_notifier_s devNotifier;

    devNotifier.dev_notifier_func = RNIC_IFACE_NetdevNotify;

    rnic_register_device_notifier(&devNotifier);
    return;
}


VOS_VOID RNIC_IFACE_RegOnDemandOpsCB(VOS_VOID)
{
    struct rnic_ondemand_cb_ops_s ondemandOpsCB;

    ondemandOpsCB.dial_mode_cfg_func    = RNIC_IFACE_OndemandDialModeProc;
    ondemandOpsCB.idle_time_cfg_func    = RNIC_IFACE_OndemandIdleTimeProc;
    ondemandOpsCB.event_report_cfg_func = RNIC_IFACE_OndemandEventReportProc;

    rnic_register_ondemand_ops(&ondemandOpsCB);
    return;
}


VOS_VOID RNIC_IFACE_RegTxDropCB(VOS_UINT8 rmNetId)
{
    struct rnic_ps_iface_drop_notifier_s dropNotify;

    /* ����0�������б��Ĵ������貦�� */
    if (rmNetId == RNIC_DEV_ID_RMNET0) {
        dropNotify.devid              = rmNetId;
        dropNotify.drop_notifier_func = RNIC_TxDropProc;

        rnic_register_ps_iface_drop_notifier(&dropNotify);
    }

    return;
}


VOS_VOID RNIC_IFACE_ResetPsIface(VOS_UINT8 rmNetId)
{
    struct rnic_ps_iface_config_s psIfaceCfg;
    VOS_UINT32 addrType;

    (VOS_VOID)memset_s(&psIfaceCfg, sizeof(psIfaceCfg), 0x00, sizeof(psIfaceCfg));

    psIfaceCfg.devid        = rmNetId;
    psIfaceCfg.data_tx_func = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    psIfaceCfg.info.iface_id       = RNIC_INVALID_IFACE_ID;
    psIfaceCfg.info.pdu_session_id = RNIC_INVALID_PDU_SESSION_ID;
    psIfaceCfg.info.fc_head        = RNIC_INVALID_FC_HEAD;
    psIfaceCfg.info.modem_id       = MODEM_ID_BUTT;
#endif

    for (addrType = RNIC_IPV4_ADDR; addrType < RNIC_ADDR_BUTT; addrType++) {
        psIfaceCfg.addr_family = (VOS_UINT8)addrType;
        rnic_set_ps_iface_down(&psIfaceCfg);
    }

    RNIC_IFACE_ResetNapiConfig(rmNetId);
    return;
}


VOS_VOID RNIC_IFACE_PsIfaceUp(VOS_UINT8 rmNetId, VOS_UINT8 addrFamilyMask)
{
    RNIC_IFACE_Ctx    *ifaceCtx    = VOS_NULL_PTR;
    RNIC_PS_IfaceInfo *psIfaceInfo = VOS_NULL_PTR;
    VOS_UINT32         addrType;

    ifaceCtx    = RNIC_GET_IFACE_CTX_ADR(rmNetId);
    psIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(rmNetId);

    RNIC_NORMAL_LOG3(ACPU_PID_RNIC, "RNIC_IFACE_PsIfaceUp: <ucRmNetId, ucIpFamilyMask, ucRatType>", rmNetId,
                     addrFamilyMask, psIfaceInfo->ratType);

    for (addrType = RNIC_IPV4_ADDR; addrType < RNIC_ADDR_BUTT; addrType++) {
        if (RNIC_BIT8_IS_SET(addrFamilyMask, addrType)) {
            RNIC_IFACE_SetNetDevUp(ifaceCtx, (VOS_UINT8)addrType);
        }
    }

    /* RMNET0ΪIPV4����ʱ�Ż��а��貦��, ֹͣ���貦�ű�����ʱ�� */
    if ((rmNetId == RNIC_DEV_ID_RMNET0) && RNIC_BIT8_IS_SET(addrFamilyMask, RNIC_IPV4_ADDR)) {
        RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_PROTECT);
    }

#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    RNIC_IFACE_RegRxHandler(ifaceCtx);
#else
    RNIC_IFACE_RegRxHandler(ifaceCtx, addrFamilyMask);
#endif

    if (rmNetId <= RNIC_3GPP_NET_ID_MAX_NUM) {
        RNIC_StartTimer(TI_RNIC_DSFLOW_STATS, TI_RNIC_DSFLOW_STATS_LEN);
    }

    RNIC_BIT32_SET(g_rnicCtx.dsflowTimerMask, rmNetId);

    return;
}


VOS_VOID RNIC_IFACE_PsIfaceDown(VOS_UINT8 rmNetId, VOS_UINT8 addrFamilyMask)
{
    RNIC_IFACE_Ctx *ifaceCtx = VOS_NULL_PTR;
    VOS_UINT32      addrType;

    ifaceCtx = RNIC_GET_IFACE_CTX_ADR(rmNetId);

    RNIC_NORMAL_LOG2(ACPU_PID_RNIC, "RNIC_IFACE_PsIfaceDown: <rmNetId, addrFamilyMask>", rmNetId, addrFamilyMask);

    for (addrType = RNIC_IPV4_ADDR; addrType < RNIC_ADDR_BUTT; addrType++) {
        if (RNIC_BIT8_IS_SET(addrFamilyMask, addrType)) {
            RNIC_IFACE_SetNetDevDown(ifaceCtx, (VOS_UINT8)addrType);
        }
    }

    /* RMNET0ΪIPV4����ʱ�Ż��а��貦��, ����down��ʱ��ͣ����Ͽ���ʱ�� */
    if ((rmNetId == RNIC_DEV_ID_RMNET0) && RNIC_BIT8_IS_SET(addrFamilyMask, RNIC_IPV4_ADDR)) {
        RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT);
        RNIC_CLEAR_TI_DIALDOWN_EXP_CONT();
    }

    if ((RNIC_BIT_OPT_FALSE == RNIC_GET_IFACE_PDN_IPV4_ACT_STATE(rmNetId)) &&
        (RNIC_BIT_OPT_FALSE == RNIC_GET_IFACE_PDN_IPV6_ACT_STATE(rmNetId))) {
        RNIC_BIT32_CLR(g_rnicCtx.dsflowTimerMask, rmNetId);

        /* �������������down�ˣ���ֹͣ����ͳ�ƶ�ʱ�����������ͳ�� */
        if (!(g_rnicCtx.dsflowTimerMask & RNIC_3GPP_NET_MAX_NUM_MASK)) {
            RNIC_StopTimer(TI_RNIC_DSFLOW_STATS);
            RNIC_IFACE_ClearDsFlowStats(rmNetId);
            RNIC_IFACE_ResetRhcLevel();
        }
    }

    return;
}


VOS_VOID RNIC_IFACE_TetherInfo(RNIC_PS_IfaceTetherInfo *tetherInfo)
{
#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
    RNIC_DEV_ID_ENUM_UINT8 rmNetId;

    if (VOS_NULL_PTR == tetherInfo) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_TetherInfo fail, pstTetherInfo is NULL!");
        return;
    }

    if (VOS_OK != rnic_get_devid_by_name((char *)tetherInfo->rmnetName, &rmNetId)) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_TetherInfo fail, no such RmnetName!");
        return;
    }

    if (rmNetId >= RNIC_DEV_ID_BUTT) {
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_IFACE_TetherInfo fail, invalid enRmNetId ", rmNetId);
        return;
    }

    /* ����ָ��������GRO���� */
    if (AT_RNIC_USB_TETHER_CONNECTED == tetherInfo->tetherConnStat) {
        /* �ȱ���ָ��������GRO���� */
        RNIC_SET_TETHER_ORIG_GRO_FEATURE(RNIC_GET_GRO_FEATURE(rmNetId));

        RNIC_SET_GRO_FEATURE(rmNetId, RNIC_FEATURE_OFF);
    } else if (AT_RNIC_USB_TETHER_DISCONNECT == tetherInfo->tetherConnStat) {
        /* �ָ�ָ��������GRO���� */
        RNIC_SET_GRO_FEATURE(rmNetId, RNIC_GET_TETHER_ORIG_GRO_FEATURE());

        /* ��ʼ��Tethering��Ϣ������Ӱ���´ε����� */
        RNIC_InitTetherInfo();
    } else {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_TetherInfo fail, enTetherConnStat invalid!");
        return;
    }

    RNIC_IFACE_SetNapiCfg(rmNetId);
#else
    (VOS_VOID)tetherInfo;
#endif

    return;
}


VOS_INT RNIC_IFACE_RmnetCfg(RNIC_PS_IfaceRmnetCfg *rmnetCfg)
{
    VOS_UINT32 indexNum;
    RNIC_DEV_ID_ENUM_UINT8 rmnetId;
    NAPI_WEIGHT_AdjModeUint8 mode;
    VOS_UINT8 weight;

    if (VOS_NULL_PTR == rmnetCfg) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_RmnetCfg fail, rmnetCfg is NULL!");
        return VOS_ERROR;
    }

    /* ���õ�weightΪ0����Ϊ��̬����ģʽ������Ĭ��weightֵ */
    if (rmnetCfg->weight == 0) {
        mode = NAPI_WEIGHT_ADJ_DYNAMIC_MODE;
        weight = RNIC_NAPI_POLL_DEFAULT_WEIGHT;
    } else {
        mode = NAPI_WEIGHT_ADJ_STATIC_MODE;
        weight = rmnetCfg->weight;
    }

    if (rmnetCfg->featureMode == AT_RNIC_ALL_RMNET) {
        for (indexNum = 0; indexNum <= RNIC_3GPP_NET_ID_MAX_NUM; indexNum++) {
            RNIC_SET_NAPI_WEIGHT_ADJ_MODE(indexNum, mode);
            RNIC_SET_NAPI_WEIGHT(indexNum, weight);
            RNIC_IFACE_SetNapiCfg((RNIC_DEV_ID_ENUM_UINT8)indexNum);
        }
    } else if (rmnetCfg->featureMode == AT_RNIC_SINGLE_RMNET) {
        if (VOS_OK != rnic_get_devid_by_name((char *)rmnetCfg->rmnetName, &rmnetId)) {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_RmnetCfg fail, no such RmnetName!");
            return VOS_ERROR;
        }

        if (rmnetId > RNIC_3GPP_NET_ID_MAX_NUM) {
            RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_IFACE_RmnetCfg fail, invalid enRmNetId ", rmnetId);
            return VOS_ERROR;
        }

        RNIC_SET_NAPI_WEIGHT_ADJ_MODE(rmnetId, mode);
        RNIC_SET_NAPI_WEIGHT(rmnetId, weight);
        RNIC_IFACE_SetNapiCfg(rmnetId);
    } else {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_IFACE_RmnetCfg fail, featureMode invalid!");
        return VOS_ERROR;
    }

    return VOS_OK;
}



VOS_INT RNIC_BST_GetModemInfo(const char *netDevName, BST_RNIC_ModemInfo *modemInfo)
{
    RNIC_IFACE_Ctx        *ifaceCtx = VOS_NULL_PTR;
    RNIC_DEV_ID_ENUM_UINT8 rmNetId;

    if (VOS_NULL_PTR == netDevName) {
        return VOS_ERROR;
    }

    if (VOS_NULL_PTR == modemInfo) {
        return VOS_ERROR;
    }

    if (VOS_OK != rnic_get_devid_by_name(netDevName, &rmNetId)) {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_BST_GetModemInfo fail, no such RmnetName!");
        return VOS_ERROR;
    }

    if (rmNetId >= RNIC_DEV_ID_BUTT) {
        return VOS_NULL_PTR;
    }

    ifaceCtx = RNIC_GET_IFACE_CTX_ADR(rmNetId);

    modemInfo->ipv4State = (RNIC_BIT_OPT_FALSE == ifaceCtx->psIfaceInfo.opIpv4Act) ? BST_RNIC_PDP_STATE_INACTIVE :
                                                                                     BST_RNIC_PDP_STATE_ACTIVE;
    modemInfo->ipv6State = (RNIC_BIT_OPT_FALSE == ifaceCtx->psIfaceInfo.opIpv6Act) ? BST_RNIC_PDP_STATE_INACTIVE :
                                                                                     BST_RNIC_PDP_STATE_ACTIVE;

    modemInfo->modemId = ifaceCtx->psIfaceInfo.modemId;
    modemInfo->ipv4RabId = ifaceCtx->psIfaceInfo.ipv4RabId;
    modemInfo->ipv6RabId = ifaceCtx->psIfaceInfo.ipv6RabId;

    return VOS_OK;
}

