
#include "hmac_mgmt_classifier.h"
#include "hmac_rx_data.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_fsm.h"
#include "hmac_sme_sta.h"
#include "hmac_mgmt_sta.h"
#include "hmac_mgmt_ap.h"
#include "hmac_tx_data.h"

#include "hmac_roam_main.h"

#include "hmac_dfx.h"

#include "securec.h"
#include "securectype.h"
#ifndef WIN32
#include "oal_net.h"
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#include "wal_linux_ioctl.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_CLASSIFIER_C
#define MAX_VHT_MCS_NSS 4

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


uint32_t hmac_mgmt_tx_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_action_mgmt_args_stru *action_args)
{
    if (oal_any_null_ptr3(hmac_vap, hmac_user, action_args)) {
        oam_error_log3(0, OAM_SF_TX, "{hmac_mgmt_tx_action::param null, %x %x %x.}",
                       (uintptr_t)hmac_vap, (uintptr_t)hmac_user, (uintptr_t)action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (action_args->uc_category != (uint8_t)MAC_ACTION_CATEGORY_BA) {
        oam_info_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                      "{hmac_mgmt_tx_action::invalid ba type[%d].}", action_args->uc_category);
        return OAL_FAIL;
    }
    switch (action_args->uc_action) {
        case MAC_BA_ACTION_ADDBA_REQ:
            oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_mgmt_tx_action::MAC_BA_ACTION_ADDBA_REQ.}");
            hmac_mgmt_tx_addba_req(hmac_vap, hmac_user, action_args);
            break;
        case MAC_BA_ACTION_DELBA:
            oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_mgmt_tx_action::MAC_BA_ACTION_DELBA.}");
            hmac_mgmt_tx_delba(hmac_vap, hmac_user, action_args);
            break;

        default:
            oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                             "{hmac_mgmt_tx_action::invalid ba type[%d].}", action_args->uc_action);
            return OAL_FAIL; /* �������ʹ��޸� */
    }

    return OAL_SUCC;
}

uint32_t hmac_mgmt_tx_priv_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_priv_req_args_stru *priv_req)
{
    mac_priv_req_11n_enum_uint8 req_type;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, priv_req)) {
        oam_error_log3(0, OAM_SF_TX, "{hmac_mgmt_tx_priv_req::param null, %x %x %x.}",
                       (uintptr_t)hmac_vap, (uintptr_t)hmac_user, (uintptr_t)priv_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    req_type = priv_req->uc_type;
    switch (req_type) {
        case MAC_A_MPDU_START:
            hmac_mgmt_tx_ampdu_start(hmac_vap, hmac_user, priv_req);
            break;

        case MAC_A_MPDU_END:
            hmac_mgmt_tx_ampdu_end(hmac_vap, hmac_user, priv_req);
            break;

        default:
            oam_info_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_mgmt_tx_priv_req::invalid req_type[%d].}", req_type);
            break;
    };

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_delba_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    frw_event_hdr_stru *event_hdr = NULL;
    dmac_ctx_action_event_stru *delba_event = NULL;
    uint8_t *da = NULL;             /* �����û�Ŀ�ĵ�ַ��ָ�� */
    hmac_vap_stru *hmac_vap = NULL; /* vapָ�� */
    hmac_user_stru *hmac_user = NULL;
    mac_action_mgmt_args_stru action_args;

    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_rx_delba_event::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    event = frw_get_event_stru(event_mem);
    event_hdr = &(event->st_event_hdr);
    delba_event = (dmac_ctx_action_event_stru *)(event->auc_event_data);

    /* ��ȡvap�ṹ��Ϣ */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_delba_event::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡĿ���û���MAC ADDR */
    da = delba_event->auc_mac_addr;

    /* ��ȡ���Ͷ˵��û�ָ�� */
    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, da);
    if (hmac_user == NULL) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_delba_event::mac_vap_find_user_by_macaddr failed.}");
        return OAL_FAIL;
    }
    action_args.uc_category = MAC_ACTION_CATEGORY_BA;
    action_args.uc_action = MAC_BA_ACTION_DELBA;
    action_args.ul_arg1 = delba_event->uc_tidno;     /* ������֡��Ӧ��TID�� */
    action_args.ul_arg2 = delba_event->uc_initiator; /* DELBA�У�����ɾ��BA�Ự�ķ���� */
    action_args.ul_arg3 = delba_event->uc_status;    /* DELBA�д���ɾ��reason */
    action_args.puc_arg5 = da;                       /* DELBA�д���Ŀ�ĵ�ַ */
    if (hmac_mgmt_tx_action(hmac_vap, hmac_user, &action_args) != OAL_SUCC) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_delba_event::hmac_mgmt_tx_action failed.}");
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_MONITOR

OAL_STATIC OAL_INLINE void hmac_fill_radiotap_set_bit(uint8_t *flags, uint8_t capable, uint8_t radiotap_flags)
{
    if (capable == 1) {
        *flags = *flags | radiotap_flags;
    }
}


