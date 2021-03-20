


/* 1 头文件包含 */
#include "oam_ext_if.h"
#include "mac_ie.h"
#include "mac_device.h"
#include "mac_resource.h"

#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_mgmt_sta.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_p2p.h"
#include "hmac_11i.h"
#include "hmac_11v.h"
#include "hmac_scan.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#include "hmac_roam_alg.h"
#include "hmac_vowifi.h"
#ifdef _PRE_WLAN_FEATURE_11K
#include "wal_config.h"
#include "wal_linux_ioctl.h"
#endif
#include "hmac_dfx.h"
#include "hisi_customize_wifi.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_MAIN_C

/* 30s */
#define HMAC_ROAM_HOST_5G_SCAN_INTERVAL 30000
#define HOME_NETWORK_MAC_CHECK_LEN      4

#define hmac_container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
#define BSS_TRANSITION_RESP_MSG_LEN     4
#endif

/* 2 全局变量定义 */
OAL_STATIC hmac_roam_homenetwork_thr_stru g_st_roam_homenetwork_thr = { -50, -60, -74};
OAL_STATIC hmac_roam_fsm_func g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_BUTT][ROAM_MAIN_FSM_EVENT_TYPE_BUTT];
OAL_STATIC uint32_t hmac_roam_main_null_fn(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_scan_init(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_scan_channel(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_check_scan_result(hmac_roam_info_stru *pst_roam_info, void *p_param,
                                                  mac_bss_dscr_stru **ppst_bss_dscr_out);
OAL_STATIC uint32_t hmac_roam_connect_to_bss(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_to_old_bss(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_to_new_bss(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_main_del_timer(hmac_roam_info_stru *pst_roam_info);
OAL_STATIC uint32_t hmac_roam_main_check_state(hmac_roam_info_stru *pst_roam_info,
                                                 mac_vap_state_enum_uint8 en_vap_state,
                                                 roam_main_state_enum_uint8 en_main_state);
OAL_STATIC void hmac_roam_main_clear(hmac_roam_info_stru *pst_roam_info);
OAL_STATIC uint32_t hmac_roam_scan_timeout(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_connecting_timeout(hmac_roam_info_stru *pst_roam_info,
                                                   void *p_param);
OAL_STATIC uint32_t hmac_roam_connecting_fail(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_handle_fail_handshake_phase(hmac_roam_info_stru *pst_roam_info, void *p_param);
OAL_STATIC uint32_t hmac_roam_handle_scan_result(hmac_roam_info_stru *pst_roam_info, void *p_param);
#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC uint32_t hmac_roam_send_neighbor_req(hmac_roam_info_stru *pst_roam_info, uint8_t *p_cur_ssid);
OAL_STATIC uint32_t hmac_roam_neighbor_response_process(hmac_roam_info_stru *pst_roam_info,
                                                          void *p_param);
OAL_STATIC uint32_t hmac_wait_neighbor_response_timeout(hmac_roam_info_stru *pst_roam_info,
                                                          void *p_param);
#endif
#ifdef _PRE_WLAN_FEATURE_11R
OAL_STATIC uint32_t hmac_roam_preauth(hmac_vap_stru *pst_hmac_vap);
#endif

/* 3 函数实现 */

OAL_STATIC void hmac_roam_fsm_init(void)
{
    uint32_t ul_state;
    uint32_t ul_event;

    for (ul_state = 0; ul_state < ROAM_MAIN_STATE_BUTT; ul_state++) {
        for (ul_event = 0; ul_event < ROAM_MAIN_FSM_EVENT_TYPE_BUTT; ul_event++) {
            g_hmac_roam_main_fsm_func[ul_state][ul_event] = hmac_roam_main_null_fn;
        }
    }
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_INIT][ROAM_MAIN_FSM_EVENT_START] = hmac_roam_scan_init;
#ifdef _PRE_WLAN_FEATURE_11K
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_NEIGHBOR_PROCESS][ROAM_MAIN_FSM_EVENT_START] =
        hmac_roam_neighbor_response_process;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_NEIGHBOR_PROCESS][ROAM_MAIN_FSM_EVENT_TIMEOUT] =
        hmac_wait_neighbor_response_timeout;
#endif
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_SCANING][ROAM_MAIN_FSM_EVENT_START] = hmac_roam_scan_channel;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_SCANING][ROAM_MAIN_FSM_EVENT_SCAN_RESULT] = hmac_roam_handle_scan_result;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_SCANING][ROAM_MAIN_FSM_EVENT_TIMEOUT] = hmac_roam_scan_timeout;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_SCANING][ROAM_MAIN_FSM_EVENT_START_CONNECT] = hmac_roam_connect_to_bss;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_CONNECTING][ROAM_MAIN_FSM_EVENT_TIMEOUT] = hmac_roam_connecting_timeout;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_CONNECTING][ROAM_MAIN_FSM_EVENT_CONNECT_FAIL] = hmac_roam_connecting_fail;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_CONNECTING][ROAM_MAIN_FSM_EVENT_HANDSHAKE_FAIL] =
        hmac_roam_handle_fail_handshake_phase;
    g_hmac_roam_main_fsm_func[ROAM_MAIN_STATE_CONNECTING][ROAM_MAIN_FSM_EVENT_CONNECT_SUCC] = hmac_roam_to_new_bss;
}


uint32_t hmac_roam_main_fsm_action(hmac_roam_info_stru *pst_roam_info,
                                         roam_main_fsm_event_type_enum en_event, void *p_param)
{
    if (oal_unlikely(pst_roam_info == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_roam_info->en_main_state >= ROAM_MAIN_STATE_BUTT) {
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    if (en_event >= ROAM_MAIN_FSM_EVENT_TYPE_BUTT) {
        return OAL_ERR_CODE_ROAM_EVENT_UXEXPECT;
    }

    return g_hmac_roam_main_fsm_func[pst_roam_info->en_main_state][en_event](pst_roam_info, p_param);
}


OAL_STATIC void hmac_roam_main_change_state(hmac_roam_info_stru *pst_roam_info, roam_main_state_enum_uint8 en_state)
{
    if (pst_roam_info != OAL_PTR_NULL) {
        oam_warning_log2(0, OAM_SF_ROAM,
                         "{hmac_roam_main_change_state::[%d]->[%d]}", pst_roam_info->en_main_state, en_state);
        pst_roam_info->en_main_state = en_state;
    }
}


OAL_STATIC uint32_t hmac_roam_main_timeout(void *p_arg)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (oal_unlikely(p_arg == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_roam_info = (hmac_roam_info_stru *)p_arg;

    oam_warning_log2(0, OAM_SF_ROAM, "{hmac_roam_main_timeout::MAIN_STATE[%d] CONNECT_STATE[%d].}",
                     pst_roam_info->en_main_state, pst_roam_info->st_connect.en_state);

    return hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_TIMEOUT, OAL_PTR_NULL);
}


OAL_STATIC uint32_t hmac_roam_renew_privacy(hmac_vap_stru *pst_hmac_vap, mac_bss_dscr_stru *pst_bss_dscr)
{
    uint32_t ul_ret;
    uint32_t ul_match_suite;
    mac_conn_security_stru st_conn_sec;
    mac_cap_info_stru *pst_cap_info;
    uint16_t us_rsn_cap_info = 0;
    oal_bool_enum_uint8 en_pmf_cap;
    oal_bool_enum_uint8 en_pmf_require;

    pst_cap_info = (mac_cap_info_stru *)&pst_bss_dscr->us_cap_info;

    if (pst_cap_info->bit_privacy == 0) {
        return OAL_SUCC;
    }

    if (NL80211_AUTHTYPE_OPEN_SYSTEM != mac_mib_get_AuthenticationMode(&pst_hmac_vap->st_vap_base_info)) {
        return OAL_SUCC;
    }

    memset_s(&st_conn_sec, OAL_SIZEOF(mac_conn_security_stru), 0, OAL_SIZEOF(mac_conn_security_stru));

    if (pst_bss_dscr->puc_wpa_ie != OAL_PTR_NULL) {
        st_conn_sec.en_privacy = OAL_TRUE;
        mac_ie_get_wpa_cipher(pst_bss_dscr->puc_wpa_ie, &(st_conn_sec.st_crypto));

        ul_match_suite = mac_mib_wpa_pair_match_suites_s(&pst_hmac_vap->st_vap_base_info,
                                                         st_conn_sec.st_crypto.aul_pair_suite,
                                                         sizeof(st_conn_sec.st_crypto.aul_pair_suite));
        if (ul_match_suite != 0) {
            st_conn_sec.st_crypto.aul_pair_suite[0] = ul_match_suite;
        }
        st_conn_sec.st_crypto.aul_pair_suite[1] = 0;

        ul_match_suite = mac_mib_wpa_akm_match_suites_s(&pst_hmac_vap->st_vap_base_info,
                                                        st_conn_sec.st_crypto.aul_akm_suite,
                                                        sizeof(st_conn_sec.st_crypto.aul_akm_suite));
        if (ul_match_suite != 0) {
            st_conn_sec.st_crypto.aul_akm_suite[0] = ul_match_suite;
        }
        st_conn_sec.st_crypto.aul_akm_suite[1] = 0;
    }

    if (pst_bss_dscr->puc_rsn_ie != OAL_PTR_NULL) {
        st_conn_sec.en_privacy = OAL_TRUE;
        mac_ie_get_rsn_cipher(pst_bss_dscr->puc_rsn_ie, &(st_conn_sec.st_crypto));

        ul_match_suite = mac_mib_rsn_pair_match_suites_s(&pst_hmac_vap->st_vap_base_info,
                                                         st_conn_sec.st_crypto.aul_pair_suite,
                                                         sizeof(st_conn_sec.st_crypto.aul_pair_suite));
        if (ul_match_suite != 0) {
            st_conn_sec.st_crypto.aul_pair_suite[0] = ul_match_suite;
        }
        st_conn_sec.st_crypto.aul_pair_suite[1] = 0;

        ul_match_suite = mac_mib_rsn_akm_match_suites_s(&pst_hmac_vap->st_vap_base_info,
                                                        st_conn_sec.st_crypto.aul_akm_suite,
                                                        sizeof(st_conn_sec.st_crypto.aul_akm_suite));
        if (ul_match_suite != 0) {
            st_conn_sec.st_crypto.aul_akm_suite[0] = ul_match_suite;
        }
        st_conn_sec.st_crypto.aul_akm_suite[1] = 0;
        /* 获取目的AP的RSN CAP信息 */
        us_rsn_cap_info = mac_get_rsn_capability(pst_bss_dscr->puc_rsn_ie);
    }

    en_pmf_cap = mac_mib_get_dot11RSNAMFPC(&pst_hmac_vap->st_vap_base_info);
    en_pmf_require = mac_mib_get_dot11RSNAMFPR(&pst_hmac_vap->st_vap_base_info);
    if ((en_pmf_require == OAL_TRUE) && (!(us_rsn_cap_info & BIT7))) { /* 本地强制，对端没有MFP能力 */
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_roam_renew_privacy:: vap required pmf and ap don't have pmf cap!}\r\n");
    }

    if ((en_pmf_cap == OAL_FALSE) && (us_rsn_cap_info & BIT6)) { /* 对端强制，本地没有MFP能力 */
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_roam_renew_privacy:: vap no pmf cap and ap required!!}\r\n");
    }

    /* 当前驱动的pmf能力由wpa控制, 故漫游时不支持pmf到非pmf ap的漫游 */
    if (pst_hmac_vap->st_vap_base_info.en_user_pmf_cap && (!(us_rsn_cap_info & BIT7))) { /* 原ap开pmf 目的ap无pmf */
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_roam_renew_privacy:: roam to no pmf ap!!}\r\n");
    }

    /* 暂时赋值上次的PMF能力，在join阶段才会根据对端PMF能力来更新VAP的PMF能力并同步到device */
    st_conn_sec.en_mgmt_proteced = pst_hmac_vap->st_vap_base_info.en_user_pmf_cap;
    st_conn_sec.en_pmf_cap = en_pmf_cap + en_pmf_require;

    ul_ret = mac_vap_init_privacy(&pst_hmac_vap->st_vap_base_info, &st_conn_sec);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG, "{hmac_roam_renew_privacy:: mac_11i_init_privacy failed[%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


void hmac_roam_check_psk(hmac_vap_stru *pst_hmac_vap, mac_conn_security_stru *pst_conn_sec)
{
    uint32_t ul_akm_suite = oal_ntoh_32(pst_conn_sec->st_crypto.aul_akm_suite[0]);
    uint8_t uc_akm_type = ul_akm_suite & 0xFF;

    if (uc_akm_type == WLAN_AUTH_SUITE_PSK ||
        uc_akm_type == WLAN_AUTH_SUITE_PSK_SHA256 ||
        uc_akm_type == WLAN_AUTH_SUITE_FT_PSK) {
        pst_hmac_vap->en_is_psk = OAL_TRUE;
    } else {
        pst_hmac_vap->en_is_psk = OAL_FALSE;
    }
    return;
}


OAL_STATIC void hmac_roam_check_home_network(hmac_vap_stru *pst_hmac_vap, hmac_roam_info_stru *pst_roam_info)
{
    if (pst_roam_info->st_config.uc_scenario_enable == OAL_FALSE) {
        return;
    }

    pst_roam_info->uc_roaming_scenario = ROAMING_SCENARIO_BUTT;

    /* 家庭网络场景识别 */
    if (pst_roam_info->st_alg.st_home_network.st_2g_bssid.uc_bssid_num == 1 &&
        pst_roam_info->st_alg.st_home_network.st_5g_bssid.uc_bssid_num == 1 &&
        pst_hmac_vap->en_is_psk == OAL_TRUE) {
        if (oal_memcmp(pst_roam_info->st_alg.st_home_network.st_2g_bssid.auc_bssid[0],
                       pst_roam_info->st_alg.st_home_network.st_5g_bssid.auc_bssid[0],
                       HOME_NETWORK_MAC_CHECK_LEN) == 0) {
            pst_roam_info->st_alg.st_home_network.uc_is_homenetwork = OAL_TRUE;
            pst_roam_info->uc_roaming_scenario = ROAMING_SCENARIO_HOME;
            return;
        }
    }

    pst_roam_info->st_alg.st_home_network.uc_is_homenetwork = OAL_FALSE;
    return;
}


OAL_STATIC void hmac_roam_home_network_init(hmac_vap_stru *pst_hmac_vap, hmac_roam_info_stru *pst_roam_info)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_scan_bss_mgmt = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_home_network_init::device is null}");
        return;
    }

    pst_roam_info->st_alg.st_home_network.uc_is_homenetwork = OAL_FALSE;
    pst_roam_info->st_alg.st_home_network.c_trigger_rssi = g_st_roam_homenetwork_thr.c_trigger_rssi;
    pst_roam_info->st_alg.st_home_network.c_5g_rssi_th = g_st_roam_homenetwork_thr.c_5g_rssi_th;
    pst_roam_info->st_alg.st_home_network.c_5g_hold_th = g_st_roam_homenetwork_thr.c_5g_hold_th;
    pst_roam_info->st_alg.st_home_network.uc_max_retry_cnt = 6;
    pst_roam_info->st_alg.st_home_network.ul_5g_scan_timestamp = oal_time_get_stamp_ms();

    pst_scan_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    oal_spin_lock(&(pst_scan_bss_mgmt->st_lock));

    /* 遍历扫描到的bss信息，识别家庭网络漫游环境 */
    oal_dlist_search_for_each(pst_entry, &(pst_scan_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        hmac_roam_alg_bss_in_ess(pst_roam_info, pst_bss_dscr);

        pst_bss_dscr = OAL_PTR_NULL;
    }

    oal_spin_unlock(&(pst_scan_bss_mgmt->st_lock));

    hmac_roam_check_home_network(pst_hmac_vap, pst_roam_info);

    if (pst_roam_info->uc_roaming_scenario == ROAMING_SCENARIO_HOME) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "hmac_roam_home_network_init::home network roaming scenario");
    }

    return;
}


