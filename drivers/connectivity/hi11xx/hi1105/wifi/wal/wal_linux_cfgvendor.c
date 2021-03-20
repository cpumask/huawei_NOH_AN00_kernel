

#include "oal_net.h"
#include "oal_cfg80211.h"
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "wal_linux_cfgvendor.h"
#include "wal_linux_ioctl.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#include "wal_linux_cfg80211.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_CFGVENDOR_C

#define OUI_VENDOR 0x001A11
#define OUI_HISI   0x001018

wal_cfgvendor_radio_stat_stru g_st_wifi_radio_stat;


OAL_STATIC uint32_t wal_cfgvendor_copy_channel_list(mac_vendor_cmd_channel_list_stru *pst_channel_list,
                                                      uint32_t band,
                                                      int32_t *pl_channel_list,
                                                      uint32_t *pul_num_channels)
{
    uint8_t *puc_chanel_list = OAL_PTR_NULL;
    uint32_t ul_loop;
    uint32_t ul_channel_num = 0;

    *pul_num_channels = 0;

    if ((pst_channel_list->uc_channel_num_5g == 0) && (pst_channel_list->uc_channel_num_2g == 0)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_copy_channel_list::get 5G or 2G channel list fail,band = %d", band);
        return OAL_FAIL;
    }

    /* ��ȡ2G,�ŵ��б� */
    if ((band & WIFI_BAND_24_GHZ) && (pst_channel_list->uc_channel_num_2g != 0)) {
        puc_chanel_list = pst_channel_list->auc_channel_list_2g;
        for (ul_loop = 0; ul_loop < pst_channel_list->uc_channel_num_2g; ul_loop++) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
            pl_channel_list[ul_channel_num++] =
                oal_ieee80211_channel_to_frequency(puc_chanel_list[ul_loop], NL80211_BAND_2GHZ);
#else
            pl_channel_list[ul_channel_num++] =
                oal_ieee80211_channel_to_frequency(puc_chanel_list[ul_loop], IEEE80211_BAND_2GHZ);
#endif
        }
    }

    /* ��ȡ5G Band�ŵ��б�������DFS Channels��DFS Channels */
    if ((band & (WIFI_BAND_5_GHZ | WIFI_BAND_5_GHZ_DFS_ONLY)) && (pst_channel_list->uc_channel_num_5g != 0)) {
        oal_bool_enum_uint8 en_dfs = OAL_FALSE;
        puc_chanel_list = pst_channel_list->auc_channel_list_5g;

        for (ul_loop = 0; ul_loop < pst_channel_list->uc_channel_num_5g; ul_loop++) {
            en_dfs = mac_is_dfs_channel(MAC_RC_START_FREQ_5, puc_chanel_list[ul_loop]);

            if (((band & WIFI_BAND_5_GHZ_DFS_ONLY) && (en_dfs == OAL_TRUE)) ||
                ((band & WIFI_BAND_5_GHZ) && (en_dfs == OAL_FALSE))) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
                pl_channel_list[ul_channel_num++] =
                    oal_ieee80211_channel_to_frequency(puc_chanel_list[ul_loop], NL80211_BAND_5GHZ);
#else
                pl_channel_list[ul_channel_num++] =
                    oal_ieee80211_channel_to_frequency(puc_chanel_list[ul_loop], IEEE80211_BAND_5GHZ);
#endif
            }
        }
    }

    if (ul_channel_num == 0) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_copy_channel_list::get 5G or 2G channel list fail,ul_channel_num = %d", ul_channel_num);
        return OAL_FAIL;
    }

    *pul_num_channels = ul_channel_num;

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_cfgvendor_get_current_channel_list(oal_net_device_stru *pst_netdev,
                                                             uint32_t band,
                                                             int32_t *pl_channel_list,
                                                             uint32_t *pul_num_channels)
{
    int32_t l_ret;
    wal_msg_query_stru st_query_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    mac_vendor_cmd_channel_list_stru *pst_channel_list = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_netdev, pl_channel_list, pul_num_channels)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_cfgvendor_get_current_channel_list::channel_list or num_channel is NULL!"
                       "netdev %p, channel_list %p, num_channels %p.}",
                       (uintptr_t)pst_netdev, (uintptr_t)pl_channel_list, (uintptr_t)pul_num_channels);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �ϲ����κ�ʱ�򶼿����·��������Ҫ���жϵ�ǰnetdev��״̬����ʱ���� */
    if (oal_unlikely(OAL_PTR_NULL == oal_net_dev_priv(pst_netdev))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_get_current_channel_list::NET_DEV_PRIV is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��Ϣ����׼�� */
    st_query_msg.en_wid = WLAN_CFGID_VENDOR_CMD_GET_CHANNEL_LIST;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_QUERY,
                                   WAL_MSG_WID_LENGTH,
                                   (uint8_t *)&st_query_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_get_current_channel_list:: wal_send_cfg_event return err code %d!}\r\n", l_ret);
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        return (uint32_t)l_ret;
    }

    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);
    pst_channel_list = (mac_vendor_cmd_channel_list_stru *)(pst_query_rsp_msg->auc_value);

    if (wal_cfgvendor_copy_channel_list(pst_channel_list, band, pl_channel_list, pul_num_channels) != OAL_SUCC) {
        oal_free(pst_rsp_msg);
        return OAL_FAIL;
    }

    oal_free(pst_rsp_msg);
    return OAL_SUCC;
}

OAL_STATIC int32_t wal_cfgvendor_get_channel_list(oal_wiphy_stru *wiphy,
                                                    oal_wireless_dev_stru *wdev,
                                                    OAL_CONST void *data,
                                                    int32_t len)
{
    int32_t err = 0;
    int32_t type;
    int32_t al_channel_list[VENDOR_CHANNEL_LIST_ALL + 1] = { 0 };
    uint32_t band = 0; /* �ϲ��·���bandֵ */
    uint32_t mem_needed;
    uint32_t num_channels = 0;
    uint32_t reply_len = 0;
    uint32_t ul_ret;
    oal_netbuf_stru *skb = OAL_PTR_NULL;

    if (oal_any_null_ptr2(wdev, wdev->netdev)) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_get_channel_list::wdev or netdev is NULL! wdev %p.}", (uintptr_t)wdev);
        return -OAL_EFAIL;
    }

    type = oal_nla_type(data);
    if (type == GSCAN_ATTRIBUTE_BAND) {
        /* ��ȡbandֵ bit0:2G�ŵ��б� bit1:5G�ŵ��б� bit2:DFS channel */
        band = oal_nla_get_u32(data);
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_get_channel_list::invalid nla type! type %d.}", type);
        return -OAL_EFAIL;
    }

    ul_ret = wal_cfgvendor_get_current_channel_list(wdev->netdev, band, al_channel_list, &num_channels);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log3(0, OAM_SF_ANY,
            "{wal_cfgvendor_get_channel_list::fail. ret_value %d, band %d, num_channels %d.}",
                         ul_ret, band, num_channels);
        return -OAL_EFAIL;
    }

    reply_len = OAL_SIZEOF(al_channel_list[0]) * (num_channels);
    mem_needed = reply_len + VENDOR_REPLY_OVERHEAD + (ATTRIBUTE_U32_LEN * 2);

    /* ����SKB �ϱ��ŵ��б� */
    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_get_channel_list::skb alloc failed.len %d\r\n", mem_needed);
        return -OAL_ENOMEM;
    }

    oal_nla_put_u32(skb, GSCAN_ATTRIBUTE_NUM_CHANNELS, num_channels);
    oal_nla_put(skb, GSCAN_ATTRIBUTE_CHANNEL_LIST, reply_len, al_channel_list);

    err = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(err)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "wal_cfgvendor_get_channel_list::oal_cfg80211_vendor_cmd_reply failed ret:%d.\r\n", err);
        return err;
    }

    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfgvendor_get_channel_list::ret_value %d, band %d, num_channels %d.}",
                     err, band, num_channels);

    return err;
}