OAL_STATIC OAL_INLINE void hmac_fill_radiotap_set_fields_wifi6_info(ieee80211_radiotap_stru *radiotap,
    hal_sniffer_rx_status_stru *rx_status, oal_uint8 bandwidth)
{
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_0] = 0;
    // fixed for GI known, pri/sec 80 known;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_1] =
        RADIO_TAP_HE_DATA2_PRI_SEC_80MHZ_KNOWN | RADIO_TAP_HE_DATA2_GI_KNOWN;
    // MCS
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_2] |=
        (rx_status->un_nss_rate.st_rate.bit_rate_mcs << RADIO_TAP_HE_DATA3_MCS);
    // LDPC
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_2] |=
        (rx_status->bit_fec_coding << RADIO_TAP_HE_DATA3_LDPC);
    // STBC
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_2] |=
        (rx_status->un_nss_rate.st_rate.bit_STBC << RADIO_TAP_HE_DATA3_STBC);
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_3] = 0;

    // HE_Data 5
    // 0=20, 1=40, 2=80, 3=160/80+80, 4=26-tone RU, 5=52-tone RU
    // 6=106-tone RU, 7=242-tone RU, 8=484-tone RU, 9=996-tone RU, 10=2x996-tone RU
    if (bandwidth == WLAN_BAND_ASSEMBLE_20M) {
        radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_4] = 0;
    } else if (bandwidth == WLAN_BAND_ASSEMBLE_40M || bandwidth == WLAN_BAND_ASSEMBLE_40M_DUP) {
        radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_4] = 1;
    } else if (bandwidth == WLAN_BAND_ASSEMBLE_80M || bandwidth == WLAN_BAND_ASSEMBLE_80M_DUP) {
        radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_4] = 2;
    } else {
        radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_4] = 3;
    }

    // GI (0=0.8us, 1=1.6us, 2=3.2us, 3=reserved), what about 3.2us case?
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_4] |= (rx_status->bit_gi_type << 4);
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_info[ARRAY_NUM_5] =
        (rx_status->un_nss_rate.st_rate.bit_nss_mode + 1);
    // set to all zero first
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_Info[ARRAY_NUM_0] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_Info[ARRAY_NUM_1] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH1[ARRAY_NUM_0] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH1[ARRAY_NUM_1] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH1[ARRAY_NUM_2] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH1[ARRAY_NUM_3] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH2[ARRAY_NUM_0] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH2[ARRAY_NUM_1] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH2[ARRAY_NUM_2] = 0;
    radiotap->st_radiotap_fields.extra_info.st_wifi6_info.HE_MU_RU_CH2[ARRAY_NUM_3] = 0;

    return;
}


OAL_STATIC OAL_INLINE void hmac_fill_radiotap_add_cap(uint32_t *flags, uint8_t capable, uint32_t radiotap_flags)
{
    if (capable == 1) {
        *flags = *flags | (1 << radiotap_flags);
    }
    return;
}

#ifndef _PRE_LINUX_TEST

#define     IEEE80211_RADIOTAP_AMPDU_STATUS_INFO (21 - 1) //  0x00100000,
#define     IEEE80211_RADIOTAP_HE_INFO       (24 - 1)     // (0x00800000)
#define     IEEE80211_RADIOTAP_HE_MU_INFO    (25 - 1)     // (0x01000000)
#define     IEEE80211_RADIOTAP_LSIG          (28 - 1)     // (0x08000000)
OAL_STATIC OAL_INLINE void hmac_fill_radiotap_set_hdr_add_cap(ieee80211_radiotap_stru *radiotap,
    oal_uint8 is_HE)
{
    radiotap->st_radiotap_header.it_version = PKTHDR_RADIOTAP_VERSION;
    radiotap->st_radiotap_header.it_pad = 0;
    radiotap->st_radiotap_header.it_len = OAL_SIZEOF(ieee80211_radiotap_stru);
    radiotap->st_radiotap_header.it_present = 0;

    /* API for 32bit integer */
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_TSFT);
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_FLAGS);
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_RATE);
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_CHANNEL);
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
    hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_DBM_ANTNOISE);
    if (is_HE) {
        hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_HE_INFO);
        hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_HE_MU_INFO);
    } else {
        hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_LOCK_QUALITY);
        hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_MCS);
        hmac_fill_radiotap_add_cap(&radiotap->st_radiotap_header.it_present, 1, IEEE80211_RADIOTAP_VHT);
    }
    return;
}
#endif


OAL_STATIC OAL_INLINE uint32_t hmac_fill_radiotap_antenna_noise(int8_t *ssi_noise, int8_t ssi_signal,
    hal_sniffer_rx_statistic_stru *sniffer_rx_statistic)
{
    *ssi_noise = ssi_signal - (sniffer_rx_statistic->c_snr_ant0 / 2) - (sniffer_rx_statistic->c_snr_ant1 / 2);
    if (((*ssi_noise) >= HMAC_PKT_CAP_NOISE_MAX) || ((*ssi_noise) < HMAC_PKT_CAP_NOISE_MIN)) {
        (*ssi_noise) = HMAC_PKT_CAP_NOISE_MIN;
    }

    return OAL_SUCC;
}
uint8_t hmac_sniffer_fill_radiotap_vht_bw(hal_statistic_stru *per_rate)
{
    uint8_t vht_bandwidth = 0;

    /* ��д��Ӧ��vht������Ϣ */
    if (per_rate->uc_bandwidth == WLAN_BAND_ASSEMBLE_20M) {
        vht_bandwidth = vht_bandwidth | IEEE80211_RADIOTAP_VHT_BW_20;
    } else if (per_rate->uc_bandwidth == WLAN_BAND_ASSEMBLE_40M ||
               per_rate->uc_bandwidth == WLAN_BAND_ASSEMBLE_40M_DUP) {
        vht_bandwidth = vht_bandwidth | IEEE80211_RADIOTAP_VHT_BW_40;
    } else if (per_rate->uc_bandwidth == WLAN_BAND_ASSEMBLE_80M ||
               per_rate->uc_bandwidth == WLAN_BAND_ASSEMBLE_80M_DUP) {
        vht_bandwidth = vht_bandwidth | IEEE80211_RADIOTAP_VHT_BW_80;
    } else {
        vht_bandwidth = vht_bandwidth | IEEE80211_RADIOTAP_VHT_BW_160;
    }

    return vht_bandwidth;
}

