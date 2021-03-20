

/* 1 ͷ�ļ����� */
#include "oal_net.h"
#include "oal_cfg80211.h"
#include "wlan_spec.h"
#include "wal_linux_event.h"
#include "wal_linux_scan.h"
#include "wal_linux_cfg80211.h"
#include "wal_main.h"
#include "wal_linux_rx_rsp.h"
#include "hmac_vap.h"
#include "hmac_device.h"
#include "mac_device.h"
#include "hmac_resource.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <net/cfg80211.h>
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)

#endif
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_SCAN_C
/* 2 ȫ�ֱ������� */
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
uint8_t g_country_code_result[COUNTRY_CODE_LEN];
oal_bool_enum_uint8 g_country_code_result_update_flag = OAL_TRUE;
oal_bool_enum_uint8 g_country_code_self_study_flag = OAL_TRUE;
#endif
/* 3 ����ʵ�� */

OAL_STATIC void wal_inform_bss_frame(wal_scanned_bss_info_stru *pst_scanned_bss_info, void *p_data)
{
    oal_cfg80211_bss_stru *pst_cfg80211_bss = OAL_PTR_NULL;
    oal_wiphy_stru *pst_wiphy = OAL_PTR_NULL;
    oal_ieee80211_channel_stru *pst_ieee80211_channel = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_scanned_bss_info, p_data)) {
        oam_error_log2(0, OAM_SF_SCAN,
            "{wal_inform_bss_frame::input param pointer is null, pst_scanned_bss_info[%p], p_data[%p]!}",
            (uintptr_t)pst_scanned_bss_info, (uintptr_t)p_data);
        return;
    }

    pst_wiphy = (oal_wiphy_stru *)p_data;

    pst_ieee80211_channel = oal_ieee80211_get_channel(pst_wiphy, (int32_t)pst_scanned_bss_info->s_freq);
    if (pst_ieee80211_channel == NULL) {
        oam_warning_log1(0, OAM_SF_SCAN, "{wal_inform_bss_frame::get channel failed, wrong s_freq[%d]}",
                         (int32_t)pst_scanned_bss_info->s_freq);
        return;
    }

    pst_scanned_bss_info->l_signal = pst_scanned_bss_info->l_signal * 100;

    /* ����ϱ��ں�bss ��Ϣ */
    pst_cfg80211_bss = oal_cfg80211_inform_bss_frame(pst_wiphy, pst_ieee80211_channel,
                                                         pst_scanned_bss_info->pst_mgmt,
                                                         pst_scanned_bss_info->ul_mgmt_len,
                                                         pst_scanned_bss_info->l_signal, GFP_ATOMIC);
    if (pst_cfg80211_bss != NULL) {
        oal_cfg80211_put_bss(pst_wiphy, pst_cfg80211_bss);
    }

    return;
}


void wal_update_bss(oal_wiphy_stru *pst_wiphy,
                            hmac_bss_mgmt_stru *pst_bss_mgmt,
                            uint8_t *puc_bssid)
{
    wal_scanned_bss_info_stru st_scanned_bss_info;
    oal_cfg80211_bss_stru *pst_cfg80211_bss = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint8_t uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    enum nl80211_band en_band;
#else
    enum ieee80211_band en_band;
#endif
    int32_t l_channel;
    oal_bool_enum_uint8 en_inform_bss = OAL_FALSE;

    if (oal_any_null_ptr3(pst_wiphy, pst_bss_mgmt, puc_bssid)) {
        oam_warning_log3(0, OAM_SF_ASSOC, "{wal_update_bss::null pointer.wiphy %p, bss_mgmt %p, bssid %p.",
                         (uintptr_t)pst_wiphy, (uintptr_t)pst_bss_mgmt, (uintptr_t)puc_bssid);
        return;
    }

    pst_bss_dscr = OAL_PTR_NULL;
    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* �ӱ���ɨ�����л�ȡbssid ��Ӧ��bss ��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);

        /* ��ɨ�����жԱȹ�����MAC ��ַ�������ͬ������ǰ�˳� */
        if (oal_memcmp(puc_bssid, pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) == 0) {
            break;
        }
        pst_bss_dscr = OAL_PTR_NULL;
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    if (pst_bss_dscr == OAL_PTR_NULL) {
        oam_warning_log4(0, OAM_SF_ASSOC, "{wal_update_bss::do not find correspond bss from scan result."
                         "%02X:XX:XX:%02X:%02X:%02X}",
                         puc_bssid[0], puc_bssid[3], puc_bssid[4], puc_bssid[5]);
        return;
    }

    /* �����Ƿ����ں��У����û�и�bss  �������bss ���������bss ʱ�� */
    pst_cfg80211_bss = oal_cfg80211_get_bss(pst_wiphy,
                                                OAL_PTR_NULL,
                                                puc_bssid,
                                                (uint8_t *)(pst_bss_dscr->ac_ssid),
                                                OAL_STRLEN(pst_bss_dscr->ac_ssid));
    if (pst_cfg80211_bss != OAL_PTR_NULL) {
        l_channel = (int32_t)oal_ieee80211_frequency_to_channel((int32_t)(pst_cfg80211_bss->channel->center_freq));

        /*
         * ����ϵ�ssid���ŵ���һ�£���Ҫunlink֮�������ϱ��µ�FRW���ں˻�ȡ��old�ŵ���
         * ��ɺ����ٴι�����Ϊ�ŵ�������޷������ɹ�
         */
        if (pst_bss_dscr->st_channel.uc_chan_number != (uint8_t)l_channel) {
            oam_warning_log2(0, OAM_SF_ASSOC,
                             "{wal_update_bss::current kernel bss channel[%d] need to update to channel[%d].",
                             (uint8_t)l_channel, pst_bss_dscr->st_channel.uc_chan_number);

            oal_cfg80211_unlink_bss(pst_wiphy, pst_cfg80211_bss);
            en_inform_bss = OAL_TRUE;
        } else {
            oal_cfg80211_put_bss(pst_wiphy, pst_cfg80211_bss);
        }
    } else {
        en_inform_bss = OAL_TRUE;
    }

    /* ��Ҫinform bss���ں� */
    if (en_inform_bss) {
        uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
        en_band = (enum nl80211_band)pst_bss_dscr->st_channel.en_band;
#else
        en_band = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;
#endif
        /* ��ʼ�� */
        memset_s(&st_scanned_bss_info, OAL_SIZEOF(wal_scanned_bss_info_stru),
                 0, OAL_SIZEOF(wal_scanned_bss_info_stru));

        /* ��дBSS �ź�ǿ�� */
        st_scanned_bss_info.l_signal = pst_bss_dscr->c_rssi;

        /* ��bss�����ŵ�������Ƶ�� */
        st_scanned_bss_info.s_freq = (int16_t)oal_ieee80211_channel_to_frequency(uc_chan_number, en_band);

        /* �����ָ֡��ͳ��� */
        st_scanned_bss_info.pst_mgmt = (oal_ieee80211_mgmt_stru *)(pst_bss_dscr->auc_mgmt_buff);
        st_scanned_bss_info.ul_mgmt_len = pst_bss_dscr->ul_mgmt_len;

        /* ��ȡ�ϱ���ɨ�����Ĺ���֡��֡ͷ */
        pst_frame_hdr = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

        /* ���ɨ��������յ���֡������beacon���ͣ�ͳһ�޸�Ϊprobe rsp�����ϱ���
           Ϊ�˽���ϱ��ں˵�ɨ����beacon֡�������е����⣬�����⣬��01���ֹ� */
        if (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_BEACON) {
            /* �޸�beacon֡����Ϊprobe rsp */
            pst_frame_hdr->st_frame_control.bit_sub_type = WLAN_PROBE_RSP;
        }

        wal_inform_bss_frame(&st_scanned_bss_info, pst_wiphy);
    }

    return;
}
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY

OAL_STATIC void wal_country_code_result(wal_country_code_stru *pst_wal_country_code,
                                            hmac_vap_stru *pst_hmac_vap)
{
    uint8_t     country_id;
    uint8_t     max_mun_country_id;
    uint8_t     max_count_num;
    uint8_t     second_max_count_num = 0;
    uint8_t     default_country_code[] = "99";

    max_count_num = pst_wal_country_code->wal_country_code_count[0].count_num;
    max_mun_country_id = 0;
    /* ���������ս�� */
    for (country_id = 1; country_id < pst_wal_country_code->country_type_num; country_id++) {
        if (pst_wal_country_code->wal_country_code_count[country_id].count_num >= max_count_num) {
            second_max_count_num = max_count_num;
            max_mun_country_id = country_id;
            max_count_num = pst_wal_country_code->wal_country_code_count[country_id].count_num;
        }
    }
    /* ��������ͳ�ƽ��Ϊ0�������ֵ������ȵ����������Ĭ�Ϲ�����99 */
    if (second_max_count_num == max_count_num || max_count_num == 0) {
        if (memcpy_s(pst_wal_country_code->country_code_result, OAL_COUNTRY_CODE_LAN,
            default_country_code, OAL_COUNTRY_CODE_LAN) != EOK) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_country_code_result: memcpy_s failed.\n");
        }
    } else {
        if (memcpy_s(pst_wal_country_code->country_code_result, OAL_COUNTRY_CODE_LAN,
            pst_wal_country_code->wal_country_code_count[max_mun_country_id].pc_country_code,
            OAL_COUNTRY_CODE_LAN) != EOK) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_country_code_result: memcpy_s failed.\n");
        }
    }
    /* WIFI�ϵ�󣬵�һ�εڶ����ϱ�ɨ���������¹����룬����ÿ�ϱ�ɨ����3�Σ�ȷ��һ�ι����� */
    pst_hmac_vap->uc_scan_upload_num++;
    if (pst_hmac_vap->uc_scan_upload_num > WAL_COUNTRY_CODE_UPDATE_COUNT ||
        pst_hmac_vap->uc_scan_upload_num == WAL_COUNTRY_CODE_UPDATE_FRIST ||
        pst_hmac_vap->uc_scan_upload_num == WAL_COUNTRY_CODE_UPDATE_SECOND) {
        pst_hmac_vap->uc_scan_upload_num = WAL_COUNTRY_CODE_UPDATE_NOMAL;
        /* �ж����¹������Ƿ����ı� */
        if (oal_compare_country_code(pst_wal_country_code->country_code_result, g_country_code_result) != 0) {
            if (memcpy_s(g_country_code_result, OAL_COUNTRY_CODE_LAN,
                pst_wal_country_code->country_code_result, OAL_COUNTRY_CODE_LAN) != EOK) {
                oam_warning_log0(0, OAM_SF_ANY, "wal_country_code_result: memcpy_s failed.\n");
            }
            g_country_code_result_update_flag = OAL_TRUE;
        } else {
            g_country_code_result_update_flag = OAL_FALSE;
        }
    }

    return;
}


