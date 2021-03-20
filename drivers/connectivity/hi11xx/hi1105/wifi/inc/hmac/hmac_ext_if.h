

#ifndef __HMAC_EXT_IF_H__
#define __HMAC_EXT_IF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_vap.h"
#include "mac_user.h"
#include "mac_frame.h"
#include "oal_hcc_host_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_EXT_IF_H

/* 2 宏定义 */
/* 3 枚举定义 */
/* 返回值类型定义 */
typedef enum {
    HMAC_TX_PASS = 0, /* 继续往下 */
    HMAC_TX_BUFF = 1, /* 已被缓存 */
    HMAC_TX_DONE = 2, /* 组播转成单播已发送 */

    HMAC_TX_DROP_PROXY_ARP = 3,    /* PROXY ARP检查后丢弃 */
    HMAC_TX_DROP_USER_UNKNOWN,     /* 未知user */
    HMAC_TX_DROP_USER_NULL,        /* user结构体为NULL */
    HMAC_TX_DROP_USER_INACTIVE,    /* 目的user未关联 */
    HMAC_TX_DROP_SECURITY_FILTER,  /* 安全检查过滤掉 */
    HMAC_TX_DROP_BA_SETUP_FAIL,    /* BA会话创建失败 */
    HMAC_TX_DROP_AMSDU_ENCAP_FAIL, /* amsdu封装失败 */
    HMAC_TX_DROP_AMSDU_BUILD_FAIL, /* amsdu组帧失败 */
    HMAC_TX_DROP_MUSER_NULL,       /* 组播user为NULL */
    HMAC_TX_DROP_MTOU_FAIL,        /* 组播转单播失败 */
    HMAC_TX_DROP_80211_ENCAP_FAIL, /* 802.11 head封装失败 */
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    HMAC_TX_HID2D_DROP,  /* 14 hid2d场景超时丢包 */
#endif
    HMAC_TX_BUTT
} hmac_tx_return_type_enum;
typedef uint8_t hmac_tx_return_type_enum_uint8;

/*
 * 枚举名  : hmac_host_ctx_event_sub_type_enum_uint8
 * 协议表格:
 * 枚举说明: HOST CTX事件子类型定义
 */
