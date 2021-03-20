

#include "oal_ext_if.h"

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/hisi/hisi_cpufreq_req.h>
#include <linux/cpufreq.h>
#include <linux/pm_qos.h>
#endif
#endif

#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "wlan_chip_i.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "mac_board.h"
#include "hmac_fsm.h"
#include "hmac_main.h"
#include "hmac_vap.h"
#include "hmac_tx_amsdu.h"
#include "hmac_rx_data.h"
#include "hmac_11i.h"
#include "hmac_mgmt_classifier.h"
#include "hmac_tx_complete.h"
#include "hmac_chan_mgmt.h"
#include "hmac_dfs.h"
#include "hmac_rx_filter.h"

#include "hmac_hcc_adapt.h"

#include "hmac_dfs.h"
#include "hmac_config.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_hcc_adapt.h"
#include "hmac_dfx.h"
#include "hmac_cali_mgmt.h"

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
#include "oal_kernel_file.h"
#endif
#include "oal_hcc_host_if.h"
#include "oal_net.h"
#include "hmac_tcp_opt.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_resource.h"
#include "hmac_mgmt_classifier.h"
#include "hmac_ext_if.h"
#include "hmac_auto_adjust_freq.h"
#include "hmac_rx_data.h"
#include "mac_board.h"

#if(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#include "mac_vap.h"
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif
#include "hmac_dbac.h"
#include "securec.h"
#include "securectype.h"

#ifdef _PRE_WLAN_FEATURE_DYN_CLOSED
#include <linux/io.h>
#endif
#ifdef _PRE_WLAN_FEATURE_TWT
#include "hmac_twt.h"
#endif
#include "hmac_tx_msdu_dscr.h"
#include "hal_host_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MAIN_C

/* 2 ȫ�ֱ������� */
/* hmacģ����ӵ�ȫ�ֿ��Ʊ��� */
mac_board_stru g_st_hmac_board;
oal_wakelock_stru g_st_hmac_wakelock;
hmac_rxdata_thread_stru g_st_rxdata_thread;
/* ��Ƶ�ӿ� */
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
struct cpufreq_req g_ast_cpufreq[OAL_BUS_MAXCPU_NUM];
hisi_max_cpu_freq g_aul_cpumaxfreq[OAL_BUS_MAXCPU_NUM];
struct pm_qos_request g_st_pmqos_requset;
#endif
oal_bool_enum_uint8 g_feature_switch[HMAC_FEATURE_SWITCH_BUTT] = {0};

OAL_STATIC uint32_t hmac_create_ba_event(frw_event_mem_stru *pst_event_mem);
OAL_STATIC uint32_t hmac_del_ba_event(frw_event_mem_stru *pst_event_mem);
OAL_STATIC uint32_t hmac_syn_info_event(frw_event_mem_stru *pst_event_mem);
OAL_STATIC uint32_t hmac_voice_aggr_event(frw_event_mem_stru *pst_event_mem);
#ifdef _PRE_WLAN_FEATURE_M2S
OAL_STATIC uint32_t hmac_m2s_sync_event(frw_event_mem_stru *pst_event_mem);
#endif

uint32_t hmac_init_scan_do(hmac_device_stru *pst_mac_dev, mac_vap_stru *pst_vap,
                                 mac_init_scan_req_stru *pst_cmd, hmac_acs_cfg_stru *pst_acs_cfg);

extern int32_t hmac_hcc_adapt_init(void);
uint32_t hmac_btcoex_check_by_ba_size(hmac_user_stru *pst_hmac_user);
/* 3 ����ʵ�� */

void hmac_board_get_instance(mac_board_stru **ppst_hmac_board)
{
    *ppst_hmac_board = g_pst_mac_board;
}


uint32_t hmac_init_event_process(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL; /* �¼��ṹ�� */
    mac_data_rate_stru *pst_data_rate = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_ctx_event = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_init_event_process::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    pst_ctx_event = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_data_rate = (mac_data_rate_stru *)(oal_netbuf_data(pst_ctx_event->pst_netbuf));

    /* ͬ��mac֧�ֵ����ʼ���Ϣ */
    pst_mac_device = mac_res_get_dev(pst_event->st_event_hdr.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_init_event_process::pst_mac_device null.}");
        oal_netbuf_free(pst_ctx_event->pst_netbuf);
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    if (EOK != memcpy_s((uint8_t *)(pst_mac_device->st_mac_rates_11g),
                        sizeof(mac_data_rate_stru) * MAC_DATARATES_PHY_80211G_NUM,
                        (uint8_t *)pst_data_rate,
                        sizeof(mac_data_rate_stru) * MAC_DATARATES_PHY_80211G_NUM)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_init_event_process::memcpy fail!");
        /* �ͷŵ�02ͬ����Ϣ���õ�netbuf��Ϣ */
        oal_netbuf_free(pst_ctx_event->pst_netbuf);
        return OAL_FAIL;
    }

    /* �ͷŵ�02ͬ����Ϣ���õ�netbuf��Ϣ */
    oal_netbuf_free(pst_ctx_event->pst_netbuf);

    return OAL_SUCC;
}

OAL_STATIC void hmac_event_fsm_netbuf_tx_adapt_subtable_register(void)
{
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_CALI_HMAC2DMAC].p_tx_adapt_func =
        hmac_send_event_netbuf_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_CALI_MATRIX_HMAC2DMAC].p_tx_adapt_func =
        hmac_send_event_netbuf_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_APP_IE_H2D].p_tx_adapt_func =
        hmac_send_event_netbuf_tx_adapt;
#ifdef _PRE_WLAN_FEATURE_APF
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_APF_CMD].p_tx_adapt_func =
        hmac_send_event_netbuf_tx_adapt;
#endif
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_CUST_HMAC2DMAC].p_tx_adapt_func =
        hmac_send_event_netbuf_tx_adapt;

}

OAL_STATIC void hmac_event_fsm_tx_adapt_subtable_register(void)
{
    /* ע��WLAN_CTX�¼��������� */
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT].p_tx_adapt_func = hmac_proc_tx_host_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_USER].p_tx_adapt_func =
        hmac_proc_add_user_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_DEL_USER].p_tx_adapt_func =
        hmac_proc_del_user_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_EDCA_REG].p_tx_adapt_func =
        hmac_proc_set_edca_param_tx_adapt;
#ifdef _PRE_WLAN_FEATURE_11AX
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_MU_EDCA_REG].p_tx_adapt_func =
        hmac_proc_set_mu_edca_param_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_SPATIAL_REUSE_REG].p_tx_adapt_func =
        hmac_proc_set_spatial_reuse_param_tx_adapt;
#endif
#ifdef _PRE_WLAN_FEATURE_TWT
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_TWT].p_tx_adapt_func =
        hmac_proc_rx_process_twt_sync_event_tx_adapt;
#endif

    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCAN_REQ].p_tx_adapt_func =
        hmac_scan_proc_scan_req_event_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_IP_FILTER].p_tx_adapt_func =
        hmac_config_update_ip_filter_tx_adapt;

    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCHED_SCAN_REQ].p_tx_adapt_func =
        hmac_scan_proc_sched_scan_req_event_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_ASOC_WRITE_REG].p_tx_adapt_func =
        hmac_mgmt_update_user_qos_table_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_SET_REG].p_tx_adapt_func =
        hmac_proc_join_set_reg_event_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_DTIM_TSF_REG].p_tx_adapt_func =
        hmac_proc_join_set_dtim_reg_event_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_CONN_RESULT].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint32;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_NOTIFY_ALG_ADD_USER].p_tx_adapt_func =
        hmac_user_add_notify_alg_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC].p_tx_adapt_func =
        hmac_proc_rx_process_sync_event_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WALN_CTX_EVENT_SUB_TYPR_SELECT_CHAN].p_tx_adapt_func =
        hmac_chan_select_channel_mac_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_SWITCH_TO_NEW_CHAN].p_tx_adapt_func =
        hmac_chan_initiate_switch_to_new_channel_tx_adapt;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPR_RESTART_NETWORK].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WALN_CTX_EVENT_SUB_TYPR_DISABLE_TX].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WALN_CTX_EVENT_SUB_TYPR_ENABLE_TX].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPR_EDCA_OPT].p_tx_adapt_func =
        hmac_edca_opt_stat_event_tx_adapt;
#endif
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_DSCR_OPT].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint8;

#ifdef _PRE_WLAN_FEATURE_DFS
#ifdef _PRE_WLAN_FEATURE_OFFCHAN_CAC
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPR_SWITCH_TO_OFF_CHAN].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPR_SWITCH_TO_HOME_CHAN].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
#endif
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WALN_CTX_EVENT_SUB_TYPR_DFS_CAC_CTRL_TX].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
#endif
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_RESET_PSM].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;

    /* ע��HOST_DRX�¼��������� */
    g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX].p_tx_adapt_func = hmac_proc_tx_host_tx_adapt;

    /* ע��HOST_CRX�¼��������� */
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_INIT].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_CREATE_CFG_VAP].p_tx_adapt_func =
        hmac_hcc_tx_convert_event_to_netbuf_uint16;
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_REG].p_tx_adapt_func = hmac_sdt_recv_reg_cmd_tx_adapt;
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_CFG].p_tx_adapt_func = hmac_proc_config_syn_tx_adapt;
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_ALG].p_tx_adapt_func = hmac_proc_config_syn_alg_tx_adapt;
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_SAMPLE].p_tx_adapt_func = hmac_sdt_recv_sample_cmd_tx_adapt;
#endif
    hmac_event_fsm_netbuf_tx_adapt_subtable_register();
}


OAL_STATIC void hmac_event_fsm_tx_adapt_subtable_register_ext(void)
{
#ifdef _PRE_WLAN_FEATURE_HIEX
    g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_HIEX_H2D_MSG].p_tx_adapt_func =
        g_wlan_spec_cfg->feature_hiex_is_open ? hmac_hiex_h2d_msg_tx_adapt : OAL_PTR_NULL;
#endif
}

OAL_STATIC void hmac_event_fsm_rx_dft_adapt_subtable_register(void)
{
    frw_event_sub_rx_adapt_table_init(g_ast_hmac_wlan_drx_event_sub_table,
                                          OAL_SIZEOF(g_ast_hmac_wlan_drx_event_sub_table) /
                                          OAL_SIZEOF(frw_event_sub_table_item_stru),
                                          hmac_hcc_rx_convert_netbuf_to_event_default);

    frw_event_sub_rx_adapt_table_init(g_ast_hmac_wlan_crx_event_sub_table,
                                          OAL_SIZEOF(g_ast_hmac_wlan_crx_event_sub_table) /
                                          OAL_SIZEOF(frw_event_sub_table_item_stru),
                                          hmac_hcc_rx_convert_netbuf_to_event_default);

    frw_event_sub_rx_adapt_table_init(g_ast_hmac_wlan_ctx_event_sub_table,
                                          OAL_SIZEOF(g_ast_hmac_wlan_ctx_event_sub_table) /
                                          OAL_SIZEOF(frw_event_sub_table_item_stru),
                                          hmac_hcc_rx_convert_netbuf_to_event_default);

    frw_event_sub_rx_adapt_table_init(g_ast_hmac_wlan_misc_event_sub_table,
                                          OAL_SIZEOF(g_ast_hmac_wlan_misc_event_sub_table) /
                                          OAL_SIZEOF(frw_event_sub_table_item_stru),
                                          hmac_hcc_rx_convert_netbuf_to_event_default);
}


OAL_STATIC void hmac_event_fsm_rx_adapt_subtable_register(void)
{

    hmac_event_fsm_rx_dft_adapt_subtable_register();

    /* ע��HMACģ��WLAN_DRX�¼��ӱ� */
    g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_RX_DATA].p_rx_adapt_func =
        hmac_rx_process_data_rx_adapt;

    /* hcc hdr�޸�Ӱ��rom��ͨ������event��ʽ����ͬ���䷽ʽ */
    g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_WOW_RX_DATA].p_rx_adapt_func =
        hmac_rx_process_wow_data_rx_adapt;

    /* ע��HMACģ��WLAN_CRX�¼��ӱ� */
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_INIT].p_rx_adapt_func =
        hmac_rx_convert_netbuf_to_netbuf_default;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX].p_rx_adapt_func =
        hmac_rx_process_mgmt_event_rx_adapt;

    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX_WOW].p_rx_adapt_func =
        hmac_rx_process_wow_mgmt_event_rx_adapt;

    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT].p_rx_adapt_func =
        hmac_rx_convert_netbuf_to_netbuf_default;

    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT_RING].p_rx_adapt_func =
        hmac_rx_convert_netbuf_to_netbuf_ring;
    /* ע��MISC�¼��ӱ� */
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_CALI_TO_HMAC].p_rx_adapt_func =
        hmac_cali2hmac_misc_event_rx_adapt;

#ifdef _PRE_WLAN_ONLINE_DPD
    /* ע��MISC�¼��ӱ� */
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_TO_HMAC_DPD].p_rx_adapt_func = hmac_dpd_rx_adapt;
#endif

#ifdef _PRE_WLAN_FEATURE_APF
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_APF_REPORT].p_rx_adapt_func =
        hmac_apf_program_report_rx_adapt;
#endif
}


OAL_STATIC uint32_t hmac_bandwidth_info_syn_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    dmac_set_chan_stru *pst_set_chan = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_bandwidth_info_syn_event::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼����¼�ͷ�Լ��¼�payload�ṹ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_set_chan = (dmac_set_chan_stru *)pst_event->auc_event_data;

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_event_hdr->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_config_bandwidth_info_syn_event::mac_res_get_mac_vap fail.vap_id:%u}",
                       pst_event_hdr->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap->st_channel.en_bandwidth = pst_set_chan->st_channel.en_bandwidth;

    mac_mib_set_FortyMHzIntolerant(pst_mac_vap, pst_set_chan->en_dot11FortyMHzIntolerant);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_protection_info_syn_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    mac_h2d_protection_stru *pst_h2d_prot = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_protection_info_syn_event::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼����¼�ͷ�Լ��¼�payload�ṹ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_h2d_prot = (mac_h2d_protection_stru *)pst_event->auc_event_data;

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_event_hdr->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_config_protection_info_syn_event::mac_res_get_mac_vap fail.vap_id:%u}",
                       pst_event_hdr->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memcpy_s((uint8_t *)&pst_mac_vap->st_protection, OAL_SIZEOF(mac_protection_stru),
             (uint8_t *)&pst_h2d_prot->st_protection, OAL_SIZEOF(mac_protection_stru));

    mac_mib_set_HtProtection(pst_mac_vap, pst_h2d_prot->en_dot11HTProtection);
    mac_mib_set_RifsMode(pst_mac_vap, pst_h2d_prot->en_dot11RIFSMode);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_ch_status_info_syn_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    mac_ap_ch_info_stru *past_ap_ch_list = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ch_status_info_syn_event::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼����¼�ͷ�Լ��¼�payload�ṹ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    past_ap_ch_list = (mac_ap_ch_info_stru *)pst_event->auc_event_data;

    pst_mac_device = mac_res_get_dev(pst_event_hdr->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_ch_status_info_syn_event::mac_res_get_mac_vap fail.vap_id:%u}",
                       pst_event_hdr->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (EOK != memcpy_s(pst_mac_device->st_ap_channel_list, OAL_SIZEOF(mac_ap_ch_info_stru) * MAC_MAX_SUPP_CHANNEL,
                        (uint8_t *)past_ap_ch_list, OAL_SIZEOF(mac_ap_ch_info_stru) * MAC_MAX_SUPP_CHANNEL)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_ch_status_info_syn_event::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC void hmac_drx_event_subtable_register(void)
{
    /* ע��HMACģ��WLAN_DRX�¼��ӱ� */
    g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_RX_DATA].p_func =
        hmac_rx_process_data_event;

    g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_WOW_RX_DATA].p_func =
        hmac_rx_process_ring_data_event;

    /* AP ��STA ������ע��HMACģ��WLAN_DRX�¼��ӱ� */
    g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_TKIP_MIC_FAILE].p_func =
        hmac_rx_tkip_mic_failure_process;
}

