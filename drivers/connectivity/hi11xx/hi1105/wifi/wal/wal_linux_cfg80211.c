

/* 1 ͷ�ļ����� */
#include "wlan_types.h"

#include "oal_net.h"
#include "oal_cfg80211.h"
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "plat_pm_wlan.h"

#include "wlan_chip_i.h"
#include "mac_device.h"
#include "mac_vap.h"
#include "mac_ie.h"
#include "mac_resource.h"
#include "hmac_device.h"
#include "hmac_resource.h"
#include "hmac_ext_if.h"
#include "hmac_vap.h"
#include "hmac_p2p.h"
#include "hmac_dfs.h"
#include "hmac_roam_main.h"

#include "wal_linux_cfg80211.h"
#include "wal_linux_event.h"
#include "wal_ext_if.h"
#include "wal_regdb.h"
#include "wal_linux_cfgvendor.h"
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
#include "wal_dfx.h"
#endif

#include "securec.h"
#include "securectype.h"
#include "hmac_tx_data.h"
#include "wal_linux_netdev_ops.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID              OAM_FILE_ID_WAL_LINUX_CFG80211_C
#define WAL_BCN_BSSID_LENGTH_ADDR (37)
/* 2 ȫ�ֱ������� */
#ifdef _PRE_WLAN_FEATURE_DFR
extern hmac_dfr_info_stru g_st_dfr_info;
#endif  // _PRE_WLAN_FEATURE_DFR

#define hi1151_a_rates      (hi1151_rates + 4)
#define hi1151_a_rates_size 8
#define hi1151_g_rates      (hi1151_rates + 0)
#define hi1151_g_rates_size 12

/* �豸֧�ֵ����� */
OAL_STATIC oal_ieee80211_rate hi1151_rates[] = {
    RATETAB_ENT(10, 0x1, 0),
    RATETAB_ENT(20, 0x2, 0),
    RATETAB_ENT(55, 0x4, 0),
    RATETAB_ENT(110, 0x8, 0),
    RATETAB_ENT(60, 0x10, 0),
    RATETAB_ENT(90, 0x20, 0),
    RATETAB_ENT(120, 0x40, 0),
    RATETAB_ENT(180, 0x80, 0),
    RATETAB_ENT(240, 0x100, 0),
    RATETAB_ENT(360, 0x200, 0),
    RATETAB_ENT(480, 0x400, 0),
    RATETAB_ENT(540, 0x800, 0),
};

/* 2.4G Ƶ�� */
OAL_STATIC oal_ieee80211_channel g_ast_supported_channels_2ghz_info[] = {
    CHAN2G(1, 2412, 0),
    CHAN2G(2, 2417, 0),
    CHAN2G(3, 2422, 0),
    CHAN2G(4, 2427, 0),
    CHAN2G(5, 2432, 0),
    CHAN2G(6, 2437, 0),
    CHAN2G(7, 2442, 0),
    CHAN2G(8, 2447, 0),
    CHAN2G(9, 2452, 0),
    CHAN2G(10, 2457, 0),
    CHAN2G(11, 2462, 0),
    CHAN2G(12, 2467, 0),
    CHAN2G(13, 2472, 0),
    CHAN2G(14, 2484, 0),
};

/* 5G Ƶ�� */
OAL_STATIC oal_ieee80211_channel g_ast_supported_channels_5ghz_info[] = {
    CHAN5G(36, 0),
    CHAN5G(40, 0),
    CHAN5G(44, 0),
    CHAN5G(48, 0),
    CHAN5G(52, 0),
    CHAN5G(56, 0),
    CHAN5G(60, 0),
    CHAN5G(64, 0),
    CHAN5G(100, 0),
    CHAN5G(104, 0),
    CHAN5G(108, 0),
    CHAN5G(112, 0),
    CHAN5G(116, 0),
    CHAN5G(120, 0),
    CHAN5G(124, 0),
    CHAN5G(128, 0),
    CHAN5G(132, 0),
    CHAN5G(136, 0),
    CHAN5G(140, 0),
    CHAN5G(144, 0),
    CHAN5G(149, 0),
    CHAN5G(153, 0),
    CHAN5G(157, 0),
    CHAN5G(161, 0),
    CHAN5G(165, 0),
    /* 4.9G */
    CHAN4_9G(184, 0),
    CHAN4_9G(188, 0),
    CHAN4_9G(192, 0),
    CHAN4_9G(196, 0),
};

/* �豸֧�ֵļ����׼� */
OAL_STATIC const uint32_t g_ast_wlan_supported_cipher_suites[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP,
#ifdef _PRE_WLAN_FEATURE_GCMP_256_CCMP256
    WLAN_CIPHER_SUITE_GCMP,
    WLAN_CIPHER_SUITE_GCMP_256,
    WLAN_CIPHER_SUITE_CCMP_256,
#endif
    WLAN_CIPHER_SUITE_AES_CMAC,
    WLAN_CIPHER_SUITE_BIP_CMAC_256,
    WLAN_CIPHER_SUITE_SMS4,
    WLAN_CIPHER_SUITE_BIP_GMAC_256,
};

extern OAL_CONST wal_ioctl_mode_map_stru g_ast_mode_map[];

/* 2.4G Ƶ����Ϣ */
oal_ieee80211_supported_band g_st_supported_band_2ghz_info = {
    .channels = g_ast_supported_channels_2ghz_info,
    .n_channels = sizeof(g_ast_supported_channels_2ghz_info) / sizeof(oal_ieee80211_channel),
    .bitrates = hi1151_g_rates,
    .n_bitrates = hi1151_g_rates_size,
    .ht_cap = {
        .ht_supported = OAL_TRUE,
        .cap = IEEE80211_HT_CAP_SUP_WIDTH_20_40 | IEEE80211_HT_CAP_SGI_20 | IEEE80211_HT_CAP_SGI_40,
    },
};

/* 5G Ƶ����Ϣ */
OAL_STATIC oal_ieee80211_supported_band g_st_supported_band_5ghz_info = {
    .channels = g_ast_supported_channels_5ghz_info,
    .n_channels = sizeof(g_ast_supported_channels_5ghz_info) / sizeof(oal_ieee80211_channel),
    .bitrates = hi1151_a_rates,
    .n_bitrates = hi1151_a_rates_size,
    .ht_cap = {
        .ht_supported = OAL_TRUE,
        .cap = IEEE80211_HT_CAP_SUP_WIDTH_20_40 | IEEE80211_HT_CAP_SGI_20 | IEEE80211_HT_CAP_SGI_40,
    },
    .vht_cap = {
        .vht_supported = OAL_TRUE,
        .cap = IEEE80211_VHT_CAP_SHORT_GI_80 | IEEE80211_VHT_CAP_HTC_VHT |
               IEEE80211_VHT_CAP_SHORT_GI_160 | IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ,
    },
};

OAL_STATIC oal_ieee80211_iface_limit sta_p2p_limits[] = {
    {
        .max = 2,
        .types = BIT(NL80211_IFTYPE_STATION),
    },
    {
        .max = 2,
        .types = BIT(NL80211_IFTYPE_P2P_GO) | BIT(NL80211_IFTYPE_P2P_CLIENT),
    },
    {
        .max = 1,
        .types = BIT(NL80211_IFTYPE_P2P_DEVICE),
    },
};

OAL_STATIC oal_ieee80211_iface_combination sta_p2p_iface_combinations[] = {
    {
        .num_different_channels = 2,
        .max_interfaces = 3,
        .limits = sta_p2p_limits,
        .n_limits = oal_array_size(sta_p2p_limits),
    },
};

/* There isn't a lot of sense in it, but you can transmit anything you like */
static const struct ieee80211_txrx_stypes
    wal_cfg80211_default_mgmt_stypes[NUM_NL80211_IFTYPES] = {
    [NL80211_IFTYPE_ADHOC] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_STATION] = {
        .tx = 0xffff, .rx = BIT(IEEE80211_STYPE_ACTION >> 4)
        | BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
#ifdef _PRE_WLAN_FEATURE_SAE
        | BIT(IEEE80211_STYPE_AUTH >> 4)
#endif
    },
    [NL80211_IFTYPE_AP] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |  //lint !e572
        BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) | BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
        BIT(IEEE80211_STYPE_DISASSOC >> 4) | BIT(IEEE80211_STYPE_AUTH >> 4) |
        BIT(IEEE80211_STYPE_DEAUTH >> 4) | BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_AP_VLAN] = { /* copy AP */
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |  //lint !e572
        BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) | BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
        BIT(IEEE80211_STYPE_DISASSOC >> 4) | BIT(IEEE80211_STYPE_AUTH >> 4) |
        BIT(IEEE80211_STYPE_DEAUTH >> 4) | BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_P2P_CLIENT] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) | BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
    },
    [NL80211_IFTYPE_P2P_GO] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |  //lint !e572
        BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) | BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
        BIT(IEEE80211_STYPE_DISASSOC >> 4) | BIT(IEEE80211_STYPE_AUTH >> 4) |
        BIT(IEEE80211_STYPE_DEAUTH >> 4) | BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_P2P_DEVICE] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) | BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
    },
};

uint8_t g_uc_cookie_array_bitmap = 0; /* ÿ��bit ��ʾcookie array ���Ƿ�ʹ�ã�1 - ��ʹ�ã�0 - δʹ�� */
cookie_arry_stru g_cookie_array[WAL_COOKIE_ARRAY_SIZE];

/* 3 ����ʵ�� */

OAL_STATIC uint32_t wal_find_wmm_uapsd(uint8_t *puc_wmm_ie)
{
    /* �ж� WMM UAPSD �Ƿ�ʹ�� */
    if (puc_wmm_ie[1] < MAC_WMM_QOS_INFO_POS) {
        return OAL_FALSE;
    }

    if (puc_wmm_ie[MAC_WMM_QOS_INFO_POS] & BIT7) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_hide_ssid(uint8_t *puc_ssid_ie, uint8_t uc_ssid_len)
{
    return (oal_bool_enum_uint8)((puc_ssid_ie == OAL_PTR_NULL) || (uc_ssid_len == 0) || (puc_ssid_ie[0] == '\0'));
}


uint32_t wal_cfg80211_open_wmm(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_open_wmm::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �������vap������ */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_CONFIG) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_open_wmm::this is config vap! can't get info.}");
        return OAL_FAIL;
    }

    /* host��,����WMM������mib��Ϣλ�е�Qosλ�� */
    return hmac_config_open_wmm(pst_mac_vap, us_len, puc_param);
}


void wal_parse_wpa_wpa2_ie(oal_beacon_parameters *pst_beacon_info,
                           mac_beacon_param_stru *pst_beacon_param)
{
    oal_ieee80211_mgmt *pst_mgmt;
    uint16_t us_capability_info;
    uint8_t *puc_ie_tmp = OAL_PTR_NULL;
    int32_t l_ret = EOK;

    /* �ж��Ƿ���� */
    pst_mgmt = (oal_ieee80211_mgmt *)pst_beacon_info->head;

    us_capability_info = pst_mgmt->u.beacon.capab_info;
    pst_beacon_param->en_privacy = OAL_FALSE;

    if (us_capability_info & WLAN_WITP_CAPABILITY_PRIVACY) {
        pst_beacon_param->en_privacy = OAL_TRUE;

        /* ���� RSN ��ϢԪ�� */
        puc_ie_tmp = mac_find_ie(MAC_EID_RSN, pst_beacon_info->tail, pst_beacon_info->tail_len);
        if (puc_ie_tmp != OAL_PTR_NULL) {
            l_ret += memcpy_s(pst_beacon_param->auc_rsn_ie, MAC_MAX_RSN_LEN,
                puc_ie_tmp, MAC_IE_HDR_LEN + puc_ie_tmp[1]);
        }

        /* ���� WPA ��ϢԪ�أ�����������֤���� */
        puc_ie_tmp = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_OUITYPE_WPA,
            pst_beacon_info->tail, pst_beacon_info->tail_len);
        if (puc_ie_tmp != OAL_PTR_NULL) {
            l_ret += memcpy_s(pst_beacon_param->auc_wpa_ie, MAC_MAX_RSN_LEN,
                puc_ie_tmp, MAC_IE_HDR_LEN + puc_ie_tmp[1]);
        }
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_parse_wpa_wpa2_ie::memcpy fail!");
    }
}


uint32_t wal_parse_wmm_ie(oal_net_device_stru *pst_dev,
                          mac_vap_stru *pst_mac_vap, oal_beacon_parameters *pst_beacon_info)
{
    uint8_t *puc_wmm_ie;
    uint16_t us_len = OAL_SIZEOF(uint8_t);
    uint8_t uc_wmm = OAL_TRUE;
    uint32_t ul_ret = OAL_SUCC;

    uint8_t uc_uapsd = OAL_TRUE;
    wal_msg_write_stru st_write_msg;

    /*  ����wmm_ie  */
    puc_wmm_ie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WMM,
                                    pst_beacon_info->tail, pst_beacon_info->tail_len);
    if (puc_wmm_ie == OAL_PTR_NULL) {
        /* wmm ieδ�ҵ�����˵��wmm �� */
        uc_wmm = OAL_FALSE;
    /*  �ҵ�wmm ie��˳���ж���uapsd�Ƿ�ʹ�� */
    } else {
        if (OAL_FALSE == wal_find_wmm_uapsd(puc_wmm_ie)) {
            /* ��ӦUAPSD �� */
            uc_uapsd = OAL_FALSE;
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_parse_wmm_ie::uapsd is disabled!!}");
        }

        /* ��д msg ��Ϣͷ */
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_UAPSD_EN, OAL_SIZEOF(uc_uapsd));

        /* ��д msg ��Ϣ�� */
        /* �ù���Ŀǰ��������������ʵ�֣�������Ҫ����ϲ����ã�TBD */
        if (IS_P2P_GO(pst_mac_vap)) {
            uc_uapsd = WLAN_FEATURE_UAPSD_IS_OPEN;
            oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                          "{wal_parse_wmm_ie:: (1103)It is a Go, set uapsd = WLAN_FEATURE_UAPSD_IS_OPEN)}");
        }
        oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                      "{wal_parse_wmm_ie:: uapsd = %d)}", uc_uapsd);

        if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(uc_uapsd), &uc_uapsd, OAL_SIZEOF(uc_uapsd))) {
            oam_error_log0(0, OAM_SF_TX, "wal_parse_wmm_ie::memcpy fail!");
            return OAL_FAIL;
        }

        /* ������Ϣ */
        ul_ret = (uint32_t)wal_send_cfg_event(pst_dev, WAL_MSG_TYPE_WRITE,
                                              WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uc_uapsd),
                                              (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            ul_ret = OAL_FAIL;
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                "{wal_parse_wmm_ie::uapsd switch set failed[%d].}", ul_ret);
        }
    }
    /*  wmm ����/�ر� ���  */
    ul_ret = wal_cfg80211_open_wmm(pst_mac_vap, us_len, &uc_wmm);
    if (ul_ret != OAL_SUCC) {
        ul_ret = OAL_FAIL;
        oam_warning_log0(0, OAM_SF_TX, "{wal_parse_wmm_ie::can not open wmm!}\r\n");
    }

    return ul_ret;
}


uint32_t wal_cfg80211_add_vap(mac_cfg_add_vap_param_stru *pst_add_vap_param)
{
    
    int32_t l_ret;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;

    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;

    mac_vap_stru *pst_cfg_mac_vap;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device;
    wlan_vap_mode_enum_uint8 en_vap_mode = pst_add_vap_param->en_vap_mode;
    wlan_p2p_mode_enum_uint8 en_p2p_mode = pst_add_vap_param->en_p2p_mode;

    /* ��ȡmac device */
    pst_net_dev = pst_add_vap_param->pst_net_dev;
    pst_wdev = pst_net_dev->ieee80211_ptr;
    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->uc_cfg_vap_id);
    if (oal_unlikely(pst_cfg_mac_vap == OAL_PTR_NULL)) {
        oam_warning_log1(0, 0, "{wal_cfg80211_add_vap::cfg_mac_vap null vap_id:%d}", pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (oal_unlikely(pst_cfg_hmac_vap == OAL_PTR_NULL)) {
        oam_warning_log1(0, 0, "{wal_cfg80211_add_vap::cfg_hmac_vap null vapid:%d}", pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (oal_unlikely(pst_cfg_net_dev == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_add_vap::pst_mac_device is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_wdev->iftype = NL80211_IFTYPE_AP;
    } else if (en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_wdev->iftype = NL80211_IFTYPE_STATION;
    }
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_DEVICE;
    } else if (en_p2p_mode == WLAN_P2P_CL_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_CLIENT;
    } else if (en_p2p_mode == WLAN_P2P_GO_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_GO;
    }

    oal_netdevice_flags(pst_net_dev) &= ~OAL_IFF_RUNNING; /* ��net device��flag��Ϊdown */

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_vap_mode = en_vap_mode;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->uc_cfg_vap_indx = pst_cfg_mac_vap->uc_vap_id;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->is_primary_vap = 0;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wal_vap_get_wlan_mode_para(&st_write_msg);
#endif
    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
                               (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_cfg80211_add_vap::return err code %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_add_vap::hmac add vap fail, ul_err_code[%u]!}\r\n", ul_err_code);
        return ul_err_code;
    }

    wal_set_random_mac_to_mib(pst_net_dev); /* set random mac to mib ; for hi1102-cb */

    return OAL_SUCC;
}


uint32_t wal_cfg80211_del_vap(mac_cfg_del_vap_param_stru *pst_del_vap_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (oal_unlikely(pst_del_vap_param == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_vap::pst_del_vap_param null ptr !}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_net_dev = pst_del_vap_param->pst_net_dev;

    /* �豸��up״̬������ɾ����������down */
    if (oal_unlikely(0 != (OAL_IFF_RUNNING & oal_netdevice_flags(pst_net_dev)))) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_del_vap::device is busy, please down it first %d!}\r\n",
            oal_netdevice_flags(pst_net_dev));
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_del_vap::can't get mac vap from netdevice priv data!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �豸��up״̬������ɾ����������down */
    if ((pst_mac_vap->en_vap_state != MAC_VAP_STATE_INIT) &&
        (pst_mac_vap->en_vap_state != MAC_VAP_STATE_STA_SCAN_COMP)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_del_vap::device is busy, please down it first %d!}\r\n",
            pst_mac_vap->en_vap_state);
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    /* ����Ҫ�ͷ�net_device �ṹ�µ�wireless_dev ��Ա */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    /* ��ʼ��ɾ��vap ���� */
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = pst_del_vap_param->en_p2p_mode;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

    if (OAL_SUCC != wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                                       (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_vap::wal_send_cfg_event fail!}");
        return OAL_EFAIL;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_vap::wal_check_and_release_msg_resp fail!}");
        return OAL_EFAIL;
    }

    return OAL_SUCC;
}


uint32_t wal_cfg80211_vowifi_report(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    frw_event_stru *pst_event;
    hmac_vap_stru *pst_hmac_vap;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_vowifi_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_TX, "{wal_cfg80211_vowifi_report::pst_hmac_vap null.vap_id[%d]}",
            pst_event->st_event_hdr.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �ϱ�vowifi�л����� */
    oal_cfg80211_vowifi_report(pst_hmac_vap->pst_net_device, GFP_KERNEL);
#endif /* (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34)) */

    return OAL_SUCC;
}


uint32_t wal_cfg80211_cac_report(frw_event_mem_stru *pst_event_mem)
{
    return OAL_SUCC;
}


OAL_STATIC oal_bool_enum wal_check_support_basic_rate_6M(uint8_t *puc_supported_rates_ie,
                                                         uint8_t uc_supported_rates_num,
                                                         uint8_t *puc_extended_supported_rates_ie,
                                                         uint8_t uc_extended_supported_rates_num)
{
    uint8_t uc_loop;
    oal_bool_enum en_support = OAL_FALSE;
    for (uc_loop = 0; uc_loop < uc_supported_rates_num; uc_loop++) {
        if (puc_supported_rates_ie == OAL_PTR_NULL) {
            break;
        }
        if (puc_supported_rates_ie[2 + uc_loop] == 0x8c) {
            en_support = OAL_TRUE;
        }
    }

    for (uc_loop = 0; uc_loop < uc_extended_supported_rates_num; uc_loop++) {
        if (puc_extended_supported_rates_ie == OAL_PTR_NULL) {
            break;
        }
        if (puc_extended_supported_rates_ie[2 + uc_loop] == 0x8c) {
            en_support = OAL_TRUE;
        }
    }

    return en_support;
}

OAL_STATIC OAL_INLINE uint32_t wal_parse_protocol_mode_rate_nums_check(uint8_t uc_rate_nums,
    uint8_t uc_supported_rates_num, uint8_t uc_extended_supported_rates_num, wlan_protocol_enum_uint8 *pen_protocol)
{
    if (uc_rate_nums < uc_supported_rates_num) {
        oam_error_log2(0, 0, "{wal_parse_protocol_mode::rate_num[%d], ext_rate_num[%d]}",
            uc_supported_rates_num, uc_extended_supported_rates_num);
        *pen_protocol = WLAN_PROTOCOL_BUTT;
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_parse_protocol_mode_by_vht_ht_ie(uint8_t *puc_vht_ie,
    uint8_t *puc_ht_ie, wlan_protocol_enum_uint8 *pen_protocol)
{
    if (puc_vht_ie != OAL_PTR_NULL) {
        /* ����AP Ϊ11ac ģʽ */
        *pen_protocol = WLAN_VHT_MODE;
        return OAL_SUCC;
    }
    if (puc_ht_ie != OAL_PTR_NULL) {
        /* ����AP Ϊ11n ģʽ */
        *pen_protocol = WLAN_HT_MODE;
        return OAL_SUCC;
    }
    return OAL_FAIL;
}


OAL_STATIC uint32_t wal_parse_protocol_mode(wlan_channel_band_enum_uint8 en_band,
    oal_beacon_parameters *pst_beacon_info, uint8_t *puc_ht_ie, uint8_t *puc_vht_ie,
    uint8_t *puc_he_ie, wlan_protocol_enum_uint8 *pen_protocol, uint8_t ul_he_flag)
{
    uint8_t *puc_supported_rates_ie = OAL_PTR_NULL;
    uint8_t *puc_extended_supported_rates_ie = OAL_PTR_NULL;
    uint8_t uc_supported_rates_num = 0;
    uint8_t uc_extended_supported_rates_num = 0;
    uint16_t us_offset;
    uint8_t uc_rate_nums = 0;
    if ((puc_he_ie != OAL_PTR_NULL) || (ul_he_flag == OAL_TRUE)) {
#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            /* ����AP Ϊ11axģʽ */
            *pen_protocol = WLAN_HE_MODE;
            return OAL_SUCC;
        }
#endif
    }
    if (wal_parse_protocol_mode_by_vht_ht_ie(puc_vht_ie, puc_ht_ie, pen_protocol) == OAL_SUCC) {
        return OAL_SUCC;
    }

    /* hostapd ������Ƶ�Σ�������add beacon �����add beacon�����У�����ʹ��Ƶ����Ϣ��������Ϣ����AP ģʽ(a/b/g) */
    if (en_band == WLAN_BAND_5G) {
        *pen_protocol = WLAN_LEGACY_11A_MODE;
        return OAL_SUCC;
    }
    if (en_band == WLAN_BAND_2G) {
        us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
        puc_supported_rates_ie = mac_find_ie(MAC_EID_RATES,
            pst_beacon_info->head + 24 + us_offset, pst_beacon_info->head_len - us_offset);
        if (puc_supported_rates_ie != OAL_PTR_NULL) {
            uc_supported_rates_num = puc_supported_rates_ie[1];
        }
        puc_extended_supported_rates_ie =
            mac_find_ie(MAC_EID_XRATES, pst_beacon_info->tail, pst_beacon_info->tail_len);
        if (puc_extended_supported_rates_ie != OAL_PTR_NULL) {
            uc_extended_supported_rates_num = puc_extended_supported_rates_ie[1];
        }

        uc_rate_nums = uc_supported_rates_num + uc_extended_supported_rates_num;
        // �˴���ֱ��ʹ��uc_supported_rates_num + uc_extended_supported_rates_num�𲻵����ȱ������ã��Ի����
        if (OAL_FAIL == wal_parse_protocol_mode_rate_nums_check(uc_rate_nums, uc_supported_rates_num,
            uc_extended_supported_rates_num, pen_protocol)){
            return OAL_FAIL;
        }

        if (uc_rate_nums == 4) {
            *pen_protocol = WLAN_LEGACY_11B_MODE;
            return OAL_SUCC;
        } else if (uc_rate_nums == 8) {
            *pen_protocol = WLAN_LEGACY_11G_MODE;
            return OAL_SUCC;
        } else if (uc_rate_nums == 12) {
            /* ���ݻ�����������Ϊ 11gmix1 ���� 11gmix2 */
            /* ����������ʼ�֧�� 6M , ���ж�Ϊ 11gmix2 */
            *pen_protocol = WLAN_MIXED_ONE_11G_MODE;
            if (OAL_TRUE == wal_check_support_basic_rate_6M(puc_supported_rates_ie, uc_supported_rates_num,
                puc_extended_supported_rates_ie, uc_extended_supported_rates_num)) {
                *pen_protocol = WLAN_MIXED_TWO_11G_MODE;
            }
            return OAL_SUCC;
        }
    }

    /* �����������Ϊ���ò����� */
    *pen_protocol = WLAN_PROTOCOL_BUTT;

    return OAL_FAIL;
}


OAL_STATIC uint8_t* wal_parse_he_ie(mac_vap_stru *pst_mac_vap, uint8_t *puc_flag,
    oal_beacon_parameters  *pst_beacon_info)
{
    uint8_t *puc_he_ie = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        /* ���ƻ����ؿ���05 device��������axģʽ��03������axģʽ */
        if (g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_switch == OAL_TRUE) {
            *puc_flag = OAL_TRUE;
        }
        /* ͨ��hostapd�·�ax���� */
        puc_he_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_CAP, pst_beacon_info->tail, pst_beacon_info->tail_len);
    }
