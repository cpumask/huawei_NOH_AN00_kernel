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
#include "taf_type_def.h"
#include "cds_ads_interface.h"
#include "AdsCtx.h"
#include "AdsLog.h"
#include "AdsIfaceMgmt.h"
#include "ads_iface_i.h"
#include "AdsMsgProc.h"
#include "AdsMntn.h"
#include "ips_mntn.h"
#include "securec.h"
#if (FEATURE_PC5_DATA_CHANNEL == FEATURE_ON)
#include "ltev_iface_i.h"
#endif


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_ADS_IFACE_MGMT_C


STATIC VOS_VOID ADS_IfaceUp(ADS_IFACE_Config *ifaceCfg, VOS_UINT8 addrType)
{
    ADS_IFACE_Info           *ifaceInfo = VOS_NULL_PTR;
    ADS_CONTEXT              *adsCtx    = VOS_NULL_PTR;
    struct ads_iface_config_s ifacePara;

    adsCtx    = ADS_GetCtx();
    ifaceInfo = ADS_GetIfaceInfo(ifaceCfg->ifaceId);

    /* TRANS MODE ��ͻ�ж� */
    if ((ifaceInfo->transMode != ADS_TRANS_MODE_BUTT) && (ifaceInfo->transMode != ifaceCfg->transMode)) {
        ADS_ERROR_LOG("ADS_IfaceUp: Trans Mode Conflicted!");
        return;
    }

    /* ���������ӿ������� */
    ifaceInfo->ifaceId   = ifaceCfg->ifaceId;
    ifaceInfo->stats     = ADS_IFACE_OP_TYPE_UP;
    ifaceInfo->transMode = ifaceCfg->transMode;
    ifaceInfo->modemId   = ifaceCfg->modemId;
    ifaceInfo->addrFamilyMask |= ifaceCfg->addrFamilyMask;
    ifaceInfo->pduSessionTbl[addrType] = ifaceCfg->pduSessionId;

    adsCtx->ifaceStatsBitMask = ADS_BIT64_SET(adsCtx->ifaceStatsBitMask, ifaceCfg->ifaceId);

    /* �����ӿ����� */
    (VOS_VOID)memset_s(&ifacePara, sizeof(ifacePara), 0x00, sizeof(ifacePara));

    ifacePara.addr_family    = addrType;
    ifacePara.modem_id       = (VOS_UINT8)ifaceInfo->modemId;
    ifacePara.pdu_session_id = ADS_BUILD_EX_PDU_SESSION(ifaceInfo->modemId, ifaceInfo->pduSessionTbl[addrType]);
    ifacePara.fc_head        = (addrType == ADS_ETH_ADDR) ? EIPF_ETH_ULFC : ADS_GET_FC_HEAD_BY_MODEMID(ifaceInfo->modemId);
#if ((FEATURE_SHARE_APN == FEATURE_ON) || (FEATURE_SHARE_PDP == FEATURE_ON))
    ifacePara.shared_iface_id = ifaceCfg->sharedIfaceId;
#endif

    ads_iface_bring_up(ifaceInfo->ifaceId, &ifacePara);

    /* ��������ͳ�ƶ�ʱ�� */
    ADS_StartTimer(TI_ADS_DSFLOW_STATS, TI_ADS_DSFLOW_STATS_LEN);

    return;
}