OAL_STATIC int32_t wal_cfgvendor_set_country(oal_wiphy_stru *wiphy,
                                               oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    int32_t rem;
    int32_t type;
    int32_t l_ret = OAL_SUCC;
    int8_t auc_country_code[WLAN_COUNTRY_STR_LEN] = { 0 };
    OAL_CONST oal_nlattr_stru *iter;

    /* ���Խ׶οɲ��ö��ƻ�99���� */
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (g_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag == OAL_TRUE) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_country::set_country ignored, flag ini[%d]",
                         g_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag);
        return l_ret;
    }
#endif

    oal_nla_for_each_attr(iter, data, len, rem)
    {
        memset_s(auc_country_code, WLAN_COUNTRY_STR_LEN, 0, WLAN_COUNTRY_STR_LEN);
        type = oal_nla_type(iter);
        switch (type) {
            case ANDR_WIFI_ATTRIBUTE_COUNTRY:
#ifdef _PRE_WLAN_FEATURE_11D
                if (EOK != memcpy_s(auc_country_code, OAL_SIZEOF(auc_country_code), oal_nla_data(iter),
                                    oal_min(oal_nla_len(iter), OAL_SIZEOF(auc_country_code)))) {
                    oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_set_country::memcpy fail!");
                    return -OAL_EFAIL;
                }
                oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_country:: len = %d!}", oal_nla_len(iter));
                /* ���ù����뵽wifi ���� */
                l_ret = wal_regdomain_update_country_code(wdev->netdev, auc_country_code);
#else
                oam_warning_log0(0, OAM_SF_ANY,
                    "{wal_cfgvendor_set_country::_PRE_WLAN_FEATURE_11D is not define!}\r\n");
#endif
                break;
            default:
                oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_country::unknow type:%d!}\r\n", type);
                break;
        }
    }

    return l_ret;
}

OAL_STATIC uint32_t wal_cfgvendor_do_get_feature_set(mac_wiphy_priv_stru *pst_wiphy_priv)
{
    uint32_t ul_feature_set = 0;
    mac_device_stru *pst_mac_device = NULL;

    if (pst_wiphy_priv == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_do_get_feature_set::pst_mac_device is null!}");
        return ul_feature_set;
    }

    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device != NULL) {
        ul_feature_set |=
            ((OAL_TRUE == mac_device_check_5g_enable(pst_mac_device->uc_device_id)) ? WIFI_FEATURE_INFRA_5G : 0);
    }
#ifdef _PRE_WLAN_FEATURE_HS20
    ul_feature_set |= WIFI_FEATURE_HOTSPOT;
#endif
    ul_feature_set |= WIFI_FEATURE_LINK_LAYER_STATS; /** 0x10000 Link layer stats collection */
    
    return ul_feature_set;
}

OAL_STATIC int32_t wal_cfgvendor_get_feature_set(oal_wiphy_stru *wiphy,
                                                   oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    int32_t l_err = 0;
    uint32_t ul_reply;
    int32_t l_reply_len = OAL_SIZEOF(int32_t);
    oal_netbuf_stru *skb = NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;

    if (wdev == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_get_feature_set::pst_wdev is null!!}\r\n");
        return -OAL_EFAUL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(oal_wireless_dev_wiphy(wdev));

    ul_reply = wal_cfgvendor_do_get_feature_set(pst_wiphy_priv);

    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, l_reply_len);
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_get_feature_set::skb alloc failed.len %d\r\n", l_reply_len);
        return -OAL_ENOMEM;
    }

    oal_nla_put_nohdr(skb, l_reply_len, &ul_reply);

    l_err = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(l_err)) {
        oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_get_feature_set::Vendor Command reply failed ret:%d.\r\n", l_err);
    }

    oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_get_feature_set::set flag:0x%x.\r\n}", ul_reply);

    return l_err;
}


OAL_STATIC int32_t wal_send_random_mac_oui(oal_net_device_stru *pst_net_dev,
    uint8_t *auc_random_mac_oui, uint8_t auc_random_mac_oui_len)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    if (oal_any_null_ptr2(pst_net_dev, auc_random_mac_oui)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_send_random_mac_oui:: null point argument,pst_net_dev:%p, auc_random_mac_oui:%p.}",
            (uintptr_t)pst_net_dev, (uintptr_t)auc_random_mac_oui);
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RANDOM_MAC_OUI, auc_random_mac_oui_len);
    if (EOK != memcpy_s(st_write_msg.auc_value, auc_random_mac_oui_len, auc_random_mac_oui, auc_random_mac_oui_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_send_random_mac_oui::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + auc_random_mac_oui_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_send_random_mac_oui::send rand_mac_oui failed, err code %d!}\r\n", l_ret);
        return l_ret;
    }

    return l_ret;
}


OAL_STATIC int32_t wal_cfgvendor_set_random_mac_oui(oal_wiphy_stru *pst_wiphy,
                                                      oal_wireless_dev_stru *pst_wdev,
                                                      OAL_CONST void *p_data,
                                                      int32_t l_len)
{
    int32_t l_type;
    int32_t l_ret;
    uint8_t auc_random_mac_oui[WLAN_RANDOM_MAC_OUI_LEN] = { 0 };

    l_type = oal_nla_type(p_data);

    if (l_type == ANDR_WIFI_ATTRIBUTE_RANDOM_MAC_OUI) {
        /* ���mac��ַǰ3�ֽ�(mac oui)��ϵͳ�·�,wps pbc������hilink���������Ὣ��3�ֽ���0 */
        if (EOK != memcpy_s(auc_random_mac_oui, WLAN_RANDOM_MAC_OUI_LEN,
            oal_nla_data(p_data), WLAN_RANDOM_MAC_OUI_LEN)) {
            oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_set_random_mac_oui::memcpy fail!");
            return -OAL_EFAIL;
        }
        oam_warning_log3(0, OAM_SF_ANY, "{wal_cfgvendor_set_random_mac_oui::mac_ou:0x%.2x:%.2x:%.2x}\r\n",
                         auc_random_mac_oui[0], auc_random_mac_oui[1], auc_random_mac_oui[2]);
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_random_mac_oui:: unknow type:%x}\r\n", l_type);
        return -OAL_EFAIL;
    }

    l_ret = wal_send_random_mac_oui(pst_wdev->netdev, auc_random_mac_oui,  WLAN_RANDOM_MAC_OUI_LEN);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_cfgvendor_blacklist_mode(oal_net_device_stru *pst_net_dev, uint8_t uc_mode)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));

    *(uint8_t *)(st_write_msg.auc_value) = uc_mode;
    us_len = OAL_SIZEOF(uc_mode);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BLACKLIST_MODE, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_cfgvendor_blacklist_mode:wal_send_cfg_event return[%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_cfgvendor_blacklist_add(oal_net_device_stru *pst_net_dev, uint8_t *puc_mac_addr)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;
    mac_blacklist_stru *pst_blklst = OAL_PTR_NULL;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    pst_blklst = (mac_blacklist_stru *)(st_write_msg.auc_value);

    if (EOK != memcpy_s(pst_blklst->auc_mac_addr, OAL_MAC_ADDR_LEN, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_CFG, "wal_cfgvendor_blacklist_add::memcpy fail!");
        return -OAL_EINVAL;
    }

    us_len = OAL_SIZEOF(mac_blacklist_stru);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_cfgvendor_blacklist_add:wal_send_cfg_event return[%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_cfgvendor_blacklist_del(oal_net_device_stru *pst_net_dev,
    uint8_t *puc_mac_addr, uint8_t puc_mac_addr_len)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));

    if (EOK != memcpy_s(st_write_msg.auc_value, puc_mac_addr_len, puc_mac_addr, puc_mac_addr_len)) {
        oam_error_log0(0, OAM_SF_CFG, "wal_cfgvendor_blacklist_del::memcpy fail!");
        return -OAL_EINVAL;
    }

    us_len = puc_mac_addr_len; /* OAL_SIZEOF(uint8_t); */

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_BLACK_LIST, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_cfgvendor_blacklist_del:wal_send_cfg_event return[%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC void wal_cfgvendor_set_bssid_blacklist_info(wal_wifi_bssid_params *pst_bssid_info)
{
    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfgvendor_set_bssid_blacklist::bssid[0]=xx:xx:xx:%02x:%02x:%02x}\r\n",
                     pst_bssid_info->bssids[0][3], pst_bssid_info->bssids[0][4], pst_bssid_info->bssids[0][5]);
    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfgvendor_set_bssid_blacklist::bssid[1]=xx:xx:xx:%02x:%02x:%02x}\r\n",
                     pst_bssid_info->bssids[1][3], pst_bssid_info->bssids[1][4], pst_bssid_info->bssids[1][5]);
    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfgvendor_set_bssid_blacklist::bssid[2]=xx:xx:xx:%02x:%02x:%02x}\r\n",
                     pst_bssid_info->bssids[2][3], pst_bssid_info->bssids[2][4], pst_bssid_info->bssids[2][5]);
}


