

#include "oal_net.h"
#include "oal_types.h"
#include "oam_ext_if.h"
#include "mac_vap.h"
#include "mac_resource.h"
#include "hmac_vap.h"
#include "hmac_auto_adjust_freq.h"
#include "hmac_ext_if.h"
#include "hmac_blockack.h"
#include "hmac_tx_data.h"
#include "hal_host_ext_if.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#include <linux/pm_qos.h>
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <linux/hisi/hisi_cpufreq_req.h>
#include <linux/cpufreq.h>
#include <linux/hisi/hisi_core_ctl.h>
#endif
#endif

#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
#include "hmac_config.h"
#endif

#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_AUTO_ADJUST_FREQ_C

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
#define INVALID_PKMODE_TH (0xFFFFFFFF)
#endif
/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
#ifdef WIN32
uint32_t jiffies;
#endif

freq_lock_control_stru g_freq_lock_control = { 0 };
wifi_txrx_pkt_stat g_st_wifi_rxtx_total = { 0 };
/* Wi-Fi驱动收发负载识别数据区 */
freq_wifi_load_stru g_st_wifi_load = { 0 };
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
extern struct cpufreq_req g_ast_cpufreq[OAL_BUS_MAXCPU_NUM];
extern hisi_max_cpu_freq g_aul_cpumaxfreq[OAL_BUS_MAXCPU_NUM];
extern struct pm_qos_request g_st_pmqos_requset;
#endif
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
oal_bool_enum_uint8 g_en_pk_mode_swtich = OAL_TRUE;

/* pk mode的判定的门限，以字节为单位 */
/*
PK模式门限基数:
{(单位Mbps)  20M     40M    80M   160M   80+80M
lagency:    {valid, valid, valid, valid, valid},   (基础协议模式没有pk mode )
HT:         {62, 123, valid, valid, valid},
VHT:        {77, 167, 329, 660, 660},
HE:         {110, 205, 460, 750, 750},
};

PK模式二级门限:
高档位门限: g_st_pk_mode_high_th_table = PK模式门限基数 * 70% *1024 *1024 /8  (单位字节)
低档位门限: g_st_pk_mode_low_th_table  = PK模式门限基数 * 30% *1024 *1024 /8  (单位字节)

 */
uint32_t g_st_pk_mode_high_th_table[WLAN_PROTOCOL_CAP_BUTT][WLAN_BW_CAP_BUTT] = {
    { INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH },
    { 45,                110,               INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH },
    { 60,                130,               280,               562,               562 },
#ifdef _PRE_WLAN_FEATURE_11AX
    { 65,                140,               300,               525,               525 }
#endif /* #ifdef _PRE_WLAN_FEATURE_11AX */

};

uint32_t g_st_pk_mode_low_th_table[WLAN_PROTOCOL_CAP_BUTT][WLAN_BW_CAP_BUTT] = {
    { INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH },
    { 30,                80,                INVALID_PKMODE_TH, INVALID_PKMODE_TH, INVALID_PKMODE_TH },
    { 40,                100,               240,               400,               400 },
#ifdef _PRE_WLAN_FEATURE_11AX
    { 50,                110,               250,               450,               450 }
#endif /* #ifdef _PRE_WLAN_FEATURE_11AX */
};
#endif

uint32_t g_ul_orig_cpu_min_freq = 0;
uint32_t g_ul_orig_cpu_max_freq = 0;
uint32_t g_ul_orig_ddr_min_freq = 0;
uint32_t g_ul_orig_ddr_max_freq = 0;

