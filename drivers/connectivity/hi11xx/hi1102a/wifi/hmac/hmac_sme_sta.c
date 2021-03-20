
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "mac_regdomain.h"
#include "hmac_main.h"
#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_mgmt_sta.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_p2p.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#include "oal_main.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SME_STA_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*****************************************************************************
    g_handle_rsp_func_sta: ������sme����Ϣ
*****************************************************************************/
OAL_STATIC hmac_sme_handle_rsp_func g_handle_rsp_func_sta[HMAC_SME_RSP_BUTT] = {
    hmac_handle_scan_rsp_sta,
    hmac_handle_join_rsp_sta,
    hmac_handle_auth_rsp_sta,
    hmac_handle_asoc_rsp_sta,
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
oal_void  hmac_send_rsp_to_sme_sta(hmac_vap_stru *pst_hmac_vap, hmac_sme_rsp_enum_uint8 en_type,
                                   oal_uint8 *puc_msg);


OAL_STATIC oal_void  hmac_prepare_scan_req(mac_scan_req_stru *pst_scan_params, oal_int8 *pc_desired_ssid)
{
#ifndef  _PRE_WLAN_FPGA_DIGITAL
    oal_uint8       uc_chan_idx;
    oal_uint32      ul_ret;
    oal_uint8       uc_2g_chan_num = 0;
    oal_uint8       uc_5g_chan_num = 0;
    oal_uint8       uc_chan_number;
#endif

    memset_s(pst_scan_params, OAL_SIZEOF(mac_scan_req_stru), 0, OAL_SIZEOF(mac_scan_req_stru));

    /* ���ó�ʼɨ������Ĳ��� */
    pst_scan_params->en_bss_type         = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type        = WLAN_SCAN_TYPE_ACTIVE;
    pst_scan_params->us_scan_time        = WLAN_DEFAULT_ACTIVE_SCAN_TIME;
    pst_scan_params->uc_probe_delay      = 0;
    pst_scan_params->uc_scan_func        = MAC_SCAN_FUNC_BSS;               /* Ĭ��ɨ��bss */
    pst_scan_params->p_fn_cb             = hmac_cfg80211_scan_comp_cb;      /* ʹ��cfg80211�ӿڻص�����Ϊut/dmt��������ӿ���� */
    pst_scan_params->uc_max_send_probe_req_count_per_channel = WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL;

    /* ���ó�ʼɨ�������ssid */
    /* ��ʼɨ������ָֻ��1��ssid, Ҫô��ͨ��, Ҫô��mibֵ */
    if (pc_desired_ssid[0] == '\0') {
        pst_scan_params->ast_mac_ssid_set[0].auc_ssid[0] = '\0';   /* ͨ��ssid */
    } else {
        if (memcpy_s(pst_scan_params->ast_mac_ssid_set[0].auc_ssid, WLAN_SSID_MAX_LEN,
                     pc_desired_ssid, OAL_STRLEN(pc_desired_ssid) + 1) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_SCAN, "hmac_prepare_scan_req::memcpy fail!");
            return;
        }
    }

    pst_scan_params->uc_ssid_num = 1;

    /* ���ó�ʼɨ�������bssid */
    /* ��ʼɨ������ָֻ��1��bssid��Ϊ�㲥��ַ */
    oal_set_mac_addr(pst_scan_params->auc_bssid[0], BROADCAST_MACADDR);
    pst_scan_params->uc_bssid_num = 1;

#ifndef  _PRE_WLAN_FPGA_DIGITAL

    /* 2G��ʼɨ���ŵ�, ȫ�ŵ�ɨ�� */
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_chan_idx++) {
            /* �ж��ŵ��ǲ����ڹ������� */
            ul_ret = mac_is_channel_idx_valid(WLAN_BAND_2G, uc_chan_idx);
            if (ul_ret == OAL_SUCC) {
                mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_chan_idx, &uc_chan_number);

                pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_chan_number = uc_chan_number;
                pst_scan_params->ast_channel_list[uc_2g_chan_num].en_band        = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_idx         = uc_chan_idx;
                pst_scan_params->uc_channel_nums++;
                uc_2g_chan_num++;
            }
        }
    OAM_INFO_LOG1(0, OAM_SF_SCAN, "{hmac_prepare_scan_req::after regdomain filter, the 2g total channel num is %d",
                  uc_2g_chan_num);

    /* 5G��ʼɨ���ŵ�, ȫ�ŵ�ɨ�� */
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_chan_idx++) {
            /* �ж��ŵ��ǲ����ڹ������� */
            ul_ret = mac_is_channel_idx_valid(WLAN_BAND_5G, uc_chan_idx);
            if (ul_ret == OAL_SUCC) {
                mac_get_channel_num_from_idx(WLAN_BAND_5G, uc_chan_idx, &uc_chan_number);

                pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].uc_chan_number = uc_chan_number;
                pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].en_band        = WLAN_BAND_5G;
                pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].uc_idx         = uc_chan_idx;
                pst_scan_params->uc_channel_nums++;
                uc_5g_chan_num++;
            }
        }
    OAM_INFO_LOG1(0, OAM_SF_SCAN, "{hmac_prepare_scan_req::after regdomain filter, the 5g total channel num is %d",
                  uc_5g_chan_num);
#endif
}
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))

oal_int32   hmac_cfg80211_dump_survey(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
                                      oal_int32 l_idx, oal_survey_info_stru *pst_info)
{
    mac_vap_stru             *pst_mac_vap;
    hmac_device_stru         *pst_hmac_dev;
    mac_scan_chan_stats_stru *pst_rcd;
    oal_int32 l_freq;

    if ((pst_wiphy == OAL_PTR_NULL) || (pst_netdev == OAL_PTR_NULL) || (pst_info == OAL_PTR_NULL) || (l_idx < 0)) {
        OAM_ERROR_LOG4(0, OAM_SF_ANY,
                       "{hmac_cfg80211_dump_survey::pst_wiphy = %x, pst_netdev = %x, pst_info = %x l_idx=%d!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)pst_info, l_idx);
        return -OAL_EINVAL;
    }

    memset_s(pst_info, OAL_SIZEOF(oal_survey_info_stru), 0, OAL_SIZEOF(oal_survey_info_stru));

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_netdev);
    if (!pst_mac_vap) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_cfg80211_dump_survey::pst_mac_vap = NULL}");
        return -OAL_EINVAL;
    }

    pst_hmac_dev = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (!pst_hmac_dev) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_cfg80211_dump_survey::pst_hmac_dev = NULL}");
        return -OAL_EINVAL;
    }

    if (pst_hmac_dev->st_scan_mgmt.en_is_scanning) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_cfg80211_dump_survey::drop request while scan running}");
        return -OAL_EINVAL;
    }

    pst_rcd = pst_hmac_dev->st_scan_mgmt.st_scan_record_mgmt.ast_chan_results;

    if (l_idx >= pst_hmac_dev->st_scan_mgmt.st_scan_record_mgmt.uc_chan_numbers) {
        return -ENOENT;
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0))
    pst_info->time          = pst_rcd[l_idx].ul_total_stats_time_us / 1000; /* 1ms����1000us */
    pst_info->time_busy     = (pst_rcd[l_idx].ul_total_stats_time_us -
                               pst_rcd[l_idx].ul_total_free_time_20M_us) / 1000; /* 1ms����1000us */
    pst_info->time_ext_busy = 0;
    pst_info->time_rx = pst_rcd[l_idx].ul_total_recv_time_us / 1000; /* 1ms����1000us */
    pst_info->time_tx = pst_rcd[l_idx].ul_total_send_time_us / 1000; /* 1ms����1000us */

    pst_info->filled = SURVEY_INFO_TIME | SURVEY_INFO_TIME_BUSY | SURVEY_INFO_TIME_RX | SURVEY_INFO_TIME_TX;
