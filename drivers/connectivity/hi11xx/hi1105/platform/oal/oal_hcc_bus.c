

/*lint -e801*/
#define HISI_LOG_TAG               "[HCC_BUS]"
#define HI11XX_LOG_MODULE_NAME     "[HCC_BUS]"
#define HI11XX_LOG_MODULE_NAME_VAR hcc_bus_loglevel
/* 头文件包含 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#include "plat_exception_rst.h"
#endif
#include "oal_hcc_bus.h"
#include "oal_sdio_host_if.h"
#include "oal_pcie_linux.h"
#include "oal_hcc_host_if.h"
#include "oam_ext_if.h"
#include "plat_pm_wlan.h"
#include "plat_firmware.h"
#include "securec.h"
#if defined(CONFIG_LOG_EXCEPTION) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <log/log_usertype.h>
#endif
#include "securec.h"
#ifndef WIN32
#include "plat_pm.h"
#include "bfgx_exception_rst.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_HCC_BUS_C

/* 全局变量定义 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static int32_t hcc_bus_pm_state_switch_event(struct notifier_block *unused,
                                             unsigned long wifi_state, void *ptr);
OAL_STATIC oal_dlist_create_head(g_hcc_bus_res_hdr);
OAL_STATIC oal_define_spinlock(g_hcc_bus_res_lock);
OAL_STATIC oal_dlist_create_head(g_hcc_bus_switch_hdr);

static struct notifier_block g_hcc_bus_pm_notifier = {
    .notifier_call = hcc_bus_pm_state_switch_event,
};

/*
 * Set Format, split with ",", like below
 * "sdio,pcie,switchon" -- board support sdio & pcie, enable dynamic switch
 * "sdio,pcie,switchoff"
 * "pcie,sdio"          -- board support pcie & sdio, default we use pcie
 * "sdio"               -- only support sdio
 * "pcie"               -- only support pcie
 */
OAL_STATIC char *g_hisi_wifi_bus_select = "";
uint32_t g_hcc_exception_enable = 1;
oal_debug_module_param(g_hcc_exception_enable, uint, S_IRUGO | S_IWUSR);
OAL_STATIC uint32_t g_hcc_bus_switch_bypass = 0;
OAL_STATIC uint32_t g_hcc_bus_switch_test_delay_time = 0;
OAL_STATIC uint32_t g_hcc_bus_switch_test_break = 0;
OAL_STATIC uint32_t g_hcc_bus_auto_switch = 0;
OAL_STATIC uint32_t g_hcc_bus_auto_bindcpu = 0;
OAL_STATIC uint32_t g_hcc_bus_manual_bindcpu = 1;

/* pps 按照大包计算 */
OAL_STATIC uint32_t g_hcc_bus_auto_bindcpu_limit = OAL_BUS_BINDCPU_LIMIT;
OAL_STATIC uint32_t g_hcc_bus_current_pps = 0x0;
OAL_STATIC uint32_t g_hcc_bus_wakelock_debug = 0x0;
OAL_STATIC uint32_t g_hcc_bus_auto_switch_limit = OAL_BUS_SWITCH_LIMIT;
OAL_STATIC char g_str_ini_hcc_bus_switch[OAL_BUS_INI_STR_LEN] = {0};
#else
oal_dlist_head_stru g_hcc_bus_res_hdr;
oal_spin_lock_stru g_hcc_bus_res_lock;
oal_dlist_head_stru g_hcc_bus_switch_hdr;
char *g_hisi_wifi_bus_select = "";
uint32_t g_hcc_exception_enable = 1;
uint32_t g_hcc_bus_switch_bypass = 0;
uint32_t g_hcc_bus_switch_test_delay_time = 0;
uint32_t g_hcc_bus_switch_test_break = 0;
char g_str_ini_hcc_bus_switch[OAL_BUS_INI_STR_LEN];
uint32_t g_hcc_bus_auto_switch = 0;
uint32_t g_hcc_bus_auto_bindcpu = 0;
uint32_t g_hcc_bus_manual_bindcpu = 1;
uint32_t g_hcc_bus_current_pps = 0x0;
uint32_t g_hcc_bus_wakelock_debug = 0x0;
/* pps 按照大包计算 */
uint32_t g_hcc_bus_auto_switch_limit = OAL_BUS_SWITCH_LIMIT;
#endif

#ifdef _PRE_SDIO_TAE_DEBUG
int32_t g_sdio_tae_debug_enable = 0;
#define HCC_BUS_INI_BUFF_SIZE  (100)
#endif

OAL_STATIC uint32_t g_switch_ip_pwrdown_bypass = 1;

OAL_STATIC int32_t g_switch_pwr_ret;
OAL_STATIC int32_t g_switch_total_count = 0;
uint32_t g_hcc_tx_err_cnt = 0;

#ifdef WIN32
uint32_t jiffies;
#endif

/* 函数声明 */
OAL_STATIC int32_t oal_register_gpio_intr(hcc_bus_dev *pst_bus_dev);
OAL_STATIC void oal_unregister_gpio_intr(hcc_bus_dev *pst_bus_dev);
OAL_STATIC void hcc_bus_sched_gpio_task(hcc_bus *pst_bus, int32_t irq);
OAL_STATIC void hcc_dev_pps_count_timeout(uintptr_t arg);
OAL_STATIC int32_t hcc_bus_auto_bindcpu_is_support(uint32_t dev_id);
OAL_STATIC int32_t hcc_bus_auto_sched_is_support(uint32_t dev_id);
OAL_STATIC int32_t hcc_switch_bus_request(uint32_t dev_id, uint32_t target, int32_t is_sync);
OAL_STATIC int32_t hcc_bus_resource_alloc(hcc_bus *pst_bus);
OAL_STATIC void hcc_bus_resource_free(hcc_bus *pst_bus);
OAL_STATIC int32_t hcc_set_pcie_switch_flag(int32_t flag);

OAL_STATIC hcc_bus *g_hisi_wifi_bus = OAL_PTR_NULL;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
/* 根据以下结构体来枚举 IP设备 */
OAL_STATIC hcc_bus_dev g_bus_dev_res[] = {
    {
        .dev_id = HCC_CHIP_110X_DEV,
        .init_bus_type = HCC_BUS_SDIO,
        .is_wakeup_gpio_support = 1,
#if defined(_PRE_PLAT_FEATURE_HI110X_PCIE)
        .bus_cap = (HCC_BUS_SDIO_CAP) | (HCC_BUS_PCIE_CAP),
#else
        .bus_cap = (HCC_BUS_SDIO_CAP),
#endif
        .name = "hi110x_dev",
        .en_flowctrl_gpio_registered = OAL_FALSE,
    },
};
#else
OAL_STATIC hcc_bus_dev g_bus_dev_res[OAL_BUS_DEV_RES_SIZE];
#endif

OAL_STATIC uint32_t g_bus_dump_mem_flag = 0;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_atomic g_wakeup_dev_wait_ack;
oal_atomic g_bus_powerup_dev_wait_ack; /* ip 上电握手标记 */
#endif

/* -1 means did't bind cpus */
OAL_STATIC int32_t g_cpu_id = -1;

#ifdef WIN32
char *g_hcc_bus_type_str[HCC_BUS_BUTT] = {
    "sdio",
    "pcie",
    "usb"
};
#else
OAL_STATIC char *g_hcc_bus_type_str[HCC_BUS_BUTT] = {
    [HCC_BUS_SDIO] = "sdio",
    [HCC_BUS_PCIE] = "pcie",
    [HCC_BUS_USB] = "usb"
};
#endif

OAL_STATIC char *hcc_bus_get_bus_type_str(uint32_t bus_type)
{
    if (oal_unlikely(bus_type >= HCC_BUS_BUTT)) {
        return "unkown";
    }

    return g_hcc_bus_type_str[bus_type] ? g_hcc_bus_type_str[bus_type] : "uninit";
}

OAL_STATIC void hcc_bus_stop_auto_pps_task(hcc_bus_dev *pst_bus_dev)
{
    /* cancel the timer */
    oal_timer_delete_sync(&pst_bus_dev->bus_pps_timer);
    pst_bus_dev->hcc->hcc_transer_info.tx_assem_info.pkt_cnt = 0;
    pst_bus_dev->hcc->hcc_transer_info.rx_assem_info.pkt_cnt = 0;
    hcc_dev_bindcpu(pst_bus_dev->dev_id, 0);
}

OAL_STATIC void hcc_bus_start_auto_pps_task(hcc_bus_dev *pst_bus_dev)
{
    /* start the timer */
    pst_bus_dev->bus_pps_start_time = jiffies;
    oal_timer_start(&pst_bus_dev->bus_pps_timer, HCC_BUS_PPS_COUNT_TIMEOUT);
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC int32_t hcc_bus_pm_state_switch_event(struct notifier_block *unused,
                                                 unsigned long wifi_state, void *ptr)
{
    hcc_bus_dev *pst_bus_dev;
    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "110x dev is null");
        return -OAL_ENODEV;
    }

    if (wifi_state == WIFI_PM_POWERUP_EVENT) {
        g_switch_total_count = 0;
    }

    if (!hcc_bus_auto_sched_is_support(pst_bus_dev->dev_id)) {
        return OAL_SUCC;
    }

    if (wifi_state == WIFI_PM_WAKEUP_EVENT) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "110x dev wakeup");
        hcc_bus_start_auto_pps_task(pst_bus_dev);
    } else if (wifi_state == WIFI_PM_SLEEP_EVENT) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "110x dev sleep");
        hcc_bus_stop_auto_pps_task(pst_bus_dev);
    } else if (wifi_state == WIFI_PM_POWERDOWN_EVENT) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "110x dev powerdown");
        hcc_bus_stop_auto_pps_task(pst_bus_dev);
    } else if (wifi_state == WIFI_PM_POWERUP_EVENT) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "110x dev powerup");
        hcc_bus_start_auto_pps_task(pst_bus_dev);
    }

    return 0;
}
#endif

int32_t hcc_switch_action_register(hcc_switch_action *action, void *data)
{
    if (oal_unlikely(action == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    action->data = data;
    oal_dlist_add_tail(&action->list, &g_hcc_bus_switch_hdr);

    return OAL_SUCC;
}

void hcc_switch_action_unregister(hcc_switch_action *action)
{
    if (oal_unlikely(action == NULL)) {
        oal_warn_on(1);
        return;
    }
    oal_dlist_delete_entry(&action->list);
}

OAL_STATIC int32_t bus_chan_init_string_cmp(const char *start, int len, const char *pc_para)
{
    if ((len == OAL_STRLEN(pc_para)) && (!oal_memcmp(start, pc_para, len))) {
        return OAL_SUCC;
    } else {
        return OAL_FAIL;
    }
}

OAL_STATIC uint32_t hcc_bus_chan_set(hcc_bus_dev *pst_bus_dev, const char *start, uint32 len)
{
    if (len == 0) {
        return HCC_BUS_BUTT;
    }

    if (bus_chan_init_string_cmp(start, len, "sdio") == OAL_SUCC) {
        pst_bus_dev->bus_cap |= HCC_BUS_SDIO_CAP;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi support sdio");
        return HCC_BUS_SDIO;
    }

    if (bus_chan_init_string_cmp(start, len, "pcie") == OAL_SUCC) {
        pst_bus_dev->bus_cap |= HCC_BUS_PCIE_CAP;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi support pcie");
        return HCC_BUS_PCIE;
    }

    /* wlan channel support pcie switch to sdio, used to pice hardwhare bugfix */
    /* current only used for vogue */
    if (bus_chan_init_string_cmp(start, len, "pcie_switch_enable") == OAL_SUCC) {
        if (pst_bus_dev->bus_cap & HCC_BUS_PCIE_CAP) {
            pst_bus_dev->bus_cap |= HCC_BUS_SDIO_CAP;
            hcc_set_pcie_switch_flag(1);
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi bus support pcie switch");
        }
        return HCC_BUS_BUTT;
    }

    /* 动态切换开关，由驱动来仲裁哪2种接口切换 */
    if (bus_chan_init_string_cmp(start, len, "switchon") == OAL_SUCC) {
        pst_bus_dev->bus_switch_enable = 1;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi enable dynamic bus switch");
        return HCC_BUS_BUTT;
    }

    if (bus_chan_init_string_cmp(start, len, "switchoff") == OAL_SUCC) {
        pst_bus_dev->bus_switch_enable = 0;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi disable dynamic bus switch");
        return HCC_BUS_BUTT;
    }

    if (bus_chan_init_string_cmp(start, len, "auto_switch") == OAL_SUCC) {
        pst_bus_dev->bus_auto_switch = 1;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi hcc bus auto switch enable");
        return HCC_BUS_BUTT;
    }

    if (bus_chan_init_string_cmp(start, len, "auto_bindcpu") == OAL_SUCC) {
        pst_bus_dev->bus_auto_bindcpu = 1;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi hcc bus auto bindcpu enable");
        return HCC_BUS_BUTT;
    }

    return HCC_BUS_BUTT;
}

OAL_STATIC void hcc_bus_chan_init_from_string(hcc_bus_dev *pst_bus_dev, const char *bus_select)
{
    int len;
    const char *start = NULL;
    const char *end = NULL;

    uint32_t init_bus_type = HCC_BUS_BUTT;
    uint32_t tmp_bus_type;

    if (OAL_STRLEN(g_str_ini_hcc_bus_switch) != 0) {
        start = g_str_ini_hcc_bus_switch;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "get ini string from module para [%s]", g_str_ini_hcc_bus_switch);
    } else {
        start = bus_select;
    }

    pst_bus_dev->bus_cap = 0;
    for (end = start;;) {
        // search to separative sign
        if ((*end != ',') && (*end != '\0')) {
            end++;
            continue;
        }

        len = end - start;

        tmp_bus_type = hcc_bus_chan_set(pst_bus_dev, start, len);
        // select the first found bus type
        if ((tmp_bus_type != HCC_BUS_BUTT) && (init_bus_type == HCC_BUS_BUTT)) {
            init_bus_type = tmp_bus_type;
            pst_bus_dev->init_bus_type = init_bus_type;
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi hcc bus type set to %s",
                                 (init_bus_type == HCC_BUS_SDIO) ? "sdio" : "pcie");
        }

        if (*end == '\0') {
            break;
        }

        start = end + 1;
        end++;
    }
}

int32_t hcc_bus_cap_init(uint32_t dev_id, const char *bus_select)
{
    hcc_bus_dev *pst_bus_dev;

    pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        return -OAL_ENODEV;
    }

    if (g_hisi_wifi_bus_select != NULL &&
        OAL_STRLEN(g_hisi_wifi_bus_select) != 0) {
        hcc_bus_chan_init_from_string(pst_bus_dev, g_hisi_wifi_bus_select);
        return OAL_SUCC;
    }

    if (bus_select != NULL &&
        OAL_STRLEN(bus_select) != 0) {
        hcc_bus_chan_init_from_string(pst_bus_dev, bus_select);
        return OAL_SUCC;
    }

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    /* default use pcie */
    hcc_bus_chan_init_from_string(pst_bus_dev, "pcie");
#else
    /* default use sdio */
    hcc_bus_chan_init_from_string(pst_bus_dev, "sdio");
#endif

    return OAL_SUCC;
}

/* This interface is not good, remove later */
hcc_bus *hcc_get_current_110x_bus(void)
{
    hcc_bus_dev *pst_bus_dev;
    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);
    if (pst_bus_dev == NULL) {
        return NULL;
    }

    return pst_bus_dev->cur_bus;
}

OAL_STATIC int32_t hcc_bus_check_dev_id(uint32_t dev_id)
{
    if (oal_warn_on(dev_id == 0xFFFFFFFF)) {
        return -OAL_ENODEV;
    }

    if (oal_warn_on(dev_id >= (uint32_t)oal_array_size(g_bus_dev_res))) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid dev id :%d", dev_id);
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_is_match(hcc_bus *pst_new, hcc_bus *pst_old)
{
    if (pst_new->bus_type != pst_old->bus_type) {
        return OAL_FALSE;
    }

    if (pst_new->bus_id != pst_old->bus_id) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

hcc_bus_dev *hcc_get_bus_dev(uint32_t dev_id)
{
    int32_t i;
    hcc_bus_dev *pst_bus_dev = NULL;

    if (hcc_bus_check_dev_id(dev_id) != OAL_SUCC) {
        return NULL;
    }

    for (i = 0; i < (int32_t)oal_array_size(g_bus_dev_res); i++) {
        pst_bus_dev = &g_bus_dev_res[i];
        if (dev_id == pst_bus_dev->dev_id) {
            return pst_bus_dev;
        }
    }

    return NULL;
}

OAL_STATIC int32_t hcc_foreach_switch_actions(uint32_t dev_id, hcc_bus *old_bus, hcc_bus *new_bus)
{
    int32_t ret = OAL_SUCC;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_switch_action *pst_action = NULL;

    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_switch_hdr)
    {
        pst_action = oal_dlist_get_entry(pst_entry, hcc_switch_action, list);
        if (pst_action->switch_notify != NULL) {
            ret = pst_action->switch_notify(dev_id, old_bus, new_bus, pst_action->data);
            if (ret != OAL_SUCC) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc switch action probe failed,name:%s, ret=%d,func:%p",
                                     pst_action->name ? pst_action->name : "unkown", ret, pst_action->switch_notify);
                return ret;
            }
        }
    }

    return ret;
}

hcc_bus *hcc_find_bus_by_devid(uint32_t dev_id, uint32_t bus_type)
{
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;

    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev_id %u invalid", dev_id);
        return NULL;
    }

    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        /* found the bus */
        if (pst_bus_tmp->bus_type == bus_type) {
            oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
            return pst_bus_tmp;
        }
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    return NULL;
}

