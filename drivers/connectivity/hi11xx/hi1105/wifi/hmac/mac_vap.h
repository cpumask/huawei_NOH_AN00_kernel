

#ifndef __MAC_VAP_H__
#define __MAC_VAP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#include "mac_device.h"
#include "mac_user.h"
#include "securec.h"
#include "securectype.h"
#include "mac_common.h"
#include "wlan_customize.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_VAP_H

/* 2 �궨�� */
#define MAC_NUM_DR_802_11A  8 /* 11A 5gģʽʱ����������(DR)���� */
#define MAC_NUM_BR_802_11A  3 /* 11A 5gģʽʱ�Ļ�������(BR)���� */
#define MAC_NUM_NBR_802_11A 5 /* 11A 5gģʽʱ�ķǻ�������(NBR)���� */

#define MAC_NUM_DR_802_11B  4 /* 11B 2.4Gģʽʱ����������(DR)���� */
#define MAC_NUM_BR_802_11B  2 /* 11B 2.4Gģʽʱ����������(BR)���� */
#define MAC_NUM_NBR_802_11B 2 /* 11B 2.4Gģʽʱ����������(NBR)���� */

#define MAC_NUM_DR_802_11G  8 /* 11G 2.4Gģʽʱ����������(DR)���� */
#define MAC_NUM_BR_802_11G  3 /* 11G 2.4Gģʽʱ�Ļ�������(BR)���� */
#define MAC_NUM_NBR_802_11G 5 /* 11G 2.4Gģʽʱ�ķǻ�������(NBR)���� */

#define MAC_NUM_DR_802_11G_MIXED      12 /* 11G ���ģʽʱ����������(DR)���� */
#define MAC_NUM_BR_802_11G_MIXED_ONE  4  /* 11G ���1ģʽʱ�Ļ�������(BR)���� */
#define MAC_NUM_NBR_802_11G_MIXED_ONE 8  /* 11G ���1ģʽʱ�ķǻ�������(NBR)���� */

#define MAC_NUM_BR_802_11G_MIXED_TWO  7 /* 11G ���2ģʽʱ�Ļ�������(BR)���� */
#define MAC_NUM_NBR_802_11G_MIXED_TWO 5 /* 11G ���2ģʽʱ�ķǻ�������(NBR)���� */

/* 11N MCS��ص����� */
#define MAC_MAX_RATE_SINGLE_NSS_20M_11N 0 /* 1���ռ���20MHz��������� */
#define MAC_MAX_RATE_SINGLE_NSS_40M_11N 0 /* 1���ռ���40MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_20M_11N 0 /* 1���ռ���80MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_40M_11N 0 /* 2���ռ���20MHz��������� */

/* 11AC MCS��ص����� */
#define MAC_MAX_SUP_MCS7_11AC_EACH_NSS  0 /* 11AC���ռ���֧�ֵ����MCS��ţ�֧��0-7 */
#define MAC_MAX_SUP_MCS8_11AC_EACH_NSS  1 /* 11AC���ռ���֧�ֵ����MCS��ţ�֧��0-8 */
#define MAC_MAX_SUP_MCS9_11AC_EACH_NSS  2 /* 11AC���ռ���֧�ֵ����MCS��ţ�֧��0-9 */
#define MAC_MAX_UNSUP_MCS_11AC_EACH_NSS 3 /* 11AC���ռ���֧�ֵ����MCS��ţ���֧��n���ռ��� */

#ifdef _PRE_WLAN_FEATURE_1024QAM
#define MAC_MAX_RATE_SINGLE_NSS_20M_11AC  107  /* 1���ռ���20MHz�����Long GI���� */
#define MAC_MAX_RATE_SINGLE_NSS_40M_11AC  225  /* 1���ռ���40MHz�����Long GI���� */
#define MAC_MAX_RATE_SINGLE_NSS_80M_11AC  487  /* 1���ռ���80MHz�����Long GI���� */
#define MAC_MAX_RATE_SINGLE_NSS_160M_11AC 975  /* 1���ռ���160MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_20M_11AC  216  /* 2���ռ���20MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_40M_11AC  450  /* 2���ռ���40MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_80M_11AC  975  /* 2���ռ���80MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_160M_11AC 1950 /* 2���ռ���160MHz�����Long GI���� */
#else
/* ����Э��Ҫ��(9.4.2.158.3�½�)���޸�Ϊlong gi���� */
#define MAC_MAX_RATE_SINGLE_NSS_20M_11AC  86   /* 1���ռ���20MHz��������� */
#define MAC_MAX_RATE_SINGLE_NSS_40M_11AC  180  /* 1���ռ���40MHz��������� */
#define MAC_MAX_RATE_SINGLE_NSS_80M_11AC  390  /* 1���ռ���80MHz��������� */
#define MAC_MAX_RATE_SINGLE_NSS_160M_11AC 780  /* 1���ռ���160MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_20M_11AC  173  /* 2���ռ���20MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_40M_11AC  360  /* 2���ռ���40MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_80M_11AC  780  /* 2���ռ���80MHz��������� */
#define MAC_MAX_RATE_DOUBLE_NSS_160M_11AC 1560 /* 2���ռ���160MHz��������� */
#endif
#define HMAC_FBT_RSSI_ADJUST_VALUE 100
#define HMAC_RSSI_SIGNAL_INVALID   0xff /* �ϱ�RSSI��Чֵ */
#define HMAC_FBT_RSSI_MAX_VALUE    100

#define MAC_VAP_USER_HASH_INVALID_VALUE 0xFFFFFFFF                    /* HSAH�Ƿ�ֵ */
#define MAC_VAP_USER_HASH_MAX_VALUE     (WLAN_ASSOC_USER_MAX_NUM * 2) /* 2Ϊ��չ���� */

#define MAC_VAP_CAP_ENABLE  1
#define MAC_VAP_CAP_DISABLE 0

#define MAC_VAP_FEATURE_ENABLE  1
#define MAC_VAP_FEATRUE_DISABLE 0

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R
#define MAC_RSN_AKM_FT_OVER_8021X_VAL        1
#define MAC_RSN_AKM_FT_PSK_VAL               2
#define MAC_RSN_AKM_FT_OVER_SAE_VAL          4
#define MAC_RSN_AKM_FT_OVER_8021X_SHA384_VAL 8
#endif
#define CIPHER_SUITE_SELECTOR(a, b, c, d) \
    ((((uint32_t)(d)) << 24) | (((uint32_t)(c)) << 16) | (((uint32_t)(b)) << 8) | (uint32_t)(a))
#define MAC_WPA_CHIPER_CCMP    CIPHER_SUITE_SELECTOR(0x00, 0x50, 0xf2, 4)
#define MAC_WPA_CHIPER_TKIP    CIPHER_SUITE_SELECTOR(0x00, 0x50, 0xf2, 2)
#define MAC_WPA_AKM_PSK        CIPHER_SUITE_SELECTOR(0x00, 0x50, 0xf2, 2)
#define MAC_WPA_AKM_PSK_SHA256 CIPHER_SUITE_SELECTOR(0x00, 0x50, 0xf2, 6)
#define MAC_RSN_CHIPER_CCMP    CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 4)
#define MAC_RSN_CHIPER_TKIP    CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 2)
#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R
#define MAC_RSN_AKM_FT_OVER_8021X        CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 3)
#define MAC_RSN_AKM_FT_PSK               CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 4)
#define MAC_RSN_AKM_FT_OVER_SAE          CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 9)
#define MAC_RSN_AKM_FT_OVER_8021X_SHA384 CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 13)
#endif
#define MAC_RSN_AKM_PSK                CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 2)
#define MAC_RSN_AKM_PSK_SHA256         CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 6)
#define MAC_RSN_AKM_SAE                CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 8)
#define MAC_RSN_AKM_OWE                CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 18)

#define MAC_WPA_CHIPER_OUI(_suit_type) CIPHER_SUITE_SELECTOR(0x00, 0x50, 0xf2, _suit_type)
#define MAC_RSN_CHIPER_OUI(_suit_type) CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, _suit_type)

#define MAC_RSN_CIPHER_SUITE_AES_128_CMAC CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 6)
#define MAC_RSN_CIPHER_SUITE_GCMP         CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 8)
#define MAC_RSN_CIPHER_SUITE_GCMP_256     CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 9)
#define MAC_RSN_CIPHER_SUITE_CCMP_256     CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 10)
#define MAC_RSN_CIPHER_SUITE_BIP_GMAC_128 CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 11)
#define MAC_RSN_CIPHER_SUITE_BIP_GMAC_256 CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 12)
#define MAC_RSN_CIPHER_SUITE_BIP_CMAC_256 CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, 13)

#define IS_SUPPORT_OWE(_pul_suites)       \
    ((MAC_RSN_AKM_OWE == *(_pul_suites)) || (MAC_RSN_AKM_OWE == *((_pul_suites) + 1)))

#ifdef _PRE_WLAN_FEATURE_SAE
#define IS_ONLY_SUPPORT_SAE(_pul_suites) \
    ((MAC_RSN_AKM_SAE == *(_pul_suites)) && (0 == *((_pul_suites) + 1)))
#endif

#define MAC_CALCULATE_HASH_VALUE(_puc_mac_addr) \
    (((_puc_mac_addr)[0] + (_puc_mac_addr)[1]           \
    + (_puc_mac_addr)[2] + (_puc_mac_addr)[3]           \
    + (_puc_mac_addr)[4] + (_puc_mac_addr)[5])          \
    & (MAC_VAP_USER_HASH_MAX_VALUE - 1))

#ifdef _PRE_WLAN_FEATURE_11AX
#define IS_HAL_DEVICE_SUPPORT_11AX(_pst_mac_vap) (OAL_TRUE == (_pst_mac_vap)->en_11ax_hal_cap)
#define IS_CUSTOM_OPEN_11AX_SWITCH(_pst_mac_vap) (OAL_TRUE == (_pst_mac_vap)->en_11ax_custom_switch)
#define MAC_VAP_IS_SUPPORT_11AX(_pst_mac_vap)                \
    (IS_HAL_DEVICE_SUPPORT_11AX(_pst_mac_vap) && IS_CUSTOM_OPEN_11AX_SWITCH(_pst_mac_vap))
#define IS_CUSTOM_OPEN_MULTI_BSSID_SWITCH(_pst_mac_vap) (OAL_TRUE == (_pst_mac_vap)->bit_multi_bssid_custom_switch)
#define IS_NON_TRANSMITTED_BSSID_BSS(_pst_mbss_info)         \
    ((OAL_TRUE == (_pst_mbss_info)->bit_is_non_transimitted_bss) && \
    (OAL_TRUE == (_pst_mbss_info)->bit_ext_cap_multi_bssid_activated))
#define IS_CUSTOM_OPEN_HTC_TOM(_pst_mac_vap)              \
    ((OAL_TRUE == (_pst_mac_vap)->bit_htc_include_custom_switch) && \
    (OAL_TRUE == (_pst_mac_vap)->bit_om_include_custom_switch))
#define VAP_SUPPORT_TOM(_pst_mac_vap) (MAC_VAP_IS_WORK_HE_PROTOCOL(_pst_mac_vap) && IS_CUSTOM_OPEN_HTC_TOM(_pst_mac_vap))
#define MAC_VAP_IS_WORK_HE_PROTOCOL(_pst_mac_vap) ((_pst_mac_vap)->en_protocol == WLAN_HE_MODE)
// for csec, cycle complexity
#define MAC_VAP_IS_STA_HE_MODE(_pst_vap, _en_vap_protocol) \
    ((_pst_vap)->en_vap_mode == WLAN_VAP_MODE_BSS_STA && (_en_vap_protocol) == WLAN_HE_MODE)
#define MAC_VAP_IS_STA_VHT_MODE(_pst_vap, _en_vap_protocol) \
    ((_pst_vap)->en_vap_mode == WLAN_VAP_MODE_BSS_STA && (_en_vap_protocol) == WLAN_VHT_MODE)
#endif

#define IS_AP(_pst_mac_vap)  (WLAN_VAP_MODE_BSS_AP  == (_pst_mac_vap)->en_vap_mode)
#define IS_STA(_pst_mac_vap) (WLAN_VAP_MODE_BSS_STA == (_pst_mac_vap)->en_vap_mode)

#define IS_P2P_DEV(_pst_mac_vap)    (WLAN_P2P_DEV_MODE == (_pst_mac_vap)->en_p2p_mode)
#define IS_P2P_GO(_pst_mac_vap)     (WLAN_P2P_GO_MODE == (_pst_mac_vap)->en_p2p_mode)
#define IS_P2P_CL(_pst_mac_vap)     (WLAN_P2P_CL_MODE == (_pst_mac_vap)->en_p2p_mode)
#define IS_LEGACY_VAP(_pst_mac_vap) (WLAN_LEGACY_VAP_MODE == (_pst_mac_vap)->en_p2p_mode)
#define IS_LEGACY_STA(_pst_mac_vap) (IS_STA(_pst_mac_vap) && IS_LEGACY_VAP(_pst_mac_vap))
#define IS_LEGACY_AP(_pst_mac_vap)  (IS_AP(_pst_mac_vap) && IS_LEGACY_VAP(_pst_mac_vap))
#define IS_BRIDGE_VAP(_pst_mac_vap) (IS_P2P_GO(_pst_mac_vap) &&\
    ((_pst_mac_vap)->ast_app_ie[OAL_APP_BEACON_IE].ul_ie_len == 0))

#define CIPHER_IS_WEP(cipher) ((WLAN_CIPHER_SUITE_WEP40 == (cipher)) || (WLAN_CIPHER_SUITE_WEP104 == (cipher)))

#ifdef _PRE_WLAN_DFT_STAT
#define MAC_VAP_STATS_PKT_INCR(_member, _cnt)  ((_member) += (_cnt))
#define MAC_VAP_STATS_BYTE_INCR(_member, _cnt) ((_member) += (_cnt))
#endif

#define MAC_DATA_CONTAINER_HEADER_LEN 4
#define MAC_DATA_CONTAINER_MAX_LEN    512
#define MAC_DATA_CONTAINER_MIN_LEN    8 /* ����Ҫ����1���¼� */
#define MAC_DATA_HEADER_LEN           4

#define MAC_SEND_TWO_DEAUTH_FLAG 0xf000

#define MAC_DBB_SCALING_2G_RATE_NUM               12 /* 2G rate���ʵĸ��� */
#define MAC_DBB_SCALING_5G_RATE_NUM               8  /* 2G rate���ʵĸ��� */
#define MAC_DBB_SCALING_2G_RATE_OFFSET            0  /* 2G Rate dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_2G_HT20_MCS_OFFSET        12 /* 2G HT20 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_2G_HT40_MCS_OFFSET        20 /* 2G HT40 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_2G_HT40_MCS32_OFFSET      61 /* 2G HT40 mcs32 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_RATE_OFFSET            28 /* 5G Rate dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT20_MCS_OFFSET        40 /* 5G HT20 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT20_MCS8_OFFSET       36 /* 5G HT20 mcs8 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT40_MCS_OFFSET        48 /* 5G HT40 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT40_MCS32_OFFSET      60 /* 5G HT40 mcs32 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT80_MCS_OFFSET        66 /* 5G HT80 dbb scaling ����ƫ��ֵ */
#define MAC_DBB_SCALING_5G_HT80_MCS0_DELTA_OFFSET 2  /* 5G HT80 mcs0/1 dbb scaling ����ƫ��ֵ����ֵ */

#define MAC_EXT_PA_GAIN_MAX_LVL    2     /* ���ƻ�����PA���λ�� */
#define CUS_RF_PA_BIAS_REG_NUM     43    /* ���ƻ�RF����PAƫ�üĴ�������� */
#define CUS_DY_CALI_PARAMS_NUM     14    /* ��̬У׼��������,2.4g 3��(ofdm 20/40 11b cw),5g 5*2��band */
#define CUS_DY_CALI_PARAMS_TIMES   3     /* ��̬У׼����������ϵ������ */
#define CUS_DY_CALI_DPN_PARAMS_NUM 4     /* ���ƻ���̬У׼2.4G DPN��������11b OFDM20/40 CW OR 5G 160/80/40/20 */
#define CUS_DY_CALI_2G_VAL_DPN_MAX (50)  /* ��̬У׼2g dpn��ȡnvram���ֵ */
#define CUS_DY_CALI_2G_VAL_DPN_MIN (-50) /* ��̬У׼2g dpn��ȡnvram��Сֵ */
#define CUS_DY_CALI_5G_VAL_DPN_MAX (50)  /* ��̬У׼5g dpn��ȡnvram���ֵ */
#define CUS_DY_CALI_5G_VAL_DPN_MIN (-50) /* ��̬У׼5g dpn��ȡnvram��Сֵ */

#if defined(_PRE_WLAN_FEATURE_11K)
#define MAC_11K_SUPPORT_AP_CHAN_RPT_NUM 8
#define MAC_MEASUREMENT_RPT_FIX_LEN     5
#define MAC_BEACON_RPT_FIX_LEN          26
#define MAC_MAX_RPT_DETAIL_LEN          224 /* 255 - 26(bcn fix) - 3(Meas rpt fix) - 2(subid 1) */

#define MAC_ACTION_RPT_FIX_LEN                 3
#define MAC_RADIO_MEAS_ACTION_REQ_FIX_LEN      5
#define MAC_NEIGHBOR_REPORT_ACTION_REQ_FIX_LEN 3
#define MAC_NEIGHBOR_REPORT_ACTION_RSP_FIX_LEN 3
#define MAC_NEIGHBOR_RPT_MIN_LEN               13
#endif
#define HMAC_RSP_MSG_MAX_LEN 128 /* get wid������Ϣ��󳤶� */

#define MAC_TEST_INCLUDE_FRAME_BODY_LEN 64

#define MAC_VAP_AMPDU_HW 8

#define MAC_VAP_GET_CAP_BW(_pst_mac_vap)          ((_pst_mac_vap)->st_channel.en_bandwidth)
#define MAC_VAP_CHECK_VAP_STATUS_VALID(_pst_mac_vap) (MAC_VAP_VAILD == (_pst_mac_vap)->uc_init_flag)

#ifdef _PRE_WLAN_FEATURE_DFS
#define MAC_VAP_GET_SUPPORT_DFS(_pst_mac_vap) ((_pst_mac_vap)->en_radar_detector_is_supp)
#define MAC_VAP_IS_SUPPORT_DFS(_pst_mac_vap)  (OAL_TRUE == (_pst_mac_vap)->en_radar_detector_is_supp)
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
#define MAC_VAP_IS_2G_SPEC_SISO(_pst_mac_vap) \
    ((WLAN_BAND_2G == (_pst_mac_vap)->st_channel.en_band && OAL_TRUE == (_pst_mac_vap)->st_cap_flag.bit_2g_custom_siso))
#define MAC_VAP_IS_5G_SPEC_SISO(_pst_mac_vap) \
    ((WLAN_BAND_5G == (_pst_mac_vap)->st_channel.en_band && OAL_TRUE == (_pst_mac_vap)->st_cap_flag.bit_5g_custom_siso))

#define MAC_VAP_SPEC_IS_SW_NEED_M2S_SWITCH(_pst_mac_vap) \
    (MAC_VAP_IS_2G_SPEC_SISO(_pst_mac_vap) || MAC_VAP_IS_5G_SPEC_SISO(_pst_mac_vap))
#endif

#define IS_USER_SINGLE_NSS(_pst_mac_user) (WLAN_SINGLE_NSS == (_pst_mac_user)->en_user_max_cap_nss)

#define IS_VAP_SINGLE_NSS(_pst_mac_vap) (WLAN_SINGLE_NSS == (_pst_mac_vap)->en_vap_rx_nss)

#define MAC_VAP_FOREACH_USER(_pst_user, _pst_vap, _pst_list_pos)                          \
    for ((_pst_list_pos) = (_pst_vap)->st_mac_user_list_head.pst_next,                    \
        (_pst_user) = oal_dlist_get_entry((_pst_list_pos), mac_user_stru, st_user_dlist); \
         (_pst_list_pos) != &((_pst_vap)->st_mac_user_list_head);                         \
         (_pst_list_pos) = (_pst_list_pos)->pst_next,                                     \
        (_pst_user) = oal_dlist_get_entry((_pst_list_pos), mac_user_stru, st_user_dlist)) \
        if (OAL_PTR_NULL != (_pst_user))

