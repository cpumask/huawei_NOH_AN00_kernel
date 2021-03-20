

/* 1 ͷ�ļ����� */
#include "oal_cfg80211.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
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
#include "hmac_smps.h"
#include "hmac_fsm.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "hmac_blacklist.h"
#include "hmac_dfs.h"
#include "hmac_p2p.h"
#include "hmac_blockack.h"
#include "hmac_ext_if.h"
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
#include "hmac_opmode.h"
#endif
#ifdef _PRE_WLAN_FEATURE_SMPS
#include "hmac_smps.h"
#endif
#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "hmac_wmmac.h"
#endif

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
#include "hmac_wds.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif

#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_AP_C

/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

void hmac_handle_disconnect_rsp_ap(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_device_stru *pst_mac_device;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_handle_disconnect_rsp_ap::pst_mac_device null.}");
        return;
    }

    if (pst_hmac_user->en_report_kernel == OAL_FALSE) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "hmac_handle_disconnect_rsp_ap::do not report kernel");
        return;
    }

    /* ��ɨ������¼���WAL */
    pst_event_mem = frw_event_alloc_m(WLAN_MAC_ADDR_LEN);
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_handle_disconnect_rsp_ap::pst_event_mem null.}");
        return;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_DISCONNECT_AP,
                       WLAN_MAC_ADDR_LEN,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* ȥ������STA mac��ַ */
    if (memcpy_s(frw_get_event_payload(pst_event_mem), WLAN_MAC_ADDR_LEN,
                 (uint8_t *)pst_hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ASSOC, "hmac_handle_disconnect_rsp_ap::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
}


void hmac_handle_connect_rsp_ap(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_device_stru *pst_mac_device;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_asoc_user_req_ie_stru *pst_asoc_user_req_info = OAL_PTR_NULL;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_handle_connect_rsp_ap::pst_mac_device null.}");
        return;
    }

    /* �׹���һ���µ�sta����¼���WAL */
    pst_event_mem = frw_event_alloc_m(WLAN_MAC_ADDR_LEN);
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_handle_connect_rsp_ap::pst_event_mem null.}");
        return;
    }

    /* ��Ǹ�user�����ɹ��������ϱ��ں� */
    pst_hmac_user->en_report_kernel = OAL_TRUE;

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_STA_CONNECT_AP,
                       WLAN_MAC_ADDR_LEN,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    pst_asoc_user_req_info = (hmac_asoc_user_req_ie_stru *)(pst_event->auc_event_data);

    /* �ϱ��ں˵Ĺ���sta���͵Ĺ�������֡ie��Ϣ */
    pst_asoc_user_req_info->puc_assoc_req_ie_buff = pst_hmac_user->puc_assoc_req_ie_buff;
    pst_asoc_user_req_info->ul_assoc_req_ie_len = pst_hmac_user->ul_assoc_req_ie_len;

    /* ������STA mac��ַ */
    memcpy_s((uint8_t *)pst_asoc_user_req_info->auc_user_mac_addr, WLAN_MAC_ADDR_LEN,
             pst_hmac_user->st_user_base_info.auc_user_mac_addr, WLAN_MAC_ADDR_LEN);

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
}


void hmac_mgmt_update_auth_mib(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_auth_rsp)
{
    uint16_t us_status;
    uint8_t auc_addr1[6] = { 0 };
    uint8_t *puc_mac_header = oal_netbuf_header(pst_auth_rsp);

    us_status = mac_get_auth_status(puc_mac_header);

    mac_get_addr1(puc_mac_header, auc_addr1, OAL_SIZEOF(auc_addr1));

    if (us_status != MAC_SUCCESSFUL_STATUSCODE) {
        /* DEBUG */
        oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                      "{hmac_mgmt_update_auth_mib::Authentication of STA Failed.Status Code=%d.}", us_status);
    }
}
#ifdef _PRE_WLAN_FEATURE_SAE
OAL_STATIC void hmac_ap_set_user_auth_alg_num(uint16_t us_user_index, uint16_t us_auth_alg_num)
{
    mac_user_stru *pst_mac_user;

    pst_mac_user = mac_res_get_mac_user(us_user_index);
    if (pst_mac_user == OAL_PTR_NULL) {
        return;
    }
    pst_mac_user->us_auth_alg_num = us_auth_alg_num;

    oam_warning_log1(0, OAM_SF_AUTH, "{hmac_ap_set_user_auth_alg_num::\
                us_auth_alg_num = [%d]!}", pst_mac_user->us_auth_alg_num);
}


void hmac_ap_up_rx_auth_req_to_host(hmac_vap_stru *pst_hmac_vap,
                                    oal_netbuf_stru *pst_auth_req, uint8_t us_auth_alg_num)
{
    uint8_t auc_addr2[ETHER_ADDR_LEN] = { 0 };
    uint8_t uc_is_seq1;
    uint16_t us_auth_seq;
    uint16_t us_user_index = 0xffff;
    uint8_t uc_auth_resend = 0;
    uint32_t ul_ret;

    hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_auth_req);
    /* ��ȡSTA�ĵ�ַ */
    mac_get_address2(oal_netbuf_header(pst_auth_req), auc_addr2, ETHER_ADDR_LEN);
    if (mac_addr_is_zero(auc_addr2)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_ap_rx_auth_req::user mac:%02X:XX:XX:%02X:%02X:%02X is all 0 and invaild!}",
                         auc_addr2[0], auc_addr2[3], auc_addr2[4], auc_addr2[5]);
        return;
    }

    /* ����auth transaction number */
    us_auth_seq = mac_get_auth_seq_num(oal_netbuf_header(pst_auth_req));
    if (us_auth_seq > HMAC_AP_AUTH_SEQ3_WEP_COMPLETE) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_ap_rx_auth_req::auth recieve invalid seq, auth seq [%d]}",
                         us_auth_seq);
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
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                             "{hmac_ft_ap_up_rx_auth_req::hmac_ap_get_user_idx fail, users exceed config spec!}");
        } else {
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                             "{hmac_ft_ap_up_rx_auth_req::hmac_ap_get_user_idx Err!}");
        }
    }

    hmac_ap_set_user_auth_alg_num(us_user_index, us_auth_alg_num);

    return;
}
#endif /* _PRE_WLAN_FEATURE_SAE */

OAL_STATIC uint32_t hmac_ap_rx_auth_sae_handle(hmac_vap_stru *pst_hmac_vap,
                                               oal_netbuf_stru *pst_auth_req, uint16_t us_auth_alg_num)
{
#if defined(_PRE_WLAN_FEATURE_SAE)
    /* �����FT/SAE��֤�㷨���ϱ�hostapd */
    if (us_auth_alg_num == WLAN_MIB_AUTH_ALG_FT || us_auth_alg_num == WLAN_MIB_AUTH_ALG_SAE) {
        hmac_ap_up_rx_auth_req_to_host(pst_hmac_vap, pst_auth_req, us_auth_alg_num);
        return OAL_SUCC;
    } else if (us_auth_alg_num == WLAN_MIB_AUTH_ALG_OPEN_SYS &&
               g_st_mac_device_custom_cfg.bit_forbit_open_auth) {
        oam_warning_log0(0, OAM_SF_CONN, "{hmac_ap_rx_auth_sae_handle::forbid open user to auth}");
        return OAL_SUCC;
    }
#endif
    return OAL_FAIL;
}


OAL_STATIC void hmac_ap_rx_auth_req(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_auth_req)
{
    oal_netbuf_stru *pst_auth_rsp = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t auc_chtxt[WLAN_CHTXT_SIZE] = { 0 };
    uint16_t us_auth_rsp_len;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t ul_pedding_data = 0;
    uint16_t us_auth_alg_num;
    uint8_t uc_vap_id;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_auth_req)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_auth_req);
        return;
    }

    uc_vap_id = pst_hmac_vap->st_vap_base_info.uc_vap_id;
    us_auth_alg_num = mac_get_auth_algo_num(pst_auth_req);

    oam_warning_log1(uc_vap_id, OAM_SF_CONN, "{hmac_ap_rx_auth_req::rx_auth_alg_num=[%d]}",
                     us_auth_alg_num);
    if (hmac_ap_rx_auth_sae_handle(pst_hmac_vap, pst_auth_req, us_auth_alg_num) == OAL_SUCC) {
        return;
    }

    if (us_auth_alg_num == WLAN_WITP_AUTH_SHARED_KEY) {
        /* ��ȡchallenge text */
        /* Ӳ���Ĵ�����ȡ�漴byte,����WEP SHARED���� */
        oal_get_random_bytes((int8_t *)&auc_chtxt, WLAN_CHTXT_SIZE);
    }

    /* AP���յ�STA��������֤����֡����Ӧ����֤��Ӧ֡ */
    pst_auth_rsp = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                           WLAN_MEM_NETBUF_SIZE2,
                                                           OAL_NETBUF_PRIORITY_MID);
    if (pst_auth_rsp == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::pst_auth_rsp null.}");
        return;
    }

    oal_mem_netbuf_trace(pst_auth_rsp, OAL_TRUE);

    memset_s(oal_netbuf_cb(pst_auth_rsp), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    us_auth_rsp_len = hmac_encap_auth_rsp(&pst_hmac_vap->st_vap_base_info,
                                          pst_auth_rsp,
                                          pst_auth_req,
                                          auc_chtxt,
                                          sizeof(auc_chtxt));
    if (us_auth_rsp_len == 0) {
        oal_netbuf_free(pst_auth_rsp);
        oam_error_log0(uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::us_auth_rsp_len is 0.}");
        return;
    }

    oal_netbuf_put(pst_auth_rsp, us_auth_rsp_len);

    hmac_mgmt_update_auth_mib(pst_hmac_vap, pst_auth_rsp);

    /* ��ȡcb�ֶ� */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_auth_rsp);
    /* ������֤��Ӧ֮֡ǰ�����û��Ľ���״̬��λ */
    /* hmac_encap_auth_rsp��user id���ֶ�ֵ�Ƿ���������֡ʧ��, ֱ�ӻظ�ʧ�ܵ���֤��Ӧ֡ */
    if (MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) == g_wlan_spec_cfg->invalid_user_id) {
        oam_warning_log1(uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::user[%d] auth fail.}",
                         MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
    } else {
        pst_hmac_user = mac_res_get_hmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
        if (pst_hmac_user == OAL_PTR_NULL) {
            oal_netbuf_free(pst_auth_rsp);
            oam_error_log0(uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::hmac_user null.}");
            return;
        }

        hmac_mgmt_reset_psm(&pst_hmac_vap->st_vap_base_info, MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
    }

    hmac_config_scan_abort(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(uint32_t), (uint8_t *)&ul_pedding_data);

    /* ������֤�ɹ�����ʧ�ܣ������¼���dmac������֤֡ */
    ul_ret = hmac_tx_mgmt_send_event(&pst_hmac_vap->st_vap_base_info, pst_auth_rsp, us_auth_rsp_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_auth_rsp);
        oam_warning_log1(uc_vap_id, OAM_SF_AUTH, "{hmac_ap_rx_auth_req::ul_ret=[%d].}", ul_ret);
    }
}


