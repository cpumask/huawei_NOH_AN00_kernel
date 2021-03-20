

#ifndef __HMAC_MGMT_STA_H__
#define __HMAC_MGMT_STA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "hmac_vap.h"
#include "hmac_mgmt_bss_comm.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_STA_H
/* 2 宏定义 */
#define HMAC_WMM_QOS_PARAMS_HDR_LEN   8
#define HMAC_WMM_QOSINFO_AND_RESV_LEN 2
#define HMAC_WMM_AC_PARAMS_RECORD_LEN 4
#define DATARATES_80211G_NUM          12
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 加入请求参数 */
typedef struct {
    mac_bss_dscr_stru st_bss_dscr; /* 要加入的bss网络 */
} hmac_join_req_stru;

/* 认证请求参数 */
typedef struct {
    uint16_t us_timeout;
    uint8_t auc_resv[2];
} hmac_auth_req_stru;

/* 关联请求参数 */
typedef struct {
    uint16_t us_assoc_timeout;
    uint8_t auc_resv[2];
} hmac_asoc_req_stru;

/* 加入结果 */
typedef struct {
    hmac_mgmt_status_enum_uint8 en_result_code;
    uint8_t auc_resv[3];
} hmac_join_rsp_stru;

/* 认证结果 */
typedef struct {
    uint8_t auc_peer_sta_addr[WLAN_MAC_ADDR_LEN]; /* mesh下peer station的地址 */
    uint16_t us_status_code;                      /* 认证结果 */
} hmac_auth_rsp_stru;

/* 去关联原因 */
typedef struct {
    hmac_mgmt_status_enum_uint8 en_disasoc_reason_code;
    uint8_t auc_resv[3];
} hmac_disasoc_rsp_stru;

typedef struct {
    hmac_mgmt_status_enum_uint8 en_result_code;
    uint8_t auc_resv[3];
} hmac_ap_start_rsp_stru;

/* 与dmac层的hi11xx_date_rate_stru g_ast_hi11xx_rates_11g同步修改 */
typedef struct {
    uint8_t uc_expand_rate;         /* 扩展速率集 */
    uint8_t uc_mac_rate;            /* MAC对应速率 */
    uint8_t uc_hal_wlan_rate_index; /* 速率index */
    uint8_t uc_rsv;
} hmac_data_rate_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_sta_wait_join(hmac_vap_stru *pst_sta, void *pst_msg);
uint32_t hmac_sta_wait_auth(hmac_vap_stru *pst_sta, void *pst_msg);
uint32_t hmac_sta_wait_asoc(hmac_vap_stru *pst_sta, void *pst_msg);
uint32_t hmac_sta_wait_asoc_rx(hmac_vap_stru *pst_sta, void *pst_msg);
uint32_t hmac_sta_wait_auth_seq2_rx(hmac_vap_stru *pst_sta, void *pst_msg);
uint32_t hmac_sta_wait_auth_seq4_rx(hmac_vap_stru *pst_sta, void *p_msg);
uint32_t hmac_sta_auth_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param);
uint32_t hmac_sta_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap_sta, void *p_param);
uint32_t hmac_sta_wait_asoc_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param);
void hmac_sta_handle_disassoc_rsp(hmac_vap_stru *pst_hmac_vap,
                                                 uint16_t us_disasoc_reason_code);
/* TBD : create new hmac_p2p.c */
uint32_t hmac_p2p_remain_on_channel(hmac_vap_stru *pst_hmac_vap_sta, void *p_param);
uint32_t hmac_p2p_listen_timeout(hmac_vap_stru *pst_hmac_vap_sta, void *p_param);

uint32_t hmac_sta_not_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap_sta, void *p_param);

uint32_t hmac_sta_get_user_protocol_etc(mac_bss_dscr_stru *pst_bss_dscr,
                                                 wlan_protocol_enum_uint8 *pen_protocol_mode);
void hmac_sta_update_wmm_info(hmac_vap_stru *pst_hmac_vap, mac_user_stru *pst_mac_user,
                                  uint8_t *puc_wmm_ie);
void hmac_update_join_req_params_2040(mac_vap_stru *pst_mac_vap,
                                              mac_bss_dscr_stru *pst_bss_dscr);
uint32_t hmac_sta_up_update_edca_params_machw(hmac_vap_stru *pst_hmac_sta,
                                                           mac_wmm_set_param_type_enum_uint8 en_wmm_set_param_type);
uint32_t hmac_sta_set_txopps_partial_aid(mac_vap_stru *pst_mac_vap);
void hmac_sta_up_update_edca_params(uint8_t *puc_payload,
                                                   uint16_t us_msg_len,
                                                   hmac_vap_stru *pst_hmac_sta,
                                                   uint8_t uc_frame_sub_type,
                                                   hmac_user_stru *pst_hmac_user);
oal_bool_enum_uint8 hmac_is_support_11grate(uint8_t *puc_rates, uint8_t uc_rate_num);
oal_bool_enum_uint8 hmac_is_support_11brate(uint8_t *puc_rates, uint8_t uc_rate_num);
uint32_t hmac_process_assoc_rsp(hmac_vap_stru *pst_hmac_sta,
                                             hmac_user_stru *pst_hmac_user,
                                             uint8_t *puc_mac_hdr,
                                             uint16_t us_hdr_len,
                                             uint8_t *puc_payload,
                                             uint16_t us_msg_len);
uint32_t hmac_sta_sync_vap(hmac_vap_stru *pst_hmac_vap, mac_channel_stru *pst_channel,
                                    wlan_protocol_enum_uint8 en_protocol);
uint8_t *hmac_sta_find_ie_in_probe_rsp(mac_vap_stru *pst_mac_vap, uint8_t uc_eid,
                                                    uint16_t *pus_index);
uint32_t hmac_sta_get_min_rate(dmac_set_rate_stru *pst_rate_params,
                                        hmac_join_req_stru *pst_join_req);

void hmac_mgmt_sta_roam_rx_action(hmac_vap_stru *p_hmac_vap, oal_netbuf_stru *p_netbuf,
    oal_bool_enum_uint8 en_is_protected);

#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_proc_he_uora_para_update(mac_vap_stru *mac_vap, uint8_t uora_para_change);
uint32_t hmac_sta_up_update_mu_edca_params_machw(hmac_vap_stru *pst_hmac_sta,
                                                 mac_wmm_set_param_type_enum_uint8 en_wmm_set_param_type);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_mgmt_sta.h */
