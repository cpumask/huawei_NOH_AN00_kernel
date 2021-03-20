

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
  2 ȫ�ֱ�������
*****************************************************************************/
#ifdef WIN32
uint32_t jiffies;
#endif

freq_lock_control_stru g_freq_lock_control = { 0 };
wifi_txrx_pkt_stat g_st_wifi_rxtx_total = { 0 };
/* Wi-Fi�����շ�����ʶ�������� */
freq_wifi_load_stru g_st_wifi_load = { 0 };
#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
extern struct cpufreq_req g_ast_cpufreq[OAL_BUS_MAXCPU_NUM];
extern hisi_max_cpu_freq g_aul_cpumaxfreq[OAL_BUS_MAXCPU_NUM];
extern struct pm_qos_request g_st_pmqos_requset;
#endif
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
oal_bool_enum_uint8 g_en_pk_mode_swtich = OAL_TRUE;

/* pk mode���ж������ޣ����ֽ�Ϊ��λ */
/*
PKģʽ���޻���:
{(��λMbps)  20M     40M    80M   160M   80+80M
lagency:    {valid, valid, valid, valid, valid},   (����Э��ģʽû��pk mode )
HT:         {62, 123, valid, valid, valid},
VHT:        {77, 167, 329, 660, 660},
HE:         {110, 205, 460, 750, 750},
};

PKģʽ��������:
�ߵ�λ����: g_st_pk_mode_high_th_table = PKģʽ���޻��� * 70% *1024 *1024 /8  (��λ�ֽ�)
�͵�λ����: g_st_pk_mode_low_th_table  = PKģʽ���޻��� * 30% *1024 *1024 /8  (��λ�ֽ�)

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
/* �ɶ��ƻ����г�ʼ�� */
host_speed_freq_level_stru g_host_speed_freq_level[HOST_SPEED_FREQ_LEVEL_BUTT] = {
    /* pps����                   CPU��Ƶ����                     DDR��Ƶ���� */
    { PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { PPS_VALUE_3, CPU_MIN_FREQ_VALUE_3, DDR_MIN_FREQ_VALUE_3 },
};
host_speed_freq_level_stru g_host_no_ba_freq_level[4] = {
    /* pps����                        CPU��Ƶ����                      DDR��Ƶ���� */
    { NO_BA_PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { NO_BA_PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { NO_BA_PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { NO_BA_PPS_VALUE_3, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
};
device_speed_freq_level_stru g_device_speed_freq_level[] = {
    /* device��Ƶ���� */
    { FREQ_IDLE },
    { FREQ_MIDIUM },
    { FREQ_HIGHER },
    { FREQ_HIGHEST },
};

struct pm_qos_request *g_pst_wifi_auto_ddr = NULL;

#else
host_speed_freq_level_stru g_host_speed_freq_level[] = {
    /* pps����                   CPU��Ƶ����                     DDR��Ƶ���� */
    { PPS_VALUE_0, CPU_MIN_FREQ_VALUE_0, DDR_MIN_FREQ_VALUE_0 },
    { PPS_VALUE_1, CPU_MIN_FREQ_VALUE_1, DDR_MIN_FREQ_VALUE_1 },
    { PPS_VALUE_2, CPU_MIN_FREQ_VALUE_2, DDR_MIN_FREQ_VALUE_2 },
    { PPS_VALUE_3, CPU_MIN_FREQ_VALUE_3, DDR_MIN_FREQ_VALUE_3 },
};
device_speed_freq_level_stru g_device_speed_freq_level[] = {
    /* device��Ƶ���� */
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

    /* ����device�Ƿ�ʹ�� */
    if (uc_freq_enable == FREQ_LOCK_ENABLE) {
        /* ��ʼƵ��Ϊidle */
        g_freq_lock_control.uc_curr_lock_level = FREQ_IDLE;
        /* ����Ƶ��Ϊidle */
        g_freq_lock_control.uc_req_lock_level = FREQ_IDLE;
        /* WIFI��ʱ,��ʱ���������� */
        g_freq_lock_control.ul_pre_jiffies = jiffies;
    } else {
        /* �ر�WIFIʱ,��CPUƵ�ʵ���ΪĬ��ֵ */
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

    /* ��ͬ����Ҫ����ϵͳ�ļ� */
    if (uc_req_lock_level == g_freq_lock_control.uc_curr_lock_level) {
        return;
    }

    oam_warning_log2(0, OAM_SF_PWR, "{hmac_adjust_set_freq: freq [%d]to[%d]}",
                     g_freq_lock_control.uc_curr_lock_level, uc_req_lock_level);

    g_freq_lock_control.uc_curr_lock_level = uc_req_lock_level;
}

uint8_t hmac_get_freq_level(uint32_t ul_speed)  // ���������ʼ���Ŀ����Ƶ�ȼ�
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
    uint32_t ul_sdio_dur_ms;  // ʱ��
    uint32_t ul_trx_total;
    uint32_t ul_tx_total;
    uint32_t ul_rx_total;

    ul_tx_total = oal_atomic_read(&g_st_wifi_rxtx_total.ul_tx_pkts);
    ul_rx_total = oal_atomic_read(&g_st_wifi_rxtx_total.ul_rx_pkts);
    ul_trx_total = ul_tx_total + ul_rx_total;
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_pkts, 0);
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_pkts, 0);

    ul_cur_jiffies = jiffies;  // jiffies��Linux�ں��е�һ��ȫ�ֱ�����������¼��ϵͳ�������������Ľ��ĵ�����
    ul_sdio_dur_ms = oal_jiffies_to_msecs(ul_cur_jiffies - g_freq_lock_control.ul_pre_jiffies);
    g_freq_lock_control.ul_pre_jiffies = ul_cur_jiffies;

    /* �����ǰͳ��ʱ�䲻�㶨ʱ�����ڵ�һ��,�ᵼ��ͳ��PPSֵƫ�󷵻� */
    if ((ul_sdio_dur_ms == 0) || (ul_sdio_dur_ms < (WLAN_FREQ_TIMER_PERIOD >> 1))) {
        return;
    }

    // ����PPS
    g_freq_lock_control.ul_total_sdio_pps = (ul_trx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
    g_freq_lock_control.ul_tx_pps = (ul_tx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
    g_freq_lock_control.ul_rx_pps = (ul_rx_total * HMAC_FREQ_S_TRANS_TO_MS) / ul_sdio_dur_ms;
}


void hmac_adjust_freq(void)
{
    uint8_t uc_req_lock_level;  // Ŀ����Ƶ�ȼ�

    hmac_pps_calc();

    /* host��Ƶ�Ƿ�ʹ�� */
    if (g_freq_lock_control.uc_lock_mod == FREQ_LOCK_DISABLE) {
        return;
    }

    g_freq_lock_control.uc_req_lock_level = hmac_get_freq_level(g_freq_lock_control.ul_total_sdio_pps);

    uc_req_lock_level = g_freq_lock_control.uc_req_lock_level;
    if (uc_req_lock_level == g_freq_lock_control.uc_curr_lock_level) { // Ŀ����Ƶ�͵�ǰ��Ƶ��ͬ������Ҫ��Ƶ
        g_freq_lock_control.ul_adjust_count = 0;
        return;
    }

    mutex_lock(&g_freq_lock_control.st_lock_freq_mtx);

    if (uc_req_lock_level < g_freq_lock_control.uc_curr_lock_level) {
        /* ����MAX_DEGRADE_FREQ_TIME_THRESHOLD��Ž�Ƶ����֤���� */
        g_freq_lock_control.ul_adjust_count++;

        if (g_freq_lock_control.ul_total_sdio_pps) { // ��ǰ����������Ϊ��
            // �а�ʱ����100�����ڶ���Ҫ��Ƶ�Ž�Ƶ
            if (g_freq_lock_control.ul_adjust_count >= MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_10) {
                g_freq_lock_control.ul_adjust_count = 0;
                /* �����������Ƶֵ�ĵ��� */
                hmac_adjust_set_freq();
            }
        } else { // û�а�ʱ����3�����ڶ���Ҫ��Ƶ�ٽ�Ƶ
            if (g_freq_lock_control.ul_adjust_count >= MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_3) {
                g_freq_lock_control.ul_adjust_count = 0;
                hmac_adjust_set_freq();
            }
        }
    } else {
        /* ��Ƶ���ȴ�������ִ�б�֤���� */
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
    /* ���к˶��������Ƶ�� */
    for (uc_cpuid_loop = 0; uc_cpuid_loop < OAL_BUS_MAXCPU_NUM; uc_cpuid_loop++) {
        /* δ��ȡ����ȷ��cpuƵ�������� */
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
        /* δ��ȡ����ȷ��cpuƵ�������� */
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
        /* ��ǰ��������Ƶ��Ƶ�ʻ�����������Һ���Ҳ�޷��������Ƶ�ʣ���Ҫ������λ��������Ҫ������Ƶʱ�䡣
         * �Լ�ȷ���Ƿ�ÿ����Ҫ����req�� */
        core_ctl_set_boost(20 * WLAN_FREQ_TIMER_PERIOD * WLAN_THROUGHPUT_STA_PERIOD); // 20����Ƶʱ��Ϊ��λʱ���20��
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
            /* sdio��Ƶ��Ƶ�ʻ����������Ҫÿ�ζ�ִ����Ƶ */
            hmac_lock_cpu_freq_high_throughput_proc();
        }
        return;
    }

    g_freq_lock_control.uc_cur_cpu_freq_state = uc_req_freq_state;

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_freq_lock_control.uc_lock_max_cpu_freq == OAL_TRUE) {
        if (uc_req_freq_state == WLAN_CPU_FREQ_SUPER) {
            /* ���к˶��������Ƶ�� */
            hmac_lock_max_cpu_freq();
        } else {
            hmac_unlock_max_cpu_freq();
        }
    }

    if (g_freq_lock_control.uc_lock_dma_latency == OAL_TRUE) {
        if (uc_req_freq_state == WLAN_CPU_FREQ_SUPER) {
            /* �޸�DMA latency,����cpu��������idle state */
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

    /* ���ƻ��ر� */
    if (g_freq_lock_control.en_irq_affinity == OAL_FALSE) {
        return;
    }

    hi110x_hcc_dev_bindcpu(((uc_cpu_id >= WLAN_IRQ_AFFINITY_BUSY_CPU) ? 1 : 0),
        OAL_FALSE, 0, 0); /* �Զ���ʱ������������λ��Σ���0���� */

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
            /* fall through */ /* 03����������� */
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

            /* ���û�����Ϣ�и���Ŀǰ�����Э��ģʽ */
            /* TBD �����Ŀǰ������֧��һ��LEGACY_STA��LEGACY_STA��һ��ҵ���û�����������޸Ĵ˺�����Ҫ�ع� */
            g_st_wifi_pk_mode_status.en_curr_bw_cap = pst_mac_user->en_bandwidth_cap;
            g_st_wifi_pk_mode_status.en_curr_num_spatial_stream = pst_mac_user->en_avail_num_spatial_stream;
            /* Ƕ������Ż���װ */
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
    /* ����ǵ�VAP,�򲻿���Ӳ���ۺ� */
    if (mac_device_calc_up_vap_num(pst_mac_device) != 1) {
        return;
    }

    /* ������ص���Ϣ */
    hmac_update_pk_mode_info(&uc_vap_id);

    /* ����Чģʽ�����������¼��� */
    if ((g_st_wifi_pk_mode_status.en_curr_bw_cap == WLAN_BW_CAP_BUTT) ||
        (g_st_wifi_pk_mode_status.en_curr_protocol_cap == WLAN_PROTOCOL_CAP_BUTT)) {
        return;
    }

    /* ����������: rxtx����֮�� / ͳ�Ƶ�ʱ����(��λs) */
    ul_txrx_throughput = ul_tx_throughput + ul_rx_throughput;

    en_cur_protocol_cap = g_st_wifi_pk_mode_status.en_curr_protocol_cap;

    /* ��������ֵ�ж�pk mode״̬ */
    en_curr_pk_mode = hmac_update_pk_mode_status_by_th(en_cur_protocol_cap, ul_txrx_throughput);
    /* PK mode״̬δ�仯����������ֵ */
    if (en_curr_pk_mode == g_st_wifi_pk_mode_status.en_is_pk_mode) {
        return;
    }

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_update_pk_mode::pst_mac_vap(%d) is null.}", uc_vap_id);
        return;
    }
    /* ����ddut Hi1103/1105ʱ����edca���� */
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
        ���¼���DMAC��, ͬ��VAP����״̬��DMAC
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

    /* ÿ������������ */
    if ((g_freq_lock_control.us_lock_cpu_th_high) && (g_freq_lock_control.us_lock_cpu_th_low)) {
        us_cpu_freq_high = g_freq_lock_control.us_lock_cpu_th_high;
        us_cpu_freq_low = g_freq_lock_control.us_lock_cpu_th_low;
    }

    /* ����800Mʱ�г�Ƶ������500M�ָ� */
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
        cmd >>= 1; /* ����1λ����2Ϊ�׵Ķ��� */
    }

    return (cpu_id - 1); /* CPU�����0��ʼ������1 */
}


OAL_STATIC void hmac_userctl_set_irq(uint8_t req_bind_cpu,
                                     uint8_t irq_cmd,
                                     uint8_t thread_cmd)
{
    uint8_t irq_cpu;
    uint8_t thread_cpu;
    uint8_t big_core = (uint8_t)(0xFF << WLAN_IRQ_AFFINITY_BUSY_CPU); /* �������CPU��Χ */
#if defined(CONFIG_ARCH_HISI) &&  defined(CONFIG_NR_CPUS)
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    struct cpumask thread_cpu_mask;
#endif
#endif

    if (irq_cmd != 0 && thread_cmd != 0) { /* Ӳ�жϺ��߳�ָ�����Ϊ0 */
        /* �û�ָ����Ӳ�ж����ں�,�Լ��߳����ں� */
        irq_cpu = hmac_bindcpu_get_cpu_id(irq_cmd);
        thread_cpu = thread_cmd;
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_irq_cpu, set_thread_cpu}");
    } else if (irq_cmd != 0 && thread_cmd == 0) {  /* ֻ���߳�ָ��Ϊ0 */
        /* �û�ָ����Ӳ�ж����ں�,�߳�����������pps���޾��� */
        irq_cpu = hmac_bindcpu_get_cpu_id(irq_cmd);
        thread_cpu = (req_bind_cpu >= WLAN_IRQ_AFFINITY_BUSY_CPU) ? (big_core) : (~big_core);
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_irq_cpu}");
    } else if (irq_cmd == 0 && thread_cmd != 0){ /* ֻ��Ӳ�ж�ָ��Ϊ0 */
        /* �û�ָ�����߳����ں�,�ж�����������pps���޾��� */
        irq_cpu = (req_bind_cpu >= WLAN_IRQ_AFFINITY_BUSY_CPU) ?
                  WLAN_IRQ_AFFINITY_BUSY_CPU : 0; /* ���ģʽirqĬ�ϰ���CPU4��,С��ģʽirqĬ�ϰ���0�� */
        thread_cpu = thread_cmd;
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_userctl_set_irq::set_thread_cpu}");
    } else {
        g_freq_lock_control.en_userctl_bindcpu = OAL_BUTT;
        return;
    }

    if (!IRQ_CMD_IS_ONECPU(thread_cpu) && (hmac_bindcpu_get_cpu_id(thread_cpu) == irq_cpu)) {
        /* �̰߳���һ��CPU����,���������Ӳ�жϰ󶨺���ͬ,��������Ӧ�ñ��� */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_userctl_set_irq::bind same CPU[%d]}", irq_cpu);
        thread_cpu = ~big_core; /* ��ʱĬ�Ͻ��̰߳���С���� */
    }

    oam_warning_log2(0, OAM_SF_ANY, "{hmac_userctl_set_irq::irq_cpu[%d], thread_cpu[0x%x]}",
        irq_cpu, thread_cpu);

    /* �ж϶��ƻ����عر� */
    if (g_freq_lock_control.en_irq_affinity == OAL_FALSE) {
        return;
    }

    hi110x_hcc_dev_bindcpu(0, g_freq_lock_control.en_userctl_bindcpu, irq_cpu, thread_cpu);

    /* ��pst_rxdata�߳� */
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

    /* ÿ������������ */
    if ((g_freq_lock_control.us_throughput_irq_high) && (g_freq_lock_control.us_throughput_irq_low)) {
        ul_limit_throughput_high = g_freq_lock_control.us_throughput_irq_high;
        ul_limit_throughput_low = g_freq_lock_control.us_throughput_irq_low;
    }

    /* ÿ��PPS���� */
    if ((g_freq_lock_control.ul_irq_pps_high) && (g_freq_lock_control.ul_irq_pps_low)) {
        ul_limit_pps_high = g_freq_lock_control.ul_irq_pps_high;
        ul_limit_pps_low = g_freq_lock_control.ul_irq_pps_low;
    }

    /* ����200Mʱ�д�ˣ�����150M��С�� */
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

    /* ѭ������ͳ�� */
    g_freq_lock_control.uc_timer_cycles++;

    ul_dur_time = oal_jiffies_to_msecs(jiffies - g_freq_lock_control.ul_pre_time);
    /* С��һ������,��ͳ�� */
    if (ul_dur_time < WLAN_FREQ_TIMER_PERIOD - 10) { // 10�ǵ�ǰ��ʱ��������Ҫ��ȥ10msΪһ������
        return;
    }

    /* ����һ��ʱ����ͳ��,�ų���ʱ���쳣 */
    if (ul_dur_time > ((WLAN_FREQ_TIMER_PERIOD * WLAN_THROUGHPUT_STA_PERIOD) << BIT_OFFSET_2)) {
        g_freq_lock_control.ul_pre_time = jiffies;
        g_freq_lock_control.uc_timer_cycles = 0;

        oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_bytes, 0);
        oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_bytes, 0);

        return;
    }

    /* ���ֽ��� */
    ul_rx_throughput = oal_atomic_read(&g_st_wifi_rxtx_total.ul_rx_bytes);
    ul_tx_throughput = oal_atomic_read(&g_st_wifi_rxtx_total.ul_tx_bytes);
    if (ul_dur_time != 0) {
        ul_tx_throughput_mbps = (ul_tx_throughput >> 7) / ul_dur_time;
        ul_rx_throughput_mbps = (ul_rx_throughput >> 7) / ul_dur_time;
    }

    /* Wi-Fiҵ���ر�� */
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
    /* AMSDU+AMPDU�ۺ��л���� */
    hmac_tx_amsdu_ampdu_switch(ul_tx_throughput_mbps);