OAL_STATIC uint32_t hmac_ap_rx_deauth_req(hmac_vap_stru *pst_hmac_vap,
                                          uint8_t *puc_mac_hdr, oal_bool_enum_uint8 en_is_protected)
{
    uint8_t *puc_sa = OAL_PTR_NULL;
    uint8_t *puc_da = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint16_t us_err_code;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_hmac_vap, puc_mac_hdr)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_ap_rx_deauth_req::param null, %p %p.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)puc_mac_hdr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

    us_err_code = *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN));

    /* ���ӽ��յ�ȥ��֤֡ʱ��ά����Ϣ */
    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CONN,
                     "{hmac_ap_rx_deauth_req:: DEAUTH rx, reason code = %d, sa[XX:XX:XX:%2X:%2X:%2X]}",
                     us_err_code, puc_sa[3], puc_sa[4], puc_sa[5]);

    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, puc_sa);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{aput rx deauth frame, pst_hmac_user null.}");
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
    if ((ether_is_multicast(puc_da) != OAL_TRUE) &&
        (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active)) {
        oam_error_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_ap_rx_deauth_req::PMF check failed %d %d.}",
                       en_is_protected, pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
        return OAL_FAIL;
    }
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                         CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_PASSIVE_DISCONNECT, us_err_code);
    /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
    hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

    hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    return OAL_SUCC;
}

OAL_STATIC void hmac_user_sort_op_rates(hmac_user_stru *pst_hmac_user)
{
    uint8_t uc_loop;
    uint8_t uc_num_rates;
    uint8_t uc_min_rate;
    uint8_t uc_temp_rate; /* ��ʱ���ʣ��������ݽ��� */
    uint8_t uc_index;
    uint8_t uc_temp_index; /* ��ʱ�������������ݽ��� */

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

    /*******************************************************************
      ����11gģʽ�Ŀɲ������ʣ�ʹ11b���ʶ��ۼ���11a֮ǰ
      802.11a ����:6��9��12��18��24��36��48��54Mbps
      802.11b ����:1��2��5.5��11Mbps
      ���ڰ���С���������802.11b�е�����11Mbps��802.11a�У��±�Ϊ5
      ���Դӵ���λ���м��鲢����
    *******************************************************************/
    if (pst_hmac_user->st_op_rates.uc_rs_nrates == MAC_DATARATES_PHY_80211G_NUM) { /* 11g_compatibility mode */
        if ((pst_hmac_user->st_op_rates.auc_rs_rates[5] & 0x7F) == 0x16) { /* 11Mbps */
            uc_temp_rate = pst_hmac_user->st_op_rates.auc_rs_rates[5];
            pst_hmac_user->st_op_rates.auc_rs_rates[5] = pst_hmac_user->st_op_rates.auc_rs_rates[4];
            pst_hmac_user->st_op_rates.auc_rs_rates[4] = pst_hmac_user->st_op_rates.auc_rs_rates[3];
            pst_hmac_user->st_op_rates.auc_rs_rates[3] = uc_temp_rate;
        }
    }
}


OAL_STATIC oal_bool_enum_uint8 hmac_ap_up_update_sta_cap_info(hmac_vap_stru *pst_hmac_vap,
                                                              uint16_t us_cap_info,
                                                              hmac_user_stru *pst_hmac_user,
                                                              mac_status_code_enum_uint16 *pen_status_code)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_cap_info_stru *pst_cap_info = (mac_cap_info_stru *)(&us_cap_info);

    if (oal_any_null_ptr2(pst_hmac_vap, pst_hmac_user)) {
        oam_error_log3(0, OAM_SF_ANY, "{hmac_ap_up_update_sta_cap_info::param null, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pen_status_code);
        *pen_status_code = MAC_UNSPEC_FAIL;
        return OAL_FALSE;
    }
    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* check bss capability info MAC,����MAC������ƥ���STA */
    ul_ret = hmac_check_bss_cap_info(us_cap_info, pst_mac_vap);
    if (ul_ret != OAL_TRUE) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
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


OAL_STATIC uint16_t hmac_check_wpa_cipher_ap(mac_vap_stru *pst_mac_vap, uint8_t *puc_ie)
{
    mac_crypto_settings_stru st_crypto;

    if (mac_ie_get_wpa_cipher(puc_ie, &st_crypto) != OAL_SUCC) {
        return MAC_INVALID_INFO_ELMNT;
    }

    if (mac_mib_get_wpa_group_suite(pst_mac_vap) != st_crypto.ul_group_suite) {
        return MAC_INVALID_GRP_CIPHER;
    }

    if (mac_mib_wpa_pair_match_suites_s(pst_mac_vap, st_crypto.aul_pair_suite, sizeof(st_crypto.aul_pair_suite)) == 0) {
        return MAC_INVALID_PW_CIPHER;
    }

    if (mac_mib_wpa_akm_match_suites_s(pst_mac_vap, st_crypto.aul_akm_suite, sizeof(st_crypto.aul_akm_suite)) == 0) {
        return MAC_INVALID_AKMP_CIPHER;
    }

    return MAC_SUCCESSFUL_STATUSCODE;
}

OAL_STATIC uint16_t hmac_check_rsn_cipher_ap(mac_vap_stru *pst_mac_vap,
                                             mac_user_stru *pst_mac_user, uint8_t *puc_ie)
{
    mac_crypto_settings_stru st_crypto;
    uint16_t us_rsn_cap;

    if (mac_ie_get_rsn_cipher(puc_ie, &st_crypto) != OAL_SUCC) {
        return MAC_INVALID_INFO_ELMNT;
    }

    if (mac_mib_get_rsn_group_suite(pst_mac_vap) != st_crypto.ul_group_suite) {
        return MAC_INVALID_GRP_CIPHER;
    }

    if (mac_mib_rsn_pair_match_suites_s(pst_mac_vap, st_crypto.aul_pair_suite, sizeof(st_crypto.aul_pair_suite)) == 0) {
        return MAC_INVALID_PW_CIPHER;
    }

    if (mac_mib_rsn_akm_match_suites_s(pst_mac_vap, st_crypto.aul_akm_suite, sizeof(st_crypto.aul_akm_suite)) == 0) {
        return MAC_INVALID_AKMP_CIPHER;
    }

    us_rsn_cap = mac_get_rsn_capability(puc_ie);
    /* Ԥ��֤������� */
    if ((us_rsn_cap & BIT0) != 0) {
        return MAC_INVALID_RSN_INFO_CAP;
    }

    /* sae auth �ɹ�����user ��֧��pmf,����������,����֤����ɾ��ά�� */
    oam_warning_log2(0, OAM_SF_ASSOC, "{hmac_check_rsn_cipher_ap::us_auth_alg_num=[%d],us_rsn_cap=[%X].}",
                     pst_mac_user->us_auth_alg_num, us_rsn_cap);
    if (pst_mac_user->us_auth_alg_num == WLAN_MIB_AUTH_ALG_SAE && (!(us_rsn_cap & BIT7))) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_check_rsn_cipher_ap::WLAN_MIB_AUTH_ALG_SAE MFP disable.}");
        return MAC_R0KH_UNREACHABLE;
    }

    /* ����ǿ�ƣ��Զ�û��MFP���� */
    if ((mac_mib_get_dot11RSNAMFPR(pst_mac_vap) == OAL_TRUE) && (!(us_rsn_cap & BIT7))) {
        return MAC_MFP_VIOLATION;
    }
    /* �Զ�ǿ�ƣ�����û��MFP���� */
    if ((mac_mib_get_dot11RSNAMFPC(pst_mac_vap) == OAL_FALSE) && (us_rsn_cap & BIT6)) {
        return MAC_MFP_VIOLATION;
    }

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    if ((mac_mib_get_dot11RSNAMFPC(pst_mac_vap) == OAL_TRUE) && (us_rsn_cap & BIT7)) {
        mac_user_set_pmf_active(pst_mac_user, OAL_TRUE);
    }
