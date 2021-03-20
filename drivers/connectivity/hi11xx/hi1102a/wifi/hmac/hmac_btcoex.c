
#ifndef __HMAC_BTCOEX_C__
#define __HMAC_BTCOEX_C__

#ifdef _PRE_WLAN_FEATURE_BTCOEX

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "hmac_vap.h"
#include "hmac_ext_if.h"
#include "mac_data.h"
#include "hmac_resource.h"
#include "hmac_user.h"
#include "hmac_btcoex.h"
#include "hmac_fsm.h"
#include "hmac_mgmt_sta.h"
#include "hmac_roam_main.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BTCOEX_C
oal_bool_enum_uint8 g_en_btcoex_reject_addba = OAL_FALSE;

OAL_STATIC oal_uint32 hmac_btcoex_delba_foreach_tid(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user,
                                                    mac_cfg_delba_req_param_stru *pst_mac_cfg_delba_param)
{
    oal_uint32 ul_ret = 0;

    oal_set_mac_addr(pst_mac_cfg_delba_param->auc_mac_addr, pst_mac_user->auc_user_mac_addr);

    for (pst_mac_cfg_delba_param->uc_tidno = 0; pst_mac_cfg_delba_param->uc_tidno < WLAN_TID_MAX_NUM;
         pst_mac_cfg_delba_param->uc_tidno++) {
        ul_ret = hmac_config_delba_req(pst_mac_vap, 0, (oal_uint8 *)pst_mac_cfg_delba_param);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{hmac_btcoex_delba_foreach_tid::ul_ret:%d,tid: %d}",
                             ul_ret, pst_mac_cfg_delba_param->uc_tidno);
            return ul_ret;
        }
    }
    return ul_ret;
}


OAL_STATIC oal_uint32 hmac_btcoex_delba_from_user(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user)
{
    oal_uint32 ul_ret;
    mac_cfg_delba_req_param_stru st_mac_cfg_delba_param;
    hmac_vap_stru *pst_hmac_vap;

    if ((pst_mac_vap == OAL_PTR_NULL) || (pst_hmac_user == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_COEX, "hmac_btcoex_delba_from_user:pst_mac_vap=%x,pst_hmac_user=%x",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }
    st_mac_cfg_delba_param.en_direction = MAC_RECIPIENT_DELBA;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{hmac_btcoex_delba_from_user::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ���ں�����AP�������Ѿ����ڵ绰ҵ��ʱ��������ɾBA�߼� */
    if (pst_hmac_user->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow == OAL_FALSE) {
        if (HMAC_BTCOEX_GET_BLACKLIST_TYPE(pst_hmac_user) == BTCOEX_BLACKLIST_TPYE_FIX_BASIZE) {
            OAM_WARNING_LOG0(0, OAM_SF_COEX, "{hmac_btcoex_delba_from_user::DO NOT DELBA.}");
        } else {
            OAM_WARNING_LOG0(0, OAM_SF_COEX, "{hmac_btcoex_delba_from_user::need to reassoc to READDBA.}");

            /* ����reassoc req */
            hmac_roam_start(pst_hmac_vap, ROAM_SCAN_CHANNEL_ORG_0, OAL_FALSE, ROAM_TRIGGER_COEX);

            /* �ع���֮��ˢ��Ϊ�������ۺ� */
            pst_hmac_user->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow = OAL_TRUE;

            /* ��֤wifi�ָ��ۺ�64 */
            pst_hmac_user->st_hmac_user_btcoex.uc_ba_size = 0;
        }

        return OAL_FAIL;
    } else if (hmac_btcoex_check_exception_in_list(pst_hmac_vap,
                                                   pst_hmac_user->st_user_base_info.auc_user_mac_addr) == OAL_TRUE) {
        /* �ں�����֮�У�����845N������������ʹ��������û�н��������������������Ȼ��Ҫ�����ع�������Ȼ�Զ˲��ظ�arp rsp */
        if ((pst_hmac_user->st_hmac_user_btcoex.uc_ba_size == WLAN_AMPDU_RX_BA_LUT_WSIZE) ||
            (pst_hmac_user->st_hmac_user_btcoex.uc_rx_no_pkt_count > 2)) { /* ��ʱʱ����û���յ�֡�Ĵ�������2 */
            OAM_WARNING_LOG2(0, OAM_SF_COEX,
                             "{hmac_btcoex_delba_from_user::blacklist user ba_size[%d],rx_no_pkt_count[%d].}",
                             pst_hmac_user->st_hmac_user_btcoex.uc_ba_size,
                             pst_hmac_user->st_hmac_user_btcoex.uc_rx_no_pkt_count);
            hmac_roam_start(pst_hmac_vap, ROAM_SCAN_CHANNEL_ORG_0, OAL_FALSE, ROAM_TRIGGER_COEX);
            return OAL_FAIL;
        } else {
            /* �绰������, �ں������е�AP���ٽ����ۺ� */
            pst_hmac_user->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow = OAL_FALSE;
        }
    }
    ul_ret = hmac_btcoex_delba_foreach_tid(pst_mac_vap, &(pst_hmac_user->st_user_base_info), &st_mac_cfg_delba_param);

    return ul_ret;
}


oal_uint32 hmac_btcoex_rx_delba_trigger(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    hmac_vap_stru *pst_hmac_vap;
    hmac_user_stru *pst_hmac_user;
    dmac_to_hmac_btcoex_rx_delba_trigger_event_stru *pst_dmac_to_hmac_btcoex_rx_delba;
    oal_uint32 ul_ret;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(pst_hmac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{hmac_btcoex_rx_delba_trigger::pst_hmac_vap is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_to_hmac_btcoex_rx_delba = (dmac_to_hmac_btcoex_rx_delba_trigger_event_stru *)puc_param;

    pst_hmac_user = mac_res_get_hmac_user(pst_dmac_to_hmac_btcoex_rx_delba->us_user_id);
    if (OAL_UNLIKELY(pst_hmac_user == OAL_PTR_NULL)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                         "{hmac_btcoex_rx_delba_trigger::pst_hmac_user is null! user_id is %d.}",
                         pst_dmac_to_hmac_btcoex_rx_delba->us_user_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_user->st_hmac_user_btcoex.uc_ba_size = pst_dmac_to_hmac_btcoex_rx_delba->uc_ba_size;

    if (pst_dmac_to_hmac_btcoex_rx_delba->en_need_delba == OAL_TRUE) {
        /* ˢ�¹��津��ɾ��BA��� */
        pst_hmac_user->st_hmac_user_btcoex.en_delba_btcoex_trigger = OAL_TRUE;
    }

    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                     "{hmac_btcoex_rx_delba_trigger:delba size:%d, need_delba:%d, delba_trigger:%d.}",
                     pst_dmac_to_hmac_btcoex_rx_delba->uc_ba_size, pst_dmac_to_hmac_btcoex_rx_delba->en_need_delba,
                     pst_hmac_user->st_hmac_user_btcoex.en_delba_btcoex_trigger);

    if (pst_dmac_to_hmac_btcoex_rx_delba->en_need_delba) {
        ul_ret = hmac_btcoex_delba_from_user(pst_mac_vap, pst_hmac_user);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                             "{hmac_btcoex_rx_delba_trigger:delba send failed:ul_ret: %d}", ul_ret);
            return ul_ret;
        }
    }
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_delba_send_timeout(oal_void *p_arg)
{
    hmac_user_stru *pst_hmac_user;
    hmac_user_btcoex_stru *pst_hmac_user_btcoex;
    hmac_btcoex_arp_req_process_stru *pst_hmac_btcoex_arp_req_process;
    oal_uint32 ui_val;

    pst_hmac_user = (hmac_user_stru *)p_arg;
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_COEX, "{hmac_delba_send_timeout::pst_hmac_user is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_user_btcoex = &(pst_hmac_user->st_hmac_user_btcoex);

    pst_hmac_btcoex_arp_req_process = &(pst_hmac_user_btcoex->st_hmac_btcoex_arp_req_process);

    ui_val = oal_atomic_read(&(pst_hmac_btcoex_arp_req_process->ul_rx_unicast_pkt_to_lan));
    if (ui_val == 0) {
        mac_vap_stru *pst_mac_vap;

        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
        if (pst_mac_vap == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(0, OAM_SF_COEX, "{hmac_delba_send_timeout::pst_mac_vap is null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{hmac_delba_send_timeout:rx_pkt:0}");

        pst_hmac_user_btcoex->uc_rx_no_pkt_count++;
        if (pst_hmac_user_btcoex->uc_rx_no_pkt_count > 2) { /* ��ʱʱ����û���յ�֡�Ĵ�������2��hmacɾ��BA */
            hmac_btcoex_delba_from_user(pst_mac_vap, pst_hmac_user);
        }
    } else {
        pst_hmac_user_btcoex->uc_rx_no_pkt_count = 0;
    }

    oal_atomic_set(&pst_hmac_btcoex_arp_req_process->ul_rx_unicast_pkt_to_lan, 0);

    return OAL_SUCC;
}


oal_void hmac_btcoex_arp_fail_delba_process(oal_netbuf_stru *pst_netbuf, mac_vap_stru *pst_mac_vap)
{
    oal_uint8 uc_data_type;
    hmac_btcoex_arp_req_process_stru *pst_hmac_btcoex_arp_req_process;
    hmac_user_stru *pst_hmac_user;
    hmac_user_btcoex_stru *pst_hmac_user_btcoex;
    mac_ether_header_stru *pst_mac_ether_hdr;

    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }

    pst_mac_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);

    pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->uc_assoc_vap_id);
    if (OAL_UNLIKELY(pst_hmac_user == OAL_PTR_NULL)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                         "{hmac_btcoex_arp_fail_delba_process::pst_hmac_user is null!assoc_vap is %d.}",
                         pst_mac_vap->uc_assoc_vap_id);
        return;
    }

    pst_hmac_user_btcoex = &(pst_hmac_user->st_hmac_user_btcoex);

    if ((pst_hmac_user_btcoex->uc_ba_size > 0) && (pst_hmac_user_btcoex->uc_ba_size < WLAN_AMPDU_RX_BA_LUT_WSIZE)) {
        /* ���������Ѿ�����飬����û��Ҫ��������� */
        uc_data_type = mac_get_data_type_from_8023((oal_uint8 *)pst_mac_ether_hdr, MAC_NETBUFF_PAYLOAD_ETH);

        pst_hmac_btcoex_arp_req_process = &(pst_hmac_user_btcoex->st_hmac_btcoex_arp_req_process);

        /* ���ͷ��򴴽���ʱ�� */
        if ((uc_data_type == MAC_DATA_ARP_REQ) &&
            (pst_hmac_btcoex_arp_req_process->st_delba_opt_timer.en_is_registerd == OAL_FALSE)) {
            /* ÿ��������ʱ��֮ǰ����,��֤ͳ�Ƶ�ʱ�� */
            oal_atomic_set(&(pst_hmac_btcoex_arp_req_process->ul_rx_unicast_pkt_to_lan), 0);

            FRW_TIMER_CREATE_TIMER(&(pst_hmac_btcoex_arp_req_process->st_delba_opt_timer),
                                   hmac_delba_send_timeout,
                                   500, /* ��ʱʱ��Ϊ500ms */
                                   pst_hmac_user,
                                   OAL_FALSE,
                                   OAM_MODULE_ID_HMAC,
                                   pst_mac_vap->ul_core_id);
        }
    }
}


oal_uint32 hmac_btcoex_check_exception_in_list(oal_void *p_arg, oal_uint8 *auc_addr)
{
    hmac_btcoex_delba_exception_stru *pst_btcoex_exception;
    oal_uint8 uc_index;
    hmac_device_stru *pst_hmac_device;
    hmac_vap_stru *pst_hmac_vap;

    if ((p_arg == OAL_PTR_NULL) || (auc_addr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_COEX, "hmac_btcoex_check_exception_in_list:p_arg=%x,auc_addr=%x", (uintptr_t)p_arg,
                       (uintptr_t)auc_addr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)p_arg;
    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_COEX,
                       "{hmac_btcoex_check_exception_in_list::pst_hmac_device null}");
        return OAL_FALSE;
    }

    for (uc_index = 0; uc_index < MAX_BTCOEX_BSS_IN_BL; uc_index++) {
        pst_btcoex_exception = &(pst_hmac_device->st_hmac_device_btcoex.ast_hmac_btcoex_delba_exception[uc_index]);

        if ((pst_btcoex_exception->uc_used != 0) &&
            (oal_compare_mac_addr(pst_btcoex_exception->auc_user_mac_addr, auc_addr) == 0)) {
            OAM_WARNING_LOG4(0, OAM_SF_COEX,
                             "{hmac_btcoex_check_exception_in_list:Find in blacklist,addr->%02x:%02x:XX:XX:%02x:%02x.}",
                             auc_addr[0], auc_addr[1], auc_addr[4], auc_addr[5]); /* auc_addr 0��1��4��5byte�����ӡ */
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


oal_void hmac_btcoex_blacklist_handle_init(oal_void *p_arg)
{
    hmac_user_stru *pst_hmac_user;
    if (p_arg == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_COEX, "hmac_btcoex_blacklist_handle_init:p_arg is null");
        return;
    }
    pst_hmac_user = (hmac_user_stru *)p_arg;
    pst_hmac_user->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_blacklist_tpye = BTCOEX_BLACKLIST_TPYE_NOT_AGGR;

    OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_COEX,
                     "{hmac_btcoex_blacklist_handle_init::en_blacklist_tpye[%d]!}",
                     pst_hmac_user->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_blacklist_tpye);
}


OAL_STATIC oal_void hmac_btcoex_add_exception_to_list(hmac_vap_stru *pst_hmac_vap, oal_uint8 *auc_mac_addr)
{
    hmac_btcoex_delba_exception_stru *pst_btcoex_exception;
    hmac_device_btcoex_stru *pst_hmac_device_btcoex;
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_COEX,
                       "{hmac_btcoex_check_exception_in_list::pst_hmac_device null}");
        return;
    }

    pst_hmac_device_btcoex = &(pst_hmac_device->st_hmac_device_btcoex);

    if (pst_hmac_device_btcoex->uc_exception_bss_index >= MAX_BTCOEX_BSS_IN_BL) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_COEX,
                         "{hmac_btcoex_add_exception_to_list::already reach max num:%d.}", MAX_BTCOEX_BSS_IN_BL);
        pst_hmac_device_btcoex->uc_exception_bss_index = 0;
    }
    pst_btcoex_exception =
    &(pst_hmac_device_btcoex->ast_hmac_btcoex_delba_exception[pst_hmac_device_btcoex->uc_exception_bss_index]);
    oal_set_mac_addr(pst_btcoex_exception->auc_user_mac_addr, auc_mac_addr);
    pst_btcoex_exception->uc_type = 0;
    pst_btcoex_exception->uc_used = 1;

    pst_hmac_device_btcoex->uc_exception_bss_index++;
}


oal_void hmac_btcoex_check_rx_same_baw_start_from_addba_req(hmac_vap_stru *pst_hmac_vap,
                                                            hmac_user_stru *pst_hmac_user,
                                                            mac_ieee80211_frame_stru *pst_frame_hdr,
                                                            oal_uint8 *puc_action)
{
    hmac_user_btcoex_stru *pst_hmac_user_btcoex;
    hmac_btcoex_addba_req_stru *pst_hmac_btcoex_addba_req;
    oal_uint16 us_baw_start;
    oal_uint8 uc_tid;

    pst_hmac_user_btcoex = &(pst_hmac_user->st_hmac_user_btcoex);

    pst_hmac_btcoex_addba_req = &(pst_hmac_user_btcoex->st_hmac_btcoex_addba_req);

    /* �����յ�addba req��start numһ���Ҳ����ش�֡����Ϊ�Զ��ƴ����� */
    if ((pst_frame_hdr->st_frame_control.bit_retry == OAL_TRUE) &&
        (pst_frame_hdr->bit_seq_num == pst_hmac_btcoex_addba_req->us_last_seq_num)) {
        OAM_WARNING_LOG0(0, OAM_SF_COEX,
                         "{hmac_btcoex_check_rx_same_baw_start_from_addba_req::retry addba req.}");
        return;
    }

    /******************************************************************/
    /* ADDBA Request Frame - Frame Body */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | Parameters | Timeout | SSN     | */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      | 2          | 2       | 2       | */
    /* --------------------------------------------------------------- */
    /******************************************************************/
    us_baw_start = (puc_action[7] >> 4) | (puc_action[8] << 4); /* puc_action[7]��4λ��puc_action[8]��4λƴ��baw start */
    uc_tid = (puc_action[3] & 0x3C) >> 2; /* puc_action[3]��Parameters���ֽڣ�����2-5bit����2λ��ֵ��tid */
    if (uc_tid != 0) {
        OAM_WARNING_LOG0(0, OAM_SF_COEX,
                         "{hmac_btcoex_check_rx_same_baw_start_from_addba_req::uc_tid != 0.}");
        return;
    }

    if ((us_baw_start != 0) && (us_baw_start == pst_hmac_btcoex_addba_req->us_last_baw_start) &&
        (pst_hmac_user_btcoex->uc_rx_no_pkt_count > 2)) { /* ��ʱʱ����û���յ�֡�Ĵ�������2 */
        OAM_WARNING_LOG1(0, OAM_SF_COEX,
                         "{check_rx_same_baw_start_from_addba_req::baw_start:%d, delba will forbidden.}", us_baw_start);
        pst_hmac_btcoex_addba_req->en_ba_handle_allow = OAL_FALSE;

        if (hmac_btcoex_check_exception_in_list(pst_hmac_vap, pst_frame_hdr->auc_address2) == OAL_FALSE) {
            OAM_WARNING_LOG0(0, OAM_SF_COEX,
                             "{hmac_btcoex_check_rx_same_baw_start_from_addba_req::write down to file.}");
            hmac_btcoex_add_exception_to_list(pst_hmac_vap, pst_frame_hdr->auc_address2);
        }

        if (HMAC_BTCOEX_GET_BLACKLIST_TYPE(pst_hmac_user) == BTCOEX_BLACKLIST_TPYE_FIX_BASIZE) {
            /* ����ȥ��֤֡��AP */
            hmac_mgmt_send_disassoc_frame(&pst_hmac_vap->st_vap_base_info,
                                          pst_frame_hdr->auc_address2,
                                          MAC_UNSPEC_REASON,
                                          pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);

            /* ɾ����Ӧ�û� */
            hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
            hmac_sta_handle_disassoc_rsp(pst_hmac_vap, MAC_AUTH_NOT_VALID);
        }
    }

    pst_hmac_btcoex_addba_req->us_last_baw_start = us_baw_start;
    pst_hmac_btcoex_addba_req->us_last_seq_num = pst_frame_hdr->bit_seq_num;
}


oal_uint32 hmac_config_print_btcoex_status(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    oal_uint32 ul_ret;
    hmac_btcoex_delba_exception_stru *pst_btcoex_exception;
    oal_uint8 uc_index;
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{hmac_btcoex_check_exception_in_list::hmac_device null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /***************************************************************************
        ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_BTCOEX_STATUS_PRINT, us_len, puc_param);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                         "{hmac_config_print_btcoex_status::send event return err code [%d].}", ul_ret);
    }

    for (uc_index = 0; uc_index < MAX_BTCOEX_BSS_IN_BL; uc_index++) {
        pst_btcoex_exception = &(pst_hmac_device->st_hmac_device_btcoex.ast_hmac_btcoex_delba_exception[uc_index]);
        if (pst_btcoex_exception->uc_used != 0) {
            OAM_WARNING_LOG4(0, OAM_SF_COEX,
                             "{hmac_config_print_btcoex_status::addr->%02x:%02x:XX:XX:%02x:%02x.}",
                             pst_btcoex_exception->auc_user_mac_addr[0],
                             pst_btcoex_exception->auc_user_mac_addr[1],
                             pst_btcoex_exception->auc_user_mac_addr[4], /* auc_user_mac_addr��4��5byteΪ���������ӡ */
                             pst_btcoex_exception->auc_user_mac_addr[5]);
        }
    }

    return ul_ret;
}


oal_uint32 hmac_btcoex_check_by_ba_size(hmac_user_stru *pst_hmac_user)
{
    hmac_user_btcoex_stru *pst_hmac_user_btcoex;

    pst_hmac_user_btcoex = &(pst_hmac_user->st_hmac_user_btcoex);

    if ((pst_hmac_user_btcoex->uc_ba_size > 0) && (pst_hmac_user_btcoex->uc_ba_size < WLAN_AMPDU_RX_BA_LUT_WSIZE)) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


oal_void hmac_btcoex_process_btble_status(mac_device_stru *pst_mac_device,
                                          hal_btcoex_btble_status_stru *pst_btble_status_old,
                                          hal_btcoex_btble_status_stru *pst_btble_status_new)
{
    hmac_user_stru *pst_hmac_user;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    mac_device_find_legacy_sta(pst_mac_device, &pst_mac_vap);
    if (pst_mac_vap == OAL_PTR_NULL) {
        return;
    }
    pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->uc_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        return;
    }
    /* BT�ɹرյ��� */
    if ((pst_btble_status_new->un_bt_status.st_bt_status.bit_bt_on == OAL_TRUE) &&
        (pst_btble_status_old->un_bt_status.st_bt_status.bit_bt_on == OAL_FALSE)) {
        /* ������泡�����Ǿܾ�����BA������BT��ʱ����ɾBA�Ķ��� */
        if (pst_hmac_user->st_hmac_user_btcoex.en_ba_type == BTCOEX_BA_TYPE_REJECT) {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "hmac_btcoex_process_btble_status:reject addba");
            hmac_btcoex_delba_from_user(pst_mac_vap, pst_hmac_user);
        }
    }
    return;
}


