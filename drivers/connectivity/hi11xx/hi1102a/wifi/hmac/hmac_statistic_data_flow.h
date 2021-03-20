
#ifndef __MAC_STATISTIC_DATA_FLOW_H__
#define __MAC_STATISTIC_DATA_FLOW_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "hmac_main.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/pm_qos.h>
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102A_HOST)
#include "hmac_auto_ddr_freq.h"
#endif
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <linux/hisi/hisi_cpufreq_req.h>
#include <linux/cpufreq.h>
#include <linux/hisi/hisi_core_ctl.h>
#endif
#endif
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define WLAN_STATIS_DATA_TIMER_PERIOD 100 /* ��ʱ��100ms��ʱ */
#define WLAN_THROUGHPUT_STA_PERIOD    20
#define WLAN_THROUGHPUT_LOAD_LOW      10 /* �͸���10M */
/* WIFI���������ϴ�ʱ���շ��жϰ��ڴ�� */
#define WLAN_IRQ_AFFINITY_IDLE_CPU 0
#define WLAN_IRQ_AFFINITY_BUSY_CPU 4
#define PM_QOS_THOUGHTPUT_VALUE    60
typedef struct {
    oal_atomic ul_tx_bytes; /* WIFI ҵ����֡ͳ�� */
    oal_atomic ul_rx_bytes; /* WIFI ҵ�����֡ͳ�� */
    oal_atomic ul_tx_large_pkt;
    oal_atomic ul_rx_large_pkt;
    oal_atomic ul_tx_small_pkt;
    oal_atomic ul_rx_small_pkt;
    oal_atomic ul_rx_large_amsdu_pkt;
    oal_atomic ul_rx_small_amsdu_pkt;
} wifi_statis_pkgs_info_stru;

typedef struct {
    wifi_statis_pkgs_info_stru st_pkgs_info;
    oal_bool_enum_uint8 en_wifi_rx_busy;
    oal_uint8 uc_timer_cycles;             /* ��ʱ�������� */
    oal_uint32 ul_pre_time;                /* ���ڼ��������� */
    oal_spin_lock_stru st_txrx_opt_lock;
    frw_timeout_stru st_statis_data_timer; /* �˳�Աһ��Ҫ�ǽṹ�����һ����Ա */
} wifi_txrx_pkt_statis_stru;

typedef struct {
    oal_uint32 ul_rx_throughput_mbps;
    oal_uint32 ul_tx_throughput_mbps;
    oal_uint32 ul_tx_large_pps;
    oal_uint32 ul_rx_large_pps;
    oal_uint32 ul_tx_small_pps;
    oal_uint32 ul_rx_small_pps;
    oal_uint32 ul_rx_large_amsdu_pps;
} wifi_txrx_calculated_stat_stru;

typedef enum {
    HMAC_TX_OPT_TPC_DISABLE = BIT0,
    HMAC_TX_OPT_EDCA_OPT = BIT1,
    HMAC_TX_OPT_TXOP_OPT = BIT2,
    HMAC_TX_OPT_PPDU_SCH = BIT3,
    HMAC_TX_OPT_APUT_TCP_ACK_FILTER = BIT4,
    HMAC_TX_OPT_STAUT_TCP_ACK_OPT = BIT5,
} hmac_tx_opt_switch_enum;
typedef oal_uint8 hmac_tx_opt_switch_enum_uint8;

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ

enum {
    /* frequency lock disable mode */
    H2D_FREQ_MODE_DISABLE = 0,
    /* frequency lock enable mode */
    H2D_FREQ_MODE_ENABLE = 1,
};
typedef oal_uint8 oal_freq_mode_enum_uint8;

typedef enum {
    CMD_SET_DEVICE_FREQ_ENDABLE,  // cmd type 0
    CMD_SET_DEVICE_FREQ_VALUE,
    CMD_SET_AUTO_BYPASS_DEVICE_AUTO_FREQ,
    CMD_GET_DEVICE_AUTO_FREQ,
    CMD_AUTO_FREQ_BUTT,
} oal_h2d_freq_cmd_enum;
typedef oal_uint8 oal_h2d_freq_cmd_enum_uint8;

typedef struct {
    oal_uint8 uc_device_type; /* device��Ƶ���� */
    oal_uint8 uc_reserve[3];  /* �����ֶ� */
} device_speed_freq_level_stru;