#else
    pst_info->channel_time          = pst_rcd[l_idx].ul_total_stats_time_us / 1000; /* 1ms����1000us */
    pst_info->channel_time_busy     = (pst_rcd[l_idx].ul_total_stats_time_us -
                                       pst_rcd[l_idx].ul_total_free_time_20M_us) / 1000; /* 1ms����1000us */
    pst_info->channel_time_ext_busy = 0;
    pst_info->channel_time_rx = pst_rcd[l_idx].ul_total_recv_time_us / 1000; /* 1ms����1000us */
    pst_info->channel_time_tx = pst_rcd[l_idx].ul_total_send_time_us / 1000; /* 1ms����1000us */

    pst_info->filled = SURVEY_INFO_CHANNEL_TIME
                       | SURVEY_INFO_CHANNEL_TIME_BUSY
                       | SURVEY_INFO_CHANNEL_TIME_RX
                       | SURVEY_INFO_CHANNEL_TIME_TX;
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)) */

    if (pst_rcd[l_idx].uc_free_power_cnt && (pst_rcd[l_idx].s_free_power_stats_20M < 0)) {
        pst_info->noise = pst_rcd[l_idx].s_free_power_stats_20M / pst_rcd[l_idx].uc_free_power_cnt;
        pst_info->filled |= SURVEY_INFO_NOISE_DBM;
    }
    l_freq = mac_get_band_by_channel_num(pst_rcd[l_idx].uc_channel_number);
    pst_info->channel = oal_ieee80211_get_channel(pst_wiphy,
                                                  oal_ieee80211_channel_to_frequency(pst_rcd[l_idx].uc_channel_number,
                                                                                     l_freq));
    return 0;
}
#endif


oal_void  hmac_cfg80211_scan_comp_cb(void  *p_scan_record)
{
    hmac_scan_record_stru           *pst_scan_record = (hmac_scan_record_stru *)p_scan_record;
    hmac_vap_stru                   *pst_hmac_vap = OAL_PTR_NULL;
    hmac_scan_rsp_stru               st_scan_rsp;

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_scan_record->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_cfg80211_scan_comp_cb::pst_hmac_vap is null.");
        return;
    }

    /* �ϱ�ɨ�����ṹ���ʼ�� */
    memset_s(&st_scan_rsp, OAL_SIZEOF(st_scan_rsp), 0, OAL_SIZEOF(st_scan_rsp));

    st_scan_rsp.uc_result_code = pst_scan_record->en_scan_rsp_status;

    /* ɨ��������sme */
    hmac_send_rsp_to_sme_sta(pst_hmac_vap, HMAC_SME_SCAN_RSP, (oal_uint8 *)&st_scan_rsp);

    return;
}
#ifdef _PRE_WLAN_FEATURE_FTM

OAL_STATIC oal_uint8 hmac_ftm_is_in_scan_list(mac_vap_stru *pst_mac_vap, oal_uint8 uc_band,
                                              oal_uint8 uc_channel_number)
{
    if (!mac_check_ftm_enable(pst_mac_vap)) {
        return OAL_TRUE;
    }

    /* For location Use,no channel switch when vap is up */
    if ((pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) &&
        (pst_mac_vap->st_channel.en_band == uc_band) &&
        (pst_mac_vap->st_channel.uc_chan_number != uc_channel_number)) {
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}
#endif

oal_void hmac_cfg80211_prepare_scan_para(mac_scan_req_stru *pst_scan_params,
                                         mac_cfg80211_scan_param_stru *pst_cfg80211_scan_param)
{
    pst_scan_params->en_bss_type = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type = pst_cfg80211_scan_param->en_scan_type;

    /* ����ɨ��ʱ�� */
    if (pst_scan_params->en_scan_type == WLAN_SCAN_TYPE_ACTIVE) {
        pst_scan_params->us_scan_time = WLAN_DEFAULT_ACTIVE_SCAN_TIME;
    } else {
        pst_scan_params->us_scan_time = WLAN_DEFAULT_PASSIVE_SCAN_TIME;
    }

    pst_scan_params->uc_probe_delay = 0;
    pst_scan_params->uc_scan_func = MAC_SCAN_FUNC_BSS; /* Ĭ��ɨ��bss */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
    pst_scan_params->uc_scan_func |= MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS;
#endif
    pst_scan_params->p_fn_cb = hmac_cfg80211_scan_comp_cb; /* ɨ����ɻص����� */
    pst_scan_params->uc_max_scan_count_per_channel = 2;  /* ÿ���ŵ������ɨ�����Ϊ2 */
}
OAL_STATIC oal_void hmac_2g_scan_channel_init(mac_cfg80211_scan_param_stru *pst_cfg80211_scan_param,
    mac_vap_stru *pst_mac_vap, mac_scan_req_stru *pst_scan_params, oal_uint8 *puc_chan_num_2g)
{
    oal_uint8 uc_loop; /* forѭ�������±� */
    oal_uint8 uc_channel_number; /* �ŵ��� */
    oal_uint8 uc_channel_idx = 0; /* �ŵ������� */
    oal_uint32 ul_ret;

    for (uc_loop = 0; uc_loop < pst_cfg80211_scan_param->uc_num_channels_2G; uc_loop++) {
        uc_channel_number = (oal_uint8)pst_cfg80211_scan_param->pul_channels_2G[uc_loop];

        /* �ж��ŵ��ǲ����ڹ������� */
        ul_ret = mac_is_channel_num_valid(WLAN_BAND_2G, uc_channel_number);
        if (ul_ret == OAL_SUCC) {
            ul_ret = mac_get_channel_idx_from_num(WLAN_BAND_2G, uc_channel_number, &uc_channel_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_SCAN, "mac_get_channel_idx_from_num fail.chn=%d", uc_channel_number);
            }
#ifdef _PRE_WLAN_FEATURE_FTM
            if (!hmac_ftm_is_in_scan_list(pst_mac_vap, WLAN_BAND_2G, uc_channel_number)) {
                continue;
            }
#endif
            pst_scan_params->ast_channel_list[*puc_chan_num_2g].en_band        = WLAN_BAND_2G;
            pst_scan_params->ast_channel_list[*puc_chan_num_2g].uc_chan_number = uc_channel_number;
            pst_scan_params->ast_channel_list[*puc_chan_num_2g].uc_idx         = uc_channel_idx;

            pst_scan_params->uc_channel_nums++;
            (*puc_chan_num_2g)++;
        }
    }
}

