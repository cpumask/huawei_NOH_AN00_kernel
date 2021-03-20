

#ifndef __HMAC_WMMAC_H__
#define __HMAC_WMMAC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC
/* 1 其他头文件包含 */
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "hmac_device.h"
#include "mac_resource.h"
#include "hmac_resource.h"
#include "mac_vap.h"
#include "mac_ie.h"

#include "hmac_ext_if.h"
#include "hmac_fsm.h"
#include "hmac_main.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WMMAC_H
/* 2 宏定义 */
#define WLAN_ADDTS_TIMEOUT 500

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_mgmt_tx_addts_rsp(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
                                         hmac_ba_rx_stru *pst_ba_rx_info, uint8_t uc_tid, uint8_t uc_status);
uint32_t hmac_mgmt_rx_addts_rsp(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user, uint8_t *puc_payload);
uint32_t hmac_mgmt_rx_delts(hmac_vap_stru *pst_hmac_vap,
                                         hmac_user_stru *pst_hmac_user, uint8_t *puc_payload);
uint32_t hmac_mgmt_rx_addts_req_frame(hmac_vap_stru *pst_hmac_vap,
                                                   oal_netbuf_stru *pst_netbuf);
uint32_t hmac_config_addts_req(mac_vap_stru *pst_mac_vap,
                                            uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_delts(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_wmmac_switch(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len, uint8_t *puc_param);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wmmac.h */
