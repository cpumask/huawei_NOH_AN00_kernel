 

#ifndef __WLAN_SPEC_H__
#define __WLAN_SPEC_H__

// �˴�����extern "C" UT���벻��
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST)
#include "wlan_oneimage_define.h"
#endif

#include "platform_spec.h"
#include "wlan_types.h"
#include "wlan_spec_1103.h"
#include "wlan_spec_1105.h"
#include "wlan_spec_1106.h"


#define WLAN_M2S_BLACKLIST_MAX_NUM 16 /* m2s�·��������û�������Ҫ���ϲ㱣��һ�� */


/* 0.0 ���ƻ��������� */
/* ���ƻ������Ϊһ���ṹ�壬�������ṩ��������������̬��������Ԥ���������̬ */
/* 0.1.2 �ȵ��������� */
/* ��ΪP2P GO �����������û��� */
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_DEFAULT WLAN_P2P_GO_ASSOC_USER_MAX_NUM_1103


/* 1.0 WLANоƬ��Ӧ��spec */
/* ÿ��board֧��chip������������ƽ̨ */
/* ÿ��chip֧��device������������ƽ̨ */
/* ���֧�ֵ�MACӲ���豸��������ƽ̨ */
/* 1.3 oam��ص�spec */
/* oam ����ƽ̨ */
/* 1.4 mem��Ӧ��spec */
/* 1.4.1 �ڴ�ع�� */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.2 �����������ڴ��������Ϣ */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.3 �������֡�ڴ��������Ϣ */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.4 ��������֡�ڴ��������Ϣ */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.5 �����ڴ��������Ϣ */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.6 �¼��ṹ���ڴ�� */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.7 �û��ڴ�� */
/* 1.4.8 MIB�ڴ��  TBD :���ո��ӳصĿռ��С��������Ҫ���¿��� */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.9 netbuf�ڴ��  TBD :���ո��ӳصĿռ��С��������Ҫ���¿��� */
/* �ڴ� spec ����ƽ̨ */
/* 1.4.9.1 sdt netbuf�ڴ�� */
/* �ڴ� spec ����ƽ̨ */
/* 1.5 frw��ص�spec */
/* �¼����� spec ����ƽ̨ */
/* 2 �궨�壬�����DR����һ�� */
/* 2.1 ����Э��/���������Э������spec */
/* 2.1.1 ɨ���STA ���� */
/* TBD һ�ο���ɨ������BSS���������������Ժϲ� */
#define WLAN_SCAN_REQ_MAX_BSSID 2
#define WLAN_SCAN_REQ_MAX_SSID  8

/* TBD  һ���ŵ��¼�¼ɨ�赽�����BSS���� �����٣���Ҫ����200 */
#define WLAN_MAX_SCAN_BSS_PER_CH 8

#define WLAN_DEFAULT_FG_SCAN_COUNT_PER_CHANNEL        2 /* ǰ��ɨ��ÿ�ŵ�ɨ����� */
#define WLAN_DEFAULT_BG_SCAN_COUNT_PER_CHANNEL        1 /* ����ɨ��ÿ�ŵ�ɨ����� */
#define WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL 1 /* ÿ���ŵ�ɨ�跢��probe req֡�Ĵ��� */

#define WLAN_DEFAULT_MAX_TIME_PER_SCAN (3 * 1500) /* ɨ���Ĭ�ϵ����ִ��ʱ�䣬������ʱ�䣬����ʱ���� */

/* TBD ɨ��ʱ��������ɨ�趨ʱʱ�䣬��λms�������������� */
#define WLAN_DEFAULT_ACTIVE_SCAN_TIME      20
#define WLAN_DEFAULT_PASSIVE_SCAN_TIME     60
#define WLAN_DEFAULT_DFS_CHANNEL_SCAN_TIME 240
#define WLAN_LONG_ACTIVE_SCAN_TIME         40 /* ָ��SSIDɨ���������3��ʱ,1��ɨ�賬ʱʱ��Ϊ40ms */

