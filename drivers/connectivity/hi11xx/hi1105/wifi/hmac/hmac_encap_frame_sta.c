

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "wlan_mib.h"
#include "mac_vap.h"
#include "mac_frame.h"
#include "hmac_encap_frame_sta.h"
#include "hmac_user.h"
#include "hmac_main.h"
#include "hmac_tx_data.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_mgmt_sta.h"
#include "hmac_device.h"
#include "hmac_resource.h"
#include "hmac_scan.h"
#include "mac_user.h"
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif
#ifdef _PRE_WLAN_FEATURE_MBO
#include "hmac_mbo.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ENCAP_FRAME_STA_C

/* 2 全局变量定义 */
/* 3 函数实现 */

OAL_STATIC int32_t encap_cowork_ie_adaptive_11r(hmac_vap_stru *pst_hmac_vap, oal_sta_ap_cowork_ie *pst_cowork_ie)
{
    if (pst_hmac_vap == OAL_PTR_NULL || pst_cowork_ie == OAL_PTR_NULL) {
        return 0;
    }
#ifdef _PRE_WLAN_FEATURE_11R
    if (pst_hmac_vap->bit_adaptive11r == OAL_TRUE) {
        pst_cowork_ie->support_adaptive_11r = 1;
        pst_cowork_ie->support_5G = 1;
        return 1;
    }
#endif
    return 0;
}


OAL_STATIC int32_t encap_cowork_ie_narrow_band(hmac_vap_stru *pst_hmac_vap, oal_sta_ap_cowork_ie *pst_cowork_ie)
{
    if (pst_hmac_vap == OAL_PTR_NULL || pst_cowork_ie == OAL_PTR_NULL) {
        return 0;
    }
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (pst_hmac_vap->bit_adaptive11r == OAL_TRUE) {
            pst_cowork_ie->support_nallow_band = 1;
            return 1;
        }
    }
#endif
    return 0;
}


void mac_add_cowork_ie(void *pst_hmac_sta, uint8_t *puc_buffer, uint16_t *pus_ie_len)
{
    mac_bss_dscr_stru *curr_bss_dscr = OAL_PTR_NULL;
    uint8_t *puc_cowork_ie = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t *puc_frame_body = OAL_PTR_NULL;
    int32_t us_frame_body_len;
    int32_t us_cowork_bit_count = 0;
    uint16_t us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    uint32_t ul_ie_oui = MAC_WLAN_OUI_HUAWEI;
    oal_sta_ap_cowork_ie cowork_ie = { MAC_EID_VENDOR, 19,
        { (ul_ie_oui >> 16) & 0xFF, (ul_ie_oui >> 8) & 0xFF, ul_ie_oui & 0xFF },
        MAC_WLAN_OUI_TYPE_HAUWEI_COWORK, TYPE_TLV_CAPABILITY, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        TYPE_TLV_DC_ROAM_INFO, 9, { 0, 0, 0, 0, 0, 0 }, 0, 0, 0
    };

    *pus_ie_len = 0;
    if (pst_hmac_sta == OAL_PTR_NULL) {
        return;
    }

    pst_hmac_vap = (hmac_vap_stru *)pst_hmac_sta;
    if (pst_hmac_vap->st_vap_base_info.en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }

    curr_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&pst_hmac_vap->st_vap_base_info,
                                                                            pst_hmac_vap->st_vap_base_info.auc_bssid);

    if (curr_bss_dscr == OAL_PTR_NULL) {
        return;
    }

    puc_frame_body = curr_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN;
    us_frame_body_len = curr_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN;
    puc_cowork_ie = mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI, MAC_WLAN_OUI_TYPE_HAUWEI_COWORK,
                                           puc_frame_body + us_offset, us_frame_body_len - us_offset);

    /* 如果接收的beacon帧不存在端管协同IE，则返回 */
    if (puc_cowork_ie == OAL_PTR_NULL) {
        return;
    }

    us_cowork_bit_count += encap_cowork_ie_adaptive_11r(pst_hmac_vap, &cowork_ie);
    us_cowork_bit_count += encap_cowork_ie_narrow_band(pst_hmac_vap, &cowork_ie);
    if (us_cowork_bit_count == 0) {
        return;
    }

    if (EOK != memcpy_s(puc_buffer, sizeof(oal_sta_ap_cowork_ie), &cowork_ie, sizeof(oal_sta_ap_cowork_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "mac_add_cowork_ie::memcpy fail!");
        return;
    }
    *pus_ie_len = sizeof(oal_sta_ap_cowork_ie);

    return;
}


