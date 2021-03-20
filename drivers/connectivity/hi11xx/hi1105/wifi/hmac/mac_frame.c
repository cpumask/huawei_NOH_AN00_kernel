

/* 1 头文件包含 */
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "mac_ie.h"
#include "mac_frame.h"
#include "mac_vap.h"
#include "mac_device.h"
#include "mac_regdomain.h"
#include "mac_common.h"
#include "mac_user.h"
#include "mac_resource.h"
#include "securec.h"
#include "securectype.h"

#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_ftm.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_C

/* WMM OUI定义 */
OAL_CONST uint8_t g_auc_wmm_oui[MAC_OUI_LEN] = { 0x00, 0x50, 0xF2 };

/* WPA OUI 定义 */
OAL_CONST uint8_t g_auc_wpa_oui[MAC_OUI_LEN] = { 0x00, 0x50, 0xF2 };

/* WFA TPC RPT OUI 定义 */
OAL_CONST uint8_t g_auc_wfa_oui[MAC_OUI_LEN] = { 0x00, 0x50, 0xF2 };

/* P2P OUI 定义 */
OAL_CONST uint8_t g_auc_p2p_oui[MAC_OUI_LEN] = { 0x50, 0x6F, 0x9A };

/* RSNA OUI 定义 */
OAL_CONST uint8_t g_auc_rsn_oui[MAC_OUI_LEN] = { 0x00, 0x0F, 0xAC };

/* WPS OUI 定义 */
OAL_CONST uint8_t g_auc_wps_oui[MAC_OUI_LEN] = { 0x00, 0x50, 0xF2 };

/* 窄带 OUI 定义 */
OAL_CONST uint8_t g_auc_huawei_oui[MAC_OUI_LEN] = { 0xac, 0x85, 0x3d };
/* 2 函数原型声明 */
/* 3 全局变量定义 */
/* 4 函数实现 */

uint8_t *mac_find_ie_ext_ie(uint8_t uc_eid, uint8_t uc_ext_eid, uint8_t *puc_ies, int32_t l_len)
{
    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    while ((l_len > MAC_IE_EXT_HDR_LEN) && ((puc_ies[0] != uc_eid) || (puc_ies[2] != uc_ext_eid))) {
        l_len -= puc_ies[1] + MAC_IE_HDR_LEN;
        puc_ies += puc_ies[1] + MAC_IE_HDR_LEN;
    }

    if ((l_len > MAC_IE_EXT_HDR_LEN) && (uc_eid == puc_ies[0]) && (uc_ext_eid == puc_ies[2]) &&
        (l_len >= (MAC_IE_HDR_LEN + puc_ies[1]))) {
        return puc_ies;
    }

    return OAL_PTR_NULL;
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC void mac_set_he_mac_capinfo_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_frame_he_mac_cap_stru *pst_he_mac_capinfo = (mac_frame_he_mac_cap_stru *)puc_buffer;

    /* 初始化置0，下面填充字段时对于不支持的能力位默认不写出来 */
    memset_s(pst_he_mac_capinfo, OAL_SIZEOF(mac_frame_he_mac_cap_stru), 0, OAL_SIZEOF(mac_frame_he_mac_cap_stru));
    /*********************** HE MAC 能力信息域 ************************************
    ----------------------------------------------------------------------------
     |-----------------------------------------------------------------------------------------|
     | +HTC    | TWT         | TWT         | Fragmentation | Max Num      | Min                |
     | HE        | Requester | Responder   | support       | Fragmented   | Fragment           |
     | Support | Support     |   Support   |               | MSDUs        |     Size           |
     |-----------------------------------------------------------------------------------------|
     | B0        | B1        | B2          |   B3  B4      |  B5 B6 B7    |  B8 B9             |
     |-----------------------------------------------------------------------------------------|
     |-----------------------------------------------------------------------------------------|
     | Trigger Frame | Muti-TID     | HE           | ALL        | UMRS        | BSR            |
     | MAC Padding   | Aggregation  | Link         | ACK        | Support     | Support        |
     | Duration      | Support      | Adaptation   | Support    |             |                |
     |-----------------------------------------------------------------------------------------|
     |    B10   B11  | B12      B14 | B15    B16   |   B17      |    B18      | B19            |
     |-----------------------------------------------------------------------------------------|
     |-----------------------------------------------------------------------------------------|
     | Broadcast | 32-bit BA | MU       | Ack-Enable             | Group Addressed   Multi-STA |
     | TWT       | bitmap    | Cascade  | Multi-TID              | BlockAck In DL MU           |
     | Support   | Support   | Support  | Aggregation Support    | Support                     |
     |-----------------------------------------------------------------------------------------|
     | B20       | B21       | B22      |   B23                  |   B24                       |
     |-----------------------------------------------------------------------------------------|
     |-----------------------------------------------------------------------------------------|
     | OM        | OFDMA   | Max A-MPDU  | A-MSDU          | Flexible  TWT| Rx Control         |
     | Control   | RA      | Length      | Fragmentation   |  schedule    | frame to           |
     | Support   | Support | Exponent    |  Support        | Support      | MultiBSS           |
     |---------------------------------------------------------------------------------------- |
     | B25       | B26     | B27  B28    |   B29           |   B30        |  B31               |
     |-----------------------------------------------------------------------------------------|
     |-----------------------------------------------------------------------------------------|
     | NSRP BQRP   | QTP     | BQR     | SR       | NDP     | OPS      | A-MSDU in |           |
     | A-MPDU      |         |         |          |Feedback |  Support | A-MPDU    |           |
     | Aggregation | Support | Support |Responder | Report  |          | Support   |           |
     |-----------------------------------------------------------------------------------------|
     | B32         | B33     | B34     |   B35    |   B36   |  B37     |   B38     |           |
     |-----------------------------------------------------------------------------------------|
     |-----------------------------------------------------------------------------------------|
     | Multi-TID            |                                                                  |
     | Aggregation  Tx      |         Reserved                                                 |
     | Support              |                                                                  |
     |-----------------------------------------------------------------------------------------|
     | B39  B40   B41       |         B42--B47                                                 |
     |-----------------------------------------------------------------------------------------|
    ***************************************************************************/
    /* B0:+HTC HE Support */
    pst_he_mac_capinfo->bit_htc_he_support = OAL_TRUE;

    /* B1:TWT Requester Support */
    pst_he_mac_capinfo->bit_twt_requester_support = mac_mib_get_he_TWTOptionActivated(pst_mac_vap);

    /* B10-11:Trigger Frame MAC Padding Duration   16us */
    pst_he_mac_capinfo->bit_trigger_mac_padding_duration = mac_mib_get_he_TriggerMacPaddingDuration(pst_mac_vap);
    /* WiFi6认证用命令配置,动态设置协商mac padding */
    if (pst_mac_vap->bit_mac_padding > 0) {
        pst_he_mac_capinfo->bit_trigger_mac_padding_duration = pst_mac_vap->bit_mac_padding - 1;
    }

    /* B12-B14:Multi-TID Aggregation Rx support */
    pst_he_mac_capinfo->bit_mtid_aggregation_rx_support = WLAN_TID_MAX_NUM - 1;

    /* B19:BSR Support */
    pst_he_mac_capinfo->bit_bsr_support = mac_mib_get_he_BSRSupport(pst_mac_vap);

    /* B25:OM Control Support */
    if (pst_he_mac_capinfo->bit_htc_he_support) {
        pst_he_mac_capinfo->bit_om_control_support = mac_mib_get_OMIOptionImplemented(pst_mac_vap);
    }

    /* B26: OFDMA RA Support */
    pst_he_mac_capinfo->bit_ofdma_ra_support = mac_mib_get_he_OFDMARandomAccess(pst_mac_vap);

    /* B27-28:Maximum AMPDU Length Exponent */
    pst_he_mac_capinfo->bit_max_ampdu_length_exponent = mac_mib_get_he_MaxAMPDULengthExponent(pst_vap);


    if (IS_STA(pst_mac_vap)) {
        /* B32:BSRP BQRP AMPDU Aggregation Support */
        pst_he_mac_capinfo->bit_bsrp_bqrp_ampdu_addregation = 1;

        /* B34:BQR Support */
        pst_he_mac_capinfo->bit_bqr_support = 1;

        /* B36:NDP Feedback Report Support */
        pst_he_mac_capinfo->bit_ndp_feedback_report_support = 0;

        /* B43:UL 2×996-tone RU Support */
        pst_he_mac_capinfo->bit_ul_2x996_ru_support = 1;

        /* B47:接收HT/VHT格式的Basic trigger帧 */
        pst_he_mac_capinfo->bit_he_and_vht_trigger_frame_rx_support = 1;
    }

}


OAL_STATIC void mac_set_he_phy_capinfo_field_part2(mac_vap_stru *pst_mac_vap,
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo)
{
    /* B53: HE ER SU PPDU TRX */
    pst_he_phy_capinfo->bit_partial_bandwidth_extended_range = OAL_TRUE;

    /* B55:PPE Threshold Present */
    pst_he_phy_capinfo->bit_ppe_threshold_present = mac_mib_get_PPEThresholdsRequired(pst_mac_vap);

    /* B58:HE SU PPDU And HE MU PPDU with 4x HE-LTF And 0.8usGI */
    pst_he_phy_capinfo->bit_he_su_mu_ppdu_4xltf_08us_gi = 1;

    /* B59-B61:Max Nc */
    pst_he_phy_capinfo->bit_max_nc = 1;

    if (IS_STA(pst_mac_vap)) {
        /* B62:UL OFDMA STBC Tx>80MHz */
        pst_he_phy_capinfo->bit_stbc_tx_over_80mhz = 1;
    }

    /* B63:STBC Rx>80MHz */
    pst_he_phy_capinfo->bit_stbc_rx_over_80mhz = OAL_FALSE;

    /* B64:HE ER SU PPDU with 4x HE-LTF And 0.8usGI */
    pst_he_phy_capinfo->bit_he_er_su_ppdu_4xltf_08us_gi = 1;

    /* B65: STA强制支持,AP reserve */
    if (IS_STA(pst_mac_vap)) {
        /* 20/40MHz HE PPDU 2.4G */
        pst_he_phy_capinfo->bit_20mhz_in_40mhz_he_ppdu_2g = 1;
        /* 5G支持160M,必须强制支持 */
        pst_he_phy_capinfo->bit_20mhz_in_160mhz_he_ppdu = 1;
        pst_he_phy_capinfo->bit_80mhz_in_160mhz_he_ppdu = 1;
    }

    /* B68:HE ER SU PPDU with 1x HE-LTF And 0.8usGI */
    pst_he_phy_capinfo->bit_he_er_su_ppdu_1xltf_08us_gi = 1;

    if (IS_STA(pst_mac_vap)) {
        pst_he_phy_capinfo->bit_longer_than_16_he_sigb_support =
            g_wlan_spec_cfg->longer_than_16_he_sigb_support;
        pst_he_phy_capinfo->bit_tx_1024qam_below_242ru_support = OAL_TRUE;
        pst_he_phy_capinfo->bit_rx_1024qam_below_242ru_support = OAL_TRUE;
        /* B76:Rx Full BW SU Using HE MU PPDU With Compressed SIGB */
        pst_he_phy_capinfo->bit_rx_full_bw_su_ppdu_with_compressed_sigb = OAL_TRUE;
        pst_he_phy_capinfo->bit_rx_full_bw_su_ppdu_with_non_compressed_sigb = OAL_TRUE;
    }

}

#ifdef _PRE_WLAN_FEATURE_M2S
OAL_STATIC OAL_INLINE void mac_set_he_sounding_bfmee_sts_field(mac_vap_stru *pst_mac_vap,
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_he_hdl_stru *pst_he_hdl = OAL_PTR_NULL;

    /* 参考标杆,该字段根据对端num of sounding和自己的能力取交集 */
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);
    if (IS_STA(pst_mac_vap) && pst_mac_user != OAL_PTR_NULL) {
        pst_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
        if (pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_below_80mhz_sounding_dimensions_num != 0) {
            pst_he_phy_capinfo->bit_beamformee_sts_below_80mhz = oal_min(
                pst_he_phy_capinfo->bit_beamformee_sts_below_80mhz,
                pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_below_80mhz_sounding_dimensions_num);
            pst_he_phy_capinfo->bit_beamformee_sts_below_80mhz =
                oal_max(pst_he_phy_capinfo->bit_beamformee_sts_below_80mhz, 3);
        }
        if (pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_over_80mhz_sounding_dimensions_num != 0) {
            pst_he_phy_capinfo->bit_beamformee_sts_over_80mhz = oal_min(
                pst_he_phy_capinfo->bit_beamformee_sts_over_80mhz,
                pst_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_over_80mhz_sounding_dimensions_num);
            pst_he_phy_capinfo->bit_beamformee_sts_over_80mhz =
                oal_max(pst_he_phy_capinfo->bit_beamformee_sts_over_80mhz, 3);
        }
    }
}
#endif


OAL_STATIC void mac_set_he_sounding_capinfo_field(mac_vap_stru *pst_mac_vap,
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo)
{
    uint8_t uc_below80mhz;
    uint8_t uc_over80mhz;

    /* B31:SU Beamformer */
    pst_he_phy_capinfo->bit_su_beamformer = mac_mib_get_he_SUBeamformer(pst_mac_vap);

    /* B32:SU Beamformee */
    pst_he_phy_capinfo->bit_su_beamformee = mac_mib_get_he_SUBeamformee(pst_mac_vap);

    /* B33:MU Beamformer */
    pst_he_phy_capinfo->bit_mu_beamformer = mac_mib_get_he_MUBeamformer(pst_mac_vap);

    uc_below80mhz = mac_mib_get_he_BeamformeeSTSBelow80Mhz(pst_mac_vap);
    uc_over80mhz = mac_mib_get_he_BeamformeeSTSOver80Mhz(pst_mac_vap);
    if ((uc_below80mhz >= 1) && (uc_over80mhz >= 1)) {
        /* B34-B36:Beamformee STS ≤ 80MHz,最小值为3， 1103支持4*2 1105支持8*2 */
        pst_he_phy_capinfo->bit_beamformee_sts_below_80mhz = oal_max(uc_below80mhz - 1, 3);
        /* B37-B39:Beamformee STS > 80MHz,最小值为3， 1103支持4*2 1105支持8*2 */
        pst_he_phy_capinfo->bit_beamformee_sts_over_80mhz = oal_max(uc_over80mhz - 1, 3);
    }

    if (pst_he_phy_capinfo->bit_su_beamformer == 1) {
        /* B40-B42: beamformer支持的最大发送能力 */
        pst_he_phy_capinfo->bit_below_80mhz_sounding_dimensions_num =
            mac_mib_get_HENumberSoundingDimensionsBelow80Mhz(pst_mac_vap);

        /* B43-B45:beamformer支持的最大发送能力 */
        pst_he_phy_capinfo->bit_over_80mhz_sounding_dimensions_num =
            mac_mib_get_HENumberSoundingDimensionsOver80Mhz(pst_mac_vap);
    }

#ifdef _PRE_WLAN_FEATURE_M2S
    mac_set_he_sounding_bfmee_sts_field(pst_mac_vap, pst_he_phy_capinfo); // 封装函数降低行数
#endif

    /* B46:Ng = 16 SU Feedback */
    pst_he_phy_capinfo->bit_ng16_su_feedback = mac_mib_get_HENg16SUFeedback(pst_mac_vap);

    /* B47:Ng = 16 MU Feedback */
    pst_he_phy_capinfo->bit_ng16_mu_feedback = mac_mib_get_HENg16MUFeedback(pst_mac_vap);

    /* B48:CodeBook Size(Φ,Ψ)={4,2} SU Feedback */
    pst_he_phy_capinfo->bit_codebook_42_su_feedback = mac_mib_get_HECodebook42SUFeedback(pst_mac_vap);

    /* B49:CodeBook Size(Φ,Ψ)={7,5} MU Feedback */
    pst_he_phy_capinfo->bit_codebook_75_mu_feedback = mac_mib_get_HECodebook75MUFeedback(pst_mac_vap);

    /* B50:trigger_su_beamforming_feedback */
    pst_he_phy_capinfo->bit_trigger_su_beamforming_feedback = OAL_FALSE;

    /* B51:trigger_mu_beamforming_partialBW_feedback */
    pst_he_phy_capinfo->bit_trigger_mu_beamforming_partialBW_feedback = OAL_FALSE;

    /* B52:trigger CQI feedback */
    pst_he_phy_capinfo->bit_trigger_cqi_feedback = OAL_FALSE;

}


OAL_STATIC uint8_t mac_set_he_channel_width(mac_vap_stru *pst_mac_vap)
{
    if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G)) {
            return mac_mib_get_2GFortyMHzOperationImplemented(pst_mac_vap);
    }

    return mac_device_trans_bandwith_to_he_capinfo(mac_mib_get_dot11VapMaxBandWidth(pst_mac_vap));

}


OAL_STATIC void mac_set_he_phy_capinfo_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo = (mac_frame_he_phy_cap_stru *)puc_buffer;

    /************************************** HE PHY 能力信息域 **************************************
    -----------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | Reserved  | Channel    | Punctured  | Device  | LDPC        | HE SU PPDU With  | Midamble   |
     |           | Width      | Preamble   | Class   | Coding In   | 1x HE-LTF And    | Rx Max     |
     |           | Set        | RX         |         | Payload     | 0.8 us GI        | NSTS       |
     |---------------------------------------------------------------------------------------------|
     | B0        | B1 B7      | B8 B11     | B12     | B13         | B14              | B15 B16    |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | NDP With      | STBC Tx   | STBC Rx   | Doppler | Doppler | Full           | Partial        |
     | 4x HE-LTF And | <= 80 MHz | <= 80 MHz | Tx      | Rx      | Bandwidth UL   | Bandwidth UL   |
     | 3.2 ms GI     |           |           |         |         | MU-MIMO        | MU-MIMO        |
     |---------------------------------------------------------------------------------------------|
     | B17           | B18       | B19       | B20     | B21     | B22            | B23            |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | DCM Max       | DCM     | DCM Max       | DCM    | Rx HE MU PPDU  | SU         | SU         |
     | Constellation | Max     | Constellation | Max    | From           | Beamformer | Beamformee |
     | Tx            | NSS Tx  | Rx            | NSS Rx | Non-AP STA     |            |            |
     |---------------------------------------------------------------------------------------------|
     | B24 B25       | B26     | B27 B28       | B29    | B30            | B31        | B32        |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | MU         | Beamformee | Beamformee  | Number Of Sounding | Number Of Sounding | Ng = 16   |
     | Beamformer | STS <= 80  | STS >= 80   | Dimensions         | Dimensions         | SU        |
     |            | MHz        | MHz         | <= 80              | >= 80              | Feedback  |
     |---------------------------------------------------------------------------------------------|
     | B33        | B34 B36    | B37  B39    | B40 B42            | B43 B45            | B46       |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | Ng = 16   | Codebook     | Codebook     | Triggered SU  | Triggered MU         | Triggered  |
     | MU        | Size = {4,2} | Size = {7,5} | Beamforming   | Beamforming          | CQI        |
     | Feedback  | SU Feedback  | MU Feedback  | Feedback      | Partial BW Feedback  | Feedback   |
     |---------------------------------------------------------------------------------------------|
     | B47       | B48          | B49          | B50           | B51                  | B52        |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | Partial        | Partial      | PPE       | SRP-based  | Power Boost  | HE SU PPDU And      |
     | Bandwidth      | Bandwidth    | Threshold | SR         | Factor       | HE MU PPDU With 4x  |
     | Extended Range | DL MU-MIMO   | Present   | Support    | Support      | HE-LTF And 0.8us GI |
     |---------------------------------------------------------------------------------------------|
     | B53            | B54          | B55       | B56        | B57          | B58                 |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | Max     | STBC      | STBC      | HE ER SU PPDU   | 20 MHz In 40 MHz  | 20 MHz In           |
     | Nc      | Tx        | Rx        | With 4x HE-LTF  | HE PPDU In        | 160/80+80 MHZ       |
     |         | > 80MHz   | > 80MHz   | And 0.8us GI    | 2.4GHz Band       | HE PPDU             |
     |---------------------------------------------------------------------------------------------|
     | B59 B61 | B62       | B63       | B64             | B65               | B66                 |
     |---------------------------------------------------------------------------------------------|
     |---------------------------------------------------------------------------------------------|
     | 80 MHz In      | HE ER SU PPDU   | Midamble Rx 2x   |                                       |
     | 160/80+80 MHZ  | With 1x HE-LTF  | And              |                Reserve                |
     | HE PPDU        | And 0.8us GI    | 1x HE-LTF        |                                       |
     |---------------------------------------------------------------------------------------------|
     | B67            | B68             | B69              |                B70 B71                |
     |---------------------------------------------------------------------------------------------|
    ************************************************************************************************/
    /* 初始化置0，下面填充字段时对于不支持的能力位默认不写出来 */
    memset_s(pst_he_phy_capinfo, OAL_SIZEOF(mac_frame_he_phy_cap_stru), 0, OAL_SIZEOF(mac_frame_he_phy_cap_stru));

    /* B1-B7:channel width set, 设置vap下的带宽能力 */
    pst_he_phy_capinfo->bit_channel_width_set = mac_set_he_channel_width(pst_mac_vap);

    /* B8-B11:punctured preamble rx */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_he_phy_capinfo->bit_punctured_preamble_rx = 0;
    }
    /* B12:device class */
    pst_he_phy_capinfo->bit_device_class = PHY_DEVICE_CLASS_A;

    /* B13:LDPC Coding in payload,认证用例5.24、5.25要求支持 */
    pst_he_phy_capinfo->bit_ldpc_coding_in_paylod = mac_mib_get_he_LDPCCodingInPayload(pst_mac_vap);

    /* B14:HE SU PPDU with 1x HE-LTF and 0.8us GI */
    pst_he_phy_capinfo->bit_he_su_ppdu_1xltf_08us_gi =
        mac_mib_get_HESUPPDUwith1xHELTFand0point8GIlmplemented(pst_mac_vap);

    /* B17:HE NDP PPDU支持4x HE-LTF+3.2us Gi */
    pst_he_phy_capinfo->bit_ndp_4xltf_32us = 1;

    /* B18:UL OFDMA STBC Tx<=80MHz */
    if (IS_STA(pst_mac_vap)) {
        pst_he_phy_capinfo->bit_stbc_tx_below_80mhz = mac_mib_get_he_STBCTxBelow80M(pst_mac_vap);
    }

    /* B19:STBC Rx<=80MHz */
    pst_he_phy_capinfo->bit_stbc_rx_below_80mhz =
        (IS_STA(pst_mac_vap)) ? pst_mac_vap->st_cap_flag.bit_rx_stbc : OAL_TRUE;

    /* B22:Full Bandwidth UL MU-MIMO */
    if (IS_STA(pst_mac_vap)) {
        pst_he_phy_capinfo->bit_full_bandwidth_ul_mu_mimo = OAL_FALSE;
    }

    /* sounding参数 */
    mac_set_he_sounding_capinfo_field(pst_mac_vap, pst_he_phy_capinfo);

    mac_set_he_phy_capinfo_field_part2(pst_mac_vap, pst_he_phy_capinfo);

}


OAL_STATIC uint8_t mac_set_he_tx_rx_mcs_nss_field(mac_vap_stru *pst_vap,
                                                    uint8_t uc_channel_width_b2,
                                                    uint8_t uc_channel_width_b3,
                                                    uint8_t *puc_buffer)
{
    uint8_t uc_len = 0;
    mac_device_stru *pst_mac_dev;
    mac_fram_he_mac_nsss_set_stru *pst_he_tx_rx_mcs_nss_info = (mac_fram_he_mac_nsss_set_stru *)puc_buffer;

    pst_mac_dev = (mac_device_stru *)mac_res_get_dev(pst_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log1(pst_vap->uc_vap_id, OAM_SF_ANY,
                       "{mac_set_he_tx_rx_mcs_nss_field::mac_res_get_dev fail.device_id:[%d].}",
                       pst_vap->uc_device_id);
        return uc_len;
    }

    /******************************** HE Supported HE-MCS And NSS Set *********************************
    |-------------------------------------------------------------------------------------------------|
    | Rx HE-MCS Map | Tx HE-MCS Map | Rx HE-MCS Map  | Tx HE-MCS Map  | Rx HE-MCS Map | Tx HE-MCS Map |
    | <= 80 MHz     | <= 80 MHz     | 160 MHz        | 160 MHz        | 80+80 MHz     | 80+80 MHz     |
    |-------------------------------------------------------------------------------------------------|
    | 2 Octets      | 2 Octets      | 0 or 2 Octets  | 0 or 2 Octets  | 0 or 2 Octets | 0 or 2 Octets |
    |-------------------------------------------------------------------------------------------------|
    **************************************************************************************************/

    memset_s(pst_he_tx_rx_mcs_nss_info, OAL_SIZEOF(mac_fram_he_mac_nsss_set_stru),
             0, OAL_SIZEOF(mac_fram_he_mac_nsss_set_stru));
    /* 带宽<=80MHz的rx能力 */
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    if (WLAN_DOUBLE_NSS == MAC_DEVICE_GET_NSS_NUM(pst_mac_dev)) {
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
    }
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_3ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_4ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_5ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_6ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_7ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_below_80mhz.bit_max_he_mcs_for_8ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;

    uc_len += OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru);

    /* 带宽<=80MHz的tx能力 */
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    if (WLAN_DOUBLE_NSS == MAC_DEVICE_GET_NSS_NUM(pst_mac_dev)) {
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
    }
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_3ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_4ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_5ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_6ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_7ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_below_80mhz.bit_max_he_mcs_for_8ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;

    uc_len += OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru);

    if (uc_channel_width_b2 != 0) {
        /* Rx HE-MCS Map 160 MHz */
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        if (WLAN_DOUBLE_NSS == MAC_DEVICE_GET_NSS_NUM(pst_mac_dev)) {
            pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
        }
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_3ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_4ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_5ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_6ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_7ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_rx_he_mcs_160mhz.bit_max_he_mcs_for_8ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;

        uc_len += OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru);

        /* Tx HE-MCS Map 160 MHz */
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        if (WLAN_DOUBLE_NSS == MAC_DEVICE_GET_NSS_NUM(pst_mac_dev)) {
            pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_2ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
        }
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_3ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_4ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_5ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_6ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_7ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
        pst_he_tx_rx_mcs_nss_info->st_tx_he_mcs_160mhz.bit_max_he_mcs_for_8ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;

        uc_len += OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru);
    }

    if (uc_channel_width_b3 != 0) {
        /* TRx HE-MCS Map 80+80 MHz: 05暂不支持 */
    }

    return uc_len;
}