#endif

    return MAC_SUCCESSFUL_STATUSCODE;
}


OAL_STATIC uint16_t hmac_check_rsn_ap(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user,
                                      uint8_t *puc_payload, uint32_t ul_msg_len)
{
    uint8_t *puc_rsn_ie = OAL_PTR_NULL;
    uint8_t *puc_wpa_ie = OAL_PTR_NULL;

    /* ������û��rsn����,���Լ��ie�������Ӽ����� */
    if (mac_mib_get_rsnaactivated(pst_mac_vap) == OAL_FALSE) {
        return MAC_SUCCESSFUL_STATUSCODE;
    }

    /* ��ȡRSN��WPA IE��Ϣ */
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, puc_payload, (int32_t)ul_msg_len);
    puc_wpa_ie = mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_OUITYPE_WPA, puc_payload, (int32_t)ul_msg_len);
    if ((puc_rsn_ie == OAL_PTR_NULL) && (puc_wpa_ie == OAL_PTR_NULL)) {
        if (mac_mib_get_WPSActive(pst_mac_vap) == OAL_TRUE) {
            return MAC_SUCCESSFUL_STATUSCODE;
        } else {
            return MAC_INVALID_INFO_ELMNT;
        }
    }

    if ((pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_TRUE) && (puc_rsn_ie != OAL_PTR_NULL)) {
        return hmac_check_rsn_cipher_ap(pst_mac_vap, pst_mac_user, puc_rsn_ie);
    }

    if ((pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE) && (puc_wpa_ie != OAL_PTR_NULL)) {
        return hmac_check_wpa_cipher_ap(pst_mac_vap, puc_wpa_ie);
    }

    return MAC_CIPHER_REJ;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_is_erp_sta(hmac_user_stru *pst_hmac_user)
{
    uint32_t ul_loop;
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


OAL_STATIC uint32_t hmac_ap_up_update_legacy_capability(hmac_vap_stru *pst_hmac_vap,
                                                        hmac_user_stru *pst_hmac_user, uint16_t us_cap_info)
{
    mac_protection_stru *pst_protection = &(pst_hmac_vap->st_vap_base_info.st_protection);
    mac_user_stru *pst_mac_user = &(pst_hmac_user->st_user_base_info);
    oal_bool_enum_uint8 en_is_erp_sta;

    /* ���STA��֧��short slot */
    if ((us_cap_info & MAC_CAP_SHORT_SLOT) != MAC_CAP_SHORT_SLOT) {
        /* ���STA֮ǰû�й����� ����֮ǰ��֧��short slotվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC)
            || (pst_hmac_user->st_hmac_cap_info.bit_short_slot_time == OAL_TRUE)) {
            pst_protection->uc_sta_no_short_slot_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_slot_time = OAL_FALSE;
    } else { /* ���STA֧��short slot */
        /* ���STA�Բ�֧��short slotվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC)
            && (pst_hmac_user->st_hmac_cap_info.bit_short_slot_time == OAL_FALSE)
            && (pst_protection->uc_sta_no_short_slot_num != 0)) {
            pst_protection->uc_sta_no_short_slot_num--;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_slot_time = OAL_TRUE;
    }

    pst_hmac_user->st_user_stats_flag.bit_no_short_slot_stats_flag = OAL_TRUE;

    /* ���STA��֧��short preamble */
    if ((us_cap_info & MAC_CAP_SHORT_PREAMBLE) != MAC_CAP_SHORT_PREAMBLE) {
        /* ���STA֮ǰû�й����� ����֮ǰ��֧��short preambleվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC)
            || (pst_hmac_user->st_hmac_cap_info.bit_short_preamble == OAL_TRUE)) {
            pst_protection->uc_sta_no_short_preamble_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_short_preamble = OAL_FALSE;
    } else { /* ���STA֧��short preamble */
        /* ���STA֮ǰ�Բ�֧��short preambleվ����ݹ�������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC)
            && (pst_hmac_user->st_hmac_cap_info.bit_short_preamble == OAL_FALSE)
            && (pst_protection->uc_sta_no_short_preamble_num != 0)) {
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
        if ((pst_mac_user->en_user_asoc_state != MAC_USER_STATE_ASSOC)
            || (pst_hmac_user->st_hmac_cap_info.bit_erp == OAL_TRUE)) {
            pst_protection->uc_sta_non_erp_num++;
        }

        pst_hmac_user->st_hmac_cap_info.bit_erp = OAL_FALSE;
    } else { /* ���STA֧��ERP */
        /* ���STA֮ǰ�Բ�֧��ERP���վ���������Ҫupdate���� */
        if ((pst_mac_user->en_user_asoc_state == MAC_USER_STATE_ASSOC)
            && (pst_hmac_user->st_hmac_cap_info.bit_erp == OAL_FALSE)
            && (pst_protection->uc_sta_non_erp_num != 0)) {
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

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_ap_up_update_asoc_entry_prot(uint8_t *puc_payload,
                                                      uint8_t uc_sub_type, uint32_t ul_msg_len,
                                                      hmac_user_stru *pst_hmac_user)
{
    /* WMM */
    hmac_uapsd_update_user_para(puc_payload, uc_sub_type, ul_msg_len, pst_hmac_user);

    return OAL_SUCC;
}



oal_bool_enum hmac_go_is_auth(mac_vap_stru *pst_mac_vap)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_dlist_tmp = OAL_PTR_NULL;
    mac_user_stru *pst_user_tmp = OAL_PTR_NULL;

    if (pst_mac_vap->en_p2p_mode != WLAN_P2P_GO_MODE) {
        return OAL_FALSE;
    }

    oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_ap_is_auth::pst_user_tmp null.}");
            continue;
        }
        if ((pst_user_tmp->en_user_asoc_state == MAC_USER_STATE_AUTH_COMPLETE)
            || (pst_user_tmp->en_user_asoc_state == MAC_USER_STATE_AUTH_KEY_SEQ1)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


void hmac_ap_check_owe_capa(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
                            uint8_t *puc_payload, uint32_t ul_payload_len,
                            mac_status_code_enum_uint16 *pen_status_code)
{
    uint32_t ul_ret;
    uint8_t *puc_rsn_ie = OAL_PTR_NULL;
    uint8_t *puc_owe_dh = OAL_PTR_NULL;
    mac_crypto_settings_stru st_crypto = { 0 };
    uint16_t us_suite_idx;
    uint16_t owe_group;

    pst_hmac_user->st_hmac_cap_info.bit_owe = OAL_FALSE;

    /* ���Ȳ��� RSN ��ϢԪ�� */
    puc_rsn_ie = mac_find_ie(MAC_EID_RSN, (uint8_t *)puc_payload, (int32_t)ul_payload_len);
    if (puc_rsn_ie == OAL_PTR_NULL) {
        return;
    }

    /* 1. check OWE AKM */
    ul_ret = mac_ie_get_rsn_cipher(puc_rsn_ie, &st_crypto);
    if (ul_ret != OAL_SUCC) {
        return;
    }

    /* AKM Suite */
    for (us_suite_idx = 0; us_suite_idx < WLAN_AUTHENTICATION_SUITES; us_suite_idx++) {
        if (st_crypto.aul_akm_suite[us_suite_idx] == MAC_RSN_AKM_OWE) {
            pst_hmac_user->st_hmac_cap_info.bit_owe = OAL_TRUE;
            break;
        }
    }
    if (pst_hmac_user->st_hmac_cap_info.bit_owe == OAL_FALSE) {
        return;
    }

    oam_warning_log3(0, OAM_SF_WPA,
                     "hmac_ap_check_owe_capa::get_rsn_cipher owe:%d, akm_suite[0]:0x%x akm_suite[1]:0x%x",
                     pst_hmac_user->st_hmac_cap_info.bit_owe,
                     st_crypto.aul_akm_suite[0], st_crypto.aul_akm_suite[1]);

    /* 2. check OWE Diffie-Hellman Parameter Element */
    /*************************************************************************/
    /*             OWE Diffie-Hellman Parameter element                      */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /* |Element ID | Length | Element ID |  element-specific data            */
    /* |           |        | Extension  |  group        | public key        */
    /* --------------------------------------------------------------------- */
    /* |     1     |    1   |    1       |      2        |                   */
    /* --------------------------------------------------------------------- */
    /* |    255    |variable|    32      |   19/20/21    |                   */
    /* --------------------------------------------------------------------- */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    puc_owe_dh = mac_find_ie_ext_ie(MAC_EID_EXTENSION, MAC_EID_EXT_OWE_DH_PARAM,
                                    puc_payload, (int32_t)ul_payload_len);
    /* owe dh ie len����3�ֽڲ����� */
    if ((puc_owe_dh == OAL_PTR_NULL) || (puc_owe_dh[1] < MAC_EXT_OWE_DH_PARAM_LEN)) {
        pst_hmac_user->st_hmac_cap_info.bit_owe = OAL_FALSE;
        return;
    }

    puc_owe_dh += MAC_IE_HDR_LEN; /* skip EID and length */
    owe_group = *(puc_owe_dh + 1);
    owe_group += (*(puc_owe_dh + 2)) << 8; /*  �ֽ�2����8λ */
    if ((owe_group == 0) || ((pst_hmac_vap->owe_group[0] != owe_group) && (pst_hmac_vap->owe_group[1] != owe_group) &&
                        (pst_hmac_vap->owe_group[2] != owe_group))) { /* group 1 2 */
        *pen_status_code = MAC_FINITE_CYCLIC_GROUP_NOT_SUPPORTED; /* OWE group not matched */
    }

    oam_warning_log4(0, OAM_SF_WPA,
                     "hmac_ap_check_owe_capa::vap owe_group_cap=%d, associated STA bit_owe=%d owe_group:%d, status:%d",
                     pst_hmac_vap->owe_group_cap, pst_hmac_user->st_hmac_cap_info.bit_owe,
                     owe_group, *pen_status_code);

    return;
}

OAL_STATIC OAL_INLINE void hmac_ap_2g_11ac_user_asoc(hmac_vap_stru *pst_hmac_vap,
                                                     uint8_t *puc_payload, uint32_t ul_msg_len,
                                                     hmac_user_stru *pst_hmac_user)
{
    uint8_t *puc_vendor_vht_ie;
    uint32_t ul_vendor_vht_ie_offset = MAC_WLAN_OUI_VENDOR_VHT_HEADER + MAC_IE_HDR_LEN;
    uint8_t *puc_ie_tmp = OAL_PTR_NULL;

    puc_vendor_vht_ie = mac_find_vendor_ie(MAC_WLAN_OUI_BROADCOM_EPIGRAM,
                                           MAC_WLAN_OUI_VENDOR_VHT_TYPE,
                                           puc_payload, ul_msg_len);
    if ((puc_vendor_vht_ie != OAL_PTR_NULL) && (puc_vendor_vht_ie[1] >= MAC_WLAN_OUI_VENDOR_VHT_HEADER)) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::find broadcom/epigram vendor ie, enable hidden bit_11ac2g}");
        /* ����˺�������user֧��2G 11ac */
        puc_ie_tmp = mac_find_ie(MAC_EID_VHT_CAP,
                                 puc_vendor_vht_ie + ul_vendor_vht_ie_offset,
                                 (int32_t)(puc_vendor_vht_ie[1] - MAC_WLAN_OUI_VENDOR_VHT_HEADER));
        if (puc_ie_tmp != OAL_PTR_NULL) {
            pst_hmac_user->en_user_vendor_vht_capable = OAL_TRUE;
            hmac_proc_vht_cap_ie(&pst_hmac_vap->st_vap_base_info, pst_hmac_user, puc_ie_tmp);
        } else {
            pst_hmac_user->en_user_vendor_novht_capable = OAL_TRUE;  // ��ʾ֧��5G 20M mcs9
        }
    }
}

OAL_STATIC oal_bool_enum hmac_ap_frame_len_check(uint8_t *puc_mac_hdr, uint32_t ul_msg_len)
{
    uint32_t ul_len_min;

    ul_len_min = ((WLAN_FC0_SUBTYPE_REASSOC_REQ | WLAN_FC0_TYPE_MGT) == mac_get_frame_type_and_subtype(puc_mac_hdr)) ?
                 (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN + WLAN_MAC_ADDR_LEN) : (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN);

    return ((ul_msg_len > ul_len_min) ? OAL_TRUE : OAL_FALSE);
}

#ifdef _PRE_WLAN_FEATURE_11AC2G
OAL_STATIC void hmac_ap_update_avail_protocol_mode(mac_vap_stru *pst_mac_vap,
                                                   mac_user_stru *pst_mac_user, uint8_t *puc_avail_mode)
{
    if ((pst_mac_vap->en_protocol == WLAN_HT_MODE) && (pst_mac_user->en_protocol_mode == WLAN_VHT_MODE) &&
        (pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_TRUE) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G)) {
        *puc_avail_mode = WLAN_VHT_MODE;
    }
}
#endif

OAL_STATIC uint32_t hmac_ap_up_update_sta_user_cmp_ssid(mac_vap_stru *pst_mac_vap,
                                                        uint8_t *puc_ie, uint32_t ul_remain_len,
                                                        mac_status_code_enum_uint16 *pen_status_code)
{
    if ((puc_ie != OAL_PTR_NULL) && (ul_remain_len > 2)) { // ��Ϣbufferʣ�೤�ȳ���2
        if ((puc_ie[1] != (uint8_t)OAL_STRLEN((int8_t *)mac_mib_get_DesiredSSID(pst_mac_vap))) ||
            (oal_memcmp(&puc_ie[2], mac_mib_get_DesiredSSID(pst_mac_vap), puc_ie[1]) != 0)) {
            *pen_status_code = MAC_UNSPEC_FAIL;
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_ap_up_update_sta_user::ssid mismatch.}");
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_ap_up_update_sta_user(hmac_vap_stru *pst_hmac_vap,
                                               uint8_t *puc_mac_hdr, uint8_t *puc_payload, uint32_t ul_msg_len,
                                               hmac_user_stru *pst_hmac_user,
                                               mac_status_code_enum_uint16 *pen_status_code)
{
    uint16_t us_cap_info;
    mac_status_code_enum_uint16 us_ret_val;
    uint8_t *puc_ie_tmp = OAL_PTR_NULL;
    wlan_bw_cap_enum_uint8 en_bandwidth_cap = WLAN_BW_CAP_BUTT;
    wlan_bw_cap_enum_uint8 en_bwcap_ap; /* ap����������� */
    uint32_t ul_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    wlan_bw_cap_enum_uint8 en_bwcap_vap;
    uint8_t uc_avail_mode;
    uint32_t ul_remain_len = 0;

    *pen_status_code = MAC_SUCCESSFUL_STATUSCODE;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    if (hmac_ap_frame_len_check(puc_mac_hdr, ul_msg_len) != OAL_TRUE) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_ap_up_update_sta_user::len[%d] err}", ul_msg_len);
        return OAL_FAIL;
    }

    /***************************************************************************
        ���AP�Ƿ�֧�ֵ�ǰ���ڹ�����STA����������
        |ESS|IBSS|CFPollable|CFPReq|Privacy|Preamble|PBCC|Agility|Reserved|
    ***************************************************************************/
    us_cap_info = oal_make_word16(puc_payload[0], puc_payload[1]);

    ul_ret = hmac_ap_up_update_sta_cap_info(pst_hmac_vap, us_cap_info, pst_hmac_user, pen_status_code);
    if (ul_ret != OAL_TRUE) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_ap_up_update_sta_user::\
            hmac_ap_up_update_sta_cap_info failed[%d], status_code=%d.}", ul_ret, *pen_status_code);
        return ul_ret;
    }

    puc_payload += (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN);
    ul_msg_len -= (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN);
    if ((WLAN_FC0_SUBTYPE_REASSOC_REQ | WLAN_FC0_TYPE_MGT) == mac_get_frame_type_and_subtype(puc_mac_hdr)) {
        puc_payload += WLAN_MAC_ADDR_LEN;
        ul_msg_len -= WLAN_MAC_ADDR_LEN;
    }

    /* �ж�SSID,���Ȼ����ݲ�һ��ʱ,��Ϊ��SSID��һ�£����Ǽ������Ҳ���ieʱ������ */
    puc_ie_tmp = mac_find_ie_sec(MAC_EID_SSID, puc_payload, (int32_t)ul_msg_len, &ul_remain_len);
    if (hmac_ap_up_update_sta_user_cmp_ssid(pst_mac_vap, puc_ie_tmp, ul_remain_len, pen_status_code) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* ����wmm ie�Ƿ���ڻ�ȡsta��wmm���� */
    puc_ie_tmp = mac_find_ie(MAC_EID_WMM, puc_payload, (int32_t)ul_msg_len);
    pst_hmac_user->en_wmm_switch = (puc_ie_tmp != OAL_PTR_NULL) ? OAL_FALSE : OAL_TRUE;

    /* ��ǰ�û��ѹ��� */
    ul_ret = hmac_ie_proc_assoc_user_legacy_rate(puc_payload, ul_msg_len, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        *pen_status_code = MAC_UNSUP_RATE;
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::rates mismatch ret[%d].}", ul_ret);
        return ul_ret;
    }
    /* ��һ��˳�������������� */
    hmac_user_sort_op_rates(pst_hmac_user);

    /* ���¶�ӦSTA��legacyЭ������ */
    hmac_ap_up_update_legacy_capability(pst_hmac_vap, pst_hmac_user, us_cap_info);

    /* ���HT capability�Լ�Extend capability�Ƿ�ƥ�䣬�����д���  */
    us_ret_val = hmac_vap_check_ht_capabilities_ap(pst_hmac_vap, puc_payload, ul_msg_len, pst_hmac_user);
    if (us_ret_val != MAC_SUCCESSFUL_STATUSCODE) {
        *pen_status_code = us_ret_val;
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::Reject STA because of ht_capability[%d].}", us_ret_val);
        return us_ret_val;
    }

    /* ����AP�б������mib�� */
    ul_ret = hmac_user_protection_sync_data(pst_mac_vap);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::protection update mib failed, ret=%d.}", ul_ret);
    }

    /* ���¶�ӦSTA��Э������ update_asoc_entry_prot(ae, msa, rx_len, cap_info, is_p2p); */
    hmac_ap_up_update_asoc_entry_prot(puc_payload, mac_get_frame_type_and_subtype(puc_mac_hdr),
                                      ul_msg_len, pst_hmac_user);

    /* ����QoS���� */
    hmac_mgmt_update_assoc_user_qos_table(puc_payload, (uint16_t)ul_msg_len, pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_TXBF
    /* ����11n txbf���� */
    puc_ie_tmp = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_EID_11NTXBF, puc_payload, (int32_t)ul_msg_len);
    hmac_mgmt_update_11ntxbf_cap(puc_ie_tmp, pst_hmac_user);
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        hmac_hiex_rx_assoc_req(pst_hmac_vap, pst_hmac_user, puc_payload, ul_msg_len);
    }
