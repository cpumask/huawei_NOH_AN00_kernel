

#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__

/* 1 ͷ�ļ����� */
#include "oal_types.h"
#include "wlan_spec.h"
#include "wlan_types.h"
#include "wlan_cali_1106.h"
#include "frw_ext_if.h"
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hiex_msg.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_COMMON_H
#ifdef _PRE_WLAN_FEATURE_BT_20DBM
/* Ϊ�˷�ֹ����20dbmǶ�����û�б���ס�����������⣬�����ӿ�������Ҫͬ����������bitmap */
typedef enum {
    HAL_BTCOEX_RF_20DBM_OCCU_TYPE_SET_RF = BIT0,
    HAL_BTCOEX_RF_20DBM_OCCU_TYPE_WORK_ENTRY = BIT1,
    HAL_BTCOEX_RF_20DBM_OCCU_TYPE_WIFI_SW = BIT2,
    HAL_BTCOEX_RF_20DBM_OCCU_TYPE_MODE_SELECT = BIT3,
    HAL_BTCOEX_RF_20DBM_OCCU_TYPE_BUTT
} hal_btcoex_rf_20dbm_occu_type_enum;
typedef uint8_t hal_btcoex_rf_20dbm_occu_enum_uint8;
#endif
/* HAL_DEVICE_WORK_STATE��״̬ */
typedef enum {
    HAL_DEVICE_WORK_SUB_STATE_ACTIVE = 0, /* active��״̬ */
    HAL_DEVICE_WORK_SUB_STATE_AWAKE = 1,  /* awake��״̬ */
    HAL_DEVICE_WORK_SUB_STATE_DOZE = 2,   /* doze��״̬ */
    HAL_DEVICE_WORK_SUB_STATE_INIT = 3,   /* init��״̬ */
    HAL_DEVICE_WORK_SUB_STATE_BUTT
} hal_device_work_sub_state_info;
#define HAL_WORK_SUB_STATE_NUM (HAL_DEVICE_WORK_SUB_STATE_INIT - HAL_DEVICE_WORK_SUB_STATE_ACTIVE)

/*****************************************************************************/
/*****************************************************************************/
/*                        �����궨�塢ö�١��ṹ��                           */
/*****************************************************************************/
/*****************************************************************************/
#define MAX_MAC_AGGR_MPDU_NUM  256
#define AL_TX_MSDU_NETBUF_MAX_LEN 2000

#define HAL_POW_11B_RATE_NUM 4 /* 11b������Ŀ */
#define HAL_POW_11G_RATE_NUM 8 /* 11g������Ŀ */
#define HAL_POW_11A_RATE_NUM 8 /* 11a������Ŀ */

#ifdef _PRE_WLAN_FEATURE_11AC_20M_MCS9
#ifdef _PRE_WLAN_FEATURE_1024QAM
#define HAL_POW_11AC_20M_NUM 12 /* 11n_11ac_2g������Ŀ */
#else
#define HAL_POW_11AC_20M_NUM 10 /* 11n_11ac_2g������Ŀ */
#endif
#else
#define HAL_POW_11AC_20M_NUM 9 /* 11n_11ac_2g������Ŀ */
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM
#define HAL_POW_11AC_40M_NUM 13 /* 11n_11ac_2g������Ŀ */
#define HAL_POW_11AC_80M_NUM 12 /* 11n_11ac_2g������Ŀ */
#else
#define HAL_POW_11AC_40M_NUM 11 /* 11n_11ac_2g������Ŀ */
#define HAL_POW_11AC_80M_NUM 10 /* 11n_11ac_2g������Ŀ */
#endif

#ifdef _PRE_WLAN_FEATURE_160M
#ifdef _PRE_WLAN_FEATURE_1024QAM
#define HAL_POW_11AC_160M_NUM 12 /* 11n_11ac_2g������Ŀ */
#else
#define HAL_POW_11AC_160M_NUM 10 /* 11n_11ac_2g������Ŀ */
#endif
#else
#define HAL_POW_11AC_160M_NUM 0 /* 11n_11ac_2g������Ŀ(���ڴ�׮����HAL_POW_RATE_POW_CODE_TABLE_LEN) */
#endif

#define HAL_POW_RATE_CODE_TBL_LEN (HAL_POW_11B_RATE_NUM + HAL_POW_11G_RATE_NUM + \
    HAL_POW_11AC_20M_NUM + HAL_POW_11AC_40M_NUM + \
    HAL_POW_11AC_80M_NUM + HAL_POW_11AC_160M_NUM) /* rate-tpccode table�����ʸ��� */

#define HAL_POW_CUSTOM_24G_11B_RATE_NUM        2 /* ���ƻ�11b������Ŀ */
#define HAL_POW_CUSTOM_11G_11A_RATE_NUM        5 /* ���ƻ�11g/11a������Ŀ */
#define HAL_POW_CUSTOM_HT20_VHT20_RATE_NUM     6 /* ���ƻ�HT20_VHT20������Ŀ */
#define HAL_POW_CUSTOM_24G_HT40_VHT40_RATE_NUM 8
#define HAL_POW_CUSTOM_5G_HT40_VHT40_RATE_NUM  7
#define HAL_POW_CUSTOM_5G_VHT80_RATE_NUM       6
/* ���ƻ�ȫ������ */
#define HAL_POW_CUSTOM_MCS9_10_11_RATE_NUM     3
#define HAL_POW_CUSTOM_MCS10_11_RATE_NUM       2
#define HAL_POW_CUSTOM_5G_VHT160_RATE_NUM      12 /* ���ƻ�5G_11ac_VHT160������Ŀ */
#define HAL_POW_CUSTOM_HT20_VHT20_DPD_RATE_NUM 5  /* ���ƻ�DPD������Ŀ */
#define HAL_POW_CUSTOM_HT40_VHT40_DPD_RATE_NUM 5
/* ���ƻ���غ� */
/* NVRAM�д洢�ĸ�Э����������书�ʲ����ĸ��� From:24G_11b_1M To:5G_VHT80_MCS7 */
#define NUM_OF_NV_NORMAL_MAX_TXPOWER (HAL_POW_CUSTOM_24G_11B_RATE_NUM +                                            \
                                      HAL_POW_CUSTOM_11G_11A_RATE_NUM + HAL_POW_CUSTOM_HT20_VHT20_RATE_NUM +       \
                                      HAL_POW_CUSTOM_24G_HT40_VHT40_RATE_NUM + HAL_POW_CUSTOM_11G_11A_RATE_NUM +   \
                                      HAL_POW_CUSTOM_HT20_VHT20_RATE_NUM + HAL_POW_CUSTOM_5G_HT40_VHT40_RATE_NUM + \
                                      HAL_POW_CUSTOM_5G_VHT80_RATE_NUM)
#define NUM_OF_NV_MAX_TXPOWER (NUM_OF_NV_NORMAL_MAX_TXPOWER + \
                               HAL_POW_CUSTOM_MCS9_10_11_RATE_NUM * 4 + \
                               HAL_POW_CUSTOM_5G_VHT160_RATE_NUM + \
                               HAL_POW_CUSTOM_MCS10_11_RATE_NUM)

#define NUM_OF_NV_DPD_MAX_TXPOWER (HAL_POW_CUSTOM_HT20_VHT20_DPD_RATE_NUM + HAL_POW_CUSTOM_HT40_VHT40_DPD_RATE_NUM)
#define NUM_OF_NV_11B_DELTA_TXPOWER      2
#define NUM_OF_NV_5G_UPPER_UPC           4
#define NUM_OF_IQ_CAL_POWER              2
#define NUM_OF_NV_2G_LOW_POW_DELTA_VAL   4
#define NUM_OF_NV_5G_LPF_LVL             4
/* TPC��λ���� */
#define HAL_POW_LEVEL_NUM          5                              /* �㷨�ܵ�λ��Ŀ */

/* ÿ���û�֧�ֵ�������ʼ����� */
#define HAL_TX_RATE_MAX_NUM 4

typedef enum {
    HAL_FCS_PROTECT_TYPE_NONE = 0,  /* NONE        */
    HAL_FCS_PROTECT_TYPE_SELF_CTS,  /* SELF CTS    */
    HAL_FCS_PROTECT_TYPE_NULL_DATA, /* NULL DATA   */

    HAL_FCS_PROTECT_TYPE_BUTT
} hal_fcs_protect_type_enum;
typedef uint8_t hal_fcs_protect_type_enum_uint8;

typedef enum {
    HAL_FCS_SERVICE_TYPE_DBAC = 0,      /* DBACҵ��    */
    HAL_FCS_SERVICE_TYPE_SCAN,          /* ɨ��ҵ��    */
    HAL_FCS_SERVICE_TYPE_M2S,           /* m2s�л�ҵ�� */
    HAL_FCS_SERVICE_TYPE_BTCOEX_NORMAL, /* btcoex����ҵ�� */
    HAL_FCS_SERVICE_TYPE_BTCOEX_LDAC,   /* btcoex����ҵ�� */

    HAL_FCS_PROTECT_NOTIFY_BUTT
} hal_fcs_service_type_enum;
typedef uint8_t hal_fcs_service_type_enum_uint8;

typedef enum {
    HAL_FCS_PROTECT_COEX_PRI_NORMAL = 0,   /* b00 */
    HAL_FCS_PROTECT_COEX_PRI_PRIORITY = 1, /* b01 */
    HAL_FCS_PROTECT_COEX_PRI_OCCUPIED = 2, /* b10 */

    HAL_FCS_PROTECT_COEX_PRI_BUTT
} hal_fcs_protect_coex_pri_enum;
typedef uint8_t hal_fcs_protect_coex_pri_enum_uint8;

typedef enum {
    HAL_FCS_PROTECT_CNT_0 = 0,
    HAL_FCS_PROTECT_CNT_1 = 1,
    HAL_FCS_PROTECT_CNT_2 = 2,
    HAL_FCS_PROTECT_CNT_3 = 3,
    HAL_FCS_PROTECT_CNT_6 = 6,
    HAL_FCS_PROTECT_CNT_10 = 10,
    HAL_FCS_PROTECT_CNT_20 = 20,

    HAL_FCS_PROTECT_CNT_BUTT
} hal_fcs_protect_cnt_enum;
typedef uint8_t hal_fcs_protect_cnt_enum_uint8;

