

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
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
extern oal_uint32 hmac_mgmt_encap_asoc_req_sta(hmac_vap_stru *pst_hmac_sta,
                                               oal_uint8 *puc_req_frame,
                                               oal_uint8 *puc_curr_bssid);
extern oal_uint16 hmac_mgmt_encap_auth_req(hmac_vap_stru *pst_sta, oal_uint8 *puc_mgmt_frame);
extern oal_uint16 hmac_mgmt_encap_auth_req_seq3(hmac_vap_stru *pst_sta,
                                                oal_uint8 *puc_mgmt_frame,
                                                oal_uint8 *puc_mac_hrd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame_sta.h */