#endif

    /* ����11ac VHT capabilities ie */
    puc_ie_tmp = mac_find_ie(MAC_EID_VHT_CAP, puc_payload, (int32_t)ul_msg_len);
    if (puc_ie_tmp != OAL_PTR_NULL) {
        hmac_proc_vht_cap_ie(pst_mac_vap, pst_hmac_user, puc_ie_tmp);
#ifdef _PRE_WLAN_FEATURE_1024QAM
        puc_ie_tmp = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_1024QAM_IE, puc_payload, ul_msg_len);
        if (puc_ie_tmp != OAL_PTR_NULL) {
            pst_hmac_user->st_user_base_info.st_cap_info.bit_1024qam_cap = OAL_TRUE;
            pst_hmac_user->st_user_base_info.st_cap_info.bit_20mmcs11_compatible_1103 = OAL_TRUE;
        }
#endif
    } else if (pst_mac_vap->en_protocol == WLAN_VHT_ONLY_MODE) {
        /* ������֧��11ac STA����11aconly ģʽ��AP */
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user:AP 11ac only, but STA not support 11ac}");
        *pen_status_code = MAC_MISMATCH_VHTCAP;
        return OAL_FAIL;
    }
    if (pst_hmac_user->st_user_base_info.st_vht_hdl.en_vht_capable == OAL_FALSE) {
        hmac_ap_2g_11ac_user_asoc(pst_hmac_vap, puc_payload, ul_msg_len, pst_hmac_user);
    }
    /* �����յ���ASOC REQ��Ϣ�е�SECURITY����.�����,�򷵻ض�Ӧ�Ĵ����� */
    mac_user_init_key(&pst_hmac_user->st_user_base_info);
    us_ret_val = hmac_check_rsn_ap(pst_mac_vap, &pst_hmac_user->st_user_base_info, puc_payload, ul_msg_len);
    if (us_ret_val != MAC_SUCCESSFUL_STATUSCODE) {
        *pen_status_code = us_ret_val;
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_check_rsn_ap fail[%d].}", us_ret_val);
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        /* ����11ax HE capabilities ie */
        puc_ie_tmp = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_CAP, puc_payload, (int32_t)ul_msg_len);
        hmac_proc_he_cap_ie(pst_mac_vap, pst_hmac_user, puc_ie_tmp);
    }