#endif
    return puc_he_ie;
}


OAL_STATIC void wal_he_flag_handle(mac_vap_stru *pst_mac_vap, uint8_t **ppuc_he_ie, uint8_t *ul_he_flag)
{
    mac_device_stru *p_mac_dev = OAL_PTR_NULL;
    if (g_wlan_spec_cfg->feature_slave_ax_is_support == OAL_TRUE) {
        return;
    }

    if (*ppuc_he_ie == OAL_PTR_NULL && *ul_he_flag == OAL_FALSE) {
        return;
    }

    p_mac_dev = (mac_device_stru *)mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (p_mac_dev == OAL_PTR_NULL) {
        return;
    }

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_he_flag_handle::p2p_scenes = %d!}\r\n",
        p_mac_dev->st_p2p_info.p2p_scenes);
    if (p_mac_dev->st_p2p_info.p2p_scenes == MAC_P2P_SCENES_LOW_LATENCY &&
        OAL_TRUE == mac_vap_can_not_start_he_protocol(pst_mac_vap)) {
        *ppuc_he_ie = OAL_PTR_NULL;
        *ul_he_flag = OAL_FALSE;
    }
}

OAL_STATIC void wal_go_protocol_handle(mac_vap_stru *pst_mac_vap, mac_beacon_param_stru *pst_beacon_param)
{
    if (!IS_P2P_GO(pst_mac_vap)) {
        return;
    }
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP || pst_mac_vap->en_vap_state == MAC_VAP_STATE_PAUSE) {
        pst_beacon_param->en_protocol = pst_mac_vap->en_protocol;
        return;
    }
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (pst_beacon_param->en_protocol == WLAN_HE_MODE) {
            return;
        }
    }
#endif

    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        pst_beacon_param->en_protocol =
            ((pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_TRUE) ? WLAN_VHT_MODE : WLAN_HT_MODE);
    }
    if (OAL_TRUE == mac_vap_avoid_dbac_close_vht_protocol(pst_mac_vap)) {
        pst_beacon_param->en_protocol = WLAN_HT_MODE;
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_go_protocol_handle::close vht = %d!}\r\n",
            pst_beacon_param->en_protocol);
    }
}


OAL_STATIC uint32_t wal_parse_ht_vht_ie(mac_vap_stru *pst_mac_vap,
                                        oal_beacon_parameters *pst_beacon_info,
                                        mac_beacon_param_stru *pst_beacon_param)
{
    uint8_t *puc_ht_ie, *puc_vht_ie;
    uint8_t *puc_he_ie = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_frame_ht_cap_stru *pst_ht_cap = OAL_PTR_NULL;
    mac_vht_cap_info_stru *pst_vht_cap = OAL_PTR_NULL;
    uint8_t ul_he_flag = OAL_FALSE;

    puc_ht_ie = mac_find_ie(MAC_EID_HT_CAP, pst_beacon_info->tail, pst_beacon_info->tail_len);
    puc_vht_ie = mac_find_ie(MAC_EID_VHT_CAP, pst_beacon_info->tail, pst_beacon_info->tail_len);
    puc_he_ie = wal_parse_he_ie(pst_mac_vap, &ul_he_flag, pst_beacon_info);
    wal_he_flag_handle(pst_mac_vap, &puc_he_ie, &ul_he_flag);

    /* ����Э��ģʽ */
    ul_ret = wal_parse_protocol_mode(pst_mac_vap->st_channel.en_band, pst_beacon_info,
        puc_ht_ie, puc_vht_ie, puc_he_ie, &pst_beacon_param->en_protocol, ul_he_flag);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_parse_ht_vht_ie::return err code!}\r\n", ul_ret);
        return ul_ret;
    }

    wal_go_protocol_handle(pst_mac_vap, pst_beacon_param);

    /* ���ht vht��Ϊ�գ����� */
    if ((puc_ht_ie == OAL_PTR_NULL) && (puc_vht_ie == OAL_PTR_NULL)) {
        return OAL_SUCC;
    }
    /* ����short gi���� */
    if (puc_ht_ie != OAL_PTR_NULL) {
        /* ʹ��ht cap ie���������2���ֽ� */
        if (puc_ht_ie[1] < OAL_SIZEOF(mac_frame_ht_cap_stru)) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{wal_parse_ht_vht_ie::invalid ht cap ie len[%d]!}\r\n", puc_ht_ie[1]);
            return OAL_SUCC;
        }

        pst_ht_cap = (mac_frame_ht_cap_stru *)(puc_ht_ie + MAC_IE_HDR_LEN);

        pst_beacon_param->en_shortgi_20 = (uint8_t)pst_ht_cap->bit_short_gi_20mhz;
        pst_beacon_param->en_shortgi_40 = 0;

        if ((pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_20M) &&
            (pst_mac_vap->st_channel.en_bandwidth != WLAN_BAND_WIDTH_BUTT)) {
            pst_beacon_param->en_shortgi_40 = (uint8_t)pst_ht_cap->bit_short_gi_40mhz;
        }
    }

    if (puc_vht_ie == OAL_PTR_NULL) {
        return OAL_SUCC;
    }

    /* ʹ��vht cap ie���������4���ֽ� */
    if (puc_vht_ie[1] < OAL_SIZEOF(mac_vht_cap_info_stru)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "{wal_parse_ht_vht_ie::invalid ht cap ie len[%d]!}", puc_vht_ie[1]);
        return OAL_SUCC;
    }

    pst_vht_cap = (mac_vht_cap_info_stru *)(puc_vht_ie + MAC_IE_HDR_LEN);

    pst_beacon_param->en_shortgi_80 = 0;

    if ((pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_40MINUS) &&
        (pst_mac_vap->st_channel.en_bandwidth != WLAN_BAND_WIDTH_BUTT)) {
        pst_beacon_param->en_shortgi_80 = pst_vht_cap->bit_short_gi_80mhz;
    }

    return OAL_SUCC;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
OAL_STATIC int32_t wal_cfg80211_sched_scan_stop(oal_wiphy_stru *pst_wiphy,
                                                oal_net_device_stru *pst_netdev,
                                                uint64_t ul_reqid);

#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
OAL_STATIC int32_t wal_cfg80211_sched_scan_stop(oal_wiphy_stru *pst_wiphy,
                                                oal_net_device_stru *pst_netdev);
#else
/* do nothing */
#endif


OAL_STATIC int32_t wal_p2p_stop_roc(mac_vap_stru *pst_mac_vap, oal_net_device_stru *pst_netdev)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device;
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{wal_p2p_stop_roc:: pst_hmac_device[%d] null!}\r\n", pst_mac_vap->uc_device_id);
        return -OAL_EFAIL;
    }

    /* tx mgmt roc ���ȼ���,���Ա��Լ���80211 roc�Լ�80211 scan��� */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        if (pst_hmac_device->st_scan_mgmt.en_is_scanning != OAL_TRUE) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
                "{wal_p2p_stop_roc::not in scan state but vap is listen state!}");
            return OAL_SUCC;
        }

        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
        if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{wal_p2p_stop_roc:: pst_hmac_vap null!}\r\n");
            return -OAL_EFAIL;
        }
        pst_hmac_vap->en_wait_roc_end = OAL_TRUE;
        oal_init_completion(&(pst_hmac_vap->st_roc_end_ready));
        wal_force_scan_complete(pst_netdev, OAL_TRUE);
        if (0 == oal_wait_for_completion_timeout(&(pst_hmac_vap->st_roc_end_ready),
            (uint32_t)oal_msecs_to_jiffies(200))) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{wal_p2p_stop_roc::cancel old roc timout!}");
            return -OAL_EFAIL;
        }
    }
    return OAL_SUCC;
}


OAL_STATIC mac_vap_stru *wal_find_another_listen_vap(mac_device_stru *pst_mac_device,
    uint8_t uc_vap_id_self)
{
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            continue;
        }

        if (uc_vap_id_self == pst_mac_vap->uc_vap_id) {
            continue;
        }

        if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
            return pst_mac_vap;
        }
    }

    return OAL_PTR_NULL;
}


OAL_STATIC void wal_another_vap_stop_p2p_listen(mac_device_stru *pst_mac_device, uint8_t uc_self_vap_id)
{
    mac_vap_stru *pst_listen_mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_listen_hmac_vap = OAL_PTR_NULL;
    int32_t l_ret;

    pst_listen_mac_vap = wal_find_another_listen_vap(pst_mac_device, uc_self_vap_id);
    if (pst_listen_mac_vap == OAL_PTR_NULL) {
        oam_info_log0(uc_self_vap_id, OAM_SF_CFG,
            "{wal_another_vap_stop_p2p_listen::not find another p2p listen vap.}");
        return;
    }

    pst_listen_hmac_vap = mac_res_get_hmac_vap(pst_listen_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_listen_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log1(uc_self_vap_id, OAM_SF_CFG,
            "{wal_another_vap_stop_p2p_listen::fail to get hmac vap, listen vap id[%d]}",
            pst_listen_mac_vap->uc_vap_id);
        return;
    }

    oam_warning_log1(uc_self_vap_id, OAM_SF_CFG,
        "{wal_another_vap_stop_p2p_listen::begin to stop p2p listen, listen vap id[%d]}",
        pst_listen_mac_vap->uc_vap_id);

    l_ret = wal_p2p_stop_roc(pst_listen_mac_vap, pst_listen_hmac_vap->pst_net_device);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(uc_self_vap_id, OAM_SF_CFG,
            "{wal_another_vap_stop_p2p_listen::fail to stop p2p listen, listen vap id[%d]}",
            pst_listen_mac_vap->uc_vap_id);
        return;
    }
}



OAL_STATIC OAL_INLINE oal_bool_enum_uint8 wal_cfg80211_scan_param_check(oal_wiphy_stru *pst_wiphy,
    oal_cfg80211_scan_request_stru *pst_request, oal_net_device_stru *pst_netdev)
{
    if (oal_any_null_ptr2(pst_wiphy, pst_request)) {
        oam_error_log2(0, OAM_SF_CFG,
            "{wal_cfg80211_scan_param_check::scan failed, null ptr, pst_wiphy[%p], pst_request[%p]!}",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_request);
        return OAL_FALSE;
    }

    /* �ж�ɨ�贫���ڴ泤�Ȳ��ܴ��ں�������ռ��С�����⿽���ڴ�Խ�� */
    if (pst_request->ie_len > WLAN_WPS_IE_MAX_SIZE) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_scan_param_check:: scan ie is too large to save. [%d]!}",
            pst_request->ie_len);
        return OAL_FALSE;
    }

    pst_netdev = pst_request->wdev->netdev;
    if (pst_netdev == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_scan_param_check::scan failed! pst_netdev[%p] null}",
            (uintptr_t)pst_netdev);
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 wal_cfg80211_dfr_and_s3s4_param_check(void)
{
#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_dfr_and_s3s4_param_check::dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return OAL_FALSE;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

#ifdef  _PRE_CONFIG_HISI_S3S4_POWER_STATE
    if (g_st_recovery_info.device_s3s4_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_dfr_and_s3s4_param_check::s3s4_process_status[%d]!}",
                         g_st_recovery_info.device_s3s4_process_flag);
        return OAL_FALSE;
    }
#endif

    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE void wal_cfg80211_scan_sched_scan_stop(hmac_scan_stru *pst_scan_mgmt,
    oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev, mac_vap_stru *pst_mac_vap)
{
    /* �����ǰ����ɨ�������У�����ͣ����ɨ�� */
    if (pst_scan_mgmt->pst_sched_scan_req != OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_scan_sched_scan_stop::stop sched scan,before normal scan}");
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
        wal_cfg80211_sched_scan_stop(pst_wiphy, pst_netdev, 0);
#else
        wal_cfg80211_sched_scan_stop(pst_wiphy, pst_netdev);
#endif
    }
}



OAL_STATIC int32_t wal_cfg80211_scan(oal_wiphy_stru *pst_wiphy,
                                       oal_cfg80211_scan_request_stru *pst_request)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_scan_stru *pst_scan_mgmt = OAL_PTR_NULL;
    int32_t l_ret;
    oal_net_device_stru *pst_netdev = OAL_PTR_NULL;

    if (wal_cfg80211_scan_param_check(pst_wiphy, pst_request, pst_netdev) == OAL_FALSE) {
        return -OAL_EFAIL;
    }

    pst_netdev = pst_request->wdev->netdev;

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_scan:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_device_stru �ṹ */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_scan::scan failed! pst_mac_vap null!}");
        return -OAL_EFAIL;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    if (IS_P2P_SCAN_REQ(pst_request) && (OAL_TRUE == hmac_user_is_wapi_connected(pst_mac_vap->uc_device_id))) {
        oam_warning_log0(0, OAM_SF_CFG, "{stop p2p scan under wapi!}");
        goto fail;
    }
#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_scan::hmac_device null device_id:%d!}", pst_mac_vap->uc_device_id);
        goto fail;
    }
    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
        "{wal_cfg80211_scan::start a new normal scan. n_channels[%d], ie_len[%d], n_ssid[%d]}",
        pst_request->n_channels, pst_request->ie_len, pst_request->n_ssids);

    /* TBD: gong �������ص�ɨ��, ���ϴ�δ��������Ҫ����busy  */
    /* �ȴ��ϴε�ɨ��������� */
    /*lint -e730*/ /* info, boolean argument to function */
    l_ret = oal_wait_event_interruptible_m(pst_scan_mgmt->st_wait_queue, (OAL_PTR_NULL == pst_scan_mgmt->pst_request));
    /*lint +e730*/
    if (l_ret < 0) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_scan::start a new scan failed, wait return error.}");
        /*lint -e801*/
        goto fail;
        /*lint +e801*/
    }

    /* p2p normal scan�Լ����Դ���Լ���һ�ε�roc */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_cfg80211_scan::stop roc scan, before normal scan.}");
        if (wal_p2p_stop_roc(pst_mac_vap, pst_netdev) < 0) {
            /*lint -e801*/
            goto fail;
            /*lint +e801*/
        }
    }

    if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
        /* ���������VAP����P2P listen״̬������ȡ��listen */
        wal_another_vap_stop_p2p_listen(pst_hmac_device->pst_device_base_info, pst_mac_vap->uc_vap_id);
    }

    l_ret = (pst_mac_vap->en_vap_state >= MAC_VAP_STATE_STA_JOIN_COMP &&
             pst_mac_vap->en_vap_state <= MAC_VAP_STATE_STA_WAIT_ASOC);
    if (l_ret) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_cfg80211_scan::stop normal scan while connecting.}");
        /*lint -e801*/
        goto fail;
        /*lint +e801*/
    }

    /* ���浱ǰ�ں��·���ɨ�����󵽱��� */
    pst_scan_mgmt->pst_request = pst_request;
    wal_cfg80211_scan_sched_scan_stop(pst_scan_mgmt, pst_wiphy, pst_netdev, pst_mac_vap);

    /* ����ɨ�� */
    if (OAL_SUCC != wal_scan_work_func(pst_scan_mgmt, pst_netdev, pst_request)) {
        pst_scan_mgmt->pst_request = OAL_PTR_NULL;
        return -OAL_EFAIL;
    }

    return OAL_SUCC;

fail:
    return -OAL_EFAIL;
}


OAL_STATIC void wal_set_crypto_info(mac_conn_param_stru *pst_conn_param, oal_cfg80211_conn_stru *pst_sme)
{
    uint8_t *puc_ie_tmp = OAL_PTR_NULL;
    uint8_t *puc_wep_key = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_WAPI
    if (pst_sme->crypto.wpa_versions == WITP_WAPI_VERSION) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_cfg80211_connect::crypt ver is wapi!");
        pst_conn_param->uc_wapi = OAL_TRUE;
    } else {
        pst_conn_param->uc_wapi = OAL_FALSE;
    }
#endif

    if (pst_sme->privacy) {
        if ((pst_sme->key_len != 0) && (pst_sme->key != OAL_PTR_NULL)) {
            /* ����wep������Ϣ */
            pst_conn_param->uc_wep_key_len = pst_sme->key_len;
            pst_conn_param->uc_wep_key_index = pst_sme->key_idx;

            puc_wep_key = (uint8_t *)oal_memalloc(pst_sme->key_len);
            if (puc_wep_key == OAL_PTR_NULL) {
                oam_error_log2(0, OAM_SF_CFG, "{wal_set_crypto_info::puc_wep_key(%d)(%p) alloc mem return null ptr!}",
                               (uint32_t)(pst_sme->key_len), (uintptr_t)((uint8_t *)pst_sme->key));
                return;
            }
            if (EOK != memcpy_s(puc_wep_key, (uint32_t)pst_sme->key_len,
                                (uint8_t *)pst_sme->key, (uint32_t)(pst_sme->key_len))) {
                oam_error_log0(0, OAM_SF_CFG, "wal_set_crypto_info::memcpy fail!");
                oal_free(puc_wep_key);
                return;
            }
            pst_conn_param->puc_wep_key = puc_wep_key;

            return;
        }

        /* ���Ȳ��� RSN ��ϢԪ�� */
        puc_ie_tmp = mac_find_ie(MAC_EID_RSN, (uint8_t *)pst_sme->ie, (int32_t)pst_sme->ie_len);
        if (puc_ie_tmp != OAL_PTR_NULL) {
            mac_ie_get_rsn_cipher(puc_ie_tmp, &pst_conn_param->st_crypto);

            oam_warning_log2(0, OAM_SF_WPA, "wal_set_crypto_info::get_rsn_cipher pair_suite[0]:0x%x pair_suite[1]:0x%x",
                             pst_conn_param->st_crypto.aul_pair_suite[0], pst_conn_param->st_crypto.aul_pair_suite[1]);
        } else {
            /* WPA */
            puc_ie_tmp = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_OUITYPE_WPA,
                (uint8_t *)pst_sme->ie, (int32_t)pst_sme->ie_len);
            if (puc_ie_tmp != OAL_PTR_NULL) {
                mac_ie_get_wpa_cipher(puc_ie_tmp, &pst_conn_param->st_crypto);
                oam_warning_log2(0, OAM_SF_WPA,
                    "wal_set_crypto_info::get_wpa_cipher pair_suite[0]:0x%x pair_suite[1]:0x%x",
                    pst_conn_param->st_crypto.aul_pair_suite[0], pst_conn_param->st_crypto.aul_pair_suite[1]);
            }
        }
    }
}


OAL_STATIC oal_bool_enum_uint8 wal_is_p2p_device(oal_net_device_stru *pst_net_device)
{
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;

    pst_mac_vap = oal_net_dev_priv(pst_net_device);
    if (pst_mac_vap == OAL_PTR_NULL) {
        return OAL_TRUE;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if ((pst_hmac_vap != OAL_PTR_NULL) &&
        (pst_hmac_vap->pst_p2p0_net_device != OAL_PTR_NULL) &&
        (pst_net_device == pst_hmac_vap->pst_p2p0_net_device)) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}


OAL_STATIC void free_connect_param_resource(mac_conn_param_stru *pst_conn_param)
{
    if (pst_conn_param->puc_wep_key != OAL_PTR_NULL) {
        oal_free(pst_conn_param->puc_wep_key);
        pst_conn_param->puc_wep_key = OAL_PTR_NULL;
    }
    if (pst_conn_param->puc_ie != OAL_PTR_NULL) {
        oal_free(pst_conn_param->puc_ie);
        pst_conn_param->puc_ie = OAL_PTR_NULL;
    }
}


OAL_STATIC int32_t wal_cfg80211_connect(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_net_device,
    oal_cfg80211_conn_stru *pst_sme)
{
    mac_conn_param_stru st_mac_conn_param;
    int32_t l_ret;
    uint8_t *puc_ie = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_wiphy, pst_net_device, pst_sme)) {
        oam_error_log3(0, OAM_SF_ANY,
            "{wal_cfg80211_connect::connect failed, pst_wiphy, pst_netdev, pst_sme %p, %p, %p!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device, (uintptr_t)pst_sme);

        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_connect:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    if (wal_is_p2p_device(pst_net_device)) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_cfg80211_connect:connect stop, p2p device should not connect.");
        return -OAL_EINVAL;
    }
    /* ��ʼ���������Ӳ��� */
    memset_s(&st_mac_conn_param, OAL_SIZEOF(mac_conn_param_stru), 0, OAL_SIZEOF(mac_conn_param_stru));

    /* �����ں��·��� ssid */
    st_mac_conn_param.uc_ssid_len = (uint8_t)pst_sme->ssid_len;
    l_ret = memcpy_s(st_mac_conn_param.auc_ssid, OAL_SIZEOF(st_mac_conn_param.auc_ssid),
                     (uint8_t *)pst_sme->ssid, st_mac_conn_param.uc_ssid_len);

    if (pst_sme->bssid) {
        if (EOK != memcpy_s(st_mac_conn_param.auc_bssid, OAL_MAC_ADDR_LEN,
                            (uint8_t *)pst_sme->bssid, OAL_MAC_ADDR_LEN)) {
            oam_error_log0(0, OAM_SF_ASSOC, "wal_cfg80211_connect::memcpy fail!");
            return -OAL_EFAIL;
        }
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0))
    else if (pst_sme->bssid_hint) {
        if (EOK != memcpy_s(st_mac_conn_param.auc_bssid, OAL_MAC_ADDR_LEN,
                            (uint8_t *)pst_sme->bssid_hint, OAL_MAC_ADDR_LEN)) {
            oam_error_log0(0, OAM_SF_ASSOC, "wal_cfg80211_connect::memcpy fail!");
            return -OAL_EFAIL;
        }
    }
#endif
    else {
        oam_warning_log0(0, OAM_SF_ASSOC, "{wal_cfg80211_connect::bssid and bssid_hint is NULL.}");
        return -OAL_EFAIL;
    }
    /* �����ں��·��İ�ȫ��ز��� */
    /* ������֤���� */
    st_mac_conn_param.en_auth_type = pst_sme->auth_type;

    /* ���ü������� */
    st_mac_conn_param.en_privacy = pst_sme->privacy;

    /* ��ȡ�ں��·���pmf��ʹ�ܵĽ�� */
    st_mac_conn_param.en_mfp = pst_sme->mfp;

    if (pst_sme->channel != OAL_PTR_NULL) {
        st_mac_conn_param.uc_channel = oal_ieee80211_frequency_to_channel(pst_sme->channel->center_freq);
    }

    oam_warning_log4(0, OAM_SF_ANY,
        "{wal_cfg80211_connect::start a new connect, ssid_len[%d], auth_type[%d], privacy[%d], mfp[%d]}\r\n",
        pst_sme->ssid_len, pst_sme->auth_type, pst_sme->privacy, pst_sme->mfp);

    /* ���ü��ܲ��� */
    wal_set_crypto_info(&st_mac_conn_param, pst_sme);
    if ((st_mac_conn_param.puc_wep_key == OAL_PTR_NULL) && (pst_sme->key_len > 0) && (pst_sme->key != OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{wal_cfg80211_connect::puc_wep_key is NULL.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ���ù���P2P/WPS ie */
    st_mac_conn_param.ul_ie_len = (uint32_t)(pst_sme->ie_len);
    if ((st_mac_conn_param.ul_ie_len > 0) && (pst_sme->ie != OAL_PTR_NULL)) {
        puc_ie = (uint8_t *)oal_memalloc(pst_sme->ie_len);
        if (puc_ie == OAL_PTR_NULL) {
            oam_error_log2(0, OAM_SF_CFG, "{wal_scan_work_func::puc_ie(%d)(%p) alloc mem return null ptr!}",
                           (uint32_t)(pst_sme->ie_len), (uintptr_t)(uint8_t *)pst_sme->ie);
            free_connect_param_resource(&st_mac_conn_param);
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
        l_ret += memcpy_s(puc_ie, pst_sme->ie_len, (uint8_t *)pst_sme->ie, st_mac_conn_param.ul_ie_len);
        st_mac_conn_param.puc_ie = puc_ie;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ASSOC, "wal_cfg80211_connect::memcpy fail!");
        free_connect_param_resource(&st_mac_conn_param);
        return -OAL_EFAIL;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_set_timeout(WLAN_SLEEP_LONG_CHECK_CNT);
#endif

    
    wal_force_scan_abort_then_scan_comp(pst_net_device);

    
    /* ���¼����������������� */
    l_ret = wal_cfg80211_start_connect(pst_net_device, &st_mac_conn_param);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_connect::wal_cfg80211_start_connect fail %d!}\r\n", l_ret);
        /* �¼��·�ʧ����WAL�ͷţ��·��ɹ�������succ fail HMAC���ͷ� */
        if (oal_value_ne_all2(l_ret, -OAL_EFAIL, -OAL_ETIMEDOUT)) {
            free_connect_param_resource(&st_mac_conn_param);
        }
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_disconnect(oal_wiphy_stru *pst_wiphy,
                                             oal_net_device_stru *pst_net_device,
                                             uint16_t us_reason_code)
{
    mac_cfg_kick_user_param_stru st_mac_cfg_kick_user_param;
    int32_t l_ret;

    mac_user_stru *pst_mac_user = NULL;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_any_null_ptr2(pst_wiphy, pst_net_device)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_cfg80211_disconnect::input param pointer is null,pst_wiphy, pst_netdev %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device);
        return -OAL_EINVAL;
    }

    oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_disconnect:: deauth us_reason_code=%d}", us_reason_code);

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_disconnect:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_device);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_disconnect::mac vap has already been del!}");
        oal_io_print("wal_cfg80211_disconnect:: mac vap has already been del!ifname %s\r\n", pst_net_device->name);
        return OAL_SUCC;
    }

    /* ��д��sta������ap mac ��ַ */
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfg80211_disconnect::mac_res_get_mac_user pst_mac_user is null, user idx[%d]!}\r\n",
            pst_mac_vap->us_assoc_vap_id);
        return OAL_SUCC;
    }

    /* �����ں��·���connect���� */
    memset_s(&st_mac_cfg_kick_user_param, OAL_SIZEOF(mac_cfg_kick_user_param_stru),
             0, OAL_SIZEOF(mac_cfg_kick_user_param_stru));

    /* �����ں��·���ȥ����ԭ��  */
    st_mac_cfg_kick_user_param.us_reason_code = us_reason_code;

    memcpy_s(st_mac_cfg_kick_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN,
             pst_mac_user->auc_user_mac_addr, WLAN_MAC_ADDR_LEN);

    l_ret = wal_cfg80211_start_disconnect(pst_net_device, &st_mac_cfg_kick_user_param);
    if (l_ret != OAL_SUCC) {
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_add_key(oal_wiphy_stru *pst_wiphy,
                                          oal_net_device_stru *pst_netdev,
                                          uint8_t uc_key_index,
                                          bool en_pairwise,
                                          OAL_CONST uint8_t *puc_mac_addr,
                                          oal_key_params_stru *pst_params)
{
    wal_msg_write_stru write_msg;
    mac_addkey_param_stru payloadParams;
    wal_msg_stru *p_rsp_msg = NULL;
    int32_t ret = EOK;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_params)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_cfg80211_add_key::Param Check ERROR,pst_wiphy, pst_netdev, pst_params %x, \
            %x, %x!}\r\n", (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)pst_params);
        return -OAL_EINVAL;
    }

    /* 1.2 key���ȼ�飬��ֹ����Խ�� */
    if ((pst_params->key_len > OAL_WPA_KEY_LEN) || (pst_params->key_len < 0) ||
        (pst_params->seq_len > OAL_WPA_SEQ_LEN) || (pst_params->seq_len < 0)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_add_key::Param Check ERROR! key_len[%x]  seq_len[%x]!}\r\n",
                       (int32_t)pst_params->key_len, (int32_t)pst_params->seq_len);
        return -OAL_EINVAL;
    }

    /* 2.1 ��Ϣ����׼�� */
    memset_s(&payloadParams, OAL_SIZEOF(payloadParams), 0, OAL_SIZEOF(payloadParams));
    payloadParams.uc_key_index = uc_key_index;

    memset_s(payloadParams.auc_mac_addr, WLAN_MAC_ADDR_LEN, 0, WLAN_MAC_ADDR_LEN);
    if (puc_mac_addr != OAL_PTR_NULL) {
        /* ����ʹ���ں��·���macָ�룬���ܱ��ͷţ���Ҫ������������ʹ�� */
        ret += memcpy_s(payloadParams.auc_mac_addr, WLAN_MAC_ADDR_LEN, puc_mac_addr, WLAN_MAC_ADDR_LEN);
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44)
    payloadParams.en_pairwise = en_pairwise;
