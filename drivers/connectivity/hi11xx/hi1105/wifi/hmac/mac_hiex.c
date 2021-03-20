

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "oal_types.h"
#include "mac_hiex.h"
#include "mac_common.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAL_FILE_ID_DRIVER_MAC_HIEX_C

/* hiex全局管理结构体 */
OAL_STATIC mac_hiex_mgr_stru g_st_hiex_mgr;

/* hiex芯片能力，由定制化下发 */
mac_hiex_cap_stru g_st_default_hiex_cap = {
#ifndef WIN32
    .bit_hiex_enable        = OAL_FALSE,
    .bit_hiex_version       = MAC_HIMIT_VERSION_V1,
    .bit_himit_enable       = OAL_FALSE,
    .bit_ht_himit_enable    = OAL_FALSE,
    .bit_vht_himit_enable   = OAL_FALSE,
    .bit_he_himit_enable    = OAL_FALSE,
    .bit_he_htc_himit_id    = 0,
    .bit_himit_version      = MAC_HIMIT_VERSION_V1,
#ifdef HIEX_CHIP_TYPE_1105
    .bit_chip_type          = MAC_HIEX_CHIP_TYPE_HI1105,
#else
    .bit_chip_type          = MAC_HIEX_CHIP_TYPE_HI1152,
#endif
    .bit_resv               = 0,
#else
    OAL_FALSE,                    // bit_hiex_enable
    MAC_HIMIT_VERSION_V1,         // bit_hiex_version
    OAL_FALSE,                    // bit_himit_enable
    OAL_FALSE,                    // bit_ht_himit_enable
    OAL_FALSE,                    // bit_vht_himit_enable
    OAL_FALSE,                    // bit_he_himit_enable
    0,                            // bit_he_htc_himit_id
    MAC_HIMIT_VERSION_V1,         // bit_himit_version
#ifdef HIEX_CHIP_TYPE_1105
    MAC_HIEX_CHIP_TYPE_HI1105,    // bit_chip_type
#else
    MAC_HIEX_CHIP_TYPE_HI1152,    // bit_chip_type
#endif
    0,                            // bit_resv
#endif
};


mac_hiex_mgr_stru *mac_hiex_get_mgr(void)
{
    return &g_st_hiex_mgr;
}