/* 2.1.1 STA�������� */
/* �����ӳ٣���λms */
#define WLAN_AUTH_TIMEOUT       300 /* STA auth timeout */
#define WLAN_AUTH_AP_TIMEOUT    500 /* APUT auth timeout */
#define WLAN_ASSOC_TIMEOUT      600

/* 2.1.2 �ȵ��������� */
/*
 * The 802.11 spec says at most 2007 stations may be
 * associated at once.  For most AP's this is way more
 * than is feasible so we use a default of 128.  This
 * number may be overridden by the driver and/or by
 * user configuration.
 */
#define WLAN_AID_MAX     2007
#define WLAN_AID_DEFAULT 128

/* ��λms */
#define WLAN_AP_USER_AGING_TIME    (60 * 1000) /* AP �û��ϻ�ʱ�� 60S */
#define WLAN_P2PGO_USER_AGING_TIME (60 * 1000) /* GO �û��ϻ�ʱ�� 60S */

/* AP keepalive����,��λms */
#define WLAN_AP_KEEPALIVE_TRIGGER_TIME (15 * 1000) /* keepalive��ʱ���������� */
#define WLAN_AP_KEEPALIVE_INTERVAL     (25 * 1000) /* ap����keepalive null֡��� */
#define WLAN_GO_KEEPALIVE_INTERVAL     (25 * 1000) /* P2P GO����keepalive null֡���  */

/* STA keepalive����,��λms */
#define WLAN_STA_KEEPALIVE_TIME (25 * 1000) /* wlan0����keepalive null֡���,ͬ1101 keepalive 25s */
#define WLAN_CL_KEEPALIVE_TIME  (20 * 1000) /* P2P CL����keepalive null֡���,����CL��GO pvb����,P2P cl 20s */

/* STA TBTT�жϲ�����ʱ������linkloss��������,��λms */
#define WLAN_STA_TBTT_PROTECT_TIME1 (15 * 1000) /* wlan0 */
#define WLAN_STA_TBTT_PROTECT_TIME2 (1000) /*wlan1 */

/* Beacon Interval���� */
/* max beacon interval, ms */
#define WLAN_BEACON_INTVAL_MAX 3500
/* min beacon interval */
#define WLAN_BEACON_INTVAL_MIN 40
/* min beacon interval */
#define WLAN_BEACON_INTVAL_DEFAULT 100
/* AP IDLE״̬��beacon intervalֵ */
#define WLAN_BEACON_INTVAL_IDLE 1000

/* 2.1.6 ����ģʽ���� */
/* TBD RTS�������ޣ�ʵ�ʿ�ɾ�� */
#define WLAN_RTS_MAX     2346

/* 2.1.7 ��Ƭ���� */
/* Fragmentation limits */
/* default frag threshold */
#define WLAN_FRAG_THRESHOLD_DEFAULT 1544
/* min frag threshold */
#define WLAN_FRAG_THRESHOLD_MIN 200 /* Ϊ�˱�֤��Ƭ��С��16: (1472(�·���󳤶�)/16)+36(����֡���֡ͷ) = 128  */
/* max frag threshold */
#define WLAN_FRAG_THRESHOLD_MAX 2346

/* 2.1.14 �������ʹ��� */
/* ������ز��� */
/* ���ڼ�¼03֧�ֵ����������� */
#define WLAN_MAX_SUPP_RATES 12

/* 2.2 ����Э��/����MAC ��Э������spec */
/* 2.2.8 �����빦�� */
/* ������������ */
#define WLAN_MAX_RC_NUM 20

/* 2.2.9 WMM���� */
/* EDCA���� */
/* STA����WLAN_EDCA_XXX����ͬWLAN_QEDCA_XXX */
#define WLAN_QEDCA_TABLE_CWMIN_MIN         1
#define WLAN_QEDCA_TABLE_CWMIN_MAX         10
#define WLAN_QEDCA_TABLE_CWMAX_MIN         1
#define WLAN_QEDCA_TABLE_CWMAX_MAX         10
#define WLAN_QEDCA_TABLE_AIFSN_MIN         2
#define WLAN_QEDCA_TABLE_AIFSN_MAX         15
#define WLAN_QEDCA_TABLE_TXOP_LIMIT_MIN    1
#define WLAN_QEDCA_TABLE_TXOP_LIMIT_MAX    65535
#define WLAN_QEDCA_TABLE_MSDU_LIFETIME_MAX 500

