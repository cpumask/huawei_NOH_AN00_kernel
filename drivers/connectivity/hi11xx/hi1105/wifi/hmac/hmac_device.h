

#ifndef __HMAC_DEVICE_H__
#define __HMAC_DEVICE_H__

#ifdef __cplusplus // windows ut���벻����������������
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"


#include "mac_vap.h"
#include "hmac_vap.h"
#ifdef _PRE_WLAN_TCP_OPT
#include "hmac_tcp_opt_struc.h"
#include "oal_hcc_host_if.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DEVICE_H
/* 2 �궨�� */
#ifdef _PRE_WLAN_TCP_OPT
#define HCC_TRANS_THREAD_POLICY   SCHED_FIFO
#define HCC_TRANS_THERAD_PRIORITY 10
#define HCC_TRANS_THERAD_NICE     (-10)
#endif

#define IS_EQUAL_RATES(rate1, rate2) (((rate1)&0x7f) == ((rate2)&0x7f))

typedef enum {
    HMAC_ACS_TYPE_INIT = 0, /* ��ʼ�Զ��ŵ�ѡ�� */
    HMAC_ACS_TYPE_CMD = 1,  /* ��������Զ��ŵ�ѡ�� */
    HMAC_ACS_TYPE_FREE = 2, /* ���е��Զ��ŵ�ѡ�� */
} hmac_acs_type_enum;
typedef uint8_t hmac_acs_type_enum_uint8;

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
#define PMF_BLACK_LIST_MAX_CNT 6
#endif

/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* �洢ÿ��ɨ�赽��bss��Ϣ */
typedef struct {
    oal_dlist_head_stru st_dlist_head; /* ����ָ�� */
    // oal_spin_lock_stru     st_lock;             /* �� */
    mac_bss_dscr_stru st_bss_dscr_info; /* bss������Ϣ�������ϱ��Ĺ���֡ */
} hmac_scanned_bss_info;

/* �洢��hmac device�µ�ɨ����ά���Ľṹ�� */
typedef struct {
    oal_spin_lock_stru st_lock;
    oal_dlist_head_stru st_bss_list_head;
    uint32_t ul_bss_num;
} hmac_bss_mgmt_stru;

/* ACSɨ����� */
typedef struct {
    mac_scan_op_enum_uint8 en_scan_op;
    oal_bool_enum_uint8 en_switch_chan;
    uint8_t uc_acs_type;
    uint8_t auc_rsv[1];
} hmac_acs_cfg_stru;

/* ɨ�����н����¼ */
typedef struct {
    hmac_bss_mgmt_stru st_bss_mgmt;                                   /* �洢ɨ��BSS����Ĺ���ṹ */
    wlan_scan_chan_stats_stru ast_chan_results[WLAN_MAX_CHANNEL_NUM]; /* �ŵ�ͳ��/������� */
    uint8_t uc_chan_numbers;                                        /* �˴�ɨ���ܹ���Ҫɨ����ŵ����� */
    uint8_t uc_device_id : 4;
    uint8_t uc_chip_id : 4;
    uint8_t uc_vap_id;                           /* ����ִ��ɨ���vap id */
    mac_scan_status_enum_uint8 en_scan_rsp_status; /* ����ɨ����ɷ��ص�״̬�룬�ǳɹ����Ǳ��ܾ� */

    oal_time_us_stru st_scan_start_timestamp; /* ɨ��ά��ʹ�� */
    mac_scan_cb_fn p_fn_cb;                   /* �˴�ɨ������Ļص�����ָ�� */

    uint64_t ull_cookie;                      /* ����P2P ���������ϱ���cookie ֵ */
    mac_vap_state_enum_uint8 en_vap_last_state; /* ����VAP����ɨ��ǰ��״̬,AP/P2P GOģʽ��20/40Mɨ��ר�� */
    oal_time_t_stru st_scan_start_time;         /* ɨ����ʼʱ��� */

    // ���Ӽ�¼ɨ�����ͣ��Ա�ʶ��HiD2Dɨ�����ش���
    wlan_scan_mode_enum_uint8 en_scan_mode;
} hmac_scan_record_stru;

/* ɨ�������ؿ�����Ϣ */
typedef struct {
    /* scan ��ؿ�����Ϣ */
    oal_bool_enum_uint8 en_is_scanning;             /* host���ɨ�������Ƿ�����ִ�� */
    oal_bool_enum_uint8 en_is_random_mac_addr_scan; /* �Ƿ�Ϊ���mac addrɨ�裬Ĭ�Ϲر�(���ƻ��꿪���·���) */
    oal_bool_enum_uint8 en_complete;                /* �ں���ͨɨ�������Ƿ���ɱ�־ */
    oal_bool_enum_uint8 en_sched_scan_complete;     /* ����ɨ���Ƿ��������б�� */

    oal_cfg80211_scan_request_stru *pst_request;              /* �ں��·���ɨ������ṹ�� */
    oal_cfg80211_sched_scan_request_stru *pst_sched_scan_req; /* �ں��·��ĵ���ɨ������ṹ�� */

    oal_wait_queue_head_stru st_wait_queue;
    oal_spin_lock_stru st_scan_request_spinlock; /* �ں��·���request��Դ�� */

    frw_timeout_stru st_scan_timeout; /* ɨ��ģ��host��ĳ�ʱ������ʹ�õĶ�ʱ�� */
    frw_timeout_stru st_init_scan_timeout;
    uint8_t auc_random_mac[WLAN_MAC_ADDR_LEN]; /* ɨ��ʱ���õ����MAC */
    uint8_t auc_resv[2];

    hmac_scan_record_stru st_scan_record_mgmt; /* ɨ�����м�¼������Ϣ������ɨ�����ͷ���ɨ���ߵ������Ϣ */
} hmac_scan_stru;