oal_void hmac_btcoex_ap_tpye_identify(mac_vap_stru *pst_mac_vap,
                                      oal_uint8 *puc_mac_addr,
                                      mac_bss_dscr_stru *pst_bss_dscr,
                                      mac_ap_type_enum_uint8 *pen_ap_type)
{
    mac_ap_type_enum_uint8 en_ap_type = 0;

    if (MAC_IS_APPLE_AP(puc_mac_addr) || MAC_IS_TPLINK_7500(puc_mac_addr) || MAC_IS_FEIXUN_K3(puc_mac_addr) ||
        MAC_IS_LINKSYS_1900(puc_mac_addr)) {
        /* ��AP��ʶ��,��Щ·������02A����֤���پ���Ҫ��Ҫ������֡��null֡��Ϊself_cts */
        if (pst_bss_dscr->en_btcoex_blacklist_chip_oui == OAL_TRUE) {
            en_ap_type |= MAC_AP_TYPE_BTCOEX_PS_BLACKLIST;
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "hmac_btcoex_ap_tpye_identify: add ps blacklist!");
        }
    }
    /* JCG·����Ϊ�˼���aptxHD��660,��Ҫ�ر�cts�ظ����� */
    if (MAC_IS_JCG_AP(puc_mac_addr)) {
        if (pst_bss_dscr->en_btcoex_blacklist_chip_oui == OAL_TRUE) {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                             "hmac_btcoex_ap_tpye_identify: btcoex disable cts blacklist!");
            en_ap_type |= MAC_AP_TYPE_BTCOEX_DISABLE_CTS;
        }
    }
    /* ��ҫ·����H1 5G������360�г���¼�ǲ�realteksоƬʱ���泡���¾ܾ�����BA�Ự */
    if ((MAC_IS_HIROUTER_H1(puc_mac_addr) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_5G)) &&
        (pst_bss_dscr->en_is_tplink_oui == WLAN_AP_CHIP_OUI_REALTEKS)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                         "hmac_btcoex_ap_tpye_identify: the ap reject addba when btcoex!");
        en_ap_type |= MAC_AP_TYPE_BTCOEX_BA;
    }
    /* ���ڵ�����AP,Ϊ���������泡���µ��û�����,�ڹ��泡���¾Ͳ��ٽ����ۺ��� */
    if (MAC_IS_360_AP(puc_mac_addr) && (pst_bss_dscr->en_support_max_nss == WLAN_SINGLE_NSS)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "hmac_btcoex_ap_tpye_identify:the ap is single nss!!!");
        en_ap_type |= MAC_AP_TYPE_BTCOEX_BA;
    }
    if (MAC_IS_TPLINK_885N(puc_mac_addr) && (pst_bss_dscr->en_is_tplink_oui == WLAN_AP_CHIP_OUI_ATHEROS)) {
        en_ap_type |= MAC_AP_TYPE_BTCOEX_BA;
    }
    *pen_ap_type |= en_ap_type;
}


oal_void hmac_btcoex_process_exception_ap(mac_vap_stru *pst_mac_vap,
                                          mac_user_stru *pst_mac_user,
                                          mac_ap_type_enum_uint8 en_ap_type)
{
    hmac_user_stru *pst_hmac_user;
    oal_uint8 *puc_mac_addr;

    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }
    if (en_ap_type & MAC_AP_TYPE_BTCOEX_BA) {
        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_user->us_assoc_id);
        if (pst_hmac_user == OAL_PTR_NULL) {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, pst_mac_vap->uc_vap_id,
                           "hmac_btcoex_process_exception_ap user is null,user_index=%d", pst_mac_user->us_assoc_id);
            return;
        }
        puc_mac_addr = pst_mac_user->auc_user_mac_addr;
        if (MAC_IS_HIROUTER_H1(puc_mac_addr) || MAC_IS_360_AP(puc_mac_addr)) {
            pst_hmac_user->st_hmac_user_btcoex.en_ba_type = BTCOEX_BA_TYPE_REJECT;
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                             "hmac_btcoex_process_exception_ap:reject addba when bt on");
        }
        if (MAC_IS_TPLINK_885N(puc_mac_addr)) {
            pst_hmac_user->st_hmac_user_btcoex.en_ba_type = BTCOEX_BA_TYPE_SIZE_1;
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                             "hmac_btcoex_process_exception_ap:set ba size to 1 when sco");
        }
    }
}
#endif /* #ifdef _PRE_WLAN_FEATURE_COEXIST_BT */
#endif /* end of __HMAC_BTCOEX_C__ */


