

#ifndef __HMAC_ARP_OFFLOAD_H__
#define __HMAC_ARP_OFFLOAD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "oal_ext_if.h"
#include "mac_vap.h"

#include "hmac_vap.h"
#include "hmac_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ARP_OFFLOAD_H

uint32_t hmac_arp_offload_set_ip_addr(mac_vap_stru *mac_vap,
    dmac_ip_type_enum_uint8 type, dmac_ip_oper_enum_uint8 oper,
    void *ip_addr, void *mask_addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_arp_offload.h */
