

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

/* 1 ͷ�ļ����� */
#include "hmac_edca_opt.h"
#include "hmac_vap.h"
#include "oam_wdk.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_EDCA_OPT_C
/* 2 �ṹ�嶨�� */
/* 3 �궨�� */
#define HMAC_EDCA_OPT_ADJ_STEP 2

/* (3-a)/3*X + a/3*Y */
#define WLAN_EDCA_OPT_MOD(X, Y, a) \
    (((X) * (WLAN_EDCA_OPT_MAX_WEIGHT_STA - a) + (Y) * (a)) / WLAN_EDCA_OPT_MAX_WEIGHT_STA);

/* 5 �ڲ���̬�������� */
OAL_STATIC oal_bool_enum_uint8 hmac_edca_opt_check_is_tcp_data(mac_ip_header_stru *pst_ip);
OAL_STATIC uint32_t hmac_edca_opt_stat_traffic_num(hmac_vap_stru *pst_hmac_vap,
                                                     uint8_t (*ppuc_traffic_num)[WLAN_TXRX_DATA_BUTT]);

/* 6 ����ʵ�� */

OAL_STATIC oal_bool_enum_uint8 hmac_edca_opt_check_is_tcp_data(mac_ip_header_stru *pst_ip)
{
    uint8_t *puc_ip = (uint8_t *)pst_ip;
    uint16_t us_ip_len;
    uint8_t uc_ip_header_len = ((*puc_ip) & 0x0F) << 2; /* IP_HDR_LEN */
    uint8_t uc_tcp_header_len;

    /* ��ȡip���ĳ��� */
    us_ip_len = (*(puc_ip + 2 /* length in ip header */)) << 8;
    us_ip_len |= *(puc_ip + 2 /* length in ip header */ + 1);

    /* ��ȡtcp header���� */
    uc_tcp_header_len = *(puc_ip + uc_ip_header_len + 12 /* length in tcp header */);
    uc_tcp_header_len = (uc_tcp_header_len >> 4) << 2;

    if ((us_ip_len - uc_ip_header_len) == uc_tcp_header_len) {
        oam_info_log3(0, OAM_SF_TX,
            "{hmac_edca_opt_check_is_tcp_data:is tcp ack, us_ip_len:%d, uc_ip_header_len:%d, uc_tcp_header_len:%d",
            us_ip_len, uc_ip_header_len, uc_tcp_header_len);
        return OAL_FALSE;
    } else {
        oam_info_log3(0, OAM_SF_TX,
            "{hmac_edca_opt_check_is_tcp_data:is tcp data, us_ip_len:%d, uc_ip_header_len:%d, uc_tcp_header_len:%d",
            us_ip_len, uc_ip_header_len, uc_tcp_header_len);
        return OAL_TRUE;
    }
}


OAL_STATIC uint32_t hmac_edca_opt_stat_traffic_num(hmac_vap_stru *pst_hmac_vap,
                                                     uint8_t (*ppuc_traffic_num)[WLAN_TXRX_DATA_BUTT])
{
    mac_user_stru *pst_user;
    hmac_user_stru *pst_hmac_user;
    uint8_t uc_ac_idx;
    uint8_t uc_data_idx;
    mac_vap_stru *pst_vap = &(pst_hmac_vap->st_vap_base_info);
    oal_dlist_head_stru *pst_list_pos = OAL_PTR_NULL;

    pst_list_pos = pst_vap->st_mac_user_list_head.pst_next;

    for (; pst_list_pos != &(pst_vap->st_mac_user_list_head); pst_list_pos = pst_list_pos->pst_next) {
        pst_user = oal_dlist_get_entry(pst_list_pos, mac_user_stru, st_user_dlist);
        pst_hmac_user = mac_res_get_hmac_user(pst_user->us_assoc_id);
        if (pst_hmac_user == OAL_PTR_NULL) {
            oam_error_log1(pst_vap->uc_vap_id, OAM_SF_CFG,
                           "hmac_edca_opt_stat_traffic_num: pst_hmac_user[%d] is null ptr!", pst_user->us_assoc_id);
            continue;
        }

        for (uc_ac_idx = 0; uc_ac_idx < WLAN_WME_AC_BUTT; uc_ac_idx++) {
            for (uc_data_idx = 0; uc_data_idx < WLAN_TXRX_DATA_BUTT; uc_data_idx++) {
                oam_info_log4(0, OAM_SF_TX,
                              "mac_edca_opt_stat_traffic_num, assoc_id = %d, uc_ac = %d, uc_idx = %d, pkt_num = %d",
                              pst_user->us_assoc_id, uc_ac_idx, uc_data_idx,
                              pst_hmac_user->aaul_txrx_data_stat[uc_ac_idx][uc_data_idx]);

                if (pst_hmac_user->aaul_txrx_data_stat[uc_ac_idx][uc_data_idx] > HMAC_EDCA_OPT_PKT_NUM) {
                    ppuc_traffic_num[uc_ac_idx][uc_data_idx]++;
                }

                /* ͳ�������0 */
                pst_hmac_user->aaul_txrx_data_stat[uc_ac_idx][uc_data_idx] = 0;
            }
        }
    }

    return OAL_SUCC;
}