/* 3.3 ���������ö�ٶ��� */
/* tx_desc_protocol_mode����������,��5x�������޸� */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
typedef enum {
    WLAN_11B_PHY_PROTOCOL_MODE = 0,                                          /* 11b CCK */
    WLAN_PHY_PROTOCOL_MODE_11B = WLAN_11B_PHY_PROTOCOL_MODE,
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE = 1,                                  /* 11g/a OFDM */
    WLAN_PHY_PROTOCOL_MODE_LEGACY_OFDM = WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,
    WLAN_HT_PHY_PROTOCOL_MODE = 2,
    WLAN_PHY_PROTOCOL_MODE_HT_MIXED_MODE = WLAN_HT_PHY_PROTOCOL_MODE,        /* 11n HT Mixed */
    WLAN_HT_GREEN_PHY_PROTOCOL_MODE = 3,
    WLAN_PHY_PROTOCOL_MODE_HT_GREEN_FIELD = WLAN_HT_GREEN_PHY_PROTOCOL_MODE, /* 11n HT Green */
    WLAN_VHT_PHY_PROTOCOL_MODE = 4,                                          /* 11ac VHT */
    WLAN_PHY_PROTOCOL_MODE_VHT = WLAN_VHT_PHY_PROTOCOL_MODE,
    WLAN_PHY_PROTOCOL_MODE_BA = 5,                                           /* 11BA */
    WLAN_HE_SU_FORMAT_MODE = 8,                                              /* 11ax SU PPDU DL & UL */
    WLAN_PHY_PROTOCOL_MODE_HE_SU = WLAN_HE_SU_FORMAT_MODE,
    WLAN_HE_MU_FORMAT_MODE = 9,                                              /* 11ax MU PPDU DL */
    WLAN_PHY_PROTOCOL_MODE_HE_MU = WLAN_HE_MU_FORMAT_MODE,
    WLAN_HE_EXT_SU_FORMAT_MODE = 10,
    WLAN_PHY_PROTOCOL_MODE_HE_ER_SU = WLAN_HE_EXT_SU_FORMAT_MODE,            /* 11ax ER SU PPDU DL & UL */
    WLAN_HE_TRIG_FORMAT_MODE = 11,
    WLAN_PHY_PROTOCOL_MODE_HE_TRIGGER = WLAN_HE_TRIG_FORMAT_MODE,            /* 11ax Trigger UL TB PPDU */
    WLAN_PHY_PROTOCOL_BUTT = 12,
} wlan_phy_protocol_enum;
typedef uint8_t wlan_phy_protocol_enum_uint8;
typedef uint8_t wlan_phy_protocol_mode_enum_uint8;
#else
typedef enum {
    WLAN_11B_PHY_PROTOCOL_MODE = 0,                                          /* 11b CCK */
    WLAN_PHY_PROTOCOL_MODE_11B = WLAN_11B_PHY_PROTOCOL_MODE,
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE = 1,                                  /* 11g/a OFDM */
    WLAN_PHY_PROTOCOL_MODE_LEGACY_OFDM = WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,
    WLAN_HT_PHY_PROTOCOL_MODE = 2,
    WLAN_PHY_PROTOCOL_MODE_HT_MIXED_MODE = WLAN_HT_PHY_PROTOCOL_MODE,        /* 11n HT Mixed */
    WLAN_VHT_PHY_PROTOCOL_MODE = 3,                                          /* 11ac VHT */
    WLAN_PHY_PROTOCOL_MODE_VHT = WLAN_VHT_PHY_PROTOCOL_MODE,
#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)
    WLAN_HE_SU_FORMAT_MODE = 0,                                              /* 11ax SU PPDU DL & UL */
    WLAN_PHY_PROTOCOL_MODE_HE_SU = WLAN_HE_SU_FORMAT_MODE,
    WLAN_HE_MU_FORMAT_MODE = 1,                                              /* 11ax MU PPDU DL */
    WLAN_PHY_PROTOCOL_MODE_HE_MU = WLAN_HE_MU_FORMAT_MODE,
    WLAN_HE_EXT_SU_FORMAT_MODE = 2,
    WLAN_PHY_PROTOCOL_MODE_HE_ER_SU = WLAN_HE_EXT_SU_FORMAT_MODE,            /* 11ax ER SU PPDU DL & UL */
    WLAN_HE_TRIG_FORMAT_MODE = 3,
    WLAN_PHY_PROTOCOL_MODE_HE_TRIGGER = WLAN_HE_TRIG_FORMAT_MODE,            /* 11ax Trigger UL TB PPDU */
#endif
    WLAN_PHY_PROTOCOL_BUTT = 4,
} wlan_phy_protocol_enum;
typedef uint8_t wlan_phy_protocol_enum_uint8;
typedef uint8_t wlan_phy_protocol_mode_enum_uint8;

#endif

typedef enum {
    WLAN_11B_1_M_BPS = 0,
    WLAN_11B_2_M_BPS = 1,
    WLAN_11B_5_HALF_M_BPS = 2,
    WLAN_11B_11_M_BPS = 3,

    WLAN_11B_BUTT,
} wlan_11b_rate_index_enum;
typedef oal_uint8 wlan_11b_rate_index_enum_uint8;

typedef enum {
    WLAN_11AG_6M_BPS = 0,
    WLAN_11AG_9M_BPS = 1,
    WLAN_11AG_12M_BPS = 2,
    WLAN_11AG_18M_BPS = 3,
    WLAN_11AG_24M_BPS = 4,
    WLAN_11AG_36M_BPS = 5,
    WLAN_11AG_48M_BPS = 6,
    WLAN_11AG_54M_BPS = 7,

    WLAN_11AG_BUTT,
} wlan_11ag_rate_value_enum;
typedef oal_uint8 wlan_legacy_ofdm_mcs_enum_uint8;

typedef enum {
    HAL_TX_RATE_RANK_0 = 0,
    HAL_TX_RATE_RANK_1,
    HAL_TX_RATE_RANK_2,
    HAL_TX_RATE_RANK_3,

    HAL_TX_RATE_RANK_BUTT
} hal_tx_rate_rank_enum;
typedef uint8_t hal_tx_rate_rank_enum_uint8;
typedef enum {
    HAL_DFS_RADAR_TYPE_NULL = 0,
    HAL_DFS_RADAR_TYPE_FCC = 1,
    HAL_DFS_RADAR_TYPE_ETSI = 2,
    HAL_DFS_RADAR_TYPE_MKK = 3,
    HAL_DFS_RADAR_TYPE_KOREA = 4,

    HAL_DFS_RADAR_TYPE_BUTT
} hal_dfs_radar_type_enum;
typedef uint8_t hal_dfs_radar_type_enum_uint8;

typedef enum {
    HAL_RX_NEW = 0x0,
    HAL_RX_SUCCESS = 0x1,
    HAL_RX_DUP_DETECTED = 0x2,
    HAL_RX_FCS_ERROR = 0x3,
    HAL_RX_KEY_SEARCH_FAILURE = 0x4,
    HAL_RX_CCMP_MIC_FAILURE = 0x5,
    HAL_RX_ICV_FAILURE = 0x6,
    HAL_RX_TKIP_REPLAY_FAILURE = 0x7,
    HAL_RX_CCMP_REPLAY_FAILURE = 0x8,
    HAL_RX_TKIP_MIC_FAILURE = 0x9,
    HAL_RX_BIP_MIC_FAILURE = 0xA,
    HAL_RX_BIP_REPLAY_FAILURE = 0xB,
    HAL_RX_MUTI_KEY_SEARCH_FAILURE = 0xC /* �鲥�㲥 */
} hal_rx_status_enum;
typedef uint8_t hal_rx_status_enum_uint8;

#define MAC_RX_DSCR_NEED_DROP(status) (HAL_RX_SUCCESS != (status))

/* ��������������״̬ */
typedef enum {
    HAL_DSCR_QUEUE_INVALID = 0,
    HAL_DSCR_QUEUE_VALID,
    HAL_DSCR_QUEUE_SUSPENDED,

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    HAL_DSCR_QUEUE_BUSY,
    HAL_DSCR_QUEUE_IDLE,
#endif

    HAL_DSCR_QUEUE_STATUS_BUTT
} hal_dscr_queue_status_enum;
typedef uint8_t hal_dscr_queue_status_enum_uint8;

/* �������������к� */
typedef enum {
    HAL_RX_DSCR_NORMAL_PRI_QUEUE = 0,
    HAL_RX_DSCR_HIGH_PRI_QUEUE,
    HAL_RX_DSCR_SMALL_QUEUE,

    HAL_RX_DSCR_QUEUE_ID_BUTT
} hal_rx_dscr_queue_id_enum;
typedef uint8_t hal_rx_dscr_queue_id_enum_uint8;

typedef enum {
    HAL_WLAN_DDR_DRX_EVENT_SUBTYPE,    /* DDR DRX ���� */
    HAL_WLAN_DDR_DRX_EVENT_SUB_TYPE_BUTT
} hal_wlan_ddr_drx_event_enum;

/* HALģ����Ҫ�׳���WLAN_DRX�¼������͵Ķ���
 ˵��:��ö����Ҫ��dmac_wlan_drx_event_sub_type_enum_uint8ö��һһ��Ӧ */
typedef enum {
    HAL_WLAN_DRX_EVENT_SUB_TYPE_RX, /* WLAN DRX ���� */
    HAL_WLAN_DRX_EVENT_SUB_TYPE_RX_WOW,

    HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT
} hal_wlan_drx_event_sub_type_enum;
typedef uint8_t hal_wlan_drx_event_sub_type_enum_uint8;

/* HALģ����Ҫ�׳���WLAN_CRX�¼������͵Ķ���
   ˵��:��ö����Ҫ��dmac_wlan_crx_event_sub_type_enum_uint8ö��һһ��Ӧ */
typedef enum {
    HAL_WLAN_CRX_EVENT_SUB_TYPE_RX, /* WLAN CRX ���� */
#ifdef _PRE_WLAN_FEATURE_FTM
    HAL_EVENT_DMAC_FTM_IRQ, /* FTM�ж� */
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    HAL_WLAN_CRX_EVENT_SUB_RPT_HE_ROM,
#endif
    HAL_WLAN_CRX_EVENT_SUB_TYPE_RX_PPDU,

    HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT
} hal_wlan_crx_event_sub_type_enum;
typedef uint8_t hal_wlan_crx_event_sub_type_enum_uint8;

typedef enum {
    HAL_TX_COMP_SUB_TYPE_TX,
    HAL_TX_COMP_SUB_TYPE_AL_TX,
    HAL_TX_COMP_SUB_TYPE_BA_INFO,

    HAL_TX_COMP_SUB_TYPE_BUTT
} hal_tx_comp_sub_type_enum;
typedef uint8_t hal_tx_comp_sub_type_enum_uint8;

