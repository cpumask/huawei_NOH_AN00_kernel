

#include "hmac_host_tx_data.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_data.h"
#include "mac_data.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HOST_TX_DATA_C

#ifdef CONFIG_ARCH_KIRIN_PCIE

#define MAC_DATA_TYPE_INVALID_MIN_VALUE        1535 /* �Ƿ���̫���������͵���С�ж�ֵ��0x0600~0xFFFFΪType */
#define MAC_DATA_DOUBLE_VLAN_MIN_LEN           22   /* ˫vlan��С���ĳ��� */
#define MAC_DATA_DOUBLE_VLAN_ETHER_TYPE_OFFSET (ETHER_ADDR_LEN * 2 + OAL_SIZEOF(mac_vlan_tag_stru) * 2)
#define HMAC_TX_CNT_LEVEL_NUM                  5

uint32_t g_tx_update_freq_level[HMAC_TX_CNT_LEVEL_NUM] = { 1, 3, 6, 12, 20 };
uint32_t g_tx_cnt_level[HMAC_TX_CNT_LEVEL_NUM] = { 0, 200, 1000, 2000, 5000 };


OAL_STATIC uint32_t hmac_host_ring_tx(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    hmac_tid_info_stru *hmac_tid_info = &hmac_user->tx_tid_info[MAC_GET_CB_WME_TID_TYPE(tx_ctl)];
    hmac_msdu_info_ring_stru *tx_msdu_ring = &hmac_tid_info->non_ba_tx_ring;
    oal_netbuf_stru *next_buf = NULL;
    uint32_t ret = 0;

    if (tx_msdu_ring->host_ring_buf == NULL || tx_msdu_ring->netbuf_list == NULL) {
        oam_error_log2(0, OAM_SF_TX, "{hmac_host_ring_tx::ptr NULL host_ring_buf[0x%x] netbuf_list[0x%x]}",
            (uintptr_t)tx_msdu_ring->host_ring_buf, (uintptr_t)tx_msdu_ring->netbuf_list);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* MAC���Ƭ�߼�ʹ�� */
    while (netbuf != NULL) {
        next_buf = oal_netbuf_next(netbuf);
        ret = hmac_tx_ring_push_msdu(mac_vap, hmac_user, hmac_tid_info, tx_msdu_ring, netbuf);
        if (ret != OAL_SUCC) {
            oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_TX, "hmac_host_ring_tx: tx ring push fail");
            oal_netbuf_free(netbuf);
        }

        netbuf = next_buf;
    }

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ether_with_double_vlan_tag(oal_netbuf_stru *netbuf, uint16_t ether_type)
{
    return oal_netbuf_len(netbuf) >= MAC_DATA_DOUBLE_VLAN_MIN_LEN &&
           (ether_type == oal_host2net_short(ETHER_TYPE_VLAN_88A8) ||
           ether_type == oal_host2net_short(ETHER_TYPE_VLAN_9100) ||
           ether_type == oal_host2net_short(ETHER_TYPE_VLAN));
}


OAL_STATIC OAL_INLINE uint16_t hmac_ether_get_second_vlan_type(uint8_t *vlan_ether_hdr)
{
    return *(uint16_t *)(vlan_ether_hdr + MAC_DATA_DOUBLE_VLAN_ETHER_TYPE_OFFSET);
}


OAL_STATIC void hmac_ether_set_vlan_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    oal_vlan_ethhdr_stru *vlan_ether_hdr = (oal_vlan_ethhdr_stru *)oal_netbuf_data(netbuf);
    uint16_t ether_type = vlan_ether_hdr->h_vlan_encapsulated_proto;

    if (hmac_ether_with_double_vlan_tag(netbuf, ether_type)) {
        /* ˫vlan��̫������ */
        MAC_GET_CB_DATA_TYPE(tx_ctl) = DATA_TYPE_2_VLAN_ETH;

        /* ȡ��2��vlan�����2�ֽ�����802.3�����ж�, DA|SA|VLAN|VLAN|TYPE(LEN)|DATA */
        ether_type = hmac_ether_get_second_vlan_type((uint8_t *)vlan_ether_hdr);
    } else {
        /* ��vlan��̫�����ͱ��� */
        MAC_GET_CB_DATA_TYPE(tx_ctl) = DATA_TYPE_1_VLAN_ETH;
    }

    /* �ж��Ƿ���802.3���� */
    MAC_GET_CB_IS_802_3_SNAP(tx_ctl) = MAC_DATA_TYPE_INVALID_MIN_VALUE >= oal_net2host_short(ether_type);
}


OAL_STATIC OAL_INLINE void hmac_ether_set_other_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    uint16_t ether_type = ((mac_ether_header_stru *)oal_netbuf_data(netbuf))->us_ether_type;

    MAC_GET_CB_IS_802_3_SNAP(tx_ctl) = MAC_DATA_TYPE_INVALID_MIN_VALUE >= oal_net2host_short(ether_type);
}


OAL_STATIC void hmac_ether_set_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf, uint8_t data_type)
{
    switch (data_type) {
        case MAC_DATA_VLAN:
            hmac_ether_set_vlan_data_type(tx_ctl, netbuf);
            break;
        case MAC_DATA_BUTT:
            hmac_ether_set_other_data_type(tx_ctl, netbuf);
            break;
        default:
            break;
    }
}


OAL_STATIC void hmac_tx_init_cb(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf, uint16_t user_id)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    mac_ether_header_stru *ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    uint8_t data_type = mac_get_data_type_from_8023((uint8_t *)oal_netbuf_data(netbuf), MAC_NETBUFF_PAYLOAD_ETH);

    MAC_GET_CB_MPDU_NUM(tx_ctl) = 1;
    MAC_GET_CB_NETBUF_NUM(tx_ctl) = 1;
    MAC_GET_CB_WLAN_FRAME_TYPE(tx_ctl) = WLAN_DATA_BASICTYPE;
    MAC_GET_CB_ACK_POLACY(tx_ctl) = WLAN_TX_NORMAL_ACK;
    MAC_GET_CB_TX_VAP_INDEX(tx_ctl) = mac_vap->uc_vap_id;
    MAC_GET_CB_TX_USER_IDX(tx_ctl) = user_id;
    MAC_GET_CB_WME_AC_TYPE(tx_ctl) = WLAN_WME_AC_VO;
    MAC_GET_CB_WME_TID_TYPE(tx_ctl) = WLAN_WME_AC_TO_TID(WLAN_WME_AC_VO);
    MAC_GET_CB_FRAME_TYPE(tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
    MAC_GET_CB_FRAME_SUBTYPE(tx_ctl) = data_type;
    MAC_GET_CB_DATA_TYPE(tx_ctl) = DATA_TYPE_ETH;
    MAC_GET_CB_IS_MCAST(tx_ctl) = ether_is_multicast(ether_hdr->auc_ether_dhost);

    /* csum�ϲ���д��skb cb��, �������̸��ݸ��ֶ���д */
    hmac_ether_set_data_type(tx_ctl, netbuf, data_type);
}


OAL_STATIC OAL_INLINE void hmac_tx_set_arp_src_mac_addr(hmac_vap_stru *hmac_vap, mac_ether_header_stru *ether_hdr)
{
    uint8_t *puc_ether_payload = (uint8_t *)(++ether_hdr);

    if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_ARP) != ether_hdr->us_ether_type)  {
        return;
    }

    /* The source MAC address is modified only if the packet is an
     * ARP Request or a Response. The appropriate bytes are checked.
     * Type field (2 bytes): ARP Request (1) or an ARP Response (2) */
    if (puc_ether_payload[6] == 0x00 && (puc_ether_payload[7] == 0x02 || puc_ether_payload[7] == 0x01)) {
        /* Set Address2 field in the WLAN Header with source address */
        oal_set_mac_addr(puc_ether_payload + 8, mac_mib_get_StationID(&hmac_vap->st_vap_base_info));
    }
}


OAL_STATIC oal_void hmac_host_tx_mpdu_netbuf_move(oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    uint32_t mac_hdr_len;
    uint32_t ret;

    while (netbuf) {
        tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        mac_hdr_len = MAC_GET_CB_FRAME_HEADER_LENGTH(tx_ctl);
        MAC_GET_CB_DATA_TYPE(tx_ctl) = DATA_TYPE_80211;
        /* MAC header��snapͷ���ִ��ڿն�����Ҫ����mac header����λ�� */
        ret = memmove_s(oal_netbuf_data(netbuf) - mac_hdr_len, mac_hdr_len,
            MAC_GET_CB_FRAME_HEADER_ADDR(tx_ctl), mac_hdr_len);
        if (oal_unlikely(ret != OAL_SUCC)) {
            oam_error_log0(0, 0, "hmac_host_tx_mpdu_netbuf_adjust:memmove fail");
        }
        oal_netbuf_push(netbuf, mac_hdr_len);
        /* ��¼�µ�MACͷ��λ�� */
        MAC_GET_CB_FRAME_HEADER_ADDR(tx_ctl) = (mac_ieee80211_frame_stru *)oal_netbuf_data(netbuf);
        /* ��Ƭ�������ڶ��netbuf */
        netbuf = oal_netbuf_next(netbuf);
    }
}


