

/* 1 ͷ�ļ����� */
#include "oal_cfg80211.h"
#include "oal_net.h"
#include "wlan_spec.h"
#include "mac_frame.h"
#include "mac_user.h"
#include "mac_vap.h"

#include "hmac_encap_frame_ap.h"
#include "hmac_main.h"
#include "hmac_tx_data.h"
#include "hmac_mgmt_ap.h"
#include "hmac_11i.h"
#include "hmac_blockack.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_blacklist.h"

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ENCAP_FRAME_AP_C

/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

uint32_t hmac_get_assoc_comeback_time(mac_vap_stru *pst_mac_vap,
                                        hmac_user_stru *pst_hmac_user)
{
    uint32_t ul_timeout = 0;

    uint32_t ul_now_time;
    uint32_t ul_passed_time;
    uint32_t ul_sa_query_maxtimeout;

    /* ��ȡ����ʱ�� */
    ul_now_time = (uint32_t)oal_time_get_stamp_ms();

    /* ����ASSOCIATION_COMEBACK_TIME��ʹSTA��AP���SA Query����֮���ٷ��͹������� */
    /* ���sa Query Max timeoutֵ */
    ul_sa_query_maxtimeout = mac_mib_get_dot11AssociationSAQueryMaximumTimeout(pst_mac_vap);

    /* �Ƿ�������sa Query�������ڽ��� */
    if ((pst_hmac_user->st_sa_query_info.ul_sa_query_count != 0) &&
        (ul_now_time >= pst_hmac_user->st_sa_query_info.ul_sa_query_start_time)) {
        /* ������SA Query���̽�����ſ��Խ���STA�������Ĺ���֡ */
        ul_passed_time = ul_now_time - pst_hmac_user->st_sa_query_info.ul_sa_query_start_time;
        ul_timeout = ul_sa_query_maxtimeout - ul_passed_time;
    } else {
        /* ����������SA Query����Ԥ��ʱ�� */
        ul_timeout = ul_sa_query_maxtimeout;
    }

    return ul_timeout;
}
#endif


void hmac_set_supported_rates_ie_asoc_rsp(mac_user_stru *pst_mac_user,
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
    uc_nrates = pst_mac_user->st_avail_op_rates.uc_rs_nrates;

    if (uc_nrates > MAC_MAX_SUPRATES) {
        uc_nrates = MAC_MAX_SUPRATES;
    }

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_mac_user->st_avail_op_rates.auc_rs_rates[uc_idx];
    }

    puc_buffer[1] = uc_nrates;
    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}


void hmac_set_exsup_rates_ie_asoc_rsp(mac_user_stru *pst_mac_user, uint8_t *puc_buffer, uint8_t *puc_ie_len)
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

    if (pst_mac_user->st_avail_op_rates.uc_rs_nrates <= MAC_MAX_SUPRATES) {
        *puc_ie_len = 0;
        return;
    }

    puc_buffer[0] = MAC_EID_XRATES;
    uc_nrates = pst_mac_user->st_avail_op_rates.uc_rs_nrates - MAC_MAX_SUPRATES;
    puc_buffer[1] = uc_nrates;

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_mac_user->st_avail_op_rates.auc_rs_rates[uc_idx + MAC_MAX_SUPRATES];
    }

    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}

#ifdef _PRE_WLAN_FEATURE_TXBF_HT
OAL_STATIC OAL_INLINE void hmac_mgmt_set_txbf_cap_para(uint8_t *puc_asoc_rsp)
{
    mac_txbf_cap_stru *pst_txbf_cap;

    pst_txbf_cap = (mac_txbf_cap_stru *)puc_asoc_rsp;
    pst_txbf_cap->bit_rx_stagg_sounding = OAL_TRUE;
    pst_txbf_cap->bit_explicit_compr_bf_fdbk = 1;
    pst_txbf_cap->bit_compr_steering_num_bf_antssup = 1;
    pst_txbf_cap->bit_minimal_grouping = 3;
    pst_txbf_cap->bit_chan_estimation = 1;
}
#endif

