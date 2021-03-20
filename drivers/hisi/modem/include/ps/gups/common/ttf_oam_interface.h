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

#ifndef TTFOAMINTERFACE_H
#define TTFOAMINTERFACE_H

#include "vos.h"
#include "ttf_link_interface.h"
#include "PsTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(push, 4)

/* �������������������Чֵ */
#define TTF_SENSITIVE_INVALID_VALUE 0

#define GRM_MNTN_TS_TOTAL 8     /* ʱ϶������ts0-ts7��8�� */
#define GRLC_MNTN_MAX_TBF_NUM 1 /* ��ǰ֧�ֵ����TBF���Ϊ1����֧�ֶ�TBF */
#define WTTF_MNTN_MAX_AM_RB_NUM 8 /* 3��AMҵ��RB,���RB2-RB4 ��������RB,����6��AM RB. 25.306 R9Э��Table 5.1b������8��AM RB */
#define WTTF_MNTN_MAX_TM_RB_NUM 4 /* TM RB����,RB0������, 3��AMR����RB + 1��TM CSD����4��TM RB, */
#define WTTF_MNTN_MAX_UM_RB_NUM 6 /* UM RB����, CBS����Ӧ��RB��RB0������,GCF����Ҫ����������UMҵ��RB,RB1ΪUM����RB,����3��UM RB ����AMʵ�������Ӧ */

#define WTTF_MNTN_MAX_RB_ID 32                  /* INTEGER (1..32) */
#define WTTF_MNTN_MAX_FLX_RLC_SIZE_LEVEL_CNT 10 /* �����ñ䳤SIZE��λ�� */
#define WTTF_MNTN_RLC_AM_MAX_SN_NUM 4096



/* ����PID��Χ */
#define IS_WUEPS_PID_PDCP(x) ((x == I0_WUEPS_PID_PDCP) || (x == I1_WUEPS_PID_PDCP))
#define IS_WUEPS_PID_RLC(x) ((x == I0_WUEPS_PID_RLC) || (x == I1_WUEPS_PID_RLC))
#define IS_WUEPS_PID_RLCDL(x) ((x == I0_WUEPS_PID_RLCDL) || (x == I1_WUEPS_PID_RLCDL))
#define IS_WUEPS_PID_MAC(x) ((x == I0_WUEPS_PID_MAC) || (x == I1_WUEPS_PID_MAC))
#define IS_WUEPS_PID_MACDL(x) ((x == I0_WUEPS_PID_MACDL) || (x == I1_WUEPS_PID_MACDL))
#define IS_UEPS_PID_SN(x) ((x == I0_UEPS_PID_SN) || (x == I1_UEPS_PID_SN) || (x == I2_UEPS_PID_SN))
#define IS_UEPS_PID_LL(x) ((x == I0_UEPS_PID_LL) || (x == I1_UEPS_PID_LL) || (x == I2_UEPS_PID_LL))
#define IS_UEPS_PID_GRM(x) ((x == I0_UEPS_PID_GRM) || (x == I1_UEPS_PID_GRM) || (x == I2_UEPS_PID_GRM))
#define IS_UEPS_PID_DL(x) ((x == I0_UEPS_PID_DL) || (x == I1_UEPS_PID_DL) || (x == I2_UEPS_PID_DL))
#define IS_UEPS_PID_GMM(x) ((x == I0_WUEPS_PID_GMM) || (x == I1_WUEPS_PID_GMM) || (x == I2_WUEPS_PID_GMM))
#define IS_UEPS_PID_SMS(x) ((x == I0_WUEPS_PID_SMS) || (x == I1_WUEPS_PID_SMS) || (x == I2_WUEPS_PID_SMS))
#define IS_UEPS_PID_GAS(x) ((x == I0_UEPS_PID_GAS) || (x == I1_UEPS_PID_GAS) || (x == I2_UEPS_PID_GAS))
#define IS_UEPS_PID_GPHY(x) ((x == I0_DSP_PID_GPHY) || (x == I1_DSP_PID_GPHY) || (x == I2_DSP_PID_GPHY))
#define IS_UEPS_PID_WPHY(x) ((x == I0_DSP_PID_WPHY) || (x == I1_DSP_PID_WPHY))
#define IS_WUEPS_PID_WAS(x) \
    ((x == I0_WUEPS_PID_WRR) || (x == I1_WUEPS_PID_WRR) || (x == I0_WUEPS_PID_WCOM) || (x == I1_WUEPS_PID_WCOM))

#define IS_UEPS_PID_RABM(x) ((x == I0_WUEPS_PID_RABM) || (x == I1_WUEPS_PID_RABM) || (x == I2_WUEPS_PID_RABM))
#define IS_UEPS_PID_TAF(x) ((x == I0_WUEPS_PID_TAF) || (x == I1_WUEPS_PID_TAF) || (x == I2_WUEPS_PID_TAF))
#define IS_PID_USIM(x) (((x) == I0_WUEPS_PID_USIM) || ((x) == I1_WUEPS_PID_USIM) || ((x) == I2_WUEPS_PID_USIM))
#define IS_PID_EHSM(x) ((x) == UEPS_PID_EHSM)
/* ö��˵��  : ��ά�ɲ���Ϣ����(͸����Ϣ), �����е�͸����Ϣ��ID������������ */
enum TTF_MntnMsgType {
    /* TTF��͸����ϢĿǰ��������MODEM */

    /* Wģ��ά�ɲ���Ϣ�ı����DO��ͷ */
    ID_OM_WTTF_RLC_DATA_TRANS_INFO_REQ      = 0xD001, /* Wģ��RLC ʵ����Ϣ�ϱ�����,���й�����AM��UM��TM����ģʽ�������û�������ȫ���򲿷���ʾ */
    ID_WTTF_OM_RLC_DATA_TRANS_INFO_CNF      = 0xD002, /* Wģ��RLC ʵ����Ϣ�ϱ��ظ� */
    ID_WTTF_OM_RLC_DATA_TRANS_INFO_IND      = 0xD003, /* Wģ��RLC ʵ����Ϣ�ϱ�ָʾ */
    ID_OM_WTTF_ADVANCED_TRACE_CONFIG_REQ    = 0xD004, /* Wģ��Trace �߼����� */
    ID_WTTF_OM_ADVANCED_TRACE_CONFIG_CNF    = 0xD005, /* Wģ��Trace �߼�����ָʾ */
    ID_OM_WTTF_RLC_PDU_SIZE_TRANS_INFO_REQ  = 0xD006, /* Wģ��RLC PDU SIZEͳ����Ϣ�ϱ����� */
    ID_WTTF_OM_RLC_PDU_SIZE_TRANS_INFO_CNF  = 0xD007, /* Wģ��RLC PDU SIZEͳ����Ϣ�ϱ��ظ� */
    ID_WTTF_OM_RLC_PDU_SIZE_TRANS_INFO_IND  = 0xD008, /* Wģ��RLC PDU SIZEͳ����Ϣ�ϱ�ָʾ */
    ID_OM_WTTF_UPA_DATA_STATUS_G_INFO_REQ   = 0xD009, /* Wģ��UPA Ҫ����ϢGֵ�ϱ����� */
    ID_WTTF_OM_UPA_DATA_STATUS_G_INFO_CNF   = 0xD00a, /* Wģ��UPA Ҫ����ϢGֵ�ϱ��ظ� */
    ID_WTTF_OM_UPA_DATA_STATUS_G_INFO_IND   = 0xD00b, /* Wģ��UPA Ҫ����ϢGֵ�ϱ�ָʾ */
    ID_OM_TTF_MNTN_MSG_CTTF_OM_XXX_REQ      = 0xD00c,
    ID_OM_TTF_MNTN_MSG_CTTF_OM_YYY_REQ      = 0xD00d,
    ID_TTF_OM_MNTN_MSG_OM_CTTF_XXX_DATA_IND = 0xD00e,
    ID_TTF_OM_MNTN_MSG_OM_CTTF_ZZZ_DATA_IND = 0xD00f,
    ID_OM_WTTF_MAC_DATA_INFO_REQ            = 0xD010, /* Wģ��MAC ʵ����Ϣ�ϱ� */
    ID_OM_WTTF_MAC_DATA_INFO_CNF            = 0xD011, /* Wģ��MAC ʵ����Ϣ�ظ� */
    ID_OM_WTTF_MAC_DATA_INFO_IND            = 0xD012, /* Wģ��MAC ʵ����Ϣָʾ */

