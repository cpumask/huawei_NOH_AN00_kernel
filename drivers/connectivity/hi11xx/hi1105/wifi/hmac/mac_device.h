

#ifndef __MAC_DEVICE_H__
#define __MAC_DEVICE_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_workqueue.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "wlan_spec.h"
#include "wlan_mib.h"
#include "mac_regdomain.h"
#include "mac_frame.h"
#include "mac_device_common.h"
#include "wlan_types.h"
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hiex_msg.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_DEVICE_H

#define MAC_NET_DEVICE_NAME_LENGTH 16 // WTBD ����
#define WLAN_USER_MAX_SUPP_RATES 16 /* ���ڼ�¼�Զ��豸֧�ֵ����������� */

/* ��λ״̬ */
#define MAC_DEV_RESET_IN_PROGRESS(_pst_device, uc_value) ((_pst_device)->uc_device_reset_in_progress = (uc_value))
#define MAC_DEV_IS_RESET_IN_PROGRESS(_pst_device)        ((_pst_device)->uc_device_reset_in_progress)

#define MAC_DEVICE_GET_CAP_BW(_pst_device)     ((_pst_device)->st_device_cap.en_channel_width)
#define MAC_DEVICE_GET_NSS_NUM(_pst_device)    ((_pst_device)->st_device_cap.en_nss_num)
#define MAC_DEVICE_GET_CAP_LDPC(_pst_device)   ((_pst_device)->st_device_cap.en_ldpc_is_supp)
#define MAC_DEVICE_GET_CAP_TXSTBC(_pst_device) ((_pst_device)->st_device_cap.en_tx_stbc_is_supp)
#define MAC_DEVICE_GET_CAP_RXSTBC(_pst_device) ((_pst_device)->st_device_cap.en_rx_stbc_is_supp)
#define MAC_DEVICE_GET_CAP_SUBFER(_pst_device) ((_pst_device)->st_device_cap.en_su_bfmer_is_supp)
#define MAC_DEVICE_GET_CAP_SUBFEE(_pst_device) ((_pst_device)->st_device_cap.en_su_bfmee_is_supp)
#define MAC_DEVICE_GET_CAP_MUBFER(_pst_device) ((_pst_device)->st_device_cap.en_mu_bfmer_is_supp)
#define MAC_DEVICE_GET_CAP_MUBFEE(_pst_device) ((_pst_device)->st_device_cap.en_mu_bfmee_is_supp)
#ifdef _PRE_WLAN_FEATURE_SMPS
#define MAC_DEVICE_GET_MODE_SMPS(_pst_device) ((_pst_device)->en_mac_smps_mode)
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
#define MAC_M2S_CALI_NSS_FROM_SMPS_MODE(en_smps_mode) \
    (((en_smps_mode) == WLAN_MIB_MIMO_POWER_SAVE_STATIC) ? WLAN_SINGLE_NSS : WLAN_DOUBLE_NSS)
#endif

/* p2p�ṹ�а�����״̬��Ա���ýṹ����mac device�£���VAP״̬ö���ƶ���mac_device.h�� */
/* VAP״̬����AP STA����һ��״̬ö�� */
typedef enum {
    /* ap sta����״̬ */
    MAC_VAP_STATE_INIT = 0,
    MAC_VAP_STATE_UP = 1,    /* VAP UP */
    MAC_VAP_STATE_PAUSE = 2, /* pause , for ap &sta */

    /* ap ����״̬ */
    MAC_VAP_STATE_AP_WAIT_START = 3,

    /* sta����״̬ */
    MAC_VAP_STATE_STA_FAKE_UP = 4,
    MAC_VAP_STATE_STA_WAIT_SCAN = 5,
    MAC_VAP_STATE_STA_SCAN_COMP = 6,
    MAC_VAP_STATE_STA_JOIN_COMP = 7,
    MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2 = 8,
    MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4 = 9,
    MAC_VAP_STATE_STA_AUTH_COMP = 10,
    MAC_VAP_STATE_STA_WAIT_ASOC = 11,
    MAC_VAP_STATE_STA_OBSS_SCAN = 12,
    MAC_VAP_STATE_STA_BG_SCAN = 13,
    MAC_VAP_STATE_STA_LISTEN = 14, /* p2p0 ���� */
    MAC_VAP_STATE_ROAMING = 15,    /* ���� */
    MAC_VAP_STATE_BUTT,
} mac_vap_state_enum;
typedef uint8_t mac_vap_state_enum_uint8;

typedef enum {
    MAC_DEVICE_DISABLE = 0,
    MAC_DEVICE_2G,
    MAC_DEVICE_5G,
    MAC_DEVICE_2G_5G,

    MAC_DEVICE_BUTT,
} mac_device_radio_cap_enum;
typedef uint8_t mac_device_radio_cap_enum_uint8;

/* btcoex���������� */
typedef enum {
    MAC_BTCOEX_BLACKLIST_LEV0 = BIT0, /* 0������������Ҫ�����жϣ����翴mac��ַ */
    MAC_BTCOEX_BLACKLIST_LEV1 = BIT1, /* һ������������mac��ַ */
    MAC_BTCOEX_BLACKLIST_BUTT,
} mac_btcoex_blacklist_enum;
typedef uint8_t mac_btcoex_blacklist_enum_uint8;

extern uint8_t g_auc_valid_blacklist_idx[];
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
extern mac_cfg_pk_mode_stru g_st_wifi_pk_mode_status;
#endif

/* device reset�¼�ͬ���ṹ�� */
typedef struct {
    mac_reset_sys_type_enum_uint8 en_reset_sys_type; /* ��λͬ������ */
    uint8_t uc_value;                              /* ͬ����Ϣֵ */
    uint8_t uc_resv[2];
} mac_reset_sys_stru;

typedef void (*mac_scan_cb_fn)(void *p_scan_record);

typedef struct {
    uint16_t us_num_networks;
    mac_ch_type_enum_uint8 en_ch_type;
#ifdef _PRE_WLAN_FEATURE_DFS
    mac_chan_status_enum_uint8 en_ch_status;
#else
    uint8_t auc_resv[1];
#endif
} mac_ap_ch_info_stru;

typedef enum {
    MAC_P2P_SCENES_LOW_LATENCY   = 0, /* ��ʱ�ӳ��� */
    MAC_P2P_SCENES_HI_THROUGHPUT = 1, /* �����³���,�Ƚ׶�ֻ����hishare */

    AC_P2P_SCENES_BUTT
} mac_p2p_scenes_enum;
typedef uint8_t mac_p2p_scenes_enum_uint8;

typedef struct {
    uint8_t uc_p2p_device_num;                /* ��ǰdevice�µ�P2P_DEVICE���� */
    uint8_t uc_p2p_goclient_num;              /* ��ǰdevice�µ�P2P_CL/P2P_GO���� */
    uint8_t uc_p2p0_vap_idx;                  /* P2P ���泡���£�P2P_DEV(p2p0) ָ�� */
    mac_vap_state_enum_uint8 en_last_vap_state; /* P2P0/P2P_CL ����VAP �ṹ�����������±���VAP �������ǰ��״̬ */
    mac_p2p_scenes_enum_uint8 p2p_scenes;       /* p2p ҵ�񳡾�:��ʱ�ӡ������� */
    uint8_t uc_resv[1];                       /* ���� */
    uint8_t en_roc_need_switch;               /* remain on channel����Ҫ�л�ԭ�ŵ� */
    uint8_t en_p2p_ps_pause;                  /* P2P �����Ƿ���pause״̬ */
    oal_net_device_stru *pst_p2p_net_device;    /* P2P ���泡������net_device(p2p0) ָ�� */
    uint64_t ull_send_action_id;              /* P2P action id/cookie */
    uint64_t ull_last_roc_id;
    oal_ieee80211_channel_stru st_listen_channel;
    oal_nl80211_channel_type en_listen_channel_type;
    oal_net_device_stru *pst_primary_net_device; /* P2P ���泡������net_device(wlan0) ָ�� */
    oal_net_device_stru *pst_second_net_device;  /* �ŵ���������,������ʹ�ÿ���ɾ�� */
} mac_p2p_info_stru;

