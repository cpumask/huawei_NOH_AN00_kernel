

#ifndef __WAL_LINUX_VENDOR_H__
#define __WAL_LINUX_VENDOR_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_VENDOR_H

/* wpa_supplicant 下发命令 */
typedef struct wal_wifi_priv_cmd {
    uint32_t ul_total_len;
    uint32_t ul_used_len;
    uint8_t *puc_buf;
} wal_wifi_priv_cmd_stru;

uint32_t wal_vendor_cmd_gather_handler(oal_net_device_stru *pst_net_dev, int8_t *pc_command);
uint8_t wal_vendor_cmd_gather(int8_t *pc_command);
int32_t wal_vendor_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, int32_t ul_cmd);
uint32_t wal_hipriv_hid2d_scan_channel(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_hid2d_switch_channel(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
int32_t wal_netdev_stop(oal_net_device_stru *pst_net_dev);

#endif
