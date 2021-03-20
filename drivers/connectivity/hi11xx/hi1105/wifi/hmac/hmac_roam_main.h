

#ifndef __HMAC_ROAM_MAIN_H__
#define __HMAC_ROAM_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* 1 其他头文件包含 */
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_MAIN_H
/* 2 宏定义 */
#define ROAM_SCAN_TIME_MAX    (3 * 1000)  /* 扫描超时时间 单位ms */
#define ROAM_CONNECT_TIME_MAX (10 * 1000) /* 关联超时时间 单位ms */
#define ROAM_INVALID_SCAN_MAX (5)         /* 连续无效扫描门限   */

/* 超时时间150ms-->100ms, 使NR Resp超时更容易复现 */
#define ROAM_NEIGHBOR_PROCESS_TIME_MAX  (150)      /* 扫描超时时间 单位ms*/

/* 漫游场景识别一，识别出密集AP场景和低密AP场景 */
#define WLAN_FULL_CHANNEL_NUM           (20)  /* 判断全信道扫描的数目门限 */
#define ROAM_ENV_CANDIDATE_GOOD_NUM     (5)   /* 统计漫游环境中强信号强度AP的个数 */
#define ROAM_ENV_CANDIDATE_WEAK_NUM     (10)  /* 统计漫游环境中弱信号强度AP的个数 */
#define ROAM_ENV_RSSI_NE60_DB           (-60) /* 漫游环境中强信号强度AP RSSI的门限 */
#define ROAM_ENV_DENSE_TO_SPARSE_PERIOD (5)   /* 从高密场景向低密场景切换的扫描周期 */

/* 漫游场景识别二，识别出不满足漫游门限、但是有更好信号强度AP的场景 */
#define ROAM_ENV_BETTER_RSSI_PERIOD      (4)  /* 不满足漫游门限，但是有更好信号强度AP的扫描周期 */
#define ROAM_ENV_BETTER_RSSI_NULL_PERIOD (1)  /* 不满足漫游门限，但是有更好信号强度AP的扫描周期扫不到合适AP的周期 */
#define ROAM_ENV_BETTER_RSSI_DISTANSE    (30) /* 同一个ESS中最强信号强度AP和当前关联AP的RSSI间隔 */

#define ROAM_FAIL_FIVE_TIMES (100)

/* 3 枚举定义 */
/* 漫游触发条件 */
typedef enum {
    ROAM_TRIGGER_DMAC = 0,
    ROAM_TRIGGER_APP = 1,
    ROAM_TRIGGER_COEX = 2,
    ROAM_TRIGGER_11V = 3,
    ROAM_TRIGGER_M2S = 4,
    ROAM_TRIGGER_BSSID = 5, /* 指定BSSID漫游 */
    ROAM_TRIGGER_HOME_NETWORK = 6,
    ROAM_TRIGGER_ARP_PROBE_FAIL_REASSOC = 7,
    ROAM_TRIGGER_D2H_REASSOC = 8,

    ROAM_TRIGGER_BUTT
} roam_trigger_condition_enum;
typedef uint8_t roam_trigger_enum_uint8;

/* DMAC触发的漫游场景类型 */
typedef enum {
    ROAM_ENV_SPARSE_AP = 0, /* default AP roaming environment, based on [-78, -74] RSSI threshold */
    ROAM_ENV_LINKLOSS = 1,
    ROAM_ENV_DENSE_AP = 2, /* dense AP roaming environment, based on [-72, -68] RSSI threshold */

    ROAM_ENV_BUTT
} roam_scenario_enum;
typedef uint8_t roam_scenario_enum_uint8;

/* 漫游主状态机状态 */
typedef enum {
    ROAM_MAIN_STATE_INIT = 0,
    ROAM_MAIN_STATE_FAIL = ROAM_MAIN_STATE_INIT,
    ROAM_MAIN_STATE_SCANING = 1,
    ROAM_MAIN_STATE_CONNECTING = 2,
    ROAM_MAIN_STATE_UP = 3,
    ROAM_MAIN_STATE_NEIGHBOR_PROCESS = 4,

    ROAM_MAIN_STATE_BUTT
} roam_main_state_enum;
typedef uint8_t roam_main_state_enum_uint8;

/* 漫游主状态机事件类型 */
typedef enum {
    ROAM_MAIN_FSM_EVENT_START = 0,
    ROAM_MAIN_FSM_EVENT_SCAN_RESULT = 1,
    ROAM_MAIN_FSM_EVENT_START_CONNECT = 2,
    ROAM_MAIN_FSM_EVENT_CONNECT_FAIL = 3,
    ROAM_MAIN_FSM_EVENT_HANDSHAKE_FAIL = 4,
    ROAM_MAIN_FSM_EVENT_CONNECT_SUCC = 5,
    ROAM_MAIN_FSM_EVENT_TIMEOUT = 6,
    ROAM_MAIN_FSM_EVENT_TYPE_BUTT
} roam_main_fsm_event_type_enum;

#define ROAM_BAND_2G_BIT BIT0
#define ROAM_BAND_5G_BIT BIT1

/* 漫游扫描信道正交属性参数,命令行传入 */
typedef enum {
    ROAM_SCAN_CHANNEL_ORG_0 = 0, /* no scan */
    ROAM_SCAN_CHANNEL_ORG_1 = 1, /* scan only one channel */
    ROAM_SCAN_CHANNEL_ORG_3 = 2, /* 2.4G channel 1\6\11 */
    ROAM_SCAN_CHANNEL_ORG_4 = 3, /* 2.4G channel 1\5\9\13 */
    ROAM_SCAN_CHANNEL_ORG_DBDC = 4, /* scan p2p channel and the other band channels */
    ROAM_SCAN_CHANNEL_ORG_5 = 5, /* scan specific channel */
    ROAM_SCAN_CHANNEL_ORG_2G = 6, /* scan 2.4G channels only */
    ROAM_SCAN_CHANNEL_ORG_5G = 7, /* scan 5G channels only */
    ROAM_SCAN_CHANNEL_ORG_FULL = 8,  /* scan full channel for CI */
    ROAM_SCAN_CHANNEL_ORG_BUTT,  /* scan full channel */
} roam_channel_org_enum;
typedef uint8_t roam_channel_org_enum_uint8;
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 漫游配置结构体 */
typedef struct {
    uint8_t uc_scan_band;                         /* 扫描频段 */
    roam_channel_org_enum_uint8 uc_scan_orthogonal; /* 扫描信道正交属性 */
    int8_t c_trigger_rssi_2G;                     /* 2G时的触发门限 */
    int8_t c_trigger_rssi_5G;                     /* 5G时的触发门限 */
    uint8_t uc_delta_rssi_2G;                     /* 2G时的增益门限 */
    uint8_t uc_delta_rssi_5G;                     /* 5G时的增益门限 */
    int8_t c_dense_env_trigger_rssi_2G;           /* 密集AP环境中2G时的触发门限 */
    int8_t c_dense_env_trigger_rssi_5G;           /* 密集AP环境中5G时的触发门限 */
    oal_bool_enum_uint8 uc_scenario_enable;         /* 是否使能场景识别 */
    int8_t c_candidate_good_rssi;                 /* 密集AP场景识别，强信号强度AP的门限 */
    uint8_t uc_candidate_good_num;                /* 密集AP场景识别，强信号强度AP的数目 */
    uint8_t uc_candidate_weak_num;                /* 密集AP场景识别，弱信号强度AP的数目 */
    uint16_t us_roam_interval;                    /* 动态调整漫游触发间隔 */
    uint8_t uc_resv[2];
    uint32_t ul_blacklist_expire_sec; /* not used for now */
    uint32_t ul_buffer_max;           /* not used for now */
} hmac_roam_config_stru;

/* 漫游统计结构体 */
typedef struct {
    uint32_t ul_trigger_rssi_cnt;     /* rssi触发漫游扫描计数 */
    uint32_t ul_trigger_linkloss_cnt; /* linkloss触发漫游扫描计数 */
    uint32_t ul_scan_cnt;             /* 漫游扫描次数 */
    uint32_t ul_scan_result_cnt;      /* 漫游扫描返回次数 */
    uint32_t ul_connect_cnt;          /* 漫游连接计数 */
    uint32_t ul_roam_old_cnt;         /* 漫游失败计数 */
    uint32_t ul_roam_new_cnt;         /* 漫游成功计数 */
    uint32_t ul_roam_scan_fail;
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    uint32_t ul_roam_11v_scan_fail;
#endif
    uint32_t ul_roam_eap_fail;
    uint32_t ul_scan_start_timetamp;    /* 漫游扫描开始时间点 */
    uint32_t ul_scan_end_timetamp;      /* 漫游扫描开始时间点 */
    uint32_t ul_connect_start_timetamp; /* 漫游关联开始时间点 */
    uint32_t ul_connect_end_timetamp;   /* 漫游关联完成时间点 */
    uint8_t uc_roam_mode;               /* 漫游模式 */
    uint8_t uc_scan_mode;               /* 扫描模式 */
} hmac_roam_static_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_roam_enable(hmac_vap_stru *pst_hmac_vap, uint8_t uc_enable);
uint32_t hmac_roam_check_signal_bridge(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_start(hmac_vap_stru *pst_hmac_vap, roam_channel_org_enum uc_scan_type,
                               oal_bool_enum_uint8 en_current_bss_ignore, uint8_t *pauc_target_bssid,
                               roam_trigger_enum_uint8 en_roam_trigger);
uint32_t hmac_roam_handle_home_network(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_show(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_init(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_info_init(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_exit(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_test(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_resume_user(hmac_vap_stru *pst_hmac_vap, void *p_param);
uint32_t hmac_roam_pause_user(hmac_vap_stru *pst_hmac_vap, void *p_param);
uint32_t hmac_sta_roam_rx_mgmt(hmac_vap_stru *pst_hmac_vap, void *p_param);
uint32_t hmac_roam_trigger_handle(hmac_vap_stru *pst_hmac_vap, int8_t c_rssi,
                                        oal_bool_enum_uint8 en_current_bss_ignore);
void hmac_roam_tbtt_handle(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_scan_complete(hmac_vap_stru *pst_hmac_vap);
void hmac_roam_connect_complete(hmac_vap_stru *pst_hmac_vap, uint32_t ul_result);
uint32_t hmac_roam_connect_ft_ds_change_to_air(hmac_vap_stru *pst_hmac_vap,
                                                     void *p_param);
void hmac_roam_add_key_done(hmac_vap_stru *pst_hmac_vap);
void hmac_roam_wpas_connect_state_notify(hmac_vap_stru *pst_hmac_vap,
                                                 wpas_connect_state_enum_uint32 conn_state);
uint32_t hmac_roam_rssi_trigger_type(hmac_vap_stru *pst_hmac_vap, roam_scenario_enum_uint8 en_val);
uint32_t hmac_roam_check_bkscan_result(hmac_vap_stru *pst_hmac_vap, void *p_scan_record);
int8_t hmac_get_rssi_from_scan_result(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_bssid);
void hmac_roam_check_psk(hmac_vap_stru *pst_hmac_vap, mac_conn_security_stru *pst_conn_sec);
oal_bool_enum_uint8 hmac_roam_check_dbac_channel(mac_vap_stru *p2p_vap, uint8_t chanNum);
#ifdef _PRE_WLAN_FEATURE_11R
uint32_t hmac_roam_reassoc(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_rx_ft_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
#endif  // _PRE_WLAN_FEATURE_11R
#ifdef _PRE_WLAN_FEATURE_11K
uint32_t hmac_roam_rx_neighbor_response_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
oal_bool_enum_uint8 hmac_roam_is_neighbor_report_allowed(hmac_vap_stru *pst_hmac_vap);
#endif
void hmac_roam_timeout_test(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_roam_pri_sta_join_check(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *new_bss, mac_vap_stru *other_vap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_roam_main.h */