typedef struct {
    uint16_t us_num_networks; /* ��¼��ǰ�ŵ���ɨ�赽��BSS���� */
    uint8_t auc_resv[2];
    uint8_t auc_bssid_array[WLAN_MAX_SCAN_BSS_PER_CH][WLAN_MAC_ADDR_LEN]; /* ��¼��ǰ�ŵ���ɨ�赽������BSSID */
} mac_bss_id_list_stru;

#define MAX_PNO_SSID_COUNT      16
#define MAX_PNO_REPEAT_TIMES    4
#define PNO_SCHED_SCAN_INTERVAL (60 * 1000)

/* PNOɨ����Ϣ�ṹ�� */
typedef struct {
    uint8_t auc_ssid[WLAN_SSID_MAX_LEN];
    oal_bool_enum_uint8 en_scan_ssid;
    uint8_t auc_resv[2];
} pno_match_ssid_stru;

typedef struct {
    pno_match_ssid_stru ast_match_ssid_set[MAX_PNO_SSID_COUNT];
    int32_t l_ssid_count;                         /* �·�����Ҫƥ���ssid���ĸ��� */
    int32_t l_rssi_thold;                         /* ���ϱ���rssi���� */
    uint32_t ul_pno_scan_interval;                /* pnoɨ���� */
    uint8_t auc_sour_mac_addr[WLAN_MAC_ADDR_LEN]; /* probe req֡��Я���ķ��Ͷ˵�ַ */
    uint8_t uc_pno_scan_repeat;                   /* pnoɨ���ظ����� */
    oal_bool_enum_uint8 en_is_random_mac_addr_scan; /* �Ƿ����mac */

    mac_scan_cb_fn p_fn_cb; /* ����ָ������������˼�ͨ�ų����� */
} mac_pno_scan_stru;

/* PNO����ɨ�����ṹ�� */
typedef struct {
    mac_pno_scan_stru st_pno_sched_scan_params; /* pno����ɨ������Ĳ��� */
    // frw_timeout_stru        st_pno_sched_scan_timer;              /* pno����ɨ�趨ʱ�� */
    void *p_pno_sched_scan_timer;           /* pno����ɨ��rtcʱ�Ӷ�ʱ�����˶�ʱ����ʱ���ܹ�����˯�ߵ�device */
    uint8_t uc_curr_pno_sched_scan_times;     /* ��ǰpno����ɨ����� */
    oal_bool_enum_uint8 en_is_found_match_ssid; /* �Ƿ�ɨ�赽��ƥ���ssid */
    uint8_t auc_resv[2];
} mac_pno_sched_scan_mgmt_stru;

typedef struct {
    uint8_t auc_ssid[WLAN_SSID_MAX_LEN];
    uint8_t auc_resv[3];
} mac_ssid_stru;

typedef struct {
    uint8_t uc_mac_rate; /* MAC��Ӧ���� */
    uint8_t uc_phy_rate; /* PHY��Ӧ���� */
    uint8_t uc_mbps;     /* ���� */
    uint8_t auc_resv[1];
} mac_data_rate_stru;

/* ɨ������ṹ�� */
typedef struct {
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type; /* Ҫɨ���bss���� */
    wlan_scan_type_enum_uint8 en_scan_type;          /* ����/���� */
    uint8_t uc_bssid_num;                          /* ����ɨ���bssid���� */
    uint8_t uc_ssid_num;                           /* ����ɨ���ssid���� */

    uint8_t auc_sour_mac_addr[WLAN_MAC_ADDR_LEN]; /* probe req֡��Я���ķ��Ͷ˵�ַ */
    uint8_t uc_p2p0_listen_channel;
    uint8_t uc_max_scan_count_per_channel; /* ÿ���ŵ���ɨ����� */

    uint8_t auc_bssid[WLAN_SCAN_REQ_MAX_BSSID][WLAN_MAC_ADDR_LEN]; /* ������bssid */
    mac_ssid_stru ast_mac_ssid_set[WLAN_SCAN_REQ_MAX_SSID];          /* ������ssid */

    uint8_t uc_max_send_probe_req_count_per_channel; /* ÿ���ŵ�����ɨ������֡�ĸ�����Ĭ��Ϊ1 */
    uint8_t bit_is_p2p0_scan : 1;                    /* �Ƿ�Ϊp2p0 ����ɨ�� */
    uint8_t bit_is_radom_mac_saved : 1;              /* �Ƿ��Ѿ��������mac */
    uint8_t bit_radom_mac_saved_to_dev : 2;          /* ���ڲ���ɨ�� */
    uint8_t bit_desire_fast_scan : 1;                /* ����ɨ������ʹ�ò��� */
    uint8_t bit_roc_type_tx_mgmt : 1;             /* remain on channel�����Ƿ�Ϊ���͹���֡ */
    uint8_t bit_rsv : 2;                          /* ����λ */

    oal_bool_enum_uint8 en_abort_scan_flag;         /* ��ֹɨ�� */
    oal_bool_enum_uint8 en_is_random_mac_addr_scan; /* �Ƿ������mac addrɨ�� */

    oal_bool_enum_uint8 en_need_switch_back_home_channel; /* ����ɨ��ʱ��ɨ����һ���ŵ����ж��Ƿ���Ҫ�лع����ŵ����� */
    uint8_t uc_scan_channel_interval;                   /* ���n���ŵ����лع����ŵ�����һ��ʱ�� */
    uint16_t us_work_time_on_home_channel;              /* ����ɨ��ʱ�����ع����ŵ�������ʱ�� */

    mac_channel_stru ast_channel_list[WLAN_MAX_CHANNEL_NUM];

    uint8_t uc_channel_nums; /* �ŵ��б����ŵ��ĸ��� */
    uint8_t uc_probe_delay;  /* ����ɨ�跢probe request֮֡ǰ�ĵȴ�ʱ�� */
    uint16_t us_scan_time;   /* ɨ����ĳһ�ŵ�ͣ����ʱ���ɨ�����, ms��������10�������� */

    wlan_scan_mode_enum_uint8 en_scan_mode; /* ɨ��ģʽ:ǰ��ɨ�� or ����ɨ�� */
    uint8_t uc_resv;
    uint8_t uc_scan_func; /* DMAC SCANNER ɨ��ģʽ */
    uint8_t uc_vap_id;    /* �·�ɨ�������vap id */
    uint64_t ull_cookie;  /* P2P �����·���cookie ֵ */

    oal_bool_enum_uint8 uc_neighbor_report_process_flag; /* ��Ǵ˴�ɨ���Ƿ���neighbor reportɨ�� */

    oal_bool_enum_uint8 uc_bss_transition_process_flag;  /* ��Ǵ˴�ɨ���Ƿ���bss transitionɨ�� */

    /* ��Ҫ:�ص�����ָ��:����ָ������������˼�ͨ�ų����� */
    mac_scan_cb_fn p_fn_cb;
} mac_scan_req_stru;
/* �ں�flag����һ�� */
typedef struct {
    uint32_t ul_scan_flag; /* �ں��·���ɨ��ģʽ,ÿ��bit�����wlan_scan_flag_enum����ʱֻ�����Ƿ�Ϊ����ɨ�� */
    mac_scan_req_stru st_scan_params;
} mac_scan_req_h2d_stru;