#if defined(_PRE_WLAN_FEATURE_160M)
OAL_STATIC void hmac_mgmt_encap_asoc_rsp_ap_set_vht_oper(mac_vap_stru *pst_mac_ap,
    uint8_t uc_ie_len, mac_user_stru *pst_mac_user, uint8_t *puc_asoc_rsp)
{
    mac_vht_opern_stru *pst_vht_opern = OAL_PTR_NULL;

    if ((pst_mac_ap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
        (pst_mac_ap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        if ((uc_ie_len != 0) && (pst_mac_user->st_vht_hdl.bit_short_gi_160mhz == 0)) {
            pst_vht_opern = (mac_vht_opern_stru *)(puc_asoc_rsp + MAC_IE_HDR_LEN);
            pst_vht_opern->uc_channel_center_freq_seg1 = 0;
        }
    }
}
#endif


uint32_t hmac_mgmt_encap_asoc_rsp_ap(mac_vap_stru *pst_mac_ap, const unsigned char *puc_sta_addr,
    uint16_t us_assoc_id, mac_status_code_enum_uint16 en_status_code, uint8_t *puc_asoc_rsp, uint16_t us_type)
{
    uint32_t us_asoc_rsp_len = 0;
    uint32_t ul_timeout;
    uint8_t uc_ie_len = 0;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint8_t *puc_asoc_rsp_original = OAL_PTR_NULL;
    mac_Timeout_Interval_type_enum en_tie_type = MAC_TIE_BUTT;

    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint16_t us_app_ie_len = 0;

    if (oal_any_null_ptr3(pst_mac_ap, puc_sta_addr, puc_asoc_rsp)) {
        oam_error_log3(0, OAM_SF_ASSOC, "{hmac_mgmt_encap_asoc_rsp_ap::pst_mac_ap[0x%x], puc_data[0x%x], \
            puc_asoc_req[0x%x].}", (uintptr_t)pst_mac_ap, (uintptr_t)puc_sta_addr, (uintptr_t)puc_asoc_rsp);
        return us_asoc_rsp_len;
    }

    /* ������ʼ��ַ��������㳤�� */
    puc_asoc_rsp_original = puc_asoc_rsp;

    /* ��ȡdevice */
    pst_mac_device = mac_res_get_dev(pst_mac_ap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_ap->uc_vap_id, OAM_SF_ASSOC, "{hmac_mgmt_encap_asoc_rsp_ap::pst_mac_device NULL}");
    }

    pst_mac_user = mac_res_get_mac_user(us_assoc_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ASSOC, "{hmac_mgmt_encap_asoc_rsp_ap::pst_mac_user[%d] null ptr.}", us_assoc_id);
        return us_asoc_rsp_len;
    }
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_assoc_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ASSOC, "{hmac_mgmt_encap_asoc_rsp_ap::pst_hmac_user(idx=%d) NULL}", us_assoc_id);
        return us_asoc_rsp_len;
    }

    if (en_status_code == MAC_REJECT_TEMP) {
        en_tie_type = MAC_TIE_ASSOCIATION_COMEBACK_TIME;
    }

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
    /* ���� Frame Control field */
    mac_hdr_set_frame_control(puc_asoc_rsp, us_type);

    /* ���� DA address1: STA MAC��ַ */
    oal_set_mac_addr(puc_asoc_rsp + WLAN_HDR_ADDR1_OFFSET, puc_sta_addr);

    /* ���� SA address2: dot11MACAddress */
    oal_set_mac_addr(puc_asoc_rsp + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(pst_mac_ap));

    /* ���� DA address3: AP MAC��ַ (BSSID) */
    oal_set_mac_addr(puc_asoc_rsp + WLAN_HDR_ADDR3_OFFSET, pst_mac_ap->auc_bssid);

    puc_asoc_rsp += MAC_80211_FRAME_LEN;

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*              Association Response Frame - Frame Body                  */
    /* --------------------------------------------------------------------- */
    /* | Capability Information |   Status Code   | AID | Supported  Rates | */
    /* --------------------------------------------------------------------- */
    /* |2                       |2                |2    |3-10              | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* ���� capability information field */
    mac_set_cap_info_ap((void *)pst_mac_ap, puc_asoc_rsp);
    puc_asoc_rsp += MAC_CAP_INFO_LEN;

    /* ���� Status Code */
    mac_set_status_code_ie(puc_asoc_rsp, en_status_code);
    puc_asoc_rsp += MAC_STATUS_CODE_LEN;

    /* ���� Association ID */
    mac_set_aid_ie(puc_asoc_rsp, us_assoc_id);
    puc_asoc_rsp += MAC_AID_LEN;

#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    /* ���� Supported Rates IE */
    mac_set_supported_rates_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;

    /* ���� Extended Supported Rates IE */
    mac_set_exsup_rates_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;
