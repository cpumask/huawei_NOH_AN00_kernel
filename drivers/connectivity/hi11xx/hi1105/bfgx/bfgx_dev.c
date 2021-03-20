

/* Include Head file */
#include "bfgx_dev.h"

#include <linux/platform_device.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/tty.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/reboot.h>
#include <linux/fs.h>
#include <linux/version.h>
#if ((defined CONFIG_LOG_EXCEPTION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)) && \
    (!defined PLATFORM_DEBUG_ENABLE))
#include <log/log_usertype.h>
#endif

#include "board.h"
#include "plat_debug.h"
#include "plat_uart.h"
#include "plat_pm.h"
#include "bfgx_user_ctrl.h"
#include "bfgx_exception_rst.h"
#include "plat_firmware.h"
#include "plat_cali.h"
#include "platform_common_clk.h"
#include "securec.h"
#include "oal_ext_if.h"
#include "bfgx_gnss.h"
#include "bfgx_data_parse.h"

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
#include "wireless_patch.h"
#endif

#ifdef CONFIG_HI110X_GPS_REFCLK
#include "gps_refclk_src_3.h"
#endif

/*
 * This references is the per-PS platform device in the arch/arm/
 * board-xx.c file.
 */
STATIC struct platform_device *g_hw_ps_device = NULL;
STATIC atomic_t g_debug_cnt = ATOMIC_INIT(0);

STATIC uint32 g_bfgx_open_cmd[BFGX_BUTT] = {
    SYS_CFG_OPEN_BT,
    SYS_CFG_OPEN_FM,
    SYS_CFG_OPEN_GNSS,
    SYS_CFG_OPEN_IR,
#ifdef HAVE_HISI_NFC
    SYS_CFG_OPEN_NFC,
#endif
};

STATIC uint32 g_bfgx_close_cmd[BFGX_BUTT] = {
    SYS_CFG_CLOSE_BT,
    SYS_CFG_CLOSE_FM,
    SYS_CFG_CLOSE_GNSS,
    SYS_CFG_CLOSE_IR,
#ifdef HAVE_HISI_NFC
    SYS_CFG_CLOSE_NFC,
#endif
};

STATIC uint32 g_bfgx_open_cmd_timeout[BFGX_BUTT] = {
    WAIT_BT_OPEN_TIME,
    WAIT_FM_OPEN_TIME,
    WAIT_GNSS_OPEN_TIME,
    WAIT_IR_OPEN_TIME,
    WAIT_NFC_OPEN_TIME,
};

STATIC uint32 g_bfgx_close_cmd_timeout[BFGX_BUTT] = {
    WAIT_BT_CLOSE_TIME,
    WAIT_FM_CLOSE_TIME,
    WAIT_GNSS_CLOSE_TIME,
    WAIT_IR_CLOSE_TIME,
    WAIT_NFC_CLOSE_TIME,
};

const uint8 *g_bfgx_subsys_name[BFGX_BUTT] = {
    "BT",
    "FM",
    "GNSS",
    "IR",
    "NFC",
};

#ifdef _PRE_CONFIG_HISI_110X_BLUEZ   /*Linux BlueZ, for pc linux */
struct hbt_recv_pkt {
    uint8  type;    /* Packet type */
    uint8  hlen;    /* Header length */
    uint8  loff;    /* Data length offset in header */
    uint8  lsize;   /* Data length field size */
    uint16 maxlen;  /* Max overall packet length */
    int (*recv)(struct hci_dev *hdev, struct sk_buff *skb);
};

#define HBT_RECV_ACL \
    .type = HCI_ACLDATA_PKT, \
    .hlen = HCI_ACL_HDR_SIZE, \
    .loff = 2, \
    .lsize = 2, \
    .maxlen = HCI_MAX_FRAME_SIZE

#define HBT_RECV_SCO \
    .type = HCI_SCODATA_PKT, \
    .hlen = HCI_SCO_HDR_SIZE, \
    .loff = 2, \
    .lsize = 1, \
    .maxlen = HCI_MAX_SCO_SIZE

#define HBT_RECV_EVENT \
    .type = HCI_EVENT_PKT, \
    .hlen = HCI_EVENT_HDR_SIZE, \
    .loff = 1, \
    .lsize = 1, \
    .maxlen = HCI_MAX_EVENT_SIZE

STATIC const struct hbt_recv_pkt g_hisi_recv_pkts[] = {
    { HBT_RECV_ACL,      .recv = hci_recv_frame },
    { HBT_RECV_SCO,      .recv = hci_recv_frame },
    { HBT_RECV_EVENT,    .recv = hci_recv_frame },
};
#endif

STATIC struct bt_data_combination g_bt_data_combination = {0};

uint32 g_gnss_me_thread_status = DEV_THREAD_EXIT;
uint32 g_gnss_lppe_thread_status = DEV_THREAD_EXIT;
atomic_t g_ir_only_mode = ATOMIC_INIT(0);

STATIC bool g_device_log_status = false;
void ps_set_device_log_status(bool status)
{
    g_device_log_status = status;
}
bool ps_is_device_log_enable(void)
{
#if ((defined CONFIG_LOG_EXCEPTION) && \
    (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)) && \
    (!defined PLATFORM_DEBUG_ENABLE))
    unsigned int log_usertype = get_log_usertype();
    bool status = ((log_usertype > COMMERCIAL_USER) && (log_usertype < OVERSEA_COMMERCIAL_USER));
    status = (status || g_device_log_status);
    return status;
#else
    return true;
#endif
}

/*
 * Prototype    : ps_get_plat_reference
 * Description  : reference the plat's dat,This references the per-PS
 *                  platform device in the arch/arm/board-xx.c file.
 */
int32 ps_get_plat_reference(struct ps_plat_s **plat_data)
{
    struct platform_device *pdev = NULL;
    struct ps_plat_s *ps_plat_d = NULL;

    pdev = g_hw_ps_device;
    if (pdev == NULL) {
        *plat_data = NULL;
        ps_print_err("%s pdev is NULL\n", __func__);
        return FAILURE;
    }

    ps_plat_d = dev_get_drvdata(&pdev->dev);
    *plat_data = ps_plat_d;

    return SUCCESS;
}

/*
 * Prototype    : ps_get_core_reference
 * Description  : reference the core's data,This references the per-PS
 *                platform device in the arch/xx/board-xx.c file..
 */
int32 ps_get_core_reference(struct ps_core_s **core_data, int type)
{
    struct platform_device *pdev = NULL;
    struct ps_plat_s *ps_plat_d = NULL;

    if (type == BUART) {
        pdev = g_hw_ps_device;
    } else if (type == GUART) {
        pdev = get_me_platform_device();
    } else {
        ps_print_err("type %d is error\n", type);
    }

    if (pdev == NULL) {
        *core_data = NULL;
        ps_print_err("%s pdev is NULL\n", __func__);
        return FAILURE;
    }

    ps_plat_d = dev_get_drvdata(&pdev->dev);
    if (ps_plat_d == NULL) {
        ps_print_err("ps_plat_d is NULL\n");
        return FAILURE;
    }

    *core_data = ps_plat_d->core_data;

    return SUCCESS;
}

/*
 * Prototype    : ps_chk_bfg_active
 * Description  : to chk wether or not bfg active
 */
bool ps_chk_bfg_active(struct ps_core_s *ps_core_d)
{
    int32 i = 0;
    for (i = 0; i < BFGX_BUTT; i++) {
        if (atomic_read(&ps_core_d->bfgx_info[i].subsys_state) != POWER_STATE_SHUTDOWN) {
            return true;
        }
    }

    return false;
}

/*
 * Prototype    : ps_bfg_subsys_active
 * Description  : check bfgx subsys active or no
 */
bool ps_bfg_subsys_active(uint32 subsys)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return false;
    }

    if (subsys >= BFGX_BUTT) {
        ps_print_err("subsys id[%d] exceed limit\n", subsys);
        return false;
    }

    if (atomic_read(&ps_core_d->bfgx_info[subsys].subsys_state) == POWER_STATE_SHUTDOWN) {
        ps_print_info("subsys %s is shutdown\n", g_bfgx_subsys_name[subsys]);
        return  false;
    }

    return true;
}


/* only gnss is open and it agree to sleep */
bool ps_chk_only_gnss_and_cldslp(struct ps_core_s *ps_core_d)
{
    struct pm_drv_data *pm_data = ps_core_d->pm_data;
    if ((atomic_read(&ps_core_d->bfgx_info[BFGX_BT].subsys_state) == POWER_STATE_SHUTDOWN) &&
        (atomic_read(&ps_core_d->bfgx_info[BFGX_FM].subsys_state) == POWER_STATE_SHUTDOWN) &&
        (atomic_read(&ps_core_d->bfgx_info[BFGX_IR].subsys_state) == POWER_STATE_SHUTDOWN) &&
        (atomic_read(&ps_core_d->bfgx_info[BFGX_NFC].subsys_state) == POWER_STATE_SHUTDOWN) &&
        (atomic_read(&ps_core_d->bfgx_info[BFGX_GNSS].subsys_state) == POWER_STATE_OPEN) &&
        (atomic_read(&pm_data->gnss_sleep_flag) == GNSS_AGREE_SLEEP) &&
        (pm_data->bfgx_dev_state == BFGX_ACTIVE)) {
        return true;
    }
    return false;
}

bool ps_chk_tx_queue_empty(struct ps_core_s *ps_core_d)
{
    PS_PRINT_FUNCTION_NAME;
    if (ps_core_d == NULL) {
        ps_print_err("ps_core_d is NULL");
        return true;
    }

    if (skb_queue_empty(&ps_core_d->tx_urgent_seq) &&
        skb_queue_empty(&ps_core_d->tx_high_seq) &&
        skb_queue_empty(&ps_core_d->tx_low_seq)) {
        return true;
    }
    return false;
}

/*
 * Prototype    : ps_alloc_skb
 * Description  : allocate mem for new skb
 */
struct sk_buff *ps_alloc_skb(uint16 len)
{
    struct sk_buff *skb = NULL;

    PS_PRINT_FUNCTION_NAME;

    skb = alloc_skb(len, GFP_KERNEL);
    if (skb == NULL) {
        ps_print_warning("can't allocate mem for new skb, len=%d\n", len);
        return NULL;
    }

    skb_put(skb, len);

    return skb;
}

/*
 * Prototype    : ps_kfree_skb
 * Description  : when close a function, kfree skb
 */
void ps_kfree_skb(struct ps_core_s *ps_core_d, uint8 type)
{
    struct sk_buff *skb = NULL;

    PS_PRINT_FUNCTION_NAME;
    if (ps_core_d == NULL) {
        ps_print_err("ps_core_d is NULL");
        return;
    }

    while ((skb = ps_skb_dequeue(ps_core_d, type))) {
        kfree_skb(skb);
    }

    switch (type) {
        case TX_URGENT_QUEUE:
            skb_queue_purge(&ps_core_d->tx_urgent_seq);
            break;
        case TX_HIGH_QUEUE:
            skb_queue_purge(&ps_core_d->tx_high_seq);
            break;
        case TX_LOW_QUEUE:
            skb_queue_purge(&ps_core_d->tx_low_seq);
            break;
        case RX_GNSS_QUEUE:
            skb_queue_purge(&ps_core_d->bfgx_info[BFGX_GNSS].rx_queue);
            break;
        case RX_FM_QUEUE:
            skb_queue_purge(&ps_core_d->bfgx_info[BFGX_FM].rx_queue);
            break;
        case RX_BT_QUEUE:
            skb_queue_purge(&ps_core_d->bfgx_info[BFGX_BT].rx_queue);
            break;
        case RX_DBG_QUEUE:
            skb_queue_purge(&ps_core_d->rx_dbg_seq);
            break;
        case RX_NFC_QUEUE:
            skb_queue_purge(&ps_core_d->bfgx_info[BFGX_NFC].rx_queue);
            break;
        case RX_IR_QUEUE:
            skb_queue_purge(&ps_core_d->bfgx_info[BFGX_IR].rx_queue);
            break;
        default:
            ps_print_err("queue type is error, type=%d\n", type);
            break;
    }
    return;
}

/*
 * Prototype    : ps_restore_skbqueue
 * Description  : when err and restore skb to seq function.
 */
int32 ps_restore_skbqueue(struct ps_core_s *ps_core_d, struct sk_buff *skb, uint8 type)
{
    PS_PRINT_FUNCTION_NAME;

    if (unlikely((skb == NULL) || (ps_core_d == NULL))) {
        ps_print_err(" skb or ps_core_d is NULL\n");
        return -EINVAL;
    }

    switch (type) {
        case RX_GNSS_QUEUE:
            skb_queue_head(&ps_core_d->bfgx_info[BFGX_GNSS].rx_queue, skb);
            break;
        case RX_FM_QUEUE:
            skb_queue_head(&ps_core_d->bfgx_info[BFGX_FM].rx_queue, skb);
            break;
        case RX_BT_QUEUE:
            skb_queue_head(&ps_core_d->bfgx_info[BFGX_BT].rx_queue, skb);
            break;
        case RX_IR_QUEUE:
            skb_queue_head(&ps_core_d->bfgx_info[BFGX_IR].rx_queue, skb);
            break;
        case RX_NFC_QUEUE:
            skb_queue_head(&ps_core_d->bfgx_info[BFGX_NFC].rx_queue, skb);
            break;
        case RX_DBG_QUEUE:
            skb_queue_head(&ps_core_d->rx_dbg_seq, skb);
            break;

        default:
            ps_print_err("queue type is error, type=%d\n", type);
            break;
    }

    return 0;
}

/* prepare to visit dev_node */
int32 prepare_to_visit_node(struct ps_core_s *ps_core_d)
{
    struct pm_drv_data *pm_data = NULL;
    uint8 uart_ready;
    uint64 flags;

    pm_data = ps_core_d->pm_data;
    if (unlikely(pm_data == NULL)) {
        ps_print_err("pm_data is NULL!\n");
        return -EFAULT;
    }

    /* lock wake_lock */
    pm_data->bfg_wake_lock(pm_data);

    /* try to wake up device */
    spin_lock_irqsave(&pm_data->uart_state_spinlock, flags);
    atomic_inc(&ps_core_d->node_visit_flag); /* mark someone is visiting dev node */
    uart_ready = pm_data->bfgx_uart_state_get(pm_data);
    spin_unlock_irqrestore(&pm_data->uart_state_spinlock, flags);

    if (uart_ready == UART_NOT_READY) {
        if (host_wkup_dev(pm_data) != 0) {
            ps_print_err("[%s]wkup device FAILED!\n", index2name(pm_data->index));
            atomic_dec(&ps_core_d->node_visit_flag);
            return -EIO;
        }
    }
    return 0;
}

/* we should do something before exit from visiting dev_node */
int32 post_to_visit_node(struct ps_core_s *ps_core_d)
{
    atomic_dec(&ps_core_d->node_visit_flag);

    return 0;
}

