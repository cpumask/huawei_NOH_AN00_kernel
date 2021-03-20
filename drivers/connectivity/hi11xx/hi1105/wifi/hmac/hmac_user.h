

#ifndef __HMAC_USER_H__
#define __HMAC_USER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "mac_user.h"
#include "mac_resource.h"
#include "mac_common.h"
#include "hmac_ext_if.h"
#include "hmac_edca_opt.h"
#include "hmac_btcoex.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_USER_H

/* 2 �궨�� */
#define HMAC_ADDBA_EXCHANGE_ATTEMPTS 10 /* ��ͼ����ADDBA�Ự������������ */

#define HMAC_USER_IS_AMSDU_SUPPORT(_user, _tid) (((_user)->uc_amsdu_supported) & (0x01 << ((_tid)&0x07)))

#define HMAC_USER_SET_AMSDU_SUPPORT(_user, _tid) (((_user)->uc_amsdu_supported) |= (0x01 << ((_tid)&0x07)))

#define HMAC_USER_SET_AMSDU_NOT_SUPPORT(_user, _tid)      \
    (((_user)->uc_amsdu_supported) &= (uint8_t)(~(0x01 << ((_tid)&0x07))))

#define MAX_JUDGE_CACHE_LENGTH 20 /* ҵ��ʶ��-�û���ʶ����г��� */
#define MAX_CONFIRMED_FLOW_NUM 2  /* ҵ��ʶ��-�û���ʶ��ҵ������ */
#define MAX_CLEAR_JUDGE_TH     2  /* ҵ��ʶ��-�û�δʶ��ҵ�����������ʶ��Ĵ������� */

#define HMAC_USER_STATS_PKT_INCR(_member, _cnt) ((_member) += (_cnt))
// for CSEC cycle complexity
#define HMAC_AP_IS_BW_2G_DOUBLE_NSS(_en_support_max_nss, _pst_mac_vap) \
    (((_en_support_max_nss) == WLAN_DOUBLE_NSS) && ((_pst_mac_vap)->st_channel.en_band == WLAN_BAND_2G))


#define DMAC_UCAST_FRAME_TX_COMP_TIMES 10
/* ax PF-��֤����Ҫ��HE SU֡ΪAMPDU */
#define DMAC_UCAST_FRAME_TX_COMP_TIMES_HE 0

#ifdef _PRE_WLAN_FEATURE_HIEX
#define HMAC_HIEX_MAX_SII_SIZE             8
#define HMAC_HIEX_SII_AGING_INTERVAL_MS    5000
#define HMAC_HIEX_SII_AGING_MS             60000
#define HMAC_HIEX_GET_USER_SIT(user) (&(user)->st_hiex_sit)
#endif

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
#ifdef _PRE_WLAN_FEATURE_HIEX
/* stream info item */
typedef struct {
    uint32_t  server_ip;    /* all net endian */
    uint32_t  client_ip;
    uint16_t  server_port;
    uint16_t  client_port;
    uint32_t  mark;         /* used for game/voice... */
    uint32_t  last_active;
} hmac_hiex_sii_stru;

/* stream info table */
typedef struct {
    oal_spin_lock_stru  lock;
    uint32_t          count;
    hmac_hiex_sii_stru  item[HMAC_HIEX_MAX_SII_SIZE];
    frw_timeout_stru    aging_timer;
} hmac_hiex_sit_stru;
#endif

/* TID��Ӧ�ķ���BA�Ự��״̬ */
typedef struct {
    frw_timeout_stru st_addba_timer;
    dmac_ba_alarm_stru st_alarm_data;

    dmac_ba_conn_status_enum_uint8 en_ba_status; /* ��TID��Ӧ��BA�Ự��״̬ */
    uint8_t uc_addba_attemps;                  /* ��������BA�Ự�Ĵ��� */
    uint8_t uc_dialog_token;                   /* �������� */
    oal_bool_enum_uint8 uc_ba_policy;            /* Immediate=1 Delayed=0 */
    oal_spin_lock_stru st_ba_status_lock;        /* ��TID��Ӧ��BA�Ự��״̬�� */
} hmac_ba_tx_stru;

