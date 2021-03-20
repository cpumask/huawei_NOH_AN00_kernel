

#include "mac_resource.h"
#include "hmac_tx_data.h"
#include "wal_linux_ioctl.h"
#include "wal_linux_atcmdsrv.h"
#include "hmac_resource.h"
#include "hmac_auto_adjust_freq.h"
#include "hmac_roam_main.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
#include "hisi_customize_wifi_hi110x.h"
#endif
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "mac_hiex.h"
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_ftm.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID      OAM_FILE_ID_WAL_LINUX_CUSTOMIZE_C

#if defined(WIN32)
#include "hisi_customize_wifi_hi110x.h"
#endif
#include "securec.h"
#include "securectype.h"
#include "wal_linux_customize.h"

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
/* 每次上下电由配置vap完成的定制化只配置一次，wlan p2p iface不再重复配置 */
OAL_STATIC uint8_t g_uc_cfg_once_flag = OAL_FALSE;
/* 后续可考虑开wifi时间只在staut或aput第一次上电时从ini配置文件中读取参数 */
extern host_speed_freq_level_stru g_host_speed_freq_level[4];
extern device_speed_freq_level_stru g_device_speed_freq_level[4];
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

uint32_t hwifi_force_refresh_rf_params(oal_net_device_stru *pst_net_dev)
{
    /* update params */
    if (wlan_chip_force_update_custom_params() != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* send data to device */
    return wlan_chip_init_nvram_main(pst_net_dev);

}
void hwifi_1106_config_host_global_11ax_ini_param(void)
{
    g_pst_mac_device_capability[0].en_11ax_switch =
        (g_cust_cap.wifi_11ax_switch_mask & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_switch =
        (g_cust_cap.wifi_11ax_switch_mask & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_ignore_non_he_cap_from_beacon =
        (g_cust_cap.wifi_11ax_switch_mask & BIT2) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_he_cap_switch =
        (g_cust_cap.wifi_11ax_switch_mask & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_responder_support =
        (g_cust_cap.wifi_11ax_switch_mask & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_requester_support =
        (g_cust_cap.wifi_11ax_switch_mask & BIT5) ? OAL_TRUE : OAL_FALSE;
#endif

    g_pst_mac_device_capability[0].bit_multi_bssid_switch =
        (g_cust_cap.mult_bssid_switch_mask & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_htc_include =
        (g_cust_cap.htc_switch_mask & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_om_in_data =
        (g_cust_cap.htc_switch_mask & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_rom_cap_switch =
        (g_cust_cap.htc_switch_mask & BIT2) ? OAL_TRUE : OAL_FALSE;
#endif
oal_io_print("hwifi_1106_config_host_global_11ax_ini_param en[%d],mult bssid[%d],rom cap[%d]\r\n",
    g_pst_mac_device_capability[0].en_11ax_switch, g_pst_mac_device_capability[0].bit_multi_bssid_switch,
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_rom_cap_switch);
}

void hwifi_config_host_global_11ax_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_11AX_SWITCH);
    g_pst_mac_device_capability[0].en_11ax_switch = (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_switch =
        (((uint32_t)l_val & 0x0F) & BIT1) ? OAL_TRUE : OAL_FALSE;

    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_ignore_non_he_cap_from_beacon =
            (((uint32_t)l_val & 0x0F) & BIT2) ? OAL_TRUE : OAL_FALSE;

    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_he_cap_switch =
        (((uint32_t)l_val & 0x0F) & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_responder_support =
        (((uint32_t)l_val & 0xFF) & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_requester_support =
        (((uint32_t)l_val & 0xFF) & BIT5) ? OAL_TRUE : OAL_FALSE;
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MULTI_BSSID_SWITCH);
    g_pst_mac_device_capability[0].bit_multi_bssid_switch = (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HTC_SWITCH);
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_htc_include =
        (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_om_in_data =
        (((uint32_t)l_val & 0x0F) & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_rom_cap_switch =
        (((uint32_t)l_val & 0x0F) & BIT2) ? OAL_TRUE : OAL_FALSE;
#endif
}

OAL_STATIC void hwifi_1106_set_voe_custom_param(void)
{
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11k = (g_cust_cap.voe_switch_mask & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11v = (g_cust_cap.voe_switch_mask & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r = (g_cust_cap.voe_switch_mask & BIT2) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r_ds =
        (g_cust_cap.voe_switch_mask & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_adaptive11r =
        (g_cust_cap.voe_switch_mask & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_nb_rpt_11k =
        (g_cust_cap.voe_switch_mask & BIT5) ? OAL_TRUE : OAL_FALSE;

    return;
}

OAL_STATIC void hwifi_set_voe_custom_param(void)
{
    uint32_t ul_val;

    ul_val = (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_VOE_SWITCH);
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11k = (ul_val & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11v = (ul_val & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r = (ul_val & BIT2) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r_ds = (ul_val & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_adaptive11r = (ul_val & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_nb_rpt_11k = (ul_val & BIT5) ? OAL_TRUE : OAL_FALSE;

    return;
}

OAL_STATIC void hwifi_config_host_roam_global_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_ROAM_SWITCH);
    g_wlan_cust->uc_roam_switch = (0 == l_val || 1 == l_val) ?
        (uint8_t)l_val : g_wlan_cust->uc_roam_switch;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SCAN_ORTHOGONAL);
        g_wlan_cust->uc_roam_scan_orthogonal = (1 <= l_val) ?
    (uint8_t)l_val : g_wlan_cust->uc_roam_scan_orthogonal;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_B);
    g_wlan_cust->c_roam_trigger_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_A);
    g_wlan_cust->c_roam_trigger_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_B);
    g_wlan_cust->c_roam_delta_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_A);
    g_wlan_cust->c_roam_delta_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DENSE_ENV_TRIGGER_B);
    g_wlan_cust->c_dense_env_roam_trigger_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DENSE_ENV_TRIGGER_A);
    g_wlan_cust->c_dense_env_roam_trigger_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SCENARIO_ENABLE);
    g_wlan_cust->uc_scenario_enable = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_GOOD_RSSI);
    g_wlan_cust->c_candidate_good_rssi = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_GOOD_NUM);
    g_wlan_cust->uc_candidate_good_num = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_WEAK_NUM);
    g_wlan_cust->uc_candidate_weak_num = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_INTERVAL_VARIABLE);
    g_wlan_cust->us_roam_interval = (uint16_t)l_val;
}

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
OAL_STATIC void hwifi_1106_config_host_amsdu_th_ini_param(void)
{
    g_st_tx_large_amsdu.uc_host_large_amsdu_en = g_cust_host.large_amsdu.large_amsdu_en;
    g_st_tx_large_amsdu.us_amsdu_throughput_high = g_cust_host.large_amsdu.throughput_high;
    g_st_tx_large_amsdu.us_amsdu_throughput_middle = g_cust_host.large_amsdu.throughput_middle;
    g_st_tx_large_amsdu.us_amsdu_throughput_low = g_cust_host.large_amsdu.throughput_low;
    oal_io_print("ampdu+amsdu lareg amsdu en[%d],high[%d],low[%d],middle[%d]\r\n",
        g_st_tx_large_amsdu.uc_host_large_amsdu_en, g_st_tx_large_amsdu.us_amsdu_throughput_high,
        g_st_tx_large_amsdu.us_amsdu_throughput_low, g_st_tx_large_amsdu.us_amsdu_throughput_middle);

    g_st_small_amsdu_switch.uc_ini_small_amsdu_en = g_cust_host.small_amsdu.en_switch;
    g_st_small_amsdu_switch.us_small_amsdu_throughput_high = g_cust_host.small_amsdu.throughput_high;
    g_st_small_amsdu_switch.us_small_amsdu_throughput_low = g_cust_host.small_amsdu.throughput_low;
    g_st_small_amsdu_switch.us_small_amsdu_pps_high = g_cust_host.small_amsdu.pps_high;
    g_st_small_amsdu_switch.us_small_amsdu_pps_low = g_cust_host.small_amsdu.pps_low;
    oal_io_print("SMALL AMSDU SWITCH en[%d],high[%d],low[%d]\r\n",
        g_st_small_amsdu_switch.uc_ini_small_amsdu_en, g_st_small_amsdu_switch.us_small_amsdu_throughput_high,
        g_st_small_amsdu_switch.us_small_amsdu_throughput_low);
}

OAL_STATIC void hwifi_config_host_amsdu_th_ini_param(void)
{
    int32_t l_val = 0;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_AMSDU_SKB);
    g_st_tx_large_amsdu.uc_host_large_amsdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH);
    g_st_tx_large_amsdu.us_amsdu_throughput_high = (l_val > 0) ? (uint16_t)l_val : 500;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_MIDDLE);
    g_st_tx_large_amsdu.us_amsdu_throughput_middle = (l_val > 0) ? (uint16_t)l_val : 100;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW);
    g_st_tx_large_amsdu.us_amsdu_throughput_low = (l_val > 0) ? (uint16_t)l_val : 50;
    oal_io_print("ampdu+amsdu lareg amsdu en[%d],high[%d],low[%d],middle[%d]\r\n",
        g_st_tx_large_amsdu.uc_host_large_amsdu_en, g_st_tx_large_amsdu.us_amsdu_throughput_high,
        g_st_tx_large_amsdu.us_amsdu_throughput_low, g_st_tx_large_amsdu.us_amsdu_throughput_middle);

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_SMALL_AMSDU);
    g_st_small_amsdu_switch.uc_ini_small_amsdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_HIGH);
    g_st_small_amsdu_switch.us_small_amsdu_throughput_high = (l_val > 0) ? (uint16_t)l_val : 300;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_LOW);
    g_st_small_amsdu_switch.us_small_amsdu_throughput_low = (l_val > 0) ? (uint16_t)l_val : 200;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH);
    g_st_small_amsdu_switch.us_small_amsdu_pps_high = (l_val > 0) ? (uint16_t)l_val : 25000;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW);
    g_st_small_amsdu_switch.us_small_amsdu_pps_low = (l_val > 0) ? (uint16_t)l_val : 5000;
    oal_io_print("SMALL AMSDU SWITCH en[%d],high[%d],low[%d]\r\n", g_st_small_amsdu_switch.uc_ini_small_amsdu_en, g_st_small_amsdu_switch.us_small_amsdu_throughput_high, g_st_small_amsdu_switch.us_small_amsdu_throughput_low);
}
#endif

