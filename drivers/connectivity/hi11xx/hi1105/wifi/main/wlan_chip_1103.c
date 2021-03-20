

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
#define THIS_FILE_ID OAM_FILE_ID_WLAN_CHIP_1103_C

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
OAL_STATIC void hwifi_cfg_host_global_switch_init(void)
{
#ifndef _PRE_LINUX_TEST
    /*************************** 低功耗定制化 *****************************/
    /* 由于device 低功耗开关不是true false,而host是,先取定制化的值赋给device开关,再根据此值给host低功耗开关赋值 */
    g_wlan_device_pm_switch = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_POWERMGMT_SWITCH);
    g_wlan_ps_mode = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_PS_MODE);
    g_wlan_fast_ps_dyn_ctl = ((MAX_FAST_PS == g_wlan_ps_mode) ? 1 : 0);
    g_wlan_min_fast_ps_idle = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MIN_FAST_PS_IDLE);
    g_wlan_max_fast_ps_idle = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MAX_FAST_PS_IDLE);
    g_wlan_auto_ps_screen_on = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENON);
    g_wlan_auto_ps_screen_off = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENOFF);
    g_wlan_host_pm_switch = (g_wlan_device_pm_switch == WLAN_DEV_ALL_ENABLE ||
        g_wlan_device_pm_switch == WLAN_DEV_LIGHT_SLEEP_SWITCH_EN) ? OAL_TRUE : OAL_FALSE;

    g_feature_switch[HMAC_MIRACAST_SINK_SWITCH] =
        (hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MIRACAST_SINK_ENABLE) == 1);
    g_feature_switch[HMAC_MIRACAST_REDUCE_LOG_SWITCH] =
        (hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_REDUCE_MIRACAST_LOG) == 1);
    g_feature_switch[HMAC_CORE_BIND_SWITCH] =
        (hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CORE_BIND_CAP) == 1);
#endif
}

