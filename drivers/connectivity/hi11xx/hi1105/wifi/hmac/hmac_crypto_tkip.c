

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ͷ�ļ����� */
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "mac_frame.h"
#include "mac_common.h"
#include "hmac_crypto_tkip.h"

/* 2 ȫ�ֱ������� */
#define MAX_TKIP_PN_GAP_ERR 1 /* Max. gap in TKIP PN before doing MIC sanity check */

/* 3 ����ʵ�� */

OAL_STATIC void hmac_crypto_tkip_michael_hdr(mac_ieee80211_frame_stru *pst_frame_header,
                                                 uint8_t *puc_hdr, uint8_t uc_hdr_size)
{
    mac_ieee80211_frame_addr4_stru *pst_frame_4addr_hdr = OAL_PTR_NULL;
    uint8_t uc_frame_dir;
    int32_t l_ret = EOK;

    pst_frame_4addr_hdr = (mac_ieee80211_frame_addr4_stru *)pst_frame_header;

    uc_frame_dir = (pst_frame_4addr_hdr->st_frame_control.bit_to_ds) ? 1 : 0;
    uc_frame_dir += (pst_frame_4addr_hdr->st_frame_control.bit_from_ds) ? 2 : 0;

    switch (uc_frame_dir) {
        case IEEE80211_FC1_DIR_NODS:
            l_ret += memcpy_s(puc_hdr, uc_hdr_size, pst_frame_4addr_hdr->auc_address1, WLAN_MAC_ADDR_LEN);
            l_ret += memcpy_s(puc_hdr + WLAN_MAC_ADDR_LEN, uc_hdr_size - WLAN_MAC_ADDR_LEN,
                              pst_frame_4addr_hdr->auc_address2, WLAN_MAC_ADDR_LEN);
            break;
        case IEEE80211_FC1_DIR_TODS:
            l_ret += memcpy_s(puc_hdr, uc_hdr_size, pst_frame_4addr_hdr->auc_address3, WLAN_MAC_ADDR_LEN);
            l_ret += memcpy_s(puc_hdr + WLAN_MAC_ADDR_LEN, uc_hdr_size - WLAN_MAC_ADDR_LEN,
                              pst_frame_4addr_hdr->auc_address2, WLAN_MAC_ADDR_LEN);
            break;
        case IEEE80211_FC1_DIR_FROMDS:
            l_ret += memcpy_s(puc_hdr, uc_hdr_size, pst_frame_4addr_hdr->auc_address1, WLAN_MAC_ADDR_LEN);
            l_ret += memcpy_s(puc_hdr + WLAN_MAC_ADDR_LEN, uc_hdr_size - WLAN_MAC_ADDR_LEN,
                              pst_frame_4addr_hdr->auc_address3, WLAN_MAC_ADDR_LEN);
            break;
        default:
            l_ret += memcpy_s(puc_hdr, uc_hdr_size, pst_frame_4addr_hdr->auc_address3, WLAN_MAC_ADDR_LEN);
            l_ret += memcpy_s(puc_hdr + WLAN_MAC_ADDR_LEN, uc_hdr_size - WLAN_MAC_ADDR_LEN,
                              pst_frame_4addr_hdr->auc_address4, WLAN_MAC_ADDR_LEN);
            break;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_crypto_tkip_michael_hdr::memcpy fail!");
        return;
    }

    puc_hdr[12] = 0;

    if (pst_frame_4addr_hdr->st_frame_control.bit_sub_type == WLAN_QOS_DATA) {
        if (IEEE80211_FC1_DIR_DSTODS == uc_frame_dir) {
            puc_hdr[12] = ((mac_ieee80211_qos_frame_addr4_stru *)pst_frame_4addr_hdr)->bit_qc_tid;
        } else {
            puc_hdr[12] = ((mac_ieee80211_qos_frame_stru *)pst_frame_4addr_hdr)->bit_qc_tid;
        }
    }

    puc_hdr[13] = puc_hdr[14] = puc_hdr[15] = 0; /* reserved */
}

