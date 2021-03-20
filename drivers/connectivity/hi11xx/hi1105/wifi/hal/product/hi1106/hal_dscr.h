

#ifndef __HAL_DSCR_H__
#define __HAL_DSCR_H__

#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"
#include "hal_device.h"
#include "oam_ext_if.h"
#include "mac_device.h"
#include "mac_user.h"
#include "securec.h"
#include "securectype.h"
#include "oal_pcie_linux.h"

#define HAL_RX_MSDU_NORMAL_RING_NUM0 256
#define HAL_RX_MSDU_NORMAL_RING_NUM1 1024
#define LARGE_NETBUF_SIZE            1580
#define SMALL_NETBUF_SIZE            1500
#define ALRX_NETBUF_SIZE             4112

#define HAL_RX_MSDU_SMALL_RING_NUM0 512
#define HAL_RX_MSDU_SMALL_RING_NUM1 1024
#define HAL_HOST_MONITOR_COUNT_2G   128
#define HAL_HOST_MONITOR_COUNT_5G   256
#define HAL_RX_ENTRY_SIZE             8
#define HAL_RX_DSCR_LEN              48


/* 为了防止rx complete ring overrun, complete ring的深度设置为最大值 */
#define HAL_RX_COMPLETE_RING_MAX 1024

#define HAL_BA_INFO_COUNT  64

typedef struct {
    uint32_t bit_user_id     : 12,
             bit_tid         : 4,
             bit_tx_ba_ssn   : 12,
             bit_resv        : 3,
             bit_ba_info_vld : 1;
    uint32_t ul_reserved;
    uint32_t ba_bitmap[WLAN_TID_MAX_NUM];
} hi1106_tx_ba_info_dscr_stru;

#define BA_INFO_DATA_SIZE OAL_SIZEOF(hi1106_tx_ba_info_dscr_stru)

/* 接收描述符定义 */
typedef struct tag_hi1106_rx_mpdu_desc_stru {
    /* word0~1 */
    uint32_t  ul_ppdu_host_buf_addr_lsb;
    uint32_t  ul_ppdu_host_buf_addr_msb;

    /* word2 */
    uint32_t  bit_start_seqnum            :   12;
    uint32_t  bit_mcast_bcast             :   1;
    uint32_t  bit_eosp                    :   1;
    uint32_t  bit_is_amsdu                :   1;
    uint32_t  bit_first_sub_msdu          :   1;
    uint32_t  bit_sub_msdu_num            :   8;
    uint32_t  bit_is_ampdu                :   1;
    uint32_t  bit_ba_session_vld          :   1;
    uint32_t  bit_bar_flag                :   1;
    uint32_t  bit_reserved                :   1;
    uint32_t  bit_rx_status               :   4;

    /* word3 */
    uint32_t  bit_process_flag            :   3;
    uint32_t  bit_reserved1               :   1;
    uint32_t  bit_release_start_seqnum    :   12;
    uint32_t  bit_release_end_seqnum      :   12;
    uint32_t  bit_reserved2               :   3;
    uint32_t  bit_release_is_valid        :   1;

    /* word4 */
    uint32_t  ul_pn_lsb;

    /* word5 */
    uint32_t  bit_pn_msb                  :   16;
    uint32_t  bit_frame_control           :   16;

    /* word6 */
    uint32_t  bit_user_id                 :   12;
    uint32_t  bit_band_id                 :   2;
    uint32_t  bit_reserved3               :   2;
    uint32_t  bit_vap_id                  :   8;
    uint32_t  bit_tid                     :   4;
    uint32_t  bit_cipher_type             :   4;

    /* word7 */
    uint32_t  bit_fragment_num            :   4;
    uint32_t  bit_sequence_num            :   12;
    uint32_t  bit_packet_len              :   16;

    /* word8 */
    uint32_t  bit_dst_user_id             :   12;
    uint32_t  bit_dst_band_id             :   2;
    uint32_t  bit_ptlcs_valid             :   1;
    uint32_t  bit_ptlcs_pass              :   1;
    uint32_t  bit_dst_vap_id              :   8;
    uint32_t  bit_frame_format            :   4;
    uint32_t  bit_ipv4cs_valid            :   1;
    uint32_t  bit_ipv4cs_pass             :   1;
    uint32_t  bit_reserved6               :   1;
    uint32_t  bit_dst_is_valid            :   1;

    /* word9 */
    uint32_t  bit_ptlcs_val               :   16;
    uint32_t  bit_ipv4cs_val              :   16;

    /* word 10~11 */
    uint32_t  next_sub_msdu_addr_lsb;
    uint32_t  next_sub_msdu_addr_msb;
} hi1106_rx_mpdu_desc_stru;

int32_t hi1106_rx_host_start_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id);
int32_t hi1106_rx_host_stop_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id);
void hi1106_tx_ba_info_dscr_get(uint8_t *ba_info_data, uint32_t size, hal_tx_ba_info_stru *tx_ba_info);
void hi1106_host_tx_intr_init(uint8_t hal_dev_id);
int32_t hi1106_rx_host_init_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id);
void hi1106_host_rx_add_buff(hal_host_device_stru *hal_device, uint8_t en_queue_num);
uint32_t hi1106_rx_mpdu_que_len(hal_host_device_stru *pst_device);
int32_t hi1106_rx_get_node_idx(hal_rx_nodes *nodes, dma_addr_t dma_addr);
oal_netbuf_stru *hi1106_rx_get_node(hal_rx_nodes *nodes, uint32_t idx);
oal_netbuf_stru *hi1106_rx_get_next_sub_msdu(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
uint32_t hi1106_host_rx_reset_smac_handler(frw_event_mem_stru *event_mem);
void hi1106_host_rx_msdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
#endif