typedef struct {
    oal_bool_enum_uint8 in_use;          /* ����BUF�Ƿ�ʹ�� */
    uint8_t uc_num_buf;                /* MPDUռ�õ�netbuf(����������)���� */
    uint16_t us_seq_num;               /* MPDU��Ӧ�����к� */
    oal_netbuf_head_stru st_netbuf_head; /* MPDU��Ӧ���������׵�ַ */
    uint32_t ul_rx_time;               /* ���ı������ʱ��� */
} hmac_rx_buf_stru;

typedef struct {
    void *pst_ba;
    uint8_t uc_tid;
    mac_delba_initiator_enum_uint8 en_direction;
    uint8_t uc_resv[1];
    uint8_t uc_vap_id;
    uint16_t us_mac_user_idx;
    uint16_t us_timeout_times;

} hmac_ba_alarm_stru;

/* Hmac����ղ�BA�Ự��� */
typedef struct {
    uint16_t us_baw_start;       /* ��һ��δ�յ���MPDU�����к� */
    uint16_t us_baw_end;         /* ���һ�����Խ��յ�MPDU�����к� */
    uint16_t us_baw_tail;        /* ĿǰRe-Order�����У��������к� */
    uint16_t us_baw_size;        /* Block_Ack�Ự��buffer size��С */
    uint16_t us_last_relseq;     /* ĿǰRe-Order�����У���һ��release seqnum */
    oal_atomic ul_ref_cnt;
    oal_spin_lock_stru st_ba_lock; /* ����hcc�̺߳��¼��̲߳��� */

    oal_bool_enum_uint8 en_timer_triggered;      /* ��һ���ϱ��Ƿ�Ϊ��ʱ���ϱ� */
    oal_bool_enum_uint8 en_is_ba;                /* Session Valid Flag */
    dmac_ba_conn_status_enum_uint8 en_ba_status; /* ��TID��Ӧ��BA�Ự��״̬ */
    uint16_t uc_mpdu_cnt;                      /* ��ǰRe-Order�����У�MPDU����Ŀ td */

    hmac_ba_alarm_stru st_alarm_data;
    /* frw_timeout_stru                st_ba_timer; */
    /* ����action֡��� */
    mac_back_variant_enum_uint8 en_back_var; /* BA�Ự�ı��� */
    uint8_t uc_dialog_token;               /* ADDBA����֡��dialog token */
    uint8_t uc_ba_policy;                  /* Immediate=1 Delayed=0 */
    uint8_t uc_lut_index;                  /* ���ն�Session H/w LUT Index */
    uint16_t us_status_code;               /* ����״̬�� */
    uint16_t us_ba_timeout;                /* BA�Ự������ʱʱ�� */
    uint8_t *puc_transmit_addr;            /* BA�Ự���Ͷ˵�ַ */
    oal_bool_enum_uint8 en_amsdu_supp;       /* BLOCK ACK֧��AMSDU�ı�ʶ */
    uint8_t auc_resv1[1];
    uint16_t us_baw_head;                  /* bitmap����ʼ���к� */
    uint32_t aul_rx_buf_bitmap[2];
    /* Re-Order���� */
    hmac_rx_buf_stru ast_re_order_list[WLAN_AMPDU_RX_HE_BUFFER_SIZE];
} hmac_ba_rx_stru;

#define HMAC_GET_BA_RX_DHL(_pst_handle, idx) &(((hmac_ba_rx_stru *)(_pst_handle))->ast_re_order_list[idx])

