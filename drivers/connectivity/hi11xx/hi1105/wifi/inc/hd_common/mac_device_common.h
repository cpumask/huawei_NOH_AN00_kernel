
#ifndef __MAC_DEVICE_COMMON_H__
#define __MAC_DEVICE_COMMON_H__

#include "wlan_mib.h"

#define MAC_DATARATES_PHY_80211G_NUM 12

#define MAC_RX_BA_LUT_BMAP_LEN ((HAL_MAX_RX_BA_LUT_SIZE + 7) >> 3)
#define MAC_TX_BA_LUT_BMAP_LEN ((HAL_MAX_TX_BA_LUT_SIZE + 7) >> 3)

#define MAC_SCAN_CHANNEL_INTERVAL_DEFAULT         6   /* 间隔6个信道，切回工作信道工作一段时间 */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_DEFAULT     110 /* 背景扫描时，返回工作信道工作的时间 */
#define MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE     2   /* 间隔2个信道，切回工作信道工作一段时间 */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE 60  /* WLAN未关联，P2P关联，返回工作信道工作的时间 */
#define MAC_SCAN_CHANNEL_INTERVAL_HIDDEN_SSID     3   /* 携带隐藏SSID的背景扫描，间隔3个信道回工作信道工作一段时间 */
#define MAC_MAX_IP_FILTER_BTABLE_SIZE 512 /* rx ip数据包过滤功能的黑名单大小 */

/* 上报关键信息的flags标记信息，对应标记位为1，则对上报对应信息 */
typedef enum {
    MAC_REPORT_INFO_FLAGS_HARDWARE_INFO = BIT(0),
    MAC_REPORT_INFO_FLAGS_QUEUE_INFO = BIT(1),
    MAC_REPORT_INFO_FLAGS_MEMORY_INFO = BIT(2),
    MAC_REPORT_INFO_FLAGS_EVENT_INFO = BIT(3),
    MAC_REPORT_INFO_FLAGS_VAP_INFO = BIT(4),
    MAC_REPORT_INFO_FLAGS_USER_INFO = BIT(5),
    MAC_REPORT_INFO_FLAGS_TXRX_PACKET_STATISTICS = BIT(6),
    MAC_REPORT_INFO_FLAGS_BUTT = BIT(7),
} mac_report_info_flags;

/* DMAC SCANNER 扫描模式 */
#define MAC_SCAN_FUNC_MEAS       0x1
#define MAC_SCAN_FUNC_STATS      0x2
#define MAC_SCAN_FUNC_RADAR      0x4 // WTBD 未用
#define MAC_SCAN_FUNC_BSS        0x8
#define MAC_SCAN_FUNC_P2P_LISTEN 0x10
#define MAC_SCAN_FUNC_ALL        \
    (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS | MAC_SCAN_FUNC_RADAR | MAC_SCAN_FUNC_BSS)

/* 扫描完成事件返回状态码 */
typedef enum {
    MAC_SCAN_SUCCESS = 0,    /* 扫描成功 */
    MAC_SCAN_PNO = 1,        /* pno扫描结束 */
    MAC_SCAN_TIMEOUT = 2,    /* 扫描超时 */
    MAC_SCAN_REFUSED = 3,    /* 扫描被拒绝 */
    MAC_SCAN_ABORT = 4,      /* 终止扫描 */
    MAC_SCAN_ABORT_SYNC = 5, /* 扫描被终止同步状态，用于上层去关联命令时强制abort不往内核上报等dmac响应abort往上报 */
    MAC_SCAN_STATUS_BUTT,    /* 无效状态码，初始化时使用此状态码 */
} mac_scan_status_enum;
typedef uint8_t mac_scan_status_enum_uint8;

typedef enum {
    MAC_CH_TYPE_NONE = 0,
    MAC_CH_TYPE_PRIMARY = 1,
    MAC_CH_TYPE_SECONDARY = 2,
} mac_ch_type_enum;
typedef uint8_t mac_ch_type_enum_uint8;