#ifndef WIN32
/* 由定制化进行初始化 */
host_speed_freq_level_stru g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_BUTT] = {
    /* pps门限                   CPU主频下限                     DDR主频下限 */
    { PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { PPS_VALUE_3, CPU_MIN_FREQ_VALUE_3, DDR_MIN_FREQ_VALUE_3 },
};
host_speed_freq_level_stru g_host_no_ba_freq_level[4] = {
    /* pps门限                        CPU主频下限                      DDR主频下限 */
    { NO_BA_PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { NO_BA_PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { NO_BA_PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { NO_BA_PPS_VALUE_3, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
};
device_speed_freq_level_stru g_device_speed_freq_level[] = {
    /* device主频类型 */
    { FREQ_IDLE },
    { FREQ_MIDIUM },
    { FREQ_HIGHER },
    { FREQ_HIGHEST },
};

struct pm_qos_request *g_pst_wifi_auto_ddr = NULL;

#else
host_speed_freq_level_stru g_host_speed_freq_level[] = {
    /* pps门限                   CPU主频下限                     DDR主频下限 */
    { PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { PPS_VALUE_3, CPU_MIN_FREQ_VALUE_3, DDR_MIN_FREQ_VALUE_3 },
};
device_speed_freq_level_stru g_device_speed_freq_level[] = {
    /* device主频类型 */
    { FREQ_IDLE },
    { FREQ_MIDIUM },
    { FREQ_HIGHEST },
    { FREQ_HIGHEST },
};

#define mutex_init(mux)
#define mutex_lock(mux)
#define mutex_unlock(mux)
#define spin_lock_init(mux)
#define mutex_destroy(mux)
#define spin_unlock_bh(mux)
#endif

extern hmac_rxdata_thread_stru g_st_rxdata_thread;


uint8_t hmac_set_auto_freq_mod(uint8_t uc_freq_enable)
{
    g_freq_lock_control.uc_lock_mod = uc_freq_enable;

    /* 设置device是否使能 */
    if (uc_freq_enable == FREQ_LOCK_ENABLE) {
        /* 初始频率为idle */
        g_freq_lock_control.uc_curr_lock_level = FREQ_IDLE;
        /* 期望频率为idle */
        g_freq_lock_control.uc_req_lock_level = FREQ_IDLE;
        /* WIFI打开时,定时器参数更新 */
        g_freq_lock_control.ul_pre_jiffies = jiffies;
    } else {
        /* 关闭WIFI时,将CPU频率调整为默认值 */
        mutex_lock(&g_freq_lock_control.st_lock_freq_mtx);
        mutex_unlock(&g_freq_lock_control.st_lock_freq_mtx);

        g_freq_lock_control.uc_curr_lock_level = FREQ_BUTT;
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_set_auto_freq_mod: freq,min[%d]max[%d]!}",
                         g_ul_orig_cpu_min_freq, g_ul_orig_cpu_max_freq);
    }

    return OAL_SUCC;
}

void hmac_adjust_set_freq(void)
{
    uint8_t uc_req_lock_level = g_freq_lock_control.uc_req_lock_level;

    /* 相同则不需要配置系统文件 */
    if (uc_req_lock_level == g_freq_lock_control.uc_curr_lock_level) {
        return;
    }

    oam_warning_log2(0, OAM_SF_PWR, "{hmac_adjust_set_freq: freq [%d]to[%d]}",
                     g_freq_lock_control.uc_curr_lock_level, uc_req_lock_level);

    g_freq_lock_control.uc_curr_lock_level = uc_req_lock_level;
}

uint8_t hmac_get_freq_level(uint32_t ul_speed)  // 根据吞吐率计算目标主频等级
{
    uint8_t level_idx = 0;

    if (hmac_is_device_ba_setup()) {
        if (ul_speed <= g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_1].ul_speed_level) {
            level_idx = HMAC_FREQ_LEVEL_0;
        } else if ((ul_speed > g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_1].ul_speed_level) &&
                   (ul_speed <= g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_2].ul_speed_level)) {
            level_idx = HMAC_FREQ_LEVEL_1;
        } else if ((ul_speed > g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_2].ul_speed_level) &&
                   (ul_speed <= g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_3].ul_speed_level)) {
            level_idx = HMAC_FREQ_LEVEL_2;
        } else {
            level_idx = HMAC_FREQ_LEVEL_3;
        }
    } else {
#ifndef WIN32
        if (ul_speed <= g_host_no_ba_freq_level[HOST_SPEED_FREQ_LEVEL_1].ul_speed_level) {
            level_idx = HMAC_FREQ_LEVEL_0;
        } else if ((ul_speed > g_host_no_ba_freq_level[HOST_SPEED_FREQ_LEVEL_1].ul_speed_level) &&
                    (ul_speed <= g_host_no_ba_freq_level[HOST_SPEED_FREQ_LEVEL_2].ul_speed_level)) {
            level_idx = HMAC_FREQ_LEVEL_1;
        } else if ((ul_speed > g_host_no_ba_freq_level[HOST_SPEED_FREQ_LEVEL_2].ul_speed_level) &&
                    (ul_speed <= g_host_no_ba_freq_level[HOST_SPEED_FREQ_LEVEL_3].ul_speed_level)) {
            level_idx = HMAC_FREQ_LEVEL_2;
        } else {
            level_idx = HMAC_FREQ_LEVEL_3;
        }
#endif
    }
    return level_idx;
}