#else
    payloadParams.en_pairwise = (puc_mac_addr != OAL_PTR_NULL) ? OAL_TRUE : OAL_FALSE;
#endif
#else
    payloadParams.en_pairwise = en_pairwise;
#endif

    /* 2.2 ��ȡ�����Կֵ */
    payloadParams.st_key.key_len = pst_params->key_len;
    payloadParams.st_key.seq_len = pst_params->seq_len;
    payloadParams.st_key.cipher = pst_params->cipher;
    ret += memcpy_s(payloadParams.st_key.auc_key, OAL_WPA_KEY_LEN, pst_params->key, (uint32_t)pst_params->key_len);
    oam_warning_log4(0, OAM_SF_ANY, "{wal_cfg80211_add_key::key_len:%d, key:0X%X, cipher:0X%X, ret=0X%X!}\r\n",
                  pst_params->key_len, pst_params->key, pst_params->cipher, ret);
    ret += memcpy_s(payloadParams.st_key.auc_seq, OAL_WPA_SEQ_LEN, pst_params->seq, (uint32_t)pst_params->seq_len);
    oam_warning_log4(0, OAM_SF_ANY, "{wal_cfg80211_add_key::puc_mac_addr:0X%X, seq_len:%d, seq:0X%X, ret =0X%X}\r\n",
                  puc_mac_addr, pst_params->seq_len, pst_params->seq, ret);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    write_msg.en_wid = WLAN_CFGID_ADD_KEY;
    write_msg.us_len = OAL_SIZEOF(mac_addkey_param_stru);

    /* 3.2 ��д msg ��Ϣ�� */
    ret += memcpy_s(write_msg.auc_value, OAL_SIZEOF(mac_addkey_param_stru),
        &payloadParams, OAL_SIZEOF(mac_addkey_param_stru));
    if (ret != EOK) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfg80211_add_key::memcpy fail! ret[%X]", ret);
    }

    /* ������Ϣ��ʹ���˾ֲ�����ָ�룬�����Ҫ�����͸ú�������Ϊͬ��������hmac����ʱ��ʹ��Ұָ�� */
    if (OAL_SUCC != wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
                                           WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_addkey_param_stru),
                                           (uint8_t *)&write_msg, OAL_TRUE, &p_rsp_msg)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_key::wal_send_cfg_event fail!}");
        return -OAL_EFAIL;
    }
    if (OAL_SUCC != wal_check_and_release_msg_resp(p_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_key::wal_check_and_release_msg_resp fail!}");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_get_key(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    uint8_t uc_key_index, bool en_pairwise, OAL_CONST uint8_t *puc_mac_addr,
    void *cookie, void (*callback)(void *cookie, oal_key_params_stru *))
{
    wal_msg_write_stru st_write_msg;
    mac_getkey_param_stru st_payload_params = { 0 };
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];
    wal_msg_stru *pst_rsp_msg = NULL;
    int32_t l_ret = EOK;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr4(pst_wiphy, pst_netdev, cookie, callback)) {
        oam_error_log4(0, OAM_SF_ANY,
            "{wal_cfg80211_get_key::Param ERROR, wiphy, netdev, cookie, callback %x, %x, %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)cookie, (uintptr_t)callback);
        return -OAL_EINVAL;
    }

    /* 2.1 ��Ϣ����׼�� */
    st_payload_params.pst_netdev = pst_netdev;
    st_payload_params.uc_key_index = uc_key_index;

    if (puc_mac_addr != OAL_PTR_NULL) {
        /* ����ʹ���ں��·���macָ�룬���ܱ��ͷţ���Ҫ������������ʹ�� */
        l_ret += memcpy_s(auc_mac_addr, WLAN_MAC_ADDR_LEN, puc_mac_addr, WLAN_MAC_ADDR_LEN);
        st_payload_params.puc_mac_addr = auc_mac_addr;
    } else {
        st_payload_params.puc_mac_addr = OAL_PTR_NULL;
    }

    st_payload_params.en_pairwise = en_pairwise;
    st_payload_params.cookie = cookie;
    st_payload_params.callback = callback;

    oam_info_log2(0, OAM_SF_ANY, "{wal_cfg80211_get_key::key_idx:%d, en_pairwise:%d!}\r\n",
        uc_key_index, st_payload_params.en_pairwise);
    if (puc_mac_addr != OAL_PTR_NULL) {
        oam_info_log4(0, OAM_SF_ANY, "{wal_cfg80211_get_key::MAC ADDR: %02X:XX:XX:%02X:%02X:%02X!}\r\n",
                      puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
    } else {
        oam_info_log0(0, OAM_SF_ANY, "{wal_cfg80211_get_key::MAC ADDR IS null!}\r\n");
    }
    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    st_write_msg.en_wid = WLAN_CFGID_GET_KEY;
    st_write_msg.us_len = OAL_SIZEOF(mac_getkey_param_stru);

    /* 3.2 ��д msg ��Ϣ�� */
    l_ret += memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_getkey_param_stru),
                      &st_payload_params, OAL_SIZEOF(mac_getkey_param_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_get_key::memcpy fail!");
        return -OAL_EINVAL;
    }

    /* ������Ϣ��ʹ���˾ֲ�����ָ�룬�����Ҫ�����͸ú�������Ϊͬ��������hmac����ʱ��ʹ��Ұָ�� */
    if (OAL_SUCC != wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
                                           WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_getkey_param_stru),
                                           (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_get_key::wal_send_cfg_event fail.}");
        return -OAL_EINVAL;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_get_key::wal_check_and_release_msg_resp fail.}");
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_remove_key(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    uint8_t uc_key_index, bool en_pairwise, OAL_CONST uint8_t *puc_mac_addr)
{
    mac_removekey_param_stru st_payload_params = { 0 };
    wal_msg_write_stru st_write_msg = { 0 };
    wal_msg_stru *pst_rsp_msg = NULL;
    int32_t l_ret = EOK;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_wiphy, pst_netdev)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_remove_key::Param Check ERROR,pst_wiphy, pst_netdev %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_remove_key:: dfr or s3s4 is processing!}");
        return OAL_SUCC;
    }

    /* 2.1 ��Ϣ����׼�� */
    st_payload_params.uc_key_index = uc_key_index;
    memset_s(st_payload_params.auc_mac_addr, OAL_MAC_ADDR_LEN, 0, OAL_MAC_ADDR_LEN);
    if (puc_mac_addr != OAL_PTR_NULL) {
        /* ����ʹ���ں��·���macָ�룬���ܱ��ͷţ���Ҫ������������ʹ�� */
        l_ret += memcpy_s(st_payload_params.auc_mac_addr, OAL_MAC_ADDR_LEN, puc_mac_addr, WLAN_MAC_ADDR_LEN);
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    st_payload_params.en_pairwise = en_pairwise;
#else
    st_payload_params.en_pairwise = en_pairwise;
#endif

    oam_info_log2(0, OAM_SF_ANY, "{wal_cfg80211_remove_key::uc_key_index:%d, en_pairwise:%d!}\r\n",
        uc_key_index, st_payload_params.en_pairwise);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    st_write_msg.en_wid = WLAN_CFGID_REMOVE_KEY;
    st_write_msg.us_len = OAL_SIZEOF(mac_removekey_param_stru);

    /* 3.2 ��д msg ��Ϣ�� */
    l_ret += memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_removekey_param_stru),
                      &st_payload_params, OAL_SIZEOF(mac_removekey_param_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_remove_key::memcpy fail!");
        return -OAL_EFAIL;
    }

    if (OAL_SUCC != wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
                                           WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_removekey_param_stru),
                                           (uint8_t *)&st_write_msg,
                                           OAL_TRUE, &pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_remove_key::wal_send_cfg_event fail.}");
        return -OAL_EFAIL;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_remove_key::wal_check_and_release_msg_resp fail.}");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_set_default_key(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    uint8_t uc_key_index, bool en_unicast, bool en_multicast)
{
    mac_setdefaultkey_param_stru st_payload_params = { 0 };
    int32_t l_ret;
    wal_msg_write_stru st_write_msg = { 0 };

    /* 1.1 ��μ�� */
    if (oal_unlikely(oal_any_null_ptr2(pst_wiphy, pst_netdev))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_set_default_key::wiphy or netdev ptr is null, error %x, %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    /* 2.1 ��Ϣ����׼�� */
    st_payload_params.uc_key_index = uc_key_index;
    st_payload_params.en_unicast = en_unicast;
    st_payload_params.en_multicast = en_multicast;

    oam_info_log3(0, OAM_SF_ANY, "{wal_cfg80211_set_default_key::key_index:%d, unicast:%d, multicast:%d!}\r\n",
                  uc_key_index, st_payload_params.en_unicast, st_payload_params.en_multicast);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    st_write_msg.en_wid = WLAN_CFGID_DEFAULT_KEY;
    st_write_msg.us_len = OAL_SIZEOF(mac_setdefaultkey_param_stru);

    /* 3.2 ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_setdefaultkey_param_stru),
                        &st_payload_params, OAL_SIZEOF(mac_setdefaultkey_param_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_set_default_key::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_setdefaultkey_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_default_key::wal_send_cfg_event return err %d}", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


int32_t wal_cfg80211_set_default_mgmt_key(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    uint8_t uc_key_index)
{
    mac_setdefaultkey_param_stru st_payload_params = { 0 };
    int32_t l_ret;
    wal_msg_write_stru st_write_msg = { 0 };

    /* 1.1 ��μ�� */
    if (oal_unlikely(oal_any_null_ptr2(pst_wiphy, pst_netdev))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_set_default_mgmt_key::wiphy or netdev null,error %x, %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    /* 2.1 ��Ϣ����׼�� */
    st_payload_params.uc_key_index = uc_key_index;
    st_payload_params.en_unicast = OAL_FALSE;
    st_payload_params.en_multicast = OAL_TRUE;

    oam_info_log3(0, OAM_SF_ANY, "{wal_cfg80211_set_default_mgmt_key::key_index:%d, unicast:%d, multicast:%d!}\r\n",
                  uc_key_index, st_payload_params.en_unicast, st_payload_params.en_multicast);

    /***************************************************************************
    ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    st_write_msg.en_wid = WLAN_CFGID_DEFAULT_KEY;
    st_write_msg.us_len = OAL_SIZEOF(mac_setdefaultkey_param_stru);

    /* 3.2 ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_setdefaultkey_param_stru),
                        &st_payload_params, OAL_SIZEOF(mac_setdefaultkey_param_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_set_default_mgmt_key::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_netdev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_setdefaultkey_param_stru),
                               (uint8_t *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfg80211_set_default_mgmt_key::wal_send_cfg_event return err code %d!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

uint8_t wal_cfg80211_convert_value_to_vht_width(int32_t l_channel_value)
{
    uint8_t uc_channel_vht_width = WLAN_MIB_VHT_OP_WIDTH_20_40;

    switch (l_channel_value) {
        case 20:
        case 40:
            uc_channel_vht_width = WLAN_MIB_VHT_OP_WIDTH_20_40;
            break;
        case 80:
            uc_channel_vht_width = WLAN_MIB_VHT_OP_WIDTH_80;
            break;
        case 160:
            uc_channel_vht_width = WLAN_MIB_VHT_OP_WIDTH_160;
            break;
        default:
            break;
    }

    return uc_channel_vht_width;
}


OAL_STATIC int32_t wal_cfg80211_set_wiphy_params(oal_wiphy_stru *pst_wiphy, uint32_t ul_changed)
{
    /* ͨ��HOSTAPD ����RTS ���ޣ���Ƭ���� ���ýӿ�wal_ioctl_set_frag�� wal_ioctl_set_rts */
    oam_warning_log0(0, OAM_SF_CFG,
        "{wal_cfg80211_set_wiphy_params::should not call this function.call wal_ioctl_set_frag/wal_ioctl_set_rts!}");
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_set_ssid(oal_net_device_stru *pst_netdev,
                                         uint8_t *puc_ssid_ie,
                                         uint8_t uc_ssid_len)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_ssid_param_stru *pst_ssid_param;
    int32_t l_ret;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SSID, OAL_SIZEOF(mac_cfg_ssid_param_stru));

    pst_ssid_param = (mac_cfg_ssid_param_stru *)(st_write_msg.auc_value);
    pst_ssid_param->uc_ssid_len = uc_ssid_len;
    if (EOK != memcpy_s(pst_ssid_param->ac_ssid, uc_ssid_len, (int8_t *)puc_ssid_ie, uc_ssid_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_set_ssid::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_netdev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                               (uint8_t *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_ssid::fail to send ssid cfg msg, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_set_auth_mode(oal_net_device_stru *pst_netdev, uint8_t en_auth_algs)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    if ((en_auth_algs != WLAN_WITP_AUTH_OPEN_SYSTEM) &&
        (en_auth_algs != WLAN_WITP_AUTH_SHARED_KEY)
#ifdef _PRE_WLAN_FEATURE_SAE
        && (en_auth_algs != WLAN_WITP_AUTH_SAE)
#endif
        && (en_auth_algs != WLAN_WITP_AUTH_AUTOMATIC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_auth_mode::en_auth_algs error[%d].}", en_auth_algs);
        return -OAL_EFAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_AUTH_MODE, OAL_SIZEOF(uint32_t));
    *((uint32_t *)(st_write_msg.auc_value)) = en_auth_algs; /* ��������������� */

    l_ret = wal_send_cfg_event(pst_netdev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                               (uint8_t *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_auth_mode::fail to send auth_tpye cfg msg, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_fill_beacon_param(oal_net_device_stru *pst_netdev,
                                                  oal_beacon_data_stru *pst_beacon_info,
                                                  mac_beacon_param_stru *pst_beacon_param)
{
    oal_beacon_parameters st_beacon_info_tmp;
    uint8_t *puc_beacon_info_tmp = NULL;
    uint32_t ul_beacon_head_len, ul_beacon_tail_len, ul_offset, ul_ret;
    uint8_t uc_vap_id;
    mac_vap_stru *pst_mac_vap = NULL;
    int32_t l_ret = EOK;

    if (oal_any_null_ptr3(pst_netdev, pst_beacon_info, pst_beacon_param)) {
        oam_error_log3(0, OAM_SF_ANY,
            "{wal_cfg80211_fill_beacon_param::param is NULL. mac_vap=0x%x, beacon_info=0x%x, beacon_param=0x%x",
            (uintptr_t)pst_netdev, (uintptr_t)pst_beacon_info, (uintptr_t)pst_beacon_param);
        return -OAL_EINVAL;
    }
    /* ��ȡvap id */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::pst_mac_vap = %x}", (uintptr_t)pst_mac_vap);
        return -OAL_EINVAL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;
    /*****************************************************************************
        1.��ȫ����ie��Ϣ��
    *****************************************************************************/
    if (oal_any_null_ptr2(pst_beacon_info->tail, pst_beacon_info->head)) {
        oam_error_log2(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::beacon frame error tail=%x, head=%x!}",
                       (uintptr_t)(pst_beacon_info->tail), (uintptr_t)(pst_beacon_info->head));
        return -OAL_EINVAL;
    }

    ul_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    /* oal_ieee80211_mgmt ǰ�湫������sizeΪ MAC_80211_FRAME_LEN(24)  */
    if (pst_beacon_info->head_len < (ul_offset + MAC_80211_FRAME_LEN)) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_fill_beacon_param::pst_beacon_info head_len[%d] error.}",
            pst_beacon_info->head_len);
        return -OAL_EINVAL;
    }

    ul_beacon_head_len = (uint32_t)pst_beacon_info->head_len;
    ul_beacon_tail_len = (uint32_t)pst_beacon_info->tail_len;

    /* ����������� */
    if ((ul_beacon_head_len + ul_beacon_tail_len) < ul_beacon_head_len) {
        oam_error_log2(0, 0, "{wal_cfg80211_fill_beacon_param::beacon_head_len:%d, beacon_tail_len:%d. len abnormal.}",
            ul_beacon_head_len, ul_beacon_tail_len);
        return -OAL_EINVAL;
    }

    puc_beacon_info_tmp = (uint8_t *)(oal_memalloc(ul_beacon_head_len + ul_beacon_tail_len));
    if (puc_beacon_info_tmp == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::puc_beacon_info_tmp memalloc failed.}");
        return -OAL_EINVAL;
    } else {
        l_ret += memcpy_s(puc_beacon_info_tmp, ul_beacon_head_len + ul_beacon_tail_len,
                          pst_beacon_info->head, ul_beacon_head_len);
        l_ret += memcpy_s(puc_beacon_info_tmp + ul_beacon_head_len, ul_beacon_tail_len,
                          pst_beacon_info->tail, ul_beacon_tail_len);
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "wal_cfg80211_fill_beacon_param::memcpy fail!");
        oal_free(puc_beacon_info_tmp);
        return -OAL_EINVAL;
    }

    /* Ϊ�˸���51�Ľ����ӿڣ������ں˽ṹ�е����ݸ�ֵ��51�ӿ�ʶ��Ľṹ�壬������ȡ��ϢԪ�� */
    memset_s(&st_beacon_info_tmp, sizeof(st_beacon_info_tmp), 0, sizeof(st_beacon_info_tmp));
    st_beacon_info_tmp.head = puc_beacon_info_tmp;
    st_beacon_info_tmp.head_len = (int32_t)ul_beacon_head_len;
    st_beacon_info_tmp.tail = puc_beacon_info_tmp + ul_beacon_head_len;
    st_beacon_info_tmp.tail_len = (int32_t)ul_beacon_tail_len;

    /* ��ȡ WPA/WPA2 ��ϢԪ�� */
    wal_parse_wpa_wpa2_ie(&st_beacon_info_tmp, pst_beacon_param);

    /* �˽ӿ���Ҫ�޸ģ�linux��û���⣬����win32�д� TBD */
    ul_ret = wal_parse_ht_vht_ie(pst_mac_vap, &st_beacon_info_tmp, pst_beacon_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::failed to parse HT/VHT ie!}");
        oal_free(puc_beacon_info_tmp);
        return -OAL_EINVAL;
    }

    /* �����Ƿ�����SSID */
    if (puc_beacon_info_tmp[WAL_BCN_BSSID_LENGTH_ADDR] == 0) {
        pst_beacon_param->uc_hidden_ssid = 1;
    }

    /* ����Wmm��ϢԪ�� */
    if (wal_parse_wmm_ie(pst_netdev, pst_mac_vap, &st_beacon_info_tmp) != OAL_SUCC) {
        oam_warning_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::Failed to parse wmm ie!}");
        oal_free(puc_beacon_info_tmp);
        return -OAL_EINVAL;
    }
    /* �ͷ���ʱ������ڴ� */
    oal_free(puc_beacon_info_tmp);

#ifdef _PRE_WLAN_FEATURE_11D
    /* ���ձ�14�ŵ��������жϣ�ֻ��11bģʽ�²�������14����11bģʽ ��Ϊ11b */
    if ((pst_mac_vap->st_channel.uc_chan_number == 14) && (pst_beacon_param->en_protocol != WLAN_LEGACY_11B_MODE)) {
        oam_error_log1(uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_fill_beacon_param::ch 14 should in 11b, but is %d!}", pst_beacon_param->en_protocol);
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_fill_beacon_param::change protocol to 11b!}");
        pst_beacon_param->en_protocol = WLAN_LEGACY_11B_MODE;
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_change_beacon(oal_wiphy_stru *pst_wiphy,
                                                oal_net_device_stru *pst_netdev,
                                                oal_beacon_data_stru *pst_beacon_info)
{
    mac_beacon_param_stru st_beacon_param; /* beacon info struct */
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    oam_info_log0(0, OAM_SF_ANY, "{wal_cfg80211_change_beacon::enter here.}");

    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_beacon_info)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_cfg80211_change_beacon::pst_wiphy=%x, pst_netdev=%x, pst_beacon_info=%x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)pst_beacon_info);
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_change_beacon:: dfr or s3s4 is processing!}");
        return OAL_SUCC;
    }

    /* ��ʼ��beacon interval ��DTIM_PERIOD ���� */
    memset_s(&st_beacon_param, sizeof(mac_beacon_param_stru), 0, sizeof(mac_beacon_param_stru));

    l_ret = wal_cfg80211_fill_beacon_param(pst_netdev, pst_beacon_info, &st_beacon_param);
    if (l_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_change_beacon::failed to fill beacon param, error[%d]}", l_ret);
        return -OAL_EINVAL;
    }

    /* ���ò������� */
    st_beacon_param.en_operation_type = MAC_SET_BEACON;

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_CONFIG_BEACON, OAL_SIZEOF(mac_beacon_param_stru));

    /* ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_beacon_param_stru),
                        &st_beacon_param, OAL_SIZEOF(mac_beacon_param_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_change_beacon::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_beacon_param_stru),
                               (uint8_t *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_change_beacon::Fail to start addset beacon, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_convert_width_to_value(int32_t l_channel_width)
{
    int32_t l_channel_width_value = 0;

    switch (l_channel_width) {
        case 0:
        case 1:
            l_channel_width_value = 20;
            break;
        case 2:
            l_channel_width_value = 40;
            break;
        case 3:
        case 4:
            l_channel_width_value = 80;
            break;
        case 5:
            l_channel_width_value = 160;
            break;
        default:
            break;
    }

    return l_channel_width_value;
}

OAL_STATIC wlan_channel_bandwidth_enum_uint8 wal_cfg80211_set_channel_cal_bw(oal_net_device_stru *netdev)
{
    int32_t channel_center_freq0;
    int32_t channel_center_freq1;
    int32_t bandwidth_value;
    uint8_t  vht_width;
    int32_t channel;
    wlan_channel_bandwidth_enum_uint8  channl_band_width;
    oal_ieee80211_channel *p_chan_stru = OAL_PTR_NULL;
    mac_vap_stru          *p_mac_vap = OAL_PTR_NULL;
    mac_device_stru       *p_mac_dev = OAL_PTR_NULL;

    p_mac_vap = oal_net_dev_priv(netdev);

    p_chan_stru = netdev->ieee80211_ptr->preset_chandef.chan;
    channel     = p_chan_stru->hw_value;

    /* �����ں˴���ֵ��WITP ����ֵת�� */
    channel_center_freq0 = oal_ieee80211_frequency_to_channel(netdev->ieee80211_ptr->preset_chandef.center_freq1);
    channel_center_freq1 = oal_ieee80211_frequency_to_channel(netdev->ieee80211_ptr->preset_chandef.center_freq2);
    bandwidth_value = wal_cfg80211_convert_width_to_value(netdev->ieee80211_ptr->preset_chandef.width);
    vht_width = wal_cfg80211_convert_value_to_vht_width(bandwidth_value);

    if (bandwidth_value == 0) {
        oam_error_log1(0, 0, "{wal_cfg80211_cal_bw::chan width invalid, bandwidth=%d",
            netdev->ieee80211_ptr->preset_chandef.width);
        return -OAL_EINVAL;
    }

    if (bandwidth_value == 80 || bandwidth_value == 160) { /* 80 160  Ϊ�ϲ��·�������� */
        channl_band_width = mac_get_bandwith_from_center_freq_seg0_seg1(vht_width, (oal_uint8)channel,
            (oal_uint8)channel_center_freq0, (oal_uint8)channel_center_freq1);
    } else if (bandwidth_value == 40) { /* 40Ϊ�ϲ��·�������� */
        switch (channel_center_freq0 - channel) {
            case -2:
                channl_band_width = WLAN_BAND_WIDTH_40MINUS;
                break;
            case 2:
                channl_band_width = WLAN_BAND_WIDTH_40PLUS;
                break;
            default:
                channl_band_width = WLAN_BAND_WIDTH_20M;
                break;
        }
    } else {
        channl_band_width = WLAN_BAND_WIDTH_20M;
    }

    p_mac_dev = (mac_device_stru *)mac_res_get_dev(p_mac_vap->uc_device_id);
    if (p_mac_dev != OAL_PTR_NULL && p_mac_dev->st_p2p_info.p2p_scenes == MAC_P2P_SCENES_LOW_LATENCY) {
        mac_vap_p2p_bw_back_to_40m(p_mac_vap, channel, &channl_band_width);
    }
    return channl_band_width;
}




