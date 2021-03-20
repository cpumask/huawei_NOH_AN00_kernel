

/* 1 头文件包含 */
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_types.h"
#include "wlan_chip_i.h"

#include "mac_device.h"
#include "mac_vap.h"
#include "mac_resource.h"

#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "wal_ext_if.h"
#include "wal_main.h"
#include "wal_config.h"
#include "wal_linux_bridge.h"

#include "hmac_cali_mgmt.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif

#if ((_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX) || (defined(WIN32)))
#include "plat_pm_wlan.h"
#endif

#include "securec.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_11AX
#include "hmac_wifi6_self_cure.h"
#endif
#include "hmac_ht_self_cure.h"
#include "hmac_p2p.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_CONFIG_C

/* 2 全局变量定义 */
/* 静态函数声明 */
OAL_STATIC uint32_t wal_config_add_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_del_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_down_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_start_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_bandwidth(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_mac_addr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_bss_type(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_bss_type(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_ssid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_ssid(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_shortgi20(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_shortgi20(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_shortgi40(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_shortgi40(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_shortgi80(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_shortgi80(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_shpreamble(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_shpreamble(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_MONITOR
OAL_STATIC uint32_t wal_config_get_monitor_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_monitor_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_sniffer(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_get_prot_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_prot_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_auth_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_auth_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_bintval(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_bintval(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_nobeacon(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_nobeacon(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_txpower(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_txpower(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_freq(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_freq(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_wmm_params(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_vap_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_btcoex_preempt_type(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_btcoex_set_perf_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_NRCOEX
OAL_STATIC uint32_t wal_config_nrcoex_priority_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_nrcoex_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_query_nrcoex_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

OAL_STATIC uint32_t wal_config_lte_gpio_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_random_mac_addr_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_random_mac_oui(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_vowifi_nat_keep_alive_params(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_add_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_del_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_sta_list(mac_vap_stru *pst_mac_vap, uint16_t *us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
OAL_STATIC uint32_t wal_config_reduce_sar(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
OAL_STATIC uint32_t wal_config_tas_pwr_ctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
OAL_STATIC uint32_t wal_config_tas_rssi_access(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_kick_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_uapsd_en(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_uapsd_en(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_country(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_country_for_dfs(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_country(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_tid(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_beacon(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_mib_by_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_add_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_remove_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_default_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_scan_abort(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_cfg80211_start_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_cfg80211_stop_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_cfg80211_start_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_cfg80211_start_join(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_alg_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_rx_fcs_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_frag_threshold(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_rts_threshold(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_vowifi_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_update_ip_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t  wal_config_set_probe_resp_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_user_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_dscr_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_log_level(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_pm_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_power_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_ant(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_ant(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_mcs(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_mcsac(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_p2p_scenes(mac_vap_stru *p_mac_vap, uint16_t len, uint8_t *p_param);
OAL_STATIC uint32_t wal_config_set_channge_go_channel(mac_vap_stru *p_mac_vap, uint16_t len, uint8_t *p_param);
OAL_STATIC uint32_t wal_config_clear_11n_blacklist(mac_vap_stru *p_mac_vap, uint16_t len, uint8_t *p_param);
#ifdef _PRE_WLAN_FEATURE_11AX
OAL_STATIC uint32_t _wal_config_set_mcsax(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
OAL_STATIC uint32_t _wal_config_set_mcsax_er(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_set_11ax_wifi6_black_list(mac_vap_stru *p_mac_vap,
    uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_11ax_close_htc(mac_vap_stru *p_mac_vap,
    uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_set_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_always_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_always_tx_num(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_always_tx_aggr_num(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_ru_index(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
OAL_STATIC uint32_t wal_config_always_rx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_tx_pow_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_get_dieid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef PLATFORM_DEBUG_ENABLE
OAL_STATIC uint32_t wal_config_reg_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_sdio_flowctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_DELAY_STATISTIC
OAL_STATIC uint32_t wal_config_pkt_time_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#ifdef PLATFORM_DEBUG_ENABLE
OAL_STATIC uint32_t wal_config_reg_write(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG
OAL_STATIC uint32_t wal_config_dpd_cfg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#endif
OAL_STATIC uint32_t wal_config_open_wmm(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_vap_wmm_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_vap_wmm_switch(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_version(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_bg_noise(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_wps_p2p_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_wps_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_blacklist_clr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_blacklist_add(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_blacklist_add_only(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_blacklist_del(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_blacklist_show(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_del_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_flush_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_remain_on_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_cancel_remain_on_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_mgmt_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_query_station_stats(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_query_rssi(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_query_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_query_psst(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_query_psm_flt_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_DFT_STAT
OAL_STATIC uint32_t wal_config_query_ani(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

#ifdef _PRE_WLAN_FEATURE_HS20
OAL_STATIC uint32_t wal_config_set_qos_map(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t  wal_config_set_dc_status(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_p2p_miracast_status(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_p2p_ps_ops(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_p2p_ps_noa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_sta_pm_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_show_pm_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_sta_pm_on(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_fast_sleep_para(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
OAL_STATIC uint32_t wal_config_set_rx_listen_ps_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
#endif

OAL_STATIC uint32_t wal_config_set_uapsd_para(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_max_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_cfg_vap_h2d(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_host_dev_init(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_host_dev_exit(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

uint32_t wal_send_cali_data(oal_net_device_stru *pst_net_dev);

#ifdef _PRE_WLAN_FEATURE_11R
OAL_STATIC uint32_t wal_config_set_ft_ies(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif  // _PRE_WLAN_FEATURE_11R

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
OAL_STATIC uint32_t wal_config_set_all_log_level(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_cus_rf(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_cus_dts_cali(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_cus_dyn_cali(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_cus_nvram_params(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_dev_customize_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
OAL_STATIC uint32_t wal_config_vap_destroy(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t wal_config_set_vendor_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

OAL_STATIC uint32_t wal_config_set_mlme(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC uint32_t wal_config_send_neighbor_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

OAL_STATIC uint32_t wal_config_vendor_cmd_get_channel_list(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
    uint8_t *puc_param);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_atomic g_wal_config_seq_num = ATOMIC_INIT(0);
oal_module_symbol(g_wal_config_seq_num);
#else
oal_atomic g_wal_config_seq_num = 0;
#endif

OAL_STATIC uint32_t wal_config_get_wmmswitch(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_get_max_user(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_M2S_MSS
OAL_STATIC uint32_t wal_config_set_m2s_switch_blacklist(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_set_m2s_switch_mss(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
#ifdef _PRE_WLAN_FEATURE_M2S_MODEM
OAL_STATIC uint32_t wal_config_set_m2s_switch_modem(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param);
#endif
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifndef CONFIG_HAS_EARLYSUSPEND
OAL_STATIC uint32_t wal_config_set_suspend_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_APF
OAL_STATIC uint32_t wal_config_apf_filter_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
OAL_STATIC uint32_t wal_config_remove_app_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_force_stop_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_SAE
OAL_STATIC uint32_t wal_config_external_auth(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif /* _PRE_WLAN_FEATURE_SAE */

#ifdef _PRE_WLAN_FEATURE_NAN
OAL_STATIC uint32_t wal_config_nan(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
OAL_STATIC uint32_t wal_config_hid2d_acs_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
OAL_STATIC uint32_t wal_config_switch_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
OAL_STATIC uint32_t wal_config_scan_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#endif

/* cfgid操作全局变量 */
OAL_STATIC OAL_CONST wal_wid_op_stru g_ast_board_wid_op[] = {
    /* cfgid                   是否复位mac  保留一字节   get函数              set函数 */
    { WLAN_CFGID_BSS_TYPE,  OAL_TRUE,  { 0 }, wal_config_get_bss_type, wal_config_set_bss_type },
    { WLAN_CFGID_ADD_VAP,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_add_vap },
    { WLAN_CFGID_START_VAP, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_start_vap },
    { WLAN_CFGID_DEL_VAP,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_del_vap },
    { WLAN_CFGID_DOWN_VAP,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_down_vap },
    { WLAN_CFGID_MODE,      OAL_FALSE, { 0 }, wal_config_get_mode, wal_config_set_mode },
    { WLAN_CFGID_BANDWIDTH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_bandwidth },

    { WLAN_CFGID_CURRENT_CHANEL, OAL_FALSE, { 0 }, wal_config_get_freq, wal_config_set_freq },
    { WLAN_CFGID_STATION_ID,     OAL_TRUE,  { 0 }, OAL_PTR_NULL, wal_config_set_mac_addr },
    { WLAN_CFGID_SSID,           OAL_FALSE, { 0 }, wal_config_get_ssid, wal_config_set_ssid },
    { WLAN_CFGID_SHORTGI,        OAL_FALSE, { 0 }, wal_config_get_shortgi20, wal_config_set_shortgi20 },
    { WLAN_CFGID_SHORTGI_FORTY,  OAL_FALSE, { 0 }, wal_config_get_shortgi40, wal_config_set_shortgi40 },
    { WLAN_CFGID_SHORTGI_EIGHTY, OAL_FALSE, { 0 }, wal_config_get_shortgi80, wal_config_set_shortgi80 },

    { WLAN_CFGID_SHORT_PREAMBLE, OAL_FALSE, { 0 }, wal_config_get_shpreamble, wal_config_set_shpreamble },
#ifdef _PRE_WLAN_FEATURE_MONITOR
    { WLAN_CFGID_SNIFFER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_sniffer },
    { WLAN_CFGID_MONITOR_MODE, OAL_FALSE, { 0 }, wal_config_get_monitor_mode, wal_config_set_monitor_mode },
#endif
    { WLAN_CFGID_PROT_MODE,       OAL_FALSE, { 0 }, wal_config_get_prot_mode, wal_config_set_prot_mode },
    { WLAN_CFGID_AUTH_MODE,       OAL_FALSE, { 0 }, wal_config_get_auth_mode, wal_config_set_auth_mode },
    { WLAN_CFGID_BEACON_INTERVAL, OAL_FALSE, { 0 }, wal_config_get_bintval, wal_config_set_bintval },
    { WLAN_CFGID_NO_BEACON,       OAL_FALSE, { 0 }, wal_config_get_nobeacon, wal_config_set_nobeacon },
    { WLAN_CFGID_TX_POWER,        OAL_FALSE, { 0 }, wal_config_get_txpower, wal_config_set_txpower },
    { WLAN_CFGID_UAPSD_EN, OAL_FALSE, { 0 }, wal_config_get_uapsd_en, wal_config_set_uapsd_en },
    { WLAN_CFGID_DTIM_PERIOD, OAL_FALSE, { 0 }, wal_config_get_dtimperiod, wal_config_set_dtimperiod },

    { WLAN_CFGID_EDCA_TABLE_CWMIN,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_EDCA_TABLE_CWMAX,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_EDCA_TABLE_AIFSN,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_EDCA_TABLE_TXOP_LIMIT,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_EDCA_TABLE_MSDU_LIFETIME,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_EDCA_TABLE_MANDATORY,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_CWMIN,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_CWMAX,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_AIFSN,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_TXOP_LIMIT,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_MSDU_LIFETIME, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },
    { WLAN_CFGID_QEDCA_TABLE_MANDATORY,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wmm_params },

    { WLAN_CFGID_VAP_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_vap_info },
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
    { WLAN_CFGID_SYNC_PK_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, hmac_config_pk_mode_debug },
#endif
    { WLAN_CFGID_BTCOEX_PREEMPT_TYPE,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_btcoex_preempt_type },
    { WLAN_CFGID_BTCOEX_SET_PERF_PARAM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_btcoex_set_perf_param },
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    { WLAN_CFGID_NRCOEX_PRIORITY_SET, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_nrcoex_priority_set },
    { WLAN_CFGID_NRCOEX_TEST,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_nrcoex_test },
    { WLAN_CFGID_QUERY_NRCOEX_STAT,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_nrcoex_stat },
#endif
    { WLAN_CFGID_CHECK_LTE_GPIO, OAL_FALSE, { 0 }, NULL, wal_config_lte_gpio_mode },

    { WLAN_CFGID_SET_RANDOM_MAC_ADDR_SCAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_random_mac_addr_scan },
    { WLAN_CFGID_SET_RANDOM_MAC_OUI,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_random_mac_oui },
    { WLAN_CFGID_ADD_USER,                 OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_add_user },
    { WLAN_CFGID_DEL_USER,                 OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_del_user },
    { WLAN_CFGID_SET_LOG_LEVEL,            OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_log_level },
    { WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_vowifi_nat_keep_alive_params },

    { WLAN_CFGID_SET_PM_SWITCH,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_pm_switch },
    { WLAN_CFGID_SET_POWER_TEST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_power_test },
    { WLAN_CFGID_SET_ANT,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_ant },
    { WLAN_CFGID_GET_ANT,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_ant },
    { WLAN_CFGID_KICK_USER,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_kick_user },
    { WLAN_CFGID_COUNTRY,          OAL_FALSE, { 0 }, wal_config_get_country, wal_config_set_country },
    { WLAN_CFGID_COUNTRY_FOR_DFS,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_country_for_dfs },
    { WLAN_CFGID_TID,              OAL_FALSE, { 0 }, wal_config_get_tid, OAL_PTR_NULL },

    { WLAN_CFGID_FRAG_THRESHOLD_REG, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_frag_threshold },
    { WLAN_CFGID_RX_FCS_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_rx_fcs_info },

    { WLAN_CFGID_USER_INFO,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_user_info },
    { WLAN_CFGID_SET_DSCR,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_dscr_param },
    { WLAN_CFGID_SET_RATE,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_rate },
    { WLAN_CFGID_SET_MCS,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_mcs },
    { WLAN_CFGID_SET_MCSAC,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_mcsac },
#ifdef _PRE_WLAN_FEATURE_11AX
    { WLAN_CFGID_SET_MCSAX, OAL_FALSE, { 0 }, OAL_PTR_NULL, _wal_config_set_mcsax },
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    { WLAN_CFGID_SET_MCSAX_ER, OAL_FALSE, { 0 }, OAL_PTR_NULL, _wal_config_set_mcsax_er },
#endif
    {WLAN_CFGID_SET_11AX_WIFI6_BLACKLIST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_11ax_wifi6_black_list},
    {WLAN_CFGID_SET_11AX_CLOSE_HTC, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_11ax_close_htc},
#endif
    {WLAN_CFGID_SET_CLEAR_11N_BLACKLIST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_clear_11n_blacklist},
    { WALN_CFGID_SET_P2P_SCENES, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_p2p_scenes },
    { WALN_CFGID_SET_CHANGE_GO_CHANNEL, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_channge_go_channel },
    { WLAN_CFGID_SET_BW, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_bw },
    { WLAN_CFGID_SET_ALWAYS_TX,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_always_tx },
    { WLAN_CFGID_SET_ALWAYS_TX_NUM,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_always_tx_num },
    { WLAN_CFGID_SET_ALWAYS_TX_AGGR_NUM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_always_tx_aggr_num },
    { WLAN_CFGID_SET_RU_INDEX, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_ru_index },
    { WLAN_CFGID_SET_ALWAYS_RX, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_always_rx },
    { WLAN_CFGID_GET_DIEID,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_dieid },
    { WLAN_CFGID_SET_TX_POW,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tx_pow_param },
#ifdef PLATFORM_DEBUG_ENABLE
    { WLAN_CFGID_REG_INFO,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_reg_info },
#endif
    { WLAN_CFGID_SDIO_FLOWCTRL, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_sdio_flowctrl },
#ifdef _PRE_WLAN_DELAY_STATISTIC
    { WLAN_CFGID_PKT_TIME_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_pkt_time_switch },
#endif
#ifdef PLATFORM_DEBUG_ENABLE
    { WLAN_CFGID_REG_WRITE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_reg_write },
#endif
#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG
    { WLAN_CFGID_DPD, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_dpd_cfg },
#endif
#endif
    { WLAN_CFGID_SCAN_ABORT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_scan_abort },
    /* 以下为内核cfg80211配置的命令 */
    { WLAN_CFGID_CFG80211_START_SCHED_SCAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg80211_start_sched_scan },
    { WLAN_CFGID_CFG80211_STOP_SCHED_SCAN,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg80211_stop_sched_scan },
    { WLAN_CFGID_CFG80211_START_SCAN,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg80211_start_scan },
    { WLAN_CFGID_CFG80211_START_CONNECT,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg80211_start_join },
    { WLAN_CFGID_CFG80211_SET_CHANNEL,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_channel },
    { WLAN_CFGID_CFG80211_SET_MIB_BY_BW,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_mib_by_bw },
    { WLAN_CFGID_CFG80211_CONFIG_BEACON,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_beacon },

    { WLAN_CFGID_ADD_KEY,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_add_key },
    { WLAN_CFGID_GET_KEY,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_key },
    { WLAN_CFGID_REMOVE_KEY, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_remove_key },

    { WLAN_CFGID_ALG_PARAM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_alg_param },

    /* START:开源APP 程序下发的私有命令 */
    { WLAN_CFGID_SET_WPS_IE,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wps_ie },
    { WLAN_CFGID_SET_RTS_THRESHHOLD, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_rts_threshold },
    { WLAN_CFGID_VOWIFI_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_vowifi_info },
    { WLAN_CFGID_IP_FILTER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_update_ip_filter },

    {WLAN_CFGID_PROBE_PESP_MODE, OAL_FALSE, {0}, OAL_PTR_NULL, wal_config_set_probe_resp_mode},

    { WLAN_CFGID_FBT_GET_STA_11H_ABILITY, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_sta_11h_abillty },
    { WLAN_CFGID_SET_VENDOR_IE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_vendor_ie },

    /* END:开源APP 程序下发的私有命令 */
    { WLAN_CFGID_SET_MLME, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_mlme },

    { WLAN_CFGID_DEFAULT_KEY,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_default_key },
    { WLAN_CFGID_WMM_SWITCH,     OAL_FALSE, { 0 }, wal_config_get_wmmswitch, wal_config_open_wmm },
    { WLAN_CFGID_VAP_WMM_SWITCH, OAL_FALSE, { 0 }, wal_config_get_vap_wmm_switch, wal_config_set_vap_wmm_switch },
    { WLAN_CFGID_GET_VERSION,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_version },
    /* 黑名单配置 */
    { WLAN_CFGID_CLR_BLACK_LIST,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_blacklist_clr },
    { WLAN_CFGID_ADD_BLACK_LIST,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_blacklist_add },
    { WLAN_CFGID_DEL_BLACK_LIST,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_blacklist_del },
    { WLAN_CFGID_BLACKLIST_MODE,      OAL_FALSE, { 0 }, wal_config_get_blacklist_mode, wal_config_set_blacklist_mode },
    { WLAN_CFGID_BLACKLIST_SHOW,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_blacklist_show },
    { WLAN_CFGID_ADD_BLACK_LIST_ONLY, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_blacklist_add_only },
    { WLAN_CFGID_CFG80211_SET_PMKSA,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_pmksa },
    { WLAN_CFGID_CFG80211_DEL_PMKSA,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_del_pmksa },
    { WLAN_CFGID_CFG80211_FLUSH_PMKSA, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_flush_pmksa },
#ifdef _PRE_WLAN_FEATURE_SAE
    { WLAN_CFGID_CFG80211_EXTERNAL_AUTH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_external_auth },
#endif /* _PRE_WLAN_FEATURE_SAE */
    { WLAN_CFGID_SET_WPS_P2P_IE,                    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_wps_p2p_ie },
    { WLAN_CFGID_CFG80211_REMAIN_ON_CHANNEL,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_remain_on_channel },
    { WLAN_CFGID_CFG80211_CANCEL_REMAIN_ON_CHANNEL, OAL_FALSE, { 0 },
        OAL_PTR_NULL, wal_config_cancel_remain_on_channel },
    { WLAN_CFGID_CFG80211_MGMT_TX,                  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_mgmt_tx },

    { WLAN_CFGID_QUERY_STATION_STATS, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_station_stats },

    { WLAN_CFGID_QUERY_RSSI, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_rssi },
    { WLAN_CFGID_QUERY_RATE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_rate },
    { WLAN_CFGID_QUERY_PSST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_psst },
    { WLAN_CFGID_QUERY_PSM_STAT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_psm_flt_stat },
#ifdef _PRE_WLAN_DFT_STAT
    { WLAN_CFGID_QUERY_ANI, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_query_ani },
#endif

    { WLAN_CFGID_SET_PS_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_sta_pm_mode },
    { WLAN_CFGID_SHOW_PS_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_show_pm_info },
    { WLAN_CFGID_SET_STA_PM_ON, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_sta_pm_on },
    { WLAN_CFGID_SET_FASTSLEEP_PARA, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_fast_sleep_para },

    { WLAN_CFGID_SET_UAPSD_PARA, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_uapsd_para },

    {WLAN_CFGID_DC_STATUS,     OAL_FALSE,  {0},   OAL_PTR_NULL,   wal_config_set_dc_status},
    { WLAN_CFGID_SET_P2P_MIRACAST_STATUS, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_p2p_miracast_status },
    { WLAN_CFGID_SET_P2P_PS_OPS,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_p2p_ps_ops },
    { WLAN_CFGID_SET_P2P_PS_NOA,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_p2p_ps_noa },
#ifdef _PRE_WLAN_FEATURE_HS20
    { WLAN_CFGID_SET_QOS_MAP, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_qos_map },
#endif

    { WLAN_CFGID_CFG_VAP_H2D,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg_vap_h2d },
    { WLAN_CFGID_HOST_DEV_INIT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_host_dev_init },
    { WLAN_CFGID_HOST_DEV_EXIT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_host_dev_exit },
    { WLAN_CFGID_SET_MAX_USER, OAL_FALSE, { 0 }, wal_config_get_max_user, wal_config_set_max_user },
    { WLAN_CFGID_GET_STA_LIST, OAL_FALSE, { 0 }, wal_config_get_sta_list, OAL_PTR_NULL },
#ifdef _PRE_WLAN_FEATURE_11R
    { WLAN_CFGID_SET_FT_IES, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_ft_ies },
#endif  // _PRE_WLAN_FEATURE_11R

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    /*  03/05定制化参数下发接口  */
    { WLAN_CFGID_SET_CUS_DYN_CALI_PARAM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_cus_dyn_cali },
    { WLAN_CFGID_SET_ALL_LOG_LEVEL,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_all_log_level },
    { WLAN_CFGID_SET_CUS_RF,             OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_cus_rf },
    { WLAN_CFGID_SET_CUS_DTS_CALI,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_cus_dts_cali },
    { WLAN_CFGID_SET_CUS_NVRAM_PARAM,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_cus_nvram_params },
    /* SHOW DEIVCE CUSTOMIZE INFOS */
    { WLAN_CFGID_SHOW_DEV_CUSTOMIZE_INFOS, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_dev_customize_info },
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

    { WLAN_CFGID_DESTROY_VAP, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_vap_destroy },
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
    { WLAN_CFGID_REDUCE_SAR, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_reduce_sar },
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    { WLAN_CFGID_TAS_PWR_CTRL, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_tas_pwr_ctrl },
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    { WLAN_CFGID_TAS_RSSI_ACCESS, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_tas_rssi_access },
#endif

#ifdef _PRE_WLAN_FEATURE_11K
    { WLAN_CFGID_SEND_NEIGHBOR_REQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_send_neighbor_req },
#endif

    { WLAN_CFGID_VENDOR_CMD_GET_CHANNEL_LIST, OAL_FALSE, { 0 }, wal_config_vendor_cmd_get_channel_list, OAL_PTR_NULL },
    { WLAN_CFGID_GET_BG_NOISE, OAL_FALSE, { 0 }, wal_config_get_bg_noise, OAL_PTR_NULL },
#ifdef _PRE_WLAN_FEATURE_M2S_MSS
    { WLAN_CFGID_SET_M2S_BLACKLIST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_m2s_switch_blacklist },
    { WLAN_CFGID_SET_M2S_MSS, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_m2s_switch_mss },
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
#ifdef _PRE_WLAN_FEATURE_M2S_MODEM
    { WLAN_CFGID_SET_M2S_MODEM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_m2s_switch_modem },
#endif
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifndef CONFIG_HAS_EARLYSUSPEND
    { WLAN_CFGID_SET_SUSPEND_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_suspend_mode },
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_APF
    { WLAN_CFGID_SET_APF_FILTER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_apf_filter_cmd },
#endif
    { WLAN_CFGID_REMOVE_APP_IE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_remove_app_ie },
    { WLAN_CFGID_FORCE_STOP_FILTER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_force_stop_filter },
    { WLAN_CFGID_SPEC_FEMLP_EN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_fem_lp_flag },
    { WLAN_CFGID_SOFTAP_MIMO_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_softap_mimo_mode },
    { WLAN_CFGID_ASSIGNED_FILTER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_assigned_filter },
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    { WLAN_CFGID_RX_LISTEN_PS_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_rx_listen_ps_switch},
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
    { WLAN_CFGID_NAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_nan },
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
    { WLAN_CFGID_HID2D_ACS_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_hid2d_acs_mode},
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
    { WLAN_CFGID_HID2D_SWITCH_CHAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_switch_channel},
    { WLAN_CFGID_HID2D_SCAN_CHAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_scan_channel},
#endif
#endif
    { WLAN_CFGID_BUTT, OAL_FALSE, { 0 }, 0, 0 },
};
/* 3 函数实现 */

OAL_STATIC uint32_t wal_config_add_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_add_vap(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        return ul_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_config_del_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_del_vap::pst_mac_vap or puc_param null ptr error [%x], [%x]}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_del_vap(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_config_del_vap:: return error code [%d].}\r\n",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_start_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_start_vap::pst_mac_vap or puc_param null ptr error %x,%x.}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_start_vap(pst_mac_vap, us_len, puc_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_start_vap:: return error code %d.}\r\n",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_config_down_vap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_down_vap::pst_mac_vap or puc_param null ptr error [%x],[%x].}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_down_vap(pst_mac_vap, us_len, puc_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_down_vap:: return error code [%d].}\r\n",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_set_bss_type(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_bss_type(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_bss_type(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_bss_type(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_mode(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_mib_by_bw_param_stru st_cfg;

    if (pst_mac_vap == OAL_PTR_NULL || puc_param == OAL_PTR_NULL) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{wal_config_set_mode::null ptr, vap=%p param=%p",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_cfg.en_band = ((mac_cfg_mode_param_stru *)puc_param)->en_band;
    st_cfg.en_bandwidth = ((mac_cfg_mode_param_stru *)puc_param)->en_bandwidth;

    hmac_config_set_mib_by_bw(pst_mac_vap, (uint16_t)OAL_SIZEOF(st_cfg), (uint8_t *)&st_cfg);

    return hmac_config_set_mode(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_bandwidth(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_mode_param_stru *pst_prot_param;

    pst_prot_param = (mac_cfg_mode_param_stru *)puc_param;

    pst_prot_param->en_protocol = pst_mac_vap->en_protocol;
    pst_prot_param->en_band = pst_mac_vap->st_channel.en_band;

    return hmac_config_set_mode(pst_mac_vap, us_len, (uint8_t *)pst_prot_param);
}


OAL_STATIC uint32_t wal_config_set_mac_addr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_set_mac_addr::pst_mac_vap or puc_param null ptr error %x,%x.}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_set_mac_addr(pst_mac_vap, us_len, puc_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_config_set_mac_addr:: return error code %d.}\r\n",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_get_ssid(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_ssid(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_ssid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_ssid(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_shpreamble(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_shpreamble(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_shpreamble(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_shpreamble(pst_mac_vap, pus_len, puc_param);
}
#ifdef _PRE_WLAN_FEATURE_MONITOR

OAL_STATIC uint32_t wal_config_set_sniffer(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_sniffer(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_monitor_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_monitor_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_monitor_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_monitor(pst_mac_vap, pus_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_set_shortgi20(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_shortgi20(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_shortgi40(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_shortgi40(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_shortgi80(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_shortgi80(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_shortgi20(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_shortgi20(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_shortgi40(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_shortgi40(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_shortgi80(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_shortgi80(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_prot_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_prot_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_prot_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_prot_mode(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_auth_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_auth_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_auth_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_auth_mode(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_bintval(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_bintval(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_bintval(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_bintval(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_dtimperiod(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_dtimperiod(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_nobeacon(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_nobeacon(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_nobeacon(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_nobeacon(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_txpower(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_txpower(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_txpower(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_txpower(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_freq(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_freq(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_freq(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_freq(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_wmm_params(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_wmm_params(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_get_wmm_params(oal_net_device_stru *pst_net_dev, uint8_t *puc_param)
{
    mac_vap_stru *pst_vap;

    pst_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY,
                       "{wal_config_get_wmm_params::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr.}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return hmac_config_get_wmm_params(pst_vap, puc_param);
}


OAL_STATIC uint32_t wal_config_set_uapsd_en(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_uapsden(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_uapsd_en(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_uapsden(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_mib_by_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_mib_by_bw(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_mib_by_bw_param_stru st_cfg;

    if ((pst_mac_vap == OAL_PTR_NULL) || (puc_param == OAL_PTR_NULL)) {
        oam_warning_log2(0, OAM_SF_CFG,
                         "{wal_config_set_channel::null ptr, vap=%p param=%p",
                         (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_cfg.en_band = ((mac_cfg_channel_param_stru *)puc_param)->en_band;
    st_cfg.en_bandwidth = ((mac_cfg_channel_param_stru *)puc_param)->en_bandwidth;

    hmac_config_set_mib_by_bw(pst_mac_vap, (uint16_t)OAL_SIZEOF(st_cfg), (uint8_t *)&st_cfg);

    return hmac_config_set_channel(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_beacon(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_set_beacon::pst_mac_vap or puc_param is null [%x],[%x].}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_set_beacon(pst_mac_vap, us_len, puc_param);

    return ul_ret;
}


OAL_STATIC uint32_t wal_config_vap_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_vap_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_btcoex_preempt_type(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_btcoex_preempt_type(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_btcoex_set_perf_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_btcoex_set_perf_param(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_NRCOEX
OAL_STATIC uint32_t wal_config_nrcoex_priority_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_nrcoex_priority_set(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_query_nrcoex_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_nrcoex_stat(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_nrcoex_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_nrcoex_test(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_lte_gpio_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_lte_gpio_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_random_mac_addr_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_random_mac_addr_scan(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_random_mac_oui(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_random_mac_oui(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_vowifi_nat_keep_alive_params(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param)
{
    return hmac_config_set_vowifi_nat_keep_alive_params(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_add_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_add_user(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_del_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_del_user(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_sta_list(mac_vap_stru *pst_mac_vap, uint16_t *us_len, uint8_t *puc_param)
{
    return hmac_config_get_sta_list(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_TPC_OPT

OAL_STATIC uint32_t wal_config_reduce_sar(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_reduce_sar(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH

OAL_STATIC uint32_t wal_config_tas_pwr_ctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_tas_pwr_ctrl(pst_mac_vap, us_len, puc_param);
}
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH

OAL_STATIC uint32_t wal_config_tas_rssi_access(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_tas_rssi_access(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_cfg80211_start_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_cfg80211_start_sched_scan(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_cfg80211_stop_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_cfg80211_stop_sched_scan(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_scan_abort(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_scan_abort(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_cfg80211_start_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_cfg80211_start_scan_sta(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_cfg80211_start_join(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_connect(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_frag_threshold(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_frag_threshold_stru *pst_frag_threshold = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_frag_threshold:: pst_mac_vap/puc_param is null ptr %x, %x!}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_frag_threshold = (mac_cfg_frag_threshold_stru *)puc_param;

    if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_config_frag_threshold:pst_mib_info[%x]!}\r\n",
                       (uintptr_t)(pst_mac_vap->pst_mib_info));
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_mib_set_FragmentationThreshold(pst_mac_vap, pst_frag_threshold->ul_frag_threshold);
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_rts_threshold(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_rts_threshold_stru *pst_rts_threshold = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{wal_config_rts_threshold:: pst_mac_vap/puc_param is null ptr %x, %x!}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_rts_threshold = (mac_cfg_rts_threshold_stru *)puc_param;

    if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{wal_config_rts_threshold: pst_mac_vap->pst_mib_info is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_mib_set_RTSThreshold(pst_mac_vap, pst_rts_threshold->ul_rts_threshold);

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_kick_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_kick_user(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_country(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_country(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_country_for_dfs(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_country_for_dfs(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_country(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_country(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_tid(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_tid(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_user_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_user_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_dscr_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_dscr_param(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_log_level(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_log_level(pst_mac_vap, us_len, puc_param);
}



OAL_STATIC uint32_t wal_config_set_pm_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    uint8_t uc_en;

    uc_en = (uint8_t)(*puc_param);

    oam_warning_log1(0, OAM_SF_PWR, "{wal_config_set_pm_switch:[%d]}\r\n", uc_en);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if ((uc_en == 1) || (uc_en == 4)) { /* 1, 4 关闭pm */
        g_wlan_host_pm_switch = OAL_TRUE;
        wlan_pm_enable();
    } else {
        wlan_pm_disable();
        g_wlan_host_pm_switch = OAL_FALSE;
    }
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    g_wlan_device_pm_switch = uc_en;
#endif

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SET_PM_SWITCH, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_set_feature_log::hmac_config_send_event failed[%d].}",
                         ul_ret);
    }

    return ul_ret;
}

OAL_STATIC uint32_t wal_config_set_power_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    uint8_t uc_en;

    uc_en = (uint8_t)(*puc_param);

    oam_warning_log1(0, OAM_SF_PWR, "{wal_config_set_power_test:[%d]}\r\n", uc_en);

    /***************************************************************************
        ....DMAC., ..DMAC..
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SET_POWER_TEST, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_set_feature_log::hmac_config_send_event failed[%d].}",
                         ul_ret);
    }

    return ul_ret;
}

OAL_STATIC uint32_t wal_config_set_ant(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    uint8_t uc_en;

    uc_en = (uint8_t)(*puc_param);

    oam_warning_log1(0, OAM_SF_PWR, "{wal_config_set_ant:[%d]}\r\n", uc_en);

    /***************************************************************************
        ....DMAC., ..DMAC..
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SET_ANT, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_set_feature_log::hmac_config_send_event failed[%d].}",
                         ul_ret);
    }

    return ul_ret;
}

OAL_STATIC uint32_t wal_config_get_ant(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_ant(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    mac_cfg_tx_comp_stru st_event_set_bcast;

    memset_s(&st_event_set_bcast, OAL_SIZEOF(mac_cfg_tx_comp_stru),
             0, OAL_SIZEOF(mac_cfg_tx_comp_stru));
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log2(0, OAM_SF_ANY,
                         "{wal_config_set_rate::pst_mac_vap/puc_param is null ptr %x %x!}\r\n",
                         (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_rate(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_rate::hmac_config_set_rate error!}\r\n");
        return ul_ret;
    }
    return ul_ret;
}


OAL_STATIC uint32_t wal_config_set_mcs(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_mcs::pst_mac_vap/puc_param is null ptr}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_mcs(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_mcs::hmac_config_set_mcs error.}\r\n");
        return ul_ret;
    }

    return ul_ret;
}


OAL_STATIC uint32_t wal_config_set_mcsac(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_mcsac::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_mcsac(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_mcsac::hmac_config_set_mcsac error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}

OAL_STATIC uint32_t wal_config_set_p2p_scenes(mac_vap_stru *p_mac_vap,
    uint16_t len, uint8_t *p_param)
{
    return hmac_p2p_set_scenes(p_mac_vap, len, p_param);
}

OAL_STATIC uint32_t wal_config_set_channge_go_channel(mac_vap_stru *mac_vap,
    uint16_t len, uint8_t *param)
{
    return hmac_p2p_change_go_channel(mac_vap, len, param);
}

OAL_STATIC uint32_t wal_config_clear_11n_blacklist(mac_vap_stru *p_mac_vap, uint16_t len, uint8_t *p_param)
{
    return hmac_ht_self_cure_clear_blacklist();
}

#ifdef _PRE_WLAN_FEATURE_11AX
OAL_STATIC uint32_t wal_config_set_11ax_wifi6_black_list(mac_vap_stru *p_mac_vap,
    uint16_t len, uint8_t *p_param)
{
    return hmac_wifi6_self_cure_black_list_set(p_mac_vap, len, p_param);
}

OAL_STATIC uint32_t wal_config_set_11ax_close_htc(mac_vap_stru *p_mac_vap,
    uint16_t len, uint8_t *p_param)
{
    return hmac_wifi6_self_cure_close_htc_handle(p_mac_vap);
}

OAL_STATIC uint32_t wal_config_set_mcsax(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_mcsax::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_mcsax(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_mcsax::hmac_config_set_mcsac error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}
OAL_STATIC uint32_t _wal_config_set_mcsax(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        return wal_config_set_mcsax(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
OAL_STATIC uint32_t wal_config_set_mcsax_er(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_mcsax_er::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_mcsax_er(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_mcsax_er::hmac_config_set_mcsac error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}
OAL_STATIC uint32_t _wal_config_set_mcsax_er(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_11ax_er_su_dcm_is_open) {
        return wal_config_set_mcsax_er(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif
#endif


OAL_STATIC uint32_t wal_config_set_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_bw::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_bw(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_config_set_bw::hmac_config_set_bw error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}


OAL_STATIC uint32_t wal_config_always_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_always_tx::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_always_tx(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_always_tx::hmac_config_always_tx failed!}\r\n");
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_always_tx_num(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_always_tx_num(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_always_tx_aggr_num(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_always_tx_aggr_num(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_ru_index(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    uint32_t ret;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_ru_index::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ret = hmac_config_ru_index(mac_vap, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_ru_index::hmac_config_always_tx failed!}\r\n");
        return ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_always_rx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_always_rx::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_always_rx(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_always_rx::hmac_config_always_rx failed!}\r\n");
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_set_tx_pow_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_tx_pow_param(pst_mac_vap, us_len, puc_param);
}
#ifdef PLATFORM_DEBUG_ENABLE

OAL_STATIC uint32_t wal_config_reg_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_reg_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_reg_write(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_reg_write(pst_mac_vap, us_len, puc_param);
}
#endif
#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG

OAL_STATIC uint32_t wal_config_dpd_cfg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_dpd_cfg(pst_mac_vap, us_len, puc_param);
}
#endif
#endif


OAL_STATIC uint32_t wal_config_sdio_flowctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_sdio_flowctrl(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_DELAY_STATISTIC

OAL_STATIC uint32_t wal_config_pkt_time_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_pkt_time_switch(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_alg_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_alg_param(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_rx_fcs_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_rx_fcs_info(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC wal_wid_op_stru *wal_config_get_wid_map(wal_wid_op_stru *pst_wid_map, uint16_t en_wid, uint32_t ul_wid_nums)
{
    uint16_t us_cfgid;
    wal_wid_op_stru *pst_current_wid = OAL_PTR_NULL;

    for (us_cfgid = 0; us_cfgid < ul_wid_nums; us_cfgid++) {
        pst_current_wid = pst_wid_map + us_cfgid;
        if (pst_current_wid->en_cfgid == en_wid) {
            return pst_current_wid;
        }
    }

    return NULL;
}


OAL_STATIC uint32_t wal_config_process_query_param_check(mac_vap_stru *pst_mac_vap, uint8_t *puc_req_msg,
    uint8_t *puc_rsp_msg, uint8_t *puc_rsp_msg_len)
{
    if (oal_any_null_ptr4(pst_mac_vap, puc_req_msg, puc_rsp_msg, puc_rsp_msg_len)) {
        oam_error_log4(0, OAM_SF_ANY,
            "{wal_config_process_query_param_check::mac_vap/req_msg/rsp_msg/rsp_msg_len null \
            ptr error: %x, %x, %x, %x!}\r\n",
            (uintptr_t)pst_mac_vap, (uintptr_t)puc_req_msg,
            (uintptr_t)puc_rsp_msg, (uintptr_t)puc_rsp_msg_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    if (pst_mac_vap->uc_init_flag == MAC_VAP_INVAILD) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_config_process_query_param_check::pst_mac_vap is deleted!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_process_query(mac_vap_stru *pst_mac_vap, uint8_t *puc_req_msg, uint16_t us_req_msg_len,
    uint8_t *puc_rsp_msg, uint8_t *puc_rsp_msg_len)
{
    uint16_t us_req_idx = 0; /* 请求消息索引 */
    uint16_t us_rsp_idx = 0; /* 返回消息索引 */
    uint16_t us_len = 0;     /* WID对应返回值的长度 */
    wal_msg_query_stru *pst_query_msg = OAL_PTR_NULL;
    wal_msg_write_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_ret;
    wal_wid_op_stru *pst_current_wid = OAL_PTR_NULL;

    ul_ret = wal_config_process_query_param_check(pst_mac_vap, puc_req_msg, puc_rsp_msg, puc_rsp_msg_len);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY,"{wal_config_process_query::param error!}\r\n");
        return ul_ret;
    }

    /* 查询消息格式如下:                                                     */
    /* +-------------------------------------------------------------------+ */
    /* | WID0          | WID1         | WID2         | ................... | */
    /* +-------------------------------------------------------------------+ */
    /* |     2 Bytes   |    2 Bytes   |    2 Bytes   | ................... | */
    /* +-------------------------------------------------------------------+ */
    /* 返回消息格式如下:                                                     */
    /* +-------------------------------------------------------------------+ */
    /* | WID0      | WID0 Length | WID0 Value  | ......................... | */
    /* +-------------------------------------------------------------------+ */
    /* | 2 Bytes   | 2 Byte      | WID Length  | ......................... | */
    /* +-------------------------------------------------------------------+ */
    while (us_req_idx < us_req_msg_len) {
        /* 从查询消息中得到一个WID值   */
        pst_query_msg = (wal_msg_query_stru *)(&puc_req_msg[us_req_idx]);
        us_req_idx += WAL_MSG_WID_LENGTH; /* 指向下一个WID */

        /* 获取返回消息内存 */
        pst_rsp_msg = (wal_msg_write_stru *)(&puc_rsp_msg[us_rsp_idx]);

        pst_current_wid = wal_config_get_wid_map((wal_wid_op_stru *)g_ast_board_wid_op,
                                                 pst_query_msg->en_wid,
                                                 oal_array_size(g_ast_board_wid_op));
        if (pst_current_wid == NULL) {
#ifdef _PRE_WLAN_CFGID_DEBUG
            pst_current_wid = wal_config_get_wid_map((wal_wid_op_stru *)g_ast_board_wid_op_debug,
                                                     pst_query_msg->en_wid,
                                                     wal_config_get_debug_wid_arrysize());
            if (pst_current_wid == NULL) {
                oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                               "{wal_config_process_query::cfgid not invalid %d!}\r\n",
                               pst_query_msg->en_wid);
                continue;
            }
            /* else, call the cfgid func. */
#else
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_query::cfgid not invalid %d!}\r\n",
                           pst_query_msg->en_wid);
            continue;
#endif
        }

        /* 异常情况，cfgid对应的get函数为空 */
        if (pst_current_wid->p_get_func == OAL_PTR_NULL) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_query:: get_func ptr is null, wid is %d!}\r\n",
                           pst_query_msg->en_wid);
            continue;
        }

        ul_ret = pst_current_wid->p_get_func(pst_mac_vap, &us_len, pst_rsp_msg->auc_value);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_config_process_query:: func return no SUCC. wid and ret value is:%d, %d!}\r\n",
                             pst_query_msg->en_wid, ul_ret);
            continue;
        }

        pst_rsp_msg->en_wid = pst_query_msg->en_wid; /* 设置返回消息的WID */
        pst_rsp_msg->us_len = us_len;

        us_rsp_idx += us_len + WAL_MSG_WRITE_MSG_HDR_LENGTH; /* 消息体的长度 再加上消息头的长度 */

        /* 消息Response 接口容易让调用者使用超过消息数组空间长度，
          这里需要加判断，检查长度和狗牌，后续需要整改 */
        if (oal_unlikely(us_rsp_idx + OAL_SIZEOF(wal_msg_hdr_stru) > HMAC_RSP_MSG_MAX_LEN)) {
            oam_error_log3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_query::us_cfgid:%d reponse msg len:%u over limit:%u}",
                           pst_current_wid->en_cfgid, us_rsp_idx + OAL_SIZEOF(wal_msg_hdr_stru),
                           HMAC_RSP_MSG_MAX_LEN);
        }
    }

    *puc_rsp_msg_len = (uint8_t)us_rsp_idx;

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_process_write_param_check(mac_vap_stru *pst_mac_vap, uint8_t *puc_req_msg)
{
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_req_msg))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_process_write_param_check::pst_mac_vap/puc_req_msg null ptr error %x, %x!}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_req_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    if (pst_mac_vap->uc_init_flag == MAC_VAP_INVAILD) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_process_write_param_check::pst_mac_vap is deleted!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_process_write(mac_vap_stru *pst_mac_vap, uint8_t *puc_req_msg, uint16_t us_msg_len,
    uint8_t *puc_rsp_msg, uint8_t *puc_rsp_msg_len)
{
    uint16_t us_req_idx = 0;
    uint16_t us_rsp_idx = 0;
    wal_msg_write_stru *pst_write_msg = OAL_PTR_NULL;
    wal_msg_write_rsp_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_ret;
    wal_wid_op_stru *pst_current_wid = OAL_PTR_NULL;

    ul_ret = wal_config_process_write_param_check(pst_mac_vap, puc_req_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_process_write::param error!}\r\n");
        return ul_ret;
    }

    /* 设置消息的格式如下:                                                   */
    /* +-------------------------------------------------------------------+ */
    /* | WID0      | WID0 Length | WID0 Value  | ......................... | */
    /* +-------------------------------------------------------------------+ */
    /* | 2 Bytes   | 2 Byte      | WID Length  | ......................... | */
    /* +-------------------------------------------------------------------+ */
    /* 返回消息的格式如下:                                                   */
    /* +-------------------------------------------------------------------+ */
    /* | WID0     | resv    | WID0 错误码 |  WID1   | resv | WID1错误码 |  | */
    /* +-------------------------------------------------------------------+ */
    /* | 2 Bytes  | 2 Bytes | 4 Byte      | 2 Bytes | 2 B  |  4 Bytes   |  | */
    /* +-------------------------------------------------------------------+ */
    while (us_req_idx < us_msg_len) {
        /* 获取一个设置WID消息   */
        pst_write_msg = (wal_msg_write_stru *)(&puc_req_msg[us_req_idx]);

        /* 获取返回消息内存 */
        pst_rsp_msg = (wal_msg_write_rsp_stru *)(&puc_rsp_msg[us_rsp_idx]);

        us_req_idx += pst_write_msg->us_len + WAL_MSG_WRITE_MSG_HDR_LENGTH; /* 指向下一个WID设置消息 */

        /* 寻找cfgid 对应的write函数 */
        pst_current_wid = wal_config_get_wid_map((wal_wid_op_stru *)g_ast_board_wid_op,
                                                 pst_write_msg->en_wid,
                                                 oal_array_size(g_ast_board_wid_op));
        if (pst_current_wid == NULL) {
#ifdef _PRE_WLAN_CFGID_DEBUG
            pst_current_wid = wal_config_get_wid_map((wal_wid_op_stru *)g_ast_board_wid_op_debug,
                                                     pst_write_msg->en_wid,
                                                     wal_config_get_debug_wid_arrysize());
            if (pst_current_wid == NULL) {
                oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                               "{wal_config_process_write::cfgid not invalid %d!}\r\n",
                               pst_write_msg->en_wid);
                continue;
            }
            /* else, go on call the cfgid func. */
#else
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_write::cfgid not invalid %d!}\r\n",
                           pst_write_msg->en_wid);
            continue;
#endif
        }

        /* 异常情况，cfgid对应的set函数为空 */
        if (pst_current_wid->p_set_func == OAL_PTR_NULL) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_write:: get_func ptr is null, wid is %d!}\r\n",
                           pst_write_msg->en_wid);
            continue;
        }

        ul_ret = pst_current_wid->p_set_func(pst_mac_vap, pst_write_msg->us_len, pst_write_msg->auc_value);

        /* 将返回错误码设置到rsp消息中 */
        pst_rsp_msg->en_wid = pst_write_msg->en_wid;
        pst_rsp_msg->ul_err_code = ul_ret;
        us_rsp_idx += OAL_SIZEOF(wal_msg_write_rsp_stru);
        /* 消息Response 接口容易让调用者使用超过消息数组空间长度，
          这里需要加判断，检查长度和狗牌，后续需要整改 */
        if (oal_unlikely(us_rsp_idx + OAL_SIZEOF(wal_msg_hdr_stru) > HMAC_RSP_MSG_MAX_LEN)) {
            oam_error_log3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{wal_config_process_write::us_cfgid:%d reponse msg len:%u over limit:%u}",
                           pst_current_wid->en_cfgid,
                           us_rsp_idx + OAL_SIZEOF(wal_msg_hdr_stru), HMAC_RSP_MSG_MAX_LEN);
        }

        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_config_process_write::func return no SUCC. wid and ret value is %d, %d!}\r\n",
                             pst_write_msg->en_wid, ul_ret);
        }
    }

    *puc_rsp_msg_len = (uint8_t)us_rsp_idx;

    return OAL_SUCC;
}


oal_net_device_stru *wal_config_get_netdev(const int8_t *pc_name, int32_t arrayLen)
{
    if (arrayLen < 0) {
        return OAL_PTR_NULL;
    }
#ifdef _PRE_CONFIG_HISI_110X_DISABLE_HISILICON0
    if ((oal_strcmp("Hisilicon0", pc_name)) == 0) {
         /* 单芯片的配置vap id=0，双芯片存在配置vap id=0和1，hisilicon0是vap id=0，hisilicon1是vap id=1 */
        return hmac_vap_get_net_device(0);
    } else
#endif
    {
        return oal_dev_get_by_name(pc_name);
    }
}


uint32_t wal_config_process_pkt(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    wal_msg_stru *pst_msg = OAL_PTR_NULL;
    wal_msg_stru *pst_rsp_msg = NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint16_t us_msg_len;
    uint8_t uc_rsp_len = 0;
    uint8_t uc_rsp_toal_len;
    uint32_t ul_ret;
    uintptr_t ul_request_address;
    uint8_t ac_rsp_msg[HMAC_RSP_MSG_MAX_LEN] = { 0 };
    uint8_t *puc_msg_data = OAL_PTR_NULL;
    uint8_t *puc_rsp_msg_data = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_config_process_pkt::pst_event_mem null ptr error!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    ul_request_address = ((wal_msg_rep_hdr *)pst_event->auc_event_data)->ul_request_address;
    pst_msg = (wal_msg_stru *)(frw_get_event_payload(pst_event_mem) + OAL_SIZEOF(wal_msg_rep_hdr));

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_event_hdr->uc_vap_id);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_warning_log0(pst_event_hdr->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_process_pkt::hmac_get_vap_by_id return err code!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 取返回消息 */
    pst_rsp_msg = (wal_msg_stru *)ac_rsp_msg;

    /* 取配置消息的长度 */
    us_msg_len = pst_msg->st_msg_hdr.us_msg_len;

    oam_info_log0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "{wal_config_process_pkt::a config event occur!}\r\n");
    puc_msg_data = &pst_msg->auc_msg_data[0];
    puc_rsp_msg_data = &pst_rsp_msg->auc_msg_data[0];

    switch (pst_msg->st_msg_hdr.en_msg_type) {
        case WAL_MSG_TYPE_QUERY:

            ul_ret = wal_config_process_query(pst_mac_vap, puc_msg_data, us_msg_len, puc_rsp_msg_data, &uc_rsp_len);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "{wal_config_process_pkt::wal_config_process_query return error code %d!}\r\n",
                                 ul_ret);
                return ul_ret;
            }
            break;

        case WAL_MSG_TYPE_WRITE:
            ul_ret = wal_config_process_write(pst_mac_vap, puc_msg_data, us_msg_len, puc_rsp_msg_data, &uc_rsp_len);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "{wal_config_process_pkt::wal_config_process_write return error code %d!}\r\n",
                                 ul_ret);
                return ul_ret;
            }
            break;

        default:
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_config_process_pkt::pst_msg->st_msg_hdr.en_msg_type error, msg_type is %d!}\r\n",
                             pst_msg->st_msg_hdr.en_msg_type);

            return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* response 长度要包含头长 */
    uc_rsp_toal_len = uc_rsp_len + OAL_SIZEOF(wal_msg_hdr_stru);
    if (oal_unlikely(uc_rsp_toal_len > HMAC_RSP_MSG_MAX_LEN)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{wal_config_process_pkt::invaild response len %u!}\r\n",
                       uc_rsp_toal_len);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 填充返回消息头 */
    pst_rsp_msg->st_msg_hdr.en_msg_type = WAL_MSG_TYPE_RESPONSE;
    pst_rsp_msg->st_msg_hdr.uc_msg_sn = pst_msg->st_msg_hdr.uc_msg_sn;
    pst_rsp_msg->st_msg_hdr.us_msg_len = uc_rsp_len;

    if (ul_request_address) {
        /* need response */
        uint8_t *pst_rsp_msg_tmp = oal_memalloc(uc_rsp_toal_len);
        if (pst_rsp_msg_tmp == NULL) {
            /* no mem */
            oam_error_log1(0, OAM_SF_ANY,
                           "{wal_config_process_pkt::wal_config_process_pkt msg alloc %u failed!",
                           uc_rsp_toal_len);
            wal_set_msg_response_by_addr(ul_request_address, NULL, OAL_ERR_CODE_PTR_NULL, uc_rsp_toal_len);
        } else {
            memcpy_s((void *)pst_rsp_msg_tmp, uc_rsp_toal_len, (void *)ac_rsp_msg, uc_rsp_toal_len);
            if (OAL_SUCC != wal_set_msg_response_by_addr(ul_request_address, (void *)pst_rsp_msg_tmp,
                OAL_SUCC, uc_rsp_toal_len)) {
                oam_error_log0(0, OAM_SF_ANY,
                               "{wal_config_process_pkt::wal_config_process_pkt did't found the request msg!");
                oal_free(pst_rsp_msg_tmp);
            }
        }
    }

    /* 唤醒WAL等待的进程 */
    wal_cfg_msg_task_sched();

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_add_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_11i_add_key(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_get_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    /* 通过函数调用，hmac具体实现 */
    return (hmac_config_11i_get_key(pst_mac_vap, us_len, puc_param));
}


OAL_STATIC uint32_t wal_config_remove_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    /* 通过函数调用，hmac具体实现 */
    return (hmac_config_11i_remove_key(pst_mac_vap, us_len, puc_param));
}


OAL_STATIC uint32_t wal_config_set_default_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    /* 通过函数调用，hmac具体实现 */
    return (hmac_config_11i_set_default_key(pst_mac_vap, us_len, puc_param));
}


uint32_t wal_config_set_wps_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_wps_ie(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_set_wps_p2p_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_wps_p2p_ie(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_open_wmm(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_open_wmm::pst_mac_vap/puc_param is null ptr %x, %x!}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 针对配置vap做保护 */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_CONFIG) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_open_wmm::this is config vap! can't get info.}");
        return OAL_FAIL;
    }

    return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_WMM_SWITCH, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_vowifi_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_vowifi_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_update_ip_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_update_ip_filter(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t  wal_config_set_probe_resp_mode(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap == OAL_PTR_NULL) || (puc_param == OAL_PTR_NULL)) {
        oam_error_log2(0, OAM_SF_TX, "{wal_config_set_probe_resp_mode::pst_mac_vap[%p] or puc_param[%p] is NULL.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    return hmac_config_set_probe_resp_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_version(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_version(pst_mac_vap, us_len, puc_param);
}



OAL_STATIC uint32_t wal_config_blacklist_add(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_blacklist_add::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    return hmac_config_blacklist_add(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_blacklist_add_only(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_blacklist_add_only::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    return hmac_config_blacklist_add_only(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_blacklist_del(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_blacklist_del::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    return hmac_config_blacklist_del(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_blacklist_clr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_blacklist_clr::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }

    if (EOK != memcpy_s(puc_param, OAL_MAC_ADDR_LEN, BROADCAST_MACADDR, OAL_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_config_blacklist_clr::memcpy fail!");
        return OAL_FAIL;
    }

    return hmac_config_blacklist_del(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_get_blacklist_mode::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    *pus_len = OAL_SIZEOF(int32_t);

    return hmac_config_get_blacklist_mode(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_set_blacklist_mode::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    return hmac_config_set_blacklist_mode(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_blacklist_show(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_config_blacklist_show::not valid vap mode=%d!}\r\n",
                         pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }
    return hmac_config_show_blacklist(pst_mac_vap, us_len, puc_param);
}


int32_t wal_recv_config_cmd(uint8_t *puc_buf, uint16_t us_len)
{
    int8_t ac_vap_name[OAL_IF_NAME_SIZE];
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;
    wal_msg_stru *pst_msg = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    uint16_t us_netbuf_len; /* 传给sdt的skb数据区不包括头尾空间的长度 */
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rep_hdr *pst_rep_hdr = NULL;

    uint16_t us_msg_size = us_len;
    uint16_t us_need_response = OAL_FALSE;
    int32_t l_ret;

    DECLARE_WAL_MSG_REQ_STRU(st_msg_request);

    WAL_MSG_REQ_STRU_INIT(st_msg_request);

    l_ret = memcpy_s(ac_vap_name, OAL_IF_NAME_SIZE, puc_buf, OAL_IF_NAME_SIZE);
    ac_vap_name[OAL_IF_NAME_SIZE - 1] = '\0'; /* 防止字符串异常 */

    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(ac_vap_name, OAL_STRLEN(ac_vap_name));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_recv_config_cmd::wal_config_get_netdev return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_dev_put(pst_net_dev); /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */

    pst_mac_vap = oal_net_dev_priv(pst_net_dev); /* 获取mac vap */
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_recv_config_cmd::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr.}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (us_msg_size < OAL_IF_NAME_SIZE) { // 确保大于0为合法值，防止减下溢，导致后续申请内存以及拷贝发生堆溢出
        oam_error_log1(0, OAM_SF_ANY, "{wal_recv_config_cmd::msg_size[%d] overrun!}", us_msg_size);
        return OAL_FAIL;
    }

    us_msg_size -= OAL_IF_NAME_SIZE;
    /* 申请内存 */
    pst_event_mem = frw_event_alloc_m(us_msg_size + OAL_SIZEOF(wal_msg_rep_hdr));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{wal_recv_config_cmd::request %d mem failed}\r\n", us_msg_size);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_HOST_CRX,
                       WAL_HOST_CRX_SUBTYPE_CFG, (uint16_t)(us_msg_size),
                       FRW_EVENT_PIPELINE_STAGE_0, pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 填写事件payload */
    l_ret += memcpy_s(frw_get_event_payload(pst_event_mem) + OAL_SIZEOF(wal_msg_rep_hdr), us_msg_size,
                      puc_buf + OAL_IF_NAME_SIZE, us_msg_size);
    pst_msg = (wal_msg_stru *)(puc_buf + OAL_IF_NAME_SIZE);
    pst_rep_hdr = (wal_msg_rep_hdr *)pst_event->auc_event_data;

    WAL_RECV_CMD_NEED_RESP(pst_msg, us_need_response);

    if (us_need_response == OAL_TRUE) {
        pst_rep_hdr->ul_request_address = (uintptr_t)&st_msg_request;
        wal_msg_request_add_queue(&st_msg_request);
    } else {
        pst_rep_hdr->ul_request_address = 0;
    }

    ul_ret = wal_config_process_pkt(pst_event_mem);

    if (us_need_response == OAL_TRUE) {
        wal_msg_request_remove_queue(&st_msg_request);
    }

    if (ul_ret != OAL_SUCC) {
        frw_event_free_m(pst_event_mem);
        WAL_MSG_REQ_RESP_MEM_FREE(st_msg_request);
        return (int32_t)ul_ret;
    }

    /* 释放内存 */
    frw_event_free_m(pst_event_mem);

    /* 如果是查询消息类型，结果上报 */
    if (us_need_response == OAL_TRUE) {
        if (oal_unlikely(g_oam_sdt_func_hook.p_sdt_report_data_func == OAL_PTR_NULL)) {
            WAL_MSG_REQ_RESP_MEM_FREE(st_msg_request);
            return OAL_ERR_CODE_PTR_NULL;
        }

        if (st_msg_request.pst_resp_mem == NULL) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_recv_config_cmd::get response ptr failed!}\r\n");
            return (int32_t)ul_ret;
        }

        pst_rsp_msg = (wal_msg_stru *)st_msg_request.pst_resp_mem;

        us_netbuf_len = pst_rsp_msg->st_msg_hdr.us_msg_len + 1; /* +1是sdt工具的需要 */

        us_netbuf_len = (us_netbuf_len > WLAN_SDT_NETBUF_MAX_PAYLOAD) ? WLAN_SDT_NETBUF_MAX_PAYLOAD : us_netbuf_len;

        pst_netbuf = oam_alloc_data2sdt(us_netbuf_len);
        if (pst_netbuf == OAL_PTR_NULL) {
            WAL_MSG_REQ_RESP_MEM_FREE(st_msg_request);
            return OAL_ERR_CODE_PTR_NULL;
        }

        oal_netbuf_data(pst_netbuf)[0] = 'M'; /* sdt需要 */
        l_ret += memcpy_s(oal_netbuf_data(pst_netbuf) + 1, us_netbuf_len - 1,
                          (uint8_t *)pst_rsp_msg->auc_msg_data, us_netbuf_len - 1);
        if (l_ret != EOK) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_recv_config_cmd::memcpy fail!");
        }

        oal_free(st_msg_request.pst_resp_mem);

        oam_report_data2sdt(pst_netbuf, OAM_DATA_TYPE_CFG, OAM_PRIMID_TYPE_DEV_ACK);
    }

    return OAL_SUCC;
}


int32_t wal_recv_memory_cmd(uint8_t *puc_buf, uint16_t us_len)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    wal_sdt_mem_frame_stru *pst_mem_frame;
    uintptr_t ul_mem_addr;           /* 读取内存地址 */
    uint16_t us_mem_len;          /* 需要读取的长度 */
    uint8_t uc_offload_core_mode; /* offload下，表示哪一个核 */
    int32_t l_ret = EOK;

    pst_mem_frame = (wal_sdt_mem_frame_stru *)puc_buf;
    ul_mem_addr = pst_mem_frame->ul_addr;
    us_mem_len = pst_mem_frame->us_len;
    uc_offload_core_mode = pst_mem_frame->en_offload_core_mode;

    if (uc_offload_core_mode == WAL_OFFLOAD_CORE_MODE_DMAC) {
        /* 如果是offload情形，并且要读取的内存是wifi芯片侧，需要抛事件，后续开发 */
        return OAL_SUCC;
    }

    if (ul_mem_addr == OAL_PTR_NULL) { /* 读写地址不合理 */
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (us_mem_len > WAL_SDT_MEM_MAX_LEN) {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    if (pst_mem_frame->uc_mode == MAC_SDT_MODE_READ) {
        if (oal_unlikely(g_oam_sdt_func_hook.p_sdt_report_data_func == OAL_PTR_NULL)) {
            return OAL_ERR_CODE_PTR_NULL;
        }

        pst_netbuf = oam_alloc_data2sdt(us_mem_len);
        if (pst_netbuf == OAL_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }
        l_ret += memcpy_s (oal_netbuf_data(pst_netbuf), us_mem_len, (void *)ul_mem_addr, us_mem_len);
        oam_report_data2sdt(pst_netbuf, OAM_DATA_TYPE_MEM_RW, OAM_PRIMID_TYPE_DEV_ACK);
    } else if (pst_mem_frame->uc_mode == MAC_SDT_MODE_WRITE) {
        l_ret += memcpy_s ((void *)ul_mem_addr, us_mem_len, pst_mem_frame->auc_data, us_mem_len);
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_recv_memory_cmd::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


int32_t wal_parse_global_var_cmd(wal_sdt_global_var_stru *pst_global_frame, unsigned long ul_global_var_addr)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    uint16_t us_skb_len;
    int32_t l_ret = EOK;

    if (pst_global_frame->uc_mode == MAC_SDT_MODE_WRITE) {
        l_ret += memcpy_s((void *)(uintptr_t)ul_global_var_addr, pst_global_frame->us_len,
                          (void *)(pst_global_frame->auc_global_value), pst_global_frame->us_len);
    } else if (pst_global_frame->uc_mode == MAC_SDT_MODE_READ) {
        if (oal_unlikely(g_oam_sdt_func_hook.p_sdt_report_data_func == OAL_PTR_NULL)) {
            return OAL_ERR_CODE_PTR_NULL;
        }

        us_skb_len = pst_global_frame->us_len;

        us_skb_len = (us_skb_len > WLAN_SDT_NETBUF_MAX_PAYLOAD) ? WLAN_SDT_NETBUF_MAX_PAYLOAD : us_skb_len;

        pst_netbuf = oam_alloc_data2sdt(us_skb_len);
        if (pst_netbuf == OAL_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }

        l_ret += memcpy_s(oal_netbuf_data(pst_netbuf), us_skb_len,
                          (void *)(uintptr_t)ul_global_var_addr, us_skb_len);
        oam_report_data2sdt(pst_netbuf, OAM_DATA_TYPE_MEM_RW, OAM_PRIMID_TYPE_DEV_ACK);
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_parse_global_var_cmd::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


int32_t wal_recv_global_var_cmd(uint8_t *puc_buf, uint16_t us_len)
{
    wal_sdt_global_var_stru *pst_global_frame = OAL_PTR_NULL;
    unsigned long ul_global_var_addr;

    if (puc_buf == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_global_frame = (wal_sdt_global_var_stru *)puc_buf;

    if (pst_global_frame->en_offload_core_mode == WAL_OFFLOAD_CORE_MODE_DMAC) {
        /* offload情形，并且要读取的全局变量在wifi芯片侧，需要抛事件，后续开发 */
        return OAL_SUCC;
    }

    ul_global_var_addr = oal_kallsyms_lookup_name(pst_global_frame->auc_global_value_name);
    if (ul_global_var_addr == 0) { /* not found */
        oam_warning_log0(0, OAM_SF_ANY,
                         "{wal_recv_global_var_cmd::kernel lookup global var address returns 0!}\r\n");
        return OAL_FAIL;
    }

    return wal_parse_global_var_cmd(pst_global_frame, ul_global_var_addr);
}


int32_t wal_recv_reg_cmd(uint8_t *puc_buf, uint16_t us_len)
{
    int8_t ac_vap_name[OAL_IF_NAME_SIZE];
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    wal_sdt_reg_frame_stru *pst_reg_frame = OAL_PTR_NULL;
    int32_t l_ret;
    hmac_vap_cfg_priv_stru *pst_cfg_priv = OAL_PTR_NULL;
    oal_netbuf_stru *pst_net_buf = OAL_PTR_NULL;
    uint32_t ul_ret;
    int32_t l_memcpy_ret;

    l_memcpy_ret = memcpy_s(ac_vap_name, OAL_IF_NAME_SIZE, puc_buf, OAL_IF_NAME_SIZE);
    ac_vap_name[OAL_IF_NAME_SIZE - 1] = '\0'; /* 防止字符串异常 */

    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(ac_vap_name, OAL_STRLEN(ac_vap_name));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_recv_reg_cmd::wal_config_get_netdev return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_dev_put(pst_net_dev); /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */

    pst_mac_vap = oal_net_dev_priv(pst_net_dev); /* 获取mac vap */

    ul_ret = hmac_vap_get_priv_cfg(pst_mac_vap, &pst_cfg_priv); /* 取配置私有结构体 */
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_recv_reg_cmd::hmac_vap_get_priv_cfg return null_ptr_err!}\r\n");
        return (int32_t)ul_ret;
    }

    pst_cfg_priv->en_wait_ack_for_sdt_reg = OAL_FALSE;

    ul_ret = hmac_sdt_recv_reg_cmd(pst_mac_vap, puc_buf, us_len);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_recv_reg_cmd::hmac_sdt_recv_reg_cmd return error code!}\r\n");

        return (int32_t)ul_ret;
    }

    pst_reg_frame = (wal_sdt_reg_frame_stru *)puc_buf;

    if (pst_reg_frame->uc_mode == MAC_SDT_MODE_READ || pst_reg_frame->uc_mode == MAC_SDT_MODE_READ16) {
        wal_wake_lock();
        /*lint -e730*/ /* info, boolean argument to function */
        l_ret = oal_wait_event_interruptible_timeout_m(pst_cfg_priv->st_wait_queue_for_sdt_reg,
            OAL_TRUE == pst_cfg_priv->en_wait_ack_for_sdt_reg, (2 * OAL_TIME_HZ)); /* 2 超时时间 */
        /*lint +e730*/
        if (l_ret == 0) {
            /* 超时 */
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_recv_reg_cmd::wal_netdev_open: wait queue timeout!}\r\n");
            wal_wake_unlock();
            return -OAL_EINVAL;
        }
        else if (l_ret < 0) {
            /* 异常 */
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{wal_recv_reg_cmd::wal_netdev_open: wait queue error!}\r\n");
            wal_wake_unlock();
            return -OAL_EINVAL;
        }
        wal_wake_unlock();
        /*lint +e774*/
        /* 读取返回的寄存器值 */
        pst_reg_frame->ul_reg_val = *((uint32_t *)(pst_cfg_priv->ac_rsp_msg));

        if (oal_unlikely(g_oam_sdt_func_hook.p_sdt_report_data_func != OAL_PTR_NULL)) {
            pst_net_buf = oam_alloc_data2sdt((uint16_t)OAL_SIZEOF(wal_sdt_reg_frame_stru));
            if (pst_net_buf == OAL_PTR_NULL) {
                return OAL_ERR_CODE_PTR_NULL;
            }

            l_memcpy_ret += memcpy_s(oal_netbuf_data(pst_net_buf), (uint16_t)OAL_SIZEOF(wal_sdt_reg_frame_stru),
                                     (uint8_t *)pst_reg_frame, (uint16_t)OAL_SIZEOF(wal_sdt_reg_frame_stru));

            oam_report_data2sdt(pst_net_buf, OAM_DATA_TYPE_REG_RW, OAM_PRIMID_TYPE_DEV_ACK);
        }
    }

    if (l_memcpy_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_recv_reg_cmd::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)

int32_t wal_recv_sample_cmd(uint8_t *puc_buf, uint16_t us_len)
{
    int8_t ac_vap_name[OAL_IF_NAME_SIZE];
    oal_net_device_stru *pst_net_dev;
    mac_vap_stru *pst_mac_vap;

    if (EOK != memcpy_s(ac_vap_name, OAL_IF_NAME_SIZE, puc_buf, OAL_IF_NAME_SIZE)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_recv_sample_cmd::memcpy fail!");
        return OAL_FAIL;
    }
    ac_vap_name[OAL_IF_NAME_SIZE - 1] = '\0'; /* 防止字符串异常 */

    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(ac_vap_name, OAL_STRLEN(ac_vap_name));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_recv_sample_cmd::wal_config_get_netdev return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_dev_put(pst_net_dev); /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev); /* 获取mac vap */

    hmac_sdt_recv_sample_cmd(pst_mac_vap, puc_buf, us_len);
    return OAL_SUCC;
}


uint32_t wal_sample_report2sdt(frw_event_mem_stru *pst_event_mem)
{
    uint16_t us_payload_len;
    oal_netbuf_stru *pst_net_buf;
    frw_event_stru *pst_event;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_sample_report2sdt::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    us_payload_len = pst_event->st_event_hdr.us_length - FRW_EVENT_HDR_LEN;

    pst_net_buf = oam_alloc_data2sdt(us_payload_len);
    if (pst_net_buf == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (EOK != memcpy_s(oal_netbuf_data(pst_net_buf), us_payload_len, pst_event->auc_event_data, us_payload_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_sample_report2sdt::memcpy fail!");
        oal_mem_sdt_netbuf_free(pst_net_buf, OAL_TRUE);
        return OAL_FAIL;
    }

    oam_report_data2sdt(pst_net_buf, OAM_DATA_TYPE_SAMPLE, OAM_PRIMID_TYPE_DEV_ACK);
    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_ONLINE_DPD

uint32_t wal_dpd_report2sdt(frw_event_mem_stru *pst_event_mem)
{
    uint16_t us_payload_len;
    oal_netbuf_stru *pst_net_buf;
    frw_event_stru *pst_event;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_sample_report2sdt::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    us_payload_len = pst_event->st_event_hdr.us_length - FRW_EVENT_HDR_LEN;

    pst_net_buf = oam_alloc_data2sdt(us_payload_len);
    if (pst_net_buf == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (EOK != memcpy_s(oal_netbuf_data(pst_net_buf), us_payload_len, pst_event->auc_event_data, us_payload_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_dpd_report2sdt::memcpy fail!");
        oal_mem_sdt_netbuf_free(pst_net_buf, OAL_TRUE);
        return OAL_FAIL;
    }

    oam_report_data2sdt(pst_net_buf, OAM_DATA_TYPE_DPD, OAM_PRIMID_TYPE_DEV_ACK);
    return OAL_SUCC;
}

#endif

void wal_drv_cfg_func_hook_init(void)
{
    g_st_wal_drv_func_hook.p_wal_recv_cfg_data_func = wal_recv_config_cmd;
    g_st_wal_drv_func_hook.p_wal_recv_mem_data_func = wal_recv_memory_cmd;
    g_st_wal_drv_func_hook.p_wal_recv_reg_data_func = wal_recv_reg_cmd;
    g_st_wal_drv_func_hook.p_wal_recv_global_var_func = wal_recv_global_var_cmd;
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    g_st_wal_drv_func_hook.p_wal_recv_sample_data_func = wal_recv_sample_cmd;
#endif
}


OAL_STATIC uint32_t wal_config_set_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_pmksa(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_del_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_del_pmksa(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_flush_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_flush_pmksa(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_remain_on_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_remain_on_channel(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_cancel_remain_on_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_cancel_remain_on_channel(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_SAE

OAL_STATIC uint32_t wal_config_external_auth(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_external_auth(pst_mac_vap, us_len, puc_param);
}
#endif /* _PRE_WLAN_FEATURE_SAE */


OAL_STATIC uint32_t wal_config_set_sta_pm_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_ps_mode_param_stru *pst_ps_mode_param = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_ps_mode_param = (mac_cfg_ps_mode_param_stru *)puc_param;
    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hmac_vap->uc_ps_mode = pst_ps_mode_param->uc_vap_ps_mode;
    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                     "wal_config_set_sta_pm_mode ps_mode[%d]",
                     pst_hmac_vap->uc_ps_mode);

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if ((pst_hmac_vap->uc_ps_mode == MAX_FAST_PS) || (pst_hmac_vap->uc_ps_mode == AUTO_FAST_PS)) {
        wlan_pm_set_timeout((g_wlan_min_fast_ps_idle > 1) ?
                                (g_wlan_min_fast_ps_idle - 1) : g_wlan_min_fast_ps_idle);
    } else
#endif
    {
        wlan_pm_set_timeout(WLAN_SLEEP_DEFAULT_CHECK_CNT);
    }
    return hmac_config_set_sta_pm_mode(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_show_pm_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        oam_warning_log0(0, OAM_SF_PWR, "{wal_config_show_pm_info::ap mode has no pm fsm.}");
        return OAL_FAIL;
    }
    return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_SHOW_PS_INFO, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_sta_pm_on(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_cfg_ps_open_stru *pst_sta_pm_open = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_PWR, "{wal_config_set_sta_pm_on::pst_mac_vap / puc_param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                         "{wal_config_set_sta_pm_on::pst_hmac_vap null,vap state[%d].}",
                         pst_mac_vap->en_vap_state);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)puc_param;

    /* 如果上层主动dhcp成功此时取消超时开低功耗的定时器 */
    if ((pst_hmac_vap->st_ps_sw_timer.en_is_registerd == OAL_TRUE) &&
        (pst_sta_pm_open->uc_pm_enable > MAC_STA_PM_SWITCH_OFF)) {
        frw_timer_immediate_destroy_timer_m(&(pst_hmac_vap->st_ps_sw_timer));
    }

    return hmac_config_set_sta_pm_on(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_fast_sleep_para(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_fast_sleep_para(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
OAL_STATIC uint32_t wal_config_set_rx_listen_ps_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->rx_listen_ps_is_open) {
        return hmac_config_set_rx_listen_ps_switch(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_config_set_uapsd_para(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    return hmac_config_set_uapsd_para(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_mgmt_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_wpas_mgmt_tx(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_query_station_stats(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_station_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_query_rssi(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_rssi(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_query_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_rate(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_query_psst(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_psst(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_query_psm_flt_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_psm_flt_stat(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_DFT_STAT

OAL_STATIC uint32_t wal_config_query_ani(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_query_ani(pst_mac_vap, us_len, puc_param);
}
#endif

#ifdef _PRE_WLAN_FEATURE_HS20

OAL_STATIC uint32_t wal_config_set_qos_map(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_qos_map(pst_mac_vap, us_len, puc_param);
}
#endif

OAL_STATIC uint32_t  wal_config_set_dc_status(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_dc_status(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_p2p_miracast_status(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_p2p_miracast_status(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_p2p_ps_ops(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_p2p_ps_ops(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_p2p_ps_noa(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_p2p_ps_noa(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_cfg_vap_h2d(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_cfg_vap_h2d(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_host_dev_init(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_host_dev_init(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_host_dev_exit(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_host_dev_exit(pst_mac_vap);
}

uint32_t wal_send_cali_matrix_data(oal_net_device_stru *pst_net_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    mac_vap_stru *pst_mac_vap;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oal_io_print("wal_send_cali_matrix_data:netdev name %s not have dev priv-vap", pst_net_dev->name);
        oam_warning_log1(0, OAM_SF_CALIBRATE,
                         "wal_send_cali_matrix_data:netdev[%p] have no dev priv-vap",
                         (uintptr_t)pst_net_dev);

        return OAL_FAIL;
    }
    wlan_chip_send_cali_matrix_data(pst_mac_vap);
#endif
    return OAL_SUCC;
}

uint32_t wal_send_cali_data(oal_net_device_stru *pst_net_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    mac_vap_stru *pst_mac_vap;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap != OAL_PTR_NULL) {
        wlan_chip_send_cali_data(pst_mac_vap);
    } else {
        oal_io_print("wal_send_cali_data:netdev name %s not have dev priv-vap", pst_net_dev->name);
        oam_warning_log1(0, OAM_SF_CALIBRATE,
                         "wal_send_cali_data:netdev[%p] have no dev priv-vap",
                         (uintptr_t)pst_net_dev);

        return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_set_max_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_max_user;

    ul_max_user = *((uint32_t *)puc_param);

    return hmac_config_set_max_user(pst_mac_vap, ul_max_user);
}

#ifdef _PRE_WLAN_FEATURE_11R

OAL_STATIC uint32_t wal_config_set_ft_ies(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_ft_ies(pst_mac_vap, us_len, puc_param);
}
#endif  // _PRE_WLAN_FEATURE_11R

OAL_STATIC uint32_t wal_config_get_dieid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_dieid(pst_mac_vap, us_len, puc_param);
}
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC uint32_t wal_config_set_all_log_level(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_all_log_level(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_cus_rf(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_cus_rf(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_cus_dts_cali(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_cus_dts_cali(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_cus_nvram_params(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_cus_nvram_params(pst_mac_vap, us_len, puc_param);
}

/* show dev customize info */
OAL_STATIC uint32_t wal_config_dev_customize_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_dev_customize_info(pst_mac_vap, us_len, puc_param);
}
uint32_t wal_send_custom_data(mac_vap_stru *pst_mac_vap, uint16_t len, uint8_t *param,
    uint16_t syn_id)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (pst_mac_vap != OAL_PTR_NULL) {
        return hmac_send_custom_data(pst_mac_vap, len, param, syn_id);
    } else {
        oam_warning_log0(0, OAM_SF_CALIBRATE, "wal_send_custom_data:mac vap is NULL");
        return OAL_FAIL;
    }
#endif
}


OAL_STATIC uint32_t wal_config_set_cus_dyn_cali(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    return hmac_config_set_cus_dyn_cali(mac_vap, len, param);
}

#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */


uint32_t wal_config_set_vendor_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_vendor_ie(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_get_sta_11h_abillty(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_sta_11h_abillty(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_mlme(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_mlme(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_vap_destroy(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    hmac_vap_stru *pst_hmac_vap;
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);

    return hmac_vap_destroy(pst_hmac_vap);
}

#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC uint32_t wal_config_send_neighbor_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_send_neighbor_req(pst_mac_vap, us_len, puc_param);
}
#endif  // _PRE_WLAN_FEATURE_11K

OAL_STATIC uint32_t wal_config_vendor_cmd_get_channel_list(mac_vap_stru *pst_mac_vap,
                                                             uint16_t *pus_len,
                                                             uint8_t *puc_param)
{
    return hmac_config_vendor_cmd_get_channel_list(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_wmmswitch(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_wmmswitch(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_vap_wmm_switch(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_vap_wmm_switch(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_vap_wmm_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_config_set_vap_wmm_switch::pst_mac_vap/puc_param is null ptr %x, %x!}\r\n",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 针对配置vap做保护 */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_CONFIG) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_set_vap_wmm_switch::this is config vap! can't get info.}");
        return OAL_FAIL;
    }

    return hmac_config_set_vap_wmm_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_bg_noise(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_bg_noise_info(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_max_user(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_max_user(pst_mac_vap, pus_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_M2S_MSS

OAL_STATIC uint32_t wal_config_set_m2s_switch_blacklist(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_m2s_switch_blacklist(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_m2s_switch_mss(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_m2s_switch_mss(pst_mac_vap, us_len, puc_param);
}

#endif

#ifdef _PRE_WLAN_FEATURE_M2S
#ifdef _PRE_WLAN_FEATURE_M2S_MODEM
OAL_STATIC uint32_t wal_config_set_m2s_switch_modem(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_m2s_modem_is_open) {
        return hmac_config_set_m2s_switch_modem(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif
#endif


#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifndef CONFIG_HAS_EARLYSUSPEND

OAL_STATIC uint32_t wal_config_set_suspend_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_device_stru *pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_config_set_suspend_mode:pst_mac_device is null ptr!");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 0:亮屏 1:暗屏 */
    if (*puc_param == 0) {
        hmac_do_suspend_action(pst_mac_device, OAL_FALSE);
    } else {
        hmac_do_suspend_action(pst_mac_device, OAL_TRUE);
    }

    return OAL_SUCC;
}
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_APF

OAL_STATIC uint32_t wal_config_apf_filter_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_apf_filter_cmd(pst_mac_vap, us_len, puc_param);
}

#endif


OAL_STATIC uint32_t wal_config_remove_app_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_remove_app_ie(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_force_stop_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_force_stop_filter(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_fem_lp_flag(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_fem_lp_flag(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_softap_mimo_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_softap_mimo_mode(pst_mac_vap, us_len, puc_param);
}


uint32_t wal_config_assigned_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_assigned_filter(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_NAN
OAL_STATIC uint32_t wal_config_nan(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    return hmac_config_nan(mac_vap, len, param);
}
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
OAL_STATIC uint32_t wal_config_hid2d_acs_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_hid2d_acs_mode(pst_mac_vap, us_len, puc_param);
}
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
OAL_STATIC uint32_t wal_config_switch_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_hid2d_switch_channel(pst_mac_vap, us_len, puc_param);
}
OAL_STATIC uint32_t wal_config_scan_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_hid2d_scan_channel(pst_mac_vap, us_len, puc_param);
}
#endif
#endif

/*lint -e19*/
oal_module_symbol(wal_drv_cfg_func_hook_init);
oal_module_symbol(wal_config_set_vendor_ie); /*lint +e19*/