/* 3 ö�ٶ��� */
#define MAC_VAP_AP_STATE_BUTT  (MAC_VAP_STATE_AP_WAIT_START + 1)
#define MAC_VAP_STA_STATE_BUTT MAC_VAP_STATE_BUTT

#define H2D_SYNC_MASK_BARK_PREAMBLE (1 << 1)
#define H2D_SYNC_MASK_MIB           (1 << 2)
#define H2D_SYNC_MASK_PROT          (1 << 3)
enum {
    FREQ_IDLE = 0,

    FREQ_MIDIUM = 1,

    FREQ_HIGHER = 2,

    FREQ_HIGHEST = 3,

    FREQ_BUTT = 4
};
#define FREQ_TC_EN   1
#define FREQ_TC_EXIT 0

typedef uint8_t oal_device_freq_type_enum_uint8;


#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
typedef struct {
    int8_t *puc_string;
    mac_tcp_ack_buf_cfg_cmd_enum_uint8 en_tcp_ack_buf_cfg_id;
    uint8_t auc_resv[3];
} mac_tcp_ack_buf_cfg_table_stru;
#endif

typedef struct {
    int8_t *puc_car_name;
    mac_rssi_limit_type_enum_uint8 en_rssi_cfg_id;
    uint8_t auc_resv[3];
} mac_rssi_cfg_table_stru;

#define WAL_HIPRIV_RATE_INVALID 255 /*  ��Ч��������ֵ  */

enum {
    PKT_STAT_SET_ENABLE = 0,
    PKT_STAT_SET_START_STAT = 1,
    PKT_STAT_SET_FRAME_LEN = 2,
    PKT_STAT_SET_BUTT
};
typedef uint8_t oal_pkts_stat_enum_uint8;

enum {
    OAL_SET_ENABLE = 0,
    OAL_SET_MAX_BUFF = 1,
    OAL_SET_THRESHOLD = 2,
    OAL_SET_LOG_PRINT = 3,
    OAL_SET_CLEAR = 10,

    OAL_SET_BUTT
};
typedef uint8_t oal_stat_enum_uint8;

/* 4 ȫ�ֱ������� */
#ifdef _PRE_WLAN_FEATURE_M2S
extern oal_bool_enum_uint8 g_en_mimo_blacklist;
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
extern oal_bool_enum_uint8 g_en_hid2d_debug;
#endif
#ifdef _PRE_WLAN_FEATURE_MBO
extern uint8_t g_uc_mbo_switch;
#endif
extern uint8_t g_uc_dbac_dynamic_switch;

/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
#ifdef _PRE_WLAN_FEATURE_11AX
/* TWT�������ò��� */
typedef struct {
    uint64_t twt_start_time;
    uint32_t twt_duration;
    uint32_t twt_interval;
    uint8_t twt_flow_id;
    uint8_t twt_announce_bit;
    uint8_t twt_trigger_bit;
    uint8_t twt_session_enable;
    uint8_t uc_is_twt_ps_pause; /* TWT ʡ����з����Ƿ���pause״̬ */
    uint8_t auc_rsv[3];
} mac_cfg_twt_param_stru;
#endif

#define SHORTGI_CFG_STRU_LEN 4

typedef struct {
    uint8_t uc_next_expect_cnt;
    uint8_t uc_csa_scan_after_tbtt;
    uint8_t uc_sta_csa_last_cnt;              /* �յ� ap��һ�η��͵��л����� */
    oal_bool_enum_uint8 en_is_fsm_attached : 4; /* ״̬���Ƿ��Ѿ�ע�� */
    oal_bool_enum_uint8 en_expect_cnt_status : 4;
    uint8_t uc_associate_channel; /* csaɨ���ȡprobe response ֡�ŵ� */
    uint8_t csaIeFrameType;         /* ��ǰcsa��Ϣ����ʲô���͵�֡ */
    uint8_t csaIeLastFrameType;     /* ��һ��csa��Ϣ����ʲô���͵�֡ */
    uint8_t auc_resv[1];          /* resv */
    oal_fsm_stru st_oal_fsm;        /* csa״̬�� */
    frw_timeout_stru st_csa_handle_timer;
    uint8_t _rom[4];
} mac_sta_csa_fsm_info_stru; // only device

typedef struct {
    uint8_t uc_rs_nrates; /* ���ʸ��� */
    uint8_t auc_resv[3];
    mac_data_rate_stru ast_rs_rates[WLAN_MAX_SUPP_RATES];
} mac_rateset_stru;

typedef struct {
    uint8_t uc_br_rate_num;  /* �������ʸ��� */
    uint8_t uc_nbr_rate_num; /* �ǻ������ʸ��� */
    uint8_t uc_max_rate;     /* ���������� */
    uint8_t uc_min_rate;     /* ��С�������� */
    mac_rateset_stru st_rate;
} mac_curr_rateset_stru;

/* wme���� */
typedef struct {
    uint32_t ul_aifsn;      /* AIFSN parameters */
    uint32_t ul_logcwmin;   /* cwmin in exponential form, ��λ2^n -1 slot */
    uint32_t ul_logcwmax;   /* cwmax in exponential form, ��λ2^n -1 slot */
    uint32_t ul_txop_limit; /* txopLimit, us */
} mac_wme_param_stru;

#ifdef _PRE_WLAN_DELAY_STATISTIC
#define DL_TIME_ARRAY_LEN 10
#define TID_DELAY_STAT    0
#define AIR_DELAY_STAT    1
typedef struct {
    uint16_t dl_time_array[DL_TIME_ARRAY_LEN];
    uint8_t dl_measure_type;
} user_delay_info_stru;

typedef struct {
    uint32_t dmac_packet_count_num;
    uint32_t dmac_report_interval;
    uint8_t dmac_stat_enable;
    uint8_t reserved[3];
} user_delay_switch_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_TXBF
typedef struct {
    uint8_t bit_imbf_receive_cap : 1, /* ��ʽTxBf�������� */
              bit_exp_comp_txbf_cap : 1,      /* Ӧ��ѹ���������TxBf������ */
              bit_min_grouping : 2,           /* 0=�����飬1=1,2���飬2=1,4���飬3=1,2,4���� */
              bit_csi_bfee_max_rows : 2,      /* bfer֧�ֵ�����bfee��CSI��ʾ������������� */
              bit_channel_est_cap : 2;        /* �ŵ����Ƶ�������0=1��ʱ�������ε��� */
    uint8_t auc_resv0[3];
} mac_vap_txbf_add_stru;
#endif

typedef struct {
    uint8_t *puc_ie;        /* APP ��ϢԪ�� */
    uint32_t ul_ie_len;     /* APP ��ϢԪ�س��� */
    uint32_t ul_ie_max_len; /* APP ��ϢԪ����󳤶� */
} mac_app_ie_stru;

typedef struct {
    uint8_t uc_dscp;
    uint8_t uc_tid;
    uint8_t auc_reserve[2];
} mac_map_dscp_to_tid_stru; // only host

#ifdef _PRE_WLAN_DFT_STAT
typedef oam_stats_vap_stat_stru mac_vap_dft_stats_stru;

typedef struct {
    mac_vap_dft_stats_stru *pst_vap_dft_stats;
    frw_timeout_stru st_vap_dft_timer;
    uint32_t ul_flg; /* ��ʼͳ�Ʊ�־ */
} mac_vap_dft_stru;
#endif

/* Power save modes specified by the user */
typedef enum {
    NO_POWERSAVE = 0,
    MIN_FAST_PS = 1,  /* idle ʱ�����ini�е����� */
    MAX_FAST_PS = 2,  /* idle ʱ�����ini�е����� */
    AUTO_FAST_PS = 3, /* �����㷨�Զ�����idleʱ����ini�е����޻�������ֵ */
    MAX_PSPOLL_PS = 4,
    NUM_PS_MODE = 5
} ps_user_mode_enum;

/* VoWiFi�ź��������� �� ���ò����ṹ�� */
typedef enum {
    VOWIFI_DISABLE_REPORT = 0,
    VOWIFI_LOW_THRES_REPORT,
    VOWIFI_HIGH_THRES_REPORT,
    VOWIFI_CLOSE_REPORT = 3, /* �ر�VoWIFI */

    VOWIFI_MODE_BUTT = 3
} mac_vowifi_mode;
typedef uint8_t mac_vowifi_mode_enum_uint8;

typedef struct {
    /*  MODE
    0: disable report of rssi change
    1: enable report when rssi lower than threshold(vowifi_low_thres)
    2: enable report when rssi higher than threshold(vowifi_high_thres)
    */
    mac_vowifi_mode_enum_uint8 en_vowifi_mode;
    uint8_t uc_trigger_count_thres; /* ��1��100��, the continuous counters of lower or higher than threshold which will trigger the report to host */
    int8_t c_rssi_low_thres;        /* [-1, -100],vowifi_low_thres */
    int8_t c_rssi_high_thres;       /* [-1, -100],vowifi_high_thres */
    uint16_t us_rssi_period_ms;     /* ��λms, ��Χ��1s��30s��, the period of monitor the RSSI when host suspended */

    oal_bool_enum_uint8 en_vowifi_reported; /* ���vowifi�Ƿ��ϱ���һ��"״̬�л�����"���������ϱ� */
    uint8_t uc_cfg_cmd_cnt;               /* �ϲ��·�������vowifi�����Ĵ���ͳ�ƣ����ڱ���Ƿ����ײ������·���ȫ����ֹ�·��������̺�vowifi�����ϱ������ص� */
    uint8_t _rom[4];
} mac_vowifi_param_stru;

#define MAC_VAP_INVAILD (0x0) /* 0Ϊvap��Ч */
#define MAC_VAP_VAILD   (0x2b)
typedef struct {
    oal_bool_enum_uint8 en_rate_cfg_tag;
    oal_bool_enum_uint8 en_tx_ampdu_last;
    uint8_t uc_reserv[2];
} mac_fix_rate_cfg_para_stru;

typedef struct {
    uint8_t uc_test_ie_info[MAC_TEST_INCLUDE_FRAME_BODY_LEN];
    oal_bool_enum_uint8 en_include_test_ie;
    uint8_t uc_test_ie_len;
    uint8_t uc_include_times;
    uint8_t uc_reserv[1];
} mac_beacon_include_test_ie_stru;

#ifdef _PRE_WLAN_FEATURE_MBO
typedef struct {
    uint8_t uc_mbo_enable;    /* MBO���Զ��ƻ����� */
    uint8_t uc_mbo_cell_capa; /* MBO STA�ƶ��������� */
    uint8_t uc_mbo_assoc_disallowed_test_switch;
    uint8_t en_disable_connect;                                 /* �Ƿ���re-assoc req֡����ӦBSS��־λ */
    uint32_t ul_btm_req_received_time;                          /* �յ�BTM Req֡��ʱ�� */
    uint32_t ul_re_assoc_delay_time;                            /* NeighborԪ���л�õ�re-assoc delay time ms */
    uint8_t auc_re_assoc_delay_bss_mac_addr[WLAN_MAC_ADDR_LEN]; /* ��re-assoc delay��BSS��ַ */
    uint8_t auc_resv[2];
} mac_mbo_para_stru;
#endif

/* MAC TX RX common report�����Ӧö�� */
typedef enum {
    MAC_TX_COMMON_REPORT_INDEX = 0,
    MAC_TX_COMMON_REPORT_STATUS,
    MAC_TX_COMMON_REPORT_BW_MODE,
    MAC_TX_COMMON_REPORT_PROTOCOL_MODE,
    MAC_TX_COMMON_REPORT_FRAME_TYPE,
    MAC_TX_COMMON_REPORT_SUB_TYPE = 5,
    MAC_TX_COMMON_REPORT_APMDU,
    MAC_TX_COMMON_REPORT_PSDU,
    MAC_TX_COMMON_REPORT_HW_RETRY,
    MAC_TX_COMMON_REPORT_CTRL_REG_CLEAR,

    MAC_TX_COMMON_REPORT_DEBUG_BUTT
} mac_tx_common_report_debug_type_enum;
typedef uint8_t mac_tx_common_report_debug_type_enum_uint8;

typedef enum {
    MAC_RX_COMMON_REPORT_INDEX = 0,
    MAC_RX_COMMON_REPORT_STATUS,
    MAC_RX_COMMON_REPORT_BW_MODE,
    MAC_RX_COMMON_REPORT_PROTOCOL_MODE,
    MAC_RX_COMMON_REPORT_FRAME_TYPE,
    MAC_RX_COMMON_REPORT_SUB_TYPE = 5,
    MAC_RX_COMMON_REPORT_APMDU,
    MAC_RX_COMMON_REPORT_PSDU,
    MAC_RX_COMMON_REPORT_VAP_CHK,
    MAC_RX_COMMON_REPORT_BSS_CHK,
    MAC_RX_COMMON_REPORT_DIRECT_CHK = 10,
    MAC_RX_COMMON_REPORT_CTRL_REG_CLEAR,

    MAC_RX_COMMON_REPORT_DEBUG_BUTT
} mac_rx_common_report_debug_type_enum;
typedef uint8_t mac_rx_common_report_debug_type_enum_uint8;

typedef enum {
    PG_EFFICIENCY_STATISTICS_ENABLE = 0,
    PG_EFFICIENCY_INFO,

    MAC_VAP_COMMON_SET_TXQ,
    MAC_VAP_COMMON_TX_INFO,
#ifdef _PRE_WLAN_FEATURE_MBO
    MBO_SWITCH,                  /* ��������1:MBO���Կ���,out-of-the-box test */
    MBO_CELL_CAP,                /* ��������2:MBO Capability Indication test */
    MBO_ASSOC_DISALLOWED_SWITCH, /* ��������6 APͨ����MBO IEָʾ���������� */
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    HIEX_DEV_CAP, /* HIEX device������������ */
    HIEX_DEBUG,   /* HIEX����debug���� */
#endif

    RIFS_ENABLE,           /* rifs����ʹ�� */
    GREENFIELD_ENABLE,     /* GreenField����ʹ�� */
    PROTECTION_LOG_SWITCH, /* ����ģʽά���ӡ */

#ifdef _PRE_WLAN_FEATURE_11AX
    DMAC_RX_FRAME_STATISTICS, /* ����֡��Ϣͳ�� */
    DMAC_TX_FRAME_STATISTICS, /* ����֡��Ϣͳ�� */
#endif
    AUTH_RSP_TIMEOUT, /* ��֤ʱ����auth �׶γ�ʱʱ�� */
    FORBIT_OPEN_AUTH, /* ��ֹopen���� */
    HT_SELF_CURE_DEBUG,
    USERCTL_BINDCPU,  /* �û�ָ��Ӳ�жϺ��̰߳�� */

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    HAL_PS_DEBUG_SWITCH,
#endif
    SU_PPDU_1xLTF_08US_GI_SWITCH,

    MAC_COMMON_DEBUG_BUTT
} mac_common_debug_type_enum;
typedef uint8_t mac_common_debug_type_enum_uint8;

typedef enum {
    MAC_HW_ALTX_VECT0,
    MAC_HW_ALTX_VECT1,
    MAC_HW_ALTX_VECT2,
    MAC_HW_ALTX_VECT3,
    MAC_HW_ALTX_VECT4,
    MAC_HW_ALTX_VECT5,
    MAC_HW_ALTX_VECT6,
    MAC_HW_ALTX_VECT7,

    MAC_HW_ALTX_EN,
    MAC_HW_ALTX_MODE,
    MAC_HW_ALTX_DATA,
    MAC_HW_ALTX_COUNT,
    MAC_HW_ALTX_GAP,
} mac_hw_altx_type_enum;

typedef enum {
    MAC_LOG_ENCAP_AMPDU,
#ifdef _PRE_WLAN_FEATURE_MONITOR
    MAC_LOG_MONITOR_OTA_RPT
#endif
} mac_debug_log_type_enum;

#ifdef _PRE_WLAN_FEATURE_11AX
typedef enum {
    MAC_VAP_11AX_DEBUG_PRINT_LOG,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_TID_NUM,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_HTC_ORGER,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_HTC_VALUE,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_FIX_POWER,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_POWER_VALUE,
    MAC_VAP_11AX_DEBUG_HE_TB_PPDU_DISABLE_BA_CHECK,
    MAC_VAP_11AX_DEBUG_DISABLE_MU_EDCA,
    MAC_VAP_11AX_DEBUG_MANUAL_CFO,
    MAC_VAP_11AX_DEBUG_BSRP_CFG,
    MAC_VAP_11AX_DEBUG_BSRP_TID,
    MAC_VAP_11AX_DEBUG_BSRP_QUEUE_SIZE,
    MAC_VAP_11AX_DEBUG_MAC_PADDING,
    MAC_VAP_11AX_DEBUG_POW_TEST,
    MAC_VAP_11AX_DEBUG_TB_PPDU_LEN,
    MAC_VAP_11AX_DEBUG_TB_PPDU_AC,
    MAC_VAP_11AX_DEBUG_PRINT_RX_TRIG_FRAME,
    MAC_VAP_11AX_DEBUG_OM_AUTH_FLAG,
    MAC_VAP_11AX_DEBUG_TOM_BW_FLAG,
    MAC_VAP_11AX_DEBUG_TOM_NSS_FLAG,
    MAC_VAP_11AX_DEBUG_TOM_UL_MU_DISABLE_FLAG,
    MAC_VAP_11AX_DEBUG_WIFI6_SELF_CURE_FLAG,
    MAC_VAP_11AX_DEBUG_UORA_OCW_UPDATE,
    MAC_VAP_11AX_DEBUG_UORA_OBO_BYPASS,
    MAC_VAP_11AX_DEBUG_BSS_COLOR,

    MAC_VAP_11AX_DEBUG_BUTT
} mac_vap_11ax_debug_type_enum;
typedef uint8_t mac_vap_11ax_debug_type_enum_uint8;

typedef enum {
    MAC_VAP_11AX_PARAM_CONFIG_INIT = 0,
    MAC_VAP_11AX_PARAM_CONFIG_UPDAT = 1,

    MAC_VAP_11AX_PARAM_CONFIG_BUTT
} mac_vap_11ax_param_config_enum;
typedef uint8_t mac_vap_11ax_param_config_enum_uint8;

typedef struct {
    uint8_t bit_new_bss_color_info : 6,
              bit_new_bss_color_exit : 2;
    uint8_t uc_rsv[3];
} mac_vap_he_config_info_stru;

typedef struct {
    uint8_t uc_enable_flag;
    uint8_t auc_srg_bss_color_bitmap[MAC_HE_SRG_BSS_COLOR_BITMAP_LEN];
    uint8_t auc_srg_partial_bssid_bitmap[MAC_HE_SRG_PARTIAL_BSSID_BITMAP_LEN];

} mac_vap_he_sr_config_info_stru;

#endif

typedef enum {
    MAC_PROBE_RESP_MODE_ACTIVE = 0, /* �����յ�probe req��������probe resp�����ظ����տ� */
    MAC_PROBE_RESP_MODE_PASSIVE = 1, /* �����յ�probe req��˳���ϲ�ľ���ѡ���Իظ�probe resp֡ */
    MAC_PROBE_RESP_MODE_BUTT
} mac_probe_resp_mode_enum;
typedef uint8_t mac_probe_resp_mode_enum_uint8;

typedef enum {
    MAC_PS_TYPE_DBAC = BIT0,
    MAC_PS_TYPE_BTCOEX = BIT1,
    MAC_PS_TYPE_CMD = BIT2,

    MAC_PS_TYPE_BUTT
} MAC_ps_type_enum;
typedef uint8_t mac_ps_type_enum_uint8;

typedef struct {
    oal_bool_enum_uint8 en_probe_resp_enable;    /* ��bool���ؿ���probe_resp_status�Ƿ���Ա��Ķ� */
    mac_probe_resp_mode_enum_uint8 en_probe_resp_status;    /* probe resp�ظ���ģʽ */
    uint8_t auc_rsv[2];
} mac_probe_resp_ctrl_stru;

