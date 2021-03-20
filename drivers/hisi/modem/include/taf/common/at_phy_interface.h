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

#ifndef __ATPHYINTERFACE_H__
#define __ATPHYINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "taf_dsp_phy.h"

#pragma pack(push, 4)

#define W_AFC_INIT_VALUE (0x959) /* W_AFC״̬��ֵ */

#define WDSP_CTRL_TX_OFF 2 /* ��ʾDSPǿ������RF�ر� */
#define WDSP_CTRL_TX_ON 3  /* ��ʾDSPǿ������RF�� */

#define WDSP_CTRL_TX_ONE_TONE 8 /* ��ʾ��ͬ�൥���ź� */
#define WDSP_CTRL_TX_TWO_TONE 9 /* ��ʾ�򿪷��൥���ź� */

#define WDSP_CTRL_TX_THREE_TONE 3 /* ��ʾ�򿪵��� */

#define GDSP_CTRL_TX_OFF 0xAAAA /* ��ʾDSPǿ������RF�ر� */
#define GDSP_CTRL_TX_ON 0x5555  /* ��ʾDSPǿ������RF�� */

#define WDSP_MAX_TX_AGC 2047
#define GDSP_MAX_TX_VPA 1023

#define BBP_PA_HIGH_MODE 1 /* PA������ģʽ */
#define BBP_PA_MID_MODE 2  /* PA������ģʽ */
#define BBP_PA_LOW_MODE 3  /* PA������ģʽ */
#define BBP_PA_AUTO_MODE 0 /* PA�Զ�����ģʽ */

#define DSP_CTRL_RX_OFF 2        /* CPU���ƹر���ƵоƬ�� */
#define DSP_CTRL_RX_ANT1_ON 3    /* CPU���ƴ���ƵоƬ,����1�� */
#define DSP_CTRL_RX_ALL_ANT_ON 7 /* CPU���ƴ���ƵоƬ,����1��2 */

#define DSP_LNA_HIGH_GAIN_MODE (0x2)    /* DSP�̶�LNAΪ������ģʽ */
#define DSP_LNA_LOW_GAIN_MODE (0x1)     /* DSP�̶�LNAΪ������ģʽ */
#define DSP_LNA_NO_CTRL_GAIN_MODE (0x0) /* WBBP����LNAģʽ,�Զ�ģʽ */

#define AT_GDSP_AGC_GAIN1_75DB (0) /* GDSP AGC����,0�� */
#define AT_GDSP_AGC_GAIN2_63DB (1) /* GDSP AGC����,1�� */
#define AT_GDSP_AGC_GAIN3_43DB (2) /* GDSP AGC����,2�� */
#define AT_GDSP_AGC_GAIN4_23DB (3) /* GDSP AGC����,3�� */

#define AT_GDSP_RX_MODE_BURST (0)           /* ���� */
#define AT_GDSP_RX_MODE_CONTINOUS_BURST (1) /* �������� */
#define AT_GDSP_RX_MODE_STOP (2)            /* ֹͣ */

#define AT_GDSP_RX_SLOW_AGC_MODE (0) /* Slow AGC */
#define AT_GDSP_RX_FAST_AGC_MODE (1) /* fast AGC */

#define AT_DSP_RF_AGC_STATE_ERROR (-1) /* ȡ�ò���ֵʱ������״̬��-1��ʾ���� */

#define AT_DSP_RSSI_MEASURE_NUM (1)          /* RSSI�Ĳ������� */
#define AT_DSP_RSSI_MEASURE_INTERVAL (0)     /* RSSI�Ĳ������ʱ�䣬��λms */
#define AT_DSP_RSSI_VALUE_UINT (0.125)       /* RSSI����ֵ����λ0.125dBm */
#define AT_DSP_RSSI_VALUE_MUL_THOUSAND (125) /* RSSI����ֵ,��λ0.125dBm*1000 */

/* tyg */
#define AT_DPS_NOISERSSI_MAX_NUM (32)

#define AT_RX_PRI_ON (0) /* �������� */
#define AT_RX_DIV_ON (1) /* �����ּ� */
#define AT_RXON_OPEN (0) /* ���һ��ִ�е��Ǵ򿪽��ջ�RXON=1���� */
#define AT_TXON_OPEN (1) /* ���һ��ִ�е��Ǵ򿪷����TXON=1���� */