/* TID��������ƽ̨SPEC */
/* ���ն��еĸ��� ��HAL_RX_DSCR_QUEUE_ID_BUTT��� */
#define HAL_RX_QUEUE_NUM 3
/* ���Ͷ��еĸ��� */
#define HAL_TX_QUEUE_NUM 6

/* 2.2.10 Э�����STA�๦�� */
/* PSM���Թ�� */
/* default DTIM period */
#define WLAN_DTIM_DEFAULT 3

/* 2.2.11 Э�����AP�๦�� */
/* 2.3 У׼����spec */
/* 2.4 ��ȫЭ������spec */
/* 2.4.7 PMF STA���� */
/* SA Query���̼��ʱ��,�ϻ�ʱ�������֮һ */
#define WLAN_SA_QUERY_RETRY_TIME 201

/* SA Query���̳�ʱʱ��,С���ϻ�ʱ�� */
#define WLAN_SA_QUERY_MAXIMUM_TIME 1000

/* 2.5 ��������spec */
/* 2.5.1 ��ȷ�Ϲ��� */
#define WLAN_ADDBA_TIMEOUT   500
#define WLAN_BAR_SEND_TIMEOUT 500

/* ֧�ֵĽ���rx ba �������� */
#define WLAN_MAX_RX_BA 32   /* WTODO:��Ҫ����оƬ */

/* ֧�ֵĽ���tx ba �������� */
#define WLAN_MAX_TX_BA 32   /* WTODO:��Ҫ����оƬ */

/* 2.5.2 AMPDU���� */
#define WLAN_AMPDU_RX_HE_BUFFER_SIZE 256 /* AMPDU���ն˽��ջ�������buffer size�Ĵ�С */
#define WLAN_AMPDU_RX_BUFFER_SIZE    64  /* AMPDU���ն˽��ջ�������buffer size�Ĵ�С */
#define WLAN_AMPDU_RX_BA_LUT_WSIZE   64  /* AMPDU���ն�������дBA RX LUT���win size,
                                               Ҫ����ڵ���WLAN_AMPDU_RX_BUFFER_SIZE */

#define WLAN_AMPDU_TX_MAX_NUM 64 /* AMPDU���Ͷ����ۺ���MPDU���� */ /* WTODO:��Ҫ����оƬ */
#define WLAN_AMPDU_TX_DEFAULT_NUM (WLAN_AMPDU_TX_MAX_NUM)

#define WLAN_AMPDU_TX_MAX_BUF_SIZE     64 /* ���Ͷ˵�buffer size */

/* MAC RX BA_LUT��32�� */
#define HAL_MAX_RX_BA_LUT_SIZE 32
/* MAC TX BA_LUT��32�� */
#define HAL_MAX_TX_BA_LUT_SIZE 32

/* 2.5.8 �Զ���Ƶ */
/* δ�����ۺ�ʱpps���� */
#define NO_BA_PPS_VALUE_0 0
#define NO_BA_PPS_VALUE_1 1000
#define NO_BA_PPS_VALUE_2 2500
#define NO_BA_PPS_VALUE_3 4000
/* mate7��� */
/* pps����       CPU��Ƶ����     DDR��Ƶ���� */
/* mate7 pps���� */
#define PPS_VALUE_0 0
#define PPS_VALUE_1 2000  /* 20M up limit */
#define PPS_VALUE_2 4000  /* 40M up limit */
#define PPS_VALUE_3 10000 /* 100M up limit */
/* mate7 CPU��Ƶ���� */
#define CPU_MIN_FREQ_VALUE_0 403200
#define CPU_MIN_FREQ_VALUE_1 604800
#define CPU_MIN_FREQ_VALUE_2 806400
#define CPU_MIN_FREQ_VALUE_3 1305600
/* mate7 DDR��Ƶ���� */
#define DDR_MIN_FREQ_VALUE_0 0
#define DDR_MIN_FREQ_VALUE_1 3456
#define DDR_MIN_FREQ_VALUE_2 6403
#define DDR_MIN_FREQ_VALUE_3 9216

