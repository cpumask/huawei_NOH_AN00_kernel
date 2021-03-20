

/* ͷ�ļ����� */
#include "oal_net.h"
#include "oal_cfg80211.h"
#include "oam_wdk.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_CFG80211_C

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static oal_kobj_uevent_env_stru g_env;
#endif

/*
 * �� �� ��  : oal_cfg80211_ready_on_channel
 * ��������  : �ϱ�linux �ں��Ѿ�����ָ���ŵ�
 */
void oal_cfg80211_ready_on_channel(oal_wireless_dev_stru *pst_wdev,
                                           uint64_t ull_cookie,
                                           oal_ieee80211_channel_stru *pst_chan,
                                           uint32_t ul_duration,
                                           oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_ready_on_channel(pst_wdev, ull_cookie, pst_chan, ul_duration, en_gfp);
#endif
}
/*
 * �� �� ��  : oal_cfg80211_vowifi_report
 * ��������  : �ϱ�linux �ں�vowifi/volte�߼��л�����
 */
void oal_cfg80211_vowifi_report(oal_net_device_stru *pst_netdev,
                                        oal_gfp_enum_uint8 en_gfp)
{
#ifdef CONFIG_HW_VOWIFI
    /* �˽ӿ�Ϊ�ն�ʵ�ֵ��ں˽ӿڣ����崦���ں˺�CONFIG_HW_VOWIFI���� */
    cfg80211_drv_vowifi(pst_netdev, en_gfp);
#endif /* CONFIG_HW_VOWIFI */
}

/*
 * �� �� ��  : oal_cfg80211_remain_on_channel_expired
 * ��������  : ������ʱ�ϱ�
 */
void oal_cfg80211_remain_on_channel_expired(oal_wireless_dev_stru *pst_wdev,
                                                    uint64_t ull_cookie,
                                                    oal_ieee80211_channel_stru *pst_listen_channel,
                                                    oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_remain_on_channel_expired(pst_wdev,
                                       ull_cookie,
                                       pst_listen_channel,
                                       GFP_ATOMIC);
#endif
}

void oal_cfg80211_mgmt_tx_status(struct wireless_dev *wdev, uint64_t cookie,
                                         OAL_CONST uint8_t *buf, size_t len,
                                         oal_bool_enum_uint8 ack, oal_gfp_enum_uint8 gfp)
{
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_mgmt_tx_status(wdev, cookie, buf, len, ack, gfp);
#endif
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
extern struct genl_family g_nl80211_fam;
extern struct genl_multicast_group g_nl80211_mlme_mcgrp;
#define NL80211_FAM (&g_nl80211_fam)
#define NL80211_GID (g_nl80211_mlme_mcgrp.id)

#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
/* For ko compile */
#else
/*
 * �� �� ��  : oal_cfg80211_calculate_bitrate
 * ��������  : oal_cfg80211_new_sta�ϱ�new sta�¼���ȡ������ֵ(�ο��ں�ʵ��)
 *             ���MCS���ڵ���32���ͷ��ش���OAL_ERR_CODE_CFG80211_MCS_EXCEED
 * �������  : pst_rate: ������Ϣ�ṹ
 * �� �� ֵ  : l_bitrate: ������
 */
OAL_STATIC int32_t oal_cfg80211_calculate_bitrate(oal_rate_info_stru *pst_rate)
{
    int32_t l_modulation;
    int32_t l_streams;
    int32_t l_bitrate;

    if ((pst_rate->flags & RATE_INFO_FLAGS_MCS) == 0) {
        return pst_rate->legacy;
    }

    /* the formula below does only work for MCS values smaller than 32 */
    if (pst_rate->mcs >= 32) {
        return -OAL_ERR_CODE_CFG80211_MCS_EXCEED;
    }
    /* ����MCS����ȡ��Ӧ�ı����� */
    l_modulation = pst_rate->mcs & 7;
    l_streams = (pst_rate->mcs >> 3) + 1;

    l_bitrate = (pst_rate->flags & RATE_INFO_FLAGS_40_MHZ_WIDTH) ? 13500000 : 6500000;

    if (l_modulation < 4) {
        l_bitrate *= (l_modulation + 1);
    } else if (l_modulation == 4) {
        l_bitrate *= (l_modulation + 2);
    } else {
        l_bitrate *= (l_modulation + 3);
    }
    l_bitrate *= l_streams;

    if (pst_rate->flags & RATE_INFO_FLAGS_SHORT_GI) {
        l_bitrate = (l_bitrate / 9) * 10;
    }
    /* do NOT round down here */
    return (l_bitrate + 50000) / 100000;
}

/*
 * �� �� ��  : oal_nl80211_send_station
 * ��������  : netlink�ϱ�send new sta�¼�����������ź�����ֵ���
 */
OAL_STATIC uint32_t oal_nl80211_send_station(oal_netbuf_stru *pst_buf, uint32_t ul_pid, uint32_t ul_seq,
                                               int32_t l_flags, oal_net_device_stru *pst_net_dev,
                                               const uint8_t *puc_mac_addr, oal_station_info_stru *pst_station_info)
{
    oal_nlattr_stru *pst_sinfoattr = OAL_PTR_NULL;
    oal_nlattr_stru *pst_txrate = OAL_PTR_NULL;
    void *p_hdr = OAL_PTR_NULL;
    int32_t us_bitrate;

    /* Add generic netlink header to netlink message, returns pointer to user specific header */
    p_hdr = oal_genlmsg_put(pst_buf, ul_pid, ul_seq, NL80211_FAM, l_flags, PRIV_NL80211_CMD_NEW_STATION);
    if (p_hdr == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }

    oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_IFINDEX, pst_net_dev->ifindex);
    oal_nla_put_m(pst_buf, PRIV_NL80211_ATTR_MAC, OAL_ETH_ALEN_SIZE, puc_mac_addr);

    /*
     * linux-2.6.30�ں�station_infoû��generation��Ա
     * NLA_PUT_U32(pst_buf, PRIV_NL80211_ATTR_GENERATION, pst_station_info->generation);
     */
    pst_sinfoattr = oal_nla_nest_start(pst_buf, PRIV_NL80211_ATTR_STA_INFO);
    if (pst_sinfoattr == OAL_PTR_NULL) {
        oal_genlmsg_cancel(pst_buf, p_hdr);
        return OAL_ERR_CODE_CFG80211_EMSGSIZE;
    }
    if (pst_station_info->filled & STATION_INFO_INACTIVE_TIME) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_STA_INFO_INACTIVE_TIME, pst_station_info->inactive_time);
    }
    if (pst_station_info->filled & STATION_INFO_RX_BYTES) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_STA_INFO_RX_BYTES, pst_station_info->rx_bytes);
    }
    if (pst_station_info->filled & STATION_INFO_TX_BYTES) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_STA_INFO_TX_BYTES, pst_station_info->tx_bytes);
    }
    if (pst_station_info->filled & STATION_INFO_LLID) {
        oal_nla_put_u16_m(pst_buf, PRIV_NL80211_STA_INFO_LLID, pst_station_info->llid);
    }
    if (pst_station_info->filled & STATION_INFO_PLID) {
        oal_nla_put_u16_m(pst_buf, PRIV_NL80211_STA_INFO_PLID, pst_station_info->plid);
    }
    if (pst_station_info->filled & STATION_INFO_PLINK_STATE) {
        oal_nla_put_u8(pst_buf, PRIV_NL80211_STA_INFO_PLINK_STATE, pst_station_info->plink_state);
    }
    if (pst_station_info->filled & STATION_INFO_SIGNAL) {
        oal_nla_put_u8(pst_buf, PRIV_NL80211_STA_INFO_SIGNAL, pst_station_info->signal);
    }
    if (pst_station_info->filled & STATION_INFO_TX_BITRATE) {
        pst_txrate = oal_nla_nest_start(pst_buf, PRIV_NL80211_STA_INFO_TX_BITRATE);
        if (pst_txrate == OAL_PTR_NULL) {
            oal_genlmsg_cancel(pst_buf, p_hdr);
            return OAL_ERR_CODE_CFG80211_EMSGSIZE;
        }

        /* cfg80211_calculate_bitrate will return negative for mcs >= 32 */
        us_bitrate = oal_cfg80211_calculate_bitrate(&pst_station_info->txrate);
        if (us_bitrate > 0) {
            oal_nla_put_u16_m(pst_buf, PRIV_NL80211_RATE_INFO_BITRATE, us_bitrate);
        }
        if (pst_station_info->txrate.flags & RATE_INFO_FLAGS_MCS) {
            oal_nla_put_u8(pst_buf, PRIV_NL80211_RATE_INFO_MCS, pst_station_info->txrate.mcs);
        }
        if (pst_station_info->txrate.flags & RATE_INFO_FLAGS_40_MHZ_WIDTH) {
            oal_nla_put_flag(pst_buf, PRIV_NL80211_RATE_INFO_40_MHZ_WIDTH);
        }
        if (pst_station_info->txrate.flags & RATE_INFO_FLAGS_SHORT_GI) {
            oal_nla_put_flag(pst_buf, PRIV_NL80211_RATE_INFO_SHORT_GI);
        }

        oal_nla_nest_end(pst_buf, pst_txrate);
    }
    if (pst_station_info->filled & STATION_INFO_RX_PACKETS) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_STA_INFO_RX_PACKETS, pst_station_info->rx_packets);
    }
    if (pst_station_info->filled & STATION_INFO_TX_PACKETS) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_STA_INFO_TX_PACKETS, pst_station_info->tx_packets);
    }

    oal_nla_nest_end(pst_buf, pst_sinfoattr);

    if (oal_genlmsg_end(pst_buf, p_hdr) < 0) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    return OAL_SUCC;