/* ��ӡ���ձ��ĵ�rssi��Ϣ�ĵ��Կ�����صĽṹ�� */
typedef struct {
    uint16_t us_data_len;        /*  �������������  */
    uint8_t uc_tone_tran_switch; /*  �������Ϳ���  */
    uint8_t uc_chain_idx;        /*  ��������ͨ����  */
} mac_tone_transmit_stru;

/* ��ӡ���ձ��ĵ�rssi��Ϣ�ĵ��Կ�����صĽṹ�� */
typedef struct {
    oal_bool_enum_uint8 en_debug_switch;          /* ��ӡ�ܿ��� */
    oal_bool_enum_uint8 en_rssi_debug_switch;     /* ��ӡ���ձ��ĵ�rssi��Ϣ�ĵ��Կ��� */
    oal_bool_enum_uint8 en_snr_debug_switch;      /* ��ӡ���ձ��ĵ�snr��Ϣ�ĵ��Կ��� */
    oal_bool_enum_uint8 en_trlr_debug_switch;     /* ��ӡ���ձ��ĵ�trailer��Ϣ�ĵ��Կ��� */
    oal_bool_enum_uint8 en_evm_debug_switch;      /* ��ӡ���ձ��ĵ�evm��Ϣ�ĵ��Կ��� */
    oal_bool_enum_uint8 en_sp_reuse_debug_switch; /* ��ӡ���ձ��ĵ�spatial reuse��Ϣ�ĵ��Կ��� */
    uint8_t auc_resv[2];
    uint32_t ul_curr_rx_comp_isr_count;     /* һ�ּ���ڣ���������жϵĲ������� */
    uint32_t ul_rx_comp_isr_interval;       /* ������ٸ���������жϴ�ӡһ��rssi��Ϣ */
    mac_tone_transmit_stru st_tone_tran;      /* �������Ͳ��� */
    uint8_t auc_trlr_sel_info[5];           /* trailerѡ���ϱ�����, һ���ֽڸ�4bitָʾtrlr or vect,��4��bitָʾѡ�� */
    uint8_t uc_trlr_sel_num;                /* ��¼������������ѡ������ֵ */
    uint8_t uc_iq_cali_switch;              /* iqУ׼��������  */
    oal_bool_enum_uint8 en_pdet_debug_switch; /* ��ӡоƬ�ϱ�pdetֵ�ĵ��Կ��� */
    oal_bool_enum_uint8 en_tsensor_debug_switch;
    uint8_t uc_force_work_switch;
    uint8_t uc_dfr_reset_switch;         /* dfr_reset��������: ��4bitΪreset_mac_submod, ��4bitΪreset_hw_type */
    uint8_t uc_fsm_info_switch;          /* hal fsm debug info */
    uint8_t uc_report_radar_switch;      /* radar�ϱ����� */
    uint8_t uc_extlna_chg_bypass_switch; /* ���Ĳ��Թر�����LNA����: 0/1/2:no_bypass/dyn_bypass/force_bypass */
    uint8_t uc_edca_param_switch;        /* EDCA�������ÿ��� */
    uint8_t uc_edca_aifsn;               /* edca����AIFSN */
    uint8_t uc_edca_cwmin;               /* edca����CWmin */
    uint8_t uc_edca_cwmax;               /* edca����CWmax */
    uint16_t us_edca_txoplimit;          /* edca����TXOP limit */

} mac_phy_debug_switch_stru;

typedef struct {
    uint8_t uc_category;
    uint8_t uc_action_code;
    uint8_t auc_oui[3];
    uint8_t uc_eid;
    uint8_t uc_lenth;
    uint8_t uc_location_type;
    uint8_t auc_mac_server[WLAN_MAC_ADDR_LEN];
    uint8_t auc_mac_client[WLAN_MAC_ADDR_LEN];
    uint8_t auc_payload[200];  // 4
} mac_location_event_stru; // only device

typedef struct {
    uint8_t uc_cmd;
    uint8_t auc_arg[3];
    uint32_t ul_cmd_cnt; /* ����ļ��� */
    uint32_t ul_cmd_len; /* �ܳ��ȣ���ָauc_data��ʵ�ʸ��س��� */
    uint8_t auc_data[4];
} mac_acs_cmd_stru;

typedef mac_acs_cmd_stru mac_init_scan_req_stru;

typedef enum {
    MAC_ACS_RSN_INIT,
    MAC_ACS_RSN_LONG_TX_BUF,
    MAC_ACS_RSN_LARGE_PER,
    MAC_ACS_RSN_MWO_DECT,
    MAC_ACS_RSN_RADAR_DECT,

    MAC_ACS_RSN_BUTT
} mac_acs_rsn_enum;
typedef uint8_t mac_acs_rsn_enum_uint8;

typedef enum {
    MAC_ACS_SW_NONE = 0x0,
    MAC_ACS_SW_INIT = 0x1,
    MAC_ACS_SW_DYNA = 0x2,
    MAC_ACS_SW_BOTH = 0x3,

    MAC_ACS_SW_BUTT
} en_mac_acs_sw_enum;
typedef uint8_t en_mac_acs_sw_enum_uint8;

typedef enum {
    MAC_ACS_SET_CH_DNYA = 0x0,
    MAC_ACS_SET_CH_INIT = 0x1,

    MAC_ACS_SET_CH_BUTT
} en_mac_acs_set_ch_enum;
typedef uint8_t en_mac_acs_set_ch_enum_uint8;

typedef struct {
    oal_bool_enum_uint8 en_sw_when_connected_enable : 1;
    oal_bool_enum_uint8 en_drop_dfs_channel_enable : 1;
    oal_bool_enum_uint8 en_lte_coex_enable : 1;
    en_mac_acs_sw_enum_uint8 en_acs_switch : 5;
} mac_acs_switch_stru;

/* DMAC SCAN �ŵ�ɨ��BSS��ϢժҪ�ṹ */
typedef struct {
    int8_t c_rssi;             /* bss���ź�ǿ�� */
    uint8_t uc_channel_number; /* �ŵ��� */
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];

    /* 11n, 11ac��Ϣ */
    oal_bool_enum_uint8 en_ht_capable;                      /* �Ƿ�֧��ht */
    oal_bool_enum_uint8 en_vht_capable;                     /* �Ƿ�֧��vht */
    wlan_bw_cap_enum_uint8 en_bw_cap;                       /* ֧�ֵĴ��� 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8 en_channel_bandwidth; /* �ŵ��������� */
} mac_scan_bss_stats_stru;

typedef struct {
    int8_t c_rssi;             /* bss���ź�ǿ�� */
    uint8_t uc_channel_number; /* �ŵ��� */

    oal_bool_enum_uint8 en_ht_capable : 1;                      /* �Ƿ�֧��ht */
    oal_bool_enum_uint8 en_vht_capable : 1;                     /* �Ƿ�֧��vht */
    wlan_bw_cap_enum_uint8 en_bw_cap : 3;                       /* ֧�ֵĴ��� 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8 en_channel_bandwidth : 3; /* �ŵ��������� */
} mac_scan_bss_stats_simple_stru;


