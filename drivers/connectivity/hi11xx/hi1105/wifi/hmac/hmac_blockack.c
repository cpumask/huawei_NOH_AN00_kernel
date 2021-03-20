

/* 1 ͷ�ļ����� */
#include "wlan_spec.h"
#include "mac_vap.h"
#include "hmac_blockack.h"
#include "hmac_main.h"
#include "hmac_rx_data.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_user.h"
#include "hmac_auto_adjust_freq.h"
#include "wlan_chip_i.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BLOCKACK_C

const uint16_t g_ba_rx_timeout_len[WLAN_WME_AC_PSM + 2] = { // 2 enum num
    HMAC_BA_RX_BE_TIMEOUT,      /* WLAN_WME_AC_BE */
    HMAC_BA_RX_BK_TIMEOUT,      /* WLAN_WME_AC_BK */
    HMAC_BA_RX_VI_TIMEOUT,      /* WLAN_WME_AC_VI */
    HMAC_BA_RX_VO_TIMEOUT,      /* WLAN_WME_AC_VO */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_SOUNDING */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MGMT */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MC */
};

OAL_STATIC uint8_t hmac_ba_timeout_param_is_valid(hmac_ba_alarm_stru *alarm_data,
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t rx_tid)
{
    if (rx_tid >= WLAN_TID_MAX_NUM) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_ba_timeout_fn::tid %d overflow.}", rx_tid);
        return OAL_FALSE;
    }

    if (hmac_vap == OAL_PTR_NULL) {
        oam_error_log2(0, 0, "{hmac_ba_timeout_fn::vap:%d, vap[%p]}", alarm_data->uc_vap_id, (uintptr_t)hmac_vap);
        return OAL_FALSE;
    }

    if (hmac_user == OAL_PTR_NULL) {
        oam_error_log2(0, 0, "{hmac_ba_timeout_fn::user:%d,user[%p]}", alarm_data->us_mac_user_idx, hmac_user);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

void hmac_reorder_ba_timer_start(hmac_vap_stru *pst_hmac_vap,
                                 hmac_user_stru *pst_hmac_user,
                                 uint8_t uc_tid)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_ba_rx_stru *pst_ba_rx_stru = OAL_PTR_NULL;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    uint16_t us_timeout;

    /* �����ʱ��ʱ���Ѿ���ע���򷵻� */
    if (pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer.en_is_registerd == OAL_TRUE) {
        return;
    }

    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_reorder_ba_timer_start::pst_device[%d] null.}",
                       pst_mac_vap->uc_device_id);
        return;
    }

    /* ҵ������Сʱ,ʹ��С���ڵ�������ʱ��,��֤��ʱ�ϱ���Э��ջ;
       ҵ�����ϴ�ʱ,ʹ�ô����ڵ�������ʱ��,��֤���������� */
    if (OAL_FALSE == hmac_wifi_rx_is_busy()) {
        us_timeout = pst_hmac_vap->us_rx_timeout_min[WLAN_WME_TID_TO_AC(uc_tid)];
    } else {
        us_timeout = pst_hmac_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(uc_tid)];
    }

    pst_ba_rx_stru = pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info;

    oal_spin_lock(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer_lock));

    frw_timer_create_timer_m(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer),
                             hmac_ba_timeout_fn,
                             us_timeout,
                             &(pst_ba_rx_stru->st_alarm_data),
                             OAL_FALSE,
                             OAM_MODULE_ID_HMAC,
                             pst_device->ul_core_id);

    oal_spin_unlock(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer_lock));
}


void hmac_ba_rx_hdl_baw_init(hmac_user_stru *pst_hmac_user,
    hmac_ba_rx_stru *pst_ba_rx_stru, uint8_t uc_tid)
{
    /* ����ba sizeʵ�ʴ�С,���ڷ���addba rsp */
    pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;

    if (pst_ba_rx_stru->us_baw_size <= 1) {
        pst_ba_rx_stru->us_baw_size = WLAN_AMPDU_RX_BUFFER_SIZE;
        /* �쳣���·���ֵ */
        pst_hmac_user->aus_tid_baw_size[uc_tid] = WLAN_AMPDU_RX_BUFFER_SIZE;
    }

#if defined(_PRE_WLAN_FEATURE_11AX)
    /* HE�豸����BA������64ʱ, */
    if (g_wlan_spec_cfg->feature_11ax_is_open &&
        ((pst_ba_rx_stru->us_baw_size > WLAN_AMPDU_RX_BUFFER_SIZE) &&
        MAC_USER_IS_HE_USER(&pst_hmac_user->st_user_base_info))) {
        /* resp����ֵ��addba req����һ��,����BA BAW��Ҫ����,�������������,��˲�����user */
        pst_ba_rx_stru->us_baw_size = WLAN_AMPDU_RX_HE_BUFFER_SIZE;

        /* ���ƻ��趨BA BAW, ��ȡС */
        if (g_st_rx_buffer_size_stru.en_rx_ampdu_bitmap_ini == OAL_TRUE) {
            pst_ba_rx_stru->us_baw_size =
                    oal_min(g_st_rx_buffer_size_stru.us_rx_buffer_size, WLAN_AMPDU_RX_HE_BUFFER_SIZE);
        }

        /* ����������:������ߵ�BAW size С�ڶԶ�BAW size,��ô����resp֡BAW size,�����նԶ˵�BAW size���� */
        pst_hmac_user->aus_tid_baw_size[uc_tid] =
                    oal_min(pst_hmac_user->aus_tid_baw_size[uc_tid], pst_ba_rx_stru->us_baw_size);
    } else
#endif
    {
        /* BA��С��64ʱЭ��BA��SIZE */
        pst_ba_rx_stru->us_baw_size = oal_min(pst_ba_rx_stru->us_baw_size, WLAN_AMPDU_RX_BUFFER_SIZE);
        /* ��HE�豸����,����ֵ��bawͬ������ */
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }

    /* �������ȼ���ߣ��ֶ����ý��վۺϸ��� */
    if (g_st_rx_buffer_size_stru.en_rx_ampdu_bitmap_cmd == OAL_TRUE) {
        pst_ba_rx_stru->us_baw_size = g_st_rx_buffer_size_stru.us_rx_buffer_size;
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }
}


void hmac_ba_rx_hdl_init(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, uint8_t uc_tid)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_ba_rx_stru *pst_ba_rx_stru = OAL_PTR_NULL;
    hmac_rx_buf_stru *pst_rx_buff = OAL_PTR_NULL;
    uint16_t us_reorder_index;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    pst_ba_rx_stru = pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info;

    /* ��ʼ��reorder���� */
    for (us_reorder_index = 0; us_reorder_index < WLAN_AMPDU_RX_HE_BUFFER_SIZE; us_reorder_index++) {
        pst_rx_buff = HMAC_GET_BA_RX_DHL(pst_ba_rx_stru, us_reorder_index);
        pst_rx_buff->in_use = 0;
        pst_rx_buff->us_seq_num = 0;
        pst_rx_buff->uc_num_buf = 0;
        oal_netbuf_list_head_init(&(pst_rx_buff->st_netbuf_head));
    }

    /* ��ʼ��BAW */
    hmac_ba_rx_hdl_baw_init(pst_hmac_user, pst_ba_rx_stru, uc_tid);

    /* Ba�Ự������ʼ�� */
    pst_ba_rx_stru->us_baw_end = DMAC_BA_SEQ_ADD(pst_ba_rx_stru->us_baw_start, (pst_ba_rx_stru->us_baw_size - 1));
    pst_ba_rx_stru->us_baw_tail = DMAC_BA_SEQNO_SUB(pst_ba_rx_stru->us_baw_start, 1);
    pst_ba_rx_stru->us_baw_head = DMAC_BA_SEQNO_SUB(pst_ba_rx_stru->us_baw_start, HMAC_BA_BMP_SIZE);
    pst_ba_rx_stru->uc_mpdu_cnt = 0;
    pst_ba_rx_stru->en_is_ba = OAL_TRUE;  // Ba session is processing
    pst_ba_rx_stru->en_amsdu_supp = ((pst_hmac_vap->bit_rx_ampduplusamsdu_active & pst_hmac_vap->en_ps_rx_amsdu) ?
                                      OAL_TRUE : OAL_FALSE);
    pst_ba_rx_stru->en_back_var = MAC_BACK_COMPRESSED;
    pst_ba_rx_stru->puc_transmit_addr = pst_hmac_user->st_user_base_info.auc_user_mac_addr;

    /* ��ʼ����ʱ����Դ */
    pst_ba_rx_stru->st_alarm_data.pst_ba = pst_ba_rx_stru;
    pst_ba_rx_stru->st_alarm_data.us_mac_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_ba_rx_stru->st_alarm_data.uc_vap_id = pst_mac_vap->uc_vap_id;
    pst_ba_rx_stru->st_alarm_data.uc_tid = uc_tid;
    pst_ba_rx_stru->st_alarm_data.us_timeout_times = 0;
    pst_ba_rx_stru->st_alarm_data.en_direction = MAC_RECIPIENT_DELBA;
    pst_ba_rx_stru->en_timer_triggered = OAL_FALSE;

    oal_spin_lock_init(&pst_ba_rx_stru->st_ba_lock);
}

/* hw reorder window init */
void hmac_ba_rx_win_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t rx_tid)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_ba_rx_stru *ba_rx_win = NULL;
    uint16_t index;

    mac_vap = &(hmac_vap->st_vap_base_info);
    ba_rx_win = hmac_user->ast_tid_info[rx_tid].pst_ba_rx_info;

    /* 1. ��ʼ�����մ��ڣ� */
    /* 1.1 BA���մ��ڲ���(SSN�봰�ڴ�С)�ɹ������յ�BA����֡���ȡ��ͬ���������� */
    ba_rx_win->us_last_relseq = DMAC_BA_SEQNO_SUB(ba_rx_win->us_baw_start, 1);

    /* 1.2 �������ڲ���ͨ�������ȡ */
    ba_rx_win->us_baw_end = DMAC_BA_SEQ_ADD(ba_rx_win->us_baw_start, (ba_rx_win->us_baw_size - 1));
    ba_rx_win->us_baw_tail = DMAC_BA_SEQNO_SUB(ba_rx_win->us_baw_start, 1);
    ba_rx_win->us_baw_head = DMAC_BA_SEQNO_SUB(ba_rx_win->us_baw_start, HMAC_BA_BMP_SIZE);

    /* 1.3 ���������������г�ʼ�� */
    ba_rx_win->uc_mpdu_cnt = 0;

    oal_spin_lock_init(&ba_rx_win->st_ba_lock);
    oal_atomic_set(&ba_rx_win->ul_ref_cnt, 1);
    ba_rx_win->en_timer_triggered = OAL_FALSE;
    ba_rx_win->en_ba_status = DMAC_BA_INIT;

    for (index = 0; index < WLAN_AMPDU_RX_HE_BUFFER_SIZE; index++) {
        ba_rx_win->ast_re_order_list[index].in_use = 0;
        ba_rx_win->ast_re_order_list[index].us_seq_num = 0;
        ba_rx_win->ast_re_order_list[index].uc_num_buf  = 0; //  ����ɾ��  ?
        oal_netbuf_list_head_init(&(ba_rx_win->ast_re_order_list[index].st_netbuf_head));
    }

    ba_rx_win->en_back_var = MAC_BACK_COMPRESSED;
    ba_rx_win->puc_transmit_addr = hmac_user->st_user_base_info.auc_user_mac_addr;
    ba_rx_win->uc_mpdu_cnt = 0;
    ba_rx_win->en_is_ba = OAL_TRUE;  // Ba session is processing
    ba_rx_win->en_amsdu_supp = ((hmac_vap->bit_rx_ampduplusamsdu_active & hmac_vap->en_ps_rx_amsdu) ?
                                      OAL_TRUE : OAL_FALSE);
    /* ��ʼ����ʱ����Դ */
    ba_rx_win->st_alarm_data.pst_ba = ba_rx_win;
    ba_rx_win->st_alarm_data.us_mac_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    ba_rx_win->st_alarm_data.uc_vap_id = mac_vap->uc_vap_id;
    ba_rx_win->st_alarm_data.uc_tid = rx_tid;
    ba_rx_win->st_alarm_data.us_timeout_times = 0;
    ba_rx_win->st_alarm_data.en_direction = MAC_RECIPIENT_DELBA;
    ba_rx_win->en_timer_triggered = OAL_FALSE;

    memset_s(ba_rx_win->aul_rx_buf_bitmap, OAL_SIZEOF(ba_rx_win->aul_rx_buf_bitmap),
        0, OAL_SIZEOF(ba_rx_win->aul_rx_buf_bitmap));

    /* 1.4 ״̬��Ϊ��ʼ״̬ */
    ba_rx_win->en_ba_status = DMAC_BA_INIT;

    hmac_user->aus_tid_baw_size[rx_tid] = ba_rx_win->us_baw_size;
}



OAL_STATIC hmac_rx_buf_stru *hmac_ba_buffer_frame_in_reorder(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                             uint16_t us_seq_num,
                                                             mac_rx_ctl_stru *pst_cb_ctrl)
{
    uint16_t us_buf_index;
    hmac_rx_buf_stru *pst_rx_buf;

    us_buf_index = (us_seq_num & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    pst_rx_buf = HMAC_GET_BA_RX_DHL(pst_ba_rx_hdl, us_buf_index);
    if (pst_rx_buf->in_use == 1) {
        hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head, pst_rx_buf->uc_num_buf);
        pst_ba_rx_hdl->uc_mpdu_cnt--;
        pst_rx_buf->in_use = 0;
        pst_rx_buf->uc_num_buf = 0;
        oam_info_log1(0, OAM_SF_BA, "{hmac_ba_buffer_frame_in_reorder::slot already used, seq[%d].}", us_seq_num);
    }

    if (pst_cb_ctrl->bit_amsdu_enable == OAL_TRUE) {
        if (pst_cb_ctrl->bit_is_first_buffer == OAL_TRUE) {
            if (oal_netbuf_list_len(&pst_rx_buf->st_netbuf_head) != 0) {
                hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head,
                                         oal_netbuf_list_len(&pst_rx_buf->st_netbuf_head));
                oam_info_log1(0, OAM_SF_BA,
                    "{hmac_ba_buffer_frame_in_reorder::seq[%d] amsdu first buffer, need clear st_netbuf_head first}",
                    us_seq_num);
            }
            pst_rx_buf->uc_num_buf = 0;
        }

        /* offload��,amsdu֡��ɵ�֡�ֱ��ϱ� */
        pst_rx_buf->uc_num_buf += pst_cb_ctrl->bit_buff_nums;

        /* �������һ��amsdu buffer �ű��in use Ϊ 1 */
        if (pst_cb_ctrl->bit_is_last_buffer == OAL_TRUE) {
            pst_ba_rx_hdl->uc_mpdu_cnt++;
            pst_rx_buf->in_use = 1;
        } else {
            pst_rx_buf->in_use = 0;
        }
    } else {
        pst_rx_buf->uc_num_buf = pst_cb_ctrl->bit_buff_nums;
        pst_ba_rx_hdl->uc_mpdu_cnt++;
        pst_rx_buf->in_use = 1;
    }
    /* Update the buffered receive packet details */
    pst_rx_buf->us_seq_num = us_seq_num;
    pst_rx_buf->ul_rx_time = (uint32_t)oal_time_get_stamp_ms();

    return pst_rx_buf;
}


OAL_STATIC uint32_t hmac_ba_send_frames_with_gap(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                 oal_netbuf_head_stru *pst_netbuf_header,
                                                 uint16_t us_last_seqnum,
                                                 mac_vap_stru *pst_vap)
{
    uint8_t uc_num_frms = 0;
    uint16_t us_seq_num;
    hmac_rx_buf_stru *pst_rx_buf = OAL_PTR_NULL;
    uint8_t uc_loop_index;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;

    us_seq_num = pst_ba_rx_hdl->us_baw_start;

    oam_info_log1(pst_vap->uc_vap_id, OAM_SF_BA, "{hmac_ba_send_frames_with_gap::to seq[%d].}", us_last_seqnum);

    while (us_seq_num != us_last_seqnum) {
        pst_rx_buf = hmac_remove_frame_from_reorder_q(pst_ba_rx_hdl, us_seq_num);
        if (pst_rx_buf != OAL_PTR_NULL) {
            pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
            if (oal_unlikely(pst_netbuf == OAL_PTR_NULL)) {
                oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_BA,
                                 "{hmac_ba_send_frames_with_gap::gap[%d].\r\n}",
                                 us_seq_num);

                us_seq_num = DMAC_BA_SEQNO_ADD(us_seq_num, 1);
                pst_rx_buf->uc_num_buf = 0;

                continue;
            }

            for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
                pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
                oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);
                if (pst_netbuf != OAL_PTR_NULL) {
                    oal_netbuf_list_tail_nolock(pst_netbuf_header, pst_netbuf);
                }
            }
            pst_rx_buf->uc_num_buf = 0;
            uc_num_frms++;
        }

        us_seq_num = DMAC_BA_SEQNO_ADD(us_seq_num, 1);
    }

    oam_warning_log4(pst_vap->uc_vap_id, OAM_SF_BA,
        "{hmac_ba_send_frames_with_gap::old_baw_start[%d], new_baw_start[%d], uc_num_frms[%d], uc_mpdu_cnt=%d.}",
        pst_ba_rx_hdl->us_baw_start, us_last_seqnum, uc_num_frms, pst_ba_rx_hdl->uc_mpdu_cnt);

    return uc_num_frms;
}


OAL_STATIC uint16_t hmac_ba_send_frames_in_order(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                 oal_netbuf_head_stru *pst_netbuf_header,
                                                 mac_vap_stru *pst_vap)
{
    uint16_t us_seq_num;
    hmac_rx_buf_stru *pst_rx_buf = OAL_PTR_NULL;
    uint8_t uc_loop_index;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;

    us_seq_num = pst_ba_rx_hdl->us_baw_start;

    while ((pst_rx_buf = hmac_remove_frame_from_reorder_q(pst_ba_rx_hdl, us_seq_num)) != OAL_PTR_NULL) {
        us_seq_num = HMAC_BA_SEQNO_ADD(us_seq_num, 1);
        pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
        if (pst_netbuf == OAL_PTR_NULL) {
            oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_BA,
                             "{hmac_ba_send_frames_in_order::[%d] slot error.}",
                             us_seq_num);
            pst_rx_buf->uc_num_buf = 0;
            continue;
        }

        for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
            pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
            oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);
            if (pst_netbuf != OAL_PTR_NULL) {
                oal_netbuf_list_tail_nolock(pst_netbuf_header, pst_netbuf);
            }
        }

        pst_rx_buf->uc_num_buf = 0;
    }

    return us_seq_num;
}


OAL_STATIC OAL_INLINE void hmac_ba_buffer_rx_frame(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                   mac_rx_ctl_stru *pst_cb_ctrl,
                                                   oal_netbuf_head_stru *pst_netbuf_header,
                                                   uint16_t us_seq_num)
{
    hmac_rx_buf_stru *pst_rx_netbuf = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    uint8_t uc_netbuf_index;

    /* Get the pointer to the buffered packet */
    pst_rx_netbuf = hmac_ba_buffer_frame_in_reorder(pst_ba_rx_hdl, us_seq_num, pst_cb_ctrl);

    /* all buffers of this frame must be deleted from the buf list */
    for (uc_netbuf_index = pst_cb_ctrl->bit_buff_nums; uc_netbuf_index > 0; uc_netbuf_index--) {
        pst_netbuf = oal_netbuf_delist_nolock(pst_netbuf_header);

        oal_mem_netbuf_trace(pst_netbuf, OAL_TRUE);
        if (oal_unlikely(pst_netbuf != OAL_PTR_NULL)) {
            /* enqueue reorder queue */
            oal_netbuf_add_to_list_tail(pst_netbuf, &pst_rx_netbuf->st_netbuf_head);
        } else {
            oam_error_log0(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA, "{hmac_ba_buffer_rx_frame:netbuff error in amsdu.}");
        }
    }

    if (oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head) != pst_rx_netbuf->uc_num_buf) {
        oam_warning_log2(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA, "{hmac_ba_buffer_rx_frame: list_len=%d numbuf=%d}",
                         oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head), pst_rx_netbuf->uc_num_buf);
        pst_rx_netbuf->uc_num_buf = oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head);
    }
}


OAL_STATIC OAL_INLINE void hmac_ba_reorder_rx_data(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                   oal_netbuf_head_stru *pst_netbuf_header,
                                                   mac_vap_stru *pst_vap,
                                                   uint16_t us_seq_num)
{
    uint8_t uc_seqnum_pos;
    uint16_t us_temp_winstart;

    uc_seqnum_pos = hmac_ba_seqno_bound_chk(pst_ba_rx_hdl->us_baw_start, pst_ba_rx_hdl->us_baw_end, us_seq_num);
    if (uc_seqnum_pos == DMAC_BA_BETWEEN_SEQLO_SEQHI) {
        pst_ba_rx_hdl->us_baw_start = hmac_ba_send_frames_in_order(pst_ba_rx_hdl, pst_netbuf_header, pst_vap);
        pst_ba_rx_hdl->us_baw_end = DMAC_BA_SEQNO_ADD(pst_ba_rx_hdl->us_baw_start, (pst_ba_rx_hdl->us_baw_size - 1));
    } else if (uc_seqnum_pos == DMAC_BA_GREATER_THAN_SEQHI) {
        us_temp_winstart = HMAC_BA_SEQNO_SUB(us_seq_num, (pst_ba_rx_hdl->us_baw_size - 1));

        hmac_ba_send_frames_with_gap(pst_ba_rx_hdl, pst_netbuf_header, us_temp_winstart, pst_vap);
        pst_ba_rx_hdl->us_baw_start = us_temp_winstart;
        pst_ba_rx_hdl->us_baw_start = hmac_ba_send_frames_in_order(pst_ba_rx_hdl, pst_netbuf_header, pst_vap);
        pst_ba_rx_hdl->us_baw_end = HMAC_BA_SEQNO_ADD(pst_ba_rx_hdl->us_baw_start, (pst_ba_rx_hdl->us_baw_size - 1));
    } else {
        oam_warning_log3(pst_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_ba_reorder_rx_data::else branch seqno[%d] ws[%d] we[%d].}",
            us_seq_num, pst_ba_rx_hdl->us_baw_start, pst_ba_rx_hdl->us_baw_end);
    }
}


OAL_STATIC void hmac_ba_flush_reorder_q(hmac_ba_rx_stru *pst_rx_ba)
{
    hmac_rx_buf_stru *pst_rx_buf = OAL_PTR_NULL;
    uint16_t us_index;

    for (us_index = 0; us_index < WLAN_AMPDU_RX_HE_BUFFER_SIZE; us_index++) {
        pst_rx_buf = HMAC_GET_BA_RX_DHL(pst_rx_ba, us_index);
        if (pst_rx_buf->in_use == OAL_TRUE) {
            hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head, pst_rx_buf->uc_num_buf);
            pst_rx_buf->in_use = OAL_FALSE;
            pst_rx_buf->uc_num_buf = 0;
            pst_rx_ba->uc_mpdu_cnt--;
        }
    }

    if (pst_rx_ba->uc_mpdu_cnt != 0) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_ba_flush_reorder_q:: %d mpdu cnt left.}", pst_rx_ba->uc_mpdu_cnt);
    }
}

OAL_STATIC OAL_INLINE uint32_t hmac_ba_check_rx_aggr(mac_vap_stru *pst_vap,
                                                     mac_ieee80211_frame_stru *pst_frame_hdr)
{
    /* ��vap�Ƿ���ht */
    if (OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(pst_vap)) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_BA, "{hmac_ba_check_rx_aggr::ht not supported by this vap.}");
        return OAL_FAIL;
    }

    /* �жϸ�֡�ǲ���qos֡ */
    if ((WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA) != ((uint8_t *)pst_frame_hdr)[0]) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_BA, "{hmac_ba_check_rx_aggr::not qos data.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ba_need_update_hw_baw(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                                     uint16_t us_seq_num)
{
    if ((OAL_TRUE == hmac_ba_seqno_lt(us_seq_num, pst_ba_rx_hdl->us_baw_start)) &&
        (OAL_FALSE == hmac_ba_rx_seqno_lt(us_seq_num, pst_ba_rx_hdl->us_baw_start))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


uint32_t hmac_ba_filter_serv(hmac_user_stru *pst_hmac_user,
                             mac_rx_ctl_stru *pst_cb_ctrl,
                             oal_netbuf_head_stru *pst_netbuf_header,
                             oal_bool_enum_uint8 *pen_is_ba_buf)
{
    hmac_ba_rx_stru *pst_ba_rx_hdl = OAL_PTR_NULL;
    uint16_t us_seq_num;
    oal_bool_enum_uint8 en_is_4addr;
    uint8_t uc_is_tods;
    uint8_t uc_is_from_ds;
    uint8_t uc_tid;
    uint16_t us_baw_start_temp;
    uint32_t ul_ret;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_netbuf_header, pst_cb_ctrl, pen_is_ba_buf)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_filter_serv::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_cb_ctrl);
    if (oal_unlikely(pst_frame_hdr == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_filter_serv::pst_frame_hdr null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_cb_ctrl->uc_mac_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_filter_serv::pst_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    ul_ret = hmac_ba_check_rx_aggr(pst_mac_vap, pst_frame_hdr);
    if (ul_ret != OAL_SUCC) {
        return OAL_SUCC;
    }

    /* �����ĵ�ַ�����ȡ���ĵ�tid */
    uc_is_tods = mac_hdr_get_to_ds((uint8_t *)pst_frame_hdr);
    uc_is_from_ds = mac_hdr_get_from_ds((uint8_t *)pst_frame_hdr);
    en_is_4addr = uc_is_tods && uc_is_from_ds;
    uc_tid = mac_get_tid_value((uint8_t *)pst_frame_hdr, en_is_4addr);

    pst_ba_rx_hdl = pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info;
    if (pst_ba_rx_hdl == OAL_PTR_NULL) {
        return OAL_SUCC;
    }
    if (pst_ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        oam_warning_log1(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA,
                         "{hmac_ba_filter_serv::ba_status = %d.",
                         pst_ba_rx_hdl->en_ba_status);
        return OAL_SUCC;
    }

    /* ��ʱ����BA���ڵ����кţ����ڼ����Ƿ���֡�ϱ� */
    us_baw_start_temp = pst_ba_rx_hdl->us_baw_start;

    us_seq_num = mac_get_seq_num((uint8_t *)pst_frame_hdr);

    if ((oal_bool_enum_uint8)pst_frame_hdr->st_frame_control.bit_more_frag == OAL_TRUE) {
        oam_warning_log1(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA,
                         "{hmac_ba_filter_serv::We get a frag_frame[seq_num=%d] When BA_session is set UP!",
                         us_seq_num);
        return OAL_SUCC;
    }

    /* duplicate frame�ж� */
    if (OAL_TRUE == hmac_ba_rx_seqno_lt(us_seq_num, pst_ba_rx_hdl->us_baw_start)) {
        /* �ϴηǶ�ʱ���ϱ���ֱ��ɾ��duplicate frame֡������ֱ���ϱ� */
        if (pst_ba_rx_hdl->en_timer_triggered == OAL_FALSE) {
            /* ȷʵ�Ѿ��յ���֡ */
            if (hmac_ba_isset(pst_ba_rx_hdl, us_seq_num)) {
                HMAC_USER_STATS_PKT_INCR(pst_hmac_user->ul_rx_pkt_drop, 1);
                return OAL_FAIL;
            }
        }

        return OAL_SUCC;
    }

    /* ����tail */
    if (OAL_TRUE == hmac_ba_seqno_lt(pst_ba_rx_hdl->us_baw_tail, us_seq_num)) {
        pst_ba_rx_hdl->us_baw_tail = us_seq_num;
    }

    /* ���յ���֡�����кŵ���BAW_START�����һ������֡����Ϊ0����ֱ���ϱ���HMAC */
    if ((us_seq_num == pst_ba_rx_hdl->us_baw_start) && (pst_ba_rx_hdl->uc_mpdu_cnt == 0)
        /* offload ��amsdu֡���ڿ��ܶ��buffer��ɣ�һ���������� */
        && (pst_cb_ctrl->bit_amsdu_enable == OAL_FALSE)
    ) {
        pst_ba_rx_hdl->us_baw_start = DMAC_BA_SEQNO_ADD(pst_ba_rx_hdl->us_baw_start, 1);
        pst_ba_rx_hdl->us_baw_end = DMAC_BA_SEQNO_ADD(pst_ba_rx_hdl->us_baw_end, 1);
    } else {
        /* Buffer the new MSDU */
        *pen_is_ba_buf = OAL_TRUE;

        /* buffer frame to reorder */
        hmac_ba_buffer_rx_frame(pst_ba_rx_hdl, pst_cb_ctrl, pst_netbuf_header, us_seq_num);

        /* put the reordered netbufs to the end of the list */
        hmac_ba_reorder_rx_data(pst_ba_rx_hdl, pst_netbuf_header, pst_mac_vap, us_seq_num);

        /* Check for Sync loss and flush the reorder queue when one is detected */
        if ((pst_ba_rx_hdl->us_baw_tail == DMAC_BA_SEQNO_SUB(pst_ba_rx_hdl->us_baw_start, 1)) &&
            (pst_ba_rx_hdl->uc_mpdu_cnt > 0)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                             "{hmac_ba_filter_serv::Sync loss and flush the reorder queue.}");
            hmac_ba_flush_reorder_q(pst_ba_rx_hdl);
        }

        /* ���������ˢ�º�,�����������֡��ô������ʱ�� */
        if (pst_ba_rx_hdl->uc_mpdu_cnt > 0) {
            hmac_reorder_ba_timer_start(pst_hmac_vap, pst_hmac_user, uc_tid);
        }
    }

    if (us_baw_start_temp != pst_ba_rx_hdl->us_baw_start) {
        pst_ba_rx_hdl->en_timer_triggered = OAL_FALSE;
    }

    return OAL_SUCC;
}


void hmac_reorder_ba_rx_buffer_bar(hmac_ba_rx_stru *pst_rx_ba, uint16_t us_start_seq_num,
    mac_vap_stru *pst_vap)
{
    oal_netbuf_head_stru st_netbuf_head;
    uint8_t uc_seqnum_pos;

    if (pst_rx_ba == NULL) {
        oam_warning_log0(0, OAM_SF_BA, "{hmac_reorder_ba_rx_buffer_bar::rcv a bar, but ba not set up.}");
        return;
    }

    /* ��� BAR ��SSN�ʹ��ڵ�start_num���ʱ������Ҫ�ƴ� */
    if (pst_rx_ba->us_baw_start == us_start_seq_num) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_reorder_ba_rx_buffer_bar::seq is equal to start num.}");
        return;
    }

    oal_netbuf_list_head_init(&st_netbuf_head);

    uc_seqnum_pos = hmac_ba_seqno_bound_chk(pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    /* ���BAR�ĵ�SSN�ڴ����ڲ��ƴ� */
    if (uc_seqnum_pos == DMAC_BA_BETWEEN_SEQLO_SEQHI) {
        hmac_ba_send_frames_with_gap(pst_rx_ba, &st_netbuf_head, us_start_seq_num, pst_vap);
        pst_rx_ba->us_baw_start = us_start_seq_num;
        pst_rx_ba->us_baw_start = hmac_ba_send_frames_in_order(pst_rx_ba, &st_netbuf_head, pst_vap);
        pst_rx_ba->us_baw_end = HMAC_BA_SEQNO_ADD(pst_rx_ba->us_baw_start, (pst_rx_ba->us_baw_size - 1));

        oam_warning_log3(pst_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::receive a bar, us_baw_start=%d us_baw_end=%d. us_seq_num=%d.}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
        hmac_rx_lan_frame(&st_netbuf_head);
    } else if (uc_seqnum_pos == DMAC_BA_GREATER_THAN_SEQHI) {
        /* �쳣 */
        oam_warning_log3(pst_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::recv a bar and ssn out of winsize,baw_start=%d baw_end=%d seq_num=%d}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    } else {
        oam_warning_log3(pst_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::recv a bar ssn less than baw_start,baw_start=%d end=%d seq_num=%d}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    }
}