OAL_STATIC int32_t wal_cfg80211_set_channel_info(oal_wiphy_stru *pst_wiphy,
                                                 oal_net_device_stru *pst_netdev)
{
    mac_cfg_channel_param_stru *pst_channel_param = NULL;
    oal_ieee80211_channel *pst_channel = NULL;
    wlan_channel_bandwidth_enum_uint8 en_bandwidth;
    wal_msg_write_stru st_write_msg;
    uint32_t ul_err_code;
    int32_t l_channel;
    int32_t l_center_freq1;
    int32_t l_center_freq2;
    int32_t l_bandwidth;
    wal_msg_stru *pst_rsp_msg = NULL;
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = NULL;
    uint8_t uc_vap_id;

    /* ��ȡvap id */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    uc_vap_id = pst_mac_vap->uc_vap_id;

    l_bandwidth = pst_netdev->ieee80211_ptr->preset_chandef.width;
    l_center_freq1 = pst_netdev->ieee80211_ptr->preset_chandef.center_freq1;
    l_center_freq2 = pst_netdev->ieee80211_ptr->preset_chandef.center_freq2;
    pst_channel = pst_netdev->ieee80211_ptr->preset_chandef.chan;
    l_channel = pst_channel->hw_value;

    oam_warning_log4(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_set_channel_info::\
        l_bandwidth = %d, l_center_freq1 = %d, l_center_freq2 = %d, l_channel = %d.}",
        l_bandwidth, l_center_freq1, l_center_freq2, l_channel);

    /* �ж��ŵ��ڲ��ڹ������� */
    l_ret = (int32_t)mac_is_channel_num_valid(pst_channel->band, (uint8_t)l_channel);
    if (l_ret != OAL_SUCC) {
        oam_warning_log2(uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_set_channel_info::chan num invalid. band, ch num [%d] [%d]!}", pst_channel->band, l_channel);
        return -OAL_EINVAL;
    }

    en_bandwidth = wal_cfg80211_set_channel_cal_bw(pst_netdev);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    pst_channel_param = (mac_cfg_channel_param_stru *)(st_write_msg.auc_value);
    pst_channel_param->uc_channel = (uint8_t)pst_channel->hw_value;
    pst_channel_param->en_band = pst_channel->band;
    pst_channel_param->en_bandwidth = en_bandwidth;

    oam_warning_log3(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_set_channel::uc_channel=%d, en_band=%d, en_bandwidth=%d.}",
                     pst_channel_param->uc_channel, pst_channel_param->en_band, pst_channel_param->en_bandwidth);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_SET_CHANNEL, OAL_SIZEOF(mac_cfg_channel_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_channel_param_stru),
                               (uint8_t *)&st_write_msg,
                               OAL_TRUE, &pst_rsp_msg);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_P2P, "{wal_cfg80211_set_channel_info::wal_send_cfg_event return err:%d}", l_ret);
        return -OAL_EFAIL;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_set_channel_info::wal_check_and_release_msg_resp fail return err code:%u}", ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_start_ap(oal_wiphy_stru *pst_wiphy,
                                           oal_net_device_stru *pst_netdev,
                                           oal_ap_settings_stru *pst_ap_settings)
{
    mac_beacon_param_stru st_beacon_param; /* beacon info struct */
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap = NULL;
    oal_beacon_data_stru *pst_beacon_info = NULL;
    uint8_t *puc_ssid_ie = NULL;
    int32_t l_ret;
    uint8_t uc_ssid_len;
    uint8_t uc_vap_id;
    uint8_t auc_ssid_ie[32];
    int32_t l_ssid_len;
    oal_netdev_priv_stru *pst_netdev_priv = NULL;
    wlan_auth_alg_mode_enum_uint8 en_auth_algs;
    int32_t l_memcpy_ret;

    oam_info_log0(0, OAM_SF_ANY, "{wal_cfg80211_start_ap::enter here.}");

    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_ap_settings)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_cfg80211_start_ap::pst_wiphy = %x, pst_netdev = %x, pst_ap_settings = %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)pst_ap_settings);
        return -OAL_EINVAL;
    }

    /* ��ȡvap id */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_start_ap::pst_mac_vap = %x}", (uintptr_t)pst_mac_vap);
        return -OAL_EINVAL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    /*****************************************************************************
        1.�����ŵ�
    *****************************************************************************/
    l_ret = wal_cfg80211_set_channel_info(pst_wiphy, pst_netdev);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::failed to set channel, return err:%d}", l_ret);
        return -OAL_EFAIL;
    }

    /*****************************************************************************
        2.1 ����ssid��Ϣ
    *****************************************************************************/
    l_ssid_len = pst_ap_settings->ssid_len;
    if ((l_ssid_len > 32) || (l_ssid_len <= 0)) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::ssid len error, len[%d].}", l_ssid_len);
        return -OAL_EFAIL;
    }
    memset_s(auc_ssid_ie, sizeof(auc_ssid_ie), 0, sizeof(auc_ssid_ie));
    l_memcpy_ret = memcpy_s(auc_ssid_ie, sizeof(auc_ssid_ie), pst_ap_settings->ssid, (uint32_t)l_ssid_len);
    puc_ssid_ie = auc_ssid_ie;
    uc_ssid_len = (uint8_t)l_ssid_len;

    if (uc_ssid_len != 0) {
        l_ret = wal_cfg80211_set_ssid(pst_netdev, puc_ssid_ie, uc_ssid_len);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::fail send ssid cfg msg, err[%d]}", l_ret);
            return -OAL_EFAIL;
        }
    }

    /*****************************************************************************
        2.2 ����auth mode��Ϣ
    *****************************************************************************/
    en_auth_algs = pst_ap_settings->auth_type;

    l_ret = wal_cfg80211_set_auth_mode(pst_netdev, en_auth_algs);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_start_ap::fail to send auth_tpye cfg msg, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    /*****************************************************************************
        3.����beaconʱ������tim period�Լ���ȫ������Ϣ��
    *****************************************************************************/
    /* ��ʼ��beacon interval ��DTIM_PERIOD ���� */
    memset_s(&st_beacon_param, sizeof(mac_beacon_param_stru), 0, sizeof(mac_beacon_param_stru));
    st_beacon_param.l_interval = pst_ap_settings->beacon_interval;
    st_beacon_param.l_dtim_period = pst_ap_settings->dtim_period;
    st_beacon_param.uc_hidden_ssid = (pst_ap_settings->hidden_ssid == 1);

    oam_warning_log3(0, OAM_SF_ANY,
        "{wal_cfg80211_fill_beacon_param::beacon_interval=%d, dtim_period=%d, hidden_ssid=%d!}",
        pst_ap_settings->beacon_interval, pst_ap_settings->dtim_period, pst_ap_settings->hidden_ssid);
    pst_beacon_info = &(pst_ap_settings->beacon);
    l_ret = wal_cfg80211_fill_beacon_param(pst_netdev, pst_beacon_info, &st_beacon_param);
    if (l_ret != OAL_SUCC) {
        oam_error_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::failed to fill beacon param, error[%d]}", l_ret);
        return -OAL_EINVAL;
    }

    /* ���ò������� */
    st_beacon_param.en_operation_type = MAC_ADD_BEACON;

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_CONFIG_BEACON, OAL_SIZEOF(mac_beacon_param_stru));

    /* ��д msg ��Ϣ�� */
    l_memcpy_ret += memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_beacon_param_stru),
                             &st_beacon_param, OAL_SIZEOF(mac_beacon_param_stru));

    if (l_memcpy_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_start_ap::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_beacon_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::fail to start addset beacon, err[%d]}", l_ret);
        return -OAL_EFAIL;
    }
    /*****************************************************************************
        4.����ap
    *****************************************************************************/
    l_ret = wal_start_vap(pst_netdev);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_start_ap::failed to start ap, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_netdev);
    if ((pst_netdev_priv->uc_napi_enable) && (!pst_netdev_priv->uc_state)) {
        oal_napi_enable(&pst_netdev_priv->st_napi);
        pst_netdev_priv->uc_state = 1;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_stop_ap(oal_wiphy_stru *pst_wiphy,
                                          oal_net_device_stru *pst_netdev)
{
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap = NULL;
    int32_t l_ret;
    uint8_t uc_vap_id;
    oal_netdev_priv_stru *pst_netdev_priv = NULL;

    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev = NULL;
    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr2(pst_wiphy, pst_netdev)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap::pst_wiphy = %x, pst_netdev = %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap:: dfr or s3s4 is processing!}");
        return OAL_SUCC;
    }

    /* ��ȡvap id */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap::can't get mac vap from netdevice priv data!}");
        return -OAL_EINVAL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    /* �ж��Ƿ�Ϊ��apģʽ */
    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_stop_ap::vap is not in ap mode!}");
        return -OAL_EINVAL;
    }

    /* ���netdev����running״̬������Ҫdown */
    if ((oal_netdevice_flags(pst_netdev) & OAL_IFF_RUNNING) == 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap::vap is already down!}\r\n");
        return OAL_SUCC;
    }

    /*****************************************************************************
        ������Ϣ��ͣ��ap
    *****************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DOWN_VAP, OAL_SIZEOF(mac_cfg_start_vap_param_stru));

    pst_wdev = pst_netdev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}");
        return -OAL_EINVAL;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_stop_ap::en_p2p_mode=%u}\r\n", en_p2p_mode);

    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_netdev;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_start_vap_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_stop_ap::failed to stop ap, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_netdev);
    if (pst_netdev_priv->uc_napi_enable) {
#ifndef WIN32
        oal_netbuf_list_purge(&pst_netdev_priv->st_rx_netbuf_queue);
#endif
        oal_napi_disable(&pst_netdev_priv->st_napi);
        pst_netdev_priv->uc_state = 0;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_change_bss(oal_wiphy_stru *pst_wiphy,
                                             oal_net_device_stru *pst_netdev,
                                             oal_bss_parameters *pst_bss_params)
{
    return OAL_SUCC;
}


OAL_STATIC void wal_cfg80211_print_sched_scan_req_info(oal_cfg80211_sched_scan_request_stru *pst_request)
{
    int8_t ac_tmp_buff[200];
    int32_t l_loop = 0;
    int32_t l_ret;

    /* ��ӡ�������� */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    oam_warning_log4(0, OAM_SF_SCAN,
        "wal_cfg80211_print_sched_scan_req_info::channels[%d],interval[%d]ms,flags[%d],rssi_thold[%d]",
        pst_request->n_channels, pst_request->scan_plans[0].interval,
        pst_request->flags,
        pst_request->min_rssi_thold);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    oam_warning_log4(0, OAM_SF_SCAN,
        "wal_cfg80211_print_sched_scan_req_info::channels[%d],interval[%d]ms,flags[%d],rssi_thold[%d]",
        pst_request->n_channels, pst_request->interval,
        pst_request->flags,
        pst_request->min_rssi_thold);
#else
    oam_warning_log4(0, OAM_SF_SCAN,
        "wal_cfg80211_print_sched_scan_req_info::channels[%d],interval[%d]ms,flags[%d],rssi_thold[%d]",
        pst_request->n_channels, pst_request->interval,
        pst_request->flags,
        pst_request->rssi_thold);
#endif

    /* ��ӡssid���ϵ���Ϣ */
    for (l_loop = 0; l_loop < pst_request->n_match_sets; l_loop++) {
        memset_s(ac_tmp_buff, OAL_SIZEOF(ac_tmp_buff), 0, OAL_SIZEOF(ac_tmp_buff));
        l_ret = snprintf_s(ac_tmp_buff, OAL_SIZEOF(ac_tmp_buff), OAL_SIZEOF(ac_tmp_buff) - 1,
                           "mactch_sets[%d] info, ssid_len[%d], ssid: %.32s.\n",
                           l_loop, pst_request->match_sets[l_loop].ssid.ssid_len,
                           pst_request->match_sets[l_loop].ssid.ssid);
        if (l_ret < 0) {
            oam_error_log0(0, OAM_SF_SCAN, "wal_cfg80211_print_sched_scan_req_info::snprintf_s error!");
            return;
        }

        oam_print(ac_tmp_buff);
    }

    for (l_loop = 0; l_loop < pst_request->n_ssids; l_loop++) {
        memset_s(ac_tmp_buff, OAL_SIZEOF(ac_tmp_buff), 0, OAL_SIZEOF(ac_tmp_buff));
        l_ret = snprintf_s(ac_tmp_buff, OAL_SIZEOF(ac_tmp_buff), OAL_SIZEOF(ac_tmp_buff) - 1,
                           "ssids[%d] info, ssid_len[%d], ssid: %.32s.\n",
                           l_loop, pst_request->ssids[l_loop].ssid_len, pst_request->ssids[l_loop].ssid);
        if (l_ret < 0) {
            oam_error_log0(0, OAM_SF_SCAN, "wal_cfg80211_print_sched_scan_req_info::snprintf_s error!");
            return;
        }

        oam_print(ac_tmp_buff);
    }

    return;
}


OAL_STATIC oal_bool_enum_uint8 wal_pno_scan_with_assigned_ssid(oal_cfg80211_ssid_stru *pst_ssid,
                                                               oal_cfg80211_ssid_stru *pst_ssid_list,
                                                               int32_t l_count)
{
    int32_t l_loop;

    if (oal_any_null_ptr2(pst_ssid, pst_ssid_list)) {
        return OAL_FALSE;
    }

    for (l_loop = 0; l_loop < l_count; l_loop++) {
        if ((pst_ssid->ssid_len == pst_ssid_list[l_loop].ssid_len) &&
            (0 == oal_memcmp(pst_ssid->ssid, pst_ssid_list[l_loop].ssid, pst_ssid->ssid_len))) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


OAL_STATIC int32_t wal_cfg80211_sched_scan_start(oal_wiphy_stru *pst_wiphy,
                                                   oal_net_device_stru *pst_netdev,
                                                   oal_cfg80211_sched_scan_request_stru *pst_request)
{
    hmac_device_stru *pst_hmac_device = NULL;
    hmac_scan_stru *pst_scan_mgmt = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    oal_cfg80211_ssid_stru *pst_ssid_tmp = NULL;
    oal_cfg80211_ssid_stru *pst_scan_ssid_list = NULL;
    mac_pno_scan_stru st_pno_scan_info;
    int32_t l_loop = 0;
    uint32_t ul_ret;
    int32_t l_ret;

    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_request)) {
        oam_error_log3(0, 0, "{wal_cfg80211_sched_scan_start::input param null, wiphy[%p], netdev[%p], request[%p]!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev, (uintptr_t)pst_request);
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_sched_scan_start:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_device_stru �ṹ */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_sched_scan_start:: pst_mac_vap is null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_sched_scan_start:: pst_mac_device is null!}");
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    /* �����ǰ�豸����ɨ��״̬��abort��ǰɨ�� */
    if (pst_scan_mgmt->pst_request != OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_sched_scan_start:: device is busy, stop current scan!}");

        wal_force_scan_abort_then_scan_comp(pst_netdev);
    }

    /* ����ں��·�����Ҫƥ���ssid���ϵĸ����Ƿ�Ϸ� */
    if (pst_request->n_match_sets <= 0) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{wal_cfg80211_sched_scan_start::match_sets = %d!}",
            pst_request->n_match_sets);
        return -OAL_EINVAL;
    }

    /* ��ʼ��pnoɨ��Ľṹ����Ϣ */
    memset_s(&st_pno_scan_info, OAL_SIZEOF(st_pno_scan_info), 0, OAL_SIZEOF(st_pno_scan_info));

    /* ���ں��·���ƥ���ssid���ϸ��Ƶ����� */
    pst_scan_ssid_list = OAL_PTR_NULL;
    if (pst_request->n_ssids > 0) {
        pst_scan_ssid_list = pst_request->ssids;
    }
    for (l_loop = 0; l_loop < oal_min(pst_request->n_match_sets, MAX_PNO_SSID_COUNT); l_loop++) {
        pst_ssid_tmp = &(pst_request->match_sets[l_loop].ssid);
        if (pst_ssid_tmp->ssid_len >= WLAN_SSID_MAX_LEN) {
            oam_warning_log1(0, 0, "{wal_cfg80211_sched_scan_start:: wrong ssid_len[%d]!}", pst_ssid_tmp->ssid_len);
            continue;
        }
        l_ret = memcpy_s(st_pno_scan_info.ast_match_ssid_set[l_loop].auc_ssid, WLAN_SSID_MAX_LEN,
                         pst_ssid_tmp->ssid, pst_ssid_tmp->ssid_len);
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_SCAN, "wal_cfg80211_sched_scan_start::memcpy fail!");
        }

        st_pno_scan_info.ast_match_ssid_set[l_loop].auc_ssid[pst_ssid_tmp->ssid_len] = '\0';
        st_pno_scan_info.ast_match_ssid_set[l_loop].en_scan_ssid =
            wal_pno_scan_with_assigned_ssid(pst_ssid_tmp, pst_scan_ssid_list, pst_request->n_ssids);
        st_pno_scan_info.l_ssid_count++;
    }

    /* ����������ֵ */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    st_pno_scan_info.l_rssi_thold = pst_request->min_rssi_thold;
#else
    st_pno_scan_info.l_rssi_thold = pst_request->rssi_thold;
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)) */
    st_pno_scan_info.ul_pno_scan_interval = PNO_SCHED_SCAN_INTERVAL; /* �����Լ�����Ϊ30s */
    st_pno_scan_info.uc_pno_scan_repeat = MAX_PNO_REPEAT_TIMES;

    /* ���浱ǰ��PNO����ɨ������ָ�� */
    pst_scan_mgmt->pst_sched_scan_req = pst_request;
    pst_scan_mgmt->en_sched_scan_complete = OAL_FALSE;

    /* ά���ӡ�ϲ��·��ĵ���ɨ�����������Ϣ */
    wal_cfg80211_print_sched_scan_req_info(pst_request);

    /* �·�pnoɨ������hmac */
    ul_ret = wal_cfg80211_start_sched_scan(pst_netdev, &st_pno_scan_info);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, 0, "{wal_cfg80211_sched_scan_start::wal_cfg80211_start_sched_scan fail[%d]!}", ul_ret);
        return -OAL_EBUSY;
    }

    return OAL_SUCC;
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
OAL_STATIC int32_t wal_cfg80211_sched_scan_stop(oal_wiphy_stru *pst_wiphy,
                                                  oal_net_device_stru *pst_netdev,
                                                  uint64_t ul_reqid)

#else
OAL_STATIC int32_t wal_cfg80211_sched_scan_stop(oal_wiphy_stru *pst_wiphy,
                                                  oal_net_device_stru *pst_netdev)
#endif
{
    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr2(pst_wiphy, pst_netdev)) {
        oam_error_log2(0, OAM_SF_CFG,
            "{wal_cfg80211_sched_scan_stop::input param pointer is null, pst_wiphy[%p], pst_netdev[%p]!}",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    return wal_stop_sched_scan(pst_netdev);
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0))
int32_t wal_cfg80211_change_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                               oal_net_device_stru *pst_net_dev,
                                               enum nl80211_iftype en_type,
                                               oal_vif_params_stru *pst_params)
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
int32_t wal_cfg80211_change_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                               oal_net_device_stru *pst_net_dev,
                                               enum nl80211_iftype en_type,
                                               uint32_t *pul_flags,
                                               oal_vif_params_stru *pst_params)
#else
int32_t wal_cfg80211_change_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                               int32_t l_ifindex,
                                               enum nl80211_iftype en_type,
                                               uint32_t *pul_flags,
                                               oal_vif_params_stru *pst_params)
#endif
{
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    wlan_vap_mode_enum_uint8 en_vap_mode;
    mac_cfg_del_vap_param_stru st_del_vap_param;
    mac_cfg_add_vap_param_stru st_add_vap_param;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    int8_t ac_p2p_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];

    /* 1.1 ��μ�� */
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_change_virtual_intf::pst_dev is null!}\r\n");
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_change_virtual_intf:: dfr or s3s4 is processing!}");
        return OAL_SUCC;
    }

    if (oal_any_null_ptr2(pst_wiphy, pst_params)) {
        oam_error_log2(0, OAM_SF_CFG,
            "{wal_cfg80211_change_virtual_intf::wiphy or flag or params ptr is null,error %x, %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_params);
        return -OAL_EINVAL;
    }

    /* ���VAP ��ǰģʽ��Ŀ��ģʽ�Ƿ���ͬ�������ͬ��ֱ�ӷ��� */
    if (pst_net_dev->ieee80211_ptr->iftype == en_type) {
        oam_warning_log1(0, 0, "{wal_cfg80211_change_virtual_intf::same iftype[%d],do not need change!}", en_type);
        return OAL_SUCC;
    }

    oam_warning_log2(0, OAM_SF_CFG, "{wal_cfg80211_change_virtual_intf::[%d][%d]}\r\n",
                     (pst_net_dev->ieee80211_ptr->iftype), en_type);

    oal_io_print("wal_cfg80211_change_virtual_intf,dev_name is:%s\n", pst_net_dev->name);

    switch (en_type) {
        case NL80211_IFTYPE_MONITOR:
        case NL80211_IFTYPE_WDS:
        case NL80211_IFTYPE_MESH_POINT:
        case NL80211_IFTYPE_ADHOC:
            oam_error_log1(0, 0, "{wal_cfg80211_change_virtual_intf::currently do not sup this type[%d]}", en_type);
            return -OAL_EINVAL;

        case NL80211_IFTYPE_STATION:
            if (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_AP) {
                /* �µ�APUT���л�netdev״̬Ϊstation */
                return wal_netdev_stop_ap(pst_net_dev);
            }

            // P2P_DEVICE ��ǰ����������Ҫͨ��wpa_supplicant ����
            pst_net_dev->ieee80211_ptr->iftype = en_type;
            oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_change_virtual_intf::change to station}\r\n");
            return OAL_SUCC;
        case NL80211_IFTYPE_P2P_CLIENT:
            en_vap_mode = WLAN_VAP_MODE_BSS_STA;
            en_p2p_mode = WLAN_P2P_CL_MODE;
            break;
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_AP_VLAN:
            en_vap_mode = WLAN_VAP_MODE_BSS_AP;
            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            break;
        case NL80211_IFTYPE_P2P_GO:
            en_vap_mode = WLAN_VAP_MODE_BSS_AP;
            en_p2p_mode = WLAN_P2P_GO_MODE;
            break;
        default:
            oam_error_log1(0, 0, "{wal_cfg80211_change_virtual_intf::currently do not sup this type[%d]}", en_type);
            return -OAL_EINVAL;
    }

    if (en_type == NL80211_IFTYPE_AP) {
        return wal_setup_ap(pst_net_dev);
    }

    /* �豸ΪP2P �豸����Ҫ����change virtual interface */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_change_virtual_intf::can't get mac vap from netdevice priv}");
        return -OAL_EINVAL;
    }

    if (IS_LEGACY_VAP(pst_mac_vap)) {
        pst_net_dev->ieee80211_ptr->iftype = en_type;
        return OAL_SUCC;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_CFG,
            "{wal_cfg80211_change_virtual_intf::mac_res_get_dev fail dev_id[%d].}", pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    /* ͨ��device id��ȡnetdev���� */
    snprintf_s(ac_p2p_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "p2p%d", pst_mac_device->uc_device_id);

    if (0 == (oal_strcmp(ac_p2p_netdev_name, pst_net_dev->name))) {
        /* ����쳣�����,wpa_supplicant�·�p2p0�豸�л���p2p go/cliģʽ����fastboot������ */
        oam_warning_log0(0, 0, "{wal_cfg80211_change_virtual_intf::p2p0 netdevice can not change to P2P CLI/GO.}");
        return -OAL_EINVAL;
    }

    /* �����ǰģʽ��Ŀ��ģʽ��ͬ������Ҫ:
       1. ֹͣ VAP
       2. ɾ�� VAP
       3. ���´�����ӦģʽVAP
       4. ����VAP
    */
    /* ֹͣVAP */
    wal_netdev_stop(pst_net_dev);

    memset_s(&st_del_vap_param, OAL_SIZEOF(st_del_vap_param), 0, OAL_SIZEOF(st_del_vap_param));
    /* ɾ��VAP */
    st_del_vap_param.pst_net_dev = pst_net_dev;
    /* �豸p2p ģʽ��Ҫ��net_device �л�ȡ */
    st_del_vap_param.en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_net_dev->ieee80211_ptr->iftype);
    if (wal_cfg80211_del_vap(&st_del_vap_param)) {
        return -OAL_EFAIL;
    }

    memset_s(&st_add_vap_param, OAL_SIZEOF(st_add_vap_param), 0, OAL_SIZEOF(st_add_vap_param));
    /* ���´�����ӦģʽVAP */
    st_add_vap_param.pst_net_dev = pst_net_dev;
    st_add_vap_param.en_vap_mode = en_vap_mode;
    st_add_vap_param.en_p2p_mode = en_p2p_mode;

    wal_cfg80211_add_vap(&st_add_vap_param);

    /* ����VAP */
    wal_netdev_open(pst_net_dev, OAL_FALSE);

    pst_net_dev->ieee80211_ptr->iftype = en_type;
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_add_station(oal_wiphy_stru *pst_wiphy,
    oal_net_device_stru *pst_dev,
    const uint8_t *puc_mac,
    oal_station_parameters_stru *pst_sta_parms)
{
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_del_station(oal_wiphy_stru *pst_wiphy,
    oal_net_device_stru *pst_dev,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    struct station_del_parameters *params
#else
    uint8_t *puc_mac
#endif
    )
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_cfg_kick_user_param_stru st_kick_user_param;
    int32_t int_user_count_ok = 0;
    int32_t int_user_count_fail = 0;
    int32_t uint_ret = OAL_FAIL;
    uint8_t auc_mac_boardcast[OAL_MAC_ADDR_LEN];
    uint16_t us_reason_code = MAC_INACTIVITY;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    uint8_t *puc_mac;
    if (params == OAL_PTR_NULL) {
        return -OAL_EFAUL;
    }
    puc_mac = (uint8_t *)params->mac;

    us_reason_code = params->reason_code;
#endif

    if (oal_any_null_ptr2(pst_wiphy, pst_dev)) {
        return -OAL_EFAUL;
    }
#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_del_station::dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return OAL_SUCC;
    }

