
#ifndef __MAC_DEVICE_COMMON_H__
#define __MAC_DEVICE_COMMON_H__

#include "wlan_mib.h"

#define MAC_DATARATES_PHY_80211G_NUM 12

#define MAC_RX_BA_LUT_BMAP_LEN ((HAL_MAX_RX_BA_LUT_SIZE + 7) >> 3)
#define MAC_TX_BA_LUT_BMAP_LEN ((HAL_MAX_TX_BA_LUT_SIZE + 7) >> 3)

#define MAC_SCAN_CHANNEL_INTERVAL_DEFAULT         6   /* ���6���ŵ����лع����ŵ�����һ��ʱ�� */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_DEFAULT     110 /* ����ɨ��ʱ�����ع����ŵ�������ʱ�� */
#define MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE     2   /* ���2���ŵ����лع����ŵ�����һ��ʱ�� */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE 60  /* WLANδ������P2P���������ع����ŵ�������ʱ�� */
#define MAC_SCAN_CHANNEL_INTERVAL_HIDDEN_SSID     3   /* Я������SSID�ı���ɨ�裬���3���ŵ��ع����ŵ�����һ��ʱ�� */
#define MAC_MAX_IP_FILTER_BTABLE_SIZE 512 /* rx ip���ݰ����˹��ܵĺ�������С */

/* �ϱ��ؼ���Ϣ��flags�����Ϣ����Ӧ���λΪ1������ϱ���Ӧ��Ϣ */
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

/* DMAC SCANNER ɨ��ģʽ */
#define MAC_SCAN_FUNC_MEAS       0x1
#define MAC_SCAN_FUNC_STATS      0x2
#define MAC_SCAN_FUNC_RADAR      0x4 // WTBD δ��
#define MAC_SCAN_FUNC_BSS        0x8
#define MAC_SCAN_FUNC_P2P_LISTEN 0x10
#define MAC_SCAN_FUNC_ALL        \
    (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS | MAC_SCAN_FUNC_RADAR | MAC_SCAN_FUNC_BSS)

/* ɨ������¼�����״̬�� */
typedef enum {
    MAC_SCAN_SUCCESS = 0,    /* ɨ��ɹ� */
    MAC_SCAN_PNO = 1,        /* pnoɨ����� */
    MAC_SCAN_TIMEOUT = 2,    /* ɨ�賬ʱ */
    MAC_SCAN_REFUSED = 3,    /* ɨ�豻�ܾ� */
    MAC_SCAN_ABORT = 4,      /* ��ֹɨ�� */
    MAC_SCAN_ABORT_SYNC = 5, /* ɨ�豻��ֹͬ��״̬�������ϲ�ȥ��������ʱǿ��abort�����ں��ϱ���dmac��Ӧabort���ϱ� */
    MAC_SCAN_STATUS_BUTT,    /* ��Ч״̬�룬��ʼ��ʱʹ�ô�״̬�� */
} mac_scan_status_enum;
typedef uint8_t mac_scan_status_enum_uint8;

typedef enum {
    MAC_CH_TYPE_NONE = 0,
    MAC_CH_TYPE_PRIMARY = 1,
    MAC_CH_TYPE_SECONDARY = 2,
} mac_ch_type_enum;
typedef uint8_t mac_ch_type_enum_uint8;

/* SDT����ģʽö�� */
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

/* ɨ���� */
typedef struct {
    mac_scan_status_enum_uint8 en_scan_rsp_status;
    uint8_t auc_resv[3];
    uint64_t ull_cookie;
} mac_scan_rsp_stru;

typedef enum {
    MAC_CHAN_NOT_SUPPORT = 0,      /* ������֧�ָ��ŵ� */
    MAC_CHAN_AVAILABLE_ALWAYS,     /* �ŵ�һֱ����ʹ�� */
    MAC_CHAN_AVAILABLE_TO_OPERATE, /* �������(CAC, etc...)�󣬸��ŵ�����ʹ�� */
    MAC_CHAN_DFS_REQUIRED,         /* ���ŵ���Ҫ�����״��� */
    MAC_CHAN_BLOCK_DUE_TO_RADAR,   /* ���ڼ�⵽�״ﵼ�¸��ŵ���Ĳ����� */

    MAC_CHAN_STATUS_BUTT
} mac_chan_status_enum;
typedef uint8_t mac_chan_status_enum_uint8;

/* device resetͬ��������ö�� */
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
  4 ȫ�ֱ�������
*****************************************************************************/
/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/
/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
#pragma pack(push, 1)
/* �ϱ���ɨ��������չ��Ϣ�������ϱ�host��Ĺ���֡netbuf�ĺ��� */
typedef struct {
    int32_t l_rssi;                                /* �ź�ǿ�� */
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type; /* ɨ�赽��bss���� */
    uint8_t auc_resv[3]; /* Ԥ���ֶ� */

    int8_t c_ant0_rssi; /* ����0��rssi */
    int8_t c_ant1_rssi; /* ����1��rssi */
    int8_t   ant2_rssi;   /* ����2��rssi */
    int8_t   ant3_rssi;   /* ����3��rssi */

#ifdef _PRE_WLAN_FEATURE_M2S
    wlan_nss_enum_uint8 en_support_max_nss; /* ��AP֧�ֵ����ռ����� */
    oal_bool_enum_uint8 en_support_opmode;  /* ��AP�Ƿ�֧��OPMODE */
    uint8_t uc_num_sounding_dim;          /* ��AP����txbf�������� */
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

/* ������Կ�����صĽṹ�� */
typedef enum {
    MAC_CSA_FLAG_NORMAL = 0,
    MAC_CSA_FLAG_START_DEBUG, /* �̶�csa ie ��beacon֡�� */
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
    mac_csa_flag_enum_uint8 en_debug_flag; /* 0:�������ŵ�; 1:��beacon֡�к���csa,�ŵ����л�;2:ȡ��beacon֡�к���csa */
    uint8_t auc_reserv[3];
} mac_csa_debug_stru;

typedef struct {
    uint32_t ul_cmd_bit_map;
    oal_bool_enum_uint8 en_band_force_switch_bit0; /* �ָ�40M�������� */
    oal_bool_enum_uint8 en_2040_ch_swt_prohi_bit1; /* ������20/40�����л����� */
    oal_bool_enum_uint8 en_40_intolerant_bit2;     /* ������40M������ */
    uint8_t uc_resv;
    mac_csa_debug_stru st_csa_debug_bit3;
    oal_bool_enum_uint8 en_lsigtxop_bit5; /* lsigtxopʹ�� */
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
    uint8_t uc_srb_switch; /* siso��beacon���� */
    uint8_t uc_dto_switch; /* ��̬tbtt offset���� */
    uint8_t uc_nfi_switch;
    uint8_t uc_apf_switch;
    uint8_t uc_ao_switch;
} mac_pm_debug_cfg_stru;

typedef enum {
    MAC_DBDC_CHANGE_HAL_DEV = 0, /* vap change hal device hal vap */
    MAC_DBDC_SWITCH = 1,         /* DBDC������� */
    MAC_FAST_SCAN_SWITCH = 2,    /* ����ɨ�迪�� */
    MAC_DBDC_STATUS = 3,         /* DBDC״̬��ѯ */

    MAC_DBDC_CMD_BUTT
} mac_dbdc_cmd_enum;
typedef uint8_t mac_dbdc_cmd_enum_uint8;

typedef struct {
    uint32_t ul_cmd_bit_map;
    uint8_t uc_dst_hal_dev_id; /* ��ҪǨ�Ƶ���hal device id */
    uint8_t uc_dbdc_enable;
    oal_bool_enum_uint8 en_fast_scan_enable; /* �Ƿ���Բ���,XXԭ��ʹӲ��֧��Ҳ���ܿ���ɨ�� */
    uint8_t uc_dbdc_status;
} mac_dbdc_debug_switch_stru;

/* ACS ����ظ���ʽ */
typedef struct {
    uint8_t uc_cmd;
    uint8_t uc_chip_id;
    uint8_t uc_device_id;
    uint8_t uc_resv;

    uint32_t ul_len;     /* �ܳ��ȣ���������ǰ4���ֽ� */
    uint32_t ul_cmd_cnt; /* ����ļ��� */
} mac_acs_response_hdr_stru;

typedef struct {
    wlan_m2s_mgr_vap_stru ast_m2s_blacklist[WLAN_M2S_BLACKLIST_MAX_NUM];
    uint8_t uc_blacklist_cnt;
} mac_m2s_ie_stru;

typedef struct {
    uint8_t uc_cfg_btcoex_mode; /* 0:������ѯģʽ; 1:��������ģʽ */
    uint8_t uc_cfg_btcoex_type; /* 0:��������; 1:�ۺϴ�С���� 2.rssi detect���޲�������ģʽ */
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
            uint8_t uc_grade;                /* 0������1�� */
            uint8_t uc_rx_size0;             /* size0��С */
            uint8_t uc_rx_size1;             /* size1��С */
            uint8_t uc_rx_size2;             /* size2��С */
            uint8_t uc_rx_size3;             /* size3��С */
        } rx_size;
        struct {
            oal_bool_enum_uint8 en_rssi_limit_on;
            oal_bool_enum_uint8 en_rssi_log_on;
            uint8_t uc_cfg_rssi_detect_cnt; /* 6 rssi ����ʱ���ڷ�������  �ߵ��������� */
            int8_t c_cfg_rssi_detect_mcm_down_th; /* m2s */
            int8_t c_cfg_rssi_detect_mcm_up_th; /* s2m */
        } rssi_param;
    } pri_data;
} mac_btcoex_mgr_stru;

