

#ifndef __HMAC_OPMODE_H__
#define __HMAC_OPMODE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "hmac_ext_if.h"
#include "oam_ext_if.h"
#include "mac_resource.h"

#include "mac_user.h"
#include "mac_ie.h"
#include "hmac_vap.h"
#include "hmac_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_OPMODE_H
/* 2 宏定义 */
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_check_opmode_notify(hmac_vap_stru *hmac_vap, uint8_t *mac_hdr,
    uint8_t *payload_offset, uint32_t msg_len, hmac_user_stru *hmac_user);
uint32_t hmac_mgmt_rx_opmode_notify_frame(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf);

#endif /* end of _PRE_WLAN_FEATURE_OPMODE_NOTIFY */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_opmode.h */