OAL_STATIC uint8_t mac_set_he_ppe_thresholds_field(void *pst_vap, uint8_t *puc_buffer)
{
    uint8_t uc_len;
    mac_frame_ppe_thresholds_pre_field_stru *pst_ppe_thresholds;

    pst_ppe_thresholds = (mac_frame_ppe_thresholds_pre_field_stru *)puc_buffer;

    memset_s(pst_ppe_thresholds, OAL_SIZEOF(mac_frame_ppe_thresholds_pre_field_stru),
             0, OAL_SIZEOF(mac_frame_ppe_thresholds_pre_field_stru));

    pst_ppe_thresholds->bit_nss = 1;            /* 双流 */
    pst_ppe_thresholds->bit_ru_index_mask = 0xC; /* 表示160M带宽接收HE SU PPDU只支持PE=16us */

    pst_ppe_thresholds->bit_ppet16_nss0_ru0 = 0; /* 表示任何mcs都需要16us */
    pst_ppe_thresholds->bit_ppet8_nss0_ru0 = 7;  /* PPET8设置为none */
    pst_ppe_thresholds->bit_ppet16_nss0_ru1 = 0;
    pst_ppe_thresholds->bit_ppet8_nss0_ru1 = 7;
    pst_ppe_thresholds->bit_ppet16_nss1_ru0 = 0;
    pst_ppe_thresholds->bit_ppet8_nss1_ru0 = 7;
    pst_ppe_thresholds->bit_ppet16_nss1_ru1 = 0;
    pst_ppe_thresholds->bit_ppet8_nss1_ru1 = 7;

    uc_len = OAL_SIZEOF(mac_frame_ppe_thresholds_pre_field_stru);

    return uc_len;
}

OAL_STATIC oal_bool_enum_uint8 mac_set_he_forbid_encap_he_ie(mac_vap_stru *p_vap)
{
    if (OAL_TRUE != mac_mib_get_HEOptionImplemented(p_vap) || !MAC_VAP_IS_WORK_HE_PROTOCOL(p_vap)) {
        return OAL_TRUE;
    }

    /* PF认证要求对于WEP、TKIP 加密方式不能关联在HE模式 */
    if ((OAL_TRUE == mac_is_wep_enabled(p_vap)) || (OAL_TRUE == mac_is_tkip_only(p_vap))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


/*lint -save -e438 */
void mac_set_he_capabilities_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    uint8_t *puc_ie_length = OAL_PTR_NULL;
    uint8_t uc_info_length;
    mac_frame_he_phy_cap_stru *pst_he_phy_capinfo = OAL_PTR_NULL;
    uint8_t uc_channel_width_b2;
    uint8_t uc_channel_width_b3;

    *puc_ie_len = 0;

    /* PF认证要求对于WEP、TKIP 加密方式不能关联在HE模式 */
    if (mac_set_he_forbid_encap_he_ie(pst_vap) == OAL_TRUE) {
        return;
    }

    /***************************************************************************
    -------------------------------------------------------------------------
    |EID |Length |EID Extension|HE MAC Capa. Info |HE PHY Capa. Info|
    -------------------------------------------------------------------------
    |1   |1          |1                  |5                         |9                       |
    -------------------------------------------------------------------------
    |Tx Rx HE MCS NSS Support |PPE Thresholds(Optional)|
    -------------------------------------------------------------------------
    |2 or More                         |Variable                       |
    -------------------------------------------------------------------------
    ***************************************************************************/
    *puc_buffer = MAC_EID_HE;
    puc_ie_length = puc_buffer + 1;

    puc_buffer += MAC_IE_HDR_LEN;
    *puc_ie_len += MAC_IE_HDR_LEN;

    *puc_buffer = MAC_EID_EXT_HE_CAP;
    puc_buffer += 1;

    *puc_ie_len += 1;

    /* 填充HE mac capabilities information域信息 */
    mac_set_he_mac_capinfo_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HE_MAC_CAP_LEN;
    *puc_ie_len += MAC_HE_MAC_CAP_LEN;

    /* 填充HE PHY Capabilities Information 域信息 */
    pst_he_phy_capinfo = (mac_frame_he_phy_cap_stru *)puc_buffer;
    mac_set_he_phy_capinfo_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HE_PHY_CAP_LEN;
    *puc_ie_len += MAC_HE_PHY_CAP_LEN;

    /* 填充 HE tx rx he mcs nss support */
    uc_channel_width_b2 = pst_he_phy_capinfo->bit_channel_width_set & BIT2;
    uc_channel_width_b3 = pst_he_phy_capinfo->bit_channel_width_set & BIT3;
    uc_info_length = mac_set_he_tx_rx_mcs_nss_field(pst_vap, uc_channel_width_b2, uc_channel_width_b3, puc_buffer);
    puc_buffer += uc_info_length;
    *puc_ie_len += uc_info_length;

    /* 填充 PPE Thresholds field */
    if (mac_mib_get_PPEThresholdsRequired(pst_mac_vap)) {
        uc_info_length = mac_set_he_ppe_thresholds_field(pst_vap, puc_buffer);
        puc_buffer += uc_info_length;
        *puc_ie_len += uc_info_length;
    }
    *puc_ie_length = *puc_ie_len - MAC_IE_HDR_LEN;
}

OAL_STATIC OAL_INLINE void mac_set_he_nss_bit_map_para(
    mac_frame_he_mcs_nss_bit_map_stru *pst_he_mcs_nss_bit_map)
{
    if (g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_FEATURE_CERTIFY_MODE)) {
        pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS7_11AX_EACH_NSS;
    } else {
        pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_1ss = MAC_MAX_SUP_MCS11_11AX_EACH_NSS;
    }
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_2ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_3ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_4ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_5ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_6ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_7ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
    pst_he_mcs_nss_bit_map->bit_max_he_mcs_for_8ss = MAC_MAX_SUP_INVALID_11AX_EACH_NSS;
}


void mac_set_he_operation_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru                         *p_mac_vap   = (mac_vap_stru *)pst_vap;
    uint8_t                              *puc_ie_length = OAL_PTR_NULL;
    mac_frame_he_operation_param_stru    *pst_he_opern_param = OAL_PTR_NULL;
    mac_frame_he_bss_color_info_stru     *pst_he_bss_color = OAL_PTR_NULL;
    mac_frame_he_mcs_nss_bit_map_stru    *pst_he_mcs_nss_bit_map = OAL_PTR_NULL;

    *puc_ie_len = 0;

    /* PF认证要求对于WEP、TKIP 加密方式不能关联在HE模式 */
    if (mac_set_he_forbid_encap_he_ie(p_mac_vap) == OAL_TRUE) {
        return;
    }

    /*******************HE Operation element**********************************
    --------------------------------------------------------------------
    |EID |Length |EID Extension|HE Operation Parameters |BSS Color Info|
    --------------------------------------------------------------------
    |1   |1      |1            |3                       |1             |
    --------------------------------------------------------------------
    |Basic HE-MCS |VHT Operation |Max Co-Hosted    |6 GHz Operation    |
    |And NSS Set  |Info          |BSSID Indicator  |Info               |
    --------------------------------------------------------------------
    |2            |0 or 3        | 0 or 1          |0 or 4             |
    --------------------------------------------------------------------
    ***************************************************************************/
    *puc_buffer    = MAC_EID_HE;
    puc_ie_length  = puc_buffer + 1;

    puc_buffer    += MAC_IE_HDR_LEN;
    *puc_ie_len   += MAC_IE_HDR_LEN;

    *puc_buffer    = MAC_EID_EXT_HE_OPERATION;
    puc_buffer    += 1;
    *puc_ie_len   += 1;

    pst_he_opern_param = (mac_frame_he_operation_param_stru *)puc_buffer;
    /*******************HE Operation Parameters********************************
    --------------------------------------------------------------------
    |Default PE |TWT      |TXOP Duration |VHT Operation |Co-Hosted |
    |Duration   |Required |RTS Threshold |Info. Present |BSS       |
    --------------------------------------------------------------------
    |3          |1        |10            |1             |1         |
    --------------------------------------------------------------------
    |ER SU   |6 GHz Operation |Reserved |
    |Disable |Info. Present   |         |
    -------------------------------------
    |1       |1               |6        |
    -------------------------------------
    ***************************************************************************/
    /* B0-B2:Default PE Duration, 不支持，5-7reserved */
    pst_he_opern_param->bit_default_pe_duration = MAC_HE_DEFAULT_PE_DURATION;

    /* B3:TWT Required */
    pst_he_opern_param->bit_twt_required = 0;

    /* B4-B13:TXOP Duration RTS Threshold, 1023表示该字段不可用 */
    pst_he_opern_param->bit_he_duration_rts_threshold = MAC_HE_DURATION_RTS_THRESHOLD;

    /* B14:VHT Operation Info. Present */
    /* 由于beacon和probe rsp中一定携带VHT信息，故此位置0 */
    pst_he_opern_param->bit_vht_operation_info_present = 0;

    /* B15:Co-Hosted BSS */
    pst_he_opern_param->bit_co_located_bss = 0;

    /* B16:ER SU Disable的取值，0表示支持接收242-tone HE ER SU PPDU */
    pst_he_opern_param->bit_er_su_disable = 0;

    /* B17:6 GHz Operation Info. Present的取值, 不支持6GHz band */
    /* 此位置0,表示HE Operation中6GHz Operation Info字段不存在 */
    pst_he_opern_param->bit_6g_oper_info_present = 0;

    /* B18-B23保留 */
    pst_he_opern_param->bit_reserved = 0;

    puc_buffer    += MAC_HE_OPE_PARAM_LEN;
    *puc_ie_len   += MAC_HE_OPE_PARAM_LEN;

    pst_he_bss_color = (mac_frame_he_bss_color_info_stru *)puc_buffer;
    memset_s(pst_he_bss_color, OAL_SIZEOF(mac_frame_he_bss_color_info_stru),
             0, OAL_SIZEOF(mac_frame_he_bss_color_info_stru));
    /*******************BSS Color Info*********************
    -------------------------------------------------------
    |BSS Color |Partial BSS Color    |BSS Color Disabled |
    -------------------------------------------------------
    |6         |1                    |1                  |
    -------------------------------------------------------
    *******************************************************/
    /* 支持bss color */
    pst_he_bss_color->bit_bss_color = p_mac_vap->aput_bss_color_info;
    if (pst_he_bss_color->bit_bss_color == 0) {
        pst_he_bss_color->bit_bss_color_disable = OAL_TRUE;
    } else {
        pst_he_bss_color->bit_bss_color_disable = OAL_FALSE;
    }
    puc_buffer    += 1;
    *puc_ie_len   += 1;

    /* 填充 Basic HE-MCS And NSS Set, 此字段为AP的基础能力, 适配支持接入AP的最小能力为单流MCS7 */
    pst_he_mcs_nss_bit_map = (mac_frame_he_mcs_nss_bit_map_stru *)puc_buffer;
    memset_s(pst_he_mcs_nss_bit_map, OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru),
             0, OAL_SIZEOF(mac_frame_he_mcs_nss_bit_map_stru));

    mac_set_he_nss_bit_map_para(pst_he_mcs_nss_bit_map); // 封装函数降低行数

    puc_buffer    += MAC_HE_OPE_BASIC_MCS_NSS_LEN;
    *puc_ie_len   += MAC_HE_OPE_BASIC_MCS_NSS_LEN;
    *puc_ie_length = *puc_ie_len - MAC_IE_HDR_LEN;
}


void mac_get_htc_uph_om_value(uint8_t uc_nss, uint8_t uc_bw,
    uint8_t uc_mimo_resound, uint8_t uc_ul_mu_disable, uint32_t *pul_htc_value)
{
    mac_htc_a_control_field_union   *pst_he_om_info;
    wlan_bw_cap_enum_uint8           en_bw_cap;

    en_bw_cap = WLAN_BANDWIDTH_TO_BW_CAP(uc_bw);

    *pul_htc_value = HTC_INVALID_VALUE;
    pst_he_om_info = (mac_htc_a_control_field_union *)pul_htc_value;

    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_vht_flag             = 1;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_he_flag              = 1;

    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_uph_id               = MAC_HTC_A_CONTROL_TYPE_UPH;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_ul_power_headroom    = 0;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_min_transmit_power_flag    = 0;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_uph_rsv    = 0;

    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_om_id                = MAC_HTC_A_CONTROL_TYPE_OM;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_rx_nss               = uc_nss;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_channel_width        = en_bw_cap;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_ul_mu_disable        = uc_ul_mu_disable;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_tx_nsts              = uc_nss;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_er_su_disable        = 0;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_dl_mu_mimo_resound_recommendation  = uc_mimo_resound;
    pst_he_om_info->un_a_control_info.st_uph_om_info.bit_ul_mu_data_disable   = 0;
}


void mac_get_htc_om_value(uint8_t uc_nss, uint8_t uc_bw,
    uint8_t uc_mimo_resound, uint8_t uc_ul_mu_disable, uint32_t *pul_htc_value)
{
    mac_htc_a_control_field_union   *pst_he_om_info;
    wlan_bw_cap_enum_uint8           en_bw_cap;

    en_bw_cap = WLAN_BANDWIDTH_TO_BW_CAP(uc_bw);

    *pul_htc_value = HTC_INVALID_VALUE;
    pst_he_om_info = (mac_htc_a_control_field_union *)pul_htc_value;

    pst_he_om_info->un_a_control_info.st_om_info.bit_vht_flag             = 1;
    pst_he_om_info->un_a_control_info.st_om_info.bit_he_flag              = 1;
    pst_he_om_info->un_a_control_info.st_om_info.bit_om_id                = MAC_HTC_A_CONTROL_TYPE_OM;
    pst_he_om_info->un_a_control_info.st_om_info.bit_rx_nss               = uc_nss;
    pst_he_om_info->un_a_control_info.st_om_info.bit_channel_width        = en_bw_cap;
    pst_he_om_info->un_a_control_info.st_om_info.bit_ul_mu_disable        = uc_ul_mu_disable;
    pst_he_om_info->un_a_control_info.st_om_info.bit_tx_nsts              = uc_nss;
    pst_he_om_info->un_a_control_info.st_om_info.bit_er_su_disable        = 0;
    pst_he_om_info->un_a_control_info.st_om_info.bit_dl_mu_mimo_resound_recommendation  = uc_mimo_resound;
    pst_he_om_info->un_a_control_info.st_om_info.bit_ul_mu_data_disable   = 0;
}


void mac_set_he_ie_in_assoc_rsp(void *pst_mac_ap, uint16_t us_assoc_id,
    uint8_t *puc_asoc_rsp, uint8_t *puc_ie_len)
{
    mac_user_stru     *pst_mac_user = OAL_PTR_NULL;
    uint8_t          uc_ie_len    = 0;
    mac_vap_stru      *p_mac_ap = (mac_vap_stru *)pst_mac_ap;

    pst_mac_user = mac_res_get_mac_user(us_assoc_id);

    *puc_ie_len = 0;

    /* ap 没有工作ax, 就不携带he cap 了 */
    if (!MAC_VAP_IS_WORK_HE_PROTOCOL(p_mac_ap)) {
        return ;
    }

    if ((pst_mac_user != OAL_PTR_NULL) && (mac_user_get_he_capable(pst_mac_user) == OAL_TRUE) &&
        ((g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_he_cap_switch == OAL_TRUE) ||
        (mac_user_get_he_capable(pst_mac_user) == OAL_TRUE))) {
        /* 设置 HE Capabilities IE */
        mac_set_he_capabilities_ie(pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        puc_asoc_rsp += uc_ie_len;
        *puc_ie_len  += uc_ie_len;

        /* 设置 HE Operation IE */
        mac_set_he_operation_ie(pst_mac_ap, puc_asoc_rsp, &uc_ie_len);
        *puc_ie_len  += uc_ie_len;
    }
}



void mac_set_he_ie_in_beacon(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t          uc_ie_len    = 0;

    *puc_ie_len = 0;

    if (g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_he_cap_switch == OAL_TRUE) {
        /* 设置 HE Capabilities IE */
        mac_set_he_capabilities_ie(pst_vap, puc_buffer, &uc_ie_len);
        puc_buffer += uc_ie_len;
        *puc_ie_len  += uc_ie_len;

        /* 设置 HE Operation IE */
        mac_set_he_operation_ie(pst_vap, puc_buffer, &uc_ie_len);
        puc_buffer += uc_ie_len;
        *puc_ie_len  += uc_ie_len;
    }
}
/*lint -restore */
#endif


void mac_set_hisi_cap_vendor_ie(void *p_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_ieee80211_vendor_ie_stru *pst_vendor_ie = OAL_PTR_NULL;
    mac_hisi_cap_vendor_ie_stru *pst_vendor_ie_param = OAL_PTR_NULL;
    mac_device_stru *p_mac_dev = OAL_PTR_NULL;
    mac_vap_stru *p_mac_vap = OAL_PTR_NULL;
    p_mac_vap = (mac_vap_stru *)p_vap;

    p_mac_dev = (mac_device_stru *)mac_res_get_dev(p_mac_vap->uc_device_id);
    if (p_mac_dev == OAL_PTR_NULL) {
        return;
    }

    pst_vendor_ie = (mac_ieee80211_vendor_ie_stru *)puc_buffer;
    pst_vendor_ie->uc_element_id = MAC_EID_VENDOR;
    pst_vendor_ie->uc_len = MAC_HISI_CAP_VENDOR_IE_LEN - MAC_IE_HDR_LEN;

    pst_vendor_ie->uc_oui_type = MAC_HISI_CAP_IE;

    pst_vendor_ie->auc_oui[0] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 16) & 0xff);
    pst_vendor_ie->auc_oui[1] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 8) & 0xff);
    pst_vendor_ie->auc_oui[2] = (uint8_t)((MAC_HUAWEI_VENDER_IE)&0xff);

    puc_buffer += OAL_SIZEOF(mac_ieee80211_vendor_ie_stru);
    pst_vendor_ie_param = (mac_hisi_cap_vendor_ie_stru *)puc_buffer;
    memset_s(pst_vendor_ie_param, OAL_SIZEOF(mac_hisi_cap_vendor_ie_stru), 0,
        OAL_SIZEOF(mac_hisi_cap_vendor_ie_stru));
#ifdef _PRE_WLAN_FEATURE_11AX
    if (MAC_VAP_IS_WORK_HE_PROTOCOL(p_mac_vap)) {
        pst_vendor_ie_param->bit_11ax_support = OAL_TRUE;
#ifdef _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
        if (g_wlan_spec_cfg->feature_11ax_er_su_dcm_is_open) {
            pst_vendor_ie_param->bit_dcm_support = OAL_TRUE;
        }
#endif
    }
#endif

    pst_vendor_ie_param->bit_p2p_csa_support = OAL_TRUE;
    pst_vendor_ie_param->bit_p2p_scenes = p_mac_dev->st_p2p_info.p2p_scenes;
    *puc_ie_len = OAL_SIZEOF(mac_ieee80211_vendor_ie_stru) + OAL_SIZEOF(mac_hisi_cap_vendor_ie_stru);
}

uint32_t mac_get_hisi_cap_vendor_ie(uint8_t *puc_payload, uint32_t ul_msg_len,
    mac_hisi_cap_vendor_ie_stru *pst_hisi_cap_ie)
{
    uint8_t *puc_ie_tmp = OAL_PTR_NULL;
    mac_hisi_cap_vendor_ie_stru *pst_vendor_ie_param = OAL_PTR_NULL;

    puc_ie_tmp = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_CAP_IE, puc_payload, (int32_t)ul_msg_len);
    if ((puc_ie_tmp != OAL_PTR_NULL) && (puc_ie_tmp[1] >= (MAC_HISI_CAP_VENDOR_IE_LEN - MAC_IE_HDR_LEN))) {
        puc_ie_tmp += MAC_HISI_CAP_VENDOR_IE_LEN - 1;
        pst_vendor_ie_param = (mac_hisi_cap_vendor_ie_stru *)puc_ie_tmp;
        memcpy_s(pst_hisi_cap_ie, OAL_SIZEOF(mac_hisi_cap_vendor_ie_stru),
            pst_vendor_ie_param, OAL_SIZEOF(mac_hisi_cap_vendor_ie_stru));
        return OAL_SUCC;
    }

    return OAL_FAIL;
}

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA

void mac_set_vender_4addr_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    *puc_ie_len = 0;

    /***************************************************************************
    -------------------------------------------------------------------------
    |EID | Length |HUAWEI OUI |WIFI OUT FOUR ADDR |HE PHY Capa. Info| Version |
    -------------------------------------------------------------------------
    |221 |variable|     3     |         4         |        待定     |         |
    -------------------------------------------------------------------------
    ***************************************************************************/
    /* 直接调用11KV接口的设置IE接口 宏未定义编译失败 */
#error "the virtual multi-sta feature dependent _PRE_WLAN_FEATURE_11KV_INTERFACE, please define it!"
}
#endif

#ifdef _PRE_WLAN_FEATURE_1024QAM

void mac_set_1024qam_vendor_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ieee80211_vendor_ie_stru *pst_vendor_ie = OAL_PTR_NULL;

    if (pst_mac_vap->st_cap_flag.bit_1024qam != OAL_TRUE) {
        *puc_ie_len = 0;
        return;
    }

    pst_vendor_ie = (mac_ieee80211_vendor_ie_stru *)puc_buffer;
    pst_vendor_ie->uc_element_id = MAC_EID_VENDOR;
    pst_vendor_ie->uc_len = sizeof(mac_ieee80211_vendor_ie_stru) - MAC_IE_HDR_LEN;

    pst_vendor_ie->uc_oui_type = MAC_HISI_1024QAM_IE;

    pst_vendor_ie->auc_oui[0] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 16) & 0xff);
    pst_vendor_ie->auc_oui[1] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 8) & 0xff);
    pst_vendor_ie->auc_oui[2] = (uint8_t)((MAC_HUAWEI_VENDER_IE)&0xff);

    *puc_ie_len = OAL_SIZEOF(mac_ieee80211_vendor_ie_stru);
}
#endif

#ifdef _PRE_WLAN_FEATURE_PRIV_CLOSED_LOOP_TPC

void mac_set_adjust_pow_vendor_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    /*******************************************************************************************
            ------------------------------------------------------------------------------------
            |ElementID | Length |          OUI           | OUI TYPE |Supported adjust pow|
            ------------------------------------------------------------------------------------
    Octets: |1         | 1      | 3                      | 1        | 1                  |
            ------------------------------------------------------------------------------------
    *********************************************************************************************/
    mac_hisi_adjust_pow_ie_stru *pst_vendor_ie;

    pst_vendor_ie = (mac_hisi_adjust_pow_ie_stru *)puc_buffer;
    pst_vendor_ie->uc_id = MAC_EID_ADJUST_POW_PRIVATE;
    pst_vendor_ie->uc_len = sizeof(mac_hisi_adjust_pow_ie_stru) - MAC_IE_HDR_LEN;
    pst_vendor_ie->uc_ouitype = MAC_HISI_ADJUST_POW_IE;

    pst_vendor_ie->auc_oui[0] = g_auc_huawei_oui[0];
    pst_vendor_ie->auc_oui[1] = g_auc_huawei_oui[1];
    pst_vendor_ie->auc_oui[2] = g_auc_huawei_oui[2];
    /* Supported adjust pow中4表示降低4dB，目前只支持0档和2档(降低8dB) */
    pst_vendor_ie->uc_adjust_pow = 8;

    *puc_ie_len = OAL_SIZEOF(mac_hisi_adjust_pow_ie_stru);
}
#endif


void mac_set_vendor_vht_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t uc_ie_len;

    puc_buffer[0] = MAC_EID_VENDOR;
    puc_buffer[1] = MAC_WLAN_OUI_VENDOR_VHT_HEADER; /* The Vendor OUI, type and subtype */
    /*lint -e572*/ /*lint -e778*/
    puc_buffer[2] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM >> 16) & 0xff);
    puc_buffer[3] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM >> 8) & 0xff);
    puc_buffer[4] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM) & 0xff);
    puc_buffer[5] = MAC_WLAN_OUI_VENDOR_VHT_TYPE;
    puc_buffer[6] = MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE;
    /*lint +e572*/ /*lint +e778*/

    mac_set_vht_capabilities_ie(pst_mac_vap, puc_buffer + puc_buffer[1] + MAC_IE_HDR_LEN, &uc_ie_len);
    if (uc_ie_len) {
        puc_buffer[1] += uc_ie_len;
        *puc_ie_len = puc_buffer[1] + MAC_IE_HDR_LEN;
    } else {
        *puc_ie_len = 0;
    }
}

void mac_set_vendor_novht_ie(void *pst_mac_vap, uint8_t *puc_buffer,
    uint8_t *puc_ie_len, uint8_t en_1024qam_capable)
{
    puc_buffer[0] = MAC_EID_VENDOR;
    puc_buffer[1] = MAC_WLAN_OUI_VENDOR_VHT_HEADER; /* The Vendor OUI, type and subtype */
    /*lint -e572*/ /*lint -e778*/
    puc_buffer[2] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM >> 16) & 0xff);
    puc_buffer[3] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM >> 8) & 0xff);
    puc_buffer[4] = (uint8_t)((MAC_WLAN_OUI_BROADCOM_EPIGRAM)&0xff);
    puc_buffer[5] = MAC_WLAN_OUI_VENDOR_VHT_TYPE;

    puc_buffer[6] = MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE3;
    /* 5G时，BCM私有vendor ie中携带1024QAM使能的话,从MCS9扩到MCS11 */
    if (en_1024qam_capable == OAL_TRUE) {
        puc_buffer[6] = MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE2;
    }
    /*lint +e572*/ /*lint +e778*/
    *puc_ie_len = puc_buffer[1] + MAC_IE_HDR_LEN;
}