OAL_STATIC void hwifi_cfg_host_global_init_sounding(void)
{
    int32_t l_priv_value = 0;
    int32_t l_ret;

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_RX_STBC, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: rx stbc[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_rx_stbc_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TX_STBC, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: tx stbc[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_tx_stbc_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_SU_BFER, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: su bfer[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_su_bfmer_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_SU_BFEE, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: su bfee[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_su_bfmee_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_MU_BFER, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: mu bfer[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_mu_bfmer_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_MU_BFEE, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: mu bfee[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_mu_bfmee_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }
}


OAL_STATIC void hwifi_cfg_host_global_init_param_extend(void)
{
#if (defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)) || \
    defined(_PRE_WLAN_FEATURE_HIEX)
    int32_t priv_value = 0;
    int32_t l_ret;
#endif

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_LOCK_CPU_FREQ, &priv_value);
    if (l_ret == OAL_SUCC) {
        g_freq_lock_control.uc_lock_max_cpu_freq = (oal_bool_enum_uint8) !!priv_value;
    }
    oal_io_print("hwifi_cfg_host_global_init_param_extend:lock_max_cpu_freq[%d]\r\n",
        g_freq_lock_control.uc_lock_max_cpu_freq);
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_HIEX_CAP, &priv_value);
    if (l_ret == OAL_SUCC) {
        if (memcpy_s(&g_st_default_hiex_cap, OAL_SIZEOF(mac_hiex_cap_stru), &priv_value,
            OAL_SIZEOF(mac_hiex_cap_stru)) != EOK) {
            oal_io_print("hwifi_cfg_host_global_init_param_extend:hiex cap memcpy_s fail!");
        }
    }
    oal_io_print("hwifi_cfg_host_global_init_param_extend:hiex cap[0x%X]\r\n", *(uint32_t *)&g_st_default_hiex_cap);
#endif
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_OPTIMIZED_FEATURE_SWITCH, &priv_value);
    if (l_ret == OAL_SUCC) {
        g_optimized_feature_switch_bitmap = (uint8_t)priv_value;
    }

    if (hwifi_get_init_priv_value(WLAN_CFG_PRIV_TX_SWITCH, &priv_value) == OAL_SUCC) {
        hmac_set_tx_switch(priv_value >= 0 && priv_value < TX_SWITCH_BUTT ? priv_value : WLAN_DEFAULT_TX_SWITCH);
    }

#ifdef _PRE_WLAN_FEATURE_FTM
        l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_FTM_CAP, &priv_value);
        if (l_ret == OAL_SUCC) {
            g_mac_ftm_cap = (uint8_t)priv_value;
        }
#endif
}

void hwifi_cfg_host_global_init_dbdc_param(void)
{
    uint8_t uc_cmd_idx;
    uint8_t uc_device_idx;
    int32_t l_priv_value = 0;
    int32_t l_ret;

    uc_cmd_idx = WLAN_CFG_PRIV_DBDC_RADIO_0;
    for (uc_device_idx = 0; uc_device_idx < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP; uc_device_idx++) {
        l_ret = hwifi_get_init_priv_value(uc_cmd_idx, &l_priv_value);
        if (l_ret == OAL_SUCC) {
            /* 定制化 RADIO_0高4bit 给dbdc软件开关用 */
            l_priv_value = (int32_t)((uint32_t)l_priv_value & 0x0F);
            wlan_service_device_per_chip[uc_device_idx] = (uint8_t)(uint32_t)l_priv_value;
        }

        uc_cmd_idx++;
    }
    /* 同步host侧业务device */
    memcpy_s(g_auc_mac_device_radio_cap, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP,
             wlan_service_device_per_chip, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP);
}

/*
 * 函 数 名  : hwifi_cfg_host_global_init_param_1103
 * 功能描述  : 初始化定制化ini文件host侧全局变量
 */
OAL_STATIC void hwifi_cfg_host_global_init_param_1103(void)
{
    int32_t l_priv_value = 0;
    int32_t l_ret;

    hwifi_cfg_host_global_switch_init();
    /*************************** 私有定制化 *******************************/
    hwifi_cfg_host_global_init_dbdc_param();
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_BW_MAX_WITH, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: bw max with[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_channel_width = (wlan_bw_cap_enum_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_LDPC_CODING, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: ldpc coding[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_ldpc_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }

    hwifi_cfg_host_global_init_sounding();

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_1024_QAM, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param: 1024 qam[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_pst_mac_device_capability[0].en_1024qam_is_supp = (oal_bool_enum_uint8)l_priv_value;
    }
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_DOWNLOAD_RATE_LIMIT_PPS, &l_priv_value);
    oal_io_print("hwifi_cfg_host_global_init_param:download rx drop threshold[%d] ret[%d]\r\n", l_priv_value, l_ret);
    if (l_ret == OAL_SUCC) {
        g_download_rate_limit_pps = (uint16_t)l_priv_value;
    }

    hwifi_cfg_host_global_init_param_extend();

    return;
}

OAL_STATIC void wlan_first_powon_mark_1103(void)
{
    oal_bool_enum_uint8 cali_fst_pwr_on = OAL_TRUE;
    int32_t l_priv_value = 0;

    if (hwifi_get_init_priv_value(WLAN_CFG_PRIV_CALI_DATA_MASK, &l_priv_value) == OAL_SUCC) {
        cali_fst_pwr_on = !!(HI1103_CALI_FIST_POWER_ON_MASK & (uint32_t)l_priv_value);
        oal_io_print("host_module_init:cali_fst_pwr_on pri_val[0x%x]first_pow[%d]\r\n",
            l_priv_value, cali_fst_pwr_on);
    }
}

OAL_STATIC uint32_t wlan_chip_update_aput_160M_enable_1103(oal_bool_enum_uint8 *en_ap_support_160m)
{
    int32_t l_priv_value;
    if (hwifi_get_init_priv_value(WLAN_CFG_APUT_160M_ENABLE, &l_priv_value) == OAL_SUCC) {
        *en_ap_support_160m = l_priv_value;
        return OAL_SUCC;
    }
    return OAL_FAIL;
}

OAL_STATIC struct wlan_flow_ctrl_params wlan_chip_get_flow_ctrl_used_mem_1103(void)
{
    struct wlan_flow_ctrl_params flow_ctrl;
    flow_ctrl.start = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_START);
    flow_ctrl.stop  = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_STOP);
    return flow_ctrl;
}