/* 2.6 �㷨����spec */
/* 2.6.3 �㷨����ģʽ����ASIC��FPGA */
// ע��˴�������㷨���غ꣬�벻Ҫ��hostʹ��
/* TBD 23 ����书�ʣ���λdBm */
#define WLAN_MAX_TXPOWER  25
#define WLAN_MIN_TXPOWER (-25)

/* 2.8.2 HAL Device0оƬ������ */
/* HAL DEV0֧�ֵĿռ����� */
#define WLAN_HAL0_NSS_NUM WLAN_DOUBLE_NSS

/* HAL DEV0֧�ֵ������� FPGAֻ֧��80M */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_160M
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_20M
#endif
#else
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#if defined(_PRE_WLAN_110X_PILOT) && defined(_PRE_WLAN_FEATURE_160M)
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_160M
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_80M
#endif
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_40M
#endif
#endif

/* HAL DEV0 ֧��SOUNDING���� */
#define WLAN_HAL0_11N_SOUNDING OAL_TRUE

/* HAL DEV0 ֧��Green Field���� */
#define WLAN_HAL0_GREEN_FIELD OAL_TRUE

/* HAL DEV0�Ƿ�֧��խ�� */
#define WLAN_HAL0_NB_IS_EN OAL_FALSE

/* HAL DEV0�Ƿ�֧��1024QAM */
#define WLAN_HAL0_1024QAM_IS_EN OAL_TRUE
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)
/* HAL DEV0��SU_BFEE���� */
#define WLAN_HAL0_SU_BFEE_ANT_SUPPORT_NUM 8

/* HE SU BFEE֧�ֵĽ��տ�ʱ���� */
#define WLAN_HAL0_NTX_STS_BELOW_80M 8

/* HE SU BFEE֧�ֵĽ��տ�ʱ���� */
#define WLAN_HAL0_NTX_STS_OVER_80M 8
#else
/* HAL DEV0��SU_BFEE���� */
#define WLAN_HAL0_SU_BFEE_ANT_SUPPORT_NUM 4

/* HE SU BFEE֧�ֵĽ��տ�ʱ���� */
#define WLAN_HAL0_NTX_STS_BELOW_80M 4

/* HE SU BFEE֧�ֵĽ��տ�ʱ���� */
#define WLAN_HAL0_NTX_STS_OVER_80M 4
#endif

/* HAL DEV0��SU_BFER���� */
#define WLAN_HAL0_NUM_DIM_BELOW_80M 1

/* HAL DEV0��SU_BFER���� */
#define WLAN_HAL0_NUM_DIM_OVER_80M 1
/* HAL DEV0�Ƿ�֧��11MC */
#define WLAN_HAL0_11MC_IS_EN OAL_FALSE

/* HAL DEV0��ͨ��ѡ�� */
#define WLAN_HAL0_PHY_CHAIN_SEL WLAN_PHY_CHAIN_DOUBLE

/* HAL DEV0����Ҫ�õ����߷���11b��֡ʱ��TXͨ��ѡ�� */
#define WLAN_HAL0_SNGL_TX_CHAIN_SEL WLAN_TX_CHAIN_ZERO

/* HAL DEV0��RFͨ��ѡ�� */
#define WLAN_HAL0_RF_CHAIN_SEL WLAN_RF_CHAIN_DOUBLE

/* HAL DEV0�Ƿ�support 2.4g dpd */
#define WLAN_HAL0_DPD_2G_IS_EN OAL_FALSE

/* HAL DEV0�Ƿ�support 5g dpd */
#define WLAN_HAL0_DPD_5G_IS_EN OAL_FALSE

/* HAL DEV0�Ƿ�support tx stbc, su/mu txbfer */
/* ��ǰdouble nss�µ�������ASIC���壬FPGA��ͨ��˽�ж��ƻ��ļ�ini������ˢ�� */
#define WLAN_HAL0_TX_STBC_IS_EN OAL_TRUE
#define WLAN_HAL0_SU_BFER_IS_EN OAL_TRUE
#define WLAN_HAL0_MU_BFER_IS_EN OAL_FALSE

