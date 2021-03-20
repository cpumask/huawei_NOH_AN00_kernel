

#include "oal_net.h"
#include "frw_ext_if.h"
#include "hmac_tx_data.h"
#include "hmac_tx_amsdu.h"
#include "mac_frame.h"
#include "mac_data.h"
#include "hmac_frag.h"
#include "hmac_11i.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif

#include "hmac_traffic_classify.h"

#include "hmac_crypto_tkip.h"
#include "hmac_device.h"
#include "hmac_resource.h"

#include "hmac_tcp_opt.h"

#include "hisi_customize_wifi.h"

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
#include "hmac_wds.h"
#endif

#include "mem_trace.h"

#include "hmac_auto_adjust_freq.h"  // Ϊ����ͳ������׼��

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
#include "hmac_rr_performance.h"
#endif

#include "hmac_host_tx_data.h"
#include "hmac_ht_self_cure.h"

#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_DATA_C
/*
 * definitions of king of games feature
 */
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
#define HID2D_FRAME_MARK 0x5b
#endif
#ifdef CONFIG_NF_CONNTRACK_MARK
#define VIP_APP_VIQUE_TID WLAN_TIDNO_VIDEO
#define VIP_APP_MARK      0x5a
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#define PKTMARK(p)       (((struct sk_buff *)(p))->mark)
#define PKTSETMARK(p, m) ((struct sk_buff *)(p))->mark = (m)
#else /* !2.6.0 */
#define PKTMARK(p)       (((struct sk_buff *)(p))->nfmark)
#define PKTSETMARK(p, m) ((struct sk_buff *)(p))->nfmark = (m)
#endif /* 2.6.0 */
#else  /* CONFIG_NF_CONNTRACK_MARK */
#define PKTMARK(p) 0
#define PKTSETMARK(p, m)
#endif /* CONFIG_NF_CONNTRACK_MARK */

/* 2 ȫ�ֱ������� */
OAL_STATIC uint8_t g_uc_ac_new = 0;

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
OAL_STATIC void hmac_tcp_ack_buf_switch_judge(mac_vap_stru *pst_mac_vap,
                                              uint32_t ul_limit_throughput_high,
                                              uint32_t ul_limit_throughput_low,
                                              uint32_t ul_rx_throughput_mbps,
                                              uint8_t  uc_vap_idx);
OAL_STATIC void hmac_tcp_ack_buf_set_throughput_threshould(uint32_t ul_high_throughput_level,
                                                           uint32_t *ul_limit_throughput_high,
                                                           uint32_t *ul_limit_throughput_low,
                                                           oal_bool_enum_uint8  en_mu_vap);
#endif

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
hmac_tx_ampdu_hw_stru g_st_ampdu_hw = { 0 };
#endif

wlan_tx_switch_stru g_tx_switch = { 0 };

/* 3 ����ʵ�� */

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tx_is_dhcp(mac_ether_header_stru *pst_ether_hdr)
{
    mac_ip_header_stru *puc_ip_hdr;

    puc_ip_hdr = (mac_ip_header_stru *)(pst_ether_hdr + 1);

    return mac_is_dhcp_port(puc_ip_hdr);
}


OAL_STATIC void hmac_tx_report_dhcp_and_arp(mac_vap_stru *pst_mac_vap,
                                                mac_ether_header_stru *pst_ether_hdr,
                                                uint16_t us_ether_len)
{
    oal_bool_enum_uint8 en_flg = OAL_FALSE;

    switch (oal_host2net_short(pst_ether_hdr->us_ether_type)) {
        case ETHER_TYPE_ARP:
            en_flg = OAL_TRUE;
            break;

        case ETHER_TYPE_IP:
            en_flg = hmac_tx_is_dhcp(pst_ether_hdr);
            break;

        default:
            en_flg = OAL_FALSE;
            break;
    }

    if (en_flg && oam_report_dhcp_arp_get_switch()) {
        if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            oam_report_eth_frame(pst_ether_hdr->auc_ether_dhost,
                                     (uint8_t *)pst_ether_hdr, us_ether_len,
                                     OAM_OTA_FRAME_DIRECTION_TYPE_TX);
        } else if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
            oam_report_eth_frame(pst_mac_vap->auc_bssid,
                                     (uint8_t *)pst_ether_hdr, us_ether_len,
                                     OAM_OTA_FRAME_DIRECTION_TYPE_TX);
        } else {
        }
    }
}


uint32_t hmac_tx_report_eth_frame(mac_vap_stru *pst_mac_vap,
                                        oal_netbuf_stru *pst_netbuf)
{
    uint16_t us_user_idx = 0;
    mac_ether_header_stru *pst_ether_hdr = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_switch_enum_uint8 en_eth_switch = 0;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, pst_netbuf))) {
        oam_error_log2(0, OAM_SF_TX,
                       "{hmac_tx_report_eth_frame::input null %x %x}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_TX,
                       "{hmac_tx_report_eth_frame::mac_res_get_hmac_vap fail. vap_id = %u}",
                       pst_mac_vap->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ͳ����̫�����������ݰ�ͳ�� */
    /* �޸����� ��1)����pst_hmac_vap ��֮ǰ����ifdef ����ġ�2)ul_rx_pkt_to_lan Ӧ����ul_rx_bytes_to_lan��ͳ���ֽ��� */
#ifdef _PRE_WLAN_DFT_STAT
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_hmac_vap->st_query_stats.ul_rx_pkt_to_lan, 1);
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_hmac_vap->st_query_stats.ul_rx_bytes_to_lan, oal_netbuf_len(pst_netbuf));
#endif
    oam_stat_vap_incr(pst_mac_vap->uc_vap_id, tx_pkt_num_from_lan, 1);
    oam_stat_vap_incr(pst_mac_vap->uc_vap_id, tx_bytes_from_lan, oal_netbuf_len(pst_netbuf));

    /* ��ȡĿ���û���Դ��id���û�MAC��ַ�����ڹ��� */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);
        if (oal_unlikely(pst_ether_hdr == OAL_PTR_NULL)) {
            oam_error_log0(0, OAM_SF_TX, "{hmac_tx_report_eth_frame::ether_hdr is null!\r\n");
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, pst_ether_hdr->auc_ether_dhost, &us_user_idx);
        if (ul_ret == OAL_ERR_CODE_PTR_NULL) {
            oam_error_log1(0, OAM_SF_TX, "{hmac_tx_report_eth_frame::find user return ptr null!!\r\n", ul_ret);
            return ul_ret;
        }

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
        if (ul_ret != OAL_SUCC) {
            ul_ret = hmac_find_valid_user_by_wds_sta(pst_hmac_vap, pst_ether_hdr->auc_ether_dhost, &us_user_idx);
        }
#endif

        if (ul_ret == OAL_FAIL) {
            /* ����Ҳ����û�����֡������dhcp����arp request����Ҫ�ϱ� */
            hmac_tx_report_dhcp_and_arp(pst_mac_vap, pst_ether_hdr, (uint16_t)oal_netbuf_len(pst_netbuf));
            return OAL_SUCC;
        }

        oal_set_mac_addr(&auc_user_macaddr[0], pst_ether_hdr->auc_ether_dhost);
    }

    else if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        if (pst_mac_vap->us_user_nums == 0) {
            return OAL_SUCC;
        }
        pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);
        if (oal_unlikely(pst_ether_hdr == OAL_PTR_NULL)) {
            oam_error_log0(0, OAM_SF_TX, "{hmac_tx_report_eth_frame::ether_hdr is null!\r\n");
            return OAL_ERR_CODE_PTR_NULL;
        }
        /* ����Ҳ����û�����֡������dhcp����arp request����Ҫ�ϱ� */
        hmac_tx_report_dhcp_and_arp(pst_mac_vap, pst_ether_hdr, (uint16_t)oal_netbuf_len(pst_netbuf));
        us_user_idx = pst_mac_vap->us_assoc_vap_id;
        oal_set_mac_addr(&auc_user_macaddr[0], pst_mac_vap->auc_bssid);
    }

    /* ����ӡ��̫��֡�Ŀ��� */
    ul_ret = oam_report_eth_frame_get_switch(us_user_idx, OAM_OTA_FRAME_DIRECTION_TYPE_TX, &en_eth_switch);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_TX, "{hmac_tx_report_eth_frame::get tx eth frame switch fail!\r\n");
        return ul_ret;
    }

    if (en_eth_switch == OAL_SWITCH_ON) {
        /* ����̫��������֡�ϱ� */
        ul_ret = oam_report_eth_frame(&auc_user_macaddr[0],
                                      oal_netbuf_data(pst_netbuf),
                                      (uint16_t)oal_netbuf_len(pst_netbuf),
                                      OAM_OTA_FRAME_DIRECTION_TYPE_TX);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_TX,
                             "{hmac_tx_report_eth_frame::oam_report_eth_frame return err: 0x%x.}\r\n", ul_ret);
        }
    }

    return OAL_SUCC;
}


uint16_t hmac_free_netbuf_list(oal_netbuf_stru *pst_buf)
{
    oal_netbuf_stru *pst_buf_tmp = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_cb = OAL_PTR_NULL;
    uint16_t us_buf_num = 0;

    if (pst_buf != OAL_PTR_NULL) {
        pst_tx_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);

        while (pst_buf != OAL_PTR_NULL) {
            pst_buf_tmp = oal_netbuf_list_next(pst_buf);
            us_buf_num++;

            pst_tx_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
            
            if ((oal_netbuf_headroom(pst_buf) < MAC_80211_QOS_HTC_4ADDR_FRAME_LEN) &&
                (MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_cb) != OAL_PTR_NULL)) {
                oal_mem_free_m(MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_cb), OAL_TRUE);
                MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_cb) = OAL_PTR_NULL;
            }

            oal_netbuf_free(pst_buf);

            pst_buf = pst_buf_tmp;
        }
    } else {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_free_netbuf_list::pst_buf is null}");
    }

    return us_buf_num;
}

#ifdef _PRE_WLAN_FEATURE_HS20

void hmac_tx_set_qos_map(oal_netbuf_stru *pst_buf, uint8_t *puc_tid)
{
    mac_ether_header_stru *pst_ether_header;
    mac_ip_header_stru *pst_ip;
    uint8_t uc_dscp;
    mac_tx_ctl_stru *pst_tx_ctl;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t uc_idx;

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl));

    /* ��ȡ��̫��ͷ */
    pst_ether_header = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);
    /* �����Ϸ��Լ�� */
    if (oal_any_null_ptr2(pst_hmac_vap, pst_ether_header)) {
        oam_error_log0(0, OAM_SF_HS20,
            "{hmac_tx_set_qos_map::The input parameter of QoS_Map_Configure_frame_with_QoSMap_Set_element is NULL.}");
        return;
    }

    /* ��IP TOS�ֶ�Ѱ��DSCP���ȼ� */
    /* ---------------------------------
      tosλ����
      ---------------------------------
    |    bit7~bit2      | bit1~bit0 |
    |    DSCP���ȼ�     | ����      |
    --------------------------------- */
    /* ƫ��һ����̫��ͷ��ȡipͷ */
    pst_ip = (mac_ip_header_stru *)(pst_ether_header + 1);
    uc_dscp = pst_ip->uc_tos >> WLAN_DSCP_PRI_SHIFT;
    oam_info_log2(0, OAM_SF_HS20, "{hmac_tx_set_qos_map::tos = %d, uc_dscp=%d.}", pst_ip->uc_tos, uc_dscp);

    if ((pst_hmac_vap->st_cfg_qos_map_param.uc_num_dscp_except > 0) &&
        (pst_hmac_vap->st_cfg_qos_map_param.uc_num_dscp_except <= MAX_DSCP_EXCEPT) &&
        (pst_hmac_vap->st_cfg_qos_map_param.uc_valid)) {
        for (uc_idx = 0; uc_idx < pst_hmac_vap->st_cfg_qos_map_param.uc_num_dscp_except; uc_idx++) {
            if (uc_dscp == pst_hmac_vap->st_cfg_qos_map_param.auc_dscp_exception[uc_idx]) {
                *puc_tid = pst_hmac_vap->st_cfg_qos_map_param.auc_dscp_exception_up[uc_idx];
                MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
                MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_HS20;
                MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
                pst_hmac_vap->st_cfg_qos_map_param.uc_valid = 0;
                return;
            }
        }
    }

    for (uc_idx = 0; uc_idx < MAX_QOS_UP_RANGE; uc_idx++) {
        if ((uc_dscp < pst_hmac_vap->st_cfg_qos_map_param.auc_up_high[uc_idx]) &&
            (uc_dscp > pst_hmac_vap->st_cfg_qos_map_param.auc_up_low[uc_idx])) {
            *puc_tid = uc_idx;
            MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
            MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_HS20;
            MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
            pst_hmac_vap->st_cfg_qos_map_param.uc_valid = 0;
            return;
        } else {
            *puc_tid = 0;
        }
    }
    pst_hmac_vap->st_cfg_qos_map_param.uc_valid = 0;
    return;
}
#endif  // _PRE_WLAN_FEATURE_HS20


OAL_STATIC uint32_t hmac_set_tid_based_pktmark(oal_netbuf_stru *pst_buf, uint8_t *puc_tid,
    mac_tx_ctl_stru *pst_tx_ctl)
{
#ifdef CONFIG_NF_CONNTRACK_MARK
    /*
     * the king of game feature will mark packets
     * and we will use VI queue to send these packets.
     */
    if (PKTMARK(pst_buf) == VIP_APP_MARK) {
        *puc_tid = VIP_APP_VIQUE_TID;
        pst_tx_ctl->bit_is_needretry = OAL_TRUE;
        return OAL_TRUE;
    }
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    if (PKTMARK(pst_buf) == HID2D_FRAME_MARK) {
        *puc_tid = WLAN_TIDNO_VIDEO;
        pst_tx_ctl->bit_is_needretry = OAL_TRUE;
        return OAL_TRUE;
    }
#endif
    return OAL_FALSE;
}