/* STA UAPSD �������� */
/* �ýṹ��host��device����,�豣֤�ṹ��һ�� */
typedef struct {
    uint8_t uc_max_sp_len;
    uint8_t uc_delivery_enabled[WLAN_WME_AC_BUTT];
    uint8_t uc_trigger_enabled[WLAN_WME_AC_BUTT];
} mac_cfg_uapsd_sta_stru;

/* VAP�����ݽṹ */
typedef struct {
    /* VAPΪAP����STA���г�Ա */
    /* word0~word1 */
    uint8_t uc_vap_id; /* vap ID   */
    /* ����Դ������ֵ */
    uint8_t uc_device_id;           /* �豸ID   */
    uint8_t uc_chip_id;                 /* chip ID  */
    wlan_vap_mode_enum_uint8 en_vap_mode; /* vapģʽ  */
    uint32_t ul_core_id;

    /* word2~word3 */
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN]; /* BSSID����MAC��ַ��MAC��ַ��mib�е�auc_dot11StationID  */
    mac_vap_state_enum_uint8 en_vap_state;  /* VAP״̬ */
    wlan_protocol_enum_uint8 en_protocol;   /* ������Э��ģʽ */

    /* word4~word5 */
    mac_channel_stru st_channel; /* vap���ڵ��ŵ� */
    mac_ch_switch_info_stru st_ch_switch_info;
    mac_sta_csa_fsm_info_stru st_sta_csa_fsm_info;
    /* word6 */
    uint8_t bit_has_user_bw_limit : 1; /* ��vap�Ƿ����user���� */
    uint8_t bit_vap_bw_limit : 1;      /* ��vap�Ƿ������� */
    uint8_t bit_voice_aggr : 1;        /* ��vap�Ƿ����VOҵ��֧�־ۺ� */
    uint8_t bit_one_tx_tcp_be : 1;     /* ��vap�Ƿ�ֻ��1·����TCP BEҵ�� */
    uint8_t bit_one_rx_tcp_be : 1;     /* ��vap�Ƿ�ֻ��1·����TCP BEҵ�� */
    uint8_t bit_no_tcp_or_udp : 1;     /* ��vapû����TCP��UDPҵ�� */
    uint8_t bit_bw_fixed : 1;          /* APģʽ��VAP�Ĵ����Ƿ�̶�20M */
    uint8_t bit_use_rts_threshold : 1; /* ��vap��RTS�����Ƿ�ʹ��Э��涨��RTS���� */

    uint8_t uc_tx_power;                /* ���书��, ��λdBm */
    uint8_t uc_channel_utilization;     /* ��ǰ�ŵ������� */
    uint8_t uc_wmm_params_update_count; /* ��ʼΪ0��APģʽ�£�ÿ����һ��wmm�������������1,��beacon֡��assoc rsp�л���д��4bit�����ܳ���15��STAģʽ�½���֡���������ֵ */

    /* word7 */
    uint16_t us_user_nums;                                        /* VAP���ѹҽӵ��û����� */
    uint16_t us_multi_user_idx;                                   /* �鲥�û�ID */
    uint8_t auc_cache_user_mac_addr[WLAN_MAC_ADDR_LEN];           /* cache user��Ӧ��MAC��ַ */
    uint16_t us_cache_user_id;                                    /* cache user��Ӧ��userID */
    oal_dlist_head_stru ast_user_hash[MAC_VAP_USER_HASH_MAX_VALUE]; /* hash����,ʹ��HASH�ṹ�ڵ�DLIST */
    oal_dlist_head_stru st_mac_user_list_head;                      /* �����û��ڵ�˫������,ʹ��USER�ṹ�ڵ�DLIST */

    /* word8 */
    wlan_nss_enum_uint8 en_vap_rx_nss; /* vap�Ľ��տռ������� */
    /* vapΪ��̬��Դ�����VAP��û�б�����
      DMAC OFFLOADģʽVAP��ɾ������˻����֡ */
    uint8_t uc_init_flag;
    uint16_t us_mgmt_frame_filters; /* ����֡���ˣ��ο� en_fbt_mgmt_frame_filter_enum */

    mac_cap_flag_stru st_cap_flag;            /* vap�������Ա�ʶ */
    wlan_mib_ieee802dot11_stru *pst_mib_info; /* mib��Ϣ(��ʱ����vapʱ������ֱ�ӽ�ָ��ֵΪNULL����ʡ�ռ�)  */

    mac_curr_rateset_stru st_curr_sup_rates;                    /* ��ǰ֧�ֵ����ʼ� */
    mac_curr_rateset_stru ast_sta_sup_rates_ie[WLAN_BAND_BUTT]; /* ֻ��staȫ�ŵ�ɨ��ʱʹ�ã�������д֧�ֵ����ʼ�ie����2.4��5G */

#ifdef _PRE_WLAN_DFT_STAT
    /* user ����ά�� */
    uint32_t ul_dlist_cnt; /* dlsitͳ�� */
    uint32_t ul_hash_cnt;  /* hash����ͳ�� */
#endif

#ifdef _PRE_WLAN_FEATURE_TXBF
    mac_vap_txbf_add_stru st_txbf_add_cap;
#endif

    /* VAPΪAP����STA���г�Ա ������� */
    /* VAPΪAP���г�Ա�� ���忪ʼ */
    mac_protection_stru st_protection; /* �뱣����ر��� */
    mac_app_ie_stru ast_app_ie[OAL_APP_IE_NUM];
    oal_bool_enum_uint8 en_40M_intol_user; /* ap���Ƿ��й�����40M intolerant��user */
    oal_bool_enum_uint8 en_vap_wmm;        /* ap wmm ���� */

    uint8_t uc_random_mac;

    /* VAPΪAP�ض���Ա�� ������� */
    /* VAPΪSTA���г�Ա�� ���忪ʼ */
    uint16_t us_sta_aid;             /* VAPΪSTAģʽʱ����AP�����STA��AID(����Ӧ֡��ȡ),ȡֵ��Χ1~2007; VAPΪAPģʽʱ�����ô˳�Ա���� */
    uint16_t us_assoc_vap_id;        /* VAPΪSTAģʽʱ����user(ap)����Դ��������VAPΪAPģʽʱ�����ô˳�Ա���� */
    uint8_t uc_uapsd_cap;            /* ������STA������AP�Ƿ�֧��uapsd������Ϣ */
    uint16_t us_assoc_user_cap_info; /* staҪ�������û���������Ϣ */
    uint8_t bit_ap_11ntxbf : 1,      /* staҪ�������û���11n txbf������Ϣ */
              bit_rsp_soft_ctl_launcher : 1,
              bit_roam_scan_valid_rslt : 1,
              bit_ap_chip_oui : 3, /* staҪ�������û���оƬ���� */
              bit_mac_padding : 2; /* 11axЭ��ʱmac padding����ֵ,����WiFi6��֤ */

#ifdef _PRE_WLAN_NARROW_BAND
    mac_cfg_narrow_bw_stru st_nb;
#endif

    /* ��������ʹ�� */
    uint8_t bit_al_tx_flag : 1;   /* ������־ */
    uint8_t bit_payload_flag : 2; /* payload����:0:ȫ0  1:ȫ1  2:random */
    uint8_t bit_ack_policy : 1;   /* ack_policy����:0:normal 1:no ack , Ŀǰ��ʹ�� */

    /* ���ղ���ʹ�� */
    uint8_t bit_al_rx_flag : 1;         /* ���ձ�־ */
    uint8_t bit_bss_include_rrm_ie : 1; /* ������bss ����rrm ie */
    uint8_t bit_vap_support_csa : 1;
    uint8_t bit_reserved : 1;

    wlan_p2p_mode_enum_uint8 en_p2p_mode; /* 0:��P2P�豸; 1:P2P_GO; 2:P2P_Device; 3:P2P_CL */
    uint8_t uc_p2p_gocl_hal_vap_id;     /* p2p go / cl��hal vap id */
    uint8_t uc_p2p_listen_channel;      /* P2P Listen channel */

    mac_cfg_uapsd_sta_stru st_sta_uapsd_cfg; /* UAPSD��������Ϣ */

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    oal_bool_enum_uint8 en_user_pmf_cap; /* STA����δ����userǰ���洢Ŀ��user��pmfʹ����Ϣ */
#endif

    oal_spin_lock_stru st_cache_user_lock; /* cache_user lock */
    mac_vowifi_param_stru *pst_vowifi_cfg_param; /* �ϲ��·���"VoWiFi�ź���������"�����ṹ�� */
    mac_fix_rate_cfg_para_stru st_fix_rate_pre_para;
    mac_beacon_include_test_ie_stru st_ap_beacon_test_ie; /* 0305�д˽ṹ����device����� */
#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)
    uint8_t uc_he_mu_edca_update_count;
    uint8_t en_11ax_custom_switch : 1,
            bit_multi_bssid_custom_switch : 1,
            bit_htc_include_custom_switch : 1,
            bit_om_include_custom_switch : 1,
            bit_ofdma_aggr_num : 4;        /* 4*32���ڣ���amsdu������4*32��ֱ����8,��ampduӲ���ۺ� */
    uint8_t en_11ax_hal_cap :1,
            aput_bss_color_info : 7;
    uint8_t bit_om_auth_flag : 1,
            bit_om_auth_bw : 2,
            bit_om_auth_nss : 3,
            bit_om_auth_ul_mu_disable : 1,
            bit_rom_custom_switch : 1;

#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    mac_he_uora_para_stru st_he_uora_eocw;
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
    oal_bool_enum_uint8 en_radar_detector_is_supp; /* ��·hal device��֧���״�̽�⣬�����ж� */
    uint8_t auc_resv10[3];                       /* 4�ֽڶ��� */
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    wlan_mib_Dot11EDCAEntry_stru st_wlan_mib_mu_edca;
    wlan_mid_Dot11HePhyCapability_rom_stru st_wlan_rom_mib_he_phy_config;
    wlan_mib_Dot11HEStationConfigEntry_rom_stru st_wlan_rom_mib_he_sta_config;
    mac_vap_he_config_info_stru st_he_config_info;
    mac_vap_he_sr_config_info_stru st_he_sr_config_info;
    mac_cfg_twt_param_stru st_twt_param;
    mac_multi_bssid_info st_mbssid_info;
#endif
    uint32_t ul_resv[10];                              /* ��������ṹ�屻ROM��,ƫ�Ʋ��ɱ� */
    oal_bool_enum_uint8 en_dot11VHTExtendedNSSBWCapable; /* VHT cap Extended NSS BW Support�ֶ��Ƿ���Ч */
    mac_vap_state_enum_uint8 en_last_vap_state;          /* wlan0 ���������±���VAP �������ǰ��״̬ */
    mac_probe_resp_ctrl_stru st_probe_resp_ctrl;
    uint8_t uc_ps_type;
    oal_bool_enum_uint8 en_ps_rx_amsdu;

#ifdef _PRE_WLAN_FEATURE_MBO
    mac_mbo_para_stru st_mbo_para_info;
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    uint8_t uc_himit_enable;
    uint8_t auc_resv[3];
#endif
    uint32_t ul_tb_aggr_bytes;        /* AP UL LENGTHָ���ľۺ��ֽ�������λ���ֽ� */
    uint8_t is_primary_vap; /* ����Ƿ�Ϊwlan0 */
} mac_vap_stru;

/* cfg id��Ӧ��get set���� */
typedef struct {
    wlan_cfgid_enum_uint16 en_cfgid;
    uint8_t auc_resv[2]; /* �ֽڶ��� */
    uint32_t (*p_get_func)(mac_vap_stru *pst_vap, uint8_t *puc_len, uint8_t *puc_param);
    uint32_t (*p_set_func)(mac_vap_stru *pst_vap, uint8_t uc_len, uint8_t *puc_param);
} mac_cfgid_stru;

typedef enum {
    HW_PAYLOAD_ALL_ZERO = 0,
    HW_PAYLOAD_ALL_ONE,
    HW_PAYLOAD_RAND,
    HW_PAYLOAD_REPEAT,
    HW_PAYLOAD_BUTT
} mac_hw_payload_enum;
typedef uint8_t mac_hw_payload_enum_uint8;

/* ����Ϊ�����ں����ò���ת��Ϊ�����ڲ������·��Ľṹ�� */
/* �����ں����õ�ɨ��������·���������ɨ����� */
typedef struct {
    oal_ssids_stru st_ssids[WLAN_SCAN_REQ_MAX_SSID];
    int32_t l_ssid_num;

    uint8_t *puc_ie;
    uint32_t ul_ie_len;

    oal_scan_enum_uint8 en_scan_type;
    uint8_t uc_num_channels_2G;
    uint8_t uc_num_channels_5G;
    uint8_t auc_arry[1];

    uint32_t *pul_channels_2G;
    uint32_t *pul_channels_5G;
    uint32_t ul_scan_flag;

    /* WLAN/P2P ��������£�p2p0 ��p2p-p2p0 cl ɨ��ʱ����Ҫʹ�ò�ͬ�豸������bit_is_p2p0_scan������ */
    uint8_t bit_is_p2p0_scan : 1; /* �Ƿ�Ϊp2p0 ����ɨ�� */
    uint8_t bit_rsv : 7;          /* ����λ */
    uint8_t auc_rsv[3];           /* ����λ */
} mac_cfg80211_scan_param_stru;

typedef struct {
    uint8_t uc_channel;  /* ap�����ŵ���ţ�eg 1,2,11,36,40... */
    uint8_t uc_ssid_len; /* SSID ���� */
    mac_nl80211_mfp_enum_uint8 en_mfp;
    uint8_t uc_wapi;

    uint8_t *puc_ie;
    uint8_t auc_ssid[32];                /* ����������AP SSID  */
    uint8_t auc_bssid[OAL_MAC_ADDR_LEN]; /* ����������AP BSSID  */

    oal_bool_enum_uint8 en_privacy;                /* �Ƿ���ܱ�־ */
    oal_nl80211_auth_type_enum_uint8 en_auth_type; /* ��֤���ͣ�OPEN or SHARE-KEY */

    uint8_t uc_wep_key_len;           /* WEP KEY���� */
    uint8_t uc_wep_key_index;         /* WEP KEY���� */
    uint8_t *puc_wep_key;             /* WEP KEY��Կ */
    mac_crypto_settings_stru st_crypto; /* ��Կ�׼���Ϣ */
    uint32_t ul_ie_len;
} mac_conn_param_stru; // only host

#ifdef _PRE_WLAN_FEATURE_11R
#define MAC_MAX_FTE_LEN 257

typedef struct {
    uint16_t us_mdid; /* Mobile Domain ID */
    uint16_t us_len;  /* FTE �ĳ��� */
    uint8_t auc_ie[MAC_MAX_FTE_LEN];
} mac_cfg80211_ft_ies_stru; // only host
#endif  // _PRE_WLAN_FEATURE_11R

typedef struct {
    uint8_t auc_mac_addr[OAL_MAC_ADDR_LEN];
    uint8_t auc_rsv[2];
} mac_cfg80211_init_port_stru; // only host

/* ��mac_cfg_lpm_wow_en_stru����Ų��_PRE_WLAN_CHIP_TEST���棬ʹ�ùر�_PRE_WLAN_CHIP_TESTҲ������������ */
typedef struct mac_cfg_lpm_wow_en {
    uint8_t uc_en;        /* 0:off, 1:on */
    uint8_t uc_null_wake; /* 1:null data wakeup */
} mac_cfg_lpm_wow_en_stru;

typedef struct {
    uint8_t auc_mac_da[WLAN_MAC_ADDR_LEN];
    uint8_t uc_category;
    uint8_t auc_resv[1];
} mac_cfg_send_action_param_stru; // only host


#ifdef _PRE_WLAN_DFT_STAT
typedef struct {
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN];
    uint8_t uc_param;
    uint8_t uc_resv;
} mac_cfg_usr_queue_param_stru;
#endif


typedef struct {
    uint16_t us_duration_ms;
    oal_bool_enum_uint8 en_probe;
    uint8_t uc_chan_cnt;
    uint8_t auc_channels[WLAN_MAX_CHANNEL_NUM];
} mac_chan_stat_param_stru; // only host
typedef struct {
    uint32_t ul_coext_info;
    uint32_t ul_channel_report;
} mac_cfg_set_2040_coexist_stru; // only host

typedef struct {
    uint8_t uc_performance_log_switch_type;
    uint8_t uc_value;
    uint8_t auc_resv[2];
} mac_cfg_set_performance_log_switch_stru;

typedef struct {
    uint8_t uc_scan_type;
    oal_bool_enum_uint8 en_current_bss_ignore;
    uint8_t auc_bssid[OAL_MAC_ADDR_LEN];
} mac_cfg_set_roam_start_stru; // only host


typedef struct {
    uint32_t ul_frag_threshold;
} mac_cfg_frag_threshold_stru; // only host

typedef struct {
    uint32_t ul_rts_threshold;
} mac_cfg_rts_threshold_stru; // only host

typedef struct {
    /* software_retryֵ */
    uint8_t uc_software_retry;
    /* �Ƿ�ȡtest���õ�ֵ��Ϊ0��Ϊ������������ */
    uint8_t uc_retry_test;

    uint8_t uc_pad[2];
} mac_cfg_set_soft_retry_stru; // only host

typedef struct {
    oal_net_device_stru *pst_netdev;
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t auc_resv1[2];
    uint8_t *puc_mac_addr;
    void *cookie;
    void (*callback)(void *cookie, oal_key_params_stru *key_param);
} mac_getkey_param_stru; // only host

typedef struct {
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_unicast;
    oal_bool_enum_uint8 en_multicast;
    uint8_t auc_resv1[1];
} mac_setdefaultkey_param_stru; // only host

typedef struct {
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv0[2];
    uint8_t auc_pmkid[WLAN_PMKID_LEN];
} mac_cfg_pmksa_param_stru; // only host

typedef struct {
    uint64_t ull_cookie;
    uint32_t ul_listen_duration;                            /* ����ʱ��   */
    uint8_t uc_listen_channel;                              /* �������ŵ� */
    wlan_channel_bandwidth_enum_uint8 en_listen_channel_type; /* �����ŵ����� */
    uint8_t uc_home_channel;                                /* �����������ص��ŵ� */
    wlan_channel_bandwidth_enum_uint8 en_home_channel_type;   /* �����������������ŵ����� */
    /* P2P0��P2P_CL ����VAP �ṹ������������ǰVAP ��״̬�����ڼ�������ʱ�ָ���״̬ */
    mac_vap_state_enum_uint8 en_last_vap_state;
    wlan_channel_band_enum_uint8 en_band;
    wlan_ieee80211_roc_type_uint8 en_roc_type; /* roc���� */
    uint8_t uc_rev;
    oal_ieee80211_channel_stru st_listen_channel;

} mac_remain_on_channel_param_stru; // only host

typedef struct {
    uint8_t uc_pool_id;
} mac_device_pool_id_stru;

/* WPAS ����֡���ͽṹ */
typedef struct {
    int32_t channel;
    uint8_t mgmt_frame_id;
    oal_netbuf_stru *frame;
} mac_mgmt_frame_stru; // only host


typedef enum {
    WPAS_CONNECT_STATE_INIT = 0,
    WPAS_CONNECT_STATE_START = 1,
    WPAS_CONNECT_STATE_ASSOCIATED = 2,
    WPAS_CONNECT_STATE_HANDSHAKED = 3,
    WPAS_CONNECT_STATE_IPADDR_OBTAINED = 4,
    WPAS_CONNECT_STATE_IPADDR_REMOVED = 5,
    WPAS_CONNECT_STATE_BUTT
} wpas_connect_state_enum;
typedef uint32_t wpas_connect_state_enum_uint32;

/* ���ƻ� power ref 2g 5g���ò��� */
/* customize rf cfg struct */
typedef struct {
    int8_t c_rf_gain_db_mult4; /* �ⲿPA/LNA bypassʱ������(����0.25dB) */
    uint8_t auc_resv[3];
} mac_cfg_gain_db_per_band;

typedef struct {
    /* 2g */
    mac_cfg_gain_db_per_band ac_gain_db_2g[MAC_NUM_2G_BAND];
    /* 5g */
    mac_cfg_gain_db_per_band ac_gain_db_5g[MAC_NUM_5G_BAND];
} mac_cfg_custom_gain_db_stru;

