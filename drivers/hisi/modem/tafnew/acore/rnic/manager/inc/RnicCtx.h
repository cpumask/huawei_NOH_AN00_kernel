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
#ifndef _RNIC_CTX_H_
#define _RNIC_CTX_H_

#include "vos.h"
#include "PsLogdef.h"
#include "PsTypeDef.h"
#include "ps_common_def.h"
#include "ps_iface_global_def.h"
#include "imm_interface.h"
#include "imsa_rnic_interface.h"
#include "AtRnicInterface.h"
#include "RnicTimerMgmt.h"
#include "mdrv_nvim.h"
#include "acore_nv_stru_gucnas.h"
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#include "mdrv_eipf.h"
#include "ads_dev_i.h"
#else
#include "ads_device_interface.h"
#endif
#include <linux/pm_wakeup.h>
#include <linux/bitops.h>
#include "nv_stru_gucnas.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

#define RNIC_NET_ID_MAX_NUM (RNIC_DEV_ID_BUTT)
#define RNIC_3GPP_NET_ID_MAX_NUM (RNIC_DEV_ID_DATA_MAX)
#define RNIC_MODEM_ID_MAX_NUM (MODEM_ID_BUTT)
#define RNIC_3GPP_NET_MAX_NUM_MASK GENMASK(RNIC_3GPP_NET_ID_MAX_NUM, 0)

#define RNIC_IPV4_VERSION (4)       /* IPͷ����IP V4�汾�� */
#define RNIC_IPV6_VERSION (6)       /* IPͷ����IP V6�汾�� */
#define RNIC_MAX_IPV6_ADDR_LEN (16) /* IPV6��ַ���� */

#define RNIC_MAX_DSFLOW_BYTE (0xFFFFFFFF) /* RNIC�������ֵ */

#define RNIC_DIAL_DEMA_IDLE_TIME (600)

/* RabId�ĸ���λ��ʾModemId, 00��ʾModem0, 01��ʾModem1, 10��ʾModem1 */
#define RNIC_RABID_TAKE_MODEM_1_MASK (0x40) /* RabidЯ��Modem1������ */
#define RNIC_RABID_TAKE_MODEM_2_MASK (0x80) /* RabidЯ��Modem2������ */
#define RNIC_RABID_UNTAKE_MODEM_MASK (0x3F) /* Rabidȥ��Modem������ */

/* ���RABIDֵ */
#define RNIC_RAB_ID_MAX_NUM (11)
#define RNIC_RAB_ID_OFFSET (5)
/* Rab Id����Сֵ */
#define RNIC_RAB_ID_MIN (5)
/* Rab Id�����ֵ */
#define RNIC_RAB_ID_MAX (15)
#define RNIC_RAB_ID_INVALID (0xFF)

/* NAPI һ��Poll��ѯ��Ĭ���������� */
#define RNIC_NAPI_POLL_DEFAULT_WEIGHT (16)
/* NAPI һ��Poll��ѯ��Э��ջ֧�ֵ��������� */
#define RNIC_NAPI_POLL_MAX_WEIGHT (64)
#define RNIC_POLL_QUEUE_DEFAULT_MAX_LEN (15000) /* NAPI Poll���е���󳤶� */

#define RNIC_INVALID_IFACE_ID (0xFF)
#define RNIC_INVALID_PDU_SESSION_ID (0xFF)
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#define RNIC_INVALID_FC_HEAD EIPF_FCHEAD_BOTTOM
#endif
/* RNIC ����NV��Ϣ������ */
#define RNIC_NV_CFG_MAX_NUM 1

#define RNIC_RAB_ID_IS_VALID(ucRabId) (((ucRabId) >= RNIC_RAB_ID_MIN) && ((ucRabId) <= RNIC_RAB_ID_MAX))

#define RNIC_RMNET_IS_VALID(RmNetId) ((RmNetId) < RNIC_DEV_ID_BUTT)
#define RNIC_IFACE_ID_IS_VALID(ucIfaceId) ((ucIfaceId) < PS_IFACE_ID_BUTT)
#define RNIC_MODEM_ID_IS_VALID(enModemId) ((enModemId) < MODEM_ID_BUTT)