#else
    /* ���� Supported Rates IE */
    hmac_set_supported_rates_ie_asoc_rsp(pst_mac_user, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;

    /* ���� Extended Supported Rates IE */
    hmac_set_exsup_rates_ie_asoc_rsp(pst_mac_user, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;
#endif

    /* ���� EDCA IE */
    mac_set_wmm_params_ie((void *)pst_mac_ap, puc_asoc_rsp, pst_mac_user->st_cap_info.bit_qos, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;

    /* ���� Timeout Interval (Association Comeback time) IE */
    ul_timeout = hmac_get_assoc_comeback_time(pst_mac_ap, pst_hmac_user);
    mac_set_timeout_interval_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len, en_tie_type, ul_timeout);
    puc_asoc_rsp += uc_ie_len;

    if (pst_mac_user->st_ht_hdl.en_ht_capable == OAL_TRUE) {
        /* ���� HT-Capabilities Information IE */
        mac_set_ht_capabilities_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
#ifdef _PRE_WLAN_FEATURE_TXBF_HT
        if (oal_all_true_value2(pst_mac_user->st_cap_info.bit_11ntxbf, pst_mac_ap->st_cap_flag.bit_11ntxbf) &&
            (uc_ie_len != 0)) {
            puc_asoc_rsp += MAC_11N_TXBF_CAP_OFFSET;
            hmac_mgmt_set_txbf_cap_para(puc_asoc_rsp); // ��װ������������
            puc_asoc_rsp -= MAC_11N_TXBF_CAP_OFFSET;
        }
#endif
        puc_asoc_rsp += uc_ie_len;

        /* ���� HT-Operation Information IE */
        mac_set_ht_opern_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;

        /* ���� Overlapping BSS Scan Parameters Information IE */
        mac_set_obss_scan_params((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;

        /* ���� Extended Capabilities Information IE */
        mac_set_ext_capabilities_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
    }

    if (pst_mac_user->st_vht_hdl.en_vht_capable == OAL_TRUE) {
        /* ���� VHT Capabilities IE */
        mac_set_vht_capabilities_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;

        /* ���� VHT Opern IE */
        mac_set_vht_opern_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        /* AP 160M ����Զ�����������VHT Oper�ֶ� */
#if defined(_PRE_WLAN_FEATURE_160M)
        hmac_mgmt_encap_asoc_rsp_ap_set_vht_oper(pst_mac_ap, uc_ie_len, pst_mac_user, puc_asoc_rsp);
#endif
        puc_asoc_rsp += uc_ie_len;

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
        mac_set_opmode_notify_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM
        mac_set_1024qam_vendor_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
#endif
    }

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        /* ���� HE Capabilities �� HE Operation IE */
        mac_set_he_ie_in_assoc_rsp((void *)pst_mac_ap, us_assoc_id, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
    }
#endif
    mac_set_hisi_cap_vendor_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;

#ifdef _PRE_WLAN_FEATURE_TXBF
    mac_set_11ntxbf_vendor_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        puc_asoc_rsp += hmac_hiex_encap_ie((void *)pst_mac_ap, OAL_PTR_NULL, puc_asoc_rsp);
    }
#endif

    /* ���WPS��Ϣ */
    mac_add_app_ie((void *)pst_mac_ap, puc_asoc_rsp, &us_app_ie_len, OAL_APP_ASSOC_RSP_IE);
    puc_asoc_rsp += us_app_ie_len;

#ifdef _PRE_WLAN_FEATURE_HISTREAM
    mac_set_histream_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;
#endif  // _PRE_WLAN_FEATURE_HISTREAM

#ifdef _PRE_WLAN_NARROW_BAND
    if (pst_mac_ap->st_nb.en_open) {
        mac_set_nb_ie(puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
    }
#endif
    /* ��� BCM Vendor VHT IE,�����BCM STA��˽��Э���ͨ���� */
    if (pst_hmac_user->en_user_vendor_vht_capable == OAL_TRUE) {
        mac_set_vendor_vht_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
    }
    /* 5Gʱ��BCM˽��vendor ie�в�Я��vht,�谴�մ˸�ʽ��֡ */
    if (pst_hmac_user->en_user_vendor_novht_capable == OAL_TRUE) {
        mac_set_vendor_novht_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len, OAL_FALSE);
        puc_asoc_rsp += uc_ie_len;
    }
    /* multi-sta����������4��ַie */
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    mac_set_vender_4addr_ie((void *)pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
    puc_asoc_rsp += uc_ie_len;
#endif

    us_asoc_rsp_len = (uint32_t)(puc_asoc_rsp - puc_asoc_rsp_original);

    if (en_status_code != MAC_SUCCESSFUL_STATUSCODE) {
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, en_status_code);
    }
    return us_asoc_rsp_len;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_mgmt_is_challenge_txt_equal(uint8_t *puc_data, uint8_t *puc_chtxt)
{
    uint8_t *puc_ch_text = 0;
    uint16_t us_idx = 0;
    uint8_t uc_ch_text_len;

    if (oal_any_null_ptr2(puc_data, puc_chtxt)) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_mgmt_is_challenge_txt_equal::puc_data[0x%x] or puc_chtxt[0x%x] is NULL.}",
                       (uintptr_t)puc_data, (uintptr_t)puc_chtxt);
        return OAL_FALSE;
    }

    /* Challenge Text Element                  */
    /* --------------------------------------- */
    /* |Element ID | Length | Challenge Text | */
    /* --------------------------------------- */
    /* | 1         |1       |1 - 253         | */
    /* --------------------------------------- */
    uc_ch_text_len = puc_data[1];
    puc_ch_text = puc_data + 2;
    if (uc_ch_text_len > WLAN_CHTXT_SIZE) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mgmt_is_challenge_txt_equal::uc_ch_text_len > 128.}");
        return OAL_FALSE;
    }
    for (us_idx = 0; us_idx < uc_ch_text_len; us_idx++) {
        /* Return false on mismatch */
        if (puc_ch_text[us_idx] != puc_chtxt[us_idx]) {
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}


oal_err_code_enum hmac_encap_auth_rsp_get_user_idx(mac_vap_stru *pst_mac_vap,
                                                       uint8_t *puc_mac_addr,
                                                       uint8_t uc_mac_len,
                                                       uint8_t uc_is_seq1,
                                                       uint8_t *puc_auth_resend,
                                                       uint16_t *pus_user_index)
{
    uint32_t ul_ret;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_user_stru *pst_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_other_vap = OAL_PTR_NULL;
    uint16_t us_user_idx;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if ((pst_hmac_vap == OAL_PTR_NULL) || (uc_mac_len != ETHER_ADDR_LEN)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{hmac_encap_auth_rsp_get_user_idx::mac_res_get_hmac_vap failed!}");
        return OAL_FAIL;
    }

    *puc_auth_resend = OAL_FALSE;
    ul_ret = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info), puc_mac_addr, pus_user_index);
    /* �ҵ��û� */
    if (ul_ret == OAL_SUCC) {
        /* ��ȡhmac�û���״̬���������0��˵�����ظ�֡ */
        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(*pus_user_index);
        if (pst_hmac_user == OAL_PTR_NULL) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                           "{hmac_encap_auth_rsp_get_user_idx::mac_res_get_hmac_user return null}");
            return OAL_FAIL;
        }
        /* en_user_asoc_stateΪö�ٱ�����ȡֵΪ1~4����ʼ��ΪMAC_USER_STATE_BUTT��
         * Ӧ��ʹ��!=MAC_USER_STATE_BUTT��Ϊ�жϣ�����ᵼ��WEP share���ܹ�����������
         */
        if (pst_hmac_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_BUTT) {
            *puc_auth_resend = OAL_TRUE;
        }

        if (pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
            oal_net_device_stru *pst_net_device;

            pst_net_device = hmac_vap_get_net_device(pst_mac_vap->uc_vap_id);
            if (pst_net_device != OAL_PTR_NULL) {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
                oal_kobject_uevent_env_sta_leave(pst_net_device, puc_mac_addr);
#endif
            }
        }

        return OAL_SUCC;
    }

    /* ����ͬһchip�µ�����VAP���ҵ����û���ɾ��֮��������ҵ��ͨ����DBAC�����䳣�� */
    if (OAL_SUCC == mac_chip_find_user_by_macaddr(pst_hmac_vap->st_vap_base_info.uc_chip_id,
                                                  puc_mac_addr, &us_user_idx)) {
        pst_user = mac_res_get_hmac_user(us_user_idx);
        if (pst_user != OAL_PTR_NULL) {
            pst_hmac_other_vap = mac_res_get_hmac_vap(pst_user->st_user_base_info.uc_vap_id);
            if (pst_hmac_other_vap != OAL_PTR_NULL && pst_hmac_other_vap != pst_hmac_vap) {
                /* ���¼��ϱ��ںˣ��Ѿ�ɾ��ĳ��STA */
                hmac_mgmt_send_disassoc_frame(&(pst_hmac_other_vap->st_vap_base_info),
                                                  puc_mac_addr, MAC_DISAS_LV_SS, OAL_FALSE);
                hmac_handle_disconnect_rsp_ap(pst_hmac_other_vap, pst_user);
                hmac_user_del(&(pst_hmac_other_vap->st_vap_base_info), pst_user);
            }
        }
    }

    /* ���յ���һ����֤֡ʱ�û��Ѵ��� */
    if (!uc_is_seq1) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_encap_auth_rsp_get_user_idx::user should have been added at seq1!}");
        return OAL_FAIL;
    }

    ul_ret = hmac_user_add(pst_mac_vap, puc_mac_addr, pus_user_index);
    if (ul_ret != OAL_SUCC) {
        return (ul_ret == OAL_ERR_CODE_CONFIG_EXCEED_SPEC) ? OAL_ERR_CODE_CONFIG_EXCEED_SPEC : OAL_FAIL;
    }

    return OAL_SUCC;
}


