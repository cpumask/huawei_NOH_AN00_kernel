

#ifndef __HMAC_CONFIG_H__
#define __HMAC_CONFIG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "mac_vap.h"
#include "mac_device.h"
#include "hmac_device.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CONFIG_H

/* 2 宏定义 */
#define HMAC_MAX_MCS_NUM               8 /* 单流和双流支持的mac最大个数 */
#define HAMC_QUERY_INFO_FROM_DMAC_TIME (5 * OAL_TIME_HZ)
#define HMAC_WAIT_EVENT_RSP_TIME 10 /* 等待配置命令事件完成的超时时间(s) */
#define HMAC_S_TO_MS 1000   /* 秒和毫秒的转换系数 */

/* 3 枚举定义 */
/* APUT OWE group definition, hipriv.sh BIT format transit to pst_hmac_vap->owe_group */
#define WAL_HIPRIV_OWE_19 BIT(0)
#define WAL_HIPRIV_OWE_20 BIT(1)
#define WAL_HIPRIV_OWE_21 BIT(2)
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* hmac_vap结构中，一部分成员的大小，这些成员在linux和windows下的定义可能不同 */
typedef struct {
    uint32_t ul_hmac_vap_cfg_priv_stru_size;
    uint32_t ul_frw_timeout_stru_size;
    uint32_t ul_oal_spin_lock_stru_size;
    uint32_t ul_mac_key_mgmt_stru_size;
    uint32_t ul_mac_pmkid_cache_stru_size;
    uint32_t ul_mac_curr_rateset_stru_size;
    uint32_t ul_hmac_vap_stru_size;
} hmac_vap_member_size_stru;

/* HMAC到DMAC配置同步操作 */
typedef struct {
    wlan_cfgid_enum_uint16 en_cfgid;
    uint8_t auc_resv[2];
    uint32_t (*p_set_func)(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);
} hmac_config_syn_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_config_start_vap_event(mac_vap_stru *pst_mac_vap,
                                                  oal_bool_enum_uint8 en_mgmt_rate_init_flag);
uint32_t hmac_set_mode_event(mac_vap_stru *pst_mac_vap);
uint32_t hmac_config_sta_update_rates(mac_vap_stru *pst_mac_vap,
                                                   mac_cfg_mode_param_stru *pst_cfg_mode,
                                                   mac_bss_dscr_stru *pst_bss_dscr);
uint32_t hmac_event_config_syn(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_event_log_syn(frw_event_mem_stru *pst_event_mem);

uint32_t hmac_protection_update_from_user(mac_vap_stru *pst_mac_vap,
                                                   uint16_t us_len,
                                                   uint8_t *puc_param);
uint32_t hmac_40M_intol_sync_event(mac_vap_stru *pst_mac_vap,
                                            uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_tlv_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_set_str_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_set_tx_ampdu_type(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
uint32_t hmac_config_sta_pm_on_syn(mac_vap_stru *pst_mac_vap);
uint32_t hmac_set_ipaddr_timeout(void *puc_para);
uint32_t hmac_config_pm_debug_switch(mac_vap_stru *pst_mac_vap,
                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_ip_addr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_show_arpoffload_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

uint32_t hmac_config_roam_enable(mac_vap_stru *pst_mac_vap,
                                       uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_roam_start(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_11R
uint32_t hmac_config_set_ft_ies(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif  // _PRE_WLAN_FEATURE_11R

uint32_t hmac_config_enable_2040bss(mac_vap_stru *pst_mac_vap,
                                          uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_2040bss_sw(mac_vap_stru *pst_mac_vap,
                                      uint16_t *pus_len, uint8_t *puc_param);
uint32_t hmac_config_get_dieid_rsp(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);
uint32_t hmac_config_get_dieid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_auto_freq_enable(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len, uint8_t *puc_param);

uint32_t hmac_config_set_sta_pm_on(mac_vap_stru *pst_mac_vap,
                                         uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_sta_pm_mode(mac_vap_stru *pst_mac_vap,
                                           uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_fast_sleep_para(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
uint32_t hmac_config_set_rx_listen_ps_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
#endif

uint32_t hmac_config_set_all_log_level(mac_vap_stru *pst_mac_vap,
                                             uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_cus_rf(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_cus_dts_cali(mac_vap_stru *pst_mac_vap,
                                            uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_cus_nvram_params(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_dev_customize_info(mac_vap_stru *pst_mac_vap,
                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_cus_dyn_cali(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_config_set_rf_front_para(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_config_set_rf_cali_para(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_config_set_dev_cap_para(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_config_set_nv_pow_para(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_config_set_nv_dyn_pow_para(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#if defined(_PRE_WLAN_FEATURE_11K)
uint32_t hmac_scan_rrm_proc_save_bss(mac_vap_stru *pst_mac_vap,
                                           uint8_t uc_len, uint8_t *puc_param);
#endif

uint32_t hmac_config_stop_altx(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_FTM
uint32_t  hmac_config_ftm_dbg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t  hmac_ftm_rx_gas_initial_response_frame(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
uint32_t hmac_config_start_ftm_ranging(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
#ifdef _PRE_WLAN_FEATURE_APF
uint32_t hmac_config_apf_filter_cmd(mac_vap_stru *pst_mac_vap,
                                             uint16_t us_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_remove_app_ie(mac_vap_stru *pst_mac_vap,
                                            uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_force_stop_filter(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len, uint8_t *puc_param);

uint32_t hmac_config_fem_lp_flag(mac_vap_stru *pst_mac_vap,
                                          uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_softap_mimo_mode(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_assigned_filter(mac_vap_stru *pst_mac_vap,
                                                  uint16_t us_len, uint8_t *puc_param);

uint32_t hmac_config_set_owe(mac_vap_stru *pst_mac_vap,
                                          uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
void hmac_config_change_wifi_state_for_power_down(void);
#endif

#ifdef _PRE_WLAN_FEATURE_NAN
uint32_t hmac_config_nan(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
uint32_t hmac_config_set_hid2d_acs_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
uint32_t hmac_config_hid2d_switch_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
uint32_t hmac_config_hid2d_scan_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
uint32_t hmac_config_set_hid2d_presentation_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param);
#endif
uint32_t hmac_send_custom_data(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param,
    wlan_cfgid_enum_uint16 syn_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_main */