OAL_STATIC oal_void hmac_5g_scan_channel_init(mac_cfg80211_scan_param_stru *pst_cfg80211_scan_param,
    mac_vap_stru *pst_mac_vap, mac_scan_req_stru *pst_scan_params, oal_uint8 uc_chan_num_2g)
{
    oal_uint8 uc_loop; /* forѭ�������±� */
    oal_uint8 uc_channel_number; /* �ŵ��� */
    oal_uint8 uc_channel_idx = 0; /* �ŵ������� */
    oal_uint32 ul_ret;
    oal_uint8 uc_chan_num_5g = 0;

    for (uc_loop = 0; uc_loop < pst_cfg80211_scan_param->uc_num_channels_5G; uc_loop++) {
        uc_channel_number = (oal_uint8)pst_cfg80211_scan_param->pul_channels_5G[uc_loop];

        /* �ж��ŵ��ǲ����ڹ������� */
        ul_ret = mac_is_channel_num_valid(WLAN_BAND_5G, uc_channel_number);
        if ((ul_ret == OAL_SUCC) && !hitalk_status) {
            ul_ret = mac_get_channel_idx_from_num(WLAN_BAND_5G, uc_channel_number, &uc_channel_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{WLAN_BAND_5G:hmac_cfg80211_prepare_scan_req_sta\
                                 ::mac_get_channel_idx_from_num fail. channel_number: %u}", uc_channel_number);
            }
#ifdef _PRE_WLAN_FEATURE_FTM
            if (!hmac_ftm_is_in_scan_list(pst_mac_vap, WLAN_BAND_5G, uc_channel_number)) {
                continue;
            }
#endif

            pst_scan_params->ast_channel_list[uc_chan_num_2g + uc_chan_num_5g].en_band        = WLAN_BAND_5G;
            pst_scan_params->ast_channel_list[uc_chan_num_2g + uc_chan_num_5g].uc_chan_number = uc_channel_number;
            pst_scan_params->ast_channel_list[uc_chan_num_2g + uc_chan_num_5g].uc_idx         = uc_channel_idx;

            pst_scan_params->uc_channel_nums++;
            uc_chan_num_5g++;
        }
    }
}

OAL_STATIC oal_void  hmac_cfg80211_prepare_scan_req_sta(mac_vap_stru *pst_mac_vap,
                                                        mac_scan_req_stru *pst_scan_params, oal_int8 *puc_param)
{
    oal_uint8                       uc_loop;            /* forѭ�������±� */
    mac_cfg80211_scan_param_stru   *pst_cfg80211_scan_param;
    mac_device_stru                *pst_mac_device = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_P2P
    mac_vap_stru                   *pst_mac_vap_temp;
#endif
    oal_uint32                      ul_ret;
    oal_bool_enum_uint8             en_scan_when_go_up = OAL_FALSE;
    oal_uint8                       uc_chan_num_2g = 0;

    pst_cfg80211_scan_param = (mac_cfg80211_scan_param_stru *)puc_param;

    /* �ⲿ�Ѿ�zero, ����ɨ����� */
    hmac_cfg80211_prepare_scan_para(pst_scan_params, pst_cfg80211_scan_param);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        return;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    /* p2p Go����ɨ��ʱ����ʹ��p2p device�豸���� */
    if (pst_mac_vap->en_p2p_mode == WLAN_P2P_DEV_MODE) {
        ul_ret = mac_device_find_up_p2p_go(pst_mac_device, &pst_mac_vap_temp);
        if (ul_ret == OAL_SUCC) {
            en_scan_when_go_up = OAL_TRUE;
        }
    }
#endif  /* _PRE_WLAN_FEATURE_P2P */

    if ((pst_cfg80211_scan_param->l_ssid_num <= 1) && ((en_scan_when_go_up == OAL_TRUE) ||
        (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) || (pst_mac_vap->en_vap_state == MAC_VAP_STATE_PAUSE) ||
         ((pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) && (pst_mac_vap->us_user_nums > 0)))) {
        pst_scan_params->uc_max_scan_count_per_channel = 1;
    }

    for (uc_loop = 0; uc_loop < pst_cfg80211_scan_param->l_ssid_num; uc_loop++) {
        if (memcpy_s(pst_scan_params->ast_mac_ssid_set[uc_loop].auc_ssid, WLAN_SSID_MAX_LEN,
            pst_cfg80211_scan_param->st_ssids[uc_loop].auc_ssid,
            pst_cfg80211_scan_param->st_ssids[uc_loop].uc_ssid_len) != EOK) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_cfg80211_prepare_scan_req_sta::memcpy fail.}");
            return;
        }
        pst_scan_params->ast_mac_ssid_set[uc_loop].auc_ssid[pst_cfg80211_scan_param->st_ssids[uc_loop].uc_ssid_len] =
            '\0';   /* ssidĩβ��'\0' */
    }

    /* ����ϲ��·���ָ��ssid����ÿ��ɨ�跢�͵�probe req֡�ĸ���Ϊ�·���ssid���� */
    pst_scan_params->uc_max_send_probe_req_count_per_channel = (oal_uint8)pst_cfg80211_scan_param->l_ssid_num;

    if (pst_scan_params->uc_max_send_probe_req_count_per_channel == 0) {
        pst_scan_params->ast_mac_ssid_set[0].auc_ssid[0] = '\0';   /* ͨ��ssid */
        pst_scan_params->uc_max_send_probe_req_count_per_channel = WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL;
    }
    /* ���ָ��SSID��������3��,��������ͳ�ʱʱ��Ϊ40ms,Ĭ�Ϸ��͵ȴ���ʱ20ms */
    if ((pst_scan_params->uc_max_send_probe_req_count_per_channel > 3) &&
        (pst_scan_params->en_scan_type == WLAN_SCAN_TYPE_ACTIVE)) {
        pst_scan_params->us_scan_time = WLAN_LONG_ACTIVE_SCAN_TIME;
    }

    pst_scan_params->uc_ssid_num = (oal_uint8)pst_cfg80211_scan_param->l_ssid_num;

    oal_set_mac_addr(pst_scan_params->auc_bssid[0], BROADCAST_MACADDR);
    pst_scan_params->uc_bssid_num = 1;  /* ��ʼɨ������ָ��1��bssid��Ϊ�㲥��ַ */

    /* 2G��ʼɨ���ŵ� */
    hmac_2g_scan_channel_init(pst_cfg80211_scan_param, pst_mac_vap, pst_scan_params, &uc_chan_num_2g);

    /* 5G��ʼɨ���ŵ� */
    hmac_5g_scan_channel_init(pst_cfg80211_scan_param, pst_mac_vap, pst_scan_params, uc_chan_num_2g);
#ifdef _PRE_WLAN_FEATURE_P2P
    /* WLAN/P2P ����ʱ���ж��Ƿ�p2p0 ����ɨ�� */
    pst_scan_params->bit_is_p2p0_scan = pst_cfg80211_scan_param->bit_is_p2p0_scan;
    if (pst_cfg80211_scan_param->bit_is_p2p0_scan) {
        pst_scan_params->en_bss_type = 0;
    }
#endif  /* _PRE_WLAN_FEATURE_P2P */
}


