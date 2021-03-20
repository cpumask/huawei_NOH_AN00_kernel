

#ifndef __HAL_COMMOM_OPS_H__
#define __HAL_COMMOM_OPS_H__

#include "oal_net.h"
#include "hal_device.h"
#include "hal_dscr.h"
#include "hal_common.h"
#include "mac_common.h"


typedef void (*p_host_board_init)(oal_void);
typedef void (*p_host_rx_add_buff)(hal_host_device_stru *hal_device, uint8_t en_queue_num);
typedef void (*p_host_chip_irq_init)(void);
typedef uint8_t (*p_host_get_rx_msdu_status)(oal_netbuf_stru *netbuf);
typedef void (*p_host_rx_amsdu_list_build)(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
typedef oal_netbuf_stru *(*p_rx_get_next_sub_msdu)(hal_host_device_stru *hal_device, oal_netbuf_stru *rx_msdu_dscr);
typedef uint32_t (*p_host_rx_get_msdu_info_dscr)(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_info);
typedef int32_t (*p_rx_host_start_dscr_queue)(oal_net_device_stru *net_dev, uint8_t hal_dev_id);
typedef int32_t (*p_rx_host_init_dscr_queue)(oal_net_device_stru *net_dev, uint8_t hal_dev_id);
typedef int32_t (*p_rx_host_stop_dscr_queue)(oal_net_device_stru *net_dev, uint8_t hal_dev_id);
typedef void (*p_rx_free_res)(hal_host_device_stru *hal_device);
typedef void (*p_tx_ba_info_dscr_get)(uint8_t *ba_info, uint32_t size, hal_tx_ba_info_stru *tx_ba_info);
typedef void (*p_host_tx_intr_init)(uint8_t hal_dev_id);
typedef uint32_t (*p_host_rx_reset_smac_handler)(frw_event_mem_stru *event_mem);
typedef void (*p_rx_alloc_list_free)(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list);
typedef void (*p_host_al_rx_fcs_info)(void);

struct hal_common_ops_stru {
    p_host_chip_irq_init        host_chip_irq_init;
    p_host_rx_add_buff          host_rx_add_buff;
    p_host_get_rx_msdu_status   host_get_rx_msdu_status;
    p_host_rx_amsdu_list_build  host_rx_amsdu_list_build;
    p_rx_get_next_sub_msdu      rx_get_next_sub_msdu;
    p_host_rx_get_msdu_info_dscr host_rx_get_msdu_info_dscr;
    p_rx_host_start_dscr_queue  rx_host_start_dscr_queue;
    p_rx_host_init_dscr_queue   rx_host_init_dscr_queue;
    p_rx_host_stop_dscr_queue   rx_host_stop_dscr_queue;
    p_rx_free_res               rx_free_res;
    p_tx_ba_info_dscr_get       tx_ba_info_dscr_get;
    p_host_tx_intr_init         host_tx_intr_init;
    p_host_rx_reset_smac_handler host_rx_reset_smac_handler;
    p_rx_alloc_list_free        rx_alloc_list_free;
    p_host_al_rx_fcs_info       host_al_rx_fcs_info;
};

typedef struct {
    hal_host_device_stru        *hal_dev;
    oal_netbuf_head_stru         netbuf_head;
} hal_host_rx_event;

#endif
