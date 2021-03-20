/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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

#ifndef __APP_RRC_INTERFACE_H__
#define __APP_RRC_INTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include  "lps_common.h"



#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/
#define RRC_APP_CELL_MAX_COUNT           (32)
#define RRC_APP_MAX_NUM_OF_MCC           (3)


/*typedef DT_CmdUint8 APP_CmdUint8;*/
/*typedef DT_ResultUint32 APP_ResultUint32;*/

/* RRC����������뵽MAC�յ�rar */
#define PS_OM_EST_PTL_SND_MACRA_TO_RCV_RAR_TIME      (11)           /* 11ms T4-T1 */
/* ��mac�յ�rar��������Ϣ3ʱ�� */
#define PS_OM_EST_PTL_RCV_RAR_TO_SND_MSG3_TIME       (3)            /* 2.5ms T5-T4*/
/* ������Ϣ3���յ�setup��Ϣ */
#define PS_OM_EST_PTL_SND_MSG3_TO_RCV_SETUP_TIME     (73)           /* 28.5ms+2*Ts1c  T6-T5*/
/* ����setup��ϢЭ��ʱ�� */
#define PS_OM_EST_PTL_SETUP_TO_CMPL_TIME             (3)             /* 3ms T7-T6*/
/* ����������뵽����setup cmplЭ��ʱ�� */
#define PS_OM_EST_PTL_RRC_SND_MACRA_TO_CMPL_TIME     (92)            /* 47.5ms+2*Ts1c--(50.5~92.5ms) T7-T1*/
/* �����û���ʱ��Э��ֵ */
#define PS_OM_EST_PTL_USER_PLANE_TIME                (106)            /* 61ms+2*Ts1c+Ts1u--66~106ms T13-T1*/

/* С������Э��ʱ�� */
#define PS_OM_REEST_PTL_SEARCH_CELL_TIME             (100)            /* <100ms */
/* ϵͳ��Ϣ����Э��ʱ�� */
#define PS_OM_REEST_PTL_RCV_SI_TIME                  (1280)           /* <1280ms */
/* ��⵽��·ʧ�ܵ�����С���Ϸ����������Э��ʱ�� */
#define PS_OM_REEST_PTL_RCV_REEST_IND_TO_MACRA       (1500)           /* <1500ms */

/*  ������л���ʼ������Э��ʱ�� */
#define PS_OM_HO_PTL_HO_REQ_TO_CNF                   (1)              /* <1ms T6-T5*/

/* ������������յ�Cnf��Э��ʱ�� */
#define PS_OM_HO_PTL_RRC_MACRA_REQ_TO_MACRA_CNF      (11)             /* 2.5 + 1+ 7.5ms T8-T7*/

/* �����л������Ϣ���յ�ȷ����ϢЭ��ʱ�� */
#define PS_OM_HO_PTL_SND_CMPL_TO_CMPL_CNF            (13)             /* 13ms T10-T9*/

/* �յ��л�������Ϣ�������������Э��ʱ�� */
#define PS_OM_HO_PTL_RCV_RBCFG_TO_MACRA_REQ          (50)             /* 50ms T7-T4*/

/* �յ��л���������Ϣ���ظ������û���ʱ��Э��ʱ�� */
#define PS_OM_HO_PTL_USER_PLANE_TIME                 (100)            /* 100ms T11-T4*/

/* �����б��б���������Ŀ�� */
#define RRC_APP_MAX_LIMITED_ITEM_COUNT         (32)
/* ����������Ϣ���ϱ�С��������*/
#define LRRC_LCSEL_INTRA_CELL_REPORT_NUM 4
#define LRRC_LCSEL_INTER_CELL_REPORT_NUM 4
#define LRRC_LCSEL_UTRAN_CELL_REPORT_NUM 4
#define LRRC_LCSEL_INTER_FREQ_REPORT_NUM 9

#define LRRC_SCELL_MAX_NUM 4
#define LRRC_INTRA_CELL_MAX_NUM 16
#define LRRC_INTER_CELL_MAX_NUM 16
#define LRRC_UTRAN_CELL_MAX_NUM 16
#define LRRC_GERAN_CELL_MAX_NUM 16


#define LRRC_ECELL_QRY_CELL_REPORT_NUM 9
#define LRRC_ECELL_QRY_NBRCELL_NUM 8
#define LRRC_ECELL_QRY_SELL_REPORT_NUM 4
#define RRC_ECELL_QRY_INVALID_UINT8        0XFF
#define RRC_ECELL_QRY_INVALID_UINT16       0XFFFF
#define RRC_ECELL_QRY_INVALID_UINT32       0XFFFFFFFF

/* UE֧�ֵ�UTRANƵ�����������Ŀ */
#define LRRC_TRRC_PHY_MAX_SUPPORT_CARRIER_NUM               9

#define LRRC_GURRC_GERAN_ARFCN_MAX_NUM                      32


/* ���BAND��Ϣ�������� */
#define LRRC_APP_MAX_BAND_IND_ARRAY_NUM                     (8)

#define LRRC_APP_MAX_MEAS_CELL_NUM                          (18)
#define LRRC_APP_MAX_GERAN_MEAS_CELL_NUM                          (32)


#define LRRC_APP_LWCLASH_MAX_SCELL_NUM                      (4)



/*****************************************************************************
  3 Massage Declare
*****************************************************************************/
/*****************************************************************************
 ö����    : APP_RRC_MsgId
 Э����  :
 ASN.1���� :
 ö��˵��  : RRC <-> APP �ӿ���ϢID
*****************************************************************************/
enum APP_RRC_MsgId
{
    /* RRC����APP��ԭ�� */
    ID_APP_RRC_TRANSPARENT_CMD_REQ      = (PS_MSG_ID_APP_TO_RRC_BASE + 0x00),   /* _H2ASN_MsgChoice APP_RRC_TRANSPARENT_CMD_REQ_STRU */
    ID_APP_RRC_CELL_MEAS_RPT_REQ        = (PS_MSG_ID_APP_TO_RRC_BASE + 0x01),   /* _H2ASN_MsgChoice APP_RRC_CellMeasRptReq */
    ID_APP_RRC_TIME_DELAY_RPT_REQ       = (PS_MSG_ID_APP_TO_RRC_BASE + 0x02),   /* _H2ASN_MsgChoice APP_RRC_TimeDelayRptReq */
    ID_APP_RRC_INQ_CAMP_CELL_INFO_REQ   = (PS_MSG_ID_APP_TO_RRC_BASE + 0x03),   /* _H2ASN_MsgChoice APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU */
    ID_APP_RRC_LOCK_WORK_INFO_REQ       = (PS_MSG_ID_APP_TO_RRC_BASE + 0x04),   /* _H2ASN_MsgChoice APP_RRC_LOCK_WORK_INFO_REQ_STRU */
    ID_APP_RRC_CSQ_REQ                  = (PS_MSG_ID_APP_TO_RRC_BASE + 0x05),   /* _H2ASN_MsgChoice APP_RRC_CSQ_REQ_STRU */
    ID_APP_RRC_PTL_STATE_QUERY_REQ      = (PS_MSG_ID_APP_TO_RRC_BASE + 0x06),   /* _H2ASN_MsgChoice APP_RRC_PTL_STATE_QUERY_REQ_STRU */
    ID_APP_RRC_CELL_INFO_QUERY_REQ      = (PS_MSG_ID_APP_TO_RRC_BASE + 0x07),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */
    ID_APP_RRC_LWCLASH_REQ              = (PS_MSG_ID_APP_TO_RRC_BASE + 0x08),   /* _H2ASN_MsgChoice APP_RRC_PTL_LWCLASH_REQ_STRU */

