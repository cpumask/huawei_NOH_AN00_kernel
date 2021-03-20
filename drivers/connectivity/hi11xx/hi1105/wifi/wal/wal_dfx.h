

#ifndef __WAL_DFX_H__
#define __WAL_DFX_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_types.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_DFX_H

/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
#ifdef  _PRE_CONFIG_HISI_S3S4_POWER_STATE
typedef struct {
    oal_net_device_stru *netdev[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    /* p2p cl��dev����һ��ҵ��vap,netdev�ĸ�������������ҵ��vap����3 */
    uint32_t netdev_num;
    oal_bool_enum_uint8 device_s3s4_process_flag;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_stru wifi_recovery_mutex;
#endif
} wal_info_recovery_stru;

extern wal_info_recovery_stru g_st_recovery_info;
#endif

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t wal_dfr_excp_rx(uint8_t uc_device_id, uint32_t ul_exception_type);
#ifdef _PRE_WLAN_FEATURE_DFR
void wal_dfr_init_param(void);
uint32_t  wal_dfx_init(void);
void    wal_dfx_exit(void);
void wal_dfr_custom_cali(void);
#endif


#endif /* end of wal_dfx.h */
