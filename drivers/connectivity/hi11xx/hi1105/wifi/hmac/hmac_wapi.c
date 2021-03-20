

#ifdef _PRE_WLAN_FEATURE_WAPI

/* 1 ͷ�ļ����� */
#include "oal_types.h"
#include "oal_mem.h"
#include "securec.h"
#include "mac_data.h"
#include "hmac_wapi_sms4.h"
#include "hmac_wapi_wpi.h"
#include "hmac_wapi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_C

/* 2 ȫ�ֱ������� */
OAL_CONST uint8_t g_auc_wapi_pn_init[WAPI_PN_LEN] = {
    0x36, 0x5c, 0x36, 0x5c, 0x36, 0x5c, 0x36,
    0x5c, 0x36, 0x5c, 0x36, 0x5c, 0x36, 0x5c, 0x36, 0x5c
};

/* 3 ����ʵ�� */

oal_bool_enum_uint8 hmac_wapi_is_qos(mac_ieee80211_frame_stru *pst_mac_hdr)
{
    return ((pst_mac_hdr->st_frame_control.bit_type == WLAN_DATA_BASICTYPE) &&
            (pst_mac_hdr->st_frame_control.bit_sub_type & WLAN_QOS_DATA));
}


uint32_t hmac_wapi_calc_mic_data(mac_ieee80211_frame_stru *pst_mac_hdr, uint8_t uc_mac_hdr_len,
                                       uint8_t uc_keyidx, uint8_t *puc_payload, uint16_t us_pdu_len,
                                       uint8_t *puc_mic, uint16_t us_mic_len)
{
    uint8_t us_is_qos;
    uint8_t *puc_mic_oringin = OAL_PTR_NULL;
    int32_t l_ret;

    memset_s(puc_mic, us_mic_len, 0, us_mic_len);

    puc_mic_oringin = puc_mic;

    /* frame control */
    l_ret = memcpy_s(puc_mic, us_mic_len,
                     (uint8_t *)&(pst_mac_hdr->st_frame_control),
                     OAL_SIZEOF(pst_mac_hdr->st_frame_control));
    puc_mic[0] &= ~(BIT4 | BIT5 | BIT6); /* sub type */
    puc_mic[1] &= ~(BIT3 | BIT4 | BIT5); /* retry, pwr Mgmt, more data */
    puc_mic[1] |= BIT6;

    puc_mic += OAL_SIZEOF(pst_mac_hdr->st_frame_control);

    /* addr1 */
    mac_get_addr1((uint8_t *)pst_mac_hdr, puc_mic, OAL_MAC_ADDR_LEN);
    puc_mic += OAL_MAC_ADDR_LEN;

    /* addr2 */
    mac_get_address2((uint8_t *)pst_mac_hdr, puc_mic, OAL_MAC_ADDR_LEN);
    puc_mic += OAL_MAC_ADDR_LEN;

    /* ���п��� */
    memset_s(puc_mic, OAL_SEQ_CTL_LEN, 0, OAL_SEQ_CTL_LEN);
    puc_mic[0] = (uint8_t)(pst_mac_hdr->bit_frag_num);
    puc_mic += 2;

    /* addr3 */
    mac_get_address3((uint8_t *)pst_mac_hdr, puc_mic, OAL_MAC_ADDR_LEN);
    puc_mic += OAL_MAC_ADDR_LEN;

    /* ����addr4 */
    puc_mic += OAL_MAC_ADDR_LEN;

    /* qos ctrl */
    us_is_qos = hmac_wapi_is_qos(pst_mac_hdr);
    if (us_is_qos == OAL_TRUE) {
        mac_get_qos_ctrl((uint8_t *)pst_mac_hdr, puc_mic, MAC_QOS_CTL_LEN);
        puc_mic += MAC_QOS_CTL_LEN;
    }

    /* keyidx + reserve�ܹ�2���ֽ� */
    *puc_mic = uc_keyidx;
    puc_mic += 2;

    /* ���pdulen Э��д������ֽ��� */
    *puc_mic = (uint8_t)((us_pdu_len & 0xff00) >> 8);
    *(puc_mic + 1) = (uint8_t)(us_pdu_len & 0x00ff);
    puc_mic += 2;

    /************����2����*******************/
    puc_mic_oringin += (OAL_TRUE == hmac_wapi_is_qos(pst_mac_hdr)) ?
        SMS4_MIC_PART1_QOS_LEN : SMS4_MIC_PART1_NO_QOS_LEN;
    l_ret += memcpy_s(puc_mic_oringin, us_pdu_len, puc_payload, us_pdu_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_wapi_calc_mic_data::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint8_t *hmac_wapi_mic_alloc(uint8_t is_qos, uint16_t us_pdu_len, uint16_t *pus_mic_len)
{
    uint16_t us_mic_part1_len;
    uint16_t us_mic_part2_len;
    uint16_t us_mic_len;

    us_mic_part1_len = (is_qos == OAL_TRUE) ? SMS4_MIC_PART1_QOS_LEN : SMS4_MIC_PART1_NO_QOS_LEN;

    /* ����Э�飬���벻��λ��16�ֽڶ��� */
    us_mic_part2_len = padding_m(us_pdu_len, SMS4_PADDING_LEN);

    us_mic_len = us_mic_part1_len + us_mic_part2_len;

    *pus_mic_len = us_mic_len;

    return oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_mic_len, OAL_TRUE);
}


OAL_STATIC void hmac_wapi_mic_free(uint8_t *puc_mic)
{
    if (puc_mic != OAL_PTR_NULL) {
        oal_mem_free_m(puc_mic, OAL_TRUE);
    }
}


uint8_t hmac_wapi_is_keyidx_valid(hmac_wapi_stru *pst_wapi, uint8_t uc_keyidx_rx)
{
    if (pst_wapi->uc_keyidx != uc_keyidx_rx && pst_wapi->uc_keyupdate_flg != OAL_TRUE) {
        oam_warning_log3(0, OAM_SF_ANY, "{hmac_wapi_is_keyidx_valid::keyidx==%u, uc_keyidx_rx==%u, update==%u.}",
                         pst_wapi->uc_keyidx, uc_keyidx_rx, pst_wapi->uc_keyupdate_flg);
        WAPI_RX_IDX_UPDATE_ERR(pst_wapi);
        return OAL_FALSE;
    }

    pst_wapi->uc_keyupdate_flg = OAL_FALSE; /* �������ȡ����־ */

    /* keyû������ */
    if (pst_wapi->ast_wapi_key[uc_keyidx_rx].uc_key_en != OAL_TRUE) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_wapi_is_keyidx_valid::keyen==%u.}",
                         pst_wapi->ast_wapi_key[uc_keyidx_rx].uc_key_en);
        WAPI_RX_IDX_UPDATE_ERR(pst_wapi);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