/* SDT操作模式枚举 */
typedef enum {
    MAC_SDT_MODE_WRITE = 0,
    MAC_SDT_MODE_READ,
    MAC_SDT_MODE_WRITE16,
    MAC_SDT_MODE_READ16,

    MAC_SDT_MODE_BUTT
} mac_sdt_rw_mode_enum;
typedef uint8_t mac_sdt_rw_mode_enum_uint8;

typedef enum {
    MAC_SCAN_OP_INIT_SCAN,
    MAC_SCAN_OP_FG_SCAN_ONLY,
    MAC_SCAN_OP_BG_SCAN_ONLY,

    MAC_SCAN_OP_BUTT
} mac_scan_op_enum;
typedef uint8_t mac_scan_op_enum_uint8;

/* 扫描结果 */
typedef struct {
    mac_scan_status_enum_uint8 en_scan_rsp_status;
    uint8_t auc_resv[3];
    uint64_t ull_cookie;
} mac_scan_rsp_stru;

typedef enum {
    MAC_CHAN_NOT_SUPPORT = 0,      /* 管制域不支持该信道 */
    MAC_CHAN_AVAILABLE_ALWAYS,     /* 信道一直可以使用 */
    MAC_CHAN_AVAILABLE_TO_OPERATE, /* 经过检测(CAC, etc...)后，该信道可以使用 */
    MAC_CHAN_DFS_REQUIRED,         /* 该信道需要进行雷达检测 */
    MAC_CHAN_BLOCK_DUE_TO_RADAR,   /* 由于检测到雷达导致该信道变的不可用 */

    MAC_CHAN_STATUS_BUTT
} mac_chan_status_enum;
typedef uint8_t mac_chan_status_enum_uint8;

/* device reset同步子类型枚举 */
typedef enum {
    MAC_RESET_SWITCH_SET_TYPE,
    MAC_RESET_SWITCH_GET_TYPE,
    MAC_RESET_STATUS_GET_TYPE,
    MAC_RESET_STATUS_SET_TYPE,
    MAC_RESET_SWITCH_SYS_TYPE = MAC_RESET_SWITCH_SET_TYPE,
    MAC_RESET_STATUS_SYS_TYPE = MAC_RESET_STATUS_SET_TYPE,

    MAC_RESET_SYS_TYPE_BUTT
} mac_reset_sys_type_enum;
typedef uint8_t mac_reset_sys_type_enum_uint8;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
#pragma pack(push, 1)
/* 上报的扫描结果的扩展信息，放在上报host侧的管理帧netbuf的后面 */
typedef struct {
    int32_t l_rssi;                                /* 信号强度 */
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type; /* 扫描到的bss类型 */
    uint8_t auc_resv[3]; /* 预留字段 */

    int8_t c_ant0_rssi; /* 天线0的rssi */
    int8_t c_ant1_rssi; /* 天线1的rssi */
    int8_t   ant2_rssi;   /* 天线2的rssi */
    int8_t   ant3_rssi;   /* 天线3的rssi */

#ifdef _PRE_WLAN_FEATURE_M2S
    wlan_nss_enum_uint8 en_support_max_nss; /* 该AP支持的最大空间流数 */
    oal_bool_enum_uint8 en_support_opmode;  /* 该AP是否支持OPMODE */
    uint8_t uc_num_sounding_dim;          /* 该AP发送txbf的天线数 */
#endif
} mac_scanned_result_extend_info_stru;
#pragma pack(pop)

typedef struct {
    uint8_t auc_transmitted_bssid[WLAN_MAC_ADDR_LEN]; /* transmitted bssid */
    uint8_t bit_is_non_transimitted_bss : 1,
              bit_ext_cap_multi_bssid_activated : 1,
              bit_rsv : 6;
    uint8_t auc_rcv[1];
} mac_multi_bssid_info;

