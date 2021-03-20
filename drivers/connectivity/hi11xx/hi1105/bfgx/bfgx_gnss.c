

/* Include Head file */
#include "bfgx_gnss.h"

#include "plat_debug.h"
#include "plat_pm.h"
#include "plat_uart.h"
#include "bfgx_dev.h"
#include "bfgx_exception_rst.h"
#include "bfgx_data_parse.h"

STATIC struct platform_device *g_hw_ps_me_device = NULL;
STATIC atomic_t g_me_oam_cnt = ATOMIC_INIT(0);
struct platform_device *get_me_platform_device(void)
{
    return g_hw_ps_me_device;
}

STATIC int32 gnss_me_open(void)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;

    ps_print_info("open gnss me\n");

    ps_get_core_reference(&ps_core_d, GUART);
    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_GNSS];

    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_OPEN) {
        ps_print_warning("gnss me has opened! It's Not necessary to send msg to device\n");
        return BFGX_POWER_SUCCESS;
    }

    if (!bfgx_is_boot_up(ps_core_d->pm_data)) {
        ps_print_err("gcpu is not boot up!\n");
        return BFGX_POWER_FAILED;
    }

    ret = alloc_seperted_rx_buf(ps_core_d, BFGX_GNSS, GNSS_RX_MAX_FRAME, VMALLOC);
    if (ret != 0) {
        ps_print_err("gnss me alloc failed! len=%d\n", GNSS_RX_MAX_FRAME);
        return ret;
    }

    if (open_tty_drv(ps_core_d) != BFGX_POWER_SUCCESS) {
        ps_print_err("gnss me open tty fail!\n");
        goto open_tty_fail;
    }

    ps_core_d->pm_data->bfgx_uart_state_set(ps_core_d->pm_data, UART_READY);
    ps_core_d->pm_data->bfgx_dev_state_set(ps_core_d->pm_data, BFGX_ACTIVE);

    if (bfgx_open_cmd_send(BFGX_GNSS, GUART) != BFGX_POWER_SUCCESS) {
        ps_print_err("bfgx open cmd fail\n");
        goto open_cmd_fail;
    }
    atomic_set(&pst_bfgx_data->subsys_state, POWER_STATE_OPEN);
    // 低功耗打开时使能bfgx_gpio_intr_enable(OAL_TRUE);
    msleep(100);

    return BFGX_POWER_SUCCESS;

open_cmd_fail:
    ps_core_d->pm_data->bfgx_uart_state_set(ps_core_d->pm_data, UART_NOT_READY);
    ps_core_d->pm_data->bfgx_dev_state_set(ps_core_d->pm_data, BFGX_SLEEP);
open_tty_fail:
    free_seperted_rx_buf(ps_core_d, BFGX_GNSS, VMALLOC);
    return BFGX_POWER_FAILED;
}

STATIC int32 gnss_me_close(void)
{
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;

    ps_get_core_reference(&ps_core_d, GUART);
    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_GNSS];

    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_SHUTDOWN) {
        ps_print_warning("gnss me has closed! It's Not necessary to send msg to device\n");
        return BFGX_POWER_SUCCESS;
    }

    ps_print_info("close gnss me\n");

    wake_up_interruptible(&pst_bfgx_data->rx_wait);

    ret = bfgx_close_cmd_send(BFGX_GNSS, GUART);
    if (ret < 0) {
        /* 发送close命令失败，不进行DFR，继续进行下电流程，DFR恢复延迟到下次open时或者其他业务运行时进行 */
        ps_print_err("bfgx close cmd fail\n");
    }
    atomic_set(&pst_bfgx_data->subsys_state, POWER_STATE_SHUTDOWN);

    if (release_tty_drv(ps_core_d) != SUCCESS) {
        /* 代码执行到此处，说明六合一所有业务都已经关闭，无论tty是否关闭成功，device都要下电 */
        ps_print_err("close gnss me tty is err!");
    }

    // 低功耗打开时使能bfgx_gpio_intr_enable(OAL_FALSE);
    free_seperted_rx_buf(ps_core_d, BFGX_GNSS, VMALLOC);
    ps_kfree_skb(ps_core_d, RX_GNSS_QUEUE);

    return 0;
}

STATIC int32 hw_gnss_me_open(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_drv_data *pm_data = pm_get_drvdata(GUART);
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

    atomic_set(&pm_data->gnss_sleep_flag, GNSS_NOT_AGREE_SLEEP);

    ret = gnss_me_open();
    if (ret != BFGX_POWER_SUCCESS) {
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
    }

    mutex_unlock(&(pm_top_data->host_mutex));
    return ret;
}