OAL_STATIC int32_t wal_cfgvendor_set_bssid_blacklist(oal_wiphy_stru *pst_wiphy,
                                                       oal_wireless_dev_stru *pst_wdev,
                                                       OAL_CONST void *p_data,
                                                       int32_t l_len)
{
    int32_t l_type = 0;
    oal_nlattr_stru *pst_nla = OAL_PTR_NULL;
    wal_wifi_bssid_params st_bssid_info;
    uint8_t i = 0;

    if (oal_any_null_ptr2(pst_wdev, pst_wdev->netdev)) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_bssid_blacklist::wdev or netdev is NULL! wdev=%p.}", (uintptr_t)pst_wdev);
        return -OAL_EFAIL;
    }

    if ((p_data == OAL_PTR_NULL) || (l_len <= NLA_HDRLEN)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_bssid_blacklist::invalid para for p_data %p, l_len=%d.}", (uintptr_t)p_data, l_len);
        return -OAL_EINVAL;
    }

    memset_s(&st_bssid_info, sizeof(st_bssid_info), 0, sizeof(st_bssid_info));
    pst_nla = (oal_nlattr_stru *)p_data;
    l_type = oal_nla_type(pst_nla);
    if (l_type == GSCAN_ATTRIBUTE_NUM_BSSID) {
        st_bssid_info.num_bssid = oal_nla_get_u32(pst_nla);
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_bssid_blacklist::num_bssid=%d}\r\n", st_bssid_info.num_bssid);
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_bssid_blacklist:: unknown bssid type:%d}\r\n", l_type);
        return -OAL_EFAIL;
    }

    pst_nla = (oal_nlattr_stru *)((uint8_t *)pst_nla + oal_nla_total_size(pst_nla));
    if (!st_bssid_info.num_bssid) /* Flush BSSID Blacklist */
    {
        oal_bool_enum_uint8 en_flush = OAL_FALSE;

        l_type = oal_nla_type(pst_nla);
        if (l_type == GSCAN_ATTRIBUTE_BSSID_BLACKLIST_FLUSH) {
            en_flush = oal_nla_get_u32(pst_nla);
        }
        oam_warning_log2(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_bssid_blacklist::l_type=%d en_flush=%d}\r\n", l_type, en_flush);

        if (en_flush == OAL_TRUE) {
            uint8_t mac_addr[OAL_MAC_ADDR_LEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            /* broadcast address as delete all bssids */
            wal_cfgvendor_blacklist_del(pst_wdev->netdev, mac_addr, OAL_MAC_ADDR_LEN);
        }
    } else { /* set BSSID Blacklist */
        if (st_bssid_info.num_bssid > MAX_BLACKLIST_BSSID) { // ��ֹ������Ϊѭ���߽翽������ջ����Լ�дԽ��
            oam_error_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_bssid_blacklist::num_bssid[%d] overrun!}",
                st_bssid_info.num_bssid);
            return -OAL_EFAIL;
        }
        for (i = 0; i < st_bssid_info.num_bssid; i++) {
            l_type = oal_nla_type(pst_nla);
            if (l_type == GSCAN_ATTRIBUTE_BLACKLIST_BSSID) {
                if (EOK != memcpy_s(st_bssid_info.bssids[i], OAL_MAC_ADDR_LEN,
                                    oal_nla_data(pst_nla), OAL_MAC_ADDR_LEN)) {
                    oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_set_bssid_blacklist::memcpy fail!");
                    return -OAL_EFAIL;
                }
            }
            pst_nla = (oal_nlattr_stru *)((uint8_t *)pst_nla + oal_nla_total_size(pst_nla));
        }

        wal_cfgvendor_blacklist_mode(pst_wdev->netdev, CS_BLACKLIST_MODE_BLACK);

        for (i = 0; i < st_bssid_info.num_bssid; i++) {
            wal_cfgvendor_blacklist_add(pst_wdev->netdev, st_bssid_info.bssids[i]);
        }

        wal_cfgvendor_set_bssid_blacklist_info(&st_bssid_info);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_cfgvendor_roam_enable(oal_net_device_stru *pst_net_dev, wal_wifi_roaming_state roam_state)
{
    int32_t l_ret;
    wal_msg_write_stru st_write_msg;

    if (pst_net_dev == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_roam_enable:: netdev is NULL! netdev=%p.}", (uintptr_t)pst_net_dev);
        return -OAL_EFAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ROAM_ENABLE, OAL_SIZEOF(uint32_t));
    *((oal_bool_enum_uint8 *)(st_write_msg.auc_value)) = roam_state;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_roam_enable::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfgvendor_set_roam_policy(oal_wiphy_stru *pst_wiphy,
                                                   oal_wireless_dev_stru *pst_wdev,
                                                   OAL_CONST void *p_data,
                                                   int32_t l_len)
{
    int32_t l_type;
    wal_wifi_roaming_state roam_state = 0;

    if (oal_any_null_ptr2(pst_wdev, pst_wdev->netdev)) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_roam_policy::wdev or netdev is NULL! wdev=%p.}", (uintptr_t)pst_wdev);
        return -OAL_EFAIL;
    }

    if ((p_data == OAL_PTR_NULL) || (l_len <= NLA_HDRLEN)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_cfgvendor_set_roam_policy::invalid para for p_data %p, l_len=%d.}",
            (uintptr_t)p_data, l_len);
        return -OAL_EINVAL;
    }

    l_type = oal_nla_type(p_data);
    if (l_type == GSCAN_ATTRIBUTE_ROAM_STATE_SET) {
        roam_state = oal_nla_get_u32(p_data);
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_roam_policy::roam_state=%d}\r\n", roam_state);
        /* enable or disable roaming */
        if ((roam_state == VENDOR_WIFI_ROAMING_ENABLE) || (VENDOR_WIFI_ROAMING_DISABLE == roam_state)) {
            wal_cfgvendor_roam_enable(pst_wdev->netdev, roam_state);
        } else {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_roam_policy::invalid roam_state=%d}\r\n", roam_state);
        }
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_set_roam_policy:: unknown roaming type:%d}\r\n", l_type);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfgvendor_dbg_get_feature(oal_wiphy_stru *wiphy,
                                                   oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    int32_t l_ret;
    uint32_t supported_features = VENDOR_DBG_MEMORY_DUMP_SUPPORTED;
    oal_netbuf_stru *skb;

    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, OAL_SIZEOF(uint32_t));
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "wal_cfgvendor_dbg_get_feature::skb alloc failed. len %d\r\n", OAL_SIZEOF(uint32_t));
        return -OAL_ENOMEM;
    }

    oal_nla_put_nohdr(skb, OAL_SIZEOF(uint32_t), &supported_features);

    l_ret = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(l_ret)) {
        oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_dbg_get_feature::Vendor Command reply failed. ret:%d.\r\n", l_ret);
    }
    oam_warning_log1(0, OAM_SF_ANY,
        "wal_cfgvendor_dbg_get_feature::SUCCESS. supported feature:0x%X.\r\n", supported_features);

    return l_ret;
}