#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    pst_mac_vap = oal_net_dev_priv(pst_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_station::can't get mac vap from netdevice priv data!}\r\n");
        return -OAL_EFAUL;
    }

    /* �ж��Ƿ���APģʽ */
    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) {
        oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_del_station::vap_mode:%d!}", pst_mac_vap->en_vap_mode);
        return -OAL_EINVAL;
    }

    if (puc_mac == OAL_PTR_NULL) {
        memset_s(auc_mac_boardcast, OAL_MAC_ADDR_LEN, 0xff, OAL_MAC_ADDR_LEN);
        puc_mac = auc_mac_boardcast;
        oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_del_station::deleting all user!}\r\n");
    } else {
        oam_info_log4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_del_station::deleting user:%02X:XX:XX:%02X:%02X:%02X!}\r\n",
            puc_mac[0], puc_mac[3], puc_mac[4], puc_mac[5]);
    }

    st_kick_user_param.us_reason_code = us_reason_code;
    if (EOK != memcpy_s(st_kick_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN, puc_mac, OAL_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_del_station::memcpy fail!");
    }
    uint_ret = wal_cfg80211_start_disconnect(pst_dev, &st_kick_user_param);
    if (uint_ret != OAL_SUCC) {
        /* ����ɾ����ʱ������û��Ѿ�ɾ������ʱ�ٽ����û����ң��᷵�ش������ERROR��ӡ���޸�Ϊwarning */
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_del_station::config_kick_user fail[%d]!}", uint_ret);
        int_user_count_fail++;
    } else {
        int_user_count_ok++;
    }

    if (int_user_count_fail > 0) {
        oam_info_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_del_station::%d usr fail del!}", int_user_count_fail);
        return -OAL_EINVAL;
    }

    oam_info_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_del_station::%d user is deleted!}", int_user_count_ok);
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_change_station(oal_wiphy_stru *pst_wiphy,
    oal_net_device_stru *pst_dev,
    const uint8_t *puc_mac,
    oal_station_parameters_stru *pst_sta_parms)
{
    return OAL_SUCC;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0))

OAL_STATIC void wal_fill_station_rate_info(oal_station_info_stru *sta_info,
    oal_station_info_stru *stats)
{
    /* 11acģʽ�� ����˽�����ʵĴ����ں��޷�ʶ�� �޸�Ϊͨ��legacy�ϱ� ������Ӧ�ı�־λ */
    if ((stats->txrate.flags & RATE_INFO_FLAGS_VHT_MCS) || (stats->txrate.flags & MAC_RATE_INFO_FLAGS_HE_MCS)) {
        sta_info->txrate.legacy = stats->txrate.legacy;      /* dmac�ѽ���λ�޸�Ϊ100kbps */
        /* �����־λRATE_INFO_FLAGS_VHT_MCS RATE_INFO_FLAGS_MCS */
        sta_info->txrate.flags = stats->txrate.flags & 0x7C;
    } else {
        sta_info->txrate.legacy = (uint16_t)(stats->txrate.legacy * 10); /* �ں��е�λΪ100kbps */
        sta_info->txrate.flags  = stats->txrate.flags;
    }

    sta_info->txrate.mcs = stats->txrate.mcs;
    sta_info->txrate.nss = stats->txrate.nss;
    sta_info->txrate.bw  = stats->txrate.bw;

    /* �ϱ��������� */
    sta_info->filled |= BIT(NL80211_STA_INFO_RX_BITRATE);
    sta_info->rxrate.legacy = stats->rxrate.legacy;
}
#endif

#define QUERY_STATION_INFO_TIME (5 * OAL_TIME_HZ)

OAL_STATIC void wal_cfg80211_fill_station_info(oal_station_info_stru *pst_sta_info,
                                                   oal_station_info_stru *pst_stats)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0))
    /* ����linux 4.0.�޸�ΪBIT(NL80211_STA_INFO_SIGNAL)��. */
    pst_sta_info->filled |= BIT(NL80211_STA_INFO_SIGNAL);

    pst_sta_info->signal = pst_stats->signal;

    pst_sta_info->filled |= BIT(NL80211_STA_INFO_RX_PACKETS);
    pst_sta_info->filled |= BIT(NL80211_STA_INFO_TX_PACKETS);

    pst_sta_info->rx_packets = pst_stats->rx_packets;
    pst_sta_info->tx_packets = pst_stats->tx_packets;

    pst_sta_info->filled |= BIT(NL80211_STA_INFO_RX_BYTES);
    pst_sta_info->filled |= BIT(NL80211_STA_INFO_TX_BYTES);
    pst_sta_info->rx_bytes = pst_stats->rx_bytes;
    pst_sta_info->tx_bytes = pst_stats->tx_bytes;

    pst_sta_info->filled |= BIT(NL80211_STA_INFO_TX_RETRIES);
    pst_sta_info->filled |= BIT(NL80211_STA_INFO_TX_FAILED);
    pst_sta_info->filled |= BIT(NL80211_STA_INFO_RX_DROP_MISC);

    pst_sta_info->tx_retries = pst_stats->tx_retries;
    pst_sta_info->tx_failed = pst_stats->tx_failed;
    pst_sta_info->rx_dropped_misc = pst_stats->rx_dropped_misc;

    pst_sta_info->filled |= BIT(NL80211_STA_INFO_TX_BITRATE);

    wal_fill_station_rate_info(pst_sta_info, pst_stats);

#else
    pst_sta_info->filled |= STATION_INFO_SIGNAL;

    pst_sta_info->signal = pst_stats->signal;

    pst_sta_info->filled |= STATION_INFO_RX_PACKETS;
    pst_sta_info->filled |= STATION_INFO_TX_PACKETS;

    pst_sta_info->rx_packets = pst_stats->rx_packets;
    pst_sta_info->tx_packets = pst_stats->tx_packets;

    pst_sta_info->filled |= STATION_INFO_RX_BYTES;
    pst_sta_info->filled |= STATION_INFO_TX_BYTES;
    pst_sta_info->rx_bytes = pst_stats->rx_bytes;
    pst_sta_info->tx_bytes = pst_stats->tx_bytes;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
    pst_sta_info->filled |= STATION_INFO_TX_RETRIES;
    pst_sta_info->filled |= STATION_INFO_TX_FAILED;
    pst_sta_info->filled |= STATION_INFO_RX_DROP_MISC;

    pst_sta_info->tx_retries = pst_stats->tx_retries;
    pst_sta_info->tx_failed = pst_stats->tx_failed;
    pst_sta_info->rx_dropped_misc = pst_stats->rx_dropped_misc;
#endif

    pst_sta_info->filled |= STATION_INFO_TX_BITRATE;
    pst_sta_info->txrate.legacy = (uint16_t)(pst_stats->txrate.legacy * 10); /* �ں��е�λΪ100kbps */
    pst_sta_info->txrate.flags = pst_stats->txrate.flags;
    pst_sta_info->txrate.mcs = pst_stats->txrate.mcs;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
    pst_sta_info->txrate.nss = pst_stats->txrate.nss;
#endif

#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)) */
}
OAL_STATIC void wal_cfg80211_fill_station_info_ext(oal_station_info_stru *sta_info,
    hmac_vap_stru *hmac_vap)
{
#ifdef CONFIG_HW_GET_EXT_SIG
    sta_info->filled |= BIT(NL80211_STA_INFO_NOISE);
    sta_info->noise = hmac_vap->station_info.noise;

    sta_info->filled |= BIT(NL80211_STA_INFO_SNR);
    sta_info->snr = oal_max(hmac_vap->st_station_info_extend.c_snr_ant0,
        hmac_vap->st_station_info_extend.c_snr_ant1);

    sta_info->filled |= BIT(NL80211_STA_INFO_CNAHLOAD);
    sta_info->chload = hmac_vap->station_info.chload;
#endif

#ifdef CONFIG_HW_GET_EXT_SIG_ULDELAY
    sta_info->filled |= BIT(NL80211_STA_INFO_UL_DELAY);
    sta_info->ul_delay = hmac_vap->station_info.ul_delay;
#endif
}

uint8_t wal_cfg80211_get_station_filter(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac)
{
    hmac_user_stru *pst_hmac_user = NULL;
    uint32_t ul_current_time = (uint32_t)oal_time_get_stamp_ms();
    uint32_t ul_runtime;
    uint32_t ul_get_station_threshold;
#ifndef WIN32
    mac_device_stru *pst_mac_dev = NULL;
#endif
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(pst_mac_vap, puc_mac);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_cfg80211_get_station_filter::user is null.}");
        return OAL_FALSE;
    }
#ifndef WIN32
    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_cfg80211_get_station_filter::dev is null.}");
        return OAL_FALSE;
    }
    
    if (IS_LEGACY_STA(pst_mac_vap) &&
        (pst_mac_vap->pst_vowifi_cfg_param != OAL_PTR_NULL &&
         pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode == VOWIFI_DISABLE_REPORT)) { /* ������vowifi����ʹ��ʱ */
        ul_get_station_threshold = WAL_VOWIFI_GET_STATION_THRESHOLD;
    } else
#endif
    {
        ul_get_station_threshold = WAL_GET_STATION_THRESHOLD;
    }

    ul_runtime = (uint32_t)oal_time_get_runtime(pst_hmac_user->ul_rssi_last_timestamp, ul_current_time);

    if (ul_get_station_threshold > ul_runtime) {
        return OAL_FALSE;
    }

    pst_hmac_user->ul_rssi_last_timestamp = ul_current_time;
    return OAL_TRUE;
}


OAL_STATIC int32_t wal_cfg80211_get_station(oal_wiphy_stru *pst_wiphy,
    oal_net_device_stru *pst_dev,
    const uint8_t *puc_mac,
    oal_station_info_stru *pst_sta_info)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    dmac_query_request_event st_dmac_query_request_event;
    dmac_query_station_info_request_event *pst_query_station_info = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    signed long i_leftime;
    int32_t l_ret;
    uint8_t uc_vap_id;

    if (oal_any_null_ptr4(pst_wiphy, pst_dev, puc_mac, pst_sta_info)) {
        oam_error_log4(0, OAM_SF_ANY,
                       "{wal_cfg80211_get_station::pst_wiphy[0x%p],pst_dev[0x%p],puc_mac[0x%p],pst_sta_info[0x%p]!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_dev, (uintptr_t)puc_mac, (uintptr_t)pst_sta_info);
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_get_station::oal_net_dev_priv, return null!}");
        return -OAL_EINVAL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    pst_query_station_info = (dmac_query_station_info_request_event *)&st_dmac_query_request_event;
    pst_query_station_info->query_event = OAL_QUERY_STATION_INFO_EVENT;
    oal_set_mac_addr(pst_query_station_info->auc_query_sta_addr, (uint8_t *)puc_mac);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_get_station::mac_res_get_hmac_vap fail.}");
        return -OAL_EINVAL;
    }

    /* �̶�ʱ����������һ��RSSI */
    if (OAL_FALSE == wal_cfg80211_get_station_filter(&pst_hmac_vap->st_vap_base_info, (uint8_t *)puc_mac)) {
        wal_cfg80211_fill_station_info(pst_sta_info, &pst_hmac_vap->station_info);
        wal_cfg80211_fill_station_info_ext(pst_sta_info, pst_hmac_vap);
        return OAL_SUCC;
    }

    pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;

    /********************************************************************************
        ���¼���wal�㴦�� �����ڵ͹�����Ҫ�����⴦���������²����¼���ֱ����ʱ��
        �͹��Ļ��ڽ���beacon֡��ʱ�������ϱ���Ϣ��
    ********************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    st_write_msg.en_wid = WLAN_CFGID_QUERY_STATION_STATS;
    st_write_msg.us_len = OAL_SIZEOF(st_dmac_query_request_event);

    /* 3.2 ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(dmac_query_station_info_request_event),
                        pst_query_station_info, OAL_SIZEOF(dmac_query_station_info_request_event))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_get_station::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(dmac_query_station_info_request_event),
                                   (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(uc_vap_id, 0, "{wal_cfg80211_get_station::wal_send_cfg_event return err code %d!}", l_ret);
        return -OAL_EFAIL;
    }
    /*lint -e730*/ /* info, boolean argument to function */
    i_leftime = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
        (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), QUERY_STATION_INFO_TIME);
    /*lint +e730*/
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_get_station::query info wait for %ld ms timeout!}",
                         ((QUERY_STATION_INFO_TIME * 1000) / OAL_TIME_HZ));
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_get_station::query info wait for %ld ms error!}",
                         ((QUERY_STATION_INFO_TIME * 1000) / OAL_TIME_HZ));
        return -OAL_EINVAL;
    } else {
        /* ��������  */
        wal_cfg80211_fill_station_info(pst_sta_info, &pst_hmac_vap->station_info);
        wal_cfg80211_fill_station_info_ext(pst_sta_info, pst_hmac_vap);
        return OAL_SUCC;
    }
}


OAL_STATIC int32_t wal_cfg80211_dump_station(oal_wiphy_stru *pst_wiphy,
                                               oal_net_device_stru *pst_dev,
                                               int32_t int_index,
                                               uint8_t *puc_mac,
                                               oal_station_info_stru *pst_sta_info)
{
    return OAL_SUCC;
}

//#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))

OAL_STATIC uint32_t wal_is_p2p_group_exist(mac_device_stru *pst_mac_device)
{
    if (OAL_SUCC != hmac_check_p2p_vap_num(pst_mac_device, WLAN_P2P_GO_MODE) ||
        OAL_SUCC != hmac_check_p2p_vap_num(pst_mac_device, WLAN_P2P_CL_MODE)) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}


OAL_STATIC void wal_set_p2p_group_del_status(hmac_vap_stru *pst_hmac_vap,
    oal_net_device_stru *pst_net_dev)
{
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{wal_set_p2p_group_del_status::hmac_device is null, device id[%d]}\r\n",
            pst_hmac_vap->st_vap_base_info.uc_device_id);
        return;
    }

    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{wal_set_p2p_group_del_status:: push deleting inf task to work queue, net_device[%p], device id[%d], \
        vap_mode[%d], p2p_mode[%d]}\r\n",
        (uintptr_t)pst_net_dev, pst_hmac_vap->st_vap_base_info.uc_device_id, pst_hmac_vap->st_vap_base_info.en_vap_mode,
        pst_hmac_vap->st_vap_base_info.en_p2p_mode);

    hmac_set_p2p_status(&pst_hmac_device->ul_p2p_intf_status, P2P_STATUS_IF_DELETING);
}


uint32_t wal_del_p2p_group(mac_device_stru *pst_mac_device)
{
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_net_device_stru *pst_net_dev = NULL;
    mac_cfg_del_vap_param_stru st_del_vap_param;

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_P2P, "{wal_del_p2p_group::get mac vap resource fail! vap id is %d}",
                             pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_P2P, "{wal_del_p2p_group::get hmac vap resource fail! vap id is %d}",
                             pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        pst_net_dev = pst_hmac_vap->pst_net_device;
        if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_P2P, "{wal_del_p2p_group::get net device fail! vap id is %d}",
                             pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        if (IS_P2P_GO(pst_mac_vap) || IS_P2P_CL(pst_mac_vap)) {
            memset_s(&st_del_vap_param, OAL_SIZEOF(st_del_vap_param), 0, OAL_SIZEOF(st_del_vap_param));
            oal_io_print("wal_del_p2p_group:: ifname %s\r\n", pst_net_dev->name);
            st_del_vap_param.pst_net_dev = pst_net_dev;
            st_del_vap_param.en_vap_mode = pst_mac_vap->en_vap_mode;
            st_del_vap_param.en_p2p_mode = mac_get_p2p_mode(pst_mac_vap);
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{wal_del_p2p_group:: vap mode[%d], p2p mode[%d]}",
                             st_del_vap_param.en_vap_mode, st_del_vap_param.en_p2p_mode);
            /* ɾ���Ѿ����ڵ�P2P group */
            wal_force_scan_complete(pst_net_dev, OAL_TRUE);
            wal_stop_vap(pst_net_dev);

            if (wal_cfg80211_del_vap(&st_del_vap_param) == OAL_SUCC) {
                wal_set_p2p_group_del_status(pst_hmac_vap, pst_net_dev);
                /* ����linux work queue ɾ��net_device����Ҫֱ�ӵ���unregister_netdev�Է�wal_netdev_stop��ѭ�� */
                pst_hmac_vap->pst_del_net_device = pst_net_dev;
                oal_workqueue_schedule(&(pst_hmac_vap->st_del_virtual_inf_worker));
            }
        }
    }
    return OAL_SUCC;
}


uint32_t wal_cfg80211_register_netdev(oal_net_device_stru *pst_net_dev)
{
    uint8_t uc_rollback_lock = OAL_FALSE;
    uint32_t ul_ret;
    oal_netdev_priv_stru *pst_netdev_priv = NULL;

    
    if (rtnl_is_locked()) {
        rtnl_unlock();
        uc_rollback_lock = OAL_TRUE;
    }

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_net_dev);
#ifdef _PRE_CONFIG_HISI_110X_NAPI_DISABLE
    pst_netdev_priv->uc_napi_enable     = OAL_FALSE;
    pst_netdev_priv->uc_gro_enable      = OAL_FALSE;
#else
    pst_netdev_priv->uc_napi_enable = OAL_TRUE;
    pst_netdev_priv->uc_gro_enable = OAL_TRUE;
#endif
    pst_netdev_priv->uc_napi_weight = NAPI_POLL_WEIGHT_LEV1;
    pst_netdev_priv->ul_queue_len_max = NAPI_NETDEV_PRIV_QUEUE_LEN_MAX;
    pst_netdev_priv->uc_state = 0;
    pst_netdev_priv->ul_period_pkts = 0;
    pst_netdev_priv->ul_period_start = 0;

    oal_netbuf_list_head_init(&pst_netdev_priv->st_rx_netbuf_queue);
    oal_netif_napi_add(pst_net_dev, &pst_netdev_priv->st_napi, hmac_rxdata_polling, NAPI_POLL_WEIGHT_LEV1);

    /* �ں�ע��net_device, ֻ����0 */
    ul_ret = (uint32_t)hmac_net_register_netdev(pst_net_dev);

    if (uc_rollback_lock) {
        rtnl_lock();
    }
    return ul_ret;
}


void wal_cfg80211_unregister_netdev(oal_net_device_stru *pst_net_dev)
{
    uint8_t uc_rollback_lock = OAL_FALSE;

    if (rtnl_is_locked()) {
        rtnl_unlock();
        uc_rollback_lock = OAL_TRUE;
    }

    /* ȥע��netdev */
    oal_net_unregister_netdev(pst_net_dev);

    if (uc_rollback_lock) {
        rtnl_lock();
    }
}


OAL_STATIC void wal_cfg80211_add_p2p_interface_init(oal_net_device_stru *pst_net_dev,mac_device_stru *pst_mac_device)
{
    uint8_t auc_primary_mac_addr[WLAN_MAC_ADDR_LEN] = {0};    /* MAC��ַ */

    if ((oal_strncmp("p2p-p2p0", pst_net_dev->name, OAL_STRLEN("p2p-p2p0")) != 0) &&
         (oal_strncmp("p2p-wlan0", pst_net_dev->name, OAL_STRLEN("p2p-wlan0")) != 0)) {
        return;
    }

    if (oal_unlikely(pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL)) {
        oal_random_ether_addr(auc_primary_mac_addr);
        auc_primary_mac_addr[0] &= (~0x02);
        auc_primary_mac_addr[1] = 0x11;
        auc_primary_mac_addr[2] = 0x02;
    } else {
        if (oal_unlikely(oal_netdevice_mac_addr(pst_mac_device->st_p2p_info.pst_primary_net_device) == OAL_PTR_NULL)) {
            oam_error_log0(0, OAM_SF_ANY,
                "{wal_cfg80211_add_p2p_interface_init() pst_primary_net_device; addr is null}\r\n");
            return;
        }

        if (memcpy_s(auc_primary_mac_addr, WLAN_MAC_ADDR_LEN,
            oal_netdevice_mac_addr(pst_mac_device->st_p2p_info.pst_primary_net_device), WLAN_MAC_ADDR_LEN) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_p2p_interface_init::memcpy fail}");
            return;
        }
    }

    oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev), auc_primary_mac_addr);
    pst_net_dev->dev_addr[0] |= 0x02;
    pst_net_dev->dev_addr[4] ^= 0x80;
}

OAL_STATIC void wal_cfg80211_add_virtual_intf_netdev_init(oal_net_device_stru *pst_net_dev, oal_wireless_dev_stru *pst_wdev)
{
    /* ���´�����net_device ��ʼ����Ӧ���� */
#ifdef CONFIG_WIRELESS_EXT
    pst_net_dev->wireless_handlers = &g_st_iw_handler_def;
#endif
    pst_net_dev->netdev_ops = &g_st_wal_net_dev_ops;

    pst_net_dev->ethtool_ops = &g_st_wal_ethtool_ops;

    oal_netdevice_destructor(pst_net_dev) = oal_net_free_netdev;
    /* host���ͱ���ʹ��csumӲ������ */
    if (g_tx_switch.tx_switch == HOST_TX) {
        pst_net_dev->features    |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
        pst_net_dev->hw_features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
    }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    oal_netdevice_master(pst_net_dev) = OAL_PTR_NULL;
#endif

    oal_netdevice_ifalias(pst_net_dev) = OAL_PTR_NULL;
    oal_netdevice_watchdog_timeo(pst_net_dev) = 5;
    oal_netdevice_wdev(pst_net_dev) = pst_wdev;
    oal_netdevice_qdisc(pst_net_dev, OAL_PTR_NULL);
}


static uint32_t wal_cfg80211_param_check(oal_wiphy_stru *wiphy, const char *name, oal_vif_params_stru *params)
{
    uint32_t ret = OAL_TRUE;
#ifdef  _PRE_CONFIG_HISI_S3S4_POWER_STATE
    if (g_st_recovery_info.device_s3s4_process_flag) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::s3s4 is processing!}\r\n");
        ret = OAL_FALSE;
    }
#endif

    if (oal_any_null_ptr3(wiphy, name, params)) {
        oam_error_log3(0, OAM_SF_CFG,
            "{wal_cfg80211_add_virtual_intf:: ptr is null,error wiphy %x, name %x, params %x!}\r\n",
            (uintptr_t)wiphy, (uintptr_t)name, (uintptr_t)params);
        ret = OAL_FALSE;
    }

    return ret;
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0))
OAL_STATIC oal_wireless_dev_stru *wal_cfg80211_add_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                                                const char *puc_name,
                                                                unsigned char name_assign_type,
                                                                enum nl80211_iftype en_type,
                                                                oal_vif_params_stru *pst_params)
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
OAL_STATIC oal_wireless_dev_stru *wal_cfg80211_add_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                                                const char *puc_name,
                                                                unsigned char name_assign_type,
                                                                enum nl80211_iftype en_type,
                                                                uint32_t *pul_flags,
                                                                oal_vif_params_stru *pst_params)
#else
OAL_STATIC oal_wireless_dev_stru *wal_cfg80211_add_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                                                const char *puc_name,
                                                                enum nl80211_iftype en_type,
                                                                uint32_t *pul_flags,
                                                                oal_vif_params_stru *pst_params)
