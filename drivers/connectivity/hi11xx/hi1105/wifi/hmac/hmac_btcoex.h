

#ifndef __HMAC_BTCOEX_H__
#define __HMAC_BTCOEX_H__
/* 1 ����ͷ�ļ����� */
#include "frw_ext_if.h"
#include "oal_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BTCOEX_H

/* 2 �궨�� */
#define BTCOEX_BSS_NUM_IN_BLACKLIST 16
#define BTCOEX_ARP_FAIL_DELBA_NUM   2
#define BTCOEX_ARP_FAIL_REASSOC_NUM 4
#define BTCOEX_ARP_PROBE_TIMEOUT    (300)  // ms

/* 3 ö�ٶ��� */
typedef enum {
    BTCOEX_BLACKLIST_TPYE_FIX_BASIZE = 0,
    BTCOEX_BLACKLIST_TPYE_NOT_AGGR = 1,

    BTCOEX_BLACKLIST_TPYE_BUTT
} btcoex_blacklist_type_enum;
typedef uint8_t btcoex_blacklist_type_enum_uint8;
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    uint16_t us_last_baw_start;                       /* ��һ�ν��յ�ADDBA REQ�е�baw_startֵ */
    uint16_t us_last_seq_num;                         /* ��һ�ν��յ�ADDBA REQ�е�seq_numֵ */
    btcoex_blacklist_type_enum_uint8 en_blacklist_tpye; /* ���������� */
    oal_bool_enum_uint8 en_ba_handle_allow;          /* ����������1:�Ƿ�������ɾ���ۺ� ����������2:�Ƿ��������ۺ� */
} hmac_btcoex_addba_req_stru;

typedef struct {
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN]; /* ������MAC��ַ */
    uint8_t uc_type;                              /* д������������� */
    uint8_t uc_used;                              /* �Ƿ��Ѿ�д��������MAC��ַ */
} hmac_btcoex_delba_exception_stru;
typedef struct {
    hmac_btcoex_delba_exception_stru btcoex_delba_exception[BTCOEX_BSS_NUM_IN_BLACKLIST];
    uint8_t uc_exception_bss_index; /* ������MAC��ַ�������±� */
    uint8_t auc_resv[3]; /* 4�ֽڶ��벹3�ֽ� */
} hmac_device_btcoex_stru;

typedef struct {
    hmac_btcoex_addba_req_stru st_hmac_btcoex_addba_req;
    uint16_t us_ba_size;
    oal_bool_enum_uint8 en_delba_btcoex_trigger; /* �Ƿ�btcoex����ɾ��BA */
    uint8_t res;
} hmac_user_btcoex_stru;

#define HMAC_BTCOEX_GET_BLACKLIST_TYPE(_pst_hmac_user) \
    ((_pst_hmac_user)->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_blacklist_tpye)

#define HMAC_BTCOEX_GET_BLACKLIST_DELBA_HANDLE_ALLOW(_pst_hmac_user) \
    ((_pst_hmac_user)->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow)

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_btcoex_check_rx_same_baw_start_from_addba_req(void *p_arg,
                                                                       void *p_arg1,
                                                                       mac_ieee80211_frame_stru *pst_frame_hdr,
                                                                       uint8_t *puc_action);
uint32_t hmac_btcoex_check_exception_in_list(void *p_arg, uint8_t *auc_addr);
void hmac_btcoex_blacklist_handle_init(void *p_arg);
uint8_t hmac_btcoex_check_addba_req(void *vap, void *user);
void hmac_btcoex_set_addba_rsp_ba_param(
    void *vap, void *user, void *ba_rx_info, uint16_t *ba_param);
#endif /* end of __HMAC_BTCOEX_H__ */

