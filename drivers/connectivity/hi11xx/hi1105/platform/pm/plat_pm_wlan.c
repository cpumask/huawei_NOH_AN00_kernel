

/* 头文件包含 */
#define HI11XX_LOG_MODULE_NAME     "[WLAN_PM]"
#define HI11XX_LOG_MODULE_NAME_VAR wlan_pm_loglevel
#include <linux/module.h> /* kernel module definitions */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/platform_device.h>
#include <linux/kobject.h>
#include <linux/irq.h>

#ifdef CONFIG_HISI_IDLE_SLEEP
#include <linux/hisi/hisi_idle_sleep.h>
#endif
#include <linux/mmc/sdio.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/host.h>
#include <linux/gpio.h>
#include <linux/pm_wakeup.h>

#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
#include <linux/reboot.h>
#endif
#include "oal_hcc_bus.h"
#include "plat_type.h"
#include "plat_debug.h"
#include "board.h"
#include "plat_pm_wlan.h"
#include "plat_pm.h"

#include "oal_hcc_host_if.h"
#include "oam_ext_if.h"
#include "bfgx_exception_rst.h"
#include "securec.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_PLAT_PM_WLAN_C

static struct wlan_pm_s *g_wlan_pm_info = OAL_PTR_NULL;

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
static pm_callback_stru g_wlan_pm_callback = {
    .pm_wakeup_dev = wlan_pm_wakeup_dev,
    .pm_state_get = wlan_pm_state_get,
    .pm_wakeup_host = wlan_pm_wakeup_host,
    .pm_feed_wdg = wlan_pm_feed_wdg,
    .pm_wakeup_dev_ack = wlan_pm_wakeup_dev_ack,
    .pm_disable = wlan_pm_disable_check_wakeup,
};
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static RAW_NOTIFIER_HEAD(wifi_pm_chain);
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_uint8 g_custom_cali_done = OAL_FALSE;
#endif

oal_bool_enum g_wlan_host_pm_switch = OAL_TRUE;
oal_uint8 g_wlan_device_pm_switch = OAL_TRUE;  // device 低功耗开关
oal_uint8 g_wlan_ps_mode = 1;
oal_uint8 g_wlan_fast_ps_dyn_ctl = 0;  // app layer dynamic ctrl enable
oal_uint8 g_wlan_min_fast_ps_idle = 1;
oal_uint8 g_wlan_max_fast_ps_idle = 10;
oal_uint8 g_wlan_auto_ps_screen_on = 5;
oal_uint8 g_wlan_auto_ps_screen_off = 5;

int g_hi11xx_wlan_open_failed_bypass = 0; /* wlan open failed bypass */
oal_debug_module_param(g_hi11xx_wlan_open_failed_bypass, int, S_IRUGO | S_IWUSR);

oal_uint16 g_download_rate_limit_pps = 0;
EXPORT_SYMBOL_GPL(g_download_rate_limit_pps);

EXPORT_SYMBOL_GPL(g_wlan_host_pm_switch);
EXPORT_SYMBOL_GPL(g_wlan_device_pm_switch);
EXPORT_SYMBOL_GPL(g_wlan_ps_mode);
EXPORT_SYMBOL_GPL(g_wlan_min_fast_ps_idle);
EXPORT_SYMBOL_GPL(g_wlan_max_fast_ps_idle);
EXPORT_SYMBOL_GPL(g_wlan_auto_ps_screen_on);
EXPORT_SYMBOL_GPL(g_wlan_auto_ps_screen_off);
EXPORT_SYMBOL_GPL(g_wlan_fast_ps_dyn_ctl);

/* 30000ms/100ms = 300 cnt */
static oal_uint32 g_wlan_slp_req_forbid_limit = (30000) / (WLAN_SLEEP_TIMER_PERIOD * WLAN_SLEEP_DEFAULT_CHECK_CNT);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
EXPORT_SYMBOL_GPL(g_custom_cali_done);
#endif

void wlan_pm_wakeup_work(oal_work_stru *pst_worker);
void wlan_pm_sleep_work(oal_work_stru *pst_worker);
void wlan_pm_freq_adjust_work(oal_work_stru *pst_worker);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0))
void wlan_pm_wdg_timeout(unsigned long data);
#else
void wlan_pm_wdg_timeout(struct timer_list *t);
#endif

int32 wlan_pm_wakeup_done_callback(void *data);
int32 wlan_pm_close_done_callback(void *data);
int32 wlan_pm_open_bcpu_done_callback(void *data);
int32 wlan_pm_close_bcpu_done_callback(void *data);
int32 wlan_pm_halt_bcpu_done_callback(void *data);
int32 wlan_pm_stop_wdg(struct wlan_pm_s *pst_wlan_pm_info);
oal_int wlan_pm_work_submit(struct wlan_pm_s *pst_wlan_pm, oal_work_stru *pst_worker);
void wlan_pm_info_clean(void);
void wlan_pm_deepsleep_delay_timeout(unsigned long data);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_int32 wlan_pm_register_notifier(struct notifier_block *nb)
{
    return raw_notifier_chain_register(&wifi_pm_chain, nb);
}

oal_void wlan_pm_unregister_notifier(struct notifier_block *nb)
{
    raw_notifier_chain_unregister(&wifi_pm_chain, nb);
}
#endif

/*
 * 函 数 名  : wlan_pm_get_drv
 * 功能描述  : 获取全局wlan结构
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
struct wlan_pm_s *wlan_pm_get_drv(oal_void)
{
    return g_wlan_pm_info;
}

EXPORT_SYMBOL_GPL(wlan_pm_get_drv);

/*
 * 函 数 名  : wlan_pm_sleep_request
 * 功能描述  : 发送sleep 请求给device
 * 返 回 值  : SUCC/FAIL
 */
OAL_STATIC oal_int32 wlan_pm_sleep_request(struct wlan_pm_s *pst_wlan_pm)
{
    oal_int32 ret;
    uint32_t time_left;
    pst_wlan_pm->ul_sleep_stage = SLEEP_REQ_SND;

    oal_init_completion(&pst_wlan_pm->st_sleep_request_ack);

    if (oal_print_rate_limit(PRINT_RATE_MINUTE)) { /* 1分钟打印一次 */
        hcc_bus_chip_info(pst_wlan_pm->pst_bus, OAL_FALSE, OAL_FALSE);
    }

    ret = hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_SLEEP_REQ);
    if (ret != OAL_SUCC) {
        pst_wlan_pm->ul_sleep_fail_request++;
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_sleep_request fail !\n");
        return OAL_ERR_CODE_SLEEP_FAIL;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "sleep request send!");
    up(&pst_wlan_pm->pst_bus->rx_sema);

    time_left = oal_wait_for_completion_timeout(&pst_wlan_pm->st_sleep_request_ack,
                                                (oal_uint32)oal_msecs_to_jiffies(WLAN_SLEEP_MSG_WAIT_TIMEOUT));
    if (time_left == 0) {
        pst_wlan_pm->ul_sleep_fail_wait_timeout++;
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_sleep_work wait completion fail !\n");
        return OAL_ERR_CODE_SLEEP_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_allow_sleep_callback
 * 功能描述  : device应答allow_sleep消息处理
 */
oal_int32 wlan_pm_allow_sleep_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_pm_allow_sleep_callback");

    pst_wlan_pm->ul_sleep_stage = SLEEP_ALLOW_RCV;
    oal_complete(&pst_wlan_pm->st_sleep_request_ack);

    return SUCCESS;
}

/*
 * 函 数 名  : wlan_pm_disallow_sleep_callback
 * 功能描述  : device应答allow_sleep消息处理
 */
oal_int32 wlan_pm_disallow_sleep_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    if (oal_print_rate_limit(PRINT_RATE_SECOND)) { /* 1s打印一次 */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "sleep request dev disalow, device busy");
    }

    pst_wlan_pm->ul_sleep_stage = SLEEP_DISALLOW_RCV;
    oal_complete(&pst_wlan_pm->st_sleep_request_ack);

    return SUCCESS;
}

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
OAL_STATIC hcc_switch_action g_plat_pm_switch_action;
OAL_STATIC oal_int32 wlan_switch_action_callback(oal_uint32 dev_id, hcc_bus *old_bus, hcc_bus *new_bus, oal_void *data)
{
    struct wlan_pm_s *pst_wlan_pm = NULL;

    if (data == NULL) {
        return -OAL_EINVAL;
    }

    if (dev_id != HCC_CHIP_110X_DEV) {
        /* ignore other wlan dev */
        return OAL_SUCC;
    }

    pst_wlan_pm = (struct wlan_pm_s *)data;

    /* Update new bus */
    pst_wlan_pm->pst_bus = new_bus;
    pst_wlan_pm->pst_bus->pst_pm_callback = &g_wlan_pm_callback;

    return OAL_SUCC;
}
#endif

