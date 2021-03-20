

#ifndef __HMAC_WIFI6_SELF_CURE_C_
#define __HMAC_WIFI6_SELF_CURE_C_
#ifdef _PRE_WLAN_FEATURE_11AX

/* 1 头文件包含 */
#include "hmac_ext_if.h"
#include "mac_data.h"
#include "hmac_resource.h"
#include "hmac_wifi6_self_cure.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "hmac_fsm.h"
#include "mac_user.h"
#include "hmac_roam_main.h"
#include "hmac_arp_probe.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WIFI6_SELF_CURE_C

OAL_STATIC hmac_wifi6_self_cure_info_stru g_wifi6SelfCureInfo;

/* 2 函数声明 */
/* 3 函数实现 */
void hmac_wifi6_close_htc_user_info(hmac_user_stru *hmac_user)
{
    memcpy_s(g_wifi6SelfCureInfo.currentProbeCloseHtcUser, WLAN_MAC_ADDR_LEN,
        hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN);
    oam_warning_log3(0, OAM_SF_SCAN, "hmac_wifi6_set_close_htc_user_info::\
        user_mac:%02X:XX:XX:XX:%02X:%02X close HTC to reassoc",
        hmac_user->st_user_base_info.auc_user_mac_addr[0], /* 0为mac地址第一个字节 */
        hmac_user->st_user_base_info.auc_user_mac_addr[4], /* 4为mac地址第一个字节 */
        hmac_user->st_user_base_info.auc_user_mac_addr[5]); /* 5为mac地址第一个字节 */
}