uint32_t hmac_roam_init(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    uint32_t i = 0;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (!IS_LEGACY_VAP(&(pst_hmac_vap->st_vap_base_info))) {
        return OAL_SUCC;
    }

    if (pst_hmac_vap->pul_roam_info == OAL_PTR_NULL) {
        /* 漫游主结构体内存申请 */
        pst_hmac_vap->pul_roam_info =
            (uint32_t *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(hmac_roam_info_stru), OAL_TRUE);
        if (pst_hmac_vap->pul_roam_info == OAL_PTR_NULL) {
            oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                           "{hmac_roam_init::oal_mem_alloc_m fail.}");
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
    }
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    else {
        hmac_roam_main_del_timer((hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info);
    }
#endif

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;

    /* TBD 参数初始化 */
    memset_s(pst_hmac_vap->pul_roam_info, OAL_SIZEOF(hmac_roam_info_stru),
             0, OAL_SIZEOF(hmac_roam_info_stru));
    pst_roam_info->uc_enable = g_wlan_cust->uc_roam_switch;
    pst_roam_info->en_roam_trigger = ROAM_TRIGGER_DMAC;
    pst_roam_info->en_main_state = ROAM_MAIN_STATE_INIT;
    pst_roam_info->en_current_bss_ignore = OAL_TRUE;
    pst_roam_info->pst_hmac_vap = pst_hmac_vap;
    pst_roam_info->pst_hmac_user = OAL_PTR_NULL;
    pst_roam_info->ul_connected_state = WPAS_CONNECT_STATE_INIT;
    pst_roam_info->uc_roaming_scenario = ROAMING_SCENARIO_BUTT;
    pst_roam_info->st_config.uc_scan_band =
        (mac_device_check_5g_enable(pst_hmac_vap->st_vap_base_info.uc_device_id)) ? (BIT0 | BIT1) : BIT0;
    pst_roam_info->st_config.uc_scan_orthogonal = g_wlan_cust->uc_roam_scan_orthogonal;
    pst_roam_info->st_config.c_trigger_rssi_2G = g_wlan_cust->c_roam_trigger_b;
    pst_roam_info->st_config.c_trigger_rssi_5G = g_wlan_cust->c_roam_trigger_a;
    pst_roam_info->st_config.c_dense_env_trigger_rssi_2G = g_wlan_cust->c_dense_env_roam_trigger_b;
    pst_roam_info->st_config.c_dense_env_trigger_rssi_5G = g_wlan_cust->c_dense_env_roam_trigger_a;
    pst_roam_info->st_config.uc_delta_rssi_2G = g_wlan_cust->c_roam_delta_b;
    pst_roam_info->st_config.uc_delta_rssi_5G = g_wlan_cust->c_roam_delta_a;
    pst_roam_info->st_config.uc_scenario_enable = g_wlan_cust->uc_scenario_enable;
    pst_roam_info->st_config.c_candidate_good_rssi = g_wlan_cust->c_candidate_good_rssi;
    pst_roam_info->st_config.uc_candidate_good_num = g_wlan_cust->uc_candidate_good_num;
    pst_roam_info->st_config.uc_candidate_weak_num = g_wlan_cust->uc_candidate_weak_num;
    pst_roam_info->st_config.us_roam_interval = g_wlan_cust->us_roam_interval;

    for (i = 0; i < ROAM_LIST_MAX; i++) {
        pst_roam_info->st_alg.st_history.ast_bss[i].us_count_limit = ROAM_HISTORY_COUNT_LIMIT;
        pst_roam_info->st_alg.st_history.ast_bss[i].ul_timeout = ROAM_HISTORY_BSS_TIME_OUT;
    }

    for (i = 0; i < ROAM_LIST_MAX; i++) {
        pst_roam_info->st_alg.st_blacklist.ast_bss[i].us_count_limit = ROAM_BLACKLIST_COUNT_LIMIT;
        pst_roam_info->st_alg.st_blacklist.ast_bss[i].ul_timeout = ROAM_BLACKLIST_NORMAL_AP_TIME_OUT;
    }

    hmac_roam_fsm_init();
    hmac_roam_connect_fsm_init();

    hmac_roam_home_network_init(pst_hmac_vap, pst_roam_info);

    oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM, "{hmac_roam_init::SUCC.}");
    return OAL_SUCC;
}



uint32_t hmac_roam_info_init(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_roam_trigger_stru st_roam_trigger;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_vap->pul_roam_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;

    pst_roam_info->en_main_state = ROAM_MAIN_STATE_INIT;
    pst_roam_info->uc_rssi_type = ROAM_ENV_SPARSE_AP;
    pst_roam_info->uc_invalid_scan_cnt = 0;
    pst_roam_info->st_connect.en_state = ROAM_CONNECT_STATE_INIT;
    pst_roam_info->st_alg.ul_max_capacity = 0;
    pst_roam_info->st_alg.pst_max_capacity_bss = OAL_PTR_NULL;
    pst_roam_info->st_alg.c_current_rssi = 0;
    pst_roam_info->st_alg.c_max_rssi = 0;
    pst_roam_info->st_alg.uc_another_bss_scaned = 0;
    pst_roam_info->st_alg.uc_invalid_scan_cnt = 0;
    pst_roam_info->st_alg.uc_candidate_bss_num = 0;
    pst_roam_info->st_alg.uc_candidate_good_rssi_num = 0;
    pst_roam_info->st_alg.uc_candidate_weak_rssi_num = 0;
    pst_roam_info->st_alg.uc_scan_period = 0;
    pst_roam_info->st_alg.pst_max_rssi_bss = OAL_PTR_NULL;
    pst_roam_info->pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (pst_roam_info->pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_info_init::assoc_vap_id[%d] can't found.}",
                       pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    st_roam_trigger.c_trigger_2G = pst_roam_info->st_config.c_trigger_rssi_2G;
    st_roam_trigger.c_trigger_5G = pst_roam_info->st_config.c_trigger_rssi_5G;
    st_roam_trigger.us_roam_interval = pst_roam_info->st_config.us_roam_interval;

    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info,
                                        WLAN_CFGID_SET_ROAM_TRIGGER,
                                        OAL_SIZEOF(mac_roam_trigger_stru),
                                        (uint8_t *)&st_roam_trigger);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_info_init::send event[INIT_ROAM_TRIGGER] failed[%d].}", ul_ret);
        return ul_ret;
    }

    oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM, "{hmac_roam_info_init::SUCC.}");
    return OAL_SUCC;
}


uint32_t hmac_roam_exit(hmac_vap_stru *pst_hmac_vap)
{
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (!IS_LEGACY_VAP(&(pst_hmac_vap->st_vap_base_info))) {
        return OAL_SUCC;
    }

    if (pst_hmac_vap->pul_roam_info == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_exit::pul_roam_info is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_roam_show(pst_hmac_vap);

    frw_timer_immediate_destroy_timer_m(&(((hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info)->st_connect.st_timer));
    hmac_roam_main_del_timer((hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info);
    if (pst_hmac_vap->pst_net_device != OAL_PTR_NULL) {
        oal_net_tx_wake_all_queues(pst_hmac_vap->pst_net_device);
    }
    oal_mem_free_m(pst_hmac_vap->pul_roam_info, OAL_TRUE);
    pst_hmac_vap->pul_roam_info = OAL_PTR_NULL;

    oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM, "{hmac_roam_exit::SUCC.}");

    return OAL_SUCC;
}


uint32_t hmac_roam_show_info(hmac_vap_stru *pst_hmac_vap)
{
    int8_t *pc_print_buff = OAL_PTR_NULL;
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_hmac_vap->pul_roam_info == OAL_PTR_NULL) {
        return OAL_SUCC;
    }

    pc_print_buff = (int8_t *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                       "{hmac_config_vap_info::pc_print_buff null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);
    snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
               "ROAM_EN[%d] MAIN_STATE[%d]\n"
               "ROAM_SCAN_BAND[%d] ROAM_SCAN_ORTH[%d]\n",
               pst_roam_info->uc_enable, pst_roam_info->en_main_state,
               pst_roam_info->st_config.uc_scan_band, pst_roam_info->st_config.uc_scan_orthogonal);

    oam_print(pc_print_buff);

    oal_mem_free_m(pc_print_buff, OAL_TRUE);

    return OAL_SUCC;
}


OAL_STATIC void hmac_roam_main_start_timer(hmac_roam_info_stru *pst_roam_info, uint32_t ul_timeout)
{
    frw_timeout_stru *pst_timer = &(pst_roam_info->st_timer);

    oam_info_log1(0, OAM_SF_ROAM, "{hmac_roam_main_start_timer [%d].}", ul_timeout);

    /* 启动认证超时定时器 */
    frw_timer_create_timer_m(pst_timer,
                           hmac_roam_main_timeout,
                           ul_timeout,
                           pst_roam_info,
                           OAL_FALSE,
                           OAM_MODULE_ID_HMAC,
                           pst_roam_info->pst_hmac_vap->st_vap_base_info.ul_core_id);
}


OAL_STATIC uint32_t hmac_roam_main_del_timer(hmac_roam_info_stru *pst_roam_info)
{
    oam_info_log0(0, OAM_SF_ROAM, "{hmac_roam_main_del_timer.}");
    frw_timer_immediate_destroy_timer_m(&(pst_roam_info->st_timer));
    return OAL_SUCC;
}


uint32_t hmac_roam_enable(hmac_vap_stru *pst_hmac_vap, uint8_t uc_enable)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_enable::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_enable::pst_roam_info null .}");
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    if (uc_enable == pst_roam_info->uc_enable) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_enable::SET[%d] fail .}", uc_enable);
        return OAL_FAIL;
    }

    /* 设置漫游开关 */
    pst_roam_info->uc_enable = uc_enable;
    pst_roam_info->en_main_state = ROAM_MAIN_STATE_INIT;
    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_enable::SET[%d] OK!}", uc_enable);

    return OAL_SUCC;
}


uint32_t hmac_roam_check_signal_bridge(hmac_vap_stru *pst_hmac_vap)
{
    mac_vap_stru *pst_mac_vap;
    mac_vap_stru *pst_other_vap = OAL_PTR_NULL;

    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    /* check all vap state in case other vap is signal bridge GO mode */
    pst_other_vap = mac_vap_find_another_up_vap_by_mac_vap(pst_mac_vap);
    if (pst_other_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_check_signal_bridge::another vap is null!}");
        return OAL_SUCC;
    }

    /* 终端需求: 打开信号桥，禁止漫游 */
    if (IS_BRIDGE_VAP(pst_other_vap)) {
        /* 如果是P2P GO模式且Beacon帧不包含P2P ie即为信号桥 */
        oam_warning_log0(pst_other_vap->uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_check_signal_bridge::vap is GO Signal Bridge, disable STA roaming!}");
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    return OAL_SUCC;
}


OAL_STATIC void hmac_roam_param_update(hmac_roam_info_stru *pst_roam_info,
    roam_channel_org_enum uc_scan_type, oal_bool_enum_uint8 en_current_bss_ignore,
    roam_trigger_enum_uint8 en_roam_trigger, uint8_t *pauc_target_bssid)
{
    pst_roam_info->st_config.uc_scan_orthogonal = uc_scan_type;
    pst_roam_info->en_current_bss_ignore = en_current_bss_ignore; /* false表示漫游到自己 */
    pst_roam_info->en_roam_trigger = en_roam_trigger;
#ifdef _PRE_WLAN_FEATURE_11R
    pst_roam_info->st_connect.uc_ft_force_air = OAL_TRUE;
    pst_roam_info->st_connect.uc_ft_failed = OAL_FALSE;
#endif
    if (pauc_target_bssid != OAL_PTR_NULL) { /* && (en_roam_trigger == ROAM_TRIGGER_BSSID) */
        oal_set_mac_addr(pst_roam_info->auc_target_bssid, pauc_target_bssid);
    }
}

/* 漫游时，大屏如果有其他VAP处于P2P listen状态，则先取消listen，防止漫游扫描失败 */
OAL_STATIC void hmac_roam_stop_p2p_listen(mac_vap_stru *mac_vap)
{
    uint8_t vap_idx;
    uint32_t pedding_data = 0;
    mac_vap_stru *stop_mac_vap = NULL;
    mac_device_stru *mac_device = NULL;
    hmac_device_stru *hmac_device = NULL;
    uint8_t self_vap_id = mac_vap->uc_vap_id;

    if (!hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
        return;
    }

    hmac_device = hmac_res_get_mac_dev(mac_vap->uc_device_id);
    if (hmac_device == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hmac_roam_stop_p2p_listen::hmac_device null device_id = [%d]!}",
            mac_vap->uc_device_id);
        return;
    }

    mac_device = hmac_device->pst_device_base_info;
    if (mac_device == NULL) {
        return;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        stop_mac_vap = mac_res_get_mac_vap(mac_device->auc_vap_id[vap_idx]);
        if (stop_mac_vap == NULL) {
            continue;
        }

        if (self_vap_id == stop_mac_vap->uc_vap_id) {
            continue;
        }

        if (stop_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
            oam_warning_log1(self_vap_id, OAM_SF_ROAM, "{hmac_roam_stop_p2p_listen::stop p2p listen vap id = [%d]!}",
                stop_mac_vap->uc_vap_id);
            hmac_config_scan_abort(stop_mac_vap, OAL_SIZEOF(uint32_t), (uint8_t*)&pedding_data);
        }
    }
}


uint32_t hmac_roam_start(hmac_vap_stru *pst_hmac_vap, roam_channel_org_enum uc_scan_type,
                               oal_bool_enum_uint8 en_current_bss_ignore, uint8_t *pauc_target_bssid,
                               roam_trigger_enum_uint8 en_roam_trigger)
{
    uint32_t              ul_ret;
    hmac_roam_info_stru    *pst_roam_info = OAL_PTR_NULL;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_start::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (en_current_bss_ignore == OAL_TRUE) {
        ul_ret = hmac_roam_check_signal_bridge(pst_hmac_vap);
        if (ul_ret != OAL_SUCC) {
            return ul_ret;
        }

        /* 非漫游到自己，黑名单路由器，不支持漫游，防止漫游出现异常 */
        if (pst_hmac_vap->en_roam_prohibit_on == OAL_TRUE) {
            oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_start::blacklist ap not support roam!}");
            return OAL_FAIL;
        }
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_INIT);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_start::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

#ifdef _PRE_WLAN_FEATURE_MBO
    if (pst_hmac_vap->st_vap_base_info.st_mbo_para_info.uc_mbo_enable != OAL_TRUE) {
#endif
        if (pst_hmac_vap->ipaddr_obtained == OAL_FALSE) {
            oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_start::ip addr not obtained!}");
            return OAL_ERR_CODE_ROAM_INVALID_VAP_STATUS;
        }
#ifdef _PRE_WLAN_FEATURE_MBO
    }
#endif

    /* 每次漫游前，刷新是否支持漫游到自己的参数 */
    hmac_roam_param_update(pst_roam_info, uc_scan_type, en_current_bss_ignore, en_roam_trigger, pauc_target_bssid);

    oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_start::START succ, uc_scan_type=%d, en_current_bss_ignore=%d en_roam_trigger=%d.}",
                     uc_scan_type, en_current_bss_ignore, en_roam_trigger);

    hmac_roam_alg_init_rssi(pst_hmac_vap, pst_roam_info);

    /* 触发漫游是否搭配扫描0表示不扫描 */
    if (ROAM_SCAN_CHANNEL_ORG_0 == uc_scan_type) {
        /* 对于不进行扫描的漫游流程,更新扫描/关联时间戳 */
        pst_roam_info->st_static.ul_scan_start_timetamp = (uint32_t)oal_time_get_stamp_ms();
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_SCANING);

        ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_TIMEOUT, OAL_PTR_NULL);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                           "{hmac_roam_start::START fail[%d].}", ul_ret);
            return ul_ret;
        }
    } else {
        ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, OAL_PTR_NULL);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                           "{hmac_roam_start::START fail[%d].}", ul_ret);
            return ul_ret;
        }
    }

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_START);
    return OAL_SUCC;
}


uint32_t hmac_roam_handle_home_network(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info;
    uint32_t ul_current_timestamp;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL || pst_roam_info->uc_enable == 0) {
        return OAL_SUCC;
    }

    hmac_roam_check_home_network(pst_hmac_vap, pst_roam_info);

    /* 漫游场景识别5: 家庭双频路由器，关联RSSI>=-50dBm 2G AP，可以漫游到5G AP，5G RSSI >= -60dB */
    if ((pst_roam_info->st_config.uc_scenario_enable == OAL_TRUE) &&
        (pst_roam_info->uc_roaming_scenario == ROAMING_SCENARIO_HOME) &&
        (pst_roam_info->st_alg.st_home_network.uc_is_homenetwork == OAL_TRUE) &&
        (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) &&
        (pst_hmac_vap->station_info.signal >=
         (pst_roam_info->st_alg.st_home_network.c_trigger_rssi + pst_roam_info->st_alg.uc_roam_fail_cnt * 5))) {
        ul_current_timestamp = oal_time_get_stamp_ms();
        if (oal_time_after32(ul_current_timestamp,
            (pst_roam_info->st_alg.st_home_network.ul_5g_scan_timestamp + HMAC_ROAM_HOST_5G_SCAN_INTERVAL))) {
            oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_handle_home_network::roam_to home network env:%d sign:%d max_rssi:%d current_rssi:%d}",
                pst_roam_info->st_alg.st_home_network.uc_is_homenetwork, pst_hmac_vap->station_info.signal,
                pst_roam_info->st_alg.c_max_rssi, pst_roam_info->st_alg.c_current_rssi);
            pst_roam_info->st_alg.st_home_network.ul_5g_scan_timestamp = ul_current_timestamp;
            return hmac_roam_start(pst_hmac_vap, ROAM_SCAN_CHANNEL_ORG_1,
                                       OAL_TRUE, NULL, ROAM_TRIGGER_HOME_NETWORK);
        }
    }

    return OAL_SUCC;
}