#define RNIC_BIT8_MASK(bit) ((VOS_UINT8)(1 << (bit)))
#define RNIC_BIT8_SET(val, bit) ((val) | RNIC_BIT8_MASK(bit))
#define RNIC_BIT8_CLR(val, bit) ((val) & ~RNIC_BIT8_MASK(bit))
#define RNIC_BIT8_IS_SET(val, bit) ((val)&RNIC_BIT8_MASK(bit))

#define RNIC_BIT32_MASK(bit) ((VOS_UINT32)(1 << (bit)))
#define RNIC_BIT32_SET(val, bit) ((val) = ((val) | RNIC_BIT32_MASK(bit)))
#define RNIC_BIT32_CLR(val, bit) ((val) = ((val) & ~RNIC_BIT32_MASK(bit)))
#define RNIC_BIT32_IS_SET(val, bit) ((val)&RNIC_BIT32_MASK(bit))

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
#define RNIC_RMNET_R_IS_EMC_BEAR(RmNetId) \
    ((RNIC_DEV_ID_RMNET_R_IMS01 == (RmNetId)) || (RNIC_DEV_ID_RMNET_R_IMS11 == (RmNetId)))
#else
#define RNIC_RMNET_R_IS_EMC_BEAR(RmNetId) (RNIC_DEV_ID_RMNET_R_IMS01 == (RmNetId))
#endif

/* ��ȡRNIC�����ĵ�ַ */
#define RNIC_GET_RNIC_CTX_ADR() (&g_rnicCtx)
/* ��ȡָ�������������ĵ�ַ */
#define RNIC_GET_IFACE_CTX_ADR(indexNum) (&(g_rnicCtx.ifaceCtx[indexNum]))
/* ��ȡָ��������PDP�����ĵ�ַ */
#define RNIC_GET_IFACE_PDN_INFO_ADR(indexNum) (&(g_rnicCtx.ifaceCtx[indexNum].psIfaceInfo))
/* ��ȡIPV4����PDN����״̬ */
#define RNIC_GET_IFACE_PDN_IPV4_ACT_STATE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].psIfaceInfo.opIpv4Act)
/* ��ȡIPV6����PDN����״̬ */
#define RNIC_GET_IFACE_PDN_IPV6_ACT_STATE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].psIfaceInfo.opIpv6Act)

/* ��ȡ��ǰ���ձ����� */
#define RNIC_GET_IFACE_PERIOD_RECV_PKT(indexNum) (g_rnicCtx.ifaceCtx[indexNum].dsFlowStats.periodRecvPktNum)
/* ��ȡ��ǰ���ͱ����� */
#define RNIC_GET_IFACE_PERIOD_SEND_PKT(indexNum) (g_rnicCtx.ifaceCtx[indexNum].dsFlowStats.periodSendPktNum)
/* ���õ�ǰ���ͱ����� */
#define RNIC_SET_IFACE_PERIOD_SEND_PKT(indexNum, val) \
    (g_rnicCtx.ifaceCtx[indexNum].dsFlowStats.periodSendPktNum = (val))
/* �����ǰ���ձ����� */
#define RNIC_CLEAN_IFACE_PERIOD_RECV_PKT(indexNum) (g_rnicCtx.ifaceCtx[indexNum].dsFlowStats.periodRecvPktNum = 0)
/* �����ǰ���ͱ����� */
#define RNIC_CLEAN_IFACE_PERIOD_SEND_PKT(indexNum) (g_rnicCtx.ifaceCtx[indexNum].dsFlowStats.periodSendPktNum = 0)

/* ��ȡָ������NAPI���Կ��� */
#define RNIC_GET_NAPI_FEATURE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiEnable)
/* ��ȡָ������NAPI���Կ��� */
#define RNIC_GET_GRO_FEATURE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.groEnable)
/* ��ȡ����NAPI weightֵ����ģʽ */
#define RNIC_GET_NAPI_WEIGHT_ADJ_MODE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiWeightAdjMode)
/* ��ȡ����NAPI weightֵ */
#define RNIC_GET_NAPI_WEIGHT(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiPollWeight)
/* ��ȡ����NAPI Poll���г������ֵ */
#define RNIC_GET_NAPI_POLL_QUE_MAX_LEN(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiPollQueMaxLen)
/* ��ȡָ��������NAPI Weight��̬�������õ�ַ */
#define RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum) \
    (&(g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiWeightDynamicAdjCfg))
/* ��ȡ����NAPI����ÿ���հ�������λֵ1 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL1(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel1)
/* ��ȡ����NAPI����ÿ���հ�������λֵ2 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL2(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel2)
/* ��ȡ����NAPI����ÿ���հ�������λֵ3 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL3(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel3)
/* ��ȡ����NAPI����ÿ���հ�������λֵ4 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL4(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel4)

/* ��ȡ����NAPI weight��λֵ1 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL1(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel1)
/* ��ȡ����NAPI weight��λֵ2 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL2(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel2)
/* ��ȡ����NAPI weight��λֵ3 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL3(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel3)
/* ��ȡ����NAPI weight��λֵ4 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL4(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel4)
/* ��ȡ����NAPI weight��λֵ5 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL5(indexNum) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel5)

/* ��������NAPI���Կ��� */
#define RNIC_SET_NAPI_FEATURE(indexNum, val) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiEnable = (val))
/* ��������NAPI���Կ��� */
#define RNIC_SET_GRO_FEATURE(indexNum, val) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.groEnable = (val))
/* ��������NAPI weightֵ����ģʽ */
#define RNIC_SET_NAPI_WEIGHT_ADJ_MODE(indexNum, val) \
    (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiWeightAdjMode = (val))
/* ��������NAPI weightֵ */
#define RNIC_SET_NAPI_WEIGHT(indexNum, val) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiPollWeight = (val))
/* ��������NAPI Poll���г������ֵ */
#define RNIC_SET_NAPI_POLL_QUE_MAX_LEN(indexNum, val) \
    (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiCfg.napiPollQueMaxLen = (val))

/* ��������NAPI����ÿ���հ�������λֵ1 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL1(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel1 = (val))
/* ��������NAPI����ÿ���հ�������λֵ2 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL2(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel2 = (val))
/* ��������NAPI����ÿ���հ�������λֵ3 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL3(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel3 = (val))
/* ��������NAPI����ÿ���հ�������λֵ4 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL4(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel4 = (val))
/* ��������NAPI����ÿ���հ�������λֵ5 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL5(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->dlPktNumPerSecLevel.dlPktNumPerSecLevel5 = (val))

/* ��������NAPI weight��λֵ1 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL1(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel1 = (val))
/* ��������NAPI weight��λֵ2 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL2(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel2 = (val))
/* ��������NAPI weight��λֵ3 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL3(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel3 = (val))
/* ��������NAPI weight��λֵ4 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL4(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel4 = (val))
/* ��������NAPI weight��λֵ5 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL5(indexNum, val) \
    (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(indexNum)->napiWeightLevel.napiWeightLevel5 = (val))

/* ��ȡRNIC�����ĵ�ַ */
#define RNIC_GET_RNIC_TIMER_ADR() (g_rnicCtx.timerCtx)

#if (defined(CONFIG_BALONG_SPE))
/* ��ȡָ��������SPE���õ�ַ */
#define RNIC_GET_SPE_CFG_ADR(indexNum) (&(g_rnicCtx.ifaceCtx[indexNum].featureCfg.stSpeCfg))
#endif

/* ���貦�Žڵ���� */
#define RNIC_GET_DIAL_MODE_ADR() (&(g_rnicCtx.dialMode))
#define RNIC_GET_PROC_DIAL_MODE() (g_rnicCtx.dialMode.dialMode)
#define RNIC_SET_PROC_DIAL_MODE(val) (g_rnicCtx.dialMode.dialMode = (val))
#define RNIC_GET_PROC_IDLE_TIME() (g_rnicCtx.dialMode.idleTime)
#define RNIC_SET_PROC_IDLE_TIME(val) (g_rnicCtx.dialMode.idleTime = (val))
#define RNIC_GET_PROC_EVENT_REPORT() (g_rnicCtx.dialMode.eventReportFlag)
#define RNIC_SET_PROC_EVENT_REPORT(val) (g_rnicCtx.dialMode.eventReportFlag = (val))