#define AT_BIT0 0x00000001L
#define AT_BIT1 0x00000002L
#define AT_BIT2 0x00000004L
#define AT_BIT3 0x00000008L
#define AT_BIT4 0x00000010L
#define AT_BIT5 0x00000020L
#define AT_BIT6 0x00000040L
#define AT_BIT7 0x00000080L
#define AT_BIT8 0x00000100L
#define AT_BIT9 0x00000200L
#define AT_BIT10 0x00000400L
#define AT_BIT11 0x00000800L
#define AT_BIT12 0x00001000L
#define AT_BIT13 0x00002000L
#define AT_BIT14 0x00004000L
#define AT_BIT15 0x00008000L

/* W_RF_CFG_REQ��������� */
#define W_RF_MASK_AFC AT_BIT0
#define W_RF_MASK_TX_ARFCN AT_BIT1
#define W_RF_MASK_TX_TXONOFF AT_BIT2
#define W_RF_MASK_TX_PAMODE AT_BIT3
#define W_RF_MASK_TX_AGC AT_BIT4
#define W_RF_MASK_TX_POWER AT_BIT5
#define W_RF_MASK_RX_RXONOFF AT_BIT6
#define W_RF_MASK_RX_ARFCN AT_BIT7
#define W_RF_MASK_RX_ANTSEL AT_BIT8
#define W_RF_MASK_RX_LNAMODE AT_BIT9
#define W_RF_MASK_RX_PGC AT_BIT10
#define W_RF_MASK_TX_POWDET AT_BIT11
#define W_RF_MASK_TX_CLT AT_BIT12
#define W_RF_MASK_TX_DCXOTEMPCOMP AT_BIT14
/* G_RF_RX_CFG_REQ_STRU�ṹ��uhwMask�ı���λ���� */
#define G_RF_MASK_RX_ARFCN AT_BIT0   /* �·�Ƶ�� */
#define G_RF_MASK_RX_MODE AT_BIT1    /* �·�����ģʽ */
#define G_RF_MASK_RX_AGCMODE AT_BIT2 /* �·�����ģʽ */
#define G_RF_MASK_RX_AGCGAIN AT_BIT3 /* �·�AGC��λ */

/* G_RF_TX_CFG_REQ_STRU�ṹ��uhwMask�ı���λ���� */
/*
 * ע��: 1)��GMSK���Ʒ�ʽ�£�Ŀ�귢�书�ʿ�ͨ��G_RF_MASK_TX_GSMK_PA_VOLT
 *   ��G_RF_MASK_TX_POWER�·�,������ͬʱ�·�;
 * 2)��8PSK���Ʒ�ʽ��:Ŀ�깦��ֵֻ��ͨ��G_RF_MASK_TX_POWER�·�
 * 3)������λ����ͬʱ�·����������໥��ͻ
 */
#define G_RF_MASK_TX_AFC AT_BIT0           /* �·�����Ƶ�ʿ���ֵ */
#define G_RF_MASK_TX_ARFCN AT_BIT1         /* �·�����Ƶ�� */
#define G_RF_MASK_TX_TXEN AT_BIT2          /* �·��������λ */
#define G_RF_MASK_TX_TXMODE AT_BIT3        /* �·�����ģʽ */
#define G_RF_MASK_TX_MOD_TYPE AT_BIT4      /* �·�����ģʽ */
#define G_RF_MASK_TX_GSMK_PA_VOLT AT_BIT5  /* �·�GMSK����ʱ PAĿ����Ƶ�ѹ */
#define G_RF_MASK_TX_POWER AT_BIT6         /* �·�Ŀ�귢�����ֵ,GMSK��8PSK���� */
#define G_RF_MASK_TX_8PSK_PA_VBIAS AT_BIT7 /* �·�8PSK����ʱPAƫ�õ�ѹ */
#define G_RF_MASK_TX_TXOLC AT_BIT8         /* �·�TXOLC�Ĵ���ֵ */
#define G_RF_MASK_TX_DATA_PATTERN AT_BIT9

/* ���Ʒ�ʽ */
#define G_MOD_TYPE_GMSK 0 /* GMSK���Ʒ�ʽ */
#define G_MOD_TYPE_8PSK 1 /* 8PSK���Ʒ�ʽ */

/* noise tool ����ϱ��Ľ������ */
#define PHY_NOISE_MAX_RESULT_NUMB 32

/* ��������·������й���Ϊ0xff��Ϊ�������� */
#define PHY_NOIST_TOOL_CLOSE_TX 0xff