    /* Gģ��ά�ɲ���Ϣ�ı����D1��ͷ */
    ID_GRM_MNTN_OM_GRM_INFO_CFG_REQ          = 0xD104, /* ����GRM��Ϣ�۲⹦�� */
    ID_GRM_MNTN_GRM_OM_INFO_CFG_CNF          = 0xD105,
    ID_GRM_MNTN_GRM_OM_INFO                  = 0xD106,
    ID_GRM_MNTN_OM_GRM_THROUGHPUT_CFG_REQ    = 0xD107, /* OM��GRM��������GRM������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_GRM_MNTN_GRM_OM_THROUGHPUT_CFG_CNF    = 0xD108, /* GRM��OM�ظ�����GRM������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_GRM_MNTN_GRM_OM_THROUGHPUT_INFO       = 0xD109, /* GRM��OM�ϱ�GRM������ͳ����Ϣ��Ϣ�� */
    ID_GRM_MNTN_OM_GTTF_ADV_TRACE_SWITCH_REQ = 0xD10a, /* Gģ��Trace �߼����ÿ������� */
    ID_GRM_MNTN_GTTF_OM_ADV_TRACE_SWITCH_CNF = 0xD10b, /* Gģ��Trace �߼����ÿ��ػظ� */
    ID_SN_MNTN_OM_THROUGHPUT_CFG_REQ         = 0xD10c, /* OM��SNDCP��������SNDCP������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_SN_MNTN_OM_THROUGHPUT_CFG_CNF         = 0xD10d, /* SNDCP��OM�ظ�����SNDCP������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_SN_MNTN_OM_THROUGHPUT_INFO            = 0xD10e, /* SNDCP��OM�ϱ�GRM������ͳ����Ϣ��Ϣ�� */
    ID_LL_MNTN_OM_THROUGHPUT_CFG_REQ         = 0xD10f, /* OM��LLC��������LLC������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_LL_MNTN_OM_THROUGHPUT_CFG_CNF         = 0xD110, /* LLC��OM�ظ�����LLC������ͳ����Ϣ�۲⹦����Ϣ�� */
    ID_LL_MNTN_OM_THROUGHPUT_INFO            = 0xD111, /* LLC��OM�ϱ�GRM������ͳ����Ϣ��Ϣ�� */

    /* RRM�Ŀ�ά�ɲ���Ϣ�ı����D2��ͷ */
    ID_RRM_OM_RESOURCE_STATE_INFO_IND = 0xD210, /* RRM�ϱ�����Դ״̬��Ϣ */

    /* AP�ϵ�TCPЭ��ջ�Ŀ�ά�ɲ���Ϣ, �����D3��ͷ */
    ID_OM_IPS_MNTN_TRAFFIC_CTRL_REQ         = 0xD312, /* ����IPS LOG���ع��� */
    ID_OM_IPS_ADVANCED_TRACE_CONFIG_REQ     = 0xD313, /* ����TCP/IPЭ��ջ �������� */
    ID_IPS_OM_ADVANCED_TRACE_CONFIG_CNF     = 0xD314, /* ����TCP/IPЭ��ջ ����ָʾ */
    ID_OM_IPS_MNTN_INFO_CONFIG_REQ          = 0xD315, /* �����Բ���TCP/IPЭ��ջ ������Ϣ���� */
    ID_IPS_OM_MNTN_INFO_CONFIG_CNF          = 0xD316, /* �����Բ���TCP/IPЭ��ջ ������Ϣ����ָʾ */
    ID_IPS_TRACE_INPUT_DATA_INFO            = 0xD317, /* TCP/IP��ά�ɲ���ձ��� */
    ID_IPS_TRACE_OUTPUT_DATA_INFO           = 0xD318, /* TCP/IP��ά�ɲⷢ�ͱ��� */
    ID_IPS_TRACE_BRIDGE_DATA_INFO           = 0xD319, /* TCP/IP������ת���� */
    ID_IPS_TRACE_RECV_ARP_PKT               = 0xD31A, /* TCP/IPЭ��ջ���յ�ARP���Ʊ��� */
    ID_IPS_TRACE_SEND_ARP_PKT               = 0xD31B, /* TCP/IPЭ��ջ���͵�ARP���Ʊ��� */
    ID_IPS_TRACE_RECV_DHCPC_PKT             = 0xD31C, /* TCP/IPЭ��ջ���յ�DHCP���Ʊ��� */
    ID_IPS_TRACE_SEND_DHCPC_PKT             = 0xD31D, /* TCP/IPЭ��ջ���͵�DHCP���Ʊ��� */
    ID_IPS_TRACE_RECV_DHCPS_PKT             = 0xD31E, /* TCP/IPЭ��ջ���յ�DHCP���Ʊ��� */
    ID_IPS_TRACE_SEND_DHCPS_PKT             = 0xD31F, /* TCP/IPЭ��ջ���͵�DHCP���Ʊ��� */
    ID_IPS_TRACE_APP_CMD                    = 0xD320, /* APP����������Ϣ */
    ID_IPS_TRACE_MNTN_INFO                  = 0xD321, /* TCP/IPЭ��ջ������Ϣ */
    ID_IPS_TRACE_BRIDGE_PRE_ROUTING_INFO    = 0xD322,
    ID_IPS_TRACE_BRIDGE_POST_ROUTING_INFO   = 0xD323,
    ID_IPS_TRACE_BRIDGE_LOCAL_IN_INFO       = 0xD324,
    ID_IPS_TRACE_BRIDGE_LOCAL_OUT_INFO      = 0xD325,
    ID_IPS_TRACE_IP4_FORWARD_INFO           = 0xD326,
    ID_IPS_TRACE_IP4_LOCAL_IN_INFO          = 0xD327,
    ID_IPS_TRACE_IP4_LOCAL_OUT_INFO         = 0xD328,
    ID_IPS_TRACE_IP6_FORWARD_INFO           = 0xD329,
    ID_IPS_TRACE_IP6_LOCAL_IN_INFO          = 0xD32A,
    ID_IPS_TRACE_IP6_LOCAL_OUT_INFO         = 0xD32B,
    ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_START = 0xD32C,
    ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_STOP  = 0xD32D,
    ID_IPS_TRACE_ADS_UL                     = 0xD330,
    ID_IPS_TRACE_ADS_DL                     = 0xD331,

    /* CP�Ϲ���IP���ġ�PPP֮֡���ά�ⶼ����D5�� */
    /*
     * CP�ϵ�TCPЭ��ջ�Ŀ�ά�ɲ���Ϣ, �����D51��ͷ
     * ����IP���������ܱȽ϶�, ��Ԥ��һЩID�ռ�, D51~D55
     */
    ID_OM_IPS_CCORE_ADVANCED_TRACE_CONFIG_REQ = 0xD513, /* ����TCP/IPЭ��ջ �������� */
    ID_IPS_CCORE_OM_ADVANCED_TRACE_CONFIG_CNF = 0xD514, /* ����TCP/IPЭ��ջ ����ָʾ */
    ID_OM_IPS_CCORE_MNTN_INFO_CONFIG_REQ      = 0xD515, /* �����Բ���TCP/IPЭ��ջ ������Ϣ���� */
    ID_IPS_CCORE_OM_MNTN_INFO_CONFIG_CNF      = 0xD516, /* �����Բ���TCP/IPЭ��ջ ������Ϣ����ָʾ */
    ID_IPS_CCORE_TRACE_INPUT_DATA_INFO        = 0xD517, /* TCP/IP��ά�ɲ���ձ��� */
    ID_IPS_CCORE_TRACE_OUTPUT_DATA_INFO       = 0xD518, /* TCP/IP��ά�ɲⷢ�ͱ��� */
    ID_IPS_CCORE_TRACE_BRIDGE_DATA_INFO       = 0xD519, /* TCP/IP������ת���� */
    ID_IPS_CCORE_TRACE_RECV_ARP_PKT           = 0xD51A, /* TCP/IPЭ��ջ���յ�ARP���Ʊ��� */
    ID_IPS_CCORE_TRACE_SEND_ARP_PKT           = 0xD51B, /* TCP/IPЭ��ջ���͵�ARP���Ʊ��� */
    ID_IPS_CCORE_TRACE_RECV_DHCPC_PKT         = 0xD51C, /* TCP/IPЭ��ջ���յ�DHCP���Ʊ��� */
    ID_IPS_CCORE_TRACE_SEND_DHCPC_PKT         = 0xD51D, /* TCP/IPЭ��ջ���͵�DHCP���Ʊ��� */
    ID_IPS_CCORE_TRACE_RECV_DHCPS_PKT         = 0xD51E, /* TCP/IPЭ��ջ���յ�DHCP���Ʊ��� */
    ID_IPS_CCORE_TRACE_SEND_DHCPS_PKT         = 0xD51F, /* TCP/IPЭ��ջ���͵�DHCP���Ʊ��� */
    ID_IPS_CCORE_TRACE_APP_CMD                = 0xD520, /* APP����������Ϣ */
    ID_IPS_CCORE_TRACE_MNTN_INFO              = 0xD521, /* TCP/IPЭ��ջ������Ϣ */
    ID_IPS_MNTN_CCORE_IMS_NIC_INFO            = 0xD522, /* IMS��������ͳ����Ϣ */
    ID_IPS_CCORE_TRACE_NIC_DATA_IND           = 0xD523, /* TCP/IP��ά�ɲ���ձ��� */
    ID_IPS_CCORE_TRACE_NIC_DATA_REQ           = 0xD524, /* TCP/IP��ά�ɲⷢ�ͱ��� */