void hmac_sniffer_fill_radiotap(ieee80211_radiotap_stru *radiotap, mac_rx_ctl_stru *rx_ctrl,
                                hal_sniffer_rx_status_stru *rx_status,
                                hal_sniffer_rx_statistic_stru *sniffer_rx_statistic,
                                uint8_t *mac_hdr, uint32_t *rate_kbps, hal_statistic_stru *per_rate)
{
#ifndef _PRE_LINUX_TEST
    uint8_t flags = 0;     /* Do not include FCS at the end, HMAC_IEEE80211_RADIOTAP_F_FCS */
    uint8_t data_rate; /* data rate��Ϣ, 11ag��11bЭ��ʱ���ֶ���Ч */
    uint16_t ch_freq;
    uint16_t ch_type;
    int8_t ssi_signal;
    int8_t ssi_noise;
    int16_t signal_quality;
    uint8_t mcs_info_known = 0; /* mcs��Ϣ, 11nЭ��ʱ���ֶ���Ч */
    uint8_t mcs_info_flags = 0;
    uint8_t mcs_info_rate = 0;
    uint16_t vht_known = 0; /* vht��Ϣ, 11acЭ��ʱ���ֶ���Ч */
    uint8_t vht_flags = 0;
    uint8_t vht_bandwidth = 0;
    uint8_t vht_mcs_nss[MAX_VHT_MCS_NSS] = { 0 };
    uint8_t vht_coding = 0;
    uint8_t vht_group_id = 0;
    uint16_t vht_partial_aid = 0;
    mac_ieee80211_frame_stru *mac_head = NULL;
    uint8_t frame_type;
    uint32_t loop = 0;
    uint8_t  is_HE = 0;
    uint8_t protocol_mode = rx_status->un_nss_rate.st_rate.bit_protocol_mode;
    uint8_t bandwidth = rx_status->bit_freq_bandwidth_mode;

    memset_s(radiotap, OAL_SIZEOF(ieee80211_radiotap_stru), 0, OAL_SIZEOF(ieee80211_radiotap_stru));

    /* ��дfields�ֶ����flags��Ա */
    mac_head = (mac_ieee80211_frame_stru *)mac_hdr;
    frame_type = mac_get_frame_type((uint8_t *)mac_hdr);

    /* ���� rx_status->bit_preabmle��1105û�У���ʱ����Ϊ0 */
    /* 1103 rx himit flag = 1103 bit_preabmle */
    hmac_fill_radiotap_set_bit(&flags, rx_status->bit_rx_himit_flag, (uint8_t)IEEE80211_RADIOTAP_F_SHORTPRE);
    hmac_fill_radiotap_set_bit(&flags, mac_head->st_frame_control.bit_more_frag, (uint8_t)IEEE80211_RADIOTAP_F_FRAG);
    hmac_fill_radiotap_set_bit(&flags, rx_status->bit_gi_type, (uint8_t)IEEE80211_RADIOTAP_F_SHORTGI);

    /* ��дfields�ֶ��е�������Աch_freq��ch_type��ssi_signal��ssi_noise��signal_quality */
    /* todo:����һ����FCSУ�����֡������ͳ�� */
    hmac_fill_radiotap_set_bit(&flags, (rx_status->bit_dscr_status == HAL_RX_FCS_ERROR),
        IEEE80211_RADIOTAP_F_BADFCS);

    ssi_signal =
        (sniffer_rx_statistic->c_rssi_dbm != OAL_RSSI_INIT_VALUE) ? (sniffer_rx_statistic->c_rssi_dbm) : 0;

    /* snr_ant����0.5dBΪ��λ��ʵ��ʹ��ǰ��Ҫ�ȳ���2���ҳ�������snr��ʾ��Χ������Сsnr��ʾ */
    hmac_fill_radiotap_antenna_noise(&ssi_noise, ssi_signal, sniffer_rx_statistic);

    signal_quality = ssi_signal - HMAC_PKT_CAP_SIGNAL_OFFSET;

    /* ���յ��ŵ���ͨ�����ҽ��������������֡���ŵ���� */
    if (rx_ctrl->uc_channel_number < 36) {
        ch_freq = 5 * rx_ctrl->uc_channel_number + WLAN_2G_CENTER_FREQ_BASE;
        ch_type = (uint16_t)IEEE80211_CHAN_2GHZ | (uint16_t)IEEE80211_CHAN_DYN;
    } else {
        ch_freq = 5 * rx_ctrl->uc_channel_number + WLAN_5G_CENTER_FREQ_BASE;
        ch_type = (uint16_t)IEEE80211_CHAN_5GHZ | (uint16_t)IEEE80211_CHAN_OFDM;
    }

#ifdef _PRE_WLAN_FEATURE_MONITOR_DEBUG
    oam_warning_log1(0, OAM_SF_11AX, "{hmac_sniffer_fill_radiotap::bit_protocol_mode[%d]}",
                     per_rate->un_nss_rate.st_ht_rate.bit_protocol_mode);
#endif
    /*
     * ��дfields�ֶ��е�������Ϣ, ����radiotap��Ҫ��,
     * 11nʱmcs_info��Ч��11acʱvht_info��Ч��11ag��11bʱdata_rate��Ч
     */
    if (protocol_mode == WLAN_HT_PHY_PROTOCOL_MODE) {
        mcs_info_known = (uint8_t)IEEE80211_RADIOTAP_MCS_HAVE_BW | (uint8_t)IEEE80211_RADIOTAP_MCS_HAVE_MCS |
                         (uint8_t)IEEE80211_RADIOTAP_MCS_HAVE_GI | (uint8_t)IEEE80211_RADIOTAP_MCS_HAVE_FMT |
                         (uint8_t)IEEE80211_RADIOTAP_MCS_HAVE_FEC;
        /* ��������BWֻ��20, 20L��20U, ��û��40M��ѡ�� */
        hmac_fill_radiotap_set_bit(&mcs_info_flags, (uint8_t)(per_rate->uc_bandwidth != WLAN_BAND_ASSEMBLE_20M),
                                   (1 << (uint8_t)IEEE80211_RADIOTAP_MCS_BW_40));
        hmac_fill_radiotap_set_bit(&mcs_info_flags, per_rate->uc_short_gi, (uint8_t)IEEE80211_RADIOTAP_MCS_SGI);
        hmac_fill_radiotap_set_bit(&mcs_info_flags, per_rate->bit_preamble, (uint8_t)IEEE80211_RADIOTAP_MCS_FMT_GF);
        hmac_fill_radiotap_set_bit(&mcs_info_flags, per_rate->bit_channel_code,
                                   (uint8_t)IEEE80211_RADIOTAP_MCS_FEC_LDPC);
        hmac_fill_radiotap_set_bit(&mcs_info_flags, per_rate->bit_stbc, (uint8_t)IEEE80211_RADIOTAP_MCS_STBC_SHIFT);

        /* ��д��Ӧ��mcs������Ϣ */
        mcs_info_rate = per_rate->un_nss_rate.st_ht_rate.bit_ht_mcs;
#ifdef _PRE_WLAN_FEATURE_MONITOR_DEBUG
        oam_warning_log2(0, OAM_SF_11AX, "{hmac_sniffer_fill_radiotap::uc_mcs_info_flags[0x%x] uc_mcs_info_rate[%d]}",
                         mcs_info_flags, mcs_info_rate);
#endif
    } else if (protocol_mode == WLAN_VHT_PHY_PROTOCOL_MODE) {
        vht_known = (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_STBC | (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_TXOP_PS_NA |
                    (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_GI | (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_BEAMFORMED |
                    (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH |
                    (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_GROUP_ID |
                    (uint16_t)IEEE80211_RADIOTAP_VHT_KNOWN_PARTIAL_AID;
        /* vht��Ӧ��flags��Ϣ, ����STBC��Short GI�� */
        hmac_fill_radiotap_set_bit(&vht_flags, (per_rate->bit_stbc != 0), IEEE80211_RADIOTAP_VHT_FLAG_STBC);
        hmac_fill_radiotap_set_bit(&vht_flags, per_rate->uc_short_gi, IEEE80211_RADIOTAP_VHT_FLAG_SGI);
        vht_bandwidth = hmac_sniffer_fill_radiotap_vht_bw(per_rate);

        /* ��д��Ӧ��vht������Ϣ�����뷽ʽ */
        vht_mcs_nss[0] = (per_rate->un_nss_rate.st_vht_nss_mcs.bit_vht_mcs << 4) +
                            (per_rate->un_nss_rate.st_vht_nss_mcs.bit_nss_mode + 1);
        for (loop = 1; (loop < MAX_VHT_MCS_NSS) &&
            (loop < (per_rate->un_nss_rate.st_vht_nss_mcs.bit_nss_mode + 1)); loop++) {
            vht_mcs_nss[loop] = vht_mcs_nss[0];
        }
        hmac_fill_radiotap_set_bit(&vht_coding, per_rate->bit_channel_code,
                                   (uint8_t)IEEE80211_RADIOTAP_CODING_LDPC_USER0);
#ifdef _PRE_WLAN_FEATURE_MONITOR_DEBUG
        oam_warning_log3(0, OAM_SF_11AX,
                         "{hmac_sniffer_fill_radiotap::uc_vht_flags[0x%x] uc_vht_bandwidth[%d], uc_vht_mcs_nss[%d]}",
                         vht_flags, vht_bandwidth, vht_mcs_nss[0]);
#endif
    } else if (protocol_mode >= WLAN_HE_SU_FORMAT_MODE && protocol_mode <= WLAN_HE_TRIG_FORMAT_MODE) {
        is_HE = 1;
        oam_warning_log3(0, OAM_SF_11AX,
            "{hmac_sniffer_fill_radiotap::bandwidth[%d], VHT_MCS[%d], bit_protocol_mode[%d]}",
            bandwidth, rx_status->un_nss_rate.st_rate.bit_rate_mcs, protocol_mode);
    }
    data_rate = (uint8_t)((*rate_kbps) / HMAC_PKT_CAP_RATE_UNIT);

#ifdef _PRE_WLAN_FEATURE_MONITOR_DEBUG
    oam_warning_log3(0, OAM_SF_11AX,
        "{hmac_sniffer_fill_radiotap::frame type:0x%x, data_rate[%d], bit_protocol_mode[%d]}", uc_frame_type, data_rate,
        rx_status->un_nss_rate.st_ht_rate.bit_protocol_mode);
#endif
    hmac_fill_radiotap_set_hdr_add_cap(radiotap, is_HE);

    radiotap->st_radiotap_fields.ull_timestamp = 0;
    radiotap->st_radiotap_fields.uc_flags = flags;
    radiotap->st_radiotap_fields.uc_data_rate = data_rate;
    radiotap->st_radiotap_fields.us_channel_freq = ch_freq;
    radiotap->st_radiotap_fields.us_channel_type = ch_type;
    radiotap->st_radiotap_fields.c_ssi_signal = ssi_signal;
    radiotap->st_radiotap_fields.c_ssi_noise = ssi_noise;
    radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.s_signal_quality = signal_quality;

    if (is_HE) {
        hmac_fill_radiotap_set_fields_wifi6_info(radiotap, rx_status, bandwidth);
    } else {
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.s_signal_quality = signal_quality;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_mcs_info_known = mcs_info_known;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_mcs_info_flags = mcs_info_flags;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_mcs_info_rate = mcs_info_rate;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.us_vht_known = vht_known;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_flags = vht_flags;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_bandwidth = vht_bandwidth;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_mcs_nss[ARRAY_NUM_0] =
            vht_mcs_nss[ARRAY_NUM_0];
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_mcs_nss[ARRAY_NUM_1] =
            vht_mcs_nss[ARRAY_NUM_1];
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_mcs_nss[ARRAY_NUM_2] =
            vht_mcs_nss[ARRAY_NUM_2];
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_mcs_nss[ARRAY_NUM_3] =
            vht_mcs_nss[ARRAY_NUM_3];
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_coding = vht_coding;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.uc_vht_group_id = vht_group_id;
        radiotap->st_radiotap_fields.extra_info.st_legacy_wifi_info.us_vht_partial_aid = vht_partial_aid;
    }
#endif
}
#endif
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
OAL_STATIC uint32_t hmac_monitor_rx_action(oal_netbuf_stru *netbuf,
    frw_event_hdr_stru *event_hdr)
{
    mac_rx_ctl_stru *rx_ctrl = OAL_PTR_NULL; /* ָ��MPDU���ƿ���Ϣ��ָ�� */
    uint8_t *mac_hdr = OAL_PTR_NULL;
    uint8_t *data = OAL_PTR_NULL;
    hmac_vap_stru *hmac_vap = OAL_PTR_NULL;      /* vapָ�� */

    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl);
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (oal_unlikely(hmac_vap == OAL_PTR_NULL)) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_monitor_rx_action::pst_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (mac_get_frame_sub_type(mac_hdr) != WLAN_FC0_SUBTYPE_ACTION) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_monitor_rx_action::not action.}");
        return OAL_SUCC;
    }

    data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl) + rx_ctrl->uc_mac_header_len;
    if ((data[MAC_ACTION_OFFSET_CATEGORY] == MAC_ACTION_CATEGORY_PUBLIC) &&
        (data[MAC_ACTION_OFFSET_ACTION] == MAC_PUB_VENDOR_SPECIFIC)) {
        if (0 == oal_memcmp(data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN)) {
            oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                             "{hmac_monitor_rx_action::hmac location get.}");
            hmac_huawei_action_process(hmac_vap, netbuf,
                                       data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
        }
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_MONITOR

OAL_STATIC uint32_t hmac_rx_process_mgmt_event_monitor_handle(hmac_device_stru *hmac_device,
    frw_event_hdr_stru *event_hdr, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctrl = NULL; /* ָ��MPDU���ƿ���Ϣ��ָ�� */
    uint8_t *mac_hdr = NULL;

    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    /* ��ȡ��MPDU�Ŀ�����Ϣ */
    mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl);
    if (mac_get_frame_type(mac_hdr) == WLAN_FC0_TYPE_CTL) {
        hmac_device->ul_control_frames_cnt++;
    } else if (mac_get_frame_type(mac_hdr) == WLAN_FC0_TYPE_MGT) {
        hmac_device->ul_mgmt_frames_cnt++;

#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
        if (hmac_monitor_rx_action(netbuf, event_hdr) == OAL_ERR_CODE_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }
#endif
    } else {
        hmac_device->ul_others_frames_cnt++;
    }
    if (oal_value_eq_any2(hmac_device->pst_device_base_info->uc_monitor_ota_mode,
        WLAN_MONITOR_OTA_HOST_RPT, WLAN_MONITOR_OTA_ALL_RPT)) {
        hmac_monitor_report_frame_to_sdt(rx_ctrl, netbuf);
    }
    hmac_sniffer_save_data(hmac_device, netbuf, 1);
    oal_netbuf_free(netbuf); /* todo: ��ʹ�����ͷ� */
    return OAL_SUCC;
}
#endif

uint32_t hmac_rx_process_mgmt_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    frw_event_hdr_stru *event_hdr = NULL;
    dmac_wlan_crx_event_stru *crx_event = NULL;
    oal_netbuf_stru *netbuf = NULL; /* ���ڱ������ָ֡���NETBUF */
    hmac_vap_stru *hmac_vap = NULL;      /* vapָ�� */
    uint32_t ul_ret;
#ifdef _PRE_WLAN_FEATURE_MONITOR
    hmac_device_stru *hmac_device = NULL;
#endif
    mac_rx_ctl_stru *rx_ctrl = NULL; /* ָ��MPDU���ƿ���Ϣ��ָ�� */

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    event = frw_get_event_stru(event_mem);
    event_hdr = &(event->st_event_hdr);
    crx_event = (dmac_wlan_crx_event_stru *)(event->auc_event_data);
    netbuf = crx_event->pst_netbuf;
    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

#ifdef _PRE_WLAN_FEATURE_MONITOR
    hmac_device = hmac_res_get_mac_dev(event_hdr->uc_device_id);
    if (hmac_device == NULL) {
        oam_error_log1(0, OAM_SF_SCAN, "{hmac_rx_process_mgmt_event::hmac_res_get_mac_dev fail.device_id = %u}",
                       event_hdr->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (hmac_device->en_monitor_mode == OAL_TRUE) {
        return hmac_rx_process_mgmt_event_monitor_handle(hmac_device, event_hdr, netbuf);
    }
#endif

    /* ��ȡvap�ṹ��Ϣ */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_delba_event::hmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* NANֱ֡���ϱ����������״̬���ӿ� */
    if (MAC_GET_RX_CB_NAN_FLAG(rx_ctrl)) {
        hmac_rx_mgmt_send_to_host(hmac_vap, netbuf);
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        ul_ret = hmac_fsm_call_func_ap(hmac_vap, HMAC_FSM_INPUT_RX_MGMT, crx_event);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_BA, "{hmac_rx_process_mgmt_event::hmac_fsm_call_func_ap fail.ret[%u]}", ul_ret);
        }
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        ul_ret = hmac_fsm_call_func_sta(hmac_vap, HMAC_FSM_INPUT_RX_MGMT, crx_event);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_BA, "{hmac_rx_process_mgmt_event::hmac_fsm_call_func_sta fail.ret[%u]}", ul_ret);
        }
    }

    /* ����֡ͳһ�ͷŽӿ� */
    oal_netbuf_free(netbuf);

    return OAL_SUCC;
}


