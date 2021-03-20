

#include "wlan_chip.h"
#include "oal_main.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "mac_hiex.h"
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_ftm.h"
#endif
#include "hmac_ext_if.h"
#include "hmac_auto_adjust_freq.h"
#include "hmac_blockack.h"
#include "hmac_cali_mgmt.h"
#include "hmac_tx_data.h"

#include "wal_linux_ioctl.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WLAN_CHIP_1106_C

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
OAL_STATIC void hwifi_1106_cfg_host_global_switch_init(void)
{
#ifndef _PRE_LINUX_TEST
    /*************************** 低功耗定制化 *****************************/
    /* 由于device 低功耗开关不是true false,而host是,先取定制化的值赋给device开关,再根据此值给host低功耗开关赋值 */
    g_wlan_device_pm_switch = g_cust_cap.wlan_device_pm_switch;
    g_wlan_ps_mode = g_cust_host.wlan_ps_mode;
    g_wlan_fast_ps_dyn_ctl = ((MAX_FAST_PS == g_wlan_ps_mode) ? 1 : 0);
    g_wlan_min_fast_ps_idle = g_cust_cap.fast_ps.wlan_min_fast_ps_idle;
    g_wlan_max_fast_ps_idle = g_cust_cap.fast_ps.wlan_max_fast_ps_idle;
    g_wlan_auto_ps_screen_on = g_cust_cap.fast_ps.wlan_auto_ps_thresh_screen_on;
    g_wlan_auto_ps_screen_off = g_cust_cap.fast_ps.wlan_auto_ps_thresh_screen_off;
    g_wlan_host_pm_switch = (g_wlan_device_pm_switch == WLAN_DEV_ALL_ENABLE ||
        g_wlan_device_pm_switch == WLAN_DEV_LIGHT_SLEEP_SWITCH_EN) ? OAL_TRUE : OAL_FALSE;

    g_feature_switch[HMAC_MIRACAST_SINK_SWITCH] =
        (g_cust_host.en_hmac_feature_switch[CUST_MIRACAST_SINK_SWITCH] == 1);
    g_feature_switch[HMAC_MIRACAST_REDUCE_LOG_SWITCH] =
        (g_cust_host.en_hmac_feature_switch[CUST_MIRACAST_REDUCE_LOG_SWITCH] == 1);
    g_feature_switch[HMAC_CORE_BIND_SWITCH] =
        (g_cust_host.en_hmac_feature_switch[CUST_CORE_BIND_SWITCH] == 1);
#endif
}

OAL_STATIC void hwifi_1106_cfg_host_global_init_sounding(void)
{
    g_pst_mac_device_capability[0].en_rx_stbc_is_supp = g_cust_cap.en_rx_stbc_is_supp;
    g_pst_mac_device_capability[0].en_tx_stbc_is_supp = g_cust_cap.en_tx_stbc_is_supp;
    g_pst_mac_device_capability[0].en_su_bfmer_is_supp = g_cust_cap.en_su_bfmer_is_supp;
    g_pst_mac_device_capability[0].en_su_bfmee_is_supp = g_cust_cap.en_su_bfmee_is_supp;
    g_pst_mac_device_capability[0].en_mu_bfmer_is_supp = g_cust_cap.en_mu_bfmer_is_supp;
    g_pst_mac_device_capability[0].en_mu_bfmee_is_supp = g_cust_cap.en_mu_bfmee_is_supp;
}

