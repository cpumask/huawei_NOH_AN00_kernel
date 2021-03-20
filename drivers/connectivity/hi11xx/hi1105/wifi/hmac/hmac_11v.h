

#ifndef __HMAC_11V_H__
#define __HMAC_11V_H__

#include "oal_ext_if.h"
#include "oal_mem.h"
#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11V_H

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE

/* Ŀǰ����֡�����ڴ�800�ֽ� ֡�Դ���󳤶�19(frame boady)+50(url)+ N*(15(neighbor)+3(sub)+12(sub))
 * �����ڳ��� �޸���������ʱ��ע�� */
#define HMAC_MAX_BSS_NEIGHBOR_LIST 20 /* BSS Transition ֡�������͵�����ѡAP�б����� */

// 11v�ȴ�֡���صĳ�ʱʱ��
#define HMAC_11V_WAIT_STATUS_TIMEOUT     5000 // 5000ms
#define HMAC_11V_MAX_URL_LENGTH          50   /* Я��URL�ַ�����󳤶�����Ϊ50 */
#define HMAC_11V_TERMINATION_TSF_LENGTH  8    /* termination_tsfʱ�����ֽڳ��� */
#define HMAC_11V_QUERY_FRAME_BODY_FIX    4    /* query֡֡��̶�ͷ���� */
#define HMAC_11V_REQUEST_FRAME_BODY_FIX  7    /* query֡֡��̶�ͷ���� */
#define HMAC_11V_RESPONSE_FRAME_BODY_FIX 5    /* response֡֡��̶�ͷ���� */
#define HMAC_11V_PERFERMANCE_ELEMENT_LEN 1    /* perfermance ie length */
#define HMAC_11V_TERMINATION_ELEMENT_LEN 10   /* termination ie length */
#define HMAC_11V_TOKEN_MAX_VALUE         255  /* ֡������������ֵ */
#define HMAC_11V_SUBELEMENT_ID_RESV      0    /* SUBELEMENTԤ�� ID */

#define HMAC_11V_REQUEST_DISASSOC_TIME_SCAN_ONE_CHANNEL_TIME 100  /* ȥ����ʱ��С��100msֱ�ӻ�Reject */
#define HMAC_11V_REQUEST_DISASSOC_TIME_SCAN_ALL_CHANNEL_TIME 3000 /* 100ms��3s֮�䣬��ɨ��һ���ŵ� */

#ifdef _PRE_WLAN_FEATURE_MBO
#define HMAC_11V_MBO_RE_ASSOC_DALAY_TIME_S_TO_MS 1000
#endif

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* IEEE Std 802.11-2012 - Table 8-253 */
enum bss_trans_mgmt_status_code {
    WNM_BSS_TM_ACCEPT = 0,
    WNM_BSS_TM_REJECT_UNSPECIFIED = 1,
    WNM_BSS_TM_REJECT_INSUFFICIENT_BEACON = 2,
    WNM_BSS_TM_REJECT_INSUFFICIENT_CAPABITY = 3,
    WNM_BSS_TM_REJECT_UNDESIRED = 4,
    WNM_BSS_TM_REJECT_DELAY_REQUEST = 5,
    WNM_BSS_TM_REJECT_STA_CANDIDATE_LIST_PROVIDED = 6,
    WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES = 7,
    WNM_BSS_TM_REJECT_LEAVING_ESS = 8
};

/*****************************************************************************
  Neighbor Report Element����ϢԪ��(Subelement)�� ID
  820.11-2012Э��583ҳ��Table 8-115��SubElement IDs
*****************************************************************************/
typedef enum {
    HMAC_NEIGH_SUB_ID_RESERVED = 0,
    HMAC_NEIGH_SUB_ID_TFS_INFO,
    HMAC_NEIGH_SUB_ID_COND_COUNTRY,
    HMAC_NEIGH_SUB_ID_BSS_CANDIDATE_PERF,
    HMAC_NEIGH_SUB_ID_TERM_DURATION,
#ifdef _PRE_WLAN_FEATURE_MBO
    HMAC_NEIGH_SUB_ID_VENDOR_SPECIFIC = 221,
#endif
    HMAC_NEIGH_SUB_ID_BUTT
} hmac_neighbor_sub_eid_enum;
typedef uint8_t hmac_neighbor_sub_eid_enum_uint8;

/*****************************************************************************
  4 STRUCT����
*****************************************************************************/
/* Subelement BSS Transition Termination Duration */
struct hmac_bss_term_duration {
    uint8_t uc_sub_ie_id;                                         /* subelement ID����ID�ó�0 ��ʾ�����ڸ�Ԫ�� */
    uint8_t auc_termination_tsf[HMAC_11V_TERMINATION_TSF_LENGTH]; /* BSS��ֹʱ��: TSF */
    uint16_t us_duration_min;                                     /* BSS��ʧʱ�� time: ���� */
    uint8_t uc_resv;                                              /* ���ֽڶ����� */
} __OAL_DECLARE_PACKED;
typedef struct hmac_bss_term_duration hmac_bss_term_duration_stru;