STATIC uint32 hw_gnss_me_poll(struct file *filp, poll_table *wait)
{
    struct ps_core_s *ps_core_d = NULL;
    uint32 mask = 0;

    ps_print_dbg("%s\n", __func__);

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    /* push curr wait event to wait queue */
    poll_wait(filp, &ps_core_d->bfgx_info[BFGX_GNSS].rx_wait, wait);

    ps_print_dbg("%s, recive gnss me data\n", __func__);

    if (ps_core_d->bfgx_info[BFGX_GNSS].rx_queue.qlen) { /* have data to read */
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

STATIC int64 hw_gnss_me_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    return 0;
}

STATIC ssize_t hw_gnss_me_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    struct sk_buff_head read_queue;
    int32 count1;
    uint8 seperate_tag = GNSS_SEPER_TAG_INIT;
    int32 copy_cnt = 0;
    uint32 ret;

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL))) {
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

STATIC ssize_t hw_gnss_me_write(struct file *filp, const int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = NULL;

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (ps_core_d->pm_data == NULL))) {
        ps_print_err("ps_core_d or buf is NULL\n");
        return -EINVAL;
    }

    if (atomic_read(&ps_core_d->bfgx_info[BFGX_GNSS].subsys_state) == POWER_STATE_SHUTDOWN) {
        ps_print_warning("gnss me has closed! It's Not necessary to send msg to device\n");
        return -EINVAL;
    }

    if (count > GNSS_TX_MAX_FRAME) {
        ps_print_err("err:gnss me packet is too large!\n");
        return -EINVAL;
    }

    /* if low queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_low_seq.qlen > TX_LOW_QUE_MAX_NUM) {
        return 0;
    }
    pm_data = ps_core_d->pm_data;
    atomic_set(&pm_data->gnss_sleep_flag, GNSS_NOT_AGREE_SLEEP);

    oal_wake_lock_timeout(&pm_data->gnss_wake_lock, DEFAULT_WAKELOCK_TIMEOUT);

    /* to divide up packet function and tx to tty work */
    if (ps_h2d_tx_data(ps_core_d, BFGX_GNSS, buf, count) < 0) {
        ps_print_err("hw_gnss_me_write is err\n");
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
        count = -EFAULT;
    }

    ps_core_d->bfgx_info[BFGX_GNSS].tx_pkt_num++;

    return count;
}

STATIC int32 hw_gnss_me_release(struct inode *inode, struct file *filp)
{
    int32 ret;
    struct pm_drv_data *pm_data = pm_get_drvdata(GUART);
    struct pm_top* pm_top_data = pm_get_top();

    mutex_lock(&(pm_top_data->host_mutex));

    ret = gnss_me_close();

    atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

STATIC int32 hw_meoam_open(struct inode *inode, struct file *filp)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    oal_atomic_inc(&g_me_oam_cnt);
    ps_print_info("%s g_me_oam_cnt=%d\n", __func__, oal_atomic_read(&g_me_oam_cnt));

    oal_atomic_set(&ps_core_d->dbg_func_has_open, 1);

    oal_atomic_set(&ps_core_d->dbg_read_delay, DBG_READ_DEFAULT_TIME);

    return 0;
}

STATIC ssize_t hw_meoam_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 count1 = 0;
    int64 timeout;

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (buf == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    if (ps_core_d->rx_dbg_seq.qlen == 0) {
        if (filp->f_flags & O_NONBLOCK) {
            return -EAGAIN;
        }

        timeout = wait_event_interruptible_timeout(ps_core_d->rx_dbg_wait,
                                                   (ps_core_d->rx_dbg_seq.qlen > 0),
                                                   msecs_to_jiffies(oal_atomic_read(&ps_core_d->dbg_read_delay)));
        if (!timeout) {
            ps_print_dbg("debug read time out!\n");
            return -ETIMEDOUT;
        }
    }

    if ((skb = ps_skb_dequeue(ps_core_d, RX_DBG_QUEUE)) == NULL) {
        ps_print_dbg("dbg read no data!\n");
        return -ETIMEDOUT;
    }

    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("debug copy_to_user is err!\n");
        ps_restore_skbqueue(ps_core_d, skb, RX_DBG_QUEUE);
        return -EFAULT;
    }

    skb_pull(skb, count1);

    if (skb->len == 0) {
        kfree_skb(skb);
    } else {
        ps_restore_skbqueue(ps_core_d, skb, RX_DBG_QUEUE);
    }

    return count1;
}

STATIC int32 hw_meoam_release(struct inode *inode, struct file *filp)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_print_info("%s", __func__);

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely((ps_core_d == NULL) || (inode == NULL) || (filp == NULL))) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    oal_atomic_dec(&g_me_oam_cnt);
    ps_print_info("%s g_me_oam_cnt=%d", __func__, oal_atomic_read(&g_me_oam_cnt));
    if (oal_atomic_read(&g_me_oam_cnt) == 0) {
        wake_up_interruptible(&ps_core_d->rx_dbg_wait);
        atomic_set(&ps_core_d->dbg_func_has_open, 0);
        ps_kfree_skb(ps_core_d, RX_DBG_QUEUE);
    }

    return 0;
}

