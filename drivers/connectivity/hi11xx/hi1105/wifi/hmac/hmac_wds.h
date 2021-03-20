

#ifndef __HMAC_WDS_H__
#define __HMAC_WDS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "oal_types.h"
#include "mac_vap.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WDS_H
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
/* 2 宏定义 */
#define WDS_CALC_MAC_HASH_VAL(mac_addr) (MAC_CALCULATE_HASH_VALUE(mac_addr) & (WDS_HASH_NUM - 1))
#define WDS_TABLE_ADD_ENTRY             0
#define WDS_TABLE_DEL_ENTRY             1
#define WDS_MAX_STAS_NUM                256
#define WDS_MAX_NODE_NUM                4
#define WDS_MAX_NEIGH_NUM               512

#define WDS_MIN_AGE_NUM 5
#define WDS_MAX_AGE_NUM 10000

#define WDS_TABLE_DEF_TIMER 15000 /* timer interval as 15 secs */

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
#define VMSTA_4ADDR_SUPPORT   0
#define VMSTA_4ADDR_UNSUPPORT -1
#endif
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
typedef struct {
    oal_dlist_head_stru st_entry;
    uint8_t auc_mac[WLAN_MAC_ADDR_LEN];
    uint8_t uc_stas_num;
    uint8_t auc_resv[1];
} hmac_wds_node_stru;

typedef struct {
    oal_dlist_head_stru st_entry;
    uint32_t ul_last_pkt_age;
    hmac_wds_node_stru *pst_related_node;
    uint8_t auc_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv[2];
} hmac_wds_stas_stru;

typedef struct {
    oal_dlist_head_stru st_entry;
    uint32_t ul_last_pkt_age;
    uint8_t auc_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv[2];
} hmac_wds_neigh_stru;

typedef struct {
    uint8_t auc_sta_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_node_mac[WLAN_MAC_ADDR_LEN];
} mac_cfg_wds_sta_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
typedef uint32_t (*p_hmac_wds_node_func)(hmac_vap_stru *pst_vap, uint8_t *puc_addr, void *pst_arg);

/* 10 函数声明 */
uint32_t hmac_wds_update_table(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_node_mac,
                                        uint8_t *puc_sta_mac, uint8_t uc_update_mode);
uint32_t hmac_wds_add_node(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_node_mac);
uint32_t hmac_wds_del_node(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_node_mac);
uint32_t hmac_wds_reset_sta_mapping_table(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_wds_add_sta(hmac_vap_stru *pst_hmac_vap,
                                   uint8_t *puc_node_mac, uint8_t *puc_sta_mac);
uint32_t hmac_wds_del_sta(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_addr);
uint32_t hmac_find_valid_user_by_wds_sta(hmac_vap_stru *pst_hmac_vap,
                                                  uint8_t *puc_sta_mac_addr, uint16_t *pus_user_idx);
uint32_t hmac_wds_node_ergodic(hmac_vap_stru *pst_hmac_vap, uint8_t *src_addr,
                                        p_hmac_wds_node_func pst_hmac_wds_node, void *pst_arg);

uint32_t hmac_wds_update_neigh(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_addr);
uint32_t hmac_wds_neigh_not_expired(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_addr);
uint32_t hmac_wds_reset_neigh_table(hmac_vap_stru *pst_hmac_vap);

uint32_t hmac_wds_table_create_timer(hmac_vap_stru *pst_hmac_vap);

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
oal_bool_enum_uint8 hmac_vmsta_get_user_a4_support(hmac_vap_stru *pst_hmac_vap,
                                                          uint8_t *pst_addr);
oal_bool_enum_uint8 hmac_vmsta_check_vap_a4_support(uint8_t *puc_ie, uint32_t ul_ie_len);
oal_bool_enum_uint8 hmac_vmsta_check_user_a4_support(uint8_t *puc_frame,
                                                            uint32_t ul_frame_len);
uint32_t hmac_vmsta_set_vap_a4_enable(mac_vap_stru *pst_mac_vap);
#endif

void hmac_wds_init_table(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_wds_find_sta(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_addr,
                                    hmac_wds_stas_stru **ppst_wds_node);
uint32_t hmac_wds_tx_broadcast_pkt(hmac_vap_stru *pst_vap,
                                            uint8_t *puc_addr, void *pst_arg);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wds.h */


