

#ifndef __HMAC_VAP_H__
#define __HMAC_VAP_H__

#ifdef __cplusplus // windows ut���벻����������������
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "mac_vap.h"
#include "hmac_ext_if.h"
#include "hmac_user.h"
#include "hmac_main.h"
#include "mac_resource.h"
#ifdef _PRE_WLAN_TCP_OPT
#include "hmac_tcp_opt_struc.h"
#include "oal_hcc_host_if.h"
#endif
#include "hmac_btcoex.h"
#include "hmac_arp_probe.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VAP_H

/* 2 �궨�� */
#ifdef _PRE_WLAN_DFT_STAT
#define HMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt) ((_member) += (_cnt))
#else
#define HMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt)
#endif
#define HMAC_VAP_STATS_PKT_INCR(_member, _cnt) ((_member) += (_cnt))

#ifdef _PRE_WLAN_FEATURE_HS20
#define MAX_QOS_UP_RANGE 8
#define MAX_DSCP_EXCEPT  21 /* maximum of DSCP Exception fields for QoS Map set */
#endif

#define HMAC_MAX_DSCP_VALUE_NUM 64
#define HMAC_DSCP_VALUE_INVALID 0xA5

#ifdef _PRE_WLAN_FEATURE_FTM
#define HMAC_GAS_DIALOG_TOKEN_INITIAL_VALUE 0x80        /* hmac gas��dialog token num */
#endif

extern uint8_t g_uc_host_rx_ampdu_amsdu;
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    oal_dlist_head_stru st_timeout_head;
} hmac_mgmt_timeout_stru;

typedef struct {
    uint16_t us_user_index;
    mac_vap_state_enum_uint8 en_state;
    uint8_t uc_vap_id;
    mac_status_code_enum_uint16 en_status_code;
    uint8_t auc_rsv[2];
} hmac_mgmt_timeout_param_stru;

#ifdef _PRE_WLAN_FEATURE_HS20
typedef struct {
    uint8_t auc_up_low[MAX_QOS_UP_RANGE]; /* User Priority */
    uint8_t auc_up_high[MAX_QOS_UP_RANGE];
    uint8_t auc_dscp_exception_up[MAX_DSCP_EXCEPT]; /* User Priority of DSCP Exception field */
    uint8_t uc_valid;
    uint8_t uc_num_dscp_except;
    uint8_t auc_dscp_exception[MAX_DSCP_EXCEPT]; /* DSCP exception field  */
} hmac_cfg_qos_map_param_stru;
#endif

/* �޸Ĵ˽ṹ����Ҫͬ��֪ͨSDT�������ϱ��޷����� */
typedef struct {
    /***************************************************************************
                                ���Ͱ�ͳ��
    ***************************************************************************/
    /* ����lan�����ݰ�ͳ�� */
    uint32_t ul_rx_pkt_to_lan;   /* �������̷�����̫�������ݰ���Ŀ��MSDU */
    uint32_t ul_rx_bytes_to_lan; /* �������̷�����̫�����ֽ��� */

    /***************************************************************************
                                ���Ͱ�ͳ��
    ***************************************************************************/
    /* ��lan���յ������ݰ�ͳ�� */
    uint32_t ul_tx_pkt_num_from_lan; /* ��lan�����İ���Ŀ,MSDU */
    uint32_t ul_tx_bytes_from_lan;   /* ��lan�������ֽ��� */
} hmac_vap_query_stats_stru;
/* װ������ */
typedef struct {
    uint32_t ul_rx_pkct_succ_num;    /* �������ݰ��� */
    uint32_t ul_dbb_num;             /* DBB�汾�� */
    uint32_t ul_check_fem_pa_status; /* fem��pa�Ƿ��ջٱ�־ */
    int16_t s_rx_rssi;
    oal_bool_enum_uint8 uc_get_dbb_completed_flag; /* ��ȡDBB�汾�ųɹ��ϱ���־ */
    oal_bool_enum_uint8 uc_check_fem_pa_flag;      /* fem��pa�Ƿ��ջ��ϱ���־ */
    oal_bool_enum_uint8 uc_get_rx_pkct_flag;       /* �������ݰ��ϱ���־λ */
    oal_bool_enum_uint8 uc_lte_gpio_check_flag;    /* �������ݰ��ϱ���־λ */
    oal_bool_enum_uint8 uc_report_efuse_reg_flag;  /* efuse �Ĵ�����ȡ */
    oal_bool_enum_uint8 uc_report_reg_flag;        /* �����Ĵ�����ȡ */
    uint32_t ul_reg_value;
    uint8_t uc_ant_status : 4,
              uc_get_ant_flag : 4;
} hmac_atcmdsrv_get_stats_stru;

