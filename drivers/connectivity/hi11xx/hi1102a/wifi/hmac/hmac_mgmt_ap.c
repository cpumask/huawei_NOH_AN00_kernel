

/* 1 ͷ�ļ����� */
#include "oal_cfg80211.h"
#include "oam_ext_if.h"
#include "hmac_mgmt_ap.h"
#include "hmac_encap_frame.h"
#include "hmac_encap_frame_ap.h"
#include "hmac_mgmt_bss_comm.h"
#include "mac_frame.h"
#include "hmac_rx_data.h"
#include "hmac_uapsd.h"
#include "hmac_tx_amsdu.h"
#include "mac_ie.h"
#include "mac_user.h"
#include "hmac_user.h"
#include "hmac_11i.h"
#include "hmac_protection.h"
#include "hmac_chan_mgmt.h"
#include "hmac_fsm.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
#include "hmac_custom_security.h"
#endif
#include "hmac_p2p.h"
#include "hmac_blockack.h"
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#include "hmac_ext_if.h"
#endif
#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "hmac_wmmac.h"
#endif
#include "plat_pm_wlan.h"
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_AP_C

/* 2 ȫ�ֱ������� */
OAL_STATIC oal_uint32 hmac_ap_up_update_sta_sup_rates(oal_uint8 *puc_payload,
                                                      hmac_user_stru *pst_hmac_user,
                                                      mac_status_code_enum_uint16 *pen_status_code,
                                                      oal_uint32 ul_msg_len,
                                                      oal_uint16 us_offset,
                                                      oal_uint8 *puc_num_rates,
                                                      oal_uint16 *pus_msg_idx);
OAL_STATIC oal_uint32 hmac_ap_prepare_assoc_req(hmac_user_stru *pst_hmac_user,
                                                oal_uint8 *puc_payload,
                                                oal_uint32 ul_payload_len,
                                                oal_uint8 uc_mgmt_frm_type);

/* 3 ����ʵ�� */

oal_void hmac_handle_disconnect_rsp_ap(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_device_stru *pst_mac_device;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_handle_disconnect_rsp_ap::pst_mac_device null.}");
        return;
    }

    /* ��ɨ������¼���WAL */
    pst_event_mem = FRW_EVENT_ALLOC(WLAN_MAC_ADDR_LEN);
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_handle_disconnect_rsp_ap::pst_event_mem null.}");
        return;
    }

    /* ��д�¼� */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_DISCONNECT_AP,
                       WLAN_MAC_ADDR_LEN,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* ȥ������STA mac��ַ */
    if (memcpy_s(frw_get_event_payload(pst_event_mem), WLAN_MAC_ADDR_LEN,
                 (oal_uint8 *)pst_hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "{hmac_handle_disconnect_rsp_ap::memcpy fail!}");
        FRW_EVENT_FREE(pst_event_mem);
        return;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);
}


OAL_STATIC oal_void hmac_handle_connect_rsp_ap(hmac_vap_stru *pst_hmac_vap,
                                               hmac_user_stru *pst_hmac_user)
{
    mac_device_stru *pst_mac_device;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    hmac_asoc_user_req_ie_stru *pst_asoc_user_req_info = OAL_PTR_NULL;
#endif

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_handle_connect_rsp_ap::pst_mac_device null.}");
        return;
    }

    /* �׹���һ���µ�sta����¼���WAL */
    pst_event_mem = FRW_EVENT_ALLOC(WLAN_MAC_ADDR_LEN);
    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_handle_connect_rsp_ap::pst_event_mem null.}");
        return;
    }

    /* ��д�¼� */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_CONNECT_AP,
                       WLAN_MAC_ADDR_LEN,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_asoc_user_req_info = (hmac_asoc_user_req_ie_stru *)(pst_event->auc_event_data);

    /* �ϱ��ں˵Ĺ���sta���͵Ĺ�������֡ie��Ϣ */
    pst_asoc_user_req_info->puc_assoc_req_ie_buff = pst_hmac_user->puc_assoc_req_ie_buff;
    pst_asoc_user_req_info->ul_assoc_req_ie_len = pst_hmac_user->ul_assoc_req_ie_len;

    /* ������STA mac��ַ */
    memcpy_s((oal_uint8 *)pst_asoc_user_req_info->auc_user_mac_addr, WLAN_MAC_ADDR_LEN,
             pst_hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN);
#else
    /* ȥ������STA mac��ַ */
    memcpy_s((oal_uint8 *)pst_event->auc_event_data, WLAN_MAC_ADDR_LEN,
             pst_hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN);

#endif

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);
}


OAL_STATIC oal_void hmac_mgmt_update_auth_mib(hmac_vap_stru *pst_hmac_vap,
                                              oal_netbuf_stru *pst_auth_rsp)
{
    oal_uint16 us_status;
    oal_uint8 auc_addr1[6] = { 0 };
    oal_uint8 *puc_mac_header = oal_netbuf_header(pst_auth_rsp);

    us_status = mac_get_auth_status(puc_mac_header);

    mac_get_address1(puc_mac_header, auc_addr1);

    if (us_status != MAC_SUCCESSFUL_STATUSCODE) {
        mac_mib_set_AuthenticateFailStatus(&pst_hmac_vap->st_vap_base_info, us_status);
        mac_mib_set_AuthenticateFailStation(&pst_hmac_vap->st_vap_base_info, auc_addr1);

        /* DEBUG */
        OAM_INFO_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                      "{hmac_mgmt_update_auth_mib::Authentication of STA Failed.Status Code=%d.}", us_status);
    } else {
    }
}

#ifdef _PRE_WLAN_FEATURE_SAE
/*
 * �� �� ��  : hmac_ap_up_rx_auth_req_to_host
 * ��������  : AP�յ�ft/sae auth֡�ϱ�hostapd�Ĵ���
 */
OAL_STATIC oal_void hmac_ap_up_rx_auth_req_to_host(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_auth_req)
{
    oal_uint8 auc_addr2[ETHER_ADDR_LEN] = { 0 };
    oal_uint8 uc_is_seq1;
    oal_uint16 us_auth_seq;
    oal_uint16 us_user_index = 0xffff;
    oal_uint8 uc_auth_resend = 0;
    oal_uint32 ul_ret;

    hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_auth_req);

    /* ��ȡSTA�ĵ�ַ */
    mac_get_address2(oal_netbuf_header(pst_auth_req), auc_addr2);
    if (mac_addr_is_zero(auc_addr2)) {
        OAM_WARNING_LOG4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_ap_rx_auth_req::user mac:%02X:XX:XX:%02X:%02X:%02X is all 0 and invaild!}",
                         auc_addr2[0], auc_addr2[3], auc_addr2[4], auc_addr2[5]); /* auc_addr2 0��3��4��5�ֽ�Ϊ�����ӡ */
        return;
    }

    /* ����auth transaction number */
    us_auth_seq = mac_get_auth_seq_num(oal_netbuf_header(pst_auth_req));
    if (us_auth_seq > HMAC_AP_AUTH_SEQ3_WEP_COMPLETE) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_ap_rx_auth_req::auth recieve invalid seq, auth seq [%d]}", us_auth_seq);
        return;
    }

    /* ��ȡ�û�idx */
    uc_is_seq1 = (us_auth_seq == WLAN_AUTH_TRASACTION_NUM_ONE);
    ul_ret = hmac_encap_auth_rsp_get_user_idx(&(pst_hmac_vap->st_vap_base_info),
                                              auc_addr2,
                                              sizeof(auc_addr2),
                                              uc_is_seq1,
                                              &uc_auth_resend,
                                              &us_user_index);
    if (ul_ret != OAL_SUCC) {
        if (ul_ret == OAL_ERR_CODE_CONFIG_EXCEED_SPEC) {
            OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                             "{hmac_ft_ap_up_rx_auth_req::hmac_ap_get_user_idx fail, users exceed config spec!}");
        } else {
            OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                             "{hmac_ft_ap_up_rx_auth_req::hmac_ap_get_user_idx Err!}");
        }
    }

    return;
}
#endif /* _PRE_WLAN_FEATURE_SAE */


OAL_STATIC oal_void hmac_ap_rx_auth_req(
    hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_auth_req, dmac_wlan_crx_event_stru *pst_mgmt_rx_event)
{
    oal_netbuf_stru *pst_auth_rsp = OAL_PTR_NULL;
    oal_uint16 us_auth_rsp_len;
    oal_uint32 ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    oal_uint32 ul_pedding_data = 0;
    oal_uint8 auc_chtxt[WLAN_CHTXT_SIZE] = { 0 };
    oal_uint16 us_auth_alg_num;

    if (OAL_ANY_NULL_PTR3(pst_hmac_vap, pst_auth_req, pst_mgmt_rx_event)) {
        OAM_ERROR_LOG3(0, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::param null, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_auth_req, (uintptr_t)pst_mgmt_rx_event);
        return;
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    /* �����FT/SAE��֤�㷨���ϱ�hostapd */
    if (mac_get_auth_algo_num(pst_auth_req) == WLAN_MIB_AUTH_ALG_SAE) {
        hmac_ap_up_rx_auth_req_to_host(pst_hmac_vap, pst_auth_req);
        return;
    }
#endif
    us_auth_alg_num = mac_get_auth_algo_num(pst_auth_req);
    if (us_auth_alg_num == WLAN_WITP_AUTH_SHARED_KEY) {
        /* ��ȡchallenge text */
        /* Ӳ���Ĵ�����ȡ�漴byte,����WEP SHARED���� */
        oal_get_random_bytes((oal_int8 *)&auc_chtxt, WLAN_CHTXT_SIZE);
    }

    /* AP���յ�STA��������֤����֡����Ӧ����֤��Ӧ֡ */
    pst_auth_rsp =
        (oal_netbuf_stru *)OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_auth_rsp == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::auth_rsp null.}");
        return;
    }

    OAL_MEM_NETBUF_TRACE(pst_auth_rsp, OAL_TRUE);

    memset_s(oal_netbuf_cb(pst_auth_rsp), OAL_NETBUF_CB_SIZE(), 0, OAL_NETBUF_CB_SIZE());

    us_auth_rsp_len = hmac_encap_auth_rsp(&pst_hmac_vap->st_vap_base_info, pst_auth_rsp, pst_auth_req,
                                          (oal_int8 *)&auc_chtxt);
    if (us_auth_rsp_len == 0) {
        oal_netbuf_free(pst_auth_rsp);
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_ap_rx_auth_req::us_auth_rsp_len is 0.}");
        return;
    }

    oal_netbuf_put(pst_auth_rsp, us_auth_rsp_len);

    hmac_mgmt_update_auth_mib(pst_hmac_vap, pst_auth_rsp);

    /* ������֤��Ӧ֮֡ǰ�����û��Ľ���״̬��λ */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_auth_rsp);
    if (mac_res_get_hmac_user(pst_tx_ctl->us_tx_user_idx) != OAL_PTR_NULL) {
        hmac_mgmt_reset_psm(&pst_hmac_vap->st_vap_base_info, pst_tx_ctl->us_tx_user_idx);
    }
    hmac_config_scan_abort(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(oal_uint32), (oal_uint8 *)&ul_pedding_data);
    /* ���¼���dmac������֤֡ */
    ul_ret = hmac_tx_mgmt_send_event(&pst_hmac_vap->st_vap_base_info, pst_auth_rsp, us_auth_rsp_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_auth_rsp);
        OAM_WARNING_LOG1(0, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
    }
}


oal_bool_enum hmac_go_is_auth(mac_vap_stru *pst_mac_vap)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_dlist_tmp = OAL_PTR_NULL;
    mac_user_stru *pst_user_tmp = OAL_PTR_NULL;

    if (pst_mac_vap->en_p2p_mode != WLAN_P2P_GO_MODE) {
        return OAL_FALSE;
    }

    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_go_is_auth::pst_user_tmp null.}");
            continue;
        }
        
        if ((pst_user_tmp->en_user_asoc_state == MAC_USER_STATE_AUTH_COMPLETE) ||
            (pst_user_tmp->en_user_asoc_state == MAC_USER_STATE_AUTH_KEY_SEQ1)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


OAL_STATIC oal_bool_enum hmac_ap_is_olbc_present(oal_uint8 *puc_payload, oal_uint32 ul_payload_len)
{
    oal_uint8 uc_num_rates = 0;
    mac_erp_params_stru *pst_erp_params = OAL_PTR_NULL;
    oal_uint8 *puc_ie = OAL_PTR_NULL;

    if (ul_payload_len <= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_ap_is_olbc_present::payload_len[%d]}", ul_payload_len);
        return OAL_FALSE;
    }

    ul_payload_len -= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    puc_payload += (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);

    puc_ie = mac_find_ie(MAC_EID_ERP, puc_payload, (oal_int32)ul_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        pst_erp_params = (mac_erp_params_stru *)(puc_ie + MAC_IE_HDR_LEN);
        /* ���use protection����Ϊ1�� ����TRUE */
        if (pst_erp_params->bit_non_erp == 1) {
            return OAL_TRUE;
        }
    }

    puc_ie = mac_find_ie(MAC_EID_RATES, puc_payload, (oal_int32)ul_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_rates += puc_ie[1];
    }

    puc_ie = mac_find_ie(MAC_EID_XRATES, puc_payload, (oal_int32)ul_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_rates += puc_ie[1];
    }

    /* ����������ʼ���ĿС�ڻ����11bЭ��֧�ֵ�������ʼ������� ����TRUE */
    if (uc_num_rates <= MAC_NUM_DR_802_11B) {
        OAM_INFO_LOG1(0, OAM_SF_ANY, "{hmac_ap_is_olbc_present::invalid uc_num_rates[%d].}", uc_num_rates);
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC oal_void hmac_ap_process_obss_erp_ie(
    hmac_vap_stru *pst_hmac_vap,
    oal_uint8 *puc_payload,
    oal_uint32 ul_payload_len)
{
    /* ����non erpվ�� */
    if (hmac_ap_is_olbc_present(puc_payload, ul_payload_len) == OAL_TRUE) {
        pst_hmac_vap->st_vap_base_info.st_protection.bit_obss_non_erp_present = OAL_TRUE;
        pst_hmac_vap->st_vap_base_info.st_protection.uc_obss_non_ht_aging_cnt = 0;
    }
}


OAL_STATIC oal_bool_enum hmac_ap_is_obss_non_ht_present(oal_uint8 *puc_payload, oal_uint32 ul_payload_len)
{
    mac_ht_opern_stru *pst_ht_opern = OAL_PTR_NULL;
    oal_uint8 *puc_ie = OAL_PTR_NULL;

    if (ul_payload_len <= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_ap_is_olbc_present::payload_len[%d]}", ul_payload_len);
        return OAL_TRUE;
    }

    ul_payload_len -= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    puc_payload += (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);

    puc_ie = mac_find_ie(MAC_EID_HT_OPERATION, puc_payload, (oal_int32)ul_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        pst_ht_opern = (mac_ht_opern_stru *)(puc_ie + MAC_IE_HDR_LEN);
        if (pst_ht_opern->bit_obss_nonht_sta_present == 1) {
            /* ���OBSS Non-HT STAs Present����Ϊ1�� ����TRUE */
            return OAL_TRUE;
        }
    }

    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_payload, (oal_int32)ul_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        /* �����HT capability��ϢԪ�أ�����FALSE */
        return OAL_FALSE;
    }

    /* ���û��HT capability��ϢԪ�أ�����TRUE */
    return OAL_TRUE;
}