OAL_STATIC int32_t wal_cfgvendor_dbg_get_version(oal_wiphy_stru *wiphy,
                                                   oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    int8_t auc_driver_version[] = "Hisi Host Driver, version Hi1102 V100";
    int32_t l_ret;
    int32_t l_buf_len = OAL_SIZEOF(auc_driver_version);
    oal_netbuf_stru *skb;

    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, l_buf_len);
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_dbg_get_version::skb alloc failed.len %d\r\n", l_buf_len);
        return -OAL_ENOMEM;
    }

    oal_nla_put_nohdr(skb, l_buf_len, auc_driver_version);

    l_ret = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(l_ret)) {
        oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_dbg_get_version::Vendor Command reply failed. ret:%d.\r\n", l_ret);
    }

    oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_dbg_get_version::SUCCESS. driver version len %d\r\n", l_buf_len);
    return l_ret;
}


int32_t wal_cfgvendor_dbg_get_ring_status(oal_wiphy_stru *wiphy,
                                                oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    int32_t l_ret, l_buf_len;
    debug_ring_status_st debug_ring_status = { { VENDOR_FW_EVENT_RING_NAME }, 0, FW_EVENT_RING_ID,
        VENDOR_FW_EVENT_RING_SIZE, 0, 0, 0, 0
    };
    oal_netbuf_stru *skb;
    l_buf_len = OAL_SIZEOF(debug_ring_status_st);

    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, l_buf_len + 100);
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "wal_cfgvendor_dbg_get_ring_status::skb alloc failed.len %d\r\n",
            l_buf_len + 100);
        return -OAL_ENOMEM;
    }

    oal_nla_put_u32(skb, DEBUG_ATTRIBUTE_RING_NUM, 1);
    oal_nla_put(skb, DEBUG_ATTRIBUTE_RING_STATUS, l_buf_len, &debug_ring_status);

    l_ret = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(l_ret)) {
        oam_error_log1(0, OAM_SF_ANY,
            "wal_cfgvendor_dbg_get_ring_status::Vendor Command reply failed. ret:%d.\r\n", l_ret);
    }

    oam_warning_log1(0, OAM_SF_ANY,
        "wal_cfgvendor_dbg_get_ring_status::SUCCESS. driver version len %d\r\n", l_buf_len);
    return l_ret;
}


int32_t wal_cfgvendor_dbg_trigger_mem_dump(oal_wiphy_stru *wiphy,
                                                 oal_wireless_dev_stru *wdev, OAL_CONST void *data, int32_t len)
{
    oam_warning_log0(0, OAM_SF_ANY, "wal_cfgvendor_dbg_trigger_mem_dump::ENTER");
    return 0;
}


OAL_STATIC int32_t wal_cfgvendor_dbg_start_logging(oal_wiphy_stru *wiphy,
                                                     oal_wireless_dev_stru *wdev,
                                                     OAL_CONST void *data,
                                                     int32_t len)
{
    oam_warning_log0(0, OAM_SF_ANY, "wal_cfgvendor_dbg_start_logging::ENTER");
    return 0;
}


OAL_STATIC int32_t wal_cfgvendor_dbg_get_ring_data(oal_wiphy_stru *wiphy,
                                                     oal_wireless_dev_stru *wdev,
                                                     OAL_CONST void *data,
                                                     int32_t len)
{
    oam_warning_log0(0, OAM_SF_ANY, "wal_cfgvendor_dbg_get_ring_data::ENTER");
    return 0;
}