hmac_ap_auth_process_code_enum_uint8 hmac_encap_auth_rsp_seq1(mac_vap_stru *pst_mac_vap,
                                                                  hmac_auth_rsp_param_stru *pst_auth_rsp_param,
                                                                  uint8_t *puc_code,
                                                                  mac_user_asoc_state_enum_uint8 *pst_usr_ass_stat)
{
    *puc_code = MAC_SUCCESSFUL_STATUSCODE;
    *pst_usr_ass_stat = MAC_USER_STATE_BUTT;
    /* ��������ش� */
    if (pst_auth_rsp_param->uc_auth_resend != OAL_TRUE) {
        if (pst_auth_rsp_param->us_auth_type == WLAN_WITP_AUTH_OPEN_SYSTEM) {
            *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;

            return HMAC_AP_AUTH_SEQ1_OPEN_ANY;
        }

        if (pst_auth_rsp_param->en_is_wep_allowed == OAL_TRUE) {
            *pst_usr_ass_stat = MAC_USER_STATE_AUTH_KEY_SEQ1;
            /* �˴����غ���Ҫwep����� */
            return HMAC_AP_AUTH_SEQ1_WEP_NOT_RESEND;
        }

        /* ��֧���㷨 */
        *puc_code = MAC_UNSUPT_ALG;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, *puc_code);
        return HMAC_AP_AUTH_BUTT;
    }

    /* ����û�״̬ */
    if ((pst_auth_rsp_param->en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (pst_auth_rsp_param->us_auth_type == WLAN_WITP_AUTH_OPEN_SYSTEM)) {
        if (OAL_TRUE == mac_mib_get_dot11RSNAMFPC(pst_mac_vap)) {
            *pst_usr_ass_stat = MAC_USER_STATE_ASSOC;
        } else {
            *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        }

        return HMAC_AP_AUTH_DUMMY;
    }

    if (pst_auth_rsp_param->us_auth_type == WLAN_WITP_AUTH_OPEN_SYSTEM) {
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;

        return HMAC_AP_AUTH_SEQ1_OPEN_ANY;
    }

    if (pst_auth_rsp_param->en_is_wep_allowed == OAL_TRUE) {
        /* seqΪ1 ����֤֡�ش� */
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        return HMAC_AP_AUTH_SEQ1_WEP_RESEND;
    }
    /* ��֧���㷨 */
    *puc_code = MAC_UNSUPT_ALG;
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, *puc_code);
    return HMAC_AP_AUTH_BUTT;
}