/* ��ѡBSS�б���Neighbor Report IE�ṹ�� ����ֻ��Ҫ�õ�subelement 3 4 �ʶ�������subelement */
struct hmac_neighbor_bss_info {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];    /* BSSID MAC��ַ */
    uint8_t uc_opt_class;                       /* Operation Class */
    uint8_t uc_chl_num;                         /* Channel number */
    uint8_t uc_phy_type;                        /* PHY type */
    uint8_t uc_candidate_perf;                  /* perference data BSSIDƫ��ֵ */
    uint8_t uc_max_candidate_perf;              /* max perference data BSSIDƫ��ֵ */
    uint8_t valid;                             /* �Ƿ�����Ч��neighbor report */
    oal_bssid_infomation_stru st_bssid_info;      /* BSSID information */
    hmac_bss_term_duration_stru st_term_duration; /* ��Ԫ��Termination duration */
#ifdef _PRE_WLAN_FEATURE_MBO
    mac_assoc_retry_delay_attr_mbo_ie_stru st_assoc_delay_attr_mbo_ie; /* ��Ԫ��MBO IE(��Assoc retry delay Attr) */
#endif
} __OAL_DECLARE_PACKED;
typedef struct hmac_neighbor_bss_info hmac_neighbor_bss_info_stru;

struct hmac_bsst_req_mode {
    uint8_t bit_candidate_list_include : 1, /* �Ƿ�����ھ�AP�б� */
              bit_abridged : 1,                     /* 1:��ʾû�а������ھ��б��AP���ȼ�����Ϊ0 */
              bit_bss_disassoc_imminent : 1,        /* �Ƿ񼴽��Ͽ�STA ��0 ֡��ʱ����ȫΪFF FF */
              bit_termination_include : 1,          /* BSS�ս�ʱ��  ��0��֡���в�����BSS Termination Duration�ֶ� */
              bit_ess_disassoc_imminent : 1,        /* EES��ֹʱ�� */
              bit_rev : 3;
} __OAL_DECLARE_PACKED;
typedef struct hmac_bsst_req_mode hmac_bsst_req_mode_stru;

/* bss transition request֡����Ϣ�ṹ�� */
struct hmac_bsst_req_info {
    uint8_t uc_validity_interval;          /* �ھ��б���Чʱ�� TBTTs */
    uint16_t us_disassoc_time;             /* APȥ����STAʱ�� TBTTs */
    uint8_t *puc_session_url;              /* Ҫ�����ַ��� �������Ϊ100���ַ� */
    hmac_bsst_req_mode_stru st_request_mode; /* request mode */
    uint8_t uc_bss_list_num;
    uint8_t uc_resv;                                  /* 4�ֽڶ��� */
    hmac_neighbor_bss_info_stru *pst_neighbor_bss_list; /* bss list������ ���������50�� */
    hmac_bss_term_duration_stru st_term_duration;       /* ��Ԫ��Termination duration */
} __OAL_DECLARE_PACKED;
typedef struct hmac_bsst_req_info hmac_bsst_req_info_stru;

/* bss transition response֡����Ϣ�ṹ�� */
struct hmac_bsst_rsp_info {
    uint8_t uc_status_code;                         /* ״̬�� ���ջ��߾ܾ� */
    uint8_t uc_termination_delay;                   /* Ҫ��AP�Ӻ���ֹʱ��:���� */
    uint8_t auc_target_bss_addr[WLAN_MAC_ADDR_LEN]; /* �����л���Ŀ��BSSID */
    uint8_t uc_chl_num;                             /* Channel number */
    uint8_t uc_bss_list_num;                        /* bss list�������������Ϊ50�� */
    int8_t c_rssi;                                  /* ��ǰ����AP��RSSI����Candiate AP��RSSI */
    uint8_t us_resv;                                /* ���ֽڶ��� */
    hmac_neighbor_bss_info_stru *pst_neighbor_bss_list;
} __OAL_DECLARE_PACKED;
typedef struct hmac_bsst_rsp_info hmac_bsst_rsp_info_stru;


/* ��Neighbor Report IE�ṹ���֡�����н������� */
hmac_neighbor_bss_info_stru *hmac_get_neighbor_ie(uint8_t *puc_data,
                                                         uint16_t us_len, uint8_t *pst_bss_num);
/* STA ����AP���͵�bss transition request frame */
uint32_t hmac_rx_bsst_req_action(hmac_vap_stru *pst_hmac_vap,
                                          hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf);
/* STA ��װbss transition response frame */
uint16_t hmac_encap_bsst_rsp_action(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             hmac_bsst_rsp_info_stru *pst_bsst_rsp_info,
                                             oal_netbuf_stru *pst_buffer);
/* STA ����bss transition response frame */
uint32_t hmac_tx_bsst_rsp_action(void *pst_void1, void *pst_void2, void *pst_void3);

uint32_t hmac_11v_roam_scan_check(hmac_vap_stru *pst_hmac_vap);
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */

#endif /* end of hmac_11v.h */
