

#include "mac_ie.h"
#include "mac_frame.h"
#include "mac_device.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_IE_C
#define IE_BUFFER_HEADER 3

/* srp ie �����С���� */
#define MAC_HE_SRP_IE_MIN_LEN 2
#define MAC_HE_SRP_IE_MAX_LEN 21

oal_bool_enum_uint8 mac_ie_proc_ht_supported_channel_width(mac_user_stru *pst_mac_user_sta, mac_vap_stru *pst_mac_vap,
    uint8_t uc_supported_channel_width, oal_bool_enum en_prev_asoc_ht)
{
    /* ��֧��20/40MhzƵ�� */
    if (uc_supported_channel_width == 0) {
        if ((en_prev_asoc_ht == OAL_FALSE) || (pst_mac_user_sta->st_ht_hdl.bit_supported_channel_width == OAL_TRUE)) {
            pst_mac_vap->st_protection.uc_sta_20M_only_num++;
        }

        return OAL_FALSE;
    } else { /* ֧��20/40MhzƵ�� */
        /*  ���STA֮ǰ�Ѿ���Ϊ��֧��20/40MhzƵ���HTվ����AP���� */
        if ((en_prev_asoc_ht == OAL_TRUE) && (pst_mac_user_sta->st_ht_hdl.bit_supported_channel_width == OAL_FALSE)) {
            pst_mac_vap->st_protection.uc_sta_20M_only_num--;
        }

        return OAL_TRUE;
    }
}


oal_bool_enum_uint8 mac_ie_proc_ht_green_field(mac_user_stru *pst_mac_user_sta, mac_vap_stru *pst_mac_vap,
    uint8_t uc_ht_green_field, oal_bool_enum en_prev_asoc_ht)
{
    /* ��֧��Greenfield */
    if (uc_ht_green_field == 0) {
        if ((en_prev_asoc_ht == OAL_FALSE) || (pst_mac_user_sta->st_ht_hdl.bit_ht_green_field == OAL_TRUE)) {
            pst_mac_vap->st_protection.uc_sta_non_gf_num++;
        }

        return OAL_FALSE;
    } else { /* ֧��Greenfield */
        /*  ���STA֮ǰ�Ѿ���Ϊ��֧��GF��HTվ����AP���� */
        if ((en_prev_asoc_ht == OAL_TRUE) && (pst_mac_user_sta->st_ht_hdl.bit_ht_green_field == OAL_FALSE)) {
            pst_mac_vap->st_protection.uc_sta_non_gf_num--;
        }

        return OAL_TRUE;
    }
}


oal_bool_enum_uint8 mac_ie_proc_lsig_txop_protection_support(mac_user_stru *pst_mac_user_sta,
    mac_vap_stru *pst_mac_vap, uint8_t uc_lsig_txop_protection_support, oal_bool_enum en_prev_asoc_ht)
{
    /* ��֧��L-sig txop protection */
    if (uc_lsig_txop_protection_support == 0) {
        if ((en_prev_asoc_ht == OAL_FALSE) || (pst_mac_user_sta->st_ht_hdl.bit_lsig_txop_protection == OAL_TRUE)) {
            pst_mac_vap->st_protection.uc_sta_no_lsig_txop_num++;
        }

        return OAL_FALSE;
    } else { /* ֧��L-sig txop protection */
        /*  ���STA֮ǰ�Ѿ���Ϊ��֧��Lsig txop protection��HTվ����AP���� */
        if ((en_prev_asoc_ht == OAL_TRUE) && (pst_mac_user_sta->st_ht_hdl.bit_lsig_txop_protection == OAL_FALSE)) {
            pst_mac_vap->st_protection.uc_sta_no_lsig_txop_num--;
        }

        return OAL_TRUE;
    }
}

OAL_STATIC void mac_ie_proc_ht_sta_ht_info(mac_user_stru *p_mac_user, mac_user_ht_hdl_stru *p_ht_hdl,
    uint16_t ht_cap_info)
{
    uint8_t smps;

    /* ���STA��֧�ֵ�LDPC�������� B0��0:��֧�֣�1:֧�� */
    p_ht_hdl->bit_ldpc_coding_cap = (ht_cap_info & BIT0);

    /* ��ȡAP��֧�ֵĴ�������  */
    p_ht_hdl->bit_supported_channel_width = ((ht_cap_info & BIT1) >> 1); /* 1 offset */

    /* ���ռ临�ý���ģʽ B2~B3 */
    smps = ((ht_cap_info & (BIT3 | BIT2)) >> 2); /* 2 offset */
    p_ht_hdl->bit_sm_power_save = mac_ie_proc_sm_power_save_field(p_mac_user, smps);

    /* ��ȡAP֧��Greenfield��� */
    p_ht_hdl->bit_ht_green_field = ((ht_cap_info & BIT4) >> 4); /* 4 offset */

    /* ��ȡAP֧��20MHz Short-GI��� */
    p_ht_hdl->bit_short_gi_20mhz = ((ht_cap_info & BIT5) >> 5); /* 5 offset */

    /* ��ȡAP֧��40MHz Short-GI��� */
    p_ht_hdl->bit_short_gi_40mhz = ((ht_cap_info & BIT6) >> 6); /* 6 offset */

    /* ��ȡAP֧��STBC PPDU��� */
    p_ht_hdl->bit_rx_stbc = (uint8_t)((ht_cap_info & (BIT9 | BIT8)) >> 8); /* 8 offset */

    /* ��ȡAP 40M��DSSS/CCK��֧����� */
    p_ht_hdl->bit_dsss_cck_mode_40mhz = ((ht_cap_info & BIT12) >> 12); /* 12 offset */

    /* ��ȡAP L-SIG TXOP ������֧����� */
    p_ht_hdl->bit_lsig_txop_protection = ((ht_cap_info & BIT15) >> 15); /* 15 offset */
}

OAL_STATIC void mac_ie_proc_ht_sta_txbf(mac_user_ht_hdl_stru *p_ht_hdl, uint32_t txbf_elem)
{
    p_ht_hdl->bit_imbf_receive_cap = (txbf_elem & BIT0);
    p_ht_hdl->bit_receive_staggered_sounding_cap = ((txbf_elem & BIT1) >> 1); /* 1 offset */
    p_ht_hdl->bit_transmit_staggered_sounding_cap = ((txbf_elem & BIT2) >> 2); /* 2 offset */
    p_ht_hdl->bit_receive_ndp_cap = ((txbf_elem & BIT3) >> 3); /* 3 offset */
    p_ht_hdl->bit_transmit_ndp_cap = ((txbf_elem & BIT4) >> 4); /* 4 offset */
    p_ht_hdl->bit_imbf_cap = ((txbf_elem & BIT5) >> 5); /* 5 offset */
    p_ht_hdl->bit_calibration = ((txbf_elem & 0x000000C0) >> 6); /* 6 offset */
    p_ht_hdl->bit_exp_csi_txbf_cap = ((txbf_elem & BIT8) >> 8); /* 8 offset */
    p_ht_hdl->bit_exp_noncomp_txbf_cap = ((txbf_elem & BIT9) >> 9); /* 9 offset */
    p_ht_hdl->bit_exp_comp_txbf_cap = ((txbf_elem & BIT10) >> 10); /* 10 offset */
    p_ht_hdl->bit_exp_csi_feedback = ((txbf_elem & 0x00001800) >> 11); /* 11 offset */
    p_ht_hdl->bit_exp_noncomp_feedback = ((txbf_elem & 0x00006000) >> 13); /* 13 offset */
    p_ht_hdl->bit_exp_comp_feedback = ((txbf_elem & 0x0001C000) >> 15); /* 15 offset */
    p_ht_hdl->bit_min_grouping = ((txbf_elem & 0x00060000) >> 17); /* 17 offset */
    p_ht_hdl->bit_csi_bfer_ant_number = ((txbf_elem & 0x001C0000) >> 19); /* 19 offset */
    p_ht_hdl->bit_noncomp_bfer_ant_number = ((txbf_elem & 0x00600000) >> 21); /* 21 offset */
    p_ht_hdl->bit_comp_bfer_ant_number = ((txbf_elem & 0x01C00000) >> 23); /* 23 offset */
    p_ht_hdl->bit_csi_bfee_max_rows = ((txbf_elem & 0x06000000) >> 25); /* 25 offset */
    p_ht_hdl->bit_channel_est_cap = ((txbf_elem & 0x18000000) >> 27); /* 27 offset */
}


uint32_t mac_ie_proc_ht_sta(mac_vap_stru *pst_mac_sta, uint8_t *puc_payload, uint16_t us_offset,
    mac_user_stru *pst_mac_user_ap, uint16_t *pus_amsdu_maxsize)
{
    uint8_t uc_mcs_bmp_index;
    mac_user_ht_hdl_stru *pst_ht_hdl = NULL;
    mac_user_ht_hdl_stru st_ht_hdl;
    uint16_t us_tmp_info_elem;
    uint16_t us_tmp_txbf_low;
    uint32_t ul_tmp_txbf_elem;
    uint16_t us_ht_cap_info;

    if ((pst_mac_sta == NULL) || (puc_payload == NULL) || (pst_mac_user_ap == NULL) ||
        (pus_amsdu_maxsize == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_ie_proc_ht_sta::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_ht_hdl = &st_ht_hdl;
    mac_user_get_ht_hdl(pst_mac_user_ap, pst_ht_hdl);

    /* ���� HT Capability Element �� AP����ʾ������HT capable. */
    pst_ht_hdl->en_ht_capable = OAL_TRUE;

    us_offset += MAC_IE_HDR_LEN;

    /* ���� HT Capabilities Info Field */
    us_ht_cap_info = oal_make_word16(puc_payload[us_offset], puc_payload[us_offset + 1]);

    mac_ie_proc_ht_sta_ht_info(pst_mac_user_ap, pst_ht_hdl, us_ht_cap_info);
    us_offset += MAC_HT_CAPINFO_LEN;

    /* ��ȡAP֧�����A-MSDU������� */
    if ((us_ht_cap_info & BIT11) == 0) {
        *pus_amsdu_maxsize = WLAN_MIB_MAX_AMSDU_LENGTH_SHORT;
    } else {
        *pus_amsdu_maxsize = WLAN_MIB_MAX_AMSDU_LENGTH_LONG;
    }

    /* ���� A-MPDU Parameters Field */
    /* ��ȡ Maximum Rx A-MPDU factor (B1 - B0) */
    pst_ht_hdl->uc_max_rx_ampdu_factor = (puc_payload[us_offset] & 0x03);

    /* ��ȡ Minmum Rx A-MPDU factor (B3 - B2) */
    pst_ht_hdl->uc_min_mpdu_start_spacing = (puc_payload[us_offset] >> 2) & 0x07;

    us_offset += MAC_HT_AMPDU_PARAMS_LEN;

    /* ���� Supported MCS Set Field */
    for (uc_mcs_bmp_index = 0; uc_mcs_bmp_index < WLAN_HT_MCS_BITMASK_LEN; uc_mcs_bmp_index++) {
        pst_ht_hdl->uc_rx_mcs_bitmask[uc_mcs_bmp_index] = (*(uint8_t*)(puc_payload + us_offset + uc_mcs_bmp_index));
    }
    pst_ht_hdl->uc_rx_mcs_bitmask[WLAN_HT_MCS_BITMASK_LEN - 1] &= 0x1F;
    us_offset += MAC_HT_SUP_MCS_SET_LEN;

    /* ���� HT Extended Capabilities Info Field */
    us_ht_cap_info = oal_make_word16(puc_payload[us_offset], puc_payload[us_offset + BIT_OFFSET_1]);
    /* ��ȡ HTC support Information */
    if ((us_ht_cap_info & BIT10) != 0) {
        pst_ht_hdl->uc_htc_support = 1;
    }

    us_offset += MAC_HT_EXT_CAP_LEN;

    /* ���� Tx Beamforming Field */
    us_tmp_info_elem = oal_make_word16(puc_payload[us_offset], puc_payload[us_offset + BIT_OFFSET_1]);
    us_tmp_txbf_low = oal_make_word16(puc_payload[us_offset + BIT_OFFSET_2], puc_payload[us_offset + BIT_OFFSET_3]);
    ul_tmp_txbf_elem = oal_make_word32(us_tmp_info_elem, us_tmp_txbf_low);
    mac_ie_proc_ht_sta_txbf(pst_ht_hdl, ul_tmp_txbf_elem);
    mac_user_set_ht_hdl(pst_mac_user_ap, pst_ht_hdl);

    return OAL_SUCC;
}


oal_bool_enum_uint8 mac_ie_check_p2p_action(uint8_t *puc_payload)
{
    /* �ҵ�WFA OUI */
    if ((oal_memcmp(puc_payload, g_auc_p2p_oui, MAC_OUI_LEN) == 0) && (puc_payload[MAC_OUI_LEN] == MAC_OUITYPE_P2P)) {
        /*  �ҵ�WFA P2P v1.0 oui type */
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


wlan_mib_mimo_power_save_enum_uint8 mac_ie_proc_sm_power_save_field(mac_user_stru *pst_mac_user, uint8_t uc_smps)
{
    if (uc_smps == MAC_SMPS_STATIC_MODE) {
        return WLAN_MIB_MIMO_POWER_SAVE_STATIC;
    } else if (uc_smps == MAC_SMPS_DYNAMIC_MODE) {
        return WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC;
    } else {
        return WLAN_MIB_MIMO_POWER_SAVE_MIMO;
    }
}

#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t mac_ie_from_he_cap_get_nss(mac_he_hdl_stru *mac_he_hdl, wlan_nss_enum_uint8 *nss)
{
    if (mac_he_hdl->st_he_cap_ie.st_he_mcs_nss.st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_4ss !=
        MAC_MAX_SUP_INVALID_11AX_EACH_NSS) {
        *nss = WLAN_FOUR_NSS;
    } else if (mac_he_hdl->st_he_cap_ie.st_he_mcs_nss.st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_3ss !=
        MAC_MAX_SUP_INVALID_11AX_EACH_NSS) {
        *nss = WLAN_TRIPLE_NSS;
    } else if (mac_he_hdl->st_he_cap_ie.st_he_mcs_nss.st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_2ss !=
        MAC_MAX_SUP_INVALID_11AX_EACH_NSS) {
        *nss = WLAN_DOUBLE_NSS;
    } else if (mac_he_hdl->st_he_cap_ie.st_he_mcs_nss.st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_1ss !=
        MAC_MAX_SUP_INVALID_11AX_EACH_NSS) {
        *nss = WLAN_SINGLE_NSS;
    } else {
        *nss = WLAN_SINGLE_NSS;
        oam_warning_log0(0, OAM_SF_ANY, "{mac_ie_from_he_cap_get_nss::invalid he nss.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


uint32_t mac_ie_parse_he_cap(uint8_t *he_cap_ie, mac_frame_he_cap_ie_stru *he_cap_value)
{
    uint8_t *he_buffer = NULL;
    mac_frame_he_mac_cap_stru *he_mac_cap = NULL;
    mac_frame_he_phy_cap_stru *pst_he_phy_cap = NULL;
    uint8_t uc_mcs_nss_set_size = 2;
    mac_fram_he_mac_nsss_set_stru *pst_mac_nss_set = NULL;
    int32_t l_ret;

    /* ����he cap IE */
    if (oal_any_null_ptr2(he_cap_ie, he_cap_value)) {
        oam_error_log2(0, OAM_SF_11AX,
                       "{mac_ie_parse_he_cap::param null,he_cap_ie[0x%x], he_cap_value[0x%x].}",
                       (uintptr_t)he_cap_ie, (uintptr_t)he_cap_value);

        return OAL_ERR_CODE_PTR_NULL;
    }

    /*
     * -------------------------------------------------------------------------
     * |EID |Length |EID Extension|HE MAC Capa. Info |HE PHY Capa. Info|
     * -------------------------------------------------------------------------
     * |1   |1      |1            |6                 |9                |
     * -------------------------------------------------------------------------
     * |Tx Rx HE MCS NSS Support |PPE Thresholds(Optional)|
     * -------------------------------------------------------------------------
     * |4or8or12                 |Variable                |
     * -------------------------------------------------------------------------
     */
    if (he_cap_ie[1] < MAC_HE_CAP_MIN_LEN) {
        oam_warning_log1(0, OAM_SF_11AX, "{hmac_proc_he_cap_ie::invalid he cap ie len[%d].}", he_cap_ie[1]);
        return OAL_FAIL;
    }

    he_buffer = he_cap_ie + IE_BUFFER_HEADER;

    /* mac cap */
    he_mac_cap = (mac_frame_he_mac_cap_stru*)he_buffer;
    l_ret = memcpy_s(&he_cap_value->st_he_mac_cap, OAL_SIZEOF(mac_frame_he_mac_cap_stru), he_mac_cap,
        OAL_SIZEOF(mac_frame_he_mac_cap_stru));
    he_buffer += OAL_SIZEOF(mac_frame_he_mac_cap_stru);

    /* PHY Cap */
    pst_he_phy_cap = (mac_frame_he_phy_cap_stru*)he_buffer;
    l_ret += memcpy_s(&he_cap_value->st_he_phy_cap, OAL_SIZEOF(mac_frame_he_phy_cap_stru), pst_he_phy_cap,
        OAL_SIZEOF(mac_frame_he_phy_cap_stru));
    he_buffer += OAL_SIZEOF(mac_frame_he_phy_cap_stru);

    /* ����Support HE-MCS NSS Set */
    /*
     * |-------------------------------------------------------------------------------------------------|
     * | Rx HE-MCS Map | Tx HE-MCS Map | Rx HE-MCS Map  | Tx HE-MCS Map  | Rx HE-MCS Map | Tx HE-MCS Map |
     * | <= 80 MHz     | <= 80 MHz     | 160 MHz        | 160 MHz        | 80+80 MHz     | 80+80 MHz     |
     * |-------------------------------------------------------------------------------------------------|
     * | 2 Octets      | 2 Octets      | 0 or 2 Octets  | 0 or 2 Octets  | 0 or 2 Octets | 0 or 2 Octets |
     * |-------------------------------------------------------------------------------------------------|
     */
    /*
     * 1. HE PHY Capabilities Info��Channel Width Set�ֶ�bit2Ϊ1ʱ��
     * HE Supported HE-MCS And NSS Set�д���Rx HE-MCS Map 160 MHz��Tx HE-MCS Map 160 MHz�ֶ�
     * 2. HE PHY Capabilities Info��Channel Width Set�ֶ�bit3Ϊ1ʱ��
     * HE Supported HE-MCS And NSS Set�д���Rx HE-MCS Map 80+80 MHz��Tx HE-MCS Map 80+80 MHz�ֶ�
     */
    pst_mac_nss_set = (mac_fram_he_mac_nsss_set_stru*)he_buffer;
    if ((he_cap_value->st_he_phy_cap.bit_channel_width_set & BIT2) != 0) {
        uc_mcs_nss_set_size += 2;  /* 2��ʾRx HE-MCS Map 160 MHz��Tx HE-MCS Map 160 MHz�ֶ� */
    }
    if ((he_cap_value->st_he_phy_cap.bit_channel_width_set & BIT3) != 0) {
        uc_mcs_nss_set_size += 2;  /* 2��ʾRx HE-MCS Map 80+80 MHz��Tx HE-MCS Map 80+80 MHz�ֶ� */
    }

    /* APֻ���¼�Զ˷�������ֵ */
    l_ret += memcpy_s((uint8_t*)(&he_cap_value->st_he_mcs_nss), sizeof(mac_fram_he_mac_nsss_set_stru),
        (uint8_t*)pst_mac_nss_set, uc_mcs_nss_set_size * sizeof(mac_frame_he_mcs_nss_bit_map_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_11AX, "mac_ie_parse_he_cap::memcpy fail!");
        return OAL_FAIL;
    }

    /* PPE thresholds �ݲ����� */
    return OAL_SUCC;
}


/*lint -save -e438 */
uint32_t mac_ie_parse_he_oper(uint8_t *puc_he_oper_ie, mac_frame_he_oper_ie_stru *pst_he_oper_ie_value)
{
    uint8_t *puc_ie_buffer = NULL;
    mac_frame_he_operation_param_stru *pst_he_oper_param = NULL;
    mac_frame_he_mcs_nss_bit_map_stru *pst_he_basic_mcs_nss = NULL;
    mac_frame_vht_operation_info_stru *pst_vht_operation_info = NULL;
    mac_frame_he_bss_color_info_stru  *pst_bss_color_info = NULL;
    int32_t l_ret;

    if (oal_unlikely(oal_any_null_ptr2(puc_he_oper_ie, pst_he_oper_ie_value))) {
        oam_error_log0(0, OAM_SF_11AX, "{mac_ie_parse_he_oper::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /*
     * -------------------------------------------------------------------------
     * |EID |Length |EID Extension|HE Operation Parameters|Basic HE MCS Ans NSS Set|
     * -------------------------------------------------------------------------
     * |1   |1          |1                  |           4                     |              3                       |
     * -------------------------------------------------------------------------
     * |VHT Operation Info  |MaxBssid Indicator|
     * -------------------------------------------------------------------------
     * |      0 or 3              |0 or More             |
     * -------------------------------------------------------------------------
     */
    if (puc_he_oper_ie[1] < MAC_HE_OPERAION_MIN_LEN) {
        oam_warning_log1(0, OAM_SF_11AX, "{mac_ie_parse_he_oper::invalid he oper ie len[%d].}", puc_he_oper_ie[1]);
        return OAL_FAIL;
    }

    puc_ie_buffer = puc_he_oper_ie + IE_BUFFER_HEADER;

    /* ����HE Operation Parameters */
    pst_he_oper_param = (mac_frame_he_operation_param_stru*)puc_ie_buffer;
    puc_ie_buffer += MAC_HE_OPE_PARAM_LEN;
    l_ret = memcpy_s(&pst_he_oper_ie_value->st_he_oper_param, sizeof(mac_frame_he_operation_param_stru),
        pst_he_oper_param, MAC_HE_OPE_PARAM_LEN);

    pst_bss_color_info = (mac_frame_he_bss_color_info_stru*)puc_ie_buffer;
    puc_ie_buffer += OAL_SIZEOF(mac_frame_he_bss_color_info_stru);
    l_ret += memcpy_s(&pst_he_oper_ie_value->st_bss_color, OAL_SIZEOF(mac_frame_he_bss_color_info_stru),
        pst_bss_color_info, OAL_SIZEOF(mac_frame_he_bss_color_info_stru));

    /* ����Basic HE MCS And NSS Set */
    pst_he_basic_mcs_nss = (mac_frame_he_mcs_nss_bit_map_stru*)puc_ie_buffer;
    puc_ie_buffer += MAC_HE_OPE_BASIC_MCS_NSS_LEN;
    l_ret += memcpy_s((uint8_t *)(&pst_he_oper_ie_value->st_he_basic_mcs_nss),
                      OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru), pst_he_basic_mcs_nss,
                      OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru));

    if ((puc_he_oper_ie[1] >= (MAC_HE_OPERAION_MIN_LEN + OAL_SIZEOF(mac_frame_vht_operation_info_stru))) &&
        (pst_he_oper_ie_value->st_he_oper_param.bit_vht_operation_info_present == 1)) {
        pst_vht_operation_info = (mac_frame_vht_operation_info_stru*)puc_ie_buffer;
        puc_ie_buffer += MAC_HE_VHT_OPERATION_INFO_LEN;
        l_ret += memcpy_s((uint8_t*)(&pst_he_oper_ie_value->st_vht_operation_info),
            OAL_SIZEOF(mac_frame_vht_operation_info_stru), pst_vht_operation_info,
            OAL_SIZEOF(mac_frame_vht_operation_info_stru));
    } else {
        pst_he_oper_ie_value->st_he_oper_param.bit_vht_operation_info_present = OAL_FALSE;
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_11AX, "mac_ie_parse_he_oper::memcpy fail!");
        return OAL_FAIL;
    }

    /* MaxBssid Indicator */
    return OAL_SUCC;
}


oal_bool_enum_uint8 mac_proc_he_uora_parameter_set_ie(mac_vap_stru *mac_vap, uint8_t *payload, uint16_t msg_len)
{
    uint8_t *uora_ie = NULL;
    mac_he_uora_para_stru *vap_he_uora_para = NULL;
    mac_he_uora_para_stru he_uora_para = { 0 };

    if (oal_any_null_ptr2(mac_vap, payload)) {
        oam_error_log2(0, OAM_SF_11AX, "{mac_proc_he_uora_parameter_set_ie::param null, %X %X.}", (uintptr_t)mac_vap,
            (uintptr_t)payload);
        return OAL_FALSE;
    }

    /* ��֧��UORA��ֱ�ӷ��أ�֧��UORA���Ž��к����Ĵ��� */
    if (mac_mib_get_he_OFDMARandomAccess(mac_vap) == OAL_FALSE) {
        return OAL_FALSE;
    }

    /*
     * -------------------------------------------------------------------------
     * |EID |Length |EID Extension|OCW Range |
     * -------------------------------------------------------------------------
     * | 1  |   1   |     1       |     1    |
     * -------------------------------------------------------------------------
     * |255 | xx    |      37     |    xxx   |
     * -------------------------------------------------------------------------
     * |          UORA Parameter Set         |
     * -------------------------------------------------------------------------
     */
    uora_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_UORA_PARAMETER_SET, payload, msg_len);
    if ((uora_ie == NULL) || (uora_ie[1] < MAC_HE_UORA_PARAMETER_SET_LEN)) {
        return OAL_FALSE;
    }

    /*
     * -------------------------------------------------------------------------
     * |B0    B2 |B3    B5 |B6     B7 |
     * -------------------------------------------------------------------------
     * | EOCWmin | EOCWmax | Reserved |
     * -------------------------------------------------------------------------
     * |    3    |    3    |    2     |
     * -------------------------------------------------------------------------
     * |           OCW Range          |
     */
    he_uora_para.bit_uora_eocw_min = uora_ie[3] & 0x07; /* OCW Range: uora ie 3 */
    he_uora_para.bit_uora_eocw_max = (uora_ie[3] & 0x38) >> 3; /* OCW Range: uora ie 3 */

    vap_he_uora_para = (mac_he_uora_para_stru*)&(mac_vap->st_he_uora_eocw);

    /* When OCW range changed, update dmac and register */
    if (he_uora_para.bit_uora_eocw_min != vap_he_uora_para->bit_uora_eocw_min ||
        he_uora_para.bit_uora_eocw_max != vap_he_uora_para->bit_uora_eocw_max) {
        oam_warning_log4(0, OAM_SF_11AX,
            "{mac_proc_he_uora_parameter_set_ie::uora ie para changed, "
            "uora_eocw_min %d uora_eocw_max=%d, orginal vap_uora_min=%d vap_uora_max=%d}",
            he_uora_para.bit_uora_eocw_min, he_uora_para.bit_uora_eocw_max, vap_he_uora_para->bit_uora_eocw_min,
            vap_he_uora_para->bit_uora_eocw_max);

        vap_he_uora_para->bit_uora_eocw_min = he_uora_para.bit_uora_eocw_min;
        vap_he_uora_para->bit_uora_eocw_max = he_uora_para.bit_uora_eocw_max;
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

/*lint -restore */

uint32_t mac_ie_parse_mu_edca_parameter(uint8_t *puc_he_edca_ie,
    mac_frame_he_mu_edca_parameter_ie_stru *pst_he_mu_edca_value)
{
    mac_frame_he_mu_edca_parameter_ie_stru *pst_he_edca = NULL;

    if (oal_unlikely(oal_any_null_ptr2(puc_he_edca_ie, pst_he_mu_edca_value))) {
        oam_error_log0(0, OAM_SF_11AX, "{mac_ie_parse_mu_edca_parameter::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* MU EDCA Parameter Set Element */
    /*
     * -------------------------------------------------------------------------------------------
     * | EID | LEN | Ext EID|MU Qos Info |MU AC_BE Parameter Record | MU AC_BK Parameter Record  |
     * -------------------------------------------------------------------------------------------
     * |  1  |  1  |   1    |    1       |     3                    |        3                   |
     * -------------------------------------------------------------------------------------------
     * ------------------------------------------------------------------------------ -------------
     * | MU AC_VI Parameter Record | MU AC_VO Parameter Record                                   |
     * -------------------------------------------------------------------------------------------
     * |    3                      |     3                                                       |
     */
    /* QoS Info field when sent from WMM AP */
    /*
     * --------------------------------------------------------------------------------------------
     *    | EDCA Parameter Set Update Count | Q-Ack | Queue Request |TXOP Request | More Data Ack|
     * ---------------------------------------------------------------------------------------------
     * bit |        0~3                      |  1    |  1            |   1         |     1        |
     * ---------------------------------------------------------------------------------------------
     */
    if (puc_he_edca_ie[1] != MAC_HE_MU_EDCA_PARAMETER_SET_LEN) {
        oam_warning_log1(0, OAM_SF_11AX, "{mac_ie_parse_mu_edca_parameter::invalid mu edca ie len[%d].}",
            puc_he_edca_ie[1]);
        return OAL_FAIL;
    }

    puc_he_edca_ie = puc_he_edca_ie + IE_BUFFER_HEADER;

    /* ����HE MU EDCA  Parameters Set Element */
    pst_he_edca = (mac_frame_he_mu_edca_parameter_ie_stru*)puc_he_edca_ie;
    if (EOK != memcpy_s((uint8_t*)(pst_he_mu_edca_value), OAL_SIZEOF(mac_frame_he_mu_edca_parameter_ie_stru),
        pst_he_edca, OAL_SIZEOF(mac_frame_he_mu_edca_parameter_ie_stru))) {
        oam_error_log0(0, OAM_SF_11AX, "mac_ie_parse_mu_edca_parameter::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


/*lint -save -e438 */
uint32_t mac_ie_parse_spatial_reuse_parameter(uint8_t *puc_he_srp_ie,
    mac_frame_he_spatial_reuse_parameter_set_ie_stru *pst_he_srp_value)
{
    uint8_t *he_buffer = NULL;
    mac_frame_he_sr_control_stru *pst_he_sr_control = NULL;
    int32_t l_ret;

    if (oal_any_null_ptr2(puc_he_srp_ie, pst_he_srp_value)) {
        oam_error_log2(0, OAM_SF_11AX,
            "{mac_ie_parse_spatial_reuse_parameter::param null,puc_he_srp_ie[0x%x], pst_he_srp_value[0x%x].}",
            (uintptr_t)puc_he_srp_ie, (uintptr_t)pst_he_srp_value);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((puc_he_srp_ie[1] < MAC_HE_SRP_IE_MIN_LEN) || (puc_he_srp_ie[1] > MAC_HE_SRP_IE_MAX_LEN)) {
        oam_error_log0(0, OAM_SF_11AX, "mac_ie_parse_spatial_reuse_parameter::IE len invalid!");
        return OAL_FAIL;
    }
    /* Spatial Reuse Parameter Set Element */
    /*
     * ------------------------------------------------------------------------------
     * | EID | LEN | Ext EID|SR Control |Non-SRG OBSS PD Max Offset | SRG OBSS PD Min Offset  |
     * ------------------------------------------------------------------------------
     * |  1   |  1   |   1       |    1          |     0 or 1              | 0 or 1 |
     * ------------------------------------------------------------------------------
     * -----------------------------------------------------------------------------/
     * |SRG OBSS PD Max Offset |SRG BSS Color Bitmap  | SRG Partial BSSID Bitmap |
     * ------------------------------------------------------------------------------
     * |    0 or 1                       |     0 or 8                     | 0 or 8  |
     */
    he_buffer = puc_he_srp_ie + IE_BUFFER_HEADER;

    /* SR Control */
    pst_he_sr_control = (mac_frame_he_sr_control_stru *)he_buffer;
    he_buffer += OAL_SIZEOF(mac_frame_he_sr_control_stru);
    l_ret = memcpy_s((uint8_t*)(&pst_he_srp_value->st_sr_control), OAL_SIZEOF(mac_frame_he_sr_control_stru),
        (uint8_t*)pst_he_sr_control, OAL_SIZEOF(mac_frame_he_sr_control_stru));

    if (pst_he_sr_control->bit_non_srg_offset_present == 1) {
        /* Non-SRG OBSS PD Max Offset  */
        pst_he_srp_value->uc_non_srg_obss_pd_max_offset = *he_buffer;
        he_buffer += 1;
    }

    if (pst_he_sr_control->bit_srg_information_present == 1) {
        /* SRG OBSS PD Min Offset */
        pst_he_srp_value->uc_srg_obss_pd_min_offset = *he_buffer;
        he_buffer += 1;

        /* SRG OBSS PD Max Offset */
        pst_he_srp_value->uc_srg_obss_pd_max_offset = *he_buffer;
        he_buffer += 1;

        /* SRG BSS Color Bitmap */
        l_ret += memcpy_s((uint8_t*)(&pst_he_srp_value->auc_srg_bss_color_bitmap), MAC_HE_SRG_BSS_COLOR_BITMAP_LEN,
            (uint8_t*)he_buffer, MAC_HE_SRG_BSS_COLOR_BITMAP_LEN);
        he_buffer += MAC_HE_SRG_BSS_COLOR_BITMAP_LEN;

        /* SRG BSS Color Bitmap */
        l_ret += memcpy_s((uint8_t*)(&pst_he_srp_value->auc_srg_partial_bssid_bitmap),
            MAC_HE_SRG_PARTIAL_BSSID_BITMAP_LEN, (uint8_t*)he_buffer, MAC_HE_SRG_PARTIAL_BSSID_BITMAP_LEN);
        he_buffer += MAC_HE_SRG_PARTIAL_BSSID_BITMAP_LEN;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_11AX, "mac_ie_parse_spatial_reuse_parameter::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*lint -restore */

uint32_t mac_ie_proc_he_opern_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_payload, mac_user_stru *pst_mac_user)
{
    mac_frame_he_oper_ie_stru st_he_oper_ie_value;
    mac_he_hdl_stru st_he_hdl;
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr3(pst_mac_vap, pst_mac_user, puc_payload))) {
        oam_error_log3(0, OAM_SF_11AX, "{mac_ie_proc_he_opern_ie::param null,%X %X %X.}", (uintptr_t)pst_mac_vap,
            (uintptr_t)pst_mac_user, (uintptr_t)puc_payload);
        return MAC_NO_CHANGE;
    }

    memset_s(&st_he_oper_ie_value, OAL_SIZEOF(st_he_oper_ie_value), 0, OAL_SIZEOF(st_he_oper_ie_value));
    ul_ret = mac_ie_parse_he_oper(puc_payload, &st_he_oper_ie_value);
    if (ul_ret != OAL_SUCC) {
        return MAC_NO_CHANGE;
    }

    mac_user_get_he_hdl(pst_mac_user, &st_he_hdl);

    /* ������he_opern_ie��������he_duration_rts_threshold */
    st_he_hdl.bit_he_duration_rts_threshold_exist = 1;

    if (st_he_oper_ie_value.st_bss_color.bit_bss_color != st_he_hdl.st_he_oper_ie.st_bss_color.bit_bss_color) {
        ul_ret |= MAC_HE_BSS_COLOR_CHANGE;
        /* ʶ��bss color��Ҫ���,dmac���� */
        st_he_hdl.bit_he_oper_bss_color_exist = OAL_TRUE;
    }

    if (st_he_oper_ie_value.st_bss_color.bit_partial_bss_color !=
        st_he_hdl.st_he_oper_ie.st_bss_color.bit_partial_bss_color) {
        ul_ret |= MAC_HE_PARTIAL_BSS_COLOR_CHANGE;
    }

    st_he_hdl.st_he_oper_ie = st_he_oper_ie_value;

    mac_user_set_he_hdl(pst_mac_user, &st_he_hdl);

    return ul_ret;
}


uint32_t mac_ie_parse_he_ndp_feedback_report_ie(uint8_t *he_ndp_ie, uint8_t *nfrp_buff_threshold_exp)
{
    uint8_t *data = NULL;

    if (he_ndp_ie[1] != MAC_HE_NDP_FEEDBACK_REPORT_LEN) {
        return OAL_FAIL;
    }

    data = he_ndp_ie + MAC_HE_NDP_FEEDBACK_REPORT_OFFSET;

    *nfrp_buff_threshold_exp = *data;

    return OAL_SUCC;
}


uint32_t mac_ie_parse_he_bss_color_change_announcement_ie(uint8_t *puc_payload,
    mac_frame_bss_color_change_annoncement_ie_stru *pst_bss_color)
{
    mac_frame_bss_color_change_annoncement_ie_stru *pst_bss_color_info = NULL;
    uint8_t *puc_data = NULL;

    if (puc_payload[1] != MAC_HE_BSS_COLOR_CHANGE_ANNOUNCEMENT_LEN) {
        return OAL_FAIL;
    }

    puc_data = puc_payload + IE_BUFFER_HEADER;

    pst_bss_color_info = (mac_frame_bss_color_change_annoncement_ie_stru*)puc_data;
    if (EOK != memcpy_s(pst_bss_color, OAL_SIZEOF(mac_frame_bss_color_change_annoncement_ie_stru), pst_bss_color_info,
        OAL_SIZEOF(mac_frame_bss_color_change_annoncement_ie_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "mac_ie_parse_he_bss_color_change_announcement_ie::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint32_t mac_ie_parse_multi_bssid_opt_subie(uint8_t *puc_frame_data,
    mac_multi_bssid_frame_info_stru *pst_mbssid_frame_info, uint8_t *puc_mbssid_body_ie_len)
{
    uint8_t *puc_data = puc_frame_data;
    uint8_t *puc_ssid = NULL;
    uint8_t *puc_non_transmitted_bssid_cap = NULL;
    uint8_t *puc_mbssid_index = NULL;
    uint8_t uc_ie_len;
    uint8_t uc_left_len;
    uint8_t uc_bssid_profile_len;

    /*
     * -------------------------------------------------------------------------
     * |EID |Length |Max BSSID Indicator| Non-Transmitted BSSID Profile ID| BSSID Profile Len|
     * -------------------------------------------------------------------------
     * |1   |1      |1                  |               1                 |    1
     * -------------------------------------------------------------------------
     * |sub_ssid |Length | sub_ssid|
     * -------------------------------------------------------------------------
     * |1        |1      |    n    |
     * -------------------------------------------------------------------------
     * |sub_nonTxBssid_Cap |Length |Nontransmitted bssid CAP     |
     * -------------------------------------------------------------------------
     * |1                  |1      |    2                        |
     * -------------------------------------------------------------------------
     * |sub_Multi BSSID Index |Length |bssid index|
     * -------------------------------------------------------------------------
     * |1                     |1      |    1      |
     * ��֤�����涨Non-transmitted BSSID Profile ����Ҫ���� NonTxBSSID Cap IE(4), SSID IE(34Bytes),M-BSSID Index IE(3)
     */
    /* �ж�Non-transmitted BSSID Profile �Ƿ����  */
    if (puc_data[0] != 0) {
        oam_warning_log0(0, OAM_SF_11AX, "{mac_ie_parse_multi_bssid_ie:: bssid profile not exist.}");
        return OAL_FAIL;
    }
    uc_bssid_profile_len = puc_data[1];
    puc_data += MAC_IE_HDR_LEN;
    uc_left_len = uc_bssid_profile_len;

    /* ����non-transmitted bssid cap IE83 */
    puc_non_transmitted_bssid_cap = mac_find_ie(MAC_EID_NONTRANSMITTED_BSSID_CAP, puc_data, uc_left_len);
    if (puc_non_transmitted_bssid_cap == NULL) {
        oam_warning_log0(0, OAM_SF_11AX, "{mac_ie_parse_multi_bssid_ie:: parase ie83 fail.}");
        return OAL_FAIL;
    }
    uc_ie_len = puc_non_transmitted_bssid_cap[1];
    pst_mbssid_frame_info->us_non_tramsmitted_bssid_cap = *(uint16_t*)(puc_non_transmitted_bssid_cap + MAC_IE_HDR_LEN);
    puc_data += (uc_ie_len + MAC_IE_HDR_LEN);
    uc_left_len -= (uc_ie_len + MAC_IE_HDR_LEN);

    /* ���� ssidԪ��    */
    puc_ssid = mac_find_ie(MAC_EID_SSID, puc_data, uc_left_len);
    if (puc_ssid == NULL || puc_ssid[1] > WLAN_SSID_MAX_LEN) {
        return OAL_FAIL;
    }
    uc_ie_len = puc_ssid[1];
    if (EOK != memcpy_s(pst_mbssid_frame_info->auc_non_transmitted_ssid, WLAN_SSID_MAX_LEN, puc_ssid + MAC_IE_HDR_LEN,
        uc_ie_len)) {
        oam_warning_log0(0, OAM_SF_11AX, "mac_ie_parse_multi_bssid_ie::memcpy fail!");
        return OAL_FAIL;
    }
    pst_mbssid_frame_info->uc_non_transmitted_ssid_len = uc_ie_len;

    puc_data += (uc_ie_len + MAC_IE_HDR_LEN);
    uc_left_len -= (uc_ie_len + MAC_IE_HDR_LEN);

    /* ����Non-transmitted BSSID Profile ��m-bssid index  ie85 */
    puc_mbssid_index = mac_find_ie(MAC_EID_MULTI_BSSID_INDEX, puc_data, uc_left_len);
    if (puc_mbssid_index == NULL) {
        oam_warning_log0(0, OAM_SF_11AX, "{mac_ie_parse_multi_bssid_ie:: parase ie85 fail.}");
        return OAL_FAIL;
    }
    pst_mbssid_frame_info->uc_bssid_index = puc_mbssid_index[MAC_IE_HDR_LEN];

    *puc_mbssid_body_ie_len = uc_bssid_profile_len + MAC_IE_HDR_LEN;

    return OAL_SUCC;
}

#endif // _PRE_WLAN_FEATURE_11AX

uint32_t mac_ie_proc_ext_cap_ie(mac_user_stru* pst_mac_user, uint8_t* puc_payload)
{
    mac_user_cap_info_stru* pst_cap_info = NULL;
    uint8_t uc_len;
    uint8_t auc_cap[NUM_8_BITS] = { 0 };

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_user, puc_payload))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_ie_proc_ext_cap_ie::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cap_info = &(pst_mac_user->st_cap_info);
    uc_len = puc_payload[BIT_OFFSET_1];
    if (uc_len >= MAC_IE_HDR_LEN && uc_len <= 8) { /* ie��ͷ���⣬�������Ϊ8 */
        /* ie�������ֵ��������IEͷ���ȣ��˴�����Ҫ���м�ȥͷ�� */
        if (memcpy_s(auc_cap, sizeof(auc_cap), &puc_payload[MAC_IE_HDR_LEN], uc_len) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "mac_ie_proc_ext_cap_ie::memcpy fail!");
            return OAL_FAIL;
        }
    }

    /* ��ȡ BIT12: ֧��proxy arp */
    pst_cap_info->bit_proxy_arp = ((auc_cap[BIT_OFFSET_1] & BIT4) == 0) ? OAL_FALSE : OAL_TRUE;
#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    /* ��ȡ BIT19: ֧��bss transition */
    pst_cap_info->bit_bss_transition = ((auc_cap[BIT_OFFSET_2] & BIT3) == 0) ? OAL_FALSE : OAL_TRUE;
#endif
    return OAL_SUCC;
}


uint8_t* mac_ie_find_vendor_vht_ie(uint8_t* puc_frame, uint16_t us_frame_len)
{
    uint8_t* puc_vendor_ie = NULL;
    uint8_t* puc_vht_ie = NULL;
    uint16_t us_offset_vendor_vht = MAC_WLAN_OUI_VENDOR_VHT_HEADER + MAC_IE_HDR_LEN;

    puc_vendor_ie =
        mac_find_vendor_ie(MAC_WLAN_OUI_BROADCOM_EPIGRAM, MAC_WLAN_OUI_VENDOR_VHT_TYPE, puc_frame, us_frame_len);
    if ((puc_vendor_ie != NULL) && (puc_vendor_ie[1] >= MAC_WLAN_OUI_VENDOR_VHT_HEADER)) {
        puc_vht_ie = mac_find_ie(MAC_EID_VHT_CAP, puc_vendor_ie + us_offset_vendor_vht,
            puc_vendor_ie[1] - MAC_WLAN_OUI_VENDOR_VHT_HEADER);
    }

    return puc_vht_ie;
}


uint16_t mac_ie_get_max_mpdu_len_by_vht_cap(uint8_t uc_vht_cap_max_mpdu_len_bit)
{
    uint16_t us_max_mpdu_length = 3895;

    if (uc_vht_cap_max_mpdu_len_bit == 0) {
        us_max_mpdu_length = 3895; /* ֧�ֵ����mpdu len,0:3895 */
    } else if (uc_vht_cap_max_mpdu_len_bit == 1) {
        us_max_mpdu_length = 7991; /* ֧�ֵ����mpdu len,1:7991 */
    } else if (uc_vht_cap_max_mpdu_len_bit == 2) { /* ֧�ֵ����mpdu len,2:11454 */
        us_max_mpdu_length = 11454;
    }

    return us_max_mpdu_length;
}

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

OAL_STATIC uint32_t mac_ie_check_proc_opmode_param(mac_user_stru* pst_mac_user,
    mac_opmode_notify_stru* pst_opmode_notify)
{
    /* USER���޶������ռ������������������ */
    if ((pst_mac_user->en_bandwidth_cap < pst_opmode_notify->bit_channel_width) ||
        (pst_mac_user->en_user_max_cap_nss < pst_opmode_notify->bit_rx_nss)) {
        /* p20pro 2G 1*1�ȵ㣬beaconЯ��opmodeΪ80M,��ɴ˴���ˢ��,���ڶԶ��쳣 */
        oam_warning_log4(pst_mac_user->uc_vap_id, OAM_SF_OPMODE,
            "{mac_ie_check_proc_opmode_param::bw or nss over limit! work bw[%d]opmode bw[%d]user_nss[%d]rx_nss[%d]!}",
            pst_mac_user->en_bandwidth_cap, pst_opmode_notify->bit_channel_width, pst_mac_user->en_user_max_cap_nss,
            pst_opmode_notify->bit_rx_nss);

        return OAL_FAIL;
    }

    /* Nss TypeֵΪ1�����ʾbeamforming Rx Nss���ܳ���������ֵ */
    if (pst_opmode_notify->bit_rx_nss_type == 1) {
        if (pst_mac_user->st_vht_hdl.bit_num_bf_ant_supported < pst_opmode_notify->bit_rx_nss) {
            oam_warning_log2(pst_mac_user->uc_vap_id, OAM_SF_OPMODE,
                "{mac_ie_check_proc_opmode_param::rx_nss is over limit!bit_num_bf_ant_supported[%d], bit_rx_nss[%d]!}",
                pst_mac_user->st_vht_hdl.bit_num_bf_ant_supported, pst_opmode_notify->bit_rx_nss);
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC wlan_bw_cap_enum_uint8 mac_ie_proc_opmode_channel_width(mac_user_stru* pst_mac_user,
    mac_opmode_notify_stru* pst_opmode_notify)
{
    wlan_bw_cap_enum_uint8 en_opmode_notify_bw = 0;

    if (pst_opmode_notify->bit_channel_width == WLAN_BW_CAP_80M) {
        en_opmode_notify_bw = (pst_opmode_notify->bit_160or8080) ? WLAN_BW_CAP_160M : WLAN_BW_CAP_80M;
    } else {
        en_opmode_notify_bw = pst_opmode_notify->bit_channel_width;
    }

    return en_opmode_notify_bw;
}

OAL_STATIC void mac_ie_proc_opmode_nss_handle(mac_vap_stru* pst_mac_vap, mac_user_stru* pst_mac_user,
    mac_opmode_notify_stru* pst_opmode_notify, oal_bool_enum_uint8 en_from_beacon)
{
    wlan_nss_enum_uint8 en_avail_nss;

    en_avail_nss = oal_min(pst_mac_vap->en_vap_rx_nss, pst_opmode_notify->bit_rx_nss);
    mac_user_set_smps_opmode_notify_nss(pst_mac_user, pst_opmode_notify->bit_rx_nss);
    if (en_avail_nss != pst_mac_user->en_avail_num_spatial_stream) {
#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            /* beacon ֡�е�opmode && he_rom_nss < en_opmode_notify_nss �������� */
            if (en_from_beacon && pst_mac_user->bit_have_recv_he_rom_flag &&
                pst_mac_user->en_he_rom_nss < en_avail_nss) {
                return;
            }
        }
#endif
        /* ��Ҫ��ȡvap�͸���nss��ȡС��������ǲ�֧��mimo�ˣ��Զ������л�mimoҲ��ִ�� */
        mac_user_set_avail_num_spatial_stream(pst_mac_user, en_avail_nss);
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_OPMODE,
            "{mac_ie_proc_opmode_nss_handle::change nss.en_vap_rx_nss=[%x],\
            en_avail_num_spatial_stream=[%d],pst_opmode_notify->bit_rx_nss=[%d],\
            bit_smps_opmode_notify_nss=[%d]!}",
            pst_mac_vap->en_vap_rx_nss, pst_mac_user->en_avail_num_spatial_stream, pst_opmode_notify->bit_rx_nss,
            mac_user_get_smps_opmode_notify_nss(pst_mac_user));
    }
}


uint32_t mac_ie_proc_opmode_field(mac_vap_stru* pst_mac_vap, mac_user_stru* pst_mac_user,
    mac_opmode_notify_stru* pst_opmode_notify, oal_bool_enum_uint8 en_from_beacon)
{
    wlan_bw_cap_enum_uint8 en_bwcap_vap = 0; /* vap����������� */
    wlan_bw_cap_enum_uint8 en_avail_bw = 0; /* vap����������� */
    wlan_bw_cap_enum_uint8 en_opmode_notify_bw;

    /* ���ָ���Ѿ��ڵ��ú�����֤�ǿգ�����ֱ��ʹ�ü��� */
    if (OAL_FAIL == mac_ie_check_proc_opmode_param(pst_mac_user, pst_opmode_notify)) {
        return OAL_FAIL;
    }

    en_opmode_notify_bw = mac_ie_proc_opmode_channel_width(pst_mac_user, pst_opmode_notify);
    /* �ж�channel_width�Ƿ���user֮ǰʹ��channel_width��ͬ */
    if (en_opmode_notify_bw != pst_mac_user->en_avail_bandwidth) {
        /* ��ȡvap�����������û����������Ľ��� */
        mac_vap_get_bandwidth_cap(pst_mac_vap, &en_bwcap_vap);
        if (en_bwcap_vap == WLAN_BW_CAP_160M && en_opmode_notify_bw == WLAN_BW_CAP_80M &&
            pst_mac_user->en_avail_bandwidth == WLAN_BW_CAP_160M) {
            en_avail_bw = oal_min(en_bwcap_vap, WLAN_BW_CAP_160M);
        } else {
            en_avail_bw = oal_min(en_bwcap_vap, en_opmode_notify_bw);
        }
#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            /* beacon ֡�е�opmode && he_rom_bw < en_opmode_notify_bw �������� */
            if (en_from_beacon && pst_mac_user->bit_have_recv_he_rom_flag &&
                pst_mac_user->en_he_rom_bw < en_avail_bw) {
                return OAL_SUCC;
            }
        }
#endif
        mac_user_set_bandwidth_info(pst_mac_user, en_avail_bw, en_avail_bw);

        oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_OPMODE,
            "{mac_ie_proc_opmode_field::change bandwidth. en_bwcap_vap[%x], user avail bandwidth = [%x]!}",
            en_bwcap_vap, pst_mac_user->en_avail_bandwidth);
    }

    /* �ж�Rx Nss Type�Ƿ�Ϊbeamformingģʽ */
    if (pst_opmode_notify->bit_rx_nss_type == 1) {
        oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_OPMODE,
            "{mac_ie_proc_opmode_field::pst_opmode_notify->bit_rx_nss_type == 1!}\r\n");

        /* �ж�Rx Nss�Ƿ���user֮ǰʹ��Rx Nss��ͬ */
        if (pst_opmode_notify->bit_rx_nss != pst_mac_user->en_avail_bf_num_spatial_stream) {
            /* ��Ҫ��ȡvap�͸���nss��ȡС��������ǲ�֧��mimo�ˣ��Զ������л�mimoҲ��ִ�� */
            mac_user_avail_bf_num_spatial_stream(pst_mac_user,
                oal_min(pst_mac_vap->en_vap_rx_nss, pst_opmode_notify->bit_rx_nss));
        }
    } else {
        mac_ie_proc_opmode_nss_handle(pst_mac_vap, pst_mac_user, pst_opmode_notify, en_from_beacon);
    }

    return OAL_SUCC;
}
#endif


uint8_t mac_ie_get_chan_num(uint8_t *frame_body, uint16_t frame_len,
                            uint16_t offset, uint8_t curr_chan)
{
    uint8_t  chan_num = 0;
    uint8_t *ie_start_addr;

    /* ��DSSS Param set ie�н���chan num */
    ie_start_addr = mac_find_ie(MAC_EID_DSPARMS, frame_body + offset, frame_len - offset);
    if ((ie_start_addr != OAL_PTR_NULL) && (ie_start_addr[1] == MAC_DSPARMS_LEN)) {
        chan_num = ie_start_addr[BIT_OFFSET_2]; /* ie��2�ֽڰ�������Ϣ���ŵ��� */
        if (mac_is_channel_num_valid(mac_get_band_by_channel_num(chan_num), chan_num) == OAL_SUCC) {
            return chan_num;
        }
    }

    /* ��HT operation ie�н��� chan num */
    ie_start_addr = mac_find_ie(MAC_EID_HT_OPERATION, frame_body + offset, frame_len - offset);
    if ((ie_start_addr != OAL_PTR_NULL) && (ie_start_addr[1] >= 1)) {
        chan_num = ie_start_addr[BIT_OFFSET_2]; /* ie��2�ֽڰ�������Ϣ���ŵ��� */
        if (mac_is_channel_num_valid(mac_get_band_by_channel_num(chan_num), chan_num) == OAL_SUCC) {
            return chan_num;
        }
    }

    chan_num = curr_chan;
    return chan_num;
}


uint32_t mac_set_second_channel_offset_ie(wlan_channel_bandwidth_enum_uint8 en_bw,
                                          uint8_t *buffer,
                                          uint8_t *output_len)
{
    if (oal_any_null_ptr2(buffer, output_len)) {
        oam_error_log0(0, OAM_SF_SCAN, "{mac_set_second_channel_offset_ie::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* Ĭ�����Ϊ�� */
    *buffer = '\0';
    *output_len = 0;

    /* 11n ����Secondary Channel Offset Element */
    /******************************************************************/
    /* -------------------------------------------------------------- */
    /* |Ele. ID |Length |Secondary channel offset |                   */
    /* -------------------------------------------------------------- */
    /* |1       |1      |1                        |                   */
    /*                                                                */
    /******************************************************************/
    buffer[BIT_OFFSET_0] = 62; /* buffer[0]��Ele.ID�ֶ�,62��ID�� */
    buffer[BIT_OFFSET_1] = 1; /* 1��ʾ��IEͷ���⣬��1���ֽ� */

    switch (en_bw) {
        case WLAN_BAND_WIDTH_20M:
            buffer[BIT_OFFSET_2] = 0; /* no secondary channel */
            break;
        case WLAN_BAND_WIDTH_40PLUS:
        case WLAN_BAND_WIDTH_80PLUSPLUS:
        case WLAN_BAND_WIDTH_80PLUSMINUS:
#ifdef _PRE_WLAN_FEATURE_160M
        case WLAN_BAND_WIDTH_160PLUSPLUSPLUS:
        case WLAN_BAND_WIDTH_160PLUSPLUSMINUS:
        case WLAN_BAND_WIDTH_160PLUSMINUSPLUS:
        case WLAN_BAND_WIDTH_160PLUSMINUSMINUS:
#endif
            buffer[BIT_OFFSET_2] = 1; /* secondary 20M channel above */
            break;
        case WLAN_BAND_WIDTH_40MINUS:
        case WLAN_BAND_WIDTH_80MINUSPLUS:
        case WLAN_BAND_WIDTH_80MINUSMINUS:
#ifdef _PRE_WLAN_FEATURE_160M
        case WLAN_BAND_WIDTH_160MINUSPLUSPLUS:
        case WLAN_BAND_WIDTH_160MINUSPLUSMINUS:
        case WLAN_BAND_WIDTH_160MINUSMINUSPLUS:
        case WLAN_BAND_WIDTH_160MINUSMINUSMINUS:
#endif
            buffer[BIT_OFFSET_2] = 3; /* 3��ʾsecondary 20M channel below */
            break;
        default:
            oam_error_log1(0, OAM_SF_SCAN, "{mac_set_second_channel_offset_ie::invalid bandwidth[%d].}", en_bw);
            return OAL_FAIL;
    }

    *output_len = 3; /* �����buffer����Ϊ3 */

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_160M
static uint32_t mac_set_new_ch_width_160M(uint8_t channel, wlan_channel_bandwidth_enum_uint8 en_bw, uint8_t *buffer)
{
    /* buffer[2]��ʾNew Ch width�ֶ�,0��ʾû���´���1��ʾ�� */
    /* buffer[3]��ʾ��1����������Ƶ�ʶ�Ӧ���ŵ�(Center Freq seg1)�ֶ� */
    /* buffer[4]��ʾ��2����������Ƶ�ʶ�Ӧ���ŵ�(Center Freq seg2)�ֶ� */
    /* channel + n ��������Ƶ������n*5��ÿ���ŵ�����Ϊ5MHZ */
    switch (en_bw) {
        case WLAN_BAND_WIDTH_160PLUSPLUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_6;
            buffer[BIT_OFFSET_4] = channel + CHAN_OFFSET_14;
            break;
        case WLAN_BAND_WIDTH_160PLUSPLUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_6;
            buffer[BIT_OFFSET_4] = channel - CHAN_OFFSET_2;
            break;
        /* ��20�ŵ�+1, ��40�ŵ�-1, ��80�ŵ�+1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_2;
            buffer[BIT_OFFSET_4] = channel + CHAN_OFFSET_6;
            break;
        /* ��20�ŵ�+1, ��40�ŵ�-1, ��80�ŵ�-1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_2;
            buffer[BIT_OFFSET_4] = channel - CHAN_OFFSET_10;
            break;
        /* ��20�ŵ�-1, ��40�ŵ�+1, ��80�ŵ�+1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_2;
            buffer[BIT_OFFSET_4] = channel + CHAN_OFFSET_10;
            break;
        /* ��20�ŵ�-1, ��40�ŵ�+1, ��80�ŵ�-1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_2;
            buffer[BIT_OFFSET_4] = channel - CHAN_OFFSET_6;
            break;
        /* ��20�ŵ�-1, ��40�ŵ�-1, ��80�ŵ�+1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_6;
            buffer[BIT_OFFSET_4] = channel + CHAN_OFFSET_2;
            break;
        /* ��20�ŵ�-1, ��40�ŵ�-1, ��80�ŵ�-1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_6;
            buffer[BIT_OFFSET_4] = channel - CHAN_OFFSET_14;
            break;
        default:
            oam_error_log1(0, OAM_SF_SCAN, "{mac_set_new_ch_width_160M::invalid bandwidth[%d].}", en_bw);
            return OAL_FAIL;
    }
    return OAL_SUCC;
}
#endif

static uint32_t mac_set_new_ch_width(uint8_t channel,
                                     wlan_channel_bandwidth_enum_uint8 en_bw,
                                     uint8_t *buffer)
{
    uint32_t ret = OAL_SUCC;
    /* buffer[2]��ʾNew Ch width�ֶ�,0��ʾû���´���1��ʾ�� */
    /* buffer[3]��ʾ��1����������Ƶ�ʶ�Ӧ���ŵ�(Center Freq seg1)�ֶ� */
    /* channel + n ��������Ƶ������n*5��ÿ���ŵ�����Ϊ5MHZ */
    switch (en_bw) {
        case WLAN_BAND_WIDTH_20M:
        case WLAN_BAND_WIDTH_40PLUS:
        case WLAN_BAND_WIDTH_40MINUS:
            buffer[BIT_OFFSET_2] = 0;
            buffer[BIT_OFFSET_3] = 0;
            break;
        case WLAN_BAND_WIDTH_80PLUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_6;
            break;
        case WLAN_BAND_WIDTH_80PLUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_2;
            break;
        case WLAN_BAND_WIDTH_80MINUSPLUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel + CHAN_OFFSET_2;
            break;
        case WLAN_BAND_WIDTH_80MINUSMINUS:
            buffer[BIT_OFFSET_2] = 1;
            buffer[BIT_OFFSET_3] = channel - CHAN_OFFSET_6;
            break;
        default:
#ifdef _PRE_WLAN_FEATURE_160M
            ret = mac_set_new_ch_width_160M(channel, en_bw, buffer);
#else
            oam_error_log1(0, OAM_SF_SCAN, "{mac_set_new_ch_width::invalid bandwidth[%d].}", en_bw);
            return OAL_FAIL;
#endif
    }
    return ret;
}

uint32_t mac_set_11ac_wideband_ie(uint8_t channel,
                                  wlan_channel_bandwidth_enum_uint8 en_bw,
                                  uint8_t *buffer,
                                  uint8_t *output_len)
{
    if (oal_any_null_ptr2(buffer, output_len)) {
        oam_error_log0(0, OAM_SF_SCAN, "{mac_set_11ac_wideband_ie::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* Ĭ�����Ϊ�� */
    *buffer = '\0';
    *output_len = 0;

    /* 11ac ����Wide Bandwidth Channel Switch Element                 */
    /******************************************************************/
    /* -------------------------------------------------------------- */
    /* |ID |Length |New Ch width |Center Freq seg1 |Center Freq seg2  */
    /* -------------------------------------------------------------- */
    /* |1  |1      |1            |1                |1                 */
    /*                                                                */
    /******************************************************************/
    buffer[BIT_OFFSET_0] = 194; /* 194��ID�� */
    buffer[BIT_OFFSET_1] = 3; /* 3�ǳ�IEͷ����ĳ��� */

    if (mac_set_new_ch_width(channel, en_bw, buffer) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if ((en_bw < WLAN_BAND_WIDTH_160PLUSPLUSPLUS) || (en_bw > WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        buffer[BIT_OFFSET_4] = 0; /* reserved. Not support 80M + 80M */
    }

    *output_len = 5; /* �����buffer����Ϊ5 */

    return OAL_SUCC;
}


uint32_t mac_ie_proc_chwidth_field(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user, uint8_t uc_chwidth)
{
    wlan_bw_cap_enum_uint8 en_bwcap_vap = 0; /* vap����������� */

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, pst_mac_user))) {
        oam_error_log2(0, OAM_SF_2040,
                       "{mac_ie_proc_opmode_field::pst_mac_user = [%x],   \
                       pst_opmode_notify = [%x], pst_mac_vap = [%x]!}\r\n",
                       (uintptr_t)pst_mac_user, (uintptr_t)pst_mac_vap);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap_get_bandwidth_cap(pst_mac_vap, &en_bwcap_vap);
    en_bwcap_vap = oal_min(en_bwcap_vap, (wlan_bw_cap_enum_uint8)uc_chwidth);
    mac_user_set_bandwidth_info(pst_mac_user, en_bwcap_vap, en_bwcap_vap);

    return OAL_SUCC;
}


uint32_t mac_proc_ht_opern_ie(mac_vap_stru *mac_vap, uint8_t *payload, mac_user_stru *mac_user)
{
    mac_ht_opern_ac_stru        *ht_opern = OAL_PTR_NULL; // todo::���������ṹ�����ĳ�һ���ṹ��
    mac_user_ht_hdl_stru         ht_hdl;
    uint32_t                     change = MAC_NO_CHANGE;
    mac_sec_ch_off_enum_uint8    secondary_chan_offset_old;
    uint8_t                      sta_chan_width_old;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, payload, mac_user))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_proc_ht_opern_ie::param null.}");
        return change;
    }

    if (payload[BIT_OFFSET_1] < 6) { /* ����У�飬�˴����õ�ǰ6�ֽڣ�����Basic MCS Setδ�漰 */
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{mac_proc_ht_opern_ie::invalid ht opern ie len[%d].}", payload[BIT_OFFSET_1]);
        return change;
    }

    mac_user_get_ht_hdl(mac_user, &ht_hdl);

    secondary_chan_offset_old = ht_hdl.bit_secondary_chan_offset;
    sta_chan_width_old = ht_hdl.bit_sta_chan_width;

    /************************ HT Operation Element *************************************
      ----------------------------------------------------------------------
      |EID |Length |PrimaryChannel |HT Operation Information |Basic MCS Set|
      ----------------------------------------------------------------------
      |1   |1      |1              |5                        |16           |
      ----------------------------------------------------------------------
    ***************************************************************************/
    /************************ HT Information Field ****************************
     |--------------------------------------------------------------------|
     | Primary | Seconday  | STA Ch | RIFS |           reserved           |
     | Channel | Ch Offset | Width  | Mode |                              |
     |--------------------------------------------------------------------|
     |    1    | B0     B1 |   B2   |  B3  |    B4                     B7 |
     |--------------------------------------------------------------------|

     |----------------------------------------------------------------|
     |     HT     | Non-GF STAs | resv      | OBSS Non-HT  | Reserved |
     | Protection |   Present   |           | STAs Present |          |
     |----------------------------------------------------------------|
     | B0     B1  |     B2      |    B3     |     B4       | B5   B15 |
     |----------------------------------------------------------------|

     |-------------------------------------------------------------|
     | Reserved |  Dual  |  Dual CTS  | Seconday | LSIG TXOP Protn |
     |          | Beacon | Protection |  Beacon  | Full Support    |
     |-------------------------------------------------------------|
     | B0    B5 |   B6   |     B7     |     B8   |       B9        |
     |-------------------------------------------------------------|

     |---------------------------------------|
     |  PCO   |  PCO  | Reserved | Basic MCS |
     | Active | Phase |          |    Set    |
     |---------------------------------------|
     |  B10   |  B11  | B12  B15 |    16     |
     |---------------------------------------|
    **************************************************************************/
    ht_opern = (mac_ht_opern_ac_stru *)(&payload[MAC_IE_HDR_LEN]);

    /* ��ȡHT Operation IE�е�"Secondary Channel Offset" */
    ht_hdl.bit_secondary_chan_offset = ht_opern->bit_secondary_chan_offset;

    /* ��2.4G�û�����20M����¸ñ������л� */
    if ((ht_opern->bit_sta_chan_width == WLAN_BAND_WIDTH_20M) &&
        (mac_vap->st_channel.en_band == WLAN_BAND_2G)) {
        ht_hdl.bit_secondary_chan_offset = MAC_SCN;
    }

    /* ������� */
    ht_hdl.bit_rifs_mode = ht_opern->bit_rifs_mode; /* ������������дʱ����Ҫ��ֵ */
    ht_hdl.bit_HT_protection = ht_opern->bit_HT_protection;
    ht_hdl.bit_nongf_sta_present = ht_opern->bit_nongf_sta_present;
    ht_hdl.bit_obss_nonht_sta_present = ht_opern->bit_obss_nonht_sta_present;
    ht_hdl.bit_lsig_txop_protection_full_support = ht_opern->bit_lsig_txop_protection_full_support;
    ht_hdl.bit_sta_chan_width = ht_opern->bit_sta_chan_width;
    ht_hdl.uc_chan_center_freq_seg2 = ht_opern->bit_chan_center_freq_seg2;

    mac_user_set_ht_hdl(mac_user, &ht_hdl);

    if ((secondary_chan_offset_old != ht_hdl.bit_secondary_chan_offset) ||
        (sta_chan_width_old != ht_hdl.bit_sta_chan_width)) {
        change = MAC_HT_CHANGE;
        oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_ANY,
                         "mac_proc_ht_opern_ie:usr_bw is updated second_chan_offset from [%d] to [%d],   \
                          chan_with from [%d] to [%d]",
                         secondary_chan_offset_old, ht_hdl.bit_secondary_chan_offset,
                         sta_chan_width_old, ht_hdl.bit_sta_chan_width);
    }

    return change;
}


uint32_t mac_ie_proc_obss_scan_ie(mac_vap_stru *mac_vap, uint8_t *payload)
{
    uint16_t trigger_scan_interval;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, payload))) {
        oam_error_log0(0, OAM_SF_SCAN, "{mac_ie_proc_obss_scan_ie::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /********************Overlapping BSS Scan Parameters element******************
     |ElementID |Length |OBSS    |OBSS   |BSS Channel   |OBSS Scan  |OBSS Scan   |
     |          |       |Scan    |Scan   |Width Trigger |Passive    |Active Total|
     |          |       |Passive |Active |Scan Interval |Total Per  |Per         |
     |          |       |Dwell   |Dwell  |              |Channel    |Channel     |
     ----------------------------------------------------------------------------
     |1         |1      |2       |2      |2             |2          |2           |
     ----------------------------------------------------------------------------
     |BSS Width   |OBSS Scan|
     |Channel     |Activity |
     |Transition  |Threshold|
     |Delay Factor|         |
     ------------------------
     |2           |2        |
    ***************************************************************************/
    if (payload[1] < MAC_OBSS_SCAN_IE_LEN) {
        return OAL_FAIL;
    }

    trigger_scan_interval = oal_make_word16(payload[BIT_OFFSET_6], payload[BIT_OFFSET_7]);
    if (trigger_scan_interval == 0) {
        mac_vap_set_peer_obss_scan(mac_vap, OAL_FALSE);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    mac_mib_set_OBSSScanPassiveDwell(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_2], payload[BIT_OFFSET_3]));
    mac_mib_set_OBSSScanActiveDwell(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_4], payload[BIT_OFFSET_5]));
    /* obssɨ��������С180��,���600S, ��ʼ��Ĭ��Ϊ300�� */
    mac_mib_set_BSSWidthTriggerScanInterval(mac_vap, oal_min(oal_max(trigger_scan_interval, 180), 600));
    mac_mib_set_OBSSScanPassiveTotalPerChannel(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_8], payload[BIT_OFFSET_9]));
    mac_mib_set_OBSSScanActiveTotalPerChannel(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_10], payload[BIT_OFFSET_11]));
    mac_mib_set_BSSWidthChannelTransitionDelayFactor(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_12], payload[BIT_OFFSET_13]));
    mac_mib_set_OBSSScanActivityThreshold(mac_vap,
        oal_make_word16(payload[BIT_OFFSET_14], payload[BIT_OFFSET_15]));
    mac_vap_set_peer_obss_scan(mac_vap, OAL_TRUE);

    return OAL_SUCC;
}


uint32_t mac_ie_proc_vht_opern_ie(mac_vap_stru *mac_vap, uint8_t *payload, mac_user_stru *mac_user)
{
    mac_vht_hdl_stru                    vht_hdl;
    uint8_t                           ret = MAC_NO_CHANGE;
    uint16_t                          basic_mcs_set_all_user;
    wlan_mib_vht_op_width_enum_uint8    channel_width_old;
    uint8_t                           channel_center_freq_seg0_old;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, payload))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_ie_proc_vht_opern_ie::param null.}");
        return ret;
    }

    /* ����У�� */
    if (payload[BIT_OFFSET_1] < MAC_VHT_OPERN_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "{mac_ie_proc_vht_opern_ie::invalid vht opern len[%d].}", payload[1]);
        return ret;
    }

    mac_user_get_vht_hdl(mac_user, &vht_hdl);

    channel_width_old = vht_hdl.en_channel_width;
    channel_center_freq_seg0_old = vht_hdl.uc_channel_center_freq_seg0;

    /* ���� "VHT Operation Information" */
    vht_hdl.en_channel_width = payload[MAC_IE_HDR_LEN];
    vht_hdl.uc_channel_center_freq_seg0 = payload[MAC_IE_HDR_LEN + 1]; /* ��1����������Ƶ�� */
    vht_hdl.uc_channel_center_freq_seg1 = payload[MAC_IE_HDR_LEN + 2]; /* ��2����������Ƶ�� */

    /* 0 -- 20/40M, 1 -- 80M,160M,80+80 2 -- 160M, 3--80M+80M */
    if (vht_hdl.en_channel_width > WLAN_MIB_VHT_OP_WIDTH_80PLUS80) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{mac_ie_proc_vht_opern_ie::invalid channel width[%d], use 20M chn width.}",
                         vht_hdl.en_channel_width);
        vht_hdl.en_channel_width = WLAN_MIB_VHT_OP_WIDTH_20_40;
    }

    /* ���� "VHT Basic MCS Set field" */
    basic_mcs_set_all_user = oal_make_word16(
        payload[MAC_IE_HDR_LEN + 3], payload[MAC_IE_HDR_LEN + 4]); /* ƴ��+3,+4�ֽ�Ϊ����mcs������ */
    vht_hdl.us_basic_mcs_set = basic_mcs_set_all_user;

    mac_user_set_vht_hdl(mac_user, &vht_hdl);

    if ((channel_width_old != vht_hdl.en_channel_width) ||
        (channel_center_freq_seg0_old != vht_hdl.uc_channel_center_freq_seg0)) {
        ret = MAC_VHT_CHANGE;
        oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_ANY,
                         "mac_ie_proc_vht_opern_ie:usr_bw is updated chanl_with from [%d] to [%d], \
                         chanl_center_freq_seg0 from [%d] to [%d]",
                         channel_width_old, vht_hdl.en_channel_width,
                         channel_center_freq_seg0_old, vht_hdl.uc_channel_center_freq_seg0);
    }

    return ret;
}

static uint32_t mac_ie_get_suite(uint8_t **ie, uint8_t *src_ie, uint32_t *suite,
                                 uint8_t wlan_suites, uint8_t type)
{
    uint8_t  suite_idx, size;
    uint8_t  ie_len = *(src_ie - 1);
    uint16_t suites_count;

    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, *ie, ie_len, 2) == OAL_FALSE) { /* 2��ʾSuite Count�ֶδ�С */
        oam_warning_log1(0, OAM_SF_ANY,
            "mac_ie_get_wpa_cipher:no enough mem for suites_count, wpa ie len[%d]", ie_len);
        return OAL_ERR_CODE_MSG_LENGTH_ERR;
    }
    suites_count = oal_make_word16((*ie)[BIT_OFFSET_0], (*ie)[BIT_OFFSET_1]);
    *ie += BIT_OFFSET_2; /* ie+=2��ʾie�Ƶ�Suite List�ֶ� */

    if (wlan_suites == 0) {
        size = 16; /* 16��ʾPMKID��С */
    } else if (type == 0) { /* 0��ʾ�׼���wpa��ȡ�������rsn��ȡ */
        if (suites_count > OAL_NL80211_MAX_NR_CIPHER_SUITES) {
            return OAL_ERR_CODE_MSG_LENGTH_ERR;
        }
        size = 4; /* 4��ʾ�׼���С */
    } else {
        if ((suites_count == 0) || (suites_count > OAL_NL80211_MAX_NR_CIPHER_SUITES)) {
            return OAL_ERR_CODE_MSG_LENGTH_ERR;
        }
        size = 4; /* 4��ʾ�׼���С */
    }

    for (suite_idx = 0; suite_idx < suites_count; suite_idx++) {
        if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, *ie, ie_len, size) == OAL_FALSE) {
            oam_warning_log2(0, OAM_SF_ANY,
                "mac_ie_get_wpa_suite: invalid ie len[%d], suites_count[%d]",
                ie_len, suites_count);
            return OAL_ERR_CODE_MSG_LENGTH_ERR;
        }
        if (suite_idx < wlan_suites) {
            suite[suite_idx] = *(uint32_t *)(*ie);
        }
        *ie += size;
    }

    return OAL_SUCC;
}


uint32_t mac_ie_get_wpa_cipher(uint8_t *ie, mac_crypto_settings_stru *crypto)
{
    uint8_t  ie_len;
    uint32_t ret;
    uint8_t *src_ie = ie + BIT_OFFSET_2; /* ie+2��ʾWPA OUI�ֶ� */

    if (oal_any_null_ptr2(ie, crypto)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s(crypto, OAL_SIZEOF(mac_crypto_settings_stru), 0, OAL_SIZEOF(mac_crypto_settings_stru));
    /**************************************************************************/
    /*                  WPA Element Format                                    */
    /* ---------------------------------------------------------------------- */
    /* |Element ID | Length |    WPA OUI    |  Version |  Group Cipher Suite  */
    /* ---------------------------------------------------------------------- */
    /* |     1     |   1    |        4      |     2    |         4            */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* Pairwise Cipher |  Pairwise Cipher   |                 |               */
    /* Suite Count     |    Suite List      | AKM Suite Count |AKM Suite List */
    /* ---------------------------------------------------------------------- */
    /*        2        |          4*m       |         2       |     4*n       */
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /**************************************************************************/
    ie_len = ie[BIT_OFFSET_1];
    if (ie_len < MAC_MIN_WPA_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "mac_ie_get_wpa_cipher:invalid wpa ie len[%d]", ie_len);
        return  OAL_ERR_CODE_MSG_LENGTH_ERR;
    }

    ie += BIT_OFFSET_8; /* ie+=8��ʾie�Ƶ�������׼�(Group Cipher Suite)�ֶ� */
    crypto->ul_wpa_versions = WITP_WPA_VERSION_1;

    /* Group Cipher Suite */
    crypto->ul_group_suite = *(uint32_t *)ie;
    ie += BIT_OFFSET_4; /* ie+=4��ʾie�Ƶ��ɶԼ����׼���(Pairwise Cipher Suite count)�ֶ� */

    /* Pairwise Cipher */
    ret = mac_ie_get_suite(&ie, src_ie, crypto->aul_pair_suite, WLAN_PAIRWISE_CIPHER_SUITES, 0);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* AKM Suite */
    ret = mac_ie_get_suite(&ie, src_ie, crypto->aul_akm_suite, WLAN_AUTHENTICATION_SUITES, 0);
    if (ret != OAL_SUCC) {
        return ret;
    }

    return OAL_SUCC;
}


uint32_t mac_ie_get_rsn_cipher(uint8_t *ie, mac_crypto_settings_stru *crypto)
{
    uint8_t  ie_len;
    uint8_t *src_ie = ie + BIT_OFFSET_2; /* ie+2��ʾVersion�ֶ� */

    if (oal_any_null_ptr2(ie, crypto)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s(crypto, OAL_SIZEOF(mac_crypto_settings_stru), 0, OAL_SIZEOF(mac_crypto_settings_stru));
    /**************************************************************************/
    /*                  RSN Element Format                                    */
    /* ---------------------------------------------------------------------- */
    /* |Element ID | Length | Version | Group Cipher Suite | Pairwise Suite   */
    /* ---------------------------------------------------------------------- */
    /* |     1     |    1   |    2    |         4          |       2          */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /*  Count| Pairwise Cipher Suite List | AKM Suite Count | AKM Suite List  */
    /* ---------------------------------------------------------------------- */
    /*       |         4*m                |     2           |   4*n           */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* |RSN Capabilities|PMKID Count|PMKID List|Group Management Cipher Suite */
    /* ---------------------------------------------------------------------- */
    /* |        2       |    2      |   16 *s  |               4           |  */
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /**************************************************************************/
    ie_len = ie[BIT_OFFSET_1];
    if (ie_len < MAC_MIN_RSN_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "mac_ie_get_rsn_cipher:invalid rsn ie len[%d]", ie_len);
        return OAL_FAIL;
    }

    /* version�ֶμ�� */
    crypto->ul_wpa_versions = *(uint16_t *)src_ie;
    if (crypto->ul_wpa_versions != 1) {
        return OAL_FAIL;
    }
    crypto->ul_wpa_versions = WITP_WPA_VERSION_2;  // wpaΪ1��rsnΪ2
    ie += BIT_OFFSET_4; /* ie+=4��ʾie�Ƶ�������׼�(Group Cipher Suite)�ֶ� */

    /* Group Cipher Suite */
    crypto->ul_group_suite = *(uint32_t *)ie;
    ie += BIT_OFFSET_4; /* ie+=4��ʾie�Ƶ��ɶԼ����׼�����(Pairwise Suite Count)�ֶ� */

    /* Pairwise Cipher Suite */
    if (mac_ie_get_suite(&ie, src_ie, crypto->aul_pair_suite, WLAN_PAIRWISE_CIPHER_SUITES, 1) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* AKM Suite */
    if (mac_ie_get_suite(&ie, src_ie, crypto->aul_akm_suite, WLAN_AUTHENTICATION_SUITES, 1) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* Խ��RSN Capabilities */
    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, ie, ie_len, 2) == OAL_FALSE) { /* 2��ʾRSN Capabilities�ֶδ�С */
        if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, ie, ie_len, 1) == OAL_TRUE) { /* 1����IE��һ�ֶεĴ�С */
            return OAL_FAIL;
        }
        return OAL_SUCC;
    }
    ie += BIT_OFFSET_2; /* ie+=2��ʾie�Ƶ�PMKID Count�ֶ� */

    /* ĿǰPMK��Ϣ�ݲ������� */
    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, ie, ie_len, 2) == OAL_FALSE) { /* 2��ʾPMKID Count�ֶδ�С */
        return OAL_SUCC;
    }
    if (mac_ie_get_suite(&ie, src_ie, crypto->aul_pair_suite, 0, 1) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* ��ȡGroup Management Cipher Suite��Ϣ */
    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(src_ie, ie, ie_len, 4) == OAL_FALSE) { /* 4��ʾGroup Management Cipher Suite��С */
        return OAL_SUCC;
    }
    if (((*(uint32_t *)ie) & 0xFFFFFF) == MAC_RSN_CHIPER_OUI(0)) {
        crypto->ul_group_mgmt_suite = *(uint32_t *)ie;
        ie += BIT_OFFSET_4; /* ie+=4��ʾie�Ƶ�IE��ĩβ */
    }

    return OAL_SUCC;
}
/*lint -restore */
/*lint -e19*/
oal_module_symbol(mac_ie_proc_sm_power_save_field);
oal_module_symbol(mac_ie_proc_ht_green_field);
oal_module_symbol(mac_ie_get_chan_num);
oal_module_symbol(mac_ie_proc_ht_supported_channel_width);
oal_module_symbol(mac_ie_proc_lsig_txop_protection_support);
oal_module_symbol(mac_ie_proc_ext_cap_ie);
oal_module_symbol(mac_ie_get_wpa_cipher);
oal_module_symbol(mac_ie_get_rsn_cipher);
oal_module_symbol(mac_set_second_channel_offset_ie);
oal_module_symbol(mac_set_11ac_wideband_ie);
oal_module_symbol(mac_ie_proc_ht_sta);
oal_module_symbol(mac_ie_proc_chwidth_field);
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
oal_module_symbol(mac_ie_proc_opmode_field);
#endif
oal_module_symbol(mac_ie_proc_obss_scan_ie);
oal_module_symbol(mac_proc_ht_opern_ie);
oal_module_symbol(mac_ie_proc_vht_opern_ie); /*lint +e19*/
