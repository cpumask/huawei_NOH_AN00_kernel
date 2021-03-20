
#ifndef __HMAC_HT_SELF_CURE_C_
#define __HMAC_HT_SELF_CURE_C_

#include "hmac_ext_if.h"
#include "mac_data.h"
#include "hmac_resource.h"
#include "hmac_ht_self_cure.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "hmac_fsm.h"
#include "mac_user.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HT_SELF_CURE_C

#define HT_SELF_CURE_CLOSE_TX_DHCP_MAX_TIMES 2

OAL_STATIC hmac_ht_self_cure_stru g_st_ht_self_cure_info;

OAL_STATIC void hmac_ht_self_cure_checkinfo_init(void)
{
    hmac_ht_self_cure_stru *ht_self_cure = NULL;

    ht_self_cure = &g_st_ht_self_cure_info;
    memset_s(&ht_self_cure->htCheckInfo, sizeof(hmac_ht_blacklist_checkinfo_stru), 0,
        sizeof(hmac_ht_blacklist_checkinfo_stru));
}

oal_bool_enum_uint8 hmac_ht_self_cure_is_htassoc_user(uint8_t *UserMac)
{
    hmac_ht_self_cure_stru *ht_self_cure = NULL;

    ht_self_cure = &g_st_ht_self_cure_info;

    if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.htAssocUser, UserMac, WLAN_MAC_ADDR_LEN)) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

OAL_STATIC void hmac_ht_self_cure_state_change(uint8_t newChange)
{
    hmac_ht_self_cure_stru *ht_self_cure = OAL_PTR_NULL;
    ht_self_cure = &g_st_ht_self_cure_info;

    oam_warning_log2(0, OAM_SF_SCAN, "hmac_ht_self_cure_state_change::state change from %d to %d",
        ht_self_cure->htCheckInfo.htSelfCureCheckState, newChange);
    ht_self_cure->htCheckInfo.htSelfCureCheckState = newChange;
}

OAL_STATIC void hmac_ht_self_cure_add_blacklist(uint8_t *userMac)
{
    uint8_t loop;
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;
    hmac_ht_blacklist_stru *blacklist = NULL;

    if (ht_self_cure->blackListNum == HMAC_HT_BLACKLIST_MAX_NUM) {
        memset_s(ht_self_cure->htBlackList, sizeof(hmac_ht_blacklist_stru) * HMAC_HT_BLACKLIST_MAX_NUM, 0,
            sizeof(hmac_ht_blacklist_stru) * HMAC_HT_BLACKLIST_MAX_NUM);
        ht_self_cure->blackListNum = 0;
    }

    for (loop = 0; loop < ht_self_cure->blackListNum; loop++) {
        blacklist = &ht_self_cure->htBlackList[loop];
        if (!OAL_MEMCMP(blacklist->userMacAddr, userMac, WLAN_MAC_ADDR_LEN)) {
            return;
        }
    }
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
        CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_HT_SELF_CURE_OCCUR, CHR_WIFI_HT_SELF_CURE_ASSOC);

    memcpy_s(ht_self_cure->htBlackList[ht_self_cure->blackListNum].userMacAddr, WLAN_MAC_ADDR_LEN,
        userMac, WLAN_MAC_ADDR_LEN);
    ht_self_cure->blackListNum++;

    oam_error_log4(0, OAM_SF_SCAN, "hmac_ht_self_cure_add_blacklist::user_mac_%d:%02X:XX:XX:XX:%02X:%02X",
        ht_self_cure->blackListNum, userMac[0], /* 0 mac地址第0个字节 */
        userMac[4],  /* 4 mac地址第4个字节 */
        userMac[5]); /* 5 mac地址第5个字节 */
}

OAL_STATIC void hmac_ht_self_cure_state_init_assoc_event_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    if (mac_vap->en_protocol == WLAN_HT_MODE) {
        memcpy_s(ht_self_cure->htCheckInfo.htAssocUser, WLAN_MAC_ADDR_LEN, userMac, WLAN_MAC_ADDR_LEN);
        hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_ASSOC);
        ht_self_cure->htCheckInfo.txDhcpTimes = 0;
        if (mac_vap->st_channel.en_band == WLAN_BAND_2G && MAC_IS_SMC_AP(userMac)) {
            memcpy_s(ht_self_cure->htCheckInfo.checkHtUser, WLAN_MAC_ADDR_LEN, userMac, WLAN_MAC_ADDR_LEN);
        }
    } else {
        hmac_ht_self_cure_checkinfo_init();
    }
}

