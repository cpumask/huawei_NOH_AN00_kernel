

/* 1 ͷ�ļ����� */
#include "oam_ext_if.h"
#include "mac_frame.h"
#include "mac_frame_common.h"
#include "hmac_config.h"
#include "mac_vap.h"
#include "mac_device.h"
#include "securec.h"
#include "securectype.h"
#include "hmac_mbo.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MBO_C

#ifdef _PRE_WLAN_FEATURE_MBO

OAL_STATIC void mac_set_mbo_cell_para(void *pst_vap, uint8_t *puc_buffer,
    uint8_t *puc_ie_len, uint8_t uc_mbo_cell_capa)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ieee80211_vendor_ie_stru *pst_vendor_ie = OAL_PTR_NULL;

    pst_mac_vap->st_mbo_para_info.uc_mbo_cell_capa = uc_mbo_cell_capa;

    pst_vendor_ie = (mac_ieee80211_vendor_ie_stru *)puc_buffer;
    /* ����MBOԪ�ص�OUI��OUI type = 0x16 */
    pst_vendor_ie->uc_element_id = MAC_EID_VENDOR;
    pst_vendor_ie->uc_len = 7;
    pst_vendor_ie->auc_oui[0] = (uint8_t)((MAC_MBO_VENDOR_IE >> 16) & 0xff);
    pst_vendor_ie->auc_oui[1] = (uint8_t)((MAC_MBO_VENDOR_IE >> 8) & 0xff);
    pst_vendor_ie->auc_oui[2] = (uint8_t)((MAC_MBO_VENDOR_IE)&0xff);
    pst_vendor_ie->uc_oui_type = MAC_MBO_IE_OUI_TYPE;

    puc_buffer[6] = MBO_ATTR_ID_CELL_DATA_CAPA;
    puc_buffer[7] = 1;
    puc_buffer[8] = uc_mbo_cell_capa;

    *puc_ie_len = pst_vendor_ie->uc_len + MAC_IE_HDR_LEN;
}


void mac_set_mbo_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    /***************************************************************************
                         --------------------------------------------------
                         |Element ID |Length |OUI|OUI Type|MBO Attributes |
                         --------------------------------------------------
          Octets:        |1          |1      |3  |1       |n              |
                         --------------------------------------------------
    ***************************************************************************/
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    uint8_t uc_mbo_ie_len = 0;

    /* ��MBO���ƻ�����δ�򿪣�����MBO IE ����return */
    if (pst_mac_vap->st_mbo_para_info.uc_mbo_enable != OAL_TRUE) {
        *puc_ie_len = 0;
        return;
    } else {
        mac_set_mbo_cell_para(pst_vap, puc_buffer, &uc_mbo_ie_len, pst_mac_vap->st_mbo_para_info.uc_mbo_cell_capa);
        *puc_ie_len = uc_mbo_ie_len;
    }
}


void mac_set_mbo_ie_ap(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    /***************************************************************************
                         --------------------------------------------------
                         |Element ID |Length |OUI|OUI Type|MBO Attributes |
                         --------------------------------------------------
          Octets:        |1          |1      |3  |1       |n              |
                         --------------------------------------------------
    ***************************************************************************/
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ieee80211_vendor_ie_stru *pst_vendor_ie = OAL_PTR_NULL;

    /* ��MBO���ƻ�����δ�򿪣�����MBO IE����Կ���δ�� ����return */
    if ((pst_mac_vap->st_mbo_para_info.uc_mbo_enable != OAL_TRUE) ||
        (pst_mac_vap->st_mbo_para_info.uc_mbo_assoc_disallowed_test_switch != OAL_TRUE)) {
        *puc_ie_len = 0;
        return;
    } else {
        pst_vendor_ie = (mac_ieee80211_vendor_ie_stru *)puc_buffer;
        /* ����MBOԪ�ص�OUI��OUI type = 0x16 */
        pst_vendor_ie->uc_element_id = MAC_EID_VENDOR;
        pst_vendor_ie->uc_len = 7;
        pst_vendor_ie->auc_oui[0] = (uint8_t)((MAC_MBO_VENDOR_IE >> 16) & 0xff);
        pst_vendor_ie->auc_oui[1] = (uint8_t)((MAC_MBO_VENDOR_IE >> 8) & 0xff);
        pst_vendor_ie->auc_oui[2] = (uint8_t)((MAC_MBO_VENDOR_IE)&0xff);
        pst_vendor_ie->uc_oui_type = MAC_MBO_IE_OUI_TYPE;

        puc_buffer[6] = MBO_ATTR_ID_ASSOC_DISALLOW;
        puc_buffer[7] = 1;   /* length */
        puc_buffer[8] = 0x0; /* reason code */
        *puc_ie_len = pst_vendor_ie->uc_len + MAC_IE_HDR_LEN;
    }
}


