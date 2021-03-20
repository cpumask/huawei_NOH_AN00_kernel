

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ͷ�ļ����� */
#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "hmac_wmmac.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_encap_frame_sta.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WMMAC_C

/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

OAL_STATIC uint16_t hmac_mgmt_encap_addts_req(hmac_vap_stru *pst_vap,
                                                uint8_t *puc_data,
                                                mac_wmm_tspec_stru *pst_addts_args)
{
    uint16_t us_index;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* Frame Control Field ��ֻ��Ҫ����Type/Subtypeֵ����������Ϊ0 */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, pst_vap->st_vap_base_info.auc_bssid);

    /* SA��ֵΪdot11MACAddress��ֵ */
    oal_set_mac_addr(puc_data + 10, pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);

    /***************************************************************************/
    /*                Set the contents of the frame body                       */
    /* ----------------------------------------------------------------------- */
    /* |MAC HEADER| Category| Action| Dialog token| Status| Elements| FCS|     */
    /* ----------------------------------------------------------------------- */
    /* |24/30     |1        |1      |1            |1      |         |4   |     */
    /* ----------------------------------------------------------------------- */
    /*                                                                         */
    /***************************************************************************/
    /* ������ָ��frame body��ʼλ�� */
    us_index = MAC_80211_FRAME_LEN;

    /* ����Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_WMMAC_QOS;

    /* ����Action */
    puc_data[us_index++] = MAC_WMMAC_ACTION_ADDTS_REQ;

    /* ����Dialog Token */
    puc_data[us_index++] = pst_vap->uc_ts_dialog_token;

    /* Status �̶�ֵΪ0 */
    puc_data[us_index++] = 0;

    /* ����WMM AC���� */
    us_index += mac_set_wmmac_ie_sta((void *)pst_vap, puc_data + us_index, pst_addts_args);

    /* ���ص�֡�����в�����FCS */
    return us_index;
}


OAL_STATIC uint16_t hmac_mgmt_encap_delts(hmac_vap_stru *pst_vap,
                                            uint8_t *puc_data,
                                            uint8_t *puc_addr,
                                            uint8_t uc_tsid)
{
    uint16_t us_index;
    uint16_t us_ie_len;
    mac_wmm_tspec_stru st_addts_args;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* Frame Control Field ��ֻ��Ҫ����Type/Subtypeֵ����������Ϊ0 */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    if (IS_STA(&(pst_vap->st_vap_base_info))) {
        /* DA is address of STA requesting association */
        oal_set_mac_addr(puc_data + 4, pst_vap->st_vap_base_info.auc_bssid);

        /* SA��ֵΪdot11MACAddress��ֵ */
        oal_set_mac_addr(puc_data + 10,
            pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
        oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);
    } else if (IS_AP(&(pst_vap->st_vap_base_info))) {
        /* DA is puc_addr, user's address */
        oal_set_mac_addr(puc_data + 4, puc_addr);

        /* SA��ֵΪdot11MACAddress��ֵ */
        oal_set_mac_addr(puc_data + 10,
            pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
        oal_set_mac_addr(puc_data + 16,
            pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    }

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /* ----------------------------------------------------------------------- */
    /* |MAC HEADER| Category| Action| Dialog token| Status| Elements| FCS|   */
    /* ----------------------------------------------------------------------- */
    /* |24/30     |1        |1      |1            |1      |         |4   |   */
    /* ----------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* ������ָ��frame body��ʼλ�� */
    us_index = MAC_80211_FRAME_LEN;

    /* ����Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_WMMAC_QOS;

    /* ����Action */
    puc_data[us_index++] = MAC_WMMAC_ACTION_DELTS;

    /* ����Dialog Token */
    puc_data[us_index++] = 0;

    puc_data[us_index++] = 0; /* Status �̶�ֵΪ0 */

    memset_s(&st_addts_args, OAL_SIZEOF(st_addts_args), 0, OAL_SIZEOF(st_addts_args));
    st_addts_args.ts_info.bit_tsid = uc_tsid;

    /* ����WMM AC����������TSID��delts����ֵ��Ϊ0 */
    us_ie_len = mac_set_wmmac_ie_sta((void *)pst_vap, puc_data + us_index, &st_addts_args);

    us_index += us_ie_len;

    /* ���ص�֡�����в�����FCS */
    return us_index;
}


OAL_STATIC uint32_t hmac_mgmt_tx_delts(hmac_vap_stru *pst_hmac_vap,
                                         hmac_user_stru *pst_hmac_user,
                                         mac_wmm_tspec_stru *pst_delts_args)
{
    mac_vap_stru *pst_mac_vap;
    oal_netbuf_stru *pst_delts = OAL_PTR_NULL;
    uint16_t us_frame_len;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* �����¼����ص��ڴ�ָ�� */
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    frw_event_stru *pst_hmac_to_dmac_ctx_event = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_tx_event = OAL_PTR_NULL;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    uint8_t uc_ac_num = 0;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    /* ȷ��vap���ڹ���״̬ */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_tx_delts:: vap has been down/del, vap_state[%d].}",
            pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /* ��������AC���ҵ���ӦTSID��AC��UP */
    if (IS_STA(&pst_hmac_vap->st_vap_base_info)) {
        for (uc_ac_num = 0; uc_ac_num < WLAN_WME_AC_BUTT; uc_ac_num++) {
            if (pst_delts_args->ts_info.bit_tsid == pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_tsid) {
                if (pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].en_ts_status == MAC_TS_NONE) {
                    continue;
                }
                /* ����ҵ��˶�Ӧ��TSID�����Զ�Ӧ��UP����,���¶�Ӧ��TS��Ϣ */
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].en_ts_status = MAC_TS_INIT;
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_tsid = 0xFF;
                pst_delts_args->ts_info.bit_user_prio = pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_up;
                break;
            }
        }
        /* ���û�ҵ���Ӧ��TSID��deltsĬ����up7���� */
        if (uc_ac_num == WLAN_WME_AC_BUTT) {
            pst_delts_args->ts_info.bit_user_prio = 0x7;
        }
    }

    /* ����DELTS����֡�ڴ� */
    pst_delts = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_delts == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_mgmt_tx_delts::pst_addts_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_delts, OAL_TRUE);

    oal_netbuf_prev(pst_delts) = OAL_PTR_NULL;
    oal_netbuf_next(pst_delts) = OAL_PTR_NULL;

    /* ���÷�װ����֡�ӿ� */
    us_frame_len = hmac_mgmt_encap_delts(pst_hmac_vap,
                                         oal_netbuf_data(pst_delts),
                                         pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                         (uint8_t)pst_delts_args->ts_info.bit_tsid);

    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));

    /* ��ֵҪ����Dmac����Ϣ */
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_WMMAC_QOS;
    st_wlan_ctx_action.uc_action = MAC_WMMAC_ACTION_DELTS;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    st_wlan_ctx_action.uc_tidno = pst_delts_args->ts_info.bit_user_prio;
    st_wlan_ctx_action.uc_tsid = pst_delts_args->ts_info.bit_tsid;

    if (EOK != memcpy_s((uint8_t *)(oal_netbuf_data(pst_delts) + us_frame_len),
                        WLAN_MEM_NETBUF_SIZE2 - us_frame_len,
                        (uint8_t *)&st_wlan_ctx_action,
                        OAL_SIZEOF(dmac_ctx_action_event_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail!");
        oal_netbuf_free(pst_delts);
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_delts, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    /* ��ʼ��CB */
    memset_s(oal_netbuf_cb(pst_delts), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_delts);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_BA_WMMAC_DELTS;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_mgmt_tx_delts::pst_event_mem null.}");
        oal_netbuf_free(pst_delts);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_to_dmac_ctx_event = (frw_event_stru *)pst_event_mem->puc_data;
    frw_event_hdr_init(&(pst_hmac_to_dmac_ctx_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    pst_tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    pst_tx_event->pst_netbuf = pst_delts;
    pst_tx_event->us_frame_len = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_tx_delts::frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_delts);
        frw_event_free_m(pst_event_mem);
        return ul_ret;
    }

    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_mgmt_tx_addts_req_timeout(void *p_arg)
{
    hmac_vap_stru *pst_vap = OAL_PTR_NULL; /* vapָ�� */
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_ts_stru *pst_ts_args = OAL_PTR_NULL;
    mac_wmm_tspec_stru st_addts_args;

    if (p_arg == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WMMAC, "{hmac_mgmt_tx_addts_req_timeout::p_arg null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_ts_args = (mac_ts_stru *)p_arg;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_ts_args->us_mac_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WMMAC, "{hmac_mgmt_tx_addts_req_timeout::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_ts_args->uc_vap_id);
    if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_tx_addts_req_timeout::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_ts_args->uc_second_flag == OAL_TRUE) {
        return OAL_SUCC;
    }

    /* ����DELTS֡,ֻ��Ҫ�ı�TSID */
    st_addts_args.ts_info.bit_tsid = pst_ts_args->uc_tsid;
    st_addts_args.ts_info.bit_user_prio = pst_ts_args->uc_up;

    oam_warning_log2(0, OAM_SF_WMMAC, "{hmac_mgmt_tx_addts_req_timeout::AC=[%d],TS_TID=[%d] send delts.}",
                     WLAN_WME_TID_TO_AC(pst_ts_args->uc_up), pst_ts_args->uc_tsid);

    return hmac_mgmt_tx_delts(pst_vap, pst_hmac_user, &st_addts_args);
}


OAL_STATIC uint32_t hmac_mgmt_tx_addts_req(hmac_vap_stru *pst_hmac_vap,
                                             hmac_user_stru *pst_hmac_user,
                                             mac_wmm_tspec_stru *pst_addts_args)
{
    mac_vap_stru *pst_mac_vap;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    oal_netbuf_stru *pst_addts_req = OAL_PTR_NULL;
    uint16_t us_frame_len;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* �����¼����ص��ڴ�ָ�� */
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    frw_event_stru *pst_hmac_to_dmac_ctx_event = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_tx_event = OAL_PTR_NULL;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    uint8_t uc_ac_num;
    mac_ts_stru *pst_ts_use = OAL_PTR_NULL;
    mac_ts_stru *pst_ts_tmp = OAL_PTR_NULL;
    mac_ts_stru *pst_ts_timer = OAL_PTR_NULL;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    /* ȷ��vap���ڹ���״̬ */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
                         "{hmac_mgmt_tx_addts_req::vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }

    /* ��ȡdevice�ṹ */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_mgmt_tx_addba_req::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����ADDTS_REQ����֡�ڴ� */
    pst_addts_req = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_addts_req == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_mgmt_tx_addts_req::pst_addts_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_addts_req, OAL_TRUE);

    oal_netbuf_prev(pst_addts_req) = OAL_PTR_NULL;
    oal_netbuf_next(pst_addts_req) = OAL_PTR_NULL;

    pst_hmac_vap->uc_ts_dialog_token++;
    uc_ac_num = WLAN_WME_TID_TO_AC(pst_addts_args->ts_info.bit_user_prio);

    /* ���÷�װ����֡�ӿ� */
    us_frame_len = hmac_mgmt_encap_addts_req(pst_hmac_vap, oal_netbuf_data(pst_addts_req), pst_addts_args);
    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));
    /* ��ֵҪ����Dmac����Ϣ */
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_WMMAC_QOS;
    st_wlan_ctx_action.uc_action = MAC_WMMAC_ACTION_ADDTS_REQ;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    st_wlan_ctx_action.uc_tidno = pst_addts_args->ts_info.bit_user_prio;
    st_wlan_ctx_action.uc_ts_dialog_token = pst_hmac_vap->uc_ts_dialog_token;
    st_wlan_ctx_action.uc_initiator = pst_addts_args->ts_info.bit_direction;
    st_wlan_ctx_action.uc_tsid = pst_addts_args->ts_info.bit_tsid;
    st_wlan_ctx_action.st_addts_info.uc_ac = uc_ac_num;
    st_wlan_ctx_action.st_addts_info.bit_psb = pst_addts_args->ts_info.bit_apsd;
    st_wlan_ctx_action.st_addts_info.bit_direction = pst_addts_args->ts_info.bit_direction;
    if (EOK != memcpy_s((uint8_t *)(oal_netbuf_data(pst_addts_req) + us_frame_len),
                        WLAN_MEM_NETBUF_SIZE2 - us_frame_len,
                        (uint8_t *)&st_wlan_ctx_action,
                        OAL_SIZEOF(dmac_ctx_action_event_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail!");
        oal_netbuf_free(pst_addts_req);
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_addts_req, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    /* ��ʼ��CB */
    memset_s(oal_netbuf_cb(pst_addts_req), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_addts_req);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_BA_WMMAC_ADDTS_REQ;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_mgmt_tx_addts_req::pst_event_mem null.}");
        oal_netbuf_free(pst_addts_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_to_dmac_ctx_event = (frw_event_stru *)pst_event_mem->puc_data;
    frw_event_hdr_init(&(pst_hmac_to_dmac_ctx_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    pst_tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    pst_tx_event->pst_netbuf = pst_addts_req;
    pst_tx_event->us_frame_len = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_tx_addts_req::frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_addts_req);
        frw_event_free_m(pst_event_mem);
        return ul_ret;
    }

    frw_event_free_m(pst_event_mem);

    /* ���¶�Ӧ��TS��Ϣ */
    pst_ts_use = &pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num];
    pst_ts_tmp = &pst_hmac_user->st_ts_tmp_info[uc_ac_num];

    if (pst_ts_use->en_ts_status == MAC_TS_SUCCESS) {
        pst_ts_tmp->en_ts_status = MAC_TS_INPROGRESS;
        pst_ts_tmp->uc_ts_dialog_token = pst_hmac_vap->uc_ts_dialog_token;
        pst_ts_tmp->uc_tsid = pst_addts_args->ts_info.bit_tsid;
        pst_ts_timer = pst_ts_tmp;
        pst_ts_timer->uc_second_flag = OAL_TRUE;
    } else {
        pst_ts_use->en_ts_status = MAC_TS_INPROGRESS;
        pst_ts_use->uc_ts_dialog_token = pst_hmac_vap->uc_ts_dialog_token;
        pst_ts_use->uc_tsid = pst_addts_args->ts_info.bit_tsid;
        pst_ts_timer = pst_ts_use;
    }

    /* ����ADDTS REQ��ʱ��ʱ�� */
    frw_timer_create_timer_m(&pst_ts_timer->st_addts_timer,
                           hmac_mgmt_tx_addts_req_timeout,
                           WLAN_ADDTS_TIMEOUT,
                           pst_ts_timer,
                           OAL_FALSE,
                           OAM_MODULE_ID_HMAC,
                           pst_device->ul_core_id);

    return OAL_SUCC;
}


OAL_STATIC uint16_t hmac_mgmt_encap_addts_rsp(hmac_vap_stru *pst_vap,
                                                uint8_t *puc_data,
                                                uint8_t *puc_addts_req,
                                                uint16_t us_frame_len)
{
    uint16_t us_index;
    uint8_t *puc_sa = OAL_PTR_NULL;


    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_addts_req, &puc_sa);

    /* ��������ADDTS REQ֡ */
    if (EOK != memcpy_s(puc_data, WLAN_MEM_NETBUF_SIZE2, puc_addts_req, us_frame_len)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_mgmt_encap_addts_rsp::memcpy fail!");
        return 0;
    }

    /* Frame Control Field ��ֻ��Ҫ����Type/Subtypeֵ����������Ϊ0 */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is ADDTS Request frame's SA */
    oal_set_mac_addr(puc_data + 4, puc_sa);

    /* SA��ֵΪdot11MACAddress��ֵ */
    oal_set_mac_addr(puc_data + 10, pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

    /***************************************************************************/
    /*                Set the contents of the frame body                       */
    /* ----------------------------------------------------------------------- */
    /* |MAC HEADER| Category| Action| Dialog token| Status| Elements| FCS|     */
    /* ----------------------------------------------------------------------- */
    /* |24/30     |1        |1      |1            |1      |         |4   |     */
    /* ----------------------------------------------------------------------- */
    /*                                                                         */
    /***************************************************************************/
    /* ������ָ��frame body��ʼλ�� */
    us_index = MAC_80211_FRAME_LEN;

    /* ����Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_WMMAC_QOS;

    /* ����Action */
    puc_data[us_index++] = MAC_WMMAC_ACTION_ADDTS_RSP;

    /* ����Dialog Token */
    us_index++;

    puc_data[us_index++] = 0; /* Status ΪSucc */

    /* ���ص�֡�����в�����FCS */
    return us_frame_len;
}


uint32_t hmac_mgmt_rx_addts_rsp(hmac_vap_stru *pst_hmac_vap,
                                      hmac_user_stru *pst_hmac_user,
                                      uint8_t *puc_payload)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint8_t uc_user_prio;
    uint8_t uc_ac_num;
    uint8_t uc_tsid;
    uint8_t uc_dialog_token;
    mac_wmm_tspec_stru st_tspec_args;
    mac_wmm_tspec_stru *pst_tspec_info = OAL_PTR_NULL;
    mac_ts_stru *pst_ts = OAL_PTR_NULL;
    mac_ts_stru *pst_ts_tmp = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_rsp::null param, %p %p %p.}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /***************************************************************************/
    /*                ADDTS RSP frame body                                     */
    /* ----------------------------------------------------------------------- */
    /* |MAC HEADER| Category| Action| Dialog token| Status| Elements| FCS|     */
    /* ----------------------------------------------------------------------- */
    /* |24/30     |1        |1      |1            |1      |         |4   |     */
    /* ----------------------------------------------------------------------- */
    /*                                                                         */
    /***************************************************************************/

    pst_tspec_info = (mac_wmm_tspec_stru *)(puc_payload + 12);
    uc_user_prio = pst_tspec_info->ts_info.bit_user_prio;

    /* Э��֧��tidΪ0~15,02ֻ֧��tid0~7 */
    if (uc_user_prio >= WLAN_TID_MAX_NUM) {
        /* ����tid > 7��respֱ�Ӻ��� */
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_rsp::addts rsp tsid[%d]} token[%d] status[%d]",
            uc_user_prio, puc_payload[2], puc_payload[3]);
        return OAL_SUCC;
    }
    uc_ac_num = WLAN_WME_TID_TO_AC(uc_user_prio);
    uc_tsid = pst_tspec_info->ts_info.bit_tsid;

    /* ���֮ǰ�����ts��Ϣ */
    pst_ts = &(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num]);
    uc_dialog_token = puc_payload[2];

    pst_ts_tmp = &pst_hmac_user->st_ts_tmp_info[uc_ac_num];
    if (pst_ts->en_ts_status == MAC_TS_SUCCESS) {
        if (pst_ts_tmp->en_ts_status == MAC_TS_INPROGRESS) {
            if (pst_ts_tmp->st_addts_timer.en_is_registerd == OAL_TRUE) {
                frw_timer_immediate_destroy_timer_m(&pst_ts_tmp->st_addts_timer);
            }
            pst_ts_tmp->en_ts_status = MAC_TS_NONE;
            pst_ts_tmp->uc_second_flag = OAL_FALSE;
        }
        return OAL_SUCC;
    }

    if (pst_ts->uc_tsid != uc_tsid) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_rsp::addba rsp tsid[%d],old_tsid[%d]} ",
            uc_tsid, pst_ts->uc_tsid);
        return OAL_SUCC;
    }

    if (uc_dialog_token != pst_ts->uc_ts_dialog_token) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_rsp::addts rspuc_dialog_token wrong.rsp dialog[%d], req dialog[%d]}",
            uc_dialog_token, pst_ts->uc_ts_dialog_token);
        return OAL_SUCC;
    }

    frw_timer_immediate_destroy_timer_m(&pst_ts->st_addts_timer);

    /* �����ӦTSID��TSδ����������DELTS */
    if (pst_ts->en_ts_status == MAC_TS_INIT) {
        /* ����DELTS֡ */
        st_tspec_args.ts_info.bit_tsid = uc_tsid;
        st_tspec_args.ts_info.bit_user_prio = uc_user_prio;
        st_tspec_args.ts_info.bit_acc_policy = 1;
        hmac_mgmt_tx_delts(pst_hmac_vap, pst_hmac_user, &st_tspec_args);
        return OAL_SUCC;
    }

    /* ����ADDTS RSP��Ϣ������dmac TS״̬ */
    if (puc_payload[3] == MAC_SUCCESSFUL_STATUSCODE) {
        pst_ts->en_ts_status = MAC_TS_SUCCESS;
    } else {
        memset_s(pst_ts, OAL_SIZEOF(mac_ts_stru), 0, OAL_SIZEOF(mac_ts_stru));
        pst_ts->en_ts_status = MAC_TS_INIT;
    }

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
                     "{hmac_mgmt_rx_addts_rsp::medium_time[%d], tsid[%d], uc_up[%d], en_direction[%d].}",
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].ul_medium_time,
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_tsid,
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_up,
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].en_direction);

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
                     "{hmac_mgmt_rx_addts_rsp::ts status[%d], dialog_token[%d], vap id[%d], ac num[%d].}",
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].en_ts_status,
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_ts_dialog_token,
                     pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num].uc_vap_id,
                     uc_ac_num);

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_delts(hmac_vap_stru *pst_hmac_vap,
                                  hmac_user_stru *pst_hmac_user,
                                  uint8_t *puc_payload)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint8_t uc_ac_num;
    uint8_t uc_tsid;
    mac_wmm_tspec_stru *pst_tspec_info = OAL_PTR_NULL;
    mac_ts_stru *pst_ts = OAL_PTR_NULL;
    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_delts::null param, %p %p %p.}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /*************************************************************************/
    /*                DELTS frame body                     */
    /* ----------------------------------------------------------------------- */
    /* |MAC HEADER| Category| Action| Dialog token| Status| Elements| FCS|   */
    /* ----------------------------------------------------------------------- */
    /* |24/30     |1        |1      |1            |1      |         |4   |   */
    /* ----------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/

    pst_tspec_info = (mac_wmm_tspec_stru *)(puc_payload + 12);
    uc_tsid = pst_tspec_info->ts_info.bit_tsid;

    /* Э��֧��tidΪ0~15,02ֻ֧��tid0~7 */
    if (uc_tsid >= WLAN_TID_MAX_NUM) {
        /* ����tid > 7��respֱ�Ӻ��� */
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_delts::tsid[%d]} token[%d] status[%d]",
            uc_tsid, puc_payload[2], puc_payload[3]);
        return OAL_SUCC;
    }
    /* ��������AC���ҵ���Ӧ��TSID�����TS��Ϣ */
    for (uc_ac_num = 0; uc_ac_num < WLAN_WME_AC_BUTT; uc_ac_num++) {
        /* ���֮ǰ�����ts��Ϣ */
        pst_ts = &(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num]);
        if (pst_ts->uc_tsid != uc_tsid) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
                "{hmac_mgmt_rx_delts::tsid[%d],old_tsid[%d]} ", uc_tsid, pst_ts->uc_tsid);
            continue;
        }

        /* ����ѱ����TS��Ϣ */
        if (pst_ts->st_addts_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&(pst_ts->st_addts_timer));
        }
        memset_s(pst_ts, OAL_SIZEOF(mac_ts_stru), 0, OAL_SIZEOF(mac_ts_stru));
        if (OAL_TRUE == mac_mib_get_QAPEDCATableMandatory(&(pst_hmac_vap->st_vap_base_info), uc_ac_num)) {
            pst_ts->en_ts_status = MAC_TS_INIT;
        } else {
            pst_ts->en_ts_status = MAC_TS_NONE;
        }
        pst_ts->uc_tsid = 0xFF;
    }

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_addts_req_frame(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint16_t us_user_idx = 0;
    uint8_t auc_sta_addr[WLAN_MAC_ADDR_LEN];
    uint32_t ul_ret;

    oal_netbuf_stru *pst_addts_rsp = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint16_t us_frame_len;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_netbuf)) {
        oam_error_log2(0, OAM_SF_WMMAC,
                       "{hmac_mgmt_rx_addts_req_frame::pst_hmac_vap = [%x], pst_netbuf = [%x]!}\r\n",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_netbuf);

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    mac_get_address2((uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl), auc_sta_addr, WLAN_MAC_ADDR_LEN);

    ul_ret = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, &us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WMMAC,
                         "{hmac_mgmt_rx_addts_req_frame::mac_vap_find_user_by_macaddr failed[%d].}", ul_ret);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(us_user_idx);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_req_frame::pst_mac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����ADDTS_RSP����֡�ڴ� */
    pst_addts_rsp = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_addts_rsp == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_req_frame::pst_addts_rsp null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_addts_rsp, OAL_TRUE);

    memset_s(oal_netbuf_cb(pst_addts_rsp), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_addts_rsp);

    oal_netbuf_prev(pst_addts_rsp) = OAL_PTR_NULL;
    oal_netbuf_next(pst_addts_rsp) = OAL_PTR_NULL;

    /* ���÷�װ����֡�ӿ� */
    us_frame_len = hmac_mgmt_encap_addts_rsp(pst_hmac_vap, oal_netbuf_data(pst_addts_rsp),
        oal_netbuf_data(pst_netbuf), pst_rx_ctrl->us_frame_len);
    if (us_frame_len == 0) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_req_frame::encap_addts_rsp, but frame len is zero.}");
        oal_netbuf_free(pst_addts_rsp);
        return OAL_FAIL;
    }

    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_idx; /* dmac������Ҫ��mpdu���� */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len;   /* ���������Ҫ��ȡuser�ṹ�� */

    oal_netbuf_put(pst_addts_rsp, us_frame_len);

    /* Buffer this frame in the Memory Queue for transmission */
    ul_ret = hmac_tx_mgmt_send_event(&(pst_hmac_vap->st_vap_base_info), pst_addts_rsp, us_frame_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_addts_rsp);
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WMMAC,
            "{hmac_mgmt_rx_addts_req_frame::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t hmac_config_addts_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_wmm_tspec_stru_param_stru *pst_addts_req = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_wmm_tspec_stru st_addts_args;
    uint8_t uc_ac;

    if (g_en_wmmac_switch == OAL_FALSE) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_config_addts_req::wmmac switch is false[%d].}", g_en_wmmac_switch);
        return OAL_SUCC;
    }

    pst_addts_req = (mac_cfg_wmm_tspec_stru_param_stru *)puc_param;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_config_addts_req::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�û���Ӧ������ */
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_config_addts_req::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �ж϶�ӦAC��ACMλ��ֻ�и�AC��ACMΪ1ʱ����������TS�� */
    uc_ac = WLAN_WME_TID_TO_AC(pst_addts_req->ts_info.bit_user_prio);
    if (OAL_FALSE == mac_mib_get_QAPEDCATableMandatory(&(pst_hmac_vap->st_vap_base_info), uc_ac)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_config_addts_req::user prio[%d] is false.}", uc_ac);
        return OAL_SUCC;
    }

    st_addts_args.ts_info.bit_tsid = pst_addts_req->ts_info.bit_tsid;
    st_addts_args.ts_info.bit_direction = pst_addts_req->ts_info.bit_direction;
    st_addts_args.ts_info.bit_apsd = pst_addts_req->ts_info.bit_apsd;
    st_addts_args.ts_info.bit_user_prio = pst_addts_req->ts_info.bit_user_prio;
    st_addts_args.us_norminal_msdu_size = pst_addts_req->us_norminal_msdu_size;
    st_addts_args.us_max_msdu_size = pst_addts_req->us_max_msdu_size;
    st_addts_args.ul_min_data_rate = pst_addts_req->ul_min_data_rate;
    st_addts_args.ul_mean_data_rate = pst_addts_req->ul_mean_data_rate;
    st_addts_args.ul_peak_data_rate = pst_addts_req->ul_peak_data_rate;
    st_addts_args.ul_min_phy_rate = pst_addts_req->ul_min_phy_rate;
    st_addts_args.us_surplus_bw = pst_addts_req->us_surplus_bw;

    /* ����ADDTS REQ������TS */
    return hmac_mgmt_tx_addts_req(pst_hmac_vap, pst_hmac_user, &st_addts_args);
}


