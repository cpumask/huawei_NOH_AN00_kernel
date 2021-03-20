
#include "hmac_frag.h"
#include "hmac_11i.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_FRAG_C

#define HMAC_FRAG_TIMEOUT  2000
#define HMAC_MAX_FRAG_SIZE 2500
#define HMAC_FRAG_HEADER_RESV_LEN 32


OAL_STATIC mac_ieee80211_frame_stru *hmac_frag_init_frag_netbuf(oal_netbuf_stru *pst_netbuf_original,
    mac_tx_ctl_stru *pst_tx_ctl, uint32_t ul_offset, oal_netbuf_stru *pst_netbuf, uint32_t ul_payload)
{
    mac_tx_ctl_stru *pst_tx_ctl_copy = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_mac_header;
    uint32_t ul_mac_hdr_size;
    mac_ieee80211_frame_stru *pst_frag_header = OAL_PTR_NULL;

    ul_mac_hdr_size = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl);
    pst_mac_header = MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl);
    pst_mac_header->st_frame_control.bit_more_frag = OAL_TRUE;

    pst_tx_ctl_copy = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    /* ����cb�ֶ� */
    if (EOK != memcpy_s(pst_tx_ctl_copy, MAC_TX_CTL_SIZE, pst_tx_ctl, MAC_TX_CTL_SIZE)) {
        return OAL_PTR_NULL;
    }
    oal_netbuf_copy_queue_mapping(pst_netbuf, pst_netbuf_original);

    /* netbuf��headroom����802.11 macͷ���� */
    pst_frag_header =
        (mac_ieee80211_frame_stru *)(OAL_NETBUF_PAYLOAD(pst_netbuf) - MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
    MAC_GET_CB_80211_MAC_HEAD_TYPE(pst_tx_ctl_copy) = 1; /* ָʾmacͷ����skb�� */

    /* ����֡ͷ���� */
    if (EOK != memcpy_s(pst_frag_header, ul_mac_hdr_size, pst_mac_header, ul_mac_hdr_size)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_frag_init_netbuf::memcpy fail!");
        return OAL_PTR_NULL;
    }

    oal_netbuf_copydata(pst_netbuf_original, ul_offset, OAL_NETBUF_PAYLOAD(pst_netbuf), ul_payload);
    oal_netbuf_set_len(pst_netbuf, ul_payload);
    MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl_copy) = pst_frag_header;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl_copy) = (uint16_t)ul_payload;

    return pst_frag_header;
}

