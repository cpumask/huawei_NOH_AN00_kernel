

/* 1 ͷ�ļ����� */
#include "hmac_scan.h"
#include "oam_ext_if.h"
#include "mac_ie.h"
#include "mac_regdomain.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_hiex.h"

#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_mgmt_sta.h"
#include "frw_ext_if.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#include "hmac_roam_alg.h"
#include "hmac_chan_mgmt.h"
#include "hmac_p2p.h"
#ifdef _PRE_WLAN_FEATURE_DFS
#include "hmac_dfs.h"
#endif

#include "hisi_customize_wifi.h"

#include "hmac_roam_main.h"

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
#include "hmac_hid2d.h"
#endif
#include "hmac_ht_self_cure.h"
#include "securec.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_MBO
#include "hmac_mbo.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SCAN_C

/* 2 ȫ�ֱ������� */
hmac_scan_state_enum_uint8 g_en_bgscan_enable_flag = HMAC_BGSCAN_ENABLE;
uint32_t g_pd_bss_expire_time = 0;
/* �ȴ�������ɵĶ���ȴ�ʱ��(��λ: ms)���ο�wpa supplicant��������ʱ�ĵȴ�ʱ�� */
#define HMAC_P2P_LISTEN_EXTRA_WAIT_TIME 30


int32_t hmac_snprintf_hex(uint8_t *puc_buf, int32_t l_buf_size, uint8_t *puc_data, int32_t l_len)
{
    int32_t l_loop;
    uint8_t *puc_pos = OAL_PTR_NULL;
    uint8_t *puc_end = OAL_PTR_NULL;
    int32_t l_ret;

    if (l_buf_size <= 0) {
        return 0;
    }

    puc_pos = puc_buf;
    puc_end = puc_buf + l_buf_size;
    for (l_loop = 0; l_loop < l_len; l_loop++) {
        l_ret = snprintf_s((int8_t *)puc_pos, (uint16_t)(puc_end - puc_pos),
                           (uint16_t)(puc_end - puc_pos) - 1, "%02x ", puc_data[l_loop]);
        if ((l_ret < 0) || (l_ret >= puc_end - puc_pos)) {
            puc_buf[l_buf_size - 1] = '\0';
            return puc_pos - puc_buf;
        }

        puc_pos += l_ret;
    }

    puc_buf[l_buf_size - 1] = '\0';
    return puc_pos - puc_buf;
}


OAL_STATIC void hmac_scan_print_scan_params(mac_scan_req_stru *pst_scan_params, mac_vap_stru *pst_mac_vap)
{
    if (!((pst_scan_params->uc_scan_func == MAC_SCAN_FUNC_P2P_LISTEN) &&
           hmac_get_feature_switch(HMAC_MIRACAST_REDUCE_LOG_SWITCH))) {
        oam_warning_log4(pst_scan_params->uc_vap_id, OAM_SF_SCAN,
            "hmac_scan_print_scan_params::Now Scan channel_num[%d] in [%d]ms with scan_func[0x%x], and ssid_num[%d]!",
            pst_scan_params->uc_channel_nums,
            pst_scan_params->us_scan_time,
            pst_scan_params->uc_scan_func,
            pst_scan_params->uc_ssid_num);

        oam_warning_log3(pst_scan_params->uc_vap_id, OAM_SF_SCAN,
            "hmac_scan_print_scan_params::p2p_scan:%d,max_scan_count_per_channel:%d,need back home_channel:%d!",
            pst_scan_params->bit_is_p2p0_scan,
            pst_scan_params->uc_max_scan_count_per_channel,
            pst_scan_params->en_need_switch_back_home_channel);
    }
    return;
}

OAL_STATIC void hmac_wifi_hide_ssid(uint8_t *puc_frame_body, uint16_t us_mac_frame_len)
{
    uint8_t *puc_ssid_ie = OAL_PTR_NULL;
    uint8_t uc_ssid_len = 0;
    uint8_t uc_index;

    if (puc_frame_body == OAL_PTR_NULL) {
        return;
    }

    puc_ssid_ie = mac_get_ssid(puc_frame_body, us_mac_frame_len, &uc_ssid_len);
    /* ����4λ���������4λ�������� */
    if (puc_ssid_ie == OAL_PTR_NULL || uc_ssid_len < 4) {
        return;
    }

    for (uc_index = 2; uc_index < uc_ssid_len - 2; uc_index++)  { // ����ǰ2λ�ͺ�2λ
        *(puc_ssid_ie + uc_index) = 0x78;  // 0x78ת��ΪASCII������ַ�'x'
    }
}


void hmac_scan_print_scanned_bss_info(uint8_t uc_device_id)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    uint8_t auc_sdt_parse_hdr[MAC_80211_FRAME_LEN];
    uint8_t *puc_tmp_mac_body_addr = OAL_PTR_NULL;
    uint8_t uc_frame_sub_type;
    int32_t l_ret;

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_SCAN, "{hmac_scan_print_scanned_bss_info::pst_hmac_device null.}");
        return;
    }

    /* ��ȡָ��ɨ�����Ĺ���ṹ���ַ */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ����ɨ�赽��bss��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head)) {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        /* ����ʾ�����뵽��BSS֡ */
        if (pst_scanned_bss->st_bss_dscr_info.en_new_scan_bss == OAL_TRUE) {
            /* �ϱ�beacon��probe֡ */
            pst_frame_hdr = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

            /* ��beacon��duration�ֶ�(2�ֽ�)����Ϊrssi�Լ�channel,����SDT��ʾ */
            l_ret = memcpy_s((uint8_t *)auc_sdt_parse_hdr, MAC_80211_FRAME_LEN,
                             (uint8_t *)pst_frame_hdr, MAC_80211_FRAME_LEN);
            auc_sdt_parse_hdr[2] = (uint8_t)pst_bss_dscr->c_rssi;
            auc_sdt_parse_hdr[3] = pst_bss_dscr->st_channel.uc_chan_number;

            puc_tmp_mac_body_addr = (uint8_t *)oal_memalloc(pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN);
            if (oal_unlikely(puc_tmp_mac_body_addr == OAL_PTR_NULL)) {
                oam_warning_log0(0, OAM_SF_SCAN,
                    "{hmac_scan_print_scanned_bss_info::alloc memory failed for storing tmp mac_frame_body.}");
                continue;
            }

            l_ret += memcpy_s(puc_tmp_mac_body_addr, pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN,
                              (uint8_t *)(pst_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN),
                              pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN);
            if (l_ret != EOK) {
                oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_print_scanned_bss_info::memcpy fail!");
                oal_free(puc_tmp_mac_body_addr);
                /* ����� */
                oal_spin_unlock(&(pst_bss_mgmt->st_lock));
                return;
            }

            uc_frame_sub_type = mac_get_frame_type_and_subtype((uint8_t *)pst_frame_hdr);
            if ((uc_frame_sub_type == WLAN_FC0_SUBTYPE_BEACON) ||
                (uc_frame_sub_type == WLAN_FC0_SUBTYPE_PROBE_RSP)) {
                hmac_wifi_hide_ssid(puc_tmp_mac_body_addr, pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN);
            }

            /* �ϱ�beacon֡����probe rsp֡ */
            /*lint -e416*/
            oam_report_80211_frame(BROADCAST_MACADDR, (uint8_t *)auc_sdt_parse_hdr, MAC_80211_FRAME_LEN,
                puc_tmp_mac_body_addr, (uint16_t)pst_bss_dscr->ul_mgmt_len, OAM_OTA_FRAME_DIRECTION_TYPE_RX);
            oal_free(puc_tmp_mac_body_addr);
            /*lint +e416*/
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
            proc_sniffer_write_file(NULL, 0, pst_bss_dscr->auc_mgmt_buff, (uint16_t)pst_bss_dscr->ul_mgmt_len, 0);
#endif
#endif
        }
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return;
}


OAL_STATIC hmac_scanned_bss_info *hmac_scan_alloc_scanned_bss(uint32_t ul_mgmt_len)
{
    hmac_scanned_bss_info *pst_scanned_bss;

    /* �����ڴ棬�洢ɨ�赽��bss��Ϣ */
    pst_scanned_bss = oal_memalloc(OAL_SIZEOF(hmac_scanned_bss_info) + ul_mgmt_len -
                                   OAL_SIZEOF(pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff));
    if (oal_unlikely(pst_scanned_bss == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_SCAN,
                         "{hmac_scan_alloc_scanned_bss::alloc memory failed for storing scanned result.}");
        return OAL_PTR_NULL;
    }

    /* Ϊ������ڴ����� */
    memset_s(pst_scanned_bss,
             OAL_SIZEOF(hmac_scanned_bss_info) + ul_mgmt_len -
             OAL_SIZEOF(pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff), 0,
             OAL_SIZEOF(hmac_scanned_bss_info) + ul_mgmt_len -
             OAL_SIZEOF(pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff));

    /* ��ʼ������ͷ�ڵ�ָ�� */
    oal_dlist_init_head(&(pst_scanned_bss->st_dlist_head));

    return pst_scanned_bss;
}


OAL_STATIC uint32_t hmac_scan_add_bss_to_list(hmac_scanned_bss_info *pst_scanned_bss,
                                              hmac_device_stru *pst_hmac_device)
{
    hmac_bss_mgmt_stru *pst_bss_mgmt; /* ����ɨ�����Ľṹ�� */

    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);
    pst_scanned_bss->st_bss_dscr_info.en_new_scan_bss = OAL_TRUE;

    /* ������д����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ���ɨ�����������У�������ɨ�赽��bss���� */
    oal_dlist_add_tail(&(pst_scanned_bss->st_dlist_head), &(pst_bss_mgmt->st_bss_list_head));

    pst_bss_mgmt->ul_bss_num++;
    /* ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_scan_del_bss_from_list_nolock(hmac_scanned_bss_info *pst_scanned_bss,
                                                       hmac_device_stru *pst_hmac_device)
{
    hmac_bss_mgmt_stru *pst_bss_mgmt; /* ����ɨ�����Ľṹ�� */

    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* ��������ɾ���ڵ㣬������ɨ�赽��bss���� */
    oal_dlist_delete_entry(&(pst_scanned_bss->st_dlist_head));

    pst_bss_mgmt->ul_bss_num--;

    return OAL_SUCC;
}


void hmac_scan_clean_scan(hmac_scan_stru *pst_scan)
{
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;

    /* �����Ϸ��Լ�� */
    if (pst_scan == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_clean_scan::pst_scan is null.}");
        return;
    }

    pst_scan_record = &pst_scan->st_scan_record_mgmt;

    /* 1.һ��Ҫ�����ɨ�赽��bss��Ϣ���ٽ������㴦�� */
    pst_bss_mgmt = &(pst_scan_record->st_bss_mgmt);

    /* ������д����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ��������ɾ��ɨ�赽��bss��Ϣ */
    while (oal_dlist_is_empty(&(pst_bss_mgmt->st_bss_list_head)) == OAL_FALSE) {
        pst_entry = oal_dlist_delete_head(&(pst_bss_mgmt->st_bss_list_head));
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);

        pst_bss_mgmt->ul_bss_num--;

        /* �ͷ�ɨ���������ڴ� */
        oal_free(pst_scanned_bss);
    }

    /* ������д����ǰ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    /* 2.������Ϣ���� */
    memset_s(pst_scan_record, OAL_SIZEOF(hmac_scan_record_stru), 0, OAL_SIZEOF(hmac_scan_record_stru));
    pst_scan_record->en_scan_rsp_status = MAC_SCAN_STATUS_BUTT; /* ��ʼ��ɨ�����ʱ״̬��Ϊ��Чֵ */
    pst_scan_record->en_vap_last_state = MAC_VAP_STATE_BUTT;    /* ������BUTT,����aputͣɨ���vap״̬�ָ��� */

    /* 3.���³�ʼ��bss������������� */
    pst_bss_mgmt = &(pst_scan_record->st_bss_mgmt);
    oal_dlist_init_head(&(pst_bss_mgmt->st_bss_list_head));
    oal_spin_lock_init(&(pst_bss_mgmt->st_lock));

    /* 4.ɾ��ɨ�賬ʱ��ʱ�� */
    if (pst_scan->st_scan_timeout.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(pst_scan->st_scan_timeout));
    }

    oam_info_log0(0, OAM_SF_SCAN, "{hmac_scan_clean_scan::cleaned scan record success.}");

    return;
}


OAL_STATIC int32_t hmac_is_connected_ap_bssid(uint8_t uc_device_id, uint8_t auc_bssid[WLAN_MAC_ADDR_LEN])
{
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device;

    pst_mac_device = mac_res_get_dev(uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_is_connected_ap_bssid::mac_res_get_dev return null.}");
        return OAL_FALSE;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_P2P, "{hmac_is_connected_ap_bssid::mac_res_get_mac_vap fail! vap id is %d}",
                             pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        if (IS_LEGACY_VAP(pst_mac_vap) &&
            (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP)) {
            if (oal_memcmp(auc_bssid, pst_mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN) == 0) {
                /* ���ϻ���ǰ������AP */
                oam_info_log3(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                              "{hmac_is_connected_ap_bssid::connected AP bssid:%02X:XX:XX:XX:%02X:%02X}",
                              auc_bssid[0], auc_bssid[4], auc_bssid[5]);

                return OAL_TRUE;
            }
        }
    }

    return OAL_FALSE;
}


OAL_STATIC void hmac_scan_clean_expire_scanned_bss(hmac_vap_stru *pst_hmac_vap,
                                                   hmac_scan_record_stru *pst_scan_record)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry_tmp = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint32_t ul_curr_time_stamp;

    /* �����Ϸ��Լ�� */
    if (pst_scan_record == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_clean_expire_scanned_bss::scan record is null.}");
        return;
    }

    /* ����ɨ���bss����Ľṹ�� */
    pst_bss_mgmt = &(pst_scan_record->st_bss_mgmt);

    ul_curr_time_stamp = (uint32_t)oal_time_get_stamp_ms();

    /* ������д����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ��������ɾ����һ��ɨ�����е��ڵ�bss��Ϣ */
    oal_dlist_search_for_each_safe(pst_entry, pst_entry_tmp, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        pst_scanned_bss->st_bss_dscr_info.en_new_scan_bss = OAL_FALSE;

        if (oal_time_after32(ul_curr_time_stamp,
                             (pst_bss_dscr->ul_timestamp + HMAC_SCAN_MAX_SCANNED_BSS_EXPIRE)) == FALSE) {
            oam_info_log0(0, OAM_SF_SCAN,
                          "{hmac_scan_clean_expire_scanned_bss::do not remove the BSS, because it has not expired.}");
            continue;
        }
        /* �����ϻ�ʹ�� */
        if ((g_pd_bss_expire_time != 0) &&
            (ul_curr_time_stamp - pst_bss_dscr->ul_timestamp < g_pd_bss_expire_time * 1000)) {
            continue;
        }

        /* ���ϻ���ǰ���ڹ�����AP */
        if (hmac_is_connected_ap_bssid(pst_scan_record->uc_device_id, pst_bss_dscr->auc_bssid)) {
            pst_bss_dscr->c_rssi = pst_hmac_vap->station_info.signal;
            continue;
        }

        /* ��������ɾ���ڵ㣬������ɨ�赽��bss���� */
        oal_dlist_delete_entry(&(pst_scanned_bss->st_dlist_head));
        pst_bss_mgmt->ul_bss_num--;

        /* �ͷŶ�Ӧ�ڴ� */
        oal_free(pst_scanned_bss);
    }

    /* ������д����ǰ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));
    return;
}



mac_bss_dscr_stru *hmac_scan_find_scanned_bss_dscr_by_index(uint8_t uc_device_id, uint32_t ul_bss_index)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    uint8_t ul_loop;

    /* ��ȡhmac device �ṹ */
    pst_hmac_device = hmac_res_get_mac_dev(uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_find_scanned_bss_by_index::pst_hmac_device is null.}");
        return OAL_PTR_NULL;
    }

    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* ������ɾ����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ������������ܹ�ɨ���bss�����������쳣 */
    if (ul_bss_index >= pst_bss_mgmt->ul_bss_num) {
        oam_warning_log0(0, OAM_SF_SCAN, "{hmac_scan_find_scanned_bss_by_index::no such bss in bss list!}");

        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));
        return OAL_PTR_NULL;
    }

    ul_loop = 0;
    /* �����������ض�Ӧindex��bss dscr��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);

        /* ��ͬ��bss index���� */
        if (ul_bss_index == ul_loop) {
            /* ���� */
            oal_spin_unlock(&(pst_bss_mgmt->st_lock));
            return &(pst_scanned_bss->st_bss_dscr_info);
        }

        ul_loop++;
    }
    /* ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return OAL_PTR_NULL;
}


hmac_scanned_bss_info *hmac_scan_find_scanned_bss_by_bssid(hmac_bss_mgmt_stru *pst_bss_mgmt, uint8_t *puc_bssid)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;

    /* �������������������Ƿ��Ѿ�������ͬbssid��bss��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        /* ��ͬ��bssid��ַ */
        if (oal_compare_mac_addr(pst_scanned_bss->st_bss_dscr_info.auc_bssid, puc_bssid) == 0) {
            return pst_scanned_bss;
        }
    }

    return OAL_PTR_NULL;
}


void *hmac_scan_get_scanned_bss_by_bssid(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr)
{
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL; /* ����ɨ���bss����Ľṹ�� */
    hmac_scanned_bss_info *pst_scanned_bss_info = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device;

    /* ��ȡhmac device �ṹ */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_SCAN, "{hmac_scan_get_scanned_bss_by_bssid::pst_hmac_device is null, dev id[%d].}",
                         pst_mac_vap->uc_device_id);
        return OAL_PTR_NULL;
    }

    /* ��ȡ����ɨ���bss����Ľṹ�� */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    pst_scanned_bss_info = hmac_scan_find_scanned_bss_by_bssid(pst_bss_mgmt, puc_mac_addr);
    if (pst_scanned_bss_info == OAL_PTR_NULL) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_get_scanned_bss_by_bssid::find the bss failed[%02X:XX:XX:%02X:%02X:%02X]}",
                         puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));
        return OAL_PTR_NULL;
    }

    /* ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return &(pst_scanned_bss_info->st_bss_dscr_info);
}


OAL_STATIC OAL_INLINE void hmac_scan_update_bss_list_wmm(mac_bss_dscr_stru *pst_bss_dscr,
                                                         uint8_t *puc_frame_body,
                                                         uint16_t us_frame_len)
{
    uint8_t *puc_ie = OAL_PTR_NULL;

    pst_bss_dscr->uc_wmm_cap = OAL_FALSE;
    pst_bss_dscr->uc_uapsd_cap = OAL_FALSE;

    puc_ie = mac_get_wmm_ie(puc_frame_body, us_frame_len);
    if (puc_ie != OAL_PTR_NULL) {
        pst_bss_dscr->uc_wmm_cap = OAL_TRUE;

        /* --------------------------------------------------------------------------------- */
        /* WMM Information/Parameter Element Format                                          */
        /* ---------------------------------------------------------------------------------- */
        /* EID | IE LEN | OUI | OUIType | OUISubtype | Version | QoSInfo | OUISubtype based | */
        /* --------------------------------------------------------------------------------- */
        /* 1   |   1    |  3  | 1       | 1          | 1       | 1       | ---------------- | */
        /* --------------------------------------------------------------------------------- */
        /* puc_ie[1] IE len ������EID��LEN�ֶ�,��ȡQoSInfo��uc_ie_len�������7�ֽڳ��� */
        /* Check if Bit 7 is set indicating U-APSD capability */
        if ((puc_ie[1] >= 7) && (puc_ie[8] & BIT7)) { /* wmm ie�ĵ�8���ֽ���QoS info�ֽ� */
            pst_bss_dscr->uc_uapsd_cap = OAL_TRUE;
        }
    } else {
        puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_frame_body, us_frame_len);
        if (puc_ie != OAL_PTR_NULL) {
            
            /* -------------------------------------------------------------- */
            /* HT capability Information/Parameter Element Format            */
            /* -------------------------------------------------------------- */
            /* EID | IE LEN |  HT capability Info |                 based   | */
            /* -------------------------------------------------------------- */
            /* 1   |   1    |         2           | ------------------------| */
            /* -------------------------------------------------------------- */
            /* puc_ie[1] IE len ������EID��LEN�ֶ�,��ȡHT cap Info��uc_ie_len�������2�ֽڳ��� */
            /* ht cap�ĵ� 2,3���ֽ���HT capability Info��Ϣ */
            /* Check if Bit 5 is set indicating short GI for 20M capability */
            if ((puc_ie[1] >= 2) && (puc_ie[2] & BIT5)) {
                pst_bss_dscr->uc_wmm_cap = OAL_TRUE;
            }
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_11D

OAL_STATIC void hmac_scan_update_bss_list_country(mac_bss_dscr_stru *pst_bss_dscr,
                                                  uint8_t *puc_frame_body,
                                                  uint16_t us_frame_len)
{
    uint8_t *puc_ie;

    puc_ie = mac_find_ie(MAC_EID_COUNTRY, puc_frame_body, us_frame_len);
    /* �����벻����, ȫ�����Ϊ0 */
    if (puc_ie == OAL_PTR_NULL) {
        pst_bss_dscr->ac_country[0] = 0;
        pst_bss_dscr->ac_country[1] = 0;
        pst_bss_dscr->ac_country[2] = 0;

        return;
    } else {
        pst_bss_dscr->puc_country_ie = puc_ie;
    }
    /* ���������2���ֽ�,IE LEN������ڵ���2 */
    if (puc_ie[1] >= 2) {
        pst_bss_dscr->ac_country[0] = (int8_t)puc_ie[MAC_IE_HDR_LEN];
        pst_bss_dscr->ac_country[1] = (int8_t)puc_ie[MAC_IE_HDR_LEN + 1];
        pst_bss_dscr->ac_country[2] = 0;
    }
}
#endif

#if defined(_PRE_WLAN_FEATURE_11K) || \
    defined(_PRE_WLAN_FEATURE_FTM)

OAL_STATIC void hmac_scan_update_bss_list_rrm(mac_bss_dscr_stru *pst_bss_dscr,
                                              uint8_t *puc_frame_body,
                                              uint16_t us_frame_len)
{
    uint8_t *puc_ie;
    oal_rrm_enabled_cap_ie_stru *rrm_ie = OAL_PTR_NULL;
    puc_ie = mac_find_ie(MAC_EID_RRM, puc_frame_body, us_frame_len);
    pst_bss_dscr->en_support_rrm = OAL_FALSE;
    pst_bss_dscr->en_support_neighbor = OAL_FALSE;
    if (puc_ie != OAL_PTR_NULL) {
        pst_bss_dscr->en_support_rrm = OAL_TRUE;
        if (puc_ie[1] >= sizeof(oal_rrm_enabled_cap_ie_stru)) {
            rrm_ie = (oal_rrm_enabled_cap_ie_stru*)(puc_ie + MAC_IE_HDR_LEN);
            pst_bss_dscr->en_support_neighbor = (rrm_ie->bit_neighbor_rpt_cap == 1) ? OAL_TRUE : OAL_FALSE;
        }
    }
}
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM

OAL_STATIC void hmac_scan_update_bss_list_1024qam(mac_bss_dscr_stru *pst_bss_dscr,
                                                  uint8_t *puc_frame_body,
                                                  uint16_t us_frame_len)
{
    uint8_t *puc_ie;
    puc_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_1024QAM_IE, puc_frame_body, us_frame_len);
    if (puc_ie == OAL_PTR_NULL) {
        pst_bss_dscr->en_support_1024qam = OAL_FALSE;
    } else {
        pst_bss_dscr->en_support_1024qam = OAL_TRUE;
    }
}
#endif

#ifdef _PRE_WLAN_NARROW_BAND

OAL_STATIC void hmac_scan_update_bss_list_nb(mac_bss_dscr_stru *pst_bss_dscr,
                                             uint8_t *puc_frame_body,
                                             uint16_t us_frame_len)
{
    uint8_t *puc_ie;

    puc_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_NB_IE, puc_frame_body, us_frame_len);
    /* �ж��Ƿ�Я����IE */
    if (puc_ie == OAL_PTR_NULL) {
        pst_bss_dscr->en_nb_capable = OAL_FALSE;
    } else {
        pst_bss_dscr->en_nb_capable = OAL_TRUE;
    }
}
#endif


OAL_STATIC void hmac_scan_update_11i(mac_bss_dscr_stru *pst_bss_dscr,
                                     uint8_t *puc_frame_body,
                                     uint16_t us_frame_len)
{
    pst_bss_dscr->puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_frame_body, (int32_t)(us_frame_len));
    pst_bss_dscr->puc_wpa_ie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT,
                                                  MAC_OUITYPE_WPA,
                                                  puc_frame_body,
                                                  (int32_t)(us_frame_len));
}

OAL_STATIC void hmac_scan_update_bss_list_11ntxbf(mac_bss_dscr_stru *p_bss_dscr,
    uint8_t *p_frame_body, uint16_t frame_len)
{
#ifdef _PRE_WLAN_FEATURE_TXBF
    uint8_t *puc_ie = OAL_PTR_NULL;

    puc_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_EID_11NTXBF, p_frame_body, (int32_t)frame_len);
    if (puc_ie != OAL_PTR_NULL) {
        p_bss_dscr->en_11ntxbf =
            (((mac_11ntxbf_vendor_ie_stru *)puc_ie)->st_11ntxbf.bit_11ntxbf == 1) ? OAL_TRUE : OAL_FALSE;
    }
#endif
}


