

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "oal_net.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#include "wlan_spec.h"

#include "mac_ie.h"

#include "frw_event_main.h"

#include "hmac_vap.h"
#include "hmac_resource.h"
#include "hmac_tx_amsdu.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_fsm.h"
#include "hmac_ext_if.h"
#include "hmac_chan_mgmt.h"
#include "hmac_sme_sta.h"
#include "hmac_blockack.h"
#include "hmac_p2p.h"
#ifdef _PRE_WLAN_TCP_OPT
#include "hmac_tcp_opt.h"
#endif
#include "hmac_roam_main.h"
#include "hmac_mgmt_sta.h"
#include "hmac_mgmt_ap.h"
#include "hmac_blacklist.h"

#include "hisi_customize_wifi.h"

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
#include "hmac_wds.h"
#endif

#include "securec.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_11AX
#include "hmac_wifi6_self_cure.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VAP_C

uint8_t g_uc_host_rx_ampdu_amsdu = OAL_FALSE;

#define HMAC_NETDEVICE_WDT_TIMEOUT (5 * OAL_TIME_HZ)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
OAL_STATIC int32_t hmac_cfg_vap_if_open(oal_net_device_stru *pst_dev);
OAL_STATIC int32_t hmac_cfg_vap_if_close(oal_net_device_stru *pst_dev);
OAL_STATIC oal_net_dev_tx_enum hmac_cfg_vap_if_xmit(oal_netbuf_stru *pst_buf, oal_net_device_stru *pst_dev);
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC oal_net_device_ops_stru gst_vap_net_dev_cfg_vap_ops = {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    .ndo_open = hmac_cfg_vap_if_open,
    .ndo_stop = hmac_cfg_vap_if_close,
    .ndo_start_xmit = hmac_cfg_vap_if_xmit,
#else
    .ndo_get_stats = oal_net_device_get_stats,
    .ndo_open = oal_net_device_open,
    .ndo_stop = oal_net_device_close,
    .ndo_start_xmit = OAL_PTR_NULL,
    .ndo_set_multicast_list = OAL_PTR_NULL,
    .ndo_do_ioctl = oal_net_device_ioctl,
    .ndo_change_mtu = oal_net_device_change_mtu,
    .ndo_init = oal_net_device_init,
    .ndo_set_mac_address = oal_net_device_set_macaddr
#endif
};
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
OAL_STATIC oal_net_device_ops_stru gst_vap_net_dev_cfg_vap_ops = {
    oal_net_device_init,
    oal_net_device_open,
    oal_net_device_close,
    OAL_PTR_NULL,
    OAL_PTR_NULL,
    oal_net_device_get_stats,
    oal_net_device_ioctl,
    oal_net_device_change_mtu,
    oal_net_device_set_macaddr
};
#endif
extern void hmac_del_virtual_inf_worker(oal_work_stru *pst_del_virtual_inf_work);

#ifdef _PRE_WLAN_FEATURE_SAE
void hmac_report_ext_auth_worker(oal_work_stru *pst_sae_report_ext_auth_worker);
#endif
/* 3 函数实现 */
#ifdef _PRE_WLAN_FEATURE_FTM

void hmac_vap_init_gas(hmac_vap_stru *pst_hmac_vap)
{
    pst_hmac_vap->st_gas_mgmt.uc_is_gas_request_sent = OAL_FALSE;
    pst_hmac_vap->st_gas_mgmt.uc_gas_dialog_token = HMAC_GAS_DIALOG_TOKEN_INITIAL_VALUE;
    pst_hmac_vap->st_gas_mgmt.uc_gas_response_dialog_token = HMAC_GAS_DIALOG_TOKEN_INITIAL_VALUE;
}
#endif


extern uint8_t g_wlan_ps_mode;
OAL_STATIC void hmac_vap_sta_init(hmac_vap_stru *p_hmac_vap,
    uint8_t chip_id, uint8_t device_id, uint8_t vap_id, mac_cfg_add_vap_param_stru *p_param)
{
    mac_blacklist_get_pointer(p_param->en_vap_mode, chip_id,
        device_id, vap_id, &(p_hmac_vap->pst_blacklist_info));
    if (p_hmac_vap->pst_blacklist_info != OAL_PTR_NULL) {
        memset_s(p_hmac_vap->pst_blacklist_info->ast_black_list,
            sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX, 0,
            sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX);
        memset_s(&(p_hmac_vap->pst_blacklist_info->st_autoblacklist_info),
            sizeof(mac_autoblacklist_info_stru), 0, sizeof(mac_autoblacklist_info_stru));
        p_hmac_vap->pst_blacklist_info->uc_mode = 0;
        p_hmac_vap->pst_blacklist_info->uc_list_num = 0;
    }

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    p_hmac_vap->uc_edca_opt_flag_sta = 0;
    p_hmac_vap->uc_edca_opt_weight_sta = WLAN_EDCA_OPT_WEIGHT_STA;
#endif

#ifdef _PRE_WLAN_FEATURE_DFS
    mac_mib_set_SpectrumManagementImplemented(&p_hmac_vap->st_vap_base_info, OAL_TRUE);
#endif
    p_hmac_vap->bit_sta_protocol_cfg = OAL_SWITCH_OFF;
    p_hmac_vap->uc_cfg_sta_pm_manual = 0xFF;
    p_hmac_vap->uc_ps_mode = g_wlan_ps_mode;
    p_hmac_vap->ipaddr_obtained = OAL_FALSE;

    /* STA模式默认业务识别功能开启 */
    mac_mib_set_TxTrafficClassifyFlag(&p_hmac_vap->st_vap_base_info, OAL_SWITCH_ON);
#ifdef _PRE_WLAN_FEATURE_11AX
    hmac_wifi6_self_cure_init(p_hmac_vap);
#endif
    p_hmac_vap->tcp_ack_buf_use_ctl_switch = OAL_SWITCH_OFF; /* 默认关闭由应用层控制的TCP ack缓存门限 */
}

OAL_STATIC void hmac_vap_init_set_rx_reorder_timeout(hmac_vap_stru *pst_hmac_vap)
{
    pst_hmac_vap->us_rx_timeout[WLAN_WME_AC_BK] = HMAC_BA_RX_BK_TIMEOUT;
    pst_hmac_vap->us_rx_timeout[WLAN_WME_AC_BE] = HMAC_BA_RX_BE_TIMEOUT;
    pst_hmac_vap->us_rx_timeout[WLAN_WME_AC_VI] = HMAC_BA_RX_VI_TIMEOUT;
    pst_hmac_vap->us_rx_timeout[WLAN_WME_AC_VO] = HMAC_BA_RX_VO_TIMEOUT;

    pst_hmac_vap->us_rx_timeout_min[WLAN_WME_AC_BE] = HMAC_BA_RX_BE_TIMEOUT_MIN;
    pst_hmac_vap->us_rx_timeout_min[WLAN_WME_AC_BK] = HMAC_BA_RX_BK_TIMEOUT_MIN;
    pst_hmac_vap->us_rx_timeout_min[WLAN_WME_AC_VI] = HMAC_BA_RX_VI_TIMEOUT_MIN;
    pst_hmac_vap->us_rx_timeout_min[WLAN_WME_AC_VO] = HMAC_BA_RX_VO_TIMEOUT_MIN;
}
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP

void hmac_vap_init_hid2d_seqnum(hmac_vap_stru *pst_hmac_vap)
{
    uint8_t uc_index;
    for (uc_index = 0; uc_index < MAC_HID2D_SEQNUM_CNT; uc_index++) {
        pst_hmac_vap->aus_last_seqnum[uc_index] = MAC_HID2D_INVALID_SEQNUM;
    }
}
#endif
OAL_STATIC void hmac_vap_11kvr_cap_init(hmac_vap_stru *hmac_vap)
{
    hmac_vap->bit_11k_enable = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11k;
    hmac_vap->bit_11v_enable = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11v;
    hmac_vap->bit_bcn_table_switch = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11k;
    hmac_vap->bit_11r_enable = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r;
    hmac_vap->bit_11r_over_ds = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r_ds;
    hmac_vap->bit_adaptive11r = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_adaptive11r;
    hmac_vap->bit_nb_rpt_11k = g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_nb_rpt_11k;
    hmac_vap->bit_11r_private_preauth = hmac_vap->bit_adaptive11r;
}

uint32_t hmac_vap_init(hmac_vap_stru *pst_hmac_vap, uint8_t uc_chip_id,
    uint8_t uc_device_id, uint8_t uc_vap_id, mac_cfg_add_vap_param_stru *pst_param)
{
    uint32_t ul_ret;
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_vap_init(&pst_hmac_vap->st_vap_base_info, uc_chip_id, uc_device_id, uc_vap_id, pst_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        return ul_ret;
    }

    /* 统计信息清零 */
    oam_stats_clear_vap_stat_info(uc_vap_id);

    /* 初始化预设参数 */
    pst_hmac_vap->st_preset_para.en_protocol = pst_hmac_vap->st_vap_base_info.en_protocol;
    pst_hmac_vap->st_preset_para.en_bandwidth = pst_hmac_vap->st_vap_base_info.st_channel.en_bandwidth;
    pst_hmac_vap->st_preset_para.en_band = pst_hmac_vap->st_vap_base_info.st_channel.en_band;

    /* 初始化配置私有结构体 */
    // 对于P2P CL 不能再初始化队列
    pst_hmac_vap->st_cfg_priv.dog_tag = OAL_DOG_TAG;
    oal_wait_queue_init_head(&(pst_hmac_vap->st_cfg_priv.st_wait_queue_for_sdt_reg));
    oal_wait_queue_init_head(&(pst_hmac_vap->st_mgmt_tx.st_wait_queue));

#ifdef _PRE_WLAN_FEATURE_11D
    pst_hmac_vap->en_updata_rd_by_ie_switch = OAL_FALSE;
#endif

    // 1103默认支持接收方向的AMPDU+AMSDU联合聚合,通过定制化决定
    pst_hmac_vap->bit_rx_ampduplusamsdu_active = g_uc_host_rx_ampdu_amsdu;
    pst_hmac_vap->en_ps_rx_amsdu = g_uc_host_rx_ampdu_amsdu;
    pst_hmac_vap->hal_dev_id = 0xff; /* 0xff invalid */
    pst_hmac_vap->d2h_amsdu_switch = OAL_TRUE; /* 默认支持amsdu */
    oal_wait_queue_init_head(&pst_hmac_vap->query_wait_q); /* 根据配置VAP，将对应函数挂接在业务VAP，区分AP、STA和WDS模式 */
    switch (pst_param->en_vap_mode) {
        case WLAN_VAP_MODE_BSS_AP:
#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
            pst_hmac_vap->ul_edca_opt_time_ms = HMAC_EDCA_OPT_TIME_MS;
            frw_timer_create_timer_m(&(pst_hmac_vap->st_edca_opt_timer), hmac_edca_opt_timeout_fn,
                                   pst_hmac_vap->ul_edca_opt_time_ms, pst_hmac_vap, OAL_TRUE,
                                   OAM_MODULE_ID_HMAC, pst_hmac_vap->st_vap_base_info.ul_core_id);
            /* also open for 1102 at 2015-10-16 */
            pst_hmac_vap->uc_edca_opt_flag_ap = 1;
            frw_timer_restart_timer_m(&(pst_hmac_vap->st_edca_opt_timer), pst_hmac_vap->ul_edca_opt_time_ms, OAL_TRUE);
            pst_hmac_vap->uc_idle_cycle_num = 0;
#endif

            /* AP模式默认业务识别功能开启 */
            mac_mib_set_TxTrafficClassifyFlag(&pst_hmac_vap->st_vap_base_info, OAL_SWITCH_ON);
            mac_blacklist_get_pointer(pst_param->en_vap_mode, uc_chip_id, uc_device_id,
                                      uc_vap_id, &(pst_hmac_vap->pst_blacklist_info));
            if (pst_hmac_vap->pst_blacklist_info != OAL_PTR_NULL) {
                memset_s(pst_hmac_vap->pst_blacklist_info->ast_black_list,
                         sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX, 0,
                         sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX);
                memset_s(&(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info),
                         sizeof(mac_autoblacklist_info_stru), 0, sizeof(mac_autoblacklist_info_stru));
                pst_hmac_vap->pst_blacklist_info->uc_mode = 0;
                pst_hmac_vap->pst_blacklist_info->uc_list_num = 0;
            }
            break;

        case WLAN_VAP_MODE_BSS_STA:
            hmac_vap_sta_init(pst_hmac_vap, uc_chip_id, uc_device_id, uc_vap_id, pst_param);
            break;

        case WLAN_VAP_MODE_WDS:
            break;

        case WLAN_VAP_MODE_CONFIG:
            /* 配置VAP直接返回 */
            return OAL_SUCC;

        default:
            return OAL_ERR_CODE_INVALID_CONFIG;
    }

    oal_spin_lock_init(&pst_hmac_vap->st_lock_state);
    oal_dlist_init_head(&(pst_hmac_vap->st_pmksa_list_head));

    /* 创建vap时 初始状态为init */
    mac_vap_state_change(&(pst_hmac_vap->st_vap_base_info), MAC_VAP_STATE_INIT);

    /* 初始化重排序超时时间 */
    hmac_vap_init_set_rx_reorder_timeout(pst_hmac_vap);

    /* 初始化删除虚拟网络接口工作队列 */
    oal_init_work(&(pst_hmac_vap->st_del_virtual_inf_worker), hmac_del_virtual_inf_worker);
    pst_hmac_vap->pst_del_net_device = OAL_PTR_NULL;
    pst_hmac_vap->pst_p2p0_net_device = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_SAE
    oal_init_work(&(pst_hmac_vap->st_sae_report_ext_auth_worker), hmac_report_ext_auth_worker);
    pst_hmac_vap->duplicate_auth_seq2_flag = OAL_FALSE;
    pst_hmac_vap->duplicate_auth_seq4_flag = OAL_FALSE;
#endif
#ifdef _PRE_WLAN_TCP_OPT
    pst_hmac_vap->st_hamc_tcp_ack[HCC_TX].filter_info.ul_ack_limit = DEFAULT_TX_TCP_ACK_THRESHOLD;
    pst_hmac_vap->st_hamc_tcp_ack[HCC_RX].filter_info.ul_ack_limit = DEFAULT_RX_TCP_ACK_THRESHOLD;
    hmac_tcp_opt_init_filter_tcp_ack_pool(pst_hmac_vap);
#endif
    /* 双wlan模式下，只wlan0支持11k和11v */
    if (mac_is_primary_legacy_vap(&pst_hmac_vap->st_vap_base_info) &&
        pst_param->en_p2p_mode == WLAN_LEGACY_VAP_MODE && IS_STA(&(pst_hmac_vap->st_vap_base_info))) {
        hmac_vap_11kvr_cap_init(pst_hmac_vap);
    }

    mac_mib_set_dot11RadioMeasurementActivated(&(pst_hmac_vap->st_vap_base_info), pst_hmac_vap->bit_11k_enable);
    mac_mib_set_dot11RMBeaconTableMeasurementActivated(&(pst_hmac_vap->st_vap_base_info),
                                                       pst_hmac_vap->bit_bcn_table_switch);

#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        hmac_vap_init_gas(pst_hmac_vap);
    }
