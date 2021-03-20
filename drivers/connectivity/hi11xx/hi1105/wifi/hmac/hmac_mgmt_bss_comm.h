

#ifndef __HMAC_MGMT_BSS_COMM_H__
#define __HMAC_MGMT_BSS_COMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "mac_frame.h"

#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_BSS_COMM_H
/* 2 宏定义 */
/* HMAC_NCW_INHIBIT_THRED_TIME时间内连续HMAC_RECEIVE_NCW_MAX_CNT次接收到ncw,不上报 */
#define HMAC_NCW_INHIBIT_THRED_TIME 60000 /* 单位ms */
#define HMAC_RECEIVE_NCW_THRED_CNT  6

#define HMAC_FTM_SEND_BUF_LEN 200
#define HMAC_CSI_SEND_BUF_LEN 3000

#define HMAC_80211_FC_AND_DID_LEN 4 /* Frame Control and Duration ID */
#define MAC_LEN 6

/* 3 枚举定义 */
/* 4 全局变量声明 */
extern uint8_t g_auc_avail_protocol_mode[WLAN_PROTOCOL_BUTT][WLAN_PROTOCOL_BUTT];

extern uint32_t g_ul_print_wakeup_mgmt;

/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
typedef struct {
    uint8_t uc_category;
    uint8_t uc_action_code;
    uint8_t auc_oui[3];
    uint8_t uc_eid;
    uint8_t uc_lenth;

    uint8_t uc_location_type;
    uint8_t auc_mac_server[WLAN_MAC_ADDR_LEN];
    uint8_t auc_mac_client[WLAN_MAC_ADDR_LEN];
    uint8_t auc_payload[4];
} hmac_location_event_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint16_t hmac_mgmt_encap_addba_req(hmac_vap_stru *pst_vap,
                                                uint8_t *puc_data,
                                                dmac_ba_tx_stru *pst_tx_ba,
                                                uint8_t uc_tid);
uint16_t hmac_mgmt_encap_addba_rsp(hmac_vap_stru *pst_vap,
                                                uint8_t *puc_data,
                                                hmac_ba_rx_stru *pst_addba_rsp,
                                                uint8_t uc_tid,
                                                uint8_t uc_status);
uint16_t hmac_mgmt_encap_delba(hmac_vap_stru *pst_vap,
                                            uint8_t *puc_data,
                                            uint8_t *puc_addr,
                                            uint8_t uc_tid,
                                            mac_delba_initiator_enum_uint8 en_initiator,
                                            uint8_t reason);
uint32_t hmac_mgmt_rx_addba_req(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             uint8_t *puc_payload);
uint32_t hmac_mgmt_rx_addba_rsp(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             uint8_t *puc_payload);
uint32_t hmac_mgmt_rx_delba(hmac_vap_stru *pst_hmac_vap,
                                         hmac_user_stru *pst_hmac_user,
                                         uint8_t *puc_payload);
uint32_t hmac_mgmt_tx_addba_req(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             mac_action_mgmt_args_stru *pst_action_args);
uint32_t hmac_mgmt_tx_addba_rsp(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             hmac_ba_rx_stru *pst_ba_rx_info,
                                             uint8_t uc_tid,
                                             uint8_t uc_status);
uint32_t hmac_mgmt_tx_delba(hmac_vap_stru *pst_hmac_vap,
                                         hmac_user_stru *pst_hmac_user,
                                         mac_action_mgmt_args_stru *pst_action_args);
uint32_t hmac_mgmt_tx_addba_timeout(void *arg);
uint32_t hmac_mgmt_tx_ampdu_start(hmac_vap_stru *pst_hmac_vap,
                                               hmac_user_stru *pst_hmac_user,
                                               mac_priv_req_args_stru *pst_priv_req);
uint32_t hmac_mgmt_tx_ampdu_end(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             mac_priv_req_args_stru *pst_priv_req);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
uint32_t hmac_sa_query_interval_timeout(void *p_arg);
void hmac_send_sa_query_rsp(mac_vap_stru *pst_mac_vap, uint8_t *pst_hdr,
                                           oal_bool_enum_uint8 en_is_protected);