OAL_STATIC uint32_t hmac_ba_rx_prepare_bufflist(hmac_vap_stru *pst_hmac_vap,
                                                hmac_rx_buf_stru *pst_rx_buf,
                                                oal_netbuf_head_stru *pst_netbuf_head)
{
    oal_netbuf_stru *pst_netbuf;
    uint8_t uc_loop_index;

    pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
    if (pst_netbuf == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_ba_rx_prepare_bufflist::pst_netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
        pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
        if (pst_netbuf != OAL_PTR_NULL) {
            oal_netbuf_add_to_list_tail(pst_netbuf, pst_netbuf_head);
        } else {
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                             "{hmac_ba_rx_prepare_bufflist::uc_num_buf in reorder list is error.}");
        }
    }

    return OAL_SUCC;
}


uint32_t hmac_ba_send_reorder_timeout(hmac_ba_rx_stru *pst_rx_ba, hmac_vap_stru *pst_hmac_vap,
    hmac_ba_alarm_stru *pst_alarm_data, uint32_t *timeout)
{
    uint32_t ul_time_diff, ul_rx_timeout, ul_ret;
    oal_netbuf_head_stru st_netbuf_head;
    uint16_t us_baw_head, us_baw_end, us_baw_start; /* ��������Ĵ�����ʼ���к� */
    hmac_rx_buf_stru *pst_rx_buf = OAL_PTR_NULL;
    uint8_t uc_buff_count = 0;
    uint16_t uc_send_count = 0;

    oal_netbuf_list_head_init(&st_netbuf_head);
    us_baw_head = pst_rx_ba->us_baw_start;
    us_baw_start = pst_rx_ba->us_baw_start;
    us_baw_end = HMAC_BA_SEQNO_ADD(pst_rx_ba->us_baw_tail, 1);
    ul_rx_timeout = (*timeout);

    oal_spin_lock(&pst_rx_ba->st_ba_lock);

    while (us_baw_head != us_baw_end) {
        pst_rx_buf = hmac_get_frame_from_reorder_q(pst_rx_ba, us_baw_head);
        if (pst_rx_buf == OAL_PTR_NULL) {
            uc_buff_count++;
            us_baw_head = HMAC_BA_SEQNO_ADD(us_baw_head, 1);
            continue;
        }

        /* �������������е�֡����ʱ��С�ڶ�ʱ����ʱʱ��,��ô��ʱ��ǿ��flush */
        ul_time_diff = (uint32_t)oal_time_get_stamp_ms() - pst_rx_buf->ul_rx_time;
        if (ul_time_diff < ul_rx_timeout) {
            *timeout = (ul_rx_timeout - ul_time_diff);
            break;
        }

        pst_rx_ba->uc_mpdu_cnt--;
        pst_rx_buf->in_use = 0;

        ul_ret = hmac_ba_rx_prepare_bufflist(pst_hmac_vap, pst_rx_buf, &st_netbuf_head);
        if (ul_ret != OAL_SUCC) {
            uc_buff_count++;
            us_baw_head = HMAC_BA_SEQNO_ADD(us_baw_head, 1);
            continue;
        }

        uc_send_count++;
        uc_buff_count++;
        us_baw_head = HMAC_BA_SEQNO_ADD(us_baw_head, 1);
        pst_rx_ba->us_baw_start = HMAC_BA_SEQNO_ADD(pst_rx_ba->us_baw_start, uc_buff_count);
        pst_rx_ba->us_baw_end = HMAC_BA_SEQNO_ADD(pst_rx_ba->us_baw_start, (pst_rx_ba->us_baw_size - 1));

        uc_buff_count = 0;
    }

    oal_spin_unlock(&pst_rx_ba->st_ba_lock);

    /* �жϱ��ζ�ʱ����ʱ�Ƿ���֡�ϱ� */
    if (us_baw_start != pst_rx_ba->us_baw_start) {
        pst_rx_ba->en_timer_triggered = OAL_TRUE;
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
            "{hmac_ba_send_reorder_timeout::new baw_start:%d, old baw_start:%d, ul_rx_timeout:%d, send mpdu cnt:%d.}",
            pst_rx_ba->us_baw_start, us_baw_start, ul_rx_timeout, uc_send_count);
    }

    hmac_rx_lan_frame(&st_netbuf_head);

    return OAL_SUCC;
}


uint32_t hmac_ba_timeout_fn(void *p_arg)
{
    hmac_ba_rx_stru *pst_rx_ba = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_ba_alarm_stru *alarm_data = NULL;
    mac_delba_initiator_enum_uint8 en_direction;
    uint8_t rx_tid;
    mac_device_stru *mac_device = NULL;
    uint32_t timeout;

    alarm_data = (hmac_ba_alarm_stru *)p_arg;
    en_direction = alarm_data->en_direction;
    rx_tid = alarm_data->uc_tid;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(alarm_data->uc_vap_id);
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(alarm_data->us_mac_user_idx);
    if (OAL_TRUE != hmac_ba_timeout_param_is_valid(alarm_data, hmac_vap, hmac_user, rx_tid)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (mac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (en_direction == MAC_RECIPIENT_DELBA) {
        pst_rx_ba = (hmac_ba_rx_stru *)alarm_data->pst_ba;
        if (pst_rx_ba == NULL) {
            oam_error_log0(0, OAM_SF_BA, "{hmac_ba_timeout_fn::pst_rx_ba is null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* ����ҵ��������ʱֻ�ܿ���ʱ��ʱ����ˢ���������,Ϊ������Ϸ֡��ʱ,��Ҫ����ʱʱ����С */
        if (OAL_FALSE == hmac_wifi_rx_is_busy()) {
            timeout = hmac_vap->us_rx_timeout_min[WLAN_WME_TID_TO_AC(rx_tid)];
        } else {
            timeout = hmac_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(rx_tid)];
        }

        if (pst_rx_ba->uc_mpdu_cnt > 0) {
            wlan_chip_ba_send_reorder_timeout(pst_rx_ba, hmac_vap, alarm_data, &timeout);
        }

        /* �����������ˢ�º�,��Ȼ�л���֡����Ҫ������ʱ��;
           �������������֡��Ϊ�˽�ʡ���Ĳ�������ʱ��,����֡���ʱ���� */
        if (pst_rx_ba->uc_mpdu_cnt > 0) {
            oal_spin_lock(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer_lock));
            /* �˴�����Ҫ�ж϶�ʱ���Ƿ��Ѿ�����,���δ������������ʱ��;
               ����˶�ʱ���Ѿ����� */
            frw_timer_create_timer_m(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer),
                hmac_ba_timeout_fn, timeout, alarm_data,
                OAL_FALSE, OAM_MODULE_ID_HMAC, mac_device->ul_core_id);
            oal_spin_unlock(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer_lock));
        }
    } else {
        /* tx ba��ɾ�� */
        frw_timer_create_timer_m(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer),
            hmac_ba_timeout_fn, hmac_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(rx_tid)],
            alarm_data, OAL_FALSE, OAM_MODULE_ID_HMAC, mac_device->ul_core_id);
    }

    return OAL_SUCC;
}


uint32_t hmac_ba_reset_rx_handle(mac_device_stru *pst_mac_device,
                                 hmac_user_stru *pst_hmac_user, uint8_t uc_tid,
                                 oal_bool_enum_uint8 en_is_aging)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;
    oal_bool_enum en_need_del_lut = OAL_TRUE;
    hmac_ba_rx_stru *pst_ba_rx_info = pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info;

    if (oal_unlikely((pst_ba_rx_info == OAL_PTR_NULL) || (pst_ba_rx_info->en_is_ba != OAL_TRUE))) {
        oam_warning_log0(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::rx ba not set yet.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_tid >= WLAN_TID_MAX_NUM) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::tid %d overflow.}", uc_tid);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_ba_rx_info->st_alarm_data.uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::pst_hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* Step1: disable the flag of ba session */
    pst_ba_rx_info->en_is_ba = OAL_FALSE;

    /* Step2: flush reorder q */
    hmac_ba_flush_reorder_q(pst_ba_rx_info);

    if (pst_ba_rx_info->uc_lut_index == MAC_INVALID_RX_BA_LUT_INDEX) {
        en_need_del_lut = OAL_FALSE;
        oam_warning_log1(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::no need to del lut index, lut index[%d]}\n",
                         pst_ba_rx_info->uc_lut_index);
    }

    /* Step3: if lut index is valid, del lut index alloc before */
    if ((pst_ba_rx_info->uc_ba_policy == MAC_BA_POLICY_IMMEDIATE) && (en_need_del_lut == OAL_TRUE)) {
        pst_mac_chip = hmac_res_get_mac_chip(pst_mac_device->uc_chip_id);
        if (pst_mac_chip == OAL_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }
        hmac_ba_del_lut_index(pst_mac_chip->st_lut_table.auc_rx_ba_lut_idx_table, pst_ba_rx_info->uc_lut_index);
    }

    /* Step4: dec the ba session cnt maitence in device struc */
    hmac_rx_ba_session_decr(pst_hmac_vap, uc_tid);

    oal_spin_lock(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer_lock));
    /* Step5: Del Timer */
    if (pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer));
    }
    oal_spin_unlock(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer_lock));

    /* Step6: Free rx handle */
    oal_mem_free_m(pst_ba_rx_info, OAL_TRUE);

    pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info = OAL_PTR_NULL;

    return OAL_SUCC;
}


uint8_t hmac_mgmt_check_set_rx_ba_ok(hmac_vap_stru *pst_hmac_vap,
                                     hmac_user_stru *pst_hmac_user,
                                     hmac_ba_rx_stru *pst_ba_rx_info,
                                     mac_device_stru *pst_device,
                                     hmac_tid_stru *pst_tid_info)
{
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;
    uint8_t ba_status;

    pst_ba_rx_info->uc_lut_index = MAC_INVALID_RX_BA_LUT_INDEX;

    /* ������ȷ���ж� */
    if (pst_ba_rx_info->uc_ba_policy == MAC_BA_POLICY_IMMEDIATE) {
        if (OAL_FALSE == mac_mib_get_dot11ImmediateBlockAckOptionImplemented(&pst_hmac_vap->st_vap_base_info)) {
            /* ��֧��������ȷ�� */
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                             "{hmac_mgmt_check_set_rx_ba_ok::not support immediate Block Ack.}");
            return MAC_INVALID_REQ_PARAMS;
        } else {
            if (pst_ba_rx_info->en_back_var != MAC_BACK_COMPRESSED) {
                /* ��֧�ַ�ѹ����ȷ�� */
                oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                                 "{hmac_mgmt_check_set_rx_ba_ok::not support non-Compressed Block Ack.}");
                return MAC_REQ_DECLINED;
            }
        }
    } else if (pst_ba_rx_info->uc_ba_policy == MAC_BA_POLICY_DELAYED) {
        /* �ӳٿ�ȷ�ϲ�֧�� */
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::not support delayed Block Ack.}");
        return MAC_INVALID_REQ_PARAMS;
    }
    pst_mac_chip = hmac_res_get_mac_chip(pst_device->uc_chip_id);
    if (pst_mac_chip == OAL_PTR_NULL) {
        return MAC_REQ_DECLINED;
    }
    if (mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info) > WLAN_MAX_RX_BA) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::uc_rx_ba_session_num[%d] is up to max.}\r\n",
                         mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info));
        return MAC_REQ_DECLINED;
    }

    /* ��ȡBA LUT INDEX */
    pst_ba_rx_info->uc_lut_index = hmac_ba_get_lut_index(pst_mac_chip->st_lut_table.auc_rx_ba_lut_idx_table,
                                                         0, HAL_MAX_RX_BA_LUT_SIZE);

    if (pst_ba_rx_info->uc_lut_index == MAC_INVALID_RX_BA_LUT_INDEX &&
        wlan_chip_ba_need_check_lut_idx() == OAL_TRUE) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_check_set_rx_ba_ok::ba lut index table full.");
        return MAC_REQ_DECLINED;
    }
    ba_status = hmac_btcoex_check_addba_req(pst_hmac_vap, pst_hmac_user);
    if (ba_status != MAC_SUCCESSFUL_STATUSCODE) {
        return ba_status;
    }
    /* ��tid�²�����BA�������������� */
    if (pst_tid_info->en_ba_handle_rx_enable == OAL_FALSE) {
        oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::uc_tid_no[%d] user_idx[%d] is not available.}",
                         pst_tid_info->uc_tid_no, pst_tid_info->us_hmac_user_idx);
        return MAC_REQ_DECLINED;
    }
    return MAC_SUCCESSFUL_STATUSCODE;
}


void hmac_up_rx_bar(hmac_vap_stru *pst_hmac_vap, dmac_rx_ctl_stru *pst_rx_ctl, oal_netbuf_stru *pst_netbuf)
{
    uint8_t *puc_payload = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    uint8_t *puc_sa_addr = OAL_PTR_NULL;
    uint8_t uc_tidno;
    hmac_user_stru *pst_ta_user = OAL_PTR_NULL;
    uint16_t us_start_seqnum;
    hmac_ba_rx_stru *pst_ba_rx_info = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint16_t us_frame_len;

    if (wlan_chip_check_need_process_bar() == OAL_FALSE) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_up_rx_bar: not need process bar, return.}");
        return;
    }

    pst_frame_hdr = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr(&(pst_rx_ctl->st_rx_info));
    puc_sa_addr = pst_frame_hdr->auc_address2;
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctl->st_rx_info));
    us_frame_len = pst_rx_info->us_frame_len - pst_rx_info->uc_mac_header_len;

    /*  ��ȡ�û�ָ�� */
    pst_ta_user = mac_vap_get_hmac_user_by_addr(&(pst_hmac_vap->st_vap_base_info), puc_sa_addr);
    if (pst_ta_user == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_up_rx_bar::pst_ta_user  is null.}");
        return;
    }

    /* BAR Control(2) +  BlockAck Starting seq num)(2) */
    if (us_frame_len < 4) {  /* mac header֡ͷ����С��4��֡��ʽ���� */
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_up_rx_bar:frame len err. frame len[%d], machdr len[%d].}",
            pst_rx_info->us_frame_len, pst_rx_info->uc_mac_header_len);
        return;
    }

    /* ��ȡ֡ͷ��payloadָ�� */
    puc_payload = MAC_GET_RX_PAYLOAD_ADDR(&(pst_rx_ctl->st_rx_info), pst_netbuf);

    /*************************************************************************/
    /*                     BlockAck Request Frame Format                     */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BAR Control|BlockAck Starting    |FCS|  */
    /* |             |        |  |  |           |Sequence number      |   |  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |2          |2                    |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    uc_tidno = (puc_payload[1] & 0xF0) >> 4; /* ȡ4~7bitλΪTID������ */
    if (uc_tidno >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_up_rx_bar::uc_tidno[%d] invalid.}", uc_tidno);
        return;
    }
    us_start_seqnum = mac_get_bar_start_seq_num(puc_payload);
    pst_ba_rx_info = pst_ta_user->ast_tid_info[uc_tidno].pst_ba_rx_info;

    hmac_reorder_ba_rx_buffer_bar(pst_ba_rx_info, us_start_seqnum, &(pst_hmac_vap->st_vap_base_info));
}

oal_bool_enum_uint8 hmac_is_device_ba_setup(void)
{
    uint8_t uc_vap_id;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_is_device_ba_setup pst_mac_vap is null.}");
            continue;
        }
        if ((pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) &&
            (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_PAUSE)) {
            continue;
        }
        if ((mac_mib_get_TxBASessionNumber(&pst_hmac_vap->st_vap_base_info) != 0) ||
            (mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info) != 0)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


uint32_t hmac_ps_rx_delba(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_cfg_delba_req_param_stru st_mac_cfg_delba_param;
    dmac_to_hmac_ps_rx_delba_event_stru *pst_dmac_to_hmac_ps_rx_delba = OAL_PTR_NULL;
    uint32_t ul_ret;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_ps_rx_delba::pst_hmac_vap is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_to_hmac_ps_rx_delba = (dmac_to_hmac_ps_rx_delba_event_stru *)puc_param;

    pst_hmac_user = mac_res_get_hmac_user(pst_dmac_to_hmac_ps_rx_delba->us_user_id);
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_ps_rx_delba::pst_hmac_user is null! user_id is %d.}",
                         pst_dmac_to_hmac_ps_rx_delba->us_user_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ˢ��HMAC VAP��PSҵ��ʱRX AMSDUʹ�ܱ�־ */
    pst_mac_vap->en_ps_rx_amsdu = pst_dmac_to_hmac_ps_rx_delba->en_rx_amsdu;
    pst_mac_vap->uc_ps_type |= pst_dmac_to_hmac_ps_rx_delba->uc_ps_type;
    /* ��֤�������ȼ���� */
    if (pst_mac_vap->uc_ps_type & MAC_PS_TYPE_CMD) {
        return OAL_SUCC;
    }
    pst_hmac_vap->en_ps_rx_amsdu = pst_mac_vap->en_ps_rx_amsdu;
    memset_s((uint8_t *)&st_mac_cfg_delba_param, OAL_SIZEOF(st_mac_cfg_delba_param),
             0, OAL_SIZEOF(st_mac_cfg_delba_param));
    oal_set_mac_addr(st_mac_cfg_delba_param.auc_mac_addr, pst_hmac_user->st_user_base_info.auc_user_mac_addr);
    st_mac_cfg_delba_param.en_direction = MAC_RECIPIENT_DELBA;
    st_mac_cfg_delba_param.en_trigger = MAC_DELBA_TRIGGER_PS;

    for (st_mac_cfg_delba_param.uc_tidno = 0;
         st_mac_cfg_delba_param.uc_tidno < WLAN_TID_MAX_NUM; st_mac_cfg_delba_param.uc_tidno++) {
        ul_ret = hmac_config_delba_req(pst_mac_vap, 0, (uint8_t *)&st_mac_cfg_delba_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                "{hmac_ps_rx_delba::delba send failed, ul_ret: %d, tid: %d}",
                ul_ret, st_mac_cfg_delba_param.uc_tidno);
            return ul_ret;
        }
    }
    return OAL_SUCC;
}


void hmac_ba_rx_update_release_seqnum(hmac_ba_rx_stru *ba_rx_hdl, uint16_t release_sn)
{
    if (oal_unlikely((ba_rx_hdl == NULL) || (release_sn > MAX_BA_SN))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ba_rx_update_release_seqnum::ba_rx_hdl[%p], release_sn[%d].}",
            (uintptr_t)ba_rx_hdl, release_sn);
        return;
    }

    if ((HMAC_BAW_RIGHTSIDE(ba_rx_hdl->us_last_relseq, release_sn)) || (ba_rx_hdl->us_last_relseq == release_sn)) {
        ba_rx_hdl->us_last_relseq = release_sn;
        return;
    } else {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_ba_rx_update_release_seqnum::sw req update wrong release sn.}");
        return;
    }
}


OAL_STATIC uint32_t hmac_ba_rx_prepare_buffer(hmac_vap_stru *hmac_vap,
    hmac_rx_buf_stru *rx_entry, oal_netbuf_head_stru *pnetbuf_head)
{
    oal_netbuf_stru    *netbuf = OAL_PTR_NULL;
    mac_rx_ctl_stru    *rx_ctl = OAL_PTR_NULL;

    if (oal_any_null_ptr2(hmac_vap, pnetbuf_head)) {
        oam_error_log2(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::hmac_vap[%p], pnetbuf_head[%p].}",
            (uintptr_t)hmac_vap, (uintptr_t)pnetbuf_head);
        return OAL_FAIL;
    }

    if (rx_entry == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::rx_reo_entry[%p].}", (uintptr_t)rx_entry);
        return OAL_FAIL;
    }

    if (oal_netbuf_list_empty(&rx_entry->st_netbuf_head)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::netbuf_head is empty.}");
        return OAL_FAIL;
    }

    /* ��˳��������֡ */
    while (!oal_netbuf_list_empty(&(rx_entry->st_netbuf_head))) {
        netbuf = oal_netbuf_delist(&(rx_entry->st_netbuf_head));
        if (netbuf == NULL) {
            oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_prepare_bufflist::netbuf null.}");
            return OAL_FAIL;
        }

        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        rx_ctl->bit_is_reo_timeout = OAL_TRUE;
        oal_netbuf_add_to_list_tail(netbuf, pnetbuf_head);
    }

    return OAL_SUCC;
}


uint32_t  hmac_ba_check_rx_aggrate(hmac_vap_stru *hmac_vap, mac_rx_ctl_stru *rx_ctl)
{
    uint8_t    frame_type;

    if (oal_unlikely(oal_any_null_ptr2(hmac_vap, rx_ctl))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ba_check_rx_aggr::hmac_vap[%p], rx_ctl[%p].}",
            (uintptr_t)hmac_vap, (uintptr_t)rx_ctl);
        return OAL_ERR_CODE_PTR_NULL;
    }

    frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);
    /* �жϸ�֡�ǲ���qos֡ */
    if (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_ba_check_rx_aggr::not qos data.}");
        return OAL_FAIL;
    }
    /* ����STA����Ҫ�ж��Ƿ��ǹ㲥֡�����Ǽ���ĳЩ�豸�㲥֡��qos�� */
    if ((hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (rx_ctl->bit_mcast_bcast == OAL_TRUE)) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_ba_check_rx_aggr::This is a multicast data frame.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


void hmac_ba_rx_hdl_hold(hmac_ba_rx_stru *ba_rx_hdl)
{
    oal_atomic_inc(&ba_rx_hdl->ul_ref_cnt);
}


void hmac_ba_rx_hdl_put(hmac_ba_rx_stru *ba_rx_hdl)
{
    oal_atomic_dec(&ba_rx_hdl->ul_ref_cnt);
}

OAL_STATIC uint32_t hmac_rx_frm_diff_time(hmac_rx_buf_stru *rx_buf, uint32_t curr_time)
{
    uint32_t time_diff;

    if (curr_time >= rx_buf->ul_rx_time) {
        time_diff = curr_time - rx_buf->ul_rx_time;
    } else {
        time_diff = (HMAC_U32_MAX - rx_buf->ul_rx_time) + curr_time;
    }

    return time_diff;
}

OAL_STATIC uint16_t hmac_rx_ba_get_baw_head(hmac_ba_rx_stru *rx_ba, uint16_t baw_head)
{
    return (HMAC_BAW_RIGHTSIDE(baw_head, HMAC_BA_SEQNO_ADD(rx_ba->us_last_relseq, 1))) ? baw_head :
        HMAC_BA_SEQNO_ADD(rx_ba->us_last_relseq, 1);
}


uint32_t hmac_ba_send_ring_reorder_timeout(hmac_ba_rx_stru *rx_ba,
    hmac_vap_stru *hmac_vap, hmac_ba_alarm_stru *alarm_data, uint32_t *p_timeout)
{
    uint32_t             curr_time, time_diff, rx_timeout;
    uint16_t             baw_head, baw_start, baw_end;
    uint32_t             ret;
    oal_netbuf_head_stru netbuf_head;
    hmac_rx_buf_stru    *rx_buf = NULL;

    oal_netbuf_list_head_init(&netbuf_head);
    baw_head = rx_ba->us_baw_start;
    baw_start = rx_ba->us_baw_start;
    rx_timeout = g_ba_rx_timeout_len[WLAN_WME_TID_TO_AC(alarm_data->uc_tid)];
    baw_end = HMAC_BA_SEQNO_ADD(rx_ba->us_baw_tail, 1);

    if (baw_head != HMAC_BA_SEQNO_ADD(rx_ba->us_last_relseq, 1)) {
        /* baw_start eq. us_last_relseq + 1 �ӽϿ����Ŀ�ʼ�жϳ�ʱ */
        oam_warning_log2(0, 0, "{hmac_ba_send_reo:baw_start[%d]!=last_relseq[%d]+1}", baw_head, rx_ba->us_last_relseq);
        baw_head = hmac_rx_ba_get_baw_head(rx_ba, baw_head);
    }

    curr_time = (uint32_t)oal_time_get_stamp_ms();
    while (baw_head != baw_end) {
        rx_buf = hmac_get_frame_from_reorder_q(rx_ba, baw_head);
        if (rx_buf == OAL_PTR_NULL) {
            baw_head = HMAC_BA_SEQNO_ADD(baw_head, 1);
            continue;
        }

        /* ��ȡ��ǰʱ����յ�����ʱ�Ĳ�ֵ����ֵ������ڶ�ʱ��timer����release���� */
        time_diff = hmac_rx_frm_diff_time(rx_buf, curr_time);
        if (time_diff < rx_timeout) {
            *p_timeout = rx_timeout - time_diff;
            break;
        }
        rx_buf = hmac_remove_frame_from_reorder_q(rx_ba, baw_head);
        ret = hmac_ba_rx_prepare_buffer(hmac_vap, rx_buf, &netbuf_head);

        /* timeout�ϱ�������check����Ƿ���Ҫ�����ϱ�����֡����Ϊ����rx_ba->us_baw_start֮ǰ�Ļ���֡һ�����ڴ� */
        /* baw_start handler�ĵط�����ˣ��˴�ֻ�����last release sequence number */
        hmac_ba_rx_update_release_seqnum(rx_ba, baw_head);
        if (ret != OAL_SUCC) {
            baw_head = HMAC_BA_SEQNO_ADD(baw_head, 1);
            continue;
        }
        baw_head = HMAC_BA_SEQNO_ADD(baw_head, 1);
    }
    rx_ba->us_baw_start = baw_head;

    if (baw_start != rx_ba->us_baw_start) {
        rx_ba->en_timer_triggered = OAL_TRUE;
    }
    /* To kernel or to wlan */
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_rx_data_sta_proc(&netbuf_head);
    }
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_data_ap_proc(hmac_vap, &netbuf_head);
    }
    return OAL_SUCC;
}


OAL_STATIC hmac_rx_buf_stru *hmac_ba_buffer_frame_in_reodr(hmac_ba_rx_stru *ba_rx_hdl,
    uint16_t sn, oal_netbuf_head_stru *pnetbuf_head)
{
    uint16_t        buf_index;
    hmac_rx_buf_stru *rx_buf = OAL_PTR_NULL;

    buf_index = (sn & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    rx_buf = &(ba_rx_hdl->ast_re_order_list[buf_index]);
    if (rx_buf == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    if (rx_buf->in_use) {
        hmac_rx_free_netbuf_list(&rx_buf->st_netbuf_head, rx_buf->uc_num_buf);
        ba_rx_hdl->uc_mpdu_cnt--;
        rx_buf->in_use = 0;
        rx_buf->uc_num_buf = 0;
        oam_info_log1(0, OAM_SF_BA, "{hmac_ba_buffer_frame_in_reorder::slot already used, seq[%d].}", sn);
    }

    ba_rx_hdl->uc_mpdu_cnt++;
    rx_buf->in_use = OAL_TRUE;

    return rx_buf;
}


OAL_STATIC void hmac_ba_add_netbuf_to_reo_list(hmac_ba_rx_stru *ba_rx_hdl,
    oal_netbuf_stru *netbuf_mpdu, oal_netbuf_head_stru *netbuf_head)
{
    mac_rx_ctl_stru  *rx_ctl = OAL_PTR_NULL;
    hmac_rx_buf_stru *rx_reo_entry = OAL_PTR_NULL;

    if (oal_any_null_ptr2(ba_rx_hdl, netbuf_mpdu)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ba_add_netbuf_to_reo_list::ba_rx_hdl[%p] netbuf_mpdu[%p].}",
            (uintptr_t)ba_rx_hdl, (uintptr_t)netbuf_mpdu);
        return;
    }

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
    rx_reo_entry = hmac_ba_buffer_frame_in_reodr(ba_rx_hdl, MAC_GET_RX_CB_SEQ_NUM(rx_ctl), netbuf_head);
    if (rx_reo_entry == OAL_PTR_NULL) {
        return;
    }

    /*
    * �ڱ��Ļ��浽���ջ�����е�ʱ�򣬶�����ÿ��Ԫ�ذ�������skb�ṹ��
    * �����amsdu����������msdu��skb����ʽ����
    **/
    rx_reo_entry->us_seq_num = MAC_GET_RX_CB_SEQ_NUM(rx_ctl);
    rx_reo_entry->ul_rx_time = (uint32_t)oal_time_get_stamp_ms();
    oal_netbuf_list_head_init(&(rx_reo_entry->st_netbuf_head));
    hmac_rx_mpdu_to_netbuf_list(&(rx_reo_entry->st_netbuf_head), netbuf_mpdu);
    rx_reo_entry->uc_num_buf = oal_netbuf_list_len(&(rx_reo_entry->st_netbuf_head));
    if (OAL_TRUE == hmac_ba_seqno_lt(ba_rx_hdl->us_baw_tail, MAC_GET_RX_CB_SEQ_NUM(rx_ctl))) {
        ba_rx_hdl->us_baw_tail = MAC_GET_RX_CB_SEQ_NUM(rx_ctl);
    }
    return;
}


OAL_STATIC uint32_t hmac_ba_rx_reo_timer_restart(hmac_user_stru *hmac_user,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf_mpdu)
{
    uint8_t          rx_tid;
    uint32_t         rx_timeout;
    hmac_vap_stru   *hmac_vap = OAL_PTR_NULL;
    mac_rx_ctl_stru *rx_ctl = OAL_PTR_NULL;
    mac_device_stru *mac_device = OAL_PTR_NULL;

    if (oal_any_null_ptr3(hmac_user, ba_rx_hdl, netbuf_mpdu)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_reo_timer_restart::invalid param.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
    rx_tid = MAC_GET_RX_CB_TID(rx_ctl);

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
    if (oal_unlikely(oal_any_null_ptr1(hmac_vap))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_ba_rx_reo_timer_restart::hmac_vap NULL.}", rx_ctl->uc_mac_vap_id);
        return OAL_FAIL;
    }

    mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_reorder_ba_timer_start::mac dev[%d] null.}",
            hmac_vap->st_vap_base_info.uc_device_id);
        return OAL_FAIL;
    }

    rx_timeout = g_ba_rx_timeout_len[WLAN_WME_TID_TO_AC(rx_tid)];
    if (hmac_user->ast_tid_info[rx_tid].st_ba_timer.en_is_registerd == OAL_TRUE) {
        return OAL_SUCC;
    } else {
        /* ��ȡdevice�ṹ */
        oal_spin_lock(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer_lock));
        frw_timer_create_timer_m(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer),
                                 hmac_ba_timeout_fn,
                                 g_ba_rx_timeout_len[WLAN_WME_TID_TO_AC(rx_tid)],
                                 &(ba_rx_hdl->st_alarm_data),
                                 OAL_FALSE,
                                 OAM_MODULE_ID_HMAC,
                                 mac_device->ul_core_id);
        oal_spin_unlock(&(hmac_user->ast_tid_info[rx_tid].st_ba_timer_lock));
    }
    return OAL_SUCC;
}
void hmac_ba_rx_buffered_data_add_list(hmac_rx_buf_stru *rx_reo_entry, oal_netbuf_stru *netbuf,
    oal_netbuf_head_stru *netbuf_head)
{
    netbuf = oal_netbuf_delist(&(rx_reo_entry->st_netbuf_head));
    if (netbuf != NULL) {
        oal_netbuf_add_to_list_tail(netbuf, netbuf_head);
    }
}

uint32_t hmac_ba_rx_buffered_data_check(hmac_ba_rx_stru *ba_rx_hdl,
    uint16_t sn, oal_netbuf_stru *netbuf_mpdu, oal_netbuf_head_stru *netbuf_head)
{
    uint16_t          release_end_sn;
    uint16_t          release_start_sn;
    oal_netbuf_stru  *netbuf = OAL_PTR_NULL;
    mac_rx_ctl_stru  *rx_ctl = OAL_PTR_NULL;
    hmac_rx_buf_stru *rx_reo_entry = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(ba_rx_hdl, netbuf_head) || (sn > MAX_BA_SN))) {
        oam_error_log3(0, OAM_SF_RX, "{buffered_data_check:param err:ba_rx_hdl[%p],sn[%d],netbuf_head[%p]}",
            (uintptr_t)ba_rx_hdl, sn, (uintptr_t)netbuf_head);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (!(HMAC_BAW_RIGHTSIDE(ba_rx_hdl->us_last_relseq, sn))) {
        if (ba_rx_hdl->us_last_relseq == sn) {
            return OAL_SUCC;
        } else {
            oam_info_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::input err check sn lrelease[%d] check[%d]}",
                ba_rx_hdl->us_last_relseq, sn);
            rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
            oam_info_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::RELEASE_VALID[%d] PROCESS_FLAG[%d]}",
                MAC_GET_RX_CB_REL_IS_VALID(rx_ctl), MAC_GET_RX_CB_PROC_FLAG(rx_ctl));
            oam_info_log4(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::SSN[%d] SN[%d] REL_START[%d] REL_END[%d].}",
                MAC_GET_RX_CB_SSN(rx_ctl), MAC_GET_RX_CB_SEQ_NUM(rx_ctl),
                MAC_GET_RX_CB_REL_START_SEQNUM(rx_ctl), MAC_GET_RX_CB_REL_END_SEQNUM(rx_ctl));
            return OAL_FAIL;
        }
    }

    /* ��us_last_relseq + 1 һֱ�ϱ���check_sequence_number -1����Ϊcheck_sequence_number���Լ����߼��ϱ� */
    release_start_sn = DMAC_BA_SEQNO_ADD(ba_rx_hdl->us_last_relseq, 1);
    release_end_sn = sn;

    if (sn == DMAC_BA_SEQNO_ADD(ba_rx_hdl->us_last_relseq, 1)) {
        return OAL_SUCC;
    }

    do {
        rx_reo_entry = hmac_remove_frame_from_reorder_q(ba_rx_hdl, release_start_sn);
        if (rx_reo_entry != OAL_PTR_NULL) {
            while (!oal_netbuf_list_empty(&(rx_reo_entry->st_netbuf_head))) {
                hmac_ba_rx_buffered_data_add_list(rx_reo_entry, netbuf, netbuf_head);
            }
        }
        release_start_sn = DMAC_BA_SEQNO_ADD(release_start_sn, 1);
    } while (release_start_sn != release_end_sn);

    hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)DMAC_BA_SEQNO_SUB(release_start_sn, 1));
    return OAL_SUCC;
}


uint32_t hmac_rx_release_valid_check(hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf_mpdu,
    oal_netbuf_head_stru *netbuf_head, uint16_t *release_start_sn, uint16_t release_end_sn)
{
    uint32_t ret;

    if (oal_unlikely((ba_rx_hdl == NULL) || (netbuf_mpdu == NULL) || (netbuf_head == NULL))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release::invalid param.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (oal_unlikely((release_start_sn == NULL) || (release_end_sn > MAX_BA_SN))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release::invalid param \
            release_start_sn[%p], release_end_sn[%d].}", (uintptr_t)release_start_sn, release_end_sn);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (oal_unlikely(*release_start_sn > MAX_BA_SN)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release::invalid param release_start_sn[%p].}",
            *release_start_sn);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((HMAC_BAW_RIGHTSIDE(*release_start_sn, release_end_sn)) || (*release_start_sn == release_end_sn)) {
        /* ���release_start_sn������ˣ���������ϱ�release_start_sn֮ǰ�İ� */
        ret = hmac_ba_rx_buffered_data_check(ba_rx_hdl, *release_start_sn, netbuf_mpdu, netbuf_head);
        if (ret != OAL_SUCC) {
            if (HMAC_BAW_RIGHTSIDE(ba_rx_hdl->us_last_relseq, release_end_sn)) {
                /* release_start_snС��last_release��release_end_sn����last_release���last_release+1��ʼ�ϱ� */
                *release_start_sn = DMAC_BA_SEQNO_ADD(ba_rx_hdl->us_last_relseq, 1);
            } else if (release_end_sn == ba_rx_hdl->us_last_relseq) {
                return OAL_FAIL;
            } else {
                oam_warning_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release:release_end_sn less than \
                    last_relseq+1, release_endsn[%d] last_relseq[%d].}", release_end_sn, ba_rx_hdl->us_last_relseq);
                return OAL_FAIL;
            }
        }
    } else {
        oam_warning_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release:report wrong realese sn,start[%d] end[%d]}",
            *release_start_sn, release_end_sn);
        /* buffered frame�ߵ��˴��������� */
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