    ID_APP_RRC_SERVING_CELL_INFO_QUERY_REQ   = (PS_MSG_ID_APP_TO_RRC_BASE + 0x09),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */
    ID_APP_RRC_CSEL_INFO_QUERY_REQ           = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0a),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */
    ID_APP_RRC_UE_CAP_INFO_QUERY_REQ         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0b),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */
    ID_APP_RRC_AC_BARRING_INFO_QUERY_REQ     = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0c),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */
    ID_APP_RRC_DRX_INFO_QUERY_REQ            = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0d),   /* _H2ASN_MsgChoice APP_RRC_CELL_INFO_QUERY_REQ_STRU */


    ID_APP_RRC_NMR_REQ                  = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0e),   /* _H2ASN_MsgChoice APP_RRC_NMR_REQ_STRU */
    ID_APP_RRC_CELLID_REQ               = (PS_MSG_ID_APP_TO_RRC_BASE + 0x0f),   /* _H2ASN_MsgChoice APP_RRC_CELLID_REQ_STRU */


    ID_APP_LRRC_RESEL_OFFSET_CFG_NTF         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x10),   /* _H2ASN_MsgChoice APP_LRRC_RESEL_OFFSET_CFG_NTF_STRU  */
    ID_APP_LRRC_CON_TO_IDLE_NTF          = (PS_MSG_ID_APP_TO_RRC_BASE + 0x11),   /* _H2ASN_MsgChoice APP_LRRC_CON_TO_IDLE_NTF_STRU  */

    ID_APP_LRRC_FAST_DORM_CFG_NTF          = (PS_MSG_ID_APP_TO_RRC_BASE + 0x12),   /* _H2ASN_MsgChoice APP_LRRC_FAST_DORM_CFG_NTF_STRU  */

    ID_APP_LRRC_GET_NCELL_INFO_REQ      = (PS_MSG_ID_APP_TO_RRC_BASE + 0x13),   /* _H2ASN_MsgChoice APP_LRRC_GetNcellInfoReq  */

    ID_APP_LRRC_INQ_TCFG_TXPOWER_REQ    = (PS_MSG_ID_APP_TO_RRC_BASE + 0x14),   /* _H2ASN_MsgChoice APP_RRC_INQ_TCFG_TXPOWER_REQ_STRU */


    ID_APP_RRC_FORCE_HOANDCSEL_REQ        = (PS_MSG_ID_APP_TO_RRC_BASE + 0x15),/*_H2ASN_MsgChoice APP_RRC_FORCE_HOANDCSEL_REQ_STRU*/
    ID_APP_RRC_BARCELL_ACCESS_REQ        = (PS_MSG_ID_APP_TO_RRC_BASE + 0x16),/*_H2ASN_MsgChoice APP_RRC_BARCELL_ACCESS_REQ_STRU*/


    ID_APP_LRRC_SET_UE_REL_VERSION_REQ      = (PS_MSG_ID_APP_TO_RRC_BASE + 0x17), /* _H2ASN_MsgChoice APP_LRRC_SET_UE_REL_VERSION_REQ_STRU  */

    /* begin:add for ·���ں� */
    ID_DT_LRRC_MEAS_REPORT_REQ         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x18),
    ID_DT_LRRC_SYNC_REPORT_REQ         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x19),
    /* end:add for ·���ں� */


    ID_APP_LRRC_GET_UE_CAP_INFO_REQ    = (PS_MSG_ID_APP_TO_RRC_BASE + 0x1a),   /* _H2ASN_MsgChoice APP_LRRC_GET_UE_CAP_INFO_REQ_STRU */



    ID_APP_RRC_SCELL_INFO_REQ    = (PS_MSG_ID_APP_TO_RRC_BASE + 0x1b),   /* _H2ASN_MsgChoice APP_RRC_SCELL_INFO_REQ_STRU */



    ID_APP_RRC_QUERY_ECELL_INFO_REQ    = (PS_MSG_ID_APP_TO_RRC_BASE + 0x1c),  /* _H2ASN_MsgChoice APP_RRC_QUERY_ECELL_INFO_REQ_STRU */
    ID_APP_RRC_QUERY_SCELL_INFO_REQ   = (PS_MSG_ID_APP_TO_RRC_BASE + 0x1d),   /* _H2ASN_MsgChoice APP_RRC_QUERY_SCELL_INFO_REQ_STRU */


    ID_APP_LRRC_SET_TLPS_PRINT2LAYER_REQ    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x18),

    /* APP����RRC��ԭ�� */
    ID_RRC_APP_TRANSPARENT_CMD_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x00),   /* _H2ASN_MsgChoice RRC_APP_TRANSPARENT_CMD_CNF_STRU */
    ID_RRC_APP_TRANSPARENT_CMD_IND      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x01),   /* _H2ASN_MsgChoice NULL */
    ID_RRC_APP_CELL_MEAS_RPT_CNF        = (PS_MSG_ID_RRC_TO_APP_BASE + 0x02),   /* _H2ASN_MsgChoice RRC_APP_CellMeasRptCnf */
    ID_RRC_APP_CELL_MEAS_RPT_IND        = (PS_MSG_ID_RRC_TO_APP_BASE + 0x03),   /* _H2ASN_MsgChoice RRC_APP_CellMeasReportInd */
    ID_RRC_APP_TIME_DELAY_RPT_CNF       = (PS_MSG_ID_RRC_TO_APP_BASE + 0x04),   /* _H2ASN_MsgChoice RRC_APP_TimeDelayRptCnf */
    ID_RRC_APP_TIME_DELAY_RPT_IND       = (PS_MSG_ID_RRC_TO_APP_BASE + 0x05),   /* _H2ASN_MsgChoice RRC_APP_TIME_DELAY_RPT_IND_STRU */
    ID_RRC_APP_INQ_CAMP_CELL_INFO_CNF   = (PS_MSG_ID_RRC_TO_APP_BASE + 0x06),   /* _H2ASN_MsgChoice RRC_APP_INQ_CAMP_CELL_INFO_CNF_STRU */
    ID_RRC_APP_INQ_CAMP_CELL_INFO_IND   = (PS_MSG_ID_RRC_TO_APP_BASE + 0x07),   /* _H2ASN_MsgChoice RRC_APP_INQ_CAMP_CELL_INFO_IND_STRU */
    ID_RRC_APP_LOCK_WORK_INFO_CNF       = (PS_MSG_ID_RRC_TO_APP_BASE + 0x08),   /* _H2ASN_MsgChoice RRC_APP_LOCK_WORK_INFO_CNF_STRU */
    ID_RRC_APP_RPT_DBG_INFO_IND         = (PS_MSG_ID_RRC_TO_APP_BASE + 0x09),   /* _H2ASN_MsgChoice RRC_APP_DEBUG_INFO_STRU */
    ID_RRC_APP_CSQ_CNF                  = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0a),   /* _H2ASN_MsgChoice RRC_APP_CSQ_CNF_STRU */
    ID_RRC_APP_CSQ_IND                  = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0b),   /* _H2ASN_MsgChoice RRC_APP_CSQ_IND_STRU */
    ID_RRC_APP_PTL_STATE_QUERY_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0c),     /* _H2ASN_MsgChoice RRC_APP_PTL_STATE_QUERY_CNF_STRU */
    ID_RRC_APP_CELL_INFO_QUERY_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0e),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_LWCLASH_CNF              = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0f),   /* _H2ASN_MsgChoice RRC_APP_PTL_LWCLASH_IND_STRU */
    ID_RRC_APP_LWCLASH_IND              = (PS_MSG_ID_RRC_TO_APP_BASE + 0x10),   /* _H2ASN_MsgChoice RRC_APP_PTL_LWCLASH_CNF_STRU */
    ID_RRC_APP_SERVING_CELL_INFO_QUERY_CNF    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x11),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_SERVING_CELL_INFO_IND    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x12),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_CSEL_INFO_QUERY_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x13),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */

    ID_RRC_APP_UE_CAP_INFO_QUERY_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x15),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_AC_BARRING_INFO_QUERY_CNF  = (PS_MSG_ID_RRC_TO_APP_BASE + 0x17),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */

    ID_RRC_APP_DRX_INFO_QUERY_CNF         = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1b),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */

    ID_RRC_APP_NMR_CNF                    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1d),   /* _H2ASN_MsgChoice RRC_APP_NMR_CNF_STRU */
    ID_RRC_APP_CELLID_CNF                 = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1e),   /* _H2ASN_MsgChoice RRC_APP_CELLID_CNF_STRU */

    ID_RRC_APP_GET_NCELL_INFO_CNF       = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1f), /*_H2ASN_MsgChoice LRRC_APP_GetNcellInfoCnf*/
    ID_RRC_APP_MEASRPT_EVENTA1_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x25),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTA2_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x26),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTA3_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x27),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTA4_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x28),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTA5_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x29),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTA6_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2a),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTB1_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2b),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTB2_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2c),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTC1_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2d),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTC2_TRIG             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2e),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTB1_NR_TRIG          = (PS_MSG_ID_RRC_TO_APP_BASE + 0x2f),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_EVENTB2_NR_TRIG          = (PS_MSG_ID_RRC_TO_APP_BASE + 0x30),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_MEASRPT_PERIOD_TRIG              = (PS_MSG_ID_RRC_TO_APP_BASE + 0x31),               /* _H2ASN_MsgChoice LRRC_LCMM_MeasRptTrig */
    ID_RRC_APP_EXCHANGE_MEASID_LINK_MEASOBJ     = (PS_MSG_ID_RRC_TO_APP_BASE + 0x32), /* _H2ASN_MsgChoice LRRC_LCMM_ExchangeMeasIdLinkObj */
    ID_RRC_APP_RESEL_MEASEVALUATE               = (PS_MSG_ID_RRC_TO_APP_BASE + 0x33), /* _H2ASN_MsgChoice LRRC_CSEL_ReselMeasEvaluate */

    ID_LRRC_APP_SET_UE_REL_VERSION_CNF      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x40),/* _H2ASN_MsgChoice LRRC_APP_SET_UE_REL_VERSION_CNF_STRU  */

    ID_APP_RRC_FORCE_HOANDCSEL_CNF        = (PS_MSG_ID_RRC_TO_APP_BASE + 0x41),/* _H2ASN_MsgChoice RRC_APP_FORCE_HOANDCSEL_CNF_STRU */
    ID_APP_RRC_BARCELL_ACCESS_CNF        = (PS_MSG_ID_RRC_TO_APP_BASE + 0x42),/* _H2ASN_MsgChoice RRC_APP_BARCELL_ACCESS_CNF_STRU */

    ID_LRRC_APP_SET_TLPS_PRINT2LAYER_CNF = (PS_MSG_ID_RRC_TO_APP_BASE + 0x47),


    ID_LRRC_DT_MEAS_REPORT_CNF   = (PS_MSG_ID_RRC_TO_APP_BASE + 0x48),
    ID_LRRC_DT_SYNC_REPORT_CNF   = (PS_MSG_ID_RRC_TO_APP_BASE + 0x49),


    ID_LRRC_APP_GET_UE_CAP_INFO_CNF       = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4a), /* _H2ASN_MsgChoice LRRC_APP_GET_UE_CAP_INFO_CNF_STRU */
    ID_LRRC_APP_FGI_INFO_IND              = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4b), /* _H2ASN_MsgChoice LRRC_APP_FGI_INFO_IND_STRU */


    ID_RRC_APP_DBG_SYSINFO_IND           = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4c),  /* _H2ASN_MsgChoice  LRRC_APP_DBG_SYSINFO_IND_STRU */



    ID_APP_RRC_DEBUG_STUB_SET_3HOUR_REQ       = (PS_MSG_ID_APP_TO_RRC_BASE + 0x20),   /* _H2ASN_MsgChoice APP_RRC_DEBUG_STUB_SET_3HOUR_REQ_STRU */
    ID_APP_RRC_DEBUG_STUB_HO_FAIL_REQ         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x21),   /* _H2ASN_MsgChoice APP_RRC_DEBUG_STUB_HO_FAIL_REQ_STRU */
    ID_APP_RRC_DEBUG_STUB_REL_ALL_REQ         = (PS_MSG_ID_APP_TO_RRC_BASE + 0x22),   /* _H2ASN_MsgChoice APP_RRC_DEBUG_STUB_REL_ALL_REQ_STRU */
    ID_APP_RRC_DEBUG_STUB_SET_EVAL_MESS_REQ   = (PS_MSG_ID_APP_TO_RRC_BASE + 0x25),   /* _H2ASN_MsgChoice APP_RRC_DEBUG_STUB_Set_Evaluation_Message_Req */
    ID_APP_RRC_DEBUG_STUB_L2W_RESEL_LIMIT_REQ   = (PS_MSG_ID_APP_TO_RRC_BASE + 0x26),
    ID_RRC_APP_DEBUG_STUB_CMD_CNF             = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4d),   /* _H2ASN_MsgChoice RRC_APP_DEBUG_STUB_CMD_CNF_STRU */



    ID_RRC_APP_SCELL_INFO_CNF              = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4e), /* _H2ASN_MsgChoice RRC_APP_SCELL_INFO_CNF_STRU */


    ID_RRC_CLOUD_CA_INFO_IND               = (PS_MSG_ID_RRC_TO_APP_BASE + 0x4f),


    ID_RRC_APP_QUERY_ECELL_INFO_CNF         = (PS_MSG_ID_RRC_TO_APP_BASE + 0x51), /* _H2ASN_MsgChoice RRC_APP_QUERY_ECELL_INFO_CNF_STRU */
    ID_RRC_APP_QUERY_SCELL_INFO_CNF         = (PS_MSG_ID_RRC_TO_APP_BASE + 0x52), /* _H2ASN_MsgChoice RRC_APP_QUERY_SCELL_INFO_CNF_STRU */



    ID_RRC_PLMN_FEATURE_INFO_IND               = (PS_MSG_ID_RRC_TO_APP_BASE + 0x53),



    ID_APP_RRC_DEBUG_STUB_ADD_BAD_CELL_REQ     = (PS_MSG_ID_APP_TO_RRC_BASE + 0x23),
