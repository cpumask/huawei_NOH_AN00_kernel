

#ifndef __MAC_FRAME_H__
#define __MAC_FRAME_H__

// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "mac_frame_common.h"

/* 此文件中定义的结构体与协议相关，需要1字节对齐 */
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_H

/* 10 函数声明 */
uint8_t *mac_find_p2p_attribute(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len);
uint8_t *mac_find_ie(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len);
uint8_t *mac_find_ie_sec(uint8_t uc_eid, uint8_t *puc_ies,
                                      int32_t l_len, uint32_t *pul_len);
uint8_t *mac_find_vendor_ie(uint32_t ul_oui,
                                         uint8_t uc_oui_type,
                                         uint8_t *puc_ies,
                                         int32_t l_len);

void mac_set_beacon_interval_field(void *pst_mac_vap, uint8_t *puc_buffer);
void mac_set_cap_info_ap(void *pst_mac_vap, uint8_t *puc_cap_info);
void mac_set_cap_info_sta(void *pst_vap, uint8_t *puc_cap_info);
void mac_set_ssid_ie(void *pst_vap, uint8_t *puc_buffer,
                                    uint8_t *puc_ie_len, uint16_t us_frm_type);

#ifdef _PRE_WLAN_NARROW_BAND
void mac_set_nb_ie(uint8_t *puc_buffer, uint8_t *puc_ie_len);
#endif
uint8_t *mac_get_wmm_ie_ram(uint8_t *puc_beacon_body, uint16_t us_frame_len);

void mac_set_supported_rates_ie(void *pst_vap, uint8_t *puc_buffer,
                                               uint8_t *puc_ie_len);
void mac_set_dsss_params(void *pst_vap, uint8_t *puc_buffer,
                                        uint8_t *puc_ie_len, uint8_t uc_channel_num);
#ifdef _PRE_WLAN_FEATURE_1024QAM
void mac_set_1024qam_vendor_ie(void *pst_vap, uint8_t *puc_buffer,
                                          uint8_t *puc_ie_len);
#endif
#ifdef _PRE_WLAN_FEATURE_PRIV_CLOSED_LOOP_TPC
void mac_set_adjust_pow_vendor_ie(void *pst_vap, uint8_t *puc_buffer,
                                             uint8_t *puc_ie_len);
#endif
#ifdef _PRE_WLAN_FEATURE_11D
void mac_set_country_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#endif
void mac_set_pwrconstraint_ie(void *pst_vap, uint8_t *puc_buffer,
                                             uint8_t *puc_ie_len);
void mac_set_quiet_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t uc_qcount,
                                     uint8_t uc_qperiod, uint16_t us_qdur, uint16_t us_qoffset,
                                     uint8_t *puc_ie_len);

void mac_set_11ntxbf_vendor_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);

void mac_set_erp_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_exsup_rates_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_bssload_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#ifdef _PRE_WLAN_FEATURE_SMPS
uint8_t mac_calc_smps_field(uint8_t en_smps);
#endif
void mac_set_timeout_interval_ie(void *pst_vap,
                                                uint8_t *puc_buffer,
                                                uint8_t *puc_ie_len,
                                                uint32_t ul_type,
                                                uint32_t ul_timeout);
void mac_set_ht_capabilities_ie(void *pst_vap, uint8_t *puc_buffer,
                                               uint8_t *puc_ie_len);
void mac_set_ht_opern_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_obss_scan_params(void *pst_vap, uint8_t *puc_buffer,
                                             uint8_t *puc_ie_len);
void mac_set_ext_capabilities_ie(void *pst_vap, uint8_t *puc_buffer,
                                                uint8_t *puc_ie_len);
void mac_set_tpc_report_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_wmm_params_ie(void *pst_vap, uint8_t *puc_buffer,
                                          oal_bool_enum_uint8 en_is_qos, uint8_t *puc_ie_len);
uint8_t *mac_get_ssid(uint8_t *puc_beacon_body, int32_t l_frame_body_len, uint8_t *puc_ssid_len);
uint16_t mac_get_beacon_period(uint8_t *puc_beacon_body);
uint8_t mac_get_dtim_period(uint8_t *puc_frame_body, uint16_t us_frame_body_len);
uint8_t mac_get_dtim_cnt(uint8_t *puc_frame_body, uint16_t us_frame_body_len);
uint8_t *mac_get_wmm_ie(uint8_t *puc_beacon_body, uint16_t us_frame_len);
void mac_set_power_cap_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_supported_channel_ie(uint8_t *pst_vap, uint8_t *puc_buffer,
                                                 uint8_t *puc_ie_len);