void mac_set_ext_capabilities_ftm_twt(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
#ifdef _PRE_WLAN_FEATURE_TWT
    mac_ext_cap_twt_ie_stru *pst_ext_cap_twt = OAL_PTR_NULL;
#endif

#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    mac_ext_cap_ie_stru *pst_ext_cap;

    pst_ext_cap = (mac_ext_cap_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    /* 首先需先使能wirelessmanagerment标志 */
    /* 然后如果是站点本地能力位和扩展控制变量均支持BSS TRANSITION 设置扩展能力bit位 */
    if ((OAL_TRUE == mac_mib_get_WirelessManagementImplemented(pst_mac_vap)) &&
        (OAL_TRUE == mac_mib_get_MgmtOptionBSSTransitionImplemented(pst_mac_vap)) &&
        (OAL_TRUE == mac_mib_get_MgmtOptionBSSTransitionActivated(pst_mac_vap))) {
        pst_ext_cap->bit_bss_transition = 1;
        // todo 该if内内容需要挪走，不属于ftm以及twt范畴
    }

#ifdef _PRE_WLAN_FEATURE_FTM // FTM认证打桩
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        if (mac_is_ftm_enable(pst_mac_vap) == OAL_TRUE) {
            pst_ext_cap->bit_interworking = 1;
        }
    }
#endif

#endif

#ifdef _PRE_WLAN_FEATURE_FTM
    mac_ftm_add_to_ext_capabilities_ie((mac_vap_stru *)pst_mac_vap, puc_buffer, puc_ie_len);
#endif

#ifdef _PRE_WLAN_FEATURE_TWT
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        if (OAL_TRUE == mac_mib_get_he_TWTOptionActivated(pst_mac_vap)) {
            puc_buffer[1] = MAC_XCAPS_EX_TWT_LEN;
            pst_ext_cap_twt = (mac_ext_cap_twt_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);
            pst_ext_cap_twt->bit_resv16 = 0;
            pst_ext_cap_twt->bit_twt_requester_support = 1;
            pst_ext_cap_twt->bit_resv17 = 0;

            (*puc_ie_len) = MAC_XCAPS_EX_TWT_LEN + MAC_IE_HDR_LEN;
        }
    }
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        pst_ext_cap->bit_multi_bssid = mac_mib_get_he_MultiBSSIDActived(pst_mac_vap);
    }
#endif
}


void mac_set_vht_capinfo_field_extend(void *pst_vap, uint8_t *puc_buffer)
{
#ifdef _PRE_WLAN_FEATURE_M2S
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_vht_cap_info_stru *pst_vht_capinfo = (mac_vht_cap_info_stru *)puc_buffer;

    /* 算法限定mu bfee只在WLAN0开启 */
    if (!IS_LEGACY_STA(pst_mac_vap)) {
        pst_vht_capinfo->bit_mu_beamformee_cap = OAL_FALSE;
    }
    if (IS_LEGACY_AP(pst_mac_vap) && ((pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS) ||
        (pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_40M))) {
        pst_vht_capinfo->bit_supported_channel_width = 0;
        pst_vht_capinfo->bit_short_gi_160mhz = 0;
    }
#ifdef _PRE_WLAN_FEATURE_160M
    if ((pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
             (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        pst_vht_capinfo->bit_rx_stbc = OAL_FALSE;
    }
#endif
#endif
}



void mac_set_vht_opern_ie_rom_cb(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_vht_opern_stru *pst_vht_opern = (mac_vht_opern_stru *)puc_buffer;

    if ((pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS) &&
        (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS)) {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_80;
    }
#ifdef _PRE_WLAN_FEATURE_160M
    else if ((pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
             (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_80;
    }
#endif
    else {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_20_40;
    }

    switch (pst_mac_vap->st_channel.en_bandwidth) {
#ifdef _PRE_WLAN_FEATURE_160M
        /* 从20信道+1, 从40信道+1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160PLUSPLUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 14;
            break;

        /* 从20信道+1, 从40信道+1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160PLUSPLUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 2;
            break;

        /* 从20信道+1, 从40信道-1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 6;
            break;

        /* 从20信道+1, 从40信道-1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 10;
            break;

        /* 从20信道-1, 从40信道+1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 10;
            break;

        /* 从20信道-1, 从40信道+1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 6;
            break;

        /* 从20信道-1, 从40信道-1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 2;
            break;

        /* 从20信道-1, 从40信道-1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 14;
            break;
#endif
        case WLAN_BAND_WIDTH_80PLUSPLUS:
            /***********************************************************************
            | 主20 | 从20 | 从40       |
                          |
                          |中心频率相对于主20偏6个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            break;

        case WLAN_BAND_WIDTH_80PLUSMINUS:
            /***********************************************************************
            | 从40        | 主20 | 从20 |
                          |
                          |中心频率相对于主20偏-2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            break;

        case WLAN_BAND_WIDTH_80MINUSPLUS:
            /***********************************************************************
            | 从20 | 主20 | 从40       |
                          |
                          |中心频率相对于主20偏2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            break;

        case WLAN_BAND_WIDTH_80MINUSMINUS:
            /***********************************************************************
            | 从40        | 从20 | 主20 |
                          |
                          |中心频率相对于主20偏-6个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            break;

        // 为了提高兼容性，40M及其以下都填0
        default:
            /* 中心频率直接填0  */
            pst_vht_opern->uc_channel_center_freq_seg0 = 0;
            break;
    }
}


uint8_t *mac_get_wmm_ie_ram(uint8_t *puc_beacon_body, uint16_t us_frame_len)
{
    uint8_t *puc_wmmie = OAL_PTR_NULL;

    puc_wmmie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WMM,
                                       puc_beacon_body, us_frame_len);
    if (puc_wmmie == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    return mac_is_wmm_ie(puc_wmmie) ? puc_wmmie : OAL_PTR_NULL;
}

/*lint -e19*/
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
oal_module_symbol(mac_set_vender_4addr_ie);
#endif /*lint +e19*/

/* 4 函数实现 */

void mac_report_beacon(mac_rx_ctl_stru *pst_rx_cb, oal_netbuf_stru *pst_netbuf)
{
    uint32_t ul_ret;

    ul_ret = oam_report_beacon((uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_cb),
                                   pst_rx_cb->uc_mac_header_len,
                                   (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_cb) + pst_rx_cb->uc_mac_header_len,
                                   pst_rx_cb->us_frame_len,
                                   OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_WIFI_BEACON,
                         "{mac_report_beacon::oam_report_beacon return err: 0x%x.}\r\n", ul_ret);
    }
}


uint32_t mac_report_80211_get_switch(mac_vap_stru *pst_mac_vap,
                                       mac_rx_ctl_stru *pst_rx_cb,
                                       oam_80211_frame_ctx_union *oam_frame_report_un)
{
    mac_ieee80211_frame_stru    *pst_frame_hdr;
    uint8_t                    uc_frame_type = 0;
    uint16_t                   us_user_idx = 0xffff;
    uint8_t                   *puc_da = OAL_PTR_NULL;
    uint32_t                   ul_ret;

    pst_frame_hdr = (mac_ieee80211_frame_stru *)(mac_get_rx_cb_mac_hdr(pst_rx_cb));
    if (pst_frame_hdr == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{mac_report_80211_get_switch::pst_frame_hdr null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((pst_frame_hdr->st_frame_control.bit_type == WLAN_CONTROL) ||
        (pst_frame_hdr->st_frame_control.bit_type == WLAN_MANAGEMENT)) {
        uc_frame_type = OAM_USER_TRACK_FRAME_TYPE_MGMT;
    }

    if (pst_frame_hdr->st_frame_control.bit_type == WLAN_DATA_BASICTYPE) {
        uc_frame_type = OAM_USER_TRACK_FRAME_TYPE_DATA;
    }

    /* probe request 和 probe response太多，单独过滤一次 */
    if (pst_frame_hdr->st_frame_control.bit_type == WLAN_MANAGEMENT) {
        if (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_PROBE_REQ ||
            pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_PROBE_RSP) {
            ul_ret = oam_report_80211_probe_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_RX,
                                                           oam_frame_report_un);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "{mac_report_80211_get_switch::oam_report_80211_probe_get_switch failed.}");

                return ul_ret;
            }

            return OAL_SUCC;
        } else if (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_DEAUTH ||
                   pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_DISASOC) {
            oam_frame_report_un->frame_ctx.bit_cb = OAL_TRUE;
            oam_frame_report_un->frame_ctx.bit_dscr = OAL_TRUE;
            oam_frame_report_un->frame_ctx.bit_content = OAL_TRUE;
            return OAL_SUCC;
        } else if (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_ACTION ||
                   pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_ACTION_NO_ACK) {
            oam_frame_report_un->frame_ctx.bit_cb = OAL_TRUE;
            oam_frame_report_un->frame_ctx.bit_dscr = OAL_TRUE;
            oam_frame_report_un->frame_ctx.bit_content = OAL_TRUE;
            return OAL_SUCC;
        }
    }

    mac_rx_get_da(pst_frame_hdr, &puc_da);

    if (ether_is_multicast(puc_da)) {
        ul_ret = oam_report_80211_mcast_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_RX,
                                                       uc_frame_type,
                                                       oam_frame_report_un);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_RX,
                             "{mac_report_80211_get_switch::80211_mcast_get_switch failed! ret[%d], frame_type[%d]}",
                             ul_ret, uc_frame_type);
            return ul_ret;
        }
    } else {
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap,
                                                  pst_frame_hdr->auc_address2,
                                                  &us_user_idx);
        if (ul_ret == OAL_ERR_CODE_PTR_NULL) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{mac_report_80211_get_switch::mac_vap_find_user_by_macaddr failed.}");
            return ul_ret;
        }
        if (ul_ret == OAL_FAIL) {
            oam_frame_report_un->value = 0;

            return OAL_FAIL;
        }
        ul_ret = oam_report_80211_ucast_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_RX,
                                                       uc_frame_type,
                                                       oam_frame_report_un,
                                                       us_user_idx);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                "{mac_report_80211_get_switch::ucast_get_switch failed! ret[%d], type[%d], user_idx[%d]}",
                ul_ret, uc_frame_type, us_user_idx);
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                "{oam_report_80211_ucast_get_switch::frame_switch[%x], cb_switch[%x], dscr_switch[%x]",
                oam_frame_report_un->frame_ctx.bit_content, oam_frame_report_un->frame_ctx.bit_cb,
                oam_frame_report_un->frame_ctx.bit_dscr);

            return ul_ret;
        }
    }

    return OAL_SUCC;
}


uint32_t mac_report_80211_get_user_macaddr(mac_rx_ctl_stru *pst_rx_cb,
                                             uint8_t auc_user_macaddr[])
{
    mac_ieee80211_frame_stru *pst_frame_hdr;
    uint8_t                *puc_da = OAL_PTR_NULL;

    pst_frame_hdr = (mac_ieee80211_frame_stru *)(mac_get_rx_cb_mac_hdr(pst_rx_cb));
    if (pst_frame_hdr == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_report_80211_get_user_macaddr::pst_frame_hdr null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_rx_get_da(pst_frame_hdr, &puc_da);

    if (ether_is_multicast(puc_da)) {
        oal_set_mac_addr(&auc_user_macaddr[0], BROADCAST_MACADDR);
    } else {
        oal_set_mac_addr(&auc_user_macaddr[0], pst_frame_hdr->auc_address2);
    }

    return OAL_SUCC;
}


uint32_t mac_report_80211_frame(uint8_t *puc_mac_vap,
                                  uint8_t *puc_rx_cb,
                                  oal_netbuf_stru *pst_netbuf,
                                  uint8_t *puc_des_addr,
                                  oam_ota_type_enum_uint8 en_ota_type)
{
    oal_switch_enum_uint8    en_frame_switch;
    oal_switch_enum_uint8    en_cb_switch;
    oal_switch_enum_uint8    en_dscr_switch;
    uint32_t               ul_ret;
    uint8_t                auc_user_macaddr[WLAN_MAC_ADDR_LEN] = {0};
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)puc_mac_vap;
    mac_rx_ctl_stru         *pst_rx_cb = (mac_rx_ctl_stru *)puc_rx_cb;
    oam_80211_frame_ctx_union oam_frame_report_un = {0};

    /* 获取打印开关 */
    ul_ret = mac_report_80211_get_switch(pst_mac_vap, pst_rx_cb, &oam_frame_report_un);
    if (ul_ret == OAL_ERR_CODE_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{mac_report_80211_frame::mac_report_80211_get_switch failed.}");

        return ul_ret;
    }

    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    en_frame_switch = oam_frame_report_un.frame_ctx.bit_content;
    en_cb_switch = oam_frame_report_un.frame_ctx.bit_cb;
    en_dscr_switch = oam_frame_report_un.frame_ctx.bit_dscr;

    /* 获取发送端用户地址，用户SDT过滤,如果是组播\广播帧，则地址填为全F */
    ul_ret = mac_report_80211_get_user_macaddr(pst_rx_cb, &auc_user_macaddr[0]);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{mac_report_80211_frame::mac_report_80211_get_user_macaddr failed.}");

        return ul_ret;
    }

    if ((en_ota_type == OAM_OTA_TYPE_RX_DSCR) || (en_ota_type == OAM_OTA_TYPE_RX_DSCR_PILOT)) {
        /* 上报接收帧的接收描述符 */
        if (en_dscr_switch == OAL_SWITCH_ON) {
            ul_ret = oam_report_dscr(&auc_user_macaddr[0], puc_des_addr,
                                         (uint16_t)WLAN_RX_DSCR_SIZE, en_ota_type);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_RX,
                                 "{mac_report_80211_frame::oam_report_dscr return err: 0x%x.}\r\n", ul_ret);
            }
        }
    } else {
        /* 上报接收到的帧 */
        if (en_frame_switch == OAL_SWITCH_ON) {
            ul_ret = oam_report_80211_frame(&auc_user_macaddr[0],
                                                (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_cb),
                                                pst_rx_cb->uc_mac_header_len,
                                                (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_cb) +  \
                                                pst_rx_cb->uc_mac_header_len,
                                                pst_rx_cb->us_frame_len,
                                                OAM_OTA_FRAME_DIRECTION_TYPE_RX);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_RX,
                                 "{mac_report_80211_frame::oam_report_80211_frame return err: 0x%x.}\r\n", ul_ret);
            }
        }

        /* 上报接收帧的CB字段 */
        if (en_cb_switch == OAL_SWITCH_ON) {
            ul_ret = oam_report_netbuf_cb(auc_user_macaddr, (uint8_t *)pst_rx_cb, OAL_SIZEOF(*pst_rx_cb),
                en_ota_type);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_RX,
                                 "{mac_report_80211_frame::oam_report_netbuf_cb return err: 0x%x.}\r\n", ul_ret);
            }
        }
    }

    return OAL_SUCC;
}


uint32_t mac_rx_report_80211_frame(uint8_t *pst_vap,
                                         uint8_t *pst_rx_cb,
                                         oal_netbuf_stru *pst_netbuf,
                                         oam_ota_type_enum_uint8 en_ota_type)
{
    uint8_t        uc_sub_type;
    mac_vap_stru    *pst_mac_vap = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_mac_rx_cb = OAL_PTR_NULL;

    pst_mac_vap = (mac_vap_stru *)pst_vap;
    pst_mac_rx_cb = (mac_rx_ctl_stru *)pst_rx_cb;

    uc_sub_type = mac_get_frame_type_and_subtype((uint8_t *)mac_get_rx_cb_mac_hdr(pst_mac_rx_cb));

    if (uc_sub_type == (WLAN_FC0_SUBTYPE_BEACON | WLAN_FC0_TYPE_MGT)) {
        mac_report_beacon(pst_mac_rx_cb, pst_netbuf);
    } else {
        mac_report_80211_frame((uint8_t *)pst_mac_vap,
                               (uint8_t *)pst_mac_rx_cb,
                               pst_netbuf, OAL_PTR_NULL,
                               en_ota_type);
    }

    return OAL_SUCC;
}


uint8_t *mac_find_p2p_attribute(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len)
{
    int32_t l_ie_len = 0;

    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* 查找P2P IE，如果不是直接找下一个 */
    while (l_len > MAC_P2P_ATTRIBUTE_HDR_LEN && puc_ies[0] != uc_eid) {
        l_ie_len = (int32_t)((puc_ies[2] << 8) + puc_ies[1]);
        l_len -= l_ie_len + MAC_P2P_ATTRIBUTE_HDR_LEN;
        puc_ies += l_ie_len + MAC_P2P_ATTRIBUTE_HDR_LEN;
    }

    /* 查找到P2P IE，剩余长度不匹配直接返回空指针 */
    l_ie_len = (int32_t)((puc_ies[2] << 8) + puc_ies[1]);
    if ((l_len < MAC_P2P_ATTRIBUTE_HDR_LEN) || (l_len < (MAC_P2P_ATTRIBUTE_HDR_LEN + l_ie_len))) {
        return OAL_PTR_NULL;
    }

    return puc_ies;
}


uint8_t *mac_find_ie(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len)
{
    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* buffer长度超过1500字节认为入参异常，不做查找操作 */
    if (l_len > 1500) {
        return OAL_PTR_NULL;
    }

    while (l_len > MAC_IE_HDR_LEN && puc_ies[0] != uc_eid) {
        l_len -= puc_ies[1] + MAC_IE_HDR_LEN;
        puc_ies += puc_ies[1] + MAC_IE_HDR_LEN;
    }

    if ((l_len < MAC_IE_HDR_LEN) || (l_len < (MAC_IE_HDR_LEN + puc_ies[1])) ||
        ((l_len == MAC_IE_HDR_LEN) && (puc_ies[0] != uc_eid))) {
        return OAL_PTR_NULL;
    }

    return puc_ies;
}


uint8_t *mac_find_ie_sec(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len, uint32_t *pul_len)
{
    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* buffer长度超过1500字节认为入参异常，不做查找操作 */
    if (l_len > 1500) {
        return OAL_PTR_NULL;
    }

    while (l_len > MAC_IE_HDR_LEN && puc_ies[0] != uc_eid) {
        l_len -= puc_ies[1] + MAC_IE_HDR_LEN;
        puc_ies += puc_ies[1] + MAC_IE_HDR_LEN;
    }

    if ((l_len < MAC_IE_HDR_LEN) || (l_len < (MAC_IE_HDR_LEN + puc_ies[1])) ||
        ((l_len == MAC_IE_HDR_LEN) && (puc_ies[0] != uc_eid))) {
        return OAL_PTR_NULL;
    }

    *pul_len = l_len;
    return puc_ies;
}



uint8_t *mac_find_vendor_ie(uint32_t ul_oui,
                                  uint8_t uc_oui_type,
                                  uint8_t *puc_ies,
                                  int32_t l_len)
{
    struct mac_ieee80211_vendor_ie  *pst_ie = OAL_PTR_NULL;
    uint8_t                       *puc_pos = OAL_PTR_NULL;
    uint8_t                       *puc_end = OAL_PTR_NULL;
    uint32_t                       ul_ie_oui;

    if (puc_ies == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    puc_pos = puc_ies;
    puc_end = puc_ies + l_len;
    while (puc_pos < puc_end) {
        puc_pos = mac_find_ie(MAC_EID_VENDOR, puc_pos, (int32_t)(puc_end - puc_pos));
        if (puc_pos == OAL_PTR_NULL) {
            return OAL_PTR_NULL;
        }

        pst_ie = (struct mac_ieee80211_vendor_ie *)puc_pos;
        if (pst_ie->uc_len >= (sizeof(*pst_ie) - MAC_IE_HDR_LEN)) {
            ul_ie_oui = pst_ie->auc_oui[0] << 16 | pst_ie->auc_oui[1] << 8 | pst_ie->auc_oui[2];
            if ((ul_ie_oui == ul_oui) && (pst_ie->uc_oui_type == uc_oui_type)) {
                return puc_pos;
            }
        }
        puc_pos += 2 + pst_ie->uc_len;
    }
    return OAL_PTR_NULL;
}


void mac_set_beacon_interval_field(void *pst_vap, uint8_t *puc_buffer)
{
    uint16_t      *pus_bcn_int;
    uint32_t       ul_bcn_int;
    mac_vap_stru    *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /*****************************************************************************
                |Beacon interval|
        Octets:        2
    *****************************************************************************/
    pus_bcn_int = (uint16_t *)puc_buffer;

    ul_bcn_int = mac_mib_get_BeaconPeriod(pst_mac_vap);

    *pus_bcn_int = (uint16_t)oal_byteorder_to_le32(ul_bcn_int);
}


void mac_set_cap_info_ap(void *pst_vap, uint8_t *puc_cap_info)
{
    mac_cap_info_stru   *pst_cap_info = (mac_cap_info_stru *)puc_cap_info;
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /**************************************************************************
         -------------------------------------------------------------------
         |B0 |B1  |B2        |B3    |B4     |B5      |B6  |B7     |B8      |
         -------------------------------------------------------------------
         |ESS|IBSS|CFPollable|CFPReq|Privacy|Preamble|PBCC|Agility|SpecMgmt|
         -------------------------------------------------------------------
         |B9 |B10      |B11 |B12     |B13      |B14        |B15            |
         -------------------------------------------------------------------
         |QoS|ShortSlot|APSD|RM      |DSSS-OFDM|Delayed BA |Immediate BA   |
         -------------------------------------------------------------------
    ***************************************************************************/
    /* 初始清零 */
    puc_cap_info[0] = 0;
    puc_cap_info[1] = 0;

    if (WLAN_MIB_DESIRED_BSSTYPE_INDEPENDENT == mac_mib_get_DesiredBSSType(pst_mac_vap)) {
        pst_cap_info->bit_ibss = 1;
    } else if (WLAN_MIB_DESIRED_BSSTYPE_INFRA == mac_mib_get_DesiredBSSType(pst_mac_vap)) {
        pst_cap_info->bit_ess = 1;
    }

    /* The Privacy bit is set if WEP is enabled */
    pst_cap_info->bit_privacy = mac_mib_get_privacyinvoked(pst_mac_vap);

    /* preamble */
    pst_cap_info->bit_short_preamble = mac_mib_get_ShortPreambleOptionImplemented(pst_mac_vap);

    /* packet binary convolutional code (PBCC) modulation */
    pst_cap_info->bit_pbcc = mac_mib_get_PBCCOptionImplemented(pst_mac_vap);

    /* Channel Agility */
    pst_cap_info->bit_channel_agility = mac_mib_get_ChannelAgilityPresent(pst_mac_vap);

    /* Spectrum Management */
    pst_cap_info->bit_spectrum_mgmt = mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap);

    /* QoS subfield */
    pst_cap_info->bit_qos = 0;

    /* short slot */
    pst_cap_info->bit_short_slot_time = mac_mib_get_ShortSlotTimeOptionImplemented(pst_mac_vap) &
                                        mac_mib_get_ShortSlotTimeOptionActivated(pst_mac_vap);

    /* APSD */
    pst_cap_info->bit_apsd = mac_mib_get_dot11APSDOptionImplemented(pst_mac_vap);

    /* Radio Measurement */
    pst_cap_info->bit_radio_measurement = mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap);

    /* DSSS-OFDM */
    pst_cap_info->bit_dsss_ofdm = mac_mib_get_DSSSOFDMOptionActivated(pst_mac_vap);

    /* Delayed BA */
    pst_cap_info->bit_delayed_block_ack = mac_mib_get_dot11DelayedBlockAckOptionImplemented(pst_mac_vap);

    /* Immediate Block Ack 参考STA及AP标杆，此能力一直为0,实际通过addba协商。此处修改为标杆一致。mib值不修改 */
    pst_cap_info->bit_immediate_block_ack = 0;
}


void mac_set_cap_info_sta(void *pst_vap, uint8_t *puc_cap_info)
{
    mac_cap_info_stru   *pst_cap_info = (mac_cap_info_stru *)puc_cap_info;
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    int32_t            l_ret;

    /**************************************************************************
         -------------------------------------------------------------------
         |B0 |B1  |B2        |B3    |B4     |B5      |B6  |B7     |B8      |
         -------------------------------------------------------------------
         |ESS|IBSS|CFPollable|CFPReq|Privacy|Preamble|PBCC|Agility|SpecMgmt|
         -------------------------------------------------------------------
         |B9 |B10      |B11 |B12     |B13      |B14        |B15            |
         -------------------------------------------------------------------
         |QoS|ShortSlot|APSD|RM      |DSSS-OFDM|Delayed BA |Immediate BA   |
         -------------------------------------------------------------------
    ***************************************************************************/
    /* 学习对端的能力信息 */
    l_ret = memcpy_s(puc_cap_info, OAL_SIZEOF(mac_cap_info_stru),
                     (uint8_t *)(&pst_mac_vap->us_assoc_user_cap_info),
                     OAL_SIZEOF(mac_cap_info_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_cap_info_sta::memcpy fail!");
        return;
    }

    /* 以下能力位不学习，保持默认值 */
    pst_cap_info->bit_ibss = 0;
    pst_cap_info->bit_cf_pollable = 0;
    pst_cap_info->bit_cf_poll_request = 0;
#if defined(_PRE_WLAN_FEATURE_11K)
    pst_cap_info->bit_radio_measurement = (mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap) &&
        (pst_cap_info->bit_radio_measurement || pst_mac_vap->bit_bss_include_rrm_ie));
#endif
}


