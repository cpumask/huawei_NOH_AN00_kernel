

#ifndef __PLAT_PM_H__
#define __PLAT_PM_H__

/* 其他头文件包含 */
#include <linux/mutex.h>
#include <linux/kernel.h>
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
#include <linux/pm_wakeup.h>
#endif
#ifdef _PRE_CONFIG_HISI_110X_BLUEZ
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#endif
#include "plat_pm_wlan.h"
#include "hw_bfg_ps.h"
#include "board.h"
#ifdef CONFIG_HUAWEI_DSM
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
#include <linux/huawei/dsm/dsm_pub.h>
#else
#include <dsm/dsm_pub.h>
#endif
#endif
#include "hi_drv_gpio.h"

/* 宏定义 */
#define BFG_LOCK_NAME               "bfg_wake_lock"
#define BT_LOCK_NAME                "bt_wake_lock"
#define GNSS_LOCK_NAME              "gnss_wake_lock"

#define DEFAULT_WAKELOCK_TIMEOUT 2000 /* msec */

#define FIRMWARE_CFG_INIT_OK 0x01

#define SUCCESS 0
#define FAILURE 1

#define GNSS_AGREE_SLEEP     1
#define GNSS_NOT_AGREE_SLEEP 0

#define BFGX_SLEEP  0
#define BFGX_ACTIVE 1

#define HOST_DISALLOW_TO_SLEEP 0
#define HOST_ALLOW_TO_SLEEP    1

#define BFGX_PM_ENABLE  1
#define BFGX_PM_DISABLE 0

#define WAIT_DEVACK_MSEC              10
#define WAIT_DEVACK_CNT               10
#define WAIT_DEVACK_TIMEOUT_MSEC      200
#define WAIT_WKUP_DEVACK_TIMEOUT_MSEC 1000

#define SLEEP_10_MSEC              10
#define SLEEP_100_MSEC             100


#define DMD_EVENT_BUFF_SIZE 1024

/* 1103 bootloader DTCM 地址区间是0x20019000~0x2001a620，共0xa20(2592)字节 */
#define HI1103_BOOTLOAD_DTCM_BASE_ADDR 0x20019000
#define HI1103_BOOTLOAD_DTCM_SIZE      2592

/* iomcu power state:0,1->ST_POWERON,8->ST_SLEEP,9->ST_WAKEUP */
#define ST_POWERON       0
#define ST_POWERON_OTHER 1
#define ST_SLEEP         8
#define ST_WAKEUP        9

/* 超时时间要大于wkup dev work中的最长执行时间，否则超时以后进入DFR和work中会同时操作tty，导致冲突 */
#define WAIT_WKUPDEV_MSEC 10000

#define RAM_TEST_RUN_VOLTAGE_BIAS_HIGH 0x0 /* 拉偏高压 */
#define RAM_TEST_RUN_VOLTAGE_BIAS_LOW  0x1 /* 拉偏低压 */

#define RAM_TEST_RUN_VOLTAGE_REG_ADDR     0x50002010
#define RAM_TEST_RUN_PROCESS_SEL_REG_ADDR 0x50002014

/* device soc test专用宏 */
#define SOC_TEST_RUN_FINISH     0
#define SOC_TEST_RUN_BEGIN      1

#define SOC_TEST_STATUS_REG     0x50000014
#define SOC_TEST_MODE_REG       0x50000018
#define SOC_TEST_RST_FLAG       "0x5000001c,4"

/* device soc test mode */
#define TIMER_TEST_FLAG         BIT0
#define REG_MEM_CHECK_TEST_FLAG BIT1
#define INTERRUPT_TEST_FLAG     BIT2
#define MEM_MONITOR_TEST_FLAG   BIT3
#define DMA_TEST_FLAG           BIT4
#define CPU_TRACE_TEST_FLAG     BIT5
#define PATCH_TEST_FLAG         BIT6
#define SSI_MASTER_TEST_FLAG    BIT7
#define EFUSE_TEST_FLAG         BIT8
#define WDT_TEST_FLAG           BIT9
#define MPU_TEST_FLAG           BIT10
#define IPC_TEST_FLAG           BIT11
#define MBIST_TEST_FLAG         BIT12
#define SCHEDULE_FPU_TEST_FLAG  BIT13
#define I3C_SLAVE_TEST_FLAG     BIT14