OAL_STATIC void hmac_crx_event_subtable_register(void)
{
    /* ע��HMACģ��WLAN_CRX�¼��ӱ� */
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_INIT].p_func = hmac_init_event_process;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX].p_func = hmac_rx_process_mgmt_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX_WOW].p_func =
        hmac_rx_process_wow_mgmt_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_DELBA].p_func = hmac_mgmt_rx_delba_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT].p_func =
        hmac_scan_proc_scanned_bss;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT_RING].p_func =
        hmac_scan_proc_scanned_bss;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_SCAN_COMP].p_func =
        hmac_scan_proc_scan_comp_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_CHAN_RESULT].p_func =
        hmac_scan_process_chan_result_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_DISASSOC].p_func =
        hmac_mgmt_send_disasoc_deauth_event;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_DEAUTH].p_func =
        hmac_mgmt_send_disasoc_deauth_event;
}

OAL_STATIC void hmac_ctx_event_subtable_register(void)
{
    /* ע�ᷢ��HOST��������¼��ӱ� */
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_UP_REG_VAL].p_func = hmac_sdt_up_reg_val;

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_UP_SAMPLE_DATA].p_func = hmac_sdt_up_sample_data;
#endif
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_CREATE_BA].p_func = hmac_create_ba_event;
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_DEL_BA].p_func = hmac_del_ba_event;
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_CFG].p_func = hmac_event_config_syn;
}


OAL_STATIC void hmac_event_fsm_action_subtable_register(void)
{
    hmac_drx_event_subtable_register();

    /* ���¼����ͺ͵��ú���������ע�ᵽ�¼�����ģ�� */
    /* ע��WLAN_DTX�¼��ӱ� */
    g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX].p_func = hmac_tx_wlan_to_wlan_ap;

    hmac_crx_event_subtable_register();

    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPR_CH_SWITCH_COMPLETE].p_func =
        hmac_chan_switch_to_new_chan_complete;
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPR_DBAC].p_func = hmac_dbac_status_notify;

    /* ע�ᷢ��HOST��������¼��ӱ� */
    hmac_ctx_event_subtable_register();

#ifdef _PRE_WLAN_FEATURE_DFS
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_RADAR_DETECT].p_func = hmac_dfs_radar_detect_event;
#endif /* end of _PRE_WLAN_FEATURE_DFS */
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_DISASOC].p_func = hmac_proc_disasoc_misc_event;

    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_ROAM_TRIGGER].p_func = hmac_proc_roam_trigger_event;

    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_ALG_INFO_SYN].p_func = hmac_syn_info_event;
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_VOICE_AGGR].p_func = hmac_voice_aggr_event;

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_CALI_TO_HMAC].p_func = wlan_chip_save_cali_event;
    g_ast_hmac_ddr_drx_sub_table[HAL_WLAN_DDR_DRX_EVENT_SUBTYPE].p_func = hmac_rx_host_ring_data_event;

#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
    g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_TX].p_func = hmac_tx_complete_event_handler;
#endif
#ifdef _PRE_WLAN_ONLINE_DPD
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_TO_HMAC_DPD].p_func = hmac_sdt_up_dpd_data;
#endif
#ifdef _PRE_WLAN_FEATURE_APF
    g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_APF_REPORT].p_func = hmac_apf_program_report_event;
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    g_ast_hmac_wlan_misc_event_sub_table[HAL_EVENT_HAL_RX_RESET_SMAC].p_func = hal_host_rx_reset_smac_handler;
#endif
#ifdef _PRE_WLAN_FEATURE_M2S
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_M2S_DATA].p_func = hmac_m2s_sync_event;
#endif

    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_BANDWIDTH_INFO_SYN].p_func = hmac_bandwidth_info_syn_event;
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_PROTECTION_INFO_SYN].p_func = hmac_protection_info_syn_event;
    g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_CH_STATUS_INFO_SYN].p_func = hmac_ch_status_info_syn_event;
}


OAL_STATIC void hmac_event_fsm_action_subtable_register_ext(void)
{
#ifdef _PRE_WLAN_FEATURE_HIEX
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_HIEX_D2H_MSG].p_func =
        g_wlan_spec_cfg->feature_hiex_is_open ? hmac_hiex_rx_local_msg : OAL_PTR_NULL;
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
    g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_TX_RING_ADDR].p_func =
        hmac_set_tx_ring_device_base_addr;
#endif
}


uint32_t hmac_event_fsm_register(void)
{
    /* ע�������¼���tx adapt�ӱ� */
    hmac_event_fsm_tx_adapt_subtable_register();
    hmac_event_fsm_tx_adapt_subtable_register_ext();

    /* ע�������¼���rx adapt�ӱ� */
    hmac_event_fsm_rx_adapt_subtable_register();

    /* ע�������¼���ִ�к����ӱ� */
    hmac_event_fsm_action_subtable_register();
    hmac_event_fsm_action_subtable_register_ext();

    event_fsm_table_register();

    return OAL_SUCC;
}


int32_t hmac_param_check(void)
{
    /* netbuf's cb size! */
    uint32_t ul_netbuf_cb_size = (uint32_t)oal_netbuf_cb_size();
    if (ul_netbuf_cb_size < (uint32_t)OAL_SIZEOF(mac_tx_ctl_stru)) {
        oal_io_print("mac_tx_ctl_stru size[%u] large then netbuf cb max size[%u]\n",
                     ul_netbuf_cb_size, (uint32_t)OAL_SIZEOF(mac_tx_ctl_stru));
        return OAL_EFAIL;
    }

    if (ul_netbuf_cb_size < (uint32_t)OAL_SIZEOF(mac_rx_ctl_stru)) {
        oal_io_print("mac_rx_ctl_stru size[%u] large then netbuf cb max size[%u]\n",
                     ul_netbuf_cb_size, (uint32_t)OAL_SIZEOF(mac_rx_ctl_stru));
        return OAL_EFAIL;
    }
    return OAL_SUCC;
}

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
/* debug sysfs */
extern int32_t hmac_tx_event_pkts_info_print(void *data, char *buf, int32_t buf_len);
OAL_STATIC oal_kobject *g_conn_syfs_hmac_object = NULL;
OAL_STATIC int32_t hmac_print_vap_stat(void *data, char *buf, int32_t buf_len)
{
    int32_t ret = 0;
    uint8_t uc_vap_id;
    oal_net_device_stru *pst_net_device = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == NULL) {
            continue;
        }
        ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1, "vap %2u info:\n", uc_vap_id);
        ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1,
                          "vap_state %2u, protocol:%2u, user nums:%2u,init:%u\n",
                          pst_hmac_vap->st_vap_base_info.en_vap_state,
                          pst_hmac_vap->st_vap_base_info.en_protocol,
                          pst_hmac_vap->st_vap_base_info.us_user_nums,
                          pst_hmac_vap->st_vap_base_info.uc_init_flag);
        pst_net_device = pst_hmac_vap->pst_net_device;
        if (pst_net_device != NULL) {
            int32_t i;
            ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1,
                              "net name:%s\n", netdev_name(pst_net_device));
            ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1,
                              "tx [%d]queues info, state [bit0:DRV_OFF], [bit1:STACK_OFF], [bit2:FROZEN]\n",
                              pst_net_device->num_tx_queues);
            for (i = 0; i < pst_net_device->num_tx_queues; i++) {
                struct netdev_queue *txq = netdev_get_tx_queue(pst_net_device, i);
                if (txq->state) {
                    ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1,
                                      "net queue[%2d]'s state:0x%lx\n", i, txq->state);
                }
            }
        }
        ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1, "\n");
    }

    return ret;
}

OAL_STATIC ssize_t hmac_get_vap_stat(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += hmac_print_vap_stat(NULL, buf, PAGE_SIZE - ret);
    return ret;
}
OAL_STATIC struct kobj_attribute dev_attr_vap_info =
    __ATTR(vap_info, S_IRUGO, hmac_get_vap_stat, NULL);

OAL_STATIC ssize_t hmac_get_adapt_info(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += hmac_tx_event_pkts_info_print(NULL, buf + ret, PAGE_SIZE - ret);
    return ret;
}

int32_t hmac_wakelock_info_print(char *buf, int32_t buf_len)
{
    int32_t ret = 0;

#ifdef CONFIG_PRINTK
    if (g_st_hmac_wakelock.locked_addr) {
        ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1, "wakelocked by:%pf\n",
                          (void *)g_st_hmac_wakelock.locked_addr);
    }
#endif

    ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1,
                      "hold %lu locks\n", g_st_hmac_wakelock.lock_count);

    return ret;
}

OAL_STATIC ssize_t hmac_get_wakelock_info(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += hmac_wakelock_info_print(buf, PAGE_SIZE - ret);

    return ret;
}