nla_put_failure:
    oal_genlmsg_cancel(pst_buf, p_hdr);
    return OAL_ERR_CODE_CFG80211_EMSGSIZE;
}


/*
 * �� �� ��  : oal_nl80211_send_connect_result
 * ��������  : ���������ں�netlink�ӿ��ϱ������ṹ
 */
OAL_STATIC uint32_t oal_nl80211_send_connect_result(oal_netbuf_stru *pst_buf,
                                                      oal_net_device_stru *pst_net_device,
                                                      const uint8_t *puc_bssid,
                                                      const uint8_t *puc_req_ie,
                                                      uint32_t ul_req_ie_len,
                                                      const uint8_t *puc_resp_ie,
                                                      uint32_t ul_resp_ie_len,
                                                      uint16_t us_status,
                                                      oal_gfp_enum_uint8 en_gfp)
{
    void *p_hdr = OAL_PTR_NULL;
    int32_t ul_let;

    p_hdr = oal_genlmsg_put(pst_buf, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_CONNECT);
    if (p_hdr == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_buf);
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }

    /*
     * ��������ϲ�û�н��������Բ��ϱ�
     * oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_WIPHY, pst_net_device->wiphy_idx);
     */
    oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_IFINDEX, pst_net_device->ifindex);
    if (puc_bssid != OAL_PTR_NULL) {
        oal_nla_put_m(pst_buf, PRIV_NL80211_ATTR_MAC, OAL_ETH_ALEN_SIZE, puc_bssid);
    }
    oal_nla_put_u16_m(pst_buf, PRIV_NL80211_ATTR_STATUS_CODE, us_status);
    if (puc_req_ie != OAL_PTR_NULL) {
        oal_nla_put_m(pst_buf, PRIV_NL80211_ATTR_REQ_IE, ul_req_ie_len, puc_req_ie);
    }
    if (puc_resp_ie != OAL_PTR_NULL) {
        oal_nla_put_m(pst_buf, PRIV_NL80211_ATTR_RESP_IE, ul_resp_ie_len, puc_resp_ie);
    }

    if (oal_genlmsg_end(pst_buf, p_hdr) < 0) {
        oal_nlmsg_free(pst_buf);
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    ul_let = oal_genlmsg_multicast(pst_buf, 0, NL80211_GID, en_gfp);
    if (ul_let < 0) {
        /* ���������hostapd��wpa_supplicant�Ļ������Ҳ��ʧ�ܣ����ﱨfail��Ӱ��ʹ�ã�ȥ������ */
        return OAL_FAIL;
    }

    return OAL_SUCC;

nla_put_failure:
    oal_genlmsg_cancel(pst_buf, p_hdr);
    oal_nlmsg_free(pst_buf);
    return OAL_ERR_CODE_CFG80211_EMSGSIZE;
}

OAL_STATIC uint32_t oal_nl80211_send_disconnected(oal_net_device_stru *pst_net_device,
                                                    uint16_t us_reason,
                                                    const uint8_t *puc_ie,
                                                    uint32_t ul_ie_len,
                                                    oal_bool_enum_uint8 from_ap,
                                                    oal_gfp_enum_uint8 en_gfp)
{
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    void *p_hdr = OAL_PTR_NULL;
    int32_t ul_let;

    pst_msg = oal_nlmsg_new(OAL_NLMSG_GOODSIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    p_hdr = oal_genlmsg_put(pst_msg, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_DISCONNECT);
    if (p_hdr == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }

    /*
     * ��������ϲ�û�н��������Բ��ϱ�
     * oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_WIPHY, pst_net_device->wiphy_idx);
     */
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_IFINDEX, pst_net_device->ifindex);
    if (from_ap && us_reason) {
        oal_nla_put_u16_m(pst_msg, PRIV_NL80211_ATTR_REASON_CODE, us_reason);
    }
    if (from_ap) {
        oal_nla_put_flag(pst_msg, PRIV_NL80211_ATTR_DISCONNECTED_BY_AP);
    }
    if (puc_ie == OAL_PTR_NULL) {
        oal_nla_put_m(pst_msg, PRIV_NL80211_ATTR_IE, ul_ie_len, puc_ie);
    }

    if (oal_genlmsg_end(pst_msg, p_hdr) < 0) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    ul_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);
    if (ul_let < 0) {
        /* oal_genlmsg_multicast�ӿ��ڲ����ͷ�skb������ʧ�ܲ���Ҫ�ֶ��ͷ� */
        return OAL_FAIL;
    }

    return OAL_SUCC;