uint32_t hmac_roam_show(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    hmac_roam_static_stru *pst_static = OAL_PTR_NULL;
    hmac_roam_config_stru *pst_info = OAL_PTR_NULL;
    uint8_t uc_vap_id;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_show::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_show::pst_roam_info null.}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_static = &pst_roam_info->st_static;
    pst_info = &(pst_roam_info->st_config);

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    if (pst_roam_info->uc_rssi_type == ROAM_ENV_SPARSE_AP) {
        oam_warning_log4(uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_show::default env band[%d] scan_orthogonal[%d], c_trigger_rssi_2G=%d c_trigger_rssi_5G=%d}",
            pst_info->uc_scan_band, pst_info->uc_scan_orthogonal,
            pst_info->c_trigger_rssi_2G, pst_info->c_trigger_rssi_5G);
    } else if (pst_roam_info->uc_rssi_type == ROAM_ENV_DENSE_AP) {
        oam_warning_log4(uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_show::dense env band[%d] scan_orthogonal[%d], c_trigger_rssi_2G=%d c_trigger_rssi_5G=%d.}",
            pst_info->uc_scan_band, pst_info->uc_scan_orthogonal,
            pst_info->c_dense_env_trigger_rssi_2G, pst_info->c_dense_env_trigger_rssi_5G);
    } else {
        oam_warning_log3(uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_show::LinkLoss env band[%d] uc_scan_orthogonal[%d] uc_rssi_type[%d]}",
                         pst_info->uc_scan_band, pst_info->uc_scan_orthogonal, pst_roam_info->uc_rssi_type);
    }

    oam_warning_log4(uc_vap_id, OAM_SF_ROAM,
        "{hmac_roam_show::trigger_rssi_cnt[%u] trigger_linkloss_cnt[%u], scan_cnt[%u] scan_result_cnt[%u].}",
        pst_static->ul_trigger_rssi_cnt, pst_static->ul_trigger_linkloss_cnt,
        pst_static->ul_scan_cnt, pst_static->ul_scan_result_cnt);
    oam_warning_log3(uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_show::total_cnt[%u] roam_success_cnt[%u] roam_fail_cnt[%u].}",
                     pst_static->ul_connect_cnt, pst_static->ul_roam_new_cnt, pst_static->ul_roam_old_cnt);
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    oam_warning_log3(uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_show::roam_to, scan fail num=%d, %d(11v), eap fail num=%d}",
                     pst_roam_info->st_static.ul_roam_scan_fail,
                     pst_roam_info->st_static.ul_roam_11v_scan_fail,
                     pst_roam_info->st_static.ul_roam_eap_fail);
#else
    oam_warning_log2(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_show::roam_to, scan fail=%d, eap fail=%d}",
                     pst_roam_info->st_static.ul_roam_scan_fail,
                     pst_roam_info->st_static.ul_roam_eap_fail);
#endif

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_main_null_fn(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_main_null_fn .}");

    return OAL_SUCC;
}


OAL_STATIC void hmac_roam_scan_comp_cb(void *p_scan_record)
{
    hmac_scan_record_stru *pst_scan_record = (hmac_scan_record_stru *)p_scan_record;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_scan_bss_mgmt = OAL_PTR_NULL;

    /* 获取hmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_scan_record->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_roam_scan_comp_cb::pst_hmac_vap is null.");
        return;
    }

    /* 获取hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_scan_complete::device null!}");
        return;
    }

    pst_scan_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        return;
    }

    /* 漫游开关没有开时，不处理扫描结果 */
    if (pst_roam_info->uc_enable == 0) {
        return;
    }

    oam_info_log0(pst_scan_record->uc_vap_id, OAM_SF_ROAM, "{hmac_roam_scan_complete::handling scan result!}");

    hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_SCAN_RESULT, (void *)pst_scan_bss_mgmt);

    return;
}

#ifdef _PRE_WLAN_FEATURE_11K

oal_bool_enum_uint8 hmac_roam_is_neighbor_report_allowed(hmac_vap_stru *pst_hmac_vap)
{
    mac_bss_dscr_stru *pst_curr_bss_dscr = OAL_PTR_NULL;
    hmac_roam_info_stru *p_roam_info = OAL_PTR_NULL;
    uint8_t *puc_cowork_ie = OAL_PTR_NULL;
    uint8_t *puc_frame_body = OAL_PTR_NULL;
    uint16_t us_frame_body_len;
    uint16_t us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    oal_sta_ap_cowork_ie_beacon *pst_cowork_ie = OAL_PTR_NULL;

    pst_curr_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&pst_hmac_vap->st_vap_base_info,
        pst_hmac_vap->st_vap_base_info.auc_bssid);
    if (pst_curr_bss_dscr == OAL_PTR_NULL || pst_curr_bss_dscr->en_support_neighbor != OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_neighbor_report_allowed:: disallowed neighbor report}");
        return OAL_FALSE;
    }

    p_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (p_roam_info == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    /* c_current_rssi < -85 不做neighbor */
    if (p_roam_info->st_alg.c_current_rssi < ROAM_RSSI_NE85_DB) {
        return OAL_FALSE;
    }

    puc_frame_body = pst_curr_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN;
    us_frame_body_len = pst_curr_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN;
    puc_cowork_ie = mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI,
                                           MAC_WLAN_OUI_TYPE_HAUWEI_COWORK,
                                           puc_frame_body + us_offset,
                                           us_frame_body_len - us_offset);

    if (puc_cowork_ie == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_neighbor_report_allowed::cowork ie not found}");
        return OAL_TRUE;
    }

    pst_cowork_ie = (oal_sta_ap_cowork_ie_beacon *)puc_cowork_ie;

    if (pst_cowork_ie->capa_bitmap.support_neighbor_report != 1) {
        oam_warning_log0(0, OAM_SF_ROAM,
                         "{hmac_roam_is_neighbor_report_allowed::support_neighbor_report in cowork ie is disabled}");
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}
#endif


OAL_STATIC uint32_t hmac_roam_scan_init(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    uint8_t *puc_cur_ssid = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_INIT);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_scan_init::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    if (mac_is_wep_enabled(&(pst_roam_info->pst_hmac_vap->st_vap_base_info))) {
        hmac_roam_rssi_trigger_type((pst_roam_info->pst_hmac_vap), ROAM_ENV_LINKLOSS);
        return OAL_SUCC;
    }

    pst_mac_vap = &(pst_roam_info->pst_hmac_vap->st_vap_base_info);
    hmac_roam_stop_p2p_listen(pst_mac_vap);

    pst_scan_params = &pst_roam_info->st_scan_h2d_params.st_scan_params;
    puc_cur_ssid = mac_mib_get_DesiredSSID(&(pst_roam_info->pst_hmac_vap->st_vap_base_info));

    /* 扫描参数初始化 */
    pst_scan_params->en_bss_type = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type = WLAN_SCAN_TYPE_ACTIVE;
    pst_scan_params->us_scan_time = WLAN_DEFAULT_ACTIVE_SCAN_TIME;
    pst_scan_params->uc_probe_delay = 0;
    pst_scan_params->uc_scan_func = MAC_SCAN_FUNC_BSS; /* 默认扫描bss */
    pst_scan_params->p_fn_cb = hmac_roam_scan_comp_cb;
    pst_scan_params->uc_max_send_probe_req_count_per_channel = 2;
    pst_scan_params->uc_max_scan_count_per_channel = 2;
    pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_ROAM_SCAN;
    pst_scan_params->bit_desire_fast_scan = pst_mac_vap->bit_roam_scan_valid_rslt;

    if (EOK != memcpy_s(pst_scan_params->ast_mac_ssid_set[0].auc_ssid,
                        WLAN_SSID_MAX_LEN, puc_cur_ssid, WLAN_SSID_MAX_LEN)) {
        oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_scan_init::memcpy fail!");
    }
    pst_scan_params->uc_ssid_num = 1;

    /* 初始扫描请求只指定1个bssid，为广播地址 */
    memset_s(pst_scan_params->auc_bssid, WLAN_MAC_ADDR_LEN, 0xff, WLAN_MAC_ADDR_LEN);
    pst_scan_params->uc_bssid_num = 1;

#ifdef _PRE_WLAN_FEATURE_11K
    if (pst_roam_info->pst_hmac_vap->bit_nb_rpt_11k
        && pst_roam_info->en_roam_trigger == ROAM_TRIGGER_DMAC
        && pst_roam_info->pst_hmac_vap->bit_11k_enable
        && hmac_roam_is_neighbor_report_allowed(pst_roam_info->pst_hmac_vap)) {
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_NEIGHBOR_PROCESS);
        ul_ret = hmac_roam_send_neighbor_req(pst_roam_info, puc_cur_ssid);
        if (ul_ret == OAL_SUCC) {
            return ul_ret;
        } else {
            pst_scan_params->uc_neighbor_report_process_flag = OAL_FALSE;
            pst_scan_params->uc_max_scan_count_per_channel = 2;
            memset_s(pst_scan_params->ast_mac_ssid_set[1].auc_ssid, WLAN_SSID_MAX_LEN, 0, WLAN_SSID_MAX_LEN);
            hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_INIT);
        }
    }
#endif

    pst_roam_info->st_static.uc_roam_mode = HMAC_CHR_ROAM_NORMAL;
    pst_roam_info->st_static.uc_scan_mode = HMAC_CHR_NORMAL_SCAN;

    ul_ret = hmac_roam_alg_scan_channel_init(pst_roam_info, pst_scan_params);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_scan_init::hmac_roam_alg_scan_channel_init fail[%d]}", ul_ret);
        return ul_ret;
    }
    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_SCANING);

    ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, (void *)pst_roam_info);
    if (ul_ret != OAL_SUCC) {
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_FAIL);
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
* 功能描述  : update scan channel list params
*/
OAL_STATIC uint32_t scan_params_update(mac_scan_req_stru *scan_params, uint8_t channel_nums_2g,
    uint8_t channel_nums_5g, mac_channel_stru *channelList)
{
    if (channel_nums_2g && memcpy_s(&scan_params->ast_channel_list[0],
        channel_nums_2g * OAL_SIZEOF(mac_channel_stru),
        &channelList[0], channel_nums_2g * OAL_SIZEOF(mac_channel_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ROAM, "{scan_params_update::ast_channel_list[0],memcpy_s fail!}");
        return OAL_FAIL;
    }
    if (channel_nums_5g && memcpy_s(&scan_params->ast_channel_list[channel_nums_2g],
        channel_nums_5g * OAL_SIZEOF(mac_channel_stru),
        &channelList[WLAN_2G_CHANNEL_NUM], channel_nums_5g * OAL_SIZEOF(mac_channel_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ROAM, "{scan_params_update::ast_channel_list[channel_nums_2g],memcpy_s fail!}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_reorder_chan_list(mac_scan_req_h2d_stru *p_s2d_scan_req)
{
    mac_scan_req_stru *p_scan_params = OAL_PTR_NULL;
    mac_channel_stru channelList[WLAN_MAX_CHANNEL_NUM];
    uint8_t channel_nums_2g = 0;
    uint8_t channel_nums_5g = 0;
    uint8_t channel_nums;
    uint8_t chan_idx;

    p_scan_params = &p_s2d_scan_req->st_scan_params;
    channel_nums = p_scan_params->uc_channel_nums;

    /* only scan 1 channel, not need reorder */
    if (channel_nums == 1) {
        return OAL_SUCC;
    }

    for (chan_idx = 0; chan_idx < channel_nums; chan_idx++) {
        if (p_scan_params->ast_channel_list[chan_idx].en_band == WLAN_BAND_2G) {
            if (EOK != memcpy_s(&channelList[channel_nums_2g], OAL_SIZEOF(mac_channel_stru),
                &p_scan_params->ast_channel_list[chan_idx], OAL_SIZEOF(mac_channel_stru))) {
                oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_reorder_chan_list::memcpy fail!");
                return OAL_FAIL;
            }
            channel_nums_2g++;
        } else if (p_scan_params->ast_channel_list[chan_idx].en_band == WLAN_BAND_5G) {
            if (EOK != memcpy_s(&channelList[WLAN_2G_CHANNEL_NUM + channel_nums_5g],
                OAL_SIZEOF(mac_channel_stru), &p_scan_params->ast_channel_list[chan_idx],
                OAL_SIZEOF(mac_channel_stru))) {
                oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_reorder_chan_list::memcpy fail!");
                return OAL_FAIL;
            }
            channel_nums_5g++;
        } else {
            continue;
        }
    }

    if (channel_nums_2g > WLAN_2G_CHANNEL_NUM || channel_nums_5g > WLAN_5G_CHANNEL_NUM) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_reorder_chan_list:: invalid ch num!}");
        return OAL_FAIL;
    }

    /* update scan channel num params if need */
    if (p_scan_params->uc_channel_nums != channel_nums_2g + channel_nums_5g) {
        p_scan_params->uc_channel_nums = channel_nums_2g + channel_nums_5g;
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_reorder_chan_list:: chnum need update [%d]!}",
            p_scan_params->uc_channel_nums);
    }

    /* update scan channel list params */
    if (scan_params_update(p_scan_params, channel_nums_2g, channel_nums_5g, channelList) == OAL_FAIL) {
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_scan_channel(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_SCANING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_scan_channel::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 下发信道列表的顺序需要满足2.4G, 5G */
    ul_ret = hmac_roam_reorder_chan_list(&pst_roam_info->st_scan_h2d_params);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_scan_channel::reorde channel list fail [%d]!}", ul_ret);
        return ul_ret;
    }

    pst_roam_info->st_static.ul_scan_cnt++;

    /* 更新扫描/关联时间戳 */
    pst_roam_info->st_static.ul_scan_start_timetamp = (uint32_t)oal_time_get_stamp_ms();
    pst_roam_info->st_static.ul_scan_end_timetamp = (uint32_t)oal_time_get_stamp_ms();
    pst_roam_info->st_static.ul_connect_start_timetamp = (uint32_t)oal_time_get_stamp_ms();
    pst_roam_info->st_static.ul_connect_end_timetamp = (uint32_t)oal_time_get_stamp_ms();

    /* 发起背景扫描 */
    ul_ret = hmac_fsm_call_func_sta(pst_roam_info->pst_hmac_vap, HMAC_FSM_INPUT_SCAN_REQ,
                                        (void *)(&pst_roam_info->st_scan_h2d_params));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_scan_channel::start scan failed!}");
    }

    /* 启动扫描超时定时器 */
    hmac_roam_main_start_timer(pst_roam_info, ROAM_SCAN_TIME_MAX);

    return OAL_SUCC;
}


uint32_t hmac_roam_check_bkscan_result(hmac_vap_stru *pst_hmac_vap, void *p_scan_record)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = (hmac_scan_record_stru *)p_scan_record;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_scan_bss_mgmt = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_resume_roaming;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_scan_record)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_check_bkscan_result::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_check_bkscan_result::device is null}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 暗屏不进行场景识别 */
    if (pst_hmac_device->pst_device_base_info->uc_in_suspend == OAL_TRUE) {
        return OAL_SUCC;
    }

    if (mac_is_wep_enabled(&pst_hmac_vap->st_vap_base_info)) {
        return OAL_SUCC;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL || pst_roam_info->uc_enable == 0) {
        return OAL_SUCC;
    }

    pst_scan_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);
    pst_roam_info->st_alg.uc_candidate_bss_num = 0;
    pst_roam_info->st_alg.uc_candidate_good_rssi_num = 0;
    pst_roam_info->st_alg.uc_candidate_weak_rssi_num = 0;
    pst_roam_info->st_alg.c_max_rssi = ROAM_RSSI_CMD_TYPE;
    pst_roam_info->st_alg.c_current_rssi = ROAM_RSSI_NE75_DB;  // in case current bss cannot scan

    oal_spin_lock(&(pst_scan_bss_mgmt->st_lock));

    /* 遍历扫描到的bss信息，查找可以漫游的bss */
    oal_dlist_search_for_each(pst_entry, &(pst_scan_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        hmac_roam_alg_bss_in_ess(pst_roam_info, pst_bss_dscr);

        pst_bss_dscr = OAL_PTR_NULL;
    }

    oal_spin_unlock(&(pst_scan_bss_mgmt->st_lock));

    /* 漫游场景识别1:密集AP场景/默认AP场景模式切换(风险:与扫描强相关,仅针对全信道扫描) */
    if ((pst_roam_info->st_config.uc_scenario_enable == OAL_TRUE) &&
        (pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_chan_numbers >= WLAN_FULL_CHANNEL_NUM)) {
        /* 密集AP场景要求: RSSI>=-60dB 的AP个数>=5 或 RSSI在[-75dB,-60dB)的AP个数>=10 */
        if ((pst_roam_info->st_alg.uc_candidate_good_rssi_num >= pst_roam_info->st_config.uc_candidate_good_num) ||
            (pst_roam_info->st_alg.uc_candidate_weak_rssi_num >= pst_roam_info->st_config.uc_candidate_weak_num)) {
            pst_roam_info->st_alg.uc_scan_period = 0;
            oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_check_bkscan_result::roam_to dense AP env: candidate bss total[%d] good[%d] weak[%d]}",
                pst_roam_info->st_alg.uc_candidate_bss_num,
                pst_roam_info->st_alg.uc_candidate_good_rssi_num,
                pst_roam_info->st_alg.uc_candidate_weak_rssi_num);
            if (pst_roam_info->uc_rssi_type != ROAM_ENV_DENSE_AP) {
                hmac_roam_rssi_trigger_type(pst_hmac_vap, ROAM_ENV_DENSE_AP);
            }
        } else {
            /* 连续5次扫描都不是密集AP环境，则切换回默认AP环境模式 */
            pst_roam_info->st_alg.uc_scan_period++;
            if ((pst_roam_info->st_alg.uc_another_bss_scaned) &&
                (pst_roam_info->st_alg.uc_scan_period == ROAM_ENV_DENSE_TO_SPARSE_PERIOD)) {
                oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                    "{hmac_roam_check_bkscan_result::roam_to sparse AP env: candidate bss total[%d] good[%d] weak[%d]}",
                    pst_roam_info->st_alg.uc_candidate_bss_num,
                    pst_roam_info->st_alg.uc_candidate_good_rssi_num,
                    pst_roam_info->st_alg.uc_candidate_weak_rssi_num);

                hmac_roam_rssi_trigger_type(pst_hmac_vap, ROAM_ENV_SPARSE_AP);
            }
        }
    } else if (pst_roam_info->st_alg.uc_another_bss_scaned) {
        hmac_roam_rssi_trigger_type(pst_hmac_vap, ROAM_ENV_SPARSE_AP);
    }

    /* 漫游场景识别2: 非漫游场景恢复，扫描到其他大于-80dB的AP立即恢复 */
    en_resume_roaming = (pst_roam_info->st_alg.uc_another_bss_scaned) &&
                        (pst_roam_info->st_alg.c_max_rssi >= ROAM_RSSI_NE80_DB) &&
                        (pst_roam_info->st_alg.c_max_rssi - pst_roam_info->st_alg.c_current_rssi > ROAM_RSSI_DIFF_4_DB);
    if ((pst_roam_info->uc_rssi_type == ROAM_ENV_LINKLOSS) &&
        (en_resume_roaming == OAL_TRUE)) {
        return hmac_roam_rssi_trigger_type(pst_hmac_vap, ROAM_ENV_SPARSE_AP);
    }

    /* 漫游场景识别3: 关闭漫游 */
    if (OAL_TRUE == hmac_roam_alg_need_to_stop_roam_trigger(pst_roam_info)) {
        return hmac_roam_rssi_trigger_type(pst_hmac_vap, ROAM_ENV_LINKLOSS);
    }

    /* 漫游场景识别4: 当前信号弱于-65dB，高概率(5次有4次)扫到比当前高30dB的AP即可触发漫游 */
    if ((pst_roam_info->st_config.uc_scenario_enable == OAL_TRUE) &&
        (pst_roam_info->st_alg.c_max_rssi - pst_roam_info->st_alg.c_current_rssi >= ROAM_ENV_BETTER_RSSI_DISTANSE) &&
        (pst_roam_info->st_alg.c_current_rssi < ROAM_RSSI_NE65_DB)) {
        if (pst_roam_info->st_alg.uc_better_rssi_null_period >= (ROAM_ENV_BETTER_RSSI_NULL_PERIOD + 1)) {
            pst_roam_info->st_alg.uc_better_rssi_scan_period = 0;  // restart scan period
            pst_roam_info->st_alg.uc_better_rssi_null_period = 0;
        }
        pst_roam_info->st_alg.uc_better_rssi_scan_period++;
        oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_check_bkscan_result::roam_to better AP env:[%d] max_rssi[%d] current_rssi[%d]}",
                         pst_roam_info->st_alg.uc_better_rssi_scan_period,
                         pst_roam_info->st_alg.c_max_rssi, pst_roam_info->st_alg.c_current_rssi);

        if (pst_roam_info->st_alg.uc_better_rssi_scan_period == ROAM_ENV_BETTER_RSSI_PERIOD) {
            pst_roam_info->st_alg.uc_better_rssi_scan_period = 0;
            pst_roam_info->st_alg.uc_better_rssi_null_period = 0;

            hmac_roam_start(pst_hmac_vap, ROAM_SCAN_CHANNEL_ORG_0, OAL_TRUE, NULL, ROAM_TRIGGER_APP);
        }
    } else {
        pst_roam_info->st_alg.uc_better_rssi_null_period++;
    }

    /* 漫游场景识别5: 家庭双频路由器，关联2G AP，可以漫游到RSSI>=-50dB 5G AP，5G weight += 30dBm */

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE

OAL_STATIC uint32_t hmac_roam_11v_send_btm_resp(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = OAL_PTR_NULL;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (oal_any_null_ptr2(pst_roam_info, pst_hmac_vap)) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_11v_send_btm_resp::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_roam_11v_send_btm_resp::mac_res_get_hmac_user failed.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);
    if (pst_11v_ctrl_info == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_roam_11v_send_btm_resp::pst_11v_ctrl_info is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 找到目标AP, 更新BTM resp中携带目标AP的BSSID和Channel num */
    if (pst_bss_dscr != OAL_PTR_NULL) {
        if (EOK != memcpy_s(pst_roam_info->st_bsst_rsp_info.auc_target_bss_addr, WLAN_MAC_ADDR_LEN,
                            pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN)) {
            oam_warning_log0(0, OAM_SF_ROAM, "hmac_roam_11v_send_btm_resp::memcpy fail!");
            return OAL_FAIL;
        }
        pst_roam_info->st_bsst_rsp_info.uc_chl_num = pst_bss_dscr->st_channel.uc_chan_number;
    }

    pst_roam_info->st_bsst_rsp_info.uc_status_code =
        (pst_bss_dscr == OAL_PTR_NULL) ? WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES : WNM_BSS_TM_ACCEPT;

    if (pst_11v_ctrl_info->mac_11v_callback_fn == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "hmac_roam_11v_send_btm_resp::mac_11v_callback_fn is NULL");
        return OAL_FAIL;
    }

    oam_warning_log1(0, OAM_SF_RX, "{hmac_roam_11v_send_btm_resp::start to send btm resp, status_code=%d}",
                     pst_roam_info->st_bsst_rsp_info.uc_status_code);
    pst_11v_ctrl_info->mac_11v_callback_fn(pst_hmac_vap, pst_hmac_user, &(pst_roam_info->st_bsst_rsp_info));
    pst_11v_ctrl_info->mac_11v_callback_fn = OAL_PTR_NULL;
    memset_s(&(pst_roam_info->st_bsst_rsp_info), sizeof(pst_roam_info->st_bsst_rsp_info),
        0, sizeof(pst_roam_info->st_bsst_rsp_info));
    if (pst_roam_info->st_bsst_rsp_info.uc_status_code != WNM_BSS_TM_ACCEPT) {
        pst_roam_info->st_static.ul_roam_11v_scan_fail++;
#ifdef PLATFORM_DEBUG_ENABLE
        hmac_config_reg_info(&(pst_hmac_vap->st_vap_base_info), BSS_TRANSITION_RESP_MSG_LEN, "all");
#endif
    }
    pst_roam_info->st_scan_h2d_params.st_scan_params.uc_bss_transition_process_flag = OAL_FALSE;
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_roam_check_11v_scan_result(hmac_roam_info_stru *pst_roam_info,
                                                      oal_bool_enum_uint8 en_find_bss)
{
    hmac_vap_stru *pst_hmac_vap;
    hmac_user_stru *pst_hmac_user;
    hmac_user_11v_ctrl_stru *pst_11v_ctrl_info = NULL;
    hmac_scan_stru *pst_scan_mgmt = NULL;
    hmac_device_stru *pst_hmac_device = NULL;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    /* 获取发送端的用户指针 */
    pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_roam_check_11v_scan_result::mac_res_get_hmac_user failed.}");
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    if (pst_scan_mgmt == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }

    pst_11v_ctrl_info = &(pst_hmac_user->st_11v_ctrl_info);

    oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                     "{hmac_roam_check_11v_scan_result::en_find_bss=[%d],uc_11v_roam_scan_times=[%d].}",
                     en_find_bss, pst_11v_ctrl_info->uc_11v_roam_scan_times);

    if (en_find_bss == OAL_TRUE) {
        pst_roam_info->st_bsst_rsp_info.uc_status_code = WNM_BSS_TM_ACCEPT;
        if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_5) {
            /* 找到指定bss,本次11v漫游结束 */
            pst_11v_ctrl_info->uc_11v_roam_scan_times = MAC_11V_ROAM_SCAN_FULL_CHANNEL_LIMIT;
        }
    } else {
        pst_hmac_vap->st_vap_base_info.bit_roam_scan_valid_rslt = OAL_FALSE;
        if (pst_roam_info->st_config.uc_scan_orthogonal == ROAM_SCAN_CHANNEL_ORG_5) {
            if ((pst_11v_ctrl_info->en_only_scan_one_time == OAL_FALSE) &&
                (pst_11v_ctrl_info->uc_11v_roam_scan_times <= MAC_11V_ROAM_SCAN_ONE_CHANNEL_LIMIT)) {
                /* 还需要再次触发漫游扫描 */
                pst_scan_mgmt->en_is_scanning = OAL_FALSE;
                return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
            }
        }
        /* When candidate BSSID is not in scan results, bss transition rsp with status code = 7 */
        pst_roam_info->st_bsst_rsp_info.uc_status_code = WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES;
    }

    return (en_find_bss == OAL_FALSE) ? OAL_ERR_CODE_ROAM_NO_VALID_BSS : OAL_SUCC;
}
#endif


int8_t hmac_get_rssi_from_scan_result(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_bssid)
{
    mac_bss_dscr_stru *pst_bss_dscr;

    pst_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&pst_hmac_vap->st_vap_base_info, puc_bssid);
    if (pst_bss_dscr != OAL_PTR_NULL) {
        return pst_bss_dscr->c_rssi;
    }

    return ROAM_RSSI_CMD_TYPE;
}


OAL_STATIC uint32_t hmac_roam_search_bss_in_scan_list(hmac_roam_info_stru *pst_roam_info,
                                                        hmac_bss_mgmt_stru *pst_bss_mgmt,
                                                        mac_bss_dscr_stru *pst_bss_dscr,
                                                        oal_bool_enum_uint8 *bp_search_flag)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_roam_info, pst_bss_mgmt, bp_search_flag)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_search_bss_in_scan_list::NULL pointer}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 如果扫描到的bss个数为0，退出 */
    if (pst_bss_mgmt->ul_bss_num == 0) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_search_bss_in_scan_list::no bss scanned}");
        return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
    }

    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    // 遍历扫描到的bss信息，查找可以漫游的bss
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        if (hmac_roam_alg_bss_check(pst_roam_info, pst_bss_dscr) != OAL_SUCC) {
            continue;
        }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
        if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
            if (hmac_roam_alg_check_bsst_bssid_in_scan_list(pst_roam_info, pst_bss_dscr->auc_mac_addr) == OAL_TRUE) {
                *bp_search_flag = OAL_TRUE;
            }
        }
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */

        pst_bss_dscr = OAL_PTR_NULL;
    }

    oal_spin_unlock(&(pst_bss_mgmt->st_lock));
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_check_scan_result(hmac_roam_info_stru *pst_roam_info,
                                                  void *p_param,
                                                  mac_bss_dscr_stru **ppst_bss_dscr_out)
{
    uint32_t ul_ret;
    hmac_bss_mgmt_stru *pst_bss_mgmt = (hmac_bss_mgmt_stru *)p_param;
    hmac_vap_stru *pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    oal_bool_enum_uint8 bflag_in_scan_results = OAL_FALSE;
#endif
    if (oal_any_null_ptr2(pst_hmac_vap, pst_bss_mgmt)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_check_scan_result::vap invalid!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_roam_info->st_static.ul_scan_result_cnt++;

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (ROAM_TRIGGER_11V == pst_roam_info->en_roam_trigger) {
        /* 用最新的扫描结果更新当前关联AP的RSSI */
        pst_roam_info->st_alg.c_current_rssi =
            hmac_get_rssi_from_scan_result(pst_hmac_vap, pst_hmac_vap->st_vap_base_info.auc_bssid);

        pst_roam_info->st_bsst_rsp_info.c_rssi = pst_roam_info->st_alg.c_current_rssi;
    }
#endif

    ul_ret = hmac_roam_search_bss_in_scan_list(pst_roam_info, pst_bss_mgmt, pst_bss_dscr, &bflag_in_scan_results);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_check_scan_result::hmac_roam_search_bss_in_scan_list() failed}");
        return ul_ret;
    }

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        if (OAL_SUCC != hmac_roam_check_11v_scan_result(pst_roam_info, bflag_in_scan_results)) {
            goto no_valid_bss;
        }
    }
#endif /* _PRE_WLAN_FEATURE_11V_ENABLE */

    pst_bss_dscr = hmac_roam_alg_select_bss(pst_roam_info);
    if (pst_bss_dscr == OAL_PTR_NULL) {
        /* 没有扫描到可用的bss，等待定时器超时即可 */
        pst_roam_info->st_static.ul_roam_scan_fail++;
        oam_warning_log1(0, OAM_SF_ROAM, "{roam_to::no bss valid fail=%d}", pst_roam_info->st_static.ul_roam_scan_fail);
#ifdef PLATFORM_DEBUG_ENABLE
        hmac_config_reg_info(&(pst_hmac_vap->st_vap_base_info), 4, "all");
#endif
        pst_hmac_vap->st_vap_base_info.bit_roam_scan_valid_rslt = OAL_FALSE;
        goto no_valid_bss;
    }

    ul_ret = hmac_roam_renew_privacy(pst_hmac_vap, pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        pst_bss_dscr = OAL_PTR_NULL;
        goto no_valid_bss;
    }

    ul_ret = hmac_check_capability_mac_phy_supplicant(&(pst_hmac_vap->st_vap_base_info), pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_check_scan_result::check mac and phy cap fail[%d]!}\r\n", ul_ret);
    }

    *ppst_bss_dscr_out = pst_bss_dscr;

no_valid_bss:
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    /* 发送携带状态码的 BTM resp */
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        if (hmac_roam_11v_send_btm_resp(pst_roam_info, pst_bss_dscr) != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_check_scan_result::send btm resp failed}");
        }
    }
#endif

    if (pst_bss_dscr == OAL_PTR_NULL) {
        hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_SCAN_FAIL);
    }
    return (pst_bss_dscr == OAL_PTR_NULL) ? OAL_ERR_CODE_ROAM_NO_VALID_BSS : OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_handle_scan_result(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    hmac_scan_rsp_stru st_scan_rsp;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    if (hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_SCANING) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_handle_scan_result::check_state fail!}");
        return OAL_FAIL;
    }

    pst_mac_device = mac_res_get_dev(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_handle_scan_result::pst_mac_device null.}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    if (hmac_roam_check_scan_result(pst_roam_info, p_param, &pst_bss_dscr) == OAL_SUCC) {
        pst_roam_info->uc_invalid_scan_cnt = 0;
        pst_roam_info->st_static.ul_scan_end_timetamp = (uint32_t)oal_time_get_stamp_ms();

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
        /* 记录11v选择的目标AP */
        if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
            if (memcpy_s(pst_roam_info->st_bsst_rsp_info.auc_target_bss_addr, WLAN_MAC_ADDR_LEN,
                pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) != EOK) {
                return OAL_FAIL;
            }
        }
#endif

        /* 扫描结果发给sme */
        memset_s(&st_scan_rsp, OAL_SIZEOF(st_scan_rsp), 0, OAL_SIZEOF(st_scan_rsp));
        st_scan_rsp.uc_result_code = MAC_SCAN_SUCCESS;
        hmac_send_rsp_to_sme_sta(pst_roam_info->pst_hmac_vap, HMAC_SME_SCAN_RSP, (uint8_t *)&st_scan_rsp);
        /* 扫描到可用的bss，开始connect */
        return hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,(void *)pst_bss_dscr);
    }

    /* 如果是亮屏的，不暂停漫游 */
    pst_roam_info->uc_invalid_scan_cnt =
        (pst_mac_device->uc_in_suspend == OAL_FALSE) ? 0 : pst_roam_info->uc_invalid_scan_cnt + 1;

    /* 多次无效扫描暂停漫游，防止在某些场景下一直唤醒HOST */
    if (pst_roam_info->uc_invalid_scan_cnt >= ROAM_INVALID_SCAN_MAX) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_handle_scan_result::ignore_rssi_trigger after %d invalid_scan.}",
                         pst_roam_info->uc_invalid_scan_cnt);
        pst_roam_info->uc_invalid_scan_cnt = 0;
        hmac_roam_rssi_trigger_type(pst_roam_info->pst_hmac_vap, ROAM_ENV_LINKLOSS);
    }
    /* 删除定时器 */
    hmac_roam_main_del_timer(pst_roam_info);
    hmac_roam_main_clear(pst_roam_info);
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    /* 重新出发漫游检查 */
    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_11V) {
        hmac_11v_roam_scan_check(pst_roam_info->pst_hmac_vap);
    }
#endif
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_main_check_state(hmac_roam_info_stru *pst_roam_info,
                                                 mac_vap_state_enum_uint8 en_vap_state,
                                                 roam_main_state_enum_uint8 en_main_state)
{
    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_roam_info->pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    if (pst_roam_info->pst_hmac_user == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }

    if (pst_roam_info->uc_enable == 0) {
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    if ((pst_roam_info->pst_hmac_vap->st_vap_base_info.en_vap_state != en_vap_state) ||
        (pst_roam_info->en_main_state != en_main_state)) {
        oam_warning_log2(0, OAM_SF_ROAM, "{hmac_roam_main_check_state::unexpect vap State[%d] main_state[%d]!}",
                         pst_roam_info->pst_hmac_vap->st_vap_base_info.en_vap_state, pst_roam_info->en_main_state);
        return OAL_ERR_CODE_ROAM_INVALID_VAP_STATUS;
    }

    return OAL_SUCC;
}


OAL_STATIC void hmac_roam_main_clear(hmac_roam_info_stru *pst_roam_info)
{
    /* 清理状态 */
    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_INIT);

    hmac_roam_connect_stop(pst_roam_info->pst_hmac_vap);
}


OAL_STATIC uint32_t hmac_roam_resume_pm(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret = OAL_SUCC;

    /* 开低功耗前关闭低功耗超时定时器 */
    if (pst_roam_info->pst_hmac_vap->st_ps_sw_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&pst_roam_info->pst_hmac_vap->st_ps_sw_timer);
    }

    ul_ret = hmac_config_set_pm_by_module(&pst_roam_info->pst_hmac_vap->st_vap_base_info,
                                              MAC_STA_PM_CTRL_TYPE_ROAM,
                                              MAC_STA_PM_SWITCH_ON);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "{hmac_roam_resume_pm::hmac_config_set_pm_by_module failed[%d].}", ul_ret);
    }

    return ul_ret;
}