#endif
    hmac_user_cap_update_by_hisi_cap_ie(pst_hmac_user, puc_payload, ul_msg_len);

    /* ��ȡ�û���Э��ģʽ */
    hmac_set_user_protocol_mode(pst_mac_vap, pst_hmac_user);

    uc_avail_mode = g_auc_avail_protocol_mode[pst_mac_vap->en_protocol][pst_mac_user->en_protocol_mode];
    if (uc_avail_mode == WLAN_PROTOCOL_BUTT) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::user not allowed:no valid protocol:  \
                         vap mode=%d, user mode=%d, user avail mode=%d.}",
                         pst_mac_vap->en_protocol, pst_mac_user->en_protocol_mode, pst_mac_user->en_avail_protocol_mode);
        *pen_status_code = MAC_UNSUP_CAP;
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_11AC2G
    hmac_ap_update_avail_protocol_mode(pst_mac_vap, pst_mac_user, &uc_avail_mode);
#endif

    /* ��ȡ�û���VAPЭ��ģʽ���� */
    mac_user_set_avail_protocol_mode(pst_mac_user, uc_avail_mode);
    mac_user_set_cur_protocol_mode(pst_mac_user, pst_mac_user->en_avail_protocol_mode);

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_ap_up_update_sta_user::vap protocol:%d,mac user protocol mode:%d,avail protocol mode:%d}",
                     pst_mac_vap->en_protocol, pst_mac_user->en_protocol_mode, pst_mac_user->en_avail_protocol_mode);

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (pst_hmac_user->st_user_base_info.en_cur_protocol_mode == WLAN_HE_MODE) {
            pst_hmac_user->uc_tx_ba_limit = DMAC_UCAST_FRAME_TX_COMP_TIMES_HE;
        }
    }
#endif

    /* ��ȡ�û���VAP ��֧�ֵ�11a/b/g ���ʽ��� */
    hmac_vap_set_user_avail_rates(pst_mac_vap, pst_hmac_user);

    if (hmac_config_user_cap_syn(pst_mac_vap, pst_mac_user) != OAL_SUCC) {
        oam_error_log1(pst_mac_user->uc_vap_id, 0, "{hmac_ap_up_update_sta_user::usr_cap_syn fail:%d}", ul_ret);
    }

    /* ��ȡ�û���VAP�ռ������� */
    if (hmac_user_set_avail_num_space_stream(pst_mac_user, pst_mac_vap->en_vap_rx_nss) != OAL_SUCC) {
        *pen_status_code = MAC_UNSPEC_FAIL;
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_ap_up_update_sta_user::set nss fail:%d}", ul_ret);
    }

    /* ��ȡ�û��Ĵ������� */
    mac_user_get_sta_cap_bandwidth(pst_mac_user, &en_bandwidth_cap);

    /* ��ȡvap�����������û����������Ľ��� */
    mac_vap_get_bandwidth_cap(&pst_hmac_vap->st_vap_base_info, &en_bwcap_ap);
    en_bwcap_vap = oal_min(en_bwcap_ap, en_bandwidth_cap);
    mac_user_set_bandwidth_info(pst_mac_user, en_bwcap_vap, en_bwcap_vap);

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_ap_up_update_sta_user::mac_vap->bandwidth:%d, \
        mac_user->bandwidth:%d, cur_bandwidth:%d.}", en_bwcap_ap, en_bandwidth_cap, pst_mac_user->en_cur_bandwidth);

    /* ��ȡ�û�160M�����µĿռ��� */
    hmac_user_set_num_spatial_stream_160M(pst_mac_user);

    /* �����û�֧�ִ���������Э�̳���ǰ����dmac offload�ܹ��£�ͬ��������Ϣ��device */
    ul_ret = hmac_config_user_info_syn(pst_mac_vap, pst_mac_user);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_user->uc_vap_id, 0, "{hmac_ap_up_update_sta_user::usr_info_syn failed[%d].}", ul_ret);
    }

#ifdef _PRE_WLAN_FEATURE_SMPS
    /* ����smps���¿ռ������� */
    if (!IS_VAP_SINGLE_NSS(pst_mac_vap) && !IS_USER_SINGLE_NSS(pst_mac_user)) {
        hmac_smps_update_user_status(pst_mac_vap, pst_mac_user);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    /* ����Operating Mode Notification ��ϢԪ�� */
    ul_ret = hmac_check_opmode_notify(pst_hmac_vap, puc_mac_hdr, puc_payload, ul_msg_len, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_update_sta_user::hmac_check_opmode_notify failed[%d].}", ul_ret);
    }
#endif

    /* ͬ���ռ�����Ϣ */
    ul_ret = hmac_config_user_num_spatial_stream_cap_syn(pst_mac_vap, pst_mac_user);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_ap_up_update_sta_user::hmac_config_user_num_spatial_stream_cap_syn failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}


uint32_t hmac_ap_save_user_assoc_req(hmac_user_stru *pst_hmac_user, uint8_t *puc_payload,
                                     uint32_t ul_payload_len, uint8_t uc_mgmt_frm_type)
{
    uint32_t ul_ret;

    /* AP ����STA �Ĺ�������֡��Ϣ���Ա��ϱ��ں� */
    ul_ret = hmac_user_free_asoc_req_ie(pst_hmac_user->st_user_base_info.us_assoc_id);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_ap_save_user_assoc_req :: hmac_user_free_asoc_req_ie fail.}");
        return OAL_FAIL;
    }

    /* Ŀǰ11rû��ʵ�֣����Դ����ع���֡�����̺͹���֡һ����11rʵ�ֺ�˴���Ҫ�޸� */
    return hmac_user_set_asoc_req_ie(pst_hmac_user,
                                     puc_payload + MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN,
                                     ul_payload_len - MAC_CAP_INFO_LEN - MAC_LIS_INTERVAL_IE_LEN,
                                     (uint8_t)(uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ));
}


