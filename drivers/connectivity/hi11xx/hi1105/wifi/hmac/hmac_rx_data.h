

#ifndef __HMAC_RX_DATA_H__
#define __HMAC_RX_DATA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_frame.h"

#include "hmac_user.h"
#include "oal_net.h"
#include "hmac_tcp_opt.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_resource.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DATA_H
/* 2 �궨�� */
#define MAC_TID_IS_VALID(tid)         (((uint16_t)tid) < WLAN_TID_MAX_NUM)
#define HMAC_RX_DATA_ETHER_OFFSET_LENGTH 6      /* ������lan�İ���protocol type����̫ͷ��Ҫ��ǰƫ��6 */
#define HMAC_RX_SMALL_PKT_MIPS_LIMIT     30000L /* rxС��MIPS���ޣ���ǰΪ30us */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* HMAC ���� TKIP MIC FAILE �쳣���ݸ���ʱ���Ĳ��� */
typedef struct {
    uint8_t uc_vap_id; /* ���ݸ���ʱ���� vap_id */
    uint8_t uc_rsv[3];
    frw_timeout_stru *pst_timeout; /* MIC faile ����ʱ����Դ */
} hmac_drx_mic_faile_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_rx_process_data_event(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_rx_process_ring_data_event(frw_event_mem_stru *pst_event_mem);
void hmac_rx_process_data_ap_tcp_ack_opt(hmac_vap_stru *pst_vap,
    oal_netbuf_head_stru *pst_netbuf_header);
uint32_t hmac_monitor_report_frame_to_sdt(mac_rx_ctl_stru *pst_cb_ctrl, oal_netbuf_stru *pst_curr_netbuf);
uint32_t hmac_rx_process_data_sta_tcp_ack_opt(hmac_vap_stru *pst_vap,
    oal_netbuf_head_stru *pst_netbuf_header);
#ifdef _PRE_WLAN_DFT_DUMP_FRAME
void hmac_rx_report_eth_frame(mac_vap_stru *pst_mac_vap,
    oal_netbuf_stru *pst_netbuf);
#endif
#ifdef _PRE_WLAN_FEATURE_MONITOR
void hmac_sniffer_save_data(hmac_device_stru *hmac_device,
    oal_netbuf_stru *pst_netbuf, uint16_t us_netbuf_num);
#endif
void hmac_rx_free_netbuf(oal_netbuf_stru *pst_netbuf, uint16_t us_nums);
void hmac_rx_free_netbuf_list(oal_netbuf_head_stru *pst_netbuf_hdr, uint16_t us_nums_buf);
void hmac_rx_lan_frame(oal_netbuf_head_stru *pst_netbuf_header);

void hmac_rx_mpdu_to_netbuf_list(oal_netbuf_head_stru *netbuf_head, oal_netbuf_stru *netbuf_mpdu);
uint32_t hmac_rx_host_ring_data_event(frw_event_mem_stru *event_mem);
void hmac_rx_data_sta_proc(oal_netbuf_head_stru *netbuf_header);
void hmac_rx_data_ap_proc(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *p_netbuf_head);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_rx_data.h */