#endif

#if defined(_PRE_WLAN_FEATURE_11K)
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
    mac_mib_set_MgmtOptionBSSTransitionActivated(&(pst_hmac_vap->st_vap_base_info), pst_hmac_vap->bit_11v_enable);
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    hmac_vap_init_hid2d_seqnum(pst_hmac_vap);
#endif
    return OAL_SUCC;
}


uint32_t hmac_vap_get_priv_cfg(mac_vap_stru *pst_mac_vap, hmac_vap_cfg_priv_stru **ppst_cfg_priv)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_vap_get_priv_cfg::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY,
                       "{hmac_vap_get_priv_cfg::mac_res_get_hmac_vap fail.vap_id = %u}",
                       pst_mac_vap->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *ppst_cfg_priv = &pst_hmac_vap->st_cfg_priv;

    return OAL_SUCC;
}


int8_t *hmac_vap_get_desired_country(uint8_t uc_vap_id)
{
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{hmac_vap_get_desired_country::pst_hmac_vap null.}");
        return OAL_PTR_NULL;
    }

    return pst_hmac_vap->ac_desired_country;
}
#ifdef _PRE_WLAN_FEATURE_11D

uint32_t hmac_vap_get_updata_rd_by_ie_switch(uint8_t uc_vap_id, oal_bool_enum_uint8 *us_updata_rd_by_ie_switch)
{
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(0);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_vap_get_updata_rd_by_ie_switch::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *us_updata_rd_by_ie_switch = pst_hmac_vap->en_updata_rd_by_ie_switch;
    return OAL_SUCC;
}
#endif

oal_net_device_stru *hmac_vap_get_net_device(uint8_t uc_vap_id)
{
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(uc_vap_id, OAM_SF_ANY, "{hmac_vap_get_net_device::pst_hmac_vap null.}");
        return OAL_PTR_NULL;
    }

    return (pst_hmac_vap->pst_net_device);
}


OAL_STATIC uint32_t hmac_vap_creat_netdev_set_net_device(oal_net_device_stru *pst_net_device, int8_t *puc_mac_addr, mac_vap_stru *pst_vap)
{
    int32_t l_ret;
    /* 如下对netdevice的赋值暂时按如下操作 */
    oal_netdevice_ops(pst_net_device) = &gst_vap_net_dev_cfg_vap_ops;
    oal_netdevice_destructor(pst_net_device) = oal_net_free_netdev;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    /* TBD  need to check the net device diff */
#else
    oal_netdevice_master(pst_net_device) = OAL_PTR_NULL;
#endif

    oal_netdevice_ifalias(pst_net_device) = OAL_PTR_NULL;
    oal_netdevice_watchdog_timeo(pst_net_device) = HMAC_NETDEVICE_WDT_TIMEOUT;
    l_ret = memcpy_s(oal_netdevice_mac_addr(pst_net_device), WLAN_MAC_ADDR_LEN,
                     puc_mac_addr, WLAN_MAC_ADDR_LEN);
    oal_net_dev_priv(pst_net_device) = pst_vap;
    oal_netdevice_qdisc(pst_net_device, OAL_PTR_NULL);

    return l_ret;
}


uint32_t hmac_vap_creat_netdev(hmac_vap_stru *pst_hmac_vap, int8_t *puc_netdev_name,
                                     uint8_t uc_netdev_name_len, int8_t *puc_mac_addr)
{
    oal_net_device_stru *pst_net_device = OAL_PTR_NULL;
#ifdef _PRE_CONFIG_HISI_110X_DISABLE_HISILICON0

#else
    uint32_t ul_return;
#endif
    mac_vap_stru *pst_vap = OAL_PTR_NULL;
    int32_t l_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_netdev_name))) {
        oam_error_log2(0, OAM_SF_ASSOC, "{hmac_vap_creat_netdev::param null %d %d.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)puc_netdev_name);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_netdev_name_len > MAC_NET_DEVICE_NAME_LENGTH) {
        return OAL_FAIL;
    }

    pst_vap = &pst_hmac_vap->st_vap_base_info;
    pst_net_device = oal_net_alloc_netdev(0, puc_netdev_name, oal_ether_setup);
    if (oal_unlikely(pst_net_device == OAL_PTR_NULL)) {
        oam_warning_log0(pst_vap->uc_vap_id, OAM_SF_ANY, "{hmac_vap_creat_netdev::pst_net_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    l_ret = hmac_vap_creat_netdev_set_net_device(pst_net_device, puc_mac_addr, pst_vap);

#ifdef _PRE_CONFIG_HISI_110X_DISABLE_HISILICON0

#else
    ul_return = (uint32_t)hmac_net_register_netdev(pst_net_device);
    if (oal_unlikely(ul_return != OAL_SUCC)) {
        oam_warning_log0(pst_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_vap_creat_netdev::oal_net_register_netdev failed.}");
        oal_net_free_netdev(pst_net_device);
        return ul_return;
    }
#endif

    pst_hmac_vap->pst_net_device = pst_net_device;
    /* 包括'\0' */
    l_ret += memcpy_s(pst_hmac_vap->auc_name, OAL_IF_NAME_SIZE, pst_net_device->name, OAL_IF_NAME_SIZE);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_vap_creat_netdev::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint32_t hmac_vap_destroy(hmac_vap_stru *pst_hmac_vap)
{
    mac_cfg_down_vap_param_stru st_down_vap;
    mac_cfg_del_vap_param_stru st_del_vap_param;
    uint32_t ul_ret;

    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_vap_destroy::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_hmac_vap->uc_edca_opt_flag_ap = 0;
        frw_timer_immediate_destroy_timer_m(&(pst_hmac_vap->st_edca_opt_timer));
    } else if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_hmac_vap->uc_edca_opt_flag_sta = 0;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_TWT
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        memset_s(&(pst_hmac_vap->st_twt_cfg), OAL_SIZEOF(mac_cfg_twt_stru), 0, OAL_SIZEOF(mac_cfg_twt_stru));
    }
#endif

    /* 先down vap */
    oal_cancel_work_sync(&(pst_hmac_vap->st_del_virtual_inf_worker));
    st_down_vap.en_p2p_mode = pst_hmac_vap->st_vap_base_info.en_p2p_mode;

    st_down_vap.pst_net_dev = pst_hmac_vap->pst_net_device;
    ul_ret = hmac_config_down_vap(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                                      (uint8_t *)&st_down_vap);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_vap_destroy::hmac_config_down_vap failed[%d].}", ul_ret);
        return ul_ret;
    }
    mac_blacklist_free_pointer(&pst_hmac_vap->st_vap_base_info, pst_hmac_vap->pst_blacklist_info);
    /* 然后再delete vap */
    st_del_vap_param.en_p2p_mode = pst_hmac_vap->st_vap_base_info.en_p2p_mode;
    st_del_vap_param.en_vap_mode = pst_hmac_vap->st_vap_base_info.en_vap_mode;
    ul_ret = hmac_config_del_vap(&pst_hmac_vap->st_vap_base_info, OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                                     (uint8_t *)&st_del_vap_param);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_vap_destroy::hmac_config_del_vap failed[%d].}", ul_ret);
        return ul_ret;
    }
    return OAL_SUCC;
}


uint16_t hmac_vap_check_ht_capabilities_ap(hmac_vap_stru *pst_hmac_vap,
                                                 uint8_t *puc_payload,
                                                 uint32_t ul_msg_len,
                                                 hmac_user_stru *pst_hmac_user_sta)
{
    oal_bool_enum en_prev_asoc_ht = OAL_FALSE;
    oal_bool_enum en_prev_asoc_non_ht = OAL_FALSE;
    mac_user_ht_hdl_stru *pst_ht_hdl = &(pst_hmac_user_sta->st_user_base_info.st_ht_hdl);
    mac_protection_stru *pst_protection = OAL_PTR_NULL;
    uint8_t *puc_ie = OAL_PTR_NULL;

    if (OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(&pst_hmac_vap->st_vap_base_info)) {
        return MAC_SUCCESSFUL_STATUSCODE;
    }
    /* 检查STA是否是作为一个HT capable STA与AP关联 */
    if ((pst_hmac_user_sta->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (pst_ht_hdl->en_ht_capable == OAL_TRUE)) {
        mac_user_set_ht_capable(&(pst_hmac_user_sta->st_user_base_info), OAL_FALSE);
        en_prev_asoc_ht = OAL_TRUE;
    }

    /* 检查STA是否是作为一个non HT capable STA与AP关联 */
    else if (pst_hmac_user_sta->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        en_prev_asoc_non_ht = OAL_TRUE;
    } else {
    }

    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_payload, (int32_t)ul_msg_len);
    if (puc_ie != OAL_PTR_NULL) {
        /* 不允许HT STA 使用 TKIP/WEP 加密 */
        if (mac_is_wep_enabled(&pst_hmac_vap->st_vap_base_info)) {
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                "{hmac_vap_check_ht_capabilities_ap::Rejecting a HT STA because of its Pairwise Cipher[%d].}",
                pst_hmac_user_sta->st_user_base_info.st_key_info.en_cipher_type);
            return MAC_MISMATCH_HTCAP;
        }

        /* 搜索 HT Capabilities Element */
        hmac_search_ht_cap_ie_ap(pst_hmac_vap, pst_hmac_user_sta, puc_ie, 0, en_prev_asoc_ht);

        if ((pst_ht_hdl->uc_rx_mcs_bitmask[3] == 0) && (pst_ht_hdl->uc_rx_mcs_bitmask[2] == 0) &&
            (pst_ht_hdl->uc_rx_mcs_bitmask[1] == 0) && (pst_ht_hdl->uc_rx_mcs_bitmask[0]) == 0) {
            oam_warning_log0(0, OAM_SF_ANY,
                "{hmac_vap_check_ht_capabilities_ap::STA support ht capability but support none space_stream.}");
            /* 对端ht能力置为不支持 */
            mac_user_set_ht_capable(&(pst_hmac_user_sta->st_user_base_info), OAL_FALSE);
        }
    }

    puc_ie = mac_find_ie(MAC_EID_EXT_CAPS, puc_payload, (int32_t)ul_msg_len);
    if (puc_ie != OAL_PTR_NULL) {
        mac_ie_proc_ext_cap_ie(&(pst_hmac_user_sta->st_user_base_info), puc_ie);
    }

    /* 走到这里，说明sta已经被统计到ht相关的统计量中 */
    pst_hmac_user_sta->st_user_stats_flag.bit_no_ht_stats_flag = OAL_TRUE;
    pst_hmac_user_sta->st_user_stats_flag.bit_no_gf_stats_flag = OAL_TRUE;
    pst_hmac_user_sta->st_user_stats_flag.bit_20M_only_stats_flag = OAL_TRUE;
    pst_hmac_user_sta->st_user_stats_flag.bit_no_lsig_txop_stats_flag = OAL_TRUE;
    pst_hmac_user_sta->st_user_stats_flag.bit_no_40dsss_stats_flag = OAL_TRUE;

    pst_protection = &(pst_hmac_vap->st_vap_base_info.st_protection);
    if (pst_ht_hdl->en_ht_capable == OAL_FALSE) { /* STA不支持HT */
        /*  如果STA之前没有与AP关联 */
        if (pst_hmac_user_sta->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
            pst_protection->uc_sta_non_ht_num++;
        }
        /* 如果STA之前已经作为HT站点与AP关联 */
        else if (en_prev_asoc_ht == OAL_TRUE) {
            pst_protection->uc_sta_non_ht_num++;

            if ((pst_ht_hdl->bit_supported_channel_width == OAL_FALSE) && (pst_protection->uc_sta_20M_only_num != 0)) {
                pst_protection->uc_sta_20M_only_num--;
            }

            if ((pst_ht_hdl->bit_ht_green_field == OAL_FALSE) && (pst_protection->uc_sta_non_gf_num != 0)) {
                pst_protection->uc_sta_non_gf_num--;
            }

            if ((pst_ht_hdl->bit_lsig_txop_protection == OAL_FALSE) && (pst_protection->uc_sta_no_lsig_txop_num != 0)) {
                pst_protection->uc_sta_no_lsig_txop_num--;
            }
        } else { /* STA 之前已经作为非HT站点与AP关联 */
        }
    } else { /* STA支持HT */
        hmac_amsdu_init_user(pst_hmac_user_sta);

        /*  如果STA之前已经以non-HT站点与AP关联, 则将uc_sta_non_ht_num减1 */
        if ((en_prev_asoc_non_ht == OAL_TRUE) && (pst_protection->uc_sta_non_ht_num != 0)) {
            pst_protection->uc_sta_non_ht_num--;
        }
    }

    return MAC_SUCCESSFUL_STATUSCODE;
}