    /* CP�ϵ�PPPC�Ŀ�ά�ɲ���Ϣ, �����D56��ͷ */
    ID_OM_PPP_MNTN_CONFIG_REQ       = 0xD560, /* PPP�յ�om������Ϣ */
    ID_PPP_OM_MNTN_CONFIG_CNF       = 0xD561, /* PPPȷ���յ�om������Ϣ */
    ID_PPP_OM_MNTN_STATISTIC_INFO   = 0xD562, /* PPP����ά����Ϣ */
    ID_OM_PPP_MNTN_TRACE_CONFIG_REQ = 0xD563, /* PPP�յ�OM ����������Ϣ */

    ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_INFO     = 0xD5F1,
    ID_IMS_NIC_MNTN_TRACE_CONFIG_PROC_RSLT    = 0xD5F2,
    ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_DEL      = 0xD5F3,
    ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_DEL_RSLT = 0xD5F4,
    ID_IMS_NIC_MNTN_TRACE_RUN_CMD             = 0xD5F5,
    ID_IMS_NIC_MNTN_TRACE_DNS_CFG             = 0xD5F6,
    ID_IMS_NIC_MNTN_TRACE_DNS_DEL             = 0xD5F7,
    ID_IMS_NIC_MNTN_TRACE_LOG_MSG             = 0xD5F8,
    ID_IMS_NIC_MNTN_TRACE_TCPIP_INPUT_LOG     = 0xD5F9,

    TTF_MNTN_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT16 TTF_MntnMsgTypeUint16;

enum TTF_MntnRptCmd {
    TTF_MNTN_RPT_CMD_STOP  = 0x00, /* ֹͣ�ϱ� */
    TTF_MNTN_RPT_CMD_START = 0x01  /* ��ʼ�ϱ� */
};
typedef VOS_UINT8 TTF_MntnRptCmdUint8;

enum TTF_MntnRlpRptCmd {
    TTF_MNTN_RLP_RPT_CMD_STOP  = 0x00, /* ֹͣ�ϱ� */
    TTF_MNTN_RLP_RPT_CMD_START = 0x01, /* ��ʼ�ϱ� */
    TTF_MNTN_RLP_RPT_CMD_CLEAR = 0x02  /* ��� */
};
typedef VOS_UINT8 TTF_MntnRlpRptCmdUint8;

enum TTF_MntnCommRptCmd {
    TTF_MNTN_COMM_RPT_CMD_STOP  = 0x00, /* ֹͣ�ϱ� */
    TTF_MNTN_COMM_RPT_CMD_START = 0x01, /* ��ʼ�ϱ� */
    TTF_MNTN_COMM_RPT_CMD_CLEAR = 0x02  /* ��� */
};
typedef VOS_UINT8 TTF_MntnCommRptCmdUint8;

enum IPS_MntnInfoAction {
    IPS_MNTN_INFO_REPORT_STOP  = 0, /* ���������Բ���TCP/IPЭ��ջ������Ϣ */
    IPS_MNTN_INFO_REPORT_START = 1  /* �ر������Բ���TCP/IPЭ��ջ������Ϣ */
};
typedef VOS_UINT16 IPS_MntnInfoActionUint16;

enum IPS_MntnResultType {
    IPS_MNTN_RESULT_OK               = 0,
    IPS_MNTN_RESULT_START_TIMER_FAIL = 1,
    IPS_MNTN_RESULT_ALLOC_MEM_FAIL   = 2,
    IPS_MNTN_RESULT_ALLOC_MSG_FAIL   = 3,
    IPS_MNTN_RESULT_INPUT_PARAM_ERR  = 4,
    IPS_MNTN_RESULT_LOCAL_PARAM_ERR  = 5
};
typedef VOS_UINT32 IPS_MntnResultTypeUint32;

enum GTTF_MntnOmRptAction {
    GTTF_MNTN_OM_REPORT_STOP  = 0,
    GTTF_MNTN_OM_REPORT_START = 1
};

typedef VOS_UINT16 GTTF_MntnOmRptActionUint16;

enum GTTF_ResultType {
    GTTF_RESULT_OK               = 0,
    GTTF_RESULT_START_TIMER_FAIL = 1,
    GTTF_RESULT_ALLOC_MEM_FAIL   = 2,
    GTTF_RESULT_ALLOC_MSG_FAIL   = 3,
    GTTF_RESULT_INPUT_PARAM_ERR  = 4,
    GTTF_RESULT_LOCAL_PARAM_ERR  = 5
};
typedef VOS_UINT32 GTTF_ResultTypeUint32;

enum TOOL_GttfCfgSwitch {
    TOOL_GTTF_CFG_SWITCH_OFF = 0,
    TOOL_GTTF_CFG_SWITCH_ON  = 1
};
typedef VOS_UINT32 TOOL_GttfCfgSwitchUint32;

enum GRM_TbfMode {
    Non_Extend_TBF = 0,
    Extend_TBF     = 1
};
typedef VOS_UINT8 GRM_TbfModeUint8;

enum GRM_TbfRlcMode {
    AM = 0, /* RLC AM mode */
    UM = 1  /* RLC UM mode */
};
typedef VOS_UINT8 GRM_TbfRlcModeUint8;

enum GRM_TbfCountDownFlag {
    No_Start_CV = 0,
    Start_CV    = 1
};
typedef VOS_UINT8 GRM_TbfCountDownFlagUint8;

enum GRM_TbfTi {
    TLLI_Absent     = 0,
    TLLI_Not_Absent = 1
};
typedef VOS_UINT8 GRM_TbfTiUint8;

enum GRM_TbfFinalBlkRxed {
    Not_received = 0, /* The Final Block the current TBF is not received. */
    Received     = 1  /* The Final Block the current TBF is received. */
};
typedef VOS_UINT8 GRM_TbfFinalBlkRxedUint8;

enum GRM_TbfEdgeCodeScheme {
    CS_1    = 0x0000,
    CS_2    = 0x0001,
    CS_3    = 0x0002,
    CS_4    = 0x0003,
    MCS_1   = 0x1000,
    MCS_2   = 0x1001,
    MCS_3   = 0x1002,
    MCS_4   = 0x1003,
    MCS_5   = 0x1004,
    MCS_6   = 0x1005,
    MCS_7   = 0x1006,
    MCS_8   = 0x1007,
    MCS_9   = 0x1008,
    MCS_5_7 = 0x1009,
    MCS_6_9 = 0x100a
};
typedef VOS_UINT16 GRM_TbfEdgeCodeSchemeUint16;

enum GRM_TbfTestMode {
    Test_Mode_A_Infinite   = 0, /* Aģʽ��PDU�������� */
    Test_Mode_A_Normal     = 1, /* Aģʽ��PDU�������� */
    Test_Mode_B            = 2, /* Bģʽ */
    Test_Mode_SRB_LoopBack = 3  /* EGPRS С����ģʽ */
};
typedef VOS_UINT8 GRM_TbfTestModeUint8;

enum GRM_TbfTimeSlotFlag {
    Not_Existed = 0, /* This TS is not existed */
    Existed     = 1  /* This TS is existed */
};
typedef VOS_UINT8 GRM_TbfTimeSlotFlagUint8;

enum IPS_MntnCcoreAction {
    IPS_MNTN_CCORE_INFO_REPORT_STOP  = 0, /* ���������Բ���TCP/IPЭ��ջ������Ϣ */
    IPS_MNTN_CCORE_INFO_REPORT_START = 1  /* �ر������Բ���TCP/IPЭ��ջ������Ϣ */
};
typedef VOS_UINT16 IPS_MntnCcoreActionUint16;

enum IPS_MntnCcoreResultType {
    IPS_MNTN_CCORE_RESULT_OK               = 0,
    IPS_MNTN_CCORE_RESULT_START_TIMER_FAIL = 1,
    IPS_MNTN_CCORE_RESULT_ALLOC_MEM_FAIL   = 2,
    IPS_MNTN_CCORE_RESULT_ALLOC_MSG_FAIL   = 3,
    IPS_MNTN_CCORE_RESULT_INPUT_PARAM_ERR  = 4,
    IPS_MNTN_CCORE_RESULT_LOCAL_PARAM_ERR  = 5
};
typedef VOS_UINT32 IPS_MntnCcoreResultTypeUint32;

/* ���ڱ�ʶPHY-->MAC ID_WTTF_PHY_MAC_DATA_IND MAC-->PHY ID_WTTF_MAC_PHY_DATA_REQ */
enum WTTF_tracePhyMacSubAttrib { 
    WTTF_TRACE_PHY_MAC_NULL  = 0,
    WTTF_TRACE_CTRL_HEADER = 1,
    WTTF_TRACE_WHOLE_DATA  = 2
};
typedef VOS_UINT8 WTTF_tracePhyMacSubAttribUint8;

enum WTTF_TraceMsgSimpleAttrib {
    WTTF_TRACE_MSG_NO  = 0,
    WTTF_TRACE_MSG_YES = 1
};
typedef VOS_UINT8 WTTF_TraceMsgSimpleAttribUint8;

enum IPS_MntnCcoreTraceChosen {
    IPS_MNTN_CCORE_TRACE_NULL_CHOSEN             = 0, /* ����������Ϣ */
    IPS_MNTN_CCORE_TRACE_MSG_HEADER_CHOSEN       = 1, /* ������ͷ�� */
    IPS_MNTN_CCORE_TRACE_CONFIGURABLE_LEN_CHOSEN = 2, /* �������ò����� */
    IPS_MNTN_CCORE_TRACE_WHOLE_DATA_LEN_CHOSEN   = 3  /* ������ȫ������ */
};
typedef VOS_UINT32 IPS_MntnCcoreTraceChosenUint32;

enum IPS_MntnTraceChosen {
    IPS_MNTN_TRACE_NULL_CHOSEN             = 0, /* ����������Ϣ */
    IPS_MNTN_TRACE_MSG_HEADER_CHOSEN       = 1, /* ������ͷ�� */
    IPS_MNTN_TRACE_CONFIGURABLE_LEN_CHOSEN = 2, /* �������ò����� */
    IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN   = 3  /* ������ȫ������ */
};
typedef VOS_UINT32 IPS_MntnTraceChosenUint32;

enum TTF_MntnTraceChosen {
    TTF_MNTN_TRACE_NULL_CHOSEN             = 0, /* ����������Ϣ */
    TTF_MNTN_TRACE_MSG_HEADER_CHOSEN       = 1, /* ������ͷ�� */
    TTF_MNTN_TRACE_CONFIGURABLE_LEN_CHOSEN = 2, /* �������ò����� */
    TTF_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN   = 3  /* ������ȫ������ */
};
typedef VOS_UINT32 TTF_MntnTraceChosenUint32;

/* ˵��: pppά����Ϣ֪ͨ��� */
enum PPP_MntnOmResultType {
    PPP_MNTN_OM_RESULT_OK              = 0,
    PPP_MNTN_OM_RESULT_INPUT_PARAM_ERR = 1,
    PPP_MNTN_OM_RESULT_BUTT
};
typedef VOS_UINT8 PPP_MntnOmResultTypeUint8;

enum WTTF_RlcMntnRbChosen {
    WTTF_RLC_MNTN_RB_NULL_CHOSEN       = 0x0,        /* ȡ������ */
    WTTF_RLC_MNTN_RB_SIGNALLING_CHOSEN = 0x0000000F, /* ����RB */
    WTTF_RLC_MNTN_RB_PS_CHOSEN         = 0xFFFFFFF0, /* ҵ��RB */
    WTTF_RLC_MNTN_RB_ALL_CHOSEN        = 0xFFFFFFFF  /* ALL RB */
};
typedef VOS_UINT32 WTTF_RlcMntnRbChosenUint32;

/* ����VISP��־������ȼ� */
enum IPS_MntnCcoreTcpipLogLevel {
    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_CRITICAL = 0,
    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_ERROR,
    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_WARNING,
    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_INFO,
    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_DEBUG,