/* 带宽调试开关相关的结构体 */
typedef enum {
    MAC_CSA_FLAG_NORMAL = 0,
    MAC_CSA_FLAG_START_DEBUG, /* 固定csa ie 在beacon帧中 */
    MAC_CSA_FLAG_CANCLE_DEBUG,
    MAC_CSA_FLAG_GO_DEBUG,

    MAC_CSA_FLAG_BUTT
} mac_csa_flag_enum;
typedef uint8_t mac_csa_flag_enum_uint8;

typedef struct {
    wlan_csa_mode_tx_enum_uint8 en_mode;
    uint8_t uc_channel;
    uint8_t uc_cnt;
    wlan_channel_bandwidth_enum_uint8 en_bandwidth;
    mac_csa_flag_enum_uint8 en_debug_flag; /* 0:正常切信道; 1:仅beacon帧中含有csa,信道不切换;2:取消beacon帧中含有csa */
    uint8_t auc_reserv[3];
} mac_csa_debug_stru;

typedef struct {
    uint32_t ul_cmd_bit_map;
    oal_bool_enum_uint8 en_band_force_switch_bit0; /* 恢复40M带宽命令 */
    oal_bool_enum_uint8 en_2040_ch_swt_prohi_bit1; /* 不允许20/40带宽切换开关 */
    oal_bool_enum_uint8 en_40_intolerant_bit2;     /* 不容忍40M带宽开关 */
    uint8_t uc_resv;
    mac_csa_debug_stru st_csa_debug_bit3;
    oal_bool_enum_uint8 en_lsigtxop_bit5; /* lsigtxop使能 */
    uint8_t auc_resv0[3];
} mac_protocol_debug_switch_stru;

typedef enum {
    MAC_PM_DEBUG_SISO_RECV_BCN = 0,
    MAC_PM_DEBUG_DYN_TBTT_OFFSET = 1,
    MAC_PM_DEBUG_NO_PS_FRM_INT = 2,
    MAC_PM_DEBUG_APF = 3,
    MAC_PM_DEBUG_AO = 4,

    MAC_PM_DEBUG_CFG_BUTT
} mac_pm_debug_cfg_enum_uint8;

typedef struct {
    uint32_t ul_cmd_bit_map;
    uint8_t uc_srb_switch; /* siso收beacon开关 */
    uint8_t uc_dto_switch; /* 动态tbtt offset开关 */
    uint8_t uc_nfi_switch;
    uint8_t uc_apf_switch;
    uint8_t uc_ao_switch;
} mac_pm_debug_cfg_stru;

typedef enum {
    MAC_DBDC_CHANGE_HAL_DEV = 0, /* vap change hal device hal vap */
    MAC_DBDC_SWITCH = 1,         /* DBDC软件开关 */
    MAC_FAST_SCAN_SWITCH = 2,    /* 并发扫描开关 */
    MAC_DBDC_STATUS = 3,         /* DBDC状态查询 */

    MAC_DBDC_CMD_BUTT
} mac_dbdc_cmd_enum;
typedef uint8_t mac_dbdc_cmd_enum_uint8;

typedef struct {
    uint32_t ul_cmd_bit_map;
    uint8_t uc_dst_hal_dev_id; /* 需要迁移到的hal device id */
    uint8_t uc_dbdc_enable;
    oal_bool_enum_uint8 en_fast_scan_enable; /* 是否可以并发,XX原因即使硬件支持也不能快速扫描 */
    uint8_t uc_dbdc_status;
} mac_dbdc_debug_switch_stru;

/* ACS 命令及回复格式 */
typedef struct {
    uint8_t uc_cmd;
    uint8_t uc_chip_id;
    uint8_t uc_device_id;
    uint8_t uc_resv;

    uint32_t ul_len;     /* 总长度，包括上面前4个字节 */
    uint32_t ul_cmd_cnt; /* 命令的计数 */
} mac_acs_response_hdr_stru;

typedef struct {
    wlan_m2s_mgr_vap_stru ast_m2s_blacklist[WLAN_M2S_BLACKLIST_MAX_NUM];
    uint8_t uc_blacklist_cnt;
} mac_m2s_ie_stru;