STATIC ssize_t hw_meexcp_read(struct file *filp, int8 __user *buf, size_t count, loff_t *f_pos)
{
    struct sk_buff *skb = NULL;
    uint16 count1;
    memdump_info_t *memdump_t = &g_gcpu_memdump_cfg;

    if (unlikely(buf == NULL)) {
        ps_print_err("buf is NULL\n");
        return -EINVAL;
    }

    if ((skb = skb_dequeue(&memdump_t->quenue)) == NULL) {
        return 0;
    }

    count1 = min_t(size_t, skb->len, count);
    if (copy_to_user(buf, skb->data, count1)) {
        ps_print_err("copy_to_user is err!\n");
        skb_queue_head(&memdump_t->quenue, skb);
        return -EFAULT;
    }

    skb_pull(skb, count1);

    if (skb->len == 0) {
        kfree_skb(skb);
    } else {
        skb_queue_head(&memdump_t->quenue, skb);
    }

    return count1;
}

STATIC int64 hw_meexcp_ioctl(struct file *file, uint32 cmd, uint64 arg)
{
    int32 ret = 0;
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, GUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    switch (cmd) {
        case PLAT_ME_DUMP_FILE_READ_CMD:
            ret = plat_bfgx_dump_rotate_cmd_read(ps_core_d, arg);
            break;

        default:
            ps_print_warning("hw_meoam_ioctl cmd = %d not find\n", cmd);
            return -EINVAL;
    }

    return ret;
}

STATIC const struct file_operations g_hw_gnss_me_fops = {
    .owner = THIS_MODULE,
    .open = hw_gnss_me_open,
    .write = hw_gnss_me_write,
    .read = hw_gnss_me_read,
    .poll = hw_gnss_me_poll,
    .unlocked_ioctl = hw_gnss_me_ioctl,
    .release = hw_gnss_me_release,
};

STATIC const struct file_operations g_hw_meoam_fops = {
    .owner = THIS_MODULE,
    .open = hw_meoam_open,
    .read = hw_meoam_read,
    .release = hw_meoam_release,
};

STATIC const struct file_operations g_hw_meexcp_fops = {
    .owner = THIS_MODULE,
    .read = hw_meexcp_read,
    .unlocked_ioctl = hw_meexcp_ioctl,
};

#ifdef HAVE_HISI_GNSS
STATIC struct miscdevice g_hw_gnss_me_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwgnss_me",
    .fops = &g_hw_gnss_me_fops,
};
#endif

STATIC struct miscdevice g_hw_meoam_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwmeoam",
    .fops = &g_hw_meoam_fops,
};

STATIC struct miscdevice g_hw_meexcp_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hwmeexcp",
    .fops = &g_hw_meexcp_fops,
};

STATIC struct hw_ps_plat_data g_hisi_me_platform_data = {
    .dev_name = "/dev/ttyAMA5",
    .flow_cntrl = FLOW_CTRL_ENABLE,
    .baud_rate = DEFAULT_BAUD_RATE,
    .ldisc_num = N_HW_GNSS,
    .suspend = NULL,
    .resume = NULL,
};

#ifdef _PRE_CONFIG_USE_DTS
STATIC int32 ps_me_misc_dev_register(void)
{
#ifdef HAVE_HISI_GNSS
    int32 err;
#endif

    if (is_bfgx_support() != OAL_TRUE) {
        /* don't support bfgx */
        ps_print_info("bfgx disabled, misc dev register bypass\n ");
        return 0;
    }

#ifdef HAVE_HISI_GNSS
    err = misc_register(&g_hw_gnss_me_device);
    if (err != 0) {
        ps_print_err("Failed to register gnss inode\n ");
        return -EFAULT;
    }
#endif

    return 0;
}

STATIC void ps_me_misc_dev_unregister(void)
{
    if (is_bfgx_support() != OAL_TRUE) {
        ps_print_info("bfgx disabled, misc dev unregister bypass\n ");
        return;
    }

#ifdef HAVE_HISI_GNSS
    misc_deregister(&g_hw_gnss_me_device);
    ps_print_info("misc gnss me device have removed\n");
#endif
}