    IPS_MNTN_CCORE_TCPIP_LOG_LEVEL_BUIT
};
typedef VOS_UINT8 IPS_MntnCcoreTcpipLogLevelUint8;

/* ö��˵��: ������Ϣ״̬ */
enum TTF_SensitiveTraceStatus {
    TTF_SENSITIVE_TRACE_NOT_SENSITIVE_STATUS = 0x00,
    TTF_SENSITIVE_TRACE_IN_SENSITIVE_STATUS  = 0x01
};
typedef VOS_UINT32 TTF_SensitiveTraceStatusUint32;

typedef struct {
    VOS_MSG_HEADER

    VOS_UINT32 msgId;

    VOS_UINT16 originalId;
    VOS_UINT16 terminalId;
    VOS_UINT32 timeStamp;
    VOS_UINT32 sn;
} DIAG_TransMsgHeader;

/* Wģ��ά�ɲ���Ϣ */
typedef struct {
    WTTF_RlcMntnRbChosenUint32 config; /* ��32bit,�ӵ͵��߷ֱ����RB1~RB32,RB0Ĭ�Ϲ�ȡ  */
                                                  /* ����RB: 0x000F */
                                                  /* ҵ��RB: 0xFFF0 */
                                                  /* ALL RB: 0xFFFF */
                                                  /* ȡ������: 0x0000 */
    VOS_UINT16 tracePduSize;
    VOS_UINT16 traceCipherPduSize;
    VOS_UINT16 tracePduCnt;
    VOS_UINT8  rsv[2];
} WTTF_TraceMacRlcDataIndCfg;
typedef WTTF_TraceMacRlcDataIndCfg WTTF_TraceRlcMacDataReqCfg;

typedef struct {
    WTTF_tracePhyMacSubAttribUint8 config;
    VOS_UINT8                      rsv[3];
} WTTF_TracePhyMacDataIndCfg;

typedef WTTF_TracePhyMacDataIndCfg WTTF_TraceMacPhyDataReqCfg;
typedef WTTF_TracePhyMacDataIndCfg WTTF_TracePhyMacHsPduDataIndCfg;
typedef WTTF_TracePhyMacDataIndCfg WTTF_TraceUpaPhyDataReqCfg;

typedef struct {
    WTTF_TraceMsgSimpleAttribUint8 config;
    VOS_UINT8                      rsv[3];
} WTTF_TraceMsgSimpleCfg;

typedef struct {
    TTF_MntnTraceChosenUint32 choice;
    VOS_UINT32                traceDataLen; /* ��ѡ��TTF_MNTN_TRACE_CONFIGURABLE_LEN_CHOSENʱ����ֵ��Ч */
} WTTF_MntnTraceDataCfg;

typedef WTTF_MntnTraceDataCfg WTTF_TracePdcpRabmDataIndCfg;
typedef WTTF_MntnTraceDataCfg WTTF_TraceRabmPdcpDataReqCfg;

typedef struct {
    IPS_MntnTraceChosenUint32 choice;
    VOS_UINT32                traceDataLen; /* ��ѡ��TRACE_CONFIGURABLE_LEN_CHOSENʱ����ֵ��Ч */
} IPS_MntnTraceCfg;

typedef IPS_MntnTraceCfg IPS_MntnBridgeTraceCfg;
typedef IPS_MntnTraceCfg IPS_MntnInputTraceCfg;
typedef IPS_MntnTraceCfg IPS_MntnOutputTraceCfg;
typedef IPS_MntnTraceCfg IPS_MntnLocalTraceCfg;

typedef struct {
    IPS_MntnCcoreTraceChosenUint32 choice;
    VOS_UINT32                     traceDataLen; /* ��ѡ��TRACE_CONFIGURABLE_LEN_CHOSENʱ����ֵ��Ч */
} IPS_MntnCcoreTraceCfg;

typedef IPS_MntnCcoreTraceCfg IPS_MntnCcoreBridgeTraceCfg;
typedef IPS_MntnCcoreTraceCfg IPS_MntnCcoreInputTraceCfg;
typedef IPS_MntnCcoreTraceCfg IPS_MntnCcoreOutputTraceCfg;


/* ��Ӧ��Ϣ: ID_OM_WTTF_RLC_DATA_TRANS_INFO_REQ */
typedef struct {
    DIAG_TransMsgHeader            transHdr;
    TTF_MntnRptCmdUint8            command; /* ��ʼ��ֹͣ�ϱ� */
    WTTF_TraceMsgSimpleAttribUint8 rptAm;
    WTTF_TraceMsgSimpleAttribUint8 rptUm;
    WTTF_TraceMsgSimpleAttribUint8 rptTm;
    VOS_UINT32                     rptPeriod; /* ��1..5����λ����,�������� */
} OM_WttfRlcDataTransReq;

/* ��Ӧ��Ϣ: ID_OM_WTTF_MAC_DATA_INFO_REQ */
typedef struct {
    DIAG_TransMsgHeader transHdr;
    TTF_MntnRptCmdUint8 command; /* ��ʼ��ֹͣ�ϱ� */
    VOS_UINT8           rsv[3];
    VOS_UINT32          rptPeriod; /* ��1..5����λ����,�������� */
} OM_WttfMacDataReq;

/* ��Ӧ��Ϣ: ID_OM_WTTF_MAC_DATA_INFO_CNF */
typedef struct {
    DIAG_TransMsgHeader  transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} OM_WttfMacDataCnf;

/* ��Ӧ��Ϣ: ID_WTTF_OM_RLC_DATA_TRANS_INFO_CNF */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} WTTF_OmRlcDataTransCnf;

typedef WTTF_OmRlcDataTransCnf OM_WttfRlcPduSizeTransCnf;
typedef WTTF_OmRlcDataTransCnf OM_WttfUpaDataStatusGCfg;

typedef struct {
    WTTF_TraceMacRlcDataIndCfg traceMacRlcDataIndCfg;
    WTTF_TraceRlcMacDataReqCfg traceRlcMacDataReqCfg;

    WTTF_TracePhyMacDataIndCfg tracePhyMacDataIndCfg;
    WTTF_TraceMacPhyDataReqCfg traceMacPhyDataReqCfg;

    WTTF_TracePhyMacHsPduDataIndCfg tracePhyMacHsPduDataIndCfg;
    WTTF_TraceUpaPhyDataReqCfg      traceUpaPhyDataReqCfg;

    WTTF_TraceMsgSimpleCfg tracePhyMacInfoIndCfg;
    WTTF_TraceMsgSimpleCfg traceUpaStatusIndCfg;
    WTTF_TraceMsgSimpleCfg traceUpaSchedStausIndCfg;

    WTTF_TraceMsgSimpleCfg traceMacPhyAmrDataIndCfg;
    WTTF_TraceMsgSimpleCfg tracePhyRlcAmrDataReqCfg;

    WTTF_TracePdcpRabmDataIndCfg tracePdcpRabmDataIndCfg;
    WTTF_TraceRabmPdcpDataReqCfg traceRabmPdcpDataReqCfg;

    WTTF_TraceMsgSimpleCfg traceRlcMacIPDataCfg;
    WTTF_TraceMsgSimpleCfg traceMacRlcIPDataCfg;
} WTTF_TraceMsgCfg;