/* HAL DEV0�Ƿ�support rx stbc, su/mu txbfee */
/* ��ǰ������ASIC�����������壬FPGA��ͨ��˽�ж��ƻ��ļ�ini������ˢ�� */
#define WLAN_HAL0_RX_STBC_IS_EN                    OAL_TRUE
#define WLAN_HAL0_SU_BFEE_IS_EN                    OAL_TRUE
#define WLAN_HAL0_MU_BFEE_IS_EN                    OAL_FALSE
#define WLAN_HAL0_11N_TXBF_IS_EN                   OAL_FALSE
#define WLAN_HAL0_CONTROL_FRM_TX_DOUBLE_CHAIN_FLAG OAL_FALSE

#define WLAN_HAL0_LDPC_IS_EN OAL_TRUE
#ifdef _PRE_WLAN_110X_PILOT
#define WLAN_HAL0_11AX_IS_EN OAL_TRUE
#else
#define WLAN_HAL0_11AX_IS_EN OAL_FALSE
#endif
#define WLAN_HAL0_DPD_IS_EN OAL_TRUE

#define WLAN_HAL0_RADAR_DETECTOR_IS_EN OAL_TRUE
/* HAL DEV0֧��TXOP PS */
#define WLAN_HAL0_TXOPPS_IS_EN OAL_TRUE

/* SU BFER֧�ֵķ��Ϳ�ʱ��������·Ĭ��֧��2�� */
#define WLAN_HAL0_NUMBER_SOUNDING_DIM 1

/* HAL DEV0�Ƿ�֧��160M��80+80M��short GI */
#ifdef _PRE_WLAN_FEATURE_160M
#define WLAN_HAL0_VHT_SGI_SUPP_160_80P80 OAL_TRUE
#else
#define WLAN_HAL0_VHT_SGI_SUPP_160_80P80 OAL_FALSE
#endif

/* оƬ�汾�ѷ���ƽ̨�Ķ��ƻ� */
/* 2.8.2 STA AP��� */
/* ͨ���� */
#define WLAN_RF_CHANNEL_NUMS 2

/* ���ͨ���� */
#define WLAN_RF_CHANNEL_MAX_NUMS 4

/* AP VAP�Ĺ��STA VAP�Ĺ��STA P2P����Ĺ�����ƽ̨ */
/* PROXY STAģʽ��VAP���궨�����ƽ̨ */
/* ÿ��device֧��vap���������ѷ���ƽ̨ */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
#define WLAN_HAL_OHTER_BSS_ID            0x1F  /* 06 rx cb vap id 5bit ȫ1��ʾ����bSS */
#else
#define WLAN_HAL_OHTER_BSS_ID 14 /* ����BSS�Ĺ㲥֡�ϱ�hal vap ID */
#define WLAN_HAL_OHTER_BSS_MC_ID 15 /* ����BSS���鲥֡�ϱ�hal vap ID */
#endif
#define WLAN_HAL_OTHER_BSS_UCAST_ID      7  /* ��������bss�ĵ�������֡������֡��ά���� */

/* ����BOARD֧�ֵ�����device��Ŀ����ƽ̨ */
/* ����BOARD֧�ֵ�����VAP��Ŀ�ѷ���ƽ̨ */
/* ����BOARD֧�ֵ����ҵ��VAP����Ŀ �ѷ���ƽ̨ */

/* 2.8.7 ����Ĭ�Ͽ����رն��� */
/* TBD Feature��̬��ǰδʹ�ã�����������û��ʹ�� */
#define WLAN_FEATURE_UAPSD_IS_OPEN       OAL_TRUE

/* 2.9.18 Proxy STA���� */
/* 2.10 MAC FRAME���� */
/* 2.10.1 ht cap info */
#define HT_GREEN_FILED_DEFAULT_VALUE 0
#define HT_TX_STBC_DEFAULT_VALUE     0
#define HT_BFEE_NTX_SUPP_ANTA_NUM    4 /* 11n֧�ֵ�bfer����soundingʱ��������߸��� */
/* 2.10.2 vht cap info */
#define VHT_TX_STBC_DEFAULT_VALUE 0
#define VHT_BFEE_NTX_SUPP_STS_CAP 4 /* Э���б�ʾ������NDP Nsts���� */