OAL_STATIC void hmac_pps_calc(void)
{
    uint32_t ul_cur_jiffies;
    uint32_t ul_sdio_dur_ms;  // 时间
    uint32_t ul_trx_total;
    uint32_t ul_tx_total;
    uint32_t ul_rx_total;

    ul_tx_total = oal_atomic_read(&g_st_wifi_rxtx_total.ul_tx_pkts);
    ul_rx_total = oal_atomic_read(&g_st_wifi_rxtx_total.ul_rx_pkts);
    ul_trx_total = ul_tx_total + ul_rx_total;
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_pkts, 0);
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_pkts, 0);

    ul_cur_jiffies = jiffies;  // jiffies是Linux内核中的一个全局变量，用来记录自系统启动以来产生的节拍的总数
    ul_sdio_dur_ms = oal_jiffies_to_msecs(ul_cur_jiffies - g_freq_lock_control.ul_pre_jiffies);
    g_freq_lock_control.ul_pre_jiffies = ul_cur_jiffies;

    /* 如果当前统计时间不足定时器周期的一半,会导致统计PPS值偏大返回 */
    if ((ul_sdio_dur_ms == 0) || (ul_sdio_dur_ms < (WLAN_FREQ_TIMER_PERIOD >> 1))) {
        return;
    }

    // 计算PPS
    g_freq_lock_control.ul_total_sdio_pps = (ul_trx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
    g_freq_lock_control.ul_tx_pps = (ul_tx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
    g_freq_lock_control.ul_rx_pps = (ul_rx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
}


void hmac_adjust_freq(void)
{
    uint8_t uc_req_lock_level;  // 目标主频等级

    hmac_pps_calc();

    /* host调频是否使能 */
    if (g_freq_lock_control.uc_lock_mod == FREQ_LOCK_DISABLE) {
        return;
    }

    g_freq_lock_control.uc_req_lock_level = hmac_get_freq_level(g_freq_lock_control.ul_total_sdio_pps);

    uc_req_lock_level = g_freq_lock_control.uc_req_lock_level;
    if (uc_req_lock_level == g_freq_lock_control.uc_curr_lock_level) { // 目标主频和当前主频相同，不需要调频
        g_freq_lock_control.ul_adjust_count = 0;
        return;
    }

    mutex_lock(&g_freq_lock_control.st_lock_freq_mtx);

    if (uc_req_lock_level < g_freq_lock_control.uc_curr_lock_level) {
        /* 连续MAX_DEGRADE_FREQ_TIME_THRESHOLD后才降频，保证性能 */
        g_freq_lock_control.ul_adjust_count++;

        if (g_freq_lock_control.ul_total_sdio_pps) { // 当前数据流量不为零
            // 有包时连续100个周期都需要降频才降频
            if (g_freq_lock_control.ul_adjust_count >= MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_10) {
                g_freq_lock_control.ul_adjust_count = 0;
                /* 在这里进行主频值的调整 */
                hmac_adjust_set_freq();
            }
        } else { // 没有包时连续3个周期都需要降频再降频
            if (g_freq_lock_control.ul_adjust_count >= MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_3) {
                g_freq_lock_control.ul_adjust_count = 0;
                hmac_adjust_set_freq();
            }
        }
    } else {
        /* 升频不等待，立即执行保证性能 */
        g_freq_lock_control.ul_adjust_count = 0;
        hmac_adjust_set_freq();
    }

    mutex_unlock(&g_freq_lock_control.st_lock_freq_mtx);
}


void hmac_wifi_auto_freq_ctrl_init(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_freq_lock_control.en_is_inited != OAL_TRUE) {
        mutex_init(&g_freq_lock_control.st_lock_freq_mtx);
        mutex_lock(&g_freq_lock_control.st_lock_freq_mtx);

        oam_warning_log4(0, OAM_SF_ANY,
            "{hmac_wifi_auto_freq_ctrl_init: orig_freq:: cpu_min[%d],cpu_max[%d],ddr_min[%d],ddr_max[%d]}",
            g_ul_orig_cpu_min_freq, g_ul_orig_cpu_max_freq, g_ul_orig_ddr_min_freq, g_ul_orig_ddr_max_freq);

        g_freq_lock_control.en_is_inited = OAL_TRUE;

        mutex_unlock(&g_freq_lock_control.st_lock_freq_mtx);
    }
#endif
}

void hmac_wifi_auto_freq_ctrl_deinit(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    mutex_lock(&g_freq_lock_control.st_lock_freq_mtx);

    if (g_freq_lock_control.en_is_inited == OAL_TRUE) {
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{hw_wifi_freq_ctrl_destroy freq lock has already been released!}");
    }

    mutex_unlock(&g_freq_lock_control.st_lock_freq_mtx);
    mutex_destroy(&g_freq_lock_control.st_lock_freq_mtx);

    g_freq_lock_control.en_is_inited = OAL_FALSE;
#endif
}

void hmac_auto_freq_wifi_rx_stat(uint32_t ul_pkt_count)
{
    oal_atomic_add(&g_st_wifi_rxtx_total.ul_rx_pkts, ul_pkt_count);
}

void hmac_auto_freq_wifi_tx_stat(uint32_t ul_pkt_count)
{
    oal_atomic_add(&g_st_wifi_rxtx_total.ul_tx_pkts, ul_pkt_count);
}

void hmac_auto_freq_wifi_rx_bytes_stat(uint32_t ul_pkt_bytes)
{
    oal_atomic_add(&g_st_wifi_rxtx_total.ul_rx_bytes, ul_pkt_bytes);
}

void hmac_auto_freq_wifi_tx_bytes_stat(uint32_t ul_pkt_bytes)
{
    oal_atomic_add(&g_st_wifi_rxtx_total.ul_tx_bytes, ul_pkt_bytes);
}

oal_bool_enum_uint8 hmac_wifi_rx_is_busy(void)
{
    return g_st_wifi_load.en_wifi_rx_busy;
}

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
OAL_STATIC OAL_INLINE void hmac_lock_max_cpu_freq(void)
{
    uint8_t uc_cpuid_loop;
    /* 所有核都锁定最高频率 */
    for (uc_cpuid_loop = 0; uc_cpuid_loop < OAL_BUS_MAXCPU_NUM; uc_cpuid_loop++) {
        /* 未获取到正确的cpu频率则不设置 */
        if (g_aul_cpumaxfreq[uc_cpuid_loop].valid != OAL_TRUE) {
            continue;
        }

        hisi_cpufreq_update_req(&g_ast_cpufreq[uc_cpuid_loop], g_aul_cpumaxfreq[uc_cpuid_loop].max_cpu_freq);
    }
}

OAL_STATIC OAL_INLINE void hmac_unlock_max_cpu_freq(void)
{
    uint8_t uc_cpuid_loop;
    for (uc_cpuid_loop = 0; uc_cpuid_loop < OAL_BUS_MAXCPU_NUM; uc_cpuid_loop++) {
        /* 未获取到正确的cpu频率则不设置 */
        if (g_aul_cpumaxfreq[uc_cpuid_loop].valid != OAL_TRUE) {
            continue;
        }

        hisi_cpufreq_update_req(&g_ast_cpufreq[uc_cpuid_loop], 0);
    }
}
#endif

OAL_STATIC OAL_INLINE void hmac_lock_cpu_freq_high_throughput_proc(void)
{
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_freq_lock_control.uc_lock_max_cpu_freq == OAL_TRUE) {
        /* 当前还存在锁频后频率会掉下来，并且后面也无法锁到最高频率，需要继续定位。后续需要调整锁频时间。
         * 以及确认是否每次需要重新req。 */
        core_ctl_set_boost(20 * WLAN_FREQ_TIMER_PERIOD * WLAN_THROUGHPUT_STA_PERIOD); // 20是锁频时间为单位时间的20倍
        hmac_lock_max_cpu_freq();
    }
#endif
#endif
}

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
OAL_STATIC void hmac_set_cpu_freq(uint8_t uc_req_freq_state)
{
    if (uc_req_freq_state == g_freq_lock_control.uc_cur_cpu_freq_state) {
        return;
    }

    g_freq_lock_control.uc_cur_cpu_freq_state = uc_req_freq_state;
}

OAL_STATIC void hmac_adjust_set_irq(uint8_t uc_cpu_id)
{
    if (uc_cpu_id == g_freq_lock_control.uc_cur_irq_cpu) {
        return;
    }

    g_freq_lock_control.uc_cur_irq_cpu = uc_cpu_id;

    return;
}
#else