OAL_STATIC void hmac_scan_update_bss_list_11n(mac_bss_dscr_stru *pst_bss_dscr,
                                              uint8_t *puc_frame_body,
                                              uint16_t us_frame_len)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    mac_ht_opern_stru *pst_ht_op = OAL_PTR_NULL;
    uint8_t uc_sec_chan_offset;
    wlan_bw_cap_enum_uint8 en_ht_cap_bw = WLAN_BW_CAP_20M;
    wlan_bw_cap_enum_uint8 en_ht_op_bw = WLAN_BW_CAP_20M;

    if (hmac_ht_self_cure_in_blacklist(pst_bss_dscr->auc_bssid)) {
        return;
    }

    /* 11n */
    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= 2)) {
        /* puc_ie[2]��HT Capabilities Info�ĵ�1���ֽ� */
        pst_bss_dscr->en_ht_capable = OAL_TRUE;        /* ֧��ht */
        pst_bss_dscr->en_ht_ldpc = (puc_ie[2] & BIT0); /* ֧��ldpc */
        en_ht_cap_bw = ((puc_ie[2] & BIT1) >> 1);      /* ȡ��֧�ֵĴ��� */
        pst_bss_dscr->en_ht_stbc = ((puc_ie[2] & BIT7) >> 7);
    }

    /* Ĭ��20M,���֡����δЯ��HT_OPERATION�����ֱ�Ӳ���Ĭ��ֵ */
    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_20M;

    puc_ie = mac_find_ie(MAC_EID_HT_OPERATION, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= 2)) {  // ����ie�����쳣���
        pst_ht_op = (mac_ht_opern_stru *)(puc_ie + MAC_IE_HDR_LEN);

        /* ��ȡ���ŵ�ƫ�� */
        uc_sec_chan_offset = pst_ht_op->bit_secondary_chan_offset;

        /* ��ֹap��channel width=0, ��channel offset = 1����3 ��ʱ��channel widthΪ�� */
        /* ht cap 20/40 enabled && ht operation 40 enabled */
        if ((pst_ht_op->bit_sta_chan_width != 0) && (en_ht_cap_bw > WLAN_BW_CAP_20M)) {  // cap > 20M��ȡchannel bw
            if (uc_sec_chan_offset == MAC_SCB) {
                pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_40MINUS;
                en_ht_op_bw = WLAN_BW_CAP_40M;
            } else if (uc_sec_chan_offset == MAC_SCA) {
                pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_40PLUS;
                en_ht_op_bw = WLAN_BW_CAP_40M;
            }
        }
    }

    /* ��AP��������ȡ������������Сֵ����ֹAP�쳣���ͳ��������������ݣ�������ݲ�ͨ */
    pst_bss_dscr->en_bw_cap = oal_min(en_ht_cap_bw, en_ht_op_bw);

    puc_ie = mac_find_ie(MAC_EID_EXT_CAPS, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= 1)) {
        /* Extract 20/40 BSS Coexistence Management Support */
        pst_bss_dscr->uc_coex_mgmt_supp = (puc_ie[2] & BIT0);
#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            /* �鿴 BSS Multi BSS֧����� */
            if (puc_ie[1] > 2) {
                pst_bss_dscr->st_mbssid_info.bit_ext_cap_multi_bssid_activated = ((puc_ie[4] & BIT6) >> 6);
            }
        }
#endif
    }
    hmac_scan_update_bss_list_11ntxbf(pst_bss_dscr, puc_frame_body, us_frame_len);
}

OAL_STATIC void hmac_scan_update_bss_list_11ac_vht_cap(mac_bss_dscr_stru *pst_bss_dscr,
    uint8_t *puc_frame_body, uint16_t us_frame_len, uint32_t en_is_vendor_ie, uint8_t uc_vendor_subtype)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_vht_capable;
    uint8_t uc_supp_ch_width;

    puc_ie = mac_find_ie(MAC_EID_VHT_CAP, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_VHT_CAP_IE_LEN)) {
        en_vht_capable = ((puc_ie[2] != 0) || (puc_ie[3] != 0) || (puc_ie[4] != 0) || (puc_ie[5] != 0) ||
                          (puc_ie[6] != 0xff) || (puc_ie[7] != 0xff));
        if (en_vht_capable == OAL_TRUE) {
            pst_bss_dscr->en_vht_capable = OAL_TRUE; /* ֧��vht */
        }

        /* ˵��vendor��Я��VHT ie�������ñ�־λ��assoc req��Ҳ��Я��vendor+vht ie */
        if (en_is_vendor_ie == OAL_TRUE) {
            pst_bss_dscr->en_vendor_vht_capable = OAL_TRUE;
        }

        /* ��ȡSupported Channel Width Set */
        uc_supp_ch_width = ((puc_ie[2] & (BIT3 | BIT2)) >> 2);

        if (uc_supp_ch_width == 0) {
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_80M; /* 80MHz */
        } else if (uc_supp_ch_width == 1) {
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_160M; /* 160MHz */
        } else if (uc_supp_ch_width == 2) {
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_80M; /* 80MHz */
        }
    } else {
        /* ˽��vendor�в�����vht ie������BCM 5g 20M ˽��Э�� */
        if (en_is_vendor_ie == OAL_TRUE) {
            pst_bss_dscr->en_vendor_novht_capable = OAL_TRUE;
            if ((get_hi110x_subchip_type() != BOARD_VERSION_HI1103) &&
                ((uc_vendor_subtype == MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE2) ||
                (uc_vendor_subtype == MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE4))) {
                pst_bss_dscr->en_vendor_1024qam_capable = OAL_TRUE;
            } else {
                pst_bss_dscr->en_vendor_1024qam_capable = OAL_FALSE;
            }
        }
    }
}


OAL_STATIC void hmac_scan_update_bss_list_11ac(mac_bss_dscr_stru *pst_bss_dscr,
                                               uint8_t *puc_frame_body,
                                               uint16_t us_frame_len,
                                               uint32_t en_is_vendor_ie,
                                               uint8_t uc_vendor_subtype)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint8_t uc_vht_chan_width, uc_chan_center_freq, uc_chan_center_freq_1;

    hmac_scan_update_bss_list_11ac_vht_cap(pst_bss_dscr, puc_frame_body, us_frame_len,
                                           en_is_vendor_ie, uc_vendor_subtype);

    puc_ie = mac_find_ie(MAC_EID_VHT_OPERN, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_VHT_OPERN_LEN)) {
        uc_vht_chan_width = puc_ie[2];
        uc_chan_center_freq = puc_ie[3];
        uc_chan_center_freq_1 = puc_ie[4];

        /* ���´�����Ϣ */
        if (uc_vht_chan_width == 0) { /* 40MHz */
            /* do nothing��en_channel_bandwidth�Ѿ���HT Operation IE�л�ȡ */
        } else if (uc_vht_chan_width == 1) { /* 80MHz */
#ifdef _PRE_WLAN_FEATURE_160M
            if ((uc_chan_center_freq_1 - uc_chan_center_freq == 8) ||
                (uc_chan_center_freq - uc_chan_center_freq_1 == 8)) {
                switch (uc_chan_center_freq_1 - pst_bss_dscr->st_channel.uc_chan_number) {
                    case 14:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSPLUS;
                        break;

                    case 10:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSPLUS;
                        break;

                    case 6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSPLUS;
                        break;

                    case 2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSPLUS;
                        break;

                    case -2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSMINUS;
                        break;

                    case -6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSMINUS;
                        break;

                    case -10:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSMINUS;
                        break;

                    case -14:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSMINUS;
                        break;

                    default:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_20M;
                        break;
                }
                return;
            }
#endif
            switch (uc_chan_center_freq - pst_bss_dscr->st_channel.uc_chan_number) {
                case 6:
                    /***********************************************************************
                    | ��20 | ��20 | ��40       |
                              |
                              |����Ƶ���������20ƫ6���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSPLUS;
                    break;

                case -2:
                    /***********************************************************************
                    | ��40        | ��20 | ��20 |
                              |
                              |����Ƶ���������20ƫ-2���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSMINUS;
                    break;

                case 2:
                    /***********************************************************************
                    | ��20 | ��20 | ��40       |
                              |
                              |����Ƶ���������20ƫ2���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSPLUS;
                    break;

                case -6:
                    /***********************************************************************
                    | ��40        | ��20 | ��20 |
                              |
                              |����Ƶ���������20ƫ-6���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSMINUS;
                    break;

                default:
                    break;
            }
        }
#ifdef _PRE_WLAN_FEATURE_160M
        else if (uc_vht_chan_width == 2) { /* 160MHz */
            switch (uc_chan_center_freq - pst_bss_dscr->st_channel.uc_chan_number) {
                case 14:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSPLUS;
                    break;
                case 10:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSPLUS;
                    break;

                case 6:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSPLUS;
                    break;
                case 2:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSPLUS;
                    break;
                case -2:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSMINUS;
                    break;
                case -6:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSMINUS;
                    break;
                case -10:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSMINUS;
                    break;
                case -14:
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSMINUS;
                    break;
                default:
                    break;
            }
        }
#endif
        else if (uc_vht_chan_width == 3) { /* 80+80MHz */
            switch (uc_chan_center_freq - pst_bss_dscr->st_channel.uc_chan_number) {
                case 6:
                    /***********************************************************************
                    | ��20 | ��20 | ��40       |
                              |
                              |����Ƶ���������20ƫ6���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSPLUS;
                    break;

                case -2:
                    /***********************************************************************
                    | ��40        | ��20 | ��20 |
                              |
                              |����Ƶ���������20ƫ-2���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSMINUS;
                    break;

                case 2:
                    /***********************************************************************
                    | ��20 | ��20 | ��40       |
                              |
                              |����Ƶ���������20ƫ2���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSPLUS;
                    break;

                case -6:
                    /***********************************************************************
                    | ��40        | ��20 | ��20 |
                              |
                              |����Ƶ���������20ƫ-6���ŵ�
                    ************************************************************************/
                    pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSMINUS;
                    break;

                default:
                    break;
            }
          /* Unsupported Channel BandWidth */
        } else {
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC void hmac_scan_update_bw_cap(mac_frame_he_cap_ie_stru *pst_he_cap_value,
    mac_bss_dscr_stru *pst_bss_dscr)
{
    pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_20M;
    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_2G) {              /* 2G */
        if (pst_he_cap_value->st_he_phy_cap.bit_channel_width_set & 0x1) { /* Bit0 2G �Ƿ�֧��40MHz */
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_40M;
        }
    } else {
        if (pst_he_cap_value->st_he_phy_cap.bit_channel_width_set & 0x8) { /* B3-5G 80+80MHz */
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_80PLUS80;
        } else if (pst_he_cap_value->st_he_phy_cap.bit_channel_width_set & 0x4) { /* B2-160MHz */
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_160M;
        } else if (pst_he_cap_value->st_he_phy_cap.bit_channel_width_set & 0x2) { /* B2-5G֧��80MHz */
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_80M;
        } else {
            pst_bss_dscr->en_bw_cap = WLAN_BW_CAP_20M;
        }
    }
}

OAL_STATIC void hmac_scan_update_bss_list_11ax_he_cap(mac_bss_dscr_stru *bss_dscr,
                                                      uint8_t *frame_body,
                                                      uint16_t frame_len)
{
    uint8_t *ie;
    mac_frame_he_cap_ie_stru he_cap_ie;
    uint32_t ret;

    /* HE CAP */
    ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_CAP, frame_body, frame_len);
    if ((ie != OAL_PTR_NULL) && (ie[1] >= MAC_HE_CAP_MIN_LEN)) {
        memset_s(&he_cap_ie, OAL_SIZEOF(he_cap_ie), 0, OAL_SIZEOF(he_cap_ie));
        /* �����̶����Ȳ���:MAC_Cap+PHY Cap + HE-MCS NSS(<=80MHz)  */
        ret = mac_ie_parse_he_cap(ie, &he_cap_ie);
        if (ret != OAL_SUCC) {
            return;
        }

        bss_dscr->en_he_capable = OAL_TRUE; /* ֧��HE */

        bss_dscr->en_he_uora = (he_cap_ie.st_he_mac_cap.bit_ofdma_ra_support & 0x1) ? OAL_TRUE : OAL_FALSE;

        hmac_scan_update_bw_cap(&he_cap_ie, bss_dscr);

        /* ����Զ˲�֧��ldpc������Ϊ���ǽ�Э����11ac */
        if (he_cap_ie.st_he_phy_cap.bit_ldpc_coding_in_paylod == OAL_FALSE) {
            bss_dscr->en_he_capable = OAL_FALSE;
        }

        /* �����û�֧�ַ��͵�txbf������ */
        bss_dscr->uc_num_sounding_dim = (bss_dscr->en_bw_cap <= WLAN_BW_CAP_80M) ?
            he_cap_ie.st_he_phy_cap.bit_below_80mhz_sounding_dimensions_num :
            he_cap_ie.st_he_phy_cap.bit_over_80mhz_sounding_dimensions_num;
    }
}


OAL_STATIC void hmac_scan_update_bss_list_11ax(mac_bss_dscr_stru *pst_bss_dscr,
                                               uint8_t *puc_frame_body,
                                               uint16_t us_frame_len)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    mac_frame_he_oper_ie_stru st_he_oper_ie_value;
    uint32_t ul_ret;

    hmac_scan_update_bss_list_11ax_he_cap(pst_bss_dscr, puc_frame_body, us_frame_len);

    /* HE Oper */
    puc_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_OPERATION, puc_frame_body, us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_HE_OPERAION_MIN_LEN)) {
        memset_s(&st_he_oper_ie_value, OAL_SIZEOF(st_he_oper_ie_value), 0, OAL_SIZEOF(st_he_oper_ie_value));
        ul_ret = mac_ie_parse_he_oper(puc_ie, &st_he_oper_ie_value);
        if (ul_ret != OAL_SUCC) {
            return;
        }

        if (st_he_oper_ie_value.st_he_oper_param.bit_vht_operation_info_present == 1) {
            if (st_he_oper_ie_value.st_vht_operation_info.uc_channel_width == 1) { /* 80MHz */
                switch (st_he_oper_ie_value.st_vht_operation_info.uc_center_freq_seg0 -
                        pst_bss_dscr->st_channel.uc_chan_number) {
                    case 6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSPLUS;
                        break;
                    case -2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80PLUSMINUS;
                        break;
                    case 2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSPLUS;
                        break;
                    case -6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_80MINUSMINUS;
                        break;
                    default:
                        break;
                }
            }
#ifdef _PRE_WLAN_FEATURE_160M
            else if (st_he_oper_ie_value.st_vht_operation_info.uc_channel_width == 2) { /* 160MHz */
                switch (st_he_oper_ie_value.st_vht_operation_info.uc_center_freq_seg0 -
                        pst_bss_dscr->st_channel.uc_chan_number) {
                    case 14:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSPLUS;
                        break;
                    case -2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSPLUSMINUS;
                        break;
                    case 6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSPLUS;
                        break;
                    case -10:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160PLUSMINUSMINUS;
                        break;
                    case 10:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSPLUS;
                        break;
                    case -6:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSPLUSMINUS;
                        break;
                    case 2:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSPLUS;
                        break;
                    case -14:
                        pst_bss_dscr->en_channel_bandwidth = WLAN_BAND_WIDTH_160MINUSMINUSMINUS;
                        break;
                    default:
                        break;
                }
            }
#endif
        }
    }
}


OAL_STATIC OAL_INLINE void hmac_scan_update_11ax_ie(hmac_vap_stru *pst_hmac_vap,
                                                    mac_bss_dscr_stru *pst_bss_dscr,
                                                    uint8_t *puc_frame_body,
                                                    uint16_t us_frame_len)
{
    if (IS_CUSTOM_OPEN_11AX_SWITCH(&pst_hmac_vap->st_vap_base_info)) {
        hmac_scan_update_bss_list_11ax(pst_bss_dscr, puc_frame_body, us_frame_len);
    }

    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        pst_bss_dscr->en_punctured_preamble = mac_hiex_ie_get_bit_chip_type(puc_frame_body, us_frame_len);
    }
}
#endif

OAL_STATIC OAL_INLINE void hmac_scan_update_hisi_cap_ie(hmac_vap_stru *pst_hmac_vap,
    mac_bss_dscr_stru *pst_bss_dscr, uint8_t *puc_frame_body, uint16_t us_frame_len)
{
    mac_hisi_cap_vendor_ie_stru st_hisi_vap_ie;

    if (mac_get_hisi_cap_vendor_ie(puc_frame_body, us_frame_len, &st_hisi_vap_ie) == OAL_SUCC) {
        if (st_hisi_vap_ie.bit_11ax_support != OAL_FALSE) {
            pst_bss_dscr->en_he_capable  = st_hisi_vap_ie.bit_11ax_support;
        }
        pst_bss_dscr->en_dcm_support = st_hisi_vap_ie.bit_dcm_support;
        pst_bss_dscr->en_p2p_scenes  = st_hisi_vap_ie.bit_p2p_scenes;
    }
}


OAL_STATIC OAL_INLINE void hmac_scan_update_cap_ie_part1(hmac_vap_stru *pst_hmac_vap,
    mac_bss_dscr_stru *pst_bss_dscr, uint8_t *puc_frame_body, uint16_t us_frame_len)
{
    /* 11ax */
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_scan_update_11ax_ie(pst_hmac_vap, pst_bss_dscr, puc_frame_body, us_frame_len);
    }
#endif
    hmac_scan_update_hisi_cap_ie(pst_hmac_vap, pst_bss_dscr, puc_frame_body, us_frame_len);
}



void hmac_scan_btcoex_backlist_check_by_oui(mac_bss_dscr_stru *pst_bss_dscr,
                                            uint8_t *puc_frame_body,
                                            uint16_t us_frame_len)
{
    /* ��ʼ��Ϊ�Ǻ����� */
    pst_bss_dscr->en_btcoex_blacklist_chip_oui = 0;

    if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_RALINK,
                           MAC_WLAN_CHIP_OUI_TYPE_RALINK,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL ||
        mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_SHENZHEN,
                           MAC_WLAN_CHIP_OUI_TYPE_SHENZHEN,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL ||
        mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_BROADCOM,
                           MAC_WLAN_CHIP_OUI_TYPE_BROADCOM,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL ||
        mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_ATHEROSC,
                           MAC_WLAN_CHIP_OUI_TYPE_ATHEROSC,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL ||
        mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_REALTEK,
                           MAC_WLAN_CHIP_OUI_TYPE_REALTEK,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL ||
        mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_METALINK,
                           MAC_WLAN_CHIP_OUI_TYPE_METALINK,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_btcoex_blacklist_chip_oui |= MAC_BTCOEX_BLACKLIST_LEV0;
    }

    if (((mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_APPLE1,
                             MAC_WLAN_CHIP_OUI_TYPE_APPLE_1_1,
                             puc_frame_body, us_frame_len) != OAL_PTR_NULL) ||
         (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_APPLE1,
                             MAC_WLAN_CHIP_OUI_TYPE_APPLE_1_2,
                             puc_frame_body, us_frame_len) != OAL_PTR_NULL) ||
         (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_APPLE2,
                             MAC_WLAN_CHIP_OUI_TYPE_APPLE_2_1,
                             puc_frame_body, us_frame_len) != OAL_PTR_NULL)) &&
        (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_BROADCOM,
                            MAC_WLAN_CHIP_OUI_TYPE_BROADCOM,
                            puc_frame_body, us_frame_len) != OAL_PTR_NULL)) {
        pst_bss_dscr->en_btcoex_blacklist_chip_oui |= MAC_BTCOEX_BLACKLIST_LEV1;
    }
}

OAL_STATIC OAL_INLINE void hmac_scan_bss_chip_oui(mac_bss_dscr_stru *pst_bss_dscr,
    uint8_t *puc_frame_body, uint16_t us_frame_len)
{
    if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_RALINK,
                           MAC_WLAN_CHIP_OUI_TYPE_RALINK,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_is_tplink_oui = WLAN_AP_CHIP_OUI_RALINK;
    } else if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_BROADCOM,
                                  MAC_WLAN_CHIP_OUI_TYPE_BROADCOM,
                                  puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_is_tplink_oui = WLAN_AP_CHIP_OUI_BCM;
    } else if (mac_find_vendor_ie(
        MAC_WLAN_CHIP_OUI_QUALCOMM, MAC_WLAN_CHIP_OUI_TYPE_QUALCOMM, puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_is_tplink_oui = WLAN_AP_CHIP_OUI_QLM;
    }
}


OAL_STATIC OAL_INLINE void hmac_scan_update_bss_list_protocol(hmac_vap_stru *pst_hmac_vap,
                                                              mac_bss_dscr_stru *pst_bss_dscr,
                                                              uint8_t *puc_frame_body,
                                                              uint16_t us_frame_len)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint16_t us_offset_vendor_vht = MAC_WLAN_OUI_VENDOR_VHT_HEADER + MAC_IE_HDR_LEN;

    puc_frame_body += MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    us_frame_len -= MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    /*************************************************************************/
    /*                       Beacon Frame - Frame Body                       */
    /* ---------------------------------------------------------------------- */
    /* |Timestamp|BcnInt|CapInfo|SSID|SupRates|DSParamSet|TIM  |CountryElem | */
    /* ---------------------------------------------------------------------- */
    /* |8        |2     |2      |2-34|3-10    |3         |6-256|8-256       | */
    /* ---------------------------------------------------------------------- */
    /* |PowerConstraint |Quiet|TPC Report|ERP |RSN  |WMM |Extended Sup Rates| */
    /* ---------------------------------------------------------------------- */
    /* |3               |8    |4         |3   |4-255|26  | 3-257            | */
    /* ---------------------------------------------------------------------- */
    /* |BSS Load |HT Capabilities |HT Operation |Overlapping BSS Scan       | */
    /* ---------------------------------------------------------------------- */
    /* |7        |28              |24           |16                         | */
    /* ---------------------------------------------------------------------- */
    /* |Extended Capabilities |                                              */
    /* ---------------------------------------------------------------------- */
    /* |3-8                   |                                              */
    /*************************************************************************/
    /* wmm */
    hmac_scan_update_bss_list_wmm(pst_bss_dscr, puc_frame_body, us_frame_len);

    /* 11i */
    hmac_scan_update_11i(pst_bss_dscr, puc_frame_body, us_frame_len);

#ifdef _PRE_WLAN_FEATURE_11D
    /* 11d */
    hmac_scan_update_bss_list_country(pst_bss_dscr, puc_frame_body, us_frame_len);
#endif

    /* 11n */
    hmac_scan_update_bss_list_11n(pst_bss_dscr, puc_frame_body, us_frame_len);

    /* rrm */
#if defined(_PRE_WLAN_FEATURE_11K) || \
    defined(_PRE_WLAN_FEATURE_FTM)
    hmac_scan_update_bss_list_rrm(pst_bss_dscr, puc_frame_body, us_frame_len);
#endif

    /* 11ac */
    hmac_scan_update_bss_list_11ac(pst_bss_dscr, puc_frame_body, us_frame_len, OAL_FALSE, OAL_FALSE);

    hmac_scan_update_cap_ie_part1(pst_hmac_vap, pst_bss_dscr, puc_frame_body, us_frame_len);
    /* ����˽��vendor ie */
    puc_ie = mac_find_vendor_ie(MAC_WLAN_OUI_BROADCOM_EPIGRAM,
                                MAC_WLAN_OUI_VENDOR_VHT_TYPE,
                                puc_frame_body,
                                us_frame_len);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_WLAN_OUI_VENDOR_VHT_HEADER)) {
        hmac_scan_update_bss_list_11ac(pst_bss_dscr, puc_ie + us_offset_vendor_vht,
                                       puc_ie[1] - MAC_WLAN_OUI_VENDOR_VHT_HEADER, OAL_TRUE, puc_ie[6]);
    }

    /* nb */
#ifdef _PRE_WLAN_NARROW_BAND
    hmac_scan_update_bss_list_nb(pst_bss_dscr, puc_frame_body, us_frame_len);
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM
    hmac_scan_update_bss_list_1024qam(pst_bss_dscr, puc_frame_body, us_frame_len);
#endif

#ifdef _PRE_WLAN_FEATURE_MBO
    /* ���AP�Ƿ������assoc disallowed attr��MBO IE��MBO���ƻ����ش򿪲Ž��в��Ҷ�ӦMBO IE,������� */
    if (pst_hmac_vap->st_vap_base_info.st_mbo_para_info.uc_mbo_enable == OAL_TRUE) {
        hmac_scan_update_bss_assoc_disallowed_attr(pst_bss_dscr, puc_frame_body, us_frame_len);
    }
#endif

    /* nb */
#ifdef _PRE_WLAN_NARROW_BAND
    hmac_scan_update_bss_list_nb(pst_bss_dscr, puc_frame_body, us_frame_len);
#endif

    if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_ATHEROSC, MAC_WLAN_CHIP_OUI_TYPE_ATHEROSC,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_atheros_chip_oui = OAL_TRUE;
    } else {
        pst_bss_dscr->en_atheros_chip_oui = OAL_FALSE;
    }

    hmac_scan_btcoex_backlist_check_by_oui(pst_bss_dscr, puc_frame_body, us_frame_len);

    if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_BROADCOM, MAC_WLAN_CHIP_OUI_TYPE_BROADCOM,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_roam_blacklist_chip_oui = OAL_TRUE;
    } else {
        pst_bss_dscr->en_roam_blacklist_chip_oui = OAL_FALSE;
    }

    if (mac_find_vendor_ie(MAC_WLAN_CHIP_OUI_MARVELL, MAC_WLAN_CHIP_OUI_TYPE_MARVELL,
                           puc_frame_body, us_frame_len) != OAL_PTR_NULL) {
        pst_bss_dscr->en_txbf_blacklist_chip_oui = OAL_TRUE;
    } else {
        pst_bss_dscr->en_txbf_blacklist_chip_oui = OAL_FALSE;
    }

    hmac_scan_bss_chip_oui(pst_bss_dscr, puc_frame_body, us_frame_len);
}


uint8_t hmac_scan_check_bss_supp_rates(mac_device_stru *pst_mac_dev,
                                       uint8_t *puc_rate,
                                       uint8_t uc_bss_rate_num,
                                       uint8_t *puc_update_rate,
                                       uint8_t uc_rate_size)
{
    mac_data_rate_stru *pst_rates = OAL_PTR_NULL;
    uint32_t i, j, k;
    uint8_t uc_rate_num = 0;

    if (uc_rate_size > WLAN_USER_MAX_SUPP_RATES) {
        oam_error_log1(0, OAM_SF_SCAN, "{hmac_scan_get_scanned_bss_by_bssid::uc_rate_size err[%d].}", uc_rate_size);
        return uc_rate_num;
    }

    pst_rates = mac_device_get_all_rates(pst_mac_dev);

    for (i = 0; i < uc_bss_rate_num; i++) {
        for (j = 0; j < MAC_DATARATES_PHY_80211G_NUM; j++) {
            if ((IS_EQUAL_RATES(pst_rates[j].uc_mac_rate, puc_rate[i]))
                && (uc_rate_num < MAC_DATARATES_PHY_80211G_NUM)) {
                /* ȥ���ظ����� */
                for (k = 0; k < uc_rate_num; k++) {
                    if (IS_EQUAL_RATES(puc_update_rate[k], puc_rate[i])) {
                        break;
                    }
                }
                /* ���������ظ�����ʱ��k����uc_rate_num */
                if (k == uc_rate_num) {
                    puc_update_rate[uc_rate_num++] = puc_rate[i];
                }
                break;
            }
        }
    }

    return uc_rate_num;
}


void hmac_scan_rm_repeat_sup_exsup_rates(mac_bss_dscr_stru *pst_bss_dscr,
                                         uint8_t *puc_rates,
                                         uint8_t uc_exrate_num)
{
    int i, j;
    for (i = 0; i < uc_exrate_num; i++) {
        /* ȥ���ظ����� */
        for (j = 0; j < pst_bss_dscr->uc_num_supp_rates; j++) {
            if (IS_EQUAL_RATES(puc_rates[i], pst_bss_dscr->auc_supp_rates[j])) {
                break;
            }
        }

        /* ֻ�в������ظ�����ʱ��j����pst_bss_dscr->uc_num_supp_rates */
        if (j == pst_bss_dscr->uc_num_supp_rates && pst_bss_dscr->uc_num_supp_rates < WLAN_USER_MAX_SUPP_RATES) {
            pst_bss_dscr->auc_supp_rates[pst_bss_dscr->uc_num_supp_rates++] = puc_rates[i];
        }
    }
}


#define MAC_DATARATES_80211B_NUM     4
OAL_STATIC void hmac_scan_remove_11b_rate(uint8_t *puc_avail_rate, uint8_t *puc_rate_num)
{
    /* �������ʴ�С��IE�б���Ϊԭ��ֵ��С��2��11b���ʴ�СΪ1, 2, 5.5, 11 */
    uint8_t auc_11b_rate[MAC_DATARATES_80211B_NUM] = { 2, 4, 11, 22 };
    uint8_t auc_target_rate[WLAN_USER_MAX_SUPP_RATES] = { 0 };
    uint8_t uc_target_rate_num = 0;
    oal_bool_enum_uint8 en_is_11b_rate;
    uint8_t i, j;

    for (i = 0; i < *puc_rate_num; i++) {
        en_is_11b_rate = OAL_FALSE;

        for (j = 0; j < MAC_DATARATES_80211B_NUM; j++) {
            if (IS_EQUAL_RATES(puc_avail_rate[i], auc_11b_rate[j])) {
                en_is_11b_rate = OAL_TRUE;
                break;
            }
        }

        if (!en_is_11b_rate) {
            auc_target_rate[uc_target_rate_num++] = puc_avail_rate[i];
        }
    }

    /* copyԭ���ʼ����ȣ�������պ������������� */
    if (memcpy_s(puc_avail_rate, WLAN_USER_MAX_SUPP_RATES, auc_target_rate, *puc_rate_num) == EOK) {
        *puc_rate_num = uc_target_rate_num;
        return;
    }
    oam_error_log0(0, OAM_SF_ANY, "hmac_scan_remove_11b_rate::memcpy fail!");
}


OAL_STATIC OAL_INLINE uint32_t hmac_scan_update_bss_list_rates(mac_bss_dscr_stru *pst_bss_dscr,
                                                               uint8_t *puc_frame_body,
                                                               uint16_t us_frame_len,
                                                               mac_device_stru *pst_mac_dev)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint8_t uc_num_rates = 0;
    uint8_t uc_num_ex_rates;
    uint8_t us_offset;
    uint8_t auc_rates[WLAN_USER_MAX_SUPP_RATES] = { 0 };

    /* ����Beacon֡��fieldƫ���� */
    us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    puc_ie = mac_find_ie(MAC_EID_RATES, puc_frame_body + us_offset, us_frame_len - us_offset);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_rates = hmac_scan_check_bss_supp_rates(pst_mac_dev, puc_ie + MAC_IE_HDR_LEN, puc_ie[1],
                                                          auc_rates, sizeof(auc_rates));
        
        if (uc_num_rates > WLAN_USER_MAX_SUPP_RATES) {
            oam_warning_log1(0, OAM_SF_SCAN, "{hmac_scan_update_bss_list_rates::uc_num_rates=%d.}", uc_num_rates);
            uc_num_rates = WLAN_USER_MAX_SUPP_RATES;
        }

        if (memcpy_s(pst_bss_dscr->auc_supp_rates, WLAN_USER_MAX_SUPP_RATES, auc_rates, uc_num_rates) != EOK) {
            oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_update_bss_list_rates::memcpy fail!");
            return OAL_FAIL;
        }

        pst_bss_dscr->uc_num_supp_rates = uc_num_rates;
    }

    puc_ie = mac_find_ie(MAC_EID_XRATES, puc_frame_body + us_offset, us_frame_len - us_offset);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_ex_rates = hmac_scan_check_bss_supp_rates(pst_mac_dev, puc_ie + MAC_IE_HDR_LEN, puc_ie[1],
                                                             auc_rates, sizeof(auc_rates));
        if (uc_num_rates + uc_num_ex_rates > WLAN_USER_MAX_SUPP_RATES) { /* ����֧�����ʸ��� */
            oam_warning_log2(0, OAM_SF_SCAN,
                "{hmac_scan_update_bss_list_rates::number of rates too large, uc_num_rates=%d, uc_num_ex_rates=%d.}",
                uc_num_rates, uc_num_ex_rates);
        }

        if (uc_num_ex_rates > 0) {
            /* support_rates��extended_ratesȥ���ظ����ʣ�һ������ɨ���������ʼ��� */
            hmac_scan_rm_repeat_sup_exsup_rates(pst_bss_dscr, auc_rates, uc_num_ex_rates);
        }
    }

    if (pst_bss_dscr->st_channel.en_band == WLAN_BAND_5G) {
        hmac_scan_remove_11b_rate(pst_bss_dscr->auc_supp_rates, &pst_bss_dscr->uc_num_supp_rates);
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R

uint32_t mac_ie_get_rsn_akm_tail_pos(uint8_t *puc_ie,
                                     int32_t *pul_akm_tail_offset,
                                     int32_t *pul_akm_cnt_offset)
{
    uint8_t *puc_ie_bak;
    uint16_t us_suites_count;
    uint16_t us_idx;
    uint32_t ul_len;

    if (puc_ie == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    /*************************************************************************/
    /*                  RSN Element Format                                   */
    /* --------------------------------------------------------------------- */
    /* |Element ID | Length | Version | Group Cipher Suite | Pairwise Suite */
    /* --------------------------------------------------------------------- */
    /* |     1     |    1   |    2    |         4          |       2         */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /*  Count| Pairwise Cipher Suite List | AKM Suite Count | AKM Suite List */
    /* --------------------------------------------------------------------- */
    /*       |         4*m                |     2           |   4*n          */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /* |RSN Capabilities|PMKID Count|PMKID List|Group Management Cipher Suite */
    /* --------------------------------------------------------------------- */
    /* |        2       |    2      |   16 *s  |               4           | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/

    puc_ie_bak = puc_ie;

    puc_ie_bak += 1 + 1 + 2;

    /* Group Cipher Suite */
    puc_ie_bak += 4;

    us_suites_count = puc_ie_bak[0] + puc_ie_bak[1] * 8;
    puc_ie_bak += 2;

    /* Pairwise Cipher Suite ����2�� */
    for (us_idx = 0; us_idx < us_suites_count; us_idx++) {
        puc_ie_bak += 4;
    }

    ul_len = (uint32_t)(puc_ie_bak - puc_ie);
    if (ul_len > puc_ie[1] + 2) {
        return OAL_FAIL;
    }

    *pul_akm_cnt_offset = (int32_t)(puc_ie_bak - puc_ie);

    us_suites_count = puc_ie_bak[0] + puc_ie_bak[1] * 8;
    puc_ie_bak += 2;

    /* AKM Suite ����2�� */
    for (us_idx = 0; us_idx < us_suites_count; us_idx++) {
        puc_ie_bak += 4;
    }

    ul_len = (uint32_t)(puc_ie_bak - puc_ie);
    if (ul_len > puc_ie[1] + 2) {
        return OAL_FAIL;
    }

    *pul_akm_tail_offset = (int32_t)(puc_ie_bak - puc_ie);

    return OAL_SUCC;
}


int32_t hmac_scan_attach_akm_suite_to_rsn_ie(uint8_t *puc_mgmt_frame, uint16_t *pus_frame_len)
{
    uint8_t *puc_tagged_para;
    uint32_t ul_tagged_para_len;
    uint8_t *puc_rsn_ie;
    uint8_t *puc_cowork_ie;
    uint8_t *puc_md_ie;
    uint32_t ul_rsn_ie_len;
    uint8_t *puc_rsn_tail;
    uint32_t ul_rsn_tail_len;
    uint32_t ul_frm_len_before_rsn;
    uint32_t ul_frm_len_after_rsn;
    uint32_t ul_rsn_ie_akm_offset = 0;
    uint32_t ul_rsn_ie_akm_cnt_offset = 0;
    uint32_t ul_rsn_ie_end_pos;
    uint8_t *puc_rsn_ie_bak;
    uint32_t ul_idx;
    uint32_t ul_ret;
    int32_t l_ret;
    oal_sta_ap_cowork_ie_beacon *pst_cowork_ie;
    oal_cowork_md_ie st_md_ie;
    oal_sta_ap_cowork_akm_para_stru akm_suite_attatch[4] = {
        { MAC_RSN_AKM_FT_OVER_8021X_VAL,        MAC_RSN_AKM_FT_OVER_8021X },
        { MAC_RSN_AKM_FT_PSK_VAL,               MAC_RSN_AKM_FT_PSK },
        { MAC_RSN_AKM_FT_OVER_SAE_VAL,          MAC_RSN_AKM_FT_OVER_SAE },
        { MAC_RSN_AKM_FT_OVER_8021X_SHA384_VAL, MAC_RSN_AKM_FT_OVER_8021X_SHA384 }
    };

    if ((((mac_ieee80211_frame_stru *)puc_mgmt_frame)->st_frame_control.bit_sub_type != WLAN_BEACON) &&
        (((mac_ieee80211_frame_stru *)puc_mgmt_frame)->st_frame_control.bit_sub_type != WLAN_PROBE_RSP)) {
        return OAL_SUCC;
    }

    /* �ҵ� RSN IE ��λ�� puc_rsn_ie  */
    puc_tagged_para = puc_mgmt_frame + MAC_80211_FRAME_LEN +
        (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    ul_tagged_para_len = *pus_frame_len - MAC_80211_FRAME_LEN -
        (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);

    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_tagged_para, ul_tagged_para_len);
    puc_cowork_ie = mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI,
                                           MAC_WLAN_OUI_TYPE_HAUWEI_COWORK, puc_tagged_para, ul_tagged_para_len);
    if ((puc_rsn_ie == OAL_PTR_NULL) || (puc_cowork_ie == OAL_PTR_NULL)) {
        return OAL_SUCC;
    }

    /* �� netbuf ��Ϊ����: ǰ��һ���� 80211֡ͷ��RSN IE(����), ����һ����RSN IE(��)�Ժ�� */
    ul_frm_len_before_rsn = (puc_rsn_ie - puc_mgmt_frame);
    ul_frm_len_after_rsn = *pus_frame_len - ul_frm_len_before_rsn;

    /* RSN IE(����ul_rsn_ie_len)�� RSN IE���������(����ul_rsn_tail_len)��ͬ��ɺ���һ�� */
    ul_rsn_ie_len = puc_rsn_ie[1] + 2;
    ul_rsn_tail_len = ul_frm_len_after_rsn - ul_rsn_ie_len;

    /* �ݴ�RSN IE���������(����ul_rsn_tail_len) */
    puc_rsn_ie_bak = puc_rsn_ie;
    ul_ret = mac_ie_get_rsn_akm_tail_pos(puc_rsn_ie_bak, &ul_rsn_ie_akm_offset, &ul_rsn_ie_akm_cnt_offset);
    if (ul_ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    puc_rsn_tail = (int8_t *)kmalloc(ul_rsn_tail_len + (ul_rsn_ie_len - ul_rsn_ie_akm_offset), GFP_KERNEL);
    if (oal_unlikely(puc_rsn_tail == NULL)) {
        return OAL_FAIL;
    }
    l_ret = memcpy_s(puc_rsn_tail, ul_rsn_tail_len + (ul_rsn_ie_len - ul_rsn_ie_akm_offset),
                     (int8_t *)(puc_rsn_ie + ul_rsn_ie_akm_offset),
                     ul_rsn_tail_len + (ul_rsn_ie_len - ul_rsn_ie_akm_offset));

    /* ���� AKM SUITE(FT) */
    pst_cowork_ie = (oal_sta_ap_cowork_ie_beacon *)puc_cowork_ie;
    for (ul_idx = 0; ul_idx < sizeof(akm_suite_attatch) / sizeof(akm_suite_attatch[0]); ul_idx++) {
        if ((pst_cowork_ie->capa_bitmap.akm_suite_val & 0xff) == akm_suite_attatch[ul_idx].real_akm_val) {
            l_ret += memcpy_s(&(puc_rsn_ie[ul_rsn_ie_akm_offset]), sizeof(akm_suite_attatch[ul_idx].akm_suite),
                              &(akm_suite_attatch[ul_idx].akm_suite), sizeof(akm_suite_attatch[ul_idx].akm_suite));
            break;
        } else if (ul_idx == sizeof(akm_suite_attatch) / sizeof(akm_suite_attatch[0]) - 1) {
            kfree(puc_rsn_tail);
            return OAL_FAIL;
        }
    }

    /* ���� AKM COUNT �ֶ�Ϊ 2 */
    puc_rsn_ie[ul_rsn_ie_akm_cnt_offset] = 0x02;

    /* ���� RSN IE �ĳ����ֶ� */
    puc_rsn_ie[1] += 4;

    puc_md_ie = mac_find_ie(MAC_EID_MOBILITY_DOMAIN, puc_tagged_para, ul_tagged_para_len);
    if (puc_md_ie != OAL_PTR_NULL) {
        /* �Ѵ�ǰ���浽 puc_rsn_tail �е����ݿ��������ӵ� AKM SUITE ֮�� */
        l_ret += memcpy_s(&(puc_rsn_ie[ul_rsn_ie_akm_offset + 4]),
                          ul_rsn_tail_len + (ul_rsn_ie_len - ul_rsn_ie_akm_offset),
                          puc_rsn_tail, ul_rsn_tail_len + (ul_rsn_ie_len - ul_rsn_ie_akm_offset));

        /* ���´�����ά���Ĺ���֡�ĳ����ֶ� */
        (*pus_frame_len) += 4;

        kfree(puc_rsn_tail);
        return OAL_SUCC;
    }

    /* ��ǰ����֡��Я�� MDIE, ����ݻ�ͨIE ���� MDIE �����丽�� RSN IE֮�� */
    memset_s(&st_md_ie, OAL_SIZEOF(st_md_ie), 0, OAL_SIZEOF(st_md_ie));
    st_md_ie.tag_num = MAC_EID_MOBILITY_DOMAIN;
    st_md_ie.tag_len = 0x03;
    if (pst_cowork_ie->capa_bitmap.mdid_flag != 0) {
        st_md_ie.mdid = pst_cowork_ie->capa_bitmap.mdid;
    }
    st_md_ie.support_res_req_proto = pst_cowork_ie->capa_bitmap.support_res_req_proto;
    st_md_ie.ft_over_ds = pst_cowork_ie->capa_bitmap.ft_over_ds;

    ul_rsn_ie_end_pos = ul_rsn_ie_akm_offset + 4;
    l_ret += memcpy_s(&(puc_rsn_ie[ul_rsn_ie_end_pos]), (ul_rsn_ie_len - ul_rsn_ie_akm_offset),
                      puc_rsn_tail, (ul_rsn_ie_len - ul_rsn_ie_akm_offset));

    ul_rsn_ie_end_pos += (ul_rsn_ie_len - ul_rsn_ie_akm_offset);

    l_ret += memcpy_s(&puc_rsn_ie[ul_rsn_ie_end_pos], sizeof(st_md_ie), &st_md_ie, sizeof(st_md_ie));
    ul_rsn_ie_end_pos += sizeof(st_md_ie);

    l_ret += memcpy_s(&(puc_rsn_ie[ul_rsn_ie_end_pos]), ul_rsn_tail_len,
                      puc_rsn_tail + (ul_rsn_ie_len - ul_rsn_ie_akm_offset), ul_rsn_tail_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_scan_attach_akm_suite_to_rsn_ie::memcpy fail!");
        kfree(puc_rsn_tail);
        return OAL_FAIL;
    }

    (*pus_frame_len) += (4 + 5);
    kfree(puc_rsn_tail);

    return OAL_SUCC;
}


uint8_t hmac_scan_extend_mgmt_len_needed(oal_netbuf_stru *pst_netbuf, uint16_t us_netbuf_len)
{
    uint8_t *puc_mgmt_frame = (uint8_t *)oal_netbuf_data(pst_netbuf);
    uint16_t us_frame_len;
    uint16_t us_len_offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    const uint8_t uc_akm_val = DEFAULT_AKM_VALUE;
    uint8_t *puc_tagged_para;
    uint32_t ul_tagged_para_len;
    uint8_t *puc_rsn_ie;
    uint8_t *puc_rsn_ie_bak;
    uint8_t *puc_md_ie;
    uint8_t *puc_cowork_ie;
    uint32_t ul_idx;
    uint32_t ul_rsn_ie_akm_offset = 0;
    uint32_t ul_rsn_ie_akm_cnt_offset = 0;
    uint32_t ul_ret;
    oal_sta_ap_cowork_ie_beacon *pst_cowork_ie;
    oal_sta_ap_cowork_akm_para_stru akm_suite_attatch[4] = {
        { MAC_RSN_AKM_FT_OVER_8021X_VAL,        MAC_RSN_AKM_FT_OVER_8021X },
        { MAC_RSN_AKM_FT_PSK_VAL,               MAC_RSN_AKM_FT_PSK },
        { MAC_RSN_AKM_FT_OVER_SAE_VAL,          MAC_RSN_AKM_FT_OVER_SAE },
        { MAC_RSN_AKM_FT_OVER_8021X_SHA384_VAL, MAC_RSN_AKM_FT_OVER_8021X_SHA384 }
    };

    if (us_netbuf_len < (us_len_offset + OAL_SIZEOF(mac_scanned_result_extend_info_stru))) {
        oam_warning_log1(0, OAM_SF_SCAN, "{hmac_scan_extend_mgmt_len_needed:: netbuf len[%d] err.}", us_netbuf_len);
        return uc_akm_val;
    }

    us_frame_len = us_netbuf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru);
    puc_tagged_para = puc_mgmt_frame + us_len_offset;
    ul_tagged_para_len = us_frame_len - us_len_offset;

    puc_cowork_ie = mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI,
                                       MAC_WLAN_OUI_TYPE_HAUWEI_COWORK, puc_tagged_para, ul_tagged_para_len);
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_tagged_para, ul_tagged_para_len);
    puc_md_ie = mac_find_ie(MAC_EID_MOBILITY_DOMAIN, puc_tagged_para, ul_tagged_para_len);

    /* ����֮һ: ����Я����ͨIE��RSN IE */
    if ((puc_cowork_ie == OAL_PTR_NULL) || (puc_rsn_ie == OAL_PTR_NULL)) {
        return uc_akm_val;
    }

    /* ����֮��: ��ͨIE�� Adaptive 11r�ֶα���ʹ�� */
    pst_cowork_ie = (oal_sta_ap_cowork_ie_beacon *)puc_cowork_ie;
    if (pst_cowork_ie->capa_bitmap.support_adaptive_11r == 0) {
        return uc_akm_val;
    }

    /* ����֮��: RSN IE�� AKM SUITE COUNT ����Ϊ 1 */
    puc_rsn_ie_bak = puc_rsn_ie;
    ul_ret = mac_ie_get_rsn_akm_tail_pos(puc_rsn_ie_bak, &ul_rsn_ie_akm_offset, &ul_rsn_ie_akm_cnt_offset);
    if ((ul_ret != OAL_SUCC) || (puc_rsn_ie[ul_rsn_ie_akm_cnt_offset] != 0x01) ||
        (puc_rsn_ie[ul_rsn_ie_akm_cnt_offset + 1] != 0x00)) {
        return uc_akm_val;
    }

    /* ����֮��: RSN IE��AKM SUITE���벻֧��11R */
    for (ul_idx = 0; ul_idx < sizeof(akm_suite_attatch) / sizeof(akm_suite_attatch[0]); ul_idx++) {
        if (akm_suite_attatch[ul_idx].akm_suite == CIPHER_SUITE_SELECTOR(puc_rsn_ie[ul_rsn_ie_akm_offset - 4],
            puc_rsn_ie[ul_rsn_ie_akm_offset - 3], puc_rsn_ie[ul_rsn_ie_akm_offset - 2],
            puc_rsn_ie[ul_rsn_ie_akm_offset - 1])) {
            return uc_akm_val;
        }
    }

    /* ����֮��: ��ͨIE����Я���� cowork_ie.capa_bitmap.akm_suite_val����֧��FT */
    for (ul_idx = 0; ul_idx < sizeof(akm_suite_attatch) / sizeof(akm_suite_attatch[0]); ul_idx++) {
        if ((pst_cowork_ie->capa_bitmap.akm_suite_val & 0xff) == akm_suite_attatch[ul_idx].real_akm_val) {
            /* �������֡Я�� MDIE, ��ֻ����չ AKM Suite �ĳ���; ��������չ (AKM Suite + MDIE) �ĳ��� */
            return ((puc_md_ie == OAL_PTR_NULL) ? (4 + OAL_SIZEOF(oal_cowork_md_ie)) : 4);
        }
    }

    return uc_akm_val;
}

#endif

oal_bool_enum_uint8 hmac_scan_is_hidden_ssid(uint8_t uc_vap_id,
                                             hmac_scanned_bss_info *pst_new_bss,
                                             hmac_scanned_bss_info *pst_old_bss)
{
    if ((pst_new_bss->st_bss_dscr_info.ac_ssid[0] == '\0') &&
        (pst_old_bss->st_bss_dscr_info.ac_ssid[0] != '\0')) {
        /*  ����SSID,���������AP��Ϣ,��ssid��Ϊ��,�˴�ͨ��BEACON֡ɨ�赽��AP��Ϣ,��SSIDΪ��,�򲻽��и��� */
        oam_warning_log3(uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_is_hidden_ssid::find hide ssid:%.2x:%.2x:%.2x,ignore this update.}",
                         pst_new_bss->st_bss_dscr_info.auc_bssid[3],
                         pst_new_bss->st_bss_dscr_info.auc_bssid[4],
                         pst_new_bss->st_bss_dscr_info.auc_bssid[5]);
        return OAL_TRUE;
    }
    return OAL_FALSE;
}


oal_bool_enum_uint8 hmac_scan_need_update_old_scan_result(hmac_vap_stru *pst_hmac_vap,
                                                          hmac_scanned_bss_info *pst_new_bss,
                                                          hmac_scanned_bss_info *pst_old_bss)
{
    hmac_roam_info_stru *pst_roam_info = OAL_PTR_NULL;
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    pst_roam_info = (hmac_roam_info_stru *)pst_hmac_vap->pul_roam_info;
    if (pst_roam_info != OAL_PTR_NULL) {
        pst_scan_params = &pst_roam_info->st_scan_h2d_params.st_scan_params;
        if (pst_scan_params == OAL_PTR_NULL) {
            return OAL_FALSE;
        }

        /* 11k 11v ������Ҫʹ������ɨ������ѡ�����RSSI AP */
        if (pst_scan_params->uc_neighbor_report_process_flag || pst_scan_params->uc_bss_transition_process_flag) {
            return OAL_TRUE;
        }
    }

    
    if ((((mac_ieee80211_frame_stru *)pst_old_bss->st_bss_dscr_info.auc_mgmt_buff)->st_frame_control.bit_sub_type ==
        WLAN_PROBE_RSP) &&
        (((mac_ieee80211_frame_stru *)pst_new_bss->st_bss_dscr_info.auc_mgmt_buff)->st_frame_control.bit_sub_type ==
        WLAN_BEACON) &&
        (pst_old_bss->st_bss_dscr_info.en_new_scan_bss == OAL_TRUE)) {
        return OAL_FALSE;
    }
    return OAL_TRUE;
}