OAL_STATIC int32_t hcc_select_bus(uint32_t dev_id, uint32_t bus_type)
{
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;

    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        return -OAL_EINVAL;
    }

    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        if (pst_bus_tmp->bus_dev == pst_bus_dev) {
            if (pst_bus_tmp->bus_type == bus_type) {
                /* match */
                pst_bus_dev->cur_bus = pst_bus_tmp;
                oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
                return OAL_SUCC;
            }
        }
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    return -OAL_ENODEV;
}

/* switch bus from sdio to pcie or pcie to sdio */
int32_t hcc_switch_bus(uint32_t dev_id, uint32_t bus_type)
{
    int32_t ret;
    hcc_bus *old_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    /* 暂时只允许在关机状态切换 */
    if (oal_warn_on(dev_id != HCC_CHIP_110X_DEV)) {
        return -OAL_EINVAL;
    }

    if (pst_bus_dev == NULL) {
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_bus_dev->cur_bus == NULL)) {
        return -OAL_ENODEV;
    }

    oal_wake_lock(&pst_bus_dev->st_switch_wakelock);

    old_bus = pst_bus_dev->cur_bus;

    ret = hcc_select_bus(dev_id, bus_type);
    if (ret == OAL_SUCC) {
        /* call switch bus actions after bus changed */
        ret = hcc_foreach_switch_actions(dev_id, old_bus, pst_bus_dev->cur_bus);
    }

    oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
    hcc_sched_transfer(pst_bus_dev->hcc);
    return ret;
}

int32_t hcc_bus_isvalid(hcc_bus *pst_bus)
{
    if (pst_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus is null");
        return -OAL_EINVAL;
    }

    if (pst_bus->bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus %d 's bus dev is null", pst_bus->bus_id);
        return -OAL_ENODEV;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_rx_response(struct hcc_handler *hcc, uint8_t stype,
                                       hcc_netbuf_stru *pst_hcc_netbuf, uint8_t *pst_context)
{
    hcc_switch_response st_switch_info_resp = {0};
    uint16_t transfer_type = 0;
    int32_t ret;
    oal_reference(pst_context);

    ret = memcpy_s((void *)&transfer_type, OAL_SIZEOF(transfer_type),
                   (void *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf), OAL_SIZEOF(transfer_type));
    if (ret != EOK) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "get transfer type failed");
    }

    if (transfer_type == HCC_BUS_TRANSFER_SWITCH_ACTION) {
        if (oal_netbuf_len(pst_hcc_netbuf->pst_netbuf) < OAL_SIZEOF(transfer_type) + OAL_SIZEOF(st_switch_info_resp)) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_rx_response netbuf too short, len %d < %d",
                                 (int32_t)oal_netbuf_len(pst_hcc_netbuf->pst_netbuf),
                                 (int32_t)(OAL_SIZEOF(transfer_type) + OAL_SIZEOF(st_switch_info_resp)));
            oal_print_hex_dump((uint8_t *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf),
                               oal_netbuf_len(pst_hcc_netbuf->pst_netbuf), HEX_DUMP_GROUP_SIZE, "switch resp: ");
        } else {
#ifdef WIN32
            ret = memcpy_s((char *)&st_switch_info_resp, OAL_SIZEOF(st_switch_info_resp),
                           (char *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf) + OAL_SIZEOF(transfer_type),
                           OAL_SIZEOF(st_switch_info_resp));
#else
            ret = memcpy_s((void *)&st_switch_info_resp, OAL_SIZEOF(st_switch_info_resp),
                           (void *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf) + OAL_SIZEOF(transfer_type),
                           OAL_SIZEOF(st_switch_info_resp));
#endif
            if (ret != EOK) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "get hcc_switch_response failed");
            }

            if (st_switch_info_resp.action_type == HCC_BUS_SWITCH_POWERUP) {
                oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus %d power up response, ret=%d",
                                     st_switch_info_resp.bus_id, st_switch_info_resp.is_succ);
                oal_complete(&hcc->bus_dev->st_switch_powerup_ready);
            } else if (st_switch_info_resp.action_type == HCC_BUS_SWITCH_POWERDOWN) {
                oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus %d power down response, ret=%d",
                                     st_switch_info_resp.bus_id, st_switch_info_resp.is_succ);
                oal_complete(&hcc->bus_dev->st_switch_powerdown_ready);
            } else {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_rx_response unkown action type:%d",
                                     st_switch_info_resp.action_type);
            }
        }
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "unkown bus rx resposne type %d", transfer_type);
        oal_print_hex_dump((uint8_t *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf),
                           oal_netbuf_len(pst_hcc_netbuf->pst_netbuf), HEX_DUMP_GROUP_SIZE, "switch resp: ");
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_notify_devip_powerctrl(hcc_bus *pst_bus, uint32_t is_powerup, uint32_t is_need_resp)
{
    int32_t cmd_len;
    hcc_switch_info st_switch_info;
    uint16_t transfer_type;
    oal_netbuf_stru *pst_netbuf = NULL;
    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = NULL;
    int32_t ret;

    if (pst_bus->bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_notify_devip_powerctrl bus_dev is null");
        return -OAL_ENODEV;
    }

    hcc = pst_bus->bus_dev->hcc;
    if (hcc == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_notify_devip_powerctrl hcc is null");
        return -OAL_ENODEV;
    }

    cmd_len = OAL_SIZEOF(uint16_t) + OAL_SIZEOF(hcc_switch_info);

    pst_netbuf = hcc_netbuf_alloc(cmd_len);
    if (pst_netbuf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_notify_devip_powerctrl alloc skb fail, cmd_len=%d", cmd_len);
        return -OAL_ENOMEM;
    }

    transfer_type = (uint16_t)HCC_BUS_TRANSFER_SWITCH_ACTION;
    st_switch_info.action_type = is_powerup ? HCC_BUS_SWITCH_POWERUP : HCC_BUS_SWITCH_POWERDOWN;
    st_switch_info.bus_id = pst_bus->bus_type;
    st_switch_info.is_need_resp = !!is_need_resp;

    oal_netbuf_put(pst_netbuf, cmd_len);

    ret = memcpy_s((void *)oal_netbuf_data(pst_netbuf), cmd_len,
                   (void *)&transfer_type, OAL_SIZEOF(uint16_t));
    if (ret != EOK) {
        oal_netbuf_free(pst_netbuf);
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "memcpy_s error, destlen=%d, srclen=%u\n ",
                             cmd_len, (uint32_t)OAL_SIZEOF(uint16_t));
        return -OAL_EFAIL;
    }

    ret = memcpy_s((void *)oal_netbuf_data(pst_netbuf) + OAL_SIZEOF(uint16_t),
                   cmd_len - OAL_SIZEOF(uint16_t), (void *)&st_switch_info, OAL_SIZEOF(st_switch_info));
    if (ret != EOK) {
        oal_netbuf_free(pst_netbuf);
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "memcpy_s error\n");
        return -OAL_EFAIL;
    }

    /* 高优先级发送 */
    hcc_hdr_param_init(&st_hcc_transfer_param,
                       HCC_ACTION_TYPE_BUS,
                       HCC_SUB_BUS_SWITCH,
                       0,
                       HCC_FC_NONE,
                       DATA_HI_QUEUE);  // DATA_HI_QUEUE

    ret = hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(pst_netbuf);
    }

    return ret;
}

/* enable to clear xfer_pending_sig, enable L1
 * disable to set xfer_pending_sig, disable L1 */
OAL_STATIC int32_t hcc_bus_ip_pm_ctrl(hcc_bus *pst_bus, uint32_t enable)
{
    int32_t ret;
    int32_t cmd_len;
    oal_netbuf_stru *pst_netbuf = NULL;
    struct hcc_transfer_param st_hcc_transfer_param = { 0 };
    struct hcc_handler *hcc = NULL;

    if (pst_bus->bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_ip_pm_ctrl bus_dev is null");
        return -OAL_ENODEV;
    }

    hcc = pst_bus->bus_dev->hcc;
    if (hcc == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_ip_pm_ctrl hcc is null");
        return -OAL_ENODEV;
    }

    cmd_len = OAL_SIZEOF(enable);

    pst_netbuf = hcc_netbuf_alloc(cmd_len);
    if (pst_netbuf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_ip_pm_ctrl alloc skb fail, cmd_len=%d", cmd_len);
        return -OAL_ENOMEM;
    }

    oal_netbuf_put(pst_netbuf, cmd_len);

    ret = memcpy_s((void *)oal_netbuf_data(pst_netbuf), cmd_len, &enable, OAL_SIZEOF(enable));
    if (ret != EOK) {
        oal_netbuf_free(pst_netbuf);
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "memcpy_s error, destlen=%d, srclen=%u\n ",
            cmd_len, (uint32_t)OAL_SIZEOF(enable));
        return -OAL_EFAIL;
    }

    /* 高优先级发送 */
    hcc_hdr_param_init(&st_hcc_transfer_param,
                       HCC_ACTION_TYPE_BUS,
                       HCC_SUB_BUS_IP_PM,
                       0,
                       HCC_FC_NONE,
                       DATA_HI_QUEUE);  // DATA_HI_QUEUE

    ret = hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(pst_netbuf);
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_ip_pm_ctrl send failed, ret=%d", ret);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_ip_pm_ctrl enable =%u", enable);
    }

    return ret;
}

int32_t hi110x_hcc_ip_pm_ctrl(uint32_t enable)
{
    hcc_bus *pst_bus = hcc_get_current_110x_bus();
    if (pst_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "current bus is null");
        return -OAL_ENODEV;
    }
    return hcc_bus_ip_pm_ctrl(pst_bus, enable);
}
oal_module_symbol(hi110x_hcc_ip_pm_ctrl);

/* 异常下电，不需要等response */
OAL_STATIC int32_t hcc_bus_switch_ip_power_down_excep(void *data)
{
    /* target ip power down process */
    hcc_bus_dev *pst_bus_dev = NULL;
    /* time cost */
    hcc_bus *target_bus = (hcc_bus *)(data);

    if (oal_warn_on(target_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "target bus is null");
        return -OAL_EFAIL;
    }

    pst_bus_dev = target_bus->bus_dev;
    if (oal_warn_on(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "target bus %d 's bus dev is null", target_bus->bus_id);
        return -OAL_ENODEV;
    }

    if (g_switch_ip_pwrdown_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "bus :%s exception power down bypass",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
        return OAL_SUCC;
    }

    /* Power Down old bus */
    oal_init_completion(&pst_bus_dev->st_switch_powerdown_ready);

    if (hcc_bus_notify_devip_powerctrl(target_bus, 0, 0) != OAL_SUCC) {
        /* 通知老的IP下电 */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_switch_thread send power down bus %s message failed",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
    }

    return OAL_SUCC;
}

/* 正常下电，等待response */
OAL_STATIC int32_t hcc_bus_switch_ip_power_down(void *data)
{
    /* target ip power down process */
    int32_t ret;
    hcc_bus_dev *pst_bus_dev = NULL;
    hcc_bus *target_bus = NULL;
    /* time cost */
    declare_time_cost_stru(cost);
    target_bus = (hcc_bus *)(data);

    if (oal_warn_on(target_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, ":%s error:target bus  is null", __FUNCTION__);
        return -OAL_EINVAL;
    };

    pst_bus_dev = target_bus->bus_dev;
    if (oal_warn_on(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "target bus %d 's bus dev is null", target_bus->bus_id);
        return -OAL_ENODEV;
    }

    oal_get_time_cost_start(cost);

    /* Power Down old bus */
    oal_init_completion(&pst_bus_dev->st_switch_powerdown_ready);

    if (hcc_bus_notify_devip_powerctrl(target_bus, 0, 1) != OAL_SUCC) {
        /* 通知老的IP下电 */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_switch_thread send power down bus %d message failed",
                             target_bus->bus_id);
    }

    ret = oal_wait_for_completion_interruptible_timeout(&pst_bus_dev->st_switch_powerdown_ready,
                                                        (uint32_t)oal_msecs_to_jiffies(10000));
    if (ret == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wait power down bus %s response timeout",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
        declare_dft_trace_key_info("power down bus response timeout failed", OAL_DFT_TRACE_FAIL);
    } else if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "wait power down bus %s response interrupt",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hardware powerdown %s cost time %llu us",
                         hcc_bus_get_bus_type_str(target_bus->bus_type), time_cost_var_sub(cost));

    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_switch_ip_power_on(void *data)
{
    int32_t ret;
    hcc_bus_dev *pst_bus_dev = NULL;
    hcc_bus *target_bus = NULL;
    /* time cost */
    declare_time_cost_stru(cost);

    target_bus = (hcc_bus *)(data);

    if (oal_warn_on(target_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, ":%s error:target bus  is null", __FUNCTION__);
        return -OAL_EINVAL;
    };

    oal_get_time_cost_start(cost);

    pst_bus_dev = target_bus->bus_dev;
    if (oal_warn_on(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "target bus %s 's bus dev is null",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
        declare_dft_trace_key_info("hcc_bus_switch_thread response timeout failed", OAL_DFT_TRACE_FAIL);
        g_switch_pwr_ret = -OAL_ENODEV;
        return -OAL_ENODEV;
    }

    /* ip上电 */
    /* 通知Device 给目标IP上电 */
    if (hcc_bus_notify_devip_powerctrl(target_bus, 1, 1) != OAL_SUCC) {
        /* 通知上电失败，忽略这次切换操作 */
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread power up bus %s failed",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
        declare_dft_trace_key_info("hcc_bus_switch_thread response timeout failed", OAL_DFT_TRACE_FAIL);
        g_switch_pwr_ret = -OAL_EBUSY;
        return -OAL_EBUSY;
    }

    /* 等待上电完成 */
    ret = oal_wait_for_completion_interruptible_timeout(&pst_bus_dev->st_switch_powerup_ready,
                                                        (uint32_t)oal_msecs_to_jiffies(10000));
    if (ret == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wait power up bus %s response timeout",
                             hcc_bus_get_bus_type_str(target_bus->bus_type));
        declare_dft_trace_key_info("hcc_bus_switch_thread response timeout failed", OAL_DFT_TRACE_FAIL);
        /* timeout, we send power down command */
        hcc_bus_notify_devip_powerctrl(target_bus, 0, 0);
        g_switch_pwr_ret = -OAL_ETIMEDOUT;
        return -OAL_ETIMEDOUT;
    } else if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wait power up bus %s response interrupt, ret=%d ",
                             hcc_bus_get_bus_type_str(target_bus->bus_type), ret);
        g_switch_pwr_ret = -OAL_ETIMEDOUT;
        return -OAL_EIO;
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hardware powerup %s cost time %llu us",
                         hcc_bus_get_bus_type_str(target_bus->bus_type), time_cost_var_sub(cost));

    g_switch_pwr_ret = OAL_SUCC;
    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_switch_thread_condtion(hcc_bus_dev *pst_bus_dev)
{
    return (pst_bus_dev->switch_bus_req != NULL);
}

OAL_STATIC void hcc_bus_switch_process_start(hcc_bus_dev *pst_bus_dev)
{
    pst_bus_dev->switch_state = HCC_BUS_SWITCH_STATE_START;
}

OAL_STATIC void hcc_bus_switch_process_end(hcc_bus_dev *pst_bus_dev)
{
    pst_bus_dev->switch_state = HCC_BUS_SWITCH_STATE_END;
    /* all done, switch success */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wake_up_interruptible(&pst_bus_dev->st_swtich_ack_wq);
#endif
}