hmac_ap_auth_process_code_enum_uint8 hmac_encap_auth_rsp_seq3(mac_vap_stru *pst_mac_vap,
                                                                  hmac_auth_rsp_param_stru *pst_auth_rsp_param,
                                                                  uint8_t *puc_code,
                                                                  mac_user_asoc_state_enum_uint8 *pst_usr_ass_stat)
{
    /* ��������ڣ����ش��� */
    if (pst_auth_rsp_param->uc_auth_resend == OAL_FALSE) {
        *pst_usr_ass_stat = MAC_USER_STATE_BUTT;
        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_BUTT;
    }
    /* ����û�״̬ */
    if ((pst_auth_rsp_param->en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (pst_auth_rsp_param->us_auth_type == WLAN_WITP_AUTH_OPEN_SYSTEM)) {
        if (OAL_TRUE == mac_mib_get_dot11RSNAMFPC(pst_mac_vap)) {
            *pst_usr_ass_stat = MAC_USER_STATE_ASSOC;
        } else {
            *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        }

        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_DUMMY;
    }

    if (pst_auth_rsp_param->us_auth_type == WLAN_WITP_AUTH_OPEN_SYSTEM) {
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_SEQ3_OPEN_ANY;
    }

    if (pst_auth_rsp_param->en_user_asoc_state == MAC_USER_STATE_AUTH_KEY_SEQ1) {
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_SEQ3_WEP_COMPLETE;
    }

    if (pst_auth_rsp_param->en_user_asoc_state == MAC_USER_STATE_AUTH_COMPLETE) {
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_COMPLETE;
        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_SEQ3_WEP_COMPLETE;
    }

    if (pst_auth_rsp_param->en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        *pst_usr_ass_stat = MAC_USER_STATE_AUTH_KEY_SEQ1;
        *puc_code = MAC_SUCCESSFUL_STATUSCODE;
        return HMAC_AP_AUTH_SEQ3_WEP_ASSOC;
    }

    /* ��֧���㷨 */
    *pst_usr_ass_stat = MAC_USER_STATE_BUTT;
    *puc_code = MAC_UNSUPT_ALG;
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, *puc_code);
    return HMAC_AP_AUTH_BUTT;
}


hmac_auth_rsp_fun hmac_encap_auth_rsp_get_func(uint16_t us_auth_seq)
{
    hmac_auth_rsp_fun st_auth_rsp_fun = OAL_PTR_NULL;
    switch (us_auth_seq) {
        case WLAN_AUTH_TRASACTION_NUM_ONE:
            st_auth_rsp_fun = hmac_encap_auth_rsp_seq1;
            break;
        case WLAN_AUTH_TRASACTION_NUM_THREE:
            st_auth_rsp_fun = hmac_encap_auth_rsp_seq3;
            break;
        default:
            st_auth_rsp_fun = OAL_PTR_NULL;
            break;
    }
    return st_auth_rsp_fun;
}


uint32_t hmac_encap_auth_rsp_support(hmac_vap_stru *pst_hmac_vap, uint16_t us_auth_type)
{
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �����֤�����Ƿ�֧�� ��֧�ֵĻ�״̬λ�ó�UNSUPT_ALG */
    if ((mac_mib_get_AuthenticationMode(&pst_hmac_vap->st_vap_base_info) != us_auth_type)
        && (WLAN_WITP_AUTH_AUTOMATIC != mac_mib_get_AuthenticationMode(&pst_hmac_vap->st_vap_base_info))) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }
    return OAL_SUCC;
}


void hmac_tid_clear(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    uint8_t uc_loop;
    hmac_amsdu_stru *pst_amsdu = OAL_PTR_NULL;
    oal_netbuf_stru *pst_amsdu_net_buf = OAL_PTR_NULL;
    hmac_tid_stru *pst_tid = OAL_PTR_NULL;

    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_AUTH, "{hmac_tid_clear::pst_hmac_user is null.}");
        return;
    }
    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_tid_clear::pst_hmac_vap null.}");
        return;
    }
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_tid_clear::pst_device null.}");
        return;
    }

    for (uc_loop = 0; uc_loop < WLAN_TID_MAX_NUM; uc_loop++) {
        pst_amsdu = &(pst_hmac_user->ast_hmac_amsdu[uc_loop]);

        /* tid��, �����ж� */
        oal_spin_lock_bh(&pst_amsdu->st_amsdu_lock);

        if (pst_amsdu->st_amsdu_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&(pst_amsdu->st_amsdu_timer));
        }

        /* ��վۺ϶��� */
        if (pst_amsdu->uc_msdu_num != 0) {
            while (OAL_TRUE != oal_netbuf_list_empty(&pst_amsdu->st_msdu_head)) {
                pst_amsdu_net_buf = oal_netbuf_delist(&(pst_amsdu->st_msdu_head));

                oal_netbuf_free(pst_amsdu_net_buf);
            }
            pst_amsdu->uc_msdu_num = 0;
            pst_amsdu->us_amsdu_size = 0;
        }

        /* tid����, ʹ�����ж� */
        oal_spin_unlock_bh(&pst_amsdu->st_amsdu_lock);

        pst_tid = &(pst_hmac_user->ast_tid_info[uc_loop]);

        if (pst_tid->st_ba_tx_info.st_addba_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&(pst_tid->st_ba_tx_info.st_addba_timer));
        }

        pst_tid->uc_tid_no = (uint8_t)uc_loop;
        pst_tid->us_hmac_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;

        /* ������շ���Ự��� */
        if (pst_tid->pst_ba_rx_info != OAL_PTR_NULL) {
            hmac_ba_reset_rx_handle(pst_device, pst_hmac_user, uc_loop, OAL_TRUE);
        }

        if (pst_tid->st_ba_tx_info.en_ba_status != DMAC_BA_INIT) {
            /* ����TX BA�Ự�����Ҫ-- */
            hmac_tx_ba_session_decr(pst_hmac_vap, uc_loop);
        }

        /* ��ʼ��ba tx������� */
        pst_tid->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
        pst_tid->st_ba_tx_info.uc_addba_attemps = 0;
        pst_tid->st_ba_tx_info.uc_dialog_token = 0;
        pst_tid->st_ba_tx_info.uc_ba_policy = 0;
        pst_hmac_user->auc_ba_flag[uc_loop] = 0;
    }
}