nla_put_failure:
    oal_genlmsg_cancel(pst_msg, p_hdr);
    oal_nlmsg_free(pst_msg);
    return OAL_ERR_CODE_CFG80211_EMSGSIZE;
}
#endif
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE) &&             \
    (_PRE_CONFIG_TARGET_PRODUCT != _PRE_TARGET_PRODUCT_TYPE_E5) &&   \
    (_PRE_CONFIG_TARGET_PRODUCT != _PRE_TARGET_PRODUCT_TYPE_CPE) &&  \
    ((_PRE_TARGET_PRODUCT_TYPE_ONT != _PRE_CONFIG_TARGET_PRODUCT) && \
        (_PRE_TARGET_PRODUCT_TYPE_5630HERA != _PRE_CONFIG_TARGET_PRODUCT))
#else
OAL_STATIC uint32_t oal_nl80211_send_mgmt(oal_cfg80211_registered_device_stru *pst_rdev,
                                            oal_net_device_stru *pst_netdev,
                                            int32_t l_freq, uint8_t uc_rssi, const uint8_t *puc_buf,
                                            uint32_t ul_len, oal_gfp_enum_uint8 en_gfp)
{
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    void *p_hdr = OAL_PTR_NULL;
    int32_t l_let;

    pst_msg = oal_nlmsg_new(OAL_NLMSG_DEFAULT_SIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    p_hdr = oal_genlmsg_put(pst_msg, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_ACTION);
    if (p_hdr == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_WIPHY, pst_rdev->wiphy_idx);
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_IFINDEX, pst_netdev->ifindex);
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_WIPHY_FREQ, l_freq);
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_RX_SIGNAL_DBM, uc_rssi);  // report rssi to hostapd
    oal_nla_put_m(pst_msg, PRIV_NL80211_ATTR_FRAME, ul_len, puc_buf);
#else
    nla_put(pst_msg, PRIV_NL80211_ATTR_WIPHY, OAL_SIZEOF(pst_rdev->wiphy_idx), &(pst_rdev->wiphy_idx));
    nla_put(pst_msg, PRIV_NL80211_ATTR_IFINDEX, OAL_SIZEOF(pst_netdev->ifindex), &(pst_netdev->ifindex));
    nla_put(pst_msg, PRIV_NL80211_ATTR_WIPHY_FREQ, OAL_SIZEOF(l_freq), &l_freq);
    nla_put(pst_msg, PRIV_NL80211_ATTR_RX_SIGNAL_DBM, OAL_SIZEOF(uc_rssi), &uc_rssi);
    nla_put(pst_msg, PRIV_NL80211_ATTR_FRAME, ul_len, puc_buf);
#endif

    if (oal_genlmsg_end(pst_msg, p_hdr) < 0) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    l_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);
    if (l_let < 0) {
        return OAL_FAIL;
    }

    return OAL_SUCC;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
nla_put_failure:
    oal_genlmsg_cancel(pst_msg, p_hdr);
    oal_nlmsg_free(pst_msg);

    return OAL_ERR_CODE_CFG80211_EMSGSIZE;
#endif
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44)) ||  \
    (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5) ||  \
    (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_CPE)
/*
 * �� �� ��  : oal_nl80211_send_cac_msg
 * ��������  : netlink�ϱ�cac�¼�����������ź�����ֵ���
 */
OAL_STATIC int32_t oal_nl80211_send_cac_msg(oal_netbuf_stru *pst_buf, oal_net_device_stru *pst_net_dev,
                                              uint32_t ul_freq,
                                              enum PRIV_NL80211_CHAN_WIDTH en_chan_width,
                                              enum PRIV_NL80211_RADAR_EVENT en_event)
{
    void *p_hdr = OAL_PTR_NULL;

    /* Add generic netlink header to netlink message, returns pointer to user specific header */
    p_hdr = oal_genlmsg_put(pst_buf, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_RADAR_DETECT);
    if (p_hdr == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    if (pst_net_dev) {
        oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_IFINDEX, pst_net_dev->ifindex);
    }
    oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_RADAR_EVENT, en_event);
    oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_WIPHY_FREQ, ul_freq);
    oal_nla_put_u32_m(pst_buf, PRIV_NL80211_ATTR_CHANNEL_WIDTH, en_chan_width);
#else
    if (pst_net_dev) {
        if (nla_put(pst_buf, PRIV_NL80211_ATTR_IFINDEX, OAL_SIZEOF(pst_net_dev->ifindex), &(pst_net_dev->ifindex))) {
            goto nla_put_failure;
        }
    }

    if (nla_put(pst_buf, PRIV_NL80211_ATTR_RADAR_EVENT, OAL_SIZEOF(en_event), &en_event)) {
        goto nla_put_failure;
    }

    if (nla_put(pst_buf, PRIV_NL80211_ATTR_WIPHY_FREQ, OAL_SIZEOF(ul_freq), &ul_freq)) {
        goto nla_put_failure;
    }

    if (nla_put(pst_buf, PRIV_NL80211_ATTR_CHANNEL_WIDTH, OAL_SIZEOF(en_chan_width), &en_chan_width)) {
        goto nla_put_failure;
    }
#endif

    if (oal_genlmsg_end(pst_buf, p_hdr) < 0) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    return OAL_SUCC;

nla_put_failure:
    oal_genlmsg_cancel(pst_buf, p_hdr);
    return OAL_ERR_CODE_CFG80211_EMSGSIZE;
}
#endif
#endif

/*
 * �� �� ��  : oal_cfg80211_notify_cac_event
 * ��������  : �ϱ�CAC�¼�
 */