#endif
    hmac_tx_small_amsdu_switch(ul_rx_throughput_mbps, g_freq_lock_control.ul_tx_pps);

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    hmac_tx_tcp_ack_buf_switch(ul_rx_throughput_mbps);
#endif

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    /* �ۺ���Ӳ���л����,ÿ������̽��һ�� */
    hmac_tx_ampdu_switch(ul_tx_throughput_mbps);
#endif
#ifdef _PRE_WLAN_TCP_OPT
    hmac_tcp_ack_filter_switch(ul_rx_throughput_mbps);
#endif
    /* �������������ж��Ƿ���Ҫbypass ����LNA */
    hmac_rx_dyn_bypass_extlna_switch(ul_tx_throughput_mbps, ul_rx_throughput_mbps);

    /* �ж϶�̬���� */
    hmac_perform_calc_throughput(ul_tx_throughput_mbps, ul_rx_throughput_mbps,
                                 g_freq_lock_control.ul_total_sdio_pps);
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
    /* ����pk mode */
    hmac_update_pk_mode(ul_tx_throughput_mbps, ul_rx_throughput_mbps, ul_dur_time);
#endif

    /* irq 2s ̽��һ�� */
    if (g_freq_lock_control.uc_timer_cycles < WLAN_THROUGHPUT_STA_PERIOD) {
        return;
    }
    g_freq_lock_control.uc_timer_cycles = 0;

    /* 2s��������һ�� */
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_rx_bytes, 0);
    oal_atomic_set(&g_st_wifi_rxtx_total.ul_tx_bytes, 0);

    g_freq_lock_control.ul_pre_time = jiffies;
}