uint32_t hmac_rx_process_wow_mgmt_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    frw_event_hdr_stru *event_hdr = NULL;
    dmac_wlan_crx_event_stru *crx_event = NULL;
    oal_netbuf_stru *netbuf = NULL; /* ���ڱ������ָ֡���NETBUF */
    hmac_vap_stru *hmac_vap = NULL;      /* vapָ�� */
    uint32_t ul_ret;
    mac_rx_ctl_stru *rx_ctrl = NULL;

    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_mgmt_event::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    event = frw_get_event_stru(event_mem);
    event_hdr = &(event->st_event_hdr);
    crx_event = (dmac_wlan_crx_event_stru *)(event->auc_event_data);
    netbuf = crx_event->pst_netbuf;
    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    /* ��ȡvap�ṹ��Ϣ */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_warning_log0(event_hdr->uc_vap_id, OAM_SF_BA, "{hmac_rx_process_wow_mgmt_event::hmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��������sdio���������޸� */
    if (rx_ctrl->us_frame_len > rx_ctrl->uc_mac_header_len) {
        rx_ctrl->us_frame_len -= rx_ctrl->uc_mac_header_len;
    }

    /* ���չ���֡��״̬����һ�����룬����״̬���ӿ� */
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        ul_ret = hmac_fsm_call_func_ap(hmac_vap, HMAC_FSM_INPUT_RX_MGMT, crx_event);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, 0, "{hmac_rx_process_wow_mgmt_event::fsm_call_func_ap fail.err[%u]}", ul_ret);
        }
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        ul_ret = hmac_fsm_call_func_sta(hmac_vap, HMAC_FSM_INPUT_RX_MGMT, crx_event);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, 0, "{hmac_rx_process_wow_mgmt_event::fsm_call_func_sta fail.err[%u]}", ul_ret);
        }
    }

    /* ����֡ͳһ�ͷŽӿ� */
    oal_netbuf_free(netbuf);

    return OAL_SUCC;
}