/* �����ߵ�ģʽ */
enum AT_AntMode {
    ANT_ONE = 1, /* ֻ���������� */
    ANT_TWO = 2, /* ͬʱ�������ͷּ����� */
    ANT_BUTT
};
typedef VOS_UINT8 AT_AntModeUint8;

/* WDSP ��ʽƵ�ζ��� */
enum AT_WBand {
    W_FREQ_BAND1 = 1,
    W_FREQ_BAND2,
    W_FREQ_BAND3,
    W_FREQ_BAND4,
    W_FREQ_BAND5,
    W_FREQ_BAND6,
    W_FREQ_BAND7,
    W_FREQ_BAND8,
    W_FREQ_BAND9,
    W_FREQ_BAND10, /*  BAND10 ��֧�� */
    W_FREQ_BAND11,
    W_FREQ_BAND_BUTT
};
typedef VOS_UINT16 AT_WDspBandUint16;

/* GDSP Ƶ�ζ��� */
enum AT_GDspBand {
    G_FREQ_BAND_GSM850 = 0,
    G_FREQ_BAND_GSM900,
    G_FREQ_BAND_DCS1800,
    G_FREQ_BAND_PCS1900,
    G_FREQ_BAND_BUTT
};
typedef VOS_UINT16 AT_GDspBandUint16;

/* AT HPA���ý�� 0:success, 1:fail */
enum AT_HPA_CfgRlst {
    AT_HPA_RSLT_SUCC = 0,
    AT_HPA_RSLT_FAIL,
    AT_HPA_RSLT_BUTT
};
typedef VOS_UINT16 AT_HPA_CfgRlstUint16;

/* SLT TEST ö��ֵ�趨 */
enum {
    SLT_SERDES_TSET = 8,
    SLT_ALINK_TEST  = 9,
    SLT_TEST_TYPE_BUTT
};
typedef VOS_UINT8 AT_PHY_SltTestType;

/* W����У׼ԭ��ID */
#define ID_AT_HPA_RF_CFG_REQ 0x2621
#define ID_HPA_AT_RF_CFG_CNF 0x62E0

#define ID_AT_HPA_RF_RX_RSSI_REQ 0x2622
#define ID_HPA_AT_RF_RX_RSSI_IND 0x62E1

/* W PLL����״̬��ѯԭ��ID */
#define ID_AT_WPHY_RF_PLL_STATUS_REQ 0x2623
#define ID_AT_WPHY_RF_PLL_STATUS_CNF 0x62E2

/* G����У׼ԭ��ID */
#define ID_AT_GHPA_RF_RX_CFG_REQ 0x2415 /* RX����У׼�������� */
#define ID_AT_GHPA_RF_TX_CFG_REQ 0x2418 /* TX����У׼�������� */
#define ID_GHPA_AT_RF_MSG_CNF 0x4212    /* ͨ�ûظ���Ϣԭ�� */

#define ID_AT_GHPA_RF_RX_RSSI_REQ 0x2416 /* RX����У׼�������� */
#define ID_GHPA_AT_RF_RX_RSSI_IND 0x4210 /* RX����У׼��������ϱ� */

/* G PLL����״̬��ѯԭ��ID */
#define ID_AT_GPHY_RF_PLL_STATUS_REQ 0x2419
#define ID_AT_GPHY_RF_PLL_STATUS_CNF 0x4219

/* PD���״̬��ѯԭ��ID */
#define ID_AT_PHY_POWER_DET_REQ 0x2624
#define ID_AT_PHY_POWER_DET_CNF 0x62e3

/* noise tool ��wphy�Ľӿ� */
#define ID_AT_HPA_RF_NOISE_CFG_REQ 0x2625
#define ID_HPA_AT_RF_NOISE_RSSI_IND 0x62e4
/* �ظ���NOISE TOOL����Ϣԭ�� ԭ��ID */
#define ID_AT_GHPA_RF_NOISE_CFG_REQ 0x241A
#define ID_GHPA_AT_RF_NOISE_RSSI_IND 0x421A

#define ID_AT_HPA_MIPI_WR_REQ 0x2490
#define ID_HPA_AT_MIPI_WR_CNF 0x4290
#define ID_AT_HPA_MIPI_RD_REQ 0x2491
#define ID_HPA_AT_MIPI_RD_CNF 0x4291