OAL_STATIC uint32_t hwifi_force_update_rf_params_1103(void)
{
    return hwifi_config_init(CUS_TAG_NV);
}

OAL_STATIC uint8_t wlan_chip_get_selfstudy_country_flag_1103(void)
{
    int32_t l_priv_value = 0;
    hwifi_get_init_priv_value(WLAN_CFG_PRIV_COUNRTYCODE_SELFSTUDY_CFG, &l_priv_value);
    return (uint8_t)l_priv_value;
}

OAL_STATIC uint32_t wlan_chip_get_11ax_switch_mask_1103(void)
{
    int32_t l_priv_value = 0;
    hwifi_get_init_priv_value(WLAN_CFG_PRIV_11AX_SWITCH, &l_priv_value);
    return l_priv_value;
}

OAL_STATIC uint32_t wlan_chip_get_11ac2g_enable_1103(void)
{
    return !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_11AC2G_ENABLE);
}

OAL_STATIC uint32_t wlan_chip_get_probe_resp_mode_1103(void)
{
    return hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_PROBE_RESP_MODE);
}
#endif // _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC uint32_t wlan_chip_h2d_cmd_need_filter_1103(uint32_t cmd_id)
{
    /* 对1103/1105产品，1106独有事件需要下发device */
    if ((cmd_id >= WLAN_CFGID_HI1106_PRIV_START) && (cmd_id <= WLAN_CFGID_HI1106_PRIV_END)) {
        return OAL_TRUE; /* 需要过滤 */
    }
    return OAL_FALSE; /* 不用过滤 */
}

OAL_STATIC uint32_t wlan_chip_update_cfg80211_mgmt_tx_wait_time_1103(uint32_t wait_time)
{
    return wait_time;
}

OAL_STATIC uint32_t wlan_chip_check_need_setup_ba_session_1103(void)
{
    return OAL_TRUE;
}

OAL_STATIC uint32_t wlan_chip_check_need_process_bar_1103(void)
{
    /* 1103/1105需要软件处理bar */
    return OAL_TRUE;
}

OAL_STATIC uint32_t wlan_chip_ba_need_check_lut_idx_1103(void)
{
    /* 1103/1105需要检查LUT idx */
    return OAL_TRUE;
}

OAL_STATIC void wlan_chip_mac_mib_set_auth_rsp_time_out_1103(mac_vap_stru *mac_vap)
{
    if (mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        mac_mib_set_AuthenticationResponseTimeOut(mac_vap, WLAN_AUTH_AP_TIMEOUT);
    } else {
        mac_mib_set_AuthenticationResponseTimeOut(mac_vap, WLAN_AUTH_TIMEOUT);
    }
}