hmac_scanned_bss_info *hmac_vap_get_scan_bss_info(mac_vap_stru *pst_mac_vap)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scaned_bss = OAL_PTR_NULL;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_QOS, "{mac_vap_get_scan_bss_info::pst_mac_device null.}");
        return OAL_PTR_NULL;
    }
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);
    pst_scaned_bss = hmac_scan_find_scanned_bss_by_bssid(pst_bss_mgmt, pst_mac_vap->auc_bssid);
    if (pst_scaned_bss == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{mac_tx_qos_enhance_list_init::do not have scan result!!!}");
        return OAL_PTR_NULL;
    }
    return pst_scaned_bss;
}


OAL_STATIC oal_bool_enum hmac_sta_check_need_set_ext_cap_ie(mac_vap_stru *pst_mac_vap)
{
    uint8_t *puc_ext_cap_ie;
    uint16_t us_ext_cap_index;

    puc_ext_cap_ie = hmac_sta_find_ie_in_probe_rsp(pst_mac_vap, MAC_EID_EXT_CAPS, &us_ext_cap_index);
    if (puc_ext_cap_ie == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


void hmac_set_supported_rates_ie_asoc_req(hmac_vap_stru *pst_hmac_sta,
                                                  uint8_t *puc_buffer,
                                                  uint8_t *puc_ie_len)
{
    uint8_t uc_nrates;
    uint8_t uc_idx;

    /**************************************************************************
                        ---------------------------------------
                        |Element ID | Length | Supported Rates|
                        ---------------------------------------
             Octets:    |1          | 1      | 1~8            |
                        ---------------------------------------
    The Information field is encoded as 1 to 8 octets, where each octet describes a single Supported
    Rate or BSS membership selector.
    **************************************************************************/
    puc_buffer[0] = MAC_EID_RATES;

    uc_nrates = mac_mib_get_SupportRateSetNums(&pst_hmac_sta->st_vap_base_info);

    if (uc_nrates > MAC_MAX_SUPRATES) {
        uc_nrates = MAC_MAX_SUPRATES;
    }

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_hmac_sta->auc_supp_rates[uc_idx];
    }

    puc_buffer[1] = uc_nrates;

    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}

void hmac_set_exsup_rates_ie_asoc_req(hmac_vap_stru *pst_hmac_sta, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t uc_nrates;
    uint8_t uc_idx;

    /***************************************************************************
                   -----------------------------------------------
                   |ElementID | Length | Extended Supported Rates|
                   -----------------------------------------------
       Octets:     |1         | 1      | 1-255                   |
                   -----------------------------------------------
    ***************************************************************************/
    if (mac_mib_get_SupportRateSetNums(&pst_hmac_sta->st_vap_base_info) <= MAC_MAX_SUPRATES) {
        *puc_ie_len = 0;

        return;
    }

    puc_buffer[0] = MAC_EID_XRATES;
    uc_nrates = mac_mib_get_SupportRateSetNums(&pst_hmac_sta->st_vap_base_info) - MAC_MAX_SUPRATES;
    puc_buffer[1] = uc_nrates;

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_hmac_sta->auc_supp_rates[uc_idx + MAC_MAX_SUPRATES];
    }

    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}
#ifdef _PRE_WLAN_FEATURE_M2S

void hmac_assoc_set_siso_mode(mac_vap_stru *pst_mac_vap, uint8_t *puc_req_frame)
{
    hmac_user_stru *pst_hmac_user;
    mac_frame_ht_cap_stru *pst_ht_capinfo;

    /* 如果是满足只支持siso入网的黑名单条件，则以SISO方式入网 */
    pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        return;
    }
    if ((pst_hmac_user->en_user_ap_type & MAC_AP_TYPE_MIMO_BLACKLIST)
        && (0 == oal_compare_mac_addr(pst_hmac_user->auc_mimo_blacklist_mac, pst_mac_vap->auc_bssid))) {
        puc_req_frame += MAC_IE_HDR_LEN;

        pst_ht_capinfo = (mac_frame_ht_cap_stru *)puc_req_frame;
        pst_ht_capinfo->bit_sm_power_save = MAC_SMPS_STATIC_MODE;
        pst_ht_capinfo->bit_tx_stbc = OAL_FALSE;

        puc_req_frame -= MAC_IE_HDR_LEN;
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_assoc_set_siso_mode::set smps to STATIC.}");
    }
}

void hmac_update_user_sounding_dim_num(mac_vap_stru *pst_mac_vap, hmac_scanned_bss_info *pst_scaned_bss)
{
    uint8_t uc_num_sounding_dim;
    mac_user_stru *pst_mac_user;
#if defined(_PRE_WLAN_FEATURE_11AX)
    mac_he_hdl_stru *pst_he_hdl;
#endif

    /* VHT中的字段需要根据AP的bf能力设置，故在此提前设置MAC_USER的bf能力 */
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);

    if (pst_mac_user == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_update_user_sounding_dim_num::pst_mac_user null.}");
        return;
    }
    uc_num_sounding_dim = pst_scaned_bss->st_bss_dscr_info.uc_num_sounding_dim;

    pst_mac_user->st_vht_hdl.bit_num_sounding_dim = uc_num_sounding_dim;