/*
 * 函 数 名  : wlan_pm_dts_init
 * 功能描述  : os dts init function
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_int32 wlan_pm_dts_init(oal_void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int ret;
    u32 host_gpio_sample_low = 0;
    struct device_node *np = NULL;
    np = of_find_compatible_node(NULL, NULL, DTS_NODE_HI110X_WIFI);
    if (np == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can't find node [%s]", DTS_NODE_HI110X_WIFI);
        return -OAL_ENODEV;
    }

    ret = of_property_read_u32(np, DTS_PROP_HI110X_HOST_GPIO_SAMPLE, &host_gpio_sample_low);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "read prop [%s] fail, ret=%d", DTS_PROP_HI110X_HOST_GPIO_SAMPLE, ret);
        return ret;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "%s=%d", DTS_PROP_HI110X_HOST_GPIO_SAMPLE, host_gpio_sample_low);

    g_hi11xx_wlan_open_failed_bypass = !!host_gpio_sample_low;
#endif
    return OAL_SUCC;
}

#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
static int wlan_poweroff_notify_sys(struct notifier_block *this, unsigned long code, void *unused)
{
    oal_uint32 err = OAL_SUCC;

    ps_print_info("wlan_poweroff_notify_sys enter\n");

    if (board_get_wlan_enable_gpio_val()) {
        err = wlan_pm_close();
        if (err != OAL_SUCC) {
            ps_print_warning("wlan_poweroff_notify_sys call wlan_pm_close failed (err=%d)\n", err);
        }
    } else {
        ps_print_info("wlan already poweroff no call wlan_pm_close\n");
    }

    return NOTIFY_OK;
}

static struct notifier_block g_wlan_poweroff_notifier = {
    .notifier_call = wlan_poweroff_notify_sys,
};
#endif
OAL_STATIC void wlan_pm_init_ext2(struct wlan_pm_s *pst_wlan_pm)
{
#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
    int32 err;
#endif

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_WAKEUP_SUCC,
                         wlan_pm_wakeup_done_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_ALLOW_SLEEP,
                         wlan_pm_allow_sleep_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_DISALLOW_SLEEP,
                         wlan_pm_disallow_sleep_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_POWEROFF_ACK,
                         wlan_pm_close_done_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_OPEN_BCPU_ACK,
                         wlan_pm_open_bcpu_done_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_CLOSE_BCPU_ACK,
                         wlan_pm_close_bcpu_done_callback,
                         pst_wlan_pm);
    hcc_message_register(hcc_get_110x_handler(),
                         D2H_MSG_HALT_BCPU,
                         wlan_pm_halt_bcpu_done_callback,
                         pst_wlan_pm);

    pst_wlan_pm->pst_bus->data_int_count = 0;
    pst_wlan_pm->pst_bus->wakeup_int_count = 0;
#endif

    wlan_pm_dts_init();
#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
    err = register_reboot_notifier(&g_wlan_poweroff_notifier);
    if (err) {
        ps_print_warning("Failed to registe wlan_poweroff_notifier (err=%d)\n", err);
    }
#endif
}

OAL_STATIC int32 wlan_pm_init_ext1(struct wlan_pm_s *pst_wlan_pm)
{
    pst_wlan_pm->ul_wlan_pm_enable = OAL_FALSE;
    pst_wlan_pm->ul_apmode_allow_pm_flag = OAL_TRUE; /* 默认允许下电 */

    /* work queue初始化 */
    pst_wlan_pm->pst_pm_wq = oal_create_singlethread_workqueue("wlan_pm_wq");
    if (pst_wlan_pm->pst_pm_wq == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "Failed to create wlan_pm_wq!");
        kfree(pst_wlan_pm);
        return OAL_PTR_NULL;
    }
    /* register wakeup and sleep work */
    oal_init_work(&pst_wlan_pm->st_wakeup_work, wlan_pm_wakeup_work);
    oal_init_work(&pst_wlan_pm->st_sleep_work, wlan_pm_sleep_work);

    /* 初始化芯片自检work */
    oal_init_work(&pst_wlan_pm->st_ram_reg_test_work, wlan_device_mem_check_work);

    /* sleep timer初始化 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0))
    init_timer(&pst_wlan_pm->st_watchdog_timer);
    pst_wlan_pm->st_watchdog_timer.data = (uintptr_t)pst_wlan_pm;
    pst_wlan_pm->st_watchdog_timer.function = (void *)wlan_pm_wdg_timeout;
#else
    timer_setup(&pst_wlan_pm->st_watchdog_timer, wlan_pm_wdg_timeout, 0);
#endif
    pst_wlan_pm->ul_wdg_timeout_cnt = WLAN_SLEEP_DEFAULT_CHECK_CNT;
    pst_wlan_pm->ul_wdg_timeout_curr_cnt = 0;
    pst_wlan_pm->ul_packet_cnt = 0;
    pst_wlan_pm->ul_packet_total_cnt = 0;
    pst_wlan_pm->ul_packet_check_time = 0;
    pst_wlan_pm->ul_sleep_forbid_check_time = 0;

    oal_wake_lock_init(&pst_wlan_pm->st_deepsleep_wakelock, "wifi_deeepsleep_delay_wakelock");

    pst_wlan_pm->ul_wlan_power_state = POWER_STATE_SHUTDOWN;
    pst_wlan_pm->ul_wlan_dev_state = HOST_ALLOW_TO_SLEEP;
    pst_wlan_pm->ul_sleep_stage = SLEEP_STAGE_INIT;

    pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_get_pm_pause_func = OAL_PTR_NULL;
    pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_open_notify = OAL_PTR_NULL;
    pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_pm_state_notify = OAL_PTR_NULL;

    pst_wlan_pm->ul_wkup_src_print_en = OAL_FALSE;

    g_wlan_pm_info = pst_wlan_pm;

    oal_init_completion(&pst_wlan_pm->st_open_bcpu_done);
    oal_init_completion(&pst_wlan_pm->st_close_bcpu_done);
    oal_init_completion(&pst_wlan_pm->st_close_done);
    oal_init_completion(&pst_wlan_pm->st_wakeup_done);
    oal_init_completion(&pst_wlan_pm->st_wifi_powerup_done);
    oal_init_completion(&pst_wlan_pm->st_sleep_request_ack);
    oal_init_completion(&pst_wlan_pm->st_halt_bcpu_done);

    return OAL_TRUE;
}

/*
 * 函 数 名  : wlan_pm_init
 * 功能描述  : WLAN PM初始化接口
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
struct wlan_pm_s *wlan_pm_init(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = NULL;
    int32 l_ret;

    hi110x_board_info *pst_board = get_hi110x_board_info();

    if (pst_board == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    pst_wlan_pm = kzalloc(sizeof(struct wlan_pm_s), GFP_KERNEL);
    if (pst_wlan_pm == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "no mem to allocate wlan_pm_data");
        return OAL_PTR_NULL;
    }

    memset_s(pst_wlan_pm, sizeof(struct wlan_pm_s), 0, sizeof(struct wlan_pm_s));
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    memset_s((oal_void *)&g_plat_pm_switch_action, OAL_SIZEOF(g_plat_pm_switch_action),
             0, OAL_SIZEOF(g_plat_pm_switch_action));
    g_plat_pm_switch_action.name = "plat_pm_wlan";
    g_plat_pm_switch_action.switch_notify = wlan_switch_action_callback;
    hcc_switch_action_register(&g_plat_pm_switch_action, (oal_void *)pst_wlan_pm);

    pst_wlan_pm->pst_bus = hcc_get_current_110x_bus();
    if (pst_wlan_pm->pst_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc bus is NULL, failed to create wlan_pm_wq!");
        kfree(pst_wlan_pm);
        return OAL_PTR_NULL;
    }
    pst_wlan_pm->pst_bus->pst_pm_callback = &g_wlan_pm_callback;
#endif

    /* 圈复杂度优化 */
    l_ret = wlan_pm_init_ext1(pst_wlan_pm);
    if (l_ret == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    wlan_pm_init_ext2(pst_wlan_pm);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_init ok!");
    return pst_wlan_pm;
}

/*
 * 函 数 名  : wlan_pm_exit
 * 功能描述  : WLAN pm退出接口
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint wlan_pm_exit(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return OAL_SUCC;
    }

    wlan_pm_stop_wdg(pst_wlan_pm);

    oal_wake_unlock_force(&pst_wlan_pm->st_deepsleep_wakelock);

    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_WAKEUP_SUCC);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_WLAN_READY);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_ALLOW_SLEEP);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_DISALLOW_SLEEP);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_POWEROFF_ACK);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_OPEN_BCPU_ACK);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_CLOSE_BCPU_ACK);
    hcc_bus_message_unregister(pst_wlan_pm->pst_bus, D2H_MSG_HALT_BCPU);
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    hcc_switch_action_unregister(&g_plat_pm_switch_action);
#endif

    kfree(pst_wlan_pm);

    g_wlan_pm_info = OAL_PTR_NULL;
#ifdef _PRE_CONFIG_HISIFASTPOWER_DOWN_STATE
    unregister_reboot_notifier(&g_wlan_poweroff_notifier);
#endif

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_exit ok!");

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_is_poweron
 * 功能描述  : wifi是否上电
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint32 wlan_pm_is_poweron(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    if (pst_wlan_pm->ul_wlan_power_state == POWER_STATE_OPEN) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}
EXPORT_SYMBOL_GPL(wlan_pm_is_poweron);

/*
 * 函 数 名  : wlan_pm_get_wifi_srv_handler
 * 功能描述  : 获取回调handler指针
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
struct wifi_srv_callback_handler *wlan_pm_get_wifi_srv_handler(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return NULL;
    }

    return &pst_wlan_pm->st_wifi_srv_handler;
}
EXPORT_SYMBOL_GPL(wlan_pm_get_wifi_srv_handler);

/*
 * 函 数 名  : wlan_pm_idle_sleep_vote
 * 功能描述  : wlan投票是否允许kirin进入32k idle模式
 * 输入参数  : TRUE:允许，FALSE:不允许
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_void wlan_pm_idle_sleep_vote(oal_uint8 uc_allow)
{
#ifdef CONFIG_HISI_IDLE_SLEEP
    if (uc_allow == ALLOW_IDLESLEEP) {
        hisi_idle_sleep_vote(ID_WIFI, 0);
    } else {
        hisi_idle_sleep_vote(ID_WIFI, 1);
    }
#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
    hcc_bus_wlan_pm_vote(hcc_get_current_110x_bus(), (uint8_t)uc_allow);
#endif
#endif
}

/*
 * 函 数 名  : wlan_pm_custom_cali_waitretry
 * 功能描述  : wifi定制化等待应答重新调度一次, 防止调度问题导致失败
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
OAL_STATIC int32_t wlan_pm_custom_cali_waitretry(struct wlan_pm_s *pst_wlan_pm)
{
    int32_t loglevel;
    int32_t wait_relt;
    declare_dft_trace_key_info("wlan_wait_custom_cali_fail_retry", OAL_DFT_TRACE_FAIL);
    oam_error_log1(0, OAM_SF_PWR, "wlan_pm_wait_custom_cali timeout retry %d", HOST_WAIT_BOTTOM_INIT_TIMEOUT);

    hcc_print_current_trans_info(1);

    loglevel = hcc_set_all_loglevel(HI11XX_LOG_VERBOSE);

    hcc_sched_transfer(hcc_get_110x_handler());

    wait_relt = oal_wait_for_completion_killable_timeout(&pst_wlan_pm->pst_bus->st_device_ready,
                                                         oal_msecs_to_jiffies(HOST_WAIT_INIT_RETRY));
    if (wait_relt == 0) {
        hcc_print_current_trans_info(1);
#ifndef CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT
        {
            struct hcc_handler *hcc = hcc_get_110x_handler();
            if (hcc != NULL && hcc->hcc_transer_info.hcc_transfer_thread != NULL) {
                sched_show_task(hcc->hcc_transer_info.hcc_transfer_thread);
            }
        }
#endif
        hcc_set_all_loglevel(loglevel);
        declare_dft_trace_key_info("wlan_wait_custom_cali_retry_fail", OAL_DFT_TRACE_FAIL);
        oam_error_log1(0, OAM_SF_PWR, "wlan_pm_wait_custom_cali timeout retry failed %d", HOST_WAIT_INIT_RETRY);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_PLAT, CHR_WIFI_DRV_ERROR_POWER_ON_CALL_TIMEOUT);
        if (!g_hi11xx_wlan_open_failed_bypass) {
            if (oal_trigger_bus_exception(pst_wlan_pm->pst_bus, OAL_TRUE) == OAL_TRUE) {
                oal_print_hi11xx_log(HI11XX_LOG_WARN, "dump device mem when cali custom failed!");
            }
            return OAL_FAIL;
        }
    } else if (wait_relt == -ERESTARTSYS) {
        oam_warning_log0(0, OAM_SF_CFG, "wlan_pm_open::receive interrupt single");
        hcc_set_all_loglevel(loglevel);
        return OAL_FAIL;
    } else {
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_PLAT, CHR_WIFI_DRV_ERROR_POWER_ON_CALL_TIMEOUT);
    }
    hcc_set_all_loglevel(loglevel);
    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_custom_cali
 * 功能描述  : wifi定制化参数下发和上电校准
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
OAL_STATIC int32_t wlan_pm_custom_cali(struct wlan_pm_s *pst_wlan_pm)
{
    int32_t wait_ret;
    // 初始化配置定制化参数
    if (hwifi_hcc_customize_h2d_data_cfg() != INI_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "wlan_pm_open::hwifi_hcc_customize_h2d_data_cfg fail");
    }

    oal_init_completion(&pst_wlan_pm->pst_bus->st_device_ready);

    if (g_custom_process_func.p_custom_cali_func == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open::NO g_custom_process_func registered");
        wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_PLAT, CHR_WIFI_DRV_ERROR_POWER_ON_NO_CUSTOM_CALL);

        return OAL_FAIL;
    }

    /* 如果校准下发成功则等待device ready；否则继续打开wifi */
    if (g_custom_process_func.p_custom_cali_func() == OAL_SUCC) {
        wait_ret = oal_wait_for_completion_killable_timeout(&pst_wlan_pm->pst_bus->st_device_ready,
                                                            oal_msecs_to_jiffies(HOST_WAIT_BOTTOM_INIT_TIMEOUT));
        if (wait_ret == -ERESTARTSYS) {
            oam_warning_log0(0, OAM_SF_CFG, "wlan_pm_open::receive interrupt single");
            return OAL_FAIL;
        }

        // 如果判断超时异常，再尝试一次
        if (wait_ret == 0) {
            if (wlan_pm_custom_cali_waitretry(pst_wlan_pm) != OAL_SUCC) {
                return OAL_FAIL;
            }
        }
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void wlan_pm_open_notify(struct wlan_pm_s *pst_wlan_pm)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    raw_notifier_call_chain(&wifi_pm_chain, WIFI_PM_POWERUP_EVENT, (oal_void *)pst_wlan_pm); /* powerup chain */
#endif

    wlan_pm_enable();

    /* 将timeout值恢复为默认值，并启动定时器 */
    wlan_pm_set_timeout(WLAN_SLEEP_DEFAULT_CHECK_CNT);

    if (hcc_dev_switch_enable(HCC_CHIP_110X_DEV) != OAL_SUCC) {
        declare_dft_trace_key_info("hcc_dev_switch_enable failed", OAL_DFT_TRACE_FAIL);
    }

#ifdef _PRE_SDIO_TAE_DEBUG
    hcc_bus_pwr_on_pcie();
#endif

    /* WIFI开机成功后,通知业务侧 */
    if (pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_open_notify != OAL_PTR_NULL) {
        pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_open_notify(OAL_TRUE);
    }
}