uint8_t hmac_scan_check_chan(oal_netbuf_stru *pst_netbuf, hmac_scanned_bss_info *pst_scanned_bss)
{
    dmac_rx_ctl_stru *pst_rx_ctrl;
    uint8_t uc_curr_chan;
    uint8_t *puc_frame_body;
    uint16_t us_frame_body_len;
    uint16_t us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    uint8_t *puc_ie_start_addr;
    uint8_t uc_chan_num;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    uc_curr_chan = pst_rx_ctrl->st_rx_info.uc_channel_number;
    puc_frame_body = pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff + MAC_80211_FRAME_LEN;
    us_frame_body_len = pst_scanned_bss->st_bss_dscr_info.ul_mgmt_len - MAC_80211_FRAME_LEN;

    /* ��DSSS Param set ie�н���chan num */
    puc_ie_start_addr = mac_find_ie(MAC_EID_DSPARMS, puc_frame_body + us_offset, us_frame_body_len - us_offset);
    if ((puc_ie_start_addr != OAL_PTR_NULL) && (puc_ie_start_addr[1] == MAC_DSPARMS_LEN)) {
        uc_chan_num = puc_ie_start_addr[2];
        if (mac_is_channel_num_valid(mac_get_band_by_channel_num(uc_chan_num), uc_chan_num) != OAL_SUCC) {
            return OAL_FALSE;
        }
    }

    /* ��HT operation ie�н��� chan num */
    puc_ie_start_addr = mac_find_ie(MAC_EID_HT_OPERATION, puc_frame_body + us_offset,
                                    us_frame_body_len - us_offset);
    if ((puc_ie_start_addr != OAL_PTR_NULL) && (puc_ie_start_addr[1] >= 1)) {
        uc_chan_num = puc_ie_start_addr[2];
        if (mac_is_channel_num_valid(mac_get_band_by_channel_num(uc_chan_num), uc_chan_num) != OAL_SUCC) {
            return OAL_FALSE;
        }
    }

    uc_chan_num = pst_scanned_bss->st_bss_dscr_info.st_channel.uc_chan_number;
    if (((uc_curr_chan > uc_chan_num) && (uc_curr_chan - uc_chan_num >= 3)) ||
        ((uc_curr_chan < uc_chan_num) && (uc_chan_num - uc_curr_chan >= 3))) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


OAL_STATIC uint32_t hmac_scan_update_bss_dscr(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *pst_netbuf, uint16_t us_buf_len, hmac_scanned_bss_info *pst_scanned_bss, uint8_t uc_ext_len)
{
    mac_scanned_result_extend_info_stru *pst_scan_result_extend_info = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device;
    mac_ieee80211_frame_stru *pst_frame_header = OAL_PTR_NULL;
    uint8_t *puc_frame_body = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint8_t *puc_ssid = OAL_PTR_NULL; /* ָ��beacon֡�е�ssid */
    uint8_t *puc_mgmt_frame = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint16_t us_frame_len;
    uint16_t us_frame_body_len;
    uint16_t us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    uint8_t uc_ssid_len;
    uint8_t uc_frame_channel;
    uint32_t ul_ret;
    int32_t l_ret;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
    struct timespec ts;
#else
    struct timeval tv;
#endif
#endif

    /* ��ȡmac device */
    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_bss_dscr::pst_mac_device is null.}");
        return OAL_FAIL;
    }

    /* ��ȡdevice�ϱ���ɨ������Ϣ����������µ�bss�����ṹ���� */
    us_frame_len = us_buf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru);
    puc_mgmt_frame = (uint8_t *)oal_netbuf_data(pst_netbuf);
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ָ��netbuf�е��ϱ���ɨ��������չ��Ϣ��λ�� */
    pst_scan_result_extend_info = (mac_scanned_result_extend_info_stru *)(puc_mgmt_frame + us_frame_len);

    /* ��������֡���� */
    l_ret = memcpy_s(pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff, (uint32_t)us_frame_len,
                     puc_mgmt_frame, (uint32_t)us_frame_len);
    puc_mgmt_frame = pst_scanned_bss->st_bss_dscr_info.auc_mgmt_buff;

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R
    /* ���ݻ�ͨIE���akm suite(FT)��RSN IE */
    if (uc_ext_len != DEFAULT_AKM_VALUE) {
        hmac_scan_attach_akm_suite_to_rsn_ie(puc_mgmt_frame, &us_frame_len);
    }
#endif

    /* ��ȡ����֡��֡ͷ��֡��ָ�� */
    pst_frame_header = (mac_ieee80211_frame_stru *)puc_mgmt_frame;
    puc_frame_body = puc_mgmt_frame + MAC_80211_FRAME_LEN;
    us_frame_body_len = us_frame_len - MAC_80211_FRAME_LEN;

    if (us_frame_body_len <= us_offset) {
        oam_error_log1(0, OAM_SF_SCAN, "hmac_scan_update_bss_dscr::frame_body_len err[%d]!", us_frame_body_len);
        return OAL_FAIL;
    }

    /* ��ȡ����֡�е��ŵ� */
    uc_frame_channel = mac_ie_get_chan_num(puc_frame_body, us_frame_body_len, us_offset,
                                           pst_rx_ctrl->st_rx_info.uc_channel_number);

    /* ����bss��Ϣ */
    pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

    /*****************************************************************************
        ����beacon/probe rsp֡����¼��pst_bss_dscr
    *****************************************************************************/
    /* ����������ssid */
    puc_ssid = mac_get_ssid(puc_frame_body, (int32_t)us_frame_body_len, &uc_ssid_len);
    if ((puc_ssid != OAL_PTR_NULL) && (uc_ssid_len != 0)) {
        /* �����ҵ���ssid���浽bss�����ṹ���� */
        l_ret += memcpy_s(pst_bss_dscr->ac_ssid, WLAN_SSID_MAX_LEN, puc_ssid, uc_ssid_len);
        pst_bss_dscr->ac_ssid[uc_ssid_len] = '\0';
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_update_bss_dscr::memcpy fail!");
        return OAL_FAIL;
    }

    /* ����bssid */
    oal_set_mac_addr(pst_bss_dscr->auc_mac_addr, pst_frame_header->auc_address2);
    oal_set_mac_addr(pst_bss_dscr->auc_bssid, pst_frame_header->auc_address3);

    /* bss������Ϣ */
    pst_bss_dscr->en_bss_type = pst_scan_result_extend_info->en_bss_type;

    pst_bss_dscr->us_cap_info = *((uint16_t *)(puc_frame_body + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN));

    pst_bss_dscr->c_rssi = (int8_t)pst_scan_result_extend_info->l_rssi;
    pst_bss_dscr->c_ant0_rssi = (int8_t)pst_scan_result_extend_info->c_ant0_rssi;
    pst_bss_dscr->c_ant1_rssi = (int8_t)pst_scan_result_extend_info->c_ant1_rssi;
    pst_bss_dscr->ant2_rssi = (int8_t)pst_scan_result_extend_info->ant2_rssi;
    pst_bss_dscr->ant3_rssi = (int8_t)pst_scan_result_extend_info->ant3_rssi;
    /* ����beacon������tim���� */
    pst_bss_dscr->us_beacon_period = mac_get_beacon_period(puc_frame_body);
    pst_bss_dscr->uc_dtim_period = mac_get_dtim_period(puc_frame_body, us_frame_body_len);
    pst_bss_dscr->uc_dtim_cnt = mac_get_dtim_cnt(puc_frame_body, us_frame_body_len);

    /* �ŵ� */
    pst_bss_dscr->st_channel.uc_chan_number = uc_frame_channel;
    pst_bss_dscr->st_channel.en_band = mac_get_band_by_channel_num(uc_frame_channel);

    /* ��¼���ʼ� */
    hmac_scan_update_bss_list_rates(pst_bss_dscr, puc_frame_body, us_frame_body_len, pst_mac_device);

    /* 03��¼֧�ֵ����ռ��� */
#ifdef _PRE_WLAN_FEATURE_M2S
    pst_bss_dscr->en_support_max_nss = pst_scan_result_extend_info->en_support_max_nss;
    pst_bss_dscr->en_support_opmode = pst_scan_result_extend_info->en_support_opmode;
    pst_bss_dscr->uc_num_sounding_dim = pst_scan_result_extend_info->uc_num_sounding_dim;
#endif

    /* Э���������ϢԪ�صĻ�ȡ */
    hmac_scan_update_bss_list_protocol(pst_hmac_vap, pst_bss_dscr, puc_frame_body, us_frame_body_len);

    /* update st_channel.bandwidth in case hmac_sta_update_join_req_params usage error */
    pst_bss_dscr->st_channel.en_bandwidth = pst_bss_dscr->en_channel_bandwidth;
    ul_ret = mac_get_channel_idx_from_num(pst_bss_dscr->st_channel.en_band,
        pst_bss_dscr->st_channel.uc_chan_number, &pst_bss_dscr->st_channel.uc_chan_idx);
    if (ul_ret == OAL_ERR_CODE_INVALID_CONFIG) {
        oam_warning_log0(0, OAM_SF_SCAN, "{hmac_scan_update_bss_dscr::mac_get_channel_idx_from_num fail.}");
    }

    /* ����ʱ��� */
    pst_bss_dscr->ul_timestamp = (uint32_t)oal_time_get_stamp_ms();

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
    get_monotonic_boottime(&ts);
    pst_bss_dscr->ul_wpa_rpt_time = ((u64)(ts.tv_sec) * 1000000) + ts.tv_nsec / 1000;
#else
    do_gettimeofday(&tv);
    pst_bss_dscr->ul_wpa_rpt_time = ((u64)tv.tv_sec * 1000000) + tv.tv_usec;
#endif
#endif

    pst_bss_dscr->ul_mgmt_len = us_frame_len;

    return OAL_SUCC;
}


uint32_t hmac_scan_proc_scan_result_handle(hmac_vap_stru *pst_hmac_vap,
                                           oal_netbuf_stru *pst_netbuf,
                                           uint16_t us_buf_len,
                                           mac_multi_bssid_frame_info_stru *pst_mbss_frame_info)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_new_scanned_bss = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_old_scanned_bss = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_mgmt_len;
    uint8_t uc_vap_id;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    uint32_t ul_curr_time_stamp;
#ifdef _PRE_WLAN_FEATURE_M2S
    mac_ieee80211_frame_stru *pst_frame_header;
#endif
    uint8_t uc_ext_len = DEFAULT_AKM_VALUE;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
    struct timespec ts;
#else
    struct timeval tv;
#endif
#endif

    /* ��ȡvap id */
    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;

    /* ��ȡhmac device �ṹ */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_proc_scan_result_handle::pst_hmac_device null.}");

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��dmac�ϱ���netbuf���ݽ��н���������������ʾ */
    /***********************************************************************************************/
    /*            netbuf data����ϱ���ɨ�������ֶεķֲ�                                        */
    /* ------------------------------------------------------------------------------------------  */
    /* beacon/probe rsp body  |     ֡����渽���ֶ�(mac_scanned_result_extend_info_stru)          */
    /* -----------------------------------------------------------------------------------------   */
    /* �յ���beacon/rsp��body | rssi(4�ֽ�) | channel num(1�ֽ�)| band(1�ֽ�)|bss_tye(1�ֽ�)|���  */
    /* ------------------------------------------------------------------------------------------  */
    /*                                                                                             */
    /***********************************************************************************************/
    /* ����֡�ĳ��ȵ����ϱ���netbuf�ĳ��ȼ�ȥ�ϱ���ɨ��������չ�ֶεĳ��� */
    us_mgmt_len = us_buf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru);

#ifdef _PRE_WLAN_FEATURE_ADAPTIVE11R
    /* adaptive 11Rģʽ�¼���Ƿ���Ҫ��RSN IE�����akm suite */
    if (pst_hmac_vap->bit_adaptive11r == OAL_TRUE) {
        uc_ext_len = hmac_scan_extend_mgmt_len_needed(pst_netbuf, us_buf_len);
        if (uc_ext_len != DEFAULT_AKM_VALUE) {
            us_mgmt_len += uc_ext_len;
        }
    }
#endif

    /* ����洢ɨ�������ڴ� */
    pst_new_scanned_bss = hmac_scan_alloc_scanned_bss(us_mgmt_len);
    if (oal_unlikely(pst_new_scanned_bss == OAL_PTR_NULL)) {
        oam_error_log0(uc_vap_id, OAM_SF_SCAN,
                       "{hmac_scan_proc_scan_result_handle::alloc memory failed for storing scanned result.}");

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��������ɨ������bss dscr�ṹ�� */
    ul_ret = hmac_scan_update_bss_dscr(pst_hmac_vap, pst_netbuf, us_buf_len, pst_new_scanned_bss, uc_ext_len);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(uc_vap_id, OAM_SF_SCAN,
                       "{hmac_scan_proc_scan_result_handle::hmac_scan_update_bss_dscr failed[%d].}", ul_ret);

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_netbuf);

        /* �ͷ�����Ĵ洢bss��Ϣ���ڴ� */
        oal_free(pst_new_scanned_bss);
        return ul_ret;
    }
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        /* ����mbss info */
        if (IS_CUSTOM_OPEN_MULTI_BSSID_SWITCH(&pst_hmac_vap->st_vap_base_info)) {
            memcpy_s(pst_new_scanned_bss->st_bss_dscr_info.st_mbssid_info.auc_transmitted_bssid, WLAN_MAC_ADDR_LEN,
                     pst_mbss_frame_info->auc_transmitted_bssid, WLAN_MAC_ADDR_LEN);
            pst_new_scanned_bss->st_bss_dscr_info.st_mbssid_info.bit_is_non_transimitted_bss =
                pst_mbss_frame_info->bit_is_non_transimitted_bss;
        }
    }
#endif
    /* ��ȡ����ɨ���bss����Ľṹ�� */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);
    /* ������ɾ����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));
    /* �ж���ͬbssid��bss�Ƿ��Ѿ�ɨ�赽 */
    pst_old_scanned_bss = hmac_scan_find_scanned_bss_by_bssid(pst_bss_mgmt,
                                                              pst_new_scanned_bss->st_bss_dscr_info.auc_bssid);
    if (pst_old_scanned_bss == OAL_PTR_NULL) {
        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));
        /*lint -e801*/
        goto add_bss;
        /*lint +e801*/
    }

#ifdef _PRE_WLAN_FEATURE_M2S
    /* ֻ��probe rsp֡��ext cap ����֧��OPMODEʱ���Զ˲�ȷʵ֧��OPMODE��beacon֡��assoc rsp֡����Ϣ������ */
    pst_frame_header = (mac_ieee80211_frame_stru *)pst_new_scanned_bss->st_bss_dscr_info.auc_mgmt_buff;
    if (pst_frame_header->st_frame_control.bit_sub_type == WLAN_PROBE_RSP) {
        pst_old_scanned_bss->st_bss_dscr_info.en_support_opmode =
            pst_new_scanned_bss->st_bss_dscr_info.en_support_opmode;
    }
#endif

    /* ����ϵ�ɨ���bss���ź�ǿ�ȴ��ڵ�ǰɨ�赽��bss���ź�ǿ�ȣ����µ�ǰɨ�赽���ź�ǿ��Ϊ��ǿ���ź�ǿ�� */
    if (pst_old_scanned_bss->st_bss_dscr_info.c_rssi > pst_new_scanned_bss->st_bss_dscr_info.c_rssi) {
        /* 1s�����ھͲ���֮ǰ��BSS�����RSSI��Ϣ������Ͳ����µ�RSSI��Ϣ */
        ul_curr_time_stamp = (uint32_t)oal_time_get_stamp_ms();
        if (oal_time_after32((ul_curr_time_stamp),
            (pst_old_scanned_bss->st_bss_dscr_info.ul_timestamp + HMAC_SCAN_MAX_SCANNED_RSSI_EXPIRE)) == FALSE) {
            oam_info_log0(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_proc_scan_result_handle::update signal strength value.}");
            pst_new_scanned_bss->st_bss_dscr_info.c_rssi = pst_old_scanned_bss->st_bss_dscr_info.c_rssi;
        }
    }

    if (hmac_scan_is_hidden_ssid(uc_vap_id, pst_new_scanned_bss, pst_old_scanned_bss) == OAL_TRUE) {
        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));

        /* �ͷ�����Ĵ洢bss��Ϣ���ڴ� */
        oal_free(pst_new_scanned_bss);

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_netbuf);

        return OAL_SUCC;
    }

    if (hmac_scan_need_update_old_scan_result(pst_hmac_vap, pst_new_scanned_bss, pst_old_scanned_bss) == OAL_FALSE
        || hmac_scan_check_chan(pst_netbuf, pst_new_scanned_bss) == OAL_FALSE) {
        pst_old_scanned_bss->st_bss_dscr_info.ul_timestamp = (uint32_t)oal_time_get_stamp_ms();

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
        get_monotonic_boottime(&ts);
        pst_old_scanned_bss->st_bss_dscr_info.ul_wpa_rpt_time = ((u64)(ts.tv_sec) * 1000000) + ts.tv_nsec / 1000;
#else
        do_gettimeofday(&tv);
        pst_old_scanned_bss->st_bss_dscr_info.ul_wpa_rpt_time = ((u64)tv.tv_sec * 1000000) + tv.tv_usec;
#endif
#endif
        pst_old_scanned_bss->st_bss_dscr_info.c_rssi = pst_new_scanned_bss->st_bss_dscr_info.c_rssi;

        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));

        /* �ͷ�����Ĵ洢bss��Ϣ���ڴ� */
        oal_free(pst_new_scanned_bss);

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_netbuf);

        return OAL_SUCC;
    }

    /* �������н�ԭ��ɨ�赽����ͬbssid��bss�ڵ�ɾ�� */
    hmac_scan_del_bss_from_list_nolock(pst_old_scanned_bss, pst_hmac_device);
    /* ���� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));
    /* �ͷ��ڴ� */
    oal_free(pst_old_scanned_bss);
add_bss:

    /* ��ɨ������ӵ������� */
    hmac_scan_add_bss_to_list(pst_new_scanned_bss, pst_hmac_device);
    /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
    oal_netbuf_free(pst_netbuf);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC uint32_t hmac_scan_get_non_transmitted_bssid(uint8_t *puc_tx_bssid, uint8_t uc_tx_bssid_len,
                                                        uint8_t uc_max_indicator, uint8_t uc_non_bssid_index,
                                                        uint8_t *puc_non_tx_bssid, uint8_t uc_ntx_bssid_len)
{
#define MAC_ADDR_MASK       0xFFFFFFFFFFFF
#define MAX_BSSID_INDICATOR 46
    uint8_t uc_index;
    uint8_t auc_non_tx_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t auc_bssid_tmp[WLAN_MAC_ADDR_LEN];
    uint64_t ull_bssid = 0;
    uint64_t ull_low_bssid;
    uint64_t ull_bssid_mask;
    int32_t l_ret;

    /* Э��涨������� */
    if (uc_max_indicator >= MAX_BSSID_INDICATOR) {
        return OAL_FAIL;
    }

    if ((uc_ntx_bssid_len != WLAN_MAC_ADDR_LEN) || (uc_tx_bssid_len != WLAN_MAC_ADDR_LEN)) {
        return OAL_FAIL;
    }

    ull_bssid_mask = ((MAC_ADDR_MASK << uc_max_indicator) ^ MAC_ADDR_MASK);

    /* �ֽڷ�ת */
    for (uc_index = 0; uc_index < 6; uc_index++) {
        auc_bssid_tmp[uc_index] = puc_tx_bssid[5 - uc_index];
    }
    l_ret = memcpy_s(&ull_bssid, sizeof(uint64_t), auc_bssid_tmp, WLAN_MAC_ADDR_LEN);
    ull_low_bssid = ull_bssid & ull_bssid_mask;
    ull_bssid &= (~ull_bssid_mask);
    ull_low_bssid = ((ull_low_bssid + uc_non_bssid_index) % (1ULL << uc_max_indicator));
    ull_bssid = ((ull_bssid & (~ull_bssid_mask)) | (ull_low_bssid));
    l_ret += memcpy_s(auc_non_tx_bssid, WLAN_MAC_ADDR_LEN, (uint8_t *)&ull_bssid, 6);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_scan_get_non_transmitted_bssid::memcpy fail!");
        return OAL_FAIL;
    }

    for (uc_index = 0; uc_index < 6; uc_index++) {
        puc_non_tx_bssid[uc_index] = auc_non_tx_bssid[5 - uc_index];
    }

    return OAL_SUCC;
}


oal_netbuf_stru *hmac_scan_proc_non_transmitted_ssid_handle(oal_netbuf_stru *pst_netbuf,
                                                            uint16_t us_buf_len,
                                                            mac_multi_bssid_frame_info_stru *pst_mbss_frame_info,
                                                            uint16_t *pus_new_buf_len)
{
    uint8_t *puc_transmitted_mgmt_frame;
    dmac_rx_ctl_stru *pst_transmitted_rx_ctrl;
    uint8_t *puc_transmitted_mgmt_frame_body;
    uint8_t *puc_transmitted_ssid;
    uint8_t *puc_transmitted_ssid_ie = OAL_PTR_NULL;
    uint8_t *puc_after_transmitted_ssid_ie = OAL_PTR_NULL;
    oal_netbuf_stru *pst_new_netbuf = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_new_rx_ctrl = OAL_PTR_NULL;
    uint8_t *puc_new_netbuf_data = OAL_PTR_NULL;
    uint8_t uc_transmitted_ssid_len;
    uint16_t us_transmitted_mgmt_frame_len;
    uint16_t us_transmitted_mgmt_frame_body_len;
    uint16_t us_before_ssid_ie_len;
    uint16_t us_use_buf_len;
    uint16_t us_after_ssid_ie_len;
    mac_ieee80211_frame_stru *pst_frame_header = OAL_PTR_NULL;
    int32_t l_ret;

    /* ��ȡ����֡֡���֡�� */
    puc_transmitted_mgmt_frame = (uint8_t *)oal_netbuf_data(pst_netbuf);
    pst_transmitted_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    us_transmitted_mgmt_frame_len = us_buf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru);

    puc_transmitted_mgmt_frame_body = puc_transmitted_mgmt_frame + MAC_80211_FRAME_LEN;
    us_transmitted_mgmt_frame_body_len =
        us_buf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru) - MAC_80211_FRAME_LEN;

    /* ��ȡtransimitted ֡��ssid ���� */
    puc_transmitted_ssid = mac_get_ssid(puc_transmitted_mgmt_frame_body,
                                        (int32_t)us_transmitted_mgmt_frame_body_len,
                                        &uc_transmitted_ssid_len);
    if ((puc_transmitted_ssid != OAL_PTR_NULL) && (uc_transmitted_ssid_len == 0)) {
        return OAL_PTR_NULL;
    }

    /* ����ssid ie ֮ǰ��֡�� */
    puc_transmitted_ssid_ie = puc_transmitted_ssid - MAC_IE_HDR_LEN;
    us_before_ssid_ie_len = puc_transmitted_ssid_ie - puc_transmitted_mgmt_frame;

    /* ����ssid ie ֮��֡�� */
    puc_after_transmitted_ssid_ie = puc_transmitted_ssid + uc_transmitted_ssid_len;
    us_after_ssid_ie_len = us_buf_len - (puc_after_transmitted_ssid_ie - puc_transmitted_mgmt_frame);

    /* ��Ҫ��������netbuf �ڴ� */
    *pus_new_buf_len = us_buf_len - uc_transmitted_ssid_len + pst_mbss_frame_info->uc_non_transmitted_ssid_len;
    pst_new_netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                             *pus_new_buf_len,
                                                             OAL_NETBUF_PRIORITY_MID);
    if (pst_new_netbuf == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_SCAN,
                       "{hmac_scan_proc_non_transmitted_ssid_handle::Allco netbuf_len=[%d] fail}",
                       *pus_new_buf_len);
        return OAL_PTR_NULL;
    }

    /* copy cb �ֶ� */
    pst_new_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_new_netbuf);
    l_ret = memcpy_s(pst_new_rx_ctrl, OAL_SIZEOF(dmac_rx_ctl_stru),
                     pst_transmitted_rx_ctrl, OAL_SIZEOF(dmac_rx_ctl_stru));

    /* copy  ssid ie ֮ǰ�Ĳ��� */
    puc_new_netbuf_data = (uint8_t *)oal_netbuf_data(pst_new_netbuf);
    /* ��鳤���Ƿ��㹻 */
    us_use_buf_len = us_before_ssid_ie_len;
    if (*pus_new_buf_len < us_use_buf_len) {
        oam_warning_log2(0, OAM_SF_SCAN,
                         "{hmac_scan_proc_non_transmitted_ssid_handle::us_use_buf_len=[%d] over netbuf_len=[%d]}",
                         us_use_buf_len, *pus_new_buf_len);

        oal_netbuf_free(pst_new_netbuf);
        return OAL_PTR_NULL;
    }
    l_ret += memcpy_s(puc_new_netbuf_data, *pus_new_buf_len, puc_transmitted_mgmt_frame, us_before_ssid_ie_len);
    // �滻mac header �е�bssid �� ta
    pst_frame_header = (mac_ieee80211_frame_stru *)puc_new_netbuf_data;
    l_ret += memcpy_s(pst_frame_header->auc_address2, WLAN_MAC_ADDR_LEN,
                      pst_mbss_frame_info->auc_non_transmitted_bssid, WLAN_MAC_ADDR_LEN);
    l_ret += memcpy_s(pst_frame_header->auc_address3, WLAN_MAC_ADDR_LEN,
                      pst_mbss_frame_info->auc_non_transmitted_bssid, WLAN_MAC_ADDR_LEN);

    puc_new_netbuf_data += us_before_ssid_ie_len;
    /* copy ssid ie */
    us_use_buf_len += (MAC_IE_HDR_LEN + pst_mbss_frame_info->uc_non_transmitted_ssid_len);
    if (*pus_new_buf_len < us_use_buf_len) {
        oam_warning_log2(0, OAM_SF_SCAN,
                         "{hmac_scan_proc_non_transmitted_ssid_handle::us_use_buf_len=[%d] over netbuf_len=[%d]}",
                         us_use_buf_len, *pus_new_buf_len);
        oal_netbuf_free(pst_new_netbuf);
        return OAL_PTR_NULL;
    }
    puc_new_netbuf_data[0] = MAC_EID_SSID;
    puc_new_netbuf_data[1] = pst_mbss_frame_info->uc_non_transmitted_ssid_len;
    puc_new_netbuf_data += MAC_IE_HDR_LEN;
    l_ret += memcpy_s(puc_new_netbuf_data, pst_mbss_frame_info->uc_non_transmitted_ssid_len,
                      pst_mbss_frame_info->auc_non_transmitted_ssid, pst_mbss_frame_info->uc_non_transmitted_ssid_len);
    puc_new_netbuf_data += pst_mbss_frame_info->uc_non_transmitted_ssid_len;

    /* copy ssid ie ֮��Ĳ��� */
    us_use_buf_len += us_after_ssid_ie_len;

    if (*pus_new_buf_len < us_use_buf_len) {
        oam_warning_log2(0, OAM_SF_SCAN,
                         "{hmac_scan_proc_non_transmitted_ssid_handle::us_use_buf_len=[%d] over netbuf_len=[%d]}",
                         us_use_buf_len, *pus_new_buf_len);
        oal_netbuf_free(pst_new_netbuf);
        return OAL_PTR_NULL;
    }
    l_ret += memcpy_s(puc_new_netbuf_data, us_after_ssid_ie_len, puc_after_transmitted_ssid_ie, us_after_ssid_ie_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_proc_non_transmitted_ssid_handle::memcpy fail!");
        oal_netbuf_free(pst_new_netbuf);
        return OAL_PTR_NULL;
    }

    return pst_new_netbuf;
}