/* 2.10.3 RSSI */
#define WLAN_NEAR_DISTANCE_RSSI              (-20) /* Ĭ�Ͻ������ź�����-35dBm */
#define WLAN_NEAR_DISTANCE_ADJUST_RSSI       15    /* ������Ĭ��У׼15dB */
#define WLAN_CLOSE_DISTANCE_RSSI             (-15) /* ����ǰ�����ж�����-25dBm */
#define WLAN_FAR_DISTANCE_RSSI               (-73) /* Ĭ��Զ�����ź�����-73dBm */
#define WLAN_NORMAL_DISTANCE_RSSI_UP         (-42) /* �ź�ǿ��С��-42dBmʱ������Ϊ�ǳ������� */
#define WLAN_NORMAL_DISTANCE_RSSI_DOWN       (-66) /* �ź�ǿ�ȴ���-66dBmʱ������Ϊ�Ƿǳ�Զ���� */
#define WLAN_FIX_MAX_POWER_RSSI              (-55) /* �̶�������ź����� */
#define WLAN_RSSI_ADJUST_TH                  (-88) /* ����ϱ�RSSI�ĵ�����ֵ, ���ڸ���ֵrssi-=2 */
#define WLAN_LOW_RATE_OPEN_NOISEEST_TH       200   /* ÿ100ms�򿪶������������յ������ʰ��ĸ��� */
#define WLAN_LOW_RATE_OPEN_MLD_TH            100   /* ÿ100ms�յ������ʰ���MLD�㷨������ */
#define WLAN_LOW_RATE_OPEN_MLD_COUNT         10    /* ÿ100ms�յ������ʰ���MLD�㷨�Ĵ������� */
#define WLAN_MIRACAST_DEFAULT_RSSI           (-58) /* miracast�����ϲ��·�probe respʱ��ʹ�õ�Ĭ��rssi,�������� */

#define WLAN_LOW_RATE_DEC_AGC_TH    100 /* ÿ100ms�յ������ʰ�����agc target������ */
#define WLAN_LOW_RATE_DEC_AGC_COUNT 3   /* ÿ100ms�յ������ʰ�����agc target�Ĵ������� */
#define WLAN_DEC_AGC_INTF_RSSI_MAX  (-57)
#define WLAN_DEC_AGC_INTF_RSSI_MIN  (-80)

#define WLAN_PHY_EXTLNA_CHGPTDBM_TH          (-25) /* �����Թ��أ��޸��л�����Ϊ-25dBm */
#define WLAN_PHY_EXTLNA_CHGPTDBM_TH_VAL(_en_band, _en_pm_flag) ( \
    ((_en_pm_flag) == OAL_FALSE) ? (WLAN_PHY_EXTLNA_CHGPTDBM_TH) : (((_en_band) == WLAN_BAND_2G) ? (-50) : (-40)))

/* mib��ʼ�� */
#define HE_BFEE_NTX_SUPP_STS_CAP_EIGHT 8 /* 1105��ʾ������NDP Nsts���� */
#define HE_BFEE_NTX_SUPP_STS_CAP_FOUR  4 /* 1103��ʾ������NDP Nsts���� */

/* 2.11 ������ƫ�� */
#define WLAN_RX_DSCR_SIZE WLAN_MEM_SHARED_RX_DSCR_SIZE  // ʵ�ʽ�����������С


/* 2.13 Calibration FEATURE spec */
/* У׼�����ϴ��·�MASK */
#define CALI_DATA_REFRESH_MASK  BIT0
#define CALI_DATA_REUPLOAD_MASK BIT1
#define CALI_POWER_LVL_DBG_MASK BIT3
#define CALI_MIMO_MASK          (BIT18 | BIT17 | BIT16)
#define CALI_HI1106_MIMO_MASK   (BIT15 | BIT13 | BIT9)
#define CALI_MUL_TIME_CALI_MASK BIT30
#define CALI_INTVL_MASK         0xe0
#define CALI_INTVL_OFFSET       5

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_spec.h */