OAL_STATIC struct kobj_attribute dev_attr_adapt_info =
    __ATTR(adapt_info, S_IRUGO, hmac_get_adapt_info, NULL);
OAL_STATIC struct kobj_attribute dev_attr_wakelock =
    __ATTR(wakelock, S_IRUGO, hmac_get_wakelock_info, NULL);

OAL_STATIC ssize_t hmac_show_roam_status(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int32_t ret = 0;
    uint8_t uc_vap_id;
    uint8_t uc_roming_now = 0;
    hmac_vap_stru *pst_hmac_vap = NULL;
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_user_stru *pst_hmac_user_multi;
#endif
    hmac_user_stru *pst_hmac_user = NULL;

    if (buf == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{buf is NULL.}");
        return ret;
    }

    if ((dev == OAL_PTR_NULL) || (attr == OAL_PTR_NULL)) {
        ret += snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "roam_status=0\n");

        return ret;
    }

    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            continue;
        }

        if ((pst_hmac_vap->st_vap_base_info.en_vap_mode != WLAN_VAP_MODE_BSS_STA)
            || (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_BUTT)) {
            continue;
        }

        if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
            uc_roming_now = 1;
            break;
        }

#ifdef _PRE_WLAN_FEATURE_WAPI
        /* wapi�£���roam��־��Ϊ1����ֹarp̽�� */
        pst_hmac_user_multi =
            (hmac_user_stru *)mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_multi_user_idx);
        if (pst_hmac_user_multi == OAL_PTR_NULL) {
            continue;
        }

        if (pst_hmac_user_multi->st_wapi.uc_port_valid == OAL_TRUE) {
            uc_roming_now = 1;
            break;
        }
#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        if (pst_hmac_user == OAL_PTR_NULL) {
            continue;
        }

        if (OAL_TRUE == hmac_btcoex_check_by_ba_size(pst_hmac_user)) {
            uc_roming_now = 1;
        }
    }
    /* �ȳ�һ���汾ǿ�ƹر�arp̽�⣬������Ч�� */
    // uc_roming_now = 1;
    ret += snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "roam_status=%1d\n", uc_roming_now);

    return ret;
}

OAL_STATIC struct kobj_attribute dev_attr_roam_status =
    __ATTR(roam_status, S_IRUGO, hmac_show_roam_status, NULL);

OAL_STATIC ssize_t hmac_set_rxthread_enable(struct kobject *dev, struct kobj_attribute *attr,
                                            const char *buf, size_t count)
{
    uint32_t ul_val;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if ((sscanf_s(buf, "%u", &ul_val) != 1)) {
        oal_io_print("set value one char!\n");
        return -OAL_EINVAL;
    }

    g_st_rxdata_thread.en_rxthread_enable = (oal_bool_enum_uint8)ul_val;

    return count;
}
OAL_STATIC ssize_t hmac_get_rxthread_info(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "rxthread_enable=%d\nrxthread_queue_len=%d\nrxthread_pkt_loss=%d\n",
                      g_st_rxdata_thread.en_rxthread_enable,
                      oal_netbuf_list_len(&g_st_rxdata_thread.st_rxdata_netbuf_head),
                      g_st_rxdata_thread.ul_pkt_loss_cnt);

    return ret;
}
OAL_STATIC struct kobj_attribute dev_attr_rxdata_info =
    __ATTR(rxdata_info, S_IRUGO | S_IWUSR, hmac_get_rxthread_info, hmac_set_rxthread_enable);

OAL_STATIC struct attribute *hmac_sysfs_entries[] = {
    &dev_attr_vap_info.attr,
    &dev_attr_adapt_info.attr,
    &dev_attr_wakelock.attr,
    &dev_attr_roam_status.attr,
    &dev_attr_rxdata_info.attr,
    NULL
};

OAL_STATIC struct attribute_group hmac_attribute_group = {
    .name = "vap",
    .attrs = hmac_sysfs_entries,
};

OAL_STATIC int32_t hmac_sysfs_entry_init(void)
{
    int32_t ret;
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sysfs_entry_init::get sysfs root object failed!}");
        return -OAL_EFAIL;
    }

    g_conn_syfs_hmac_object = kobject_create_and_add("hmac", pst_root_object);
    if (g_conn_syfs_hmac_object == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sysfs_entry_init::create hmac object failed!}");
        return -OAL_EFAIL;
    }

    ret = sysfs_create_group(g_conn_syfs_hmac_object, &hmac_attribute_group);
    if (ret) {
        kobject_put(g_conn_syfs_hmac_object);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sysfs_entry_init::sysfs create group failed!}");
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t hmac_sysfs_entry_exit(void)
{
    if (g_conn_syfs_hmac_object) {
        sysfs_remove_group(g_conn_syfs_hmac_object, &hmac_attribute_group);
        kobject_put(g_conn_syfs_hmac_object);
    }
    return OAL_SUCC;
}
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
OAL_STATIC declare_wifi_panic_stru(hmac_panic_vap_stat, hmac_print_vap_stat);
#endif
oal_bool_enum_uint8 hmac_get_pm_pause_func(void)
{
    hmac_device_stru *pst_hmac_device = NULL;

    uint8_t uc_vap_idx;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_bool_enum_uint8 uc_is_any_cnt_exceed_limit = OAL_FALSE;
    oal_bool_enum_uint8 uc_is_any_timer_registerd = OAL_FALSE;
    /* ��ȡmac device�ṹ��ָ�� */
    pst_hmac_device = hmac_res_get_mac_dev(0);

    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_get_pm_pause_func::pst_device null.}");
        return OAL_FALSE;
    }

    if (pst_hmac_device->st_scan_mgmt.en_is_scanning == OAL_TRUE) {
        oam_info_log0(0, OAM_SF_ANY, "{hmac_get_pm_pause_func::in scanning}");
        return OAL_TRUE;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_hmac_device->pst_device_base_info->uc_vap_num; uc_vap_idx++) {
        pst_hmac_vap =
            (hmac_vap_stru *)mac_res_get_hmac_vap(pst_hmac_device->pst_device_base_info->auc_vap_id[uc_vap_idx]);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            oam_error_log0(0, OAM_SF_CFG, "{hmac_get_pm_pause_func::pst_hmac_vap null.}");
            return OAL_FALSE;
        }

        if (pst_hmac_vap->st_ps_sw_timer.en_is_registerd == OAL_TRUE) {
            uc_is_any_timer_registerd = OAL_TRUE;
            pst_hmac_vap->us_check_timer_pause_cnt++;
            if (pst_hmac_vap->us_check_timer_pause_cnt % 1000 == 0) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                                 "{hmac_get_pm_pause_func::g_uc_check_timer_cnt[%d]",
                                 pst_hmac_vap->us_check_timer_pause_cnt);
            }

            if (pst_hmac_vap->us_check_timer_pause_cnt > HMAC_SWITCH_STA_PSM_MAX_CNT) {
                uc_is_any_cnt_exceed_limit = OAL_TRUE;
                oam_error_log2(0, OAM_SF_CFG, "{hmac_get_pm_pause_func::sw ps timer cnt too large[%d]> max[%d]}",
                               pst_hmac_vap->us_check_timer_pause_cnt, HMAC_SWITCH_STA_PSM_MAX_CNT);
            }
        } else {
            if (pst_hmac_vap->us_check_timer_pause_cnt != 0) {
                oam_warning_log2(0, OAM_SF_CFG, "{hmac_get_pm_pause_func::g_uc_check_timer_cnt end[%d],max[%d]",
                                 pst_hmac_vap->us_check_timer_pause_cnt, HMAC_SWITCH_STA_PSM_MAX_CNT);
            }

            pst_hmac_vap->us_check_timer_pause_cnt = 0;
        }
    }

    if (uc_is_any_cnt_exceed_limit) {
        return OAL_FALSE;
    } else if (uc_is_any_timer_registerd) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }

    return OAL_FALSE;
}

void hmac_register_pm_callback(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

    struct wifi_srv_callback_handler *pst_wifi_srv_handler;

    pst_wifi_srv_handler = wlan_pm_get_wifi_srv_handler();

    if (pst_wifi_srv_handler != OAL_PTR_NULL) {
        pst_wifi_srv_handler->p_wifi_srv_get_pm_pause_func = hmac_get_pm_pause_func;
        pst_wifi_srv_handler->p_wifi_srv_open_notify = hmac_wifi_state_notify;
        pst_wifi_srv_handler->p_wifi_srv_pm_state_notify = hmac_wifi_pm_state_notify;
    } else {
        oal_io_print("hmac_register_pm_callback:wlan_pm_get_wifi_srv_handler is null\n");
    }

#endif
}

