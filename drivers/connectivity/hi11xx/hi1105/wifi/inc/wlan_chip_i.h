

#ifndef __WLAN_CHIP_I_H__
#define __WLAN_CHIP_I_H__

#include "wlan_chip.h"
#include "oneimage.h"

uint32_t wlan_chip_ops_init(void);

OAL_STATIC OAL_INLINE void wlan_chip_host_global_init_param(void)
{
    if (g_wlan_chip_ops->host_global_init_param) {
        g_wlan_chip_ops->host_global_init_param();
    }
}

OAL_STATIC OAL_INLINE void wlan_chip_first_power_on_mark(void)
{
    if (g_wlan_chip_ops->first_power_on_mark) {
        g_wlan_chip_ops->first_power_on_mark();
    }
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_update_aput_160M_enable(oal_bool_enum_uint8 *en_ap_support_160m)
{
    if (g_wlan_chip_ops->update_aput_160M_enable) {
        return g_wlan_chip_ops->update_aput_160M_enable(en_ap_support_160m);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE struct wlan_flow_ctrl_params wlan_chip_get_flow_ctrl_used_mem(void)
{
    struct wlan_flow_ctrl_params flow_ctrl = {0, 0};
    if (g_wlan_chip_ops->get_flow_ctrl_used_mem) {
        return g_wlan_chip_ops->get_flow_ctrl_used_mem();
    }
    return flow_ctrl;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_h2d_cmd_need_filter(uint32_t cmd_id)
{
    if (g_wlan_chip_ops->h2d_cmd_need_filter) {
        return g_wlan_chip_ops->h2d_cmd_need_filter(cmd_id);
    }
    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_force_update_custom_params(void)
{
    if (g_wlan_chip_ops->force_update_custom_params) {
        return g_wlan_chip_ops->force_update_custom_params();
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_init_nvram_main(oal_net_device_stru *cfg_net_dev)
{
    if (g_wlan_chip_ops->init_nvram_main) {
        return g_wlan_chip_ops->init_nvram_main(cfg_net_dev);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE void wlan_chip_cpu_freq_ini_param_init(void)
{
    if (g_wlan_chip_ops->cpu_freq_ini_param_init) {
        g_wlan_chip_ops->cpu_freq_ini_param_init();
    }
}

OAL_STATIC OAL_INLINE void wlan_chip_host_global_ini_param_init(void)
{
    if (g_wlan_chip_ops->host_global_ini_param_init) {
        g_wlan_chip_ops->host_global_ini_param_init();
    }
}

OAL_STATIC OAL_INLINE uint8_t wlan_chip_get_selfstudy_country_flag(void)
{
    if (g_wlan_chip_ops->get_selfstudy_country_flag) {
        return g_wlan_chip_ops->get_selfstudy_country_flag();
    }
    return 0;
}
OAL_STATIC OAL_INLINE uint32_t wlan_chip_custom_cali(void)
{
    if (g_wlan_chip_ops->custom_cali) {
        return g_wlan_chip_ops->custom_cali();
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_get_11ax_switch_mask(void)
{
    if (g_wlan_chip_ops->get_11ax_switch_mask) {
        return g_wlan_chip_ops->get_11ax_switch_mask();
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_get_11ac2g_enable(void)
{
    if (g_wlan_chip_ops->get_11ac2g_enable) {
        return g_wlan_chip_ops->get_11ac2g_enable();
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_get_probe_resp_mode(void)
{
    if (g_wlan_chip_ops->get_probe_resp_mode) {
        return g_wlan_chip_ops->get_probe_resp_mode();
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_update_cfg80211_mgmt_tx_wait_time(uint32_t wait_time)
{
    if (g_wlan_chip_ops->update_cfg80211_mgmt_tx_wait_time) {
        return g_wlan_chip_ops->update_cfg80211_mgmt_tx_wait_time(wait_time);
    }
    return wait_time;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_check_need_setup_ba_session(void)
{
    if (g_wlan_chip_ops->check_need_setup_ba_session) {
        return g_wlan_chip_ops->check_need_setup_ba_session();
    }
    return OAL_FALSE;
}

OAL_STATIC OAL_INLINE void wlan_chip_tx_update_amsdu_num(mac_vap_stru *pst_mac_vap, uint32_t ul_tx_throughput_mbps,
    oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu)
{
    if (g_wlan_chip_ops->tx_update_amsdu_num) {
        g_wlan_chip_ops->tx_update_amsdu_num(pst_mac_vap, ul_tx_throughput_mbps, en_mu_vap_flag, pen_tx_amsdu);
    }
}

OAL_STATIC OAL_INLINE void wlan_chip_ba_rx_hdl_init(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t uc_tid)
{
    if (g_wlan_chip_ops->ba_rx_hdl_init) {
        g_wlan_chip_ops->ba_rx_hdl_init(pst_hmac_vap, pst_hmac_user, uc_tid);
    }
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_check_need_process_bar(void)
{
    if (g_wlan_chip_ops->check_need_process_bar) {
        return g_wlan_chip_ops->check_need_process_bar();
    }
    return OAL_FALSE;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_ba_send_reorder_timeout(hmac_ba_rx_stru *rx_ba, hmac_vap_stru *hmac_vap,
    hmac_ba_alarm_stru *alarm_data, uint32_t *timeout)
{
    if (g_wlan_chip_ops->ba_send_reorder_timeout) {
        return g_wlan_chip_ops->ba_send_reorder_timeout(rx_ba, hmac_vap, alarm_data, timeout);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_ba_need_check_lut_idx(void)
{
    if (g_wlan_chip_ops->ba_need_check_lut_idx) {
        return g_wlan_chip_ops->ba_need_check_lut_idx();
    }
    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_send_cali_matrix_data(mac_vap_stru *mac_vap)
{
    if (g_wlan_chip_ops->send_cali_matrix_data) {
        return g_wlan_chip_ops->send_cali_matrix_data(mac_vap);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_send_cali_data(mac_vap_stru *mac_vap)
{
    if (g_wlan_chip_ops->send_cali_data) {
        return g_wlan_chip_ops->send_cali_data(mac_vap);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_save_cali_event(frw_event_mem_stru *event_mem)
{
    if (g_wlan_chip_ops->save_cali_event) {
        return g_wlan_chip_ops->save_cali_event(event_mem);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE void wlan_chip_mac_vap_init_mib_11ax(mac_vap_stru *mac_vap, uint32_t nss_num)
{
    if (g_wlan_chip_ops->mac_vap_init_mib_11ax) {
        g_wlan_chip_ops->mac_vap_init_mib_11ax(mac_vap, nss_num);
    }
}

OAL_STATIC OAL_INLINE void wlan_chip_mac_mib_set_auth_rsp_time_out(mac_vap_stru *mac_vap)
{
    if (g_wlan_chip_ops->mac_mib_set_auth_rsp_time_out) {
        g_wlan_chip_ops->mac_mib_set_auth_rsp_time_out(mac_vap);
    }
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 wlan_chip_mac_vap_need_set_user_htc_cap(mac_vap_stru *mac_vap)
{
    if (g_wlan_chip_ops->mac_vap_need_set_user_htc_cap) {
        return g_wlan_chip_ops->mac_vap_need_set_user_htc_cap(mac_vap);
    }
    return OAL_FALSE;
}

#endif /* end of wlan_chip_i.h */