uint32_t hmac_ap_send_assoc_rsp(hmac_vap_stru *pst_hmac_vap,
                                hmac_user_stru *pst_hmac_user,
                                const unsigned char *puc_sta_addr,
                                uint8_t uc_mgmt_frm_type,
                                mac_status_code_enum_uint16 *pen_status_code)
{
    uint32_t ul_rslt;
    oal_netbuf_stru *pst_asoc_rsp;
    uint32_t ul_asoc_rsp_len;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;
    uint8_t uc_tx_mgmt_frm_type = WLAN_FC0_SUBTYPE_ASSOC_RSP;

    pst_asoc_rsp = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                           WLAN_MEM_NETBUF_SIZE2,
                                                           OAL_NETBUF_PRIORITY_MID);
    if (pst_asoc_rsp == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_ap_up_rx_asoc_req::pst_asoc_rsp null.}");
        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_asoc_rsp);
    memset_s(pst_tx_ctl, oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    oal_mem_netbuf_trace(pst_asoc_rsp, OAL_TRUE);

    /* update tx mgmt frame type according to assoc req type */
    if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_ASSOC_REQ) {
        uc_tx_mgmt_frm_type = WLAN_FC0_SUBTYPE_ASSOC_RSP;
    } else if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) {
        uc_tx_mgmt_frm_type = WLAN_FC0_SUBTYPE_REASSOC_RSP;
    }
    ul_asoc_rsp_len = hmac_mgmt_encap_asoc_rsp_ap(&(pst_hmac_vap->st_vap_base_info),
                                                  puc_sta_addr,
                                                  pst_hmac_user->st_user_base_info.us_assoc_id,
                                                  *pen_status_code,
                                                  oal_netbuf_header(pst_asoc_rsp),
                                                  uc_tx_mgmt_frm_type);
    if (ul_asoc_rsp_len == 0) {
        oam_warning_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::hmac_mgmt_encap_asoc_rsp_ap encap msg fail.}");
        oal_netbuf_free(pst_asoc_rsp);

        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return OAL_FAIL;
    }

    oal_netbuf_put(pst_asoc_rsp, ul_asoc_rsp_len);

    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_hmac_user->st_user_base_info.us_assoc_id;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (uint16_t)ul_asoc_rsp_len;

    /* ���͹�����Ӧ֮֡ǰ�����û��Ľ���״̬��λ */
    hmac_mgmt_reset_psm(&pst_hmac_vap->st_vap_base_info, MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));

    /* �жϵ�ǰ״̬������û��Ѿ������ɹ������ϱ��û��뿪��Ϣ */
    if (pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        pst_net_device = hmac_vap_get_net_device(pst_hmac_vap->st_vap_base_info.uc_vap_id);
        if (pst_net_device != OAL_PTR_NULL) {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            oal_kobject_uevent_env_sta_leave(pst_net_device, puc_sta_addr);
#endif
        }
    }

    if ((*pen_status_code != MAC_SUCCESSFUL_STATUSCODE) &&
        (*pen_status_code != MAC_REJECT_TEMP)) {
        hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info),
                                 &pst_hmac_user->st_user_base_info,
                                 MAC_USER_STATE_AUTH_COMPLETE);
    }

    ul_rslt = hmac_tx_mgmt_send_event(&(pst_hmac_vap->st_vap_base_info), pst_asoc_rsp, (uint16_t)ul_asoc_rsp_len);
    if (ul_rslt != OAL_SUCC) {
        oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::hmac_tx_mgmt_send_event failed[%d].}", ul_rslt);
        oal_netbuf_free(pst_asoc_rsp);

        /* �쳣����֮ǰɾ��user */
        hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);

        return ul_rslt;
    }

    if (*pen_status_code == MAC_SUCCESSFUL_STATUSCODE) {
        /* AP���STA�ɹ�������������ɹ� */
        ul_rslt = hmac_config_user_rate_info_syn(&(pst_hmac_vap->st_vap_base_info),
                                                 &pst_hmac_user->st_user_base_info);
        if (ul_rslt != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_sta_wait_asoc_rx::hmac_config_user_rate_info_syn failed[%d].}", ul_rslt);
        }

        /*  user�Ѿ������ϣ����¼���DMAC����DMAC����û��㷨���� */
        hmac_user_add_notify_alg((&pst_hmac_vap->st_vap_base_info), pst_hmac_user->st_user_base_info.us_assoc_id);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_ap_up_rx_asoc_req(hmac_vap_stru *pst_hmac_vap,
                                           uint8_t uc_mgmt_frm_type,
                                           uint8_t *puc_mac_hdr,
                                           uint32_t ul_mac_hdr_len,
                                           uint8_t *puc_payload,
                                           uint32_t ul_payload_len)
{
    uint32_t ul_rslt;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint16_t us_user_idx = 0;
    mac_status_code_enum_uint16 en_status_code;
    uint8_t auc_sta_addr[WLAN_MAC_ADDR_LEN];
    mac_cfg_user_info_param_stru st_hmac_user_info_event;
    int32_t l_len;
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    int32_t l_not_ie_len;
#endif
    uint8_t uc_frm_least_len;

    uc_frm_least_len = MAC_CAP_INFO_LEN + MAC_LIS_INTERVAL_IE_LEN;
    uc_frm_least_len += (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) ? WLAN_MAC_ADDR_LEN : 0;

    if (ul_payload_len <= uc_frm_least_len) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "hmac_ap_up_rx_asoc_req:assoc req len[%d] too short!",
                         ul_payload_len);
        return OAL_FAIL;
    }

    mac_get_address2(puc_mac_hdr, auc_sta_addr, WLAN_MAC_ADDR_LEN);

    ul_rslt = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, &us_user_idx);
    if (ul_rslt != OAL_SUCC) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_asoc_req::find user by macaddr XX:XX:XX:%02X:%02X:%02X result is %d }",
                         auc_sta_addr[3], auc_sta_addr[4], auc_sta_addr[5], ul_rslt);
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, MAC_ASOC_NOT_AUTH, OAL_FALSE);

        return ul_rslt;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ASSOC,
                       "{hmac_ap_up_rx_asoc_req::pst_hmac_user[%d] null.}", us_user_idx);

        /* û�в鵽��Ӧ��USER,����ȥ��֤��Ϣ */
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, MAC_ASOC_NOT_AUTH, OAL_FALSE);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->st_mgmt_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&pst_hmac_user->st_mgmt_timer);
    }

    en_status_code = MAC_SUCCESSFUL_STATUSCODE;

    /* �Ƿ���ϴ���SA query���̵����� */
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    if ((pst_hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active == OAL_TRUE)) {
        ul_rslt = hmac_start_sa_query(&pst_hmac_vap->st_vap_base_info, pst_hmac_user,
                                      pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active);
        if (ul_rslt != OAL_SUCC) {
            oam_error_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_ap_up_rx_asoc_req::hmac_start_sa_query failed[%d].}", ul_rslt);
            return ul_rslt;
        }
        oam_info_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                      "{hmac_ap_up_rx_asoc_req::set en_status_code is MAC_REJECT_TEMP.}");
        en_status_code = MAC_REJECT_TEMP;
    }