oal_bool_enum_uint8 hmac_wapi_is_pn_odd_ucast(uint8_t *puc_pn)
{
    return (oal_bool_enum_uint8)(((*puc_pn & BIT0) == 0) ? OAL_FALSE : OAL_TRUE);
}


uint8_t hmac_wapi_is_pn_bigger(uint8_t *puc_pn, uint8_t *puc_pn_rx)
{
    uint8_t uc_pnidx;

    for (uc_pnidx = SMS4_PN_LEN - 1; uc_pnidx > 0; uc_pnidx--) {
        if ((puc_pn[uc_pnidx] != puc_pn_rx[uc_pnidx])) {
            if (puc_pn[uc_pnidx] > puc_pn_rx[uc_pnidx]) {
                oam_warning_log2(0, OAM_SF_ANY,
                                 "{hmac_wapi_is_pn_bigger::err! puc_pn==%u, puc_pn_rx==%u.}",
                                 puc_pn[uc_pnidx], puc_pn_rx[uc_pnidx]);
                return OAL_FALSE;
            }

            return OAL_TRUE;
        }
    }

    return OAL_TRUE;
}


uint8_t hmac_wapi_is_pn_odd_bcast(uint8_t *puc_pn)
{
    return OAL_TRUE;
}


void hmac_wapi_pn_update(uint8_t *puc_pn, uint8_t uc_inc)
{
    uint32_t ul_loop;
    uint32_t ul_loop_num;
    uint32_t ul_overlow; /* ��λ */
    uint32_t *pul_pn;

    pul_pn = (uint32_t *)puc_pn;
    ul_loop_num = WAPI_PN_LEN / OAL_SIZEOF(uint32_t);
    ul_overlow = uc_inc;

    for (ul_loop = 0; ul_loop < ul_loop_num; ul_loop++) {
        if (*pul_pn > (*pul_pn + ul_overlow)) {
            *pul_pn += ul_overlow;
            ul_overlow = 1; /* �����λ��1 */
        } else {
            *pul_pn += ul_overlow;
            break;
        }
        pul_pn++;
    }
}