int32 alloc_seperted_rx_buf(struct ps_core_s *ps_core_d, uint8 subsys, uint32 len, uint8 alloctype)
{
    struct bfgx_sepreted_rx_st *pst_sepreted_data = NULL;
    uint8 *p_rx_buf = NULL;

    if (subsys == BFGX_BT) {
        ps_print_dbg("%s no sepreted buf\n", g_bfgx_subsys_name[subsys]);
        return 0;
    }

    pst_sepreted_data = &ps_core_d->bfgx_info[subsys].sepreted_rx;

    if (alloctype == KZALLOC) {
        p_rx_buf = kzalloc(len, GFP_KERNEL);
    } else if (alloctype == VMALLOC) {
        p_rx_buf = vmalloc(len);
    }

    if (p_rx_buf == NULL) {
        ps_print_err("alloc failed! subsys=%d, len=%d\n", subsys, len);
        return -ENOMEM;
    }

    spin_lock(&pst_sepreted_data->sepreted_rx_lock);
    pst_sepreted_data->rx_prev_seq = RX_SEQ_NULL;
    pst_sepreted_data->rx_buf_all_len = 0;
    pst_sepreted_data->rx_buf_ptr = p_rx_buf;
    pst_sepreted_data->rx_buf_org_ptr = p_rx_buf;
    spin_unlock(&pst_sepreted_data->sepreted_rx_lock);

    return 0;
}

void free_seperted_rx_buf(struct ps_core_s *ps_core_d, uint8 subsys, uint8 alloctype)
{
    struct bfgx_sepreted_rx_st *pst_sepreted_data = NULL;
    uint8 *buf_ptr = NULL;

    if (subsys == BFGX_BT) {
        ps_print_info("%s no sepreted buf\n", g_bfgx_subsys_name[subsys]);
        return;
    }

    pst_sepreted_data = &ps_core_d->bfgx_info[subsys].sepreted_rx;

    buf_ptr = pst_sepreted_data->rx_buf_org_ptr;
    spin_lock(&pst_sepreted_data->sepreted_rx_lock);
    pst_sepreted_data->rx_prev_seq = RX_SEQ_NULL;
    pst_sepreted_data->rx_buf_all_len = 0;
    pst_sepreted_data->rx_buf_ptr = NULL;
    pst_sepreted_data->rx_buf_org_ptr = NULL;
    spin_unlock(&pst_sepreted_data->sepreted_rx_lock);
    if (buf_ptr != NULL) {
        if (alloctype == KZALLOC) {
            kfree(buf_ptr);
        } else if (alloctype == VMALLOC) {
            vfree(buf_ptr);
        }
    }

    return;
}

int32 bfgx_open_fail_process(uint8 subsys, int32 error)
{
    struct ps_core_s *ps_core_d = NULL;

    if ((subsys >= BFGX_BUTT) || (error >= BFGX_POWER_ENUM_BUTT)) {
        ps_print_err("subsys or errorno is error, subsys=[%d], errno = [%d]\n",
                     subsys, error);
        return BFGX_POWER_FAILED;
    }

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return BFGX_POWER_FAILED;
    }

    ps_print_info("bfgx open fail, type=[%d]\n", error);

    (void)ssi_dump_err_regs(SSI_ERR_BFGX_OPEN_FAIL);

    switch (error) {
        case BFGX_POWER_PULL_POWER_GPIO_FAIL:
        case BFGX_POWER_TTY_OPEN_FAIL:
        case BFGX_POWER_TTY_FLOW_ENABLE_FAIL:
            break;

        case BFGX_POWER_WIFI_DERESET_BCPU_FAIL:
        case BFGX_POWER_WIFI_ON_BOOT_UP_FAIL:
            if (plat_power_fail_exception_info_set(SUBSYS_BFGX, subsys, BFGX_POWERON_FAIL) == BFGX_POWER_SUCCESS) {
                bfgx_system_reset();
                plat_power_fail_process_done();
            } else {
                ps_print_err("bfgx power fail, set exception info fail\n");
            }

            ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, subsys, BFG_POWER_GPIO_DOWN);
            break;

        case BFGX_POWER_WIFI_OFF_BOOT_UP_FAIL:
        case BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL:
        case BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT:
            ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, subsys, BFG_POWER_GPIO_DOWN);
            break;

        case BFGX_POWER_WAKEUP_FAIL:
        case BFGX_POWER_OPEN_CMD_FAIL:
            if (plat_power_fail_exception_info_set(SUBSYS_BFGX, subsys, BFGX_POWERON_FAIL) == BFGX_POWER_SUCCESS) {
                if (bfgx_subsystem_reset() != EXCEPTION_SUCCESS) {
                    ps_print_err("bfgx_subsystem_reset failed \n");
                }
                plat_power_fail_process_done();
            } else {
                ps_print_err("bfgx power fail, set exception info fail\n");
            }

            ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, subsys, BFG_POWER_GPIO_DOWN);
            break;

        default:
            ps_print_err("error is undefined, error=[%d]\n", error);
            break;
    }

    return BFGX_POWER_SUCCESS;
}

/*
 * Prototype    : uart_bfgx_close_cmd
 * Description  : functions called by bfgn pm to close bcpu throuhg bfgx system
 */
int32 uart_bfgx_close_cmd(void)
{
#define WAIT_CLOSE_TIMES 100
    struct ps_core_s *ps_core_d = NULL;
    int bwkup_gpio_val = 1;
    int32 ret;
    int i;

    ps_print_info("%s\n", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return -EINVAL;
    }

    /* 单红外dev不处理系统消息 */
    if (oal_atomic_read(&g_ir_only_mode) != 0) {
        return SUCCESS;
    }

    /* 如果BFGIN睡眠，则唤醒之 */
    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return ret;
    }

    /* 下发BFGIN shutdown命令 */
    ps_print_info("[%s]uart shutdown CPU\n", index2name(ps_core_d->pm_data->index));

    ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_SHUTDOWN_SLP);

    ret = FAILURE;
    for (i = 0; i < WAIT_CLOSE_TIMES; i++) {
        bwkup_gpio_val = board_get_bwkup_gpio_val();
        if (bwkup_gpio_val == 0) {
            ret = SUCCESS;
            break;
        }
        msleep(10); // sleep 10ms
    }
    ps_print_info("[%s]bfg gpio level:%d, i=%d\n", index2name(ps_core_d->pm_data->index), bwkup_gpio_val, i);

    if (ret == FAILURE) {
        ps_uart_state_dump(ps_core_d);
    }

    post_to_visit_node(ps_core_d);

    return ret;
}

int32 bfgx_open_cmd_send(uint32 subsys, uart_enum uart)
{
    uint64 timeleft;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;

    ps_get_core_reference(&ps_core_d, uart);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is null\n");
        return -EINVAL;
    }

    /* 单红外dev不处理系统消息 */
    if (oal_atomic_read(&g_ir_only_mode) != 0) {
        return SUCCESS;
    }

    if (subsys >= BFGX_BUTT) {
        ps_print_err("subsys is err, subsys is [%d]\n", subsys);
        return -EINVAL;
    }

    if (subsys == BFGX_IR) {
        ps_tx_sys_cmd(ps_core_d, SYS_MSG, g_bfgx_open_cmd[subsys]);
        msleep(20); // sleep 20ms
        return 0;
    }

#ifdef PLATFORM_DEBUG_ENABLE
    if (is_dfr_test_en(BFGX_POWEON_FAULT)) {
        ps_print_warning("[dfr test]:trigger powon fail\n");
        return -EINVAL;
    }
#endif

    if (subsys == BFGX_GNSS) {
        g_gnss_me_thread_status = DEV_THREAD_EXIT;
        g_gnss_lppe_thread_status = DEV_THREAD_EXIT;
    }

    pst_bfgx_data = &ps_core_d->bfgx_info[subsys];

    oal_reinit_completion(pst_bfgx_data->wait_opened);
    ps_tx_sys_cmd(ps_core_d, SYS_MSG, g_bfgx_open_cmd[subsys]);
    timeleft = wait_for_completion_timeout(&pst_bfgx_data->wait_opened,
                                           msecs_to_jiffies(g_bfgx_open_cmd_timeout[subsys]));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait %s open ack timeout\n", g_bfgx_subsys_name[subsys]);
        if (subsys == BFGX_GNSS) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_OPEN, CHR_PLAT_DRV_ERROR_OPEN_GNSS);
        } else if (subsys == BFGX_BT) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_OPEN, CHR_PLAT_DRV_ERROR_OPEN_BT);
        }
        return -ETIMEDOUT;
    }

    return 0;
}

int32 bfgx_close_cmd_send(uint32 subsys, uart_enum uart)
{
    uint64 timeleft;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;

    ps_get_core_reference(&ps_core_d, uart);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is null\n");
        return -EINVAL;
    }

    if (subsys >= BFGX_BUTT) {
        ps_print_err("subsys is err, subsys is [%d]\n", subsys);
        return -EINVAL;
    }

    /* 单红外dev不处理系统消息 */
    if (oal_atomic_read(&g_ir_only_mode) != 0) {
        return SUCCESS;
    }

#ifdef PLATFORM_DEBUG_ENABLE
    if (is_dfr_test_en(BFGX_POWEOFF_FAULT)) {
        ps_print_warning("[dfr test]:trigger power off fail\n");
        return -EINVAL;
    }
#endif

    if (subsys == BFGX_IR) {
        ps_tx_sys_cmd(ps_core_d, SYS_MSG, g_bfgx_close_cmd[subsys]);
        msleep(20); // sleep 20ms
        return 0;
    }

    pst_bfgx_data = &ps_core_d->bfgx_info[subsys];

    oal_reinit_completion(pst_bfgx_data->wait_closed);
    ps_tx_sys_cmd(ps_core_d, SYS_MSG, g_bfgx_close_cmd[subsys]);
    timeleft = wait_for_completion_timeout(&pst_bfgx_data->wait_closed,
                                           msecs_to_jiffies(g_bfgx_close_cmd_timeout[subsys]));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait %s close ack timeout\n", g_bfgx_subsys_name[subsys]);
        if (subsys == BFGX_GNSS) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_CLOSE, CHR_PLAT_DRV_ERROR_CLOSE_GNSS);
        } else if (subsys == BFGX_BT) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_CLOSE, CHR_PLAT_DRV_ERROR_CLOSE_BT);
        }
        return -ETIMEDOUT;
    }

    return 0;
}

/* 单红外模式打开其他子系统时调用,关闭红外 */
int32 hw_ir_only_open_other_subsys(void)
{
    int32 ret;

    ret = hw_bfgx_close(BFGX_IR);
    oal_atomic_set(&g_ir_only_mode, 0);
    return ret;
}

STATIC int32 hw_bfgx_input_check(uint32 subsys)
{
    if (subsys >= BFGX_BUTT) {
        ps_print_err("subsys is err, subsys is [%d]\n", subsys);
        return -EINVAL;
    }
    if (oal_warn_on(is_bfgx_support() != OAL_TRUE)) {
        ps_print_err("subsys is [%d], bfgx %s support\n", subsys,
                     (is_bfgx_support() == OAL_TRUE) ? "" : "don't");
        return -ENODEV;
    }

    return 0;
}

int32 hw_bfgx_open(uint32 subsys)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;
    int32 error;
    struct pm_drv_data *pm_data = NULL;

    ret = hw_bfgx_input_check(subsys);
    if (ret != 0) {
        return ret;
    }

    ps_print_info("open %s\n", g_bfgx_subsys_name[subsys]);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->pm_data == NULL) ||
                 (ps_core_d->pm_data->bfg_power_set == NULL))) {
        return -EINVAL;
    }
    pm_data = ps_core_d->pm_data;
    pst_bfgx_data = &ps_core_d->bfgx_info[subsys];

    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_OPEN) {
        ps_print_warning("[%s]%s has opened! It's Not necessary to send msg to device\n",
                         index2name(pm_data->index), g_bfgx_subsys_name[subsys]);
        return BFGX_POWER_SUCCESS;
    }

    if (alloc_seperted_rx_buf(ps_core_d, subsys, g_bfgx_rx_max_frame[subsys], VMALLOC) != BFGX_POWER_SUCCESS) {
        return -ENOMEM;
    }

    /* 当单红外模式打开其他子系统时，需要关闭单红外才能其他子系统正常上电 */
    if ((oal_atomic_read(&g_ir_only_mode) != 0) && subsys != BFGX_IR) {
        if (hw_ir_only_open_other_subsys() != BFGX_POWER_SUCCESS) {
            ps_print_err("ir only mode,but close ir only mode fail!\n");
            free_seperted_rx_buf(ps_core_d, subsys, VMALLOC);
            return -ENOMEM;
        }
    }

    error = pm_data->bfg_power_set(pm_data, subsys, BFG_POWER_GPIO_UP);
    if (error != BFGX_POWER_SUCCESS) {
        ps_print_err("[%s]set %s power on err! error = %d\n",
                     index2name(pm_data->index), g_bfgx_subsys_name[subsys], error);
        goto bfgx_power_on_fail;
    }

    if (prepare_to_visit_node(ps_core_d) != BFGX_POWER_SUCCESS) {
        error = BFGX_POWER_WAKEUP_FAIL;
        goto bfgx_wakeup_fail;
    }

    if (bfgx_open_cmd_send(subsys, BUART) != BFGX_POWER_SUCCESS) {
        ps_print_err("bfgx open cmd fail\n");
        error = BFGX_POWER_OPEN_CMD_FAIL;
        goto bfgx_open_cmd_fail;
    }

    /* 单红外没有低功耗 */
    if (oal_atomic_read(&g_ir_only_mode) == 0) {
        mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
        ps_core_d->pm_data->bfg_timer_mod_cnt++;
    }

    atomic_set(&pst_bfgx_data->subsys_state, POWER_STATE_OPEN);
    post_to_visit_node(ps_core_d);

    return BFGX_POWER_SUCCESS;

bfgx_open_cmd_fail:
    post_to_visit_node(ps_core_d);
bfgx_wakeup_fail:
bfgx_power_on_fail:
    free_seperted_rx_buf(ps_core_d, subsys, VMALLOC);
    bfgx_open_fail_process(subsys, error);
    return BFGX_POWER_FAILED;
}