typedef struct {
    oal_dlist_head_stru st_entry;
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t uc_reserved[2];
    uint8_t auc_pmkid[WLAN_PMKID_LEN];
} hmac_pmksa_cache_stru;

typedef enum _hmac_tcp_opt_queue_ {
    HMAC_TCP_ACK_QUEUE = 0,
    HMAC_TCP_OPT_QUEUE_BUTT
} hmac_tcp_opt_queue;

#ifdef _PRE_WLAN_TCP_OPT
struct hmac_vap_tag;
typedef uint16_t (*hmac_trans_cb_func)(struct hmac_vap_tag *hmac_vap, hmac_tcp_opt_queue type, hcc_chan_type dir, oal_netbuf_head_stru *data);
/* tcp_ack�Ż� */
typedef struct {
    struct wlan_perform_tcp hmac_tcp_ack;
    struct wlan_perform_tcp_list hmac_tcp_ack_list;
    wlan_perform_tcp_impls filter_info;
    hmac_trans_cb_func filter[HMAC_TCP_OPT_QUEUE_BUTT];  // ���˴����Ӻ���
    uint64_t all_ack_count[HMAC_TCP_OPT_QUEUE_BUTT];   // ������TCP ACKͳ��
    uint64_t drop_count[HMAC_TCP_OPT_QUEUE_BUTT];      // ������TCP ACKͳ��
    oal_netbuf_head_stru data_queue[HMAC_TCP_OPT_QUEUE_BUTT];
    oal_spin_lock_stru data_queue_lock[HMAC_TCP_OPT_QUEUE_BUTT];
} hmac_tcp_ack_stru;
#endif

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
#define WDS_HASH_NUM   (1 << 2)
#define WDS_AGING_TIME (300 * OAL_TIME_HZ)

typedef enum {
    WDS_MODE_NONE = 0,
    WDS_MODE_ROOTAP,
    WDS_MODE_REPEATER_STA,
    WDS_MODE_BUTT
} hmac_vap_wds_vap_mode_enum;
typedef uint8_t hmac_vap_wds_vap_mode_enum_uint8;

typedef struct {
    oal_rwlock_stru st_lock;
    frw_timeout_stru st_wds_timer;
    oal_dlist_head_stru st_peer_node[WDS_HASH_NUM];
    oal_dlist_head_stru st_wds_stas[WDS_HASH_NUM];
    oal_dlist_head_stru st_neigh[WDS_HASH_NUM];
    uint32_t ul_wds_aging;
    hmac_vap_wds_vap_mode_enum_uint8 en_wds_vap_mode;
    uint8_t uc_wds_node_num;
    uint16_t uc_wds_stas_num;
    uint16_t uc_neigh_num;
    uint8_t auc_resv[2];
} hmac_vap_wds_stru;
#endif

/* ��ѯ�¼�IDö�� */
typedef enum {
    QUERY_ID_KO_VERSION = 0,
    QUERY_ID_PWR_REF = 1,
    QUERY_ID_BCAST_RATE = 2,
    QUERY_ID_STA_INFO = 3,
    QUERY_ID_ANT_RSSI = 4,
    QUERY_ID_STA_DIAG_INFO = 5,
    QUERY_ID_VAP_DIAG_INFO = 6,
    QUERY_ID_SENSING_BSSID_INFO = 7,
    QUERY_ID_BG_NOISE = 8,
#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    QUERY_ID_TCP_ACK_BUF = 10,
#endif
    QUERY_ID_MODE_BUTT
} hmac_vap_query_enent_id_enum;
typedef uint8_t hmac_vap_query_enent_id_enumm_uint8;

typedef struct {
    uint8_t uc_type;
    uint8_t uc_eid;
    uint8_t auc_resv[2];
} hmac_remove_ie_stru;

typedef enum {

    HMAC_VAP_ARP_PROBE_TYPE_HTC = BIT0, /* htc arp ̽��ʧ�ܻ�ر�htc�����ع��� */
    HMAC_VAP_ARP_PROBE_TYPE_M2S = BIT1, /* �л���siso �����ع��� */
    HMAC_VAP_ARP_PROBE_TYPE_DBAC = BIT2, /* Ƶ������NULL DATA���ر�RX AMSDU�����ع��� */
    HMAC_VAP_ARP_PROBE_TYPE_BTCOEX = BIT3, /* Ƶ������NULL DATA���ر�RX AMSDU�����ع��� */
    HMAC_VAP_ARP_PROBE_TYPE_BTCOEX_DELBA = BIT4, /* ���津����delba, ���ARP��ͨ��Ҫ�ָ��ۺϸ����������ع��� */
    HMAC_VAP_ARP_PROBE_TYPE_BUTT,
} hmac_vap_arp_probe_type_enum;
typedef uint8_t hmac_vap_arp_probe_type_enum_uint8;


typedef struct {
    frw_timeout_stru st_arp_probe_timer; /* ����ARP REQ��������ʱ�� */
    oal_atomic ul_rx_unicast_pkt_to_lan; /* ���յ��ĵ���֡���� */

    uint8_t uc_rx_no_pkt_count;        /* ��ʱʱ����û���յ�֡�Ĵ��� */
    oal_bool_enum_uint8 en_arp_probe_on; /* �Ƿ��arpͳ�ƣ����ع������� */
    uint8_t uc_arp_probe_type;
    oal_bool_enum_uint8 en_init_succ;
} hmac_vap_arp_probe_stru;

/* hmac vap�ṹ�� */
/* ����˽ṹ�������ӳ�Ա��ʱ���뱣�������ṹ��8�ֽڶ��� */
typedef struct hmac_vap_tag {
    /* ap sta�����ֶ� */
    oal_net_device_stru *pst_net_device;  /* VAP��Ӧ��net_devices */
    uint8_t auc_name[OAL_IF_NAME_SIZE]; /* VAP���� */
    hmac_vap_cfg_priv_stru st_cfg_priv;   /* wal hmac����ͨ�Žӿ� */

    oal_spin_lock_stru st_lock_state; /* ������Ϳ������VAP״̬���л��� */

    oal_mgmt_tx_stru st_mgmt_tx;
    frw_timeout_stru st_mgmt_timer;
    hmac_mgmt_timeout_param_stru st_mgmt_timetout_param;

    frw_timeout_stru st_scan_timeout; /* vap����ɨ��ʱ����������ʱ��������ʱ�������� */
    uint8_t uc_scan_upload_num;

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    oal_delayed_work st_ampdu_work;
    oal_spin_lock_stru st_ampdu_lock;
    oal_delayed_work st_set_hw_work;
    mac_cfg_ampdu_tx_on_param_stru st_mode_set;
#endif

    hmac_remove_ie_stru st_remove_ie; /* ������Ҫ���ε�IE��Ϣ */

    mac_blacklist_info_stru *pst_blacklist_info; /* ��������Ϣ */

    oal_net_device_stru *pst_p2p0_net_device; /* ָ��p2p0 net device */
    oal_net_device_stru *pst_del_net_device;  /* ָ����Ҫͨ��cfg80211 �ӿ�ɾ���� net device */
    oal_work_stru st_del_virtual_inf_worker;  /* ɾ��net_device �������� */
    oal_bool_enum_uint8 en_wait_roc_end;

    uint8_t bit_in_p2p_listen:1; /* vap�Ƿ���p2p listen���� */
    uint8_t bit_resv:7;
    uint8_t auc_resv6[2];

    oal_completion st_roc_end_ready; /* roc end completion */


#ifdef _PRE_WLAN_FEATURE_HS20
    hmac_cfg_qos_map_param_stru st_cfg_qos_map_param;
#endif

    uint8_t bit_init_flag : 1;  /* �����ر��ٴδ򿪱�־ */
    uint8_t bit_ack_policy : 1; /* ack policy: 0:normal ack 1:normal ack */
    uint8_t bit_reserved : 6;
    uint8_t auc_resv1[2];
    uint8_t ipaddr_obtained;  /* ����Ƿ��ȡ��IPv4��ַ����vap modeΪSTAʱʹ�� */

    uint32_t *pul_roam_info;
    oal_bool_enum_uint8 en_roam_prohibit_on; /* �Ƿ��ֹ���� */

#ifdef _PRE_WLAN_DFT_STAT
    uint8_t uc_device_distance;
    uint8_t uc_intf_state_cca;
    uint8_t uc_intf_state_co;
    uint8_t auc_resv2[1];
#endif

    uint16_t us_rx_timeout[WLAN_WME_AC_BUTT];     /* ��ͬҵ��������ʱʱ�� */
    uint16_t us_rx_timeout_min[WLAN_WME_AC_BUTT]; /* ��ͬҵ��������ʱʱ�� */

    uint16_t us_del_timeout;                     /* �೤ʱ�䳬ʱɾ��ba�Ự �����0��ɾ�� */
    mac_cfg_mode_param_stru st_preset_para;        /* STAЭ����ʱ���ǰ��Э��ģʽ */
    uint8_t auc_supp_rates[WLAN_MAX_SUPP_RATES]; /* ֧�ֵ����ʼ� */

    uint8_t auc_resv4[1];

#ifdef _PRE_WLAN_FEATURE_WMMAC
    uint8_t uc_ts_dialog_token; /* TS�Ự����α���ֵ */
    oal_bool_enum_uint8 en_wmmac_auth_flag;
    uint8_t uc_resv5[2];
#endif  // _PRE_WLAN_FEATURE_WMMAC

    oal_bool_enum_uint8 auc_query_flag[QUERY_ID_MODE_BUTT];
    mac_cfg_param_char_stru st_param_char;

    /* sta�����ֶ� */
    oal_bool_enum_uint8 en_no_beacon;
    uint8_t uc_no_smps_user_cnt_ap;              /* AP�����Ĳ�֧��SMPS���û���  */
    uint8_t uc_ba_dialog_token;                  /* BA�Ự����α���ֵ */
    oal_bool_enum_uint8 en_updata_rd_by_ie_switch; /* �Ƿ���ݹ�����ap�����Լ��Ĺ����� */

    uint8_t bit_sta_protocol_cfg : 1;
    uint8_t bit_protocol_fall : 1; /* ��Э���־λ */
    uint8_t bit_reassoc_flag : 1;  /* �����������ж��Ƿ�Ϊ�ع������� */
    
    uint8_t bit_rx_ampduplusamsdu_active : 3; /* 0��1����bit0:5G��bit1: 2g��11ax�� bit2:2g 11ax(0����1��) */
    uint8_t bit_sae_connect_with_pmkid : 1; /* 0:������PMKID��SAE���ӣ�1:����PMKID��SAE���� */
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R) ||      \
    defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    uint8_t bit_11k_auth_flag : 1; /* 11k ��֤��־λ */
    uint8_t bit_voe_11r_auth : 1;
    uint8_t bit_11k_auth_oper_class : 2;
    uint8_t bit_11r_over_ds : 1; /* �Ƿ�ʹ��11r over ds��0��ʾover ds��over air���� */
    uint8_t bit_11k_enable : 1;
    uint8_t bit_11v_enable : 1;
    uint8_t bit_11r_enable : 1;
    uint8_t bit_bcn_table_switch : 1;
    uint8_t bit_adaptive11r : 1; /* �Ƿ�ʹ��adaptive 11r���� */
    uint8_t bit_nb_rpt_11k : 1;
    uint8_t bit_11r_private_preauth : 1;
    uint8_t bit_resv2 : 5;
    uint8_t uc_resv;
#else
    uint8_t bit_resv : 1;
#endif                              // _PRE_WLAN_FEATURE_11K
    int8_t ac_desired_country[3]; /* Ҫ�����AP�Ĺ����ַ�����ǰ�����ֽ�Ϊ������ĸ��������Ϊ\0 */

    oal_dlist_head_stru st_pmksa_list_head;

    /* ��Ϣ�ϱ� */
    oal_wait_queue_head_stru query_wait_q; /* ��ѯ�ȴ����� */
    oal_station_info_stru station_info;
    station_info_extend_stru st_station_info_extend; /* CHR2.0ʹ�õ�STAͳ����Ϣ */

    oal_bool_enum_uint8 station_info_query_completed_flag; /* ��ѯ������־��OAL_TRUE����ѯ������OAL_FALSE����ѯδ���� */
    int16_t s_free_power;                                /* ���� */
    oal_bool_enum_uint8 en_monitor_mode;

    int32_t center_freq; /* ����Ƶ�� */
    mac_cfg_show_dieid_stru st_dieid;
    hmac_atcmdsrv_get_stats_stru st_atcmdsrv_get_status;
    oal_proc_dir_entry_stru *pst_proc_dir; /* vap��Ӧ��procĿ¼ */

#ifdef _PRE_WLAN_DFT_STAT
    /* ͳ����Ϣ+��Ϣ�ϱ������ֶΣ��޸�����ֶΣ������޸�SDT���ܽ�����ȷ */
    hmac_vap_query_stats_stru st_query_stats;
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    frw_timeout_stru st_edca_opt_timer; /* edca����������ʱ�� */
    uint32_t ul_edca_opt_time_ms;     /* edca����������ʱ������ */
    uint8_t uc_edca_opt_flag_ap;      /* apģʽ���Ƿ�ʹ��edca�Ż����� */
    uint8_t uc_edca_opt_flag_sta;     /* staģʽ���Ƿ�ʹ��edca�Ż����� */
    uint8_t uc_edca_opt_weight_sta;   /* ����beacon��edca������Ȩ�أ����ֵΪ 3 */
    uint8_t uc_idle_cycle_num;        /* vap��������idle״̬�����ڸ��� */
#endif

#ifdef _PRE_WLAN_TCP_OPT
    hmac_tcp_ack_stru st_hamc_tcp_ack[HCC_DIR_COUNT];
#endif

    mac_h2d_protection_stru st_prot;

    frw_timeout_stru st_ps_sw_timer; /* �͹��Ŀ��� */
    uint8_t uc_cfg_sta_pm_manual;  /* �ֶ�����sta pm mode�ı�־ */
    uint8_t uc_ps_mode;
    uint16_t us_check_timer_pause_cnt; /* �͹���pause���� */

    int32_t l_temp;
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    hmac_vap_wds_stru st_wds_table;
#endif
    uint8_t auc_dscp_tid_map[HMAC_MAX_DSCP_VALUE_NUM];
    hmac_vap_arp_probe_stru st_hmac_arp_probe;
#ifdef _PRE_WLAN_FEATURE_SAE
    oal_work_stru st_sae_report_ext_auth_worker; /* SAE report external auth req�������� */
    uint8_t duplicate_auth_seq2_flag;
    uint8_t duplicate_auth_seq4_flag;
    uint8_t resv[2];
#endif
    uint8_t owe_group[MAC_OWE_GROUP_SUPPORTED_NUM];
    uint8_t owe_group_cap;

    oal_bool_enum_uint8 en_is_psk; /* ��ι����Ƿ���psk */
    oal_bool_enum_uint8 en_sae_connect;
    oal_bool_enum_uint8 en_ps_rx_amsdu;
    oal_bool_enum_uint8 d2h_amsdu_switch;
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    uint8_t en_is_hid2d_state;
    uint8_t  uc_seq_idx;
    uint16_t aus_last_seqnum[MAC_HID2D_SEQNUM_CNT];
    uint8_t  uc_resv10[2];
#endif

#ifdef _PRE_WLAN_FEATURE_TWT
    mac_cfg_twt_stru st_twt_cfg; /* TWT�������ò��� */
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    uint8_t auc_resv11[1];
    uint8_t uc_mac_padding;
    uint8_t uc_disable_mu_edca;
    uint8_t uc_htc_order_flag;
    uint32_t ul_htc_info;
#endif

#ifdef _PRE_WLAN_FEATURE_FTM // FTM��֤������gas init comeback��ʱ
    frw_timeout_stru    st_ftm_timer;
    mac_gas_mgmt_stru   st_gas_mgmt;
#endif
    uint8_t             hal_dev_id; /* host vap��Ӧ��hal device id */
    oal_switch_enum_uint8 tcp_ack_buf_use_ctl_switch;   /* ��Ӧ�ò���Ƶ�TCP ack�������޿��� */

    mac_vap_stru st_vap_base_info; /* MAC vap��ֻ�ܷ������! */
} hmac_vap_stru;

typedef enum _hmac_cac_event_ {
    HMAC_CAC_STARTED = 0,
    HMAC_CAC_FINISHED,
    HMAC_CAC_ABORTED,

    HMAC_CAC_BUTT
} hmac_cac_event;

typedef struct {
    hmac_cac_event en_type;
    uint32_t ul_freq;
    wlan_channel_bandwidth_enum_uint8 en_bw_mode;
    uint8_t auc[23];
} hmac_cac_event_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_vap_destroy(hmac_vap_stru *pst_vap);
uint32_t hmac_vap_init(hmac_vap_stru *pst_hmac_vap,
                                    uint8_t uc_chip_id,
                                    uint8_t uc_device_id,
                                    uint8_t uc_vap_id,
                                    mac_cfg_add_vap_param_stru *pst_param);

uint32_t hmac_vap_creat_netdev(hmac_vap_stru *pst_hmac_vap,
                                            int8_t *puc_netdev_name,
                                            uint8_t uc_netdev_name_len,
                                            int8_t *puc_mac_addr);

uint16_t hmac_vap_check_ht_capabilities_ap(hmac_vap_stru *pst_hmac_vap,
                                                        uint8_t *puc_payload,
                                                        uint32_t ul_msg_len,
                                                        hmac_user_stru *pst_hmac_user_sta);
uint32_t hmac_search_ht_cap_ie_ap(hmac_vap_stru *pst_hmac_vap,
                                               hmac_user_stru *pst_hmac_user_sta,
                                               uint8_t *puc_payload,
                                               uint16_t us_index,
                                               oal_bool_enum en_prev_asoc_ht);
oal_bool_enum_uint8 hmac_vap_addba_check(hmac_vap_stru *pst_hmac_vap,
                                                hmac_user_stru *pst_hmac_user,
                                                uint8_t uc_tidno);

void hmac_vap_net_stopall(void);
void hmac_vap_net_startall(void);

oal_bool_enum_uint8 hmac_flowctl_check_device_is_sta_mode(void);
void hmac_vap_net_start_subqueue(uint16_t us_queue_idx);
void hmac_vap_net_stop_subqueue(uint16_t us_queue_idx);
void hmac_handle_disconnect_rsp(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
                                               mac_reason_code_enum_uint16 en_disasoc_reason);
uint8_t *hmac_vap_get_pmksa(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_bssid);
uint32_t hmac_tx_get_mac_vap(uint8_t uc_vap_id, mac_vap_stru **pst_vap_stru);
uint32_t hmac_restart_all_work_vap(uint8_t uc_chip_id);
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
void hmac_set_ampdu_worker(oal_delayed_work *pst_work);
void hmac_set_ampdu_hw_worker(oal_delayed_work *pst_work);
#endif

void hmac_vap_state_restore(hmac_vap_stru *pst_hmac_vap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_vap.h */
