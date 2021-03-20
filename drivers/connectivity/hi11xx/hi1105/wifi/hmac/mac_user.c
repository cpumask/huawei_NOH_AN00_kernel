

/* 1 ͷ�ļ����� */
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "mac_resource.h"
#include "mac_device.h"
#include "mac_user.h"
#include "securec.h"
#include "securectype.h"
#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_USER_C


uint32_t mac_user_update_ap_bandwidth_cap(mac_user_stru *pst_mac_user)
{
    mac_user_ht_hdl_stru *pst_mac_ht_hdl = OAL_PTR_NULL;
    mac_vht_hdl_stru *pst_mac_vht_hdl = OAL_PTR_NULL;

    if (pst_mac_user == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡHT��VHT�ṹ��ָ�� */
    pst_mac_ht_hdl = &(pst_mac_user->st_ht_hdl);
    pst_mac_vht_hdl = &(pst_mac_user->st_vht_hdl);

    pst_mac_user->en_bandwidth_cap = WLAN_BW_CAP_20M;

    if (pst_mac_ht_hdl->en_ht_capable == OAL_TRUE) {
        if (pst_mac_ht_hdl->bit_secondary_chan_offset != MAC_SCN) {
            pst_mac_user->en_bandwidth_cap = WLAN_BW_CAP_40M;
        }
    }

    if (pst_mac_vht_hdl->en_vht_capable == OAL_TRUE) {
        /* en_channel_width��ȡֵ:VHT opern ie��0 -- 20/40M, 1 -- 80/160M, ͬʱ�����ѷ�����2��3 */
        /* bit_supported_channel_width: VHT capabilities,0--��160,1--160M, 2--160 and 80+80 */
        if (pst_mac_vht_hdl->en_channel_width == WLAN_MIB_VHT_OP_WIDTH_80) {
            if ((pst_mac_vht_hdl->bit_supported_channel_width == 0) && (pst_mac_vht_hdl->bit_extend_nss_bw_supp != 0)) {
                pst_mac_user->en_bandwidth_cap =
                    (pst_mac_ht_hdl->uc_chan_center_freq_seg2 != 0) ? WLAN_BW_CAP_160M : WLAN_BW_CAP_80M;
            } else {
                pst_mac_user->en_bandwidth_cap =
                    (pst_mac_vht_hdl->uc_channel_center_freq_seg1 != 0) ? WLAN_BW_CAP_160M : WLAN_BW_CAP_80M;
            }
        } else if (pst_mac_vht_hdl->en_channel_width == WLAN_MIB_VHT_OP_WIDTH_160) {
            pst_mac_user->en_bandwidth_cap = WLAN_BW_CAP_160M;
        } else if (pst_mac_vht_hdl->en_channel_width == WLAN_MIB_VHT_OP_WIDTH_80PLUS80) {
            pst_mac_user->en_bandwidth_cap = WLAN_BW_CAP_80M;
        }
    }

    return OAL_SUCC;
}


void mac_user_set_num_spatial_stream_160M(mac_user_stru *pst_mac_user, uint8_t uc_value)
{
    pst_mac_user->st_vht_hdl.bit_user_num_spatial_stream_160M = uc_value;
}


uint8_t mac_user_get_sta_cap_bandwidth_11ac(wlan_channel_band_enum_uint8 en_band,
                                              mac_user_ht_hdl_stru *pst_mac_ht_hdl,
                                              mac_vht_hdl_stru *pst_mac_vht_hdl,
                                              mac_user_stru *pst_mac_user)
{
    wlan_bw_cap_enum_uint8 en_bandwidth_cap = WLAN_BW_CAP_20M;

    /* 2.4g band��Ӧ����vht cap��ȡ������Ϣ */
    if ((en_band == WLAN_BAND_2G) && (pst_mac_ht_hdl->en_ht_capable == OAL_TRUE)) {
        en_bandwidth_cap =
            (pst_mac_ht_hdl->bit_supported_channel_width == WLAN_BW_CAP_40M) ? WLAN_BW_CAP_40M : WLAN_BW_CAP_20M;
    } else {
        if (pst_mac_vht_hdl->bit_supported_channel_width == 0) {
            if ((pst_mac_vht_hdl->bit_extend_nss_bw_supp == WLAN_EXTEND_NSS_BW_SUPP0) ||
                (pst_mac_user->en_user_max_cap_nss == WLAN_SINGLE_NSS)) {
                en_bandwidth_cap = WLAN_BW_CAP_80M;
            } else {
                en_bandwidth_cap = WLAN_BW_CAP_160M;
            }
        } else {
            en_bandwidth_cap = WLAN_BW_CAP_160M;
        }
    }
    return en_bandwidth_cap;
}

uint32_t mac_user_init_rom(mac_user_stru *pst_mac_user, uint16_t us_user_idx)
{
    mac_user_set_num_spatial_stream_160M(pst_mac_user, WLAN_NSS_LIMIT);

    /* �鲥�û�portĬ�ϴ�,����aput�����ܳ����������鲥֡ */
    if (pst_mac_user->en_is_multi_user == OAL_TRUE) {
        mac_user_set_port(pst_mac_user, OAL_TRUE);
    }
    return OAL_SUCC;
}

oal_bool_enum_uint8 mac_user_get_port(mac_user_stru *mac_user)
{
    if (mac_user) {
        return mac_user->en_port_valid;
    }

    return OAL_FALSE;
}

#ifdef _PRE_WLAN_FEATURE_11AX

void mac_user_set_he_capable(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_he_capable)
{
    mac_he_hdl_stru *pst_he_hdl = OAL_PTR_NULL;

    if (pst_mac_user != OAL_PTR_NULL) {
        pst_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
        pst_he_hdl->en_he_capable = en_he_capable;
    }
}


oal_bool_enum_uint8 mac_user_get_he_capable(mac_user_stru *pst_mac_user)
{
    mac_he_hdl_stru *pst_he_hdl = OAL_PTR_NULL;

    if (pst_mac_user != OAL_PTR_NULL) {
        pst_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
        return pst_he_hdl->en_he_capable;
    }

    return OAL_FALSE;
}


void mac_user_get_he_hdl(mac_user_stru *pst_mac_user, mac_he_hdl_stru *pst_he_hdl)
{
    if (EOK != memcpy_s((uint8_t *)pst_he_hdl, OAL_SIZEOF(mac_he_hdl_stru),
                        (uint8_t *)(MAC_USER_HE_HDL_STRU(pst_mac_user)), OAL_SIZEOF(mac_he_hdl_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_get_he_hdl::memcpy fail!");
    }
}


void mac_user_set_he_hdl(mac_user_stru *pst_mac_user, mac_he_hdl_stru *pst_he_hdl)
{
    if (EOK != memcpy_s((uint8_t *)(MAC_USER_HE_HDL_STRU(pst_mac_user)),
                        OAL_SIZEOF(mac_he_hdl_stru),
                        (uint8_t *)pst_he_hdl,
                        OAL_SIZEOF(mac_he_hdl_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_set_he_hdl::memcpy fail!");
    }
}

#endif


uint32_t mac_user_add_wep_key(mac_user_stru *pst_mac_user, uint8_t uc_key_index, mac_key_params_stru *pst_key)
{
    int32_t l_ret;

    if (uc_key_index >= WLAN_NUM_TK) {
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    if ((uint32_t)pst_key->key_len > WLAN_WEP104_KEY_LEN) {
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }

    if ((uint32_t)pst_key->seq_len > WLAN_WPA_SEQ_LEN) {
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }

    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_cipher = pst_key->cipher;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_key_len = (uint32_t)pst_key->key_len;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_seq_len = (uint32_t)pst_key->seq_len;

    l_ret = memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_key, WLAN_WPA_KEY_LEN,
                     pst_key->auc_key, (uint32_t)pst_key->key_len);
    l_ret += memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_seq, WLAN_WPA_SEQ_LEN,
                      pst_key->auc_seq, (uint32_t)pst_key->seq_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_add_wep_key::memcpy fail!");
        return OAL_FAIL;
    }

    pst_mac_user->st_user_tx_info.st_security.en_cipher_key_type = WLAN_KEY_TYPE_TX_GTK;

    return OAL_SUCC;
}


uint32_t mac_user_add_rsn_key(mac_user_stru *pst_mac_user, uint8_t uc_key_index, mac_key_params_stru *pst_key)
{
    int32_t l_ret;

    if (uc_key_index >= WLAN_NUM_TK) {
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }
    if ((uint32_t)pst_key->key_len > WLAN_WPA_KEY_LEN) {
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }

    if ((uint32_t)pst_key->seq_len > WLAN_WPA_SEQ_LEN) {
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }

    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_cipher = pst_key->cipher;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_key_len = (uint32_t)pst_key->key_len;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_seq_len = (uint32_t)pst_key->seq_len;

    l_ret = memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_key, WLAN_WPA_KEY_LEN,
                     pst_key->auc_key, (uint32_t)pst_key->key_len);
    l_ret += memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_seq, WLAN_WPA_SEQ_LEN,
                      pst_key->auc_seq, (uint32_t)pst_key->seq_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_add_rsn_key::memcpy fail!");
        return OAL_FAIL;
    }

    pst_mac_user->st_key_info.en_cipher_type = (uint8_t)pst_key->cipher;
    pst_mac_user->st_key_info.uc_default_index = uc_key_index;

    return OAL_SUCC;
}


uint32_t mac_user_add_bip_key(mac_user_stru *pst_mac_user, uint8_t uc_key_index, mac_key_params_stru *pst_key)
{
    int32_t l_ret;

    /* keyidУ�� */
    if (uc_key_index < WLAN_NUM_TK || uc_key_index > WLAN_MAX_IGTK_KEY_INDEX) {
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_cipher = pst_key->cipher;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_key_len = (uint32_t)pst_key->key_len;
    pst_mac_user->st_key_info.ast_key[uc_key_index].ul_seq_len = (uint32_t)pst_key->seq_len;

    l_ret = memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_key, WLAN_WPA_KEY_LEN,
                     pst_key->auc_key, (uint32_t)pst_key->key_len);
    l_ret += memcpy_s(&pst_mac_user->st_key_info.ast_key[uc_key_index].auc_seq, WLAN_WPA_SEQ_LEN,
                      pst_key->auc_seq, (uint32_t)pst_key->seq_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_add_bip_key::memcpy fail!");
        return OAL_FAIL;
    }

    pst_mac_user->st_key_info.uc_igtk_key_index = uc_key_index;

    return OAL_SUCC;
}



uint32_t mac_user_set_port(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_port_valid)
{
    oam_warning_log3(0, OAM_SF_ANY, "mac_user_set_port::user[%d] old %d, new %d",
        pst_mac_user->us_assoc_id, pst_mac_user->en_port_valid, en_port_valid);
    pst_mac_user->en_port_valid = en_port_valid;

    return OAL_SUCC;
}


uint32_t mac_user_init_key(mac_user_stru *pst_mac_user)
{
    memset_s(&pst_mac_user->st_key_info, sizeof(mac_key_mgmt_stru), 0, sizeof(mac_key_mgmt_stru));
    pst_mac_user->st_key_info.en_cipher_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    pst_mac_user->st_key_info.uc_last_gtk_key_idx = 0xFF;
#ifdef _PRE_WLAN_FEATURE_SOFT_CRYPTO
    /* should be a random number, use 100 temporary */
    pst_mac_user->st_key_info.ul_iv = 100;
#endif
    pst_mac_user->st_cap_info.bit_pmf_active = OAL_FALSE;

    return OAL_SUCC;
}


uint32_t mac_user_set_key(mac_user_stru *pst_multiuser,
                                wlan_cipher_key_type_enum_uint8 en_keytype,
                                wlan_ciper_protocol_type_enum_uint8 en_ciphertype,
                                uint8_t uc_keyid)
{
    pst_multiuser->st_user_tx_info.st_security.en_cipher_key_type = en_keytype;
    pst_multiuser->st_user_tx_info.st_security.en_cipher_protocol_type = en_ciphertype;
    pst_multiuser->st_user_tx_info.st_security.uc_cipher_key_id = uc_keyid;
    oam_warning_log4(0, OAM_SF_WPA,
                     "{mac_user_set_key::keytpe==%u, ciphertype==%u, keyid==%u, usridx==%u}",
                     en_keytype, en_ciphertype, uc_keyid, pst_multiuser->us_assoc_id);

    return OAL_SUCC;
}


uint32_t mac_user_init(mac_user_stru *pst_mac_user,
                             uint16_t us_user_idx,
                             uint8_t *puc_mac_addr,
                             uint8_t uc_chip_id,
                             uint8_t uc_device_id,
                             uint8_t uc_vap_id)
{
#ifdef _PRE_WLAN_FEATURE_WMMAC
    uint8_t uc_ac_loop;
#endif

    if (oal_unlikely(pst_mac_user == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_user_init::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ʼ��chip id, device ip, vap id */
    pst_mac_user->uc_chip_id = uc_chip_id;
    pst_mac_user->uc_device_id = uc_device_id;
    pst_mac_user->uc_vap_id = uc_vap_id;
    pst_mac_user->us_assoc_id = us_user_idx;

    /* ��ʼ����Կ */
    pst_mac_user->st_user_tx_info.st_security.en_cipher_key_type = WLAN_KEY_TYPE_PTK;
    pst_mac_user->st_user_tx_info.st_security.en_cipher_protocol_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;

    /* ��ʼ����ȫ������Ϣ */
    mac_user_init_key(pst_mac_user);
    mac_user_set_key(pst_mac_user, WLAN_KEY_TYPE_PTK, WLAN_80211_CIPHER_SUITE_NO_ENCRYP, 0);
    mac_user_set_port(pst_mac_user, OAL_FALSE);
    pst_mac_user->en_user_asoc_state = MAC_USER_STATE_BUTT;

    if (puc_mac_addr == OAL_PTR_NULL) {
        pst_mac_user->en_is_multi_user = OAL_TRUE;
        pst_mac_user->en_user_asoc_state = MAC_USER_STATE_ASSOC;
    } else {
        /* ��ʼ��һ���û��Ƿ����鲥�û��ı�־���鲥�û���ʼ��ʱ������ñ����� */
        pst_mac_user->en_is_multi_user = OAL_FALSE;

        /* ����mac��ַ */
        oal_set_mac_addr(pst_mac_user->auc_user_mac_addr, puc_mac_addr);
    }

    /* ��ʼ������ */
    mac_user_set_pmf_active(pst_mac_user, OAL_FALSE);
    pst_mac_user->st_cap_info.bit_proxy_arp = OAL_FALSE;

    mac_user_set_avail_num_spatial_stream(pst_mac_user, WLAN_SINGLE_NSS);

#ifdef _PRE_WLAN_FEATURE_WMMAC
    /* TS��Ϣ��ʼ�� */
    for (uc_ac_loop = 0; uc_ac_loop < WLAN_WME_AC_BUTT; uc_ac_loop++) {
        pst_mac_user->st_ts_info[uc_ac_loop].uc_up = WLAN_WME_AC_TO_TID(uc_ac_loop);
        pst_mac_user->st_ts_info[uc_ac_loop].en_ts_status = MAC_TS_NONE;
        pst_mac_user->st_ts_info[uc_ac_loop].uc_vap_id = pst_mac_user->uc_vap_id;
        pst_mac_user->st_ts_info[uc_ac_loop].us_mac_user_idx = pst_mac_user->us_assoc_id;
        pst_mac_user->st_ts_info[uc_ac_loop].uc_tsid = 0xFF;
    }
#endif

    return OAL_SUCC;
}


void mac_user_avail_bf_num_spatial_stream(mac_user_stru *pst_mac_user, uint8_t uc_value)
{
    pst_mac_user->en_avail_bf_num_spatial_stream = uc_value;
}


void mac_user_set_avail_num_spatial_stream(mac_user_stru *pst_mac_user, uint8_t uc_value)
{
    pst_mac_user->en_avail_num_spatial_stream = uc_value;
}

void mac_user_set_num_spatial_stream(mac_user_stru *pst_mac_user, uint8_t uc_value)
{
    pst_mac_user->en_user_max_cap_nss = uc_value;
}

void mac_user_set_bandwidth_cap(mac_user_stru *pst_mac_user, wlan_bw_cap_enum_uint8 en_bandwidth_value)
{
    pst_mac_user->en_bandwidth_cap = en_bandwidth_value;
}

void mac_user_set_bandwidth_info(mac_user_stru *pst_mac_user,
                                         wlan_bw_cap_enum_uint8 en_avail_bandwidth,
                                         wlan_bw_cap_enum_uint8 en_cur_bandwidth)
{
    pst_mac_user->en_avail_bandwidth = en_avail_bandwidth;
    pst_mac_user->en_cur_bandwidth = en_cur_bandwidth;

    /* Autorate��Э���11n�л���11b��, cur_bandwidth���Ϊ20M
       ��ʱ�������������Ϊ40M, cur_bandwidth����Ҫ����20M */
    if ((pst_mac_user->en_cur_protocol_mode == WLAN_LEGACY_11B_MODE) &&
        (pst_mac_user->en_cur_bandwidth != WLAN_BW_CAP_20M)) {
        pst_mac_user->en_cur_bandwidth = WLAN_BW_CAP_20M;
    }
}
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)

void mac_user_set_num_spatial_stream_160M(mac_user_stru *pst_mac_user, uint8_t uc_value)
{
    pst_mac_user->st_vht_hdl.bit_user_num_spatial_stream_160M = uc_value;
}


uint8_t mac_user_get_sta_cap_bandwidth_11ac(wlan_channel_band_enum_uint8 en_band,
    mac_user_ht_hdl_stru *pst_mac_ht_hdl, mac_vht_hdl_stru *pst_mac_vht_hdl, mac_user_stru *pst_mac_user)
{
    wlan_bw_cap_enum_uint8        en_bandwidth_cap = WLAN_BW_CAP_20M;

    /* 2.4g band��Ӧ����vht cap��ȡ������Ϣ */
    if ((en_band == WLAN_BAND_2G) && (pst_mac_ht_hdl->en_ht_capable == OAL_TRUE)) {
        en_bandwidth_cap = (pst_mac_ht_hdl->bit_supported_channel_width == WLAN_BW_CAP_40M) ?     \
                            WLAN_BW_CAP_40M : WLAN_BW_CAP_20M;
    } else {
        if (pst_mac_vht_hdl->bit_supported_channel_width == 0) {
            if ((pst_mac_vht_hdl->bit_extend_nss_bw_supp == WLAN_EXTEND_NSS_BW_SUPP0) ||
                (pst_mac_user->en_user_max_cap_nss == WLAN_SINGLE_NSS)) {
                en_bandwidth_cap = WLAN_BW_CAP_80M;
            } else {
                en_bandwidth_cap = WLAN_BW_CAP_160M;
            }
        } else {
            en_bandwidth_cap = WLAN_BW_CAP_160M;
        }
    }
    return en_bandwidth_cap;
}
#endif
#ifdef _PRE_WLAN_FEATURE_11AX

uint8_t mac_user_get_sta_cap_bandwidth_11ax(wlan_channel_band_enum_uint8 en_band,
    mac_he_hdl_stru *pst_mac_he_hdl)
{
    wlan_bw_cap_enum_uint8 en_bandwidth_cap = WLAN_BW_CAP_20M;
    if (en_band == WLAN_BAND_2G) {
        if (pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_channel_width_set & 0x1) { /* Bit0 2G �Ƿ�֧��40MHz */
            en_bandwidth_cap = WLAN_BW_CAP_40M;
        } else {
            en_bandwidth_cap = WLAN_BW_CAP_20M;
        }
    } else {
        if (pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_channel_width_set & 0x4) {
            /* Bit3 ָʾ 160MHz 1105֧�� */
            en_bandwidth_cap = WLAN_BW_CAP_160M;
        } else if (pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_channel_width_set & 0x2) {
            /* Bit2 ָʾ5G 80MHz */
            en_bandwidth_cap = WLAN_BW_CAP_80M;
        } else if (pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_channel_width_set & 0x1) {
            en_bandwidth_cap = WLAN_BW_CAP_40M;
        } else {
            en_bandwidth_cap = WLAN_BW_CAP_20M;
        }
    }
    return en_bandwidth_cap;
}
#endif

void mac_user_get_sta_cap_bandwidth(mac_user_stru *pst_mac_user,
    wlan_bw_cap_enum_uint8 *pen_bandwidth_cap)
{
    mac_user_ht_hdl_stru    *pst_mac_ht_hdl = OAL_PTR_NULL;
    mac_vht_hdl_stru        *pst_mac_vht_hdl = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_11AX
    mac_he_hdl_stru         *pst_mac_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
#endif
    wlan_bw_cap_enum_uint8   en_bandwidth_cap = WLAN_BW_CAP_BUTT;
    mac_vap_stru            *pst_mac_vap;

    pst_mac_vap = mac_res_get_mac_vap(pst_mac_user->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        return;
    }
    /* ��ȡHT��VHT�ṹ��ָ�� */
    pst_mac_ht_hdl = &(pst_mac_user->st_ht_hdl);
    pst_mac_vht_hdl = &(pst_mac_user->st_vht_hdl);
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open && (OAL_TRUE == MAC_USER_IS_HE_USER(pst_mac_user))) {
        en_bandwidth_cap = mac_user_get_sta_cap_bandwidth_11ax(pst_mac_vap->st_channel.en_band, pst_mac_he_hdl);
    } else if (pst_mac_vht_hdl->en_vht_capable == OAL_TRUE)
#else // ut 11AX��δ�������������޸�
    if (pst_mac_vht_hdl->en_vht_capable == OAL_TRUE)
#endif
    {
        en_bandwidth_cap = mac_user_get_sta_cap_bandwidth_11ac(pst_mac_vap->st_channel.en_band,
            pst_mac_ht_hdl, pst_mac_vht_hdl, pst_mac_user);
    } else if (pst_mac_ht_hdl->en_ht_capable == OAL_TRUE) {
        en_bandwidth_cap = (pst_mac_ht_hdl->bit_supported_channel_width == 1) ? WLAN_BW_CAP_40M : WLAN_BW_CAP_20M;
    } else {
        en_bandwidth_cap = WLAN_BW_CAP_20M;
    }

    mac_user_set_bandwidth_cap(pst_mac_user, en_bandwidth_cap);

    /* ������ֵ�ɳ��δ��� */
    *pen_bandwidth_cap = en_bandwidth_cap;
}


void mac_user_set_assoc_id(mac_user_stru *pst_mac_user, uint16_t us_assoc_id)
{
    pst_mac_user->us_assoc_id = us_assoc_id;
}

void mac_user_set_avail_protocol_mode(mac_user_stru *pst_mac_user,
                                              wlan_protocol_enum_uint8 en_avail_protocol_mode)
{
    pst_mac_user->en_avail_protocol_mode = en_avail_protocol_mode;
}

void mac_user_set_cur_protocol_mode(mac_user_stru *pst_mac_user, wlan_protocol_enum_uint8 en_cur_protocol_mode)
{
    pst_mac_user->en_cur_protocol_mode = en_cur_protocol_mode;
}



void mac_user_set_protocol_mode(mac_user_stru *pst_mac_user, wlan_protocol_enum_uint8 en_protocol_mode)
{
    pst_mac_user->en_protocol_mode = en_protocol_mode;
}

void mac_user_set_asoc_state(mac_user_stru *pst_mac_user, mac_user_asoc_state_enum_uint8 en_value)
{
    pst_mac_user->en_user_asoc_state = en_value;
}

void mac_user_set_avail_op_rates(mac_user_stru *pst_mac_user, uint8_t uc_rs_nrates, uint8_t *puc_rs_rates)
{
    int32_t l_ret;

    pst_mac_user->st_avail_op_rates.uc_rs_nrates = uc_rs_nrates;
    l_ret = memcpy_s(pst_mac_user->st_avail_op_rates.auc_rs_rates,
                     WLAN_MAX_SUPP_RATES, puc_rs_rates, WLAN_MAX_SUPP_RATES);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_set_avail_op_rates::memcpy fail!");
    }
}

void mac_user_set_vht_hdl(mac_user_stru *pst_mac_user, mac_vht_hdl_stru *pst_vht_hdl)
{
    int32_t l_ret;

    l_ret = memcpy_s((uint8_t *)(&pst_mac_user->st_vht_hdl), OAL_SIZEOF(mac_vht_hdl_stru),
                     (uint8_t *)pst_vht_hdl, OAL_SIZEOF(mac_vht_hdl_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_set_vht_hdl::memcpy fail!");
    }
}

void mac_user_get_vht_hdl(mac_user_stru *pst_mac_user, mac_vht_hdl_stru *pst_vht_hdl)
{
    int32_t l_ret;

    l_ret = memcpy_s((uint8_t *)pst_vht_hdl, OAL_SIZEOF(mac_vht_hdl_stru),
                     (uint8_t *)(&pst_mac_user->st_vht_hdl), OAL_SIZEOF(mac_vht_hdl_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_get_vht_hdl::memcpy fail!");
    }
}


void mac_user_set_ht_hdl(mac_user_stru *pst_mac_user, mac_user_ht_hdl_stru *pst_ht_hdl)
{
    int32_t l_ret;

    l_ret = memcpy_s((uint8_t *)(&pst_mac_user->st_ht_hdl), OAL_SIZEOF(mac_user_ht_hdl_stru),
                     (uint8_t *)pst_ht_hdl, OAL_SIZEOF(mac_user_ht_hdl_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_set_ht_hdl::memcpy fail!");
    }
}

void mac_user_get_ht_hdl(mac_user_stru *pst_mac_user, mac_user_ht_hdl_stru *pst_ht_hdl)
{
    int32_t l_ret;

    l_ret = memcpy_s((uint8_t *)pst_ht_hdl, OAL_SIZEOF(mac_user_ht_hdl_stru),
                     (uint8_t *)(&pst_mac_user->st_ht_hdl), OAL_SIZEOF(mac_user_ht_hdl_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_user_get_ht_hdl::memcpy fail!");
    }
}


#ifdef _PRE_WLAN_FEATURE_SMPS

void mac_user_set_sm_power_save(mac_user_stru *pst_mac_user, uint8_t uc_sm_power_save)
{
    pst_mac_user->st_ht_hdl.bit_sm_power_save = uc_sm_power_save;
}
#endif


void mac_user_set_pmf_active(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_pmf_active)
{
    pst_mac_user->st_cap_info.bit_pmf_active = en_pmf_active;
}

void mac_user_set_barker_preamble_mode(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_barker_preamble_mode)
{
    pst_mac_user->st_cap_info.bit_barker_preamble_mode = en_barker_preamble_mode;
}


void mac_user_set_qos(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_qos_mode)
{
    pst_mac_user->st_cap_info.bit_qos = en_qos_mode;
}


wlan_priv_key_param_stru *mac_user_get_key(mac_user_stru *pst_mac_user, uint8_t uc_key_id)
{
    if (uc_key_id >= WLAN_NUM_TK + WLAN_NUM_IGTK) {
        return OAL_PTR_NULL;
    }
    return &pst_mac_user->st_key_info.ast_key[uc_key_id];
}



void mac_user_set_cur_bandwidth(mac_user_stru *pst_mac_user, wlan_bw_cap_enum_uint8 en_cur_bandwidth)
{
    pst_mac_user->en_cur_bandwidth = en_cur_bandwidth;
}



void mac_user_set_ht_capable(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_ht_capable)
{
    pst_mac_user->st_ht_hdl.en_ht_capable = en_ht_capable;
}



void mac_user_set_spectrum_mgmt(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_spectrum_mgmt)
{
    pst_mac_user->st_cap_info.bit_spectrum_mgmt = en_spectrum_mgmt;
}

void mac_user_set_apsd(mac_user_stru *pst_mac_user, oal_bool_enum_uint8 en_apsd)
{
    pst_mac_user->st_cap_info.bit_apsd = en_apsd;
}


uint32_t mac_user_update_wep_key(mac_user_stru *pst_mac_usr, uint16_t us_multi_user_idx)
{
    mac_user_stru *pst_multi_user = OAL_PTR_NULL;
    int32_t      l_ret;

    MAC_11I_ASSERT(pst_mac_usr != OAL_PTR_NULL, OAL_ERR_CODE_PTR_NULL);

    pst_multi_user = (mac_user_stru *)mac_res_get_mac_user(us_multi_user_idx);
    if (pst_multi_user == OAL_PTR_NULL) {
        return OAL_ERR_CODE_SECURITY_USER_INVAILD;
    }

    if (pst_multi_user->st_key_info.en_cipher_type != WLAN_80211_CIPHER_SUITE_WEP_104 &&
        pst_multi_user->st_key_info.en_cipher_type != WLAN_80211_CIPHER_SUITE_WEP_40) {
        oam_error_log1(0, OAM_SF_WPA, "{mac_wep_add_usr_key::en_cipher_type==%d}",
                       pst_multi_user->st_key_info.en_cipher_type);
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    if (pst_multi_user->st_key_info.uc_default_index >= WLAN_MAX_WEP_KEY_COUNT) {
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    /* wep�����£������鲥�û�����Կ��Ϣ�������û� */
    l_ret = memcpy_s(&pst_mac_usr->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru),
                     &pst_multi_user->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_WPA, "mac_user_update_wep_key::memcpy fail!");
        return OAL_FAIL;
    }

    /* TBD ������ϢҪŲ��ȥ */
    pst_mac_usr->st_user_tx_info.st_security.en_cipher_key_type =
        pst_mac_usr->st_key_info.uc_default_index + HAL_KEY_TYPE_PTK;  // ��ȡWEP default key id

    return OAL_SUCC;
}

oal_bool_enum_uint8 mac_addr_is_zero(const unsigned char *puc_mac)
{
    uint8_t auc_mac_zero[OAL_MAC_ADDR_LEN] = {0};

    MAC_11I_ASSERT((puc_mac != OAL_PTR_NULL), OAL_TRUE);

    return (0 == oal_memcmp(auc_mac_zero, puc_mac, OAL_MAC_ADDR_LEN));
}


void *mac_res_get_mac_user(uint16_t us_idx)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;

    pst_mac_user = (mac_user_stru *)_mac_res_get_mac_user(us_idx);
    if (pst_mac_user == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* �쳣: �û���Դ�ѱ��ͷ� */
    if (pst_mac_user->uc_is_user_alloced != MAC_USER_ALLOCED) {
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
        /*lint -e718*/ /*lint -e746*/
        oam_warning_log2(0, OAM_SF_UM,
                         "{mac_res_get_mac_user::[E]user has been freed,user_idx=%d, func[%x].}",
                         us_idx, (uint32_t)__return_address());
        /*lint +e718*/ /*lint +e746*/
#endif
        /* device���ȡ�û�ʱ�û��Ѿ��ͷ��������������ؿ�ָ�룬
         * ���������߲����û�ʧ�ܣ����ӡWARNING��ֱ���ͷ�buf����������֧�ȵ�
         */
        return OAL_PTR_NULL;
    }

    return (void *)pst_mac_user;
}

/*lint -e19*/
oal_module_symbol(mac_user_get_key);
oal_module_symbol(mac_user_set_port);

oal_module_symbol(mac_user_set_key);
oal_module_symbol(mac_user_init);
oal_module_symbol(mac_user_set_avail_num_spatial_stream);
oal_module_symbol(mac_user_set_num_spatial_stream);
oal_module_symbol(mac_user_set_bandwidth_cap);
oal_module_symbol(mac_user_get_sta_cap_bandwidth);

oal_module_symbol(mac_user_set_bandwidth_info);
oal_module_symbol(mac_user_set_assoc_id);
oal_module_symbol(mac_user_set_protocol_mode);
oal_module_symbol(mac_user_set_avail_protocol_mode);
oal_module_symbol(mac_user_set_cur_protocol_mode);
oal_module_symbol(mac_user_set_cur_bandwidth);

oal_module_symbol(mac_user_avail_bf_num_spatial_stream);
oal_module_symbol(mac_user_set_asoc_state);
oal_module_symbol(mac_user_set_avail_op_rates);
oal_module_symbol(mac_user_set_vht_hdl);
oal_module_symbol(mac_user_get_vht_hdl);
oal_module_symbol(mac_user_set_ht_hdl);
oal_module_symbol(mac_user_get_ht_hdl);
oal_module_symbol(mac_user_set_ht_capable);
#ifdef _PRE_WLAN_FEATURE_SMPS
oal_module_symbol(mac_user_set_sm_power_save);
#endif
oal_module_symbol(mac_user_set_pmf_active);
oal_module_symbol(mac_user_set_barker_preamble_mode);
oal_module_symbol(mac_user_set_qos);
oal_module_symbol(mac_user_set_spectrum_mgmt);
oal_module_symbol(mac_user_set_apsd);

oal_module_symbol(mac_user_init_key);
oal_module_symbol(mac_user_update_wep_key);
oal_module_symbol(mac_addr_is_zero);
oal_module_symbol(mac_res_get_mac_user); /*lint +e19*/