OAL_STATIC int32_t wal_send_vowifi_nat_keep_alive_params(oal_net_device_stru *pst_net_dev,
                                                           uint8_t *pc_keep_alive_info, uint8_t uc_msg_len)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    if (oal_any_null_ptr2(pst_net_dev, pc_keep_alive_info)) {
        oam_error_log2(0, OAM_SF_ANY,
            "{wal_send_vowifi_nat_keep_alive_params:: null point argument,pst_net_dev:%p, auc_random_mac_oui:%p.}",
            (uintptr_t)pst_net_dev, (uintptr_t)pc_keep_alive_info);
        return -OAL_EFAIL;
    }

    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /***************************************************************************
        ���¼���wal�㴦��  WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE, uc_msg_len);
    if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), pc_keep_alive_info, uc_msg_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_send_vowifi_nat_keep_alive_params::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + uc_msg_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_send_vowifi_nat_keep_alive_params::send WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE failed, err code %d!}\r\n",
            l_ret);
        return l_ret;
    }

    return l_ret;
}


OAL_STATIC int32_t wl_cfgvendor_start_vowifi_nat_keep_alive(oal_wiphy_stru *pst_wiphy,
                                                              oal_wireless_dev_stru *pst_wdev,
                                                              OAL_CONST void *p_data,
                                                              int32_t l_len)
{
    int32_t l_type;
    int32_t l_rem;
    uint32_t ul_period_msec = 0;
    uint8_t uc_keep_alive_id = 0;
    uint8_t uc_ip_pkt_len = 0;
    oal_bool_enum_uint8 en_find_keepid_flag = OAL_FALSE;
    uint8_t uc_msg_len;
    uint8_t auc_src_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_dst_mac[WLAN_MAC_ADDR_LEN];
    uint8_t *puc_ip_pkt = OAL_PTR_NULL;
    OAL_CONST oal_nlattr_stru *pst_iter;
    mac_vowifi_nat_keep_alive_start_info_stru *pst_start_info = NULL;
    int32_t l_ret = EOK;

    oal_nla_for_each_attr(pst_iter, p_data, l_len, l_rem)
    {
        l_type = oal_nla_type(pst_iter);
        switch (l_type) {
            case MKEEP_ALIVE_ATTRIBUTE_ID:
                uc_keep_alive_id = oal_nla_get_u8(pst_iter);
                en_find_keepid_flag = OAL_TRUE;
                break;
            case MKEEP_ALIVE_ATTRIBUTE_IP_PKT_LEN:
                uc_ip_pkt_len = oal_nla_get_u16(pst_iter);
                if (uc_ip_pkt_len > MKEEP_ALIVE_IP_PKT_MAXLEN || uc_ip_pkt_len == 0) {
                    WAL_CHECK_FREE(puc_ip_pkt);

                    oam_warning_log1(0, OAM_SF_ANY,
                        "{wl_cfgvendor_start_vowifi_nat_keep_alive::uc_ip_pkt_len=[%d]is invalid.}", uc_ip_pkt_len);
                    return OAL_EFAIL;
                }
                break;
            case MKEEP_ALIVE_ATTRIBUTE_IP_PKT:
                WAL_CHECK_FREE(puc_ip_pkt);
                if (uc_ip_pkt_len > 0) {
                    puc_ip_pkt = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, uc_ip_pkt_len, OAL_TRUE);
                    if (puc_ip_pkt == OAL_PTR_NULL) {
                        oam_error_log1(0, OAM_SF_ANY,
                            "{wl_cfgvendor_start_vowifi_nat_keep_alive::alloc %d fail.}", uc_ip_pkt_len);
                        return OAL_EFAIL;
                    }
                    l_ret += memcpy_s(puc_ip_pkt, uc_ip_pkt_len, oal_nla_data(pst_iter), uc_ip_pkt_len);
                }
                break;
            case MKEEP_ALIVE_ATTRIBUTE_SRC_MAC_ADDR:
                l_ret += memcpy_s(auc_src_mac, WLAN_MAC_ADDR_LEN, oal_nla_data(pst_iter), WLAN_MAC_ADDR_LEN);
                break;
            case MKEEP_ALIVE_ATTRIBUTE_DST_MAC_ADDR:
                l_ret += memcpy_s(auc_dst_mac, WLAN_MAC_ADDR_LEN, oal_nla_data(pst_iter), WLAN_MAC_ADDR_LEN);
                break;
            case MKEEP_ALIVE_ATTRIBUTE_PERIOD_MSEC:
                ul_period_msec = oal_nla_get_u32(pst_iter);
                break;
            default:
                WAL_CHECK_FREE(puc_ip_pkt);
                return OAL_EFAIL;
        }
    }

    if (ul_period_msec == 0 || en_find_keepid_flag == OAL_FALSE) {
        WAL_CHECK_FREE(puc_ip_pkt);
        return OAL_FAIL;
    }

    if (uc_ip_pkt_len > 0 && puc_ip_pkt != OAL_PTR_NULL) {
        uc_msg_len = uc_ip_pkt_len + OAL_SIZEOF(mac_vowifi_nat_keep_alive_start_info_stru);
        pst_start_info =
            (mac_vowifi_nat_keep_alive_start_info_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, uc_msg_len, OAL_TRUE);
        if (pst_start_info == OAL_PTR_NULL) {
            WAL_CHECK_FREE(puc_ip_pkt);
            return OAL_EFAIL;
        }

        pst_start_info->st_basic_info.uc_keep_alive_id = uc_keep_alive_id;
        pst_start_info->st_basic_info.en_type = VOWIFI_MKEEP_ALIVE_TYPE_START;
        pst_start_info->us_ip_pkt_len = uc_ip_pkt_len;
        pst_start_info->ul_period_msec = ul_period_msec;

        l_ret += memcpy_s(pst_start_info->auc_ip_pkt_data, uc_ip_pkt_len, puc_ip_pkt, uc_ip_pkt_len);
        l_ret += memcpy_s(pst_start_info->auc_src_mac, WLAN_MAC_ADDR_LEN, auc_src_mac, WLAN_MAC_ADDR_LEN);
        l_ret += memcpy_s(pst_start_info->auc_dst_mac, WLAN_MAC_ADDR_LEN, auc_dst_mac, WLAN_MAC_ADDR_LEN);

        oam_warning_log4(0, OAM_SF_ANY,
                         "{wl_cfgvendor_start_vowifi_nat_keep_alive::en_type=[%d],id=[%d],ip_len=[%d],period=[%d].}",
                         pst_start_info->st_basic_info.en_type, pst_start_info->st_basic_info.uc_keep_alive_id,
                         pst_start_info->us_ip_pkt_len, pst_start_info->ul_period_msec);
        wal_send_vowifi_nat_keep_alive_params(pst_wdev->netdev, (uint8_t *)pst_start_info, uc_msg_len);
        oal_mem_free_m(pst_start_info, OAL_TRUE);
    }

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wl_cfgvendor_start_vowifi_nat_keep_alive::memcpy fail!");
        WAL_CHECK_FREE(puc_ip_pkt);
        return OAL_EFAIL;
    }

    WAL_CHECK_FREE(puc_ip_pkt);

    return OAL_SUCC;
}


OAL_STATIC int32_t wl_cfgvendor_stop_vowifi_nat_keep_alive(oal_wiphy_stru *pst_wiphy,
                                                             oal_wireless_dev_stru *pst_wdev,
                                                             OAL_CONST void *p_data,
                                                             int32_t l_len)
{
    int32_t l_type;
    int32_t l_ret;
    int32_t l_rem;
    oal_bool_enum_uint8 en_find_keepid_flag = OAL_FALSE;
    OAL_CONST oal_nlattr_stru *pst_iter;
    mac_vowifi_nat_keep_alive_basic_info_stru st_stop_info;

    memset_s(&st_stop_info, OAL_SIZEOF(mac_vowifi_nat_keep_alive_basic_info_stru),
             0, OAL_SIZEOF(mac_vowifi_nat_keep_alive_basic_info_stru));

    oal_nla_for_each_attr(pst_iter, p_data, l_len, l_rem)
    {
        l_type = oal_nla_type(pst_iter);
        switch (l_type) {
            case MKEEP_ALIVE_ATTRIBUTE_ID:
                st_stop_info.uc_keep_alive_id = oal_nla_get_u8(pst_iter);
                en_find_keepid_flag = OAL_TRUE;
                break;
            default:
                return OAL_EFAIL;
        }
    }

    if (en_find_keepid_flag == OAL_TRUE) {
        st_stop_info.en_type = VOWIFI_MKEEP_ALIVE_TYPE_STOP;
#if 1 /* ����ʽ������ɺ�ɾ�� */
        oam_error_log2(0, OAM_SF_ANY, "{wl_cfgvendor_stop_vowifi_nat_keep_alive::en_type=[%d],id=[%d].}",
                       st_stop_info.en_type, st_stop_info.uc_keep_alive_id);
#endif
        l_ret = wal_send_vowifi_nat_keep_alive_params(pst_wdev->netdev,
            (uint8_t *)&st_stop_info, OAL_SIZEOF(mac_vowifi_nat_keep_alive_basic_info_stru));
        if (oal_unlikely(l_ret != OAL_SUCC)) {
            return -OAL_EFAIL;
        }
    }

    return OAL_SUCC;
}