#define ID_AT_HPA_SSI_WR_REQ 0x2492
#define ID_HPA_AT_SSI_WR_CNF 0x4292
#define ID_AT_HPA_SSI_RD_REQ 0x2493
#define ID_HPA_AT_SSI_RD_CNF 0x4293

#define ID_AT_HPA_PDM_CTRL_REQ 0x2494
#define ID_HPA_AT_PDM_CTRL_CNF 0x4294

#define ID_WPHY_AT_TX_CLT_IND 0x62e5

/* CDMA����У׼ԭ��ID */
#define ID_AT_CHPA_RF_CFG_REQ 0x6300
#define ID_CHPA_AT_RF_CFG_CNF 0x6301

#define ID_AT_CHPA_RF_RX_RSSI_REQ 0x6302
#define ID_CHPA_AT_RF_RX_RSSI_IND 0x6303

#define ID_AT_PHY_SERDES_AGING_TEST_REQ 0x2626
#define ID_PHY_AT_SERDES_AGING_TEST_CNF 0x62E6

#define ID_AT_PHY_SERDES_TEST_ASYNC_REQ 0x2628
#define ID_PHY_AT_SERDES_TEST_ASYNC_CNF 0x62e8
/* slt test */
#define ID_AT_PHY_SLT_TEST_REQ 0x262a
#define ID_PHY_AT_SLT_TEST_CNF 0x62ea


typedef struct {
    /* Tx Cfg */
    VOS_UINT16 mask;      /* ��λ��ʶ�������� */
    VOS_UINT16 txAfcInit; /* AFC */
    VOS_UINT16 txBand;    /* 1,2,3...,Э���е�band���,ע�ⲻ��BandId */
    VOS_UINT16 txFreqNum; /* Arfcn */
    VOS_UINT16 txEnable;  /* TXͨ��ʹ�ܿ��� */
    VOS_UINT16 txPaMode;  /* PAģʽ���� */
    VOS_INT16  txPower;   /* 0.1dBm */

    /* Rx Cfg */
    VOS_UINT16 rxEnable;  /* RXͨ��ʹ�ܿ��� */
    VOS_UINT16 rxBand;    /* 1,2,3...,Э���е�band��� */
    VOS_UINT16 rxFreqNum; /* Arfcn */
    VOS_UINT16 rxAntSel;  /* Ant1/Ant2 */
    VOS_UINT16 rsv;       /* ����λ */

} C_RfCfgPara;

/* AT_HPA ��������ԭ��ӿ� */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16  msgId;     /* Msg ID */
    VOS_UINT16  rsv;       /* ����λ */
    C_RfCfgPara rfCfgPara; /* RF���ò����ṹ */
} AT_CHPA_RfCfgReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;    /* Msg ID */
    VOS_UINT16 rsv;      /* ����λ */
    VOS_UINT16 measNum;  /* RSSI�Ĳ������� */
    VOS_UINT16 interval; /* RSSI�Ĳ������ʱ�䣬��λms */
} AT_CHPA_RfRxRssiReq;

/* W��RF���ýṹ */
/* RF���ò����ṹ */

typedef struct {
    /* TX RX��ʹ�� */
    VOS_INT16 rrcWidth; /* �ñ�����ʱ����,˫�ز�ģʽʱ,��ֵΪRRC����,��Χ[-5,5]MHz */

    /* Tx Cfg */
    VOS_UINT16 mask;      /* ��λ��ʶ�������� */
    VOS_UINT16 txAfcInit; /* AFC */
    VOS_UINT16 txBand;    /* 1,2,3...,Э���е�band���,ע�ⲻ��BandId */
    VOS_UINT16 txFreqNum; /* Arfcn */
    VOS_UINT16 txEnable;  /* TXͨ��ʹ�ܿ��� */
    VOS_UINT16 txPaMode;  /* PAģʽ���� */
    VOS_INT16  txPower;   /* 0.1dBm */
    VOS_INT16  dbbAtten;  /* DBB���ź�˥���Ŀ����� ӳ�䵽usTxAGC???�������岻һ�£�������ȷ�� */
    VOS_UINT16 rfAtten;   /* RF���ź�˥���Ŀ����� */
    VOS_UINT16 txAgc;     /* ����AGC���� */
    VOS_UINT16 paVbias;   /* W PA Vbias �Ŀ����� */

    /* Rx Cfg */
    VOS_UINT16 rxEnable;      /* RXͨ��ʹ�ܿ��� */
    VOS_UINT16 rxBand;        /* 1,2,3...,Э���е�band��� */
    VOS_UINT16 rxFreqNum;     /* Arfcn */
    VOS_UINT16 rxAntSel;      /* Ant1/Ant2 */
    VOS_UINT16 rxLnaGainMode; /* LNAģʽ���� */

    VOS_UINT16 rxCarrMode; /* 0:���ز�;1:˫�ز� */
} W_RfCfgPara;

