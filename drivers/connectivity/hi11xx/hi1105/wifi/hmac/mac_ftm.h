

#ifndef __MAC_FTM_H__
#define __MAC_FTM_H__

#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAL_FILE_ID_MAC_FTM_H

extern uint8_t g_mac_ftm_cap; /* FTM¶¨ÖÆ»¯ */

typedef enum {
    MAC_FTM_DISABLE_MODE = 0,
    MAC_FTM_RESPONDER_MODE = 1,
    MAC_FTM_INITIATOR_MODE = 2,
    MAC_FTM_MIX_MODE = 3,
    MAC_FTM_MODE_BUTT,
} mac_ftm_mode_enum;

typedef enum {
    FTM_FORMAT_BANDWIDTH_HTMIXED_20 = 9,
    FTM_FORMAT_BANDWIDTH_VHT20,
    FTM_FORMAT_BANDWIDTH_HTMIXED_40,
    FTM_FORMAT_BANDWIDTH_VHT_40,
    FTM_FORMAT_BANDWIDTH_VHT_80 = 13,
} mac_ftm_para_fomat_and_bandwidth_field_enum;

uint8_t mac_is_ftm_enable(mac_vap_stru *mac_vap);
void mac_ftm_mib_init(mac_vap_stru *mac_vap);
void mac_ftm_add_to_ext_capabilities_ie(mac_vap_stru *mac_vap, oal_uint8 *buffer, oal_uint8 *ie_len);
#endif /* _PRE_WLAN_FEATURE_FTM */
#endif /* __MAC_FTM_H__ */