#if defined(_PRE_WLAN_FEATURE_11AX)
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        pst_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
        if (pst_scaned_bss->st_bss_dscr_info.en_bw_cap <= WLAN_BW_CAP_80M) {
            pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_below_80mhz_sounding_dimensions_num = uc_num_sounding_dim;
        } else {
            pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_over_80mhz_sounding_dimensions_num = uc_num_sounding_dim;
        }
    }
#endif
}
#endif
#ifdef _PRE_WLAN_FEATURE_TXBF

void hmac_sta_roam_and_assoc_update_txbf(mac_vap_stru *pst_mac_vap, mac_bss_dscr_stru *pst_mac_bss_dscr)
{
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        /* linksys 2G黑名单设备,关闭txbf能力 */
        if (MAC_IS_LINKSYS(pst_mac_vap->auc_bssid) && pst_mac_bss_dscr->en_txbf_blacklist_chip_oui == OAL_TRUE) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_sta_roam_and_assoc_update_txbf: txbf blacklist!");
            hmac_config_vap_close_txbf_cap(pst_mac_vap);
        }
        /* 2.4G双天线情况下(除过1152芯片,通过en_punctured_preamble字段判断是否为52芯片)，关闭txbf能力 */
#ifdef _PRE_WLAN_FEATURE_M2S
        if ((pst_mac_bss_dscr->uc_num_sounding_dim <= 1) && (pst_mac_bss_dscr->en_punctured_preamble != OAL_TRUE)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_sta_roam_and_assoc_update_txbf: 2.4G antenna < 2,close txbf!");
            hmac_config_vap_close_txbf_cap(pst_mac_vap);
        }
#endif
    }
}
#endif


void hmac_set_ht_capabilities_ie_update(void *pst_mac_vap, uint8_t *puc_buffer,
    uint8_t uc_ie_len, hmac_scanned_bss_info *pst_scaned_bss)
{
    mac_frame_ht_cap_stru *pst_ht_capinfo = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_TXBF_HT
    mac_txbf_cap_stru *pst_txbf_cap;
#endif

    if (uc_ie_len == 0) {
        return;
    }

    /* 偏移到ht capabilities information位置 */
    pst_ht_capinfo = (mac_frame_ht_cap_stru *)(puc_buffer + MAC_IE_HDR_LEN);
    if (pst_scaned_bss->st_bss_dscr_info.en_ht_ldpc == OAL_FALSE) {
        pst_ht_capinfo->bit_ldpc_coding_cap = OAL_FALSE;
    }

    if (pst_scaned_bss->st_bss_dscr_info.en_ht_stbc == OAL_FALSE) {
        pst_ht_capinfo->bit_rx_stbc = OAL_FALSE;
    }

#ifdef _PRE_WLAN_FEATURE_TXBF_HT
    if ((pst_mac_vap->bit_ap_11ntxbf == OAL_TRUE)
        && (pst_mac_vap->st_cap_flag.bit_11ntxbf == OAL_TRUE)) {
        /* 偏移到txbf cap information位置 */
        pst_txbf_cap = (mac_txbf_cap_stru *)(puc_buffer + MAC_11N_TXBF_CAP_OFFSET);

        pst_txbf_cap->bit_rx_stagg_sounding = OAL_TRUE;
        pst_txbf_cap->bit_explicit_compr_bf_fdbk = 1;
        pst_txbf_cap->bit_compr_steering_num_bf_antssup = 1;
        pst_txbf_cap->bit_minimal_grouping = 3;
        pst_txbf_cap->bit_chan_estimation = 1;
    }
#endif


}

#ifdef _PRE_WLAN_FEATURE_11AX
void hmac_mgmt_proc_he_uora(mac_vap_stru *mac_vap, hmac_scanned_bss_info *scaned_bss)
{
    if ((g_wlan_spec_cfg->feature_11ax_uora_is_supported == OAL_TRUE) && IS_LEGACY_STA(mac_vap)) {
        if (scaned_bss->st_bss_dscr_info.en_he_uora == OAL_TRUE) {
            hmac_proc_he_uora_para_update(mac_vap, OAL_TRUE);
        } else {
            hmac_proc_he_uora_para_update(mac_vap, OAL_FALSE);
        }
    }
}


OAL_STATIC void hmac_set_he_capabilities_punctured_preamble(uint8_t *puc_buffer,
    oal_bool_enum_uint8 en_punctured_preamble)
{
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo = OAL_PTR_NULL;

    /* 获取HE PHY Capabilities Information 域信息 */
    puc_buffer += MAC_IE_HDR_LEN + 1 + MAC_HE_MAC_CAP_LEN;
    pst_he_phy_capinfo = (mac_frame_he_phy_cap_stru *)puc_buffer;
    /* 填充punctured_preamble字段 */
    if (en_punctured_preamble == OAL_TRUE) {
        pst_he_phy_capinfo->bit_punctured_preamble_rx = PUNCTURED_PREAMBLE_RX_EN; /* punctured preamble使能 */
    } else {
        pst_he_phy_capinfo->bit_punctured_preamble_rx = 0; /* 0代表punctured preamble未使能 */
    }
}