OAL_STATIC void hmac_ht_self_cure_state_init_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_ASSOC:
            hmac_ht_self_cure_state_init_assoc_event_handle(mac_vap, userMac, eventId);
            break;
        default:
            break;
    }
}

OAL_STATIC void hmac_ht_self_cure_state_assoc_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_TX_DHCP_FRAME:
            ht_self_cure->htCheckInfo.txDhcpTimes++;
            hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_HT_TX_DHCP);
            break;
        case HAMC_HT_SELF_CURE_EVENT_RX_EAPOL:
            if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                memset_s(ht_self_cure->htCheckInfo.checkHtUser, WLAN_MAC_ADDR_LEN, 0, WLAN_MAC_ADDR_LEN);
            }
            break;
        case HMAC_HT_SELF_CURE_EVENT_ASSOC:
            if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_11BG_ASSOC);
            }
            break;
        default:
            break;
    }
}

OAL_STATIC void hmac_ht_self_cure_state_ht_txdhcp_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_TX_DHCP_FRAME:
            ht_self_cure->htCheckInfo.txDhcpTimes++;
            if (ht_self_cure->htCheckInfo.txDhcpTimes > HT_SELF_CURE_CLOSE_TX_DHCP_MAX_TIMES) {
                hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_CLOSE_HT);
                memcpy_s(ht_self_cure->htCheckInfo.checkHtUser, WLAN_MAC_ADDR_LEN, userMac, WLAN_MAC_ADDR_LEN);
            }
            break;
        case HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME:
            hmac_ht_self_cure_checkinfo_init();
            break;
        default:
            break;
    }
}

OAL_STATIC void hmac_ht_self_cure_state_closeht_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_ASSOC:
            if (mac_vap->en_protocol < WLAN_HT_MODE &&
                !OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_11BG_ASSOC);
                ht_self_cure->htCheckInfo.txDhcpTimes = 0;
            } else {
                hmac_ht_self_cure_checkinfo_init();
            }
            break;
        case HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME:
            hmac_ht_self_cure_checkinfo_init();
            break;
        default:
            break;
    }
}

OAL_STATIC void hmac_ht_self_cure_state_11bg_assoc_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_TX_DHCP_FRAME:
            hmac_ht_self_cure_state_change(HMAC_HT_SELF_CURE_STATE_11BG_TX_DHCP);
            ht_self_cure->htCheckInfo.txDhcpTimes++;
            break;
        case HAMC_HT_SELF_CURE_EVENT_RX_EAPOL:
            if (mac_vap->st_channel.en_band == WLAN_BAND_2G && MAC_IS_SMC_AP(userMac)) {
                if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                    hmac_ht_self_cure_add_blacklist(userMac);
                    hmac_ht_self_cure_checkinfo_init();
                }
            }
            break;
        case HMAC_HT_SELF_CURE_EVENT_ASSOC:
            if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                hmac_ht_self_cure_checkinfo_init();
            }
            break;
        default:
            break;
    }
}

OAL_STATIC void hmac_ht_self_cure_state_11bg_tx_dhcp_handle(mac_vap_stru *mac_vap,
    uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = &g_st_ht_self_cure_info;

    switch (eventId) {
        case HMAC_HT_SELF_CURE_EVENT_TX_DHCP_FRAME:
            ht_self_cure->htCheckInfo.txDhcpTimes++;
            if (ht_self_cure->htCheckInfo.txDhcpTimes > HT_SELF_CURE_CLOSE_TX_DHCP_MAX_TIMES) {
                hmac_ht_self_cure_checkinfo_init();
                oam_warning_log0(0, OAM_SF_SCAN, "hmac_ht_self_cure_state_11bg_tx_dhcp_handle:: fail to initcheckinfo");
            }
            break;
        case HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME:
            if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
                hmac_ht_self_cure_add_blacklist(userMac);
                hmac_ht_self_cure_checkinfo_init();
            }
            break;
        default:
            break;
    }
}