OAL_STATIC void hmac_set_cpu_freq(uint8_t uc_req_freq_state)
{
    if (uc_req_freq_state == g_freq_lock_control.uc_cur_cpu_freq_state) {
        if (uc_req_freq_state == WLAN_CPU_FREQ_SUPER) {
            /* sdio锁频后频率会掉下来，需要每次都执行锁频 */
            hmac_lock_cpu_freq_high_throughput_proc();
        }
        return;
    }

    g_freq_lock_control.uc_cur_cpu_freq_state = uc_req_freq_state;

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_freq_lock_control.uc_lock_max_cpu_freq == OAL_TRUE) {
        if (uc_req_freq_state == WLAN_CPU_FREQ_SUPER) {
            /* 所有核都锁定最高频率 */
            hmac_lock_max_cpu_freq();
        } else {
            hmac_unlock_max_cpu_freq();
        }
    }

    if (g_freq_lock_control.uc_lock_dma_latency == OAL_TRUE) {
        if (uc_req_freq_state == WLAN_CPU_FREQ_SUPER) {
            /* 修改DMA latency,避免cpu进入过深的idle state */
            pm_qos_update_request(&g_st_pmqos_requset, g_freq_lock_control.dma_latency_value);
        } else {
            pm_qos_update_request(&g_st_pmqos_requset, PM_QOS_DEFAULT_VALUE);
        }
    }
#endif
#endif
}


OAL_STATIC void hmac_adjust_set_irq(uint8_t uc_cpu_id)
{
    if (uc_cpu_id == g_freq_lock_control.uc_cur_irq_cpu) {
        return;
    }

    g_freq_lock_control.uc_cur_irq_cpu = uc_cpu_id;

    /* 定制化关闭 */
    if (g_freq_lock_control.en_irq_affinity == OAL_FALSE) {
        return;
    }

    hi110x_hcc_dev_bindcpu(((uc_cpu_id >= WLAN_IRQ_AFFINITY_BUSY_CPU) ? 1 : 0),
        OAL_FALSE, 0, 0); /* 自动绑定时无需关心最后两位入参，填0即可 */

#if defined(CONFIG_ARCH_HISI)
#ifdef CONFIG_NR_CPUS
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    if (hmac_get_feature_switch(HMAC_CORE_BIND_SWITCH)) {
        if (g_st_rxdata_thread.pst_rxdata_thread) {
            struct cpumask fast_cpus;
            if (uc_cpu_id >= WLAN_IRQ_AFFINITY_BUSY_CPU) {
                hisi_get_fast_cpus(&fast_cpus);
                cpumask_clear_cpu(OAL_BUS_HPCPU_NUM, &fast_cpus);
                set_cpus_allowed_ptr(g_st_rxdata_thread.pst_rxdata_thread, &fast_cpus);
            } else {
                set_cpus_allowed_ptr(g_st_rxdata_thread.pst_rxdata_thread, cpumask_of(1));
            }
            g_st_rxdata_thread.uc_allowed_cpus = uc_cpu_id;
        }
    }
#endif
#endif
#endif
}
#endif

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
OAL_STATIC OAL_INLINE void hmac_update_pk_mode_curr_protocol_cap(mac_user_stru *pst_mac_user)
{
    switch (pst_mac_user->en_protocol_mode) {
        case WLAN_LEGACY_11A_MODE:
        case WLAN_LEGACY_11B_MODE:
        case WLAN_LEGACY_11G_MODE:
        case WLAN_MIXED_ONE_11G_MODE:
        case WLAN_MIXED_TWO_11G_MODE:
            g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_LEGACY;
            break;
        case WLAN_HT_MODE:
        case WLAN_HT_ONLY_MODE:
        case WLAN_HT_11G_MODE:
            g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_HT;
            break;
        case WLAN_VHT_MODE:
        case WLAN_VHT_ONLY_MODE:
            g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_VHT;
            break;
#ifdef _PRE_WLAN_FEATURE_11AX
        case WLAN_HE_MODE:
            if (g_wlan_spec_cfg->feature_11ax_is_open) {
                g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_HE;
                break;
            }
            /* fall through */ /* 03走下面的流程 */
#endif
        case WLAN_PROTOCOL_BUTT:
        default:
            g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_BUTT;
            break;
    }
}


void hmac_update_pk_mode_info(uint8_t *puc_vap_id)
{
    uint8_t uc_vap_id;
    mac_vap_stru *pst_mac_vap = NULL;
    mac_user_stru *pst_mac_user = NULL;
    oal_dlist_head_stru *pst_entry = NULL;
    oal_dlist_head_stru *pst_dlist_tmp = NULL;

    g_st_wifi_pk_mode_status.en_curr_bw_cap = WLAN_BW_CAP_BUTT;
    g_st_wifi_pk_mode_status.en_curr_protocol_cap = WLAN_PROTOCOL_CAP_BUTT;
    g_st_wifi_pk_mode_status.en_curr_num_spatial_stream = WLAN_SINGLE_NSS;

    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(uc_vap_id);
        if (pst_mac_vap == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_ANY, "{hmac_update_pk_mode_info::pst_mac_vap(%d) is null.}", uc_vap_id);
            continue;
        }

        if (!IS_LEGACY_STA(pst_mac_vap)) {
            continue;
        }

        oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head)) {
            pst_mac_user = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
            if (pst_mac_user == OAL_PTR_NULL) {
                oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_update_pk_mode_info::pst_user_tmp null.}");
                continue;
            }

            /* 从用户的信息中更新目前带宽和协议模式 */
            /* TBD 此设计目前仅适用支持一个LEGACY_STA且LEGACY_STA仅一个业务用户，若规格有修改此函数需要重构 */
            g_st_wifi_pk_mode_status.en_curr_bw_cap = pst_mac_user->en_bandwidth_cap;
            g_st_wifi_pk_mode_status.en_curr_num_spatial_stream = pst_mac_user->en_avail_num_spatial_stream;
            /* 嵌套深度优化封装 */
            hmac_update_pk_mode_curr_protocol_cap(pst_mac_user);

            *puc_vap_id = uc_vap_id;
        }
    }

    return;
}

