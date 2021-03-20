

#ifndef __HMAC_TX_MSDU_DSCR_H__
#define __HMAC_TX_MSDU_DSCR_H__

/* 1 其他头文件包含 */
#include "hmac_user.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DRIVER_HMAC_TX_MSDU_DSCR_H

#define TX_RING_MSDU_INFO_LEN 12
#define TX_HW_MSDU_DSCR_LEN   24
#define GET_LOW_32_BITS(a)  ((uint32_t)(((uint64_t)(a)) & 0x00000000FFFFFFFFUL))
#define GET_HIGH_32_BITS(a) ((uint32_t)((((uint64_t)(a)) & 0xFFFFFFFF00000000UL) >> 32UL))
#define HOST_AL_TX_FLAG   0x5a


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_get_tx_ring_enable(hmac_user_stru *hmac_user, uint8_t tid)
{
    return hmac_user->tx_tid_info[tid].non_ba_tx_ring.enabled;
}

/* 10 函数声明 */
void hmac_tx_ring_release_msdu(hmac_user_stru *hmac_user, uint8_t tid);
uint32_t hmac_tx_ring_push_msdu(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf);
uint32_t hmac_tx_ring_init(hmac_user_stru *hmac_user, uint8_t tid, uint8_t ring_size);
void hmac_user_tx_ring_deinit(hmac_user_stru *hmac_user);
uint32_t hmac_set_tx_ring_device_base_addr(frw_event_mem_stru *frw_mem);
uint32_t hmac_tx_hostva_to_devva(uint8_t *hostva, uint32_t alloc_size, uint64_t *devva);
uint8_t *hmac_tx_netbuf_init_msdu_dscr(oal_netbuf_stru *netbuf);
uint32_t hmac_tx_alloc_ring(hmac_msdu_info_ring_stru *tx_ring);
void hmac_tx_host_ring_release(hmac_msdu_info_ring_stru *tx_ring);

#endif