OAL_STATIC void hcc_bus_switch_process_abort(hcc_bus_dev *pst_bus_dev)
{
    pst_bus_dev->switch_state = HCC_BUS_SWITCH_STATE_ABORT;
    /* all done, switch success */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wake_up_interruptible(&pst_bus_dev->st_swtich_ack_wq);
#endif
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
/*
 * Prototype    : hcc_bus_switch_thread
 * Description  : hcc bus switch thread
 * Input        : adapter
 * Return Value : succ or fail
 */
OAL_STATIC int32_t hcc_bus_switch_thread(void *data)
{
    int32_t ret;
    oal_uint flags;
    hcc_bus *target_bus = NULL;
    hcc_bus *old_bus = NULL;
#if defined (_PRE_LINUX_TEST) || defined (_PRE_WINDOWS_TEST)
    struct oal_sched_param_stru param;
#else
    struct sched_param param;
#endif
    hcc_bus_dev *pst_bus_dev = NULL;

    /* time cost */
    declare_time_cost_stru(cost);

    pst_bus_dev = (hcc_bus_dev *)data;
    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread pst_bus_dev is null, exit");
        return OAL_SUCC;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_switch_thread[%d] start", pst_bus_dev->dev_id);

    /* Must response immediately */
    param.sched_priority = OAL_BUS_SWITCH_THREAD_PRIORITY;
    oal_set_thread_property(current,
                            SCHED_RR,
                            &param,
                            MIN_NICE);

    allow_signal(SIGTERM);

    forever_loop() {
        if (oal_unlikely(oal_kthread_should_stop())) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "exit hcc_bus_switch_thread[%d]", pst_bus_dev->dev_id);
            break;
        }

        ret = oal_wait_event_interruptible_m(pst_bus_dev->st_switch_request_wq,
                                             hcc_bus_switch_thread_condtion(pst_bus_dev));
        if (oal_unlikely(ret == -ERESTARTSYS)) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc switch task %s was interrupted by a signal",
                                 oal_get_current_task_name());
            break;
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[%s]:%d", __FUNCTION__, __LINE__);

        oal_wake_lock(&pst_bus_dev->st_switch_wakelock);

        oal_spin_lock_irq_save(&pst_bus_dev->st_switch_lock, &flags);
        target_bus = pst_bus_dev->switch_bus_req;
        pst_bus_dev->switch_bus_req = NULL;
        if (target_bus == NULL) {
            oal_print_hi11xx_log(HI11XX_LOG_DBG, "target_bus is null");
            oal_spin_unlock_irq_restore(&pst_bus_dev->st_switch_lock, &flags);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            continue;
        }
        oal_spin_unlock_irq_restore(&pst_bus_dev->st_switch_lock, &flags);

        if (target_bus == pst_bus_dev->cur_bus) {
            oal_print_hi11xx_log(HI11XX_LOG_DBG, "req bus %s is working now!",
                                 hcc_bus_get_bus_type_str(pst_bus_dev->cur_bus->bus_type));
            hcc_bus_switch_process_end(pst_bus_dev);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            continue;
        }

        old_bus = pst_bus_dev->cur_bus;

        hcc_bus_switch_process_start(pst_bus_dev);

        if (target_bus->bus_type == HCC_BUS_SDIO) {
            declare_dft_trace_key_info("dync_switch_request_sdio", OAL_DFT_TRACE_OTHER);
        } else if (target_bus->bus_type == HCC_BUS_PCIE) {
            declare_dft_trace_key_info("dync_switch_request_pcie", OAL_DFT_TRACE_OTHER);
        } else {
            declare_dft_trace_key_info("dync_switch_request_xxxx", OAL_DFT_TRACE_OTHER);
        }

        oal_init_completion(&pst_bus_dev->st_switch_powerup_ready);

        wlan_pm_statesave();

        hcc_bus_power_action(target_bus, HCC_BUS_SW_POWER_PATCH_LOAD_PREPARE);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "switch to %s: g_switch_total_count:%d",
                             hcc_bus_get_bus_type_str(target_bus->bus_type), ++g_switch_total_count);
        oal_get_time_cost_start(cost);

        g_switch_pwr_ret = OAL_SUCC;

        /* Host发起目标IP硬件初始化 */
        hcc_bus_power_ctrl_register(target_bus, HCC_BUS_CTRL_POWER_UP, hcc_bus_switch_ip_power_on, (void *)target_bus);
        ret = hcc_bus_power_action(target_bus, HCC_BUS_SW_POWER_UP);
        if (ret != OAL_SUCC || g_switch_pwr_ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread init bus %s falied=%d, ret=%d",
                                 hcc_bus_get_bus_type_str(target_bus->bus_type),
                                 ret, g_switch_pwr_ret);
            declare_dft_trace_key_info("hcc_bus_switch_thread init bus failed", OAL_DFT_TRACE_FAIL);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            wlan_pm_staterestore();
            hcc_bus_switch_process_abort(pst_bus_dev);
            continue;
        }
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[%s]:%d", __FUNCTION__, __LINE__);
        /* 资源初始化 */
        ret = hcc_bus_reinit(target_bus);
        if (ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread reinit bus %s falied=%d",
                                 hcc_bus_get_bus_type_str(target_bus->bus_type), ret);
            declare_dft_trace_key_info("hcc_bus_switch_thread reinit bus failed", OAL_DFT_TRACE_FAIL);
            hcc_bus_power_ctrl_register(target_bus, HCC_BUS_CTRL_POWER_DOWN,
                                        hcc_bus_switch_ip_power_down_excep,
                                        (void *)target_bus);
            hcc_bus_power_action(target_bus, HCC_BUS_SW_POWER_DOWN);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            wlan_pm_staterestore();
            hcc_bus_switch_process_abort(pst_bus_dev);
            continue;
        }
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[%s]:%d", __FUNCTION__, __LINE__);
        /* Host发起目标IP软件初始化 */
        /* ip初始化 不需要下载PATCH 此时WCPU在正常运行 */
        ret = hcc_bus_power_action(target_bus, HCC_BUS_SW_POWER_PATCH_LAUCH);
        if (ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread lauch bus %s falied=%d",
                                 hcc_bus_get_bus_type_str(target_bus->bus_type), ret);
            declare_dft_trace_key_info("hcc_bus_switch_thread lauch bus failed", OAL_DFT_TRACE_FAIL);
            hcc_bus_power_ctrl_register(target_bus, HCC_BUS_CTRL_POWER_DOWN,
                                        hcc_bus_switch_ip_power_down_excep,
                                        (void *)target_bus);
            hcc_bus_power_action(target_bus, HCC_BUS_SW_POWER_DOWN);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            wlan_pm_staterestore();
            hcc_bus_switch_process_abort(pst_bus_dev);
            continue;
        }

        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus res init %s cost time %llu us",
                             hcc_bus_get_bus_type_str(target_bus->bus_type), time_cost_var_sub(cost));
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[%s]:%d", __FUNCTION__, __LINE__);

        if (hcc_bus_auto_sched_is_support(pst_bus_dev->dev_id)) {
            /* stop auto pps timer */
            hcc_bus_stop_auto_pps_task(pst_bus_dev);
        }

        /* 硬件初始化完成，切换软件 */
        /* 暂停hcc 线程,或者hcc_tx锁 */
        oal_get_time_cost_start(cost);
        hcc_tx_transfer_lock(pst_bus_dev->hcc);

        /* 挂起发送通道,等待通道清空, 清空后WCPU会切换bus */
        ret = hcc_bus_switch_suspend_tx(old_bus);

        ret = hcc_bus_switch_clean_res(old_bus);
        if (ret) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread clean_res bus %s failed",
                                 hcc_bus_get_bus_type_str(old_bus->bus_type));
            declare_dft_trace_key_info("hcc_bus_switch_thread clean_res bus failed", OAL_DFT_TRACE_FAIL);

            hcc_tx_transfer_unlock(pst_bus_dev->hcc);
            if (hcc_bus_auto_sched_is_support(pst_bus_dev->dev_id)) {
                /* stop auto pps timer */
                hcc_bus_start_auto_pps_task(pst_bus_dev);
            }
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            wlan_pm_staterestore();
            hcc_bus_switch_process_abort(pst_bus_dev);
            continue;
        }

        /* 重入问题 */
        hcc_switch_bus(pst_bus_dev->dev_id, target_bus->bus_type);

        if (old_bus == pst_bus_dev->cur_bus) {
            /* switch failed? */
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_switch_thread switch bus %s failed",
                                 hcc_bus_get_bus_type_str(old_bus->bus_type));
            declare_dft_trace_key_info("hcc_bus_switch_thread switch bus failed", OAL_DFT_TRACE_FAIL);
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_switch_thread switch bus from %s to %s",
                                 hcc_bus_get_bus_type_str(old_bus->bus_type),
                                 hcc_bus_get_bus_type_str(target_bus->bus_type));
        }

        /* 发消息通知DEV锁住DMA调度关闭消息接口，等待IP空闲 */
        /* 切换完成恢复发送通道, 上面Host bus 切换有时间差， 所有完成后需要重新调度受影响的任务 */
        hcc_bus_sched_gpio_task(pst_bus_dev->cur_bus, 0);

        ret = hcc_bus_switch_resume_tx(pst_bus_dev->cur_bus);

        hcc_tx_transfer_unlock(pst_bus_dev->hcc);
        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "lock bus %s tx chan cost time %llu us",
                             hcc_bus_get_bus_type_str(target_bus->bus_type), time_cost_var_sub(cost));

        if (old_bus != pst_bus_dev->cur_bus) {
            /* Power Down old bus */
            oal_init_completion(&pst_bus_dev->st_switch_powerdown_ready);

            hcc_bus_rx_transfer_lock(old_bus);

            ret = hcc_bus_power_ctrl_register(old_bus,
                                              HCC_BUS_CTRL_POWER_DOWN,
                                              hcc_bus_switch_ip_power_down, (void *)old_bus);
            hcc_bus_power_action(old_bus, HCC_BUS_SW_POWER_DOWN);
            /* 此时不管低功耗是否打开，old_bus已经持有一把锁，需要释放,并且锁住new_bus */
            hcc_bus_wake_lock(pst_bus_dev->cur_bus);
            hcc_bus_wake_unlock(old_bus);
            hcc_bus_rx_transfer_unlock(old_bus);
        }

        if (pst_bus_dev->cur_bus->bus_type == HCC_BUS_SDIO) {
            declare_dft_trace_key_info("dync_switch_sdio_done", OAL_DFT_TRACE_OTHER);
        } else if (pst_bus_dev->cur_bus->bus_type == HCC_BUS_PCIE) {
            declare_dft_trace_key_info("dync_switch_pcie_done", OAL_DFT_TRACE_OTHER);
        } else {
            declare_dft_trace_key_info("dync_switch_xxxx_done", OAL_DFT_TRACE_OTHER);
        }

        /* pm had disabled */
        if (hcc_bus_auto_bindcpu_is_support(pst_bus_dev->dev_id)) {
            hcc_bus_bindcpu(old_bus, HCC_DIR_COUNT, 0);
        }

        if (hcc_bus_auto_sched_is_support(pst_bus_dev->dev_id)) {
            /* stop auto pps timer */
            hcc_bus_start_auto_pps_task(pst_bus_dev);
        }

        wlan_pm_staterestore();

        hcc_bus_switch_process_end(pst_bus_dev);
        oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_switch_thread[%d] end", pst_bus_dev->dev_id);
    return OAL_SUCC;
}
#endif

OAL_STATIC int32_t hcc_bus_auto_switch_is_support(uint32_t dev_id)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (oal_unlikely(pst_bus_dev == NULL)) {
        return 0;
    }

    if (g_hcc_bus_auto_switch) {
        return 1;
    }

    return pst_bus_dev->bus_auto_switch;
}

OAL_STATIC int32_t hcc_bus_auto_bindcpu_is_support(uint32_t dev_id)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (oal_unlikely(pst_bus_dev == NULL)) {
        return 0;
    }

    if (g_hcc_bus_auto_bindcpu) {
        return 1;
    }

    return pst_bus_dev->bus_auto_bindcpu;
}

OAL_STATIC int32_t hcc_bus_auto_sched_is_support(uint32_t dev_id)
{
    if (!hcc_bus_auto_switch_is_support(dev_id) && !hcc_bus_auto_bindcpu_is_support(dev_id)) {
        return 0;
    }

    return 1;
}

int32_t hi110x_hcc_dev_bindcpu(int32_t is_bind, oal_bool_enum_uint8 is_userctl,
                               uint8_t irq_cpu, uint8_t thread_cmd)
{
    hcc_bus *pst_bus = NULL;
    uint32_t dev_id = HCC_CHIP_110X_DEV;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    oal_pcie_bindcpu_cfg bindcpu_cfg;

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    if (hcc_bus_auto_bindcpu_is_support(dev_id)) {
        /* 自动绑核打开直接返回 */
        return OAL_SUCC;
    }

    if (g_hcc_bus_manual_bindcpu == 0) {
        /* forbid manual control */
        return OAL_SUCC;
    }

    pst_bus = pst_bus_dev->cur_bus;

    bindcpu_cfg.is_bind = is_bind;
    bindcpu_cfg.irq_cpu = irq_cpu;
    bindcpu_cfg.thread_cmd = thread_cmd;
    bindcpu_cfg.is_userctl = is_userctl;

    return hcc_bus_bindcpu(pst_bus, HCC_DIR_COUNT, *(int32_t *)&bindcpu_cfg);
}
EXPORT_SYMBOL_GPL(hi110x_hcc_dev_bindcpu);

int32_t hcc_dev_bindcpu(uint32_t dev_id, int32_t is_bind)
{
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    pst_bus = pst_bus_dev->cur_bus;
    if (hcc_bus_auto_bindcpu_is_support(dev_id)) {
        return hcc_bus_bindcpu(pst_bus, HCC_DIR_COUNT, is_bind);
    } else {
        return -OAL_EIO;
    }
}
EXPORT_SYMBOL_GPL(hcc_dev_bindcpu);

OAL_STATIC void hcc_bus_performance_switch(uint32_t dev_id, hcc_bus_dev *pst_bus_dev, unsigned long pkt_pps_total)
{
    if (hcc_bus_auto_switch_is_support(dev_id)) {
        if (pkt_pps_total >= g_hcc_bus_auto_switch_limit) {
            /* switch to PCIe */
            hcc_switch_bus_request(dev_id, HCC_BUS_PCIE, 0);
        } else if (pkt_pps_total <= g_hcc_bus_auto_switch_limit / 2) {
            if (hdev_to_hbus(pst_bus_dev)->bus_type != HCC_BUS_SDIO) {
                /* 低吞吐不需要重复切换 */
                hcc_switch_bus_request(dev_id, HCC_BUS_SDIO, 0);
            }
        }
    }
}

/* bus性能调度 */
int32_t hcc_bus_performance_core_schedule(uint32_t dev_id)
{
    uint32_t run_time;
    unsigned long current_time, pkt_pps_rx, pkt_pps_tx, pkt_pps_total;
    unsigned long tx_cnt, rx_cnt, total_cnt;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    if (!hcc_bus_auto_sched_is_support(dev_id)) {
        oal_print_hi11xx_log(HI11XX_LOG_VERBOSE, "hcc bus auto sched disabled");
        return OAL_SUCC;
    }

    current_time = jiffies;

    if (oal_time_after(current_time, (pst_bus_dev->bus_pps_start_time + 1 * HZ))) {
        /* time is up, calc pps */
        tx_cnt = pst_bus_dev->hcc->hcc_transer_info.tx_assem_info.pkt_cnt;
        rx_cnt = pst_bus_dev->hcc->hcc_transer_info.rx_assem_info.pkt_cnt;
        total_cnt = tx_cnt + rx_cnt;

        run_time = oal_jiffies_to_msecs(current_time)
                   - oal_jiffies_to_msecs(pst_bus_dev->bus_pps_start_time);
        if (run_time == 0) {
            run_time = 1; /* 1 msec at least */
        }

        /* 计算吞吐率 */
        pkt_pps_tx = (tx_cnt * 1000) / (unsigned long)run_time;
        pkt_pps_rx = (rx_cnt * 1000) / (unsigned long)run_time;

        pkt_pps_total = pkt_pps_tx + pkt_pps_rx;

        g_hcc_bus_current_pps = pkt_pps_total;

        hcc_bus_performance_switch(dev_id, pst_bus_dev, pkt_pps_total);

        if (hcc_bus_auto_bindcpu_is_support(dev_id)) {
            hcc_bus *pst_bus = pst_bus_dev->cur_bus;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            if (pkt_pps_total >= g_hcc_bus_auto_bindcpu_limit) {
                hcc_bus_bindcpu(pst_bus, HCC_DIR_COUNT, 1);
            } else if (pkt_pps_total <= g_hcc_bus_auto_bindcpu_limit / 2) {
                hcc_bus_bindcpu(pst_bus, HCC_DIR_COUNT, 0);
            }
#endif
        }

        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pkt_pps_tx: %lu , pkt_pps_rx: %lu, runtime:%u",
                             pkt_pps_tx, pkt_pps_rx, run_time);

        pst_bus_dev->hcc->hcc_transer_info.tx_assem_info.pkt_cnt = 0;
        pst_bus_dev->hcc->hcc_transer_info.rx_assem_info.pkt_cnt = 0;
        pst_bus_dev->bus_pps_start_time = current_time;
    }

    return OAL_SUCC;
}
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC int32_t hcc_bus_performance_core_init(uint32_t dev_id)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    pst_bus_dev->bus_pps_start_time = jiffies;
    pst_bus_dev->hcc->hcc_transer_info.tx_assem_info.pkt_cnt = 0;
    pst_bus_dev->hcc->hcc_transer_info.rx_assem_info.pkt_cnt = 0;
    oal_timer_init(&pst_bus_dev->bus_pps_timer, HCC_BUS_PPS_COUNT_TIMEOUT,
                   hcc_dev_pps_count_timeout, (uintptr_t)pst_bus_dev);

    return OAL_SUCC;
}
#endif
OAL_STATIC int32_t hcc_bus_performance_core_exit(uint32_t dev_id)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    oal_timer_delete_sync(&pst_bus_dev->bus_pps_timer);
    return OAL_SUCC;
}

/*
 *   switch hcc to target bus SDIO/PCIE/USB
 *   target: the bus type you want to use
 *   is_sync: 1 means function util switch done
 */