OAL_STATIC void hwifi_1106_config_tcp_ack_buf_ini_param(void)
{
    g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_en = g_cust_host.tcp_ack_buf.buf_en;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high = g_cust_host.tcp_ack_buf.throughput_high;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low = g_cust_host.tcp_ack_buf.throughput_low;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M = g_cust_host.tcp_ack_buf.throughput_high_40M;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M = g_cust_host.tcp_ack_buf.throughput_low_40M;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M = g_cust_host.tcp_ack_buf.throughput_high_80M;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M = g_cust_host.tcp_ack_buf.throughput_low_80M;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M = g_cust_host.tcp_ack_buf.throughput_high_160M;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M = g_cust_host.tcp_ack_buf.buf_userctl_test_en;
    g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_userctl_test_en = g_cust_host.tcp_ack_buf.buf_en;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_high = g_cust_host.tcp_ack_buf.buf_userctl_high;
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_low = g_cust_host.tcp_ack_buf.buf_userctl_low;

    oal_io_print("TCP ACK BUF en[%d],high/low:20M[%d]/[%d],40M[%d]/[%d],80M[%d]/[%d],160M[%d]/[%d]\
        TCP ACK BUF USERCTL[%d], userctl[%d]/[%d]",
                 g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_en,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M,
                 g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_userctl_test_en,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_high,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_low);
}
OAL_STATIC void hwifi_config_tcp_ack_buf_ini_param(void)
{
    int32_t l_val = 0;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_TCP_ACK_BUF);
    g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high = (l_val > 0) ? (uint16_t)l_val : 90;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low = (l_val > 0) ? (uint16_t)l_val : 30;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M = (l_val > 0) ? (uint16_t)l_val : 300;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M = (l_val > 0) ? (uint16_t)l_val : 150;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M = (l_val > 0) ? (uint16_t)l_val : 550;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M = (l_val > 0) ? (uint16_t)l_val : 450;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_160M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M = (l_val > 0) ? (uint16_t)l_val : 800;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_160M);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M = (l_val > 0) ? (uint16_t)l_val : 700;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_TCP_ACK_BUF_USERCTL);
    g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_userctl_test_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    /* 上层默认设置TCP ack上门限30M */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_HIGH);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_high = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_USERCTL_HIGH;
     /* 上层默认设置TCP ack上门限20M */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_LOW);
    g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_low = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_USERCTL_LOW;
    oal_io_print("TCP ACK BUF en[%d],high/low:20M[%d]/[%d],40M[%d]/[%d],80M[%d]/[%d],160M[%d]/[%d],\
                 TCP ACK BUF USERCTL[%d], userctl[%d]/[%d]",
                 g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_en,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M,
                 g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_userctl_test_en,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_high,
                 g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_low);

}
OAL_STATIC void hwifi_1106_config_performance_ini_param(void)
{
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    g_st_ampdu_hw.uc_ampdu_hw_en = g_cust_host.tx_ampdu.ampdu_hw_en;
    g_st_ampdu_hw.us_throughput_high = g_cust_host.tx_ampdu.throughput_high;
    g_st_ampdu_hw.us_throughput_low = g_cust_host.tx_ampdu.throughput_low;
    oal_io_print("ampdu_hw enable[%d]H[%u]L[%u]\r\n", g_st_ampdu_hw.uc_ampdu_hw_en, g_st_ampdu_hw.us_throughput_high,
        g_st_ampdu_hw.us_throughput_low);
#endif

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    hwifi_1106_config_host_amsdu_th_ini_param();
#endif
#ifdef _PRE_WLAN_TCP_OPT
    g_st_tcp_ack_filter.uc_tcp_ack_filter_en = g_cust_host.tcp_ack_filt.filter_en;
    g_st_tcp_ack_filter.us_rx_filter_throughput_high = g_cust_host.tcp_ack_filt.throughput_high;
    g_st_tcp_ack_filter.us_rx_filter_throughput_low = g_cust_host.tcp_ack_filt.throughput_low;
    oal_io_print("tcp ack filter en[%d],high[%d],low[%d]\r\n", g_st_tcp_ack_filter.uc_tcp_ack_filter_en,
        g_st_tcp_ack_filter.us_rx_filter_throughput_high, g_st_tcp_ack_filter.us_rx_filter_throughput_low);
#endif

    g_uc_host_rx_ampdu_amsdu = g_cust_host.host_rx_ampdu_amsdu;
    oal_io_print("Rx:ampdu+amsdu skb en[%d]\r\n", g_uc_host_rx_ampdu_amsdu);

    g_st_rx_buffer_size_stru.us_rx_buffer_size = g_cust_host.rx_buffer_size;
    g_st_rx_buffer_size_stru.en_rx_ampdu_bitmap_ini = (g_cust_host.rx_buffer_size > 0) ? OAL_TRUE : OAL_FALSE;
    oal_io_print("Rx:ampdu bitmap size[%d]\r\n", g_st_rx_buffer_size_stru.us_rx_buffer_size);
}

OAL_STATIC void hwifi_config_performance_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_TX_MAX_NUM);
    g_wlan_cust->ul_ampdu_tx_max_num = (WLAN_AMPDU_TX_MAX_NUM >= l_val && 1 <= l_val) ?
        (uint32_t)l_val : g_wlan_cust->ul_ampdu_tx_max_num;
    oal_io_print("hwifi_config_host_global_ini_param::ampdu_tx_max_num:%d", g_wlan_cust->ul_ampdu_tx_max_num);

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU);
    g_st_ampdu_hw.uc_ampdu_hw_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU_TH_HIGH);
    g_st_ampdu_hw.us_throughput_high = (l_val > 0) ? (uint16_t)l_val : 300;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU_TH_LOW);
    g_st_ampdu_hw.us_throughput_low = (l_val > 0) ? (uint16_t)l_val : 200;
    oal_io_print("ampdu_hw enable[%d]H[%u]L[%u]\r\n", g_st_ampdu_hw.uc_ampdu_hw_en,
        g_st_ampdu_hw.us_throughput_high, g_st_ampdu_hw.us_throughput_low);
#endif

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    hwifi_config_host_amsdu_th_ini_param();
#endif
#ifdef _PRE_WLAN_TCP_OPT
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER);
    g_st_tcp_ack_filter.uc_tcp_ack_filter_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH);
    g_st_tcp_ack_filter.us_rx_filter_throughput_high = (l_val > 0) ? (uint16_t)l_val : 50;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW);
    g_st_tcp_ack_filter.us_rx_filter_throughput_low = (l_val > 0) ? (uint16_t)l_val : 20;
    oal_io_print("tcp ack filter en[%d],high[%d],low[%d]\r\n", g_st_tcp_ack_filter.uc_tcp_ack_filter_en,
        g_st_tcp_ack_filter.us_rx_filter_throughput_high, g_st_tcp_ack_filter.us_rx_filter_throughput_low);
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB);
    g_uc_host_rx_ampdu_amsdu = (l_val > 0) ? (uint8_t)l_val : OAL_FALSE;
    oal_io_print("Rx:ampdu+amsdu skb en[%d]\r\n", g_uc_host_rx_ampdu_amsdu);

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_AMPDU_BITMAP);
    g_st_rx_buffer_size_stru.en_rx_ampdu_bitmap_ini = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    g_st_rx_buffer_size_stru.us_rx_buffer_size = l_val;
    oal_io_print("Rx:ampdu bitmap size[%d]\r\n", l_val);
}

void hwifi_config_cpu_freq_ini_param(void)
{
    wlan_chip_cpu_freq_ini_param_init();
}


void hwifi_1106_config_cpu_freq_ini_param(void)
{
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    g_freq_lock_control.uc_lock_dma_latency = (g_cust_host.freq_lock.dma_latency_val > 0) ?  OAL_TRUE : OAL_FALSE;
    g_freq_lock_control.dma_latency_value = g_cust_host.freq_lock.dma_latency_val;
    oal_io_print("DMA latency[%d]\r\n", g_freq_lock_control.uc_lock_dma_latency);
    g_freq_lock_control.us_lock_cpu_th_high = g_cust_host.freq_lock.lock_cpu_th_high;
    g_freq_lock_control.us_lock_cpu_th_low = g_cust_host.freq_lock.lock_cpu_th_low;
#endif

    g_freq_lock_control.en_irq_affinity = g_cust_host.freq_lock.en_irq_affinity;
    g_freq_lock_control.us_throughput_irq_high = g_cust_host.freq_lock.throughput_irq_high;
    g_freq_lock_control.us_throughput_irq_low = g_cust_host.freq_lock.throughput_irq_low;
    g_freq_lock_control.ul_irq_pps_high = g_cust_host.freq_lock.irq_pps_high;
    g_freq_lock_control.ul_irq_pps_low = g_cust_host.freq_lock.irq_pps_low;
    oal_io_print("hwifi_1106_config_cpu_freq_ini_param irq affinity enable[%d]High_th[%u]Low_th[%u]\r\n",
        g_freq_lock_control.en_irq_affinity, g_freq_lock_control.us_throughput_irq_high,
        g_freq_lock_control.us_throughput_irq_low);
}