/* user�ṹ�У�TID��Ӧ��BA��Ϣ�ı���ṹ */
typedef struct {
    uint8_t uc_tid_no;
    oal_bool_enum_uint8 en_ampdu_start; /* ��ʶ��tid�µ�AMPDU�Ƿ��Ѿ������� */
    uint16_t us_hmac_user_idx;
    hmac_ba_tx_stru st_ba_tx_info;
    hmac_ba_rx_stru *pst_ba_rx_info;            /* ���ڲ��ִ������ƣ��ⲿ���ڴ浽LocalMem������ */
    frw_timeout_stru st_ba_timer;               /* ���������򻺳峬ʱ */
    oal_spin_lock_stru st_ba_timer_lock;        /* ����hcc�̺߳��¼��̲߳��� */
    oal_bool_enum_uint8 en_ba_handle_tx_enable; /* ��tid�·��ͷ����Ƿ�����BA�������������� */
    oal_bool_enum_uint8 en_ba_handle_rx_enable; /* ��tid�½��ͷ����Ƿ�����BA�������������� */
    uint8_t auc_resv[2];
} hmac_tid_stru;
typedef struct {
    uint32_t bit_short_preamble : 1,          /* �Ƿ�֧��802.11b��ǰ���� 0=��֧�֣� 1=֧�� */
               bit_erp : 1,                     /* AP����STA����ʹ��,ָʾuser�Ƿ���ERP������ 0=��֧�֣�1=֧�� */
               bit_short_slot_time : 1,         /* ��ʱ϶: 0=��֧��, 1=֧�� */
               bit_11ac2g : 1,
               bit_owe : 1,
               bit_resv : 27;
} hmac_user_cap_info_stru;
typedef struct {
    uint8_t uc_rs_nrates;                           /* ���� */
    uint8_t auc_rs_rates[WLAN_USER_MAX_SUPP_RATES]; /* ���� */
} hmac_rate_stru;

#ifdef _PRE_WLAN_FEATURE_WAPI
#define WAPI_KEY_LEN        16
#define WAPI_PN_LEN         16
#define HMAC_WAPI_MAX_KEYID 2

typedef struct {
    uint32_t ulrx_mic_calc_fail;   /* ���ڲ���������mic������� */
    uint32_t ultx_ucast_drop;      /* ����Э��û����ɣ���֡drop�� */
    uint32_t ultx_wai;
    uint32_t ultx_port_valid;      /* Э����ɵ�����£����͵�֡���� */
    uint32_t ulrx_port_valid;      /* Э����ɵ�����£����յ�֡���� */
    uint32_t ulrx_idx_err;         /* ����idx������� */
    uint32_t ulrx_netbuff_len_err; /* ����netbuff���ȴ��� */
    uint32_t ulrx_idx_update_err;  /* ��Կ���´��� */
    uint32_t ulrx_key_en_err;      /* ��Կû��ʹ�� */
    uint32_t ulrx_pn_odd_err;      /* PN��żУ����� */
    uint32_t ulrx_pn_replay_err;   /* PN�ط� */
    uint32_t ulrx_memalloc_err;    /* rx�ڴ�����ʧ�� */
    uint32_t ulrx_decrypt_ok;      /* ���ܳɹ��Ĵ��� */

    uint32_t ultx_memalloc_err;    /* �ڴ����ʧ�� */
    uint32_t ultx_mic_calc_fail;   /* ���ڲ���������mic������� */
    // uint32_t ultx_drop_wai;     /* wai֡drop�Ĵ��� */
    uint32_t ultx_encrypt_ok;      /* ���ܳɹ��Ĵ��� */
    uint8_t aucrx_pn[WAPI_PN_LEN]; /* ���ⷢ��ʱ����¼���շ���֡��PN,��pn����ʱ��ˢ�� */

} hmac_wapi_debug;

typedef struct {
    uint8_t auc_wpi_ek[WAPI_KEY_LEN];
    uint8_t auc_wpi_ck[WAPI_KEY_LEN];
    uint8_t auc_pn_rx[WAPI_PN_LEN];
    uint8_t auc_pn_tx[WAPI_PN_LEN];
    uint8_t uc_key_en;
    uint8_t auc_rsv[3];
} hmac_wapi_key_stru;

typedef struct tag_hmac_wapi_stru {
    uint8_t uc_port_valid;    /* wapi���ƶ˿� */
    uint8_t uc_keyidx;
    uint8_t uc_keyupdate_flg; /* key���±�־ */
    uint8_t uc_pn_inc;        /* pn����ֵ */

    hmac_wapi_key_stru ast_wapi_key[HMAC_WAPI_MAX_KEYID]; /* keyed: 0~1 */

#ifdef _PRE_WAPI_DEBUG
    hmac_wapi_debug st_debug; /* ά�� */
#endif

    uint8_t (*wapi_filter_frame)(struct tag_hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuff);
    oal_bool_enum_uint8 (*wapi_is_pn_odd)(uint8_t *puc_pn); /* �ж�pn�Ƿ�Ϊ���� */
    uint32_t (*wapi_decrypt)(struct tag_hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuff);
    uint32_t (*wapi_encrypt)(struct tag_hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuf);
    oal_netbuf_stru *(*wapi_netbuff_txhandle)(struct tag_hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuf);
    oal_netbuf_stru *(*wapi_netbuff_rxhandle)(struct tag_hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuf);
} hmac_wapi_stru;

