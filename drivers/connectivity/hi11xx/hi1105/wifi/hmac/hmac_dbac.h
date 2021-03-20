

#ifndef __HMAC_DBAC_H__
#define __HMAC_DBAC_H__

/* 1 ����ͷ�ļ����� */
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DBAC_H
/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    frw_timeout_stru st_dbac_timer;
} hmac_dbac_handle_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_dbac_status_notify(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_go_flow_sta_channel_handle(mac_device_stru *mac_device);
#endif /* end of hmac_dbac.h */