#ifdef _PRE_DEBUG_CODE
void hmac_scan_mbssid_printf(mac_multi_bssid_frame_info_stru *pst_mbssid)
{
    oam_error_log4(0, OAM_SF_SCAN,
        "{>>>>>>>::bit_is_non_transimitted_bss=%d,maxbssid_indicator=%d,bssid_index=%d,non_transmitted_ssid_len=%d.}",
        pst_mbssid->bit_is_non_transimitted_bss, pst_mbssid->uc_maxbssid_indicator,
        pst_mbssid->uc_bssid_index, pst_mbssid->uc_non_transmitted_ssid_len);

    oam_error_log1(0, OAM_SF_SCAN, "{>>>>>>>::us_non_tramsmitted_bssid_cap=[%d].}",
                   pst_mbssid->us_non_tramsmitted_bssid_cap);

    oam_error_log4(0, OAM_SF_UM, "{>>>>>>>::Transmitted BSSID:%02X:%02X:XX:XX:%02X:%02X.}",
                   pst_mbssid->auc_transmitted_bssid[0],
                   pst_mbssid->auc_transmitted_bssid[1],
                   pst_mbssid->auc_transmitted_bssid[4],
                   pst_mbssid->auc_transmitted_bssid[5]);
    oam_error_log4(0, OAM_SF_UM, "{>>>>>>>::Non_Transmitted BSSID:%02X:%02X:XX:XX:%02X:%02X.}",
                   pst_mbssid->auc_non_transmitted_bssid[0],
                   pst_mbssid->auc_non_transmitted_bssid[1],
                   pst_mbssid->auc_non_transmitted_bssid[4],
                   pst_mbssid->auc_non_transmitted_bssid[5]);
}
#endif

uint8_t hmac_scan_proc_non_transmit_bssid(hmac_vap_stru *pst_hmac_vap,
    uint8_t *puc_ie_data, mac_multi_bssid_frame_info_stru *pst_mbss_frame_info,
    uint16_t us_left_frame_body_len, mac_ieee80211_frame_stru *pst_frame_header)
{
    uint8_t uc_mbssid_ie_len;
    uint8_t auc_non_transmitted_bssid[WLAN_MAC_ADDR_LEN] = { 0 };
    uint32_t ul_ret;
    int32_t l_ret;

    if (mac_ie_parse_multi_bssid_opt_subie(puc_ie_data,
        pst_mbss_frame_info, &uc_mbssid_ie_len) != OAL_SUCC) {
        return 0;
    }

    /* �����ǰʣ��֡��С�ڽ�����mbssid ie len ����  */
    if (uc_mbssid_ie_len == 0 || uc_mbssid_ie_len > us_left_frame_body_len) {
        return 0;
    }

    /* ����transimitted bssid  */
    l_ret = memcpy_s(pst_mbss_frame_info->auc_transmitted_bssid, WLAN_MAC_ADDR_LEN,
                     pst_frame_header->auc_address2, WLAN_MAC_ADDR_LEN);

    /* ����non-transmitted bssid */
    ul_ret = hmac_scan_get_non_transmitted_bssid(pst_mbss_frame_info->auc_transmitted_bssid,
                                                 sizeof(pst_mbss_frame_info->auc_transmitted_bssid),
                                                 pst_mbss_frame_info->uc_maxbssid_indicator,
                                                 pst_mbss_frame_info->uc_bssid_index,
                                                 auc_non_transmitted_bssid,
                                                 sizeof(auc_non_transmitted_bssid));
    if (ul_ret != OAL_SUCC) {
        return 0;
    }

    /* non_transmitted bssid */
    l_ret += memcpy_s(pst_mbss_frame_info->auc_non_transmitted_bssid, WLAN_MAC_ADDR_LEN,
                      auc_non_transmitted_bssid, WLAN_MAC_ADDR_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_proc_multi_bssid_scanned_bss::memcpy fail!");
        return 0;
    }

    pst_mbss_frame_info->bit_is_non_transimitted_bss = OAL_TRUE;

    return uc_mbssid_ie_len;
}


uint32_t hmac_scan_proc_multi_bssid_scanned_bss(hmac_vap_stru *pst_hmac_vap,
                                                oal_netbuf_stru *pst_netbuf,
                                                uint16_t us_buf_len)
{
    uint16_t us_mgmt_len;
    uint8_t *puc_mgmt_frame;
    mac_ieee80211_frame_stru *pst_frame_header;
    uint8_t *puc_ie_data = OAL_PTR_NULL;
    uint8_t *puc_frame_body;
    uint16_t us_frame_body_len;
    uint16_t us_left_frame_body_len;
    mac_multi_bssid_frame_info_stru st_mbss_frame_info;
    uint8_t *puc_mbssid_ie = OAL_PTR_NULL;
    oal_netbuf_stru *pst_new_netbuf = OAL_PTR_NULL;
    uint8_t uc_mbssid_ie_len;
    uint8_t uc_mgmt_filed_len;
    uint16_t us_new_buf_len = 0;
    uint8_t uc_multi_bssid_ie_len;

    /* ��dmac�ϱ���netbuf���ݽ��н���������������ʾ */
    /***********************************************************************************************/
    /*            netbuf data����ϱ���ɨ�������ֶεķֲ�                                        */
    /* ------------------------------------------------------------------------------------------  */
    /* beacon/probe rsp body  |     ֡����渽���ֶ�(mac_scanned_result_extend_info_stru)          */
    /* -----------------------------------------------------------------------------------------   */
    /* �յ���beacon/rsp��body | rssi(4�ֽ�) | channel num(1�ֽ�)| band(1�ֽ�)|bss_tye(1�ֽ�)|���  */
    /* ------------------------------------------------------------------------------------------  */
    /*                                                                                             */
    /***********************************************************************************************/
    /* ����֡�ĳ��ȵ����ϱ���netbuf�ĳ��ȼ�ȥ�ϱ���ɨ��������չ�ֶεĳ��� */
    us_mgmt_len    = us_buf_len - OAL_SIZEOF(mac_scanned_result_extend_info_stru);
    puc_mgmt_frame = (uint8_t *)oal_netbuf_data(pst_netbuf);

    /* ��ȡ����֡��֡ͷ��֡��ָ�� */
    pst_frame_header = (mac_ieee80211_frame_stru *)puc_mgmt_frame;
    puc_frame_body = puc_mgmt_frame + MAC_80211_FRAME_LEN;
    us_frame_body_len = us_mgmt_len - MAC_80211_FRAME_LEN;
    uc_mgmt_filed_len = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    if (us_frame_body_len <= uc_mgmt_filed_len) {
        return OAL_FAIL;
    }

    /* ie ��ʼλ�� */
    puc_ie_data = puc_frame_body + uc_mgmt_filed_len;
    us_left_frame_body_len = us_frame_body_len - uc_mgmt_filed_len;

    if (us_left_frame_body_len < MAC_MULTIPLE_BSSID_IE_MIN_LEN) {
        return OAL_FAIL;
    }

    /* ����multi-bssid ie */
    puc_mbssid_ie = mac_find_ie(MAC_EID_MULTI_BSSID, puc_ie_data, us_left_frame_body_len);
    if (puc_mbssid_ie == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    memset_s(&st_mbss_frame_info, OAL_SIZEOF(mac_multi_bssid_frame_info_stru),
        0, OAL_SIZEOF(mac_multi_bssid_frame_info_stru));

    /* ����multi-bssid ie len */
    uc_multi_bssid_ie_len                    = (uint8_t)(*(puc_mbssid_ie + 1));
    puc_ie_data                              = puc_mbssid_ie + MAC_IE_HDR_LEN;

    /* ���� MaxBssid Indicator */
    st_mbss_frame_info.uc_maxbssid_indicator = (uint8_t)*puc_ie_data;
    puc_ie_data                             += 1;
    us_left_frame_body_len                   = uc_multi_bssid_ie_len - 1;

    /* �����Ƿ������Ч��Multi BSSID IE */
    while (us_left_frame_body_len > MAC_MULTIPLE_BSSID_IE_MIN_LEN) {
        uc_mbssid_ie_len = hmac_scan_proc_non_transmit_bssid(pst_hmac_vap, puc_ie_data,
            &st_mbss_frame_info, us_left_frame_body_len, pst_frame_header);
        if (uc_mbssid_ie_len == 0) {
            return OAL_FAIL;
        }
        pst_new_netbuf = hmac_scan_proc_non_transmitted_ssid_handle(pst_netbuf, us_buf_len,
                                                                    &st_mbss_frame_info, &us_new_buf_len);
        if (pst_new_netbuf == OAL_PTR_NULL) {
            return OAL_FAIL;
        }
        hmac_scan_proc_scan_result_handle(pst_hmac_vap, pst_new_netbuf, us_new_buf_len, &st_mbss_frame_info);

        us_left_frame_body_len  -= uc_mbssid_ie_len;
        puc_ie_data             += uc_mbssid_ie_len;
    }

    return OAL_SUCC;
}
#endif


uint32_t hmac_scan_proc_scanned_bss(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_netbuf_stru *pst_bss_mgmt_netbuf = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_dtx_event = OAL_PTR_NULL;
    mac_multi_bssid_frame_info_stru st_mbss_frame_info;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_proc_scanned_bss::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_dtx_event = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_bss_mgmt_netbuf = pst_dtx_event->pst_netbuf;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_event_hdr->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_proc_scanned_bss::pst_hmac_vap null.}");

        /* �ͷ��ϱ���bss��Ϣ��beacon����probe rsp֡���ڴ� */
        oal_netbuf_free(pst_bss_mgmt_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (IS_CUSTOM_OPEN_MULTI_BSSID_SWITCH(&pst_hmac_vap->st_vap_base_info)) {
            hmac_scan_proc_multi_bssid_scanned_bss(pst_hmac_vap, pst_bss_mgmt_netbuf, pst_dtx_event->us_frame_len);
        }
    }
#endif

    /* ������Beacon ��Probe Rsp��bss */
    memset_s((uint8_t *)&st_mbss_frame_info, OAL_SIZEOF(st_mbss_frame_info), 0, OAL_SIZEOF(st_mbss_frame_info));
    hmac_scan_proc_scan_result_handle(pst_hmac_vap, pst_bss_mgmt_netbuf,
                                      pst_dtx_event->us_frame_len, &st_mbss_frame_info);

    return OAL_SUCC;
}


OAL_STATIC void hmac_scan_print_channel_statistics_info(hmac_scan_record_stru *pst_scan_record)
{
    wlan_scan_chan_stats_stru *pst_chan_stats = pst_scan_record->ast_chan_results;
    uint8_t uc_vap_id = pst_scan_record->uc_vap_id;
    uint8_t uc_idx;

    /* ��Ȿ��ɨ���Ƿ������ŵ����������û��ֱ�ӷ��� */
    if (pst_chan_stats[0].uc_stats_valid == 0) {
        oam_info_log0(uc_vap_id, OAM_SF_SCAN,
                      "{hmac_scan_print_channel_statistics_info:: curr scan don't enable channel measure.\n}");
        return;
    }

    /* ��ӡ�ŵ�������� */
    oam_info_log0(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info:: The chan measure result: \n}");

    for (uc_idx = 0; uc_idx < pst_scan_record->uc_chan_numbers; uc_idx++) {
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Chan num      : %d\n}",
                      pst_chan_stats[uc_idx].uc_channel_number);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Stats cnt     : %d\n}",
                      pst_chan_stats[uc_idx].uc_stats_cnt);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Stats valid   : %d\n}",
                      pst_chan_stats[uc_idx].uc_stats_valid);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Stats time us : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_stats_time_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free time 20M : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_free_time_20M_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free time 40M : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_free_time_40M_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free time 80M : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_free_time_80M_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Send time     : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_send_time_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Recv time     : %d\n}",
                      pst_chan_stats[uc_idx].ul_total_recv_time_us);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free power cnt: %d\n}",
                      pst_chan_stats[uc_idx].uc_free_power_cnt);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free power 20M: %d\n}",
                      (int32_t)pst_chan_stats[uc_idx].s_free_power_stats_20M);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free power 40M: %d\n}",
                      (int32_t)pst_chan_stats[uc_idx].s_free_power_stats_40M);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Free power 80M: %d\n}",
                      (int32_t)pst_chan_stats[uc_idx].s_free_power_stats_80M);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Radar detected: %d\n}",
                      pst_chan_stats[uc_idx].uc_radar_detected);
        oam_info_log1(uc_vap_id, OAM_SF_SCAN, "{hmac_scan_print_channel_statistics_info::Radar bw      : %d\n}",
                      pst_chan_stats[uc_idx].uc_radar_bw);
    }

    return;
}


OAL_STATIC void hmac_scan_print_scan_record_info(hmac_vap_stru *pst_hmac_vap,
                                                     hmac_scan_record_stru *pst_scan_record)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_time_t_stru st_timestamp_diff;

    /* ��ȡɨ����ʱ��� */
    st_timestamp_diff = oal_ktime_sub(oal_ktime_get(), pst_scan_record->st_scan_start_time);

    /* �����ں˽ӿڣ���ӡ�˴�ɨ���ʱ */
    oam_warning_log4(pst_scan_record->uc_vap_id, OAM_SF_SCAN,
        "{hmac_scan_print_scan_record_info::scan comp,scan_status:%d,vap ch_num:%d,cookie:%x, duration time:%lums.}",
        pst_scan_record->en_scan_rsp_status,
        pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
        pst_scan_record->ull_cookie,
        ktime_to_ms(st_timestamp_diff));
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
    // �����HID2D�����ɨ�裬����Ҫ��ӡ��Ϣ */
    if (pst_scan_record->en_scan_mode == WLAN_SCAN_MODE_HID2D_SCAN) {
        return;
    }
#endif

    if (!hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH) || pst_hmac_vap->bit_in_p2p_listen == OAL_FALSE) {
        /* ��ӡɨ�赽��bss��Ϣ */
        hmac_scan_print_scanned_bss_info(pst_scan_record->uc_device_id);
    }
    /* �ŵ�������� */
    hmac_scan_print_channel_statistics_info(pst_scan_record);

    return;
}


uint32_t hmac_scan_proc_scan_comp_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    mac_scan_rsp_stru *pst_d2h_scan_rsp_info = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;
    oal_bool_enum_uint8 temp_flag;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_proc_scan_comp_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_event_hdr->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_proc_scan_comp_event::pst_hmac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_d2h_scan_rsp_info = (mac_scan_rsp_stru *)(pst_event->auc_event_data);
    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    if ((pst_event_hdr->uc_vap_id != pst_scan_mgmt->st_scan_record_mgmt.uc_vap_id) ||
        (pst_d2h_scan_rsp_info->ull_cookie != pst_scan_mgmt->st_scan_record_mgmt.ull_cookie)) {
        oam_warning_log4(pst_event_hdr->uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_comp_event::Report vap:%d Scan_rsp(cookie %d), another vap:%d scan(cookie %d)!}",
            pst_event_hdr->uc_vap_id, pst_d2h_scan_rsp_info->ull_cookie,
            pst_scan_mgmt->st_scan_record_mgmt.uc_vap_id, pst_scan_mgmt->st_scan_record_mgmt.ull_cookie);
        return OAL_SUCC;
    }

    if (!hmac_get_feature_switch(HMAC_MIRACAST_REDUCE_LOG_SWITCH)) {
        oam_warning_log1(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{hmac_scan_proc_scan_comp_event::scan status:%d!}",
            pst_d2h_scan_rsp_info->en_scan_rsp_status);
    }

    /* ɾ��ɨ�賬ʱ������ʱ�� */
    temp_flag = ((pst_scan_mgmt->st_scan_timeout.en_is_registerd == OAL_TRUE) &&
                 (pst_d2h_scan_rsp_info->en_scan_rsp_status != MAC_SCAN_PNO));
    if (temp_flag) {
        /* PNOû������ɨ�趨ʱ��,���ǵ�ȡ��PNOɨ��,�����·���ͨɨ��,PNOɨ������¼���������ͨɨ���Ӱ�� */
        frw_timer_immediate_destroy_timer_m(&(pst_scan_mgmt->st_scan_timeout));
    }

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event_hdr->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{hmac_scan_proc_scan_comp_event::hmac_vap null.}");

        /* ���õ�ǰ���ڷ�ɨ��״̬ */
        pst_scan_mgmt->en_is_scanning = OAL_FALSE;
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ���ݵ�ǰɨ������ͺ͵�ǰvap��״̬�������л�vap��״̬�������ǰ��ɨ�裬����Ҫ�л�vap��״̬ */
    if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (pst_d2h_scan_rsp_info->en_scan_rsp_status != MAC_SCAN_PNO)) {
        if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_STA_WAIT_SCAN) {
            /* �ı�vap״̬��SCAN_COMP */
            hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
        }
    }

    if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
        (pst_scan_mgmt->st_scan_record_mgmt.en_vap_last_state != MAC_VAP_STATE_BUTT)) {
        hmac_fsm_change_state(pst_hmac_vap, pst_scan_mgmt->st_scan_record_mgmt.en_vap_last_state);
        pst_scan_mgmt->st_scan_record_mgmt.en_vap_last_state = MAC_VAP_STATE_BUTT;
    }
    /* ����device�ϱ���ɨ�������ϱ�sme */
    /* ��ɨ��ִ�����(ɨ��ִ�гɹ�������ʧ�ܵȷ��ؽ��)��¼��ɨ�����м�¼�ṹ���� */
    pst_scan_mgmt->st_scan_record_mgmt.en_scan_rsp_status = pst_d2h_scan_rsp_info->en_scan_rsp_status;
    pst_scan_mgmt->st_scan_record_mgmt.ull_cookie = pst_d2h_scan_rsp_info->ull_cookie;

    hmac_scan_print_scan_record_info(pst_hmac_vap, &(pst_scan_mgmt->st_scan_record_mgmt));

    if (pst_scan_mgmt->st_scan_record_mgmt.p_fn_cb != OAL_PTR_NULL) {
        /* ��ֹɨ��������ûص�,��ֹ��ֹɨ������������PNOɨ���Ӱ�� */
        pst_scan_mgmt->st_scan_record_mgmt.p_fn_cb(&(pst_scan_mgmt->st_scan_record_mgmt));
    }

    /* ���õ�ǰ���ڷ�ɨ��״̬ */
    if (pst_d2h_scan_rsp_info->en_scan_rsp_status != MAC_SCAN_PNO) {
        /* PNOɨ��û���ô�λΪOAL_TRUE,PNOɨ�����,����Ӱ�����ĳ���ɨ�� */
        pst_scan_mgmt->en_is_scanning = OAL_FALSE;
    }
    if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        hmac_p2p_listen_timeout(pst_hmac_vap, &pst_hmac_vap->st_vap_base_info);
    }
    if (pst_hmac_vap->en_wait_roc_end == OAL_TRUE) {
        
        oam_warning_log1(pst_event_hdr->uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_comp_event::scan rsp status[%d]}", pst_d2h_scan_rsp_info->en_scan_rsp_status);
        oal_complete(&(pst_hmac_vap->st_roc_end_ready));
        pst_hmac_vap->en_wait_roc_end = OAL_FALSE;
    }
    /* STA����ɨ��ʱ����Ҫ��ǰʶ�����γ��� */
    if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_UP)) {
        hmac_roam_check_bkscan_result(pst_hmac_vap, &(pst_scan_mgmt->st_scan_record_mgmt));
    }

    return OAL_SUCC;
}

uint32_t hmac_scan_proc_scan_req_event_exception(hmac_vap_stru *pst_hmac_vap, void *p_params)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_scan_rsp_stru st_scan_rsp;
    hmac_scan_rsp_stru *pst_scan_rsp = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, p_params))) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_scan_proc_scan_req_event_exception::param null, %d %d.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_params);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��֧�ַ���ɨ���״̬������ɨ�� */
    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                     "{hmac_scan_proc_scan_req_event_exception::vap state is=%x.}",
                     pst_hmac_vap->st_vap_base_info.en_vap_state);

    memset_s(&st_scan_rsp, OAL_SIZEOF(hmac_scan_rsp_stru), 0, OAL_SIZEOF(hmac_scan_rsp_stru));

    /* ��ɨ������¼���WAL, ִ��SCAN_DONE , �ͷ�ɨ�������ڴ� */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(hmac_scan_rsp_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_scan_proc_scan_req_event_exception::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_scan_rsp.uc_result_code = MAC_SCAN_REFUSED;
    /* When STA is roaming, scan req return success instead of failure,
       in case roaming failure which will cause UI scan list null  */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
        st_scan_rsp.uc_result_code = MAC_SCAN_SUCCESS;
    }
    st_scan_rsp.uc_num_dscr = 0;

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_SCAN_COMP_STA,
                       OAL_SIZEOF(hmac_scan_rsp_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    pst_scan_rsp = (hmac_scan_rsp_stru *)pst_event->auc_event_data;

    if (memcpy_s(pst_scan_rsp, OAL_SIZEOF(hmac_scan_rsp_stru),
                        (void *)(&st_scan_rsp), OAL_SIZEOF(hmac_scan_rsp_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_proc_scan_req_event_exception::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


void hmac_scan_set_sour_mac_addr_in_probe_req(hmac_vap_stru *pst_hmac_vap,
                                              uint8_t *puc_sour_mac_addr,
                                              oal_bool_enum_uint8 en_is_rand_mac_addr_scan,
                                              oal_bool_enum_uint8 en_is_p2p0_scan)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_hmac_vap, puc_sour_mac_addr)) {
        oam_error_log2(0, OAM_SF_CFG,
            "{hmac_scan_set_sour_mac_addr_in_probe_req::param null,pst_hmac_vap:%p,puc_sour_mac_addr:%p.}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)puc_sour_mac_addr);
        return;
    }

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_scan_set_sour_mac_addr_in_probe_req::pst_mac_device is null.}");
        return;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_CFG,
                         "{hmac_scan_set_sour_mac_addr_in_probe_req::pst_hmac_device is null. device_id %d}",
                         pst_hmac_vap->st_vap_base_info.uc_device_id);
        return;
    }

    /* WLAN/P2P ��������£�p2p0 ��p2p-p2p0 cl ɨ��ʱ����Ҫʹ�ò�ͬ�豸 */
    if (en_is_p2p0_scan == OAL_TRUE) {
        oal_set_mac_addr(puc_sour_mac_addr, mac_mib_get_p2p0_dot11StationID(&pst_hmac_vap->st_vap_base_info));
    } else
    {
        /* ������mac addrɨ�����Կ����ҷ�P2P�������������mac addr��probe req֡�� */
        if ((en_is_rand_mac_addr_scan == OAL_TRUE) && (IS_LEGACY_VAP(&(pst_hmac_vap->st_vap_base_info))) &&
            ((pst_mac_device->auc_mac_oui[0] != 0) || (pst_mac_device->auc_mac_oui[1] != 0) ||
             (pst_mac_device->auc_mac_oui[2] != 0))) {
            /* �������mac ��ַ,ʹ���·����MAC OUI ���ɵ����mac ��ַ���µ�����ɨ�� */
            oal_set_mac_addr(puc_sour_mac_addr, pst_hmac_device->st_scan_mgmt.auc_random_mac);
        } else {
            /* ���õ�ַΪ�Լ���MAC��ַ */
            oal_set_mac_addr(puc_sour_mac_addr, mac_mib_get_StationID(&pst_hmac_vap->st_vap_base_info));
        }
    }

    return;
}