uint32_t hmac_mgmt_send_disasoc_deauth_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    frw_event_hdr_stru *event_hdr = NULL;
    dmac_diasoc_deauth_event *disasoc_deauth_event = NULL;
    uint8_t *da = NULL;      /* �����û�Ŀ�ĵ�ַ��ָ�� */
    hmac_vap_stru *hmac_vap = NULL; /* vapָ�� */
    hmac_user_stru *hmac_user = NULL;
    uint32_t ul_rslt;
    uint16_t us_user_idx;
    uint8_t uc_event;
    mac_vap_stru *mac_vap = NULL;
    uint16_t err_code;
    mac_vap_stru *up_vap1 = NULL;
    mac_vap_stru *up_vap2 = NULL;
    mac_device_stru *mac_device = NULL;

    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_mgmt_send_disasoc_deauth_event::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    event = frw_get_event_stru(event_mem);
    event_hdr = &(event->st_event_hdr);
    disasoc_deauth_event = (dmac_diasoc_deauth_event *)(event->auc_event_data);

    /* ��ȡvap�ṹ��Ϣ */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(event_hdr->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_mgmt_send_disasoc_deauth_event::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mac_vap = &hmac_vap->st_vap_base_info;

    /* ��ȡĿ���û���MAC ADDR */
    da = disasoc_deauth_event->auc_des_addr;
    uc_event = disasoc_deauth_event->uc_event;
    err_code = disasoc_deauth_event->uc_reason;

    /* ����ȥ��֤, δ����״̬�յ�������֡ */
    if (uc_event == DMAC_WLAN_CRX_EVENT_SUB_TYPE_DEAUTH) {
        hmac_mgmt_send_deauth_frame(mac_vap, da, err_code, OAL_FALSE);  // ��PMF

        mac_device = mac_res_get_dev(mac_vap->uc_device_id);
        if (mac_device == NULL) {
            return OAL_SUCC;
        }
        /* �ж�����ƵDBACģʽʱ���޷��ж����ĸ��ŵ��յ�������֡�������ŵ�����Ҫ��ȥ��֤ */
        ul_rslt = mac_device_find_2up_vap(mac_device, &up_vap1, &up_vap2);
        if (ul_rslt != OAL_SUCC) {
            return OAL_SUCC;
        }

        if (up_vap1->st_channel.uc_chan_number == up_vap2->st_channel.uc_chan_number) {
            return OAL_SUCC;
        }

        /* ��ȡ��һ��VAP */
        if (mac_vap->uc_vap_id != up_vap1->uc_vap_id) {
            up_vap2 = up_vap1;
        }

        /* ����һ��VAPҲ��ȥ��֤֡��error code���������ǣ���ȥ��֤֡ʱҪ�޸�Դ��ַ */
        hmac_mgmt_send_deauth_frame(up_vap2, da, err_code | MAC_SEND_TWO_DEAUTH_FLAG, OAL_FALSE);

        return OAL_SUCC;
    }

    /* ��ȡ���Ͷ˵��û�ָ�� */
    ul_rslt = mac_vap_find_user_by_macaddr(mac_vap, da, &us_user_idx);
    if (ul_rslt != OAL_SUCC) {
        oam_warning_log4(0, OAM_SF_RX,
            "{hmac_mgmt_send_disasoc_deauth_event::Hmac cannot find USER by addr[%02X:XX:XX:%02X:%02X:%02X], \
            just del DMAC user}", da[0], da[3], da[4], da[5]);
        /*
         * �Ҳ����û���˵���û��Ѿ�ɾ����ֱ�ӷ��سɹ���
         * ����Ҫ�����¼���dmacɾ���û�(ͳһ��hmac_user_del������ɾ���û�)
         */
        return OAL_SUCC;
    }

    /* ��ȡ��hmac user,ʹ��protected��־ */
    hmac_user = mac_res_get_hmac_user(us_user_idx);

    hmac_mgmt_send_disassoc_frame(mac_vap, da, err_code, ((hmac_user == NULL) ?
                                  OAL_FALSE : hmac_user->st_user_base_info.st_cap_info.bit_pmf_active));

    if (hmac_user != NULL) {
        hmac_handle_disconnect_rsp(hmac_vap, hmac_user, err_code);
    }

    /* ɾ���û� */
    hmac_user_del(mac_vap, hmac_user);

    return OAL_SUCC;
}

