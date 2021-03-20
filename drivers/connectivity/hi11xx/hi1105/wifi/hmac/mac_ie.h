

#ifndef __MAC_IE_H__
#define __MAC_IE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "wlan_spec.h"
#include "mac_frame.h"
#include "wlan_mib.h"
#include "wlan_types.h"
#include "mac_user.h"
#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_IE_H

#define MAC_IE_REAMIN_LEN_IS_ENOUGH(puc_src_ie, puc_curr_ie, uc_ie_len, uc_remain_len) \
    (((((puc_curr_ie) - (puc_src_ie)) + (uc_remain_len)) <= (uc_ie_len)) ? OAL_TRUE : OAL_FALSE)

/* 信道偏移定义 */
#define CHAN_OFFSET_2   2
#define CHAN_OFFSET_6   6
#define CHAN_OFFSET_10  10
#define CHAN_OFFSET_14  14

typedef struct {
    wlan_channel_bandwidth_enum_uint8 en_bw;
    uint8_t uc_ch_width;
    int8_t c_center_freq_seg1;
    int8_t c_center_freq_seg2;
} mac_bw_center_freq_map_stru;

typedef struct {
    int8_t auc_non_transmitted_ssid[WLAN_SSID_MAX_LEN];
    uint8_t uc_non_transmitted_ssid_len;
    uint16_t us_non_tramsmitted_bssid_cap;

    uint8_t auc_transmitted_bssid[WLAN_MAC_ADDR_LEN];     /* transmitted bssid */
    uint8_t auc_non_transmitted_bssid[WLAN_MAC_ADDR_LEN]; /* transmitted bssid */

    uint8_t uc_maxbssid_indicator;
    uint8_t uc_bssid_index;
    uint8_t uc_dtim_period;
    uint8_t uc_dtim_count;

    uint8_t bit_is_non_transimitted_bss : 1,
              bit_rsv : 7;
    uint8_t auc_rcv[3];
} mac_multi_bssid_frame_info_stru;


OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_get_bandwidth_from_sco(
    mac_sec_ch_off_enum_uint8 en_sec_chan_offset)
{
    switch (en_sec_chan_offset) {
        case MAC_SCA: /* Secondary Channel Above */
            return WLAN_BAND_WIDTH_40PLUS;

        case MAC_SCB: /* Secondary Channel Below */
            return WLAN_BAND_WIDTH_40MINUS;

        default: /* No Secondary Channel    */
            return WLAN_BAND_WIDTH_20M;
    }
}


OAL_STATIC OAL_INLINE mac_sec_ch_off_enum_uint8 mac_get_sco_from_bandwidth(
    wlan_channel_bandwidth_enum_uint8 en_bandwidth)
{
    switch (en_bandwidth) {
        case WLAN_BAND_WIDTH_40PLUS:
        case WLAN_BAND_WIDTH_80PLUSPLUS:
        case WLAN_BAND_WIDTH_80PLUSMINUS:
            return MAC_SCA;

        case WLAN_BAND_WIDTH_40MINUS:
        case WLAN_BAND_WIDTH_80MINUSPLUS:
        case WLAN_BAND_WIDTH_80MINUSMINUS:
            return MAC_SCB;

        default:
            return MAC_SCN;
    }
}

OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_get_80mbandwith_from_offset_channel(
    int8_t offsetChannel)
{
    switch (offsetChannel) {
        case 6: /* 中心频率相对于主20偏6个信道 */
            return WLAN_BAND_WIDTH_80PLUSPLUS;
        case -2: /* 中心频率相对于主20偏-2个信道 */
            return WLAN_BAND_WIDTH_80PLUSMINUS;
        case 2: /* 中心频率相对于主20偏2个信道 */
            return WLAN_BAND_WIDTH_80MINUSPLUS;
        case -6: /* 中心频率相对于主20偏-6个信道 */
            return WLAN_BAND_WIDTH_80MINUSMINUS;
        default:
            return WLAN_BAND_WIDTH_20M;
    }
}

OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_get_160mbandwith_from_offset_channel(
    int8_t offsetChannel)
{
    switch (offsetChannel) {
        case 14: /* 中心频率相对于主20偏14个信道 */
            return WLAN_BAND_WIDTH_160PLUSPLUSPLUS;

        case 10: /* 中心频率相对于主20偏10个信道 */
            return WLAN_BAND_WIDTH_160MINUSPLUSPLUS;

        case 6: /* 中心频率相对于主20偏6个信道 */
            return WLAN_BAND_WIDTH_160PLUSMINUSPLUS;

        case 2: /* 中心频率相对于主20偏2个信道 */
            return WLAN_BAND_WIDTH_160MINUSMINUSPLUS;

        case -2: /* 中心频率相对于主20偏-2个信道 */
            return WLAN_BAND_WIDTH_160PLUSPLUSMINUS;

        case -6:  /* 中心频率相对于主20偏-6个信道 */
            return WLAN_BAND_WIDTH_160MINUSPLUSMINUS;

        case -10:  /* 中心频率相对于主20偏-10个信道 */
            return WLAN_BAND_WIDTH_160PLUSMINUSMINUS;

        case -14:  /* 中心频率相对于主20偏-14个信道 */
            return WLAN_BAND_WIDTH_160MINUSMINUSMINUS;
        default:
            return WLAN_BAND_WIDTH_20M;
    }
}


OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 mac_get_bandwith_from_center_freq_seg0(
    uint8_t uc_chan_width, uint8_t channel, uint8_t chan_center_freq)
{
    /* 80+80不支持，暂按80M处理 */
    if (uc_chan_width & WLAN_MIB_VHT_OP_WIDTH_80) {
        return mac_get_80mbandwith_from_offset_channel(chan_center_freq - channel);
    }
#ifdef _PRE_WLAN_FEATURE_160M
    else if (uc_chan_width == WLAN_MIB_VHT_OP_WIDTH_160) {
        return mac_get_160mbandwith_from_offset_channel(chan_center_freq - channel);
    }
#endif
    else {
        return WLAN_BAND_WIDTH_20M;
    }
}

OAL_STATIC OAL_INLINE uint8_t mac_get_bandwith_from_center_freq_seg0_seg1(
    uint8_t chan_width, uint8_t channel, uint8_t chan_center_freq0, uint8_t chan_center_freq1)
{
    /* 80+80不支持，暂按80M处理 */
    if (chan_width & WLAN_MIB_VHT_OP_WIDTH_80) {
#ifdef _PRE_WLAN_FEATURE_160M
        if ((chan_center_freq1 - chan_center_freq0 == 8) || (chan_center_freq0 - chan_center_freq1 == 8)) {
            return mac_get_160mbandwith_from_offset_channel(chan_center_freq1 - channel);
        }
#endif
        return mac_get_80mbandwith_from_offset_channel(chan_center_freq0 - channel);
    }
#ifdef _PRE_WLAN_FEATURE_160M
    else if (chan_width == WLAN_MIB_VHT_OP_WIDTH_160) {
        if ((chan_center_freq1 - chan_center_freq0 == 8) || (chan_center_freq0 - chan_center_freq1 == 8)) {
            return mac_get_160mbandwith_from_offset_channel(chan_center_freq1 - channel);
        } else {
            return mac_get_160mbandwith_from_offset_channel(chan_center_freq0 - channel);
        }
    }
#endif
    else {
        return WLAN_BAND_WIDTH_20M;
    }
}

/* 11 函数声明 */
wlan_mib_mimo_power_save_enum_uint8 mac_ie_proc_sm_power_save_field(
    mac_user_stru *pst_mac_user, uint8_t uc_smps);

oal_bool_enum_uint8 mac_ie_proc_ht_green_field(mac_user_stru *pst_mac_user_sta,
                                                          mac_vap_stru *pst_mac_vap,
                                                          uint8_t uc_ht_green_field,
                                                          oal_bool_enum en_prev_asoc_ht);

oal_bool_enum_uint8 mac_ie_proc_ht_supported_channel_width(mac_user_stru *pst_mac_user_sta,
                                                                      mac_vap_stru *pst_mac_vap,
                                                                      uint8_t uc_supported_channel_width,
                                                                      oal_bool_enum en_prev_asoc_ht);

oal_bool_enum_uint8 mac_ie_proc_lsig_txop_protection_support(mac_user_stru *pst_mac_user_sta,
                                                                        mac_vap_stru *pst_mac_vap,
                                                                        uint8_t uc_lsig_txop_protection_support,
                                                                        oal_bool_enum en_prev_asoc_ht);