OAL_STATIC uint32_t hmac_roam_resume_security_port(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    mac_h2d_roam_sync_stru st_h2d_sync = { 0 };

    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_resume_security_port::pst_roam_info null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_roam_info->pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_resume_security_port::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    if (pst_roam_info->pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_resume_security_port::user null!}");
        return OAL_ERR_CODE_ROAM_INVALID_USER;
    }

    /* 设置用户8021x端口合法性的状态为合法 */
    mac_user_set_port(&pst_roam_info->pst_hmac_user->st_user_base_info, OAL_TRUE);

    // 填充同步信息
    st_h2d_sync.ul_back_to_old = OAL_FALSE;

    // 发送同步信息
    ul_ret = hmac_config_send_event(&pst_roam_info->pst_hmac_vap->st_vap_base_info,
                                        WLAN_CFGID_ROAM_HMAC_SYNC_DMAC,
                                        OAL_SIZEOF(mac_h2d_roam_sync_stru),
                                        (uint8_t *)&st_h2d_sync);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_resume_security_port::send event[WLAN_CFGID_ROAM_HMAC_SYNC_DMAC] failed[%d].}", ul_ret);
    }

    return ul_ret;
}


OAL_STATIC uint32_t hmac_roam_scan_timeout(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_SCANING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_scan_timeout::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    hmac_roam_scan_complete(pst_roam_info->pst_hmac_vap);
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_connecting_timeout(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    oal_bool_enum_uint8 post_dmac = OAL_TRUE;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_ROAMING, ROAM_MAIN_STATE_CONNECTING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_connecting_timeout::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_TIMEOUT_FAIL);

    pst_roam_info->st_connect.uc_roam_main_fsm_state = ROAM_MAIN_FSM_EVENT_TIMEOUT;
    ul_ret = hmac_roam_to_old_bss(pst_roam_info, p_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM,
                         "{hmac_roam_connecting_timeout:: hmac_roam_to_old_bss fail[%d]!}", ul_ret);
    }

    /* 握手失败不需要恢复device漫游状态 */
    if (pst_roam_info->st_connect.en_state == ROAM_CONNECT_STATE_HANDSHAKING) {
        post_dmac = OAL_FALSE;
    }

    /* 切换vap的状态为UP，恢复用户节能，恢复发送 */
    ul_ret = hmac_fsm_call_func_sta(pst_roam_info->pst_hmac_vap, HMAC_FSM_INPUT_ROAMING_STOP,
        (void *)&post_dmac);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ROAM,
                       "{hmac_roam_connecting_timeout:: hmac_fsm_call_func_sta fail[%d]!}", ul_ret);
    }

    hmac_roam_main_clear(pst_roam_info);

    return ul_ret;
}


OAL_STATIC uint32_t hmac_roam_connecting_fail(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_is_protected;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_ROAMING, ROAM_MAIN_STATE_CONNECTING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_connecting_fail::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_CONNECT_FAIL);

    pst_roam_info->st_connect.uc_roam_main_fsm_state = ROAM_MAIN_FSM_EVENT_CONNECT_FAIL;
    hmac_roam_main_del_timer(pst_roam_info);
    ul_ret = hmac_roam_connecting_timeout(pst_roam_info, p_param);
    if (ul_ret) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_connecting_fail::hmac_roam_to_old_bss fail ul_ret=[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 为了提高漫游成功的概率，在Auth/Reassoc No rsp时立即触发重新漫游 */
    if (pst_roam_info->st_alg.c_current_rssi > ROAM_RSSI_MAX_TYPE) {
        /* 如果是弱信号触发的漫游，先把rssi修改成ROAM_RSSI_LINKLOSS_TYPE来将弱信号跟LINKLOSS触发的重漫游归一 */
        if (pst_roam_info->st_alg.c_current_rssi > ROAM_RSSI_LINKLOSS_TYPE) {
            pst_roam_info->st_alg.c_current_rssi = ROAM_RSSI_LINKLOSS_TYPE;
        }
        /* 漫游失败时，rssi 逐次减1dBm，一直到到ROAM_RSSI_MAX_TYPE。这样可以最多触发5次重漫游 */
        return hmac_roam_trigger_handle(pst_roam_info->pst_hmac_vap,
                                            pst_roam_info->st_alg.c_current_rssi - 1, OAL_TRUE);
    }

    
    if (pst_roam_info->st_connect.en_status_code == MAC_REJECT_TEMP) {
        return ul_ret;
    }

    /* 管理帧加密是否开启 */
    en_is_protected = pst_roam_info->pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active;

    /* 发去关联帧 */
    hmac_mgmt_send_disassoc_frame(&(pst_roam_info->pst_hmac_vap->st_vap_base_info),
                                      pst_roam_info->pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                      MAC_DEAUTH_LV_SS, en_is_protected);

    /* 删除对应用户 */
    hmac_user_del(&(pst_roam_info->pst_hmac_vap->st_vap_base_info), pst_roam_info->pst_hmac_user);

    hmac_sta_handle_disassoc_rsp(pst_roam_info->pst_hmac_vap, MAC_NOT_ASSOCED);

    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                         CHR_LAYER_DRV, CHR_WIFI_DEV_EVENT_CHIP, ROAM_FAIL_FIVE_TIMES);

    return ul_ret;
}


OAL_STATIC uint32_t hmac_roam_handle_fail_handshake_phase(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_is_protected;
    oal_bool_enum_uint8 post_dmac = OAL_FALSE; /* 握手失败不需要恢复device漫游状态 */

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_ROAMING, ROAM_MAIN_STATE_CONNECTING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_handle_fail_handshake_phase::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }
    pst_roam_info->st_connect.uc_roam_main_fsm_state = ROAM_MAIN_FSM_EVENT_HANDSHAKE_FAIL;

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_HANDSHAKE_FAIL);

    /* 切换vap的状态为UP，恢复用户节能，恢复发送 */
    ul_ret = hmac_fsm_call_func_sta(pst_roam_info->pst_hmac_vap, HMAC_FSM_INPUT_ROAMING_STOP, &post_dmac);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_handle_fail_handshake_phase:: hmac_fsm_call_func_sta fail[%d]!}", ul_ret);
    }

    hmac_roam_main_clear(pst_roam_info);
    hmac_roam_main_del_timer(pst_roam_info);

    /* 为提高漫游成功的概率，在握手失败时触发立即重新漫游 */
    if (pst_roam_info->st_alg.c_current_rssi > ROAM_RSSI_MAX_TYPE) {
        /* 如果是弱信号触发的漫游，先把rssi修改成ROAM_RSSI_LINKLOSS_TYPE来将弱信号跟LINKLOSS触发的重漫游归一 */
        if (pst_roam_info->st_alg.c_current_rssi > ROAM_RSSI_LINKLOSS_TYPE) {
            pst_roam_info->st_alg.c_current_rssi = ROAM_RSSI_LINKLOSS_TYPE;
        }
        /* 漫游握手失败时，rssi 逐次减1dBm，一直到到ROAM_RSSI_MAX_TYPE。这样可以最多触发5次重漫游 */
        return hmac_roam_trigger_handle(pst_roam_info->pst_hmac_vap,
                                            pst_roam_info->st_alg.c_current_rssi - 1, OAL_TRUE);
    }

    oam_warning_log1(0, OAM_SF_ROAM,
                     "{hmac_roam_handle_fail_handshake_phase:: report deauth to wpas! c_current_rssi=%d}",
                     pst_roam_info->st_alg.c_current_rssi);

    /* 管理帧加密是否开启 */
    en_is_protected = pst_roam_info->pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active;

    /* 发去关联帧 */
    hmac_mgmt_send_disassoc_frame(&(pst_roam_info->pst_hmac_vap->st_vap_base_info),
                                      pst_roam_info->pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                      MAC_DEAUTH_LV_SS, en_is_protected);

    /* 删除对应用户 */
    hmac_user_del(&(pst_roam_info->pst_hmac_vap->st_vap_base_info), pst_roam_info->pst_hmac_user);

    hmac_sta_handle_disassoc_rsp(pst_roam_info->pst_hmac_vap, MAC_4WAY_HANDSHAKE_TIMEOUT);

    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                         CHR_LAYER_DRV, CHR_WIFI_DEV_EVENT_CHIP, ROAM_FAIL_FIVE_TIMES);

    return ul_ret;
}

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R

OAL_STATIC oal_bool_enum_uint8 hmac_roam_is_over_ds_allowed(hmac_roam_info_stru *pst_roam_info)
{
    hmac_vap_stru *pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    mac_bss_dscr_stru *pst_curr_bss_dscr;
    uint8_t *puc_cowork_ie;
    uint8_t *puc_frame_body;
    uint16_t us_frame_body_len;
    uint16_t us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    oal_sta_ap_cowork_ie_beacon *pst_cowork_ie;

    /* 检查是否满足over ds的两个条件: 1. IsWithCoworkIE 2. current_AP_RSSI>-80 3. uc_ft_failed不为ture */
    pst_curr_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&pst_hmac_vap->st_vap_base_info,
        pst_hmac_vap->st_vap_base_info.auc_bssid);
    if (pst_curr_bss_dscr == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_over_ds_allowed::curr_bss_dscr is null}");
        return OAL_FALSE;
    }

    if (pst_curr_bss_dscr->c_rssi < -80) {
        oam_warning_log1(0, OAM_SF_ROAM,
                         "{hmac_roam_is_over_ds_allowed::current rssi = [%d] is lower than -80 dbm}",
                         pst_curr_bss_dscr->c_rssi);
        return OAL_FALSE;
    }

    if (pst_roam_info->st_connect.uc_ft_failed == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_over_ds_allowed::11r over ds has been failed yet}");
        return OAL_FALSE;
    }

    puc_frame_body = pst_curr_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN;
    us_frame_body_len = pst_curr_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN;
    puc_cowork_ie = mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI,
                                           MAC_WLAN_OUI_TYPE_HAUWEI_COWORK,
                                           puc_frame_body + us_offset,
                                           us_frame_body_len - us_offset);
    if (puc_cowork_ie == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_over_ds_allowed::cowork ie not found}");
        return OAL_FALSE;
    }

    pst_cowork_ie = (oal_sta_ap_cowork_ie_beacon *)puc_cowork_ie;

    if (pst_cowork_ie->capa_bitmap.ft_over_ds != 1) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_is_over_ds_allowed::ft_over_ds in cowork ie is disabled}");
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}
#endif

OAL_STATIC uint32_t hmac_roam_connect_check_other_vap(hmac_roam_info_stru *roam_info, mac_bss_dscr_stru *bss_dscr)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    mac_vap_stru *other_vap = OAL_PTR_NULL;
    mac_device_stru *mac_device;

    mac_device = (mac_device_stru *)mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (mac_device == NULL) {
        oam_error_log0(0, OAM_SF_ROAM,
                       "{hmac_roam_connect_check_other_vap::pst_mac_device ptr is null!}");
        return OAL_FAIL;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        other_vap = mac_res_get_mac_vap(mac_device->auc_vap_id[vap_idx]);
        if (other_vap == NULL) {
            oam_warning_log0(mac_device->auc_vap_id[vap_idx], OAM_SF_ROAM,
                             "{hmac_roam_connect_check_other_vap::vap is null!}");
            continue;
        }
        /* check all vap state in case other vap is connecting now */
        if (oal_value_in_valid_range(other_vap->en_vap_state,
                                     MAC_VAP_STATE_STA_JOIN_COMP,
                                     MAC_VAP_STATE_STA_WAIT_ASOC)) {
            oam_warning_log1(other_vap->uc_vap_id, OAM_SF_ROAM,
                             "{hmac_roam_connect_check_other_vap::vap is connecting, state=[%d]!}",
                             other_vap->en_vap_state);
            hmac_roam_main_change_state(roam_info, ROAM_MAIN_STATE_INIT);
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_roam_connect_to_bss(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    hmac_vap_stru *pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    hmac_user_stru *pst_hmac_user = pst_roam_info->pst_hmac_user;
    mac_bss_dscr_stru *pst_bss_dscr = (mac_bss_dscr_stru *)p_param;
    hmac_roam_old_bss_stru *pst_old_bss = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t ul_need_to_stop_user = 1;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_SCANING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_connect_to_bss::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    if (hmac_roam_connect_check_other_vap(pst_roam_info, pst_bss_dscr) != OAL_SUCC) {
        return -OAL_EFAIL;
    }

#ifdef _PRE_WLAN_FEATURE_11R
    if (OAL_TRUE == (mac_mib_get_ft_trainsistion(&pst_hmac_vap->st_vap_base_info) &&
                     mac_mib_get_ft_over_ds(&pst_hmac_vap->st_vap_base_info))) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_connect_to_bss::cmd bit_11r_over_ds = [%d]!}",
                         pst_hmac_vap->bit_11r_over_ds);

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R
        if (OAL_TRUE == hmac_roam_is_over_ds_allowed(pst_roam_info)) {
            pst_roam_info->st_connect.uc_ft_force_air = OAL_FALSE;
        }
#endif

        /* modify仅第一次使用ds方式 */
        if ((pst_hmac_vap->bit_11r_over_ds == OAL_TRUE) && (pst_roam_info->st_connect.uc_ft_force_air != OAL_TRUE)) {
            ul_need_to_stop_user = 0;
        }
    }