typedef enum {
    HMAC_HOST_CTX_EVENT_SUB_TYPE_SCAN_COMP_STA = 0, /* STA　扫描完成子类型 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_ASOC_COMP_STA,     /* STA 关联完成子类型 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_DISASOC_COMP_STA,  /* STA 上报去关联完成 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_CONNECT_AP,    /* AP 上报新加入BSS的STA情况 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_DISCONNECT_AP, /* AP 上报离开BSS的STA情况 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_MIC_FAILURE,       /* 上报MIC攻击 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_ACS_RESPONSE,      /* 上报ACS命令执行结果 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_RX_MGMT,           /* 上报接收到的管理帧 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_LISTEN_EXPIRED,    /* 上报监听超时 */
    HMAC_HOST_CTX_EVENT_SUB_TYPE_INIT,
    HMAC_HOST_CTX_EVENT_SUB_TYPE_MGMT_TX_STATUS,
    HMAC_HOST_CTX_EVENT_SUB_TYPE_ROAM_COMP_STA, /* STA 漫游完成子类型 */
#ifdef _PRE_WLAN_FEATURE_11R
    HMAC_HOST_CTX_EVENT_SUB_TYPE_FT_EVENT_STA, /* STA 漫游完成子类型 */
#endif                                         // _PRE_WLAN_FEATURE_11R

#ifdef _PRE_WLAN_FEATURE_DFR
    HMAC_HOST_CTX_EVENT_SUB_TYPE_DEV_ERROR, /* device异常处理流程 */
#endif                                      // _PRE_WLAN_FEATURE_DFR
    HMAC_HOST_CTX_EVENT_SUB_TYPE_VOWIFI_REPORT, /* 上报vowifi质量评估结果的切换请求 */

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    HMAC_HOST_CTX_EVENT_SUB_TYPE_SAMPLE_REPORT,
#endif

#ifdef _PRE_WLAN_ONLINE_DPD
    HMAC_HOST_CTX_EVENT_SUB_TYPE_DPD,
#endif

#ifdef _PRE_WLAN_FEATURE_DFS
    HMAC_HOST_CTX_EVENT_SUB_TYPE_CAC_REPORT, /* 上报CAC事件 */
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
    HMAC_HOST_CTX_EVENT_SUB_TYPE_M2S_STATUS, /* 上报m2s事件 */
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    HMAC_HOST_CTX_EVENT_SUB_TYPE_TAS_NOTIFY_RSSI, /* 上报TAS天线测量事件 */
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    HMAC_HOST_CTX_EVENT_SUB_TYPE_HID2D_SEQNUM, /* 上报HID2D丢帧序列号事件 */
#endif
    HMAC_HOST_CTX_EVENT_SUB_TYPE_EXT_AUTH_REQ, /* STA上报SAE认证事件 */

    HMAC_HOST_CTX_EVENT_SUB_TYPE_CH_SWITCH_NOTIFY,
#ifdef _PRE_WLAN_FEATURE_NAN
    HMAC_HOST_CTX_EVENT_SUB_TYPE_NAN_RSP,
#endif
    HMAC_HOST_CTX_EVENT_SUB_TYPE_BUTT
} hmac_host_ctx_event_sub_type_enum;
typedef uint8_t hmac_host_ctx_event_sub_type_enum_uint8;

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
#ifdef _PRE_WLAN_FEATURE_DFR
/* dfr相关功能信息 */
typedef struct {
    // p2p cl和dev共用一个业务vap,netdev的个数不会大于最大业务vap个数3
    oal_net_device_stru *past_netdev[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    uint32_t ul_netdev_num;
    uint32_t bit_hw_reset_enable : 1,    /* 硬件不去关联复位开关 */
               bit_device_reset_enable : 1,       /* device挂死异常恢复开关 */
               bit_soft_watchdog_enable : 1,      /* 软狗功能开关 */
               bit_device_reset_process_flag : 1, /* device挂死异常复位操作启动 */

               bit_ready_to_recovery_flag : 1,
               bit_user_disconnect_flag : 1, /* device挂死异常，需要在dfr恢复后告诉对端去关联的状态 */
               bit_resv : 26;
    uint32_t ul_excp_type; /* 异常类型 */
    uint32_t ul_dfr_num;   /* DFR statistics */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_stru wifi_excp_mutex;
#endif
    oal_completion st_plat_process_comp; /* 用来检测device异常恢复过程中平台工作是否完成的信号量 */

} hmac_dfr_info_stru;
#endif  // _PRE_WLAN_FEATURE_DFR

typedef struct {
    oal_netbuf_head_stru st_msdu_head; /* msdu链表头 */
    frw_timeout_stru st_amsdu_timer;
    oal_spin_lock_stru st_amsdu_lock; /* amsdu task lock */

    uint16_t us_amsdu_maxsize;
    uint16_t us_amsdu_size; /* Present size of the AMSDU */

    uint8_t uc_amsdu_maxnum;
    uint8_t uc_msdu_num;      /* Number of sub-MSDUs accumulated */
    uint8_t uc_last_pad_len;  // 51合入后可删除 /* 最后一个msdu的pad长度 */
    uint8_t auc_reserve[1];

    uint8_t auc_eth_da[WLAN_MAC_ADDR_LEN];  // 51合入后可删除
    uint8_t auc_eth_sa[WLAN_MAC_ADDR_LEN];  // 51合入后可删除
} hmac_amsdu_stru;

/* hmac配置私有结构 */
typedef struct {
    /* 用于wal_config层线程等待(wal_config-->hmac),给SDT下发读寄存器命令时用 */
    oal_wait_queue_head_stru st_wait_queue_for_sdt_reg;
    oal_bool_enum_uint8 en_wait_ack_for_sdt_reg;
    uint8_t auc_resv2[3];
    int8_t ac_rsp_msg[HMAC_RSP_MSG_MAX_LEN]; /* get wid返回消息内存空间 */
    uint32_t dog_tag;

} hmac_vap_cfg_priv_stru;

/* WAL抛事件给HMAC时的事件PAYLOAD结构体 */
typedef struct {
    oal_netbuf_stru *pst_netbuf; /* netbuf链表一个元素 */
    mac_vap_stru *pst_vap;
} hmac_tx_event_stru;

/* HMAC抛去关联完成事件结构体 */
typedef struct {
    uint8_t *puc_msg;
} hmac_disasoc_comp_event_stru;

/* 扫描结果 */
typedef struct {
    uint8_t uc_num_dscr;
    uint8_t uc_result_code;
    uint8_t auc_resv[2];
} hmac_scan_rsp_stru;

/* Status code for MLME operation confirm */
typedef enum {
    HMAC_MGMT_SUCCESS = 0,
    HMAC_MGMT_INVALID = 1,
    HMAC_MGMT_TIMEOUT = 2,
    HMAC_MGMT_REFUSED = 3,
    HMAC_MGMT_TOMANY_REQ = 4,
    HMAC_MGMT_ALREADY_BSS = 5
} hmac_mgmt_status_enum;
typedef uint8_t hmac_mgmt_status_enum_uint8;

/* 关联结果 */
typedef struct {
    hmac_mgmt_status_enum_uint8 en_result_code; /* 关联成功,超时等 */
    uint8_t auc_resv1[1];
    mac_status_code_enum_uint16 en_status_code; /* ieee协议规定的16位状态码  */

    uint8_t auc_addr_ap[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv2[2];

    uint32_t ul_asoc_req_ie_len;
    uint32_t ul_asoc_rsp_ie_len;

    uint8_t *puc_asoc_req_ie_buff;
    uint8_t *puc_asoc_rsp_ie_buff;
} hmac_asoc_rsp_stru;
/* 漫游结果 */
typedef struct {
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv1[2];
    mac_channel_stru st_channel;
    uint32_t ul_asoc_req_ie_len;
    uint32_t ul_asoc_rsp_ie_len;
    uint8_t *puc_asoc_req_ie_buff;
    uint8_t *puc_asoc_rsp_ie_buff;
} hmac_roam_rsp_stru;

#ifdef _PRE_WLAN_FEATURE_11R
typedef struct {
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
    uint16_t us_ft_ie_len;
    uint8_t *puc_ft_ie_buff;
} hmac_roam_ft_stru;

#endif  // _PRE_WLAN_FEATURE_11R

/* mic攻击 */
typedef struct {
    uint8_t auc_user_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_reserve[2];
    oal_nl80211_key_type en_key_type;
    int32_t l_key_id;
} hmac_mic_event_stru;

typedef enum {
    HMAC_RX_MGMT_EVENT_TYPE_NORMAL = 0,
    HMAC_RX_MGMT_EVENT_TYPE_NAN
} hmac_rx_mgmt_event_type_enum;
typedef uint8_t hmac_rx_mgmt_event_type_enum_uint8;

/* 上报接收到管理帧事件的数据结构 */
typedef struct {
    uint8_t *puc_buf;
    uint16_t us_len;
    uint8_t uc_rssi; /* 已经在驱动加上HMAC_FBT_RSSI_ADJUST_VALUE将负值转成正值 */
    hmac_rx_mgmt_event_type_enum_uint8 event_type;
    int32_t l_freq;
    int8_t ac_name[OAL_IF_NAME_SIZE];
} hmac_rx_mgmt_event_stru;

/* 上报监听超时数据结构 */
typedef struct {
    oal_ieee80211_channel_stru st_listen_channel;
    uint64_t ull_cookie;
    oal_wireless_dev_stru *pst_wdev;

} hmac_p2p_listen_expired_stru;

/* 上报接收到管理帧事件的数据结构 */
typedef struct {
    uint8_t uc_dev_mode;
    uint8_t uc_vap_mode;
    uint8_t uc_vap_status;
    uint8_t uc_write_read;
    uint32_t ul_val;
} hmac_cfg_rx_filter_stru;

/* 上报SAE 认证请求事件的数据结构 */
typedef struct {
    oal_nl80211_external_auth_action en_action;
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
    oal_ssids_stru st_ssid;
    uint32_t ul_key_mgmt_suite;
    uint16_t us_status;

} hmac_external_auth_req_stru;

typedef struct {
    uint16_t center_freq;
    uint8_t  uc_resv[2];
    oal_nl80211_chan_width width;
    uint32_t center_freq1;
    uint32_t center_freq2;
} hmac_channel_switch_stru;

typedef struct {
    oal_wait_queue_head_stru st_wait_queue;
    oal_bool_enum_uint8 mgmt_tx_complete;
    uint32_t mgmt_tx_status;
    uint8_t mgmt_frame_id;
} oal_mgmt_tx_stru;

typedef enum {
    HMAC_MIRACAST_SINK_SWITCH = 0,
    HMAC_MIRACAST_REDUCE_LOG_SWITCH = 1,
    HMAC_CORE_BIND_SWITCH = 2,
    HMAC_FEATURE_SWITCH_BUTT
} hmac_feature_switch_type_enum;
typedef uint8_t hmac_feature_switch_type_enum_uint8;

void hmac_parse_packet(oal_netbuf_stru *pst_netbuf_eth);

#define WIFI_WAKESRC_TAG "plat:wifi_wake_src,"
#define IPADDR(addr)     \
    ((uint8_t *)&addr)[0], \
        ((uint8_t *)&addr)[3]

#define IPADDR6(addr)           \
    ntohs((addr).s6_addr16[0]), \
        ntohs((addr).s6_addr16[7])

#define IPV6_ADDRESS_SIZEINBYTES 0x10

struct ieee8021x_hdr {
    uint8_t version;
    uint8_t type;
    uint16_t length;
};

#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG
#define WIFI_SEPCIAL_IPV4_PKT_TAG        "wifi:special_ipv4_pkt,"
#define GET_PKT_DIRECTION_STR(direction) ((direction == HMAC_PKT_DIRECTION_TX) ? "tx" : "rx")
#define HWMACSTR                         "%02x:%02x:%02x:**:**:%02x"
#define HWMAC2STR(a)                     (a)[0], (a)[1], (a)[2], (a)[5]

typedef enum {
    HMAC_PKT_DIRECTION_TX = 0,
    HMAC_PKT_DIRECTION_RX = 1,
} hmac_pkt_direction_enum;
void hmac_parse_special_ipv4_packet(uint8_t *puc_pktdata,
    uint32_t ul_datalen, hmac_pkt_direction_enum en_pkt_direction);
#endif

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
void hmac_board_get_instance(mac_board_stru **ppst_hmac_board);
int32_t hmac_main_init(void);
void hmac_main_exit(void);
uint32_t hmac_tx_wlan_to_wlan_ap(frw_event_mem_stru *pst_event_mem);

extern oal_bool_enum_uint8 g_feature_switch[HMAC_FEATURE_SWITCH_BUTT];
oal_bool_enum_uint8 hmac_get_feature_switch(hmac_feature_switch_type_enum_uint8 feature_id);
#ifdef _PRE_WLAN_TCP_OPT
uint32_t hmac_tx_lan_to_wlan_no_tcp_opt(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf);
#endif
uint32_t hmac_tx_lan_to_wlan(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf);
oal_net_dev_tx_enum hmac_bridge_vap_xmit(oal_netbuf_stru *pst_buf,
                                                    oal_net_device_stru *pst_dev);

uint16_t hmac_free_netbuf_list(oal_netbuf_stru *pst_buf);
uint32_t hmac_vap_get_priv_cfg(mac_vap_stru *pst_mac_vap, hmac_vap_cfg_priv_stru **ppst_cfg_priv);
oal_net_device_stru *hmac_vap_get_net_device(uint8_t uc_vap_id);
int8_t *hmac_vap_get_desired_country(uint8_t uc_vap_id);
#ifdef _PRE_WLAN_FEATURE_11D
uint32_t hmac_vap_get_updata_rd_by_ie_switch(uint8_t uc_vap_id,
                                                          oal_bool_enum_uint8 *us_updata_rd_by_ie_switch);
#endif
uint32_t hmac_config_add_vap(mac_vap_stru *pst_mac_vap,
                                          uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_del_vap(mac_vap_stru *pst_mac_vap,
                                          uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_start_vap(mac_vap_stru *pst_mac_vap,
                                            uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_down_vap(mac_vap_stru *pst_mac_vap,
                                           uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_mac_addr(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_get_wmmswitch(mac_vap_stru *pst_mac_vap,
                                            uint16_t *pus_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_get_vap_wmm_switch(mac_vap_stru *pst_mac_vap,
                                                 uint16_t *pus_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_vap_wmm_switch(mac_vap_stru *pst_mac_vap,
                                                 uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_get_max_user(mac_vap_stru *pst_mac_vap,
                                           uint16_t *pus_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_bg_noise_info(mac_vap_stru *pst_mac_vap,
                                            uint16_t *pus_len,
                                            uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_M2S
#ifdef _PRE_WLAN_FEATURE_M2S_MSS
uint32_t hmac_config_set_m2s_switch_blacklist(mac_vap_stru *pst_mac_vap,
                                                       uint16_t us_len,
                                                       uint8_t *puc_param);
uint32_t hmac_config_set_m2s_switch_mss(mac_vap_stru *pst_mac_vap,
                                                 uint16_t us_len,
                                                 uint8_t *puc_param);
#endif
uint32_t hmac_config_mimo_compatibility(mac_vap_stru *pst_mac_vap,
                                                     uint8_t uc_len,
                                                     uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_M2S_MODEM
uint32_t hmac_config_set_m2s_switch_modem(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param);
#endif
#endif
uint32_t hmac_config_get_mode(mac_vap_stru *pst_mac_vap,
                                           uint16_t *pus_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_mode(mac_vap_stru *pst_mac_vap,
                                           uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_bss_type(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_get_bss_type(mac_vap_stru *pst_mac_vap,
                                               uint16_t *pus_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_set_ssid(mac_vap_stru *pst_mac_vap,
                                           uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_get_ssid(mac_vap_stru *pst_mac_vap,
                                           uint16_t *pus_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_get_ampdu(mac_vap_stru *pst_mac_vap,
                                        uint16_t *pus_len,
                                        uint8_t *puc_param);
uint32_t hmac_config_set_shpreamble(mac_vap_stru *pst_mac_vap,
                                                 uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_get_shpreamble(mac_vap_stru *pst_mac_vap,
                                                 uint16_t *pus_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_shortgi20(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_get_shortgi20(mac_vap_stru *pst_mac_vap,
                                                uint16_t *pus_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_set_shortgi40(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_get_shortgi40(mac_vap_stru *pst_mac_vap,
                                                uint16_t *pus_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_set_shortgi80(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_get_shortgi80(mac_vap_stru *pst_mac_vap,
                                                uint16_t *pus_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_MONITOR
uint32_t hmac_config_set_sniffer(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_monitor_mode(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_monitor(mac_vap_stru *pst_mac_vap,
                                              uint16_t *pus_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_set_prot_mode(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_prot_mode(mac_vap_stru *pst_mac_vap,
                                                uint16_t *pus_len, uint8_t *puc_param);
uint32_t hmac_config_set_auth_mode(mac_vap_stru *pst_mac_vap,
                                                uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_auth_mode(mac_vap_stru *pst_mac_vap,
                                                uint16_t *pus_len, uint8_t *puc_param);
uint32_t hmac_config_set_max_user(mac_vap_stru *mac_vap, uint32_t max_user_num);
uint32_t hmac_config_set_bintval(mac_vap_stru *pst_mac_vap,
                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_bintval(mac_vap_stru *pst_mac_vap,
                                              uint16_t *pus_len, uint8_t *puc_param);
uint32_t hmac_config_set_nobeacon(mac_vap_stru *pst_mac_vap,
                                               uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_nobeacon(mac_vap_stru *pst_mac_vap,
                                               uint16_t *pus_len, uint8_t *puc_param);
uint32_t hmac_config_set_txpower(mac_vap_stru *pst_mac_vap,
                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_ota_beacon_switch(mac_vap_stru *pst_mac_vap,
                                                    uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_ota_rx_dscr_switch(mac_vap_stru *pst_mac_vap,
                                                     uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_all_ota(mac_vap_stru *pst_mac_vap,
                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_dhcp_arp_switch(mac_vap_stru *pst_mac_vap,
                                                      uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_random_mac_addr_scan(mac_vap_stru *pst_mac_vap,
                                                           uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_random_mac_oui(mac_vap_stru *pst_mac_vap,
                                                     uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_get_txpower(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_set_freq(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
uint32_t hmac_hid2d_drop_report(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_get_freq(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_wmm_params(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_get_wmm_params(mac_vap_stru *pst_mac_vap, uint8_t *puc_param);
uint32_t hmac_config_vap_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
uint32_t hmac_config_pk_mode_debug(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#endif
uint32_t hmac_config_eth_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_80211_ucast_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_set_mgmt_log(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user,
                                               oal_bool_enum_uint8 en_start);
uint32_t hmac_config_80211_mcast_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_probe_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_protocol_debug_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_config_phy_debug_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);

uint32_t hmac_config_report_vap_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_wfa_cfg_aifsn(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_wfa_cfg_cw(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_lte_gpio_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_set_vowifi_nat_keep_alive_params(mac_vap_stru *pst_mac_vap,
                                                               uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_log_level(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_set_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_get_dtimperiod(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_user_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_add_user(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_del_user(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#ifdef _PRE_WLAN_CHIP_FPGA_PCIE_TEST
uint32_t hmac_config_pcie_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_ampdu_end(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_config_twt_setup_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_twt_teardown_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);
#endif
uint32_t hmac_config_addba_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_delba_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_set_tx_pow_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_set_dscr_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_mcs(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_set_mcsac(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_config_set_mcsax(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
uint32_t hmac_config_set_mcsax_er(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#endif
#endif
uint32_t hmac_config_set_nss(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                      uint8_t *puc_param);
uint32_t hmac_config_set_rfch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_set_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
uint32_t hmac_config_always_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                        uint8_t *puc_param);
uint32_t hmac_config_always_tx_num(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_always_rx(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_always_tx_aggr_num(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_ru_index(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#ifdef PLATFORM_DEBUG_ENABLE
uint32_t hmac_config_reg_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#endif
uint32_t hmac_config_sdio_flowctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);

#ifdef _PRE_WLAN_DELAY_STATISTIC
uint32_t hmac_config_pkt_time_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
#endif
uint32_t hmac_config_list_sta(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_get_sta_list(mac_vap_stru *pst_mac_vap, uint16_t *us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_list_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_sta_initiate_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_cfg80211_start_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_cfg80211_stop_sched_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_cfg80211_start_scan_sta(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);
uint32_t hmac_sta_initiate_join(mac_vap_stru *pst_mac_vap, mac_bss_dscr_stru *pst_bss_dscr);
void hmac_mgmt_send_deauth_frame(mac_vap_stru *pst_mac_vap,
                                                const unsigned char *puc_da,
                                                uint16_t us_err_code,
                                                oal_bool_enum_uint8 en_is_protected);
uint32_t hmac_config_send_deauth(mac_vap_stru *pst_mac_vap, uint8_t *puc_da);
void hmac_mgmt_send_disassoc_frame(mac_vap_stru *pst_mac_vap, uint8_t *puc_da,
                                                  uint16_t us_err_code, oal_bool_enum_uint8 en_is_protected);

uint32_t hmac_config_11i_add_key(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_11i_get_key(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_11i_remove_key(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_11i_set_default_key(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                      uint8_t *puc_param);
uint32_t hmac_config_kick_user(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_vowifi_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_update_ip_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);

uint32_t hmac_config_set_probe_resp_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_ONLINE_DPD
uint32_t hmac_config_dpd_cfg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_send_bar(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#ifdef PLATFORM_DEBUG_ENABLE
uint32_t hmac_config_reg_write(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#endif
uint32_t hmac_check_capability_mac_phy_supplicant(mac_vap_stru *pst_mac_vap,
                                                               mac_bss_dscr_stru *pst_bss_dscr);
uint32_t hmac_config_amsdu_ampdu_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_11i_add_wep_entry(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_sdt_recv_reg_cmd(mac_vap_stru *pst_mac_vap, uint8_t *puc_buf,
                                            uint16_t us_len);
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
uint32_t hmac_sdt_recv_sample_cmd(mac_vap_stru *pst_mac_vap, uint8_t *puc_buf,
                                           uint16_t us_len);
#endif
uint32_t hmac_init_event_process(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_config_alg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
uint32_t hmac_config_tcp_ack_buf(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
#endif

#ifdef _PRE_WLAN_FEATURE_TXOPPS
uint32_t hmac_config_set_txop_ps_machw(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
#endif
uint32_t hmac_config_btcoex_preempt_type(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_btcoex_set_perf_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_btcoex_rx_delba_trigger(mac_vap_stru *pst_mac_vap, uint8_t uc_len,
                                                   uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_NRCOEX
uint32_t hmac_config_nrcoex_priority_set(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_nrcoex_test(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_query_nrcoex_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
#endif

uint32_t hmac_config_set_uapsden(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_get_uapsden(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_set_reset_state(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_reset_hw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_DFT_STAT
uint32_t hmac_config_usr_queue_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_report_vap_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_report_all_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
#endif

uint32_t hmac_config_get_hipkt_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_set_flowctl_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_get_flowctl_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);

uint32_t hmac_config_send_frame(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
uint32_t hmac_config_dscp_map_to_tid(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_clean_dscp_tid_map(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_amsdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_set_ampdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_get_amsdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_get_ampdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_set_country_for_dfs(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                      uint8_t *puc_param);
uint32_t hmac_config_set_country(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
uint32_t hmac_config_reduce_sar(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
uint32_t hmac_config_tas_pwr_ctrl(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
uint32_t hmac_config_tas_rssi_access(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
#endif
uint32_t hmac_config_get_country(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_connect(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_get_tid(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_reset_hw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_reset_operate(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);

uint32_t hmac_config_set_mib_by_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_set_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_set_beacon(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_set_wps_p2p_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_wps_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_set_app_ie_to_vap(mac_vap_stru *pst_mac_vap,
                                                    oal_app_ie_stru *pst_wps_ie,
                                                    en_app_ie_type_uint8 en_type);

uint32_t hmac_config_alg_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_cali_debug(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
uint32_t hmac_11v_cfg_bsst_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_11v_sta_tx_query(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

uint32_t hmac_config_set_acs_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_beacon_chain_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_set_2040_coext_support(mac_vap_stru *pst_mac_vap,
                                                         uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_rx_fcs_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_sync_cmd_common(mac_vap_stru *pst_mac_vap,
                                                  wlan_cfgid_enum_uint16 en_cfg_id,
                                                  uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_open_wmm(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                       uint8_t *puc_param);

uint32_t hmac_config_set_pm_by_module(mac_vap_stru *pst_mac_vap,
                                                   mac_pm_ctrl_type_enum pm_ctrl_type, mac_pm_switch_enum pm_enable);
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
uint32_t hmac_enable_pmf(mac_vap_stru *pst_mac_vap, uint8_t *puc_param);
uint32_t hmac_config_vap_pmf_cap(mac_vap_stru *pst_mac_vap,
                                              wlan_pmf_cap_status_uint8 en_pmf_cap);
#endif
uint32_t hmac_config_set_coex(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_auto_protection(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                      uint8_t *puc_param);
uint32_t hmac_config_set_dfx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_send_2040_coext(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_get_version(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_get_ant(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);

uint32_t hmac_tx_report_eth_frame(mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_netbuf);

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
uint32_t hmac_config_get_user_rssbw(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
#endif
#ifdef _PRE_WLAN_FEATURE_M2S
uint32_t hmac_config_set_m2s_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
#endif
uint32_t hmac_config_ru_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_radar_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

uint32_t hmac_config_get_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t *pus_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_blacklist_mode(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_blacklist_add(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_blacklist_add_only(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_blacklist_del(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_show_blacklist(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_autoblacklist_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                       uint8_t *puc_param);
uint32_t hmac_config_set_autoblacklist_aging(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                          uint8_t *puc_param);
uint32_t hmac_config_set_autoblacklist_threshold(mac_vap_stru *pst_mac_vap,
                                                              uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_set_autoblacklist_reset_time(mac_vap_stru *pst_mac_vap,
                                                               uint16_t us_len, uint8_t *puc_param);

#ifdef _PRE_WLAN_FEATURE_HIEX
uint32_t hmac_config_set_user_hiex_enable(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_set_ampdu_aggr_num(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_set_amsdu_aggr_num(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
uint32_t hmac_config_set_psd_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_cfg_psd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#endif
#ifdef _PRE_WLAN_FEATURE_CSI
uint32_t hmac_config_set_csi(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif

uint32_t hmac_config_set_stbc_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_set_ldpc_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                               uint8_t *puc_param);
uint32_t hmac_config_set_txbf_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_vap_close_txbf_cap(mac_vap_stru *pst_mac_vap);
uint32_t hmac_config_set_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_del_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_flush_pmksa(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_scan_abort(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_remain_on_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_config_cancel_remain_on_channel(mac_vap_stru *pst_mac_vap,
                                                           uint16_t us_len, uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_SAE
uint32_t hmac_config_external_auth(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
#endif /* _PRE_WLAN_FEATURE_SAE */
#ifdef _PRE_WLAN_FEATURE_HS20
uint32_t hmac_config_set_qos_map(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
#endif
uint32_t hmac_config_set_dc_status(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);

uint32_t hmac_config_set_p2p_miracast_status(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                      uint8_t *puc_param);
uint32_t hmac_config_set_p2p_ps_ops(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_p2p_ps_noa(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_set_p2p_ps_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_wpas_mgmt_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                        uint8_t *puc_param);
uint32_t hmac_config_vap_classify_en(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_vap_classify_tid(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);
uint32_t hmac_config_query_station_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                     uint8_t *puc_param);
uint32_t hmac_config_query_rssi(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_config_query_psst(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
uint32_t hmac_config_query_psm_flt_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                              uint8_t *puc_param);
uint32_t hmac_config_query_rate(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);

#ifdef _PRE_WLAN_DFT_STAT
uint32_t hmac_config_query_ani(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#endif

uint32_t hmac_config_vap_state_syn(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_set_uapsd_para(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
uint32_t hmac_config_enable_rr_time_info(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
#endif

uint32_t hmac_config_enable_arp_offload(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                          uint8_t *puc_param);
uint32_t hmac_config_set_ip_addr(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
uint32_t hmac_config_show_arpoffload_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_user_num_spatial_stream_cap_syn(mac_vap_stru *pst_mac_vap,
                                                       mac_user_stru *pst_mac_user);

uint32_t hmac_config_cfg_vap_h2d(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                       uint8_t *puc_param);
uint32_t hmac_config_user_asoc_state_syn(mac_vap_stru *pst_mac_vap,
                                                      mac_user_stru *pst_mac_user);
uint32_t hmac_config_user_cap_syn(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user);
uint32_t hmac_config_user_rate_info_syn(mac_vap_stru *pst_mac_vap,
                                                     mac_user_stru *pst_mac_user);
uint32_t hmac_config_user_info_syn(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user);
#ifdef _PRE_WLAN_FEATURE_M2S
uint32_t hmac_config_vap_m2s_info_syn(mac_vap_stru *pst_mac_vap);
#endif
uint32_t hmac_config_sta_vap_info_syn(mac_vap_stru *pst_mac_vap);
uint32_t hmac_init_user_security_port(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user);
uint32_t hmac_user_set_asoc_state(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user,
                                               mac_user_asoc_state_enum_uint8 en_value);
uint32_t hmac_config_ch_status_sync(mac_device_stru *pst_mac_dev);
#ifdef _PRE_WLAN_TCP_OPT
uint32_t hmac_config_get_tcp_ack_stream_info(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);
uint32_t hmac_config_tx_tcp_ack_opt_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_rx_tcp_ack_opt_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                 uint8_t *puc_param);
uint32_t hmac_config_tx_tcp_ack_limit(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_rx_tcp_ack_limit(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
#endif
void hmac_config_del_p2p_ie(uint8_t *puc_ie, uint32_t *pul_ie_len);
uint32_t hmac_find_p2p_listen_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);

void hmac_set_device_freq_mode(uint8_t uc_device_freq_type);
uint32_t hmac_config_set_device_freq(uint8_t uc_device_freq_type);

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
uint32_t hmac_config_dyn_cali_param(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
#endif
uint32_t hmac_config_get_sta_11h_abillty(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                  uint8_t *puc_param);
uint32_t hmac_config_set_vendor_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                            uint8_t *puc_param);
uint32_t hmac_config_set_mlme(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
int32_t hmac_cfg80211_dump_survey(oal_wiphy_stru *pst_wiphy,
                                               oal_net_device_stru *pst_netdev,
                                               int32_t l_idx, oal_survey_info_stru *pst_info);
#endif
#ifdef _PRE_WLAN_FEATURE_11K
uint32_t hmac_config_send_neighbor_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                    uint8_t *puc_param);
uint32_t hmac_config_bcn_table_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                   uint8_t *puc_param);
#endif
uint32_t hmac_config_voe_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                             uint8_t *puc_param);
#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_config_11ax_debug(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
#endif
uint32_t hmac_config_vendor_cmd_get_channel_list(mac_vap_stru *pst_mac_vap,
                                                       uint16_t *pus_len, uint8_t *puc_param);

uint32_t hmac_config_set_bw_fixed(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                           uint8_t *puc_param);
uint32_t hmac_config_dbdc_debug_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
#ifndef CONFIG_HAS_EARLYSUSPEND
void hmac_do_suspend_action(mac_device_stru *pst_mac_device, uint8_t uc_in_suspend);
#endif
#ifdef _PRE_WLAN_FEATURE_APF
uint32_t hmac_config_set_apf_filter(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);
#endif
uint32_t hmac_config_cali_debug(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);

int32_t hmac_net_register_netdev(oal_net_device_stru *p_net_device);


void hmac_set_tx_switch(int32_t tx_switch_ini_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_ext_if.h */