OAL_STATIC int32_t hcc_switch_bus_request(uint32_t dev_id, uint32_t target, int32_t is_sync)
{
    int32_t ret;
    oal_uint flags;
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);

    if (oal_warn_on(dev_id != HCC_CHIP_110X_DEV)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "non-support switch dev id:%u", dev_id);
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev id:%u 's bus dev is null", dev_id);
        return -OAL_EINVAL;
    }

    if (oal_warn_on(target >= HCC_BUS_BUTT)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid bus type:%d", target);
        return -OAL_EINVAL;
    }

    if (pst_bus_dev->bus_switch_enable == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "hcc switch disable");
        return OAL_SUCC;
    }

    if (pst_bus_dev->pst_switch_task == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "switch task is not exist");
        return -OAL_ENODEV;
    }

    pst_bus = hcc_find_bus_by_devid(dev_id, target);
    if (oal_warn_on(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "find bus failed by devid:%u , bus type:%u", dev_id, target);
        return -OAL_ENODEV;
    }

    if (hcc_bus_isvalid(pst_bus) != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus %d is not ready", target);
        return -OAL_ENODEV;
    }

    if (pst_bus_dev->pst_switch_task == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "switch task is not ready");
        return -OAL_ENODEV;
    }

    oal_wake_lock(&pst_bus_dev->st_switch_wakelock);

    oal_spin_lock_irq_save(&pst_bus_dev->st_switch_lock, &flags);

    if (pst_bus_dev->switch_bus_req != NULL) {
        /* Last req is processing, overwrite */
        if (hcc_bus_is_match(pst_bus, pst_bus_dev->cur_bus) == OAL_TRUE) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus %s is already working",
                                 hcc_bus_get_bus_type_str(pst_bus->bus_type));
            return OAL_SUCC;
        }
    }

    pst_bus_dev->switch_bus_req = pst_bus;

    oal_spin_unlock_irq_restore(&pst_bus_dev->st_switch_lock, &flags);

    hcc_bus_switch_process_start(pst_bus_dev);

    /* wakeup the switch thread */
    oal_wait_queue_wake_up_interrupt(&pst_bus_dev->st_switch_request_wq);

    if (is_sync) {
        /* 等待切换完成 */
        ret = oal_wait_event_interruptible_timeout_m(pst_bus_dev->st_swtich_ack_wq,
                                                     ((pst_bus_dev->switch_state == HCC_BUS_SWITCH_STATE_ABORT) ||
                                                     (pst_bus_dev->switch_state == HCC_BUS_SWITCH_STATE_END)),
                                                     30 * OAL_TIME_HZ);
        if (oal_unlikely(ret == -ERESTARTSYS)) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO,
                                 "hcc switch task %s was interrupted by a signal, ret=%d, switch_state=%d",
                                 oal_get_current_task_name(), ret, pst_bus_dev->switch_state);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            return -OAL_EFAIL;
        } else if (ret == 0) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc switch task %s was timeout, ret=%d, switch_state=%d",
                                 oal_get_current_task_name(), ret, pst_bus_dev->switch_state);
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            return -OAL_ETIMEDOUT;
        }

        if (pst_bus_dev->switch_state == HCC_BUS_SWITCH_STATE_END) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "switch bus type %s sync done",
                                 hcc_bus_get_bus_type_str(pst_bus->bus_type));
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            return OAL_SUCC;
        } else if (pst_bus_dev->switch_state == HCC_BUS_SWITCH_STATE_ABORT) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "switch bus type %s abort",
                                 hcc_bus_get_bus_type_str(pst_bus->bus_type));
            oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
            return -OAL_EFAIL;
        }

        oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
        return -OAL_EFAIL;
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "switch bus type %s async done",
                             hcc_bus_get_bus_type_str(pst_bus->bus_type));
        oal_wake_unlock(&pst_bus_dev->st_switch_wakelock);
        return OAL_SUCC;
    }
}

#ifdef _PRE_SDIO_TAE_DEBUG
OAL_STATIC int32_t hcc_bus_pwr_on_pcie_handler(void *data)
{
    /* try to reinit pcie */
    hcc_bus_send_message(hcc_get_current_110x_bus(), H2D_MSG_RESERV_15);
    oal_msleep(500);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_pwr_on_pcie_handler done");
    return OAL_SUCC;
}

int32_t hcc_bus_pwr_on_pcie(void)
{
    int32_t ret;
    uint32_t dev_id = HCC_CHIP_110X_DEV;
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus_dev is null");
        return -OAL_ENODEV;
    }

    if (!g_sdio_tae_debug_enable) {
        return OAL_SUCC;
    }

    pst_bus = hcc_find_bus_by_devid(dev_id, HCC_BUS_PCIE);
    if (oal_warn_on(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "find bus failed by devid:%u , bus type:%u", dev_id, HCC_BUS_PCIE);
        return -OAL_ENODEV;
    }

    if (hcc_bus_isvalid(pst_bus) != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus %d is not ready", HCC_BUS_PCIE);
        return -OAL_ENODEV;
    }

    if (pst_bus == pst_bus_dev->cur_bus) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "current is pcie");
        return OAL_SUCC;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_pwr_on_pcie");

    hcc_bus_power_action(pst_bus, HCC_BUS_SW_POWER_PATCH_LOAD_PREPARE);

    hcc_bus_power_ctrl_register(pst_bus, HCC_BUS_CTRL_POWER_UP, hcc_bus_pwr_on_pcie_handler, NULL);
    ret = hcc_bus_power_action(pst_bus, HCC_BUS_POWER_UP);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_pwr_on_pcie init bus %s falied=%d",
                             hcc_bus_get_bus_type_str(pst_bus->bus_type),
                             ret);
        declare_dft_trace_key_info("HCC_BUS_SW_POWER_UP", OAL_DFT_TRACE_FAIL);
        return ret;
    }

    ret = hcc_bus_reinit(pst_bus);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_reinit failed ret=%d", ret);
        return ret;
    }

    ret = hcc_bus_power_action(pst_bus, HCC_BUS_SW_POWER_PCIE_TAE_INIT);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_bus_power_action failed ret=%d", ret);
        return ret;
    }

    return OAL_SUCC;
}

int32_t hcc_bus_pwr_off_pcie(void)
{
    uint32_t dev_id = HCC_CHIP_110X_DEV;
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus_dev is null");
        return -OAL_ENODEV;
    }

    if (!g_sdio_tae_debug_enable) {
        return OAL_SUCC;
    }

    pst_bus = hcc_find_bus_by_devid(dev_id, HCC_BUS_PCIE);
    if (oal_warn_on(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "find bus failed by devid:%u , bus type:%u", dev_id, HCC_BUS_PCIE);
        return -OAL_ENODEV;
    }

    if (hcc_bus_isvalid(pst_bus) != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus %d is not ready", dev_id);
        return -OAL_ENODEV;
    }

    if (pst_bus == pst_bus_dev->cur_bus) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "current is pcie");
        return OAL_SUCC;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_bus_pwr_off_pcie");

    hcc_bus_power_ctrl_register(pst_bus, HCC_BUS_CTRL_POWER_DOWN,
                                NULL,
                                NULL);
    hcc_bus_power_action(pst_bus, HCC_BUS_POWER_DOWN);
    return OAL_SUCC;
}
#endif

/* high speed chan */
int32_t hi110x_switch_to_hcc_highspeed_chan(uint32_t is_high)
{
    uint32_t target = is_high ? HCC_BUS_PCIE : HCC_BUS_SDIO;
    return hi110x_switch_hcc_bus_request(target);
}
/*lint -e19 */
oal_module_symbol(hi110x_switch_to_hcc_highspeed_chan);
/*lint +e19 */
int32_t hi110x_switch_hcc_bus_request(uint32_t target)
{
    if (g_hcc_bus_switch_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hi110x_switch_bus_request %d bypass", target);
        return OAL_SUCC;
    }

    if (hcc_bus_auto_switch_is_support(HCC_CHIP_110X_DEV)) {
        return OAL_SUCC;
    }

    return hcc_switch_bus_request(HCC_CHIP_110X_DEV, target, 0);
}
/*lint -e19 */
oal_module_symbol(hi110x_switch_hcc_bus_request);
/*lint +e19 */
int32_t hcc_switch_bus_loop_test(uint32_t dev_id, uint32_t loop_count)
{
    uint32_t i;
    int32_t ret;
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = NULL;
    const uint32_t ul_loop_times = loop_count * 2;

    for (i = 0; i < ul_loop_times; i++) {
        if (oal_unlikely(g_hcc_bus_switch_test_break)) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_switch_bus_loop_test abort");
            return -OAL_EBUSY;
        }

        pst_bus_dev = hcc_get_bus_dev(dev_id);
        if (pst_bus_dev == NULL) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev id %d invalid", dev_id);
            return -OAL_ENODEV;
        }

        pst_bus = pst_bus_dev->cur_bus;
        if (pst_bus == NULL) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev id %d invalid, bus is null", dev_id);
            return -OAL_ENODEV;
        }

        ret = hcc_switch_bus_request(dev_id, ((pst_bus->bus_type == HCC_BUS_SDIO) ? HCC_BUS_PCIE : HCC_BUS_SDIO), 1);
        if (ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "switch to %s failed ret=%d",
                                 hcc_bus_get_bus_type_str(pst_bus->bus_type), ret);
            return ret;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "switch to %s ok, test times %u",
                                 hcc_bus_get_bus_type_str(pst_bus->bus_type), (i >> 1) + 1);
        }

        if (g_hcc_bus_switch_test_delay_time) {
            oal_msleep(g_hcc_bus_switch_test_delay_time);
        }
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "test bus loop %u times done!", loop_count);

    return OAL_SUCC;
}

/* after all bus inited */
OAL_STATIC int32_t hcc_probe_bus(hcc_bus *pst_bus)
{
    hcc_bus_dev *pst_bus_dev = NULL;

    if (oal_warn_on(pst_bus->bus_dev != NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "%s had probed", pst_bus->name);
        return OAL_SUCC;
    }

    pst_bus_dev = hcc_get_bus_dev(pst_bus->dev_id);
    if (pst_bus_dev == NULL) {
        return -OAL_EINVAL;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus_type:%s, cap:%u, dev_name:%s",
                         hcc_bus_get_bus_type_str(pst_bus->bus_type), pst_bus_dev->bus_cap, pst_bus_dev->name);

    if ((1 << pst_bus->bus_type) & pst_bus_dev->bus_cap) {
        pst_bus->bus_dev = pst_bus_dev;
        pst_bus_dev->bus_num++;
        return OAL_SUCC;
    }

    return -OAL_ENODEV;
}

/*
 * Prototype    : hcc_message_register
 * Description  : used for sdio messsage callback, 1 bit to 1 msg event
 * Input        : struct hcc_handler *hcc
 */
int32_t hcc_message_register(struct hcc_handler *hcc, uint8_t msg, hcc_msg_rx cb, void *data)
{
    int32_t flag = 0;
    int32_t ret = OAL_SUCC;
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;

    if (oal_unlikely(hcc == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        if (pst_bus_tmp->bus_dev == hcc->bus_dev) {
            /* 注册在当前设备上的所有BUS 都需要注册 */
            ret = hcc_bus_message_register(pst_bus_tmp, msg, cb, data);
            if (ret != OAL_SUCC) {
                oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "register bus %d failed, ret=%d", pst_bus_tmp->bus_id, ret);
                return ret;
            }
            flag++;
        }
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    if (flag) {
        return OAL_SUCC;
    } else {
        return -OAL_ENODEV;
    }
}

void hcc_message_unregister(struct hcc_handler *hcc, uint8_t msg)
{
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;

    if (oal_unlikely(hcc == NULL)) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        if (pst_bus_tmp->bus_dev == hcc->bus_dev) {
            /* 注册在当前设备上的所有BUS 都需要注册 */
            hcc_bus_message_unregister(pst_bus_tmp, msg);
        }
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
}

hcc_bus *hcc_alloc_bus(void)
{
    hcc_bus *pst_bus = OAL_PTR_NULL;

    pst_bus = oal_memalloc(OAL_SIZEOF(hcc_bus));
    if (pst_bus == OAL_PTR_NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc hcc bus failed, size:%u", (uint32_t)OAL_SIZEOF(hcc_bus));
        return NULL;
    }

    memset_s((void *)pst_bus, OAL_SIZEOF(hcc_bus), 0, OAL_SIZEOF(hcc_bus));

    pst_bus->dev_id = 0xFFFFFFFF;

    /* 设置4字节对齐 */
    pst_bus->cap.align_size[HCC_TX] = sizeof(uint32_t);
    pst_bus->cap.align_size[HCC_RX] = sizeof(uint32_t);

    pst_bus->cap.is_full_duplex = 0;

    pst_bus->cap.max_trans_size = 0x7fffffff;
    return pst_bus;
}

void hcc_free_bus(hcc_bus *pst_bus)
{
    if (oal_likely(pst_bus != NULL)) {
        oal_free(pst_bus);
    }
}

int32_t hcc_send_message(struct hcc_handler *hcc, uint32_t val)
{
    uint32_t ret = OAL_SUCC;
    hcc_bus *hi_bus = NULL;

    if (oal_unlikely(hcc == NULL)) {
        return -OAL_ENODEV;
    }

    hi_bus = hcc_to_bus(hcc);

    hcc_tx_transfer_lock(hcc);
    if (hcc_bus_pm_wakeup_device(hi_bus) == OAL_SUCC) {
        ret = hcc_bus_send_message(hi_bus, val);
    } else {
        hcc_tx_transfer_unlock(hcc);
        return -OAL_EFAIL;
    }
    hcc_tx_transfer_unlock(hcc);

    return ret;
}

OAL_STATIC void hcc_dev_res_exit_by_id(int32_t dev_max)
{
    int32_t i;
    hcc_bus_dev *pst_bus_dev = NULL;
    dev_max = oal_min(dev_max, (int32_t)oal_array_size(g_bus_dev_res));
    for (i = 0; i < dev_max; i++) {
        pst_bus_dev = &g_bus_dev_res[i];
        oal_wake_lock_exit(&pst_bus_dev->st_switch_wakelock);
        if (pst_bus_dev->is_wakeup_gpio_support) {
            oal_unregister_gpio_intr(pst_bus_dev);
        }

        /* HCC支持GPIO流控 */
        if (pst_bus_dev->en_flowctrl_gpio_registered) {
            /* 注销GPIO流控 */
            oal_unregister_gpio_flowctrl_intr(pst_bus_dev);
        }
    }
}

OAL_STATIC void hcc_dev_res_exit(void)
{
    hcc_dev_res_exit_by_id((int32_t)oal_array_size(g_bus_dev_res));
}

OAL_STATIC int32_t hcc_dev_res_init(void)
{
    int32_t i;
    int32_t ret;
    hcc_bus_dev *pst_bus_dev = NULL;
    for (i = 0; i < (int32_t)oal_array_size(g_bus_dev_res); i++) {
        pst_bus_dev = &g_bus_dev_res[i];
        oal_spin_lock_init(&pst_bus_dev->st_irq_lock);
        oal_spin_lock_init(&pst_bus_dev->st_switch_lock);
        oal_wait_queue_init_head(&pst_bus_dev->st_switch_request_wq);
        oal_wake_lock_init(&pst_bus_dev->st_switch_wakelock, "bus_dev_wakelock");
        if (pst_bus_dev->is_wakeup_gpio_support) {
            ret = oal_register_gpio_intr(pst_bus_dev);
            if (ret) {
                goto failed_reg_gpio_int;
            }
        }
    }

    return OAL_SUCC;
failed_reg_gpio_int:
    hcc_dev_res_exit_by_id(i);
    return ret;
}

int32_t hcc_add_bus(hcc_bus *pst_bus, const char *bus_name)
{
    int32_t ret;
    int32_t init_flag = 0;
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;
    if (oal_warn_on(pst_bus == NULL)) {
        return -OAL_EINVAL;
    }

    /* Check duplicate bus */
    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        if (oal_warn_on(hcc_bus_is_match(pst_bus_tmp, pst_bus) == OAL_TRUE)) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus[%p] is duplicate, type:%s, id:%d",
                                 pst_bus, hcc_bus_get_bus_type_str(pst_bus->bus_type), pst_bus->bus_id);
            oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
            return -OAL_EBUSY;
        }
    }

    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    ret = hcc_bus_resource_alloc(pst_bus);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = snprintf_s(pst_bus->name, OAL_SIZEOF(pst_bus->name), OAL_SIZEOF(pst_bus->name) - 1,
                     "%s:%u", bus_name, pst_bus->bus_id);
    if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus name format err\n");
        return ret;
    }

    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    /* If empty init dev res */
    if (oal_dlist_is_empty(&g_hcc_bus_res_hdr) == OAL_TRUE) {
        oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
        ret = hcc_dev_res_init();
        oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
        if (ret) {
            goto failed_dev_res_init;
        }
        init_flag = 1;
    }

    oal_dlist_add_tail(&pst_bus->list, &g_hcc_bus_res_hdr);
    ret = hcc_probe_bus(pst_bus);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc bus %s probed failed!", pst_bus->name);
        goto failed_probe_bus;
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    return OAL_SUCC;
failed_probe_bus:
    oal_dlist_delete_entry(&pst_bus->list);
    if (init_flag) {
        oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
        hcc_dev_res_exit();
        oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    }
failed_dev_res_init:
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
    hcc_bus_resource_free(pst_bus);
    return ret;
}

int32_t hcc_remove_bus(hcc_bus *pst_bus)
{
    oal_uint irqsave;

    if (oal_warn_on(pst_bus == NULL)) {
        return OAL_SUCC;
    }

    /* delete bus and remove from dev resource */
    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);

    oal_dlist_delete_entry(&pst_bus->list);

    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
    hcc_bus_resource_free(pst_bus);
    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);

    if (oal_dlist_is_empty(&g_hcc_bus_res_hdr) == OAL_TRUE) {
        oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
        hcc_dev_res_exit();
        oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    }

    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
    return OAL_SUCC;
}

#ifdef _PRE_HISI_BINDCPU
/* Debug */
int32_t hcc_bus_printbindcpu(void)
{
    hcc_bus *hi_bus = NULL;
    struct hcc_handler *hcc = NULL;
    struct cpumask slow_cpus, fast_cpus;

    hisi_get_slow_cpus(&slow_cpus);
    hisi_get_fast_cpus(&fast_cpus);

    hi_bus = hcc_get_current_110x_bus();
    if (hi_bus == NULL) {
        return -OAL_EFAIL;
    }

    hcc = hbus_to_hcc(hi_bus);
    if (hcc == NULL) {
        return -OAL_EFAIL;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc cpu mask:0x%lx",
                         cpumask_bits(&(hcc->hcc_transer_info.hcc_transfer_thread->cpus_allowed))[0]);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc cpu fast_cpus:0x%lx", cpumask_bits(&fast_cpus)[0]);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc cpu slow_cpus:0x%lx", cpumask_bits(&slow_cpus)[0]);

    return OAL_SUCC;
}
#endif


OAL_STATIC void hcc_bus_hcctransfer_bindcpu(struct hcc_handler *hcc,
    int32_t is_bind, oal_bool_enum_uint8 is_userctl, uint8_t thread_cpu)
{
#ifdef _PRE_HISI_BINDCPU
    struct cpumask slow_cpus, fast_cpus;
    struct cpumask thread_cpu_mask = *((struct cpumask *)&thread_cpu);

    if (is_userctl == OAL_TRUE) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "hisi_hcc bind to cpu[0x%x]", thread_cpu);
        set_cpus_allowed_ptr(hcc->hcc_transer_info.hcc_transfer_thread, &thread_cpu_mask);
    } else {
        hisi_get_slow_cpus(&slow_cpus);
        hisi_get_fast_cpus(&fast_cpus);
        cpumask_clear_cpu(OAL_BUS_HPCPU_NUM, &fast_cpus);
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "hcc thread hisi rt opt cpu");
        if (is_bind) {
            set_cpus_allowed_ptr(hcc->hcc_transer_info.hcc_transfer_thread, &fast_cpus);
        } else {
            set_cpus_allowed_ptr(hcc->hcc_transer_info.hcc_transfer_thread, &slow_cpus);
        }
    }
#endif
}


OAL_STATIC void hcc_bus_rx_tsk_bindcpu(hcc_bus *hi_bus,
    int32_t is_bind, oal_bool_enum_uint8 is_userctl, uint8_t thread_cpu)
{
#ifdef _PRE_HISI_BINDCPU
    struct cpumask slow_cpus, fast_cpus;
    struct cpumask thread_cpu_mask = *((struct cpumask *)&thread_cpu);

    if (is_userctl == OAL_TRUE) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "wlan_bus_rx bind to cpu[0x%x]", thread_cpu);
        set_cpus_allowed_ptr(hi_bus->pst_rx_tsk, &thread_cpu_mask);
    } else {
        hisi_get_slow_cpus(&slow_cpus);
        hisi_get_fast_cpus(&fast_cpus);
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "bus rx task hisi rt opt cpu");
        if (is_bind) {
            set_cpus_allowed_ptr(hi_bus->pst_rx_tsk, &fast_cpus);
        } else {
            set_cpus_allowed_ptr(hi_bus->pst_rx_tsk, &slow_cpus);
        }
    }
#endif
}

int32_t hcc_bus_bindcpu(hcc_bus *hi_bus, uint32_t chan, int32_t is_bind)
{
    oal_pcie_bindcpu_cfg bindcpu_cfg = *(oal_pcie_bindcpu_cfg *)&is_bind;
    struct hcc_handler *hcc = NULL;

    if (oal_warn_on(hi_bus == NULL)) {
        return -OAL_ENODEV;
    }

    if (oal_unlikely(hi_bus->opt_ops->bindcpu == NULL)) {
        return -OAL_EIO;
    }

    hcc = hbus_to_hcc(hi_bus);

    if (bindcpu_cfg.is_userctl == OAL_FALSE) {
        if (hi_bus->is_bind == bindcpu_cfg.is_bind) {
            return OAL_SUCC;
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc %s cpu", is_bind ? "bind" : "unbind");

        hi_bus->is_bind = bindcpu_cfg.is_bind;
    }

    if (oal_unlikely(hcc != NULL)) {
        if (hcc->hcc_transer_info.hcc_transfer_thread) {
            /* 绑定线程hcc_transfer_thread */
            hcc_bus_hcctransfer_bindcpu(hcc, bindcpu_cfg.is_bind,
                bindcpu_cfg.is_userctl, bindcpu_cfg.thread_cmd);
        }
    }

    if (hi_bus->pst_rx_tsk != NULL) {
        /* 绑定线程pst_rx_tsk */
        hcc_bus_rx_tsk_bindcpu(hi_bus, bindcpu_cfg.is_bind,
            bindcpu_cfg.is_userctl, bindcpu_cfg.thread_cmd);
    }

    return hi_bus->opt_ops->bindcpu(hi_bus, chan, is_bind);
}

EXPORT_SYMBOL_GPL(hcc_bus_bindcpu);

/* PM */
int32_t hcc_bus_pm_wakeup_device(hcc_bus *hi_bus)
{
    uint32_t uret;

    if (oal_warn_on(hi_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hi_bus is null");
        return -OAL_EIO;
    }

    if (oal_unlikely(hi_bus->pst_pm_callback == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pm_callback is null");
        return -OAL_EIO;
    }

    if (oal_unlikely(hi_bus->pst_pm_callback->pm_wakeup_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pm_wakeup_dev is null");
        return -OAL_EIO;
    }

    uret = hi_bus->pst_pm_callback->pm_wakeup_dev();
    if (oal_unlikely(uret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pm_wakeup_dev failed ret=%u, bus_type=%u", uret, hi_bus->bus_type);
        wlan_pm_dump_host_info();
        return -OAL_EBUSY;
    }

    return OAL_SUCC;
}
EXPORT_SYMBOL_GPL(hcc_bus_pm_wakeup_device);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
void hcc_bus_exception_submit(hcc_bus *hi_bus, int32_t excep_type)
{
    unsigned long flags;
    oal_workqueue_stru *pst_excp_workqueue = NULL;

    declare_dft_trace_key_info("wifi_trans_exception", OAL_DFT_TRACE_EXCEP);
    if (oal_unlikely(hi_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[E]%s, hi_bus is null", __FUNCTION__);
        return;
    }

    pst_excp_workqueue = wifi_get_exception_workqueue();
    if (oal_unlikely(pst_excp_workqueue == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[E]%s, pst_excp_workqueue is null", __FUNCTION__);
        return;
    }

    /* disable bus */
    hcc_bus_disable_state(hi_bus, OAL_BUS_STATE_RX);

    oal_spin_lock_irq_save(&hi_bus->bus_excp_lock, &flags);

    if (oal_work_is_busy(&hi_bus->bus_excp_worker)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "excep %d block, exception %d is working",
                             excep_type, hi_bus->bus_excp_type);
        oal_spin_unlock_irq_restore(&hi_bus->bus_excp_lock, &flags);
        return;
    }

    hcc_change_state_exception();
    hi_bus->bus_excp_type = excep_type;

    if (excep_type != WIFI_DEVICE_PANIC) {
        oal_spin_unlock_irq_restore(&hi_bus->bus_excp_lock, &flags);
        if (hi_bus->bus_type == HCC_BUS_SDIO) {
            (void)ssi_dump_err_regs(SSI_ERR_HCC_EXCP_SDIO);
        }
        if (hi_bus->bus_type == HCC_BUS_PCIE) {
            (void)ssi_dump_err_regs(SSI_ERR_HCC_EXCP_PCIE);
        }
        oal_spin_lock_irq_save(&hi_bus->bus_excp_lock, &flags);
    }

    oal_dft_print_error_key_info();

    if (g_hcc_exception_enable) {
        /* stop switch thread before dfr */
        hcc_dev_switch_disable(hi_bus->dev_id);
        oal_wake_lock_timeout(&hi_bus->bus_excp_wlock, 10 * 1000);
        queue_work(pst_excp_workqueue, &hi_bus->bus_excp_worker);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "wifi exception memdump function disabled");
    }
    oal_spin_unlock_irq_restore(&hi_bus->bus_excp_lock, &flags);
}
/*lint -e19*/
oal_module_symbol(hcc_bus_exception_submit);
/*lint +e19*/
int32_t hcc_bus_exception_is_busy(hcc_bus *hi_bus)
{
    if (oal_unlikely(hi_bus == NULL)) {
        return OAL_FALSE;
    }

    if (oal_work_is_busy(&hi_bus->bus_excp_worker)) {
        /* mem dump is processing,
          can't power off or submit repeat */
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
/*lint -e19*/
oal_module_symbol(hcc_bus_exception_is_busy);
/*lint +e19*/
int32_t oal_trigger_bus_exception(hcc_bus *hi_bus, int32_t is_sync)
{
    unsigned long timeout_jiffies;
    if (oal_unlikely(hi_bus == NULL)) {
        return OAL_FALSE;
    }

    if (hcc_bus_exception_is_busy(hi_bus) == OAL_TRUE) {
        return OAL_TRUE;
    }

    if (oal_is_chip_error_block() == OAL_TRUE) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "bypass bus exception");
        return OAL_TRUE;
    }

    /* trigger device panic */
    if (hcc_bus_send_message(hi_bus, H2D_MSG_TEST)) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "send device panic message failed!");
        return OAL_FALSE;
    }

    if (is_sync != OAL_TRUE) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "wifi exception is doing...");
        return OAL_TRUE;
    }

    /* wait device panic */
    timeout_jiffies = jiffies + oal_msecs_to_jiffies(2000);
    forever_loop() {
        if (hcc_bus_exception_is_busy(hi_bus) == OAL_TRUE) {
            break;
        }

        if (time_after(jiffies, timeout_jiffies)) {
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "wait panic message timeout!");
            return OAL_FALSE;
        }

        oal_msleep(oal_jiffies_to_msecs(1));
        up(&hi_bus->rx_sema);
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "trigger wifi exception manually sucuess");
    return OAL_TRUE;
}
/*lint -e19*/
oal_module_symbol(oal_trigger_bus_exception);
/*lint +e19*/
void hcc_bus_exception_submit_test(void)
{
    hcc_bus_exception_submit(hcc_get_current_110x_bus(), WIFI_TRANS_FAIL);
}

OAL_STATIC int32_t hcc_bus_device_panic_callback(void *data)
{
    hcc_bus *hi_bus = (hcc_bus *)data;
    hcc_bus_disable_state(hi_bus, OAL_BUS_STATE_ALL);
    hcc_bus_exception_submit(hi_bus, WIFI_DEVICE_PANIC);

    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_DEV, CHR_PLAT_DRV_ERROR_WIFI_DEV_PANIC);

    return OAL_SUCC;
}

OAL_STATIC int32_t hcc_bus_device_init_ready(void *data)
{
    hcc_bus *hi_bus = (hcc_bus *)data;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device ready, bus:%s", hcc_bus_get_bus_type_str(hi_bus->bus_type));
    oal_complete(&hi_bus->st_device_ready);
    return OAL_SUCC;
}

OAL_STATIC int32 wait_wlan_wkup_gpio_level(int32 gpio_level, unsigned long timeout)
{
    int32 gpio_value;
    gpio_level = (gpio_level == GPIO_LOWLEVEL) ? GPIO_LOWLEVEL : GPIO_HIGHLEVEL;
    forever_loop() {
        gpio_value = board_get_wlan_wkup_gpio_val();
        gpio_value = (gpio_value == GPIO_LOWLEVEL) ? GPIO_LOWLEVEL : GPIO_HIGHLEVEL;
        if (gpio_value == gpio_level) {
            return SUCCESS;
        }

        if (time_after(jiffies, timeout)) {
            return -FAILURE;
        } else {
            if (wlan_is_shutdown()) {
                oal_io_print("[E]hold_time_check:wifi is shutdown!\n");
                return (gpio_level == GPIO_LOWLEVEL) ? SUCCESS : -FAILURE;
            }
            oal_usleep_range(10, 20);
            cpu_relax();
        }
    }
}

/* 检查DEVICE WAKEUP HOST gpio 是否拉高。 */
OAL_STATIC int32_t oal_dev2host_gpio_hold_time_check(uint32_t switch_timeout, uint32_t hold_time)
{
    unsigned long timeout;
    declare_time_cost_stru(cost);
    const uint32_t ul_default_timeout = 200;

    if (!switch_timeout) {
        switch_timeout = ul_default_timeout;
    }

    timeout = jiffies + oal_msecs_to_jiffies(switch_timeout);
    oal_get_time_cost_start(cost);

    // wait gpio high
    if (wait_wlan_wkup_gpio_level(GPIO_HIGHLEVEL, timeout) != SUCCESS) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[E]wait wakeup gpio to high timeout [%u] ms", switch_timeout);
        return OAL_FALSE;
    }
    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device reset  ip cost %llu us", time_cost_var_sub(cost));

    // high level hold time
    timeout = jiffies + oal_msecs_to_jiffies(hold_time);
    // wait high level hold time
    if (wait_wlan_wkup_gpio_level(GPIO_LOWLEVEL, timeout) == SUCCESS) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[E]gpio pull down again!");
        return OAL_FALSE;
    }

    // gpio high and hold enough time.
    return OAL_TRUE;
}

/* check whether device had panic */
OAL_STATIC int32_t hcc_bus_panic_status_check(hcc_bus *hi_bus)
{
    int32_t ret = OAL_FALSE;

    if (hi_bus->bus_excp_type == WIFI_DEVICE_PANIC) {
        ret = OAL_TRUE;
    }

    /* gpio mode,两参数均代表设置的超时 */
    if (oal_dev2host_gpio_hold_time_check(5000, 100) == OAL_TRUE) {
        ret = OAL_TRUE;
    } else {
        ret = OAL_FALSE;
    }
    return ret;
}

/* for 1103 */
/* mpw2 config **/
#define WCPU_1103_MPW2_PANIC_MEMDUMP_MAX_SIZE 0x3c00 /* 15KB */
#define WCPU_1103_MPW2_PANIC_MEMDUMP_MAX_ADDR 0x60080000
/* 0x6007c400 */
#define WCPU_1103_MPW2_PANIC_MEMDUMP_STORE_ADDR \
    (WCPU_1103_MPW2_PANIC_MEMDUMP_MAX_ADDR - WCPU_1103_MPW2_PANIC_MEMDUMP_MAX_SIZE)

#define WCPU_1103_MPW2_ITCM_RAM_MAX_SIZE 0x88000 /* 544KB */
#define WCPU_1103_MPW2_ITCM_RAM_MAX_ADDR 0x8c000
/* 0x4000 */
#define WCPU_1103_MPW2_ITCM_RAM_STORE_ADDR (WCPU_1103_MPW2_ITCM_RAM_MAX_ADDR - WCPU_1103_MPW2_ITCM_RAM_MAX_SIZE)

#define WCPU_1103_MPW2_DTCM_RAM_MAX_SIZE 0x68000 /* 416KB */
#define WCPU_1103_MPW2_DTCM_RAM_MAX_ADDR 0x20068000
/* 0x20000000 */
#define WCPU_1103_MPW2_DTCM_RAM_STORE_ADDR (WCPU_1103_MPW2_DTCM_RAM_MAX_ADDR - WCPU_1103_MPW2_DTCM_RAM_MAX_SIZE)

#define WCPU_1103_MPW2_PKT_MEM_MAX_SIZE 0x90000 /* 576KB */
#define WCPU_1103_MPW2_PKT_MEM_MAX_ADDR 0x60090000
/* 0x60000000 */
#define WCPU_1103_MPW2_PKT_MEM_STORE_ADDR (WCPU_1103_MPW2_PKT_MEM_MAX_ADDR - WCPU_1103_MPW2_PKT_MEM_MAX_SIZE)
OAL_STATIC struct st_wifi_dump_mem_info g_hi1103_mpw2_meminfo[] = {
    {
        .file_name = "wifi_device_panic_mem",
        .mem_addr = WCPU_1103_MPW2_PANIC_MEMDUMP_STORE_ADDR,
        .size = WCPU_1103_MPW2_PANIC_MEMDUMP_MAX_SIZE, /* 15KB */
    },
    {
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = WCPU_1103_MPW2_ITCM_RAM_STORE_ADDR,
        .size = WCPU_1103_MPW2_ITCM_RAM_MAX_SIZE, /* 544KB */
    },
    {
        .file_name = "wifi_device_dtcm_ram",
        .mem_addr = WCPU_1103_MPW2_DTCM_RAM_STORE_ADDR,
        .size = WCPU_1103_MPW2_DTCM_RAM_MAX_SIZE, /* 416KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = WCPU_1103_MPW2_PKT_MEM_STORE_ADDR,
        .size = WCPU_1103_MPW2_PKT_MEM_MAX_SIZE, /* 576KB */
    },

};

/* pilot config **/
#define WCPU_1103_PILOT_PANIC_MEMDUMP_MAX_SIZE 0x3c00 /* 15KB */
#define WCPU_1103_PILOT_PANIC_MEMDUMP_MAX_ADDR 0x60080000
/* 0x6007c400 */
#define WCPU_1103_PILOT_PANIC_MEMDUMP_STORE_ADDR \
    (WCPU_1103_PILOT_PANIC_MEMDUMP_MAX_ADDR - WCPU_1103_PILOT_PANIC_MEMDUMP_MAX_SIZE)

#define WCPU_1103_PILOT_TICM_RAM_MAX_SIZE 0x98000 /* 608KB */
#define WCPU_1103_PILOT_TICM_RAM_MAX_ADDR 0xa8000
/* 0x10000 */
#define WCPU_1103_PILOT_TICM_RAM_STORE_ADDR (WCPU_1103_PILOT_TICM_RAM_MAX_ADDR - WCPU_1103_PILOT_TICM_RAM_MAX_SIZE)

#define WCPU_1103_PILOT_DTCM_RAM_MAX_SIZE 0x68000 /* 416KB */
#define WCPU_1103_PILOT_DTCM_RAM_MAX_ADDR 0x20080000
/* 0x20018000 */
#define WCPU_1103_PILOT_DTCM_RAM_STORE_ADDR (WCPU_1103_PILOT_DTCM_RAM_MAX_ADDR - WCPU_1103_PILOT_DTCM_RAM_MAX_SIZE)

#define WCPU_1103_PILOT_PKT_MEM_MAX_SIZE 0x80000 /* 512KB */
#define WCPU_1103_PILOT_PKT_MEM_MAX_ADDR 0x60080000
/* 0x60000000 */
#define WCPU_1103_PILOT_PKT_MEM_STORE_ADDR (WCPU_1103_PILOT_PKT_MEM_MAX_ADDR - WCPU_1103_PILOT_PKT_MEM_MAX_SIZE)
OAL_STATIC struct st_wifi_dump_mem_info g_hi1103_pilot_meminfo[] = {
    {
        .file_name = "wifi_device_panic_mem",
        .mem_addr = WCPU_1103_PILOT_PANIC_MEMDUMP_STORE_ADDR,
        .size = WCPU_1103_PILOT_PANIC_MEMDUMP_MAX_SIZE, /* 15KB */
    },
    {
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = WCPU_1103_PILOT_TICM_RAM_STORE_ADDR,
        .size = WCPU_1103_PILOT_TICM_RAM_MAX_SIZE, /* 608KB */
    },
    {
        .file_name = "wifi_device_dtcm_ram",
        .mem_addr = WCPU_1103_PILOT_DTCM_RAM_STORE_ADDR,
        .size = WCPU_1103_PILOT_DTCM_RAM_MAX_SIZE, /* 416KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = WCPU_1103_PILOT_PKT_MEM_STORE_ADDR,
        .size = WCPU_1103_PILOT_PKT_MEM_MAX_SIZE, /* 512KB */
    },

};

#define WCPU_1105_ASIC_PANIC_MEMDUMP_MAX_SIZE 0x3c00 /* 15KB */
#define WCPU_1105_ASIC_PANIC_MEMDUMP_MAX_ADDR 0x60080000
/* 0x6007c400 */
#define WCPU_1105_ASIC_PANIC_MEMDUMP_STORE_ADDR \
    (WCPU_1105_ASIC_PANIC_MEMDUMP_MAX_ADDR - WCPU_1105_ASIC_PANIC_MEMDUMP_MAX_SIZE)

#define WCPU_1105_ASIC_ITCM_RAM_MAX_SIZE 0xb0000 /* 640+64KB */
#define WCPU_1105_ASIC_ITCM_RAM_MAX_ADDR 0xc0000
/* 0x10000 */
#define WCPU_1105_ASIC_ITCM_RAM_STORE_ADDR (WCPU_1105_ASIC_ITCM_RAM_MAX_ADDR - WCPU_1105_ASIC_ITCM_RAM_MAX_SIZE)

#define WCPU_1105_ASIC_DTCM_RAM_MAX_SIZE 0x68000 /* 416KB */
#define WCPU_1105_ASIC_DTCM_RAM_MAX_ADDR 0x20080000
/* 0x20018000 */
#define WCPU_1105_ASIC_DTCM_RAM_STORE_ADDR (WCPU_1105_ASIC_DTCM_RAM_MAX_ADDR - WCPU_1105_ASIC_DTCM_RAM_MAX_SIZE)

#define WCPU_1105_ASIC_PKT_MEM_MAX_SIZE 0x100000 /* 1024KB */
#define WCPU_1105_ASIC_PKT_MEM_MAX_ADDR 0x60100000
/* 0x60000000 */
#define WCPU_1105_ASIC_PKT_MEM_STORE_ADDR (WCPU_1105_ASIC_PKT_MEM_MAX_ADDR - WCPU_1105_ASIC_PKT_MEM_MAX_SIZE)
OAL_STATIC struct st_wifi_dump_mem_info g_hi1105_pilot_asic_meminfo[] = {
    {
        .file_name = "wifi_device_panic_mem",
        .mem_addr = WCPU_1105_ASIC_PANIC_MEMDUMP_STORE_ADDR,
        .size = WCPU_1105_ASIC_PANIC_MEMDUMP_MAX_SIZE, /* 15KB */
    },
    {
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = WCPU_1105_ASIC_ITCM_RAM_STORE_ADDR,
        .size = WCPU_1105_ASIC_ITCM_RAM_MAX_SIZE, /* 640+64KB */
    },
    {
        .file_name = "wifi_device_dtcm_ram",
        .mem_addr = WCPU_1105_ASIC_DTCM_RAM_STORE_ADDR,
        .size = WCPU_1105_ASIC_DTCM_RAM_MAX_SIZE, /* 416KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = WCPU_1105_ASIC_PKT_MEM_STORE_ADDR,
        .size = WCPU_1105_ASIC_PKT_MEM_MAX_SIZE, /* 512*2KB */
    },
};

#define WCPU_1105_FPGA_PANIC_MEMDUMP_MAX_SIZE 0x3c00 /* 15KB */
#define WCPU_1105_FPGA_PANIC_MEMDUMP_MAX_ADDR 0x60080000
/* 0x6007c400 */
#define WCPU_1105_FPGA_PANIC_MEMDUMP_STORE_ADDR \
    (WCPU_1105_FPGA_PANIC_MEMDUMP_MAX_ADDR - WCPU_1105_FPGA_PANIC_MEMDUMP_MAX_SIZE)

#define WCPU_1105_FPGA_TICM_RAM_MAX_SIZE 0xa0000 /* 640KB */
#define WCPU_1105_FPGA_TICM_RAM_MAX_ADDR 0xb0000
/* 0x10000 */
#define WCPU_1105_FPGA_TICM_RAM_STORE_ADDR (WCPU_1105_FPGA_TICM_RAM_MAX_ADDR - WCPU_1105_FPGA_TICM_RAM_MAX_SIZE)

#define WCPU_1105_FPGA_DTCM_RAM_MAX_SIZE 0x68000 /* 416KB */
#define WCPU_1105_FPGA_DTCM_RAM_MAX_ADDR 0x20080000
/* 0x20018000 */
#define WCPU_1105_FPGA_DTCM_RAM_STORE_ADDR (WCPU_1105_FPGA_DTCM_RAM_MAX_ADDR - WCPU_1105_FPGA_DTCM_RAM_MAX_SIZE)

#define WCPU_1105_FPGA_PKT_MEM_MAX_SIZE 0x80000 /* 512KB */
#define WCPU_1105_FPGA_PKT_MEM_MAX_ADDR 0x60080000
/* 0x60000000 */
#define WCPU_1105_FPGA_PKT_MEM_STORE_ADDR (WCPU_1105_FPGA_PKT_MEM_MAX_ADDR - WCPU_1105_FPGA_PKT_MEM_MAX_SIZE)
OAL_STATIC struct st_wifi_dump_mem_info g_hi1105_pilot_fpga_meminfo[] = {
    {
        .file_name = "wifi_device_panic_mem",
        .mem_addr = WCPU_1105_FPGA_PANIC_MEMDUMP_STORE_ADDR,
        .size = WCPU_1105_FPGA_PANIC_MEMDUMP_MAX_SIZE, /* 15KB */
    },
    {
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = WCPU_1105_FPGA_TICM_RAM_STORE_ADDR,
        .size = WCPU_1105_FPGA_TICM_RAM_MAX_SIZE, /* 640KB */
    },
    {
        .file_name = "wifi_device_dtcm_ram",
        .mem_addr = WCPU_1105_FPGA_DTCM_RAM_STORE_ADDR,
        .size = WCPU_1105_FPGA_DTCM_RAM_MAX_SIZE, /* 416KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = WCPU_1105_FPGA_PKT_MEM_STORE_ADDR,
        .size = WCPU_1105_FPGA_PKT_MEM_MAX_SIZE, /* 512KB */
    },
};

/* Hi1106 mem macros */
#define HI1106_WL_WRAM_BASEADDR            (0x40000)
#define HI1106_WL_WRAM_LEN                 (0x170000)

#define HI1106_WL_S6_SHARE_RAM_BASEADDR    (0x2000000)
#define HI1106_WL_SHARE_RAM_LEN            (0x88000)

OAL_STATIC struct st_wifi_dump_mem_info g_hi1106_pilot_fpga_meminfo[] = {
    {
        /* Hi1106 对应WSRAM,名字需要修改对应oam_hisi */
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = HI1106_WL_WRAM_BASEADDR,
        .size = HI1106_WL_WRAM_LEN, /* 1280KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = HI1106_WL_S6_SHARE_RAM_BASEADDR,
        .size = HI1106_WL_SHARE_RAM_LEN, /* 544KB */
    },
};

OAL_STATIC struct st_wifi_dump_mem_info g_hi1106_pilot_asic_meminfo[] = {
    {
        /* Hi1106 对应WSRAM,名字需要修改对应oam_hisi */
        .file_name = "wifi_device_itcm_ram",
        .mem_addr = HI1106_WL_WRAM_BASEADDR,
        .size = HI1106_WL_WRAM_LEN, /* 1280KB */
    },
    {
        .file_name = "wifi_device_pkt_mem",
        .mem_addr = HI1106_WL_S6_SHARE_RAM_BASEADDR,
        .size = HI1106_WL_SHARE_RAM_LEN, /* 544KB */
    },
};

OAL_STATIC void hcc_bus_exception_handler(oal_work_stru *work)
{
    hcc_bus *hi_bus = hcc_get_current_110x_bus();
    struct st_wifi_dump_mem_info *mem_info = NULL;
    uint32_t size;
    int32_t excp_type;

    if (hi_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hi_bus is null");
        return;
    }

    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1105 %s memdump ", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            /* 1105 Asic */
            mem_info = g_hi1105_pilot_asic_meminfo;
            size = sizeof(g_hi1105_pilot_asic_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        } else {
            /* 1105 Fpga */
            mem_info = g_hi1105_pilot_fpga_meminfo;
            size = sizeof(g_hi1105_pilot_fpga_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        }
    } else if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1106 %s memdump ", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            /* 1106 Asic */
            mem_info = g_hi1106_pilot_asic_meminfo;
            size = sizeof(g_hi1106_pilot_asic_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        } else {
            /* 1106 Fpga */
            mem_info = g_hi1106_pilot_fpga_meminfo;
            size = sizeof(g_hi1106_pilot_fpga_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        }
    } else {
        if (get_hi110x_asic_type() == HI1103_ASIC_PILOT) {
            mem_info = g_hi1103_pilot_meminfo;
            size = sizeof(g_hi1103_pilot_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        } else {
            mem_info = g_hi1103_mpw2_meminfo;
            size = sizeof(g_hi1103_mpw2_meminfo) / sizeof(struct st_wifi_dump_mem_info);
        }
    }

    hi_bus->mem_info = mem_info;
    hi_bus->mem_size = size;

    hcc_bus_wake_lock(hi_bus);
    if (hi_bus->bus_excp_type == WIFI_DEVICE_PANIC) {
        declare_dft_trace_key_info("wifi_device_panic", OAL_DFT_TRACE_EXCEP);
        oam_error_log0(0, OAM_SF_ANY, "[E]WiFi Device Panic");
    }

    if (hi_bus->bus_excp_type == WIFI_TRANS_FAIL) {
        declare_dft_trace_key_info("wifi_trans_fail", OAL_DFT_TRACE_EXCEP);
        oam_error_log0(0, OAM_SF_ANY, "[E]wifi_trans_fail");
    }

    hcc_bus_disable_state(hi_bus, OAL_BUS_STATE_ALL);

    oal_dft_print_all_key_info();

#ifdef CONFIG_PRINTK
    hwifi_panic_log_dump(KERN_INFO);
    hcc_bus_print_trans_info(hi_bus, 0);
#endif

    if (hi_bus->pst_pm_callback->pm_disable) {
        hi_bus->pst_pm_callback->pm_disable(OAL_FALSE);
    }

    excp_type = hi_bus->bus_excp_type;
    if (hcc_bus_panic_status_check(hi_bus) == OAL_TRUE) {
        /* device panic */
        oam_error_log0(0, OAM_SF_ANY, "[E]check wakeup gpio ok, wcpu had panic");
        if (hi_bus->bus_excp_type != WIFI_DEVICE_PANIC) {
            oam_error_log1(0, OAM_SF_ANY, "[E]change sdio excp type from %d to device panic ", hi_bus->bus_excp_type);
        }
        hi_bus->bus_excp_type = WIFI_DEVICE_PANIC;
        oal_msleep(10);
    } else {
        /* device is't panic, reset sdio ip from host */
        oam_error_log0(0, OAM_SF_ANY, "[E]check wakeup gpio fail, wcpu crash");
        hi_bus->bus_excp_type = WIFI_TRANS_FAIL;
    }

    if (wlan_is_shutdown()) {
        hcc_bus_wake_unlock(hi_bus);
        oal_io_print("[E]dfr ignored, wifi shutdown, wakelock cnt %lu\n", hi_bus->st_bus_wakelock.lock_count);
        return;
    }

    /* close sdio data transfer when device panic. */

    if (wlan_is_shutdown()) {
        oam_error_log0(0, OAM_SF_ANY, "wifi is shutdown, dfr submit ignored");
    } else {
        wifi_exception_work_submit(excp_type);
    }

    hcc_bus_wake_unlock(hi_bus);
}

#undef COFNIG_TEST_SDIO_INT_LOSS
/*
 * Prototype    : oal_gpio_rxdata_thread
 * Description  : gpio rx data function
 * Input        : adapter   oal_sdio handler
 * Return Value : succ or fail
 */
OAL_STATIC int32_t hcc_bus_rxdata_thread(void *data)
{
#ifdef COFNIG_TEST_SDIO_INT_LOSS
    int i = 0;
    const int l_loop_times = 5;
#endif
    hcc_bus *hi_bus;

#ifdef _PRE_LINUX_TEST
    struct oal_sched_param_stru param;
#else
    struct sched_param param;
#endif

    hi_bus = (hcc_bus *)data;

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi sched ctrl rx thread high speed");
    param.sched_priority = OAL_BUS_RXDATA_THREAD_PRIORITY;
    oal_set_thread_property(current,
                            OAL_BUS_RX_THREAD_POLICY,
                            &param,
                            MIN_NICE);

    allow_signal(SIGTERM);
    while (oal_likely(!down_interruptible(&hi_bus->rx_sema))) {
        if (oal_unlikely(kthread_should_stop())) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "exit gpio rx thread");
            break;
        }
#ifdef COFNIG_TEST_SDIO_INT_LOSS
        for (i = 0; i < l_loop_times; i++) {
#endif
            /* start to read GPIO interrupt */
            hcc_bus_rx_transfer_lock(hi_bus);
            if (hi_bus->opt_ops->wlan_gpio_rxdata_proc) {
                hi_bus->opt_ops->wlan_gpio_rxdata_proc(hi_bus);
            }
            hcc_bus_rx_transfer_unlock(hi_bus);
#ifdef COFNIG_TEST_SDIO_INT_LOSS
        }
#endif
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "gpio rx data is terminated");
    return OAL_SUCC;
}

uint64_t oal_get_gpio_int_count_para(void)
{
    hcc_bus *hi_bus;

    hi_bus = hcc_get_current_110x_bus();
    if (hi_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hi_bus is null");
        return 0;
    }

    return hi_bus->gpio_int_count;
}

OAL_STATIC void hcc_bus_sched_gpio_task(hcc_bus *pst_bus, int32_t irq)
{
    if (oal_likely(pst_bus->opt_ops->wlan_gpio_handler)) {
        pst_bus->opt_ops->wlan_gpio_handler(pst_bus, irq);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wlan_gpio_handler is NULL, irq:%d, bus type:%s, disable gpio interrupt!",
                             irq, hcc_bus_get_bus_type_str(pst_bus->bus_type));
    }
}

/*
 * Prototype    : hcc_bus_wlan_gpio_irq
 * Description  : wlan irq process function
 * Return Value : OAL_STATIC irqreturn_t
 */
#ifdef _PRE_CONFIG_ARCH_KIRIN_S4_FEATURE
irqreturn_t hcc_bus_wlan_gpio_irq(int32_t irq, void *dev_id)
#else
OAL_STATIC irqreturn_t hcc_bus_wlan_gpio_irq(int32_t irq, void *dev_id)
#endif
{
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = NULL;

    pst_bus_dev = (hcc_bus_dev *)dev_id;
    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev id  is NULL, irq:%d", irq);
        return IRQ_HANDLED;
    }

    pst_bus = hdev_to_hbus(pst_bus_dev);
    if (oal_unlikely(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "curr bus is NULL, irq:%d, bus_dev id:%u", irq, pst_bus_dev->dev_id);
        return IRQ_HANDLED;
    }

    if (oal_likely(pst_bus->opt_ops->wlan_gpio_handler)) {
        hcc_bus_sched_gpio_task(pst_bus, irq);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR,
                             "wlan_gpio_handler is NULL, irq:%d, bus_dev id:%u, disable gpio interrupt!",
                             irq, pst_bus_dev->dev_id);
        oal_wlan_gpio_intr_enable(pst_bus_dev, OAL_FALSE);
    }

    return IRQ_HANDLED;
}

/*
 * Prototype    : oal_register_gpio_intr
 * Description  : initialize sdio pm interface
 */
OAL_STATIC int32_t oal_register_gpio_intr(hcc_bus_dev *pst_bus_dev)
{
    int32_t ret;
    hi110x_board_info *pst_board = get_hi110x_board_info();

    unsigned int wlan_irq = pst_board->wlan_irq;

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wlan gpio irq = %u", wlan_irq);
    if (!wlan_irq) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[BUS][ERROR]wlan irq should't be 0");
        return -OAL_EFAIL;
    }

    pst_bus_dev->ul_wlan_irq = pst_board->wlan_irq;

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    ret = request_irq(wlan_irq, hcc_bus_wlan_gpio_irq,
                      IRQF_TRIGGER_FALLING | IRQF_DISABLED,
                      "wifi_gpio_intr", pst_bus_dev);
#else

#ifdef _PRE_HI_DRV_GPIO
    ret = request_irq(gpio_to_irq(pst_board->wlan_wakeup_host), hcc_bus_wlan_gpio_irq,
                      IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_DISABLED,
                      "wifi_gpio_intr", pst_bus_dev);
#else
#if (KERNEL_VERSION(3, 10, 44) <= LINUX_VERSION_CODE)
    ret = request_irq(wlan_irq, hcc_bus_wlan_gpio_irq,
                      IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_DISABLED,
                      "wifi_gpio_intr", pst_bus_dev);