/* DMAC SCAN �ص��¼��ṹ�� */
typedef struct {
    uint8_t uc_nchans;    /* �ŵ�����       */
    uint8_t uc_nbss;      /* BSS���� */
    uint8_t uc_scan_func; /* ɨ�������Ĺ��� */

    uint8_t uc_need_rank : 1;  // kernel write, app read
    uint8_t uc_obss_on : 1;
    uint8_t uc_dfs_on : 1;
    uint8_t uc_dbac_on : 1;
    uint8_t uc_chip_id : 2;
    uint8_t uc_device_id : 2;

    uint8_t uc_acs_type; /* 0:��ʼacs,1:�����acs,2:����ʱacs */
    wlan_channel_band_enum_uint8 en_band;
    uint8_t uc_pre_channel;
    wlan_channel_bandwidth_enum_uint8 en_pre_bw;
    uint32_t ul_time_stamp;
    oal_bool_enum_uint8 en_switch_chan;
    uint8_t auc_resv[3];
} mac_scan_event_stru;

#ifdef _PRE_WLAN_FEATURE_DFS
typedef struct {
    oal_bool_enum_uint8 en_dfs_switch; /* DFSʹ�ܿ��� bit0:dfsʹ��,bit1:��ʾAP��ΪDFS������ʱ������wait_start */
    oal_bool_enum_uint8 en_cac_switch;
    oal_bool_enum_uint8 en_offchan_cac_switch;
    uint8_t uc_debug_level;  /* bit0:��ӡ�״��ҵ��bit1:��ӡ�״���ҵ�� */
    uint8_t uc_offchan_flag; /* bit0:0��ʾhomechan,1��ʾoffchan; bit1:0��ʾ��ͨ,1��ʾoffchancac */
    uint8_t uc_offchan_num;
    uint8_t uc_timer_cnt;
    uint8_t uc_timer_end_cnt;
    uint8_t uc_cts_duration;
    uint8_t uc_dmac_channel_flag; /* dmac���ڱ�ʾ��ǰ�ŵ�off or home */
    oal_bool_enum_uint8 en_dfs_init;
    uint8_t uc_custom_next_chnum;                   /* Ӧ�ò�ָ����DFS��һ���ŵ��� */
    uint32_t ul_dfs_cac_outof_5600_to_5650_time_ms; /* CAC���ʱ����5600MHz ~ 5650MHzƵ���⣬Ĭ��60�� */
    uint32_t ul_dfs_cac_in_5600_to_5650_time_ms;    /* CAC���ʱ����5600MHz ~ 5650MHzƵ���ڣ�Ĭ��10���� */
    /* Off-Channel CAC���ʱ����5600MHz ~ 5650MHzƵ���⣬Ĭ��6���� */
    uint32_t ul_off_chan_cac_outof_5600_to_5650_time_ms;
    /* Off-Channel CAC���ʱ����5600MHz ~ 5650MHzƵ���ڣ�Ĭ��60���� */
    uint32_t ul_off_chan_cac_in_5600_to_5650_time_ms;
    uint16_t us_dfs_off_chan_cac_opern_chan_dwell_time; /* Off-channel CAC�ڹ����ŵ���פ��ʱ�� */
    uint16_t us_dfs_off_chan_cac_off_chan_dwell_time;   /* Off-channel CAC��Off-Channel�ŵ���פ��ʱ�� */
    uint32_t ul_radar_detected_timestamp;
    int32_t l_radar_th;                  // ���õ��״������ޣ���λdb
    uint32_t ul_custom_chanlist_bitmap;  // Ӧ�ò�ͬ���������ŵ��б�
    // (0x0000000F) /* 36--48 */
    // (0x000000F0) /* 52--64 */
    // (0x000FFF00) /* 100--144 */
    // (0x01F00000) /* 149--165 */
    wlan_channel_bandwidth_enum_uint8 en_next_ch_width_type;  // ���õ���һ���ŵ��Ĵ���ģʽ
    uint8_t uac_resv[3];
    uint32_t ul_dfs_non_occupancy_period_time_ms;
    uint8_t uc_radar_type;
    uint8_t uc_band;
    uint8_t uc_channel_num;
    uint8_t uc_flag;
} mac_dfs_info_stru;

typedef struct {
    uint8_t uc_chan_idx;  /* �ŵ����� */
    uint8_t uc_device_id; /* device id */
    uint8_t auc_resv[2];
    frw_timeout_stru st_dfs_nol_timer; /* NOL�ڵ㶨ʱ�� */
    oal_dlist_head_stru st_entry;      /* NOL���� */
} mac_dfs_nol_node_stru;

typedef struct {
    frw_timeout_stru st_dfs_cac_timer;          /* CAC��ʱ�� */
    frw_timeout_stru st_dfs_off_chan_cac_timer; /* Off-Channel CAC��ʱ�� */
    frw_timeout_stru st_dfs_chan_dwell_timer;   /* �ŵ�פ����ʱ������ʱ�����ڣ�������ŵ� */
    frw_timeout_stru st_dfs_radar_timer;
    mac_dfs_info_stru st_dfs_info;
    oal_dlist_head_stru st_dfs_nol;
} mac_dfs_core_stru;
#endif

/* ɨ�赽��BSS�����ṹ�� */
typedef struct {
    /* ������Ϣ */
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type; /* bss�������� */
    uint8_t uc_dtim_period;                        /* dtime���� */
    uint8_t uc_dtim_cnt;                           /* dtime cnt */
    oal_bool_enum_uint8 en_11ntxbf;                  /* 11n txbf */
    oal_bool_enum_uint8 en_new_scan_bss;             /* �Ƿ�����ɨ�赽��BSS */
    wlan_ap_chip_oui_enum_uint8 en_is_tplink_oui;
    int8_t c_rssi;                                    /* bss���ź�ǿ�� */
    int8_t ac_ssid[WLAN_SSID_MAX_LEN];                /* ����ssid */
    uint16_t us_beacon_period;                        /* beacon���� */
    uint16_t us_cap_info;                             /* ����������Ϣ */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];          /* ���������� mac��ַ��bssid��ͬ */
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];             /* ����bssid */
    mac_channel_stru st_channel;                        /* bss���ڵ��ŵ� */
    uint8_t uc_wmm_cap;                               /* �Ƿ�֧��wmm */
    uint8_t uc_uapsd_cap;                             /* �Ƿ�֧��uapsd */
    oal_bool_enum_uint8 en_desired;                     /* ��־λ����bss�Ƿ��������� */
    uint8_t uc_num_supp_rates;                        /* ֧�ֵ����ʼ����� */
    uint8_t auc_supp_rates[WLAN_USER_MAX_SUPP_RATES]; /* ֧�ֵ����ʼ� */

#ifdef _PRE_WLAN_FEATURE_11D
    int8_t ac_country[WLAN_COUNTRY_STR_LEN]; /* �����ַ��� */
    uint8_t auc_resv2[1];
    uint8_t *puc_country_ie; /* ���ڴ��country ie */
#endif

    /* ��ȫ��ص���Ϣ */
    uint8_t *puc_rsn_ie; /* ���ڴ��beacon��probe rsp��rsn ie */
    uint8_t *puc_wpa_ie; /* ���ڴ��beacon��probe rsp��wpa ie */

    /* 11n 11ac��Ϣ */
    oal_bool_enum_uint8 en_ht_capable;                      /* �Ƿ�֧��ht */
    oal_bool_enum_uint8 en_vht_capable;                     /* �Ƿ�֧��vht */
    oal_bool_enum_uint8 en_vendor_vht_capable;              /* �Ƿ�֧��hidden vendor vht */
    wlan_bw_cap_enum_uint8 en_bw_cap;                       /* ֧�ֵĴ��� 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8 en_channel_bandwidth; /* �ŵ����� */
    uint8_t uc_coex_mgmt_supp;                            /* �Ƿ�֧�ֹ������ */
    oal_bool_enum_uint8 en_ht_ldpc;                         /* �Ƿ�֧��ldpc */
    oal_bool_enum_uint8 en_ht_stbc;                         /* �Ƿ�֧��stbc */
    uint8_t uc_wapi;
    uint8_t en_vendor_novht_capable; /* ˽��vendor�в�����Я�� */
    oal_bool_enum_uint8 en_atheros_chip_oui;
    oal_bool_enum_uint8 en_vendor_1024qam_capable;          /* ˽��vendor���Ƿ�Я��1024QAMʹ�� */

    mac_btcoex_blacklist_enum_uint8 en_btcoex_blacklist_chip_oui; /* ps����one pkt֡������Ҫ�޶�Ϊself-cts�� */
    oal_bool_enum_uint8 en_punctured_preamble; /* �Ƿ�֧��punctured preamble���� */
    uint32_t ul_timestamp; /* ���´�bss��ʱ��� */