typedef struct {
    uint8_t uc_cfg_btcoex_mode; /* 0:参数查询模式; 1:参数配置模式 */
    uint8_t uc_cfg_btcoex_type; /* 0:门限类型; 1:聚合大小类型 2.rssi detect门限参数配置模式 */
    union {
        struct {
            wlan_nss_enum_uint8 en_btcoex_nss; /* 0:siso 1:mimo */
            uint8_t uc_20m_low;              /* 2G 20M low */
            uint8_t uc_20m_high;             /* 2G 20M high */
            uint8_t uc_40m_low;              /* 2G 40M low */
            uint16_t us_40m_high;            /* 2G 40M high */
        } threhold;
        struct {
            wlan_nss_enum_uint8 en_btcoex_nss; /* 0:siso 1:mimo */
            uint8_t uc_grade;                /* 0级或者1级 */
            uint8_t uc_rx_size0;             /* size0大小 */
            uint8_t uc_rx_size1;             /* size1大小 */
            uint8_t uc_rx_size2;             /* size2大小 */
            uint8_t uc_rx_size3;             /* size3大小 */
        } rx_size;
        struct {
            oal_bool_enum_uint8 en_rssi_limit_on;
            oal_bool_enum_uint8 en_rssi_log_on;
            uint8_t uc_cfg_rssi_detect_cnt; /* 6 rssi 配置时用于防护次数  高低门限配置 */
            int8_t c_cfg_rssi_detect_mcm_down_th; /* m2s */
            int8_t c_cfg_rssi_detect_mcm_up_th; /* s2m */
        } rssi_param;
    } pri_data;
} mac_btcoex_mgr_stru;

typedef struct {
    /* 0:硬件preempt; 1:软件preempt 2:ps 提前slot量 */
    uint8_t uc_cfg_preempt_mode;
    /* 0 noframe 1 self-cts 2 nulldata 3 qosnull  0/1 软件ps打开或者关闭 */
    uint8_t uc_cfg_preempt_type;
} mac_btcoex_preempt_mgr_stru;
/* m2s device信息结构体 */
typedef struct {
    uint8_t uc_device_id;                           /* 业务vap id */
    wlan_nss_enum_uint8 en_nss_num;                   /* device的接收空间流个数 */
    wlan_mib_mimo_power_save_enum_uint8 en_smps_mode; /* mac device的smps能力，用于切换后vap能力初始化 */
    uint8_t auc_reserved[1];
} mac_device_m2s_stru;

typedef enum {
    MAC_PSM_QUERY_FLT_STAT = 0,
    MAC_PSM_QUERY_FASTSLEEP_STAT = 1,
#ifdef _PRE_WLAN_FEATURE_PSM_DFX_EXT
    MAC_PSM_QUERY_BEACON_CNT = 2,
#endif

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    MAC_PSM_QUERY_RX_LISTEN_STATE = 3,
#endif
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
    MAC_PSM_QUERY_ABNORMAL_PKTS_CNT = 4,
#endif

    MAC_PSM_QUERY_TYPE_BUTT
} mac_psm_query_type_enum;
typedef uint8_t mac_psm_query_type_enum_uint8;
#define MAC_PSM_QUERY_MSG_MAX_STAT_ITEM  10
typedef struct {
    uint32_t                          ul_query_item;
    uint32_t                          aul_val[MAC_PSM_QUERY_MSG_MAX_STAT_ITEM];
}mac_psm_query_stat_stru;

typedef struct {
    mac_psm_query_type_enum_uint8 en_query_type;
    mac_psm_query_stat_stru st_stat;
}mac_psm_query_msg;

enum {
    FREQ_SET_MODE = 0,
    /* sync ini data */
    FREQ_SYNC_DATA = 1,
    /* for device debug */
    FREQ_SET_FREQ = 2,
    FREQ_SET_PLAT_FREQ = 3,
    FREQ_GET_FREQ = 4,
    FREQ_SET_FREQ_TC_EN = 5,
    FREQ_SET_FREQ_TC_EXIT = 6,
    FREQ_SET_BUTT
};
typedef uint8_t oal_freq_sync_enum_uint8;