#endif  // _PRE_WLAN_FEATURE_11R

    if (ul_need_to_stop_user != 0) {
        pst_roam_info->st_static.ul_connect_start_timetamp = (uint32_t)oal_time_get_stamp_ms();

        /* 切换vap的状态为ROAMING，将用户节能，暂停发送 */
        ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_ROAMING_START, (void *)pst_bss_dscr);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ROAM,
                             "{hmac_roam_connect_to_bss::hmac_fsm_call_func_sta fail[%ld]!}", ul_ret);
            return ul_ret;
        }
    }

    /* 原bss信息保存，以便回退 */
    pst_old_bss = &pst_roam_info->st_old_bss;
    pst_old_bss->us_sta_aid = pst_hmac_vap->st_vap_base_info.us_sta_aid;
    pst_old_bss->en_protocol_mode = pst_hmac_vap->st_vap_base_info.en_protocol;
    memcpy_s(&pst_old_bss->st_cap_info, OAL_SIZEOF(mac_user_cap_info_stru),
             &(pst_hmac_user->st_user_base_info.st_cap_info), OAL_SIZEOF(mac_user_cap_info_stru));
    memcpy_s(&pst_old_bss->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru),
             &(pst_hmac_user->st_user_base_info.st_key_info), OAL_SIZEOF(mac_key_mgmt_stru));
    memcpy_s(&pst_old_bss->st_user_tx_info, OAL_SIZEOF(mac_user_tx_param_stru),
             &(pst_hmac_user->st_user_base_info.st_user_tx_info), OAL_SIZEOF(mac_user_tx_param_stru));
    memcpy_s(&pst_old_bss->st_mib_info, OAL_SIZEOF(wlan_mib_ieee802dot11_stru),
             pst_hmac_vap->st_vap_base_info.pst_mib_info, OAL_SIZEOF(wlan_mib_ieee802dot11_stru));
    if (EOK != memcpy_s(&pst_old_bss->st_op_rates, OAL_SIZEOF(mac_rate_stru),
                        &(pst_hmac_user->st_op_rates), OAL_SIZEOF(mac_rate_stru))) {
        oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_connect_to_bss::memcpy fail!");
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_INIT);
        return -OAL_EFAIL;
    }
    memcpy_s(&pst_old_bss->st_ht_hdl, OAL_SIZEOF(mac_user_ht_hdl_stru),
             &(pst_hmac_user->st_user_base_info.st_ht_hdl), OAL_SIZEOF(mac_user_ht_hdl_stru));
    memcpy_s(&pst_old_bss->st_vht_hdl, OAL_SIZEOF(mac_vht_hdl_stru),
             &(pst_hmac_user->st_user_base_info.st_vht_hdl), OAL_SIZEOF(mac_vht_hdl_stru));
    pst_old_bss->en_avail_bandwidth = pst_hmac_user->st_user_base_info.en_avail_bandwidth;
    pst_old_bss->en_cur_bandwidth = pst_hmac_user->st_user_base_info.en_cur_bandwidth;
    memcpy_s(&pst_old_bss->st_channel, OAL_SIZEOF(mac_channel_stru),
             &(pst_hmac_vap->st_vap_base_info.st_channel), OAL_SIZEOF(mac_channel_stru));
    memcpy_s(&pst_old_bss->auc_bssid, WLAN_MAC_ADDR_LEN,
             &(pst_hmac_vap->st_vap_base_info.auc_bssid), WLAN_MAC_ADDR_LEN);
    pst_old_bss->us_cap_info = pst_hmac_vap->st_vap_base_info.us_assoc_user_cap_info;
    pst_old_bss->en_ap_type = pst_hmac_user->en_user_ap_type;

    /* 切换状态至connecting */
    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_CONNECTING);

    pst_hmac_user->ul_rssi_last_timestamp = 0;
    pst_roam_info->st_static.ul_connect_cnt++;

    /* 设置漫游到的bss能力位，重关联请求使用 */
    pst_hmac_vap->st_vap_base_info.us_assoc_user_cap_info = pst_bss_dscr->us_cap_info;

    hmac_config_set_mgmt_log(&pst_hmac_vap->st_vap_base_info, &pst_hmac_user->st_user_base_info, OAL_TRUE);

    /* 启动connect状态机 */
    ul_ret = hmac_roam_connect_start(pst_hmac_vap, pst_bss_dscr);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_connect_to_bss::hmac_roam_connect_start fail[%ld]!}", ul_ret);
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_FAIL);
        return ul_ret;
    }

    pst_roam_info->st_connect.uc_roam_main_fsm_state = ROAM_MAIN_FSM_EVENT_START_CONNECT;

    /* 启动connect超时定时器 */
    hmac_roam_main_start_timer(pst_roam_info, ROAM_CONNECT_TIME_MAX);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_to_old_bss(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    uint32_t ul_ret;
    hmac_vap_stru *pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    hmac_user_stru *pst_hmac_user = pst_roam_info->pst_hmac_user;
    hmac_roam_old_bss_stru *pst_old_bss = &pst_roam_info->st_old_bss;
    mac_h2d_roam_sync_stru *pst_h2d_sync = OAL_PTR_NULL;

    if (pst_old_bss == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_to_old_bss::pst_old_bss null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info->st_static.ul_roam_old_cnt++;
    pst_roam_info->st_static.ul_roam_eap_fail++;

    /* 恢复原来bss相关信息 */
    pst_hmac_vap->st_vap_base_info.us_sta_aid = pst_old_bss->us_sta_aid;
    pst_hmac_vap->st_vap_base_info.en_protocol = pst_old_bss->en_protocol_mode;
    memcpy_s(&(pst_hmac_user->st_user_base_info.st_cap_info), OAL_SIZEOF(mac_user_cap_info_stru),
             &pst_old_bss->st_cap_info, OAL_SIZEOF(mac_user_cap_info_stru));
    memcpy_s(&(pst_hmac_user->st_user_base_info.st_key_info), OAL_SIZEOF(mac_key_mgmt_stru),
             &pst_old_bss->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru));
    memcpy_s(&(pst_hmac_user->st_user_base_info.st_user_tx_info), OAL_SIZEOF(mac_user_tx_param_stru),
             &pst_old_bss->st_user_tx_info, OAL_SIZEOF(mac_user_tx_param_stru));
    memcpy_s(pst_hmac_vap->st_vap_base_info.pst_mib_info, OAL_SIZEOF(wlan_mib_ieee802dot11_stru),
             &pst_old_bss->st_mib_info, OAL_SIZEOF(wlan_mib_ieee802dot11_stru));
    if (EOK != memcpy_s(&(pst_hmac_user->st_op_rates), OAL_SIZEOF(hmac_rate_stru),
                        &pst_old_bss->st_op_rates, OAL_SIZEOF(mac_rate_stru))) {
        oam_error_log0(0, OAM_SF_ROAM, "hmac_roam_to_old_bss::memcpy fail!");
        return OAL_FAIL;
    }
    memcpy_s(&(pst_hmac_user->st_user_base_info.st_ht_hdl), OAL_SIZEOF(mac_user_ht_hdl_stru),
             &pst_old_bss->st_ht_hdl, OAL_SIZEOF(mac_user_ht_hdl_stru));
    memcpy_s(&(pst_hmac_user->st_user_base_info.st_vht_hdl), OAL_SIZEOF(mac_vht_hdl_stru),
             &pst_old_bss->st_vht_hdl, OAL_SIZEOF(mac_vht_hdl_stru));
    pst_hmac_user->st_user_base_info.en_avail_bandwidth = pst_old_bss->en_avail_bandwidth;
    pst_hmac_user->st_user_base_info.en_cur_bandwidth = pst_old_bss->en_cur_bandwidth;
    memcpy_s(&(pst_hmac_vap->st_vap_base_info.st_channel), OAL_SIZEOF(mac_channel_stru),
             &pst_old_bss->st_channel, OAL_SIZEOF(mac_channel_stru));
    memcpy_s(pst_hmac_vap->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN,
             pst_old_bss->auc_bssid, WLAN_MAC_ADDR_LEN);
    pst_hmac_vap->st_vap_base_info.us_assoc_user_cap_info = pst_old_bss->us_cap_info;
    pst_hmac_user->en_user_ap_type = pst_old_bss->en_ap_type;

    /* 设置用户8021x端口合法性的状态为合法 */
    mac_user_set_port(&pst_hmac_user->st_user_base_info, OAL_TRUE);

    ul_ret = hmac_config_start_vap_event(&pst_hmac_vap->st_vap_base_info, OAL_FALSE);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_to_old_bss::hmac_config_start_vap_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 相关参数需要配置到dmac */
    hmac_roam_connect_set_join_reg(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

    /* 更新用户的mac地址，漫游时mac会更新 */
    oal_set_mac_addr(pst_hmac_user->st_user_base_info.auc_user_mac_addr, pst_hmac_vap->st_vap_base_info.auc_bssid);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    /* 此函数要在同步user能力前调用，同步user能力后会开启硬件PMF使能 */
    hmac_config_vap_pmf_cap(&pst_hmac_vap->st_vap_base_info, pst_old_bss->st_cap_info.bit_pmf_active);
#endif

    ul_ret = hmac_config_user_info_syn(&(pst_hmac_vap->st_vap_base_info), &(pst_hmac_user->st_user_base_info));
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_to_old_bss::hmac_syn_vap_state failed[%d].}", ul_ret);
    }

    /* 回退 bss 时，hmac 2 dmac 同步的相关信息，以便失败的时候回退 */
    pst_h2d_sync = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(mac_h2d_roam_sync_stru), OAL_TRUE);
    if (pst_h2d_sync == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_to_old_bss::no buff to alloc sync info!}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    // 填充同步信息
    pst_h2d_sync->ul_back_to_old = OAL_TRUE;
    pst_h2d_sync->us_sta_aid = pst_old_bss->us_sta_aid;
    memcpy_s(&(pst_h2d_sync->st_channel), OAL_SIZEOF(mac_channel_stru),
             &pst_old_bss->st_channel, OAL_SIZEOF(mac_channel_stru));
    memcpy_s(&(pst_h2d_sync->st_cap_info), OAL_SIZEOF(mac_user_cap_info_stru),
             &pst_old_bss->st_cap_info, OAL_SIZEOF(mac_user_cap_info_stru));
    memcpy_s(&(pst_h2d_sync->st_key_info), OAL_SIZEOF(mac_key_mgmt_stru),
             &pst_old_bss->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru));
    memcpy_s(&(pst_h2d_sync->st_user_tx_info), OAL_SIZEOF(mac_user_tx_param_stru),
             &pst_old_bss->st_user_tx_info, OAL_SIZEOF(mac_user_tx_param_stru));

    /* 在漫游过程中可能又建立了聚合，因此回退时需要删除掉 */
    hmac_tid_clear(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

    // 发送同步信息
    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info, WLAN_CFGID_ROAM_HMAC_SYNC_DMAC,
        OAL_SIZEOF(mac_h2d_roam_sync_stru), (uint8_t *)pst_h2d_sync);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_to_old_bss::send event[WLAN_CFGID_ROAM_HMAC_SYNC_DMAC] failed[%d].}", ul_ret);
    }

    /* 释放同步数据 */
    if (pst_h2d_sync != OAL_PTR_NULL) {
        oal_mem_free_m(pst_h2d_sync, OAL_TRUE);
    }

    /* user已经关联上，抛事件给DMAC，在DMAC层挂用户算法钩子 */
    hmac_user_add_notify_alg(&pst_hmac_vap->st_vap_base_info, pst_hmac_user->st_user_base_info.us_assoc_id);
    hmac_config_set_mgmt_log(&pst_hmac_vap->st_vap_base_info, &pst_hmac_user->st_user_base_info, OAL_FALSE);
    pst_hmac_user->ul_rssi_last_timestamp = 0;

    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_to_old_bss::now resuming to [%02X:XX:XX:%02X:%02X:%02X]}",
                     pst_old_bss->auc_bssid[0], pst_old_bss->auc_bssid[3],
                     pst_old_bss->auc_bssid[4], pst_old_bss->auc_bssid[5]);
#ifdef PLATFORM_DEBUG_ENABLE
    hmac_config_reg_info(&(pst_hmac_vap->st_vap_base_info), 4, "all");
#endif
    return ul_ret;
}


OAL_STATIC uint32_t hmac_roam_to_new_bss(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    hmac_vap_stru *pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    hmac_user_stru *pst_hmac_user = pst_roam_info->pst_hmac_user;
    uint32_t ul_ret;
    wlan_roam_main_band_state_enum_uint8 uc_roam_band_state;
    oal_bool_enum_uint8 post_dmac = OAL_TRUE;

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_ROAMING, ROAM_MAIN_STATE_CONNECTING);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_to_new_bss::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    pst_roam_info->st_connect.uc_roam_main_fsm_state = ROAM_MAIN_FSM_EVENT_CONNECT_SUCC;
    pst_roam_info->st_static.ul_roam_new_cnt++;
    hmac_roam_alg_add_history(pst_roam_info, pst_hmac_vap->st_vap_base_info.auc_bssid);

    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_UP);

    hmac_roam_main_del_timer(pst_roam_info);

    /* 切换vap的状态为UP，恢复用户节能，恢复发送 */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_ROAMING_STOP, (void *)&post_dmac);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_to_new_bss::hmac_fsm_call_func_sta fail! erro code is %u}", ul_ret);
    }
    hmac_config_set_mgmt_log(&pst_hmac_vap->st_vap_base_info, &pst_hmac_user->st_user_base_info, OAL_FALSE);

    pst_hmac_vap->st_vap_base_info.bit_roam_scan_valid_rslt = OAL_TRUE;

    oam_warning_log4(0, OAM_SF_ROAM, "{hmac_roam_to_new_bss::roam to [%02X:XX:XX:%02X:%02X:%02X] succ}",
                     pst_hmac_vap->st_vap_base_info.auc_bssid[0], pst_hmac_vap->st_vap_base_info.auc_bssid[3],
                     pst_hmac_vap->st_vap_base_info.auc_bssid[4], pst_hmac_vap->st_vap_base_info.auc_bssid[5]);
    hmac_roam_main_clear(pst_roam_info);

    /* 标识漫游类型，传到dmac做对应业务处理 */
    if (WLAN_BAND_2G == pst_roam_info->st_old_bss.st_channel.en_band) {
        if (WLAN_BAND_2G == pst_hmac_vap->st_vap_base_info.st_channel.en_band) {
            uc_roam_band_state = WLAN_ROAM_MAIN_BAND_STATE_2TO2;
        } else {
            uc_roam_band_state = WLAN_ROAM_MAIN_BAND_STATE_2TO5;
        }
    } else {
        if (WLAN_BAND_2G == pst_hmac_vap->st_vap_base_info.st_channel.en_band) {
            uc_roam_band_state = WLAN_ROAM_MAIN_BAND_STATE_5TO2;
        } else {
            uc_roam_band_state = WLAN_ROAM_MAIN_BAND_STATE_5TO5;
        }
    }

    /* 漫游成功之后，同步到dmac给相关业务做处理 */
    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info,
        WLAN_CFGID_ROAM_SUCC_H2D_SYNC, OAL_SIZEOF(uint8_t), (uint8_t *)&uc_roam_band_state);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_to_new_bss::send event[WLAN_CFGID_ROAM_SUCC_H2D_SYNC] failed[%d].}", ul_ret);
    }

    /* 如果漫游到5G vap,切PCIE */
    hmac_sdio_to_pcie_switch(&(pst_hmac_vap->st_vap_base_info));

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_SUCCESS);
    return ul_ret;
}


uint32_t hmac_roam_rssi_trigger_type(hmac_vap_stru *pst_hmac_vap, roam_scenario_enum_uint8 en_val)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_vap_id;
    mac_roam_trigger_stru st_roam_trigger;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_rssi_trigger_type::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_rssi_trigger_type::roam info null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    if (pst_roam_info->uc_rssi_type == en_val) {
        return OAL_SUCC;
    }

    if (en_val == ROAM_ENV_LINKLOSS) {
        st_roam_trigger.c_trigger_2G = ROAM_RSSI_LINKLOSS_TYPE;
        st_roam_trigger.c_trigger_5G = ROAM_RSSI_LINKLOSS_TYPE;
        pst_roam_info->st_alg.uc_invalid_scan_cnt = 0;
    } else if (en_val == ROAM_ENV_DENSE_AP) {
        st_roam_trigger.c_trigger_2G = pst_roam_info->st_config.c_dense_env_trigger_rssi_2G;
        st_roam_trigger.c_trigger_5G = pst_roam_info->st_config.c_dense_env_trigger_rssi_5G;
    } else {
        st_roam_trigger.c_trigger_2G = pst_roam_info->st_config.c_trigger_rssi_2G;
        st_roam_trigger.c_trigger_5G = pst_roam_info->st_config.c_trigger_rssi_5G;
    }
    st_roam_trigger.us_roam_interval = pst_roam_info->st_config.us_roam_interval;

    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info,
                                        WLAN_CFGID_SET_ROAM_TRIGGER,
                                        OAL_SIZEOF(mac_roam_trigger_stru),
                                        (uint8_t *)&st_roam_trigger);
    if (ul_ret != OAL_SUCC) {
        oam_error_log2(uc_vap_id, OAM_SF_CFG,
                       "{hmac_roam_rssi_trigger_type::send event[%d] failed[%d].}", en_val, ul_ret);
        return ul_ret;
    }
    pst_roam_info->uc_rssi_type = en_val;

    oam_warning_log1(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_rssi_trigger_type::[%d]}", en_val);
    return OAL_SUCC;
}


uint32_t hmac_roam_pause_user(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_vap_id;
    uint8_t uc_roaming_mode;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_pause_user::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if ((pst_roam_info == OAL_PTR_NULL) || (pst_roam_info->uc_enable == 0)) {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_pause_user::roam disable!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_net_device = pst_hmac_vap->pst_net_device;
    if (pst_net_device == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_pause_user::net_device null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 必须保证vap的状态是UP */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        oam_warning_log1(uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_pause_user::vap state =[%d]NOT FOR ROAMING!}", pst_hmac_vap->st_vap_base_info.en_vap_state);
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    /* 暂停所有协议层数据，这样就不需要再hmac搞一个缓存队列了 */
    oal_net_tx_stop_all_queues(pst_net_device);
    oal_net_wake_subqueue(pst_net_device, WLAN_HI_QUEUE);

    /* 清空 HMAC层TID信息 */
    hmac_tid_clear(&pst_hmac_vap->st_vap_base_info, pst_roam_info->pst_hmac_user);
#ifdef _PRE_WLAN_FEATURE_HIEX
    memset_s(&pst_roam_info->pst_hmac_user->st_ersru, OAL_SIZEOF(mac_hiex_ersru_ctrl_msg_stru),
        0, OAL_SIZEOF(mac_hiex_ersru_ctrl_msg_stru));
#endif
    ul_ret = hmac_config_set_pm_by_module(&pst_hmac_vap->st_vap_base_info,
                                              MAC_STA_PM_CTRL_TYPE_ROAM,
                                              MAC_STA_PM_SWITCH_OFF);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(uc_vap_id, OAM_SF_CFG,
                       "{hmac_roam_pause_user::hmac_config_set_pm_by_module failed[%d].}", ul_ret);
        oal_net_tx_wake_all_queues(pst_net_device);
        return ul_ret;
    }

    uc_roaming_mode = 1;
    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info, WLAN_CFGID_SET_ROAMING_MODE,
                                        OAL_SIZEOF(uint8_t), (uint8_t *)&uc_roaming_mode);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(uc_vap_id, OAM_SF_CFG,
                       "{hmac_roam_pause_user::send event[WLAN_CFGID_SET_ROAMING_MODE] failed[%d].}", ul_ret);
        oal_net_tx_wake_all_queues(pst_net_device);
        return ul_ret;
    }

    if (pst_hmac_vap->st_vap_base_info.pst_vowifi_cfg_param != OAL_PTR_NULL) {
        if (pst_hmac_vap->st_vap_base_info.pst_vowifi_cfg_param->en_vowifi_mode == VOWIFI_LOW_THRES_REPORT) {
            /* 针对漫游和去关联场景,切换vowifi语音状态 */
            hmac_config_vowifi_report((&pst_hmac_vap->st_vap_base_info), 0, OAL_PTR_NULL);
        }
    }
    hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_ROAMING);

    oam_warning_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_pause_user::queues stopped!}");
    return OAL_SUCC;
}

OAL_STATIC void hamc_roam_mode_h2d_syn(hmac_vap_stru *pst_hmac_vap,
    hmac_roam_info_stru *pst_roam_info, oal_bool_enum_uint8 need_post_stop_event)
{
    uint8_t uc_roaming_mode;
    uint32_t ul_ret;

    if (need_post_stop_event ==  OAL_TRUE) {
        uc_roaming_mode = 0;
        ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info, WLAN_CFGID_SET_ROAMING_MODE,
            OAL_SIZEOF(uint8_t), (uint8_t *)&uc_roaming_mode);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{hamc_roam_mode_h2d_syn::send event[WLAN_CFGID_SET_ROAMING_MODE] failed[%d].}", ul_ret);
        }
    }

}