void hmac_search_txbf_cap_ie_ap(mac_user_ht_hdl_stru *pst_ht_hdl,
                                        uint32_t ul_info_elem)
{
    uint32_t ul_tmp_txbf_elem = ul_info_elem;

    pst_ht_hdl->bit_imbf_receive_cap = (ul_tmp_txbf_elem & BIT0);
    pst_ht_hdl->bit_receive_staggered_sounding_cap = ((ul_tmp_txbf_elem & BIT1) >> 1);
    pst_ht_hdl->bit_transmit_staggered_sounding_cap = ((ul_tmp_txbf_elem & BIT2) >> 2);
    pst_ht_hdl->bit_receive_ndp_cap = ((ul_tmp_txbf_elem & BIT3) >> 3);
    pst_ht_hdl->bit_transmit_ndp_cap = ((ul_tmp_txbf_elem & BIT4) >> 4);
    pst_ht_hdl->bit_imbf_cap = ((ul_tmp_txbf_elem & BIT5) >> 5);
    pst_ht_hdl->bit_calibration = ((ul_tmp_txbf_elem & 0x000000C0) >> 6);
    pst_ht_hdl->bit_exp_csi_txbf_cap = ((ul_tmp_txbf_elem & BIT8) >> 8);
    pst_ht_hdl->bit_exp_noncomp_txbf_cap = ((ul_tmp_txbf_elem & BIT9) >> 9);
    pst_ht_hdl->bit_exp_comp_txbf_cap = ((ul_tmp_txbf_elem & BIT10) >> 10);
    pst_ht_hdl->bit_exp_csi_feedback = ((ul_tmp_txbf_elem & 0x00001800) >> 11);
    pst_ht_hdl->bit_exp_noncomp_feedback = ((ul_tmp_txbf_elem & 0x00006000) >> 13);

    pst_ht_hdl->bit_exp_comp_feedback = ((ul_tmp_txbf_elem & 0x0001C000) >> 15);
    pst_ht_hdl->bit_min_grouping = ((ul_tmp_txbf_elem & 0x00060000) >> 17);
    pst_ht_hdl->bit_csi_bfer_ant_number = ((ul_tmp_txbf_elem & 0x001C0000) >> 19);
    pst_ht_hdl->bit_noncomp_bfer_ant_number = ((ul_tmp_txbf_elem & 0x00600000) >> 21);
    pst_ht_hdl->bit_comp_bfer_ant_number = ((ul_tmp_txbf_elem & 0x01C00000) >> 23);
    pst_ht_hdl->bit_csi_bfee_max_rows = ((ul_tmp_txbf_elem & 0x06000000) >> 25);
    pst_ht_hdl->bit_channel_est_cap = ((ul_tmp_txbf_elem & 0x18000000) >> 27);
}


uint32_t hmac_search_ht_cap_ie_ap(hmac_vap_stru *pst_hmac_vap,
                                        hmac_user_stru *pst_hmac_user_sta,
                                        uint8_t *puc_payload,
                                        uint16_t us_current_offset,
                                        oal_bool_enum en_prev_asoc_ht)
{
    uint8_t uc_smps;
    uint8_t uc_supported_channel_width;
    uint8_t uc_ht_green_field;
    uint8_t uc_lsig_txop_protection_support;
    uint8_t uc_mcs_bmp_index;
    uint8_t *puc_tmp_payload = OAL_PTR_NULL;
    uint16_t us_tmp_info_elem;
    uint16_t us_tmp_txbf_low;
    uint32_t ul_tmp_txbf_elem;

    mac_user_ht_hdl_stru *pst_ht_hdl = OAL_PTR_NULL;
    mac_user_ht_hdl_stru st_ht_hdl;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_search_ht_cap_ie_ap::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 保存 入参 */
    puc_tmp_payload = puc_payload;

    if (puc_tmp_payload[1] < MAC_HT_CAP_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_search_ht_cap_ie_ap::invalid ht cap len[%d]}", puc_tmp_payload[1]);
        return OAL_FAIL;
    }

    pst_ht_hdl = &st_ht_hdl;
    mac_user_get_ht_hdl(&(pst_hmac_user_sta->st_user_base_info), pst_ht_hdl);
    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mac_user = &(pst_hmac_user_sta->st_user_base_info);

    /* 带有 HT Capability Element 的 STA，标示它具有HT capable. */
    pst_ht_hdl->en_ht_capable = 1;
    us_current_offset += MAC_IE_HDR_LEN;

    /***************************************************************************
                    解析 HT Capabilities Info Field
    ***************************************************************************/

    us_tmp_info_elem = oal_make_word16(puc_tmp_payload[us_current_offset], puc_tmp_payload[us_current_offset + 1]);

    /* 检查STA所支持的LDPC编码能力 B0，0:不支持，1:支持 */
    pst_ht_hdl->bit_ldpc_coding_cap = (us_tmp_info_elem & BIT0);

    /* 检查STA所支持的信道宽度 B1，0:仅20M运行，1:20M与40M运行 */
    uc_supported_channel_width = (us_tmp_info_elem & BIT1) >> 1;
    pst_ht_hdl->bit_supported_channel_width = mac_ie_proc_ht_supported_channel_width(pst_mac_user, pst_mac_vap,
        uc_supported_channel_width, en_prev_asoc_ht);

    /* 检查空间复用节能模式 B2~B3 */
    uc_smps = ((us_tmp_info_elem & (BIT3 | BIT2)) >> 2);
    pst_ht_hdl->bit_sm_power_save = mac_ie_proc_sm_power_save_field(pst_mac_user, uc_smps);

    /* 检查Greenfield 支持情况 B4， 0:不支持，1:支持 */
    uc_ht_green_field = (us_tmp_info_elem & BIT4) >> 4;
    pst_ht_hdl->bit_ht_green_field = mac_ie_proc_ht_green_field(pst_mac_user, pst_mac_vap,
                                                                    uc_ht_green_field, en_prev_asoc_ht);

    /* 检查20MHz Short-GI B5,  0:不支持，1:支持，之后与AP取交集  */
    pst_ht_hdl->bit_short_gi_20mhz = ((us_tmp_info_elem & BIT5) >> 5);
    pst_ht_hdl->bit_short_gi_20mhz &= mac_mib_get_ShortGIOptionInTwentyImplemented(pst_mac_vap);
    /* 检查40MHz Short-GI B6,  0:不支持，1:支持，之后与AP取交集 */
    pst_ht_hdl->bit_short_gi_40mhz = ((us_tmp_info_elem & BIT6) >> 6);
    pst_ht_hdl->bit_short_gi_40mhz &= mac_mib_get_ShortGIOptionInFortyImplemented(&pst_hmac_vap->st_vap_base_info);

    /* 检查支持接收STBC PPDU B8,  0:不支持，1:支持 */
    pst_ht_hdl->bit_rx_stbc = ((us_tmp_info_elem & 0x0300) >> 8);

    /* 检查最大A-MSDU长度 B11，0:3839字节, 1:7935字节 */
    pst_hmac_user_sta->us_amsdu_maxsize = ((us_tmp_info_elem & BIT11) == 0) ?
        WLAN_MIB_MAX_AMSDU_LENGTH_SHORT : WLAN_MIB_MAX_AMSDU_LENGTH_LONG;

    /* 检查在40M上DSSS/CCK的支持情况 B12 */
    /* 在非Beacon帧或probe rsp帧中时 */
    /* 0: STA在40MHz上不使用DSSS/CCK, 1: STA在40MHz上使用DSSS/CCK */
    pst_ht_hdl->bit_dsss_cck_mode_40mhz = ((us_tmp_info_elem & BIT12) >> 12);

    if ((pst_ht_hdl->bit_dsss_cck_mode_40mhz == 0) &&
        (pst_ht_hdl->bit_supported_channel_width == 1)) {
        pst_hmac_vap->st_vap_base_info.st_protection.uc_sta_no_40dsss_cck_num++;
    }

    /* 检查Forty MHz Intolerant */
    pst_ht_hdl->bit_forty_mhz_intolerant = ((us_tmp_info_elem & BIT14) >> 14);
    /*  检查对L-SIG TXOP 保护的支持情况 B15, 0:不支持，1:支持 */
    uc_lsig_txop_protection_support = (us_tmp_info_elem & BIT15) >> 15;
    pst_ht_hdl->bit_lsig_txop_protection = mac_ie_proc_lsig_txop_protection_support(pst_mac_user, pst_mac_vap,
        uc_lsig_txop_protection_support, en_prev_asoc_ht);

    us_current_offset += MAC_HT_CAPINFO_LEN;

    /***************************************************************************
                        解析 A-MPDU Parameters Field
    ***************************************************************************/
    /* 提取 Maximum Rx A-MPDU factor (B1 - B0) */
    pst_ht_hdl->uc_max_rx_ampdu_factor = (puc_tmp_payload[us_current_offset] & 0x03);

    /* 提取 the Minimum MPDU Start Spacing (B2 - B4) */
    pst_ht_hdl->uc_min_mpdu_start_spacing = (puc_tmp_payload[us_current_offset] >> 2) & 0x07;

    us_current_offset += MAC_HT_AMPDU_PARAMS_LEN;

    /***************************************************************************
                        解析 Supported MCS Set Field
    ***************************************************************************/
    for (uc_mcs_bmp_index = 0; uc_mcs_bmp_index < WLAN_HT_MCS_BITMASK_LEN; uc_mcs_bmp_index++) {
        pst_ht_hdl->uc_rx_mcs_bitmask[uc_mcs_bmp_index] =
            //        (mac_mib_get_SupportedMCSTxValue(&pst_hmac_vap->st_vap_base_info, uc_mcs_bmp_index))&
           (*(uint8_t *)(puc_tmp_payload + us_current_offset + uc_mcs_bmp_index));
    }

    pst_ht_hdl->uc_rx_mcs_bitmask[WLAN_HT_MCS_BITMASK_LEN - 1] &= 0x1F;

    us_current_offset += MAC_HT_SUP_MCS_SET_LEN;

    /***************************************************************************
                        解析 HT Extended Capabilities Info Field
    ***************************************************************************/
    us_tmp_info_elem = oal_make_word16(puc_tmp_payload[us_current_offset], puc_tmp_payload[us_current_offset + 1]);
    /* 提取 HTC support Information */
    if ((us_tmp_info_elem & BIT10) != 0) {
        pst_ht_hdl->uc_htc_support = 1;
    }

    us_current_offset += MAC_HT_EXT_CAP_LEN;

    /***************************************************************************
                        解析 Tx Beamforming Field
    ***************************************************************************/
    us_tmp_info_elem = oal_make_word16(puc_tmp_payload[us_current_offset], puc_tmp_payload[us_current_offset + 1]);
    us_tmp_txbf_low = oal_make_word16(puc_tmp_payload[us_current_offset + 2], puc_tmp_payload[us_current_offset + 3]);
    ul_tmp_txbf_elem = oal_make_word32(us_tmp_info_elem, us_tmp_txbf_low);
    hmac_search_txbf_cap_ie_ap(pst_ht_hdl, ul_tmp_txbf_elem);

    mac_user_set_ht_hdl(&(pst_hmac_user_sta->st_user_base_info), pst_ht_hdl);
    hmac_chan_update_40M_intol_user(pst_mac_vap);
    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                     "{hmac_search_ht_cap_ie_ap::en_40M_intol_user[%d].}",
                     pst_mac_vap->en_40M_intol_user);

    /* 如果存在40M不容忍的user，则不允许AP在40MHz运行 */
    if (pst_mac_vap->en_40M_intol_user == OAL_TRUE) {
        if (pst_mac_vap->st_cap_flag.bit_2040_autoswitch) {
            hmac_40M_intol_sync_data(pst_mac_vap, WLAN_BAND_WIDTH_BUTT, pst_mac_vap->en_40M_intol_user);
        }
    } else {
        mac_mib_set_FortyMHzIntolerant(pst_mac_vap, OAL_FALSE);
    }
    return OAL_SUCC;
}