typedef enum {
    HAL_EVENT_TBTT_SUB_TYPE,

    HAL_EVENT_TBTT_SUB_TYPE_BUTT
} hal_event_tbtt_sub_type_enum;
typedef uint8_t hal_event_tbtt_sub_type_enum_uint8;
/* оƬ�����ֶζ��� */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
typedef enum {
    WLAN_BAND_ASSEMBLE_20M = 0,
    WLAN_BAND_ASSEMBLE_40M = 1,
    WLAN_BAND_ASSEMBLE_80M = 2,
    WLAN_BAND_ASSEMBLE_160M = 3,

    /* ����4���������� */
    WLAN_BAND_ASSEMBLE_40M_DUP = 5,
    WLAN_BAND_ASSEMBLE_80M_DUP = 9,
    WLAN_BAND_ASSEMBLE_160M_DUP = 13,
    WLAN_BAND_ASSEMBLE_80M_80M = 15,

    WLAN_BAND_ASSEMBLE_AUTO = 16,

    WLAN_BAND_ASSEMBLE_BUTT = 17,

    /* axЭ��ģʽ��������չ���� */
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    WLAN_BAND_ASSEMBLE_242_RU = 6,    /* оƬ����Ϊ0 */
    WLAN_BAND_ASSEMBLE_106_RU_UP = 7, /* оƬ����Ϊ1 */
    WLAN_BAND_ASSEMBLE_ER_BUTT = 8,   /* оƬ����Ϊ2,�������� */
#endif

} hal_channel_assemble_enum;
typedef uint8_t hal_channel_assemble_enum_uint8;

typedef enum {
    WLAN_FREQ_BW_20M = WLAN_BAND_ASSEMBLE_20M,
    WLAN_FREQ_BW_SU_ER_242T = WLAN_FREQ_BW_20M,
    WLAN_FREQ_BW_40M = WLAN_BAND_ASSEMBLE_40M,
    WLAN_FREQ_BW_PRI20M_UPPER_106T = WLAN_FREQ_BW_40M,
    WLAN_FREQ_BW_80M = WLAN_BAND_ASSEMBLE_80M,
    WLAN_FREQ_BW_160M = WLAN_BAND_ASSEMBLE_160M,

    WLAN_FREQ_BW_BUTT,
} wlan_freq_bw_mode_enum;
typedef uint8_t wlan_freq_bw_mode_enum_uint8;

#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
#define HAL_TX_SET_DSCR_ER_OFFSET (WLAN_BAND_ASSEMBLE_242_RU - WLAN_BAND_ASSEMBLE_20M) /* д��оƬ�Ĵ���ƫ���� */
#endif

#else
typedef enum {
    WLAN_BAND_ASSEMBLE_20M = 0,

    WLAN_BAND_ASSEMBLE_40M = 4,
    WLAN_BAND_ASSEMBLE_40M_DUP = 5,

    WLAN_BAND_ASSEMBLE_80M = 8,
    WLAN_BAND_ASSEMBLE_80M_DUP = 9,

    WLAN_BAND_ASSEMBLE_160M = 12,
    WLAN_BAND_ASSEMBLE_160M_DUP = 13,

    WLAN_BAND_ASSEMBLE_80M_80M = 15,

    WLAN_BAND_ASSEMBLE_AUTO,

    WLAN_BAND_ASSEMBLE_BUTT
} hal_channel_assemble_enum;
typedef uint8_t hal_channel_assemble_enum_uint8;

#endif

typedef enum {
    WLAN_HE_RU_SIZE_26,
    WLAN_HE_RU_SIZE_52,
    WLAN_HE_RU_SIZE_106,
    WLAN_HE_RU_SIZE_242,
    WLAN_HE_RU_SIZE_484,
    WLAN_HE_RU_SIZE_996,
    WLAN_HE_RU_SIZE_1992,

    WLAN_HE_RU_SIZE_BUTT,
} wlan_he_rusize_enum;
typedef uint8_t wlan_he_rusize_enum_uint8;

