

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ͷ�ļ����� */
#include "mac_ie.h"
#include "mac_regdomain.h"
#include "mac_device.h"
#include "wlan_types.h"
#include "hmac_mgmt_sta.h"
#include "hmac_sme_sta.h"
#include "hmac_fsm.h"
#include "hmac_dfs.h"
#include "hmac_chan_mgmt.h"
#include "mac_device.h"
#include "hmac_scan.h"
#include "frw_ext_if.h"
#include "hmac_resource.h"
#include "hmac_encap_frame.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CHAN_MGMT_C

/* 2 ȫ�ֱ������� */
#define HMAC_CENTER_FREQ_2G_40M_OFFSET 2 /* ����Ƶ����������ŵ�idx��ƫ���� */
#define HMAC_AFFECTED_CH_IDX_OFFSET    5 /* 2.4GHz�£�40MHz������Ӱ����ŵ��뾶������Ƶ�� +/- 5���ŵ� */

/* 3 �������� */
/* 4 ����ʵ�� */
uint32_t hmac_dump_chan(mac_vap_stru *pst_mac_vap, uint8_t *puc_param)
{
    dmac_set_chan_stru *pst_chan = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_chan = (dmac_set_chan_stru *)puc_param;
    oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_2040, "channel mgmt info\r\n");
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_chan_number=%d\r\n", pst_chan->st_channel.uc_chan_number);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_band=%d\r\n", pst_chan->st_channel.en_band);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_bandwidth=%d\r\n", pst_chan->st_channel.en_bandwidth);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_idx=%d\r\n", pst_chan->st_channel.uc_chan_idx);

    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "c_announced_channel=%d\r\n",
                  pst_chan->st_ch_switch_info.uc_announced_channel);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_announced_bandwidth=%d\r\n",
                  pst_chan->st_ch_switch_info.en_announced_bandwidth);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_ch_switch_cnt=%d\r\n",
                  pst_chan->st_ch_switch_info.uc_ch_switch_cnt);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_ch_switch_status=%d\r\n",
                  pst_chan->st_ch_switch_info.en_ch_switch_status);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_bw_switch_status=%d\r\n",
                  pst_chan->st_ch_switch_info.en_bw_switch_status);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_csa_present_in_bcn=%d\r\n",
                  pst_chan->st_ch_switch_info.en_csa_present_in_bcn);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_start_chan_idx=%d\r\n",
                  pst_chan->st_ch_switch_info.uc_start_chan_idx);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_end_chan_idx=%d\r\n",
                  pst_chan->st_ch_switch_info.uc_end_chan_idx);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_user_pref_bandwidth=%d\r\n",
                  pst_chan->st_ch_switch_info.en_user_pref_bandwidth);

    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "uc_new_channel=%d\r\n",
                  pst_chan->st_ch_switch_info.uc_new_channel);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_new_bandwidth=%d\r\n",
                  pst_chan->st_ch_switch_info.en_new_bandwidth);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "en_te_b=%d\r\n", pst_chan->st_ch_switch_info.en_te_b);
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040, "ul_chan_report_for_te_a=%d\r\n",
                  pst_chan->st_ch_switch_info.ul_chan_report_for_te_a);

    return OAL_SUCC;
}