/* begin: add for B5000 ·�� */
#if (FEATURE_ON == FEATURE_LTE_DT)
    ID_LRRC_LPHY_DT_MEAS_IN_CONNECTED_IND           = (0x0988),
    ID_LRRC_LPHY_DT_INACTIVE_MEAS_IN_CONNECTED_IND  = (0x09b5),
    ID_LRRC_LPHY_DT_MEAS_IN_IDLE_IND                = (0x0986),
    ID_LRRC_LPHY_DT_INACTIVE_MEAS_IN_IDLE_IND       = (0x09b4),
    ID_LRRC_LPHY_DT_MEAS_INFO_IND                   = (0x09a6),
    ID_LRRC_LPHY_DT_RL_SETUP_REQ                    = (0x08c2),
    ID_RRC_APP_LTE_CSEL_INFO_IND                    = (0x0d14),
    ID_RRC_APP_UTRA_CSEL_INFO_IND                   = (0x0d19),
    ID_RRC_APP_GERAN_CSEL_INFO_IND                  = (0x0d1a),
    ID_RRC_APP_UE_CAP_INFO_IND                      = (0x0d16),
    ID_RRC_APP_AC_BARRING_INFO_IND                  = (0x0d18),
    ID_RRC_APP_PTL_STATE_CHANGE_IND                 = (0x0d0d),
    ID_RRC_APP_DRX_INFO_IND                         = (0x0d1c),
    ID_LRRC_DT_SYNC_INFO_IND                        = (0x0904),
    ID_LRRC_DT_OUT_OF_SYNC_INFO_IND                 = (0x090f),
    ID_RRC_APP_SERVING_CELL_CA_INFO_IND             = (0x8D12),
#else
    ID_LRRC_DT_MEAS_INFO_IND                        = (0x988),/* ���ݶ���ӿڣ�·�����Ind��ϢidΪ0x988������id_lrrc_lphy_conn_meas_ind */
    ID_LRRC_DT_SYNC_INFO_IND                        = (0x904),/* ���ݶ���ӿڣ�·�����Ind��ϢidΪ0x904������id_lrrc_lphy_sync_info_ind */
    ID_LRRC_DT_OUT_OF_SYNC_INFO_IND                 = (0x90f),/* ���ݶ���ӿڣ�·�����Ind��ϢidΪ0x90f������id_lrrc_lphy_out_of_sync_info_ind */
    ID_RRC_APP_SERVING_CELL_CA_INFO_IND             = (PS_MSG_ID_RRC_TO_DT_BASE + 0x12),
    ID_RRC_APP_LTE_CSEL_INFO_IND                    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x14),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_UTRA_CSEL_INFO_IND                   = (PS_MSG_ID_RRC_TO_APP_BASE + 0x19),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_GERAN_CSEL_INFO_IND                  = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1a),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_UE_CAP_INFO_IND                      = (PS_MSG_ID_RRC_TO_APP_BASE + 0x16),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_AC_BARRING_INFO_IND                  = (PS_MSG_ID_RRC_TO_APP_BASE + 0x18),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
    ID_RRC_APP_PTL_STATE_CHANGE_IND                 = (PS_MSG_ID_RRC_TO_APP_BASE + 0x0d),   /* _H2ASN_MsgChoice RRC_APP_PTL_STATE_CHANGE_IND_STRU */
    ID_RRC_APP_DRX_INFO_IND                         = (PS_MSG_ID_RRC_TO_APP_BASE + 0x1c),   /* _H2ASN_MsgChoice RRC_APP_CELL_INFO_QUERY_CNF_STRU */
#endif
/* end: add for B5000 ·�� */
    /* begin: add for B5000 ·�� */
    ID_APP_RRC_RL_SETUP_REQ                    = (PS_MSG_ID_APP_TO_RRC_BASE + 0x24),
    ID_RRC_APP_RL_SETUP_CNF                    = (PS_MSG_ID_RRC_TO_APP_BASE + 0x54),
    /* end: add for B5000 ·�� */

    ID_APP_RRC_MSG_ID_BUTT
};
typedef VOS_UINT32    APP_RRC_MsgIdUint32;

/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : DT_Cmd
 �ṹ˵��  : DT����ö��
*****************************************************************************/
enum DT_Cmd
{
    DT_CMD_STOP,        /*ֹͣ*/
    DT_CMD_START,       /*����*/
    DT_CMD_BUTT
};
typedef VOS_UINT8 DT_CmdUint8;

/*****************************************************************************
 �ṹ��    : DT_Result
 �ṹ˵��  : DT�������ö��
*****************************************************************************/
enum DT_Result
{
    DT_RESULT_SUCC = 0,
    DT_RESULT_FAIL,
    DT_RESULT_NOT_SUPPORT_MEASURE,                /*��ǰ��֧�ֵĲ���*/

    /**********����ģ��ֱ�������������Ҫ�Ľ��ֵ********/
    /*RRCģʽ�Ľ��ֵ����Begin*/
    DT_RESULT_RRC_BEGIN = 0x1000,

    /*RRCģʽ�Ľ��ֵ����End*/

    /*NASģʽ�Ľ��ֵ����Begin*/
    DT_RESULT_NAS_BEGIN                 = 0x2000,
    DT_RESULT_NAS_PLMN_LOCK             = 0x2001,
    DT_RESULT_NAS_PLMN_UNLOCK           = 0x2002,

    /*NASģʽ�Ľ��ֵ����End*/

    /*L2ģʽ�Ľ��ֵ����Begin*/
   DT_RESULT_L2_BEGIN = 0x3000,

    /*L2ģʽ�Ľ��ֵ����End*/

   /*RRUģʽ�Ľ��ֵ����Begin*/
   DT_RESULT_RRU_BEGIN = 0x4000,

   /*RRUģʽ�Ľ��ֵ����End*/

    DT_RESULT_BUTT
};
typedef VOS_UINT32 DT_ResultUint32;

typedef DT_CmdUint8       APP_CmdUint8;
typedef DT_ResultUint32   APP_ResultUint32;
/*****************************************************************************
 ö����    : APP_RRC_CellRptPeriod
 ö��˵��  :
*****************************************************************************/
enum APP_RRC_CellRptPeriod
{
    APP_RRC_CELL_REPORT_PERIOD_300MS = 0,
    APP_RRC_CELL_REPORT_PERIOD_600MS,
    APP_RRC_CELL_REPORT_PERIOD_900MS,
    APP_RRC_CELL_REPORT_PERIOD_1200MS,
    APP_RRC_CELL_REPORT_PERIOD_BUTT
};
typedef VOS_UINT8 APP_RRC_CellRptPeriodUint8;

/*****************************************************************************
 ö����    : APP_LatencyType
 ö��˵��  :
*****************************************************************************/
enum APP_LatencyType
{
    APP_LATENCY_EST =0,               /*���ӽ���ʱ��*/
    APP_LATENCY_HO,                   /*�л�ʱ��*/
    APP_LATENCY_REEST,                /*�ؽ�ʱ��*/
    APP_LATENCY_BUTT
};
typedef VOS_UINT8 APP_LatencyTypeUint8;

/*****************************************************************************
 ö����    : APP_RRC_LockWorkInfoType
 ö��˵��  : ����������
*****************************************************************************/
enum APP_RRC_LockWorkInfoType
{
    RRC_APP_LOCK_FREQPOINT,              /* Ƶ������ */
    RRC_APP_LOCK_FREQANDCELL,            /* Ƶ���С���������� */
    RRC_APP_LOCK_FREQBAND,               /* Ƶ������ */
    RRC_APP_LOCK_BUTT
};
typedef VOS_UINT8 APP_RRC_LockWorkInfoTypeUing8;
/*****************************************************************************
 ö����    : APP_DelayType
 ö��˵��  :
*****************************************************************************/
enum APP_DelayType
{
    APP_DELAY_CONTROL_PLANE =0,     /*������ʱ��*/
    APP_DELAY_USER_PLANE,           /*�û���ʱ��*/
    APP_DELAY_HO,                   /*�л�ʱ��*/
    APP_DELAY_BUTT
};
typedef VOS_UINT8 APP_DelayTypeUint8;
/*****************************************************************************
 ö����    : APP_CAMPED_FLAG_ENUM
 ö��˵��  :
*****************************************************************************/
enum APP_CampedFlag
{
    APP_CAMPED,                 /* ��פ�� */
    APP_NOT_CAMPED,             /* δפ�� */
    APP_CAMPED_BUTT
};
typedef VOS_UINT8 APP_CampedFlagUint8;
/*****************************************************************************
 ö����    : APP_DelayType
 ö��˵��  :
*****************************************************************************/
enum APP_StateFlag
{
    APP_STATE_NOT_IN_RANGE,                 /* ���ڳ�ͻ��Χ��,״̬2 */
    APP_STATE_IN_RANGE,                     /* �ڳ�ͻ��Χ��,״̬1*/
    APP_STATE_BUTT
};
typedef VOS_UINT8 APP_StateFlagUint8;


/*****************************************************************************
 ö����    : LRRC_CaCcNumInfo
 ö��˵��  :
*****************************************************************************/
enum LRRC_CaCcNumInfo
{
    LRRC_CA_CC_NUM_2,                             /* �������֧��CA CC����2CC */
    LRRC_CA_CC_NUM_3,                             /* �������֧��CA CC����3CC */
    LRRC_CA_CC_NUM_4,                             /* �������֧��CA CC����4CC */
    LRRC_CA_CC_NUM_BUTT
};
typedef VOS_UINT8 LRRC_CaCcNumUint8;


/*****************************************************************************
   5 STRUCT
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : APP_OM_Msg
 �ṹ˵��  : APP(��̨����)��OM��������Ϣ��
*****************************************************************************/
typedef struct
{
     VOS_MSG_HEADER                     /*VOSͷ
 */
     VOS_UINT32          msgId;
     APP_MSG_HEADER                     /*APPͷ
 */
     VOS_UINT8           para[4];    /* �������� */
}APP_OM_Msg;

/*****************************************************************************
 �ṹ��    : APP_PLMN_ID_STRU
 �ṹ˵��  :
    MCC, Mobile country code (aucPlmnId[0], aucPlmnId[1] bits 1 to 4)
    MNC, Mobile network code (aucPlmnId[2], aucPlmnId[1] bits 5 to 8).

    The coding of this field is the responsibility of each administration but BCD
    coding shall be used. The MNC shall consist of 2 or 3 digits. For PCS 1900 for NA,
    Federal regulation mandates that a 3-digit MNC shall be used. However a network
    operator may decide to use only two digits in the MNC over the radio interface.
    In this case, bits 5 to 8 of octet 4 shall be coded as "1111". Mobile equipment
    shall accept MNC coded in such a way.

    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2            |           MCC digit 1
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3            |           MCC digit 3
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2            |           MNC digit 1
    ---------------------------------------------------------------------------

    AT���
    at+cops=1,2,"mcc digit 1, mcc digit 2, mcc digit 3, mnc digit 1, mnc digit 2, mnc

digit 3",2 :

    e.g.
    at+cops=1,2,"789456",2 :
    --------------------------------------------------------------------------------
    (mcc digit 1)|(mcc digit 2)|(mcc digit 3)|(mnc digit 1)|(mnc digit 2)|(mnc digit 3)
    --------------------------------------------------------------------------------
       7         |     8       |      9      |     4       |      5      |     6
    --------------------------------------------------------------------------------

    ��aucPlmnId[3]�еĴ�Ÿ�ʽ:
    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2 = 8        |           MCC digit 1 = 7
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3 = 6        |           MCC digit 3 = 9
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2 = 5        |           MNC digit 1 = 4
    ---------------------------------------------------------------------------

*****************************************************************************/
typedef struct
{
    VOS_UINT8                           plmnId[3];
    VOS_UINT8                           rsv;
} APP_PlmnId;