OAL_STATIC oal_bool_enum_uint8 hmac_scan_need_skip_channel(hmac_vap_stru *pst_hmac_vap, uint8_t uc_channel)
{
    wlan_channel_band_enum_uint8 en_band = mac_get_band_by_channel_num(uc_channel);
    if (mac_chip_run_band(pst_hmac_vap->st_vap_base_info.uc_chip_id, en_band) != OAL_TRUE) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC void hmac_scan_set_neighbor_report_scan_params(mac_scan_req_stru *pst_scan_params)
{
    if (pst_scan_params->uc_neighbor_report_process_flag == OAL_TRUE ||
        pst_scan_params->uc_bss_transition_process_flag == OAL_TRUE) {
        pst_scan_params->uc_scan_channel_interval = 2;
        pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE;
    }
    return;
}

OAL_STATIC oal_bool_enum_uint8 hmac_scan_get_random_mac_flag(hmac_vap_stru *pst_hmac_vap,
                                                             oal_bool_enum_uint8 en_customize_random_mac_scan)
{
    /* ��P2P�豸���������macɨ���־��device�����·���DMAC */
    return (IS_LEGACY_VAP(&(pst_hmac_vap->st_vap_base_info))) ? en_customize_random_mac_scan : OAL_FALSE;
}


OAL_STATIC uint32_t hmac_scan_update_scan_params(hmac_vap_stru *pst_hmac_vap,
                                                 mac_scan_req_stru *pst_scan_params,
                                                 oal_bool_enum_uint8 en_is_random_mac_addr_scan)
{
    mac_device_stru *pst_mac_device;
    mac_vap_stru *pst_mac_vap_temp = OAL_PTR_NULL;
    uint32_t ul_ret;
    wlan_vap_mode_enum_uint8 en_vap_mode;
    uint8_t uc_loop;
    uint8_t uc_chan_cnt = 0;

    /* ��ȡmac device */
    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_device_id, OAM_SF_SCAN,
                         "{hmac_scan_update_scan_params::pst_mac_device null.}");
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }
    /* 1.��¼����ɨ���vap id��ɨ����� */
    pst_scan_params->uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;
    pst_scan_params->en_need_switch_back_home_channel = OAL_FALSE;

    if (pst_scan_params->en_scan_mode != WLAN_SCAN_MODE_ROAM_SCAN)
    {
        pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_FOREGROUND;
    }

    /* 2.�޸�ɨ��ģʽ���ŵ�ɨ�����: �����Ƿ����up״̬�µ�vap������ǣ����Ǳ���ɨ�裬������ǣ�����ǰ��ɨ�� */
    ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_mac_vap_temp);
    if ((ul_ret == OAL_SUCC) && (pst_mac_vap_temp != OAL_PTR_NULL)) {
        /* �ж�vap�����ͣ������sta��Ϊsta�ı���ɨ�裬�����ap������ap�ı���ɨ�裬�������͵�vap�ݲ�֧�ֱ���ɨ�� */
        en_vap_mode = pst_hmac_vap->st_vap_base_info.en_vap_mode;
        if (en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
            if (pst_scan_params->en_scan_mode != WLAN_SCAN_MODE_ROAM_SCAN)
            {
                /* �޸�ɨ�����Ϊsta�ı���ɨ�� */
                pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_BACKGROUND_STA;
            }
        } else if (en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            /* �޸�ɨ�����Ϊsta�ı���ɨ�� */
            pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_BACKGROUND_AP;
        } else {
            oam_error_log1(0, OAM_SF_SCAN, "{hmac_scan_update_scan_params::vap mode[%d], not support bg scan.}",
                           en_vap_mode);
            return OAL_FAIL;
        }
        pst_scan_params->en_need_switch_back_home_channel = OAL_TRUE;

        if (mac_device_calc_up_vap_num(pst_mac_device) == 1 &&
            !IS_LEGACY_VAP(pst_mac_vap_temp) &&
            IS_LEGACY_VAP(&pst_hmac_vap->st_vap_base_info)) {
            /*
             * �޸�ɨ���ŵ����(2)�ͻع����ŵ�����ʱ��(60ms):�������P2P���ڹ���״̬��
             * wlan����ȥ����״̬,wlan�����ɨ��
             */
            pst_scan_params->uc_scan_channel_interval = MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE;
            pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE;

            if ((pst_scan_params->us_scan_time > WLAN_DEFAULT_ACTIVE_SCAN_TIME) &&
                (pst_scan_params->en_scan_type == WLAN_SCAN_TYPE_ACTIVE)) {
                /* ָ��SSIDɨ�賬��3��,���޸�ÿ��ɨ��ʱ��Ϊ40ms(Ĭ����20ms) */
                /* P2P������wlanδ��������,���ǵ�ɨ��ʱ�����Ӷ�p2p wfd������Ӱ��,����ÿ�ŵ�ɨ�����Ϊ1��(Ĭ��Ϊ2��) */
                pst_scan_params->uc_max_scan_count_per_channel = 1;
            }
        } else {
            /* Я������SSID�������ɨ3���ŵ���һ��home�ŵ����������Ĭ��ɨ��6���ŵ���home�ŵ�����100ms */
            pst_scan_params->uc_scan_channel_interval = (pst_scan_params->uc_ssid_num > 1) ?
                MAC_SCAN_CHANNEL_INTERVAL_HIDDEN_SSID : MAC_SCAN_CHANNEL_INTERVAL_DEFAULT;
            pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_DEFAULT;
        }
        hmac_scan_set_neighbor_report_scan_params(pst_scan_params);
    }

    /* 3.���÷��͵�probe req֡��Դmac addr */
    pst_scan_params->en_is_random_mac_addr_scan =
        hmac_scan_get_random_mac_flag(pst_hmac_vap, en_is_random_mac_addr_scan);
    hmac_scan_set_sour_mac_addr_in_probe_req(pst_hmac_vap, pst_scan_params->auc_sour_mac_addr,
                                                 en_is_random_mac_addr_scan, pst_scan_params->bit_is_p2p0_scan);

    for (uc_loop = 0; uc_loop < pst_scan_params->uc_channel_nums; uc_loop++) {
        if (!hmac_scan_need_skip_channel(pst_hmac_vap, pst_scan_params->ast_channel_list[uc_loop].uc_chan_number)) {
            if (uc_chan_cnt != uc_loop) {
                pst_scan_params->ast_channel_list[uc_chan_cnt] = pst_scan_params->ast_channel_list[uc_loop];
            }
            uc_chan_cnt++;
        }
    }

    if (!uc_chan_cnt) {
        oam_warning_log1(0, OAM_SF_SCAN, "{hmac_scan_update_scan_params::channels trimed to none!, ori cnt=%d}",
                         pst_scan_params->uc_channel_nums);

        return OAL_FAIL;
    }

    pst_scan_params->uc_channel_nums = uc_chan_cnt;
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_scan_check_can_enter_scan_state(mac_vap_stru *pst_mac_vap)
{
    /* p2p�п��ܽ��м������������Ǻ�scan req�����ȼ�һ������˵��ϲ㷢�����ɨ������ʱ��ͳһ��ʹ������Ľӿ��ж� */
    return hmac_p2p_check_can_enter_state(pst_mac_vap, HMAC_FSM_INPUT_SCAN_REQ);
}


uint32_t hmac_scan_check_is_dispatch_scan_req(hmac_vap_stru *pst_hmac_vap,
    hmac_device_stru *pst_hmac_device)
{
    uint32_t ul_ret;

    /* 1.�ȼ������vap��״̬�Ӷ��ж��Ƿ�ɽ���ɨ��״̬��ʹ��ɨ�辡��������������������� */
    ul_ret = hmac_scan_check_can_enter_scan_state(&(pst_hmac_vap->st_vap_base_info));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_check_is_dispatch_scan_req::Because of err_code[%d], can't enter into scan state.}", ul_ret);
        return ul_ret;
    }

    /* 2.�жϵ�ǰɨ���Ƿ�����ִ�� */
    if (pst_hmac_device->st_scan_mgmt.en_is_scanning == OAL_TRUE) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_check_is_dispatch_scan_req::the scan request is rejected.}");
        return OAL_FAIL;
    }

    /* 3.�жϵ�ǰ�Ƿ�����ִ������ */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_check_is_dispatch_scan_req:: roam reject new scan.}");
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_DFS
    /* 4.�жϵ�ǰ�Ƿ�����ִ��CAC */
    if (hmac_cac_abort_scan_check(pst_hmac_device) == OAL_SUCC) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_check_is_dispatch_scan_req:: cac abort scan.}");
        return OAL_FAIL;
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC void hmac_scan_proc_last_scan_record(hmac_vap_stru *pst_hmac_vap,
                                                hmac_device_stru *pst_hmac_device)
{
    oam_info_log0(0, OAM_SF_SCAN, "{hmac_scan_proc_scan_req_event:: start clean last scan record.}");

    /* ����ɨ��������ʱ�������һ��ɨ�����й��ڵ�bss��Ϣ */
    hmac_scan_clean_expire_scanned_bss(pst_hmac_vap, &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt));

    return;
}


OAL_STATIC uint32_t hmac_scan_proc_scan_timeout_fn(void *p_arg)
{
    hmac_device_stru *pst_hmac_device = (hmac_device_stru *)p_arg;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;
    uint32_t ul_pedding_data = 0;

    /* ��ȡɨ���¼��Ϣ */
    pst_scan_record = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt);

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_scan_record->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_scan_record->uc_vap_id, OAM_SF_SCAN, "{hmac_scan_proc_scan_timeout_fn::pst_hmac_vap null.}");

        /* ɨ��״̬�ָ�Ϊδ��ִ�е�״̬ */
        pst_hmac_device->st_scan_mgmt.en_is_scanning = OAL_FALSE;
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ���ݵ�ǰɨ������ͺ͵�ǰvap��״̬�������л�vap��״̬�������ǰ��ɨ�裬����Ҫ�л�vap��״̬ */
    if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_STA_WAIT_SCAN) {
            /* �ı�vap״̬��SCAN_COMP */
            hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
        }
    }

    if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
        (pst_scan_record->en_vap_last_state != MAC_VAP_STATE_BUTT)) {
        hmac_fsm_change_state(pst_hmac_vap, pst_scan_record->en_vap_last_state);
        pst_scan_record->en_vap_last_state = MAC_VAP_STATE_BUTT;
    }
    /* ����ɨ����Ӧ״̬Ϊ��ʱ */
    pst_scan_record->en_scan_rsp_status = MAC_SCAN_TIMEOUT;
    oam_warning_log1(pst_scan_record->uc_vap_id, OAM_SF_SCAN,
                     "{hmac_scan_proc_scan_timeout_fn::scan time out cookie [%x].}",
                     pst_scan_record->ull_cookie);

    /* ���ɨ��ص�������Ϊ�գ�����ûص����� */
    if (pst_scan_record->p_fn_cb != OAL_PTR_NULL) {
        oam_warning_log0(pst_scan_record->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_scan_timeout_fn::scan callback func proc.}");
        pst_scan_record->p_fn_cb(pst_scan_record);
    }

    /* DMAC ��ʱδ�ϱ�ɨ����ɣ�HMAC �·�ɨ��������ֹͣDMAC ɨ�� */
    hmac_config_scan_abort(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(uint32_t), (uint8_t *)&ul_pedding_data);

    /* ɨ��״̬�ָ�Ϊδ��ִ�е�״̬ */
    pst_hmac_device->st_scan_mgmt.en_is_scanning = OAL_FALSE;

    chr_exception(chr_wifi_drv(CHR_WIFI_DRV_EVENT_SCAN, CHR_WIFI_DRV_ERROR_SCAN_TIMEOUT));

    return OAL_SUCC;
}


OAL_STATIC void hmac_scan_proc_clear_last_scan_record(mac_scan_req_stru *pst_scan_params,
                                                      hmac_vap_stru *pst_hmac_vap,
                                                      hmac_device_stru *pst_hmac_device)
{
    if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
        if (pst_scan_params->uc_scan_func & MAC_SCAN_FUNC_P2P_LISTEN) {
            pst_hmac_vap->bit_in_p2p_listen = OAL_TRUE;
        } else {
            pst_hmac_vap->bit_in_p2p_listen = OAL_FALSE;
            /* ������һ��ɨ���¼��Ŀǰֱ�������һ�ν��������������Ҫ�ϻ�ʱ�䴦�� */
            hmac_scan_proc_last_scan_record(pst_hmac_vap, pst_hmac_device);
        }
    } else {
        /* ������һ��ɨ���¼��Ŀǰֱ�������һ�ν��������������Ҫ�ϻ�ʱ�䴦�� */
        hmac_scan_proc_last_scan_record(pst_hmac_vap, pst_hmac_device);
    }
}

uint32_t hmac_scan_preproc_scan_req_event(hmac_vap_stru *pst_hmac_vap, void *p_params,
    mac_scan_req_h2d_stru **ppst_h2d_scan_req_params, mac_scan_req_stru **ppst_scan_params,
    hmac_device_stru **ppst_hmac_device)
{
    oal_bool_enum_uint8 temp_flag;
    mac_scan_req_h2d_stru *pst_h2d_scan_req_params = OAL_PTR_NULL; /* hmac���͵�dmac��ɨ��������� */
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    /* �����Ϸ��Լ�� */
    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, p_params))) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_scan_proc_scan_req_event::param null, %p %p.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_params);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ɨ��ֹͣģ����� */
    temp_flag = (((g_en_bgscan_enable_flag == HMAC_BGSCAN_DISABLE) &&
                  (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_UP)) ||
                  (g_en_bgscan_enable_flag == HMAC_SCAN_DISABLE));
    if (temp_flag) {
        oam_warning_log1(0, 0, "hmac_scan_proc_scan_req_event:bgscan_en_flag:%d", g_en_bgscan_enable_flag);
        return OAL_FAIL;
    }

    pst_h2d_scan_req_params = (mac_scan_req_h2d_stru *)p_params;
    pst_scan_params = &(pst_h2d_scan_req_params->st_scan_params);

    /* �쳣�ж�: ɨ����ŵ�����Ϊ0 */
    if (pst_scan_params->uc_channel_nums == 0) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_scan_req_event::channel_nums=0.}");
        return OAL_FAIL;
    }

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_req_event::hmac_device null.}");
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    *ppst_h2d_scan_req_params = pst_h2d_scan_req_params;
    *ppst_scan_params = pst_scan_params;
    *ppst_hmac_device = pst_hmac_device;

    return OAL_SUCC;
}

uint32_t hmac_scan_proc_scan_req_event(hmac_vap_stru *pst_hmac_vap, void *p_params)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    mac_scan_req_h2d_stru *pst_h2d_scan_req_params = OAL_PTR_NULL; /* hmac���͵�dmac��ɨ��������� */
    mac_scan_req_stru *pst_scan_params = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;
    uint32_t ul_scan_timeout, ul_ret;
    oal_netbuf_stru *pst_netbuf_scan_req = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_scan_req_event = OAL_PTR_NULL;
    uint8_t *puc_netbuf_data = OAL_PTR_NULL;
    int32_t l_ret;
    oal_bool_enum_uint8 temp_flag;

    ul_ret = hmac_scan_preproc_scan_req_event(pst_hmac_vap, p_params, &pst_h2d_scan_req_params, &pst_scan_params,
        &pst_hmac_device);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

#ifdef _PRE_WLAN_FEATURE_MONITOR
    if (pst_hmac_device->en_monitor_mode == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_scan_proc_scan_req_event_etc: in sniffer monitor mode, scan abort!}");
        return -OAL_EINVAL;
    }
#endif

    /* ���´˴�ɨ�������ɨ����� */
    if (pst_scan_params->uc_scan_func == MAC_SCAN_FUNC_P2P_LISTEN) {
        ul_ret = hmac_scan_update_scan_params(pst_hmac_vap, pst_scan_params, OAL_FALSE);
    }
#ifdef _PRE_WLAN_FEATURE_HID2D
    /* HiD2D������ɨ�����Ϊ�ŵ�����ɨ�裬������probe request���������ɨ����� */
    else if (pst_scan_params->en_scan_mode == WLAN_SCAN_MODE_HID2D_SCAN) {
        ul_ret = OAL_SUCC;
    }
#endif
    else {
        /* ���´˴�ɨ�������ɨ����� */
        ul_ret = hmac_scan_update_scan_params(pst_hmac_vap, pst_scan_params,
                                              g_wlan_cust->uc_random_mac_addr_scan);
    }

    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_scan_req_event::update scan mode failed, error[%d].}", ul_ret);
        return ul_ret;
    }

    /* ����Ƿ���Ϸ���ɨ���������������������ϣ�ֱ�ӷ��� */
    ul_ret = hmac_scan_check_is_dispatch_scan_req(pst_hmac_vap, pst_hmac_device);
    if (ul_ret != OAL_SUCC) {
        if (pst_scan_params->uc_scan_func == MAC_SCAN_FUNC_P2P_LISTEN) {
            hmac_vap_state_restore(pst_hmac_vap);
        }

        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_scan_req_event::Because of error[%d], can't dispatch scan req.}", ul_ret);
        return ul_ret;
    }

    /* ����ɨ��ģ�鴦��ɨ��״̬������ɨ�����󽫶��� */
    pst_hmac_device->st_scan_mgmt.en_is_scanning = OAL_TRUE;

    hmac_scan_proc_clear_last_scan_record(pst_scan_params, pst_hmac_vap, pst_hmac_device);

    /* ��¼ɨ�跢���ߵ���Ϣ��ĳЩģ��ص�����ʹ�� */
    pst_scan_record = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt);
    pst_scan_record->uc_chip_id = pst_hmac_device->pst_device_base_info->uc_chip_id;
    pst_scan_record->uc_device_id = pst_hmac_device->pst_device_base_info->uc_device_id;
    pst_scan_record->uc_vap_id = pst_scan_params->uc_vap_id;
    pst_scan_record->uc_chan_numbers = pst_scan_params->uc_channel_nums;
    pst_scan_record->p_fn_cb = pst_scan_params->p_fn_cb;
    pst_scan_record->en_scan_mode = pst_scan_params->en_scan_mode;

    if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_req_event::save last vap_state:%d}", pst_hmac_vap->st_vap_base_info.en_vap_state);

        pst_scan_record->en_vap_last_state = pst_hmac_vap->st_vap_base_info.en_vap_state;
    }

    pst_scan_record->ull_cookie = pst_scan_params->ull_cookie;

    /* ��¼ɨ�迪ʼʱ�� */
    pst_scan_record->st_scan_start_time = oal_ktime_get();

    /* ����netbuf�ڴ�  */
    pst_netbuf_scan_req = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
        (OAL_SIZEOF(mac_scan_req_h2d_stru)), OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf_scan_req == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_req_event::pst_netbuf_scan_req alloc failed.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��ɨ�������¼���DMAC, ����event�¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oal_netbuf_free(pst_netbuf_scan_req);
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_req_event::alloc memory failed.}");
        /* �ָ�ɨ��״̬Ϊ������״̬ */
        pst_hmac_device->st_scan_mgmt.en_is_scanning = OAL_FALSE;
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �������ɨ���vap��ģʽΪsta�����ң������״̬Ϊ��up״̬���ҷ�p2p����״̬�����л���ɨ��״̬ */
    temp_flag = ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
                 (pst_scan_params->uc_scan_func != MAC_SCAN_FUNC_P2P_LISTEN) &&
                 (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP));
    if (temp_flag) {
        /* �л�vap��״̬ΪWAIT_SCAN״̬ */
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_WAIT_SCAN);
    }

    /* AP������ɨ�������⴦����hostapd�·�ɨ������ʱ��VAP������INIT״̬ */
    temp_flag = ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
                 (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_INIT));
    if (temp_flag) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_scan_req_event::ap startup scan.}");
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_AP_WAIT_START);
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCAN_REQ,
        OAL_SIZEOF(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1, pst_hmac_vap->st_vap_base_info.uc_chip_id,
        pst_hmac_vap->st_vap_base_info.uc_device_id, pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /***************************** copy data **************************/
    memset_s(oal_netbuf_cb(pst_netbuf_scan_req), OAL_TX_CB_LEN, 0, OAL_TX_CB_LEN);
    puc_netbuf_data = (uint8_t *)(oal_netbuf_data(pst_netbuf_scan_req));
    /* ����ɨ�����������netbuf data���� */
    l_ret = memcpy_s(puc_netbuf_data, OAL_SIZEOF(mac_scan_req_h2d_stru),
                     (uint8_t *)p_params, OAL_SIZEOF(mac_scan_req_h2d_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_rsn_ie::memcpy fail!");
        oal_netbuf_free(pst_netbuf_scan_req);
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* ����netbuf ���¼��������� */
    pst_scan_req_event = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_scan_req_event->pst_netbuf = pst_netbuf_scan_req;
    pst_scan_req_event->us_frame_len = OAL_SIZEOF(mac_scan_req_h2d_stru);
    pst_scan_req_event->us_remain = 0;

    /******************************************************************/
    /* ��ӡɨ�����������ʹ�� */
    /* �����P2P ���������������HMAC ɨ�賬ʱʱ��ΪP2P ����ʱ�� */
    ul_scan_timeout = (pst_scan_params->uc_scan_func == MAC_SCAN_FUNC_P2P_LISTEN) ?
                      (pst_scan_params->us_scan_time * 2) : WLAN_DEFAULT_MAX_TIME_PER_SCAN;
    if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
        /* ��Ϊ��������Է���dmac�л��ŵ���ҪԼ20msʱ�䣬��wpa supplicant��p2p listen���ó�10msʱ��
           ���ul_scan_timeoutֻ����2 * 10ms��������dmac�ϱ�ɨ����ǰ�ö�ʱ���ͳ�ʱ */
        ul_scan_timeout = (pst_scan_params->uc_scan_func == MAC_SCAN_FUNC_P2P_LISTEN) ?
            (pst_scan_params->us_scan_time * 4 + HMAC_P2P_LISTEN_EXTRA_WAIT_TIME) : WLAN_DEFAULT_MAX_TIME_PER_SCAN;
    }

    hmac_scan_print_scan_params(pst_scan_params, &pst_hmac_vap->st_vap_base_info);

    /* ����ɨ�豣����ʱ������ֹ���¼����˼�ͨ��ʧ�ܵ�����µ��쳣��������ʱ�������ĳ�ʱʱ��Ϊ4.5�� */
    frw_timer_create_timer_m(&(pst_hmac_device->st_scan_mgmt.st_scan_timeout), hmac_scan_proc_scan_timeout_fn,
        ul_scan_timeout, pst_hmac_device, OAL_FALSE, OAM_MODULE_ID_HMAC,
        pst_hmac_device->pst_device_base_info->ul_core_id);

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);

    oal_netbuf_free(pst_netbuf_scan_req);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_scan_proc_sched_scan_req_event(hmac_vap_stru *pst_hmac_vap, void *p_params)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;
    mac_pno_scan_stru *pst_pno_scan_params = OAL_PTR_NULL;
    uint32_t ul_ret;

    /* �����Ϸ��Լ�� */
    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, p_params))) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_scan_proc_sched_scan_req_event::param null, %p %p.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_params);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_pno_scan_params = (mac_pno_scan_stru *)p_params;

    /* �ж�PNO����ɨ���·��Ĺ��˵�ssid����С�ڵ���0 */
    if (pst_pno_scan_params->l_ssid_count <= 0) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_sched_scan_req_event::ssid_count <=0.}");
        return OAL_FAIL;
    }

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_proc_sched_scan_req_event::pst_hmac_device[%d] null.}",
                         pst_hmac_vap->st_vap_base_info.uc_device_id);
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    /* ����Ƿ���Ϸ���ɨ���������������������ϣ�ֱ�ӷ��� */
    ul_ret = hmac_scan_check_is_dispatch_scan_req(pst_hmac_vap, pst_hmac_device);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_scan_proc_sched_scan_req_event::Because of error[%d], can't dispatch scan req.}", ul_ret);
        return ul_ret;
    }

    /* �����һ�ε�ɨ���� */
    hmac_scan_proc_last_scan_record(pst_hmac_vap, pst_hmac_device);

    /* ��¼ɨ�跢���ߵ���Ϣ��ĳЩģ��ص�����ʹ�� */
    pst_scan_record = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt);
    pst_scan_record->uc_chip_id = pst_hmac_device->pst_device_base_info->uc_chip_id;
    pst_scan_record->uc_device_id = pst_hmac_device->pst_device_base_info->uc_device_id;
    pst_scan_record->uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;
    pst_scan_record->p_fn_cb = pst_pno_scan_params->p_fn_cb;

    /* ��ɨ�������¼���DMAC, �����¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(mac_pno_scan_stru *));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_scan_proc_sched_scan_req_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCHED_SCAN_REQ,
                       OAL_SIZEOF(mac_pno_scan_stru *),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* �¼�data����Я��PNOɨ��������� */
    if (EOK != memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(mac_pno_scan_stru *),
                        (uint8_t *)&pst_pno_scan_params, OAL_SIZEOF(mac_pno_scan_stru *))) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_scan_proc_sched_scan_req_event::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_scan_process_chan_result_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    dmac_crx_chan_result_stru *pst_chan_result_param = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;
    uint8_t uc_scan_idx;

    /* ��ȡ�¼���Ϣ */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_chan_result_param = (dmac_crx_chan_result_stru *)(pst_event->auc_event_data);
    uc_scan_idx = pst_chan_result_param->uc_scan_idx;

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_event_hdr->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_process_chan_result_event::pst_hmac_device is null.}");
        return OAL_FAIL;
    }

    pst_scan_record = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt);

    /* ����ϱ��������Ƿ�Ϸ� */
    if (uc_scan_idx >= pst_scan_record->uc_chan_numbers) {
        /* dmac�ϱ���ɨ����������Ҫɨ����ŵ����� */
        oam_warning_log2(0, OAM_SF_SCAN,
            "{hmac_scan_process_chan_result_event::result from dmac error! scan_idx[%d], chan_numbers[%d].}",
            uc_scan_idx, pst_scan_record->uc_chan_numbers);

        return OAL_FAIL;
    }

    pst_scan_record->ast_chan_results[uc_scan_idx] = pst_chan_result_param->st_chan_result;

    /* HiD2D get scan results */
#ifdef _PRE_WLAN_FEATURE_HID2D
    hmac_hid2d_scan_complete_handler(pst_scan_record, uc_scan_idx);
#endif

    return OAL_SUCC;
}
#if defined(_PRE_WLAN_FEATURE_11K)

