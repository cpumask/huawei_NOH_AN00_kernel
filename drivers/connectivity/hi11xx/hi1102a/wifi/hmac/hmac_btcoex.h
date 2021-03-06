
#ifndef __HMAC_BTCOEX_H__
#define __HMAC_BTCOEX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_BTCOEX

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "frw_ext_if.h"
#include "oal_ext_if.h"
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define MAX_BTCOEX_BSS_IN_BL 16

#define HMAC_BTCOEX_GET_BLACKLIST_TYPE(_pst_hmac_user) \
    ((_pst_hmac_user)->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_blacklist_tpye)

#define HMAC_BTCOEX_GET_BLACKLIST_DELBA_HANDLE_ALLOW(_pst_hmac_user) \
    ((_pst_hmac_user)->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow)
typedef enum {
    BTCOEX_BLACKLIST_TPYE_FIX_BASIZE = 0,  // 固定聚合个数
    BTCOEX_BLACKLIST_TPYE_NOT_AGGR = 1,    // 不聚合

    BTCOEX_BLACKLIST_TPYE_BUTT
} btcoex_blacklist_type_enum;
typedef oal_uint8 btcoex_blacklist_type_enum_uint8;

typedef enum {
    BTCOEX_BA_TYPE_NORMAL,  // host侧不做任何处理
    BTCOEX_BA_TYPE_REJECT,  // 拒绝建立BA
    BTCOEX_BA_TYPE_SIZE_1,  // 聚合个数为1
} btcoex_ba_type_enum;
typedef oal_uint8 btcoex_ba_type_enum_uint8;
typedef struct {
    oal_uint16 us_last_baw_start;                       /* 上一次接收到ADDBA REQ中的baw_start值 */
    oal_uint16 us_last_seq_num;                         /* 上一次接收到ADDBA REQ中的seq_num值 */
    btcoex_blacklist_type_enum_uint8 en_blacklist_tpye; /* 黑名单方案 */
    oal_bool_enum_uint8 en_ba_handle_allow;             /* 黑名单方案1:是否允许共存删建聚合 黑名单方案2:是否允许建立聚合 */
    oal_uint8 auc_resv[2];
} hmac_btcoex_addba_req_stru;

typedef struct {
    oal_uint8 auc_user_mac_addr[WLAN_MAC_ADDR_LEN]; /* 黑名单MAC地址 */
    oal_uint8 uc_type;                              /* 写入黑名单的类型 */
    oal_uint8 uc_used;                              /* 是否已经写过黑名单MAC地址 */
} hmac_btcoex_delba_exception_stru;

typedef struct {
    frw_timeout_stru st_delba_opt_timer; /* 发送ARP REQ后启动定时器 */
    oal_atomic ul_rx_unicast_pkt_to_lan; /* 接收到的单播帧个数 */
} hmac_btcoex_arp_req_process_stru;

typedef struct {
    hmac_btcoex_delba_exception_stru ast_hmac_btcoex_delba_exception[MAX_BTCOEX_BSS_IN_BL];
    oal_uint8 uc_exception_bss_index; /* 黑名单MAC地址的数组下标 */
    oal_uint8 auc_resv[3];
    hal_btcoex_btble_status_stru st_btble_status;
} hmac_device_btcoex_stru;

typedef struct {
    hmac_btcoex_arp_req_process_stru st_hmac_btcoex_arp_req_process;
    hmac_btcoex_addba_req_stru st_hmac_btcoex_addba_req;
    oal_uint8 uc_ba_size;
    oal_uint8 uc_rx_no_pkt_count;                /* 超时时间内没有收到帧的次数 */
    oal_bool_enum_uint8 en_delba_btcoex_trigger; /* 是否btcoex触发删建BA */
    btcoex_ba_type_enum_uint8 en_ba_type;        /* 标记共存场景下是否特殊处理BA会话 */
} hmac_user_btcoex_stru;
extern oal_uint32 hmac_btcoex_check_exception_in_list(oal_void *p_arg, oal_uint8 *auc_addr);
extern oal_void hmac_btcoex_blacklist_handle_init(oal_void *p_arg);
extern oal_void hmac_btcoex_process_btble_status(mac_device_stru *pst_mac_device,
                                                 hal_btcoex_btble_status_stru *pst_btble_status_old,
                                                 hal_btcoex_btble_status_stru *pst_btble_status_new);

extern oal_void hmac_btcoex_ap_tpye_identify(mac_vap_stru *pst_mac_vap,
                                             oal_uint8 *puc_mac_addr,
                                             mac_bss_dscr_stru *pst_bss_dscr,
                                             mac_ap_type_enum_uint8 *pen_ap_type);

extern oal_void hmac_btcoex_process_exception_ap(mac_vap_stru *pst_mac_vap,
                                                 mac_user_stru *pst_mac_user,
                                                 mac_ap_type_enum_uint8 en_ap_type);

#endif /* #ifdef _PRE_WLAN_FEATURE_COEXIST_BT */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of __HMAC_BTCOEX_H__ */