int32 hw_bfgx_close(uint32 subsys)
{
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;
    int32 ret;
    struct pm_drv_data *pm_data = NULL;

    if (subsys >= BFGX_BUTT) {
        ps_print_err("subsys is err, subsys is [%d]\n", subsys);
        return -EINVAL;
    }

    ps_print_info("close %s\n", g_bfgx_subsys_name[subsys]);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->pm_data == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }
    pm_data = ps_core_d->pm_data;
    pst_bfgx_data = &ps_core_d->bfgx_info[subsys];

    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_SHUTDOWN) {
        ps_print_warning("[%s]%s has closed! It's Not necessary to send msg to device\n",
                         index2name(pm_data->index), g_bfgx_subsys_name[subsys]);
        return BFGX_POWER_SUCCESS;
    }
    wake_up_interruptible(&pst_bfgx_data->rx_wait);

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        /* 唤醒失败，bfgx close时的唤醒失败不进行DFR恢复 */
        ps_print_err("[%s]prepare work FAIL\n", index2name(pm_data->index));
    }

    ret = bfgx_close_cmd_send(subsys, BUART);
    if (ret < 0) {
        /* 发送close命令失败，不进行DFR，继续进行下电流程，DFR恢复延迟到下次open时或者其他业务运行时进行 */
        ps_print_err("[%s]bfgx close cmd fail\n", index2name(pm_data->index));
    }

    atomic_set(&pst_bfgx_data->subsys_state, POWER_STATE_SHUTDOWN);
    free_seperted_rx_buf(ps_core_d, subsys, VMALLOC);
    ps_kfree_skb(ps_core_d, g_bfgx_rx_queue[subsys]);

    pst_bfgx_data->rx_pkt_num = 0;
    pst_bfgx_data->tx_pkt_num = 0;

    if (bfgx_other_subsys_all_shutdown(pm_data, BFGX_GNSS)) {
        del_timer_sync(&pm_data->bfg_timer);
        pm_data->bfg_timer_mod_cnt = 0;
        pm_data->bfg_timer_mod_cnt_pre = 0;
    }

    ret = pm_data->bfg_power_set(pm_data, subsys, BFG_POWER_GPIO_DOWN);
    if (ret) {
        /* 下电失败，不进行DFR，DFR恢复延迟到下次open时或者其他业务运行时进行 */
        ps_print_err("set %s power off err!ret = %d", g_bfgx_subsys_name[subsys], ret);
    }

    post_to_visit_node(ps_core_d);

    return 0;
}

// 子系统关闭则不再写数据
STATIC int32 hw_bfgx_write_check(struct file *filp, const int8 __user *buf,
                                 struct ps_core_s *ps_core_d, uint32 subsys)
{
    struct st_bfgx_data *pst_bfgx_data = NULL;
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    pst_bfgx_data = &ps_core_d->bfgx_info[subsys];
    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_SHUTDOWN) {
        ps_print_warning("%s has closed! It's Not necessary to send msg to device\n", g_bfgx_subsys_name[subsys]);
        return -EINVAL;
    }
    return 0;
}

#ifdef _PRE_CONFIG_HISI_110X_BLUEZ
STATIC int hisi_recv_buf_update_len(struct hci_dev *hdev, struct sk_buff **skb,
    const struct hbt_recv_pkt *pkts, unsigned int pkts_index)
{
    if ((*skb)->len == (&pkts[pkts_index])->hlen) {
        u16 dlen;

        switch ((&pkts[pkts_index])->lsize) {
            case 0: // size 0
                /* No variable data length */
                dlen = 0;
                break;
            case 1: // size 1
                /* Single octet variable length */
                dlen = (*skb)->data[(&pkts[pkts_index])->loff];
                hci_skb_expect(*skb) += dlen;

                if (skb_tailroom(*skb) < dlen) {
                    kfree_skb(*skb);
                    return -1; /* ERR_PTR(-EMSGSIZE) */
                }
                break;
            case 2: // size 2
                /* Double octet variable length */
                dlen = get_unaligned_le16((*skb)->data +
                                (&pkts[pkts_index])->loff);
                hci_skb_expect(*skb) += dlen;

                if (skb_tailroom(*skb) < dlen) {
                    kfree_skb(*skb);
                    return -1; /* ERR_PTR(-EMSGSIZE) */
                }
                break;
            default:
                /* Unsupported variable length */
                kfree_skb(*skb);
                return -1; /* ERR_PTR(-EILSEQ) */
        }

        if (!dlen) {
            /* No more data, complete frame */
            (&pkts[pkts_index])->recv(hdev, (*skb));
            (*skb) = NULL;
        }
    } else {
        /* Complete frame */
        (&pkts[pkts_index])->recv(hdev, (*skb));
        (*skb) = NULL;
    }

    return 0;
}

struct sk_buff *hisi_recv_buf(struct hci_dev *hdev, struct sk_buff *skb,
    const unsigned char *buffer, int count,
    const struct hbt_recv_pkt *pkts, int pkts_count)
{
    int err;

    while (count) {
        int i, len;

        if (!count)
            break;

        if (skb == NULL) {
            for (i = 0; i < pkts_count; i++) {
                if (buffer[0] != (&pkts[i])->type)
                    continue;

                skb = bt_skb_alloc((&pkts[i])->maxlen, GFP_ATOMIC);
                if (!skb)
                    return ERR_PTR(-ENOMEM);

                hci_skb_pkt_type(skb) = (&pkts[i])->type;
                hci_skb_expect(skb) = (&pkts[i])->hlen;
                break;
            }

            /* Check for invalid packet type */
            if (skb == NULL) {
                return ERR_PTR(-EILSEQ);
            }

            count -= 1;
            buffer += 1;
        }

        len = min_t(uint, hci_skb_expect(skb) - skb->len, count);
        skb_put_data(skb, buffer, len);

        count -= len;
        buffer += len;

        /* Check for partial packet */
        if (skb->len < hci_skb_expect(skb))
            continue;

        for (i = 0; i < pkts_count; i++) {
            if (hci_skb_pkt_type(skb) == (&pkts[i])->type)
                break;
        }

        if (i >= pkts_count) {
            kfree_skb(skb);
            return ERR_PTR(-EILSEQ);
        }

        err = hisi_recv_buf_update_len(hdev, &skb, pkts, i);
        if (err != 0) {
            return ERR_PTR(-EMSGSIZE);
        }
    }

    return skb;
}

STATIC int hisi_recv(struct hci_dev *hdev, const void *data, int count)
{
    struct sk_buff *rx_skb;
    rx_skb = hisi_recv_buf(hdev, NULL, (const unsigned char *)data,
        count, g_hisi_recv_pkts, ARRAY_SIZE(g_hisi_recv_pkts));
    if (IS_ERR(rx_skb)) {
        int err = PTR_ERR(rx_skb);
        ps_print_info("ERROR rx_skb=%d\n", err);
        return err;
    }

    hdev->stat.byte_rx += count;

    return count;
}

STATIC uint32 char2byte_bt(const char *strori, char *outbuf)
{
    int i = 0;
    uint8 temp = 0;
    uint32 sum = 0;
    uint8 *ptr_out = (uint8 *)outbuf;
    const int l_loop_times = 12; /* 单字节遍历是不是正确的mac地址:xx:xx:xx:xx:xx:xx */

    for (i = 0; i < l_loop_times; i++) {
        switch (strori[i]) {
            case '0' ... '9':
                temp = strori[i] - '0';
                break;

            case 'a' ... 'f':
                temp = (strori[i] - 'a') + 10; /* 加10为了保证'a'~'f'分别对应10~15 */
                break;

            case 'A' ... 'F':
                temp = (strori[i] - 'A') + 10; /* 加10为了保证'A'~'F'分别对应10~15 */
                break;
            default:
                break;
        }

        sum += temp;
        /* 为了组成正确的mac地址:xx:xx:xx:xx:xx:xx */
        if (i % 2 == 0) {
            ptr_out[i / 2] |= (temp << 4);
        } else {
            ptr_out[i / 2] |= temp;
        }
    }

    return sum;
}

#define HCI_SET_BD_ADDR 0xFC32
#define CHECK_MAX_CNT    3
#define CHECK_WAIT_TIME  50
STATIC int hci_bt_bdaddr(struct hci_dev *hdev)
{
    struct sk_buff *skb = NULL;
    int ret = INI_FAILED;
    int i;
    unsigned int offset = 0;
    unsigned int buf_len = 12;
    uint8 puc_buf[6] = { 0 };
    bdaddr_t bt_bdaddr = { { 0x12, 0x34, 0x56, 0x78, 0x90, 0xab } };
    char original_mac_addr[12] = { 0 };
    uint32 ul_sum = 0;

    /* READ MAC frome Eeprom */
    for (i = 0; i < CHECK_MAX_CNT; i++) {
        if (DrvEepromRead("MACBT", offset, original_mac_addr, buf_len) == INI_SUCC) {
            ret = INI_SUCC;
            break;
        }
        msleep(CHECK_WAIT_TIME);
    }

    if (ret == 0) {
        oal_io_print("DrvEepromRead MACBT return success\n");
        ul_sum = char2byte_bt(original_mac_addr, (int8 *)puc_buf);
        if (ul_sum != 0) {
            ini_warning("hci_bt_bdaddr get MAC from EEPROM: mac=" MACFMT "\n", ini_mac2str(puc_buf));
            bt_bdaddr.b[5] = puc_buf[0];
            bt_bdaddr.b[4] = puc_buf[1];
            bt_bdaddr.b[3] = puc_buf[2];
            bt_bdaddr.b[2] = puc_buf[3];
            bt_bdaddr.b[1] = puc_buf[4];
            bt_bdaddr.b[0] = puc_buf[5];
        }

        /* send macaddr to 1103 */
        for (i = 0; i < CHECK_MAX_CNT; i++) {
            skb = __hci_cmd_sync(hdev, HCI_SET_BD_ADDR, sizeof(bdaddr_t), &bt_bdaddr, HCI_INIT_TIMEOUT);
            if (!IS_ERR(skb)) {
                oal_io_print("retry %d times, MACBT from eeprom send success to device\n", i);
                break;
            }
            msleep(CHECK_WAIT_TIME);
        }

        if (IS_ERR(skb)) {
            int err = PTR_ERR(skb);
            ps_print_err("__hci_cmd_sync changing bt device address failed,%d\n", err);
            return 0;
        }
        kfree_skb(skb);
    } else {
        ps_print_err("DrvEepromRead read error,use device default bt_bdaddr\n");
        chr_exception_report(CHR_READ_EEPROM_ERROR_EVENTID, CHR_SYSTEM_BT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_EEPROM_READ_INIT);
    }

    return 0;
}

STATIC int hci_bt_init(struct hci_dev *hdev)
{
    int ret;
    PS_PRINT_FUNCTION_NAME;
    set_bit(HCI_RUNNING, &hdev->flags);
    atomic_set(&hdev->cmd_cnt, 1);
    set_bit(HCI_INIT, &hdev->flags);

    ret = hci_bt_bdaddr(hdev);
    return ret;
}

/* Initialize device */
STATIC int hci_bt_open(struct hci_dev *hdev)
{
    int ret;
    struct pm_drv_data *pm_data = (struct pm_drv_data *)hci_get_drvdata(hdev);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    ps_print_info("hci_bt_open %p, glb:%p\n", pm_data, pm_get_drvdata(BUART));

    /* Undo clearing this from hci_uart_close() */
    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_open(BFGX_BT);

    mutex_unlock(&(pm_top_data->host_mutex));

    if (ret == 0) {
        ret = hci_bt_init(hdev);
    }

    return ret;
}

/* Close device */
STATIC int hci_bt_close(struct hci_dev *hdev)
{
    int ret;
    struct pm_drv_data *pm_data = (struct pm_drv_data *)hci_get_drvdata(hdev);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    ps_print_info("hci_bt_close\n");

    // hci_uart_flush(hdev)
    hdev->flush = NULL;

    /* bt power off */
    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_close(BFGX_BT);

    oal_wake_unlock_force(&pm_data->bt_wake_lock);

    mutex_unlock(&(pm_top_data->host_mutex));
    return ret;
}

STATIC void hci_bt_tx_complete(struct hci_dev *hdev, struct sk_buff *skb)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 5, 0))
    int pkt_type = hci_skb_pkt_type(skb);
#else
    int pkt_type = bt_cb(skb)->pkt_type;
#endif

    /* Update HCI stat counters */
    switch (pkt_type) {
        case HCI_COMMAND_PKT:
            hdev->stat.cmd_tx++;
            break;

        case HCI_ACLDATA_PKT:
            hdev->stat.acl_tx++;
            break;

        case HCI_SCODATA_PKT:
            hdev->stat.sco_tx++;
            break;

        default:
            /* debug */
            ps_print_info("hci default type=%d\n", pkt_type);
            break;
    }

    ps_print_dbg("hci bt tx type=%d\n", pkt_type);
}

STATIC int hci_bt_send(struct hci_dev *hdev, struct ps_core_s *ps_core_d,
    struct sk_buff *skb, uint32 count)
{
    uint16 total_len;
    int32 ret;

    int32 headroom_add, headroom, tailroom, tailroom_add;

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work fail, bring to reset work\n");
        g_bt_data_combination.len = 0;
        plat_exception_handler(SUBSYS_BFGX, THREAD_BT, BFGX_WAKEUP_FAIL);
        return ret;
    }

    oal_wake_lock_timeout(&ps_core_d->pm_data->bt_wake_lock, DEFAULT_WAKELOCK_TIMEOUT);

    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
    ps_core_d->pm_data->bfg_timer_mod_cnt++;

    headroom = g_bt_data_combination.len + sizeof(struct ps_packet_head) + 1;
    tailroom = sizeof(struct ps_packet_end);

    headroom_add = (headroom > oal_netbuf_headroom(skb) ? (headroom - oal_netbuf_headroom(skb)) : 0);
    tailroom_add = (tailroom > oal_netbuf_tailroom(skb) ? (tailroom - oal_netbuf_headroom(skb)) : 0);

    total_len = count + headroom + tailroom;

    if ((headroom_add > 0) || (tailroom_add > 0)) {
        /* head rom too small */
        ret = oal_netbuf_expand_head(skb, headroom_add, tailroom_add, GFP_ATOMIC);
        if (ret != 0) {
            ps_print_err("relloc skb failed, add=%d\n", headroom_add);
            post_to_visit_node(ps_core_d);
            g_bt_data_combination.len = 0;
            return ret;
        }
    }

    oal_netbuf_push(skb, headroom);
    oal_netbuf_put(skb, tailroom);

    skb->data[sizeof(struct ps_packet_head)] = hci_skb_pkt_type(skb);
    if (g_bt_data_combination.len == BT_TYPE_DATA_LEN) {
        skb->data[sizeof(struct ps_packet_head) + 1] = g_bt_data_combination.type;
    }

    g_bt_data_combination.len = 0;

    hdev->stat.byte_tx += (count + 1);

    hci_bt_tx_complete(hdev, skb);

    ps_add_packet_head(skb->data, BT_MSG, total_len);
    ps_skb_enqueue(ps_core_d, skb, TX_HIGH_QUEUE);
    queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

    ps_core_d->bfgx_info[BFGX_BT].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    return 0;
}

