

#ifndef __HMAC_SMPS_H__
#define __HMAC_SMPS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "hmac_main.h"
#include "oam_ext_if.h"
#include "mac_resource.h"

#include "mac_device.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "mac_user.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SMPS_H
/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_mgmt_rx_smps_frame(mac_vap_stru *pst_mac_vap,
                                          hmac_user_stru *pst_hmac_user,
                                          uint8_t *puc_data);
uint32_t hmac_smps_update_user_status(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user);

#endif /* end of _PRE_WLAN_FEATURE_SMPS */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_smps.h */