OAL_STATIC oal_bool_enum_uint8 hmac_update_pk_mode_status_by_th(
    wlan_protocol_cap_enum_uint8 en_cur_protocol_cap, uint32_t ul_txrx_throughput)
{
    uint32_t ul_pk_mode_th;
    oal_bool_enum_uint8 en_curr_pk_mode;

    if (g_st_wifi_pk_mode_status.en_is_pk_mode == OAL_TRUE) {
        ul_pk_mode_th = (g_st_pk_mode_low_th_table[en_cur_protocol_cap][g_st_wifi_pk_mode_status.en_curr_bw_cap]) *
            (1 + g_st_wifi_pk_mode_status.en_curr_num_spatial_stream);
        en_curr_pk_mode = (ul_txrx_throughput < ul_pk_mode_th) ? OAL_FALSE : OAL_TRUE;
    } else {
        ul_pk_mode_th = (g_st_pk_mode_high_th_table[en_cur_protocol_cap][g_st_wifi_pk_mode_status.en_curr_bw_cap]) *
            (1 + g_st_wifi_pk_mode_status.en_curr_num_spatial_stream);
        en_curr_pk_mode = (ul_txrx_throughput > ul_pk_mode_th) ? OAL_TRUE : OAL_FALSE;
    }

    return en_curr_pk_mode;
}

OAL_STATIC oal_bool_enum_uint8 hmac_pk_mode_is_not_supp(mac_vap_stru *mac_vap)
{
    if ((mac_vap->en_protocol != WLAN_VHT_MODE) &&
        (mac_vap->en_protocol != WLAN_VHT_ONLY_MODE)) {
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
#ifdef _PRE_WLAN_FEATURE_11AX
            if (mac_vap->en_protocol != WLAN_HE_MODE) {
                return OAL_TRUE;
            }
#endif
        } else {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


OAL_STATIC oal_bool_enum_uint8 hmac_update_pk_mode_hisi(mac_vap_stru *mac_vap)
{
    hmac_user_stru *hmac_user = NULL;

    if (mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return OAL_FALSE;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_update_pk_mode_hisi::get_mac_user null!}");
        return OAL_FALSE;
    }

    return hmac_user->user_hisi;
}


void hmac_update_pk_mode(uint32_t ul_tx_throughput,
    uint32_t ul_rx_throughput,  uint32_t ul_dur_time)
{
    oal_bool_enum_uint8 en_curr_pk_mode;
    uint8_t uc_vap_id;
    mac_cfg_pk_mode_stru st_pk_mode_info;
    mac_vap_stru *pst_mac_vap = NULL;
    uint32_t ul_ret;
    uint32_t ul_txrx_throughput;
    mac_device_stru *pst_mac_device = NULL;
    wlan_protocol_cap_enum_uint8 en_cur_protocol_cap;

    if (g_en_pk_mode_swtich == OAL_FALSE) {
        return;
    }

    pst_mac_device = mac_res_get_dev(0);
    /* 如果非单VAP,则不开启硬件聚合 */
    if (mac_device_calc_up_vap_num(pst_mac_device) != 1) {
        return;
    }

    /* 更新相关的信息 */
    hmac_update_pk_mode_info(&uc_vap_id);

    /* 非有效模式，不进行吞吐计算 */
    if ((g_st_wifi_pk_mode_status.en_curr_bw_cap == WLAN_BW_CAP_BUTT) ||
        (g_st_wifi_pk_mode_status.en_curr_protocol_cap == WLAN_PROTOCOL_CAP_BUTT)) {
        return;
    }

    /* 计算吞吐率: rxtx吞吐之和 / 统计的时间间隔(单位s) */
    ul_txrx_throughput = ul_tx_throughput + ul_rx_throughput;

    en_cur_protocol_cap = g_st_wifi_pk_mode_status.en_curr_protocol_cap;

    /* 依据门限值判定pk mode状态 */
    en_curr_pk_mode = hmac_update_pk_mode_status_by_th(en_cur_protocol_cap, ul_txrx_throughput);
    /* PK mode状态未变化，不更新数值 */
    if (en_curr_pk_mode == g_st_wifi_pk_mode_status.en_is_pk_mode) {
        return;
    }

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_update_pk_mode::pst_mac_vap(%d) is null.}", uc_vap_id);
        return;
    }
    /* 避免ddut Hi1103/1105时调整edca参数 */
    if (hmac_pk_mode_is_not_supp(pst_mac_vap) || (hmac_update_pk_mode_hisi(pst_mac_vap) == OAL_TRUE)) {
        return;
    }
    oam_warning_log4(0, OAM_SF_ANY,
                     "{hmac_update_pk_mode: pk_mode changed (%d) -> (%d),tx_throughput[%d],rx_throughput[%d]!}",
                     g_st_wifi_pk_mode_status.en_is_pk_mode, en_curr_pk_mode, ul_tx_throughput, ul_rx_throughput);
    g_st_wifi_pk_mode_status.en_is_pk_mode = en_curr_pk_mode;
    g_st_wifi_pk_mode_status.ul_rx_bytes = ul_rx_throughput;
    g_st_wifi_pk_mode_status.ul_tx_bytes = ul_tx_throughput;
    g_st_wifi_pk_mode_status.ul_dur_time = ul_dur_time;

    /***************************************************************************
        抛事件到DMAC层, 同步VAP最新状态到DMAC
    ***************************************************************************/
    st_pk_mode_info.ul_rx_bytes = g_st_wifi_pk_mode_status.ul_rx_bytes;
    st_pk_mode_info.ul_tx_bytes = g_st_wifi_pk_mode_status.ul_tx_bytes;
    st_pk_mode_info.ul_dur_time = g_st_wifi_pk_mode_status.ul_dur_time;
    st_pk_mode_info.en_is_pk_mode = g_st_wifi_pk_mode_status.en_is_pk_mode;
    st_pk_mode_info.en_curr_bw_cap = g_st_wifi_pk_mode_status.en_curr_bw_cap;
    st_pk_mode_info.en_curr_protocol_cap = g_st_wifi_pk_mode_status.en_curr_protocol_cap;
    st_pk_mode_info.en_curr_num_spatial_stream = g_st_wifi_pk_mode_status.en_curr_num_spatial_stream;

    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SYNC_PK_MODE,
                                    OAL_SIZEOF(mac_cfg_pk_mode_stru), (uint8_t *)(&st_pk_mode_info));
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_update_pk_mode::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return;
}
#endif


OAL_STATIC void hmac_calc_throughput_cpu_freq(
    uint32_t ul_tx_throughput_mbps, uint32_t ul_rx_throughput_mbps)
{
    uint16_t us_cpu_freq_high = WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_HIGH;
    uint16_t us_cpu_freq_low = WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_LOW;

    /* 每秒吞吐量门限 */
    if ((g_freq_lock_control.us_lock_cpu_th_high) && (g_freq_lock_control.us_lock_cpu_th_low)) {
        us_cpu_freq_high = g_freq_lock_control.us_lock_cpu_th_high;
        us_cpu_freq_low = g_freq_lock_control.us_lock_cpu_th_low;
    }

    /* 高于800M时切超频，低于500M恢复 */
    if ((ul_tx_throughput_mbps > us_cpu_freq_high) ||
        (ul_rx_throughput_mbps > us_cpu_freq_high)) {
        g_freq_lock_control.uc_req_super_cpu_freq = WLAN_CPU_FREQ_SUPER;
    } else if ((ul_tx_throughput_mbps < us_cpu_freq_low) &&
               (ul_rx_throughput_mbps < us_cpu_freq_low)) {
        g_freq_lock_control.uc_req_super_cpu_freq = WLAN_CPU_FREQ_IDLE;
    }

    hmac_set_cpu_freq(g_freq_lock_control.uc_req_super_cpu_freq);
}


OAL_STATIC uint8_t hmac_bindcpu_get_cpu_id(uint8_t cmd)
{
    uint8_t cpu_id;

    for (cpu_id = 0; cmd; cpu_id++) {
        cmd >>= 1; /* 右移1位求以2为底的对数 */
    }

    return (cpu_id - 1); /* CPU编号以0开始而不是1 */
}


OAL_STATIC void hmac_userctl_set_irq(uint8_t req_bind_cpu,
                                     uint8_t irq_cmd,
                                     uint8_t thread_cmd)
{
    uint8_t irq_cpu;
    uint8_t thread_cpu;
    uint8_t big_core = (uint8_t)(0xFF << WLAN_IRQ_AFFINITY_BUSY_CPU); /* 大核所在CPU范围 */
#if defined(CONFIG_ARCH_HISI) &&  defined(CONFIG_NR_CPUS)
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    struct cpumask thread_cpu_mask;
#endif
#endif

    if (irq_cmd != 0 && thread_cmd != 0) { /* 硬中断和线程指令均不为0 */
        /* 用户指定了硬中断所在核,以及线程所在核 */
        irq_cpu = hmac_bindcpu_get_cpu_id(irq_cmd);
        thread_cpu = thread_cmd;
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_irq_cpu, set_thread_cpu}");
    } else if (irq_cmd != 0 && thread_cmd == 0) {  /* 只有线程指令为0 */
        /* 用户指定了硬中断所在核,线程由吞吐量与pps门限决定 */
        irq_cpu = hmac_bindcpu_get_cpu_id(irq_cmd);
        thread_cpu = (req_bind_cpu >= WLAN_IRQ_AFFINITY_BUSY_CPU) ? (big_core) : (~big_core);
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_irq_cpu}");
    } else if (irq_cmd == 0 && thread_cmd != 0){ /* 只有硬中断指令为0 */
        /* 用户指定了线程所在核,中断由吞吐量与pps门限决定 */
        irq_cpu = (req_bind_cpu >= WLAN_IRQ_AFFINITY_BUSY_CPU) ?
                  WLAN_IRQ_AFFINITY_BUSY_CPU : 0; /* 大核模式irq默认绑定在CPU4核,小核模式irq默认绑定在0核 */
        thread_cpu = thread_cmd;
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_thread_cpu}");
    } else {
        g_freq_lock_control.en_userctl_bindcpu = OAL_BUTT;
        return;
    }

    if (!IRQ_CMD_IS_ONECPU(thread_cpu) && (hmac_bindcpu_get_cpu_id(thread_cpu) == irq_cpu)) {
        /* 线程绑定在一个CPU核上,且这个核与硬中断绑定核相同,这种命令应该避免 */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_userctl_set_irq::bind same CPU[%d]}", irq_cpu);
        thread_cpu = ~big_core; /* 此时默认将线程绑定在小核上 */
    }

    oam_warning_log2(0, OAM_SF_ANY, "{hmac_userctl_set_irq::irq_cpu[%d], thread_cpu[0x%x]}",
        irq_cpu, thread_cpu);

    /* 中断定制化开关关闭 */
    if (g_freq_lock_control.en_irq_affinity == OAL_FALSE) {
        return;
    }

    hi110x_hcc_dev_bindcpu(0, g_freq_lock_control.en_userctl_bindcpu, irq_cpu, thread_cpu);

    /* 绑定pst_rxdata线程 */
#if defined(CONFIG_ARCH_HISI) &&  defined(CONFIG_NR_CPUS)
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    thread_cpu_mask = *((struct cpumask *)&thread_cpu);

    if (hmac_get_feature_switch(HMAC_CORE_BIND_SWITCH)) {
        if (g_st_rxdata_thread.pst_rxdata_thread) {
            set_cpus_allowed_ptr(g_st_rxdata_thread.pst_rxdata_thread, &thread_cpu_mask);
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_userctl_set_irq::hisi_rxdata bind CPU[0x%x]}", thread_cpu);
        }
    }
#endif
#endif
}