#endif

/* ҵ��ʶ��-��Ԫ��ṹ��: ����Ψһ�ر�ʶҵ���� */
typedef struct {
    uint32_t ul_sip; /* ip */
    uint32_t ul_dip;

    uint16_t us_sport; /* �˿� */
    uint16_t us_dport;

    uint32_t ul_proto; /* Э�� */

} hmac_tx_flow_info_stru;

/* ҵ��ʶ��-��ʶ����нṹ��: */
typedef struct {
    hmac_tx_flow_info_stru st_flow_info;

    uint32_t ul_len;          /* �������� */
    uint8_t uc_flag;          /* ��Чλ�����ڼ��� */

    uint8_t uc_udp_flag;      /* udp flagΪ1��ΪUDP֡ */
    uint8_t uc_tcp_flag;      /* tcp flagΪ1��ΪTCP֡ */

    uint8_t uc_rtpver;        /* RTP version */
    uint32_t ul_rtpssrc;      /* RTP SSRC */
    uint32_t ul_payload_type; /* RTP:���1bit�����룬����¼��Ч�غ�����(PT)7bit */

} hmac_tx_judge_info_stru;

/* ҵ��ʶ��-��ʶ�������Ҫҵ��ṹ��: */
typedef struct {
    hmac_tx_flow_info_stru st_flow_info;

    uint32_t ul_average_len;  /* ҵ������ƽ������ */
    uint8_t uc_flag;          /* ��Чλ */

    uint8_t uc_udp_flag;      /* udp flagΪ1��ΪUDP֡ */
    uint8_t uc_tcp_flag;      /* tcp flagΪ1��ΪTCP֡ */

    uint8_t uc_rtpver;        /* RTP version */
    uint32_t ul_rtpssrc;      /* RTP SSRC */
    uint32_t ul_payload_type; /* ���1bit�����룬����¼��Ч�غ�����(PT)7bit */

    uint32_t ul_wait_check_num; /* ������б��д�ҵ������� */

} hmac_tx_major_flow_stru;

/* ҵ��ʶ��-�û���ʶ��ṹ��: */
typedef struct {
    hmac_tx_flow_info_stru st_cfm_flow_info; /* ��ʶ��ҵ�����Ԫ����Ϣ */

    unsigned long ul_last_jiffies;               /* ��¼��ʶ��ҵ�����������ʱ�� */
    uint16_t us_cfm_tid;                   /* ��ʶ��ҵ��tid */

    uint16_t us_cfm_flag;                  /* ��Чλ */

} hmac_tx_cfm_flow_stru;

/* ҵ��ʶ��-�û���ʶ��ҵ�����: */
typedef struct {
    unsigned long ul_jiffies_st;                                         /* ��¼��ʶ��ҵ����е���ʼʱ������������ʱ�� */
    unsigned long ul_jiffies_end;
    uint32_t ul_to_judge_num;                                      /* �û���ʶ��ҵ����г��� */
    hmac_tx_judge_info_stru ast_judge_cache[MAX_JUDGE_CACHE_LENGTH]; /* ��ʶ�������� */

} hmac_tx_judge_list_stru;

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
#define MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT  2
#define MAC_11V_ROAM_SCAN_FULL_CHANNEL_LIMIT (MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT + 1)

typedef uint32_t (*mac_user_callback_func_11v)(void *, void *, void *);
/* 11v ������Ϣ�ṹ�� */
typedef struct {
    uint8_t uc_user_bsst_token;                   /* �û�����bss transition ֡������ */
    uint8_t uc_user_status;                       /* �û�11V״̬ */
    uint8_t uc_11v_roam_scan_times;               /* ���ŵ�11v����ɨ�����  */
    oal_bool_enum_uint8 en_only_scan_one_time;      /* ֻɨ��һ�α�־λ */
    frw_timeout_stru st_status_wait_timer;          /* �ȴ��û���Ӧ֡�ļ�ʱ�� */
    mac_scan_req_h2d_stru st_scan_h2d_params;       /* �����ھ��б��ȡɨ���ŵ��б� */
    mac_user_callback_func_11v mac_11v_callback_fn; /* �ص�����ָ�� */
} hmac_user_11v_ctrl_stru;
#endif