/* ��ȡ��ǰ��IPFģʽ */
#define RNIC_GET_IPF_MODE() (g_rnicCtx.ipfMode)
/* ��ȡ��λ�ź� */
#define RNIC_GET_RESET_SEM() (g_rnicCtx.resetSem)

/* ����Ͽ��������� */
#define RNIC_GET_TI_DIALDOWN_EXP_CONT() (g_rnicCtx.tiDialDownExpCount)
#define RNIC_CLEAR_TI_DIALDOWN_EXP_CONT() (g_rnicCtx.tiDialDownExpCount = 0)
#define RNIC_ADD_TI_DIALDOWN_EXP_CONT() (g_rnicCtx.tiDialDownExpCount++)

/* Tethering���� */
#define RNIC_GET_TETHER_ORIG_GRO_FEATURE() (g_rnicCtx.tetherInfo.origGroEnable)
#define RNIC_SET_TETHER_ORIG_GRO_FEATURE(val) (g_rnicCtx.tetherInfo.origGroEnable = (val))

/* ��ȡָ������NAPI LB���õ�ַ */
#define RNIC_GET_NAPI_LB_CFG_ADR(indexNum) (&(g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg))

/* ��ȡָ������NAPI���Կ��� */
#define RNIC_GET_NAPI_LB_FEATURE(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbEnable)
/* ��ȡָ������NAPI���ؾ���CPU���� */
#define RNIC_GET_NAPI_LB_CPUMASK(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbCpumask)
/* ��ȡָ������NAPI���ؾ��⵱ǰ��λ */
#define RNIC_GET_NAPI_LB_CUR_LEVEL(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbCurLevel)
/* ��ȡָ������NAPI���ؾ��⵵λ��Ӧ��pps */
#define RNIC_GET_NAPI_LB_LEVEL_PPS(indexNum, level) \
    (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbLevelCfg[(level)].pps)
/* ��ȡָ������NAPI���ؾ��⵵λ���� */
#define RNIC_GET_NAPI_LB_LEVEL_CFG(indexNum) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbLevelCfg)

/* ��������NAPI���Կ��� */
#define RNIC_SET_NAPI_LB_FEATURE(indexNum, val) (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbEnable = (val))
/* ����ָ������NAPI���ؾ���CPU���� */
#define RNIC_SET_NAPI_LB_CPUMASK(indexNum, val) \
    (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbCpumask = (val))
/* ����ָ������NAPI���ؾ��⵱ǰ��λ */
#define RNIC_SET_NAPI_LB_CUR_LEVEL(indexNum, val) \
    (g_rnicCtx.ifaceCtx[indexNum].featureCfg.napiLbCfg.napiLbCurLevel = (val))


enum RNIC_NetcardStatusType {
    RNIC_NETCARD_STATUS_CLOSED, /* RNICΪ�ر�״̬ */
    RNIC_NETCARD_STATUS_OPENED, /* RNICΪ��״̬ */
    RNIC_NETCARD_STATUS_BUTT
};
typedef VOS_UINT8 RNIC_NetcardStatusUint8;


enum RNIC_FlowCtrlStatus {
    RNIC_FLOW_CTRL_STATUS_STOP  = 0x00, /* ����ֹͣ */
    RNIC_FLOW_CTRL_STATUS_START = 0x01, /* �������� */
    RNIC_FLOW_CTRL_STATUS_BUTT  = 0xFF
};
typedef VOS_UINT32 RNIC_FlowCtrlStatusUint32;


enum RNIC_IPF_Mode {
    RNIC_IPF_MODE_INT  = 0x00, /* �ж������� */
    RNIC_IPF_MODE_THRD = 0x01, /* �߳������� */
    RNIC_IPF_MODE_BUTT
};
typedef VOS_UINT8 RNIC_IPF_ModeUint8;