#endif
{
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    wlan_vap_mode_enum_uint8 en_vap_mode;
    oal_net_device_stru *pst_net_dev = NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = NULL;
    mac_device_stru *pst_mac_device = NULL;
    hmac_device_stru *pst_hmac_device = NULL;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = NULL;
    uint8_t uc_cfg_vap_id;
    mac_vap_stru *pst_cfg_mac_vap = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = NULL;
    oal_net_device_stru *pst_cfg_net_dev = NULL;
    uint32_t ul_ret;
    int32_t l_ret, l_timeout;
    mac_cfg_add_vap_param_stru *pst_add_vap_param = NULL;
    int8_t ac_name[OAL_IF_NAME_SIZE] = { 0 };
    uint8_t uc_rollback_lock = 0;

    /* 1.1 ��μ�� */
    if (wal_cfg80211_param_check(pst_wiphy, puc_name, pst_params) != OAL_TRUE) {
        return ERR_PTR(-OAL_EINVAL);
    }

    /* ��μ�����쳣��ֵ��������OALͳһ�ӿ� */
    pst_wiphy_priv = oal_wiphy_priv(pst_wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::pst_wiphy_priv is null!}\r\n");
        return ERR_PTR(-OAL_EINVAL);
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::pst_mac_device is null!}\r\n");
        return ERR_PTR(-OAL_EINVAL);
    }

    oam_warning_log1(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::en_type[%d]!}\r\n", en_type);
    /* ��ʾ:SDT�޷���ӡ%s�ַ�����ʽ */
    oal_io_print("wal_cfg80211_add_virtual_intf,dev_name is:%s\n", puc_name);

    switch (en_type) {
        case NL80211_IFTYPE_ADHOC:
        case NL80211_IFTYPE_AP_VLAN:
        case NL80211_IFTYPE_WDS:
        case NL80211_IFTYPE_MESH_POINT:
        case NL80211_IFTYPE_MONITOR:
            oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::Unsupported interface type[%d]!}", en_type);
            return ERR_PTR(-OAL_EINVAL);

        case NL80211_IFTYPE_P2P_DEVICE:
            pst_net_dev = pst_mac_device->st_p2p_info.pst_p2p_net_device;
            pst_wdev = pst_net_dev->ieee80211_ptr;
            return pst_wdev;

        case NL80211_IFTYPE_P2P_CLIENT:
            en_vap_mode = WLAN_VAP_MODE_BSS_STA;
            en_p2p_mode = WLAN_P2P_CL_MODE;
            break;
        case NL80211_IFTYPE_STATION:
            en_vap_mode = WLAN_VAP_MODE_BSS_STA;
            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            break;
        case NL80211_IFTYPE_P2P_GO:
            en_vap_mode = WLAN_VAP_MODE_BSS_AP;
            en_p2p_mode = WLAN_P2P_GO_MODE;
            break;
        case NL80211_IFTYPE_AP:
            en_vap_mode = WLAN_VAP_MODE_BSS_AP;
            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            break;
        default:
            oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::Unsupported interface type[%d]!}", en_type);
            return ERR_PTR(-OAL_EINVAL);
    }

    /* ���������net device�Ѿ����ڣ�ֱ�ӷ��� */
    /* ����dev_name�ҵ�dev */
    pst_net_dev = wal_config_get_netdev(puc_name, OAL_STRLEN(puc_name));
    if (pst_net_dev != OAL_PTR_NULL) {
        /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
        oal_dev_put(pst_net_dev);

        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_virtual_intf::the net_device is already exist!}\r\n");
        pst_wdev = pst_net_dev->ieee80211_ptr;
        return pst_wdev;
    }

    /* ���net_device ǰ���жϵ�ǰ�Ƿ�����ɾ��net_device ״̬��
        �������ɾ��net_device����ȴ�ɾ����ɣ������ */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, 0, "{wal_cfg80211_add_virtual_intf::hmac_device is null!}", pst_mac_device->uc_device_id);
        return ERR_PTR(-OAL_ENODEV);
    }

    if (hmac_get_p2p_status(pst_hmac_device->ul_p2p_intf_status, P2P_STATUS_IF_DELETING) == OAL_TRUE) {
        /* �ȴ�ɾ����� */
        if (rtnl_is_locked()) {
            rtnl_unlock();
            uc_rollback_lock = OAL_TRUE;
        }
        oam_warning_log0(0, 0, "{wal_cfg80211_add_virtual_intf:Released the lock and wait till IF_DEL is complete!}");
        l_timeout = oal_wait_event_interruptible_timeout_m(pst_hmac_device->st_netif_change_event,
            (hmac_get_p2p_status(pst_hmac_device->ul_p2p_intf_status, P2P_STATUS_IF_DELETING) == OAL_FALSE),
            oal_msecs_to_jiffies(WAL_MAX_WAIT_TIME));

        /* put back the rtnl_lock again */
        if (uc_rollback_lock) {
            rtnl_lock();
        }

        if (l_timeout > 0) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_virtual_intf::IF DEL is Success!}\r\n");
        } else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_virtual_intf::timeount < 0, return -EAGAIN!}\r\n");
            return ERR_PTR(-OAL_EAGAIN);
        }
    }

    /* ���wifi �����У�P2P group �Ƿ��Ѿ����������P2P group �Ѿ�������
        �򽫸�P2P group ɾ�����������´���P2P group */
    if (OAL_TRUE == wal_is_p2p_group_exist(pst_mac_device)) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::found exist p2p group, delet it first!}\r\n");
        if (OAL_SUCC != wal_del_p2p_group(pst_mac_device)) {
            return ERR_PTR(-OAL_EAGAIN);
        }
    }

    /* ��ȡ����VAP �ṹ */
    uc_cfg_vap_id = pst_mac_device->uc_cfg_vap_id;
    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(uc_cfg_vap_id);
    if (pst_cfg_mac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, 0, "{wal_cfg80211_add_virtual_intf::pst_cfg_mac_vap is null vap_id:%d!}", uc_cfg_vap_id);
        return ERR_PTR(-OAL_ENODEV);
    }
    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, 0, "{wal_cfg80211_add_virtual_intf::pst_cfg_hmac_vap is null vap_id:%d!}", uc_cfg_vap_id);
        return ERR_PTR(-OAL_ENODEV);
    }
    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (EOK != memcpy_s(ac_name, OAL_IF_NAME_SIZE, puc_name, OAL_STRLEN(puc_name))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_add_virtual_intf::memcpy fail!");
    }

    pst_net_dev = oal_net_alloc_netdev_mqs(OAL_SIZEOF(oal_netdev_priv_stru), ac_name,
        oal_ether_setup, WLAN_NET_QUEUE_BUTT, 1); /* �˺�����һ����δ���˽�г��ȣ��˴����漰Ϊ0 */

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_cfg80211_add_virtual_intf::pst_net_dev null ptr error!}");
        return ERR_PTR(-OAL_ENOMEM);
    }

    pst_wdev = (oal_wireless_dev_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
        OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (oal_unlikely(pst_wdev == OAL_PTR_NULL)) {
        oam_error_log0(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_cfg80211_add_virtual_intf::alloc mem, pst_wdev null ptr!}");
        /* �쳣�����ͷ��ڴ� */
        oal_net_free_netdev(pst_net_dev);
        return ERR_PTR(-OAL_ENOMEM);
    }

    memset_s(pst_wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));

    /* ��netdevice���и�ֵ */
    wal_cfg80211_add_virtual_intf_netdev_init(pst_net_dev, pst_wdev);

    pst_wdev->iftype = en_type;
    pst_wdev->wiphy = pst_mac_device->pst_wiphy;
    pst_wdev->netdev = pst_net_dev; /* ��wdev �е�net_device ��ֵ */
    if (WLAN_VAP_MODE_LEGACY_VAP(en_p2p_mode, en_vap_mode)) {
        /* �������wlan0�� �򱣴�wlan0 Ϊ��net_device,p2p0 ��p2p-p2p0 MAC ��ַ����netdevice ��ȡ */
        if (pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL) {
            /* ����wlan0 �ڼ���wifi ����ʱ������Ҫͨ���ýӿ� */
            oam_error_log0(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_cfg80211_add_virtual_intf::should not go here!}\r\n");
            oal_mem_free_m(pst_wdev, OAL_FALSE);
            oal_net_free_netdev(pst_net_dev);
            return ERR_PTR(-OAL_ENODEV);
        }
    }

    /* ��ֹ�ϲ㲻�·�mac addr,����������p2p-p2p0-x mac addrδ��ʼ�� */
    wal_cfg80211_add_p2p_interface_init(pst_net_dev, pst_mac_device);

    oal_netdevice_flags(pst_net_dev) &= ~OAL_IFF_RUNNING; /* ��net device��flag��Ϊdown */

    ul_ret = wal_cfg80211_register_netdev(pst_net_dev);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        /* ע�᲻�ɹ����ͷ���Դ */
        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return ERR_PTR(-OAL_EBUSY);
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));

    pst_add_vap_param = (mac_cfg_add_vap_param_stru *)(st_write_msg.auc_value);
    pst_add_vap_param->pst_net_dev = pst_net_dev;
    pst_add_vap_param->en_vap_mode = en_vap_mode;
    pst_add_vap_param->uc_cfg_vap_indx = uc_cfg_vap_id;
    pst_add_vap_param->en_p2p_mode = en_p2p_mode;
    pst_add_vap_param->is_primary_vap = 0;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wal_vap_get_wlan_mode_para(&st_write_msg);
#endif
    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
                                   (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_add_virtual_intf::wal_send_cfg_event err code:%d!}", l_ret);
        /*lint -e801*/
        goto ERR_STEP;
        /*lint +e801*/
    }

    /* ��ȡ���صĴ����� */
    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_add_virtual_intf::wal_check_and_release_msg_resp fail:ul_err_code!}");
        /*lint -e801*/
        goto ERR_STEP;
        /*lint +e801*/
    }

    wal_set_random_mac_to_mib(pst_net_dev); /* set random mac to mib ; for hi1102-cb */

    /* ����netdevice��MAC��ַ��MAC��ַ��HMAC�㱻��ʼ����MIB�� */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_add_virtual_intf::oal_net_dev_priv(pst_net_dev) is null ptr.}");
        /*lint -e801*/
        goto ERR_STEP;
        /*lint +e801*/
    }
    oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev), mac_mib_get_StationID(pst_mac_vap));

    /* ����VAP UP */
    wal_netdev_open(pst_net_dev, OAL_FALSE);

    oam_warning_log2(0, OAM_SF_CFG, "{wal_cfg80211_add_virtual_intf::succ. en_type[%d],vap_id[%d]!}\r\n",
                     en_type, pst_mac_vap->uc_vap_id);

    return pst_wdev;

    /* �쳣���� */
ERR_STEP:
    wal_cfg80211_unregister_netdev(pst_net_dev);
    /* ��ȥע�ᣬ���ͷ� */
    oal_mem_free_m(pst_wdev, OAL_FALSE);
    return ERR_PTR(-OAL_EAGAIN);
}

OAL_STATIC oal_bool_enum_uint8 wal_cfg80211_check_is_primary_netdev(oal_wiphy_stru *wiphy,
    oal_net_device_stru *net_dev)
{
    mac_device_stru *mac_device = NULL;
    mac_wiphy_priv_stru *wiphy_priv = NULL;
    wiphy_priv = oal_wiphy_priv(wiphy);
    if (wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_check_is_primary_netdev::pst_wiphy_priv is null!}");
        return OAL_FALSE;
    }
    mac_device = wiphy_priv->pst_mac_device;
    if (mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_check_is_primary_netdev::pst_mac_device is null!}");
        return OAL_FALSE;
    }
    return mac_device->st_p2p_info.pst_primary_net_device == net_dev;
}

OAL_STATIC int32_t wal_cfg80211_del_virtual_intf(oal_wiphy_stru *pst_wiphy,
                                                   oal_wireless_dev_stru *pst_wdev)
{
    /* �첽ȥע��net_device */
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = NULL;
    int32_t l_ret;
    oal_net_device_stru *pst_net_dev = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    hmac_device_stru *pst_hmac_device = NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_wiphy, pst_wdev))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::pst_wiphy or pst_wdev null ptr error %x, %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_wdev);
        return -OAL_EINVAL;
    }

    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf:: dfr or s3s4 is processing!}");
        return OAL_SUCC;
    }

    pst_net_dev = pst_wdev->netdev;
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::pst_net_dev is null by netdev, mode[%d]!}");
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::mac_vap is null by netdev, mode[%d]!}\r\n",
            pst_net_dev->ieee80211_ptr->iftype);
        return -OAL_EINVAL;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, 0, "{wal_cfg80211_del_virtual_intf::get_hmac_vap fail.vap_id[%u]}", pst_mac_vap->uc_vap_id);
        return -OAL_EINVAL;
    }
    if (wal_cfg80211_check_is_primary_netdev(pst_wiphy, pst_net_dev)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::cannot del primary netdev}");
        return -OAL_EINVAL;
    }
    oal_net_tx_stop_all_queues(pst_net_dev);
    wal_netdev_stop(pst_net_dev);
    /* ���ͷ���net_device ���ͷ�wireless device */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    /* ��ʼ��ɾ��vap ���� */
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    pst_wdev = pst_net_dev->ieee80211_ptr;
    if (wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype) == WLAN_P2P_BUTT) {
        oam_error_log0(0, 0, "{wal_cfg80211_del_virtual_intf::wal_wireless_iftype_to_mac_p2p_mode return BUTT}");
        return -OAL_EINVAL;
    }

    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = mac_get_p2p_mode(pst_mac_vap);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

    /* ����ɾ��net_device ��ʶ */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, 0, "{wal_cfg80211_del_virtual_intf::hmac_device is null!}", pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }
    hmac_set_p2p_status(&pst_hmac_device->ul_p2p_intf_status, P2P_STATUS_IF_DELETING);

    /* ����linux work ɾ��net_device */
    pst_hmac_vap->pst_del_net_device = pst_net_dev;
    oal_workqueue_schedule(&(pst_hmac_vap->st_del_virtual_inf_worker));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                                   (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::wal_check_and_release_msg_resp fail}");
    }

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_del_virtual_intf::return err code %d}\r\n", l_ret);
        l_ret = -OAL_EFAIL;
    }

    oam_warning_log2(0, OAM_SF_ANY,
        "{wal_cfg80211_del_virtual_intf::pst_hmac_device->ul_p2p_intf_status %d, del result: %d}\r\n",
        pst_hmac_device->ul_p2p_intf_status, l_ret);

    return l_ret;
}
//#endif


OAL_STATIC int32_t wal_cfg80211_mgmt_tx_cancel_wait(oal_wiphy_stru *pst_wiphy,
                                                      oal_wireless_dev_stru *pst_wdev,
                                                      uint64_t ull_cookie)
{
    oal_net_device_stru *pst_netdev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (!pst_wdev) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx_cancel_wait::pst_wdev is Null!}");
        return -OAL_EFAIL;
    }

    pst_netdev = pst_wdev->netdev;
    if (!pst_netdev) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx_cancel_wait::pst_netdev is Null!}");
        return -OAL_EFAIL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG,
            "{wal_cfg80211_mgmt_tx_cancel_wait::can't get mac vap from netdevice priv data!}");
        return -OAL_EFAIL;
    }

    return wal_p2p_stop_roc(pst_mac_vap, pst_netdev);
}


OAL_STATIC int32_t wal_cfg80211_set_pmksa(oal_wiphy_stru *pst_wiphy,
                                            oal_net_device_stru *pst_net_device,
                                            oal_cfg80211_pmksa_stru *pmksa)
{
    wal_msg_write_stru st_write_msg = { 0 };
    mac_cfg_pmksa_param_stru *pst_cfg_pmksa = NULL;
    int32_t l_ret;
    int32_t l_memcpy_ret;

    if (oal_any_null_ptr3(pst_wiphy, pst_net_device, pmksa)) {
        oam_error_log3(0, OAM_SF_CFG,
                       "{wal_cfg80211_set_pmksa::param null! pst_wiphy[%x], pst_net_device[%x], pmksa[%x]!!}\r\n",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device, (uintptr_t)pmksa);
        return -OAL_EINVAL;
    }

    if (oal_any_null_ptr2(pmksa->bssid, pmksa->pmkid)) {
        oam_error_log2(0, OAM_SF_CFG, "{wal_cfg80211_set_pmksa::param null! bssid[%x] pmkid[%x]}\r\n",
                       (uintptr_t)(pmksa->bssid), (uintptr_t)(pmksa->pmkid));
        return -OAL_EINVAL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_SET_PMKSA, OAL_SIZEOF(mac_cfg_pmksa_param_stru));
    pst_cfg_pmksa = (mac_cfg_pmksa_param_stru *)st_write_msg.auc_value;
    l_memcpy_ret = memcpy_s(pst_cfg_pmksa->auc_bssid, WLAN_MAC_ADDR_LEN, pmksa->bssid, WLAN_MAC_ADDR_LEN);
    l_memcpy_ret += memcpy_s(pst_cfg_pmksa->auc_pmkid, WLAN_PMKID_LEN, pmksa->pmkid, WLAN_PMKID_LEN);
    if (l_memcpy_ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "wal_cfg80211_set_pmksa::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_device,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_pmksa_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_set_pmksa::wal_send_cfg_event fail[%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_del_pmksa(oal_wiphy_stru *pst_wiphy,
                                            oal_net_device_stru *pst_net_device,
                                            oal_cfg80211_pmksa_stru *pmksa)
{
    wal_msg_write_stru st_write_msg = { 0 };
    mac_cfg_pmksa_param_stru *pst_cfg_pmksa = NULL;
    int32_t l_ret;
    int32_t l_memcpy_ret;

    if (oal_any_null_ptr3(pst_wiphy, pst_net_device, pmksa)) {
        oam_error_log3(0, OAM_SF_CFG,
                       "{wal_cfg80211_del_pmksa::param null! pst_wiphy[%d], pst_net_device[%x], pmksa[%x]!!}\r\n",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device, (uintptr_t)pmksa);
        return -OAL_EINVAL;
    }

    if (oal_any_null_ptr2(pmksa->bssid, pmksa->pmkid)) {
        oam_error_log2(0, OAM_SF_CFG, "{wal_cfg80211_del_pmksa::param null! bssid[%x] pmkid[%x]}\r\n",
                       (uintptr_t)(pmksa->bssid), (uintptr_t)(pmksa->pmkid));
        return -OAL_EINVAL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_DEL_PMKSA, OAL_SIZEOF(mac_cfg_pmksa_param_stru));
    pst_cfg_pmksa = (mac_cfg_pmksa_param_stru *)st_write_msg.auc_value;
    l_memcpy_ret = memcpy_s(pst_cfg_pmksa->auc_bssid, WLAN_MAC_ADDR_LEN, pmksa->bssid, WLAN_MAC_ADDR_LEN);
    l_memcpy_ret += memcpy_s(pst_cfg_pmksa->auc_pmkid, WLAN_PMKID_LEN, pmksa->pmkid, WLAN_PMKID_LEN);
    if (l_memcpy_ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "wal_cfg80211_del_pmksa::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_device,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_pmksa_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_cfg80211_del_pmksa::wal_send_cfg_event fail[%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_flush_pmksa(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_net_device)
{
    wal_msg_write_stru st_write_msg = { 0 };
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_any_null_ptr2(pst_wiphy, pst_net_device)) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{wal_cfg80211_flush_pmksa::param null! pst_wiphy[%x], pst_net_device[%x]!!}\r\n",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_net_device);
        return -OAL_EINVAL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_vap_stru �ṹ���Է�wal_alloc_cfg_event����ERROR */
    pst_mac_vap = oal_net_dev_priv(pst_net_device);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_flush_pmksa::can't get mac vap from netdevice priv data!}");
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_FLUSH_PMKSA, 0);

    l_ret = wal_send_cfg_event(pst_net_device,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_cfg80211_flush_pmksa::wal_send_cfg_event fail[%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC void wal_drv_remain_on_channel_debug_info(mac_vap_stru *pst_mac_vap,
    int32_t l_channel, uint32_t ul_duration, uint64_t *pull_cookie, wlan_ieee80211_roc_type_uint8 en_roc_type)
{
    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
        "{wal_drv_remain_on_channel::SUCC! l_channel=%d, ul_duration=%d, cookie 0x%x, roc type=%d!}\r\n",
        l_channel, ul_duration, *pull_cookie, en_roc_type);
}


OAL_STATIC int32_t wal_drv_remain_on_channel(oal_wiphy_stru *pst_wiphy,
                                               oal_wireless_dev_stru *pst_wdev,
                                               struct ieee80211_channel *pst_chan,
                                               uint32_t ul_duration,
                                               uint64_t *pull_cookie,
                                               wlan_ieee80211_roc_type_uint8 en_roc_type)
{
    wal_msg_write_stru st_write_msg = { 0 };
    wal_msg_stru *pst_rsp_msg = NULL;
    uint32_t ul_err_code;
    oal_net_device_stru *pst_netdev = NULL;
    mac_remain_on_channel_param_stru st_remain_on_channel = { 0 };
    uint16_t us_center_freq;
    int32_t l_channel;
    int32_t l_ret;
    mac_device_stru *pst_mac_device = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    mac_vap_stru *pst_other_vap = NULL;
    uint8_t uc_vap_idx;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr4(pst_wiphy, pst_wdev, pst_chan, pull_cookie)) {
        oam_error_log4(0, OAM_SF_P2P, "{wal_drv_remain_on_channel::wiphy/wdev/chan/cookie null, %x, %x, %x, %x!}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_wdev, (uintptr_t)pst_chan, (uintptr_t)pull_cookie);
        return -OAL_EINVAL;
    }

    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_P2P, "{wal_drv_remain_on_channel::pst_netdev ptr is null!}\r\n");
        return -OAL_EINVAL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_device_stru �ṹ */
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_P2P, "{wal_drv_remain_on_channel::can't get mac vap from netdevice priv data!}\r\n");
        return -OAL_EINVAL;
    }

    pst_mac_device = (mac_device_stru *)mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_P2P, "{wal_drv_remain_on_channel::pst_mac_device ptr is null!}\r\n");
        return -OAL_EINVAL;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    if (OAL_TRUE == hmac_user_is_wapi_connected(pst_mac_vap->uc_device_id)) {
        oam_warning_log0(0, OAM_SF_CFG, "{stop p2p remaining under wapi!}");
        return -OAL_EINVAL;
    }
#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

    /* check all vap state in case other vap is connecting now */
    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_other_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (pst_other_vap == OAL_PTR_NULL) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
                "{wal_drv_remain_on_channel::vap is null! vap id is %d}", pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }
        if (oal_value_in_valid_range(pst_other_vap->en_vap_state,
            MAC_VAP_STATE_STA_JOIN_COMP, MAC_VAP_STATE_STA_WAIT_ASOC)
            || (pst_other_vap->en_vap_state == MAC_VAP_STATE_ROAMING)
        ) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
                             "{wal_drv_remain_on_channel::invalid ROC[%d] for roaming or connecting state[%d]!}",
                             en_roc_type, pst_other_vap->en_vap_state);
            return -OAL_EFAIL;
        }
    }

    /* tx mgmt roc ���ȼ���,���Ա��Լ���80211 roc�Լ�80211 scan��� */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{wal_drv_remain_on_channel::new roc type[%d],cancel old roc!}", en_roc_type);
        l_ret = wal_p2p_stop_roc(pst_mac_vap, pst_netdev);
        if (l_ret < 0) {
            return -OAL_EFAIL;
        }
    }

    /* 2.1 ��Ϣ����׼�� */
    us_center_freq = pst_chan->center_freq;
    l_channel = (int32_t)oal_ieee80211_frequency_to_channel((int32_t)us_center_freq);

    st_remain_on_channel.uc_listen_channel = (uint8_t)l_channel;
    st_remain_on_channel.ul_listen_duration = ul_duration;
    st_remain_on_channel.st_listen_channel = *pst_chan;
    st_remain_on_channel.en_listen_channel_type = WLAN_BAND_WIDTH_20M;
    st_remain_on_channel.en_roc_type = en_roc_type;

    if (pst_chan->band == HISI_IEEE80211_BAND_2GHZ) {
        st_remain_on_channel.en_band = WLAN_BAND_2G;
    } else if (pst_chan->band == HISI_IEEE80211_BAND_5GHZ) {
        st_remain_on_channel.en_band = WLAN_BAND_5G;
    } else {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{wal_drv_remain_on_channel::wrong band en_roc_type[%d]!}\r\n", pst_chan->band);
        return -OAL_EINVAL;
    }

    if (en_roc_type == IEEE80211_ROC_TYPE_NORMAL) {
        /* ����cookie ֵ */ /* cookieֵ�ϲ������Ҫ�ж��Ƿ�����εķ��͵��µ�callback */
        *pull_cookie = ++pst_mac_device->st_p2p_info.ull_last_roc_id;
        if (*pull_cookie == 0) {
            *pull_cookie = ++pst_mac_device->st_p2p_info.ull_last_roc_id;
        }

        /* ����cookie ֵ���·���HMAC ��DMAC */
        st_remain_on_channel.ull_cookie = pst_mac_device->st_p2p_info.ull_last_roc_id;
    }
    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* 3.1 ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_REMAIN_ON_CHANNEL,
        OAL_SIZEOF(mac_remain_on_channel_param_stru));

    /* 3.2 ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, WAL_MSG_WRITE_MAX_LEN,
                        &st_remain_on_channel, OAL_SIZEOF(mac_remain_on_channel_param_stru))) {
        oam_error_log0(0, OAM_SF_P2P, "wal_drv_remain_on_channel::memcpy fail!");
    }

    /* 3.3 ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH +
        OAL_SIZEOF(mac_remain_on_channel_param_stru), (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (l_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{wal_drv_remain_on_channel::wal_send_cfg_event return err code:[%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    /* 4.1 ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{wal_drv_remain_on_channel::wal_send_cfg_event return err code:[%u]!}", ul_err_code);
        return -OAL_EFAIL;
    }

    if (en_roc_type == IEEE80211_ROC_TYPE_NORMAL) {
        /* �ϱ���ͣ��ָ���ŵ��ɹ� */
        oal_cfg80211_ready_on_channel(pst_wdev, *pull_cookie, pst_chan, ul_duration, GFP_KERNEL);
    }
    // ��װ������������
    wal_drv_remain_on_channel_debug_info(pst_mac_vap, l_channel, ul_duration, pull_cookie, en_roc_type);

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_remain_on_channel(oal_wiphy_stru *pst_wiphy,
                                                    oal_wireless_dev_stru *pst_wdev,
                                                    struct ieee80211_channel *pst_chan,
                                                    uint32_t ul_duration,
                                                    uint64_t *pull_cookie)
{
    return wal_drv_remain_on_channel(pst_wiphy, pst_wdev, pst_chan, ul_duration,
                                     pull_cookie, IEEE80211_ROC_TYPE_NORMAL);
}