/* Send frames from HCI layer */
STATIC int hci_bt_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
    int32 ret;
    uint32 count;
    struct ps_core_s *ps_core_d = NULL;

    PS_PRINT_FUNCTION_NAME;

    ret = ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ret != SUCCESS) || (ps_core_d == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        g_bt_data_combination.len = 0;
        return -EINVAL;
    }

    count = skb->len;

    if (count > BT_TX_MAX_FRAME) {
        ps_print_err("bt skb len is too large, len=%u\n", skb->len);
        g_bt_data_combination.len = 0;
        return -EINVAL;
    }

    /* 适配O，BT数据分两次下发，先发数据类型，长度固定为1Byte，然后发数据，需要在驱动中组合起来发给device */
    if (count == BT_TYPE_DATA_LEN) {
        g_bt_data_combination.type = skb->data[0];
        g_bt_data_combination.len = count;
        kfree_skb(skb);
        return 0;
    }

    /* if high queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_high_seq.qlen > TX_HIGH_QUE_MAX_NUM) {
        ps_print_err("bt tx high seqlen large than MAXNUM\n");
        g_bt_data_combination.len = 0;
        kfree_skb(skb);
        return 0;
    }

    ret = hci_bt_send(hdev, ps_core_d, skb, count);

    return ret;
}

/* Reset device */
STATIC int hci_bt_flush(struct hci_dev *hdev)
{
    PS_PRINT_FUNCTION_NAME;

    return 0;
}


STATIC int hci_bt_setup(struct hci_dev *hdev)
{
    ps_print_info("hci_bt_setup\n");
    return 0;
}


STATIC int hci_bt_recv_thread(oal_void *data)
{
    int ret = 0;
    struct hci_dev *hdev = NULL;
    struct sk_buff *skb = NULL;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = (struct pm_drv_data *)data;

    if (pm_data == NULL) {
        return 0;
    }

    allow_signal(SIGTERM);

    for (;;) {
        ret = ps_get_core_reference(&ps_core_d, BUART);
        if (unlikely((ret != SUCCESS) || (ps_core_d == NULL))) {
            ps_print_err("ps_core_d is NULL\n");
            break;
        }

        if (oal_unlikely(kthread_should_stop())) {
            ps_print_info("hci_bt_recv_thread stop\n");
            break;
        }

        ret = oal_wait_event_interruptible_m(ps_core_d->bfgx_info[BFGX_BT].rx_wait,
                                             (ps_skb_queue_len(ps_core_d, RX_BT_QUEUE) != 0));
        if (ret != 0) {
            if (ret == -ERESTARTSYS) {
                ps_print_info("hci_bt_recv_thread interrupt\n");
                break;
            }
            ps_print_warning("hci_bt_recv_thread error ret=%d\n", ret);
        }

        skb = ps_skb_dequeue(ps_core_d, RX_BT_QUEUE);
        if (skb == NULL) {
            ps_print_warning("bt read skb queue is null!\n");
            continue;
        }

        hdev = pm_data->st_bt_dev.hdev;
        if (hdev == NULL) {
            ps_print_warning("hdev is null, drop recv pkt\n");
            kfree_skb(skb);
            continue;
        }

        hdev->stat.byte_rx += skb->len;

        hisi_recv(hdev, skb->data, skb->len);
        kfree_skb(skb);
    }

    return 0;
}

STATIC int bt_register_hci_dev(struct pm_drv_data *pm_data)
{
    int err;
    struct hci_dev *hdev;

    /* Initialize and register HCI device */
    hdev = hci_alloc_dev();
    if (hdev == NULL) {
        ps_print_err("can't allocate Hisi HCI device\n");
        return -ENOMEM;
    }

    hdev->bus = HCI_UART;

    hci_set_drvdata(hdev, pm_data);

    hdev->manufacturer = 16; /* 16 is hci manufacture */

    hdev->open  = hci_bt_open;
    hdev->close = hci_bt_close;
    hdev->flush = hci_bt_flush;
    hdev->send  = hci_bt_send_frame;
    hdev->setup = hci_bt_setup;

    err = hci_register_dev(hdev);
    if (err < 0) {
        ps_print_err("can't register hisi HCI device, err=%d\n", err);
        hci_free_dev(hdev);
        return -ENODEV;
    }

    /* create recv thread */
    pm_data->st_bt_dev.bt_recv_task = oal_thread_create(hci_bt_recv_thread,
                                                        (void *)pm_data,
                                                        NULL,
                                                        "hisi_hci_recv",
                                                        SCHED_FIFO,
                                                        0,
                                                        -1);
    if (IS_ERR(pm_data->st_bt_dev.bt_recv_task)) {
        ps_print_err("create hci_recv thread failed, err=%p\n", pm_data->st_bt_dev.bt_recv_task);
        hci_unregister_dev(hdev);
        hci_free_dev(hdev);
        return -ENOMEM;
    }

    pm_data->st_bt_dev.hdev = hdev;

    return 0;
}

STATIC void bt_unregister_hci_dev(struct pm_drv_data *pm_data)
{
    struct hci_dev *hdev;

    if (pm_data->st_bt_dev.hdev == NULL) {
        ps_print_info("hdev is null\n");
        return;
    }

    hdev = pm_data->st_bt_dev.hdev;

    if (!IS_ERR(pm_data->st_bt_dev.bt_recv_task)) {
        oal_thread_stop(pm_data->st_bt_dev.bt_recv_task, NULL);
        pm_data->st_bt_dev.bt_recv_task = NULL;
    }

    pm_data->st_bt_dev.hdev = NULL;
    hci_unregister_dev(hdev);

    hci_free_dev(hdev);

    ps_print_info("bt_unregister_hci_dev\n");
}
/*
 * Prototype    : hw_bt_open
 * Description  : functions called from above bt hal,when open bt file
 * input        : "/dev/hwbt"
 * output       : return 0 --> open is ok
 *              : return !0--> open is false
 */
STATIC int32 hw_bt_open(struct inode *inode, struct file *filp)
{
    int32 ret = 0;

    ps_print_info("hw_bt_open\n");

    return ret;
}

/*
 * Prototype    : hw_bt_read
 * Description  : functions called from above bt hal,read count data to buf
 * input        : file handle, buf, count
 * output       : return size --> actual read byte size
 */
STATIC ssize_t hw_bt_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 count1;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if ((skb = ps_skb_dequeue(ps_core_d, RX_BT_QUEUE)) == NULL) {
        ps_print_warning("bt read skb queue is null!\n");
        return 0;
    }

    /* read min value from skb->len or count */
    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_BT_QUEUE);
        return -EFAULT;
    }

    /* have read count1 byte */
    skb_pull(skb, count1);

    /* if skb->len = 0: read is over */
    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        ps_restore_skbqueue(ps_core_d, skb, RX_BT_QUEUE);
    }

    return count1;
}

/*
 * Prototype    : hw_bt_write
 * Description  : functions called from above bt hal,write count data to buf
 * input        : file handle, buf, count
 * output       : return size --> actual write byte size
 */
STATIC ssize_t hw_bt_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 total_len;
    int32 ret = 0;
    uint8 __user *puser = (uint8 __user *)buf;
    uint8 type = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL) || (ps_core_d->pm_data == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        g_bt_data_combination.len = 0;
        return -EINVAL;
    }

    if (count > BT_TX_MAX_FRAME) {
        ps_print_err("bt skb len is too large!\n");
        g_bt_data_combination.len = 0;
        return -EINVAL;
    }

    if (count == BT_TYPE_DATA_LEN) {
        get_user(type, puser);
        g_bt_data_combination.type = type;
        g_bt_data_combination.len = count;
        return count;
    }

    /* if high queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_high_seq.qlen > TX_HIGH_QUE_MAX_NUM) {
        ps_print_err("bt tx high seqlen large than MAXNUM\n");
        g_bt_data_combination.len = 0;
        return 0;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work fail, bring to reset work\n");
        g_bt_data_combination.len = 0;
        plat_exception_handler(SUBSYS_BFGX, THREAD_BT, BFGX_WAKEUP_FAIL);
        return ret;
    }

    oal_wake_lock_timeout(&ps_core_d->pm_data->bt_wake_lock, DEFAULT_WAKELOCK_TIMEOUT);

    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
    ps_core_d->pm_data->bfg_timer_mod_cnt++;

    total_len = count + g_bt_data_combination.len + sizeof(struct ps_packet_head) + sizeof(struct ps_packet_end);

    skb = ps_alloc_skb(total_len);
    if (skb == NULL) {
        ps_print_err("ps alloc skb mem fail\n");
        post_to_visit_node(ps_core_d);
        g_bt_data_combination.len = 0;
        return -EFAULT;
    }

    if (copy_from_user(&skb->data[sizeof(struct ps_packet_head) + g_bt_data_combination.len], buf, count)) {
        ps_print_err("copy_from_user from bt is err\n");
        kfree_skb(skb);
        post_to_visit_node(ps_core_d);
        g_bt_data_combination.len = 0;
        return -EFAULT;
    }

    if (g_bt_data_combination.len == BT_TYPE_DATA_LEN) {
        skb->data[sizeof(struct ps_packet_head)] = g_bt_data_combination.type;
    }

    g_bt_data_combination.len = 0;

    ps_add_packet_head(skb->data, BT_MSG, total_len);
    ps_skb_enqueue(ps_core_d, skb, TX_HIGH_QUEUE);
    queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

    ps_core_d->bfgx_info[BFGX_BT].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    return count;
}

/*
 * Prototype    : hw_bt_poll
 * Description  : called by bt func from hal;
 *                check whether or not allow read and write
 */
STATIC uint32 hw_bt_poll(struct file *filp, poll_table *wait)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    uint32 mask = 0;

    PS_PRINT_FUNCTION_NAME;

    ret = ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ret != SUCCESS) || (ps_core_d == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    /* push curr wait event to wait queue */
    poll_wait(filp, &ps_core_d->bfgx_info[BFGX_BT].rx_wait, wait);

    if (ps_core_d->bfgx_info[BFGX_BT].rx_queue.qlen) { /* have data to read */
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

/*
 * Prototype    : hw_bt_ioctl
 * Description  : called by bt func from hal; default not use
 */
int64 hw_bt_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    int64 ret = 0;

    struct pm_drv_data *pm_data = pm_get_drvdata(BUART);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    ps_print_info("cmd is %d,arg is %lu", cmd, arg);

    mutex_lock(&(pm_top_data->host_mutex));

    /* hal下发cmd,控制不实际关闭close bt */
    switch (cmd) {
        case BT_IOCTL_FAKE_CLOSE_CMD:
            if (arg == BT_FAKE_CLOSE) {
                pm_data->bt_fake_close_flag = OAL_TRUE;
            } else {
                pm_data->bt_fake_close_flag = OAL_FALSE;
            }
            break;
        case BT_IOCTL_OPEN:
            ret = hw_bfgx_open(BFGX_BT);
            break;
        case BT_IOCTL_RELEASE:
            ret = hw_bfgx_close(BFGX_BT);
            oal_wake_unlock_force(&pm_data->bt_wake_lock);
            break;
        case BT_IOCTL_HCISETPROTO:
            if (pm_data->st_bt_dev.hdev != NULL) {
                ret = -EEXIST;
                break;
            }
            ret = (int64)bt_register_hci_dev(pm_data);
            if (ret != 0) {
                ps_print_err("bt_register_hci_dev failed =%ld\n", ret);
                break;
            }
            break;
        case BT_IOCTL_HCIUNSETPROTO:
            mutex_unlock(&(pm_top_data->host_mutex));
            bt_unregister_hci_dev(pm_data);
            mutex_lock(&(pm_top_data->host_mutex));
            ps_print_info("bt_unregister_hci_dev BT_IOCTL_HCIUNSETPROTO has unregister");
            break;
        default:
            ps_print_info("\n");
    }

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}


/*
 * Prototype    : hw_bt_release
 * Description  : called by bt func from hal when close bt inode
 * input        : "/dev/hwbt"
 * output       : return 0 --> close is ok
 *                return !0--> close is false
 */
STATIC int32 hw_bt_release(struct inode *inode, struct file *filp)
{
    return 0;
}

#else  /* Android bluedroid */

/*
 * Prototype    : hw_bt_open
 * Description  : functions called from above bt hal,when open bt file
 * input        : "/dev/hwbt"
 * output       : return 0 --> open is ok
 *              : return !0--> open is false
 */
STATIC int32 hw_bt_open(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_top* pm_top_data = pm_get_top();

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_open(BFGX_BT);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/*
 * Prototype    : hw_bt_read
 * Description  : functions called from above bt hal,read count data to buf
 * input        : file handle, buf, count
 * output       : return size --> actual read byte size
 */
STATIC ssize_t hw_bt_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 count1;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if ((skb = ps_skb_dequeue(ps_core_d, RX_BT_QUEUE)) == NULL) {
        ps_print_warning("bt read skb queue is null!\n");
        return 0;
    }

    /* read min value from skb->len or count */
    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_BT_QUEUE);
        return -EFAULT;
    }

    /* have read count1 byte */
    skb_pull(skb, count1);

    /* if skb->len = 0: read is over */
    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        ps_restore_skbqueue(ps_core_d, skb, RX_BT_QUEUE);
    }

    return count1;
}

/*
 * Prototype    : hw_bt_write
 * Description  : functions called from above bt hal,write count data to buf
 * input        : file handle, buf, count
 * output       : return size --> actual write byte size
 */
STATIC ssize_t hw_bt_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 total_len;
    int32 ret = 0;
    uint8 __user *puser = (uint8 __user *)buf;
    uint8 type = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    ret = hw_bfgx_write_check(filp, buf, ps_core_d, BFGX_BT);
    if (ret < 0) {
        g_bt_data_combination.len = 0;
        return ret;
    }

    if (count > BT_TX_MAX_FRAME) {
        ps_print_err("bt skb len is too large!\n");
        g_bt_data_combination.len = 0;
        return -EINVAL;
    }

    /* 适配O，BT数据分两次下发，先发数据类型，长度固定为1Byte，然后发数据，需要在驱动中组合起来发给device */
    if (count == BT_TYPE_DATA_LEN) {
        get_user(type, puser);
        g_bt_data_combination.type = type;
        g_bt_data_combination.len = count;

        return count;
    }

    /* if high queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_high_seq.qlen > TX_HIGH_QUE_MAX_NUM) {
        ps_print_err("bt tx high seqlen large than MAXNUM\n");
        g_bt_data_combination.len = 0;
        return 0;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work fail, bring to reset work\n");
        g_bt_data_combination.len = 0;
        plat_exception_handler(SUBSYS_BFGX, THREAD_BT, BFGX_WAKEUP_FAIL);
        return ret;
    }

    oal_wake_lock_timeout(&ps_core_d->pm_data->bt_wake_lock, DEFAULT_WAKELOCK_TIMEOUT);

    /* modify expire time of uart idle timer */
    ps_core_d->pm_data->bfg_timer_mod_cnt++;
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));

    total_len = count + g_bt_data_combination.len + sizeof(struct ps_packet_head) + sizeof(struct ps_packet_end);

    skb = ps_alloc_skb(total_len);
    if (skb == NULL) {
        ps_print_err("ps alloc skb mem fail\n");
        post_to_visit_node(ps_core_d);
        g_bt_data_combination.len = 0;
        return -EFAULT;
    }

    if (copy_from_user(&skb->data[sizeof(struct ps_packet_head) + g_bt_data_combination.len], buf, count)) {
        ps_print_err("copy_from_user from bt is err\n");
        kfree_skb(skb);
        post_to_visit_node(ps_core_d);
        g_bt_data_combination.len = 0;
        return -EFAULT;
    }

    if (g_bt_data_combination.len == BT_TYPE_DATA_LEN) {
        skb->data[sizeof(struct ps_packet_head)] = g_bt_data_combination.type;
    }

    g_bt_data_combination.len = 0;

    ps_add_packet_head(skb->data, BT_MSG, total_len);
    ps_skb_enqueue(ps_core_d, skb, TX_HIGH_QUEUE);
    queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

    ps_core_d->bfgx_info[BFGX_BT].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    return count;
}