typedef struct {
    int8_t c_cfg_delta_pwr_ref_rssi_2g[CUS_NUM_2G_DELTA_RSSI_NUM];
    int8_t c_cfg_delta_pwr_ref_rssi_5g[CUS_NUM_5G_DELTA_RSSI_NUM];
} mac_cfg_custom_delta_pwr_ref_stru;

typedef struct {
    int8_t ac_cfg_delta_amend_rssi_2g[CUS_NUM_2G_AMEND_RSSI_NUM];
    int8_t c_rsv;
    int8_t ac_cfg_delta_amend_rssi_5g[CUS_NUM_5G_AMEND_RSSI_NUM];
} mac_cfg_custom_amend_rssi_stru;

typedef struct {
    oal_int8 filter_narrowing_amend_rssi_2g[CUS_2G_FILTER_NARROW_AMEND_RSSI_NUM];
    oal_int8 filter_narrowing_amend_rssi_5g[CUS_5G_FILTER_NARROW_AMEND_RSSI_NUM];
    oal_int8 ac_rsv[3]; /* reserved 3�ֽ� */
} mac_cfg_custom_filter_narrow_amend_rssi_stru;

typedef struct {
    uint16_t us_lna_on2off_time_ns; /* LNA����LNA�ص�ʱ��(ns) */
    uint16_t us_lna_off2on_time_ns; /* LNA�ص�LNA����ʱ��(ns) */

    int8_t c_lna_bypass_gain_db;   /* �ⲿLNA bypassʱ������(dB) */
    int8_t c_lna_gain_db;          /* �ⲿLNA����(dB) */
    int8_t c_pa_gain_b0_db;        /* �ⲿPA b0 ����(dB) */
    uint8_t uc_ext_switch_isexist; /* �Ƿ�ʹ���ⲿswitch */

    uint8_t uc_ext_pa_isexist;                  /* �Ƿ�ʹ���ⲿpa */
    oal_fem_lp_state_enum_uint8 en_fem_lp_enable; /* �Ƿ�֧��fem�͹��ı�־ */
    int8_t c_fem_spec_value;                    /* fem spec���ʵ� */
    uint8_t uc_ext_lna_isexist;                 /* �Ƿ�ʹ���ⲿlna */

    int8_t c_pa_gain_b1_db;     /* �ⲿPA b1����(dB) */
    uint8_t uc_pa_gain_lvl_num; /* �ⲿPA ���浵λ�� */
    uint8_t auc_resv[2];
} mac_cfg_custom_ext_rf_stru;

typedef struct {
    mac_cfg_custom_gain_db_stru ast_rf_gain_db_rf[WLAN_RF_CHANNEL_NUMS];           /* 2.4g 5g ���� */
    mac_cfg_custom_ext_rf_stru ast_ext_rf[WLAN_BAND_BUTT][WLAN_RF_CHANNEL_NUMS];   /* 2.4g 5g fem */
    mac_cfg_custom_delta_pwr_ref_stru ast_delta_pwr_ref_cfg[WLAN_RF_CHANNEL_NUMS]; /* delta_rssi */
    mac_cfg_custom_amend_rssi_stru ast_rssi_amend_cfg[WLAN_RF_CHANNEL_NUMS];       /* rssi_amend */

    uint8_t uc_far_dist_pow_gain_switch;           /* ��Զ���빦�����濪�� */
    uint8_t uc_far_dist_dsss_scale_promote_switch; /* ��Զ��11b 1m 2m dbb scale����ʹ�ܿ��� */
    uint8_t uc_chn_radio_cap;
    int8_t c_delta_cca_ed_high_80th_5g;

    /* ע�⣬����޸��˶�Ӧ��λ�ã���Ҫͬ���޸ĺ���: hal_config_custom_rf  */
    int8_t c_delta_cca_ed_high_20th_2g;
    int8_t c_delta_cca_ed_high_40th_2g;
    int8_t c_delta_cca_ed_high_20th_5g;
    int8_t c_delta_cca_ed_high_40th_5g;
    uint32_t aul_5g_upc_mix_gain_rf_reg[MAC_NUM_5G_BAND];
    uint32_t aul_2g_pa_bias_rf_reg[CUS_RF_PA_BIAS_REG_NUM]; /* ���ƻ�RF����PAƫ�üĴ���reg236~244, reg253~284 */
    mac_cfg_custom_filter_narrow_amend_rssi_stru filter_narrow_rssi_amend[WLAN_RF_CHANNEL_NUMS];
} mac_cfg_customize_rf; // only host

typedef struct {
    uint16_t us_tx_ratio;        /* txռ�ձ� */
    uint16_t us_tx_pwr_comp_val; /* ���书�ʲ���ֵ */
} mac_tx_ratio_vs_pwr_stru;

/* ���ƻ�TXռ�ձ�&�¶Ȳ������书�ʵĲ��� */
typedef struct {
    mac_tx_ratio_vs_pwr_stru ast_txratio2pwr[3]; /* 3��ռ�ձȷֱ��Ӧ���ʲ���ֵ */
    uint32_t ul_more_pwr;                      /* �����¶ȶ��ⲹ���ķ��书�� */
} mac_cfg_customize_tx_pwr_comp_stru;

/* RF�Ĵ������ƻ��ṹ�� */
typedef struct {
    uint16_t us_rf_reg117;
    uint16_t us_rf_reg123;
    uint16_t us_rf_reg124;
    uint16_t us_rf_reg125;
    uint16_t us_rf_reg126;
    uint8_t auc_resv[2];
} mac_cus_dts_rf_reg;

/* FCC��֤ �����ṹ�� */
typedef struct {
    uint8_t uc_index;       /* �±��ʾƫ�� */
    uint8_t uc_max_txpower; /* ����͹��� */
    uint16_t us_dbb_scale;  /* dbb scale */
} mac_cus_band_edge_limit_stru;

/* ���ƻ� У׼���ò��� */
typedef struct {
    /* dts */
    int16_t aus_cali_txpwr_pa_dc_ref_2g_val_chan[MAC_2G_CHANNEL_NUM]; /* txpwr���ŵ�refֵ */
    int16_t aus_cali_txpwr_pa_dc_ref_5g_val_band[MAC_NUM_5G_BAND];
} mac_cus_cali_stru;

#ifdef _PRE_WLAN_ONLINE_DPD
/* ���ƻ� DPDУ׼���ò��� */
typedef struct {
    uint32_t aul_dpd_cali_cus_dts[MAC_DPD_CALI_CUS_PARAMS_NUM];
} mac_dpd_cus_cali_stru;
#endif

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
typedef struct {
    int32_t al_dy_cali_base_ratio_params[CUS_DY_CALI_PARAMS_NUM][CUS_DY_CALI_PARAMS_TIMES]; /* ���ⶨ�ƻ��������� */
    int32_t al_dy_cali_base_ratio_ppa_params[CUS_DY_CALI_PARAMS_TIMES]; /* ppa-pow���ƻ��������� */
    int8_t ac_dy_cali_2g_dpn_params[MAC_2G_CHANNEL_NUM][CUS_DY_CALI_DPN_PARAMS_NUM];
    int8_t ac_dy_cali_5g_dpn_params[MAC_NUM_5G_BAND][WLAN_BW_CAP_80PLUS80];
    int16_t as_extre_point_val[CUS_DY_CALI_NUM_5G_BAND];
    uint8_t uc_rf_id;
} mac_cus_dy_cali_param_stru;
#endif

typedef struct {
    mac_cus_cali_stru ast_cali[WLAN_RF_CHANNEL_NUMS];
#ifdef _PRE_WLAN_ONLINE_DPD
    mac_dpd_cus_cali_stru ast_dpd_cali_para[WLAN_RF_CHANNEL_NUMS];
#endif
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    uint16_t aus_dyn_cali_dscr_interval[WLAN_BAND_BUTT]; /* ��̬У׼����2.4g 5g */
    int16_t as_gm0_dB10_amend[WLAN_RF_CHANNEL_NUMS];

    oal_bool_enum_uint8 en_dyn_cali_opt_switch;
#endif  // #ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    int8_t uc_band_5g_enable;
    uint8_t uc_tone_amp_grade;
    uint8_t auc_resv[1];
} mac_cus_dts_cali_stru;

/* perf */
typedef struct {
    signed char ac_used_mem_param[16];
    unsigned char uc_sdio_assem_d2h;
    unsigned char auc_resv[3];
} config_device_perf_h2d_stru;


extern uint8_t g_uc_uapsd_cap;

/* 1102 wiphy Vendor CMD���� ��Ӧcfgid: WLAN_CFGID_VENDOR_CMD */
typedef struct mac_vendor_cmd_channel_list_info {
    uint8_t uc_channel_num_2g;
    uint8_t uc_channel_num_5g;
    uint8_t auc_channel_list_2g[MAC_CHANNEL_FREQ_2_BUTT];
    uint8_t auc_channel_list_5g[MAC_CHANNEL_FREQ_5_BUTT];
} mac_vendor_cmd_channel_list_stru; // only host

typedef struct {
    const int8_t *pstr;
    wlan_channel_bandwidth_enum_uint8 en_width;
} width_type_info_stru;

typedef struct {
    int8_t *pc_cmd_name;          /* �����ַ��� */
    uint8_t uc_is_check_para;     /* �Ƿ����ȡ�Ĳ��� */
    uint32_t ul_bit;              /* ��Ҫ��λ������ */
} wal_ftm_cmd_entry_stru; // only host

typedef struct {
    uint8_t           uc_is_gas_request_sent;
    uint8_t           uc_gas_dialog_token;
    uint8_t           uc_gas_response_dialog_token;
    uint8_t           auc_resv[1];
} mac_gas_mgmt_stru; // only host

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
typedef struct {
    oal_bool_enum_uint8 en_mac_mib_dot11RSNAMFPC;
    oal_bool_enum_uint8 en_mac_mib_dot11RSNAMFPR;
} mac_vap_pmf_mab_stru;
#endif


OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_vap_str_to_width(int8_t *p_width_str)
{
    uint32_t i;
    width_type_info_stru st_width_tbl[] = {
        { "20",   WLAN_BAND_WIDTH_20M },
        { "40+",  WLAN_BAND_WIDTH_40PLUS },
        { "40-",  WLAN_BAND_WIDTH_40MINUS },
        { "80++", WLAN_BAND_WIDTH_80PLUSPLUS },
        { "80--", WLAN_BAND_WIDTH_80MINUSMINUS },
        { "80+-", WLAN_BAND_WIDTH_80PLUSMINUS },
        { "80-+", WLAN_BAND_WIDTH_80MINUSPLUS },
    };

    for (i = 0; i < sizeof(st_width_tbl) / sizeof(width_type_info_stru); i++) {
        if (0 == oal_strcmp(p_width_str, st_width_tbl[i].pstr)) {
            return st_width_tbl[i].en_width;
        }
    }

    return WLAN_BAND_WIDTH_BUTT;
}

OAL_STATIC OAL_INLINE const int8_t *mac_vap_width_to_str(wlan_channel_bandwidth_enum_uint8 en_width)
{
    uint32_t i;
    width_type_info_stru st_width_tbl[] = {
        { "20",   WLAN_BAND_WIDTH_20M },
        { "40+",  WLAN_BAND_WIDTH_40PLUS },
        { "40-",  WLAN_BAND_WIDTH_40MINUS },
        { "80++", WLAN_BAND_WIDTH_80PLUSPLUS },
        { "80--", WLAN_BAND_WIDTH_80MINUSMINUS },
        { "80+-", WLAN_BAND_WIDTH_80PLUSMINUS },
        { "80-+", WLAN_BAND_WIDTH_80MINUSPLUS },
    };

    for (i = 0; i < sizeof(st_width_tbl) / sizeof(width_type_info_stru); i++) {
        if (en_width == st_width_tbl[i].en_width) {
            return st_width_tbl[i].pstr;
        }
    }

    return "unknow";
}

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
typedef struct {
    /* ���ƻ��Ƿ��amsdu����ۺ� */
    uint8_t uc_host_large_amsdu_en;
    /* ��ǰ�ۺ��Ƿ�Ϊamsdu�ۺ� */
    wlan_tx_amsdu_enum_uint8 en_tx_amsdu_level[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    uint16_t us_amsdu_throughput_high;
    uint16_t us_amsdu_throughput_low;
    uint16_t us_amsdu_throughput_middle;
} mac_tx_large_amsdu_ampdu_stru; // only host
extern mac_tx_large_amsdu_ampdu_stru g_st_tx_large_amsdu;
#endif
#ifdef _PRE_WLAN_TCP_OPT
typedef struct {
    /* ���ƻ��Ƿ��tcp ack���� */
    uint8_t uc_tcp_ack_filter_en;
    /* ��ǰ״̬ */
    uint8_t uc_cur_filter_status;
    uint16_t us_rx_filter_throughput_high;
    uint16_t us_rx_filter_throughput_low;
    uint16_t us_resv;
} mac_tcp_ack_filter_stru; // only host
extern mac_tcp_ack_filter_stru g_st_tcp_ack_filter;
#endif

typedef struct {
    /* �Ƿ�ʹ���ֶ����ý��վۺϸ��� */
    oal_bool_enum_uint8 en_rx_ampdu_bitmap_cmd;
    /* �Ƿ��ƻ�ʹ�����ý��վۺϸ��� */
    oal_bool_enum_uint8 en_rx_ampdu_bitmap_ini;
    /* addba rsp�ظ��ľۺ�BAW SIZE */
    uint16_t us_rx_buffer_size;
} mac_rx_buffer_size_stru; // only host
extern mac_rx_buffer_size_stru g_st_rx_buffer_size_stru;

typedef struct {
    /* ���ƻ�С��amsdu���� */
    uint8_t uc_ini_small_amsdu_en;
    uint8_t uc_cur_small_amsdu_en;
    uint16_t us_small_amsdu_throughput_high;
    uint16_t us_small_amsdu_throughput_low;
    uint16_t us_small_amsdu_pps_high;
    uint16_t us_small_amsdu_pps_low;
    uint16_t us_resv;
} mac_small_amsdu_switch_stru; // only host
extern mac_small_amsdu_switch_stru g_st_small_amsdu_switch;

typedef struct {
    uint8_t uc_ini_tcp_ack_buf_en;
    uint8_t uc_ini_tcp_ack_buf_userctl_test_en;
    uint8_t uc_cur_tcp_ack_buf_en[2];// 2����2��vap
    uint16_t us_tcp_ack_buf_throughput_high;
    uint16_t us_tcp_ack_buf_throughput_low;
    uint16_t us_tcp_ack_buf_throughput_high_40M;
    uint16_t us_tcp_ack_buf_throughput_low_40M;
    uint16_t us_tcp_ack_buf_throughput_high_80M;
    uint16_t us_tcp_ack_buf_throughput_low_80M;
    uint16_t us_tcp_ack_buf_throughput_high_160M;
    uint16_t us_tcp_ack_buf_throughput_low_160M;
    uint16_t us_tcp_ack_buf_userctl_high;
    uint16_t us_tcp_ack_buf_userctl_low;
    uint16_t us_tcp_ack_smooth_throughput;
} mac_tcp_ack_buf_switch_stru; // only host
extern mac_tcp_ack_buf_switch_stru g_st_tcp_ack_buf_switch;

typedef struct {
    uint8_t uc_ini_en;     /* ���ƻ��������¶�̬bypass extLNA���� */
    uint8_t uc_cur_status; /* ��ǰ�Ƿ�Ϊ�͹��Ĳ���״̬ */
    uint16_t us_throughput_high;
    uint16_t us_throughput_low;
    uint16_t us_resv;
} mac_rx_dyn_bypass_extlna_stru; // only host
extern mac_rx_dyn_bypass_extlna_stru g_st_rx_dyn_bypass_extlna_switch;


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_pre_auth_actived(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAPreauthenticationActivated;
}

OAL_STATIC OAL_INLINE void mac_mib_set_pre_auth_actived(mac_vap_stru *pst_mac_vap,
                                                            oal_bool_enum_uint8 en_pre_auth)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAPreauthenticationActivated = en_pre_auth;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsnacfg_ptksareplaycounters(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigPTKSAReplayCounters;
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsnacfg_ptksareplaycounters(mac_vap_stru *pst_mac_vap,
                                                                       uint8_t uc_ptksa_replay_counters)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigPTKSAReplayCounters = uc_ptksa_replay_counters;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsnacfg_gtksareplaycounters(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGTKSAReplayCounters;
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsnacfg_gtksareplaycounters(mac_vap_stru *pst_mac_vap,
                                                                       uint8_t uc_gtksa_replay_counters)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGTKSAReplayCounters = uc_gtksa_replay_counters;
}

OAL_STATIC OAL_INLINE void mac_mib_init_rsnacfg_suites(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_wpa_group_suite = MAC_WPA_CHIPER_TKIP;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[0] = MAC_WPA_CHIPER_CCMP;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[1] = MAC_WPA_CHIPER_TKIP;

    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[0] = MAC_WPA_AKM_PSK;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[1] = MAC_WPA_AKM_PSK_SHA256;

    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_suite = MAC_RSN_CHIPER_CCMP;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[0] = MAC_RSN_CHIPER_CCMP;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[1] = MAC_RSN_CHIPER_TKIP;

    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[0] = MAC_RSN_AKM_PSK;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[1] = MAC_RSN_AKM_PSK_SHA256;
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_mgmt_suite = MAC_RSN_CIPHER_SUITE_AES_128_CMAC;
}

OAL_STATIC OAL_INLINE void mac_mib_set_wpa_group_suite(mac_vap_stru *pst_mac_vap, uint32_t ul_suite)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_wpa_group_suite = ul_suite;
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsn_group_suite(mac_vap_stru *pst_mac_vap, uint32_t ul_suite)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_suite = ul_suite;
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsn_group_mgmt_suite(mac_vap_stru *pst_mac_vap,
                                                                uint32_t ul_suite)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_mgmt_suite = ul_suite;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_wpa_group_suite(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_wpa_group_suite;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_rsn_group_suite(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_suite;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_rsn_group_mgmt_suite(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.ul_rsn_group_mgmt_suite;
}

OAL_STATIC OAL_INLINE void mac_mib_set_wpa_pair_suites(mac_vap_stru *pst_mac_vap,
                                                           uint32_t *pul_suites)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[0] = pul_suites[0];
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[1] = pul_suites[1];
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsn_pair_suites(mac_vap_stru *pst_mac_vap,
                                                           uint32_t *pul_suites)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[0] = pul_suites[0];
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[1] = pul_suites[1];
}

OAL_STATIC OAL_INLINE void mac_mib_set_wpa_akm_suites(mac_vap_stru *pst_mac_vap,
                                                          uint32_t *pul_suites)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[0] = pul_suites[0];
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[1] = pul_suites[1];
}

OAL_STATIC OAL_INLINE void mac_mib_set_rsn_akm_suites(mac_vap_stru *pst_mac_vap,
                                                          uint32_t *pul_suites)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[0] = pul_suites[0];
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[1] = pul_suites[1];
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_wpa_pair_match_suites(mac_vap_stru *pst_mac_vap,
                                                               uint32_t *pul_suites)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_rsn_pair_match_suites(mac_vap_stru *pst_mac_vap,
                                                               uint32_t *pul_suites)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_wpa_akm_match_suites(mac_vap_stru *pst_mac_vap,
                                                              uint32_t *pul_suites)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_rsn_akm_match_suites(mac_vap_stru *pst_mac_vap,
                                                              uint32_t *pul_suites)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < WLAN_PAIRWISE_CIPHER_SUITES; uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_wpa_pair_suites(mac_vap_stru *pst_mac_vap,
                                                            uint32_t *pul_suites)
{
    uint8_t uc_loop;
    uint8_t uc_num = 0;

    for (uc_loop = 0; uc_loop < WLAN_PAIRWISE_CIPHER_SUITES; uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_loop];
        }
    }
    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsn_pair_suites(mac_vap_stru *pst_mac_vap,
                                                            uint32_t *pul_suites)
{
    uint8_t uc_loop;
    uint8_t uc_num = 0;

    for (uc_loop = 0; uc_loop < WLAN_PAIRWISE_CIPHER_SUITES; uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_loop];
        }
    }
    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_wpa_akm_suites(mac_vap_stru *pst_mac_vap,
                                                           uint32_t *pul_suites)
{
    uint8_t uc_num = 0;
    uint8_t uc_loop;

    for (uc_loop = 0; uc_loop < WLAN_AUTHENTICATION_SUITES; uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_loop];
        }
    }

    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsn_akm_suites(mac_vap_stru *pst_mac_vap,
                                                           uint32_t *pul_suites)
{
    uint8_t uc_num = 0;
    uint8_t uc_loop;

    for (uc_loop = 0; uc_loop < WLAN_AUTHENTICATION_SUITES; uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_loop];
        }
    }

    return uc_num;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_wpa_pair_match_suites_s(mac_vap_stru *pst_mac_vap,
                                                                 uint32_t *pul_suites,
                                                                 uint8_t uc_cipher_len)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < uc_cipher_len / sizeof(uint32_t); uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < uc_cipher_len / sizeof(uint32_t); uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_rsn_pair_match_suites_s(mac_vap_stru *pst_mac_vap,
                                                                 uint32_t *pul_suites,
                                                                 uint8_t uc_cipher_len)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < uc_cipher_len / sizeof(uint32_t); uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < uc_cipher_len / sizeof(uint32_t); uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_wpa_akm_match_suites_s(mac_vap_stru *pst_mac_vap,
                                                                uint32_t *pul_suites,
                                                                uint8_t uc_cipher_len)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < uc_cipher_len / sizeof(uint32_t); uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < uc_cipher_len / sizeof(uint32_t); uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_rsn_akm_match_suites_s(mac_vap_stru *pst_mac_vap,
                                                                uint32_t *pul_suites,
                                                                uint8_t uc_cipher_len)
{
    uint8_t uc_idx_local;
    uint8_t uc_idx_peer;

    for (uc_idx_local = 0; uc_idx_local < uc_cipher_len / sizeof(uint32_t); uc_idx_local++) {
        for (uc_idx_peer = 0; uc_idx_peer < uc_cipher_len / sizeof(uint32_t); uc_idx_peer++) {
            if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_idx_local] ==
                pul_suites[uc_idx_peer]) {
                return pul_suites[uc_idx_peer];
            }
        }
    }
    return 0;
}
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_wpa_pair_suites_s(mac_vap_stru *pst_mac_vap,
                                                              uint32_t *pul_suites,
                                                              uint8_t uc_cipher_len)
{
    uint8_t uc_loop;
    uint8_t uc_num = 0;

    for (uc_loop = 0; uc_loop < uc_cipher_len / sizeof(uint32_t); uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_pair_suites[uc_loop];
        }
    }
    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsn_pair_suites_s(mac_vap_stru *pst_mac_vap,
                                                              uint32_t *pul_suites,
                                                              uint8_t uc_cipher_len)
{
    uint8_t uc_loop;
    uint8_t uc_num = 0;

    for (uc_loop = 0; uc_loop < uc_cipher_len / sizeof(uint32_t); uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_pair_suites[uc_loop];
        }
    }
    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_wpa_akm_suites_s(mac_vap_stru *pst_mac_vap,
                                                             uint32_t *pul_suites,
                                                             uint8_t uc_cipher_len)
{
    uint8_t uc_num = 0;
    uint8_t uc_loop;

    for (uc_loop = 0; uc_loop < uc_cipher_len / sizeof(uint32_t); uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_wpa_akm_suites[uc_loop];
        }
    }

    return uc_num;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_rsn_akm_suites_s(mac_vap_stru *pst_mac_vap,
                                                             uint32_t *pul_suites,
                                                             uint8_t uc_cipher_len)
{
    uint8_t uc_num = 0;
    uint8_t uc_loop;

    for (uc_loop = 0; uc_loop < uc_cipher_len / sizeof(uint32_t); uc_loop++) {
        if (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_loop] != 0) {
            pul_suites[uc_num++] = pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.st_wlan_mib_rsna_cfg.aul_rsn_akm_suites[uc_loop];
        }
    }

    return uc_num;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_maxmpdu_length(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11MaxMPDULength;
}