OAL_STATIC oal_int32 wlan_pm_open_check(struct wlan_pm_s *pst_wlan_pm)
{
    if (pst_wlan_pm == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open:pst_wlan_pm is NULL!");
        return OAL_FAIL;
    }

    if (pst_wlan_pm->pst_bus == NULL) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open::get 110x bus failed!");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_OPEN, CHR_PLAT_DRV_ERROR_POWER_ON_NON_BUS);
        return OAL_FAIL;
    }

    if (pst_wlan_pm->ul_wlan_power_state == POWER_STATE_OPEN) {
        oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_open::aleady opened");
        return OAL_ERR_CODE_ALREADY_OPEN;
    }

    if (pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count == 0) {
        /* make sure open only lock once */
        hcc_bus_wake_lock(pst_wlan_pm->pst_bus);
        wlan_pm_idle_sleep_vote(DISALLOW_IDLESLEEP);
        oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_idle_sleep_vote DISALLOW::hisi_idle_sleep_vote ID_WIFI 1!");
    }

    oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_open::get wakelock %lu!",
                     pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    pst_wlan_pm->ul_open_cnt++;

    return OAL_SUCC;
}
/*
 * 函 数 名  : wlan_pm_open
 * 功能描述  : open wifi,如果bfgx没有开启,上电,否则，下命令开WCPU
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_int32 wlan_pm_open(oal_void)
{
    oal_int32 ret;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct pm_top* pm_top_data = pm_get_top();

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_pm_open enter");

    mutex_lock(&(pm_top_data->host_mutex));
    ret = wlan_pm_open_check(pst_wlan_pm);
    if (ret != OAL_SUCC) {
        mutex_unlock(&(pm_top_data->host_mutex));
        return ret;
    }

    ret = wlan_power_on();
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open::wlan_power_on fail!");
        pst_wlan_pm->ul_wlan_power_state = POWER_STATE_SHUTDOWN;
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
        wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
        mutex_unlock(&(pm_top_data->host_mutex));
        if (ret != OAL_EINTR) {
            declare_dft_trace_key_info("wlan_power_on_fail", OAL_DFT_TRACE_FAIL);
        }
        return OAL_FAIL;
    }

    if (wlan_pm_custom_cali(pst_wlan_pm) == OAL_FAIL) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open::cali fail!");
        wifi_power_fail_process(WIFI_POWER_ON_CALI_FAIL);
        pst_wlan_pm->ul_wlan_power_state = POWER_STATE_SHUTDOWN;
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
        wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
        mutex_unlock(&(pm_top_data->host_mutex));
        /* block here to debug */
        oal_chip_error_task_block();
        return OAL_FAIL;
    }

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_open::wlan_pm_open SUCC!!");
    declare_dft_trace_key_info("wlan_open_succ", OAL_DFT_TRACE_SUCC);

    wlan_pm_open_notify(pst_wlan_pm);

    mutex_unlock(&(pm_top_data->host_mutex));

    return OAL_SUCC;
}
oal_int32 g_ram_reg_test_result = OAL_SUCC;
unsigned long long g_ram_reg_test_time = 0;
/* 03产测delay W、B的delay分别是3s和5s，05产测delay W、B的delay暂定是5s和4s */
oal_int32 g_wlan_mem_check_mdelay = 3000;
oal_int32 g_bfgx_mem_check_mdelay = 5000;

wlan_memdump_t g_wlan_memdump_cfg = { 0x60000000, 0x1000 };

oal_uint32 set_wlan_mem_check_mdelay(int32 mdelay)
{
    g_wlan_mem_check_mdelay = mdelay;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "set_wlan_mem_check_mdelay::set delay:%dms!!", g_wlan_mem_check_mdelay);
    return 0;
}
oal_uint32 set_bfgx_mem_check_mdelay(int32 mdelay)
{
    g_bfgx_mem_check_mdelay = mdelay;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "g_bfgx_mem_check_mdelay::set delay:%dms!!", g_bfgx_mem_check_mdelay);
    return 0;
}

EXPORT_SYMBOL_GPL(set_wlan_mem_check_mdelay);

wlan_memdump_t *get_wlan_memdump_cfg(void)
{
    return &g_wlan_memdump_cfg;
}

oal_uint32 set_wlan_mem_check_memdump(int32 addr, int32 len)
{
    g_wlan_memdump_cfg.addr = addr;
    g_wlan_memdump_cfg.len = len;
    g_wlan_memdump_cfg.en = 1;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "set_wlan_mem_check_memdump set ok: addr:0x%x,len:%d", addr, len);
    return 0;
}

EXPORT_SYMBOL_GPL(set_wlan_mem_check_memdump);

oal_int32 wlan_device_mem_check(oal_int32 l_runing_test_mode)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_int32        l_chip_type = get_hi110x_subchip_type();
    oal_uint32       ul_ret;

    if (oal_warn_on(pst_wlan_pm == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_device_mem_check: pst_wlan_pm is null \n");
        return -OAL_FAIL;
    }

    g_ram_reg_test_result = OAL_SUCC;
    g_ram_reg_test_time = 0;
    hcc_bus_wake_lock(pst_wlan_pm->pst_bus);

    /* 03&05共host，后续项目调试时间时可注掉这部分 */
    if (l_chip_type == BOARD_VERSION_HI1105) {
        ul_ret = set_wlan_mem_check_mdelay(5000);  // wifi 5000ms
        ul_ret += set_bfgx_mem_check_mdelay(4000); // bfg 4000ms
    } else {
        ul_ret = set_wlan_mem_check_mdelay(3000);  // wifi 3000ms
        ul_ret += set_bfgx_mem_check_mdelay(5000); // bfg 5000ms
    }

    if (ul_ret != 0) {
        oam_warning_log0(0, OAM_SF_PWR, "st_ram_reg_test_work set mdelay fail !\n");
    }
    if (l_runing_test_mode == 0) {
        /* dbc工位，低压memcheck */
        g_ram_test_run_voltage_bias_sel = RAM_TEST_RUN_VOLTAGE_BIAS_LOW;
    } else {
        /* 老化工位，高压memcheck */
        g_ram_test_run_voltage_bias_sel = RAM_TEST_RUN_VOLTAGE_BIAS_HIGH;
    }

    if (wlan_pm_work_submit(pst_wlan_pm, &pst_wlan_pm->st_ram_reg_test_work) != 0) {
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
        oam_warning_log0(0, OAM_SF_PWR, "st_ram_reg_test_work submit work fail !\n");
    }

    return OAL_SUCC;
}

oal_int32 wlan_device_mem_check_result(unsigned long long *time)
{
    *time = g_ram_reg_test_time;
    return g_ram_reg_test_result;
}

oal_void wlan_device_mem_check_work(oal_work_stru *pst_worker)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct pm_top* pm_top_data = pm_get_top();

    mutex_lock(&(pm_top_data->host_mutex));

    hcc_bus_disable_state(pst_wlan_pm->pst_bus, OAL_BUS_STATE_ALL);
    g_ram_reg_test_result = device_mem_check(&g_ram_reg_test_time);
    hcc_bus_enable_state(pst_wlan_pm->pst_bus, OAL_BUS_STATE_ALL);

    hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);

    mutex_unlock(&(pm_top_data->host_mutex));
}
EXPORT_SYMBOL_GPL(wlan_device_mem_check);
EXPORT_SYMBOL_GPL(wlan_device_mem_check_result);

EXPORT_SYMBOL_GPL(wlan_pm_open);

OAL_STATIC oal_uint32 wlan_pm_close_pre(struct wlan_pm_s *pst_wlan_pm)
{
    if (pst_wlan_pm == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_PWR, "pst_wlan_pm is null");
        return OAL_FAIL;
    }

    if (!pst_wlan_pm->ul_apmode_allow_pm_flag) {
        oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_close,AP mode,do not shutdown power.");
        return OAL_ERR_CODE_FOBID_CLOSE_DEVICE;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_close start!!");

    hcc_dev_switch_disable(HCC_CHIP_110X_DEV);

    pst_wlan_pm->ul_close_cnt++;

    if (pst_wlan_pm->ul_wlan_power_state == POWER_STATE_SHUTDOWN) {
        return OAL_ERR_CODE_ALREADY_CLOSE;
    }

    /* WIFI关闭前,通知业务侧 */
    if (pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_open_notify != OAL_PTR_NULL) {
        pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_open_notify(OAL_FALSE);
    }

    wlan_pm_disable();

    wlan_pm_stop_wdg(pst_wlan_pm);

    oal_wake_unlock_force(&pst_wlan_pm->st_deepsleep_wakelock);

    wlan_pm_info_clean();

    /* mask rx ip data interrupt */
    hcc_bus_rx_int_mask(hcc_get_current_110x_bus());

#ifdef _PRE_SDIO_TAE_DEBUG
    hcc_bus_pwr_off_pcie();
#endif

    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wlan_pm_close_after(struct wlan_pm_s *pst_wlan_pm)
{
    pst_wlan_pm->ul_wlan_power_state = POWER_STATE_SHUTDOWN;

    /* unmask rx ip data interrupt */
    hcc_bus_rx_int_unmask(hcc_get_current_110x_bus());

    hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    raw_notifier_call_chain(&wifi_pm_chain, WIFI_PM_POWERDOWN_EVENT, (oal_void *)pst_wlan_pm); /* powerdown chain */
#endif

    wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);

    oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_close release wakelock %lu!\n",
                     pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    hcc_bus_wakelocks_release_detect(pst_wlan_pm->pst_bus);

    hcc_bus_reset_trans_info(pst_wlan_pm->pst_bus);

    return OAL_SUCC;
}


/*
 * 函 数 名  : wlan_pm_close
 * 功能描述  : close wifi,如果bfgx没有开,下电，否则下命令关WCPU
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint32 wlan_pm_close(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct pm_top* pm_top_data = pm_get_top();
    oal_uint32 ret;

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_pm_close enter");

    /* For Debug, print key_info_log */
    if (hi11xx_get_os_build_variant() != HI1XX_OS_BUILD_VARIANT_USER) {
        if (oal_print_rate_limit(PRINT_RATE_HOUR)) { /* 1小时打印一次 */
            oal_dft_print_all_key_info();
        }
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = wlan_pm_close_pre(pst_wlan_pm);
    if (ret != OAL_SUCC) {
        mutex_unlock(&(pm_top_data->host_mutex));
        return ret;
    }

    if (wlan_power_off() != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_power_off FAIL!\n");
        mutex_unlock(&(pm_top_data->host_mutex));
        declare_dft_trace_key_info("wlan_power_off_fail", OAL_DFT_TRACE_FAIL);
        return OAL_FAIL;
    }

    ret = wlan_pm_close_after(pst_wlan_pm);
    if (ret != OAL_SUCC) {
        mutex_unlock(&(pm_top_data->host_mutex));
        return ret;
    }

    mutex_unlock(&(pm_top_data->host_mutex));

    hcc_dev_flowctrl_on(hcc_get_110x_handler(), 0);

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_close succ!\n");
    declare_dft_trace_key_info("wlan_close_succ", OAL_DFT_TRACE_SUCC);
    return OAL_SUCC;
}
EXPORT_SYMBOL_GPL(wlan_pm_close);