/*****************************************************************************
 �ṹ��    : APP_RRC_CellMeasRptReq
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                        command;
    APP_RRC_CellRptPeriodUint8          period;
    VOS_UINT8                           reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
}APP_RRC_CellMeasRptReq;

/*****************************************************************************
 �ṹ��    : RRC_APP_CellMeasRptCnf
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}RRC_APP_CellMeasRptCnf;

/*****************************************************************************
 �ṹ��    :RRC_APP_CellMeasRslt
 �ṹ˵��  :С���������
*****************************************************************************/
typedef struct
{
    VOS_UINT16 cellId;
    VOS_INT16  rsrp;
    VOS_INT16  rsrq;
    VOS_INT16  rssi;
}RRC_APP_CellMeasRslt;

/*****************************************************************************
 �ṹ��    :RRC_APP_CellMeasReportInd
 �ṹ˵��  :RRCͨ����ԭ�������ϱ�С�����������������
            ��ǰ����ͬƵ����ʱ��RRC�ϱ�С���Ĳ�������������ڣ����ϱ����ݣ�
            �����ǰ������ͬƵ���������������֮������ͬƵ������RRC����֮ǰ��OM���ý����ϱ�С�����������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT8                           cellCnt;                              /* С������ */
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    RRC_APP_CellMeasRslt         cellMeasInd[RRC_APP_CELL_MAX_COUNT];
}RRC_APP_CellMeasReportInd;

/*****************************************************************************
 �ṹ��    : APP_RRC_TimeDelayRptReq
 �ṹ˵��  : Ҫ���ϱ�������ʱ�ӡ�������ʱ�ӣ�����RB�����Ŀ���ʱ�䣬���������ӽ�������RB1�����ɹ���ʱ�䡣
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                  command;
    APP_DelayTypeUint8           delayType;
    VOS_UINT8                           reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
}APP_RRC_TimeDelayRptReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_TimeDelayRptCnf
 �ṹ˵��  :��ԭ��APP_RRC_TIME_DELAY_RPT_REQ_STRU�Ļظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_DelayTypeUint8           delayType;
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    APP_ResultUint32              result;
}RRC_APP_TimeDelayRptCnf;

/*****************************************************************************
 �ṹ��    :APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU
 �ṹ˵��  :��ȡ��ǰפ��С����ID��Ƶ�����Ϣ�Ĳ�ѯ���󣬻���ֹͣ�ϱ�פ��С����Ϣ������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                  command;
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
}APP_RRC_InqCampCellInfoReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_INQ_CAMP_CELL_INFO_CNF_STRU
 �ṹ˵��  :��ԭ��APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU�Ļظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}RRC_APP_InqCampCellInfoCnf;

/*****************************************************************************
 �ṹ��    :RRC_APP_INQ_CAMP_CELL_INFO_IND_STRU
 �ṹ˵��  :�ϱ���ǰפ��С����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    PS_BOOL_ENUM_UINT8                  valueFlag;
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT16                          cellId;
    VOS_UINT16                          freqInfo;
}RRC_APP_InqCampCellInfoInd;

/*****************************************************************************
 �ṹ��    :APP_RRC_FREQ_BAND_STRU
 �ṹ˵��  :Ƶ����Χ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          lowBand;                              /* Ƶ������ */
    VOS_UINT16                          highBand;                             /* Ƶ������ */
}APP_RRC_FreqBand;

/*****************************************************************************
 �ṹ��    :APP_RRC_CELL_INFO_STRU
 �ṹ˵��  :С����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          cellId;                               /* С��ID */
    VOS_UINT16                          freqPoint;                            /* Ƶ����Ϣ */
}APP_RRC_CellInfo;

/*****************************************************************************
 �ṹ��    :APP_RRC_LOCK_INFO_STRU
 �ṹ˵��  :�����������Ϣ
*****************************************************************************/
typedef struct
{
    APP_RRC_LockWorkInfoTypeUing8  lockType;                         /* �������� */
    VOS_UINT8                               bandInd;
    union
    {
        VOS_UINT16                      freqPoint;                            /* ������Ƶ�� */
        APP_RRC_CellInfo          freqAndCell;                          /* ������С��ID��Ƶ�� */
        APP_RRC_FreqBand          freqBand;                             /* ����Ƶ���������� */
    }u;
}APP_RRC_LockInfo;

/*****************************************************************************
 �ṹ��    :APP_RRC_LOCK_WORK_INFO_REQ_STRU
 �ṹ˵��  :·���·�����������Ľṹ��
*****************************************************************************/
/*lint -save -e959*/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 msgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                        command;                              /* �������ǽ��� */
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    APP_RRC_LockInfo              lockInfo;
}APP_RRC_LockWorkInfoReq;
/*lint -restore*/

/*****************************************************************************
 �ṹ��    :RRC_APP_LOCK_WORK_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_LOCK_WORK_INFO_REQ���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32                    result;
}RRC_APP_LockWorkInfoCnf;


/*****************************************************************************
 �ṹ��    :APP_LRRC_GET_UE_CAP_INFO_REQ_STRU
 �ṹ˵��  :HIDS�·��Ļ�ȡ�ؼ�UE������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT8                           reserved[128]; /* �����ֶΣ�����Ϊ128,����4�ֽڶ��� */
}APP_LRRC_GetUeCapInfoReq;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UE_CAP_INFO_STRU
 �ṹ˵��  : LRRC��APP֮���ϱ���UE�����ؼ���Ԫ
*****************************************************************************/
typedef struct
{
    /* ulFGI01To32: APP������bitλ����: short DRX:FGI4, C-DRX:FGI5, L-W,PSHO:FGI8,
       L-G,PSHO:FGI9, CSFB w/measurements(REL9):FGI22, TTI Bundling:FGI28, SPS:FGI29 */
    VOS_UINT32                          fGI01To32;                            /* fgi bitλ�����λ�����λ��ӦFGI1��FGI32 */
    VOS_UINT32                          fGI33To64;                            /* fgi bitλ�����λ�����λ��ӦFGI33��FGI64 */
    VOS_UINT32                          fGI101To132;                          /* fgi bitλ�����λ�����λ��ӦFGI101��FGI132 */
    VOS_UINT32                          fGIReserved;                          /* fgi bit����λ������32bit�Ա���Э����չ */

    /* �����bit��ʼ������Ϊprofile0x0001,�Դ����ƣ����ֵΪ1����ʾ֧�֣�0��ʾ
       ��֧��;�ֱ��Ӧprofile0x0001,profile0x0002,profile0x0003,profile0x0004,
       profile0x0006,profile0x0101,profile0x0103,profile0x0104 */
    VOS_UINT16                          rOHC;

    VOS_UINT8                           category;
    LRRC_CaCcNumUint8                   caCCNum;                              /* ���֧�ֵ�CA CC�� */


    VOS_UINT32                          bandNum;                                   /* usBandNum ȡֵ��Χ[1,256] */
    VOS_UINT32                          bandInd[LRRC_APP_MAX_BAND_IND_ARRAY_NUM]; /* aulBandInd,ÿ��BIT��1:����֧�֣�0����֧�֣�
                                                                                        ulBandInd[0]�е�һ��BIT����BAND1,�Դ�����; */
    VOS_UINT32                          qam256[LRRC_APP_MAX_BAND_IND_ARRAY_NUM]; /* aul256Qam,ÿ��BIT��1:����֧�֣�0����֧�֣�
                                                                                        aul256Qam[0]�е�һ��BIT���Դ�����; */
    PS_BOOL_ENUM_UINT8                  ulMimo4Layer;                           /* �Ƿ�֧������4�� */
    PS_BOOL_ENUM_UINT8                  dlMimo4Layer;                           /* �Ƿ�֧������4�� */
    VOS_UINT8                           reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
}LRRC_APP_UeCapInfo;

/*****************************************************************************
 �ṹ��    :LRRC_APP_GET_UE_CAP_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_LRRC_GET_UE_CAP_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32                    result;
    LRRC_APP_UeCapInfo                  ueCapInfo;
    /* 32�ֽڣ�����ָʾ�����Ƿ���Indָʾ��Ŀǰ��Ҫ������չCA��ϣ���ΪCA���ռ�ÿռ�ܴ� */
    VOS_UINT8                           reserved[32];
}LRRC_APP_GetUeCapInfoCnf;


/*****************************************************************************
 �ṹ��    : APP_RRC_TRANSPARENT_CMD_REQ
 Э����  :
 ASN.1���� :
 �ṹ˵��  :͸������ͷ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 msgId;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT16 transCmdNum;
    VOS_UINT16 paraSize;
    VOS_UINT8  para[4];
}APP_RRC_TransparentCmdReq;


/*****************************************************************************
 �ṹ��    : RRC_APP_TRANSPARENT_CMD_CNF_STRU
 �ṹ˵��  : PS->OMT��͸������ִ�н�����ݽṹ(
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32          msgId;                            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT8           transCmdCnf[4];                 /*͸���������������ɱ�*/
}RRC_APP_TransparentCmdCnf;