void mac_set_supported_operating_classes_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    /*******************************************************************************************
            ------------------------------------------------------------------------------------
            |ElementID | Length | Current Operating Class| Operating Class |
            ------------------------------------------------------------------------------------
    Octets: |1         | 1      | 1                      | variable        |
            ------------------------------------------------------------------------------------
            |Current Operating Class Extension Sequence(O) | Operating Class Duple Sequence(O) |
            ------------------------------------------------------------------------------------
    Octets: |              variable                        |             variable              |
            ------------------------------------------------------------------------------------

    *********************************************************************************************/
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /* ��MBO ���ƻ�����δ�򿪣�����Supported Operating Classed IE */
    if (pst_mac_vap->st_mbo_para_info.uc_mbo_enable != OAL_TRUE) {
        *puc_ie_len = 0;
        return;
    } else {
        puc_buffer[0] = MAC_EID_OPERATING_CLASS;
        puc_buffer[1] = 3;
        if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
            puc_buffer[2] = MAC_GLOBAL_OPERATING_CLASS_NUMBER81;
        }
        if (pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) {
            puc_buffer[2] = MAC_GLOBAL_OPERATING_CLASS_NUMBER115;
        }
        puc_buffer[3] = MAC_GLOBAL_OPERATING_CLASS_NUMBER81;
        puc_buffer[4] = MAC_GLOBAL_OPERATING_CLASS_NUMBER115;

        *puc_ie_len = MAC_IE_HDR_LEN + 3;
    }
}


uint8_t *mac_find_mbo_attribute(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len)
{
    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* ����MBO Attribute���������ֱ������һ�� */
    while (l_len > MAC_MBO_ATTRIBUTE_HDR_LEN && puc_ies[0] != uc_eid) {
        l_len -= puc_ies[1] + MAC_MBO_ATTRIBUTE_HDR_LEN;
        puc_ies += puc_ies[1] + MAC_MBO_ATTRIBUTE_HDR_LEN;
    }

    /* ���ҵ�MBO Attribute��ʣ�೤�Ȳ�ƥ��ֱ�ӷ��ؿ�ָ�� */
    if ((l_len < MAC_MBO_ATTRIBUTE_HDR_LEN) || (l_len < (MAC_MBO_ATTRIBUTE_HDR_LEN + puc_ies[1]))) {
        return OAL_PTR_NULL;
    }

    return puc_ies;
}

void hmac_handle_ie_specific_mbo(uint8_t *puc_ie_data,
    hmac_neighbor_bss_info_stru *pst_bss_list_alloc, uint8_t uc_bss_list_index)
{
    if (EOK != memcpy_s(pst_bss_list_alloc[uc_bss_list_index].st_assoc_delay_attr_mbo_ie.auc_oui,
                        sizeof(pst_bss_list_alloc[uc_bss_list_index].st_assoc_delay_attr_mbo_ie.auc_oui),
                        puc_ie_data + MAC_IE_HDR_LEN, MAC_MBO_OUI_LENGTH)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_handle_ie_by_data_length::memcpy oui fail!");
        return;
    }
    if ((puc_ie_data[5] == MAC_MBO_IE_OUI_TYPE) &&
        (puc_ie_data[6] == MBO_ATTR_ID_ASSOC_RETRY_DELAY)) {
        pst_bss_list_alloc[uc_bss_list_index].st_assoc_delay_attr_mbo_ie.ul_re_assoc_delay_time =
           (((uint16_t)puc_ie_data[9]) << 8) | (puc_ie_data[8]);
    }

    return;
}