OAL_STATIC OAL_INLINE void mac_mib_set_maxmpdu_length(mac_vap_stru *pst_mac_vap,
                                                          uint32_t ul_maxmpdu_length)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11MaxMPDULength = ul_maxmpdu_length;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_vht_max_rx_ampdu_factor(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTMaxRxAMPDUFactor;
}

OAL_STATIC OAL_INLINE void mac_mib_set_vht_max_rx_ampdu_factor(mac_vap_stru *pst_mac_vap,
                                                                   uint32_t ul_vht_max_rx_ampdu_factor)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTMaxRxAMPDUFactor = ul_vht_max_rx_ampdu_factor;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_vht_ctrl_field_cap(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTControlFieldSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_vht_ctrl_field_cap(mac_vap_stru *pst_mac_vap,
                                                              oal_bool_enum_uint8 en_vht_ctrl_field_supported)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTControlFieldSupported = en_vht_ctrl_field_supported;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_txopps(mac_vap_stru *pst_vap)
{
    return pst_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTTXOPPowerSaveOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_txopps(mac_vap_stru *pst_vap, oal_bool_enum_uint8 en_vht_txop_ps)
{
    pst_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTTXOPPowerSaveOptionImplemented = en_vht_txop_ps;
}

OAL_STATIC OAL_INLINE uint16_t mac_mib_get_vht_rx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTRxMCSMap;
}

OAL_STATIC OAL_INLINE void *mac_mib_get_ptr_vht_rx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return &(pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTRxMCSMap);
}

OAL_STATIC OAL_INLINE void mac_mib_set_vht_rx_mcs_map(mac_vap_stru *pst_mac_vap,
                                                          uint16_t us_vht_mcs_mpa)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTRxMCSMap = us_vht_mcs_mpa;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_us_rx_highest_rate(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTRxHighestDataRateSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_us_rx_highest_rate(mac_vap_stru *pst_mac_vap,
                                                              uint32_t ul_rx_highest_rate)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTRxHighestDataRateSupported = ul_rx_highest_rate;
}

OAL_STATIC OAL_INLINE uint16_t mac_mib_get_vht_tx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTTxMCSMap;
}

OAL_STATIC OAL_INLINE void *mac_mib_get_ptr_vht_tx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return &(pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTTxMCSMap);
}

OAL_STATIC OAL_INLINE void mac_mib_set_vht_tx_mcs_map(mac_vap_stru *pst_mac_vap,
                                                          uint16_t us_vht_mcs_mpa)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.us_dot11VHTTxMCSMap = us_vht_mcs_mpa;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_us_tx_highest_rate(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTTxHighestDataRateSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_us_tx_highest_rate(mac_vap_stru *pst_mac_vap,
                                                              uint32_t ul_tx_highest_rate)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.ul_dot11VHTTxHighestDataRateSupported = ul_tx_highest_rate;
}

#ifdef _PRE_WLAN_FEATURE_SMPS
OAL_STATIC OAL_INLINE wlan_mib_mimo_power_save_enum_uint8 mac_mib_get_smps(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MIMOPowerSave;
}

OAL_STATIC OAL_INLINE void mac_mib_set_smps(mac_vap_stru *pst_mac_vap,
                                                wlan_mib_mimo_power_save_enum_uint8 en_sm_power_save)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MIMOPowerSave = en_sm_power_save;
}
#endif  // _PRE_WLAN_FEATURE_SMPS

OAL_STATIC OAL_INLINE wlan_mib_max_amsdu_lenth_enum_uint16 mac_mib_get_max_amsdu_length(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MaxAMSDULength;
}

OAL_STATIC OAL_INLINE void mac_mib_set_max_amsdu_length(mac_vap_stru *pst_mac_vap,
                                                            wlan_mib_max_amsdu_lenth_enum_uint16 en_max_amsdu_length)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MaxAMSDULength = en_max_amsdu_length;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_lsig_txop_protection(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11LsigTxopProtectionOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_lsig_txop_protection(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_lsig_txop_protection)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11LsigTxopProtectionOptionImplemented = en_lsig_txop_protection;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_max_ampdu_len_exponent(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11MaxRxAMPDUFactor;
}

OAL_STATIC OAL_INLINE void mac_mib_set_max_ampdu_len_exponent(mac_vap_stru *pst_mac_vap,
                                                                  uint32_t ul_max_ampdu_len_exponent)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11MaxRxAMPDUFactor = ul_max_ampdu_len_exponent;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_min_mpdu_start_spacing(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11MinimumMPDUStartSpacing;
}

OAL_STATIC OAL_INLINE void mac_mib_set_min_mpdu_start_spacing(mac_vap_stru *pst_mac_vap,
                                                                  uint8_t ul_min_mpdu_start_spacing)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11MinimumMPDUStartSpacing = ul_min_mpdu_start_spacing;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_pco_option_implemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11PCOOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_pco_option_implemented(mac_vap_stru *pst_mac_vap,
                                                                  oal_bool_enum_uint8 en_pco_option_implemented)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11PCOOptionImplemented = en_pco_option_implemented;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_transition_time(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11TransitionTime;
}

OAL_STATIC OAL_INLINE void mac_mib_set_transition_time(mac_vap_stru *pst_mac_vap,
                                                           uint32_t ul_transition_time)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.ul_dot11TransitionTime = ul_transition_time;
}

OAL_STATIC OAL_INLINE wlan_mib_mcs_feedback_opt_implt_enum_uint8 mac_mib_get_mcs_fdbk(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MCSFeedbackOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_mcs_fdbk(mac_vap_stru *pst_mac_vap,
                                                    wlan_mib_mcs_feedback_opt_implt_enum_uint8 en_mcs_fdbk)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MCSFeedbackOptionImplemented = en_mcs_fdbk;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_htc_sup(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11HTControlFieldSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_htc_sup(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_htc_sup)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11HTControlFieldSupported = en_htc_sup;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_rd_rsp(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11RDResponderOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_rd_rsp(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_rd_rsp)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11RDResponderOptionImplemented = en_rd_rsp;
}

#if defined(_PRE_WLAN_FEATURE_11R)
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ft_trainsistion(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FastBSSTransitionActivated;
}

OAL_STATIC OAL_INLINE void mac_mib_set_ft_trainsistion(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_ft_trainsistion)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FastBSSTransitionActivated = en_ft_trainsistion;
}

OAL_STATIC OAL_INLINE uint16_t mac_mib_get_ft_mdid(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.us_dot11FTMobilityDomainID;
}

OAL_STATIC OAL_INLINE void mac_mib_set_ft_mdid(mac_vap_stru *pst_mac_vap, uint16_t ul_ft_mdid)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.us_dot11FTMobilityDomainID = ul_ft_mdid;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ft_over_ds(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FTOverDSActivated;
}

OAL_STATIC OAL_INLINE void mac_mib_set_ft_over_ds(mac_vap_stru *pst_mac_vap,
                                                      oal_bool_enum_uint8 en_ft_over_ds)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FTOverDSActivated = en_ft_over_ds;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ft_resource_req(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FTResourceRequestSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_ft_resource_req(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_ft_resource_req)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_fast_bss_trans_cfg.en_dot11FTResourceRequestSupported = en_ft_resource_req;
}
#endif  // _PRE_WLAN_FEATURE_11R


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_privacyinvoked(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11PrivacyInvoked;
}


OAL_STATIC OAL_INLINE void mac_mib_set_privacyinvoked(mac_vap_stru *pst_mac_vap,
                                                          oal_bool_enum_uint8 en_privacyinvoked)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11PrivacyInvoked = en_privacyinvoked;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_rsnaactivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_rsnaactivated(mac_vap_stru *pst_mac_vap,
                                                         oal_bool_enum_uint8 en_rsnaactivated)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated = en_rsnaactivated;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_wep_enabled(mac_vap_stru *pst_mac_vap)
{
    if (pst_mac_vap == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    if ((mac_mib_get_privacyinvoked(pst_mac_vap) == OAL_FALSE) ||
        (mac_mib_get_rsnaactivated(pst_mac_vap) == OAL_TRUE)) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_wep_allowed(mac_vap_stru *pst_mac_vap)
{
    if (mac_mib_get_rsnaactivated(pst_mac_vap) == OAL_TRUE) {
        return OAL_FALSE;
    } else {
        return mac_is_wep_enabled(pst_mac_vap);
    }
}


OAL_STATIC OAL_INLINE void mac_set_wep_default_keyid(mac_vap_stru *pst_mac_vap,
                                                         uint8_t uc_default_key_id)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.uc_dot11WEPDefaultKeyID = uc_default_key_id;
}


OAL_STATIC OAL_INLINE uint8_t mac_get_wep_default_keyid(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.uc_dot11WEPDefaultKeyID);
}


OAL_STATIC OAL_INLINE uint8_t mac_get_wep_default_keysize(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.ast_wlan_mib_wep_dflt_key[mac_get_wep_default_keyid(pst_mac_vap)].auc_dot11WEPDefaultKeyValue[WLAN_WEP_SIZE_OFFSET]);
}


OAL_STATIC OAL_INLINE uint8_t mac_get_wep_keysize(mac_vap_stru *pst_mac_vap, uint8_t uc_idx)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.ast_wlan_mib_wep_dflt_key[uc_idx].auc_dot11WEPDefaultKeyValue[WLAN_WEP_SIZE_OFFSET]);
}