const struct wlan_chip_ops g_wlan_chip_ops_1103 = {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    .host_global_init_param = hwifi_cfg_host_global_init_param_1103,
    .first_power_on_mark = wlan_first_powon_mark_1103,
    .update_aput_160M_enable = wlan_chip_update_aput_160M_enable_1103,
    .get_flow_ctrl_used_mem = wlan_chip_get_flow_ctrl_used_mem_1103,
    .force_update_custom_params = hwifi_force_update_rf_params_1103,
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    .init_nvram_main = hwifi_config_init_nvram_main_1103,
#endif
    .cpu_freq_ini_param_init = hwifi_config_cpu_freq_ini_param_1103,
    .host_global_ini_param_init = hwifi_config_host_global_ini_param_1103,
    .get_selfstudy_country_flag = wlan_chip_get_selfstudy_country_flag_1103,
    .custom_cali = wal_custom_cali_1103,
    .get_11ax_switch_mask = wlan_chip_get_11ax_switch_mask_1103,
    .get_11ac2g_enable = wlan_chip_get_11ac2g_enable_1103,
    .get_probe_resp_mode = wlan_chip_get_probe_resp_mode_1103,
#endif
    .h2d_cmd_need_filter = wlan_chip_h2d_cmd_need_filter_1103,
    .update_cfg80211_mgmt_tx_wait_time = wlan_chip_update_cfg80211_mgmt_tx_wait_time_1103,

    // 收发和聚合相关
    .ba_rx_hdl_init = hmac_ba_rx_hdl_init,
    .check_need_setup_ba_session = wlan_chip_check_need_setup_ba_session_1103,
    .tx_update_amsdu_num = hmac_update_amsdu_num, /* 注意：该函数1103/1105实现不同 */
    .check_need_process_bar = wlan_chip_check_need_process_bar_1103,
    .ba_send_reorder_timeout = hmac_ba_send_reorder_timeout,
    .ba_need_check_lut_idx = wlan_chip_ba_need_check_lut_idx_1103,

    // 校准相关
    .send_cali_matrix_data = hmac_send_cali_matrix_data_1103,
    .send_cali_data = hmac_send_cali_data_1103,
    .save_cali_event = hmac_save_cali_event_1103,
#ifdef _PRE_WLAN_FEATURE_11AX
    .mac_vap_init_mib_11ax = mac_vap_init_mib_11ax_1103,
#endif
    .mac_mib_set_auth_rsp_time_out = wlan_chip_mac_mib_set_auth_rsp_time_out_1103,

    .mac_vap_need_set_user_htc_cap = mac_vap_need_set_user_htc_cap_1103,
};

const struct wlan_chip_ops g_wlan_chip_ops_1105 = {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    .host_global_init_param = hwifi_cfg_host_global_init_param_1103,
    .first_power_on_mark = wlan_first_powon_mark_1103,
    .update_aput_160M_enable = wlan_chip_update_aput_160M_enable_1103,
    .get_flow_ctrl_used_mem = wlan_chip_get_flow_ctrl_used_mem_1103,
    .force_update_custom_params = hwifi_force_update_rf_params_1103,
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    .init_nvram_main = hwifi_config_init_nvram_main_1103,
#endif
    .cpu_freq_ini_param_init = hwifi_config_cpu_freq_ini_param_1103,
    .host_global_ini_param_init = hwifi_config_host_global_ini_param_1103,
    .get_selfstudy_country_flag = wlan_chip_get_selfstudy_country_flag_1103,
    .custom_cali = wal_custom_cali_1103,
    .get_11ax_switch_mask = wlan_chip_get_11ax_switch_mask_1103,
    .get_11ac2g_enable = wlan_chip_get_11ac2g_enable_1103,
    .get_probe_resp_mode = wlan_chip_get_probe_resp_mode_1103,
#endif
    .h2d_cmd_need_filter = wlan_chip_h2d_cmd_need_filter_1103,
    .update_cfg80211_mgmt_tx_wait_time = wlan_chip_update_cfg80211_mgmt_tx_wait_time_1103,

    // 收发和聚合相关
    .ba_rx_hdl_init = hmac_ba_rx_hdl_init,
    .check_need_setup_ba_session = wlan_chip_check_need_setup_ba_session_1103,
    .tx_update_amsdu_num = hmac_update_amsdu_num_1105, /* 注意：该函数1103/1105实现不同 */
    .check_need_process_bar = wlan_chip_check_need_process_bar_1103,
    .ba_send_reorder_timeout = hmac_ba_send_reorder_timeout,
    .ba_need_check_lut_idx = wlan_chip_ba_need_check_lut_idx_1103,

    // 校准相关
    .send_cali_matrix_data = hmac_send_1105_cali_matrix_data,
    .send_cali_data = hmac_send_cali_data_1105,
    .save_cali_event = hmac_save_cali_event_1105,
#ifdef _PRE_WLAN_FEATURE_11AX
    .mac_vap_init_mib_11ax = mac_vap_init_mib_11ax_1105,
#endif
    .mac_mib_set_auth_rsp_time_out = wlan_chip_mac_mib_set_auth_rsp_time_out_1103,

    .mac_vap_need_set_user_htc_cap = mac_vap_need_set_user_htc_cap_1103,
};