OAL_STATIC oal_void hmac_ap_process_obss_ht_operation_ie(
    hmac_vap_stru *pst_hmac_vap,
    oal_uint8 *puc_payload,
    oal_uint32 ul_payload_len)
{
    /* �������non-htվ�� */
    if (hmac_ap_is_obss_non_ht_present(puc_payload, ul_payload_len) == OAL_TRUE) {
        pst_hmac_vap->st_vap_base_info.st_protection.bit_obss_non_ht_present = OAL_TRUE;
        pst_hmac_vap->st_vap_base_info.st_protection.uc_obss_non_ht_aging_cnt = 0;
    }
}


oal_uint32 hmac_ap_rx_obss_beacon(hmac_vap_stru *pst_hmac_vap,
                                  oal_uint8 *puc_payload,
                                  oal_uint32 ul_payload_len)
{
    /* ����ERP��� */
    hmac_ap_process_obss_erp_ie(pst_hmac_vap, puc_payload, ul_payload_len);

    /* ����HT operation��� */
    hmac_ap_process_obss_ht_operation_ie(pst_hmac_vap, puc_payload, ul_payload_len);

    /* ����AP�б������mib�� */
    hmac_protection_update_mib_ap(&(pst_hmac_vap->st_vap_base_info));

    /* ����vap�ı���ģʽ */
    hmac_protection_update_mode_ap(&(pst_hmac_vap->st_vap_base_info));

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_ap_rx_deauth_req(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_mac_hdr, oal_bool_enum_uint8 en_is_protected)
{
    oal_uint8 *puc_sa = OAL_PTR_NULL;
    oal_uint8 *puc_da = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    oal_uint16 us_err_code;
    oal_uint32 ul_ret;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (puc_mac_hdr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_AUTH,
                       "{hmac_ap_rx_deauth_req::param null, %p %p.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)puc_mac_hdr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

    us_err_code = *((oal_uint16 *)(puc_mac_hdr + MAC_80211_FRAME_LEN));

    /* ���ӽ��յ�ȥ��֤֡ʱ��ά����Ϣ */
    OAM_WARNING_LOG4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                     "{aput rx deauth frame, reason code = %d, sa[XX:XX:XX:%X:%X:%X]}",
                     us_err_code, puc_sa[3], puc_sa[4], puc_sa[5]); /* us_err_code,puc_sa 3��4��5�ֽ�Ϊ���������ӡ */

    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, puc_sa);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_AUTH, "{aput rx deauth frame, pst_hmac_user null.}");
        return OAL_FAIL;
    }

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    /* ����Ƿ���Ҫ����SA query request */
    if ((pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (hmac_pmf_check_err_code(&pst_hmac_user->st_user_base_info, en_is_protected, puc_mac_hdr) == OAL_SUCC)) {
        /* �ڹ���״̬���յ�δ���ܵ�ReasonCode 6/7��Ҫ����SA Query���� */
        ul_ret = hmac_start_sa_query(&pst_hmac_vap->st_vap_base_info, pst_hmac_user,
                                     pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
        if (ul_ret != OAL_SUCC) {
            return OAL_ERR_CODE_PMF_SA_QUERY_START_FAIL;
        }
        return OAL_SUCC;
    }
#endif

    /* ������û��Ĺ���֡�������Բ�һ�£������ñ��� */
    mac_rx_get_da((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_da);
    if ((ETHER_IS_MULTICAST(puc_da) != OAL_TRUE) &&
        (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active)) {
        OAM_ERROR_LOG2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_ap_rx_deauth_req::PMF check failed %d %d.}",
                       en_is_protected, pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
        return OAL_FAIL;
    }
#ifdef _PRE_WLAN_1102A_CHR
    CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_WIFI_DRV_EVENT_SOFTAP_PASSIVE_DISCONNECT, us_err_code);
#endif
    /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
    hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

    hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    return OAL_SUCC;
}

OAL_STATIC oal_void hmac_user_sort_op_rates(hmac_user_stru *pst_hmac_user)
{
    oal_uint8 uc_loop;
    oal_uint8 uc_num_rates;
    oal_uint8 uc_min_rate;
    oal_uint8 uc_temp_rate; /* ��ʱ���ʣ��������ݽ��� */
    oal_uint8 uc_index;
    oal_uint8 uc_temp_index; /* ��ʱ�������������ݽ��� */

    uc_num_rates = pst_hmac_user->st_op_rates.uc_rs_nrates;

    for (uc_loop = 0; uc_loop < uc_num_rates; uc_loop++) {
        /* ��¼��ǰ����Ϊ��С���� */
        uc_min_rate = (pst_hmac_user->st_op_rates.auc_rs_rates[uc_loop] & 0x7F);
        uc_temp_index = uc_loop;

        /* ���β�����С���� */
        for (uc_index = uc_loop + 1; uc_index < uc_num_rates; uc_index++) {
            /* ��¼����С���ʴ��������ǰ���� */
            if (uc_min_rate > (pst_hmac_user->st_op_rates.auc_rs_rates[uc_index] & 0x7F)) {
                /* ������С���� */
                uc_min_rate = (pst_hmac_user->st_op_rates.auc_rs_rates[uc_index] & 0x7F);
                uc_temp_index = uc_index;
            }
        }

        uc_temp_rate = pst_hmac_user->st_op_rates.auc_rs_rates[uc_loop];
        pst_hmac_user->st_op_rates.auc_rs_rates[uc_loop] = pst_hmac_user->st_op_rates.auc_rs_rates[uc_temp_index];
        pst_hmac_user->st_op_rates.auc_rs_rates[uc_temp_index] = uc_temp_rate;
    }
}


OAL_STATIC oal_bool_enum_uint8 hmac_ap_up_update_sta_cap_info(
    hmac_vap_stru *pst_hmac_vap, oal_uint16 us_cap_info, hmac_user_stru *pst_hmac_user,
    mac_status_code_enum_uint16 *pen_status_code)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    mac_cap_info_stru *pst_cap_info = (mac_cap_info_stru *)(&us_cap_info);

    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_user == OAL_PTR_NULL)) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY,
                       "{hmac_ap_up_update_sta_cap_info::param null, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pen_status_code);
        *pen_status_code = MAC_UNSPEC_FAIL;
        return OAL_FALSE;
    }
    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* check bss capability info MAC,����MAC������ƥ���STA */
    ul_ret = hmac_check_bss_cap_info(us_cap_info, pst_mac_vap);
    if (ul_ret != OAL_TRUE) {
        OAM_ERROR_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_ap_up_update_sta_cap_info::hmac_check_bss_cap_info failed[%d].}", ul_ret);
        *pen_status_code = MAC_UNSUP_CAP;
        return OAL_FALSE;
    }

    /* ������ϸ�������Ϣ���������Ҫ���������������������Ϣ */
    mac_vap_check_bss_cap_info_phy_ap(us_cap_info, pst_mac_vap);

    if ((pst_cap_info->bit_privacy == 0) &&
        (pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type != WLAN_80211_CIPHER_SUITE_NO_ENCRYP)) {
        *pen_status_code = MAC_UNSPEC_FAIL;
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


OAL_STATIC oal_uint32 hmac_user_check_update_exp_rate(
    hmac_user_stru *pst_hmac_user, oal_uint8 *pst_params, oal_uint8 *puc_erp_rates_num)
{
    oal_uint8 uc_rate_idx = 0;
    oal_uint8 uc_loop;
    mac_rate_stru *pst_mac_rate = OAL_PTR_NULL;

    if ((pst_hmac_user == OAL_PTR_NULL) || (pst_params == OAL_PTR_NULL) || (puc_erp_rates_num == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    *puc_erp_rates_num = pst_params[uc_rate_idx + 1];
    uc_rate_idx += MAC_IE_HDR_LEN;
    pst_mac_rate = &(pst_hmac_user->st_op_rates);

    if ((*puc_erp_rates_num) > (oal_uint8)WLAN_MAX_SUPP_RATES) {
        return OAL_FAIL;
    }

    for (uc_loop = 0; uc_loop < *puc_erp_rates_num; uc_loop++) {
        pst_mac_rate->auc_rs_rates[pst_mac_rate->uc_rs_nrates + uc_loop] = pst_params[uc_rate_idx + uc_loop] & 0x7F;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_ap_up_update_sta_sup_rates(oal_uint8 *puc_payload,
                                                      hmac_user_stru *pst_hmac_user,
                                                      mac_status_code_enum_uint16 *pen_status_code,
                                                      oal_uint32 ul_msg_len,
                                                      oal_uint16 us_offset,
                                                      oal_uint8 *puc_num_rates,
                                                      oal_uint16 *pus_msg_idx)
{
    oal_uint8 uc_num_of_erp_rates = 0;
    oal_uint32 ul_loop;
    mac_user_stru *pst_mac_user;
    mac_vap_stru *pst_mac_vap;
    oal_uint8 *puc_sup_rates_ie = OAL_PTR_NULL;
    oal_uint8 *puc_ext_sup_rates_ie = OAL_PTR_NULL;
    oal_uint8 uc_temp_rate;
    oal_uint32 ul_ret;
    oal_uint16 us_msg_idx = 0;

    /* ��ʼ�� */
    *puc_num_rates = 0;

    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    pst_mac_vap = mac_res_get_mac_vap(pst_mac_user->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        *pen_status_code = MAC_UNSPEC_FAIL;
        OAM_ERROR_LOG0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                       "{hmac_ap_up_update_sta_sup_rates::pst_mac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    puc_sup_rates_ie = mac_find_ie(MAC_EID_RATES, puc_payload + us_offset, (oal_int32)(ul_msg_len - us_offset));
    if (puc_sup_rates_ie == OAL_PTR_NULL) {
        *pen_status_code = MAC_UNSUP_RATE;
        OAM_ERROR_LOG0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                       "{hmac_ap_up_update_sta_user::puc_ie null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (puc_sup_rates_ie[0] == MAC_EID_RATES) {
        *puc_num_rates = puc_sup_rates_ie[1];

        /* ������ʸ���Ϊ0 ��ֱ�ӷ���ʧ�� */
        if (*puc_num_rates == 0) {
            *pen_status_code = MAC_UNSUP_RATE;
            *pus_msg_idx = us_msg_idx;
            OAM_ERROR_LOG0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                           "{hmac_ap_up_update_sta_sup_rates::the sta's rates are not supported.}");
            return OAL_FAIL;
        }

        us_msg_idx += MAC_IE_HDR_LEN;

        if (*puc_num_rates > MAC_MAX_SUPRATES) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac_ap_up_update_sta_sup_rates: rates num error: %d", *puc_num_rates);
            *puc_num_rates = MAC_MAX_SUPRATES;
        }

        for (ul_loop = 0; ul_loop < *puc_num_rates; ul_loop++) {
            /* �����Ӧ�����ʵ�USER�� */
            pst_hmac_user->st_op_rates.auc_rs_rates[ul_loop] = puc_sup_rates_ie[us_msg_idx + ul_loop] & 0x7F;
        }

        us_msg_idx += *puc_num_rates;
        pst_hmac_user->st_op_rates.uc_rs_nrates = *puc_num_rates;
    }

    /* ���������չ���� */
    puc_ext_sup_rates_ie = mac_find_ie(MAC_EID_XRATES, puc_payload + us_offset, (oal_int32)(ul_msg_len - us_offset));
    if (puc_ext_sup_rates_ie == OAL_PTR_NULL) {
        if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
            OAM_WARNING_LOG0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                             "{hmac_ap_up_update_sta_user::puc_ext_sup_rates_ie null.}");
        }
    } else {
        /* ֻ��11g���ģʽ���߸���Э��汾��֧��ERP */
        if ((puc_ext_sup_rates_ie[0] == MAC_EID_XRATES) &&
            (pst_mac_vap->en_protocol > WLAN_LEGACY_11G_MODE)) {
            /* ����EXTRACE EXTENDED RATES,���õ���չ���ʵĸ��� */
            ul_ret = hmac_user_check_update_exp_rate(pst_hmac_user, puc_ext_sup_rates_ie, &uc_num_of_erp_rates);
            if (ul_ret != OAL_SUCC) {
                uc_num_of_erp_rates = 0;
            }
        }
    }
    /* ���ʸ������� */
    *puc_num_rates += uc_num_of_erp_rates;

    /* ����STA֧�ֵ����ʸ��� */
    pst_hmac_user->st_op_rates.uc_rs_nrates = *puc_num_rates;

    /* ��һ��˳�������������� */
    hmac_user_sort_op_rates(pst_hmac_user);

    /*
     * ����11gģʽ�Ŀɲ������ʣ�ʹ11b���ʶ��ۼ���11a֮ǰ
     * 802.11a ����:6��9��12��18��24��36��48��54Mbps
     * 802.11b ����:1��2��5.5��11Mbps
     * ���ڰ���С���������802.11b�е�����11Mbps��802.11a�У��±�Ϊ5
     * ���Դӵ���λ���м��鲢����
 */
    if (pst_hmac_user->st_op_rates.uc_rs_nrates == MAC_DATARATES_PHY_80211G_NUM) { /* 11g_compatibility mode */
        if ((pst_hmac_user->st_op_rates.auc_rs_rates[5] & 0x7F) == 0x16) {         /* 11Mbps��802.11a�У��±�Ϊ5 */
            uc_temp_rate = pst_hmac_user->st_op_rates.auc_rs_rates[5]; /* ��auc_rs_rates 5byte����ֵ������uc_temp_rate */
            /* auc_rs_rates 4byte������ֵ����auc_rs_rates 5byte */
            pst_hmac_user->st_op_rates.auc_rs_rates[5] = pst_hmac_user->st_op_rates.auc_rs_rates[4];
            /* auc_rs_rates 3byte������ֵ����auc_rs_rates 4byte */
            pst_hmac_user->st_op_rates.auc_rs_rates[4] = pst_hmac_user->st_op_rates.auc_rs_rates[3];
            pst_hmac_user->st_op_rates.auc_rs_rates[3] = uc_temp_rate; /* uc_temp_rate����ֵ��auc_rs_rates 3byte */
        }
    }

    /*******************************************************************
      ���STA��֧�����л������ʷ��ز�֧�����ʵĴ�����
    *******************************************************************/
#ifdef _PRE_WLAN_NARROW_BAND
    if (hitalk_status) {
        hmac_check_sta_base_rate((oal_uint8 *)pst_hmac_user, pen_status_code);
    }
#endif

    *pus_msg_idx = us_msg_idx;
    return OAL_SUCC;
}

#if defined(_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2)

OAL_STATIC oal_uint32 hmac_check_assoc_req_security_cap_common(mac_vap_stru *pst_mac_vap,
                                                               oal_uint8 *puc_ie,
                                                               oal_uint32 ul_msg_len,
                                                               oal_uint8 uc_80211i_mode,
                                                               oal_uint8 uc_offset,
                                                               mac_status_code_enum_uint16 *pen_status_code)
{
    oal_uint8 auc_pcip_policy[WLAN_PAIRWISE_CIPHER_SUITES] = { 0 };
    oal_uint8 auc_auth_policy[WLAN_AUTHENTICATION_SUITES] = { 0 };
    wlan_mib_ieee802dot11_stru *pst_mib_info = OAL_PTR_NULL;
    oal_uint8 uc_grp_policy;
    OAL_CONST oal_uint8 *puc_oui;
    oal_uint8 uc_index = uc_offset;
    oal_uint8 uc_len;
    oal_uint16 us_pmkid_count;
    oal_uint16 us_pmkid_idx;

    pst_mib_info = pst_mac_vap->pst_mib_info;

    puc_oui = hmac_get_security_oui(uc_80211i_mode);
    if (puc_oui == OAL_PTR_NULL) {
        *pen_status_code = MAC_UNSUP_RSN_INFO_VER;
        return OAL_FAIL;
    }

    /* ���汾��Ϣ */
    if (OAL_MAKE_WORD16(puc_ie[uc_index], puc_ie[uc_index + 1]) != MAC_RSN_IE_VERSION) {
        *pen_status_code = MAC_UNSUP_RSN_INFO_VER;
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_check_assoc_req_security_cap_common::unsup rsn version=%d.}",
                       OAL_MAKE_WORD16(puc_ie[uc_index], puc_ie[uc_index + 1]));
        return OAL_FAIL;
    }

    /* ���԰汾��Ϣ(2�ֽ�)  */
    uc_index += 2;

    if (oal_memcmp(puc_oui, puc_ie + uc_index, MAC_OUI_LEN) != 0) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_assoc_req_security_cap_common::invalid group OUI.}");
    }
    uc_index += MAC_OUI_LEN;
    uc_grp_policy = puc_ie[uc_index++]; /* ��ȡ�鲥��Կ */

    /* ��ȡ���������׼� */
    uc_len = hmac_get_pcip_policy_auth(puc_ie + uc_index, auc_pcip_policy);
    if (uc_len == 0) {
        *pen_status_code = MAC_INVALID_PW_CIPHER;
        return OAL_FAIL;
    }

    uc_index += uc_len;

    /* ��ȡ��֤�׼���Ϣ */
    uc_len = hmac_get_auth_policy_auth(puc_ie + uc_index, auc_auth_policy);
    if (uc_len == 0) {
        *pen_status_code = MAC_INVALID_AKMP_CIPHER;
        return OAL_FAIL;
    }

    uc_index += uc_len;

    /* �ɶ���Կ�׼��Ƿ�Ϊ�鲥��Կ�׼� */
    if (auc_pcip_policy[0] == WLAN_80211_CIPHER_SUITE_GROUP_CIPHER) {
        auc_pcip_policy[0] = uc_grp_policy;
    }

    /* ����鲥�׼� */
    if (mac_check_group_policy(pst_mac_vap, uc_grp_policy, uc_80211i_mode) != OAL_SUCC) {
        *pen_status_code = MAC_INVALID_GRP_CIPHER;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_assoc_req_security_cap_common::invalid group[%d].}", uc_grp_policy);
        return OAL_FAIL;
    }

    /* ��ⵥ����Կ�׼� */
    if (hmac_check_pcip_policy(pst_mac_vap, auc_pcip_policy, uc_80211i_mode) != OAL_SUCC) {
        *pen_status_code = MAC_INVALID_PW_CIPHER;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_assoc_req_security_cap_common::invalid pcip[%d].}", auc_pcip_policy[0]);
        return OAL_FAIL;
    }

    /* �����֤�׼� */
    if (mac_check_auth_policy(pst_mac_vap, auc_auth_policy, uc_80211i_mode) != OAL_SUCC) {
        *pen_status_code = MAC_INVALID_AKMP_CIPHER;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_assoc_req_security_cap_common::invalid auth[%d].}", auc_auth_policy[0]);
        return OAL_FAIL;
    }

    /* ��ֹ�����׼�ΪTKIP, �鲥�׼�ΪCCMP */
    if ((uc_grp_policy == WLAN_80211_CIPHER_SUITE_CCMP) &&
        (auc_pcip_policy[0] == WLAN_80211_CIPHER_SUITE_TKIP)) {
        *pen_status_code = MAC_CIPHER_REJ;
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_assoc_req_security_cap_common::group and pairwise cipher conflict.}");
        return OAL_FAIL;
    }

    /* ��RSN�����ֶ� ��ǰ��2Ϊie��ID��len����,RSN Capabilities����2�ֽ� */
    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(2, uc_index, puc_ie[1], 2) == OAL_FALSE) {
        return OAL_SUCC;
    }

    /* �Թ�RSN�����ֶ�(2�ֽ�) */
    uc_index += 2;

    /* ��pmkid count/pmkid list�ֶΣ�ǰ��2Ϊie��ID��len����,pmkid count����2�ֽ� */
    if (MAC_IE_REAMIN_LEN_IS_ENOUGH(2, uc_index, puc_ie[1], 2) == OAL_FALSE) {
        return OAL_SUCC;
    }

    us_pmkid_count = OAL_MAKE_WORD16(puc_ie[uc_index], puc_ie[uc_index + 1]);
    for (us_pmkid_idx = 0; us_pmkid_idx < us_pmkid_count; us_pmkid_idx++) {
        /* ǰ��2Ϊie��ID��len����,PMKID Listһ�鳤����16�ֽ� */
        if (MAC_IE_REAMIN_LEN_IS_ENOUGH(2, uc_index, puc_ie[1], 16) == OAL_FALSE) {
            *pen_status_code = MAC_INVALID_PMKID;
            return OAL_FAIL;
        }

        uc_index += 16; /* PMKID Listһ�鳤����16�ֽ� */
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_check_assoc_req_security_cap_authenticator(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_mac_hdr, oal_uint8 *puc_payload,
    oal_uint32 ul_msg_len, hmac_user_stru *pst_hmac_user, mac_status_code_enum_uint16 *pen_status_code)
{
    oal_uint8 uc_pcip_policy = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    oal_uint8 uc_grp_policy = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    oal_uint8 uc_80211i_mode = 0;
    oal_uint8 *puc_ie = OAL_PTR_NULL;
    oal_uint8 *puc_rsn_ie = OAL_PTR_NULL;
    oal_uint8 *puc_wpa_ie = OAL_PTR_NULL;
    oal_uint8 uc_index = 0;
    oal_bool_enum_uint8 en_rsn_found = OAL_FALSE; /* AP ���յ���ASSOC REQ֡IE �Ƿ�ƥ�� */
    mac_vap_stru *pst_mac_vap = NULL;
    mac_user_stru *pst_mac_user = NULL;
    wlan_mib_ieee802dot11_stru *pst_mib_info = NULL;
    oal_uint32 ul_ret;
    oal_uint8 uc_offset;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mib_info = pst_mac_vap->pst_mib_info;

    if (mac_mib_get_rsnaactivated(pst_mac_vap) != OAL_TRUE) {
        return OAL_SUCC;
    }

    uc_offset = MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN;

    if (mac_get_frame_sub_type(puc_mac_hdr) == WLAN_FC0_SUBTYPE_REASSOC_REQ) {
        uc_offset += OAL_MAC_ADDR_LEN;
    }

    /* ��ȡRSNA��WPA IE��Ϣ */
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_payload + uc_offset, (oal_int32)(ul_msg_len - uc_offset));
    puc_wpa_ie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_OUITYPE_WPA, puc_payload + uc_offset,
                                    (oal_int32)(ul_msg_len - uc_offset));
    if ((puc_rsn_ie == OAL_PTR_NULL) && (puc_wpa_ie == OAL_PTR_NULL)) {
        if (pst_hmac_vap->en_wps_active == OAL_TRUE) {
            mac_user_init_key(&pst_hmac_user->st_user_base_info);
            return OAL_SUCC;
        } else {
            *pen_status_code = MAC_INVALID_INFO_ELMNT;

            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_check_assoc_req_security_cap_authenticator::not WPA/WPA2.}");

            return OAL_FAIL;
        }
    } else { /* RSNA �� WPA���ܷ�ʽ���� */
        if (pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_TRUE) {
            if (puc_rsn_ie != OAL_PTR_NULL) {
                en_rsn_found = OAL_TRUE;
                uc_80211i_mode = DMAC_RSNA_802_11I;

                /* �Թ� IE + LEN(2�ֽ�) */
                uc_index += 2;

                puc_ie = puc_rsn_ie;
            }
        }

        if ((en_rsn_found == OAL_FALSE) &&
            (pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE)) {
            if (puc_wpa_ie != OAL_PTR_NULL) {
                en_rsn_found = OAL_TRUE;
                uc_80211i_mode = DMAC_WPA_802_11I;

                uc_index += 6; /* �Թ� IE(1�ֽ�) + LEN(1�ֽ�) + WPA OUI(4�ֽ�)����6�ֽ� */

                puc_ie = puc_wpa_ie;
            } else {
                *pen_status_code = MAC_CIPHER_REJ;
                return OAL_FAIL;
            }
        }

        if ((en_rsn_found == OAL_TRUE) && (puc_ie != OAL_PTR_NULL)) {
            ul_ret = hmac_check_assoc_req_security_cap_common(pst_mac_vap, puc_ie, ul_msg_len,
                                                              uc_80211i_mode, uc_index, pen_status_code);
            if (ul_ret != OAL_SUCC) {
                return OAL_FAIL;
            }
        }
    }

    if (*pen_status_code == MAC_SUCCESSFUL_STATUSCODE) {
        /* �����������Ӳ�����ܷ�ʽӦ�ñ��浽 mac_user -> en_cipher_type �� */
        pst_mac_user = &(pst_hmac_user->st_user_base_info);

        /* �����û��ļ��ܷ�ʽ */
        pst_mac_user->st_key_info.en_cipher_type = (uc_pcip_policy);
    }

    OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_check_assoc_req_security_cap_authenticator::\
                  mode=%d, group=%d, pairwise=%d, auth=%d}\r\n",
                  uc_80211i_mode, uc_grp_policy, uc_pcip_policy);

    return OAL_SUCC;
}
#endif /* (_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2) */


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_is_erp_sta(hmac_user_stru *pst_hmac_user)
{
    oal_uint32 ul_loop = 0;
    oal_bool_enum_uint8 en_is_erp_sta;

    /* ȷ���Ƿ���erp վ�� */
    if (pst_hmac_user->st_op_rates.uc_rs_nrates <= MAC_NUM_DR_802_11B) {
        en_is_erp_sta = OAL_FALSE;
        for (ul_loop = 0; ul_loop < pst_hmac_user->st_op_rates.uc_rs_nrates; ul_loop++) {
            /* ���֧�����ʲ���11b��1M, 2M, 5.5M, 11M��Χ�ڣ���˵��վ��Ϊ֧��ERP��վ�� */
            if (((pst_hmac_user->st_op_rates.auc_rs_rates[ul_loop] & 0x7F) != 0x2) &&
                ((pst_hmac_user->st_op_rates.auc_rs_rates[ul_loop] & 0x7F) != 0x4) &&
                ((pst_hmac_user->st_op_rates.auc_rs_rates[ul_loop] & 0x7F) != 0xb) &&
                ((pst_hmac_user->st_op_rates.auc_rs_rates[ul_loop] & 0x7F) != 0x16)) {
                en_is_erp_sta = OAL_TRUE;
                break;
            }
        }
    } else {
        en_is_erp_sta = OAL_TRUE;
        ;
    }

    return en_is_erp_sta;
}


OAL_STATIC oal_void hmac_ap_up_update_legacy_capability(
    hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, oal_uint16 us_cap_info)
{
    mac_protection_stru *pst_protection = &(pst_hmac_vap->st_vap_base_info.st_protection);
    mac_user_stru *pst_mac_user = &(pst_hmac_user->st_user_base_info);
    oal_bool_enum_uint8 en_is_erp_sta;

    /* ���STA��֧��short slot */
    if ((us_cap_info & MAC_CAP_SHORT_SLOT) != MAC_CAP_SHORT_SLOT) {
        /* ���STA֮ǰû�й����� ����֮ǰ��֧��short slotվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC) ||
            (pst_hmac_user->st_hmac_cap_info.bit_short_slot_time == OAL_TRUE)) {
            pst_protection->uc_sta_no_short_slot_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_slot_time = OAL_FALSE;
    } else { /* ���STA֧��short slot */
        /* ���STA�Բ�֧��short slotվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
            (pst_hmac_user->st_hmac_cap_info.bit_short_slot_time == OAL_FALSE) &&
            (pst_protection->uc_sta_no_short_slot_num != 0)) {
            pst_protection->uc_sta_no_short_slot_num--;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_slot_time = OAL_TRUE;
    }

    pst_hmac_user->st_user_stats_flag.bit_no_short_slot_stats_flag = OAL_TRUE;

    /* ���STA��֧��short preamble */
    if ((us_cap_info & MAC_CAP_SHORT_PREAMBLE) != MAC_CAP_SHORT_PREAMBLE) {
        /* ���STA֮ǰû�й����� ����֮ǰ��֧��short preambleվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC) ||
            (pst_hmac_user->st_hmac_cap_info.bit_short_preamble == OAL_TRUE)) {
            pst_protection->uc_sta_no_short_preamble_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_preamble = OAL_FALSE;
    } else { /* ���STA֧��short preamble */
        /* ���STA֮ǰ�Բ�֧��short preambleվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
            (pst_hmac_user->st_hmac_cap_info.bit_short_preamble == OAL_FALSE) &&
            (pst_protection->uc_sta_no_short_preamble_num != 0)) {
            pst_protection->uc_sta_no_short_preamble_num--;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_preamble = OAL_TRUE;
    }

    pst_hmac_user->st_user_stats_flag.bit_no_short_preamble_stats_flag = OAL_TRUE;

    /* ȷ��user�Ƿ���erpվ�� */
    en_is_erp_sta = hmac_is_erp_sta(pst_hmac_user);
    /* ���STA��֧��ERP */
    if (en_is_erp_sta == OAL_FALSE) {
        /* ���STA֮ǰû�й����� ����֮ǰ��֧��ERPվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC) ||
            (pst_hmac_user->st_hmac_cap_info.bit_erp == OAL_TRUE)) {
            pst_protection->uc_sta_non_erp_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_erp = OAL_FALSE;
    } else { /* ���STA֧��ERP */
        /* ���STA֮ǰ�Բ�֧��ERP���վ���������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
            (pst_hmac_user->st_hmac_cap_info.bit_erp == OAL_FALSE) &&
            (pst_protection->uc_sta_non_erp_num != 0)) {
            pst_protection->uc_sta_non_erp_num--;
        }

        pst_hmac_user->st_hmac_cap_info.bit_erp = OAL_TRUE;
    }

    pst_hmac_user->st_user_stats_flag.bit_no_erp_stats_flag = OAL_TRUE;

    if ((us_cap_info & MAC_CAP_SPECTRUM_MGMT) != MAC_CAP_SPECTRUM_MGMT) {
        mac_user_set_spectrum_mgmt(&pst_hmac_user->st_user_base_info, OAL_FALSE);
    } else {
        mac_user_set_spectrum_mgmt(&pst_hmac_user->st_user_base_info, OAL_TRUE);
    }
}


OAL_STATIC oal_void hmac_ap_up_update_asoc_entry_prot(
    oal_uint8 *puc_mac_hdr, oal_uint8 uc_sub_type, oal_uint32 ul_msg_len, oal_uint16 us_info_elem_offset,
    oal_uint16 us_cap_info, hmac_user_stru *pst_hmac_user, hmac_vap_stru *pst_hmac_vap)
{
    /* WMM */
    hmac_uapsd_update_user_para(puc_mac_hdr, uc_sub_type, ul_msg_len, pst_hmac_user);
}

oal_void hmac_ap_update_2g11ac(mac_vap_stru *pst_mac_vap,
                               mac_user_stru *pst_mac_user,
                               oal_uint8 *puc_avail_mode)
{
    *puc_avail_mode = g_auc_avail_protocol_mode[pst_mac_vap->en_protocol][pst_mac_user->en_protocol_mode];

#ifdef _PRE_WLAN_FEATURE_11AC2G
    if ((pst_mac_vap->en_protocol == WLAN_HT_MODE) && (pst_mac_user->en_protocol_mode == WLAN_VHT_MODE) &&
        (pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_TRUE) &&
        (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G)) {
        *puc_avail_mode = WLAN_VHT_MODE;
    }
#endif
}


OAL_STATIC oal_uint32 hmac_ap_up_update_sta_user(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_mac_hdr, oal_uint8 *puc_payload,
    oal_uint32 ul_msg_len, hmac_user_stru *pst_hmac_user, mac_status_code_enum_uint16 *pen_status_code)
{
    oal_uint32 ul_rslt;
    oal_uint16 us_msg_idx = 0;
    oal_uint16 us_cap_info;
    oal_uint16 us_ssid_len;
    oal_uint8 uc_num_rates;
    oal_uint16 us_rate_len = 0;
    mac_status_code_enum_uint16 us_ret_val;
    mac_cfg_ssid_param_stru st_cfg_ssid;
    const oal_uint8 *puc_rsn_ie = OAL_PTR_NULL;
    oal_uint16 us_offset;
    wlan_bw_cap_enum_uint8 en_bandwidth_cap = WLAN_BW_CAP_BUTT;
    wlan_bw_cap_enum_uint8 en_bwcap_ap; /* ap����������� */
    oal_uint32 ul_ret;
    mac_vap_stru *pst_mac_vap;
    mac_user_stru *pst_mac_user;
    wlan_bw_cap_enum_uint8 en_bwcap_vap;
#ifdef _PRE_WLAN_FEATURE_TXBF
    oal_uint8 *puc_vendor_ie;
#endif
    oal_uint8 *puc_ie_tmp = OAL_PTR_NULL;
    oal_uint8 uc_avail_mode;

    *pen_status_code = MAC_SUCCESSFUL_STATUSCODE;
    us_offset = MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    /***************************************************************************
        ���AP�Ƿ�֧�ֵ�ǰ���ڹ�����STA����������
        |ESS|IBSS|CFPollable|CFPReq|Privacy|Preamble|PBCC|Agility|Reserved|
    ***************************************************************************/
    /* puc_payload[us_msg_idx]����8λ����puc_payload[(us_msg_idx + 1)]����8λ�����16bit�� */
    us_cap_info = puc_payload[us_msg_idx] | (puc_payload[(us_msg_idx + 1)] << 8);

    ul_rslt = hmac_ap_up_update_sta_cap_info(pst_hmac_vap, us_cap_info, pst_hmac_user, pen_status_code);
    if (ul_rslt != OAL_TRUE) {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{ap up update sta cap info failed[%d], status_code=%d.}", ul_rslt, *pen_status_code);
        return ul_rslt;
    }

    us_msg_idx += MAC_CAP_INFO_LEN;
    us_msg_idx += MAC_LIS_INTERVAL_IE_LEN;

    if (mac_get_frame_sub_type(puc_mac_hdr) == WLAN_FC0_SUBTYPE_REASSOC_REQ) {
        /* �ع����ȹ�������֡ͷ����AP��MAC��ַ */
        us_msg_idx += WLAN_MAC_ADDR_LEN;
        us_offset += WLAN_MAC_ADDR_LEN;
    }

    /* �ж�SSID,���Ȼ����ݲ�һ��ʱ,��Ϊ��SSID��һ�£����Ǽ������Ҳ���ieʱ������ */
    puc_ie_tmp = mac_find_ie(MAC_EID_SSID, puc_payload + us_msg_idx, (oal_int32)(ul_msg_len - us_msg_idx));
    if (puc_ie_tmp != OAL_PTR_NULL) {
        us_ssid_len = 0;

        st_cfg_ssid.uc_ssid_len = 0;

        hmac_config_get_ssid (pst_mac_vap, &us_ssid_len, (oal_uint8 *)(&st_cfg_ssid));

        if (st_cfg_ssid.uc_ssid_len != puc_ie_tmp[1]) {
            *pen_status_code = MAC_UNSPEC_FAIL;
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                             "AP refuse STA assoc, ssid len mismatch, status_code=%d.", *pen_status_code);
            return OAL_FAIL;
        }
        /* puc_ie_tmp��2�ֽڿ�ʼ��ssid�ڴ� */
        if (oal_memcmp(&puc_ie_tmp[2], st_cfg_ssid.ac_ssid, st_cfg_ssid.uc_ssid_len) != 0) {
            *pen_status_code = MAC_UNSPEC_FAIL;
            OAM_WARNING_LOG1(0, OAM_SF_ASSOC, "AP refuse STA assoc, ssid mismatch, status_code=%d.}", *pen_status_code);
            return OAL_FAIL;
        }
    }

    /* ��ǰ�û��ѹ��� */
    ul_rslt = hmac_ap_up_update_sta_sup_rates(puc_payload, pst_hmac_user, pen_status_code, ul_msg_len,
                                              us_msg_idx, &uc_num_rates, &us_rate_len);
    if (ul_rslt != OAL_SUCC) {
        OAM_WARNING_LOG2(0, OAM_SF_ASSOC, "{AP refuse STA assoc, update rates failed, status_code[%d] ul_rslt[%d].}",
                         *pen_status_code, ul_rslt);
        return ul_rslt;
    }

#if defined(_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2)
    /* �����յ���ASOC REQ��Ϣ�е�SECURITY����.�����,�򷵻ض�Ӧ�Ĵ����� */
    ul_rslt = hmac_check_assoc_req_security_cap_authenticator(pst_hmac_vap, puc_mac_hdr, puc_payload,
                                                              ul_msg_len, pst_hmac_user, pen_status_code);
    if (ul_rslt != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "Reject STA because of security_cap_auth[%d]", ul_rslt);
        return OAL_FAIL;
    }
#endif /* defined (_PRE_WLAN_FEATURE_WPA) ||��defined(_PRE_WLAN_FEATURE_WPA2) */

    /* ���¶�ӦSTA��legacyЭ������ */
    hmac_ap_up_update_legacy_capability(pst_hmac_vap, pst_hmac_user, us_cap_info);

    /* ���HT capability�Ƿ�ƥ�䣬�����д��� */
    us_ret_val = hmac_vap_check_ht_capabilities_ap(pst_hmac_vap, puc_payload, us_msg_idx, ul_msg_len, pst_hmac_user);
    if (us_ret_val != MAC_SUCCESSFUL_STATUSCODE) {
        *pen_status_code = us_ret_val;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "Reject STA because of ht_capability[%d]", us_ret_val);
        return us_ret_val;
    }

    /* ����AP�б������mib�� */
    ul_ret = hmac_protection_update_mib_ap(pst_mac_vap);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "protection update mib failed, ret=%d", ul_ret);
    }

    /* ���¶�ӦSTA��Э������ update_asoc_entry_prot(ae, msa, rx_len, cap_info, is_p2p); */
    hmac_ap_up_update_asoc_entry_prot(puc_payload, mac_get_frame_sub_type(puc_mac_hdr), ul_msg_len,
                                      us_msg_idx, us_cap_info, pst_hmac_user, pst_hmac_vap);

    /* ����QoS���� */
    hmac_mgmt_update_assoc_user_qos_table(puc_payload, (oal_uint16)ul_msg_len, us_msg_idx, pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_TXBF
    /* ����11n txbf���� */
    puc_vendor_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_EID_11NTXBF, puc_payload + us_msg_idx,
                                       (oal_int32)(ul_msg_len - us_msg_idx));
    hmac_mgmt_update_11ntxbf_cap(puc_vendor_ie, pst_hmac_user);
#endif

    /* ����11ac VHT capabilities ie */
    us_ret_val = hmac_vap_check_vht_capabilities_ap(pst_hmac_vap, puc_payload, us_msg_idx, ul_msg_len, pst_hmac_user);
    if (us_ret_val != MAC_SUCCESSFUL_STATUSCODE) {
        *pen_status_code = us_ret_val;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::Reject STA because of vht_capability[%d].}", us_ret_val);
        return us_ret_val;
    }

    /* ����RSN��ϢԪ��,���û��RSN��ϢԪ��,���ղ�֧�ִ��� */
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_payload + us_offset, (oal_int32)(ul_msg_len - us_offset));

    /* ����RSN��ϢԪ��, �ж�RSN�����Ƿ�ƥ�� */
    ul_ret = hmac_check_rsn_capability(pst_mac_vap, pst_mac_user, puc_rsn_ie, pen_status_code);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::Reject STA because of rsn_capability[%d].}", ul_ret);
        return ul_ret;
    }

    /* ��ȡ�û���Э��ģʽ */
    hmac_set_user_protocol_mode(pst_mac_vap, pst_hmac_user);

    hmac_ap_update_2g11ac(pst_mac_vap, pst_mac_user, &uc_avail_mode);

    /* ��ȡ�û���VAPЭ��ģʽ���� */
    mac_user_set_avail_protocol_mode(pst_mac_user, uc_avail_mode);
    mac_user_set_cur_protocol_mode(pst_mac_user, pst_mac_user->en_avail_protocol_mode);

    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_ap_up_update_sta_user::mac_vap->en_protocol:%d,mac_user->en_protocol_mode:%d, \
                     en_avail_protocol_mode:%d.}",
                     pst_mac_vap->en_protocol, pst_mac_user->en_protocol_mode, pst_mac_user->en_avail_protocol_mode);

    /* ��ȡ�û���VAP ��֧�ֵ�11a/b/g ���ʽ��� */
    hmac_vap_set_user_avail_rates(pst_mac_vap, pst_hmac_user);

    /* ��ȡ�û��Ĵ������� */
    mac_user_get_sta_cap_bandwidth(pst_mac_user, &en_bandwidth_cap);

    /* ��ȡvap�����������û����������Ľ��� */
    mac_vap_get_bandwidth_cap(&pst_hmac_vap->st_vap_base_info, &en_bwcap_ap);
    en_bwcap_vap = OAL_MIN(en_bwcap_ap, en_bandwidth_cap);
    mac_user_set_bandwidth_info(pst_mac_user, en_bwcap_vap, en_bwcap_vap);

    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_ap_up_update_sta_user::mac_vap->bandwidth:%d,mac_user->bandwidth:%d,cur_bandwidth:%d.}",
                     en_bwcap_ap, en_bandwidth_cap,
                     pst_mac_user->en_cur_bandwidth);

    ul_ret = hmac_config_user_cap_syn(pst_mac_vap, pst_mac_user);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(pst_mac_user->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_ap_up_update_sta_user::hmac_config_usr_cap_syn failed[%d].}", ul_ret);
    }

    /* �����û�֧�ִ���������Э�̳���ǰ����dmac offload�ܹ��£�ͬ��������Ϣ��device */
    ul_ret = hmac_config_user_info_syn(pst_mac_vap, pst_mac_user);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(pst_mac_user->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_ap_up_update_sta_user::usr_info_syn failed[%d].}", ul_ret);
    }

    /* ��ȡ�û���VAP�ռ������� */
    ul_ret = hmac_user_set_avail_num_space_stream(pst_mac_user, pst_mac_vap->en_vap_rx_nss);
    if (ul_ret != OAL_SUCC) {
        *pen_status_code = MAC_UNSPEC_FAIL;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::mac_user_set_avail_num_space_stream failed[%d].}", ul_ret);
    }

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    /* ����Operating Mode Notification ��ϢԪ�� */
    ul_ret = hmac_check_opmode_notify(pst_hmac_vap, puc_mac_hdr, puc_payload, us_msg_idx, ul_msg_len, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::hmac_check_opmode_notify failed[%d].}", ul_ret);
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_ap_prepare_assoc_req(
    hmac_user_stru *pst_hmac_user, oal_uint8 *puc_payload, oal_uint32 ul_payload_len, oal_uint8 uc_mgmt_frm_type)
{
    oal_uint32 ul_payload_size;

    /* AP ����STA �Ĺ�������֡��Ϣ���Ա��ϱ��ں� */
    if (pst_hmac_user->puc_assoc_req_ie_buff != OAL_PTR_NULL) {
        OAL_MEM_FREE(pst_hmac_user->puc_assoc_req_ie_buff, OAL_TRUE);
        pst_hmac_user->puc_assoc_req_ie_buff = OAL_PTR_NULL;
        pst_hmac_user->ul_assoc_req_ie_len = 0;
    }
    ul_payload_size = ul_payload_len;

    /* Ŀǰ11rû��ʵ�֣����Դ����ع���֡�����̺͹���֡һ����11rʵ�ֺ�˴���Ҫ�޸� */
    if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) {
        /* �ع����ȹ�������֡ͷ����AP��MAC��ַ */
        puc_payload += (WLAN_MAC_ADDR_LEN + MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN);
        ul_payload_len -= (WLAN_MAC_ADDR_LEN + MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN);
    } else {
        puc_payload += (MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN);
        ul_payload_len -= (MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN);
    }

    pst_hmac_user->puc_assoc_req_ie_buff = OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, (oal_uint16)ul_payload_size, OAL_TRUE);
    if (pst_hmac_user->puc_assoc_req_ie_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_ap_prepare_assoc_req::Alloc Assoc Req for kernel failed.}");
        pst_hmac_user->ul_assoc_req_ie_len = 0;
        return OAL_FAIL;
    } else {
        if (memcpy_s(pst_hmac_user->puc_assoc_req_ie_buff, ul_payload_size, puc_payload, ul_payload_len) != EOK) {
            OAM_ERROR_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_ap_prepare_assoc_req::memcpy failed, payload_size[%d].}", ul_payload_size);
            OAL_MEM_FREE(pst_hmac_user->puc_assoc_req_ie_buff, OAL_TRUE);
            pst_hmac_user->puc_assoc_req_ie_buff = OAL_PTR_NULL;
            pst_hmac_user->ul_assoc_req_ie_len = 0;
            return OAL_FAIL;
        }
        pst_hmac_user->ul_assoc_req_ie_len = ul_payload_len;
        return OAL_SUCC;
    }
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_uint32 hmac_ap_set_is_wavetest_sta(hmac_vap_stru *pst_hmac_vap, oal_uint8 *auc_sta_addr)
{
    mac_device_stru *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "{hmac_ap_set_is_wavetest_sta::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����MAC��ַʶ�� */
    pst_mac_device->en_is_wavetest = MAC_IS_WAVETEST_STA(auc_sta_addr);

    return OAL_SUCC;
}
#endif
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

OAL_STATIC oal_uint32 hmac_check_sa_query_trigger(
    hmac_user_stru *pst_hmac_user, hmac_vap_stru *pst_hmac_vap, mac_status_code_enum_uint16 *pen_status_code)
{
    oal_uint32 ul_rslt;

    if ((pst_hmac_user->st_sa_query_info.st_sa_query_interval_timer.en_is_enabled != OAL_TRUE) &&
        (pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active == OAL_TRUE)) {
        OAM_INFO_LOG0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_check_sa_query_trigger::AP rx STA assoc req ,and start sa query process.}");
        ul_rslt = hmac_start_sa_query(&pst_hmac_vap->st_vap_base_info, pst_hmac_user,
                                      pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
        if (ul_rslt != OAL_SUCC) {
            OAM_ERROR_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_check_sa_query_trigger::hmac_start_sa_query failed[%d].}", ul_rslt);
            return ul_rslt;
        }
        OAM_INFO_LOG0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_check_sa_query_trigger::set en_status_code is MAC_REJECT_TEMP.}");
        *pen_status_code = MAC_REJECT_TEMP;
    }
    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32 hmac_ap_up_rx_asoc_req(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 uc_mgmt_frm_type, oal_uint8 *puc_mac_hdr,
    oal_uint32 ul_mac_hdr_len, oal_uint8 *puc_payload, oal_uint32 ul_payload_len)
{
    oal_uint32 ul_rslt;
    oal_netbuf_stru *pst_asoc_rsp = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    oal_uint16 us_user_idx = 0;
    oal_uint32 ul_asoc_rsp_len = 0;
    mac_status_code_enum_uint16 en_status_code;
    oal_uint8 auc_sta_addr[WLAN_MAC_ADDR_LEN];
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    mac_cfg_user_info_param_stru st_hmac_user_info_event;
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_P2P
    oal_int32 l_len;
#endif
    oal_uint8 uc_frm_least_len;

    uc_frm_least_len = MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN;
    uc_frm_least_len += (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) ? WLAN_MAC_ADDR_LEN : 0;

    if (ul_payload_len < uc_frm_least_len) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::RX assoc req len abnormal[%d]}", ul_payload_len);
        return OAL_FAIL;
    }

    mac_get_address2(puc_mac_hdr, auc_sta_addr);

    ul_rslt = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, &us_user_idx);
    if (ul_rslt != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::mac_vap_find_user_by_macaddr failed[%d].}", ul_rslt);
        OAM_WARNING_LOG4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::user mac:%02X:XX:XX:%02X:%02X:%02X.}",
                         /* auc_sta_addr 0��3��4��5byteΪ���������ӡ */
                         auc_sta_addr[0], auc_sta_addr[3], auc_sta_addr[4], auc_sta_addr[5]);
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, MAC_ASOC_NOT_AUTH, OAL_FALSE);

        return ul_rslt;
    }

    OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_ap_up_rx_asoc_req::us_user_idx[%d].}", us_user_idx);

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_asoc_req::pst_hmac_user[%d] null.}", us_user_idx);

        /* û�в鵽��Ӧ��USER,����ȥ��֤��Ϣ */
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, MAC_ASOC_NOT_AUTH, OAL_FALSE);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->st_mgmt_timer.en_is_registerd == OAL_TRUE) {
        FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_hmac_user->st_mgmt_timer);
    }

    en_status_code = MAC_SUCCESSFUL_STATUSCODE;

    /* �Ƿ���ϴ���SA query���̵����� */
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    ul_rslt = hmac_check_sa_query_trigger(pst_hmac_user, pst_hmac_vap, &en_status_code);
    if (ul_rslt != OAL_SUCC) {
        return ul_rslt;
    }
#endif

    if (en_status_code != MAC_REJECT_TEMP) {
        /* �����Բ��ҵ��û�ʱ,˵����ǰUSER��״̬Ϊ�ѹ���������֤��� */
        ul_rslt = hmac_ap_up_update_sta_user(pst_hmac_vap, puc_mac_hdr, puc_payload, ul_payload_len,
                                             pst_hmac_user, &en_status_code);
        if (en_status_code != MAC_SUCCESSFUL_STATUSCODE) {
            OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_ap_up_rx_asoc_req::hmac_ap_up_update_sta_user failed[%d].}", en_status_code);
#ifdef _PRE_DEBUG_MODE_USER_TRACK
            mac_user_change_info_event(pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                       pst_hmac_vap->st_vap_base_info.uc_vap_id,
                                       pst_hmac_user->st_user_base_info.en_user_asoc_state,
                                       MAC_USER_STATE_AUTH_COMPLETE, OAM_MODULE_ID_HMAC,
                                       OAM_USER_INFO_CHANGE_TYPE_ASSOC_STATE);
#endif
            hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info,
                                     MAC_USER_STATE_AUTH_COMPLETE);
        }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        /* ͬ��ap������������Ϣ��dmac */
        hmac_chan_sync(&pst_hmac_vap->st_vap_base_info, pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
                       pst_hmac_vap->st_vap_base_info.st_channel.en_bandwidth, OAL_FALSE);

#endif

        /* �����û�֧�ִ���������Э�̳���ǰ����dmac offload�ܹ��£�ͬ��������Ϣ��device */
        ul_rslt = hmac_config_user_info_syn(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info);
        if (ul_rslt != OAL_SUCC) {
            OAM_ERROR_LOG1(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_asoc_req::usr_info_syn failed[%d].}", ul_rslt);
        }

        if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
            ul_rslt = hmac_init_security(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, OAL_SIZEOF(auc_sta_addr));
            if (ul_rslt != OAL_SUCC) {
                OAM_ERROR_LOG2(0, OAM_SF_ASSOC, "hmac_ap_up_rx_asoc_req:hmac_init_security failed[%d] status_code[%d]",
                               ul_rslt, MAC_UNSPEC_FAIL);
                en_status_code = MAC_UNSPEC_FAIL;
            }

#if defined(_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2)
            ul_rslt = hmac_init_user_security_port(&(pst_hmac_vap->st_vap_base_info),
                                                   &(pst_hmac_user->st_user_base_info));
            if (ul_rslt != OAL_SUCC) {
                OAM_ERROR_LOG1(0, OAM_SF_ASSOC, "hmac_ap_up_rx_asoc_req:init user security_port failed[%d]", ul_rslt);
            }
#endif /* defined(_PRE_WLAN_FEATURE_WPA) ||��defined(_PRE_WLAN_FEATURE_WPA2) */
        }

        if ((ul_rslt != OAL_SUCC) ||
            (en_status_code != MAC_SUCCESSFUL_STATUSCODE)) {
            OAM_WARNING_LOG2(0, OAM_SF_CFG, "hmac_ap_up_rx_asoc_req:ap update user fail rslt[%d] status_code[%d]",
                             ul_rslt, en_status_code);
#ifdef _PRE_DEBUG_MODE_USER_TRACK
            mac_user_change_info_event(pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                       pst_hmac_vap->st_vap_base_info.uc_vap_id,
                                       pst_hmac_user->st_user_base_info.en_user_asoc_state,
                                       MAC_USER_STATE_AUTH_COMPLETE, OAM_MODULE_ID_HMAC,
                                       OAM_USER_INFO_CHANGE_TYPE_ASSOC_STATE);
#endif
            hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info,
                                     MAC_USER_STATE_AUTH_COMPLETE);
        }
#ifdef _PRE_WLAN_FEATURE_P2P
        l_len = ((uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) ?
                (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN + WLAN_MAC_ADDR_LEN) : (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN));
        if (IS_P2P_GO(&pst_hmac_vap->st_vap_base_info) &&
            (mac_find_vendor_ie(MAC_WLAN_OUI_WFA, MAC_WLAN_OUI_TYPE_WFA_P2P, puc_payload + l_len,
                                (oal_int32)ul_payload_len - l_len) == OAL_PTR_NULL)) {
            hmac_disable_p2p_pm(pst_hmac_vap);
        }
#endif
    }

    pst_asoc_rsp =
        (oal_netbuf_stru *)OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_asoc_rsp == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_asoc_req::pst_asoc_rsp null.}");
        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_asoc_rsp);
    memset_s(pst_tx_ctl, OAL_NETBUF_CB_SIZE(), 0, OAL_NETBUF_CB_SIZE());

    OAL_MEM_NETBUF_TRACE(pst_asoc_rsp, OAL_TRUE);

    if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_ASSOC_REQ) {
        ul_asoc_rsp_len = hmac_mgmt_encap_asoc_rsp_ap(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr,
                                                      pst_hmac_user->st_user_base_info.us_assoc_id, en_status_code,
                                                      OAL_NETBUF_HEADER(pst_asoc_rsp), WLAN_FC0_SUBTYPE_ASSOC_RSP);
    } else if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) {
        ul_asoc_rsp_len = hmac_mgmt_encap_asoc_rsp_ap(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr,
                                                      pst_hmac_user->st_user_base_info.us_assoc_id,
                                                      en_status_code, OAL_NETBUF_HEADER(pst_asoc_rsp),
                                                      WLAN_FC0_SUBTYPE_REASSOC_RSP);
    }

    if (ul_asoc_rsp_len == 0) {
        OAM_WARNING_LOG0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::hmac_mgmt_encap_asoc_rsp_ap encap msg fail.}");
        oal_netbuf_free(pst_asoc_rsp);

        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return OAL_FAIL;
    }

    oal_netbuf_put(pst_asoc_rsp, ul_asoc_rsp_len);

    pst_tx_ctl->us_tx_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_tx_ctl->us_mpdu_len = (oal_uint16)ul_asoc_rsp_len;

    /* ���͹�����Ӧ֮֡ǰ�����û��Ľ���״̬��λ */
    hmac_mgmt_reset_psm(&pst_hmac_vap->st_vap_base_info, pst_tx_ctl->us_tx_user_idx);

    /* �жϵ�ǰ״̬������û��Ѿ������ɹ������ϱ��û��뿪��Ϣ */
    if (pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        pst_net_device = hmac_vap_get_net_device(pst_hmac_vap->st_vap_base_info.uc_vap_id);
        if (pst_net_device != OAL_PTR_NULL) {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
            oal_kobject_uevent_env_sta_leave(pst_net_device, auc_sta_addr);
#endif
#endif
        }
    }

    if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
        hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info,
                                 MAC_USER_STATE_ASSOC);
    }

    ul_rslt = hmac_tx_mgmt_send_event(&(pst_hmac_vap->st_vap_base_info), pst_asoc_rsp, (oal_uint16)ul_asoc_rsp_len);
    if (ul_rslt != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::hmac_tx_mgmt_send_event failed[%d].}", ul_rslt);
        oal_netbuf_free(pst_asoc_rsp);

        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return ul_rslt;
    }

    if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
        /* Ϊ�˽��wavetest����MCS9 shortGI�������ܵ͵�����:wavetest���Գ����£�AGC�̶���ͨ��0 */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
        hmac_ap_set_is_wavetest_sta(pst_hmac_vap, auc_sta_addr);
#endif

        /* AP���STA�ɹ�������������ɹ� */
#ifdef _PRE_DEBUG_MODE_USER_TRACK
        mac_user_change_info_event(pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                   pst_hmac_vap->st_vap_base_info.uc_vap_id,
                                   pst_hmac_user->st_user_base_info.en_user_asoc_state,
                                   MAC_USER_STATE_ASSOC, OAM_MODULE_ID_HMAC,
                                   OAM_USER_INFO_CHANGE_TYPE_ASSOC_STATE);
#endif

        ul_rslt = hmac_config_user_rate_info_syn(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info);
        if (ul_rslt != OAL_SUCC) {
            OAM_ERROR_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_sta_wait_asoc_rx::hmac_config_user_rate_info_syn failed[%d].}", ul_rslt);
        }

        /* user�Ѿ������ϣ����¼���DMAC����DMAC����û��㷨���� */
        hmac_user_add_notify_alg((&pst_hmac_vap->st_vap_base_info), us_user_idx);

        /* AP ����STA �Ĺ�������֡��Ϣ���Ա��ϱ��ں� */
        hmac_ap_prepare_assoc_req(pst_hmac_user, puc_payload, ul_payload_len, uc_mgmt_frm_type);

        /* �ϱ�WAL��(WAL�ϱ��ں�) AP��������һ���µ�STA */
        hmac_handle_connect_rsp_ap(pst_hmac_vap, pst_hmac_user);
        OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::STA assoc AP SUCC! STA_indx=%d.}", us_user_idx);
    } else {
        /* AP���STAʧ�ܣ�����ɾ�� */
        if (en_status_code != MAC_REJECT_TEMP) {
            hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
        }
    }

    /* 1102 STA �������ϱ�VAP ��Ϣ���û���Ϣ */
    st_hmac_user_info_event.us_user_idx = us_user_idx;

    hmac_config_vap_info(&(pst_hmac_vap->st_vap_base_info), OAL_SIZEOF(oal_uint32), (oal_uint8 *)&ul_rslt);
    hmac_config_user_info(&(pst_hmac_vap->st_vap_base_info), OAL_SIZEOF(mac_cfg_user_info_param_stru),
                          (oal_uint8 *)&st_hmac_user_info_event);
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_ap_up_rx_disasoc(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_mac_hdr, oal_uint32 ul_mac_hdr_len,
    oal_uint8 *puc_payload, oal_uint32 ul_payload_len, oal_bool_enum_uint8 en_is_protected)
{
    oal_uint32 ul_ret;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    oal_uint8 *puc_da = OAL_PTR_NULL;
    oal_uint8 *puc_sa = OAL_PTR_NULL;
    oal_uint8 auc_sta_addr[WLAN_MAC_ADDR_LEN];

    if (ul_payload_len < MAC_80211_REASON_CODE_LEN) {
        OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_disasoc::invalid ul_payload_len len [%d]}", ul_payload_len);
        return OAL_FAIL;
    }

    mac_get_address2(puc_mac_hdr, auc_sta_addr);

    /* ���ӽ��յ�ȥ����֡ʱ��ά����Ϣ */
    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

    OAM_WARNING_LOG4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_ap_up_rx_disasoc::Because of err_code[%d], received disassoc frame from source addr \
                     %02x:xx:xx:xx:%02x:%02x.}", *((oal_uint16 *)(puc_mac_hdr + MAC_80211_FRAME_LEN)),
                     puc_sa[0], puc_sa[4], puc_sa[5]); /* puc_sa 0��4��5byteΪ���������ӡ */

    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_disasoc::pst_hmac_user null.}");
        /* û�в鵽��Ӧ��USER,����ȥ��֤��Ϣ */
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, MAC_NOT_ASSOCED, OAL_FALSE);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
        /* ����Ƿ���Ҫ����SA query request */
        if (hmac_pmf_check_err_code(&pst_hmac_user->st_user_base_info, en_is_protected, puc_mac_hdr) == OAL_SUCC) {
            /* �ڹ���״̬���յ�δ���ܵ�ReasonCode 6/7��Ҫ����SA Query���� */
            ul_ret = hmac_start_sa_query(&pst_hmac_vap->st_vap_base_info, pst_hmac_user,
                                         pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
            if (ul_ret != OAL_SUCC) {
                OAM_ERROR_LOG1(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_disasoc::hmac_start_sa_query failed[%d].}", ul_ret);
                return OAL_ERR_CODE_PMF_SA_QUERY_START_FAIL;
            }
            return OAL_SUCC;
        }
#endif

        /* ������û��Ĺ���֡�������Բ�һ�£������ñ��� */
        mac_rx_get_da((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_da);
        if ((ETHER_IS_MULTICAST(puc_da) != OAL_TRUE) &&
            (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active)) {
            OAM_ERROR_LOG1(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_disasoc:en_is_protected=%d.}", en_is_protected);
            return OAL_FAIL;
        }

        mac_user_set_asoc_state(&pst_hmac_user->st_user_base_info, MAC_USER_STATE_AUTH_COMPLETE);
#ifdef _PRE_WLAN_1102A_CHR
        CHR_EXCEPTION_REPORT (CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                              CHR_WIFI_DRV_EVENT_SOFTAP_PASSIVE_DISCONNECT,
                              *((oal_uint16 *)(puc_mac_hdr + MAC_80211_FRAME_LEN)));
#endif
        /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
        hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

        /* ��Щ����ȥ����ʱֻ����DISASOC,Ҳ��ɾ������AP�ڲ������ݽṹ */
        ul_ret = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ASSOC, "{hmac_ap_up_rx_disasoc::hmac_user_del failed[%d].}", ul_ret);
        }
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST

oal_bool_enum_uint8 hmac_ap_up_rx_should_ingore_2040(
    wlan_channel_band_enum_uint8 en_band, oal_bool_enum_uint8 en_support_2040)
{
    return ((en_band == WLAN_BAND_5G) || (en_support_2040 == OAL_FALSE));
}

OAL_STATIC oal_void hmac_ap_up_rx_2040_coext(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    mac_device_stru *pst_mac_device;
    mac_vap_stru *pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    dmac_rx_ctl_stru *pst_rx_ctrl;
    oal_uint32 ul_index;
    oal_uint8 uc_coext_info;
    oal_uint8 *puc_data;
    oal_uint32 ul_ret = OAL_SUCC;

    /* 5GHzƵ�κ��� 20/40 BSS�������֡ */
    if (hmac_ap_up_rx_should_ingore_2040(pst_mac_vap->st_channel.en_band,
                                         mac_mib_get_2040BSSCoexistenceManagementSupport(pst_mac_vap)) == OAL_TRUE) {
        OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_2040,
                      "{hmac_ap_up_rx_2040_coext::Now in 5GHz.}");
        return;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_2040, "{hmac_ap_up_rx_2040_coext::pst_mac_device null.}");
        return;
    }

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ��ȡ֡��ָ�� */
    puc_data = (oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr + pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    ul_index = MAC_ACTION_OFFSET_ACTION + 1;

    /* 20/40 BSS Coexistence IE */
    if (puc_data[ul_index] == MAC_EID_2040_COEXT) {
        uc_coext_info = puc_data[ul_index + MAC_IE_HDR_LEN];

        /* BIT1 - Forty MHz Intolerant */
        /* BIT2 - 20 MHz BSS Width Request */
        if (uc_coext_info & (BIT1 | BIT2)) {
            hmac_chan_start_40M_recovery_timer(pst_mac_vap);

            /* ���BIT1��BIT2����Ϊ1���ҵ�ǰ�����ŵ���ȴ���20MHz����AP��Ҫ�л���20MHz���� */
            if (pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_20M) {
                hmac_chan_multi_switch_to_20MHz_ap(pst_hmac_vap);
            } else {
                pst_mac_device->en_40MHz_intol_bit_recd = OAL_TRUE;
            }
        }

        ul_index += (MAC_IE_HDR_LEN + puc_data[ul_index + 1]);
    }

    /* 20/40 BSS Intolerant Channel Report IE */
    if (puc_data[ul_index] == MAC_EID_2040_INTOLCHREPORT) {
        oal_uint8 uc_len = puc_data[ul_index + 1];
        oal_uint8 uc_chan_idx = 0;
        oal_uint8 uc_loop;
        oal_bool_enum_uint8 en_reval_chan = OAL_FALSE;

        ul_index += (MAC_IE_HDR_LEN + 1); /* skip Element ID��Length��Operating Class */
        for (uc_loop = 0; (uc_len >= 1) && (uc_loop < uc_len - 1); uc_loop++, ul_index++) {
            ul_ret = mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band, puc_data[ul_index], &uc_chan_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_2040,
                                 "{hmac_ap_up_rx_2040_coext::mac_get_channel_idx_from_num failed[%d].}", ul_ret);
                continue;
            }

            ul_ret = mac_is_channel_idx_valid(pst_mac_vap->st_channel.en_band, uc_chan_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_2040,
                                 "hmac_ap_up_rx_2040_coext::chan_indx=%d not valid, return[%d]", uc_chan_idx, ul_ret);
                continue;
            }

            if (pst_mac_device->st_ap_channel_list[uc_chan_idx].en_ch_type != MAC_CH_TYPE_PRIMARY) {
                pst_mac_device->st_ap_channel_list[uc_chan_idx].en_ch_type = MAC_CH_TYPE_PRIMARY;
                en_reval_chan = OAL_TRUE;
            }
        }

        if (en_reval_chan == OAL_TRUE) {
            OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_2040,
                             "{hmac_ap_up_rx_2040_coext::Re-evaluation needed as some channel status changed.}");

            /* ��������ŵ����ߴ����л����Ž����л� */
            if (pst_hmac_vap->en_2040_switch_prohibited == 0) {
                hmac_chan_start_40M_recovery_timer(pst_mac_vap);
                /* ���������Ƿ���Ҫ�����ŵ��л� */
                hmac_chan_reval_status(pst_mac_device, pst_mac_vap);
            }
        }
    }
}

OAL_STATIC oal_bool_enum_uint8 hmac_ap_is_40MHz_intol_bit_set(oal_uint8 *puc_payload, oal_uint16 us_payload_len)
{
    oal_uint16 us_ht_cap_info = 0;
    oal_uint8 *puc_ht_cap = OAL_PTR_NULL;

    if (us_payload_len <= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_ap_is_40MHz_intol_bit_set::payload_len[%d]}", us_payload_len);
        return OAL_FALSE;
    }

    us_payload_len -= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    puc_payload += (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);

    puc_ht_cap = mac_find_ie(MAC_EID_HT_CAP, puc_payload, us_payload_len);
    if (puc_ht_cap != OAL_PTR_NULL) {
        us_ht_cap_info = OAL_MAKE_WORD16(puc_ht_cap[MAC_IE_HDR_LEN], puc_ht_cap[MAC_IE_HDR_LEN + 1]);
        /* Forty MHz Intolerant BIT Ϊ1 */
        if (us_ht_cap_info & BIT14) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


OAL_STATIC oal_bool_enum_uint8 hmac_ap_update_2040_chan_info(
    mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap, oal_uint8 *puc_payload,
    oal_uint16 us_payload_len, oal_uint8 uc_pri_chan_idx, mac_sec_ch_off_enum_uint8 en_sec_ch_offset)
{
    wlan_channel_band_enum_uint8 en_band = pst_mac_vap->st_channel.en_band;
    mac_ap_ch_info_stru *pst_ap_ch_list = pst_mac_device->st_ap_channel_list;
    oal_bool_enum_uint8 en_status_change = OAL_FALSE;
    oal_uint8 uc_sec_ch_idx_offset = mac_get_sec_ch_idx_offset(en_band);
    oal_uint8 uc_sec_chan_idx = 0;
    oal_uint32 ul_ret;

    if (hmac_ap_is_40MHz_intol_bit_set(puc_payload, us_payload_len) == OAL_TRUE) {
        pst_mac_device->en_40MHz_intol_bit_recd = OAL_TRUE;
    }

    if (pst_ap_ch_list[uc_pri_chan_idx].en_ch_type != MAC_CH_TYPE_PRIMARY) {
        pst_ap_ch_list[uc_pri_chan_idx].en_ch_type = MAC_CH_TYPE_PRIMARY;
        en_status_change = OAL_TRUE;
    }

    if (en_sec_ch_offset != MAC_SCN) {
        if (en_sec_ch_offset == MAC_SCA) {
            uc_sec_chan_idx = uc_pri_chan_idx + uc_sec_ch_idx_offset;
        } else if (en_sec_ch_offset == MAC_SCB) {
            uc_sec_chan_idx = uc_pri_chan_idx - uc_sec_ch_idx_offset;
        }

        ul_ret = mac_is_channel_idx_valid(en_band, uc_sec_chan_idx);
        if (ul_ret != OAL_SUCC) {
            return en_status_change;
        }

        if (uc_sec_chan_idx >= MAC_MAX_SUPP_CHANNEL) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_2040,
                             "{hmac_ap_update_2040_chan_info::invalid uc_sec_chan_idx[%d].}", uc_sec_chan_idx);
            return en_status_change;
        }

        if (pst_ap_ch_list[uc_sec_chan_idx].en_ch_type != MAC_CH_TYPE_PRIMARY) {
            if (pst_ap_ch_list[uc_sec_chan_idx].en_ch_type != MAC_CH_TYPE_SECONDARY) {
                pst_ap_ch_list[uc_sec_chan_idx].en_ch_type = MAC_CH_TYPE_SECONDARY;
                en_status_change = OAL_TRUE;
            }
        }
    }

    return en_status_change;
}


OAL_STATIC oal_void hmac_ap_process_obss_40mhz_intol(
    hmac_vap_stru *pst_hmac_vap, oal_uint8 *puc_payload, oal_uint16 us_payload_len)
{
    mac_device_stru *pst_mac_device;

    /* ��HTվ�㲻֧��OBSS Beacon/Probe Response֡�Ĵ��� */
    if ((mac_mib_get_HighThroughputOptionImplemented(&(pst_hmac_vap->st_vap_base_info)) == OAL_FALSE)) {
        OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                      "{hmac_ap_process_obss_40mhz_intol::HT not support.}");
        return;
    }

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_ap_process_obss_40mhz_intol::pst_mac_device null.}");
        return;
    }

    if (hmac_ap_is_40MHz_intol_bit_set(puc_payload, us_payload_len) == OAL_TRUE) {
        hmac_chan_start_40M_recovery_timer(&pst_hmac_vap->st_vap_base_info);
        /* ��BSS��ǰ�����ŵ���ȴ���20MHz����AP��Ҫ�л���20MHz���� */
        if ((pst_hmac_vap->st_vap_base_info.st_channel.en_bandwidth > WLAN_BAND_WIDTH_20M) &&
            (pst_hmac_vap->st_vap_base_info.st_ch_switch_info.en_bw_switch_status == WLAN_BW_SWITCH_DONE)) {
            OAM_INFO_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                          "{hmac_ap_process_obss_40mhz_intol::40MHz Intolerant AP in neighbourhood.}");
            hmac_chan_multi_switch_to_20MHz_ap(pst_hmac_vap);
        } else {
            pst_mac_device->en_40MHz_intol_bit_recd = OAL_TRUE;
        }
    }
}

#endif


OAL_STATIC oal_void hmac_ap_up_rx_obss_beacon(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    oal_uint8 *puc_payload;
    oal_uint16 us_payload_len;

    /* ��ȡ֡�峤�� */
    us_payload_len = pst_rx_ctrl->st_rx_info.us_frame_len - pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* ��ȡ֡��ָ�� */
    puc_payload =
        (oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr + pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* ����ERP��� */
    if (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) {
        hmac_ap_process_obss_erp_ie(pst_hmac_vap, puc_payload, us_payload_len);
    }

    /* ����HT operation��� */
    hmac_ap_process_obss_ht_operation_ie(pst_hmac_vap, puc_payload, us_payload_len);

    /* ����AP�б������mib�� */
    hmac_protection_update_mib_ap(&(pst_hmac_vap->st_vap_base_info));

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
    if ((mac_mib_get_FortyMHzOperationImplemented(&(pst_hmac_vap->st_vap_base_info)) == OAL_TRUE) &&
        (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) &&
        (pst_hmac_vap->st_vap_base_info.st_cap_flag.bit_2040_autoswitch == OAL_TRUE)) {
        hmac_ap_process_obss_40mhz_intol(pst_hmac_vap, puc_payload, us_payload_len);
    }

#endif
}


OAL_STATIC oal_void hmac_ap_up_rx_action_nonuser(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    oal_uint8 *puc_data = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL; /* ����mac֡��ָ�� */

    if (OAL_UNLIKELY((pst_hmac_vap == OAL_PTR_NULL) || (pst_netbuf == OAL_PTR_NULL))) {
        OAM_WARNING_LOG0(0, OAM_SF_RX, "{hmac_ap_up_rx_action_nonuser:: NULL param.}");
        return;
    }

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ��ȡ֡ͷ��Ϣ */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr;

    /* ��ȡ֡��ָ�� */
    puc_data = (oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr + pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* Category */
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_PUBLIC: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_LOCATION_RAM
                case MAC_PUB_VENDOR_SPECIFIC: {
                    if (oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN) == 0) {
                        OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                         "{hmac_ap_up_rx_action::hmac location get.}");
                        hmac_huawei_action_process(pst_hmac_vap, pst_netbuf,
                                                   puc_data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
                    }
                    break;
                }
#endif
                default:
                    break;
            }
        }
        break;
        default:
            break;
    }
    return;
}


OAL_STATIC oal_void hmac_ap_up_rx_action(
    hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf, oal_bool_enum_uint8 en_is_protected)
{
    dmac_rx_ctl_stru *pst_rx_ctrl;
    oal_uint8 *puc_data = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr; /* ����mac֡��ָ�� */
    hmac_user_stru *pst_hmac_user;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ��ȡ֡ͷ��Ϣ */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr;

    /* ��ȡ���Ͷ˵��û�ָ�� */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, pst_frame_hdr->auc_address2);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_ap_up_rx_action::mac_vap_find_user_by_macaddr failed.}");
        hmac_ap_up_rx_action_nonuser(pst_hmac_vap, pst_netbuf);
        return;
    }

    /* ��ȡ֡��ָ�� */
    puc_data = (oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr + pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* Category */
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_BA: {
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_BA_ACTION_ADDBA_REQ:
                    hmac_mgmt_rx_addba_req(pst_hmac_vap, pst_hmac_user, puc_data);
                    break;
                case MAC_BA_ACTION_ADDBA_RSP:
                    hmac_mgmt_rx_addba_rsp(pst_hmac_vap, pst_hmac_user, puc_data);
                    break;
                case MAC_BA_ACTION_DELBA:
                    hmac_mgmt_rx_delba(pst_hmac_vap, pst_hmac_user, puc_data);
                    break;
                default:
                    break;
            }
        }
        break;

        case MAC_ACTION_CATEGORY_PUBLIC: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
                case MAC_PUB_COEXT_MGMT:
                    if (pst_hmac_vap->st_vap_base_info.st_cap_flag.bit_2040_autoswitch == OAL_TRUE) {
                        hmac_ap_up_rx_2040_coext(pst_hmac_vap, pst_netbuf);
                    }
                    break;
#endif /* _PRE_WLAN_FEATURE_20_40_80_COEXIST */
                case MAC_PUB_VENDOR_SPECIFIC: {
                    /* ����OUI-OUI typeֵΪ 50 6F 9A - 09 (WFA P2P v1.0) */
                    /* ����hmac_rx_mgmt_send_to_host�ӿ��ϱ� */
                    if (mac_ie_check_p2p_action(puc_data + MAC_ACTION_OFFSET_ACTION) == OAL_TRUE) {
                        hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
                    }
#ifdef _PRE_WLAN_FEATURE_LOCATION_RAM
                    if (oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN) == 0) {
                        OAM_WARNING_LOG0(0, OAM_SF_RX, "{hmac_ap_up_rx_action::hmac location get.}");
                        hmac_huawei_action_process(pst_hmac_vap, pst_netbuf,
                                                   puc_data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
                    }
#endif
                    break;
                }
                default:
                    break;
            }
        }
        break;

        case MAC_ACTION_CATEGORY_HT: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
                case MAC_HT_ACTION_NOTIFY_CHANNEL_WIDTH:
                    hmac_rx_notify_channel_width(&(pst_hmac_vap->st_vap_base_info), pst_netbuf);
                    break;
#endif
                case MAC_HT_ACTION_BUTT:
                default:
                    break;
            }
        }
        break;
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
        case MAC_ACTION_CATEGORY_SA_QUERY: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_SA_QUERY_ACTION_REQUEST:
                    hmac_rx_sa_query_req(pst_hmac_vap, pst_netbuf, en_is_protected);
                    break;
                case MAC_SA_QUERY_ACTION_RESPONSE:
                    hmac_rx_sa_query_rsp(pst_hmac_vap, pst_netbuf, en_is_protected);
                    break;
                default:
                    break;
            }
        }
        break;
#endif
        case MAC_ACTION_CATEGORY_VHT: {
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
                case MAC_VHT_ACTION_OPREATE_MODE_NOTIFY:
                    hmac_mgmt_rx_opmode_notify_frame(pst_hmac_vap, pst_netbuf);
                    break;
#endif
                case MAC_VHT_ACTION_BUTT:
                default:
                    break;
            }
        }
        break;

        case MAC_ACTION_CATEGORY_VENDOR: {
            /* ����OUI-OUI typeֵΪ 50 6F 9A - 09 (WFA P2P v1.0) */
            /* ����hmac_rx_mgmt_send_to_host�ӿ��ϱ� */
            if (mac_ie_check_p2p_action(puc_data + MAC_ACTION_OFFSET_CATEGORY) == OAL_TRUE) {
                hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
            }
        }
        break;

#ifdef _PRE_WLAN_FEATURE_WMMAC
        case MAC_ACTION_CATEGORY_WMMAC_QOS: {
            if (g_en_wmmac_switch == OAL_TRUE) {
                switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                    case MAC_WMMAC_ACTION_ADDTS_REQ:
                        hmac_mgmt_rx_addts_req_frame(pst_hmac_vap, pst_netbuf);
                        break;
                    default:
                        break;
                }
            }
        }
        break;
#endif  // _PRE_WLAN_FEATURE_WMMAC

        case MAC_PUB_VENDOR_SPECIFIC:
#ifdef _PRE_WLAN_FEATURE_LOCATION_RAM
            if (oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN) == 0) {
                OAM_WARNING_LOG0(0, OAM_SF_RX, "{hmac_ap_up_rx_action::hmac location get.}");
                hmac_huawei_action_process(pst_hmac_vap, pst_netbuf,
                                           puc_data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
            }
#endif /* _PRE_WLAN_FEATURE_FTM */
            break;
        default:
            break;
    }
}


OAL_STATIC oal_void hmac_ap_up_rx_probe_req(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl;
    mac_rx_ctl_stru *pst_rx_info;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    enum nl80211_band en_band;
#else
    enum ieee80211_band en_band;
#endif
    oal_int l_freq;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));

    /* ��ȡAP ��ǰ�ŵ� */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    if (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) {
        en_band = NL80211_BAND_2GHZ;
    } else if (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_5G) {
        en_band = NL80211_BAND_5GHZ;
    } else {
        en_band = NUM_NL80211_BANDS;
    }
#else
    if (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_2G) {
        en_band = IEEE80211_BAND_2GHZ;
    } else if (pst_hmac_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_5G) {
        en_band = IEEE80211_BAND_5GHZ;
    } else {
        en_band = IEEE80211_NUM_BANDS;
    }
#endif
    l_freq = oal_ieee80211_channel_to_frequency(pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
                                                en_band);

    /* �ϱ����յ���probe req ����֡ */
    hmac_send_mgmt_to_host(pst_hmac_vap, pst_netbuf, pst_rx_info->us_frame_len, l_freq);
}


oal_uint32 hmac_ap_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap, oal_void *p_param)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    oal_uint8 *puc_mac_hdr = OAL_PTR_NULL;
    oal_uint8 *puc_payload = OAL_PTR_NULL;
    oal_uint32 ul_msg_len;     /* ��Ϣ�ܳ���,������FCS */
    oal_uint32 ul_mac_hdr_len; /* MACͷ���� */
    oal_uint8 uc_mgmt_frm_type;
    oal_bool_enum_uint8 en_is_protected;
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    oal_uint8 *puc_sa;
    oal_bool_enum_uint8 en_blacklist_result;