/* �������ķ���MSDU RING���������� */
typedef struct {
    msdu_info_ring_stru base_ring_info;
    uint8_t *host_ring_buf;
    uint64_t host_ring_dma_addr;
    uint32_t host_ring_buf_size;
    uint16_t release_index;              /* �ϴ��ͷŵ��ĵ�ַ */
    uint32_t msdu_cnt;                   /* msdu���� */
    uint32_t msdu_len;                   /* msdu�ܳ��� */
    oal_bool_enum_uint8 enabled;
    uint8_t host_tx_flag;
    oal_netbuf_stru **netbuf_list;       /* ring��netbuf����, ����ringλ��һ�� */
    oal_spin_lock_stru lock;
} hmac_msdu_info_ring_stru;

typedef union {
    uint32_t word2;
    struct {
        uint16_t write_ptr;
        uint16_t tx_msdu_ring_depth : 4;
        uint16_t max_aggr_amsdu_num : 4;
        uint16_t reserved : 8;
    } word2_bit;
} un_msdu_info_word2;

typedef union {
    uint32_t word3;
    struct {
        uint16_t read_ptr;
        uint16_t reserved;
    } word3_bit;
} un_msdu_info_word3;

#define TX_RING_INFO_LEN      16
#define TX_RING_INFO_WORD_NUM (TX_RING_INFO_LEN / OAL_SIZEOF(uint32_t))

typedef struct {
    /* ����user/tid��Ӧtx_msdu_info_ring4��word��ַ */
    uint64_t word_addr[TX_RING_INFO_WORD_NUM]; /* hal_tx_msdu_info_ring_stru */
    /* д��/�ض�tx msdu info ring��Ϣ */
    un_msdu_info_word2 msdu_info_word2;
    un_msdu_info_word3 msdu_info_word3;
} hmac_tx_ring_device_info_stru;

typedef struct {
    uint32_t tx_ring_device_base_addr;
    hmac_tx_ring_device_info_stru tx_ring_device_info[WLAN_ASSOC_USER_MAX_NUM][WLAN_TIDNO_BUTT];
} hmac_tx_ring_device_info_mgmt_stru;

/* TID������Ϣ */
typedef struct {
    tid_state_enum_uint8 state;  /* TID���ԣ�״̬���ö�ٶ��� */
    uint8_t tid_no;
    uint16_t user_index;
    uint8_t vap_index;
    uint8_t soft_retries;        /* TID���ԣ�����ش����� */
    txmode_enum_uint8 tx_mode;   /* TID���ԣ�����ģʽrifs/aggr/normal */
    uint8_t ampdu_num;           /* TID���ԣ�ampdu�ۺϸ��� */
    uint8_t amsdu_num;           /* TID���ԣ�amsdu�ۺϸ��� */
    uint8_t encap_80211_offload; /* TID���ԣ�Ӳ������80211����ͷ */
    uint32_t cur_msdus;          /* TID״̬�������е�msdu���� */
    uint32_t cur_bytes;          /* TID״̬�������е��ֽ��� */
    uint32_t tx_msdus;           /* TID״̬�������е��ܷ��Ͱ��� */
    uint32_t tx_bytes;           /* TID״̬�������е��ܷ����ֽ��� */
    uint32_t rx_msdus;           /* TID״̬�������е��ܽ��հ��� */
    uint32_t rx_bytes;           /* TID״̬�������е��ܽ����ֽ��� */
    uint32_t last_period_tx_msdus; /* ��һͳ�������ڷ��ͼ��� */
    uint32_t tx_update_freq;       /* ��TID�·������¼���Ƶ�� */
    hmac_tx_ring_device_info_stru *tx_ring_device_info;
    hmac_ba_tx_stru *tx_ba_info;
    hmac_ba_rx_stru *rx_ba_info;
    oal_netbuf_head_stru tx_msdu_buffer;
    hmac_msdu_info_ring_stru non_ba_tx_ring;
    hmac_msdu_info_ring_stru ba_tx_ring;
} hmac_tid_info_stru;