OAL_STATIC OAL_INLINE void hmac_tx_classify_lan_to_wlan(oal_netbuf_stru *pst_buf, uint8_t *puc_tid)
{
    mac_ether_header_stru *pst_ether_header = OAL_PTR_NULL;
    mac_ip_header_stru *pst_ip = OAL_PTR_NULL;
    oal_vlan_ethhdr_stru *pst_vlan_ethhdr = OAL_PTR_NULL;
    uint32_t ul_ipv6_hdr;
    uint32_t ul_pri;
    uint16_t us_vlan_tci;
    uint8_t uc_tid = 0;
    uint8_t uc_dscp;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
#endif
#ifdef _PRE_WLAN_FEATURE_SCHEDULE
    mac_tcp_header_stru *pst_tcp;
#endif

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl));
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_TX, "{hmac_tx_classify_lan_to_wlan::mac_res_get_hmac_vap fail.vap_index[%u]}",
            MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl));
        return;
    }

    if (hmac_set_tid_based_pktmark(pst_buf, puc_tid, pst_tx_ctl) == OAL_TRUE){
        return ;
    }

    /* ��ȡ��̫��ͷ */
    pst_ether_header = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);

    switch (pst_ether_header->us_ether_type) {
            /*lint -e778*/ /* ����Info-- Constant expression evaluates to 0 in operation '&' */
        case oal_host2net_short(ETHER_TYPE_IP):
            oam_info_log0(0, OAM_SF_TX, "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_IP.}");

#ifdef _PRE_WLAN_FEATURE_HS20
            if (pst_hmac_vap->st_cfg_qos_map_param.uc_valid) {
                hmac_tx_set_qos_map(pst_buf, &uc_tid);
                *puc_tid = uc_tid;
                return;
            }
#endif  // _PRE_WLAN_FEATURE_HS20

            /* ��IP TOS�ֶ�Ѱ�����ȼ� */
            /* ----------------------------------------------------------------------
                        tosλ����
               ----------------------------------------------------------------------
                    | bit7~bit5 | bit4 |  bit3  |  bit2  |   bit1   | bit0 |
                    | �����ȼ�  | ʱ�� | ������ | �ɿ��� | ����ɱ� | ���� |
               ---------------------------------------------------------------------- */
            pst_ip = (mac_ip_header_stru *)(pst_ether_header + 1); /* ƫ��һ����̫��ͷ��ȡipͷ */

            uc_dscp = pst_ip->uc_tos >> WLAN_DSCP_PRI_SHIFT;
            if (uc_dscp >= HMAC_MAX_DSCP_VALUE_NUM) {
                break;
            }
            if (pst_hmac_vap->auc_dscp_tid_map[uc_dscp] != HMAC_DSCP_VALUE_INVALID) {
                uc_tid = pst_hmac_vap->auc_dscp_tid_map[uc_dscp];
                break;
            }

            uc_tid = pst_ip->uc_tos >> WLAN_IP_PRI_SHIFT;

            if (OAL_SWITCH_ON == mac_mib_get_TxTrafficClassifyFlag(&pst_hmac_vap->st_vap_base_info)) {
                if (uc_tid != 0) {
                    break;
                }
                /* RTP RTSP ����ֻ�� P2P�ϲſ���ʶ���� */
                if (!IS_LEGACY_VAP(&(pst_hmac_vap->st_vap_base_info))) {
                    hmac_tx_traffic_classify(pst_tx_ctl, pst_ip, &uc_tid);
                }
            }

            oam_info_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::tos = %d, uc_tid=%d.}",
                          pst_ip->uc_tos, uc_tid);
            /* �����DHCP֡�������VO���з��� */
            if (OAL_TRUE == mac_is_dhcp_port(pst_ip)) {
                uc_tid = WLAN_DATA_VIP_TID;
                MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
                MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_DHCP;
                MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
            }
#ifdef _PRE_WLAN_FEATURE_SCHEDULE
            /* ����chariot����Ľ������⴦����ֹ���� */
            else if (pst_ip->uc_protocol == MAC_TCP_PROTOCAL) {
                pst_tcp = (mac_tcp_header_stru *)(pst_ip + 1);

                if (oal_value_eq_any2(MAC_CHARIOT_NETIF_PORT, pst_tcp->us_dport, pst_tcp->us_sport)) {
                    oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                                  "{hmac_tx_classify_lan_to_wlan::chariot netif tcp pkt.}");
                    uc_tid = WLAN_DATA_VIP_TID;
                    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
                    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_CHARIOT_SIG;
                    MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
                }
            }
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
            pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
            if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
                oam_warning_log1(0, OAM_SF_CFG, "{hmac_edca_opt_rx_pkts_stat::null param,pst_hmac_user[%d].}",
                                 MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
                break;
            }

            if ((pst_hmac_vap->uc_edca_opt_flag_ap == OAL_TRUE) &&
                (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP)) {
                /* mips�Ż�:�������ҵ��ͳ�����ܲ�10M���� */
                if (((pst_ip->uc_protocol == MAC_UDP_PROTOCAL) &&
                     (pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tid)][WLAN_TX_UDP_DATA] <
                      (HMAC_EDCA_OPT_PKT_NUM + 10))) ||
                    ((pst_ip->uc_protocol == MAC_TCP_PROTOCAL) &&
                     (pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tid)][WLAN_TX_TCP_DATA] <
                      (HMAC_EDCA_OPT_PKT_NUM + 10)))) {
                    hmac_edca_opt_tx_pkts_stat(pst_tx_ctl, uc_tid, pst_ip);
                }
            }
#endif
            break;

        case oal_host2net_short(ETHER_TYPE_IPV6):
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_IPV6.}");
            /* ��IPv6 traffic class�ֶλ�ȡ���ȼ� */
            /* ----------------------------------------------------------------------
                IPv6��ͷ ǰ32Ϊ����
             -----------------------------------------------------------------------
            | �汾�� | traffic class   | ������ʶ |
            | 4bit   | 8bit(ͬipv4 tos)|  20bit   |
            ----------------------------------------------------------------------- */
            ul_ipv6_hdr = *((uint32_t *)(pst_ether_header + 1)); /* ƫ��һ����̫��ͷ��ȡipͷ */

            ul_pri = (oal_net2host_long(ul_ipv6_hdr) & WLAN_IPV6_PRIORITY_MASK) >> WLAN_IPV6_PRIORITY_SHIFT;

            uc_tid = (uint8_t)(ul_pri >> WLAN_IP_PRI_SHIFT);
            oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::uc_tid=%d.}", uc_tid);

            /* �����DHCPV6֡�������VO���з��� */
            if (OAL_TRUE == mac_is_dhcp6((oal_ipv6hdr_stru *)(pst_ether_header + 1))) {
                oam_info_log0(0, OAM_SF_TX, "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_DHCP6.}");
                uc_tid = WLAN_DATA_VIP_TID;
                MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
                MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_DHCPV6;
                MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
            }
            break;

        case oal_host2net_short(ETHER_TYPE_PAE):
            /* �����EAPOL֡�������VO���з��� */
            uc_tid = WLAN_DATA_VIP_TID;
            MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
            MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_EAPOL;
            MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;

            /* �����4 ���������õ�����Կ��������tx cb ��bit_is_eapol_key_ptk ��һ��dmac ���Ͳ����� */
            if (OAL_TRUE == mac_is_eapol_key_ptk((mac_eapol_header_stru *)(pst_ether_header + 1))) {
                MAC_GET_CB_IS_EAPOL_KEY_PTK(pst_tx_ctl) = OAL_TRUE;
            }

            oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CONN,
                             "{hmac_tx_classify_lan_to_wlan:: EAPOL tx : uc_tid=%d,IS_EAPOL_KEY_PTK=%d.}",
                             uc_tid, MAC_GET_CB_IS_EAPOL_KEY_PTK(pst_tx_ctl));

            break;

        /* TDLS֡��������������������ȼ�TID���� */
        case oal_host2net_short(ETHER_TYPE_TDLS):
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_TDLS.}");
            uc_tid = WLAN_DATA_VIP_TID;
            oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::uc_tid=%d.}", uc_tid);
            break;

        /* PPPOE֡������������(���ֽ׶�, �Ự�׶�)��������ȼ�TID���� */
        case oal_host2net_short(ETHER_TYPE_PPP_DISC):
        case oal_host2net_short(ETHER_TYPE_PPP_SES):
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_PPP_DISC, ETHER_TYPE_PPP_SES.}");
            uc_tid = WLAN_DATA_VIP_TID;
            MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
            MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_PPPOE;
            MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;

            oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::uc_tid=%d.}", uc_tid);
            break;

#ifdef _PRE_WLAN_FEATURE_WAPI
        case oal_host2net_short(ETHER_TYPE_WAI):
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_WAI.}");
            uc_tid = WLAN_DATA_VIP_TID;
            MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
            MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = MAC_DATA_WAPI;
            MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
            break;
#endif

        case oal_host2net_short(ETHER_TYPE_VLAN):
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::ETHER_TYPE_VLAN.}");
            /* ��ȡvlan tag�����ȼ� */
            pst_vlan_ethhdr = (oal_vlan_ethhdr_stru *)oal_netbuf_data(pst_buf);

            /* ------------------------------------------------------------------
                802.1Q(VLAN) TCI(tag control information)λ����
             -------------------------------------------------------------------
            |Priority | DEI  | Vlan Identifier |
            | 3bit    | 1bit |      12bit      |
             ------------------------------------------------------------------ */
            us_vlan_tci = oal_net2host_short(pst_vlan_ethhdr->h_vlan_TCI);

            uc_tid = us_vlan_tci >> OAL_VLAN_PRIO_SHIFT; /* ����13λ����ȡ��3λ���ȼ� */
            oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::uc_tid=%d.}", uc_tid);

            break;
        /*lint +e778*/
        default:
            oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                          "{hmac_tx_classify_lan_to_wlan::default us_ether_type[%d]}", pst_ether_header->us_ether_type);
            break;
    }

    /* ���θ�ֵ */
    *puc_tid = uc_tid;
}


OAL_STATIC OAL_INLINE void hmac_tx_update_tid(oal_bool_enum_uint8 en_wmm, uint8_t *puc_tid)
{
    if (oal_likely(en_wmm == OAL_TRUE)) { /* wmmʹ�� */
        *puc_tid = (*puc_tid < WLAN_TIDNO_BUTT) ? WLAN_TOS_TO_TID(*puc_tid) : WLAN_TIDNO_BCAST;
    } else { /* wmm��ʹ�� */
        *puc_tid = MAC_WMM_SWITCH_TID;
    }
}


uint8_t hmac_tx_wmm_acm(oal_bool_enum_uint8 en_wmm, hmac_vap_stru *pst_hmac_vap, uint8_t *puc_tid)
{
    uint8_t uc_ac;

    if (oal_any_null_ptr2(pst_hmac_vap, puc_tid)) {
        return OAL_FALSE;
    }

    if (en_wmm == OAL_FALSE) {
        return OAL_FALSE;
    }

    uc_ac = WLAN_WME_TID_TO_AC(*puc_tid);
    g_uc_ac_new = uc_ac;
    while ((g_uc_ac_new != WLAN_WME_AC_BK) &&
           (OAL_TRUE == mac_mib_get_QAPEDCATableMandatory(&pst_hmac_vap->st_vap_base_info, g_uc_ac_new))) {
        switch (g_uc_ac_new) {
            case WLAN_WME_AC_VO:
                g_uc_ac_new = WLAN_WME_AC_VI;
                break;

            case WLAN_WME_AC_VI:
                g_uc_ac_new = WLAN_WME_AC_BE;
                break;

            default:
                g_uc_ac_new = WLAN_WME_AC_BK;
                break;
        }
    }

    if (g_uc_ac_new != uc_ac) {
        *puc_tid = WLAN_WME_AC_TO_TID(g_uc_ac_new);
    }

    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE void hmac_tx_classify_get_tid(mac_device_stru *pst_mac_dev,
    hmac_vap_stru *pst_hmac_vap, mac_tx_ctl_stru *pst_tx_ctl, uint8_t *puc_tid)
{
    /* ���ʹ����vap���ȼ�����������õ�vap���ȼ� */
    if (pst_mac_dev->en_vap_classify == OAL_TRUE) {
        *puc_tid = mac_mib_get_VAPClassifyTidNo(&pst_hmac_vap->st_vap_base_info);
    }

    /* ���ap�ر���WMM�������б�����BE ���� */
    if (!pst_hmac_vap->st_vap_base_info.en_vap_wmm) {
        *puc_tid = WLAN_TIDNO_BEST_EFFORT;
    }

    /* tid7��VIP֡ʹ�ã�������VO�ۺϣ���ͨ����֡tid��Ϊtid6 */
    if ((*puc_tid == WLAN_TIDNO_BCAST) && (!MAC_GET_CB_IS_VIPFRAME(pst_tx_ctl))) {
        *puc_tid = WLAN_TIDNO_VOICE;
    }
}
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP

OAL_STATIC void hmac_hid2d_set_pktmark(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_buf,
    mac_tx_ctl_stru *pst_tx_ctl, uint8_t *puc_tid)
{
    if ((pst_hmac_vap->en_is_hid2d_state == OAL_TRUE) && (PKTMARK(pst_buf) == HID2D_FRAME_MARK)) {
        MAC_GET_CB_IS_HID2D_FRAME(pst_tx_ctl) = OAL_TRUE;
        return;
    }

    if (g_st_hid2d_debug_switch.uc_hid2d_debug_en == OAL_TRUE) {
        mac_ether_header_stru *_e = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);
        mac_ip_header_stru    *_ip = (mac_ip_header_stru *)(_e + 1);

        if (_e->us_ether_type == oal_host2net_short(ETHER_TYPE_IP) && _ip->uc_protocol == MAC_UDP_PROTOCAL) {
            MAC_GET_CB_IS_HID2D_FRAME(pst_tx_ctl) = OAL_TRUE;
            MAC_GET_CB_HID2D_TX_DELAY_TIME(pst_tx_ctl) = g_st_hid2d_debug_switch.uc_hid2d_delay_time;
            *puc_tid = WLAN_TIDNO_VIDEO;
        }
    }
}
#endif

void hmac_tx_classify(hmac_vap_stru *pst_hmac_vap, mac_user_stru *pst_user, oal_netbuf_stru *pst_buf)
{
    uint8_t uc_tid = 0;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    mac_device_stru *pst_mac_dev = OAL_PTR_NULL;

    /* ��qos��ͬ����Ҫ��EAPOL����ҵ��ʶ�� */
    hmac_tx_classify_lan_to_wlan(pst_buf, &uc_tid);

    /* ��QoSվ�㣬ֱ�ӷ��� */
    if (oal_unlikely(pst_user->st_cap_info.bit_qos != OAL_TRUE)) {
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{hmac_tx_classify::user isn't a QoS sta.}");
        return;
    }

    pst_mac_dev = mac_res_get_dev(pst_user->uc_device_id);
    if (oal_unlikely(pst_mac_dev == OAL_PTR_NULL)) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{hmac_tx_classify::pst_mac_dev null.}");
        return;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);

#if defined(_PRE_WLAN_FEATURE_WMMAC)
    if (g_en_wmmac_switch) {
        uint8_t uc_ac_num;
        uc_ac_num = WLAN_WME_TID_TO_AC(uc_tid);
        /* ���ACMλΪ1���Ҷ�ӦAC��TSû�н����ɹ����򽫸�AC������ȫ���ŵ�BE���з��� */
        if ((OAL_TRUE == mac_mib_get_QAPEDCATableMandatory(&(pst_hmac_vap->st_vap_base_info), uc_ac_num)) &&
            !MAC_GET_CB_IS_VIPFRAME(pst_tx_ctl) && (pst_user->st_ts_info[uc_ac_num].en_ts_status != MAC_TS_SUCCESS)) {
            uc_tid = WLAN_WME_AC_BE;
        }
    } else
#endif  //defined(_PRE_WLAN_FEATURE_WMMAC)
    {
        hmac_tx_wmm_acm(pst_mac_dev->en_wmm, pst_hmac_vap, &uc_tid);

        
        /* 1102��������ֻʹ����4��tid:0 1 5 6 */
        if ((!MAC_GET_CB_IS_VIPFRAME(pst_tx_ctl)) || (pst_mac_dev->en_wmm == OAL_FALSE)) {
            hmac_tx_update_tid(pst_mac_dev->en_wmm, &uc_tid);
        }
    }

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        /* �������Ϸ������ݣ���VO���� */
        hmac_hiex_judge_is_game_marked_enter_to_vo(pst_hmac_vap, pst_user, pst_buf, &uc_tid);
    }
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
        /* �����hid2d�������ұ����Ͷ��֡��������cb֡���� */
        hmac_hid2d_set_pktmark(pst_hmac_vap, pst_buf, pst_tx_ctl, &uc_tid);
#endif
    hmac_tx_classify_get_tid(pst_mac_dev, pst_hmac_vap, pst_tx_ctl, &uc_tid);

    /* ����ac��tid��cb�ֶ� */
    MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = uc_tid;
    MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_TID_TO_AC(uc_tid);

    return;
}


void hmac_rx_dyn_bypass_extlna_switch(uint32_t ul_tx_throughput_mbps, uint32_t ul_rx_throughput_mbps)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_limit_throughput_high;
    uint32_t ul_limit_throughput_low;
    uint32_t ul_throughput_mbps = oal_max(ul_tx_throughput_mbps, ul_rx_throughput_mbps);
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_is_pm_test;

    /* ������ƻ���֧�ָ�������bypass����LNA */
    if (g_st_rx_dyn_bypass_extlna_switch.uc_ini_en == OAL_FALSE) {
        return;
    }

    /* ÿ������������ */
    if ((g_st_rx_dyn_bypass_extlna_switch.us_throughput_high != 0) &&
        (g_st_rx_dyn_bypass_extlna_switch.us_throughput_low != 0)) {
        ul_limit_throughput_high = g_st_rx_dyn_bypass_extlna_switch.us_throughput_high;
        ul_limit_throughput_low = g_st_rx_dyn_bypass_extlna_switch.us_throughput_low;
    } else {
        ul_limit_throughput_high = WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_HIGH;
        ul_limit_throughput_low = WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_LOW;
    }
    if (ul_throughput_mbps > ul_limit_throughput_high) {
        /* ����100M,�ǵ͹��Ĳ��Գ��� */
        en_is_pm_test = OAL_FALSE;
    } else if (ul_throughput_mbps < ul_limit_throughput_low) {
        /* ����50M,�͹��Ĳ��Գ��� */
        en_is_pm_test = OAL_TRUE;
    } else {
        /* ����50M-100M֮��,�����л� */
        return;
    }

    /* ��Ҫ�л�ʱ������������֪ͨdevice���� */
    pst_mac_device = mac_res_get_dev(0);
    /* ����ǵ�VAP,�򲻴��� */
    if (1 != mac_device_calc_up_vap_num(pst_mac_device)) {
        return;
    }

    ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_mac_vap);
    if ((ul_ret != OAL_SUCC) || (pst_mac_vap == OAL_PTR_NULL)) {
        return;
    }

    /* ��ǰ��ʽ��ͬ,������ */
    if (g_st_rx_dyn_bypass_extlna_switch.uc_cur_status == en_is_pm_test) {
        return;
    }

    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_DYN_EXTLNA_BYPASS_SWITCH,
                                        OAL_SIZEOF (uint8_t), (uint8_t *)(&en_is_pm_test));
    if (ul_ret == OAL_SUCC) {
        g_st_rx_dyn_bypass_extlna_switch.uc_cur_status = en_is_pm_test;
    }

    oam_warning_log4(0, OAM_SF_ANY,
        "{hmac_rx_dyn_bypass_extlna_switch: limit_high:%d,limit_low:%d,throughput:%d,cur_status:%d(0:not pm, 1:pm))!}",
        ul_limit_throughput_high, ul_limit_throughput_low, ul_throughput_mbps, en_is_pm_test);
}

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU

OAL_STATIC void hmac_set_amsdu_num_based_protocol(mac_vap_stru *pst_mac_vap,
                                             wlan_tx_amsdu_enum_uint8 *pen_large_amsdu_ampdu,
                                             wlan_tx_amsdu_enum_uint8 en_tx_amsdu_level_type)
{
    /* Э��涨HT mpdu���Ȳ��ó���4095�ֽ� */
    if (oal_value_eq_any3(pst_mac_vap->en_protocol, WLAN_HT_ONLY_MODE, WLAN_HT_11G_MODE, WLAN_HT_MODE)) {
        *pen_large_amsdu_ampdu = WLAN_TX_AMSDU_BY_2;
    } else if ((pst_mac_vap->en_protocol == WLAN_VHT_MODE) ||
               (pst_mac_vap->en_protocol == WLAN_VHT_ONLY_MODE) ||
               (g_wlan_spec_cfg->feature_11ax_is_open && (pst_mac_vap->en_protocol == WLAN_HE_MODE))) {
        *pen_large_amsdu_ampdu = en_tx_amsdu_level_type;
    } else {
        *pen_large_amsdu_ampdu = WLAN_TX_AMSDU_NONE;
    }
}


void hmac_update_amsdu_num(mac_vap_stru *pst_mac_vap,
    uint32_t ul_tx_throughput_mbps, oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu)
{
    uint32_t ul_limit_throughput_high;
    uint32_t ul_limit_throughput_low;

    /* ÿ������������ */
    ul_limit_throughput_high = g_st_tx_large_amsdu.us_amsdu_throughput_high >> en_mu_vap_flag;
    ul_limit_throughput_low  = g_st_tx_large_amsdu.us_amsdu_throughput_low >> en_mu_vap_flag;

    if (ul_tx_throughput_mbps > ul_limit_throughput_high) {
        /* ���ڸ�����,�л�amsdu����ۺ� */
        *pen_tx_amsdu = WLAN_TX_AMSDU_BY_2;
    } else if (ul_tx_throughput_mbps < ul_limit_throughput_low) {
        /* ���ڵ�����,�ر�amsdu����ۺ� */
        *pen_tx_amsdu = WLAN_TX_AMSDU_NONE;
    } else {
        /* ���ڵ�����-������֮��,�����л� */
        *pen_tx_amsdu = g_st_tx_large_amsdu.en_tx_amsdu_level[pst_mac_vap->uc_vap_id];
    }
}

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
static void hmac_update_amsdu_num_vap_info(void)
{
    mac_device_stru *mac_device = mac_res_get_dev(0);
    uint32_t up_vap_num = mac_device_calc_up_vap_num(mac_device);
    uint8_t vap_idx;
    mac_vap_stru *mac_vap = OAL_PTR_NULL;

    oam_warning_log2(0, OAM_SF_ANY, "{hmac_update_amsdu_num_vap_info::up_vap_num[%d], dev vap cnt[%d]!}", up_vap_num,
        mac_device->uc_vap_num);

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(mac_device->auc_vap_id[vap_idx]);
        if (mac_vap == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_ANY, "{hmac_update_amsdu_num_vap_info::vap null! id[%d]}", vap_idx);
            continue;
        }

        oam_warning_log4(0, OAM_SF_ANY,
            "{hmac_update_amsdu_num_vap_info::up vap mode[%d], p2p mode[%d], vap state[%d], user_nums[%d]!}",
            mac_vap->en_vap_mode, mac_vap->en_p2p_mode, mac_vap->en_vap_state, mac_vap->us_user_nums);
    }
}
#endif

void hmac_update_amsdu_num_1105(mac_vap_stru *pst_mac_vap,
    uint32_t ul_tx_throughput_mbps, oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu)
{
    uint32_t ul_limit_throughput_high, ul_limit_throughput_middle, ul_limit_throughput_low;
    wlan_tx_amsdu_enum_uint8 en_max_amsdu;
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    hmac_vap_stru   *pst_hmac_vap;

    if (pst_mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_update_amsdu_num_1105::pst_mac_vap null!}");
        hmac_update_amsdu_num_vap_info(); // vap�쳣Ϊ�գ���ӡ����ǰdevice�ҽ�����vap״̬
        return;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap != OAL_PTR_NULL) {
        if (pst_hmac_vap->en_is_hid2d_state == OAL_TRUE) {
            *pen_tx_amsdu = WLAN_TX_AMSDU_NONE;
            return;
        }
    }
#endif
    /* ÿ������������ */
    ul_limit_throughput_high   = g_st_tx_large_amsdu.us_amsdu_throughput_high >> en_mu_vap_flag;
    ul_limit_throughput_middle = g_st_tx_large_amsdu.us_amsdu_throughput_middle >> en_mu_vap_flag;
    ul_limit_throughput_low    = g_st_tx_large_amsdu.us_amsdu_throughput_low >> en_mu_vap_flag;
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        /* �����1103��sdio���������⣺11ax���ޱ�11ac��100M */
        if (pst_mac_vap->en_protocol == WLAN_HE_MODE) {
            ul_limit_throughput_high -= WLAN_AMSDU_THROUGHPUT_TH_HE_VHT_DIFF;
        }
    }
#endif

    /* ��AMSDU, middle�����ǿ���amsdu����ʼ���� */
    if (ul_tx_throughput_mbps > ul_limit_throughput_middle) {
        if (ul_tx_throughput_mbps > ul_limit_throughput_high) {
            /* ���ڸ�����, ����4xmsdu */
            en_max_amsdu = WLAN_TX_AMSDU_BY_4;
        } else if (ul_tx_throughput_mbps > (ul_limit_throughput_middle << 1)) {
            /* ��ʼ���޵��������ϲ���С�ڸ�����, ����3xmsdu */
            en_max_amsdu = WLAN_TX_AMSDU_BY_3;
        } else {
            /* ������ʼ����,����2xmsdu */
            en_max_amsdu = WLAN_TX_AMSDU_BY_2;
        }

        /* �Ѿ����ǶԶ˵�AMSDU�ۺϳ��� */
        hmac_set_amsdu_num_based_protocol(pst_mac_vap, pen_tx_amsdu, en_max_amsdu);
    } else if (ul_tx_throughput_mbps > ul_limit_throughput_low) {
        /* ������-������, ���ϴ���AMSDU�ۺϵģ���ۺ�2��; ���򲻾ۺ� */
        if (g_st_tx_large_amsdu.en_tx_amsdu_level[pst_mac_vap->uc_vap_id] != WLAN_TX_AMSDU_NONE) {
            hmac_set_amsdu_num_based_protocol(pst_mac_vap, pen_tx_amsdu, WLAN_TX_AMSDU_BY_2);
        } else {
            *pen_tx_amsdu = WLAN_TX_AMSDU_NONE;
        }
    } else {
        /* ���ڵ�����, �ر�amsdu����ۺ� */
        *pen_tx_amsdu = WLAN_TX_AMSDU_NONE;
    }
}


OAL_STATIC uint32_t hmac_get_amsdu_judge_result(uint32_t ul_ret,
                                                  oal_bool_enum_uint8 en_mu_vap_flag,
                                                  mac_vap_stru *pst_vap1,
                                                  mac_vap_stru *pst_vap2)
{
    if ((ul_ret != OAL_SUCC) || (pst_vap1 == OAL_PTR_NULL) || (en_mu_vap_flag && (pst_vap2 == OAL_PTR_NULL))) {
        return OAL_FAIL;;
    }
    return OAL_SUCC;
}


void hmac_tx_amsdu_ampdu_switch(uint32_t ul_tx_throughput_mbps)
{
    mac_device_stru *pst_mac_device = mac_res_get_dev(0);
    mac_vap_stru *pst_vap[2] = {OAL_PTR_NULL}; // 2����2��vap
    wlan_tx_amsdu_enum_uint8 en_tx_amsdu = 0;
    uint32_t ul_up_ap_num = mac_device_calc_up_vap_num(pst_mac_device);
    oal_bool_enum_uint8 en_mu_vap = (ul_up_ap_num > 1);
    uint32_t ul_ret;
    uint8_t  uc_idx;
    hmac_vap_stru *hmac_vap = OAL_PTR_NULL;
    /* ������ƻ���֧��Ӳ���ۺ� */
    if (g_st_tx_large_amsdu.uc_host_large_amsdu_en == OAL_FALSE) {
        return;
    }

    if (en_mu_vap) {
        ul_ret = mac_device_find_2up_vap(pst_mac_device, &pst_vap[0], &pst_vap[1]);
    } else {
        ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_vap[0]);
    }

    if (hmac_get_amsdu_judge_result(ul_ret, en_mu_vap, pst_vap[0], pst_vap[1]) == OAL_FAIL) {
        return;
    }

    for (uc_idx = 0; uc_idx < ul_up_ap_num; uc_idx++) {
        wlan_chip_tx_update_amsdu_num(pst_vap[uc_idx], ul_tx_throughput_mbps, en_mu_vap, &en_tx_amsdu);
#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            en_tx_amsdu = oal_min(oal_max(en_tx_amsdu, pst_vap[uc_idx]->bit_ofdma_aggr_num),
                    WLAN_TX_AMSDU_BY_4);
        }
#endif

        /* ��ǰ�ۺϷ�ʽ��ͬ,������ */
        if (g_st_tx_large_amsdu.en_tx_amsdu_level[pst_vap[uc_idx]->uc_vap_id] == en_tx_amsdu) {
            continue;
        }

        g_st_tx_large_amsdu.en_tx_amsdu_level[pst_vap[uc_idx]->uc_vap_id] = en_tx_amsdu;
        hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_vap[uc_idx]->uc_vap_id);
        if ((hmac_vap != OAL_PTR_NULL) && (hmac_vap->d2h_amsdu_switch == OAL_FALSE)) {
            g_st_tx_large_amsdu.en_tx_amsdu_level[pst_vap[uc_idx]->uc_vap_id] = WLAN_TX_AMSDU_NONE;
        }

        oam_warning_log4(pst_vap[uc_idx]->uc_vap_id, OAM_SF_ANY,
            "{hmac_tx_amsdu_ampdu_switch: up vap num[%d], en_mu_vap[%d], amsdu level[%d],tx_throught= [%d]!}",
            ul_up_ap_num, en_mu_vap, en_tx_amsdu, ul_tx_throughput_mbps);
    }
}
#endif
#ifdef _PRE_WLAN_TCP_OPT

void hmac_tcp_ack_filter_switch(uint32_t ul_rx_throughput_mbps)
{
    mac_device_stru *pst_mac_device = mac_res_get_dev(0);
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    uint32_t ul_limit_throughput_high;
    uint32_t ul_limit_throughput_low;
    oal_bool_enum_uint8 en_tcp_ack_filter;
    oal_bool_enum_uint8 en_mu_vap_flag = (mac_device_calc_up_vap_num(pst_mac_device) > 1);
    /* ������ƻ���֧��tcp ack���˶�̬���� */
    if (g_st_tcp_ack_filter.uc_tcp_ack_filter_en == OAL_FALSE) {
        return;
    }

    /* ÿ������������ */
    if ((g_st_tcp_ack_filter.us_rx_filter_throughput_high != 0) &&
        (g_st_tcp_ack_filter.us_rx_filter_throughput_low != 0)) {
        ul_limit_throughput_high = g_st_tcp_ack_filter.us_rx_filter_throughput_high >> en_mu_vap_flag;
        ul_limit_throughput_low = g_st_tcp_ack_filter.us_rx_filter_throughput_low >> en_mu_vap_flag;
    } else {
        ul_limit_throughput_high = WLAN_TCP_ACK_FILTER_THROUGHPUT_TH_HIGH >> en_mu_vap_flag;
        ul_limit_throughput_low = WLAN_TCP_ACK_FILTER_THROUGHPUT_TH_LOW >> en_mu_vap_flag;
    }
    if (ul_rx_throughput_mbps > ul_limit_throughput_high) {
        /* ����60M, ��tcp ack���� */
        en_tcp_ack_filter = OAL_TRUE;
    } else if (ul_rx_throughput_mbps < ul_limit_throughput_low) {
        /* ����20M,�ر�tcp ack���� */
        en_tcp_ack_filter = OAL_FALSE;
    } else {
        /* ����20M-60M֮��,�����л� */
        return;
    }
    if (g_st_tcp_ack_filter.uc_cur_filter_status == en_tcp_ack_filter) {
        return;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return;
    }
    pst_hmac_device->sys_tcp_tx_ack_opt_enable = en_tcp_ack_filter;
    g_st_tcp_ack_filter.uc_cur_filter_status = en_tcp_ack_filter;

    oam_warning_log3(0, OAM_SF_ANY,
                     "{hmac_tcp_ack_filter_switch: limit_high = [%d],limit_low = [%d],rx_throught= [%d]!}",
                     ul_limit_throughput_high, ul_limit_throughput_low, ul_rx_throughput_mbps);
}
#endif

void hmac_tx_small_amsdu_switch(uint32_t ul_rx_throughput_mbps, uint32_t ul_tx_pps)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_cfg_ampdu_tx_on_param_stru st_ampdu_tx_on = { 0 };
    uint32_t ul_limit_throughput_high;
    uint32_t ul_limit_throughput_low;
    uint32_t ul_limit_pps_high;
    uint32_t ul_limit_pps_low;
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_small_amsdu;

    /* ������ƻ���֧��Ӳ���ۺ� */
    if (g_st_small_amsdu_switch.uc_ini_small_amsdu_en == OAL_FALSE) {
        return;
    }

    /* ÿ������������ */
    if ((g_st_small_amsdu_switch.us_small_amsdu_throughput_high != 0) &&
        (g_st_small_amsdu_switch.us_small_amsdu_throughput_low != 0)) {
        ul_limit_throughput_high = g_st_small_amsdu_switch.us_small_amsdu_throughput_high;
        ul_limit_throughput_low = g_st_small_amsdu_switch.us_small_amsdu_throughput_low;
    } else {
        ul_limit_throughput_high = WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_HIGH;
        ul_limit_throughput_low = WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_LOW;
    }
    /* ÿ��PPS���� */
    if ((g_st_small_amsdu_switch.us_small_amsdu_pps_high != 0) &&
        (g_st_small_amsdu_switch.us_small_amsdu_pps_low != 0)) {
        ul_limit_pps_high = g_st_small_amsdu_switch.us_small_amsdu_pps_high;
        ul_limit_pps_low = g_st_small_amsdu_switch.us_small_amsdu_pps_low;
    } else {
        ul_limit_pps_high = WLAN_SMALL_AMSDU_PPS_THRESHOLD_HIGH;
        ul_limit_pps_low = WLAN_SMALL_AMSDU_PPS_THRESHOLD_LOW;
    }

    if ((ul_rx_throughput_mbps > ul_limit_throughput_high) || (ul_tx_pps > ul_limit_pps_high)) {
        /* rx����������300M����tx pps����25000,��С��amsdu�ۺ� */
        en_small_amsdu = OAL_TRUE;
    } else if ((ul_rx_throughput_mbps < ul_limit_throughput_low) && (ul_tx_pps < ul_limit_pps_low)) {
        /* rx����������200M��tx ppsС��15000,�ر�С��amsdu�ۺϣ����������л� */
        en_small_amsdu = OAL_FALSE;
    } else {
        /* ����200M-300M֮��,�����л� */
        return;
    }

    /* ��ǰ�ۺϷ�ʽ��ͬ,������ */
    if (g_st_small_amsdu_switch.uc_cur_small_amsdu_en == en_small_amsdu) {
        return;
    }

    pst_mac_device = mac_res_get_dev(0);
    /* ����ǵ�VAP,���л� */
    if (1 != mac_device_calc_up_vap_num(pst_mac_device)) {
        return;
    }

    ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_mac_vap);
    if ((ul_ret != OAL_SUCC) || (pst_mac_vap == OAL_PTR_NULL)) {
        return;
    }

    oam_warning_log3(0, OAM_SF_ANY,
                     "{hmac_tx_small_amsdu_switch: limit_high = [%d],limit_low = [%d],rx_throught= [%d]!}",
                     ul_limit_throughput_high, ul_limit_throughput_low, ul_rx_throughput_mbps);
    oam_warning_log3(0, OAM_SF_ANY,
                     "{hmac_tx_small_amsdu_switch: PPS limit_high = [%d],PPS limit_low = [%d],tx_pps = %d!}",
                     ul_limit_pps_high, ul_limit_pps_low, ul_tx_pps);

    st_ampdu_tx_on.uc_aggr_tx_on = en_small_amsdu;

    g_st_small_amsdu_switch.uc_cur_small_amsdu_en = en_small_amsdu;

    hmac_config_set_amsdu_tx_on(pst_mac_vap,
                                    OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru),
                                    (uint8_t *)&st_ampdu_tx_on);
}

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER

void hmac_tx_tcp_ack_buf_switch(uint32_t ul_rx_throughput_mbps)
{
    mac_device_stru *pst_mac_device = mac_res_get_dev(0);
    mac_vap_stru *pst_mac_vap[2] = {OAL_PTR_NULL}; // 2����2��vap
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t ul_limit_throughput_high = 550;
    uint32_t ul_limit_throughput_low = 450;
    uint8_t  uc_vap_idx;
    uint8_t  uc_vap_num = mac_device_calc_up_vap_num(pst_mac_device);
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_mu_vap = (uc_vap_num > 1);

    /* ������ƻ����ز�֧���л���ֱ�ӷ��� */
    if (g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_en == OAL_FALSE) {
        return ;
    }

    if (en_mu_vap) {
        ul_ret = mac_device_find_2up_vap(pst_mac_device, &pst_mac_vap[0], &pst_mac_vap[1]);
    } else {
        ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_mac_vap[0]);
    }
    if ((ul_ret != OAL_SUCC) || (pst_mac_vap[0] == OAL_PTR_NULL)
        || (en_mu_vap && (pst_mac_vap[1] == OAL_PTR_NULL))) {
        return ;
    }

    g_st_tcp_ack_buf_switch.us_tcp_ack_smooth_throughput >>= 1;
    g_st_tcp_ack_buf_switch.us_tcp_ack_smooth_throughput += (ul_rx_throughput_mbps >> 1);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap[0]->uc_vap_id);
    if (!en_mu_vap && IS_LEGACY_STA(pst_mac_vap[0]) && pst_hmac_vap != OAL_PTR_NULL &&
        ((pst_hmac_vap->tcp_ack_buf_use_ctl_switch == OAL_SWITCH_ON) ||
        (g_st_tcp_ack_buf_switch.uc_ini_tcp_ack_buf_userctl_test_en == OAL_TRUE))) {
        /* �ϲ��´�ָ�� ָ������������ */
        ul_limit_throughput_high = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_high;
        ul_limit_throughput_low = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_userctl_low;
        hmac_tcp_ack_buf_switch_judge(pst_mac_vap[0], ul_limit_throughput_high,
                                      ul_limit_throughput_low, ul_rx_throughput_mbps, 0); // 0����ֻ��һ��vap
        return;
    }

    /* �ϲ�δ�´�ָ�����������޻�֧�ָù��� */
    for (uc_vap_idx = 0; uc_vap_idx < uc_vap_num; uc_vap_idx++) {
        if (pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth == WLAN_BAND_WIDTH_20M) {
            /* ����20M��ÿ������������ */
            hmac_tcp_ack_buf_set_throughput_threshould(WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH,
                                                       &ul_limit_throughput_high,
                                                       &ul_limit_throughput_low,
                                                       en_mu_vap);
        } else if ((pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40PLUS) ||
                   (pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth == WLAN_BAND_WIDTH_40MINUS)) {
            /* ����40M��ÿ������������ */
            hmac_tcp_ack_buf_set_throughput_threshould(WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M,
                                                       &ul_limit_throughput_high,
                                                       &ul_limit_throughput_low,
                                                       en_mu_vap);
        } else if ((pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS) &&
                   (pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS)) {
            /* ����80M��ÿ������������ */
            hmac_tcp_ack_buf_set_throughput_threshould(WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M,
                                                       &ul_limit_throughput_high,
                                                       &ul_limit_throughput_low,
                                                       en_mu_vap);
        }
#ifdef _PRE_WLAN_FEATURE_160M
        else if ((pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
                 (pst_mac_vap[uc_vap_idx]->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
            /* ����160M��ÿ������������ */
            hmac_tcp_ack_buf_set_throughput_threshould(WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_160M,
                                                       &ul_limit_throughput_high,
                                                       &ul_limit_throughput_low,
                                                       en_mu_vap);
        }
#endif
        hmac_tcp_ack_buf_switch_judge(pst_mac_vap[uc_vap_idx], ul_limit_throughput_high,
                                      ul_limit_throughput_low, ul_rx_throughput_mbps, uc_vap_idx);
    }
}


OAL_STATIC void hmac_tcp_ack_buf_set_throughput_threshould(uint32_t ul_high_throughput_level,
                                                           uint32_t *ul_limit_throughput_high,
                                                           uint32_t *ul_limit_throughput_low,
                                                           oal_bool_enum_uint8  en_mu_vap)
{
    switch (ul_high_throughput_level) {
        case WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH:
            *ul_limit_throughput_high = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH >> en_mu_vap;
            *ul_limit_throughput_low = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW >> en_mu_vap;
            if ((g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high != 0) &&
                (g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low != 0)) {
                *ul_limit_throughput_high = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high >> en_mu_vap;
                *ul_limit_throughput_low = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low >> en_mu_vap;
            }
            break;
        case WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M:
            *ul_limit_throughput_high = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M >> en_mu_vap;
            *ul_limit_throughput_low = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_40M >> en_mu_vap;
            if ((g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M != 0) &&
                (g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M != 0)) {
                *ul_limit_throughput_high = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_40M >> en_mu_vap;
                *ul_limit_throughput_low = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_40M >> en_mu_vap;
            }
            break;
        case WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M:
            *ul_limit_throughput_high = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M >> en_mu_vap;
            *ul_limit_throughput_low = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_80M >> en_mu_vap;
            if ((g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M != 0) &&
                (g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M != 0)) {
                *ul_limit_throughput_high = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_80M >> en_mu_vap;
                *ul_limit_throughput_low = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_80M >> en_mu_vap;
            }
            break;
#ifdef _PRE_WLAN_FEATURE_160M
        case WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_160M:
            *ul_limit_throughput_high = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_160M >> en_mu_vap;
            *ul_limit_throughput_low = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_160M >> en_mu_vap;
            if ((g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M != 0) &&
                (g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M != 0)) {
                *ul_limit_throughput_high = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_high_160M >> en_mu_vap;
                *ul_limit_throughput_low = g_st_tcp_ack_buf_switch.us_tcp_ack_buf_throughput_low_160M;
            }
            break;
#endif
        default:
            break;
    }
}

OAL_STATIC void hmac_tcp_ack_buf_switch_judge(mac_vap_stru *pst_mac_vap,
                                              uint32_t ul_limit_throughput_high,
                                              uint32_t ul_limit_throughput_low,
                                              uint32_t ul_rx_throughput_mbps,
                                              uint8_t  uc_vap_idx)
{
    mac_cfg_tcp_ack_buf_stru st_tcp_ack_param = { 0 };
    oal_bool_enum_uint8 en_tcp_ack_buf;

    if (g_st_tcp_ack_buf_switch.us_tcp_ack_smooth_throughput > ul_limit_throughput_high) {
        /* ����������,��tcp ack buf */
        en_tcp_ack_buf = OAL_TRUE;
    } else if (g_st_tcp_ack_buf_switch.us_tcp_ack_smooth_throughput < ul_limit_throughput_low) {
        /* ����������,�ر�tcp ack buf */
        en_tcp_ack_buf = OAL_FALSE;
    } else {
        /* ������������֮�䣬 �����л� */
        return;
    }

    /* δ�����仯 ,������ */
    if (g_st_tcp_ack_buf_switch.uc_cur_tcp_ack_buf_en[uc_vap_idx] == en_tcp_ack_buf) {
        return;
    }

    oam_warning_log4(0, OAM_SF_ANY,
        "{hmac_tx_tcp_ack_buf_switch: limit_high = [%d],limit_low = [%d],rx_throught= [%d]! en_tcp_ack_buf=%d}",
        ul_limit_throughput_high, ul_limit_throughput_low, ul_rx_throughput_mbps, en_tcp_ack_buf);

    g_st_tcp_ack_buf_switch.uc_cur_tcp_ack_buf_en[uc_vap_idx] = en_tcp_ack_buf;

    st_tcp_ack_param.en_cmd = MAC_TCP_ACK_BUF_ENABLE;
    st_tcp_ack_param.en_enable = en_tcp_ack_buf;

    hmac_config_tcp_ack_buf(pst_mac_vap, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru),
                            (uint8_t *)&st_tcp_ack_param);
}
#endif

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
OAL_STATIC uint32_t hmac_tx_ampdu_hw_switch(mac_vap_stru *pst_mac_vap,
    uint32_t ul_tx_throughput_mbps, uint32_t ul_limit_throughput_high,
    uint32_t ul_limit_throughput_low, oal_bool_enum_uint8 *pen_ampdu_hw)
{
    if ((ul_tx_throughput_mbps > ul_limit_throughput_high)
#ifdef _PRE_WLAN_FEATURE_11AX
        || (g_wlan_spec_cfg->feature_11ax_is_open &&
         (pst_mac_vap->bit_ofdma_aggr_num == MAC_VAP_AMPDU_HW) &&
         (ul_tx_throughput_mbps > (ul_limit_throughput_high >> 2))) /* 2�������޳���4 */
#endif
    ) {
        /* ����350M����UL OFDMA��������Ҫ�ۺ��ֽ����ر𳤲�����һ��������,�л�Ӳ���ۺ� */
        *pen_ampdu_hw = OAL_TRUE;
    } else if (ul_tx_throughput_mbps < ul_limit_throughput_low) {
        /* ����200M,�л�Ӳ���ۺ� */
        *pen_ampdu_hw = OAL_FALSE;
    } else {
        /* ����200M-300M֮��,�����л� */
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_tx_ampdu_close_hw_switch(mac_vap_stru *pst_mac_vap)
{
    if (oal_value_ne_all2(pst_mac_vap->en_protocol, WLAN_VHT_MODE, WLAN_VHT_ONLY_MODE)) {
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
#ifdef _PRE_WLAN_FEATURE_11AX
            if (pst_mac_vap->en_protocol != WLAN_HE_MODE) {
                return OAL_FAIL;
            }
#endif
        } else {
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}


OAL_STATIC void hmac_tx_ampdu_hw_cfg_send(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_ampdu_hw)
{
    mac_cfg_ampdu_tx_on_param_stru st_ampdu_tx_on = { 0 };

    if (en_ampdu_hw == OAL_TRUE) {
        st_ampdu_tx_on.uc_aggr_tx_on = 4;
    } else {
        st_ampdu_tx_on.uc_aggr_tx_on = 8;
    }

    st_ampdu_tx_on.uc_snd_type = OAL_TRUE;
#ifdef _PRE_WLAN_FEATURE_11AX
    if (pst_mac_vap->bit_ofdma_aggr_num == MAC_VAP_AMPDU_HW) {
        st_ampdu_tx_on.uc_snd_type = OAL_FALSE;
    }
#endif
    st_ampdu_tx_on.en_aggr_switch_mode = AMPDU_SWITCH_BY_BA_LUT;
    hmac_config_set_ampdu_tx_on(pst_mac_vap, OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru),
                                    (uint8_t *)&st_ampdu_tx_on);
}


void hmac_tx_ampdu_switch(uint32_t ul_tx_throughput_mbps)
{
    mac_device_stru *pst_mac_device;
    mac_vap_stru *pst_mac_vap;
    uint32_t ul_limit_throughput_high = WLAN_AMPDU_THROUGHPUT_THRESHOLD_HIGH;
    uint32_t ul_limit_throughput_low = WLAN_AMPDU_THROUGHPUT_THRESHOLD_LOW;
    uint32_t ul_ret;
    oal_bool_enum_uint8 en_ampdu_hw = OAL_TRUE;
    hmac_user_stru *pst_hmac_user;
    /* ������ƻ���֧��Ӳ���ۺ� */
    if (g_st_ampdu_hw.uc_ampdu_hw_en == OAL_FALSE) {
        return;
    }
    /* ÿ������������ */
    if ((g_st_ampdu_hw.us_throughput_high != 0) && (g_st_ampdu_hw.us_throughput_low != 0)) {
        ul_limit_throughput_high = g_st_ampdu_hw.us_throughput_high;
        ul_limit_throughput_low = g_st_ampdu_hw.us_throughput_low;
    }

    pst_mac_device = mac_res_get_dev(0);
    /* ����ǵ�VAP,�򲻿���Ӳ���ۺ� */
    if (1 != mac_device_calc_up_vap_num(pst_mac_device)) {
        en_ampdu_hw = OAL_FALSE;
    } else {
        ul_ret = mac_device_find_up_vap(pst_mac_device, &pst_mac_vap);
        if ((ul_ret != OAL_SUCC) || (pst_mac_vap == OAL_PTR_NULL)) {
            return;
        }
        /* Ƕ������Ż���װ */
        if (hmac_tx_ampdu_hw_switch(pst_mac_vap, ul_tx_throughput_mbps, ul_limit_throughput_high,
            ul_limit_throughput_low, &en_ampdu_hw) != OAL_SUCC) {
            return;
        }
    }

    /* ��ǰ�ۺϷ�ʽ��ͬ,������ */
    if (g_st_ampdu_hw.uc_ampdu_hw_enable == en_ampdu_hw) {
        return;
    }

    /* �л���Ӳ���ۺ�ʱ,��Ҫ�ж��Ƿ�����л����� */
    if (en_ampdu_hw == OAL_TRUE) {
        g_st_ampdu_hw.us_remain_hw_cnt = 0;

        /* VHT/HE��֧���л� */ /* Ƕ������Ż���װ */
        if (hmac_tx_ampdu_close_hw_switch(pst_mac_vap) != OAL_SUCC) {
            return;
        }

        /* ����:���鹤��Ƶ�ʵ���160Mhz(80M���´���)��֧���л� */
        if (WLAN_BAND_WIDTH_80PLUSPLUS > pst_mac_vap->st_channel.en_bandwidth) {
            return;
        }
        /* ��������AP������Ӳ���ۺ��л� */
        if (IS_LEGACY_STA(pst_mac_vap)) {
            pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
            if (pst_hmac_user == OAL_PTR_NULL) {
                oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_M2S, "hmac_tx_ampdu_switch: pst_hmac_user is null ptr.");
                return;
            }
            if (pst_hmac_user->en_user_ap_type & MAC_AP_TYPE_AGGR_BLACKLIST) {
                return;
            }
        }
    } else {
        /* ��������Ƶ���л�,��Ӳ���ۺ��л�����ۺ�,�������ܽ������л���ֵ,
           ����Ҫ��Ӳ���ۺ�����������һ��ʱ�� */
        /* �ǵ�vap,ֱ���л� */
        if (1 == mac_device_calc_up_vap_num(pst_mac_device)) {
            g_st_ampdu_hw.us_remain_hw_cnt++;
            if (g_st_ampdu_hw.us_remain_hw_cnt < WLAN_AMPDU_HW_SWITCH_PERIOD) {
                return;
            }
            g_st_ampdu_hw.us_remain_hw_cnt = 0;
        }
        pst_mac_vap = mac_res_get_mac_vap(0);
        if (pst_mac_vap == OAL_PTR_NULL) {
            return;
        }
    }

    g_st_ampdu_hw.uc_ampdu_hw_enable = en_ampdu_hw;

    oam_warning_log3(0, OAM_SF_ANY, "{hmac_tx_ampdu_switch: limit_high = [%d],limit_low = [%d],tx_throught= [%d]!}",
                     ul_limit_throughput_high, ul_limit_throughput_low, ul_tx_throughput_mbps);

    hmac_tx_ampdu_hw_cfg_send(pst_mac_vap, en_ampdu_hw);
}
#endif


uint32_t hmac_tx_filter_security(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_buf, hmac_user_stru *pst_hmac_user)
{
    mac_ether_header_stru *pst_ether_header = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_ret = OAL_SUCC;
    uint16_t us_ether_type;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    if (OAL_TRUE == mac_mib_get_rsnaactivated(pst_mac_vap)) { /* �ж��Ƿ�ʹ��WPA/WPA2 */
        if (pst_mac_user->en_port_valid != OAL_TRUE) { /* �ж϶˿��Ƿ�� */
            /* ��ȡ��̫��ͷ */
            pst_ether_header = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);
            /* ��������ʱ����Է�EAPOL ������֡������ */
            if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_PAE) != pst_ether_header->us_ether_type) {
                us_ether_type = oal_byteorder_host_to_net_uint16(pst_ether_header->us_ether_type);
                oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                                 "{hmac_tx_filter_security::TYPE 0x%04x, 0x%04x.}",
                                 us_ether_type, ETHER_TYPE_PAE);
                ul_ret = OAL_FAIL;
            }
        }
    }

    return ul_ret;
}