#define BT_SLEEP_TIME       1500
#define PLATFORM_SLEEP_TIME 50

/* 检查是否bfg timer工作正常,5s内无有mod timer, 异常处理,防止始终不睡眠 */
#define PL_CHECK_TIMER_WORK 5000

/* 检查是否gnss异常不投票睡眠的次数:5s */
#define PL_CHECK_GNSS_VOTE_CNT (5000 / PLATFORM_SLEEP_TIME)

enum UART_STATE_ENUM {
    UART_NOT_READY = 0,
    UART_READY = 1,
};

/* BFGX系统上电加载异常类型 */
enum BFGX_POWER_ON_EXCEPTION_ENUM {
    BFGX_POWER_FAILED = -1,
    BFGX_POWER_SUCCESS = 0,

    BFGX_POWER_PULL_POWER_GPIO_FAIL = 1,
    BFGX_POWER_TTY_OPEN_FAIL = 2,
    BFGX_POWER_TTY_FLOW_ENABLE_FAIL = 3,

    BFGX_POWER_WIFI_DERESET_BCPU_FAIL = 4,
    BFGX_POWER_WIFI_ON_BOOT_UP_FAIL = 5,

    BFGX_POWER_WIFI_OFF_BOOT_UP_FAIL = 6,
    BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL = 7,

    BFGX_POWER_WAKEUP_FAIL = 8,
    BFGX_POWER_OPEN_CMD_FAIL = 9,

    BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT = 10,

    BFGX_POWER_ENUM_BUTT,
};

/* wifi系统上电加载异常类型 */
enum WIFI_POWER_ON_EXCEPTION_ENUM {
    WIFI_POWER_FAIL = -1,
    WIFI_POWER_SUCCESS = 0,
    WIFI_POWER_PULL_POWER_GPIO_FAIL = 1,

    WIFI_POWER_BFGX_OFF_BOOT_UP_FAIL = 2,
    WIFI_POWER_BFGX_OFF_FIRMWARE_DOWNLOAD_FAIL = 3,

    WIFI_POWER_BFGX_ON_BOOT_UP_FAIL = 4,
    WIFI_POWER_BFGX_DERESET_WCPU_FAIL = 5,
    WIFI_POWER_BFGX_ON_FIRMWARE_DOWNLOAD_FAIL = 6,
    WIFI_POWER_ON_FIRMWARE_DOWNLOAD_INTERRUPT = 7,

    WIFI_POWER_BFGX_OFF_PULL_WLEN_FAIL = 8,
    WIFI_POWER_BFGX_ON_PULL_WLEN_FAIL = 9,
    WIFI_POWER_ON_FIRMWARE_FILE_OPEN_FAIL = 10,

    WIFI_POWER_ON_CALI_FAIL = 11,

    WIFI_POWER_ENUM_BUTT,
};

#ifdef CONFIG_HI110X_GPS_SYNC
enum gnss_rat_mode_enum {
    gnss_rat_mode_no_service = 0,
    gnss_rat_mode_gsm = 1,
    gnss_rat_mode_cdma = 2,
    gnss_rat_mode_wcdma = 3,
    gnss_rat_mode_lte = 6,
    gnss_rat_mode_nr = 11,
    gnss_rat_mode_butt
};

enum gnss_sync_mode_enum {
    gnss_sync_mode_unknown = -1,
    gnss_sync_mode_lte = 0,
    gnss_sync_mode_lte2 = 1,
    gnss_sync_mode_cdma = 2,
    gnss_sync_mode_g1 = 3,
    gnss_sync_mode_g2 = 4,
    gnss_sync_mode_g3 = 5,
    gnss_sync_mode_pw = 6,
    gnss_sync_mode_sw = 7,
    gnss_sync_mode_nstu = 8,
    gnss_sync_mode_butt
};

enum gnss_sync_version_enum {
    gnss_sync_version_off = 0, // feature switch off
    gnss_sync_version_4g = 1, // support 2/3/4g
    gnss_sync_version_5g = 2, // based 1 and add 5g
    gnss_sync_version_butt
};
#endif

