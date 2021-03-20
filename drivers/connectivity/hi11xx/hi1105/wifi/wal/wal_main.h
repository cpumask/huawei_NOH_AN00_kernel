

#ifndef __WAL_MAIN_H__
#define __WAL_MAIN_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_MAIN_H

/* 2 �궨�� */
#define WAL_EVENT_WID(_puc_macaddr, _uc_vap_id, en_event_type, output_data, data_len)

/* 3 ö�ٶ��� */
/* HOST CRX�¼������� */
typedef enum {
    WAL_HOST_CRX_SUBTYPE_CFG,
    WAL_HOST_CRX_SUBTYPE_RESET_HW,
    WAL_HOST_CRX_SUBTYPE_BUTT
} wal_host_crx_subtype_enum;
typedef uint8_t wal_host_crx_subtype_enum_uint8;

/* HOST DRX�¼������� */
typedef enum {
    WAL_HOST_DRX_SUBTYPE_TX,

    WAL_HOST_DRX_SUBTYPE_BUTT
} wal_host_drx_subtype_enum;
typedef uint8_t wal_host_drx_subtype_enum_uint8;

extern oam_wal_func_hook_stru g_st_wal_drv_func_hook;

/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
int32_t wal_main_init(void);
void wal_main_exit(void);
extern oal_wakelock_stru g_st_wal_wakelock;
#define wal_wake_lock()   oal_wake_lock(&g_st_wal_wakelock)
#define wal_wake_unlock() oal_wake_unlock(&g_st_wal_wakelock)

#endif /* end of wal_main */