oal_bool_enum_uint8 hmac_get_rxthread_enable(void)
{
    return g_st_rxdata_thread.en_rxthread_enable;
}

void hmac_rxdata_sched(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    up(&g_st_rxdata_thread.st_rxdata_sema);
#endif
    return;
}

void hmac_rxdata_update_napi_weight(oal_netdev_priv_stru *pst_netdev_priv)
{
#if defined(CONFIG_ARCH_HISI)

    uint32_t ul_now;
    uint8_t uc_new_napi_weight;

    /* ����ppsˮ�ߵ���napi weight,��������1s */
    ul_now = (uint32_t)oal_time_get_stamp_ms();
    if (oal_time_get_runtime(pst_netdev_priv->ul_period_start, ul_now) > NAPI_STAT_PERIOD) {
        pst_netdev_priv->ul_period_start = ul_now;
        if (pst_netdev_priv->ul_period_pkts < NAPI_WATER_LINE_LEV1) {
            uc_new_napi_weight = NAPI_POLL_WEIGHT_LEV1;
        } else if (pst_netdev_priv->ul_period_pkts < NAPI_WATER_LINE_LEV2) {
            uc_new_napi_weight = NAPI_POLL_WEIGHT_LEV2;
        } else if (pst_netdev_priv->ul_period_pkts < NAPI_WATER_LINE_LEV3) {
            uc_new_napi_weight = NAPI_POLL_WEIGHT_LEV3;
        } else {
            uc_new_napi_weight = NAPI_POLL_WEIGHT_MAX;
        }
        pst_netdev_priv->ul_period_pkts = 0;
        if (uc_new_napi_weight != pst_netdev_priv->uc_napi_weight) {
            pst_netdev_priv->uc_napi_weight = uc_new_napi_weight;
            pst_netdev_priv->st_napi.weight = uc_new_napi_weight;
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_rxdata_update_napi_weight::new_napi_weight [%d]",
                             uc_new_napi_weight);
        }
    }
#endif
}


OAL_STATIC void hmac_rxdata_netbuf_delist(oal_netdev_priv_stru *netdev_priv)
{
    oal_netbuf_stru *netbuf = OAL_PTR_NULL;

    netbuf = oal_netbuf_delist(&netdev_priv->st_rx_netbuf_queue);
    while (netbuf != OAL_PTR_NULL) {
#ifdef _PRE_WLAN_PKT_TIME_STAT
        if (DELAY_STATISTIC_SWITCH_ON && IS_NEED_RECORD_DELAY(netbuf, TP_SKB_HMAC_RX)) {
            skbprobe_record_time(netbuf, TP_SKB_HMAC_UPLOAD);
        }
#endif
        oal_notice_netif_rx(netbuf);
        oal_netif_rx_ni(netbuf);
        netbuf = oal_netbuf_delist(&netdev_priv->st_rx_netbuf_queue);
    }
}


OAL_STATIC void hmac_rxdata_napi_shedule_process(oal_netdev_priv_stru *netdev_priv)
{
#ifdef _PRE_CONFIG_NAPI_DYNAMIC_SWITCH
    /* RR�����Ż�����napiȨ��Ϊ1ʱ������napi��ѯ���� */
    if (netdev_priv->uc_napi_weight <= NAPI_POLL_WEIGHT_LEV1) {
        hmac_rxdata_netbuf_delist(netdev_priv);
    } else {
        oal_napi_schedule(&netdev_priv->st_napi);
    }
#else
    oal_napi_schedule(&netdev_priv->st_napi);
#endif
}

void hmac_rxdata_netbuf_enqueue(oal_netbuf_stru *pst_netbuf)
{
    oal_netdev_priv_stru *pst_netdev_priv;

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_netbuf->dev);
    if (pst_netdev_priv->ul_queue_len_max < oal_netbuf_list_len(&pst_netdev_priv->st_rx_netbuf_queue)) {
        oal_netbuf_free(pst_netbuf);
        // TBD stats for netdev
        return;
    }

    oal_netbuf_list_tail(&pst_netdev_priv->st_rx_netbuf_queue, pst_netbuf);

    pst_netdev_priv->ul_period_pkts++;
}

OAL_STATIC int32_t hmac_rxdata_thread(void *p_data)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint8_t uc_vap_idx;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_netdev_priv_stru *pst_netdev_priv = OAL_PTR_NULL;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct sched_param param;

    param.sched_priority = 97;
    oal_set_thread_property(current, SCHED_FIFO, &param, -10);

    allow_signal(SIGTERM);
    while (oal_likely(!down_interruptible(&g_st_rxdata_thread.st_rxdata_sema)))
#else
    for (;;)
#endif
    {
        if (oal_kthread_should_stop()) {
            break;
        }

        pst_mac_device = mac_res_get_dev(0);
        for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
            pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
            if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
                continue;
            }

            if (pst_hmac_vap->pst_net_device == OAL_PTR_NULL) {
                continue;
            }

            pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_hmac_vap->pst_net_device);
            if (0 == oal_netbuf_list_len(&pst_netdev_priv->st_rx_netbuf_queue)) {
                continue;
            }

            if (pst_netdev_priv->uc_napi_enable == OAL_TRUE) {
                hmac_rxdata_update_napi_weight(pst_netdev_priv);
                hmac_rxdata_napi_shedule_process(pst_netdev_priv);
            } else {
                hmac_rxdata_netbuf_delist(pst_netdev_priv);
            }
        }
    }
    return OAL_SUCC;
}

int32_t hmac_rxdata_polling(struct napi_struct *pst_napi, int32_t l_weight)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    oal_netdev_priv_stru *pst_netdev_priv;
    int32_t l_rx_num = 0;
    oal_netbuf_head_stru *netbuf_hdr;
    int32_t l_rx_max = l_weight;  // Ĭ����������Ϊl_weight

    pst_netdev_priv = oal_container_of(pst_napi, oal_netdev_priv_stru, st_napi);
    netbuf_hdr = &pst_netdev_priv->st_rx_netbuf_queue;

    /* ��rx_data�߳��е���ˣ��������ж�����С��ʱ�����100%���ں�bug */
    /* �����Ҫ������ֳ�������ǰ�����������жϣ������ں��ڴ�����������ж� */
    /* ���ϱ�һ֡���������ж���Ϊû�и��౨�ĴӶ���ֹrepoll */
#if defined(CONFIG_ARCH_HISI)
#ifdef CONFIG_NR_CPUS
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    if (g_st_rxdata_thread.uc_allowed_cpus == WLAN_IRQ_AFFINITY_BUSY_CPU) {
        if (get_cpu() < OAL_BUS_HPCPU_NUM) {
            l_rx_max = l_weight - 1;
        }
        put_cpu();
    }
#endif
#endif
#endif

    while (l_rx_num < l_rx_max) {
        pst_netbuf = oal_netbuf_delist(&pst_netdev_priv->st_rx_netbuf_queue);

        if (pst_netbuf == OAL_PTR_NULL) {
            break;
        }

#ifdef _PRE_WLAN_PKT_TIME_STAT
        if (DELAY_STATISTIC_SWITCH_ON && IS_NEED_RECORD_DELAY(pst_netbuf, TP_SKB_HMAC_RX)) {
            skbprobe_record_time(pst_netbuf, TP_SKB_HMAC_UPLOAD);
        }
#endif

        if (pst_netdev_priv->uc_gro_enable == OAL_TRUE) {
            oal_napi_gro_receive(pst_napi, pst_netbuf);
        } else {
            oal_netif_receive_skb(pst_netbuf);
        }

        l_rx_num++;
    }

    if (l_rx_num < l_weight) {
        oal_napi_complete(pst_napi);
        if (l_rx_num == l_rx_max) {
            hmac_rxdata_sched();  // �ں���Ϊ�Ѿ�û���ˣ�������Ҫ���µ���һ��
        }
    }

    return l_rx_num;
}

