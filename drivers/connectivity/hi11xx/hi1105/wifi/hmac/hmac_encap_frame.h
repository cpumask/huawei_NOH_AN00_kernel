

#ifndef __HMAC_ENCAP_FRAME_H__
#define __HMAC_ENCAP_FRAME_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "hmac_user.h"
#include "mac_vap.h"

/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint16_t hmac_mgmt_encap_deauth(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                             const unsigned char *puc_da, uint16_t us_err_code);
uint16_t hmac_mgmt_encap_disassoc(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                               uint8_t *puc_da, uint16_t us_err_code);
uint16_t hmac_encap_sa_query_req(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                              uint8_t *puc_da, uint16_t us_trans_id);
uint16_t hmac_encap_sa_query_rsp(mac_vap_stru *pst_mac_vap, uint8_t *pst_hdr,
                                              uint8_t *puc_data);
uint16_t hmac_encap_notify_chan_width(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                                   uint8_t *puc_da);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame.h */