/* user�ṹ�У�����pn�Ŷ�Ӧ�ı���ṹ */
typedef struct {
    uint16_t qos_last_msb_pn[WLAN_TID_MAX_NUM];
    uint32_t qos_last_lsb_pn[WLAN_TID_MAX_NUM];
    uint8_t  resv[2];
    uint16_t mcast_nonqos_last_msb_pn;
    uint32_t mcast_nonqos_last_lsb_pn;
    uint8_t  resv1[2];
    uint16_t ucast_nonqos_last_msb_pn;
    uint32_t ucast_nonqos_last_lsb_pn;
} hmac_last_pn_stru;

/* �����ýṹ���������һ��Ҫ����oal_memzero */
typedef struct {
    /* ��ǰVAP������AP��STAģʽ�������ֶ�Ϊuser��STA��APʱ�����ֶΣ�������ֶ���ע��!!! */
    uint8_t uc_is_wds;                              /* �Ƿ���wds�û� */
    uint8_t uc_amsdu_supported;                     /* ÿ��λ����ĳ��TID�Ƿ�֧��AMSDU */
    uint16_t us_amsdu_maxsize;                      /* amsdu��󳤶� */
    hmac_amsdu_stru ast_hmac_amsdu[WLAN_TID_MAX_NUM]; /* asmdu���� */
    hmac_tid_stru ast_tid_info[WLAN_TID_MAX_NUM];     /* ������TID��ص���Ϣ */
    uint32_t aul_last_timestamp[WLAN_TID_MAX_NUM];  /* ʱ�������ʵ��5���������Ľ���BA */
    uint8_t auc_ch_text[WLAN_CHTXT_SIZE];           /* WEP�õ���ս���� */
    frw_timeout_stru st_mgmt_timer;                   /* ��֤�����ö�ʱ�� */
    frw_timeout_stru st_defrag_timer;                 /* ȥ��Ƭ��ʱ��ʱ�� */
    oal_netbuf_stru *pst_defrag_netbuf;
    /* �ñ�־��ʾ��TID�Ƿ���Խ���BA�Ự�����ڵ���5ʱ���Խ���BA�Ự���ñ�־���û���ʼ����ɾ��BA�Ựʱ���� */
    uint8_t auc_ba_flag[WLAN_TID_MAX_NUM];
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    mac_sa_query_stru st_sa_query_info; /* sa query���̵Ŀ�����Ϣ */
#endif
    /* user��ѡ����,��¼�Զ�assoc req����assoc rsp֡��supported rates��assoc rsp���Ƶ����� */
    hmac_rate_stru st_op_rates;
    hmac_user_cap_info_stru st_hmac_cap_info; /* hmac���û�������־λ */
    uint32_t ul_rssi_last_timestamp;        /* ��ȡuser rssi����ʱ���, 1s������һ��rssi */
    oal_bool_enum_uint8 user_hisi;       /* ��ʶ�Զ��Ƿ�Ϊhisi 1103/1105оƬ */
    uint8_t resv[3];

    /* ��ǰVAP������APģʽ�������ֶ�Ϊuser��STAʱ�����ֶΣ�������ֶ���ע��!!! */
    uint32_t ul_assoc_req_ie_len;
    uint8_t *puc_assoc_req_ie_buff;
    oal_bool_enum_uint8 en_user_bw_limit; /* �����Ƿ������� */
    oal_bool_enum_uint8 en_wmm_switch;    /* ���û��Ƿ�wmm */
    oal_bool_enum_uint8 en_user_vendor_vht_capable;
    oal_bool_enum_uint8 en_user_vendor_novht_capable;
    oal_bool_enum_uint8 en_report_kernel;
    wlan_tx_amsdu_enum_uint8 en_amsdu_level; /* ����Э���max_mpdu_len ��ȡuser amsdu levelֵ */
    uint8_t lut_index;
    uint16_t aus_tid_baw_size[WLAN_TID_MAX_NUM]; /* ������TID��ص���Ϣ */

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    uint32_t aaul_txrx_data_stat[WLAN_WME_AC_BUTT][WLAN_TXRX_DATA_BUTT]; /* ����/���� tcp/udp be,bk,vi,vo���� */
#endif

#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_wapi_stru st_wapi;
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    hmac_hiex_sit_stru              st_hiex_sit;    // stream info table
    mac_hiex_ersru_ctrl_msg_stru    st_ersru;    /* extend range small resource unit */
#endif

    uint8_t uc_cfm_num;                                            /* �û��ѱ�ʶ��ҵ����� */
    uint8_t auc_resv2[1];
    /* δʶ���RTP���������ʶ��Ĵ��� �ڴﵽ����ǰ���ȿ���Ԥʶ�� �ӿ�ʶ����� ��ֹ�ײ�BE VI ���ȿ��ܵ��µ����� */
    uint16_t us_clear_judge_count;
    hmac_tx_cfm_flow_stru ast_cfm_flow_list[MAX_CONFIRMED_FLOW_NUM]; /* ��ʶ��ҵ�� */
    hmac_tx_judge_list_stru st_judge_list;                           /* ��ʶ�������� */

    // oal_spin_lock_stru          st_lock;

    hmac_user_btcoex_stru st_hmac_user_btcoex;

    /* ��ǰVAP������STAģʽ�������ֶ�Ϊuser��APʱ�����ֶΣ�������ֶ���ע��!!! */
    mac_user_stats_flag_stru st_user_stats_flag; /* ��user��staʱ��ָʾuser�Ƿ�ͳ�Ƶ���Ӧ�� */
    uint32_t ul_rx_pkt_drop;                   /* �������ݰ�host�౻drop�ļ��� */

    uint32_t ul_first_add_time; /* �û�����ʱ��ʱ�䣬����ͳ���û�����ʱ�� */

    /* dmac����ͬ����Ϣ */
    int8_t c_rssi;
    uint8_t uc_ps_st;
    int8_t c_free_power;
    uint8_t uc_tx_ba_limit;
#ifdef _PRE_WLAN_DFT_STAT
    uint8_t uc_cur_per;
    uint8_t uc_bestrate_per;
    uint8_t auc_resv4[2];
#endif
    uint32_t ul_tx_rate;
    uint32_t ul_tx_rate_min;
    uint32_t ul_tx_rate_max;
    uint32_t ul_rx_rate;
    uint32_t ul_rx_rate_min;
    uint32_t ul_rx_rate_max;
    uint32_t aul_drop_num[WLAN_WME_AC_BUTT];
    uint32_t aul_tx_delay[3];              /* 0 1 2�ֱ�Ϊ�������ʱ,��С������ʱ,ƽ��������ʱ */
    mac_ap_type_enum_uint16 en_user_ap_type; /* �û���AP���ͣ�����������ʹ�� */
    uint8_t auc_mimo_blacklist_mac[WLAN_MAC_ADDR_LEN];

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    hmac_user_11v_ctrl_stru st_11v_ctrl_info; /* 11v������Ϣ�ṹ�� */
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC
    mac_ts_stru st_ts_tmp_info[WLAN_WME_AC_BUTT]; /* ĳ��AC��TS�Ѿ������ɹ����ٴν���ʱ��ʱ�������� */
#endif

    hmac_tid_info_stru tx_tid_info[WLAN_TIDNO_BUTT];
    frw_timeout_stru tx_update_freq_adjust_timer;

    uint16_t    qos_last_seq_frag_num[WLAN_TID_MAX_NUM]; /* ���汾������յ���seq_numֵ */

    uint8_t closeHtFalg;
    uint8_t assoc_err_code; /* ȥ���� error code */
    uint8_t rsv[2]; /* 2Ԥ���ֽ��� */

    hmac_last_pn_stru  last_pn_info; /* ���汾������յ���pnֵ */

    /* ����������ܴ���HMAC USER�ṹ���ڵ����һ�� */
    mac_user_stru st_user_base_info; /* hmac user��dmac user�������� */

} hmac_user_stru;