#define QUERY_STATION_INFO_TIME_INTER (5 * OAL_TIME_HZ)
OAL_STATIC int32_t wal_cfgvendor_lstats_get_station_info(oal_wiphy_stru *pst_wiphy,
                                                           oal_wireless_dev_stru *pst_wdev,
                                                           wal_wifi_iface_stat_stru *pst_iface_stat)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    dmac_query_request_event st_dmac_query_request_event;
    dmac_query_station_info_request_event *pst_query_station_info = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    signed long i_leftime;
    int32_t l_ret;
    uint8_t uc_vap_id;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    oal_net_device_stru *pst_dev = pst_wdev->netdev;
    uint8_t *puc_mac = OAL_PTR_NULL;

    pst_mac_vap = oal_net_dev_priv(pst_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_station_info::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    pst_query_station_info = (dmac_query_station_info_request_event *)&st_dmac_query_request_event;
    pst_query_station_info->query_event = OAL_QUERY_STATION_INFO_EVENT;

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->us_assoc_vap_id);
        if (pst_mac_user == OAL_PTR_NULL) {
            return -OAL_EINVAL;
        }
        puc_mac = pst_mac_user->auc_user_mac_addr;
    } else {
        oam_warning_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_station_info:: not sta mode!}");
        return -OAL_EINVAL;
    }

    oal_set_mac_addr(pst_query_station_info->auc_query_sta_addr, (uint8_t *)puc_mac);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_station_info::mac_res_get_hmac_vap fail.}");
        return -OAL_EINVAL;
    }

    /* �̶�ʱ����������һ��RSSI */
    if (OAL_FALSE == wal_cfg80211_get_station_filter(&pst_hmac_vap->st_vap_base_info, (uint8_t *)puc_mac)) {
        pst_iface_stat->ac[0].ul_tx_mpdu = pst_hmac_vap->station_info.tx_packets;
        pst_iface_stat->ac[0].ul_rx_mpdu = pst_hmac_vap->station_info.rx_packets;
        pst_iface_stat->ac[0].ul_retries = pst_hmac_vap->station_info.tx_retries;
        pst_iface_stat->ac[0].ul_mpdu_lost = pst_hmac_vap->station_info.tx_failed;
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
        oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_lstats_get_station_info::memcpy fail!");
        return -OAL_EINVAL;
    }

    l_ret = wal_send_cfg_event(pst_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(dmac_query_station_info_request_event),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(uc_vap_id, OAM_SF_ANY,
            "{wal_cfgvendor_lstats_get_station_info::wal_send_cfg_event return err code %d!}", l_ret);
        return -OAL_EFAIL;
    }
    /*lint -e730*/ /* info, boolean argument to function */
    i_leftime = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
        (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), QUERY_STATION_INFO_TIME_INTER);
    /*lint +e730*/
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        oam_warning_log1(uc_vap_id, OAM_SF_ANY,
                         "{wal_cfgvendor_lstats_get_station_info::query info wait for %ld ms timeout!}",
                         ((QUERY_STATION_INFO_TIME_INTER * 1000) / OAL_TIME_HZ));
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        oam_warning_log1(uc_vap_id, OAM_SF_ANY,
                         "{wal_cfgvendor_lstats_get_station_info::query info wait for %ld ms error!}",
                         ((QUERY_STATION_INFO_TIME_INTER * 1000) / OAL_TIME_HZ));
        return -OAL_EINVAL;
    } else {
        /* ��������  */
        pst_iface_stat->ac[0].ul_tx_mpdu = pst_hmac_vap->station_info.tx_packets;
        pst_iface_stat->ac[0].ul_rx_mpdu = pst_hmac_vap->station_info.rx_packets;
        pst_iface_stat->ac[0].ul_retries = pst_hmac_vap->station_info.tx_retries;
        pst_iface_stat->ac[0].ul_mpdu_lost = pst_hmac_vap->station_info.tx_failed;
        return OAL_SUCC;
    }
}


OAL_STATIC int32_t wal_cfgvendor_lstats_get_info(oal_wiphy_stru *pst_wiphy,
                                                   oal_wireless_dev_stru *pst_wdev,
                                                   OAL_CONST void *p_data,
                                                   int32_t l_len)
{
    int32_t l_err = 0;
    uint32_t ul_reply_len;
    void *p_out_data = OAL_PTR_NULL;
    oal_netbuf_stru *pst_skb = OAL_PTR_NULL;
    wal_wifi_radio_stat_stru *pst_radio_stat = OAL_PTR_NULL;
    wal_wifi_iface_stat_stru *pst_iface_stat = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_wiphy, pst_wdev)) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{wal_cfgvendor_lstats_get_info:wiphy or wdev or data is null. %p, %p}",
                       (uintptr_t)pst_wiphy,
                       (uintptr_t)pst_wdev);
        return -OAL_EFAUL;
    }

    ul_reply_len = OAL_SIZEOF(wal_wifi_radio_stat_stru) + OAL_SIZEOF(wal_wifi_iface_stat_stru);
    p_out_data = (void *)oal_memalloc(ul_reply_len);
    if (p_out_data == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_info:alloc memory fail.[%d]}", ul_reply_len);
        return -OAL_ENOMEM;
    }
    memset_s(p_out_data, ul_reply_len, 0, ul_reply_len);

    /* ??radio ?? */
    pst_radio_stat = (wal_wifi_radio_stat_stru *)p_out_data;
    pst_radio_stat->ul_num_channels = VENDOR_NUM_CHAN;
    pst_radio_stat->ul_on_time = oal_jiffies_to_msecs(OAL_TIME_JIFFY - g_st_wifi_radio_stat.ull_wifi_on_time_stamp);
    pst_radio_stat->ul_tx_time = 0;
    pst_radio_stat->ul_rx_time = 0;

    /* ??interfac ?? */
    pst_iface_stat = (wal_wifi_iface_stat_stru *)(p_out_data + OAL_SIZEOF(*pst_radio_stat));
    pst_iface_stat->ul_num_peers = VENDOR_NUM_PEER;
    pst_iface_stat->peer_info->ul_num_rate = VENDOR_NUM_RATE;
    l_err = wal_cfgvendor_lstats_get_station_info(pst_wiphy, pst_wdev, pst_iface_stat);
    if (l_err != OAL_SUCC) {
        oal_free(p_out_data);
        return -OAL_EFAUL;
    }

    /* ??link ?? */
    pst_skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(pst_wiphy, ul_reply_len);
    if (oal_unlikely(!pst_skb)) {
        oal_free(p_out_data);
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_info::skb alloc failed.len %d}", ul_reply_len);
        return -OAL_ENOMEM;
    }

    oal_nla_put_nohdr(pst_skb, ul_reply_len, p_out_data);

    l_err = oal_cfg80211_vendor_cmd_reply(pst_skb);
    oam_warning_log4(0, OAM_SF_ANY, "{wal_cfgvendor_lstats_get_info::on_time %d, tx_time %d, rx_time %d, err %d",
                     pst_radio_stat->ul_on_time,
                     pst_radio_stat->ul_tx_time,
                     pst_radio_stat->ul_rx_time,
                     l_err);
    oal_free(p_out_data);
    return l_err;
}

#ifdef _PRE_WLAN_FEATURE_APF

OAL_STATIC int32_t wal_cfgvendor_apf_get_capabilities(oal_wiphy_stru *wiphy,
                                                        oal_wireless_dev_stru *wdev,
                                                        OAL_CONST void *data,
                                                        int32_t len)
{
    oal_netbuf_stru *skb;
    int32_t l_ret, l_mem_needed;
    uint32_t max_len;
    l_mem_needed = VENDOR_REPLY_OVERHEAD + (ATTRIBUTE_U32_LEN * 2);

    skb = oal_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, l_mem_needed);
    if (oal_unlikely(!skb)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_apf_get_capabilities::skb alloc failed, len %d", l_mem_needed);
        return -OAL_ENOMEM;
    }

    if (oal_strcmp(oal_netdevice_name(wdev->netdev), "wlan0") == 0) {
        max_len = APF_PROGRAM_MAX_LEN;
    } else {
        max_len = 0;
    }

    oal_nla_put_u32(skb, APF_ATTRIBUTE_VERSION, APF_VERSION);
    oal_nla_put_u32(skb, APF_ATTRIBUTE_MAX_LEN, max_len);

    l_ret = oal_cfg80211_vendor_cmd_reply(skb);
    if (oal_unlikely(l_ret)) {
        oam_error_log1(0, OAM_SF_ANY,
            "wal_cfgvendor_apf_get_capabilities::Vendor Command reply failed, ret:%d.", l_ret);
    }
    return l_ret;
}