void mac_set_ssid_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len, uint16_t us_frm_type)
{
    uint8_t       *puc_ssid = OAL_PTR_NULL;
    uint8_t        uc_ssid_len;
    mac_vap_stru    *pst_mac_vap = (mac_vap_stru *)pst_vap;
    int32_t        l_ret;

    /***************************************************************************
                    ----------------------------
                    |Element ID | Length | SSID|
                    ----------------------------
           Octets:  |1          | 1      | 0~32|
                    ----------------------------
    ***************************************************************************/
    /***************************************************************************
      A SSID  field  of length 0 is  used  within Probe
      Request management frames to indicate the wildcard SSID.
    ***************************************************************************/
    /* 只有beacon会隐藏ssid */
    if ((pst_mac_vap->st_cap_flag.bit_hide_ssid) && (us_frm_type == WLAN_FC0_SUBTYPE_BEACON)) {
        /* ssid ie */
        *puc_buffer = MAC_EID_SSID;
        /* ssid len */
        *(puc_buffer + 1) = 0;
        *puc_ie_len = MAC_IE_HDR_LEN;
        return;
    }

    *puc_buffer = MAC_EID_SSID;

    puc_ssid = mac_mib_get_DesiredSSID(pst_mac_vap);

    uc_ssid_len = (uint8_t)OAL_STRLEN((int8_t *)puc_ssid); /* 不包含'\0' */

    *(puc_buffer + 1) = uc_ssid_len;

    l_ret = memcpy_s(puc_buffer + MAC_IE_HDR_LEN, uc_ssid_len, puc_ssid, uc_ssid_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_ssid_ie::memcpy fail!");
        return;
    }

    *puc_ie_len = uc_ssid_len + MAC_IE_HDR_LEN;
}

#ifdef _PRE_WLAN_NARROW_BAND

void mac_set_nb_ie(uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t uc_index;
    int32_t l_ret;

    /* ----------------------------------------------------------------- */
    /* NB Information/Parameter Element Format                           */
    /* ----------------------------------------------------------------- */
    /* EID | IE LEN | OUI | OUIType | Narrow Band|                       */
    /* ----------------------------------------------------------------- */
    /* 1   |   1    |  3  | 1       |3           |                       */
    /* ----------------------------------------------------------------- */
    /* 填写EID, 长度最后填 */
    puc_buffer[0] = MAC_EID_VENDOR;

    /* 初始化填写buffer的位置 */
    uc_index = MAC_IE_HDR_LEN;

    l_ret = memcpy_s(puc_buffer + uc_index, MAC_OUI_LEN, g_auc_huawei_oui, MAC_OUI_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_nb_ie::memcpy fail!");
        return;
    }

    uc_index += MAC_OUI_LEN;

    puc_buffer[uc_index++] = MAC_HISI_NB_IE; /* oui_type */

    puc_buffer[uc_index++] = NARROW_BW_1M;
    puc_buffer[uc_index++] = NARROW_BW_5M;
    puc_buffer[uc_index++] = NARROW_BW_10M;

    /* 设置信息元素长度 */
    puc_buffer[1] = uc_index - MAC_IE_HDR_LEN;
    *puc_ie_len = uc_index;
}
#endif


void mac_set_supported_rates_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_rateset_stru    *pst_rates_set;
    uint8_t            uc_nrates;
    uint8_t            uc_idx;

    pst_rates_set = &(pst_mac_vap->st_curr_sup_rates.st_rate);

    /* STA全信道扫描时根据频段设置supported rates */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA && ((pst_mac_vap->en_protocol == WLAN_VHT_MODE) ||
        (g_wlan_spec_cfg->feature_11ax_is_open && (pst_mac_vap->en_protocol == WLAN_HE_MODE)))) {
        if (pst_mac_vap->st_channel.en_band < WLAN_BAND_BUTT) {
            pst_rates_set = &(pst_mac_vap->ast_sta_sup_rates_ie[pst_mac_vap->st_channel.en_band].st_rate);
        }
    }

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

    uc_nrates = pst_rates_set->uc_rs_nrates;

    if (uc_nrates > MAC_MAX_SUPRATES) {
        uc_nrates = MAC_MAX_SUPRATES;
    }

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_rates_set->ast_rs_rates[uc_idx].uc_mac_rate;
    }

    puc_buffer[1] = uc_nrates;

    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}


void mac_set_dsss_params(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len, uint8_t uc_chan_num)
{
    /***************************************************************************
                        ----------------------------------------
                        | Element ID  | Length |Current Channel|
                        ----------------------------------------
              Octets:   | 1           | 1      | 1             |
                        ----------------------------------------
    The DSSS Parameter Set element contains information to allow channel number identification for STAs.
    ***************************************************************************/
    
    puc_buffer[0] = MAC_EID_DSPARMS;
    puc_buffer[1] = MAC_DSPARMS_LEN;
    puc_buffer[2] = uc_chan_num;

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_DSPARMS_LEN;
}

#ifdef _PRE_WLAN_FEATURE_11D

void mac_set_country_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_regdomain_info_stru *pst_rd_info = OAL_PTR_NULL;
    uint8_t                uc_band;
    uint8_t                uc_index;
    uint32_t               ul_ret;
    uint8_t                uc_len = 0;

    if (OAL_TRUE != mac_mib_get_dot11MultiDomainCapabilityActivated(pst_mac_vap) &&
        OAL_TRUE != mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap) &&
        OAL_TRUE != mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap)) {
        /* 没有使能管制域ie */
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
                               |....These three fields are repeated...|
    -------------------------------------------------------------------------------
    |EID | Len | CountryString | First Channel |Number of |Maximum    | Pad       |
    |    |     |               |   Number/     |Channels/ |Transmit   |(if needed)|
    |    |     |               |   Operating   | Operating|Power Level|           |
    |    |     |               |   Extension   | Class    |/Coverage  |           |
    |    |     |               |   Identifier  |          |Class      |           |
    -------------------------------------------------------------------------------
    |1   |1    |3              |1              |1         |1          |0 or 1     |
    -------------------------------------------------------------------------------
    ***************************************************************************/
    /* 读取管制域信息 */
    mac_get_regdomain_info(&pst_rd_info);

    /* 获取当前工作频段 */
    uc_band = pst_mac_vap->st_channel.en_band;

    /* 填写EID, 长度最后填 */
    puc_buffer[0] = MAC_EID_COUNTRY;

    /* 初始化填写buffer的位置 */
    uc_index = MAC_IE_HDR_LEN;

    /* 国家码 */
    puc_buffer[uc_index++] = (uint8_t)(pst_rd_info->ac_country[0]);
    puc_buffer[uc_index++] = (uint8_t)(pst_rd_info->ac_country[1]);
    puc_buffer[uc_index++] = ' '; /* 0表示室内室外规定相同 */

    if (uc_band == WLAN_BAND_2G) {
        ul_ret = mac_set_country_ie_2g(pst_rd_info, &(puc_buffer[uc_index]), &uc_len);
    } else if (uc_band == WLAN_BAND_5G) {
        ul_ret = mac_set_country_ie_5g(pst_rd_info, &(puc_buffer[uc_index]), &uc_len);
    } else {
        ul_ret = OAL_FAIL;
    }

    if (ul_ret != OAL_SUCC) {
        *puc_ie_len = 0;
        return;
    }

    if (uc_len == 0) {
        /* 无管制域内容 */
        *puc_ie_len = 0;
        return;
    }

    uc_index += uc_len;

    /* 如果总长度为奇数，则补1字节pad */
    if ((uc_index & BIT0) == 1) {
        puc_buffer[uc_index] = 0;
        uc_index += 1;
    }

    /* 设置信息元素长度 */
    puc_buffer[1] = uc_index - MAC_IE_HDR_LEN;
    *puc_ie_len = uc_index;
}
#endif

void mac_set_11ntxbf_vendor_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru                *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_11ntxbf_vendor_ie_stru  *pst_vendor_ie = OAL_PTR_NULL;
    if (pst_mac_vap->st_cap_flag.bit_11ntxbf != OAL_TRUE) {
        *puc_ie_len = 0;
        return;
    }

    pst_vendor_ie = (mac_11ntxbf_vendor_ie_stru *)puc_buffer;
    pst_vendor_ie->uc_id = MAC_EID_VENDOR;
    pst_vendor_ie->uc_len = sizeof(mac_11ntxbf_vendor_ie_stru) - MAC_IE_HDR_LEN;
    /* 此值为CCB决策 */
    pst_vendor_ie->uc_ouitype = MAC_EID_11NTXBF;

    /* lint -e572 */ /* lint -e778 */
    pst_vendor_ie->auc_oui[0] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 16) & 0xff);
    pst_vendor_ie->auc_oui[1] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 8) & 0xff);
    pst_vendor_ie->auc_oui[2] = (uint8_t)((MAC_HUAWEI_VENDER_IE)&0xff);
    /* lint +e572 */ /* lint +e778 */
    memset_s(&pst_vendor_ie->st_11ntxbf, OAL_SIZEOF(mac_11ntxbf_info_stru), 0, OAL_SIZEOF(mac_11ntxbf_info_stru));
    pst_vendor_ie->st_11ntxbf.bit_11ntxbf = pst_mac_vap->st_cap_flag.bit_11ntxbf;
    *puc_ie_len = OAL_SIZEOF(mac_11ntxbf_vendor_ie_stru);
}


void mac_set_pwrconstraint_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /***************************************************************************
                   -------------------------------------------
                   |ElementID | Length | LocalPowerConstraint|
                   -------------------------------------------
       Octets:     |1         | 1      | 1                   |
                   -------------------------------------------

    向工作站描述其所允许的最大传输功率，此信息元素记录规定最大值
    减去实际使用时的最大值
    ***************************************************************************/
    if (OAL_FALSE == mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    *puc_buffer = MAC_EID_PWRCNSTR;
    *(puc_buffer + 1) = MAC_PWR_CONSTRAINT_LEN;

    /* Note that this field is always set to 0 currently. Ideally            */
    /* this field can be updated by having an algorithm to decide transmit   */
    /* power to be used in the BSS by the AP.                                */
    /* TBD,  */
    *(puc_buffer + MAC_IE_HDR_LEN) = 0;
    *puc_ie_len = MAC_IE_HDR_LEN + MAC_PWR_CONSTRAINT_LEN;
}


void mac_set_quiet_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t uc_qcount,
                              uint8_t uc_qperiod, uint16_t us_qduration, uint16_t us_qoffset,
                              uint8_t *puc_ie_len)
{
    /* 管制域相关 tbd, 需要11h特性进一步分析此ie的设置 */
    mac_quiet_ie_stru   *pst_quiet = OAL_PTR_NULL;
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;

    if ((OAL_TRUE != mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap)) &&
        (OAL_TRUE != mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap))) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
    -----------------------------------------------------------------------------
    |ElementID | Length | QuietCount | QuietPeriod | QuietDuration | QuietOffset|
    -----------------------------------------------------------------------------
    |1         | 1      | 1          | 1           | 2             | 2          |
    -----------------------------------------------------------------------------
    ***************************************************************************/
    if (us_qduration == 0 || uc_qcount == 0) {
        *puc_ie_len = 0;
        return;
    }

    *puc_buffer = MAC_EID_QUIET;

    *(puc_buffer + 1) = MAC_QUIET_IE_LEN;

    pst_quiet = (mac_quiet_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    pst_quiet->quiet_count = uc_qcount;
    pst_quiet->quiet_period = uc_qperiod;
    pst_quiet->quiet_duration = oal_byteorder_to_le16(us_qduration);
    pst_quiet->quiet_offset = oal_byteorder_to_le16(us_qoffset);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_QUIET_IE_LEN;
}


void mac_set_tpc_report_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /***************************************************************************
                -------------------------------------------------
                |ElementID  |Length  |TransmitPower  |LinkMargin|
                -------------------------------------------------
       Octets:  |1          |1       |1              |1         |
                -------------------------------------------------

    TransimitPower, 此帧的传送功率，以dBm为单位
    ***************************************************************************/
    if (OAL_FALSE == mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap) &&
        OAL_FALSE == mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    *puc_buffer = MAC_EID_TPCREP;
    *(puc_buffer + 1) = MAC_TPCREP_IE_LEN;
    *(puc_buffer + 2) = pst_mac_vap->uc_tx_power;
    *(puc_buffer + 3) = 0; /* 此字段管理帧中不用 */

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_TPCREP_IE_LEN;
}


void mac_set_erp_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_erp_params_stru *pst_erp_params = OAL_PTR_NULL;

    /***************************************************************************
    --------------------------------------------------------------------------
    |EID  |Len  |NonERP_Present|Use_Protection|Barker_Preamble_Mode|Reserved|
    --------------------------------------------------------------------------
    |B0-B7|B0-B7|B0            |B1            |B2                  |B3-B7   |
    --------------------------------------------------------------------------
    ***************************************************************************/
    if ((pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) || (pst_mac_vap->en_protocol == WLAN_LEGACY_11B_MODE)) {
        *puc_ie_len = 0;
        return; /* 5G频段和11b协议模式 没有erp信息 */
    }

    *puc_buffer = MAC_EID_ERP;
    *(puc_buffer + 1) = MAC_ERP_IE_LEN;
    *(puc_buffer + 2) = 0; /* 初始清0 */

    pst_erp_params = (mac_erp_params_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    /* 如果存在non erp站点与ap关联， 或者obss中存在non erp站点 */
    if ((pst_mac_vap->st_protection.uc_sta_non_erp_num != 0) ||
        (pst_mac_vap->st_protection.bit_obss_non_erp_present == OAL_TRUE)) {
        pst_erp_params->bit_non_erp = 1;
    } else {
        pst_erp_params->bit_non_erp = 0;
    }

    /* 如果ap已经启用erp保护 */
    if (pst_mac_vap->st_protection.en_protection_mode == WLAN_PROT_ERP) {
        pst_erp_params->bit_use_protection = 1;
    } else {
        pst_erp_params->bit_use_protection = 0;
    }

    /* 如果存在不支持short preamble的站点与ap关联， 或者ap自身不支持short preamble */
    if ((pst_mac_vap->st_protection.uc_sta_no_short_preamble_num != 0) ||
        (OAL_FALSE == mac_mib_get_ShortPreambleOptionImplemented(pst_mac_vap))) {
        pst_erp_params->bit_preamble_mode = 1;
    }

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_ERP_IE_LEN;
}


void mac_set_rsn_ie(void *pst_vap, uint8_t *puc_pmkid, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_rsn_cap_stru    *pst_rsn_cap = OAL_PTR_NULL;
    uint8_t            uc_index;
    uint8_t            uc_pair_suites_num;
    uint8_t            uc_akm_suites_num;
    uint8_t            uc_loop = 0;
    uint32_t           ul_group_suit;
    uint32_t           ul_group_mgmt_suit;
    uint32_t           aul_pcip[WLAN_PAIRWISE_CIPHER_SUITES] = {0};
    uint32_t           aul_akm[WLAN_AUTHENTICATION_SUITES] = {0};
    int32_t            l_ret;

    if (OAL_TRUE != mac_mib_get_rsnaactivated(pst_mac_vap) || (pst_mac_vap->st_cap_flag.bit_wpa2 != OAL_TRUE)) {
        *puc_ie_len = 0;
        return;
    }

    ul_group_suit = mac_mib_get_rsn_group_suite(pst_mac_vap);
    ul_group_mgmt_suit = mac_mib_get_rsn_group_mgmt_suite(pst_mac_vap);
    uc_pair_suites_num = mac_mib_get_rsn_pair_suites_s(pst_mac_vap, aul_pcip, sizeof(aul_pcip));
    uc_akm_suites_num = mac_mib_get_rsn_akm_suites_s(pst_mac_vap, aul_akm, sizeof(aul_akm));

    if ((uc_pair_suites_num == 0) || (uc_akm_suites_num == 0)) {
        *puc_ie_len = 0;
        return;
    }

    /**************************************************************************/
    /*                  RSN Element Format                                    */
    /* ---------------------------------------------------------------------- */
    /* |Element ID | Length | Version | Group Cipher Suite | Pairwise Cipher  */
    /* ---------------------------------------------------------------------- */
    /* | 1         | 1      | 2       |      4             |     2            */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* Suite | Pairwise Cipher Suite List | AKM Suite Count | AKM Suite List  */
    /* ---------------------------------------------------------------------- */
    /*       | 4-m                        |     2          | 4-n              */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* |RSN Capabilities|PMKID Count|PMKID List|Group Management Cipher Suite */
    /* ---------------------------------------------------------------------- */
    /* |    2           |    2      |16 -s     |         4                 |  */
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /**************************************************************************/
    uc_index = MAC_IE_HDR_LEN;

    /* 设置RSN ie的EID */
    puc_buffer[0] = MAC_EID_RSN;

    /* 设置version字段 */
    puc_buffer[uc_index++] = MAC_RSN_IE_VERSION;
    puc_buffer[uc_index++] = 0;

    l_ret = memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                     (uint8_t *)(&ul_group_suit), OAL_SIZEOF(uint32_t));
    uc_index += 4;

    /* 设置成对加密套件 */
    puc_buffer[uc_index++] = uc_pair_suites_num;
    puc_buffer[uc_index++] = 0;

    for (uc_loop = 0; uc_loop < uc_pair_suites_num; uc_loop++) {
        l_ret += memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                          (uint8_t *)(&aul_pcip[uc_loop]), OAL_SIZEOF(uint32_t));
        uc_index += 4;
    }

    /* 设置认证套件数 */
    puc_buffer[uc_index++] = uc_akm_suites_num;
    puc_buffer[uc_index++] = 0;

    /* 根据MIB 值，设置认证套件内容 */
    for (uc_loop = 0; uc_loop < uc_akm_suites_num; uc_loop++) {
        l_ret += memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                          (uint8_t *)(&aul_akm[uc_loop]), OAL_SIZEOF(uint32_t));
        uc_index += 4;
    }

    /* 设置 RSN Capabilities字段 */
    /*************************************************************************/
    /* --------------------------------------------------------------------- */
    /* | B15 - B6  |  B5 - B4      | B3 - B2     |       B1    |     B0    | */
    /* --------------------------------------------------------------------- */
    /* | Reserved  |  GTSKA Replay | PTSKA Replay| No Pairwise | Pre - Auth| */
    /* |           |    Counter    |   Counter   |             |           | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* 设置RSN Capabilities 值，包括Pre_Auth, no_pairwise,  */
    /* Replay counters (PTKSA and GTKSA)                    */
    /* WPA 不需要填写RSN Capabilities 字段                  */
    pst_rsn_cap = (mac_rsn_cap_stru *)(puc_buffer + uc_index);
    memset_s(pst_rsn_cap, OAL_SIZEOF(mac_rsn_cap_stru), 0, OAL_SIZEOF(mac_rsn_cap_stru));
    uc_index += MAC_RSN_CAP_LEN;

    pst_rsn_cap->bit_mfpr = mac_mib_get_dot11RSNAMFPR(pst_mac_vap);
    pst_rsn_cap->bit_mfpc = mac_mib_get_dot11RSNAMFPC(pst_mac_vap);
    pst_rsn_cap->bit_pre_auth = mac_mib_get_pre_auth_actived(pst_mac_vap);
    pst_rsn_cap->bit_no_pairwise = 0;
    pst_rsn_cap->bit_ptska_relay_counter = mac_mib_get_rsnacfg_ptksareplaycounters(pst_mac_vap);
    pst_rsn_cap->bit_gtska_relay_counter = mac_mib_get_rsnacfg_gtksareplaycounters(pst_mac_vap);

    /* 设置 PMKID 信息 */
    if (puc_pmkid != OAL_PTR_NULL) {
        puc_buffer[uc_index++] = 0x01;
        puc_buffer[uc_index++] = 0x00;
        l_ret += memcpy_s(&(puc_buffer[uc_index]), WLAN_PMKID_LEN, puc_pmkid, WLAN_PMKID_LEN);
        uc_index += WLAN_PMKID_LEN;
    }

    if ((OAL_TRUE == mac_mib_get_dot11RSNAMFPC((mac_vap_stru *)pst_mac_vap) && ul_group_mgmt_suit)) {
        /* 如果已经填过pmkid信息，不需要再填，否则需要填写一个空的PMKID */
        if (puc_pmkid == OAL_PTR_NULL) {
            puc_buffer[uc_index++] = 0x00;
            puc_buffer[uc_index++] = 0x00;
        }
        l_ret += memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                          (uint8_t *)(&ul_group_mgmt_suit), OAL_SIZEOF(uint32_t));
        uc_index += 4;
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_rsn_ie::memcpy fail!");
        return;
    }

    /* 设置RSN element的长度 */
    puc_buffer[1] = uc_index - MAC_IE_HDR_LEN;

    *puc_ie_len = uc_index;
}


void mac_set_wpa_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru    *pst_mac_vap = (mac_vap_stru *)pst_vap;
    uint8_t        uc_index;
    uint8_t        uc_pair_suites_num;
    uint8_t        uc_akm_suites_num;
    uint8_t        uc_loop = 0;
    uint32_t       ul_group_suit;
    uint32_t       aul_pcip[WLAN_PAIRWISE_CIPHER_SUITES] = {0};
    uint32_t       aul_akm[WLAN_AUTHENTICATION_SUITES] = {0};
    int32_t        l_ret;

    if (OAL_TRUE != mac_mib_get_rsnaactivated(pst_mac_vap) || (pst_mac_vap->st_cap_flag.bit_wpa != OAL_TRUE)) {
        *puc_ie_len = 0;
        return;
    }

    ul_group_suit = mac_mib_get_wpa_group_suite(pst_mac_vap);
    uc_pair_suites_num = mac_mib_get_wpa_pair_suites_s(pst_mac_vap, aul_pcip, sizeof(aul_pcip));
    uc_akm_suites_num = mac_mib_get_wpa_akm_suites_s(pst_mac_vap, aul_akm, sizeof(aul_akm));

    if ((uc_pair_suites_num == 0) || (uc_akm_suites_num == 0)) {
        *puc_ie_len = 0;
        return;
    }

    /**************************************************************************/
    /*                  RSN Element Format                                    */
    /* ---------------------------------------------------------------------- */
    /* |Element ID | Length | Version | Group Cipher Suite | Pairwise Cipher  */
    /* ---------------------------------------------------------------------- */
    /* | 1         | 1      | 2       |      4             |     2            */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* Suite | Pairwise Cipher Suite List | AKM Suite Count | AKM Suite List  */
    /* ---------------------------------------------------------------------- */
    /*       | 4-m                        |     2          | 4-n              */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* |RSN Capabilities|PMKID Count|PMKID List|Group Management Cipher Suite */
    /* ---------------------------------------------------------------------- */
    /* |    2           |    2      |16 -s     |         4                 |  */
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /**************************************************************************/
    uc_index = MAC_IE_HDR_LEN;

    /* 设置RSN ie的EID */
    puc_buffer[0] = MAC_EID_WPA;

    l_ret = memcpy_s(puc_buffer + uc_index, MAC_OUI_LEN, g_auc_wpa_oui, MAC_OUI_LEN);

    uc_index += MAC_OUI_LEN;

    puc_buffer[uc_index++] = MAC_OUITYPE_WPA; /* 填充WPA 的OUI 类型 */

    /* 设置version字段 */
    puc_buffer[uc_index++] = MAC_RSN_IE_VERSION;
    puc_buffer[uc_index++] = 0;

    /* 设置Group Cipher Suite */
    /*************************************************************************/
    /*                  Group Cipher Suite                                   */
    /* --------------------------------------------------------------------- */
    /*                  | OUI | Suite type |                                 */
    /* --------------------------------------------------------------------- */
    /*          Octets: |  3  |     1      |                                 */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    l_ret += memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                      (uint8_t *)(&ul_group_suit), OAL_SIZEOF(uint32_t));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_wpa_ie::memcpy fail!");
        return;
    }

    uc_index += 4;

    /* 设置成对加密套件 */
    puc_buffer[uc_index++] = uc_pair_suites_num;
    puc_buffer[uc_index++] = 0;

    for (uc_loop = 0; uc_loop < uc_pair_suites_num; uc_loop++) {
        if (EOK != memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                            (uint8_t *)(&aul_pcip[uc_loop]), OAL_SIZEOF(uint32_t))) {
            oam_error_log0(0, OAM_SF_ANY, "mac_set_wpa_ie::memcpy fail!");
            return;
        }
        uc_index += 4;
    }

    /* 设置认证套件数 */
    puc_buffer[uc_index++] = uc_akm_suites_num;
    puc_buffer[uc_index++] = 0;

    for (uc_loop = 0; uc_loop < uc_akm_suites_num; uc_loop++) {
        if (EOK != memcpy_s(&puc_buffer[uc_index], OAL_SIZEOF(uint32_t),
                            (uint8_t *)(&aul_akm[uc_loop]), OAL_SIZEOF(uint32_t))) {
            oam_error_log0(0, OAM_SF_ANY, "mac_set_wpa_ie::memcpy fail!");
            return;
        }
        uc_index += 4;
    }

    /* 设置wpa element的长度 */
    puc_buffer[1] = uc_index - MAC_IE_HDR_LEN;

    *puc_ie_len = uc_index;
}

uint8_t mac_get_uapsd_config_max_sp_len(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_sta_uapsd_cfg.uc_max_sp_len;
}

uint8_t mac_get_uapsd_config_ac(mac_vap_stru *pst_mac_vap, uint8_t uc_ac)
{
    if (uc_ac < WLAN_WME_AC_BUTT) {
        return pst_mac_vap->st_sta_uapsd_cfg.uc_trigger_enabled[uc_ac];
    }

    return 0;
}

void mac_set_qos_info_wmm_sta(mac_vap_stru *pst_mac_vap, uint8_t *puc_buffer)
{
    uint8_t uc_qos_info = 0;
    uint8_t uc_max_sp_bits;
    uint8_t uc_max_sp_length;

    /* QoS Information field                                          */
    /* -------------------------------------------------------------- */
    /* | B0    | B1    | B2    | B3    | B4      | B5:B6 | B7       | */
    /* -------------------------------------------------------------- */
    /* | AC_VO | AC_VI | AC_BK | AC_BE |         | Max SP|          | */
    /* | U-APSD| U-APSD| U-APSD| U-APSD| Reserved| Length| Reserved | */
    /* | Flag  | Flag  | Flag  | Flag  |         |       |          | */
    /* -------------------------------------------------------------- */
    /* Set the UAPSD configuration information in the QoS info field if the  */
    /* BSS type is Infrastructure and the AP supports UAPSD.                 */
    if (pst_mac_vap->uc_uapsd_cap == OAL_TRUE) {
        uc_max_sp_length = mac_get_uapsd_config_max_sp_len(pst_mac_vap);
        /*lint -e734*/
        uc_qos_info |= (mac_get_uapsd_config_ac(pst_mac_vap, WLAN_WME_AC_VO) << 0);
        uc_qos_info |= (mac_get_uapsd_config_ac(pst_mac_vap, WLAN_WME_AC_VI) << 1);
        uc_qos_info |= (mac_get_uapsd_config_ac(pst_mac_vap, WLAN_WME_AC_BK) << 2);
        uc_qos_info |= (mac_get_uapsd_config_ac(pst_mac_vap, WLAN_WME_AC_BE) << 3);
        /*lint +e734*/
        if (uc_max_sp_length <= 6) {
            uc_max_sp_bits = uc_max_sp_length >> 1;

            uc_qos_info |= ((uc_max_sp_bits & 0x03) << 5);
        }
    }

    puc_buffer[0] = uc_qos_info;
}