enum RNIC_FeatureEnable {
    RNIC_FEATURE_OFF = 0x00,
    RNIC_FEATURE_ON  = 0x01,
    RNIC_FEATURE_BUTT
};
typedef VOS_UINT8 RNIC_FeatureEnableUint8;


enum RNIC_DialMode {
    RNIC_DIAL_MODE_MANUAL,            /* Manual dial mode */
    RNIC_DIAL_MODE_DEMAND_CONNECT,    /* Demand dial mode */
    RNIC_DIAL_MODE_DEMAND_DISCONNECT, /* Demand dial mode */
    RNIC_DIAL_MODE_BUTT
};
typedef VOS_UINT32 RNIC_DialModeUint32;

enum RNIC_DialEventReportFlag {
    RNIC_FORBID_EVENT_REPORT = 0x0000, /* ����Ӧ���ϱ� */
    RNIC_ALLOW_EVENT_REPORT  = 0X0001, /* �����Ӧ���ϱ� */

    RNIC_DIAL_EVENT_REPORT_FLAG_BUTT
};
typedef VOS_UINT32 RNIC_DialEventReportFlagUint32;


enum RNIC_PS_RatType {
    RNIC_PS_RAT_TYPE_3GPP  = 0x00,
    RNIC_PS_RAT_TYPE_IWLAN = 0x01,
    RNIC_PS_RAT_TYPE_BUTT
};
typedef VOS_UINT8 RNIC_PS_RatTypeUint8;

typedef VOS_VOID (*RNIC_SPE_RX_PUSH_CB)(VOS_VOID);
typedef VOS_INT (*RNIC_SPE_MEM_RECYCLE_CB)(IMM_Zc *pstImmZc);

/*
 * �ṹ˵��: ����ͳ�ƽṹ
 */
typedef struct {
    /* ��ǰ�������ʣ�����PDP�����1������ʣ�ȥ�������� */
    VOS_UINT32 currentRecvRate;
    /* ��ǰ�������ʣ�����PDP�����1������ʣ�ȥ�������� */
    VOS_UINT32 currentSendRate;

    /* �����յ����ݰ�����,ͳ��һ�����ŶϿ���ʱ���������յ��ĸ�������ʱ����� */
    VOS_UINT32 periodRecvPktNum;
    /* ���з�����Ч����,ͳ��һ�����ŶϿ���ʱ���������յ��ĸ�������ʱ����� */
    VOS_UINT32 periodSendPktNum;

    /* ���е�λʱ�����յ��ķ�TCP������ */
    VOS_UINT32 periodRecvNonTcpPktNum;
    /* ���е�λʱ�����յ���TCP������ */
    VOS_UINT32 periodRecvTcpPktNum;
} RNIC_DSFLOW_Stats;


typedef struct {
    VOS_UINT8                  rmnetName[RNIC_RMNET_NAME_MAX_LEN]; /* Rmnet������ */
    AT_RNIC_UsbTetherConnUint8 tetherConnStat;                     /* Tethering����״̬ */
    VOS_UINT8                  origGroEnable;                      /* Tethering����״̬ */
    VOS_UINT8                  rsv[6];                             /* ����λ */
} RNIC_TetherInfo;


typedef struct {
    VOS_UINT8     feature;                           /* ӵ���������Կ��� */
    VOS_UINT8     rhcLevel;                          /* ��ǰ���ո��ٿ��Ƶ�λ */
    VOS_UINT8     qccEnable;                         /* NAPI����ӵ�����ƹ��ܿ��� */
    VOS_UINT8     napiWtExpEnable;                   /* NAPI����ӵ������Weight���Ź��ܿ��� */
    VOS_UINT8     ddrReqEnable;                      /* ����NAPI����ӵ���̶ȸ�DDRͶƱ���ܿ��� */
    VOS_UINT8     qccTimeout;                        /* NAPI����ӵ�����Ƴ���ʱ�� */
    VOS_UINT8     napiPollMax;                       /* NAPI poll�������ִ�д��� */
    VOS_UINT8     reserved1;                         /* ����λ */
    VOS_INT32     ddrMidBd;                          /* �е�λDDR���� */
    VOS_INT32     ddrHighBd;                         /* �ߵ�λDDR���� */
    RNIC_RhcLevel levelCfg[RNIC_NVIM_RHC_MAX_LEVEL]; /* ���ո��ٿ��Ƶ�λ���� */
} RNIC_RHC_Cfg;