uint32_t hmac_roam_resume_user(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;
    uint8_t uc_vap_id;

    if (pst_hmac_vap == OAL_PTR_NULL || p_param == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_resume_user::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_resume_user::roam info null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_net_device = pst_hmac_vap->pst_net_device;
    if (pst_net_device == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_resume_user::net_device null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 必须保证vap的状态是roaming */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_ROAMING) {
        hmac_roam_resume_pm(pst_roam_info, OAL_PTR_NULL);
        hmac_roam_resume_security_port(pst_roam_info, OAL_PTR_NULL);
        oal_net_tx_wake_all_queues(pst_net_device);
        oam_warning_log1(uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_resume_user::vap state[%d] NOT ROAMING!}",
                         pst_hmac_vap->st_vap_base_info.en_vap_state);
        return OAL_SUCC;
    }

    hamc_roam_mode_h2d_syn(pst_hmac_vap, pst_roam_info, *(uint8_t *)p_param);

    hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_UP);

    hmac_roam_resume_pm(pst_roam_info, OAL_PTR_NULL);

    hmac_roam_resume_security_port(pst_roam_info, OAL_PTR_NULL);

    oal_net_tx_wake_all_queues(pst_net_device);

    oam_warning_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_resume_user::all_queues awake!}");

    return OAL_SUCC;
}


uint32_t hmac_roam_scan_complete(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    uint8_t uc_vap_id;
    hmac_bss_mgmt_stru *pst_scan_bss_mgmt = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_scan_complete::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 漫游开关没有开时，不处理扫描结果 */
    if (pst_roam_info->uc_enable == 0) {
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    /* 获取hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_scan_complete::device null!}");
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    pst_scan_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    oam_info_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_scan_complete::handling scan result!}");
    return hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_SCAN_RESULT, (void *)pst_scan_bss_mgmt);
}


void hmac_roam_tbtt_handle(hmac_vap_stru *pst_hmac_vap)
{
}

OAL_STATIC void hmac_roam_set_scan_channel(hmac_vap_stru *pst_hmac_vap, hmac_roam_info_stru *pst_roam_info)
{
    mac_vap_stru *mac_vap = &pst_hmac_vap->st_vap_base_info;

    if (!mac_is_dual_sta_mode() || mac_is_primary_legacy_vap(mac_vap)) {
        mac_vap_stru *other_vap = NULL;
        pst_roam_info->st_config.uc_scan_orthogonal = ROAM_SCAN_CHANNEL_ORG_FULL;

        /* check p2p up state */
        other_vap = mac_vap_find_another_up_vap_by_mac_vap(mac_vap);
        if (hmac_p2p_get_scenes(other_vap) == MAC_P2P_SCENES_LOW_LATENCY) {
            pst_roam_info->st_config.uc_scan_orthogonal = ROAM_SCAN_CHANNEL_ORG_DBDC;
        }
    } else {
        /* WLAN1只需要扫描当前频段 */
        pst_roam_info->st_config.uc_scan_orthogonal =
            mac_vap->st_channel.en_band == WLAN_BAND_2G ? ROAM_SCAN_CHANNEL_ORG_2G : ROAM_SCAN_CHANNEL_ORG_5G;
    }
}


uint32_t hmac_roam_trigger_handle(hmac_vap_stru *pst_hmac_vap, int8_t c_rssi,
                                        oal_bool_enum_uint8 en_current_bss_ignore)
{
    uint32_t ul_ret;
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_trigger_handle::pst_hmac_vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 黑名单路由器，不支持漫游，防止漫游出现异常 */
    if (pst_hmac_vap->en_roam_prohibit_on == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_trigger_handle::blacklist ap not support roam!}");
        return OAL_FAIL;
    }

    ul_ret = hmac_roam_check_signal_bridge(pst_hmac_vap);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_trigger_handle::pst_hmac_vap->pul_roam_info is NULL,Return!}");
        return OAL_ERR_CODE_ROAM_EVENT_UXEXPECT;
    }

    /* 每次漫游前，刷新是否支持漫游到自己的参数 */
    pst_roam_info->en_current_bss_ignore = en_current_bss_ignore;
    pst_roam_info->en_roam_trigger = ROAM_TRIGGER_DMAC;
#ifdef _PRE_WLAN_FEATURE_11R
    pst_roam_info->st_connect.uc_ft_force_air = OAL_TRUE;
    pst_roam_info->st_connect.uc_ft_failed = OAL_FALSE;
#endif
    hmac_roam_set_scan_channel(pst_hmac_vap, pst_roam_info);

    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_INIT);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_trigger_handle::check_state fail[%d]!}", ul_ret);
        return ul_ret;
    }

    hmac_roam_alg_init(pst_roam_info, c_rssi);

    ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, OAL_PTR_NULL);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_trigger_handle::START fail[%d].}", ul_ret);
        return ul_ret;
    }

    hmac_chr_roam_info_report(pst_roam_info, HMAC_CHR_ROAM_START);
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11K

OAL_STATIC uint32_t hmac_roam_send_neighbor_req(hmac_roam_info_stru *pst_roam_info, uint8_t  *p_cur_ssid)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_ssid_param_stru *pst_ssid = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_send_neighbor_req::pst_hmac_vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }
    pst_net_dev = pst_hmac_vap->pst_net_device;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_NEIGHBOR_REQ, OAL_SIZEOF(mac_cfg_ssid_param_stru));
    pst_ssid = (mac_cfg_ssid_param_stru *)st_write_msg.auc_value;
    if (EOK != memcpy_s(pst_ssid->ac_ssid, WLAN_SSID_MAX_LEN, p_cur_ssid, WLAN_SSID_MAX_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_roam_send_neighbor_req::memcpy ssid fail!}");
    }
    pst_ssid->ac_ssid[WLAN_SSID_MAX_LEN - 1] = '\0';
    pst_ssid->uc_ssid_len = OAL_STRLEN(pst_ssid->ac_ssid);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_roam_send_neighbor_req::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    pst_roam_info->st_timer.ul_timeout = ROAM_NEIGHBOR_PROCESS_TIME_MAX;

    /* 启动扫描超时定时器 */
    hmac_roam_main_start_timer(pst_roam_info, ROAM_NEIGHBOR_PROCESS_TIME_MAX);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wait_neighbor_response_timeout(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (pst_roam_info == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_RRM, "{hmac_wait_neighbor_rsp_timeout::pst_roam_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 更改状态 */
    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_SCANING);

    /* 设置扫描参数 */
    pst_scan_params = &pst_roam_info->st_scan_h2d_params.st_scan_params;

    if (pst_scan_params->uc_channel_nums != 0) {
        pst_scan_params->uc_scan_channel_interval = 2;
        pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE;
        pst_scan_params->uc_max_scan_count_per_channel = 2;
    } else {
        pst_roam_info->st_static.uc_scan_mode = HMAC_CHR_NORMAL_SCAN;
        pst_scan_params->uc_neighbor_report_process_flag = OAL_FALSE;
        pst_scan_params->uc_max_scan_count_per_channel = 2;
        memset_s(pst_scan_params->ast_mac_ssid_set[1].auc_ssid, WLAN_SSID_MAX_LEN, 0, WLAN_SSID_MAX_LEN);
        ul_ret = hmac_roam_alg_scan_channel_init(pst_roam_info, pst_scan_params);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                           "{hmac_wait_neighbor_rsp_timeout::hmac_roam_alg_scan_channel_init fail[%d]}", ul_ret);
            return ul_ret;
        }
    }

    ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, (void *)pst_roam_info);
    if (ul_ret != OAL_SUCC) {
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_FAIL);
        oam_warning_log1(pst_roam_info->pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_wait_neighbor_rsp_timeout::START fail[%d].}", ul_ret);
        return ul_ret;
    }

    /* only use last neighbor list once */
    pst_scan_params->uc_channel_nums = 0;
    return OAL_SUCC;
}
#endif


uint32_t hmac_sta_roam_rx_mgmt(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    dmac_wlan_crx_event_stru *p_crx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *p_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *p_rx_info = OAL_PTR_NULL;
    uint8_t *mac_hdr = OAL_PTR_NULL;
    uint8_t frameSubType;
    uint8_t frameType;
    oal_bool_enum_uint8 isProtected;

    p_crx_event = (dmac_wlan_crx_event_stru *)p_param;
    p_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(p_crx_event->pst_netbuf);
    p_rx_info = (mac_rx_ctl_stru *)(&(p_rx_ctrl->st_rx_info));
    mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(p_rx_info);

    frameType = mac_get_frame_type(mac_hdr);
    frameSubType = mac_get_frame_sub_type(mac_hdr);
    isProtected = mac_is_protectedframe(mac_hdr);

    if (frameType == WLAN_FC0_TYPE_MGT && frameSubType == WLAN_FC0_SUBTYPE_ACTION) {
        hmac_mgmt_sta_roam_rx_action(pst_hmac_vap, p_crx_event->pst_netbuf, isProtected);
    }

    hmac_roam_connect_rx_mgmt(pst_hmac_vap, p_crx_event);
    return OAL_SUCC;
}

void hmac_roam_add_key_done(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_connect_key_done(pst_hmac_vap);
}


void hmac_roam_wpas_connect_state_notify(hmac_vap_stru *pst_hmac_vap, wpas_connect_state_enum_uint32 conn_state)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t notify_data;
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_wpas_connect_state_notify::vap null!}");
        return;
    }

    if (!IS_LEGACY_VAP(&pst_hmac_vap->st_vap_base_info)) {
        return;
    }

    if (pst_hmac_vap->st_vap_base_info.en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        return;
    }

    if (pst_roam_info->ul_connected_state == conn_state) {
        return;
    }

    oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                     "{hmac_roam_wpas_connect_state_notify:: state changed: [%d]-> [%d]}",
                     pst_roam_info->ul_connected_state, conn_state);

    pst_roam_info->ul_connected_state = conn_state;
    notify_data = pst_hmac_vap->ipaddr_obtained;
    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info, WLAN_CFGID_ROAM_NOTIFY_STATE,
        OAL_SIZEOF(uint32_t), (uint8_t *)&notify_data);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                       "{hmac_roam_wpas_connect_state_notify::send event failed[%d].}", ul_ret);
        return;
    }
}

#ifdef _PRE_WLAN_FEATURE_11R

uint32_t hmac_roam_reassoc(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru *pst_roam_info;
    uint32_t ul_ret;
    uint8_t uc_vap_id;
    mac_bss_dscr_stru *pst_bss_dscr;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_reassoc::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_warning_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_reassoc::roam info null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 漫游开关没有开时，不处理 */
    if (pst_roam_info->uc_enable == 0) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM, "{hmac_roam_reassoc::roam disable}");
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    /* 主状态机为非CONNECTING状态，失败 */
    if (pst_roam_info->en_main_state != ROAM_MAIN_STATE_CONNECTING) {
        oam_warning_log1(0, OAM_SF_ROAM, "{hmac_roam_reassoc::main_state=[%d] not CONNECTING}",
                         pst_roam_info->en_main_state);
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    /* CONNECT状态机为非WAIT_JOIN状态，失败 */
    if (pst_roam_info->st_connect.en_state != ROAM_CONNECT_STATE_WAIT_ASSOC_COMP) {
        if (pst_roam_info->st_connect.en_state == ROAM_CONNECT_STATE_WAIT_PREAUTH_COMP) {
            ul_ret =  hmac_roam_preauth(pst_hmac_vap);
            return ul_ret;
        }
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_reassoc::connect state[%d] error.}", pst_roam_info->st_connect.en_state);
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    if (OAL_TRUE == (mac_mib_get_ft_trainsistion(&pst_hmac_vap->st_vap_base_info))) {
        if (mac_mib_get_ft_over_ds(&pst_hmac_vap->st_vap_base_info) &&
            (pst_hmac_vap->bit_11r_over_ds == OAL_TRUE)) { /* over ds 首先pause user */
            pst_bss_dscr = pst_roam_info->st_connect.pst_bss_dscr;
            if (pst_bss_dscr == OAL_PTR_NULL) {
                oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                               "{hmac_roam_reassoc::pst_bss_dscr is null.}");
                return OAL_ERR_CODE_ROAM_NO_VALID_BSS;
            }

            pst_roam_info->st_static.ul_connect_start_timetamp = (uint32_t)oal_time_get_stamp_ms();

            ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_ROAMING_START, (void *)pst_bss_dscr);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ROAM,
                                 "{hmac_roam_reassoc::hmac_fsm_call_func_sta fail[%ld]!}", ul_ret);
                return ul_ret;
            }
        }
        oam_warning_log1(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_reassoc::ft_over_ds=[%d],to send reassoc!}",
                         mac_mib_get_ft_over_ds(&pst_hmac_vap->st_vap_base_info));
        ul_ret = hmac_roam_connect_ft_reassoc(pst_hmac_vap);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_reassoc::reassoc FAIL[%d]!}", ul_ret);
            return ul_ret;
        }
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_preauth(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_info_stru              *pst_roam_info = OAL_PTR_NULL;
    uint32_t                        ul_ret;
    uint8_t                         uc_vap_id;

    if (pst_hmac_vap == OAL_PTR_NULL)
    {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_preauth::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;
    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL)
    {
        oam_error_log0(uc_vap_id, OAM_SF_ROAM, "{hmac_roam_preauth::roam info null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    /* 漫游开关没有开时，不处理 */
    if (0 == pst_roam_info->uc_enable) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_preauth::roam disabled.}");
        return OAL_ERR_CODE_ROAM_DISABLED;
    }

    /* 主状态机为非CONNECTING状态，失败 */
    if (pst_roam_info->en_main_state != ROAM_MAIN_STATE_CONNECTING) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_preauth::roam_en_main_state=[%d] not ROAM_MAIN_STATE_CONNECTING, return.}",
            pst_roam_info->en_main_state);
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    /* CONNECT状态机为非WAIT_JOIN状态，失败 */
    if (pst_roam_info->st_connect.en_state != ROAM_CONNECT_STATE_WAIT_PREAUTH_COMP) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_preauth::connect state[%d] error.}", pst_roam_info->st_connect.en_state);
        return OAL_ERR_CODE_ROAM_STATE_UNEXPECT;
    }

    if (pst_hmac_vap->bit_11r_private_preauth != OAL_TRUE) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_preauth::not support preauth.}");
        return OAL_FAIL;
    }

    if (mac_mib_get_ft_trainsistion(&pst_hmac_vap->st_vap_base_info) == OAL_TRUE) {
        oam_warning_log1(uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_preauth::ft_over_ds=[%d],to send preauth!}",
            mac_mib_get_ft_over_ds(&pst_hmac_vap->st_vap_base_info));
        ul_ret = hmac_roam_connect_ft_preauth(pst_hmac_vap);
        if (OAL_SUCC != ul_ret) {
            oam_warning_log1(uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_preauth::hmac_roam_connect_ft_preauth FAIL[%d]!}", ul_ret);
            return ul_ret;
        }
    }

    return OAL_SUCC;
}


uint32_t hmac_roam_rx_ft_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_wlan_crx_event_stru st_crx_event;

    memset_s(&st_crx_event, OAL_SIZEOF(dmac_wlan_crx_event_stru), 0, OAL_SIZEOF(dmac_wlan_crx_event_stru));
    st_crx_event.pst_netbuf = pst_netbuf;

    hmac_roam_connect_rx_mgmt(pst_hmac_vap, &st_crx_event);
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_11R