#ifdef _PRE_WLAN_FEATURE_M2S
    wlan_nss_enum_uint8 en_support_max_nss; /* ��AP֧�ֵ����ռ����� */
    oal_bool_enum_uint8 en_support_opmode;  /* ��AP�Ƿ�֧��OPMODE */
    uint8_t uc_num_sounding_dim;          /* ��AP����txbf�������� */
#endif

    oal_bool_enum_uint8 en_he_capable; /* �Ƿ�֧��11ax */
    oal_bool_enum_uint8 en_he_uora;
    oal_bool_enum_uint8 en_dcm_support;
    mac_p2p_scenes_enum_uint8 en_p2p_scenes;

#if defined(_PRE_WLAN_FEATURE_11K) ||  \
    defined(_PRE_WLAN_FEATURE_FTM)
    oal_bool_enum_uint8 en_support_rrm; /* �Ƿ�֧��RRM */
    oal_bool_enum_uint8 en_support_neighbor; /* �Ƿ�֧��Neighbor report */
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM
    oal_bool_enum_uint8 en_support_1024qam;
#endif

#ifdef _PRE_WLAN_NARROW_BAND
    oal_bool_enum_uint8 en_nb_capable; /* �Ƿ�֧��nb */
#endif
    oal_bool_enum_uint8 en_roam_blacklist_chip_oui; /* ��֧��roam */
    oal_bool_enum_uint8 en_txbf_blacklist_chip_oui; /* ��֧��txbf */

    int8_t c_ant0_rssi; /* ����0��rssi */
    int8_t c_ant1_rssi; /* ����1��rssi */
    int8_t   ant2_rssi;   /* ����2��rssi */
    int8_t   ant3_rssi;   /* ����3��rssi */
    int8_t   resv[2];     /* ����2λ */

#ifdef _PRE_WLAN_FEATURE_MBO
    uint8_t uc_bss_assoc_disallowed; /* MBO IE��ָʾAP�Ƿ�������� */
#endif
    /* ����֡��Ϣ */
    uint32_t ul_mgmt_len; /* ����֡�ĳ��� */

    /* multi_bssid ��� */
#ifdef _PRE_WLAN_FEATURE_11AX
    mac_multi_bssid_info st_mbssid_info;
#endif
    uint64_t ul_wpa_rpt_time;                                 /* ɨ�����ϱ�supplicantʱ�� */
    uint8_t auc_mgmt_buff[MAC_80211_QOS_HTC_4ADDR_FRAME_LEN]; /* ��¼beacon֡��probe rsp֡ */
    /* ��Ҫ�ڹ���֡��������Ԫ�أ�������ڹ���֡����֮ǰ */
    uint8_t auc_mgmt_frame_body[4]; /* *puc_mgmt_buff --- ��¼beacon֡��probe rsp֡ */
} mac_bss_dscr_stru; // only host

#ifdef _PRE_WLAN_DFT_STAT
/* ����֡ͳ����Ϣ */
typedef struct {
    /* ���չ���֡ͳ�� */
    uint32_t aul_rx_mgmt[WLAN_MGMT_SUBTYPE_BUTT];

    /* �ҵ�Ӳ�����еĹ���֡ͳ�� */
    uint32_t aul_tx_mgmt_soft[WLAN_MGMT_SUBTYPE_BUTT];

    /* ������ɵĹ���֡ͳ�� */
    uint32_t aul_tx_mgmt_hardware[WLAN_MGMT_SUBTYPE_BUTT];
} mac_device_mgmt_statistic_stru; // only device
#endif

#ifdef _PRE_WLAN_DFT_STAT
/* �ϱ��տڻ�����ά������Ŀ��ƽṹ */
typedef struct {
    uint32_t ul_non_directed_frame_num;               /* ���յ��Ǳ���֡����Ŀ */
    uint8_t uc_collect_period_cnt;                    /* �ɼ����ڵĴ���������100����ϱ�������Ȼ���������¿�ʼ */
    oal_bool_enum_uint8 en_non_directed_frame_stat_flg; /* �Ƿ�ͳ�ƷǱ�����ַ֡�����ı�־ */
    int16_t s_ant_power;                              /* ���߿ڹ��� */
    frw_timeout_stru st_collect_period_timer;           /* �ɼ����ڶ�ʱ�� */
} mac_device_dbb_env_param_ctx_stru; // only device
#endif

typedef struct {
    /* ֧��2*2 */                            /* ֧��MU-MIMO */
    wlan_nss_enum_uint8 en_nss_num;          /* device Nss �ռ��������� */
    wlan_bw_cap_enum_uint8 en_channel_width; /* ֧�ֵĴ��� */
    oal_bool_enum_uint8 en_nb_is_supp;       /* ֧��խ�� */
    oal_bool_enum_uint8 en_1024qam_is_supp;  /* ֧��1024QAM���� */

    oal_bool_enum_uint8 en_80211_mc_is_supp; /* ֧��80211 mc */
    oal_bool_enum_uint8 en_ldpc_is_supp;     /* �Ƿ�֧�ֽ���LDPC����İ� */
    oal_bool_enum_uint8 en_tx_stbc_is_supp;  /* �Ƿ�֧������2x1 STBC���� */
    oal_bool_enum_uint8 en_rx_stbc_is_supp;  /* �Ƿ�֧��stbc����,֧��2���ռ��� */

    oal_bool_enum_uint8 en_su_bfmer_is_supp; /* �Ƿ�֧�ֵ��û�beamformer */
    oal_bool_enum_uint8 en_su_bfmee_is_supp; /* �Ƿ�֧�ֵ��û�beamformee */
    oal_bool_enum_uint8 en_mu_bfmer_is_supp; /* �Ƿ�֧�ֶ��û�beamformer */
    oal_bool_enum_uint8 en_mu_bfmee_is_supp; /* �Ƿ�֧�ֶ��û�beamformee */
    uint8_t en_11ax_switch : 1,            /* 11ax���� */
              bit_multi_bssid_switch : 1,          /* mbssid ���� */
              bit_11ax_rsv : 6;
    uint8_t _rom[3];
} mac_device_capability_stru;

typedef struct {
    oal_bool_enum_uint8 en_11k;
    oal_bool_enum_uint8 en_11v;
    oal_bool_enum_uint8 en_11r;
    oal_bool_enum_uint8 en_11r_ds;
    oal_bool_enum_uint8 en_adaptive11r;
    oal_bool_enum_uint8 en_nb_rpt_11k;
    uint8_t auc_rsv[2];
} mac_device_voe_custom_stru;