void hwifi_config_cpu_freq_ini_param_1103(void)
{
    int32_t l_val;

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_DMA_LATENCY);
    g_freq_lock_control.uc_lock_dma_latency = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    g_freq_lock_control.dma_latency_value = (uint16_t)l_val;
    oal_io_print("DMA latency[%d]\r\n", g_freq_lock_control.uc_lock_dma_latency);
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_LOCK_CPU_TH_HIGH);
    g_freq_lock_control.us_lock_cpu_th_high = (uint16_t)l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_LOCK_CPU_TH_LOW);
    g_freq_lock_control.us_lock_cpu_th_low = (uint16_t)l_val;
    oal_io_print("CPU freq high[%d] low[%d]\r\n", g_freq_lock_control.us_lock_cpu_th_high,
        g_freq_lock_control.us_lock_cpu_th_low);
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_AFFINITY);
    g_freq_lock_control.en_irq_affinity = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_TH_HIGH);
    g_freq_lock_control.us_throughput_irq_high = (l_val > 0) ? (uint16_t)l_val : 200;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_TH_LOW);
    g_freq_lock_control.us_throughput_irq_low = (l_val > 0) ? (uint16_t)l_val : 150;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_PPS_TH_HIGH);
    g_freq_lock_control.ul_irq_pps_high = (l_val > 0) ? (uint32_t)l_val : 25000;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_PPS_TH_LOW);
    g_freq_lock_control.ul_irq_pps_low = (l_val > 0) ? (uint32_t)l_val : 5000;
    g_freq_lock_control.en_userctl_bindcpu = OAL_FALSE; /* 用户指定绑核命令默认不开启 */
    g_freq_lock_control.uc_userctl_irqbind = 0;
    g_freq_lock_control.uc_userctl_threadbind = 0;
    oal_io_print("irq affinity enable[%d]High_th[%u]Low_th[%u]\r\n", g_freq_lock_control.en_irq_affinity,
        g_freq_lock_control.us_throughput_irq_high, g_freq_lock_control.us_throughput_irq_low);
}
OAL_STATIC void hwifi_1106_config_dmac_freq_ini_param(void)
{
    oal_uint8 uc_flag = OAL_TRUE;
    oal_uint8 uc_index;
    /******************************************** 自动调频 ********************************************/
    for (uc_index = 0; uc_index < DEV_WORK_FREQ_LVL_NUM; ++uc_index) {
        if (g_cust_cap.dev_frequency[uc_index].cpu_freq_type > FREQ_HIGHEST) {
            uc_flag = OAL_FALSE;
            break;
        }
    }

    if (uc_flag) {
        for (uc_index = 0; uc_index < DEV_WORK_FREQ_LVL_NUM; ++uc_index) {
            g_host_speed_freq_level[uc_index].ul_speed_level = g_cust_cap.dev_frequency[uc_index].speed_level;
            g_host_speed_freq_level[uc_index].ul_min_cpu_freq = g_cust_cap.dev_frequency[uc_index].min_cup_freq;
            g_host_speed_freq_level[uc_index].ul_min_ddr_freq = g_cust_cap.dev_frequency[uc_index].min_ddr_freq;
            g_device_speed_freq_level[uc_index].uc_device_type = g_cust_cap.dev_frequency[uc_index].cpu_freq_type;
        }
    }
}



OAL_STATIC void hwifi_config_dmac_freq_ini_param(void)
{
    uint32_t cfg_id;
    uint32_t ul_val;
    int32_t l_cfg_value;
    int8_t *pc_tmp;
    host_speed_freq_level_stru ast_host_speed_freq_level_tmp[4];
    device_speed_freq_level_stru ast_device_speed_freq_level_tmp[4];
    uint8_t uc_flag = OAL_FALSE;
    uint8_t uc_index;
    int32_t l_ret = EOK;

    /******************************************** 自动调频 ********************************************/
    /* config g_host_speed_freq_level */
    pc_tmp = (int8_t *)&ast_host_speed_freq_level_tmp;

    for (cfg_id = WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0; cfg_id <= WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_3; ++cfg_id) {
        ul_val = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        *(uint32_t *)pc_tmp = ul_val;
        pc_tmp += 4;
    }

    /* config g_device_speed_freq_level */
    pc_tmp = (int8_t *)&ast_device_speed_freq_level_tmp;
    for (cfg_id = WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0; cfg_id <= WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3; ++cfg_id) {
        l_cfg_value = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if (oal_value_in_valid_range(l_cfg_value, FREQ_IDLE, FREQ_HIGHEST)) {
            *pc_tmp = l_cfg_value;
            pc_tmp += 4;
        } else {
            uc_flag = OAL_TRUE;
            break;
        }
    }

    if (!uc_flag) {
        l_ret += memcpy_s(&g_host_speed_freq_level, OAL_SIZEOF(g_host_speed_freq_level),
                          &ast_host_speed_freq_level_tmp, OAL_SIZEOF(g_host_speed_freq_level));
        l_ret += memcpy_s(&g_device_speed_freq_level, OAL_SIZEOF(g_device_speed_freq_level),
                          &ast_device_speed_freq_level_tmp, OAL_SIZEOF(g_device_speed_freq_level));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_config_host_global_ini_param::memcpy fail!");
            return;
        }

        for (uc_index = 0; uc_index < 4; uc_index++) {
            oam_warning_log4(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::ul_speed_level = %d, \
                ul_min_cpu_freq = %d, ul_min_ddr_freq = %d, uc_device_type = %d}\r\n",
                g_host_speed_freq_level[uc_index].ul_speed_level,
                g_host_speed_freq_level[uc_index].ul_min_cpu_freq,
                g_host_speed_freq_level[uc_index].ul_min_ddr_freq,
                g_device_speed_freq_level[uc_index].uc_device_type);
        }
    }

}
OAL_STATIC void hwifi_1106_config_bypass_extlna_ini_param(void)
{
    g_st_rx_dyn_bypass_extlna_switch.uc_ini_en = g_cust_host.dyn_extlna.switch_en;
    g_st_rx_dyn_bypass_extlna_switch.uc_cur_status = OAL_TRUE; /* 默认低功耗场景 */
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_high = g_cust_host.dyn_extlna.throughput_high;
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_low = g_cust_host.dyn_extlna.throughput_low;
}
OAL_STATIC void hwifi_config_bypass_extlna_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA);
    g_st_rx_dyn_bypass_extlna_switch.uc_ini_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    g_st_rx_dyn_bypass_extlna_switch.uc_cur_status = OAL_TRUE; /* 默认低功耗场景 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH);
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_high = (l_val > 0) ? (uint16_t)l_val : 100;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW);
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_low = (l_val > 0) ? (uint16_t)l_val : 50;

    oal_io_print("DYN_BYPASS_EXTLNA SWITCH en[%d],high[%d],low[%d]\r\n", g_st_rx_dyn_bypass_extlna_switch.uc_ini_en,
        g_st_rx_dyn_bypass_extlna_switch.us_throughput_high, g_st_rx_dyn_bypass_extlna_switch.us_throughput_low);
}
OAL_STATIC void hwifi_1106_config_host_global_ini_param_extend(void)
{
#ifdef _PRE_WLAN_FEATURE_MBO
    g_uc_mbo_switch = g_cust_cap.mbo_switch;
#endif
    g_uc_dbac_dynamic_switch = g_cust_cap.dbac_dynamic_switch;
    g_ul_ddr_freq = g_cust_host.ddr_freq;

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (memcpy_s(&g_st_default_hiex_cap, OAL_SIZEOF(mac_hiex_cap_stru), &g_cust_cap.hiex_cap,
            OAL_SIZEOF(mac_hiex_cap_stru)) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_1106_config_host_global_ini_param_extend::hiex cap memcpy fail!");
        }
    }
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        g_mac_ftm_cap = g_cust_cap.ftm_cap;
    }
#endif
}

OAL_STATIC void hwifi_config_host_global_ini_param_extend(void)
{
    int32_t l_val;

#ifdef _PRE_WLAN_FEATURE_MBO
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MBO_SWITCH);
    g_uc_mbo_switch = !!l_val;
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYNAMIC_DBAC_SWITCH);
    g_uc_dbac_dynamic_switch = !!l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DDR_FREQ);
    g_ul_ddr_freq = (uint32_t)l_val;

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HIEX_CAP);
        if (memcpy_s(&g_st_default_hiex_cap, OAL_SIZEOF(mac_hiex_cap_stru), &l_val,
            OAL_SIZEOF(mac_hiex_cap_stru)) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_config_host_global_ini_param::hiex cap memcpy fail!");
        }
    }
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FTM_CAP);
        g_mac_ftm_cap = (uint8_t)l_val;
    }
#endif
}
void hwifi_1106_config_host_global_ini_param(void)
{
    /******************************************** 性能 ********************************************/
    hwifi_config_cpu_freq_ini_param();

    hwifi_1106_config_tcp_ack_buf_ini_param();

    hwifi_1106_config_dmac_freq_ini_param();

    hwifi_1106_config_bypass_extlna_ini_param();
    oal_io_print("DYN_BYPASS_EXTLNA SWITCH en[%d],high[%d],low[%d]\r\n", g_st_rx_dyn_bypass_extlna_switch.uc_ini_en,
        g_st_rx_dyn_bypass_extlna_switch.us_throughput_high, g_st_rx_dyn_bypass_extlna_switch.us_throughput_low);

    hwifi_1106_config_performance_ini_param();

    hwifi_1106_config_host_global_ini_param_extend();

    hwifi_1106_set_voe_custom_param();
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hwifi_1106_config_host_global_11ax_ini_param();
    }