uint32_t hmac_mbo_check_is_assoc_or_re_assoc_allowed(mac_vap_stru *pst_mac_vap,
    mac_conn_param_stru *pst_connect_param, mac_bss_dscr_stru *pst_bss_dscr)
{
    uint32_t       ul_current_time;

    /* MBO����δ�򿪣�return true */
    if (pst_mac_vap->st_mbo_para_info.uc_mbo_enable == OAL_FALSE) {
        return OAL_TRUE;
    }

    /* ���жϴ�bss�ǲ��Ǻ��в��������MBO IE��BSS��������򷵻�false */
    if (pst_bss_dscr->uc_bss_assoc_disallowed == OAL_TRUE) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_mbo_check_is_assoc_or_re_assoc_allowed::bssid:%02X:XX:XX:%02X:%02X:%02X mbo assoc disallowed}",
            pst_connect_param->auc_bssid[0], pst_connect_param->auc_bssid[3], pst_connect_param->auc_bssid[4],
            pst_connect_param->auc_bssid[5]);
        return OAL_FALSE;
    }

    /* re-assoc delay�������������Ϊ0��return true */
    if (pst_mac_vap->st_mbo_para_info.en_disable_connect == OAL_FALSE) {
        return OAL_TRUE;
    }

    /* �жϴ�BSS�Ƿ���re-assoc delay bss������ǽ�����ش��� */
    if (!oal_memcmp(pst_mac_vap->st_mbo_para_info.auc_re_assoc_delay_bss_mac_addr, 
        pst_connect_param->auc_bssid, WLAN_MAC_ADDR_LEN)) {
        ul_current_time = (uint32_t)oal_time_get_stamp_ms(); /* ��ȡ��ǰʱ�� */
        /* ���յ�btm req֡��ul_re_assoc_delay_time�ڲ��������bss���ع�������֡ */
        if ((ul_current_time - pst_mac_vap->st_mbo_para_info.ul_btm_req_received_time) <=
             pst_mac_vap->st_mbo_para_info.ul_re_assoc_delay_time) {
            oam_warning_log3(0, OAM_SF_CFG, "hmac_mbo_check_is_assoc_or_re_assoc_allowed::current time[%u] < \
                             (btm recv time[%u] + re-assoc_delay time[%d]) not timed out!bss connect disallowed!",
                             ul_current_time, pst_mac_vap->st_mbo_para_info.ul_btm_req_received_time,
                             pst_mac_vap->st_mbo_para_info.ul_re_assoc_delay_time);
            return OAL_FALSE;
        }

        /* �ѳ�ʱ�������������־λ���� */
        pst_mac_vap->st_mbo_para_info.en_disable_connect = OAL_FALSE;
    }

    return OAL_TRUE;
}


void hmac_scan_update_bss_assoc_disallowed_attr(mac_bss_dscr_stru *pst_bss_dscr,
                                                uint8_t *puc_frame_body,
                                                uint16_t us_frame_len)
{
    uint8_t *puc_mbo_ie = OAL_PTR_NULL;
    uint8_t *puc_mbo_attribute = OAL_PTR_NULL;

    puc_mbo_ie = mac_find_vendor_ie(MAC_MBO_VENDOR_IE, MAC_MBO_IE_OUI_TYPE, puc_frame_body, us_frame_len);
    if (puc_mbo_ie == OAL_PTR_NULL) {
        pst_bss_dscr->uc_bss_assoc_disallowed = OAL_FALSE;
        return;
    }

    puc_mbo_attribute = mac_find_mbo_attribute(MBO_ATTR_ID_ASSOC_DISALLOW,
                                               puc_mbo_ie + MBO_IE_HEADER,
                                               MAC_MBO_ASSOC_DIS_ATTR_LEN);
    if (puc_mbo_attribute == OAL_PTR_NULL) {
        pst_bss_dscr->uc_bss_assoc_disallowed = OAL_FALSE; /* ������Association Disallowed Attribute��������� */
    } else {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_scan_update_bss_assoc_disallowed_attr::Assoc Disallowed Attr found!}");
        pst_bss_dscr->uc_bss_assoc_disallowed = OAL_TRUE; /* ����Association Disallowed Attribute,AP ���������� */
    }

    return;
}

#endif

