

#include "oal_ext_if.h"
#include "oal_net.h"
#include "mac_frame.h"
#include "mac_resource.h"
#include "mac_ie.h"
#include "mac_vap.h"
#include "mac_user.h"
#include "frw_ext_if.h"

#include "mac_resource.h"
#include "wlan_types.h"

#include "hmac_mgmt_bss_comm.h"
#include "hmac_11v.h"
#include "hmac_dfx.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#include "hmac_roam_alg.h"
#include "hmac_scan.h"
#include "hmac_p2p.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_MBO
#include "hmac_mbo.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11V_C

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE

uint32_t hmac_11v_roam_scan_check(hmac_vap_stru *hmac_vap)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_user_11v_ctrl_stru *user_11v_ctrl_info = NULL;
    hmac_roam_info_stru *roam_info = NULL;

    roam_info = (hmac_roam_info_stru *)hmac_vap->pul_roam_info;
    if (roam_info == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_11v_roam_scan_check::pst_roam_info IS NULL}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 获取发送端的用户指针 */
    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_11v_roam_scan_check::pst_hmac_user is NULL}");
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }
    user_11v_ctrl_info = &(hmac_user->st_11v_ctrl_info);

    if (user_11v_ctrl_info->mac_11v_callback_fn == NULL) {
        return OAL_SUCC;
    }

    if (user_11v_ctrl_info->uc_11v_roam_scan_times < MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT) {
        /* 触发指定信道扫描漫游 */
        user_11v_ctrl_info->uc_11v_roam_scan_times++;
        oam_warning_log3(0, OAM_SF_ANY,
            "{hmac_11v_roam_scan_check::Trig spec chan scan roam, 11v_roam_scan_times[%d],limit_times[%d].chan[%d]}",
            user_11v_ctrl_info->uc_11v_roam_scan_times, MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT,
            roam_info->st_bsst_rsp_info.uc_chl_num);
        roam_info->st_static.uc_scan_mode = HMAC_CHR_11V_SCAN;
        hmac_roam_start(hmac_vap, ROAM_SCAN_CHANNEL_ORG_5, OAL_TRUE, NULL, ROAM_TRIGGER_11V);
    } else if (user_11v_ctrl_info->uc_11v_roam_scan_times == MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT) {
        /* 触发全信道扫描漫游 */
        user_11v_ctrl_info->uc_11v_roam_scan_times++;
        roam_info->st_scan_h2d_params.st_scan_params.uc_bss_transition_process_flag = OAL_FALSE;
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_11v_roam_scan_check::Trigger ALL Channel scan roam.}");
        roam_info->st_static.uc_scan_mode = HMAC_CHR_NORMAL_SCAN;
        hmac_roam_start(hmac_vap, ROAM_SCAN_CHANNEL_ORG_DBDC, OAL_TRUE, NULL, ROAM_TRIGGER_11V);
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_MBO

OAL_STATIC void hmac_set_bss_re_assoc_delay_params(hmac_bsst_req_info_stru bsst_req_info,
    mac_user_stru *mac_user, hmac_vap_stru *hmac_vap)
{
    uint8_t bss_index_loop = 0;

    for (bss_index_loop = 0; bss_index_loop < bsst_req_info.uc_bss_list_num; bss_index_loop++) {
        if (!oal_memcmp(mac_user->auc_user_mac_addr,
            bsst_req_info.pst_neighbor_bss_list[bss_index_loop].auc_mac_addr, WLAN_MAC_ADDR_LEN)) {
            hmac_vap->st_vap_base_info.st_mbo_para_info.ul_re_assoc_delay_time =
                HMAC_11V_MBO_RE_ASSOC_DALAY_TIME_S_TO_MS *
                bsst_req_info.pst_neighbor_bss_list[bss_index_loop].st_assoc_delay_attr_mbo_ie.ul_re_assoc_delay_time;
            hmac_vap->st_vap_base_info.st_mbo_para_info.ul_btm_req_received_time =
                (uint32_t)oal_time_get_stamp_ms();
            hmac_vap->st_vap_base_info.st_mbo_para_info.en_disable_connect = OAL_TRUE;

            memcpy_s(hmac_vap->st_vap_base_info.st_mbo_para_info.auc_re_assoc_delay_bss_mac_addr,
                     WLAN_MAC_ADDR_LEN, mac_user->auc_user_mac_addr, WLAN_MAC_ADDR_LEN);
            break;
        }
    }
    return;
}
#endif