/*
 * Prototype    : hw_bt_poll
 * Description  : called by bt func from hal;
 *                check whether or not allow read and write
 */
STATIC uint32 hw_bt_poll(struct file *filp, poll_table *wait)
{
    struct ps_core_s *ps_core_d = NULL;
    uint32 mask = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    /* push curr wait event to wait queue */
    poll_wait(filp, &ps_core_d->bfgx_info[BFGX_BT].rx_wait, wait);

    if (ps_core_d->bfgx_info[BFGX_BT].rx_queue.qlen) { /* have data to read */
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

/*
 * Prototype    : hw_bt_ioctl
 * Description  : called by bt func from hal; default not use
 */
STATIC int64 hw_bt_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    struct pm_drv_data *pm_data = pm_get_drvdata(BUART);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    ps_print_warning("cmd is %d,arg is %lu", cmd, arg);

    mutex_lock(&(pm_top_data->host_mutex));

    /* hal下发cmd,控制不实际关闭close bt */
    if (cmd == BT_IOCTL_FAKE_CLOSE_CMD) {
        if (arg == BT_FAKE_CLOSE) {
            pm_data->bt_fake_close_flag = OAL_TRUE;
        } else {
            pm_data->bt_fake_close_flag = OAL_FALSE;
        }
    }

    mutex_unlock(&(pm_top_data->host_mutex));

    return 0;
}

STATIC int32 hw_bt_fake_close(void)
{
#ifndef HAVE_HISI_NFC
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        /* 唤醒失败DFR恢复 */
        ps_print_err("wakeup device FAIL\n");
        plat_exception_handler(SUBSYS_BFGX, THREAD_BT, BFGX_WAKEUP_FAIL);
        return ret;
    }

    ps_tx_sys_cmd(ps_core_d, SYS_MSG, PL_FAKE_CLOSE_BT_EVT);

    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
    ps_core_d->pm_data->bfg_timer_mod_cnt++;

    post_to_visit_node(ps_core_d);
#endif
    return 0;
}


/*
 * Prototype    : hw_bt_release
 * Description  : called by bt func from hal when close bt inode
 * input        : "/dev/hwbt"
 * output       : return 0 --> close is ok
 *                return !0--> close is false
 */
STATIC int32 hw_bt_release(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_drv_data *pm_data = pm_get_drvdata(BUART);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("param is error");
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    /* hal下发cmd,控制不实际关闭close bt */
    if (pm_data->bt_fake_close_flag == OAL_TRUE) {
        hw_bt_fake_close();
        mutex_unlock(&(pm_top_data->host_mutex));
        ps_print_err("bt_fake_close_flag is %d ,not really close bt!", pm_data->bt_fake_close_flag);
        return BFGX_POWER_SUCCESS;
    }

    ret = hw_bfgx_close(BFGX_BT);

    oal_wake_unlock_force(&pm_data->bt_wake_lock);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}
#endif

/*
 * Prototype    : hw_ir_open
 * Description  : open ir device
 */
STATIC int32 hw_ir_open(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_top* pm_top_data = pm_get_top();

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    /* judge ir only mode */
    if ((wlan_is_shutdown() == true) && (bfgx_is_shutdown() == true)
        && ((get_hi110x_asic_type() == HI1103_ASIC_PILOT) || (get_hi110x_asic_type() == HI1105_ASIC))) {
        oal_atomic_set(&g_ir_only_mode, 1);
    }

    ret = hw_bfgx_open(BFGX_IR);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/*
 * Prototype    : hw_ir_read
 * Description  : read ir node data
 */
STATIC ssize_t hw_ir_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    uint16 ret_count;
    struct sk_buff *skb = NULL;
    struct ps_core_s *ps_core_d = NULL;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if ((skb = ps_skb_dequeue(ps_core_d, RX_IR_QUEUE)) == NULL) {
        ps_print_dbg("ir read skb queue is null!\n");
        return 0;
    }

    ret_count = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, ret_count)) {
        ps_print_err("copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_IR_QUEUE);
        return -EFAULT;
    }

    skb_pull(skb, ret_count);

    if (skb->len == 0) {
        kfree_skb(skb);
    } else {
        ps_restore_skbqueue(ps_core_d, skb, RX_IR_QUEUE);
    }

    return ret_count;
}

/*
 * Prototype    : hw_ir_write
 * Description  : write data to ir node
 */
STATIC ssize_t hw_ir_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 ret = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);

    ret = hw_bfgx_write_check(filp, buf, ps_core_d, BFGX_IR);
    if (ret < 0) {
        return ret;
    }

    if (count > IR_TX_MAX_FRAME) {
        ps_print_err("IR skb len is too large!\n");
        return -EINVAL;
    }

    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        return 0;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work fail, bring to reset work\n");
        plat_exception_handler(SUBSYS_BFGX, THREAD_IR, BFGX_WAKEUP_FAIL);
        return ret;
    }

    if (oal_atomic_read(&g_ir_only_mode) == 0) {
        /* modify expire time of uart idle timer */
        mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
        ps_core_d->pm_data->bfg_timer_mod_cnt++;
    }

    /* to divide up packet function and tx to tty work */
    if (ps_h2d_tx_data(ps_core_d, BFGX_IR, buf, count) < 0) {
        ps_print_err("hw_ir_write is err\n");
        post_to_visit_node(ps_core_d);
        return -EFAULT;
    }

    ps_core_d->bfgx_info[BFGX_IR].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    ps_print_dbg("IR data write end\n");

    return count;
}

STATIC int32 hw_ir_release(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_top* pm_top_data = pm_get_top();

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_close(BFGX_IR);
    oal_atomic_set(&g_ir_only_mode, 0);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/*
 * Prototype    : hw_fm_open
 * Description  : functions called from above fm hal,when open fm file
 * input        : "/dev/hwfm"
 * output       : return 0 --> open is ok
 *              : return !0--> open is false
 */
STATIC int32 hw_fm_open(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_open(BFGX_FM);

    ps_core_d->bfgx_info[BFGX_FM].read_delay = FM_READ_DEFAULT_TIME;

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/*
 * Prototype    : hw_fm_read
 * Description  : functions called from above fm hal,read count data to buf
 */
STATIC ssize_t hw_fm_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 count1;
    int64 timeout;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (ps_core_d->bfgx_info[BFGX_FM].rx_queue.qlen == 0) { /* if don,t data, and wait timeout function */
        if (filp->f_flags & O_NONBLOCK) {                   /* if O_NONBLOCK read and return */
            return -EAGAIN;
        }
        /* timeout function;when have data,can interrupt */
        timeout = wait_event_interruptible_timeout(ps_core_d->bfgx_info[BFGX_FM].rx_wait,
                                                   (ps_core_d->bfgx_info[BFGX_FM].rx_queue.qlen > 0),
                                                   msecs_to_jiffies(ps_core_d->bfgx_info[BFGX_FM].read_delay));
        if (!timeout) {
            ps_print_dbg("fm read time out!\n");
            return -ETIMEDOUT;
        }
    }

    if ((skb = ps_skb_dequeue(ps_core_d, RX_FM_QUEUE)) == NULL) {
        ps_print_warning("fm read no data!\n");
        return -ETIMEDOUT;
    }

    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_FM_QUEUE);
        return -EFAULT;
    }

    skb_pull(skb, count1);

    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        ps_restore_skbqueue(ps_core_d, skb, RX_FM_QUEUE);
    }

    return count1;
}

/*
 * Prototype    : hw_fm_write
 * Description  : functions called from above fm hal,write count data to buf
 */
STATIC ssize_t hw_fm_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 ret = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);

    ret = hw_bfgx_write_check(filp, buf, ps_core_d, BFGX_FM);
    if (ret < 0) {
        return ret;
    }

    /* if count is too large;and don,t tx */
    if (count > (FM_TX_MAX_FRAME - sizeof(struct ps_packet_head) - sizeof(struct ps_packet_end))) {
        ps_print_err("err:fm packet is too large!\n");
        return -EINVAL;
    }

    /* if low queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        return 0;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work fail, bring to reset work\n");
        plat_exception_handler(SUBSYS_BFGX, THREAD_FM, BFGX_WAKEUP_FAIL);
        return ret;
    }

    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
    ps_core_d->pm_data->bfg_timer_mod_cnt++;

    /* to divide up packet function and tx to tty work */
    if (ps_h2d_tx_data(ps_core_d, BFGX_FM, buf, count) < 0) {
        ps_print_err("hw_fm_write is err\n");
        post_to_visit_node(ps_core_d);
        return -EFAULT;
    }
    ps_core_d->bfgx_info[BFGX_FM].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    ps_print_dbg("FM data write end\n");

    return count;
}

/*
 * Prototype    : hw_fm_ioctl
 * Description  : called by hw func from hal when open power gpio or close power gpio
 */
STATIC int64 hw_fm_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    struct ps_core_s *ps_core_d = NULL;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (file == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (cmd == FM_SET_READ_TIME) {
        if (arg < FM_MAX_READ_TIME) { /* set timeout for fm read function */
            ps_core_d->bfgx_info[BFGX_FM].read_delay = arg;
        } else {
            ps_print_err("arg is too large!\n");
            return -EINVAL;
        }
    }

    return 0;
}

/*
 * Prototype    : hw_fm_release
 * Description  : called by fm func from hal when close fm inode
 * input        : have opened file handle
 * output       : return 0 --> close is ok
 *                return !0--> close is false
 */
STATIC int32 hw_fm_release(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_top* pm_top_data = pm_get_top();

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_close(BFGX_FM);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/* device bfgx pm debug switch on/off */
void plat_pm_debug_switch(void)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 ret;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, BUART);

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return;
    }

    ps_tx_sys_cmd(ps_core_d, SYS_MSG, PL_PM_DEBUG);

    post_to_visit_node(ps_core_d);

    return;
}

/*
 * Prototype    : hw_gnss_open
 * Description  : functions called from above gnss hal,when open gnss file
 * input        : "/dev/hwgnss"
 * output       : return 0 --> open is ok
 *                return !0--> open is false
 */