oal_void  hmac_prepare_join_req(hmac_join_req_stru *pst_join_req, mac_bss_dscr_stru *pst_bss_dscr)
{
    memset_s(pst_join_req, OAL_SIZEOF(hmac_join_req_stru), 0, OAL_SIZEOF(hmac_join_req_stru));

    if (memcpy_s(&(pst_join_req->st_bss_dscr), OAL_SIZEOF(mac_bss_dscr_stru),
                 pst_bss_dscr, OAL_SIZEOF(mac_bss_dscr_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_prepare_join_req::memcpy fail!");
        return;
    }

    pst_join_req->us_join_timeout = WLAN_JOIN_START_TIMEOUT;
    pst_join_req->us_probe_delay = WLAN_PROBE_DELAY_TIME;
}


OAL_STATIC oal_void hmac_prepare_auth_req(hmac_vap_stru *pst_hmac_vap, hmac_auth_req_stru *pst_auth_req)
{
    memset_s(pst_auth_req, OAL_SIZEOF(*pst_auth_req), 0, OAL_SIZEOF(*pst_auth_req));
#ifdef _PRE_WLAN_FEATURE_SAE
    if ((pst_hmac_vap->en_auth_mode == WLAN_WITP_AUTH_SAE) &&
        (pst_hmac_vap->bit_sae_connect_with_pmkid == OAL_FALSE)) {
        /* SAE auth timeout time increase from 150ms to 600ms for compatibility, such as 1151 AP */
        pst_auth_req->us_timeout = WLAN_AUTH_TIMEOUT * 2; /* 2��sta auth��timeoutʱ�䣬��600ms */
    } else
#endif /* _PRE_WLAN_FEATURE_SAE */
    {
        /* ����AUTH���Ĵ����󣬽���ʱʱ����� */
        pst_auth_req->us_timeout = WLAN_AUTH_TIMEOUT >> 1;
    }
}


OAL_STATIC oal_void  hmac_prepare_asoc_req(mac_vap_stru *pst_mac_vap, hmac_asoc_req_stru *pst_asoc_req)
{
    memset_s(pst_asoc_req, OAL_SIZEOF(hmac_asoc_req_stru), 0, OAL_SIZEOF(hmac_asoc_req_stru));

    /* ����ASOC���Ĵ����󣬽���ʱʱ����� */
    pst_asoc_req->us_assoc_timeout = (oal_uint16)
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11AssociationResponseTimeOut >> 1;
}


oal_uint32  hmac_sta_initiate_scan(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    oal_uint32              ul_ret;
    mac_scan_req_stru       st_scan_params;
    hmac_vap_stru          *pst_hmac_vap = OAL_PTR_NULL;
    oal_int8               *pc_desired_ssid = OAL_PTR_NULL;

#ifdef  _PRE_WLAN_FEATURE_P2P
    mac_device_stru        *pst_mac_device;
#endif

    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_sta_initial_scan: pst_mac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �������vap������ */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_CONFIG) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_sta_initiate_scan::config vap! can't get info.}");
        return OAL_FAIL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_sta_initiate_scan::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_desired_ssid = (oal_int8 *)pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11DesiredSSID;
    hmac_prepare_scan_req(&st_scan_params, pc_desired_ssid);
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_sta_initial_scan: pst_mac_device is null device id[%d].}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    st_scan_params.bit_is_p2p0_scan = puc_param[0];
    /* ��¼ɨ�������Ϣ��device�ṹ�� */
    if (memcpy_s(&(pst_mac_device->st_scan_params), OAL_SIZEOF(mac_scan_req_stru),
                 &st_scan_params, OAL_SIZEOF(mac_scan_req_stru)) != EOK) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_sta_initiate_scan::memcpy fail!");
        return OAL_FAIL;
    }
#endif  /* _PRE_WLAN_FEATURE_P2P */

    /* ״̬������: hmac_scan_proc_scan_req_event */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_SCAN_REQ, (oal_void *)(&st_scan_params));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_sta_initiate_scan::fsm_call_func_sta fail[%d].}",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_uint32  hmac_cfg80211_check_can_start_sched_scan(mac_vap_stru *pst_mac_vap)
{
    hmac_device_stru      *pst_hmac_device;
    mac_vap_stru          *pst_mac_vap_tmp = OAL_PTR_NULL;
    oal_uint32             ul_ret;

    /* ��ȡmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(pst_hmac_device == OAL_PTR_NULL)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_check_can_start_sched_scan::pst_hmac_device[%d] null.}",
                         pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ���vap��ģʽ����STA���򷵻أ���֧������ģʽ��vap�ĵ���ɨ�� */
    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_check_can_start_sched_scan::vap mode[%d] don't support sched scan.}",
                         pst_mac_vap->en_vap_mode);

        pst_hmac_device->st_scan_mgmt.pst_sched_scan_req = OAL_PTR_NULL;
        pst_hmac_device->st_scan_mgmt.en_sched_scan_complete = OAL_TRUE;
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /* ������ڵ�ǰdevice����up��vap������������ɨ�� */
    ul_ret = mac_device_find_up_vap(pst_hmac_device->pst_device_base_info, &pst_mac_vap_tmp);
    if (ul_ret == OAL_SUCC) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_check_can_start_sched_scan::exist up vap, don't start sched scan.}");

        pst_hmac_device->st_scan_mgmt.pst_sched_scan_req = OAL_PTR_NULL;
        pst_hmac_device->st_scan_mgmt.en_sched_scan_complete = OAL_TRUE;
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    return OAL_SUCC;
}



oal_uint32  hmac_cfg80211_start_sched_scan(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    hmac_vap_stru                       *pst_hmac_vap = OAL_PTR_NULL;
    mac_pno_scan_stru                    st_pno_scan_params;
    oal_uint32                           ul_ret;
    oal_bool_enum_uint8                  en_is_random_mac_addr_scan;
    hmac_device_stru                     *pst_hmac_device = OAL_PTR_NULL;
    mac_pno_scan_stru    *pst_cfg80211_pno_scan_params = (mac_pno_scan_stru *)(uintptr_t)(*(oal_uint *)puc_param);

    /* �����Ϸ��Լ�� */
    if (OAL_UNLIKELY(puc_param == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_cfg80211_start_sched_scan::puc_param is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_mac_vap == OAL_PTR_NULL) {
        OAL_MEM_FREE(pst_cfg80211_pno_scan_params, OAL_TRUE);
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "hmac_cfg80211_start_sched_scan:pst_mac_vap is null");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡhmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(pst_hmac_vap == OAL_PTR_NULL)) {
        OAL_MEM_FREE(pst_cfg80211_pno_scan_params, OAL_TRUE);
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_cfg80211_start_sched_scan::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ��ȡhmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAL_MEM_FREE(pst_cfg80211_pno_scan_params, OAL_TRUE);
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_start_sched_scan::hmac_device[%d] null.}",
                         pst_hmac_vap->st_vap_base_info.uc_device_id);
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    /* �����ϲ��·���pnoɨ����� */
    if (memcpy_s(&st_pno_scan_params, OAL_SIZEOF(mac_pno_scan_stru),
                 pst_cfg80211_pno_scan_params, OAL_SIZEOF(mac_pno_scan_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "hmac_cfg80211_start_sched_scan::memcpy fail!");
        OAL_MEM_FREE(pst_cfg80211_pno_scan_params, OAL_TRUE);
        return OAL_FAIL;
    }

    OAL_MEM_FREE(pst_cfg80211_pno_scan_params, OAL_TRUE);

    /* ��⵱ǰdevice�Ƿ������������ɨ�� */
    ul_ret = hmac_cfg80211_check_can_start_sched_scan(pst_mac_vap);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /* ����PNO����ɨ�����ʱ������н���ϱ������ϱ�ɨ�����Ļص����� */
    st_pno_scan_params.p_fn_cb = hmac_cfg80211_scan_comp_cb;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    en_is_random_mac_addr_scan = wlan_customize.uc_random_mac_addr_scan;
#else
    en_is_random_mac_addr_scan = pst_hmac_device->st_scan_mgmt.en_is_random_mac_addr_scan;
#endif

    /* ���÷��͵�probe req֡��Դmac addr */
    st_pno_scan_params.en_is_random_mac_addr_scan = en_is_random_mac_addr_scan;
    hmac_scan_set_sour_mac_addr_in_probe_req(pst_hmac_vap, st_pno_scan_params.auc_sour_mac_addr,
                                             en_is_random_mac_addr_scan, OAL_FALSE);

    /* ״̬������: hmac_scan_proc_sched_scan_req_event */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_SCHED_SCAN_REQ, (oal_void *)(&st_pno_scan_params));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_start_sched_scan::hmac_fsm_call_func_sta fail[%d].}", ul_ret);
        pst_hmac_device->st_scan_mgmt.pst_sched_scan_req     = OAL_PTR_NULL;
        pst_hmac_device->st_scan_mgmt.en_sched_scan_complete = OAL_TRUE;
    }

    return ul_ret;
}