/*****************************************************************************
 �ṹ��    :PS_APP_HO_LATENCY_DETAIL_STRU
 �ṹ˵��  :�л�ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          t4ulRrcRcvHoRecfg;
    VOS_UINT32                          rrcRcvNasRabmRsp;
    VOS_UINT32                          rrcRcvCmmHoCnf;
    VOS_UINT32                          rrcRcvStopPdcpCnf;
    VOS_UINT32                          t5ulRrcSndDspHoReq;
    VOS_UINT32                          t6ulRrcRcvDspHoCnf;
    VOS_UINT32                          t7ulRrcSndMacRaReq;
    VOS_UINT32                          t12ulMacSndPhyAccessReq;
    VOS_UINT32                          t13ulMacRcvRar;
    VOS_UINT32                          t8ulRrcRcvMacRaCnf;
    VOS_UINT32                          rrcSndSmcSecuCfg;
    VOS_UINT32                          t9ulRrcSndRecfgCmp;
    VOS_UINT32                          t10ulRrcRcvAmDataCnf;
    VOS_UINT32                          rrcRcvCqiSrsCnf;
    VOS_UINT32                          rrcRcvPdcpContineCnf;
    VOS_UINT32                          t11ulRrcRcvRabmStatusRsp;
}PS_APP_HoLatencyDetail;
/*****************************************************************************
 �ṹ��    :PS_APP_HO_COMP_DETAIL_STRU
 �ṹ˵��  :�л�ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          realTestT6T5;
    VOS_UINT32                          ptlT6T5;          /* 1ms */
    VOS_UINT32                          realTestT8T7;
    VOS_UINT32                          ptlT8T7;          /* 2.5 + 1+ 7.5ms */
    VOS_UINT32                          realTestT10T9;
    VOS_UINT32                          ptlT10T9;         /* 13ms */
    VOS_UINT32                          realTestT7T4;
    VOS_UINT32                          ptlT7T4;          /* 50ms */
    VOS_UINT32                          realTestT11T4;
    VOS_UINT32                          ptlT11T4;         /* 100ms */
}PS_APP_HoCompDetail;
/*****************************************************************************
 �ṹ��    :PS_APP_EST_LATENCY_DETAIL_STRU
 �ṹ˵��  :����ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          t1ulRrcSndMacRaReq;
    VOS_UINT32                          t2ulMacSndPhyAccessReq;
    VOS_UINT32                          t4ulMacRcvRar;
    VOS_UINT32                          rrcRcvMacRaCnf;
    VOS_UINT32                          t5ulMacSndMsg3;
    VOS_UINT32                          t6ulRrcRcvSetUp;
    VOS_UINT32                          t7ulRrcSndEstCmp;
    VOS_UINT32                          rrcRcvSecCmd;
    VOS_UINT32                          rrcSndSecCmdCmp;
    VOS_UINT32                          rrcRcvCapEnq;
    VOS_UINT32                          rrcSndCapInfo;
    VOS_UINT32                          rrcRcvRecfg;
    VOS_UINT32                          t13ulRrcSndRecfgCmp;
}PS_APP_EstLatencyDetail;
/*****************************************************************************
 �ṹ��    :PS_APP_EST_COMP_STRU
 �ṹ˵��  :����ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          realTestT4T1;
    VOS_UINT32                          ptlT4T1;              /* 5s + 6ms */
    VOS_UINT32                          realTestT5T4;
    VOS_UINT32                          ptlT5T4;              /* 2.5ms */
    VOS_UINT32                          testRealT6T5;
    VOS_UINT32                          ptlT6T5;              /* 28.5ms+2*Ts1c */
    VOS_UINT32                          realTestT7T6;
    VOS_UINT32                          ptlT7T6;              /* 3ms */
    VOS_UINT32                          reaTestlT7T1;
    VOS_UINT32                          ptlT7T1;              /* 47.5ms+2*Ts1c */
    VOS_UINT32                          reaTestlT13T1;
    VOS_UINT32                          ptlT13T1;             /* 61ms+2*Ts1c+Ts1u */
}PS_APP_EstCompDetail;
/*****************************************************************************
 �ṹ��    :PS_APP_REEST_LATENCY_DETAIL_STRU
 �ṹ˵��  :�ؽ�ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          t2ulRrcRcvReestInd;
    VOS_UINT32                          t3ulRrcSndCellSearchReq;
    VOS_UINT32                          t4ulRrcRcvCellSearchInd;
    VOS_UINT32                          t5ulRrcRcvSi;
    VOS_UINT32                          t6ulRrcSndCampReq;
    VOS_UINT32                          t7ulRrcRcvCampCnf;
    VOS_UINT32                          t10ulRrcSndMacRaReq;
    VOS_UINT32                          macSndPhyAccessReq;
    VOS_UINT32                          macRcvRar;
    VOS_UINT32                          rrcRcvMacRaCnf;
    VOS_UINT32                          macSndMsg3;
    VOS_UINT32                          rrcRcvReest;
    VOS_UINT32                          rrcSndReestCmpl;
}PS_APP_ReestLatencyDetail;

/*****************************************************************************
 �ṹ��    :PS_APP_REEST_COMP_DETAIL_STRU
 �ṹ˵��  :�ؽ�ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          realTestT4T3;
    VOS_UINT32                          ptlT4T3;              /* 100ms */
    VOS_UINT32                          realTestT5T4;
    VOS_UINT32                          ptlT5T4;              /* 1280ms */
    VOS_UINT32                          realTestT10T2;
    VOS_UINT32                          ptlT10T2;             /* 1500ms */

}PS_APP_ReestCompDetail;
/*****************************************************************************
 �ṹ��    :RRC_APP_TIME_LATENCY_RPT_IND_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  t7T4ulHoCPlaneRealLatency;
    VOS_UINT32                                  hoCPlanePtlLatency;
    VOS_UINT32                                  t11T4ulHoUPlaneRealLatency;
    VOS_UINT32                                  hoUPlanePtlLatency;
    PS_APP_HoLatencyDetail                      hoLatencyDetail;
    PS_APP_HoCompDetail                         hoCompDetail;
}PS_APP_HoLatency;

/*****************************************************************************
 �ṹ��    :PS_APP_REEST_LATENCY_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  t10T2ulReestRealLatency;
    VOS_UINT32                                  reestPtlLatency;
    PS_APP_ReestLatencyDetail                   reestLatencyDetail;
    PS_APP_ReestCompDetail                      reestCompDetail;
}PS_APP_ReestLatency;

/*****************************************************************************
 �ṹ��    :PS_APP_EST_LATENCY_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  t13T1ulEstUPlaneRealLatency;
    VOS_UINT32                                  estUPlanePtlLatency;
    VOS_UINT32                                  t7T1ulEstCPlaneRealLatency;
    VOS_UINT32                                  estCPlanePtlLatency;
    PS_APP_EstLatencyDetail                     estLatencyDetail;
    PS_APP_EstCompDetail                        estCompDetail;
}PS_APP_EstLatency;

/*****************************************************************************
 �ṹ��    :RRC_APP_TIME_LATENCY_RPT_IND_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;

    APP_LatencyTypeUint8         latencyType;
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
union
    {
        PS_APP_HoLatency             hoElapse;
        PS_APP_ReestLatency          reestElapse;
        PS_APP_EstLatency            estElapse;
    }u;
} RRC_APP_TimeDelayRptInd;

/*****************************************************************************
 �ṹ��    :APP_RRC_CSQ_REQ_STRU
 �ṹ˵��  :CSQ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT16                          swt;              /* 0: ֹͣ�����ϱ� 1: ��ѯRSSI��������IND�ϱ�  2: ���������ϱ�RSSI */
    VOS_UINT16                          period;
    VOS_UINT16                          ndb;              /* ��Χ 0-5 dbm*/
    VOS_UINT16                          ms;               /* ��Χ 1-20 s*/
}APP_RRC_CsqReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_CNF_STRU
 �ṹ˵��  :CSQ��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          ri;                /*RIֵ*/
    VOS_UINT16                          cqi[2];           /* CQI�������� */
    VOS_UINT8                           res[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */

}APP_RRC_CqiInfo;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_CNF_STRU
 �ṹ˵��  :CSQ��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
    VOS_INT16                           rsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           rsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_INT16                           rssi;              /* RSSI����ֵ */
    VOS_UINT16                          ber;              /* ������ */
    VOS_INT32                           sinr;              /* SINR  RS_SNR */
    APP_RRC_CqiInfo                   cqi;              /* CQI�������� */
}RRC_APP_CsqCnf;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_IND_STRU
 �ṹ˵��  :RRC�ϱ���APP������CSQ��ѯָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_INT16                           rsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           rsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_INT16                           rssi;              /* RSSI����ֵ */
    VOS_UINT16                          ber;              /* ������ */
    VOS_INT32                           sinr;              /* SINR  RS_SNR */
    APP_RRC_CqiInfo               cqi;              /* CQI�������� */
}RRC_APP_CsqInd;

/*****************************************************************************
 �ṹ��    :APP_RRC_PTL_STATE_QUERY_REQ_STRU
 �ṹ˵��  :Э��״̬��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                  command;
    APP_RRC_CellRptPeriodUint8  period;
    VOS_UINT8                           reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */

}APP_RRC_PtlStateQueryReq;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_QUERY_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;             /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    APP_ResultUint32              result;
}RRC_APP_PtlStateQueryCnf;


/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_CHANGE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    VOS_UINT32                          currentState;     /* RRCЭ��״̬, 0:��ʾIDLE̬ 1:��ʾCONNECTED 2:��ʾЭ��״̬��Ч */
}RRC_APP_PtlStateChangeInd;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_CHANGE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT32                          currentState;     /* RRCЭ��״̬, 0:��ʾIDLE̬ 1:��ʾCONNECTED 2:��ʾЭ��״̬��Ч */
}RRC_APP_DtPtlStateInd;



/*****************************************************************************
 �ṹ��    :APP_RRC_PTL_STATE_QUERY_REQ_STRU
 �ṹ˵��  :С��������Ϣ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
}APP_RRC_CellInfoQueryReq;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_QUERY_CNF_STRU
 �ṹ˵��  : RRC�ظ���С��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT16                          freq;             /* 0xFFFFΪ��Ч,��λ��100KHz */
    VOS_UINT16                          pci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_UINT8                           dlBandWidth;      /* 0xffΪ��Ч,��Χ:(0,5): (0 : 1.4M , 1 : 3M , 2 : 5M ,3 : 10M , 4 : 15M, 5 : 20M) */
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
}RRC_APP_CellInfoQueryCnf;


typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_CmdUint8                  command;
    APP_RRC_CellRptPeriodUint8  period;
    VOS_UINT8                           reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
}APP_RRC_InqCmdReq;

typedef APP_RRC_InqCmdReq APP_RRC_InqServingCellInfoReq;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}APP_RRC_InqCmdCnfHeader;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
}APP_RRC_InqCmdIndHeader;


typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}RRC_APP_ForceHoandcselCnf;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 msgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    DT_CmdUint8                   cmd;              /* FORCE HO/CSEL FLAG */
    VOS_UINT8                           rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT32                          pci;              /* PHY Cell ID */
}APP_RRC_ForceHoandcselReq;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}RRC_APP_BarcellAccessCnf;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 msgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    DT_CmdUint8                   cmd;              /* Bar Cell Access FLAG */
    VOS_UINT8                           rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
}APP_RRC_BarcellAccessReq;



/*****************************************************************************
 �ṹ��    : APP_RRC_MSG_DATA
 Э����  :
 ASN.1���� :
 �ṹ˵��  : APP_RRC_MSG_DATA���ݽṹ,����ΪRRCר��
*****************************************************************************/
typedef struct
{
    APP_RRC_MsgIdUint32          msgID;        /*_H2ASN_MsgChoice_Export APP_RRC_MsgIdUint32*/
    VOS_UINT8                           msg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          APP_RRC_MsgIdUint32
    ****************************************************************************/
}APP_RRC_MsgData;

/*_H2ASN_Length UINT32*/

/*****************************************************************************
 �ṹ��    : App_Rrc_InterfaceMsg
 Э����  :
 ASN.1���� :
 �ṹ˵��  : AppRrcInterface_MSG���ݽṹ,����ΪRRCר��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    APP_RRC_MsgData                    msgData;
}App_Rrc_InterfaceMsg;
/*********************************************************
 ö����    : LRRC_LPHY_LTE_BAND_WIDTH_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  :
**********************************************************/
enum RRC_APP_BandWidth
{
    RRC_APP_LTE_BAND_WIDTH_1D4M = 0,
    RRC_APP_LTE_BAND_WIDTH_3M,
    RRC_APP_LTE_BAND_WIDTH_5M,
    RRC_APP_LTE_BAND_WIDTH_10M,
    RRC_APP_LTE_BAND_WIDTH_15M,
    RRC_APP_LTE_BAND_WIDTH_20M,
    RRC_APP_LTE_BAND_WIDTH_BUTT
};
typedef VOS_UINT16 RRC_APP_BandWidthUint16;

/*****************************************************************************
 �ṹ��    : APP_RRC_LWCLASH_REQ_STRU
 �ṹ˵��  :Э��״̬��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
} APP_RRC_LwclashReq;

/*********************************************************
 ö����    : APP_RRC_ANTENNA_MAX_LAYERS_MIMO_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  :
**********************************************************/
enum APP_RRC_AntennaMaxLayersMimo
{
    APP_RRC_ANTENNA_TWO_LAYERS      = 0,      /* UE MIMO����2�� */
    APP_RRC_ANTENNA_FOUR_LAYERS,              /* UE MIMO����4�� */
    APP_RRC_ANTENNA_EIGHT_LAYERS,             /* UE MIMO����8�� */
};
typedef VOS_UINT8    APP_RRC_AntennaMaxLayersMimoUint8;