STATIC int32 hw_gnss_open(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    pm_data = ps_core_d->pm_data;
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    atomic_set(&pm_data->gnss_sleep_flag, GNSS_NOT_AGREE_SLEEP);

    ret = hw_bfgx_open(BFGX_GNSS);

    ps_core_d->bfgx_info[BFGX_GNSS].read_delay = GNSS_READ_DEFAULT_TIME;

    if (ret != BFGX_POWER_SUCCESS) {
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
    }

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

/*
 * Prototype    : hw_gnss_poll
 * Description  : called by gnss func from hal;
 *                check whether or not allow read and write
 */
STATIC uint32 hw_gnss_poll(struct file *filp, poll_table *wait)
{
    struct ps_core_s *ps_core_d = NULL;
    uint32 mask = 0;

    ps_print_dbg("%s\n", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    /* push curr wait event to wait queue */
    poll_wait(filp, &ps_core_d->bfgx_info[BFGX_GNSS].rx_wait, wait);

    ps_print_dbg("%s, recive gnss data\n", __func__);

    if (ps_core_d->bfgx_info[BFGX_GNSS].rx_queue.qlen) { /* have data to read */
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

/*
 * Prototype    : hw_gnss_read
 * Description  : functions called from above gnss hal,read count data to buf
 */
STATIC ssize_t hw_gnss_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    struct sk_buff_head read_queue;
    int32 count1;
    uint8 seperate_tag = GNSS_SEPER_TAG_INIT;
    int32 copy_cnt = 0;
    uint32 ret;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    skb_queue_head_init(&read_queue);

    spin_lock(&ps_core_d->bfgx_info[BFGX_GNSS].rx_lock);
    do {
        if ((skb = ps_skb_dequeue(ps_core_d, RX_GNSS_QUEUE)) == NULL) {
            spin_unlock(&ps_core_d->bfgx_info[BFGX_GNSS].rx_lock);
            if (read_queue.qlen != 0) {
                // 没有找到last包，skb queue就空了
                ps_print_err("skb dequeue error, qlen=%x!\n", read_queue.qlen);
                goto skb_dequeue_error;
            } else {
                ps_print_info("gnss read no data!\n");
                return 0;
            }
        }

        seperate_tag = skb->data[skb->len - 1];
        switch (seperate_tag) {
            case GNSS_SEPER_TAG_INIT:
            case GNSS_SEPER_TAG_LAST:
                break;
            default:
                ps_print_err("seperate_tag=%x not support\n", seperate_tag);
                seperate_tag = GNSS_SEPER_TAG_LAST;
                break;
        }

        skb_queue_tail(&read_queue, skb);
    } while (seperate_tag == GNSS_SEPER_TAG_INIT);
    spin_unlock(&ps_core_d->bfgx_info[BFGX_GNSS].rx_lock);

    copy_cnt = 0;
    do {
        skb = skb_dequeue(&read_queue);
        if (skb == NULL) {
            ps_print_err("copy dequeue error, copy_cnt=%x\n", copy_cnt);
            goto skb_dequeue_error;
        }

        if (skb->len <= 1) {
            ps_print_err("skb len error,skb->len=%x,copy_cnt=%x,count=%x\n", skb->len, copy_cnt, (uint32)count);
            goto copy_error;
        }

        count1 = skb->len - 1;
        if (count1 + copy_cnt > count) {
            ps_print_err("copy total len error,skb->len=%x,tag=%x,copy_cnt=%x,read_cnt=%x\n",
                         skb->len, skb->data[skb->len - 1], copy_cnt, (uint32)count);
            goto copy_error;
        }

        ret = copy_to_user(buf + copy_cnt, skb->data, count1);
        if (ret != 0) {
            ps_print_err("copy_to_user err,ret=%x,dest=%p,src=%p,tag:%x,count1=%x,copy_cnt=%x,read_cnt=%x\n",
                         ret, buf + copy_cnt, skb->data, skb->data[skb->len - 1], count1, copy_cnt, (uint32)count);
            goto copy_error;
        }

        copy_cnt += count1;
        kfree_skb(skb);
    } while (read_queue.qlen != 0);

    return copy_cnt;

copy_error:
    kfree_skb(skb);
skb_dequeue_error:
    while ((skb = skb_dequeue(&read_queue)) != NULL) {
        ps_print_err("free skb: len=%x, tag=%x\n", skb->len, skb->data[skb->len - 1]);
        kfree_skb(skb);
    }

    return -EFAULT;
}

/*
 * Prototype    : hw_gnss_write
 * Description  : functions called from above gnss hal,write count data to buf
 */
STATIC ssize_t hw_gnss_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 ret = 0;
    struct pm_drv_data *pm_data = NULL;
    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    pm_data = ps_core_d->pm_data;
    if (unlikely(pm_data == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    ret = hw_bfgx_write_check(filp, buf, ps_core_d, BFGX_GNSS);
    if (ret < 0) {
        return ret;
    }

    if (count > GNSS_TX_MAX_FRAME) {
        ps_print_err("err:gnss packet is too large!\n");
        return -EINVAL;
    }

    /* if low queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        return 0;
    }

    atomic_set(&pm_data->gnss_sleep_flag, GNSS_NOT_AGREE_SLEEP);
    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
        ps_print_err("prepare work fail, bring to reset work\n");
        plat_exception_handler(SUBSYS_BFGX, THREAD_GNSS, BFGX_WAKEUP_FAIL);
        return ret;
    }

    oal_wake_lock_timeout(&ps_core_d->pm_data->gnss_wake_lock, DEFAULT_WAKELOCK_TIMEOUT);

    /* to divide up packet function and tx to tty work */
    if (ps_h2d_tx_data(ps_core_d, BFGX_GNSS, buf, count) < 0) {
        ps_print_err("hw_gnss_write is err\n");
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
        count = -EFAULT;
    }

    ps_core_d->bfgx_info[BFGX_GNSS].tx_pkt_num++;

    post_to_visit_node(ps_core_d);

    return count;
}

#ifdef CONFIG_HI110X_GPS_REFCLK
STATIC int32 plat_gnss_refclk_para_set(uint64 arg)
{
    gps_refclk_param para = { 0 };
    uint32 __user *puser = (uint32 __user *)(uintptr_t)arg;
    if (copy_from_user(&para, puser, sizeof(gps_refclk_param))) {
        ps_print_err("get gnss ref clk params error\n");
        return -EINVAL;
    }
    return set_gps_ref_clk_enable_hi110x(para.enable, para.modem_id, para.rat);
}
#endif

/*
 * Prototype    : hw_gnss_ioctl
 * Description  : called by gnss func from hal when open power gpio or close power gpio
 */
STATIC int64 hw_gnss_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 __user *puser;
    oal_int32 coeff_para[COEFF_NUM];

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (file == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    switch (cmd) {
        case GNSS_SET_READ_TIME:
            if (arg < GNSS_MAX_READ_TIME) { /* set timeout for gnss read function */
                ps_core_d->bfgx_info[BFGX_GNSS].read_delay = arg;
            } else {
                ps_print_err("arg is too large!\n");
                return -EINVAL;
            }
            break;
        case PLAT_GNSS_DCXO_SET_PARA_CMD:
            puser = (int32 __user *)arg;
            if (copy_from_user(coeff_para, puser, COEFF_NUM * sizeof(oal_int32))) {
                ps_print_err("[dcxo] get gnss update para error\n");
                return -EINVAL;
            }
            update_dcxo_coeff(coeff_para, COEFF_NUM * sizeof(oal_int32));
            break;
#ifdef CONFIG_HI110X_GPS_REFCLK
        case PLAT_GNSS_REFCLK_PARA_CMD:
            return plat_gnss_refclk_para_set(arg);
#endif
        default:
            ps_print_warning("hw_gnss_ioctl cmd = %d not find\n", cmd);
            return -EINVAL;
    }

    return 0;
}

/*
 * Prototype    : hw_gnss_release
 * Description  : called by gnss func from hal when close gnss inode
 */
STATIC int32 hw_gnss_release(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_drv_data *pm_data = pm_get_drvdata(BUART);
    struct pm_top* pm_top_data = pm_get_top();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EINVAL;
    }

    if (unlikely((inode == NULL) || (filp == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_close(BFGX_GNSS);

    oal_wake_unlock_force(&pm_data->gnss_wake_lock);

    atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

STATIC int32 plat_excp_dump_rotate_cmd_read(uint64 arg, memdump_info_t *memdump_info)
{
    uint32 __user *puser = (uint32 __user *)(uintptr_t)arg;
    struct sk_buff *skb = NULL;

    if (!oal_access_write_ok(puser, (unsigned long)sizeof(uint32))) {
        ps_print_err("address can not write\n");
        return -EINVAL;
    }

    if (wait_event_interruptible(memdump_info->dump_type_wait, (skb_queue_len(&memdump_info->dump_type_queue)) > 0)) {
        return -EINVAL;
    }

    skb = skb_dequeue(&memdump_info->dump_type_queue);
    if (skb == NULL) {
        ps_print_warning("skb is NULL\n");
        return -EINVAL;
    }

    if (copy_to_user(puser, skb->data, sizeof(uint32))) {
        ps_print_warning("copy_to_user err!restore it, len=%d,arg=%ld\n", (int32)sizeof(uint32), arg);
        skb_queue_head(&memdump_info->dump_type_queue, skb);
        return -EINVAL;
    }

    ps_print_info("read rotate cmd [%d] from queue\n", *(uint32 *)skb->data);

    skb_pull(skb, skb->len);
    kfree_skb(skb);

    return 0;
}

int32 plat_bfgx_dump_rotate_cmd_read(struct ps_core_s *ps_core_d, uint64 arg)
{
    uint8 index = ps_core_d->dev_index;

    if (index == BUART) {
        return plat_excp_dump_rotate_cmd_read(arg, &g_bcpu_memdump_cfg);
    } else if (index == GUART) {
        return plat_excp_dump_rotate_cmd_read(arg, &g_gcpu_memdump_cfg);
    } else {
        ps_print_err("unknown dev index:%d\n", index);
        return -EINVAL;
    }
}

STATIC int32 plat_wifi_dump_rotate_cmd_read(uint64 arg)
{
    return plat_excp_dump_rotate_cmd_read(arg, &g_wcpu_memdump_cfg);
}

/*
 * Prototype    : hw_debug_ioctl
 * Description  : called by ini_plat_dfr_set
 */
STATIC int64 hw_debug_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    if (file == NULL) {
        ps_print_err("file is null\n");
        return -EINVAL;
    }

    switch (cmd) {
        case PLAT_DFR_CFG_CMD:
            plat_dfr_cfg_set(arg);
            break;
        case PLAT_BEATTIMER_TIMEOUT_RESET_CFG_CMD:
            plat_beat_timeout_reset_set(arg);
            break;
        case PLAT_BFGX_CALI_CMD:
#ifdef _PRE_BFGX_CALI
#else
            bfgx_cali_data_init();
#endif
            break;
        default:
            ps_print_warning("hw_debug_ioctl cmd = %d not find\n", cmd);
            return -EINVAL;
    }

    return 0;
}

STATIC int32 arm_timeout_submit(enum BFGX_THREAD_ENUM subs)
{
#define DFR_SUBMIT_LIMIT_TIME 300 /* second */
    static unsigned long long dfr_submit_last_time = 0;
    unsigned long long dfr_submit_current_time;
    unsigned long long dfr_submit_interval_time;
    struct timespec dfr_submit_time;

    if (subs >= BFGX_THREAD_BOTTOM) {
        return -EINVAL;
    }

    ps_print_info("[subs id:%d]arm timeout trigger", subs);

    dfr_submit_time = current_kernel_time();
    dfr_submit_current_time = dfr_submit_time.tv_sec;
    dfr_submit_interval_time = dfr_submit_current_time - dfr_submit_last_time;

    /* 5分钟内最多触发一次 */
    if ((dfr_submit_interval_time > DFR_SUBMIT_LIMIT_TIME) || (dfr_submit_last_time == 0)) {
        dfr_submit_last_time = dfr_submit_current_time;
        plat_exception_handler(SUBSYS_BFGX, subs, BFGX_ARP_TIMEOUT);
        return 0;
    } else {
        ps_print_err("[subs id:%d]arm timeout cnt max than limit", subs);
        return -EAGAIN;
    }
}

STATIC int32 hw_excp_read(struct file *filp, int8 __user *buf,
                          size_t count, loff_t *f_pos, memdump_info_t *memdump_t)
{
    struct sk_buff *skb = NULL;
    uint16 count1;

    if (unlikely((buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }
    if ((skb = skb_dequeue(&memdump_t->quenue)) == NULL) {
        return 0;
    }

    /* read min value from skb->len or count */
    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("copy_to_user is err!\n");
        skb_queue_head(&memdump_t->quenue, skb);
        return -EFAULT;
    }

    /* have read count1 byte */
    skb_pull(skb, count1);

    /* if skb->len = 0: read is over */
    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        skb_queue_head(&memdump_t->quenue, skb);
    }

    return count1;
}

STATIC ssize_t hw_bfgexcp_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    return hw_excp_read(filp, buf, count, f_pos, &g_bcpu_memdump_cfg);
}

STATIC int64 hw_bfgexcp_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    int32 ret = 0;
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    switch (cmd) {
        case PLAT_BFGX_DUMP_FILE_READ_CMD:
            ret = plat_bfgx_dump_rotate_cmd_read(ps_core_d, arg);
            break;
        case DFR_HAL_GNSS_CFG_CMD:
            return arm_timeout_submit(THREAD_GNSS);
        case DFR_HAL_BT_CFG_CMD:
            return arm_timeout_submit(THREAD_BT);
        case DFR_HAL_FM_CFG_CMD:
            return arm_timeout_submit(THREAD_FM);
        default:
            ps_print_warning("hw_debug_ioctl cmd = %d not find\n", cmd);
            return -EINVAL;
    }

    return ret;
}

STATIC int64 hw_wifiexcp_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    int32 ret = 0;

    if (file == NULL) {
        ps_print_err("file is null\n");
        return -EINVAL;
    }
    switch (cmd) {
        case PLAT_WIFI_DUMP_FILE_READ_CMD:
            ret = plat_wifi_dump_rotate_cmd_read(arg);
            break;
        default:
            ps_print_warning("hw_debug_ioctl cmd = %d not find\n", cmd);
            return -EINVAL;
    }

    return ret;
}

STATIC ssize_t hw_wifiexcp_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    return hw_excp_read(filp, buf, count, f_pos, &g_wcpu_memdump_cfg);
}

/*
 * Prototype    : hw_debug_open
 * Description  : functions called from above oam hal,when open debug file
 * input        : "/dev/hwbfgdbg"
 * output       : return 0 --> open is ok
 *              : return !0--> open is false
 */
STATIC int32 hw_debug_open(struct inode *inode, struct file *filp)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    oal_atomic_inc(&g_debug_cnt);
    ps_print_info("%s debug_cnt=%d\n", __func__, oal_atomic_read(&g_debug_cnt));

    oal_atomic_set(&ps_core_d->dbg_func_has_open, 1);

    oal_atomic_set(&ps_core_d->dbg_read_delay, DBG_READ_DEFAULT_TIME);

    return 0;
}

STATIC ssize_t hw_debug_read(struct file *filp, int8 __user *buf,
                             size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 count1 = 0;
    int64 timeout;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (ps_core_d->rx_dbg_seq.qlen == 0) { /* if no data, and wait timeout function */
        if (filp->f_flags & O_NONBLOCK) {  /* if O_NONBLOCK read and return */
            return -EAGAIN;
        }

        /* timeout function;when have data,can interrupt */
        timeout = wait_event_interruptible_timeout(ps_core_d->rx_dbg_wait,
                                                   (ps_core_d->rx_dbg_seq.qlen > 0),
                                                   msecs_to_jiffies(oal_atomic_read(&ps_core_d->dbg_read_delay)));
        if (!timeout) {
            ps_print_dbg("debug read time out!\n");
            return -ETIMEDOUT;
        }
    }

    /* pull skb data from skb queue */
    if ((skb = ps_skb_dequeue(ps_core_d, RX_DBG_QUEUE)) == NULL) {
        ps_print_dbg("dbg read no data!\n");
        return -ETIMEDOUT;
    }
    /* read min value from skb->len or count */
    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("debug copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_DBG_QUEUE);
        return -EFAULT;
    }

    skb_pull(skb, count1);

    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        ps_restore_skbqueue(ps_core_d, skb, RX_DBG_QUEUE);
    }

    return count1;
}

/*
 * Prototype    : hw_debug_write
 * Description  : functions called from above oam hal,write count data to buf
 */
#ifdef PLATFORM_DEBUG_ENABLE
STATIC int32 hw_debug_write_check(struct ps_core_s *ps_core_d)
{
    if (ps_core_d->tty_have_open == false) {
        ps_print_err("err: uart not opened!\n");
        return -EFAULT;
    }

    if (is_bfgx_exception() == PLAT_EXCEPTION_RESET_BUSY) {
        ps_print_err("dfr is processing, skip.\n");
        return -EBUSY;
    }

    return OAL_SUCC;
}

STATIC ssize_t hw_debug_write(struct file *filp, const int8 __user *buf,
                              size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb;
    uint16 total_len;
    int32 ret = 0;

    PS_PRINT_FUNCTION_NAME;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (hw_debug_write_check(ps_core_d) < 0) {
        ps_print_err("check failed\n");
        return -EINVAL;
    }

    if (count > (DBG_TX_MAX_FRAME - sizeof(struct ps_packet_head) - sizeof(struct ps_packet_end))) {
        ps_print_err("err: dbg packet is too large!\n");
        return -EINVAL;
    }

    /* if low queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        return 0;
    }

    if (ps_chk_bfg_active(ps_core_d) == false) {
        ps_print_err("bfg is closed, /dev/hwdebug cant't write!!!\n");
        return -EINVAL;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return ret;
    }
    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
    ps_core_d->pm_data->bfg_timer_mod_cnt++;

    total_len = count + sizeof(struct ps_packet_head) + sizeof(struct ps_packet_end);

    skb = ps_alloc_skb(total_len);
    if (skb == NULL) {
        ps_print_err("ps alloc skb mem fail\n");
        post_to_visit_node(ps_core_d);
        return -EFAULT;
    }

    if (copy_from_user(&skb->data[sizeof(struct ps_packet_head)], buf, count)) {
        ps_print_err("copy_from_user from dbg is err\n");
        kfree_skb(skb);
        post_to_visit_node(ps_core_d);
        return -EFAULT;
    }

    ps_add_packet_head(skb->data, OML_MSG, total_len);
    ps_skb_enqueue(ps_core_d, skb, TX_LOW_QUEUE);
    queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

    post_to_visit_node(ps_core_d);

    return count;
}
#endif

/*
 * Prototype    : hw_debug_release
 * Description  : called by oam func from hal when close debug inode
 * input        : have opened file handle
 * output       : return 0 --> close is ok
 *                return !0--> close is false
 */
STATIC int32 hw_debug_release(struct inode *inode, struct file *filp)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    oal_atomic_dec(&g_debug_cnt);
    ps_print_info("%s debug_cnt=%d", __func__, oal_atomic_read(&g_debug_cnt));
    if (oal_atomic_read(&g_debug_cnt) == 0) {
        /* wake up bt dbg wait queue */
        wake_up_interruptible(&ps_core_d->rx_dbg_wait);
        atomic_set(&ps_core_d->dbg_func_has_open, 0);

        /* kfree have rx dbg skb */
        ps_kfree_skb(ps_core_d, RX_DBG_QUEUE);
    }

    return 0;
}

