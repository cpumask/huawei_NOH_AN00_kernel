

#ifndef __OAL_MAIN_H__
#define __OAL_MAIN_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_types.h"

/* �궨�� */
#define WLAN_INIT_DEVICE_RADIO_CAP 3 /* WLANʹ��2G_5G */
#define NARROW_BAND_ON_MASK        0x1
#define NBFH_ON_MASK               0x2

/* ȫ�ֱ������� */
extern oal_uint8 wlan_service_device_per_chip[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP];
extern oal_uint8 hitalk_status; /* ��¼��ǰ�̼����� */
extern struct genl_family *nl80211_fam;
extern struct genl_multicast_group *nl80211_mlme_mcgrp;

/* �������� */
extern oal_int32 oal_main_init(oal_void);
extern oal_void oal_main_exit(oal_void);
extern oal_uint8 oal_chip_get_device_num(oal_uint32 ul_chip_ver);
extern oal_uint8 oal_board_get_service_vap_start_id(oal_void);

#endif /* end of oal_main */