uint32_t hmac_edca_opt_timeout_fn(void *p_arg)
{
    uint8_t aast_uc_traffic_num[WLAN_WME_AC_BUTT][WLAN_TXRX_DATA_BUTT];
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event;

    if (oal_unlikely(p_arg == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_edca_opt_timeout_fn::p_arg is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)p_arg;

    /* ������ʼ�� */
    memset_s(aast_uc_traffic_num, OAL_SIZEOF(aast_uc_traffic_num), 0, OAL_SIZEOF(aast_uc_traffic_num));

    /* ͳ��device�������û���/���� TPC/UDP����Ŀ */
    hmac_edca_opt_stat_traffic_num(pst_hmac_vap, aast_uc_traffic_num);

    /***************************************************************************
        ���¼���dmacģ��,��ͳ����Ϣ����dmac
    ***************************************************************************/

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(aast_uc_traffic_num));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANTI_INTF,
                       "{hmac_edca_opt_timeout_fn::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPR_EDCA_OPT,
                       OAL_SIZEOF(aast_uc_traffic_num),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* �������� */
    if (EOK != memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(aast_uc_traffic_num),
                        (uint8_t *)aast_uc_traffic_num, OAL_SIZEOF(aast_uc_traffic_num))) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


void hmac_edca_opt_rx_pkts_stat(uint16_t us_assoc_id, uint8_t uc_tidno, mac_ip_header_stru *pst_ip)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_assoc_id);
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_edca_opt_rx_pkts_stat::null param,pst_hmac_user[%d].}", us_assoc_id);
        return;
    }
    oam_info_log0(0, OAM_SF_RX, "{hmac_edca_opt_rx_pkts_stat}");

    /* ����IP_LEN С�� HMAC_EDCA_OPT_MIN_PKT_LEN�ı��� */
    if (oal_net2host_short(pst_ip->us_tot_len) < HMAC_EDCA_OPT_MIN_PKT_LEN) {
        return;
    }

    if (pst_ip->uc_protocol == MAC_UDP_PROTOCAL) {
        pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_RX_UDP_DATA]++;
        oam_info_log4(0, OAM_SF_RX,
                      "{hmac_edca_opt_rx_pkts_stat:is udp_data, assoc_id = %d, tidno = %d, type = %d, num = %d",
                      pst_hmac_user->st_user_base_info.us_assoc_id, uc_tidno, WLAN_RX_UDP_DATA,
                      pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_RX_UDP_DATA]);
    } else if (pst_ip->uc_protocol == MAC_TCP_PROTOCAL) {
        if (hmac_edca_opt_check_is_tcp_data(pst_ip)) {
            pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_RX_TCP_DATA]++;
            oam_info_log4(0, OAM_SF_RX,
                          "{hmac_edca_opt_rx_pkts_stat:is tcp_data, assoc_id = %d, tidno = %d, type = %d, num = %d",
                          pst_hmac_user->st_user_base_info.us_assoc_id, uc_tidno, WLAN_RX_TCP_DATA,
                          pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_RX_TCP_DATA]);
        }
    } else {
        oam_info_log0(0, OAM_SF_RX, "{hmac_tx_pkts_stat_for_edca_opt: neither UDP nor TCP ");
    }
}


void hmac_edca_opt_tx_pkts_stat(mac_tx_ctl_stru *pst_tx_ctl, uint8_t uc_tidno, mac_ip_header_stru *pst_ip)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_CFG, "{hmac_edca_opt_rx_pkts_stat::null param,pst_hmac_user[%d].}",
                       MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
        return;
    }
    oam_info_log0(0, OAM_SF_RX, "{hmac_edca_opt_tx_pkts_stat}");

    /* ����IP_LEN С�� HMAC_EDCA_OPT_MIN_PKT_LEN�ı��� */
    if (oal_net2host_short(pst_ip->us_tot_len) < HMAC_EDCA_OPT_MIN_PKT_LEN) {
        return;
    }

    if (pst_ip->uc_protocol == MAC_UDP_PROTOCAL) {
        pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_TX_UDP_DATA]++;
        oam_info_log4(0, OAM_SF_TX,
                      "{hmac_edca_opt_tx_pkts_stat:is udp_data, assoc_id = %d, tidno = %d, type = %d, num = %d",
                      pst_hmac_user->st_user_base_info.us_assoc_id, uc_tidno, WLAN_TX_UDP_DATA,
                      pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_TX_UDP_DATA]);
    } else if (pst_ip->uc_protocol == MAC_TCP_PROTOCAL) {
        if (hmac_edca_opt_check_is_tcp_data(pst_ip)) {
            pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_TX_TCP_DATA]++;
            oam_info_log4(0, OAM_SF_TX,
                          "{hmac_edca_opt_tx_pkts_stat:is tcp_data, assoc_id = %d, tidno = %d, type = %d, num = %d",
                          pst_hmac_user->st_user_base_info.us_assoc_id, uc_tidno, WLAN_TX_TCP_DATA,
                          pst_hmac_user->aaul_txrx_data_stat[WLAN_WME_TID_TO_AC(uc_tidno)][WLAN_TX_TCP_DATA]);
        }
    } else {
        oam_info_log0(0, OAM_SF_TX, "{hmac_edca_opt_tx_pkts_stat: neither UDP nor TCP");
    }
}

#endif /* end of _PRE_WLAN_FEATURE_EDCA_OPT_AP */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