OAL_STATIC oal_bool_enum_uint8 wal_country_code_is_alpha_upper(int8_t c_letter)
{
    if (c_letter >= 'A' && c_letter <= 'Z') {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

OAL_STATIC oal_bool_enum_uint8 wal_country_code_illegal_confim(uint8_t           *pc_country_code)
{
    if (wal_country_code_is_alpha_upper(pc_country_code[0]) && wal_country_code_is_alpha_upper(pc_country_code[1])) {
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

OAL_STATIC oal_bool_enum_uint8 wal_bssid_confirm(mac_bss_dscr_stru *pst_mac_bss_dscr_stru, uint8_t *puc_bssid_name,
                                                 uint8_t uc_bssid_num)
{
    uint8_t   bssid_name_id = 0;
    oal_bool_enum_uint8   bssid_name_exist_flag = OAL_FALSE;

    if (uc_bssid_num >= WAL_BSSID_MAX_NUM) {
        return bssid_name_exist_flag;
    }

    for (bssid_name_id = 0; bssid_name_id < uc_bssid_num; bssid_name_id++) {
        /* ��ͬ��bssid��ַ */
        if (oal_compare_mac_addr(pst_mac_bss_dscr_stru->auc_bssid,
            (puc_bssid_name + WLAN_MAC_ADDR_LEN * bssid_name_id)) == 0) {
            bssid_name_exist_flag = OAL_TRUE;
        }
    }

    if (bssid_name_exist_flag == OAL_FALSE) {
        if (memcpy_s((puc_bssid_name + uc_bssid_num * WLAN_MAC_ADDR_LEN), OAL_MAC_ADDRESS_LAN,
            pst_mac_bss_dscr_stru->auc_bssid, OAL_MAC_ADDRESS_LAN) != EOK) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_country_code_count: memcpy_s failed.\n");
        }
    }
    return !bssid_name_exist_flag;
}


OAL_STATIC void wal_country_code_count(mac_bss_dscr_stru *pst_mac_bss_dscr_stru,
                                           wal_country_code_stru *pst_wal_country_code)
{
    uint8_t     *pst_new_bssid_country_code;
    uint8_t     country_id;
    uint32_t    country_type_num;
    oal_bool_enum_uint8     country_code_exist_flag = OAL_FALSE;

    pst_new_bssid_country_code = pst_mac_bss_dscr_stru->ac_country;
    country_type_num = pst_wal_country_code->country_type_num;

    if (country_type_num >= WAL_MAX_COUNTRY_CODE_NUM) {
        return;
    }
    /* ������Ϸ���У�� */
    if (wal_country_code_illegal_confim(pst_new_bssid_country_code)) {
        return;
    }
    /* ������ͳ�� */
    for (country_id = 0; country_id < pst_wal_country_code->country_type_num; country_id++) {
        if (oal_compare_country_code(pst_wal_country_code->wal_country_code_count[country_id].pc_country_code,
            pst_new_bssid_country_code) == 0) {
            pst_wal_country_code->wal_country_code_count[country_id].count_num++;
            country_code_exist_flag = OAL_TRUE;
        }
    }
    if (country_code_exist_flag == OAL_FALSE) {
        if (memcpy_s(pst_wal_country_code->wal_country_code_count[country_type_num].pc_country_code,
            OAL_COUNTRY_CODE_LAN, pst_new_bssid_country_code, OAL_COUNTRY_CODE_LAN) != EOK) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_country_code_count: memcpy_s failed.\n");
            }
        pst_wal_country_code->wal_country_code_count[country_type_num].count_num++;
        pst_wal_country_code->country_type_num++;
    }
}

OAL_STATIC void wal_countrycode_selstudy_statis(mac_bss_dscr_stru *pst_bss_dscr,
                                                    uint8_t *puc_bssid_name,
                                                    wal_country_code_stru *pst_wal_country_code,
                                                    uint8_t uc_bssid_num)
{
    if (g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag == OAL_FALSE) {
        if (g_country_code_self_study_flag == OAL_TRUE) {
                /* ������ͳ�� */
                if (wal_bssid_confirm(pst_bss_dscr, puc_bssid_name, uc_bssid_num)) {
                    uc_bssid_num++;
                    wal_country_code_count(pst_bss_dscr, pst_wal_country_code);
                }
        }
    }
}

OAL_STATIC void wal_countrycode_selstudy_result(wal_country_code_stru* pst_wal_country_code,
                                                    hmac_vap_stru *pst_hmac_vap)
{
    if (g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag == OAL_FALSE) {
    /* ��������ȷ�� */
        if (g_country_code_self_study_flag == OAL_TRUE) {
            wal_country_code_result(pst_wal_country_code, pst_hmac_vap);
        }
    }
}

OAL_STATIC void wal_scan_comp_regdomain_update(hmac_vap_stru *pst_hmac_vap)
{
    int32_t l_ret = OAL_TRUE;
    uint8_t country_code_change = OAL_TRUE;

    if (g_country_code_self_study_flag == OAL_TRUE) {
        /* PC��������²��ԣ�WIFI�ϵ�ǰ����ɨ����¹����� */
        if (pst_hmac_vap->uc_scan_upload_num == WAL_COUNTRY_CODE_UPDATE_FRIST ||
            pst_hmac_vap->uc_scan_upload_num == WAL_COUNTRY_CODE_UPDATE_SECOND) {
            l_ret = wal_regdomain_update_selfstudy_country_code(pst_hmac_vap->pst_net_device,
                                                                    g_country_code_result);
            chr_exception_p(CHR_WIFI_COUNTRYCODE_UPDATE_EVENTID, &country_code_change, sizeof(uint8_t));
        }
        /* ����ÿ3��ɨ����й�����ȷ�� */
        if (g_country_code_result_update_flag == OAL_TRUE) {
            l_ret = wal_regdomain_update_selfstudy_country_code(pst_hmac_vap->pst_net_device,
                                                                    g_country_code_result);
            chr_exception_p(CHR_WIFI_COUNTRYCODE_UPDATE_EVENTID, &country_code_change, sizeof(uint8_t));
        }
        if (oal_unlikely(l_ret == -OAL_EFAIL)) {
            oam_error_log0(0, OAM_SF_ANY, "{wal_scan_comp_regdomain_update::regdomain update failed!}");
        }
    }
}

void wal_counrtycode_selfstudy_scan_comp(hmac_vap_stru *pst_hmac_vap)
{
    /* ��������� */
    if (g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag == OAL_FALSE) {
        wal_scan_comp_regdomain_update(pst_hmac_vap);
    }
}
#endif

void wal_update_bss_for_csa(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_net_device)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_cfg80211_bss_stru *pst_cfg80211_bss = OAL_PTR_NULL;
    int32_t l_channel = 0;

    if (oal_any_null_ptr2(pst_wiphy, pst_net_device)) {
        oam_warning_log2(0, OAM_SF_CHAN, "{wal_update_bss_for_csa::null pointer. wiphy %p, net_device %p}",
                         (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device);
        return;
    }

    pst_mac_vap = (mac_vap_stru *)oal_net_dev_priv(pst_net_device);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_CHAN, "{wal_update_bss_for_csa::can't get mac vap from netdevice priv data!}");
        return ;
    }

    if (oal_any_null_ptr1(pst_mac_vap->pst_mib_info)) {
        oam_warning_log1(0, OAM_SF_CHAN, "{wal_update_bss_for_csa::null pointer. pst_mib_info %p}",
                         (uintptr_t)pst_mac_vap->pst_mib_info);
        return;
    }

    /* ɾ��kernel��ԭ���ŵ���BSS */
    pst_cfg80211_bss = oal_cfg80211_get_bss(pst_wiphy,
                                                OAL_PTR_NULL,
                                                pst_mac_vap->auc_bssid,
                                                mac_mib_get_DesiredSSID(pst_mac_vap),
                                                OAL_STRLEN(mac_mib_get_DesiredSSID(pst_mac_vap)));
    if (pst_cfg80211_bss != OAL_PTR_NULL) {
        l_channel = (int32_t)oal_ieee80211_frequency_to_channel((int32_t)(pst_cfg80211_bss->channel->center_freq));

        oam_warning_log1(0, OAM_SF_CHAN,
                         "{wal_update_bss_for_csa::csa unlink kernel bss, channel[%d]}", (uint8_t)l_channel);

        oal_cfg80211_unlink_bss(pst_wiphy, pst_cfg80211_bss);
    }

    return;
}