void mac_set_qos_info_field(mac_vap_stru *pst_mac_vap, uint8_t *puc_buffer)
{
    mac_qos_info_stru *pst_qos_info = (mac_qos_info_stru *)puc_buffer;

    /* QoS Information field  (AP MODE)            */
    /* ------------------------------------------- */
    /* | B0:B3               | B4:B6    | B7     | */
    /* ------------------------------------------- */
    /* | Parameter Set Count | Reserved | U-APSD | */

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_qos_info->bit_params_count = pst_mac_vap->uc_wmm_params_update_count;
        pst_qos_info->bit_uapsd = pst_mac_vap->st_cap_flag.bit_uapsd;
        pst_qos_info->bit_resv = 0;
    }

    /* QoS Information field  (STA MODE)           */
    /* ---------------------------------------------------------------------------------------------------------- */
    /* | B0              | B1              | B2              | B3              | B4      |B5   B6      | B7     | */
    /* ---------------------------------------------------------------------------------------------------------- */
    /* |AC_VO U-APSD Flag|AC_VI U-APSD Flag|AC_BK U-APSD Flag|AC_BE U-APSD Flag|Reserved |Max SP Length|Reserved| */
    /* ---------------------------------------------------------------------------------------------------------- */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        mac_set_qos_info_wmm_sta(pst_mac_vap, puc_buffer);
    }
}


void mac_set_wmm_ac_params(mac_vap_stru *pst_mac_vap, uint8_t *puc_buffer, wlan_wme_ac_type_enum_uint8 en_ac)
{
    mac_wmm_ac_params_stru *pst_ac_params = (mac_wmm_ac_params_stru *)puc_buffer;

    /* AC_** Parameter Record field               */
    /* ------------------------------------------ */
    /* | Byte 1    | Byte 2        | Byte 3:4   | */
    /* ------------------------------------------ */
    /* | ACI/AIFSN | ECWmin/ECWmax | TXOP Limit | */
    /* ------------------------------------------ */
    /* ACI/AIFSN Field                    */
    /* ---------------------------------- */
    /* | B0:B3 | B4  | B5:B6 | B7       | */
    /* ---------------------------------- */
    /* | AIFSN | ACM | ACI   | Reserved | */
    /* ---------------------------------- */
    /* AIFSN */
    pst_ac_params->bit_aifsn = mac_mib_get_EDCATableAIFSN(pst_mac_vap, en_ac);

    /* ACM */
    pst_ac_params->bit_acm = mac_mib_get_EDCATableMandatory(pst_mac_vap, en_ac);

    /* ACI */
    pst_ac_params->bit_aci = mac_mib_get_EDCATableIndex(pst_mac_vap, en_ac) - 1;

    pst_ac_params->bit_resv = 0;

    /* ECWmin/ECWmax Field */
    /* ------------------- */
    /* | B0:B3  | B4:B7  | */
    /* ------------------- */
    /* | ECWmin | ECWmax | */
    /* ------------------- */
    /* ECWmin */
    pst_ac_params->bit_ecwmin = mac_mib_get_EDCATableCWmin(pst_mac_vap, en_ac);

    /* ECWmax */
    pst_ac_params->bit_ecwmax = mac_mib_get_EDCATableCWmax(pst_mac_vap, en_ac);

    /* TXOP Limit. The value saved in MIB is in usec while the value to be   */
    /* set in this element should be in multiple of 32us                     */
    pst_ac_params->us_txop = (uint16_t)(mac_mib_get_EDCATableTXOPLimit(pst_mac_vap, en_ac) >> 5);
}


void mac_set_wmm_params_ie(void *pst_vap, uint8_t *puc_buffer,
                                   oal_bool_enum_uint8 en_is_qos, uint8_t *puc_ie_len)
{
    uint8_t       uc_index;
    mac_vap_stru   *pst_mac_vap = (mac_vap_stru *)pst_vap;
    int32_t       l_ret;

    if (en_is_qos == OAL_FALSE) {
        *puc_ie_len = 0;
        return;
    }

    /* WMM Parameter Element Format                                          */
    /* --------------------------------------------------------------------- */
    /* | 3Byte | 1        | 1           | 1             | 1        | 1     | */
    /* --------------------------------------------------------------------- */
    /* | OUI   | OUI Type | OUI Subtype | Version field | QoS Info | Resvd | */
    /* --------------------------------------------------------------------- */
    /* | 4              | 4              | 4              | 4              | */
    /* --------------------------------------------------------------------- */
    /* | AC_BE ParamRec | AC_BK ParamRec | AC_VI ParamRec | AC_VO ParamRec | */
    /* --------------------------------------------------------------------- */
    puc_buffer[0] = MAC_EID_WMM;
    puc_buffer[1] = MAC_WMM_PARAM_LEN;

    uc_index = MAC_IE_HDR_LEN;

    /* OUI */
    l_ret = memcpy_s(&puc_buffer[uc_index], MAC_OUI_LEN, g_auc_wmm_oui, MAC_OUI_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_wmm_params_ie::memcpy fail!");
        return;
    }

    uc_index += MAC_OUI_LEN;

    /* OUI Type */
    puc_buffer[uc_index++] = MAC_OUITYPE_WMM;

    /* OUI Subtype */
    puc_buffer[uc_index++] = MAC_OUISUBTYPE_WMM_PARAM;

    /* Version field */
    puc_buffer[uc_index++] = MAC_OUI_WMM_VERSION;

    /* QoS Information Field */
    mac_set_qos_info_field(pst_mac_vap, &puc_buffer[uc_index]);
    uc_index += MAC_QOS_INFO_LEN;

    /* Reserved */
    puc_buffer[uc_index++] = 0;

    /* Set the AC_BE, AC_BK, AC_VI, AC_VO Parameter Record fields */
    mac_set_wmm_ac_params(pst_mac_vap, &puc_buffer[uc_index], WLAN_WME_AC_BE);
    uc_index += MAC_AC_PARAM_LEN;

    mac_set_wmm_ac_params(pst_mac_vap, &puc_buffer[uc_index], WLAN_WME_AC_BK);
    uc_index += MAC_AC_PARAM_LEN;

    mac_set_wmm_ac_params(pst_mac_vap, &puc_buffer[uc_index], WLAN_WME_AC_VI);
    uc_index += MAC_AC_PARAM_LEN;

    mac_set_wmm_ac_params(pst_mac_vap, &puc_buffer[uc_index], WLAN_WME_AC_VO);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_WMM_PARAM_LEN;
}


void mac_set_exsup_rates_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_rateset_stru    *pst_rates_set;
    uint8_t            uc_nrates;
    uint8_t            uc_idx;

    pst_rates_set = &(pst_mac_vap->st_curr_sup_rates.st_rate);

    /* STA全信道扫描时根据频段设置supported rates */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA && ((pst_mac_vap->en_protocol == WLAN_VHT_MODE) ||
        (g_wlan_spec_cfg->feature_11ax_is_open && (pst_mac_vap->en_protocol == WLAN_HE_MODE)))) {
        pst_rates_set = &(pst_mac_vap->ast_sta_sup_rates_ie[pst_mac_vap->st_channel.en_band].st_rate);
    }

    /***************************************************************************
                   -----------------------------------------------
                   |ElementID | Length | Extended Supported Rates|
                   -----------------------------------------------
       Octets:     |1         | 1      | 1-255                   |
                   -----------------------------------------------
    ***************************************************************************/
    if (pst_rates_set->uc_rs_nrates <= MAC_MAX_SUPRATES) {
        *puc_ie_len = 0;

        return;
    }

    puc_buffer[0] = MAC_EID_XRATES;
    uc_nrates = pst_rates_set->uc_rs_nrates - MAC_MAX_SUPRATES;
    puc_buffer[1] = uc_nrates;

    for (uc_idx = 0; uc_idx < uc_nrates; uc_idx++) {
        puc_buffer[MAC_IE_HDR_LEN + uc_idx] = pst_rates_set->ast_rs_rates[uc_idx + MAC_MAX_SUPRATES].uc_mac_rate;
    }

    *puc_ie_len = MAC_IE_HDR_LEN + uc_nrates;
}
#ifdef _PRE_WLAN_FEATURE_SMPS

uint8_t mac_calc_smps_field(uint8_t en_smps)
{
    if (en_smps == WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC) {
        return MAC_SMPS_DYNAMIC_MODE;
    } else if (en_smps == WLAN_MIB_MIMO_POWER_SAVE_STATIC) {
        return MAC_SMPS_STATIC_MODE;
    }

    return MAC_SMPS_MIMO_MODE;
}
#endif


void mac_set_ht_capinfo_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;

    mac_frame_ht_cap_stru   *pst_ht_capinfo = (mac_frame_ht_cap_stru *)puc_buffer;
    /*********************** HT Capabilities Info field*************************
     ----------------------------------------------------------------------------
     |-------------------------------------------------------------------|
     | LDPC   | Supp    | SM    | Green- | Short  | Short  |  Tx  |  Rx  |
     | Coding | Channel | Power | field  | GI for | GI for | STBC | STBC |
     | Cap    | Wth Set | Save  |        | 20 MHz | 40 MHz |      |      |
     |-------------------------------------------------------------------|
     |   B0   |    B1   |B2   B3|   B4   |   B5   |    B6  |  B7  |B8  B9|
     |-------------------------------------------------------------------|
     |-------------------------------------------------------------------|
     |    HT     |  Max   | DSS/CCK | Reserved | 40 MHz     | L-SIG TXOP |
     |  Delayed  | AMSDU  | Mode in |          | Intolerant | Protection |
     | Block-Ack | Length | 40MHz   |          |            | Support    |
     |-------------------------------------------------------------------|
     |    B10    |   B11  |   B12   |   B13    |    B14     |    B15     |
     |-------------------------------------------------------------------|
     ***************************************************************************/
    /* 初始清0 */
    puc_buffer[0] = 0;
    puc_buffer[1] = 0;

    pst_ht_capinfo->bit_ldpc_coding_cap = mac_mib_get_LDPCCodingOptionImplemented(pst_mac_vap);

    /* 设置所支持的信道宽度集"，0:仅20MHz运行; 1:20MHz与40MHz运行 */
    pst_ht_capinfo->bit_supported_channel_width = mac_mib_get_FortyMHzOperationImplemented(pst_mac_vap);

    /* 只有支持40M的情况下，才可以宣称支持40M short GI */
    if (pst_ht_capinfo->bit_supported_channel_width) {
        pst_ht_capinfo->bit_short_gi_40mhz = mac_mib_get_ShortGIOptionInFortyImplemented(pst_mac_vap);
    } else {
        pst_ht_capinfo->bit_short_gi_40mhz = 0;
    }

#ifdef _PRE_WLAN_FEATURE_SMPS
    pst_ht_capinfo->bit_sm_power_save = mac_calc_smps_field(mac_mib_get_smps(pst_mac_vap));
#else
    pst_ht_capinfo->bit_sm_power_save = MAC_SMPS_MIMO_MODE;
#endif

    pst_ht_capinfo->bit_ht_green_field = mac_mib_get_HTGreenfieldOptionImplemented(pst_mac_vap);

    pst_ht_capinfo->bit_short_gi_20mhz = mac_mib_get_ShortGIOptionInTwentyImplemented(pst_mac_vap);
    pst_ht_capinfo->bit_tx_stbc = mac_mib_get_TxSTBCOptionImplemented(pst_mac_vap);
    pst_ht_capinfo->bit_rx_stbc = mac_mib_get_RxSTBCOptionImplemented(pst_mac_vap);

    pst_ht_capinfo->bit_ht_delayed_block_ack = mac_mib_get_dot11DelayedBlockAckOptionImplemented(pst_mac_vap);

    pst_ht_capinfo->bit_max_amsdu_length = mac_mib_get_max_amsdu_length(pst_mac_vap);

    /* 是否在具有40MHz能力，而运行于20/40MHz模式的BSS上使用DSSS/CCK */
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        if ((pst_mac_vap->en_protocol == WLAN_LEGACY_11B_MODE) ||
            (pst_mac_vap->en_protocol == WLAN_MIXED_ONE_11G_MODE) ||
            (pst_mac_vap->en_protocol == WLAN_MIXED_TWO_11G_MODE) ||
            (pst_mac_vap->en_protocol == WLAN_HT_MODE)) {
            pst_ht_capinfo->bit_dsss_cck_mode_40mhz = pst_mac_vap->st_cap_flag.bit_dsss_cck_mode_40mhz;
        } else {
            pst_ht_capinfo->bit_dsss_cck_mode_40mhz = 0;
        }
    } else {
        pst_ht_capinfo->bit_dsss_cck_mode_40mhz = 0;
    }

    /* 设置"40MHz不容许"，只在2.4GHz下有效 */
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        pst_ht_capinfo->bit_forty_mhz_intolerant = mac_mib_get_FortyMHzIntolerant(pst_mac_vap);
    } else {
        /* 5G 40MHz不容忍设置为0 */
        pst_ht_capinfo->bit_forty_mhz_intolerant = OAL_FALSE;
    }

    pst_ht_capinfo->bit_lsig_txop_protection = mac_mib_get_LsigTxopFullProtectionActivated(pst_mac_vap);
}


void mac_set_ampdu_params_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ampdu_params_stru   *pst_ampdu_params = (mac_ampdu_params_stru *)puc_buffer;

    /******************** AMPDU Parameters Field ******************************
      |-----------------------------------------------------------------------|
      | Maximum AMPDU Length Exponent | Minimum MPDU Start Spacing | Reserved |
      |-----------------------------------------------------------------------|
      | B0                         B1 | B2                      B4 | B5     B7|
      |-----------------------------------------------------------------------|
     **************************************************************************/
    /* 初始清0 */
    puc_buffer[0] = 0;

    pst_ampdu_params->bit_max_ampdu_len_exponent = mac_mib_get_max_ampdu_len_exponent(pst_mac_vap);
    pst_ampdu_params->bit_min_mpdu_start_spacing = mac_mib_get_min_mpdu_start_spacing(pst_mac_vap);
}


void mac_set_sup_mcs_set_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_sup_mcs_set_stru    *pst_sup_mcs_set = (mac_sup_mcs_set_stru *)puc_buffer;

    /************************* Supported MCS Set Field **********************
    |-------------------------------------------------------------------|
    | Rx MCS Bitmask | Reserved | Rx Highest    | Reserved |  Tx MCS    |
    |                |          | Supp Data Rate|          |Set Defined |
    |-------------------------------------------------------------------|
    | B0         B76 | B77  B79 | B80       B89 | B90  B95 |    B96     |
    |-------------------------------------------------------------------|
    | Tx Rx MCS Set  | Tx Max Number     |   Tx Unequal     | Reserved  |
    |  Not Equal     | Spat Stream Supp  | Modulation Supp  |           |
    |-------------------------------------------------------------------|
    |      B97       | B98           B99 |       B100       | B101 B127 |
    |-------------------------------------------------------------------|
    *************************************************************************/
    /* 初始清零 */
    memset_s(puc_buffer, OAL_SIZEOF(mac_sup_mcs_set_stru), 0, OAL_SIZEOF(mac_sup_mcs_set_stru));

    if (EOK != memcpy_s(pst_sup_mcs_set->auc_rx_mcs, WLAN_HT_MCS_BITMASK_LEN,
                        mac_mib_get_SupportedMCSRx(pst_mac_vap), WLAN_HT_MCS_BITMASK_LEN)) {
        return;
    }

    pst_sup_mcs_set->bit_rx_highest_rate = mac_mib_get_HighestSupportedDataRate(pst_mac_vap);

    if (OAL_TRUE == mac_mib_get_TxMCSSetDefined(pst_mac_vap)) {
        pst_sup_mcs_set->bit_tx_mcs_set_def = 1;

        if (OAL_TRUE == mac_mib_get_TxRxMCSSetNotEqual(pst_mac_vap)) {
            pst_sup_mcs_set->bit_tx_rx_not_equal = 1;

            pst_sup_mcs_set->bit_tx_max_stream = mac_mib_get_TxMaximumNumberSpatialStreamsSupported(pst_mac_vap);
            if (OAL_TRUE == mac_mib_get_TxUnequalModulationSupported(pst_mac_vap)) {
                pst_sup_mcs_set->bit_tx_unequal_modu = 1;
            }
        }
    }

    /* reserve位清0 */
    pst_sup_mcs_set->bit_resv1 = 0;
    pst_sup_mcs_set->bit_resv2 = 0;
}

void mac_set_ht_extcap_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ext_cap_stru    *pst_ext_cap = (mac_ext_cap_stru *)puc_buffer;

    /***************** HT Extended Capabilities Field **********************
      |-----------------------------------------------------------------|
      | PCO | PCO Trans | Reserved | MCS  |  +HTC   |  RD    | Reserved |
      |     |   Time    |          | Fdbk | Support | Resp   |          |
      |-----------------------------------------------------------------|
      | B0  | B1     B2 | B3    B7 | B8 B9|   B10   |  B11   | B12  B15 |
      |-----------------------------------------------------------------|
    ***********************************************************************/
    /* 初始清0 */
    puc_buffer[0] = 0;
    puc_buffer[1] = 0;

    if (OAL_TRUE == mac_mib_get_pco_option_implemented(pst_mac_vap)) {
        pst_ext_cap->bit_pco = 1;
        pst_ext_cap->bit_pco_trans_time = mac_mib_get_transition_time(pst_mac_vap);
    }

    pst_ext_cap->bit_mcs_fdbk = mac_mib_get_mcs_fdbk(pst_mac_vap);

    pst_ext_cap->bit_htc_sup = mac_mib_get_htc_sup(pst_mac_vap);

    pst_ext_cap->bit_rd_resp = mac_mib_get_rd_rsp(pst_mac_vap);
}

void mac_set_txbf_cap_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_txbf_cap_stru   *pst_txbf_cap = (mac_txbf_cap_stru *)puc_buffer;
    /*************** Transmit Beamforming Capability Field *********************
     |-------------------------------------------------------------------------|
     |   Implicit | Rx Stagg | Tx Stagg  | Rx NDP   | Tx NDP   | Implicit      |
     |   TxBF Rx  | Sounding | Sounding  | Capable  | Capable  | TxBF          |
     |   Capable  | Capable  | Capable   |          |          | Capable       |
     |-------------------------------------------------------------------------|
     |      B0    |     B1   |    B2     |   B3     |   B4     |    B5         |
     |-------------------------------------------------------------------------|
     |              | Explicit | Explicit Non- | Explicit      | Explicit      |
     |  Calibration | CSI TxBF | Compr Steering| Compr steering| TxBF CSI      |
     |              | Capable  | Cap.          | Cap.          | Feedback      |
     |-------------------------------------------------------------------------|
     |  B6       B7 |   B8     |       B9      |       B10     | B11  B12      |
     |-------------------------------------------------------------------------|
     | Explicit Non- | Explicit | Minimal  | CSI Num of | Non-Compr Steering   |
     | Compr BF      | Compr BF | Grouping | Beamformer | Num of Beamformer    |
     | Fdbk Cap.     | Fdbk Cap.|          | Ants Supp  | Ants Supp            |
     |-------------------------------------------------------------------------|
     | B13       B14 | B15  B16 | B17  B18 | B19    B20 | B21        B22       |
     |-------------------------------------------------------------------------|
     | Compr Steering    | CSI Max Num of     |   Channel     |                |
     | Num of Beamformer | Rows Beamformer    | Estimation    | Reserved       |
     | Ants Supp         | Supported          | Capability    |                |
     |-------------------------------------------------------------------------|
     | B23           B24 | B25            B26 | B27       B28 | B29  B31       |
     |-------------------------------------------------------------------------|
    ***************************************************************************/
    /* 初始清零 */
    puc_buffer[0] = 0;
    puc_buffer[1] = 0;
    puc_buffer[2] = 0;
    puc_buffer[3] = 0;

    /* 指示STA是否可以接收staggered sounding帧 */
    pst_txbf_cap->bit_rx_stagg_sounding = mac_mib_get_ReceiveStaggerSoundingOptionImplemented(pst_mac_vap);

    /* 指示STA是否可以发送staggered sounding帧. */
    pst_txbf_cap->bit_tx_stagg_sounding = mac_mib_get_TransmitStaggerSoundingOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_rx_ndp = mac_mib_get_ReceiveNDPOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_tx_ndp = mac_mib_get_TransmitNDPOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_implicit_txbf = mac_mib_get_ImplicitTransmitBeamformingOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_calibration = mac_mib_get_CalibrationOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_explicit_csi_txbf = mac_mib_get_ExplicitCSITransmitBeamformingOptionImplemented(pst_mac_vap);

    pst_txbf_cap->bit_explicit_noncompr_steering =
        mac_mib_get_ExplicitNonCompressedBeamformingMatrixOptionImplemented(pst_mac_vap);

    /* Indicates if this STA can apply transmit beamforming using compressed */
    /* beamforming feedback matrix explicit feedback in its tranmission.     */
    /*************************************************************************/
    /*************************************************************************/
    /* No MIB exists, not clear what needs to be set    B10                  */
    /*************************************************************************/
    /*************************************************************************/
    /* Indicates if this receiver can return CSI explicit feedback */
    pst_txbf_cap->bit_explicit_txbf_csi_fdbk =
        mac_mib_get_ExplicitTransmitBeamformingCSIFeedbackOptionImplemented(pst_mac_vap);

    /* Indicates if this receiver can return non-compressed beamforming      */
    /* feedback matrix explicit feedback.                                    */
    pst_txbf_cap->bit_explicit_noncompr_bf_fdbk =
        mac_mib_get_ExplicitNonCompressedBeamformingFeedbackOptionImplemented(pst_mac_vap);

    /* Indicates if this STA can apply transmit beamforming using explicit   */
    /* compressed beamforming feedback matrix.                               */
    pst_txbf_cap->bit_explicit_compr_bf_fdbk =
        mac_mib_get_ExplicitCompressedBeamformingFeedbackOptionImplemented(pst_mac_vap);

    /* Indicates the minimal grouping used for explicit feedback reports */
    /*************************************************************************/
    /*************************************************************************/
    /*  No MIB exists, not clear what needs to be set       B17              */
    /*************************************************************************/
    /*************************************************************************/
    /* Indicates the maximum number of beamformer antennas the beamformee    */
    /* can support when CSI feedback is required.                            */
    pst_txbf_cap->bit_csi_num_bf_antssup = mac_mib_get_NumberBeamFormingCSISupportAntenna(pst_mac_vap);

    /* Indicates the maximum number of beamformer antennas the beamformee    */
    /* can support when non-compressed beamforming feedback matrix is        */
    /* required                                                              */
    pst_txbf_cap->bit_noncompr_steering_num_bf_antssup =
        mac_mib_get_NumberNonCompressedBeamformingMatrixSupportAntenna(pst_mac_vap);

    /* Indicates the maximum number of beamformer antennas the beamformee   */
    /* can support when compressed beamforming feedback matrix is required  */
    pst_txbf_cap->bit_compr_steering_num_bf_antssup =
        mac_mib_get_NumberCompressedBeamformingMatrixSupportAntenna(pst_mac_vap) - 1;

    /* Indicates the maximum number of rows of CSI explicit feedback from    */
    /* beamformee that the beamformer can support when CSI feedback is       */
    /* required                                                              */
    /*************************************************************************/
    /*************************************************************************/
    /*  No MIB exists, not clear what needs to be set     B25                */
    /*************************************************************************/
    /*************************************************************************/
    /* Indicates maximum number of space time streams (columns of the MIMO   */
    /* channel matrix) for which channel dimensions can be simultaneously    */
    /* estimated. When staggered sounding is supported this limit applies    */
    /* independently to both the data portion and to the extension portion   */
    /* of the long training fields.                                          */
    /*************************************************************************/
    /*************************************************************************/
    /*      No MIB exists, not clear what needs to be set          B27       */
    /*************************************************************************/
    /*************************************************************************/
#ifdef _PRE_WLAN_FEATURE_TXBF
    pst_txbf_cap->bit_explicit_compr_Steering = pst_mac_vap->st_txbf_add_cap.bit_exp_comp_txbf_cap;
    pst_txbf_cap->bit_chan_estimation = pst_mac_vap->st_txbf_add_cap.bit_channel_est_cap;
    pst_txbf_cap->bit_minimal_grouping = pst_mac_vap->st_txbf_add_cap.bit_min_grouping;
    pst_txbf_cap->bit_csi_maxnum_rows_bf_sup = pst_mac_vap->st_txbf_add_cap.bit_csi_bfee_max_rows;
    pst_txbf_cap->bit_implicit_txbf_rx = pst_mac_vap->st_txbf_add_cap.bit_imbf_receive_cap;
#endif
}