void hmac_perform_calc_throughput(uint32_t ul_tx_throughput_mbps,
                                  uint32_t ul_rx_throughput_mbps,
                                  uint32_t ul_trx_pps)
{
    uint32_t ul_limit_throughput_high = WLAN_IRQ_THROUGHPUT_THRESHOLD_HIGH;
    uint32_t ul_limit_throughput_low = WLAN_IRQ_THROUGHPUT_THRESHOLD_LOW;
    uint32_t ul_limit_pps_high = WLAN_IRQ_PPS_THRESHOLD_HIGH;
    uint32_t ul_limit_pps_low = WLAN_IRQ_PPS_THRESHOLD_LOW;

    /* 每秒吞吐量门限 */
    if ((g_freq_lock_control.us_throughput_irq_high) && (g_freq_lock_control.us_throughput_irq_low)) {
        ul_limit_throughput_high = g_freq_lock_control.us_throughput_irq_high;
        ul_limit_throughput_low = g_freq_lock_control.us_throughput_irq_low;
    }

    /* 每秒PPS门限 */
    if ((g_freq_lock_control.ul_irq_pps_high) && (g_freq_lock_control.ul_irq_pps_low)) {
        ul_limit_pps_high = g_freq_lock_control.ul_irq_pps_high;
        ul_limit_pps_low = g_freq_lock_control.ul_irq_pps_low;
    }

    /* 高于200M时切大核，低于150M切小核 */
    if ((ul_tx_throughput_mbps > ul_limit_throughput_high) ||
        (ul_rx_throughput_mbps > ul_limit_throughput_high) ||
        (ul_trx_pps >= ul_limit_pps_high)) {
        g_freq_lock_control.uc_req_irq_cpu = WLAN_IRQ_AFFINITY_BUSY_CPU;
    } else if ((ul_tx_throughput_mbps < ul_limit_throughput_low) &&
               (ul_rx_throughput_mbps < ul_limit_throughput_low) &&
               (ul_trx_pps < ul_limit_pps_low)) {
        g_freq_lock_control.uc_req_irq_cpu = WLAN_IRQ_AFFINITY_IDLE_CPU;
    }

    if (g_freq_lock_control.en_userctl_bindcpu == OAL_TRUE) {
        hmac_userctl_set_irq(g_freq_lock_control.uc_req_irq_cpu,
                             g_freq_lock_control.uc_userctl_irqbind,
                             g_freq_lock_control.uc_userctl_threadbind);
        g_freq_lock_control.en_userctl_bindcpu = OAL_BUTT;
    } else if (g_freq_lock_control.en_userctl_bindcpu == OAL_FALSE){
        if (g_freq_lock_control.uc_req_irq_cpu != g_freq_lock_control.uc_cur_irq_cpu) {
            oam_warning_log4(0, OAM_SF_ANY,
                "{hmac_perform_calc_throughput:throughput rx = %d , tx = %d, high_th = %d,low_th = %d!}",
                ul_rx_throughput_mbps, ul_tx_throughput_mbps, ul_limit_throughput_high, ul_limit_throughput_low);
            oam_warning_log3(0, OAM_SF_ANY, "{hmac_perform_calc_throughput: rx pps = %d , tx pps = %d, trx pps = %d,!}",
                g_freq_lock_control.ul_rx_pps, g_freq_lock_control.ul_tx_pps, ul_trx_pps);
            oam_warning_log4(0, OAM_SF_ANY,
                "{hmac_perform_calc_throughput: req cpu id[%d],cur cpu id[%d],pps_high = %d, pps low = %d!}",
                g_freq_lock_control.uc_req_irq_cpu, g_freq_lock_control.uc_cur_irq_cpu,
                ul_limit_pps_high, ul_limit_pps_low);
        }
        hmac_adjust_set_irq(g_freq_lock_control.uc_req_irq_cpu);
    }

    hmac_calc_throughput_cpu_freq(ul_tx_throughput_mbps, ul_rx_throughput_mbps);
}


void hmac_adjust_throughput(void)
{
    uint32_t ul_tx_throughput;
    uint32_t ul_rx_throughput;
    uint32_t ul_dur_time;
    uint32_t ul_tx_throughput_mbps = 0;
    uint32_t ul_rx_throughput_mbps = 0;

    /* 循环次数统计 */
    g_freq_lock_control.uc_timer_cycles++;

    ul_dur_time = oal_jiffies_to_msecs(jiffies - g_freq_lock_control.ul_pre_time);
    /* 小于一个周期,不统计 */
    if (ul_dur_time < WLAN_FREQ_TIMER_PERIOD - 10) { // 10是当前定时器周期需要减去10ms为一个周期
        return;
    }

    /* 超过一定时常则不统计,排除定时器异常 */
    if (ul_dur_time > ((WLAN_FREQ_TIMER_PERIOD * WLAN_THROUGHPUT_STA_PERIOD) << BIT_OFFSET_2)) {
        g_freq_lock_control.ul_pre_time = jiffies;
        g_freq_lock_control.uc_timer_cycles = 0;

        oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_bytes, 0);
        oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_bytes, 0);

        return;
    }

    /* 总字节数 */
    ul_rx_throughput = oal_atomic_read(&g_st_wifi_rxtx_total.ul_rx_bytes);
    ul_tx_throughput = oal_atomic_read(&g_st_wifi_rxtx_total.ul_tx_bytes);
    if (ul_dur_time != 0) {
        ul_tx_throughput_mbps = (ul_tx_throughput >> 7) / ul_dur_time;
        ul_rx_throughput_mbps = (ul_rx_throughput >> 7) / ul_dur_time;
    }

    /* Wi-Fi业务负载标记 */
    if (ul_rx_throughput_mbps <= WLAN_THROUGHPUT_LOAD_LOW) {
        g_st_wifi_load.en_wifi_rx_busy = OAL_FALSE;
    } else {
        g_st_wifi_load.en_wifi_rx_busy = OAL_TRUE;
    }
    if (g_st_wifi_rxtx_total.uc_trx_stat_log_en == OAL_TRUE) {
        oam_warning_log4(0, OAM_SF_ANY,
                         "{hmac_tx_tcp_ack_buf_switch: Throught:: rx[%d],tx[%d],smooth[%d],dur_time[%d]ms!}",
                         ul_rx_throughput_mbps, ul_tx_throughput_mbps,
                         g_st_tcp_ack_buf_switch.us_tcp_ack_smooth_throughput, ul_dur_time);
    }

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    /* AMSDU+AMPDU聚合切换入口 */
    hmac_tx_amsdu_ampdu_switch(ul_tx_throughput_mbps);