OAL_STATIC int32_t wal_cfgvendor_apf_set_filter(oal_wiphy_stru *wiphy,
                                                  oal_wireless_dev_stru *wdev,
                                                  OAL_CONST void *data,
                                                  int32_t len)
{
    OAL_CONST oal_nlattr_stru *iter;
    int32_t l_tmp, l_type;
    mac_apf_filter_cmd_stru st_apf_filter_cmd;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = NULL;
    uint32_t ul_program_len;

    st_apf_filter_cmd.us_program_len = 0;

    /* assumption: length attribute must come first */
    oal_nla_for_each_attr(iter, data, len, l_tmp)
    {
        l_type = oal_nla_type(iter);
        switch (l_type) {
            case APF_ATTRIBUTE_PROGRAM_LEN:
                ul_program_len = oal_nla_get_u32(iter);
                if (oal_unlikely(!ul_program_len || ul_program_len > APF_PROGRAM_MAX_LEN)) {
                    oam_error_log1(0, OAM_SF_ANY,
                        "wal_cfgvendor_apf_set_filter::program len[%d] is invalid", ul_program_len);
                    return -OAL_EINVAL;
                }
                st_apf_filter_cmd.us_program_len = (uint16_t)ul_program_len;
                break;
            case APF_ATTRIBUTE_PROGRAM:
                if (oal_unlikely(!st_apf_filter_cmd.us_program_len)) {
                    oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_apf_set_filter::program len not set");
                    return -OAL_EINVAL;
                }

                st_apf_filter_cmd.puc_program = (uint8_t *)oal_nla_data(iter);
                st_apf_filter_cmd.en_cmd_type = APF_SET_FILTER_CMD;

                /***************************************************************************
                    ���¼���wal�㴦��
                ***************************************************************************/
                WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_APF_FILTER, OAL_SIZEOF(st_apf_filter_cmd));
                if (EOK != memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_apf_filter_cmd),
                                    &st_apf_filter_cmd, OAL_SIZEOF(st_apf_filter_cmd))) {
                    oam_error_log0(0, OAM_SF_ANY, "wal_cfgvendor_apf_set_filter::memcpy fail!");
                    return -OAL_EFAIL;
                }

                /* ������Ϣ */
                /* ��Ҫ�����͸ú�������Ϊͬ��������hmac����ʱ��ʹ�����ͷŵ��ڴ� */
                if (OAL_SUCC != wal_send_cfg_event(wdev->netdev,
                                                       WAL_MSG_TYPE_WRITE,
                                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_apf_filter_cmd),
                                                       (uint8_t *)&st_write_msg,
                                                       OAL_TRUE,
                                                       &pst_rsp_msg)) {
                    oam_error_log0(0, OAM_SF_ANY, "{wal_cfgvendor_apf_set_filter::wal_send_cfg_event fail!}");
                    return -OAL_EFAIL;
                }
                if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
                    oam_warning_log0(0, OAM_SF_ANY,
                        "{wal_cfgvendor_apf_set_filter::wal_check_and_release_msg_resp fail!}");
                    return -OAL_EFAIL;
                }
                break;
            default:
                oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_apf_set_filter::no such attribute %d", l_type);
                return -OAL_EINVAL;
        }
    }

    oam_warning_log1(0, 0, "wal_cfgvendor_apf_set_filter: program len %d", st_apf_filter_cmd.us_program_len);
    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_NAN

OAL_STATIC oal_net_device_stru *wal_cfgvendor_nan_get_attached_netdev(oal_wiphy_stru *wiphy)
{
    oal_net_device_stru *attached_dev;
    mac_wiphy_priv_stru *wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(wiphy));
    /* ת����wlan */
    attached_dev = wiphy_priv->pst_mac_device->st_p2p_info.pst_primary_net_device;
    if (!attached_dev) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::primary netdev is null}");
        return NULL;
    }
    /* wlan running���û����λʱ�����·�nan�¼� */
    if (!(oal_netdevice_flags(attached_dev) & OAL_IFF_RUNNING)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::wlan0 not running!}");
        return NULL;
    }
    return attached_dev;
}
OAL_STATIC int32_t wal_cfgvendor_nan_send_event(oal_wiphy_stru *wiphy, wal_msg_write_stru *write_msg,
    uint16_t len, oal_bool_enum_uint8 en_need_rsp, wal_msg_stru **ppst_rsp_msg)
{
    int32_t ret;
    oal_net_device_stru *attached_dev = wal_cfgvendor_nan_get_attached_netdev(wiphy);
    if (!attached_dev) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::get attached netdev failed}");
        return -OAL_EFAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(write_msg, WLAN_CFGID_NAN, len);
    ret = wal_send_cfg_event(attached_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + len,
        (uint8_t*)write_msg, en_need_rsp, ppst_rsp_msg);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::wal_send_cfg_event fail[%d]!}", ret);
    }
    return ret;
}

OAL_STATIC int32_t wal_cfgvendor_nan_check_netdev(oal_net_device_stru *net_dev)
{
    if (!net_dev || oal_strcmp(oal_netdevice_name(net_dev), "nan0")) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_check_netdev::Not nan netdev!}");

        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfgvendor_nan_set_param(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)

{
    wal_msg_write_stru write_msg;
    OAL_CONST oal_nlattr_stru *iter;
    int32_t tmp;
    int32_t attribute;
    mac_nan_cfg_msg_stru *nan_msg;
    mac_nan_param_stru *param;

    if (wal_cfgvendor_nan_check_netdev(wdev->netdev) != OAL_SUCC) {
        return -OAL_EFAIL;
    }

    nan_msg = (mac_nan_cfg_msg_stru*)write_msg.auc_value;
    nan_msg->type = NAN_CFG_TYPE_SET_PARAM;
    param = &nan_msg->param;
    nan_msg->transaction_id = 0;
    oal_nla_for_each_attr(iter, data, len, tmp) {
        attribute = oal_nla_type(iter);
        switch (attribute) {
            case NAN_ATTRIBUTE_TRANSACTION_ID:
                nan_msg->transaction_id = oal_nla_get_u8(iter);
                break;
            case NAN_ATTRIBUTE_DURATION_CONFIGURE:
                param->duration = oal_nla_get_u32(iter);
                break;
            case NAN_ATTRIBUTE_PERIOD_CONFIGURE:
                param->period = oal_nla_get_u32(iter);
                break;
            case NAN_ATTRIBUTE_TYPE_CONFIGURE:
                param->type = oal_nla_get_u8(iter);
                break;
            case NAN_ATTRIBUTE_BAND_CONFIGURE:
                param->band = oal_nla_get_u8(iter);
                break;
            case NAN_ATTRIBUTE_CHANNEL_CONFIGURE:
                param->channel = oal_nla_get_u8(iter);
                break;
            default:
                oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_nan_requset::no such attribute %d", attribute);
                return -OAL_EINVAL;
        }
    }

    oam_warning_log4(0, OAM_SF_ANY, "wal_cfgvendor_nan_requset: duration[%d], period[%d], band[%d], channel[%d]",
        param->duration, param->period, param->band, param->channel);

    return wal_cfgvendor_nan_send_event(wiphy, &write_msg, OAL_SIZEOF(mac_nan_cfg_msg_stru), OAL_FALSE, OAL_PTR_NULL);
}


OAL_STATIC int32_t wal_cfgvendor_nan_tx_mgmt(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len, uint8_t action)
{
    OAL_CONST oal_nlattr_stru *iter;
    int32_t tmp;
    int32_t attribute;
    wal_msg_write_stru write_msg;
    wal_msg_stru *pst_rsp_msg = NULL;
    mac_nan_cfg_msg_stru *nan_msg;
    mac_nan_mgmt_info_stru *mgmt_info;

    if (wal_cfgvendor_nan_check_netdev(wdev->netdev) != OAL_SUCC) {
        return -OAL_EFAIL;
    }

    nan_msg = (mac_nan_cfg_msg_stru*)write_msg.auc_value;
    nan_msg->type = NAN_CFG_TYPE_SET_TX_MGMT;
    mgmt_info = &nan_msg->mgmt_info;

    mgmt_info->action = action;
    mgmt_info->len = 0;
    mgmt_info->data = OAL_PTR_NULL;
    oal_nla_for_each_attr(iter, data, len, tmp) {
        attribute = oal_nla_type(iter);
        switch (attribute) {
            case NAN_ATTRIBUTE_TRANSACTION_ID:
                nan_msg->transaction_id = oal_nla_get_u16(iter);
                break;
            case NAN_ATTRIBUTE_DATATYPE:
                mgmt_info->periodic = oal_nla_get_u8(iter);
                break;
            case NAN_ATTRIBUTE_DATALEN:
                mgmt_info->len = oal_nla_get_u16(iter);
                break;
            case NAN_ATTRIBUTE_DATA_FRAME:
                mgmt_info->data = (uint8_t*)oal_nla_data(iter);
                break;
            default:
                oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_nan_tx_mgmt::no such attribute %d", attribute);
                return -OAL_EINVAL;
        }
    }
    if (mgmt_info->len > 400) { /* nan֡���������400�ֽ� */
        oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_nan_tx_mgmt::frame len[%d] too large %d", mgmt_info->len);
        return -OAL_EINVAL;
    }
    return wal_cfgvendor_nan_send_event(wiphy, &write_msg, OAL_SIZEOF(mac_nan_cfg_msg_stru), OAL_TRUE, &pst_rsp_msg);
}