void mac_set_asel_cap_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_asel_cap_stru   *pst_asel_cap = (mac_asel_cap_stru *)puc_buffer;

    /************** Antenna Selection Capability Field *************************
     |-------------------------------------------------------------------|
     |  Antenna  | Explicit CSI  | Antenna Indices | Explicit | Antenna  |
     | Selection | Fdbk based TX | Fdbk based TX   | CSI Fdbk | Indices  |
     |  Capable  | ASEL Capable  | ASEL Capable    | Capable  | Fdbk Cap.|
     |-------------------------------------------------------------------|
     |    B0     |     B1        |      B2         |    B3    |    B4    |
     |-------------------------------------------------------------------|

     |------------------------------------|
     |  RX ASEL |   Transmit   |          |
     |  Capable |   Sounding   | Reserved |
     |          | PPDU Capable |          |
     |------------------------------------|
     |    B5    |     B6       |    B7    |
     |------------------------------------|
    ***************************************************************************/
    /* 初始清0 */
    puc_buffer[0] = 0;

    /* 指示STA是否支持天线选择 */
    pst_asel_cap->bit_asel = mac_mib_get_AntennaSelectionOptionImplemented(pst_mac_vap);

    /* 指示STA是否具有基于显示CSI(信道状态信息)反馈的发射天线选择能力 */
    pst_asel_cap->bit_explicit_sci_fdbk_tx_asel =
        mac_mib_get_TransmitExplicitCSIFeedbackASOptionImplemented(pst_mac_vap);

    /* 指示STA是否具有基于天线指数反馈的发射天线选择能力 */
    pst_asel_cap->bit_antenna_indices_fdbk_tx_asel =
        mac_mib_get_TransmitIndicesFeedbackASOptionImplemented(pst_mac_vap);

    /* 指示STA在天线选择的支持下是否能够计算CSI(信道状态信息)并提供CSI反馈 */
    pst_asel_cap->bit_explicit_csi_fdbk = mac_mib_get_ExplicitCSIFeedbackASOptionImplemented(pst_mac_vap);

    /* Indicates whether or not this STA can conduct antenna indices */
    /* selection computation and feedback the results in support of  */
    /* Antenna Selection. */
    pst_asel_cap->bit_antenna_indices_fdbk = mac_mib_get_TransmitExplicitCSIFeedbackASOptionImplemented(pst_mac_vap);

    /* 指示STA是否具有接收天线选择能力 */
    pst_asel_cap->bit_rx_asel = mac_mib_get_ReceiveAntennaSelectionOptionImplemented(pst_mac_vap);

    /* 指示STA是否能够在每一次请求中都可以为天线选择序列发送探测PPDU */
    pst_asel_cap->bit_trans_sounding_ppdu = mac_mib_get_TransmitSoundingPPDUOptionImplemented(pst_mac_vap);
}


void mac_set_timeout_interval_ie(void *pst_vap,
                                         uint8_t *puc_buffer,
                                         uint8_t *puc_ie_len,
                                         uint32_t ul_type,
                                         uint32_t ul_timeout)
{
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    mac_Timeout_Interval_type_enum en_tie_type;

    en_tie_type = (mac_Timeout_Interval_type_enum)ul_type;
    *puc_ie_len = 0;

    /* 判断是否需要设置timeout_interval IE */
    if (en_tie_type >= MAC_TIE_BUTT) {
        return;
    }

    /* Timeout Interval Parameter Element Format
    -----------------------------------------------------------------------
    |ElementID | Length | Timeout Interval Type| Timeout Interval Value  |
    -----------------------------------------------------------------------
    |1         | 1      | 1                    |  4                      |
    -----------------------------------------------------------------------
    */
    puc_buffer[0] = MAC_EID_TIMEOUT_INTERVAL;
    puc_buffer[1] = MAC_TIMEOUT_INTERVAL_INFO_LEN;
    puc_buffer[2] = en_tie_type;

    /* 设置Timeout Interval Value */
    puc_buffer[3] = ul_timeout & 0x000000FF;
    puc_buffer[4] = (ul_timeout & 0x0000FF00) >> 8;
    puc_buffer[5] = (ul_timeout & 0x00FF0000) >> 16;
    puc_buffer[6] = (ul_timeout & 0xFF000000) >> 24;

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_TIMEOUT_INTERVAL_INFO_LEN;
#else
    *puc_ie_len = 0;
#endif
    return;
}


void mac_set_ht_capabilities_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    if ((OAL_TRUE != mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) ||
        (((OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) ||
          (OAL_TRUE == mac_is_tkip_only(pst_mac_vap))) &&
         (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP))) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
    -------------------------------------------------------------------------
    |EID |Length |HT Capa. Info |A-MPDU Parameters |Supported MCS Set|
    -------------------------------------------------------------------------
    |1   |1      |2             |1                 |16               |
    -------------------------------------------------------------------------
    |HT Extended Cap. |Transmit Beamforming Cap. |ASEL Cap.          |
    -------------------------------------------------------------------------
    |2                |4                         |1                  |
    -------------------------------------------------------------------------
    ***************************************************************************/
    *puc_buffer = MAC_EID_HT_CAP;
    *(puc_buffer + 1) = MAC_HT_CAP_LEN;

    puc_buffer += MAC_IE_HDR_LEN;

    /* 填充ht capabilities information域信息 */
    mac_set_ht_capinfo_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HT_CAPINFO_LEN;

    /* 填充A-MPDU parameters域信息 */
    mac_set_ampdu_params_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HT_AMPDU_PARAMS_LEN;

    /* 填充supported MCS set域信息 */
    mac_set_sup_mcs_set_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HT_SUP_MCS_SET_LEN;

    /* 填充ht extended capabilities域信息 */
    mac_set_ht_extcap_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HT_EXT_CAP_LEN;

    /* 填充 transmit beamforming capabilities域信息 */
    mac_set_txbf_cap_field(pst_vap, puc_buffer);
    puc_buffer += MAC_HT_TXBF_CAP_LEN;

    /* 填充asel(antenna selection) capabilities域信息 */
    mac_set_asel_cap_field(pst_vap, puc_buffer);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_HT_CAP_LEN;
}


void mac_set_ht_opern_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru           *pst_mac_vap    = (mac_vap_stru *)pst_vap;
    mac_ht_opern_ac_stru   *pst_ht_opern   = OAL_PTR_NULL;
    uint8_t                 uc_obss_non_ht = 0;

    if ((OAL_TRUE != mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) ||
        (((OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) ||
          (OAL_TRUE == mac_is_tkip_only(pst_mac_vap))) &&
         (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP))) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
      ----------------------------------------------------------------------
      |EID |Length |PrimaryChannel |HT Operation Information |Basic MCS Set|
      ----------------------------------------------------------------------
      |1   |1      |1              |5                        |16           |
      ----------------------------------------------------------------------
    ***************************************************************************/
    /************************ HT Information Field ****************************
     |--------------------------------------------------------------------|
     | Primary | Seconday  | STA Ch | RIFS |           reserved           |
     | Channel | Ch Offset | Width  | Mode |             resv1            |
     |--------------------------------------------------------------------|
     |    1    | B0     B1 |   B2   |  B3  |    B4                     B7 |
     |--------------------------------------------------------------------|

     |--------------------------------------------------------------------|
     |     HT     | Non-GF STAs | reserved | OBSS Non-HT  | Channel Center|
     | Protection |   Present   |   resv2  | STAs Present | Freq Segment2 |
     |--------------------------------------------------------------------|
     | B8     B9  |     B10     |    B11   |     B12      | B13   B20     |
     |--------------------------------------------------------------------|

     |--------------------------------------------------------------|
     | Reserved  |  Dual  |  Dual CTS  | Seconday | LSIG TXOP Protn |
     |resv3 resv4| Beacon | Protection |  Beacon  | Full Support    |
     |--------------------------------------------------------------|
     |21-23 24-29|   B30  |    B31     |    B32   |       B33       |
     |--------------------------------------------------------------|

     |---------------------------|
     |  PCO   |  PCO  | Reserved |
     | Active | Phase |          |
     |---------------------------|
     |  B34   |  B35  | B36  B39 |
     |---------------------------|
    **************************************************************************/
    *puc_buffer = MAC_EID_HT_OPERATION;

    *(puc_buffer + 1) = MAC_HT_OPERN_LEN;
    pst_ht_opern = (mac_ht_opern_ac_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    /* 主信道编号 */
    pst_ht_opern->uc_primary_channel = pst_mac_vap->st_channel.uc_chan_number;
    pst_ht_opern->bit_chan_center_freq_seg2 = 0;

    /* 设置"次信道偏移量" */
    switch (pst_mac_vap->st_channel.en_bandwidth) {
        case WLAN_BAND_WIDTH_40PLUS:
        case WLAN_BAND_WIDTH_80PLUSPLUS:
        case WLAN_BAND_WIDTH_80PLUSMINUS:
            pst_ht_opern->bit_secondary_chan_offset = MAC_SCA;
            break;
        case WLAN_BAND_WIDTH_40MINUS:
        case WLAN_BAND_WIDTH_80MINUSPLUS:
        case WLAN_BAND_WIDTH_80MINUSMINUS:
            pst_ht_opern->bit_secondary_chan_offset = MAC_SCB;
            break;
#ifdef _PRE_WLAN_FEATURE_160M
        case WLAN_BAND_WIDTH_160PLUSPLUSPLUS:
        case WLAN_BAND_WIDTH_160PLUSPLUSMINUS:
        case WLAN_BAND_WIDTH_160PLUSMINUSPLUS:
        case WLAN_BAND_WIDTH_160PLUSMINUSMINUS:
            pst_ht_opern->bit_secondary_chan_offset = MAC_SCA;
            break;
        case WLAN_BAND_WIDTH_160MINUSPLUSPLUS:
        case WLAN_BAND_WIDTH_160MINUSPLUSMINUS:
        case WLAN_BAND_WIDTH_160MINUSMINUSPLUS:
        case WLAN_BAND_WIDTH_160MINUSMINUSMINUS:
            pst_ht_opern->bit_secondary_chan_offset = MAC_SCB;
            break;
#endif
        default:
            pst_ht_opern->bit_secondary_chan_offset = MAC_SCN;
            break;
    }

    /* 设置"STA信道宽度"，当BSS运行信道宽度 >= 40MHz时，需要将此field设置为1 */
    pst_ht_opern->bit_sta_chan_width = (pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_20M) ? 1 : 0;

    /* 指示基本服务集里是否允许使用减小的帧间距 */
    pst_ht_opern->bit_rifs_mode = mac_mib_get_RifsMode(pst_mac_vap);

    /* B4-B7保留 */
    pst_ht_opern->bit_resv1 = 0;

    /* 指示ht传输的保护要求 */
    pst_ht_opern->bit_HT_protection = mac_mib_get_HtProtection(pst_mac_vap);

    /* Non-GF STAs */
    pst_ht_opern->bit_nongf_sta_present = mac_mib_get_NonGFEntitiesPresent(pst_mac_vap);

    /* B3 resv */
    pst_ht_opern->bit_resv2 = 0;

    /* B4  obss_nonht_sta_present */
    if ((pst_mac_vap->st_protection.bit_obss_non_ht_present != 0) ||
        (pst_mac_vap->st_protection.uc_sta_non_ht_num != 0)) {
        uc_obss_non_ht = 1;
    }
    pst_ht_opern->bit_obss_nonht_sta_present = uc_obss_non_ht;

    /* B5-B15 保留 */
    pst_ht_opern->bit_resv3 = 0;
    pst_ht_opern->bit_resv4 = 0;

    /* B6  dual_beacon */
    pst_ht_opern->bit_dual_beacon = 0;

    /* Dual CTS protection */
    pst_ht_opern->bit_dual_cts_protection = mac_mib_get_DualCTSProtection(pst_mac_vap);

    /* secondary_beacon: Set to 0 in a primary beacon */
    pst_ht_opern->bit_secondary_beacon = 0;

    /* BSS support L-SIG TXOP Protection */
    pst_ht_opern->bit_lsig_txop_protection_full_support = mac_mib_get_LsigTxopFullProtectionActivated(pst_mac_vap);

    /* PCO active */
    pst_ht_opern->bit_pco_active = mac_mib_get_PCOActivated(pst_mac_vap);

    /* PCO phase */
    pst_ht_opern->bit_pco_phase = 0;

    /* B12-B15  保留 */
    pst_ht_opern->bit_resv5 = 0;

    /* Basic MCS Set: set all bit zero,Indicates the MCS values that are supported by all HT STAs in the BSS. */
    memset_s(pst_ht_opern->auc_basic_mcs_set, MAC_HT_BASIC_MCS_SET_LEN, 0, MAC_HT_BASIC_MCS_SET_LEN);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_HT_OPERN_LEN;
}


void mac_set_obss_scan_params(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru                *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_obss_scan_params_stru   *pst_obss_scan = OAL_PTR_NULL;
    uint32_t                   ul_ret;

    if (OAL_TRUE != mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    if ((pst_mac_vap->st_channel.en_band != WLAN_BAND_2G) ||
        (OAL_TRUE != mac_mib_get_FortyMHzOperationImplemented(pst_mac_vap))) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
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
    puc_buffer[0] = MAC_EID_OBSS_SCAN;
    puc_buffer[1] = MAC_OBSS_SCAN_IE_LEN;

    pst_obss_scan = (mac_obss_scan_params_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    ul_ret = mac_mib_get_OBSSScanPassiveDwell(pst_mac_vap);
    pst_obss_scan->us_passive_dwell = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_OBSSScanActiveDwell(pst_mac_vap);
    pst_obss_scan->us_active_dwell = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_BSSWidthTriggerScanInterval(pst_mac_vap);
    pst_obss_scan->us_scan_interval = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_OBSSScanPassiveTotalPerChannel(pst_mac_vap);
    pst_obss_scan->us_passive_total_per_chan = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_OBSSScanActiveTotalPerChannel(pst_mac_vap);
    pst_obss_scan->us_active_total_per_chan = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_BSSWidthChannelTransitionDelayFactor(pst_mac_vap);
    pst_obss_scan->us_transition_delay_factor = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    ul_ret = mac_mib_get_OBSSScanActivityThreshold(pst_mac_vap);
    pst_obss_scan->us_scan_activity_thresh = (uint16_t)(oal_byteorder_to_le32(ul_ret));

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_OBSS_SCAN_IE_LEN;
}


void mac_set_ext_capabilities_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_ext_cap_ie_stru *pst_ext_cap = OAL_PTR_NULL;

    if ((OAL_TRUE != mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) ||
        (((OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) ||
          (OAL_TRUE == mac_is_tkip_only(pst_mac_vap))) &&
         (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP))) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
                         ----------------------------------
                         |Element ID |Length |Capabilities|
                         ----------------------------------
          Octets:        |1          |1      |n           |
                         ----------------------------------
    -------------------------------------------------------------------------------------------------------------------
    |  B0       | B1 | B2             | B3   | B4   |  B5  |  B6    |  B7   | ...|  B38    |   B39      |...|  B62
    ----------------------------------------------------------------------------
    |20/40 coex |resv|extended channel| resv | PSMP | resv | S-PSMP | Event |    |TDLS Pro-  TDLS Channel     Operating
                                                                                             Switching        mode
    |mgmt supp  |    |switching       |      |      |      |        |       | ...| hibited | Prohibited |...| notify
    -------------------------------------------------------------------------------------------------------------------
    ***************************************************************************/
    puc_buffer[0] = MAC_EID_EXT_CAPS;
    puc_buffer[1] = MAC_XCAPS_EX_LEN;

    /* 初始清零 */
    memset_s(puc_buffer + MAC_IE_HDR_LEN, OAL_SIZEOF(mac_ext_cap_ie_stru), 0, OAL_SIZEOF(mac_ext_cap_ie_stru));

    pst_ext_cap = (mac_ext_cap_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    /* 设置20/40 BSS Coexistence Management Support fieid */
    if ((OAL_TRUE == mac_mib_get_2040BSSCoexistenceManagementSupport(pst_mac_vap)) &&
        (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) &&
        (OAL_TRUE == mac_mib_get_FortyMHzOperationImplemented(pst_mac_vap))) {
        pst_ext_cap->bit_2040_coexistence_mgmt = 1;
    }

    /* 设置TDLS prohibited */
    pst_ext_cap->bit_tdls_prhibited = pst_mac_vap->st_cap_flag.bit_tdls_prohibited;

    /* 设置TDLS channel switch prohibited */
    pst_ext_cap->bit_tdls_channel_switch_prhibited = pst_mac_vap->st_cap_flag.bit_tdls_channel_switch_prohibited;

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    /* 如果是11ac 站点 设置OPMODE NOTIFY标志 */
    if ((OAL_TRUE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) &&
        (((OAL_FALSE == mac_is_wep_enabled(pst_mac_vap)) &&
          (OAL_FALSE == mac_is_tkip_only(pst_mac_vap))) ||
         (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP))) {
        pst_ext_cap->bit_operating_mode_notification = mac_mib_get_OperatingModeNotificationImplemented(pst_mac_vap);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_HS20
    /*  如果支持Hotspot2.0的Interwoking标志  */
    pst_ext_cap->bit_interworking = 1;
#else
    pst_ext_cap->bit_interworking = 0;
#endif

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_XCAPS_EX_LEN;

    mac_set_ext_capabilities_ftm_twt(pst_mac_vap, puc_buffer, puc_ie_len);
}


void mac_set_vht_capinfo_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_vht_cap_info_stru   *pst_vht_capinfo = (mac_vht_cap_info_stru *)puc_buffer;
#ifdef _PRE_WLAN_FEATURE_M2S
    mac_user_stru           *pst_mac_user;
#endif
    /*********************** VHT 能力信息域 ************************************
    ----------------------------------------------------------------------------
     |-----------------------------------------------------------------------|
     | Max    | Supp    | RX   | Short GI| Short  | Tx   |  Rx  |  SU        |
     | MPDU   | Channel | LDPC | for 80  | GI for | STBC | STBC | Beamformer |
     | Length | Wth Set |      |         | 160MHz |      |      | Capable    |
     |-----------------------------------------------------------------------|
     | B0 B1  | B2 B3   | B4   |   B5    |   B6   |  B7  |B8 B10|   B11      |
     |-----------------------------------------------------------------------|
     |-----------------------------------------------------------------------|
     | SU         | Compressed   | Num of    | MU        | MU        | VHT   |
     | Beamformee | Steering num | Sounding  | Beamformer| Beamformee| TXOP  |
     | Capable    | of bf ant sup| Dimensions| Capable   | Capable   | PS    |
     |-----------------------------------------------------------------------|
     |    B12     | B13      B15 | B16    B18|   B19     |    B20    | B21   |
     |-----------------------------------------------------------------------|
     |-----------------------------------------------------------------------|
     | +HTC   | Max AMPDU| VHT Link  | Rx ANT     | Tx ANT     |  Extended   |
     | VHT    | Length   | Adaptation| Pattern    | Pattern    |  NSS BW     |
     | Capable| Exponent | Capable   | Consistency| Consistency|  Support    |
     |-----------------------------------------------------------------------|
     | B22    | B23  B25 | B26   B27 |   B28      |   B29      |  B30 B31    |
     |-----------------------------------------------------------------------|
    ***************************************************************************/
    pst_vht_capinfo->bit_max_mpdu_length = mac_mib_get_maxmpdu_length(pst_mac_vap);

    /* 设置"所支持的信道宽度集"，0:neither 160 nor 80+80:; 1:160MHz; 2:160/80+80MHz */
    pst_vht_capinfo->bit_supported_channel_width = mac_mib_get_VHTChannelWidthOptionImplemented(pst_mac_vap);

    pst_vht_capinfo->bit_rx_ldpc = mac_mib_get_VHTLDPCCodingOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_short_gi_80mhz = mac_mib_get_VHTShortGIOptionIn80Implemented(pst_mac_vap);
    pst_vht_capinfo->bit_short_gi_160mhz = mac_mib_get_VHTShortGIOptionIn160and80p80Implemented(pst_mac_vap);
    pst_vht_capinfo->bit_tx_stbc = mac_mib_get_VHTTxSTBCOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_rx_stbc = (OAL_TRUE == mac_mib_get_VHTRxSTBCOptionImplemented(pst_mac_vap)) ? 1 : 0;
    pst_vht_capinfo->bit_su_beamformer_cap = mac_mib_get_VHTSUBeamformerOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_su_beamformee_cap = mac_mib_get_VHTSUBeamformeeOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_num_bf_ant_supported = mac_mib_get_VHTBeamformeeNTxSupport(pst_mac_vap) - 1;

#ifdef _PRE_WLAN_FEATURE_M2S
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA && pst_mac_user != OAL_PTR_NULL &&
        (pst_mac_user->st_vht_hdl.bit_num_sounding_dim != 0)) {
         // 非认证模式学习能力，认证模式采用默认值
        if ((g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_FEATURE_CERTIFY_MODE)) == 0) {
            pst_vht_capinfo->bit_num_bf_ant_supported = oal_min(pst_vht_capinfo->bit_num_bf_ant_supported,
                                                                pst_mac_user->st_vht_hdl.bit_num_sounding_dim);
        }
    }
#endif

    /* sounding dim是bfer的能力 */
    pst_vht_capinfo->bit_num_sounding_dim = mac_mib_get_VHTNumberSoundingDimensions(pst_mac_vap);

    pst_vht_capinfo->bit_mu_beamformer_cap = mac_mib_get_VHTMUBeamformerOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_mu_beamformee_cap = mac_mib_get_VHTMUBeamformeeOptionImplemented(pst_mac_vap);
    pst_vht_capinfo->bit_vht_txop_ps = mac_mib_get_txopps(pst_mac_vap);
    pst_vht_capinfo->bit_htc_vht_capable = mac_mib_get_vht_ctrl_field_cap(pst_mac_vap);
    pst_vht_capinfo->bit_max_ampdu_len_exp = mac_mib_get_vht_max_rx_ampdu_factor(pst_mac_vap);

    pst_vht_capinfo->bit_vht_link_adaptation    = 0;
    pst_vht_capinfo->bit_rx_ant_pattern         = 0;   /* 在该关联中不改变天线模式，设为1,；改变则设为0 */
    pst_vht_capinfo->bit_tx_ant_pattern         = 0;   /* 在该关联中不改变天线模式，设为1,；改变则设为0 */
    pst_vht_capinfo->bit_extend_nss_bw_supp     = 0;

    // todo ::以下函数内容待进一步整合到该函数内
    mac_set_vht_capinfo_field_extend(pst_mac_vap, puc_buffer);
}


void mac_set_vht_supported_mcsset_field(void *pst_vap, uint8_t *puc_buffer)
{
    mac_vap_stru                *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_vht_sup_mcs_set_stru    *pst_vht_mcsset = (mac_vht_sup_mcs_set_stru *)puc_buffer;

    memset_s(pst_vht_mcsset, OAL_SIZEOF(mac_vht_sup_mcs_set_stru), 0, OAL_SIZEOF(mac_vht_sup_mcs_set_stru));
    /*********************** VHT 支持的MCS集 ************************************
    ----------------------------------------------------------------------------
     |-----------------------------------------------------------------------|
     | Rx MCS Map | Rx Highest Supported | Resv    | Tx MCS Map  |
     |            | Long gi Data Rate    |         |             |
     |-----------------------------------------------------------------------|
     | B0     B15 | B16              B28 | B29 B31 | B32     B47 |
     |-----------------------------------------------------------------------|
     |-----------------------------------------------------------------------|
     | Tx Highest Supported |VHT Extended   |  Resv2  |
     | Long gi Data Rate    |NSS BW Capable |         |
     |-----------------------------------------------------------------------|
     |  B48             B60 | B61           | B62-B63 |
     |-----------------------------------------------------------------------|
    ***************************************************************************/
    pst_vht_mcsset->bit_rx_mcs_map = mac_mib_get_vht_rx_mcs_map(pst_mac_vap);
    pst_vht_mcsset->bit_rx_highest_rate = mac_mib_get_us_rx_highest_rate(pst_mac_vap);
    pst_vht_mcsset->bit_tx_mcs_map = mac_mib_get_vht_tx_mcs_map(pst_mac_vap);
    pst_vht_mcsset->bit_tx_highest_rate = mac_mib_get_us_tx_highest_rate(pst_mac_vap);
    pst_vht_mcsset->bit_vht_extend_nss_bw_capable = mac_mib_get_dot11VHTExtendedNSSBWCapable(pst_mac_vap);

    /* 用于指示VHT  DL  MU MIMO场景下支持的最大Nsts,如果设置为0,那么由Beamformee  STS  Capability决定 */
    pst_vht_mcsset->bit_max_nsts = 0;
    /* resv清0 */
    pst_vht_mcsset->bit_resv2 = 0;
}


void mac_set_vht_capabilities_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;
    if ((OAL_TRUE != mac_mib_get_VHTOptionImplemented(pst_mac_vap)) ||
        (((OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) ||
          (OAL_TRUE == mac_is_tkip_only(pst_mac_vap))) &&
         (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP))
#ifdef _PRE_WLAN_FEATURE_11AC2G
        || ((pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_FALSE) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G))
#endif /* _PRE_WLAN_FEATURE_11AC2G */
        ) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
    -------------------------------------------------------------------------
    |EID |Length |VHT Capa. Info |VHT Supported MCS Set|
    -------------------------------------------------------------------------
    |1   |1      | 4             | 8                   |
    -------------------------------------------------------------------------
    ***************************************************************************/
    puc_buffer[0] = MAC_EID_VHT_CAP;
    puc_buffer[1] = MAC_VHT_CAP_IE_LEN;

    puc_buffer += MAC_IE_HDR_LEN;

    mac_set_vht_capinfo_field(pst_vap, puc_buffer);

    puc_buffer += MAC_VHT_CAP_INFO_FIELD_LEN;

    mac_set_vht_supported_mcsset_field(pst_vap, puc_buffer);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_VHT_CAP_IE_LEN;
}


void mac_set_vht_opern_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_vht_opern_stru  *pst_vht_opern = OAL_PTR_NULL;

    if ((OAL_TRUE != mac_mib_get_VHTOptionImplemented(pst_mac_vap)) ||
        (((OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) ||
          (OAL_TRUE == mac_is_tkip_only(pst_mac_vap))) &&
         (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP))
#ifdef _PRE_WLAN_FEATURE_11AC2G
        || ((pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_FALSE) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G))