OAL_STATIC void hwifi_1106_cfg_host_global_init_param_extend(void)
{
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    if (hwifi_get_cust_read_status(CUS_TAG_HOST, WLAN_CFG_HOST_LOCK_CPU_FREQ)) {
        g_freq_lock_control.uc_lock_max_cpu_freq = g_cust_host.lock_max_cpu_freq;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (hwifi_get_cust_read_status(CUS_TAG_CAP, WLAN_CFG_CAP_HIEX_CAP)) {
        if (memcpy_s(&g_st_default_hiex_cap, OAL_SIZEOF(mac_hiex_cap_stru), &g_cust_cap.hiex_cap,
            OAL_SIZEOF(mac_hiex_cap_stru)) != EOK) {
            oal_io_print("hwifi_cfg_host_global_init_param_extend:hiex cap memcpy_s fail!");
        }
    }
    oal_io_print("hwifi_1106_cfg_host_global_init_param_extend:hiex cap[0x%X]\r\n",
        *(oal_uint32 *)&g_st_default_hiex_cap);
#endif
    if (hwifi_get_cust_read_status(CUS_TAG_HOST, WLAN_CFG_HOST_OPTIMIZED_FEATURE_SWITCH)) {
        g_optimized_feature_switch_bitmap = g_cust_cap.optimized_feature_mask;
    }

    if (hwifi_get_cust_read_status(CUS_TAG_CAP, WLAN_CFG_CAP_TX_SWITCH)) {
        hmac_set_tx_switch(g_cust_cap.tx_switch);
    }

#ifdef _PRE_WLAN_FEATURE_FTM
    if (hwifi_get_cust_read_status(CUS_TAG_CAP, WLAN_CFG_CAP_FTM)) {
        g_mac_ftm_cap = g_cust_cap.ftm_cap;
    }
#endif
}

OAL_STATIC void hwifi_1106_cfg_host_global_init_param(void)
{
    uint8_t device_idx;

    hwifi_1106_cfg_host_global_switch_init();
    /*************************** 私有定制化 *******************************/
    for (device_idx = 0; device_idx < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP; device_idx++) {
        /* 定制化 RADIO_0高4bit 给dbdc软件开关用 */
        wlan_service_device_per_chip[device_idx] = g_cust_cap.radio_cap[device_idx] & 0x0F;
    }
    /* 同步host侧业务device */
    memcpy_s(g_auc_mac_device_radio_cap, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP,
             wlan_service_device_per_chip, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP);
        g_pst_mac_device_capability[0].en_channel_width = g_cust_cap.en_channel_width;
        g_pst_mac_device_capability[0].en_ldpc_is_supp = g_cust_cap.en_ldpc_is_supp;

    hwifi_1106_cfg_host_global_init_sounding();

    g_pst_mac_device_capability[0].en_1024qam_is_supp = g_cust_cap.en_1024qam_is_supp;
    g_download_rate_limit_pps = g_cust_cap.download_rate_limit_pps;
    hwifi_1106_cfg_host_global_init_param_extend();
    return;
}

OAL_STATIC void wlan_first_powon_mark_1106(void)
{
    oal_bool_enum_uint8 cali_fst_pwr_on = OAL_TRUE;
    int32_t l_priv_value = 0;

    if (hwifi_get_cust_read_status(CUS_TAG_CAP, WLAN_CFG_CAP_CALI_DATA_MASK)) {
        cali_fst_pwr_on = !!(HI1103_CALI_FIST_POWER_ON_MASK & g_cust_cap.cali_data_mask);
        oal_io_print("host_module_init_etc:cali_fst_pwr_on pri_val[0x%x]first_pow[%d]\r\n",
                     l_priv_value, cali_fst_pwr_on);
    }
}

OAL_STATIC uint32_t wlan_chip_update_aput_160M_enable_1106(oal_bool_enum_uint8 *en_ap_support_160m)
{
    if (hwifi_get_cust_read_status(CUS_TAG_CAP, WLAN_CFG_CAP_APUT_160M_ENABLE)) {
        *en_ap_support_160m = g_cust_cap.aput_160M_switch;
        return OAL_SUCC;
    }
    return OAL_FAIL;
}

OAL_STATIC struct wlan_flow_ctrl_params wlan_chip_get_flow_ctrl_used_mem_1106(void)
{
    struct wlan_flow_ctrl_params flow_ctrl;
    flow_ctrl.start = g_cust_cap.used_mem_for_start;
    flow_ctrl.stop  = g_cust_cap.used_mem_for_stop;
    return flow_ctrl;
}

OAL_STATIC uint32_t hwifi_force_update_rf_params_1106(void)
{
    return hwifi_1106_config_init(CUS_TAG_POW);
}

OAL_STATIC uint8_t wlan_chip_get_selfstudy_country_flag_1106(void)
{
    return g_cust_cap.en_country_self_study;
}

OAL_STATIC uint32_t wlan_chip_get_11ax_switch_mask_1106(void)
{
    return g_cust_cap.wifi_11ax_switch_mask;
}

OAL_STATIC uint32_t wlan_chip_get_11ac2g_enable_1106(void)
{
    return g_cust_host.wlan_11ac2g_switch;
}

OAL_STATIC uint32_t wlan_chip_get_probe_resp_mode_1106(void)
{
    return g_cust_host.wlan_probe_resp_mode;
}
#endif // _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC uint32_t wlan_chip_h2d_cmd_need_filter_1106(uint32_t cmd_id)
{
    return OAL_FALSE; /* 不用过滤 */
}

OAL_STATIC uint32_t wlan_chip_update_cfg80211_mgmt_tx_wait_time_1106(uint32_t wait_time)
{
    if (wait_time <= 100) { // 100 1106 FPGA 切换信道时间较长，对于等待时间小于100ms 发帧，修改为150ms
        wait_time = 150;    // 小于100ms 监听时间，增加为150ms，增加发送时间。避免管理帧未来得及发送，超时定时器就结束。
    }
    return wait_time;
}

OAL_STATIC uint32_t wlan_chip_check_need_setup_ba_session_1106(void)
{
    /* 06 device tx由于硬件约束不允许建立聚合 */
    return (hmac_get_tx_switch() == DEVICE_TX) ? OAL_FALSE : OAL_TRUE;
}

OAL_STATIC uint32_t wlan_chip_check_need_process_bar_1106(void)
{
    /* 1106 硬件处理bar，软件不需要处理 */
    return OAL_FALSE;
}

OAL_STATIC uint32_t wlan_chip_ba_need_check_lut_idx_1106(void)
{
    /* 1106无lut限制，不用检查LUT idx */
    return OAL_FALSE;
}

OAL_STATIC void wlan_chip_mac_mib_set_auth_rsp_time_out_1106(mac_vap_stru *mac_vap)
{
    if (mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        mac_mib_set_AuthenticationResponseTimeOut(mac_vap, WLAN_AUTH_AP_TIMEOUT);
    } else {
        /* 1106FPGA阶段 入网auth时间加长为1105一倍 */
        mac_mib_set_AuthenticationResponseTimeOut(mac_vap, WLAN_AUTH_TIMEOUT * 2); /* 入网时间增加2倍 */
    }
}

const struct wlan_chip_ops g_wlan_chip_ops_1106 = {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    .host_global_init_param = hwifi_1106_cfg_host_global_init_param,
    .first_power_on_mark = wlan_first_powon_mark_1106,
    .update_aput_160M_enable = wlan_chip_update_aput_160M_enable_1106,
    .get_flow_ctrl_used_mem = wlan_chip_get_flow_ctrl_used_mem_1106,
    .force_update_custom_params = hwifi_force_update_rf_params_1106,
    .init_nvram_main = hwifi_config_init_nvram_main_1106,
    .cpu_freq_ini_param_init = hwifi_1106_config_cpu_freq_ini_param,
    .host_global_ini_param_init = hwifi_1106_config_host_global_ini_param,
    .get_selfstudy_country_flag = wlan_chip_get_selfstudy_country_flag_1106,
    .custom_cali = wal_1106_custom_cali,
    .get_11ax_switch_mask = wlan_chip_get_11ax_switch_mask_1106,
    .get_11ac2g_enable = wlan_chip_get_11ac2g_enable_1106,
    .get_probe_resp_mode = wlan_chip_get_probe_resp_mode_1106,
#endif
    .h2d_cmd_need_filter = wlan_chip_h2d_cmd_need_filter_1106,
    .update_cfg80211_mgmt_tx_wait_time = wlan_chip_update_cfg80211_mgmt_tx_wait_time_1106,
    // 收发和聚合相关
    .ba_rx_hdl_init = hmac_ba_rx_win_init,
    .check_need_setup_ba_session = wlan_chip_check_need_setup_ba_session_1106,
    .tx_update_amsdu_num = hmac_update_amsdu_num, /* 注意：该函数为1103使用，1106当前host发送流程不会走到该函数 */
    .check_need_process_bar = wlan_chip_check_need_process_bar_1106,
    .ba_send_reorder_timeout = hmac_ba_send_ring_reorder_timeout,
    .ba_need_check_lut_idx = wlan_chip_ba_need_check_lut_idx_1106,
    // 校准相关
    .send_cali_matrix_data = hmac_send_1106_cali_matrix_data,
    .send_cali_data = hmac_send_cali_data_1106,
    .save_cali_event = hmac_save_cali_event_1106,
#ifdef _PRE_WLAN_FEATURE_11AX
    .mac_vap_init_mib_11ax = mac_vap_init_mib_11ax_1106,
#endif
    .mac_mib_set_auth_rsp_time_out = wlan_chip_mac_mib_set_auth_rsp_time_out_1106,
    .mac_vap_need_set_user_htc_cap = mac_vap_need_set_user_htc_cap_1106,
};

