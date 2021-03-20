

#ifndef __PREPARE_FRAME_STA_H__
#define __PREPARE_FRAME_STA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_types.h"
#include "hmac_vap.h"

/* 2 �궨�� */
// for CSEC cycle complexity
#ifdef _PRE_WLAN_NARROW_BAND
#define HMAC_MGMT_ASOC_REQ_ENABLE_NB(_pst_scaned_bss, _pst_mac_vap) \
    (((_pst_scaned_bss)->st_bss_dscr_info.en_nb_capable == OAL_TRUE) && \
    ((_pst_mac_vap)->st_nb.en_open == OAL_TRUE) && ((_pst_mac_vap)->st_cap_flag.bit_nb == OAL_TRUE))
#endif

#define HMAC_MGMT_ASOC_REQ_ENABLE_VHT_CAPABLE(_pst_scaned_bss) \
    (((_pst_scaned_bss)->st_bss_dscr_info.en_vht_capable == OAL_TRUE) &&  \
    ((_pst_scaned_bss)->st_bss_dscr_info.en_vendor_vht_capable == OAL_FALSE))

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_mgmt_encap_asoc_req_sta(hmac_vap_stru *pst_hmac_sta,
                                                   uint8_t *puc_req_frame,
                                                   uint8_t *puc_curr_bssid);
uint16_t hmac_mgmt_encap_auth_req(hmac_vap_stru *pst_sta, uint8_t *puc_mgmt_frame);
uint16_t hmac_mgmt_encap_auth_req_seq3(hmac_vap_stru *pst_sta,
                                                    uint8_t *puc_mgmt_frame,
                                                    uint8_t *puc_mac_hrd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame_sta.h */