/*
 * 函 数 名  : wlan_pm_close_by_shutdown
 * 功能描述  : 在Linux全系统shutdown时调用，关闭硬件通道，不需要释放软件资源
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint32 wlan_pm_close_by_shutdown(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct pm_top* pm_top_data = pm_get_top();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_PWR, "pst_wlan_pm is null");
        return OAL_FAIL;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_close_by_shutdown start!!");

    mutex_lock(&(pm_top_data->host_mutex));

    if (pst_wlan_pm->ul_wlan_power_state == POWER_STATE_SHUTDOWN) {
        mutex_unlock(&(pm_top_data->host_mutex));
        return OAL_ERR_CODE_ALREADY_CLOSE;
    }

    if (wlan_power_off() != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_close_by_shutdown FAIL!\n");
    }

    pst_wlan_pm->ul_wlan_power_state = POWER_STATE_SHUTDOWN;

    mutex_unlock(&(pm_top_data->host_mutex));

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_close_by_shutdown succ!\n");
    return OAL_SUCC;
}
EXPORT_SYMBOL_GPL(wlan_pm_close_by_shutdown);

/*
 * 函 数 名  : wlan_pm_enable
 * 功能描述  : 使能wlan平台低功耗
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint32 wlan_pm_enable(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (!g_wlan_host_pm_switch) {
        return OAL_SUCC;
    }

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    if (pst_wlan_pm->ul_wlan_pm_enable == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_enable already enabled!");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_SUCC;
    }

    pst_wlan_pm->ul_wlan_pm_enable = OAL_TRUE;

    wlan_pm_feed_wdg();

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_enable SUCC!");

    return OAL_SUCC;
}
EXPORT_SYMBOL_GPL(wlan_pm_enable);

/*
 * 函 数 名  : wlan_pm_disable
 * 功能描述  : 去使能wlan平台低功耗
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint32 wlan_pm_disable_check_wakeup(oal_int32 flag)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    if (pst_wlan_pm->ul_wlan_pm_enable == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_disable already disabled!");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_SUCC;
    }

    if (flag == OAL_TRUE) {
        if (wlan_pm_wakeup_dev() != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_PWR, "pm wake up dev fail!");
        }
    }

    pst_wlan_pm->ul_wlan_pm_enable = OAL_FALSE;

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    wlan_pm_stop_wdg(pst_wlan_pm);

    oal_cancel_work_sync(&pst_wlan_pm->st_wakeup_work);
    oal_cancel_work_sync(&pst_wlan_pm->st_sleep_work);

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_disable SUCC!");

    return OAL_SUCC;
}
EXPORT_SYMBOL_GPL(wlan_pm_disable_check_wakeup);

oal_uint32 wlan_pm_disable(oal_void)
{
    return wlan_pm_disable_check_wakeup(OAL_TRUE);
}
EXPORT_SYMBOL_GPL(wlan_pm_disable);

oal_uint32 wlan_pm_statesave(oal_void)
{
    if (g_wlan_host_pm_switch) {
        return wlan_pm_disable();
    } else {
        return OAL_SUCC;
    }
}

EXPORT_SYMBOL_GPL(wlan_pm_statesave);

oal_uint32 wlan_pm_staterestore(oal_void)
{
    if (g_wlan_host_pm_switch) {
        return wlan_pm_enable();
    } else {
        return OAL_SUCC;
    }
}
EXPORT_SYMBOL_GPL(wlan_pm_staterestore);

/*
 * 函 数 名  : wlan_pm_init_dev
 * 功能描述  : 初始化device的状态
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint wlan_pm_init_dev(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_int ret;
    hcc_bus *pst_bus = NULL;

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    pst_bus = hcc_get_current_110x_bus();
    if (oal_warn_on(pst_bus == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_init get non bus!");

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_PM_INIT_NO_BUS);
        return OAL_FAIL;
    }

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_init_dev!\n");

    pst_wlan_pm->ul_wlan_dev_state = HOST_DISALLOW_TO_SLEEP;

    /* wait for bus wakeup */
    ret = down_timeout(&pst_bus->sr_wake_sema, WLAN_BUS_SEMA_TIME);
    if (ret == -ETIME) {
        oam_error_log0(0, OAM_SF_PWR, "host bus controller is not ready!");
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
        declare_dft_trace_key_info("wifi_controller_wait_init_fail", OAL_DFT_TRACE_FAIL);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_PM_SDIO_NO_READY);
        return OAL_FAIL;
    }
    up(&pst_bus->sr_wake_sema);

    return (oal_uint)hcc_bus_wakeup_request(pst_wlan_pm->pst_bus);
}

/*
 * 函 数 名  : wlan_pm_pcie_wakeup_cmd
 * 功能描述  : pcie拉GPIO唤醒device，并发送wakeup cmd消息
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
OAL_STATIC oal_uint wlan_pm_pcie_wakeup_cmd(struct wlan_pm_s *pst_wlan_pm)
{
    oal_int32 ret;

    oal_init_completion(&pst_wlan_pm->st_wakeup_done);
    /* 依赖回来的GPIO 做唤醒，此时回来的消息PCIE 还不确定是否已经唤醒，PCIE通道不可用 */
    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_FALSE);
    oal_atomic_set(&g_wakeup_dev_wait_ack, 1);
    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wifi wakeup cmd send,wakelock cnt %lu",
                         pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);
    ret = hcc_bus_wakeup_request(pst_wlan_pm->pst_bus);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "wakeup request failed ret=%d", ret);
        declare_dft_trace_key_info("wifi wakeup cmd send fail", OAL_DFT_TRACE_FAIL);
        (void)ssi_dump_err_regs(SSI_ERR_PCIE_GPIO_WAKE_FAIL);
        return OAL_FAIL;
    }

    pst_wlan_pm->ul_wlan_dev_state = HOST_DISALLOW_TO_SLEEP;

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_sdio_wakeup_cmd
 * 功能描述  : sdio对唤醒寄存器写0
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
OAL_STATIC oal_uint wlan_pm_sdio_wakeup_cmd(struct wlan_pm_s *pst_wlan_pm)
{
    oal_int32 ret;
    oal_uint8 uc_retry;

    oal_init_completion(&pst_wlan_pm->st_wakeup_done);
    /* set sdio register */
    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_FALSE);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wifi wakeup cmd send,wakelock cnt %lu",
                         pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);
    ret = hcc_bus_wakeup_request(pst_wlan_pm->pst_bus);
    if (ret != OAL_SUCC) {
        declare_dft_trace_key_info("wifi wakeup cmd send fail", OAL_DFT_TRACE_FAIL);
        for (uc_retry = 0; uc_retry < WLAN_SDIO_MSG_RETRY_NUM; uc_retry++) {
            oal_msleep(SLEEP_10_MSEC);
            ret = hcc_bus_wakeup_request(pst_wlan_pm->pst_bus);
            if (ret == OAL_SUCC) {
                break;
            }

            (void)ssi_dump_err_regs(SSI_ERR_SDIO_CMD_WAKE_FAIL);

            oam_error_log2(0, OAM_SF_PWR, "oal_wifi_wakeup_dev retry %d ret = %d", uc_retry, ret);
            declare_dft_trace_key_info("wifi wakeup cmd send retry fail", OAL_DFT_TRACE_FAIL);
        }

        /* after max retry still fail,log error */
        if (ret != OAL_SUCC) {
            pst_wlan_pm->ul_wakeup_fail_set_reg++;
            oam_error_log1(0, OAM_SF_PWR, "oal_wifi_wakeup_dev Fail ret = %d", ret);
            declare_dft_trace_key_info("oal_wifi_wakeup_dev final fail", OAL_DFT_TRACE_EXCEP);
            oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);
            return OAL_FAIL;
        }
    }

    oal_atomic_set(&g_wakeup_dev_wait_ack, 1);

    pst_wlan_pm->ul_wlan_dev_state = HOST_DISALLOW_TO_SLEEP;
    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);
    return OAL_SUCC;
}
/*
 * 函 数 名  : wlan_pm_gpio_wakeup_dev
 * 功能描述  : 唤醒device
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
OAL_STATIC oal_uint wlan_pm_gpio_wakeup_dev(struct wlan_pm_s *pst_wlan_pm)
{
#ifdef _PRE_PLAT_FEATURE_HI110X_SDIO_GPIO_WAKE
    oal_uint32 ul_ret;
    oal_uint8 uc_retry;

    /*
     * use gpio to wakeup sdio device
     * 1.触发上升沿
     * 2.sdio wakeup 寄存器写0,写0会取消sdio mem pg功能
     */
    for (uc_retry = 0; uc_retry < WLAN_SDIO_MSG_RETRY_NUM; uc_retry++) {
        oal_init_completion(&pst_wlan_pm->st_wakeup_done);
        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_FALSE);
        oal_atomic_set(&g_wakeup_dev_wait_ack, 1);
        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);
        board_host_wakeup_dev_set(0); /* wakeup dev */
        oal_udelay(100);  // delay 100us
        board_host_wakeup_dev_set(1); /* wakeup dev */
        ul_ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_wakeup_done,
                                                 (oal_uint32)oal_msecs_to_jiffies(WLAN_WAKUP_MSG_WAIT_TIMEOUT));
        if (ul_ret != 0) {
            /* sdio gpio wakeup dev sucess */
            declare_dft_trace_key_info("gpio_wakeup_sdio_succ", OAL_DFT_TRACE_SUCC);
            break;
        }
    }

    if (uc_retry == WLAN_SDIO_MSG_RETRY_NUM) {
        oam_error_log1(0, OAM_SF_PWR, "oal_sdio_gpio_wakeup_dev retry %d failed", uc_retry);
        declare_dft_trace_key_info("oal_sdio_gpio_wakeup_dev final fail", OAL_DFT_TRACE_EXCEP);
        (void)ssi_dump_err_regs(SSI_ERR_SDIO_GPIO_WAKE_FAIL);

        return OAL_FAIL;
    }

    hcc_bus_enable_state(pst_wlan_pm->pst_bus, OAL_BUS_STATE_ALL);

#endif
    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_wakeup_cmd_send
 * 功能描述  : 唤醒命令发送
 * 返 回 值  : OAL_ERR_CODE_SEMA_TIMEOUT : 获取总线信号量失败，不发送，也不做DFR处理。
 *             OAL_ERR_CODE_WAKEUP_FAIL_PROC:唤醒命令发送失败，不重试，唤醒失败处理。
 *             OAL_SUCC : 成功
 */