uint32_t mac_ie_proc_ext_cap_ie(mac_user_stru *pst_mac_user, uint8_t *puc_payload);
uint8_t mac_ie_get_chan_num(uint8_t *puc_frame_body,
                                         uint16_t us_frame_len,
                                         uint16_t us_offset,
                                         uint8_t us_curr_chan);

uint32_t mac_ie_proc_chwidth_field(mac_vap_stru *pst_mac_vap,
                                                mac_user_stru *pst_mac_user,
                                                uint8_t uc_chwidth);

uint32_t mac_set_second_channel_offset_ie(wlan_channel_bandwidth_enum_uint8 en_bw,
                                                       uint8_t *pauc_buffer,
                                                       uint8_t *puc_output_len);
uint32_t mac_set_11ac_wideband_ie(uint8_t uc_channel,
                                               wlan_channel_bandwidth_enum_uint8 en_bw,
                                               uint8_t *pauc_buffer,
                                               uint8_t *puc_output_len);
oal_bool_enum_uint8 mac_ie_check_p2p_action(uint8_t *puc_payload);
uint32_t mac_ie_proc_ht_sta(mac_vap_stru *pst_mac_sta,
                                         uint8_t *puc_payload,
                                         uint16_t us_offset,
                                         mac_user_stru *pst_mac_user_ap,
                                         uint16_t *pus_amsdu_maxsize);
uint32_t mac_ie_proc_obss_scan_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_payload);
#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t mac_ie_proc_he_opern_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_payload,
                                          mac_user_stru *pst_mac_user);
uint32_t mac_ie_parse_he_cap(uint8_t *puc_he_cap_ie, mac_frame_he_cap_ie_stru *pst_he_cap_value);
uint32_t mac_ie_from_he_cap_get_nss(mac_he_hdl_stru *pst_mac_he_hdl,
    wlan_nss_enum_uint8 *puc_nss);
uint32_t mac_ie_parse_he_oper(uint8_t *puc_he_oper_ie,
                                       mac_frame_he_oper_ie_stru *pst_he_oper_ie_value);
oal_bool_enum_uint8 mac_proc_he_uora_parameter_set_ie(mac_vap_stru *pst_mac_vap,
                                                      uint8_t *puc_payload,
                                                      uint16_t us_msg_len);
uint32_t mac_ie_parse_mu_edca_parameter(uint8_t *puc_he_edca_ie,
                                                 mac_frame_he_mu_edca_parameter_ie_stru *pst_he_mu_edca_value);
uint32_t mac_ie_parse_spatial_reuse_parameter(uint8_t *puc_he_cap_ie,
    mac_frame_he_spatial_reuse_parameter_set_ie_stru *pst_he_srp_value);
uint32_t mac_ie_parse_he_bss_color_change_announcement_ie(uint8_t *puc_payload,
    mac_frame_bss_color_change_annoncement_ie_stru *pst_bss_color);
uint32_t mac_ie_parse_he_ndp_feedback_report_ie(uint8_t *he_ndp_ie, uint8_t *nfrp_buff_threshold_exp);
uint32_t mac_ie_parse_multi_bssid_opt_subie(uint8_t *puc_frame_data,
                                                     mac_multi_bssid_frame_info_stru *pst_mbssid_info,
                                                     uint8_t *puc_mbssid_body_ie_len);
#endif
uint32_t mac_proc_ht_opern_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_payload,
                                           mac_user_stru *pst_mac_user);
uint32_t mac_ie_proc_vht_opern_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_payload,
                                               mac_user_stru *pst_mac_user);

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
uint32_t  mac_ie_proc_opmode_field(mac_vap_stru *pst_mac_vap,
    mac_user_stru *pst_mac_user, mac_opmode_notify_stru *pst_opmode_notify,
    oal_bool_enum_uint8 en_from_beacon);
#endif
uint32_t mac_ie_get_wpa_cipher(uint8_t *puc_ie, mac_crypto_settings_stru *pst_crypto);
uint32_t mac_ie_get_rsn_cipher(uint8_t *puc_ie, mac_crypto_settings_stru *pst_crypto);
uint8_t *mac_ie_find_vendor_vht_ie(uint8_t *puc_frame, uint16_t us_frame_len);
uint16_t mac_ie_get_max_mpdu_len_by_vht_cap(uint8_t uc_vht_cap_max_mpdu_len_bit);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_ie.h */