STATIC VOS_VOID ADS_IfaceDown(ADS_IFACE_Config *ifaceCfg, VOS_UINT8 addrType)
{
    ADS_IFACE_Info           *ifaceInfo = VOS_NULL_PTR;
    ADS_CONTEXT              *adsCtx    = VOS_NULL_PTR;
    struct ads_iface_config_s ifacePara;

    adsCtx    = ADS_GetCtx();
    ifaceInfo = ADS_GetIfaceInfo(ifaceCfg->ifaceId);

    if (ADS_IFACE_OP_TYPE_DOWN == ifaceInfo->stats) {
        ADS_NORMAL_LOG1("ADS_IfaceDown: Iface is already down ", ifaceCfg->ifaceId);
        return;
    }

    /* ˢ�������ӿ�״̬ */
    ifaceInfo->addrFamilyMask &= ~(ifaceCfg->addrFamilyMask);

    /* ��ַ��������Ϊ0 �����ӿ�down */
    if (0 == ifaceInfo->addrFamilyMask) {
        ADS_IfaceInfoInit(ifaceCfg->ifaceId);
        adsCtx->ifaceStatsBitMask = ADS_BIT64_CLR(adsCtx->ifaceStatsBitMask, ifaceCfg->ifaceId);
    }

    /* �����ӿ����� */
    (VOS_VOID)memset_s(&ifacePara, sizeof(ifacePara), 0x00, sizeof(ifacePara));

    ifacePara.addr_family              = addrType;
    ifaceInfo->pduSessionTbl[addrType] = ADS_INVAILD_PDU_SESSION_ID;

    ads_iface_bring_down(ifaceCfg->ifaceId, &ifacePara);

    /* �����ӿھ�δ���� ֹͣ����ͳ�ƶ�ʱ�� ����ͳ�� */
    if (0 == adsCtx->ifaceStatsBitMask) {
        ADS_StopTimer(TI_ADS_DSFLOW_STATS, ADS_TIMER_STOP_CAUSE_USER);
    }

    return;
}


VOS_VOID ADS_IfaceConfig(ADS_IFACE_Config *ifaceCfg)
{
    VOS_UINT32 i;
    VOS_UINT8  addrFamilyMask;

    /* ������Ч���ж� */
    if (PS_IFACE_ID_BUTT <= ifaceCfg->ifaceId) {
        ADS_ERROR_LOG1("ADS_IfaceConfig: IfaceId is invaild! IfaceId is", ifaceCfg->ifaceId);
        return;
    }

    /* ��¼��ڵ�ַ���� */
    addrFamilyMask = ifaceCfg->addrFamilyMask;

    /* ÿ������һ�ֵ�ַ���� */
    for (i = 0; i < ADS_ADDR_BUTT; i++) {
        if (!ADS_BIT8_IS_SET(addrFamilyMask, i)) {
            continue;
        }

        ifaceCfg->addrFamilyMask = ADS_BIT8_SET(0x00, i);

        /* ���ݲ������ͼ��ȥ���� */
        if (ADS_IFACE_OP_TYPE_UP == ifaceCfg->ifaceOpType) {
            ADS_IfaceUp(ifaceCfg, (VOS_UINT8)i);
        } else if (ADS_IFACE_OP_TYPE_DOWN == ifaceCfg->ifaceOpType) {
            ADS_IfaceDown(ifaceCfg, (VOS_UINT8)i);
        } else {
            ADS_ERROR_LOG("ADS_IfaceConfig: Iface Op is invaild!");
        }
    }

    return;
}

VOS_BOOL ADS_RaThrot(uint8_t ifaceId)
{
    ADS_CONTEXT *adsCtx = ADS_GetCtx();
    VOS_ULONG    now = jiffies;
    VOS_BOOL     ret = VOS_FALSE;

    if (ifaceId >= PS_IFACE_ID_BUTT) {
        return VOS_TRUE;
    }

    adsCtx->raThrot[ifaceId].raNum++;

    if (time_after(adsCtx->raThrot[ifaceId].startTime + ADS_RA_PROTECT_DUR, now)) {
        /* RA��������, ����RA���� */
        if (adsCtx->raThrot[ifaceId].raNum > ADS_RA_MAX_NUM) {
            return VOS_TRUE;
        }
    } else {
        /* ����������, ���¸�ֵ */
        adsCtx->raThrot[ifaceId].raNum = 1;
        adsCtx->raThrot[ifaceId].startTime = now;
    }

    return ret;
}


VOS_INT ADS_SpecialDataHook(IMM_Zc *immZc)
{
    IMM_ZcQueueTail(ADS_GetDataQue(), immZc);
    ADS_SndEvent(ADS_EVENT_SPECIAL_DATA_IND);
    return 0;
}


