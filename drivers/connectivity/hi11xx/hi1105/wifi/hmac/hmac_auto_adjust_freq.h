

#ifndef __MAC_AUTO_ADJUST_FREQ_H__
#define __MAC_AUTO_ADJUST_FREQ_H__

/*
 * set cpu freq A53-0
 * this function used on K3V3+ platform
 */
#define CPU_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define CPU_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"

/*
 * set ddr freq
 * this function used on K3V3+ platform
 */
#define DDR_MAX_FREQ "/sys/class/devfreq/ddrfreq/max_freq"
#define DDR_MIN_FREQ "/sys/class/devfreq/ddrfreq/min_freq"

#define MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_3  (3)   /* ����3�����ڶ���Ҫ��Ƶ�Ž�Ƶ */
#define MAX_DEGRADE_FREQ_COUNT_THRESHOLD_SUCCESSIVE_10 (100) /* �а�ʱ����100�����ڶ���Ҫ��Ƶ�Ž�Ƶ */
/* WIFI���������ϴ�ʱ���շ��жϰ��ڴ�� */
#define WLAN_IRQ_AFFINITY_IDLE_CPU 0
#define WLAN_IRQ_AFFINITY_BUSY_CPU 4

#define WLAN_CPU_FREQ_IDLE  0
#define WLAN_CPU_FREQ_SUPER 1

#define WLAN_IRQ_THROUGHPUT_THRESHOLD_HIGH 200 /* mbps */
#define WLAN_IRQ_THROUGHPUT_THRESHOLD_LOW  150 /* mbps */

#define WLAN_IRQ_PPS_THRESHOLD_HIGH 25000 /* pps */
#define WLAN_IRQ_PPS_THRESHOLD_LOW  5000  /* pps */

#define WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_HIGH 800 /* mbps */
#define WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_LOW  500 /* mbps */

#define WLAN_FREQ_TIMER_PERIOD     100 /* ��ʱ��100ms��ʱ */
#define WLAN_THROUGHPUT_STA_PERIOD 20

#define WLAN_THROUGHPUT_LOAD_LOW   10 /* �͸���10M */
#define WLAN_THROUGHPUT_LOAD_HIGH 200
#define PM_QOS_THOUGHTPUT_VALUE 60
#define HMAC_FREQ_S_TRANS_TO_MS 1000

#define IRQ_CMD_IS_ONECPU(_cmd) ((_cmd) & ((_cmd) - 1)) /* �ж�_cmd���Ƿ�ֻ��һλbitΪ1 */

enum {
    /* indicate to set cpu/ddr max frequencies */
    SCALING_MAX_FREQ = 0,
    /* indicate to set cpu/ddr min frequencies */
    SCALING_MIN_FREQ = 1,
};

enum {
    /* frequency lock disable mode */
    FREQ_LOCK_DISABLE = 0,
    /* frequency lock enable mode */
    FREQ_LOCK_ENABLE = 1,
};
typedef uint8_t oal_freq_lock_enum_uint8;

typedef enum {
    CMD_SET_AUTO_FREQ_ENDABLE,
    CMD_SET_DEVICE_FREQ_VALUE,
    CMD_SET_CPU_FREQ_VALUE,
    CMD_SET_DDR_FREQ_VALUE,
    CMD_SET_AUTO_BYPASS_DEVICE_AUTO_FREQ,
    CMD_GET_DEVICE_AUTO_FREQ,
    CMD_SET_DEVICE_FREQ_TC,
    CMD_AUTO_FREQ_BUTT,
} oal_auto_freq_cmd_enum;
typedef uint8_t oal_auto_freq_cmd_enum_uint8;

typedef enum {
    HMAC_FREQ_LEVEL_0,
    HMAC_FREQ_LEVEL_1,
    HMAC_FREQ_LEVEL_2,
    HMAC_FREQ_LEVEL_3,

    HMAC_FREQ_LEVEL_BUTT
} hmac_freq_level_enum;
typedef uint8_t hmac_freq_level_enum_uint8;

typedef enum {
    HOST_SPEED_FREQ_LEVEL_0,
    HOST_SPEED_FREQ_LEVEL_1,
    HOST_SPEED_FREQ_LEVEL_2,
    HOST_SPEED_FREQ_LEVEL_3,

    HOST_SPEED_FREQ_LEVEL_BUTT
} host_speed_freq_level_enum;
typedef uint8_t host_speed_freq_level_uint8;