void hmac_chan_initiate_switch_to_new_channel(mac_vap_stru *pst_mac_vap, uint8_t uc_channel,
    wlan_channel_bandwidth_enum_uint8 en_bandwidth)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;
    dmac_set_ch_switch_info_stru *pst_ch_switch_info = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    /* AP׼���л��ŵ� */
    pst_mac_vap->st_ch_switch_info.en_ch_switch_status = WLAN_CH_SWITCH_STATUS_1;
    pst_mac_vap->st_ch_switch_info.uc_announced_channel = uc_channel;
    pst_mac_vap->st_ch_switch_info.en_announced_bandwidth = en_bandwidth;

    /* ��Beacon֡�����Channel Switch Announcement IE */
    pst_mac_vap->st_ch_switch_info.en_csa_present_in_bcn = OAL_TRUE;

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_2040,
        "{hmac_chan_initiate_switch_to_new_channel::uc_announced_channel=%d,en_announced_bandwidth=%d csa_cnt=%d}",
        uc_channel, en_bandwidth, pst_mac_vap->st_ch_switch_info.uc_ch_switch_cnt);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_chan_initiate_switch_to_new_channel::pst_mac_device null.}");
        return;
    }
    /* �����¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_set_ch_switch_info_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_chan_initiate_switch_to_new_channel::pst_event_mem null.}");
        return;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_SWITCH_TO_NEW_CHAN,
                       OAL_SIZEOF(dmac_set_ch_switch_info_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* ��д�¼�payload */
    pst_ch_switch_info = (dmac_set_ch_switch_info_stru *)pst_event->auc_event_data;
    pst_ch_switch_info->en_ch_switch_status = WLAN_CH_SWITCH_STATUS_1;
    pst_ch_switch_info->uc_announced_channel = uc_channel;
    pst_ch_switch_info->en_announced_bandwidth = en_bandwidth;
    pst_ch_switch_info->uc_ch_switch_cnt = pst_mac_vap->st_ch_switch_info.uc_ch_switch_cnt;
    pst_ch_switch_info->en_csa_present_in_bcn = OAL_TRUE;
    pst_ch_switch_info->uc_csa_vap_cnt = pst_mac_device->uc_csa_vap_cnt;

    pst_ch_switch_info->en_csa_mode = pst_mac_vap->st_ch_switch_info.en_csa_mode;

    /* �ַ��¼� */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_chan_initiate_switch_to_new_channel::frw_event_dispatch_event failed[%d].}", ul_ret);
        frw_event_free_m(pst_event_mem);
        return;
    }

    /* �ͷ��¼� */
    frw_event_free_m(pst_event_mem);
}