uint32_t oal_cfg80211_notify_cac_event(oal_net_device_stru *pst_net_device,
                                         uint32_t ul_freq,
                                         enum PRIV_NL80211_CHAN_WIDTH en_chan_width,
                                         enum PRIV_NL80211_RADAR_EVENT en_event,
                                         oal_gfp_enum_uint8 en_gfp)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44)) ||  \
    (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5) ||  \
    (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_CPE)
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    int32_t l_let;

    /* ����һ���µ�netlink��Ϣ */
    pst_msg = oal_nlmsg_new(OAL_NLMSG_GOODSIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    l_let = oal_nl80211_send_cac_msg(pst_msg, pst_net_device, ul_freq, en_chan_width, en_event);
    if (l_let != OAL_SUCC) {
        oal_nlmsg_free(pst_msg);
        return OAL_FAIL;
    }

    /* ���÷�װ���ں�netlink�㲥���ͺ��������ͳɹ�����0��ʧ��Ϊ��ֵ */
    l_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);
    if (l_let < 0) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
#else
    return OAL_FAIL;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_sched_scan_result
 * ��������  : �ϱ�����ɨ����
 */
void oal_cfg80211_sched_scan_result(oal_wiphy_stru *pst_wiphy)
{
#if (KERNEL_VERSION(4, 12, 0) <= LINUX_VERSION_CODE)
    cfg80211_sched_scan_results(pst_wiphy, 0);
    return;
#elif (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_sched_scan_results(pst_wiphy);
    return;
#else
    /* 51��֧�֣�do nothing */
    return;
#endif
}