oal_uint32  hmac_cfg80211_stop_sched_scan(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    oal_uint32      ul_ret;

    /***************************************************************************
                         ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap,
                                    WLAN_CFGID_STOP_SCHED_SCAN,
                                    us_len,
                                    puc_param);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_cfg80211_stop_sched_scan::hmac_config_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_free_scan_param_resource(mac_cfg80211_scan_param_stru *pst_scan_param)
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


oal_uint32  hmac_cfg80211_start_scan_sta(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    mac_scan_req_stru                    st_scan_params;
    hmac_vap_stru                       *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru                     *pst_mac_device = OAL_PTR_NULL;
    mac_cfg80211_scan_param_stru        *pst_mac_cfg80211_scan_param = OAL_PTR_NULL;
    oal_app_ie_stru                     *pst_app_ie = OAL_PTR_NULL;
    oal_uint32                           ul_ret = OAL_FAIL;
    if (OAL_UNLIKELY(puc_param == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{hmac_cfg80211_start_scan_sta::null ptr, %p.}", (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&st_scan_params, OAL_SIZEOF(mac_scan_req_stru), 0, OAL_SIZEOF(mac_scan_req_stru));

    pst_mac_cfg80211_scan_param = (mac_cfg80211_scan_param_stru *)puc_param;
    if (OAL_UNLIKELY(pst_mac_cfg80211_scan_param == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN,
                       "{hmac_cfg80211_start_scan_sta::scan failed,pst_mac_cfg80211_scan_param is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_cfg80211_start_scan_sta::scan failed, pst_mac_vap is NULL.}");
        /*lint -e801*/
        goto ERROR_STEP;
        /*lint +e801*/
    }

    /* ��ʼɨ���б���0 */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_start_scan_sta::scan failed, null ptr, pst_mac_device null.}");
        /*lint -e801*/
        goto ERROR_STEP;
        /*lint +e801*/
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_start_scan_sta::scan failed, null ptr, pst_hmac_vap null.}");
        /*lint -e801*/
        goto ERROR_STEP;
        /*lint +e801*/
    }

    /* ���ں��·���ɨ��������µ�����ɨ������ṹ���� */
    hmac_cfg80211_prepare_scan_req_sta(pst_mac_vap, &st_scan_params, (oal_int8 *)pst_mac_cfg80211_scan_param);

    /* ����P2P/WPS IE ��Ϣ�� vap �ṹ���� */
    if (IS_LEGACY_VAP(pst_mac_vap)) {
        hmac_config_del_p2p_ie((oal_uint8 *)(pst_mac_cfg80211_scan_param->puc_ie),
                               &(pst_mac_cfg80211_scan_param->ul_ie_len));
    }
#ifdef _PRE_WLAN_FEATURE_P2P
    /* P2P0 ɨ��ʱ��¼P2P listen channel */
    if (pst_mac_cfg80211_scan_param->bit_is_p2p0_scan == OAL_TRUE) {
        hmac_find_p2p_listen_channel(pst_mac_vap, (oal_uint16)(pst_mac_cfg80211_scan_param->ul_ie_len),
                                     (oal_uint8 *)(pst_mac_cfg80211_scan_param->puc_ie));
        st_scan_params.uc_p2p0_listen_channel = pst_mac_vap->uc_p2p_listen_channel;
    }
#endif
    pst_app_ie = (oal_app_ie_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(oal_app_ie_stru), OAL_TRUE);
    if (OAL_UNLIKELY(pst_app_ie == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_cfg80211_start_scan_sta::scan failed, oal_memalloc failed.}");
        /*lint -e801*/
        goto ERROR_STEP;
        /*lint +e801*/
    }

    pst_app_ie->ul_ie_len = pst_mac_cfg80211_scan_param->ul_ie_len;
    if ((pst_app_ie->ul_ie_len > 0) &&
        memcpy_s(pst_app_ie->auc_ie, OAL_SIZEOF(pst_app_ie->auc_ie),
                 pst_mac_cfg80211_scan_param->puc_ie, pst_app_ie->ul_ie_len) != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "hmac_cfg80211_start_scan_sta::memcpy fail! src_len[%d]", pst_app_ie->ul_ie_len);
    }
    pst_app_ie->en_app_ie_type = OAL_APP_PROBE_REQ_IE;
    ul_ret = hmac_config_set_app_ie_to_vap(pst_mac_vap, pst_app_ie, pst_app_ie->en_app_ie_type);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{hmac_cfg80211_start_scan_sta::set_app_ie_to_vap fail err_code=%d.}", ul_ret);
        OAL_MEM_FREE(pst_app_ie, OAL_TRUE);
        /*lint -e801*/
        goto ERROR_STEP;
        /*lint +e801*/
    }
    OAL_MEM_FREE(pst_app_ie, OAL_TRUE);

#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    /* ut do noting */
#else
    /* ��Ϊɨ�������·��޸�Ϊ��������ʽ����ˣ��ŵ�������ڴ���Ҫ�ٴ˴��ͷ� */
    hmac_free_scan_param_resource(pst_mac_cfg80211_scan_param);
#endif

    /* ״̬������: hmac_scan_proc_scan_req_event, hmac_scan_proc_scan_req_event_exception */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_SCAN_REQ, (oal_void *)(&st_scan_params));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_cfg80211_start_scan_sta::hmac_fsm_call_func_sta fail[%d].}", ul_ret);

        hmac_scan_proc_scan_req_event_exception(pst_hmac_vap, &ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;

ERROR_STEP:
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    /* ut do noting */
#else
    /* ��Ϊɨ�������·��޸�Ϊ��������ʽ����ˣ��ŵ�������ڴ���Ҫ�ٴ˴��ͷ� */
    hmac_free_scan_param_resource(pst_mac_cfg80211_scan_param);
#endif
    return OAL_ERR_CODE_PTR_NULL;
}