OAL_STATIC uint32_t hmac_rx_bsst_req_candidate_info_check(hmac_vap_stru *hmac_vap,
    uint8_t *channel, uint8_t *bssid)
{
    mac_bss_dscr_stru *bss_dscr = NULL;
    uint8_t candidate_channel = *channel;
    wlan_channel_band_enum_uint8 channel_band = mac_get_band_by_channel_num(candidate_channel);

    uint32_t check = mac_is_channel_num_valid(channel_band, candidate_channel);
    bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info, bssid);
    if (check != OAL_SUCC && bss_dscr == NULL) {
        /* 无效信道 */
        oam_warning_log3(0, OAM_SF_CFG,
            "{hmac_rx_bsst_req_candidate_info_check::chan[%d] invalid, bssid:XX:XX:XX:XX:%02X:%02X not in scan list}",
            candidate_channel, bssid[4], bssid[5]);
        return OAL_FAIL;
    }
    /* 有效 */
    if (bss_dscr != NULL && candidate_channel != bss_dscr->st_channel.uc_chan_number) {
        /* 纠正为实际信道 */
        *channel = bss_dscr->st_channel.uc_chan_number;
        oam_warning_log4(0, OAM_SF_CFG,
            "{hmac_rx_bsst_req_candidate_info_check::bssid:XX:XX:XX:XX:%02X:%02X in bssinfo channel[%d],not[%d]}",
            bssid[4], bssid[5],
            bss_dscr->st_channel.uc_chan_number, candidate_channel);
    }
    /* 双wlan模式，推荐bss的频段和当前关联频段不一致时，不采用该bss */
    channel_band = mac_get_band_by_channel_num(*channel);
    if (mac_is_dual_sta_mode() &&
        channel_band != hmac_vap->st_vap_base_info.st_channel.en_band) {
        oam_warning_log3(0, OAM_SF_CFG,
            "{hmac_rx_bsst_req_candidate_info_check::dual sta mode, ignore diff band[%d] bss[xx:xx:xx:xx:%02x:%02x]}",
            channel_band, bssid[4], bssid[5]); /* 打印bssid 4 5两个字节 */
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_rx_bsst_is_rejected(hmac_bsst_req_info_stru bsst_req_info,
    mac_user_stru *mac_user, mac_vap_stru *mac_vap)
{
    return (bsst_req_info.us_disassoc_time * mac_mib_get_BeaconPeriod(mac_vap) <
            HMAC_11V_REQUEST_DISASSOC_TIME_SCAN_ONE_CHANNEL_TIME
            && bsst_req_info.us_disassoc_time * mac_mib_get_BeaconPeriod(mac_vap) > 0);
}

OAL_STATIC void hmac_rx_bsst_free_res(hmac_bsst_req_info_stru *bsst_req_info)
{
    if (bsst_req_info->puc_session_url != NULL) {
        oal_mem_free_m(bsst_req_info->puc_session_url, OAL_TRUE);
        bsst_req_info->puc_session_url = NULL;
    }

    if (bsst_req_info->pst_neighbor_bss_list != NULL) {
        oal_mem_free_m(bsst_req_info->pst_neighbor_bss_list, OAL_TRUE);
        bsst_req_info->pst_neighbor_bss_list = NULL;
    }
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_rx_bsst_check_disassoc_time(uint16_t disassoc_time,
    uint32_t beacon_period)
{
    uint32_t disassoc_time_in_tu = disassoc_time * beacon_period;
    return disassoc_time_in_tu >= HMAC_11V_REQUEST_DISASSOC_TIME_SCAN_ONE_CHANNEL_TIME &&
           disassoc_time_in_tu < HMAC_11V_REQUEST_DISASSOC_TIME_SCAN_ALL_CHANNEL_TIME;
}

OAL_STATIC void hmac_11v_roam_check(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
    oal_bool_enum_uint8 *pen_need_roam, hmac_bsst_req_info_stru st_bsst_req_info)
{
    uint32_t ul_ret;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = OAL_PTR_NULL;
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    /* Signal Bridge disable 11v roaming */
    ul_ret = hmac_roam_check_signal_bridge(pst_hmac_vap);
    if (ul_ret != OAL_SUCC) {
        *pen_need_roam = OAL_FALSE;
    }

    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);
    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;

    if (*pen_need_roam == OAL_TRUE) {
#ifdef _PRE_WLAN_FEATURE_MBO
        hmac_set_bss_re_assoc_delay_params(st_bsst_req_info, &pst_hmac_user->st_user_base_info, pst_hmac_vap);
#endif
        pst_11v_ctrl_info->uc_11v_roam_scan_times = 1;
        hmac_roam_start(pst_hmac_vap, ROAM_SCAN_CHANNEL_ORG_5, OAL_TRUE, NULL, ROAM_TRIGGER_11V);
    } else {
        pst_roam_info->st_bsst_rsp_info.uc_status_code = WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES;
        hmac_tx_bsst_rsp_action(pst_hmac_vap, pst_hmac_user, &(pst_roam_info->st_bsst_rsp_info));
    }
}


OAL_STATIC void hmac_bsst_fill_chan_and_bssid_info(hmac_roam_info_stru *roam_info,
    mac_scan_req_stru *scan_params, hmac_neighbor_bss_info_stru *neighbor_bss_info, uint8_t bss_num)
{
    uint8_t bss_list_index;
    uint8_t channum;
    uint8_t channel_idx;
    wlan_channel_band_enum_uint8 band;
    hmac_neighbor_bss_info_stru *neighbor_bss = NULL;

    for (bss_list_index = 0; bss_list_index < bss_num; bss_list_index++) {
        neighbor_bss = &(neighbor_bss_info[bss_list_index]);
        if (neighbor_bss->valid == OAL_FALSE) {
            continue;
        }
        if (neighbor_bss->uc_max_candidate_perf != neighbor_bss->uc_candidate_perf) {
            oam_warning_log1(0, OAM_SF_ROAM,
                             "{hmac_bsst_fill_chan_and_bssid_info::candidate_pref=%d}",
                             neighbor_bss->uc_candidate_perf);
            continue;
        }

        channum = neighbor_bss->uc_chl_num;
        band = mac_get_band_by_channel_num(channum);
        if (mac_get_channel_idx_from_num(band, channum, &channel_idx) == OAL_SUCC) {
            hmac_roam_neighbor_report_add_chan(scan_params, channum, band, channel_idx);
        }

        hmac_roam_neighbor_report_add_bssid(roam_info, neighbor_bss->auc_mac_addr);
    }
}


OAL_STATIC void hmac_bsst_neighbor_fill_chan_bssid(hmac_roam_info_stru *roam_info,
    mac_scan_req_stru *scan_params, hmac_neighbor_bss_info_stru *neighbor_bss_info, uint8_t bss_num)
{
    if (oal_unlikely(oal_any_null_ptr3(roam_info, scan_params, neighbor_bss_info))) {
        oam_error_log0(0, OAM_SF_ROAM,
            "{hmac_bsst_neighbor_fill_chan_bssid:: pst_hmac_vap, pst_scan_params or pst_neighbor_bss_info is null}");
        return;
    }

    /* 将channel list和BSSID list相关统计信息清零 */
    scan_params->uc_channel_nums = 0;
    memset_s(&scan_params->ast_channel_list[0], WLAN_MAX_CHANNEL_NUM * sizeof(mac_channel_stru),
        0, WLAN_MAX_CHANNEL_NUM * sizeof(mac_channel_stru));

    roam_info->uc_neighbor_rpt_bssid_num = 0;
    memset_s(&roam_info->auc_neighbor_rpt_bssid[0][0], ROAM_NEIGHBOR_RPT_LIST_BSSID_MAX_NUM * WLAN_MAC_ADDR_LEN,
        0, ROAM_NEIGHBOR_RPT_LIST_BSSID_MAX_NUM * WLAN_MAC_ADDR_LEN);

    hmac_bsst_fill_chan_and_bssid_info(roam_info, scan_params, neighbor_bss_info, bss_num);
    scan_params->uc_bss_transition_process_flag = OAL_TRUE;

    oam_warning_log2(0, OAM_SF_ROAM,
                     "{hmac_bsst_neighbor_fill_chan_bssid::channel_nums=%d neighbor_rpt_bssid_num=%d}",
                     scan_params->uc_channel_nums, roam_info->uc_neighbor_rpt_bssid_num);
    /* 未解析到信道, BSST流程的标志位清零 */
    if (scan_params->uc_channel_nums == 0) {
        scan_params->uc_bss_transition_process_flag = OAL_FALSE;
    }
}

OAL_STATIC oal_bool_enum_uint8 hmac_bsst_check_bssid_channel(hmac_vap_stru *pst_hmac_vap,
    hmac_neighbor_bss_info_stru *neighbor_bss, mac_user_stru *pst_mac_user, mac_vap_stru *p2p_vap)
{
    uint32_t ul_ret;

    /* current connect or invalid AP */
    if (ether_is_broadcast(neighbor_bss->auc_mac_addr) || ether_is_all_zero(neighbor_bss->auc_mac_addr) ||
        !oal_memcmp(pst_mac_user->auc_user_mac_addr, neighbor_bss->auc_mac_addr, WLAN_MAC_ADDR_LEN)) {
        return OAL_FALSE;
    }

    /* 检查channel num 是否有效 */
    ul_ret = hmac_rx_bsst_req_candidate_info_check(pst_hmac_vap, &(neighbor_bss->uc_chl_num),
                                                   neighbor_bss->auc_mac_addr);
    if (ul_ret != OAL_SUCC) {
        return OAL_FALSE;
    }

    /* check dbac channel compared with p2p up channel */
    if (hmac_roam_check_dbac_channel(p2p_vap, neighbor_bss->uc_chl_num) == OAL_TRUE) {
        oam_warning_log3(0, OAM_SF_ROAM,
            "{hmac_roam_check_dbac_channel::dbac channel ignored, %0x:%0x chanNum=%d}",
            neighbor_bss->auc_mac_addr[4], neighbor_bss->auc_mac_addr[5], /* mac addr 4 & 5 */
            neighbor_bss->uc_chl_num);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


OAL_STATIC void hmac_bsst_set_max_pref_to_neighbor_bss(hmac_vap_stru *pst_hmac_vap,
    hmac_bsst_req_info_stru st_bsst_req_info, mac_user_stru *pst_mac_user)
{
    uint8_t idx_loop = 0;
    uint8_t max_bss_pref = 0;
    hmac_neighbor_bss_info_stru *neighbor_bss;
    mac_vap_stru *other_vap = mac_vap_find_another_up_vap_by_mac_vap(&(pst_hmac_vap->st_vap_base_info));

    /* find the max candidate preference but ignore current connect or invalid AP */
    for (idx_loop = 0; idx_loop < st_bsst_req_info.uc_bss_list_num; idx_loop++) {
        neighbor_bss = &(st_bsst_req_info.pst_neighbor_bss_list[idx_loop]);
        if (hmac_bsst_check_bssid_channel(pst_hmac_vap, neighbor_bss, pst_mac_user, other_vap) == OAL_FALSE) {
            continue;
        }

        neighbor_bss->valid = OAL_TRUE;
        if (neighbor_bss->uc_candidate_perf > max_bss_pref) {
            max_bss_pref = neighbor_bss->uc_candidate_perf;
        }
    }

    /* Neighbor Report don't include optional subelement: Candidate Preference */
    if (max_bss_pref == 0) {
        return;
    }

    /* set the max candidate pref to neighbor bss */
    for (idx_loop = 0; idx_loop < st_bsst_req_info.uc_bss_list_num; idx_loop++) {
        neighbor_bss = &(st_bsst_req_info.pst_neighbor_bss_list[idx_loop]);
        if (neighbor_bss->valid == OAL_FALSE) {
            continue;
        }
        neighbor_bss->uc_max_candidate_perf = max_bss_pref;
        oam_warning_log2(0, OAM_SF_ROAM,
                         "{hmac_bsst_set_max_pref_to_neighbor_bss:: candidate_pref=%d max_bss_pref=%d}",
                         neighbor_bss->uc_candidate_perf, neighbor_bss->uc_max_candidate_perf);
    }
}

OAL_STATIC void hmac_bsst_parse_req_mode(hmac_bsst_req_info_stru *bsst_req_info, uint8_t req_mode)
{
    bsst_req_info->st_request_mode.bit_candidate_list_include = req_mode & BIT0;
    bsst_req_info->st_request_mode.bit_abridged = (req_mode & BIT1) ? OAL_TRUE : OAL_FALSE;
    bsst_req_info->st_request_mode.bit_bss_disassoc_imminent = (req_mode & BIT2) ? OAL_TRUE : OAL_FALSE;
    bsst_req_info->st_request_mode.bit_termination_include = (req_mode & BIT3) ? OAL_TRUE : OAL_FALSE;
    bsst_req_info->st_request_mode.bit_ess_disassoc_imminent = (req_mode & BIT4) ? OAL_TRUE : OAL_FALSE;
}


uint32_t hmac_rx_bsst_req_action(hmac_vap_stru *pst_hmac_vap,
                                   hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf)
{
    uint16_t us_handle_len;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint16_t us_frame_len;
    uint8_t *puc_data = OAL_PTR_NULL;
    hmac_bsst_req_info_stru st_bsst_req_info;
    hmac_bsst_rsp_info_stru st_bsst_rsp_info;
    uint16_t us_url_count = 0;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    int32_t l_ret = EOK;
    hmac_roam_info_stru *pst_roam_info = NULL;
    oal_bool_enum_uint8 en_need_roam = OAL_TRUE;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_netbuf)) {
        oam_error_log3(0, OAM_SF_ANY,
                       "{hmac_rx_bsst_req_action::null param, vap:0x%x user:0x%x buf:0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action::roam info is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 开关未打开不处理 */
    if (OAL_FALSE == mac_mib_get_MgmtOptionBSSTransitionActivated(&pst_hmac_vap->st_vap_base_info)) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action:: BSSTransitionActivated is disabled}");
        return OAL_SUCC;
    }

    pst_mac_user = mac_res_get_mac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action::pst_mac_user is NULL}");
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }

    memset_s(&st_bsst_rsp_info, sizeof(st_bsst_rsp_info), 0, sizeof(st_bsst_rsp_info));
    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);
    memset_s(pst_11v_ctrl_info, OAL_SIZEOF(hmac_user_11v_ctrl_stru), 0, OAL_SIZEOF(hmac_user_11v_ctrl_stru));

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    /* 获取帧体指针 */
    puc_data = MAC_GET_RX_PAYLOAD_ADDR(pst_rx_info, pst_netbuf);
    us_frame_len = MAC_GET_RX_CB_PAYLOAD_LEN(pst_rx_info); /* 帧体长度 */
    /* 帧体的最小长度为7 小于7则格式异常 */
    if (us_frame_len < HMAC_11V_REQUEST_FRAME_BODY_FIX) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action:: frame length error %d.}", us_frame_len);
        return OAL_FAIL;
    }

    /* 将帧的各种参数解析出来 供上层调用 */
    /* 解析Token 如果与当前用户下不一致 刷新Token */
    if (puc_data[2] != pst_11v_ctrl_info->uc_user_bsst_token) {
        pst_11v_ctrl_info->uc_user_bsst_token = puc_data[2];
    }

    memset_s(&st_bsst_req_info, sizeof(st_bsst_req_info), 0, sizeof(st_bsst_req_info));
    /* 解析request mode */
    hmac_bsst_parse_req_mode(&st_bsst_req_info, puc_data[3]); /* 帧体offset3是Request Mode field */

    /* us_disassoc_time = The number of beacon transmission times (TBTTs) */
    st_bsst_req_info.us_disassoc_time = ((uint16_t)(puc_data[5]) << 8) | puc_data[4];

    /* us_disassoc_time表示The number of beacon transmission times (TBTTs)，计算时间需要乘以 beacon period */
    if (hmac_rx_bsst_check_disassoc_time(st_bsst_req_info.us_disassoc_time,
                                         mac_mib_get_BeaconPeriod(&(pst_hmac_vap->st_vap_base_info)))) {
        pst_11v_ctrl_info->en_only_scan_one_time = OAL_TRUE;
    }

    st_bsst_req_info.uc_validity_interval = puc_data[6];
    us_handle_len = 7; /* 前面7个字节已被处理完 */
    /* 12字节的termination duration 如果有的话 */
    if ((st_bsst_req_info.st_request_mode.bit_termination_include) &&
        (us_frame_len >= us_handle_len + MAC_IE_HDR_LEN + HMAC_11V_TERMINATION_TSF_LENGTH + 2)) {
        us_handle_len += MAC_IE_HDR_LEN; /* 去掉元素头 */
        l_ret += memcpy_s(st_bsst_req_info.st_term_duration.auc_termination_tsf, HMAC_11V_TERMINATION_TSF_LENGTH,
                          puc_data + us_handle_len, HMAC_11V_TERMINATION_TSF_LENGTH);
        us_handle_len += HMAC_11V_TERMINATION_TSF_LENGTH;
        st_bsst_req_info.st_term_duration.us_duration_min =
            (((uint16_t)puc_data[us_handle_len + 1]) << 8) | (puc_data[us_handle_len]);
        us_handle_len += 2;
    }

    /* 解析URL */
    /* URL字段 如果有的话 URL第一个字节为URL长度 申请动态内存保存 */
    st_bsst_req_info.puc_session_url = OAL_PTR_NULL;
    if ((st_bsst_req_info.st_request_mode.bit_ess_disassoc_imminent) && (us_frame_len >= us_handle_len + 1)) {
        if ((puc_data[us_handle_len] != 0) &&
            (us_frame_len >= ((us_handle_len + 1) + puc_data[us_handle_len]))) {
            /* 申请内存数量加1 用于存放字符串结束符 */
            us_url_count = puc_data[us_handle_len] * OAL_SIZEOF(uint8_t) + 1;
            st_bsst_req_info.puc_session_url =
                (uint8_t *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_url_count, OAL_TRUE);
            if (st_bsst_req_info.puc_session_url == OAL_PTR_NULL) {
                oam_error_log0(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action:: puc_session_url alloc fail.}");
                return OAL_FAIL;
            }

            l_ret += memcpy_s(st_bsst_req_info.puc_session_url, us_url_count,
                              puc_data + (us_handle_len + 1), puc_data[us_handle_len]);
            /* 转化成字符串 */
            st_bsst_req_info.puc_session_url[puc_data[us_handle_len]] = '\0';
        }
        us_handle_len += (puc_data[us_handle_len] + 1);
    }

    if (us_handle_len > us_frame_len) {
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_rx_bsst_req_action::us_handle_len[%d] > us_frame_len[%d]}",
                         us_handle_len, us_frame_len);
        /* 释放已经申请的内存 */
        if (st_bsst_req_info.puc_session_url != OAL_PTR_NULL) {
            oal_mem_free_m(st_bsst_req_info.puc_session_url, OAL_TRUE);
            st_bsst_req_info.puc_session_url = OAL_PTR_NULL;
        }
        return OAL_FAIL;
    }

    /* Candidate bss list由于STA的Response frame为可选 需要解析出来放在此结构体中 供上层处理 */
    st_bsst_req_info.pst_neighbor_bss_list = OAL_PTR_NULL;
    if (st_bsst_req_info.st_request_mode.bit_candidate_list_include) {
        puc_data += us_handle_len;
        st_bsst_req_info.pst_neighbor_bss_list = hmac_get_neighbor_ie(puc_data, us_frame_len - us_handle_len,
                                                                      &st_bsst_req_info.uc_bss_list_num);
        hmac_bsst_set_max_pref_to_neighbor_bss(pst_hmac_vap, st_bsst_req_info, pst_mac_user);

        /* 解析并保存邻居AP的channel list和BSSID list */
        hmac_bsst_neighbor_fill_chan_bssid(pst_roam_info, &pst_11v_ctrl_info->st_scan_h2d_params.st_scan_params,
                                           st_bsst_req_info.pst_neighbor_bss_list,
                                           st_bsst_req_info.uc_bss_list_num);
    }

    oam_warning_log4(0, OAM_SF_ANY,
                     "{hmac_rx_bsst_req_action::associated user mac address=xx:xx:xx:%02x:%02x:%02x bss_list_num=%d}",
                     pst_mac_user->auc_user_mac_addr[3], pst_mac_user->auc_user_mac_addr[4],
                     pst_mac_user->auc_user_mac_addr[5], st_bsst_req_info.uc_bss_list_num);

    /* 根据终端需求实现11v漫游 */
    if (pst_11v_ctrl_info->st_scan_h2d_params.st_scan_params.uc_channel_nums > 0) {
        /* 设置BSS Rsp帧内容 发送Response给AP */
        st_bsst_rsp_info.uc_status_code = 0;       /* 默认设置为同意切换 */
        st_bsst_rsp_info.uc_termination_delay = 0; /* 仅当状态码为5时有效，此次无意义设为0 */
        st_bsst_rsp_info.uc_chl_num =
            pst_11v_ctrl_info->st_scan_h2d_params.st_scan_params.ast_channel_list[0].uc_chan_number;
        st_bsst_rsp_info.c_rssi =
            hmac_get_rssi_from_scan_result(pst_hmac_vap, pst_hmac_vap->st_vap_base_info.auc_bssid);

        /* register BSS Transition Response callback function:
           * so that check roaming scan results firstly, and then send bsst rsp frame with right status code */
        pst_11v_ctrl_info->mac_11v_callback_fn = hmac_tx_bsst_rsp_action;

        l_ret += memcpy_s(&(pst_roam_info->st_bsst_rsp_info), sizeof(pst_roam_info->st_bsst_rsp_info),
                          &st_bsst_rsp_info, sizeof(st_bsst_rsp_info));

        /* disassoc time > 0,disassoc time < 100ms, BSST reject */
        if (hmac_rx_bsst_is_rejected(st_bsst_req_info, pst_mac_user, &pst_hmac_vap->st_vap_base_info)) {
            oam_warning_log1(0, OAM_SF_ANY,
                "{hmac_rx_bsst_req_action::bsst req candidate bssid invalid, isassoc_time=%d, will reject}",
                st_bsst_req_info.us_disassoc_time * mac_mib_get_BeaconPeriod(&(pst_hmac_vap->st_vap_base_info)));
            en_need_roam = OAL_FALSE;
        }
        hmac_11v_roam_check(pst_hmac_vap, pst_hmac_user, &en_need_roam, st_bsst_req_info);
    } else {
        /* 扫描信道数为0，不发起漫游。如果Disassociation Timer不为0，需要回复拒绝。 */
        if (st_bsst_req_info.us_disassoc_time > 0) {
            pst_roam_info->st_bsst_rsp_info.uc_status_code = WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES;
            hmac_tx_bsst_rsp_action(pst_hmac_vap, pst_hmac_user, &(pst_roam_info->st_bsst_rsp_info));
        }
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_rx_bsst_req_action::memcpy fail!");
    }

    /* 释放指针 */
    hmac_rx_bsst_free_res(&st_bsst_req_info);

    return OAL_SUCC;
}