oal_netbuf_stru *hmac_wapi_netbuff_tx_handle(hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_buf)
{
    uint32_t ul_ret;
    oal_netbuf_stru *pst_netbuf_tmp = OAL_PTR_NULL;  /* ָ����Ҫ�ͷŵ�netbuff */
    oal_netbuf_stru *pst_netbuf_prev = OAL_PTR_NULL; /* ָ���Ѿ����ܵ�netbuff */
    oal_netbuf_stru *pst_buf_first = OAL_PTR_NULL;   /* ָ��δ���ܵ�netbuff */

    /* buf�ĳ�ʼλ����snapͷ��llc�� */
    if (MAC_DATA_WAPI == mac_get_data_type_from_80211(pst_buf, 0)) {
        oam_warning_log0(0, OAM_SF_WAPI, "{hmac_wapi_netbuff_tx_handle::wapi, dont encrypt!.}");
        return pst_buf;
    }

    ul_ret = pst_wapi->wapi_encrypt(pst_wapi, pst_buf);
    if (ul_ret != OAL_SUCC) {
        hmac_free_netbuf_list(pst_buf);
        return OAL_PTR_NULL;
    }

    pst_netbuf_tmp = pst_buf;

    /* ʹnetbuffָ��ָ����һ����Ҫ���ܵķ�Ƭ֡ */
    pst_netbuf_prev = oal_netbuf_next(pst_buf);
    if (pst_netbuf_prev == OAL_PTR_NULL) {
        // print TODO
        return OAL_PTR_NULL;
    }
    pst_buf_first = pst_netbuf_prev;
    pst_buf = oal_netbuf_next(pst_netbuf_prev);

    oal_netbuf_free(pst_netbuf_tmp);

    while (pst_buf != OAL_PTR_NULL) {
        ul_ret = pst_wapi->wapi_encrypt(pst_wapi, pst_buf);
        if (ul_ret != OAL_SUCC) {
            hmac_free_netbuf_list(pst_buf_first);
            return OAL_PTR_NULL;
        }
        oal_netbuf_next(pst_netbuf_prev) = oal_netbuf_next(pst_buf);
        pst_netbuf_tmp = pst_buf;
        pst_netbuf_prev = oal_netbuf_next(pst_buf);
        if (pst_netbuf_prev == OAL_PTR_NULL) {
            // print TODO
            return OAL_PTR_NULL;
        }
        pst_buf = oal_netbuf_next(pst_netbuf_prev);

        oal_netbuf_free(pst_netbuf_tmp);
    }
    return pst_buf_first;
}


oal_netbuf_stru *hmac_wapi_netbuff_rx_handle(hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_buf)
{
    uint32_t ul_ret;
    oal_netbuf_stru *pst_netbuf_tmp = OAL_PTR_NULL; /* ָ����Ҫ�ͷŵ�netbuff */

    /* �Ǽ���֡�������н��� */
    if (!((oal_netbuf_data(pst_buf))[1] & 0x40)) {
        return pst_buf;
    }

    ul_ret = pst_wapi->wapi_decrypt(pst_wapi, pst_buf);
    if (ul_ret != OAL_SUCC) {
        return OAL_PTR_NULL;
    }

    pst_netbuf_tmp = pst_buf;
    pst_buf = oal_netbuf_next(pst_buf);
    oal_netbuf_free(pst_netbuf_tmp);

    return pst_buf;
}