uint32_t hmac_check_ap_channel_follow_sta(mac_vap_stru *pst_check_mac_vap,
    const mac_channel_stru *pst_set_mac_channel, uint8_t *puc_ap_follow_channel)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_index_mac_vap = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_set_mac_channel, puc_ap_follow_channel, pst_check_mac_vap)) {
        oam_error_log0(0, OAM_SF_2040, "{hmac_check_ap_channel_follow_sta:: input param is null,return}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (!IS_LEGACY_VAP(pst_check_mac_vap)) {
        oam_warning_log1(pst_check_mac_vap->uc_vap_id, OAM_SF_2040,
            "{hmac_check_ap_channel_follow_sta:: vap_p2p_mode=%d, not neet to check}", pst_check_mac_vap->en_p2p_mode);
        return OAL_FAIL;
    }

    if (!IS_STA(pst_check_mac_vap) && !IS_AP(pst_check_mac_vap)) {
        oam_error_log1(pst_check_mac_vap->uc_vap_id, OAM_SF_2040,
                       "{hmac_check_ap_channel_follow_sta:: pst_mac_vap_current->en_vap_mode=%d,not neet to check}\n",
                       pst_check_mac_vap->en_vap_mode);
        return OAL_FAIL;
    }

    pst_mac_device = mac_res_get_dev(pst_check_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(pst_check_mac_vap->uc_vap_id, OAM_SF_2040,
            "{hmac_check_ap_channel_follow_sta::get mac_device(%d) is null. Return}", pst_check_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log3(pst_check_mac_vap->uc_vap_id, OAM_SF_2040,
        "{hmac_check_ap_channel_follow_sta:: check_vap_state=%d, check_vap_band=%d  check_vap_channel=%d.}",
        pst_check_mac_vap->en_vap_state, pst_set_mac_channel->en_band, pst_set_mac_channel->uc_chan_number);

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_index_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if ((pst_index_mac_vap == OAL_PTR_NULL) ||
            (pst_check_mac_vap->uc_vap_id == pst_index_mac_vap->uc_vap_id) ||
            (pst_index_mac_vap->en_p2p_mode != WLAN_LEGACY_VAP_MODE)) {
            continue;
        }
        if ((pst_index_mac_vap->en_vap_state != MAC_VAP_STATE_UP) &&
            (pst_index_mac_vap->en_vap_state != MAC_VAP_STATE_PAUSE)) {
            continue;
        }

        oam_warning_log4(pst_index_mac_vap->uc_vap_id, OAM_SF_2040,
            "{hmac_check_ap_channel_follow_sta::index_vap_state=%d,index_vap_mode=%d,idx_vap_band=%d,idx_vap_chan=%d.}",
            pst_index_mac_vap->en_vap_state, pst_check_mac_vap->en_vap_mode,
            pst_index_mac_vap->st_channel.en_band, pst_index_mac_vap->st_channel.uc_chan_number);

        if (IS_STA(pst_check_mac_vap) && IS_AP(pst_index_mac_vap)) { /* AP������;STA������ */
            if ((pst_set_mac_channel->en_band == pst_index_mac_vap->st_channel.en_band) &&
                (pst_set_mac_channel->uc_chan_number != pst_index_mac_vap->st_channel.uc_chan_number)) { /* CSA */
                oam_warning_log2(pst_index_mac_vap->uc_vap_id, OAM_SF_2040,
                    "{hmac_check_ap_channel_follow_sta::<vap_current_mode=STA vap_index_mode=Ap> \
                    SoftAp CSA Operate, Channel from [%d] To [%d]}.\n",
                    pst_index_mac_vap->st_channel.uc_chan_number, pst_set_mac_channel->uc_chan_number);
                pst_mac_device->uc_csa_vap_cnt++;
                pst_index_mac_vap->st_ch_switch_info.uc_ch_switch_cnt = HMAC_CHANNEL_SWITCH_COUNT; /* CSA cnt ����Ϊ5 */
                pst_index_mac_vap->st_ch_switch_info.en_csa_mode = WLAN_CSA_MODE_TX_DISABLE;
                hmac_chan_initiate_switch_to_new_channel(pst_index_mac_vap, pst_set_mac_channel->uc_chan_number,
                                                         pst_index_mac_vap->st_channel.en_bandwidth);
                *puc_ap_follow_channel = pst_set_mac_channel->uc_chan_number;
                return OAL_SUCC;
            }
        } else if (IS_AP(pst_check_mac_vap) && IS_STA(pst_index_mac_vap)) { /* STA������;AP������ */
            if ((pst_set_mac_channel->en_band == pst_index_mac_vap->st_channel.en_band) &&
                (pst_set_mac_channel->uc_chan_number != pst_index_mac_vap->st_channel.uc_chan_number)) {
                /* �滻�ŵ�ֵ */
                *puc_ap_follow_channel = pst_index_mac_vap->st_channel.uc_chan_number;
                oam_warning_log2(pst_index_mac_vap->uc_vap_id, OAM_SF_2040,
                    "{hmac_check_ap_channel_follow_sta::<vap_current_mode=Ap vap_index_mode=Sta> \
                    SoftAp change Channel from [%d] To [%d]}.\n",
                    pst_set_mac_channel->uc_chan_number, *puc_ap_follow_channel);
                return OAL_SUCC;
            }
        }
    }

    return OAL_FAIL;
}

void hmac_chan_sync_init(mac_vap_stru *pst_mac_vap, dmac_set_chan_stru *pst_set_chan)
{
    int32_t l_ret;

    memset_s(pst_set_chan, OAL_SIZEOF(dmac_set_chan_stru), 0, OAL_SIZEOF(dmac_set_chan_stru));
    l_ret = memcpy_s(&pst_set_chan->st_channel, OAL_SIZEOF(mac_channel_stru),
                     &pst_mac_vap->st_channel, OAL_SIZEOF(mac_channel_stru));
    l_ret += memcpy_s(&pst_set_chan->st_ch_switch_info, OAL_SIZEOF(mac_ch_switch_info_stru),
                      &pst_mac_vap->st_ch_switch_info, OAL_SIZEOF(mac_ch_switch_info_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_chan_sync_init::memcpy fail!");
        return;
    }
}


void hmac_chan_do_sync(mac_vap_stru *pst_mac_vap, dmac_set_chan_stru *pst_set_chan)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_idx;

    hmac_dump_chan(pst_mac_vap, (uint8_t *)pst_set_chan);
    /* ����VAP�µ���20MHz�ŵ��š�����ģʽ���ŵ����� */
    ul_ret = mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band,
                                          pst_set_chan->st_channel.uc_chan_number, &uc_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_chan_sync::mac_get_channel_idx_from_num failed[%d].}", ul_ret);

        return;
    }

    pst_mac_vap->st_channel.uc_chan_number = pst_set_chan->st_channel.uc_chan_number;
    pst_mac_vap->st_channel.en_bandwidth = pst_set_chan->st_channel.en_bandwidth;
    pst_mac_vap->st_channel.uc_chan_idx = uc_idx;

    /* �����¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_set_chan_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_chan_sync::pst_event_mem null.}");
        return;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WALN_CTX_EVENT_SUB_TYPR_SELECT_CHAN,
                       OAL_SIZEOF(dmac_set_chan_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    if (EOK != memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(dmac_set_chan_stru),
                        (uint8_t *)pst_set_chan, OAL_SIZEOF(dmac_set_chan_stru))) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_chan_do_sync::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return;
    }

    /* �ַ��¼� */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_chan_sync::frw_event_dispatch_event failed[%d].}", ul_ret);
        frw_event_free_m(pst_event_mem);
        return;
    }

    /* �ͷ��¼� */
    frw_event_free_m(pst_event_mem);
}