#endif
    return;
}
void hwifi_config_factory_lte_gpio_ini_param(void)
{
    int32_t val;

    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_LTE_GPIO_CHECK_SWITCH);
    g_wlan_cust->ul_lte_gpio_check_switch = (uint32_t) !!val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_ISM_PRIORITY);
    g_wlan_cust->ul_ism_priority = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_RX);
    g_wlan_cust->ul_lte_rx = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_TX);
    g_wlan_cust->ul_lte_tx = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_INACT);
    g_wlan_cust->ul_lte_inact = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_ISM_RX_ACT);
    g_wlan_cust->ul_ism_rx_act = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_BANT_PRI);
    g_wlan_cust->ul_bant_pri = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_BANT_STATUS);
    g_wlan_cust->ul_bant_status = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_WANT_PRI);
    g_wlan_cust->ul_want_pri = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_WANT_STATUS);
    g_wlan_cust->ul_want_status = (uint32_t)val;
}

void hwifi_config_host_global_ini_param(void)
{
    wlan_chip_host_global_ini_param_init();
}



void hwifi_config_host_global_ini_param_1103(void)
{
    int32_t l_val = 0;

    /******************************************** 漫游 ********************************************/
    hwifi_config_host_roam_global_ini_param();

    /******************************************** 性能 ********************************************/
    hwifi_config_cpu_freq_ini_param();

    hwifi_config_tcp_ack_buf_ini_param();

    hwifi_config_dmac_freq_ini_param();

    hwifi_config_bypass_extlna_ini_param();

    hwifi_config_performance_ini_param();
    /******************************************** 随机MAC地址扫描 ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN);
    g_wlan_cust->uc_random_mac_addr_scan = !!l_val;

    /******************************************** CAPABILITY ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40);
    g_wlan_cust->uc_disable_capab_2ght40 = (uint8_t) !!l_val;
    /********************************************factory_lte_gpio_check ********************************************/
    hwifi_config_factory_lte_gpio_ini_param();

    hwifi_config_host_global_ini_param_extend();

    hwifi_set_voe_custom_param();
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hwifi_config_host_global_11ax_ini_param();
    }
#endif
    return;
}


OAL_STATIC void hwifi_config_init_ini_country(oal_net_device_stru *pst_cfg_net_dev)
{
    int32_t l_ret;
    l_ret = (int32_t)wal_hipriv_setcountry(pst_cfg_net_dev, hwifi_get_country_code());
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_ini_country::send_cfg_event return err code [%d]!}", l_ret);
    }

    /* 开关wifi定制化配置国家码 */
    g_cust_country_code_ignore_flag.en_country_code_ingore_hipriv_flag = OAL_FALSE;
}
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY

OAL_STATIC void hwifi_config_selfstudy_init_country(oal_net_device_stru *pst_cfg_net_dev)
{
    int32_t l_ret;
    uint8_t default_country_code[] = "99";
    uint8_t uc_val;

    uc_val = wlan_chip_get_selfstudy_country_flag();
    if (uc_val & CUS_PARAM_COUNTRYCODE_SELFSTUDY_OPEN0) {
        g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag = OAL_FALSE;
        l_ret = (int32_t)wal_hipriv_setcountry(pst_cfg_net_dev, default_country_code);
        if (oal_unlikely(l_ret != OAL_SUCC)) {
            oam_error_log1(0, OAM_SF_ANY,
                           "{hwifi_config_selfstudy_init_country::send_cfg_event return err code [%d]!}", l_ret);
        }
    } else {
        g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag = OAL_TRUE;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{hwifi_config_selfstudy_init_country::custom_value [%d]!}", uc_val);
}
#endif


OAL_STATIC void hwifi_config_init_ini_log(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    int32_t l_loglevel;

    /* log_level */
    l_loglevel = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LOGLEVEL);
    if (l_loglevel < OAM_LOG_LEVEL_ERROR ||
        l_loglevel > OAM_LOG_LEVEL_INFO) {
        oam_error_log3(0, OAM_SF_ANY, "{hwifi_config_init_ini_clock::loglevel[%d] out of range[%d,%d], check ini file}",
                       l_loglevel, OAM_LOG_LEVEL_ERROR, OAM_LOG_LEVEL_INFO);
        return;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALL_LOG_LEVEL, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_loglevel;
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                               (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_ini_log::return err code[%d]!}\r\n", l_ret);
    }
}


int8_t hwifi_check_pwr_ref_delta(int8_t c_pwr_ref_delta)
{
    int8_t c_ret = 0;
    if (c_pwr_ref_delta > WAL_HIPRIV_PWR_REF_DELTA_HI) {
        c_ret = WAL_HIPRIV_PWR_REF_DELTA_HI;
    } else if (c_pwr_ref_delta < WAL_HIPRIV_PWR_REF_DELTA_LO) {
        c_ret = WAL_HIPRIV_PWR_REF_DELTA_LO;
    } else {
        c_ret = c_pwr_ref_delta;
    }

    return c_ret;
}


int8_t hwifi_get_valid_amend_rssi_val(int8_t pwr_ref_delta)
{
    int8_t rssi_amend_val;
    rssi_amend_val = cus_val_valid(pwr_ref_delta, WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ?
        pwr_ref_delta : 0;
    return rssi_amend_val;
}


void hwifi_cfg_filter_narrow_ref_delta(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_rf_idx;
    WLAN_CFG_INIT cfg_id;
    int32_t l_pwr_ref_delta;
    mac_cfg_custom_filter_narrow_amend_rssi_stru *filter_narrow_rssi_amend;

    for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
        filter_narrow_rssi_amend = &pst_customize_rf->filter_narrow_rssi_amend[uc_rf_idx];
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C0 :
            WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_2g[0] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_2g[1] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C0 :
            WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[0] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[1] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[2] = /* rssi修正5G phy mode160M 数组下标2 */
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_third_byte(l_pwr_ref_delta));
    }
}