OAL_STATIC uint16_t hmac_get_auth_rsp_timout(mac_vap_stru *pst_mac_vap)
{
    uint16_t us_auth_rsp_timeout;

    us_auth_rsp_timeout = ((g_st_mac_device_custom_cfg.us_cmd_auth_rsp_timeout != 0) ? \
            g_st_mac_device_custom_cfg.us_cmd_auth_rsp_timeout : \
                (uint16_t)mac_mib_get_AuthenticationResponseTimeOut(pst_mac_vap));
    return us_auth_rsp_timeout;
}


uint16_t hmac_encap_auth_rsp(mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_auth_rsp,
                                   oal_netbuf_stru *pst_auth_req, uint8_t *puc_chtxt, uint8_t uc_chtxt_len)
{
    uint16_t us_auth_rsp_len = 0;
    hmac_user_stru *pst_hmac_user_sta = OAL_PTR_NULL;
    uint8_t *puc_frame = OAL_PTR_NULL;
    uint16_t us_index;
    uint16_t us_auth_type;
    uint8_t uc_is_seq1;
    uint16_t us_auth_seq;
    uint8_t auc_addr2[6] = { 0 };
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint16_t us_user_index = 0xffff;
    uint32_t ul_ret;
    uint16_t us_auth_rsp_timeout;

    /* ��֤���� */
    hmac_ap_auth_process_code_enum_uint8 ul_auth_proc_rst;

    uint8_t *puc_data = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    hmac_auth_rsp_handle_stru st_auth_rsp_handle;
    uint32_t ul_alg_suppt;

    if (oal_any_null_ptr4(pst_mac_vap, pst_auth_rsp, pst_auth_req, puc_chtxt)) {
        oam_error_log4(0, OAM_SF_AUTH,
                       "{hmac_encap_auth_rsp::pst_mac_vap[0x%x], p_data[0x%x], p_auth_req[0x%x], p_chtxt[0x%x]}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_auth_rsp, (uintptr_t)pst_auth_req, (uintptr_t)puc_chtxt);
        return us_auth_rsp_len;
    }

    if (uc_chtxt_len > WLAN_CHTXT_SIZE) {
        return us_auth_rsp_len;
    }

    puc_data = (uint8_t *)oal_netbuf_header(pst_auth_rsp);
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_auth_rsp);

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
    /* ���ú���ͷ��frame control�ֶ� */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_AUTH);

    /* ��ȡSTA�ĵ�ַ */
    mac_get_address2(oal_netbuf_header(pst_auth_req), auc_addr2, sizeof(auc_addr2));

    /* ��DA����ΪSTA�ĵ�ַ */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1, auc_addr2);

    /* ��SA����Ϊdot11MacAddress */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_mac_vap->auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*              Authentication Frame - Frame Body                        */
    /* --------------------------------------------------------------------- */
    /* |Auth Algo Number|Auth Trans Seq Number|Status Code| Challenge Text | */
    /* --------------------------------------------------------------------- */
    /* | 2              |2                    |2          | 3 - 256        | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    us_index = MAC_80211_FRAME_LEN;
    puc_frame = (uint8_t *)(puc_data + us_index);

    /* ������֤��Ӧ֡�ĳ��� */
    us_auth_rsp_len = MAC_80211_FRAME_LEN + MAC_AUTH_ALG_LEN + MAC_AUTH_TRANS_SEQ_NUM_LEN +
                      MAC_STATUS_CODE_LEN;

    /* ������֤���� */
    us_auth_type = mac_get_auth_algo_num(pst_auth_req);

    /* ����auth transaction number */
    us_auth_seq = mac_get_auth_seq_num(oal_netbuf_header(pst_auth_req));
    if (us_auth_seq > HMAC_AP_AUTH_SEQ3_WEP_COMPLETE) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_encap_auth_rsp::auth recieve invalid seq, auth seq [%d]}",
                         us_auth_seq);
        return 0;
    }

    /* ������֤����IE */
    puc_frame[0] = (us_auth_type & 0x00FF);
    puc_frame[1] = (us_auth_type & 0xFF00) >> 8;

    /* ���յ���transaction number + 1���Ƹ��µ���֤��Ӧ֡ */
    puc_frame[2] = ((us_auth_seq + 1) & 0x00FF);
    puc_frame[3] = ((us_auth_seq + 1) & 0xFF00) >> 8;

    /* ״̬Ϊ��ʼ��Ϊ�ɹ� */
    puc_frame[4] = MAC_SUCCESSFUL_STATUSCODE;
    puc_frame[5] = 0;

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_CONN,
                     "{hmac_encap_auth_rsp:: AUTH_RSP tx : user mac:%02X:XX:XX:%02X:%02X:%02X}",
                     auc_addr2[0], auc_addr2[3], auc_addr2[4], auc_addr2[5]);

    if (mac_addr_is_zero(auc_addr2)) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_encap_auth_rsp::user mac:%02X:XX:XX:%02X:%02X:%02X is all 0 and invaild!}",
                         auc_addr2[0], auc_addr2[3], auc_addr2[4], auc_addr2[5]);
        puc_frame[4] = MAC_UNSPEC_FAIL;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;
        return us_auth_rsp_len;
    }
    /* ��������ܾ���auth,status code��37 */
    if (OAL_TRUE == hmac_blacklist_filter(pst_mac_vap, auc_addr2)) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_encap_auth_rsp::user mac:%02X:XX:XX:%02X:%02X:%02X is all 0 and invaild!}",
                         auc_addr2[0], auc_addr2[3], auc_addr2[4], auc_addr2[5]);
        puc_frame[4] = MAC_REQ_DECLINED;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;
        return us_auth_rsp_len;
    }
    /* ��ȡ�û�idx */
    uc_is_seq1 = (us_auth_seq == WLAN_AUTH_TRASACTION_NUM_ONE);
    ul_ret = hmac_encap_auth_rsp_get_user_idx(pst_mac_vap,
                                                  auc_addr2,
                                                  sizeof(auc_addr2),
                                                  uc_is_seq1,
                                                  &st_auth_rsp_handle.st_auth_rsp_param.uc_auth_resend,
                                                  &us_user_index);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_encap_auth_rsp::hmac_ap_get_user_idx fail[%d]! (1002:exceed config spec)}", ul_ret);

        puc_frame[4] = (ul_ret == OAL_ERR_CODE_CONFIG_EXCEED_SPEC) ? MAC_AP_FULL : MAC_UNSPEC_FAIL;

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;
        return us_auth_rsp_len;
    }

    /* ��ȡhmac userָ�� */
    pst_hmac_user_sta = mac_res_get_hmac_user(us_user_index);
    if (pst_hmac_user_sta == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                       "{hmac_encap_auth_rsp::pst_hmac_user_sta[%d] is NULL}",
                       us_user_index);
        puc_frame[4] = MAC_UNSPEC_FAIL;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;
        return us_auth_rsp_len;
    }

    /* ��ȡhmac vapָ�� */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                       "{hmac_encap_auth_rsp::pst_hmac_vap is NULL,and change user[idx==%d] state to BUTT!}",
                       pst_hmac_user_sta->st_user_base_info.us_assoc_id);
        puc_frame[4] = MAC_UNSPEC_FAIL;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;

        mac_user_set_asoc_state(&(pst_hmac_user_sta->st_user_base_info), MAC_USER_STATE_BUTT);
        return us_auth_rsp_len;
    }

    /* CB�ֶ�user idx���и�ֵ */
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_index;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_auth_rsp_len;

    /* �ж��㷨�Ƿ�֧�� */
    ul_alg_suppt = hmac_encap_auth_rsp_support(pst_hmac_vap, us_auth_type);
    if (ul_alg_suppt != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_encap_auth_rsp::auth type[%d] not support!}",
                         us_auth_type);
        puc_frame[4] = MAC_UNSUPT_ALG;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, puc_frame[4]);

        hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info),
                                     &pst_hmac_user_sta->st_user_base_info, MAC_USER_STATE_BUTT);
        return us_auth_rsp_len;
    }

    /*  ��ʼ��������� */
    st_auth_rsp_handle.st_auth_rsp_param.en_is_wep_allowed = mac_is_wep_allowed(pst_mac_vap);
    st_auth_rsp_handle.st_auth_rsp_param.en_user_asoc_state = pst_hmac_user_sta->st_user_base_info.en_user_asoc_state;
    st_auth_rsp_handle.st_auth_rsp_param.us_auth_type = us_auth_type;
    st_auth_rsp_handle.st_auth_rsp_fun = hmac_encap_auth_rsp_get_func(us_auth_seq);

    /*  ����seq1����seq3 */
    if (st_auth_rsp_handle.st_auth_rsp_fun != OAL_PTR_NULL) {
        ul_auth_proc_rst = st_auth_rsp_handle.st_auth_rsp_fun(pst_mac_vap,
                                                              &st_auth_rsp_handle.st_auth_rsp_param,
                                                              &puc_frame[4],
                                                              &pst_hmac_user_sta->st_user_base_info.en_user_asoc_state);
        /* ��� HMAC��TID��Ϣ */
        hmac_tid_clear(pst_mac_vap, pst_hmac_user_sta);
    } else {
        ul_auth_proc_rst = HMAC_AP_AUTH_BUTT;

        mac_user_set_asoc_state(&pst_hmac_user_sta->st_user_base_info, MAC_USER_STATE_BUTT);
        puc_frame[4] = MAC_AUTH_SEQ_FAIL;
    }

    us_auth_rsp_timeout = hmac_get_auth_rsp_timout(pst_mac_vap);

    oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                     "{hmac_encap_auth_rsp::ul_auth_proc_rst:%d,us_auth_rsp_timeout:%d}",
                     ul_auth_proc_rst, us_auth_rsp_timeout);

    /*  ���ݷ��ص�code���к������� */
    switch (ul_auth_proc_rst) {
        case HMAC_AP_AUTH_SEQ1_OPEN_ANY: {
            mac_user_init_key(&pst_hmac_user_sta->st_user_base_info);
            frw_timer_create_timer_m(&pst_hmac_user_sta->st_mgmt_timer,
                                   hmac_mgmt_timeout_ap,
                                   us_auth_rsp_timeout,
                                   pst_hmac_user_sta,
                                   OAL_FALSE,
                                   OAM_MODULE_ID_HMAC,
                                   pst_mac_vap->ul_core_id);
            break;
        }
        case HMAC_AP_AUTH_SEQ1_WEP_NOT_RESEND: {
            hmac_config_11i_add_wep_entry(pst_mac_vap, 6, pst_hmac_user_sta->st_user_base_info.auc_user_mac_addr);

            hmac_mgmt_encap_chtxt(puc_frame, puc_chtxt, &us_auth_rsp_len, pst_hmac_user_sta);
            /* Ϊ���û�����һ����ʱ������ʱ��֤ʧ�� */
            frw_timer_create_timer_m(&pst_hmac_user_sta->st_mgmt_timer,
                                   hmac_mgmt_timeout_ap,
                                   us_auth_rsp_timeout,
                                   pst_hmac_user_sta,
                                   OAL_FALSE,
                                   OAM_MODULE_ID_HMAC,
                                   pst_mac_vap->ul_core_id);
            pst_hmac_user_sta->st_user_base_info.st_key_info.en_cipher_type =
                mac_get_wep_type(pst_mac_vap, mac_get_wep_default_keyid(pst_mac_vap));
            break;
        }
        case HMAC_AP_AUTH_SEQ1_WEP_RESEND: {
            /* seqΪ1 ����֤֡�ش� */
            hmac_mgmt_encap_chtxt(puc_frame, puc_chtxt, &us_auth_rsp_len, pst_hmac_user_sta);

            if (pst_hmac_vap->st_mgmt_timer.en_is_registerd == OAL_TRUE) {
                /* ������ʱ��ʱ�� */
                frw_timer_restart_timer(&pst_hmac_user_sta->st_mgmt_timer,
                                            pst_hmac_user_sta->st_mgmt_timer.ul_timeout,
                                            OAL_FALSE);
            }
            break;
        }
        case HMAC_AP_AUTH_SEQ3_OPEN_ANY: {
            mac_user_init_key(&pst_hmac_user_sta->st_user_base_info);
            break;
        }
        case HMAC_AP_AUTH_SEQ3_WEP_COMPLETE: {
            puc_chtxt = mac_get_auth_ch_text(oal_netbuf_header(pst_auth_req));
            if (hmac_mgmt_is_challenge_txt_equal(puc_chtxt,
                                                 pst_hmac_user_sta->auc_ch_text) == OAL_TRUE) {

                mac_user_set_asoc_state(&pst_hmac_user_sta->st_user_base_info, MAC_USER_STATE_AUTH_COMPLETE);

                /* cancel timer for auth */
                frw_timer_immediate_destroy_timer_m(&pst_hmac_user_sta->st_mgmt_timer);
            } else {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                                 "{hmac_encap_auth_rsp::challenage txt not equal.}");
                puc_frame[4] = MAC_CHLNG_FAIL;

                mac_user_set_asoc_state(&pst_hmac_user_sta->st_user_base_info, MAC_USER_STATE_BUTT);
            }
            break;
        }
        case HMAC_AP_AUTH_SEQ3_WEP_ASSOC: {
            hmac_mgmt_encap_chtxt(puc_frame, puc_chtxt, &us_auth_rsp_len, pst_hmac_user_sta);

            /* ������ʱ��ʱ�� */
            frw_timer_create_timer_m(&pst_hmac_user_sta->st_mgmt_timer,
                                   hmac_mgmt_timeout_ap,
                                   us_auth_rsp_timeout,
                                   pst_hmac_user_sta,
                                   OAL_FALSE,
                                   OAM_MODULE_ID_HMAC,
                                   pst_mac_vap->ul_core_id);
            break;
        }
        case HMAC_AP_AUTH_DUMMY: {
            break;
        }
        case HMAC_AP_AUTH_BUTT:
        default:
        {
            mac_user_init_key(&pst_hmac_user_sta->st_user_base_info);
            pst_hmac_user_sta->st_user_base_info.en_user_asoc_state = MAC_USER_STATE_BUTT;
            break;
        }
    }

    /* dmac offload�ܹ��£�ͬ��user����״̬��Ϣ��dmac */
    ul_ret = hmac_config_user_asoc_state_syn(&(pst_hmac_vap->st_vap_base_info),
                                                 &pst_hmac_user_sta->st_user_base_info);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_ap_rx_auth_req::hmac_config_user_asoc_state_syn failed[%d].}", ul_ret);
    }
    return us_auth_rsp_len;
}