uint32_t hmac_config_delts(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_wmm_tspec_stru_param_stru *pst_delts = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_wmm_tspec_stru st_delts_args;

    if (g_en_wmmac_switch == OAL_FALSE) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_config_delts::wmmac switch is false[%d].}", g_en_wmmac_switch);
        return OAL_SUCC;
    }

    pst_delts = (mac_cfg_wmm_tspec_stru_param_stru *)puc_param;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_config_delts::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�û���Ӧ������ */
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_config_delts::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_delts_args.ts_info.bit_tsid = pst_delts->ts_info.bit_tsid;

    return hmac_mgmt_tx_delts(pst_hmac_vap, pst_hmac_user, &st_delts_args);
}


uint32_t hmac_config_wmmac_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    uint8_t uc_ac_idx;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_cfg_wmm_ac_param_stru *pst_wmm_ac_param = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC, "{hmac_config_wmmac_switch::pst_hmac_vap is  NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_wmm_ac_param = (mac_cfg_wmm_ac_param_stru *)puc_param;
    g_en_wmmac_switch = pst_wmm_ac_param->en_wmm_ac_switch;
    pst_hmac_vap->en_wmmac_auth_flag = pst_wmm_ac_param->en_auth_flag;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_config_wmmac_switch::pst_hmac_user is deleted already.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* �ر�wmmac����ʱ����Ҫɾ��user�����е�TS��Ϣ */
    if (!g_en_wmmac_switch) {
        for (uc_ac_idx = 0; uc_ac_idx < WLAN_WME_AC_BUTT; uc_ac_idx++) {
            if (pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_idx].st_addts_timer.en_is_registerd == OAL_TRUE) {
                frw_timer_immediate_destroy_timer_m(
                    &(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_idx].st_addts_timer));
            }
            memset_s(&(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_idx]),
                     OAL_SIZEOF(mac_ts_stru), 0, OAL_SIZEOF(mac_ts_stru));
            pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_idx].en_ts_status = MAC_TS_NONE;
            pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_idx].uc_tsid = 0xFF;

            if (pst_hmac_user->st_ts_tmp_info[uc_ac_idx].st_addts_timer.en_is_registerd == OAL_TRUE) {
                frw_timer_immediate_destroy_timer_m(&(pst_hmac_user->st_ts_tmp_info[uc_ac_idx].st_addts_timer));
            }
            memset_s(&(pst_hmac_user->st_ts_tmp_info[uc_ac_idx]),
                     OAL_SIZEOF(mac_ts_stru), 0, OAL_SIZEOF(mac_ts_stru));
            pst_hmac_user->st_ts_tmp_info[uc_ac_idx].en_ts_status = MAC_TS_NONE;
            pst_hmac_user->st_ts_tmp_info[uc_ac_idx].uc_tsid = 0xFF;
        }
    }

    /***************************************************************************
    ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_WMMAC_SWITCH, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WMMAC,
            "{hmac_config_wmmac_switch::hmac_config_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#endif  // _PRE_WLAN_FEATURE_WMMAC

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