OAL_STATIC uint32_t hmac_wlan_freq_wdg_timeout(void *p_arg)
{
    /* CPU��Ƶ */
    hmac_adjust_freq();

    /* ����ͳ�� */
    hmac_adjust_throughput();
    return OAL_SUCC;
}


void hmac_freq_timer_init(void)
{
    /* ��������ʱ���Ѿ�ע��ɹ��������ٴα�ע�ᣬ��Ȼ�������� */
    if (g_freq_lock_control.hmac_freq_timer.en_is_registerd == OAL_TRUE) {
        return;
    }

    g_freq_lock_control.ul_pre_time = jiffies;

    /* uc_timer_cycles �������㡣����ֹͣ��������͹��ĺ󣬴�С���л���PK mode�ж��޷��ָ����� */
    /* ���ͳ�� */
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
    /* ��ʱ��û��ע�ᣬ�Ͳ���ɾ���ˣ���Ȼ�ᷢ������ */
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
        /* WIFI����,����������ͳ�ƶ�ʱ�� */
        hmac_freq_timer_init();
    } else {
        /* WIFI˯��,�ر�������ͳ�ƶ�ʱ�� */
        hmac_freq_timer_deinit();
    }
}

void hmac_userctl_bindcpu_get_cmd(uint32_t cmd)
{
    uint8_t irq_cmd;
    uint8_t thread_cmd;

    irq_cmd = (uint8_t)((uint16_t)cmd >> 8);  /* ����8λ��ȡ��λӲ�ж����� */
    thread_cmd = (uint8_t)((uint16_t)cmd & 0xff); /* ��0xff��ȡ��λ�߳����� */

    if (IRQ_CMD_IS_ONECPU(irq_cmd)) { /* Ӳ�ж�ֻ�ܰ���һ��CPU�� */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_config_userctl_set_bindcpu::error cmd is [0x%x]}", cmd);
        return;
    }

    if (!irq_cmd && !thread_cmd) {
        /* ��Ĭ�Ϸ�ʽ,��������������PPS�����Զ�ָ��CPU��� */
        /* ����һ�δ�С���л�,����ǰ״̬����ΪС��ģʽ */
        hmac_adjust_set_irq(WLAN_IRQ_AFFINITY_BUSY_CPU);
        hmac_adjust_set_irq(WLAN_IRQ_AFFINITY_IDLE_CPU);
        g_freq_lock_control.en_userctl_bindcpu = OAL_FALSE;
    } else {
        /* ���û�ָ��CPU��� */
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
            /* ��ʼ��ÿ��hal dev��ba info ring�ڴ���Ϣ.TBD:DBDC���丨· */
            hal_host_tx_intr_init(0);
        }
#endif
        /* WIFI�ϵ�,����������ͳ�ƶ�ʱ��,�򿪵�Ƶ���� */
        hmac_freq_timer_init();
    } else {
        /* WIFI�µ�,�ر�������ͳ�ƶ�ʱ��,�رյ�Ƶ���� */
        hmac_freq_timer_deinit();
#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
        /* WIFI�µ�, ֹͣ���ڽ��е�wifiҵ�� */
        hmac_config_change_wifi_state_for_power_down();
#endif
    }
}