uint32_t hmac_start_sa_query(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user,
                                          oal_bool_enum_uint8 en_is_protected);
uint32_t hmac_pmf_check_err_code(mac_user_stru *pst_user_base_info,
                                              oal_bool_enum_uint8 en_is_protected,
                                              uint8_t *puc_mac_hdr);

#endif
uint32_t hmac_tx_mgmt_send_event(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_mgmt_frame,
                                              uint16_t us_frame_len);
void hmac_mgmt_update_assoc_user_qos_table(uint8_t *puc_payload,
                                                          uint16_t ul_msg_len,
                                                          hmac_user_stru *pst_hmac_user);
uint32_t hmac_check_bss_cap_info(uint16_t us_cap_info, mac_vap_stru *pst_mac_vap);

#ifdef _PRE_WLAN_FEATURE_TXBF
void hmac_mgmt_update_11ntxbf_cap(uint8_t *puc_payload,
                                                 hmac_user_stru *pst_hmac_user);
#endif
void hmac_set_user_protocol_mode(mac_vap_stru *pst_mac_vap,
                                                hmac_user_stru *pst_hmac_user);
uint32_t hmac_mgmt_reset_psm(mac_vap_stru *pst_vap, uint16_t us_user_id);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
void hmac_rx_sa_query_req(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf,
                                         oal_bool_enum_uint8 en_is_protected);
void hmac_rx_sa_query_rsp(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf,
                                         oal_bool_enum_uint8 en_is_protected);
#endif
void hmac_send_mgmt_to_host(hmac_vap_stru *pst_hmac_vap,
                                           oal_netbuf_stru *puc_buf,
                                           uint16_t us_len,
                                           int32_t l_freq);

void hmac_rx_mgmt_send_to_host(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
uint32_t hmac_huawei_action_process(hmac_vap_stru *pst_hmac_vap,
                                      oal_netbuf_stru *pst_netbuf,
                                      uint8_t uc_type);
#endif
#ifdef _PRE_WLAN_FEATURE_HS20
uint32_t hmac_interworking_check(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_param);
#endif
uint32_t hmac_mgmt_tx_event_status(mac_vap_stru *pst_mac_vap,
                                                uint8_t uc_len,
                                                uint8_t *puc_param);
void hmac_user_init_rates(hmac_user_stru *pst_hmac_user);
uint8_t hmac_add_user_rates(hmac_user_stru *pst_hmac_user,
                                         uint8_t uc_rates_cnt,
                                         uint8_t *puc_rates);
void hmac_rx_ba_session_decr(hmac_vap_stru *pst_hmac_vap, uint8_t uc_tidno);
void hmac_tx_ba_session_decr(hmac_vap_stru *pst_hmac_vap, uint8_t uc_tidno);
void hmac_vap_set_user_avail_rates(mac_vap_stru *pst_mac_vap,
                                                  hmac_user_stru *pst_hmac_user);
uint32_t hmac_proc_ht_cap_ie(mac_vap_stru *pst_mac_vap,
                                          mac_user_stru *pst_mac_user,
                                          uint8_t *puc_ht_cap_ie);
uint32_t hmac_proc_vht_cap_ie(mac_vap_stru *pst_mac_vap,
                                           hmac_user_stru *pst_hmac_user,
                                           uint8_t *puc_vht_cap_ie);
uint32_t hmac_ie_proc_assoc_user_legacy_rate(uint8_t *puc_payload,
                                                      uint32_t us_rx_len,
                                                      hmac_user_stru *pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_proc_he_cap_ie(mac_vap_stru *pst_mac_vap,
                                      hmac_user_stru *pst_hmac_user,
                                      uint8_t *puc_he_cap_ie);
uint32_t hmac_proc_he_bss_color_change_announcement_ie(mac_vap_stru *pst_mac_vap,
                                                                hmac_user_stru *pst_hmac_user,
                                                                uint8_t *puc_bss_color_ie);
#endif
#ifdef _PRE_WLAN_FEATURE_LOCATION
uint32_t drv_netlink_location_send(void *buff, uint32_t ul_len);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_mgmt_bss_comm.h */