#endif

    if (en_status_code != MAC_REJECT_TEMP) {
        /* �����Բ��ҵ��û�ʱ,˵����ǰUSER��״̬Ϊ�ѹ���������֤��� �����û������Ϣ�Լ��������� */
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CONN,
                         "{hmac_ap_up_rx_asoc_req:: ASSOC_REQ rx : user mac:%02X:XX:XX:%02X:%02X:%02X.}",
                         auc_sta_addr[0], auc_sta_addr[3], auc_sta_addr[4], auc_sta_addr[5]);

        ul_rslt = hmac_ap_up_update_sta_user(pst_hmac_vap, puc_mac_hdr,
                                             puc_payload, ul_payload_len,
                                             pst_hmac_user, &en_status_code);
        if (en_status_code != MAC_SUCCESSFUL_STATUSCODE) {
            oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_ap_up_rx_asoc_req::hmac_ap_up_update_sta_user failed[%d].}", en_status_code);
            hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info),
                                     &pst_hmac_user->st_user_base_info,
                                     MAC_USER_STATE_AUTH_COMPLETE);
        }

        /* check OWE capability in assoc req frame */
        l_len = ((uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) ?
                 (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN + WLAN_MAC_ADDR_LEN) : (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN));
        hmac_ap_check_owe_capa(pst_hmac_vap, pst_hmac_user,
                               (puc_payload + l_len), (ul_payload_len - l_len), &en_status_code);

        /* ͬ��ap������������Ϣ��dmac */
        hmac_chan_sync(&pst_hmac_vap->st_vap_base_info, pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
                       MAC_VAP_GET_CAP_BW(&pst_hmac_vap->st_vap_base_info), OAL_FALSE);

        /* �����û�֧�ִ���������Э�̳���ǰ����dmac offload�ܹ��£�ͬ��������Ϣ��device */
        ul_rslt = hmac_config_user_info_syn(&(pst_hmac_vap->st_vap_base_info), &pst_hmac_user->st_user_base_info);
        if (ul_rslt != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_ap_up_rx_asoc_req::usr_info_syn failed[%d].}", ul_rslt);
        }

        if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
            ul_rslt = hmac_init_security(&(pst_hmac_vap->st_vap_base_info), auc_sta_addr, sizeof(auc_sta_addr));
            if (ul_rslt != OAL_SUCC) {
                oam_error_log2(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                               "{hmac_ap_up_rx_asoc_req::hmac_init_security failed[%d] status_code[%d].}",
                               ul_rslt, MAC_UNSPEC_FAIL);
                en_status_code = MAC_UNSPEC_FAIL;
            }

            ul_rslt = hmac_init_user_security_port(&(pst_hmac_vap->st_vap_base_info),
                                                   &(pst_hmac_user->st_user_base_info));
            if (ul_rslt != OAL_SUCC) {
                oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                               "{hmac_ap_up_rx_asoc_req::hmac_init_user_security_port failed[%d].}", ul_rslt);
            }
        }

        if ((ul_rslt != OAL_SUCC)
            || (en_status_code != MAC_SUCCESSFUL_STATUSCODE)) {
            oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                             "{hmac_ap_up_rx_asoc_req::hmac_ap_up_update_sta_user fail rslt[%d] status_code[%d].",
                             ul_rslt, en_status_code);
            hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info),
                                     &pst_hmac_user->st_user_base_info,
                                     MAC_USER_STATE_AUTH_COMPLETE);
        }
        l_len = ((uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_REASSOC_REQ) ?
                 (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN + WLAN_MAC_ADDR_LEN) : (MAC_CAP_INFO_LEN + MAC_LISTEN_INT_LEN));
        if (IS_P2P_GO(&pst_hmac_vap->st_vap_base_info) &&
            (mac_find_vendor_ie(MAC_WLAN_OUI_WFA, MAC_WLAN_OUI_TYPE_WFA_P2P,
                                puc_payload + l_len, (int32_t)ul_payload_len - l_len) == OAL_PTR_NULL)) {
            oam_info_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                          "{hmac_ap_up_rx_asoc_req::GO got assoc request from legacy device, length = [%d]}",
                          ul_payload_len);
            hmac_disable_p2p_pm(pst_hmac_vap);
        }

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
        /* ����check�ӿ�У���û��Ƿ�֧��4��ַ �����û���4��ַ��������λ��1 */
        if (pst_hmac_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_ROOTAP) {
            if (hmac_vmsta_check_user_a4_support(puc_mac_hdr, ul_mac_hdr_len + ul_payload_len)) {
                pst_hmac_user->uc_is_wds = OAL_TRUE;
                oam_warning_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                                 "{hmac_ap_up_rx_asoc_req::user surpport 4 address.}");
            }
        }
#endif  // _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    }

    if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
        hmac_user_set_asoc_state(&(pst_hmac_vap->st_vap_base_info),
                                 &pst_hmac_user->st_user_base_info,
                                 MAC_USER_STATE_ASSOC);
    }

    if (pst_hmac_user->st_hmac_cap_info.bit_owe == OAL_TRUE) {
        /* APUT received OWE assoc req && check valid OWE DH Parameter Element, report assoc req to hostapd;
         * otherwise, APUT send assoc rsp frame with reject status code */
        if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
            /* AP ����STA �Ĺ�������֡��Ϣ���Ա��ϱ��ں� */
            hmac_ap_save_user_assoc_req(pst_hmac_user, puc_payload, ul_payload_len, uc_mgmt_frm_type);

            /* �ϱ�WAL��(WAL�ϱ��ں�) AP��������һ���µ�STA */
            hmac_handle_connect_rsp_ap(pst_hmac_vap, pst_hmac_user);
            oam_warning_log4(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_CONN,
                             "{hmac_ap_up_rx_asoc_req:: new OWE STA assoc AP! STA_indx=%d. \
                             user mac:XX:XX:XX:%02X:%02X:%02X.}",
                             us_user_idx, auc_sta_addr[3], auc_sta_addr[4], auc_sta_addr[5]);
        } else {
            hmac_ap_send_assoc_rsp(pst_hmac_vap, pst_hmac_user, auc_sta_addr, uc_mgmt_frm_type, &en_status_code);

            hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
        }
    } else {
        hmac_ap_send_assoc_rsp(pst_hmac_vap, pst_hmac_user, auc_sta_addr, uc_mgmt_frm_type, &en_status_code);

        if (en_status_code == MAC_SUCCESSFUL_STATUSCODE) {
            /* AP ����STA �Ĺ�������֡��Ϣ���Ա��ϱ��ں� */
            hmac_ap_save_user_assoc_req(pst_hmac_user, puc_payload, ul_payload_len, uc_mgmt_frm_type);

            /* �ϱ�WAL��(WAL�ϱ��ں�) AP��������һ���µ�STA */
            hmac_handle_connect_rsp_ap(pst_hmac_vap, pst_hmac_user);
            oam_warning_log4(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_CONN,
                             "{hmac_ap_up_rx_asoc_req::ASSOC_RSP tx,STA assoc AP SUCC! \
                             STA_idx=%d.user mac:XX:XX:XX:%02X:%02X:%02X}",
                             us_user_idx, auc_sta_addr[3], auc_sta_addr[4], auc_sta_addr[5]);
        } else {
            /* AP���STAʧ�ܣ�����ɾ�� */
            if (en_status_code != MAC_REJECT_TEMP) {
                if (pst_hmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPC == OAL_FALSE) {
                    if (en_status_code == MAC_INVALID_AKMP_CIPHER) {
                        hmac_mgmt_send_deauth_frame(&pst_hmac_vap->st_vap_base_info,
                                                    pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                                    MAC_AUTH_NOT_VALID, OAL_FALSE);
                    }
                    hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
                } else {
                    hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
                }
            }
        }
    }

    /* STA �������ϱ�VAP ��Ϣ���û���Ϣ */
    st_hmac_user_info_event.us_user_idx = us_user_idx;

    hmac_config_vap_info(&(pst_hmac_vap->st_vap_base_info), OAL_SIZEOF(uint32_t), (uint8_t *)&ul_rslt);
    hmac_config_user_info(&(pst_hmac_vap->st_vap_base_info),
                          OAL_SIZEOF(mac_cfg_user_info_param_stru),
                          (uint8_t *)&st_hmac_user_info_event);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_ap_up_rx_disasoc(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_mac_hdr, uint32_t ul_mac_hdr_len,
                                          uint8_t *puc_payload, uint32_t ul_payload_len,
                                          oal_bool_enum_uint8 en_is_protected)
{
    uint32_t ul_ret;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t *puc_da = OAL_PTR_NULL;
    uint8_t *puc_sa = OAL_PTR_NULL;
    uint8_t auc_sta_addr[WLAN_MAC_ADDR_LEN];

    if (ul_payload_len < MAC_80211_REASON_CODE_LEN) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CONN,
                         "{hmac_ap_up_rx_disasoc::invalid ul_payload_len len [%d]}", ul_payload_len);
        return OAL_FAIL;
    }

    mac_get_address2(puc_mac_hdr, auc_sta_addr, WLAN_MAC_ADDR_LEN);

    /* ���ӽ��յ�ȥ����֡ʱ��ά����Ϣ */
    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CONN,
                     "{hmac_ap_up_rx_disasoc::DISASSOC rx, Because of err_code[%d], \
                     received disassoc frame from source addr %02X:XX:XX:XX:%02X:%02X.}",
                     *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)), puc_sa[0], puc_sa[4], puc_sa[5]);

    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&(pst_hmac_vap->st_vap_base_info), &auc_sta_addr[0]);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_ap_up_rx_disasoc::pst_hmac_user null.}");
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
                oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                               "{hmac_ap_up_rx_disasoc::hmac_start_sa_query failed[%d].}", ul_ret);
                return OAL_ERR_CODE_PMF_SA_QUERY_START_FAIL;
            }
            return OAL_SUCC;
        }
#endif

        /* ������û��Ĺ���֡�������Բ�һ�£������ñ��� */
        mac_rx_get_da((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_da);
        if ((ether_is_multicast(puc_da) != OAL_TRUE) &&
            (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active)) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                           "{hmac_ap_up_rx_disasoc::PMF check failed,en_is_protected=%d.}", en_is_protected);
            return OAL_FAIL;
        }
        mac_user_set_asoc_state(&pst_hmac_user->st_user_base_info, MAC_USER_STATE_AUTH_COMPLETE);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                             CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_PASSIVE_DISCONNECT,
                             *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)));

        /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
        hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

        /* ��Щ����ȥ����ʱֻ����DISASOC,Ҳ��ɾ������AP�ڲ������ݽṹ */
        ul_ret = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_ap_up_rx_disasoc::hmac_user_del failed[%d].}", ul_ret);
        }
    }

    return OAL_SUCC;
}


OAL_STATIC void hmac_ap_up_rx_action_nonuser(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl;
    uint8_t *puc_data;
    mac_ieee80211_frame_stru *pst_frame_hdr; /* ����mac֡��ָ�� */

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ��ȡ֡ͷ��Ϣ */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info);

    /* ��ȡ֡��ָ�� */
    puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info) +
               pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* Category */
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_PUBLIC: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_PUB_VENDOR_SPECIFIC: {
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
                    if (oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN) == 0) {
                        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                         "{hmac_ap_up_rx_action::hmac location get.}");
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

        default:
            break;
    }
    return;
}