uint32_t hmac_wapi_add_key(hmac_wapi_stru *pst_wapi, uint8_t uc_key_index, uint8_t *puc_key)
{
    hmac_wapi_key_stru *pst_key = OAL_PTR_NULL;
    int32_t l_ret;

    WAPI_PORT_FLAG(pst_wapi) = OAL_TRUE;
    pst_wapi->uc_keyidx = uc_key_index;
    pst_wapi->uc_keyupdate_flg = OAL_TRUE;
    pst_key = &(pst_wapi->ast_wapi_key[uc_key_index]);

    l_ret = memcpy_s(pst_key->auc_wpi_ek, WAPI_KEY_LEN, puc_key, WAPI_KEY_LEN);
    l_ret += memcpy_s(pst_key->auc_wpi_ck, WAPI_KEY_LEN, puc_key + WAPI_KEY_LEN, WAPI_KEY_LEN);
    pst_key->uc_key_en = OAL_TRUE;

    /* ����PN */
    l_ret += memcpy_s(pst_key->auc_pn_rx, WAPI_PN_LEN, g_auc_wapi_pn_init, WAPI_PN_LEN);
    l_ret += memcpy_s(pst_key->auc_pn_tx, WAPI_PN_LEN, g_auc_wapi_pn_init, WAPI_PN_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_wapi_add_key::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint8_t hmac_wapi_is_wpihdr_valid(hmac_wapi_stru *pst_wapi, uint8_t *puc_wapi_hdr)
{
    uint8_t uc_keyidx_rx;
    uint8_t *puc_pn_rx = OAL_PTR_NULL;

    uc_keyidx_rx = *puc_wapi_hdr;

    if (OAL_TRUE != hmac_wapi_is_keyidx_valid(pst_wapi, uc_keyidx_rx)) {
        return OAL_FALSE;
    }

    puc_pn_rx = puc_wapi_hdr + SMS4_KEY_IDX + SMS4_WAPI_HDR_RESERVE;
    if (pst_wapi->wapi_is_pn_odd(puc_pn_rx) != OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wapi_is_wpihdr_valid::wapi_is_pn_odd==false.}");
        WAPI_RX_PN_ODD_ERR(pst_wapi, puc_pn_rx);
        return OAL_FALSE;
    }

    /* �˴�Ϊ�طż�⣬ʵ�ʲ����зǳ�Ӱ�����ܣ���ʱ���� */
    return OAL_TRUE;
}


uint32_t hmac_wapi_decrypt(hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuf)
{
    oal_netbuf_stru *pst_netbuff_des = NULL;
    uint8_t *puc_key_ek = NULL;
    uint8_t *puc_key_ck = NULL;
    uint8_t uc_key_index;
    uint16_t us_pdu_len;
    uint8_t auc_calc_mic[SMS4_MIC_LEN];
    uint32_t ul_wapi_result;

    mac_ieee80211_frame_stru *pst_mac_hdr = OAL_PTR_NULL;
    uint8_t *puc_netbuff = OAL_PTR_NULL;
    uint16_t us_netbuff_len;
    uint8_t *puc_wapi_hdr = OAL_PTR_NULL;
    uint8_t uc_mac_hdr_len;
    uint8_t *puc_pn = OAL_PTR_NULL;
    uint8_t *puc_pdu = OAL_PTR_NULL;
    uint8_t *puc_mic_data = OAL_PTR_NULL; /* ����Э�飬����mic����Ҫ�����ݣ��� wapiʵʩָ�� ͼ51 */
    uint8_t *puc_mic = OAL_PTR_NULL;
    uint16_t us_mic_len;
    mac_rx_ctl_stru *pst_rx_ctl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctl_in = OAL_PTR_NULL;

    WAPI_RX_PORT_VALID(pst_wapi);
    /************ 1. ����ǰ������׼��,��ȡ��ͷָ������ݳ��� ************/
    puc_netbuff = oal_netbuf_data(pst_netbuf);  // for ut,del temprarily
    us_netbuff_len = (uint16_t)oal_netbuf_len(pst_netbuf);

    /* ��ȡmacͷ */
    pst_mac_hdr = (mac_ieee80211_frame_stru *)puc_netbuff;

    /* wapi������֡һ��ΪQOS֡  */
    pst_rx_ctl_in = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    uc_mac_hdr_len = pst_rx_ctl_in->uc_mac_header_len;
    puc_wapi_hdr = (uint8_t *)pst_mac_hdr + uc_mac_hdr_len;
    puc_pn = puc_wapi_hdr + SMS4_KEY_IDX + SMS4_WAPI_HDR_RESERVE;
    puc_pdu = puc_pn + SMS4_PN_LEN;

    oam_warning_log1(0, OAM_SF_ANY, "{hmac_wpi_decrypt::uc_mac_hdr_len %u!.}", uc_mac_hdr_len);

    if (us_netbuff_len < (uint16_t)(uc_mac_hdr_len + HMAC_WAPI_HDR_LEN + SMS4_MIC_LEN)) {
        oam_error_log2(0, OAM_SF_ANY,
                       "{hmac_wpi_decrypt::ul_netbuff_len %u, machdr len %u err!.}",
                       us_netbuff_len, uc_mac_hdr_len);
        oal_netbuf_free(pst_netbuf);
        WAPI_RX_NETBUF_LEN_ERR(pst_wapi);
        return OAL_FAIL;
    }
    us_pdu_len = us_netbuff_len - uc_mac_hdr_len - HMAC_WAPI_HDR_LEN - SMS4_MIC_LEN;

    uc_key_index = *puc_wapi_hdr;

    if (uc_key_index >= HMAC_WAPI_MAX_KEYID) {
        WAPI_RX_IDX_ERR(pst_wapi);
        oam_error_log1(0, OAM_SF_ANY, "{hmac_wpi_decrypt::uc_key_index %u err!.}", uc_key_index);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    if (OAL_TRUE != hmac_wapi_is_wpihdr_valid(pst_wapi, puc_wapi_hdr)) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wpi_decrypt::hmac_wapi_is_wpihdr_valid err!.}");
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    /************ 2. ׼���µ�netbuff,������Ž��ܺ������, ��дcb�ֶ� ************/
    pst_netbuff_des = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuff_des == NULL) {
        WAPI_RX_MEMALLOC_ERR(pst_wapi);
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* �ȿ���macͷ */
    oal_netbuf_init(pst_netbuff_des, uc_mac_hdr_len);
    oal_netbuf_copydata(pst_netbuf, 0, oal_netbuf_data(pst_netbuff_des), uc_mac_hdr_len);

    /* ����cb */
    pst_rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuff_des);
    if (EOK != memcpy_s(pst_rx_ctl, MAC_TX_CTL_SIZE, oal_netbuf_cb(pst_netbuf), MAC_TX_CTL_SIZE)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_wpi_decrypt::memcpy fail!");
        oal_netbuf_free(pst_netbuff_des);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    /************ 3. ����ǰ����Կ׼����PN׼�� ************/
    puc_key_ek = pst_wapi->ast_wapi_key[uc_key_index].auc_wpi_ek;
    hmac_wpi_swap_pn(puc_pn, SMS4_PN_LEN);

    /******************** 4. ����**************************/
    ul_wapi_result = hmac_wpi_decrypt(puc_pn,
                                          puc_pdu,
                                          (us_pdu_len + SMS4_MIC_LEN), /* ��Ҫ���ܵ����ݳ��� */
                                          puc_key_ek,
                                          (oal_netbuf_data(pst_netbuff_des) + uc_mac_hdr_len));
    if (ul_wapi_result != OAL_SUCC) {
        oal_netbuf_free(pst_netbuff_des);
        /* ����֮ǰע�����netbuff�Ƿ������汻�ͷ� */
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_WAPI_DECRYPT_FAIL;
    }

    /* mic��ΪУ����������Ҫput */
    oal_netbuf_put(pst_netbuff_des, us_pdu_len);

    /************ 5. ����mic��������У�� ************/
    puc_mic_data = hmac_wapi_mic_alloc(hmac_wapi_is_qos(pst_mac_hdr), us_pdu_len, &us_mic_len);
    if (puc_mic_data == OAL_PTR_NULL) {
        WAPI_RX_MEMALLOC_ERR(pst_wapi);
        oal_netbuf_free(pst_netbuff_des);
        /* ע��netbuff�����Ƿ����ͷŴ��� */
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ����micԤ������ */
    (void)hmac_wapi_calc_mic_data(pst_mac_hdr, uc_mac_hdr_len,
                                      uc_key_index, oal_netbuf_data(pst_netbuff_des) + uc_mac_hdr_len,
                                      us_pdu_len, puc_mic_data, us_mic_len);

    puc_key_ck = pst_wapi->ast_wapi_key[uc_key_index].auc_wpi_ck;
    ul_wapi_result = hmac_wpi_pmac(puc_pn,
                                       puc_mic_data,
                                       (us_mic_len >> 4),
                                       puc_key_ck,
                                       auc_calc_mic,
                                       sizeof(auc_calc_mic));

    /* ������mic���ͷ�mic data */
    hmac_wapi_mic_free(puc_mic_data);
    if (ul_wapi_result != OAL_SUCC) {
        oal_netbuf_free(pst_netbuff_des);

        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_WAPI_MIC_CALC_FAIL;
    }

    puc_mic = oal_netbuf_data(pst_netbuff_des) + uc_mac_hdr_len + us_pdu_len;
    if (0 != oal_memcmp(puc_mic, auc_calc_mic, SMS4_MIC_LEN)) { /* �Ƚ�MIC */
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wpi_decrypt::mic check fail!.}");
        WAPI_RX_MIC_ERR(pst_wapi);
        oal_netbuf_free(pst_netbuff_des);
        oal_netbuf_free(pst_netbuf);
        return OAL_ERR_CODE_WAPI_MIC_CMP_FAIL;
    }

    /* ����ǰ��protected */
    (oal_netbuf_data(pst_netbuff_des))[1] &= ~0x40;

    /* ��дcb */
    MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctl) = (uint32_t *)oal_netbuf_header(pst_netbuff_des);
    pst_rx_ctl->uc_mac_header_len = uc_mac_hdr_len;
    pst_rx_ctl->us_frame_len = (uint16_t)oal_netbuf_len(pst_netbuff_des);

    oal_netbuf_next(pst_netbuff_des) = oal_netbuf_next(pst_netbuf);
    oal_netbuf_next(pst_netbuf) = pst_netbuff_des;

    hmac_wapi_pn_update(pst_wapi->ast_wapi_key[pst_wapi->uc_keyidx].auc_pn_rx, pst_wapi->uc_pn_inc);

    oam_warning_log0(0, OAM_SF_ANY, "{hmac_wpi_decrypt::OK!.}");
    WAPI_RX_DECRYPT_OK(pst_wapi);
    return OAL_SUCC;
}