typedef struct {
    uint8_t uc_trx_stat_log_en;
    uint8_t auc_resv[3];
    oal_atomic ul_tx_pkts; /* WIFI ҵ����֡ͳ�� */
    oal_atomic ul_rx_pkts; /* WIFI ҵ�����֡ͳ�� */
    oal_atomic ul_tx_bytes; /* WIFI ҵ����֡ͳ�� */
    oal_atomic ul_rx_bytes; /* WIFI ҵ�����֡ͳ�� */
} wifi_txrx_pkt_stat;
extern wifi_txrx_pkt_stat g_st_wifi_rxtx_total;
typedef struct {
    uint8_t uc_device_type; /* device��Ƶ���� */
    uint8_t uc_reserve[3];  /* �����ֶ� */
} device_speed_freq_level_stru;
typedef struct {
    uint32_t ul_speed_level;  /* ���������� */
    uint32_t ul_min_cpu_freq; /* CPU��Ƶ���� */
    uint32_t ul_min_ddr_freq; /* DDR��Ƶ���� */
} host_speed_freq_level_stru;
extern host_speed_freq_level_stru g_host_speed_freq_level[];
extern device_speed_freq_level_stru g_device_speed_freq_level[];

typedef struct {
    oal_bool_enum_uint8 en_is_inited;
    uint8_t uc_lock_mod;        /* ʹ�ܿ��� */
    uint8_t uc_curr_lock_level; /* ��ǰ��Ƶ�ȼ� */
    uint8_t uc_req_lock_level;  /* Ŀ����Ƶ�ȼ� */
    uint32_t ul_pre_jiffies;
    uint32_t ul_adjust_count;
    oal_mutex_stru st_lock_freq_mtx;
    oal_bool_enum_uint8 en_irq_affinity; /* �жϿ��� */
    uint8_t uc_timer_cycles;           /* ��ʱ�������� */
    uint8_t uc_cur_irq_cpu;            /* ��ǰ�ж�����CPU */
    uint8_t uc_req_irq_cpu;            /* �����ж�����CPU */
    uint32_t ul_pre_time;              /* ��һ��ͳ��ʱ�� */
    uint32_t ul_total_sdio_pps;        /* �������� */
    uint32_t ul_tx_pps;                /* ���ͷ���pps */
    uint32_t ul_rx_pps;                /* ���շ���pps */
    uint16_t us_throughput_irq_high;
    uint16_t us_throughput_irq_low;
    uint32_t ul_irq_pps_high;
    uint32_t ul_irq_pps_low;
    oal_bool_enum_uint8 en_userctl_bindcpu;
    uint8_t uc_userctl_irqbind;
    uint8_t uc_userctl_threadbind;
    uint8_t uc_resv;               /* �����ֶ� */
    /* CPU��Ƶ */
    oal_bool_enum_uint8 uc_lock_max_cpu_freq;
    oal_bool_enum_uint8 uc_lock_dma_latency;
    uint8_t uc_req_super_cpu_freq;            /* ����CPU��Ƶ */
    uint8_t uc_cur_cpu_freq_state;
    uint16_t us_lock_cpu_th_high;
    uint16_t us_lock_cpu_th_low;
    uint16_t dma_latency_value;

    frw_timeout_stru hmac_freq_timer; /* auto freq timer */
} freq_lock_control_stru;
extern freq_lock_control_stru g_freq_lock_control;

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
extern oal_bool_enum_uint8 g_en_pk_mode_swtich;
extern uint32_t g_st_pk_mode_high_th_table[WLAN_PROTOCOL_CAP_BUTT][WLAN_BW_CAP_BUTT];
extern uint32_t g_st_pk_mode_low_th_table[WLAN_PROTOCOL_CAP_BUTT][WLAN_BW_CAP_BUTT];
#endif

/* ��ǰwifiҵ���� */
typedef struct {
    oal_bool_enum_uint8 en_wifi_rx_busy;
    oal_bool_enum_uint8 en_wifi_tx_busy;
    uint8_t uc_res[2];
} freq_wifi_load_stru;
extern freq_wifi_load_stru g_st_wifi_load;


oal_bool_enum_uint8 hmac_set_auto_freq_mod(oal_freq_lock_enum_uint8 uc_freq_enable);
void hmac_auto_freq_wifi_rx_stat(uint32_t ul_pkt_count);
void hmac_auto_freq_wifi_tx_stat(uint32_t ul_pkt_count);
void hmac_adjust_freq(void);
void hmac_adjust_set_freq(void);
void hmac_wifi_auto_freq_ctrl_init(void);
void hmac_wifi_auto_freq_ctrl_deinit(void);

void hmac_freq_timer_init(void);
void hmac_freq_timer_deinit(void);
void hmac_auto_freq_wifi_rx_bytes_stat(uint32_t ul_pkt_bytes);
void hmac_auto_freq_wifi_tx_bytes_stat(uint32_t ul_pkt_bytes);
void hmac_wifi_pm_state_notify(oal_bool_enum_uint8 en_wake_up);
void hmac_wifi_state_notify(oal_bool_enum_uint8 en_wifi_on);
void hmac_userctl_bindcpu_get_cmd(uint32_t cmd);
oal_bool_enum_uint8 hmac_wifi_rx_is_busy(void);
#endif