OAL_STATIC void wal_inform_all_bss_init(wal_scanned_bss_info_stru *pst_scanned_bss_info,
                                            mac_bss_dscr_stru *pst_bss_dscr)
{
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    uint8_t uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    enum nl80211_band en_band;
#else
    enum ieee80211_band en_band;
#endif

    uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    en_band = (enum nl80211_band)pst_bss_dscr->st_channel.en_band;
#else
    en_band = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;
#endif

    pst_scanned_bss_info->l_signal = pst_bss_dscr->c_rssi;

    /* ��bss�����ŵ�������Ƶ�� */
    pst_scanned_bss_info->s_freq = (int16_t)oal_ieee80211_channel_to_frequency(uc_chan_number, en_band);

    /* �����ָ֡��ͳ��� */
    pst_scanned_bss_info->pst_mgmt = (oal_ieee80211_mgmt_stru *)(pst_bss_dscr->auc_mgmt_buff);
    pst_scanned_bss_info->ul_mgmt_len = pst_bss_dscr->ul_mgmt_len;

    /* ��ȡ�ϱ���ɨ�����Ĺ���֡��֡ͷ */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

    /* ���ɨ��������յ���֡������beacon���ͣ�ͳһ�޸�Ϊprobe rsp�����ϱ���
       Ϊ�˽���ϱ��ں˵�ɨ����beacon֡�������е����⣬�����⣬��01���ֹ� */
    if (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_BEACON) {
        /* �޸�beacon֡����Ϊprobe rsp */
        pst_frame_hdr->st_frame_control.bit_sub_type = WLAN_PROBE_RSP;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    /* ɨ��ʹ����ʵʱ����ϱ�,���´�û��ɨ�赽��AP,���ϱ�ʱ�����ӹ̶�ֵ */
    pst_scanned_bss_info->pst_mgmt->u.probe_resp.timestamp = pst_bss_dscr->ul_wpa_rpt_time;
    pst_bss_dscr->ul_wpa_rpt_time += WLAN_BOOTTIME_REFLUSH;
#endif

}

OAL_STATIC uint32_t wal_channel_num_valid(mac_bss_dscr_stru *pst_bss_dscr)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    enum nl80211_band en_band;
#else
    enum ieee80211_band en_band;
#endif
    uint8_t uc_chan_number;
    uint32_t ul_ret;

    uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    en_band = (enum nl80211_band)pst_bss_dscr->st_channel.en_band;
#else
    en_band = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;
#endif

    /* �ж��ŵ��ǲ����ڹ������ڣ�������ڣ����ϱ��ں� */
    ul_ret = mac_is_channel_num_valid(en_band, uc_chan_number);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log2(0, OAM_SF_SCAN,
                         "{wal_inform_all_bss::curr channel[%d] and band[%d] is not in regdomain.}",
                         uc_chan_number, en_band);
    }
    return ul_ret;
}

void wal_inform_all_bss(oal_wiphy_stru *pst_wiphy, hmac_bss_mgmt_stru *pst_bss_mgmt, uint8_t uc_vap_id)
{
    uint8_t *puc_p2p_ie = OAL_PTR_NULL;
    uint16_t us_offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    hmac_scanned_bss_info *pst_scanned_bss = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    wal_scanned_bss_info_stru st_scanned_bss_info;
    uint32_t ul_ret;
    uint32_t ul_bss_num_not_in_regdomain = 0;
    uint32_t ul_bss_num = 0;

#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_voe_11r_auth;
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    wal_country_code_stru       wal_country_code;
    uint8_t                       uc_bssid_name[WAL_BSSID_MAX_NUM][WLAN_MAC_ADDR_LEN];
    uint8_t                       uc_bssid_num = 0;
    /* ��ʼ�� */
    memset_s(&wal_country_code, OAL_SIZEOF(wal_country_code_stru), 0, OAL_SIZEOF(wal_country_code_stru));
#endif
    /* ��ȡhmac vap */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(uc_vap_id, OAM_SF_SCAN, "{wal_inform_all_bss::hmac_vap is null, vap_id[%d]!}");
        return;
    }

    uc_voe_11r_auth = pst_hmac_vap->bit_voe_11r_auth;
    if (uc_voe_11r_auth == 1) {
        oam_warning_log1(uc_vap_id, OAM_SF_SCAN, "{wal_inform_all_bss::uc_11r_auth=[%d]!}", uc_voe_11r_auth);
    }