oal_bool_enum_uint8 hmac_roam_check_dbac_channel(mac_vap_stru *p2p_vap, uint8_t chanNum)
{
    mac_p2p_scenes_enum_uint8 p2p_scene = hmac_p2p_get_scenes(p2p_vap);

    /* check dbac channel compared with p2p up channel */
    if (p2p_vap != OAL_PTR_NULL && p2p_scene == MAC_P2P_SCENES_LOW_LATENCY) {
        if (chanNum != p2p_vap->st_channel.uc_chan_number &&
            mac_get_band_by_channel_num(chanNum) == p2p_vap->st_channel.en_band) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

#ifdef _PRE_WLAN_FEATURE_11K
oal_bool_enum_uint8 hmac_neighbor_report_check_validity(hmac_vap_stru *hmac_vap,
    mac_neighbor_rpt_ie_stru *neighbor_rpt_ie, mac_vap_stru *p2p_vap)
{
    if (hmac_vap == NULL || neighbor_rpt_ie == NULL) {
        return OAL_FALSE;
    }

    if (neighbor_rpt_ie->uc_len < MAC_NEIGHBOR_REPORT_IE_LEN) {
        oam_error_log1(0, OAM_SF_ROAM,
            "{hmac_neighbor_report_check_validity:: invalid ie len[%d]}", neighbor_rpt_ie->uc_len);
        return OAL_FALSE;
    }

    /* check dbac channel compared with p2p up channel */
    if (hmac_roam_check_dbac_channel(p2p_vap, neighbor_rpt_ie->uc_channum) == OAL_TRUE) {
        oam_warning_log3(0, OAM_SF_ROAM,
            "{hmac_neighbor_report_check_validity::dbac channel ignored, %0x:%0x chanNum=%d}",
            neighbor_rpt_ie->auc_bssid[4], neighbor_rpt_ie->auc_bssid[5], /* mac addr 4 & 5 */
            neighbor_rpt_ie->uc_channum);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

void hmac_roam_neighbor_report_add_chan_and_bssid(hmac_roam_info_stru *roam_info,
    mac_neighbor_rpt_ie_stru *neighbor_rpt_ie)
{
    wlan_channel_band_enum_uint8 en_band;
    uint8_t channel_idx = 0;
    uint8_t channl_num;
    mac_scan_req_stru *scan_params = OAL_PTR_NULL;

    if (roam_info == OAL_PTR_NULL || neighbor_rpt_ie == OAL_PTR_NULL) {
        return;
    }

    scan_params = &roam_info->st_scan_h2d_params.st_scan_params;

    channl_num = neighbor_rpt_ie->uc_channum;
    en_band = mac_get_band_by_channel_num(channl_num);
    if (mac_get_channel_idx_from_num(en_band, channl_num, &channel_idx) == OAL_SUCC) {
        hmac_roam_neighbor_report_add_chan(scan_params, channl_num, en_band, channel_idx);
    }
    /* 将解析出来的 auc_bssid 存储到 pst_roam_info->auc_neighbor_rpt_bssid[] 中 */
    /* 同时更新要扫描的邻居AP数量(pst_roam_info->uc_neighbor_rpt_bssid_num) 的值 */
    hmac_roam_neighbor_report_add_bssid(roam_info, neighbor_rpt_ie->auc_bssid);
}


OAL_STATIC uint32_t hmac_roam_parse_neighbor_response(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    oal_netbuf_stru *pst_netbuf = (oal_netbuf_stru *)p_param;
    int32_t l_framebody_len;
    dmac_rx_ctl_stru *pst_rx_ctrl;
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint8_t *puc_neighbor_rpt_ie = OAL_PTR_NULL;
    mac_neighbor_rpt_ie_stru *pst_neighbor_rpt_ie = OAL_PTR_NULL;
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    hmac_vap_stru *hmac_vap;
    mac_vap_stru *other_vap;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if (pst_rx_ctrl == OAL_PTR_NULL || pst_roam_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = pst_roam_info->pst_hmac_vap;
    if (hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    other_vap = mac_vap_find_another_up_vap_by_mac_vap(&(hmac_vap->st_vap_base_info));

    l_framebody_len = (int32_t)MAC_GET_RX_CB_PAYLOAD_LEN(&(pst_rx_ctrl->st_rx_info));

    /*************************************************************************/
    /*                   Neighbor Report Response Frame - Frame Body         */
    /* --------------------------------------------------------------------- */
    /* |Category |Action |Dialog Token| Neighbor Report Elements            |*/
    /* --------------------------------------------------------------------- */
    /* |1        |1      | 1          | var                                  */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    pst_roam_info->uc_neighbor_rpt_bssid_num = 0;
    pst_scan_params = &pst_roam_info->st_scan_h2d_params.st_scan_params;
    pst_scan_params->uc_channel_nums = 0;
    pst_scan_params->uc_neighbor_report_process_flag = OAL_TRUE;
    pst_scan_params->uc_max_scan_count_per_channel = 1;
    memcpy_s(pst_scan_params->ast_mac_ssid_set[1].auc_ssid, WLAN_SSID_MAX_LEN,
        pst_scan_params->ast_mac_ssid_set[0].auc_ssid, WLAN_SSID_MAX_LEN);

    if (l_framebody_len <= MAC_NEIGHBOR_REPORT_ACTION_RSP_FIX_LEN) {
        return OAL_SUCC;
    }

    l_framebody_len -= MAC_NEIGHBOR_REPORT_ACTION_RSP_FIX_LEN;
    while (l_framebody_len >= MAC_NEIGHBOR_RPT_MIN_LEN + MAC_IE_HDR_LEN) {
        if (puc_ie == OAL_PTR_NULL) {
            puc_ie =((mac_action_neighbor_rsp_stru *)mac_netbuf_get_payload(pst_netbuf))->neighbor_rpt_ies;
        }
        puc_neighbor_rpt_ie = mac_find_ie(MAC_EID_NEIGHBOR_REPORT, puc_ie, l_framebody_len);
        if (puc_neighbor_rpt_ie == OAL_PTR_NULL) {
            break;
        }
        pst_neighbor_rpt_ie = (mac_neighbor_rpt_ie_stru *)puc_neighbor_rpt_ie;
        if (hmac_neighbor_report_check_validity(hmac_vap, pst_neighbor_rpt_ie, other_vap) == OAL_TRUE) {
            hmac_roam_neighbor_report_add_chan_and_bssid(pst_roam_info, pst_neighbor_rpt_ie);
        }

        l_framebody_len -= pst_neighbor_rpt_ie->uc_len + MAC_IE_HDR_LEN;
        puc_ie += pst_neighbor_rpt_ie->uc_len + MAC_IE_HDR_LEN;
    }
    oam_warning_log2(0, OAM_SF_RRM, "hmac_roam_parse_neighbor_response::chan num[%d] bssid num[%d]",
                     pst_scan_params->uc_channel_nums, pst_roam_info->uc_neighbor_rpt_bssid_num);

    return OAL_SUCC;
}


uint32_t hmac_roam_rx_neighbor_response_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_rx_neighbor_rsp_action::vap null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_rx_neighbor_rsp_action::roam info null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* connecting state: hmac_roam_parse_neighbor_response 只解析Action Frame
        * neighbor_process: hmac_roam_neighbor_response_process: 解析Action Frame，并且切换Roaming FSM到扫描状态
        */
    ul_ret = hmac_roam_main_check_state(pst_roam_info, MAC_VAP_STATE_UP, ROAM_MAIN_STATE_NEIGHBOR_PROCESS);
    if (ul_ret != OAL_SUCC) {
        hmac_roam_parse_neighbor_response(pst_roam_info, (void *)pst_netbuf);
        return ul_ret;
    }

    ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, (void *)pst_netbuf);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                         "{hmac_roam_rx_neighbor_rsp_action::MGMT_RX FAIL[%d]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_roam_neighbor_response_process(hmac_roam_info_stru *pst_roam_info, void *p_param)
{
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (pst_roam_info == OAL_PTR_NULL || p_param == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RRM, "{hmac_roam_neighbor_response_process::p_param or pst_roam_info is NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_roam_parse_neighbor_response(pst_roam_info, p_param);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_RRM,
            "{hmac_roam_neighbor_response_process::hmac_roam_parse_neighbor_response ul_ret=%d}", ul_ret);
        return ul_ret;
    }

    pst_scan_params = &pst_roam_info->st_scan_h2d_params.st_scan_params;

    if (pst_scan_params->uc_channel_nums == 0) {
        pst_scan_params->uc_neighbor_report_process_flag = OAL_FALSE;
        pst_scan_params->uc_max_scan_count_per_channel = 2;
        memset_s(pst_scan_params->ast_mac_ssid_set[1].auc_ssid, WLAN_SSID_MAX_LEN, 0, WLAN_SSID_MAX_LEN);
        ul_ret = hmac_roam_alg_scan_channel_init(pst_roam_info, pst_scan_params);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_ROAM, "{hmac_roam_neighbor_response_process::channel init fail[%d]}", ul_ret);
            return ul_ret;
        }
    } else {
        pst_scan_params->en_need_switch_back_home_channel = OAL_TRUE;
        pst_scan_params->uc_scan_channel_interval = MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE;
        pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE;
    }

    pst_roam_info->st_static.uc_scan_mode =
        (pst_scan_params->uc_channel_nums == 0) ? HMAC_CHR_NORMAL_SCAN : HMAC_CHR_11K_SCAN;

    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_SCANING);
    ul_ret = hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_START, (void *)pst_roam_info);
    if (ul_ret != OAL_SUCC) {
        hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_FAIL);
        return ul_ret;
    }

    return OAL_SUCC;
}
#endif


void hmac_roam_connect_complete(hmac_vap_stru *pst_hmac_vap, uint32_t ul_result)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_connect_complete::vap null!}");
        return;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_connect_complete::pst_roam_info null!}");
        return;
    }

    /* 漫游开关没有开时，不处理扫描结果 */
    if (pst_roam_info->uc_enable == 0) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                       "{hmac_roam_connect_complete::roam disabled!}");
        return;
    }

    pst_roam_info->st_static.ul_connect_end_timetamp = (uint32_t)oal_time_get_stamp_ms();

    if (pst_roam_info->uc_roaming_scenario == ROAMING_SCENARIO_HOME) {
        pst_roam_info->st_alg.st_home_network.ul_5g_scan_timestamp = (uint32_t)oal_time_get_stamp_ms();
    }

    if (pst_roam_info->en_roam_trigger == ROAM_TRIGGER_HOME_NETWORK) {
        if (ul_result == OAL_SUCC) {
            pst_roam_info->st_alg.uc_roam_fail_cnt = 0;
        } else {
            pst_roam_info->st_alg.uc_roam_fail_cnt++;
        }
    }

    if (ul_result == OAL_SUCC) {
        hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_CONNECT_SUCC, OAL_PTR_NULL);
    } else if (ul_result == OAL_ERR_CODE_ROAM_HANDSHAKE_FAIL) {
        hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_HANDSHAKE_FAIL, OAL_PTR_NULL);
    } else if (ul_result == OAL_ERR_CODE_ROAM_NO_RESPONSE) {
        hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_CONNECT_FAIL, OAL_PTR_NULL);
    } else {
        /* 上层触发停止漫游时，先删除相关定时器 */
        hmac_roam_main_del_timer(pst_roam_info);
        frw_timer_immediate_destroy_timer_m(&(pst_roam_info->st_connect.st_timer));
        hmac_roam_main_fsm_action(pst_roam_info, ROAM_MAIN_FSM_EVENT_TIMEOUT, OAL_PTR_NULL);
    }
}


uint32_t hmac_roam_connect_ft_ds_change_to_air(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    uint32_t ul_ret;
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_connect_ft_ds_change_to_air::vap null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    if (p_param == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_connect_ft_ds_change_to_air::param null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_connect_ft_ds_change_to_air::roam_info null!}");
        return OAL_ERR_CODE_ROAM_INVALID_VAP;
    }

    hmac_roam_main_clear(pst_roam_info);
    ul_ret = hmac_roam_main_del_timer(pst_roam_info);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    hmac_roam_main_change_state(pst_roam_info, ROAM_MAIN_STATE_SCANING);

    ul_ret = hmac_roam_connect_to_bss(pst_roam_info, p_param);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    return OAL_SUCC;
}

uint32_t hmac_roam_pri_sta_join_check(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *new_bss, mac_vap_stru *other_vap)
{
    hmac_roam_info_stru *roam_info = (hmac_roam_info_stru*)hmac_vap->pul_roam_info;
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    hmac_roam_old_bss_stru *old_bss = NULL;
    if (!roam_info) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_roam_dual_sta_join_check::roam info is null!}");
        return OAL_SUCC;
    }
    old_bss = &roam_info->st_old_bss;
    if (new_bss->st_channel.en_band != old_bss->st_channel.en_band) {
        // wlan0漫游到wlan1工作的band
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ASSOC,
            "{hmac_roam_dual_sta_join_check::pri sta is roaming to diff band, disconnect sec sta!}");
        return OAL_FAIL;
    }
    if (g_wlan_spec_cfg->feature_slave_ax_is_support == OAL_FALSE &&
        (new_bss->en_he_capable || new_bss->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS) &&
        (other_vap->en_protocol == WLAN_HE_MODE ||
         other_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS)) {
        // wlan1不能去辅路，wlan0漫游后也不能去辅路
        oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_roam_dual_sta_join_check::"
            "pri sta is roaming, both the new bss[HE %d, bandwidth %d] and sec sta[HE %d, bandwidth %d]"
            "cannot work on slave, disconnect sec sta!}", new_bss->en_he_capable, new_bss->st_channel.en_bandwidth,
            other_vap->en_protocol == WLAN_HE_MODE, other_vap->st_channel.en_bandwidth);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC oal_bool_enum_uint8 hmac_roam_is_vaild_report_chr_info(hmac_roam_info_stru *pst_roam_info,
    roam_fail_reason_enum_uint8 ul_result)
{
    oam_warning_log3(0, OAM_SF_ROAM, "{hmac_roam_is_vaild_report_chr_info::[%d]->[%d]->[%d]}",
                     pst_roam_info->en_roam_trigger, pst_roam_info->st_static.uc_scan_mode, ul_result);

    if (ul_result == HMAC_CHR_ROAM_START) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


OAL_STATIC void hmac_chr_report_roam_fail_info(hmac_roam_info_stru *pst_roam_info,
    roam_fail_reason_enum_uint8 ul_result)
{
    hmac_chr_roam_connect_info_stru st_chr_roam_connect_info = { {0, 0, 0, 0, 0, 0}, 0, 0, {0, 0, 0, 0, 0, 0}, 0 };
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint32_t ul_roam_dur;
    uint32_t ul_now;

    if (!hmac_roam_is_vaild_report_chr_info(pst_roam_info, ul_result)) {
        return;
    }

    ul_now = (uint32_t)oal_time_get_stamp_ms();
    ul_roam_dur = oal_time_get_runtime(pst_roam_info->st_static.ul_scan_start_timetamp, ul_now);

    st_chr_roam_connect_info.uc_roam_time = ul_roam_dur;
    st_chr_roam_connect_info.uc_roam_result = ul_result;
    st_chr_roam_connect_info.uc_roam_mode = pst_roam_info->en_roam_trigger;
    st_chr_roam_connect_info.uc_roam_stage = pst_roam_info->en_main_state;

    /* current AP bssid, rssi and channel */
    pst_hmac_vap = pst_roam_info->pst_hmac_vap;
    memcpy_s(&st_chr_roam_connect_info.auc_src_bssid, WLAN_MAC_ADDR_LEN,
             &(pst_hmac_vap->st_vap_base_info.auc_bssid), WLAN_MAC_ADDR_LEN);
    st_chr_roam_connect_info.uc_src_channel = pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number;
    st_chr_roam_connect_info.uc_src_rssi = pst_roam_info->st_alg.c_current_rssi; // maybe not right

    /* roaming target AP bssid, rssi and channel */
    pst_bss_dscr = pst_roam_info->st_connect.pst_bss_dscr;
    if (pst_bss_dscr != OAL_PTR_NULL && ul_result != HMAC_CHR_ROAM_SCAN_FAIL) {
        memcpy_s(&st_chr_roam_connect_info.auc_target_bssid, WLAN_MAC_ADDR_LEN,
                 &(pst_bss_dscr->auc_bssid), WLAN_MAC_ADDR_LEN);
        st_chr_roam_connect_info.uc_target_channel = pst_bss_dscr->st_channel.uc_chan_number;
        st_chr_roam_connect_info.uc_target_rssi = pst_bss_dscr->c_rssi;
    }

    oam_warning_log3(0, OAM_SF_ROAM, "{hmac_chr_report_roam_fail_info::1[%d]->[%d] [%d]}",
                     st_chr_roam_connect_info.uc_roam_mode, st_chr_roam_connect_info.uc_roam_result,
                     st_chr_roam_connect_info.uc_roam_time);

    chr_exception_p(CHR_WIFI_ROAM_FAIL_REPORT_EVENTID,
                    (uint8_t *)(&st_chr_roam_connect_info),
                    OAL_SIZEOF(hmac_chr_roam_connect_info_stru));
}


void hmac_chr_roam_info_report(hmac_roam_info_stru *pst_roam_info, roam_fail_reason_enum_uint8 ul_result)
{
    hmac_chr_roam_info_stru st_chr_roam_info = { 0 };
    uint32_t ul_scan_dur;
    uint32_t ul_connect_dur;

    if (pst_roam_info == OAL_PTR_NULL || pst_roam_info->pst_hmac_vap == OAL_PTR_NULL) {
        return;
    }

    /* when connect fail, not double report timeout event */
    if (pst_roam_info->st_connect.uc_roam_main_fsm_state == ROAM_MAIN_FSM_EVENT_CONNECT_FAIL
        && ul_result == HMAC_CHR_ROAM_TIMEOUT_FAIL)
    {
        return;
    }

    /* chr report roam connect info */
    hmac_chr_report_roam_fail_info(pst_roam_info, ul_result);

    ul_scan_dur = oal_time_get_runtime(pst_roam_info->st_static.ul_scan_start_timetamp,
                                       pst_roam_info->st_static.ul_scan_end_timetamp);
    ul_connect_dur = oal_time_get_runtime(pst_roam_info->st_static.ul_connect_start_timetamp,
                                          pst_roam_info->st_static.ul_connect_end_timetamp);
    if (ul_result == HMAC_CHR_ROAM_SUCCESS) {
        st_chr_roam_info.uc_scan_time = ul_scan_dur;
        st_chr_roam_info.uc_connect_time = ul_connect_dur;
    } else {
        st_chr_roam_info.uc_scan_time = 0;
        st_chr_roam_info.uc_connect_time = 0;
    }
    st_chr_roam_info.uc_trigger = pst_roam_info->en_roam_trigger;
    st_chr_roam_info.uc_roam_result = ul_result;
    st_chr_roam_info.uc_roam_mode = pst_roam_info->st_static.uc_roam_mode;
    st_chr_roam_info.uc_scan_mode = pst_roam_info->st_static.uc_scan_mode;

    oam_warning_log2(0, OAM_SF_ROAM, "{hmac_chr_roam_info_report::1[%d]->[%d]}",
                     st_chr_roam_info.uc_trigger, st_chr_roam_info.uc_roam_result);
    oam_warning_log2(0, OAM_SF_ROAM, "{hmac_chr_roam_info_report::2[%d]->[%d]}",
                     st_chr_roam_info.uc_scan_time, st_chr_roam_info.uc_connect_time);

    chr_exception_p(CHR_WIFI_ROAM_INFO_REPORT_EVENTID,
                    (uint8_t *)(&st_chr_roam_info),
                    OAL_SIZEOF(hmac_chr_roam_info_stru));
    return;
}


void hmac_roam_timeout_test(hmac_vap_stru *pst_hmac_vap)
{
    hmac_roam_connect_timeout(pst_hmac_vap->pul_roam_info);
}