typedef struct {
    /* 0:Ӳ��preempt; 1:���preempt 2:ps ��ǰslot�� */
    uint8_t uc_cfg_preempt_mode;
    /* 0 noframe 1 self-cts 2 nulldata 3 qosnull  0/1 ���ps�򿪻��߹ر� */
    uint8_t uc_cfg_preempt_type;
} mac_btcoex_preempt_mgr_stru;
/* m2s device��Ϣ�ṹ�� */
typedef struct {
    uint8_t uc_device_id;                           /* ҵ��vap id */
    wlan_nss_enum_uint8 en_nss_num;                   /* device�Ľ��տռ������� */
    wlan_mib_mimo_power_save_enum_uint8 en_smps_mode; /* mac device��smps�����������л���vap������ʼ�� */
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
    uint32_t ul_speed_level;    /* ���������� */
    uint32_t ul_cpu_freq_level; /* CPUƵ��level */
} device_level_stru;

typedef struct {
    uint8_t uc_set_type;
    uint8_t uc_set_freq;
    uint8_t uc_device_freq_enable;
    uint8_t uc_resv;
    device_level_stru st_device_data[4];
} config_device_freq_h2d_stru;

/* rx ip���ݰ����˵��������� */
typedef enum {
    MAC_IP_FILTER_ENABLE = 0,        /* ��/��ip���ݰ����˹��� */
    MAC_IP_FILTER_UPDATE_BTABLE = 1, /* ���º����� */
    MAC_IP_FILTER_CLEAR = 2,         /* ��������� */

    MAC_IP_FILTER_BUTT
} mac_ip_filter_cmd_enum;
typedef uint8_t mac_ip_filter_cmd_enum_uint8;

/* ��������Ŀ��ʽ */
typedef struct {
    uint16_t us_port; /* Ŀ�Ķ˿ںţ��������ֽ����ʽ�洢 */
    uint8_t uc_protocol;
    uint8_t uc_resv;
    // uint32_t                  ul_filter_cnt;     /* Ŀǰδ����"ͳ�ƹ��˰�����"�����󣬴˳�Ա�ݲ�ʹ�� */
} mac_ip_filter_item_stru;

/* ���������ʽ */
typedef struct {
    uint8_t uc_item_count;
    oal_bool_enum_uint8 en_enable; /* �·�����ʹ�ܱ�־ */
    mac_ip_filter_cmd_enum_uint8 en_cmd;
    uint8_t uc_resv;
    mac_ip_filter_item_stru ast_filter_items[1];
} mac_ip_filter_cmd_stru;

/* rx����ָ�����ݰ������������� */
typedef enum {
    MAC_ICMP_FILTER = 1, /* icmp���ݰ����˹��� */

    MAC_FILTER_ID_BUTT
} mac_assigned_filter_id_enum;
typedef uint8_t mac_assigned_filter_id_enum_uint8;

/* ָ��filter���������ʽ */
typedef struct {
    mac_assigned_filter_id_enum_uint8 en_filter_id;
    oal_bool_enum_uint8 en_enable; /* �·�����ʹ�ܱ�־ */
    uint8_t uc_resv[2];
} mac_assigned_filter_cmd_stru;

typedef enum {
    MAC_RX_ICMP_FILTER_STOPED = 0,   // ���ܹرգ�δʹ�ܡ���������״����������˶�����
    MAC_RX_ICMP_FILTER_WORKING = 1,  // ���ܴ򿪣����չ�����������
    MAC_RX_ICMP_FILTER_BUTT
} mac_icmp_filter_state_enum;