#endif
    hmac_tx_small_amsdu_switch(ul_rx_throughput_mbps, g_freq_lock_control.ul_tx_pps);

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    hmac_tx_tcp_ack_buf_switch(ul_rx_throughput_mbps);
#endif

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    /* 聚合软硬件切换入口,每个周期探测一次 */
    hmac_tx_ampdu_switch(ul_tx_throughput_mbps);
#endif
#ifdef _PRE_WLAN_TCP_OPT
    hmac_tcp_ack_filter_switch(ul_rx_throughput_mbps);
#endif
    /* 根据吞吐量，判断是否需要bypass 外置LNA */
    hmac_rx_dyn_bypass_extlna_switch(ul_tx_throughput_mbps, ul_rx_throughput_mbps);

    /* 中断动态调整 */
    hmac_perform_calc_throughput(ul_tx_throughput_mbps, ul_rx_throughput_mbps,
                                 g_freq_lock_control.ul_total_sdio_pps);
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
    /* 更新pk mode */
    hmac_update_pk_mode(ul_tx_throughput_mbps, ul_rx_throughput_mbps, ul_dur_time);
#endif

    /* irq 2s 探测一次 */
    if (g_freq_lock_control.uc_timer_cycles < WLAN_THROUGHPUT_STA_PERIOD) {
        return;
    }
    g_freq_lock_control.uc_timer_cycles = 0;

    /* 2s周期清零一次 */
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_bytes, 0);
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_bytes, 0);

    g_freq_lock_control.ul_pre_time = jiffies;
}


OAL_STATIC uint32_t hmac_wlan_freq_wdg_timeout(void *p_arg)
{
    /* CPU调频 */
    hmac_adjust_freq();

    /* 吞吐统计 */
    hmac_adjust_throughput();
    return OAL_SUCC;
}


void hmac_freq_timer_init(void)
{
    /* 如果这个定时器已经注册成功，则不能再次被注册，不然后发生错误！ */
    if (g_freq_lock_control.hmac_freq_timer.en_is_registerd == OAL_TRUE) {
        return;
    }

    g_freq_lock_control.ul_pre_time = jiffies;

    /* uc_timer_cycles 无需清零。避免停止跑流进入低功耗后，大小核切换、PK mode判断无法恢复问题 */
    /* 清空统计 */
    memset_s(&g_st_wifi_rxtx_total, OAL_SIZEOF(g_st_wifi_rxtx_total), 0, OAL_SIZEOF(g_st_wifi_rxtx_total));

    frw_timer_create_timer_m(&g_freq_lock_control.hmac_freq_timer,  // pst_timeout
                             hmac_wlan_freq_wdg_timeout,                                   // p_timeout_func
                             WLAN_FREQ_TIMER_PERIOD,                                       // ul_timeout
                             OAL_PTR_NULL,                                                 // p_timeout_arg
                             OAL_TRUE,                                                     // en_is_periodic
                             OAM_MODULE_ID_HMAC, 0);                                       // en_module_id && ul_core_id
}


void hmac_freq_timer_deinit(void)
{
    /* 定时器没有注册，就不用删除了，不然会发生错误！ */
    if (g_freq_lock_control.hmac_freq_timer.en_is_registerd == OAL_FALSE) {
        return;
    }

    hmac_adjust_set_irq(WLAN_IRQ_AFFINITY_IDLE_CPU);
    hmac_set_cpu_freq(WLAN_CPU_FREQ_IDLE);
    frw_timer_immediate_destroy_timer_m(&g_freq_lock_control.hmac_freq_timer);
}

void hmac_wifi_pm_state_notify(oal_bool_enum_uint8 en_wake_up)
{
    if (en_wake_up == OAL_TRUE) {
        /* WIFI唤醒,启动吞吐量统计定时器 */
        hmac_freq_timer_init();
    } else {
        /* WIFI睡眠,关闭吞吐量统计定时器 */
        hmac_freq_timer_deinit();
    }
}

void hmac_userctl_bindcpu_get_cmd(uint32_t cmd)
{
    uint8_t irq_cmd;
    uint8_t thread_cmd;

    irq_cmd = (uint8_t)((uint16_t)cmd >> 8);  /* 右移8位获取高位硬中断命令 */
    thread_cmd = (uint8_t)((uint16_t)cmd & 0xff); /* 与0xff获取低位线程命令 */

    if (IRQ_CMD_IS_ONECPU(irq_cmd)) { /* 硬中断只能绑定在一个CPU上 */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_config_userctl_set_bindcpu::error cmd is [0x%x]}", cmd);
        return;
    }

    if (!irq_cmd && !thread_cmd) {
        /* 由默认方式,即依据吞吐量与PPS门限自动指定CPU绑核 */
        /* 进行一次大小核切换,将当前状态重置为小核模式 */
        hmac_adjust_set_irq(WLAN_IRQ_AFFINITY_BUSY_CPU);
        hmac_adjust_set_irq(WLAN_IRQ_AFFINITY_IDLE_CPU);
        g_freq_lock_control.en_userctl_bindcpu = OAL_FALSE;
    } else {
        /* 由用户指定CPU绑核 */
        g_freq_lock_control.uc_userctl_irqbind = irq_cmd;
        g_freq_lock_control.uc_userctl_threadbind = thread_cmd;
        g_freq_lock_control.en_userctl_bindcpu = OAL_TRUE;
    }
}


void hmac_wifi_state_notify(oal_bool_enum_uint8 en_wifi_on)
{
    if (en_wifi_on == OAL_TRUE) {
#ifdef CONFIG_ARCH_KIRIN_PCIE
        if (g_tx_switch.tx_switch == HOST_TX) {
            /* 初始化每个hal dev的ba info ring内存信息.TBD:DBDC适配辅路 */
            hal_host_tx_intr_init(0);
        }
#endif
        /* WIFI上电,启动吞吐量统计定时器,打开调频功能 */
        hmac_freq_timer_init();
    } else {
        /* WIFI下电,关闭吞吐量统计定时器,关闭调频功能 */
        hmac_freq_timer_deinit();
#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
        /* WIFI下电, 停止正在进行的wifi业务 */
        hmac_config_change_wifi_state_for_power_down();
#endif
    }
}