OAL_STATIC oal_uint wlan_pm_wakeup_cmd_send(struct wlan_pm_s *pst_wlan_pm)
{
    oal_int32 ret;
    hcc_bus *pst_bus = pst_wlan_pm->pst_bus;

    wlan_pm_idle_sleep_vote(DISALLOW_IDLESLEEP);

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wait bus wakeup");

    /* wait for bus wakeup */
    ret = down_timeout(&pst_bus->sr_wake_sema, WLAN_BUS_SEMA_TIME);
    if (ret == -ETIME) {
        pst_wlan_pm->ul_wakeup_fail_wait_sdio++;
        oam_error_log0(0, OAM_SF_PWR, "wifi controller is not ready!");
        declare_dft_trace_key_info("wifi_controller_wait_fail", OAL_DFT_TRACE_FAIL);
        return OAL_ERR_CODE_SEMA_TIMEOUT;
    }
    up(&pst_bus->sr_wake_sema);

    if (pst_wlan_pm->pst_bus->bus_type == HCC_BUS_PCIE) {
        if (wlan_pm_pcie_wakeup_cmd(pst_wlan_pm) == OAL_FAIL) {
            return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
        }
    } else if (pst_wlan_pm->pst_bus->bus_type == HCC_BUS_SDIO) {
        if (wlan_pm_gpio_wakeup_dev(pst_wlan_pm) == OAL_FAIL) {
            return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
        }

        if (wlan_pm_sdio_wakeup_cmd(pst_wlan_pm) == OAL_FAIL) {
            return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
        }
        up(&pst_wlan_pm->pst_bus->rx_sema);
    } else {
        declare_dft_trace_key_info("oal_wifi_wakeup_dev final fail", OAL_DFT_TRACE_EXCEP);
        return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
    }

#ifdef PLATFORM_DEBUG_ENABLE
    if (is_dfr_test_en(WIFI_WKUP_FAULT)) {
        oam_error_log0(0, OAM_SF_PWR, "[dfr test] trigger wkup fail!\n");
        pst_wlan_pm->wakeup_err_count = WLAN_WAKEUP_FAIL_MAX_TIMES;
        return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
    }
#endif

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_wakeup_wait_ack
 * 功能描述  : 唤醒等待芯片的ack
 * 返 回 值  : OAL_ERR_CODE_WAKEUP_RETRY : 唤醒命令发送成功，ack超时，重试。
 *             OAL_ERR_CODE_WAKEUP_FAIL_PROC:唤醒命令发送失败，不重试，DFR处理。
 */
OAL_STATIC oal_uint wlan_pm_wakeup_wait_ack(struct wlan_pm_s *pst_wlan_pm)
{
    oal_uint32 ret;

    ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_wakeup_done,
                                          (oal_uint32)oal_msecs_to_jiffies(WLAN_WAKUP_MSG_WAIT_TIMEOUT));
    if (ret == 0) {
        oal_int32 sleep_state = hcc_bus_get_sleep_state(pst_wlan_pm->pst_bus);
        if ((sleep_state == DISALLOW_TO_SLEEP_VALUE) || (sleep_state < 0)) {
            if (oal_unlikely(sleep_state < 0)) {
                oam_error_log1(0, OAM_SF_PWR, "get state failed, sleep_state=%d", sleep_state);
            }

            pst_wlan_pm->ul_wakeup_fail_timeout++;
            oam_warning_log0(0, OAM_SF_PWR, "oal_wifi_wakeup_dev SUCC to set 0xf0 = 0");
            hcc_bus_sleep_request(pst_wlan_pm->pst_bus);
            pst_wlan_pm->ul_wlan_dev_state = HOST_ALLOW_TO_SLEEP;

            oam_error_log2(0, OAM_SF_PWR, "wlan_pm_wakeup_dev [%d]wait device complete fail,wait time %d ms!",
                           pst_wlan_pm->wakeup_err_count, WLAN_WAKUP_MSG_WAIT_TIMEOUT);
            oal_print_hi11xx_log(HI11XX_LOG_INFO,
                                 KERN_ERR "wlan_pm_wakeup_dev [%d]wait device complete fail,wait time %d ms!",
                                 pst_wlan_pm->wakeup_err_count, WLAN_WAKUP_MSG_WAIT_TIMEOUT);
            return OAL_ERR_CODE_WAKEUP_RETRY;
        } else {
            pst_wlan_pm->ul_wakeup_fail_set_reg++;
            oam_error_log0(0, OAM_SF_PWR, "wakeup_dev Fail to set 0xf0 = 0");
            oal_print_hi11xx_log(HI11XX_LOG_INFO, KERN_ERR "wakeup_dev Fail to set 0xf0 = 0");
            pst_wlan_pm->ul_wlan_dev_state = HOST_ALLOW_TO_SLEEP;
            return OAL_ERR_CODE_WAKEUP_FAIL_PROC;
        }
    }

    declare_dft_trace_key_info("wlan_wakeup_succ", OAL_DFT_TRACE_SUCC);
    pst_wlan_pm->ul_wakeup_succ++;
    pst_wlan_pm->wakeup_err_count = 0;
    pst_wlan_pm->ul_wdg_timeout_curr_cnt = 0;
    pst_wlan_pm->ul_packet_cnt = 0;
    pst_wlan_pm->ul_packet_check_time = jiffies + msecs_to_jiffies(WLAN_PACKET_CHECK_TIME);
    pst_wlan_pm->ul_packet_total_cnt = 0;
    pst_wlan_pm->ul_sleep_fail_forbid_cnt = 0;
    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_wakeup_fail_proc
 * 功能描述  : 唤醒失败后处理:启动DFR
 */
OAL_STATIC oal_void wlan_pm_wakeup_fail_proc(struct wlan_pm_s *pst_wlan_pm)
{
    declare_dft_trace_key_info("wlan_wakeup_fail", OAL_DFT_TRACE_FAIL);
    pst_wlan_pm->wakeup_err_count++;

    /* pm唤醒失败超出门限，启动dfr流程 */
    if (pst_wlan_pm->wakeup_err_count > WLAN_WAKEUP_FAIL_MAX_TIMES) {
        oam_error_log1(0, OAM_SF_PWR, "Now ready to enter DFR process after [%d]times wlan_wakeup_fail!",
                       pst_wlan_pm->wakeup_err_count);
        pst_wlan_pm->wakeup_err_count = 0;
        hcc_bus_exception_submit(pst_wlan_pm->pst_bus, WIFI_WAKEUP_FAIL);
    }
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_WIFI_WKUP_DEV);
}

/*
 * 函 数 名  : wlan_pm_wakeup_notify
 * 功能描述  : 唤醒后通知周边模块
 */
OAL_STATIC oal_void wlan_pm_wakeup_notify(struct wlan_pm_s *pst_wlan_pm)
{
    /* HOST WIFI退出低功耗,通知业务侧开启定时器 */
    if (pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_pm_state_notify != OAL_PTR_NULL) {
        pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_pm_state_notify(OAL_TRUE);
    }
    oal_usleep_range(500, 510); // 睡眠时间在500到510us之间

    hcc_bus_wakeup_complete(pst_wlan_pm->pst_bus);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    raw_notifier_call_chain(&wifi_pm_chain, WIFI_PM_WAKEUP_EVENT, (oal_void *)pst_wlan_pm); /* wakeup chain */
#endif
}

/*
 * 函 数 名  : wlan_pm_wakeup_check
 * 功能描述  : 唤醒前检查是否需要发唤醒CMD
 * 返 回 值  : OAL_SUCC : 已处于唤醒状态，无需处理，调用者可以发包。
 *             OAL_ERR_CODE_WAKEUP_PROCESSING:已有一个唤醒在处理中，无需处理，不允许发包
 *             OAL_SUCC_GO_ON : 满足唤醒条件检查。继续处理发送wakeup cmd。
 */
OAL_STATIC oal_uint wlan_pm_wakeup_check(struct wlan_pm_s *pst_wlan_pm)
{
    if (pst_wlan_pm == OAL_PTR_NULL || pst_wlan_pm->pst_bus == NULL) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_dev get non bus!\n");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_PM_WKUP_NON_BUS);
        return OAL_FAIL;
    }

    if (pst_wlan_pm->ul_wlan_pm_enable == OAL_FALSE) {
        if (pst_wlan_pm->ul_wlan_dev_state == HOST_ALLOW_TO_SLEEP) {
            /* 唤醒流程没走完不允许发送数据 */
            return OAL_ERR_CODE_WAKEUP_PROCESSING;
        } else {
            return OAL_SUCC;
        }
    }

    if (pst_wlan_pm->ul_wlan_dev_state == HOST_DISALLOW_TO_SLEEP) {
        return OAL_SUCC;
    }

    return OAL_SUCC_GO_ON;
}

/*
 * 函 数 名  : wlan_pm_wakeup_dev
 * 功能描述  : 唤醒device
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint wlan_pm_wakeup_dev(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_uint32 ret;
    ktime_t time_start, time_stop;
    oal_uint64 trans_us;
    int i;

    ret = wlan_pm_wakeup_check(pst_wlan_pm);
    if (ret != OAL_SUCC_GO_ON) {
        return ret;
    }

    oal_wake_unlock_force(&pst_wlan_pm->st_deepsleep_wakelock);

    time_start = ktime_get();
    hcc_bus_wake_lock(pst_wlan_pm->pst_bus);

    for (i = 0; i <= WLAN_WAKEUP_FAIL_MAX_TIMES; i++) {
        ret = wlan_pm_wakeup_cmd_send(pst_wlan_pm);
        if (ret != OAL_SUCC) {
            if (ret == OAL_ERR_CODE_WAKEUP_FAIL_PROC) {
                break;
            } else {
                hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
                return ret;
            }
        }

        ret = wlan_pm_wakeup_wait_ack(pst_wlan_pm);
        if (ret == OAL_SUCC || ret == OAL_ERR_CODE_WAKEUP_FAIL_PROC) {
            break;
        }
    }

    if (ret != OAL_SUCC) {
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
        wlan_pm_wakeup_fail_proc(pst_wlan_pm);
        return OAL_FAIL;
    }

    wlan_pm_feed_wdg();

    wlan_pm_wakeup_notify(pst_wlan_pm);

    time_stop = ktime_get();
    trans_us = (oal_uint64)ktime_to_us(ktime_sub(time_stop, time_start));
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wakeup dev succ, cost %llu us", trans_us);
    return OAL_SUCC;
}

oal_void wlan_pm_wakeup_dev_ack(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = NULL;

    if (oal_atomic_read(&g_wakeup_dev_wait_ack)) {
        pst_wlan_pm = wlan_pm_get_drv();
        if (pst_wlan_pm == OAL_PTR_NULL) {
            return;
        }

        pst_wlan_pm->ul_wakeup_dev_ack++;

        oal_complete(&pst_wlan_pm->st_wakeup_done);

        oal_atomic_set(&g_wakeup_dev_wait_ack, 0);
    }

    return;
}

/*
 * 函 数 名  : wlan_pm_open_bcpu
 * 功能描述  : 唤醒BCPU
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint wlan_pm_open_bcpu(oal_void)
{
#define RETRY_TIMES 3
    oal_uint32 i;
    oal_int32 ret = OAL_FAIL;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_int32 ul_ret;

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    /* 通过sdio配置命令，解复位BCPU */
    oam_warning_log0(0, OAM_SF_PWR, "open BCPU");

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    for (i = 0; i < RETRY_TIMES; i++) {
        ret = wlan_pm_wakeup_dev();
        if (ret == OAL_SUCC) {
            break;
        }
    }

    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_dev fail!");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }

    oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_wakeup_dev succ, retry times [%d]", i);

    oal_init_completion(&pst_wlan_pm->st_open_bcpu_done);

    ret = hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_RESET_BCPU);
    if (ret == OAL_SUCC) {
        /* 等待device执行命令 */
        up(&pst_wlan_pm->pst_bus->rx_sema);
        ul_ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_open_bcpu_done,
                                                 (oal_uint32)oal_msecs_to_jiffies(WLAN_OPEN_BCPU_WAIT_TIMEOUT));
        if (ul_ret == 0) {
            oam_error_log0(0, OAM_SF_PWR, "wlan_pm_open_bcpu wait device ACK timeout !");
            hcc_tx_transfer_unlock(hcc_get_110x_handler());
            return OAL_FAIL;
        }

        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_SUCC;
    } else {
        oam_error_log1(0, OAM_SF_PWR, "fail to send H2D_MSG_RESET_BCPU, ret=%d", ret);
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }
}

/*
 * 函 数 名  : wlan_pm_wakeup_host
 * 功能描述  : device唤醒host
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_uint wlan_pm_wakeup_host(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (oal_warn_on(pst_wlan_pm == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_host: st_wlan_pm is null \n");
        return -OAL_FAIL;
    }

    hcc_bus_wake_lock(pst_wlan_pm->pst_bus);
    wlan_pm_idle_sleep_vote(DISALLOW_IDLESLEEP);
    oam_info_log1(0, OAM_SF_PWR, "wlan_pm_wakeup_host get wakelock %lu!\n",
                  pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    if (wlan_pm_work_submit(pst_wlan_pm, &pst_wlan_pm->st_wakeup_work) != 0) {
        pst_wlan_pm->ul_wakeup_fail_submit_work++;
        hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
    } else {
        pst_wlan_pm->ul_wakeup_succ_work_submit++;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_work_submit
 * 功能描述  : 提交一个kernel work
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_int wlan_pm_work_submit(struct wlan_pm_s *pst_wlan_pm, oal_work_stru *pst_worker)
{
    oal_int i_ret = 0;

    if (oal_work_is_busy(pst_worker)) {
        /* If comm worker is processing,
          we need't submit again */
        i_ret = -OAL_EBUSY;
        goto done;
    } else {
        oam_info_log1(0, OAM_SF_PWR, "WiFi %lX Worker Submit\n", (uintptr_t)pst_worker->func);
        if (queue_work(pst_wlan_pm->pst_pm_wq, pst_worker) == false) {
            i_ret = -OAL_EFAIL;
        }
    }
