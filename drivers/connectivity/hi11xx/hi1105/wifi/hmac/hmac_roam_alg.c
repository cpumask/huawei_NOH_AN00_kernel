
/* 1 头文件包含 */
#include "oam_ext_if.h"
#include "mac_ie.h"
#include "mac_device.h"
#include "mac_resource.h"

#include "hmac_dfx.h"
#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_p2p.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#include "hmac_roam_alg.h"
#include "hmac_mgmt_sta.h"
#include "hmac_blacklist.h"

#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_ftm.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_ALG_C

/* 2 全局变量定义 */
hmac_roam_rssi_capacity_stru gst_rssi_table_11a_ofdm[ROAM_RSSI_LEVEL] = {
    { -75, 29300 },
    { -82, 17300 },
    { -90, 5400 }
};
hmac_roam_rssi_capacity_stru gst_rssi_table_11b[ROAM_RSSI_LEVEL] = {
    { -88, 8100 },
    { -92, 4900 },
    { -94, 900 }

};
hmac_roam_rssi_capacity_stru gst_rssi_table_11g_ofdm[ROAM_RSSI_LEVEL] = {
    { -75, 29300 },
    { -82, 17300 },
    { -90, 5400 }
};
hmac_roam_rssi_capacity_stru gst_rssi_table_ht20_ofdm[ROAM_RSSI_LEVEL] = {
    { -72, 58800 },
    { -80, 35300 },
    { -90, 5800 }
};
hmac_roam_rssi_capacity_stru gst_rssi_table_ht40_ofdm[ROAM_RSSI_LEVEL] = {
    { -75, 128100 },
    { -77, 70500 },
    { -87, 11600 }
};
hmac_roam_rssi_capacity_stru gst_rssi_table_vht80_ofdm[ROAM_RSSI_LEVEL] = {
    { -72, 256200 },
    { -74, 141000 },
    { -84, 23200 }
};

/* 3 函数实现 */

void hmac_roam_alg_init(hmac_roam_info_stru *pst_roam_info, int8_t c_current_rssi)
{
    hmac_roam_alg_stru *pst_roam_alg = NULL;

    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_alg_init::param null.}");
        return;
    }
    /*lint -e571*/
    oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_alg_init c_current_rssi = %d.}", c_current_rssi);
    /*lint +e571*/
    pst_roam_alg = &(pst_roam_info->st_alg);
    if (c_current_rssi == ROAM_RSSI_LINKLOSS_TYPE) {
        pst_roam_info->st_static.ul_trigger_linkloss_cnt++;
    } else {
        pst_roam_info->st_static.ul_trigger_rssi_cnt++;
    }

    pst_roam_alg->ul_max_capacity = 0;
    pst_roam_alg->pst_max_capacity_bss = OAL_PTR_NULL;
    pst_roam_alg->c_current_rssi = c_current_rssi;
    pst_roam_alg->c_max_rssi = 0;
    pst_roam_alg->uc_another_bss_scaned = 0;
    /* 首次关联时初始化 pst_roam_alg->uc_invalid_scan_cnt   = 0x0; */
    pst_roam_alg->pst_max_rssi_bss = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        if (mac_is_ftm_enable(&pst_roam_info->pst_hmac_vap->st_vap_base_info)) {
            pst_roam_info->st_config.uc_scan_orthogonal = ROAM_SCAN_CHANNEL_ORG_1;
        }
    }
#endif

    return;
}


void hmac_roam_alg_init_rssi(hmac_vap_stru *pst_hmac_vap, hmac_roam_info_stru *pst_roam_info)
{
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return;
    }

    if (oal_unlikely(pst_roam_info == OAL_PTR_NULL)) {
        return;
    }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        hmac_roam_alg_init(pst_roam_info, pst_roam_info->st_bsst_rsp_info.c_rssi);
    } else
#endif
        if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_HOME_NETWORK) {
            hmac_roam_alg_init(pst_roam_info, pst_hmac_vap->station_info.signal);
        } else {
            hmac_roam_alg_init(pst_roam_info, ROAM_RSSI_CMD_TYPE);
        }

    return;
}


OAL_STATIC int8_t hmac_roam_alg_adjust_5G_rssi_weight(int8_t c_orginal_rssi)
{
    int8_t c_current_rssi = c_orginal_rssi;

    if (c_current_rssi >= ROAM_RSSI_NE65_DB) {
        c_current_rssi += ROAM_RSSI_DIFF_20_DB;
    } else if (c_current_rssi < ROAM_RSSI_NE65_DB && c_current_rssi >= ROAM_RSSI_NE72_DB) {
        c_current_rssi += ROAM_RSSI_DIFF_10_DB;
    } else {
        c_current_rssi += ROAM_RSSI_DIFF_4_DB;
    }

    return c_current_rssi;
}


OAL_STATIC int8_t hmac_roam_alg_adjust_ax_rssi_weight(int8_t orginal_rssi)
{
    int8_t current_rssi = orginal_rssi;

    if (current_rssi >= ROAM_RSSI_NE65_DB) {
        current_rssi += ROAM_RSSI_DIFF_6_DB;
    } else if (current_rssi < ROAM_RSSI_NE65_DB && current_rssi > ROAM_RSSI_NE75_DB) {
        current_rssi += ROAM_RSSI_DIFF_2_DB;
    }

    return current_rssi;
}


OAL_STATIC int16_t hmac_roam_alg_compare_rssi_increase(hmac_roam_info_stru *pst_roam_info,
                                                         mac_bss_dscr_stru *pst_bss_dscr, int8_t c_target_weight_rssi)
{
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap = NULL;
    int16_t s_current_rssi;
    int16_t s_target_rssi;
    uint8_t uc_delta_rssi;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return -1;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    s_target_rssi = pst_bss_dscr->c_rssi;
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        uc_delta_rssi = pst_roam_info->st_config.uc_delta_rssi_5G;
    } else {
        uc_delta_rssi = pst_roam_info->st_config.uc_delta_rssi_2G;
    }

    if ((pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) &&
        (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G))  { // current AP is 2G, target AP is 5G
        s_target_rssi = c_target_weight_rssi;
    }

    s_current_rssi = pst_roam_info->st_alg.c_current_rssi;
    if ((pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) &&
        (pst_bss_dscr->st_channel.en_band == WLAN_BAND_2G)) {  // current AP is 5G, target AP is 2G
        s_current_rssi = (int16_t)hmac_roam_alg_adjust_5G_rssi_weight((int8_t)s_current_rssi);
    }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V &&
        (pst_bss_dscr->c_rssi >= ROAM_RSSI_NE70_DB)) {
        return 1;
    }
#endif

    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_BSSID) {
        return 1;
    }

    if (uc_delta_rssi < ROAM_RSSI_DIFF_4_DB) {
        uc_delta_rssi = ROAM_RSSI_DIFF_4_DB;
    }

    if (s_current_rssi >= ROAM_RSSI_NE70_DB) {
        return (s_target_rssi - s_current_rssi - (int16_t)uc_delta_rssi);
    }

    if (uc_delta_rssi >= ROAM_RSSI_DIFF_4_DB + 2) {
        /* 步进2DB至4DB */
        uc_delta_rssi -= 2;
    }

    if (s_current_rssi >= ROAM_RSSI_NE75_DB) {
        return (s_target_rssi - s_current_rssi - (int16_t)uc_delta_rssi);
    }

    if (uc_delta_rssi >= ROAM_RSSI_DIFF_4_DB + 2) {
        /* 步进2DB至4DB */
        uc_delta_rssi -= 2;
    }

    if (s_current_rssi >= ROAM_RSSI_NE80_DB) {
        return (s_target_rssi - s_current_rssi - (int16_t)uc_delta_rssi);
    }

    return (s_target_rssi - s_current_rssi - ROAM_RSSI_DIFF_4_DB);
}


OAL_STATIC uint32_t hmac_roam_alg_add_bsslist(hmac_roam_bss_list_stru *pst_roam_bss_list,
                                                uint8_t *puc_bssid, roam_blacklist_type_enum_uint8 list_type)
{
    hmac_roam_bss_info_stru *pst_cur_bss = NULL;
    hmac_roam_bss_info_stru *pst_oldest_bss;
    hmac_roam_bss_info_stru *pst_zero_bss;
    uint8_t auc_mac_zero[WLAN_MAC_ADDR_LEN] = { 0 };
    uint32_t ul_current_index;
    uint32_t ul_now;
    uint32_t ul_timeout;

    pst_oldest_bss = OAL_PTR_NULL;
    pst_zero_bss = OAL_PTR_NULL;
    ul_now = (uint32_t)oal_time_get_stamp_ms();

    for (ul_current_index = 0; ul_current_index < ROAM_LIST_MAX; ul_current_index++) {
        pst_cur_bss = &pst_roam_bss_list->ast_bss[ul_current_index];
        ul_timeout = (uint32_t)pst_cur_bss->ul_timeout;
        if (0 == oal_compare_mac_addr(pst_cur_bss->auc_bssid, puc_bssid)) {
            /* 优先查找已存在的记录，如果名单超时更新时间戳，否则更新count */
            if (oal_time_get_runtime(pst_cur_bss->ul_time_stamp, ul_now) > ul_timeout) {
                pst_cur_bss->ul_time_stamp = ul_now;
                pst_cur_bss->us_count = 1;
            } else {
                pst_cur_bss->us_count++;
                if (pst_cur_bss->us_count == pst_cur_bss->us_count_limit) {
                    pst_cur_bss->ul_time_stamp = ul_now;
                }
            }
            return OAL_SUCC;
        }

        /* 记录第一个空记录 */
        if (pst_zero_bss != OAL_PTR_NULL) {
            continue;
        }

        if (0 == oal_compare_mac_addr(pst_cur_bss->auc_bssid, auc_mac_zero)) {
            pst_zero_bss = pst_cur_bss;
            continue;
        }

        /* 记录一个非空最老记录 */
        if (pst_oldest_bss == OAL_PTR_NULL) {
            pst_oldest_bss = pst_cur_bss;
        } else {
            if (oal_time_get_runtime(pst_cur_bss->ul_time_stamp, ul_now) >
                oal_time_get_runtime(pst_oldest_bss->ul_time_stamp, ul_now)) {
                pst_oldest_bss = pst_cur_bss;
            }
        }
    }

    if (pst_zero_bss == OAL_PTR_NULL) {
        pst_zero_bss = pst_oldest_bss;
    }

    if (pst_zero_bss != OAL_PTR_NULL) {
        oal_set_mac_addr(pst_zero_bss->auc_bssid, puc_bssid);
        pst_zero_bss->ul_time_stamp = ul_now;
        pst_zero_bss->us_count = 1;
        return OAL_SUCC;
    }
    return OAL_FAIL;
}


OAL_STATIC oal_bool_enum_uint8 hmac_roam_alg_find_in_bsslist(hmac_roam_bss_list_stru *pst_roam_bss_list,
                                                             uint8_t *puc_bssid)
{
    hmac_roam_bss_info_stru *pst_cur_bss = NULL;
    uint32_t ul_current_index;
    uint32_t ul_now;
    uint32_t ul_timeout;
    uint32_t ul_delta_time;
    uint16_t us_count_limit;

    ul_now = (uint32_t)oal_time_get_stamp_ms();

    for (ul_current_index = 0; ul_current_index < ROAM_LIST_MAX; ul_current_index++) {
        pst_cur_bss = &pst_roam_bss_list->ast_bss[ul_current_index];
        ul_timeout = pst_cur_bss->ul_timeout;
        us_count_limit = pst_cur_bss->us_count_limit;

        if (0 == oal_compare_mac_addr(pst_cur_bss->auc_bssid, puc_bssid)) {
            /* 如果在超时时间内出现count_limit次以上记录 */
            ul_delta_time = oal_time_get_runtime(pst_cur_bss->ul_time_stamp, ul_now);
            if ((ul_delta_time <= ul_timeout) &&
                (pst_cur_bss->us_count >= us_count_limit)) {
                return OAL_TRUE;
            }
            return OAL_FALSE;
        }
    }

    return OAL_FALSE;
}


uint32_t hmac_roam_alg_add_blacklist(hmac_roam_info_stru *pst_roam_info,
                                           uint8_t *puc_bssid,
                                           roam_blacklist_type_enum_uint8 list_type)
{
    uint32_t ul_ret;
    if (oal_any_null_ptr2(pst_roam_info, puc_bssid)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_roam_alg_add_bsslist(&pst_roam_info->st_alg.st_blacklist, puc_bssid, list_type);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_alg_add_blacklist::hmac_roam_alg_add_list failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_roam_alg_find_in_blacklist(hmac_roam_info_stru *pst_roam_info, uint8_t *puc_bssid)
{
    if (oal_any_null_ptr2(pst_roam_info, puc_bssid)) {
        return OAL_FALSE;
    }

    return hmac_roam_alg_find_in_bsslist(&pst_roam_info->st_alg.st_blacklist, puc_bssid);
}


uint32_t hmac_roam_alg_add_history(hmac_roam_info_stru *pst_roam_info, uint8_t *puc_bssid)
{
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_roam_info, puc_bssid)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_roam_alg_add_bsslist(&pst_roam_info->st_alg.st_history, puc_bssid, ROAM_BLACKLIST_TYPE_NORMAL_AP);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_alg_add_history::hmac_roam_alg_add_list failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_roam_alg_find_in_history(hmac_roam_info_stru *pst_roam_info, uint8_t *puc_bssid)
{
    if (oal_any_null_ptr2(pst_roam_info, puc_bssid)) {
        return OAL_FALSE;
    }

    return hmac_roam_alg_find_in_bsslist(&pst_roam_info->st_alg.st_history, puc_bssid);
}

void hmac_roam_alg_scan_inc_one_channel(mac_scan_req_stru *p_scan_params,
                                        mac_channel_stru *p_channel)
{
    uint8_t index = p_scan_params->uc_channel_nums;

    oam_warning_log3(0, OAM_SF_ROAM,
                     "{hmac_roam_alg_scan_inc_one_channel::increase one channel, ChanNum[%d], Band[%d], ChanIdx[%d]}",
                     p_channel->uc_chan_number, p_channel->en_band, p_channel->uc_chan_idx);

    p_scan_params->ast_channel_list[index].uc_chan_number = p_channel->uc_chan_number;
    p_scan_params->ast_channel_list[index].en_band = p_channel->en_band;
    p_scan_params->ast_channel_list[index].uc_chan_idx = p_channel->uc_chan_idx;

    p_scan_params->uc_channel_nums = (++index);
}

uint32_t hmac_roam_alg_scan_one_channel_init(hmac_roam_info_stru *pst_roam_info,
                                                   mac_scan_req_stru *pst_scan_params)
{
    hmac_vap_stru *pst_hmac_vap = NULL;
    mac_channel_stru *pst_channel = NULL;

    if (oal_any_null_ptr2(pst_roam_info, pst_scan_params)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_alg_scan_one_channel_init::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    pst_channel = &(pst_hmac_vap->st_vap_base_info.st_channel);

    oam_info_log3(0, OAM_SF_ROAM,
                  "{hmac_roam_alg_scan_channel_init::scan one channel: ChanNum[%d], Band[%d], ChanIdx[%d]}",
                  pst_channel->uc_chan_number, pst_channel->en_band, pst_channel->uc_chan_idx);

    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_HOME_NETWORK) {  // scan 5G home network
        pst_channel = &(pst_roam_info->st_alg.st_home_network.st_5g_bssid.st_channel[0]);
        hmac_roam_alg_scan_inc_one_channel(pst_scan_params, pst_channel);
    }
    else {
        hmac_roam_alg_scan_inc_one_channel(pst_scan_params, pst_channel);
    }

    pst_scan_params->uc_max_scan_count_per_channel = 1;

    return OAL_SUCC;
}


uint32_t hmac_roam_alg_scan_spec_2g_chan(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params)
{
    uint32_t ul_ret = OAL_FAIL;

    if (pst_roam_info->st_config.uc_scan_band & ROAM_BAND_2G_BIT) {
        switch (pst_roam_info->st_config.uc_scan_orthogonal) {
            case ROAM_SCAN_CHANNEL_ORG_3: {
                pst_scan_params->ast_channel_list[0].uc_chan_number = 1;
                pst_scan_params->ast_channel_list[0].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[0].uc_chan_idx = 0;

                pst_scan_params->ast_channel_list[1].uc_chan_number = 6;
                pst_scan_params->ast_channel_list[1].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[1].uc_chan_idx = 5;

                pst_scan_params->ast_channel_list[2].uc_chan_number = 11;
                pst_scan_params->ast_channel_list[2].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[2].uc_chan_idx = 10;

                pst_scan_params->uc_channel_nums = 3;
                pst_scan_params->uc_max_scan_count_per_channel = 1;

                ul_ret = OAL_SUCC;
                break;
            }
            case ROAM_SCAN_CHANNEL_ORG_4: {
                pst_scan_params->ast_channel_list[0].uc_chan_number = 1;
                pst_scan_params->ast_channel_list[0].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[0].uc_chan_idx = 0;

                pst_scan_params->ast_channel_list[1].uc_chan_number = 5;
                pst_scan_params->ast_channel_list[1].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[1].uc_chan_idx = 4;

                pst_scan_params->ast_channel_list[2].uc_chan_number = 9;
                pst_scan_params->ast_channel_list[2].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[2].uc_chan_idx = 8;

                pst_scan_params->ast_channel_list[3].uc_chan_number = 13;
                pst_scan_params->ast_channel_list[3].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[3].uc_chan_idx = 12;

                pst_scan_params->uc_channel_nums = 4;
                pst_scan_params->uc_max_scan_count_per_channel = 1;

                ul_ret = OAL_SUCC;
                break;
            }
            default: {
                break;
            }
        }
    }
    return ul_ret;
}


void hmac_roam_alg_scan_full_chan(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params)
{
    uint32_t ul_ret;
    uint8_t uc_chan_idx;
    uint8_t uc_chan_number;

    if (pst_roam_info->st_config.uc_scan_band & ROAM_BAND_2G_BIT) {
        for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_chan_idx++) {
            ul_ret = mac_is_channel_idx_valid(WLAN_BAND_2G, uc_chan_idx);
            if (ul_ret != OAL_SUCC) {
                continue;
            }
            mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_chan_idx, &uc_chan_number);
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_number = uc_chan_number;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].en_band = WLAN_BAND_2G;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_idx = uc_chan_idx;
            pst_scan_params->uc_channel_nums++;
        }
    }
    if (pst_roam_info->st_config.uc_scan_band & ROAM_BAND_5G_BIT) {
        for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_chan_idx++) {
            ul_ret = mac_is_channel_idx_valid(WLAN_BAND_5G, uc_chan_idx);
            if (ul_ret != OAL_SUCC) {
                continue;
            }
            mac_get_channel_num_from_idx(WLAN_BAND_5G, uc_chan_idx, &uc_chan_number);
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_number = uc_chan_number;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].en_band = WLAN_BAND_5G;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_idx = uc_chan_idx;
            pst_scan_params->uc_channel_nums++;
        }
    }
    pst_scan_params->uc_max_scan_count_per_channel = 2;
}

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE

uint32_t hmac_roam_alg_scan_11v_chan(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params)
{
    uint8_t uc_chan_idx;
    uint8_t uc_chan_nums;
    mac_scan_req_stru *pst_src_scan_params;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info;
    hmac_user_stru *pst_hmac_user = pst_roam_info->pst_hmac_user;

    if (pst_hmac_user == OAL_PTR_NULL || pst_roam_info->en_roam_trigger != ROAM_TRIGGER_11V) {
        return OAL_FAIL;
    }

    pst_11v_ctrl_info = &pst_hmac_user->st_11v_ctrl_info;
    pst_src_scan_params = &pst_11v_ctrl_info->st_scan_h2d_params.st_scan_params;

    uc_chan_nums = pst_src_scan_params->uc_channel_nums;

    for (uc_chan_idx = 0; uc_chan_idx < uc_chan_nums ; uc_chan_idx++) {
        pst_scan_params->ast_channel_list[uc_chan_idx].uc_chan_number =
            pst_src_scan_params->ast_channel_list[uc_chan_idx].uc_chan_number;
        pst_scan_params->ast_channel_list[uc_chan_idx].en_band =
            pst_src_scan_params->ast_channel_list[uc_chan_idx].en_band;
        pst_scan_params->ast_channel_list[uc_chan_idx].uc_chan_idx =
            pst_src_scan_params->ast_channel_list[uc_chan_idx].uc_chan_idx;
    }
    pst_scan_params->uc_channel_nums = uc_chan_nums;
    pst_scan_params->uc_max_scan_count_per_channel = 1;
    pst_scan_params->en_need_switch_back_home_channel = OAL_TRUE;
    pst_scan_params->uc_scan_channel_interval = MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE;
    pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE;
    oam_warning_log1(0, OAM_SF_ROAM, "hmac_roam_alg_scan_11v_chan:: chan_nums %d", pst_scan_params->uc_channel_nums);
    return OAL_SUCC;
}
#endif

uint32_t hmac_roam_single_band_channel_list(mac_scan_req_stru *scan_params, wlan_channel_band_enum_uint8 band)
{
    uint8_t chan_idx;
    uint8_t chan_number;
    uint8_t chan_end_idx;
    mac_channel_stru *channel_item = NULL;
    if (band == WLAN_BAND_2G) {
        chan_end_idx = MAC_CHANNEL_FREQ_2_BUTT;
    } else {
        chan_end_idx = MAC_CHANNEL_FREQ_5_BUTT;
    }

    for (chan_idx = 0; chan_idx < chan_end_idx; chan_idx++) {
        /* 判断信道是不是在管制域内 */
        if (mac_is_channel_idx_valid(band, chan_idx) == OAL_SUCC) {
            mac_get_channel_num_from_idx(band, chan_idx, &chan_number);
            channel_item = &scan_params->ast_channel_list[scan_params->uc_channel_nums];
            channel_item->uc_chan_number = chan_number;
            channel_item->en_band = band;
            channel_item->uc_chan_idx = chan_idx;
            scan_params->uc_channel_nums++;
        }
    }
    return OAL_SUCC;
}


uint32_t hmac_roam_alg_scan_channel_init(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params)
{
    if (oal_any_null_ptr2(pst_roam_info, pst_scan_params)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_alg_scan_channel_init::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_scan_params->uc_channel_nums = 0;

    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_1) {
        return hmac_roam_alg_scan_one_channel_init(pst_roam_info, pst_scan_params);
    }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_5) {
        return hmac_roam_alg_scan_11v_chan(pst_roam_info, pst_scan_params);
    }
#endif

    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_3 ||
        pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_4) {
        return hmac_roam_alg_scan_spec_2g_chan(pst_roam_info, pst_scan_params);
    }
    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_2G) {
        return hmac_roam_single_band_channel_list(pst_scan_params, WLAN_BAND_2G);
    }
    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_5G) {
        return hmac_roam_single_band_channel_list(pst_scan_params, WLAN_BAND_5G);
    }
    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_DBDC) {
        /* check p2p up state: if there is no up p2p, will execute full scan by default */
        mac_vap_stru *other_vap;
        other_vap = mac_vap_find_another_up_vap_by_mac_vap(&pst_roam_info->pst_hmac_vap->st_vap_base_info);
        if ((other_vap != NULL) && (hmac_p2p_get_scenes(other_vap) == MAC_P2P_SCENES_LOW_LATENCY)) {
            mac_channel_stru *channel = &(other_vap->st_channel);
            if (channel->en_band == WLAN_BAND_5G) {
                // P2P working at 5G band, scan 2G band and p2p channel
                hmac_roam_single_band_channel_list(pst_scan_params, WLAN_BAND_2G);
                hmac_roam_alg_scan_inc_one_channel(pst_scan_params, channel);
            } else {
                // P2P working at 2G band, scan p2p channel and 5G band
                hmac_roam_alg_scan_inc_one_channel(pst_scan_params, channel);
                hmac_roam_single_band_channel_list(pst_scan_params, WLAN_BAND_5G);
            }
            return OAL_SUCC;
        }
    }
    hmac_roam_alg_scan_full_chan(pst_roam_info, pst_scan_params);
    return OAL_SUCC;
}

#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11V_ENABLE)

void hmac_roam_neighbor_report_add_chan(mac_scan_req_stru *pst_scan_params,
    uint8_t uc_channum, wlan_channel_band_enum_uint8 en_band, uint8_t uc_channel_idx)
{
    uint8_t   uc_loop;

    if (pst_scan_params == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_neighbor_report_add_chan::pst_scan_params is NULL}");
        return;
    }

    if (pst_scan_params->uc_channel_nums >= ROAM_NEIGHBOR_RPT_LIST_CHN_MAX_NUM) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_neighbor_report_add_chan::exceed max neighbor list}");
        return;
    }

    for (uc_loop = 0; uc_loop < pst_scan_params->uc_channel_nums; uc_loop++) {
        if (pst_scan_params->ast_channel_list[uc_loop].uc_chan_number == uc_channum) {
            return;
        }
    }

    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_number  = uc_channum;
    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].en_band         = en_band;
    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_idx     = uc_channel_idx;
    pst_scan_params->uc_channel_nums++;
}


void hmac_roam_neighbor_report_add_bssid(hmac_roam_info_stru *pst_roam_info, uint8_t *puc_bssid)
{
    uint8_t *puc_idex = OAL_PTR_NULL;

    if (pst_roam_info == OAL_PTR_NULL || puc_bssid == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_neighbor_report_add_bssid::NULL pointer}");
        return;
    }

    puc_idex = &pst_roam_info->uc_neighbor_rpt_bssid_num;
    if (*puc_idex >= ROAM_NEIGHBOR_RPT_LIST_BSSID_MAX_NUM) {
        return;
    }

    if (EOK == memcpy_s(pst_roam_info->auc_neighbor_rpt_bssid[*puc_idex], WLAN_MAC_ADDR_LEN,
        puc_bssid, WLAN_MAC_ADDR_LEN)) {
        (*puc_idex)++;
        return;
    }
    oam_error_log0(0, OAM_SF_ANY, "hmac_roam_neighbor_report_add_bssid::memcpy fail!");
}
#endif


OAL_STATIC uint32_t hmac_roam_alg_get_capacity_by_rssi(wlan_protocol_enum_uint8 en_protocol,
                                                         wlan_bw_cap_enum_uint8 en_bw_cap, int8_t c_rssi)
{
    hmac_roam_rssi_capacity_stru *pst_rssi_table = OAL_PTR_NULL;
    uint8_t uc_index;

    switch (en_protocol) {
        case WLAN_LEGACY_11A_MODE:
            pst_rssi_table = gst_rssi_table_11a_ofdm;
            break;

        case WLAN_LEGACY_11B_MODE:
            pst_rssi_table = gst_rssi_table_11b;
            break;

        case WLAN_LEGACY_11G_MODE:
        case WLAN_MIXED_ONE_11G_MODE:
        case WLAN_MIXED_TWO_11G_MODE:
            pst_rssi_table = gst_rssi_table_11g_ofdm;
            break;

        case WLAN_HT_MODE:
        case WLAN_VHT_MODE:
            if (en_bw_cap == WLAN_BW_CAP_20M) {
                pst_rssi_table = gst_rssi_table_ht20_ofdm;
            } else if (en_bw_cap == WLAN_BW_CAP_40M) {
                pst_rssi_table = gst_rssi_table_ht40_ofdm;
            } else {
                pst_rssi_table = gst_rssi_table_vht80_ofdm;
            }
            break;

        default:
            break;
    }

    if (pst_rssi_table == OAL_PTR_NULL) {
        return 0;
    }

    for (uc_index = 0; uc_index < ROAM_RSSI_LEVEL; uc_index++) {
        if (c_rssi >= pst_rssi_table[uc_index].c_rssi) {
            return pst_rssi_table[uc_index].ul_capacity_kbps;
        }
    }

    return 0;
}

OAL_STATIC uint32_t hmac_roam_alg_calc_avail_channel_capacity(mac_bss_dscr_stru *pst_bss_dscr)
{
    uint32_t ul_capacity;
    uint32_t ul_avail_channel_capacity;
    uint32_t ul_ret;
    uint8_t uc_channel_utilization = 0;
    uint8_t *puc_obss_ie = NULL;
    uint8_t uc_ie_offset;
    wlan_protocol_enum_uint8 en_protocol;

    ul_ret = hmac_sta_get_user_protocol_etc(pst_bss_dscr, &en_protocol);
    if (ul_ret != OAL_SUCC) {
        return 0;
    }
    /***************************************************************************
        ------------------------------------------------------------------------
        |EID |Len |StationCount |ChannelUtilization |AvailableAdmissionCapacity|
        ------------------------------------------------------------------------
        |1   |1   |2            |1                  |2                         |
        ------------------------------------------------------------------------
        ***************************************************************************/
    /*lint -e416*/
    uc_ie_offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    puc_obss_ie = mac_find_ie(MAC_EID_QBSS_LOAD, (uint8_t *)(pst_bss_dscr->auc_mgmt_buff + uc_ie_offset),
                                  (int32_t)(pst_bss_dscr->ul_mgmt_len - uc_ie_offset));
    /*lint +e416*/
    /* 长度要到达ChannelUtilization这个域，至少为3 */
    if (puc_obss_ie && (puc_obss_ie[1] >= 3)) {
        uc_channel_utilization = *(puc_obss_ie + 4);
    }

    ul_capacity = hmac_roam_alg_get_capacity_by_rssi(en_protocol, pst_bss_dscr->en_bw_cap, pst_bss_dscr->c_rssi);

    ul_avail_channel_capacity = ul_capacity * (255 - uc_channel_utilization) / 255 / ROAM_CONCURRENT_USER_NUMBER;

    return ul_avail_channel_capacity;
}


OAL_STATIC void hmac_roam_alg_record_bss_info(hmac_roam_record_stru *pst_roam_record,
    mac_bss_dscr_stru *pst_bss_dscr)
{
    mac_channel_stru *pst_channel = NULL;

    if (oal_any_null_ptr2(pst_roam_record, pst_bss_dscr)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_alg_record_bss_info::param null.}");
        return;
    }

    if (pst_roam_record->uc_bssid_num >= MAX_RECORD_BSSID_NUM) {
        return;
    }

    /* update channel info if bssid is already recorded */
    if (pst_roam_record->uc_bssid_num == 1 &&
        oal_memcmp(pst_roam_record->auc_bssid[0], pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) == 0) {
        if (pst_bss_dscr->st_channel.uc_chan_number != pst_roam_record->st_channel[0].uc_chan_number) {
            pst_roam_record->st_channel[0] = pst_bss_dscr->st_channel;
        }
        return;
    }

    if (memcpy_s(pst_roam_record->auc_bssid[pst_roam_record->uc_bssid_num], WLAN_MAC_ADDR_LEN,
                 pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_alg_record_bss_info::bssid memcpy fail!");
        return;
    }

    pst_channel = &(pst_roam_record->st_channel[pst_roam_record->uc_bssid_num]);
    if (memcpy_s(pst_channel, sizeof(mac_channel_stru),
                 &(pst_bss_dscr->st_channel), sizeof(mac_channel_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_alg_record_bss_info::channel memcpy fail!");
        return;
    }

    pst_roam_record->uc_bssid_num++;

    return;
}

OAL_STATIC void hmac_roam_alg_record_bss(hmac_roam_alg_stru *pst_roam_alg, mac_bss_dscr_stru *pst_bss_dscr)
{
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        hmac_roam_alg_record_bss_info(&(pst_roam_alg->st_home_network.st_5g_bssid), pst_bss_dscr);
    } else if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_2G) {
        hmac_roam_alg_record_bss_info(&(pst_roam_alg->st_home_network.st_2g_bssid), pst_bss_dscr);
    } else {
        oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_in_ess::en_band[%d].}", pst_bss_dscr->st_channel.en_band);
    }
}


uint32_t hmac_roam_alg_bss_in_ess(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru *pst_hmac_vap = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_roam_alg_stru *pst_roam_alg = NULL;
    mac_cfg_ssid_param_stru st_cfg_ssid;
    uint8_t uc_stru_len;
    uint32_t roam_timeout;

    if (oal_any_null_ptr2(pst_roam_info, pst_bss_dscr)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_in_ess::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_mib_get_ssid (pst_mac_vap, &uc_stru_len, (uint8_t *)(&st_cfg_ssid));

    if ((OAL_STRLEN(pst_bss_dscr->ac_ssid) != st_cfg_ssid.uc_ssid_len) ||
        (0 != oal_memcmp(st_cfg_ssid.ac_ssid, pst_bss_dscr->ac_ssid, st_cfg_ssid.uc_ssid_len))) {
        return OAL_SUCC;
    }

    /* 挑选HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE时间以内的有效bss */
    roam_timeout = ((g_pd_bss_expire_time * 1000) < HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE) ?  /* 1000 ms */
        HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE : (g_pd_bss_expire_time * 1000); /* 1000 ms */
    if (oal_time_after32(oal_time_get_stamp_ms(),
                         (pst_bss_dscr->ul_timestamp + roam_timeout))) {
        return OAL_SUCC;
    }

    pst_roam_alg = &(pst_roam_info->st_alg);
    /* 是否扫描到了当前关联的 bss, 仅置位，不过滤 */
    if (0 != oal_compare_mac_addr(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid)) {
        pst_roam_alg->uc_another_bss_scaned = 1;

        /* Roaming Scenario Recognition
         * dense AP standard: RSSI>=-60dB, candidate num>=5;
         *                    RSSI<-60dB && RSSI >=-75dB, candidate num>=10;
         */
        pst_roam_alg->uc_candidate_bss_num++;
        if (pst_bss_dscr->c_rssi >= pst_roam_info->st_config.c_candidate_good_rssi) {
            pst_roam_alg->uc_candidate_good_rssi_num++;
        } else if ((pst_bss_dscr->c_rssi < pst_roam_info->st_config.c_candidate_good_rssi) &&
                   pst_bss_dscr->c_rssi >= ROAM_RSSI_NE75_DB) {
            pst_roam_alg->uc_candidate_weak_rssi_num++;
        }

        if (pst_bss_dscr->c_rssi > pst_roam_alg->c_max_rssi) {
            pst_roam_alg->c_max_rssi = pst_bss_dscr->c_rssi;
        }
    } else {
        pst_roam_alg->c_current_rssi = pst_bss_dscr->c_rssi;
    }

    hmac_roam_alg_record_bss(pst_roam_alg, pst_bss_dscr);

    return OAL_SUCC;
}


uint32_t hmac_roam_check_cipher_limit(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    mac_vap_stru *pst_mac_vap;
    mac_cap_info_stru *pst_cap_info;
#ifdef _PRE_WLAN_FEATURE_SAE
    mac_crypto_settings_stru st_crypto;
    uint32_t ul_match_suite;
    uint32_t aul_rsn_akm_suites[WLAN_PAIRWISE_CIPHER_SUITES] = { 0 };
#ifdef _PRE_WLAN_FEATURE_11R
    wlan_auth_alg_mode_enum_uint8 en_auth_mode;
    uint8_t *puc_mde = OAL_PTR_NULL;
#endif
#endif

    pst_mac_vap = &pst_roam_info->pst_hmac_vap->st_vap_base_info;
    pst_cap_info = (mac_cap_info_stru *)&pst_bss_dscr->us_cap_info;

    /*  wep的bss直接过滤掉 */
    if ((pst_bss_dscr->puc_rsn_ie == OAL_PTR_NULL) &&
        (pst_bss_dscr->puc_wpa_ie == OAL_PTR_NULL) &&
        (pst_cap_info->bit_privacy != 0)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /*  open加密方式到wpa/wpa2直接过滤掉 */
    if ((pst_cap_info->bit_privacy == 0) != (OAL_TRUE != mac_mib_get_privacyinvoked(pst_mac_vap))) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    /* 不支持SAE与其他任何非SAE认证方式的漫游 */
    memset_s(&st_crypto, OAL_SIZEOF(mac_crypto_settings_stru), 0, OAL_SIZEOF(mac_crypto_settings_stru));
    mac_ie_get_rsn_cipher(pst_bss_dscr->puc_rsn_ie, &st_crypto);
    ul_match_suite = mac_mib_rsn_akm_match_suites_s(pst_mac_vap, st_crypto.aul_akm_suite,
                                                    sizeof(st_crypto.aul_akm_suite));
    if (ul_match_suite == 0) {
        mac_mib_get_rsn_akm_suites_s(pst_mac_vap, aul_rsn_akm_suites, sizeof(aul_rsn_akm_suites));
        if (IS_ONLY_SUPPORT_SAE(aul_rsn_akm_suites) || IS_ONLY_SUPPORT_SAE(st_crypto.aul_akm_suite)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_roam_check_cipher_limit::forbid roam between sae and others");

            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }
#endif

#ifdef _PRE_WLAN_FEATURE_11R
    if (pst_roam_info->pst_hmac_vap->bit_11r_enable) {
        en_auth_mode = mac_mib_get_AuthenticationMode(pst_mac_vap);
        puc_mde = mac_find_ie(MAC_EID_MOBILITY_DOMAIN,
                                  pst_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN + MAC_SSID_OFFSET,
                                  pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN - MAC_SSID_OFFSET);
        /* SAE不支持11R漫游 */
        if ((pst_roam_info->pst_hmac_vap->en_sae_connect == OAL_TRUE) && (puc_mde != OAL_PTR_NULL)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_roam_check_cipher_limit::SAE do not support 11r roam");
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
            /* 不支持ft到非ft */
        } else if ((en_auth_mode == WLAN_WITP_AUTH_FT) && (puc_mde == OAL_PTR_NULL)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_roam_check_cipher_limit::not support roam from FT  to Non_FT");
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }
#endif

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11K

oal_bool_enum_uint8 hmac_roam_alg_neighbor_rpt_is_target_ap(hmac_roam_info_stru *pst_roam_info,
                                                                uint8_t *puc_mac_addr)
{
    uint8_t uc_loop;
    for (uc_loop = 0; uc_loop < pst_roam_info->uc_neighbor_rpt_bssid_num; uc_loop++) {
        if (0 == oal_compare_mac_addr(pst_roam_info->auc_neighbor_rpt_bssid[uc_loop], puc_mac_addr)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}
#endif

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE

oal_bool_enum_uint8 hmac_roam_alg_check_bsst_bssid_in_scan_list(hmac_roam_info_stru *pst_roam_info,
    uint8_t *puc_mac_addr)
{
    uint8_t uc_bss_list_idx = 0;
    oal_bool_enum_uint8 uc_bssid_find = OAL_FALSE;
    uint8_t *puc_dst_mac_addr;

    if (pst_roam_info == OAL_PTR_NULL || puc_mac_addr == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_roam_alg_check_bsst_bssid_in_scan_list:: NULL pointer}");
        return OAL_FALSE;
    }

    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        uint8_t uc_neighbor_bssid_num = pst_roam_info->uc_neighbor_rpt_bssid_num;
        for (uc_bss_list_idx = 0; uc_bss_list_idx < uc_neighbor_bssid_num; uc_bss_list_idx++) {
            puc_dst_mac_addr = pst_roam_info->auc_neighbor_rpt_bssid[uc_bss_list_idx];
            if (!oal_memcmp(pst_roam_info->auc_neighbor_rpt_bssid[uc_bss_list_idx], puc_mac_addr, WLAN_MAC_ADDR_LEN)) {
                uc_bssid_find = OAL_TRUE;
                break;
            }
        }
    }
    return uc_bssid_find;
}
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */


uint32_t hmac_roam_alg_check_bssid_limit(hmac_roam_info_stru *pst_roam_info, uint8_t *puc_mac_addr)
{
    uint32_t ul_ret;

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        ul_ret = hmac_roam_alg_check_bsst_bssid_in_scan_list(pst_roam_info, puc_mac_addr);
        if (ul_ret != OAL_TRUE) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
        oam_warning_log3(0, OAM_SF_ROAM,
                         "{hmac_roam_alg_check_bssid_limit :: [%02X:XX:XX:XX:%02X:%02X] is in scan list}",
                         puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]);
    }
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */

    /* 检查黑名单 */
    ul_ret = hmac_roam_alg_find_in_blacklist(pst_roam_info, puc_mac_addr);
    if (ul_ret == OAL_TRUE) {
        oam_warning_log3(0, OAM_SF_ROAM, "{hmac_roam_alg_check_bssid_limit:: [%02X:XX:XX:XX:%02X:%02X] in blacklist!}",
                         puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]);
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    return OAL_SUCC;
}

#define IS_ROAM_HOMENETWORK_2GTO5G_CONDITION(pst_hmac_vap, pst_roam_info, pst_bss_dscr) \
    ((pst_roam_info->st_alg.st_home_network.uc_is_homenetwork == OAL_TRUE) &&           \
        (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) &&          \
        (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) &&                           \
        (pst_bss_dscr->c_rssi > pst_roam_info->st_alg.st_home_network.c_5g_rssi_th))

#define IS_ROAM_WEAK_RSSI_CONDITION(c_tmp_rssi, pst_bss_dscr)                                    \
    (((c_tmp_rssi < ROAM_RSSI_NE80_DB) && (pst_bss_dscr->st_channel.en_band == WLAN_BAND_2G)) ||  \
        ((c_tmp_rssi < ROAM_RSSI_NE78_DB) && (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G)))

OAL_STATIC uint32_t hmac_roam_alg_check_aging_time(hmac_roam_info_stru *pst_roam_info,
                                                     mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru *pst_hmac_vap;
    uint32_t roam_timeout;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    roam_timeout = ((g_pd_bss_expire_time * 1000) < HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE) ? /* 1000 ms */
        HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE : (g_pd_bss_expire_time * 1000); /* 1000 ms */

    if (pst_roam_info->en_current_bss_ignore == OAL_TRUE) {
        /* 非voe认证, 挑选HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE时间以内的有效bss */
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
        if (pst_hmac_vap->bit_voe_11r_auth == 0)
#endif
        {
            if (oal_time_after32(oal_time_get_stamp_ms(),
                                 (pst_bss_dscr->ul_timestamp + roam_timeout))) {
                return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
            }
        }
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_alg_check_channel(mac_vap_stru *mac_vap, hmac_roam_info_stru *roam_info,
    mac_bss_dscr_stru *bss_dscr)
{
    hmac_roam_alg_stru *roam_alg = &(roam_info->st_alg);
    mac_vap_stru *p2p_vap = mac_vap_find_another_up_vap_by_mac_vap(mac_vap);

    /* check dbac channel compared with p2p up channel */
    if (hmac_roam_check_dbac_channel(p2p_vap, bss_dscr->st_channel.uc_chan_number) == OAL_TRUE) {
        oam_warning_log3(0, OAM_SF_ROAM,
            "{hmac_roam_check_dbac_channel::dbac channel ignored, %0x:%0x chanNum=%d}",
            bss_dscr->auc_mac_addr[4], bss_dscr->auc_mac_addr[5], /* mac addr 4 & 5 */
            bss_dscr->st_channel.uc_chan_number);
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* 非双sta模式，不做判断 */
    if (!mac_is_dual_sta_mode()) {
        return OAL_SUCC;
    }

    if (mac_is_primary_legacy_vap(mac_vap)) {
        /* wlan0已经找到了同频段可漫游的bss, 待处理bss是异频，不处理该bss。否则需要继续处理 */
        if (roam_alg->pst_max_rssi_bss &&
            roam_alg->pst_max_rssi_bss->st_channel.en_band == mac_vap->st_channel.en_band &&
            bss_dscr->st_channel.en_band != mac_vap->st_channel.en_band) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    } else {
        /* wlan1漫游不考虑异频的bss */
        if (bss_dscr->st_channel.en_band != mac_vap->st_channel.en_band) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC uint8_t hmac_roam_alg_compare_bss(hmac_vap_stru *pst_hmac_vap, hmac_roam_alg_stru *roam_alg,
    mac_bss_dscr_stru *bss_dscr, int8_t tmp_rssi)
{
    mac_vap_stru *mac_vap = &pst_hmac_vap->st_vap_base_info;

    if (roam_alg->pst_max_rssi_bss == NULL) {
        return OAL_TRUE;
    }

    /* 双sta模式下，如果已找到的bss是异频的，当前处理的bss是同频。不比较rssi，优先同频。 */
    if (mac_is_dual_sta_mode() &&
        roam_alg->pst_max_rssi_bss->st_channel.en_band != mac_vap->st_channel.en_band &&
        bss_dscr->st_channel.en_band == mac_vap->st_channel.en_band) {
        oam_warning_log1(0, OAM_SF_ROAM,
            "{hmac_roam_compare_bss::dual sta mode find first same band[%d] bss, replace the result!}",
            bss_dscr->st_channel.en_band);
        return OAL_TRUE;
    }

    return tmp_rssi > roam_alg->c_max_rssi;
}


uint32_t hmac_roam_alg_bss_check(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_roam_alg_stru *pst_roam_alg = NULL;
    uint8_t *puc_pmkid = NULL;
    mac_cfg_ssid_param_stru st_cfg_ssid;
    uint32_t ul_ret;
    uint32_t ul_avail_channel_capacity;
    uint8_t uc_stru_len;
    int16_t s_delta_rssi;
    int8_t c_tmp_rssi;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_mib_get_ssid (pst_mac_vap, &uc_stru_len, (uint8_t *)(&st_cfg_ssid));

    if ((OAL_STRLEN(pst_bss_dscr->ac_ssid) != st_cfg_ssid.uc_ssid_len) ||
        (0 != oal_memcmp(st_cfg_ssid.ac_ssid, pst_bss_dscr->ac_ssid, st_cfg_ssid.uc_ssid_len))) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }
    pst_roam_alg = &(pst_roam_info->st_alg);

    /* 检查漫游到自己 */
    if ((pst_roam_info->en_current_bss_ignore == OAL_FALSE) &&
        oal_memcmp(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid, OAL_MAC_ADDR_LEN)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* 检查特定BSSID的漫游 */
    if ((pst_roam_info->en_roam_trigger == ROAM_TRIGGER_BSSID) &&
        oal_memcmp(pst_roam_info->auc_target_bssid, pst_bss_dscr->auc_bssid, OAL_MAC_ADDR_LEN)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    ul_ret = hmac_roam_alg_check_channel(pst_mac_vap, pst_roam_info, pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    ul_ret = hmac_roam_alg_check_aging_time(pst_roam_info, pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    ul_ret = hmac_roam_alg_check_bssid_limit(pst_roam_info, pst_bss_dscr->auc_bssid);
    if (ul_ret != OAL_SUCC) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* check bssid blacklist from Framework setting */
    ul_ret = hmac_blacklist_filter(pst_mac_vap, pst_bss_dscr->auc_bssid);
    if (ul_ret == OAL_TRUE) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* 检查漫游到其它BSSID */
    if (pst_roam_info->en_current_bss_ignore == OAL_TRUE) {
        /* 排除当前bss的rssi值计算，本地已经保存了dmac上报的rssi */
        if (0 == oal_compare_mac_addr(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid)) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }

    if (OAL_SUCC != hmac_roam_check_cipher_limit(pst_roam_info, pst_bss_dscr)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    if (IS_ROAM_HOMENETWORK_2GTO5G_CONDITION(pst_hmac_vap, pst_roam_info, pst_bss_dscr)) {
        pst_roam_alg->c_max_rssi = pst_bss_dscr->c_rssi;
        pst_roam_alg->pst_max_rssi_bss = pst_bss_dscr;
        return OAL_SUCC;
    }

    c_tmp_rssi = pst_bss_dscr->c_rssi;
    // 终端评审: 2.4G候选AP小于-80dB不再漫游，5G候选AP小于-78dB不再漫游
    if (IS_ROAM_WEAK_RSSI_CONDITION(c_tmp_rssi, pst_bss_dscr)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        c_tmp_rssi = hmac_roam_alg_adjust_5G_rssi_weight(c_tmp_rssi);
    }

    /* 支持ax 加分处理 */
    if (pst_bss_dscr->en_he_capable == OAL_TRUE) {
        c_tmp_rssi = hmac_roam_alg_adjust_ax_rssi_weight(c_tmp_rssi);
    }

    /* c_current_rssi为0时，表示linkloss上报的触发，不需要考虑rssi增益 */
    s_delta_rssi = hmac_roam_alg_compare_rssi_increase(pst_roam_info, pst_bss_dscr, c_tmp_rssi);
    if (s_delta_rssi <= 0) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    ul_avail_channel_capacity = hmac_roam_alg_calc_avail_channel_capacity(pst_bss_dscr);
    if ((ul_avail_channel_capacity != 0) &&
        ((pst_roam_alg->pst_max_capacity_bss == OAL_PTR_NULL) ||
         (ul_avail_channel_capacity > pst_roam_alg->ul_max_capacity))) {
        // 暂时不考虑容量
        // pst_roam_alg->ul_max_capacity      = ul_avail_channel_capacity;
        // pst_roam_alg->pst_max_capacity_bss = pst_bss_dscr;
    }

    /* 对于已存在pmk缓存的bss进行加分处理 */
    puc_pmkid = hmac_vap_get_pmksa(pst_hmac_vap, pst_bss_dscr->auc_bssid);
    if (puc_pmkid != OAL_PTR_NULL) {
        c_tmp_rssi += ROAM_RSSI_DIFF_4_DB;
    }

    if (hmac_roam_alg_compare_bss(pst_hmac_vap, pst_roam_alg, pst_bss_dscr, c_tmp_rssi)) {
        pst_roam_alg->c_max_rssi = c_tmp_rssi;
        pst_roam_alg->pst_max_rssi_bss = pst_bss_dscr;
    }

    /* 当2G AP RSSI和5G AP加权RSSI相等时，优选5G低band AP */
    if ((c_tmp_rssi == pst_roam_alg->c_max_rssi) &&
        (pst_roam_alg->pst_max_rssi_bss->st_channel.en_band == WLAN_BAND_2G) &&
        (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G)) {
        pst_roam_alg->c_max_rssi = c_tmp_rssi;
        pst_roam_alg->pst_max_rssi_bss = pst_bss_dscr;
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_roam_alg_need_to_stop_roam_trigger(hmac_roam_info_stru *pst_roam_info)
{
    hmac_vap_stru *pst_hmac_vap = NULL;
    hmac_roam_alg_stru *pst_roam_alg = NULL;

    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    pst_roam_alg = &(pst_roam_info->st_alg);

    if (pst_roam_alg->uc_another_bss_scaned) {
        pst_roam_alg->uc_invalid_scan_cnt = 0xff;
        return OAL_FALSE;
    }

    if (pst_roam_alg->uc_invalid_scan_cnt == 0xff) {
        return OAL_FALSE;
    }

    if (pst_roam_alg->uc_invalid_scan_cnt++ > 4) {
        pst_roam_alg->uc_invalid_scan_cnt = 0xff;
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


mac_bss_dscr_stru *hmac_roam_alg_select_bss(hmac_roam_info_stru *pst_roam_info)
{
    hmac_vap_stru *pst_hmac_vap = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    mac_bss_dscr_stru *pst_bss_dscr = NULL;
    hmac_roam_alg_stru *pst_roam_alg = NULL;
    int16_t s_delta_rssi;

    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    pst_roam_alg = &(pst_roam_info->st_alg);

    /* 取得最大 rssi 的 bss */
    pst_bss_dscr = pst_roam_alg->pst_max_rssi_bss;

    if ((pst_roam_alg->pst_max_capacity_bss != OAL_PTR_NULL) &&
        (pst_roam_alg->ul_max_capacity >= ROAM_THROUGHPUT_THRESHOLD)) {
        /* 取得最大 capacity 的 bss */
        pst_bss_dscr = pst_roam_alg->pst_max_capacity_bss;
    }

    if (pst_bss_dscr == OAL_PTR_NULL) {
        /* should not be here */
        return OAL_PTR_NULL;
    }

    if (pst_roam_info->st_alg.st_home_network.uc_is_homenetwork == OAL_TRUE &&
        pst_roam_info->en_roam_trigger == ROAM_TRIGGER_HOME_NETWORK) {
        /* 5g优选，选中的5g bssid */
        if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
            /*lint -e571*/
            oam_warning_log4(0, OAM_SF_ROAM,
                "{hmac_roam_alg_select_bss::roam_to 5G home network AP, max_rssi=%d, [%02X:XX:XX:XX:%02X:%02X]}",
                pst_roam_alg->c_max_rssi, pst_bss_dscr->auc_bssid[0],
                pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]);
            /*lint +e571*/
            return pst_bss_dscr;
        }
    }

    /* rssi增益处理 */
    s_delta_rssi = hmac_roam_alg_compare_rssi_increase(pst_roam_info, pst_bss_dscr, pst_roam_alg->c_max_rssi);
    if (s_delta_rssi <= 0) {
        return OAL_PTR_NULL;
    }

    /*lint -e571*/
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        oam_warning_log4(0, OAM_SF_ROAM,
                         "{hmac_roam_alg_select_bss::roam_to 5G candidate AP, max_rssi=%d, [%02X:XX:XX:XX:%02X:%02X]}",
                         pst_roam_alg->c_max_rssi, pst_bss_dscr->auc_bssid[0],
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]);
    } else {
        oam_warning_log4(0, OAM_SF_ROAM,
                         "{hmac_roam_alg_select_bss::roam_to candidate AP, max_rssi=%d, [%02X:XX:XX:XX:%02X:%02X]}",
                         pst_roam_alg->c_max_rssi, pst_bss_dscr->auc_bssid[0],
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]);
    }

    /*lint +e571*/
    return pst_bss_dscr;
}
