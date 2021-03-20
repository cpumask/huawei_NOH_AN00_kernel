

#ifndef __HMAC_EDCA_OPT_H__
#define __HMAC_EDCA_OPT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_common.h"
#include "oam_ext_if.h"
/* 2 �궨�� */
#define HMAC_EDCA_OPT_MIN_PKT_LEN 256 /* С�ڸó��ȵ�ip���Ĳ���ͳ�ƣ��ų�chariot���Ʊ��� */

#define HMAC_EDCA_OPT_TIME_MS 30000 /* edca��������Ĭ�϶�ʱ�� */

#define HMAC_EDCA_OPT_PKT_NUM ((HMAC_EDCA_OPT_TIME_MS) >> 3) /* ƽ��ÿ���뱨�ĸ��� */

#define WLAN_EDCA_OPT_MAX_WEIGHT_STA (3)
#define WLAN_EDCA_OPT_WEIGHT_STA     (2)

#define HMAC_IDLE_CYCLE_TH (5) /* ������������������ */
/* 3 ö�ٶ��� */
typedef enum {
    WLAN_TX_TCP_DATA = 0, /* ����TCP data */
    WLAN_RX_TCP_DATA = 1, /* ����TCP data */
    WLAN_TX_UDP_DATA = 2, /* ����UDP data */
    WLAN_RX_UDP_DATA = 3, /* ����UDP data */

    WLAN_TXRX_DATA_BUTT = 4,
} wlan_txrx_data_type_enum;
typedef uint8_t wlan_txrx_data_enum_uint8;

/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_edca_opt_rx_pkts_stat(uint16_t us_assoc_id, uint8_t uc_tidno,
                                               mac_ip_header_stru *pst_ip);
void hmac_edca_opt_tx_pkts_stat(mac_tx_ctl_stru *pst_tx_ctl, uint8_t uc_tidno,
                                               mac_ip_header_stru *pst_ip);
uint32_t hmac_edca_opt_timeout_fn(void *p_arg);

#endif /* end of _PRE_WLAN_FEATURE_EDCA_OPT_AP */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_edca_opt.h */
