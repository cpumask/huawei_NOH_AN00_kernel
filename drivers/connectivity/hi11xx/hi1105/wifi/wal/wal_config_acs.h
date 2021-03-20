

#ifndef __WAL_CONFIG_ACS_H__
#define __WAL_CONFIG_ACS_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "wlan_types.h"
#include "wlan_spec.h"
#include "mac_vap.h"
#include "hmac_ext_if.h"
#include "wal_ext_if.h"
#include "wal_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_CONFIG_ACS_H
/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t wal_acs_response_event_handler(frw_event_mem_stru *pst_event_mem);
uint32_t wal_acs_exit(void);
uint32_t wal_acs_init(void);
uint32_t wal_alloc_cfg_event(oal_net_device_stru *pst_net_dev, frw_event_mem_stru **ppst_event_mem,
                                          void *pst_resp_addr, wal_msg_stru **ppst_cfg_msg, uint16_t us_len);
#endif /* end of wal_config_acs.h */