void hmac_chan_sync(mac_vap_stru *pst_mac_vap,
                    uint8_t uc_channel, wlan_channel_bandwidth_enum_uint8 en_bandwidth,
                    oal_bool_enum_uint8 en_switch_immediately)
{
    dmac_set_chan_stru st_set_chan;

    hmac_chan_sync_init(pst_mac_vap, &st_set_chan);
    st_set_chan.st_channel.uc_chan_number = uc_channel;
    st_set_chan.st_channel.en_bandwidth = en_bandwidth;
    st_set_chan.en_switch_immediately = en_switch_immediately;
    st_set_chan.en_dot11FortyMHzIntolerant = mac_mib_get_FortyMHzIntolerant(pst_mac_vap);
    hmac_chan_do_sync(pst_mac_vap, &st_set_chan);
}


void hmac_chan_multi_select_channel_mac(mac_vap_stru *pst_mac_vap, uint8_t uc_channel,
                                                wlan_channel_bandwidth_enum_uint8 en_bandwidth)
{
    uint8_t uc_vap_idx;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_vap = OAL_PTR_NULL;

    oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                     "{hmac_chan_multi_select_channel_mac::uc_channel=%d,en_bandwidth=%d}",
                     uc_channel, en_bandwidth);

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                       "{hmac_chan_multi_select_channel_mac::pst_device null,device_id=%d.}",
                       pst_mac_vap->uc_device_id);
        return;
    }

    if (pst_device->uc_vap_num == 0) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_2040, "{hmac_chan_multi_select_channel_mac::none vap.}");
        return;
    }

    if (mac_is_dbac_running(pst_device)) {
        hmac_chan_sync(pst_mac_vap, uc_channel, en_bandwidth, OAL_TRUE);
        return;
    }

    /* ����device������vap�� */
    for (uc_vap_idx = 0; uc_vap_idx < pst_device->uc_vap_num; uc_vap_idx++) {
        pst_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_device->auc_vap_id[uc_vap_idx]);
        if (pst_vap == OAL_PTR_NULL) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                           "{hmac_chan_multi_select_channel_mac::pst_vap null,vap_id=%d.}",
                           pst_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        hmac_chan_sync(pst_vap, uc_channel, en_bandwidth, OAL_TRUE);
    }
}