void oal_kobject_uevent_env_sta_join(oal_net_device_stru *pst_net_device, const uint8_t *puc_mac_addr)
{
    memset_s(&g_env, sizeof(g_env), 0, sizeof(g_env));
    /* �ϲ���ҪSTA_JOIN��mac��ַ���м������Ч�����Ǳ�����4������ */
    add_uevent_var(&g_env, "SOFTAP=STA_JOIN wlan0 wlan0 %02x:%02x:%02x:%02x:%02x:%02x",
                   puc_mac_addr[0], puc_mac_addr[1], puc_mac_addr[2],
                   puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
#if ( KERNEL_VERSION(4, 1, 0) <= LINUX_VERSION_CODE)
    kobject_uevent_env(&(pst_net_device->dev.kobj), KOBJ_CHANGE, g_env.envp);
#else
    kobject_uevent_env(&(pst_net_device->dev.kobj), KOBJ_CHANGE, (char **)&g_env);
#endif
}

void oal_kobject_uevent_env_sta_leave(oal_net_device_stru *pst_net_device, const unsigned char *puc_mac_addr)
{
    memset_s(&g_env, sizeof(g_env), 0, sizeof(g_env));
    /* �ϲ���ҪSTA_LEAVE��mac��ַ���м������Ч�����Ǳ�����4������ */
    add_uevent_var(&g_env, "SOFTAP=STA_LEAVE wlan0 wlan0 %02x:%02x:%02x:%02x:%02x:%02x",
                   puc_mac_addr[0], puc_mac_addr[1], puc_mac_addr[2],
                   puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
#if ( KERNEL_VERSION(4, 1, 0) <= LINUX_VERSION_CODE)
    kobject_uevent_env(&(pst_net_device->dev.kobj), KOBJ_CHANGE, g_env.envp);
#else
    kobject_uevent_env(&(pst_net_device->dev.kobj), KOBJ_CHANGE, (char **)&g_env);
#endif
}

void oal_cfg80211_put_bss(oal_wiphy_stru *pst_wiphy, oal_cfg80211_bss_stru *pst_cfg80211_bss)
{
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_put_bss(pst_wiphy, pst_cfg80211_bss);
#else
    cfg80211_put_bss(pst_cfg80211_bss);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_get_bss
 * ��������  : ����bssid ��ssid �����ں˱����bss ��Ϣ
 */
oal_cfg80211_bss_stru *oal_cfg80211_get_bss(oal_wiphy_stru *pst_wiphy,
                                                oal_ieee80211_channel_stru *pst_channel,
                                                uint8_t *puc_bssid,
                                                uint8_t *puc_ssid,
                                                uint32_t ul_ssid_len)
{
#if ( KERNEL_VERSION(4, 1, 0) <= LINUX_VERSION_CODE)
    return cfg80211_get_bss(pst_wiphy, pst_channel, puc_bssid, puc_ssid, ul_ssid_len,
                            IEEE80211_BSS_TYPE_ANY, IEEE80211_PRIVACY_ANY);
#else
    return cfg80211_get_bss(pst_wiphy, pst_channel, puc_bssid, puc_ssid, ul_ssid_len,
                            WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_unlink_bss
 * ��������  : ��kernelɾ��bss entry��
 */
void oal_cfg80211_unlink_bss(oal_wiphy_stru *pst_wiphy, oal_cfg80211_bss_stru *pst_cfg80211_bss)
{
    cfg80211_unlink_bss(pst_wiphy, pst_cfg80211_bss);
}

oal_cfg80211_bss_stru *oal_cfg80211_inform_bss_frame(oal_wiphy_stru *pst_wiphy,
                                                         oal_ieee80211_channel_stru *pst_ieee80211_channel,
                                                         oal_ieee80211_mgmt_stru *pst_mgmt,
                                                         uint32_t ul_len,
                                                         int32_t l_signal,
                                                         oal_gfp_enum_uint8 en_ftp)
{
    return cfg80211_inform_bss_frame(pst_wiphy, pst_ieee80211_channel, pst_mgmt, ul_len, l_signal, en_ftp);
}

/*
 * �� �� ��  : oal_cfg80211_scan_done
 * ��������  : �ϱ�ɨ����ɽ��
 */
void oal_cfg80211_scan_done(oal_cfg80211_scan_request_stru *pst_cfg80211_scan_request, int8_t c_aborted)
{
#if (KERNEL_VERSION(4, 8, 0) <= LINUX_VERSION_CODE)
    struct cfg80211_scan_info info = {0};
    info.aborted = c_aborted;
    cfg80211_scan_done(pst_cfg80211_scan_request, &info);
#else
    cfg80211_scan_done(pst_cfg80211_scan_request, c_aborted);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_connect_result
 * ��������  : STA�ϱ�����������ṹ��
 */
uint32_t oal_cfg80211_connect_result(oal_net_device_stru *pst_net_device,
                                           const uint8_t *puc_bssid,
                                           const uint8_t *puc_req_ie,
                                           uint32_t ul_req_ie_len,
                                           const uint8_t *puc_resp_ie,
                                           uint32_t ul_resp_ie_len,
                                           uint16_t us_status,
                                           oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_connect_result(pst_net_device, puc_bssid, puc_req_ie, ul_req_ie_len,
                            puc_resp_ie, ul_resp_ie_len, us_status, en_gfp);

    return OAL_SUCC;
#else
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;

    /* ����һ���µ�netlink��Ϣ */
    pst_msg = oal_nlmsg_new(OAL_NLMSG_GOODSIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    pst_wdev = pst_net_device->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_wdev->iftype != NL80211_IFTYPE_STATION) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /*
     * ���������hostapd��wpa_supplicant�Ļ������Ҳ��ʧ�ܣ����ﱨfail��Ӱ��ʹ�ã�ȥ������
     * �ɹ��Ļ�����ӡSUCC, ���ɹ��Ļ�������ӡ
     */
    return oal_nl80211_send_connect_result(pst_msg, pst_net_device, puc_bssid, puc_req_ie, ul_req_ie_len,
                                           puc_resp_ie, ul_resp_ie_len, us_status, en_gfp);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_disconnected
 * ��������  : STA�ϱ����ں�ȥ�������
 */
uint32_t oal_cfg80211_disconnected(oal_net_device_stru *pst_net_device,
                                         uint16_t us_reason,
                                         uint8_t *puc_ie,
                                         uint32_t ul_ie_len,
                                         oal_gfp_enum_uint8 en_gfp)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    cfg80211_disconnected(pst_net_device, us_reason, puc_ie, ul_ie_len, 0, en_gfp);

    return OAL_SUCC;
#elif (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_disconnected(pst_net_device, us_reason, puc_ie, ul_ie_len, en_gfp);

    return OAL_SUCC;
#else
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;

    pst_wdev = pst_net_device->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_wdev->iftype != NL80211_IFTYPE_STATION) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /*
     * If this disconnect was due to a disassoc, we
     * we might still have an auth BSS around. For
     * the userspace SME that's currently expected,
     * but for the kernel SME (nl80211 CONNECT or
     * wireless extensions) we want to clear up all
     * state.
     */
    return oal_nl80211_send_disconnected(pst_net_device, us_reason, puc_ie,
                                         ul_ie_len, OAL_TRUE, en_gfp);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_roamed
 * ��������  : STA�ϱ����ں�ȥ�������
 */
uint32_t oal_cfg80211_roamed(oal_net_device_stru *pst_net_device,
                                   struct ieee80211_channel *pst_channel,
                                   const uint8_t *puc_bssid,
                                   const uint8_t *puc_req_ie,
                                   uint32_t ul_req_ie_len,
                                   const uint8_t *puc_resp_ie,
                                   uint32_t ul_resp_ie_len,
                                   oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(4, 12, 0) <= LINUX_VERSION_CODE)
    struct cfg80211_roam_info info = {0};
    info.channel = pst_channel;
    info.bssid = puc_bssid;
    info.req_ie = puc_req_ie;
    info.req_ie_len = ul_req_ie_len;
    info.resp_ie = puc_resp_ie;
    info.resp_ie_len = ul_resp_ie_len;
    cfg80211_roamed(pst_net_device, &info, en_gfp);
    return OAL_SUCC;
#elif (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_roamed(pst_net_device, pst_channel, puc_bssid,
                    puc_req_ie, ul_req_ie_len,
                    puc_resp_ie, ul_resp_ie_len, en_gfp);

    return OAL_SUCC;

#else
    return OAL_ERR_CODE_CONFIG_UNSUPPORT;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_ft_event
 * ��������  : STA�ϱ����ں�ft�¼�
 */
uint32_t oal_cfg80211_ft_event(oal_net_device_stru *pst_net_device, oal_cfg80211_ft_event_stru *pst_ft_event)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_ft_event(pst_net_device, pst_ft_event);

    return OAL_SUCC;

#else
    return OAL_ERR_CODE_CONFIG_UNSUPPORT;
#endif
}

/*
 * �� �� ��  : cfg80211_new_sta
 * ��������  : AP�ϱ��¹���ĳ��STA���
 */
uint32_t oal_cfg80211_new_sta(oal_net_device_stru *pst_net_device,
                                    const uint8_t *puc_mac_addr,
                                    oal_station_info_stru *pst_station_info,
                                    oal_gfp_enum_uint8 en_gfp)
{
#if defined(_PRE_PRODUCT_ID_HI110X_HOST) && (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    oal_kobject_uevent_env_sta_join(pst_net_device, puc_mac_addr);
#endif

#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_new_sta(pst_net_device, puc_mac_addr, pst_station_info, en_gfp);

    return OAL_SUCC;
#else
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    uint32_t ul_ret;
    int32_t l_let;

    /* ����һ���µ�netlink��Ϣ */
    pst_msg = oal_nlmsg_new(OAL_NLMSG_GOODSIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    ul_ret = oal_nl80211_send_station(pst_msg, 0, 0, 0, pst_net_device, puc_mac_addr, pst_station_info);
    if (ul_ret != OAL_SUCC) {
        oal_nlmsg_free(pst_msg);
        return ul_ret;
    }

    /* ���÷�װ���ں�netlink�㲥���ͺ��������ͳɹ�����0��ʧ��Ϊ��ֵ */
    l_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);
    if (l_let < 0) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_mic_failure
 * ��������  : �ϱ�mic����
 */
void oal_cfg80211_mic_failure(oal_net_device_stru *pst_net_device,
                                      const uint8_t *puc_mac_addr,
                                      enum nl80211_key_type key_type,
                                      int32_t key_id,
                                      const uint8_t *puc_tsc,
                                      oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    cfg80211_michael_mic_failure(pst_net_device, puc_mac_addr, key_type, key_id, puc_tsc, en_gfp);
#else
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    oal_cfg80211_registered_device_stru *pst_rdev = OAL_PTR_NULL;
    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    void *p_hdr = OAL_PTR_NULL;
    int32_t l_let;

    pst_wdev = pst_net_device->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        return;
    }

    if (pst_wdev->wiphy == OAL_PTR_NULL) {
        return;
    }
    pst_rdev = oal_wiphy_to_dev(pst_wdev->wiphy);

    pst_msg = oal_nlmsg_new(OAL_NLMSG_DEFAULT_SIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return;
    }

    p_hdr = oal_genlmsg_put(pst_msg, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_MICHAEL_MIC_FAILURE);
    if (p_hdr == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_msg);
        return;
    }

    /* rdev��Ӧ�ں�core.h�е�cfg80211_registered_device�ṹ�壬����������ϲ�û�д��� */
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_WIPHY, pst_rdev->wiphy_idx);
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_IFINDEX, pst_net_device->ifindex);

    if (puc_mac_addr != OAL_PTR_NULL) {
        oal_nla_put_m(pst_msg, PRIV_NL80211_ATTR_MAC, OAL_ETH_ALEN_SIZE, puc_mac_addr);
    }
    oal_nla_put_u32_m(pst_msg, PRIV_NL80211_ATTR_KEY_TYPE, key_type);
    oal_nla_put_u8(pst_msg, PRIV_NL80211_ATTR_KEY_IDX, key_id);
    if (puc_tsc != OAL_PTR_NULL) {
        oal_nla_put_m(pst_msg, PRIV_NL80211_ATTR_KEY_SEQ, 6, puc_tsc); /* 6Ϊ���ӵĳ��� */
    }

    if (oal_genlmsg_end(pst_msg, p_hdr) < 0) {
        oal_nlmsg_free(pst_msg);
        return;
    }

    l_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);
    if (l_let < 0) {
        return;
    }
    return;
nla_put_failure:
    oal_genlmsg_cancel(pst_msg, p_hdr);
    oal_nlmsg_free(pst_msg);
    return;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_del_sta
 * ��������  : AP�ϱ�ȥ����ĳ��STA���
 */
int32_t oal_cfg80211_del_sta(oal_net_device_stru *pst_net_device,
                                   const uint8_t *puc_mac_addr,
                                   oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    oal_kobject_uevent_env_sta_leave(pst_net_device, puc_mac_addr);
#endif

#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    cfg80211_del_sta(pst_net_device, puc_mac_addr, en_gfp);

    return OAL_SUCC;
#else

    oal_netbuf_stru *pst_msg = OAL_PTR_NULL;
    void *p_hdr = OAL_PTR_NULL;
    int32_t l_let;

    pst_msg = oal_nlmsg_new(OAL_NLMSG_DEFAULT_SIZE, en_gfp);
    if (pst_msg == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    p_hdr = oal_genlmsg_put(pst_msg, 0, 0, NL80211_FAM, 0, PRIV_NL80211_CMD_DEL_STATION);
    if (p_hdr == OAL_PTR_NULL) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL;
    }

    if (oal_nla_put_u32(pst_msg, PRIV_NL80211_ATTR_IFINDEX, pst_net_device->ifindex) ||
        oal_nla_put(pst_msg, PRIV_NL80211_ATTR_MAC, OAL_ETH_ALEN_SIZE, puc_mac_addr)) {
        oal_genlmsg_cancel(pst_msg, p_hdr);
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_EMSGSIZE;
    }

    if (oal_genlmsg_end(pst_msg, p_hdr) < 0) {
        oal_nlmsg_free(pst_msg);
        return OAL_ERR_CODE_CFG80211_ENOBUFS;
    }

    /*
     * liuux-2.6.30��liuux-2.6.34�ں˶��Ǵ����������,���ܴﵽҪ��
     * linux-2.6.34�ں˽��ŵ���genlmsg_multicast_netns(&init_net......)
     * linux-2.6.30�ں˽��ŵ���nlmsg_multicast(genl_sock......)
     */
    l_let = oal_genlmsg_multicast(pst_msg, 0, NL80211_GID, en_gfp);

    return l_let;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_rx_mgmt
 * ��������  : �ϱ����յ��Ĺ���֡
 * �������  : OAL_SUCC �ϱ��ɹ������������� �ϱ�ʧ��
 */
uint32_t oal_cfg80211_rx_mgmt(oal_net_device_stru *pst_dev,
                                    int32_t l_freq,
                                    uint8_t uc_rssi,
                                    const uint8_t *puc_buf,
                                    uint32_t ul_len,
                                    oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE) &&             \
    (_PRE_CONFIG_TARGET_PRODUCT != _PRE_TARGET_PRODUCT_TYPE_E5) &&   \
    (_PRE_CONFIG_TARGET_PRODUCT != _PRE_TARGET_PRODUCT_TYPE_CPE) &&  \
    ((_PRE_TARGET_PRODUCT_TYPE_ONT != _PRE_CONFIG_TARGET_PRODUCT) && \
     (_PRE_TARGET_PRODUCT_TYPE_5630HERA != _PRE_CONFIG_TARGET_PRODUCT))
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    uint32_t ul_ret;
    oal_bool_enum_uint8 uc_ret;
    pst_wdev = pst_dev->ieee80211_ptr;
    uc_ret = cfg80211_rx_mgmt(pst_wdev, l_freq, 0, puc_buf, ul_len, en_gfp);
    (uc_ret == OAL_TRUE) ? (ul_ret = OAL_SUCC) : (ul_ret = OAL_FAIL);
    return ul_ret;
#else
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    oal_cfg80211_registered_device_stru *pst_rdev = OAL_PTR_NULL;
    const uint8_t *action_data = OAL_PTR_NULL;

    pst_wdev = pst_dev->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_rdev = oal_wiphy_to_dev(pst_wdev->wiphy);

    /* action data starts with category */
    action_data = puc_buf + OAL_IEEE80211_MIN_ACTION_SIZE - 1;

    return oal_nl80211_send_mgmt(pst_rdev, pst_dev, l_freq, uc_rssi, puc_buf, ul_len, en_gfp);
#endif
}

/*
 * �� �� ��  : oal_cfg80211_rx_exception
 * ��������  : �յ��쳣���ϱ��ϲ�,˽������
 */
uint32_t oal_cfg80211_rx_exception(oal_net_device_stru *pst_netdev,
                                         uint8_t *puc_data,
                                         uint32_t ul_data_len)
{
    dev_netlink_send(puc_data, ul_data_len);
    oal_io_print("DFR OAL send[%s] over\n", puc_data);
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_vendor_cmd_alloc_reply_skb
 * ��������  : ���볧���Զ��巵������
 * �������  : oal_wiphy_stru * pst_wiphy: wiphy �ṹ
 *             uint32_t     ul_len     : ���볤��
 */
oal_netbuf_stru *oal_cfg80211_vendor_cmd_alloc_reply_skb(oal_wiphy_stru *pst_wiphy, uint32_t ul_len)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    return cfg80211_vendor_cmd_alloc_reply_skb(pst_wiphy, ul_len);
#else
    return OAL_PTR_NULL;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_vendor_cmd_reply
 * ��������  : �����Զ��������ϱ�
 * �������  : oal_netbuf_stru *pst_skb: ��������
 */
int32_t oal_cfg80211_vendor_cmd_reply(oal_netbuf_stru *pst_skb)
{
#if (KERNEL_VERSION(2, 6, 34) <= LINUX_VERSION_CODE)
    return cfg80211_vendor_cmd_reply(pst_skb);
#else
    return OAL_SUCC;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_m2s_status_report
 * ��������  : �ϱ�linux �ں�m2s�л����
 */
void oal_cfg80211_m2s_status_report(oal_net_device_stru *pst_netdev,
                                        oal_gfp_enum_uint8 en_gfp, uint8_t *puc_buf, uint32_t ul_len)
{
#ifdef CONFIG_HW_WIFI_MSS
    /* �˽ӿ�Ϊ�ն�ʵ�ֵ��ں˽ӿڣ����崦���ں˺�CONFIG_HW_WIFI_MSS���� */
    cfg80211_drv_mss_result(pst_netdev, en_gfp, puc_buf, ul_len);
#endif
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/*
 * �� �� ��  : oal_cfg80211_tas_rssi_access_report
 * ��������  : �ϱ�linux �ں�TAS���߲������
 */
void oal_cfg80211_tas_rssi_access_report(oal_net_device_stru *pst_netdev, oal_gfp_enum_uint8 en_gfp,
                                             uint8_t *puc_buf, uint32_t ul_len)
{
#ifdef CONFIG_HW_WIFI_RSSI
    cfg80211_drv_tas_result(pst_netdev, en_gfp, puc_buf, ul_len);
#endif
}
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
void oal_cfg80211_hid2d_seqnum_report(oal_net_device_stru *pst_netdev, oal_gfp_enum_uint8 en_gfp,
                                             uint8_t *puc_buf, uint32_t ul_len)
{
#ifdef CONFIG_HW_WIFI_HID2D_DROP
    cfg80211_drv_hid2d_seqnum_result(pst_netdev, en_gfp, puc_buf, ul_len);
#endif
}
#endif
/*
 * �� �� ��  : oal_cfg80211_external_auth_request
 * ��������  : �ϱ��ں�external_auth �¼�
 */
int oal_cfg80211_external_auth_request(oal_net_device_stru *pst_netdev,
                                           oal_cfg80211_external_auth_stru *pst_params,
                                           oal_gfp_enum_uint8 en_gfp)
{
#if (KERNEL_VERSION(4, 14, 0) <= LINUX_VERSION_CODE)
    return cfg80211_external_auth_request(pst_netdev, pst_params, en_gfp);
#else
    return -OAL_EFAIL;
#endif
}

/*
 * �� �� ��  : oal_cfg80211_ch_switch_notify
 * ��������  : �ϱ��ں�ch_switch �¼�
 */
void oal_cfg80211_ch_switch_notify(oal_net_device_stru *pst_netdev,
                                       oal_cfg80211_chan_def *pst_chandef)
{
    cfg80211_ch_switch_notify(pst_netdev,
                              pst_chandef);
}

/*
 * �� �� ��  : oal_cfg80211_vendor_event_alloc
 * ��������  : ����vendor�¼��ڴ�
 */
oal_netbuf_stru *oal_cfg80211_vendor_event_alloc(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    int approxlen, int event_idx, oal_gfp_enum_uint8 gfp)
{
    return cfg80211_vendor_event_alloc(wiphy, wdev, approxlen, event_idx, gfp);
}

/*
 * �� �� ��  : oal_cfg80211_vendor_event
 * ��������  : �ϱ�vendor�¼�
 */
void oal_cfg80211_vendor_event(oal_netbuf_stru *skb, oal_gfp_enum_uint8 gfp)
{
    cfg80211_vendor_event(skb, gfp);
}

#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
void oal_cfg80211_put_bss(oal_wiphy_stru *pst_wiphy, oal_cfg80211_bss_stru *pst_cfg80211_bss)
{
}

/*
 * �� �� ��  : oal_cfg80211_unlink_bss
 * ��������  : ��kernelɾ��bss entry��
 */
void oal_cfg80211_unlink_bss(oal_wiphy_stru *pst_wiphy, oal_cfg80211_bss_stru *pst_cfg80211_bss)
{
}

/*
 * �� �� ��  : oal_cfg80211_get_bss
 * ��������  : ����bssid ��ssid ���ұ����bss ��Ϣ
 */
oal_cfg80211_bss_stru *oal_cfg80211_get_bss(oal_wiphy_stru *pst_wiphy,
                                                oal_ieee80211_channel_stru *pst_channel,
                                                uint8_t *puc_bssid,
                                                uint8_t *puc_ssid,
                                                uint32_t ul_ssid_len)
{
    return (oal_cfg80211_bss_stru *)OAL_PTR_NULL;
}

oal_cfg80211_bss_stru *oal_cfg80211_inform_bss_frame(oal_wiphy_stru *pst_wiphy,
                                                         oal_ieee80211_channel_stru *pst_ieee80211_channel,
                                                         oal_ieee80211_mgmt_stru *pst_mgmt,
                                                         uint32_t ul_len,
                                                         int32_t l_signal,
                                                         oal_gfp_enum_uint8 en_ftp)
{
    return (oal_cfg80211_bss_stru *)OAL_PTR_NULL;
}

void oal_cfg80211_scan_done(oal_cfg80211_scan_request_stru *pst_cfg80211_scan_req, int8_t c_aborted)
{
}

/*
 * �� �� ��  : oal_cfg80211_sched_scan_result
 * ��������  : �ϱ�����ɨ����
 */
void oal_cfg80211_sched_scan_result(oal_wiphy_stru *pst_wiphy)
{
    return;
}

uint32_t oal_cfg80211_connect_result(oal_net_device_stru *pst_net_device,
                                           const uint8_t *puc_bssid,
                                           const uint8_t *puc_req_ie,
                                           uint32_t ul_req_ie_len,
                                           const uint8_t *puc_resp_ie,
                                           uint32_t ul_resp_ie_len,
                                           uint16_t us_status,
                                           oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_notify_cac_event
 * ��������  : �ϱ�CAC�¼�
 */
uint32_t oal_cfg80211_notify_cac_event(oal_net_device_stru *pst_net_device,
                                         uint32_t ul_freq,
                                         enum PRIV_NL80211_CHAN_WIDTH en_chan_width,
                                         enum PRIV_NL80211_RADAR_EVENT en_event,
                                         oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

uint32_t oal_cfg80211_roamed(oal_net_device_stru *pst_net_device,
                                   struct ieee80211_channel *pst_channel,
                                   const uint8_t *puc_bssid,
                                   const uint8_t *puc_req_ie,
                                   uint32_t ul_req_ie_len,
                                   const uint8_t *puc_resp_ie,
                                   uint32_t ul_resp_ie_len,
                                   oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

uint32_t oal_cfg80211_ft_event(oal_net_device_stru *pst_net_device, oal_cfg80211_ft_event_stru *pst_ft_event)
{
    return OAL_SUCC;
}

uint32_t oal_cfg80211_disconnected(oal_net_device_stru *pst_net_device,
                                         uint16_t us_reason,
                                         uint8_t *puc_ie,
                                         uint32_t ul_ie_len,
                                         oal_gfp_enum en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_new_sta
 * ��������  : AP �ϱ�������ĳ��sta�����
 */
uint32_t oal_cfg80211_new_sta(oal_net_device_stru *pst_net_device,
                                    const uint8_t *puc_mac_addr,
                                    oal_station_info_stru *pst_station_info,
                                    oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_fbt_notify_find_sta
 * ��������  : hilink fbt ֪ͨ�ҵ�sta
 */
uint32_t oal_cfg80211_fbt_notify_find_sta(oal_net_device_stru *pst_net_device,
                                            const uint8_t *puc_mac_addr,
                                            oal_station_info_stru *pst_station_info,
                                            oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_mic_failure
 * ��������  : �ϱ�mic����
 */
void oal_cfg80211_mic_failure(oal_net_device_stru *pst_net_device,
                                      const uint8_t *puc_mac_addr,
                                      enum nl80211_key_type key_type,
                                      int32_t key_id,
                                      const uint8_t *tsc,
                                      oal_gfp_enum_uint8 en_gfp)
{
    /* do nothing */
}

/*
 * �� �� ��  : oal_cfg80211_del_sta
 * ��������  : AP �ϱ�ȥ������ĳ��sta�����
 */
int32_t oal_cfg80211_del_sta(oal_net_device_stru *pst_net_device,
                                   const uint8_t *puc_mac_addr,
                                   oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_rx_mgmt
 * ��������  : �ϱ����յ��Ĺ���֡
 * �������  : OAL_SUCC �ϱ��ɹ������������� �ϱ�ʧ��
 */
uint32_t oal_cfg80211_rx_mgmt(oal_net_device_stru *pst_dev,
                                    int32_t l_freq,
                                    uint8_t uc_rssi,
                                    const uint8_t *puc_buf,
                                    uint32_t ul_len,
                                    oal_gfp_enum_uint8 en_gfp)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_rx_exception
 * ��������  : �յ��쳣���ϱ��ϲ�,˽������
 */
uint32_t oal_cfg80211_rx_exception(oal_net_device_stru *pst_netdev,
                                         uint8_t *puc_data,
                                         uint32_t ul_data_len)
{
    oal_io_print("DFR report excp to APP!!");
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_vendor_cmd_alloc_reply_skb
 * ��������  : ���볧���Զ��巵������
 * �������  : oal_wiphy_stru * pst_wiphy: wiphy �ṹ
 *             uint32_t     ul_len     : ���볤��
 */
oal_netbuf_stru *oal_cfg80211_vendor_cmd_alloc_reply_skb(oal_wiphy_stru *pst_wiphy, uint32_t ul_approxlen)
{
    return (oal_netbuf_stru *)OAL_PTR_NULL;
}

/*
 * �� �� ��  : oal_cfg80211_vendor_cmd_reply
 * ��������  : �����Զ��������ϱ�
 * �������  : oal_netbuf_stru *pst_skb: ��������
 */
int32_t oal_cfg80211_vendor_cmd_reply(oal_netbuf_stru *pst_skb)
{
    return OAL_SUCC;
}

void oal_cfg80211_m2s_status_report(oal_net_device_stru *pst_netdev,
                                        oal_gfp_enum_uint8 en_gfp, uint8_t *puc_buf, uint32_t ul_len)
{
    return;
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/*
 * �� �� ��  : oal_cfg80211_tas_rssi_access_report
 * ��������  : �ϱ�linux �ں�TAS���߲������
 */
void oal_cfg80211_tas_rssi_access_report(oal_net_device_stru *pst_netdev, oal_gfp_enum_uint8 en_gfp,
                                             uint8_t *puc_buf, uint32_t ul_len)
{
    return;
}
#endif

/*
 * �� �� ��  : oal_cfg80211_external_auth_request
 * ��������  : �ϱ��ں�external_auth �¼�
 */

int oal_cfg80211_external_auth_request(oal_net_device_stru *pst_netdev,
                                           oal_cfg80211_external_auth_stru *pst_params,
                                           oal_gfp_enum_uint8 en_gfp)
{
    /* for windows */
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_cfg80211_ch_switch_notify
 * ��������  : �ϱ��ں�ch_switch �¼�
 */
void oal_cfg80211_ch_switch_notify(oal_net_device_stru *pst_netdev,
                                       oal_cfg80211_chan_def *pst_chandef)
{
    /* for windows */
    return ;
}

/*
 * �� �� ��  : oal_cfg80211_vendor_event_alloc
 * ��������  : ����vendor�¼��ڴ�
 */
oal_netbuf_stru *oal_cfg80211_vendor_event_alloc(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    int approxlen, int event_idx, oal_gfp_enum_uint8 gfp)
{
    return (oal_netbuf_stru *)OAL_PTR_NULL;
}

/*
 * �� �� ��  : oal_cfg80211_vendor_event
 * ��������  : �ϱ�vendor�¼�
 */
void oal_cfg80211_vendor_event(oal_netbuf_stru *skb, oal_gfp_enum_uint8 gfp)
{
    /* for windows */
    return ;
}
#endif

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(oal_cfg80211_put_bss);
oal_module_symbol(oal_cfg80211_get_bss);
oal_module_symbol(oal_cfg80211_unlink_bss);
oal_module_symbol(oal_cfg80211_inform_bss_frame);
oal_module_symbol(oal_cfg80211_scan_done);
oal_module_symbol(oal_cfg80211_sched_scan_result);
oal_module_symbol(oal_cfg80211_connect_result);
oal_module_symbol(oal_cfg80211_roamed);
oal_module_symbol(oal_cfg80211_ft_event);
oal_module_symbol(oal_cfg80211_disconnected);
oal_module_symbol(oal_cfg80211_new_sta);
oal_module_symbol(oal_cfg80211_mic_failure);
oal_module_symbol(oal_cfg80211_del_sta);
oal_module_symbol(oal_cfg80211_rx_mgmt);
oal_module_symbol(oal_cfg80211_mgmt_tx_status);
oal_module_symbol(oal_cfg80211_ready_on_channel);
oal_module_symbol(oal_cfg80211_remain_on_channel_expired);
oal_module_symbol(oal_kobject_uevent_env_sta_join);
oal_module_symbol(oal_kobject_uevent_env_sta_leave);
oal_module_symbol(oal_cfg80211_rx_exception);
oal_module_symbol(oal_cfg80211_vendor_cmd_alloc_reply_skb);
oal_module_symbol(oal_cfg80211_vendor_cmd_reply);

oal_module_symbol(oal_cfg80211_vowifi_report);
oal_module_symbol(oal_cfg80211_m2s_status_report);
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
oal_module_symbol(oal_cfg80211_tas_rssi_access_report);
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
oal_module_symbol(oal_cfg80211_hid2d_seqnum_report);
#endif

oal_module_symbol(oal_cfg80211_external_auth_request);
oal_module_symbol(oal_cfg80211_ch_switch_notify);
oal_module_symbol(oal_cfg80211_vendor_event_alloc);
oal_module_symbol(oal_cfg80211_vendor_event);