done:
    return i_ret;
}

/*
 * 函 数 名  : wlan_pm_wakeup_work
 * 功能描述  : device唤醒host work
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
void wlan_pm_wakeup_work(oal_work_stru *pst_worker)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_uint l_ret;

    oam_info_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_work start!\n");

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    l_ret = wlan_pm_wakeup_dev();
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        declare_dft_trace_key_info("wlan_wakeup_fail", OAL_DFT_TRACE_FAIL);
    }

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    /* match for the work submit */
    hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
    declare_dft_trace_key_info("wlan_d2h_wakeup_succ", OAL_DFT_TRACE_SUCC);
    oam_info_log1(0, OAM_SF_PWR, "wlan_pm_wakeup_work release wakelock %lu!\n",
                  pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    return;
}

/*
 * 函 数 名  : wlan_pm_wakeup_done_callback
 * 功能描述  : device应答wakeup succ消息处理
 */
int32 wlan_pm_wakeup_done_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oam_info_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_done_callback !");

    pst_wlan_pm->ul_wakeup_done_callback++;

    wlan_pm_wakeup_dev_ack();

    return SUCCESS;
}

/*
 * 函 数 名  : wlan_pm_close_done_callback
 * 功能描述  : device应答poweroff ack消息处理
 */
int32 wlan_pm_close_done_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_close_done_callback !");

    /* 关闭RX通道，防止SDIO RX thread继续访问SDIO */
    hcc_bus_disable_state(hcc_get_current_110x_bus(), OAL_BUS_STATE_RX);

    pst_wlan_pm->ul_close_done_callback++;
    oal_complete(&pst_wlan_pm->st_close_done);

    oam_warning_log0(0, OAM_SF_PWR, "complete H2D_MSG_PM_WLAN_OFF done!");

    return SUCCESS;
}

/*
 * 函 数 名  : wlan_pm_open_bcpu_done_callback
 * 功能描述  : device应答open bcpu ack消息处理
 */
int32 wlan_pm_open_bcpu_done_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_open_bcpu_done_callback !");

    pst_wlan_pm->ul_open_bcpu_done_callback++;
    oal_complete(&pst_wlan_pm->st_open_bcpu_done);

    oam_warning_log0(0, OAM_SF_PWR, "complete H2D_MSG_RESET_BCPU done!");

    return SUCCESS;
}

/*
 * 函 数 名  : wlan_pm_close_bcpu_done_callback
 * 功能描述  : device应答open bcpu ack消息处理
 */
int32 wlan_pm_close_bcpu_done_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_close_bcpu_done_callback !");

    pst_wlan_pm->ul_close_bcpu_done_callback++;
    oal_complete(&pst_wlan_pm->st_close_bcpu_done);

    oam_warning_log0(0, OAM_SF_PWR, "complete H2D_MSG_PM_BCPU_OFF done!");

    return SUCCESS;
}

/*
 * 函 数 名  : wlan_pm_halt_bcpu_done_callback
 * 功能描述  : device应答open bcpu ack消息处理
 */
int32 wlan_pm_halt_bcpu_done_callback(void *data)
{
    struct wlan_pm_s *pst_wlan_pm = (struct wlan_pm_s *)data;

    oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_halt_bcpu_done_callback !");

    oal_complete(&pst_wlan_pm->st_halt_bcpu_done);

    oam_warning_log0(0, OAM_SF_PWR, "complete wlan_pm_halt_bcpu_done_callback done!");

    return SUCCESS;
}

OAL_STATIC oal_void sleep_request_host_forbid_print(struct wlan_pm_s *pst_wlan_pm,
                                                    const oal_uint32 host_forbid_sleep_limit)
{
    if (pst_wlan_pm->ul_sleep_request_host_forbid >= host_forbid_sleep_limit) {
        /* 防止频繁打印 */
        if (oal_print_rate_limit(10 * PRINT_RATE_SECOND)) { /* 10s打印一次 */
            oal_int32 allow_print;
            oam_warning_log2(0, OAM_SF_PWR, "wlan_pm_sleep_work host forbid sleep %ld, forbid_cnt:%u",
                             pst_wlan_pm->ul_sleep_stage, pst_wlan_pm->ul_sleep_request_host_forbid);
            allow_print = oal_print_rate_limit(10 * PRINT_RATE_MINUTE); /* 10分钟打印一次 */
            hcc_bus_print_trans_info(pst_wlan_pm->pst_bus,
                                     allow_print ?
                                     (HCC_PRINT_TRANS_FLAG_DEVICE_STAT | HCC_PRINT_TRANS_FLAG_DEVICE_REGS) : 0x0);
        }
    } else {
        /* 防止频繁打印 */
        if (oal_print_rate_limit(10 * PRINT_RATE_SECOND)) { /* 10s打印一次 */
            oam_warning_log2(0, OAM_SF_PWR, "wlan_pm_sleep_work host forbid sleep %ld, forbid_cnt:%u",
                             pst_wlan_pm->ul_sleep_stage, pst_wlan_pm->ul_sleep_request_host_forbid);
        }
    }
}

OAL_STATIC oal_void trigger_bus_execp_check(struct wlan_pm_s *pst_wlan_pm)
{
    if (hi11xx_get_os_build_variant() == HI1XX_OS_BUILD_VARIANT_ROOT) {
        if (oal_trigger_bus_exception(pst_wlan_pm->pst_bus, OAL_TRUE) == OAL_TRUE) {
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "tigger dump device mem for device_forbid_sleep %d second",
                                 WLAN_SLEEP_FORBID_CHECK_TIME / 1000);  // 1000 ms
        }
    }
}

static oal_uint64 g_old_tx, g_old_rx;
static oal_uint64 g_new_tx, g_new_rx;
/*
 * 函 数 名  : wlan_pm_sleep_forbid_debug
 * 功能      : 获取HCC层的报文统计数据
*/
OAL_STATIC void wlan_pm_sleep_forbid_debug(struct wlan_pm_s *pst_wlan_pm)
{
    pst_wlan_pm->ul_sleep_fail_forbid++;
    if (pst_wlan_pm->ul_sleep_fail_forbid == 1) {
        pst_wlan_pm->ul_sleep_forbid_check_time = jiffies + msecs_to_jiffies(WLAN_SLEEP_FORBID_CHECK_TIME);
    } else if ((pst_wlan_pm->ul_sleep_fail_forbid != 0) &&
               (time_after(jiffies, pst_wlan_pm->ul_sleep_forbid_check_time))) {
        /* 暂时连续2分钟被forbid sleep，上报一次CHR，看大数据再决定做不做DFR */
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_SLEEP_FORBID);
        pst_wlan_pm->ul_sleep_fail_forbid = 0;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "device_forbid_sleep for %d second",
                             WLAN_SLEEP_FORBID_CHECK_TIME / 1000);  // 1000 ms
        trigger_bus_execp_check(pst_wlan_pm);
    }

    pst_wlan_pm->ul_sleep_fail_forbid_cnt++;
    if (pst_wlan_pm->ul_sleep_fail_forbid_cnt <= 1) {
        /* get hcc trans count */
        hcc_bus_get_trans_count(pst_wlan_pm->pst_bus, &g_old_tx, &g_old_rx);
    } else {
        /* ul_sleep_fail_forbid_cnt > 1 */
        hcc_bus_get_trans_count(pst_wlan_pm->pst_bus, &g_new_tx, &g_new_rx);
        /* trans pending */
        if (pst_wlan_pm->ul_sleep_fail_forbid_cnt >= g_wlan_slp_req_forbid_limit) {
            /* maybe device memleak */
            declare_dft_trace_key_info("wlan_forbid_sleep_print_info", OAL_DFT_TRACE_SUCC);
            oam_warning_log2(0, OAM_SF_PWR,
                "wlan_pm_sleep_work device forbid sleep %ld, forbid_cnt:%u try dump device mem info",
                pst_wlan_pm->ul_sleep_stage, pst_wlan_pm->ul_sleep_fail_forbid_cnt);
            oam_warning_log4(0, OAM_SF_PWR, "old[tx:%u rx:%u] new[tx:%u rx:%u]",
                             g_old_tx, g_old_rx, g_new_tx, g_new_rx);
            pst_wlan_pm->ul_sleep_fail_forbid_cnt = 0;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            hcc_print_current_trans_info(1);
#endif
            hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_DEVICE_MEM_DUMP);
        } else if ((pst_wlan_pm->ul_sleep_fail_forbid_cnt % (g_wlan_slp_req_forbid_limit / 10)) == 0) { // 打印10次
            oal_print_hi11xx_log(HI11XX_LOG_INFO,
                "sleep request too many forbid %ld, device busy, forbid_cnt:%u, old[tx:%u rx:%u] new[tx:%u rx:%u]",
                pst_wlan_pm->ul_sleep_stage, pst_wlan_pm->ul_sleep_fail_forbid_cnt,
                (oal_uint32)g_old_tx, (oal_uint32)g_old_rx, (oal_uint32)g_new_tx, (oal_uint32)g_new_rx);
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_DBG,
                "sleep request forbid %ld, device busy, forbid_cnt:%u, old[tx:%u rx:%u] new[tx:%u rx:%u]",
                pst_wlan_pm->ul_sleep_stage, pst_wlan_pm->ul_sleep_fail_forbid_cnt,
                (oal_uint32)g_old_tx, (oal_uint32)g_old_rx, (oal_uint32)g_new_tx, (oal_uint32)g_new_rx);
        }
    }

    return;
}

/*
 * 函 数 名  : wlan_pm_sleep_cmd_send
 * 功能描述  : 发送 sleep cmd msg
 */
OAL_STATIC int32 wlan_pm_sleep_cmd_send(struct wlan_pm_s *pst_wlan_pm)
{
    oal_int32 l_ret;
    oal_uint8 uc_retry;

    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_FALSE);

    pst_wlan_pm->ul_wlan_dev_state = HOST_ALLOW_TO_SLEEP;

    l_ret = hcc_bus_sleep_request(pst_wlan_pm->pst_bus);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wifi sleep cmd send,pkt_num:[%d],wakelock cnt %lu",
                         pst_wlan_pm->ul_packet_total_cnt, pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    if (l_ret != OAL_SUCC) {
        for (uc_retry = 0; uc_retry < WLAN_SDIO_MSG_RETRY_NUM; uc_retry++) {
            oal_msleep(SLEEP_10_MSEC);
            l_ret = hcc_bus_sleep_request(pst_wlan_pm->pst_bus);
            if (l_ret == OAL_SUCC) {
                break;
            }
            oam_error_log2(0, OAM_SF_PWR, "sleep_dev retry %d ret = %d", uc_retry, l_ret);
        }

        /* after max retry still fail,log error */
        if (l_ret != OAL_SUCC) {
            pst_wlan_pm->ul_sleep_fail_set_reg++;
            declare_dft_trace_key_info("wlan_sleep_cmd_fail", OAL_DFT_TRACE_FAIL);
            oam_error_log1(0, OAM_SF_PWR, "sleep_dev Fail ret = %d\r\n", l_ret);
            pst_wlan_pm->ul_wlan_dev_state = HOST_DISALLOW_TO_SLEEP;
            oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);
            return OAL_FAIL;
        }
    }

    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_wlan_pm->pst_bus), OAL_TRUE);

    pst_wlan_pm->ul_sleep_fail_forbid_cnt = 0;
    pst_wlan_pm->ul_sleep_fail_forbid = 0;

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_sleep_work_check
 * 功能: 检查是否可以睡眠
 */
OAL_STATIC oal_uint32  wlan_pm_sleep_check(struct wlan_pm_s *pst_wlan_pm)
{
    oal_bool_enum_uint8 en_wifi_pause_pm = OAL_FALSE;
    if (pst_wlan_pm->ul_wlan_pm_enable == OAL_FALSE) {
        wlan_pm_feed_wdg();
        return OAL_FAIL;
    }

    /* 协议栈回调获取是否pause低功耗 */
    if (pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_get_pm_pause_func) {
        en_wifi_pause_pm = pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_get_pm_pause_func();
    }

    if (en_wifi_pause_pm == OAL_TRUE) {
        wlan_pm_feed_wdg();
        return OAL_FAIL;
    }

    if (pst_wlan_pm->ul_wlan_dev_state == HOST_ALLOW_TO_SLEEP) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "wakeuped,need not do again");
        wlan_pm_feed_wdg();
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_sleep_fail_proc
 * 功能: 睡眠失败处理
 */