typedef struct {
    oal_uint32 ul_speed_level;  /* ���������� */
    oal_uint32 ul_min_cpu_freq; /* CPU��Ƶ���� */
    oal_uint32 ul_min_ddr_freq; /* DDR��Ƶ���� */
} host_speed_freq_level_stru;

typedef struct {
    oal_uint16 us_throughput_pps_irq_high;
    oal_uint16 us_throughput_pps_irq_low;
    oal_uint16 us_btcoex_throughput_pps_irq_high;
    oal_uint16 us_btcoex_throughput_pps_irq_low;
    oal_bool_enum_uint8 en_irq_cpu_state; /* ���״̬ */
    oal_bool_enum_uint8 en_irq_bindcpu;   /* ����ܿ��� */
    oal_uint8 uc_cpumask;
    oal_bool_enum_uint8 en_btcoex_flag; /* ��ʾBT������޲����Ƿ���Ч */
} thread_bindcpu_stru;

typedef struct {
    oal_uint8 uc_tx_opt_switch_sta;
    oal_uint8 uc_tx_opt_switch_ap;   /*
                                      * ���ܱ�ƴ�Ż�����(STA/AP��ͬ)
                                      * BIT0: TPC; BIT1: EDCA/TXOP; BIT2: DYN TXOP;
                                      * BIT3: PPDU SCH; BIT4: APUT TCP ACK FILTER;
                                      * BIT5-BIT7: RESERVE
                                      */
    oal_uint8 uc_cwmin;              /* ���ܱ�ƴ�Ż�cwmin */
    oal_uint8 uc_cwmax;              /* ���ܱ�ƴ�Ż�cwmax */
    oal_uint8 uc_resv;
    oal_uint16 us_txoplimit;         /* ���ܱ�ƴ�Ż�txoplimit */
    oal_uint16 us_dyn_txoplimit;     /* ���ܱ�ƴ�Ż���̬txoplimit */
    oal_uint16 us_txopt_th_pps_high; /* �Ż�pps���� */
    oal_uint16 us_txopt_th_pps_low;  /* ���������� */
} wifi_tx_opt_stru;

extern wifi_tx_opt_stru g_st_wifi_tx_opt;
extern host_speed_freq_level_stru g_host_speed_freq_level[];
extern device_speed_freq_level_stru g_device_speed_freq_level[];
extern thread_bindcpu_stru g_st_thread_bindcpu;
extern device_speed_freq_level_stru *hmac_wifi_get_device_speed_freq_level_addr(oal_void);
extern host_speed_freq_level_stru *hmac_wifi_get_host_speed_freq_level_addr(oal_void);
extern oal_uint32 hmac_wifi_get_host_speed_freq_level_size(oal_void);
extern oal_uint32 hmac_wifi_get_device_speed_freq_level_size(oal_void);
#endif
extern wifi_txrx_pkt_statis_stru g_st_wifi_rxtx_statis;

extern hmac_rxdata_thread_stru g_st_rxdata_thread;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
extern struct cpufreq_req g_ast_cpufreq[OAL_BUS_MAXCPU_NUM];
extern struct pm_qos_request g_st_pmqos_requset;
extern hisi_max_cpu_freq g_ast_cpumaxfreq[OAL_BUS_MAXCPU_NUM];
#endif
#endif

extern oal_void hmac_wifi_statistic_rx_amsdu(oal_uint32 ul_frame_len);
extern oal_void hmac_wifi_statistic_rx_tcp_ack(oal_uint32 ul_small_pkt_cnt,
                                               oal_uint32 ul_large_pkt_cnt,
                                               oal_uint32 ul_filtered_rx_bytes);
extern oal_void hmac_wifi_statistic_rx_packets(oal_uint32 ul_pkt_bytes);
extern oal_void hmac_wifi_statistic_tx_packets(oal_uint32 ul_pkt_bytes);
extern oal_bool_enum_uint8 hmac_wifi_rx_is_busy(oal_void);
extern oal_void hmac_wifi_calculate_throughput(oal_void);
extern oal_void hmac_wifi_statis_data_timer_init(oal_void);
extern oal_void hmac_wifi_statis_data_timer_deinit(oal_void);
extern oal_void hmac_wifi_pm_state_notify(oal_bool_enum_uint8 en_wake_up);
extern oal_void hmac_wifi_state_notify(oal_bool_enum_uint8 en_wifi_on);

extern oal_void hmac_wfd_statistic_tx_packets(mac_vap_stru *pst_vap);
#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
extern oal_uint8 hmac_get_tx_opt_switch(oal_void);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif




