

#ifndef __DMAC_BLOCKACK_H__
#define __DMAC_BLOCKACK_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"

#include "hmac_ext_if.h"
#include "hmac_user.h"
#include "hmac_main.h"
#include "hmac_rx_data.h"
#include "hmac_vap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BLOCKACK_H

/* 2 �궨�� */
/* ��ֵ���²���ʱ,Ϊ����ҵ��ඪ֡,��Ҫ����������г�ʱ��ʱ������ */
#define HMAC_BA_RX_VO_TIMEOUT 40  /* 40 milliseconds */
#define HMAC_BA_RX_VI_TIMEOUT 100 /* 100 milliseconds */
#define HMAC_BA_RX_BE_TIMEOUT 60  /* 100 milliseconds */
#define HMAC_BA_RX_BK_TIMEOUT 100 /* 100 milliseconds */
#define HMAC_BA_RX_DEFAULT_TIMEOUT 100 /* 100 milliseconds */

/* Ϊ������Ϸ����,�������������ϵ�ʱ,��������г�ʱ��ʱ�����ڵ�С,������ҵ��������Ϸ���� */
#define HMAC_BA_RX_VO_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_VI_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_BE_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_BK_TIMEOUT_MIN 20 /* 20 milliseconds */

#define HMAC_BA_SEQNO_MASK              0x0FFF /* max sequece number */
#define HMAC_BA_SEQNO_SUB(_seq1, _seq2) (((_seq1) - (_seq2)) & HMAC_BA_SEQNO_MASK)
#define HMAC_BA_SEQNO_ADD(_seq1, _seq2) (((_seq1) + (_seq2)) & HMAC_BA_SEQNO_MASK)
#define MAX_BA_SN    4095
#define HMAC_U32_MAX 0xffffffff

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 inline�������� */

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_rx_seqno_lt(uint16_t us_seq1, uint16_t us_seq2)
{
    return (((us_seq1 < us_seq2) && ((us_seq2 - us_seq1) < DMAC_BA_MAX_SEQNO_BY_TWO)) ||
            ((us_seq1 > us_seq2) && ((us_seq1 - us_seq2) > DMAC_BA_MAX_SEQNO_BY_TWO)));
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_rx_seqno_gt(uint16_t us_seq1, uint16_t us_seq2)
{
    return hmac_ba_rx_seqno_lt(us_seq2, us_seq1);
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_seqno_lt(uint16_t us_seq1, uint16_t us_seq2)
{
    return (((us_seq1 < us_seq2) && ((us_seq2 - us_seq1) < DMAC_BA_MAX_SEQNO_BY_TWO)) ||
            ((us_seq1 > us_seq2) && ((us_seq1 - us_seq2) > DMAC_BA_MAX_SEQNO_BY_TWO)));
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_seqno_gt(uint16_t us_seq1, uint16_t us_seq2)
{
    return hmac_ba_seqno_lt(us_seq2, us_seq1);
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_rx_seqno_leq(uint16_t us_seq1, uint16_t us_seq2)
{
    return (((us_seq1 <= us_seq2) && ((us_seq2 - us_seq1) < DMAC_BA_MAX_SEQNO_BY_TWO)) ||
            ((us_seq1 > us_seq2) && ((us_seq1 - us_seq2) > DMAC_BA_MAX_SEQNO_BY_TWO)));
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_rx_seqno_geq(uint16_t us_seq1, uint16_t us_seq2)
{
    return hmac_ba_rx_seqno_leq(us_seq2, us_seq1);
}

/* ����seq num��ba��start��ƫ���� */
#define HMAC_BA_INDEX(_st, _seq) (((_seq) - (_st)) & 4095)

#define HMAC_TX_BUF_BITMAP_LOG2_WORD_SIZE 5 /* log2(32) == 5 */
/* ����BA���ڼ�¼seq number��bitmap��ʹ�õ����ͳ��� */
#define HMAC_TX_BUF_BITMAP_WORD_SIZE 32

#define HMAC_TX_BUF_BITMAP_WORD_MASK (HMAC_TX_BUF_BITMAP_WORD_SIZE - 1)

#define HMAC_TX_BUF_BITMAP_SET(_bitmap, _idx)              \
    ((_bitmap)[(_idx) >> HMAC_TX_BUF_BITMAP_LOG2_WORD_SIZE] |= \
        (1 << ((_idx) & HMAC_TX_BUF_BITMAP_WORD_MASK)))

#define HMAC_TX_BUF_BITMAP_CLR(_bitmap, _idx)              \
    ((_bitmap)[(_idx) >> HMAC_TX_BUF_BITMAP_LOG2_WORD_SIZE] &= \
        ~((uint32_t)(1 << ((_idx) & HMAC_TX_BUF_BITMAP_WORD_MASK))))

/* ��������б���bitmap���ڻ�������֡ */
#define HMAC_BA_BMP_SIZE 64

/* �ж�indexΪn ��bitmap�е�bitλ�Ƿ���1 */
#define HMAC_BA_ISSET(_bm, _n) (((_n) < (HMAC_BA_BMP_SIZE)) && \
                                ((_bm)[(_n) >> 5] & (1 << ((_n)&31))))

/* �ж�һ��seq num�Ƿ��ڷ��ʹ����� */
#define HMAC_BAW_WITHIN(_start, _bawsz, _seqno) \
    ((((_seqno) - (_start)) & 4095) < (_bawsz))

/* �ж�һ��seq num�Ƿ��ڷ��ʹ����ұ� */
#define HMAC_BAW_RIGHTSIDE(_start, _seqno) \
    ((((_seqno) >= (_start)) ? ((((_seqno) - (_start)) & 4095) < (2048)) : \
     ((((_seqno) + 4095 - (_start)) & 4095) < (2048))) && \
     (_start != _seqno))


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_isset(hmac_ba_rx_stru *pst_ba_rx_hdl, uint16_t us_seqno)
{
    uint16_t us_index;

    if (HMAC_BAW_WITHIN(pst_ba_rx_hdl->us_baw_head, HMAC_BA_BMP_SIZE, us_seqno)) {
        us_index = us_seqno & (HMAC_BA_BMP_SIZE - 1);

        if (HMAC_BA_ISSET(pst_ba_rx_hdl->aul_rx_buf_bitmap, us_index)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


OAL_STATIC OAL_INLINE void hmac_ba_addto_rx_bitmap(hmac_ba_rx_stru *pst_ba_rx_hdl, uint16_t us_seqno)
{
    uint16_t us_index;

    if (HMAC_BAW_WITHIN(pst_ba_rx_hdl->us_baw_head, HMAC_BA_BMP_SIZE, us_seqno)) {
        us_index = us_seqno & (HMAC_BA_BMP_SIZE - 1);
        HMAC_TX_BUF_BITMAP_SET(pst_ba_rx_hdl->aul_rx_buf_bitmap, us_index);
    }
}


OAL_STATIC OAL_INLINE void hmac_ba_clear_rx_bitmap(hmac_ba_rx_stru *pst_ba_rx_hdl)
{
    uint16_t us_index;

    us_index = DMAC_BA_SEQNO_SUB(pst_ba_rx_hdl->us_baw_head, 1) & (HMAC_BA_BMP_SIZE - 1);
    HMAC_TX_BUF_BITMAP_CLR(pst_ba_rx_hdl->aul_rx_buf_bitmap, us_index);
}


OAL_STATIC OAL_INLINE void hmac_ba_update_rx_bitmap(hmac_user_stru *pst_hmac_user,
                                                    mac_ieee80211_frame_stru *pst_frame_hdr)
{
    oal_bool_enum_uint8 en_is_4addr;
    uint8_t uc_is_tods;
    uint8_t uc_is_from_ds;
    uint8_t uc_tid;
    hmac_ba_rx_stru *pst_ba_rx_hdl;

    /* �����ĵ�ַ�����ȡ���ĵ�tid */
    uc_is_tods = mac_hdr_get_to_ds((uint8_t *)pst_frame_hdr);
    uc_is_from_ds = mac_hdr_get_from_ds((uint8_t *)pst_frame_hdr);
    en_is_4addr = uc_is_tods && uc_is_from_ds;
    uc_tid = mac_get_tid_value((uint8_t *)pst_frame_hdr, en_is_4addr);

    pst_ba_rx_hdl = pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info;
    if (pst_ba_rx_hdl == OAL_PTR_NULL) {
        return;
    }
    if (pst_ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        return;
    }

    oal_spin_lock(&pst_ba_rx_hdl->st_ba_lock);

    /* ����baw start����bitmap��64bits */
    while (hmac_ba_seqno_lt(pst_ba_rx_hdl->us_baw_head,
                            DMAC_BA_SEQNO_SUB(pst_ba_rx_hdl->us_baw_start, (HMAC_BA_BMP_SIZE - 1)))) {
        pst_ba_rx_hdl->us_baw_head = DMAC_BA_SEQNO_ADD(pst_ba_rx_hdl->us_baw_head, 1);
        hmac_ba_clear_rx_bitmap(pst_ba_rx_hdl);
    }

    hmac_ba_addto_rx_bitmap(pst_ba_rx_hdl, mac_get_seq_num((uint8_t *)pst_frame_hdr));

    oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);

    return;
}


OAL_STATIC OAL_INLINE uint8_t hmac_ba_seqno_bound_chk(uint16_t us_seq_lo, uint16_t us_seq_hi, uint16_t us_seq)
{
    oal_bool_enum_uint8 en_lo_chk;
    oal_bool_enum_uint8 en_hi_chk;
    uint8_t uc_chk_res = 0;

    en_lo_chk = hmac_ba_rx_seqno_leq(us_seq_lo, us_seq);
    en_hi_chk = hmac_ba_rx_seqno_leq(us_seq, us_seq_hi);
    if ((en_lo_chk == OAL_TRUE) && (en_hi_chk == OAL_TRUE)) {
        uc_chk_res = DMAC_BA_BETWEEN_SEQLO_SEQHI;
    } else if (en_hi_chk == OAL_FALSE) {
        uc_chk_res = DMAC_BA_GREATER_THAN_SEQHI;
    }

    return uc_chk_res;
}

/* This function reads out the TX-Dscr indexed by the specified sequence number in */
/* the Retry-Q Ring-Buffer. */

OAL_STATIC OAL_INLINE hmac_rx_buf_stru *hmac_remove_frame_from_reorder_q(
    hmac_ba_rx_stru *pst_ba_rx_hdl, uint16_t us_seq_num)
{
    uint16_t us_idx;
    hmac_rx_buf_stru *pst_rx_buff;

    us_idx = (us_seq_num & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    pst_rx_buff = HMAC_GET_BA_RX_DHL(pst_ba_rx_hdl, us_idx);

    oal_spin_lock(&pst_ba_rx_hdl->st_ba_lock);

    if ((pst_rx_buff->in_use == 0) || (pst_rx_buff->us_seq_num != us_seq_num)) {
        oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);
        return OAL_PTR_NULL;
    }

    pst_rx_buff->in_use = 0;
    pst_ba_rx_hdl->uc_mpdu_cnt--;

    oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);

    return pst_rx_buff;
}


OAL_STATIC OAL_INLINE hmac_rx_buf_stru *hmac_get_frame_from_reorder_q(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                                      uint16_t us_seq_num)
{
    uint16_t us_idx;
    hmac_rx_buf_stru *pst_rx_buff;

    us_idx = (us_seq_num & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    pst_rx_buff = HMAC_GET_BA_RX_DHL(pst_ba_rx_hdl, us_idx);
    if ((pst_rx_buff->in_use == 0) || (pst_rx_buff->us_seq_num != us_seq_num)) {
        return OAL_PTR_NULL;
    }

    return pst_rx_buff;
}

OAL_STATIC OAL_INLINE uint8_t hmac_ba_get_lut_index(uint8_t *puc_ba_lut_index_table,
                                                    uint16_t us_start,
                                                    uint16_t us_stop)
{
    return oal_get_lut_index(puc_ba_lut_index_table, MAC_RX_BA_LUT_BMAP_LEN, HAL_MAX_RX_BA_LUT_SIZE, us_start, us_stop);
}

OAL_STATIC OAL_INLINE void hmac_ba_del_lut_index(uint8_t *puc_ba_lut_index_table, uint8_t uc_lut_index)
{
    oal_del_lut_index(puc_ba_lut_index_table, uc_lut_index);
}

/*****************************************************************************
  10 ��������
*****************************************************************************/
uint32_t hmac_ba_filter_serv(hmac_user_stru *pst_hmac_user, mac_rx_ctl_stru *pst_cb_ctrl,
    oal_netbuf_head_stru *pst_netbuf_header, oal_bool_enum_uint8 *pen_is_ba_buf);

uint32_t hmac_ba_timeout_fn(void *p_arg);

uint8_t hmac_mgmt_check_set_rx_ba_ok(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
    hmac_ba_rx_stru *pst_ba_rx_info, mac_device_stru *pst_device, hmac_tid_stru *pst_tid_info);

void hmac_reorder_ba_rx_buffer_bar(hmac_ba_rx_stru *pst_rx_ba, uint16_t us_start_seq_num,
    mac_vap_stru *pst_vap);
uint32_t hmac_ba_reset_rx_handle(mac_device_stru *pst_mac_device, hmac_user_stru *pst_hmac_user, uint8_t uc_tid,
    oal_bool_enum_uint8 en_is_aging);
void hmac_up_rx_bar(hmac_vap_stru *pst_hmac_vap, dmac_rx_ctl_stru *pst_rx_ctl,
    oal_netbuf_stru *pst_netbuf);
oal_bool_enum_uint8 hmac_is_device_ba_setup(void);
void hmac_ba_rx_hdl_init(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, uint8_t uc_tid);

void hmac_ba_rx_win_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t rx_tid);

uint32_t hmac_ps_rx_delba(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);

uint32_t hmac_ba_rx_data_release(hmac_user_stru *pst_user,
    hmac_ba_rx_stru *pst_ba_rx_hdl, oal_netbuf_stru *pst_netbuf_mpdu, oal_netbuf_head_stru *pst_netbuf_head);

uint32_t hmac_ba_rx_data_buffer(hmac_user_stru *hmac_user,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf_mpdu, oal_netbuf_head_stru *netbuf_head);

uint32_t  hmac_ba_check_rx_aggrate(hmac_vap_stru *pst_vap,
    mac_rx_ctl_stru *pst_rx_ctl);

void hmac_ba_rx_hdl_hold(hmac_ba_rx_stru *ba_rx_hdl);
void hmac_ba_rx_hdl_put(hmac_ba_rx_stru *ba_rx_hdl);
uint32_t hmac_ba_send_ring_reorder_timeout(hmac_ba_rx_stru *rx_ba,
    hmac_vap_stru *hmac_vap, hmac_ba_alarm_stru *alarm_data, uint32_t *p_timeout);
uint32_t hmac_ba_send_reorder_timeout(hmac_ba_rx_stru *pst_rx_ba, hmac_vap_stru *pst_hmac_vap,
    hmac_ba_alarm_stru *pst_alarm_data, uint32_t *pus_timeout);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of dmac_blockack.h */
