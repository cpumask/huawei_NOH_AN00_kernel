

#ifndef __WAL_MAIN_H__
#define __WAL_MAIN_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_MAIN_H

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define WAL_EVENT_WID(_puc_macaddr, _uc_vap_id, en_event_type, output_data, data_len)

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/* HOST CRX事件子类型 */
typedef enum {
    WAL_HOST_CRX_SUBTYPE_CFG,
    WAL_HOST_CRX_SUBTYPE_RESET_HW,

    WAL_HOST_CRX_SUBTYPE_BUTT
}wal_host_crx_subtype_enum;
typedef oal_uint8 wal_host_crx_subtype_enum_uint8;

/* HOST DRX事件子类型 */
typedef enum {
    WAL_HOST_DRX_SUBTYPE_TX,

    WAL_HOST_DRX_SUBTYPE_BUTT
}wal_host_drx_subtype_enum;
typedef oal_uint8 wal_host_drx_subtype_enum_uint8;

extern oam_wal_func_hook_stru     g_st_wal_drv_func_hook;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
extern oal_int32 wal_atcmsrv_ioctl_get_rx_pckg(oal_net_device_stru *pst_net_dev, oal_int32 *pl_rx_pckg_succ_num);
extern oal_int32  wal_main_init(oal_void);
extern oal_void  wal_main_exit(oal_void);
extern oal_wakelock_stru   g_st_wal_wakelock;
#define wal_wake_lock()  oal_wake_lock(&g_st_wal_wakelock)
#define wal_wake_unlock()  oal_wake_unlock(&g_st_wal_wakelock)
#endif /* end of wal_main */