VOS_VOID ADS_ProcSpecialData(IMM_Zc *immZc)
{
    struct rx_cb_map_s *map = VOS_NULL_PTR;
    uint8_t ifaceId;

    map = (struct rx_cb_map_s *)immZc->cb;

    switch (map->ipf_result.bits.bid_qosid) {
        case CDS_ADS_DL_IPF_BID_ICMPV6_RA:
            ifaceId = ads_iface_get_iface_id(map);
            if (ADS_RaThrot(ifaceId) == VOS_TRUE) {
                break;
            }
            /* �����͵�RAֱ�������� */
        case CDS_ADS_DL_IPF_BID_DHCPV6:
        case CDS_ADS_DL_IPF_BID_ICMPV6:
        case CDS_ADS_DL_IPF_BID_MIP_ADV:
        case CDS_ADS_DL_IPF_BID_MIP_REG_REPLY:
            ADS_SndCdsDataInd(immZc, map);
            break;
        default:
            break;
    }

    IMM_ZcFreeAny(immZc); /* �ɲ���ϵͳ�ӹ� */
    return;
}


VOS_VOID ADS_ReportDlPktHook(struct ads_rx_ip_pkt_rec_s *adsDlIpkRec)
{
    ADS_MntnSndDlPktDataStatsInd(adsDlIpkRec);
    return;
}


VOS_VOID ADS_ReportUlPktHook(struct ads_tx_ip_pkt_rec_s *adsUlIpkRec)
{
    ADS_MntnSndUlPktDataStatsInd(adsUlIpkRec);
    return;
}


VOS_VOID ADS_RegIfaceCB(VOS_VOID)
{
    struct ads_iface_hook_ops_s ifaceHook;

    memset_s(&ifaceHook, sizeof(ifaceHook), 0x00, sizeof(ifaceHook));

    ifaceHook.special_data_hook = ADS_SpecialDataHook;
    ifaceHook.report_rx_pkt     = ADS_ReportDlPktHook;
    ifaceHook.report_tx_pkt     = ADS_ReportUlPktHook;
#if (FEATURE_PC5_DATA_CHANNEL == FEATURE_ON)
    ifaceHook.report_ltev_rx_pkt = ADS_ReportLtevDlPktHook;
    ifaceHook.report_ltev_tx_pkt = ADS_ReportLtevUlPktHook;
#endif

    ads_iface_register_hook(&ifaceHook);

    return;
}

#if (FEATURE_PC5_DATA_CHANNEL == FEATURE_ON)

VOS_VOID ADS_LtevIfaceUp(VOS_VOID)
{
    struct ads_iface_config_s stIfaceInfo;

    (VOS_VOID)memset_s(&stIfaceInfo, sizeof(stIfaceInfo), 0x00, sizeof(stIfaceInfo));

    stIfaceInfo.addr_family    = ADS_RAW_ADDR;
    stIfaceInfo.pdu_session_id = ADS_BUILD_EX_PDU_SESSION(0, 0);
    stIfaceInfo.fc_head        = EIPF_LTEV_ULFC;
    stIfaceInfo.modem_id       = 0;
#if ((FEATURE_SHARE_APN == FEATURE_ON) || (FEATURE_SHARE_PDP == FEATURE_ON))
    stIfaceInfo.shared_iface_id = PS_IFACE_ID_BUTT;
#endif

    ads_iface_bring_up(PS_IFACE_ID_LTEV, &stIfaceInfo);

    ltev_create(PS_IFACE_ID_LTEV);
}


VOS_VOID ADS_ReportLtevUlPktHook(struct ads_tx_ltev_pkt_rec_s *ads_ltev_ul_ipk_rec)
{
    ADS_MntnSndLtevUlPktDataStatsInd(ads_ltev_ul_ipk_rec);
    return;
}


VOS_VOID ADS_ReportLtevDlPktHook(struct ads_rx_ltev_pkt_rec_s *ads_ltev_dl_ipk_rec)
{
    ADS_MntnSndLtevDlPktDataStatsInd(ads_ltev_dl_ipk_rec);
    return;
}
#endif