uint32_t hmac_frag_process(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *pst_netbuf_original, mac_tx_ctl_stru *pst_tx_ctl,
    uint32_t ul_cip_hdrsize, uint32_t ul_max_tx_unit)
{
    mac_ieee80211_frame_stru *pst_frag_header = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf_prev = OAL_PTR_NULL;
    uint32_t ul_total_hdrsize;
    uint32_t ul_frag_num;
    uint32_t ul_frag_size;
    int32_t l_remainder;
    uint32_t ul_payload;
    uint32_t ul_offset;
    uint32_t ul_mac_hdr_size;

    ul_mac_hdr_size = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl);
    ul_total_hdrsize = ul_mac_hdr_size + ul_cip_hdrsize;
    ul_frag_num = 1;
    /* �����ֽ��������ڷ�Ƭ�����У�Ԥ�������ֽڳ��ȣ���Ӳ����д����ͷ */
    ul_offset = ul_max_tx_unit - ul_cip_hdrsize - ul_mac_hdr_size;
    l_remainder = (int32_t)(oal_netbuf_len(pst_netbuf_original) - ul_offset);
    pst_netbuf_prev = pst_netbuf_original;

    do {
        ul_frag_size = ul_total_hdrsize + (uint32_t)l_remainder;

        /* �ж��Ƿ��и���ķ�Ƭ */
        if (ul_frag_size > ul_max_tx_unit) {
            ul_frag_size = ul_max_tx_unit;
        }

        /* ��ֹ��������ͷ���ռ�Ԥ��32�ֽ� */
        pst_netbuf = oal_netbuf_alloc(ul_frag_size + MAC_80211_QOS_HTC_4ADDR_FRAME_LEN + HMAC_FRAG_HEADER_RESV_LEN,
                                      MAC_80211_QOS_HTC_4ADDR_FRAME_LEN + HMAC_FRAG_HEADER_RESV_LEN, 4); /* 4 �ֽڶ��� */
        if (pst_netbuf == OAL_PTR_NULL) {
            /* ���ⲿ�ͷ�֮ǰ����ı��� */
            oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_frag_process::netbuf null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        /* �����Ƭ����֡�峤�� */
        ul_payload = ul_frag_size - ul_total_hdrsize;

        /* ��ֵ��Ƭ�� */
        pst_frag_header = hmac_frag_init_frag_netbuf(pst_netbuf_original, pst_tx_ctl, ul_offset,
                                                     pst_netbuf, ul_payload);
        if (pst_frag_header == OAL_PTR_NULL) {
            oal_netbuf_free(pst_netbuf);
            return OAL_FAIL;
        }
        pst_frag_header->bit_frag_num = ul_frag_num;
        ul_frag_num++;
        oal_netbuf_next(pst_netbuf_prev) = pst_netbuf;
        pst_netbuf_prev = pst_netbuf;

        /* ������һ����Ƭ���ĵĳ��Ⱥ�ƫ�� */
        l_remainder -= (int32_t)ul_payload;
        ul_offset += ul_payload;
    } while (l_remainder > 0);

    pst_frag_header->st_frame_control.bit_more_frag = OAL_FALSE;
    oal_netbuf_next(pst_netbuf) = OAL_PTR_NULL;

    /* ԭʼ������Ϊ��Ƭ���ĵĵ�һ�� */
    oal_netbuf_trim(pst_netbuf_original, oal_netbuf_len(pst_netbuf_original) -
                    (ul_max_tx_unit - ul_cip_hdrsize - ul_mac_hdr_size));
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (uint16_t)(oal_netbuf_len(pst_netbuf_original));

    return OAL_SUCC;
}


uint32_t hmac_defrag_timeout_fn(void *p_arg)
{
    hmac_user_stru *pst_hmac_user;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    pst_hmac_user = (hmac_user_stru *)p_arg;

    /* ��ʱ���ͷ���������ķ�Ƭ���� */
    if (pst_hmac_user->pst_defrag_netbuf != OAL_PTR_NULL) {
        pst_netbuf = pst_hmac_user->pst_defrag_netbuf;

        oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);
        oal_netbuf_free(pst_netbuf);
        pst_hmac_user->pst_defrag_netbuf = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_void hmac_defrag_same_frag_netbuf_check(hmac_user_stru *pst_hmac_user,
    mac_ieee80211_frame_stru *pst_mac_hdr)
{
    mac_ieee80211_frame_stru *pst_last_hdr = OAL_PTR_NULL;
    uint16_t us_rx_seq;
    uint8_t  uc_frag_num;
    uint16_t us_last_seq;
    uint8_t  uc_last_frag_num;

    us_rx_seq = pst_mac_hdr->bit_seq_num;
    uc_frag_num = (uint8_t)pst_mac_hdr->bit_frag_num;

    if (pst_hmac_user->pst_defrag_netbuf != OAL_PTR_NULL) {
        frw_timer_restart_timer(&pst_hmac_user->st_defrag_timer, HMAC_FRAG_TIMEOUT, OAL_FALSE);

        pst_last_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_hmac_user->pst_defrag_netbuf);

        us_last_seq = pst_last_hdr->bit_seq_num;
        uc_last_frag_num = (uint8_t)pst_last_hdr->bit_frag_num;

        /* �����ַ��ƥ�䣬���кŲ�ƥ�䣬��Ƭ�Ų�ƥ�����ͷ�������������ı��� */
        if (us_rx_seq != us_last_seq ||
            uc_frag_num != (uc_last_frag_num + 1) ||
            oal_compare_mac_addr(pst_last_hdr->auc_address1, pst_mac_hdr->auc_address1) ||
            oal_compare_mac_addr(pst_last_hdr->auc_address2, pst_mac_hdr->auc_address2)) {
            frw_timer_immediate_destroy_timer_m(&pst_hmac_user->st_defrag_timer);
            oal_netbuf_free(pst_hmac_user->pst_defrag_netbuf);
            pst_hmac_user->pst_defrag_netbuf = OAL_PTR_NULL;
        }
    }
}