void hwifi_cfg_pwr_ref_delta(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_rf_idx;
    WLAN_CFG_INIT cfg_id;
    int32_t l_pwr_ref_delta;
    mac_cfg_custom_delta_pwr_ref_stru *pst_delta_pwr_ref;
    mac_cfg_custom_amend_rssi_stru *pst_rssi_amend_ref;

    for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
        pst_delta_pwr_ref = &pst_customize_rf->ast_delta_pwr_ref_cfg[uc_rf_idx];
        /* 2G 20M/40M */
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C0_MULT4 : WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C1_MULT4;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_2g[0] = hwifi_check_pwr_ref_delta((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_2g[1] = hwifi_check_pwr_ref_delta((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        /* 5G 20M/40M/80M/160M */
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C0_MULT4 : WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C1_MULT4;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_5g[0] = hwifi_check_pwr_ref_delta((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_5g[1] = hwifi_check_pwr_ref_delta((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_5g[2] = hwifi_check_pwr_ref_delta((int8_t)cus_get_third_byte(l_pwr_ref_delta));
        pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_5g[3] = hwifi_check_pwr_ref_delta((int8_t)cus_get_fourth_byte(l_pwr_ref_delta));

        /* RSSI amend */
        pst_rssi_amend_ref = &pst_customize_rf->ast_rssi_amend_cfg[uc_rf_idx];
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C0 : WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_2g[0] =
            cus_val_valid((int8_t)cus_get_first_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_first_byte(l_pwr_ref_delta) : 0;
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_2g[1] =
            cus_val_valid((int8_t)cus_get_second_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_second_byte(l_pwr_ref_delta) : 0;
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_2g[2] =
            cus_val_valid((int8_t)cus_get_third_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_third_byte(l_pwr_ref_delta) : 0;
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C0 : WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_5g[0] =
            cus_val_valid((int8_t)cus_get_first_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_first_byte(l_pwr_ref_delta) : 0;
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_5g[1] =
            cus_val_valid((int8_t)cus_get_second_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_second_byte(l_pwr_ref_delta) : 0;
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_5g[2] =
            cus_val_valid((int8_t)cus_get_third_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_third_byte(l_pwr_ref_delta) : 0;
        pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_5g[3] =
            cus_val_valid((int8_t)cus_get_fourth_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_fourth_byte(l_pwr_ref_delta) : 0;
    }
}

OAL_STATIC void hwifi_cfg_front_end_2g_rf0_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* 2g 外部fem */
    /* RF0 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_pa_isexist =
        (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_lna_isexist =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].us_lna_on2off_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].us_lna_off2on_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G));
}

OAL_STATIC void hwifi_cfg_front_end_2g_rf1_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* RF1 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_pa_isexist =
        (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_lna_isexist =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].us_lna_on2off_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].us_lna_off2on_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G));
}


OAL_STATIC OAL_INLINE uint32_t hwifi_cfg_front_end_value_range_check(mac_cfg_customize_rf *pst_customize_rf,
    int32_t l_wlan_band, int32_t l_rf_db_min)
{
    return ((pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num == 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num > MAC_EXT_PA_GAIN_MAX_LVL ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db < LNA_GAIN_DB_MIN ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db > LNA_GAIN_DB_MAX) ||
        (pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db < l_rf_db_min ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db > 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num == 0 ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num > MAC_EXT_PA_GAIN_MAX_LVL ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_lna_gain_db < LNA_GAIN_DB_MIN ||
        pst_customize_rf->ast_ext_rf[l_wlan_band][WLAN_RF_CHANNEL_ONE].c_lna_gain_db > LNA_GAIN_DB_MAX));
}


OAL_STATIC OAL_INLINE uint32_t hwifi_cfg_front_end_adjustment_range_check(int8_t c_delta_cca_ed_high_20th_2g,
    int8_t c_delta_cca_ed_high_40th_2g, int8_t c_delta_cca_ed_high_20th_5g,
    int8_t c_delta_cca_ed_high_40th_5g, int8_t c_delta_cca_ed_high_80th_5g)
{
    return (cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_20th_2g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_40th_2g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_20th_5g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_40th_5g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_80th_5g));
}

OAL_STATIC uint32_t hwifi_cfg_front_end(uint8_t *puc_param)
{
    mac_cfg_customize_rf *pst_customize_rf;
    uint8_t uc_idx; /* 结构体数组下标 */
    int32_t l_mult4;
    int8_t c_mult4_rf[2];

    pst_customize_rf = (mac_cfg_customize_rf *)puc_param;
    memset_s(pst_customize_rf, OAL_SIZEOF(mac_cfg_customize_rf), 0, OAL_SIZEOF(mac_cfg_customize_rf));

    /* 配置: 2g rf */
    for (uc_idx = 0; uc_idx < MAC_NUM_2G_BAND; ++uc_idx) {
        /* 获取各2p4g 各band 0.25db及0.1db精度的线损值 */
        l_mult4 = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_START + uc_idx);
        /* rf0 */
        c_mult4_rf[0] = (int8_t)cus_get_first_byte(l_mult4);
        /* rf1 */
        c_mult4_rf[1] = (int8_t)cus_get_second_byte(l_mult4);
        if (cus_val_valid(c_mult4_rf[0], RF_LINE_TXRX_GAIN_DB_MAX, RF_LINE_TXRX_GAIN_DB_2G_MIN) &&
            cus_val_valid(c_mult4_rf[1], RF_LINE_TXRX_GAIN_DB_MAX, RF_LINE_TXRX_GAIN_DB_2G_MIN)) {
            pst_customize_rf->ast_rf_gain_db_rf[0].ac_gain_db_2g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[0];
            pst_customize_rf->ast_rf_gain_db_rf[1].ac_gain_db_2g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[1];
        }
        else {
            /* 值超出有效范围 */
            oam_error_log2(0, OAM_SF_CFG, "{hwifi_cfg_front_end::ini_id[%d]value out of range, 2g mult4[0x%0x}!}",
                           WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_START + uc_idx, l_mult4);
            return OAL_FAIL;
        }
    }

    hwifi_cfg_pwr_ref_delta(pst_customize_rf);
    hwifi_cfg_filter_narrow_ref_delta(pst_customize_rf);

    /* 通道radio cap */
    pst_customize_rf->uc_chn_radio_cap = (uint8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CHANN_RADIO_CAP);

    /* 2g 外部fem */
    /* RF0 */
    hwifi_cfg_front_end_2g_rf0_fem(pst_customize_rf);

    /* RF1 */
    hwifi_cfg_front_end_2g_rf1_fem(pst_customize_rf);

    if (hwifi_cfg_front_end_value_range_check(pst_customize_rf, WLAN_BAND_2G, RF_LINE_TXRX_GAIN_DB_2G_MIN)) {
        /* 值超出有效范围 */
        oam_error_log4(0, OAM_SF_CFG,
            "{hwifi_cfg_front_end::2g gain db out of range! rf0 lna_bypass[%d] pa_b0[%d] lna gain[%d] pa_b1[%d]}",
            pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db,
            pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db,
            pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db,
            pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db);

        return OAL_FAIL;
    }

    /* 2g定制化RF部分PA偏置寄存器  */
    for (uc_idx = 0; uc_idx < CUS_RF_PA_BIAS_REG_NUM; uc_idx++) {
        pst_customize_rf->aul_2g_pa_bias_rf_reg[uc_idx] =
        (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_TX2G_PA_GATE_VCTL_REG236 + uc_idx);
    }

    if (OAL_TRUE == mac_device_check_5g_enable_per_chip()) {
        /* 配置: 5g rf */
        /* 配置: fem口到天线口的负增益 */
        for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; ++uc_idx) {
            /* 获取各5g 各band 0.25db及0.1db精度的线损值 */
            l_mult4 = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_START + uc_idx);
            c_mult4_rf[0] = (int8_t)cus_get_first_byte(l_mult4);
            c_mult4_rf[1] = (int8_t)cus_get_second_byte(l_mult4);
            if (c_mult4_rf[0] <= RF_LINE_TXRX_GAIN_DB_MAX && c_mult4_rf[1] <= RF_LINE_TXRX_GAIN_DB_MAX) {
                pst_customize_rf->ast_rf_gain_db_rf[0].ac_gain_db_5g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[0];
                pst_customize_rf->ast_rf_gain_db_rf[1].ac_gain_db_5g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[1];
            }
            else {
                /* 值超出有效范围 */
                oam_error_log2(0, OAM_SF_CFG, "{hwifi_cfg_front_end::ini_id[%d]value out of range, 5g mult4[0x%0x}}",
                               WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_START + uc_idx, l_mult4);
                return OAL_FAIL;
            }
        }

        /* 5g 外部fem */
        /* RF0 */
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db = (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db = (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db = (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db = (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num = (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_switch_isexist = (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_pa_isexist = (uint8_t) !!(cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_PA_ISEXIST_5G_MASK);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].en_fem_lp_enable = (oal_fem_lp_state_enum_uint8)((cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_LP_STATUS_MASK) >> EXT_FEM_LP_STATUS_OFFSET);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_fem_spec_value = (int8_t)((cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_FEM_SPEC_MASK) >> EXT_FEM_FEM_SPEC_OFFSET);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_lna_isexist = (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].us_lna_on2off_time_ns = (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].us_lna_off2on_time_ns = (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G));
        /* RF1 */
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db = (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db = (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db = (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db = (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num = (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_switch_isexist = (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_pa_isexist = (uint8_t) !!(cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_PA_ISEXIST_5G_MASK);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].en_fem_lp_enable = (oal_fem_lp_state_enum_uint8)((cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_LP_STATUS_MASK) >> EXT_FEM_LP_STATUS_OFFSET);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_fem_spec_value = (int8_t)((cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_FEM_SPEC_MASK) >> EXT_FEM_FEM_SPEC_OFFSET);
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_lna_isexist = (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].us_lna_on2off_time_ns = (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G));
        pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].us_lna_off2on_time_ns = (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G));

        /* 5g upc mix_bf_gain_ctl for P10 */
        for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; uc_idx++) {
            pst_customize_rf->aul_5g_upc_mix_gain_rf_reg[uc_idx] = (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_TX5G_UPC_MIX_GAIN_CTRL_1 + uc_idx);
        }

        if (hwifi_cfg_front_end_value_range_check(pst_customize_rf, WLAN_BAND_5G, RF_LINE_TXRX_GAIN_DB_5G_MIN)) {
            /* 值超出有效范围 */
            oam_error_log4(0, OAM_SF_CFG,
                           "{hwifi_cfg_front_end::2g gain db out of range! rf0 lna_bypass[%d] pa_b0[%d] lna gain[%d] pa_b1[%d]}",
                           pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db,
                           pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db,
                           pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db,
                           pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db);

            return OAL_FAIL;
        }
    }

    pst_customize_rf->uc_far_dist_pow_gain_switch = (uint8_t) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH);
    pst_customize_rf->uc_far_dist_dsss_scale_promote_switch = (uint8_t) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH);

    /* 配置: cca能量门限调整值 */
    {
        int8_t c_delta_cca_ed_high_20th_2g = (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G);
        int8_t c_delta_cca_ed_high_40th_2g = (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G);
        int8_t c_delta_cca_ed_high_20th_5g = (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G);
        int8_t c_delta_cca_ed_high_40th_5g = (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G);
        int8_t c_delta_cca_ed_high_80th_5g = (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_80TH_5G);
        /* 检查每一项的调整幅度是否超出最大限制 */
        if (hwifi_cfg_front_end_adjustment_range_check(c_delta_cca_ed_high_20th_2g, c_delta_cca_ed_high_40th_2g,
                                                       c_delta_cca_ed_high_20th_5g, c_delta_cca_ed_high_40th_5g,
                                                       c_delta_cca_ed_high_80th_5g)) {
            oam_error_log4(0, OAM_SF_ANY, "{hwifi_cfg_front_end::one or more delta cca ed high threshold out of range \
                [delta_20th_2g=%d, delta_40th_2g=%d, delta_20th_5g=%d, delta_40th_5g=%d], please check the value!}",
                c_delta_cca_ed_high_20th_2g, c_delta_cca_ed_high_40th_2g,
                c_delta_cca_ed_high_20th_5g, c_delta_cca_ed_high_40th_5g);
            /* set 0 */
            pst_customize_rf->c_delta_cca_ed_high_20th_2g = 0;
            pst_customize_rf->c_delta_cca_ed_high_40th_2g = 0;
            pst_customize_rf->c_delta_cca_ed_high_20th_5g = 0;
            pst_customize_rf->c_delta_cca_ed_high_40th_5g = 0;
            pst_customize_rf->c_delta_cca_ed_high_80th_5g = 0;
        } else {
            pst_customize_rf->c_delta_cca_ed_high_20th_2g = c_delta_cca_ed_high_20th_2g;
            pst_customize_rf->c_delta_cca_ed_high_40th_2g = c_delta_cca_ed_high_40th_2g;
            pst_customize_rf->c_delta_cca_ed_high_20th_5g = c_delta_cca_ed_high_20th_5g;
            pst_customize_rf->c_delta_cca_ed_high_40th_5g = c_delta_cca_ed_high_40th_5g;
            pst_customize_rf->c_delta_cca_ed_high_80th_5g = c_delta_cca_ed_high_80th_5g;
        }
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI

OAL_STATIC uint32_t hwifi_cfg_init_cus_dyn_cali(mac_cus_dy_cali_param_stru *puc_dyn_cali_param)
{
    int32_t  l_val;
    uint8_t  uc_idx = 0;
    uint8_t  uc_rf_idx, uc_dy_cal_param_idx;
    uint8_t  uc_cfg_id = WLAN_CFG_DTS_2G_CORE0_DPN_CH1;
    uint8_t  uc_dpn_2g_nv_id = WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0;
    uint8_t  uc_dpn_5g_nv_id = WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C0;
    int8_t   ac_dpn_nv[HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_CW][MAC_2G_CHANNEL_NUM];
    int8_t   ac_dpn_5g_nv[OAL_5G_160M_CHANNEL_NUM];
    uint8_t  uc_num_idx;
    uint8_t *puc_cust_nvram_info;
    uint8_t *pc_end = ";";
    uint8_t *pc_sep = ",";
    int8_t  *pc_ctx;
    int8_t  *pc_token;
    uint8_t  auc_nv_pa_params[CUS_PARAMS_LEN_MAX] = { 0 };
    int32_t  l_ret;

    for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
        puc_dyn_cali_param->uc_rf_id = uc_rf_idx;

        /* 动态校准二次项系数入参检查 */
        for (uc_dy_cal_param_idx = 0; uc_dy_cal_param_idx < DY_CALI_PARAMS_NUM; uc_dy_cal_param_idx++) {
            if (!g_pro_line_params[uc_rf_idx][uc_dy_cal_param_idx].l_pow_par2) {
                oam_error_log1(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::unexpect val[%d] s_pow_par2[0]!}",
                    uc_dy_cal_param_idx);
                return OAL_FAIL;
            }
        }
        l_ret = memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_params,
                         OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_params),
                         g_pro_line_params[uc_rf_idx], OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_params));

        l_ret += memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params,
                          OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params),
                          &g_pro_line_params[uc_rf_idx][CUS_DY_CALI_PARAMS_NUM],
                          OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params));

        l_ret += memcpy_s(puc_dyn_cali_param->as_extre_point_val, OAL_SIZEOF(puc_dyn_cali_param->as_extre_point_val),
                          g_gs_extre_point_vals[uc_rf_idx], OAL_SIZEOF(puc_dyn_cali_param->as_extre_point_val));

        /* DPN */
        for (uc_idx = 0; uc_idx < MAC_2G_CHANNEL_NUM; uc_idx++) {
            l_val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
            l_ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_2g_dpn_params[uc_idx],
                              CUS_DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(int8_t),
                              &l_val, CUS_DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(int8_t));
        }
        uc_cfg_id += MAC_2G_CHANNEL_NUM;

        for (uc_idx = HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_11B;
            uc_idx <= HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_OFDM40; uc_idx++) {
            /* 获取产线计算DPN值修正 */
            puc_cust_nvram_info = hwifi_get_nvram_param(uc_dpn_2g_nv_id);
            uc_dpn_2g_nv_id++;

            if (0 == OAL_STRLEN(puc_cust_nvram_info)) {
                continue;
            }

            memset_s(auc_nv_pa_params, OAL_SIZEOF(auc_nv_pa_params), 0, OAL_SIZEOF(auc_nv_pa_params));
            l_ret += memcpy_s(auc_nv_pa_params, OAL_SIZEOF(auc_nv_pa_params),
                              puc_cust_nvram_info, OAL_STRLEN(puc_cust_nvram_info));
            pc_token = oal_strtok(auc_nv_pa_params, pc_end, &pc_ctx);
            pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
            uc_num_idx = 0;
            while ((pc_token != OAL_PTR_NULL)) {
                if (uc_num_idx >= MAC_2G_CHANNEL_NUM) {
                    uc_num_idx++;
                    break;
                }
                l_val = oal_strtol(pc_token, OAL_PTR_NULL, 10) / 10;
                pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
                if (oal_value_not_in_valid_range(l_val, CUS_DY_CALI_2G_VAL_DPN_MIN, CUS_DY_CALI_2G_VAL_DPN_MAX)) {
                    oam_error_log3(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn val[%d]\
                        unexpected idx[%d] num_idx[%d}!}", l_val, uc_idx, uc_num_idx);
                    l_val = 0;
                }
                ac_dpn_nv[uc_idx][uc_num_idx] = (int8_t)l_val;
                uc_num_idx++;
            }

            if (uc_num_idx != MAC_2G_CHANNEL_NUM) {
                oam_error_log2(0, OAM_SF_CUSTOM,
                    "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn num is unexpect uc_id[%d] rf[%d}}", uc_idx, uc_rf_idx);
                continue;
            }

            for (uc_num_idx = 0; uc_num_idx < MAC_2G_CHANNEL_NUM; uc_num_idx++) {
                puc_dyn_cali_param->ac_dy_cali_2g_dpn_params[uc_num_idx][uc_idx] += ac_dpn_nv[uc_idx][uc_num_idx];
            }
        }

        for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; uc_idx++) {
            l_val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
            l_ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_5g_dpn_params[uc_idx],
                              CUS_DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(int8_t),
                              &l_val, CUS_DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(int8_t));
        }
        uc_cfg_id += MAC_NUM_5G_BAND;

        /* 5G 160M DPN */
        puc_cust_nvram_info = hwifi_get_nvram_param(uc_dpn_5g_nv_id);
        uc_dpn_5g_nv_id++;
        if (OAL_STRLEN(puc_cust_nvram_info)) {
            memset_s(auc_nv_pa_params, OAL_SIZEOF(auc_nv_pa_params), 0, OAL_SIZEOF(auc_nv_pa_params));
            l_ret += memcpy_s(auc_nv_pa_params, OAL_SIZEOF(auc_nv_pa_params),
                              puc_cust_nvram_info, OAL_STRLEN(puc_cust_nvram_info));
            pc_token = oal_strtok(auc_nv_pa_params, pc_end, &pc_ctx);
            pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
            uc_num_idx = 0;
            while ((pc_token != OAL_PTR_NULL)) {
                if (uc_num_idx >= OAL_5G_160M_CHANNEL_NUM) {
                    uc_num_idx++;
                    break;
                }
                l_val = oal_strtol(pc_token, OAL_PTR_NULL, 10) / 10;
                pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
                if (oal_value_not_in_valid_range(l_val, CUS_DY_CALI_5G_VAL_DPN_MIN, CUS_DY_CALI_5G_VAL_DPN_MAX)) {
                    oam_error_log3(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn val[%d]\
                        unexpect:idx[%d] num_idx[%d}}", l_val, uc_idx, uc_num_idx);
                    l_val = 0;
                }
                ac_dpn_5g_nv[uc_num_idx] = (int8_t)l_val;
                uc_num_idx++;
            }

            if (uc_num_idx != OAL_5G_160M_CHANNEL_NUM) {
                oam_error_log2(0, OAM_SF_CUSTOM,
                    "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn num unexpected id[%d] rf[%d}}", uc_idx, uc_rf_idx);
                continue;
            }
            /* 5250  5570 */
            for (uc_num_idx = 0; uc_num_idx < OAL_5G_160M_CHANNEL_NUM; uc_num_idx++) {
                puc_dyn_cali_param->ac_dy_cali_5g_dpn_params[uc_num_idx + 1][WLAN_BW_CAP_160M] +=
                    ac_dpn_5g_nv[uc_num_idx];
            }
        }

        puc_dyn_cali_param++;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_cfg_init_cus_dyn_cali::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC void hwifi_config_init_ini_rf(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    uint16_t us_event_len = OAL_SIZEOF(mac_cfg_customize_rf);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_RF, us_event_len);

    /*lint -e774*/
    /* 定制化下发不能超过事件内存长 */
    if (us_event_len > WAL_MSG_WRITE_MAX_LEN) {
        oam_error_log2(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::event size[%d] larger than msg size[%d]!}",
                       us_event_len, WAL_MSG_WRITE_MAX_LEN);
        return;
    }
    /*lint +e774*/
    /*  */
    ul_ret = hwifi_cfg_front_end(st_write_msg.auc_value);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::front end rf wrong value, not send cfg!}");
        return;
    }

    /* 如果所有参数都在有效范围内，则下发配置值 */
    ul_ret = (uint32_t)wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + \
        us_event_len, (uint8_t *)&st_write_msg,  OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::EVENT[wal_send_cfg_event] fail[%d]!}", ul_ret);
    }
}