/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_PARA_STRU
�ṹ˵��  : RRC�ϱ�����Ϣ
*****************************************************************************/
typedef struct
{

    VOS_UINT32                              ulFreq;          /*��������Ƶ�� ��λ:100Khz*/
    VOS_UINT32                              dlFreq;          /*��������Ƶ�� ��λ:100Khz*/

    RRC_APP_BandWidthUint16          ulBandwidth;     /*���д��� */
    RRC_APP_BandWidthUint16          dlBandwidth;     /*���д��� */
    APP_CampedFlagUint8              camped;          /*�Ƿ�פ�� */
    APP_StateFlagUint8               state;           /*�Ƿ�Ϊ��ͻ״̬ */
    VOS_UINT8                               band;            /*Ƶ��ָʾ */
    APP_RRC_AntennaMaxLayersMimoUint8   dlMimo;
} RRC_APP_LwclashPara;


/*****************************************************************************
 �ṹ��    : RRC_APP_SCELL_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT32                              ulFreq;           /*��������Ƶ�� ��λ:100Khz*/
    VOS_UINT32                              dlFreq;           /*��������Ƶ�� ��λ:100Khz*/
    RRC_APP_BandWidthUint16          ulBandwidth;      /*���д��� */
    RRC_APP_BandWidthUint16          dlBandwidth;      /*���д��� */
    APP_RRC_AntennaMaxLayersMimoUint8   dlMimo;
    VOS_UINT8                               resv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
}RRC_APP_ScellInfo;



/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;         /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    RRC_APP_LwclashPara           lWClashPara;
    VOS_UINT32                          scellNum;
    RRC_APP_ScellInfo             scellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
    PS_BOOL_ENUM_UINT8                  dl256QamFlag;
    VOS_UINT8                           resv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} RRC_APP_LwclashCnf;
/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_IND_STRU
�ṹ˵��  : RRC�ϱ�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;        /*_H2ASN_Skip*/
    RRC_APP_LwclashPara           lWClashPara;
    VOS_UINT32                          scellNum;
    RRC_APP_ScellInfo             scellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
    PS_BOOL_ENUM_UINT8                  dl256QamFlag;
    VOS_UINT8                           resv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} RRC_APP_LwclashInd;

/*****************************************************************************
 �ṹ��    :APP_RRC_NMR_REQ_STRU
 �ṹ˵��  :NMR��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
}APP_RRC_NmrReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          mncNum;           /*ָʾ MNC ����*/
    VOS_UINT16                          mcc;
    VOS_UINT16                          mnc;
    VOS_UINT8                           res[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} RRC_APP_PlmnId;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    RRC_APP_PlmnId                plmnId;
    VOS_UINT32                          cellId;            /* ��Χ��(0,503) */
} RRC_APP_CellGlobalId;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    RRC_APP_CellGlobalId         cellGloId;        /* CellGlobalId */
    VOS_UINT16                          tac;
    VOS_UINT16                          pci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_INT16                           rsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           rsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_UINT16                          ta;               /* TAֵ*/
    VOS_UINT8                           res[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} RRC_APP_LteCellInfo;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CNF_STRU
 �ṹ˵��  :NMR��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
    RRC_APP_LteCellInfo          lteCelInfo;
}RRC_APP_NmrCnf;

/*****************************************************************************
 �ṹ��    :APP_RRC_CELLID_REQ_STRU
 �ṹ˵��  :С��������Ϣ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
}APP_RRC_CellidReq;

/*****************************************************************************
 �ṹ��    : RRC_APP_CELLID_CNF_STRU
 �ṹ˵��  : RRC�ظ���С��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
    APP_PlmnId                    plmnId;
    VOS_UINT32                          ci;               /* 0xFFFFΪ��Ч, */
    VOS_UINT16                          pci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_UINT16                          tac;             /* TAC */
}RRC_APP_CellidCnf;

/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : APP_LPS_MSG_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          msgId;      /*��ϢID*/
    VOS_UINT32                          size;       /*��Ϣ��ĳ���*/
    VOS_UINT8                           value[4];  /*��Ϣ����Ч���ݵ�ָ��*/
}APP_LPS_Msg;


/*****************************************************************************
 �ṹ��    : APP_LRRC_RESEL_OFFSET_CFG_NTF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER

    VOS_UINT32             opId;
    VOS_UINT32             flag;
}APP_LRRC_ReselOffsetCfgNtf;

/*****************************************************************************
 �ṹ��    : APP_LRRC_CON_TO_IDLE_NTF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32              opId;
    VOS_INT32              reserv;
}APP_LRRC_ConToIdleNtf;


/*****************************************************************************
 ö����    : LRRC_LPDCP_FAST_DORMANCY_CMD_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : ָʾPDCP������ֹͣFAST_DORMANCY
*****************************************************************************/
enum APP_LRRC_FastDorm
{
    APP_LRRC_FAST_DORMANCY_STOP         = 0,                                  /* ֹͣFAST_DORMANCY */
    APP_LRRC_FAST_DORMANCY_START,                                             /* ����FAST_DORMANCY */
    APP_LRRC_FAST_DORMANCY_BUTT
};
typedef VOS_UINT32 APP_LRRC_FastDormUint32;

/*****************************************************************************
 �ṹ��    : APP_LRRC_FAST_DORM_CFG_NTF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;

    APP_LRRC_FastDormUint32      flag;
    VOS_UINT32            timerLen;   /* unite: s */
}APP_LRRC_FastDormCfgNtf;



/*****************************************************************************
 �ṹ��    : ID_APP_LRRC_SET_UE_REL_VERSION_REQ
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT32              mode;     /* 2:LTE , 1:TDS */
    VOS_UINT32              version;  /* 9 - 14 */
}APP_LRRC_SetUeRelVersionReq;

/*****************************************************************************
 �ṹ��    : ID_LRRC_APP_SET_UE_REL_VERSION_CNF
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;         /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    APP_ResultUint32              result;
} LRRC_APP_SetUeRelVersionCnf;


/*****************************************************************************
 �ṹ��    :APP_RRC_SCELL_INFO_REQ_STRU
 �ṹ˵��  :AT�·��Ĳ�ѯSCELL��Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    VOS_UINT8                           reserved[4]; /* �����ֶΣ�����Ϊ4,����4�ֽڶ��� */
}APP_RRC_ScellInfoReq;

/*****************************************************************************
 �ṹ��    : RRC_SCELL_INFO_STRU
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT32  phyCellId;          /* scell������С��id */
    VOS_UINT32  scellUlFreq;        /* scell����Ƶ�� */
    VOS_UINT32  scellDlFreq;        /* scell����Ƶ�� */
    VOS_UINT32  scellUlFreqPoint;     /* scell����Ƶ��*/
    VOS_UINT32  scellDlFreqPoint;     /* scell����Ƶ��*/
    VOS_UINT8    scellUlBandWidth;   /* scell���д���*/
    VOS_UINT8    scellDlBandWidth;   /* scell���д���*/
    VOS_UINT8    freqBandIndicator;   /* scell Ƶ��*/
    VOS_UINT8    rsv;   /* ���� */
    VOS_INT16    rssi;
    VOS_INT16    rsrp;
    VOS_INT16    rsrq;
    VOS_UINT16  scellIndex;
}RRC_SCELL_Info;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                            msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                            opId;
    APP_ResultUint32      result;
    VOS_UINT32                            sCellCnt;
    RRC_SCELL_Info           sCellInfo[LRRC_SCELL_MAX_NUM];
}RRC_APP_ScellInfoCnf;

/*****************************************************************************
 �ṹ��    : LRRC_DAM_BAR_LIST_ITEM_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          freqInfo;
    VOS_UINT16                          cellId;
    VOS_UINT8                           bandInd;
    VOS_UINT8                           reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT32                          timeOutTimeInMs;
    VOS_UINT32                          remainTimeOutInMs;
}LRRC_DAM_BarListItem;

/*****************************************************************************
 �ṹ��    : LRRC_APP_DAM_BAR_LIST_DBG_INFO_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    VOS_UINT16                          itemCount;        /* Bar List����Ŀ���� */
    VOS_UINT16                          rev;              /* ���� */
    LRRC_DAM_BarListItem         damBarListItem[RRC_APP_MAX_LIMITED_ITEM_COUNT];
}LRRC_APP_DamBarListDbgInfoInd;
/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_SERVE_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          currReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                          bandInd;           /* Ƶ��ָʾ */
    VOS_UINT16                          freqInfo;          /* ����С��Ƶ�� */
    VOS_UINT16                          cellId;            /* ����С��ID */
    VOS_INT16                           rsrp;               /* RSRP����ֵ */
    VOS_INT16                           rsrq;               /* RSRQ����ֵ */
    VOS_INT16                           rssi;               /* RSSI����ֵ */
    VOS_INT16                           value;             /*����õ���SPֵ*/
    VOS_INT16                           squal;              /*����õ���SQֵ*/
    VOS_INT16                           qrxLevMin;          /*׼��RSRP��������*/
    VOS_INT16                           pcompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                           qqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                           sIntraSearchP;      /* ����ͬƵ������rsrp��ֵ���� */
    VOS_INT16                           sIntraSearchQ;      /* ����ͬƵ������rsrq��ֵ���� */
    VOS_INT16                           sNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                           sNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                           threshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                           threshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
} LRRC_APP_IdleServeInfo;


/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_INTRA_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          cellId;
    VOS_INT16                           sValue;            /*����õ���SPֵ*/
    VOS_INT16                           squal;             /*����õ���SQֵ*/
    VOS_INT16                           rsrp;              /* RSRP����ֵ */
    VOS_INT16                           rsrq;              /* RSRQ����ֵ */
    VOS_INT16                           rssi;              /* RSSI����ֵ */
}LRRC_APP_IdleCellMeasResult;

typedef struct
{
    VOS_UINT16                          currReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                          bandInd;           /* Ƶ��ָʾ */
    VOS_UINT16                          freqInfo;          /*Ƶ��*/
    VOS_UINT16                          totalCellNum;      /*С������*/
    VOS_UINT16                          detectedCellNum;   /*��⵽��С����Ŀ*/
    VOS_INT16                           qrxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                           pcompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                           qqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                           sIntraSearchP;      /* ����ͬƵ������rsrp��ֵ���� */
    VOS_INT16                           sIntraSearchQ;      /* ����ͬƵ������rsrq��ֵ���� */
    LRRC_APP_IdleCellMeasResult         intraCellMesRslt[LRRC_LCSEL_INTRA_CELL_REPORT_NUM];
}LRRC_APP_IdleIntraInfo;



/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_INTER_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              currReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              freqInfo;
    VOS_UINT16                              bandInd;           /* Ƶ��ָʾ */
    VOS_INT16                               threshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               threshLowQ;         /* ��ѡ����ʱʹ��R9���� */
    VOS_INT16                               threshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               threshHighQ;        /* ��ѡ����ʱʹ��R9���� */
    VOS_UINT16                              totalCellNum;      /*С������*/
    VOS_UINT16                              detectedCellNum;   /*��⵽��С����Ŀ*/
    LRRC_APP_IdleCellMeasResult             interCellMesRslt[LRRC_LCSEL_INTER_CELL_REPORT_NUM];/* SCELL���ڵ�һ����λ�� */
}LRRC_APP_IdleSingleFreqMeasResult;