uint32_t hmac_wapi_encrypt(hmac_wapi_stru *pst_wapi, oal_netbuf_stru *pst_netbuf)
{
    mac_ieee80211_frame_stru *pst_mac_hdr = OAL_PTR_NULL;
    uint8_t uc_mac_hdr_len;
    uint8_t *puc_mic_data = OAL_PTR_NULL;
    uint16_t us_pdu_len;
    uint16_t us_mic_len;
    uint8_t uc_key_index;
    uint8_t *puc_key_ck = OAL_PTR_NULL;
    uint8_t *puc_key_ek = OAL_PTR_NULL;
    uint8_t auc_calc_mic[SMS4_MIC_LEN];
    uint8_t auc_pn_swap[SMS4_PN_LEN]; /* ����任���pn,��������mic�ͼ��� */
    oal_netbuf_stru *pst_netbuff_des = OAL_PTR_NULL;
    uint8_t *puc_wapi_hdr = OAL_PTR_NULL;
    uint8_t *puc_datain = OAL_PTR_NULL;
    uint32_t ul_result;
    uint8_t *puc_payload = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl;
    int32_t l_ret;

    /************ 1. ����ǰ������׼��,��ȡ��ͷָ������ݳ��� ************/
    /* ��ȡmacͷ */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    pst_mac_hdr = MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl);
    uc_mac_hdr_len = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl);

    /* ���ü���λ ע�⣬macͷ�漰���ܣ�������Ҫ���ʼ���� */
    mac_set_protectedframe((uint8_t *)pst_mac_hdr);

    oam_warning_log2(0, OAM_SF_ANY,
                     "{hmac_wapi_encrypt::uc_mac_hdr_len %u! bit_more==%u.}",
                     uc_mac_hdr_len, pst_mac_hdr->st_frame_control.bit_more_frag);
    us_pdu_len = MAC_GET_CB_MPDU_LEN(pst_tx_ctl);

    /* payload����ʼλ����snapͷ��llc�� */
    puc_payload = oal_netbuf_data(pst_netbuf);

    /************ 2. ����mic,wapi������֡һ��ΪQOS֡  ************/
    puc_mic_data = hmac_wapi_mic_alloc(hmac_wapi_is_qos(pst_mac_hdr), us_pdu_len, &us_mic_len);
    if (puc_mic_data == OAL_PTR_NULL) {
        WAPI_TX_MEMALLOC_ERR(pst_wapi);

        oam_error_log0(0, OAM_SF_ANY, "{hmac_wapi_encrypt::hmac_wapi_mic_alloc err!");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ����micԤ������ */
    uc_key_index = pst_wapi->uc_keyidx;
    (void)hmac_wapi_calc_mic_data(pst_mac_hdr, uc_mac_hdr_len,
                                      uc_key_index, puc_payload, us_pdu_len,
                                      puc_mic_data, us_mic_len);

    puc_key_ck = pst_wapi->ast_wapi_key[uc_key_index].auc_wpi_ck;
    l_ret = memcpy_s(auc_pn_swap, SMS4_PN_LEN, pst_wapi->ast_wapi_key[uc_key_index].auc_pn_tx, SMS4_PN_LEN);
    hmac_wpi_swap_pn(auc_pn_swap, SMS4_PN_LEN);
    /* ����mic */
    ul_result = hmac_wpi_pmac(auc_pn_swap,
                                  puc_mic_data,
                                  (us_mic_len >> 4),
                                  puc_key_ck,
                                  auc_calc_mic,
                                  sizeof(auc_calc_mic));

    hmac_wapi_mic_free(puc_mic_data);
    if (ul_result == OAL_FAIL) {
        WAPI_TX_MIC_ERR(pst_wapi);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wapi_encrypt::hmac_wpi_pmac mic calc err!");
        return OAL_ERR_CODE_WAPI_MIC_CALC_FAIL;
    }

    /************ 3. ׼���µ�netbuff,������ż��ܺ������, ��дcb,��׼������ǰ������ ************/
    pst_netbuff_des = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuff_des == OAL_PTR_NULL) {
        WAPI_TX_MEMALLOC_ERR(pst_wapi);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wapi_encrypt::pst_netbuff_des alloc err!");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��дcb */
    l_ret += memcpy_s(oal_netbuf_cb(pst_netbuff_des), MAC_TX_CTL_SIZE, oal_netbuf_cb(pst_netbuf), MAC_TX_CTL_SIZE);

    /* �ȿ���macͷ,Ϊ�˺���hcc�����˴���д���Ŀռ� */
    oal_netbuf_init(pst_netbuff_des, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
    l_ret += memcpy_s(oal_netbuf_data(pst_netbuff_des), uc_mac_hdr_len, pst_mac_hdr, uc_mac_hdr_len);

    puc_datain = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_pdu_len + SMS4_MIC_LEN, OAL_TRUE);
    if (puc_datain == OAL_PTR_NULL) {
        oal_netbuf_free(pst_netbuff_des);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wapi_encrypt::puc_datain alloc err!");
        WAPI_TX_MEMALLOC_ERR(pst_wapi);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    l_ret += memcpy_s(puc_datain, us_pdu_len + SMS4_MIC_LEN, puc_payload, us_pdu_len);
    /* ����mic */
    l_ret += memcpy_s(puc_datain + us_pdu_len, SMS4_MIC_LEN, auc_calc_mic, SMS4_MIC_LEN);

    puc_key_ek = pst_wapi->ast_wapi_key[uc_key_index].auc_wpi_ek;
    /************************ 4. ���� ************************/
    ul_result = hmac_wpi_encrypt(auc_pn_swap,
        puc_datain, us_pdu_len + SMS4_MIC_LEN, puc_key_ek,
        oal_netbuf_data(pst_netbuff_des) + HMAC_WAPI_HDR_LEN + MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);

    oal_mem_free_m(puc_datain, OAL_TRUE);
    if (ul_result != OAL_SUCC) {
        oal_netbuf_free(pst_netbuff_des);
        oam_error_log1(0, OAM_SF_ANY, "{hmac_wapi_encrypt::hmac_wpi_encrypt err==%u!", ul_result);
        return OAL_ERR_CODE_WAPI_ENRYPT_FAIL;
    }
    /* �˴�put��֮��netbuff��lenΪmacͷ+pdulen+sms4+wapi�ĳ��� */
    oal_netbuf_put(pst_netbuff_des, us_pdu_len + SMS4_MIC_LEN + HMAC_WAPI_HDR_LEN);

    /***************** 5. ��дwapiͷ *****************/
    puc_wapi_hdr = oal_netbuf_data(pst_netbuff_des) + MAC_80211_QOS_HTC_4ADDR_FRAME_LEN;

    /* ��дWPIͷ -- keyIndex */
    *(puc_wapi_hdr) = uc_key_index;
    /* ����λ���� */
    *(puc_wapi_hdr + SMS4_KEY_IDX) = 0;
    /* ��дPN */
    l_ret += memcpy_s((puc_wapi_hdr + SMS4_KEY_IDX + SMS4_WAPI_HDR_RESERVE), SMS4_PN_LEN,
                      pst_wapi->ast_wapi_key[uc_key_index].auc_pn_tx, SMS4_PN_LEN);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_wapi_encrypt::memcpy fail!");
        oal_netbuf_free(pst_netbuff_des);
        return OAL_FAIL;
    }

    /* �ٴ���дcb */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuff_des);
    MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctl) = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuff_des);

    /* netbuf ��dataָ��ָ��payload */
    oal_netbuf_pull(pst_netbuff_des, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);

    /* ������mac hdr */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (uint16_t)(HMAC_WAPI_HDR_LEN + us_pdu_len + SMS4_MIC_LEN);
    oal_netbuf_next(pst_netbuff_des) = oal_netbuf_next(pst_netbuf);
    oal_netbuf_next(pst_netbuf) = pst_netbuff_des;
    /* ����pn */
    hmac_wapi_pn_update(pst_wapi->ast_wapi_key[pst_wapi->uc_keyidx].auc_pn_tx, pst_wapi->uc_pn_inc);
    oam_warning_log0(0, OAM_SF_ANY, "{hmac_wapi_encrypt::hmac_wpi_encrypt OK!");

    WAPI_TX_ENCRYPT_OK(pst_wapi);
    return OAL_SUCC;
}