oal_uint32  hmac_sta_initiate_join(mac_vap_stru *pst_mac_vap, mac_bss_dscr_stru *pst_bss_dscr)
{
    hmac_join_req_stru  st_join_req;
    hmac_vap_stru      *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32          ul_ret;
    oal_uint8           uc_rate_num;

    if (OAL_UNLIKELY((pst_mac_vap == OAL_PTR_NULL) || (pst_bss_dscr == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_ASSOC, "{hmac_sta_initiate_join::input null %x %x.}", (uintptr_t)pst_mac_vap,
                       (uintptr_t)pst_bss_dscr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_sta_initiate_join::mac_res_get_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    uc_rate_num = (pst_bss_dscr->uc_num_supp_rates < WLAN_MAX_SUPP_RATES) ? pst_bss_dscr->uc_num_supp_rates :
                  WLAN_MAX_SUPP_RATES;
    if (memcpy_s(pst_hmac_vap->auc_supp_rates, WLAN_MAX_SUPP_RATES,
        pst_bss_dscr->auc_supp_rates, uc_rate_num) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "hmac_sta_initiate_join::memcpy fail!");
        return OAL_FAIL;
    }

    pst_hmac_vap->uc_rs_nrates = pst_bss_dscr->uc_num_supp_rates;

    hmac_prepare_join_req(&st_join_req, pst_bss_dscr);

    /* ״̬������ hmac_sta_wait_join */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_JOIN_REQ, (oal_void *)(&st_join_req));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_initiate_join::hmac_fsm_call_func_sta fail[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_uint32  hmac_sta_initiate_auth(mac_vap_stru *pst_mac_vap)
{
    hmac_auth_req_stru  st_auth_req;
    hmac_vap_stru      *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32          ul_ret;

    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_sta_initiate_auth: pst_mac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_sta_initiate_auth: pst_hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_prepare_auth_req(pst_hmac_vap, &st_auth_req);

    /* ״̬������ hmac_sta_wait_auth */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_AUTH_REQ, (oal_void *)(&st_auth_req));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_initiate_join::hmac_fsm_call_func_sta fail[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_uint32  hmac_sta_initiate_asoc(mac_vap_stru *pst_mac_vap)
{
    hmac_asoc_req_stru  st_asoc_req;
    hmac_vap_stru      *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32          ul_ret;

    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "{hmac_sta_initiate_asoc::pst_mac_vap null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "hmac_sta_initiate_asoc: pst_hmac_vap null!");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_prepare_asoc_req(pst_mac_vap, &st_asoc_req);

    /* ״̬������ */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_ASOC_REQ, (oal_void *)(&st_asoc_req));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "hmac_sta_initiate_asoc::hmac_fsm_call_func_sta fail[%d]", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


#if  (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)

OAL_STATIC oal_void  hmac_join_bss_sta(hmac_vap_stru *pst_hmac_vap)
{
    mac_bss_dscr_stru   *pst_bss_dscr = OAL_PTR_NULL;
    oal_uint32           ul_bss_idx;
    hmac_device_stru    *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "hmac_join_bss_sta::pst_mac_device null");
        return;
    }

    /* �Ƿ���������AP���� */
    if (pst_hmac_device->uc_desired_bss_num > 0) {
        ul_bss_idx   = pst_hmac_device->auc_desired_bss_idx[pst_hmac_device->uc_desired_bss_num - 1];  /* �Ӻ���ǰȡ������bss */
        pst_bss_dscr =
            hmac_scan_find_scanned_bss_dscr_by_index(pst_hmac_vap->st_vap_base_info.uc_device_id, ul_bss_idx);

        pst_hmac_device->uc_desired_bss_num--;      /* ���������AP����-1 */

        OAM_INFO_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_join_bss_sta::try to join bss[%d].}", ul_bss_idx);

        /* �������� */
        hmac_sta_initiate_join(&pst_hmac_vap->st_vap_base_info, pst_bss_dscr);
    }
}


oal_void  hmac_prepare_bss_list(hmac_vap_stru *pst_hmac_vap)
{
    oal_int8               *pc_mib_ssid = OAL_PTR_NULL;
    oal_uint32              ul_bss_idx = 0;
    hmac_device_stru       *pst_hmac_device;
    hmac_bss_mgmt_stru     *pst_bss_mgmt = OAL_PTR_NULL;
    hmac_scanned_bss_info  *pst_scanned_bss = OAL_PTR_NULL;
    mac_bss_dscr_stru      *pst_bss_dscr = OAL_PTR_NULL;
    oal_dlist_head_stru    *pst_entry = OAL_PTR_NULL;
    oal_uint8               uc_device_id;

    /* ��ȡdevice id */
    uc_device_id = pst_hmac_vap->st_vap_base_info.uc_device_id;

    /* ��ȡhmac device�ṹ�� */
    pst_hmac_device = hmac_res_get_mac_dev(uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ASSOC, "hmac_prepare_bss_list::pst_hmac_device null");
        return;
    }

    pc_mib_ssid = (oal_int8 *)pst_hmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11DesiredSSID;

    /* û������������ssid, ��������ɨ�� */
    if (pc_mib_ssid[0] == 0) {
        OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_prepare_bss_list::desired ssid is not set.}");
        return;
    }

    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* ��ȡ�� */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ����������ap�б�����ɨ�赽��bss, ���Ƿ��������� */
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = OAL_DLIST_GET_ENTRY(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr    = &(pst_scanned_bss->st_bss_dscr_info);

        if (oal_strcmp(pc_mib_ssid, pst_bss_dscr->ac_ssid) == 0) {
            /* �������join ��AP������Ϣ�Ƿ�ƥ�� */
            if (hmac_check_capability_mac_phy_supplicant(&pst_hmac_vap->st_vap_base_info, pst_bss_dscr) != OAL_SUCC) {
                continue;
            }

            pst_bss_dscr->en_desired = OAL_TRUE;

            pst_hmac_device->auc_desired_bss_idx[pst_hmac_device->uc_desired_bss_num] = (oal_uint8)ul_bss_idx;
            pst_hmac_device->uc_desired_bss_num++;
        } else {
            pst_bss_dscr->en_desired = OAL_FALSE;
        }

        ul_bss_idx++;
    }

    /* ����� */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    hmac_join_bss_sta(pst_hmac_vap);
}
#endif


oal_void  hmac_handle_scan_rsp_sta(hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_msg)
{
    frw_event_mem_stru  *pst_event_mem;
    frw_event_stru      *pst_event = OAL_PTR_NULL;
    hmac_scan_rsp_stru  *pst_scan_rsp = OAL_PTR_NULL;

    /* ��ɨ������¼���WAL */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(hmac_scan_rsp_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "{hmac_handle_scan_rsp_sta::FRW_EVENT_ALLOC fail.}");
        return;
    }

    /* ��д�¼� */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_SCAN_COMP_STA,
                       OAL_SIZEOF(hmac_scan_rsp_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    pst_scan_rsp = (hmac_scan_rsp_stru *)pst_event->auc_event_data;

    if (memcpy_s(pst_scan_rsp, OAL_SIZEOF(hmac_scan_rsp_stru), puc_msg, OAL_SIZEOF(hmac_scan_rsp_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "hmac_handle_scan_rsp_sta::memcpy fail!");
        FRW_EVENT_FREE(pst_event_mem);
        return;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

#if  (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    /* ׼�����������bss�б� */
    hmac_prepare_bss_list(pst_hmac_vap);
#endif
}


oal_void  hmac_handle_join_rsp_sta(hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_msg)
{
    hmac_join_rsp_stru  *pst_join_rsp = (hmac_join_rsp_stru *)puc_msg;

    if (pst_join_rsp->en_result_code == HMAC_MGMT_SUCCESS) {
        OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC, "{hmac_handle_join_rsp_sta::join succ.}");

        /* ��ʼ��AUTH���� */
        pst_hmac_vap->uc_auth_cnt = 1;
        hmac_sta_initiate_auth(&(pst_hmac_vap->st_vap_base_info));
    } else {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "hmac_handle_join_rsp_sta::join fail[%d]", pst_join_rsp->en_result_code);
#ifdef _PRE_WLAN_1102A_CHR
        hmac_chr_connect_fail_query_and_report(pst_hmac_vap, MAC_JOIN_RSP_TIMEOUT);
#endif
        hmac_send_connect_result_to_dmac_sta(pst_hmac_vap, OAL_FAIL);
    }
}


oal_void hmac_report_connect_failed_result(hmac_vap_stru *pst_hmac_vap, mac_status_code_enum_uint16 reason_code)
{
    frw_event_mem_stru  *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru      *pst_event = OAL_PTR_NULL;
    hmac_asoc_rsp_stru   st_asoc_rsp;

    memset_s(&st_asoc_rsp, OAL_SIZEOF(st_asoc_rsp), 0, OAL_SIZEOF(st_asoc_rsp));
    st_asoc_rsp.en_result_code       = HMAC_MGMT_TIMEOUT;
    st_asoc_rsp.en_status_code       = reason_code;
    /* ɨ�賬ʱ��Ҫ�ͷŶ�ӦHMAC VAP�µĹ�������buff */
    st_asoc_rsp.puc_asoc_req_ie_buff = pst_hmac_vap->puc_asoc_req_ie_buff;

    /* �׼�������¼���WAL */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(hmac_asoc_rsp_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_report_connect_failed_result::FRW_EVENT_ALLOC fail!}");
        return;
    }

    /* ��д�¼� */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_ASOC_COMP_STA,
                       OAL_SIZEOF(hmac_asoc_rsp_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    if (memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(hmac_asoc_rsp_stru),
        &st_asoc_rsp, OAL_SIZEOF(hmac_asoc_rsp_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_report_connect_failed_result::memcpy fail!}");
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);
}


