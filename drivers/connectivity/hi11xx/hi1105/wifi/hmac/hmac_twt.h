

#ifndef __HMAC_TWT_H__
#define __HMAC_TWT_H__

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TWT_H

/* 1 其他头文件包含 */
/* 2 宏定义 */
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
#define HMAC_TWT_P2P_RUNNING_VAP_NUM    3

typedef enum {
    HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_0 = 0,
    HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_32 = 1,
    HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_48 = 2,
    HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_64 = 3,

    HMAC_NEXT_TWT_SUBFIELD_SIZE_BUTT
} hmac_next_twt_subfield_size_enum;

/* 9 OTHERS定义 */
/* 10 函数声明 */
#ifdef _PRE_WLAN_FEATURE_TWT
uint32_t hmac_mgmt_tx_twt_setup(hmac_vap_stru *pst_hmac_vap,
                                hmac_user_stru *pst_hmac_user,
                                mac_twt_action_mgmt_args_stru *pst_twt_action_args);
uint32_t hmac_mgmt_tx_twt_teardown(hmac_vap_stru *pst_hmac_vap,
                                   hmac_user_stru *pst_hmac_user,
                                   mac_twt_action_mgmt_args_stru *pst_twt_action_args);
uint32_t hmac_sta_rx_twt_setup_frame(hmac_vap_stru *pst_hmac_vap,
                                     hmac_user_stru *pst_hmac_user,
                                     uint8_t *puc_payload);
uint32_t hmac_sta_rx_twt_teardown_frame(hmac_vap_stru *pst_hmac_vap,
                                        hmac_user_stru *pst_hmac_user,
                                        uint8_t *puc_payload);
uint32_t hmac_sta_rx_twt_information_frame(hmac_vap_stru *pst_hmac_vap,
                                           hmac_user_stru *pst_hmac_user,
                                           uint8_t *puc_payload);
uint32_t hmac_dbac_teardown_twt_session(mac_device_stru *pst_dev);
uint32_t hmac_twt_auto_setup_session(mac_device_stru *pst_device);
uint32_t hmac_get_chip_vap_num(mac_chip_stru *pst_chip);
uint32_t hmac_config_twt_setup_req_auto(mac_vap_stru *pst_mac_vap);
uint32_t hmac_config_twt_teardown_req_auto(mac_vap_stru *pst_mac_vap);
uint32_t hmac_config_twt_setup_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_twt_teardown_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_proc_rx_process_twt_sync_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_mgmt_tx_twt_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
                                 mac_twt_action_mgmt_args_stru *twt_action_args);
uint32_t mac_device_find_up_sta_wlan(mac_device_stru *pst_mac_device,
                                     mac_vap_stru **ppst_mac_vap);
#endif

#endif /* end of hmac_twt.h */