/* AT_HPA ��������ԭ��ӿ� */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16  msgId;     /* Msg ID */
    VOS_UINT16  rsv;       /* ����λ */
    W_RfCfgPara rfCfgPara; /* RF���ò����ṹ */
} AT_HPA_RfCfgReq;

/* G��RF���ýṹ */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;   /* Msg ID */
    VOS_UINT16 rsv;     /* ����λ */
    VOS_UINT16 mask;    /* Reference MASK_CAL_RF_G_RX_* section */
    VOS_UINT16 freqNum;
    VOS_UINT16 rxMode;  /* 0:burst����; 1:��������;, */
    VOS_UINT16 agcMode; /* Fast AGC,Slow AGC */
    VOS_UINT16 agcGain; /* AGC��λ�����ĵ�,ȡֵΪ0-3 */
    VOS_UINT16 rsv2;
} AT_GHPA_RfRxCfgReq;


enum AT_GHPA_RfCtrlmodeType {
    AT_GHPA_RF_CTRLMODE_TYPE_GMSK,
    AT_GHPA_RF_CTRLMODE_TYPE_TXPOWER,
    AT_GHPA_RF_CTRLMODE_TYPE_8PSK,
    AT_GHPA_RF_CTRLMODE_TYPE_BUTT
};
typedef VOS_UINT16 AT_GHPA_RfCtrlmodeTypeUint16;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16                   msgId;    /* Msg ID */
    VOS_UINT16                   rsv;      /* ����λ */
    VOS_UINT16                   mask;     /* Reference MASK_CAL_RF_G_TX_* section */
    VOS_UINT16                   afc;      /* AFC */
    VOS_UINT16                   modType;  /* ������Ʒ�ʽ:0��ʾGMSK����;1��ʾ8PSK���Ʒ�ʽ */
    VOS_UINT16                   freqNum;
    VOS_UINT16                   txEnable; /* ����ʹ�ܿ���:0x5555-ʹ�ܷ���;0xAAAA-ֹͣ����;TSC 0; TxData: ����� */
    VOS_UINT16                   txMode;   /* 0:burst����; 1:�������� */
    AT_GHPA_RfCtrlmodeTypeUint16 ctrlMode; /* ������Ʒ�ʽ��
                                            * 0��GMSK��ѹ����,�˷�ʽ��usTxVpaҪ���ã�
                                            * 1�����ʿ���,�˷�ʽ��usTxPowerҪ���ã�
                                            * 2��8PSK PaVbias��ѹ&DBB Atten&RF Atten���ƣ�
                                            * usPAVbias��usRfAtten,sDbbAtten����������Ҫ���ã�
                                            */
    VOS_UINT16 reserved;
    VOS_UINT32 rfAtten;
    VOS_UINT16 txPower; /* Ŀ�귢�书��,��λΪ0.1dBm,GSM��EDGE���� */
    VOS_UINT16 txVpa;   /* GSM PA���书�ʿ��Ƶ�ѹ,ȡֵ��Χ: */
    VOS_INT16  dbbAtten;
    VOS_UINT16 paVbias; /* EDGE PA Vbais ��ѹ */
} AT_GHPA_RfTxCfgReq;

/* Response Head Struct    W/Gͨ�� */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 transPrimId; /* 0x8001 */
    VOS_UINT16 rsv1;        /* ����λ */
    VOS_UINT16 msgId;       /* Msg ID */
    VOS_UINT16 rsv;         /* ����λ */
} HPA_AT_Header;

typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT16    cfgMsgId; /* ���յ�����ϢID */
    VOS_UINT16    errFlg;   /* 0:success, 1:fail */
} HPA_AT_RfCfgCnf;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;    /* Msg ID */
    VOS_UINT16 rsv;      /* ����λ */
    VOS_UINT16 measNum;  /* RSSI�Ĳ������� */
    VOS_UINT16 interval; /* RSSI�Ĳ������ʱ�䣬��λms */
} AT_HPA_RfRxRssiReq;