oal_void hmac_handle_connect_failed_result(hmac_vap_stru *pst_hmac_vap, oal_uint16 us_status)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                     "{hmac_handle_connect_failed_result::connect fail[%d]}", us_status);

    /* �ϱ�����ʧ�ܵ�wpa_supplicant */
    hmac_report_connect_failed_result(pst_hmac_vap, us_status);

    /* ��ȡ�û�ָ�� */
    pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.uc_assoc_vap_id);
    if (pst_hmac_user != OAL_PTR_NULL) {
        /* ����ȥ��֤֡��AP */
        hmac_mgmt_send_deauth_frame(&pst_hmac_vap->st_vap_base_info,
                                    pst_hmac_user->st_user_base_info.auc_user_mac_addr, us_status, OAL_FALSE);

        /* ɾ����Ӧ�û� */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    } else {
        /* ����״̬ΪFAKE UP */
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_FAKE_UP);
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "asoc:pst_hmac_user[%d] NULL.",
                         pst_hmac_vap->st_vap_base_info.uc_assoc_vap_id);
    }

    /* ͬ��DMAC״̬ */
    hmac_send_connect_result_to_dmac_sta(pst_hmac_vap, OAL_FAIL);
}


#ifdef _PRE_WLAN_FEATURE_SAE
/*
 * �� �� ��  : hmac_report_external_auth_req
 * ��������  : ����SAE external auth����wpa_supplicant
 */
oal_uint32 hmac_report_external_auth_req(hmac_vap_stru *pst_hmac_vap, oal_nl80211_external_auth_action en_action)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event;
    hmac_external_auth_req_stru st_ext_auth_req;
    oal_uint8 auc_akm[WLAN_AUTHENTICATION_SUITES] = {0};
    oal_uint8 uc_akm_suites_num;
    oal_int32 l_ret;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_SAE, "{hmac_report_external_auth_req:: hmac_vap is NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&st_ext_auth_req, OAL_SIZEOF(st_ext_auth_req), 0, OAL_SIZEOF(st_ext_auth_req));

    st_ext_auth_req.en_action = en_action;
    st_ext_auth_req.us_status = MAC_SUCCESSFUL_STATUSCODE;

    l_ret = memcpy_s(st_ext_auth_req.auc_bssid, WLAN_MAC_ADDR_LEN,
                     pst_hmac_vap->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN);

    uc_akm_suites_num = mac_mib_get_rsn_akm_suites(&pst_hmac_vap->st_vap_base_info, auc_akm);
    if (uc_akm_suites_num != 1) {
        OAM_ERROR_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
                       "{hmac_report_external_auth_req::get AKM suite failed! akm_suite_num [%d]}",
                       uc_akm_suites_num);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    st_ext_auth_req.ul_key_mgmt_suite = CIPHER_SUITE_SELECTOR(0x00, 0x0f, 0xac, auc_akm[0]);
    st_ext_auth_req.st_ssid.uc_ssid_len =
        OAL_MIN(OAL_SIZEOF(st_ext_auth_req.st_ssid.auc_ssid),
                OAL_STRLEN(mac_mib_get_DesiredSSID(&(pst_hmac_vap->st_vap_base_info))));
    l_ret += memcpy_s(st_ext_auth_req.st_ssid.auc_ssid,
                      OAL_IEEE80211_MAX_SSID_LEN,
                      mac_mib_get_DesiredSSID(&(pst_hmac_vap->st_vap_base_info)),
                      st_ext_auth_req.st_ssid.uc_ssid_len);

    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(st_ext_auth_req));
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
                         "{hmac_report_connect_failed_result::FRW_EVENT_ALLOC fail! size[%d]}",
                         OAL_SIZEOF(st_ext_auth_req));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_EXT_AUTH_REQ,
                       OAL_SIZEOF(st_ext_auth_req), FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    l_ret += memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(st_ext_auth_req),
                      &st_ext_auth_req, OAL_SIZEOF(st_ext_auth_req));
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_SAE, "hmac_report_external_auth_req::memcpy fail!");
        FRW_EVENT_FREE(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}
#endif /* _PRE_WLAN_FEATURE_SAE */


oal_void  hmac_handle_auth_rsp_sta(hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_msg)
{
    hmac_auth_rsp_stru     *pst_auth_rsp = (hmac_auth_rsp_stru *)puc_msg;
    hmac_user_stru         *pst_hmac_user = OAL_PTR_NULL;
    oal_uint8               auc_param[] = "all";

    if (pst_auth_rsp->us_status_code == HMAC_MGMT_SUCCESS) {
        OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "{hmac_handle_auth_rsp_sta::auth succ.}");

        /* ��ʼ��AOSC���� */
        pst_hmac_vap->uc_asoc_cnt = 1;
        hmac_sta_initiate_asoc(&pst_hmac_vap->st_vap_base_info);

        return;
    }

    OAM_WARNING_LOG2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
        "hmac_handle_auth_rsp_sta::auth fail[%d],cnt[%d]", pst_auth_rsp->us_status_code, pst_hmac_vap->uc_auth_cnt);

    if (((pst_auth_rsp->us_status_code == MAC_UNSUPT_ALG) ||
         (pst_hmac_vap->uc_auth_cnt >= MAX_AUTH_CNT)) &&
        (pst_hmac_vap->en_auth_mode == WLAN_WITP_AUTH_AUTOMATIC)) {
        pst_hmac_vap->en_auth_mode = WLAN_WITP_AUTH_SHARED_KEY;
        /* ��Ҫ��״̬������Ϊ */
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_JOIN_COMP);

        /* ����AUTH�Ĵ��� */
        pst_hmac_vap->uc_auth_cnt = 0;

        /* ���·���������� */
        hmac_sta_initiate_auth(&(pst_hmac_vap->st_vap_base_info));

        return;
    }

    if (pst_hmac_vap->uc_auth_cnt < MAX_AUTH_CNT) {
        /* ��Ҫ��״̬������Ϊ */
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_JOIN_COMP);

        /* ����AUTH�Ĵ��� */
        pst_hmac_vap->uc_auth_cnt++;

        /* ���·���������� */
        hmac_sta_initiate_auth(&(pst_hmac_vap->st_vap_base_info));

        return;
    } else {
        hmac_config_reg_info(&(pst_hmac_vap->st_vap_base_info), OAL_SIZEOF(auc_param), auc_param);
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    if (pst_hmac_vap->en_auth_mode == WLAN_WITP_AUTH_SAE && pst_auth_rsp->us_status_code != MAC_AP_FULL) {
        /* SAE����ʧ�ܣ��ϱ�ֹͣexternal auth��wpa_s */
        hmac_report_external_auth_req(pst_hmac_vap, NL80211_EXTERNAL_AUTH_ABORT);
    }
#endif

    hmac_send_connect_result_to_dmac_sta(pst_hmac_vap, OAL_FAIL);

    /* ��ȡ�û�ָ�� */
    pst_hmac_user = mac_res_get_hmac_user((oal_uint16)pst_hmac_vap->st_vap_base_info.uc_assoc_vap_id);
    if (pst_hmac_user != OAL_PTR_NULL) {
        /* ɾ����Ӧ�û� */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    } else {
        /* The MAC state is changed to fake up state. Further MLME */
        /* requests are processed in this state. */
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_FAKE_UP);
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "auth pst_hmac_user[%d] NULL.",
                         pst_hmac_vap->st_vap_base_info.uc_assoc_vap_id);
    }