typedef struct {
    oal_wait_queue_head_stru st_wait_queue;
    oal_bool_enum_uint8 auc_complete_flag[MAC_PSM_QUERY_TYPE_BUTT];
    mac_psm_query_stat_stru ast_psm_stat[MAC_PSM_QUERY_TYPE_BUTT];
}hmac_psm_flt_stat_query_stru;

#ifdef _PRE_WLAN_FEATURE_NRCOEX
typedef struct {
    oal_wait_queue_head_stru st_wait_queue;
    mac_nrcoex_stat_stru st_nrcoex_stat;
    oal_bool_enum_uint8 en_query_completed_flag;
} hmac_nrcoex_stat_query_stru;
#endif

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
typedef struct {
    uint8_t uc_cnt;
    uint8_t uc_first_idx; /* ���ڼ�¼����ĺ������±� */
    uint8_t auc_black_list[PMF_BLACK_LIST_MAX_CNT][WLAN_MAC_ADDR_LEN];
} hmac_pmf_black_list_stru;
#endif
/* hmac device�ṹ�壬��¼ֻ������hmac��device������Ϣ */
typedef struct {
    hmac_scan_stru st_scan_mgmt; /* ɨ�����ṹ�� */
    uint32_t ul_p2p_intf_status;
    oal_wait_queue_head_stru st_netif_change_event;
    mac_device_stru *pst_device_base_info; /* ָ�򹫹�����mac device */
    oal_bool_enum_uint8 en_init_scan : 1;
    oal_bool_enum_uint8 en_rfu : 1;
    oal_bool_enum_uint8 en_in_init_scan : 1;
    oal_bool_enum_uint8 en_rescan_idle : 1;
    uint8_t uc_resv_bit : 4;
    uint8_t auc_resvx[3];
    mac_channel_stru ast_best_channel[WLAN_BAND_BUTT];

#ifdef _PRE_WLAN_TCP_OPT
    oal_bool_enum_uint8 sys_tcp_rx_ack_opt_enable;
    oal_bool_enum_uint8 sys_tcp_tx_ack_opt_enable;
    oal_bool_enum_uint8 en_start_via_priv : 1;
    uint8_t uc_rfu_bit : 7;
    uint8_t uc_rfu;
#else
    oal_bool_enum_uint8 en_start_via_priv : 1;
    uint8_t uc_rfu_bit : 7;
    uint8_t auc_rfu[3];
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef CONFIG_HAS_EARLYSUSPEND
    struct early_suspend early_suspend;  // early_suspend֧��
#endif
    oal_spin_lock_stru st_suspend_lock;
#endif
    hmac_device_btcoex_stru st_hmac_device_btcoex;

    frw_timeout_stru st_scan_timer; /* hostɨ�趨ʱ�������л��ŵ� */
    hmac_psm_flt_stat_query_stru st_psm_flt_stat_query;

    oal_bool_enum_uint8 en_ap_support_160m;

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    hmac_pmf_black_list_stru st_pmf_black_list;
#endif

#ifdef _PRE_WLAN_FEATURE_MONITOR
    oal_bool_enum_uint8 en_monitor_mode;
    uint8_t sniffer_mode;
    uint8_t auc_rsv[2];
    uint32_t ul_mgmt_frames_cnt;
    uint32_t ul_control_frames_cnt;
    uint32_t ul_data_frames_cnt;
    uint32_t ul_others_frames_cnt;
#endif
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    hmac_nrcoex_stat_query_stru st_nrcoex_stat_query;
#endif
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
    uint32_t ul_addba_req_cnt; // addba req����
    uint32_t ul_group_rekey_cnt; // group_rekey����
#endif
} hmac_device_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
uint32_t hmac_board_exit(mac_board_stru *pst_board);

uint32_t hmac_config_host_dev_init(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                                uint8_t *puc_param);
uint32_t hmac_config_host_dev_exit(mac_vap_stru *pst_mac_vap);
uint32_t hmac_board_init(mac_board_stru *pst_board);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
oal_bool_enum_uint8 hmac_device_pmf_find_black_list(hmac_device_stru *pst_hmac_dev,
                                                           uint8_t *puc_mac_addr);
void hmac_device_pmf_add_black_list(hmac_device_stru *pst_hmac_dev, uint8_t *puc_mac_addr);
#endif

void hmac_device_create_random_mac_addr(mac_device_stru *pst_mac_dev,
                                                       mac_vap_stru *pst_mac_vap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_device.h */