/* 3.4 �ж����ö�ٶ��� */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || \
    (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
/* ��Ϊmac error��dmac misc���ȼ�һ�£�03��high prio��ʵʱ�¼�����������mac error����dmac misc */
/* 3.4.1 ʵʱ�¼��ж����� */
typedef enum {
    HAL_EVENT_DMAC_HIGH_PRIO_BTCOEX_PS,   /* BTCOEX ps�ж�, ��Ϊrom����Ŀǰֻ�ܷ���һ�� */
    HAL_EVENT_DMAC_HIGH_PRIO_BTCOEX_LDAC, /* BTCOEX LDAC�ж� */

    HAL_EVENT_DMAC_HIGH_PRIO_SUB_TYPE_BUTT
} hal_event_dmac_high_prio_sub_type_enum;
typedef uint8_t hal_event_dmac_high_prio_sub_type_enum_uint8;

#else
/* 3.4.1 оƬ�����ж����� */
typedef enum {
    HAL_EVENT_ERROR_IRQ_MAC_ERROR, /* MAC�����ж�ʱ�� */
    HAL_EVENT_ERROR_IRQ_SOC_ERROR, /* SOC�����ж��¼� */

    HAL_EVENT_ERROR_IRQ_SUB_TYPE_BUTT
} hal_event_error_irq_sub_type_enum;
typedef uint8_t hal_event_error_irq_sub_type_enum_uint8;
#endif

#define HAL_MAC_ERROR2_BASE 32  /* �����жϼĴ���2��base offset */

typedef enum {
    HAL_ALG_ISR_NOTIFY_DBAC,
    HAL_ALG_ISR_NOTIFY_MWO_DET,
    HAL_ALG_ISR_NOTIFY_ANTI_INTF,

    HAL_ALG_ISR_NOTIFY_BUTT,
} hal_alg_noify_enum;
typedef uint8_t hal_alg_noify_enum_uint8;

typedef enum {
    HAL_ISR_TYPE_TBTT,
    HAL_ISR_TYPE_ONE_PKT,
    HAL_ISR_TYPE_MWO_DET,
    HAL_ISR_TYPE_NOA_START,
    HAL_ISR_TYPE_NOA_END,

    HAL_ISR_TYPE_BUTT,
} hal_isr_type_enum;
typedef uint8_t hal_isr_type_enum_uint8;

/* ���ܲ������ */
typedef enum {
    HAL_ALWAYS_TX_DISABLE,      /* ���ó��� */
    HAL_ALWAYS_TX_RF,           /* ������RF���Թ㲥���� */
    HAL_ALWAYS_TX_AMPDU_ENABLE, /* ʹ��AMPDU�ۺϰ����� */
    HAL_ALWAYS_TX_MPDU,         /* ʹ�ܷǾۺϰ����� */
    HAL_ALWAYS_TX_BUTT
} hal_device_always_tx_state_enum;
typedef uint8_t hal_device_always_tx_enum_uint8;

typedef enum {
    HAL_ALWAYS_RX_DISABLE,      /* ���ó��� */
    HAL_ALWAYS_RX_RESERVED,     /* ������RF���Թ㲥���� */
    HAL_ALWAYS_RX_AMPDU_ENABLE, /* ʹ��AMPDU�ۺϰ����� */
    HAL_ALWAYS_RX_ENABLE,       /* ʹ�ܷǾۺϰ����� */
    HAL_ALWAYS_RX_BUTT
} hal_device_always_rx_state_enum;
typedef uint8_t hal_device_always_rx_enum_uint8;

#ifdef _PRE_WLAN_FEATURE_MONITOR
typedef enum {
    WLAN_SINFFER_OFF, /* snifferץ�����ܹر� */
    WLAN_SNIFFER_TRAVEL_CAP_ON, /* sniffer���г���ץ�����ܿ��� */
    WLAN_SINFFER_ON,  /* snifferץ�����ܿ��� */
    WLAN_SNIFFER_STATE_BUTT
} wlan_sniffer_state_enum;
typedef uint8_t wlan_sniffer_state_enum_uint8;

typedef enum {
    WLAN_MONITOR_OTA_HOST_RPT, /* host ota�ϱ� */
    WLAN_MONITOR_OTA_DEVICE_RPT,  /* device ota�ϱ� */
    WLAN_MONITOR_OTA_ALL_RPT,

    WLAN_MONITOR_OTA_RPT_BUTT
} wlan_monitor_ota_state_enum;
typedef uint8_t wlan_monitor_ota_mode_enum_uint8;

typedef enum {
    WLAN_MONITOR_OFF, /* monitorץ�����ܹر� */
    WLAN_MONITOR_ON,  /* monitorץ�����ܿ��� */
    WLAN_MONITOR_STATE_BUTT
} wlan_monitor_state_enum;
typedef uint8_t wlan_monitor_state_enum_uint8;

#define WLAN_SNIFFER_RX_Q_DSCR_TH 30

/* ����ץ����غ� */
#define HMAC_PKT_CAP_SIGNAL_OFFSET (-94)
#define HMAC_PKT_CAP_NOISE_MAX     0
#define HMAC_PKT_CAP_NOISE_MIN     (-100)
#define HMAC_PKT_CAP_RATE_UNIT     500

/* 2G��5G��Ĭ������ */
#define HMAC_PKT_CAP_2G_RATE 1000
#define HMAC_PKT_CAP_5G_RATE 6000

#define WLAN_2G_CENTER_FREQ_BASE 2407
#define WLAN_5G_CENTER_FREQ_BASE 5000

/* 8 bytes */
typedef struct {
    uint8_t it_version;  /* ʹ��radiotap header����Ҫ�汾, Ŀǰ����Ϊ0 */
    uint8_t it_pad;      /* Ŀǰ��δʹ��, ֻ��Ϊ��4�ֽڶ��� */
    uint16_t it_len;     /* radiotap�ĳ���, ������header+fields */
    uint32_t it_present; /* ͨ��bitλ����fields����Щ��Ա */
} ieee80211_radiotap_header_stru;

/* 30 bytes */
typedef struct {
    uint64_t ull_timestamp;    /* ��ǰ֡��ʱ���, ��λΪus */
    uint8_t uc_flags;          /* ��־λ */
    uint8_t uc_data_rate;      /* TX/RX��������, ��λΪ500Kbps */
    uint16_t us_channel_freq;  /* AP�����ŵ�������Ƶ��, ��λMHz */
    uint16_t us_channel_type;  /* �ŵ�����, ��ʶ5G����2G */
    int8_t c_ssi_signal;       /* �ź�ǿ��, ��λΪdBm */
    int8_t c_ssi_noise;

    union {
        uint8_t uc_mcs_info_flags;
        struct {
            uint8_t uc_mcs_info_rate;
            int16_t s_signal_quality;  /* �������岻��, ��Ʒ���ļ��㷽����RSSI + 94 */
            uint16_t us_vht_known; /* vht��Ϣ, 11acЭ��ʱ���ֶ���Ч */
            uint8_t uc_mcs_info_known; /* mcs��Ϣ, 11nЭ��ʱ���ֶ���Ч */
            uint8_t uc_vht_flags;
            uint8_t uc_mcs_info_flags;
            uint8_t uc_vht_bandwidth;
            uint8_t uc_vht_mcs_nss[4];
            uint8_t uc_vht_coding;
            uint8_t uc_vht_group_id;
            uint16_t us_vht_partial_aid;
        } st_legacy_wifi_info; /* radiotap info before WiFi5  */
        struct {
            uint16_t HE_info[6]; // 16+12 = 28
            uint16_t HE_MU_Info[2]; // +4
            uint8_t  HE_MU_RU_CH1[4]; // +4
            uint8_t  HE_MU_RU_CH2[4]; // +4
        } st_wifi6_info; /* RadioTap info of wifi6 */
    } extra_info;
} ieee80211_radiotap_fields_stru;

typedef struct {
    ieee80211_radiotap_header_stru st_radiotap_header; /* radiotapͷ�ṹ�� */
    ieee80211_radiotap_fields_stru st_radiotap_fields; /* radiotap����ṹ�� */
} ieee80211_radiotap_stru;

/* Radiotap��չ���ֵ�vht info���ӳ�Աbandwidth */
typedef enum {
    IEEE80211_RADIOTAP_VHT_BW_20 = 0,
    IEEE80211_RADIOTAP_VHT_BW_40 = 1,
    IEEE80211_RADIOTAP_VHT_BW_80 = 4,
    IEEE80211_RADIOTAP_VHT_BW_160 = 11,

    IEEE80211_RADIOTAP_VHT_BW_BUTT
} ieee80211_radiotap_vht_bandwidth;
typedef uint8_t ieee80211_radiotap_vht_bandwidth_uint8;

#define IEEE80211_RADIOTAP_F_SHORTGI 0x80 /* short gi */

/* ͬ1105��hal_rx_status_stru����deviceҪ����һ�� */
typedef struct {
    /* byte 0 */
    uint8_t bit_cipher_protocol_type : 4; /* ����֡�������� */
    uint8_t bit_dscr_status : 4;          /* ����״̬ */

    /* byte 1 */
    uint8_t bit_AMPDU : 1;
    uint8_t bit_last_mpdu_flag : 1; /* �̶�λ�� */
    uint8_t bit_gi_type : 2;
    uint8_t bit_he_ltf_type : 2;
    uint8_t bit_sounding_mode : 2;

    /* byte 2 */
    uint8_t bit_freq_bandwidth_mode : 3;
    uint8_t bit_rx_himit_flag : 1; /* rx himit flag = 1103 bit_preabmle */
    uint8_t bit_ext_spatial_streams : 2;
    uint8_t bit_smoothing : 1;
    uint8_t bit_fec_coding : 1; /* channel code */

    /* byte 3-4 */
    union {
        struct {
            uint8_t bit_rate_mcs : 4;
            uint8_t bit_nss_mode : 2;

            uint8_t bit_protocol_mode : 4;
            uint8_t bit_is_rx_vip : 1; /* place dcm bit */
            uint8_t bit_rsp_flag : 1;  /* beaformingʱ���Ƿ��ϱ��ŵ�����ı�ʶ��0:�ϱ���1:���ϱ� */
            uint8_t bit_mu_mimo_flag : 1;
            uint8_t bit_ofdma_flag : 1;
            uint8_t bit_beamforming_flag : 1; /* ����֡�Ƿ�����beamforming */
            uint8_t bit_STBC : 1;
        } st_rate; /* 11a/b/g/11ac/11ax�����ʼ����� */
        struct {
            uint8_t bit_ht_mcs : 6;
            uint8_t bit_protocol_mode : 4;
            uint8_t bit_is_rx_vip : 1; /* place dcm bit */
            uint8_t bit_rsp_flag : 1;  /* beaformingʱ���Ƿ��ϱ��ŵ�����ı�ʶ��0:�ϱ���1:���ϱ� */
            uint8_t bit_mu_mimo_flag : 1;
            uint8_t bit_ofdma_flag : 1;
            uint8_t bit_beamforming_flag : 1; /* ����֡�Ƿ�����beamforming */
            uint8_t bit_STBC : 1;
        } st_ht_rate; /* 11n�����ʼ����� */
    } un_nss_rate;
} hal_sniffer_rx_status_stru;

typedef struct {
    /* byte 0 */
    int8_t c_rssi_dbm;

    /* byte 1-4 */
    uint8_t uc_code_book;
    uint8_t uc_grouping;
    uint8_t uc_row_number;

    /* byte 5-6 */
    int8_t c_snr_ant0; /* ant0 SNR */
    int8_t c_snr_ant1; /* ant1 SNR */

    /* byte 7-8 */
    int8_t c_ant0_rssi; /* ANT0�ϱ���ǰ֡RSSI */
    int8_t c_ant1_rssi; /* ANT1�ϱ���ǰ֡RSSI */

} hal_sniffer_rx_statistic_stru;

#define SNIFFER_RX_INFO_SIZE                                                              \
    (OAL_SIZEOF(hal_sniffer_rx_status_stru) + OAL_SIZEOF(hal_sniffer_rx_statistic_stru) + \
        OAL_SIZEOF(uint32_t) + OAL_SIZEOF(hal_statistic_stru))
#endif
/* 3.6 �������ö�ٶ��� */
/* 3.6.1  оƬ��Կ���Ͷ��� */
typedef enum {
    HAL_KEY_TYPE_TX_GTK = 0, /* Hi1102:HAL_KEY_TYPE_TX_IGTK */
    HAL_KEY_TYPE_PTK = 1,
    HAL_KEY_TYPE_RX_GTK = 2,
    HAL_KEY_TYPE_RX_GTK2 = 3, /* 02ʹ�ã�03��51��ʹ�� */
    HAL_KEY_TYPE_BUTT
} hal_cipher_key_type_enum;
typedef uint8_t hal_cipher_key_type_enum_uint8;

#define HAL_FCS_PROT_MAX_FRAME_LEN  24
typedef struct {
    hal_fcs_protect_type_enum_uint8 en_protect_type;
    hal_fcs_protect_cnt_enum_uint8 en_protect_cnt;
    uint16_t bit_protect_coex_pri : 2; /* btcoex��ʹ�ã�one pkt�������ȼ� */
    uint16_t bit_cfg_one_pkt_tx_vap_index : 4;
    uint16_t bit_cfg_one_pkt_tx_peer_index : 5;
    uint16_t bit_rsv : 5;
    uint32_t ul_tx_mode;
    uint32_t ul_tx_data_rate;
    uint16_t us_duration; /* ��λ us */
    uint16_t us_timeout;
    uint16_t us_wait_timeout; /* �����ʱ����ʱʱ�� */
    uint8_t auc_rsv[1];
    uint8_t mac_vap_id;
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
    uint8_t onepkt_dscr[WLAN_MEM_SHARED_TX_DSCR_SIZE1]; /* ���ڴ��onepkt֡���������� */
#endif
    uint8_t auc_protect_frame[HAL_FCS_PROT_MAX_FRAME_LEN];
} hal_one_packet_cfg_stru;

typedef struct {
    oal_bool_enum_uint8 en_mac_in_one_pkt_mode : 1;
    oal_bool_enum_uint8 en_self_cts_success : 1;
    oal_bool_enum_uint8 en_null_data_success : 1;
    uint32_t ul_resv : 5;
} hal_one_packet_status_stru;
/*****************************************************************************
  �ṹ��  : hal_rate_pow_code_gain_tbl_stru
  �ṹ˵��: HALģ��POW Code��ṹ
*****************************************************************************/
typedef struct {
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || \
    (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
    uint32_t aul_pow_code_level[HAL_POW_LEVEL_NUM];
#else
    uint8_t auc_pow_code_level[HAL_POW_LEVEL_NUM];
#endif
    int16_t as_pow_gain_level[HAL_POW_LEVEL_NUM];
} hal_rate_pow_code_gain_tbl_stru;
/* 7.5 ���ⲿ�����ṩ�ӿ��������ݽṹ */
typedef struct {
    /* byte 0 */
    int8_t c_rssi_dbm;

    /* byte 1 */
#if defined(_PRE_WLAN_FEATURE_TXBF_HW) || defined(_PRE_WLAN_110X_PILOT)
    uint8_t bit_resv4;
#else
    uint8_t bit_code_book : 2;
    uint8_t bit_grouping : 2;
    uint8_t bit_row_number : 4;
#endif

    /* byte 2-3 */
    int8_t c_snr_ant0; /* ant0 SNR */
    int8_t c_snr_ant1; /* ant1 SNR */

    /* byte 4-7 */
    int8_t c_ant0_rssi; /* ANT0�ϱ���ǰ֡RSSI */
    int8_t c_ant1_rssi; /* ANT1�ϱ���ǰ֡RSSI */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
    int8_t ant2_rssi; /* ANT2�ϱ���ǰ֡RSSI */
    int8_t ant3_rssi; /* ANT3�ϱ���ǰ֡RSSI */
#endif
} hal_rx_statistic_stru;

typedef enum {
    /* "��ǰ֡������Ϊ0������1���ϱ� 2������ 3�������ⶪ��4���ظ�֡����others������" */
    HAL_RX_PROC_FLAGS_RSV = 0,
    HAL_RX_PROC_FLAGS_RELEASE = 1,
    HAL_RX_PROC_FLAGS_BUFFER = 2,
    HAL_RX_PROC_FLAGS_OUT_OF_WIND_DROP = 3,
    HAL_RX_PROC_FLAGS_DUPLICATE_DROP = 4,

    HAL_RX_PROC_FLAGS_BUTT
} hal_rx_proc_flags_enum;
typedef uint8_t hal_rx_proc_flags_enum_uint8;

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
typedef enum {
    HAL_TAS_MEASURE_INIT,
    HAL_TAS_MEASURING,
    HAL_TAS_ANT_INVALID,
    HAL_TAS_ANT_AVAILABLE,             // ˲̬���ϱ��ں˺��л�init״̬
    HAL_TAS_NOTIFY_COMPLETED,          // ˲̬���ϱ��ں˺��л�init״̬
    HAL_TAS_ANT_RSSI_MEASURE_TIMEOUT,  // ˲̬���ϱ��ں˺��л�init״̬
    HAL_TAS_STATUS_BUTT,
} hal_tas_rssi_measure_flag_enum;
typedef uint8_t hal_tas_rssi_measure_flag_enum_uint8;

typedef struct {
    frw_timeout_stru st_tas_rssi_measure_timer; /* tas���߲�����ʱ�� */
    int16_t s_tas_rssi_smth_access;           /* tas���߲��� core RSSI */
    uint8_t auc_resv[2];

    uint8_t uc_cur_measured_core;
    uint8_t uc_measure_vap_id;
    uint8_t uc_frame_cnt;
    hal_tas_rssi_measure_flag_enum_uint8 en_tas_rssi_measure_flag; /* tas���߲���״̬��־λ */
} hal_tas_rssi_measure_stru;
#endif

/* dmac_pkt_capturesʹ��,tx rx����ʹ�� */
typedef struct {
    uint8_t en_ant_rssi_sw; /* ͨ��ANT RSSI�л�ʹ�ܡ�bit0:����֡�л�ʹ�� bit1:����֡�л�ʹ�� */
    oal_bool_enum_uint8 en_log_print;
    uint8_t uc_rssi_high; /* RSSI������ */
    uint8_t uc_rssi_low;  /* RSSI������ */

    int16_t s_ant0_rssi_smth; /* ƽ���������ʷRSSI */
    int16_t s_ant1_rssi_smth;
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
    int16_t ant2_rssi_smth;
    int16_t ant3_rssi_smth;
#endif

    uint16_t uc_rssi_high_cnt;
    uint16_t uc_rssi_high_cnt_th;

    uint16_t uc_rssi_low_cnt;
    uint16_t uc_rssi_low_cnt_th;

    int8_t c_min_rssi_th; /* ��Сrssi���л����� */
    uint8_t auc_resv[3];
} hal_rx_ant_rssi_stru;

typedef struct {
    oal_bool_enum_uint8 en_ant_rssi_sw; /* ͨ��ANT RSSI�л�ʹ�� */
    oal_bool_enum_uint8 en_log_print;
    int8_t c_ctrl_rssi_th;            /* ����֡��˫ͨ���������� */
    uint8_t uc_rssi_change_valid_cnt; /* RSSI mgmt chain�л��������޵�cnt */

    uint8_t uc_rssi_mgmt_pre_chain;                         /* ��ǰrssi mgmt��preͨ�� */
    uint8_t uc_rssi_mgmt_new_chain;                         /* ��ǰrssi mgmt��newͨ�� */
    oal_bool_enum_uint8 en_ctrl_frm_tx_double_chain_pre_flag; /* ��Ӧ֡����˫ͨ�����͵�pre��־ */
    oal_bool_enum_uint8 en_ctrl_frm_tx_double_chain_new_flag; /* ��Ӧ֡����˫ͨ�����͵�new��־ */

    int16_t s_ant0_rssi_smth; /* ƽ���������ʷRSSI */
    int16_t s_ant1_rssi_smth;
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
    int16_t ant2_rssi_smth;
    int16_t ant3_rssi_smth;
#endif
} hal_rx_ant_rssi_mgmt_stru;

typedef struct {
    int8_t c_ant0_snr;   /* ANT0�ϱ���ǰ֡SNR */
    int8_t c_ant1_snr;   /* ANT1�ϱ���ǰ֡SNR */
    uint8_t uc_snr_high; /* SNR������ */
    uint8_t uc_snr_low;  /* SNR������ */

    uint16_t uc_snr_high_cnt;
    uint16_t uc_snr_high_cnt_th;

    uint16_t uc_snr_low_cnt;
    uint16_t uc_snr_low_cnt_th;

    oal_bool_enum_uint8 en_ant_snr_sw; /* ͨ��ANT SNR�л�ʹ�� */
    oal_bool_enum_uint8 en_log_print;
    oal_bool_enum_uint8 en_reserv[2];
} hal_rx_ant_snr_stru;

/* ant_detect�ṹ�� */
typedef struct {
    hal_rx_ant_rssi_stru st_rx_rssi;
    hal_rx_ant_snr_stru st_rx_snr;
    void *pst_cb;

    oal_bool_enum_uint8 en_rssi_trigger; /* ״̬���Ƿ���RSSI/SNR������MIMO�л���SISO */
    oal_bool_enum_uint8 en_miso_hold;    /* �Ƿ񱣳���MISO״̬ */
    uint8_t uc_tbtt_cnt;               /* ��ǰͳ�Ƶ�tbtt�ж��� */
    uint8_t uc_tbtt_cnt_th;            /* tbtt�ж�����ֵ */

    int8_t c_ori_max_th;
    int8_t c_cur_max_th;
    uint8_t uc_mimo_tbtt_open_th;  /* tbtt�жϿ���̽������ֵ */
    uint8_t uc_mimo_tbtt_close_th; /* tbtt�жϹر�̽������ֵ */
    int8_t c_ori_min_th;
    int8_t c_cur_min_th;
    uint8_t uc_diff_th;
    uint8_t _rom[1];
} hal_rssi_stru;

/* dmac_pkt_capturesʹ��,tx rx����ʹ�� */
typedef struct {
    union {
        struct {
            uint16_t bit_vht_mcs : 4;
            uint16_t bit_nss_mode : 2;
            uint16_t bit_protocol_mode : 4;
            uint16_t bit_reserved2 : 6;
        } st_vht_nss_mcs; /* 11ac�����ʼ����� */
        struct {
            uint16_t bit_ht_mcs : 6;
            uint16_t bit_protocol_mode : 4;
            uint16_t bit_reserved2 : 6;
        } st_ht_rate; /* 11n�����ʼ����� */
        struct {
            uint16_t bit_legacy_rate : 4;
            uint16_t bit_reserved1 : 2;
            uint16_t bit_protocol_mode : 4;
            uint16_t bit_reserved2 : 6;
        } st_legacy_rate; /* 11a/b/g�����ʼ����� */
    } un_nss_rate;

    uint8_t uc_short_gi;
    uint8_t uc_bandwidth;

    uint8_t bit_preamble : 1,
              bit_channel_code : 1,
              bit_stbc : 2,
              bit_reserved2 : 4;
} hal_statistic_stru;

/* HOSTר�� */
typedef struct {
    uint32_t *pul_mac_hdr_start_addr; /* ��Ӧ��֡��֡ͷ��ַ,�����ַ */
    uint16_t us_da_user_idx;          /* Ŀ�ĵ�ַ�û����� */
    uint16_t us_rsv;                  /* ���� */
} mac_rx_expand_cb_stru;

/* ��ϵͳ����Ҫ�����HMACģ�����Ϣ */
/* hal_rx_ctl_stru�ṹ���޸�Ҫ����hi110x_rx_get_info_dscr�����е��Ż� */
/* 1�ֽڶ��� */
#pragma pack(push, 1)
struct mac_rx_ctl {
    /* byte 0 */
    uint8_t bit_vap_id : 5;
    uint8_t bit_amsdu_enable : 1;    /* �Ƿ�Ϊamsdu֡,ÿ��skb��� */
    uint8_t bit_is_first_buffer : 1; /* ��ǰskb�Ƿ�Ϊamsdu���׸�skb */
    uint8_t bit_is_fragmented : 1;

    /* byte 1 */
    uint8_t uc_msdu_in_buffer; /* ÿ��skb������msdu��,amsdu��,ÿ֡��� */

    /* byte 2 */
    /* TODO: host add macro then open */
    uint8_t bit_ta_user_idx : 5;
    uint8_t bit_tid : 1;
    uint8_t bit_nan_flag : 1;
    uint8_t bit_is_key_frame : 1;
    /* byte 3 */
    uint8_t uc_mac_header_len : 6; /* mac header֡ͷ���� */
    uint8_t bit_is_beacon : 1;
    uint8_t bit_is_last_buffer : 1;

    /* byte 4-5 */
    uint16_t us_frame_len; /* ֡ͷ��֡����ܳ���,AMSDU����֡���� */

    /* byte 6 */
    /* TODO: host add macro then open �޸�ԭ��:host��device��һ�µ���uc_mac_vap_id��ȡ���󣬳���HOST���� */
    uint8_t uc_mac_vap_id : 4; /* ҵ���vap id�� */
    uint8_t bit_buff_nums : 4; /* ÿ��MPDUռ�õ�SKB��,AMSDU֡ռ��� */
    /* byte 7 */
    uint8_t uc_channel_number; /* ����֡���ŵ� */
#if (defined(_PRE_PRODUCT_ID_HI110X_HOST) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
    /* ����06; ����03/05 host skb cb 48�ֽڣ�hcc��copy�������࣬�����й������� */
    uint16_t   us_frame_control;
    uint16_t   bit_start_seqnum   : 12;
    uint16_t   bit_cipher_type    : 4;

    uint16_t   bit_release_end_sn : 12;
    uint16_t   bit_fragment_num   : 4;

    uint16_t   bit_rx_user_id     : 12;
    uint16_t   bit_frame_format   : 2;
    uint16_t   bit_dst_is_valid   : 1;
    uint16_t   bit_mcast_bcast    : 1;

    uint16_t   us_seq_num         : 12;
    uint16_t   bit_process_flag   : 3;
    uint16_t   bit_release_valid  : 1;

    uint8_t    rx_tid;
    uint8_t    dst_user_id;

    uint16_t   bit_release_start_sn : 12;
    uint16_t   bit_is_reo_timeout   : 1;
    uint16_t   bit_first_sub_msdu   : 1;
    uint16_t   bit_is_ampdu         : 1;
    uint16_t   resv                 : 1;

    uint8_t    dst_hal_vap_id;
    uint8_t    bit_band_id          : 4;
    uint8_t    bit_dst_band_id      : 4;
    uint8_t    rx_status;

    uint32_t   ul_rx_lsb_pn;
    uint16_t   us_rx_msb_pn;
#endif
#ifndef _PRE_PRODUCT_ID_HI110X_DEV
    /* OFFLOAD�ܹ��£�HOST���DEVICE��CB���� */
    mac_rx_expand_cb_stru st_expand_cb;
#endif
} __OAL_DECLARE_PACKED;
typedef struct mac_rx_ctl hal_rx_ctl_stru;
#pragma pack(pop)

/*
 * ��ϵͳ����Խ��գ��ṩ��ȡ�ӿ�
 * frame_len����
 * 802.11֡ͷ����(uc_mac_hdr_len)
*/
#pragma pack(push, 1)
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
typedef struct {
    /* byte 0 */
    uint8_t bit_cipher_protocol_type : 4; /* ����֡�������� */
    uint8_t bit_dscr_status : 4;          /* ����״̬ */

    /* byte 1 */
    uint8_t bit_AMPDU : 1;
    uint8_t bit_last_mpdu_flag : 1; /* �̶�λ�� */
    uint8_t bit_gi_type : 2;
    uint8_t bit_he_ltf_type : 2;
    uint8_t bit_sounding_mode : 2;

    /* byte 2 */
    uint8_t bit_freq_bandwidth_mode : 3;
    uint8_t bit_rx_himit_flag : 1; /* rx himit flag */
    uint8_t bit_ext_spatial_streams : 2;
    uint8_t bit_smoothing : 1;
    uint8_t bit_fec_coding : 1; /* channel code */

    /* byte 3-4 */
    union { /* todo */
        struct {
            uint8_t bit_rate_mcs : 4;
            uint8_t bit_nss_mode : 2;

            uint8_t bit_protocol_mode : 4;
            uint8_t bit_is_rx_vip : 1; /* place dcm bit */
            uint8_t bit_rsp_flag : 1;  /* beaformingʱ���Ƿ��ϱ��ŵ�����ı�ʶ��0:�ϱ���1:���ϱ� */
            uint8_t bit_mu_mimo_flag : 1;
            uint8_t bit_ofdma_flag : 1;
            uint8_t bit_beamforming_flag : 1; /* ����֡�Ƿ�����beamforming */
            uint8_t bit_STBC : 1;
        } st_rate; /* 11a/b/g/11ac/11ax�����ʼ����� */
        struct {
            uint8_t bit_ht_mcs : 6;

            uint8_t bit_protocol_mode : 4;
            uint8_t bit_is_rx_vip : 1; /* place dcm bit */
            uint8_t bit_rsp_flag : 1;  /* beaformingʱ���Ƿ��ϱ��ŵ�����ı�ʶ��0:�ϱ���1:���ϱ� */
            uint8_t bit_mu_mimo_flag : 1;
            uint8_t bit_ofdma_flag : 1;
            uint8_t bit_beamforming_flag : 1; /* ����֡�Ƿ�����beamforming */
            uint8_t bit_STBC : 1;
        } st_ht_rate; /* 11n�����ʼ����� */
    } un_nss_rate;

} hal_rx_status_stru;

#else //(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)
typedef struct {
    /* byte 0 */
    uint8_t bit_cipher_protocol_type : 4; /* ����֡�������� */
    uint8_t bit_dscr_status : 4;          /* ����״̬ */

    /* byte 1 */
    uint8_t bit_fec_coding : 1;
    uint8_t bit_last_mpdu_flag : 1;
    uint8_t bit_resv : 1;
#ifdef _PRE_WLAN_110X_PILOT
    uint8_t bit_gi_type : 2;
#else
    uint8_t bit_gi_type : 1;
    uint8_t bit_rsvd : 1;
#endif
    uint8_t bit_AMPDU : 1;
    uint8_t bit_sounding_mode : 2;

    /* byte 2 */
    uint8_t bit_ext_spatial_streams : 2;
    uint8_t bit_smoothing : 1;
    uint8_t bit_freq_bandwidth_mode : 4;
    uint8_t bit_preabmle : 1;

    /* byte 3-4 */
    union {
        struct {
            uint8_t bit_rate_mcs : 4;
            uint8_t bit_nss_mode : 2;
            uint8_t bit_protocol_mode : 2;

            uint8_t bit_rsp_flag : 1;
            uint8_t bit_STBC : 2;
            uint8_t bit_he_flag : 1;
#if defined(_PRE_WLAN_FEATURE_TXBF_HW) || defined(_PRE_WLAN_110X_PILOT)
            uint8_t bit_is_rx_vip : 1;
            uint8_t bit_he_ltf_type : 2;
            uint8_t bit_reserved3 : 1;
#else
            uint8_t bit_column_number : 4;
#endif
        } st_rate; /* 11a/b/g/11ac/11ax�����ʼ����� */
        struct {
            uint8_t bit_ht_mcs : 6;
            uint8_t bit_protocol_mode : 2;

            uint8_t bit_rsp_flag : 1;
            uint8_t bit_STBC : 2;
            uint8_t bit_he_flag : 1;
#if defined(_PRE_WLAN_FEATURE_TXBF_HW) || defined(_PRE_WLAN_110X_PILOT)
            uint8_t bit_is_rx_vip : 1;
            uint8_t bit_he_ltf_type : 2;
            uint8_t bit_reserved3 : 1;
#else
            uint8_t bit_column_number : 4;
#endif
        } st_ht_rate; /* 11n�����ʼ����� */
    } un_nss_rate;
} hal_rx_status_stru;
#endif //(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)
#pragma pack(pop)
/*****************************************************************************
  �ṹ��  : hal_rx_dscr_queue_header_stru
  �ṹ˵��: ��������������ͷ�Ľṹ��
*****************************************************************************/
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
typedef struct {
    oal_dlist_head_stru st_header;                    /* ��������������ͷ��� */
    uint16_t us_element_cnt;                        /* ����������������Ԫ�صĸ��� */
    hal_dscr_queue_status_enum_uint8 uc_queue_status; /* �������������е�״̬ */
    uint8_t uc_available_res_cnt;                   /* ��ǰ������Ӳ���������������� */
} hal_rx_dscr_queue_header_stru;
#else
typedef struct {
    uint32_t *pul_element_head;                     /* ָ���������������ĵ�һ��Ԫ�� */
    uint32_t *pul_element_tail;                     /* ָ�������������������һ��Ԫ�� */
    uint16_t us_element_cnt;                        /* ����������������Ԫ�صĸ��� */
    hal_dscr_queue_status_enum_uint8 uc_queue_status; /* �������������е�״̬ */
    uint8_t auc_resv[1];
} hal_rx_dscr_queue_header_stru;
#endif
/*****************************************************************************
  �ṹ��  : dmac_tx_dscr_queue_header_stru
  �ṹ˵��: ��������������ͷ�Ľṹ��
*****************************************************************************/
typedef struct {
    oal_dlist_head_stru st_header;                    /* ��������������ͷ��� */
    hal_dscr_queue_status_enum_uint8 en_queue_status; /* ��������������״̬ */
    uint8_t uc_ppdu_cnt;                            /* ����������������Ԫ�صĸ��� */
    uint8_t uc_txq_id;                              /* 0:��һ�׶��У�1:�ڶ��׶��� */
    uint8_t uc_vap_id;
} hal_tx_dscr_queue_header_stru;

/*****************************************************************************
  �ṹ��  : hal_lpm_chip_state
  �ṹ˵��: оƬ�͹���״̬ö��
*****************************************************************************/
typedef enum {
    HAL_LPM_STATE_POWER_DOWN,
    HAL_LPM_STATE_IDLE,
    HAL_LPM_STATE_LIGHT_SLEEP,
    HAL_LPM_STATE_DEEP_SLEEP,
    HAL_LPM_STATE_NORMAL_WORK,
    HAL_LPM_STATE_WOW,

    HAL_LPM_STATE_BUTT
} hal_lpm_state_enum;
typedef uint8_t hal_lpm_state_enum_uint8;
/* 7.7 ���ⲿ�������豸���ӿ��б������ⲿ����ֱ�ӵ��ã����ǵ��ö�Ӧ���������� */
typedef void (*p_hal_alg_isr_func)(uint8_t uc_vap_id, void *p_hal_to_dmac_device);
typedef void (*p_hal_gap_isr_func)(uint8_t uc_vap_id, void *p_hal_to_dmac_device);
#ifdef _PRE_PRODUCT_ID_HI110X_DEV
typedef uint32_t (*p_tbtt_ap_isr_func)(uint8_t uc_mac_vap_id);
#endif
//ONLY FOR UT
typedef struct {
    uint32_t ul_phy_addr;
    uint8_t uc_status;
    uint8_t uc_q;
    uint8_t auc_resv[2];
    uint32_t ul_timestamp;

    uint32_t ul_arrive_time; /* �°벿����ʱ�� */
    uint32_t ul_handle_time; /* �°벿����ʱ�� */
    uint32_t ul_irq_cnt;
    oal_cpu_usage_stat_stru st_cpu_state;
} hal_rx_dpart_track_stru;

typedef enum {
    HAL_DFR_TIMER_STEP_1 = 0,
    HAL_DFR_TIMER_STEP_2 = 1,

} hal_dfr_timer_step_enum;
typedef uint8_t hal_dfr_timer_step_enum_uint8;

typedef struct {
    frw_timeout_stru st_tx_prot_timer;                   /* ����޷�������ж϶�ʱ�� */
    hal_dfr_timer_step_enum_uint8 en_tx_prot_timer_step; /* ��ʱ��־�������ڼ��γ�ʱ */
    uint16_t us_tx_prot_time;                          /* ��ʱʱ�� */
    uint8_t auc_resv[1];
} hal_dfr_tx_prot_stru;

typedef struct {
    uint16_t us_tbtt_cnt; /* TBTT�жϼ�����ÿ10��TBTT�жϣ���us_err_cnt���� */
    uint16_t us_err_cnt;  /* ÿ10��TBTT�жϣ�������MAC�����жϸ��� */
} hal_dfr_err_opern_stru;
//only for UT
typedef struct {
    oal_dlist_head_stru st_entry;
    uint32_t ul_phy_addr; /* ���������������ַ */
} witp_rx_dscr_recd_stru;

/*****************************************************************************
  �ṹ��  : hal_phy_pow_param_stru
  �ṹ˵��: PHY ������ؼĴ�������, ��2.4G/5GƵ���л�ʱʹ��
*****************************************************************************/
typedef struct {
    /* ��ͬ֡��tpc code */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || \
    (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
    uint32_t aul_ack_cts_pow_code[WLAN_OFDM_ACK_CTS_TYPE_BUTT][WLAN_2G_SUB_BAND_NUM + WLAN_5G_SUB_BAND_NUM];
    uint32_t aul_2g_dsss_ack_cts_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint32_t aul_rts_pow_code[WLAN_2G_SUB_BAND_NUM + WLAN_5G_SUB_BAND_NUM];
    uint32_t aul_one_pkt_pow_code[WLAN_2G_SUB_BAND_NUM + WLAN_5G_SUB_BAND_NUM];
    uint32_t aul_bar_pow_code[WLAN_2G_SUB_BAND_NUM + WLAN_5G_SUB_BAND_NUM];
    uint32_t aul_cfend_pow_code[WLAN_2G_SUB_BAND_NUM + WLAN_5G_SUB_BAND_NUM];
    uint32_t aul_5g_vht_report_pow_code[WLAN_5G_SUB_BAND_NUM];
#else
    uint32_t aul_2g_ofdm_ack_cts_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint32_t aul_5g_ack_cts_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_2g_dsss_ack_cts_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_rts_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_one_pkt_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_abort_selfcts_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_abort_cfend_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_abort_null_data_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_2g_cfend_pow_code[WLAN_2G_SUB_BAND_NUM];
    uint8_t auc_5g_rts_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_one_pkt_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_abort_cfend_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_cfend_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_ndp_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_vht_report_pow_code[WLAN_5G_SUB_BAND_NUM];
    uint8_t auc_5g_abort_null_data_pow_code[WLAN_5G_SUB_BAND_NUM];
#endif

    /* ��ȡ��ͬ֡��ʽ������ */
#ifdef _PRE_PRODUCT_ID_HI110X_DEV
    uint32_t aul_rate_ofdm_ack_cts[WLAN_OFDM_ACK_CTS_TYPE_BUTT];
#else
    uint32_t ul_rate_ofdm_ack_cts;
#endif
    uint32_t ul_rate_bar;
    uint32_t ul_rate_rts;
    uint32_t ul_rate_one_pkt;
    uint32_t ul_rate_cfend;
    uint32_t ul_rate_vht_report;
    uint32_t ul_rate_abort_selfcts;
    uint32_t ul_rate_abort_cfend;
    uint32_t ul_rate_abort_null_data;
    uint8_t auc_reserve[128];
} hal_phy_pow_param_stru;
#ifdef _PRE_WLAN_FEATURE_DFS
typedef struct {
    /* normal pulse det */
    uint8_t uc_irq_cnt;
    uint8_t uc_radar_cnt;
    uint8_t uc_irq_cnt_old;
    oal_bool_enum_uint8 en_timeout;

    oal_bool_enum_uint8 en_timer_start;
    oal_bool_enum_uint8 en_is_enabled;
    uint8_t auc_reserv1[2];

    uint32_t ul_period_cnt;
    frw_timeout_stru st_timer;
} hal_dfs_normal_pulse_det_stru;

typedef struct {
    /* �������������� */
    oal_bool_enum_uint8 en_fcc_chirp_duration_diff;
    oal_bool_enum_uint8 en_fcc_chirp_pow_diff;
    oal_bool_enum_uint8 en_fcc_type4_duration_diff;
    oal_bool_enum_uint8 en_fcc_chirp_eq_duration_num;
} hal_dfs_pulse_check_filter_stru;

typedef struct {
    oal_bool_enum_uint8 en_chirp_enable;
    uint8_t uc_chirp_wow_wake_flag;  // ��wow��gpio�ж��ϰ벿��ǣ���ʾ�մ�wow����
    uint8_t uc_chirp_detect_flag;    // ��wow��gpio�ж�ʹ��
#if defined(_PRE_WLAN_FEATURE_DFS)
    oal_bool_enum_uint8 en_log_switch;

    hal_dfs_radar_type_enum_uint8 en_radar_type;
    uint8_t uc_chirp_cnt;
    uint8_t uc_chirp_cnt_total;
    oal_bool_enum_uint8 en_chirp_timeout;

    uint32_t ul_last_timestamp_for_chirp_pulse;

    frw_timeout_stru st_timer;
    uint32_t ul_min_pri;

    /* crazy report det */
    uint8_t uc_chirp_cnt_for_crazy_report_det;
    oal_bool_enum_uint8 en_crazy_report_cnt;
    oal_bool_enum_uint8 en_crazy_report_is_enabled;
    uint8_t auc_resv[1];

    frw_timeout_stru st_timer_crazy_report_det;
    frw_timeout_stru st_timer_disable_chirp_det;

    /* normal pulse det timer */
    hal_dfs_normal_pulse_det_stru st_dfs_normal_pulse_det;

    /* �󱨹������� */
    hal_dfs_pulse_check_filter_stru st_dfs_pulse_check_filter;

#else
    uint8_t uc_resv;
#endif
    uint32_t ul_chirp_time_threshold;
    uint32_t ul_chirp_cnt_threshold;
    uint32_t ul_time_threshold;
    uint32_t ul_last_burst_timestamp;
    uint32_t ul_last_burst_timestamp_for_chirp;
} hal_dfs_radar_filter_stru;
#endif

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
typedef struct {
    uint16_t us_max_offset_tsf; /* ���ʱ��ʱ�� */
    uint16_t us_mpdu_len;       /* ���ʱ��mpdu���� */
    uint16_t us_tx_excp_cnt;    /* ������ɺ�ʱ�쳣ͳ�� */
    uint8_t uc_q_num;           /* ���ʱ��q_num */
    uint8_t auc_resv;
} hal_tx_excp_info_stru;
#endif

typedef struct {
    uint32_t ul_tkipccmp_rep_fail_cnt;    /* �طŹ���������TKIP + CCMP */
    uint32_t ul_tx_mpdu_cnt;              /* ���ͼ�����ampdu�����ȼ� + ��ͨ���ȼ� + ampdu��mpdu */
    uint32_t ul_rx_passed_mpdu_cnt;       /* ����AMPDU MPDU��FCS��ȷ��MPDU���� */
    uint32_t ul_rx_failed_mpdu_cnt;       /* ����AMPDU MPDU��FCS�����MPDU���� */
    uint32_t ul_rx_tkipccmp_mic_fail_cnt; /* kip mic + ccmp mic fail��֡�� */
    uint32_t ul_key_search_fail_cnt;      /* ����key serach fail��֡�� */
    uint32_t ul_phy_rx_dotb_ok_frm_cnt;   /* PHY����dotb ok��֡���� */
    uint32_t ul_phy_rx_htvht_ok_frm_cnt;  /* PHY����vht ht ok��֡���� */
    uint32_t ul_phy_rx_lega_ok_frm_cnt;   /* PHY����legace ok��֡���� */
    uint32_t ul_phy_rx_dotb_err_frm_cnt;  /* PHY����dotb err��֡���� */
    uint32_t ul_phy_rx_htvht_err_frm_cnt; /* PHY����vht ht err��֡���� */
    uint32_t ul_phy_rx_lega_err_frm_cnt;  /* PHY����legace err��֡���� */

} hal_mac_key_statis_info_stru;

/* ��Ӱ��Ŀ��vdetֵ�Ĳ������� */
typedef union { /* todo */
    struct {
        wlan_channel_band_enum_uint8 en_freq : 4;
        wlan_bw_cap_enum_uint8 en_band_width : 4;
        uint8_t uc_channel;
        uint8_t uc_pdet_chain : 4;
        wlan_mod_enum_uint8 en_mod : 4;
        uint8_t uc_tx_pow;
    } st_rf_core_para;
    uint32_t ul_para;
} hal_dyn_cali_record_union;

typedef struct {
    hal_dyn_cali_record_union un_record_para;
    int16_t s_real_pdet;
    int16_t s_exp_pdet;
    uint8_t auc_resv[4];
} hal_dyn_cali_usr_record_stru;

typedef struct {
    oal_bool_enum_uint8 aen_dyn_cali_chain_comp_flag[WLAN_RF_CHANNEL_NUMS];
    uint8_t uc_tx_chain_sel;
    uint8_t auc_resv[1];
} hal_dyn_cali_chain_status_stru;

typedef struct hal_dyn_cali_val {
    uint16_t aus_cali_en_cnt[WLAN_BAND_BUTT];
    uint16_t aus_cali_en_interval[WLAN_BAND_BUTT]; /* ���ζ�̬У׼�������������  */

    uint16_t us_cali_ch_sw_interval; /* ͨ���л����У׼ʹ������������  */
    uint16_t us_cali_ch_sw_cnt;      /* ͨ�����Ͷ�̬У׼ʹ������������ */

    uint8_t uc_cali_pdet_min_th;               /* ��̬У׼��С��ֵ  */
    uint8_t uc_cali_pdet_max_th;               /* ��̬У׼������ֵ  */
    uint8_t uc_cali_chain;                     /*  ��ǰ��̬У׼У׼ͨ����  */
    oal_bool_enum_uint8 en_realtime_cali_adjust; /* ��̬У׼ʹ�ܲ������� */

    hal_dyn_cali_usr_record_stru ast_vap_pow_info[WLAN_RF_CHANNEL_NUMS];
    hal_dyn_cali_chain_status_stru st_cali_chain_status_info;

    int16_t s_temp;                               /* Tsensor����¶� */
    oal_bool_enum_uint8 en_realtime_cali_comp_flag; /* ��̬У׼������ɱ�־ */
#ifdef _PRE_WLAN_FEATURE_FULL_QUAN_PROD_CAL
    uint8_t uc_full_data_coll_idx;
#endif
} hal_dyn_cali_val_stru;

#ifdef _PRE_WLAN_FEATURE_FULL_QUAN_PROD_CAL
typedef struct {
    int16_t s_real_pdet;
    uint8_t uc_tx_pow;
    uint8_t uc_rsv;
} hal_dyn_full_cali_data_stru;

typedef struct {
    hal_dyn_full_cali_data_stru *past_real_pdet_coll_data;
    uint8_t uc_cnt;
} hal_dyn_full_cali_data_coll_stru;
#endif

/* ɨ��״̬��ͨ���жϵ�ǰɨ���״̬���ж϶��ɨ������Ĵ�������Լ��ϱ�ɨ�����Ĳ��� */
typedef enum {
    MAC_SCAN_STATE_IDLE,
    MAC_SCAN_STATE_RUNNING,

    MAC_SCAN_STATE_BUTT
} mac_scan_state_enum;
typedef uint8_t mac_scan_state_enum_uint8;

/* ���ż��ģ���⵽����Ƶ��Ƶ����ģʽ�ı�ʱ�ص�����ö�ٶ��� */
typedef enum {
    HAL_ALG_INTF_DET_ADJINTF_NO = 0,      /* �޸��� */
    HAL_ALG_INTF_DET_ADJINTF_MEDIUM = 1,  /* �е�ǿ�ȸ��� */
    HAL_ALG_INTF_DET_ADJINTF_STRONG = 2,  /* ǿ���� */
    HAL_ALG_INTF_DET_ADJINTF_CERTIFY = 3, /* ��֤���� */
    HAL_ALG_INTF_DET_STATE_PKADJ = 4,     /* PK��������״̬ */
    HAL_ALG_INTF_DET_STATE_PK = 5,        /* PK״̬ */
    HAL_ALG_INTF_DET_ADJINTF_BUTT
} hal_alg_intf_det_mode_enum;
typedef oal_uint8 hal_alg_intf_det_mode_enum_uint8;
/* ���ż��ģ���⵽����Ƶ��Ƶ����ģʽ�ı�ʱ�ص�����ö�ٶ��� */
typedef enum {
    HAL_ACI_TYPE_ADJINTF_NO = 0,      /* �޸��� */
    HAL_ACI_TYPE_ADJINTF_EN = 1,  /* ACI���� */

    HAL_ACI_TYPE_ADJINTF_BUTT
} hal_alg_aci_type_enum;
typedef oal_uint8 hal_alg_aci_type_enum_uint8;

/* �㷨ʹ�õ��û�����״̬ */
typedef enum {
    HAL_ALG_USER_DISTANCE_NEAR = 0,
    HAL_ALG_USER_DISTANCE_NORMAL = 1,
    HAL_ALG_USER_DISTANCE_FAR = 2,

    HAL_ALG_USER_DISTANCE_BUTT
} hal_alg_user_distance_enum;
typedef uint8_t hal_alg_user_distance_enum_uint8;

/* �豸���롢����״̬����Ϣ */
typedef struct {
    hal_alg_user_distance_enum_uint8 en_alg_distance_stat; /* ����״̬  */
    hal_alg_intf_det_mode_enum_uint8 en_adj_intf_state;    /* ��Ƶ����״̬ */
    oal_bool_enum_uint8 en_co_intf_state;                  /* ͬƵ����״̬ */
    uint8_t uc_reserv;
} hal_alg_stat_info_stru;
typedef struct {
    uint32_t ul_rx_rate;
    uint16_t us_rx_rate_stat_count;
    oal_bool_enum_uint8 en_compatibility_enable;
    oal_bool_enum_uint8 en_compatibility_stat;
    uint32_t aul_compatibility_rate_limit[WLAN_BW_CAP_BUTT][WLAN_PROTOCOL_BUTT];
} hal_compatibility_stat_stru;

typedef struct {
    uint8_t uc_fe_print_ctrl;  // �Ĵ����������
    uint8_t auc_rev[3];
} hal_device_dft_stat_stru;

typedef struct {
    uint16_t us_last_sn;  // ����ʧ��ά��������Ʊ���sdt�����ӡ��������
    uint8_t uc_print_ctl;
    uint8_t auc_rev[1];
} hal_device_sdt_stat_stru;

/* ���ն������������㷨����������ͳ�ƵĽṹ�� */
typedef struct {
    uint16_t us_rx_dscr_alg_thres;       /* ��¼�㷨���� */
    uint16_t us_rx_event_pkts_sum;       /* ��¼һ������(100ms)�ڽ��ն��н���֡������ */
    uint16_t us_rx_event_cnt;            /* ��¼һ������(100ms)�ڽ��ն��в���isr info���� */
    uint16_t us_rx_cur_min_thres;        /* ��¼�ϸ�����(100ms)�������д���æ�е�״̬ */
    uint16_t us_rx_max_netbuff_used_cnt; /* ��¼һ������(100ms)�ڽ��ն��й��ص���Դ�����ķ�ֵ */
    uint16_t us_rx_max_dscr_used_cnt;    /* ��¼һ������(100ms)�ڽ��ն��й��ص���Դ�����ķ�ֵ */
    uint16_t us_rx_max_element_avr_cnt;  /* ��¼���ն��й��ص���Դ�����ķ�ֵ��ƽ�� */
    uint8_t uc_previous_cycle_stats;     /* ��¼�ϸ�����(100ms)�������д���æ�е�״̬ */
    uint8_t uc_continus_idle_cnt;        /* �������ɸ����ڿ��к�Żָ����ն���δ���У���ֹ�㷨����̫���� */
    uint16_t us_rx_smooth_event_pkts;    /* ��¼һ�����ڣ�100ms�����ն���֡������ƽ��ֵ */
    uint16_t us_resv;
} alg_rx_dscr_ctl_alg_info_stru;

/* ���ն������������㷨����������ͳ�ƵĽṹ�� */
typedef struct {
    void *pst_rx_dscr_ctl; /* hal dev�±���chip��������������ṹ��ָ�� */
    alg_rx_dscr_ctl_alg_info_stru *pst_rx_dscr_ctl_alg_info;
    uint16_t aus_pending_isr_cnt[HAL_RX_DSCR_QUEUE_ID_BUTT]; /* ��¼pending��ƹ�Ҷ������isr info���� */
    uint16_t aus_pending_pkt_cnt[HAL_RX_DSCR_QUEUE_ID_BUTT]; /* ��¼pending��ƹ�Ҷ������֡���� */
} alg_rx_dscr_ctl_device_info_stru;

#ifdef _PRE_WLAN_FEATURE_11AX
typedef enum {
    HAL_HE_HTC_CONFIG_UPH_AUTO = 0,                   /* Ĭ��auto, mac ����ͼ���uph */
    HAL_HE_HTC_CONFIG_NO_HTC = 1,                     /* ����֡������htc ͷ */
    HAL_HE_HTC_CONFIG_SOFT_INSERT_SOFT_ADD = 2,       /* ������ htcͷ */
    HAL_HE_HTC_CONFIG_UPH_MAC_INSERT_TRIGGER_CAL = 3, /* mac����htc,���ͨ��trigger֡����uph */
    HAL_HE_HTC_CONFIG_MAC_INSERT_SOFT_ADD = 4,        /* mac��������̶���htcֵ */

    HAL_HE_HTC_CONFIG_BUTT
} hal_he_htc_config_enum;
#endif

/* ���շ���֡���ͣ�ֵ�����޸� */
typedef enum {
    MAC_FRAME_TYPE_RTH   = 0,   /* Ethernet II֡��ʽ */
    MAC_FRAME_TYPE_RSV0  = 1,
    MAC_FRAME_TYPE_RSV1  = 2,
    MAC_FRAME_TYPE_80211 = 3,   /* 802.11֡��ʽ */

    FRAME_TYPE_RESEREVD         /* ���� */
} mac_frame_type_enum;
typedef uint8_t mac_frame_type_enum_uint8;

typedef struct tag_hal_rx_ppdu_dscr_list_head_stru {
    oal_dlist_head_stru st_list_head;
    uint32_t          ul_list_num;
    oal_spin_lock_stru  st_spin_lock;
} hal_rx_ppdu_dscr_list_head_stru;

typedef struct tag_hal_mpdu_buf_list_head_stru {
    oal_netbuf_head_stru st_netbuf_head;
    /* ����Ѿ������netbuf�������ڵ���max�����ٲ��ڴ� */
    uint32_t ul_netbuf_cnt_max;
} hal_mpdu_buf_list_head_stru;

typedef struct tag_hal_rx_ppdu_dscr_stru {
    oal_dlist_head_stru st_entry;
    uint8_t           hw_ppdu_data[4];
} hal_rx_ppdu_dscr_stru;

/* ��Ӳ��֧�ֵ���Դ����һ�£������������ѡ��ֻʹ�ò���Ring */
typedef enum {
    SMGMT_FREE_RING = 0,
    NMGMT_FREE_RING,
    LMGMT_FREE_RING,
    SDATA_FREE_RING,
    LDATA_FREE_RING,
    RX_PPDU_FREE_RING,

    RX_FREE_RING_TYPE_BUTT
} RX_FREE_RING_TYPE;
typedef uint8_t hal_free_ring_type_enum_uint8;

typedef enum {
    MGMT_COMP_RING = 0,
    DATA_COMP_RING,
    RX_PPDU_COMP_RING,
    BA_INFO_RING,

    RX_COMP_RING_TYPE_BUTT
} RX_COMP_RING_TYPE;
typedef uint8_t hal_comp_ring_type_enum_uint8;

#define HAL_RX_MPDU_QNUM 2

typedef struct tag_hal_device_rx_mpdu_stru {
    /* MPDU������У��ж��ϰ벿���°벿ƹ���л����Լ��ٲ������� */
    oal_netbuf_head_stru  ast_rx_mpdu_list[HAL_RX_MPDU_QNUM];
    oal_spin_lock_stru    st_spin_lock;

    /* �ж�<��>�벿��ǰʹ�õ�MPDU���������� */
    uint8_t              cur_idx;
    uint8_t              auc_resv[3]; /* 3 resv */

    /* ��󻺴�Ľ���MPDU��������Ŀ����ֹ��������MPDU������ */
    uint32_t             max_buff_num;
    /* ÿ������������Ŀ */
    uint32_t             process_num_per_round;
} hal_rx_mpdu_que;


typedef struct {
    /* ring������: free ring����complete ring */
    uint8_t uc_ring_type;
    uint8_t uc_ring_subtype;
    uint16_t resv;
    /* ring����� */
    uint16_t us_entries;
    /* ring��ÿ��Ԫ�صĴ�С,����, BA infoԪ�ش�С64�ֽ� */
    uint16_t us_entry_size;
    /* ��������� */
    union {
        uint16_t us_read_ptr; /* read index */
        struct {
            uint16_t bit_read_ptr  : 15,
                       bit_wrap_flag : 1;
        } st_read_ptr;
    } un_read_ptr;

    union {
        uint16_t us_write_ptr; /* write index */
        struct {
            uint16_t bit_write_ptr : 15,
                       bit_wrap_flag : 1;
        } st_write_ptr;
    } un_write_ptr;

    /* ring����ַ */
    uint64_t *p_entries;

    /* �Ĵ�����ַ */
    volatile uint32_t ul_write_ptr_reg;
    volatile uint32_t ul_read_ptr_reg;
    volatile uint32_t ul_ring_size_reg;
    volatile uint32_t ul_ring_base_reg;
    volatile uint32_t cfg_write_ptr_reg;
    volatile uint32_t cfg_read_ptr_reg;
} hal_ring_ctl_stru;

typedef struct {
    volatile uint32_t tx_msdu_info_ring_base_lsb;
    volatile uint32_t tx_msdu_info_ring_base_msb;

    volatile uint16_t write_ptr;
    volatile uint16_t tx_msdu_ring_depth : 4;
    volatile uint16_t max_aggr_amsdu_num : 4;
    volatile uint16_t reserved : 8;

    volatile uint16_t read_ptr;
    volatile uint16_t reserved1;
} hal_tx_msdu_info_ring_stru;

/* HALģ���DMACģ�鹲�õ�WLAN RX�ṹ�� */
typedef struct {
    oal_netbuf_stru *pst_netbuf; /* netbuf����һ��Ԫ�� */
    uint16_t us_netbuf_num;    /* netbuf����ĸ��� */
    uint8_t auc_resv[2];       /* �ֽڶ��� */
} hal_cali_hal2hmac_event_stru;

typedef struct hal_pwr_fit_para_stru {
    oal_int32 l_pow_par2; /* ������ϵ�� */
    oal_int32 l_pow_par1; /* һ�� */
    oal_int32 l_pow_par0; /* ������ */
} hal_pwr_fit_para_stru;

typedef struct {
    uint32_t ul_packet_idx;

    uint8_t auc_payoald[4];
} hal_cali_hal2hmac_payload_stru;

typedef struct {
    uint16_t user_id;
    uint8_t tid;
    uint8_t ba_info_vld;
} hal_tx_ba_info_stru;

#ifdef _PRE_WLAN_FEATURE_HIEX
typedef mac_hiex_cap_stru hal_hiex_cap_stru;
#endif

/* hal device id ö�� */
typedef enum {
    HAL_DEVICE_ID_MASTER        = 0,    /* master��hal device id */
    HAL_DEVICE_ID_SLAVE         = 1,    /* slave��hal device id */

    HAL_DEVICE_ID_BUTT                  /* ���id */
} hal_device_id_enum;
typedef uint8_t hal_device_id_enum_enum_uint8;

typedef enum {
    HAL_RAM_RX = 0,  /* Device��������� */
    HAL_DDR_RX = 1,  /* Host DDR�������� */

    HAL_RX_BUTT
} hal_rx_switch_enum;

#endif /* end of hal_common.h */