typedef struct {
    RNIC_DialModeUint32            dialMode;        /* Dialģʽ */
    VOS_UINT32                     idleTime;        /* ��ʱ�����ȣ���λΪ�� */
    RNIC_DialEventReportFlagUint32 eventReportFlag; /* �Ƿ��Ӧ���ϱ���ʶ */
    VOS_UINT32                     reserved;
} RNIC_DialModePara;


typedef struct {
    VOS_UINT8 napiEnable; /* NAPI���� */
    /* NAPI Weight����ģʽ, 0: ��̬ģʽ��1����̬����ģʽ */
    NAPI_WEIGHT_AdjModeUint8 napiWeightAdjMode;
    /* RNIC����NAPI��ʽһ��poll��������� */
    VOS_UINT8                     napiPollWeight;
    VOS_UINT8                     groEnable;               /* GRO���� */
    VOS_UINT16                    napiPollQueMaxLen;       /* Napi poll�������֧�ֳ��� */
    VOS_UINT8                     reserved[2];             /* ����λ */
    RNIC_NAPI_WeightDynamicAdjCfg napiWeightDynamicAdjCfg; /* Napi Weight��̬�������� */

} RNIC_NapiCfg;


typedef struct {
    VOS_UINT8            napiLbEnable;                                /* NAPI���ؾ��⹦�ܿ��� */
    VOS_UINT8            napiLbCurLevel;                              /* ��ǰ���ؾ��⵵λ */
    VOS_UINT16           napiLbCpumask;                               /* ����Napi���ؾ����CPU���� */
    VOS_UINT8            reserved0;                                   /* ����λ */
    VOS_UINT8            reserved1;                                   /* ����λ */
    VOS_UINT8            reserved2;                                   /* ����λ */
    VOS_UINT8            reserved3;                                   /* ����λ */
    VOS_UINT32           rsv0;                                        /* ����λ */
    VOS_UINT32           rsv1;                                        /* ����λ */
    RNIC_NAPI_LbLevelCfg napiLbLevelCfg[RNIC_NVIM_NAPI_LB_MAX_LEVEL]; /* Napi���ؾ��⵵λ���� */

} RNIC_NapiLbCfg;

#if (defined(CONFIG_BALONG_SPE))

typedef struct {
    VOS_UINT32              tdDepth;           /* TD��� */
    VOS_UINT32              rdDepth;           /* RD��� */
    RNIC_SPE_RX_PUSH_CB     speRxPushFunc;     /* SPE�����ж���ɻص� */
    RNIC_SPE_MEM_RECYCLE_CB speMemRecylceFunc; /* SPE�ڴ���ջص� */

} RNIC_SpeCfg;
#endif


typedef struct {
    RNIC_NapiCfg   napiCfg;   /* NAPI,GRO�������� */
    RNIC_NapiLbCfg napiLbCfg; /* NAPI���жϸ��ؾ������� */

#if (defined(CONFIG_BALONG_SPE))
    RNIC_SpeCfg stSpeCfg; /* SPE���� */
#endif
} RNIC_FeatureCfg;


typedef struct {
    VOS_UINT32 opIpv4Act : 1;
    VOS_UINT32 opIpv6Act : 1;
    VOS_UINT32 opEthAct : 1;
    VOS_UINT32 opSpare : 29;

    VOS_UINT8  ipv4RabId;
    VOS_UINT8  ipv4ExRabId; /* ��չ����:Modem ID + Rab ID */
    VOS_UINT8  reserved1[2];
    VOS_UINT32 ipv4Addr; /* IPv4��ַ */

    VOS_UINT8 ipv6RabId;
    VOS_UINT8 ipv6ExRabId; /* ��չ����:Modem ID + Rab ID */

    VOS_UINT8  ethRabId;
    VOS_UINT8  ethExRabId;
    /* ��AT������IPV6��ַ���ȣ�������":" */
    VOS_UINT8 ipv6Addr[RNIC_MAX_IPV6_ADDR_LEN];

    ModemIdUint16        modemId; /* ���������ĸ�modem */
    RNIC_PS_RatTypeUint8 ratType; /* psע���� */
    VOS_UINT8            reserved3[5];

    IMM_ZcHeader imsQue; /* IMS�������ݷ��ͻ������ */

} RNIC_PS_IfaceInfo;