OAL_STATIC int32_t wal_cfg80211_cancel_remain_on_channel(oal_wiphy_stru *pst_wiphy,
                                                           oal_wireless_dev_stru *pst_wdev,
                                                           uint64_t ull_cookie)
{
    wal_msg_write_stru st_write_msg = { 0 };
    mac_remain_on_channel_param_stru st_cancel_remain_on_channel = { 0 };
    wal_msg_stru *pst_rsp_msg = NULL;
    uint32_t ul_err_code;
    oal_net_device_stru *pst_netdev = NULL;
    int32_t l_ret;

    if (oal_any_null_ptr2(pst_wiphy, pst_wdev)) {
        oam_error_log2(0, OAM_SF_P2P,
            "{wal_cfg80211_cancel_remain_on_channel::pst_wiphy or pst_wdevis null,error %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_wdev);
        return -OAL_EINVAL;
    }

    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_P2P, "{wal_cfg80211_cancel_remain_on_channel::pst_netdev ptr is null!}\r\n");
        return -OAL_EINVAL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    oam_warning_log1(0, OAM_SF_P2P, "wal_cfg80211_cancel_remain_on_channel[0x%x].", ull_cookie);

    /* 3.1 ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_CANCEL_REMAIN_ON_CHANNEL,
        OAL_SIZEOF(mac_remain_on_channel_param_stru));

    /* 3.2 ��д msg ��Ϣ�� */
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_remain_on_channel_param_stru),
                        &st_cancel_remain_on_channel, OAL_SIZEOF(mac_remain_on_channel_param_stru))) {
        oam_error_log0(0, OAM_SF_P2P, "wal_cfg80211_cancel_remain_on_channel::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* 3.3 ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_remain_on_channel_param_stru),
                                   (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_P2P,
            "{wal_cfg80211_cancel_remain_on_channel::wal_send_cfg_event return err code: [%d]!}", l_ret);
        return -OAL_EFAIL;
    }

    /* 4.1 ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_P2P,
            "{wal_cfg80211_cancel_remain_on_channel::wal_check_and_release_msg_resp fail return err code:%u!}",
            ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


uint8_t wal_find_oldest_cookie(cookie_arry_stru *pst_cookie_array)
{
    uint8_t uc_loops = 0;
    uint8_t uc_target_index = 0;

    /* �ⲿ��ֻ֤����cookie����������²Ż���øú�������˳�ʼindexѡ0���� */
    for (uc_loops = 1; uc_loops < WAL_COOKIE_ARRAY_SIZE; uc_loops++) {
        if (oal_time_after32(pst_cookie_array[uc_target_index].ul_record_time,
            pst_cookie_array[uc_loops].ul_record_time)) {
            uc_target_index = uc_loops;
        }
    }

    return uc_target_index;
}


void wal_check_cookie_timeout(cookie_arry_stru *pst_cookie_array,
                                  uint8_t *puc_cookie_bitmap,
                                  uint32_t ul_current_time)
{
    uint8_t uc_loops = 0;
    cookie_arry_stru *pst_tmp_cookie = NULL;

    oam_warning_log0(0, OAM_SF_CFG, "{wal_check_cookie_timeout::time_out!}\r\n");
    for (uc_loops = 0; uc_loops < WAL_COOKIE_ARRAY_SIZE; uc_loops++) {
        pst_tmp_cookie = &pst_cookie_array[uc_loops];
        if (oal_time_after32(OAL_TIME_JIFFY,
            pst_tmp_cookie->ul_record_time + oal_msecs_to_jiffies(WAL_MGMT_TX_TIMEOUT_MSEC))) {
            /* cookie array �б����cookie ֵ��ʱ */
            /* ���cookie array �г�ʱ��cookie */
            pst_tmp_cookie->ul_record_time = 0;
            pst_tmp_cookie->ull_cookie = 0;
            /* ���ռ�õ�cookie bitmapλ */
            oal_bit_clear_bit_one_byte(puc_cookie_bitmap, uc_loops);
        }
    }
}


OAL_STATIC uint32_t wal_del_cookie_from_array(cookie_arry_stru *pst_cookie_array,
                                     uint8_t *puc_cookie_bitmap,
                                     uint8_t uc_cookie_idx)
{
    cookie_arry_stru *pst_tmp_cookie = NULL;

    /* �����Ӧcookie bitmap λ */
    oal_bit_clear_bit_one_byte(puc_cookie_bitmap, uc_cookie_idx);

    /* ���cookie array �г�ʱ��cookie */
    pst_tmp_cookie = &pst_cookie_array[uc_cookie_idx];
    pst_tmp_cookie->ull_cookie = 0;
    pst_tmp_cookie->ul_record_time = 0;
    return OAL_SUCC;
}


uint32_t wal_add_cookie_to_array(cookie_arry_stru *pst_cookie_array,
                                   uint8_t *puc_cookie_bitmap,
                                   uint64_t *pull_cookie,
                                   uint8_t *puc_cookie_idx)
{
    uint8_t uc_idx;
    cookie_arry_stru *pst_tmp_cookie = NULL;

    if (*puc_cookie_bitmap == WAL_COOKIE_FULL_MASK) {
        /* cookie array �������ش��� */
        oam_warning_log0(0, OAM_SF_CFG, "{wal_add_cookie_to_array::array full!}\r\n");
        return OAL_FAIL;
    }

    /* ��cookie ��ӵ�array �� */
    uc_idx = oal_bit_find_first_zero_one_byte(*puc_cookie_bitmap);
    oal_bit_set_bit_one_byte(puc_cookie_bitmap, uc_idx);

    pst_tmp_cookie = &pst_cookie_array[uc_idx];
    pst_tmp_cookie->ull_cookie = *pull_cookie;
    pst_tmp_cookie->ul_record_time = OAL_TIME_JIFFY;

    *puc_cookie_idx = uc_idx;
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_check_cookie_from_array(uint8_t *puc_cookie_bitmap, uint8_t uc_cookie_idx)
{
    /* ��cookie bitmap�в�����Ӧ��cookie index�����λͼΪ0����ʾ�Ѿ���del */
    if (*puc_cookie_bitmap & (BIT(uc_cookie_idx))) {
        return OAL_SUCC;
    }
    /* �Ҳ����򷵻�FAIL */
    return OAL_FAIL;
}

OAL_STATIC oal_netbuf_stru *wal_mgmt_tx_prepare_netbuf(OAL_CONST uint8_t *buff, size_t len)
{
    oal_netbuf_stru *netbuf;
    int32_t ret;

    /*  ����netbuf �ռ� */
    netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, len, OAL_NETBUF_PRIORITY_MID);

    /* Reserved Memory pool tried for high priority frames */
    if (netbuf == NULL && len <= WLAN_MEM_NETBUF_SIZE2) {
        netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
            WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    }

    if (netbuf == NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{wal_mgmt_tx_prepare_netbuf::alloc netbuf failed, len %d.}", len);
        return NULL;
    }

    /* ���netbuf */
    ret = memcpy_s(oal_netbuf_header(netbuf), len, buff, len);
    if (ret != EOK) {
        oam_error_log1(0, OAM_SF_P2P, "{wal_mgmt_tx_prepare_netbuf::memcpy fail!errno[%d]}", ret);
        oal_netbuf_free(netbuf);
        return NULL;
    }

    oal_netbuf_put(netbuf, len);
    oal_mem_netbuf_trace(netbuf, OAL_TRUE);
    return netbuf;
}


OAL_STATIC uint32_t wal_mgmt_do_tx(oal_net_device_stru *pst_netdev,
                                   mac_mgmt_frame_stru *pst_mgmt_tx_param,
                                   bool en_offchan,
                                   uint32_t ul_wait,
                                   OAL_CONST uint8_t *buff,
                                   size_t len)
{
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_mgmt_tx_stru *pst_mgmt_tx = NULL;
    int32_t l_ret;
    signed long i_leftime;

    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_mgmt_do_tx::can't get mac vap from netdevice priv data.}");
        return OAL_FAIL;
    }

    if (!IS_P2P_GO(pst_mac_vap)) {
        // �������Go�����������ж�
        if (en_offchan == OAL_TRUE && pst_mac_vap->en_vap_state != MAC_VAP_STATE_STA_LISTEN) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                "{wal_mgmt_do_tx::pst_mac_vap state[%d]not in listen!}", pst_mac_vap->en_vap_state);
            return OAL_ERR_CODE_WRONG_CHANNEL;  // ��������ش�,�����ϲ�˴�tx mgmtʧ�ܽ���
        }
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_mgmt_do_tx::pst_hmac_vap ptr is null!}");
        return OAL_FAIL;
    }

    pst_mgmt_tx_param->frame = wal_mgmt_tx_prepare_netbuf(buff, len);
    if (pst_mgmt_tx_param->frame == NULL) {
        return OAL_FAIL;
    }

    pst_mgmt_tx = &(pst_hmac_vap->st_mgmt_tx);
    pst_mgmt_tx->mgmt_tx_complete = OAL_FALSE;
    pst_mgmt_tx->mgmt_tx_status = OAL_FAIL;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    l_ret = wal_cfg80211_start_req(pst_netdev, pst_mgmt_tx_param, OAL_SIZEOF(mac_mgmt_frame_stru),
        WLAN_CFGID_CFG80211_MGMT_TX, OAL_FALSE);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oal_netbuf_free(pst_mgmt_tx_param->frame); // need rsp��������false������fail˵���¼�û���׳ɹ�����Ҫ�ͷ�netbuf��
        pst_mgmt_tx_param->frame = NULL;
        oam_warning_log1(0, OAM_SF_ANY, "{wal_mgmt_do_tx::wal_send_cfg_event return err code %d!}", l_ret);
        return OAL_FAIL;
    }

    /*lint -e730*/
    oam_info_log1(0, OAM_SF_ANY, "{wal_mgmt_do_tx:: It will wait %d ms at most for tx status report!}", ul_wait);
    i_leftime = oal_wait_event_interruptible_timeout_m(pst_mgmt_tx->st_wait_queue,
        (oal_bool_enum_uint8)(OAL_TRUE == pst_mgmt_tx->mgmt_tx_complete),
        (uint32_t)oal_msecs_to_jiffies(ul_wait));
    if (i_leftime == 0) {
        /* ��ʱ����ʱ */
        oam_warning_log0(0, OAM_SF_ANY, "{wal_mgmt_do_tx::mgmt tx timeout!}");
        return OAL_FAIL;
    }
    else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        oam_warning_log0(0, OAM_SF_ANY, "{wal_mgmt_do_tx::mgmt tx timer error!}");
        return OAL_FAIL;
    }
    else {
        /* ��������  */
        oam_info_log0(0, OAM_SF_ANY, "{wal_mgmt_do_tx::mgmt tx commpleted!}");
        return pst_mgmt_tx->mgmt_tx_status;
    }
}

OAL_STATIC int32_t wal_cfg80211_mgmt_tx_bss_ap_need_offchan(hmac_vap_stru *pst_hmac_vap,
    mac_mgmt_frame_stru *pst_mgmt_tx, mac_device_stru *pst_mac_device,
    oal_wireless_dev_stru *pst_roc_wireless_dev, bool *pen_need_offchan)
{
    // �˴�Ӧ����channel number��������channel index
    oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{wal_cfg80211_mgmt_tx::p2p mode[%d](0=Legacy, 1=Go, 2=Dev, 3=Gc), tx mgmt vap chan[%d], mgmt tx chan[%d]",
        pst_hmac_vap->st_vap_base_info.en_p2p_mode, pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
        pst_mgmt_tx->channel);

    if ((pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number != pst_mgmt_tx->channel) &&
        IS_P2P_GO(&pst_hmac_vap->st_vap_base_info)) {
        if (pst_mac_device->st_p2p_info.pst_p2p_net_device == OAL_PTR_NULL) {
            oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::go,p2p dev null");
            return -OAL_EINVAL;
        }
        // ʹ��p2p0 roc
        pst_roc_wireless_dev = oal_netdevice_wdev(pst_mac_device->st_p2p_info.pst_p2p_net_device);
        *pen_need_offchan = OAL_TRUE;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_mgmt_tx(oal_wiphy_stru *pst_wiphy,
                                          oal_wireless_dev_stru *pst_wdev,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
                                          struct cfg80211_mgmt_tx_params *pst_params,
#else
                                          oal_ieee80211_channel *pst_chan,
                                          bool en_offchan,
                                          uint32_t ul_wait,
                                          OAL_CONST uint8_t *puc_buf,
                                          size_t ul_len,
                                          bool en_no_cck,
                                          bool en_dont_wait_for_ack,
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)) */
                                          uint64_t *pull_cookie)
{
    oal_net_device_stru *pst_netdev = NULL;
    mac_device_stru *pst_mac_device = NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    OAL_CONST oal_ieee80211_mgmt *pst_mgmt = NULL;
    int32_t ul_ret = 0;
    mac_mgmt_frame_stru st_mgmt_tx;
    uint8_t uc_cookie_idx;
    uint8_t uc_retry;
    mac_p2p_info_stru *pst_p2p_info = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_mgmt_tx_stru *pst_mgmt_tx = NULL;
    oal_wireless_dev_stru *pst_roc_wireless_dev = pst_wdev;
    unsigned long ul_start_time_stamp;
    bool en_need_offchan = OAL_FALSE;
    uint32_t temp_flag;
    oal_ieee80211_channel st_chan = { 0 };

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    oal_ieee80211_channel *pst_chan;
    OAL_CONST uint8_t *puc_buf;
    oal_size_t ul_len;
    uint32_t ul_wait;
    bool en_offchan = OAL_FALSE;
    if (pst_params == OAL_PTR_NULL) {
        return -OAL_EINVAL;
    }
    pst_chan = pst_params->chan;
    puc_buf = pst_params->buf;
    ul_len = pst_params->len;
    en_offchan = pst_params->offchan;
    ul_wait = pst_params->wait;
    /* 1106 FPGA�ŵ��л�ʱ�䳤����Ҫ���Ӽ���ʱ���Ա�֤����֡����ʱ��,��Ƭ���ɾ�� */
    ul_wait = wlan_chip_update_cfg80211_mgmt_tx_wait_time(ul_wait);
#endif

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr4(pst_wiphy, pst_wdev, pull_cookie, puc_buf)) {
        oam_error_log4(0, 0, "{wal_cfg80211_mgmt_tx::para null:wiphy/wdev/pull_cookie/puc_buf:%x, %x, %x, %x}",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_wdev, (uintptr_t)pull_cookie, (uintptr_t)puc_buf);
        return -OAL_EINVAL;
    }

    /* ͨ��net_device �ҵ���Ӧ��mac_device_stru �ṹ */
    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::pst_netdev ptr is null!}\r\n");
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::can't get mac vap from netdevice priv data!}\r\n");
        return -OAL_EINVAL;
    }

    pst_mac_device = (mac_device_stru *)mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::pst_hmac_vap ptr is null!}\r\n");
        return -OAL_EINVAL;
    }

    pst_p2p_info = &pst_mac_device->st_p2p_info;
    *pull_cookie = pst_p2p_info->ull_send_action_id++; /* cookieֵ�ϲ������Ҫ�ж��Ƿ�����εķ��͵��µ�callback */
    if (*pull_cookie == 0) {
        *pull_cookie = pst_p2p_info->ull_send_action_id++;
    }
    pst_mgmt = (const struct ieee80211_mgmt *)puc_buf;

    temp_flag = oal_ieee80211_is_probe_resp(pst_mgmt->frame_control);
    temp_flag = !hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH) ? temp_flag :
        (temp_flag && (pst_mac_vap->st_probe_resp_ctrl.en_probe_resp_status == MAC_PROBE_RESP_MODE_ACTIVE));
    if (temp_flag) {
        *pull_cookie = 0; /* set cookie default value */
        /* host should not send PROE RESPONSE,
           device will send immediately when receive probe request packet */
        oal_cfg80211_mgmt_tx_status(pst_wdev, *pull_cookie, puc_buf, ul_len, OAL_TRUE, GFP_KERNEL);
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    if (pst_chan == OAL_PTR_NULL) {
        mac_bss_dscr_stru *pst_bss_dscr;
        uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];

        if (!IS_STA(pst_mac_vap)) {
            oam_error_log2(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::chan null! vap_mode[%d], p2p_mode[%d]}",
                           pst_mac_vap->en_vap_mode, pst_mac_vap->en_p2p_mode);
            return -OAL_EINVAL;
        }

        /* SAE����auth commit/comfirm ֡���·���pst_channel�����NULL,
         * ��Ҫ����Ŀ��MAC��ɨ�����в����ŵ���Ϣ */
        oam_warning_log0(0, OAM_SF_SAE, "{wal_cfg80211_mgmt_tx::STA tx frame, find channel from scan result}");
        /* �жϴ����֡�����Ƿ񹻳� */
        if (ul_len < MAC_80211_FRAME_LEN) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::auth frame len too short.}", ul_len);
            return -OAL_EINVAL;
        }

        if (EOK != memcpy_s(auc_mac_addr, WLAN_MAC_ADDR_LEN, pst_mgmt->da, WLAN_MAC_ADDR_LEN)) {
            return -OAL_EINVAL;
        }
        pst_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(pst_mac_vap, auc_mac_addr);
        if (pst_bss_dscr == OAL_PTR_NULL) {
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_SAE,
                "{wal_cfg80211_mgmt_tx::can not find [%02X:XX:XX:XX:%02X:%02X] from scan result.}",
                auc_mac_addr[0], auc_mac_addr[4], auc_mac_addr[5]);
            return -OAL_EINVAL;
        }
        st_chan.band = pst_bss_dscr->st_channel.en_band;
        st_chan.center_freq = oal_ieee80211_channel_to_frequency(pst_bss_dscr->st_channel.uc_chan_number,
            pst_bss_dscr->st_channel.en_band);
        st_chan.hw_value = pst_bss_dscr->st_channel.uc_chan_number;
    } else {
        st_chan = *pst_chan;
    }
#else
    if (pst_chan == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx:: pst_chan ptr is null!}");
        return -OAL_EINVAL;
    }
    st_chan = *pst_chan;
#endif /* _PRE_WLAN_FEATURE_SAE */

    /* 2.1 ��Ϣ����׼�� */
    memset_s(&st_mgmt_tx, OAL_SIZEOF(st_mgmt_tx), 0, OAL_SIZEOF(st_mgmt_tx));

    st_mgmt_tx.channel = oal_ieee80211_frequency_to_channel(st_chan.center_freq);

    if (WAL_COOKIE_FULL_MASK == g_uc_cookie_array_bitmap) {
        uc_cookie_idx = wal_find_oldest_cookie(g_cookie_array);
        wal_del_cookie_from_array(g_cookie_array, &g_uc_cookie_array_bitmap, uc_cookie_idx);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx::cookies is [0x%x] after clear}",
            g_uc_cookie_array_bitmap);
    }

    ul_ret = wal_add_cookie_to_array(g_cookie_array, &g_uc_cookie_array_bitmap, pull_cookie, &uc_cookie_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::Fail to add cooki, ret[%d]!}", ul_ret);
        return -OAL_EINVAL;
    } else {
        st_mgmt_tx.mgmt_frame_id = uc_cookie_idx;
    }

    pst_mgmt_tx = &(pst_hmac_vap->st_mgmt_tx);
    pst_mgmt_tx->mgmt_tx_complete = OAL_FALSE;
    pst_mgmt_tx->mgmt_tx_status = OAL_FAIL;

    switch (pst_hmac_vap->st_vap_base_info.en_vap_mode) {
        case WLAN_VAP_MODE_BSS_AP:
            if (wal_cfg80211_mgmt_tx_bss_ap_need_offchan(pst_hmac_vap, &st_mgmt_tx, pst_mac_device,
                pst_roc_wireless_dev, &en_need_offchan) == -OAL_EINVAL) {
                return -OAL_EINVAL;
            }
            break;

        /* P2P CL DEV */
        case WLAN_VAP_MODE_BSS_STA:
            if ((en_offchan == OAL_TRUE) && (pst_wiphy->flags & WIPHY_FLAG_OFFCHAN_TX)) {
                en_need_offchan = OAL_TRUE;
            }
            if ((pst_hmac_vap->st_vap_base_info.en_p2p_mode == WLAN_LEGACY_VAP_MODE) &&
                (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_UP) &&
                (st_mgmt_tx.channel == pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number)) {
                en_need_offchan = OAL_FALSE;
            }
            break;

        default:
            break;
    }

    if ((en_need_offchan == OAL_TRUE) && (st_chan.center_freq == 0)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::need offchan but chan null} ", en_offchan);
        return -OAL_EINVAL;
    }

    if (ul_wait == 0) {
        ul_wait = WAL_MGMT_TX_TIMEOUT_MSEC;
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::wait time is 0, set to %d ms}", ul_wait);
    }

    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{wal_cfg80211_mgmt_tx::offchan[%d].chan[%d]vap state[%d],wait[%d]}",
        en_need_offchan, st_mgmt_tx.channel, pst_hmac_vap->st_vap_base_info.en_vap_state, ul_wait);

    /* ��Ҫoffchannel,��������е���Ӧ���ŵ�XXms */
    if (en_need_offchan == OAL_TRUE) {
        ul_ret = wal_drv_remain_on_channel(pst_wiphy, pst_roc_wireless_dev, &st_chan,
            ul_wait, pull_cookie, IEEE80211_ROC_TYPE_MGMT_TX);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{wal_cfg80211_mgmt_tx::wal_drv_remain_on_channel[%d]!!!offchannel[%d].channel[%d],vap state[%d]}\r\n",
                ul_ret, en_need_offchan, st_mgmt_tx.channel, pst_hmac_vap->st_vap_base_info.en_vap_state);

            oal_cfg80211_mgmt_tx_status(pst_wdev, *pull_cookie, puc_buf, ul_len, OAL_FALSE, GFP_KERNEL);
            return OAL_SUCC;
        }
    }

    ul_start_time_stamp = OAL_TIME_JIFFY;

    uc_retry = 0;
    /* ����ʧ�ܣ������ش� */
    do {
        ul_ret = wal_mgmt_do_tx(pst_netdev, &st_mgmt_tx, en_need_offchan, ul_wait, puc_buf, ul_len);
        uc_retry++;
    } while (oal_value_ne_all2(ul_ret, OAL_SUCC, OAL_ERR_CODE_WRONG_CHANNEL) && (uc_retry <= WAL_MGMT_TX_RETRY_CNT)
             && time_before(OAL_TIME_JIFFY, ul_start_time_stamp + oal_msecs_to_jiffies(ul_wait))); /*lint !e666*/
    // ������������Device Discoverability Response��֡ʱ�����ܻ����ڶԷ�������������ʱʧ�ܡ�
    if (uc_retry > WAL_MGMT_TX_RETRY_CNT) {
        oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_mgmt_tx::retry count[%d]>max[%d],tx status[%d],stop tx mgmt}\r\n",
            uc_retry, WAL_MGMT_TX_RETRY_CNT, ul_ret);
    }

    if (ul_ret != OAL_SUCC) {
        /* ����ʧ�ܣ�����ʱ֡��bitmap */
        wal_check_cookie_timeout(g_cookie_array, &g_uc_cookie_array_bitmap, OAL_TIME_JIFFY);

        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_mgmt_tx::vap status[%d], tx mgmt timeout}\r\n", pst_mac_vap->en_vap_state);

        if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
            if (pst_hmac_vap->st_vap_base_info.en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
                mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                    pst_mac_device->st_p2p_info.en_last_vap_state);
            } else {
                mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                    pst_hmac_vap->st_vap_base_info.en_last_vap_state);
            }

            /* ���¼���DMAC �����ؼ����ŵ� */
            hmac_p2p_send_listen_expired_to_device(pst_hmac_vap);
        }

        oal_cfg80211_mgmt_tx_status(pst_wdev, *pull_cookie, puc_buf, ul_len, OAL_FALSE, GFP_KERNEL);
    } else {
        /* ��������  */
        *pull_cookie = g_cookie_array[pst_mgmt_tx->mgmt_frame_id].ull_cookie;
        wal_del_cookie_from_array(g_cookie_array, &g_uc_cookie_array_bitmap, pst_mgmt_tx->mgmt_frame_id);
        oal_cfg80211_mgmt_tx_status(pst_wdev, *pull_cookie, puc_buf, ul_len, OAL_TRUE, GFP_KERNEL);
    }

    // ������������Device Discoverability Response��֡ʱ�����ܻ����ڶԷ�������������ʱʧ�ܡ�
    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx::tx status[%d], retry cnt[%d]}, delta_time[%d]",
                     ul_ret, uc_retry, oal_jiffies_to_msecs(OAL_TIME_JIFFY - ul_start_time_stamp));

    return OAL_SUCC;
}


uint32_t wal_cfg80211_mgmt_tx_status(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = NULL;
    dmac_crx_mgmt_tx_status_stru *pst_mgmt_tx_status_param = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_mgmt_tx_stru *pst_mgmt_tx = NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx_status::pst_event_mem is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_TX, "{wal_cfg80211_mgmt_tx_status::pst_hmac_vap null.vap_id[%d]}",
            pst_event->st_event_hdr.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_tx_status_param = (dmac_crx_mgmt_tx_status_stru *)(pst_event->auc_event_data);
    pst_mgmt_tx = &(pst_hmac_vap->st_mgmt_tx);
    pst_mgmt_tx->mgmt_tx_complete = OAL_TRUE;
    pst_mgmt_tx->mgmt_tx_status = pst_mgmt_tx_status_param->uc_dscr_status;
    pst_mgmt_tx->mgmt_frame_id = pst_mgmt_tx_status_param->mgmt_frame_id;

    /* �Ҳ�����Ӧ��cookieֵ��˵���Ѿ���ʱ����������Ҫ�ٻ��� */
    if (OAL_SUCC == wal_check_cookie_from_array(&g_uc_cookie_array_bitmap, pst_mgmt_tx->mgmt_frame_id)) {
        /* �ñ������Ż�ʱ��֤OAL_WAIT_QUEUE_WAKE_UP�����ִ�� */
        oal_smp_mb();
        oal_wait_queue_wake_up_interrupt(&pst_mgmt_tx->st_wait_queue);
    }

    return OAL_SUCC;
}

/* P2P ����ȱʧ��CFG80211�ӿ� */
void wal_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
                                              struct wireless_dev *wdev,
                                              uint16_t frame_type,
                                              bool reg)
{
    if (wiphy == OAL_PTR_NULL || wdev == OAL_PTR_NULL) {
        oam_error_log2(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_frame_register::wiphy[%x], wdev[%x]}",
                       (uintptr_t)wiphy, (uintptr_t)wdev);
        return;
    }

    oam_info_log3(0, OAM_SF_CFG,
        "{wal_cfg80211_mgmt_frame_register::enter.frame_type[0x%02x], reg[%d], if_type[%d]}",
        frame_type, reg, wdev->iftype);

    return;
}