#ifdef _PRE_CONFIG_HISI_110X_BLUEZ
struct hisi_bt_dev {
    struct hci_dev *hdev;
    struct task_struct *bt_recv_task;
};
#endif

struct pm_top {
    /* mutex for sync */
    struct mutex host_mutex;
        /* flag for firmware cfg file init */
    uint64 firmware_cfg_init_flag;
    struct wlan_pm_s *wlan_pm_info;
    struct pm_drv_data *bfg_pm_info;
    struct pm_drv_data *gnss_pm_info;
};

/* private data for pm driver */
struct pm_drv_data {
    struct ps_core_s *ps_core_data;
    int index;
    /* wake lock for bfg,be used to prevent host form suspend */
    oal_wakelock_stru bus_wake_lock;
    oal_wakelock_stru bt_wake_lock;
    oal_wakelock_stru gnss_wake_lock;

    /* wakelock protect spinlock while wkup isr VS allow sleep ack and devack_timer */
    spinlock_t wakelock_protect_spinlock;

    /* bfgx VS. bfg timer spinlock */
    spinlock_t uart_state_spinlock;

    /* uart could be used or not */
    uint8 uart_state;

    /* mark receiving data after set dev as sleep state but before get ack of device */
    uint8 rcvdata_bef_devack_flag;

    uint8 uc_dev_ack_wait_cnt;

    /* bfgx sleep state */
    uint8 bfgx_dev_state;

    /* hal layer ioctl set flag not really close bt */
    uint32 bt_fake_close_flag;

    spinlock_t bfg_irq_spinlock;

    /* bfg irq num */
    uint32 bfg_irq;

    /* wakeup gpio */
    int32 wakeup_host_gpio;

    uint32 ul_irq_stat;

    /* bfg wakeup host count */
    uint32 bfg_wakeup_host;

    uint32 wakeup_src_debug;

    /* gnss lowpower state */
    atomic_t gnss_sleep_flag;

    atomic_t bfg_needwait_devboot_flag;

    /* flag to mark whether enable lowpower or not */
    uint32 bfgx_pm_ctrl_enable;
    uint8 bfgx_lowpower_enable;
    uint8 bfgx_bt_lowpower_enable;
    uint8 bfgx_gnss_lowpower_enable;
    uint8 bfgx_nfc_lowpower_enable;

    /* workqueue for wkup device */
    struct workqueue_struct *wkup_dev_workqueue;
    struct work_struct wkup_dev_work;
    struct work_struct send_disallow_msg_work;
    struct work_struct send_allow_sleep_work;
    uint32 uart_baud_switch_to;

    /* wait device ack timer */
    struct timer_list bfg_timer;
    uint32 bfg_timer_mod_cnt;
    uint32 bfg_timer_mod_cnt_pre;
    unsigned long bfg_timer_check_time;
    uint64 rx_pkt_gnss_pre;
    uint32 gnss_votesleep_check_cnt;
    struct timer_list dev_ack_timer;

    /* the completion for waiting for host wake up device ok */
    struct completion host_wkup_dev_comp;
    /* the completion for waiting for host wake up device ack ok */
    struct completion dev_ack_comp;
    /* the completion for waiting for device sleep ack ok */
    struct completion dev_slp_comp;
    /* the completion for waiting for dev boot ok */
    struct completion dev_bootok_ack_comp;
#ifdef _PRE_CONFIG_HISI_110X_BLUEZ
    struct hisi_bt_dev st_bt_dev;
#endif

    int32 (*change_baud_rate)(struct ps_core_s *, int64, uint8);
    int32 (*bfg_power_set)(struct pm_drv_data *, uint8, uint8);
    void (*bfg_wake_lock)(struct pm_drv_data *);
    void (*bfg_wake_unlock)(struct pm_drv_data *);
    int32 (*bfgx_dev_state_get)(struct pm_drv_data *);
    void (*bfgx_dev_state_set)(struct pm_drv_data *, uint8);
    int32 (*operate_beat_timer)(uint8);
    void (*bfgx_uart_state_set)(struct pm_drv_data *, uint8);
    int8 (*bfgx_uart_state_get)(struct pm_drv_data *);

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    int32 (*download_patch)(void);
    int32 (*recv_patch)(const uint8 *, int32 count);
    int32 (*write_patch)(uint8 *, int32 count);
#endif
};