uint32_t hmac_scan_rrm_proc_save_bss(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    oal_netbuf_stru *pst_action_table_bcn_rpt;
    uint8_t *puc_data;
    mac_user_stru *pst_mac_user;
    uint16_t us_index;
    mac_meas_rpt_ie_stru *pst_meas_rpt_ie;
    mac_bcn_rpt_stru *pst_bcn_rpt;
    mac_tx_ctl_stru *pst_tx_ctl;
    mac_vap_rrm_trans_req_info_stru *pst_trans_req_info;
    uint32_t ul_ret;
    hmac_device_stru *pst_hmac_device;
    hmac_bss_mgmt_stru *pst_bss_mgmt;
    oal_dlist_head_stru *pst_entry;
    hmac_scanned_bss_info *pst_scanned_bss;
    uint8_t uc_bss_idx = 0;
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_snr_ant0;
    uint8_t uc_snr_ant1;
    uint8_t uc_snr;
    uint16_t us_len;

    if (puc_param == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_scan_rrm_proc_save_bss::puc_param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_scan_rrm_proc_save_bss::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_scan_rrm_proc_save_bss::pst_hmac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_TX, "{hmac_scan_rrm_proc_save_bss::pst_mac_user[%d] null.",
                         pst_mac_vap->us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_trans_req_info = (mac_vap_rrm_trans_req_info_stru *)puc_param;

    pst_action_table_bcn_rpt = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                                       WLAN_MEM_NETBUF_SIZE2,
                                                                       OAL_NETBUF_PRIORITY_MID);
    if (pst_action_table_bcn_rpt == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_scan_rrm_proc_save_bss::pst_action_table_bcn_rpt null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(oal_netbuf_cb(pst_action_table_bcn_rpt), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    puc_data = (uint8_t *)oal_netbuf_header(pst_action_table_bcn_rpt);

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
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* duration */
    puc_data[2] = 0;
    puc_data[3] = 0;

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, pst_mac_user->auc_user_mac_addr);

    /* SA is the dot11MACAddress */
    oal_set_mac_addr(puc_data + 10, mac_mib_get_StationID(pst_mac_vap));

    oal_set_mac_addr(puc_data + 16, pst_mac_vap->auc_bssid);

    /* seq control */
    puc_data[22] = 0;
    puc_data[23] = 0;

    /*************************************************************************/
    /*                    Radio Measurement Report Frame - Frame Body        */
    /* --------------------------------------------------------------------- */
    /* |Category |Action |Dialog Token| Measurement Report Elements         | */
    /* --------------------------------------------------------------------- */
    /* |1        |1      | 1          |  var                                 */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* Initialize index and the frame data pointer */
    us_index = MAC_80211_FRAME_LEN;

    /* Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_RADIO_MEASURMENT;

    /* Action */
    puc_data[us_index++] = MAC_RM_ACTION_RADIO_MEASUREMENT_REPORT;

    /* Dialog Token  */
    puc_data[us_index++] = pst_trans_req_info->uc_action_dialog_token;

    us_len = us_index;
    /* ��ȡ����ɨ���bss����Ľṹ�� */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    if ((WLAN_MEM_NETBUF_SIZE2 - us_len) < (MAC_MEASUREMENT_RPT_FIX_LEN + MAC_BEACON_RPT_FIX_LEN)) {
        /* �ͷ� */
        oal_netbuf_free(pst_action_table_bcn_rpt);
        return OAL_SUCC;
    }
    pst_meas_rpt_ie = (mac_meas_rpt_ie_stru *)(puc_data + us_index);

    /* ������ɾ����ǰ���� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        /* BSSID���� */
        if (!ether_is_broadcast(pst_trans_req_info->auc_bssid) &&
            OAL_MEMCMP(pst_scanned_bss->st_bss_dscr_info.auc_bssid, pst_trans_req_info->auc_bssid, WLAN_MAC_ADDR_LEN)) {
            continue;
        }

        /* SSID���ˣ���������ssid����Ϊ0���򲻹��� */
        if (pst_trans_req_info->us_ssid_len != 0 &&
            OAL_MEMCMP(pst_scanned_bss->st_bss_dscr_info.ac_ssid,
                       pst_trans_req_info->auc_ssid, pst_trans_req_info->us_ssid_len)) {
            continue;
        }

        /*************************************************************************/
        /*                   Measurement Report IE - Frame Body                  */
        /* --------------------------------------------------------------------- */
        /* |Element ID |Length |Meas Token| Meas Rpt Mode | Meas Type | Meas Rpt| */
        /* --------------------------------------------------------------------- */
        /* |1          |1      | 1        |  1            | 1         | var      */
        /* --------------------------------------------------------------------- */
        /*                                                                       */
        /*************************************************************************/
        pst_meas_rpt_ie->uc_eid = MAC_EID_MEASREP;
        pst_meas_rpt_ie->uc_len = MAC_MEASUREMENT_RPT_FIX_LEN - MAC_IE_HDR_LEN + MAC_BEACON_RPT_FIX_LEN;
        pst_meas_rpt_ie->uc_token = pst_trans_req_info->uc_meas_token;
        memset_s(&(pst_meas_rpt_ie->st_rptmode),
                 OAL_SIZEOF(mac_meas_rpt_mode_stru), 0,
                 OAL_SIZEOF(mac_meas_rpt_mode_stru));
        pst_meas_rpt_ie->uc_rpttype = RM_RADIO_MEAS_BCN;

        pst_bcn_rpt = (mac_bcn_rpt_stru *)pst_meas_rpt_ie->auc_meas_rpt;
        memset_s(pst_bcn_rpt, OAL_SIZEOF(mac_bcn_rpt_stru), 0, OAL_SIZEOF(mac_bcn_rpt_stru));
        memcpy_s(pst_bcn_rpt->auc_bssid, WLAN_MAC_ADDR_LEN,
                 pst_scanned_bss->st_bss_dscr_info.auc_bssid, WLAN_MAC_ADDR_LEN);
        pst_bcn_rpt->uc_channum = pst_scanned_bss->st_bss_dscr_info.st_channel.uc_chan_number;
        if (pst_hmac_vap->bit_11k_auth_oper_class != 0) {
            pst_bcn_rpt->uc_optclass = pst_hmac_vap->bit_11k_auth_oper_class;
        } else {
            pst_bcn_rpt->uc_optclass = pst_trans_req_info->uc_oper_class;
        }

        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_scan_rrm_proc_save_bss::In Channel [%d] Find BSS %02X:XX:XX:XX:%02X:%02X.}",
                         pst_scanned_bss->st_bss_dscr_info.st_channel.uc_chan_number,
                         pst_scanned_bss->st_bss_dscr_info.auc_bssid[0],
                         pst_scanned_bss->st_bss_dscr_info.auc_bssid[4],
                         pst_scanned_bss->st_bss_dscr_info.auc_bssid[5]);

        pst_bcn_rpt->uc_rcpi = (uint8_t)((uint32_t)(pst_scanned_bss->st_bss_dscr_info.c_rssi + 110) << 1);

        /* ��ȡ�����  RSNI=(����� + 10) * 2 */
        uc_snr_ant0 = (uint8_t)pst_scanned_bss->st_bss_dscr_info.c_ant0_rssi;
        uc_snr_ant1 = (uint8_t)pst_scanned_bss->st_bss_dscr_info.c_ant0_rssi;
        if (uc_snr_ant0 == OAL_SNR_INIT_VALUE && uc_snr_ant1 != OAL_SNR_INIT_VALUE) {
            uc_snr = ((uc_snr_ant1 >> 1) + 10) << 1;
        } else if (uc_snr_ant0 != OAL_SNR_INIT_VALUE && uc_snr_ant1 == OAL_SNR_INIT_VALUE) {
            uc_snr = ((uc_snr_ant0 >> 1) + 10) << 1;
        } else if (uc_snr_ant0 != OAL_SNR_INIT_VALUE && uc_snr_ant1 != OAL_SNR_INIT_VALUE) {
            uc_snr = oal_max(uc_snr_ant0, uc_snr_ant1);
            uc_snr = ((uc_snr >> 1) + 10) << 1;
        } else {
            uc_snr = 0xFF; /* ��ЧֵΪ0xFF */
        }

        pst_bcn_rpt->uc_rsni = uc_snr;

        us_len += (MAC_MEASUREMENT_RPT_FIX_LEN + MAC_BEACON_RPT_FIX_LEN);
        uc_bss_idx++;
        if ((WLAN_MEM_NETBUF_SIZE2 - us_len) < (MAC_MEASUREMENT_RPT_FIX_LEN + MAC_BEACON_RPT_FIX_LEN)) {
            break;
        }

        pst_meas_rpt_ie = (mac_meas_rpt_ie_stru *)pst_bcn_rpt->auc_subelm;
    }
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "{hmac_scan_rrm_proc_save_bss::Find BssNum=[%d],us_len=[%d].buf_size=[%d].}",
                     uc_bss_idx, us_len, WLAN_MEM_NETBUF_SIZE2);

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_action_table_bcn_rpt);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_len;
    ul_ret = mac_vap_set_cb_tx_user_idx(pst_mac_vap, pst_tx_ctl, pst_mac_user->auc_user_mac_addr);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "(hmac_scan_rrm_proc_save_bss::fail to find user by xx:xx:xx:0x:0x:0x.}",
                         pst_mac_user->auc_user_mac_addr[3],
                         pst_mac_user->auc_user_mac_addr[4],
                         pst_mac_user->auc_user_mac_addr[5]);
    }

    if (MAC_GET_CB_MPDU_LEN(pst_tx_ctl) > WLAN_MEM_NETBUF_SIZE2) {
        oal_netbuf_free(pst_action_table_bcn_rpt);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_scan_rrm_proc_save_bss::invalid us_len=[%d].}",
                         MAC_GET_CB_MPDU_LEN(pst_tx_ctl));
        return OAL_SUCC;
    }
    oal_netbuf_put(pst_action_table_bcn_rpt, MAC_GET_CB_MPDU_LEN(pst_tx_ctl));

    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_action_table_bcn_rpt, MAC_GET_CB_MPDU_LEN(pst_tx_ctl));
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_action_table_bcn_rpt);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_scan_rrm_proc_save_bss::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}
#endif

void hmac_scan_init(hmac_device_stru *pst_hmac_device)
{
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;

    /* ��ʼ��ɨ�����ṹ����Ϣ */
    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    memset_s(pst_scan_mgmt, OAL_SIZEOF(hmac_scan_stru), 0, OAL_SIZEOF(hmac_scan_stru));
    pst_scan_mgmt->en_is_scanning = OAL_FALSE;
    pst_scan_mgmt->st_scan_record_mgmt.en_vap_last_state = MAC_VAP_STATE_BUTT;

    /* ��ʼ��bss������������� */
    pst_bss_mgmt = &(pst_scan_mgmt->st_scan_record_mgmt.st_bss_mgmt);
    oal_dlist_init_head(&(pst_bss_mgmt->st_bss_list_head));
    oal_spin_lock_init(&(pst_bss_mgmt->st_lock));

    /* ��ʼ���ں��·�ɨ��request��Դ�� */
    oal_spin_lock_init(&(pst_scan_mgmt->st_scan_request_spinlock));

    /* ��ʼ�� st_wiphy_mgmt �ṹ */
    oal_wait_queue_init_head(&(pst_scan_mgmt->st_wait_queue));

    /* ��ʼ��ɨ���������MAC ��ַ */
    oal_random_ether_addr(pst_hmac_device->st_scan_mgmt.auc_random_mac);
    return;
}


void hmac_scan_exit(hmac_device_stru *pst_hmac_device)
{
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;

    /* ���ɨ���¼��Ϣ */
    hmac_scan_clean_scan(&(pst_hmac_device->st_scan_mgmt));

    if (pst_hmac_device->st_scan_mgmt.st_init_scan_timeout.en_is_registerd) {
        frw_timer_immediate_destroy_timer_m(&pst_hmac_device->st_scan_mgmt.st_init_scan_timeout);
    }

    /* ���ɨ�����ṹ����Ϣ */
    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    memset_s(pst_scan_mgmt, OAL_SIZEOF(hmac_scan_stru), 0, OAL_SIZEOF(hmac_scan_stru));
    pst_scan_mgmt->en_is_scanning = OAL_FALSE;
    return;
}

uint32_t hmac_bgscan_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    hmac_scan_state_enum_uint8 pen_bgscan_enable_flag;

    pen_bgscan_enable_flag = *puc_param; /* ����ɨ��ֹͣʹ��λ */

    /* ����ɨ��ֹͣ���� */
    switch (pen_bgscan_enable_flag) {
        case 0:
            g_en_bgscan_enable_flag = HMAC_BGSCAN_DISABLE;
            break;
        case 1:
            g_en_bgscan_enable_flag = HMAC_BGSCAN_ENABLE;
            break;
        case 2:
            g_en_bgscan_enable_flag = HMAC_SCAN_DISABLE;
            break;
        default:
            g_en_bgscan_enable_flag = HMAC_BGSCAN_ENABLE;
            break;
    }

    oam_warning_log1(0, OAM_SF_SCAN, "hmac_bgscan_enable: g_en_bgscan_enable_flag=%d.",
                     g_en_bgscan_enable_flag);

    return OAL_SUCC;
}


uint32_t hmac_scan_start_dbac(mac_device_stru *pst_dev)
{
    uint8_t auc_cmd[32];
    uint16_t us_len;
    uint32_t ul_ret = OAL_FAIL;
    uint8_t uc_idx;
#define DBAC_START_STR     " dbac start"
#define DBAC_START_STR_LEN 11
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    mac_ioctl_alg_config_stru *pst_alg_config = (mac_ioctl_alg_config_stru *)auc_cmd;

    if (memcpy_s(auc_cmd + OAL_SIZEOF(mac_ioctl_alg_config_stru),
                        sizeof(auc_cmd) - OAL_SIZEOF(mac_ioctl_alg_config_stru),
                        (const int8_t *)DBAC_START_STR, 11) != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "hmac_scan_start_dbac::memcpy fail!");
        return OAL_FAIL;
    }
    auc_cmd[OAL_SIZEOF(mac_ioctl_alg_config_stru) + DBAC_START_STR_LEN] = 0;

    pst_alg_config->uc_argc = 2;
    pst_alg_config->auc_argv_offset[0] = 1;
    pst_alg_config->auc_argv_offset[1] = 6;

    for (uc_idx = 0; uc_idx < pst_dev->uc_vap_num; uc_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_dev->auc_vap_id[uc_idx]);
        if (pst_mac_vap != OAL_PTR_NULL &&
            pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            break;
        }
    }
    if (pst_mac_vap) {
        us_len = OAL_SIZEOF(mac_ioctl_alg_config_stru) + DBAC_START_STR_LEN + 1;
        ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ALG, us_len, auc_cmd);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                             "{hmac_config_alg::hmac_config_send_event failed[%d].}", ul_ret);
        }
        oal_io_print("start dbac\n");
    } else {
        oal_io_print("no vap found to start dbac\n");
    }

    return ul_ret;
}

uint32_t hmac_start_all_bss_of_device(hmac_device_stru *pst_hmac_dev)
{
    uint8_t uc_idx;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_dev = pst_hmac_dev->pst_device_base_info;

    oam_warning_log1(0, OAM_SF_ACS, "{hmac_start_all_bss_of_device:device id=%d}",
                     pst_hmac_dev->pst_device_base_info->uc_device_id);
    if (oal_unlikely(pst_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_start_all_bss_of_device::pst_device_base_info null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_idx = 0; uc_idx < pst_dev->uc_vap_num; uc_idx++) {
        pst_hmac_vap = mac_res_get_hmac_vap(pst_dev->auc_vap_id[uc_idx]);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            oam_warning_log2(0, OAM_SF_ACS, "hmac_start_all_bss_of_device:null ap, idx=%d id=%d",
                             uc_idx, pst_dev->auc_vap_id[uc_idx]);
            continue;
        }

        if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
            (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_AP_WAIT_START
             || (mac_is_dbac_enabled(pst_dev) && pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_PAUSE)
             || pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_UP)) {
            hmac_chan_start_bss(pst_hmac_vap, OAL_PTR_NULL, WLAN_PROTOCOL_BUTT);
        } else {
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ACS, "vap %d not started\n",
                             pst_dev->auc_vap_id[uc_idx]);
            continue;
        }
    }

    if (mac_is_dbac_enabled(pst_dev)) {
        return hmac_scan_start_dbac(pst_dev);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_get_pri_sec_chan(mac_bss_dscr_stru *pst_bss_dscr,
                                          uint32_t *pul_pri_chan,
                                          uint32_t *pul_sec_chan)
{
    *pul_pri_chan = *pul_sec_chan = 0;

    *pul_pri_chan = pst_bss_dscr->st_channel.uc_chan_number;

    oam_info_log1(0, OAM_SF_2040, "hmac_get_pri_sec_chan:pst_bss_dscr->st_channel.en_bandwidth = %d\n",
                  pst_bss_dscr->en_channel_bandwidth);

    if (pst_bss_dscr->en_channel_bandwidth == WLAN_BAND_WIDTH_40PLUS) {
        *pul_sec_chan = *pul_pri_chan + 4;
    } else if (pst_bss_dscr->en_channel_bandwidth == WLAN_BAND_WIDTH_40MINUS) {
        *pul_sec_chan = *pul_pri_chan - 4;
    } else {
        oam_warning_log1(0, OAM_SF_2040,
                         "hmac_get_pri_sec_chan: pst_bss_dscr is not support 40Mhz, *pul_sec_chan = %d\n",
                         *pul_sec_chan);
    }

    oam_info_log2(0, OAM_SF_2040, "*pul_pri_chan = %d, *pul_sec_chan = %d\n\n", *pul_pri_chan, *pul_sec_chan);

    return OAL_SUCC;
}


OAL_STATIC void hmac_switch_pri_sec(mac_channel_stru *pst_channel)
{
    if (pst_channel->en_bandwidth == WLAN_BAND_WIDTH_40PLUS) {
        pst_channel->uc_chan_number = pst_channel->uc_chan_number + 4;
        pst_channel->en_bandwidth = WLAN_BAND_WIDTH_40MINUS;
    } else if (pst_channel->en_bandwidth == WLAN_BAND_WIDTH_40MINUS) {
        pst_channel->uc_chan_number = pst_channel->uc_chan_number - 4;
        pst_channel->en_bandwidth = WLAN_BAND_WIDTH_40PLUS;
    } else {
        oam_warning_log1(0, OAM_SF_2040, "hmac_switch_pri_sec:en_bandwidth = %d\n not need obss scan\n",
                         pst_channel->en_bandwidth);
    }
}


OAL_STATIC oal_bool_enum_uint8 hmac_obss_check_40mhz_2g(mac_vap_stru *pst_mac_vap,
                                                        hmac_scan_record_stru *pst_scan_record,
                                                        mac_channel_stru *pst_dst_channel)
{
    uint32_t ul_pri_freq;
    uint32_t ul_sec_freq;
    uint32_t ul_affected_start;
    uint32_t ul_affected_end;

    uint32_t ul_pri;
    uint32_t ul_sec;
    uint32_t ul_sec_chan, ul_pri_chan;
    int8_t c_obss_rssi_th;

    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    *pst_dst_channel = pst_mac_vap->st_channel;
    c_obss_rssi_th = HMAC_OBSS_RSSI_TH;
    /* ��ȡ���ŵ������ŵ�����Ƶ�� */
    ul_pri_freq = (int32_t)g_ast_freq_map_2g[pst_mac_vap->st_channel.uc_chan_number - 1].us_freq;

    if (pst_mac_vap->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40PLUS) {
        ul_sec_freq = ul_pri_freq + 20;
    } else if (pst_mac_vap->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40MINUS) {
        ul_sec_freq = ul_pri_freq - 20;
    } else {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                         "hmac_obss_check_40mhz_2g:en_bandwidth = %d not need obss\n",
                         pst_mac_vap->st_channel.en_bandwidth);
        return OAL_TRUE;
    }

    /* 2.4G������,��ⷶΧ��40MHz��������Ƶ��Ϊ����,���Ҹ�25MHZ */
    ul_affected_start = ((ul_pri_freq + ul_sec_freq) >> 1) - 25;
    ul_affected_end = ((ul_pri_freq + ul_sec_freq) >> 1) + 25;

    oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                  "hmac_obss_check_40mhz_2g:40 MHz affected channel range: [%d, %d] MHz",
                  ul_affected_start, ul_affected_end);

    /* ��ȡɨ�����Ĺ���ṹ��ַ */
    pst_bss_mgmt = &(pst_scan_record->st_bss_mgmt);

    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ����ɨ�赽��bss��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);
        if (pst_bss_dscr == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_2040, "{hmac_obss_check_40mhz_2g::pst_bss_dscr is NULL}");
            continue;
        }

        if (pst_bss_dscr->c_rssi < c_obss_rssi_th) {
            continue;
        }
        ul_pri = (int32_t)g_ast_freq_map_2g[pst_bss_dscr->st_channel.uc_chan_number - 1].us_freq;
        ul_sec = ul_pri;

        oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                      "pst_bss_dscr->st_channel.uc_chan_number = %d, ul_pri = %d\n",
                      pst_bss_dscr->st_channel.uc_chan_number, ul_pri);

        /* ��ȡɨ�赽��BSS���ŵ���Ƶ����Ϣ */
        hmac_get_pri_sec_chan(pst_bss_dscr, &ul_pri_chan, &ul_sec_chan);

        /* ��BSSΪ40MHz����,������ŵ�Ƶ�� */
        if (ul_sec_chan) {
            if (ul_sec_chan < ul_pri_chan) {
                ul_sec = ul_pri - 20;
            } else {
                ul_sec = ul_pri + 20;
            }
        }

        if ((ul_pri < ul_affected_start || ul_pri > ul_affected_end) &&
            (ul_sec < ul_affected_start || ul_sec > ul_affected_end)) {
            continue; /* not within affected channel range */
        }

        if (ul_pri_freq != ul_pri) {  // �е�Ƶ���Ҳ���ͬһ���ŵ�����ǰAP����ʹ��40M
            oam_warning_log4(0, OAM_SF_2040,
                             "hmac_obss_check_40mhz_2g:40 MHz pri/sec <%d, %d >mismatch with BSS <%d, %d>\n",
                             ul_pri_freq, ul_sec_freq, ul_pri, ul_sec);
            /* ����� */
            oal_spin_unlock(&(pst_bss_mgmt->st_lock));

            // just trim bandwidth to 2G
            pst_dst_channel->en_bandwidth = WLAN_BAND_WIDTH_20M;

            return OAL_FALSE;
        }
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return OAL_TRUE;
}