#ifdef _PRE_WLAN_1102A_CHR
    hmac_chr_connect_fail_query_and_report(pst_hmac_vap, pst_auth_rsp->us_status_code);
#endif
    /* �ϱ�����ʧ�ܵ�wpa_supplicant */
    hmac_report_connect_failed_result(pst_hmac_vap, pst_auth_rsp->us_status_code);
}


oal_void  hmac_handle_asoc_rsp_sta(hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_msg)
{
    frw_event_mem_stru      *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru          *pst_event = OAL_PTR_NULL;
    hmac_asoc_rsp_stru      *pst_asoc_rsp = (hmac_asoc_rsp_stru *)puc_msg;
    hmac_user_stru          *pst_hmac_user = OAL_PTR_NULL;
    oal_uint8               *puc_mgmt_data = OAL_PTR_NULL;
    mac_vap_stru            *pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
#ifdef _PRE_WLAN_NARROW_BAND
    mac_cfg_ps_open_stru     st_ps_open;
#endif
    oal_int32                l_ret;

    if (pst_asoc_rsp->en_result_code == HMAC_MGMT_SUCCESS) {
        /* �׼�������¼���WAL */
        pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(hmac_asoc_rsp_stru));
        if (pst_event_mem == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{hmac_handle_asoc_rsp_sta::FRW_EVENT_ALLOC fail!}");
            return;
        }

        puc_mgmt_data = (oal_uint8 *)oal_memalloc(pst_asoc_rsp->ul_asoc_rsp_ie_len);
        if (puc_mgmt_data == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_handle_asoc_rsp_sta::mgmt_data alloc null.}");
            FRW_EVENT_FREE(pst_event_mem);
            return;
        }

#ifdef _PRE_WLAN_NARROW_BAND
        if (hitalk_status & NARROW_BAND_ON_MASK) {
            st_ps_open.uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_CMD;
            st_ps_open.uc_pm_enable = 0;
            hmac_config_set_sta_pm_on(pst_mac_vap, OAL_SIZEOF(st_ps_open), (oal_uint8 *)&st_ps_open);
        }
#endif

        l_ret = memcpy_s(puc_mgmt_data, pst_asoc_rsp->ul_asoc_rsp_ie_len,
                         (oal_uint8 *)pst_asoc_rsp->puc_asoc_rsp_ie_buff, pst_asoc_rsp->ul_asoc_rsp_ie_len);
        pst_asoc_rsp->puc_asoc_rsp_ie_buff = puc_mgmt_data;

        /* ��д�¼� */
        pst_event = (frw_event_stru *)pst_event_mem->puc_data;

        FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_HOST_CTX,
            HMAC_HOST_CTX_EVENT_SUB_TYPE_ASOC_COMP_STA, OAL_SIZEOF(hmac_asoc_rsp_stru),
            FRW_EVENT_PIPELINE_STAGE_0, pst_mac_vap->uc_chip_id, pst_mac_vap->uc_device_id, pst_mac_vap->uc_vap_id);

        l_ret += memcpy_s((oal_uint8 *)frw_get_event_payload(pst_event_mem), OAL_SIZEOF(hmac_asoc_rsp_stru),
                          (oal_uint8 *)puc_msg, OAL_SIZEOF(hmac_asoc_rsp_stru));
        if (l_ret != EOK) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "hmac_handle_asoc_rsp_sta::memcpy fail!");
            FRW_EVENT_FREE(pst_event_mem);
            oal_free(puc_mgmt_data);
            puc_mgmt_data = OAL_PTR_NULL;
            return;
        }

        /* �ַ��¼� */
        frw_event_dispatch_event(pst_event_mem);
        FRW_EVENT_FREE(pst_event_mem);
    } else {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_AUTH, "{hmac_handle_asoc_rsp_sta::asoc fail[%d],assoc_cnt[%d]}",
                         pst_asoc_rsp->en_result_code, pst_hmac_vap->uc_asoc_cnt);

        if (pst_hmac_vap->uc_asoc_cnt >= MAX_ASOC_CNT) {
            /* ��ȡ�û�ָ�� */
            pst_hmac_user = mac_res_get_hmac_user((oal_uint16)pst_hmac_vap->st_vap_base_info.uc_assoc_vap_id);
            if (pst_hmac_user != NULL) {
                /* ����ȥ��֤֡��AP */
                hmac_mgmt_send_deauth_frame(&pst_hmac_vap->st_vap_base_info,
                    pst_hmac_user->st_user_base_info.auc_user_mac_addr, MAC_AUTH_NOT_VALID, OAL_FALSE);

                /* ɾ����Ӧ�û� */
                hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
            } else {
                /* ����״̬ΪFAKE UP */
                hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_FAKE_UP);
                OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH, "asoc:pst_hmac_user[%d] NULL.",
                                 pst_mac_vap->uc_assoc_vap_id);
            }

            /* ͬ��DMAC״̬ */
            hmac_send_connect_result_to_dmac_sta(pst_hmac_vap, OAL_FAIL);

#ifdef _PRE_WLAN_1102A_CHR
            hmac_chr_connect_fail_query_and_report(pst_hmac_vap, pst_asoc_rsp->en_status_code);
#endif
            /* �ϱ�����ʧ�ܵ�wpa_supplicant */
            hmac_report_connect_failed_result(pst_hmac_vap, pst_asoc_rsp->en_status_code);
        } else {
            /* ��Ҫ��״̬������Ϊ */
            hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_AUTH_COMP);

            /* ����ASOC�Ĵ��� */
            pst_hmac_vap->uc_asoc_cnt++;

            /* ���·���������� */
            hmac_sta_initiate_asoc(&pst_hmac_vap->st_vap_base_info);
        }
    }
}


oal_void  hmac_send_rsp_to_sme_sta(hmac_vap_stru *pst_hmac_vap, hmac_sme_rsp_enum_uint8 en_type,
                                   oal_uint8 *puc_msg)
{
    g_handle_rsp_func_sta[en_type](pst_hmac_vap, puc_msg);
}


oal_void  hmac_send_rsp_to_sme_ap(hmac_vap_stru *pst_hmac_vap, hmac_ap_sme_rsp_enum_uint8 en_type,
                                  oal_uint8 *puc_msg)
{
    /* Ŀǰֻ��DMTʹ�� */
}



oal_void  hmac_send_connect_result_to_dmac_sta(hmac_vap_stru *pst_hmac_vap, oal_uint32 ul_result)
{
    frw_event_mem_stru             *pst_event_mem;
    frw_event_stru                 *pst_event = OAL_PTR_NULL;

    /* ���¼���DMAC, �����¼��ڴ� */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(oal_uint32));
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC, "pst_event_mem null.");
        return;
    }

    /* ��д�¼� */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_CONN_RESULT,
                       OAL_SIZEOF(oal_uint32),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    *((oal_uint32 *)(pst_event->auc_event_data)) = ul_result;

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    return;
}

/*lint -e578*//*lint -e19*/
oal_module_symbol(hmac_sta_initiate_scan);
oal_module_symbol(hmac_sta_initiate_join);
oal_module_symbol(hmac_cfg80211_start_scan_sta);
oal_module_symbol(hmac_cfg80211_start_sched_scan);
oal_module_symbol(hmac_cfg80211_stop_sched_scan);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
oal_module_symbol(hmac_cfg80211_dump_survey);
#endif
/*lint +e578*//*lint +e19*/