uint32_t hmac_tx_bsst_rsp_action(void *pst_void1, void *pst_void2, void *pst_void3)
{
    hmac_vap_stru *pst_hmac_vap = (hmac_vap_stru *)pst_void1;
    hmac_user_stru *pst_hmac_user = (hmac_user_stru *)pst_void2;
    hmac_bsst_rsp_info_stru *pst_bsst_rsp_info = (hmac_bsst_rsp_info_stru *)pst_void3;
    oal_netbuf_stru *pst_bsst_rsp_buf = OAL_PTR_NULL;
    uint16_t us_frame_len;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_bsst_rsp_info)) {
        oam_error_log3(0, OAM_SF_ANY, "{hmac_tx_bsst_rsp_action::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_bsst_rsp_info);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);

    /* 申请bss transition request管理帧内存 */
    pst_bsst_rsp_buf = oal_mem_netbuf_alloc(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (pst_bsst_rsp_buf == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_tx_bsst_rsp_action::pst_bsst_rsq_buf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_bsst_rsp_buf, OAL_TRUE);
    oal_set_netbuf_prev(pst_bsst_rsp_buf, OAL_PTR_NULL);
    oal_set_netbuf_next(pst_bsst_rsp_buf, OAL_PTR_NULL);
    /* 调用封装管理帧接口 */
    us_frame_len = hmac_encap_bsst_rsp_action(pst_hmac_vap, pst_hmac_user, pst_bsst_rsp_info, pst_bsst_rsp_buf);
    if (us_frame_len == 0) {
        oal_netbuf_free(pst_bsst_rsp_buf);
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_tx_bsst_rsp_action::encap bsst rsp action frame failed.}");
        return OAL_FAIL;
    }
    /* 初始化CB */
    memset_s(oal_netbuf_cb(pst_bsst_rsp_buf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_bsst_rsp_buf);
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_hmac_user->st_user_base_info.us_assoc_id;
    MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_AC_MGMT;

    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len;

    oal_netbuf_put(pst_bsst_rsp_buf, us_frame_len);

    oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                     "{hmac_tx_bsst_rsp_action::tx 11v bsst rsp frame, us_frame_len=%d frametype=%d.}",
                     us_frame_len, MAC_GET_CB_FRAME_TYPE(pst_tx_ctl));

    /* 抛事件让dmac将该帧发送 */
    ul_ret = hmac_tx_mgmt_send_event(&pst_hmac_vap->st_vap_base_info, pst_bsst_rsp_buf, us_frame_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_bsst_rsp_buf);
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_tx_bsst_req_action::tx bsst rsp action frame failed.}");
        return ul_ret;
    }
    /* STA发送完Response后 一次交互流程就完成了 需要将user下的Token值加1 供下次发送使用 */
    if (HMAC_11V_TOKEN_MAX_VALUE == pst_11v_ctrl_info->uc_user_bsst_token) {
        pst_11v_ctrl_info->uc_user_bsst_token = 1;
    } else {
        pst_11v_ctrl_info->uc_user_bsst_token++;
    }

    return OAL_SUCC;
}