void hmac_tx_ba_setup(hmac_vap_stru *pst_hmac_vap,
                              hmac_user_stru *pst_user,
                              uint8_t uc_tidno)
{
    mac_action_mgmt_args_stru st_action_args; /* ������дACTION֡�Ĳ��� */

    /*
    ����BA�Ựʱ��st_action_args�ṹ������Ա��������
    (1)uc_category:action�����
    (2)uc_action:BA action�µ����
    (3)ul_arg1:BA�Ự��Ӧ��TID
    (4)ul_arg2:BUFFER SIZE��С
    (5)ul_arg3:BA�Ự��ȷ�ϲ���
    (6)ul_arg4:TIMEOUTʱ��
    */
    st_action_args.uc_category = MAC_ACTION_CATEGORY_BA;
    st_action_args.uc_action = MAC_BA_ACTION_ADDBA_REQ;
    st_action_args.ul_arg1 = uc_tidno; /* ������֡��Ӧ��TID�� */
    /* ADDBA_REQ�У�buffer_size��Ĭ�ϴ�С */
    st_action_args.ul_arg2 = (uint32_t)g_wlan_cust->ul_ampdu_tx_max_num;
    oam_warning_log1(0, OAM_SF_TX, "hmac_tx_ba_setup::[ba buffer size:%d]", st_action_args.ul_arg2);

    st_action_args.ul_arg3 = MAC_BA_POLICY_IMMEDIATE; /* BA�Ự��ȷ�ϲ��� */
    st_action_args.ul_arg4 = 0;                       /* BA�Ự�ĳ�ʱʱ������Ϊ0 */

    /* ����BA�Ự */
    hmac_mgmt_tx_action(pst_hmac_vap, pst_user, &st_action_args);
}


uint32_t hmac_tx_ucast_process(hmac_vap_stru *pst_hmac_vap,
                                     oal_netbuf_stru *pst_buf,
                                     hmac_user_stru *pst_user,
                                     mac_tx_ctl_stru *pst_tx_ctl)
{
    uint32_t ul_ret;
    /* ��ȫ���� */
    if (oal_unlikely(OAL_SUCC != hmac_tx_filter_security(pst_hmac_vap, pst_buf, pst_user))) {
        oam_stat_vap_incr(pst_hmac_vap->st_vap_base_info.uc_vap_id, tx_security_check_faild, 1);
        return HMAC_TX_DROP_SECURITY_FILTER;
    }

    /* ��̫��ҵ��ʶ�� */
    hmac_tx_classify(pst_hmac_vap, &(pst_user->st_user_base_info), pst_buf);

    if (hmac_get_tx_switch() == HOST_TX) {
        MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = WLAN_TIDNO_BCAST;
        MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_TID_TO_AC(WLAN_TIDNO_BCAST);
    }

    /* �����EAPOL��DHCP֡����������������BA�Ự */
    if (MAC_GET_CB_IS_VIPFRAME(pst_tx_ctl)) {
        return HMAC_TX_PASS;
    }

    if (hmac_tid_need_ba_session(pst_hmac_vap, pst_user, MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl), pst_buf)) {
        /* �Զ���������BA�Ự������AMPDU�ۺϲ�����Ϣ��DMACģ��Ĵ���addba rsp֡��ʱ�̺��� */
        hmac_tx_ba_setup(pst_hmac_vap, pst_user, MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl));
    }

    ul_ret = hmac_amsdu_notify(pst_hmac_vap, pst_user, pst_buf);
    if (oal_unlikely(ul_ret != HMAC_TX_PASS)) {
        return ul_ret;
    }
    return HMAC_TX_PASS;
}


uint32_t hmac_tx_need_frag(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl)
{
    uint32_t threshold;
    oal_bool_enum_uint8 need_frag = OAL_TRUE;
    /* �жϱ����Ƿ���Ҫ���з�Ƭ */
    /* 1�����ȴ�������          */
    /* 2����legacЭ��ģʽ       */
    /* 3�����ǹ㲥֡            */
    /* 4�����Ǿۺ�֡            */
    /* 6��DHCP֡�����з�Ƭ      */
    threshold = mac_mib_get_FragmentationThreshold(&hmac_vap->st_vap_base_info);
#ifdef _PRE_WLAN_FEATURE_HIEX
    if (hmac_user->st_ersru.bit_enable) {
        threshold = oal_min(threshold, hmac_user->st_ersru.bit_frag_len);
    }
#endif
    threshold = (threshold & (~(BIT0 | BIT1))) + 2;
    /* ��ǰ֡�����ڷ�Ƭ����,����Ƭ */
    if (threshold >= (oal_netbuf_len(netbuf) + MAC_GET_CB_FRAME_HEADER_LENGTH(tx_ctl))) {
        return OAL_FALSE;
    }

    /* ��LegacyЭ��/VIP֡/�㲥֡/����Ƭ,˽�ж�ͨ��Ƭ */
    if ((hmac_user->st_user_base_info.en_cur_protocol_mode >= WLAN_HT_MODE) ||
        (hmac_vap->st_vap_base_info.en_protocol >= WLAN_HT_MODE) ||
        (MAC_GET_CB_IS_VIPFRAME(tx_ctl) == OAL_TRUE) ||
        (MAC_GET_CB_IS_MCAST(tx_ctl) == OAL_TRUE)) {
        need_frag = OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_HIEX
        if (hmac_user->st_ersru.bit_enable) {
            need_frag = OAL_TRUE;
        }
#endif
    }

    /* �ۺϲ���Ƭ */
    if ((need_frag == OAL_TRUE) &&
        (MAC_GET_CB_IS_AMSDU(tx_ctl) == OAL_FALSE)
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
        && (MAC_GET_CB_AMSDU_LEVEL(tx_ctl) == WLAN_TX_AMSDU_NONE)
#endif
        && (OAL_FALSE == hmac_tid_ba_is_setup(hmac_user, MAC_GET_CB_WME_TID_TYPE(tx_ctl)))) {
        return threshold;
    }

    return 0;
}

#if defined _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tx_encap_wds_check(hmac_vap_stru *pst_vap,
    hmac_user_stru *pst_user)
{
    return ((pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_REPEATER_STA) ||
            ((pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_ROOTAP) && (pst_user->uc_is_wds == OAL_TRUE)));
}
#endif


uint32_t hmac_tx_encap_mac_hdr(hmac_vap_stru *hmac_vap,
    mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf, uint8_t **mac_hdr)
{
    uint32_t ret;
    uint32_t headroom_size = oal_netbuf_headroom(netbuf);

    /* �����HOST�������̣�MAC HEADER��������netbuf�� */
    if ((g_tx_switch.tx_switch == HOST_TX) &&
        (headroom_size < MAC_80211_QOS_HTC_4ADDR_FRAME_LEN)) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_tx_encap_mac_hdr::headroom[%d] too short}", headroom_size);
        ret = oal_netbuf_expand_head(netbuf, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN, 0, GFP_ATOMIC);
        if (ret != OAL_SUCC) {
            oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                "{hmac_tx_encap_mac_hdr::expand headroom failed}");
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
    }

    /* device��������dataָ��ǰԤ���Ŀռ����802.11 mac head len������Ҫ���������ڴ���802.11ͷ */
    if (headroom_size >= MAC_80211_QOS_HTC_4ADDR_FRAME_LEN) {
        *mac_hdr = (oal_netbuf_header(netbuf) - MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
        MAC_GET_CB_80211_MAC_HEAD_TYPE(tx_ctl) = 1; /* ָʾmacͷ����skb�� */
    } else {
        /* ��������80211ͷ */
        *mac_hdr = oal_mem_alloc_m(OAL_MEM_POOL_ID_SHARED_DATA_PKT, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN, OAL_FALSE);
        if (oal_unlikely(*mac_hdr == OAL_PTR_NULL)) {
            oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{hmac_tx_encap::pst_hdr null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        MAC_GET_CB_80211_MAC_HEAD_TYPE(tx_ctl) = 0; /* ָʾmacͷ������skb�У������˶����ڴ��ŵ� */
    }

    return OAL_SUCC;
}


/*lint -e695*/
uint32_t hmac_tx_encap(hmac_vap_stru *pst_vap, hmac_user_stru *pst_user, oal_netbuf_stru *pst_buf) /*lint !e695*/
{
    uint8_t *puc_80211_hdr = OAL_PTR_NULL; /* 802.11ͷ */
    uint32_t ul_qos = HMAC_TX_BSS_NOQOS;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret, ul_threshold;
    mac_ieee80211_frame_stru *pst_head;
    uint8_t uc_buf_is_amsdu;
    uint8_t uc_ic_header = 0;
    uint16_t us_mpdu_len = 0;
    mac_ether_header_stru st_ether_hdr;

    memset_s(&st_ether_hdr, sizeof(mac_ether_header_stru), 0, sizeof(mac_ether_header_stru));
    /* ��ȡCB */
    pst_tx_ctl = (mac_tx_ctl_stru *)(oal_netbuf_cb(pst_buf));
    uc_buf_is_amsdu = MAC_GET_CB_IS_AMSDU(pst_tx_ctl);

    /* ��ȡ��̫��ͷ, ԭ��ַ��Ŀ�ĵ�ַ, ��̫������ */
    if (EOK != memcpy_s(&st_ether_hdr, sizeof(st_ether_hdr), oal_netbuf_data(pst_buf), ETHER_HDR_LEN)) {
        oam_error_log0(0, OAM_SF_TX, "hmac_tx_encap::memcpy fail!");
    }

    /* ��amsdu֡ */
    if (uc_buf_is_amsdu == OAL_TRUE) {
        st_ether_hdr.us_ether_type = 0;
    } else {
        /* len = EHTER HEAD LEN + PAYLOAD LEN */
        us_mpdu_len = (uint16_t)oal_netbuf_get_len(pst_buf);

        /* ����frame���ȣ�ָ��skb payload--LLC HEAD */
        MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (us_mpdu_len - ETHER_HDR_LEN + SNAP_LLC_FRAME_LEN);

        MAC_GET_CB_ETHER_HEAD_PADDING(pst_tx_ctl) = 0;

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
        hmac_tx_encap_large_skb_amsdu(pst_vap, pst_user, pst_buf, pst_tx_ctl);

        if (MAC_GET_CB_AMSDU_LEVEL(pst_tx_ctl)) {
            /* �ָ�dataָ�뵽ETHER HEAD - LLC HEAD */
            oal_netbuf_pull(pst_buf, SNAP_LLC_FRAME_LEN);
        }
#endif
        /* ����LLC HEAD */
        mac_set_snap(pst_buf, st_ether_hdr.us_ether_type);

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
        if (MAC_GET_CB_AMSDU_LEVEL(pst_tx_ctl)) {
            /* �ָ�dataָ�뵽ETHER HEAD */
            oal_netbuf_push(pst_buf, ETHER_HDR_LEN);

            /* ��֤4bytes���� */
            if ((unsigned long)(uintptr_t)oal_netbuf_data(pst_buf) !=
                OAL_ROUND_DOWN((unsigned long)(uintptr_t)oal_netbuf_data(pst_buf), 4)) {
                MAC_GET_CB_ETHER_HEAD_PADDING(pst_tx_ctl) = (unsigned long)(uintptr_t)oal_netbuf_data(pst_buf) -
                    OAL_ROUND_DOWN ((unsigned long)(uintptr_t)oal_netbuf_data(pst_buf), 4);
                oal_netbuf_push(pst_buf, MAC_GET_CB_ETHER_HEAD_PADDING(pst_tx_ctl));
            }
        }
#endif
    }

    ul_ret = hmac_tx_encap_mac_hdr(pst_vap, pst_tx_ctl, pst_buf, &puc_80211_hdr);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        return ul_ret;
    }

    /* ���鲥֡����ȡ�û���QOS����λ��Ϣ */
    if (MAC_GET_CB_IS_MCAST(pst_tx_ctl) == OAL_FALSE) {
        /* �����û��ṹ���cap_info���ж��Ƿ���QOSվ�� */
        ul_qos = pst_user->st_user_base_info.st_cap_info.bit_qos;
        MAC_SET_CB_IS_QOS_DATA(pst_tx_ctl, ul_qos);
    }

    /* ����֡���� */
    hmac_tx_set_frame_ctrl(ul_qos, pst_tx_ctl, (mac_ieee80211_qos_htc_frame_addr4_stru *)puc_80211_hdr);
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_tx_set_frame_htc(pst_vap, ul_qos, pst_tx_ctl, pst_user,
                              (mac_ieee80211_qos_htc_frame_addr4_stru *)puc_80211_hdr);
    }
#endif
    /* ���õ�ַ */
    hmac_tx_set_addresses(pst_vap, pst_user, pst_tx_ctl, &st_ether_hdr,
                          (mac_ieee80211_qos_htc_frame_addr4_stru *)puc_80211_hdr);

    /* �ҽ�802.11ͷ */
    pst_head = (mac_ieee80211_frame_stru *)puc_80211_hdr;
    MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl) = pst_head;

    /* ��Ƭ���� */
    ul_threshold = hmac_tx_need_frag(pst_vap, pst_user, pst_buf, pst_tx_ctl);
    if (ul_threshold != 0) {
        /* TBD���ü��ܽӿ���ʹ��TKIPʱ��MSDU���м��ܺ��ڽ��з�Ƭ */
        ul_ret = hmac_en_mic(pst_vap, pst_user, pst_buf, &uc_ic_header);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_en_mic failed[%d].}", ul_ret);
            return ul_ret;
        }

        /* ���з�Ƭ���� */
        ul_ret = hmac_frag_process(pst_vap, pst_buf, pst_tx_ctl, (uint32_t)uc_ic_header, ul_threshold);
    }

