

#ifndef __HAL_HOST_EXT_IF_H__
#define __HAL_HOST_EXT_IF_H__

#include "oal_ext_if.h"
#include "wlan_types.h"
#include "hal_common_ops.h"
#include "mac_common.h"

extern const struct hal_common_ops_stru *g_hal_common_ops;

int32_t hal_main_init(void);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

static inline void hal_host_rx_add_buff(hal_host_device_stru *hal_device, uint8_t en_queue_num)
{
    if (g_hal_common_ops->host_rx_add_buff) {
        return g_hal_common_ops->host_rx_add_buff(hal_device, en_queue_num);
    }
}

static inline uint32_t hal_host_rx_reset_smac_handler(frw_event_mem_stru *event_mem)
{
    if (g_hal_common_ops->host_rx_reset_smac_handler) {
        return g_hal_common_ops->host_rx_reset_smac_handler(event_mem);
    }
    return OAL_FAIL;
}

static inline void hal_host_chip_irq_init(void)
{
    if (g_hal_common_ops->host_chip_irq_init) {
        g_hal_common_ops->host_chip_irq_init();
    }
}

static inline uint8_t hal_host_get_rx_msdu_status(oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->host_get_rx_msdu_status) {
        return g_hal_common_ops->host_get_rx_msdu_status(netbuf);
    }
    return 0;
}

static inline void hal_host_rx_amsdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->host_rx_amsdu_list_build) {
        g_hal_common_ops->host_rx_amsdu_list_build(hal_device, netbuf);
    }
}

static inline oal_netbuf_stru *hal_rx_get_next_sub_msdu(hal_host_device_stru *hal_device,
    oal_netbuf_stru *hal_rx_msdu_dscr)
{
    if (g_hal_common_ops->rx_get_next_sub_msdu) {
        return g_hal_common_ops->rx_get_next_sub_msdu(hal_device, hal_rx_msdu_dscr);
    }
    return NULL;
}

static inline uint32_t hal_host_rx_get_msdu_info_dscr(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_info)
{
    if (g_hal_common_ops->host_rx_get_msdu_info_dscr) {
        return g_hal_common_ops->host_rx_get_msdu_info_dscr(netbuf, rx_info);
    }
    return OAL_FAIL;
}

static inline int32_t hal_rx_host_start_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    if (g_hal_common_ops->rx_host_start_dscr_queue) {
        return g_hal_common_ops->rx_host_start_dscr_queue(net_dev, hal_dev_id);
    }
    return OAL_FAIL;
}

static inline int32_t hal_rx_host_init_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    if (g_hal_common_ops->rx_host_init_dscr_queue) {
        return g_hal_common_ops->rx_host_init_dscr_queue(net_dev, hal_dev_id);
    }
    return OAL_FAIL;
}

static inline int32_t hal_rx_host_stop_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    if (g_hal_common_ops->rx_host_stop_dscr_queue) {
        return g_hal_common_ops->rx_host_stop_dscr_queue(net_dev, hal_dev_id);
    }
    return OAL_SUCC;
}
static inline void hal_rx_alloc_list_free(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list)
{
    if (g_hal_common_ops->rx_alloc_list_free) {
        g_hal_common_ops->rx_alloc_list_free(hal_dev, alloc_list);
    }
}
static inline void hal_rx_free_res(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->rx_free_res) {
        g_hal_common_ops->rx_free_res(hal_device);
    }
}
static inline void hal_tx_ba_info_dscr_get(uint8_t *ba_info, uint32_t size, hal_tx_ba_info_stru *tx_ba_info)
{
    if (g_hal_common_ops->tx_ba_info_dscr_get) {
        g_hal_common_ops->tx_ba_info_dscr_get(ba_info, size, tx_ba_info);
    }
}

static inline void hal_host_tx_intr_init(uint8_t hal_dev_id)
{
    if (g_hal_common_ops->host_tx_intr_init) {
        g_hal_common_ops->host_tx_intr_init(hal_dev_id);
    }
}

static inline void hal_host_al_rx_fcs_info(void)
{
    if (g_hal_common_ops->host_al_rx_fcs_info) {
        g_hal_common_ops->host_al_rx_fcs_info();
    }
}

#else
static inline void hal_host_tx_intr_init(uint8_t hal_dev_id)
{
}

static inline void hal_host_rx_add_buff(hal_host_device_stru *hal_device,
    uint8_t en_queue_num)
{
    return OAL_TRUE;
}

static inline void hal_host_chip_irq_init(void)
{
}

static inline uint8_t hal_host_get_rx_msdu_status(oal_netbuf_stru *netbuf)
{
    return OAL_TRUE;
}

static inline void hal_host_rx_amsdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
}

static inline oal_netbuf_stru *hal_rx_get_next_sub_msdu(hal_host_device_stru *hal_device,
    oal_netbuf_stru *hal_rx_msdu_dscr)
{
    return NULL;
}

static inline uint32_t hal_host_rx_get_msdu_info_dscr(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_info)
{
    return OAL_TRUE;
}
static inline int32_t hal_rx_host_start_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    return OAL_SUCC;
}

static inline int32_t hal_rx_host_init_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    return OAL_SUCC;
}

static inline int32_t hal_rx_host_stop_dscr_queue(oal_net_device_stru *net_dev, uint8_t hal_dev_id)
{
    return OAL_SUCC;
}

static inline void hal_rx_free_res(hal_host_device_stru *hal_device)
{
}
#endif
#endif