typedef struct {
    uint32_t ul_speed_level;    /* 吞吐量门限 */
    uint32_t ul_cpu_freq_level; /* CPU频率level */
} device_level_stru;

typedef struct {
    uint8_t uc_set_type;
    uint8_t uc_set_freq;
    uint8_t uc_device_freq_enable;
    uint8_t uc_resv;
    device_level_stru st_device_data[4];
} config_device_freq_h2d_stru;

/* rx ip数据包过滤的配置命令 */
typedef enum {
    MAC_IP_FILTER_ENABLE = 0,        /* 开/关ip数据包过滤功能 */
    MAC_IP_FILTER_UPDATE_BTABLE = 1, /* 更新黑名单 */
    MAC_IP_FILTER_CLEAR = 2,         /* 清除黑名单 */

    MAC_IP_FILTER_BUTT
} mac_ip_filter_cmd_enum;
typedef uint8_t mac_ip_filter_cmd_enum_uint8;

/* 黑名单条目格式 */
typedef struct {
    uint16_t us_port; /* 目的端口号，以主机字节序格式存储 */
    uint8_t uc_protocol;
    uint8_t uc_resv;
    // uint32_t                  ul_filter_cnt;     /* 目前未接受"统计过滤包数量"的需求，此成员暂不使用 */
} mac_ip_filter_item_stru;

/* 配置命令格式 */
typedef struct {
    uint8_t uc_item_count;
    oal_bool_enum_uint8 en_enable; /* 下发功能使能标志 */
    mac_ip_filter_cmd_enum_uint8 en_cmd;
    uint8_t uc_resv;
    mac_ip_filter_item_stru ast_filter_items[1];
} mac_ip_filter_cmd_stru;

/* rx方向指定数据包过滤配置命令 */
typedef enum {
    MAC_ICMP_FILTER = 1, /* icmp数据包过滤功能 */

    MAC_FILTER_ID_BUTT
} mac_assigned_filter_id_enum;
typedef uint8_t mac_assigned_filter_id_enum_uint8;

/* 指定filter配置命令格式 */
typedef struct {
    mac_assigned_filter_id_enum_uint8 en_filter_id;
    oal_bool_enum_uint8 en_enable; /* 下发功能使能标志 */
    uint8_t uc_resv[2];
} mac_assigned_filter_cmd_stru;

typedef enum {
    MAC_RX_ICMP_FILTER_STOPED = 0,   // 功能关闭，未使能、或者其他状况不允许过滤动作。
    MAC_RX_ICMP_FILTER_WORKING = 1,  // 功能打开，按照规则正常过滤
    MAC_RX_ICMP_FILTER_BUTT
} mac_icmp_filter_state_enum;

typedef uint8_t mac_icmp_filter_state_enum_uint8;

typedef struct {
    mac_assigned_filter_id_enum_uint8 en_filter_id;
    mac_icmp_filter_state_enum_uint8 en_state;  // 功能状态：过滤、非过滤等
    uint8_t uc_resv[2];
} mac_rx_icmp_filter_struc;

#ifdef _PRE_WLAN_FEATURE_APF
#define APF_PROGRAM_MAX_LEN 512
#define APF_VERSION         2
typedef enum {
    APF_SET_FILTER_CMD,
    APF_GET_FILTER_CMD
} mac_apf_cmd_type_enum;
typedef uint8_t mac_apf_cmd_type_uint8;

typedef struct {
    oal_bool_enum_uint8 en_is_enabled;
    uint16_t us_program_len;
    uint32_t ul_install_timestamp;
    uint8_t auc_program[APF_PROGRAM_MAX_LEN];
} mac_apf_stru;