#if ((defined (PRE_WLAN_FEATURE_SNIFFER)) && (defined (CONFIG_HW_SNIFFER)))
    proc_sniffer_write_file((const uint8_t *)pst_head, MAC_80211_QOS_FRAME_LEN,
                            (const uint8_t *)oal_netbuf_data(pst_buf), oal_netbuf_len(pst_buf), 1);
#endif
    return ul_ret;
}

/*lint +e695*/

OAL_STATIC uint32_t hmac_tx_lan_mpdu_process_sta(hmac_vap_stru *pst_vap,
                                                   oal_netbuf_stru *pst_buf,
                                                   mac_tx_ctl_stru *pst_tx_ctl)
{
    hmac_user_stru *pst_user = OAL_PTR_NULL; /* Ŀ��STA�ṹ�� */
    mac_ether_header_stru *pst_ether_hdr;    /* ��̫��ͷ */
    uint32_t ul_ret;
    uint16_t us_user_idx;
    uint8_t *puc_ether_payload = OAL_PTR_NULL;

    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);
    MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl) = pst_vap->st_vap_base_info.uc_vap_id;

    us_user_idx = pst_vap->st_vap_base_info.us_assoc_vap_id;

    pst_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (pst_user == OAL_PTR_NULL) {
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return HMAC_TX_DROP_USER_NULL;
    }
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    if (pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_REPEATER_STA) {
        hmac_wds_update_neigh(pst_vap, pst_ether_hdr->auc_ether_shost);
    } else if ((pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_NONE) &&
               (oal_byteorder_host_to_net_uint16(ETHER_TYPE_ARP) == pst_ether_hdr->us_ether_type))
#else
    if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_ARP) == pst_ether_hdr->us_ether_type)
#endif
    {
        pst_ether_hdr++;
        puc_ether_payload = (uint8_t *)pst_ether_hdr;
        /* The source MAC address is modified only if the packet is an   */
        /* ARP Request or a Response. The appropriate bytes are checked. */
        /* Type field (2 bytes): ARP Request (1) or an ARP Response (2)  */
        if ((puc_ether_payload[6] == 0x00) &&
            (puc_ether_payload[7] == 0x02 || puc_ether_payload[7] == 0x01)) {
            /* Set Address2 field in the WLAN Header with source address */
            oal_set_mac_addr(puc_ether_payload + 8, mac_mib_get_StationID(&pst_vap->st_vap_base_info));
        }
    }

    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_idx;

    ul_ret = hmac_tx_ucast_process(pst_vap, pst_buf, pst_user, pst_tx_ctl);
    if (oal_unlikely(ul_ret != HMAC_TX_PASS)) {
        return ul_ret;
    }

    /* ��װ802.11ͷ */
    ul_ret = hmac_tx_encap(pst_vap, pst_user, pst_buf);
    if (oal_unlikely((ul_ret != OAL_SUCC))) {
        oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                         "{hmac_tx_lan_mpdu_process_sta::hmac_tx_encap failed[%d].}", ul_ret);
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return HMAC_TX_DROP_80211_ENCAP_FAIL;
    }

    return HMAC_TX_PASS;
}


OAL_STATIC OAL_INLINE uint32_t hmac_tx_lan_mpdu_process_ap(hmac_vap_stru *pst_vap,
                                                             oal_netbuf_stru *pst_buf,
                                                             mac_tx_ctl_stru *pst_tx_ctl)
{
    hmac_user_stru *pst_user = OAL_PTR_NULL; /* Ŀ��STA�ṹ�� */
    mac_ether_header_stru *pst_ether_hdr;    /* ��̫��ͷ */
    uint8_t *puc_addr;                     /* Ŀ�ĵ�ַ */
    uint32_t ul_ret;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    uint8_t *src_addr; /* Դ��ַ */
#endif

    /* �ж����鲥�򵥲�,����lan to wlan�ĵ���֡��������̫����ַ */
    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_buf);
    puc_addr = pst_ether_hdr->auc_ether_dhost;
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    src_addr = pst_ether_hdr->auc_ether_shost;
#endif
    /* ��������֡ */
    if (oal_likely(!ether_is_multicast(puc_addr)))
    {
        ul_ret = mac_vap_find_user_by_macaddr(&(pst_vap->st_vap_base_info), puc_addr, &us_user_idx);
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
        if (ul_ret != OAL_SUCC) {
            ul_ret = hmac_find_valid_user_by_wds_sta(pst_vap, puc_addr, &us_user_idx);
        }
#endif

        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_warning_log4(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                             "{hmac_tx_lan_mpdu_process_ap::hmac_tx_find_user failed %2x:%2x:%2x:%2x}",
                             puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
            return HMAC_TX_DROP_USER_UNKNOWN;
        }

        /* ת��HMAC��USER�ṹ�� */
        pst_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
        if (oal_unlikely(pst_user == OAL_PTR_NULL)) {
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
            return HMAC_TX_DROP_USER_NULL;
        }

        /* �û�״̬�ж� */
        if (oal_unlikely(pst_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC)) {
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
            return HMAC_TX_DROP_USER_INACTIVE;
        }

        /* Ŀ��userָ�� */
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_idx;

        ul_ret = hmac_tx_ucast_process(pst_vap, pst_buf, pst_user, pst_tx_ctl);
        if (oal_unlikely(ul_ret != HMAC_TX_PASS)) {
            return ul_ret;
        }
    } else { /* �鲥 or �㲥 */
        /* �����鲥��ʶλ */
        MAC_GET_CB_IS_MCAST(pst_tx_ctl) = OAL_TRUE;

        /* ����ACK���� */
        MAC_GET_CB_ACK_POLACY(pst_tx_ctl) = WLAN_TX_NO_ACK;

        /* ��ȡ�鲥�û� */
        pst_user = mac_res_get_hmac_user(pst_vap->st_vap_base_info.us_multi_user_idx);

        if (oal_unlikely(pst_user == OAL_PTR_NULL)) {
            oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                             "{hmac_tx_lan_mpdu_process_ap::get multi user failed[%d].}",
                             pst_vap->st_vap_base_info.us_multi_user_idx);
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
            return HMAC_TX_DROP_MUSER_NULL;
        }

        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_vap->st_vap_base_info.us_multi_user_idx;
        MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = WLAN_TIDNO_BCAST;
        MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_TID_TO_AC(WLAN_TIDNO_BCAST);

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
        
        if ((pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_ROOTAP) && ether_is_broadcast(puc_addr)) {
            hmac_wds_node_ergodic(pst_vap, src_addr, hmac_wds_tx_broadcast_pkt, (void *)pst_buf);
        }
#endif
    }

    /* ��װ802.11ͷ */
    ul_ret = hmac_tx_encap(pst_vap, pst_user, pst_buf);
    if (oal_unlikely((ul_ret != OAL_SUCC))) {
        oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                         "{hmac_tx_lan_mpdu_process_ap::hmac_tx_encap failed[%d].}", ul_ret);
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return HMAC_TX_DROP_80211_ENCAP_FAIL;
    }

    return HMAC_TX_PASS;
}


void hmac_tx_vip_info(mac_vap_stru *pst_vap, uint8_t uc_data_type,
    oal_netbuf_stru *pst_buf, mac_tx_ctl_stru *pst_tx_ctl)
{
#ifndef WIN32
    mac_eapol_type_enum_uint8 en_eapol_type = MAC_EAPOL_PTK_BUTT;
    uint8_t uc_dhcp_type;
    mac_ieee80211_qos_htc_frame_addr4_stru *pst_mac_header = OAL_PTR_NULL;
    mac_llc_snap_stru *pst_llc = OAL_PTR_NULL;
    oal_ip_header_stru *pst_rx_ip_hdr = OAL_PTR_NULL;
    oal_eth_arphdr_stru *puc_arp_head = OAL_PTR_NULL;
    int32_t l_ret = EOK;

    uint8_t auc_ar_sip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */
    uint8_t auc_ar_dip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */

    /* ����skb�Ѿ���װ80211ͷ */
    /* ��ȡLLC SNAP */
    pst_llc = (mac_llc_snap_stru *)oal_netbuf_data(pst_buf);
    pst_mac_header = (mac_ieee80211_qos_htc_frame_addr4_stru *)MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl);
    if (pst_mac_header == OAL_PTR_NULL) {
        return;
    }
    if (uc_data_type == MAC_DATA_EAPOL) {
        en_eapol_type = mac_get_eapol_key_type((uint8_t *)(pst_llc + 1));
        oam_warning_log2(pst_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_tx_vip_info::EAPOL type=%u, len==%u}[1:1/4 2:2/4 3:3/4 4:4/4]",
                         en_eapol_type, oal_netbuf_len(pst_buf));
    } else if (uc_data_type == MAC_DATA_DHCP) {
        pst_rx_ip_hdr = (oal_ip_header_stru *)(pst_llc + 1);

        l_ret += memcpy_s((uint8_t *)auc_ar_sip, ETH_SENDER_IP_ADDR_LEN,
                          (uint8_t *)&pst_rx_ip_hdr->ul_saddr, OAL_SIZEOF(uint32_t));
        l_ret += memcpy_s((uint8_t *)auc_ar_dip, ETH_SENDER_IP_ADDR_LEN,
                          (uint8_t *)&pst_rx_ip_hdr->ul_daddr, OAL_SIZEOF(uint32_t));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_tx_vip_info::memcpy fail!");
            return;
        }

        uc_dhcp_type = mac_get_dhcp_frame_type(pst_rx_ip_hdr);
        hmac_ht_self_cure_event_set(pst_vap, pst_mac_header->auc_address1, HMAC_HT_SELF_CURE_EVENT_TX_DHCP_FRAME);
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_tx_vip_info::DHCP type=%d.[1:discovery 2:offer \
            3:request 4:decline 5:ack 6:nack 7:release 8:inform]", uc_dhcp_type);
        oam_warning_log4(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_tx_vip_info:: DHCP sip: %d.%d, dip: %d.%d.",
                         auc_ar_sip[2], auc_ar_sip[3], auc_ar_dip[2], auc_ar_dip[3]);
    } else {
        puc_arp_head = (oal_eth_arphdr_stru *)(pst_llc + 1);
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_tx_vip_info:: ARP type=%d.[2:arp resp 3:arp req.]",
                         uc_data_type);
        oam_warning_log4(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_tx_lan_mpdu_info:: ARP sip: %d.%d, dip: %d.%d",
                         puc_arp_head->auc_ar_sip[2], puc_arp_head->auc_ar_sip[3],
                         puc_arp_head->auc_ar_tip[2], puc_arp_head->auc_ar_tip[3]);
    }

    oam_warning_log4(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_tx_vip_info::send to wlan smac: %x:%x, dmac: %x:%x]",
        pst_mac_header->auc_address2[4], pst_mac_header->auc_address2[5],
        pst_mac_header->auc_address1[4], pst_mac_header->auc_address1[5]);
#endif
}
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP

OAL_STATIC oal_bool_enum_uint8 hmac_hid2d_need_update_seq_num(uint16_t us_cur_seq, uint16_t us_last_seq)
{
    uint16_t diff;
    if (us_cur_seq > us_last_seq) {
        diff = us_cur_seq - us_last_seq;
    } else {
        /* ��ת��Ĵ��� */
        diff = us_cur_seq + MAC_HID2D_MAX_SEQNUM - us_last_seq;
    }
    if (diff > MAC_HID2D_SEQNUM_AGE_CYCLE) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}


OAL_STATIC  void hmac_check_is_same_seq_num(hmac_vap_stru *pst_hmac_vap, uint16_t us_seq_num)
{
    uint8_t uc_index = 0;
    for (uc_index = 0; uc_index < MAC_HID2D_SEQNUM_CNT; uc_index++) {
        if (us_seq_num == pst_hmac_vap->aus_last_seqnum[uc_index]) {
            return;
        } else if (hmac_hid2d_need_update_seq_num(us_seq_num, pst_hmac_vap->aus_last_seqnum[uc_index])) {
            pst_hmac_vap->aus_last_seqnum[uc_index] = MAC_HID2D_INVALID_SEQNUM;
        }
    }
    /* ������������ϱ����к�, ��ౣ��3�� */
    uc_index = (pst_hmac_vap->uc_seq_idx) % MAC_HID2D_SEQNUM_CNT;
    pst_hmac_vap->aus_last_seqnum[uc_index] = us_seq_num;
    pst_hmac_vap->uc_seq_idx++;
    if (g_en_hid2d_debug) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_check_is_same_seq_num:    \
            last seq0[%d],last seq1[%d],last seq2[%d],seq_idx[%d]}", pst_hmac_vap->aus_last_seqnum[0],
            pst_hmac_vap->aus_last_seqnum[1], pst_hmac_vap->aus_last_seqnum[2], pst_hmac_vap->uc_seq_idx); /* 2:ƫ�� */
    }
    /* �ϱ� */
    hmac_hid2d_drop_report(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(uint16_t),(uint8_t*)&us_seq_num);
}