OAL_STATIC uint32_t hmac_defrag_first_frag_netbuf_process(hmac_user_stru *pst_hmac_user,
    oal_netbuf_stru *pst_netbuf)
{
    oal_netbuf_stru *pst_new_buf = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctl = OAL_PTR_NULL;
    uint8_t uc_device_id;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    uc_device_id = pst_hmac_user->st_user_base_info.uc_device_id;
    pst_mac_device = mac_res_get_dev((uint32_t)uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log4(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_defrag_process::user index[%d] user mac:XX:XX:XX:%02X:%02X:%02X}",
                       pst_hmac_user->st_user_base_info.us_assoc_id,
                       pst_hmac_user->st_user_base_info.auc_user_mac_addr[3],  /* mac��ַ3 ���ĸ��ֽ� */
                       pst_hmac_user->st_user_base_info.auc_user_mac_addr[4],  /* mac��ַ4 ������ֽ� */
                       pst_hmac_user->st_user_base_info.auc_user_mac_addr[5]); /* mac��ַ5 �������ֽ� */
        /* user�쳣���������� */
        oal_netbuf_free(pst_netbuf);
        oam_stat_vap_incr(pst_hmac_user->st_user_base_info.uc_vap_id, rx_defrag_process_dropped, 1);
        return OAL_FAIL;
    }

    /* ������ʱ��ʱ������ʱ�ͷ����鱨�� */
    frw_timer_create_timer_m(&pst_hmac_user->st_defrag_timer,
                             hmac_defrag_timeout_fn,
                             HMAC_FRAG_TIMEOUT,
                             pst_hmac_user,
                             OAL_FALSE,
                             OAM_MODULE_ID_HMAC,
                             pst_mac_device->ul_core_id);

    /* �ڴ��netbufֻ��1600 ���ܲ���������A��˾����2500����ϵͳԭ��̬���� */
    pst_new_buf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, HMAC_MAX_FRAG_SIZE, OAL_NETBUF_PRIORITY_MID);
    if (pst_new_buf == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_defrag_process::Alloc new_buf null,size[%d].}",
                       HMAC_MAX_FRAG_SIZE);
        /* δ����ɹ������Ƭ��netbuf        ��Ҫ�ͷŷ�Ƭnetbuf */
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }
    oal_mem_netbuf_trace(pst_new_buf, OAL_FALSE);

    /* ����Ƭ���Ŀ�����������ı����в��ҽӵ��û��ṹ���£��ͷ�ԭ�еı��� */
    oal_netbuf_init(pst_new_buf, oal_netbuf_len(pst_netbuf));
    oal_netbuf_copydata(pst_netbuf, 0, oal_netbuf_data(pst_new_buf), oal_netbuf_len(pst_netbuf));
    memcpy_s(oal_netbuf_cb(pst_new_buf), MAC_TX_CTL_SIZE, oal_netbuf_cb(pst_netbuf), MAC_TX_CTL_SIZE);
    pst_rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_new_buf);
    MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctl) = (uint32_t *)oal_netbuf_header(pst_new_buf);
    pst_hmac_user->pst_defrag_netbuf = pst_new_buf;

    /* �ѿ�����pst_defrag_netbuf �ͷŷ�Ƭnetbuf */
    oal_netbuf_free(pst_netbuf);
    return OAL_SUCC;
}

OAL_STATIC oal_void hmac_defrag_non_first_frag_netbuf_process(hmac_user_stru *pst_hmac_user,
    oal_netbuf_stru *pst_netbuf, uint32_t ul_hrdsize)
{
    mac_ieee80211_frame_stru *pst_mac_hdr = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_last_hdr = OAL_PTR_NULL;

    pst_mac_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuf);
    pst_last_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_hmac_user->pst_defrag_netbuf);

    /* �˷�Ƭ�������ĵ����ķ�Ƭ��������ʱ�������������� */
    frw_timer_restart_timer(&pst_hmac_user->st_defrag_timer, HMAC_FRAG_TIMEOUT, OAL_FALSE);

    /* ��¼���·�Ƭ���ĵķ�Ƭ�� */
    pst_last_hdr->bit_seq_num = pst_mac_hdr->bit_seq_num;
    pst_last_hdr->bit_frag_num = pst_mac_hdr->bit_frag_num;

    oal_netbuf_pull(pst_netbuf, ul_hrdsize);

    /* �˽ӿ��ڻ����dev_kfree_skb */
    oal_netbuf_concat(pst_hmac_user->pst_defrag_netbuf, pst_netbuf);
}

OAL_STATIC oal_netbuf_stru *hmac_defrag_last_netbuf_process(hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf)
{
    mac_ieee80211_frame_stru *pst_last_hdr = OAL_PTR_NULL;
    uint32_t ul_ret;

    frw_timer_immediate_destroy_timer_m(&pst_hmac_user->st_defrag_timer);
    pst_netbuf = pst_hmac_user->pst_defrag_netbuf;
    pst_hmac_user->pst_defrag_netbuf = OAL_PTR_NULL;

    /* ������õı��Ľ���mic��� */
    ul_ret = hmac_de_mic(pst_hmac_user, pst_netbuf);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_netbuf);
        oam_stat_vap_incr(pst_hmac_user->st_user_base_info.uc_vap_id, rx_de_mic_fail_dropped, 1);
        oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_defrag_process::hmac_de_mic failed[%d].}", ul_ret);
        return OAL_PTR_NULL;
    }

    pst_last_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuf);
    pst_last_hdr->bit_frag_num = 0;
    return pst_netbuf;
}

oal_netbuf_stru *hmac_defrag_process(hmac_user_stru *pst_hmac_user,
    oal_netbuf_stru *pst_netbuf, uint32_t ul_hrdsize)
{
    mac_ieee80211_frame_stru *pst_mac_hdr = OAL_PTR_NULL;
    uint16_t us_rx_seq;
    uint8_t uc_frag_num;
    oal_bool_enum_uint8 en_more_frag;

    if (ul_hrdsize == 0) {
        oal_netbuf_free(pst_netbuf);
        oam_warning_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_defrag_process::mac head len is 0.}");
        return OAL_PTR_NULL;
    }

    pst_mac_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuf);
    us_rx_seq = pst_mac_hdr->bit_seq_num;
    uc_frag_num = (uint8_t)pst_mac_hdr->bit_frag_num;
    en_more_frag = (oal_bool_enum_uint8)pst_mac_hdr->st_frame_control.bit_more_frag;

    /* ���û��ʲô����ȥ��Ƭ����ֱ�ӷ��� */
    if (!en_more_frag && uc_frag_num == 0 && pst_hmac_user->pst_defrag_netbuf == OAL_PTR_NULL) {
        return pst_netbuf;
    }

    oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);

    /* ���ȼ�鵽���ķ�Ƭ�����ǲ���������������ķ�Ƭ���� */
    hmac_defrag_same_frag_netbuf_check(pst_hmac_user, pst_mac_hdr);

    if (pst_hmac_user->pst_defrag_netbuf == OAL_PTR_NULL) {
        /* û�з�Ƭ,ֱ�ӷ��� */
        if (en_more_frag == 0 && uc_frag_num == 0) {
            return pst_netbuf;
        }
        /* ��Ƭ��Ƭ�ķ�Ƭ�Ų�Ϊ0���ͷ� */
        if (uc_frag_num != 0) {
            oal_netbuf_free(pst_netbuf);
            oam_stat_vap_incr(pst_hmac_user->st_user_base_info.uc_vap_id, rx_defrag_process_dropped, 1);
            oam_info_log3(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ANY,
                          "{hmac_defrag_process::The first frag_num is not Zero(%d), seq_num[%d], mor_frag[%d].}",
                          uc_frag_num, us_rx_seq, en_more_frag);
            return OAL_PTR_NULL;
        }
        /* �����һ����Ƭ���� */
        if (hmac_defrag_first_frag_netbuf_process(pst_hmac_user, pst_netbuf) != OAL_SUCC) {
            return OAL_PTR_NULL;
        }
    } else {
        hmac_defrag_non_first_frag_netbuf_process(pst_hmac_user, pst_netbuf, ul_hrdsize);
    }

    /* δ������ϣ����ڸ��౨�ķ��ؿ�ָ�� */
    if (en_more_frag) {
        return OAL_PTR_NULL;
    }
    /* ������Ϸ�����õı��� */
    return hmac_defrag_last_netbuf_process(pst_hmac_user, pst_netbuf);
}