void hmac_chan_update_40M_intol_user(mac_vap_stru *pst_mac_vap)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;

    oal_dlist_search_for_each(pst_entry, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_mac_user = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
        if (oal_unlikely(pst_mac_user == OAL_PTR_NULL)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                             "{hmac_chan_update_40M_intol_user::pst_user null pointer.}");
            continue;
        } else {
            if (pst_mac_user->st_ht_hdl.bit_forty_mhz_intolerant) {
                pst_mac_vap->en_40M_intol_user = OAL_TRUE;
                return;
            }
        }
    }

    pst_mac_vap->en_40M_intol_user = OAL_FALSE;
}


OAL_STATIC OAL_INLINE uint8_t hmac_chan_get_user_pref_primary_ch(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->uc_max_channel;
}


OAL_STATIC OAL_INLINE wlan_channel_bandwidth_enum_uint8 hmac_chan_get_user_pref_bandwidth(mac_vap_stru *pst_mac_vap)
{
    return pst_mac_vap->st_ch_switch_info.en_user_pref_bandwidth;
}


void hmac_chan_reval_bandwidth_sta(mac_vap_stru *pst_mac_vap, uint32_t ul_change)
{
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_chan_reval_bandwidth_sta::pst_mac_vap null.}");
        return;
    }

    /* ��Ҫ���д����л� */
    if (MAC_BW_CHANGE & ul_change) {
        hmac_chan_multi_select_channel_mac(pst_mac_vap, pst_mac_vap->st_channel.uc_chan_number,
                                               pst_mac_vap->st_channel.en_bandwidth);
    }
}


OAL_STATIC void hmac_chan_ctrl_machw_tx(mac_vap_stru *pst_mac_vap, uint8_t uc_sub_type)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;

    /* �����¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(0);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_chan_ctrl_machw_tx::pst_event_mem null.}");
        return;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       uc_sub_type,
                       0,
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* �ַ��¼� */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_chan_ctrl_machw_tx::frw_event_dispatch_event failed[%d].}", ul_ret);
        frw_event_free_m(pst_event_mem);
        return;
    }

    /* �ͷ��¼� */
    frw_event_free_m(pst_event_mem);
}


void hmac_chan_disable_machw_tx(mac_vap_stru *pst_mac_vap)
{
    hmac_chan_ctrl_machw_tx(pst_mac_vap, DMAC_WALN_CTX_EVENT_SUB_TYPR_DISABLE_TX);
}


void hmac_chan_enable_machw_tx(mac_vap_stru *pst_mac_vap)
{
    hmac_chan_ctrl_machw_tx(pst_mac_vap, DMAC_WALN_CTX_EVENT_SUB_TYPR_ENABLE_TX);
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_chan_check_channnel_avaible(wlan_channel_band_enum_uint8 en_band,
                                                                           uint8_t *puc_start_ch_idx,
                                                                           uint8_t *puc_end_ch_idx)
{
    int32_t l_ch_idx;
    uint8_t uc_num_supp_chan = mac_get_num_supp_channel(en_band);
    uint32_t ul_ret;

    /* ȡ����Ч�ŵ� */
    for (l_ch_idx = *puc_start_ch_idx; l_ch_idx < uc_num_supp_chan; l_ch_idx++) {
        ul_ret = mac_is_channel_idx_valid(en_band, (uint8_t)l_ch_idx);
        if (ul_ret == OAL_SUCC) {
            *puc_start_ch_idx = (uint8_t)l_ch_idx;
            break;
        }
    }

    if (l_ch_idx == uc_num_supp_chan) {
        return OAL_FALSE;
    }

    /* ȡ����Ч�ŵ� */
    for (l_ch_idx = *puc_end_ch_idx; l_ch_idx >= 0; l_ch_idx--) {
        ul_ret = mac_is_channel_idx_valid(en_band, (uint8_t)l_ch_idx);
        if (ul_ret == OAL_SUCC) {
            *puc_end_ch_idx = (uint8_t)l_ch_idx;
            break;
        }
    }

    if (l_ch_idx < 0) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


uint32_t hmac_start_bss_in_available_channel(hmac_vap_stru *pst_hmac_vap)
{
    hmac_ap_start_rsp_stru st_ap_start_rsp;
    uint32_t ul_ret;

    mac_vap_init_rates(&(pst_hmac_vap->st_vap_base_info));

    /* ����AP��״̬��Ϊ UP */
    hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_UP);

    /* ����hmac_config_start_vap_event������BSS */
    ul_ret = hmac_config_start_vap_event(&(pst_hmac_vap->st_vap_base_info), OAL_TRUE);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_INIT);
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_start_bss_in_available_channel::hmac_config_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* ����bssid */
    mac_vap_set_bssid(&pst_hmac_vap->st_vap_base_info, mac_mib_get_StationID(&pst_hmac_vap->st_vap_base_info));

    /* �����Ż�����ͬƵ���µ�������һ�� */
    if (WLAN_BAND_2G == pst_hmac_vap->st_vap_base_info.st_channel.en_band) {
        mac_mib_set_SpectrumManagementRequired(&(pst_hmac_vap->st_vap_base_info), OAL_FALSE);
    } else {
        mac_mib_set_SpectrumManagementRequired(&(pst_hmac_vap->st_vap_base_info), OAL_TRUE);
    }

    /* ������ϱ���sme */
    st_ap_start_rsp.en_result_code = HMAC_MGMT_SUCCESS;

    return OAL_SUCC;
}


uint32_t hmac_chan_start_bss(hmac_vap_stru *pst_hmac_vap, mac_channel_stru *pst_channel,
                             wlan_protocol_enum_uint8 en_protocol)
{
    uint32_t ul_ret;

    // ͬ���ŵ���ģʽ
    ul_ret = hmac_sta_sync_vap(pst_hmac_vap, pst_channel, en_protocol);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{hmac_chan_start_bss::hmac_sta_sync_vap failed[%d].}", ul_ret);
        return ul_ret;
    }

    // ����vap
    return hmac_start_bss_in_available_channel(pst_hmac_vap);
}


uint16_t mac_get_center_freq1_from_freq_and_bandwidth(uint16_t freq,
    wlan_channel_bandwidth_enum_uint8 en_bandwidth)
{
    uint16_t       us_center_freq1;

    /******************************************************
     * channel number to center channel idx map
    BAND WIDTH                            CENTER CHAN INDEX:

    WLAN_BAND_WIDTH_20M                   chan_num
    WLAN_BAND_WIDTH_40PLUS                chan_num + 2
    WLAN_BAND_WIDTH_40MINUS               chan_num - 2
    WLAN_BAND_WIDTH_80PLUSPLUS            chan_num + 6
    WLAN_BAND_WIDTH_80PLUSMINUS           chan_num - 2
    WLAN_BAND_WIDTH_80MINUSPLUS           chan_num + 2
    WLAN_BAND_WIDTH_80MINUSMINUS          chan_num - 6
    WLAN_BAND_WIDTH_160PLUSPLUSPLUS       chan_num + 14
    WLAN_BAND_WIDTH_160MINUSPLUSPLUS      chan_num + 10
    WLAN_BAND_WIDTH_160PLUSMINUSPLUS      chan_num + 6
    WLAN_BAND_WIDTH_160MINUSMINUSPLUS     chan_num + 2
    WLAN_BAND_WIDTH_160PLUSPLUSMINUS      chan_num - 2
    WLAN_BAND_WIDTH_160MINUSPLUSMINUS     chan_num - 6
    WLAN_BAND_WIDTH_160PLUSMINUSMINUS     chan_num - 10
    WLAN_BAND_WIDTH_160MINUSMINUSMINUS    chan_num - 14
    ********************************************************/
    int8_t ac_center_chan_offset[WLAN_BAND_WIDTH_BUTT - 2] = {
            0, 2, -2, 6, -2, 2, -6
#ifdef _PRE_WLAN_FEATURE_160M
            , 14, -2, 6, -10, 10, -6, 2, -14
#endif
    };

    if (freq < 2412 || freq > 5825) {
        return OAL_FAIL;
    }

    if (en_bandwidth >= WLAN_BAND_WIDTH_BUTT - 2) {
        return OAL_FAIL;
    }

    us_center_freq1 = freq + ac_center_chan_offset[en_bandwidth] * 5;

    return us_center_freq1;
}