uint16_t hmac_encap_bsst_rsp_action(hmac_vap_stru *pst_hmac_vap,
                                      hmac_user_stru *pst_hmac_user,
                                      hmac_bsst_rsp_info_stru *pst_bsst_rsp_info,
                                      oal_netbuf_stru *pst_buffer)
{
    uint16_t us_index;
    uint8_t *puc_mac_header = OAL_PTR_NULL;
    uint8_t *puc_payload_addr = OAL_PTR_NULL;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = OAL_PTR_NULL;

    if (oal_any_null_ptr4(pst_hmac_vap, pst_hmac_user, pst_bsst_rsp_info, pst_buffer)) {
        oam_error_log4(0, OAM_SF_ANY, "{hmac_encap_bsst_rsp_action::null param.vap:%x user:%x info:%x buf:%x}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user,
                       (uintptr_t)pst_bsst_rsp_info, (uintptr_t)pst_buffer);
        return 0;
    }

    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);

    puc_mac_header = oal_netbuf_header(pst_buffer);
    puc_payload_addr = mac_netbuf_get_payload(pst_buffer);
    /*************************************************************************/
    /* Management Frame Format */
    /* -------------------------------------------------------------------- */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS| */
    /* -------------------------------------------------------------------- */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  | */
    /* -------------------------------------------------------------------- */
    /*************************************************************************/
    /*************************************************************************/
    /* Set the fields in the frame header */
    /*************************************************************************/
    /* Frame Control Field 中只需要设置Type/Subtype值，其他设置为0 */
    mac_hdr_set_frame_control(puc_mac_header, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    /* DA is address of STA addr */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR1_OFFSET, pst_hmac_user->st_user_base_info.auc_user_mac_addr);
    /* SA的值为本身的MAC地址 */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(&pst_hmac_vap->st_vap_base_info));
    /* TA的值为VAP的BSSID */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR3_OFFSET, pst_hmac_vap->st_vap_base_info.auc_bssid);

    /*************************************************************************************************************/
    /* Set the contents of the frame body */
    /*************************************************************************************************************/
    /*************************************************************************************************************/
    /* BSS Transition Response Frame - Frame Body */
    /* ---------------------------------------------------------------------------------------------------------- */
    /* |Category |Action | Token| Status Code | Termination Delay | Target BSSID |   BSS Candidate List Entry */
    /* --------------------------------------------------------------------------------------------------------- */
    /* |1        |1      | 1    |  1          | 1                 | 0-6          |    Optional */
    /* --------------------------------------------------------------------------------------------------------- */
    /*************************************************************************************************************/
    /* 将索引指向frame body起始位置 */
    us_index = 0;
    /* 设置Category */
    puc_payload_addr[us_index] = MAC_ACTION_CATEGORY_WNM;
    us_index++;
    /* 设置Action */
    puc_payload_addr[us_index] = MAC_WNM_ACTION_BSS_TRANSITION_MGMT_RESPONSE;
    us_index++;
    /* 设置Dialog Token */
    puc_payload_addr[us_index] = pst_11v_ctrl_info->uc_user_bsst_token;
    us_index++;
    /* 设置Status Code */
    puc_payload_addr[us_index] = pst_bsst_rsp_info->uc_status_code;
    us_index++;
    /* 设置Termination Delay */
    puc_payload_addr[us_index] = pst_bsst_rsp_info->uc_termination_delay;
    us_index++;
    /* 设置Target BSSID */
    if (pst_bsst_rsp_info->uc_status_code == 0) {
        if (EOK != memcpy_s(puc_payload_addr + us_index, WLAN_MGMT_NETBUF_SIZE - us_index,
                            pst_bsst_rsp_info->auc_target_bss_addr, WLAN_MAC_ADDR_LEN)) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail!");
            return 0;
        }
        us_index += WLAN_MAC_ADDR_LEN;
    }
    return (uint16_t)(us_index + MAC_80211_FRAME_LEN);
}