OAL_STATIC oal_void  wlan_pm_sleep_fail_proc(struct wlan_pm_s *pst_wlan_pm)
{
    pst_wlan_pm->fail_sleep_count++;
    wlan_pm_feed_wdg();

    /* 失败超出门限，启动dfr流程 */
    if (pst_wlan_pm->fail_sleep_count > WLAN_WAKEUP_FAIL_MAX_TIMES) {
        oam_error_log1(0, OAM_SF_PWR, "Now ready to enter DFR process after [%d]times wlan_sleep_fail!",
                       pst_wlan_pm->fail_sleep_count);
        pst_wlan_pm->fail_sleep_count = 0;
        wlan_pm_stop_wdg(pst_wlan_pm);
        hcc_bus_exception_submit(pst_wlan_pm->pst_bus, WIFI_WAKEUP_FAIL);
    }
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_PM, CHR_PLAT_DRV_ERROR_WIFI_SLEEP_REQ);
}

/*
 * 函 数 名  : wlan_pm_sleep_forbid_proc
 * 功能: 睡眠失败处理
 */
OAL_STATIC oal_void  wlan_pm_sleep_forbid_proc(struct wlan_pm_s *pst_wlan_pm)
{
    pst_wlan_pm->fail_sleep_count = 0;
    wlan_pm_feed_wdg();
    return;
}

/*
 * 函 数 名  : wlan_pm_sleep_notify
 * 功能: 睡眠成功通知周边模块
 */
OAL_STATIC oal_void  wlan_pm_sleep_notify(struct wlan_pm_s *pst_wlan_pm)
{
    /* HOST WIFI进入低功耗,通知业务侧关闭定时器 */
    if (pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_pm_state_notify != OAL_PTR_NULL) {
        pst_wlan_pm->st_wifi_srv_handler.p_wifi_srv_pm_state_notify(OAL_FALSE);
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    raw_notifier_call_chain(&wifi_pm_chain, WIFI_PM_SLEEP_EVENT, (oal_void *)pst_wlan_pm); /* sleep chain */
#endif
    return;
}


/*
 * 函 数 名  : wlan_pm_sleep_cmd_send
 * 功能: 检查是否可以睡眠
 */
OAL_STATIC oal_uint32  wlan_pm_sleep_cmd_proc(struct wlan_pm_s *pst_wlan_pm)
{
    oal_uint32 ret;

    if (pst_wlan_pm->ul_sleep_stage == SLEEP_ALLOW_RCV) {
        /* check host */
        ret = wlan_pm_sleep_cmd_send(pst_wlan_pm);
        if (ret == OAL_FAIL) {
            return OAL_ERR_CODE_SLEEP_FAIL;
        }
    } else {
        wlan_pm_sleep_forbid_debug(pst_wlan_pm);
        declare_dft_trace_key_info("wlan_forbid_sleep", OAL_DFT_TRACE_SUCC);
        return OAL_ERR_CODE_SLEEP_FORBID;
    }

    pst_wlan_pm->ul_sleep_stage = SLEEP_CMD_SND;

    /* 继续持锁500ms, 防止系统频繁进入退出PM */
    oal_wake_lock_timeout(&pst_wlan_pm->st_deepsleep_wakelock, WLAN_WAKELOCK_HOLD_TIME);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_sleep_work hold deepsleep_wakelock %d ms",
                         WLAN_WAKELOCK_HOLD_TIME);

    pst_wlan_pm->ul_sleep_succ++;
    pst_wlan_pm->fail_sleep_count = 0;

    wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
    hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
    if (pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count != 0) {
        oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_sleep_work release wakelock %lu!",
                         pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);
    }

    declare_dft_trace_key_info("wlan_sleep_ok", OAL_DFT_TRACE_SUCC);
    return OAL_SUCC;
}

static uint32_t wlan_pm_sleep_request_host(struct wlan_pm_s *pst_wlan_pm)
{
    const oal_uint32 host_forbid_sleep_limit = 10;
    int32_t ret = hcc_bus_sleep_request_host(pst_wlan_pm->pst_bus);
    if (ret != OAL_SUCC) {
        // pst_wlan_pm->ul_sleep_fail_forbid++;
        pst_wlan_pm->ul_sleep_request_host_forbid++;
        declare_dft_trace_key_info("wlan_forbid_sleep_host", OAL_DFT_TRACE_SUCC);
        sleep_request_host_forbid_print(pst_wlan_pm, host_forbid_sleep_limit);
        return OAL_ERR_CODE_SLEEP_FORBID;
    } else {
        pst_wlan_pm->ul_sleep_request_host_forbid = 0;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_sleep_work
 * 返 回 值  : 成功或失败原因
 */
void wlan_pm_sleep_work(oal_work_stru *pst_worker)
{
#define SLEEP_DELAY_THRESH      0

    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_int32 ret;

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    if (wlan_pm_sleep_check(pst_wlan_pm) != OAL_SUCC) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return;
    }

    // check host
    ret = wlan_pm_sleep_request_host(pst_wlan_pm);
    if ( ret != OAL_SUCC) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        wlan_pm_sleep_forbid_proc(pst_wlan_pm);
        return;
    }

    ret = wlan_pm_sleep_request(pst_wlan_pm);
    if (ret == OAL_ERR_CODE_SLEEP_FAIL) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        wlan_pm_sleep_fail_proc(pst_wlan_pm);
        return;
    }

    ret = wlan_pm_sleep_cmd_proc(pst_wlan_pm);
    if (ret == OAL_ERR_CODE_SLEEP_FAIL) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        wlan_pm_sleep_fail_proc(pst_wlan_pm);
        return;
    } else if (ret == OAL_ERR_CODE_SLEEP_FORBID) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        wlan_pm_sleep_forbid_proc(pst_wlan_pm);
        return;
    }

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    wlan_pm_sleep_notify(pst_wlan_pm);

    return;
}

void wlan_pm_freq_adjust_work(oal_work_stru *pst_worker)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    if (pst_wlan_pm->ul_wlan_pm_enable == OAL_FALSE) {
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return;
    }

    hcc_tx_transfer_unlock(hcc_get_110x_handler());
}

/*
 * 函 数 名  : wlan_pm_state_get
 * 功能描述  : 获取pm的sleep状态
 * 返 回 值  : 1:allow to sleep; 0:disallow to sleep
 */
oal_uint wlan_pm_state_get(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    return pst_wlan_pm->ul_wlan_dev_state;
}

/*
 * 函 数 名  : wlan_pm_state_set
 * 功能描述  : 获取pm的sleep状态
 * 返 回 值  : 1:allow to sleep; 0:disallow to sleep
 */
oal_void wlan_pm_state_set(struct wlan_pm_s *pst_wlan_pm, oal_uint ul_state)
{
    pst_wlan_pm->ul_wlan_dev_state = ul_state;
}

/*
 * 函 数 名  : wlan_pm_set_timeout
 * 功能描述  : 睡眠定时器超时时间设置
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_void wlan_pm_set_timeout(oal_uint32 ul_timeout)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return;
    }

    oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_set_timeout[%d]", ul_timeout);

    pst_wlan_pm->ul_wdg_timeout_cnt = ul_timeout;

    pst_wlan_pm->ul_wdg_timeout_curr_cnt = 0;

    pst_wlan_pm->ul_packet_cnt = 0;

    wlan_pm_feed_wdg();
}
EXPORT_SYMBOL_GPL(wlan_pm_set_timeout);
/*
 * 函 数 名  : wlan_pm_feed_wdg
 * 功能描述  : 启动睡眠定时器
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
oal_void wlan_pm_feed_wdg(oal_void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    pst_wlan_pm->ul_sleep_feed_wdg_cnt++;

    if (g_download_rate_limit_pps != 0) {
        mod_timer(&pst_wlan_pm->st_watchdog_timer, jiffies + msecs_to_jiffies(SLEEP_10_MSEC));
    } else {
        mod_timer(&pst_wlan_pm->st_watchdog_timer, jiffies + msecs_to_jiffies(WLAN_SLEEP_TIMER_PERIOD));
    }
}

/*
 * 函 数 名  : wlan_pm_stop_wdg
 * 功能描述  : 停止50ms睡眠定时器
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
int32 wlan_pm_stop_wdg(struct wlan_pm_s *pst_wlan_pm_info)
{
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_stop_wdg");

    pst_wlan_pm_info->ul_wdg_timeout_curr_cnt = 0;
    pst_wlan_pm_info->ul_packet_cnt = 0;

    if (in_interrupt()) {
        return del_timer(&pst_wlan_pm_info->st_watchdog_timer);
    } else {
        return del_timer_sync(&pst_wlan_pm_info->st_watchdog_timer);
    }
}

static int wlan_pm_submit_sleep_work(struct wlan_pm_s *pm_data)
{
    if (pm_data->ul_packet_cnt == 0) {
        pm_data->ul_wdg_timeout_curr_cnt++;
        if ((pm_data->ul_wdg_timeout_curr_cnt >= pm_data->ul_wdg_timeout_cnt)) {
            if (wlan_pm_work_submit(pm_data, &pm_data->st_sleep_work) == 0) {
                /* 提交了sleep work后，定时器不重启，避免重复提交sleep work */
                pm_data->ul_sleep_work_submit++;
                pm_data->ul_wdg_timeout_curr_cnt = 0;
                return OAL_SUCC;
            }
            oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_sleep_work submit fail,work is running !\n");
        }
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "plat:wlan_pm_wdg_timeout %d have packet %d....",
                             pm_data->ul_wdg_timeout_curr_cnt, pm_data->ul_packet_cnt);
        pm_data->ul_wdg_timeout_curr_cnt = 0;
        pm_data->ul_packet_cnt = 0;

        /* 有报文收发,连续forbid sleep次数清零 */
        pm_data->ul_sleep_fail_forbid = 0;
    }
    return -OAL_EFAIL;
}

/*
 * 函 数 名  : wlan_pm_wdg_timeout
 * 功能描述  : 50ms睡眠定时器超时处理，提交一个sleep work
 * 返 回 值  : 初始化返回值，成功或失败原因
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0))
void wlan_pm_wdg_timeout(unsigned long data)
#else
void wlan_pm_wdg_timeout(struct timer_list *t)
#endif
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0))
    struct wlan_pm_s *pm_data = (struct wlan_pm_s *)(uintptr_t)data;
#else
    struct wlan_pm_s *pm_data = from_timer(pm_data, t, st_watchdog_timer);
#endif

    if (pm_data == NULL) {
        return;
    }
    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_pm_wdg_timeout....%d", pm_data->ul_wdg_timeout_curr_cnt);

    /* hcc bus switch process */
    hcc_bus_performance_core_schedule(HCC_CHIP_110X_DEV);

    pm_data->ul_packet_cnt += pm_wifi_rxtx_count;  // 和hmac中统计收发包分离

    pm_data->ul_packet_total_cnt += pm_wifi_rxtx_count;
    if (time_after(jiffies, pm_data->ul_packet_check_time)) {
#ifdef _PRE_UART_PRINT_LOG
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pkt_num:WIFI[%d]", pm_data->ul_packet_total_cnt);
#else
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "pkt_num:WIFI[%d]", pm_data->ul_packet_total_cnt);
#endif
        pm_data->ul_packet_check_time = jiffies + msecs_to_jiffies(WLAN_PACKET_CHECK_TIME);
    }

    pm_wifi_rxtx_count = 0;

    /* 低功耗关闭时timer不会停 */
    if (pm_data->ul_wlan_pm_enable) {
        if (wlan_pm_submit_sleep_work(pm_data) == OAL_SUCC) {
            return;
        }
    } else {
        pm_data->ul_packet_cnt = 0;
    }

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_pm_feed_wdg");
    wlan_pm_feed_wdg();

    return;
}