typedef struct {
    DIAG_TransMsgHeader transHdr;
    WTTF_TraceMsgCfg    advancedConfig;
} OM_WttfAdvancedTraceCfgReq;

/* ��Ӧ��Ϣ: ID_WTTF_OM_ADVANCED_TRACE_CONFIG_CNF */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} WTTF_OmAdvancedTraceCfgCnf;

/* ID_OM_WTTF_RLC_PDU_SIZE_TRANS_INFO_REQ */
typedef struct {
    DIAG_TransMsgHeader transHdr;
    TTF_MntnRptCmdUint8 command; /* ��ʼ��ֹͣ�ϱ� */
    VOS_UINT8           rsv[3];
    VOS_UINT32          rptPeriod; /* ��1..5����λ����,�������� */
} OM_WttfRlcPduSizeTransReq;

typedef struct {
    VOS_UINT16 pduSize;
    VOS_UINT8  rsv[2];
    VOS_UINT32 pduCount;
} WTTF_MntnRlcPdu;

typedef struct {
    VOS_UINT8          rbId;
    PS_BOOL_ENUM_UINT8 isExist;           /* PS_TRUE:��ʾ��RbId���ڲ���֧��RLC PDU SIZE�ɱ䡣 */
    VOS_UINT8          rlcSizeLevelCnt; /* ��Ч�ĵ�λ�ȼ� */
    VOS_UINT8          rsv[1];
    WTTF_MntnRlcPdu    pduSize[WTTF_MNTN_MAX_FLX_RLC_SIZE_LEVEL_CNT]; /* ��Ч��RLC SIZE,�±��ӦucPduSizeIdx */
} OM_WttfRlcPduSizeTrans;

/* �ṹ˵��: WTTF MNTN RLC Data Trans Info ���� */
typedef struct {
    VOS_UINT32                simuClock;
    OM_WttfRlcPduSizeTransReq omRlcPduSizeReq;
    OM_WttfRlcPduSizeTrans    pduSizeTrans[WTTF_MNTN_MAX_RB_ID + 1];
} WTTF_MntnRlcPduSizeTransObj;

/* ��Ӧ��Ϣ: ID_WTTF_OM_RLC_PDU_SIZE_TRANS_INFO_CNF */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} WTTF_OmRlcPduSizeTransCnf;

/* ��Ӧ��Ϣ:  ID_OM_WTTF_UPA_DATA_STATUS_G_INFO_REQ */
typedef OM_WttfRlcPduSizeTransReq TTF_OmWttfUpaDataStatusGReq;

/* ��Ӧ��Ϣ:  ID_WTTF_OM_UPA_DATA_STATUS_G_INFO_CNF */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} WTTF_OmUpaDataStatusGCnf;

typedef struct {
    VOS_UINT32 rlcResetNum; /* ����Reset���� */
    /*
     * ��������������ͷ��ᷢ��Reset
     * 1)"No_Discard after MaxDAT number of transmissions" is configured and VT(DAT) equals the value MaxDAT (see
     * subclause 9.7.3.4) 2)VT(MRW) equals the value MaxMRW; 3)A STATUS PDU or a piggybacked STATUS PDU including
     * "erroneous Sequence Number" is received (see clause 10)
     */

    VOS_UINT32 lowRlcPduTxBytes;  /* ��λ:���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs���ֽ��������а����²��ɵ����ݺͿ���PDU���ش���PDU��Reset PDU�� */
    VOS_UINT32 highRlcPduTxBytes; /* ��λ:���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs���ֽ��������а����²��ɵ����ݺͿ���PDU���ش���PDU��Reset PDU�� */

    VOS_UINT32 lowRlcSduTxBytes; /* ��λ:���͵�RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����ֶγ�PDU��SDUs���ֽ��� */
    VOS_UINT32 highRlcSduTxBytes; /* ��λ:���͵�RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����ֶγ�PDU��SDUs���ֽ��� */

    VOS_UINT32 curPrdRlcSduTxBytes; /* ��ǰ�ϱ������ڷ���RLC SDU�ֽ��� */
    VOS_UINT32 rlcDataPduTxNum; /* ���͵�����PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵ĵ�MAC����²���������PDUs���������в������ش�������PDU�� */
    VOS_UINT32 rlcDataPduRetxNum; /* �����ش�������PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ������ش���PDU������ͬһ��PDU�ش���Σ��򵱶���ۼӣ�,�²���������PDU���������͵�����PDU������ �����ش�������PDU���� */
    VOS_UINT32 rlcNackedPduNum;   /* ��ȷ�ϵ�PDU����:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ,�ܹ��ӶԶ��յ���ȷ�ϵ�PDU���� */
    VOS_UINT32 rlcCtrlPduTxNum; /* ���͵Ŀ���PDU����:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ,�ܹ����͵�MAC��Ŀ���PDU����(���а���״̬PDU��Reset��Reset Ack PDU) */
    VOS_UINT16 rlcLastTxPduSn;       /* [0..4095]���һ�η��͵�SNֵ:VtS-1 */
    VOS_UINT16 rlcLastAckInseqPduSn; /* [0..4095]���һ������ȷ�ϵ�SN:VtA */
    VOS_UINT16 rlcVacantTxWinSize;   /* [1..4095]ָVtMs-VtS */
    VOS_UINT8  rsv[2];
    VOS_UINT32 rlcBo;                  /* ʵ��BO */
    VOS_UINT32 rlcDataPdu3ReTxNum;     /* �ش������ﵽ���ε�����PDU���� */
    VOS_UINT32 rlcDataPdu4ReTxNum;     /* �ش������ﵽ�Ĵε�����PDU���� */
    VOS_UINT32 rlcDataPdu5ReTxNum;     /* �ش������ﵽ��ε�����PDU���� */
    VOS_UINT32 rlcDataPdu6ReTxNum;     /* �ش������ﵽ���ε�����PDU���� */
    VOS_UINT32 rlcDataPduMore7ReTxNum; /* �ش������ﵽ�ߴ����ϣ������ߴΣ�������PDU���� */
    VOS_UINT32 reserve[7];             /* RTSOck cache-line32�ֽڶ��룬��ֹα���� */
} WTTF_MntnRlcAmUlDataTrans;

typedef struct {
    VOS_UINT32 rlcResetNum;       /* ����Reset���� */
    VOS_UINT32 lowRlcPduRxBytes;  /* ��λ:���յ���RLC PDU �ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ��������а�������ȷ����������PDU�Ϳ���PDU���Լ��ظ����ջ��ڽ��մ��ڵ�����PDU��������CRC���󣩣���ָ����MAC���յ������� */
    VOS_UINT32 highRlcPduRxBytes; /* ��λ:���յ���RLC PDU �ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ��������а�������ȷ����������PDU�Ϳ���PDU���Լ��ظ����ջ��ڽ��մ��ڵ�����PDU��������CRC���󣩣���ָ����MAC���յ������� */

    VOS_UINT32 lowRlcSduRxBytes;    /* ��λ:���յ���SDU�ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������ɵ�SDU�ֽ��� */
    VOS_UINT32 highRlcSduRxBytes;   /* ��λ:���յ���SDU�ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������ɵ�SDU�ֽ��� */
    VOS_UINT32 curPrdRlcSduRxBytes; /* ��ǰ�ϱ������ڽ���RLC SDU�ֽ��� */
    VOS_UINT32 rlcCrcErrPduRxNum; /* Crc Error PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����������CRC�����PDU���� */
    VOS_UINT32 rlcInvalidDataPduRxNum; /* ��Ч����PDU����: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC������������Ч����PDU���������а����������������PDU���ظ����ջ��ڽ��մ��ڵ�����PDU�� */
    VOS_UINT32 rlcDataPduRxNum; /* ���յ�������PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ,�ܹ���MAC������������Ч����PDU���������в������ظ����ջ��������PDU�� */
    VOS_UINT32 rlcCtrlPduRxNum; /* ���յ��Ŀ���PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC���������Ŀ���PDU���������а����˽�������Ŀ���PDU�� */
    VOS_UINT32 rlcNackedPduNum;     /* ��ȷ�ϵ�PDU����:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ,�ܹ����͵��Զ˸�ȷ�ϵ�PDU���� */
    VOS_UINT16 rlcLastRxInseqPduSn; /* [0..4095]����һ��Ҫ���յ�SNֵ  */
    VOS_UINT16 rlcHighestPduSn;     /* [0..4095]����߽��յ�PDU��� */
    VOS_UINT32 rsv[4];          /* RTSOck cache-line32�ֽڶ��룬��ֹα���� */
} WTTF_MntnRlcAmDlDataTrans;