OAL_STATIC uint32_t hmac_host_tx_preprocess(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);

    hmac_tx_set_arp_src_mac_addr(hmac_vap, (mac_ether_header_stru *)oal_netbuf_data(netbuf));

    if (hmac_tx_filter_security(hmac_vap, netbuf, hmac_user) != OAL_SUCC) {
        return HMAC_TX_DROP_SECURITY_FILTER;
    }

    hmac_tx_classify(hmac_vap, &hmac_user->st_user_base_info, netbuf);

    if (MAC_GET_CB_WME_TID_TYPE(tx_ctl) == WLAN_TIDNO_BCAST) {
        MAC_GET_CB_WME_TID_TYPE(tx_ctl) = WLAN_TIDNO_VOICE;
        MAC_GET_CB_WME_AC_TYPE(tx_ctl) = WLAN_WME_TID_TO_AC(WLAN_TIDNO_VOICE);
    }

    if (!hmac_get_tx_ring_enable(hmac_user, MAC_GET_CB_WME_TID_TYPE(tx_ctl))) {
        return HMAC_TX_DROP_USER_NULL;
    }

    if (hmac_tid_need_ba_session(hmac_vap, hmac_user, MAC_GET_CB_WME_TID_TYPE(tx_ctl), netbuf)) {
        hmac_tx_ba_setup(hmac_vap, hmac_user, MAC_GET_CB_WME_TID_TYPE(tx_ctl));
    }

    if (!hmac_tx_need_frag(hmac_vap, hmac_user, netbuf, tx_ctl)) {
        /*
         * host txģʽ��ֻ�з�Ƭʱ��Ҫ��װ802.11֡ͷ, �������Ӳ������з�װ
         * Ҳ��ͨ������msdu info�е�data type = 3���������װ, ���Host�������ĵ�
         */
        return HMAC_TX_PASS;
    }

    /* MAC���Ƭ��CSUMӲ�����ܲ����棬�˳�������������CSUMֵ */
    if (netbuf->ip_summed == CHECKSUM_PARTIAL) {
        oal_skb_checksum_help(netbuf);
        netbuf->ip_summed = CHECKSUM_COMPLETE;
    }

    /* ��Ƭ�����������MACͷ */
    if (hmac_tx_encap(hmac_vap, hmac_user, netbuf) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_TX, "{hmac_host_tx_preprocess_sta::hmac_tx_encap failed}");
        return HMAC_TX_DROP_80211_ENCAP_FAIL;
    }

    hmac_host_tx_mpdu_netbuf_move(netbuf);
    return HMAC_TX_PASS;
}


OAL_STATIC uint16_t hmac_host_tx_get_user_id(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf)
{
    uint16_t user_idx;
    uint8_t *dest_addr = NULL;

    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        user_idx = hmac_vap->st_vap_base_info.us_assoc_vap_id;
    } else {
        dest_addr = ((mac_ether_header_stru *)oal_netbuf_data(netbuf))->auc_ether_dhost;
        if (mac_vap_find_user_by_macaddr(&hmac_vap->st_vap_base_info, dest_addr, &user_idx) != OAL_SUCC) {
            return WLAN_ASSOC_USER_MAX_NUM;
        }
    }

    return user_idx;
}


OAL_STATIC OAL_INLINE uint32_t hmac_host_tx_netbuf_allowed(mac_vap_stru *mac_vap, hmac_vap_stru *hmac_vap)
{
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP && mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    if (mac_vap->us_user_nums == 0) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint32_t hmac_host_tx(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf)
{
    uint32_t ret;
    uint16_t user_idx;
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);

    ret = hmac_host_tx_netbuf_allowed(mac_vap, hmac_vap);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_TX, "{hmac_tx_lan_to_wlan_no_tcp_opt::tx netbuf not allowed[%d]}", ret);
        return ret;
    }

    user_idx = hmac_host_tx_get_user_id(hmac_vap, netbuf);
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_tx_init_cb(mac_vap, netbuf, user_idx);

    switch (hmac_host_tx_preprocess(hmac_vap, hmac_user, netbuf)) {
        case HMAC_TX_PASS:
            return hmac_host_ring_tx(mac_vap, hmac_user, netbuf);
        case HMAC_TX_BUFF:
        case HMAC_TX_DONE:
            return OAL_SUCC;
        default:
            return OAL_FAIL;
    }
}


OAL_STATIC void hmac_get_update_freq_by_tx_cnt(hmac_tid_info_stru *tx_tid_info)
{
    uint32_t level;

    for (level = 0; level < HMAC_TX_CNT_LEVEL_NUM - 1; level++) {
        if (tx_tid_info->last_period_tx_msdus >= g_tx_cnt_level[level] &&
            tx_tid_info->last_period_tx_msdus <= g_tx_cnt_level[level + 1]) {
            tx_tid_info->tx_update_freq = g_tx_update_freq_level[level];
            return;
        }
    }

    tx_tid_info->tx_update_freq = g_tx_update_freq_level[level];
}


uint32_t hmac_tx_update_freq_timeout(void *arg)
{
    uint32_t tid;
    hmac_tid_info_stru *tx_tid_info = NULL;
    hmac_user_stru *hmac_user = (hmac_user_stru *)arg;

    /* TID7�������̵���, �������Ƶ�� */
    for (tid = 0; tid < WLAN_TIDNO_BUTT - 1; tid++) {
        tx_tid_info = &hmac_user->tx_tid_info[tid];
        hmac_get_update_freq_by_tx_cnt(tx_tid_info);
        tx_tid_info->last_period_tx_msdus = 0;
    }

    return OAL_SUCC;
}

#endif