OAL_STATIC uint32_t hmac_hid2d_tx_drop_based_time(hmac_vap_stru *pst_hmac_vap,
    mac_tx_ctl_stru *pst_tx_ctl, hid2d_video_hdr_stru *pst_hid2d_hdr, uint16_t us_remain_time)
{
    uint32_t ul_ret = OAL_SUCC;
    uint16_t    us_seq_num;
     /* ���ַ�ת(1����������ת��2����ʱ)��������deviceʱ������500us������ */
    if ((us_remain_time > WLAN_HID2D_100MS_CNT) || (us_remain_time < WLAN_HID2D_1500US_CNT)) {
        ul_ret = HMAC_TX_HID2D_DROP;
        MAC_GET_CB_HID2D_TX_DELAY_TIME(pst_tx_ctl) = 0;  /* ����device����ʱ��Ϊ0 */
        /* �ϱ����к� */
        us_seq_num = (uint16_t)(pst_hid2d_hdr->bit_nalu_num << 4) | (uint16_t)(pst_hid2d_hdr->bit_spn); /* 4:֡��ʽ */
        hmac_check_is_same_seq_num(pst_hmac_vap, us_seq_num);
    } else {
        /* ��������device�ķ���ʱ�䣬us_remain_time��λΪ32us������5λ��ȡms��64��ʾdevice���Ԥ��ʱ�� */
        if ((us_remain_time >> 5) < 64) {
            /* ����5λ��ʾȡms */
            MAC_GET_CB_HID2D_TX_DELAY_TIME(pst_tx_ctl) = ((us_remain_time >> 5) == 0) ? 1 : (us_remain_time >> 5);
        } else {
            MAC_GET_CB_HID2D_TX_DELAY_TIME(pst_tx_ctl) = 63; /* 63Ϊdevice���Ԥ��ʱ�� */
        }
    }
    return ul_ret;
}

OAL_STATIC uint32_t hmac_hid2d_check_tx_drop(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_buf,
    mac_tx_ctl_stru *pst_tx_ctl)
{
    mac_llc_snap_stru *pst_llc = OAL_PTR_NULL;
    mac_ip_header_stru *pst_ip = OAL_PTR_NULL;
    udp_hdr_stru *pst_udp_hdr = OAL_PTR_NULL;
    uint8_t    *puc_prefix_udp = NULL;
    uint8_t     uc_prefix_len;
    hid2d_video_hdr_stru *pst_hid2d_hdr = NULL;
    uint16_t    us_deadline;
    uint16_t    us_cur_time = 0;    /* ��λΪ32us */
    uint16_t    us_remain_time; /* Ԥ����оƬ�����ʱ�䣬��λΪ32us */
    uint32_t    ul_ret = OAL_SUCC;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct timeval tv;
#endif
    /* debugģʽ��hmac������ */
    if (g_st_hid2d_debug_switch.uc_hid2d_debug_en == OAL_TRUE) {
        return OAL_SUCC;
    }
    if (pst_hmac_vap->en_is_hid2d_state == OAL_FALSE || MAC_GET_CB_IS_HID2D_FRAME(pst_tx_ctl) == OAL_FALSE) {
        return OAL_SUCC;
    }
    /* ����deadline */
    /* ��ȡ��̫��ͷ */
    pst_llc = (mac_llc_snap_stru *)oal_netbuf_data(pst_buf);
    if (pst_llc->us_ether_type == oal_host2net_short(ETHER_TYPE_IP)) {
        pst_ip = (mac_ip_header_stru *)(pst_llc + 1);
         if (pst_ip->uc_protocol == MAC_UDP_PROTOCAL) {
            pst_udp_hdr = (udp_hdr_stru *)(pst_ip + 1);
            puc_prefix_udp = (uint8_t*)(pst_udp_hdr + 1);
            uc_prefix_len = *puc_prefix_udp;    /* ���prefix udp���ȣ�payloadǰ8bit       */
            pst_hid2d_hdr = (hid2d_video_hdr_stru *)(puc_prefix_udp + uc_prefix_len);

            /* ��ȡdeadline */
            us_deadline = pst_hid2d_hdr->bit_deadline;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            do_gettimeofday(&tv);
            us_cur_time = tv.tv_usec >> 5;  /* ��λΪ32us */
#endif
            /* �ж��Ƿ񳬹�����ʱ�� */
            if (us_deadline >= us_cur_time) {
                us_remain_time = us_deadline - us_cur_time;
            } else {
                us_remain_time = WLAN_HID2D_ONE_SEC_CNT - (us_cur_time - us_deadline);
            }
            if (g_en_hid2d_debug) {
                oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_hid2d_check_tx_drop:    \
                    prefix_len[%d],naluNum[%d],spn[%d],deadline[%d]}", uc_prefix_len, pst_hid2d_hdr->bit_nalu_num,
                    pst_hid2d_hdr->bit_spn, pst_hid2d_hdr->bit_deadline);
                oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_hid2d_check_tx_drop:    \
                    deadline[%d],cur_time[%d],remain_device_time[%d]}", us_deadline, us_cur_time, us_remain_time);
            }
            ul_ret = hmac_hid2d_tx_drop_based_time(pst_hmac_vap, pst_tx_ctl, pst_hid2d_hdr, us_remain_time);
        }
    }
    return ul_ret;
}
#endif

OAL_STATIC void hmac_set_not_qos_tid_type_ap(hmac_vap_stru *pst_hmac_vap, mac_tx_ctl_stru *pst_tx_ctl)
{
    if (mac_mib_get_dot11QosOptionImplemented(&pst_hmac_vap->st_vap_base_info) == OAL_FALSE) {
        MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_AC_VO; /* APģʽ ��WMM ��VO���� */
        MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = WLAN_WME_AC_TO_TID(MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl));
    }
}

OAL_STATIC void hmac_tx_cb_field_init(mac_tx_ctl_stru *pst_tx_ctl, mac_vap_stru *pst_vap, uint8_t uc_data_type)
{
    MAC_GET_CB_MPDU_NUM(pst_tx_ctl) = 1;
    MAC_GET_CB_NETBUF_NUM(pst_tx_ctl) = 1;
    MAC_GET_CB_WLAN_FRAME_TYPE(pst_tx_ctl) = WLAN_DATA_BASICTYPE;
    MAC_GET_CB_ACK_POLACY(pst_tx_ctl) = WLAN_TX_NORMAL_ACK;
    MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl) = pst_vap->uc_vap_id;
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
    MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_AC_BE; /* ��ʼ����BE���� */
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = uc_data_type;
}


/*lint -e695*/
uint32_t hmac_tx_lan_to_wlan_no_tcp_opt(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf)
{
    frw_event_stru *pst_event = OAL_PTR_NULL; /* �¼��ṹ�� */
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap;                /* VAP�ṹ�� */
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL; /* SKB CB */
    uint32_t ul_ret = HMAC_TX_PASS;
    dmac_tx_event_stru *pst_dtx_stru = OAL_PTR_NULL;
    uint8_t uc_data_type;

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    uint32_t  ul_hid2d_ret = OAL_SUCC;
#endif
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_wapi_stru *pst_wapi;
    mac_ieee80211_frame_stru *pst_mac_hdr;
    oal_bool_enum_uint8 en_is_mcast = OAL_FALSE;
#endif

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_vap->uc_vap_id);

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_vap->uc_vap_id, OAM_SF_TX, "{hmac_tx_lan_to_wlan_no_tcp_opt::pst_hmac_vap null.}");
        oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* VAPģʽ�ж� */
    if (oal_unlikely(pst_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP && pst_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX, "{no_tcp_opt::en_vap_mode=%d.}", pst_vap->en_vap_mode);
        oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /* ��������û�����Ϊ0���������� */
    if (oal_unlikely(pst_hmac_vap->st_vap_base_info.us_user_nums == 0)) {
        oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return OAL_FAIL;
    }

    /* �˴����ݿ��ܴ��ں˶�����Ҳ�п�����dev��������ͨ���տ�ת��ȥ��ע��һ�� */
    uc_data_type = mac_get_data_type_from_8023((uint8_t *)oal_netbuf_data(pst_buf), MAC_NETBUFF_PAYLOAD_ETH);
    /* ��ʼ��CB tx rx�ֶ� , CB�ֶ���ǰ���Ѿ������㣬 �����ﲻ��Ҫ�ظ���ĳЩ�ֶθ���ֵ */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    hmac_tx_cb_field_init(pst_tx_ctl, pst_vap, uc_data_type);

#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG
    hmac_parse_special_ipv4_packet(oal_netbuf_data(pst_buf), oal_netbuf_get_len(pst_buf), HMAC_PKT_DIRECTION_TX);
#endif

    /* ����LAN TO WLAN��WLAN TO WLAN��netbuf�������������Ϊ�����֣���Ҫ���ж�
       ��������������netbufȻ���ٶ�CB���¼������ֶθ�ֵ */
    if (MAC_GET_CB_EVENT_TYPE(pst_tx_ctl) != FRW_EVENT_TYPE_WLAN_DTX) {
        MAC_GET_CB_EVENT_TYPE(pst_tx_ctl) = FRW_EVENT_TYPE_HOST_DRX;
        MAC_GET_CB_EVENT_SUBTYPE(pst_tx_ctl) = DMAC_TX_HOST_DRX;
    }

    oal_spin_lock_bh(&pst_hmac_vap->st_lock_state);
    /* ����host�ඪ������ڴ���ͳ�� */
    HMAC_TX_PKTS_STAT(1);
    hmac_auto_freq_wifi_tx_bytes_stat(oal_netbuf_len(pst_buf));

    /* �����ݰ�����ͳ�� */
    hmac_auto_freq_wifi_tx_stat(1);

    if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        /*  ����ǰ MPDU */
        hmac_set_not_qos_tid_type_ap(pst_hmac_vap, pst_tx_ctl);
        ul_ret = hmac_tx_lan_mpdu_process_ap(pst_hmac_vap, pst_buf, pst_tx_ctl);
    } else if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        /* ����ǰMPDU */
        MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl) = WLAN_WME_AC_VO; /* STAģʽ ��qos֡��VO���� */
        MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = WLAN_WME_AC_TO_TID(MAC_GET_CB_WME_AC_TYPE(pst_tx_ctl));

        ul_ret = hmac_tx_lan_mpdu_process_sta(pst_hmac_vap, pst_buf, pst_tx_ctl);
#ifdef _PRE_WLAN_FEATURE_WAPI
        if (ul_ret == HMAC_TX_PASS) {
            // && oal_unlikely(WAPI_IS_WORK(pst_hmac_vap)))
            /* ��ȡwapi���� �鲥/���� */
            pst_mac_hdr = MAC_GET_CB_FRAME_HEADER_ADDR((mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf));
            en_is_mcast = ether_is_multicast(pst_mac_hdr->auc_address1);
            /*lint -e730*/
            pst_wapi = hmac_user_get_wapi_ptr(pst_vap, !en_is_mcast, pst_vap->us_assoc_vap_id);
            if (pst_wapi == OAL_PTR_NULL) {
                oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
                oam_warning_log1(0, OAM_SF_ANY,
                    "no_tcp_opt::get_wapi_ptr fail! us_assoc_id[%u]}", pst_vap->us_assoc_vap_id);
                oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);
                return OAL_ERR_CODE_PTR_NULL;
            }

            /*lint +e730*/
            if ((OAL_TRUE == WAPI_PORT_FLAG(pst_wapi)) &&
                (pst_wapi->wapi_netbuff_txhandle != OAL_PTR_NULL)) {
                pst_buf = pst_wapi->wapi_netbuff_txhandle(pst_wapi, pst_buf);
                if (pst_buf == OAL_PTR_NULL) {
                    oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
                    oam_warning_log0(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac wapi_netbuff_txhandle fail!}");
                    oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);
                    return OAL_ERR_CODE_PTR_NULL;
                }
                /*  ����wapi�����޸�netbuff���˴���Ҫ���»�ȡһ��cb */
                pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
            }
        }

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */
    }

    oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);

    if (oal_likely(ul_ret == HMAC_TX_PASS)) {
        /* ά�⣬���һ���ؼ�֡��ӡ */
        if (uc_data_type <= MAC_DATA_ARP_REQ) {
            hmac_tx_vip_info(pst_vap, uc_data_type, pst_buf, pst_tx_ctl);
        }

#ifdef _PRE_WLAN_PKT_TIME_STAT
        if (DELAY_STATISTIC_SWITCH_ON && IS_NEED_RECORD_DELAY(pst_buf, TP_SKB_HMAC_XMIT)) {
            skbprobe_record_time(pst_buf, TP_SKB_HMAC_TX);
        }
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
        ul_hid2d_ret = hmac_hid2d_check_tx_drop(pst_hmac_vap, pst_buf, pst_tx_ctl);
        /* �ж���Ҫ���� */
        if (ul_hid2d_ret != OAL_SUCC) {
            return ul_hid2d_ret;
        }
#endif
        /* ���¼�������DMAC */
        pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
        if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
            oam_error_log0(pst_vap->uc_vap_id, OAM_SF_TX, "{hmac_tx_lan_to_wlan::frw_event_alloc_m failed.}");
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }

        pst_event = frw_get_event_stru(pst_event_mem);

        /* ��д�¼�ͷ */
        frw_event_hdr_init(&(pst_event->st_event_hdr),
                           FRW_EVENT_TYPE_HOST_DRX,
                           DMAC_TX_HOST_DRX,
                           OAL_SIZEOF(dmac_tx_event_stru),
                           FRW_EVENT_PIPELINE_STAGE_1,
                           pst_vap->uc_chip_id,
                           pst_vap->uc_device_id,
                           pst_vap->uc_vap_id);

        pst_dtx_stru = (dmac_tx_event_stru *)pst_event->auc_event_data;
        pst_dtx_stru->pst_netbuf = pst_buf;
        pst_dtx_stru->us_frame_len = MAC_GET_CB_MPDU_LEN(pst_tx_ctl);

        /* �����¼� */
        ul_ret = frw_event_dispatch_event(pst_event_mem);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX,
                             "{hmac_tx_lan_to_wlan::frw_event_dispatch_event failed[%d].}", ul_ret);
            oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
        }

        /* �ͷ��¼� */
        frw_event_free_m(pst_event_mem);

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
        /* RR���ܼ�ⷢ������hmac to dmac λ�ô�� */
        hmac_rr_tx_h2d_timestamp();