OAL_STATIC OAL_INLINE void hmac_parse_neighbor_report_subelement(int16_t s_sub_ie_len,
    uint8_t *puc_ie_data, hmac_neighbor_bss_info_stru *pst_bss_list_alloc, uint8_t uc_bss_list_index)
{
    while (s_sub_ie_len > 1) {
        switch (puc_ie_data[0]) {
            case HMAC_NEIGH_SUB_ID_BSS_CANDIDATE_PERF: /* 占用3个字节 */
                if (s_sub_ie_len < 3) { // 子ie至少包含3个字节
                    oam_error_log1(0, OAM_SF_ANY, "hmac_parse_neighbor_report_subelement::PREF len[%d]", s_sub_ie_len);
                    return;
                }
                pst_bss_list_alloc[uc_bss_list_index].uc_candidate_perf = puc_ie_data[2];
                s_sub_ie_len -= (HMAC_11V_PERFERMANCE_ELEMENT_LEN + MAC_IE_HDR_LEN);
                puc_ie_data += (HMAC_11V_PERFERMANCE_ELEMENT_LEN + MAC_IE_HDR_LEN);
                break;
            case HMAC_NEIGH_SUB_ID_TERM_DURATION: /* 占用12个字节 */
                if (s_sub_ie_len < 12) { // 子ie至少包含12个字节
                    oam_error_log1(0, OAM_SF_ANY,
                        "hmac_parse_neighbor_report_subelement::DURATION len[%d]", s_sub_ie_len);
                    return;
                }
                if (EOK != memcpy_s(pst_bss_list_alloc[uc_bss_list_index].st_term_duration.auc_termination_tsf,
                                    sizeof(pst_bss_list_alloc[uc_bss_list_index].st_term_duration.auc_termination_tsf),
                                    puc_ie_data + 2, HMAC_11V_TERMINATION_TSF_LENGTH)) {
                    oam_error_log0(0, OAM_SF_ANY, "hmac_handle_ie_by_data_length::memcpy fail!");
                    return;
                }
                pst_bss_list_alloc[uc_bss_list_index].st_term_duration.us_duration_min =
                   (((uint16_t)puc_ie_data[11]) << 8) | (puc_ie_data[10]);
                s_sub_ie_len -= (HMAC_11V_TERMINATION_ELEMENT_LEN + MAC_IE_HDR_LEN);
                puc_ie_data += (HMAC_11V_TERMINATION_ELEMENT_LEN + MAC_IE_HDR_LEN);
                break;
#ifdef _PRE_WLAN_FEATURE_MBO
            case HMAC_NEIGH_SUB_ID_VENDOR_SPECIFIC:
                if (s_sub_ie_len < 10) { // 子ie至少包含10个字节
                    oam_error_log1(0, OAM_SF_ANY, "hmac_parse_neighbor_report_subelement::MBO len[%d]", s_sub_ie_len);
                    return;
                }
                hmac_handle_ie_specific_mbo(puc_ie_data, pst_bss_list_alloc, uc_bss_list_index); // 封装函数降低行数
                s_sub_ie_len -= (puc_ie_data[1] + MAC_IE_HDR_LEN);
                puc_ie_data += (puc_ie_data[1] + MAC_IE_HDR_LEN);
                break;
#endif
            /* 其他IE跳过 不处理 */
            default:
                s_sub_ie_len -= (puc_ie_data[1] + MAC_IE_HDR_LEN);
                puc_ie_data += (puc_ie_data[1] + MAC_IE_HDR_LEN);
                break;
        }
    }
}