#endif
    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ����ɨ�赽��bss��Ϣ */
    oal_dlist_search_for_each(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = oal_dlist_get_entry(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        if (pst_scanned_bss == OAL_PTR_NULL) {
            oam_error_log0(uc_vap_id, OAM_SF_SCAN, "{wal_inform_all_bss::pst_scanned_bss is null. }");
            continue;
        }
        pst_bss_dscr = &(pst_scanned_bss->st_bss_dscr_info);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
        /* ��������ѧϰͳ�� */
        wal_countrycode_selstudy_statis(pst_bss_dscr, &uc_bssid_name[0][0], &wal_country_code, uc_bssid_num);
#endif
        /* �ж��ŵ��ǲ����ڹ������ڣ�������ڣ����ϱ��ں� */
        ul_ret = wal_channel_num_valid(pst_bss_dscr);
        if (ul_ret != OAL_SUCC) {
            ul_bss_num_not_in_regdomain++;
            continue;
        }
#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
        if (uc_voe_11r_auth == OAL_FALSE)
#endif
        {   /* voe 11r ��֤ʱ������ʱ������ */
            /* �ϱ�WAL_SCAN_REPORT_LIMIT���ڵ�ɨ���� */
            if (oal_time_after32(oal_time_get_stamp_ms(), (pst_bss_dscr->ul_timestamp + WAL_SCAN_REPORT_LIMIT))) {
                continue;
            }
        }

        puc_p2p_ie = mac_find_vendor_ie(MAC_WLAN_OUI_WFA, MAC_WLAN_OUI_TYPE_WFA_WFD,
                                            (pst_bss_dscr->auc_mgmt_buff + us_offset),
                                            (int32_t)(pst_bss_dscr->ul_mgmt_len - us_offset));

        if ((puc_p2p_ie != OAL_PTR_NULL) && (puc_p2p_ie[1] > MAC_P2P_MIN_IE_LEN)
            && (pst_bss_dscr->en_new_scan_bss == OAL_FALSE)) {
            oam_warning_log2(uc_vap_id, OAM_SF_SCAN,
                "wal_inform_all_bss::[%02X:%02X] include WFD ie, is an old scan result in 5s",
                pst_bss_dscr->auc_bssid[4], pst_bss_dscr->auc_bssid[5]);
            continue ;
        }

        /* ��ʼ�� */
        memset_s(&st_scanned_bss_info, OAL_SIZEOF(wal_scanned_bss_info_stru), 0, OAL_SIZEOF(wal_scanned_bss_info_stru));

        wal_inform_all_bss_init(&st_scanned_bss_info, pst_bss_dscr);

        /* �ϱ�ɨ�������ں� */
        wal_inform_bss_frame(&st_scanned_bss_info, pst_wiphy);
        ul_bss_num++;
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    oam_warning_log3(uc_vap_id, OAM_SF_SCAN,
        "{wal_inform_all_bss::there are %d bss not in regdomain, so inform kernel bss num is [%d] in [%d]!}",
        ul_bss_num_not_in_regdomain, ul_bss_num, (pst_bss_mgmt->ul_bss_num - ul_bss_num_not_in_regdomain));
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    /* ��������ѧϰ���ȷ�� */
    wal_countrycode_selstudy_result(&wal_country_code, pst_hmac_vap);
#endif
    return;
}


OAL_STATIC void free_scan_param_resource(mac_cfg80211_scan_param_stru *pst_scan_param)
{
    if (pst_scan_param->pul_channels_2G != OAL_PTR_NULL) {
        oal_free(pst_scan_param->pul_channels_2G);
        pst_scan_param->pul_channels_2G = OAL_PTR_NULL;
    }
    if (pst_scan_param->pul_channels_5G != OAL_PTR_NULL) {
        oal_free(pst_scan_param->pul_channels_5G);
        pst_scan_param->pul_channels_5G = OAL_PTR_NULL;
    }
    if (pst_scan_param->puc_ie != OAL_PTR_NULL) {
        oal_free(pst_scan_param->puc_ie);
        pst_scan_param->puc_ie = OAL_PTR_NULL;
    }
}


OAL_STATIC uint32_t wal_set_scan_channel(oal_cfg80211_scan_request_stru *pst_request,
                                           mac_cfg80211_scan_param_stru *pst_scan_param)
{
    uint32_t ul_loop;
    uint32_t ul_num_chan_2G;
    uint32_t ul_num_chan_5G;

    if (oal_any_null_ptr2(pst_request, pst_scan_param)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_get_scan_channel_num::request[%p] null ptr or scan_param[%p] null ptr.}",
            (uintptr_t)pst_request, (uintptr_t)pst_scan_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_request->n_channels == 0) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_get_scan_channel_num::channels in scan requst is zero.}");
        return OAL_FAIL;
    }

    pst_scan_param->pul_channels_2G = oal_memalloc(pst_request->n_channels * OAL_SIZEOF(uint32_t));
    if (oal_unlikely(pst_scan_param->pul_channels_2G == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_scan_work_func::2.4G channel alloc mem return null ptr!}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_scan_param->pul_channels_5G = oal_memalloc(pst_request->n_channels * OAL_SIZEOF(uint32_t));
    if (oal_unlikely(pst_scan_param->pul_channels_5G == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_scan_work_func::5G channel alloc mem return null ptr!}");
        free_scan_param_resource(pst_scan_param);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    ul_num_chan_2G = 0;
    ul_num_chan_5G = 0;

    for (ul_loop = 0; ul_loop < pst_request->n_channels; ul_loop++) {
        uint16_t us_center_freq;
        uint32_t ul_chan;

        us_center_freq = pst_request->channels[ul_loop]->center_freq;

        /* ��������Ƶ�ʣ������ŵ��� */
        ul_chan = (uint32_t)oal_ieee80211_frequency_to_channel((int32_t)us_center_freq);
        if (us_center_freq <= WAL_MAX_FREQ_2G) {
            pst_scan_param->pul_channels_2G[ul_num_chan_2G++] = ul_chan;
        } else {
            pst_scan_param->pul_channels_5G[ul_num_chan_5G++] = ul_chan;
        }
    }

    pst_scan_param->uc_num_channels_2G = (uint8_t)ul_num_chan_2G;
    pst_scan_param->uc_num_channels_5G = (uint8_t)ul_num_chan_5G;

    if (ul_num_chan_2G == 0) {
        oal_free(pst_scan_param->pul_channels_2G);
        pst_scan_param->pul_channels_2G = OAL_PTR_NULL;
    }
    if (ul_num_chan_5G == 0) {
        oal_free(pst_scan_param->pul_channels_5G);
        pst_scan_param->pul_channels_5G = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}


OAL_STATIC void wal_set_scan_ssid(oal_cfg80211_scan_request_stru *pst_request,
                                      mac_cfg80211_scan_param_stru *pst_scan_param)
{
    int32_t l_loop;
    int32_t l_ssid_num;

    if (oal_any_null_ptr2(pst_request, pst_scan_param)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_set_scan_ssid::scan failed, null ptr, pst_request[%p], pst_scan_param[%p].}",
            (uintptr_t)pst_request, (uintptr_t)pst_scan_param);

        return;
    }

    pst_scan_param->en_scan_type = OAL_ACTIVE_SCAN; /* active scan */
    pst_scan_param->l_ssid_num = 0;

    /* ȡ�ں��·���ssid�ĸ��� */
    l_ssid_num = pst_request->n_ssids;
    if (l_ssid_num > WLAN_SCAN_REQ_MAX_SSID) {
        /* ����û��·���ָ��ssid�ĸ�����������֧�ֵ�����������ȡ����֧�ֵ�ָ��ssid�������� */
        l_ssid_num = WLAN_SCAN_REQ_MAX_SSID;
    }

    /* ���û��·���ssid��Ϣ��������Ӧ�Ľṹ���� */
    if ((l_ssid_num > 0) && (pst_request->ssids != OAL_PTR_NULL)) {
        pst_scan_param->l_ssid_num = l_ssid_num;

        for (l_loop = 0; l_loop < l_ssid_num; l_loop++) {
            pst_scan_param->st_ssids[l_loop].uc_ssid_len = pst_request->ssids[l_loop].ssid_len;

            if (pst_scan_param->st_ssids[l_loop].uc_ssid_len > OAL_IEEE80211_MAX_SSID_LEN) {
                oam_warning_log2(0, OAM_SF_ANY,
                                 "{wal_set_scan_ssid::ssid scan set failed, ssid_len[%d] is exceed, max[%d].}",
                                 pst_scan_param->st_ssids[l_loop].uc_ssid_len,
                                 OAL_IEEE80211_MAX_SSID_LEN);

                pst_scan_param->st_ssids[l_loop].uc_ssid_len = OAL_IEEE80211_MAX_SSID_LEN;
            } else if (pst_scan_param->st_ssids[l_loop].uc_ssid_len == 0) {
                continue;
            }

            if (EOK != memcpy_s(pst_scan_param->st_ssids[l_loop].auc_ssid,
                                OAL_SIZEOF(pst_scan_param->st_ssids[l_loop].auc_ssid),
                                pst_request->ssids[l_loop].ssid,
                                pst_scan_param->st_ssids[l_loop].uc_ssid_len)) {
                oam_warning_log3(0, OAM_SF_ANY,
                    "wal_set_scan_ssid::memcpy do nothing. cur_idx[%d], cur_ssid_len[%d]. scan_req_ssid_num[%d]",
                    l_loop, pst_scan_param->st_ssids[l_loop].uc_ssid_len, pst_request->n_ssids);
            }
        }
    }
}


