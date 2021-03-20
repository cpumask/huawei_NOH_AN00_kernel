

#ifndef __WAL_DFX_H__
#define __WAL_DFX_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "oal_types.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_DFX_H

/* 2 宏定义 */
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
#ifdef  _PRE_CONFIG_HISI_S3S4_POWER_STATE
typedef struct {
    oal_net_device_stru *netdev[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    /* p2p cl和dev共用一个业务vap,netdev的个数不会大于最大业务vap个数3 */
    uint32_t netdev_num;
    oal_bool_enum_uint8 device_s3s4_process_flag;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_stru wifi_recovery_mutex;
#endif
} wal_info_recovery_stru;

extern wal_info_recovery_stru g_st_recovery_info;
#endif

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t wal_dfr_excp_rx(uint8_t uc_device_id, uint32_t ul_exception_type);
#ifdef _PRE_WLAN_FEATURE_DFR
void wal_dfr_init_param(void);
uint32_t  wal_dfx_init(void);
void    wal_dfx_exit(void);
void wal_dfr_custom_cali(void);
#endif


#endif /* end of wal_dfx.h */