uint32_t  hmac_wifi6_self_cure_close_htc_handle(mac_vap_stru *p_mac_vap)
{
    hmac_user_stru *p_hmac_user                 = OAL_PTR_NULL;
    hmac_vap_stru  *p_hmac_vap                  = OAL_PTR_NULL;

    if (!IS_LEGACY_STA(p_mac_vap)) {
        return OAL_FALSE;
    }
    p_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(p_mac_vap->uc_vap_id);
    if (oal_unlikely(p_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY,
                       "{hmac_wifi6_self_cure_close_htc_handle::mac_res_get_hmac_vap fail.vap_id = %u}",
                       p_mac_vap->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    p_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(p_mac_vap->us_assoc_vap_id);
    if (p_hmac_user == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }
    if (!MAC_USER_TX_DATA_INCLUDE_HTC(&p_hmac_user->st_user_base_info)) {
        return OAL_SUCC;
    }
    hmac_wifi6_close_htc_user_info(p_hmac_user);
    oam_warning_log2(p_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_wifi6_self_cure_close_htc_handle::user rssi[%d] threhold[%d].}",
        p_hmac_user->c_rssi, WLAN_FAR_DISTANCE_RSSI);
    /* 非远场才触发重关联逻辑 */
    if (p_hmac_user->c_rssi > WLAN_FAR_DISTANCE_RSSI) {
        /* 发起reassoc req */
        oam_warning_log0(p_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_M2S,
            "{hmac_arp_probe_fail_reassoc_trigger::to reassoc.}");
        hmac_roam_start(p_hmac_vap, ROAM_SCAN_CHANNEL_ORG_0, OAL_FALSE,
            NULL, ROAM_TRIGGER_ARP_PROBE_FAIL_REASSOC);
    }
    return OAL_SUCC;
}


uint32_t  hmac_wifi6_self_cure_black_list_set(mac_vap_stru *p_mac_vap,
    uint8_t len, uint8_t *p_param)
{
    uint8_t cure_info_len;
    hmac_wifi6_self_cure_info_stru *p_save_info            = OAL_PTR_NULL;
    hmac_wifi6_self_cure_info_stru *p_wifi6_self_cure_info = OAL_PTR_NULL;
    uint8_t index;

    cure_info_len = OAL_SIZEOF(hmac_wifi6_self_cure_info_stru);
    if (len != cure_info_len) {
        return OAL_FAIL;
    }

    if (!IS_LEGACY_STA(p_mac_vap)) {
        return OAL_FAIL;
    }

    p_wifi6_self_cure_info = (hmac_wifi6_self_cure_info_stru *)p_param;
    oam_warning_log1(0, OAM_SF_M2S, "{hmac_wifi6_self_cure_black_list_set::uc_blaclist_num=[%d] .}",
        p_wifi6_self_cure_info->blaclistNum);

    p_save_info = &g_wifi6SelfCureInfo;
    if (p_save_info->selfCureType == HMAC_WIFI6_SELF_CURE_HANDLE_TYPE_DRIVER) {
        return OAL_SUCC;
    }

    memcpy_s(p_save_info, cure_info_len, p_wifi6_self_cure_info, cure_info_len);

    for (index = 0; index < p_save_info->blaclistNum; index++) {
            oam_warning_log4(0, OAM_SF_SCAN, "hmac_wifi6_self_cure_black_list_set::\
                user_mac_%d:%02X:XX:XX:XX:%02X:%02X",
                index,
                p_save_info->wifi6Blacklist[index].userMacAddr[0], /* 0为mac地址第一个字节 */
                p_save_info->wifi6Blacklist[index].userMacAddr[4], /* 4为mac地址第一个字节 */
                p_save_info->wifi6Blacklist[index].userMacAddr[5]); /* 5为mac地址第一个字节 */
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_wifi6_self_cure_mac_is_wifi6_blacklist_type(uint8_t *puc_mac)
{
    uint8_t loop;
    hmac_wifi6_self_cure_info_stru *p_save_info       = OAL_PTR_NULL;
    hmac_wifi6_self_cure_blacklist_stru *p_wifi6_list = OAL_PTR_NULL;

    p_save_info = &g_wifi6SelfCureInfo;

    for (loop = 0; loop < p_save_info->blaclistNum; loop++) {
        p_wifi6_list = &p_save_info->wifi6Blacklist[loop];
        if (!OAL_MEMCMP(p_wifi6_list->userMacAddr, puc_mac, WLAN_MAC_ADDR_LEN) &&
            p_wifi6_list->blacklistType == HMAC_WIFI6_SELF_CURE_BLACKLIST_TYPE_WIFI6) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


oal_bool_enum_uint8 hmac_wifi6_self_cure_mac_is_htc_blacklist_type(uint8_t *p_mac)
{
    uint8_t loop;
    hmac_wifi6_self_cure_info_stru *p_save_info = OAL_PTR_NULL;
    hmac_wifi6_self_cure_blacklist_stru *p_wifi6_list = OAL_PTR_NULL;

    p_save_info = &g_wifi6SelfCureInfo;

    if (!OAL_MEMCMP(p_save_info->currentProbeCloseHtcUser, p_mac, WLAN_MAC_ADDR_LEN)) {
        return OAL_TRUE;
    }

    for (loop = 0; loop < p_save_info->blaclistNum; loop++) {
        p_wifi6_list = &p_save_info->wifi6Blacklist[loop];
        if (!OAL_MEMCMP(p_wifi6_list->userMacAddr, p_mac, WLAN_MAC_ADDR_LEN) &&
            p_wifi6_list->blacklistType == HMAC_WIFI6_SELF_CURE_BLACKLIST_TYPE_HTC) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


void hmac_wifi6_self_cure_init(hmac_vap_stru *pst_hmac_vap)
{
    g_wifi6SelfCureInfo.selfCureType = HMAC_WIFI6_SELF_CURE_HANDLE_TYPE_APP;
}


void hmac_wifi6_self_cure_set_handle_type(hmac_wifi6_self_cure_handle_type_enum_uint8 type)
{
    oam_warning_log1(0, OAM_SF_M2S, "{hmac_wifi6_self_cure_set_handle_type::en_type=[%d].}", type);
    g_wifi6SelfCureInfo.selfCureType = type;
}

uint8_t hmac_wifi6_self_cure_get_handle_type(void)
{
    return g_wifi6SelfCureInfo.selfCureType;
}

#endif
#endif /* end of __HMAC_WIFI6_SELF_CURE_C__ */