int32_t wal_cfg80211_set_bitrate_mask(struct wiphy *wiphy,
                                            struct net_device *dev,
                                            const uint8_t *peer,
                                            const struct cfg80211_bitrate_mask *mask)
{
    oam_info_log0(0, OAM_SF_CFG, "{wal_cfg80211_set_bitrate_mask::enter 000.}");

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfg80211_start_p2p_device(oal_wiphy_stru *pst_wiphy,
                                                   oal_wireless_dev_stru *pst_wdev)
{
    /* TBD */
    return -OAL_EFAIL;
}


OAL_STATIC void wal_cfg80211_stop_p2p_device(oal_wiphy_stru *pst_wiphy,
                                             oal_wireless_dev_stru *pst_wdev)
{
    /* TBD */
}


static int32_t wal_cfg80211_set_power_mgmt(oal_wiphy_stru *pst_wiphy,
                                             oal_net_device_stru *pst_netdev,
                                             bool enabled, int32_t timeout)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_ps_open_stru *pst_sta_pm_open = NULL;
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_wiphy, pst_netdev))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_cfg80211_set_power_mgmt::pst_wiphy or pst_wdev null ptrerror %x, %x!}\r\n",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_netdev);
        return -OAL_EINVAL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_STA_PM_ON, OAL_SIZEOF(mac_cfg_ps_open_stru));

#if 1
    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_warning_log0(0, OAM_SF_PWR, "{wal_cfg80211_set_power_mgmt::get mac vap failed.}");
        return OAL_SUCC;
    }

    /* P2P dev���·� */
    if (IS_P2P_DEV(pst_mac_vap)) {
        oam_warning_log0(0, OAM_SF_PWR, "wal_cfg80211_set_power_mgmt:vap is p2p dev return");
        return OAL_SUCC;
    }

    oam_warning_log3(0, OAM_SF_PWR, "{wal_cfg80211_set_power_mgmt::vap mode[%d]p2p mode[%d]set pm:[%d]}",
        pst_mac_vap->en_vap_mode, pst_mac_vap->en_p2p_mode, enabled);
#endif
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)(st_write_msg.auc_value);
    /* MAC_STA_PM_SWITCH_ON / MAC_STA_PM_SWITCH_OFF */
    pst_sta_pm_open->uc_pm_enable = enabled;
    pst_sta_pm_open->uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_HOST;

    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_open_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_power_mgmt::fail to send pm cfg msg, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_11R


OAL_STATIC int32_t wal_cfg80211_update_ft_ies(oal_wiphy_stru *pst_wiphy,
                                                oal_net_device_stru *pst_netdev,
                                                oal_cfg80211_update_ft_ies_stru *pst_fties)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg80211_ft_ies_stru *pst_mac_ft_ies;
    wal_msg_stru *pst_rsp_msg;
    uint32_t ul_err_code;
    int32_t l_ret;

    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_fties)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies::param is null.}\r\n");

        return -OAL_EINVAL;
    }

    if ((pst_fties->ie == OAL_PTR_NULL) || (pst_fties->ie_len == 0) || (pst_fties->ie_len >= MAC_MAX_FTE_LEN)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies::unexpect ie or len[%d].}\r\n", pst_fties->ie_len);

        return -OAL_EINVAL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    st_write_msg.en_wid = WLAN_CFGID_SET_FT_IES;
    st_write_msg.us_len = OAL_SIZEOF(mac_cfg80211_ft_ies_stru);

    pst_mac_ft_ies = (mac_cfg80211_ft_ies_stru *)st_write_msg.auc_value;
    pst_mac_ft_ies->us_mdid = pst_fties->md;
    pst_mac_ft_ies->us_len = pst_fties->ie_len;
    if (EOK != memcpy_s(pst_mac_ft_ies->auc_ie, pst_fties->ie_len, pst_fties->ie, pst_fties->ie_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_update_ft_ies::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg80211_ft_ies_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfg80211_update_ft_ies::wal_send_cfg_event: return err code %d!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfg80211_update_ft_ies::wal_check_and_release_msg_resp fail return err code:[%u]!}\r\n",
            ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_11R


int32_t wal_cfg80211_dump_survey(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
                                       int32_t l_idx, oal_survey_info_stru *pst_info)
{
    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_dump_survey:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    return hmac_cfg80211_dump_survey(pst_wiphy, pst_netdev, l_idx, pst_info);
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
OAL_STATIC void wal_cfg80211_abort_scan(oal_wiphy_stru *pst_wiphy,
                                            oal_wireless_dev_stru *pst_wdev)
{
    oal_net_device_stru *pst_netdev;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_wiphy, pst_wdev)) {
        oam_error_log2(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::wiphy or wdev is null, %p, %p!}\r\n",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_wdev);
        return;
    }

    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::netdev is null!}\r\n");
        return;
    }
    oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::enter!}\r\n");
    wal_force_scan_abort_then_scan_comp(pst_netdev);
    return;
}
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#ifdef _PRE_WLAN_FEATURE_SAE

OAL_STATIC int32_t wal_cfg80211_external_auth(oal_wiphy_stru *pst_wiphy,
                                                oal_net_device_stru *pst_netdev,
                                                oal_cfg80211_external_auth_stru *pst_external_auth_params)
{
    uint32_t ul_ret;
    int32_t l_ret;
    hmac_external_auth_req_stru st_ext_auth;

    memset_s(&st_ext_auth, OAL_SIZEOF(st_ext_auth), 0, OAL_SIZEOF(st_ext_auth));
    st_ext_auth.us_status = pst_external_auth_params->status;
    l_ret = memcpy_s(st_ext_auth.auc_bssid, WLAN_MAC_ADDR_LEN, pst_external_auth_params->bssid, WLAN_MAC_ADDR_LEN);
    st_ext_auth.st_ssid.uc_ssid_len = oal_min(pst_external_auth_params->ssid.ssid_len, OAL_IEEE80211_MAX_SSID_LEN);
    l_ret += memcpy_s(st_ext_auth.st_ssid.auc_ssid, OAL_IEEE80211_MAX_SSID_LEN,
                      pst_external_auth_params->ssid.ssid, st_ext_auth.st_ssid.uc_ssid_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SAE, "wal_cfg80211_external_auth::memcpy fail!");
        return -OAL_EFAIL;
    }

    oam_warning_log4(0, OAM_SF_SAE, "{wal_cfg80211_external_auth::status %d, bssid[%02X:XX:XX:XX:%02X:%02X]}",
                     st_ext_auth.us_status,
                     st_ext_auth.auc_bssid[0],
                     st_ext_auth.auc_bssid[4],
                     st_ext_auth.auc_bssid[5]);

    ul_ret = wal_cfg80211_do_external_auth(pst_netdev, &st_ext_auth);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_SAE, "{wal_cfg80211_external_auth::do external auth fail. ret %d}", ul_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif /* _PRE_WLAN_FEATURE_SAE */
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0) */

/* ��ͬ����ϵͳ����ָ��ṹ�巽ʽ��ͬ */
OAL_STATIC oal_cfg80211_ops_stru g_wal_cfg80211_ops = {
    .scan = wal_cfg80211_scan,
    .connect = wal_cfg80211_connect,
    .disconnect = wal_cfg80211_disconnect,
    .add_key = wal_cfg80211_add_key,
    .get_key = wal_cfg80211_get_key,
    .del_key = wal_cfg80211_remove_key,
    .set_default_key = wal_cfg80211_set_default_key,
    .set_default_mgmt_key = wal_cfg80211_set_default_mgmt_key,
    .set_wiphy_params = wal_cfg80211_set_wiphy_params,
    .change_beacon = wal_cfg80211_change_beacon,
    .start_ap = wal_cfg80211_start_ap,
    .stop_ap = wal_cfg80211_stop_ap,
    .change_bss = wal_cfg80211_change_bss,
    .sched_scan_start = wal_cfg80211_sched_scan_start,
    .sched_scan_stop = wal_cfg80211_sched_scan_stop,
    .change_virtual_intf = wal_cfg80211_change_virtual_intf,
    .add_station = wal_cfg80211_add_station,
    .del_station = wal_cfg80211_del_station,
    .change_station = wal_cfg80211_change_station,
    .get_station = wal_cfg80211_get_station,
    .dump_station = wal_cfg80211_dump_station,
    .dump_survey = wal_cfg80211_dump_survey,
    .set_pmksa = wal_cfg80211_set_pmksa,
    .del_pmksa = wal_cfg80211_del_pmksa,
    .flush_pmksa = wal_cfg80211_flush_pmksa,
    .remain_on_channel = wal_cfg80211_remain_on_channel,
    .cancel_remain_on_channel = wal_cfg80211_cancel_remain_on_channel,
    .mgmt_tx = wal_cfg80211_mgmt_tx,
    .mgmt_frame_register = wal_cfg80211_mgmt_frame_register,
    .set_bitrate_mask = wal_cfg80211_set_bitrate_mask,
    .add_virtual_intf = wal_cfg80211_add_virtual_intf,
    .del_virtual_intf = wal_cfg80211_del_virtual_intf,
    .mgmt_tx_cancel_wait = wal_cfg80211_mgmt_tx_cancel_wait,
    .start_p2p_device = wal_cfg80211_start_p2p_device,
    .stop_p2p_device = wal_cfg80211_stop_p2p_device,
    .set_power_mgmt = wal_cfg80211_set_power_mgmt,
#ifdef _PRE_WLAN_FEATURE_11R
    .update_ft_ies = wal_cfg80211_update_ft_ies,
#endif  // _PRE_WLAN_FEATURE_11R
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    .abort_scan = wal_cfg80211_abort_scan,
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)) */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#ifdef _PRE_WLAN_FEATURE_SAE
    .external_auth = wal_cfg80211_external_auth,
#endif /* _PRE_WLAN_FEATURE_SAE */
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0) */
};


void wal_cfg80211_reset_bands(uint8_t uc_dev_id)
{
    int i;

    /*
     * ÿ�θ��¹�����,flags���ᱻ�޸�,���ϴ��޸ĵ�ֵ���ᱻ���,
     * �൱��ÿ���޸ĵĹ����붼����Ч,��˸��¹�����Ҫ���flag��־
     */
    for (i = 0; i < g_st_supported_band_2ghz_info.n_channels; i++) {
        g_st_supported_band_2ghz_info.channels[i].flags = 0;
    }

    if (mac_device_check_5g_enable(uc_dev_id)) {
        for (i = 0; i < g_st_supported_band_5ghz_info.n_channels; i++) {
            g_st_supported_band_5ghz_info.channels[i].flags = 0;
        }
    }
}


void wal_cfg80211_save_bands(uint8_t uc_dev_id)
{
    int i;

    /* ÿ�θ��¹�����,flags���ᱻ�޸�,���ϴ��޸ĵ�ֵ���ᱻ���,
       �൱��ÿ���޸ĵĹ����붼����Ч,��˸��¹�����Ҫ���flag��־��
       ÿ�����ù�����flag �󣬱��浱ǰ���õ�orig_flags��
     */
    for (i = 0; i < g_st_supported_band_2ghz_info.n_channels; i++) {
        g_st_supported_band_2ghz_info.channels[i].orig_flags = g_st_supported_band_2ghz_info.channels[i].flags;
    }

    if (mac_device_check_5g_enable(uc_dev_id)) {
        for (i = 0; i < g_st_supported_band_5ghz_info.n_channels; i++) {
            g_st_supported_band_5ghz_info.channels[i].orig_flags = g_st_supported_band_5ghz_info.channels[i].flags;
        }
    }
}

OAL_STATIC int32_t wal_init_dual_wlan_netdev(mac_device_stru *mac_device, oal_wiphy_stru *wiphy,
    uint8_t dev_id)
{
    const char *netdev_name = "wlan1";
    if (!g_wlan_spec_cfg->feature_dual_wlan_is_supported) {
        oal_io_print("{wal_init_dual_wlan_netdev::dual wlan unsupported, Not create hwlan netdev!}");
        return OAL_SUCC;
    }
    return wal_init_wlan_netdev(wiphy, netdev_name);
}


OAL_STATIC uint32_t wal_cfg80211_init_netdev(mac_device_stru *pst_device, oal_wiphy_stru *pst_wiphy,
    uint8_t uc_dev_id)
{
    int32_t l_return;
    int8_t ac_vap_netdev_name[MAC_NET_DEVICE_NAME_LENGTH] = {0};

    oal_io_print("wal_init_wlan_netdev wlan and p2p[%d].\n", uc_dev_id);

    /* ��·��·��netdev���:��·wlan0 p2p0,��·wlan1 p2p1 */
    snprintf_s(ac_vap_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "wlan%d", uc_dev_id);

    l_return = wal_init_wlan_netdev(pst_wiphy, ac_vap_netdev_name);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_wlan_netdev wlan[%d] failed.l_return:%d\n", uc_dev_id, l_return);
        return (uint32_t)l_return;
    }

    snprintf_s(ac_vap_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "p2p%d", uc_dev_id);

    l_return = wal_init_wlan_netdev(pst_wiphy, ac_vap_netdev_name);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_wlan_netdev p2p[%d] failed.l_return:%d\n", uc_dev_id, l_return);

        /* �ͷ�wlan�����豸��Դ */
        oal_mem_free_m(oal_netdevice_wdev(pst_device->st_p2p_info.pst_primary_net_device), OAL_TRUE);
        oal_net_unregister_netdev(pst_device->st_p2p_info.pst_primary_net_device);
        return (uint32_t)l_return;
    }
    l_return = wal_init_dual_wlan_netdev(pst_device, pst_wiphy, uc_dev_id);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_dual_wlan_netdev wlan1 failed.l_return:%d\n", l_return);
        oal_mem_free_m(oal_netdevice_wdev(pst_device->st_p2p_info.pst_p2p_net_device), OAL_TRUE);
        oal_net_unregister_netdev(pst_device->st_p2p_info.pst_p2p_net_device);
        oal_mem_free_m(oal_netdevice_wdev(pst_device->st_p2p_info.pst_primary_net_device), OAL_TRUE);
        oal_net_unregister_netdev(pst_device->st_p2p_info.pst_primary_net_device);
        return (uint32_t)l_return;
    }

#ifdef _PRE_WLAN_FEATURE_NAN
    /* ע��NAN netdev */
    wal_init_nan_netdev(pst_wiphy);
#endif
    return OAL_SUCC;
}


uint32_t wal_cfg80211_init(void)
{
    uint32_t ul_chip, ul_chip_max_num;
    uint8_t uc_device, uc_dev_id;
    int32_t l_return;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_board_stru *pst_hmac_board = OAL_PTR_NULL;
    oal_wiphy_stru *pst_wiphy = OAL_PTR_NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_board_get_instance(&pst_hmac_board);

    ul_chip_max_num = oal_bus_get_chip_num();

    for (ul_chip = 0; ul_chip < ul_chip_max_num; ul_chip++) {
        for (uc_device = 0; uc_device < pst_hmac_board->ast_chip[ul_chip].uc_device_nums; uc_device++) {
            /* ��ȡdevice_id */
            uc_dev_id = pst_hmac_board->ast_chip[ul_chip].auc_device_id[uc_device];

            pst_device = mac_res_get_dev(uc_dev_id);

            if (oal_unlikely(pst_device == OAL_PTR_NULL)) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::mac_res_get_dev,pst_dev null!}\r\n");
                return OAL_FAIL;
            }

            pst_device->pst_wiphy = oal_wiphy_new(&g_wal_cfg80211_ops, OAL_SIZEOF(mac_wiphy_priv_stru));

            if (pst_device->pst_wiphy == OAL_PTR_NULL) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::oal_wiphy_new failed!}\r\n");
                return OAL_FAIL;
            }

            /* ��ʼ��wiphy �ṹ������ */
            pst_wiphy = pst_device->pst_wiphy;

            pst_wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) | BIT(NL80211_IFTYPE_AP)
                                         | BIT(NL80211_IFTYPE_P2P_CLIENT) | BIT(NL80211_IFTYPE_P2P_GO)
                                         | BIT(NL80211_IFTYPE_P2P_DEVICE);

            pst_wiphy->iface_combinations = sta_p2p_iface_combinations;
            pst_wiphy->n_iface_combinations = oal_array_size(sta_p2p_iface_combinations);
            pst_wiphy->mgmt_stypes = wal_cfg80211_default_mgmt_stypes;
            pst_wiphy->max_remain_on_channel_duration = 5000;
            pst_wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL | WIPHY_FLAG_OFFCHAN_TX;
            pst_wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME; /* ����GO ����λ */

            /* 1102ע��֧��pno����ɨ�����������Ϣ */
            pst_wiphy->max_sched_scan_ssids = MAX_PNO_SSID_COUNT;
            pst_wiphy->max_match_sets = MAX_PNO_SSID_COUNT;
            pst_wiphy->max_sched_scan_ie_len = WAL_MAX_SCAN_IE_LEN;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0))
            pst_wiphy->max_sched_scan_reqs = 1;
#else
            pst_wiphy->flags |= WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
#endif

            pst_wiphy->max_scan_ssids = WLAN_SCAN_REQ_MAX_SSID;

            pst_wiphy->max_scan_ie_len = WAL_MAX_SCAN_IE_LEN;
            pst_wiphy->cipher_suites = g_ast_wlan_supported_cipher_suites;
            pst_wiphy->n_cipher_suites = sizeof(g_ast_wlan_supported_cipher_suites) / sizeof(uint32_t);

            /* ��ʹ�ܽ��� */
            pst_wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;

            /* wifi �����ϱ�֧��FW_ROAM,����ʱ(cfg80211_connect)��ʹ��bssid_hint ���bssid�� */
            pst_wiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM;
#ifdef _PRE_WLAN_FEATURE_DFS_OFFLOAD
            pst_wiphy->flags |= 0x200;
#endif

            /* linux 3.14 �汾�����������������޸� */
            pst_wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;

#ifdef _PRE_WLAN_FEATURE_SAE
            pst_wiphy->features |= NL80211_FEATURE_SAE; /* ����֧��SAE ���� */
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
            pst_wiphy->bands[NL80211_BAND_2GHZ] = &g_st_supported_band_2ghz_info; /* ֧�ֵ�Ƶ����Ϣ 2.4G */
            if (mac_device_check_5g_enable(uc_dev_id)) {
                pst_wiphy->bands[NL80211_BAND_5GHZ] = &g_st_supported_band_5ghz_info; /* ֧�ֵ�Ƶ����Ϣ 5G */
            }
#else
            pst_wiphy->bands[IEEE80211_BAND_2GHZ] = &g_st_supported_band_2ghz_info; /* ֧�ֵ�Ƶ����Ϣ 2.4G */
            if (mac_device_check_5g_enable(uc_dev_id)) {
                pst_wiphy->bands[IEEE80211_BAND_5GHZ] = &g_st_supported_band_5ghz_info; /* ֧�ֵ�Ƶ����Ϣ 5G */
            }
#endif
            pst_wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;

            oal_wiphy_apply_custom_regulatory(pst_wiphy, &g_st_default_regdom);

            wal_cfgvendor_init(pst_wiphy);

            oal_io_print("wiphy_register start.\n");
            l_return = oal_wiphy_register(pst_wiphy);
            if (l_return != 0) {
                oal_wiphy_free(pst_device->pst_wiphy);
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::oal_wiphy_register failed!}\r\n");
                return (uint32_t)l_return;
            }

            /* P2P add_virtual_intf ����wiphy ��������wiphy priv ָ�뱣��wifi ����mac_devie_stru �ṹָ�� */
            pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wiphy));
            pst_wiphy_priv->pst_mac_device = pst_device;

            l_return = wal_cfg80211_init_netdev(pst_device,pst_wiphy,uc_dev_id);
            if (l_return != OAL_SUCC)
                return (uint32_t)l_return;

        }
    }

    return OAL_SUCC;
}


void wal_cfg80211_exit(void)
{
    uint32_t ul_chip;
    uint8_t uc_device;
    uint8_t uc_dev_id;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    uint32_t ul_chip_max_num;
    mac_board_stru *pst_hmac_board = OAL_PTR_NULL;

    hmac_board_get_instance(&pst_hmac_board);

    ul_chip_max_num = oal_bus_get_chip_num(); /* ����ط���ȷ�� */

    for (ul_chip = 0; ul_chip < ul_chip_max_num; ul_chip++) {
        for (uc_device = 0; uc_device < pst_hmac_board->ast_chip[ul_chip].uc_device_nums; uc_device++) {
            /* ��ȡdevice_id */
            uc_dev_id = pst_hmac_board->ast_chip[ul_chip].auc_device_id[uc_device];

            pst_device = mac_res_get_dev(uc_dev_id);

            if (pst_device == OAL_PTR_NULL) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::mac_res_get_dev pst_device is null!}\r\n");
                return;
            }
            wal_cfgvendor_deinit(pst_device->pst_wiphy);

            /* ע��ע�� wiphy device */
            oal_wiphy_unregister(pst_device->pst_wiphy);

            /* ж��wiphy device */
            oal_wiphy_free(pst_device->pst_wiphy);
        }
    }

    return;
}


uint32_t wal_cfg80211_init_evt_handle(frw_event_mem_stru *pst_event_mem)
{
    wal_cfg80211_init();
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_M2S

uint32_t wal_cfg80211_m2s_status_report(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    dmac_m2s_complete_syn_stru *pst_m2s_switch_comp_status;
    wlan_m2s_mgr_vap_stru *pst_m2s_vap_mgr;
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_vap_idx;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_M2S, "{wal_cfg80211_m2s_status_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ��ȡ����vap */
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_M2S,
            "{wal_cfg80211_m2s_status_report::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_m2s_switch_comp_status = (dmac_m2s_complete_syn_stru *)(pst_event->auc_event_data);

    /* ��Ҫ�ϲ��ṩoal�ӿڣ�TBD */
    oam_warning_log4(0, OAM_SF_M2S,
        "{wal_cfg80211_m2s_status_report::vap num[%d]device result[%d]state[%d]m2s_mode[%d].}",
        pst_m2s_switch_comp_status->pri_data.mss_result.uc_vap_num,
        pst_m2s_switch_comp_status->pri_data.mss_result.en_m2s_result,
        pst_m2s_switch_comp_status->uc_m2s_state, pst_m2s_switch_comp_status->pri_data.mss_result.uc_m2s_mode);

    for (uc_vap_idx = 0; uc_vap_idx < pst_m2s_switch_comp_status->pri_data.mss_result.uc_vap_num; uc_vap_idx++) {
        pst_m2s_vap_mgr = &(pst_m2s_switch_comp_status->ast_m2s_comp_vap[uc_vap_idx]);

        oam_warning_log3(0, OAM_SF_M2S,
                         "{wal_cfg80211_m2s_status_report::Notify to host, ap index[%d]action type[%d]state[%d].}",
                         uc_vap_idx, pst_m2s_vap_mgr->en_action_type, pst_m2s_vap_mgr->en_m2s_result);

        oam_warning_log4(0, OAM_SF_M2S,
                         "{wal_cfg80211_m2s_status_report::Notify to host, ap addr->%02x:XX:XX:%02x:%02x:%02x.}",
                         pst_m2s_vap_mgr->auc_user_mac_addr[0], pst_m2s_vap_mgr->auc_user_mac_addr[3],
                         pst_m2s_vap_mgr->auc_user_mac_addr[4], pst_m2s_vap_mgr->auc_user_mac_addr[5]);
    }

    if (!pst_hmac_vap->pst_net_device) {
        oam_warning_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_M2S,
            "{wal_cfg80211_m2s_status_report::net dev is null, vap maybe deleted!}");
        return OAL_SUCC;
    }

    /* �ϱ��ں� */
    oal_cfg80211_m2s_status_report(pst_hmac_vap->pst_net_device, GFP_KERNEL,
                                   (uint8_t *)pst_m2s_switch_comp_status, OAL_SIZEOF(dmac_m2s_complete_syn_stru));

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH

uint32_t wal_cfg80211_tas_rssi_access_report(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_vap_idx;
    dmac_tas_rssi_notify_stru *pst_tas_rssi_comp_status;
    int32_t l_tas_state;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ��ȡ����vap */
    uc_vap_idx = pst_event->st_event_hdr.uc_vap_id;
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_idx);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_tas_rssi_comp_status = (dmac_tas_rssi_notify_stru *)(pst_event->auc_event_data);

    l_tas_state = board_get_wifi_tas_gpio_state();
    oam_warning_log3(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::core[%d] c_ant%d_rssi[%d].}",
                     pst_tas_rssi_comp_status->l_core_idx, l_tas_state, pst_tas_rssi_comp_status->l_rssi);

    /* �ϱ��ں� */
    oal_cfg80211_tas_rssi_access_report(pst_hmac_vap->pst_net_device, GFP_KERNEL, (uint8_t *)pst_tas_rssi_comp_status,
                                        OAL_SIZEOF(dmac_tas_rssi_notify_stru));

    return OAL_SUCC;
}
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP

uint32_t wal_cfg80211_hid2d_seqnum_report(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_vap_idx;
    uint16_t *pus_seq_num;
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_hid2d_seqnum_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ��ȡ����vap */
    uc_vap_idx = pst_event->st_event_hdr.uc_vap_id;
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_idx);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_hid2d_seqnum_report::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pus_seq_num = (uint16_t *)(pst_event->auc_event_data);

    oam_warning_log1(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_hid2d_seqnum_report::seq_num[%d].}", *pus_seq_num);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    /* �ϱ��ں� */
    oal_cfg80211_hid2d_seqnum_report(pst_hmac_vap->pst_net_device, GFP_KERNEL, (uint8_t *)pus_seq_num,
                                        OAL_SIZEOF(uint16_t));
#endif
    return OAL_SUCC;
}
#endif
