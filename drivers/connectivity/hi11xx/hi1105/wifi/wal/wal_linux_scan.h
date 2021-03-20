

#ifndef __WAL_LINUX_SCAN_H__
#define __WAL_LINUX_SCAN_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hmac_device.h"
#include "wal_linux_rx_rsp.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_SCAN_H
/* 2 宏定义 */
/* 扫描上报"最近时间" 范围内的扫描结果 */
#define WAL_SCAN_REPORT_LIMIT 5000 /* 5000 milliseconds */
#define WLAN_BOOTTIME_REFLUSH 500  /* 500 us */
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
/* 国家码统计 */
#define WAL_MAX_COUNTRY_CODE_NUM       10
#define WAL_COUNTRY_CODE_UPDATE_FRIST  1
#define WAL_COUNTRY_CODE_UPDATE_SECOND 2
#define WAL_COUNTRY_CODE_UPDATE_NOMAL  3
#define WAL_COUNTRY_CODE_UPDATE_COUNT  5
#define WAL_BSSID_MAX_NUM              100
#endif
/* 3 枚举定义 */
/* 4 全局变量声明 */
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
extern uint8_t g_country_code_result[COUNTRY_CODE_LEN];
extern oal_bool_enum_uint8 g_country_code_result_update_flag;
extern oal_bool_enum_uint8 g_country_code_self_study_flag;
#endif
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
typedef struct {
    int8_t   ac_ssid[WLAN_SSID_MAX_LEN];
    uint8_t  pc_country_code[COUNTRY_CODE_LEN];
    uint32_t count_num;
}wal_country_code_count_stru;

typedef struct {
    wal_country_code_count_stru wal_country_code_count[WAL_MAX_COUNTRY_CODE_NUM];
    uint32_t country_type_num;
    uint8_t country_code_result[COUNTRY_CODE_LEN];
}wal_country_code_stru;
#endif
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
void wal_inform_all_bss(oal_wiphy_stru *pst_wiphy,
                                       hmac_bss_mgmt_stru *pst_bss_mgmt,
                                       uint8_t uc_vap_id);
uint32_t wal_scan_work_func(hmac_scan_stru *pst_scan_mgmt,
                                         oal_net_device_stru *pst_netdev,
                                         oal_cfg80211_scan_request_stru *pst_request);
int32_t wal_force_scan_complete(oal_net_device_stru *pst_net_dev, oal_bool_enum en_is_aborted);
int32_t wal_force_scan_abort_then_scan_comp(oal_net_device_stru *pst_net_dev);
int32_t wal_stop_sched_scan(oal_net_device_stru *pst_netdev);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
void wal_counrtycode_selfstudy_scan_comp(hmac_vap_stru *pst_hmac_vap);
#endif
#define IS_P2P_SCAN_REQ(pst_request) (((pst_request)->n_ssids > 0) && ((pst_request)->ssids != NULL) && \
    ((pst_request)->ssids[0].ssid_len == OAL_STRLEN("DIRECT-"))  && \
    (0 == oal_memcmp((pst_request)->ssids[0].ssid, "DIRECT-", OAL_STRLEN("DIRECT-"))))

void wal_update_bss(oal_wiphy_stru *pst_wiphy, hmac_bss_mgmt_stru *pst_bss_mgmt, uint8_t *puc_bssid);
void wal_update_bss_for_csa(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_net_device);

#endif /* end of wal_linux_scan.h */