#ifdef _PRE_WLAN_FEATURE_11AX
typedef struct {
    uint8_t   bit_htc_include : 1, /* ����֡����htc ͷ,Ĭ�ϳ�ʼ��ֵδȫ F */
                bit_om_in_data  : 1, /* ÿ����������֡��Я��om�ֶ� */
                bit_rom_cap_switch : 1,
                bit_11ax_aput_switch  :1,  /* 11ax aput���� */
                bit_ignore_non_he_cap_from_beacon : 1, /* he sta ������beacon �е�non he cap ie ���� */
                bit_11ax_aput_he_cap_switch: 1, /* 11ax aputЯ��he�������� */
                bit_twt_responder_support : 1,
                bit_twt_requester_support : 1;
} mac_device_11ax_custom_stru;
#endif

typedef struct {
    mac_device_voe_custom_stru st_voe_custom_cfg;
#ifdef _PRE_WLAN_FEATURE_11AX
    mac_device_11ax_custom_stru st_11ax_custom_cfg;
#endif
    uint16_t us_cmd_auth_rsp_timeout;
    uint8_t  bit_forbit_open_auth : 1,
               bit_rsv              : 7;
    uint8_t  auc_rsv[1];
} mac_device_custom_cfg_stru;
/* device�ṹ�� */
typedef struct {
    /* device�µ�ҵ��vap���˴�ֻ��¼VAP ID */
    uint32_t ul_core_id;
    uint8_t auc_vap_id[WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE];
    uint8_t uc_cfg_vap_id;               /* ����vap ID */
    uint8_t uc_device_id;                /* оƬID */
    uint8_t uc_chip_id;                  /* �豸ID */
    uint8_t uc_device_reset_in_progress; /* ��λ������ */

    oal_bool_enum_uint8 en_device_state; /* ��ʶ�Ƿ��Ѿ������䣬(OAL_TRUE��ʼ����ɣ�OAL_FALSEδ��ʼ�� ) */
    uint8_t uc_vap_num;                /* ��ǰdevice�µ�ҵ��VAP����(AP+STA) */
    uint8_t uc_sta_num;                /* ��ǰdevice�µ�STA���� */
    /* begin: P2P */
    mac_p2p_info_stru st_p2p_info; /* P2P �����Ϣ */
    /* end: P2P */
    uint8_t auc_hw_addr[WLAN_MAC_ADDR_LEN]; /* ��eeprom��flash��õ�mac��ַ��ko����ʱ����hal�ӿڸ�ֵ */
    /* device������� */
    uint8_t uc_max_channel;                 /* ������VAP���ŵ��ţ�����VAP����ֵ�������ֵì�ܣ����ڷ�DBACʱʹ�� */
    wlan_channel_band_enum_uint8 en_max_band; /* ������VAP��Ƶ�Σ�����VAP����ֵ�������ֵì�ܣ����ڷ�DBACʱʹ�� */

    oal_bool_enum_uint8 en_wmm; /* wmmʹ�ܿ��� */
    wlan_tidno_enum_uint8 en_tid;
    uint8_t en_reset_switch; /* �Ƿ�ʹ�ܸ�λ���� */
    uint8_t uc_csa_vap_cnt;  /* ÿ��running AP����һ��CSA֡,�ü�����1,�������,APֹͣ��ǰӲ������,׼����ʼ�л��ŵ� */

    uint32_t ul_beacon_interval; /* device����beacon interval,device������VAPԼ��Ϊͬһֵ */

    uint8_t uc_auth_req_sendst;
    uint8_t uc_asoc_req_sendst;

#ifdef _PRE_WLAN_FEATURE_HIEX
    mac_hiex_cap_stru st_hiex_cap;
    uint32_t ul_hiex_debug_switch;
#else
    uint32_t ul_resv1;
    uint32_t ul_resv2;
#endif
    uint32_t ul_resv3;

    /* device���� */
    wlan_protocol_cap_enum_uint8 en_protocol_cap; /* Э������ */
    wlan_band_cap_enum_uint8 en_band_cap;         /* Ƶ������ */
    /* ������VAP���������ֵ������VAP����ֵ�������ֵì�ܣ����ڷ�DBACʱʹ�� */
    wlan_channel_bandwidth_enum_uint8 en_max_bandwidth;

    int16_t s_upc_amend; /* UPC����ֵ */

    uint16_t us_device_reset_num; /* ��λ�Ĵ���ͳ�� */

    mac_data_rate_stru st_mac_rates_11g[MAC_DATARATES_PHY_80211G_NUM]; /* 11g���� */

    mac_pno_sched_scan_mgmt_stru *pst_pno_sched_scan_mgmt; /* pno����ɨ�����ṹ��ָ�룬�ڴ涯̬���룬�Ӷ���ʡ�ڴ� */
    mac_scan_req_stru st_scan_params;                      /* ����һ�ε�ɨ�������Ϣ */
    frw_timeout_stru st_obss_scan_timer;                   /* obssɨ�趨ʱ����ѭ����ʱ�� */
    mac_channel_stru st_p2p_vap_channel;                   /* p2p listenʱ��¼p2p���ŵ�������p2p listen������ָ� */

    oal_bool_enum_uint8 en_2040bss_switch; /* 20/40 bss��⿪�� */
    uint8_t uc_in_suspend;

    /* linux�ں��е�device������Ϣ */
    /* ���ڴ�ź�VAP��ص�wiphy�豸��Ϣ����AP/STAģʽ�¾�Ҫʹ�ã����Զ��VAP��Ӧһ��wiphy */
    oal_wiphy_stru *pst_wiphy;
    mac_bss_id_list_stru st_bss_id_list; /* ��ǰ�ŵ��µ�ɨ���� */

    uint8_t uc_mac_vap_id; /* ��vap����ʱ������˯��ǰ��mac vap id */
    oal_bool_enum_uint8 en_dbac_enabled;
    oal_bool_enum_uint8 en_dbac_running;       /* DBAC�Ƿ������� */
    oal_bool_enum_uint8 en_dbac_has_vip_frame; /* ���DBAC����ʱ�յ��˹ؼ�֡ */

    uint8_t uc_arpoffload_switch;
    uint8_t uc_wapi;
#ifdef _PRE_WLAN_FEATURE_MONITOR
    uint8_t uc_monitor_ota_mode;
#else
    uint8_t uc_reserve;
#endif
    oal_bool_enum_uint8 en_is_random_mac_addr_scan; /* ���macɨ�迪��,��hmac�·� */

    uint8_t auc_mac_oui[WLAN_RANDOM_MAC_OUI_LEN]; /* ���mac��ַOUI,��ϵͳ�·� */
    oal_bool_enum_uint8 en_dbdc_running;            /* DBDC�Ƿ������� */

    mac_device_capability_stru st_device_cap; /* device�Ĳ����������������ƻ� */

#ifdef _PRE_WLAN_FEATURE_SMPS
    /* SMPS��MAC����������Ҫdevice�����е�VAP��֧��SMPS�ŻῪ��MAC��SMPS���� */
    /* ��¼��ǰMAC������SMPS����(����mac�࣬����ΪhmacҲ���������ж�) */
    wlan_mib_mimo_power_save_enum_uint8 en_mac_smps_mode;
#endif

#ifdef _PRE_WLAN_FEATURE_DFS
    mac_dfs_core_stru st_dfs;
#endif
    mac_ap_ch_info_stru st_ap_channel_list[MAC_MAX_SUPP_CHANNEL];
    /* ���Device�ĳ�Ա�����ƶ���dmac_device */
#if IS_DEVICE
    uint16_t us_total_mpdu_num;                                /* device������TID���ܹ���mpdu_num���� */
    uint16_t aus_ac_mpdu_num[WLAN_WME_AC_BUTT];                /* device�¸���AC��Ӧ��mpdu_num�� */
    uint16_t aus_vap_mpdu_num[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT]; /* ͳ�Ƹ���vap��Ӧ��mpdu_num�� */

    uint16_t us_dfs_timeout;
    uint32_t ul_first_timestamp; /* ��¼����ͳ�Ƶ�һ��ʱ��� */

    /* ɨ����س�Ա���� */
    uint32_t ul_scan_timestamp; /* ��¼����һ��ɨ�迪ʼ��ʱ�� */
    /* ��ǰɨ��״̬�����ݴ�״̬����obssɨ���host���·���ɨ�������Լ�ɨ�������ϱ����� */
    mac_scan_state_enum_uint8 en_curr_scan_state;
    uint8_t uc_resume_qempty_flag; /* ʹ�ָܻ�qempty��ʶ, Ĭ�ϲ�ʹ�� */
    uint8_t uc_scan_count;
    uint8_t uc_dc_status;
#ifdef _PRE_WLAN_DFT_STAT
    /* ����֡ͳ����Ϣ */
    mac_device_mgmt_statistic_stru st_mgmt_stat;
    mac_device_dbb_env_param_ctx_stru st_dbb_env_param_ctx; /* �ϱ��տڻ�����ά������Ŀ��ƽṹ */
#endif
    uint8_t uc_csa_cnt;               /* ÿ��AP����һ��CSA֡���ü�����1��AP�л����ŵ��󣬸ü������� */
    oal_bool_enum_uint8 en_txop_enable; /* �����޾ۺ�ʱ����TXOPģʽ */
    uint8_t uc_tx_ba_num;             /* ���ͷ���BA�Ự���� */
    uint8_t auc_resv[1];

    frw_timeout_stru st_keepalive_timer; /* keepalive��ʱ�� */

#endif /* IS_DEVICE */

    /* ���Host�ĳ�Ա�����ƶ���hmac_device */
#if IS_HOST
    oal_bool_enum_uint8 en_vap_classify; /* �Ƿ�ʹ�ܻ���vap��ҵ����� */
    uint8_t uc_ap_chan_idx;            /* ��ǰɨ���ŵ����� */
    uint8_t auc_resv4[2];
#endif /* IS_HOST */

    oal_bool_enum_uint8 en_40MHz_intol_bit_recd;
    uint8_t uc_ftm_vap_id; /* ftm�ж϶�Ӧ vap ID */
    uint8_t auc_resv5[2];

    frw_timeout_stru st_send_frame; /* send frame��ʱ�� */

    uint8_t is_ready_to_get_scan_result;
    mac_channel_stru st_best_chan_for_hid2d;
    /* �����᳡����־λ: 0��ʾ�Ƿ�����ģʽ��1��ʾ������ģʽ */
    uint8_t is_presentation_mode;
    /* ˫staģʽ��ǣ�wlan1��Ӧvap����ʱ��true��ɾ��ʱ��false */
    uint8_t is_dual_sta_mode;
} mac_device_stru;
#ifdef _PRE_WLAN_FEATURE_HIEX
#define MAC_BAND_GET_HIEX_CAP(band)     (&(band)->st_hiex_cap)
#define MAC_BAND_HIEX_ENABLED(band)     ((band)->st_hiex_cap.bit_hiex_enable)
#define MAC_BAND_HIMIT_ENABLED(band)    ((band)->st_hiex_cap.bit_himit_enable)
#define MAC_BAND_ERSRU_ENABLED(band)    ((band)->st_hiex_cap.bit_ersru_enable)
#endif