#endif
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;

    if (OAL_ANY_NULL_PTR2(pst_hmac_vap, p_param)) {
        OAM_ERROR_LOG2(0, OAM_SF_RX, "hmac_ap_up_rx_mgmt:null, %x %x", (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    puc_mac_hdr = (oal_uint8 *)(pst_rx_info->pul_mac_hdr_start_addr);
    ul_mac_hdr_len = pst_rx_info->uc_mac_header_len; /* MACͷ���� */
    puc_payload = (oal_uint8 *)(puc_mac_hdr) + ul_mac_hdr_len;
    ul_msg_len = pst_rx_info->us_frame_len; /* ��Ϣ�ܳ���,������FCS */
    en_is_protected = (oal_uint8)mac_get_protectedframe(puc_mac_hdr);

    /* AP��UP״̬�� ���յ��ĸ��ֹ���֡���� */
    uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (wlan_pm_wkup_src_debug_get() == OAL_TRUE) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        OAM_WARNING_LOG1(0, OAM_SF_RX, "{wifi_wake_src:hmac_ap_up_rx_mgmt::wakeup mgmt type[0x%x]}", uc_mgmt_frm_type);
    }
#endif

    /* Bar frame proc here */
    if (mac_get_frame_type(puc_mac_hdr) == WLAN_FC0_TYPE_CTL) {
        uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);
        if ((uc_mgmt_frm_type >> 4) == WLAN_BLOCKACK_REQ) { /* �ж�uc_mgmt_frm_type��4λ�Ƿ�Ϊ bar��ֵ(1000) */
            hmac_up_rx_bar(pst_hmac_vap, pst_rx_ctrl, pst_mgmt_rx_event->pst_netbuf);
        }
    }

#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

    /* �Զ������������� */
    if ((uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_ASSOC_REQ) || (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ)) {
        hmac_autoblacklist_filter(&pst_hmac_vap->st_vap_base_info, puc_sa);
    }

    /* ���������˼�� */
    en_blacklist_result = hmac_blacklist_filter(&pst_hmac_vap->st_vap_base_info, puc_sa);
    if (en_blacklist_result == OAL_TRUE) {
#ifdef _PRE_WLAN_NARROW_BAND
        if (!hitalk_status)
#endif
        {
            /* խ��APUTģʽ�£������STA�޷��������������λ */
            return OAL_SUCC;
        }
    }
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
    proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
    switch (uc_mgmt_frm_type) {
        case WLAN_FC0_SUBTYPE_BEACON:
        case WLAN_FC0_SUBTYPE_PROBE_RSP:
            hmac_ap_up_rx_obss_beacon(pst_hmac_vap, pst_mgmt_rx_event->pst_netbuf);
            break;
        case WLAN_FC0_SUBTYPE_AUTH:
            hmac_ap_rx_auth_req(pst_hmac_vap, pst_mgmt_rx_event->pst_netbuf, pst_mgmt_rx_event);
            break;
        case WLAN_FC0_SUBTYPE_DEAUTH:
            if (ul_msg_len < ul_mac_hdr_len + MAC_80211_REASON_CODE_LEN) {
                OAM_WARNING_LOG1(0, OAM_SF_RX, "{hmac_ap_up_rx_mgmt::invalid deauth_req length[%d]}", ul_msg_len);
                break;
            }
            hmac_ap_rx_deauth_req(pst_hmac_vap, puc_mac_hdr, en_is_protected);
            break;
        case WLAN_FC0_SUBTYPE_ASSOC_REQ:
        case WLAN_FC0_SUBTYPE_REASSOC_REQ:
            hmac_ap_up_rx_asoc_req(pst_hmac_vap, uc_mgmt_frm_type, puc_mac_hdr, ul_mac_hdr_len,
                                   puc_payload, (ul_msg_len - ul_mac_hdr_len));
            break;
        case WLAN_FC0_SUBTYPE_DISASSOC:
            hmac_ap_up_rx_disasoc(pst_hmac_vap, puc_mac_hdr, ul_mac_hdr_len, puc_payload,
                                  (ul_msg_len - ul_mac_hdr_len), en_is_protected);
            break;
        case WLAN_FC0_SUBTYPE_ACTION:
            hmac_ap_up_rx_action(pst_hmac_vap, pst_mgmt_rx_event->pst_netbuf, en_is_protected);
            break;
        case WLAN_FC0_SUBTYPE_PROBE_REQ:
            hmac_ap_up_rx_probe_req(pst_hmac_vap, pst_mgmt_rx_event->pst_netbuf);
            break;
        default:
            break;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_ap_store_network(mac_device_stru *pst_mac_device, oal_netbuf_stru *pst_netbuf)
{
    mac_bss_id_list_stru *pst_bss_id_list = &pst_mac_device->st_bss_id_list;
    dmac_rx_ctl_stru *pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    oal_uint8 auc_network_bssid[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_bool_enum_uint8 en_already_present = OAL_FALSE;
    oal_uint8 uc_loop;

    /* ��ȡ֡���е�BSSID */
    mac_get_bssid((oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr, auc_network_bssid);

    /* ���Թ㲥BSSID */
    if (oal_compare_mac_addr(BROADCAST_MACADDR, auc_network_bssid) == 0) {
        return;
    }

    /* �ж��Ƿ��Ѿ������˸�BSSID */
    for (uc_loop = 0; (uc_loop < pst_bss_id_list->us_num_networks) && (uc_loop < WLAN_MAX_SCAN_BSS_PER_CH); uc_loop++) {
        if (oal_compare_mac_addr(pst_bss_id_list->auc_bssid_array[uc_loop], auc_network_bssid) == 0) {
            en_already_present = OAL_TRUE;
            break;
        }
    }

    /* ����һ���µ�BSS��֡�������BSSID */
    if ((en_already_present == OAL_FALSE) &&
        (pst_bss_id_list->us_num_networks < WLAN_MAX_SCAN_BSS_PER_CH) &&
        (uc_loop < WLAN_MAX_SCAN_BSS_PER_CH)) {
        oal_set_mac_addr((oal_uint8 *)pst_bss_id_list->auc_bssid_array[uc_loop], (oal_uint8 *)auc_network_bssid);
        pst_bss_id_list->us_num_networks++;
    }
}


OAL_STATIC oal_uint32 hmac_ap_get_chan_idx_of_network(mac_vap_stru *pst_mac_vap,
                                                      oal_uint8 *puc_payload,
                                                      oal_uint16 us_payload_len,
                                                      oal_uint8 uc_curr_chan_idx,
                                                      oal_uint8 *puc_chan_idx,
                                                      mac_sec_ch_off_enum_uint8 *pen_sec_ch_offset)
{
    wlan_channel_band_enum_uint8 en_band = pst_mac_vap->st_channel.en_band;
    oal_uint8 uc_ch_idx = 0xFF;
    oal_uint32 ul_ret;
    oal_uint8 *puc_ie = OAL_PTR_NULL;

    if (us_payload_len <= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN)) {
        *puc_chan_idx = uc_curr_chan_idx;

        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_ap_get_chan_idx_of_network::payload_len[%d]}", us_payload_len);
        return OAL_SUCC;
    }

    us_payload_len -= (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    puc_payload += (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);

    puc_ie = mac_find_ie(MAC_EID_HT_OPERATION, puc_payload, us_payload_len);
    if (puc_ie != OAL_PTR_NULL) {
        ul_ret = mac_get_channel_idx_from_num(en_band, puc_ie[MAC_IE_HDR_LEN], &uc_ch_idx);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{hmac_ap_get_chan_idx_of_network:get channel idx failed(band [%d], channel[%d])!}",
                             en_band, puc_ie[MAC_IE_HDR_LEN]);
            return ul_ret;
        }

        ul_ret = mac_is_channel_idx_valid(en_band, uc_ch_idx);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "hmac_ap_get_chan_idx_of_network:channel_idx_valid failed(band[%d], channel[%d], idx[%d])",
                             en_band, puc_ie[MAC_IE_HDR_LEN], uc_ch_idx);
            return ul_ret;
        }

        *puc_chan_idx = uc_ch_idx;
        *pen_sec_ch_offset = puc_ie[MAC_IE_HDR_LEN + 1] & 0x03;

        return OAL_SUCC;
    }

    if (en_band == WLAN_BAND_2G) {
        puc_ie = mac_find_ie(MAC_EID_DSPARMS, puc_payload, us_payload_len);
        if (puc_ie != OAL_PTR_NULL) {
            ul_ret = mac_get_channel_idx_from_num(en_band, puc_ie[MAC_IE_HDR_LEN], &uc_ch_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "{hmac_ap_get_chan_idx_of_network:get channel idx failed(band [%d], channel[%d])!}",
                                 en_band, puc_ie[MAC_IE_HDR_LEN]);
                return ul_ret;
            }

            ul_ret = mac_is_channel_idx_valid(en_band, uc_ch_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "hmac_ap_get_chan_idx_of_network:channel_idx_valid failed, (channel[%d], idx[%d])",
                                 puc_ie[MAC_IE_HDR_LEN], uc_ch_idx);
                return ul_ret;
            }

            *puc_chan_idx = uc_ch_idx;

            return OAL_SUCC;
        }
    }

    *puc_chan_idx = uc_curr_chan_idx;

    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_ap_wait_start_rx_obss_beacon(mac_device_stru *pst_mac_device,
                                                      mac_vap_stru *pst_mac_vap,
                                                      oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    oal_uint8 *puc_payload;
    oal_uint16 us_payload_len;
    oal_uint8 uc_chan_idx = 0xFF;
    oal_uint8 uc_curr_chan_idx = pst_mac_device->uc_ap_chan_idx;
    mac_sec_ch_off_enum_uint8 en_sec_ch_offset = MAC_SCN;
    oal_uint32 ul_ret;

    /* ��ȡ֡�峤�� */
    us_payload_len = pst_rx_ctrl->st_rx_info.us_frame_len - pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* ��ȡ֡��ָ�� */
    puc_payload =
        (oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr + pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* ��֡���л�ȡ�ŵ������ʹ��ŵ�ƫ���� */
    ul_ret = hmac_ap_get_chan_idx_of_network(pst_mac_vap, puc_payload, us_payload_len, uc_curr_chan_idx,
                                             &uc_chan_idx, &en_sec_ch_offset);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                         "{hmac_ap_wait_start_rx_obss_beacon::hmac_ap_get_chan_idx_of_network failed[%d].}", ul_ret);
        return;
    }

    if (uc_chan_idx != uc_curr_chan_idx) {
        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                      "{hmac_ap_wait_start_rx_obss_beacon::The channel of this BSS not matched to current channel.}");
        return;
    }

    hmac_ap_store_network(pst_mac_device, pst_netbuf);

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
    if (hmac_ap_update_2040_chan_info(pst_mac_device, pst_mac_vap, puc_payload, us_payload_len,
                                      uc_chan_idx, en_sec_ch_offset) == OAL_FALSE) {
        /* do nothing */
    }
#endif
}


oal_uint32 hmac_ap_wait_start_rx_mgmt(hmac_vap_stru *pst_hmac_vap, oal_void *p_param)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    oal_uint8 uc_mgmt_frm_type;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (p_param == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_RX,
                       "{hmac_ap_wait_start_rx_mgmt::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                       "{hmac_ap_wait_start_rx_mgmt::pst_mac_device null.}");
        return OAL_FALSE;
    }

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);

    /* �ɼ��յ����κ�֡��������BSSID */
    /* ��ȡ����֡���� */
    uc_mgmt_frm_type = mac_get_frame_sub_type((oal_uint8 *)pst_rx_ctrl->st_rx_info.pul_mac_hdr_start_addr);

    /* AP��WAIT START״̬�� ���յ��ĸ��ֹ���֡���� */
    switch (uc_mgmt_frm_type) {
        case WLAN_FC0_SUBTYPE_BEACON:
        case WLAN_FC0_SUBTYPE_PROBE_RSP:
            hmac_ap_wait_start_rx_obss_beacon(pst_mac_device, &(pst_hmac_vap->st_vap_base_info),
                                              pst_mgmt_rx_event->pst_netbuf);
            break;
        default:
            break;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_mgmt_timeout_ap(oal_void *p_param)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user;
    oal_uint32 ul_ret;

    pst_hmac_user = (hmac_user_stru *)p_param;
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_AUTH, "{hmac_mgmt_timeout_ap::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_mgmt_timeout_ap::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                     "{hmac_mgmt_timeout_ap::Wait AUTH timeout!! After %d ms.}", WLAN_AUTH_TIMEOUT);
#ifdef _PRE_WLAN_1102A_CHR
    CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT, MAC_AP_AUTH_RSP_TIMEOUT);
#endif
    /* ����ȥ����֡��Ϣ��STA */
    hmac_mgmt_send_deauth_frame(&pst_hmac_vap->st_vap_base_info, pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                MAC_AUTH_NOT_VALID, OAL_FALSE);

    ul_ret = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_mgmt_timeout_ap::hmac_user_del failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}


oal_uint32 hmac_ap_wait_start_misc(hmac_vap_stru *pst_hmac_vap, oal_void *p_param)
{
    hmac_misc_input_stru *pst_misc_input = OAL_PTR_NULL;

    if (OAL_UNLIKELY((pst_hmac_vap == OAL_PTR_NULL) || (p_param == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_RX,
                       "{hmac_ap_wait_start_misc::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_misc_input = (hmac_misc_input_stru *)p_param;

    switch (pst_misc_input->en_type) {
        case HMAC_MISC_RADAR:
            break;
        default:
            break;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_ap_up_misc(hmac_vap_stru *pst_hmac_vap, oal_void *p_param)
{
    hmac_misc_input_stru *pst_misc_input = OAL_PTR_NULL;

    if (OAL_UNLIKELY((pst_hmac_vap == OAL_PTR_NULL) || (p_param == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_RX,
                       "{hmac_ap_up_misc::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_misc_input = (hmac_misc_input_stru *)p_param;

    switch (pst_misc_input->en_type) {
        case HMAC_MISC_RADAR:
            break;
        default:
            break;
    }

    return OAL_SUCC;
}

oal_uint32 hmac_ap_clean_bss(hmac_vap_stru *pst_hmac_vap)
{
    oal_dlist_head_stru *pst_user_list_head = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_user_tmp = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user_tmp = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_is_protected;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_RX, "{hmac_ap_clean_bss::hmac vap is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ɾ��vap�������ѹ����û�����֪ͨ�ں� */
    pst_user_list_head = &pst_hmac_vap->st_vap_base_info.st_mac_user_list_head;
    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    for (pst_entry = pst_user_list_head->pst_next; pst_entry != pst_user_list_head;) {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        /* ָ��˫��������һ�� */
        pst_entry = pst_entry->pst_next;

        /* ����֡�����Ƿ��� */
        en_is_protected = pst_user_tmp->st_cap_info.bit_pmf_active;

        /* ��ȥ����֡ */
        hmac_mgmt_send_disassoc_frame(pst_mac_vap, pst_user_tmp->auc_user_mac_addr, MAC_DISAS_LV_SS, en_is_protected);

        /* ֪ͨ�ں� */
        hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user_tmp);

        /* ɾ���û� */
        hmac_user_del(pst_mac_vap, pst_hmac_user_tmp);
    }

    return OAL_SUCC;
}