/* ���յ����������ϱ�������ϱ�ǰһ�ν��յ�RSSI�������  G/Wͨ�� */
typedef struct {
    HPA_AT_Header msgHeader;
    VOS_INT16     rssi;    /* RSSI����ֵ [-2048,+2047]����λ0.125dBm */
    VOS_INT16     agcGain; /* ȡ�ò���ֵʱ������״̬��-1��ʾ���� */
} HPA_AT_RfRxRssiInd;


typedef struct {
    HPA_AT_Header msgHeader;          /* ��Ϣͷ */
    VOS_INT16     gammaReal;          /* ����ϵ��ʵ�� */
    VOS_INT16     gammaImag;          /* ����ϵ���鲿 */
    VOS_UINT16    gammaAmpUc0;        /* פ����ⳡ��0����ϵ������ */
    VOS_UINT16    gammaAmpUc1;        /* פ����ⳡ��1����ϵ������ */
    VOS_UINT16    gammaAmpUc2;        /* פ����ⳡ��2����ϵ������ */
    VOS_UINT16    gammaAntCoarseTune; /* �ֵ����λ�� */
    VOS_UINT32    fomcoarseTune;      /* �ֵ�FOMֵ */
    VOS_UINT16    cltAlgState;        /* �ջ��㷨����״̬ */
    VOS_UINT16    cltDetectCount;     /* �ջ������ܲ��� */
    VOS_UINT16    dac0;               /* DAC0 */
    VOS_UINT16    dac1;               /* DAC1 */
    VOS_UINT16    dac2;               /* DAC2 */
    VOS_UINT16    dac3;               /* DAC3 */
} WPHY_AT_TxCltInd;


typedef struct {
    VOS_UINT16 dlFreq;    /* PHY�ϱ���ǰ����Ƶ�� */
    VOS_INT16  dlPriRssi; /* PHY�ϱ���Ӧ����RSSI */
    VOS_INT16  dlDivRssi; /* PHY�ϱ���Ӧ�ּ�RSSI */
    VOS_UINT16 rsv;
} PHY_NOISE_Result;


typedef struct {
    VOS_UINT16       dlRssiNum; /* PHY�ϱ�����RSSI���� */
    VOS_UINT16       meaStatus; /* PHY�ϱ�����״̬��0��ʾ�ɹ���1��ʾ��������2��ʾ��ʱ */
    PHY_NOISE_Result dlRssiResult[AT_DPS_NOISERSSI_MAX_NUM];
} HPA_NOISE_RssiInd;


typedef struct {
    HPA_AT_Header     msgHeader; /* ��׼ͷ */
    HPA_NOISE_RssiInd rssiInd;   /* ��� */
} PHY_AT_RfNoiseRssiInd;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;   /* Msg ID */
    VOS_UINT16 rsv1;    /* ����λ */
    VOS_UINT16 dspBand; /* DSP��ʽ��Ƶ��ֵ */
    VOS_UINT16 rsv2;    /* ����λ */
} AT_PHY_RfPllStatusReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT16    txStatus; /* 0:PLLʧ��, 1:PLL���� */
    VOS_UINT16    rxStatus; /* 0:PLLʧ��, 1:PLL���� */
} PHY_AT_RfPllStatusCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv;   /* ����λ */
} AT_PHY_PowerDetReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_INT16     powerDet; /* ���ʼ������0.1dBm���� */
    VOS_UINT16    rsv;      /* ����λ */
} PHY_AT_PowerDetCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;       /* Msg ID */
    VOS_UINT16 mode;        /* UE ģʽ */
    VOS_UINT16 band;        /* band��Ϣ */
    VOS_UINT16 dlStartFreq; /* ���п�ʼƵ�� */
    VOS_UINT16 dlEndFreq;   /* ���н���Ƶ�� */
    VOS_UINT16 freqStep;    /* ���� */
    VOS_INT16  txPwr;       /* ���书�� */
    VOS_UINT16 rsv;         /* ����λ */
} AT_HPA_RfNoiseCfgReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 secondaryAddr;
    VOS_UINT16 regAddr;
    VOS_UINT16 regData;
    VOS_UINT16 mipiChannel;
    VOS_UINT16 rsv;
} AT_HPA_MipiWrReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT32    errFlg; /* 0:success, 1:fail */
} HPA_AT_MipiWrCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;         /* Msg ID */
    VOS_UINT16 secondaryAddr; /* ���ն˵�ַ */
    VOS_UINT16 reg;           /* �Ĵ������� */
    VOS_UINT16 channel;       /* ��ʼ�ļĴ��� */
} AT_HPA_MipiRdReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT32    value;
    VOS_UINT32    result;
} HPA_AT_MipiRdCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 rficSsi; /* Mipi */
    VOS_UINT16 regAddr; /* ��ʼ�ļĴ��� */
    VOS_UINT16 data;    /* ���� */
} AT_HPA_SsiWrReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT32    errFlg; /* 0:success, 1:fail */
} HPA_AT_SsiWrCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;     /* Msg ID */
    VOS_UINT16 channelNo; /* ���ն˵�ַ */
    VOS_UINT32 rficReg;
} AT_HPA_SsiRdReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT32    value;
    VOS_UINT32    result;
} HPA_AT_SsiRdCnf;