void hmac_wapi_deinit(hmac_wapi_stru *pst_wapi)
{
    memset_s(pst_wapi, OAL_SIZEOF(hmac_wapi_stru), 0, OAL_SIZEOF(hmac_wapi_stru));
}


void hmac_wapi_reset_port(hmac_wapi_stru *pst_wapi)
{
    WAPI_PORT_FLAG(pst_wapi) = OAL_FALSE;
}


uint32_t hmac_wapi_init(hmac_wapi_stru *pst_wapi, uint8_t uc_pairwise)
{
    uint32_t ul_loop;

    hmac_wapi_deinit(pst_wapi);

    if (uc_pairwise == OAL_TRUE) {
        pst_wapi->uc_pn_inc = WAPI_UCAST_INC;
        pst_wapi->wapi_is_pn_odd = hmac_wapi_is_pn_odd_ucast;
    } else {
        pst_wapi->uc_pn_inc = WAPI_BCAST_INC;
        pst_wapi->wapi_is_pn_odd = hmac_wapi_is_pn_odd_bcast;
    }

    for (ul_loop = 0; ul_loop < HMAC_WAPI_MAX_KEYID; ul_loop++) {
        pst_wapi->ast_wapi_key[ul_loop].uc_key_en = OAL_FALSE;
    }

    WAPI_PORT_FLAG(pst_wapi) = OAL_FALSE;
    pst_wapi->wapi_decrypt = hmac_wapi_decrypt;
    pst_wapi->wapi_encrypt = hmac_wapi_encrypt;
    pst_wapi->wapi_netbuff_txhandle = hmac_wapi_netbuff_tx_handle;
    pst_wapi->wapi_netbuff_rxhandle = hmac_wapi_netbuff_rx_handle;
    return OAL_SUCC;
}