OAL_STATIC void hmac_set_he_capabilities_all_ie(hmac_scanned_bss_info *pst_scaned_bss, mac_vap_stru *pst_vap,
                                                uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t *pus_buffer_tp = puc_buffer;

    /* 对全部he capabilities信息进行填充 */
    mac_set_he_capabilities_ie((void *)pst_vap, puc_buffer, puc_ie_len);

    /* 对HE PHY Capabilities Information域信息中的punctured_preamble字段重新填充 */
    if (g_wlan_spec_cfg->feature_hiex_is_open && pst_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_set_he_capabilities_punctured_preamble(pus_buffer_tp,
            pst_scaned_bss->st_bss_dscr_info.en_punctured_preamble);
    }
}
#endif
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

static uint16_t hmac_set_assoc_req_opmode_ie(mac_vap_stru *mac_vap, uint8_t *req_frame)
{
    uint8_t ie_len = 0;
    hmac_user_stru *hmac_user = mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);
    if ((hmac_user != NULL) && (mac_vap->st_cap_flag.bit_opmode == OAL_TRUE) &&
        (mac_vap->en_protocol != WLAN_HE_MODE)) {
        mac_set_opmode_notify_ie((void *)mac_vap, req_frame, &ie_len);
    }

    oam_warning_log3(0, OAM_SF_ASSOC, "{hmac_set_assoc_req_opmode_ie::opmode[%d]ap_chip_oui[%d], protocol[%d].}\r\n",
        mac_vap->st_cap_flag.bit_opmode, mac_vap->bit_ap_chip_oui,mac_vap->en_protocol);
    return ie_len;
}
#endif

uint32_t hmac_mgmt_encap_asoc_req_sta(hmac_vap_stru *pst_hmac_sta, uint8_t *puc_req_frame,
    uint8_t *puc_curr_bssid)
{
    uint16_t us_fc;
    uint8_t uc_ie_len = 0;
    uint32_t us_asoc_rsq_len = 0;
    uint8_t *puc_req_frame_origin = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint16_t us_app_ie_len = 0;
    en_app_ie_type_uint8 en_app_ie_type = OAL_APP_ASSOC_REQ_IE;
    hmac_scanned_bss_info *pst_scaned_bss = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_11R
    wlan_wme_ac_type_enum_uint8 en_aci;
    uint8_t uc_tid;
#endif
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_FTM)
    oal_bool_enum_uint8 en_rrm_enable = OAL_TRUE;
#endif

    if (oal_any_null_ptr2(pst_hmac_sta, puc_req_frame)) {
        return us_asoc_rsq_len;
    }

    /* 保存起始地址，方便计算长度 */
    puc_req_frame_origin = puc_req_frame;

    pst_mac_vap = &(pst_hmac_sta->st_vap_base_info);

    /* 获取device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    pst_scaned_bss = hmac_vap_get_scan_bss_info(pst_mac_vap);

    if (oal_any_null_ptr2(pst_mac_device, pst_scaned_bss)) {
        return us_asoc_rsq_len;
    }
#ifdef _PRE_WLAN_FEATURE_11K
    pst_mac_vap->bit_bss_include_rrm_ie = pst_scaned_bss->st_bss_dscr_info.en_support_rrm;
#endif
#ifdef _PRE_WLAN_FEATURE_M2S
    hmac_update_user_sounding_dim_num(pst_mac_vap, pst_scaned_bss);
#endif
#ifdef _PRE_WLAN_FEATURE_TXBF
    hmac_sta_roam_and_assoc_update_txbf(pst_mac_vap, &pst_scaned_bss->st_bss_dscr_info);
#endif
    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* 设置 Frame Control field */
    /* 判断是否为reassoc操作 */
    us_fc = (puc_curr_bssid != OAL_PTR_NULL) ? WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_REASSOC_REQ :
        WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ASSOC_REQ;
    mac_hdr_set_frame_control(puc_req_frame, us_fc);

    /* 设置 DA address1: AP MAC地址 (BSSID) */
    oal_set_mac_addr(puc_req_frame + WLAN_HDR_ADDR1_OFFSET, pst_hmac_sta->st_vap_base_info.auc_bssid);

    /* 设置 SA address2: dot11MACAddress */
    oal_set_mac_addr(puc_req_frame + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(&pst_hmac_sta->st_vap_base_info));

    /* 设置 DA address3: AP MAC地址 (BSSID) */
    oal_set_mac_addr(puc_req_frame + WLAN_HDR_ADDR3_OFFSET, pst_hmac_sta->st_vap_base_info.auc_bssid);

    puc_req_frame += MAC_80211_FRAME_LEN;

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*              Association Request Frame - Frame Body                   */
    /* --------------------------------------------------------------------- */
    /* | Capability Information | Listen Interval | SSID | Supported Rates | */
    /* --------------------------------------------------------------------- */
    /* |2                       |2                |2-34  |3-10             | */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /* |Externed Surpported rates| Power Capability | Supported Channels   | */
    /* --------------------------------------------------------------------- */
    /* |3-257                    |4                 |4-256                 | */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /* | RSN   | QoS Capability | HT Capabilities | Extended Capabilities  | */
    /* --------------------------------------------------------------------- */
    /* |36-256 |3               |28               |3-8                     | */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /* | WPS   | P2P |                                                       */
    /* --------------------------------------------------------------------- */
    /* |7-257  |X    |                                                       */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    mac_set_cap_info_sta((void *)pst_mac_vap, puc_req_frame);
    puc_req_frame += MAC_CAP_INFO_LEN;

    /* 设置 Listen Interval IE */
    mac_set_listen_interval_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    /* Ressoc组帧设置Current AP address */
    if (puc_curr_bssid != OAL_PTR_NULL) {
        oal_set_mac_addr(puc_req_frame, puc_curr_bssid);
        puc_req_frame += OAL_MAC_ADDR_LEN;
    }
    /* 设置 SSID IE */
    mac_set_ssid_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len, WLAN_FC0_SUBTYPE_ASSOC_REQ);
    puc_req_frame += uc_ie_len;
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    /* 设置 Supported Rates IE */
    mac_set_supported_rates_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    /* 设置 Extended Supported Rates IE */
    mac_set_exsup_rates_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