uart_loop_cfg g_uart_loop_test_cfg = { 256, 60000, 0, 0, 0 };
uart_loop_test_struct *g_uart_loop_test_info = NULL;

STATIC void uart_loop_test_tx_buf_init(uint8 *puc_data, uint16 us_data_len)
{
    get_random_bytes(puc_data, us_data_len);
}

STATIC int32 alloc_uart_loop_test(void)
{
    uint8 *uart_loop_tx_buf = NULL;
    uint8 *uart_loop_rx_buf = NULL;
    uint16 pkt_len = 0;

    if (g_uart_loop_test_info == NULL) {
        g_uart_loop_test_info = (uart_loop_test_struct *)kzalloc(sizeof(uart_loop_test_struct), GFP_KERNEL);
        if (g_uart_loop_test_info == NULL) {
            ps_print_err("malloc uart_loop_test_info fail\n");
            goto malloc_test_info_fail;
        }

        pkt_len = g_uart_loop_test_cfg.pkt_len;
        if (pkt_len == 0 || pkt_len > UART_LOOP_MAX_PKT_LEN) {
            pkt_len = UART_LOOP_MAX_PKT_LEN;
            g_uart_loop_test_cfg.pkt_len = UART_LOOP_MAX_PKT_LEN;
        }

        uart_loop_tx_buf = (uint8 *)kzalloc(pkt_len, GFP_KERNEL);
        if (uart_loop_tx_buf == NULL) {
            ps_print_err("malloc uart_loop_tx_buf fail\n");
            goto malloc_tx_buf_fail;
        }

        memset_s(uart_loop_tx_buf, pkt_len, 0xa5, pkt_len);

        uart_loop_rx_buf = (uint8 *)kzalloc(pkt_len, GFP_KERNEL);
        if (uart_loop_rx_buf == NULL) {
            ps_print_err("malloc uart_loop_rx_buf fail\n");
            goto malloc_rx_buf_fail;
        }

        g_uart_loop_test_cfg.uart_loop_enable = 1;
        g_uart_loop_test_cfg.uart_loop_tx_random_enable = 1;

        init_completion(&g_uart_loop_test_info->set_done);
        init_completion(&g_uart_loop_test_info->loop_test_done);

        g_uart_loop_test_info->test_cfg = &g_uart_loop_test_cfg;
        g_uart_loop_test_info->tx_buf = uart_loop_tx_buf;
        g_uart_loop_test_info->rx_buf = uart_loop_rx_buf;
        g_uart_loop_test_info->rx_pkt_len = 0;

        ps_print_info("uart loop test, pkt len is [%d]\n", pkt_len);
        ps_print_info("uart loop test, loop count is [%d]\n", g_uart_loop_test_cfg.loop_count);
    }

    return 0;

malloc_rx_buf_fail:
    kfree(uart_loop_tx_buf);
malloc_tx_buf_fail:
    kfree(g_uart_loop_test_info);
    g_uart_loop_test_info = NULL;
malloc_test_info_fail:
    return -ENOMEM;
}

STATIC void free_uart_loop_test(void)
{
    if (g_uart_loop_test_info == NULL) {
        return;
    }
    ps_print_err("free uart loop test buf\n");
    g_uart_loop_test_cfg.uart_loop_enable = 0;
    kfree(g_uart_loop_test_info->rx_buf);
    kfree(g_uart_loop_test_info->tx_buf);
    kfree(g_uart_loop_test_info);
    g_uart_loop_test_info = NULL;

    return;
}

STATIC int32 uart_loop_test_open(void)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 error;

    ps_print_info("%s\n", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->pm_data == NULL) || (ps_core_d->pm_data->bfg_power_set == NULL))) {
        ps_print_err("ps_core_d is err\n");
        return -EINVAL;
    }

    if (ps_chk_bfg_active(ps_core_d)) {
        ps_print_err("bfgx subsys must all close\n");
        return -EINVAL;
    }

    if (alloc_uart_loop_test() != BFGX_POWER_SUCCESS) {
        ps_print_err("alloc mem for uart loop test fail!\n");
        goto alloc_mem_fail;
    }

    error = ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, BFGX_GNSS, BFG_POWER_GPIO_UP);
    if (error != BFGX_POWER_SUCCESS) {
        ps_print_err("uart loop test, power on err! error = %d\n", error);
        goto power_on_fail;
    }

    if (prepare_to_visit_node(ps_core_d) != BFGX_POWER_SUCCESS) {
        ps_print_err("uart loop test, prepare work fail\n");
        error = BFGX_POWER_WAKEUP_FAIL;
        goto wakeup_fail;
    }

    post_to_visit_node(ps_core_d);

    return BFGX_POWER_SUCCESS;

wakeup_fail:
    ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, BFGX_GNSS, BFG_POWER_GPIO_DOWN);
power_on_fail:
    free_uart_loop_test();
alloc_mem_fail:
    return BFGX_POWER_FAILED;
}

STATIC int32 uart_loop_test_close(void)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->pm_data == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (prepare_to_visit_node(ps_core_d) != 0) {
        ps_print_err("uart loop test, prepare work fail\n");
    }

    if (ps_core_d->pm_data->bfg_power_set(ps_core_d->pm_data, BFGX_GNSS, BFG_POWER_GPIO_DOWN) != 0) {
        ps_print_err("uart loop test, power off err!");
    }

    free_uart_loop_test();

    post_to_visit_node(ps_core_d);

    return 0;
}

STATIC int32 uart_loop_test_set(uint8 flag)
{
    uint64 timeleft;
    struct ps_core_s *ps_core_d = NULL;
    uint8 cmd;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is null\n");
        return -EINVAL;
    }

    if (flag == UART_LOOP_SET_DEVICE_DATA_HANDLER) {
        cmd = SYS_CFG_SET_UART_LOOP_HANDLER;
    } else {
        cmd = SYS_CFG_SET_UART_LOOP_FINISH;
    }

    oal_reinit_completion(g_uart_loop_test_info->set_done);
    ps_tx_sys_cmd(ps_core_d, SYS_MSG, cmd);
    timeleft = wait_for_completion_timeout(&g_uart_loop_test_info->set_done, msecs_to_jiffies(5000));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait set uart loop ack timeout\n");
        return -ETIMEDOUT;
    }

    return 0;
}

STATIC int32 uart_loop_test_send_data(struct ps_core_s *ps_core_d, uint8 *buf, size_t count)
{
    struct sk_buff *skb = NULL;
    uint16 tx_skb_len;
    uint16 tx_gnss_len;
    uint8 start = 0;

    PS_PRINT_FUNCTION_NAME;

    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    while (count > 0) {
        if (count > PS_TX_PACKET_LIMIT) {
            tx_gnss_len = PS_TX_PACKET_LIMIT;
        } else {
            tx_gnss_len = count;
        }
        /* curr tx skb total lenth */
        tx_skb_len = tx_gnss_len + sizeof(struct ps_packet_head);
        tx_skb_len = tx_skb_len + sizeof(struct ps_packet_end);

        skb = ps_alloc_skb(tx_skb_len);
        if (skb == NULL) {
            ps_print_err("ps alloc skb mem fail\n");
            return -EFAULT;
        }

        if (count > PS_TX_PACKET_LIMIT) {
            if (start == false) { /* this is a start gnss packet */
                ps_add_packet_head(skb->data, GNSS_FIRST_MSG, tx_skb_len);
                start = true;
            } else { /* this is a int gnss packet */
                ps_add_packet_head(skb->data, GNSS_COMMON_MSG, tx_skb_len);
            }
        } else { /* this is the last gnss packet */
            ps_add_packet_head(skb->data, GNSS_LAST_MSG, tx_skb_len);
        }

        if (memcpy_s(&skb->data[sizeof(struct ps_packet_head)], tx_skb_len - sizeof(struct ps_packet_head),
                     buf, tx_gnss_len) != EOK) {
            ps_print_err("buf is not enough\n");
        }

        /* push the skb to skb queue */
        ps_skb_enqueue(ps_core_d, skb, TX_LOW_QUEUE);
        queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

        buf = buf + tx_gnss_len;
        count = count - tx_gnss_len;
    }

    return 0;
}

STATIC int32 uart_loop_test_send_pkt(void)
{
    uint64 timeleft;
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (g_uart_loop_test_info == NULL) ||
                 (g_uart_loop_test_info->tx_buf == NULL))) {
        ps_print_err("para is invalided\n");
        return -EFAULT;
    }

    /* if low queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        ps_print_err("uart loop test, tx low seq is too large [%d]\n", ps_core_d->tx_low_seq.qlen);
        return 0;
    }

    if (prepare_to_visit_node(ps_core_d) < 0) {
        ps_print_err("prepare work fail\n");
        return -EFAULT;
    }

    oal_reinit_completion(g_uart_loop_test_info->loop_test_done);

    /* to divide up packet function and tx to tty work */
    if (uart_loop_test_send_data(ps_core_d, g_uart_loop_test_info->tx_buf, g_uart_loop_test_cfg.pkt_len) < 0) {
        ps_print_err("uart loop test pkt send is err\n");
        post_to_visit_node(ps_core_d);
        return -EFAULT;
    }

    timeleft = wait_for_completion_timeout(&g_uart_loop_test_info->loop_test_done, msecs_to_jiffies(5000));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait uart loop done timeout\n");
        post_to_visit_node(ps_core_d);
        return -ETIMEDOUT;
    }

    post_to_visit_node(ps_core_d);

    return 0;
}

STATIC int32 uart_loop_test_data_check(uint8 *puc_src, uint8 *puc_dest, uint16 us_data_len)
{
    uint16 us_index;

    for (us_index = 0; us_index < us_data_len; us_index++) {
        if (puc_src[us_index] != puc_dest[us_index]) {
            return false;
        }
    }

    return true;
}

int32 uart_loop_test_recv_pkt(struct ps_core_s *ps_core_d, const uint8 *buf_ptr, uint16 pkt_len)
{
    uint16 expect_pkt_len;
    uint8 *rx_buf = NULL;
    uint16 recvd_len;

    if (unlikely((ps_core_d == NULL) || (g_uart_loop_test_info == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    expect_pkt_len = g_uart_loop_test_info->test_cfg->pkt_len;
    rx_buf = g_uart_loop_test_info->rx_buf;
    recvd_len = g_uart_loop_test_info->rx_pkt_len;

    if ((uint32)recvd_len + (uint32)pkt_len <= expect_pkt_len) {
        if (memcpy_s(&rx_buf[recvd_len], expect_pkt_len - recvd_len, buf_ptr, pkt_len) != EOK) {
            ps_print_err("memcpy_s error, destlen=%d, srclen=%d\n ", expect_pkt_len - recvd_len, pkt_len);
        }
        g_uart_loop_test_info->rx_pkt_len += pkt_len;
    } else {
        ps_print_err("pkt len err! pkt_len=[%d], recvd_len=[%d], max len=[%d]\n", pkt_len, recvd_len, expect_pkt_len);
    }

    if (expect_pkt_len == g_uart_loop_test_info->rx_pkt_len) {
        if (uart_loop_test_data_check(rx_buf, g_uart_loop_test_info->tx_buf, expect_pkt_len)) {
            ps_print_info("uart loop recv pkt SUCC\n");
        }
        g_uart_loop_test_info->rx_pkt_len = 0;
        complete(&g_uart_loop_test_info->loop_test_done);
    }

    return 0;
}

int32 uart_loop_test(void)
{
    uint32 i, count;
    uint16 pkt_len;
    unsigned long long tx_total_len;
    unsigned long long total_time = 0;
    unsigned long long throughout;
    unsigned long long effect;
    ktime_t start_time, end_time, trans_time;
    uint8 *puc_buf = NULL;

    if (uart_loop_test_open() < 0) {
        goto open_fail;
    }

    if (uart_loop_test_set(UART_LOOP_SET_DEVICE_DATA_HANDLER) < 0) {
        goto test_set_fail;
    }

    count = g_uart_loop_test_info->test_cfg->loop_count;
    pkt_len = g_uart_loop_test_info->test_cfg->pkt_len;
    tx_total_len = ((unsigned long long)count) * ((unsigned long long)pkt_len);
    puc_buf = g_uart_loop_test_info->tx_buf;

    for (i = 0; i < count; i++) {
        if (g_uart_loop_test_info->test_cfg->uart_loop_tx_random_enable) {
            uart_loop_test_tx_buf_init(puc_buf, pkt_len);  // 初始化tx_buf为随机数
        }

        start_time = ktime_get();

        if (uart_loop_test_send_pkt() != SUCCESS) {
            ps_print_err("uart loop test fail, i=[%d]\n", i);
            goto send_pkt_fail;
        }

        end_time = ktime_get();
        trans_time = ktime_sub(end_time, start_time);
        total_time += (unsigned long long)ktime_to_us(trans_time);
    }

    if (uart_loop_test_set(UART_LOOP_RESUME_DEVICE_DATA_HANDLER) < 0) {
        ps_print_err("uart loop test, resume device data handler failer\n");
    }

    uart_loop_test_close();

    /*
     *  将传输字节数转换成bit数(B->b)以方便后文计算
     *  1000000 :1M波特率
     *   10=8+2 :uart每传输1字节数据需要另加2bit的开始位和结束位
     *     2    :因为uart是环回的，发1字节数据还会回1字节数据
     */
    throughout = tx_total_len * 1000000 * 10 * 2;
    if (total_time == 0) {
        ps_print_err("divisor can not be zero!\n");
        return -FAILURE;
    }
    do_div(throughout, total_time);
    effect = throughout;
    do_div(throughout, 8192); /* b->B->KB : x*8*1024 */
    do_div(effect, (g_uart_default_baud_rate / 100)); /* 除以波特率算出百分比效率 */

    ps_print_info("[UART Test] pkt count      [%d] pkts sent\n", count);
    ps_print_info("[UART Test] pkt len        [%d] is pkt len\n", pkt_len);
    ps_print_info("[UART Test] data lenth     [%llu]\n",
                  tx_total_len * 2); /* uart是环回的，发1字节数据还会回1字节数据 */
    ps_print_info("[UART Test] used time      [%llu] us\n", total_time);
    ps_print_info("[UART Test] throughout     [%llu] KBps\n", throughout);
    ps_print_info("[UART Test] effect         [%llu]%%\n", effect);

    return SUCCESS;

send_pkt_fail:
test_set_fail:
    uart_loop_test_close();
open_fail:
    return -FAILURE;
}

int conn_test_uart_loop(char *param)
{
    return uart_loop_test();
}
EXPORT_SYMBOL(conn_test_uart_loop);

STATIC const struct file_operations g_hw_bt_fops = {
    .owner = THIS_MODULE,
    .open = hw_bt_open,
    .write = hw_bt_write,
    .read = hw_bt_read,
    .poll = hw_bt_poll,
    .unlocked_ioctl = hw_bt_ioctl,
    .release = hw_bt_release,
};

STATIC const struct file_operations g_hw_fm_fops = {
    .owner = THIS_MODULE,
    .open = hw_fm_open,
    .write = hw_fm_write,
    .read = hw_fm_read,
    .unlocked_ioctl = hw_fm_ioctl,
    .release = hw_fm_release,
};

STATIC const struct file_operations g_hw_gnss_fops = {
    .owner = THIS_MODULE,
    .open = hw_gnss_open,
    .write = hw_gnss_write,
    .read = hw_gnss_read,
    .poll = hw_gnss_poll,
    .unlocked_ioctl = hw_gnss_ioctl,
    .release = hw_gnss_release,
};

STATIC const struct file_operations g_hw_ir_fops = {
    .owner = THIS_MODULE,
    .open = hw_ir_open,
    .write = hw_ir_write,
    .read = hw_ir_read,
    .release = hw_ir_release,
};

STATIC const struct file_operations g_hw_debug_fops = {
    .owner = THIS_MODULE,
    .open = hw_debug_open,
#ifdef PLATFORM_DEBUG_ENABLE
    .write = hw_debug_write,
#endif
    .read = hw_debug_read,
    .unlocked_ioctl = hw_debug_ioctl,
    .release = hw_debug_release,
};

STATIC const struct file_operations g_hw_bfgexcp_fops = {
    .owner = THIS_MODULE,
    .read = hw_bfgexcp_read,
    .unlocked_ioctl = hw_bfgexcp_ioctl,
};

STATIC const struct file_operations g_hw_wifiexcp_fops = {
    .owner = THIS_MODULE,
    .read = hw_wifiexcp_read,
    .unlocked_ioctl = hw_wifiexcp_ioctl,
};

#ifdef HAVE_HISI_BT
STATIC struct miscdevice g_hw_bt_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwbt",
    .fops = &g_hw_bt_fops,
};
#endif

#ifdef HAVE_HISI_FM
STATIC struct miscdevice g_hw_fm_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwfm",
    .fops = &g_hw_fm_fops,
};
#endif

#ifdef HAVE_HISI_GNSS
STATIC struct miscdevice g_hw_gnss_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwgnss",
    .fops = &g_hw_gnss_fops,
};
#endif