typedef struct {
    PS_BOOL_ENUM_UINT8        isExist; /* ��ʾ��ʵ���Ƿ���ڣ�0��ʾ�����ڣ�1��ʾ���� */
    VOS_UINT8                 rbId;
    VOS_UINT8                 rsv[2];
    WTTF_MntnRlcAmUlDataTrans rlcAmUlDataTrans;
    WTTF_MntnRlcAmDlDataTrans rlcAmDlDataTrans;
} WTTF_MntnRlcAmDataTrans;

typedef struct {
    VOS_UINT32 lowRlcPduTxBytes; /* ��λ: ���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs���ֽ��� */
    VOS_UINT32 highRlcPduTxBytes; /* ��λ: ���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs���ֽ��� */
    VOS_UINT32 lowRlcSduTxBytes; /* ��λ: ���͵�RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����ֶγ�PDU��SDUs���ֽ��� */
    VOS_UINT32 highRlcSduTxBytes; /* ��λ: ���͵�RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����ֶγ�PDU��SDUs���ֽ��� */
    VOS_UINT32 rlcPduTxNum;       /* ���͵�RLC PDU ������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs���� */
    VOS_UINT16 rlcLastTxPduSn; /* [0..4095]�����һ�η��͵�SNֵ:VtUs-1 */
    VOS_UINT8  rsv[2];
    VOS_UINT32 rlcBo; /* ʵ��BO */
} WTTF_MntnRlcUmUlDataTrans;

typedef struct {
    VOS_UINT32 lowRlcPduRxBytes; /* ��λ: ���յ�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ������������������PDU */
    VOS_UINT32 highRlcPduRxBytes; /* ��λ: ���յ�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ������������������PDU */
    VOS_UINT32 lowRlcSduRxBytes;  /* ��λ: ���յ���SDU�ֽ���:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������ɵ�SDU�ֽ��� */
    VOS_UINT32 highRlcSduRxBytes; /* ��λ: ���յ���SDU�ֽ���:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������ɵ�SDU�ֽ��� */
    VOS_UINT32 rlcCrcErrPduRxNum; /* Crc Error PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����������CRC�����PDU���� */
    VOS_UINT32 rlcInvalidPduRxNum; /* ��ЧPDU����: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC������������ЧPDU���������а�����Ų�������Li����ָʾǰһ��PDU�����PDU�� */
    VOS_UINT32 rlcValidPduRxNum; /* ��ЧPDU����: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC������������ЧPDU����(��CrcError,��invalid PDU���PDU) */
    VOS_UINT16 rlcLastRxInseqPduSn; /* [0..4095]�����һ�ν��յ���SNֵ:VrUs - 1 */
    VOS_UINT8  rsv[2];
} WTTF_MntnRlcUmDlDataTrans;

typedef struct {
    PS_BOOL_ENUM_UINT8        isExist; /* ��ʾ��ʵ���Ƿ���ڣ�0��ʾ�����ڣ�1��ʾ���� */
    VOS_UINT8                 rbId;
    VOS_UINT8                 rsv[2];
    WTTF_MntnRlcUmUlDataTrans rlcUmUlDataTrans;
    WTTF_MntnRlcUmDlDataTrans rlcUmDlDataTrans;
} WTTF_MntnRlcUmDataTrans;

typedef struct {
    VOS_UINT32 lowRlcSduTxBytes; /* ��λ: ���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC SDUs���ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 highRlcSduTxBytes; /* ��λ: ���͵�RLC PDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC SDUs���ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 lowRlcDiscardSduTxBytes; /* ��λ: Discard RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������RLC SDUs���ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 hightRlcDiscardSduTxBytes; /* ��λ: Discard RLC SDU�ֽ�����ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ�������RLC SDUs���ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 rlcPduTxNum; /* ���͵�RLC PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����͵�MAC���RLC PDUs�ĸ��� */
    VOS_UINT32 rsv[2];
} WTTF_MntnRlcTmUlDataTrans;

typedef struct {
    VOS_UINT32 lowRlcSduRxBytes; /* ��λ: ���յ���SDU�ֽ���:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����յ���SDU�ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 highRlcSduRxBytes; /* ��λ: ���յ���SDU�ֽ���:ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ����յ���SDU�ֽ���������8bit�İ�8����������ȡ�� */
    VOS_UINT32 lowRlcPduRxBytes;  /* ��λ:���յ���RLC PDU �ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ��������а�����CrcError��PDU������ָ����MAC���յ�������������8bit�İ�8����������ȡ�� */
    VOS_UINT32 highRlcPduRxBytes; /* ��λ:���յ���RLC PDU �ֽ���: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����յ���RLC PDUs���ֽ��������а�����CrcError��PDU������ָ����MAC���յ�������������8bit�İ�8����������ȡ�� */
    VOS_UINT32 rlcCrcErrPduRxNum; /* Crc Error PDU������ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC����������CRC�����PDU���� */
    VOS_UINT32 rlcValidPduRxNum; /* ��ЧPDU����: ͳ�Ƶ��ǴӲ�ѯʱ�̿�ʼ��ʱ���ܹ���MAC������������ЧPDU����(��CrcError���PDU) */
    VOS_UINT32 rsv[2]; /* RTSOck cache-line32�ֽڶ��룬��ֹα���� */
} WTTF_MntnRlcTmDlDataTrans;

typedef struct {
    PS_BOOL_ENUM_UINT8        isExist; /* ��ʾ��ʵ���Ƿ���ڣ�0��ʾ�����ڣ�1��ʾ���� */
    VOS_UINT8                 rbId;
    VOS_UINT8                 rsv[2];
    WTTF_MntnRlcTmUlDataTrans rlcTmUlDataTrans;
    WTTF_MntnRlcTmDlDataTrans rlcTmDlDataTrans;
} WTTF_MntnRlcTmDataTrans;

/* ��������: �������� */
enum WTTFMNTN_AmrType {
    WTTFMNTN_AMR_TYPE_AMR2  = 0,
    WTTFMNTN_AMR_TYPE_AMR   = 1,
    WTTFMNTN_AMR_TYPE_AMRWB = 2,
    WTTFMNTN_AMR_TYPE_BUTT
};
typedef VOS_UINT16 WTTFMNTN_AmrTypeUint16;

/* ��������: AMRխ���Ϳ������, �μ�3GPP TS 26.101 */
enum WTTFMNTN_AmrBandwidthType {
    WTTFMNTN_AMR_BANDWIDTH_TYPE_NB = 0,
    WTTFMNTN_AMR_BANDWIDTH_TYPE_WB = 1,
    WTTFMNTN_AMR_BANDWIDTH_TYPE_BUTT
};
typedef VOS_UINT16 WTTFMNTN_AmrBandwidthTypeUint16;

/* ��Ӧ��Ϣ:ID_WTTF_OM_RLC_DATA_TRANS_INFO_IND */
typedef struct {
    WTTFMNTN_AmrTypeUint16          amrType;      /* AMR ���� */
    WTTFMNTN_AmrBandwidthTypeUint16 codecType;    /* 0: NB;  1: WB */
    VOS_UINT16                      frameType;    /* AMR����֡���� ȡֵ��Χ:0-15 */
    VOS_UINT8                       rsv[2]; /* ����λ */
} WTTF_OmMacVocoder;

/* �ṹ˵��: ID_WTTF_OM_RLC_DATA_TRANS_INFO_IND�ϱ���ѯ���� */
typedef struct {
    VOS_UINT32              curPrdRlcSduTxRate; /* ��ǰ�ϱ������ڷ���RLC SDU�ֽ��� */
    VOS_UINT32              curPrdRlcSduRxRate; /* ��ǰ�ϱ������ڷ���RLC SDU�ֽ��� */
    WTTF_MntnRlcAmDataTrans rlcAmDataTrans[WTTF_MNTN_MAX_AM_RB_NUM];
    WTTF_MntnRlcUmDataTrans rlcUmDataTrans[WTTF_MNTN_MAX_UM_RB_NUM];
    WTTF_MntnRlcTmDataTrans rlcTmDataTrans[WTTF_MNTN_MAX_TM_RB_NUM];
} WTTF_OmRlcDataTransInd;

/* �ṹ˵��: WTTF MNTN RLC Data Trans Info ���� */
typedef struct {
    VOS_UINT32             simuClock;
    OM_WttfRlcDataTransReq omRlcReq;
    WTTF_OmRlcDataTransInd rlcOmInd;
} WTTF_MntnRlcDataTransObj;

/* �ṹ˵��: WTTF MNTN MAC Data Info ���� */
typedef struct {
    VOS_UINT32           simuClock;
    VOS_UINT32           rptPeriod; /* ��1..5����λ����,�������� */
    TTF_MntnRptCmdUint8  command;
    VOS_UINT8            rsv[3];                 /* ����λ */
    WTTF_OmMacVocoder rlcUlVocoder; /* ��������vocoder ֡���ͺ�֡�����ϱ�������֡���ͺ�֡������HIFI�ϱ� */
} WTTF_MntnMacDataObj;