#ifdef _PRE_WAPI_DEBUG

void hmac_wapi_dump_frame(uint8_t *puc_info, uint8_t *puc_data, uint32_t ul_len)
{
    uint32_t ul_loop;
    oal_io_print("%s: \r\n", puc_info);
    for (ul_loop = 0; ul_loop < ul_len; ul_loop += 4) {
        oal_io_print("%2x ", ul_loop / 4);
        oal_io_print("%2x %2x %2x %2x \r\n", puc_data[ul_loop], puc_data[ul_loop + 1],
                     puc_data[ul_loop + 2], puc_data[ul_loop + 3]);
    }
}


void hmac_wapi_display_usr_info(hmac_user_stru *pst_hmac_user)
{
    uint32_t ul_loop;
    hmac_wapi_stru *pst_wapi = OAL_PTR_NULL;
    hmac_wapi_key_stru *pst_key = OAL_PTR_NULL;
    hmac_wapi_debug *pst_debug = OAL_PTR_NULL;

    oam_warning_log1(0, OAM_SF_ANY, "wapi port is %u!", WAPI_PORT_FLAG(&pst_hmac_user->st_wapi));
    if (WAPI_PORT_FLAG(&pst_hmac_user->st_wapi) != OAL_TRUE) {
        oal_io_print("Err! wapi port is not valid!\n");

        return;
    }

    pst_wapi = &(pst_hmac_user->st_wapi);
    oam_warning_log0(0, OAM_SF_ANY, "keyidx\tupdate\t\tpn_inc\t\n");
    oam_warning_log3(0, OAM_SF_ANY, "%u\t%08x%04x\t\n",
                     pst_wapi->uc_keyidx,
                     pst_wapi->uc_keyupdate_flg,
                     pst_wapi->uc_pn_inc);

    for (ul_loop = 0; ul_loop < HMAC_WAPI_MAX_KEYID; ul_loop++) {
        pst_key = &pst_wapi->ast_wapi_key[ul_loop];
        /*lint -e64*/
        hmac_wapi_dump_frame("ek :", pst_key->auc_wpi_ek, WAPI_KEY_LEN);
        hmac_wapi_dump_frame("ck :", pst_key->auc_wpi_ck, WAPI_KEY_LEN);
        hmac_wapi_dump_frame("pn_local_rx :", pst_key->auc_pn_rx, WAPI_PN_LEN);
        hmac_wapi_dump_frame("pn_local_tx :", pst_key->auc_pn_tx, WAPI_PN_LEN);
        oam_warning_log1(0, OAM_SF_ANY, "key_en: %u\n", pst_key->uc_key_en);
        /*lint +e64*/
    }

    pst_debug = &pst_wapi->st_debug;
    oam_warning_log0(0, OAM_SF_ANY, "TX DEBUG INFO:");
    /*lint -e64 */
    hmac_wapi_dump_frame("pn_rx :", pst_debug->aucrx_pn, WAPI_PN_LEN);
    /*lint +e64 */
    oam_warning_log4(0, OAM_SF_ANY, "tx_drop==%u, tx_wai==%u, tx_port_valid==%u, tx_memalloc_fail==%u",
                     pst_debug->ultx_ucast_drop,
                     pst_debug->ultx_wai,
                     pst_debug->ultx_port_valid,
                     pst_debug->ultx_memalloc_err);
    oam_warning_log3(0, OAM_SF_ANY, "tx_mic_calc_fail==%u, tx_encrypt_ok==%u, tx_memalloc_err==%u",
                     pst_debug->ultx_mic_calc_fail,
                     // pst_debug->ultx_drop_wai,
                     pst_debug->ultx_encrypt_ok,
                     pst_debug->ultx_memalloc_err);

    oam_warning_log0(0, OAM_SF_ANY, "RX DEBUG INFO:");
    oam_warning_log4(0, OAM_SF_ANY, "rx_port_valid==%u, rx_idx_err==%u, rx_netbuff_len_err==%u, rx_idx_update_err==%u",
                     pst_debug->ulrx_port_valid,
                     pst_debug->ulrx_idx_err,
                     pst_debug->ulrx_netbuff_len_err,
                     pst_debug->ulrx_idx_update_err);

    oam_warning_log4(0, OAM_SF_ANY, "rx_key_en_err==%u, rx_pn_odd_err==%u, rx_pn_replay_err==%u, rx_decrypt_ok==%u",
                     pst_debug->ulrx_key_en_err,
                     pst_debug->ulrx_pn_odd_err,
                     pst_debug->ulrx_pn_replay_err,
                     pst_debug->ulrx_decrypt_ok);
    return;
}


uint32_t hmac_wapi_display_info(mac_vap_stru *pst_mac_vap, uint16_t us_usr_idx)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_multi_user;

    pst_hmac_multi_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_multi_user_idx);
    if (pst_hmac_multi_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "Err! multi usr %u does not exist!",
                         pst_mac_vap->us_multi_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "*****************multi usr info start****************");
    hmac_wapi_display_usr_info(pst_hmac_multi_user);
    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "*****************multi usr info end****************");

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_usr_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "Err! ucast usr %u does not exist!", us_usr_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "*****************ucast usr info start****************");
    hmac_wapi_display_usr_info(pst_hmac_user);
    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "*****************ucast usr info end****************");

    return OAL_SUCC;
}
#endif

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