OAL_STATIC void hmac_crypto_tkip_micheal_padding(uint32_t ul_data_len,
                                                     uint8_t uc_pd_src0, uint8_t uc_pd_src1,
                                                     uint8_t uc_pd_src2, uint32_t *pul_msb)
{
    switch (ul_data_len) {
        case 0:
            *pul_msb ^= get_le32_split(0x5a, 0, 0, 0);
            break;
        case 1:
            *pul_msb ^= get_le32_split(uc_pd_src0, 0x5a, 0, 0);
            break;
        case 2:
            *pul_msb ^= get_le32_split(uc_pd_src0, uc_pd_src1, 0x5a, 0);
            break;
        case 3:
            *pul_msb ^= get_le32_split(uc_pd_src0, uc_pd_src1, uc_pd_src2, 0x5a);
            break;
        default:
            break;
    }
}


OAL_STATIC uint32_t hmac_crypto_tkip_michael_mic(const unsigned char *puc_key,
                                                   oal_netbuf_stru *pst_netbuf,
                                                   mac_ieee80211_frame_stru *pst_frame_header,
                                                   uint32_t ul_offset, uint32_t ul_data_len,
                                                   uint8_t auc_mic[IEEE80211_WEP_MICLEN],
                                                   uint8_t uc_mic_len)
{
    uint8_t auc_hdr[16] = { 0 };
    uint32_t ul_msb;
    uint32_t ul_lsb;
    const uint8_t *puc_data = OAL_PTR_NULL;
    uint32_t ul_space;
    const uint8_t *puc_data_next = OAL_PTR_NULL;
    uint8_t *puc_hdr = OAL_PTR_NULL;

    hmac_crypto_tkip_michael_hdr(pst_frame_header, auc_hdr, sizeof(auc_hdr));

    ul_msb = get_le32(puc_key);
    ul_lsb = get_le32(puc_key + 4);

    /* Michael MIC pseudo header: DA, SA, 3 x 0, Priority */
    puc_hdr = &auc_hdr[0];
    ul_msb ^= get_le32(puc_hdr);
    michael_block(ul_msb, ul_lsb);
    puc_hdr = &auc_hdr[4];
    ul_msb ^= get_le32(puc_hdr);
    michael_block(ul_msb, ul_lsb);
    puc_hdr = &auc_hdr[8];
    ul_msb ^= get_le32(puc_hdr);
    michael_block(ul_msb, ul_lsb);
    puc_hdr = &auc_hdr[12];
    ul_msb ^= get_le32(&auc_hdr[12]);
    michael_block(ul_msb, ul_lsb);

    /* first buffer has special handling */
    puc_data = oal_netbuf_data(pst_netbuf) + ul_offset;
    ul_space = oal_netbuf_len(pst_netbuf) - ul_offset;
    for (;;) {
        if (ul_space > ul_data_len) {
            ul_space = ul_data_len;
        }

        /* collect 32-bit blocks from current buffer */
        while (ul_space >= sizeof(uint32_t)) {
            ul_msb ^= get_le32(puc_data);
            michael_block(ul_msb, ul_lsb);
            puc_data += sizeof(uint32_t);
            ul_space -= sizeof(uint32_t);
            ul_data_len -= sizeof(uint32_t);
        }

        if (ul_data_len < sizeof(uint32_t)) {
            break;
        }

        pst_netbuf = oal_netbuf_list_next(pst_netbuf);
        if (pst_netbuf == NULL) {
            return OAL_ERR_CODE_SECURITY_BUFF_NUM;
        }
        if (ul_space != 0) {
            /*
             * Block straddles buffers, split references.
             */
            puc_data_next = oal_netbuf_data(pst_netbuf);
            if (sizeof(uint32_t) - ul_space > oal_netbuf_len(pst_netbuf)) {
                return OAL_ERR_CODE_SECURITY_BUFF_LEN;
            }
            switch (ul_space) {
                case 1:
                    ul_msb ^= get_le32_split(puc_data[0], puc_data_next[0], puc_data_next[1], puc_data_next[2]);
                    puc_data = puc_data_next + 3;
                    ul_space = oal_netbuf_len(pst_netbuf) - 3;
                    break;
                case 2:
                    ul_msb ^= get_le32_split(puc_data[0], puc_data[1], puc_data_next[0], puc_data_next[1]);
                    puc_data = puc_data_next + 2;
                    ul_space = oal_netbuf_len(pst_netbuf) - 2;
                    break;
                case 3:
                    ul_msb ^= get_le32_split(puc_data[0], puc_data[1], puc_data[2], puc_data_next[0]);
                    puc_data = puc_data_next + 1;
                    ul_space = oal_netbuf_len(pst_netbuf) - 1;
                    break;
                default:
                    break;
            }

            michael_block(ul_msb, ul_lsb);
            ul_data_len -= sizeof(uint32_t);
        } else {
            /*
            * Setup for next buffer.
            */
            puc_data = oal_netbuf_data(pst_netbuf);
            ul_space = oal_netbuf_len(pst_netbuf);
        }
    }
    /* Last block and padding (0x5a, 4..7 x 0) */
    hmac_crypto_tkip_micheal_padding(ul_data_len, puc_data[0], puc_data[1], puc_data[2], &ul_msb);

    michael_block(ul_msb, ul_lsb);
    /* l ^= 0; */
    michael_block(ul_msb, ul_lsb);

    if (uc_mic_len < IEEE80211_WEP_MICLEN) {
        return OAL_FAIL;
    }

    put_le32(auc_mic, ul_msb);
    put_le32(auc_mic + 4, ul_lsb);

    return OAL_SUCC;
}