OAL_STATIC int32_t wal_cfgvendor_nan_tx_publish(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    return wal_cfgvendor_nan_tx_mgmt(wiphy, wdev, data, len, WLAN_ACTION_NAN_PUBLISH);
}

OAL_STATIC int32_t wal_cfgvendor_nan_tx_followup(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    return wal_cfgvendor_nan_tx_mgmt(wiphy, wdev, data, len, WLAN_ACTION_NAN_FLLOWUP);
}

OAL_STATIC int32_t wal_cfgvendor_nan_tx_subscribe(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    return wal_cfgvendor_nan_tx_mgmt(wiphy, wdev, data, len, WLAN_ACTION_NAN_SUBSCRIBE);
}

OAL_STATIC int32_t wal_cfgvendor_nan_cancel_tx(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    /* �ݲ����� */
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_cfgvendor_nan_enable(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    oal_net_device_stru *attached_dev;
    oal_netbuf_stru *skb;
    mac_vap_stru *mac_vap;
    int32_t mem_needed;
    oal_gfp_enum_uint8 kflags = oal_in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
    OAL_CONST oal_nlattr_stru *iter;
    int32_t tmp;
    int32_t attribute;
    uint16_t rsp_data[4]; /* 4��short, 2�ֽ�transaction id��6�ֽ�mac��ַ */
    if (wal_cfgvendor_nan_check_netdev(wdev->netdev) != OAL_SUCC) {
        return -OAL_EFAIL;
    }
    oal_nla_for_each_attr(iter, data, len, tmp) {
        attribute = oal_nla_type(iter);
        switch (attribute) {
            case NAN_ATTRIBUTE_TRANSACTION_ID:
                rsp_data[0] = oal_nla_get_u16(iter);
                break;
            default:
                oam_warning_log1(0, OAM_SF_ANY, "wal_cfgvendor_nan_enable::no such attribute %d", attribute);
                return -OAL_EINVAL;
        }
    }

    attached_dev = wal_cfgvendor_nan_get_attached_netdev(wiphy);
    if (!attached_dev) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::cannot find netdev named wlan0}");
        return -OAL_EFAIL;
    }


    mem_needed = VENDOR_DATA_OVERHEAD + oal_nlmsg_length(OAL_SIZEOF(rsp_data));
    skb = oal_cfg80211_vendor_event_alloc(wdev->wiphy, wdev, mem_needed, NAN_EVENT_RX, kflags);
    if (oal_unlikely(skb == NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "wal_cfgvendor_nan_enable::skb alloc failed, len %d", mem_needed);
        return -OAL_EINVAL;
    }
    mac_vap = oal_net_dev_priv(attached_dev);
    if (!mac_vap) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::vap of wlan0 is null}");
        return -OAL_EFAIL;
    }
    oal_set_mac_addr((uint8_t*)&rsp_data[1], mac_mib_get_StationID(mac_vap)); /* 2-7�ֽڷ�wlan0 mac��ַ */
    oal_nla_put(skb, NAN_ATTRIBUTE_ENABLE_RSP, OAL_SIZEOF(rsp_data), (uint8_t*)rsp_data);
    oal_cfg80211_vendor_event(skb, kflags);

    return OAL_SUCC;
}

OAL_STATIC int32_t wal_cfgvendor_nan_disable(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    OAL_CONST void *data, int32_t len)
{
    return OAL_SUCC;
}
#endif
OAL_STATIC OAL_CONST oal_wiphy_vendor_command_stru wal_vendor_cmds[] = {
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = GSCAN_SUBCMD_GET_CHANNEL_LIST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_get_channel_list
    },

    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = ANDR_WIFI_SET_COUNTRY
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_set_country
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = ANDR_WIFI_SUBCMD_GET_FEATURE_SET
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_get_feature_set
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = ANDR_WIFI_RANDOM_MAC_OUI
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_set_random_mac_oui
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_SET_BSSID_BLACKLIST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_set_bssid_blacklist
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_FW_ROAM_POLICY
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_set_roam_policy
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_GET_FEATURE
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_get_feature
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_GET_VER
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_get_version
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_GET_RING_STATUS
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_get_ring_status
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_TRIGGER_MEM_DUMP
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_trigger_mem_dump
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_START_LOGGING
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_start_logging
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = DEBUG_GET_RING_DATA
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_dbg_get_ring_data
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_OFFLOAD_SUBCMD_START_MKEEP_ALIVE
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wl_cfgvendor_start_vowifi_nat_keep_alive
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_OFFLOAD_SUBCMD_STOP_MKEEP_ALIVE
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wl_cfgvendor_stop_vowifi_nat_keep_alive
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = LSTATS_SUBCMD_GET_INFO
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_lstats_get_info
    },
#ifdef _PRE_WLAN_FEATURE_APF
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = APF_SUBCMD_GET_CAPABILITIES
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_apf_get_capabilities
    },

    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = APF_SUBCMD_SET_FILTER
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_apf_set_filter
    },
#endif

#ifdef _PRE_WLAN_FEATURE_NAN
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_SET_PARAM
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_set_param
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_ENABLE
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_enable
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_DISABLE
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_disable
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_PUBLISH_REQUEST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_tx_publish
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_CANCEL_PUBLISH_REQUEST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_cancel_tx
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_SUBSCRIBE_REQUEST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_tx_subscribe
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_SUBSCRIBE_CANCEL_REQUEST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_cancel_tx
    },
    {   {   .vendor_id = OUI_VENDOR,
            .subcmd = WIFI_SUBCMD_NAN_FOLLOWUP_REQUEST
        },
        .flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
        .doit = wal_cfgvendor_nan_tx_followup
    },
#endif
};

OAL_STATIC OAL_CONST oal_nl80211_vendor_cmd_info_stru wal_vendor_events[] = {
    { OUI_HISI, HISI_VENDOR_EVENT_UNSPEC },
    { OUI_HISI, HISI_VENDOR_EVENT_PRIV_STR },
    [NAN_EVENT_RX] = { OUI_HISI, NAN_EVENT_RX },
};

void wal_cfgvendor_init(oal_wiphy_stru *wiphy)
{
    wiphy->vendor_commands = wal_vendor_cmds;
    wiphy->n_vendor_commands = oal_array_size(wal_vendor_cmds);
    wiphy->vendor_events = wal_vendor_events;
    wiphy->n_vendor_events = oal_array_size(wal_vendor_events);
}

void wal_cfgvendor_deinit(oal_wiphy_stru *wiphy)
{
    wiphy->vendor_commands = NULL;
    wiphy->vendor_events = NULL;
    wiphy->n_vendor_commands = 0;
    wiphy->n_vendor_events = 0;
}