OAL_STATIC OAL_INLINE void hmac_analysis_ie_bssid_info(hmac_neighbor_bss_info_stru *pst_bss_list_alloc,
    uint8_t uc_bss_list_index, uint8_t *puc_ie_data)
{
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_ap_reachability =
        (puc_ie_data[8] & BIT1) | (puc_ie_data[8] & BIT0); /* bit0-1 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_security =
        (puc_ie_data[8] & BIT2) ? OAL_TRUE : OAL_FALSE; /* bit2 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_key_scope =
        (puc_ie_data[8] & BIT3) ? OAL_TRUE : OAL_FALSE; /* bit3 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_spectrum_mgmt =
        (puc_ie_data[8] & BIT4) ? OAL_TRUE : OAL_FALSE; /* bit4 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_qos =
        (puc_ie_data[8] & BIT5) ? OAL_TRUE : OAL_FALSE; /* bit5 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_apsd =
        (puc_ie_data[8] & BIT6) ? OAL_TRUE : OAL_FALSE; /* bit6 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_radio_meas =
        (puc_ie_data[8] & BIT7) ? OAL_TRUE : OAL_FALSE; /* bit7 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_delay_block_ack =
        (puc_ie_data[9] & BIT0) ? OAL_TRUE : OAL_FALSE; /* bit0 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_immediate_block_ack =
        (puc_ie_data[9] & BIT1) ? OAL_TRUE : OAL_FALSE; /* bit1 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_mobility_domain =
        (puc_ie_data[9] & BIT2) ? OAL_TRUE : OAL_FALSE; /* bit2 */
    pst_bss_list_alloc[uc_bss_list_index].st_bssid_info.bit_high_throughput =
        (puc_ie_data[9] & BIT3) ? OAL_TRUE : OAL_FALSE; /* bit3 */
    /* 保留字段不解析 */
    pst_bss_list_alloc[uc_bss_list_index].uc_opt_class = puc_ie_data[12];
    pst_bss_list_alloc[uc_bss_list_index].uc_chl_num = puc_ie_data[13];
    pst_bss_list_alloc[uc_bss_list_index].uc_phy_type = puc_ie_data[14];
}