uint32_t hmac_crypto_tkip_enmic(wlan_priv_key_param_stru *pst_key, oal_netbuf_stru *pst_netbuf)
{
    uint32_t ul_hdrlen = 0; /* ����ʱ��ul_pktlen�ﲻ��80211macͷ */
    uint32_t ul_pktlen;
    uint32_t ul_ret;
    uint8_t *puc_mic_tail = OAL_PTR_NULL;
    uint8_t *puc_tx_mic_key = OAL_PTR_NULL;
    uint8_t auc_mic[IEEE80211_WEP_MICLEN] = { 0 };
    mac_tx_ctl_stru *pst_cb = OAL_PTR_NULL;
    uint32_t ul_tailroom;

    if (WLAN_80211_CIPHER_SUITE_TKIP != ((uint8_t)pst_key->ul_cipher)) {
        oam_warning_log1(0, OAM_SF_WPA, "hmac_crypto_tkip_enmic::pst_key->ul_cipher=0x%x", pst_key->ul_cipher);
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    /* 1.1 �����������ĵĳ��ȣ���������ڶ��netbuf����� */
    ul_pktlen = oal_netbuf_len(pst_netbuf);
    if (OAL_PTR_NULL != oal_netbuf_next(pst_netbuf)) {
        return OAL_ERR_CODE_SECURITY_BUFF_NUM;
    }

    /* 2.1 ��netbuf������mic�ռ� */
    ul_tailroom = oal_netbuf_tailroom(pst_netbuf);
    if (oal_unlikely(ul_tailroom < IEEE80211_WEP_MICLEN)) {
        /* 2.2 ���ԭ����netbuf���Ȳ�������Ҫ�������� */
        pst_netbuf = oal_netbuf_realloc_tailroom(pst_netbuf, IEEE80211_WEP_MICLEN);
        if (pst_netbuf == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_WPA, "{hmac_crypto_tkip_enmic::tailroom[%d] realloc fail.}", ul_tailroom);
            return OAL_ERR_CODE_PTR_NULL;
        }
    }

    /* 3.1 ��ȡmic����Կ */
    puc_mic_tail = (uint8_t *)OAL_NETBUF_TAIL(pst_netbuf);
    puc_tx_mic_key = pst_key->auc_key + WLAN_TEMPORAL_KEY_LENGTH;

    oal_netbuf_put(pst_netbuf, IEEE80211_WEP_MICLEN);

    pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* 4.1 ����mic */
    ul_ret = hmac_crypto_tkip_michael_mic(puc_tx_mic_key, pst_netbuf,
        MAC_GET_CB_FRAME_HEADER_ADDR(pst_cb), ul_hdrlen,
        ul_pktlen - ul_hdrlen, auc_mic, sizeof(auc_mic));
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /* 4.1 ����mic��֡β�� */
    ul_ret = memcpy_s(puc_mic_tail, ul_tailroom, auc_mic, IEEE80211_WEP_MICLEN);
    if (ul_ret != EOK) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_crypto_tkip_enmic::memcpy error!}");
    }
    return OAL_SUCC;
}