STATIC int32 ps_me_probe(struct platform_device *pdev)
{
    struct hw_ps_plat_data *pdata = NULL;
    struct ps_plat_s *ps_plat_d = NULL;
    int32 err;
    hi110x_board_info *bd_info = get_hi110x_board_info();

    strncpy_s(g_hisi_me_platform_data.dev_name, sizeof(g_hisi_me_platform_data.dev_name),
              bd_info->me_uart_port, sizeof(g_hisi_me_platform_data.dev_name) - 1);
    g_hisi_me_platform_data.dev_name[sizeof(g_hisi_me_platform_data.dev_name) - 1] = '\0';

    pdev->dev.platform_data = &g_hisi_me_platform_data;
    pdata = &g_hisi_me_platform_data;

    if (hi110x_is_asic() == VERSION_FPGA) {
        g_hisi_me_platform_data.baud_rate = UART_BAUD_RATE_2M;
    }

    g_hw_ps_me_device = pdev;

    ps_plat_d = kzalloc(sizeof(struct ps_plat_s), GFP_KERNEL);
    if (ps_plat_d == NULL) {
        ps_print_err("no mem to allocate\n");
        return -ENOMEM;
    }
    dev_set_drvdata(&pdev->dev, ps_plat_d);

    err = ps_core_init(&ps_plat_d->core_data, GUART);
    if (err != 0) {
        ps_print_err("me core init failed\n");
        goto err_core_init;
    }

    /* refer to itself */
    ps_plat_d->core_data->ps_plat = ps_plat_d;
    /* get reference of pdev */
    ps_plat_d->pm_pdev = pdev;

    /* copying platform data */
    if (strncpy_s(ps_plat_d->dev_name, sizeof(ps_plat_d->dev_name),
                  pdata->dev_name, HISI_UART_DEV_NAME_LEN - 1) != EOK) {
        ps_print_err("strncpy_s failed, please check!\n");
    }
    ps_plat_d->flow_cntrl = pdata->flow_cntrl;
    ps_plat_d->baud_rate = pdata->baud_rate;
    ps_plat_d->ldisc_num = pdata->ldisc_num;

    me_tty_recv = ps_core_recv;

    err = ps_me_misc_dev_register();
    if (err != 0) {
        goto err_misc_dev;
    }

    err = misc_register(&g_hw_meoam_device);
    if (err != 0) {
        ps_print_err("Failed to register hwmeoam inode\n");
        goto err_register_meoam;
    }

    err = misc_register(&g_hw_meexcp_device);
    if (err != 0) {
        ps_print_err("Failed to register hwmeexcp inode\n");
        goto err_register_meexcp;
    }

    ps_print_suc("%s is success!\n", __func__);

    return 0;

err_register_meexcp:
    misc_deregister(&g_hw_meoam_device);
err_register_meoam:
    ps_me_misc_dev_unregister();
err_misc_dev:
    ps_core_exit(ps_plat_d->core_data, GUART);
err_core_init:
    kfree(ps_plat_d);
    ps_plat_d = NULL;
    return -EFAULT;
}

STATIC int32 ps_me_remove(struct platform_device *pdev)
{
    struct ps_plat_s *ps_plat_d = NULL;

    misc_deregister(&g_hw_meexcp_device);
    misc_deregister(&g_hw_meoam_device);
    ps_me_misc_dev_unregister();

    ps_plat_d = dev_get_drvdata(&pdev->dev);
    if (ps_plat_d == NULL) {
        ps_print_err("ps_plat_d is null\n");
        return -EFAULT;
    }

    ps_core_exit(ps_plat_d->core_data, GUART);
    kfree(ps_plat_d);
    ps_plat_d = NULL;

    return 0;
}

STATIC int32 ps_me_suspend(struct platform_device *pdev, pm_message_t state)
{
    return 0;
}

STATIC int32 ps_me_resume(struct platform_device *pdev)
{
    return 0;
}

STATIC struct of_device_id g_hi110x_ps_me_match_table[] = {
    {
        .compatible = DTS_COMP_HI110X_PS_ME_NAME,
        .data = NULL,
    },
    {},
};
#endif

STATIC struct platform_driver g_ps_me_platform_driver = {
#ifdef _PRE_CONFIG_USE_DTS
    .probe = ps_me_probe,
    .remove = ps_me_remove,
    .suspend = ps_me_suspend,
    .resume = ps_me_resume,
#endif
    .driver = {
        .name = "hisi_me",
        .owner = THIS_MODULE,
#ifdef _PRE_CONFIG_USE_DTS
        .of_match_table = g_hi110x_ps_me_match_table,
#endif
    },
};

int32 hw_ps_me_init(void)
{
    int32 ret;

    ret = platform_driver_register(&g_ps_me_platform_driver);
    if (ret) {
        ps_print_err("Unable to register platform me driver.\n");
    }

    return ret;
}

void hw_ps_me_exit(void)
{
    platform_driver_unregister(&g_ps_me_platform_driver);
}

MODULE_DESCRIPTION("Public serial Driver for huawei GNSS ME chips");
MODULE_LICENSE("GPL");