#define  index2name(index) ((index == 0) ? "BUART" : "GUART")

#ifdef CONFIG_HI110X_GPS_SYNC
struct gnss_sync_data {
    void __iomem *addr_base_virt;
    uint32 addr_offset;
    uint32 version;
};
#endif

#define BUCK_MODE_OFFSET                        14
#define BUCK_MODE_MASK                          0xC000   // bit[15:14]
#define EXT_BUCK_OFF                             0        // 无外置buck,全内置
#define EXT_BUCK_I2C_CTL                         1        // I2C控制独立外置buck
#define EXT_BUCK_GPIO_CTL                        2        // GPIO控制独立外置buck
#define EXT_BUCK_HOST_CTL                        3        // host控制共享外置buck

#define BUCK_NUMBER_OFFSET                      0
#define BUCK_NUMBER_MASK                        0xFF     // bit[7:0]
#define BUCK_NUMBER_AE                          0x0      // ANNA ELSA上用的外置buck
#define BUCK_NUMBER_ONNT                        0x2      // ONNT 上用的外置buck

/* EXTERN VARIABLE */
extern int g_ram_test_detail_tsensor_dump;
extern int g_ram_test_run_voltage_bias_sel;

#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
typedef oal_uint32 (*work_cb)(oal_void);
void pm_host_walcb_register(work_cb suspend_cb, work_cb resume_cb);
#endif
#ifdef _PRE_CONFIG_ARCH_KIRIN_S4_FEATURE
extern irqreturn_t bfg_wake_host_isr(int irq, void *dev_id);
#endif

/* EXTERN FUNCTION */
extern void BuckPowerCtrl(uint8 enable, uint8 subsys);
extern int32 wait_gpio_level(int32 gpio_num, int32 gpio_level, unsigned long timeout);
extern void bfgx_gpio_intr_enable(struct pm_drv_data *pm_data, uint32 ul_en);
extern bool bfgx_is_boot_up(struct pm_drv_data *pm_data);
extern struct pm_drv_data *pm_get_drvdata(int index);
extern int32 host_wkup_dev(struct pm_drv_data *pm_data);
extern int32 bfgx_other_subsys_all_shutdown(struct pm_drv_data *pm_data, uint8 subsys);
extern void bfgx_print_subsys_state(void);
extern bool wlan_is_shutdown(void);
extern bool bfgx_is_shutdown(void);
extern int32 wlan_power_on(void);
extern int32 wlan_power_off(void);
extern int32_t wifi_power_fail_process(int32_t error);
extern int32 bfgx_power_on(struct pm_drv_data *pm_data, uint8 subsys);
extern int32 bfgx_power_off(struct pm_drv_data *pm_data, uint8 subsys);
extern int32 bfgx_pm_feature_set(struct pm_drv_data *pm_data);
extern int firmware_download_function(uint32 which_cfg);
typedef int32 (*firmware_downlaod_privfunc)(void);
extern int firmware_download_function_priv(uint32 which_cfg, firmware_downlaod_privfunc);
extern oal_int32 hi110x_get_wifi_power_stat(oal_void);
extern int32 device_mem_check(unsigned long long *time);
extern int32 memcheck_is_working(void);
extern void bfg_check_timer_work(struct pm_drv_data *pm_data);
#ifdef CONFIG_HI110X_GPS_SYNC
extern struct gnss_sync_data *gnss_get_sync_data(void);
extern int gnss_sync_init(void);
extern void gnss_sync_exit(void);
#endif
#ifdef CONFIG_HUAWEI_DSM
extern void hw_110x_dsm_client_notify(int sub_sys, int dsm_id, const char *fmt, ...);
#endif
extern int32 ps_pm_register(struct ps_core_s *ps_core_d, int index);
extern int32 ps_pm_unregister(struct ps_core_s *ps_core_d, int index);
extern int low_power_init(void);
extern void low_power_exit(void);
extern struct pm_top* pm_get_top(void);
#ifdef _PRE_SUSPORT_OEMINFO
void resume_wlan_wakeup_host_gpio(void);
#endif
#endif