#else
    /* 低版本内核没有IRQF_NO_SUSPEND标记 */
    ret = request_irq(wlan_irq, hcc_bus_wlan_gpio_irq,
                      IRQF_TRIGGER_RISING | IRQF_DISABLED,
                      "wifi_gpio_intr", pst_bus_dev);
#endif
#endif
#endif
    if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "failed to request wlan irq");

        return ret;
    }

    pst_bus_dev->ul_irq_stat = 0; /* irq enabled default. */

    oal_wlan_gpio_intr_enable(pst_bus_dev, OAL_FALSE);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "success to register sdio gpio intr");
    return OAL_SUCC;
}

/*
 * Prototype    : oal_unregister_gpio_intr
 * Description  : initialize sdio pm interface
 */
OAL_STATIC void oal_unregister_gpio_intr(hcc_bus_dev *pst_bus_dev)
{
    /* disable wlan irq */
    oal_wlan_gpio_intr_enable(pst_bus_dev, OAL_FALSE);

    /* free irq when sdio driver deinit */
#ifndef _PRE_HI_DRV_GPIO
    free_irq(pst_bus_dev->ul_wlan_irq, pst_bus_dev);
#endif
}

/*
 * 函 数 名  : oal_wlan_gpio_intr_enable
 * 功能描述  : 使能/关闭 WLAN GPIO 中断
 * 输入参数  : 1:enable; 0:disenable
 * 返 回 值  : 成功或失败原因
 */
void oal_wlan_gpio_intr_enable(hcc_bus_dev *pst_bus_dev, uint32_t ul_en)
{
    oal_uint flags;

    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_bus_dev->st_irq_lock, &flags);
    if (ul_en) {
        /* 不再支持中断开关嵌套 */
        if (pst_bus_dev->ul_irq_stat) {
#ifdef _PRE_HI_DRV_GPIO
            enable_irq(gpio_to_irq(get_hi110x_board_info()->wlan_wakeup_host));
#else
            enable_irq(pst_bus_dev->ul_wlan_irq);
#endif
            oal_print_hi11xx_log(HI11XX_LOG_DBG, "gpio irq: %d enable", pst_bus_dev->ul_wlan_irq);
            pst_bus_dev->ul_irq_stat = 0;
        }
    } else {
        if (!pst_bus_dev->ul_irq_stat) {
#ifdef _PRE_HI_DRV_GPIO
            disable_irq_nosync(gpio_to_irq(get_hi110x_board_info()->wlan_wakeup_host));
#else
            disable_irq_nosync(pst_bus_dev->ul_wlan_irq);
#endif
            oal_print_hi11xx_log(HI11XX_LOG_DBG, "gpio irq: %d disable nosync", pst_bus_dev->ul_wlan_irq);
            pst_bus_dev->ul_irq_stat = 1;
        }
    }
    oal_spin_unlock_irq_restore(&pst_bus_dev->st_irq_lock, &flags);
}

/*
 * 函 数 名  : oal_flowctrl_gpio_intr_enable
 * 功能描述  : 使能/关闭GPIO流控中断
 * 输入参数  : 1: enabled; 0: disabled
 */
OAL_STATIC void oal_flowctrl_gpio_intr_enable(hcc_bus_dev *pst_bus_dev, oal_bool_enum_uint8 en_enable)
{
    if (en_enable) {
        /* 当前流控状态为关闭 */
        if (!pst_bus_dev->en_flowctrl_irq_stat) {
            /* 使能GPIO流控中断 */
            enable_irq(pst_bus_dev->l_flowctrl_irq);
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "gpio flowctrl irq: %d enable", pst_bus_dev->l_flowctrl_irq);
            /* 同步当前流控状态为使能 */
            pst_bus_dev->en_flowctrl_irq_stat = OAL_TRUE;
        }
    } else {
        /* 当前流控状态为使能 */
        if (pst_bus_dev->en_flowctrl_irq_stat) {
            /* 关闭GPIO流控中断 */
            disable_irq_nosync(pst_bus_dev->l_flowctrl_irq);
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "gpio flowctrl irq: %d disable nosync", pst_bus_dev->l_flowctrl_irq);
            /* 同步当前流控状态为关闭 */
            pst_bus_dev->en_flowctrl_irq_stat = OAL_FALSE;
        }
    }
}

/*
 * 函 数 名  : hcc_bus_flowctrl_gpio_irq
 * 功能描述  : GPIO流控中断回调函数
 */
OAL_STATIC irqreturn_t hcc_bus_flowctrl_gpio_irq(int32_t l_irq, void *p_dev_id)
{
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = NULL;

    pst_bus_dev = (hcc_bus_dev *)p_dev_id;
    if (oal_unlikely(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "dev id  is NULL, irq:%d", l_irq);
        return IRQ_HANDLED;
    }

    pst_bus = hdev_to_hbus(pst_bus_dev);
    if (oal_unlikely(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "curr bus is NULL, irq:%d, bus_dev id:%u", l_irq, pst_bus_dev->dev_id);
        return IRQ_HANDLED;
    }

    if (oal_likely(pst_bus->opt_ops->flowctrl_gpio_handler)) {
        /* 执行GPIO流控逻辑(oal_sdio_gpio_flowctrl_irq) */
        pst_bus->opt_ops->flowctrl_gpio_handler(pst_bus, l_irq);
    } else {
        /* flowctrl_gpio_handler不存在, 关闭GPIO流控功能 */
        oal_print_hi11xx_log(HI11XX_LOG_ERR,
                             "flowctrl_gpio_handler is NULL, irq:%d, bus_dev id:%u, disable gpio interrupt!",
                             l_irq, pst_bus_dev->dev_id);
        oal_flowctrl_gpio_intr_enable(pst_bus_dev, OAL_FALSE);
    }

    return IRQ_HANDLED;
}

/*
 * 函 数 名  : oal_register_gpio_flowctrl_intr
 * 功能描述  : 注册GPIO流控中断
 */
OAL_STATIC int32_t oal_register_gpio_flowctrl_intr(hcc_bus_dev *pst_bus_dev)
{
    int32_t ret = OAL_SUCC;
    hi110x_board_info *pst_board = get_hi110x_board_info();
    uint32_t flowctrl_irq = pst_board->flowctrl_irq;

    if (!flowctrl_irq) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[BUS][ERROR]flowctrl irq should't be 0");
        return OAL_FAIL;
    }

    pst_bus_dev->l_flowctrl_irq = pst_board->flowctrl_irq;

    /* 调用内核接口注册GPIO流控中断, 中断触发时回调hcc_bus_flowctrl_gpio_irq */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    ret = request_irq(flowctrl_irq, hcc_bus_flowctrl_gpio_irq,
                      IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_DISABLED,
                      "wifi_gpio_flowctrl_intr", pst_bus_dev);
#else
    /* 低版本内核没有IRQF_NO_SUSPEND标记 */
    ret = request_irq(flowctrl_irq, hcc_bus_flowctrl_gpio_irq,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_DISABLED,
                      "wifi_gpio_flowctrl_intr", pst_bus_dev);
#endif
    if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "failed to request flowctrl irq, ret code = %d", ret);
        return OAL_FAIL;
    }

    /* 中断状态标记初始化(使能) */
    pst_bus_dev->en_flowctrl_irq_stat = OAL_FALSE;

    /* 初始化GPIO流控中断为使能状态 */
    oal_flowctrl_gpio_intr_enable(pst_bus_dev, OAL_FALSE);
    oal_flowctrl_gpio_intr_enable(pst_bus_dev, OAL_TRUE);

    pst_bus_dev->en_flowctrl_gpio_registered = OAL_TRUE;

    return OAL_SUCC;
}

/*
 * 函 数 名  : oal_unregister_gpio_flowctrl_intr
 * 功能描述  : 注销GPIO流控中断
 */
void oal_unregister_gpio_flowctrl_intr(hcc_bus_dev *pst_bus_dev)
{
    /* 关闭GPIO流控中断 */
    oal_flowctrl_gpio_intr_enable(pst_bus_dev, OAL_FALSE);

    /* 注销GPIO流控中断 */
    free_irq(pst_bus_dev->l_flowctrl_irq, pst_bus_dev);

    pst_bus_dev->en_flowctrl_gpio_registered = OAL_FALSE;
}

/*
 * 函 数 名  : hcc_bus_flowctrl_init
 * 功能描述  : 流控初始化接口, 流控类型来自私有定制化文件
 */
uint32_t hcc_bus_flowctrl_init(uint8_t uc_hcc_flowctrl_type)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(0);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "flowctrl init get 110x dev is null");
        return OAL_FAIL;
    }

    /* 流控类型: 0.SDIO 1.GPIO */
    if (uc_hcc_flowctrl_type == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "flowctrl type = SDIO, already init");
        return OAL_SUCC;
    }

    /* hcc已注册GPIO中断 */
    if (pst_bus_dev->en_flowctrl_gpio_registered) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "flowctrl type = GPIO, already init");
        return OAL_SUCC;
    }

    /* Step1: 申请管脚 */
    if (board_flowctrl_gpio_init() != BOARD_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "request flowctrl gpio failed!");
        return OAL_FAIL;
    }

    /* Step2: 获取管脚对应中断号 */
    board_flowctrl_irq_init();

    /* Step3: 使用中断号注册GPIO中断 */
    if (oal_register_gpio_flowctrl_intr(pst_bus_dev) != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "request flowctrl gpio irq failed!");
        free_board_flowctrl_gpio();
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE void hcc_bus_print_state(uint32_t old_state, uint32_t new_state)
{
    if (old_state != new_state) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "bus state changed, tx[%s=>%s],rx[%s=>%s]",
                             (old_state & OAL_BUS_STATE_TX) ? "on " : "off",
                             (new_state & OAL_BUS_STATE_TX) ? "on " : "off",
                             (old_state & OAL_BUS_STATE_RX) ? "on " : "off",
                             (new_state & OAL_BUS_STATE_RX) ? "on " : "off");
    }
}

/*
 * 函 数 名  : hcc_bus_wakelocks_release_detect
 * 功能描述  : 强行释放wakelock锁
 * 返 回 值  : 成功或失败原因
 */
void hcc_bus_wakelocks_release_detect(hcc_bus *pst_bus)
{
    /* before call this function , please make sure the rx/tx queue is empty and no data transfer!! */
    if (oal_warn_on(!pst_bus)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[E]pst_bus is error can not release wake locks");
        return;
    };

    if (hcc_bus_wakelock_active(pst_bus)) {
#if ((KERNEL_VERSION(2, 6, 37) <= LINUX_VERSION_CODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "[E]We still hold %s   %lu wake locks, Now release all",
                             pst_bus->st_bus_wakelock.st_wakelock.name,
                             pst_bus->st_bus_wakelock.lock_count);
#endif
        declare_dft_trace_key_info("wlan_wakelock_error_hold", OAL_DFT_TRACE_EXCEP);
        pst_bus->st_bus_wakelock.lock_count = 1;
        hcc_bus_wake_unlock(pst_bus);
    }
}

OAL_STATIC int32_t hcc_bus_resource_alloc(hcc_bus *pst_bus)
{
    int32_t ret;
    uint32_t len;
    char *wakelock_name = NULL;
    uint8_t name[200]; /* 因编译原因，暂不支持用const变量定义数组大小 */

    if (oal_warn_on(pst_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus is null,alloc fail ");
        return -OAL_EFAIL;
    };

    ret = snprintf_s(name, OAL_SIZEOF(name), OAL_SIZEOF(name) - 1, "wlan_bus_lock/%s:%u",
                     hcc_bus_get_bus_type_str(pst_bus->bus_type), pst_bus->dev_id);
    if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wakelock name formate err\n");
        return ret;
    }
    len = (uint32_t)OAL_STRLEN(name) + 1;
    wakelock_name = oal_memalloc(len);
    if (wakelock_name == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc wakelock name space failed, len=%u, [%s]", len, name);
        return -OAL_ENOMEM;
    }

    memcpy_s((void *)wakelock_name, len, (const void *)name, len);

    oal_wake_lock_init(&pst_bus->st_bus_wakelock, wakelock_name);

    pst_bus->st_bus_wakelock.debug = g_hcc_bus_wakelock_debug;

    oal_init_completion(&pst_bus->st_device_ready);

    ret = snprintf_s(name, OAL_SIZEOF(name), OAL_SIZEOF(name) - 1, "wlan_bus_rx/%s:%u",
                     hcc_bus_get_bus_type_str(pst_bus->bus_type), pst_bus->dev_id);
    if (ret < 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus name formate err\n");
        oal_free(wakelock_name);
        oal_wake_lock_exit(&pst_bus->st_bus_wakelock);
        return ret;
    }

    /* create thread for gpio rx data in interrupt handler */
    pst_bus->pst_rx_tsk = oal_thread_create(hcc_bus_rxdata_thread,
                                            (hcc_bus *)pst_bus,
                                            &pst_bus->rx_sema,
                                            name,
                                            OAL_BUS_RX_THREAD_POLICY,
                                            OAL_BUS_RXDATA_THREAD_PRIORITY, g_cpu_id);
    if (pst_bus->pst_rx_tsk == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[SDIO][ERROR]failed to create rx data thread");
        oal_free(wakelock_name);
        oal_wake_lock_exit(&pst_bus->st_bus_wakelock);
        return -OAL_EFAIL;
    }
    mutex_init(&pst_bus->rx_transfer_lock);

    oal_atomic_set(&g_wakeup_dev_wait_ack, 0);
    oal_atomic_set(&g_bus_powerup_dev_wait_ack, 0);

    sema_init(&pst_bus->sr_wake_sema, 1);

    /* init exception work */
    oal_spin_lock_init(&pst_bus->bus_excp_lock);
    oal_init_work(&pst_bus->bus_excp_worker, hcc_bus_exception_handler);
    oal_wake_lock_init(&pst_bus->bus_excp_wlock, hcc_bus_get_bus_type_str(pst_bus->bus_type));

    hcc_bus_message_register(pst_bus,
                             D2H_MSG_DEVICE_PANIC,
                             hcc_bus_device_panic_callback,
                             pst_bus);

    hcc_bus_message_register(pst_bus,
                             D2H_MSG_WLAN_READY,
                             hcc_bus_device_init_ready,
                             pst_bus);

    g_hisi_wifi_bus = pst_bus;

    return OAL_SUCC;
}

OAL_STATIC void hcc_bus_resource_free(hcc_bus *pst_bus)
{
    char *name = NULL;

    if (oal_unlikely(pst_bus == NULL)) {
        oal_warn_on(1);
        return;
    }

    if (pst_bus->pst_rx_tsk != NULL) {
        oal_thread_stop(pst_bus->pst_rx_tsk, &pst_bus->rx_sema);
        pst_bus->pst_rx_tsk = NULL;
    }
    mutex_destroy(&pst_bus->rx_transfer_lock);
#if ((KERNEL_VERSION(2, 6, 37) <= LINUX_VERSION_CODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    name = (char *)pst_bus->st_bus_wakelock.st_wakelock.name;
#endif
    oal_wake_lock_exit(&pst_bus->st_bus_wakelock);

    oal_wake_lock_exit(&pst_bus->bus_excp_wlock);
#if ((KERNEL_VERSION(2, 6, 37) <= LINUX_VERSION_CODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    oal_free((void *)name);
#endif
}

OAL_STATIC int32_t hcc_dev_switch_thread_create(hcc_bus_dev *bus_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    int32_t ret;
    struct task_struct *tsk;
    struct sched_param param;

    tsk = kthread_create(hcc_bus_switch_thread, (void *)bus_dev, "switch_bus/%d", bus_dev->dev_id);
    if (oal_is_err_or_null(tsk)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "failed to run thread:switch_bus/%d, task=%p", bus_dev->dev_id, tsk);
        return -OAL_EFAIL;
    }

    bus_dev->switch_bus_req = NULL;

    bus_dev->pst_switch_task = tsk;

    /* set thread priority and schedule policy */
    param.sched_priority = OAL_BUS_SWITCH_THREAD_CREATE_PRIORITY;
    ret = sched_setscheduler(tsk, SCHED_FIFO, &param);
    if (oal_unlikely(ret)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "switch_bus/%d sched_setscheduler failed! ret =%d, prio=%d",
                             bus_dev->dev_id, ret, OAL_BUS_SWITCH_THREAD_CREATE_PRIORITY);
    }

    wake_up_process(tsk);
#endif
    return OAL_SUCC;
}

OAL_STATIC void hcc_dev_switch_thread_destory(hcc_bus_dev *bus_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (bus_dev->pst_switch_task) {
        /* sema_sync can be NULL */
        /* create thread for gpio rx data in interrupt handler */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "stop switch task");
        send_sig(SIGTERM, bus_dev->pst_switch_task, 1);
        kthread_stop(bus_dev->pst_switch_task);
        bus_dev->pst_switch_task = NULL;
    }
#endif
}

int32_t hcc_dev_switch_enable(uint32_t dev_id)
{
    int32_t ret;
    unsigned long flags;

    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (oal_warn_on(pst_bus_dev == NULL)) {
        return -OAL_ENODEV;
    }

    if (pst_bus_dev->bus_switch_enable == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc switch disable");
        return OAL_SUCC;
    }

    oal_spin_lock_irq_save(&pst_bus_dev->st_switch_lock, &flags);

    /* default no switch request */
    pst_bus_dev->switch_bus_req = NULL;

    oal_spin_unlock_irq_restore(&pst_bus_dev->st_switch_lock, &flags);

    ret = hcc_dev_switch_thread_create(pst_bus_dev);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc dev switch enable ret=%d", ret);

    return ret;
}

int32_t hcc_dev_switch_disable(uint32_t dev_id)
{
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (oal_warn_on(pst_bus_dev == NULL)) {
        return -OAL_ENODEV;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc dev switch disable+");
    hcc_dev_switch_thread_destory(pst_bus_dev);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc dev switch disable-");

    return OAL_SUCC;
}

OAL_STATIC void hcc_dev_pps_count_timeout(uintptr_t arg)
{
    hcc_bus_dev *pst_bus_dev = (hcc_bus_dev *)arg;
    if (oal_warn_on(pst_bus_dev == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus dev is null");
        return;
    }

    if (!hcc_bus_auto_sched_is_support(pst_bus_dev->dev_id)) {
        return;
    }

    hcc_bus_performance_core_schedule(pst_bus_dev->dev_id);

    oal_timer_start(&pst_bus_dev->bus_pps_timer, HCC_BUS_PPS_COUNT_TIMEOUT);
}

OAL_STATIC int32_t hcc_get_pcie_switch_flag(void)
{
    hcc_bus_dev *pst_bus_dev;

    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);
    if (pst_bus_dev == NULL) {
        return 0;
    }

    return pst_bus_dev->pcie_bugfix_enable;
}

OAL_STATIC int32_t hcc_set_pcie_switch_flag(int32_t flag)
{
    hcc_bus_dev *pst_bus_dev;

    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);
    if (pst_bus_dev == NULL) {
        return -OAL_ENODEV;
    }

    pst_bus_dev->pcie_bugfix_enable = (flag == 0) ? 0 : 1;

    return 0;
}

OAL_STATIC void hcc_dev_exit(void)
{
    int32_t i;
    hcc_bus_dev *pst_bus_dev = NULL;

    for (i = 0; i < (int32_t)oal_array_size(g_bus_dev_res); i++) {
        pst_bus_dev = &g_bus_dev_res[i];

        hcc_bus_performance_core_exit(pst_bus_dev->dev_id);

        if (pst_bus_dev->hcc != NULL) {
            hcc_dev_switch_thread_destory(pst_bus_dev);
            hcc_module_exit(pst_bus_dev->hcc);
            pst_bus_dev->hcc = NULL;
        }
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_unregister_notifier(&g_hcc_bus_pm_notifier);
#endif
}

OAL_STATIC void hcc_bus_config_init(void)
{
    char buff[HCC_BUS_INI_BUFF_SIZE]; /* 因编译原因，暂不支持用const变量定义数组大小 */

#ifdef _PRE_SDIO_TAE_DEBUG
    memset_s(buff, sizeof(buff), 0, sizeof(buff));
    if (get_cust_conf_string(INI_MODU_PLAT, "sdio_tae_debug", buff, sizeof(buff) - 1) == INI_SUCC) {
        if (!oal_strncmp("enable", buff, OAL_STRLEN("enable"))) {
            g_sdio_tae_debug_enable = 1;
        } else if (!strncmp("disable", buff, OAL_STRLEN("disable"))) {
            g_sdio_tae_debug_enable = 0;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "unkdown sdio_tae_debug ini:%s", buff);
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "sdio_tae_debug %s", g_sdio_tae_debug_enable ? "enable" : "disable");
    }
#endif
    memset_s(buff, sizeof(buff), 0, sizeof(buff));
    if (get_cust_conf_string(INI_MODU_PLAT, "g_hcc_exception_enable", buff, sizeof(buff) - 1) == INI_SUCC) {
        if (!oal_strncmp("enable", buff, OAL_STRLEN("enable"))) {
            g_hcc_exception_enable = 1;
        } else if (!strncmp("disable", buff, OAL_STRLEN("disable"))) {
            g_hcc_exception_enable = 0;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_WARN, "g_hcc_exception_enable:%s", buff);
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_exception_enable %s", g_hcc_exception_enable ? "enable" : "disable");
    }
}

int32_t hcc_dev_init(void)
{
    int32_t i;
    int32_t ret;
    hcc_bus_dev *pst_bus_dev = NULL;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc_dev_init we had %d devs", (int32_t)oal_array_size(g_bus_dev_res));

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_register_notifier(&g_hcc_bus_pm_notifier);
#endif

    for (i = 0; i < (int32_t)oal_array_size(g_bus_dev_res); i++) {
        pst_bus_dev = &g_bus_dev_res[i];
        ret = hcc_select_bus(pst_bus_dev->dev_id, pst_bus_dev->init_bus_type);
        if (ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus dev %s select bus failed, ret=%d", pst_bus_dev->name, ret);
            goto hcc_init_fail;
        }

        pst_bus_dev->hcc = hcc_module_init(pst_bus_dev);
        if (pst_bus_dev->hcc == NULL) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc hcc failed!");
            goto hcc_init_fail;
        }

        hcc_bus_performance_core_init(pst_bus_dev->dev_id);

        oal_init_completion(&pst_bus_dev->st_switch_powerup_ready);
        oal_init_completion(&pst_bus_dev->st_switch_powerdown_ready);
        oal_wait_queue_init_head(&pst_bus_dev->st_swtich_ack_wq);
        hcc_rx_register(pst_bus_dev->hcc, HCC_ACTION_TYPE_BUS, hcc_bus_rx_response, NULL);

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hcc init dev %d succ", pst_bus_dev->dev_id);
    }

    hcc_bus_config_init();

    return OAL_SUCC;

hcc_init_fail:
    hcc_dev_exit();
    return -OAL_EFAIL;
}

#ifdef _PRE_SUSPORT_OEMINFO
void resume_wlan_wakeup_host_gpio(void)
{
    int32 ret;
    hi110x_board_info *board = get_hi110x_board_info();
    hcc_bus_dev *pst_bus_dev = NULL;
    uint32_t dev_id = HCC_CHIP_110X_DEV;
    uint32 wlan_irq;

    pst_bus_dev = hcc_get_bus_dev(dev_id);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus_dev get fail:\n");
        return;
    }
    pst_bus_dev->ul_wlan_irq = board->wlan_irq;

    oal_wlan_gpio_intr_enable(pst_bus_dev, OAL_FALSE);
    free_irq(pst_bus_dev->ul_wlan_irq, pst_bus_dev);
    gpio_free(g_st_board_info.wlan_wakeup_host);

    ret = gpio_request(g_st_board_info.wlan_wakeup_host, PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
    if (ret == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "req gpio ok wl_wakeup_h = %d\n",
            g_st_board_info.wlan_wakeup_host);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "req gpio fail wl_wakeup_h = %d\n",
            g_st_board_info.wlan_wakeup_host);
       return;
    }
    gpio_direction_input(g_st_board_info.wlan_wakeup_host);
    wlan_irq = gpio_to_irq(g_st_board_info.wlan_wakeup_host);
    ret = request_irq(wlan_irq, hcc_bus_wlan_gpio_irq,
        IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_DISABLED,
        "wifi_gpio_intr", pst_bus_dev);
    if (ret != 0) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "ret = %d\n", ret);
    }
    disable_irq_nosync(wlan_irq);
}
#endif

OAL_STATIC int32_t oal_load_dev(void)
{
    int32_t load_sdio;
    int32_t load_pcie;

    load_sdio = oal_wifi_platform_load_sdio();
    if (load_sdio != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "load sdio failed ret=%d", load_sdio);
        return load_sdio;
    }

    load_pcie = oal_wifi_platform_load_pcie();
    if (load_pcie != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "load pcie failed ret=%d", load_pcie);
        oal_wifi_platform_unload_sdio();
        return load_pcie;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_load_dev_retry(void)
{
    int32_t load_sdio;
    int32_t load_pcie;
    hcc_bus_dev *pst_bus_dev;

    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);
    if (pst_bus_dev == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hcc_get_bus_dev fail!\n");
        return -OAL_ENODEV;
    }

    load_pcie = oal_wifi_platform_load_pcie();
    if (load_pcie == OAL_SUCC) {
        return OAL_SUCC;
    }

    /* pcie 枚举失败，尝试使用sdio */
    load_sdio = oal_wifi_platform_load_sdio();
    if (load_sdio == OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi bus pcie fail, sdio succ");
        hcc_set_pcie_switch_flag(0);
        pst_bus_dev->init_bus_type = HCC_BUS_SDIO;
#ifdef CONFIG_HUAWEI_DSM
        hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_PCIE_SWITCH_SDIO_SUCC,
            "probe:pcie switch to sdio succ\n");
#endif
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "hisi wifi bus pcie and sdio all fail");
#ifdef CONFIG_HUAWEI_DSM
        hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_PCIE_SWITCH_SDIO_FAIL,
            "probe:pcie switch to sdio fail\n");
#endif
    }

    return load_sdio;
}

/* 枚举接口设备 */
int32_t oal_wifi_platform_load_dev(void)
{
    if (hcc_get_pcie_switch_flag() != 1) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi bus pcie switch disable");
        return oal_load_dev();
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "hisi wifi bus pcie switch enable");
        return oal_load_dev_retry();
    }
}

/* 卸载接口设备 */
void oal_wifi_platform_unload_dev(void)
{
    oal_wifi_platform_unload_pcie();
    oal_wifi_platform_unload_sdio();
}

#else
int32_t hcc_get_pcie_switch_flag(void)
{
    return OAL_SUCC;
}

int32_t hcc_set_pcie_switch_flag(int32_t flag)
{
    return OAL_SUCC;
}
void hcc_dev_exit(void)
{
}

int32_t hcc_dev_init(void)
{
    return OAL_SUCC;
}

int32_t hcc_bus_resource_alloc(hcc_bus *pst_bus)
{
    oal_reference(pst_bus);
    return OAL_SUCC;
}

void hcc_bus_resource_free(hcc_bus *pst_bus)
{
    oal_reference(pst_bus);
}

int32_t oal_register_gpio_flowctrl_intr(hcc_bus_dev *pst_bus_dev)
{
    oal_reference(pst_bus_dev);
    return OAL_SUCC;
}

int32_t oal_register_gpio_intr(hcc_bus_dev *pst_bus_dev)
{
    oal_reference(pst_bus_dev);
    return OAL_SUCC;
}

void oal_unregister_gpio_flowctrl_intr(hcc_bus_dev *pst_bus_dev)
{
    oal_reference(pst_bus_dev);
}

void oal_unregister_gpio_intr(hcc_bus_dev *pst_bus_dev)
{
    oal_reference(pst_bus_dev);
}

void hcc_bus_exception_handler(oal_work_stru *work)
{
    oal_reference(work);
}

int32_t oal_trigger_bus_exception(hcc_bus *hi_bus, int32_t is_sync)
{
    oal_reference(hi_bus);
    oal_reference(is_sync);
    return OAL_SUCC;
}

/* 枚举接口设备 */
int32_t oal_wifi_platform_load_dev(void)
{
    return OAL_SUCC;
}

/* 卸载接口设备 */
void oal_wifi_platform_unload_dev(void)
{
}
#endif

uint32_t hcc_bus_dump_mem_check(hcc_bus *hi_bus)
{
    oal_reference(hi_bus);
    return !!g_bus_dump_mem_flag;
}
/*lint -e19*/
oal_module_symbol(hcc_bus_dump_mem_check);
/*lint +e19*/
/*
 * Prototype    : hcc_bus_message_register
 * Description  : msg irq
 * Input        : struct oal_sdio *hi_sdio
 */
int32_t hcc_bus_message_register(hcc_bus* hi_bus,
                                 uint8_t msg,
                                 hcc_bus_msg_rx cb,
                                 void* data)
{
    if (hi_bus == NULL || msg >= D2H_MSG_COUNT) {
        return -OAL_EFAIL;
    }
    hi_bus->msg[msg].msg_rx = cb;
    hi_bus->msg[msg].data = data;
    return OAL_SUCC;
}

/*
 * Prototype    : oal_sdio_message_unregister
 * Description  : msg irq
 * Input        : struct oal_sdio *hi_sdio
 */
void hcc_bus_message_unregister(hcc_bus* hi_bus, uint8_t msg)
{
    if (hi_bus == NULL || msg >= D2H_MSG_COUNT) {
        return;
    }
    hi_bus->msg[msg].msg_rx = NULL;
    hi_bus->msg[msg].data = NULL;
}

int32_t hcc_bus_transfer_rx_register(hcc_bus *hi_bus, void *data, hcc_bus_data_rx rx)
{
    if (oal_warn_on(hi_bus == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus is null, register rx bus fail\n");
        return OAL_FAIL;
    }
    hi_bus->bus_ops.rx = rx;
    hi_bus->bus_ops_data = data;
    return OAL_SUCC;
}

/*
 * Prototype    : oal_sdio_transfer_rx_unregister
 * Description  : msg irq
 * Input        : hisdio_rx rx
 */
void hcc_bus_transfer_rx_unregister(hcc_bus *hi_bus)
{
    if (oal_unlikely(hi_bus == NULL)) {
        oal_warn_on(1);
        return;
    }
    hi_bus->bus_ops.rx = NULL;
    hi_bus->bus_ops_data = NULL;
}

int32_t hcc_transfer_rx_register(struct hcc_handler *hcc, void *data, hcc_bus_data_rx rx)
{
    int32_t flag = 0;
    int32_t ret = OAL_SUCC;
    oal_uint irqsave;
    oal_dlist_head_stru *pst_entry = NULL;
    hcc_bus *pst_bus_tmp = NULL;
    if (oal_unlikely(hcc == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }
    oal_spin_lock_irq_save(&g_hcc_bus_res_lock, &irqsave);
    oal_dlist_search_for_each(pst_entry, &g_hcc_bus_res_hdr)
    {
        pst_bus_tmp = oal_dlist_get_entry(pst_entry, hcc_bus, list);
        if (pst_bus_tmp->bus_dev == hcc->bus_dev) {
            /* 注册在当前设备上的所有BUS 都需要注册 */
            ret = hcc_bus_transfer_rx_register(pst_bus_tmp, data, rx);
            if (ret != OAL_SUCC) {
                oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "register rx bus %d failed, ret=%d", pst_bus_tmp->bus_id, ret);
                return ret;
            }
            flag++;
        }
    }
    oal_spin_unlock_irq_restore(&g_hcc_bus_res_lock, &irqsave);

    if (flag) {
        return OAL_SUCC;
    } else {
        return -OAL_ENODEV;
    }
}

uint32_t hcc_get_max_trans_size(struct hcc_handler *hcc)
{
    if (oal_warn_on(hcc == NULL)) {
        return 0;
    }

    if (oal_warn_on(hcc->bus_dev == NULL)) {
        return 0;
    }

    return hcc_bus_get_max_trans_size(hcc->bus_dev->cur_bus);
}

/*
 * Prototype    :oal_thread_create
 * Description  : create thread
 */
struct task_struct* oal_thread_create(int (*threadfn)(void* data),
                                      void* data,
                                      struct semaphore* sema_sync,
                                      const char* namefmt,
                                      uint32_t policy,
                                      int32_t prio,
                                      int32_t cpuid)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    int ret;
    struct task_struct *tsk = NULL;
    struct sched_param param;

    /* create thread for gpio rx data in interrupt handler */
    if (sema_sync != NULL) {
        sema_init(sema_sync, 0);
    }

    tsk = kthread_create(threadfn, data, "%s", namefmt);
    if (oal_is_err_or_null(tsk)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "failed to run thread:%s", namefmt);
        return NULL;
    }

    /* set thread priority and schedule policy */
    param.sched_priority = prio;
    ret = sched_setscheduler(tsk, policy, &param);
    if (oal_unlikely(ret)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "%s sched_setscheduler failed! ret =%d, prio=%d", namefmt, ret, prio);
    }
    if (cpuid >= 0) {
        oal_kthread_bind(tsk, cpuid);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "did not bind cpu...");
    }
    wake_up_process(tsk);
    return tsk;
#else
    return NULL;
#endif
}

/*lint -e19*/
oal_module_symbol(oal_thread_create);
/*lint +e19*/
/*
 * Prototype    : oal_thread_stop
 * Description  : create thread
 */
void oal_thread_stop(struct task_struct *tsk,
                         struct semaphore *sema_sync)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    /* sema_sync can be NULL */
    /* create thread for gpio rx data in interrupt handler */
    send_sig(SIGTERM, tsk, 1);
    kthread_stop(tsk);
#endif
}
/*lint -e19*/
oal_module_symbol(oal_thread_stop);
/*lint +e19*/
void oal_set_thread_property(struct task_struct *p, int policy,
#ifdef _PRE_LINUX_TEST
                             const struct oal_sched_param_stru *param,
#else
                             const struct sched_param *param,
#endif
                             long nice)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (oal_warn_on(p == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "param set thread scheduler policy fail task_struct is null");
        return;
    };

    if (param == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "param set thread scheduler policy fail, param in null");
        return;
    };

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "set thread scheduler policy %d", policy);

    if (sched_setscheduler(p, policy, (struct sched_param *)param)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "[Error]set scheduler failed! %d", policy);
    }

    if (policy != SCHED_FIFO && policy != SCHED_RR) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "set thread scheduler nice %ld", nice);
        set_user_nice(p, nice);
    }
#endif
}

/*lint -e19*/
oal_module_symbol(oal_set_thread_property);
/*lint +e19*/
/* Try to dump device mem, controlled by flag sdio_dump_mem_flag */
void hcc_bus_try_to_dump_device_mem(hcc_bus *hi_bus, int32_t is_sync)
{
    if (oal_warn_on(hi_bus == NULL)) {
        return;
    }

    if (!hcc_bus_dump_mem_check(hi_bus)) {
        return;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "Try to dump device mem!");
    oal_trigger_bus_exception(hi_bus, is_sync);
}
/*lint -e19*/
oal_module_symbol(hcc_bus_try_to_dump_device_mem);
oal_module_symbol(hcc_bus_transfer_rx_register);
oal_module_symbol(hcc_bus_flowctrl_init);

/*lint +e19*/