uint32_t hmac_ba_rx_buffered_data_release(hmac_user_stru *hmac_user,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf, oal_netbuf_head_stru *pnetbuf_head)
{
    uint8_t           tid;
    uint16_t          release_end_sn;
    uint16_t          release_start_sn;
    uint32_t          rel_valid_check;
    mac_rx_ctl_stru  *rx_ctl = OAL_PTR_NULL;
    hmac_rx_buf_stru *rx_reo_entry = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr4(hmac_user, ba_rx_hdl, netbuf, pnetbuf_head))) {
        oam_error_log0(0, 0, "{hmac_ba_rx_buffered_data_release:param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    /* �������֡��Ҫ����������������е���Ϣ�ѻ���֡����pst_netbuf_head������ */
    tid = MAC_GET_RX_CB_TID(rx_ctl);
    release_start_sn = MAC_GET_RX_CB_REL_START_SEQNUM(rx_ctl);
    release_end_sn = MAC_GET_RX_CB_REL_END_SEQNUM(rx_ctl);

    rel_valid_check = hmac_rx_release_valid_check(ba_rx_hdl, netbuf, pnetbuf_head, &release_start_sn, release_end_sn);
    if (rel_valid_check != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release::release valid check failed.}");
        /* ֱ�ӷ��أ������������ͷŲ��� */
        return OAL_SUCC;
    }
    /* ����release sequence number */
    hmac_ba_rx_update_release_seqnum(ba_rx_hdl, release_end_sn);

    release_end_sn = DMAC_BA_SEQNO_ADD(release_end_sn, 1);
    do {
        rx_reo_entry = hmac_remove_frame_from_reorder_q(ba_rx_hdl, release_start_sn);
        if (rx_reo_entry != OAL_PTR_NULL) {
            oal_netbuf_queue_splice_tail_init(&(rx_reo_entry->st_netbuf_head), pnetbuf_head);
        }

        release_start_sn = DMAC_BA_SEQNO_ADD(release_start_sn, 1);
    } while ((release_start_sn != release_end_sn) && (ba_rx_hdl->uc_mpdu_cnt != 0));

    /* ����us_mpdu_cnt�ж�������ֹоƬ�ϱ��쳣��release sequence��ɿ�ѭ�� */
    /* ������ջ���������滹�б��ģ�restart timer */
    if (ba_rx_hdl->uc_mpdu_cnt > 0) {
        oam_info_log3(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_release::restart timer user[%d],tid[%d], mpdu cnt[%d]}",
            rx_ctl->bit_ta_user_idx, rx_ctl->rx_tid, ba_rx_hdl->uc_mpdu_cnt);
        hmac_ba_rx_reo_timer_restart(hmac_user, ba_rx_hdl, netbuf);
    } else {
        if (hmac_user->ast_tid_info[tid].st_ba_timer.en_is_enabled == OAL_TRUE) {
            frw_timer_stop_timer_m(&(hmac_user->ast_tid_info[tid].st_ba_timer));
        }
    }
    return OAL_SUCC;
}


uint32_t hmac_ba_rx_data_release(hmac_user_stru *hmac_user,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf, oal_netbuf_head_stru *pnetbuf_head)
{
    uint32_t         ret = 0;
    uint32_t         in_wnd = 0;
    mac_rx_ctl_stru *rx_ctl = OAL_PTR_NULL;

    /* ����ж� */
    if (oal_unlikely(oal_any_null_ptr3(ba_rx_hdl, netbuf, pnetbuf_head))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_data_release::param null.}");
        return OAL_FAIL;
    }
    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (HMAC_BAW_WITHIN(ba_rx_hdl->us_baw_start, ba_rx_hdl->us_baw_size, MAC_GET_RX_CB_SEQ_NUM(rx_ctl))) {
        /* �˴����߼���Ӳ��û���ƶ����ڣ��Ҵ˰��ڴ���������Ҫ�ϱ�����ô�˰�һ���Ǵ˴�����ǰ��Ķ� */
        /* �����ڵ�������ֻ�ᱨ���棬������İ��ᱨ�����ƶ� */
        if (ba_rx_hdl->us_baw_start == MAC_GET_RX_CB_SSN(rx_ctl)) {
            in_wnd = 1;
            /* ���������ϱ�ǰ���˱���֮ǰ�Ƿ��л���ı��ģ�Ӳ��û���ƶ����ڣ���������SSN���м�� */
            ret = hmac_ba_rx_buffered_data_check(ba_rx_hdl, MAC_GET_RX_CB_SEQ_NUM(rx_ctl), netbuf, pnetbuf_head);
            if (ret != OAL_SUCC) {
                oam_warning_log2(0, OAM_SF_RX, "{hmac_ba_rx_data_release::indicate to rpt a frame bf last release sn,\
                    last release:%d, curr frm:%d", ba_rx_hdl->us_last_relseq, MAC_GET_RX_CB_SEQ_NUM(rx_ctl));
                return ret;
            }
            hmac_rx_mpdu_to_netbuf_list(pnetbuf_head, netbuf);
            /* ���������ϱ�������release_seqnum */
            hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)MAC_GET_RX_CB_SEQ_NUM(rx_ctl));
        }
    }
    /* ���� ssn */
    ba_rx_hdl->us_baw_start = MAC_GET_RX_CB_SSN(rx_ctl);
    ba_rx_hdl->en_timer_triggered = OAL_FALSE;
    if (MAC_GET_RX_CB_REL_IS_VALID(rx_ctl) != 0) {
        /* �������֡��Ҫ����������������е���Ϣ�ѻ���֡����pst_netbuf_head������ */
        ret = hmac_ba_rx_buffered_data_release(hmac_user, ba_rx_hdl, netbuf, pnetbuf_head);
    }
    if (in_wnd == 0) {
        /* ���������ϱ�ǰ���˱���֮ǰ�Ƿ��л���ı��� */
        ret = hmac_ba_rx_buffered_data_check(ba_rx_hdl, MAC_GET_RX_CB_SEQ_NUM(rx_ctl), netbuf, pnetbuf_head);
        if (ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_RX, "{hmac_ba_rx_data_release::indicate to release a frame bf last release sn,\
                last release:[%d] curr frame[%d]}", ba_rx_hdl->us_last_relseq, MAC_GET_RX_CB_SEQ_NUM(rx_ctl));
            return ret;
        }

        hmac_rx_mpdu_to_netbuf_list(pnetbuf_head, netbuf);
        /* ���������ϱ�������release_seqnum */
        hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)MAC_GET_RX_CB_SEQ_NUM(rx_ctl));
    }

    if (HMAC_BAW_RIGHTSIDE(ba_rx_hdl->us_last_relseq, MAC_GET_RX_CB_SSN(rx_ctl))) {
        /* ���SSN����last_release_seqnum���棬����Լ���release */
        return hmac_ba_rx_buffered_data_check(ba_rx_hdl, MAC_GET_RX_CB_SSN(rx_ctl), netbuf, pnetbuf_head);
    }
    return ret;
}


uint32_t hmac_ba_rx_data_buffer(hmac_user_stru *hmac_user,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf_mpdu, oal_netbuf_head_stru *netbuf_head)
{
    mac_rx_ctl_stru *rx_ctl = NULL;

    if (oal_unlikely(oal_any_null_ptr3(ba_rx_hdl, netbuf_mpdu, netbuf_head))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_data_buffer:param null}");
        return OAL_FAIL;
    }
    /* ���� ssn */
    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
    ba_rx_hdl->us_baw_start = MAC_GET_RX_CB_SSN(rx_ctl);

    /* BA window start�Ƶ�baw_start��Ӳ��������������baw_start֮ǰ�İ��������baw_start֮ǰ�İ��������ϱ� */
    if (OAL_SUCC != hmac_ba_rx_buffered_data_check(ba_rx_hdl, MAC_GET_RX_CB_SSN(rx_ctl), netbuf_mpdu, netbuf_head)) {
        oam_info_log0(0, OAM_SF_RX, "{hmac_ba_rx_data_buffer:hmac_ba_rx_buffered_data_check fail}");
    }

    if (MAC_GET_RX_CB_REL_IS_VALID(rx_ctl) != 0) {
        /* �������֡��Ҫ����������������е���Ϣ�ѻ���֡����pst_netbuf_head������ */
        hmac_ba_rx_buffered_data_release(hmac_user, ba_rx_hdl, netbuf_mpdu, netbuf_head);
    }

    hmac_ba_add_netbuf_to_reo_list(ba_rx_hdl, netbuf_mpdu, netbuf_head);
    /* ���timerû��������������timer�� */
    if (hmac_user->ast_tid_info[MAC_GET_RX_CB_TID(rx_ctl)].st_ba_timer.en_is_enabled == OAL_FALSE) {
        hmac_ba_rx_reo_timer_restart(hmac_user, ba_rx_hdl, netbuf_mpdu);
    }

    return OAL_SUCC;
}