void hmac_vap_net_stopall(void)
{
    uint8_t uc_vap_id;
    oal_net_device_stru *pst_net_device = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == NULL) {
            break;
        }

        pst_net_device = pst_hmac_vap->pst_net_device;

        if (pst_net_device == NULL) {
            break;
        }

        oal_net_tx_stop_all_queues(pst_net_device);
    }
}

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW

void hmac_set_ampdu_worker(oal_delayed_work *pst_work)
{
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = oal_container_of(pst_work, hmac_vap_stru, st_ampdu_work);

    /* 配置完毕后再开启使能聚合 */
    mac_mib_set_CfgAmpduTxAtive(&pst_hmac_vap->st_vap_base_info, OAL_TRUE);
    oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{hmac_set_ampdu_worker:: enable ampdu!}");
}


void hmac_set_ampdu_hw_worker(oal_delayed_work *pst_work)
{
    uint32_t ul_ret;
    hmac_vap_stru *pst_hmac_vap;
    mac_cfg_ampdu_tx_on_param_stru st_ampdu_tx_on = { 0 };

    pst_hmac_vap = oal_container_of(pst_work, hmac_vap_stru, st_set_hw_work);
    /* 聚合硬化:4 */
    if ((pst_hmac_vap->st_mode_set.uc_aggr_tx_on >> 2) & BIT0) {
        st_ampdu_tx_on.uc_aggr_tx_on = 1;
        st_ampdu_tx_on.uc_snd_type = pst_hmac_vap->st_mode_set.uc_snd_type;
    } else { /* 切换为软件聚合:8 */
        st_ampdu_tx_on.uc_aggr_tx_on = 0;
        st_ampdu_tx_on.uc_snd_type = 0;
    }

    /***************************************************************************
            抛事件到DMAC层, 同步DMAC数据
     ***************************************************************************/
    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info, WLAN_CFGID_AMPDU_TX_ON,
                                        OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru), (uint8_t *)&st_ampdu_tx_on);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "hmac_set_ampdu_hw_worker::hmac_config_send_event fail(%u)", ul_ret);
    }
    oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                     "{hmac_set_ampdu_hw_worker:: en_tx_aggr_on[0x%x],snd type[%d]!}",
                     pst_hmac_vap->st_mode_set.uc_aggr_tx_on, pst_hmac_vap->st_mode_set.uc_snd_type);
}
#endif


oal_bool_enum_uint8 hmac_flowctl_check_device_is_sta_mode(void)
{
    mac_device_stru *pst_dev = NULL;
    mac_vap_stru *pst_vap = NULL;
    uint8_t uc_vap_index;
    uint8_t uc_device_max;
    uint8_t uc_device;
    oal_bool_enum_uint8 en_device_is_sta = OAL_FALSE;
    mac_chip_stru *pst_mac_chip;

    pst_mac_chip = hmac_res_get_mac_chip(0);

    /* OAL接口获取支持device个数 */
    uc_device_max = oal_chip_get_device_num(pst_mac_chip->ul_chip_ver);

    for (uc_device = 0; uc_device < uc_device_max; uc_device++) {
        pst_dev = mac_res_get_dev(pst_mac_chip->auc_device_id[uc_device]);
        if (oal_unlikely(pst_dev == OAL_PTR_NULL)) {
            continue;
        }

        if (pst_dev->uc_vap_num >= 1) {
            for (uc_vap_index = 0; uc_vap_index < pst_dev->uc_vap_num; uc_vap_index++) {
                pst_vap = mac_res_get_mac_vap(pst_dev->auc_vap_id[uc_vap_index]);
                if (pst_vap == OAL_PTR_NULL) {
                    oam_info_log1(0, OAM_SF_BA,
                        "{hmac_flowctl_check_device_is_sta_mode::mac_res_get_mac_vap fail.vap_index = %u}",
                        uc_vap_index);
                    continue;
                }

                if (pst_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                    en_device_is_sta = OAL_TRUE;
                    break;
                }
            }
        }
    }
    return en_device_is_sta;
}


OAL_STATIC void hmac_vap_wake_subq(uint8_t uc_vap_id, uint16_t us_queue_idx)
{
    oal_net_device_stru *pst_net_device = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == NULL) {
        return;
    }

    pst_net_device = pst_hmac_vap->pst_net_device;
    if (pst_net_device == NULL) {
        return;
    }

    oal_net_wake_subqueue(pst_net_device, us_queue_idx);
}


void hmac_vap_net_start_subqueue(uint16_t us_queue_idx)
{
    uint8_t uc_vap_id;
    OAL_STATIC uint8_t g_uc_start_subq_flag = 0;

    /* 自旋锁内，任务和软中断都被锁住，不需要FRW锁 */

    if (g_uc_start_subq_flag == 0) {
        g_uc_start_subq_flag = 1;
        uc_vap_id = oal_board_get_service_vap_start_id();

        /* vap id从低到高恢复 跳过配置vap */
        for (; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
            hmac_vap_wake_subq(uc_vap_id, us_queue_idx);
        }
    } else {
        g_uc_start_subq_flag = 0;
        uc_vap_id = WLAN_VAP_SUPPORT_MAX_NUM_LIMIT;

        /* vap id从高到低恢复 */
        for (; uc_vap_id > oal_board_get_service_vap_start_id(); uc_vap_id--) {
            hmac_vap_wake_subq(uc_vap_id - 1, us_queue_idx);
        }
    }
}


OAL_STATIC void hmac_vap_stop_subq(uint8_t uc_vap_id, uint16_t us_queue_idx)
{
    oal_net_device_stru *pst_net_device = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
    if (pst_hmac_vap == NULL) {
        return;
    }

    pst_net_device = pst_hmac_vap->pst_net_device;
    if (pst_net_device == NULL) {
        return;
    }

    oal_net_stop_subqueue(pst_net_device, us_queue_idx);
}


void hmac_vap_net_stop_subqueue(uint16_t us_queue_idx)
{
    uint8_t uc_vap_id;
    OAL_STATIC uint8_t g_uc_stop_subq_flag = 0;

    /* 自旋锁内，任务和软中断都被锁住，不需要FRW锁 */
    /* 由按照VAP ID顺序停止subq，改为不依据VAP ID顺序 */
    if (g_uc_stop_subq_flag == 0) {
        g_uc_stop_subq_flag = 1;
        uc_vap_id = oal_board_get_service_vap_start_id();

        for (; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
            hmac_vap_stop_subq(uc_vap_id, us_queue_idx);
        }
    } else {
        g_uc_stop_subq_flag = 0;
        uc_vap_id = WLAN_VAP_SUPPORT_MAX_NUM_LIMIT;

        for (; uc_vap_id > oal_board_get_service_vap_start_id(); uc_vap_id--) {
            hmac_vap_stop_subq(uc_vap_id - 1, us_queue_idx);
        }
    }
}


void hmac_vap_net_startall(void)
{
    uint8_t uc_vap_id;
    oal_net_device_stru *pst_net_device = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == NULL) {
            break;
        }

        pst_net_device = pst_hmac_vap->pst_net_device;

        if (pst_net_device == NULL) {
            break;
        }

        oal_net_tx_wake_all_queues(pst_net_device);
    }
}


void hmac_del_virtual_inf_worker(oal_work_stru *pst_del_virtual_inf_work)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap;
    oal_wireless_dev_stru *pst_wireless_dev = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    pst_hmac_vap = oal_container_of(pst_del_virtual_inf_work, hmac_vap_stru, st_del_virtual_inf_worker);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_P2P, "{hmac_del_virtual_inf_worker:: hmac_vap is null}");
        return;
    }
    pst_net_dev = pst_hmac_vap->pst_del_net_device;
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_del_virtual_inf_worker:: net_dev is null}");
        return;
    }
    pst_wireless_dev = oal_netdevice_wdev(pst_net_dev);

    /* 不存在rtnl_lock锁问题 */
    oal_net_unregister_netdev(pst_net_dev);

    /* 在释放net_device 后释放wireless_device 内存 */
    oal_mem_free_m(pst_wireless_dev, OAL_TRUE);

    pst_hmac_vap->pst_del_net_device = OAL_PTR_NULL;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_del_virtual_inf_worker::pst_hmac_device is null !device_id[%d]}",
                       pst_hmac_vap->st_vap_base_info.uc_device_id);
        return;
    }

    hmac_clr_p2p_status(&pst_hmac_device->ul_p2p_intf_status, P2P_STATUS_IF_DELETING);

    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                     "{hmac_del_virtual_inf_worker::end !pst_hmac_device->ul_p2p_intf_status[%x]}",
                     pst_hmac_device->ul_p2p_intf_status);
    oal_smp_mb();
    oal_wait_queue_wake_up_interrupt(&pst_hmac_device->st_netif_change_event);
}

#ifdef _PRE_WLAN_FEATURE_SAE

void hmac_report_ext_auth_worker(oal_work_stru *pst_sae_report_ext_auth_worker)
{
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = oal_container_of(pst_sae_report_ext_auth_worker, hmac_vap_stru, st_sae_report_ext_auth_worker);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_report_ext_auth_worker:: hmac_vap is null}");
        return;
    }

    oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                     "{hmac_report_ext_auth_worker:: hmac_report_external_auth_req auth start}");

    hmac_report_external_auth_req(pst_hmac_vap, NL80211_EXTERNAL_AUTH_START);
}
#endif


void hmac_handle_disconnect_rsp(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
                                        mac_reason_code_enum_uint16 en_disasoc_reason)
{
    /* 修改 state & 删除 user */
    switch (pst_hmac_vap->st_vap_base_info.en_vap_mode) {
        case WLAN_VAP_MODE_BSS_AP: {
            /* 抛事件上报内核，已经去关联某个STA */
            hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);
            break;
        }
        case WLAN_VAP_MODE_BSS_STA: {
            /* 上报内核sta已经和某个ap去关联 */
            hmac_sta_handle_disassoc_rsp(pst_hmac_vap, en_disasoc_reason);
            break;
        }
        default:
            break;
    }
    return;
}