uint32_t hmac_crypto_tkip_demic(wlan_priv_key_param_stru *pst_key, oal_netbuf_stru *pst_netbuf)
{
    mac_rx_ctl_stru *pst_cb = OAL_PTR_NULL;
    uint32_t ul_hdrlen; /* ����ʱ��ul_pktlen�����80211macͷ */
    uint8_t *puc_rx_mic_key = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t ul_pktlen;
    uint8_t auc_mic[IEEE80211_WEP_MICLEN] = { 0 };
    uint8_t auc_mic_peer[IEEE80211_WEP_MICLEN] = { 0 };

    if (WLAN_80211_CIPHER_SUITE_TKIP != ((uint8_t)pst_key->ul_cipher)) {
        oam_warning_log1(0, OAM_SF_WPA, "hmac_crypto_tkip_demic::pst_key->ul_cipher=0x%x", pst_key->ul_cipher);
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    /* 1.1 �����������ĵĳ��ȣ���������ڶ��netbuf����� */
    ul_pktlen = oal_netbuf_len(pst_netbuf);
    if (OAL_PTR_NULL != oal_netbuf_list_next(pst_netbuf)) {
        return OAL_ERR_CODE_SECURITY_BUFF_NUM;
    }

    /* 2.1 ��CB�л�ȡ80211ͷ���� */
    pst_cb = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    ul_hdrlen = pst_cb->uc_mac_header_len;

    /* 3.1 ֻ��������֡ */
    /* 4.1 ��ȡ������Կ��������Կ��Ҫƫ��8���ֽ� */
    puc_rx_mic_key = pst_key->auc_key + WLAN_TEMPORAL_KEY_LENGTH + WLAN_MIC_KEY_LENGTH;

    /* 5.1 ����mic */
    ul_ret = hmac_crypto_tkip_michael_mic(puc_rx_mic_key, pst_netbuf,
        (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_cb),
        ul_hdrlen, ul_pktlen - (ul_hdrlen + IEEE80211_WEP_MICLEN), auc_mic, sizeof(auc_mic));
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /* 6.1 ��ȡ�Զ˵�mic�������ؼ����mic���бȽ� */
    oal_netbuf_copydata(pst_netbuf, ul_pktlen - IEEE80211_WEP_MICLEN, (void *)auc_mic_peer, IEEE80211_WEP_MICLEN);
    if (oal_memcmp(auc_mic, auc_mic_peer, IEEE80211_WEP_MICLEN)) {
        /* NB: 802.11 layer handles statistic and debug msg
        ieee80211_notify_michael_failure(vap, wh, k->wk_keyix); */
        return OAL_ERR_CODE_SECURITY_WRONG_KEY;
    }

    /* 7.1 ȥ��micβ�� */
    oal_netbuf_trim(pst_netbuf, IEEE80211_WEP_MICLEN);
    return OAL_SUCC;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