OAL_STATIC OAL_INLINE wlan_ciper_protocol_type_enum_uint8 mac_get_wep_type(mac_vap_stru *pst_mac_vap,
                                                                           uint8_t uc_key_id)
{
    wlan_ciper_protocol_type_enum_uint8 en_cipher_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;

    switch (mac_get_wep_keysize(pst_mac_vap, uc_key_id)) {
        case 40:
            en_cipher_type = WLAN_80211_CIPHER_SUITE_WEP_40;
            break;
        case 104:
            en_cipher_type = WLAN_80211_CIPHER_SUITE_WEP_104;
            break;
        default:
            en_cipher_type = WLAN_80211_CIPHER_SUITE_WEP_40;
            break;
    }
    return en_cipher_type;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_tkip_only(mac_vap_stru *pst_mac_vap)
{
    uint8_t uc_pair_suites_num;
    uint32_t aul_pcip[WLAN_PAIRWISE_CIPHER_SUITES] = { 0 };

    if (pst_mac_vap == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    if ((pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_FALSE) && (pst_mac_vap->st_cap_flag.bit_wpa == OAL_FALSE)) {
        return OAL_FALSE;
    }

    if ((mac_mib_get_privacyinvoked(pst_mac_vap) == OAL_FALSE) ||
        (mac_mib_get_rsnaactivated(pst_mac_vap) == OAL_FALSE)) {  // �����ܻ�����WEP����ʱ������false
        return OAL_FALSE;
    }

    if (pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE) {
        uc_pair_suites_num = mac_mib_get_wpa_pair_suites(pst_mac_vap, &aul_pcip[0]);
        if ((uc_pair_suites_num != 1) || (aul_pcip[0] != MAC_WPA_CHIPER_TKIP)) {  // wpa����ʱ������tkip only������false
            return OAL_FALSE;
        }
    }

    if (pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_TRUE) {
        uc_pair_suites_num = mac_mib_get_rsn_pair_suites(pst_mac_vap, &aul_pcip[0]);
        if ((uc_pair_suites_num != 1) || (aul_pcip[0] != MAC_RSN_CHIPER_TKIP)) {  // wpa2����ʱ������tkip only������false
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}


OAL_STATIC OAL_INLINE uint8_t *mac_mib_get_StationID(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID;
}


OAL_STATIC OAL_INLINE void mac_mib_set_StationID(mac_vap_stru *pst_mac_vap, uint8_t *puc_sta_id)
{
    oal_set_mac_addr(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID, puc_sta_id);
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_OBSSScanPassiveDwell(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanPassiveDwell;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OBSSScanPassiveDwell(mac_vap_stru *pst_mac_vap,
                                                                uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanPassiveDwell = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_OBSSScanActiveDwell(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActiveDwell;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OBSSScanActiveDwell(mac_vap_stru *pst_mac_vap,
                                                               uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActiveDwell = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_BSSWidthTriggerScanInterval(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11BSSWidthTriggerScanInterval;
}


OAL_STATIC OAL_INLINE void mac_mib_set_BSSWidthTriggerScanInterval(mac_vap_stru *pst_mac_vap,
                                                                       uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11BSSWidthTriggerScanInterval = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_OBSSScanPassiveTotalPerChannel(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanPassiveTotalPerChannel;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OBSSScanPassiveTotalPerChannel(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanPassiveTotalPerChannel = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_OBSSScanActiveTotalPerChannel(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActiveTotalPerChannel;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OBSSScanActiveTotalPerChannel(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActiveTotalPerChannel = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_BSSWidthChannelTransitionDelayFactor(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11BSSWidthChannelTransitionDelayFactor;
}


OAL_STATIC OAL_INLINE void mac_mib_set_BSSWidthChannelTransitionDelayFactor(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11BSSWidthChannelTransitionDelayFactor = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_OBSSScanActivityThreshold(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActivityThreshold;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OBSSScanActivityThreshold(mac_vap_stru *pst_mac_vap,
                                                                     uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11OBSSScanActivityThreshold = ul_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HighThroughputOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11HighThroughputOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_HighThroughputOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11HighThroughputOptionImplemented = en_val;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTOptionImplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11VHTOptionImplemented;
}

#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)


OAL_STATIC OAL_INLINE void mac_mib_set_HEOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                               oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11HEOptionImplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HEOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11HEOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_OMIOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11OMIOptionImplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_OMIOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11OMIOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_he_TWTOptionActivated(mac_vap_stru *pst_mac_vap,
                                                                 oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11TWTOptionActivated = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_TWTOptionActivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11TWTOptionActivated;
}

/*
 * �� �� ��  : mac_mib_get_he_OperatingModeIndication
 * ��������  : ��ȡMIB�� dot11OperatingModeIndication ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_OperatingModeIndication(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11OperatingModeIndication;
}

/*
 * �� �� ��  : mac_mib_set_he_OperatingModeIndication
 * ��������  : ����MIB�� en_dot11OperatingModeIndication ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_OperatingModeIndication(mac_vap_stru *pst_mac_vap,
                                                                      oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11OperatingModeIndication = en_val;
}

/*
 * �� �� ��  : mac_mib_get_he_BSRSupport
 * ��������  : ��ȡMIB�� en_dot11BSRSupport ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_BSRSupport(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11BSRSupport;
}

/*
 * �� �� ��  : mac_mib_set_he_BSRSupport
 * ��������  : ����MIB�� en_dot11BSRSupport ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_BSRSupport(mac_vap_stru *pst_mac_vap,
                                                         oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11BSRSupport = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_BQRSupport(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11BQRSupport;
}


OAL_STATIC OAL_INLINE void mac_mib_set_he_BQRSupport(mac_vap_stru *pst_mac_vap,
                                                         oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11BQRSupport = en_val;
}


OAL_STATIC OAL_INLINE void mac_mib_set_he_OFDMARandomAccess(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11TOFDMARandomAccess = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_OFDMARandomAccess(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11TOFDMARandomAccess;
}

/*
 * �� �� ��  : mac_mib_set_he_TriggerMacPaddingDuration
 * ��������  : ����MIB�� dot11TriggerMacPaddingDuration ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_TriggerMacPaddingDuration(mac_vap_stru *pst_mac_vap,
                                                                        uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.uc_dot11TriggerMacPaddingDuration = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_TriggerMacPaddingDuration
 * ��������  : ��ȡMIB�� dot11TriggerMacPaddingDuration ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_TriggerMacPaddingDuration(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.uc_dot11TriggerMacPaddingDuration;
}

/*
 * �� �� ��  : mac_mib_set_he_MultiBSSIDImplemented
 * ��������  : ����MIB�� dot11MultiBSSIDImplemented ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_MultiBSSIDImplemented(mac_vap_stru *pst_mac_vap,
                                                                    uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11MultiBSSIDImplemented = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_max_ampdu_length
 * ��������  : ��ȡMIB�� dot11MultiBSSIDImplemented ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_MultiBSSIDImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.en_dot11MultiBSSIDImplemented;
}

/*
 * �� �� ��  : mac_mib_set_he_MaxAMPDULength
 * ��������  : ����MIB�� uc_dot11MaxAMPDULength ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_MaxAMPDULengthExponent(mac_vap_stru *pst_mac_vap,
                                                                     uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.uc_dot11MaxAMPDULengthExponent = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_MaxAMPDULength
 * ��������  : ��ȡMIB�� dot11MaxAMPDULength ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_MaxAMPDULengthExponent(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.uc_dot11MaxAMPDULengthExponent;
}

OAL_STATIC OAL_INLINE void mac_mib_set_he_rx_mcs_map(mac_vap_stru *pst_mac_vap,
                                                         uint32_t ul_he_mcs_map)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HERxMCSMap = ul_he_mcs_map;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_he_rx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HERxMCSMap;
}

OAL_STATIC OAL_INLINE void *mac_mib_get_ptr_he_rx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return &(pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HERxMCSMap);
}
OAL_STATIC OAL_INLINE void mac_mib_set_he_tx_mcs_map(mac_vap_stru *pst_mac_vap,
                                                         uint32_t ul_he_mcs_map)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HETxMCSMap = ul_he_mcs_map;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_he_tx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HETxMCSMap;
}

OAL_STATIC OAL_INLINE void *mac_mib_get_ptr_he_tx_mcs_map(mac_vap_stru *pst_mac_vap)
{
    return &(pst_mac_vap->pst_mib_info->st_wlan_mib_he_sta_config.ul_dot11HETxMCSMap);
}

/*
 * �� �� ��  : mac_mib_get_he_DualBandSupport
 * ��������  : ����MIB�� en_dot11HEDualBandSupport ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_DualBandSupport(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HEDualBandSupport;
}

/*
 * �� �� ��  : mac_mib_set_he_DualBandSupport
 * ��������  : ����MIB�� en_dot11HEDualBandSupport ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_DualBandSupport(mac_vap_stru *pst_mac_vap,
                                                              uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HEDualBandSupport = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_LDPCCodingInPayload
 * ��������  : ����MIB�� en_dot11HELDPCCodingInPayload ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_LDPCCodingInPayload(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HELDPCCodingInPayload;
}

/*
 * �� �� ��  : mac_mib_set_he_LDPCCodingInPayload
 * ��������  : ����MIB�� en_dot11HELDPCCodingInPayload ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_LDPCCodingInPayload(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HELDPCCodingInPayload = en_val;
}

/*
 * �� �� ��  : mac_mib_get_he_LDPCCodingInPayload
 * ��������  : ����MIB�� en_dot11HELDPCCodingInPayload ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_STBCTxBelow80M(mac_vap_stru *mac_vap)
{
    return mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HESTBCTxBelow80MHz;
}

/*
 * �� �� ��  : mac_mib_set_he_LDPCCodingInPayload
 * ��������  : ����MIB�� en_dot11HELDPCCodingInPayload ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_STBCTxBelow80M(mac_vap_stru *mac_vap, oal_bool_enum_uint8 en_val)
{
    mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11HESTBCTxBelow80MHz = en_val;
}

/*
 * �� �� ��  : mac_mib_set_he_SUBeamformer
 * ��������  : ����MIB�� en_dot11SUBeamformer ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_SUBeamformer(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11SUBeamformer = en_val;
}

/*
 * �� �� ��  : mac_mib_set_he_SUBeamformer
 * ��������  : ����MIB�� en_dot11SUBeamformer ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_SUBeamformer(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11SUBeamformer;
}

/*
 * �� �� ��  : mac_mib_set_he_SUBeamformee
 * ��������  : ����MIB�� en_dot11SUBeamformee ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_SUBeamformee(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11SUBeamformee = en_val;
}

/*
 * �� �� ��  : mac_mib_set_he_SUBeamformee
 * ��������  : ����MIB�� en_dot11SUBeamformee ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_SUBeamformee(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11SUBeamformee;
}
/*
 * �� �� ��  : mac_mib_set_he_MUBeamformer
 * ��������  : ����MIB�� en_dot11MUBeamformer ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_MUBeamformer(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11MUBeamformer = en_val;
}

/*
 * �� �� ��  : mac_mib_set_he_MUBeamformer
 * ��������  : ����MIB�� en_dot11MUBeamformer ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_MUBeamformer(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.en_dot11MUBeamformer;
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC OAL_INLINE void mac_mib_set_he_BeamformeeSTSBelow80Mhz(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HEBeamformeeSTSBelow80Mhz = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_BeamformeeSTSBelow80Mhz(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HEBeamformeeSTSBelow80Mhz;
}

OAL_STATIC OAL_INLINE void mac_mib_set_he_BeamformeeSTSOver80Mhz(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HEBeamformeeSTSOver80Mhz = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_BeamformeeSTSOver80Mhz(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HEBeamformeeSTSOver80Mhz;
}

OAL_STATIC OAL_INLINE void mac_mib_set_HENumberSoundingDimensionsBelow80Mhz(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HENumberSoundingDimensionsBelow80Mhz = en_val;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HENumberSoundingDimensionsBelow80Mhz(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HENumberSoundingDimensionsBelow80Mhz;
}

OAL_STATIC OAL_INLINE void mac_mib_set_HENumberSoundingDimensionsOver80Mhz(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HENumberSoundingDimensionsOver80Mhz = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HENumberSoundingDimensionsOver80Mhz(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.uc_dot11HENumberSoundingDimensionsOver80Mhz;
}


OAL_STATIC OAL_INLINE void mac_mib_set_HENg16SUFeedback(mac_vap_stru *pst_mac_vap,
                                                            oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Ng16SUFeedback = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HENg16SUFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Ng16SUFeedback;
}

OAL_STATIC OAL_INLINE void mac_mib_set_HENg16MUFeedback(mac_vap_stru *pst_mac_vap,
                                                            oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Ng16MUFeedback = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HENg16MUFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Ng16MUFeedback;
}


OAL_STATIC OAL_INLINE void mac_mib_set_HECodebook42SUFeedback(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Codebook42SUFeedback = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HECodebook42SUFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Codebook42SUFeedback;
}

OAL_STATIC OAL_INLINE void mac_mib_set_HECodebook75MUFeedback(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Codebook75MUFeedback = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HECodebook75MUFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11Codebook75MUFeedback;
}

OAL_STATIC OAL_INLINE void mac_mib_set_he_TriggeredSUBeamformingFeedback(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11TriggeredSUBeamformingFeedback = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_TriggeredSUBeamformingFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11TriggeredSUBeamformingFeedback;
}


OAL_STATIC OAL_INLINE void mac_mib_set_he_TriggeredCQIFeedback(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11TriggeredCQIFeedback = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_TriggeredCQIFeedback(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11TriggeredCQIFeedback;
}

OAL_STATIC OAL_INLINE oal_void mac_mib_set_HESUPPDUwith1xHELTFand0point8GIlmplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11HESUPPDUwith1xHELTFand0point8GIlmplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_HESUPPDUwith1xHELTFand0point8GIlmplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11HESUPPDUwith1xHELTFand0point8GIlmplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_he_MultiBSSIDActived(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->st_wlan_rom_mib_he_sta_config.en_dot11MultiBSSIDActived = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_he_MultiBSSIDActived(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_sta_config.en_dot11MultiBSSIDActived;
}

#endif

/*
 * �� �� ��  : mac_mib_set_HEChannelWidthSet
 * ��������  : ����MIB�� dot11HEChannelWidthSet ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_HEChannelWidthSet(mac_vap_stru *pst_mac_vap,
                                                                uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HEChannelWidthSet = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_ChannelWidthSet
 * ��������  : ��ȡMIB�� dot11HEChannelWidthSet ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_HEChannelWidthSet(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HEChannelWidthSet;
}

/*
 * �� �� ��  : mac_mib_set_he_HighestNSS
 * ��������  : ����MIB�� dot11HighestNSS ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_HighestNSS(mac_vap_stru *pst_mac_vap, uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HighestNSS = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_HighestNSS
 * ��������  : ��ȡMIB�� dot11HighestNSS ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_HighestNSS(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HighestNSS;
}

/*
 * �� �� ��  : mac_mib_set_he_HighestMCS
 * ��������  : ����MIB�� dot11HighestNSS ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE void mac_mib_set_he_HighestMCS(mac_vap_stru *pst_mac_vap, uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HighestMCS = uc_val;
}

/*
 * �� �� ��  : mac_mib_get_he_HighestMCS
 * ��������  : ��ȡMIB�� dot11HighestNSS ��ֵ
 * �޸���ʷ  :
 * 1.��    ��  : 2017��5��26��
 *   �޸�����  : �����ɺ���
 */
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_he_HighestMCS(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_he_phy_config.uc_dot11HighestMCS;
}

#endif


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_FortyMHzOperationImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) ? pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GFortyMHzOperationImplemented : pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GFortyMHzOperationImplemented;
}



OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_2GFortyMHzOperationImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GFortyMHzOperationImplemented);
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_5GFortyMHzOperationImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GFortyMHzOperationImplemented);
}


OAL_STATIC OAL_INLINE void mac_mib_set_FortyMHzOperationImplemented(mac_vap_stru *pst_mac_vap,
                                                                        oal_bool_enum_uint8 en_val)
{
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GFortyMHzOperationImplemented = en_val;
    } else {
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GFortyMHzOperationImplemented = en_val;
    }
}


OAL_STATIC OAL_INLINE void mac_mib_set_2GFortyMHzOperationImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GFortyMHzOperationImplemented = en_val;
}

OAL_STATIC OAL_INLINE void mac_mib_set_5GFortyMHzOperationImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GFortyMHzOperationImplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_SpectrumManagementImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11SpectrumManagementImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_SpectrumManagementImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11SpectrumManagementImplemented = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_FortyMHzIntolerant(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11FortyMHzIntolerant;
}


OAL_STATIC OAL_INLINE void mac_mib_set_FortyMHzIntolerant(mac_vap_stru *pst_mac_vap,
                                                              oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11FortyMHzIntolerant = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_2040BSSCoexistenceManagementSupport(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot112040BSSCoexistenceManagementSupport;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RSNAMFPC(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPC;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RSNAMFPR(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPR;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11RSNAMFPC(mac_vap_stru *pst_mac_vap,
                                                         oal_bool_enum_uint8 ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPC = ul_val;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11RSNAMFPR(mac_vap_stru *pst_mac_vap,
                                                         oal_bool_enum_uint8 ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPR = ul_val;
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_dot11AssociationSAQueryMaximumTimeout(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationSAQueryMaximumTimeout;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_dot11AssociationSAQueryRetryTimeout(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationSAQueryRetryTimeout;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11AssociationSAQueryMaximumTimeout(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationSAQueryMaximumTimeout = ul_val;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11AssociationSAQueryRetryTimeout(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationSAQueryRetryTimeout = ul_val;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11ExtendedChannelSwitchActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11ExtendedChannelSwitchActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11ExtendedChannelSwitchActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11ExtendedChannelSwitchActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11RadioMeasurementActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RadioMeasurementActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RadioMeasurementActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RadioMeasurementActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11QBSSLoadImplemented(mac_vap_stru *pst_mac_vap,
                                                                    oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QBSSLoadImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11QBSSLoadImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QBSSLoadImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11VHTExtendedNSSBWCapable(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    ((mac_vap_stru *)pst_mac_vap)->en_dot11VHTExtendedNSSBWCapable = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11VHTExtendedNSSBWCapable(
    mac_vap_stru *pst_mac_vap)
{
    return ((mac_vap_stru *)pst_mac_vap)->en_dot11VHTExtendedNSSBWCapable;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11APSDOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11APSDOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11APSDOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11APSDOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11DelayedBlockAckOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11DelayedBlockAckOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11DelayedBlockAckOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11DelayedBlockAckOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11ImmediateBlockAckOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11ImmediateBlockAckOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11ImmediateBlockAckOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11ImmediateBlockAckOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11QosOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                     oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QosOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11QosOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QosOptionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11MultiDomainCapabilityActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11MultiDomainCapabilityActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11MultiDomainCapabilityActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11MultiDomainCapabilityActivated;
}

OAL_STATIC OAL_INLINE void mac_mib_set_dot11AssociationResponseTimeOut(
    mac_vap_stru *pst_mac_vap, uint32_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationResponseTimeOut = en_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_dot11AssociationResponseTimeOut(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationResponseTimeOut;
}

OAL_STATIC OAL_INLINE void mac_mib_set_MaxAssocUserNums(mac_vap_stru *pst_mac_vap,
                                                            uint16_t us_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.us_dot11MaxAssocUserNums = us_val;
}

OAL_STATIC OAL_INLINE uint16_t mac_mib_get_MaxAssocUserNums(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.us_dot11MaxAssocUserNums;
}

OAL_STATIC OAL_INLINE void mac_mib_set_SupportRateSetNums(mac_vap_stru *pst_mac_vap,
                                                              uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11SupportRateSetNums = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_SupportRateSetNums(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11SupportRateSetNums;
}

OAL_STATIC OAL_INLINE void mac_mib_set_CfgAmsduTxAtive(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11CfgAmsduTxAtive = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_CfgAmsduTxAtive(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11CfgAmsduTxAtive;
}

OAL_STATIC OAL_INLINE void mac_mib_set_AmsduAggregateAtive(mac_vap_stru *pst_mac_vap,
                                                               uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AmsduAggregateAtive = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_AmsduAggregateAtive(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AmsduAggregateAtive;
}

OAL_STATIC OAL_INLINE void mac_mib_set_AmsduPlusAmpduActive(mac_vap_stru *pst_mac_vap,
                                                                uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AmsduPlusAmpduActive = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_AmsduPlusAmpduActive(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AmsduPlusAmpduActive;
}

OAL_STATIC OAL_INLINE void mac_mib_set_WPSActive(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11WPSActive = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_WPSActive(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11WPSActive;
}

OAL_STATIC OAL_INLINE void mac_mib_set_2040SwitchProhibited(mac_vap_stru *pst_mac_vap,
                                                                uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot112040SwitchProhibited = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_2040SwitchProhibited(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot112040SwitchProhibited;
}

OAL_STATIC OAL_INLINE void mac_mib_set_TxAggregateActived(mac_vap_stru *pst_mac_vap,
                                                              uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11TxAggregateActived = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxAggregateActived(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11TxAggregateActived;
}

OAL_STATIC OAL_INLINE void mac_mib_set_CfgAmpduTxAtive(mac_vap_stru *pst_mac_vap,
                                                           uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11CfgAmpduTxAtive = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_CfgAmpduTxAtive(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11CfgAmpduTxAtive;
}

OAL_STATIC OAL_INLINE void mac_mib_set_RxBASessionNumber(mac_vap_stru *pst_mac_vap,
                                                             uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11RxBASessionNumber = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_RxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11RxBASessionNumber;
}

OAL_STATIC OAL_INLINE void mac_mib_incr_RxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11RxBASessionNumber++;
}

OAL_STATIC OAL_INLINE void mac_mib_decr_RxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11RxBASessionNumber--;
}

OAL_STATIC OAL_INLINE void mac_mib_set_TxBASessionNumber(mac_vap_stru *pst_mac_vap,
                                                             uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11TxBASessionNumber = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11TxBASessionNumber;
}

OAL_STATIC OAL_INLINE void mac_mib_incr_TxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11TxBASessionNumber++;
}

OAL_STATIC OAL_INLINE void mac_mib_decr_TxBASessionNumber(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11TxBASessionNumber--;
}

OAL_STATIC OAL_INLINE void mac_mib_set_VAPClassifyTidNo(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11VAPClassifyTidNo = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VAPClassifyTidNo(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11VAPClassifyTidNo;
}

OAL_STATIC OAL_INLINE void mac_mib_set_AuthenticationMode(mac_vap_stru *pst_mac_vap,
                                                              uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AuthenticationMode = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_AuthenticationMode(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AuthenticationMode;
}

OAL_STATIC OAL_INLINE void mac_mib_set_AddBaMode(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AddBaMode = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_AddBaMode(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11AddBaMode;
}

OAL_STATIC OAL_INLINE void mac_mib_set_80211iMode(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot1180211iMode = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_80211iMode(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot1180211iMode;
}

OAL_STATIC OAL_INLINE void mac_mib_set_TxTrafficClassifyFlag(mac_vap_stru *pst_mac_vap,
                                                                 uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11TxTrafficClassifyFlag = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxTrafficClassifyFlag(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11TxTrafficClassifyFlag;
}

OAL_STATIC OAL_INLINE void mac_mib_set_StaAuthCount(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAuthCount = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_StaAuthCount(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAuthCount;
}

OAL_STATIC OAL_INLINE void mac_mib_set_StaAssocCount(mac_vap_stru *pst_mac_vap, uint8_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAssocCount = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_StaAssocCount(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAssocCount;
}

OAL_STATIC OAL_INLINE void mac_mib_incr_StaAuthCount(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAuthCount++;
}

OAL_STATIC OAL_INLINE void mac_mib_incr_StaAssocCount(mac_vap_stru *pst_mac_vap)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11StaAssocCount++;
}

#if defined(_PRE_WLAN_FEATURE_11K)

OAL_STATIC OAL_INLINE void mac_mib_set_dot11RMBeaconTableMeasurementActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconTableMeasurementActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RMBeaconTableMeasurementActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconTableMeasurementActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11RMBeaconActiveMeasurementActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconActiveMeasurementActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RMBeaconActiveMeasurementActivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconActiveMeasurementActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11RMBeaconPassiveMeasurementActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconPassiveMeasurementActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RMBeaconPassiveMeasurementActivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMBeaconPassiveMeasurementActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11RMLinkMeasurementActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMLinkMeasurementActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11RMLinkMeasurementActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11RMLinkMeasurementActivated;
}

#endif

OAL_STATIC OAL_INLINE void mac_mib_set_2040BSSCoexistenceManagementSupport(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot112040BSSCoexistenceManagementSupport = en_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_dot11dtimperiod(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11DTIMPeriod;
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11VapMaxBandWidth(mac_vap_stru *pst_mac_vap,
                                                                wlan_channel_bandwidth_enum_uint8 en_bandwidth)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11VapMaxBandWidth = en_bandwidth;
}


OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_mib_get_dot11VapMaxBandWidth(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11VapMaxBandWidth;
}

OAL_STATIC OAL_INLINE void mac_mib_set_BeaconPeriod(mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    if (ul_val != 0) {
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11BeaconPeriod = ul_val;
    }
}
OAL_STATIC OAL_INLINE uint32_t mac_mib_get_BeaconPeriod(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11BeaconPeriod;
}

OAL_STATIC OAL_INLINE void mac_mib_set_DesiredBSSType(mac_vap_stru *pst_mac_vap, uint8_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11DesiredBSSType = ul_val;
}
OAL_STATIC OAL_INLINE uint8_t mac_mib_get_DesiredBSSType(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11DesiredBSSType;
}

OAL_STATIC OAL_INLINE uint8_t *mac_mib_get_DesiredSSID(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11DesiredSSID;
}

OAL_STATIC OAL_INLINE void mac_mib_set_AuthenticationResponseTimeOut(
    mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AuthenticationResponseTimeOut = ul_val;
}
OAL_STATIC OAL_INLINE uint32_t mac_mib_get_AuthenticationResponseTimeOut(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AuthenticationResponseTimeOut;
}

OAL_STATIC OAL_INLINE uint8_t *mac_mib_get_p2p0_dot11StationID(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID;
}

#if (defined(_PRE_WLAN_FEATURE_FTM)) || defined(_PRE_WLAN_FEATURE_11V_ENABLE)

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_MgmtOptionBSSTransitionActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11MgmtOptionBSSTransitionActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_MgmtOptionBSSTransitionActivated(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11MgmtOptionBSSTransitionActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_MgmtOptionBSSTransitionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11MgmtOptionBSSTransitionImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_MgmtOptionBSSTransitionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11MgmtOptionBSSTransitionImplemented = en_val;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_WirelessManagementImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11WirelessManagementImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_WirelessManagementImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11WirelessManagementImplemented = en_val;
}
#endif  // _PRE_WLAN_FEATURE_11V_ENABLE

#if (defined(_PRE_WLAN_FEATURE_FTM)) || defined(_PRE_WLAN_FEATURE_11V_ENABLE)
OAL_STATIC OAL_INLINE void mac_mib_set_FtmInitiatorModeActivated(mac_vap_stru *pst_mac_vap,
                                                                     oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11FtmInitiatorModeActivated = en_val;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_FtmInitiatorModeActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11FtmInitiatorModeActivated;
}
OAL_STATIC OAL_INLINE void mac_mib_set_FtmResponderModeActivated(mac_vap_stru *pst_mac_vap,
    oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11FtmResponderModeActivated = en_val;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_FtmResponderModeActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11FtmResponderModeActivated;
}
OAL_STATIC OAL_INLINE void mac_mib_set_FtmRangeReportActivated(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11RMFtmRangeReportActivated = en_val;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_FtmRangeReportActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_wireless_mgmt_op.en_dot11RMFtmRangeReportActivated;
}
#endif


OAL_STATIC OAL_INLINE void mac_mib_init_2040(mac_vap_stru *pst_mac_vap)
{
    mac_mib_set_FortyMHzIntolerant(pst_mac_vap, OAL_FALSE);
    mac_mib_set_SpectrumManagementImplemented(pst_mac_vap, OAL_TRUE);
    mac_mib_set_2040BSSCoexistenceManagementSupport(pst_mac_vap, OAL_TRUE);
}


OAL_STATIC OAL_INLINE void mac_mib_init_obss_scan(mac_vap_stru *pst_mac_vap)
{
    mac_mib_set_OBSSScanPassiveDwell(pst_mac_vap, 20);
    mac_mib_set_OBSSScanActiveDwell(pst_mac_vap, 10);
    mac_mib_set_BSSWidthTriggerScanInterval(pst_mac_vap, 300);
    mac_mib_set_OBSSScanPassiveTotalPerChannel(pst_mac_vap, 200);
    mac_mib_set_OBSSScanActiveTotalPerChannel(pst_mac_vap, 20);
    mac_mib_set_BSSWidthChannelTransitionDelayFactor(pst_mac_vap, 5);
    mac_mib_set_OBSSScanActivityThreshold(pst_mac_vap, 25);
}


OAL_STATIC OAL_INLINE void mac_mib_set_dot11dtimperiod(mac_vap_stru *pst_mac_vap, uint32_t ul_val)
{
    if (ul_val != 0) {
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11DTIMPeriod = ul_val;
    }
}


OAL_STATIC OAL_INLINE uint32_t mac_mib_get_powermanagementmode(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11PowerManagementMode;
}


OAL_STATIC OAL_INLINE void mac_mib_set_powermanagementmode(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_val)
{
    if (uc_val != 0) {
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.uc_dot11PowerManagementMode = uc_val;
    }
}

#if defined _PRE_WLAN_FEATURE_OPMODE_NOTIFY || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION)

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_OperatingModeNotificationImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11OperatingModeNotificationImplemented;
}

OAL_STATIC OAL_INLINE void mac_mib_set_OperatingModeNotificationImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_vht_sta_config.en_dot11OperatingModeNotificationImplemented = en_val;
}
#endif


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_LsigTxopFullProtectionActivated(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11LSIGTXOPFullProtectionActivated);
}


OAL_STATIC OAL_INLINE void mac_mib_set_LsigTxopFullProtectionActivated(mac_vap_stru *pst_mac_vap,
    oal_bool_enum_uint8 en_lsig_txop_full_protection_activated)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11LSIGTXOPFullProtectionActivated = en_lsig_txop_full_protection_activated;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_NonGFEntitiesPresent(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11NonGFEntitiesPresent);
}


OAL_STATIC OAL_INLINE void mac_mib_set_NonGFEntitiesPresent(mac_vap_stru *pst_mac_vap,
                                                                oal_bool_enum_uint8 en_non_gf_entities_present)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11NonGFEntitiesPresent = en_non_gf_entities_present;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_RifsMode(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11RIFSMode);
}


OAL_STATIC OAL_INLINE void mac_mib_set_RifsMode(mac_vap_stru *pst_mac_vap,
                                                    oal_bool_enum_uint8 en_rifs_mode)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11RIFSMode = en_rifs_mode;
}


OAL_STATIC OAL_INLINE wlan_mib_ht_protection_enum_uint8 mac_mib_get_HtProtection(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11HTProtection);
}

OAL_STATIC OAL_INLINE void mac_mib_set_DualCTSProtection(mac_vap_stru *pst_mac_vap,
                                                             oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11DualCTSProtection = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_DualCTSProtection(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11DualCTSProtection);
}

OAL_STATIC OAL_INLINE void mac_mib_set_PCOActivated(mac_vap_stru *pst_mac_vap,
                                                        oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11PCOActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_PCOActivated(mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11PCOActivated);
}


OAL_STATIC OAL_INLINE void mac_mib_set_HtProtection(mac_vap_stru *pst_mac_vap,
                                                        wlan_mib_ht_protection_enum_uint8 en_ht_protection)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.en_dot11HTProtection = en_ht_protection;
}


OAL_STATIC OAL_INLINE void mac_mib_set_SpectrumManagementRequired(mac_vap_stru *pst_mac_vap,
                                                                      oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11SpectrumManagementRequired = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_dot11SpectrumManagementRequired(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11SpectrumManagementRequired;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ShortGIOptionInFortyImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) ?
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GShortGIOptionInFortyImplemented :
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GShortGIOptionInFortyImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ShortGIOptionInFortyImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GShortGIOptionInFortyImplemented = en_val;
    } else {
        pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GShortGIOptionInFortyImplemented = en_val;
    }
}

OAL_STATIC OAL_INLINE void mac_mib_set_2GShortGIOptionInFortyImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot112GShortGIOptionInFortyImplemented = en_val;
}

OAL_STATIC OAL_INLINE void mac_mib_set_5GShortGIOptionInFortyImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot115GShortGIOptionInFortyImplemented = en_val;
}


OAL_STATIC OAL_INLINE void mac_mib_set_FragmentationThreshold(mac_vap_stru *pst_mac_vap,
                                                                  uint32_t ul_frag_threshold)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11FragmentationThreshold = ul_frag_threshold;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_FragmentationThreshold(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11FragmentationThreshold;
}


OAL_STATIC OAL_INLINE void mac_mib_set_RTSThreshold(mac_vap_stru *pst_mac_vap,
                                                        uint32_t ul_rts_threshold)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11RTSThreshold = ul_rts_threshold;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_RTSThreshold(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_operation.ul_dot11RTSThreshold;
}


OAL_STATIC OAL_INLINE void mac_mib_set_AntennaSelectionOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11AntennaSelectionOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_AntennaSelectionOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11AntennaSelectionOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TransmitExplicitCSIFeedbackASOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitExplicitCSIFeedbackASOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TransmitExplicitCSIFeedbackASOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitExplicitCSIFeedbackASOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TransmitIndicesFeedbackASOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitIndicesFeedbackASOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TransmitIndicesFeedbackASOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitIndicesFeedbackASOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitCSIFeedbackASOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11ExplicitCSIFeedbackASOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitCSIFeedbackASOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11ExplicitCSIFeedbackASOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ReceiveAntennaSelectionOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11ReceiveAntennaSelectionOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ReceiveAntennaSelectionOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11ReceiveAntennaSelectionOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TransmitSoundingPPDUOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitSoundingPPDUOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TransmitSoundingPPDUOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_antenna.en_dot11TransmitSoundingPPDUOptionImplemented;
}


OAL_STATIC OAL_INLINE wlan_11b_mib_preamble_enum_uint8 mac_mib_get_ShortPreambleOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return (pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11ShortPreambleOptionImplemented);
}

OAL_STATIC OAL_INLINE void mac_mib_set_ShortPreambleOptionImplemented(
    mac_vap_stru *pst_mac_vap, wlan_11b_mib_preamble_enum_uint8 en_preamble)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11ShortPreambleOptionImplemented = en_preamble;
}


OAL_STATIC OAL_INLINE void mac_mib_set_PBCCOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                 oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11PBCCOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_PBCCOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11PBCCOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ChannelAgilityPresent(mac_vap_stru *pst_mac_vap,
                                                                 oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11ChannelAgilityPresent = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ChannelAgilityPresent(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_hrdsss.en_dot11ChannelAgilityPresent;
}


OAL_STATIC OAL_INLINE void mac_mib_set_DSSSOFDMOptionActivated(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11DSSSOFDMOptionActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_DSSSOFDMOptionActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11DSSSOFDMOptionActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ShortSlotTimeOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                          oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11ShortSlotTimeOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ShortSlotTimeOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11ShortSlotTimeOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ShortSlotTimeOptionActivated(mac_vap_stru *pst_mac_vap,
                                                                        oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11ShortSlotTimeOptionActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ShortSlotTimeOptionActivated(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_dsss.st_phy_erp.en_dot11ShortSlotTimeOptionActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ReceiveStaggerSoundingOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ReceiveStaggerSoundingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ReceiveStaggerSoundingOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ReceiveStaggerSoundingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TransmitStaggerSoundingOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11TransmitStaggerSoundingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TransmitStaggerSoundingOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11TransmitStaggerSoundingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ReceiveNDPOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                       oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ReceiveNDPOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ReceiveNDPOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ReceiveNDPOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TransmitNDPOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                        oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11TransmitNDPOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TransmitNDPOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11TransmitNDPOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ImplicitTransmitBeamformingOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ImplicitTransmitBeamformingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ImplicitTransmitBeamformingOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ImplicitTransmitBeamformingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_CalibrationOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                        oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11CalibrationOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_CalibrationOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11CalibrationOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitCSITransmitBeamformingOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ExplicitCSITransmitBeamformingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitCSITransmitBeamformingOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ExplicitCSITransmitBeamformingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitNonCompressedBeamformingMatrixOptionImplemented(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ExplicitNonCompressedBeamformingMatrixOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitNonCompressedBeamformingMatrixOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.en_dot11ExplicitNonCompressedBeamformingMatrixOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitTransmitBeamformingCSIFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitTransmitBeamformingCSIFeedbackOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitTransmitBeamformingCSIFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitTransmitBeamformingCSIFeedbackOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitNonCompressedBeamformingFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitNonCompressedBeamformingFeedbackOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitNonCompressedBeamformingFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitNonCompressedBeamformingFeedbackOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ExplicitCompressedBeamformingFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitCompressedBeamformingFeedbackOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_ExplicitCompressedBeamformingFeedbackOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11ExplicitCompressedBeamformingFeedbackOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_NumberBeamFormingCSISupportAntenna(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberBeamFormingCSISupportAntenna = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_NumberBeamFormingCSISupportAntenna(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberBeamFormingCSISupportAntenna;
}


OAL_STATIC OAL_INLINE void mac_mib_set_NumberNonCompressedBeamformingMatrixSupportAntenna(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberNonCompressedBeamformingMatrixSupportAntenna = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_NumberNonCompressedBeamformingMatrixSupportAntenna(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberNonCompressedBeamformingMatrixSupportAntenna;
}


OAL_STATIC OAL_INLINE void mac_mib_set_NumberCompressedBeamformingMatrixSupportAntenna(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberCompressedBeamformingMatrixSupportAntenna = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_NumberCompressedBeamformingMatrixSupportAntenna(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.st_wlan_mib_txbf_config.uc_dot11NumberCompressedBeamformingMatrixSupportAntenna;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTSUBeamformeeOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTSUBeamformeeOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTSUBeamformeeOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTSUBeamformeeOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTSUBeamformerOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTSUBeamformerOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTSUBeamformerOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTSUBeamformerOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTMUBeamformeeOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTMUBeamformeeOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTMUBeamformeeOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTMUBeamformeeOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTMUBeamformerOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTMUBeamformerOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTMUBeamformerOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.en_dot11VHTMUBeamformerOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTNumberSoundingDimensions(mac_vap_stru *pst_mac_vap,
                                                                       oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.uc_dot11VHTNumberSoundingDimensions = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTNumberSoundingDimensions(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.uc_dot11VHTNumberSoundingDimensions;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTBeamformeeNTxSupport(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.uc_dot11VHTBeamformeeNTxSupport = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_VHTBeamformeeNTxSupport(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.st_wlan_mib_vht_txbf_config.uc_dot11VHTBeamformeeNTxSupport;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTChannelWidthOptionImplemented(
    mac_vap_stru *pst_mac_vap, wlan_mib_vht_supp_width_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTChannelWidthOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTChannelWidthOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTChannelWidthOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTShortGIOptionIn80Implemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn80Implemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTShortGIOptionIn80Implemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn80Implemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTShortGIOptionIn160and80p80Implemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn160and80p80Implemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTShortGIOptionIn160and80p80Implemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn160and80p80Implemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTLDPCCodingOptionImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTLDPCCodingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTLDPCCodingOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTLDPCCodingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTTxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                      oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTTxSTBCOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTTxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTTxSTBCOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_VHTRxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                      oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTRxSTBCOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_VHTRxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTRxSTBCOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_HTGreenfieldOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                         oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11HTGreenfieldOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_HTGreenfieldOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11HTGreenfieldOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_ShortGIOptionInTwentyImplemented(
    mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11ShortGIOptionInTwentyImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_ShortGIOptionInTwentyImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11ShortGIOptionInTwentyImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_LDPCCodingOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                       oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11LDPCCodingOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_LDPCCodingOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11LDPCCodingOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_LDPCCodingOptionActivated(mac_vap_stru *pst_mac_vap,
                                                                     oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11LDPCCodingOptionActivated = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_LDPCCodingOptionActivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11LDPCCodingOptionActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxSTBCOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TxSTBCOptionImplemented(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxSTBCOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxSTBCOptionActivated(mac_vap_stru *pst_mac_vap,
                                                                 oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxSTBCOptionActivated = en_val;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_TxSTBCOptionActivated(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxSTBCOptionActivated;
}


OAL_STATIC OAL_INLINE void mac_mib_set_RxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap,
                                                                   oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11RxSTBCOptionImplemented = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_RxSTBCOptionImplemented(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11RxSTBCOptionImplemented;
}


OAL_STATIC OAL_INLINE void mac_mib_set_HighestSupportedDataRate(mac_vap_stru *pst_mac_vap,
                                                                    uint32_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.ul_dot11HighestSupportedDataRate = en_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_HighestSupportedDataRate(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.ul_dot11HighestSupportedDataRate;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxMCSSetDefined(mac_vap_stru *pst_mac_vap,
                                                           oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxMCSSetDefined = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxMCSSetDefined(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxMCSSetDefined;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxRxMCSSetNotEqual(mac_vap_stru *pst_mac_vap,
                                                              oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxRxMCSSetNotEqual = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxRxMCSSetNotEqual(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxRxMCSSetNotEqual;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxMaximumNumberSpatialStreamsSupported(
    mac_vap_stru *pst_mac_vap, uint32_t en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.ul_dot11TxMaximumNumberSpatialStreamsSupported = en_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_TxMaximumNumberSpatialStreamsSupported(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.ul_dot11TxMaximumNumberSpatialStreamsSupported;
}


OAL_STATIC OAL_INLINE void mac_mib_set_TxUnequalModulationSupported(mac_vap_stru *pst_mac_vap,
                                                                        oal_bool_enum_uint8 en_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxUnequalModulationSupported = en_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_TxUnequalModulationSupported(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_phy_ht.en_dot11TxUnequalModulationSupported;
}

OAL_STATIC OAL_INLINE void mac_mib_set_SupportedMCSTxValue(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index, uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcstx.auc_dot11SupportedMCSTxValue[uc_index] = uc_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_SupportedMCSTxValue(mac_vap_stru *pst_mac_vap,
                                                                uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcstx.auc_dot11SupportedMCSTxValue[uc_index];
}

OAL_STATIC OAL_INLINE uint8_t *mac_mib_get_SupportedMCSTx(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcstx.auc_dot11SupportedMCSTxValue;
}

OAL_STATIC OAL_INLINE void mac_mib_set_SupportedMCSRxValue(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index, uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcsrx.auc_dot11SupportedMCSRxValue[uc_index] = uc_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_SupportedMCSRxValue(mac_vap_stru *pst_mac_vap,
                                                                uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcsrx.auc_dot11SupportedMCSRxValue[uc_index];
}

OAL_STATIC OAL_INLINE uint8_t *mac_mib_get_SupportedMCSRx(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->pst_mib_info->st_supported_mcs.st_supported_mcsrx.auc_dot11SupportedMCSRxValue;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableIndex(mac_vap_stru *pst_mac_vap,
                                                          uint8_t uc_index,
                                                          uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableIndex = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_EDCATableIndex(mac_vap_stru *pst_mac_vap,
                                                            uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableIndex;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableCWmin(mac_vap_stru *pst_mac_vap,
                                                          uint8_t uc_index,
                                                          uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableCWmin = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_EDCATableCWmin(mac_vap_stru *pst_mac_vap,
    uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableCWmin;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableCWmax(mac_vap_stru *pst_mac_vap,
                                                          uint8_t uc_index,
                                                          uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableCWmax = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_EDCATableCWmax(mac_vap_stru *pst_mac_vap,
                                                            uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableCWmax;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                          uint8_t uc_index,
                                                          uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableAIFSN = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_EDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                            uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableAIFSN;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableTXOPLimit(mac_vap_stru *pst_mac_vap,
                                                              uint8_t uc_index, uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableTXOPLimit = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_EDCATableTXOPLimit(mac_vap_stru *pst_mac_vap,
                                                                uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].ul_dot11EDCATableTXOPLimit;
}

OAL_STATIC OAL_INLINE void mac_mib_set_EDCATableMandatory(mac_vap_stru *pst_mac_vap,
                                                              uint8_t uc_index, uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].en_dot11EDCATableMandatory = uc_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_EDCATableMandatory(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_edca[uc_index].en_dot11EDCATableMandatory;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableIndex(mac_vap_stru *pst_mac_vap,
                                                             uint8_t uc_index,
                                                             uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableIndex = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableIndex(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableIndex;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableCWmin(mac_vap_stru *pst_mac_vap,
                                                             uint8_t uc_index,
                                                             uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmin = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableCWmin(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmin;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableCWmax(mac_vap_stru *pst_mac_vap,
                                                             uint8_t uc_index,
                                                             uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmax = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableCWmax(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmax;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                             uint8_t uc_index,
                                                             uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableAIFSN = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableAIFSN;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableTXOPLimit(mac_vap_stru *pst_mac_vap,
                                                                 uint8_t uc_index,
                                                                 uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableTXOPLimit = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableTXOPLimit(mac_vap_stru *pst_mac_vap,
                                                                   uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableTXOPLimit;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableMandatory(mac_vap_stru *pst_mac_vap,
                                                                 uint8_t uc_index,
                                                                 uint8_t uc_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].en_dot11QAPEDCATableMandatory = uc_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_QAPEDCATableMandatory(mac_vap_stru *pst_mac_vap,
                                                                  uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].en_dot11QAPEDCATableMandatory;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPEDCATableMSDULifetime(mac_vap_stru *pst_mac_vap,
                                                                    uint8_t uc_index,
                                                                    uint32_t ul_val)
{
    pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableMSDULifetime =
        ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPEDCATableMSDULifetime(mac_vap_stru *pst_mac_vap,
                                                                      uint8_t uc_index)
{
    return pst_mac_vap->pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableMSDULifetime;
}

#ifdef _PRE_WLAN_FEATURE_11AX
OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableIndex(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index,
                                                               uint32_t ul_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableIndex = ul_val;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableCWmin(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index,
                                                               uint32_t ul_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmin = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPMUEDCATableCWmin(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index)
{
    return pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmin;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableCWmax(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index,
                                                               uint32_t ul_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmax = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPMUEDCATableCWmax(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index)
{
    return pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableCWmax;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                               uint8_t uc_index,
                                                               uint32_t ul_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableAIFSN = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPMUEDCATableAIFSN(mac_vap_stru *pst_mac_vap,
                                                                 uint8_t uc_index)
{
    return pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableAIFSN;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableTXOPLimit(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index, uint32_t ul_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableTXOPLimit = ul_val;
}

OAL_STATIC OAL_INLINE uint32_t mac_mib_get_QAPMUEDCATableTXOPLimit(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index)
{
    return pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].ul_dot11QAPEDCATableTXOPLimit;
}

OAL_STATIC OAL_INLINE void mac_mib_set_QAPMUEDCATableMandatory(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index, uint8_t uc_val)
{
    pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].en_dot11QAPEDCATableMandatory = uc_val;
}

OAL_STATIC OAL_INLINE uint8_t mac_mib_get_QAPMUEDCATableMandatory(
    mac_vap_stru *pst_mac_vap, uint8_t uc_index)
{
    return pst_mac_vap->st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac[uc_index].en_dot11QAPEDCATableMandatory;
}

OAL_STATIC OAL_INLINE void mac_mib_set_PPEThresholdsRequired(mac_vap_stru *pst_mac_vap,
                                                                 oal_bool_enum_uint8 en_value)
{
    pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11PPEThresholdsRequired = en_value;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_mib_get_PPEThresholdsRequired(
    mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_wlan_rom_mib_he_phy_config.en_dot11PPEThresholdsRequired;
}

#endif

static inline uint8_t mac_is_primary_legacy_vap(mac_vap_stru *mac_vap)
{
    return mac_vap->is_primary_vap;
}

static inline uint8_t mac_is_primary_legacy_sta(mac_vap_stru *mac_vap)
{
    return mac_is_primary_legacy_vap(mac_vap) && IS_LEGACY_STA(mac_vap);
}

static inline uint8_t mac_is_secondary_sta(mac_vap_stru *mac_vap)
{
    return !mac_is_primary_legacy_vap(mac_vap) && IS_LEGACY_STA(mac_vap);
}

/* 10 �������� */
uint32_t mac_vap_init(mac_vap_stru *pst_vap,
                                   uint8_t uc_chip_id,
                                   uint8_t uc_device_id,
                                   uint8_t uc_vap_id,
                                   mac_cfg_add_vap_param_stru *pst_param);
void mac_vap_init_rates(mac_vap_stru *pst_vap);
void mac_sta_init_bss_rates(mac_vap_stru *pst_vap, void *pst_bss_dscr);
void mac_vap_init_rates_by_protocol(mac_vap_stru *pst_vap,
                                                   wlan_protocol_enum_uint8 en_vap_protocol,
                                                   mac_data_rate_stru *pst_rates);
uint32_t mac_vap_exit(mac_vap_stru *pst_vap);
uint32_t mac_vap_del_user(mac_vap_stru *pst_vap, uint16_t us_user_idx);
uint32_t mac_vap_find_user_by_macaddr(mac_vap_stru *pst_vap,
                                                   const unsigned char *puc_sta_mac_addr,
                                                   uint16_t *pus_user_idx);
uint32_t mac_device_find_user_by_macaddr(uint8_t uc_device_id,
                                                      const unsigned char *puc_sta_mac_addr,
                                                      uint16_t *pus_user_idx);
uint32_t mac_chip_find_user_by_macaddr(uint8_t uc_chip_id,
                                                const unsigned char *puc_sta_mac_addr,
                                                uint16_t *pus_user_idx);
uint32_t mac_board_find_user_by_macaddr(const unsigned char *puc_sta_mac_addr,
                                                 uint16_t *pus_user_idx);
uint32_t mac_vap_add_assoc_user(mac_vap_stru *pst_vap, uint16_t us_user_idx);

uint8_t mac_vap_get_bandwith(wlan_bw_cap_enum_uint8 en_dev_cap,
                                      wlan_channel_bandwidth_enum_uint8 en_bss_cap);


OAL_STATIC OAL_INLINE hal_fcs_protect_type_enum_uint8 mac_fcs_get_protect_type(mac_vap_stru *pst_mac_vap)
{
    hal_fcs_protect_type_enum_uint8 en_protect_type;

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        en_protect_type = HAL_FCS_PROTECT_TYPE_SELF_CTS;
    } else if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NULL_DATA;
    } else {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NONE;
    }

    if (pst_mac_vap->en_p2p_mode == WLAN_P2P_GO_MODE) {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NONE;
    }

    return en_protect_type;
}


OAL_STATIC OAL_INLINE uint8_t mac_fcs_get_protect_cnt(mac_vap_stru *pst_mac_vap)
{
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        return HAL_FCS_PROTECT_CNT_1;
    }

    /* staģʽ��Ҫ��Ӳ����ʱ�����one pkt���ͣ��������Ծ����������ã�btcoex��ͨ��������Ҫ���ø���ʱ�� */
    return HAL_FCS_PROTECT_CNT_20;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_sta_get_he_ignore_non_he_cap(mac_vap_stru *pst_mac_vap)
{
    oal_bool_enum_uint8 en_ignore_non_he_cap = OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (MAC_VAP_IS_WORK_HE_PROTOCOL(pst_mac_vap) &&
            g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_ignore_non_he_cap_from_beacon) {
            en_ignore_non_he_cap = OAL_TRUE;
        }
    }
#endif
    return en_ignore_non_he_cap;
}

/*****************************************************************************
    mib��������
*****************************************************************************/
uint32_t mac_mib_get_beacon_period(mac_vap_stru *pst_mac_vap,
                                                uint8_t *puc_len,
                                                uint8_t *puc_param);
uint32_t mac_mib_get_dtim_period(mac_vap_stru *pst_mac_vap,
                                              uint8_t *puc_len,
                                              uint8_t *puc_param);
uint32_t mac_mib_get_bss_type(mac_vap_stru *pst_mac_vap,
                                           uint8_t *puc_len,
                                           uint8_t *puc_param);
uint32_t mac_mib_get_ssid(mac_vap_stru *pst_mac_vap, uint8_t *puc_len, uint8_t *puc_param);
uint32_t mac_mib_set_beacon_period(mac_vap_stru *pst_mac_vap,
                                                uint8_t uc_len,
                                                uint8_t *puc_param);
uint32_t mac_mib_set_dtim_period(mac_vap_stru *pst_mac_vap,
                                              uint8_t uc_len,
                                              uint8_t *puc_param);
uint32_t mac_mib_set_bss_type(mac_vap_stru *pst_mac_vap,
                                           uint8_t uc_len,
                                           uint8_t *puc_param);
uint32_t mac_mib_set_ssid(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);
uint32_t mac_mib_set_station_id(mac_vap_stru *pst_mac_vap,
                                             uint8_t uc_len,
                                             uint8_t *puc_param);
uint32_t mac_mib_get_shpreamble(mac_vap_stru *pst_mac_vap,
                                             uint8_t *puc_len,
                                             uint8_t *puc_param);
uint32_t mac_mib_set_shpreamble(mac_vap_stru *pst_mac_vap,
                                             uint8_t uc_len,
                                             uint8_t *puc_param);
uint32_t mac_vap_set_bssid(mac_vap_stru *pst_mac_vap, uint8_t *puc_bssid);
uint32_t mac_vap_set_current_channel(mac_vap_stru *pst_vap,
                                                  wlan_channel_band_enum_uint8 en_band,
                                                  uint8_t uc_channel);
void mac_vap_state_change(mac_vap_stru *pst_mac_vap, mac_vap_state_enum_uint8 en_vap_state);
uint32_t mac_vap_config_vht_ht_mib_by_protocol(mac_vap_stru *pst_mac_vap);
uint32_t mac_vap_init_wme_param(mac_vap_stru *pst_mac_vap);
#ifdef _PRE_WLAN_FEATURE_TXOPPS
uint8_t mac_vap_get_txopps(mac_vap_stru *pst_vap);
void mac_vap_set_txopps(mac_vap_stru *pst_vap, uint8_t uc_value);
void mac_vap_update_txopps(mac_vap_stru *pst_vap, mac_user_stru *pst_user);
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS
wlan_mib_mimo_power_save_enum_uint8 mac_vap_get_smps_mode(mac_vap_stru *pst_mac_vap);
wlan_mib_mimo_power_save_enum_uint8 mac_vap_get_smps_en(mac_vap_stru *pst_mac_vap);
void mac_vap_set_smps(mac_vap_stru *pst_vap, uint8_t uc_value);
#endif
uint32_t mac_vap_set_uapsd_en(mac_vap_stru *pst_mac_vap, uint8_t uc_value);
uint8_t mac_vap_get_uapsd_en(mac_vap_stru *pst_mac_vap);
uint32_t mac_vap_init_by_protocol(mac_vap_stru *pst_mac_vap,
                                               wlan_protocol_enum_uint8 en_protocol);

oal_bool_enum_uint8 mac_vap_check_bss_cap_info_phy_ap(uint16_t us_cap_info,
                                                                 mac_vap_stru *pst_mac_vap);
mac_wme_param_stru *mac_get_wmm_cfg(wlan_vap_mode_enum_uint8 en_vap_mode);
#ifdef _PRE_WLAN_FEATURE_EDCA_MULTI_USER_MULTI_AC
mac_wme_param_stru *mac_get_wmm_cfg_multi_user_multi_ac(
    oal_traffic_type_enum_uint8 en_vap_mode);
#endif
void mac_vap_get_bandwidth_cap(mac_vap_stru *pst_mac_vap,
                                              wlan_bw_cap_enum_uint8 *pen_cap);
void mac_vap_change_mib_by_bandwidth(mac_vap_stru *pst_mac_vap,
                                                    wlan_channel_band_enum_uint8 en_band,
                                                    wlan_channel_bandwidth_enum_uint8 en_bandwidth);
wlan_bw_cap_enum_uint8 mac_vap_bw_mode_to_bw(wlan_channel_bandwidth_enum_uint8 en_mode);
void mac_vap_init_rx_nss_by_protocol(mac_vap_stru *pst_mac_vap);
uint8_t mac_vap_get_ap_usr_opern_bandwidth(mac_vap_stru *pst_mac_sta,
                                                    mac_user_stru *pst_mac_user);
uint32_t mac_device_find_up_vap_ram(mac_device_stru *pst_mac_device,
    mac_vap_stru **ppst_mac_vap);
uint32_t mac_vap_check_ap_usr_opern_bandwidth(mac_vap_stru *pst_mac_sta,
                                                       mac_user_stru *pst_mac_user);
uint8_t mac_vap_set_bw_check(mac_vap_stru *pst_mac_sta,
                                      wlan_channel_bandwidth_enum_uint8 en_sta_new_bandwidth);
void mac_dec_p2p_num(mac_vap_stru *pst_vap);
void mac_inc_p2p_num(mac_vap_stru *pst_vap);
void mac_vap_set_p2p_mode(mac_vap_stru *pst_vap, wlan_p2p_mode_enum_uint8 en_p2p_mode);
wlan_p2p_mode_enum_uint8 mac_get_p2p_mode(mac_vap_stru *pst_vap);
void mac_vap_set_aid(mac_vap_stru *pst_vap, uint16_t us_aid);
void mac_vap_set_uapsd_cap(mac_vap_stru *pst_vap, uint8_t uc_uapsd_cap);
void mac_vap_set_assoc_id(mac_vap_stru *pst_vap, uint16_t us_assoc_vap_id);
void mac_vap_set_tx_power(mac_vap_stru *pst_vap, uint8_t uc_tx_power);
void mac_vap_set_al_tx_flag(mac_vap_stru *pst_vap, oal_bool_enum_uint8 en_flag);
void mac_vap_set_al_tx_payload_flag(mac_vap_stru *pst_vap, uint8_t uc_paylod);
uint32_t mac_dump_protection(mac_vap_stru *pst_mac_vap, uint8_t *puc_param);
void mac_vap_set_multi_user_idx(mac_vap_stru *pst_vap, uint16_t us_multi_user_idx);
void mac_vap_set_uapsd_para(mac_vap_stru *pst_mac_vap,
                                           mac_cfg_uapsd_sta_stru *pst_uapsd_info);
void mac_vap_set_wmm_params_update_count(mac_vap_stru *pst_vap,
                                                        uint8_t uc_update_count);
void mac_vap_set_rifs_tx_on(mac_vap_stru *pst_vap, uint8_t uc_value);
/* ����tdls���ܿ��ܻ�� */
void mac_vap_set_11ac2g(mac_vap_stru *pst_vap, uint8_t uc_value);
void mac_vap_set_hide_ssid(mac_vap_stru *pst_vap, uint8_t uc_value);
uint8_t mac_vap_get_peer_obss_scan(mac_vap_stru *pst_vap);
void mac_vap_set_peer_obss_scan(mac_vap_stru *pst_vap, uint8_t uc_value);
uint32_t mac_vap_clear_app_ie(mac_vap_stru *pst_mac_vap, en_app_ie_type_uint8 en_type);
uint32_t mac_vap_save_app_ie(mac_vap_stru *pst_mac_vap,
                                          oal_app_ie_stru *pst_app_ie,
                                          en_app_ie_type_uint8 en_type);
void mac_vap_set_rx_nss(mac_vap_stru *pst_vap, wlan_nss_enum_uint8 en_rx_nss);
uint32_t mac_vap_init_privacy(mac_vap_stru *pst_mac_vap, mac_conn_security_stru *pst_conn_sec);
uint32_t mac_mib_set_wep(mac_vap_stru *pst_mac_vap,
                                      uint8_t uc_key_id,
                                      uint8_t uc_key_value);
mac_user_stru *mac_vap_get_user_by_addr(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr);
uint32_t mac_vap_set_security(mac_vap_stru *pst_mac_vap, mac_beacon_param_stru *pst_beacon_param);
uint32_t mac_vap_add_key(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user,
                                      uint8_t uc_key_id, mac_key_params_stru *pst_key);
uint8_t mac_vap_get_default_key_id(mac_vap_stru *pst_mac_vap);
uint32_t mac_vap_set_default_wep_key(mac_vap_stru *pst_mac_vap, uint8_t uc_key_index);
uint32_t mac_vap_set_default_mgmt_key(mac_vap_stru *pst_mac_vap, uint8_t uc_key_index);
void mac_vap_init_user_security_port(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user);
uint8_t *mac_vap_get_mac_addr(mac_vap_stru *pst_mac_vap);
#ifdef _PRE_WLAN_FEATURE_11R
uint32_t mac_mib_init_ft_cfg(mac_vap_stru *pst_mac_vap, uint8_t *puc_mde);
uint32_t mac_mib_get_md_id(mac_vap_stru *pst_mac_vap, uint16_t *pus_mdid);
#endif  // _PRE_WLAN_FEATURE_11R
oal_switch_enum_uint8 mac_vap_protection_autoprot_is_enabled(mac_vap_stru *pst_mac_vap);
wlan_prot_mode_enum_uint8 mac_vap_get_user_protection_mode(mac_vap_stru *pst_mac_vap_sta,
                                                                      mac_user_stru *pst_mac_user);

oal_bool_enum mac_protection_lsigtxop_check(mac_vap_stru *pst_mac_vap);
void mac_protection_set_lsig_txop_mechanism(mac_vap_stru *pst_mac_vap,
                                                           oal_switch_enum_uint8 en_flag);
void mac_device_set_vap_id(mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap,
                                          uint8_t uc_vap_idx, wlan_vap_mode_enum_uint8 en_vap_mode,
                                          wlan_p2p_mode_enum_uint8 en_p2p_mode, uint8_t is_add_vap);
uint32_t mac_device_find_up_vap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
mac_vap_stru *mac_device_find_another_up_vap(mac_device_stru *pst_mac_device,
                                                        uint8_t uc_vap_id_self);
uint32_t mac_device_find_up_ap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
uint32_t mac_device_calc_up_vap_num(mac_device_stru *pst_mac_device);
uint32_t mac_device_calc_work_vap_num(mac_device_stru *pst_mac_device);
uint32_t mac_device_find_up_p2p_go(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
uint32_t mac_device_get_up_vap_num(mac_device_stru *pst_mac_device);
uint32_t mac_device_find_2up_vap(mac_device_stru *pst_mac_device,
    mac_vap_stru **ppst_mac_vap1, mac_vap_stru **ppst_mac_vap2);
uint32_t mac_fcs_dbac_state_check(mac_device_stru *pst_mac_device);
uint32_t mac_device_find_up_sta(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
uint32_t mac_device_is_p2p_connected(mac_device_stru *pst_mac_device);

#ifdef _PRE_WLAN_FEATURE_SMPS
uint32_t mac_device_find_smps_mode_en(mac_device_stru *pst_mac_device,
                                               wlan_mib_mimo_power_save_enum_uint8 en_smps_mode);
#endif
void mac_device_set_channel(mac_device_stru *pst_mac_device,
                                           mac_cfg_channel_param_stru *pst_channel_param);
void mac_device_get_channel(mac_device_stru *pst_mac_device,
                                           mac_cfg_channel_param_stru *pst_channel_param);
uint32_t mac_fcs_get_prot_datarate(mac_vap_stru *pst_src_vap);

uint32_t mac_fcs_get_prot_mode(mac_vap_stru *pst_src_vap);
void mac_blacklist_free_pointer(mac_vap_stru *pst_mac_vap, mac_blacklist_info_stru *pst_blacklist_info);

void mac_vap_init_legacy_rates(mac_vap_stru *pst_vap, mac_data_rate_stru *pst_rates);
void mac_vap_init_11n_rates(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
void mac_vap_init_11g_mixed_one_rates(mac_vap_stru *pst_vap, mac_data_rate_stru *pst_rates);
#ifdef _PRE_WLAN_FEATURE_11AX
void mac_vap_stru_init_11ax_rates(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
void mac_vap_init_11ax_rates(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
#endif
void mac_vap_init_mib_11ax(mac_vap_stru *pst_mac_vap);
void mac_vap_init_mib_11ax_1103(mac_vap_stru *mac_vap, uint32_t nss_num);
void mac_vap_init_mib_11ax_1105(mac_vap_stru *mac_vap, uint32_t nss_num);
void mac_vap_init_mib_11ax_1106(mac_vap_stru *mac_vap, uint32_t nss_num);
void mac_vap_init_11ac_rates(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
void mac_init_mib_extend(mac_vap_stru *pst_mac_vap);
void mac_vap_init_11n_rates_extend(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
void mac_vap_init_mib_11n_txbf(mac_vap_stru *pst_mac_vap);
void mac_vap_init_11ac_mcs_singlenss(mac_vap_stru *pst_mac_vap,
                                                wlan_channel_bandwidth_enum_uint8 en_bandwidth);
uint32_t mac_vap_add_wep_key(mac_vap_stru *pst_mac_vap, uint8_t us_key_idx,
                                      uint8_t uc_key_len, uint8_t *puc_key);
oal_bool_enum_uint8 mac_vap_need_set_user_htc_cap_1103(mac_vap_stru *mac_vap);
oal_bool_enum_uint8 mac_vap_need_set_user_htc_cap_1106(mac_vap_stru *mac_vap);

void mac_vap_tx_data_set_user_htc_cap(mac_vap_stru *pst_mac_vap,
                                                 mac_user_stru *pst_mac_user);
void mac_init_mib(mac_vap_stru *pst_mac_vap);
void mac_vap_free_mib(mac_vap_stru *pst_vap);
mac_vap_stru *mac_vap_find_another_up_vap_by_mac_vap(mac_vap_stru *pst_vap);
void mac_vap_csa_support_set(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_cap);
oal_bool_enum_uint8 mac_vap_go_can_not_in_160M_check(mac_vap_stru *p_mac_vap,
    uint8_t vap_channel);
uint32_t mac_vap_set_cb_tx_user_idx(mac_vap_stru *pst_mac_vap,
    mac_tx_ctl_stru *pst_tx_ctl, const unsigned char *puc_data);
extern oal_bool_enum_uint8 mac_vap_can_not_start_he_protocol(mac_vap_stru *p_mac_vap);
extern oal_bool_enum_uint8 mac_vap_p2p_bw_back_to_40m(mac_vap_stru *p_mac_vap,
    int32_t channel,  wlan_channel_bandwidth_enum_uint8  *p_channlBandWidth);
extern oal_bool_enum_uint8 mac_vap_avoid_dbac_close_vht_protocol(mac_vap_stru *p_mac_vap);
oal_bool_enum_uint8 mac_vap_need_proto_downgrade(mac_vap_stru *vap);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_vap.h */