OAL_STATIC oal_bool_enum_uint8 hmac_obss_check_40mhz_5g(mac_vap_stru *pst_mac_vap,
                                                        hmac_scan_record_stru *pst_scan_record,
                                                        mac_channel_stru *pst_dst_channel)
{
    uint32_t ul_pri_chan;
    uint32_t ul_sec_chan;
    uint32_t ul_pri_bss;
    uint32_t ul_sec_bss;
    uint32_t ul_bss_pri_chan;
    uint32_t ul_bss_sec_chan;
    uint8_t uc_match;
    uint8_t uc_inverse;
    uint8_t uc_pri_20_bss;
    uint8_t uc_sec_20_bss;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    int8_t c_obss_rssi_th;

    *pst_dst_channel = pst_mac_vap->st_channel;

    /* ��ȡ���ŵ��ʹ��ŵ� */
    ul_pri_chan = pst_mac_vap->st_channel.uc_chan_number;
    if (pst_mac_vap->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40PLUS) {
        ul_sec_chan = ul_pri_chan + 4;
    } else if (pst_mac_vap->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40MINUS) {
        ul_sec_chan = ul_pri_chan - 4;
    } else {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                         "hmac_obss_check_40mhz_5g: en_bandwidth = %d not need obss scan",
                         pst_mac_vap->st_channel.en_bandwidth);
        return OAL_TRUE;
    }

    /* ��ȡɨ�����Ĺ���ṹ��ַ */
    pst_bss_mgmt = &(pst_scan_record->st_bss_mgmt);

    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ���ڴ��ŵ���⵽Beacon, �������ŵ���û��, ����Ҫ���������ŵ� */
    ul_pri_bss = ul_sec_bss = 0;
    c_obss_rssi_th = HMAC_OBSS_RSSI_TH;

    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        if (pst_bss_dscr == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_2040, "{hmac_obss_check_40mhz_5g::pst_bss_dscr is NULL}");
            continue;
        }
        if (pst_bss_dscr->c_rssi < c_obss_rssi_th) {
            continue;
        }

        oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "hmac_obss_check_40mhz_5g:bss uc_channel_number = %d\n",
                      pst_bss_dscr->st_channel.uc_chan_number);
        if (pst_bss_dscr->st_channel.uc_chan_number == ul_pri_chan) {
            ul_pri_bss++;
        } else if (pst_bss_dscr->st_channel.uc_chan_number == ul_sec_chan) {
            ul_sec_bss++;
        }
    }

    if (ul_sec_bss && !ul_pri_bss) {
        oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_2040,
            "hmac_obss_check_40mhz_5g:Switch own primary and sec chan to get sec chan with no Beacons from other BSS");

        hmac_switch_pri_sec(pst_dst_channel);

        /* �˴����ν������ֱ�ӷ��ؼ���, ��hostapd-2.4.0�汾�޸� */
        /* �ͷ��� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));
        // ���ﲻ���ж��Ƿ���Ҫ�л���20M
        return OAL_TRUE;
    }

    /*
     * Match PRI/SEC channel with any existing HT40 BSS on the same
     * channels that we are about to use (if already mixed order in
     * existing BSSes, use own preference).
     */
    // �Ƿ����뵱ǰAP�����ŵ���ȫһ�»����෴��40M BSS������Ҳ��ȷ���Ƿ��������ŵ����ߴ��ŵ���20M BSS
    uc_match = OAL_FALSE;
    uc_inverse = OAL_FALSE;
    uc_pri_20_bss = OAL_FALSE;
    uc_sec_20_bss = OAL_FALSE;
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);
        if (pst_bss_dscr == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_2040, "{hmac_obss_check_40mhz_5g::pst_bss_dscr is NULL}");
            continue;
        }
        if (pst_bss_dscr->c_rssi < c_obss_rssi_th) {
            continue;
        }

        hmac_get_pri_sec_chan(pst_bss_dscr, &ul_bss_pri_chan, &ul_bss_sec_chan);
        if (ul_pri_chan == ul_bss_pri_chan && ul_bss_sec_chan == 0) {
            uc_pri_20_bss = OAL_TRUE;  // �����ڵ�ǰAP���ŵ���20M BSS
        }
        if (ul_sec_chan == ul_bss_pri_chan && ul_bss_sec_chan == 0) {
            uc_sec_20_bss = OAL_TRUE;  // �����ڵ�ǰAP���ŵ���20M BSS
        }
        if (ul_pri_chan == ul_bss_sec_chan &&
            ul_sec_chan == ul_bss_pri_chan) {
            uc_inverse = OAL_TRUE;  // �����뵱ǰAP�����ŵ��෴��
        }
        if (ul_pri_chan == ul_bss_pri_chan &&
            ul_sec_chan == ul_bss_sec_chan) {
            uc_match = OAL_TRUE;  // �����뵱ǰAP�����ŵ�һ�µ�
        }
    }
    // ���෴��40M,û��һ�µģ�����20û��20M BSS,���������ŵ�
    if (uc_match == OAL_FALSE && uc_inverse == OAL_TRUE && uc_pri_20_bss == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_2040,
                         "hmac_obss_check_40mhz_5g:switch own primary and secondary channel due to BSS overlap with\n");

        hmac_switch_pri_sec(pst_dst_channel);
    } else if (uc_match == OAL_FALSE && uc_inverse == OAL_TRUE &&
               uc_pri_20_bss == OAL_TRUE && uc_sec_20_bss == OAL_FALSE) {
        // ���෴��40M,û��һ�µģ�����20��20M BSS,��20û��20M BSS,��Ҫ���������ŵ������л���20M
        oam_warning_log0(0, OAM_SF_2040,
            "hmac_obss_check_40mhz_5g:switch own primary and secondary channel due to BSS overlap and to 20M\n");

        hmac_switch_pri_sec(pst_dst_channel);
        pst_dst_channel->en_bandwidth = WLAN_BAND_WIDTH_20M;
    } else if (uc_sec_20_bss == OAL_TRUE) {  // ���ŵ���20M BSS����Ҫ�л���20M
        oam_warning_log2(0, OAM_SF_2040, "hmac_obss_check_40mhz_5g:40 MHz pri/sec <%d, %d > to 20M\n",
                         ul_pri_chan, ul_sec_chan);
        pst_dst_channel->en_bandwidth = WLAN_BAND_WIDTH_20M;
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    return OAL_TRUE;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_obss_check_40mhz(mac_vap_stru *pst_mac_vap,
                                                                hmac_scan_record_stru *pst_scan_record,
                                                                mac_channel_stru *pst_dst_channel)
{
    return (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) ?
            hmac_obss_check_40mhz_2g(pst_mac_vap, pst_scan_record, pst_dst_channel) :
            hmac_obss_check_40mhz_5g(pst_mac_vap, pst_scan_record, pst_dst_channel);
}


uint32_t hmac_obss_init_scan_hook(hmac_scan_record_stru *pst_scan_record,
                                  hmac_device_stru *pst_dev)
{
    /*
    1����ǰVAP���ŵ���ΪĿ���ŵ�
    2���ж�֮������Ҫ����VAP���ŵ��ʹ���
    3����ǰʵ���У������Ǹ����ŵ��ţ�ֻ���Ĵ���
    4���ŵ������浽device�£�vap���ŵ��������£���init scan hook�м��д���
    */
    uint8_t uc_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_vap_stru *pst_vap[WLAN_BAND_BUTT] = { OAL_PTR_NULL, OAL_PTR_NULL };

    oam_warning_log0(0, OAM_SF_ACS, "hmac_obss_init_scan_hook run\n");

    /* 2G/5G vap ����һ�� */
    for (uc_idx = 0; uc_idx < pst_dev->pst_device_base_info->uc_vap_num; uc_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_dev->pst_device_base_info->auc_vap_id[uc_idx]);
        if (pst_mac_vap != OAL_PTR_NULL && pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            if ((pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) && !pst_vap[WLAN_BAND_2G]) {
                pst_vap[WLAN_BAND_2G] = pst_mac_vap;
            } else if ((pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) && !pst_vap[WLAN_BAND_5G]) {
                pst_vap[WLAN_BAND_5G] = pst_mac_vap;
            }
        }
    }

    for (uc_idx = 0; uc_idx < WLAN_BAND_BUTT; uc_idx++) {
        if (pst_vap[uc_idx]) {
            hmac_obss_check_40mhz(pst_vap[uc_idx], pst_scan_record, pst_dev->ast_best_channel + uc_idx);
            pst_vap[uc_idx]->st_ch_switch_info.en_user_pref_bandwidth = pst_dev->ast_best_channel[uc_idx].en_bandwidth;

            oam_warning_log4(pst_vap[uc_idx]->uc_vap_id, OAM_SF_2040,
                             "hmac_obss_init_scan_hook::before:ch=%d bw=%d, after :ch=%d bw=%d",
                             pst_vap[uc_idx]->st_channel.uc_chan_number,
                             pst_vap[uc_idx]->st_channel.en_bandwidth,
                             pst_dev->ast_best_channel[uc_idx].uc_chan_number,
                             pst_dev->ast_best_channel[uc_idx].en_bandwidth);
        }
    }

    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_device_in_init_scan(mac_device_stru *pst_mac_device)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_res_get_mac_dev:pst_hmac_device is null\n");
        return OAL_FALSE;
    } else {
        return pst_hmac_device->en_in_init_scan;
    }
}


typedef enum {
    MAC_INIT_SCAN_NOT_NEED,
    MAC_INIT_SCAN_NEED,
    MAC_INIT_SCAN_IN_SCAN,
} mac_need_init_scan_res;
typedef uint8_t mac_need_init_scan_res_enum_uint8;

 mac_need_init_scan_res_enum_uint8 hmac_need_init_scan(hmac_device_stru *pst_hmac_device,
                                                       mac_vap_stru *pst_in_mac_vap,
                                                       mac_try_init_scan_type en_type)
{
    mac_need_init_scan_res_enum_uint8 en_result = MAC_INIT_SCAN_NOT_NEED;
    mac_device_stru *pst_mac_device = pst_hmac_device->pst_device_base_info;

    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_need_init_scan::pst_device_base_info null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_in_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP && pst_in_mac_vap->en_vap_mode != WLAN_VAP_MODE_CONFIG) {
        return MAC_INIT_SCAN_NOT_NEED;
    }

    if (pst_hmac_device->en_in_init_scan) {
        return en_type == MAC_TRY_INIT_SCAN_RESCAN ? MAC_INIT_SCAN_NOT_NEED : MAC_INIT_SCAN_IN_SCAN;
    }

#ifdef _PRE_WLAN_FEATURE_DFS
    /*lint -e514*/
    en_result |= (mac_vap_get_dfs_enable(pst_in_mac_vap) &&
                  mac_dfs_get_cac_enable(pst_hmac_device->pst_device_base_info) &&
                  hmac_dfs_need_for_cac(pst_mac_device, pst_in_mac_vap)) ? OAL_TRUE : OAL_FALSE;
    /*lint +e514*/
#endif

    if (!pst_in_mac_vap->bit_bw_fixed) {
        en_result |= (mac_get_2040bss_switch(pst_mac_device) &&
                      (pst_in_mac_vap->st_channel.en_bandwidth != WLAN_BAND_WIDTH_20M)) ? OAL_TRUE : OAL_FALSE;
    }

    return en_result;
}


uint32_t hmac_init_scan_sync_channel(hmac_scan_record_stru *pst_scan_record,
                                     hmac_device_stru *pst_hmac_dev)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_channel_stru *pst_channel = OAL_PTR_NULL;
    uint8_t uc_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_acs_en = OAL_FALSE;
    oal_bool_enum_uint8 en_obss_en = OAL_FALSE;
    if (oal_any_null_ptr3(pst_scan_record, pst_hmac_dev, pst_hmac_dev->pst_device_base_info)) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_init_scan_sync_channel:null ptr\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = pst_hmac_dev->pst_device_base_info;
    pst_channel = pst_hmac_dev->ast_best_channel;

    // just in case someone missing
    for (uc_idx = 0; uc_idx < WLAN_BAND_BUTT; uc_idx++) {
        mac_get_channel_idx_from_num(pst_channel[uc_idx].en_band,
                                         pst_channel[uc_idx].uc_chan_number,
                                         &pst_channel[uc_idx].uc_chan_idx);
    }

    /*
     * ACSδʹ�ܡ�OBSSʹ��ʱ��ʹ��dev�����õ��ŵ�
     * ACSʹ�ܣ�  δomit acsʱ��ʹ��dev�����õ��ŵ�
     *              omit acsʱ��ʹ��ԭvap�ŵ�������OBSS���
     */
    for (uc_idx = 0; uc_idx < pst_mac_device->uc_vap_num; uc_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_idx]);
        if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
            continue;
        }

        if (mac_get_2040bss_switch(pst_mac_device) && (!pst_mac_vap->bit_bw_fixed)) {
            en_obss_en = OAL_TRUE;
        }

        if (!en_acs_en && !en_obss_en) {
            continue;
        }

        if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP)
            && (pst_mac_vap->en_vap_state == MAC_VAP_STATE_AP_WAIT_START)) {
            // ��dev��ѡ����ŵ��Ϸ������串�ǵ�vap�ṹ��
            if (pst_channel[pst_mac_vap->st_channel.en_band].uc_chan_number) {
                pst_mac_vap->st_channel = pst_channel[pst_mac_vap->st_channel.en_band];
                oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                                 "hmac_init_scan_sync_channel:vap ch info(band=%d,bw=%d,ch=%d) sync from acs\n",
                                 pst_mac_vap->st_channel.en_band,
                                 pst_mac_vap->st_channel.en_bandwidth,
                                 pst_mac_vap->st_channel.uc_chan_number);
            } else {
                oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                                 "hmac_init_scan_sync_channel:no acs channel found for band %d\n",
                                 pst_mac_vap->st_channel.en_band);
            }
        }
    }

    return OAL_SUCC;
}


uint32_t hmac_init_scan_timeout(void *p_arg)
{
    hmac_device_stru *pst_dev = (hmac_device_stru *)p_arg;

    if (!pst_dev->en_init_scan) {
        return OAL_SUCC;
    }

    pst_dev->en_init_scan = OAL_FALSE;
    pst_dev->en_in_init_scan = OAL_FALSE;
    hmac_start_all_bss_of_device(pst_dev);

    return OAL_SUCC;
}


uint32_t hmac_init_scan_cancel_timer(hmac_device_stru *pst_hmac_dev)
{
    if (pst_hmac_dev != OAL_PTR_NULL && pst_hmac_dev->st_scan_mgmt.st_init_scan_timeout.en_is_registerd) {
        /* �رճ�ʱ��ʱ�� */
        frw_timer_immediate_destroy_timer_m(&pst_hmac_dev->st_scan_mgmt.st_init_scan_timeout);
    }

    return OAL_SUCC;
}


void hmac_init_scan_cb(void *p_scan_record)
{
    hmac_scan_record_stru *pst_scan_record = (hmac_scan_record_stru *)p_scan_record;
    uint8_t uc_device_id = pst_scan_record->uc_device_id;
    hmac_device_stru *pst_hmac_dev = hmac_res_get_mac_dev(uc_device_id);
    mac_device_stru *pst_mac_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    oam_warning_log0(0, OAM_SF_ACS, "{hmac_init_scan_cb called}");

    if (pst_hmac_dev == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ACS, "{hmac_init_scan_cb:pst_hmac_dev=NULL, device_id=%d}", uc_device_id);
        return;
    }

    pst_mac_dev = pst_hmac_dev->pst_device_base_info;
    if (oal_any_null_ptr2(pst_hmac_dev, pst_mac_dev)) {
        oam_error_log1(0, OAM_SF_ACS, "{hmac_init_scan_cb:pst_mac_dev=NULL, device_id=%d}", uc_device_id);
        return;
    }

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_scan_record->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ACS, "{hmac_init_scan_cb:pst_mac_vap=NULL, vap_id=%d}",
                       pst_scan_record->uc_vap_id);
        return;
    }

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_CHAN, "{hmac_init_scan_cb::scan report ch=%d bss=%d init=%d}\n",
                     pst_scan_record->uc_chan_numbers,
                     pst_scan_record->st_bss_mgmt.ul_bss_num, pst_hmac_dev->en_init_scan);

    oam_warning_log0(0, OAM_SF_ACS, "acs not enable, cancel timer\n");
    /* ACSδִ�У���ʱ�������˽��� */
    hmac_init_scan_cancel_timer(pst_hmac_dev);

    /* ��ACSδִ�л���ִ��ʧ�ܣ�����ִ�к������� */
    if (mac_get_2040bss_switch(pst_mac_dev) && (!pst_mac_vap->bit_bw_fixed)) {
        hmac_obss_init_scan_hook(pst_scan_record, pst_hmac_dev);
    }

    /*
     * ���ˣ��ŵ�����ȷ�ϣ���������device����δͬ����vap���ŵ��ṹ��
     * �˴�ͬ������¼ԭʼ�ŵ���ͬʱ��֤CAC���������ŵ���ִ��
     * �����������ŵ���Ӳ�����ɺ����������
     */
    hmac_init_scan_sync_channel(pst_scan_record, pst_hmac_dev);

#ifdef _PRE_WLAN_FEATURE_DFS
    if (mac_vap_get_dfs_enable(pst_mac_vap)) {
        /* ���ɹ���ʼ��CAC�����Ѿ���ʼCAC�����أ���CAC��ʱ��������VAP START */
        if (hmac_dfs_init_scan_hook(pst_scan_record, pst_hmac_dev) == OAL_SUCC) {
            return;
        }
    }
#endif

    /* ACSδ���С�DFSδ���У�ֱ������BSS */
    hmac_init_scan_timeout(pst_hmac_dev);
}


uint32_t hmac_init_scan_do(hmac_device_stru *pst_hmac_dev, mac_vap_stru *pst_mac_vap,
                           mac_init_scan_req_stru *pst_cmd, hmac_acs_cfg_stru *pst_acs_cfg)
{
    uint8_t uc_idx, uc_cnt;
    mac_scan_req_h2d_stru st_h2d_scan_req;
    wlan_channel_band_enum_uint8 en_band;
    wlan_channel_bandwidth_enum_uint8 en_bandwidth;
    uint32_t ul_ret = OAL_FAIL;
    hmac_vap_stru *pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);

    if (oal_any_null_ptr2(pst_hmac_vap, pst_acs_cfg)) {
        oam_error_log1(0, OAM_SF_ACS, "{hmac_init_scan_do:get hmac vap=NULL or acs_cfg is NULL, vapid=%d}",
                       pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    memset_s(&st_h2d_scan_req, OAL_SIZEOF(st_h2d_scan_req), 0, OAL_SIZEOF(st_h2d_scan_req));
    st_h2d_scan_req.st_scan_params.en_scan_mode = pst_cmd->auc_arg[0];
    st_h2d_scan_req.st_scan_params.en_scan_type = pst_cmd->auc_arg[1];
    st_h2d_scan_req.st_scan_params.uc_scan_func = pst_cmd->auc_arg[2];
    st_h2d_scan_req.st_scan_params.en_bss_type = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    st_h2d_scan_req.st_scan_params.uc_bssid_num = 0;
    st_h2d_scan_req.st_scan_params.uc_ssid_num = 0;

    st_h2d_scan_req.st_scan_params.uc_max_scan_count_per_channel = 1;
    st_h2d_scan_req.st_scan_params.uc_max_send_probe_req_count_per_channel = st_h2d_scan_req.st_scan_params.en_scan_mode == WLAN_SCAN_MODE_FOREGROUND ? 2 : 1;

    st_h2d_scan_req.st_scan_params.us_scan_time = st_h2d_scan_req.st_scan_params.en_scan_mode == WLAN_SCAN_MODE_FOREGROUND ? 120 : 30;
    st_h2d_scan_req.st_scan_params.uc_probe_delay = 0;
    st_h2d_scan_req.st_scan_params.uc_vap_id = pst_mac_vap->uc_vap_id; /* ��ʵ�Ǹ�device�µ�vap_id[0] */
    st_h2d_scan_req.st_scan_params.p_fn_cb = hmac_init_scan_cb;

    st_h2d_scan_req.st_scan_params.uc_channel_nums = 0; /* �ŵ��б����ŵ��ĸ��� */
    uc_cnt = 0;
    for (uc_idx = 0; uc_idx < pst_cmd->ul_cmd_len; uc_idx += 2) {
        en_band = pst_cmd->auc_data[uc_idx] & 0X0F;
        en_bandwidth = (pst_cmd->auc_data[uc_idx] >> 4) & 0x0F;
        if (mac_is_channel_num_valid(en_band, pst_cmd->auc_data[uc_idx + 1]) != OAL_SUCC) {
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ACS,
                             "{hmac_init_scan_do:invalid channel number, ch=%d, band=%d bw=%d}",
                             pst_cmd->auc_data[uc_idx + 1], en_band, en_bandwidth);
            oal_io_print("{hmac_init_scan_do:invalid channel number, ch=%d, band=%d bw=%d}\n",
                         pst_cmd->auc_data[uc_idx + 1], en_band, en_bandwidth);

            continue;
        }

        st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].uc_chan_number = pst_cmd->auc_data[uc_idx + 1];
        st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].en_band = en_band;
        st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].en_bandwidth = en_bandwidth;
        ul_ret = mac_get_channel_idx_from_num(st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].en_band,
            st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].uc_chan_number,
            &st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].uc_chan_idx);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ACS,
                "{hmac_init_scan_do:mac_get_channel_idx_from_num failed en_band:[%d],chan_number:[%d]}",
                st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].en_band, st_h2d_scan_req.st_scan_params.ast_channel_list[uc_cnt].uc_chan_number);
        }
        uc_cnt++;
    }

    oal_io_print("hmac_init_scan_do::got=5:do scan mode=%d func=0x%x type=%d ch_cnt=%d\n",
                 st_h2d_scan_req.st_scan_params.en_scan_mode,
                 st_h2d_scan_req.st_scan_params.uc_scan_func, st_h2d_scan_req.st_scan_params.en_scan_type, uc_cnt);

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                     "hmac_init_scan_do::got=5:do scan mode=%d func=0x%x type=%d ch_cnt=%d\n",
                     st_h2d_scan_req.st_scan_params.en_scan_mode,
                     st_h2d_scan_req.st_scan_params.uc_scan_func, st_h2d_scan_req.st_scan_params.en_scan_type, uc_cnt);

    if (uc_cnt != 0) {
        st_h2d_scan_req.st_scan_params.uc_channel_nums = uc_cnt;

        /* ֱ�ӵ���ɨ��ģ��ɨ���������� */
        ul_ret = hmac_scan_proc_scan_req_event(pst_hmac_vap, &st_h2d_scan_req);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "hmac_init_scan_do:hmac_scan_add_req failed, ret=%d", ul_ret);
        }
    } else {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_init_scan_do:no valid channel found, not scan");
    }

    return ul_ret;
}


oal_bool_enum_uint8 hmac_init_scan_skip_channel(hmac_device_stru *pst_hmac_dev,
                                                wlan_channel_band_enum_uint8 en_band,
                                                uint8_t uc_idx)
{
    /* skip any illegal channel */
    if (mac_is_channel_idx_valid(en_band, uc_idx) != OAL_SUCC) {
        return OAL_TRUE;
    }
    if (oal_unlikely(pst_hmac_dev->pst_device_base_info == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_init_scan_skip_channel::pst_device_base_info null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_dev->pst_device_base_info
        && pst_hmac_dev->pst_device_base_info->en_max_band != en_band) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


uint32_t hmac_init_scan_process(hmac_device_stru *pst_hmac_dev,
                                mac_vap_stru *pst_mac_vap,
                                hmac_acs_cfg_stru *pst_acs_cfg)
{
    uint8_t ast_buf[OAL_SIZEOF(mac_acs_cmd_stru) - 4 + WLAN_MAX_CHANNEL_NUM * 2];
    uint8_t uc_idx;
    uint8_t *puc_dat, uc_tot, uc_chan_num;
    uint32_t ul_ret;

    mac_init_scan_req_stru *pst_cmd = (mac_init_scan_req_stru *)ast_buf;

    puc_dat = pst_cmd->auc_data;

    pst_hmac_dev->en_init_scan = (pst_acs_cfg->en_scan_op == MAC_SCAN_OP_INIT_SCAN) ? OAL_TRUE : OAL_FALSE;
    pst_hmac_dev->en_in_init_scan = OAL_TRUE;

    pst_cmd->uc_cmd = 5;
    pst_cmd->auc_arg[0] = (pst_acs_cfg->en_scan_op == MAC_SCAN_OP_FG_SCAN_ONLY ||
        pst_acs_cfg->en_scan_op == MAC_SCAN_OP_INIT_SCAN) ? WLAN_SCAN_MODE_FOREGROUND : WLAN_SCAN_MODE_BACKGROUND_AP;

    pst_cmd->auc_arg[1] = WLAN_SCAN_TYPE_ACTIVE;
    pst_cmd->auc_arg[2] = MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS | MAC_SCAN_FUNC_BSS;
    pst_cmd->ul_cmd_cnt = 0;

    uc_tot = 0;
    for (uc_idx = 0; uc_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_idx++) {
        if (!hmac_init_scan_skip_channel(pst_hmac_dev, WLAN_BAND_2G, uc_idx)) {
            mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_idx, &uc_chan_num);
            *puc_dat++ = ((WLAN_BAND_WIDTH_20M) << 4) | WLAN_BAND_2G;
            *puc_dat++ = uc_chan_num;
            uc_tot++;
        }
    }
    for (uc_idx = 0; uc_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_idx++) {
        if (!hmac_init_scan_skip_channel(pst_hmac_dev, WLAN_BAND_5G, uc_idx)) {
            mac_get_channel_num_from_idx(WLAN_BAND_5G, uc_idx, &uc_chan_num);
            *puc_dat++ = ((WLAN_BAND_WIDTH_20M) << 4) | WLAN_BAND_5G;
            *puc_dat++ = uc_chan_num;
            uc_tot++;
        }
    }

    pst_cmd->ul_cmd_len = uc_tot * 2;

    /* best�ŵ����Ϊ0����start bss��Ϊ�Ϸ����ж����� */
    if (pst_hmac_dev->en_init_scan) {
        memset_s(pst_hmac_dev->ast_best_channel, OAL_SIZEOF(pst_hmac_dev->ast_best_channel),
                 0, OAL_SIZEOF(pst_hmac_dev->ast_best_channel));
    }

    /* ������ʱ��, ��ʱ��ǿ������BSS: ����ʱ�����ɨ��ʱ����APP����ʱ�� */
    {
        frw_timer_create_timer_m(&pst_hmac_dev->st_scan_mgmt.st_init_scan_timeout,
                               hmac_init_scan_timeout,
                               HMAC_INIT_SCAN_TIMEOUT_MS,
                               pst_hmac_dev,
                               OAL_FALSE,
                               OAM_MODULE_ID_HMAC,
                               pst_hmac_dev->pst_device_base_info->ul_core_id);
    }

    ul_ret = hmac_init_scan_do(pst_hmac_dev, pst_mac_vap, pst_cmd, pst_acs_cfg);
    if (ul_ret != OAL_SUCC) {
        pst_hmac_dev->en_init_scan = OAL_FALSE;
        pst_hmac_dev->en_in_init_scan = OAL_FALSE;
    }

    return ul_ret;
}


uint32_t hmac_init_scan_try(mac_device_stru *pst_mac_device, mac_vap_stru *pst_in_mac_vap,
                            mac_try_init_scan_type en_type, hmac_acs_cfg_stru *pst_acs_cfg)
{
    uint8_t uc_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap_scan = NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t en_scan_type;
    hmac_device_stru *pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    oal_bool_enum_uint8 en_need_do_init_scan = OAL_FALSE;

    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_SCAN, "{hmac_init_scan_try:mac_res_get_hmac_vap failed. device_id:%d.}",
                         pst_mac_device->uc_device_id);
        return OAL_FAIL;
    }
    en_scan_type = hmac_need_init_scan(pst_hmac_device, pst_in_mac_vap, en_type);
    if (en_scan_type == MAC_INIT_SCAN_NOT_NEED) {
        return OAL_FAIL;
    } else if (en_scan_type == MAC_INIT_SCAN_IN_SCAN) {
        oal_io_print("just in init scan\n");

        mac_vap_init_rates(pst_in_mac_vap);
        pst_hmac_vap = mac_res_get_hmac_vap(pst_in_mac_vap->uc_vap_id);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            oam_warning_log1(0, OAM_SF_SCAN, "{hmac_init_scan_try:mac_res_get_hmac_vap failed vap_id:%d.}",
                             pst_in_mac_vap->uc_vap_id);
            return OAL_FAIL;
        }
        if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_UP) {
            hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_AP_WAIT_START);
        }

        return OAL_SUCC;
    } else {
        oam_warning_log0(0, OAM_SF_SCAN, "hmac_init_scan_try: need init scan");
    }

    for (uc_idx = 0; uc_idx < pst_hmac_device->pst_device_base_info->uc_vap_num; uc_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_hmac_device->pst_device_base_info->auc_vap_id[uc_idx]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            continue;
        }

        if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            mac_vap_init_rates(pst_mac_vap);

            /* ǿ������AP��״̬��Ϊ WAIT_START����Ϊ��Ҫִ�г�ʼ�ŵ���� */
            pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
            if (pst_hmac_vap == OAL_PTR_NULL) {
                oam_warning_log1(0, OAM_SF_SCAN,
                                 "{hmac_init_scan_try::need init scan fail.vap_id = %u}",
                                 pst_mac_vap->uc_vap_id);
                continue;
            }
            switch (pst_hmac_vap->st_vap_base_info.en_vap_state) {
                case MAC_VAP_STATE_UP:
                case MAC_VAP_STATE_PAUSE:  // dbac
                    hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_AP_WAIT_START);
                // no break here!
                case MAC_VAP_STATE_AP_WAIT_START:
                    en_need_do_init_scan = OAL_TRUE;
                    pst_mac_vap_scan = pst_mac_vap;
                    break;
                default:
                    break;
            }
        }
    }

    if (en_need_do_init_scan) {
        return hmac_init_scan_process(pst_hmac_device, pst_mac_vap_scan, pst_acs_cfg);
    }

    return OAL_SUCC;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_scan_find_scanned_bss_dscr_by_index);
oal_module_symbol(hmac_scan_find_scanned_bss_by_bssid);
/*lint -e578*/ /*lint -e19*/

