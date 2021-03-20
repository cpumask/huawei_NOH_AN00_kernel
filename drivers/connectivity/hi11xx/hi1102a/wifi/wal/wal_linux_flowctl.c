

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "oal_ext_if.h"
#include "oal_util.h"
#include "frw_ext_if.h"

#include "mac_vap.h"
#include "mac_device.h"
#include "mac_resource.h"

#include "hmac_ext_if.h"

#include "wal_main.h"
#include "wal_linux_flowctl.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_FLOWCTL_C

/* lint +e830 */ /* lint +e550 */
#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0))
oal_uint16 wal_netdev_select_queue(oal_net_device_stru *pst_dev, oal_netbuf_stru *pst_buf)
#else
oal_uint16 wal_netdev_select_queue(oal_net_device_stru *pst_dev, oal_netbuf_stru *pst_buf, void *accel_priv,
                                   select_queue_fallback_t fallback)
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(4,1,0)) */
{
    return oal_netbuf_select_queue(pst_buf);
}

#endif /* endif of _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL */


