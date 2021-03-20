
#ifdef _PRE_WLAN_FEATURE_ROAM
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "oam_ext_if.h"
#include "oal_schedule.h"
#include "mac_ie.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "dmac_ext_if.h"
#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#include "hmac_roam_alg.h"
#include "hmac_mgmt_sta.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_ALG_C

#define CURRENT_2G_TARGET_5G(_pst_mac_vap, _pst_bss_dscr) (((_pst_mac_vap->st_channel.en_band) == WLAN_BAND_2G) && ((_pst_bss_dscr->st_channel.en_band) == WLAN_BAND_5G))
#define CURRENT_5G_TARGET_2G(_pst_mac_vap, _pst_bss_dscr) (((_pst_mac_vap->st_channel.en_band) == WLAN_BAND_5G) && ((_pst_bss_dscr->st_channel.en_band) == WLAN_BAND_2G))

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
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

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

oal_void hmac_roam_alg_init(hmac_roam_info_stru *pst_roam_info, oal_int8 c_current_rssi)
{
    hmac_roam_alg_stru *pst_roam_alg;

    if (pst_roam_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_init::param null.}");
        return;
    }
    /*lint -e571*/
    OAM_WARNING_LOG1(0, OAM_SF_ROAM, "{hmac_roam_alg_init c_current_rssi = %d.}", c_current_rssi);
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
    /* �״ι���ʱ��ʼ�� pst_roam_alg->uc_invalid_scan_cnt   = 0x0; */
    pst_roam_alg->pst_max_rssi_bss = OAL_PTR_NULL;

    return;
}


OAL_STATIC oal_int8 hmac_roam_alg_adjust_5G_rssi_weight(oal_int8 c_original_rssi)
{
    oal_int8 c_current_rssi = c_original_rssi;
    if (c_current_rssi >= ROAM_RSSI_NE65_DB) {
        c_current_rssi += ROAM_RSSI_DIFF_20_DB;
    } else if ((c_current_rssi < ROAM_RSSI_NE65_DB) && (c_current_rssi >= ROAM_RSSI_NE72_DB)) {
        c_current_rssi += ROAM_RSSI_DIFF_10_DB;
    } else {
        c_current_rssi += ROAM_RSSI_DIFF_4_DB;
    }

    return c_current_rssi;
}


OAL_STATIC oal_int16 hmac_roam_alg_compare_rssi_increase(hmac_roam_info_stru *pst_roam_info,
                                                         mac_bss_dscr_stru *pst_bss_dscr, oal_int8 c_target_weight_rssi)
{
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap;
    oal_int16 s_current_rssi;
    oal_int16 s_target_rssi;
    oal_uint8 uc_delta_rssi;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (OAL_UNLIKELY(pst_hmac_vap == OAL_PTR_NULL)) {
        return -1;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    s_target_rssi = pst_bss_dscr->c_rssi;
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        uc_delta_rssi = pst_roam_info->st_config.uc_delta_rssi_5G;
    } else {
        uc_delta_rssi = pst_roam_info->st_config.uc_delta_rssi_2G;
    }

    /* current AP is 2G, target AP is 5G */
    if (CURRENT_2G_TARGET_5G(pst_mac_vap, pst_bss_dscr)) {
        s_target_rssi = c_target_weight_rssi;
    }
    s_current_rssi = pst_roam_info->st_alg.c_current_rssi;
    /* current AP is 5G, target AP is 2G */
    if (CURRENT_5G_TARGET_2G(pst_mac_vap, pst_bss_dscr)) {
        s_current_rssi = (oal_int16)hmac_roam_alg_adjust_5G_rssi_weight((oal_int8)s_current_rssi);
    }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if ((pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) && (pst_bss_dscr->c_rssi >= ROAM_RSSI_NE70_DB)) {
        return 1;
    }
#endif
    /* ָ��BSSID���� */
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_BSSID) {
        return 1;
    }

    if (uc_delta_rssi < ROAM_RSSI_DIFF_4_DB) {
        uc_delta_rssi = ROAM_RSSI_DIFF_4_DB;
    }

    if (s_current_rssi >= ROAM_RSSI_NE70_DB) {
        return (s_target_rssi - s_current_rssi - (oal_int16)uc_delta_rssi);
    }

    if (uc_delta_rssi >= ROAM_RSSI_DIFF_4_DB + 2) { /* ����2DB��4DB */
        uc_delta_rssi -= 2;
    }

    if (s_current_rssi >= ROAM_RSSI_NE75_DB) {
        return (s_target_rssi - s_current_rssi - (oal_int16)uc_delta_rssi);
    }

    if (uc_delta_rssi >= ROAM_RSSI_DIFF_4_DB + 2) { /* ����2DB��4DB */
        uc_delta_rssi -= 2;
    }

    if (s_current_rssi >= ROAM_RSSI_NE80_DB) {
        return (s_target_rssi - s_current_rssi - (oal_int16)uc_delta_rssi);
    }

    return (s_target_rssi - s_current_rssi - ROAM_RSSI_DIFF_4_DB);
}


OAL_STATIC oal_uint32 hmac_roam_alg_add_bsslist(hmac_roam_bss_list_stru *pst_roam_bss_list, oal_uint8 *puc_bssid,
                                                roam_blacklist_type_enum_uint8 list_type)
{
    hmac_roam_bss_info_stru *pst_cur_bss;
    hmac_roam_bss_info_stru *pst_oldest_bss;
    hmac_roam_bss_info_stru *pst_zero_bss;
    oal_uint8 auc_mac_zero[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_uint32 ul_current_index;
    oal_uint32 ul_now;
    oal_uint32 ul_timeout;

    pst_oldest_bss = OAL_PTR_NULL;
    pst_zero_bss = OAL_PTR_NULL;
    ul_now = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    for (ul_current_index = 0; ul_current_index < ROAM_LIST_MAX; ul_current_index++) {
        pst_cur_bss = &pst_roam_bss_list->ast_bss[ul_current_index];
        ul_timeout = (oal_uint32)pst_cur_bss->ul_timeout;
        if (oal_compare_mac_addr(pst_cur_bss->auc_bssid, puc_bssid) == 0) {
            /* ���Ȳ����Ѵ��ڵļ�¼�����������ʱ����ʱ������������count */
            if (OAL_TIME_GET_RUNTIME(pst_cur_bss->ul_time_stamp, ul_now) > ul_timeout) {
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

        /* ��¼��һ���ռ�¼ */
        if (pst_zero_bss != OAL_PTR_NULL) {
            continue;
        }

        if (oal_compare_mac_addr(pst_cur_bss->auc_bssid, auc_mac_zero) == 0) {
            pst_zero_bss = pst_cur_bss;
            continue;
        }

        /* ��¼һ���ǿ����ϼ�¼ */
        if (pst_oldest_bss == OAL_PTR_NULL) {
            pst_oldest_bss = pst_cur_bss;
        } else {
            if (OAL_TIME_GET_RUNTIME(pst_cur_bss->ul_time_stamp, ul_now) >
                OAL_TIME_GET_RUNTIME(pst_oldest_bss->ul_time_stamp, ul_now)) {
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
                                                             oal_uint8 *puc_bssid)
{
    hmac_roam_bss_info_stru *pst_cur_bss;
    oal_uint32 ul_current_index;
    oal_uint32 ul_now;
    oal_uint32 ul_timeout;
    oal_uint32 ul_delta_time;
    oal_uint16 us_count_limit;

    ul_now = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    for (ul_current_index = 0; ul_current_index < ROAM_LIST_MAX; ul_current_index++) {
        pst_cur_bss = &pst_roam_bss_list->ast_bss[ul_current_index];
        ul_timeout = pst_cur_bss->ul_timeout;
        us_count_limit = pst_cur_bss->us_count_limit;

        if (oal_compare_mac_addr(pst_cur_bss->auc_bssid, puc_bssid) == 0) {
            /* ����ڳ�ʱʱ���ڳ���count_limit�����ϼ�¼ */
            ul_delta_time = OAL_TIME_GET_RUNTIME(pst_cur_bss->ul_time_stamp, ul_now);
            if ((ul_delta_time <= ul_timeout) &&
                (pst_cur_bss->us_count >= us_count_limit)) {
                return OAL_TRUE;
            }
            return OAL_FALSE;
        }
    }

    return OAL_FALSE;
}


oal_uint32 hmac_roam_alg_add_blacklist(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid,
                                       roam_blacklist_type_enum_uint8 list_type)
{
    oal_uint32 ul_ret;
    if ((pst_roam_info == OAL_PTR_NULL) || (puc_bssid == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_roam_alg_add_bsslist(&pst_roam_info->st_alg.st_blacklist, puc_bssid, list_type);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ROAM, "{hmac_roam_alg_add_blacklist::hmac_roam_alg_add_list failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_roam_alg_find_in_blacklist(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid)
{
    if ((pst_roam_info == OAL_PTR_NULL) || (puc_bssid == OAL_PTR_NULL)) {
        return OAL_FALSE;
    }

    return hmac_roam_alg_find_in_bsslist(&pst_roam_info->st_alg.st_blacklist, puc_bssid);
}


oal_uint32 hmac_roam_alg_add_history(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid)
{
    oal_uint32 ul_ret;

    if ((pst_roam_info == OAL_PTR_NULL) || (puc_bssid == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_roam_alg_add_bsslist(&pst_roam_info->st_alg.st_history, puc_bssid, ROAM_BLACKLIST_TYPE_NORMAL_AP);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ROAM, "{hmac_roam_alg_add_history::hmac_roam_alg_add_list failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_roam_alg_find_in_history(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid)
{
    if ((pst_roam_info == OAL_PTR_NULL) || (puc_bssid == OAL_PTR_NULL)) {
        return OAL_FALSE;
    }

    return hmac_roam_alg_find_in_bsslist(&pst_roam_info->st_alg.st_history, puc_bssid);
}
OAL_STATIC oal_void hmac_roam_alg_scan_channel_2g_init(hmac_roam_info_stru *pst_roam_info,
    mac_scan_req_stru *pst_scan_params)
{
    oal_uint8 uc_chan_idx;
    oal_uint8 uc_chan_number;
    oal_uint32 ul_ret;

    if (pst_roam_info->st_config.uc_scan_band & ROAM_BAND_2G_BIT) {
        switch (pst_roam_info->st_config.uc_scan_orthogonal) {
            case ROAM_SCAN_CHANNEL_ORG_3: {
                pst_scan_params->ast_channel_list[0].uc_chan_number = 1;
                pst_scan_params->ast_channel_list[0].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[0].uc_idx = 0;

                pst_scan_params->ast_channel_list[1].uc_chan_number = 6; /* ��1���ŵ��б����20MHz�ŵ�����6 */
                pst_scan_params->ast_channel_list[1].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[1].uc_idx = 5; /* ��1���ŵ��б���ŵ���������5 */

                pst_scan_params->ast_channel_list[2].uc_chan_number = 11; /* ��2���ŵ��б����20MHz�ŵ�����11 */
                pst_scan_params->ast_channel_list[2].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[2].uc_idx = 10; /* ��2���ŵ��б���ŵ���������10 */

                pst_scan_params->uc_channel_nums = 3; /* �ŵ��б����ŵ��ĸ�����3 */
                pst_scan_params->uc_max_scan_count_per_channel = 1;
                break;
            }
            case ROAM_SCAN_CHANNEL_ORG_4: {
                pst_scan_params->ast_channel_list[0].uc_chan_number = 1;
                pst_scan_params->ast_channel_list[0].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[0].uc_idx = 0;

                pst_scan_params->ast_channel_list[1].uc_chan_number = 5; /* ��1���ŵ��б����20MHz�ŵ�����5 */
                pst_scan_params->ast_channel_list[1].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[1].uc_idx = 4; /* ��1���ŵ��б���ŵ���������4 */

                pst_scan_params->ast_channel_list[2].uc_chan_number = 7; /* ��2���ŵ��б����20MHz�ŵ�����7 */
                pst_scan_params->ast_channel_list[2].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[2].uc_idx = 6; /* ��2���ŵ��б���ŵ���������6 */

                pst_scan_params->ast_channel_list[3].uc_chan_number = 13; /* ��3���ŵ��б����20MHz�ŵ�����13 */
                pst_scan_params->ast_channel_list[3].en_band = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[3].uc_idx = 12; /* ��3���ŵ��б���ŵ���������12 */

                pst_scan_params->uc_channel_nums = 4; /* �ŵ��б����ŵ��ĸ�����4 */
                pst_scan_params->uc_max_scan_count_per_channel = 1;
                break;
            }
            default:
            {
                for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_chan_idx++) {
                    ul_ret = mac_is_channel_idx_valid(WLAN_BAND_2G, uc_chan_idx);
                    if (ul_ret != OAL_SUCC) {
                        continue;
                    }
                    mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_chan_idx, &uc_chan_number);
                    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_number = uc_chan_number;
                    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].en_band = WLAN_BAND_2G;
                    pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_idx = uc_chan_idx;
                    pst_scan_params->uc_channel_nums++;
                }
                break;
            }
        }
    }
}
OAL_STATIC oal_void hmac_roam_alg_scan_channel_5g_init(hmac_roam_info_stru *pst_roam_info,
    mac_scan_req_stru *pst_scan_params)
{
    oal_uint8 uc_chan_idx;
    oal_uint8 uc_chan_number;
    oal_uint32 ul_ret;

    if (pst_roam_info->st_config.uc_scan_band & ROAM_BAND_5G_BIT) {
        for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_chan_idx++) {
            ul_ret = mac_is_channel_idx_valid(WLAN_BAND_5G, uc_chan_idx);
            if (ul_ret != OAL_SUCC) {
                continue;
            }
            mac_get_channel_num_from_idx(WLAN_BAND_5G, uc_chan_idx, &uc_chan_number);
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_chan_number = uc_chan_number;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].en_band = WLAN_BAND_5G;
            pst_scan_params->ast_channel_list[pst_scan_params->uc_channel_nums].uc_idx = uc_chan_idx;
            pst_scan_params->uc_channel_nums++;
        }
    }
}

oal_uint32 hmac_roam_alg_scan_channel_init(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params)
{
    hmac_vap_stru *pst_hmac_vap;
    mac_channel_stru *pst_channel;
    oal_uint32 ul_ret;

    if ((pst_roam_info == OAL_PTR_NULL) || (pst_scan_params == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_scan_channel_init::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_scan_params->uc_channel_nums = 0;
    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "hmac_roam_alg_scan_channel_init:pst_hmac_vap is null");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_channel = &(pst_hmac_vap->st_vap_base_info.st_channel);

    if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_1) {
        OAM_INFO_LOG3(0, OAM_SF_ROAM,
                      "{hmac_roam_alg_scan_channel_init::scan one channel: ChanNum[%d], Band[%d], ChanIdx[%d]}",
                      pst_channel->uc_chan_number, pst_channel->en_band, pst_channel->uc_idx);
        if (pst_roam_info->en_roam_trigger != ROAM_TRIGGER_11V) {
            pst_scan_params->ast_channel_list[0].uc_chan_number = pst_channel->uc_chan_number;
            pst_scan_params->ast_channel_list[0].en_band = pst_channel->en_band;
            pst_scan_params->ast_channel_list[0].uc_idx = pst_channel->uc_idx;
        }
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
        /* ֧��11vЭ��ʱ���ڱ�ѡ�б������ŵ�ɨ�� */
        else {
            pst_scan_params->ast_channel_list[0].uc_chan_number = pst_roam_info->st_bsst_rsp_info.uc_chl_num;
            pst_scan_params->ast_channel_list[0].en_band =
                mac_get_band_by_channel_num(pst_roam_info->st_bsst_rsp_info.uc_chl_num);
            ul_ret = mac_get_channel_idx_from_num(pst_scan_params->ast_channel_list[0].en_band,
                                                  pst_scan_params->ast_channel_list[0].uc_chan_number,
                                                  &(pst_scan_params->ast_channel_list[0].uc_idx));
            /* ά�� */
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_SCAN, "hmac_roam_alg_scan_channel_init::get_channel_idx_from_num fail=%d",
                                 ul_ret);
            }
        }
#endif
        pst_scan_params->uc_channel_nums = 1;
        pst_scan_params->uc_max_scan_count_per_channel = 1;
        return OAL_SUCC;
    }
    hmac_roam_alg_scan_channel_2g_init(pst_roam_info, pst_scan_params);
    hmac_roam_alg_scan_channel_5g_init(pst_roam_info, pst_scan_params);
    pst_scan_params->uc_max_scan_count_per_channel = 2;  /* ÿ���ŵ���ɨ�������2 */

    return OAL_SUCC;
}

oal_uint32 hmac_roam_alg_check_bss_is_valid(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    oal_int8 c_tmp_rssi;
    oal_int16 s_delta_rssi;

    c_tmp_rssi = pst_bss_dscr->c_rssi;

    if (hmac_roam_check_cipher_limit(pst_roam_info, pst_bss_dscr) != OAL_SUCC) {
        OAM_WARNING_LOG2(0, OAM_SF_ROAM, "hmac_roam_alg_check_bss_is_valid:0x%02x:0x%02x cipher limit, ignore roaming",
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]); /* auc_bssid��4��5byteΪ���������ӡ */
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* �ն�����:AP�źź���ʱ���������Ρ���������ʱ��̫�� */
    /* 2.4G��ѡAPС��-80db�������Σ�5G��ѡAPС��-78dB�������� */
    if (((c_tmp_rssi < ROAM_RSSI_NE80_DB) && (pst_bss_dscr->st_channel.en_band == WLAN_BAND_2G)) ||
        ((c_tmp_rssi < ROAM_RSSI_NE78_DB) && (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G))) {
        /*lint -e571*/
        OAM_WARNING_LOG3(0, OAM_SF_ROAM,
                         "hmac_roam_alg_check_bss_is_valid:candidate 0x%02x:0x%02x rssi=%d, ignore roaming.}",
                         /* auc_bssid��4��5byteΪ���������ӡ */
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5], c_tmp_rssi);
        /*lint +e571*/
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        c_tmp_rssi = hmac_roam_alg_adjust_5G_rssi_weight(c_tmp_rssi);
    }

    /* c_current_rssiΪ0ʱ����ʾlinkloss�ϱ��Ĵ���������Ҫ����rssi���� */
    s_delta_rssi = hmac_roam_alg_compare_rssi_increase(pst_roam_info, pst_bss_dscr, c_tmp_rssi);
    if (s_delta_rssi <= 0) {
        /*lint -e571*/
        OAM_WARNING_LOG3(0, OAM_SF_ROAM, "hmac_roam_alg_check_bss_is_valid:0x%02x:0x%02xdelta rssi[%d], ignore roaming",
                         /* auc_bssid��4��5byteΪ���������ӡ */
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5], s_delta_rssi);
        /*lint +e571*/
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_roam_alg_bss_in_ess(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap;
    hmac_roam_alg_stru *pst_roam_alg;
    mac_cfg_ssid_param_stru st_cfg_ssid;
    oal_uint8 uc_stru_len;
    oal_uint32 ul_ret;

    if ((pst_roam_info == OAL_PTR_NULL) || (pst_bss_dscr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_in_ess::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_mib_get_ssid (pst_mac_vap, &uc_stru_len, (oal_uint8 *)(&st_cfg_ssid));

    if ((OAL_STRLEN(pst_bss_dscr->ac_ssid) != st_cfg_ssid.uc_ssid_len) ||
        (oal_memcmp(st_cfg_ssid.ac_ssid, pst_bss_dscr->ac_ssid, st_cfg_ssid.uc_ssid_len) != 0)) {
        return OAL_SUCC;
    }
    pst_roam_alg = &(pst_roam_info->st_alg);

    /* �Ƿ�ɨ�赽�˵�ǰ������ bss, ����λ�������� */
    if (oal_compare_mac_addr(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid) != 0) {
        pst_roam_alg->uc_another_bss_scaned = 1;

        if (pst_roam_info->uc_rssi_ignore == OAL_TRUE) {
            ul_ret = hmac_roam_alg_check_bss_is_valid(pst_roam_info, pst_bss_dscr);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_in_ess::no bss valid, ignore roaming.}");
            } else {
                hmac_roam_ignore_rssi_trigger(pst_hmac_vap, OAL_FALSE);
            }
        } else {
            hmac_roam_ignore_rssi_trigger(pst_hmac_vap, OAL_FALSE);
        }
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SAE

OAL_STATIC oal_bool_enum_uint8 hmac_only_support_sae(oal_uint8 *puc_rsn_akm)
{
    return ((puc_rsn_akm[0] == WLAN_AUTH_SUITE_SAE_SHA256) && ((puc_rsn_akm[1] == 0xFF) || puc_rsn_akm[1] == 0)) ||
           ((puc_rsn_akm[1] == WLAN_AUTH_SUITE_SAE_SHA256) && ((puc_rsn_akm[0] == 0xFF) || puc_rsn_akm[0] == 0));
}


OAL_STATIC oal_bool_enum_uint8 hmac_check_illegal_sae_roam(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_bss_rsn_akm)
{
    oal_uint8 auc_sta_rsn_akm[MAC_AUTHENTICATION_SUITE_NUM] = { 0 };

    /* ������ͬakm, �������� */
    if (mac_mib_rsn_akm_match_suites(pst_mac_vap, puc_bss_rsn_akm, MAC_AUTHENTICATION_SUITE_NUM) != 0) {
        /* not illegal */
        return OAL_FALSE;
    }

    mac_mib_get_rsn_akm_suites(pst_mac_vap, auc_sta_rsn_akm);

    /* ����ͬakm, ��ĳһ��ֻ֧��SAE, ˵����һ�˲�֧��SAE, ���������� */
    return hmac_only_support_sae(puc_bss_rsn_akm) || hmac_only_support_sae(auc_sta_rsn_akm);
}
#endif

/*
 * �� �� �� : hmac_roam_check_cipher_limit
 * �������� : ���Ŀ��BSS�����׼��Ƿ�֧������
 */
oal_uint32 hmac_roam_check_cipher_limit(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    mac_vap_stru *pst_mac_vap;
    mac_cap_info_stru *pst_cap_info;
#if defined(_PRE_WLAN_FEATURE_SAE) && defined(_PRE_WLAN_FEATURE_11R)
    oal_uint8 *puc_mde = OAL_PTR_NULL;
#endif

    pst_mac_vap = &pst_roam_info->pst_hmac_vap->st_vap_base_info;
    pst_cap_info = (mac_cap_info_stru *)&pst_bss_dscr->us_cap_info;

    /* wep��bssֱ�ӹ��˵� */
    pst_cap_info = (mac_cap_info_stru *)&pst_bss_dscr->us_cap_info;
    if ((pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode == 0) &&
        (pst_cap_info->bit_privacy != 0)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* open���ܷ�ʽ��wpa/wpa2ֱ�ӹ��˵� */
    if ((pst_cap_info->bit_privacy == 0) != (mac_mib_get_privacyinvoked(pst_mac_vap) != OAL_TRUE)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    /* ��֧��SAE�������κη�SAE��֤��ʽ������ */
    if (hmac_check_illegal_sae_roam(pst_mac_vap, pst_bss_dscr->st_bss_sec_info.auc_rsn_auth_policy)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_check_cipher_limit::Roaming between SAE-only AP and non-SAE AP is not allowed}");

        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

#ifdef _PRE_WLAN_FEATURE_11R
    /* SAE��֧��11R���� */
    if ((pst_roam_info->pst_hmac_vap->en_auth_mode == WLAN_WITP_AUTH_SAE) &&
        (pst_bss_dscr->ul_mgmt_len > (MAC_80211_FRAME_LEN + MAC_SSID_OFFSET))) {
        puc_mde = mac_find_ie(MAC_EID_MOBILITY_DOMAIN,
                              pst_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN + MAC_SSID_OFFSET, /*lint !e416*/
                              pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN - MAC_SSID_OFFSET);
        if (puc_mde) {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_roam_check_cipher_limit::SAE do not support 11r roam");

            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }
#endif
#endif
    return OAL_SUCC;
}

oal_uint32 hmac_roam_alg_check_bssid_limit(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_mac_addr)
{
    oal_uint32 ul_ret;

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        if (oal_memcmp(pst_roam_info->st_bsst_rsp_info.auc_target_bss_addr,
                       puc_mac_addr,
                       WLAN_MAC_ADDR_LEN)) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */

    /* �������� */
    ul_ret = hmac_roam_alg_find_in_blacklist(pst_roam_info, puc_mac_addr);
    if (ul_ret == OAL_TRUE) {
        OAM_WARNING_LOG3(0, OAM_SF_ROAM, "{hmac_roam_alg_check_bssid_limit:: [%02X:XX:XX:XX:%02X:%02X] in blacklist!}",
                         puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]); /* puc_mac_addr��0��4��5byteΪ���������ӡ */
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_roam_alg_bss_check(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru              *pst_hmac_vap;
    mac_vap_stru               *pst_mac_vap;
    hmac_roam_alg_stru         *pst_roam_alg;
    oal_uint8                  *puc_pmkid;
    mac_cfg_ssid_param_stru     st_cfg_ssid;
    oal_uint32                  ul_ret;
    oal_uint8                   uc_stru_len;
    oal_int8                    c_tmp_rssi;

    if (OAL_ANY_NULL_PTR3(pst_roam_info, pst_bss_dscr, pst_roam_info->pst_hmac_vap)) {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_check::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_mib_get_ssid (pst_mac_vap, &uc_stru_len, (oal_uint8 *)(&st_cfg_ssid));

    if ((OAL_STRLEN(pst_bss_dscr->ac_ssid) != st_cfg_ssid.uc_ssid_len) ||
        (oal_memcmp(st_cfg_ssid.ac_ssid, pst_bss_dscr->ac_ssid, st_cfg_ssid.uc_ssid_len) != 0)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }
    pst_roam_alg = &(pst_roam_info->st_alg);

    /* ������ε��Լ�:δɨ�赽�Լ�������ʧ�� */
    if ((pst_roam_info->en_current_bss_ignore == OAL_FALSE)
        && oal_memcmp(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid, OAL_MAC_ADDR_LEN)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* ����ض�BSSID������ */
    if ((oal_memcmp(pst_roam_info->auc_target_bssid, pst_bss_dscr->auc_bssid, OAL_MAC_ADDR_LEN) != 0) &&
        (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_BSSID)) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    if (pst_roam_info->en_current_bss_ignore == OAL_TRUE) {
        /* ��voe��֤��ѡHMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIREʱ�����ڵ���Чbss */
        /* 11Rʱ���������ɨ��ʱ��bss�ϱ���wpas������bssѡ�񶼻�ֻѡ��ǰʱ��֮ǰ���5s�ڵģ����߼��ᵼ������ʱ�Ҳ������õ�bss����������ʧ�� */
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
        if (pst_hmac_vap->bit_voe_11r_auth == 0)
#endif
        {
            if (oal_time_after32(OAL_TIME_GET_STAMP_MS(),
                                 (pst_bss_dscr->ul_timestamp + HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE))) {
                OAM_WARNING_LOG2(0, OAM_SF_ROAM,
                                 "{hmac_roam_alg_bss_check::Over the expire time!ul_timestamp[%d],now time[%d].}",
                                 pst_bss_dscr->ul_timestamp, OAL_TIME_GET_STAMP_MS());
                return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
            }
        }
    }
    ul_ret = hmac_roam_alg_check_bssid_limit(pst_roam_info, pst_bss_dscr->auc_bssid);
    if (ul_ret != OAL_SUCC) {
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    /* ������ε�����BSSID */
    if (pst_roam_info->en_current_bss_ignore == OAL_TRUE) {
        /* �ų���ǰbss��rssiֵ���㣬�����Ѿ�������dmac�ϱ���rssi */
        if (oal_compare_mac_addr(pst_mac_vap->auc_bssid, pst_bss_dscr->auc_bssid) == 0) {
            return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
        }
    }

    ul_ret = hmac_roam_alg_check_bss_is_valid(pst_roam_info, pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ROAM, "{hmac_roam_alg_bss_check::no bss valid, ignore roaming.}");
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    c_tmp_rssi = pst_bss_dscr->c_rssi;
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        c_tmp_rssi = hmac_roam_alg_adjust_5G_rssi_weight(c_tmp_rssi);
    }
    /* �����Ѵ���pmk�����bss���мӷִ��� */
    puc_pmkid = hmac_vap_get_pmksa(pst_hmac_vap, pst_bss_dscr->auc_bssid);
    if (puc_pmkid != OAL_PTR_NULL) {
        c_tmp_rssi += ROAM_RSSI_DIFF_4_DB;
    }

    if ((pst_roam_alg->pst_max_rssi_bss == OAL_PTR_NULL) ||
        (c_tmp_rssi > pst_roam_alg->c_max_rssi)) {
        pst_roam_alg->c_max_rssi = c_tmp_rssi;
        pst_roam_alg->pst_max_rssi_bss = pst_bss_dscr;
    }

    /* ��2G AP RSSI��5G AP��ȨRSSI���ʱ����ѡ5G��band AP */
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
    hmac_vap_stru *pst_hmac_vap;
    hmac_roam_alg_stru *pst_roam_alg;

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
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap;
    mac_bss_dscr_stru *pst_bss_dscr;
    hmac_roam_alg_stru *pst_roam_alg;
    oal_int16 s_delta_rssi;

    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    pst_roam_alg = &(pst_roam_info->st_alg);

    /* ȡ����� rssi �� bss */
    pst_bss_dscr = pst_roam_alg->pst_max_rssi_bss;

    if ((pst_roam_alg->pst_max_capacity_bss != OAL_PTR_NULL) &&
        (pst_roam_alg->ul_max_capacity >= ROAM_THROUGHPUT_THRESHOLD)) {
        /* ȡ����� capacity �� bss */
        pst_bss_dscr = pst_roam_alg->pst_max_capacity_bss;
    }

    if (pst_bss_dscr == OAL_PTR_NULL) {
        /* should not be here */
        return OAL_PTR_NULL;
    }

    /* rssi���洦�� */
    s_delta_rssi = hmac_roam_alg_compare_rssi_increase(pst_roam_info, pst_bss_dscr, pst_roam_alg->c_max_rssi);
    if (s_delta_rssi <= 0) {
        return OAL_PTR_NULL;
    }

    /*lint -e571*/
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        OAM_WARNING_LOG4(0, OAM_SF_ROAM,
                         "{hmac_roam_alg_select_bss::roam_to 5G candidate AP, max_rssi=%d, [%02X:XX:XX:XX:%02X:%02X]}",
                         pst_roam_alg->c_max_rssi, pst_bss_dscr->auc_bssid[0],
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]); /* auc_bssid��4��5byteΪ���������ӡ */
    } else {
        OAM_WARNING_LOG4(0, OAM_SF_ROAM,
                         "{hmac_roam_alg_select_bss::roam_to candidate AP, max_rssi=%d, [%02X:XX:XX:XX:%02X:%02X]}",
                         pst_roam_alg->c_max_rssi, pst_bss_dscr->auc_bssid[0],
                         pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]); /* auc_bssid��4��5byteΪ���������ӡ */
    }
    /*lint +e571*/
    return pst_bss_dscr;
}

#endif  // _PRE_WLAN_FEATURE_ROAM


