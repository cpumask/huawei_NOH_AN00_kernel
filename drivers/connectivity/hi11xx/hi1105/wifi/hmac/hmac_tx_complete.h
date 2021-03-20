

#ifndef __HMAC_TX_COMPLETE_H__
#define __HMAC_TX_COMPLETE_H__

#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_COMPLETE_H

#ifdef CONFIG_ARCH_KIRIN_PCIE

uint32_t hmac_tx_ba_complete_event_handler(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
oal_uint32 hmac_tx_complete_event_handler(frw_event_mem_stru *event_mem);

#endif
#endif