typedef struct {
    mac_apf_cmd_type_uint8 en_cmd_type;
    uint16_t us_program_len;
    uint8_t *puc_program;
} mac_apf_filter_cmd_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
typedef struct {
    uint8_t uc_core_idx;
    oal_bool_enum_uint8 en_need_improved;
    uint8_t auc_rev[2];
} mac_cfg_tas_pwr_ctrl_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
typedef struct {
    oal_bool_enum_uint8 en_is_pk_mode;
    wlan_bw_cap_enum_uint8 en_curr_bw_cap;             /* 目前使用的带宽，本参数仅在lagency sta模式下生效 */
    wlan_protocol_cap_enum_uint8 en_curr_protocol_cap; /* 目前使用的协议模式，本参数仅在lagency sta模式下生效 */
    wlan_nss_enum_uint8 en_curr_num_spatial_stream;    /* 目前单双流的计数 */

    uint32_t ul_tx_bytes; /* WIFI 业务发送帧统计 */
    uint32_t ul_rx_bytes; /* WIFI 业务接收帧统计 */
    uint32_t ul_dur_time; /* 统计时间间隔 */
} mac_cfg_pk_mode_stru;
#endif

/* MSS使用时命令形式 */
typedef enum {
    MAC_M2S_COMMAND_MODE_SET_AUTO = 0,
    MAC_M2S_COMMAND_MODE_SET_SISO_C0 = 1,
    MAC_M2S_COMMAND_MODE_SET_SISO_C1 = 2,
    MAC_M2S_COMMAND_MODE_SET_MIMO = 3,
    MAC_M2S_COMMAND_MODE_GET_STATE = 4,

    MAC_M2S_COMMAND_MODE_BUTT,
} mac_m2s_command_mode_enum;
typedef oal_uint8 mac_m2s_command_mode_enum_uint8;

/* 配置命令使用，mimo-siso切换mode */
typedef enum {
    MAC_M2S_MODE_QUERY          = 0,    /* 参数查询模式 */
    MAC_M2S_MODE_MSS            = 1,    /* MSS下发模式 */
    MAC_M2S_MODE_DELAY_SWITCH   = 2,    /* 延迟切换测试模式 */
    MAC_M2S_MODE_SW_TEST        = 3,    /* 软切换测试模式,测试siso和mimo */
    MAC_M2S_MODE_HW_TEST        = 4,    /* 硬切换测试模式,测试siso和mimo */
    MAC_M2S_MODE_RSSI           = 5,    /* rssi切换 */
    MAC_M2S_MODE_MODEM          = 6,    /* MODEM切换模式 */

    MAC_M2S_MODE_BUTT,
} mac_m2s_mode_enum;
typedef uint8_t mac_m2s_mode_enum_uint8;

/* MODEM命令使用，期望切换状态 */
typedef enum {
    MAC_MCM_MODEM_STATE_SISO_C0     = 0,
    MAC_MCM_MODEM_STATE_MIMO        = 2,

    MAC_MCM_MODEM_STATE_BUTT,
} mac_mcm_modem_state_enum;
typedef uint8_t mac_mcm_modem_state_enum_uint8;

typedef struct {
    /* 0:参数查询模式; 1:参数配置模式;2.切换模式;3.软切换测试模式;4.硬切换测试模式 5.RSSI配置命令 */
    mac_m2s_mode_enum_uint8         en_cfg_m2s_mode;
    union {
        struct {
            oal_bool_enum_uint8      en_m2s_type;    /* 切换类型 */
            uint8_t                uc_master_id;   /* 主辅路id */
            uint8_t uc_m2s_state;   /* 期望切换到状态 */
            wlan_m2s_trigger_mode_enum_uint8 uc_trigger_mode; /* 切换触发业务模式 */
        } test_mode;

        struct {
            oal_bool_enum_uint8 en_mss_on;
        } mss_mode;

        struct {
            uint8_t uc_opt;
            int8_t c_value;
        } rssi_mode;

        struct {
            mac_mcm_modem_state_enum_uint8    en_m2s_state;   /* 期望切换到状态 */
        } modem_mode;
    } pri_data;
} mac_m2s_mgr_stru;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
#endif /* end of mac_device_common.h */
