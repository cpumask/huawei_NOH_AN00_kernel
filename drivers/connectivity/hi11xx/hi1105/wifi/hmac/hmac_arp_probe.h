

#ifndef __HMAC_ARP_PROBEH__
#define __HMAC_ARP_PROBEH__

/* 1 ����ͷ�ļ����� */
#include "frw_ext_if.h"
#include "oal_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ARP_PROBE_H

/* 2 �궨�� */
#define ARP_PROBE_FAIL_REASSOC_NUM 9 /* 9*300=2.7�� */
#define ARP_PROBE_TIMEOUT    300  // ms
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_arp_probe_type_set(void *p_arg, oal_bool_enum_uint8 en_arp_detect_on,
    uint8_t en_probe_type);
void hma_arp_probe_timer_start(oal_netbuf_stru *pst_netbuf, void *p_arg);
void hmac_arp_probe_init(void *p_hmac_vap, void *p_hmac_user);
void hmac_arp_probe_destory(void *p_hmac_vap, void *p_hmac_user);
uint32_t hmac_ps_rx_amsdu_arp_probe_process(mac_vap_stru *pst_mac_vap,
    uint8_t uc_len, uint8_t *puc_param);
oal_bool_enum_uint8 hmac_arp_probe_fail_reassoc_trigger(void *p_hmac_vap, void *p_hmac_user);

#endif /* end of __HMAC_M2S_H__ */

