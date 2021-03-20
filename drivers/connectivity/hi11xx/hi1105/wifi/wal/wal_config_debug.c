

#ifdef _PRE_WLAN_CFGID_DEBUG

/* 1 头文件包含 */
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_types.h"

#include "mac_device.h"
#include "mac_vap.h"
#include "mac_resource.h"

#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "wal_ext_if.h"
#include "wal_main.h"
#include "wal_config.h"
#include "wal_linux_ioctl.h"
#include "wal_linux_bridge.h"

#include "hmac_cali_mgmt.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "hmac_wmmac.h"
#endif
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_CONFIG_DEBUG_C

uint32_t wal_config_set_vendor_ie(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param);


OAL_STATIC uint32_t wal_config_get_hipkt_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_hipkt_stat(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_flowctl_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_flowctl_param(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_flowctl_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_flowctl_stat(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_eth_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_eth_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_80211_ucast_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_80211_ucast_switch(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_TXOPPS


OAL_STATIC uint32_t wal_config_set_txop_ps_machw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_txop_ps_machw(pst_mac_vap, us_len, puc_param);
}

#endif


OAL_STATIC uint32_t wal_config_80211_mcast_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_80211_mcast_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_probe_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_probe_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_phy_debug_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_phy_debug_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_protocol_debug_switch(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_protocol_debug_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_report_vap_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_report_vap_info(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_ota_beacon_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_ota_beacon_switch(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_ota_rx_dscr_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_ota_rx_dscr_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_all_ota(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_all_ota(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_dhcp_arp_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_dhcp_arp_switch(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_CHIP_FPGA_PCIE_TEST

OAL_STATIC uint32_t wal_config_pcie_test(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_pcie_test(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_ampdu_end(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_ampdu_end(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_addba_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_addba_req(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_delba_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_delba_req(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_WMMAC

OAL_STATIC uint32_t wal_config_addts_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_addts_req(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_delts(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_delts(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_wmmac_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_wmmac_switch(pst_mac_vap, us_len, puc_param);
}

#endif  // #ifdef _PRE_WLAN_FEATURE_WMMAC

OAL_STATIC uint32_t wal_config_voe_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_voe_enable(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC uint32_t wal_config_bcn_table_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_bcn_table_switch(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_list_sta(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_list_sta(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_list_channel(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_list_channel(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_start_scan(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_sta_initiate_scan(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_start_join(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_bss_dscr_stru *pst_bss_dscr;
    uint32_t ul_bss_idx;

    ul_bss_idx = (uint8_t)oal_atoi((int8_t *)puc_param);

    /* ??bss???,??????bss dscr??? */
    pst_bss_dscr = hmac_scan_find_scanned_bss_dscr_by_index(pst_mac_vap->uc_device_id, ul_bss_idx);
    if (pst_bss_dscr == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_config_start_join::find bss failed by index!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return hmac_sta_initiate_join(pst_mac_vap, pst_bss_dscr);
}


OAL_STATIC uint32_t wal_config_start_deauth(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    hmac_config_send_deauth(pst_mac_vap, pst_mac_vap->auc_bssid);
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_send_bar(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_send_bar(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_alg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_alg(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
OAL_STATIC uint32_t wal_config_tcp_ack_buf(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_tcp_ack_buf(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_amsdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_amsdu_tx_on(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_ampdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_ampdu_tx_on(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_ampdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_ampdu_tx_on(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_get_amsdu_tx_on(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_amsdu_tx_on(pst_mac_vap, pus_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_txbf_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_txbf_cap(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_reset_hw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_reset_hw(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_reset_operate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_reset_operate(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_dscp_map_to_tid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_dscp_map_to_tid(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_clean_dscp_tid_map(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_clean_dscp_tid_map(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_DFT_STAT

OAL_STATIC uint32_t wal_config_usr_queue_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_usr_queue_stat(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_report_vap_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_report_vap_stat(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_report_all_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_report_all_stat(pst_mac_vap, us_len, puc_param);
}

#endif


OAL_STATIC uint32_t wal_config_set_feature_log(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SET_FEATURE_LOG, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_set_feature_log::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return ul_ret;
}


OAL_STATIC uint32_t wal_config_set_nss(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_nss::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_nss(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_config_set_nss::hmac_config_set_nss error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}


OAL_STATIC uint32_t wal_config_set_rfch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_set_rfch::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设置参数 */
    ul_ret = hmac_config_set_rfch(pst_mac_vap, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_config_set_rfch::hmac_config_set_rfch error!}\r\n");
        return ul_ret;
    }

    return ul_ret;
}

#ifdef _PRE_WLAN_NARROW_BAND

OAL_STATIC uint32_t wal_config_narrow_bw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    mac_cfg_narrow_bw_stru *pst_nb;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_config_narrow_bw::pst_mac_vap/puc_param is null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_nb = (mac_cfg_narrow_bw_stru *)puc_param;

    pst_mac_vap->st_nb = *pst_nb;

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_NARROW_BW, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{wal_config_narrow_bw::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_config_beacon_chain_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_beacon_chain_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_2040_coext_support(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_2040_coext_support(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_alg_cfg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    hmac_vap_stru *pst_hmac_vap;
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{wal_config_alg_cfg::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_net_device = pst_hmac_vap->pst_net_device;

    return wal_hipriv_alg_cfg(pst_net_device, (int8_t *)puc_param);
}

OAL_STATIC uint32_t wal_config_cali_debug(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    return hmac_config_cali_debug(mac_vap, len, param);
}

#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)

OAL_STATIC uint32_t wal_11v_cfg_bsst_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_11v_cfg_bsst_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_11v_sta_tx_query(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_11v_sta_tx_query(pst_mac_vap, us_len, puc_param);
}
#endif

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

OAL_STATIC uint32_t wal_config_enable_pmf(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    /* 设置一下hmac的芯片验证开关 */
    hmac_enable_pmf(pst_mac_vap, puc_param);

    return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_PMF_ENABLE, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_send_frame(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_send_frame_param_stru *pst_param = OAL_PTR_NULL;

    pst_param = (mac_cfg_send_frame_param_stru *)puc_param;

    if (pst_param->en_frame_type == MAC_TEST_MGMT_ACTION) {
        return hmac_config_send_frame(pst_mac_vap, us_len, puc_param);
    } else {
        return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_SEND_FRAME, us_len, puc_param);
    }
}


OAL_STATIC uint32_t wal_config_set_auto_protection(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_auto_protection(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_send_2040_coext(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_send_2040_coext(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_FTM

OAL_STATIC uint32_t wal_config_ftm_dbg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_ftm_dbg(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t _wal_config_ftm_dbg(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        return wal_config_ftm_dbg(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_ftm_startranging(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        return hmac_config_start_ftm_ranging(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

OAL_STATIC uint32_t wal_config_get_user_rssbw(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_user_rssbw(pst_mac_vap, us_len, puc_param);
}
#endif

#ifdef _PRE_WLAN_FEATURE_M2S

OAL_STATIC uint32_t wal_config_set_m2s_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_m2s_switch(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_radar_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_radar_set(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_ru_set(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_ru_set(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS

OAL_STATIC uint32_t wal_config_set_psd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_psd_cap(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_cfg_psd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_cfg_psd(pst_mac_vap, us_len, puc_param);
}

#endif
#ifdef _PRE_WLAN_FEATURE_CSI

OAL_STATIC uint32_t wal_config_set_csi(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_csi(pst_mac_vap, us_len, puc_param);
}
#endif

#ifdef _PRE_WLAN_FEATURE_WAPI

#ifdef _PRE_WAPI_DEBUG
OAL_STATIC uint32_t wal_config_wapi_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_user_info_param_stru *pst_user_info;
    pst_user_info = (mac_cfg_user_info_param_stru *)puc_param;

    hmac_wapi_display_info(pst_mac_vap, pst_user_info->us_user_idx);
    return OAL_SUCC;
}
#endif
#endif /* _PRE_WLAN_FEATURE_WAPI */


OAL_STATIC uint32_t wal_config_set_ampdu_aggr_num(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_ampdu_aggr_num(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_amsdu_aggr_num(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    return hmac_config_set_amsdu_aggr_num(mac_vap, len, param);
}


OAL_STATIC uint32_t wal_config_set_stbc_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_stbc_cap(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_ldpc_cap(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_ldpc_cap(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_show_device_meminfo(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_DEVICE_MEM_INFO, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_pm_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_sync_cmd_common(pst_mac_vap, WLAN_CFGID_SET_PSM_PARAM, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_vap_classify_en(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_vap_classify_en(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_vap_classify_tid(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_vap_classify_tid(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_set_p2p_ps_stat(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_p2p_ps_stat(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG

OAL_STATIC uint32_t wal_config_enable_rr_time_info(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    return hmac_config_enable_rr_time_info(mac_vap, len, param);
}
#endif

OAL_STATIC uint32_t wal_config_show_arpoffload_info(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_show_arpoffload_info(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_TCP_OPT

OAL_STATIC uint32_t wal_config_get_tcp_ack_stream_info(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_get_tcp_ack_stream_info(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_tx_tcp_ack_opt_enable(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_tx_tcp_ack_opt_enable(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_rx_tcp_ack_opt_enable(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_rx_tcp_ack_opt_enable(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_tx_tcp_ack_limit(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_tx_tcp_ack_limit(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_rx_tcp_ack_limit(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_rx_tcp_ack_limit(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_roam_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_roam_enable(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_roam_start(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_roam_start(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_enable_2040bss(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_enable_2040bss(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t wal_config_get_2040bss_sw(mac_vap_stru *pst_mac_vap, uint16_t *pus_len, uint8_t *puc_param)
{
    return hmac_config_get_2040bss_sw(pst_mac_vap, pus_len, puc_param);
}

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI

OAL_STATIC uint32_t wal_config_dyn_cali_param(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_dyn_cali_param(pst_mac_vap, us_len, puc_param);
}
#endif


OAL_STATIC uint32_t wal_config_set_bw_fixed(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_bw_fixed(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_pm_debug_switch(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_pm_debug_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_dbdc_debug_switch(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_dbdc_debug_switch(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_tlv_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_tlv_cmd(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_str_cmd(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_str_cmd(pst_mac_vap, us_len, puc_param);
}


OAL_STATIC uint32_t wal_config_set_owe(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_set_owe(pst_mac_vap, us_len, puc_param);
}

#ifdef _PRE_WLAN_FEATURE_TWT

OAL_STATIC uint32_t wal_config_twt_setup_req(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    return hmac_config_twt_setup_req(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t _wal_config_twt_setup_req(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        return wal_config_twt_setup_req(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_config_twt_teardown_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param)
{
    return hmac_config_twt_teardown_req(pst_mac_vap, us_len, puc_param);
}

OAL_STATIC uint32_t _wal_config_twt_teardown_req(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        return wal_config_twt_teardown_req(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif


#ifdef _PRE_WLAN_FEATURE_HIEX

OAL_STATIC uint32_t  wal_config_set_user_hiex_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
    uint8_t *puc_param)
{
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        return hmac_config_set_user_hiex_enable(pst_mac_vap, us_len, puc_param);
    }

    return OAL_SUCC;
}
#endif

OAL_CONST wal_wid_op_stru g_ast_board_wid_op_debug[] = {
    { WLAN_CFGID_ETH_SWITCH,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_eth_switch },
    { WLAN_CFGID_80211_UCAST_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_80211_ucast_switch },
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    { WLAN_CFGID_TXOP_PS_MACHW, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_txop_ps_machw },
#endif
    { WLAN_CFGID_80211_MCAST_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_80211_mcast_switch },
    { WLAN_CFGID_PROBE_SWITCH,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_probe_switch },
    { WLAN_CFGID_PROTOCOL_DBG,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_protocol_debug_switch },
    { WLAN_CFGID_PHY_DEBUG_SWITCH,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_phy_debug_switch },
    { WLAN_CFGID_REPORT_VAP_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_report_vap_info },
    { WLAN_CFGID_OTA_BEACON_SWITCH,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_ota_beacon_switch },
    { WLAN_CFGID_OTA_RX_DSCR_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_ota_rx_dscr_switch },
    { WLAN_CFGID_SET_ALL_OTA,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_all_ota },
    { WLAN_CFGID_SET_DHCP_ARP,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_dhcp_arp_switch },
#ifdef _PRE_WLAN_CHIP_FPGA_PCIE_TEST
    { WLAN_CFGID_PCIE_TEST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_pcie_test },
#endif
    { WLAN_CFGID_AMPDU_END,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_ampdu_end },
    { WLAN_CFGID_ADDBA_REQ,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_addba_req },
    { WLAN_CFGID_DELBA_REQ,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_delba_req },
    { WLAN_CFGID_SET_FEATURE_LOG,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_feature_log },

#ifdef _PRE_WLAN_FEATURE_WMMAC
    { WLAN_CFGID_ADDTS_REQ,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_addts_req },
    { WLAN_CFGID_DELTS,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_delts },
    { WLAN_CFGID_WMMAC_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_wmmac_switch },
#endif

#ifdef _PRE_WLAN_FEATURE_11K
    { WLAN_CFGID_BCN_TABLE_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_bcn_table_switch },
#endif  // _PRE_WLAN_FEATURE_11K
    { WLAN_CFGID_VOE_ENABLE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_voe_enable },
    { WLAN_CFGID_LIST_STA,             OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_list_sta },
    { WLAN_CFGID_START_SCAN,           OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_start_scan },
    { WLAN_CFGID_START_JOIN,           OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_start_join },
    { WLAN_CFGID_START_DEAUTH,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_start_deauth },
    { WLAN_CFGID_AMPDU_TX_ON,          OAL_FALSE, { 0 }, wal_config_get_ampdu_tx_on, wal_config_ampdu_tx_on },
    { WLAN_CFGID_AMSDU_TX_ON,          OAL_FALSE, { 0 }, wal_config_get_amsdu_tx_on, wal_config_amsdu_tx_on },
    { WLAN_CFGID_SEND_BAR,             OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_send_bar },
    { WLAN_CFGID_RESET_HW,             OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_reset_hw },
    { WLAN_CFGID_RESET_HW_OPERATE,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_reset_operate },
    { WLAN_CFGID_DSCP_MAP_TO_TID,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_dscp_map_to_tid },
    { WLAN_CFGID_CLEAN_DSCP_TID_MAP,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_clean_dscp_tid_map },
    { WLAN_CFGID_SET_VENDOR_IE,        OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_vendor_ie },
#ifdef _PRE_WLAN_DFT_STAT
    { WLAN_CFGID_USR_QUEUE_STAT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_usr_queue_stat },
    { WLAN_CFGID_VAP_STAT,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_report_vap_stat },
    { WLAN_CFGID_ALL_STAT,       OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_report_all_stat },
#endif
    { WLAN_CFGID_ALG,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_alg },
#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    { WLAN_CFGID_TCP_ACK_BUF, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_tcp_ack_buf },
#endif
    { WLAN_CFGID_BEACON_CHAIN_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_beacon_chain_switch },
    { WLAN_CFGID_2040_COEXISTENCE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_2040_coext_support },
    { WLAN_CFGID_ALG_CFG,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_alg_cfg },
    { WLAN_CFGID_CALI_CFG,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cali_debug },
    { WLAN_CFGID_LIST_CHAN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_list_channel },
    { WLAN_CFGID_SET_NSS,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_nss },
    { WLAN_CFGID_SET_RFCH,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_rfch },
#ifdef _PRE_WLAN_NARROW_BAND
    { WLAN_CFGID_NARROW_BW, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_narrow_bw },
#endif
    { WLAN_CFGID_TXBF_SWITCH,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_txbf_cap },
    { WLAN_CFGID_GET_HIPKT_STAT,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_hipkt_stat },
    { WLAN_CFGID_SET_FLOWCTL_PARAM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_flowctl_param },
    { WLAN_CFGID_GET_FLOWCTL_STAT,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_flowctl_stat },
    { WLAN_CFGID_SEND_FRAME, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_send_frame },

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    { WLAN_CFGID_PMF_ENABLE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_enable_pmf },
#endif
    { WLAN_CFGID_SET_AUTO_PROTECTION, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_auto_protection },
    { WLAN_CFGID_SEND_2040_COEXT,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_send_2040_coext },

#ifdef _PRE_WLAN_FEATURE_FTM
    { WLAN_CFGID_FTM_DBG, OAL_FALSE, { 0 }, OAL_PTR_NULL, _wal_config_ftm_dbg },
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    { WLAN_CFGID_GET_USER_RSSBW, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_user_rssbw },
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
    { WLAN_CFGID_SET_M2S_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_m2s_switch },
#endif

#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    { WLAN_CFGID_SET_PSD, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_psd },
    { WLAN_CFGID_CFG_PSD, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_cfg_psd },
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
    { WLAN_CFGID_SET_CSI, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_csi },
#endif
    { WLAN_CFGID_SET_AGGR_NUM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_ampdu_aggr_num },
    { WLAN_CFGID_SET_AMSDU_AGGR_NUM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_amsdu_aggr_num },
    { WLAN_CFGID_SET_STBC_CAP, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_stbc_cap },
    { WLAN_CFGID_SET_LDPC_CAP, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_ldpc_cap },

#ifdef _PRE_WLAN_FEATURE_WAPI
#ifdef _PRE_WAPI_DEBUG
    { WLAN_CFGID_WAPI_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_wapi_info },
#endif
#endif

    { WLAN_CFGID_VAP_CLASSIFY_EN, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_vap_classify_en },
    { WLAN_CFGID_VAP_CLASSIFY_TID, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_vap_classify_tid },
    { WLAN_CFGID_DEVICE_MEM_INFO,          OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_show_device_meminfo },
    { WLAN_CFGID_SET_DEVICE_PKT_STAT,      OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_SET_TX_AMPDU_TYPE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_AMSDU_AMPDU_SWITCH,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_RX_AMPDU_AMSDU,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_SK_PACING_SHIFT,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_ADDBA_RSP_BUFFER, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_TRX_STAT_LOG,     OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_SET_DFS_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_DATA_COLLECT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    { WLAN_CFGID_HID2D_DEBUG_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
#endif
    { WLAN_CFGID_MIMO_BLACKLIST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_ADC_DAC_FREQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_SET_MAC_FREQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_SET_DEVICE_FREQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
    { WLAN_CFGID_SET_LINKLOSS_DISABLE_CSA, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
#ifdef _PRE_WLAN_FEATURE_11AX
    { WLAN_CFGID_11AX_DEBUG, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_str_cmd },
#endif
    { WLAN_CFGID_MAC_TX_COMMON_REPORT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_str_cmd },
    { WLAN_CFGID_MAC_RX_COMMON_REPORT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_str_cmd },
    { WLAN_CFGID_COMMON_DEBUG,         OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_str_cmd },
    { WLAN_CFGID_LOG_DEBUG,              OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_str_cmd },

    { WLAN_CFGID_SET_PSM_PARAM, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_pm_param },
    { WLAN_CFGID_SET_P2P_PS_STAT, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_p2p_ps_stat },

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    { WLAN_CFGID_ENABLE_RR_TIMEINFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_enable_rr_time_info },
#endif
    { WLAN_CFGID_SHOW_ARPOFFLOAD_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_show_arpoffload_info },
#ifdef _PRE_WLAN_TCP_OPT
    { WLAN_CFGID_GET_TCP_ACK_STREAM_INFO, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_get_tcp_ack_stream_info },
    { WLAN_CFGID_TX_TCP_ACK_OPT_ENALBE,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_tx_tcp_ack_opt_enable },
    { WLAN_CFGID_RX_TCP_ACK_OPT_ENALBE,   OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_rx_tcp_ack_opt_enable },
    { WLAN_CFGID_TX_TCP_ACK_OPT_LIMIT,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_tx_tcp_ack_limit },
    { WLAN_CFGID_RX_TCP_ACK_OPT_LIMIT,    OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_rx_tcp_ack_limit },
#endif
    { WLAN_CFGID_ROAM_ENABLE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_roam_enable },
    { WLAN_CFGID_ROAM_START,  OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_roam_start },

    { WLAN_CFGID_2040BSS_ENABLE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_enable_2040bss },
    { WLAN_CFGID_GET_2040BSS_SW, OAL_FALSE, { 0 }, wal_config_get_2040bss_sw, OAL_PTR_NULL },

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    { WLAN_CFGID_DYN_CALI_CFG, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_dyn_cali_param },
#endif

#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    { WLAN_CFGID_11V_BSST_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_11v_cfg_bsst_switch },
    { WLAN_CFGID_11V_TX_QUERY, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_11v_sta_tx_query },
#endif
    { WLAN_CFGID_SET_BW_FIXED, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_bw_fixed },
    { WLAN_CFGID_DBDC_DEBUG_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_dbdc_debug_switch },
    { WLAN_CFGID_PM_DEBUG_SWITCH, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_pm_debug_switch },

    { WLAN_CFGID_SET_OWE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_owe },

#ifdef _PRE_WLAN_FEATURE_TWT
    { WLAN_CFGID_TWT_SETUP_REQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, _wal_config_twt_setup_req },
    { WLAN_CFGID_TWT_TEARDOWN_REQ, OAL_FALSE, { 0 }, OAL_PTR_NULL, _wal_config_twt_teardown_req },
#endif

    { WLAN_CFGID_SET_RADAR, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_radar_set },
    { WLAN_CFGID_SET_RU_TEST, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_ru_set },

    { WLAN_CFGID_SET_WARNING_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },

    { WLAN_CFGID_SET_CHR_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
#ifdef _PRE_WLAN_FEATURE_HIEX
    { WLAN_CFGID_USER_HIEX_ENABLE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_user_hiex_enable },
#endif

#ifdef _PRE_WLAN_FEATURE_FTM
    { WLAN_CFGID_FTM_STARTRANGING, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_ftm_startranging },
#endif

#if defined(_PRE_WLAN_FEATURE_HID2D) && defined(_PRE_WLAN_FEATURE_HID2D_PRESENTATION)
    { WLAN_CFGID_HID2D_PRESENTATION_MODE, OAL_FALSE, { 0 }, OAL_PTR_NULL, wal_config_set_tlv_cmd },
#endif

    { WLAN_CFGID_BUTT, OAL_FALSE, { 0 }, 0, 0 },
};

uint32_t wal_config_get_debug_wid_arrysize(void)
{
    return oal_array_size(g_ast_board_wid_op_debug);
}
#endif