typedef struct {
    uint8_t auc_tx_ba_index_table[MAC_TX_BA_LUT_BMAP_LEN];   /* ���Ͷ�BA LUT��λͼ */
    uint8_t auc_rx_ba_lut_idx_table[MAC_RX_BA_LUT_BMAP_LEN]; /* ���ն�BA LUT��λͼ */
    uint8_t auc_ra_lut_index_table[WLAN_ACTIVE_USER_IDX_BMAP_LEN]; /* �����û� LUT��λͼ */
} mac_lut_table_stru;

/* chip�ṹ�� */
typedef struct {
    uint8_t auc_device_id[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP]; /* CHIP�¹ҵ�DEV������¼��Ӧ��ID����ֵ */
    uint8_t uc_device_nums;                                      /* chip��device����Ŀ */
    uint8_t uc_chip_id;                                          /* оƬID */
    /* ��ʶ�Ƿ��ѳ�ʼ����OAL_TRUE��ʼ����ɣ�OAL_FALSEδ��ʼ�� */
    oal_bool_enum_uint8 en_chip_state;
    uint32_t ul_chip_ver;              /* оƬ�汾 */
    mac_lut_table_stru st_lut_table;     /* ���ά��LUT����Դ�Ľṹ�� */
    void *p_alg_priv;                /* chip�����㷨˽�нṹ�� */

    /* �û���س�Ա���� */
    frw_timeout_stru st_active_user_timer; /* �û���Ծ��ʱ�� */
    uint8_t uc_assoc_user_cnt;  /* �����û��� */
    uint8_t uc_active_user_cnt; /* ��Ծ�û��� */
} mac_chip_stru;

typedef enum {
    MAC_RX_IP_FILTER_STOPED = 0,   // ���ܹرգ�δʹ�ܡ���������״����������˶�����
    MAC_RX_IP_FILTER_WORKING = 1,  // ���ܴ򿪣����չ�����������
    MAC_RX_IP_FILTER_BUTT
} mac_ip_filter_state_enum;
typedef uint8_t mac_ip_filter_state_enum_uint8;

typedef struct {
    mac_ip_filter_state_enum_uint8 en_state;  // ����״̬�����ˡ��ǹ��˵�
    uint8_t uc_btable_items_num;            // ��������Ŀǰ�洢��items����
    uint8_t uc_btable_size;                 // ��������С����ʾ���洢��items����
    uint8_t uc_resv;
    mac_ip_filter_item_stru *pst_filter_btable;  // ������ָ��
} mac_rx_ip_filter_struc;

/* board�ṹ�� */
typedef struct {
    mac_chip_stru ast_chip[WLAN_CHIP_MAX_NUM_PER_BOARD]; /* board�ҽӵ�оƬ */
    uint8_t uc_chip_id_bitmap;                         /* ��ʶchip�Ƿ񱻷����λͼ */
    uint8_t auc_resv[3];                               /* �ֽڶ��� */
    mac_rx_ip_filter_struc st_rx_ip_filter; /* rx ip���˹��ܵĹ���ṹ�� */
    mac_rx_icmp_filter_struc st_rx_icmp_filter;
} mac_board_stru;

typedef struct {
    mac_device_stru *pst_mac_device;
} mac_wiphy_priv_stru;

/* ������ */
typedef struct {
    uint8_t auc_mac_addr[OAL_MAC_ADDR_LEN]; /* mac��ַ          */
    uint8_t auc_reserved[2];                /* �ֽڶ���         */
    uint32_t ul_cfg_time;                   /* �����������ʱ�� */
    uint32_t ul_aging_time;                 /* �ϻ�ʱ��         */
    uint32_t ul_drop_counter;               /* ���Ķ���ͳ��     */
} mac_blacklist_stru;

/* �Զ������� */
typedef struct {
    uint8_t auc_mac_addr[OAL_MAC_ADDR_LEN]; /* mac��ַ  */
    uint8_t auc_reserved[2];                /* �ֽڶ��� */
    uint32_t ul_cfg_time;                   /* ��ʼʱ�� */
    uint32_t ul_asso_counter;               /* �������� */
} mac_autoblacklist_stru;

/* 2.9.12 ˽�а�ȫ��ǿ */
#define WLAN_BLACKLIST_MAX 32