OAL_STATIC void hmac_ap_up_rx_action(hmac_vap_stru *pst_hmac_vap,
                                     oal_netbuf_stru *pst_netbuf,
                                     oal_bool_enum_uint8 en_is_protected)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint8_t *puc_data = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL; /* ����mac֡��ָ�� */
    hmac_user_stru *pst_hmac_user;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ��ȡ֡ͷ��Ϣ */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info);

    /* ��ȡ���Ͷ˵��û�ָ�� */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, pst_frame_hdr->auc_address2);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_ap_up_rx_action::mac_vap_find_user_by_macaddr failed.}");
        hmac_ap_up_rx_action_nonuser(pst_hmac_vap, pst_netbuf);
        return;
    }

    /* ��ȡ֡��ָ�� */
    puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info) +
               pst_rx_ctrl->st_rx_info.uc_mac_header_len;

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
                case MAC_PUB_VENDOR_SPECIFIC: {
                    /* ����OUI-OUI typeֵΪ 50 6F 9A - 09 (WFA P2P v1.0)  */
                    /* ����hmac_rx_mgmt_send_to_host�ӿ��ϱ� */
                    if (mac_ie_check_p2p_action(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN) == OAL_TRUE) {
                        hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
                    }
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
                    if (oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN) == 0) {
                        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                         "{hmac_ap_up_rx_action::hmac location get.}");
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
                case MAC_HT_ACTION_NOTIFY_CHANNEL_WIDTH:
                    break;

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
                case MAC_VHT_ACTION_BUTT:
                default:
                    break;
            }
        }
        break;

        case MAC_ACTION_CATEGORY_VENDOR: {
            /* ����OUI-OUI typeֵΪ 50 6F 9A - 09 (WFA P2P v1.0)  */
            /* ����hmac_rx_mgmt_send_to_host�ӿ��ϱ� */
            if (mac_ie_check_p2p_action(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN) == OAL_TRUE) {
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

        default:
            break;
    }
    return;
}


OAL_STATIC void hmac_ap_up_rx_probe_req(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    enum nl80211_band en_band;
#else
    enum ieee80211_band en_band;
#endif
    int32_t l_freq;

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


uint32_t hmac_ap_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint8_t *puc_payload = OAL_PTR_NULL;
    uint32_t ul_msg_len;     /* ��Ϣ�ܳ���,������FCS */
    uint32_t ul_mac_hdr_len; /* MACͷ���� */
    uint8_t uc_mgmt_frm_type;
    oal_bool_enum_uint8 en_is_protected;

    uint8_t *puc_sa = NULL;
    oal_bool_enum_uint8 en_blacklist_result;

    if (oal_any_null_ptr2(pst_hmac_vap, p_param)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ap_up_rx_mgmt::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    ul_mac_hdr_len = pst_rx_info->uc_mac_header_len; /* MACͷ���� */
    puc_payload = (uint8_t *)(puc_mac_hdr) + ul_mac_hdr_len;
    ul_msg_len = pst_rx_info->us_frame_len; /* ��Ϣ�ܳ���,������FCS */
    en_is_protected = (uint8_t)mac_is_protectedframe(puc_mac_hdr);

    /* AP��UP״̬�� ���յ��ĸ��ֹ���֡���� */
    uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (wlan_pm_wkup_src_debug_get() == OAL_TRUE) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:chmac_ap_up_rx_mgmt::wakeup mgmt type[0x%x]}", uc_mgmt_frm_type);
    }
#endif
    /* Bar frame proc here */
    if (mac_get_frame_type(puc_mac_hdr) == WLAN_FC0_TYPE_CTL) {
        uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);
        if (uc_mgmt_frm_type == WLAN_FC0_SUBTYPE_BAR) {
            hmac_up_rx_bar(pst_hmac_vap, pst_rx_ctrl, pst_mgmt_rx_event->pst_netbuf);
        }
    } else if (mac_get_frame_type(puc_mac_hdr) == WLAN_FC0_TYPE_MGT) {
        mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);

        /* �Զ������������� */
        if (oal_value_eq_any2(uc_mgmt_frm_type, WLAN_FC0_SUBTYPE_ASSOC_REQ, WLAN_FC0_SUBTYPE_REASSOC_REQ)) {
            hmac_autoblacklist_filter(&pst_hmac_vap->st_vap_base_info, puc_sa);
        }

        /* ���������˼�� */
        en_blacklist_result = hmac_blacklist_filter(&pst_hmac_vap->st_vap_base_info, puc_sa);
        if ((en_blacklist_result == OAL_TRUE) && (uc_mgmt_frm_type != WLAN_FC0_SUBTYPE_AUTH)) {
            return OAL_SUCC;
        }

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
        proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
#endif

        switch (uc_mgmt_frm_type) {
            case WLAN_FC0_SUBTYPE_AUTH:
                hmac_ap_rx_auth_req(pst_hmac_vap, pst_mgmt_rx_event->pst_netbuf);
                break;

            case WLAN_FC0_SUBTYPE_DEAUTH:
                if (ul_msg_len < ul_mac_hdr_len + MAC_80211_REASON_CODE_LEN) {
                    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                     "{hmac_ap_up_rx_mgmt::invalid deauth_req length[%d]}", ul_msg_len);
                } else {
                    hmac_ap_rx_deauth_req(pst_hmac_vap, puc_mac_hdr, en_is_protected);
                }
                break;

            case WLAN_FC0_SUBTYPE_ASSOC_REQ:
            case WLAN_FC0_SUBTYPE_REASSOC_REQ:
                hmac_ap_up_rx_asoc_req(pst_hmac_vap, uc_mgmt_frm_type,
                                       puc_mac_hdr, ul_mac_hdr_len, puc_payload, (ul_msg_len - ul_mac_hdr_len));
                break;

            case WLAN_FC0_SUBTYPE_DISASSOC:
                hmac_ap_up_rx_disasoc(pst_hmac_vap, puc_mac_hdr,
                                      ul_mac_hdr_len, puc_payload,
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
    }

    return OAL_SUCC;
}


uint32_t hmac_mgmt_timeout_ap(void *p_param)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user;
    uint32_t ul_ret;

    pst_hmac_user = (hmac_user_stru *)p_param;
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_AUTH, "{hmac_mgmt_timeout_ap::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_mgmt_timeout_ap::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                     "{hmac_mgmt_timeout_ap::Wait AUTH timeout!! After %d ms.}",
                     WLAN_AUTH_AP_TIMEOUT);
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                         CHR_LAYER_DRV, CHR_WIFI_DRV_EVENT_SOFTAP_CONNECT,
                         MAC_AP_AUTH_RSP_TIMEOUT);
    /* ����ȥ����֡��Ϣ��STA */
    hmac_mgmt_send_deauth_frame(&pst_hmac_vap->st_vap_base_info,
                                pst_hmac_user->st_user_base_info.auc_user_mac_addr,
                                MAC_AUTH_NOT_VALID, OAL_FALSE);

    /* ���¼��ϱ��ںˣ��Ѿ�ȥ����ĳ��STA */
    hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

    ul_ret = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_mgmt_timeout_ap::hmac_user_del failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}


uint32_t hmac_ap_wait_start_misc(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    hmac_misc_input_stru *pst_misc_input = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, p_param))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ap_wait_start_misc::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_misc_input = (hmac_misc_input_stru *)p_param;

    switch (pst_misc_input->en_type) {
        case HMAC_MISC_RADAR:
#ifdef _PRE_WLAN_FEATURE_DFS
            hmac_dfs_ap_wait_start_radar_handler(pst_hmac_vap);
#endif
            break;

        default:
            break;
    }

    return OAL_SUCC;
}


uint32_t hmac_ap_up_misc(hmac_vap_stru *pst_hmac_vap, void *p_param)
{
    hmac_misc_input_stru *pst_misc_input = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, p_param))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ap_up_misc::param null, %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_misc_input = (hmac_misc_input_stru *)p_param;

    switch (pst_misc_input->en_type) {
        case HMAC_MISC_RADAR:
#ifdef _PRE_WLAN_FEATURE_DFS
            hmac_dfs_ap_up_radar_handler(pst_hmac_vap);
#endif
            break;

        default:
            break;
    }

    return OAL_SUCC;
}

uint32_t hmac_ap_clean_bss(hmac_vap_stru *pst_hmac_vap)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_user_tmp = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user_tmp = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_is_protected;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ap_clean_bss::hmac vap is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ɾ��vap�������ѹ����û�����֪ͨ�ں� */
    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;
    oal_dlist_search_for_each_safe(pst_entry, pst_next_entry, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        /* ����֡�����Ƿ��� */
        en_is_protected = pst_user_tmp->st_cap_info.bit_pmf_active;

        /* ��ȥ����֡ */
        hmac_mgmt_send_disassoc_frame(pst_mac_vap, pst_user_tmp->auc_user_mac_addr,
                                      MAC_DISAS_LV_SS, en_is_protected);

        /* ֪ͨ�ں� */
        hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user_tmp);

        /* ɾ���û� */
        hmac_user_del(pst_mac_vap, pst_hmac_user_tmp);
    }

    return OAL_SUCC;
}