#ifdef HAVE_HISI_IR
STATIC struct miscdevice g_hw_ir_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwir",
    .fops = &g_hw_ir_fops,
};
#endif

STATIC struct miscdevice g_hw_debug_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwbfgdbg",
    .fops = &g_hw_debug_fops,
};

STATIC struct miscdevice g_hw_bfgexcp_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwbfgexcp",
    .fops = &g_hw_bfgexcp_fops,
};

STATIC struct miscdevice g_hw_wifiexcp_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwwifiexcp",
    .fops = &g_hw_wifiexcp_fops,
};

static struct hw_ps_plat_data g_hisi_platform_data = {
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    .dev_name = "/dev/ttySC_SPI0",
#else
    .dev_name = "/dev/ttyAMA4",
#endif
    .flow_cntrl = FLOW_CTRL_ENABLE,
    .baud_rate = DEFAULT_BAUD_RATE,
    .ldisc_num = N_HW_BFG,
    .suspend = NULL,
    .resume = NULL,
};

#ifdef _PRE_CONFIG_USE_DTS
STATIC int32 ps_misc_dev_register(void)
{
    int32 err;

    if (is_bfgx_support() != OAL_TRUE) {
        /* don't support bfgx */
        ps_print_info("bfgx disabled, misc dev register bypass\n ");
        return 0;
    }

#ifdef HAVE_HISI_BT
    err = misc_register(&g_hw_bt_device);
    if (err != 0) {
        ps_print_err("Failed to register bt inode\n ");
        goto err_register_bt;
    }
#endif

#ifdef HAVE_HISI_FM
    err = misc_register(&g_hw_fm_device);
    if (err != 0) {
        ps_print_err("Failed to register fm inode\n ");
        goto err_register_fm;
    }
#endif

#ifdef HAVE_HISI_GNSS
    err = misc_register(&g_hw_gnss_device);
    if (err != 0) {
        ps_print_err("Failed to register gnss inode\n ");
        goto err_register_gnss;
    }
#endif

#ifdef HAVE_HISI_IR
    err = misc_register(&g_hw_ir_device);
    if (err != 0) {
        ps_print_err("Failed to register ir inode\n");
        goto err_register_ir;
    }
#endif

    return 0;

#ifdef HAVE_HISI_IR
    misc_deregister(&g_hw_ir_device);
err_register_ir:
#endif
#ifdef HAVE_HISI_GNSS
    misc_deregister(&g_hw_gnss_device);
err_register_gnss:
#endif
#ifdef HAVE_HISI_FM
    misc_deregister(&g_hw_fm_device);
err_register_fm:
#endif
#ifdef HAVE_HISI_BT
    misc_deregister(&g_hw_bt_device);
err_register_bt:
#endif
    return -EFAULT;
}

STATIC void ps_misc_dev_unregister(void)
{
    if (is_bfgx_support() != OAL_TRUE) {
        /* don't support bfgx */
        ps_print_info("bfgx disabled, misc dev unregister bypass\n ");
        return;
    }
#ifdef HAVE_HISI_BT
    misc_deregister(&g_hw_bt_device);
    ps_print_info("misc bt device have removed\n");
#endif
#ifdef HAVE_HISI_FM
    misc_deregister(&g_hw_fm_device);
    ps_print_info("misc fm device have removed\n");
#endif
#ifdef HAVE_HISI_GNSS
    misc_deregister(&g_hw_gnss_device);
    ps_print_info("misc gnss device have removed\n");
#endif
#ifdef HAVE_HISI_IR
    misc_deregister(&g_hw_ir_device);
    ps_print_info("misc ir have removed\n");
#endif
}

STATIC oal_void baudrate_init(oal_void)
{
    if (!hi110x_is_asic()) {
        g_hisi_platform_data.baud_rate = UART_BAUD_RATE_2M;
        g_uart_default_baud_rate = UART_BAUD_RATE_2M;
    } else {
        /* 从hi1105开始，波特率支持到8M，其他项目为4M */
        if (get_hi110x_subchip_type() >= BOARD_VERSION_HI1105) {
            g_hisi_platform_data.baud_rate = UART_BAUD_RATE_8M;
            g_uart_default_baud_rate = UART_BAUD_RATE_8M;
        } else {
            g_hisi_platform_data.baud_rate = UART_BAUD_RATE_4M;
            g_uart_default_baud_rate = UART_BAUD_RATE_4M;
        }
    }
    ps_print_info("init baudrate=%d\n", g_uart_default_baud_rate);
    return ;
}

STATIC int32 ps_probe(struct platform_device *pdev)
{
    struct hw_ps_plat_data *pdata = NULL;
    struct ps_plat_s *ps_plat_d = NULL;
    int32 err;
    hi110x_board_info *bd_info = NULL;

    bd_info = get_hi110x_board_info();
    if (unlikely(bd_info == NULL)) {
        ps_print_err("board info is err\n");
        return -FAILURE;
    }

    strncpy_s(g_hisi_platform_data.dev_name, sizeof(g_hisi_platform_data.dev_name),
              bd_info->uart_port, sizeof(g_hisi_platform_data.dev_name) - 1);
    g_hisi_platform_data.dev_name[sizeof(g_hisi_platform_data.dev_name) - 1] = '\0';

    pdev->dev.platform_data = &g_hisi_platform_data;
    pdata = &g_hisi_platform_data;

    baudrate_init();

    g_hw_ps_device = pdev;

    ps_plat_d = kzalloc(sizeof(struct ps_plat_s), GFP_KERNEL);
    if (ps_plat_d == NULL) {
        ps_print_err("no mem to allocate\n");
        return -ENOMEM;
    }
    dev_set_drvdata(&pdev->dev, ps_plat_d);

    err = ps_core_init(&ps_plat_d->core_data, BUART);
    if (err != 0) {
        ps_print_err(" PS core init failed\n");
        goto err_core_init;
    }

    /* refer to itself */
    ps_plat_d->core_data->ps_plat = ps_plat_d;
    /* get reference of pdev */
    ps_plat_d->pm_pdev = pdev;

    err = plat_bfgx_exception_rst_register(ps_plat_d);
    if (err < 0) {
        ps_print_err("bfgx_exception_rst_register failed\n");
        goto err_exception_rst_reg;
    }

    err = bfgx_user_ctrl_init();
    if (err < 0) {
        ps_print_err("bfgx_user_ctrl_init failed\n");
        goto err_user_ctrl_init;
    }

    err = bfgx_customize_init();
    if (err < 0) {
        ps_print_err("bfgx_customize_init failed\n");
        goto err_bfgx_custmoize_exit;
    }

    /* copying platform data */
    if (strncpy_s(ps_plat_d->dev_name, sizeof(ps_plat_d->dev_name),
                  pdata->dev_name, HISI_UART_DEV_NAME_LEN - 1) != EOK) {
        ps_print_err("strncpy_s failed, please check!\n");
    }
    ps_plat_d->flow_cntrl = pdata->flow_cntrl;
    ps_plat_d->baud_rate = pdata->baud_rate;
    ps_plat_d->ldisc_num = pdata->ldisc_num;

    st_tty_recv = ps_core_recv;

    err = ps_misc_dev_register();
    if (err != 0) {
        goto err_misc_dev;
    }

    err = misc_register(&g_hw_debug_device);
    if (err != 0) {
        ps_print_err("Failed to register debug inode\n");
        goto err_register_debug;
    }

    err = misc_register(&g_hw_bfgexcp_device);
    if (err != 0) {
        ps_print_err("Failed to register hw_bfgexcp_device inode\n");
        goto err_register_bfgexcp;
    }

    err = misc_register(&g_hw_wifiexcp_device);
    if (err != 0) {
        ps_print_err("Failed to register g_hw_wifiexcp_device inode\n");
        goto err_register_wifiexcp;
    }

#ifdef _PRE_BFGX_CALI
    /* PC产品定制化：开机bfgx校准 */
    bfgx_cali_data_init();
#endif

    ps_print_suc("%s is success!\n", __func__);

    return 0;

err_register_wifiexcp:
    misc_deregister(&g_hw_bfgexcp_device);
err_register_bfgexcp:
    misc_deregister(&g_hw_debug_device);
err_register_debug:
    ps_misc_dev_unregister();
err_misc_dev:
err_bfgx_custmoize_exit:
    bfgx_user_ctrl_exit();
err_user_ctrl_init:
err_exception_rst_reg:
    ps_core_exit(ps_plat_d->core_data, BUART);
err_core_init:
    kfree(ps_plat_d);

    return -EFAULT;
}

/*
 * Prototype    : ps_suspend
 * Description  : called by kernel when kernel goto suspend
 */
STATIC int32 ps_suspend(struct platform_device *pdev, pm_message_t state)
{
    return 0;
}

/*
 * Prototype    : ps_resume
 * Description  : called by kernel when kernel resume from suspend
 */
STATIC int32 ps_resume(struct platform_device *pdev)
{
    return 0;
}

/*
 * Prototype    : ps_remove
 * Description  : called when user applation rmmod driver
 */
STATIC int32 ps_remove(struct platform_device *pdev)
{
    struct ps_plat_s *ps_plat_d = NULL;

    misc_deregister(&g_hw_wifiexcp_device);
    misc_deregister(&g_hw_bfgexcp_device);
    misc_deregister(&g_hw_debug_device);
    ps_misc_dev_unregister();
    ps_print_info("misc device have removed\n");

    bfgx_user_ctrl_exit();
    ps_print_info("sysfs user ctrl removed\n");

    ps_plat_d = dev_get_drvdata(&pdev->dev);
    if (ps_plat_d == NULL) {
        ps_print_err("ps_plat_d is null\n");
        return -EFAULT;
    }

    ps_core_exit(ps_plat_d->core_data, BUART);
    kfree(ps_plat_d);
    ps_plat_d = NULL;

    return 0;
}

STATIC struct of_device_id g_hi110x_ps_match_table[] = {
    {
        .compatible = DTS_COMP_HI110X_PS_NAME,
        .data = NULL,
    },
    {},
};
#endif

/*  platform_driver struct for PS module */
STATIC struct platform_driver g_ps_platform_driver = {
#ifdef _PRE_CONFIG_USE_DTS
    .probe = ps_probe,
    .remove = ps_remove,
    .suspend = ps_suspend,
    .resume = ps_resume,
#endif
    .driver = {
        .name = "hisi_bfgx",
        .owner = THIS_MODULE,
#ifdef _PRE_CONFIG_USE_DTS
        .of_match_table = g_hi110x_ps_match_table,
#endif
    },
};

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
struct platform_device  g_ps_platform_device  = {
    .name          = "hisi_bfgx",
    .id          = 0,
};
#endif

int32 hw_ps_init(void)
{
    int32 ret;

    PS_PRINT_FUNCTION_NAME;

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    platform_device_register(&g_ps_platform_device);
#endif

    ret = platform_driver_register(&g_ps_platform_driver);
    if (ret) {
        ps_print_err("Unable to register platform bfgx driver.\n");
    }

    ret = hw_ps_me_init();
    if (ret) {
        platform_driver_unregister(&g_ps_platform_driver);
    }

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    if (g_hw_ps_device == NULL) {
        ps_print_err("g_hw_ps_device is NULL.\n");
        return -1;
    }
#endif

    return ret;
}

void hw_ps_exit(void)
{
    platform_driver_unregister(&g_ps_platform_driver);
    hw_ps_me_exit();
}

MODULE_DESCRIPTION("Public serial Driver for huawei BT/FM/GNSS chips");
MODULE_LICENSE("GPL");