/* ��Ӧ��Ϣ:  ID_OM_IPS_ADVANCED_TRACE_CONFIG_REQ */
typedef struct {
    IPS_MntnBridgeTraceCfg          bridgeArpTraceCfg;   /* ���ò���������Ϣ */
    IPS_MntnInputTraceCfg           preRoutingTraceCfg;  /* ���ò���TCP/IPЭ��ջ������Ϣ */
    IPS_MntnOutputTraceCfg          postRoutingTraceCfg; /* ���ò���TCP/IPЭ��ջ������Ϣ */
    IPS_MntnLocalTraceCfg           localTraceCfg;       /* ���ò���TCP/IPЭ��ջ������Ϣ */
    WTTF_TraceMsgSimpleAttribUint8  adsIpConfig;
    VOS_UINT8                       rsv[3]; /* ����λ */
} IPS_MntnTraceCfgReq;

/* ��Ӧ��Ϣ:  OM_IpsAdvancedTracCfgReq */
typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnTraceCfgReq ipsAdvanceCfgReq;
} OM_IpsAdvancedTraceCfgReq;

/* ��Ӧ��Ϣ:  ID_IPS_OM_ADVANCED_TRACE_CONFIG_CNF */
typedef struct {
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�ܣ�PS_SUCC��ʾ�ɹ���PS_FAIL��ʾʧ�� */
} IPS_MntnTraceCfgCnf;

typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnTraceCfgCnf ipsAdvanceCfgCnf;
} IPS_OmAdvancedTraceCfgCnf;

/* ��Ӧ��Ϣ:  ID_OM_IPS_MNTN_TRAFFIC_CTRL_REQ */
typedef struct {
    DIAG_TransMsgHeader diagHdr;
    VOS_UINT16          trafficCtrlOn;
    VOS_UINT16          speedKBps; /* ��λ��KB/s */
} OM_IpsMntnTrafficCtrlReq;

/* ��Ӧ��Ϣ:  ID_OM_IPS_MNTN_INFO_CONFIG_REQ */
typedef struct {
    IPS_MntnInfoActionUint16 command;
    VOS_UINT16               timeLen; /* ��λ���� */
} IPS_MntnCfgReq;

typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnCfgReq      ipsMntnCfgReq;
} OM_IpsMntnCfgReq;

/* ��Ӧ��Ϣ:  ID_IPS_OM_MNTN_INFO_CONFIG_CNF */
typedef struct {
    IPS_MntnResultTypeUint32 result;
    IPS_MntnInfoActionUint16 command;
    VOS_UINT8                rsv[2];
} IPS_MntnCfgCnf;

typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnCfgCnf      ipsMntnCfgCnf;
} IPS_OmMntnCfgCnf;

/* Gģ��ά�ɲ���Ϣ */

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    VOS_UINT32           tlli;     /* 0xffffffff:��Чֵ */
    GRM_TbfTestModeUint8 testMode; /* GPRS����ʱ��ʵ�ʹ���ģʽ, 0xff:��Чֵ */
    VOS_UINT8            tsv[1];
    VOS_UINT16           llcPduNum; /* aģʽ�´����ص�LLC PDU��Ŀ, 0xffff:��Чֵ */
} GRM_MntnCom;

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    GRM_TbfRlcModeUint8        ulRlcMode;     /* 0:GRLC_MODE_AM, 1:GRLC_MODE_UM, 0xff:��Чֵ */
    GRM_TbfCountDownFlagUint8   countdownFlag; /* �˱�־ֻ�ڷ���չ����TBF��ʹ�ã���ʾ:�Ƿ���뵹��������.VOS_YES:�����˵�����, VOS_NO:û�н���, 0xff:��Чֵ */
    GRM_TbfEdgeCodeSchemeUint16 ulCodeType; /* EDGE_CODE_SCHEME_BUTT: 0xffff ��Чֵ */
    VOS_UINT8                   ulTfi;      /* 0xff: ��Чֵ */
    GRM_TbfTiUint8              ti;         /* 1: ��ͻ���δ���, 0: ��ͻ������, 0xff: ��Чֵ */
    VOS_UINT8                   rsv[2];
} GRM_MntnUlTbf;

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    GRM_TbfModeUint8        ulTbfMode; /* 1:Extend ��չTBF, 0: no-extend ����չ, 0xff:��Чֵ */
    VOS_UINT8                bsCvMax;   /* Range: 0 to 15. 0xff:��Чֵ */
    GRM_TbfTimeSlotFlagUint8 ulTsFlag[GRM_MNTN_TS_TOTAL]; /* 0xff: ��Чֵ */
    VOS_UINT8                rsv[2];
    GRM_MntnUlTbf            tbf[GRLC_MNTN_MAX_TBF_NUM]; /* Tbf��Ϣ */
} GRM_MntnUlEntity;

/* ������Ϣ */

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    VOS_FLOAT afCs1Data;   /* Percentage occupied by each CS_1 time slot. */
    VOS_FLOAT afCs2Data;   /* Percentage occupied by each CS_2 time slot. */
    VOS_FLOAT afCs3Data;   /* Percentage occupied by each CS_3 time slot. */
    VOS_FLOAT afCs4Data;   /* Percentage occupied by each CS_4 time slot. */
    VOS_FLOAT afMcs1Data;  /* Percentage occupied by each MCS_1 time slot. */
    VOS_FLOAT afMcs2Data;  /* Percentage occupied by each MCS_2 time slot. */
    VOS_FLOAT afMcs3Data;  /* Percentage occupied by each MCS_3 time slot. */
    VOS_FLOAT afMcs4Data;  /* Percentage occupied by each MCS_4 time slot. */
    VOS_FLOAT afMcs5Data;  /* Percentage occupied by each MCS_5 time slot. */
    VOS_FLOAT afMcs6Data;  /* Percentage occupied by each MCS_6 time slot. */
    VOS_FLOAT afMcs7Data;  /* Percentage occupied by each MCS_7 time slot. */
    VOS_FLOAT afMcs8Data;  /* Percentage occupied by each MCS_8 time slot. */
    VOS_FLOAT afMcs9Data;  /* Percentage occupied by each MCS_9 time slot. */
    VOS_FLOAT afMcs57Data; /* Percentage occupied by each MCS_5_7 time slot. */
    VOS_FLOAT afMcs69Data; /* Percentage occupied by each MCS_6_9 time slot. */
    VOS_FLOAT afGmskData;  /* Percentage occupied by each GMSK time slot. */
    VOS_FLOAT af8PskData;  /* Percentage occupied by each 8PSK time slot. */
} GRM_MntnCodeModulatePercentage;

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    /*
     * AM:���ڿ�ά�ɲ�
     * UMģʽ:�����յĿ�ı��뷽ʽ���ṩ��UMģʽ�����ʹ�ã�
     * ��ʼֵΪEDGE_CODE_SCHEME_BUTT��������δ�յ���ȷ�����п�
     */
    GRM_TbfEdgeCodeSchemeUint16 edgeCodeScheme;   /* EDGE_CODE_SCHEME_BUTT: 0xffff ��Чֵ */
    GRM_TbfFinalBlkRxedUint8    finalBlkRxed; /* FBI=1�Ŀ��Ƿ��Ѿ����յ�, 0xff: ��Чֵ */
    GRM_TbfRlcModeUint8        dlRlcMode;    /* 0:AM, 1:UM, 0xff: ��Чֵ */
    VOS_UINT8                   dlTfi;        /* TFI value 0xff: ��Чֵ */
    VOS_UINT8                   rsv[3];
    GRM_MntnCodeModulatePercentage dlCodeType[GRM_MNTN_TS_TOTAL];
} GRM_MntnDlTbf;

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    GRM_TbfTimeSlotFlagUint8 dlTsFlag[GRM_MNTN_TS_TOTAL]; /* ����ʱ϶���� 0xff: ��Чֵ */
    GRM_MntnDlTbf            tbf[GRLC_MNTN_MAX_TBF_NUM];  /* ��ǰֻ֧�ֵ�TBF */
} GRM_MntnDlEntity;

/* �ϱ�OM������ṹ��Ϣ */

/* ��Ӧ��Ϣ:  GRM_MNTN_GRM_OM_INFO */
typedef struct {
    GRM_MntnCom      grmMntnComInfo; /* ������Ϣ */
    GRM_MntnUlEntity grmMntnUlInfo;  /* ������Ϣ */
    GRM_MntnDlEntity grmMntnDlInfo;  /* ������Ϣ */
} GRM_MntnGrmOm;

/* ��Ӧ��Ϣ:  ID_GRM_MNTN_OM_GRM_INFO_CFG_REQ */
typedef struct {
    DIAG_TransMsgHeader        diagHdr; /* ͸����Ϣͷ */
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT16                 rptPeriod; /* ��λ: s */
} GRM_MntnOmCfgReq;

/*
 * �ṹ��:  GRM_MntnOmCfgCnf
 * ��Ӧ��Ϣ:  ID_GRM_MNTN_GRM_OM_INFO_CFG_CNF
 */
typedef struct {
    DIAG_TransMsgHeader        diagHdr; /* ͸����Ϣͷ */
    GTTF_ResultTypeUint32      result;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT16                 rptPeriod; /* ��λ: s */
} GRM_MntnOmCfgCnf;

/* ��Ӧ��Ϣ:  ID_GRM_MNTN_OM_GRM_THROUGHPUT_CFG_REQ */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} GRM_MntnOmThroughputCfgReq;

