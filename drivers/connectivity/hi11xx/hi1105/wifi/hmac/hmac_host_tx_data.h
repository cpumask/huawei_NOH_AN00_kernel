

#ifndef __HMAC_HOST_TX_DATA_H__
#define __HMAC_HOST_TX_DATA_H__

#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HOST_TX_DATA_H

#ifdef CONFIG_ARCH_KIRIN_PCIE

uint32_t hmac_host_tx(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf);
uint32_t hmac_tx_update_freq_timeout(void *arg);

#endif
#endif
