

#ifndef __WLAN_CHIP_H__
#define __WLAN_CHIP_H__

#include "wlan_types.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "hmac_user.h"

struct wlan_flow_ctrl_params {
    uint16_t start;
    uint16_t stop;
};

struct wlan_chip_ops {
    // 定制化相关
    /* 配置host全局变量值 */
    void (*host_global_init_param)(void);

    /* 开机首次次上电标志位刷新 */
    void (*first_power_on_mark)(void);

    /* 获取硬件是否支持160M APUT */
    uint32_t (*update_aput_160M_enable)(oal_bool_enum_uint8 *en_ap_support_160m);

    /* 获取定制化流控参数 */
    struct wlan_flow_ctrl_params (*get_flow_ctrl_used_mem)(void);

    /* hmac命令下发到dmac过滤判断 */
    /* 返回值：TRUE:过滤该命令，不下发dmac；FALSE:命令下发dmac */
    uint32_t (*h2d_cmd_need_filter)(uint32_t cmd_id);

    /* 读取ini文件，更新定制化成员数值 */
    uint32_t (*force_update_custom_params)(void);

    /* 下发host定制化nv参数到device */
    uint32_t (*init_nvram_main)(oal_net_device_stru *cfg_net_dev);

    /* 根据定制化更新CPU调频参数 */
    void (*cpu_freq_ini_param_init)(void);

    /* 读取全部定制化配置 */
    void (*host_global_ini_param_init)(void);

    /* 读取定制化国家码自学习标志 */
    uint8_t (*get_selfstudy_country_flag)(void);

    /* host侧上电校准事件和校准参数下发接口 */
    uint32_t (*custom_cali)(void);

    /* 获取11ax特性配置参数 */
    uint32_t (*get_11ax_switch_mask)(void);

    /* 获取2G 11AC 特性是否使能开关 */
    /* 返回值：TRUE:2G  11AC功能使能；FALSE:2G 11AC功能关闭 */
    uint32_t (*get_11ac2g_enable)(void);

    /* 获取定制化文件probe_resp模式标识 */
    uint32_t (*get_probe_resp_mode)(void);

    /* 更新调用wal_cfg80211_mgmt_tx 监听时间 */
    uint32_t (*update_cfg80211_mgmt_tx_wait_time)(uint32_t wait_time);

    // 收发和聚合相关
    /* 判断是否需要建立BA会话 */
    /* 返回值：TRUE:允许建立BA会话；FALSE:不允许建立BA 会话 */
    uint32_t (*check_need_setup_ba_session)(void);

    /* 根据当前吞吐决定amsdu聚合个数 */
    void (*tx_update_amsdu_num)(mac_vap_stru *pst_mac_vap, uint32_t ul_tx_throughput_mbps,
        oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu);

    /* 初始化ba相关参数 */
    void (*ba_rx_hdl_init)(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, uint8_t uc_tid);

    /* 检查软件是否需要处理BAR帧 */
    /* 返回值：TRUE:需要处理BAR帧；FALSE:不需要处理BAR帧 */
    uint32_t (*check_need_process_bar)(void);

    /* 上报重排序队列中超时的报文 */
    uint32_t (*ba_send_reorder_timeout)(hmac_ba_rx_stru *rx_ba, hmac_vap_stru *hmac_vap,
        hmac_ba_alarm_stru *alarm_data, uint32_t *pus_timeout);

    /* 软件接收到ADDBA REQ,是否检查BA LUT index */
    /* 返回值：TRUE:需要检查BA LUT index;FALSE:不需要检查BA LUT index */
    uint32_t (*ba_need_check_lut_idx)(void);

    // 校准相关
    /* 下发校准用到的矩阵信息 */
    uint32_t (*send_cali_matrix_data)(mac_vap_stru *mac_vap);

    /* 校准数据下发 */
    uint32_t (*send_cali_data)(mac_vap_stru *mac_vap);

    /* 保存device上报的校准参数 */
    uint32_t (*save_cali_event)(frw_event_mem_stru *event_mem);

    /* 11ax mib值初始化 */
    void (*mac_vap_init_mib_11ax)(mac_vap_stru *mac_vap, uint32_t nss_num);

    /* 更新auth超时时间 */
    void (*mac_mib_set_auth_rsp_time_out)(mac_vap_stru *mac_vap);

    /* 当前vap模式是否设置user htc cap */
    oal_bool_enum_uint8 (*mac_vap_need_set_user_htc_cap)(mac_vap_stru *mac_vap);
};

extern const struct wlan_chip_ops *g_wlan_chip_ops;
extern const struct wlan_chip_ops g_wlan_chip_dummy_ops;
extern const struct wlan_chip_ops g_wlan_chip_ops_1103;
extern const struct wlan_chip_ops g_wlan_chip_ops_1105;
extern const struct wlan_chip_ops g_wlan_chip_ops_1106;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

extern oal_bool_enum g_wlan_host_pm_switch;
extern uint8_t g_wlan_device_pm_switch;
extern uint8_t g_wlan_ps_mode;
extern uint8_t g_wlan_fast_ps_dyn_ctl;
extern uint8_t g_wlan_min_fast_ps_idle;
extern uint8_t g_wlan_max_fast_ps_idle;
extern uint8_t g_wlan_auto_ps_screen_on;
extern uint8_t g_wlan_auto_ps_screen_off;
extern uint8_t g_auc_mac_device_radio_cap[];
extern uint16_t g_download_rate_limit_pps;

#endif  // #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE


#endif /* end of wlan_chip.h */