OAL_STATIC uint32_t wal_wait_for_scan_timeout_fn(void *p_arg)
{
    hmac_vap_stru *pst_hmac_vap = (hmac_vap_stru *)p_arg;
    mac_vap_stru *pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_bss_mgmt_stru *pst_bss_mgmt = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;
    oal_wiphy_stru *pst_wiphy = OAL_PTR_NULL;

    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                     "{wal_wait_for_scan_timeout_fn:: 5 seconds scan timeout proc.}");

    /* ���ݵ�ǰɨ������ͺ͵�ǰvap��״̬�������л�vap��״̬��ɨ���쳣�����У��ϱ��ں�ɨ��״̬Ϊɨ����� */
    if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_WAIT_SCAN)) {
        /* �ı�vap״̬��SCAN_COMP */
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
    }

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_SCAN, "{wal_wait_for_scan_complete_time_out::pst_hmac_device[%d] is null.}",
                         pst_mac_vap->uc_device_id);
        return OAL_FAIL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    pst_wiphy = pst_hmac_device->pst_device_base_info->pst_wiphy;

    /* ��ȡɨ�����Ĺ���ṹ��ַ */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* �����ں��·���ɨ��request��Դ���� */
    oal_spin_lock(&(pst_scan_mgmt->st_scan_request_spinlock));

    if (pst_scan_mgmt->pst_request != OAL_PTR_NULL) {
        /* �ϱ�ɨ�赽�����е�bss */
        wal_inform_all_bss(pst_wiphy, pst_bss_mgmt, pst_mac_vap->uc_vap_id);

        /* ֪ͨ kernel scan �Ѿ����� */
        oal_cfg80211_scan_done(pst_scan_mgmt->pst_request, 0);

        pst_scan_mgmt->pst_request = OAL_PTR_NULL;
        pst_scan_mgmt->en_complete = OAL_TRUE;

        /* �ñ������Ż�ʱ��֤OAL_WAIT_QUEUE_WAKE_UP�����ִ�� */
        oal_smp_mb();
        oal_wait_queue_wake_up_interrupt(&pst_scan_mgmt->st_wait_queue);
    }

    /* ֪ͨ���ںˣ��ͷ���Դ����� */
    oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));

    return OAL_SUCC;
}


OAL_STATIC void wal_start_timer_for_scan_timeout(uint8_t uc_vap_id)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint32_t ul_timeout;

    /* ��ʱʱ��Ϊ5�� */
    ul_timeout = WAL_MAX_SCAN_TIME_PER_SCAN_REQ;

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_hmac_vap is null!}");
        return;
    }

    /* ��ȡmac device */
    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_mac_device is null!}");
        return;
    }

    /* ����ɨ�豣����ʱ������ָ��ʱ��û���ϱ�ɨ�����������ϱ�ɨ����� */
    frw_timer_create_timer_m(&(pst_hmac_vap->st_scan_timeout),
                           wal_wait_for_scan_timeout_fn,
                           ul_timeout,
                           pst_hmac_vap,
                           OAL_FALSE,
                           OAM_MODULE_ID_WAL,
                           pst_mac_device->ul_core_id);

    return;
}