#else
    /* 设置 Supported Rates IE */
    hmac_set_supported_rates_ie_asoc_req(pst_hmac_sta, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    /* 设置 Extended Supported Rates IE */
    hmac_set_exsup_rates_ie_asoc_req(pst_hmac_sta, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;
#endif

    /* 设置 Power Capability IE */
    mac_set_power_cap_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    /* 设置 Supported channel IE */
    mac_set_supported_channel_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34)
    if (pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_TRUE) {
        /* 设置 RSN Capability IE */
        mac_set_rsn_ie((void *)pst_mac_vap, OAL_PTR_NULL, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    } else if (pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE) {
        /* 设置 WPA Capability IE */
        mac_set_wpa_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#endif
    /* 填充WMM element */
    if (pst_mac_vap->st_cap_flag.bit_wmm_cap == OAL_TRUE) {
        mac_set_wmm_ie_sta((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }

    /* 设置 HT Capability IE  */
    mac_set_ht_capabilities_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    /* 更新 HT Capability IE  */
    hmac_set_ht_capabilities_ie_update((void *)pst_mac_vap, puc_req_frame, uc_ie_len, pst_scaned_bss);

#ifdef _PRE_WLAN_FEATURE_M2S
    if ((g_en_mimo_blacklist == OAL_TRUE) && (uc_ie_len != 0)) {
        hmac_assoc_set_siso_mode(pst_mac_vap, puc_req_frame);
    }
#endif

    puc_req_frame += uc_ie_len;

#if defined(_PRE_WLAN_FEATURE_11K) ||  defined(_PRE_WLAN_FEATURE_FTM)
        en_rrm_enable = (pst_hmac_sta->bit_11k_enable) && (pst_scaned_bss->st_bss_dscr_info.en_support_rrm);
        if ((pst_hmac_sta->bit_11k_auth_flag == OAL_TRUE) || (en_rrm_enable == OAL_TRUE)) {
            mac_set_rrm_enabled_cap_field((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
            puc_req_frame += uc_ie_len;
        }
#endif  // _PRE_WLAN_FEATURE_11K

    /* 设置 Extended Capability IE */
    if (hmac_sta_check_need_set_ext_cap_ie(pst_mac_vap) == OAL_TRUE) {
        mac_set_ext_capabilities_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }

    /* 设置 VHT Capability IE */
    if (HMAC_MGMT_ASOC_REQ_ENABLE_VHT_CAPABLE(pst_scaned_bss)) {
        mac_set_vht_capabilities_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    puc_req_frame += hmac_set_assoc_req_opmode_ie(pst_mac_vap, puc_req_frame);
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    hmac_mgmt_proc_he_uora(pst_mac_vap, pst_scaned_bss);

    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_set_he_capabilities_all_ie(pst_scaned_bss, pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM
    if (pst_scaned_bss->st_bss_dscr_info.en_support_1024qam == OAL_TRUE) {
        mac_set_1024qam_vendor_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_PRIV_CLOSED_LOOP_TPC
    if (!g_wlan_spec_cfg->feature_priv_closed_is_open) {
        /* 增加adjust pow IE */
        mac_set_adjust_pow_vendor_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_MBO
    /* MBO特性认证要求关联请求帧带Supported Operating Classes element 函数中已添加MBO定制化开关，开关未开启直接return */
    mac_set_supported_operating_classes_ie(pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;
#endif

#ifdef _PRE_WLAN_FEATURE_TXBF
    mac_set_11ntxbf_vendor_ie(pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;
#endif

    /* 填充 BCM Vendor VHT IE,解决与BCM AP的私有协议对通问题 */
    if (pst_scaned_bss->st_bss_dscr_info.en_vendor_vht_capable == OAL_TRUE) {
        mac_set_vendor_vht_ie(pst_mac_vap, puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
    if (pst_scaned_bss->st_bss_dscr_info.en_vendor_novht_capable == OAL_TRUE) {
        mac_set_vendor_novht_ie(pst_mac_vap, puc_req_frame, &uc_ie_len,
                                pst_scaned_bss->st_bss_dscr_info.en_vendor_1024qam_capable);
        puc_req_frame += uc_ie_len;
    }

    if (puc_curr_bssid != OAL_PTR_NULL) {
        en_app_ie_type = OAL_APP_REASSOC_REQ_IE;
        if ((pst_hmac_sta->bit_11r_enable == OAL_TRUE) && (mac_mib_get_ft_trainsistion(pst_mac_vap) == OAL_TRUE)) {
            en_app_ie_type = OAL_APP_FT_IE;
        }
    }

#ifdef _PRE_WLAN_NARROW_BAND
    if (HMAC_MGMT_ASOC_REQ_ENABLE_NB(pst_scaned_bss, pst_mac_vap)) {
        mac_set_nb_ie(puc_req_frame, &uc_ie_len);
        puc_req_frame += uc_ie_len;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        puc_req_frame += hmac_hiex_encap_ie(pst_mac_vap, OAL_PTR_NULL, puc_req_frame);
        puc_req_frame += hmac_assoc_req_set_max_tx_power_ie(puc_req_frame);
    }
#endif
    mac_set_hisi_cap_vendor_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;

    /* 填充P2P/WPS IE 信息 */
    mac_add_app_ie(pst_mac_vap, puc_req_frame, &us_app_ie_len, en_app_ie_type);
    puc_req_frame += us_app_ie_len;

#ifdef _PRE_WLAN_FEATURE_11R
    if (pst_hmac_sta->bit_11r_enable == OAL_TRUE) {
        /* Q版本关联时wpa_s下发IE中携带MD IE, P版本不携带，此处先判断是否已经携带，避免重复添加MD IE */
        if ((OAL_FALSE == pst_hmac_sta->bit_reassoc_flag && OAL_PTR_NULL == mac_find_ie(MAC_EID_MOBILITY_DOMAIN,
                pst_mac_vap->ast_app_ie[en_app_ie_type].puc_ie, pst_mac_vap->ast_app_ie[en_app_ie_type].ul_ie_len))
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
            || pst_hmac_sta->bit_voe_11r_auth == 1) /* voe 11r 认证实验室环境必须携带两个mdie否则无法正常漫游 */
#else
        )
#endif
        {
            mac_set_md_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
            puc_req_frame += uc_ie_len;
        } else { /* Reasoc中包含RIC-Req */
            for (en_aci = WLAN_WME_AC_BE; en_aci < WLAN_WME_AC_BUTT; en_aci++) {
                if (mac_mib_get_QAPEDCATableMandatory(&pst_hmac_sta->st_vap_base_info, en_aci)) {
                    uc_tid = WLAN_WME_AC_TO_TID(en_aci);
                    mac_set_rde_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
                    puc_req_frame += uc_ie_len;

                    mac_set_tspec_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len, uc_tid);
                    puc_req_frame += uc_ie_len;
                }
            }
        }
    }
#endif  // _PRE_WLAN_FEATURE_11R

    /* 填充端管协同IE */
    mac_add_cowork_ie((void *)pst_hmac_sta, puc_req_frame, &us_app_ie_len);
    puc_req_frame += us_app_ie_len;

    /* multi-sta特性下新增4地址ie */
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    mac_set_vender_4addr_ie((void *)pst_mac_vap, puc_req_frame, &uc_ie_len);
    puc_req_frame += uc_ie_len;
#endif
    us_asoc_rsq_len = (uint32_t)(puc_req_frame - puc_req_frame_origin);

    return us_asoc_rsq_len;
}


uint16_t hmac_mgmt_encap_auth_req(hmac_vap_stru *pst_hmac_sta, uint8_t *puc_mgmt_frame)
{
    uint16_t us_auth_req_len;
    hmac_user_stru *pst_user_ap = OAL_PTR_NULL;
    uint16_t us_auth_type;
    uint32_t ul_ret;
    uint16_t us_user_index;
#ifdef _PRE_WLAN_FEATURE_11R
    uint16_t us_app_ie_len;
#endif  // _PRE_WLAN_FEATURE_11R

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    mac_hdr_set_frame_control(puc_mgmt_frame, WLAN_FC0_SUBTYPE_AUTH);

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address1,
                     pst_hmac_sta->st_vap_base_info.auc_bssid);

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address2,
                     mac_mib_get_StationID(&pst_hmac_sta->st_vap_base_info));

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address3,
                     pst_hmac_sta->st_vap_base_info.auc_bssid);
    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*              Authentication Frame (Sequence 1) - Frame Body           */
    /* --------------------------------------------------------------------  */
    /* |Auth Algorithm Number|Auth Transaction Sequence Number|Status Code|  */
    /* --------------------------------------------------------------------  */
    /* | 2                   |2                               |2          |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    if (OAL_FALSE == mac_mib_get_privacyinvoked(&pst_hmac_sta->st_vap_base_info)) {
        /* Open System */
        puc_mgmt_frame[MAC_80211_FRAME_LEN] = 0x00;
        puc_mgmt_frame[MAC_80211_FRAME_LEN + 1] = 0x00;
    } else {
        us_auth_type = (uint16_t)mac_mib_get_AuthenticationMode(&pst_hmac_sta->st_vap_base_info);

        if (us_auth_type == WLAN_WITP_AUTH_SHARED_KEY) {
            oam_info_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                          "{hmac_mgmt_encap_auth_req::WLAN_WITP_AUTH_SHARED_KEY.}");
            us_auth_type = WLAN_WITP_AUTH_SHARED_KEY;
        } else {
            oam_info_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                          "{hmac_mgmt_encap_auth_req::WLAN_WITP_AUTH_OPEN_SYSTEM.}");
            us_auth_type = WLAN_WITP_AUTH_OPEN_SYSTEM;
        }

        puc_mgmt_frame[MAC_80211_FRAME_LEN] = (us_auth_type & 0xFF);
        puc_mgmt_frame[MAC_80211_FRAME_LEN + 1] = ((us_auth_type & 0xFF00) >> 8);
    }

    /* 设置 Authentication Transaction Sequence Number 为 1 */
    puc_mgmt_frame[MAC_80211_FRAME_LEN + 2] = 0x01;
    puc_mgmt_frame[MAC_80211_FRAME_LEN + 3] = 0x00;

    /* 设置 Status Code 为0. 这个包的这个字段没用 . */
    puc_mgmt_frame[MAC_80211_FRAME_LEN + 4] = 0x00;
    puc_mgmt_frame[MAC_80211_FRAME_LEN + 5] = 0x00;

    /* 设置 认证帧的长度 */
    us_auth_req_len = MAC_80211_FRAME_LEN + MAC_AUTH_ALG_LEN + MAC_AUTH_TRANS_SEQ_NUM_LEN +
                      MAC_STATUS_CODE_LEN;

#ifdef _PRE_WLAN_FEATURE_11R
    if (pst_hmac_sta->bit_11r_enable == OAL_TRUE) {
        if ((OAL_TRUE == mac_mib_get_ft_trainsistion(&pst_hmac_sta->st_vap_base_info))
            && (pst_hmac_sta->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING)) {
            /* FT System */
            puc_mgmt_frame[MAC_80211_FRAME_LEN] = 0x02;
            puc_mgmt_frame[MAC_80211_FRAME_LEN + 1] = 0x00;
            puc_mgmt_frame += us_auth_req_len;

            mac_add_app_ie((void *)&pst_hmac_sta->st_vap_base_info, puc_mgmt_frame,
                               &us_app_ie_len, OAL_APP_FT_IE);
            us_auth_req_len += us_app_ie_len;
            puc_mgmt_frame += us_app_ie_len;
        }
    }
#endif  // _PRE_WLAN_FEATURE_11R

    pst_user_ap = (hmac_user_stru *)mac_res_get_hmac_user(pst_hmac_sta->st_vap_base_info.us_assoc_vap_id);
    if (pst_user_ap == OAL_PTR_NULL) {
        oam_info_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_mgmt_encap_auth_req::no present ap, alloc new ap.}");
        ul_ret = hmac_user_add(&pst_hmac_sta->st_vap_base_info,
                                   pst_hmac_sta->st_vap_base_info.auc_bssid, &us_user_index);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_mgmt_encap_auth_req::hmac_user_add failed[%d].}", ul_ret);
            us_auth_req_len = 0;
        }
    }
    return us_auth_req_len;
}


uint16_t hmac_mgmt_encap_auth_req_seq3(hmac_vap_stru *pst_sta, uint8_t *puc_mgmt_frame, uint8_t *puc_mac_hrd)
{
    uint8_t *puc_data = OAL_PTR_NULL;
    uint16_t us_index, us_auth_req_len;
    uint8_t *puc_ch_text = OAL_PTR_NULL;
    uint8_t uc_ch_text_len = 0;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    mac_hdr_set_frame_control(puc_mgmt_frame, WLAN_FC0_SUBTYPE_AUTH);

    /* 将帧保护字段置1 */
    mac_set_wep(puc_mgmt_frame, 1);

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address1, pst_sta->st_vap_base_info.auc_bssid);

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address2,
                     mac_mib_get_StationID(&pst_sta->st_vap_base_info));

    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_mgmt_frame)->auc_address3, pst_sta->st_vap_base_info.auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*              Authentication Frame (Sequence 3) - Frame Body           */
    /* --------------------------------------------------------------------- */
    /* |Auth Algo Number|Auth Trans Seq Number|Status Code| Challenge Text | */
    /* --------------------------------------------------------------------- */
    /* | 2              |2                    |2          | 3 - 256        | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* 获取认证帧payload */
    us_index = MAC_80211_FRAME_LEN;
    puc_data = (uint8_t *)(puc_mgmt_frame + us_index);

    /* 设置 认证帧的长度 */
    us_auth_req_len = MAC_80211_FRAME_LEN + MAC_AUTH_ALG_LEN + MAC_AUTH_TRANS_SEQ_NUM_LEN + MAC_STATUS_CODE_LEN;

    /* In case of no failure, the frame must be WEP encrypted. 4 bytes must  */
    /* be   left for the  IV  in  that  case. These   fields will  then  be  */
    /* reinitialized, using the correct index, with offset for IV field.     */
    puc_data[0] = WLAN_WITP_AUTH_SHARED_KEY; /* Authentication Algorithm Number               */
    puc_data[1] = 0x00;

    puc_data[2] = 0x03; /* Authentication Transaction Sequence Number    */
    puc_data[3] = 0x00;

    /* If WEP subfield in the  incoming  authentication frame is 1,  respond */
    /* with  'challenge text failure' status,  since the STA does not expect */
    /* an encrypted frame in this state.                                     */
    if (1 == mac_is_protectedframe(puc_mac_hrd)) {
        puc_data[4] = MAC_CHLNG_FAIL;
        puc_data[5] = 0x00;
    }

    /* If the STA does not support WEP, respond with 'unsupported algo'      */
    /* status, since WEP is necessary for Shared Key Authentication.         */
    else if (OAL_FALSE == mac_is_wep_enabled(&(pst_sta->st_vap_base_info))) {
        puc_data[4] = MAC_UNSUPT_ALG;
        puc_data[5] = 0x00;
    }

    /* If the default WEP key is NULL, respond with 'challenge text failure' */
    /* status, since a NULL key value cannot be used for WEP operations.     */
    else if (mac_get_wep_default_keysize(&pst_sta->st_vap_base_info) == 0) {
        puc_data[4] = MAC_CHLNG_FAIL;
        puc_data[5] = 0x00;
    }

    /* If there is a mapping in dot11WEPKeyMappings matching the address of  */
    /* the AP, and the corresponding key is NULL respond with 'challenge     */
    /* text failure' status. This is currently not being used.               */
    /* No error condition detected */
    else {
        /* Set Status Code to 'success' */
        puc_data[4] = MAC_SUCCESSFUL_STATUSCODE;
        puc_data[5] = 0x00;

        /* Extract 'Challenge Text' and its 'length' from the incoming       */
        /* authentication frame                                              */
        uc_ch_text_len = puc_mac_hrd[MAC_80211_FRAME_LEN + 7];
        puc_ch_text = (uint8_t *)(&puc_mac_hrd[MAC_80211_FRAME_LEN + 8]);

        /* Challenge Text Element                  */
        /* --------------------------------------- */
        /* |Element ID | Length | Challenge Text | */
        /* --------------------------------------- */
        /* | 1         |1       |1 - 253         | */
        /* --------------------------------------- */

        puc_mgmt_frame[us_index + 6] = MAC_EID_CHALLENGE;
        puc_mgmt_frame[us_index + 7] = uc_ch_text_len;
        if (memcpy_s(&puc_mgmt_frame[us_index + 8], uc_ch_text_len, puc_ch_text, uc_ch_text_len) != EOK) {
            oam_warning_log0(0, 0, "{hmac_mgmt_encap_auth_req_seq3::memcpy fail}");
        }

        /* Add the challenge text element length to the authentication       */
        /* request frame length. The IV, ICV element lengths will be added   */
        /* after encryption.                                                 */
        us_auth_req_len += (uc_ch_text_len + MAC_IE_HDR_LEN);
    }
    return us_auth_req_len;
}