uint8_t hmac_get_neighbor_ie_number(uint8_t *puc_data, uint16_t us_len)
{
    uint8_t bss_number = 0;
    uint8_t *puc_ie_data_find = puc_data;
    uint16_t us_len_find = us_len;
    uint8_t *puc_ie_data = OAL_PTR_NULL;

    /* 先确认含有多少个neighbor list */
    while (puc_ie_data_find != OAL_PTR_NULL) {
        puc_ie_data = mac_find_ie(MAC_EID_NEIGHBOR_REPORT, puc_ie_data_find, us_len_find);
        /* 没找到则退出循环 */
        if (puc_ie_data == OAL_PTR_NULL) {
            break;
        }
        bss_number++; /* Neighbor Report IE 数量加1 */

        if (us_len_find >= puc_ie_data[1] + MAC_IE_HDR_LEN) {
            puc_ie_data_find += (puc_ie_data[1] + MAC_IE_HDR_LEN);
            us_len_find -= (puc_ie_data[1] + MAC_IE_HDR_LEN);
        } else {
            oam_warning_log2(0, OAM_SF_ANY,
                             "{hmac_get_neighbor_ie_number::ie len[%d] greater than remain frame len[%d]!}",
                             puc_ie_data[1] + MAC_IE_HDR_LEN, us_len_find);
            return 0;
        }
    }

    return bss_number;
}