uint32_t wal_scan_work_func(hmac_scan_stru *pst_scan_mgmt,
                                  oal_net_device_stru *pst_netdev,
                                  oal_cfg80211_scan_request_stru *pst_request)
{
    mac_cfg80211_scan_param_stru st_scan_param;
    uint32_t ul_ret;
    mac_vap_stru *pst_mac_vap = oal_net_dev_priv(pst_netdev);
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t uc_vap_id;
    uint8_t *puc_ie = OAL_PTR_NULL;

    if (pst_mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_scan_work_func::pst_mac_vap is null!}");
        return OAL_FAIL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    memset_s(&st_scan_param, sizeof(mac_cfg80211_scan_param_stru), 0, sizeof(mac_cfg80211_scan_param_stru));

    /* �����ں��·���ɨ���ŵ��б� */
    ul_ret = wal_set_scan_channel(pst_request, &st_scan_param);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{wal_scan_work_func::wal_set_scan_channel proc failed, err_code[%d]!}", ul_ret);
        return OAL_FAIL;
    }

    /* �����ں��·���ssid */
    wal_set_scan_ssid(pst_request, &st_scan_param);

    /* �����ں��·���ie */
    st_scan_param.ul_ie_len = pst_request->ie_len;
    if (st_scan_param.ul_ie_len > 0) {
        puc_ie = (uint8_t *)oal_memalloc(pst_request->ie_len);
        if (puc_ie == OAL_PTR_NULL) {
            oam_error_log2(0, OAM_SF_CFG, "{wal_scan_work_func::puc_ie(%d)(%p) alloc mem return null ptr!}",
                           pst_request->ie_len, (uintptr_t)(pst_request->ie));
            free_scan_param_resource(&st_scan_param);
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
        if (EOK != memcpy_s(puc_ie, pst_request->ie_len, pst_request->ie, st_scan_param.ul_ie_len)) {
            oam_error_log0(0, OAM_SF_CFG, "wal_scan_work_func::memcpy fail!");
            oal_free(puc_ie);
            free_scan_param_resource(&st_scan_param);
            return OAL_FAIL;
        }
        st_scan_param.puc_ie = puc_ie;
    }

    /* �û����·���scan flag ת������һ�׶ν����������ݲ�����dmac,�� P �汾�ϲ�������ɺ��ٵ���ʵ��Ӧ�� */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    if (pst_request->flags & WLAN_SCAN_FLAG_LOW_POWER) {
        st_scan_param.ul_scan_flag |= WLAN_SCAN_FLAG_LOW_POWER; /* ʹ�ò���ɨ�� */
        oam_warning_log0(0, OAM_SF_SCAN, "{wal_scan_work_func::kernel set fast scan!}\r\n");
    }
#endif

    /* P2P WLAN/P2P ��������£�����ɨ���ssid �ж��Ƿ�Ϊp2p device �����ɨ�裬
        ssid Ϊ"DIRECT-"����Ϊ��p2p device �����ɨ�� */
    /* �����·�ɨ���device �Ƿ�Ϊp2p device(p2p0) */
    st_scan_param.bit_is_p2p0_scan = OAL_FALSE;

    if (IS_P2P_SCAN_REQ(pst_request)) {
        st_scan_param.bit_is_p2p0_scan = OAL_TRUE;
    }

    /* ���¼�ǰ��ֹ�첽�������ɨ���,����ͬ������ */
    pst_scan_mgmt->en_complete = OAL_FALSE;

    /* ������ʱ��������ɨ�賬ʱ���� */
    /* ����������post event�����п��ܴ������ȣ�����ɨ������¼����ڶ�ʱ�����������յ��³�ʱ��ʱ������ */
    wal_start_timer_for_scan_timeout(uc_vap_id);

    /* ���¼���֪ͨ��������ɨ�� */
    ul_ret = wal_cfg80211_start_scan(pst_netdev, &st_scan_param);
    if (ul_ret != OAL_SUCC) {
        pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap != OAL_PTR_NULL && pst_hmac_vap->st_scan_timeout.en_is_registerd) {
            frw_timer_immediate_destroy_timer_m(&pst_hmac_vap->st_scan_timeout);
        }

        oam_error_log1(0, OAM_SF_CFG,
                       "{wal_scan_work_func::wal_cfg80211_start_scan proc failed, err_code[%d]!}", ul_ret);
        /* ��������£���hmac�ͷ� */
        free_scan_param_resource(&st_scan_param);

        pst_scan_mgmt->en_complete = OAL_TRUE;
        return OAL_FAIL;
    }

    /* win32 UTģʽ������һ���¼����� */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    frw_event_process_all_event(0);
#endif

    return OAL_SUCC;
}


int32_t wal_send_scan_abort_msg(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_pedding_data = 0; /* ������ݣ���ʹ�ã�ֻ��Ϊ�˸��ýӿ� */
    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* ���¼�֪ͨdevice����ֹɨ�� */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SCAN_ABORT, OAL_SIZEOF(ul_pedding_data));

    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                        (int8_t *)&ul_pedding_data, OAL_SIZEOF(ul_pedding_data))) {
        oam_error_log0(0, OAM_SF_SCAN, "wal_send_scan_abort_msg::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_pedding_data),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_SCAN, "{wal_send_scan_abort_msg::fail to stop scan, error[%d]}", l_ret);
        return l_ret;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_SCAN, "{wal_send_scan_abort_msg::wal_check_and_release_msg_resp fail.}");
    }

    return OAL_SUCC;
}