typedef uint8_t mac_icmp_filter_state_enum_uint8;

typedef struct {
    mac_assigned_filter_id_enum_uint8 en_filter_id;
    mac_icmp_filter_state_enum_uint8 en_state;  // ����״̬�����ˡ��ǹ��˵�
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
    wlan_bw_cap_enum_uint8 en_curr_bw_cap;             /* Ŀǰʹ�õĴ�������������lagency staģʽ����Ч */
    wlan_protocol_cap_enum_uint8 en_curr_protocol_cap; /* Ŀǰʹ�õ�Э��ģʽ������������lagency staģʽ����Ч */
    wlan_nss_enum_uint8 en_curr_num_spatial_stream;    /* Ŀǰ��˫���ļ��� */

    uint32_t ul_tx_bytes; /* WIFI ҵ����֡ͳ�� */
    uint32_t ul_rx_bytes; /* WIFI ҵ�����֡ͳ�� */
    uint32_t ul_dur_time; /* ͳ��ʱ���� */
} mac_cfg_pk_mode_stru;
#endif

/* MSSʹ��ʱ������ʽ */
typedef enum {
    MAC_M2S_COMMAND_MODE_SET_AUTO = 0,
    MAC_M2S_COMMAND_MODE_SET_SISO_C0 = 1,
    MAC_M2S_COMMAND_MODE_SET_SISO_C1 = 2,
    MAC_M2S_COMMAND_MODE_SET_MIMO = 3,
    MAC_M2S_COMMAND_MODE_GET_STATE = 4,

    MAC_M2S_COMMAND_MODE_BUTT,
} mac_m2s_command_mode_enum;
typedef oal_uint8 mac_m2s_command_mode_enum_uint8;

/* ��������ʹ�ã�mimo-siso�л�mode */
typedef enum {
    MAC_M2S_MODE_QUERY          = 0,    /* ������ѯģʽ */
    MAC_M2S_MODE_MSS            = 1,    /* MSS�·�ģʽ */
    MAC_M2S_MODE_DELAY_SWITCH   = 2,    /* �ӳ��л�����ģʽ */
    MAC_M2S_MODE_SW_TEST        = 3,    /* ���л�����ģʽ,����siso��mimo */
    MAC_M2S_MODE_HW_TEST        = 4,    /* Ӳ�л�����ģʽ,����siso��mimo */
    MAC_M2S_MODE_RSSI           = 5,    /* rssi�л� */
    MAC_M2S_MODE_MODEM          = 6,    /* MODEM�л�ģʽ */

    MAC_M2S_MODE_BUTT,
} mac_m2s_mode_enum;
typedef uint8_t mac_m2s_mode_enum_uint8;

/* MODEM����ʹ�ã������л�״̬ */
typedef enum {
    MAC_MCM_MODEM_STATE_SISO_C0     = 0,
    MAC_MCM_MODEM_STATE_MIMO        = 2,

    MAC_MCM_MODEM_STATE_BUTT,
} mac_mcm_modem_state_enum;
typedef uint8_t mac_mcm_modem_state_enum_uint8;

typedef struct {
    /* 0:������ѯģʽ; 1:��������ģʽ;2.�л�ģʽ;3.���л�����ģʽ;4.Ӳ�л�����ģʽ 5.RSSI�������� */
    mac_m2s_mode_enum_uint8         en_cfg_m2s_mode;
    union {
        struct {
            oal_bool_enum_uint8      en_m2s_type;    /* �л����� */
            uint8_t                uc_master_id;   /* ����·id */
            uint8_t uc_m2s_state;   /* �����л���״̬ */
            wlan_m2s_trigger_mode_enum_uint8 uc_trigger_mode; /* �л�����ҵ��ģʽ */
        } test_mode;

        struct {
            oal_bool_enum_uint8 en_mss_on;
        } mss_mode;

        struct {
            uint8_t uc_opt;
            int8_t c_value;
        } rssi_mode;

        struct {
            mac_mcm_modem_state_enum_uint8    en_m2s_state;   /* �����л���״̬ */
        } modem_mode;
    } pri_data;
} mac_m2s_mgr_stru;

/*****************************************************************************
  8 UNION����
*****************************************************************************/
/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
/*****************************************************************************
  10 ��������
*****************************************************************************/
#endif /* end of mac_device_common.h */