hmac_neighbor_bss_info_stru *hmac_get_neighbor_ie(uint8_t *puc_data,
                                                  uint16_t us_len, uint8_t *puc_bss_num)
{
    /* Neighbor Report element format */
    /* ------------------------------------------------------------------------------ -------------_----------- */
    /* | Ele ID | Length | BSSID | BSSID Info | Operating Class | Chan Num |  PHY Type | Optional Subelements | */
    /* -------------------------------------------------------------------------------------------------------- */
    /* |   1    |    1   |   6   |     4      |        1        |    1     |     1     |       variable       | */
    /* -------------------------------------------------------------------------------------------------------- */
    uint8_t *puc_ie_data_find = puc_data;
    uint8_t *puc_ie_data = OAL_PTR_NULL;
    uint16_t us_len_find = us_len;
    uint8_t uc_minmum_ie_len = 13;
    uint8_t uc_bss_number;
    uint8_t uc_bss_list_index = 0;
    uint8_t uc_neighbor_ie_len = 0;
    int16_t s_sub_ie_len = 0;
    hmac_neighbor_bss_info_stru *pst_bss_list_alloc = OAL_PTR_NULL;

    if (oal_any_null_ptr2(puc_data, puc_bss_num)) {
        oam_warning_log2(0, OAM_SF_ANY,
                         "{hmac_get_neighbor_ie::null pointer puc_data[%x] puc_bss_num[%x].}",
                         (uintptr_t)puc_data, (uintptr_t)puc_bss_num);
        if (puc_bss_num != OAL_PTR_NULL) {
            *puc_bss_num = 0;
        }
        return OAL_PTR_NULL;
    }

    *puc_bss_num = 0;

    /* 传入的帧长度为0，则不需要进行解析了 */
    if (us_len == 0) {
        return OAL_PTR_NULL;
    }

    uc_bss_number = hmac_get_neighbor_ie_number(puc_data, us_len);
    /* 如果neighbor ie 长度为0 直接返回 */
    if (uc_bss_number == 0) {
        return OAL_PTR_NULL;
    }
    /* 数据还原后再次从头解析数据 */
    pst_bss_list_alloc = (hmac_neighbor_bss_info_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
        uc_bss_number * OAL_SIZEOF(hmac_neighbor_bss_info_stru), OAL_TRUE);
    if (pst_bss_list_alloc == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_get_neighbor_ie::pst_bss_list null pointer.}");
        return OAL_PTR_NULL;
    }
    memset_s(pst_bss_list_alloc, uc_bss_number * OAL_SIZEOF(hmac_neighbor_bss_info_stru),
        0, uc_bss_number * OAL_SIZEOF(hmac_neighbor_bss_info_stru));
    for (uc_bss_list_index = 0; uc_bss_list_index < uc_bss_number; uc_bss_list_index++) {
        /* 前面已经查询过一次，这里不会返回空，所以不作判断 */
        puc_ie_data = mac_find_ie(MAC_EID_NEIGHBOR_REPORT, puc_ie_data_find, us_len_find);
        if (puc_ie_data == OAL_PTR_NULL) {
            oam_error_log2(0, OAM_SF_ANY, "{hmac_get_neighbor_ie::not find ie! bss_list_index[%d], bss_number[%d].}",
                           uc_bss_list_index, uc_bss_number);
            break;
        }

        uc_neighbor_ie_len = puc_ie_data[1];  // 元素长度
        if (uc_neighbor_ie_len < uc_minmum_ie_len) {
            oam_warning_log2(0, OAM_SF_ANY, "{hmac_get_neighbor_ie::neighbor_ie_len[%d] < minmum_ie_len[%d].}",
                             uc_neighbor_ie_len, uc_minmum_ie_len);
            oal_mem_free_m(pst_bss_list_alloc, OAL_TRUE);
            return OAL_PTR_NULL;
        }
        /* 解析Neighbor Report IE结构体 帧中只含有subelement 3 4，其他subelement已被过滤掉 */
        if (EOK != memcpy_s(pst_bss_list_alloc[uc_bss_list_index].auc_mac_addr, WLAN_MAC_ADDR_LEN,
            puc_ie_data + MAC_IE_HDR_LEN, WLAN_MAC_ADDR_LEN)) {
            oal_mem_free_m(pst_bss_list_alloc, OAL_TRUE);
            oam_error_log0(0, OAM_SF_ANY, "hmac_get_neighbor_ie::memcpy fail!");
            return OAL_PTR_NULL;
        }

        /* 解析BSSID Information */
        hmac_analysis_ie_bssid_info(pst_bss_list_alloc, uc_bss_list_index, puc_ie_data);

        /* 解析Subelement 长度大于最小ie长度才存在subelement 只处理3 4 subelement */
        /* CSEC:此时进入判断保证uc_neighbor_ie_len至少为14,后面puc_ie_data访问不会越界 */
        if (uc_neighbor_ie_len > uc_minmum_ie_len) {
            s_sub_ie_len = uc_neighbor_ie_len - uc_minmum_ie_len; /* subelement长度 */
            puc_ie_data += (uc_minmum_ie_len + MAC_IE_HDR_LEN);   /* 帧体数据移动到subelement处 */
            hmac_parse_neighbor_report_subelement(s_sub_ie_len, puc_ie_data, pst_bss_list_alloc, uc_bss_list_index);
        }
        oam_info_log4(0, OAM_SF_ANY,
            "hmac_get_neighbor_ie::bss_index=%d, chl_num=%d, candidate_pref=%d, max_pref=%d",
            uc_bss_list_index, pst_bss_list_alloc[uc_bss_list_index].uc_chl_num,
            pst_bss_list_alloc[uc_bss_list_index].uc_candidate_perf,
            pst_bss_list_alloc[uc_bss_list_index].uc_max_candidate_perf);
        puc_ie_data_find += (uc_neighbor_ie_len + MAC_IE_HDR_LEN);
        us_len_find -= (uc_neighbor_ie_len + MAC_IE_HDR_LEN);
    }

    *puc_bss_num = uc_bss_number;

    return pst_bss_list_alloc;
}
#endif  // _PRE_WLAN_FEATURE_11V_ENABLE