OAL_STATIC uint32_t hwifi_custom_data_send(oal_net_device_stru *cfg_net_dev,
    uint8_t *cust_param, int32_t param_len, uint16_t cfg_id)
{
    oal_uint32 ret = OAL_SUCC;
#ifndef _PRE_PRODUCT_HI1620S_KUNPENG
    uint16 data_len;
    int32 remain_len = param_len;
    uint8_t *data = cust_param;
    mac_vap_stru *mac_vap = OAL_PTR_NULL;

    mac_vap = oal_net_dev_priv(cfg_net_dev);
    while (remain_len > 0) {
        data_len = oal_min(remain_len, WAL_CUST_DATA_SEND_LEN);

        /* 如果所有参数都在有效范围内，则下发配置值 */
        ret += wal_send_custom_data(mac_vap, data_len, data, cfg_id);
        if (oal_unlikely(ret != OAL_SUCC)) {
            oam_error_log2(0, OAM_SF_ANY, "{hwifi_custom_data_send::cutsom data cfg id[id] fail[%d]!}", cfg_id, ret);
        }
        data += data_len;
        remain_len -= data_len;
    }
#endif
    return ret;
}

/*
 * 函 数 名  : hwifi_cust_rf_front_data_send
 * 功能描述  : hw 2g 5g 前端定制化
 */
OAL_STATIC uint32_t hwifi_cust_rf_front_data_send(oal_net_device_stru *cfg_net_dev)
{
    return hwifi_custom_data_send(cfg_net_dev, (uint8_t *)(&g_cust_rf_front),
        sizeof(g_cust_rf_front), WLAN_CFGID_SET_CUS_RF_CFG);
}