int32_t wal_force_scan_complete(oal_net_device_stru *pst_net_dev,
                                      oal_bool_enum en_is_aborted)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_SCAN,
                         "{wal_force_scan_complete:: mac_vap of net_dev is deleted!iftype:[%d]}",
                         pst_net_dev->ieee80211_ptr->iftype);
        return OAL_SUCC;
    }

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return -OAL_EINVAL;
    }

    /* ��ȡhmac vap */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_complete::hmac_vap is null, vap_id[%d]!}", pst_mac_vap->uc_vap_id);
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    /* ����������ڲ���ɨ�� */
    if (pst_scan_mgmt->pst_request == OAL_PTR_NULL) {
        /* �ж��Ƿ�����ڲ�ɨ�裬������ڣ�Ҳ��Ҫֹͣ */
        if ((pst_scan_mgmt->en_is_scanning == OAL_TRUE) &&
            (pst_mac_vap->uc_vap_id == pst_scan_mgmt->st_scan_record_mgmt.uc_vap_id)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "{wal_force_scan_complete::may be internal scan, stop scan!}");
            /* ��ֹɨ�� */
            wal_send_scan_abort_msg(pst_net_dev);
        }

        return OAL_SUCC;
    }

    /* �����ں��·���ɨ��request��Դ���� */
    oal_spin_lock(&(pst_scan_mgmt->st_scan_request_spinlock));

    /* ������ϲ��·���ɨ��������֪ͨ�ں�ɨ��������ڲ�ɨ�費��֪ͨ */
    if ((pst_scan_mgmt->pst_request != OAL_PTR_NULL) && oal_wdev_match(pst_net_dev, pst_scan_mgmt->pst_request)) {
        /* ɾ���ȴ�ɨ�賬ʱ��ʱ�� */
        if (pst_hmac_vap->st_scan_timeout.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&(pst_hmac_vap->st_scan_timeout));
        }

        /* �ϱ��ں�ɨ���� */
        wal_inform_all_bss(pst_hmac_device->pst_device_base_info->pst_wiphy,
                               &(pst_scan_mgmt->st_scan_record_mgmt.st_bss_mgmt),
                               pst_mac_vap->uc_vap_id);

        /* ֪ͨ�ں�ɨ����ֹ */
        oal_cfg80211_scan_done(pst_scan_mgmt->pst_request, en_is_aborted);

        pst_scan_mgmt->pst_request = OAL_PTR_NULL;
        pst_scan_mgmt->en_complete = OAL_TRUE;
        /* ֪ͨ���ںˣ��ͷ���Դ����� */
        oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));
        /* �·�device��ֹɨ�� */
        wal_send_scan_abort_msg(pst_net_dev);

        /* ���ɨ�����ϱ��Ļص������������ϱ� */
        if (pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_vap_id == pst_mac_vap->uc_vap_id) {
            pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.p_fn_cb = OAL_PTR_NULL;
        }
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_complete::vap_id[%d] notify kernel scan abort!}",
                         pst_mac_vap->uc_vap_id);

        /* �ñ������Ż�ʱ��֤OAL_WAIT_QUEUE_WAKE_UP�����ִ�� */
        oal_smp_mb();
        oal_wait_queue_wake_up_interrupt(&pst_scan_mgmt->st_wait_queue);
    } else {
        /* ֪ͨ���ںˣ��ͷ���Դ����� */
        oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));
    }

    return OAL_SUCC;
}


int32_t wal_force_scan_abort_then_scan_comp(oal_net_device_stru *pst_net_dev)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_SCAN,
                         "{wal_force_scan_abort_then_scan_comp:: mac_vap of net_dev is deleted!iftype:[%d]}",
                         pst_net_dev->ieee80211_ptr->iftype);
        return OAL_SUCC;
    }

    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_abort_then_scan_comp::pst_hmac_device[%d] is null!}",
                         pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    /* ��ȡhmac vap */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_abort_then_scan_comp::hmac_vap is null, vap_id[%d]!}",
                         pst_mac_vap->uc_vap_id);
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    /* ����������ڲ���ɨ�� */
    if (pst_scan_mgmt->pst_request == OAL_PTR_NULL) {
        /* �ж��Ƿ�����ڲ�ɨ�裬������ڣ�Ҳ��Ҫֹͣ */
        if ((pst_scan_mgmt->en_is_scanning == OAL_TRUE) &&
            (pst_mac_vap->uc_vap_id == pst_scan_mgmt->st_scan_record_mgmt.uc_vap_id)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "{wal_force_scan_abort_then_scan_comp::may be internal scan, stop scan!}");
            /* ��ֹɨ�� */
            wal_send_scan_abort_msg(pst_net_dev);
        }

        return OAL_SUCC;
    }

    
    if ((pst_scan_mgmt->pst_request != OAL_PTR_NULL) && oal_wdev_match(pst_net_dev, pst_scan_mgmt->pst_request)) {
        /* ɾ���ȴ�ɨ�賬ʱ��ʱ�� */
        if (pst_hmac_vap->st_scan_timeout.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&(pst_hmac_vap->st_scan_timeout));
        }

        /* �·�device��ֹɨ�� */
        wal_send_scan_abort_msg(pst_net_dev);

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_abort_then_scan_comp::vap_id[%d] notify kernel scan abort!}",
                         pst_mac_vap->uc_vap_id);
    }

    return OAL_SUCC;
}


int32_t wal_stop_sched_scan(oal_net_device_stru *pst_netdev)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    uint32_t ul_pedding_data = 0; /* ������ݣ���ʹ�ã�ֻ��Ϊ�˸��ýӿ� */
    int32_t l_ret = 0;

    /* �����Ϸ��Լ�� */
    if (pst_netdev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{wal_stop_sched_scan::pst_netdev is null}");
        return -OAL_EINVAL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_device_stru �ṹ */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_SCAN, "{wal_stop_sched_scan:: pst_mac_vap is null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_device = (hmac_device_stru *)hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_SCAN, "{wal_stop_sched_scan:: pst_mac_device[%d] is null!}",
                       pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    oam_warning_log2(0, OAM_SF_SCAN,
                     "{wal_stop_sched_scan::sched scan req[0x%x],sched scan complete[%d]}",
                     (uintptr_t)(pst_scan_mgmt->pst_sched_scan_req),
                     pst_scan_mgmt->en_sched_scan_complete);

    if ((pst_scan_mgmt->pst_sched_scan_req != OAL_PTR_NULL) &&
        (pst_scan_mgmt->en_sched_scan_complete != OAL_TRUE)) {
        /* �������ɨ������δִ�У����ϱ�����ɨ���� */
        // if (pst_scan_mgmt->pst_request == OAL_PTR_NULL)
        {
            oal_cfg80211_sched_scan_result(pst_hmac_device->pst_device_base_info->pst_wiphy);
        }

        pst_scan_mgmt->pst_sched_scan_req = OAL_PTR_NULL;
        pst_scan_mgmt->en_sched_scan_complete = OAL_TRUE;

        /* ���¼�֪ͨdevice��ֹͣPNO����ɨ�� */
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_STOP_SCHED_SCAN, OAL_SIZEOF(ul_pedding_data));

        if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                            (int8_t *)&ul_pedding_data, OAL_SIZEOF(ul_pedding_data))) {
            oam_error_log0(0, OAM_SF_SCAN, "wal_stop_sched_scan::memcpy fail!");
            return OAL_FAIL;
        }

        l_ret = wal_send_cfg_event(pst_netdev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_pedding_data),
                                       (uint8_t *)&st_write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_SCAN,
                             "{wal_stop_sched_scan::fail to stop pno sched scan, error[%d]}", l_ret);
        }
    }

    return OAL_SUCC;
}