OAL_STATIC uint32_t hmac_hisi_thread_init(void)
{
#if defined(_PRE_WLAN_TCP_OPT) && !defined(WIN32)
    hmac_set_hmac_tcp_ack_process_func(hmac_tcp_ack_process);
    hmac_set_hmac_tcp_ack_need_schedule(hmac_tcp_ack_need_schedule);

    hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread = oal_thread_create(hcc_transfer_thread,
        hcc_get_110x_handler(), NULL, "hisi_hcc", HCC_TRANS_THREAD_POLICY, HCC_TRANS_THERAD_PRIORITY, -1);
    if (!hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread) {
        oal_io_print("hcc thread create failed!\n");
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
        mutex_destroy(&hcc_get_110x_handler()->tx_transfer_lock);
#endif
        return OAL_FAIL;
    }
#endif

    oal_wait_queue_init_head(&g_st_rxdata_thread.st_rxdata_wq);

    oal_netbuf_list_head_init(&g_st_rxdata_thread.st_rxdata_netbuf_head);
    oal_spin_lock_init(&g_st_rxdata_thread.st_lock);
    g_st_rxdata_thread.en_rxthread_enable = OAL_TRUE;
    g_st_rxdata_thread.ul_pkt_loss_cnt = 0;
    g_st_rxdata_thread.uc_allowed_cpus = 0;

    g_st_rxdata_thread.pst_rxdata_thread = oal_thread_create(hmac_rxdata_thread,
                                                                     NULL,
                                                                     &g_st_rxdata_thread.st_rxdata_sema,
                                                                     "hisi_rxdata",
                                                                     SCHED_FIFO,
                                                                     98,
                                                                     -1);

#ifndef WIN32
    if (g_st_rxdata_thread.pst_rxdata_thread == OAL_PTR_NULL) {
        oal_io_print("hisi_rxdata thread create failed!\n");
        oal_thread_stop(hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread, NULL);
        hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread = NULL;
        return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC void hmac_hisi_thread_exit(void)
{
#if defined(_PRE_WLAN_TCP_OPT) && !defined(WIN32)
    if (hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread) {
        oal_thread_stop(hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread, NULL);
        hcc_get_110x_handler()->hcc_transer_info.hcc_transfer_thread = NULL;
    }
#endif
    if (g_st_rxdata_thread.pst_rxdata_thread != OAL_PTR_NULL) {
        oal_thread_stop(g_st_rxdata_thread.pst_rxdata_thread, &g_st_rxdata_thread.st_rxdata_sema);
        g_st_rxdata_thread.pst_rxdata_thread = NULL;
    }
}

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#define MAX_CPU_FREQ_LIMIT 2900000


uint32_t hisi_cpufreq_get_maxfreq(unsigned int cpu)
{
    struct cpufreq_policy *policy = cpufreq_cpu_get_raw(cpu);
    uint32_t ret_freq;
    int32_t idx;

    if (!policy) {
        oam_error_log0(0, OAM_SF_ANY, "hisi_cpufreq_get_maxfreq: get cpufreq policy fail!");
        return 0;
    }

    ret_freq = policy->cpuinfo.max_freq;
    if (ret_freq > MAX_CPU_FREQ_LIMIT && policy->freq_table != NULL) {
        idx = cpufreq_frequency_table_target(policy, MAX_CPU_FREQ_LIMIT, CPUFREQ_RELATION_H);
        if (idx >= 0) {
            ret_freq = (policy->freq_table + idx)->frequency;
            oam_warning_log1(0, OAM_SF_ANY, "{hisi_cpufreq_get_maxfreq::limit freq:%u}", ret_freq);
        }
    }
    cpufreq_cpu_put(policy);
    return ret_freq;
}

OAL_STATIC uint32_t hmac_hisi_cpufreq_init(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    uint8_t uc_cpuid;

    if (g_freq_lock_control.uc_lock_max_cpu_freq == OAL_FALSE) {
        return OAL_SUCC;
    }

    memset_s(&g_aul_cpumaxfreq, sizeof(g_aul_cpumaxfreq), 0, sizeof(g_aul_cpumaxfreq));

    for (uc_cpuid = 0; uc_cpuid < OAL_BUS_MAXCPU_NUM; uc_cpuid++) {
        /* ��Ч��CPU id�򷵻�1,���򷵻�0 */
        if (cpu_possible(uc_cpuid) == 0) {
            continue;
        }

        hisi_cpufreq_init_req(&g_ast_cpufreq[uc_cpuid], uc_cpuid);
        g_aul_cpumaxfreq[uc_cpuid].max_cpu_freq = hisi_cpufreq_get_maxfreq(uc_cpuid);
        g_aul_cpumaxfreq[uc_cpuid].valid = OAL_TRUE;
    }

    pm_qos_add_request(&g_st_pmqos_requset, PM_QOS_CPU_DMA_LATENCY, PM_QOS_DEFAULT_VALUE);
#endif
    return OAL_SUCC;
}
OAL_STATIC uint32_t hmac_hisi_cpufreq_exit(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    uint8_t uc_cpuid;

    if (g_freq_lock_control.uc_lock_max_cpu_freq == OAL_FALSE) {
        return OAL_SUCC;
    }

    for (uc_cpuid = 0; uc_cpuid < OAL_BUS_MAXCPU_NUM; uc_cpuid++) {
        /* δ��ȡ����ȷ��cpuƵ�������� */
        if (g_aul_cpumaxfreq[uc_cpuid].valid != OAL_TRUE) {
            continue;
        }

        hisi_cpufreq_exit_req(&g_ast_cpufreq[uc_cpuid]);
        g_aul_cpumaxfreq[uc_cpuid].valid = OAL_FALSE;
    }
    pm_qos_remove_request(&g_st_pmqos_requset);
#endif
    return OAL_SUCC;
}
#endif


int32_t hmac_main_init(void)
{
    uint32_t ul_return;
    frw_init_enum_uint16 en_init_state;
    oal_wake_lock_init(&g_st_hmac_wakelock, "wlan_hmac_wakelock");

    /* Ϊ�˽��ģ�������ʱ�䣬����ʱ�����ӡ TBD */
    if (OAL_SUCC != hmac_param_check()) {
        oal_io_print("hmac_main_init:hmac_param_check failed!\n");
        return -OAL_EFAIL;  //lint !e527
    }

    hmac_hcc_adapt_init();

    en_init_state = frw_get_init_state();

    if (oal_unlikely((en_init_state == FRW_INIT_STATE_BUTT) || (en_init_state < FRW_INIT_STATE_FRW_SUCC))) {
        oal_io_print("hmac_main_init:en_init_state is error %d\n", en_init_state);
        frw_timer_delete_all_timer();
        return -OAL_EFAIL;  //lint !e527
    }
    hmac_wifi_auto_freq_ctrl_init();

    ul_return = hmac_hisi_thread_init();
    if (ul_return != OAL_SUCC) {
        frw_timer_delete_all_timer();
        oal_io_print("hmac_main_init: hmac_hisi_thread_init failed\n");

        return -OAL_EFAIL;
    }

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    hmac_hisi_cpufreq_init();
#endif

    ul_return = mac_res_init();
    if (ul_return != OAL_SUCC) {
        oal_io_print("hmac_main_init: mac_res_init return err code %d\n", ul_return);
        frw_timer_delete_all_timer();
        return -OAL_EFAIL;
    }

    /* hmac��Դ��ʼ�� */
    hmac_res_init();
    memset_s(g_hmac_lut_index_tbl, sizeof(hmac_lut_index_tbl_stru) * HAL_MAX_TX_BA_LUT_SIZE, 0,
        sizeof(hmac_lut_index_tbl_stru) * HAL_MAX_TX_BA_LUT_SIZE);

    /* �����ʼ��״̬��������VAP�ɹ�ǰ��״̬�������˴�ΪHMAC��һ�γ�ʼ�������ؼ��ػ�������ʼ�� */
    if (en_init_state < FRW_INIT_STATE_HMAC_CONFIG_VAP_SUCC) {
        /* ����״̬����ʼ���ӿ� */
        hmac_fsm_init();

        /* �¼�ע�� */
        hmac_event_fsm_register();

        ul_return = hmac_board_init(g_pst_mac_board);
        if (ul_return != OAL_SUCC) {
            frw_timer_delete_all_timer();
            event_fsm_unregister();
            mac_res_exit();
            hmac_res_exit(g_pst_mac_board); /* �ͷ�hmac res��Դ */
            return OAL_FAIL;
        }

        frw_set_init_state(FRW_INIT_STATE_HMAC_CONFIG_VAP_SUCC);

        /* �����ɹ��������ӡ */
    } else {
        /* TBD ����10 �������޸� �����ʼ��״̬Ϊ����VAP�ɹ����״̬�����������Ϊ���ô����ĳ�ʼ����
           ��Ҫ��������ҵ��VAP���������״̬����δ��ʼ��������Ҫ��ʼ�����������
           ��֧�����Խӿڹҽ� */
    }

    /* DFX ģ���ʼ�� */
    hmac_dfx_init();

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
    hmac_sysfs_entry_init();
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_register(&hmac_panic_vap_stat, NULL);
#endif

    hmac_register_pm_callback();
    return OAL_SUCC;
}


void hmac_main_exit(void)
{
    uint32_t ul_return;

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
    hmac_sysfs_entry_exit();
#endif
    event_fsm_unregister();

    hmac_hisi_thread_exit();

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    hmac_hisi_cpufreq_exit();
#endif

    ul_return = hmac_board_exit(g_pst_mac_board);
    if (oal_unlikely(ul_return != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_main_exit::hmac_board_exit failed[%d].}", ul_return);
        return;
    }

    /* DFX ģ���ʼ�� */
    hmac_dfx_exit();

    hmac_wifi_auto_freq_ctrl_deinit();

    frw_set_init_state(FRW_INIT_STATE_FRW_SUCC);

    oal_wake_lock_exit(&g_st_hmac_wakelock);

    frw_timer_clean_timer(OAM_MODULE_ID_HMAC);
}


uint32_t hmac_sdt_recv_reg_cmd(mac_vap_stru *pst_mac_vap,
                                     uint8_t *puc_buf,
                                     uint16_t us_len)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event = OAL_PTR_NULL;

    pst_event_mem = frw_event_alloc_m(us_len - OAL_IF_NAME_SIZE);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sdt_recv_reg_cmd::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CRX,
                       HMAC_TO_DMAC_SYN_REG,
                       (uint16_t)(us_len - OAL_IF_NAME_SIZE),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    if (EOK != memcpy_s(pst_event->auc_event_data, us_len - OAL_IF_NAME_SIZE,
                        puc_buf + OAL_IF_NAME_SIZE, us_len - OAL_IF_NAME_SIZE)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_sdt_recv_reg_cmd::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_sdt_up_reg_val(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t *pst_reg_val = OAL_PTR_NULL;

    pst_event = frw_get_event_stru(pst_event_mem);

    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_ANY, "{hmac_sdt_up_reg_val::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_reg_val = (uint32_t *)pst_hmac_vap->st_cfg_priv.ac_rsp_msg;
    *pst_reg_val = *((uint32_t *)pst_event->auc_event_data);

    /* ����wal_sdt_recv_reg_cmd�ȴ��Ľ��� */
    pst_hmac_vap->st_cfg_priv.en_wait_ack_for_sdt_reg = OAL_TRUE;
    oal_wait_queue_wake_up_interrupt(&(pst_hmac_vap->st_cfg_priv.st_wait_queue_for_sdt_reg));

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_ONLINE_DPD

uint32_t hmac_sdt_up_dpd_data(frw_event_mem_stru *pst_event_mem)
{
    uint16_t us_payload_len;
    frw_event_stru *pst_event;
    uint8_t *puc_payload;
    frw_event_hdr_stru *pst_event_hdr;
    hal_cali_hal2hmac_event_stru *pst_cali_save_event;

    int8_t *pc_print_buff;

    oam_error_log0(0, 0, "hmac_sdt_up_dpd_data");
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_sdt_up_sample_data::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_cali_save_event = (hal_cali_hal2hmac_event_stru *)pst_event->auc_event_data;

    puc_payload = (uint8_t *)oal_netbuf_data(pst_cali_save_event->pst_netbuf);

    pst_event_hdr = &(pst_event->st_event_hdr);
    us_payload_len = OAL_STRLEN(puc_payload);

    oam_error_log1(0, 0, "hmac dpd payload len %d", us_payload_len);

    pc_print_buff = (int8_t *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_sdt_up_sample_data::pc_print_buff null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????OAM_REPORT_MAX_STRING_LEN,???oam_print */
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    if (EOK != memcpy_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, puc_payload, us_payload_len)) {
        oam_error_log0(0, OAM_SF_WPA, "hmac_sdt_up_dpd_data::memcpy fail!");
        oal_mem_free_m(pc_print_buff, OAL_TRUE);
        return OAL_FAIL;
    }

    pc_print_buff[us_payload_len] = '\0';
    oam_print(pc_print_buff);

    oal_mem_free_m(pc_print_buff, OAL_TRUE);

    return OAL_SUCC;
}
#endif

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)

uint32_t hmac_sdt_recv_sample_cmd(mac_vap_stru *pst_mac_vap,
                                    uint8_t *puc_buf,
                                    uint16_t us_len)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event;

    pst_event_mem = frw_event_alloc_m(us_len - OAL_IF_NAME_SIZE);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sdt_recv_reg_cmd::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* ��д�¼�ͷ */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CRX,
                       HMAC_TO_DMAC_SYN_SAMPLE,
                       (uint16_t)(us_len - OAL_IF_NAME_SIZE),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    if (EOK != memcpy_s(pst_event->auc_event_data, us_len - OAL_IF_NAME_SIZE,
                        puc_buf + OAL_IF_NAME_SIZE, us_len - OAL_IF_NAME_SIZE)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_sdt_recv_sample_cmd::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_sdt_up_sample_data(frw_event_mem_stru *pst_event_mem)
{
    uint16_t us_payload_len;
    frw_event_stru *pst_event;
    frw_event_stru *pst_event_up;
    frw_event_mem_stru *pst_hmac_event_mem;
    frw_event_hdr_stru *pst_event_hdr;
    dmac_sdt_sample_frame_stru *pst_sample_frame;
    dmac_sdt_sample_frame_stru *pst_sample_frame_syn;
    frw_event_mem_stru *pst_syn_event_mem;
    frw_event_stru *pst_event_syn;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sdt_up_sample_data::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    us_payload_len = pst_event_hdr->us_length - OAL_SIZEOF(frw_event_hdr_stru);

    /* �׵�WAL */
    pst_hmac_event_mem = frw_event_alloc_m(us_payload_len);
    if (pst_hmac_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "{hmac_sdt_up_sample_data::pst_hmac_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event_up = frw_get_event_stru(pst_hmac_event_mem);

    frw_event_hdr_init(&(pst_event_up->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_SAMPLE_REPORT,
                       us_payload_len,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_event_hdr->uc_chip_id,
                       pst_event_hdr->uc_device_id,
                       pst_event_hdr->uc_vap_id);

    if (EOK != memcpy_s(pst_event_up->auc_event_data, us_payload_len,
                        (uint8_t *)frw_get_event_payload(pst_event_mem), us_payload_len)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_sdt_up_sample_data::memcpy fail!");
        frw_event_free_m(pst_hmac_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_hmac_event_mem);
    frw_event_free_m(pst_hmac_event_mem);

    pst_sample_frame = (dmac_sdt_sample_frame_stru *)pst_event->auc_event_data;

    if (pst_sample_frame->ul_count && pst_sample_frame->ul_count <= pst_sample_frame->ul_reg_num) {
        pst_syn_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_sdt_sample_frame_stru));
        if (pst_syn_event_mem == OAL_PTR_NULL) {
            oam_error_log0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "{hmac_sdt_up_sample_data::pst_syn_event_mem null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        pst_event_syn = frw_get_event_stru(pst_syn_event_mem);
        /* ��д�¼�ͷ */
        frw_event_hdr_init(&(pst_event_syn->st_event_hdr),
                           FRW_EVENT_TYPE_HOST_CRX,
                           HMAC_TO_DMAC_SYN_SAMPLE,
                           OAL_SIZEOF(dmac_sdt_sample_frame_stru),
                           FRW_EVENT_PIPELINE_STAGE_1,
                           pst_event_hdr->uc_chip_id,
                           pst_event_hdr->uc_device_id,
                           pst_event_hdr->uc_vap_id);

        pst_sample_frame_syn = (dmac_sdt_sample_frame_stru *)pst_event_syn->auc_event_data;
        pst_sample_frame_syn->ul_reg_num = pst_sample_frame->ul_reg_num;
        pst_sample_frame_syn->ul_count = pst_sample_frame->ul_count;
        pst_sample_frame_syn->ul_type = 0;
        frw_event_dispatch_event(pst_syn_event_mem);
        frw_event_free_m(pst_syn_event_mem);
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC uint32_t hmac_create_ba_event(frw_event_mem_stru *pst_event_mem)
{
    uint8_t uc_tidno;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    dmac_to_hmac_ctx_event_stru *pst_create_ba_event = OAL_PTR_NULL;

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_create_ba_event = (dmac_to_hmac_ctx_event_stru *)pst_event->auc_event_data;
    uc_tidno = pst_create_ba_event->uc_tid;

    pst_hmac_user = mac_res_get_hmac_user(pst_create_ba_event->us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_event->st_event_hdr.uc_vap_id, OAM_SF_ANY, "{hmac_create_ba_event::pst_hmac_user[%d] null.}",
                       pst_create_ba_event->us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_create_ba_event->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_event->st_event_hdr.uc_vap_id, OAM_SF_ANY, "{hmac_create_ba_event::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��tid�²�����BA�������������� */
    if (pst_hmac_user->ast_tid_info[uc_tidno].en_ba_handle_tx_enable == OAL_FALSE) {
        return OAL_FAIL;
    }

    hmac_tx_ba_setup(pst_hmac_vap, pst_hmac_user, uc_tidno);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_del_ba_event(frw_event_mem_stru *pst_event_mem)
{
    uint8_t uc_tid;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    mac_action_mgmt_args_stru st_action_args; /* ������дACTION֡�Ĳ��� */
    hmac_tid_stru *pst_hmac_tid = OAL_PTR_NULL;
    uint32_t ul_ret;
    dmac_to_hmac_ctx_event_stru *pst_del_ba_event;

    pst_event = frw_get_event_stru(pst_event_mem);

    pst_del_ba_event = (dmac_to_hmac_ctx_event_stru *)pst_event->auc_event_data;

    pst_hmac_user = mac_res_get_hmac_user(pst_del_ba_event->us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        /* dmac���¼���hmac��ɾ��ba����ʱhost������Ѿ�ɾ���û��ˣ���ʱ����������ֱ�ӷ��ؼ��� */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_del_ba_event::pst_hmac_user[%d] null.}",
                         pst_del_ba_event->us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_del_ba_event->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_del_ba_event::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_set_cur_protocol_mode(&pst_hmac_user->st_user_base_info, pst_del_ba_event->uc_cur_protocol);
    ul_ret = hmac_config_user_info_syn(&(pst_hmac_vap->st_vap_base_info), &(pst_hmac_user->st_user_base_info));
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    for (uc_tid = 0; uc_tid < WLAN_TID_MAX_NUM; uc_tid++) {
        pst_hmac_tid = &pst_hmac_user->ast_tid_info[uc_tid];

        if (pst_hmac_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                          "{hmac_del_ba_event::the tx hdl is not exist.}");
            continue;
        }

        st_action_args.uc_category = MAC_ACTION_CATEGORY_BA;
        st_action_args.uc_action = MAC_BA_ACTION_DELBA;
        st_action_args.ul_arg1 = uc_tid; /* ������֡��Ӧ��TID�� */
        /* ADDBA_REQ�У�buffer_size��Ĭ�ϴ�С */
        st_action_args.ul_arg2 = MAC_ORIGINATOR_DELBA;
        st_action_args.ul_arg3 = MAC_UNSPEC_REASON;                                   /* BA�Ự��ȷ�ϲ��� */
        st_action_args.puc_arg5 = pst_hmac_user->st_user_base_info.auc_user_mac_addr; /* ba�Ự��Ӧ��user */

        /* ɾ��BA�Ự */
        ul_ret = hmac_mgmt_tx_action(pst_hmac_vap, pst_hmac_user, &st_action_args);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                             "{hmac_del_ba_event::hmac_mgmt_tx_action failed.}");
            continue;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_syn_info_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_relt;

    dmac_to_hmac_syn_info_event_stru *pst_syn_info_event;

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_syn_info_event = (dmac_to_hmac_syn_info_event_stru *)pst_event->auc_event_data;
    pst_hmac_user = mac_res_get_hmac_user(pst_syn_info_event->us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_syn_info_event: pst_hmac_user null,user_idx=%d.}",
                         pst_syn_info_event->us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_vap = mac_res_get_mac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_syn_info_event: pst_mac_vap null! vap_idx=%d, user_idx=%d.}",
                         pst_hmac_user->st_user_base_info.uc_vap_id,
                         pst_syn_info_event->us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_set_cur_protocol_mode(&pst_hmac_user->st_user_base_info, pst_syn_info_event->uc_cur_protocol);
    mac_user_set_cur_bandwidth(&pst_hmac_user->st_user_base_info, pst_syn_info_event->uc_cur_bandwidth);
    ul_relt = hmac_config_user_info_syn(pst_mac_vap, &pst_hmac_user->st_user_base_info);

    return ul_relt;
}


OAL_STATIC uint32_t hmac_voice_aggr_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    mac_vap_stru *pst_mac_vap;

    dmac_to_hmac_voice_aggr_event_stru *pst_voice_aggr_event;

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_voice_aggr_event = (dmac_to_hmac_voice_aggr_event_stru *)pst_event->auc_event_data;

    pst_mac_vap = mac_res_get_mac_vap(pst_voice_aggr_event->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_voice_aggr_event: pst_mac_vap null! vap_idx=%d}",
                       pst_voice_aggr_event->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap->bit_voice_aggr = pst_voice_aggr_event->en_voice_aggr;

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_M2S

OAL_STATIC uint32_t hmac_m2s_sync_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    dmac_to_hmac_m2s_event_stru *pst_m2s_event;
    mac_device_stru *pst_mac_device;

    pst_event = frw_get_event_stru(pst_event_mem);

    pst_m2s_event = (dmac_to_hmac_m2s_event_stru *)pst_event->auc_event_data;

    pst_mac_device = mac_res_get_dev(pst_m2s_event->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_M2S, "{hmac_m2s_sync_event: mac device is null ptr. device id:%d}",
                       pst_m2s_event->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (WLAN_M2S_TYPE_HW == pst_m2s_event->en_m2s_type) {
        /* Ӳ�л�����mac device��nss���� */
        MAC_DEVICE_GET_NSS_NUM(pst_mac_device) = pst_m2s_event->en_m2s_nss;
    }

    return OAL_SUCC;
}
#endif

oal_bool_enum_uint8 hmac_get_feature_switch(hmac_feature_switch_type_enum_uint8 feature_id)
{
    if (oal_unlikely(feature_id >= HMAC_FEATURE_SWITCH_BUTT)) {
        return OAL_FALSE;
    }

    return g_feature_switch[feature_id];
}

#ifdef _PRE_WLAN_FEATURE_DYN_CLOSED
int32_t is_hisi_insmod_hi110x_wlan(void)
{
    int32_t ret;
    const char *dts_wifi_status = NULL;
    ret = get_board_custmize(DTS_NODE_HI110X_WIFI, "status", &dts_wifi_status);
    if (ret != BOARD_SUCC) {
        oal_io_print("is_hisi_insmod_hi110x_wlan:needn't insmod ko because of not get dts node.");
        return -OAL_EFAIL;
    }

    if (!oal_strcmp("ok", dts_wifi_status)) {
        oal_io_print("is_hisi_insmod_hi110x_wlan:must be insmod ko.");
        ret = OAL_SUCC;
    } else {
        oal_io_print("is_hisi_insmod_hi110x_wlan:needn't insmod ko because of get status not ok.");
        ret = -OAL_EFAIL;
    }
    return ret;
}
#endif
int32_t hmac_net_register_netdev(oal_net_device_stru *p_net_device)
{
#ifdef _PRE_WLAN_FEATURE_DYN_CLOSED
    if (is_hisi_insmod_hi110x_wlan() != OAL_SUCC) {
        return OAL_SUCC;
    }
#endif
    return oal_net_register_netdev(p_net_device);
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_board_get_instance);
oal_module_symbol(hmac_sdt_recv_reg_cmd);
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
oal_module_symbol(hmac_sdt_recv_sample_cmd);
#endif

oal_module_license("GPL");
/*lint +e578*/ /*lint +e19*/