/*
 * 函 数 名  : hwifi_cust_rf_front_data_send
 * 功能描述  : hw 2g 5g 前端定制化
 */
OAL_STATIC uint32_t hwifi_cust_rf_cali_data_send(oal_net_device_stru *cfg_net_dev)
{
    g_cust_rf_cali.band_5g_enable = mac_device_check_5g_enable_per_chip();
    return hwifi_custom_data_send(cfg_net_dev, (uint8_t *)(&g_cust_rf_cali),
        sizeof(g_cust_rf_cali), WLAN_CFGID_SET_CUS_RF_CALI);
}
OAL_STATIC uint32_t hwifi_cust_nv_pow_data_send(oal_net_device_stru *cfg_net_dev)
{
    return hwifi_custom_data_send(cfg_net_dev, (uint8_t *)(&g_cust_pow),
        sizeof(g_cust_pow), WLAN_CFGID_SET_CUS_POW);
}

OAL_STATIC uint32_t hwifi_cust_nv_dyn_pow_data_send(oal_net_device_stru *cfg_net_dev)
{
    return hwifi_custom_data_send(cfg_net_dev, (uint8_t *)(&g_cust_dyn_pow),
        sizeof(g_cust_dyn_pow), WLAN_CFGID_SET_CUS_DYN_POW_CALI);
}


extern oal_bool_enum_uint8 g_en_fact_cali_completed;

OAL_STATIC uint32_t hwifi_cfg_init_dts_cus_cali(uint8_t *puc_param, uint8_t uc_5g_Band_enable)
{
    int32_t l_val;
    int16_t s_ref_val_ch1;
    int16_t s_ref_val_ch0;
    uint8_t uc_idx; /* 结构体数组下标 */
    mac_cus_dts_cali_stru *pst_cus_cali;
    uint8_t uc_gm_opt;

    pst_cus_cali = (mac_cus_dts_cali_stru *)puc_param;
    /** 配置: TXPWR_PA_DC_REF **/
    /* 2G REF: 分13个信道 */
    for (uc_idx = 0; uc_idx < 13; uc_idx++) {
        l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_START + uc_idx);
        s_ref_val_ch1 = (int16_t)cus_get_high_16bits(l_val);
        s_ref_val_ch0 = (int16_t)cus_get_low_16bits(l_val);

        /* 2G判断参考值先不判断<0, 待与RF同事确认, TBD */
        if (s_ref_val_ch0 <= CALI_TXPWR_PA_DC_REF_MAX) {
            pst_cus_cali->ast_cali[0].aus_cali_txpwr_pa_dc_ref_2g_val_chan[uc_idx] = s_ref_val_ch0;
        } else {
            /* 值超出有效范围 */
            oam_error_log2(0, OAM_SF_ANY, "{hwifi_cfg_init_dts_cus_cali::dts 2g ref id[%d]value[%d] out of range!}",
                           WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_START + uc_idx, s_ref_val_ch0);  //lint !e571
            return OAL_FAIL;
        }
        /* 02不需要适配双通道 */
        if (s_ref_val_ch1 <= CALI_TXPWR_PA_DC_REF_MAX) {
            pst_cus_cali->ast_cali[1].aus_cali_txpwr_pa_dc_ref_2g_val_chan[uc_idx] = s_ref_val_ch1;
        } else {
            /* 值超出有效范围 */
            oam_error_log2(0, OAM_SF_ANY, "{hwifi_cfg_init_dts_cus_cali::dts ch1 2g ref id[%d]value[%d] invalid!}",
                           WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_START + uc_idx, s_ref_val_ch1);  //lint !e571
            return OAL_FAIL;
        }

    }

    /* 5G REF: 分7个band */
    if (uc_5g_Band_enable) {
        for (uc_idx = 0; uc_idx < 7; ++uc_idx) {
            l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_START + uc_idx);
            s_ref_val_ch1 = (int16_t)cus_get_high_16bits(l_val);
            s_ref_val_ch0 = (int16_t)cus_get_low_16bits(l_val);

            if (s_ref_val_ch0 >= 0 && s_ref_val_ch0 <= CALI_TXPWR_PA_DC_REF_MAX) {
                pst_cus_cali->ast_cali[0].aus_cali_txpwr_pa_dc_ref_5g_val_band[uc_idx] = s_ref_val_ch0;
            } else {
                /* 值超出有效范围 */
                oam_error_log2(0, OAM_SF_ANY, "{hwifi_cfg_init_dts_cus_cali::dts 5g ref id[%d]val[%d] invalid}",
                               WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_START + uc_idx, s_ref_val_ch0);  //lint !e571
                return OAL_FAIL;
            }
            if (s_ref_val_ch1 >= 0 && s_ref_val_ch1 <= CALI_TXPWR_PA_DC_REF_MAX) {
                pst_cus_cali->ast_cali[1].aus_cali_txpwr_pa_dc_ref_5g_val_band[uc_idx] = s_ref_val_ch1;
            } else {
                /* 值超出有效范围 */
                oam_error_log2(0, OAM_SF_ANY, "{hwifi_cfg_init_dts_cus_cali::dts ch1 5g ref id[%d]val[%d] invalid!}",
                               WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_START + uc_idx, s_ref_val_ch1);  //lint !e571
                return OAL_FAIL;
            }
        }
    }

    /* 配置BAND 5G ENABLE */
    pst_cus_cali->uc_band_5g_enable = !!uc_5g_Band_enable;

    /* 配置单音幅度档位 */
    pst_cus_cali->uc_tone_amp_grade =
        (uint8_t)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TONE_AMP_GRADE);

    /* 配置DPD校准参数 */
#ifdef _PRE_WLAN_ONLINE_DPD
    for (uc_idx = 0; uc_idx < MAC_DPD_CALI_CUS_PARAMS_NUM; uc_idx++) {
        /* 通道0 */
        l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DPD_CALI_START + uc_idx);
        pst_cus_cali->ast_dpd_cali_para[0].aul_dpd_cali_cus_dts[uc_idx] = l_val;
        /* 通道1 */
        l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DPD_CALI_START +
            uc_idx + MAC_DPD_CALI_CUS_PARAMS_NUM);
        pst_cus_cali->ast_dpd_cali_para[1].aul_dpd_cali_cus_dts[uc_idx] = l_val;
    }
#endif

    /* 配置动态校准参数 */
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL);
    pst_cus_cali->aus_dyn_cali_dscr_interval[WLAN_BAND_2G] = (uint16_t)((uint32_t)l_val & 0x0000FFFF);

    if (uc_5g_Band_enable) {
        pst_cus_cali->aus_dyn_cali_dscr_interval[WLAN_BAND_5G] = (uint16_t)(((uint32_t)l_val & 0xFFFF0000) >> 16);
    }

    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_OPT_SWITCH);
    uc_gm_opt = ((uint32_t)l_val & BIT2) >> NUM_1_BITS;

    if (((uint32_t)l_val & 0x3) >> 1) {
        /* 自适应选择 */
        l_val = !g_en_fact_cali_completed;
    } else {
        l_val = (int32_t)((uint32_t)l_val & BIT0);
    }

    pst_cus_cali->en_dyn_cali_opt_switch = (uint32_t)l_val | uc_gm_opt;

    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_GM0_DB10_AMEND);
    pst_cus_cali->as_gm0_dB10_amend[WLAN_RF_CHANNEL_ZERO] = (int16_t)cus_get_low_16bits(l_val);
    pst_cus_cali->as_gm0_dB10_amend[WLAN_RF_CHANNEL_ONE] = (int16_t)cus_get_high_16bits(l_val);
#endif  // #ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI

    return OAL_SUCC;
}


OAL_STATIC uint32_t hwifi_config_init_dts_cali(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    mac_cus_dts_cali_stru st_cus_cali;
    uint32_t ul_offset = 0;
    oal_bool_enum en_5g_Band_enable; /* mac device是否支持5g能力 */

    if (oal_warn_on(pst_cfg_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hwifi_config_init_dts_cali::pst_cfg_net_dev is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 检查硬件是否需要使能5g */
    en_5g_Band_enable = mac_device_check_5g_enable_per_chip();

    /* 配置校准参数TXPWR_PA_DC_REF */
    ul_ret = hwifi_cfg_init_dts_cus_cali((uint8_t *)&st_cus_cali, en_5g_Band_enable);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_config_init_dts_cali::init dts cus cali failed ret[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 如果所有参数都在有效范围内，则下发配置值 */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_cus_dts_cali_stru),
                        (int8_t *)&st_cus_cali, OAL_SIZEOF(mac_cus_dts_cali_stru))) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_config_init_dts_cali::memcpy fail!");
        return OAL_FAIL;
    }
    ul_offset += OAL_SIZEOF(mac_cus_dts_cali_stru);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_DTS_CALI, ul_offset);
    ul_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + ul_offset,
        (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali::wal_send_cfg_event failed, ret[%d]!}", ul_ret);
        return ul_ret;
    }

    oam_warning_log0(0, OAM_SF_CFG, "{hwifi_config_init_dts_cali::wal_send_cfg_event send succ}");

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI

OAL_STATIC void hwifi_config_init_cus_dyn_cali(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    uint32_t ul_offset = 0;
    mac_cus_dy_cali_param_stru st_dy_cus_cali[WLAN_RF_CHANNEL_NUMS];
    uint8_t uc_rf_id;
    mac_cus_dy_cali_param_stru *pst_dy_cus_cali;
    wal_msg_stru *pst_rsp_msg;
    wal_msg_write_rsp_stru *pst_write_rsp_msg = OAL_PTR_NULL;
    int32_t l_ret;

    if (oal_warn_on(pst_cfg_net_dev == OAL_PTR_NULL)) {
        return;
    }

    /* 配置动态校准参数TXPWR_PA_DC_REF */
    memset_s(st_dy_cus_cali, OAL_SIZEOF(mac_cus_dy_cali_param_stru) * WLAN_RF_CHANNEL_NUMS,
             0, OAL_SIZEOF(mac_cus_dy_cali_param_stru) * WLAN_RF_CHANNEL_NUMS);

    ul_ret = hwifi_cfg_init_cus_dyn_cali(st_dy_cus_cali);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        return;
    }

    for (uc_rf_id = 0; uc_rf_id < WLAN_RF_CHANNEL_NUMS; uc_rf_id++) {
        pst_dy_cus_cali = &st_dy_cus_cali[uc_rf_id];
        pst_rsp_msg = OAL_PTR_NULL;

        /* 如果所有参数都在有效范围内，则下发配置值 */
        l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_cus_dy_cali_param_stru),
                         (int8_t *)pst_dy_cus_cali, OAL_SIZEOF(mac_cus_dy_cali_param_stru));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_CFG, "hwifi_config_init_cus_dyn_cali::memcpy fail!");
            return;
        }

        ul_offset = OAL_SIZEOF(mac_cus_dy_cali_param_stru);
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_DYN_CALI_PARAM, ul_offset);

        ul_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + ul_offset, (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            return;
        }

        if (pst_rsp_msg != OAL_PTR_NULL) {
            pst_write_rsp_msg = (wal_msg_write_rsp_stru *)(pst_rsp_msg->auc_msg_data);
            if (pst_write_rsp_msg->ul_err_code != OAL_SUCC) {
                oam_error_log2(0, OAM_SF_SCAN, "{wal_check_and_release_msg_resp::detect err code:[%u],wid:[%u]}",
                               pst_write_rsp_msg->ul_err_code, pst_write_rsp_msg->en_wid);
                oal_free(pst_rsp_msg);
                return;
            }

            oal_free(pst_rsp_msg);
        }
    }

    return;
}

uint32_t hwifi_config_init_nvram_main(oal_net_device_stru *cfg_net_dev)
{
    return wlan_chip_init_nvram_main(cfg_net_dev);
}

uint32_t hwifi_config_init_nvram_main_1106(oal_net_device_stru *pst_cfg_net_dev)
{
    hwifi_cust_nv_pow_data_send(pst_cfg_net_dev);
    return OAL_SUCC;
}


uint32_t hwifi_config_init_nvram_main_1103(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_offset = OAL_SIZEOF(wlan_cust_nvram_params); /* 包括4个基准功率 */

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_NVRAM_PARAM, us_offset);
    l_ret = memcpy_s(st_write_msg.auc_value, us_offset, hwifi_get_nvram_params(), us_offset);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_nvram_main::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + us_offset,
        (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_nvram_main::err [%d]!}", l_ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
#endif
OAL_STATIC void hwifi_1106_config_init_ini_main(oal_net_device_stru *cfg_net_dev)
{
    hwifi_cust_nv_pow_data_send(cfg_net_dev);

    /* 国家码 */
    hwifi_config_init_ini_country(cfg_net_dev);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    /* 自学习国家码初始化 */
    hwifi_config_selfstudy_init_country(cfg_net_dev);
#endif
    hwifi_cust_rf_front_data_send(cfg_net_dev);
    hwifi_cust_nv_dyn_pow_data_send(cfg_net_dev);
}

OAL_STATIC void hwifi_config_init_ini_main(oal_net_device_stru *pst_cfg_net_dev)
{
    /* 国家码 */
    hwifi_config_init_ini_country(pst_cfg_net_dev);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    /* 自学习国家码初始化 */
    hwifi_config_selfstudy_init_country(pst_cfg_net_dev);
#endif
    /* 可维可测 */
    hwifi_config_init_ini_log(pst_cfg_net_dev);
    /* RF */
    hwifi_config_init_ini_rf(pst_cfg_net_dev);
}

uint32_t hwifi_config_init_dts_main(oal_net_device_stru *cfg_net_dev)
{
    uint32_t ul_ret = OAL_SUCC;

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    /* 下发动态校准参数 */
    hwifi_config_init_cus_dyn_cali(cfg_net_dev);
#endif

    /* 校准 */
    if (OAL_SUCC != hwifi_config_init_dts_cali(cfg_net_dev)) {
        return OAL_FAIL;
    }
    /* 校准放到第一个进行 */
    return ul_ret;
}


OAL_STATIC int32_t hwifi_config_init_ini_wlan(oal_net_device_stru *pst_net_dev)
{
    return OAL_SUCC;
}


OAL_STATIC int32_t hwifi_config_init_ini_p2p(oal_net_device_stru *pst_net_dev)
{
    return OAL_SUCC;
}


int32_t hwifi_config_init_ini(oal_net_device_stru *pst_net_dev)
{
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    int8_t ac_wlan_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    int8_t ac_p2p_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];

    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::net_dev null!}");
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::mac_vap null}");
        return -OAL_EINVAL;
    }

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::wdev null!}");
        return -OAL_EFAUL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::wiphy_priv null!}");
        return -OAL_EFAUL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::mac_device null!}");
        return -OAL_EFAUL;
    }

    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, 0, "{hwifi_config_init_ini::cfg_hmac_vap[%d] null}", pst_mac_device->uc_cfg_vap_id);
        return -OAL_EFAUL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;

    if (pst_cfg_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_cfg_net_dev is null!}");
        return -OAL_EFAUL;
    }

    snprintf_s(ac_wlan_netdev_name, NET_DEV_NAME_LEN, NET_DEV_NAME_LEN - 1, "wlan%d", pst_mac_device->uc_device_id);
    snprintf_s(ac_p2p_netdev_name, NET_DEV_NAME_LEN, NET_DEV_NAME_LEN - 1, "p2p%d", pst_mac_device->uc_device_id);

    if ((pst_wdev->iftype == NL80211_IFTYPE_STATION) ||
        (pst_wdev->iftype == NL80211_IFTYPE_P2P_DEVICE) || (pst_wdev->iftype == NL80211_IFTYPE_AP))

    {
        if (!g_uc_cfg_once_flag) {
            hwifi_config_init_nvram_main(pst_cfg_net_dev);
            hwifi_config_init_ini_main(pst_cfg_net_dev);
            g_uc_cfg_once_flag = OAL_TRUE;
        }
        if (0 == (oal_strcmp(ac_wlan_netdev_name, pst_net_dev->name))) {
            hwifi_config_init_ini_wlan(pst_net_dev);
        }
        else if (0 == (oal_strcmp(ac_p2p_netdev_name, pst_net_dev->name))) {
            hwifi_config_init_ini_p2p(pst_net_dev);
        }
        else {
            oam_warning_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini::net_dev is not wlan0 or p2p0!}");
            return OAL_SUCC;
        }
    }

    return OAL_SUCC;
}

void hwifi_config_init_force(void)
{
    /* 重新上电时置为FALSE */
    g_uc_cfg_once_flag = OAL_FALSE;

    hwifi_config_host_global_ini_param();
}

#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
uint32_t wal_1106_custom_cali(void)
{
    oal_net_device_stru *net_dev;
    uint32_t ret;

    net_dev = wal_config_get_netdev("Hisilicon0", OAL_STRLEN("Hisilicon0"));  // 通过cfg vap0来下c0 c1校准
    if (net_dev == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    } else {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(net_dev);
        oam_warning_log0(0, OAM_SF_ANY, "{wal_1106_custom_cali_etc::the net_device is already exist!}");
    }

    hwifi_1106_config_init_ini_main(net_dev);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_custom_cali_done == OAL_TRUE) {
        /* 校准数据下发 */
        wal_send_cali_data(net_dev);
    } else {
        g_custom_cali_done = OAL_TRUE;
    }

    wal_send_cali_matrix_data(net_dev);
#endif

    /* 下发参数 */
    ret = hwifi_cust_rf_cali_data_send(net_dev);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_1106_custom_cali:init_dts_main fail!}");
    }

    return ret;
}


uint32_t wal_custom_cali(void)
{
    return wlan_chip_custom_cali();
}


uint32_t wal_custom_cali_1103(void)
{
    oal_net_device_stru *pst_net_dev;
    uint32_t ul_ret;

    pst_net_dev = wal_config_get_netdev("Hisilicon0", OAL_STRLEN("Hisilicon0"));  // 通过cfg vap0来下c0 c1校准
    if (oal_warn_on(pst_net_dev == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    } else {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(pst_net_dev);
        oam_warning_log0(0, OAM_SF_ANY, "{wal_custom_cali::the net_device is already exist!}");
    }

    if (hwifi_config_init_nvram_main(pst_net_dev)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_custom_cali::init_nvram fail!}");
    }

    hwifi_config_init_ini_main(pst_net_dev);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_custom_cali_done == OAL_TRUE) {
        /* 校准数据下发 */
        wal_send_cali_data(pst_net_dev);
    } else {
        g_custom_cali_done = OAL_TRUE;
    }

    wal_send_cali_matrix_data(pst_net_dev);
#endif

    /* 下发参数 */
    ul_ret = hwifi_config_init_dts_main(pst_net_dev);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_custom_cali:init_dts_main fail!}");
    }

    return ul_ret;
}


int32_t wal_set_custom_process_func(void)
{
    struct custom_process_func_handler *pst_custom_process_func_handler;

    pst_custom_process_func_handler = oal_get_custom_process_func();
    if (pst_custom_process_func_handler == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_auto_freq_process_func get handler failed!}");
    } else {
        pst_custom_process_func_handler->p_custom_cali_func = wal_custom_cali;
    }

    return OAL_SUCC;
}

#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