/* �Զ���������Ϣ */
typedef struct {
    uint8_t uc_enabled;                                          /* ʹ�ܱ�־ 0:δʹ��  1:ʹ�� */
    uint8_t list_num;                                            /* �ж��ٸ��Զ�������        */
    uint8_t auc_reserved[2];                                     /* �ֽڶ���                  */
    uint32_t ul_threshold;                                       /* ����                      */
    uint32_t ul_reset_time;                                      /* ����ʱ��                  */
    uint32_t ul_aging_time;                                      /* �ϻ�ʱ��                  */
    mac_autoblacklist_stru ast_autoblack_list[WLAN_BLACKLIST_MAX]; /* �Զ���������              */
} mac_autoblacklist_info_stru;
#if 1
/* �ڰ�������Ϣ */
typedef struct {
    uint8_t uc_mode;                                     /* �ڰ�����ģʽ   */
    uint8_t uc_list_num;                                 /* ������         */
    uint8_t uc_blacklist_vap_index;                      /* ������vap index */
    uint8_t uc_blacklist_device_index;                   /* ������device index */
    mac_autoblacklist_info_stru st_autoblacklist_info;     /* �Զ���������Ϣ */
    mac_blacklist_stru ast_black_list[WLAN_BLACKLIST_MAX]; /* ��Ч�ڰ������� */
} mac_blacklist_info_stru;
#endif

typedef struct {
    uint32_t                          ul_ao_drop_cnt;
    uint32_t                          ul_ao_send_rsp_cnt;
    uint32_t                          ul_apf_flt_drop_cnt;
    uint32_t                          ul_icmp_flt_drop_cnt;
}mac_psm_flt_stat_stru; // only device
typedef struct {
    uint32_t                          ul_ps_short_idle_cnt;
    uint32_t                          ul_ps_long_idle_cnt;
}mac_psm_fastsleep_stat_stru; // only device

#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
typedef struct {
    uint32_t ul_small_amsdu_total_cnt; // С��amsdu����
    uint32_t ul_large_amsdu_total_cnt; // ���amsdu����
    uint32_t ul_small_amsdu_mcast_ucast_cnt; // С��amsdu�鲥����������
} mac_psm_abnormal_stat_stru;
#endif

typedef struct {
    uint16_t us_wifi_center_freq;  /* WiFi����Ƶ�� */
    uint16_t us_modem_center_freq; /* Modem����Ƶ�� */
    uint8_t uc_wifi_bw;            /* WiFi���� */
    uint8_t uc_modem_bw;           /* Modem���� */
    uint8_t uc_wifi_avoid_flag;    /* WiFi��ǰ����ֶ� */
    uint8_t uc_wifi_priority;      /* WiFi���ȼ� */
} mac_nrcoex_stat_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* ���߼��в��뿴���� */
#define MAC_DBAC_ENABLE(_pst_device) (_pst_device->en_dbac_enabled == OAL_TRUE)

extern mac_device_capability_stru *g_pst_mac_device_capability;

#ifdef _PRE_WLAN_FEATURE_WMMAC
extern oal_bool_enum_uint8 g_en_wmmac_switch;
#endif

extern mac_board_stru *g_pst_mac_board;


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_dbac_enabled(mac_device_stru *pst_device)
{
    return pst_device->en_dbac_enabled;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_dbac_running(mac_device_stru *pst_device)
{
    if (pst_device->en_dbac_enabled == OAL_FALSE) {
        return OAL_FALSE;
    }

    return pst_device->en_dbac_running;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_dbdc_running(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->en_dbdc_running;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_get_2040bss_switch(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->en_2040bss_switch;
}
OAL_STATIC OAL_INLINE void mac_set_2040bss_switch(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_switch)
{
    pst_mac_device->en_2040bss_switch = en_switch;
}
#if IS_DEVICE
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_device_is_scaning(mac_device_stru *pst_mac_device)
{
    return (pst_mac_device->en_curr_scan_state == MAC_SCAN_STATE_RUNNING) ? OAL_TRUE : OAL_FALSE;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_device_is_listening(mac_device_stru *pst_mac_device)
{
    return ((pst_mac_device->en_curr_scan_state == MAC_SCAN_STATE_RUNNING)
            && (pst_mac_device->st_scan_params.uc_scan_func & MAC_SCAN_FUNC_P2P_LISTEN)) ? OAL_TRUE : OAL_FALSE;
}
#endif /* IS_DEVICE */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_chip_run_band(uint8_t uc_chip_id, wlan_channel_band_enum_uint8 en_band)
{
    /*
     * �ж�ָ��оƬ�Ƿ����������ָ��BAND��
     *     -˫оƬʱ��оƬֻ������ָ����BAND����������˫оƬ��Ƶ���޸Ĵ˴�
     *     -��оƬ˫Ƶʱ��������������BAND
     *     -��оƬ��Ƶʱֻ���������ں궨��ָ����BAND
     *     -note:Ŀǰ����witp wifiоƬ��֧��˫Ƶ���������е�ƵоƬ����Ҫ��������
     *      plat_chip_supp_band(chip_id, band)�Ľӿڣ����ڴ˴������ж�
     */
    if ((en_band != WLAN_BAND_2G) && (en_band != WLAN_BAND_5G)) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

uint32_t mac_device_init(mac_device_stru *pst_mac_device, uint32_t ul_chip_ver,
                                      uint8_t chip_id, uint8_t uc_device_id);
uint32_t mac_chip_init(mac_chip_stru *pst_chip, uint8_t uc_device_max);
uint32_t mac_board_init(void);

uint32_t mac_device_exit(mac_device_stru *pst_device);
uint32_t mac_chip_exit(mac_board_stru *pst_board, mac_chip_stru *pst_chip);
uint32_t mac_board_exit(mac_board_stru *pst_board);

/* 10.2 ������Ա���ʲ��� */
void mac_chip_inc_assoc_user(mac_chip_stru *pst_mac_chip);
void mac_chip_dec_assoc_user(mac_chip_stru *pst_mac_chip);

#ifdef _PRE_WLAN_FEATURE_11AX
uint8_t mac_device_trans_bandwith_to_he_capinfo(wlan_bw_cap_enum_uint8 en_max_op_bd);
#endif

void mac_device_set_dfr_reset(mac_device_stru *pst_mac_device, uint8_t uc_device_reset_in_progress);
void mac_device_set_state(mac_device_stru *pst_mac_device, uint8_t en_device_state);

void mac_device_set_beacon_interval(mac_device_stru *pst_mac_device, uint32_t ul_beacon_interval);

void mac_blacklist_get_pointer(wlan_vap_mode_enum_uint8 en_vap_mode, uint8_t uc_dev_id,
                                          uint8_t uc_chip_id, uint8_t uc_vap_id,
                                          mac_blacklist_info_stru **pst_blacklist_info);

void *mac_device_get_all_rates(mac_device_stru *pst_dev);
uint32_t mac_device_check_5g_enable(uint8_t uc_device_id);

extern uint32_t g_ul_ddr_freq;
extern mac_device_custom_cfg_stru g_st_mac_device_custom_cfg;
extern uint8_t g_optimized_feature_switch_bitmap;


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_device_check_5g_enable_per_chip(void)
{
    uint8_t uc_dev_idx = 0;

    while (uc_dev_idx < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP) {
        if (mac_device_check_5g_enable(uc_dev_idx)) {
            return OAL_TRUE;
        }
        uc_dev_idx++;
    }

    return OAL_FALSE;
}

wlan_mib_vht_supp_width_enum_uint8 mac_device_trans_bandwith_to_vht_capinfo(wlan_bw_cap_enum_uint8 en_max_op_bd);
void mac_set_dual_sta_mode(uint8_t mode);
uint8_t mac_is_dual_sta_mode(void);
#endif /* end of mac_device.h */

