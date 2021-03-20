

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
    // ���ƻ����
    /* ����hostȫ�ֱ���ֵ */
    void (*host_global_init_param)(void);

    /* �����״δ��ϵ��־λˢ�� */
    void (*first_power_on_mark)(void);

    /* ��ȡӲ���Ƿ�֧��160M APUT */
    uint32_t (*update_aput_160M_enable)(oal_bool_enum_uint8 *en_ap_support_160m);

    /* ��ȡ���ƻ����ز��� */
    struct wlan_flow_ctrl_params (*get_flow_ctrl_used_mem)(void);

    /* hmac�����·���dmac�����ж� */
    /* ����ֵ��TRUE:���˸�������·�dmac��FALSE:�����·�dmac */
    uint32_t (*h2d_cmd_need_filter)(uint32_t cmd_id);

    /* ��ȡini�ļ������¶��ƻ���Ա��ֵ */
    uint32_t (*force_update_custom_params)(void);

    /* �·�host���ƻ�nv������device */
    uint32_t (*init_nvram_main)(oal_net_device_stru *cfg_net_dev);

    /* ���ݶ��ƻ�����CPU��Ƶ���� */
    void (*cpu_freq_ini_param_init)(void);

    /* ��ȡȫ�����ƻ����� */
    void (*host_global_ini_param_init)(void);

    /* ��ȡ���ƻ���������ѧϰ��־ */
    uint8_t (*get_selfstudy_country_flag)(void);

    /* host���ϵ�У׼�¼���У׼�����·��ӿ� */
    uint32_t (*custom_cali)(void);

    /* ��ȡ11ax�������ò��� */
    uint32_t (*get_11ax_switch_mask)(void);

    /* ��ȡ2G 11AC �����Ƿ�ʹ�ܿ��� */
    /* ����ֵ��TRUE:2G  11AC����ʹ�ܣ�FALSE:2G 11AC���ܹر� */
    uint32_t (*get_11ac2g_enable)(void);

    /* ��ȡ���ƻ��ļ�probe_respģʽ��ʶ */
    uint32_t (*get_probe_resp_mode)(void);

    /* ���µ���wal_cfg80211_mgmt_tx ����ʱ�� */
    uint32_t (*update_cfg80211_mgmt_tx_wait_time)(uint32_t wait_time);

    // �շ��;ۺ����
    /* �ж��Ƿ���Ҫ����BA�Ự */
    /* ����ֵ��TRUE:������BA�Ự��FALSE:��������BA �Ự */
    uint32_t (*check_need_setup_ba_session)(void);

    /* ���ݵ�ǰ���¾���amsdu�ۺϸ��� */
    void (*tx_update_amsdu_num)(mac_vap_stru *pst_mac_vap, uint32_t ul_tx_throughput_mbps,
        oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu);

    /* ��ʼ��ba��ز��� */
    void (*ba_rx_hdl_init)(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, uint8_t uc_tid);

    /* �������Ƿ���Ҫ����BAR֡ */
    /* ����ֵ��TRUE:��Ҫ����BAR֡��FALSE:����Ҫ����BAR֡ */
    uint32_t (*check_need_process_bar)(void);

    /* �ϱ�����������г�ʱ�ı��� */
    uint32_t (*ba_send_reorder_timeout)(hmac_ba_rx_stru *rx_ba, hmac_vap_stru *hmac_vap,
        hmac_ba_alarm_stru *alarm_data, uint32_t *pus_timeout);

    /* ������յ�ADDBA REQ,�Ƿ���BA LUT index */
    /* ����ֵ��TRUE:��Ҫ���BA LUT index;FALSE:����Ҫ���BA LUT index */
    uint32_t (*ba_need_check_lut_idx)(void);

    // У׼���
    /* �·�У׼�õ��ľ�����Ϣ */
    uint32_t (*send_cali_matrix_data)(mac_vap_stru *mac_vap);

    /* У׼�����·� */
    uint32_t (*send_cali_data)(mac_vap_stru *mac_vap);

    /* ����device�ϱ���У׼���� */
    uint32_t (*save_cali_event)(frw_event_mem_stru *event_mem);

    /* 11ax mibֵ��ʼ�� */
    void (*mac_vap_init_mib_11ax)(mac_vap_stru *mac_vap, uint32_t nss_num);

    /* ����auth��ʱʱ�� */
    void (*mac_mib_set_auth_rsp_time_out)(mac_vap_stru *mac_vap);

    /* ��ǰvapģʽ�Ƿ�����user htc cap */
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