#endif /* _PRE_WLAN_FEATURE_11AC2G */
        ) {
        *puc_ie_len = 0;
        return;
    }

    /***********************VHT Operation element*******************************
    -------------------------------------------------------------------------
            |EID |Length |VHT Opern Info |VHT Basic MCS Set|
    -------------------------------------------------------------------------
    Octes:  |1   |1      | 3             | 2               |
    -------------------------------------------------------------------------
    ***************************************************************************/
    puc_buffer[0] = MAC_EID_VHT_OPERN;
    puc_buffer[1] = MAC_VHT_INFO_IE_LEN;

    puc_buffer += MAC_IE_HDR_LEN;

    /**********************VHT Opern Info***************************************
    -------------------------------------------------------------------------
            | Channel Width | Channel Center | Channel Center |
            |               | Freq Seg0      | Freq Seg1      |
    -------------------------------------------------------------------------
    Octes:  |       1       |       1        |       1        |
    -------------------------------------------------------------------------
    ***************************************************************************/
    pst_vht_opern = (mac_vht_opern_stru *)puc_buffer;

    /* uc_channel_width的取值，0 -- 20/40M, 1 -- 80M, 2 -- 160M, 3 -- 80+80M */
    if ((pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS) &&
        (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS)) {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_80;
    }
#ifdef _PRE_WLAN_FEATURE_160M
    else if ((pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
             (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_80;
    }
#endif
    else {
        pst_vht_opern->uc_channel_width = WLAN_MIB_VHT_OP_WIDTH_20_40;
    }

    switch (pst_mac_vap->st_channel.en_bandwidth) {
#ifdef _PRE_WLAN_FEATURE_160M
        /* 从20信道+1, 从40信道+1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160PLUSPLUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 14;
            break;
        /* 从20信道+1, 从40信道+1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160PLUSPLUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 2;
            break;
        /* 从20信道+1, 从40信道-1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 6;
            break;
        /* 从20信道+1, 从40信道-1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160PLUSMINUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 10;
            break;
        /* 从20信道-1, 从40信道+1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 10;
            break;
        /* 从20信道-1, 从40信道+1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160MINUSPLUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 6;
            break;
        /* 从20信道-1, 从40信道-1, 从80信道+1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSPLUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number + 2;
            break;
        /* 从20信道-1, 从40信道-1, 从80信道-1 */
        case WLAN_BAND_WIDTH_160MINUSMINUSMINUS:
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            pst_vht_opern->uc_channel_center_freq_seg1 = pst_mac_vap->st_channel.uc_chan_number - 14;
            break;
#endif
        case WLAN_BAND_WIDTH_80PLUSPLUS:
            /***********************************************************************
            | 主20 | 从20 | 从40       |
                          |
                          |中心频率相对于主20偏6个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 6;
            break;
        case WLAN_BAND_WIDTH_80PLUSMINUS:
            /***********************************************************************
            | 从40        | 主20 | 从20 |
                          |
                          |中心频率相对于主20偏-2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            break;
        case WLAN_BAND_WIDTH_80MINUSPLUS:
            /***********************************************************************
            | 从20 | 主20 | 从40       |
                          |
                          |中心频率相对于主20偏2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            break;
        case WLAN_BAND_WIDTH_80MINUSMINUS:
            /***********************************************************************
            | 从40        | 从20 | 主20 |
                          |
                          |中心频率相对于主20偏-6个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 6;
            break;
        case WLAN_BAND_WIDTH_40MINUS:
            /***********************************************************************
            | 从20 | 主20 |
            |
            | 中心频率相对于主20偏-2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number - 2;
            break;
        case WLAN_BAND_WIDTH_40PLUS:
            /***********************************************************************
            | 主20 | 从20 |
            |
            | 中心频率相对于主20偏+2个信道
            ************************************************************************/
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number + 2;
            break;
        case WLAN_BAND_WIDTH_20M:
            /* 中心频率就是主信道频率   */
            pst_vht_opern->uc_channel_center_freq_seg0 = pst_mac_vap->st_channel.uc_chan_number;
            break;
        default:
            break;
    }

    if ((pst_mac_vap->st_channel.en_bandwidth < WLAN_BAND_WIDTH_160PLUSPLUSPLUS) ||
        (pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        pst_vht_opern->uc_channel_center_freq_seg1 = 0;
    }
    pst_vht_opern->us_basic_mcs_set = mac_mib_get_vht_rx_mcs_map(pst_mac_vap);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_VHT_INFO_IE_LEN;
}


uint32_t mac_set_csa_ie(uint8_t uc_mode, uint8_t uc_channel,
                              uint8_t uc_csa_cnt, uint8_t *puc_buffer,
                              uint8_t *puc_ie_len)
{
    if (oal_unlikely(oal_any_null_ptr2(puc_buffer, puc_ie_len))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*  Channel Switch Announcement Information Element Format               */
    /* --------------------------------------------------------------------- */
    /* | Element ID | Length | Chnl Switch Mode | New Chnl | Ch Switch Cnt | */
    /* --------------------------------------------------------------------- */
    /* | 1          | 1      | 1                | 1        | 1             | */
    /* --------------------------------------------------------------------- */
    /* 设置Channel Switch Announcement Element */
    puc_buffer[0] = MAC_EID_CHANSWITCHANN;
    puc_buffer[1] = MAC_CHANSWITCHANN_LEN;
    puc_buffer[2] = uc_mode; /* ask all associated STAs to stop transmission */
    puc_buffer[3] = uc_channel;
    puc_buffer[4] = uc_csa_cnt;

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_CHANSWITCHANN_LEN;

    return OAL_SUCC;
}


uint32_t mac_set_csa_bw_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru                        *pst_mac_vap = (mac_vap_stru *)pst_vap;
    wlan_channel_bandwidth_enum_uint8    en_bw;
    uint8_t                            uc_len;
    uint8_t                            uc_channel;

    if (oal_unlikely(oal_any_null_ptr3(puc_buffer, puc_ie_len, pst_mac_vap))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_bw = pst_mac_vap->st_ch_switch_info.en_announced_bandwidth;
    uc_channel = pst_mac_vap->st_ch_switch_info.uc_announced_channel;
    uc_len = 0;
    /* 封装Second channel offset IE */
    if (OAL_SUCC != mac_set_second_channel_offset_ie(en_bw, puc_buffer, &uc_len)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CSA,
                       "{mac_set_csa_bw_ie::mac_set_second_channel_offset_ie failed}");
        return 0;
    }

    puc_buffer += uc_len;
    *puc_ie_len = uc_len;

    if (OAL_TRUE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) {
        /* 11AC Wide Bandwidth Channel Switch IE */
        uc_len = 0;
        if (OAL_SUCC != mac_set_11ac_wideband_ie(uc_channel, en_bw, puc_buffer, &uc_len)) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CSA,
                           "{mac_set_csa_bw_ie::mac_set_11ac_wideband_ie failed}");
            return 0;
        }
        *puc_ie_len += uc_len;
    }

    return OAL_SUCC;
}


uint8_t *mac_get_ssid(uint8_t *puc_beacon_body, int32_t l_frame_body_len, uint8_t *puc_ssid_len)
{
    const uint8_t *puc_ssid_ie = OAL_PTR_NULL;
    uint16_t       us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    /*************************************************************************/
    /*                       Beacon Frame - Frame Body                       */
    /* --------------------------------------------------------------------- */
    /* |Timestamp |BeaconInt |CapInfo |SSID |SupRates |DSParSet |TIM elm   | */
    /* --------------------------------------------------------------------- */
    /* |8         |2         |2       |2-34 |3-10     |3        |4-256     | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /***************************************************************************
                    ----------------------------
                    |Element ID | Length | SSID|
                    ----------------------------
           Octets:  |1          | 1      | 0~32|
                    ----------------------------
    ***************************************************************************/
    /* ssid的长度初始赋值为0 */
    *puc_ssid_len = 0;

    /* 检测beacon帧或者probe rsp帧的长度的合法性 */
    if (l_frame_body_len <= us_offset) {
        oam_warning_log0(0, OAM_SF_ANY, "{mac_get_ssid:: the length of beacon/probe rsp frame body is invalid.}");
        return OAL_PTR_NULL;
    }

    /* 查找ssid的ie */
    puc_ssid_ie = mac_find_ie(MAC_EID_SSID,
                                  (puc_beacon_body + us_offset),
                                  (int32_t)(l_frame_body_len - us_offset));
    if ((puc_ssid_ie != OAL_PTR_NULL) && (puc_ssid_ie[1] < WLAN_SSID_MAX_LEN)) {
        /* 获取ssid ie的长度 */
        *puc_ssid_len = puc_ssid_ie[1];

        return (uint8_t *)(puc_ssid_ie + MAC_IE_HDR_LEN);
    }

    oam_warning_log0(0, OAM_SF_ANY, "{mac_get_ssid:: ssid ie isn't found.}");
    return OAL_PTR_NULL;
}


oal_bool_enum_uint8 mac_is_wmm_ie(uint8_t *puc_ie)
{
    /* --------------------------------------------------------------------- */
    /* WMM Information/Parameter Element Format                              */
    /* --------------------------------------------------------------------- */
    /* | OUI | OUIType | OUISubtype | Version | QoSInfo | OUISubtype based | */
    /* --------------------------------------------------------------------- */
    /* |3    | 1       | 1          | 1       | 1       | ---------------- | */
    /* --------------------------------------------------------------------- */
    return ((puc_ie[0] == MAC_EID_WMM) &&
            (puc_ie[1] >= MAC_WMM_IE_LEN) &&
            (puc_ie[2] == MAC_WMM_OUI_BYTE_ONE) &&
            (puc_ie[3] == MAC_WMM_OUI_BYTE_TWO) &&
            (puc_ie[4] == MAC_WMM_OUI_BYTE_THREE) &&                                               /* OUI */
            (puc_ie[5] == MAC_OUITYPE_WMM) &&                                                      /* OUI Type     */
            ((puc_ie[6] == MAC_OUISUBTYPE_WMM_INFO) || (puc_ie[6] == MAC_OUISUBTYPE_WMM_PARAM)) && /* OUI Sub Type */
            (puc_ie[7] == MAC_OUI_WMM_VERSION));                                                   /* Version field */
}


uint16_t mac_get_rsn_capability(const uint8_t *puc_rsn_ie)
{
    uint16_t us_pairwise_count;
    uint16_t us_akm_count;
    uint16_t us_rsn_capability;
    uint8_t  uc_ie_len;
    uint16_t us_suite_idx;
    uint16_t us_index;

    if (puc_rsn_ie == OAL_PTR_NULL) {
        return 0;
    }

    /******************************************************************************/
    /*                  RSN Element Format                                        */
    /* -------------------------------------------------------------------------- */
    /* |Element ID | Length | Version | Group Cipher Suite | Pairwise Cipher      */
    /* -------------------------------------------------------------------------- */
    /* |     1     |    1   |    2    |         4          |       2              */
    /* -------------------------------------------------------------------------- */
    /* -------------------------------------------------------------------------- */
    /* Suite Count| Pairwise Cipher Suite List | AKM Suite Count | AKM Suite List */
    /* -------------------------------------------------------------------------- */
    /*            |         4*m                |     2           |   4*n          */
    /* -------------------------------------------------------------------------- */
    /* -------------------------------------------------------------------------- */
    /* |RSN Capabilities|PMKID Count|PMKID List|Group Management Cipher Suite     */
    /* -------------------------------------------------------------------------- */
    /* |        2       |    2      |   16 *s  |               4           |      */
    /* -------------------------------------------------------------------------- */
    /*                                                                            */
    /******************************************************************************/
    uc_ie_len = puc_rsn_ie[1];
    if (uc_ie_len < MAC_MIN_RSN_LEN) {
        oam_warning_log1(0, OAM_SF_WPA, "{hmac_get_rsn_capability::invalid rsn ie len[%d].}", puc_rsn_ie[1]);
        return 0;
    }

    us_index = 8;
    /* 获取Pairwise Suite Count */
    us_pairwise_count = oal_make_word16(puc_rsn_ie[us_index], puc_rsn_ie[us_index + 1]);
    if (us_pairwise_count > MAC_PAIRWISE_CIPHER_SUITES_NUM) {
        oam_warning_log1(0, OAM_SF_WPA, "{hmac_get_rsn_capability::invalid us_pairwise_count[%d].}", us_pairwise_count);
        return 0;
    }
    us_index += 2;

    /* Pairwise Cipher Suite List */
    for (us_suite_idx = 0; us_suite_idx < us_pairwise_count; us_suite_idx++) {
        if (OAL_FALSE == MAC_IE_REAMIN_LEN_IS_ENOUGH(&puc_rsn_ie[2], &puc_rsn_ie[us_index], uc_ie_len, 4)) {
            return 0;
        }
        us_index += 4;
    }

    /* AKM Suite Count */
    if (OAL_FALSE == MAC_IE_REAMIN_LEN_IS_ENOUGH(&puc_rsn_ie[2], &puc_rsn_ie[us_index], uc_ie_len, 2)) {
        return 0;
    }
    us_akm_count = oal_make_word16(puc_rsn_ie[us_index], puc_rsn_ie[us_index + 1]);
    if (us_akm_count > WLAN_AUTHENTICATION_SUITES) {
        oam_warning_log1(0, OAM_SF_WPA, "{hmac_get_rsn_capability::invalid us_akm_count[%d].}", us_akm_count);
        return 0;
    }
    us_index += 2;

    /* AKM Suite List */
    for (us_suite_idx = 0; us_suite_idx < us_akm_count; us_suite_idx++) {
        if (OAL_FALSE == MAC_IE_REAMIN_LEN_IS_ENOUGH(&puc_rsn_ie[2], &puc_rsn_ie[us_index], uc_ie_len, 4)) {
            return 0;
        }
        us_index += 4;
    }

    /* 越过RSN Capabilities */
    if (OAL_FALSE == MAC_IE_REAMIN_LEN_IS_ENOUGH(&puc_rsn_ie[2], &puc_rsn_ie[us_index], uc_ie_len, 2)) {
        return 0;
    }

    us_rsn_capability = oal_make_word16(puc_rsn_ie[us_index], puc_rsn_ie[us_index + 1]);

    return us_rsn_capability;
}


uint16_t mac_get_beacon_period(uint8_t *puc_beacon_body)
{
    /*************************************************************************/
    /*                       Beacon Frame - Frame Body                       */
    /* --------------------------------------------------------------------- */
    /* |Timestamp |BeaconInt |CapInfo |SSID |SupRates |DSParSet |TIM elm   | */
    /* --------------------------------------------------------------------- */
    /* |8         |2         |2       |2-34 |3-10     |3        |4-256     | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    return *((uint16_t *)(puc_beacon_body + MAC_TIME_STAMP_LEN));
}


uint8_t mac_get_dtim_period(uint8_t *puc_frame_body, uint16_t us_frame_body_len)
{
    uint8_t   *puc_ie;
    uint16_t   us_offset;

    us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    puc_ie = mac_find_ie(MAC_EID_TIM, puc_frame_body + us_offset, us_frame_body_len - us_offset);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_MIN_TIM_LEN)) {
        return puc_ie[3];
    }

    return 0;
}


uint8_t mac_get_dtim_cnt(uint8_t *puc_frame_body, uint16_t us_frame_body_len)
{
    uint8_t   *puc_ie;
    uint16_t   us_offset;

    us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    puc_ie = mac_find_ie(MAC_EID_TIM, puc_frame_body + us_offset, us_frame_body_len - us_offset);
    if ((puc_ie != OAL_PTR_NULL) && (puc_ie[1] >= MAC_MIN_TIM_LEN)) {
        return puc_ie[2];
    }

    return 0;
}



uint8_t *mac_get_wmm_ie(uint8_t *puc_beacon_body, uint16_t us_frame_len)
{
    uint8_t *puc_wmmie = OAL_PTR_NULL;

    puc_wmmie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WMM,
                                       puc_beacon_body, us_frame_len);
    if (puc_wmmie == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    return mac_is_wmm_ie(puc_wmmie) ? puc_wmmie : OAL_PTR_NULL;
}


void mac_set_power_cap_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru            *pst_mac_vap = (mac_vap_stru *)pst_vap;
    mac_regclass_info_stru  *pst_regclass_info = OAL_PTR_NULL;

    if (OAL_FALSE == mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap) &&
        OAL_FALSE == mac_mib_get_dot11RadioMeasurementActivated(pst_mac_vap)) {
        oam_warning_log0(0, OAM_SF_TPC,
                         "{mac_set_power_cap_ie::en_dot11SpectrumManagementRequired,  \
                         en_dot11RadioMeasurementActivated are FALSE!}");
        *puc_ie_len = 0;
        return;
    }

    /********************************************************************************************
            ------------------------------------------------------------------------------------
            |ElementID | Length | MinimumTransmitPowerCapability| MaximumTransmitPowerCapability|
            ------------------------------------------------------------------------------------
    Octets: |1         | 1      | 1                             | 1                             |
            -------------------------------------------------------------------------------------

    *********************************************************************************************/

    *puc_buffer = MAC_EID_PWRCAP;
    *(puc_buffer + 1) = MAC_PWR_CAP_LEN;

    /* 成功获取管制域信息则根据国家码和TPC设置最大和最小发射功率，否则默认为0 */
    pst_regclass_info = mac_get_channel_num_rc_info(pst_mac_vap->st_channel.en_band,
                                                        pst_mac_vap->st_channel.uc_chan_number);

    if (pst_regclass_info != OAL_PTR_NULL) {
        *(puc_buffer + 2) = (uint8_t)((pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) ? 4 : 3);
        *(puc_buffer + 3) = (uint8_t)oal_min(pst_regclass_info->uc_max_reg_tx_pwr,
                                               pst_regclass_info->us_max_tx_pwr / 10);
    } else {
        *(puc_buffer + 2) = 0;
        *(puc_buffer + 3) = 0;
    }
    *puc_ie_len = MAC_IE_HDR_LEN + MAC_PWR_CAP_LEN;
}


void mac_set_supported_channel_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t        uc_channel_max_num;
    uint8_t        uc_channel_idx;
    uint8_t        us_channel_ie_len = 0;
    uint8_t       *puc_ie_len_buffer = 0;
    mac_vap_stru    *pst_mac_vap = (mac_vap_stru *)pst_vap;
    uint8_t        uc_channel_idx_cnt = 0;

    if (OAL_FALSE == mac_mib_get_dot11SpectrumManagementRequired(pst_mac_vap) ||
        OAL_TRUE == mac_mib_get_dot11ExtendedChannelSwitchActivated(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    /********************************************************************************************
            长度不定，信道号与信道数成对出现
            ------------------------------------------------------------------------------------
            |ElementID | Length | Fisrt Channel Number| Number of Channels|
            ------------------------------------------------------------------------------------
    Octets: |1         | 1      | 1                   | 1                 |
            -------------------------------------------------------------------------------------

    *********************************************************************************************/
    /* 根据支持的频段获取最大信道个数 */
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        uc_channel_max_num = (uint8_t)MAC_CHANNEL_FREQ_2_BUTT;
    } else if (pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) {
        uc_channel_max_num = (uint8_t)MAC_CHANNEL_FREQ_5_BUTT;
    } else {
        *puc_ie_len = 0;
        return;
    }

    *puc_buffer = MAC_EID_SUPPCHAN;
    puc_buffer++;
    puc_ie_len_buffer = puc_buffer;

    /* 填写信道信息 */
    for (uc_channel_idx = 0; uc_channel_idx < uc_channel_max_num; uc_channel_idx++) {
        /* 修改管制域结构体后，需要增加该是否支持信号的判断 */
        if (OAL_SUCC == mac_is_channel_idx_valid(pst_mac_vap->st_channel.en_band, uc_channel_idx)) {
            uc_channel_idx_cnt++;
            /* uc_channel_idx_cnt为1的时候表示是第一个可用信道，需要写到Fisrt Channel Number */
            if (uc_channel_idx_cnt == 1) {
                puc_buffer++;

                mac_get_channel_num_from_idx(pst_mac_vap->st_channel.en_band, uc_channel_idx, puc_buffer);
            } else if ((uc_channel_max_num - 1) == uc_channel_idx) {
                /* 将Number of Channels写入帧体中 */
                puc_buffer++;
                *puc_buffer = uc_channel_idx_cnt;

                us_channel_ie_len += 2;
            }
        } else {
            /* uc_channel_idx_cnt不为0的时候表示之前有可用信道，需要将可用信道的长度写到帧体中 */
            if (uc_channel_idx_cnt != 0) {
                /* 将Number of Channels写入帧体中 */
                puc_buffer++;
                *puc_buffer = uc_channel_idx_cnt;

                us_channel_ie_len += 2;
            }
            /* 将Number of Channels统计清零 */
            uc_channel_idx_cnt = 0;
        }
    }

    *puc_ie_len_buffer = us_channel_ie_len;
    *puc_ie_len = us_channel_ie_len + MAC_IE_HDR_LEN;
}


void mac_set_wmm_ie_sta(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t       uc_index;
    mac_vap_stru   *pst_mac_vap = (mac_vap_stru *)pst_vap;
    int32_t       l_ret;

    /* WMM Information Element Format                                */
    /* ------------------------------------------------------------- */
    /* | 3     | 1        | 1           | 1             | 1        | */
    /* ------------------------------------------------------------- */
    /* | OUI   | OUI Type | OUI Subtype | Version field | QoS Info | */
    /* ------------------------------------------------------------- */
    /* 判断STA是否支持WMM */
    if (OAL_TRUE != mac_mib_get_dot11QosOptionImplemented(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    puc_buffer[0] = MAC_EID_WMM;
    puc_buffer[1] = MAC_WMM_INFO_LEN;

    uc_index = MAC_IE_HDR_LEN;

    /* OUI */
    l_ret = memcpy_s(&puc_buffer[uc_index], MAC_OUI_LEN, g_auc_wmm_oui, MAC_OUI_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_wmm_ie_sta::memcpy fail!");
        return;
    }

    uc_index += MAC_OUI_LEN;

    /* OUI Type */
    puc_buffer[uc_index++] = MAC_OUITYPE_WMM;

    /* OUI Subtype */
    puc_buffer[uc_index++] = MAC_OUISUBTYPE_WMM_INFO;

    /* Version field */
    puc_buffer[uc_index++] = MAC_OUI_WMM_VERSION;

    /* QoS Information Field */
    mac_set_qos_info_field(pst_mac_vap, &puc_buffer[uc_index]);
    uc_index += MAC_QOS_INFO_LEN;

    /* Reserved */
    puc_buffer[uc_index++] = 0;

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_WMM_INFO_LEN;
}

#ifdef _PRE_WLAN_FEATURE_WMMAC

void mac_set_tspec_info_field(uint8_t *pst_vap, mac_wmm_tspec_stru *pst_addts_args, uint8_t *puc_buffer)
{
    mac_wmm_tspec_stru *pst_tspec_info;
    /**************************************************************************************************/
    /* TSPEC字段:
              ----------------------------------------------------------------------------------------
              |TS Info|Nominal MSDU Size|Max MSDU Size|Min Serv Itvl|Max Serv Itvl|
              ----------------------------------------------------------------------------------------
     Octets:  | 3     |  2              |   2         |4            |4            |
              ----------------------------------------------------------------------------------------
              | Inactivity Itvl | Suspension Itvl | Serv Start Time |Min Data Rate | Mean Data Rate |
              ----------------------------------------------------------------------------------------
     Octets:  |4                | 4               | 4               |4             |  4             |
              ----------------------------------------------------------------------------------------
              |Peak Data Rate|Burst Size|Delay Bound|Min PHY Rate|Surplus BW Allowance  |Medium Time|
              ----------------------------------------------------------------------------------------
     Octets:  |4             |4         | 4         | 4          |  2                   |2          |
              ----------------------------------------------------------------------------------------

     TS info字段:
              ----------------------------------------------------------------------------------------
              |Reserved |TSID |Direction |1 |0 |Reserved |PSB |UP |Reserved |Reserved |Reserved |
              ----------------------------------------------------------------------------------------
       Bits:  |1        |4    |2         |  2  |1        |1   |3  |2        |1        |7        |
              ----------------------------------------------------------------------------------------
    ***************************************************************************************************/
    /* 初始化TSPEC结构内存信息 */
    memset_s(puc_buffer, MAC_WMMAC_TSPEC_LEN, 0, MAC_WMMAC_TSPEC_LEN);

    pst_tspec_info = (mac_wmm_tspec_stru *)(puc_buffer);  // TSPEC Body

    pst_tspec_info->ts_info.bit_tsid = pst_addts_args->ts_info.bit_tsid;
    pst_tspec_info->ts_info.bit_direction = pst_addts_args->ts_info.bit_direction;
    pst_tspec_info->ts_info.bit_acc_policy = 1;
    pst_tspec_info->ts_info.bit_apsd = pst_addts_args->ts_info.bit_apsd;
    pst_tspec_info->ts_info.bit_user_prio = pst_addts_args->ts_info.bit_user_prio;

    pst_tspec_info->us_norminal_msdu_size = pst_addts_args->us_norminal_msdu_size;
    pst_tspec_info->us_max_msdu_size = pst_addts_args->us_max_msdu_size;
    pst_tspec_info->ul_min_data_rate = pst_addts_args->ul_min_data_rate;
    pst_tspec_info->ul_mean_data_rate = pst_addts_args->ul_mean_data_rate;
    pst_tspec_info->ul_peak_data_rate = pst_addts_args->ul_peak_data_rate;
    pst_tspec_info->ul_min_phy_rate = pst_addts_args->ul_min_phy_rate;
    pst_tspec_info->us_surplus_bw = pst_addts_args->us_surplus_bw;
}


uint16_t mac_set_wmmac_ie_sta(uint8_t *pst_vap, uint8_t *puc_buffer, mac_wmm_tspec_stru *pst_addts_args)
{
    uint8_t uc_index;
    int32_t l_ret;

    /************************************************************************************/
    /*                                Set WMM TSPEC 信息:                               */
    /*       ---------------------------------------------------------------------------
             |ID | Length| OUI |OUI Type| OUI subtype| Version| TSPEC body|
             ---------------------------------------------------------------------------
    Octets:  |1  | 1     | 3   |1       | 1          | 1      | 55        |
             ---------------------------------------------------------------------------
    *************************************************************************************/

    puc_buffer[0] = MAC_EID_WMM;
    puc_buffer[1] = MAC_WMMAC_INFO_LEN;

    uc_index = MAC_IE_HDR_LEN;

    /* OUI */
    l_ret = memcpy_s(&puc_buffer[uc_index], MAC_OUI_LEN, g_auc_wmm_oui, MAC_OUI_LEN);
    if (l_ret != EOK) {
        oam_warning_log0(0, OAM_SF_ANY, "mac_set_wmmac_ie_sta::memcpy fail!");
    }

    uc_index += MAC_OUI_LEN;

    /* OUI Type */
    puc_buffer[uc_index++] = MAC_OUITYPE_WMM;

    /* OUI Subtype */
    puc_buffer[uc_index++] = MAC_OUISUBTYPE_WMMAC_TSPEC;

    /* Version field */
    puc_buffer[uc_index++] = MAC_OUI_WMM_VERSION;

    /* wmmac tspec Field */
    mac_set_tspec_info_field(pst_vap, pst_addts_args, &puc_buffer[uc_index]);

    return (MAC_IE_HDR_LEN + MAC_WMMAC_INFO_LEN);
}

#endif  // _PRE_WLAN_FEATURE_WMMAC


void mac_set_listen_interval_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    puc_buffer[0] = 0x0a;
    puc_buffer[1] = 0x00;
    *puc_ie_len = MAC_LIS_INTERVAL_IE_LEN;
}


void mac_set_status_code_ie(uint8_t *puc_buffer, mac_status_code_enum_uint16 en_status_code)
{
    puc_buffer[0] = (uint8_t)(en_status_code & 0x00FF);
    puc_buffer[1] = (uint8_t)((en_status_code & 0xFF00) >> 8);
}


void mac_set_aid_ie(uint8_t *puc_buffer, uint16_t uc_aid)
{
    /* The 2 MSB bits of Association ID is set to 1 as required by the standard. */
    uc_aid |= 0xC000;
    puc_buffer[0] = (uc_aid & 0x00FF);
    puc_buffer[1] = (uc_aid & 0xFF00) >> 8;
}


uint8_t mac_get_bss_type(uint16_t us_cap_info)
{
    mac_cap_info_stru *pst_cap_info = (mac_cap_info_stru *)&us_cap_info;

    if (pst_cap_info->bit_ess != 0) {
        return (uint8_t)WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    }

    if (pst_cap_info->bit_ibss != 0) {
        return (uint8_t)WLAN_MIB_DESIRED_BSSTYPE_INDEPENDENT;
    }

    return (uint8_t)WLAN_MIB_DESIRED_BSSTYPE_ANY;
}


uint32_t mac_check_mac_privacy(uint16_t us_cap_info, uint8_t *pst_vap)
{
    mac_vap_stru        *pst_mac_vap = OAL_PTR_NULL;
    mac_cap_info_stru   *pst_cap_info = (mac_cap_info_stru *)&us_cap_info;

    if (pst_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = (mac_vap_stru *)pst_vap;

    if (OAL_TRUE == mac_mib_get_privacyinvoked(pst_mac_vap)) {
        /* 该VAP有Privacy invoked但其他VAP没有 */
        if (pst_cap_info->bit_privacy == 0) {
            return (uint32_t)OAL_FALSE;
        }
    }

    return (uint32_t)OAL_TRUE;
}


void mac_add_app_ie(void *pst_vap,
                            uint8_t *puc_buffer,
                            uint16_t *pus_ie_len,
                            en_app_ie_type_uint8 en_type)
{
    mac_vap_stru    *pst_mac_vap;
    uint8_t       *puc_app_ie;
    uint32_t       ul_app_ie_len;
    int32_t        l_ret;

    pst_mac_vap = (mac_vap_stru *)pst_vap;
    puc_app_ie = pst_mac_vap->ast_app_ie[en_type].puc_ie;
    ul_app_ie_len = pst_mac_vap->ast_app_ie[en_type].ul_ie_len;

    if (ul_app_ie_len == 0) {
        *pus_ie_len = 0;
        return;
    } else {
        l_ret = memcpy_s(puc_buffer, ul_app_ie_len, puc_app_ie, ul_app_ie_len);
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "mac_add_app_ie::memcpy fail!");
            return;
        }

        *pus_ie_len = (uint16_t)ul_app_ie_len;
    }

    return;
}