OAL_STATIC oal_bool_enum_uint8 hmac_ht_self_cure_is_need_check(mac_vap_stru *mac_vap, uint8_t *userMac)
{
        /* 非legacy sta直接退出 */
    if (!IS_LEGACY_STA(mac_vap) || g_wlan_spec_cfg->feature_ht_self_cure_is_open == OAL_FALSE) {
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

void  hmac_ht_self_cure_event_set(mac_vap_stru *mac_vap, uint8_t *userMac, uint8_t eventId)
{
    hmac_ht_self_cure_stru *ht_self_cure = NULL;
    hmac_ht_self_cure_state_enum nowState;

    ht_self_cure = &g_st_ht_self_cure_info;
    if (hmac_ht_self_cure_is_need_check(mac_vap, userMac) == OAL_FALSE) {
        return;
    }

    if (!hmac_ht_self_cure_is_htassoc_user(userMac)) {
        hmac_ht_self_cure_checkinfo_init();
    }

    nowState = ht_self_cure->htCheckInfo.htSelfCureCheckState;
    oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_ht_self_cure_event_set:Begin:nowState=%d eventId=%d \
        user:%02X:%02X", nowState, eventId,
        userMac[4],  /* 4 mac地址第4个字节 */
        userMac[5]); /* 5 mac地址第5个字节 */

    switch (nowState) {
        case HMAC_HT_SELF_CURE_STATE_INIT:
            hmac_ht_self_cure_state_init_handle(mac_vap, userMac, eventId);
            break;
        case HMAC_HT_SELF_CURE_STATE_ASSOC:
            hmac_ht_self_cure_state_assoc_handle(mac_vap, userMac, eventId);
            break;
        case HMAC_HT_SELF_CURE_STATE_HT_TX_DHCP:
            hmac_ht_self_cure_state_ht_txdhcp_handle(mac_vap, userMac, eventId);
            break;
        case HMAC_HT_SELF_CURE_STATE_CLOSE_HT:
            hmac_ht_self_cure_state_closeht_handle(mac_vap, userMac, eventId);
            break;
        case HMAC_HT_SELF_CURE_STATE_11BG_ASSOC:
            hmac_ht_self_cure_state_11bg_assoc_handle(mac_vap, userMac, eventId);
            break;
        case HMAC_HT_SELF_CURE_STATE_11BG_TX_DHCP:
            hmac_ht_self_cure_state_11bg_tx_dhcp_handle(mac_vap, userMac, eventId);
            break;
        default:
            break;
    }

    oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_ht_self_cure_event_set:After:pro=%d afterState=%d \
        txDhcpTimes=%d blackListNum=%d", mac_vap->en_protocol, ht_self_cure->htCheckInfo.htSelfCureCheckState,
        ht_self_cure->htCheckInfo.txDhcpTimes, g_st_ht_self_cure_info.blackListNum);
}

oal_bool_enum_uint8 hmac_ht_self_cure_in_blacklist(uint8_t *userMac)
{
    uint8_t loop;
    hmac_ht_self_cure_stru *ht_self_cure = NULL;
    hmac_ht_blacklist_stru *blacklist = NULL;

    if (g_wlan_spec_cfg->feature_ht_self_cure_is_open == OAL_FALSE) {
        return OAL_FALSE;
    }

    ht_self_cure = &g_st_ht_self_cure_info;

    if (ht_self_cure->htCheckInfo.htSelfCureCheckState == HMAC_HT_SELF_CURE_STATE_CLOSE_HT &&
        !OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
        return OAL_TRUE;
    }

    if (MAC_IS_SMC_AP(userMac)) {
        if (!OAL_MEMCMP(ht_self_cure->htCheckInfo.checkHtUser, userMac, WLAN_MAC_ADDR_LEN)) {
            return OAL_TRUE;
        }
    }

    for (loop = 0; loop < ht_self_cure->blackListNum; loop++) {
        blacklist = &ht_self_cure->htBlackList[loop];
        if (!OAL_MEMCMP(blacklist->userMacAddr, userMac, WLAN_MAC_ADDR_LEN)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

void hmac_ht_self_cure_need_check_flag_set(uint8_t flag)
{
    g_st_ht_self_cure_info.needCheckFlag = flag;

    oam_warning_log1(0, OAM_SF_SCAN, "hmac_ht_self_cure_need_check_flag_set::needCheckFlag = %d",
        g_st_ht_self_cure_info.needCheckFlag);
}

uint32_t hmac_ht_self_cure_clear_blacklist(void)
{
    memset_s(&g_st_ht_self_cure_info, sizeof(hmac_ht_self_cure_stru), 0, sizeof(hmac_ht_self_cure_stru));
    oam_warning_log0(0, OAM_SF_SCAN, "hmac_ht_self_cure_clear_blacklist::manual close and open wifi to clear");

    return OAL_SUCC;
}

#endif