typedef struct
{
    VOS_UINT16                                   interFreqNum;
    VOS_INT16                                    sNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                    sNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                    servValue;          /*����õ���SPֵ*/
    VOS_INT16                                    servqual;           /*����õ���SQֵ*/
    VOS_INT16                                    threshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                    threshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
    LRRC_APP_IdleSingleFreqMeasResult            interFreqMesRslt[LRRC_LCSEL_INTER_FREQ_REPORT_NUM];
}LRRC_APP_IdleInterInfo;

/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_UTRAN_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              primaryCode;
    VOS_INT16                               rscp;               /* ����1/8 */
    VOS_INT16                               ecn0;               /* ����1/8 */
    VOS_INT16                               sValue;             /*����õ���SPֵ*/
    VOS_INT16                               squal;              /*����õ���SQֵ*/
}LRRC_APP_UtranCellMeasResult;

typedef struct
{
    VOS_UINT16                              currReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              arfcn;
    VOS_INT16                               rssi;               /* ����1/8 */
    VOS_INT16                               qrxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                               pcompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                               qqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                               threshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               threshLowQ;         /* ��ѡ����ʱʹ��R9���� */
    VOS_INT16                               threshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               threshHighQ;        /* ��ѡ����ʱʹ��R9���� */
    VOS_UINT16                              cellNum;
    LRRC_APP_UtranCellMeasResult    utranCellMesRslt[LRRC_LCSEL_UTRAN_CELL_REPORT_NUM];
}LRRC_APP_UtranSignleFreqMeasResult;

typedef struct
{
    VOS_UINT16                                      arfcnNum;
    VOS_INT16                                       sNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                       sNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                       servValue;          /*����õ���SPֵ*/
    VOS_INT16                                       servqual;           /*����õ���SQֵ*/
    VOS_INT16                                       threshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                       threshServingLowQ;  /* ��ѡ����ʱ�S���R9���� */
    LRRC_APP_UtranSignleFreqMeasResult              utranFreqMesRslt[LRRC_TRRC_PHY_MAX_SUPPORT_CARRIER_NUM];
}LRRC_APP_IdleUtranInfo;

/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_UTRAN_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              currReselPrior;   /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              arfcn;
    VOS_UINT16                              bandInd;          /* Ƶ��ָʾ */
    VOS_INT16                               rssi;              /* ����1/8 */
    VOS_UINT16                              ncc;
    VOS_UINT16                              bcc;
    VOS_INT16                               svalue;             /*����õ���SPֵ*/
    VOS_INT16                               qrxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                               pcompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                               threshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               threshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
}LRRC_APP_GeranSignleFreqMeasResult;

typedef struct
{
    VOS_UINT16                                    arfcnNum;
    VOS_INT16                                     sNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                     sNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                     servValue;          /*����õ���SPֵ*/
    VOS_INT16                                     servqual;           /*����õ���SQֵ*/
    VOS_INT16                                     threshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                     threshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
    LRRC_APP_GeranSignleFreqMeasResult            geranFreqMesRslt[LRRC_GURRC_GERAN_ARFCN_MAX_NUM];
}LRRC_APP_IdleGeranInfo;


/*****************************************************************************
 �ṹ��    : LRRC_APP_CELL_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          cellId;
    VOS_UINT16                          freqInfo;
    VOS_INT16                           rsrp;              /* RSRP����ֵ */
    VOS_INT16                           rsrq;              /* RSRQ����ֵ */
    VOS_INT16                           rssi;              /* RSSI����ֵ */
} LRRC_APP_CellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UTRAN_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          arfcn;
    VOS_UINT16                          primaryCode;
    VOS_INT16                           rscp;               /* ����1/8 */
    VOS_INT16                           ecn0;               /* ����1/8 */
    VOS_INT16                           rssi;               /* ����1/8 */
    VOS_UINT16                          utranType;
} LRRC_APP_UtranCellRslt;
/*****************************************************************************
 ö����     :LRRC_GRR_BANDINDICATOR_ENUM
 Э����  :
 ASN.1����   :
 ö��˵�� : 2GС��Ƶ��ָʾ
*****************************************************************************/
enum LRRC_APP_GeranBandind
{
    DCS1800                          = 0,
    PCS1900                          = 1,

    LRRC_APP_GERAN_BANDIND_BUTT      = 0x7FFF
};
typedef VOS_UINT16    LRRC_APP_GeranBandindUint16;

/*****************************************************************************
 ö����    : RRC_APP_PROTOCOL_STATE
 Э����  :
 ASN.1���� :
 ö��˵��  : RRCЭ��״̬����
*****************************************************************************/
enum RRC_APP_ProtocolState
{
    RRC_MEAS_PROTOCOL_STATE_IDLE            = 0 ,
    RRC_MEAS_PROTOCOL_STATE_CONNECTED,
    RRC_MEAS_PROTOCOL_STATE_BUTT            = 0x7FFF
};
typedef VOS_UINT16 RRC_APP_ProtocolStateUint16;

/*****************************************************************************
 ö����     :LRRC_GRR_BANDINDICATOR_ENUM
 Э����  :
 ASN.1����   :
 ö��˵�� : 2GС��Ƶ��ָʾ
*****************************************************************************/
enum LRRC_GRR_BandInd
{
    LRRC_GRR_BANDINDICATOR_DCS1800                          = 0,
    LRRC_GRR_BANDINDICATOR_PCS1900                             ,

    LRRC_GRR_BANDINDICATOR_BUTT
};
typedef VOS_UINT16    LRRC_GRR_BandIndUint16;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UTRAN_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          arfcn;
    LRRC_APP_GeranBandindUint16         bandInd;          /* Ƶ��ָʾ */
    VOS_INT16                           rssi;              /* ����1/8 */
} LRRC_APP_GeranCellRslt;

/*****************************************************************************
 �ṹ��    : LRRC_APP_SERV_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    RRC_APP_ProtocolStateUint16 state;             /* ��ǰЭ��״̬ */
    LRRC_APP_CellMeasRslt       servCellRslt;      /* ����С���ϱ���� */
} LRRC_APP_ServMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_APP_SCELL_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CellMeasRslt       sCellRslt[LRRC_SCELL_MAX_NUM];          /* SCell�ϱ���� */
} LRRC_APP_ScellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_APP_INTRA_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CellMeasRslt       intraCellRslt[LRRC_INTRA_CELL_MAX_NUM]; /* ͬƵС���ϱ���� */
} LRRC_APP_IntraMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CellMeasRslt       interCellRslt[LRRC_INTER_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_InterMeasRslt;
/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_UtranCellRslt     utranCellRslt[LRRC_UTRAN_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_UtranMeasRslt;
/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_GeranCellRslt       geranCellRslt[LRRC_GERAN_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_GeranMeasRslt;

/* add for Conn Meas Filter Optimize begin */
/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_INFO_IND_TYPE_ENUM
 �ṹ˵��  : LRRC�ϱ��˲����������
*****************************************************************************/
enum LRRC_CONN_MeasFilterInfoIndType
{
    LRRC_CONN_MEAS_FILTER_EUTRA_INFO,    /* _H2ASN_MsgChoice LRRC_CONN_MEAS_FILTER_EUTRA_INFO_STRU */
    LRRC_CONN_MEAS_FILTER_GERA_INFO,    /* _H2ASN_MsgChoice LRRC_CONN_MEAS_FILTER_GERA_INFO_STRU */
    LRRC_CONN_MEAS_FILTER_UTRA_INFO,    /* _H2ASN_MsgChoice LRRC_CONN_MEAS_FILTER_UTRA_INFO_STRU */
    LRRC_CONN_MEAS_FILTER_CDMA_INFO,    /* _H2ASN_MsgChoice LRRC_CONN_MeasFilterCdmaInfo */
    LRRC_CONN_MEAS_FILTER_NR_INFO,      /* _H2ASN_MsgChoice LRRC_CONN_MeasFilterNrInfo */
    LRRC_CONN_MEAS_FILTER_INFO_IND_TYPE_BUTT
};

typedef VOS_UINT32 LRRC_CONN_MeasFilterInfoIndTypeUint32;
/*****************************************************************************
 �ṹ��    : LRRC_APP_MeasCellRslt
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC EUTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16         cellId;
    VOS_INT16          rsrp;
    VOS_INT16          rsrq;
    VOS_INT16          rssi;
    VOS_INT16          sinr;
    VOS_UINT16         rsv;
} LRRC_APP_EutraCellMeasRslt;
/*****************************************************************************
 �ṹ��    : LRRC_APP_SendFilterMeasRslt
 �ṹ˵��  : LRRC�ϱ��˲����С��������
*****************************************************************************/

enum LRRC_APP_SendFilterMeasRslt {
    LRRC_APP_PCELL_CELL_MEAS_RSLT = 0,
    LRRC_APP_PSCELL_CELL_MEAS_RSLT,
    LRRC_APP_OTHER_CELL_MEAS_RSLT,
    LRRC_APP_CELL_MEAS_RSLT_BUTT,
};
typedef VOS_UINT8 LRRC_APP_SendFilterMeasRsltUint8;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_EUTRA_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC EUTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          arfcn;
    VOS_UINT16                          freqInfo;
    VOS_UINT8                           bandInd;
    VOS_UINT8                           measCellNum;
    LRRC_APP_EutraCellMeasRslt          measCellList[LRRC_APP_MAX_MEAS_CELL_NUM];
}LRRC_CONN_MeasFilterEutraInfo;


/*****************************************************************************
 �ṹ��    :LRRC_GRR_BSIC_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :CELL BAIC Info��Э��36331 6.3.4
            usNcc(GSM Network Colour Code)  ������Χ:(0..7), 8 ��ʾ��Чֵ
            usBcc(GSM Base Station Colour Code)  ������Χ:(0..7) , 8 ��ʾ��Чֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                              ncc;
    VOS_UINT16                                              bcc;
}LRRC_GRR_BsicInfo;
/*****************************************************************************
 �ṹ��    : LRRC_APP_GeranCellMeasRslt
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC GERAN�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16                             arfcn;            /* Ƶ�� */
    LRRC_GRR_BandIndUint16                 bandInd;          /* Ƶ��ָʾ */
    PS_BOOL_ENUM_UINT8                     bsicVaild;    /* BSIC��Ч��־ */
    VOS_UINT8                              resv;         /* ����λ */
    VOS_INT16                              rssi;          /* �˲����Rssi */
    LRRC_GRR_BsicInfo                      bsic;         /* BSIC��־��Чʱ��BSIC��Ϣ */

} LRRC_APP_GeranCellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_GERA_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC GERAN�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                              arfcnNum;           /* Ƶ����� */
    VOS_UINT8                                               reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
    LRRC_APP_GeranCellMeasRslt                              geranCellResult[LRRC_APP_MAX_GERAN_MEAS_CELL_NUM];
}LRRC_CONN_MeasFilterGeraInfo;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_UTRA_TYPE_ENUM
 �ṹ˵��  : LRRC�ϱ��˲����������
*****************************************************************************/
enum LRRC_CONN_MeasUtraType
{
    LRRC_CONN_MEAS_FILTER_UTRA_FDD,    /*_H2ASN_Skip*/
    LRRC_CONN_MEAS_FILTER_UTRA_TDD,    /*_H2ASN_Skip*/
    LRRC_CONN_MEAS_FILTER_UTRA_TYPE_BUTT
};
typedef VOS_UINT8 LRRC_CONN_MeasUtraTypeUint8;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UtraCellMeasRslt
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC UTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16                cellId;        /* С��id */
    VOS_INT16                 rscp;          /* �˲����Rscp */
    VOS_INT16                 ecN0;          /* �˲����EcN0 */
    VOS_UINT16                rsv;
} LRRC_APP_UtraCellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_UTRA_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC FDD UTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              arfcn;
    LRRC_CONN_MeasUtraTypeUint8             utraType;             /* Utra������ */
    VOS_UINT8                               measCellNum;
    LRRC_APP_UtraCellMeasRslt               measCellList[LRRC_APP_MAX_MEAS_CELL_NUM];
}LRRC_CONN_MeasFilterUtraInfo;
/*****************************************************************************
 �ṹ��    : LRRC_APP_CdmaCellMeasRslt
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC CDMA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16         cellId;
    VOS_UINT16         pilotPhase;
    VOS_INT16          sPilotStrength;
    VOS_UINT16         reserved; /* 4�ֽڶ��뱣���ֽ� */
} LRRC_APP_CdmaCellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MeasFilterCdmaInfo
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC CDMA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16                        arfcn;
    VOS_UINT8                         bandClass;
    VOS_UINT8                         cdma2000Type;
    VOS_UINT32                        measCellNum;
    LRRC_APP_CdmaCellMeasRslt         measCellList[LRRC_APP_MAX_MEAS_CELL_NUM];
} LRRC_CONN_MeasFilterCdmaInfo;

/*****************************************************************************
 �ṹ��    : LRRC_APP_MeasCellRslt
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC Nr�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT16         cellId;
    VOS_INT16          rsrp;
    VOS_INT16          rsrq;
    VOS_INT16          sinr;
} LRRC_APP_NrCellMeasRslt;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MeasFilterNrInfo
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC NR�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct {
    VOS_UINT32                                arfcnNr;                                    /* NR��Ƶ����Ϣ */
    VOS_UINT32                                measCellNum;
    LRRC_APP_NrCellMeasRslt                   detectCellList[LRRC_APP_MAX_MEAS_CELL_NUM]; /* DetectС�� */
} LRRC_CONN_MeasFilterNrInfo;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_INFO_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : LRRC�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    LRRC_CONN_MeasFilterInfoIndTypeUint32       unionStructSelChoice;/*_H2ASN_Skip*/
    union  /* _H2ASN_Skip */
    {      /* _H2ASN_Skip */
        LRRC_CONN_MeasFilterEutraInfo       eutraConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MeasFilterGeraInfo        geraConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MeasFilterUtraInfo        utraConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MeasFilterCdmaInfo        cdmaConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MeasFilterNrInfo          nrConnMeasFilterInfo;     /* _H2ASN_Skip */
    }u;  /* _H2ASN_Skip */
}LRRC_CONN_MeasFilterInfoInd;
/* add for Conn Meas Filter Optimize end */

/*****************************************************************************
 �ṹ��    : APP_OM_MSG_REDF_STRU
 �ṹ˵��  : APP(��̨����)��OM��������Ϣ��(��APP_OM_MSG_STRU��ͬ)
*****************************************************************************/
typedef struct
{
     VOS_MSG_HEADER                     /*VOSͷ*/
     VOS_UINT32          msgId;
     APP_MSG_HEADER                     /*APPͷ*/
     VOS_UINT8           para[4];    /* �������� */
}APP_OM_MsgRedf;



/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_SET_3HOUR_REQ_STRU
 �ṹ˵��  :��׮3Сʱ��ʱ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      msgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      opId;
    VOS_UINT32                      timerLength;         /* 3Сʱ��ʱ��ʱ�� */
}APP_RRC_DebugStubSet3hourReq;
/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_HO_FAIL_REQ_STRU
 �ṹ˵��  :��׮�ؽ���
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      msgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      opId;
    VOS_UINT32                      hoFailFlag;         /* 1Ϊ��׮�ؽ�����0Ϊ����׮ */
}APP_RRC_DebugStubHoFailReq;
/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_REL_ALL_REQ_STRU
 �ṹ˵��  :��׮REL ALL
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      msgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      opId;
    VOS_UINT32                      relAllCmd;            /* 1Ϊ��׮REL_ALL��0Ϊ����׮ */
}APP_RRC_DebugStubRelAllReq;
/*****************************************************************************
 �ṹ��    :RRC_APP_DEBUG_STUB_CMD_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          msgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          opId;
    APP_ResultUint32              result;
}RRC_APP_DebugStubCmdCnf;



/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_ADD_BAD_CELL_REQ_STRU
 �ṹ˵��  :��׮��С���б�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      msgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      opId;
    VOS_UINT32                      addBadCellFlag;         /* 1�ӻ�С����0Ϊɾ����С�� */
    VOS_UINT16                      band;
    VOS_UINT16                      freq;
    VOS_UINT16                      phyCellId;
    VOS_UINT16                      cause;
    VOS_UINT32                      exitFLow;
    VOS_UINT32                      rsv;
    VOS_UINT8                       rsv0;
    VOS_UINT8                       rsv1;
    VOS_UINT8                       rsv2;
    VOS_UINT8                       rsv3;
    VOS_INT16                       rsrp;
    VOS_INT16                       rsrq;    /* ��С�����ʱ������ */
    VOS_UINT16                      rsv4;
    VOS_UINT16                      rsv5;
}APP_RRC_DebugStubAddBadCellReq;

/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_L2W_RESEL_RESTRAIN_REQ_STRU
 �ṹ˵��  :��׮�ؽ���
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      msgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      opId;
    VOS_UINT32                      limitFlag;         /* 1Ϊ��׮L2W��ѡ���ƣ�0Ϊ����׮ */
}APP_RRC_DebugStubL2WReselLimitReq;

/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :����С����ͬƵ��Ƶ������Ϣ
****************************************************************************/
typedef struct
{
    VOS_UINT32    mcc;         //��Чֵ0xFFFFFFFF
    VOS_UINT32    mnc;         //MCC-MNC����Чֵ0xFFFFFFFF
    VOS_UINT16    arfcn;       //Ƶ��
    VOS_UINT16    band;        //Ƶ��
    VOS_UINT32    pci;         //PCI
    VOS_UINT32    cgi;         //CGI����Чֵ0xFFFFFFFF
    VOS_UINT16    tac;         //TAC����Чֵ0xFFFF
    VOS_UINT16    bW;          //BW����Чֵ0xFFFF
    VOS_INT16     rsrp;
    VOS_INT16     rssi;
    VOS_INT16     rsrq;
    VOS_UINT8     reserved[6]; /* �����ֶΣ�����Ϊ6,����4�ֽڶ��� */
} PS_LTE_CellInfo;

/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ǿС����Ϣ����������С������������Ϣ�����֧��8������
****************************************************************************/
typedef struct
{
    VOS_UINT32    flag;           //Ϊ��ǰ̨�������ݣ���ǰĬ��Ϊ1
    VOS_UINT16    length;         //���ȣ����ֽ�֮�����ݳ��ȣ����������ֽڣ�
    VOS_UINT8     ueState;        /*״̬*,��ЧֵΪ0xFF*/
    VOS_UINT8     reserved[5]; /* �����ֶΣ�����Ϊ5,����4�ֽڶ��� */
    VOS_UINT32    activeCount;    //����С������
    VOS_UINT32    nbrCount;       //��������
    PS_LTE_CellInfo     cellInfo[LRRC_ECELL_QRY_CELL_REPORT_NUM];
} PS_LTE_EcellInfo;

/*****************************************************************************
 �ṹ��    :CA_STATE_INFO
 Э����  :
 ASN.1���� :
 �ṹ˵��  :Scell�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32    cellId;    //С��ID
    VOS_UINT32    arfcn;     //���ȷ���Ƶ�㡢��Ƶ�ʣ�
} CA_STATE_Info;

/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :CA�����Ϣ
****************************************************************************/
typedef struct
{
    VOS_UINT32        flag;        //Ĭ��Ϊ1
    VOS_UINT16        length;      //����
    VOS_UINT8         reserved[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
    VOS_UINT32        count;       //CA��Ϣ�ĸ���
    CA_STATE_Info     cAInfo[LRRC_ECELL_QRY_CELL_REPORT_NUM];      //CA������Ϣ
} LTE_CA_StateInfo;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ѯ��ǿС����Ϣ����������С������������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32         msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32         opId;
    VOS_UINT8          reserved[4]; /* �����ֶΣ�����Ϊ4,����4�ֽڶ��� */
}APP_RRC_QueryEcellInfoReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32         msgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32         opId;
    VOS_UINT8          reserved[4]; /* �����ֶΣ�����Ϊ4,����4�ֽڶ��� */
}APP_RRC_QueryScellInfoReq;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_ECELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                               /*_H2ASN_Skip*/
    VOS_UINT32                 msgId;          /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                 opId;
    PS_LTE_EcellInfo     eCellInfo;   /*��ǿ��С��������С��������*/
}RRC_APP_QueryEcellInfoCnf;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                               /*_H2ASN_Skip*/
    VOS_UINT32                 msgId;          /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                 opId;
    LTE_CA_StateInfo         cellInfo;      /*CAС����Ϣ*/
}RRC_APP_QueryScellInfoCnf;



extern PS_BOOL_ENUM_UINT8  LRRC_COMM_LoadDspAlready( MODEM_ID_ENUM_UINT16 modemId);

extern VOS_UINT32 LHPA_InitDsp( VOS_VOID );
extern VOS_VOID LHPA_DbgSendSetWorkMode_toMaterMode(MODEM_ID_ENUM_UINT16 modemId);
extern VOS_VOID LHPA_DbgSendSetWorkMode_toSlaveMode(MODEM_ID_ENUM_UINT16 modemId);

extern VOS_VOID * LAPP_MemAlloc( MODEM_ID_ENUM_UINT16 modemId,VOS_UINT32 size );
extern VOS_UINT32  LApp_MemFree(MODEM_ID_ENUM_UINT16 modemId,VOS_VOID *addr );
extern VOS_UINT32  LAppSndMsgToLPs(MODEM_ID_ENUM_UINT16 modemId,APP_LPS_Msg  *appToPsMsg );
extern 	VOS_UINT32 LHPA_InitDsp_ForAT( VOS_VOID );
extern VOS_UINT32 LHPA_InitDspNvForLteTdsCBT(MODEM_ID_ENUM_UINT16 modemId);
extern VOS_UINT32 LHPA_LoadDspForLteCBT(MODEM_ID_ENUM_UINT16 modemId);
extern VOS_VOID   LHPA_DbgSetSlaveModeThenMasterMode(MODEM_ID_ENUM_UINT16 modemId);


/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/










#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of app_rrc_interface.h */