uint8_t *hmac_vap_get_pmksa(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_bssid)
{
    hmac_pmksa_cache_stru *pst_pmksa_cache = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_pmksa_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_pmksa_entry_tmp = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_hmac_vap, puc_bssid)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_vap_get_pmksa param null}\r\n");
        return OAL_PTR_NULL;
    }

    if (WLAN_WITP_AUTH_SAE == mac_mib_get_AuthenticationMode(&(pst_hmac_vap->st_vap_base_info))) {
        return OAL_PTR_NULL;
    }

    if (oal_dlist_is_empty(&(pst_hmac_vap->st_pmksa_list_head))) {
        return OAL_PTR_NULL;
    }

    oal_dlist_search_for_each_safe(pst_pmksa_entry, pst_pmksa_entry_tmp, &(pst_hmac_vap->st_pmksa_list_head))
    {
        pst_pmksa_cache = oal_dlist_get_entry(pst_pmksa_entry, hmac_pmksa_cache_stru, st_entry);
        if (0 == oal_compare_mac_addr(puc_bssid, pst_pmksa_cache->auc_bssid)) {
            oal_dlist_delete_entry(pst_pmksa_entry);
            oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                             "{hmac_vap_get_pmksa:: FIND Pmksa of [%02X:XX:XX:XX:%02X:%02X]}",
                             puc_bssid[0], puc_bssid[4], puc_bssid[5]);
            break;
        }
        pst_pmksa_cache = OAL_PTR_NULL;
    }

    if (pst_pmksa_cache != OAL_PTR_NULL) {
        oal_dlist_add_head(&(pst_pmksa_cache->st_entry), &(pst_hmac_vap->st_pmksa_list_head));
        return pst_pmksa_cache->auc_pmkid;
    }
    return OAL_PTR_NULL;
}


uint32_t hmac_tx_get_mac_vap(uint8_t uc_vap_id, mac_vap_stru **pst_vap_stru)
{
    mac_vap_stru *pst_vap;

    /* 获取vap结构信息 */
    pst_vap = (mac_vap_stru *)mac_res_get_mac_vap(uc_vap_id);
    if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
        oam_error_log0(uc_vap_id, OAM_SF_TX, "{hmac_tx_get_mac_vap::pst_vap null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* VAP模式判断 */
    if (pst_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) {
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX,
                         "hmac_tx_get_mac_vap::vap_mode error[%d]",
                         pst_vap->en_vap_mode);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /* VAP状态判断 */
    if (pst_vap->en_vap_state != MAC_VAP_STATE_UP && pst_vap->en_vap_state != MAC_VAP_STATE_PAUSE) {
        oam_warning_log1(pst_vap->uc_vap_id, OAM_SF_TX,
                         "hmac_tx_get_mac_vap::vap_state[%d] error",
                         pst_vap->en_vap_state);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    *pst_vap_stru = pst_vap;

    return OAL_SUCC;
}


uint32_t hmac_restart_all_work_vap(uint8_t uc_chip_id)
{
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;
    mac_device_stru *pst_mac_dev = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t uc_dev_idx;
    uint8_t uc_vap_idx;
    mac_cfg_down_vap_param_stru st_down_vap;
    mac_cfg_start_vap_param_stru st_start_vap;
    uint32_t ul_ret;

    pst_mac_chip = hmac_res_get_mac_chip(uc_chip_id);
    if (oal_unlikely(pst_mac_chip == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_restart_all_work_vap::pst_mac_chip null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 遍历chip下的所有vap，将work的vap down掉后再up */
    for (uc_dev_idx = 0; uc_dev_idx < pst_mac_chip->uc_device_nums; uc_dev_idx++) {
        pst_mac_dev = mac_res_get_dev(pst_mac_chip->auc_device_id[uc_dev_idx]);
        if (oal_unlikely(pst_mac_dev == OAL_PTR_NULL)) {
            oam_warning_log0(0, OAM_SF_CFG, "{hmac_restart_all_work_vap::pst_mac_dev null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        /* 遍历dev下的所有vap */
        for (uc_vap_idx = 0; uc_vap_idx < pst_mac_dev->uc_vap_num; uc_vap_idx++) {
            pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_dev->auc_vap_id[uc_vap_idx]);
            if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
                oam_warning_log0(pst_mac_dev->auc_vap_id[uc_vap_idx], OAM_SF_CFG,
                                 "{hmac_restart_all_work_vap::pst_hmac_vap null.}");
                return OAL_ERR_CODE_PTR_NULL;
            }

            /* update vap's maximum user num */
            mac_mib_set_MaxAssocUserNums(&pst_hmac_vap->st_vap_base_info, mac_chip_get_max_asoc_user(uc_chip_id));

            /* return if not work vap */
            if (pst_hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_INIT) {
                continue;
            }

            /* restart work vap */
            /* down vap */
            st_down_vap.pst_net_dev = pst_hmac_vap->pst_net_device;
            ul_ret = hmac_config_down_vap(&pst_hmac_vap->st_vap_base_info,
                                              OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                                              (uint8_t *)&st_down_vap);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                                 "{hmac_restart_all_work_vap::hmac_config_down_vap failed[%d].}", ul_ret);
                return ul_ret;
            }
            /* down netdev */
            oal_net_device_close(pst_hmac_vap->pst_net_device);

            /* start vap */
            st_start_vap.pst_net_dev = pst_hmac_vap->pst_net_device;
            st_start_vap.en_mgmt_rate_init_flag = OAL_TRUE;
            ul_ret = hmac_config_start_vap(&pst_hmac_vap->st_vap_base_info,
                                               OAL_SIZEOF(mac_cfg_start_vap_param_stru),
                                               (uint8_t *)&st_start_vap);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                                 "{hmac_config_wifi_enable::hmac_config_start_vap failed[%d].}", ul_ret);
                return ul_ret;
            }
            /* up netdev */
            oal_net_device_open(pst_hmac_vap->pst_net_device);
        }
    }

    return OAL_SUCC;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
OAL_STATIC int32_t hmac_cfg_vap_if_open(oal_net_device_stru *pst_dev)
{
    pst_dev->flags |= OAL_IFF_RUNNING;

    return OAL_SUCC;
}

OAL_STATIC int32_t hmac_cfg_vap_if_close(oal_net_device_stru *pst_dev)
{
    pst_dev->flags &= ~OAL_IFF_RUNNING;

    return OAL_SUCC;
}

OAL_STATIC oal_net_dev_tx_enum hmac_cfg_vap_if_xmit(oal_netbuf_stru *pst_buf, oal_net_device_stru *pst_dev)
{
    if (pst_buf != OAL_PTR_NULL) {
        oal_netbuf_free(pst_buf);
    }
    return OAL_NETDEV_TX_OK;
}
#endif

void hmac_vap_state_restore(hmac_vap_stru *pst_hmac_vap)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    pst_hmac_device = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_vap_state_restore::pst_hmac_device[%d] null.}",
                         pst_hmac_vap->st_vap_base_info.uc_device_id);
        return ;
    }

    if (pst_hmac_vap->st_vap_base_info.en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                                 pst_hmac_device->pst_device_base_info->st_p2p_info.en_last_vap_state);
    } else {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                                 pst_hmac_vap->st_vap_base_info.en_last_vap_state);
    }

    return ;
}

/*lint -e19*/
oal_module_symbol(hmac_vap_get_priv_cfg);
oal_module_symbol(hmac_vap_get_net_device);
oal_module_symbol(hmac_vap_get_desired_country);
oal_module_symbol(hmac_vap_destroy);

#ifdef _PRE_WLAN_FEATURE_11D
oal_module_symbol(hmac_vap_get_updata_rd_by_ie_switch);
#endif
oal_module_symbol(hmac_del_virtual_inf_worker);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