typedef struct {
    RNIC_DEV_ID_ENUM_UINT8 rmNetId; /* �����豸��Ӧ������ID */
    PS_IFACE_IdUint8       ifaceId;
    VOS_UINT8              reserved[6];
    RNIC_PS_IfaceInfo      psIfaceInfo; /* �����豸��Ӧ��PDN��Ϣ */
    RNIC_FeatureCfg        featureCfg;  /* RNIC�������� */
    RNIC_DSFLOW_Stats      dsFlowStats; /* ������Ϣ */

} RNIC_IFACE_Ctx;


typedef struct {
    /* RNIC�����豸������ */
    RNIC_IFACE_Ctx ifaceCtx[RNIC_NET_ID_MAX_NUM];

    /* RNIC��ʱ�������� */
    RNIC_TIMER_Ctx timerCtx[RNIC_MAX_TIMER_NUM];

    /* RNIC���������� */
    /* ����ģʽ */
    RNIC_DialModePara dialMode;
    /* ���ŶϿ���ʱ����ʱ��������ͳ�� */
    VOS_UINT32 tiDialDownExpCount;
    /* IPF����ADS�������ݵ�ģʽ, 0: �ж�������(Ĭ��)��1���߳������� */
    ADS_IpfModeUint8 ipfMode;
    VOS_UINT8 devReady;
    VOS_UINT8 rsv[2];
    /* �������ź��������ڸ�λ���� */
    VOS_SEM         resetSem;
    RNIC_TetherInfo tetherInfo;
    /* ���ո��ٿ������� */
    RNIC_RHC_Cfg         rhcCfg;
    struct wakeup_source onDemandDisconnWakeLock;
    VOS_UINT32           dsflowTimerMask;
    VOS_UINT16           rnicCfgIndNum;
    VOS_UINT16           rnicLbCfgIndNum;
    VOS_UINT16           rhcCfgIndNum;
    VOS_UINT16           reserved;
    VOS_UINT32           reserved1;
} RNIC_CTX;

extern RNIC_CTX g_rnicCtx;

VOS_VOID RNIC_InitCtx(VOS_VOID);
VOS_VOID RNIC_InitPdnInfo(RNIC_PS_IfaceInfo *pdnInfo);
VOS_VOID RNIC_InitNapiCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId, RNIC_NapiCfg *napiCfg);
VOS_VOID RNIC_InitNapiLbCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId, RNIC_NapiLbCfg *napiLbCfg);
VOS_VOID RNIC_InitIpv4PdnInfo(RNIC_PS_IfaceInfo *pdnInfo);
VOS_VOID RNIC_InitIpv6PdnInfo(RNIC_PS_IfaceInfo *pdnInfo);
VOS_VOID RNIC_InitEthPdnInfo(RNIC_PS_IfaceInfo *pdnInfo);
VOS_VOID RNIC_InitOnDemandDialInfo(VOS_VOID);
RNIC_IFACE_Ctx* RNIC_GetNetCntxtByRmNetId(RNIC_DEV_ID_ENUM_UINT8 rmNetId);
VOS_VOID        RNIC_InitResetSem(VOS_VOID);
VOS_VOID        RNIC_CheckNapiCfgValid(TAF_NV_RnicNapiCfg *napiCfg, TAF_NV_RnicNapiLbCfg *napiLbCfg);
VOS_VOID        RNIC_InitTetherInfo(VOS_VOID);

#if (defined(CONFIG_BALONG_SPE))
VOS_VOID RNIC_InitSpeCfg(RNIC_DEV_ID_ENUM_UINT8 rmNetId);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _RNIC_CTX_H_ */