/*
 * 函 数 名  : wlan_pm_poweroff_cmd
 * 功能描述  : 发消息到device，wifi device关闭wifi系统资源，等待bcpu给它下电
 */
oal_int32 wlan_pm_poweroff_cmd(oal_void)
{
#define ACK_CHECK_MAX_CNT    10
#define ACK_CHECK_WAIT_TIME  100

    oal_int32 ret;
    oal_uint32 i;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    oam_warning_log0(0, OAM_SF_PWR, "Send H2D_MSG_PM_WLAN_OFF cmd");

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    if (wlan_pm_wakeup_dev() != OAL_SUCC) {
        (void)ssi_dump_err_regs(SSI_ERR_WLAN_POWEROFF_FAIL);
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }

    ret = hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_PM_WLAN_OFF);
    if (ret == OAL_SUCC) {
        ret = OAL_FAIL;
        for (i = 0; i < ACK_CHECK_MAX_CNT; i++) {
            msleep(ACK_CHECK_WAIT_TIME);
            if (board_get_wlan_wkup_gpio_val() == 1) {
                oam_warning_log0(0, OAM_SF_PWR, "wlan_pm_poweroff_cmd  wait device ACK SUCC");
                ret = OAL_SUCC;
                break;
            }
        }

        if (ret == OAL_FAIL) {
            oam_error_log1(0, OAM_SF_PWR, "wlan_pm_poweroff_cmd  wait device ACK timeout && GPIO_LEVEL[%d] !",
                           board_get_wlan_wkup_gpio_val());
            (void)ssi_dump_err_regs(SSI_ERR_WLAN_POWEROFF_FAIL);
            hcc_tx_transfer_unlock(hcc_get_110x_handler());
            return OAL_FAIL;
        }
    } else {
        oam_error_log0(0, OAM_SF_PWR, "fail to send H2D_MSG_PM_WLAN_OFF");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    return OAL_SUCC;
}

void wlan_pm_wkup_src_debug_set(oal_uint32 ul_en)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return;
    }
    pst_wlan_pm->ul_wkup_src_print_en = ul_en;
}

EXPORT_SYMBOL_GPL(wlan_pm_wkup_src_debug_set);

oal_uint32 wlan_pm_wkup_src_debug_get(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if (pst_wlan_pm == OAL_PTR_NULL) {
        return OAL_FALSE;
    }
    return pst_wlan_pm->ul_wkup_src_print_en;
}
EXPORT_SYMBOL_GPL(wlan_pm_wkup_src_debug_get);

/*
 * 函 数 名  : wlan_pm_shutdown_bcpu_cmd
 * 功能描述  : 发消息到device，wifi device关闭BCPU
 */
oal_int32 wlan_pm_shutdown_bcpu_cmd(oal_void)
{
#define RETRY_TIMES 3
    oal_uint32 i;
    oal_int32 ret = OAL_FAIL;
    oal_uint32 ul_ret;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    oam_warning_log0(0, OAM_SF_PWR, "Send H2D_MSG_PM_BCPU_OFF cmd");

    hcc_tx_transfer_lock(hcc_get_110x_handler());

    for (i = 0; i < RETRY_TIMES; i++) {
        ret = wlan_pm_wakeup_dev();
        if (ret == OAL_SUCC) {
            break;
        }
    }

    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_PWR, "wlan_pm_wakeup_dev fail!");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }

    oam_warning_log1(0, OAM_SF_PWR, "wlan_pm_wakeup_dev succ, retry times [%d]", i);

    oal_init_completion(&pst_wlan_pm->st_close_bcpu_done);

    ret = hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_PM_BCPU_OFF);
    if (ret == OAL_SUCC) {
        /* 等待device执行命令 */
        ul_ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_close_bcpu_done,
                                                 (oal_uint32)oal_msecs_to_jiffies(WLAN_POWEROFF_ACK_WAIT_TIMEOUT));
        if (ul_ret == 0) {
            oam_error_log0(0, OAM_SF_PWR, "wlan_pm_shutdown_bcpu_cmd wait device ACK timeout !");
            hcc_tx_transfer_unlock(hcc_get_110x_handler());
            return OAL_FAIL;
        }
    } else {
        oam_error_log0(0, OAM_SF_PWR, "fail to send H2D_MSG_PM_BCPU_OFF");
        hcc_tx_transfer_unlock(hcc_get_110x_handler());
        return OAL_FAIL;
    }

    hcc_tx_transfer_unlock(hcc_get_110x_handler());

    return OAL_SUCC;
}

/*
 * 函 数 名  : wlan_pm_dump_info
 * 功能描述  : debug, 发消息到device，串口输出维测信息
 */
void wlan_pm_dump_host_info(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct oal_sdio *pst_sdio = oal_get_sdio_default_handler();

    if (pst_wlan_pm == NULL) {
        return;
    }

    oal_io_print("----------wlan_pm_dump_host_info begin-----------\n");
    oal_io_print("power on:%ld, enable:%ld,g_wlan_host_pm_switch:%d\n",
                 pst_wlan_pm->ul_wlan_power_state, pst_wlan_pm->ul_wlan_pm_enable, g_wlan_host_pm_switch);
    oal_io_print("dev state:%ld, sleep stage:%ld\n", pst_wlan_pm->ul_wlan_dev_state, pst_wlan_pm->ul_sleep_stage);
    oal_io_print("open:%d,close:%d\n", pst_wlan_pm->ul_open_cnt, pst_wlan_pm->ul_close_cnt);
    if (pst_sdio != NULL) {
        oal_io_print("sdio suspend:%d,sdio resume:%d\n", pst_sdio->ul_sdio_suspend, pst_sdio->ul_sdio_resume);
    }
    oal_io_print("gpio_intr[no.%d]:%llu\n",
                 pst_wlan_pm->pst_bus->bus_dev->ul_wlan_irq, pst_wlan_pm->pst_bus->gpio_int_count);
    oal_io_print("data_intr:%llu\n", pst_wlan_pm->pst_bus->data_int_count);
    oal_io_print("wakeup_intr:%llu\n", pst_wlan_pm->pst_bus->wakeup_int_count);
    oal_io_print("D2H_MSG_WAKEUP_SUCC:%d\n", pst_wlan_pm->pst_bus->msg[D2H_MSG_WAKEUP_SUCC].count);
    oal_io_print("D2H_MSG_ALLOW_SLEEP:%d\n", pst_wlan_pm->pst_bus->msg[D2H_MSG_ALLOW_SLEEP].count);
    oal_io_print("D2H_MSG_DISALLOW_SLEEP:%d\n", pst_wlan_pm->pst_bus->msg[D2H_MSG_DISALLOW_SLEEP].count);

    oal_io_print("wakeup_dev_wait_ack:%d\n", oal_atomic_read(&g_wakeup_dev_wait_ack));
    oal_io_print("wakeup_succ:%d\n", pst_wlan_pm->ul_wakeup_succ);
    oal_io_print("wakeup_dev_ack:%d\n", pst_wlan_pm->ul_wakeup_dev_ack);
    oal_io_print("wakeup_done_callback:%d\n", pst_wlan_pm->ul_wakeup_done_callback);
    oal_io_print("wakeup_succ_work_submit:%d\n", pst_wlan_pm->ul_wakeup_succ_work_submit);
    oal_io_print("wakeup_fail_wait_sdio:%d\n", pst_wlan_pm->ul_wakeup_fail_wait_sdio);
    oal_io_print("wakeup_fail_timeout:%d\n", pst_wlan_pm->ul_wakeup_fail_timeout);
    oal_io_print("wakeup_fail_set_reg:%d\n", pst_wlan_pm->ul_wakeup_fail_set_reg);
    oal_io_print("wakeup_fail_submit_work:%d\n", pst_wlan_pm->ul_wakeup_fail_submit_work);
    oal_io_print("sleep_succ:%d\n", pst_wlan_pm->ul_sleep_succ);
    oal_io_print("sleep feed wdg:%d\n", pst_wlan_pm->ul_sleep_feed_wdg_cnt);
    oal_io_print("sleep_fail_request:%d\n", pst_wlan_pm->ul_sleep_fail_request);
    oal_io_print("sleep_fail_set_reg:%d\n", pst_wlan_pm->ul_sleep_fail_set_reg);
    oal_io_print("sleep_fail_wait_timeout:%d\n", pst_wlan_pm->ul_sleep_fail_wait_timeout);
    oal_io_print("sleep_fail_forbid:%d\n", pst_wlan_pm->ul_sleep_fail_forbid);
    oal_io_print("sleep_work_submit:%d\n", pst_wlan_pm->ul_sleep_work_submit);
    oal_io_print("wklock_cnt:%lu\n \n", pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);
    oal_io_print("----------wlan_pm_dump_host_info end-----------\n");
}

void wlan_pm_dump_device_info(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    hcc_bus_send_message(pst_wlan_pm->pst_bus, H2D_MSG_PM_DEBUG);
}

void wlan_pm_info_clean(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    struct oal_sdio *pst_sdio = oal_get_sdio_default_handler();

    pst_wlan_pm->pst_bus->data_int_count = 0;
    pst_wlan_pm->pst_bus->wakeup_int_count = 0;

    pst_wlan_pm->pst_bus->msg[D2H_MSG_WAKEUP_SUCC].count = 0;
    pst_wlan_pm->pst_bus->msg[D2H_MSG_ALLOW_SLEEP].count = 0;
    pst_wlan_pm->pst_bus->msg[D2H_MSG_DISALLOW_SLEEP].count = 0;

    if (pst_sdio != NULL) {
        pst_sdio->ul_sdio_suspend = 0;
        pst_sdio->ul_sdio_resume = 0;
    }

    pst_wlan_pm->ul_wakeup_succ = 0;
    pst_wlan_pm->ul_wakeup_dev_ack = 0;
    pst_wlan_pm->ul_wakeup_done_callback = 0;
    pst_wlan_pm->ul_wakeup_succ_work_submit = 0;
    pst_wlan_pm->ul_wakeup_fail_wait_sdio = 0;
    pst_wlan_pm->ul_wakeup_fail_timeout = 0;
    pst_wlan_pm->ul_wakeup_fail_set_reg = 0;
    pst_wlan_pm->ul_wakeup_fail_submit_work = 0;

    pst_wlan_pm->ul_sleep_succ = 0;
    pst_wlan_pm->ul_sleep_feed_wdg_cnt = 0;
    pst_wlan_pm->ul_wakeup_done_callback = 0;
    pst_wlan_pm->ul_sleep_fail_set_reg = 0;
    pst_wlan_pm->ul_sleep_fail_wait_timeout = 0;
    pst_wlan_pm->ul_sleep_fail_forbid = 0;
    pst_wlan_pm->ul_sleep_work_submit = 0;

    return;
}

oal_void wlan_pm_debug_sleep(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if ((pst_wlan_pm != NULL) && (pst_wlan_pm->pst_bus != NULL)) {
        hcc_bus_sleep_request(pst_wlan_pm->pst_bus);

        pst_wlan_pm->ul_wlan_dev_state = HOST_ALLOW_TO_SLEEP;
    }

    return;
}

oal_void wlan_pm_debug_wakeup(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    if ((pst_wlan_pm != NULL) && (pst_wlan_pm->pst_bus != NULL)) {
        hcc_bus_wakeup_request(pst_wlan_pm->pst_bus);

        pst_wlan_pm->ul_wlan_dev_state = HOST_DISALLOW_TO_SLEEP;
    }

    return;
}

oal_void wlan_pm_debug_wake_lock(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    hcc_bus_wake_lock(pst_wlan_pm->pst_bus);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_debug_wake_lock:wklock_cnt = %lu",
                         pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    return;
}

oal_void wlan_pm_debug_wake_unlock(void)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    hcc_bus_wake_unlock(pst_wlan_pm->pst_bus);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan_pm_debug_wake_unlock:wklock_cnt = %lu",
                         pst_wlan_pm->pst_bus->st_bus_wakelock.lock_count);

    return;
}