/* ��Ӧ��Ϣ:  ID_GRM_MNTN_GRM_OM_THROUGHPUT_CFG_CNF */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    GTTF_ResultTypeUint32      result;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} GRM_MntnThroughputCfgCnf;

typedef WTTF_MntnTraceDataCfg GTTF_MntnLlGrmTraceCfg;
typedef WTTF_MntnTraceDataCfg GTTF_MntnSnLlTraceCfg;
typedef WTTF_MntnTraceDataCfg GTTF_MntnRabmSnTraceCfg;

typedef struct {
    GTTF_MntnLlGrmTraceCfg  llGrmCfg;
    GTTF_MntnSnLlTraceCfg   snLlCfg;
    GTTF_MntnRabmSnTraceCfg rabmSnCfg;
} GTTF_TraceMsgCfg;

/* ��Ӧ��Ϣ:  ID_GRM_MNTN_OM_GTTF_ADV_TRACE_SWITCH_REQ */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    TOOL_GttfCfgSwitchUint32 l2CfgSwitch; /* TRACE����0:�ر�1:�� */
    GTTF_TraceMsgCfg         advancedConfig;
} GRM_MntnOmGttfAdvTraceSwitchReq;

/* ��Ӧ��Ϣ:  ID_GRM_MNTN_GTTF_OM_ADV_TRACE_SWITCH_CNF */
typedef struct {
    DIAG_TransMsgHeader      transHdr;
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�� */
} GRM_MntnGttfOmAdvTraceSwitchCnf;

/* CP�Ϲ���IP���ġ�PPP֮֡���ά�� */
typedef struct {
    WTTF_TraceMsgSimpleAttribUint8  vispLogOutConfig;          /* VISP��־�ܿ��� */
    IPS_MntnCcoreTcpipLogLevelUint8 vispRunLogLevelConfig;     /* VISP RUN��־�������� */
    WTTF_TraceMsgSimpleAttribUint8  vispUnEncryptPktCapConfig; /* VISP�Ǽ��ܱ���ץ������ */
    WTTF_TraceMsgSimpleAttribUint8  vispEncryptPktCapConfig;   /* VISP���ܱ���ץ������ */
} IPS_MntnCcoreVispLogCfgReq;

/* ��Ӧ��Ϣ:  ID_OM_IPS_CCORE_ADVANCED_TRACE_CONFIG_REQ */
typedef struct {
    IPS_MntnCcoreBridgeTraceCfg bridgeTraceCfg; /* ���ò���������Ϣ */
    IPS_MntnCcoreInputTraceCfg  inputTraceCfg;  /* ���ò���TCP/IPЭ��ջ������Ϣ */
    IPS_MntnCcoreOutputTraceCfg outputTraceCfg; /* ���ò���TCP/IPЭ��ջ������Ϣ */
    IPS_MntnCcoreVispLogCfgReq  vispLogConfig;
} IPS_MntnCcoreTraceCfgReq;

typedef struct {
    DIAG_TransMsgHeader      diagHdr;
    IPS_MntnCcoreTraceCfgReq ipsCcoreCfgReq;
} OM_IpsCcoreAdvTraceCfgReq;

/* ��Ӧ��Ϣ:  ID_IPS_CCORE_OM_ADVANCED_TRACE_CONFIG_CNF */
typedef struct {
    PS_RSLT_CODE_ENUM_UINT32 result; /* ���ز�ѯ�ṹ�ɹ���ʧ�ܣ�PS_SUCC��ʾ�ɹ���PS_FAIL��ʾʧ�� */
} IPS_MntnCcoreTraceCfgCnf;

typedef struct {
    DIAG_TransMsgHeader      diagHdr;
    IPS_MntnCcoreTraceCfgCnf ipsCcoreCfgCnf;
} IPS_CcoreOmAdvTraceCfgCnf;

/* ��Ӧ��Ϣ:  ID_OM_IPS_CCORE_MNTN_INFO_CONFIG_REQ */
typedef struct {
    IPS_MntnCcoreActionUint16 command;
    VOS_UINT16                timeLen; /* ��λ���� */
} IPS_MntnCcoreCfgReq;

typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnCcoreCfgReq ipsCcoreMntnCfgReq;
} OM_IpsCcoreCfgReq;

/* ��Ӧ��Ϣ:  ID_IPS_CCORE_OM_MNTN_INFO_CONFIG_CNF */
typedef struct {
    IPS_MntnCcoreResultTypeUint32 result;
    IPS_MntnCcoreActionUint16     command;
    VOS_UINT8                     rsv[2];
} IPS_MntnCcoreCfgCnf;

typedef struct {
    DIAG_TransMsgHeader diagHdr;
    IPS_MntnCcoreCfgCnf ipsCcoreMntnCfgCnf;
} IPS_CcoreOmMntnCfgCnf;

/* ��Ӧ��Ϣ:  ID_SN_MNTN_OM_THROUGHPUT_CFG_REQ */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} SNDCP_MntnOmThroughputCfgReq;

/* ��Ӧ��Ϣ:  ID_SN_MNTN_OM_THROUGHPUT_CFG_CNF */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    VOS_UINT32                 result;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} SNDCP_MntnOmThroughputCfgCnf;

/*
 * SNDCP��ά�ɲ���Ϣʵ��
 * ��Ӧ��Ϣ:  ID_SN_MNTN_OM_THROUGHPUT_INFO
 */
typedef struct {
    VOS_UINT32 ulThroughput; /* ������������Ϣ */
    VOS_UINT32 dlThroughput; /* ������������Ϣ */
} SNDCP_MntnOmThroughputInfo;

/* ��Ӧ��Ϣ:  ID_LL_MNTN_OM_THROUGHPUT_CFG_REQ */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} LLC_MntnOmThroughputCfgReq;

/* ��Ӧ��Ϣ:  ID_LL_MNTN_OM_THROUGHPUT_CFG_CNF */
typedef struct {
    DIAG_TransMsgHeader        diagHdr;
    VOS_UINT32                 result;
    GTTF_MntnOmRptActionUint16 rptAction;
    VOS_UINT8                  rsv[2];
} LLC_MntnOmThroughputCfgCnf;

/*
 * LLC��ά�ɲ���Ϣʵ��
 * ��Ӧ��Ϣ:  ID_LL_MNTN_OM_THROUGHPUT_INFO
 */
typedef struct {
    VOS_UINT32 ulThroughput;       /* ������������Ϣ */
    VOS_UINT32 dlThroughput;       /* ������������Ϣ */
    VOS_UINT32 ulRetransPduNumber; /* ȷ��ģʽ�����ش�PDU���� */
    VOS_UINT32 ulTotalNumber;      /* ȷ��ģʽ���з����ܵ�PDU���� */
    VOS_UINT32 dlRetransPduNumber; /* ȷ��ģʽ�����ش�PDU���� */
    VOS_UINT32 dlTotalNumber;      /* ȷ��ģʽ���з����ܵ�PDU���� */
} LLC_MntnOmThroughputInfo;


/* ��Ӧ��Ϣ:  pppc�ӿ� */
typedef struct {
    VOS_UINT32 succCnt;
    VOS_UINT32 failCnt;
    VOS_UINT32 nameOrPwdAuthFailCnt;
    VOS_UINT32 invalidIpAddrCnt;
    VOS_UINT32 invalidDnsAddrCnt;
    VOS_UINT32 otherFail;
} PPP_MntnHrpdStatisticsReport;


/* ��Ӧ��Ϣ:  ID_OM_PPP_MNTN_CONFIG_REQ */
typedef struct {
    DIAG_TransMsgHeader transHdr;

    TTF_MntnCommRptCmdUint8 command; /* ��ʼ��ֹͣ����� */
    VOS_UINT8                        reserve[3];
} PPP_MntnOmConfigReq;

/*
 * ��Ӧ��Ϣ:  ID_PPP_OM_MNTN_STATISTIC_INFO
 * ˵��:  �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 * �ṹ˵��: 1X�����ŵ���ͳ��ֵ
 */
typedef struct {
    VOS_UINT16                                primId;
    VOS_UINT16                                toolId;
    PPP_MntnHrpdStatisticsReport pppcStatisticInfo;
} PPP_MntnOmStatisticsInd;

/* ��Ӧ��Ϣ:  ID_CTTF_OM_PPP_MNTN_CONFIG_CNF */
typedef struct {
    DIAG_TransMsgHeader diagHdr;
    PS_RSLT_CODE_ENUM_UINT32 rslt;   /* ���óɹ���ʧ�� */
    VOS_UINT32               reason; /* ������ʧ�ܵ�ԭ�� */
} PPP_MntnOmConfigCnf;


#ifndef COMM_ITT
extern VOS_VOID TTF_TraceModuleSet(VOS_UINT8* moduleId);
#endif
extern VOS_VOID WTTF_TraceModuleCfgReq(VOS_VOID* msg);
extern VOS_VOID WMAC_R99MailBoxReadForIpcHalt(MODEM_ID_ENUM_UINT16 modemId);
extern VOS_VOID WTTF_R99MailboxInitForCbt(MODEM_ID_ENUM_UINT16 modemId);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