typedef struct {
    hmac_user_stru *user;
} hmac_lut_index_tbl_stru;
extern hmac_lut_index_tbl_stru g_hmac_lut_index_tbl[];


/* �洢AP��������֡��ie��Ϣ�������ϱ��ں� */
typedef struct {
    uint32_t ul_assoc_req_ie_len;
    uint8_t *puc_assoc_req_ie_buff;
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN];
} hmac_asoc_user_req_ie_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_user_ht_support(hmac_user_stru *pst_hmac_user)
{
    return pst_hmac_user->st_user_base_info.st_ht_hdl.en_ht_capable;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_user_vht_support(hmac_user_stru *pst_hmac_user)
{
    return pst_hmac_user->st_user_base_info.st_vht_hdl.en_vht_capable;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_user_xht_support(hmac_user_stru *pst_hmac_user)
{
    return ((pst_hmac_user->st_user_base_info.en_cur_protocol_mode >= WLAN_HT_MODE) &&
            (pst_hmac_user->st_user_base_info.en_cur_protocol_mode < WLAN_PROTOCOL_BUTT));
}

extern hmac_lut_index_tbl_stru g_hmac_lut_index_tbl[HAL_MAX_TX_BA_LUT_SIZE];

/* 10 �������� */
uint32_t hmac_user_set_asoc_req_ie(hmac_user_stru *pst_hmac_user, uint8_t *puc_payload,
                                            uint32_t ul_payload_len, uint8_t uc_reass_flag);
uint32_t hmac_user_free_asoc_req_ie(uint16_t us_idx);
uint32_t hmac_user_alloc(uint16_t *pus_user_idx);
uint32_t hmac_user_free(uint16_t us_idx);
uint32_t hmac_user_init(hmac_user_stru *pst_hmac_user);
uint32_t hmac_user_set_avail_num_space_stream(mac_user_stru *pst_mac_user,
                                                           wlan_nss_enum_uint8 en_vap_nss);
void hmac_sdio_to_pcie_switch(mac_vap_stru *pst_mac_vap);
void hmac_user_set_num_spatial_stream_160M(mac_user_stru *pst_mac_user);
void hmac_dbdc_need_kick_user(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev);
uint32_t hmac_user_del(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user);
uint32_t hmac_user_add(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr,
                                    uint16_t *pus_user_index);
uint32_t hmac_user_add_multi_user(mac_vap_stru *pst_mac_vap, uint16_t *us_user_index);
uint32_t hmac_user_del_multi_user(mac_vap_stru *pst_mac_vap);
uint32_t hmac_user_add_notify_alg(mac_vap_stru *pst_mac_vap, uint16_t us_user_idx);
uint32_t hmac_update_user_last_active_time(mac_vap_stru *pst_mac_vap, uint8_t uc_len,
                                                    uint8_t *puc_param);
void hmac_tid_clear(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user);
hmac_user_stru *mac_res_get_hmac_user_alloc(uint16_t us_idx);
hmac_user_stru *mac_res_get_hmac_user(uint16_t us_idx);
hmac_user_stru *mac_vap_get_hmac_user_by_addr(mac_vap_stru *pst_mac_vap,
                                                         uint8_t *puc_mac_addr);
mac_ap_type_enum_uint16 hmac_compability_ap_tpye_identify(mac_vap_stru *pst_mac_vap,
                                                                     uint8_t *puc_mac_addr);
void hmac_user_set_amsdu_level_by_max_mpdu_len(hmac_user_stru *pst_hmac_user,
    uint16_t us_max_mpdu_len);
void hmac_user_set_amsdu_level(hmac_user_stru *pst_hmac_user,
    wlan_tx_amsdu_enum_uint8 en_amsdu_level);
#ifdef _PRE_WLAN_FEATURE_WAPI
hmac_wapi_stru *hmac_user_get_wapi_ptr(mac_vap_stru *pst_mac_vap,
                                                  oal_bool_enum_uint8 en_pairwise,
                                                  uint16_t us_pairwise_idx);
uint8_t hmac_user_is_wapi_connected(uint8_t uc_device_id);

#endif
void hmac_user_set_close_ht_flag(mac_vap_stru *p_mac_vap, hmac_user_stru *p_hmac_user);
void hmac_user_cap_update_by_hisi_cap_ie(hmac_user_stru *pst_hmac_user,
    uint8_t *puc_payload, uint32_t ul_msg_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_user.h */