/* ���յ����������ϱ�������ϱ�ǰһ�ν��յ�RSSI�������  G/Wͨ�� */
typedef struct {
    HPA_AT_Header msgHeader;
    VOS_INT16     rssi;    /* RSSI����ֵ [-2048,+2047]����λ0.125dBm */
    VOS_INT16     agcGain; /* ȡ�ò���ֵʱ������״̬��-1��ʾ���� */
} CHPA_AT_RfRxRssiInd;

typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT16    cfgMsgId; /* ���յ�����ϢID */
    VOS_UINT16    errFlg;   /* 0:success, 1:fail */
} CHPA_AT_RfCfgCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv;   /* ����λ */
    VOS_UINT16 pdmRegValue;
    VOS_UINT16 paVbias;
    VOS_UINT16 paVbias2;
    VOS_UINT16 paVbias3;
} AT_HPA_PdmCtrlReq;


typedef struct {
    HPA_AT_Header msgHeader;
    VOS_UINT32    result;
} HPA_AT_PdmCtrlCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv;
    VOS_UINT32 testNum;     /* ���Դ�������ֻ��AT�������õ�ֵ����λPHY�빤��ȷ�� */
    VOS_UINT16 syncLen;     /* ����sync rept len */
    VOS_UINT16 burstTime;   /* ����Burstʱ�䣬��λMC */
    VOS_UINT16 stallTime;   /* ����Stallʱ�䣬��λMC */
    VOS_UINT16 dlSyncLen;   /* ����sync rept len */
    VOS_UINT16 dlBurstTime; /* ����Burstʱ�䣬��λMC */
    VOS_UINT16 dlStallTime; /* ����Stallʱ�䣬��λMC */
    VOS_UINT16 lsTest;
    VOS_UINT16 fwdClkAmp;
} AT_PHY_SerdesAgingTestReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv;
    VOS_INT32  failNum; /* ʧ�ܴ�����0��ʾû��ʧ�� */
} PHY_AT_SerdesAgingTestCnf;


typedef struct {
    VOS_UINT32 para1;
    VOS_UINT32 para2;
    VOS_UINT32 para3;
    VOS_UINT32 para4;
    VOS_UINT32 para5;
    VOS_UINT32 para6;
    VOS_UINT32 para7;
    VOS_UINT32 para8;
    VOS_UINT32 para9;
    VOS_UINT32 para10;
    VOS_UINT32 para11;
    VOS_UINT32 para12;
    VOS_UINT32 para13;
    VOS_UINT32 para14;
    VOS_UINT32 para15;
    VOS_UINT32 para16;
    VOS_UINT32 para17;
    VOS_UINT32 para18;
    VOS_UINT32 para19;
    VOS_UINT32 para20;
} AT_PHY_SerdesTestParaList;


typedef union {
    AT_PHY_SerdesTestParaList atSerdesTestParaList; /* NAS�����б� */
} AT_PHY_SerdesTestPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16            msgID;
    VOS_UINT8             type;
    VOS_UINT8             rsv;
    AT_PHY_SerdesTestPara serdesTestPara;
} AT_PHY_SerdesTestReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgID;
    VOS_INT16  result;
} PHY_AT_SerdesTestCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16         msgId; /* Msg ID */
    VOS_UINT16         rsv;
    AT_PHY_SltTestType testType;
    VOS_UINT8          rsv1[3];
} AT_PHY_SltTestReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv;
    VOS_INT32  result;
} PHY_AT_SltTestCnf;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of at_phy_interface.h */