void mac_add_wps_ie(void *pst_vap,
                            uint8_t *puc_buffer,
                            uint16_t *pus_ie_len,
                            en_app_ie_type_uint8 en_type)
{
    mac_vap_stru    *pst_mac_vap;
    uint8_t       *puc_app_ie;
    uint8_t       *puc_wps_ie = OAL_PTR_NULL;
    uint32_t       ul_app_ie_len;
    int32_t        l_ret;

    pst_mac_vap = (mac_vap_stru *)pst_vap;
    puc_app_ie = pst_mac_vap->ast_app_ie[en_type].puc_ie;
    ul_app_ie_len = pst_mac_vap->ast_app_ie[en_type].ul_ie_len;

    if (ul_app_ie_len == 0) {
        *pus_ie_len = 0;
        return;
    }

    puc_wps_ie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WPS,
                                        puc_app_ie, (int32_t)ul_app_ie_len);
    if ((puc_wps_ie == OAL_PTR_NULL) || (puc_wps_ie[1] < MAC_MIN_WPS_IE_LEN)) {
        *pus_ie_len = 0;
        return;
    }

    /* 将WPS ie 信息拷贝到buffer 中 */
    l_ret = memcpy_s(puc_buffer, puc_wps_ie[1] + MAC_IE_HDR_LEN, puc_wps_ie, puc_wps_ie[1] + MAC_IE_HDR_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_add_wps_ie::memcpy fail!");
        return;
    }

    *pus_ie_len = puc_wps_ie[1] + MAC_IE_HDR_LEN;

    return;
}

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

void mac_set_opmode_field(uint8_t *pst_vap, uint8_t *puc_buffer, wlan_nss_enum_uint8 en_nss)
{
    mac_vap_stru           *pst_mac_vap       = (mac_vap_stru *)pst_vap;
    mac_opmode_notify_stru *pst_opmode_notify = (mac_opmode_notify_stru *)puc_buffer;
    wlan_bw_cap_enum_uint8  en_cp_bw          = WLAN_BW_CAP_BUTT;

    /******************************************************************/
    /* -------------------------------------------------------------- */
    /* |B0-B1         |B2        |B3       |B4-B6   |B7         |     */
    /* -------------------------------------------------------------- */
    /* |Channel Width |160M/80+80|no_LDPC  |Rx Nss  |Rx Nss Type|     */
    /* -------------------------------------------------------------- */
    /******************************************************************/
    /* bit_rx_nss_type目前写定0即可，后续出现三流以上，但是txbf流小于三流时，才需要发送1类型 再扩展TBD */
    memset_s(pst_opmode_notify, OAL_SIZEOF(mac_opmode_notify_stru), 0, OAL_SIZEOF(mac_opmode_notify_stru));
    mac_vap_get_bandwidth_cap(pst_mac_vap, &en_cp_bw);
     /* 16版11ac协议 160M且博通 */
    if ((en_cp_bw == WLAN_BW_CAP_160M) && (pst_mac_vap->bit_ap_chip_oui == WLAN_AP_CHIP_OUI_BCM)) {
        pst_opmode_notify->bit_channel_width = WLAN_BW_CAP_80M;
        pst_opmode_notify->bit_160or8080     = OAL_TRUE;
    } else {
        /* 13版协议 */
        pst_opmode_notify->bit_channel_width = en_cp_bw;
    }

    /* 要切换到的流，采用vap下的nss能力的话，需要提前置vap的nss能力，接口不灵活，增加入参，提高场景可扩展性 */
    pst_opmode_notify->bit_rx_nss        = en_nss;
    pst_opmode_notify->bit_rx_nss_type   = 0;
}



void mac_set_opmode_notify_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /********************************************
            -------------------------------------
            |ElementID | Length | Operating Mode|
            -------------------------------------
    Octets: |1         | 1      | 1             |
            -------------------------------------

    ********************************************/
    if ((OAL_FALSE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) ||
        (OAL_FALSE == mac_mib_get_OperatingModeNotificationImplemented(pst_mac_vap))) {
        *puc_ie_len = 0;
        return;
    }

    puc_buffer[0] = MAC_EID_OPMODE_NOTIFY;
    puc_buffer[1] = MAC_OPMODE_NOTIFY_LEN;

    mac_set_opmode_field((void *)pst_vap, (puc_buffer + MAC_IE_HDR_LEN), pst_mac_vap->en_vap_rx_nss);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_OPMODE_NOTIFY_LEN;
}
#endif

#ifdef _PRE_WLAN_FEATURE_11K

void mac_set_wfa_tpc_report_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    uint8_t        uc_index = 0;
    mac_vap_stru    *pst_mac_vap = (mac_vap_stru *)pst_vap;
    int32_t        l_ret;

    /***************************************************************************
       -------------------------------------------------------------------------
       |EID |Length |OUI   |OUI Type|OUI Subtype |Transmit Power |Link Margin |
       -------------------------------------------------------------------------
       |1   |1      | 3    |1       |1           |1              |1           |
       -------------------------------------------------------------------------
       ***************************************************************************/
    puc_buffer[0] = MAC_EID_WFA_TPC_RPT;
    puc_buffer[1] = MAC_WFA_TPC_RPT_LEN;
    uc_index += MAC_IE_HDR_LEN;

    l_ret = memcpy_s(puc_buffer + uc_index, MAC_OUI_LEN, g_auc_wfa_oui, MAC_OUI_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_set_wfa_tpc_report_ie::set wfa_oui fail!");
        return;
    }

    uc_index += MAC_OUI_LEN;

    puc_buffer[uc_index++] = MAC_OUITYPE_WFA;
    puc_buffer[uc_index++] = MAC_OUISUBTYPE_WFA;
    puc_buffer[uc_index++] = pst_mac_vap->uc_tx_power;
    puc_buffer[uc_index] = 0;

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_WFA_TPC_RPT_LEN;
}

#endif  // _PRE_WLAN_FEATURE_11K

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)


wlan_pmf_cap_status_uint8 mac_get_pmf_cap(uint8_t *puc_ie, uint32_t ul_ie_len)
{
    uint8_t *puc_rsn_ie = NULL;
    uint16_t us_rsn_cap;

    if (oal_unlikely(puc_ie == OAL_PTR_NULL)) {
        return MAC_PMF_DISABLED;
    }

    /* 查找RSN信息元素,如果没有RSN信息元素,则按照不支持处理 */
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_ie, (int32_t)(ul_ie_len));
    if (puc_rsn_ie == OAL_PTR_NULL) {
        return MAC_PMF_DISABLED;
    }

    /* 根据RSN信息元素, 判断RSN能力是否匹配 */
    us_rsn_cap = mac_get_rsn_capability(puc_rsn_ie);
    if ((us_rsn_cap & BIT6) && (us_rsn_cap & BIT7)) {
        return MAC_PMF_REQUIRED;
    }

    if (us_rsn_cap & BIT7) {
        return MAC_PMF_ENABLED;
    }
    return MAC_PMF_DISABLED;
}
#endif
#if defined(_PRE_WLAN_FEATURE_11R)

void mac_set_md_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    if (OAL_TRUE != mac_mib_get_ft_trainsistion(pst_mac_vap)) {
        *puc_ie_len = 0;
        return;
    }

    /***************************************************************************
                    ---------------------------------------------------
                    |Element ID | Length | MDID| FT Capbility & Policy|
                    ---------------------------------------------------
           Octets:  |1          | 1      | 2   |  1                   |
                    ---------------------------------------------------
    ***************************************************************************/

    *puc_buffer = MAC_EID_MOBILITY_DOMAIN;
    *(puc_buffer + 1) = 3;
    *(puc_buffer + 2) = mac_mib_get_ft_mdid(pst_mac_vap) & 0xFF;
    *(puc_buffer + 3) = (mac_mib_get_ft_mdid(pst_mac_vap) >> 8) & 0xFF;
    *(puc_buffer + 4) = 0;

    /***************************************************************************
                    ------------------------------------------
                    |FT over DS| RRP Capability | Reserved   |
                    ------------------------------------------
             Bits:  |1         | 1              | 6          |
                    ------------------------------------------
    ***************************************************************************/
    if (OAL_TRUE == mac_mib_get_ft_over_ds(pst_mac_vap)) {
        *(puc_buffer + 4) += 1;
    }
    if (OAL_TRUE == mac_mib_get_ft_resource_req(pst_mac_vap)) {
        *(puc_buffer + 4) += 2;
    }
    *puc_ie_len = 5;
}


void mac_set_ft_ie(void *pst_vap, uint8_t *puc_buffer, uint16_t *pus_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    if (OAL_TRUE != mac_mib_get_ft_trainsistion(pst_mac_vap)) {
        *pus_ie_len = 0;
        return;
    }

    /*********************************************************************************************
              ------------------------------------------------------------------------------
              |Element ID | Length | MIC Control | MIC | ANonce | SNonce | Optional Params |
              ------------------------------------------------------------------------------
     Octets:  |1          | 1      | 2           |  16 |   32   |   32   |   variable      |
              ------------------------------------------------------------------------------
    ***********************************************************************************************/

    *puc_buffer = MAC_EID_FT;

    *pus_ie_len = 84;
}
#ifdef _PRE_WLAN_FEATURE_11R
void mac_set_rde_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    /*********************************************************************************************
              -------------------------------------------------------------------
              |Element ID | Length | RDE Identifier | Res Dscr Cnt| Status Code |
              -------------------------------------------------------------------
     Octets:  |1          | 1      | 1              |  1          |   2         |
              -------------------------------------------------------------------
    ***********************************************************************************************/
    *puc_buffer = MAC_EID_RDE;
    *(puc_buffer + 1) = 4;
    *(puc_buffer + 2) = 0;
    /* TBD:count对应RDE后跟随的RIC descriptor IE个数 */
    *(puc_buffer + 3) = 0;
    *(puc_buffer + 4) = 0;
    *(puc_buffer + 5) = 0;

    *puc_ie_len = 6;
}

void mac_set_tspec_ie(void *pst_mac_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len, uint8_t uc_tid)
{
    uint8_t         uc_len = 0;
    mac_ts_info_stru *pst_ts_info;

    /***********************************************************************************************
              --------------------------------------------------------------------------------------
              |Element ID|Length|TS Info|Nominal MSDU Size|Max MSDU Size|Min Serv Itvl|Max Serv Itvl|
              ---------------------------------------------------------------------------------------
     Octets:  |1         | 1(55)| 3     |  2              |   2         |4            |4            |
              ---------------------------------------------------------------------------------------
              | Inactivity Itvl | Suspension Itvl | Serv Start Time |Min Data Rate | Mean Data Rate |
              ---------------------------------------------------------------------------------------
     Octets:  |4                | 4               | 4               |4             |  4             |
              ---------------------------------------------------------------------------------------
              |Peak Data Rate|Burst Size|Delay Bound|Min PHY Rate|Surplus BW Allowance  |Medium Time|
              ---------------------------------------------------------------------------------------
     Octets:  |4             |4         | 4         | 4          |  2                   |2          |
              ---------------------------------------------------------------------------------------

    ***********************************************************************************************/
    *puc_buffer = MAC_EID_TSPEC;
    *(puc_buffer + 1) = 55;
    uc_len += 2;

    memset_s(puc_buffer + uc_len, *(puc_buffer + 1), 0, *(puc_buffer + 1));

    pst_ts_info = (mac_ts_info_stru *)(puc_buffer + uc_len);  // TS Info
    uc_len += 3;

    pst_ts_info->bit_tsid = uc_tid;
    pst_ts_info->bit_direction = 3;  // bidirection
    pst_ts_info->bit_apsd = 1;
    pst_ts_info->bit_user_prio = uc_tid;

    *(uint16_t *)(puc_buffer + uc_len) = 0x812c;  // Nominal MSDU Size
    uc_len += 28;

    *(uint32_t *)(puc_buffer + uc_len) = 0x0001D4C0;  // Mean Data Rate
    uc_len += 16;

    *(uint16_t *)(puc_buffer + uc_len) = 0x3000;  // Surplus BW Allowance

    *puc_ie_len = 57;
}
#endif
#endif  // _PRE_WLAN_FEATURE_11R

#if defined(_PRE_WLAN_FEATURE_11K) ||   \
    defined(_PRE_WLAN_FEATURE_FTM)

void mac_set_rrm_enabled_cap_field(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru                *pst_mac_vap = (mac_vap_stru *)pst_vap;
    oal_rrm_enabled_cap_ie_stru *pst_rrm_enabled_cap_ie = OAL_PTR_NULL;

    puc_buffer[0] = MAC_EID_RRM;
    puc_buffer[1] = MAC_RRM_ENABLE_CAP_IE_LEN;

    pst_rrm_enabled_cap_ie = (oal_rrm_enabled_cap_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);

    memset_s(pst_rrm_enabled_cap_ie,
             OAL_SIZEOF(oal_rrm_enabled_cap_ie_stru), 0,
             OAL_SIZEOF(oal_rrm_enabled_cap_ie_stru));

#if defined(_PRE_WLAN_FEATURE_11K)
    /* 只有bit0 4 5 6位置1 */
    pst_rrm_enabled_cap_ie->bit_link_cap = mac_mib_get_dot11RMLinkMeasurementActivated(pst_mac_vap);
    pst_rrm_enabled_cap_ie->bit_bcn_passive_cap = mac_mib_get_dot11RMBeaconPassiveMeasurementActivated(pst_vap);
    pst_rrm_enabled_cap_ie->bit_bcn_active_cap = mac_mib_get_dot11RMBeaconActiveMeasurementActivated(pst_vap);
    pst_rrm_enabled_cap_ie->bit_bcn_table_cap = mac_mib_get_dot11RMBeaconTableMeasurementActivated(pst_vap);
#endif

#if (defined(_PRE_WLAN_FEATURE_11K))
    /* st_wlan_mib_sta_config.en_dot11RMNeighborReportActivated ROM化,对于03,
      如果11k打开,直接设置为 1  */
    if (IS_LEGACY_STA(pst_mac_vap)) {
        pst_rrm_enabled_cap_ie->bit_neighbor_rpt_cap = 1;
    }
#endif

#if (defined(_PRE_WLAN_FEATURE_FTM)) || defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    pst_rrm_enabled_cap_ie->bit_ftm_range_report_cap = mac_mib_get_FtmRangeReportActivated(pst_mac_vap);
#endif

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_RRM_ENABLE_CAP_IE_LEN;
}
#endif

#ifdef _PRE_WLAN_FEATURE_HISTREAM

void mac_set_histream_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    puc_buffer[0] = MAC_EID_VENDOR;
    puc_buffer[1] = MAC_OUI_LEN + 1;
    /*lint -e572*/ /*lint -e778*/
    puc_buffer[2] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 16) & 0xff);
    puc_buffer[3] = (uint8_t)((MAC_HUAWEI_VENDER_IE >> 8) & 0xff);
    puc_buffer[4] = (uint8_t)((MAC_HUAWEI_VENDER_IE)&0xff);
    puc_buffer[5] = MAC_HISI_HISTREAM_IE;

    /*lint +e572*/ /*lint +e778*/
    *puc_ie_len = MAC_IE_HDR_LEN + puc_buffer[1];
}
#endif  // _PRE_WLAN_FEATURE_HISTREAM


uint16_t mac_encap_2040_coext_mgmt(void *pst_vap,
                                         oal_netbuf_stru *pst_buffer,
                                         uint8_t uc_coext_info,
                                         uint32_t ul_chan_report)
{
    uint8_t                       *puc_mac_header = oal_netbuf_header(pst_buffer);
    uint8_t                       *puc_payload_addr = puc_mac_header + MAC_80211_FRAME_LEN;
    uint8_t                        uc_chan_idx = 0;
    uint16_t                       us_ie_len_idx;
    uint16_t                       us_index = 0;
    mac_vap_stru                    *pst_mac_vap = (mac_vap_stru *)pst_vap;
    wlan_channel_band_enum_uint8     en_band = pst_mac_vap->st_channel.en_band;
    uint8_t                        uc_max_num_chan = mac_get_num_supp_channel(en_band);
    uint8_t                        uc_channel_num;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /* 设置 Frame Control field */
    mac_hdr_set_frame_control(puc_mac_header, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number(puc_mac_header, 0);

    /* 设置 address1(接收端): AP MAC地址 (BSSID) */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR1_OFFSET, pst_mac_vap->auc_bssid);

    /* 设置 address2(发送端): dot11StationID */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(pst_mac_vap));

    /* 设置 address3: AP MAC地址 (BSSID) */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR3_OFFSET, pst_mac_vap->auc_bssid);

    /*************************************************************************************/
    /*                 20/40 BSS Coexistence Management frame - Frame Body               */
    /* --------------------------------------------------------------------------------- */
    /* |Category |Public Action |20/40 BSS Coex IE| 20/40 BSS Intolerant Chan Report IE| */
    /* --------------------------------------------------------------------------------- */
    /* |1        |1             |3                |Variable                            | */
    /* --------------------------------------------------------------------------------- */
    /*                                                                                   */
    /*************************************************************************************/

    puc_payload_addr[us_index++] = MAC_ACTION_CATEGORY_PUBLIC; /* Category */
    puc_payload_addr[us_index++] = MAC_PUB_COEXT_MGMT;         /* Public Action */

    /* 封装20/40 BSS Coexistence element */
    puc_payload_addr[us_index++] = MAC_EID_2040_COEXT; /* Element ID */
    puc_payload_addr[us_index++] = MAC_2040_COEX_LEN;  /* Length */
    puc_payload_addr[us_index++] = uc_coext_info;      /* 20/40 BSS Coexistence Information field */

    /* 封装20/40 BSS Intolerant Channel Report element */
    /* 只有当STA检测到Trigger Event A时，才包含Operating Class，参见802.11n 10.15.12 */
    puc_payload_addr[us_index++] = MAC_EID_2040_INTOLCHREPORT; /* Element ID */
    us_ie_len_idx = us_index;
    puc_payload_addr[us_index++] = MAC_2040_INTOLCHREPORT_LEN_MIN; /* Length */
    puc_payload_addr[us_index++] = 0;                              /* Operating Class */
    if (ul_chan_report > 0) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{mac_encap_2040_coext_mgmt::Channel List = 0x%x.}", ul_chan_report);
        /* Channel List */
        for (uc_chan_idx = 0; uc_chan_idx < uc_max_num_chan; uc_chan_idx++) {
            if (((ul_chan_report >> uc_chan_idx) & BIT0) != 0) {
                mac_get_channel_num_from_idx(en_band, uc_chan_idx, &uc_channel_num);
                puc_payload_addr[us_index++] = uc_channel_num;
                puc_payload_addr[us_ie_len_idx]++;
            }
        }
    }

    return (uint16_t)(us_index + MAC_80211_FRAME_LEN);
}

oal_bool_enum_uint8 mac_frame_is_null_data(oal_netbuf_stru *pst_net_buf)
{
    uint8_t           *pst_mac_header;
    uint8_t            uc_frame_type;
    uint8_t            uc_frame_subtype;
    mac_tx_ctl_stru     *pst_tx_ctl;

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_net_buf);
    pst_mac_header = (uint8_t *)(MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl));
    uc_frame_type = mac_frame_get_type_value((uint8_t *)pst_mac_header);
    uc_frame_subtype = mac_frame_get_subtype_value((uint8_t *)pst_mac_header);

    if ((uc_frame_type == WLAN_DATA_BASICTYPE) &&
        ((uc_frame_subtype == WLAN_NULL_FRAME) || (uc_frame_subtype == WLAN_QOS_NULL_FRAME))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

/*lint -e19*/
oal_module_symbol(mac_find_ie);
oal_module_symbol(mac_find_ie_sec);
oal_module_symbol(mac_find_vendor_ie);
oal_module_symbol(mac_set_aid_ie);
oal_module_symbol(mac_set_supported_rates_ie);
oal_module_symbol(mac_set_ext_capabilities_ie);
oal_module_symbol(mac_set_ssid_ie);
oal_module_symbol(mac_set_exsup_rates_ie);
oal_module_symbol(mac_set_cap_info_ap);
oal_module_symbol(mac_set_cap_info_sta);
oal_module_symbol(mac_set_wmm_params_ie);
oal_module_symbol(mac_set_wmm_ie_sta);
#ifdef _PRE_WLAN_FEATURE_WMMAC
oal_module_symbol(mac_set_wmmac_ie_sta);
#endif  // _PRE_WLAN_FEATURE_WMMAC
oal_module_symbol(mac_set_ht_capabilities_ie);
oal_module_symbol(mac_set_obss_scan_params);
oal_module_symbol(mac_set_ht_opern_ie);
oal_module_symbol(mac_set_listen_interval_ie);
oal_module_symbol(mac_set_status_code_ie);
oal_module_symbol(mac_set_power_cap_ie);
oal_module_symbol(mac_set_supported_channel_ie);
oal_module_symbol(mac_set_rsn_ie);
oal_module_symbol(mac_set_wpa_ie);
oal_module_symbol(mac_set_vht_capabilities_ie);
oal_module_symbol(mac_set_vht_opern_ie);
oal_module_symbol(mac_get_dtim_period);
oal_module_symbol(mac_get_dtim_cnt);
oal_module_symbol(mac_is_wmm_ie);
oal_module_symbol(mac_check_mac_privacy);
oal_module_symbol(g_auc_rsn_oui);
oal_module_symbol(g_auc_wmm_oui);
oal_module_symbol(mac_set_timeout_interval_ie);
oal_module_symbol(mac_add_app_ie);
oal_module_symbol(mac_get_bss_type);
oal_module_symbol(mac_get_beacon_period);

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
oal_module_symbol(mac_set_opmode_notify_ie);
#endif
oal_module_symbol(mac_report_80211_frame);
oal_module_symbol(mac_rx_report_80211_frame);
oal_module_symbol(mac_set_11ntxbf_vendor_ie);
oal_module_symbol(mac_get_wmm_ie);
oal_module_symbol(mac_get_rsn_capability);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
oal_module_symbol(mac_get_pmf_cap);
#endif

oal_module_symbol(mac_encap_2040_coext_mgmt);
#ifdef _PRE_WLAN_FEATURE_HISTREAM
oal_module_symbol(mac_set_histream_ie);
#endif  // _PRE_WLAN_FEATURE_HISTREAM

#if defined(_PRE_WLAN_FEATURE_11K)
oal_module_symbol(mac_set_rrm_enabled_cap_field);
#endif /*lint +e19*/