OAL_STATIC uint32_t hmac_report_channel_switch(hmac_vap_stru *pst_hmac_vap, mac_channel_stru *pst_channel)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_channel_switch_stru st_channel_switch = {0};
    /* ��������״̬csa�ϱ�����supplicant������״̬��һ�� */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
        oam_warning_log0(0, OAM_SF_CHAN, "{hmac_report_channel_switch::vap in roam state, No csa report!}");
        return OAL_SUCC;
    }

    if (OAL_SUCC != mac_is_channel_num_valid(pst_channel->en_band, pst_channel->uc_chan_number)) {
        oam_error_log2(0, OAM_SF_CHAN, "{hmac_report_channel_switch::invalid channel, en_band=%d, ch_num=%d",
            pst_channel->en_band, pst_channel->uc_chan_number);
        return OAL_FAIL;
    }

    st_channel_switch.center_freq = (pst_channel->en_band == WLAN_BAND_2G) ?
            (g_ast_freq_map_2g[pst_channel->uc_chan_idx].us_freq) :
            (g_ast_freq_map_5g[pst_channel->uc_chan_idx].us_freq);
    st_channel_switch.width = (oal_nl80211_chan_width)WLAN_BANDWIDTH_TO_IEEE_CHAN_WIDTH(pst_channel->en_bandwidth,
                               mac_mib_get_HighThroughputOptionImplemented(&pst_hmac_vap->st_vap_base_info));

    st_channel_switch.center_freq1 = mac_get_center_freq1_from_freq_and_bandwidth(st_channel_switch.center_freq,
                                                                                  pst_channel->en_bandwidth);
    /* 5MHz, 10MHz, 20MHz, 40MHz, 80MHz center_freq2 is zero */
    /* 80+80MHz need set center_freq2 */
    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHAN,
        "hmac_report_channel_switch: center_freq=%d width=%d, center_freq1=%d center_freq2=%d",
        st_channel_switch.center_freq, st_channel_switch.width,
        st_channel_switch.center_freq1, st_channel_switch.center_freq2);

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(st_channel_switch));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHAN,
            "{hmac_report_channel_switch::frw_event_alloc_m fail! size[%d]}", OAL_SIZEOF(st_channel_switch));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_CH_SWITCH_NOTIFY,
                       OAL_SIZEOF(st_channel_switch),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    if (EOK != memcpy_s((uint8_t *)frw_get_event_payload(pst_event_mem), OAL_SIZEOF(st_channel_switch),
                        (uint8_t *)&st_channel_switch, OAL_SIZEOF(st_channel_switch))) {
        oam_error_log0(0, OAM_SF_CHAN, "hmac_report_channel_switch::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_chan_restart_network_after_switch(mac_vap_stru *pst_mac_vap)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;

    oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_2040, "{hmac_chan_restart_network_after_switch}");

    /* �����¼��ڴ� */
    pst_event_mem = frw_event_alloc_m(0);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_chan_restart_network_after_switch::pst_event_mem null.}");

        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPR_RESTART_NETWORK,
                       0,
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* �ַ��¼� */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_chan_restart_network_after_switch::frw_event_dispatch_event failed[%d].}", ul_ret);
        frw_event_free_m(pst_event_mem);

        return ul_ret;
    }
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_chan_switch_to_new_chan_complete(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    dmac_set_chan_stru *pst_set_chan = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_idx;
    uint8_t uc_ap_follow_channel = 0;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_2040, "{hmac_switch_to_new_chan_complete::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_set_chan = (dmac_set_chan_stru *)pst_event->auc_event_data;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_2040,
                       "{hmac_switch_to_new_chan_complete::pst_hmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    oam_warning_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_2040, "hmac_chan_switch_to_new_chan_complete");
    hmac_dump_chan(pst_mac_vap, (uint8_t *)pst_set_chan);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_2040,
                       "{hmac_switch_to_new_chan_complete::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band,
                                          pst_set_chan->st_channel.uc_chan_number, &uc_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                         "{hmac_switch_to_new_chan_complete::mac_get_channel_idx_from_num failed[%d].}", ul_ret);

        return OAL_FAIL;
    }

    pst_mac_vap->st_channel.uc_chan_number = pst_set_chan->st_channel.uc_chan_number;
    pst_mac_vap->st_channel.en_bandwidth = pst_set_chan->st_channel.en_bandwidth;
    pst_mac_vap->st_channel.uc_chan_idx = uc_idx;

    pst_mac_vap->st_ch_switch_info.en_waiting_to_shift_channel =
        pst_set_chan->st_ch_switch_info.en_waiting_to_shift_channel;

    pst_mac_vap->st_ch_switch_info.en_ch_switch_status = pst_set_chan->st_ch_switch_info.en_ch_switch_status;
    pst_mac_vap->st_ch_switch_info.en_bw_switch_status = pst_set_chan->st_ch_switch_info.en_bw_switch_status;

    /* aput�����ŵ�ͬ�����ŵ��ı�־λ,��ֹ�����û�����,�Ѵ˱�����ͬ����ȥ */
    pst_mac_vap->st_ch_switch_info.uc_ch_switch_cnt = pst_set_chan->st_ch_switch_info.uc_ch_switch_cnt;
    pst_mac_vap->st_ch_switch_info.en_csa_present_in_bcn = pst_set_chan->st_ch_switch_info.en_csa_present_in_bcn;

    /* ͬ��device��Ϣ */
    pst_mac_device->uc_max_channel = pst_mac_vap->st_channel.uc_chan_number;
    pst_mac_device->en_max_band = pst_mac_vap->st_channel.en_band;
    pst_mac_device->en_max_bandwidth = pst_mac_vap->st_channel.en_bandwidth;

    /* �ŵ������� */
    if (IS_STA(pst_mac_vap)) {
        ul_ret = hmac_check_ap_channel_follow_sta(pst_mac_vap, &pst_mac_vap->st_channel, &uc_ap_follow_channel);
        if (ul_ret == OAL_SUCC) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                "{hmac_chan_switch_to_new_chan_complete::ap channel change from %d to %d}",
                pst_mac_vap->st_channel.uc_chan_number, uc_ap_follow_channel);
        }
    }
    ul_ret = hmac_report_channel_switch(pst_hmac_vap, &(pst_mac_vap->st_channel));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
            "hmac_chan_switch_to_new_chan_complete::hmac_report_channel_switch return fail");
    }

    if (pst_set_chan->en_check_cac == OAL_FALSE) {
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_DFS
    if (OAL_TRUE == hmac_dfs_need_for_cac(pst_mac_device, &pst_hmac_vap->st_vap_base_info)) {
        hmac_dfs_cac_start(pst_mac_device, pst_hmac_vap);
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_2040,
                      "{hmac_chan_switch_to_new_chan_complete::[DFS]CAC START!}");

        return OAL_SUCC;
    }

    hmac_chan_restart_network_after_switch(&(pst_hmac_vap->st_vap_base_info));
#endif

    return OAL_SUCC;
}

void hmac_40M_intol_sync_data(mac_vap_stru *pst_mac_vap,
                              wlan_channel_bandwidth_enum_uint8 en_40M_bandwidth,
                              oal_bool_enum_uint8 en_40M_intol_user)
{
    mac_bandwidth_stru st_band_prot;

    memset_s(&st_band_prot, OAL_SIZEOF(mac_bandwidth_stru), 0, OAL_SIZEOF(mac_bandwidth_stru));

    st_band_prot.en_40M_bandwidth = en_40M_bandwidth;
    st_band_prot.en_40M_intol_user = en_40M_intol_user;
    hmac_40M_intol_sync_event(pst_mac_vap, OAL_SIZEOF(st_band_prot), (uint8_t *)&st_band_prot);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