OAL_STATIC mac_reason_code_enum_uint16 hmac_disassoc_reason_exchange(
    dmac_disasoc_misc_reason_enum_uint16 en_driver_disasoc_reason)
{
    switch (en_driver_disasoc_reason) {
        case DMAC_DISASOC_MISC_LINKLOSS:
        case DMAC_DISASOC_MISC_KEEPALIVE:
        case DMAC_DISASOC_MISC_GET_CHANNEL_IDX_FAIL:
            return MAC_DEAUTH_LV_SS;

        case DMAC_DISASOC_MISC_CHANNEL_MISMATCH:
            return MAC_UNSPEC_REASON;
        default:
            break;
    }
    oam_warning_log1(0, OAM_SF_ASSOC, "{hmac_disassoc_reason_exchange::Unkown driver_disasoc_reason[%d].}",
                     en_driver_disasoc_reason);

    return MAC_UNSPEC_REASON;
}


uint32_t hmac_proc_disasoc_misc_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    oal_bool_enum_uint8 en_is_protected = OAL_FALSE; /* PMF */
    dmac_disasoc_misc_stru *pdmac_disasoc_misc_stru = NULL;
    mac_reason_code_enum_uint16 en_disasoc_reason_code = MAC_UNSPEC_REASON;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_proc_disasoc_misc_event::event_mem is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    pdmac_disasoc_misc_stru = (dmac_disasoc_misc_stru *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_proc_disasoc_misc_event::hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_proc_disasoc_misc_event::Device noticed to dissasoc user[%d] within reason[%d]!}",
                     pdmac_disasoc_misc_stru->us_user_idx,
                     pdmac_disasoc_misc_stru->en_disasoc_reason);

    hmac_chr_set_disasoc_reason(pdmac_disasoc_misc_stru->us_user_idx, pdmac_disasoc_misc_stru->en_disasoc_reason);

    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_user = mac_res_get_hmac_user(pdmac_disasoc_misc_stru->us_user_idx);
        if (hmac_user == NULL) {
            oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_proc_disasoc_misc_event::hmac_user[%d] is null.}",
                             pdmac_disasoc_misc_stru->us_user_idx);
            return OAL_ERR_CODE_PTR_NULL;
        }

        en_is_protected = hmac_user->st_user_base_info.st_cap_info.bit_pmf_active;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_DISCONNECT,
                             pdmac_disasoc_misc_stru->en_disasoc_reason);
        /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
        hmac_handle_disconnect_rsp_ap(hmac_vap, hmac_user);

        en_disasoc_reason_code = MAC_ASOC_NOT_AUTH;

        /* ��ȥ����֡ */
        hmac_mgmt_send_disassoc_frame(&hmac_vap->st_vap_base_info, hmac_user->st_user_base_info.auc_user_mac_addr,
                                      en_disasoc_reason_code, en_is_protected);
        /* ɾ���û� */
        hmac_user_del(&hmac_vap->st_vap_base_info, hmac_user);
    } else {
        /* ���û� */
        hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
        if (hmac_user == NULL) {
            /* ��ap��һ�����ϲ��Ѿ�ɾ���˵Ļ����������� */
            oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_proc_disasoc_misc_event::hmac_user[%d] is null.}",
                             hmac_vap->st_vap_base_info.us_assoc_vap_id);
            return OAL_ERR_CODE_PTR_NULL;
        }

        en_is_protected = hmac_user->st_user_base_info.st_cap_info.bit_pmf_active;

        /* �ϱ��������͵�ת�� */
        en_disasoc_reason_code = hmac_disassoc_reason_exchange(pdmac_disasoc_misc_stru->en_disasoc_reason);

        if (pdmac_disasoc_misc_stru->en_disasoc_reason != DMAC_DISASOC_MISC_CHANNEL_MISMATCH) {
            /* ����ȥ��֤֡��AP */
            hmac_mgmt_send_disassoc_frame(&hmac_vap->st_vap_base_info, hmac_user->st_user_base_info.auc_user_mac_addr,
                                          en_disasoc_reason_code, en_is_protected);
        }

        /* ɾ����Ӧ�û� */
        hmac_user_del(&hmac_vap->st_vap_base_info, hmac_user);
        hmac_sta_handle_disassoc_rsp(hmac_vap, en_disasoc_reason_code);
    }

    return OAL_SUCC;
}


uint32_t hmac_proc_roam_trigger_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    int8_t c_rssi;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_proc_roam_trigger_event::event_mem is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    c_rssi = *(int8_t *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_proc_roam_trigger_event::hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_roam_trigger_handle(hmac_vap, c_rssi, OAL_TRUE);

    return OAL_SUCC;
}

/*lint -e19*/
oal_module_symbol(hmac_mgmt_tx_priv_req);
/*lint +e19*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

