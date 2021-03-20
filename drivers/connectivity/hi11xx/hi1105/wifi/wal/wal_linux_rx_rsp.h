

#ifndef __WAL_LINUX_RX_RSP_H__
#define __WAL_LINUX_RX_RSP_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_types.h"
#include "wal_ext_if.h"
#include "frw_ext_if.h"
#include "hmac_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_RX_RSP_H
/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* ����sta�ϱ��ں˵�ɨ���� */
typedef struct {
    int32_t l_signal; /* �ź�ǿ�� */

    int16_t s_freq; /* bss�����ŵ�������Ƶ�� */
    uint8_t auc_arry[2];

    uint32_t ul_mgmt_len;            /* ����֡���� */
    oal_ieee80211_mgmt_stru *pst_mgmt; /* ����֡��ʼ��ַ */

} wal_scanned_bss_info_stru;

/* ����sta�ϱ��ں˵Ĺ������ */
typedef struct {
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN]; /* sta������ap mac��ַ */
    uint16_t us_status_code;              /* ieeeЭ��涨��16λ״̬�� */

    uint8_t *puc_rsp_ie; /* asoc_req_ie  */
    uint8_t *puc_req_ie;

    uint32_t ul_req_ie_len; /* asoc_req_ie len */
    uint32_t ul_rsp_ie_len;

    uint16_t us_connect_status;
    uint8_t auc_resv[2];

} oal_connet_result_stru;

/* ����sta�ϱ��ں˵�ȥ������� */
typedef struct {
    uint16_t us_reason_code; /* ȥ���� reason code */
    uint8_t auc_resv[2];

    uint8_t *pus_disconn_ie;    /* ȥ��������֡ ie */
    uint32_t us_disconn_ie_len; /* ȥ��������֡ ie ���� */
} oal_disconnect_result_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t wal_scan_comp_proc_sta(frw_event_mem_stru *pst_event_mem);
uint32_t wal_asoc_comp_proc_sta(frw_event_mem_stru *pst_event_mem);
uint32_t wal_disasoc_comp_proc_sta(frw_event_mem_stru *pst_event_mem);
uint32_t wal_connect_new_sta_proc_ap(frw_event_mem_stru *pst_event_mem);
uint32_t wal_disconnect_sta_proc_ap(frw_event_mem_stru *pst_event_mem);
uint32_t wal_mic_failure_proc(frw_event_mem_stru *pst_event_mem);
uint32_t wal_send_mgmt_to_host(frw_event_mem_stru *pst_event_mem);
uint32_t wal_p2p_listen_timeout(frw_event_mem_stru *pst_event_mem);

uint32_t wal_report_external_auth_req(frw_event_mem_stru *pst_event_mem);

uint32_t wal_report_channel_switch(frw_event_mem_stru *pst_event_mem);
uint32_t wal_nan_response_event_process(frw_event_mem_stru *event_mem);
#endif /* end of wal_linux_rx_rsp.h */