OAL_STATIC oal_bool_enum_uint8 mac_hiex_cap_compatible(mac_hiex_cap_stru *local, mac_hiex_cap_stru *peer)
{
    if (local->bit_himit_enable && peer->bit_himit_enable &&
        local->bit_he_htc_himit_id != peer->bit_he_htc_himit_id) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

uint32_t mac_hiex_nego_cap(mac_hiex_cap_stru *target, mac_hiex_cap_stru *local, mac_hiex_cap_stru *peer)
{
    mac_hiex_cap_stru  *tmp = OAL_PTR_NULL;

    if (target == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    if (memset_s(target, sizeof(mac_hiex_cap_stru), 0, sizeof(mac_hiex_cap_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_HIEX, "{mac_hiex_nego_cap::memset_s failed}");
        return OAL_FAIL;
    }

    /* 无对端设置，直接用本地能力，如封装beacon、probe request/rsp 、assoc request/rsp */
    if (local != OAL_PTR_NULL && peer == OAL_PTR_NULL) {
        tmp = local;
    } else if (local != OAL_PTR_NULL && peer != OAL_PTR_NULL) {
        if (!mac_hiex_cap_compatible(local, peer)) {
            return OAL_FAIL;
        }
        tmp = peer;
    } else {
        oam_warning_log0(0, OAM_SF_HIEX, "{mac_hiex_nego_cap::invalid config}");
        return OAL_FAIL;
    }

    target->bit_hiex_enable      = tmp->bit_hiex_enable && local->bit_hiex_enable;
    target->bit_hiex_version     = oal_min(tmp->bit_hiex_version, local->bit_hiex_version);
    target->bit_himit_enable     = tmp->bit_himit_enable && local->bit_himit_enable;
    target->bit_ht_himit_enable  = tmp->bit_ht_himit_enable && local->bit_ht_himit_enable;
    target->bit_vht_himit_enable = tmp->bit_vht_himit_enable && local->bit_vht_himit_enable;
    target->bit_he_himit_enable  = tmp->bit_he_himit_enable && local->bit_he_himit_enable;
    target->bit_he_htc_himit_id  = tmp->bit_he_htc_himit_id;
    target->bit_himit_version    = oal_min(tmp->bit_himit_version, local->bit_himit_version);
    target->bit_chip_type        = tmp->bit_chip_type;
    /* 不协商,本地能力 */
    target->bit_ersru_enable     = local->bit_ersru_enable;

    return OAL_SUCC;
}

uint8_t mac_hiex_set_vendor_ie(mac_hiex_cap_stru *band_cap, mac_hiex_cap_stru *user_cap,
                                 uint8_t *buffer)
{
    mac_hiex_vendor_ie_stru *vendor_ie = OAL_PTR_NULL;
    mac_hiex_cap_stru       *ie_cap = OAL_PTR_NULL;
    uint8_t                size = 0;

    if (!band_cap->bit_hiex_enable || (user_cap && !user_cap->bit_hiex_enable)) {
        return size;
    }

    vendor_ie = (mac_hiex_vendor_ie_stru *)buffer;
    vendor_ie->vender.uc_element_id = MAC_EID_VENDOR;
    vendor_ie->vender.uc_len = sizeof(mac_hiex_vendor_ie_stru) - MAC_IE_HDR_LEN;

    vendor_ie->vender.auc_oui[0] = (uint8_t)(((uint32_t)MAC_WLAN_OUI_HUAWEI >> NUM_16_BITS) & 0xff); //lint !e572
    vendor_ie->vender.auc_oui[1] = (uint8_t)(((uint32_t)MAC_WLAN_OUI_HUAWEI >> NUM_8_BITS) & 0xff);
    vendor_ie->vender.auc_oui[2] = (uint8_t)(((uint32_t)MAC_WLAN_OUI_HUAWEI) & 0xff);  /* hiex oui 数组2 */

    vendor_ie->vender.uc_oui_type = MAC_HISI_HIEX_IE;

    ie_cap = &vendor_ie->cap;
    if (mac_hiex_nego_cap(ie_cap, band_cap, user_cap) != OAL_SUCC) {
        return size;
    }

    return OAL_SIZEOF(mac_hiex_vendor_ie_stru);
}

#ifdef _PRE_WLAN_FEATURE_11AX

oal_bool_enum_uint8 mac_hiex_ie_get_bit_chip_type(uint8_t *puc_frame_body, uint16_t us_frame_len)
{
    mac_hiex_vendor_ie_stru *hiex_ie = OAL_PTR_NULL;
    hiex_ie = (mac_hiex_vendor_ie_stru *)mac_find_vendor_ie(MAC_WLAN_OUI_HUAWEI,
        MAC_HISI_HIEX_IE, puc_frame_body, (uint32_t)us_frame_len);
    /* 暂只与1152AP试行punctured preamble技术 */
    if (hiex_ie != OAL_PTR_NULL && hiex_ie->cap.bit_chip_type == MAC_HIEX_CHIP_TYPE_HI1152) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX_DBG_TST

oal_bool_enum_uint8 mac_hiex_is_valid_udp(uint8_t *ip_header)
{
    mac_ip_header_stru    *ip = (mac_ip_header_stru *)ip_header;
    udp_hdr_stru          *udp = OAL_PTR_NULL;
    uint16_t             len, src_port, dst_port;

    if (ip == OAL_PTR_NULL || ip->uc_protocol != MAC_UDP_PROTOCAL) {
        return OAL_FALSE;
    }
    udp = (udp_hdr_stru *)(ip + 1);
    len      = oal_net2host_short(udp->us_udp_len);
    dst_port = oal_net2host_short(udp->us_des_port);
    src_port = oal_net2host_short(udp->us_src_port);
    if (dst_port != DPERF_BASE_PORT && src_port != DPERF_BASE_PORT) {
        return OAL_FALSE;
    }
    if (len < OAL_SIZEOF(udp_hdr_stru) + OAL_SIZEOF(mac_hiex_rtt_stru) + OAL_SIZEOF(mac_hiex_rtt_stru)) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

oal_bool_enum_uint8 mac_hiex_pst_rx_mark(oal_netbuf_stru *netbuf, skb_header_type_enum hdr_type)
{
    uint8_t *ip_header = OAL_PTR_NULL;
    udp_hdr_stru *udp_hdr = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *mac_hdr = OAL_PTR_NULL;

    MAC_HIEX_RX_TRACED(netbuf) = 0;

    /* data at snap header */
    if (hdr_type == SKB_HEADER_SNAP) {
        ip_header = (uint8_t *)((mac_llc_snap_stru *)oal_netbuf_data(netbuf) + 1);
    } else if (hdr_type == SKB_HEADER_ETH) {
        ip_header = (uint8_t *)((mac_ether_header_stru *)oal_netbuf_data(netbuf) + 1);
    } else if ((hdr_type == SKB_HEADER_80211)) {
        mac_hdr = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr((mac_rx_ctl_stru *)oal_netbuf_cb(netbuf));
        ip_header = (uint8_t *)mac_hdr + MAC_GET_RX_CB_MAC_HEADER_LEN((mac_rx_ctl_stru *)oal_netbuf_cb(netbuf));
    } else if ((hdr_type == SKB_HEADER_IP)) {
        ip_header = oal_netbuf_data(netbuf);
    } else {
        oam_warning_log1(0, OAM_SF_HIEX, "{mac_hiex_pst_rx_mark::invalid type=%d}", hdr_type);
        return OAL_FALSE;
    }

    if (ip_header == OAL_PTR_NULL || !mac_hiex_is_valid_udp(ip_header)) {
        return OAL_FALSE;
    }

    MAC_HIEX_RX_TRACED(netbuf) = 1;
    udp_hdr = (udp_hdr_stru *)(ip_header + OAL_SIZEOF(mac_ip_header_stru));
    udp_hdr->us_check_sum = 0x0; /* disable udp checksum */

    return OAL_TRUE;
}

oal_bool_enum_uint8 mac_hiex_pst_tx_mark(oal_netbuf_stru *netbuf, skb_header_type_enum hdr_type)
{
    mac_ether_header_stru *ether_hdr = OAL_PTR_NULL;
    udp_hdr_stru          *udp_hdr = OAL_PTR_NULL;

    MAC_HIEX_TX_TRACED(netbuf) = 0;

    /* 待开发:other type */
    if (hdr_type != SKB_HEADER_ETH) {
        oam_error_log0(0, OAM_SF_HIEX, "{mac_hiex_pst_tx_mark::only supprot ether_hdr}");
        return OAL_FALSE;
    }

    /* data at ether header */
    ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (ether_hdr->us_ether_type != oal_host2net_short(ETHER_TYPE_IP)) {
        return OAL_FALSE;
    }

    /* 待开发:vlan not supported */
    if (!mac_hiex_is_valid_udp((uint8_t *)(ether_hdr + 1))) {
        return OAL_FALSE;
    }

    udp_hdr = (udp_hdr_stru *)((mac_ip_header_stru *)(ether_hdr + 1) + 1);
    udp_hdr->us_check_sum = 0x0; /* disable udp checksum */

    MAC_HIEX_TX_TRACED(netbuf) = 1;

    return OAL_TRUE;
}

uint32_t mac_hiex_get_timestamp(mac_vap_stru *vap)
{
#if IS_DEVICE
    return dmac_hiex_get_timestamp(vap);
#else
    oal_time_us_stru st_tst;
    oal_time_get_stamp_us(&st_tst);

    return (uint32_t)((uint64_t)st_tst.i_sec * 1000000ull + (uint64_t)st_tst.i_usec);
#endif
}


void mac_hiex_tst_update(mac_vap_stru *vap, oal_netbuf_stru *skb, skb_header_type_enum hdr_type,
    mac_hiex_tst_point_enum point)
{
    mac_hiex_rtt_stru *rtt = OAL_PTR_NULL;
    uint32_t         tst;

    if (point >= MAC_HIEX_TST_POINT_CNT) {
        oam_error_log1(0, OAM_SF_HIEX, "invalid tst point=%d", point);
        return;
    }

    if (MAC_HIEX_IS_TX_POINT(point) && !MAC_HIEX_TX_TRACED(skb)) {
        return;
    }

    if (MAC_HIEX_IS_RX_POINT(point) && !MAC_HIEX_RX_TRACED(skb)) {
        return;
    }

    if (MAC_HIEX_IS_DEVICE_POINT(point) && (vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_HIEX, "{mac_hiex_tst_update::VAP==null in device, point=%d}", point);
        return;
    }

    rtt = mac_hiex_get_rtt_stru(skb, hdr_type);
    if (rtt == OAL_PTR_NULL) {
        return;
    }

    tst = mac_hiex_get_timestamp(vap);

    MAC_HIEX_TST_IDX(&rtt->tst, point) = oal_host2net_long(tst);
    MAC_HIEX_TST_MAP(&rtt->tst) |= MAC_HIEX_TST_POINT_BIT(point);
}


void mac_hiex_show_delay(oal_netbuf_stru *netbuf, skb_header_type_enum header_type,
                             mac_hiex_tst_point_enum point_start, mac_hiex_tst_point_enum point_done)
{
    mac_hiex_tst_stru   *tst = OAL_PTR_NULL;
    mac_hiex_rtt_stru   *rtt = OAL_PTR_NULL;
    uint32_t           seq, start, done;

    if (MAC_HIEX_TX_TRACED(netbuf) || MAC_HIEX_RX_TRACED(netbuf)) {
        rtt  = mac_hiex_get_rtt_stru(netbuf, header_type);
        if (rtt == OAL_PTR_NULL) {
            return;
        }

        tst  = &rtt->tst;

        seq   = oal_net2host_long(rtt->iperf.seq);
        start = MAC_HIEX_TST_IDX(tst, point_start);
        done  = MAC_HIEX_TST_IDX(tst, point_done);
        start = oal_net2host_long(start);
        done  = oal_net2host_long(done);

        oam_warning_log1(0, OAM_SF_HIEX, "{mac_hiex_show_tst::map=0x%08x}", MAC_HIEX_TST_MAP(tst));
        if (!(MAC_HIEX_TST_MAP(tst) & MAC_HIEX_TST_POINT_BIT(point_start))
                || !(MAC_HIEX_TST_MAP(tst) & MAC_HIEX_TST_POINT_BIT(point_done))) {
            oam_error_log4(0, OAM_SF_HIEX, "{mac_hiex_show_tst::not all valid:point=%d seq=%d t1=%d t2=%d}",
                point_done, seq, start, done);
        } else {
            oam_warning_log3(0, OAM_SF_HIEX, "{HIEX_SHOW_START:point=%d seq=%d delay=%d HIEX_SHOW_DONE}",
                point_done, seq, OAL_DELTA_COUNT32(start, done));
        }
    }
}
#endif /* endif of _PRE_WLAN_FEATURE_HIEX_DBG_TST */
#endif /* endif of _PRE_WLAN_FEATURE_HIEX */