#endif
    } else if (oal_unlikely(ul_ret == HMAC_TX_BUFF)) {
        ul_ret = OAL_SUCC;
    } else if (ul_ret == HMAC_TX_DONE) {
        ul_ret = OAL_SUCC;
    } else {
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX, "{::HMAC_TX_DROP.reason[%d]!}", ul_ret);
    }

    return ul_ret;
}
/*lint +e695*/
#ifdef _PRE_WLAN_TCP_OPT
OAL_STATIC uint32_t hmac_transfer_tx_handler(hmac_device_stru *hmac_device,
                                               hmac_vap_stru *hmac_vap,
                                               oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *pst_tx_ctl = NULL;
    uint32_t ul_ret = OAL_SUCC;

    if (WLAN_TCP_ACK_QUEUE == oal_netbuf_select_queue(netbuf)) {
#ifdef _PRE_WLAN_TCP_OPT_DEBUG
        oam_warning_log0(0, OAM_SF_TX,
                         "{hmac_transfer_tx_handler::netbuf is tcp ack.}\r\n");
#endif
        oal_spin_lock_bh(&hmac_vap->st_hamc_tcp_ack[HCC_TX].data_queue_lock[HMAC_TCP_ACK_QUEUE]);
        oal_netbuf_list_tail(&hmac_vap->st_hamc_tcp_ack[HCC_TX].data_queue[HMAC_TCP_ACK_QUEUE], netbuf);

        /* ����TCP ACK�ȴ�����, ���ⱨ�����Ϸ��� */
        if (hmac_judge_tx_netbuf_is_tcp_ack((oal_ether_header_stru *)oal_netbuf_data(netbuf))) {
            pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
            MAC_GET_CB_TCP_ACK(pst_tx_ctl) = OAL_TRUE;
            oal_spin_unlock_bh(&hmac_vap->st_hamc_tcp_ack[HCC_TX].data_queue_lock[HMAC_TCP_ACK_QUEUE]);
            hmac_sched_transfer();
        } else {
            oal_spin_unlock_bh(&hmac_vap->st_hamc_tcp_ack[HCC_TX].data_queue_lock[HMAC_TCP_ACK_QUEUE]);
            hmac_tcp_ack_process();
        }
    } else {
        ul_ret = hmac_tx_lan_to_wlan_no_tcp_opt(&(hmac_vap->st_vap_base_info), netbuf);
    }
    return ul_ret;
}
#endif


uint32_t hmac_tx_wlan_to_wlan_ap(oal_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL; /* �¼��ṹ�� */
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_netbuf_stru *pst_buf = OAL_PTR_NULL;     /* ��netbuf����ȡ������ָ��netbuf��ָ�� */
    oal_netbuf_stru *pst_buf_tmp = OAL_PTR_NULL; /* �ݴ�netbufָ�룬����whileѭ�� */
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;
    /* ����ж� */
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_wlan_to_wlan_ap::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ��ȡ�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);
    if (oal_unlikely(pst_event == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_wlan_to_wlan_ap::pst_event null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡPAYLOAD�е�netbuf�� */
    pst_buf = (oal_netbuf_stru *)(uintptr_t)(*((unsigned long *)(pst_event->auc_event_data)));

    ul_ret = hmac_tx_get_mac_vap(pst_event->st_event_hdr.uc_vap_id, &pst_mac_vap);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(pst_event->st_event_hdr.uc_vap_id, OAM_SF_TX,
                       "{hmac_tx_wlan_to_wlan_ap::hmac_tx_get_mac_vap failed[%d].}", ul_ret);
        hmac_free_netbuf_list(pst_buf);
        return ul_ret;
    }

    /* ѭ������ÿһ��netbuf��������̫��֡�ķ�ʽ���� */
    while (pst_buf != OAL_PTR_NULL) {
        pst_buf_tmp = oal_netbuf_next(pst_buf);

        oal_netbuf_next(pst_buf) = OAL_PTR_NULL;
        oal_netbuf_prev(pst_buf) = OAL_PTR_NULL;

        
        pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
        memset_s(pst_tx_ctl, sizeof(mac_tx_ctl_stru), 0, sizeof(mac_tx_ctl_stru));

        MAC_GET_CB_EVENT_TYPE(pst_tx_ctl) = FRW_EVENT_TYPE_WLAN_DTX;
        MAC_GET_CB_EVENT_SUBTYPE(pst_tx_ctl) = DMAC_TX_WLAN_DTX;

        /* set the queue map id when wlan to wlan */
        oal_skb_set_queue_mapping(pst_buf, WLAN_NORMAL_QUEUE);

        ul_ret = hmac_tx_lan_to_wlan(pst_mac_vap, pst_buf);

        /* ����ʧ�ܣ��Լ������Լ��ͷ�netbuff�ڴ� */
        if (ul_ret != OAL_SUCC) {
            hmac_free_netbuf_list(pst_buf);
        }

        pst_buf = pst_buf_tmp;
    }

    return OAL_SUCC;
}


void hmac_set_tx_switch(int32_t tx_switch_ini_param)
{
    g_tx_switch.tx_switch = tx_switch_ini_param & BIT0;
    g_tx_switch.tx_update_mode = tx_switch_ini_param & BIT1;

    oam_warning_log2(0, OAM_SF_CFG, "{hmac_set_tx_switch::tx switch[%d], update mode[%d]}",
        g_tx_switch.tx_switch, g_tx_switch.tx_update_mode);
}


oal_bool_enum_uint8 hmac_is_host_tx(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf)
{
    mac_ether_header_stru *eth_hdr = NULL;

    if (hmac_get_tx_switch() != HOST_TX) {
        /* ������get data typeӰ��03/05 */
        return OAL_FALSE;
    }

    eth_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);

    return mac_get_data_type_from_8023((uint8_t *)eth_hdr, MAC_NETBUFF_PAYLOAD_ETH) > MAC_DATA_VIP_FRAME &&
           !ether_is_multicast(eth_hdr->auc_ether_dhost);
}


/*lint -e695*/
uint32_t hmac_tx_lan_to_wlan(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf)
{
    uint32_t ret = HMAC_TX_PASS;
#ifdef _PRE_WLAN_TCP_OPT
    hmac_device_stru *hmac_device = NULL;
    hmac_vap_stru *hmac_vap = NULL; /* VAP�ṹ�� */
#endif

#ifdef CONFIG_ARCH_KIRIN_PCIE
    if (hmac_is_host_tx(mac_vap, netbuf)) {
        return hmac_host_tx(mac_vap, netbuf);
    }
#endif
    /* csumӲ����host��������֧�֣�device������Ҫ���������� */
    if (netbuf->ip_summed == CHECKSUM_PARTIAL) {
        oal_skb_checksum_help(netbuf);
    }
#ifdef _PRE_WLAN_TCP_OPT
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_lan_to_wlan_tcp_opt::pst_dmac_vap null.}\r\n");
        return OAL_FAIL;
    }
    hmac_device = hmac_res_get_mac_dev(mac_vap->uc_device_id);
    if (oal_unlikely(hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                       "{hmac_tx_lan_to_wlan_tcp_opt::pst_hmac_device null.}\r\n");
        return OAL_FAIL;
    }
    if (hmac_device->sys_tcp_tx_ack_opt_enable == OAL_TRUE) {
        ret = hmac_transfer_tx_handler(hmac_device, hmac_vap, netbuf);
    } else
#endif
    {
        ret = hmac_tx_lan_to_wlan_no_tcp_opt(mac_vap, netbuf);
    }
    return ret;
}
/*lint +e695*/

oal_net_dev_tx_enum hmac_bridge_vap_xmit(oal_netbuf_stru *pst_buf, oal_net_device_stru *pst_dev)
{
    mac_vap_stru *pst_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_data_type;
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    hmac_user_stru *pst_hmac_user;
#endif

    hmac_device_stru *pst_hmac_device = NULL;
    oal_bool_enum_uint8 en_drop_frame = OAL_FALSE;

    if (oal_unlikely(pst_buf == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_bridge_vap_xmit::pst_buf = OAL_PTR_NULL!}\r\n");
        return OAL_NETDEV_TX_OK;
    }

#ifdef _PRE_SKB_TRACE
    mem_trace_add_node((uintptr_t)pst_buf);
#endif

    if (oal_unlikely(pst_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_bridge_vap_xmit::pst_dev = OAL_PTR_NULL!}\r\n");
        oal_netbuf_free(pst_buf);
        oam_stat_vap_incr(0, tx_abnormal_msdu_dropped, 1);
        return OAL_NETDEV_TX_OK;
    }

    /* ��ȡVAP�ṹ�� */
    pst_vap = (mac_vap_stru *)oal_net_dev_priv(pst_dev);

    /* ���VAP�ṹ�岻���ڣ��������� */
    if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
        /* will find vap fail when receive a pkt from
         * kernel while vap is deleted, return OAL_NETDEV_TX_OK is so. */
        oam_warning_log0(0, OAM_SF_TX, "{hmac_bridge_vap_xmit::pst_vap = OAL_PTR_NULL!}\r\n");
        oal_netbuf_free(pst_buf);
        oam_stat_vap_incr(0, tx_abnormal_msdu_dropped, 1);
        return OAL_NETDEV_TX_OK;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_vap->uc_vap_id, OAM_SF_CFG, "{hmac_bridge_vap_xmit::pst_hmac_vap null.}");
        oal_netbuf_free(pst_buf);
        return OAL_NETDEV_TX_OK;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_vap->uc_vap_id, OAM_SF_PROXYSTA, "{hmac_bridge_vap_xmit::pst_hmac_device is null!}");
        oal_netbuf_free(pst_buf);

        return OAL_NETDEV_TX_OK;
    }

    /* ��������ģ�pst_device_struָ���л�δ״̬, ���������л�δ����״̬ */
    if (pst_vap->bit_al_tx_flag == OAL_SWITCH_ON) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_TX, "{hmac_bridge_vap_xmit::the vap alway tx/rx!}\r\n");
        oal_netbuf_free(pst_buf);
        return OAL_NETDEV_TX_OK;
    }

    pst_buf = oal_netbuf_unshare(pst_buf, GFP_ATOMIC);
    if (oal_unlikely(pst_buf == OAL_PTR_NULL)) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_TX,
                      "{hmac_bridge_vap_xmit::the unshare netbuf = OAL_PTR_NULL!}\r\n");
        return OAL_NETDEV_TX_OK;
    }
#ifdef _PRE_WLAN_PKT_TIME_STAT
    if (DELAY_STATISTIC_SWITCH_ON && IS_NEED_RECORD_DELAY(pst_buf, TP_SKB_IP)) {
        skbprobe_record_time(pst_buf, TP_SKB_HMAC_XMIT);
    }
#endif

    /* ����̫��������֡�ϱ�SDT */
    hmac_tx_report_eth_frame(pst_vap, pst_buf);

    /* ����VAP״̬������滥�⣬��Ҫ�������� */
    oal_spin_lock_bh(&pst_hmac_vap->st_lock_state);

    /* �ж�VAP��״̬�����ROAM���������� MAC_DATA_DHCP/MAC_DATA_ARP */
    if (pst_vap->en_vap_state == MAC_VAP_STATE_ROAMING) {
        uc_data_type = mac_get_data_type_from_8023((uint8_t *)oal_netbuf_payload(pst_buf),
                                                       MAC_NETBUFF_PAYLOAD_ETH);
        if (uc_data_type != MAC_DATA_EAPOL) {
            oal_netbuf_free(pst_buf);
            oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);
            return OAL_NETDEV_TX_OK;
        }
    } else {
        /* �ж�VAP��״̬�����û��UP/PAUSE����������. */
        if (oal_unlikely(!((pst_vap->en_vap_state == MAC_VAP_STATE_UP) ||
                           (pst_vap->en_vap_state == MAC_VAP_STATE_PAUSE)))) {
            /* ���������д���p2pɨ�費��dhcp��eapol֡����ֹ����ʧ�� */
            if (pst_vap->en_vap_state != MAC_VAP_STATE_STA_LISTEN) {
                en_drop_frame = OAL_TRUE;
            } else {
                uc_data_type = mac_get_data_type_from_8023((uint8_t *)oal_netbuf_payload(pst_buf),
                                                               MAC_NETBUFF_PAYLOAD_ETH);
                if ((uc_data_type != MAC_DATA_EAPOL) && (uc_data_type != MAC_DATA_DHCP)) {
                    en_drop_frame = OAL_TRUE;
                } else {
                    oam_warning_log2(pst_vap->uc_vap_id, OAM_SF_TX,
                                     "{hmac_bridge_vap_xmit::donot drop [%d]frame[EAPOL:1,DHCP:0]. vap state[%d].}",
                                     uc_data_type, pst_vap->en_vap_state);
                }
            }

            if (en_drop_frame == OAL_TRUE) {
                /* filter the tx xmit pkts print */
                if (pst_vap->en_vap_state == MAC_VAP_STATE_INIT || pst_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
                    oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX,
                                     "{hmac_bridge_vap_xmit::vap state[%d] != MAC_VAP_STATE_{UP|PAUSE}!}\r\n",
                                     pst_vap->en_vap_state);
                } else {
                    oam_info_log1(pst_vap->uc_vap_id, OAM_SF_TX,
                                  "{hmac_bridge_vap_xmit::vap state[%d] != MAC_VAP_STATE_{UP|PAUSE}!}\r\n",
                                  pst_vap->en_vap_state);
                }
                oal_netbuf_free(pst_buf);
                oam_stat_vap_incr(pst_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);

                oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);
                return OAL_NETDEV_TX_OK;
            }
        }
    }

    oal_netbuf_next(pst_buf) = OAL_PTR_NULL;
    oal_netbuf_prev(pst_buf) = OAL_PTR_NULL;

#ifdef _PRE_WLAN_PKT_TIME_STAT
    memset_s(oal_netbuf_cb(pst_buf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
    memset_s(oal_netbuf_cb(pst_buf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif

    if (IS_STA(&(pst_hmac_vap->st_vap_base_info))) {
        /* ���ͷ����arp_req ͳ�ƺ��ع����Ĵ��� */
        hma_arp_probe_timer_start(pst_buf, pst_hmac_vap);
    }

    oal_spin_unlock_bh(&pst_hmac_vap->st_lock_state);

    ul_ret = hmac_tx_lan_to_wlan(pst_vap, pst_buf);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        /* ����ʧ�ܣ�Ҫ�ͷ��ں������netbuff�ڴ�� */
        oal_netbuf_free(pst_buf);
    }

    return OAL_NETDEV_TX_OK;
}

/* ���ڲ����������� */
uint8_t g_uc_tx_ba_policy_select = OAL_TRUE;


void hmac_tx_ba_cnt_vary(hmac_vap_stru *pst_hmac_vap,
                                 hmac_user_stru *pst_hmac_user,
                                 uint8_t uc_tidno,
                                 oal_netbuf_stru *pst_buf)
{
    uint32_t ul_current_timestamp;

    if ((pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (g_uc_tx_ba_policy_select == OAL_TRUE)) {
        
        pst_hmac_user->auc_ba_flag[uc_tidno]++;
    } else {
        ul_current_timestamp = (uint32_t)oal_time_get_stamp_ms();

        /* ��һ����ֱ�Ӽ�����
           ��ʱ����������ʱ����ʼ����BA;
           TCP ACK�ظ�����������ʱ�����ơ� */
        if ((pst_hmac_user->auc_ba_flag[uc_tidno] == 0) ||
            (oal_netbuf_is_tcp_ack((oal_ip_header_stru *)(oal_netbuf_data(pst_buf) + ETHER_HDR_LEN))) ||
            ((uint32_t)oal_time_get_runtime(pst_hmac_user->aul_last_timestamp[uc_tidno], ul_current_timestamp) <
             WLAN_BA_CNT_INTERVAL)) {
            pst_hmac_user->auc_ba_flag[uc_tidno]++;
        } else {
            pst_hmac_user->auc_ba_flag[uc_tidno] = 0;
        }

        pst_hmac_user->aul_last_timestamp[uc_tidno] = ul_current_timestamp;
    }
}

/*lint -e19*/
oal_module_symbol(hmac_tx_wlan_to_wlan_ap);
oal_module_symbol(hmac_tx_lan_to_wlan);
oal_module_symbol(hmac_free_netbuf_list);

oal_module_symbol(hmac_tx_report_eth_frame);
oal_module_symbol(hmac_bridge_vap_xmit); /*lint +e19*/