void mac_set_wmm_ie_sta(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#ifdef _PRE_WLAN_FEATURE_WMMAC
uint16_t mac_set_wmmac_ie_sta(uint8_t *pst_vap, uint8_t *puc_buffer,
                                           mac_wmm_tspec_stru *pst_addts_args);
#endif  // _PRE_WLAN_FEATURE_WMMAC
void mac_set_listen_interval_ie(uint8_t *pst_vap, uint8_t *puc_buffer,
                                               uint8_t *puc_ie_len);
void mac_set_rsn_ie(void *pv_mac_vap, uint8_t *puc_pmkid, uint8_t *puc_buffer,
                                   uint8_t *puc_ie_len);
void mac_set_wpa_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_status_code_ie(uint8_t *puc_buffer,
                                           mac_status_code_enum_uint16 en_status_code);
void mac_set_aid_ie(uint8_t *puc_buffer, uint16_t uc_aid);
uint8_t mac_get_bss_type(uint16_t us_cap_info);
uint32_t mac_check_mac_privacy(uint16_t us_cap_info, uint8_t *pst_mac_vap);
oal_bool_enum_uint8 mac_is_wmm_ie(uint8_t *puc_ie);
void mac_set_vht_capabilities_ie(void *pst_vap, uint8_t *puc_buffer,
                                                uint8_t *puc_ie_len);
void mac_set_vht_opern_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#ifdef _PRE_WLAN_FEATURE_11AX
void mac_set_he_capabilities_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_get_htc_uph_om_value(uint8_t uc_nss, uint8_t uc_bw,
    uint8_t uc_mimo_resound, uint8_t uc_ul_mu_disable,
    uint32_t *pul_htc_value);
void mac_get_htc_om_value(uint8_t uc_nss, uint8_t uc_bw,
    uint8_t uc_mimo_resound, uint8_t uc_ul_mu_disable,
    uint32_t *pul_htc_value);
void mac_set_htc_om_field(uint8_t uc_nss, uint8_t uc_bw, uint8_t uc_mimo_resound,
                                     uint32_t *pul_htc_value);
void mac_set_he_operation_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_he_ie_in_assoc_rsp(void *pst_mac_ap, uint16_t us_assoc_id,
                                           uint8_t *puc_asoc_rsp, uint8_t *puc_ie_len);
void mac_set_he_ie_in_beacon(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#endif
void mac_set_hisi_cap_vendor_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
uint32_t mac_get_hisi_cap_vendor_ie(uint8_t *puc_payload, uint32_t ul_msg_len,
    mac_hisi_cap_vendor_ie_stru *pst_hisi_cap_ie);

uint8_t *mac_find_ie_ext_ie(uint8_t uc_eid, uint8_t uc_ext_ie, uint8_t *puc_ies, int32_t l_len);
uint32_t mac_set_csa_bw_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
uint32_t mac_set_csa_ie(uint8_t uc_mode, uint8_t uc_channel,
                                     uint8_t uc_csa_cnt, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#if defined(_PRE_WLAN_FEATURE_11R)
void mac_set_md_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_rde_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_tspec_ie(void *pst_mac_vap, uint8_t *puc_buffer,
                                     uint8_t *puc_ie_len, uint8_t uc_tid);
#endif  // _PRE_WLAN_FEATURE_11R

#if defined(_PRE_WLAN_FEATURE_11K)
void mac_set_rrm_enabled_cap_field(void *pst_vap, uint8_t *puc_buffer,
                                                  uint8_t *puc_ie_len);
#endif

#ifdef _PRE_WLAN_FEATURE_11K
void mac_set_wfa_tpc_report_ie(void *pst_vap, uint8_t *puc_buffer,
                                              uint8_t *puc_ie_len);
#endif

#ifdef _PRE_WLAN_FEATURE_HISTREAM
void mac_set_histream_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#endif  // _PRE_WLAN_FEATURE_HISTREAM

uint32_t mac_rx_report_80211_frame(uint8_t *pst_mac_vap,
                                                uint8_t *pst_rx_cb,
                                                oal_netbuf_stru *pst_netbuf,
                                                oam_ota_type_enum_uint8 en_ota_type);
uint32_t mac_report_80211_frame(uint8_t *puc_mac_vap,
                                         uint8_t *puc_rx_cb,
                                         oal_netbuf_stru *pst_netbuf,
                                         uint8_t *puc_des_addr,
                                         oam_ota_type_enum_uint8 en_ota_type);
void mac_set_ext_capabilities_ftm_twt(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_vht_capinfo_field_extend(void *pst_mac_vap, uint8_t *puc_buffer);
void mac_set_vht_opern_ie_rom_cb(void *pst_vap, uint8_t *puc_buffer,
                                            uint8_t *puc_ie_len);
void mac_add_cowork_ie(void *pst_hmac_sta, uint8_t *puc_buffer, uint16_t *pus_ie_len);

void mac_add_app_ie(void *pst_mac_vap, uint8_t *puc_buffer,
    uint16_t *pus_ie_len, en_app_ie_type_uint8 en_type);
void mac_add_wps_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint16_t *pus_ie_len,
    en_app_ie_type_uint8 en_type);

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
void mac_set_opmode_field(uint8_t *pst_vap, uint8_t *puc_buffer,
    wlan_nss_enum_uint8 en_nss);
void mac_set_opmode_notify_ie(uint8_t *pst_vap, uint8_t *puc_buffer,
    uint8_t *puc_ie_len);
#endif
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
wlan_pmf_cap_status_uint8 mac_get_pmf_cap(uint8_t *puc_ie, uint32_t ul_ie_len);
#endif
uint16_t mac_encap_2040_coext_mgmt(void *pst_mac_vap, oal_netbuf_stru *pst_buffer,
    uint8_t uc_coext_info, uint32_t ul_chan_report);
oal_bool_enum_uint8 mac_frame_is_null_data(oal_netbuf_stru *pst_net_buf);

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
void mac_set_vender_4addr_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
#endif

void mac_set_vendor_vht_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_vendor_novht_ie(void *pst_mac_vap, uint8_t *puc_buffer,
    uint8_t *puc_ie_len, uint8_t en_1024qam_capable);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_frame.h */
