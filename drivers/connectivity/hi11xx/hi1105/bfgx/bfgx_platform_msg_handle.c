

/* ͷ�ļ����� */
#include "bfgx_platform_msg_handle.h"

#include <linux/sched.h>

#include "bfgx_dev.h"
#include "plat_debug.h"
#include "plat_uart.h"
#include "bfgx_exception_rst.h"
#include "plat_pm.h"

#ifdef CONFIG_HI1102_PLAT_HW_CHR
#include "chr_user.h"
#endif

/* ȫ�ֱ������� */
typedef void (*platform_msg_handler)(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void platform_unused_msg(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_thread_msg_handle(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_agree_host_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_not_agree_host_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_read_mem_complete(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_set_uart_loop_complete(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void bfgx_recv_device_chr(struct ps_core_s *ps_core_d, uint8 *buf_ptr);
STATIC void gnss_trickle_request_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr);

STATIC platform_msg_handler g_sys_msg_handle[SYS_INF_BUTT] = {
    platform_unused_msg,         // SYS_INF_PF_INIT = 0x00,                /* ƽ̨�����ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_BT_INIT = 0x01,                /* BT�����ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_GNSS_INIT = 0x02,              /* GNSS�����ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_FM_INIT = 0x03,                /* FM�����ʼ����� */
    platform_unused_msg,         // SYS_INF_BT_DISABLE = 0x04,             /* BT���� */
    platform_unused_msg,         // SYS_INF_GNSS_DISABLE = 0x05,           /* GNSS���� */
    platform_unused_msg,         // SYS_INF_FM_DISABLE = 0x06,             /* FM���� */
    bfgx_thread_msg_handle,      // SYS_INF_BT_EXIT = 0x07,                /* BT�˳� */
    bfgx_thread_msg_handle,      // SYS_INF_GNSS_EXIT = 0x08,              /* GNSS�˳� */
    bfgx_thread_msg_handle,      // SYS_INF_FM_EXIT = 0x09,                /* FM�˳� */
    platform_unused_msg,         // SYS_INF_GNSS_WAIT_DOWNLOAD = 0x0A,     /* �ȴ�GNSS�������� */
    platform_unused_msg,         // SYS_INF_GNSS_DOWNLOAD_COMPLETE = 0x0B, /* GNSS����������� */
    platform_unused_msg,         // SYS_INF_BFG_HEART_BEAT = 0x0C,         /* �����ź� */
    bfgx_agree_host_sleep,       // SYS_INF_DEV_AGREE_HOST_SLP = 0x0D,     /* device�ظ�host��˯ */
    bfgx_not_agree_host_sleep,   // SYS_INF_DEV_NOAGREE_HOST_SLP = 0x0E,   /* device�ظ�host����˯ */
    platform_unused_msg,         // SYS_INF_WIFI_OPEN = 0x0F,              /* WCPU�ϵ���� */
    bfgx_thread_msg_handle,      // SYS_INF_IR_INIT = 0x10,                /* IR�����ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_IR_EXIT = 0x11,                /* IR�˳� */
    bfgx_thread_msg_handle,      // SYS_INF_NFC_INIT = 0x12,               /* NFC�����ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_NFC_EXIT = 0x13,               /* NFC�˳� */
    platform_unused_msg,         // SYS_INF_WIFI_CLOSE = 0x14,             /* WCPU�µ���� */
    platform_unused_msg,         // SYS_INF_RF_TEMP_NORMAL = 0x15,         /* RF�¶����� */
    platform_unused_msg,         // SYS_INF_RF_TEMP_OVERHEAT = 0x16,       /* RF�¶ȹ��� */
    bfgx_read_mem_complete,      // SYS_INF_MEM_DUMP_COMPLETE = 0x17,      /* bfgx�쳣ʱ��MEM DUMP����� */
    platform_unused_msg,         // SYS_INF_WIFI_MEM_DUMP_COMPLETE = 0X18, /* bfgx�쳣ʱ��MEM DUMP����� */
    platform_unused_msg,         // SYS_INF_UART_HALT_WCPU = 0x19,         /* uart halt wcpu ok */
    bfgx_set_uart_loop_complete, // SYS_INF_UART_LOOP_SET_DONE = 0x1a,     /* device ����uart����ok */
    bfgx_recv_device_chr,        // SYS_INF_CHR_ERRNO_REPORT = 0x1b,       /* device��host�ϱ�CHR�쳣�� */
    platform_unused_msg,         // SYS_INF_RSV0 = 0x1c,                   /* rsv */
    platform_unused_msg,         // SYS_INF_RSV1 = 0x1d,                   /* rsv */
    platform_unused_msg,         // SYS_INF_RSV2 = 0x1e,                   /* rsv */
    bfgx_thread_msg_handle,      // SYS_INF_GNSS_LPPE_INIT = 0x1F,         /* 1103 GNSS �������̳߳�ʼ����� */
    bfgx_thread_msg_handle,      // SYS_INF_GNSS_LPPE_EXIT = 0x20,         /* 1103 GNSS �������߳��˳� */
    gnss_trickle_request_sleep,  // SYS_INF_GNSS_TRICKLE_SLEEP = 0x21,     /* 1103 GNSS TRICKLE Sleep */
};

STATIC void platform_unused_msg(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    ps_print_warning("unused msg:%d, need to check\n", *buf_ptr);
}

STATIC void thread_completon_handle(struct ps_core_s *ps_core_d, uint8 flag, uint8 thread)
{
    if (thread >= BFGX_BUTT) {
        ps_print_err("thread index error:%d\n", thread);
        return;
    }

    if (flag == THREAD_FLAG_INIT) {
        ps_print_info("%s thread init success\n", ps_core_d->bfgx_info[thread].name);
        complete(&ps_core_d->bfgx_info[thread].wait_opened);
    } else if (flag == THREAD_FLAG_EXIT) {
        ps_print_info("%s thread exit success\n", ps_core_d->bfgx_info[thread].name);
        complete(&ps_core_d->bfgx_info[thread].wait_closed);
    } else {
        ps_print_err("bfgx thread handle flag error:%d, thread:%d", flag, thread);
    }
}

STATIC void gnss_thread_handle(struct ps_core_s *ps_core_d, uint8 flag, uint8 info)
{
    switch (info) {
        case SYS_INF_GNSS_INIT:
            ps_print_info("GNSS me thread has opened\n");
            g_gnss_me_thread_status = DEV_THREAD_INIT;
            break;
        case SYS_INF_GNSS_LPPE_INIT:
            ps_print_info("GNSS lppe thread has opened\n");
            g_gnss_lppe_thread_status = DEV_THREAD_INIT;
            break;
        case SYS_INF_GNSS_EXIT:
            ps_print_info("GNSS me thread has exit\n");
            g_gnss_me_thread_status = DEV_THREAD_EXIT;
            break;
        case SYS_INF_GNSS_LPPE_EXIT:
            ps_print_info("GNSS lppe thread has exit\n");
            g_gnss_lppe_thread_status = DEV_THREAD_EXIT;
            break;
        default:
            ps_print_warning("GNSS thread info is error %d\n", info);
            break;
    }

    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        thread_completon_handle(ps_core_d, flag, BFGX_GNSS);
    } else {
        if (g_gnss_me_thread_status == g_gnss_lppe_thread_status) {
            thread_completon_handle(ps_core_d, flag, BFGX_GNSS);
        }
    }
}

STATIC void bfgx_thread_msg_handle(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    uint8 msg = *buf_ptr;

    switch (msg) {
        case SYS_INF_BT_INIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_INIT, BFGX_BT);
            break;
        case SYS_INF_FM_INIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_INIT, BFGX_FM);
            break;
        case SYS_INF_IR_INIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_INIT, BFGX_IR);
            break;
#ifdef HAVE_HISI_NFC
        case SYS_INF_NFC_INIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_INIT, BFGX_NFC);
            break;
        case SYS_INF_NFC_EXIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_EXIT, BFGX_NFC);
            break;
#else
        case SYS_INF_FAKE_CLOSE_BT:
            ps_print_info("SYS_INF_FAKE_CLOSE_BT received\n");
            break;
#endif
        case SYS_INF_GNSS_INIT:
        case SYS_INF_GNSS_LPPE_INIT:
            gnss_thread_handle(ps_core_d, THREAD_FLAG_INIT, msg);
            break;
        case SYS_INF_BT_EXIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_EXIT, BFGX_BT);
            break;
        case SYS_INF_FM_EXIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_EXIT, BFGX_FM);
            break;
        case SYS_INF_IR_EXIT:
            thread_completon_handle(ps_core_d, THREAD_FLAG_EXIT, BFGX_IR);
            break;
        case SYS_INF_GNSS_EXIT:
        case SYS_INF_GNSS_LPPE_EXIT:
            gnss_thread_handle(ps_core_d, THREAD_FLAG_EXIT, msg);
            break;
        default:
            ps_print_err("thread msg error:%d\n", msg);
            break;
    };
}

STATIC void bfgx_not_agree_host_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    struct pm_drv_data *pm_data = NULL;

    pm_data = ps_core_d->pm_data;
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return;
    }

    ps_print_info("bfgx dev not agree host sleep\n");

    del_timer_sync(&pm_data->dev_ack_timer);
    pm_data->bfgx_dev_state_set(pm_data, BFGX_ACTIVE);
    pm_data->bfgx_uart_state_set(pm_data, UART_READY);

    if ((!bfgx_other_subsys_all_shutdown(pm_data, BFGX_GNSS)) ||
        (atomic_read(&pm_data->gnss_sleep_flag) == GNSS_AGREE_SLEEP)) {
        mod_timer(&pm_data->bfg_timer, jiffies + msecs_to_jiffies(BT_SLEEP_TIME));
        pm_data->bfg_timer_mod_cnt++;
    }

    complete(&pm_data->dev_slp_comp);
}

STATIC void bfgx_agree_host_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    uint64 flags;
    struct pm_drv_data *pm_data;

    pm_data = ps_core_d->pm_data;
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return;
    }

    ps_print_info("bfgx dev agree host sleep\n");

    if (timer_pending(&pm_data->dev_ack_timer)) {
        ps_print_dbg("dev ack to agree to sleep in normal case\n");
        del_timer_sync(&pm_data->dev_ack_timer);
        complete(&pm_data->dev_slp_comp);

        if (unlikely(pm_data->rcvdata_bef_devack_flag == 1)) {
            ps_print_info("device send data to host before dev rcv allow slp msg\n");
            pm_data->rcvdata_bef_devack_flag = 0;
        } else if (atomic_read(&ps_core_d->node_visit_flag) > 0) {
            ps_print_info("someone visit dev node during waiting for dev_ack\n");
        } else {
            spin_lock_irqsave(&pm_data->wakelock_protect_spinlock, flags);
            if (pm_data->bfgx_dev_state == BFGX_ACTIVE) {
                ps_print_info("dev wkup isr occur during waiting for dev_ack\n");
            } else {
                pm_data->operate_beat_timer(BEAT_TIMER_DELETE);
                pm_data->bfg_wake_unlock(pm_data);
            }
            spin_unlock_irqrestore(&pm_data->wakelock_protect_spinlock, flags);
        }
    } else {
        ps_print_info("dev ack to agree to sleep after devack timer expired, gpio state:%d\n",
                      oal_gpio_get_value(pm_data->wakeup_host_gpio));
        spin_lock_irqsave(&pm_data->uart_state_spinlock, flags);
        pm_data->bfgx_uart_state_set(pm_data, UART_NOT_READY);
        pm_data->bfgx_dev_state_set(pm_data, BFGX_SLEEP);
        spin_unlock_irqrestore(&pm_data->uart_state_spinlock, flags);
    }
}

STATIC void bfgx_read_mem_complete(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    struct st_exception_info *pst_exception_data = NULL;

    bfgx_memdump_finish(ps_core_d);

    get_exception_info_reference(&pst_exception_data);
    if (pst_exception_data != NULL) {
        ps_print_info("plat bfgx mem dump complete\n");

        atomic_set(&pst_exception_data->is_memdump_runing, 0);
        complete_all(&pst_exception_data->wait_read_bfgx_stack);
    }
}

STATIC void bfgx_set_uart_loop_complete(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    if (g_uart_loop_test_info != NULL) {
        ps_print_info("bfgx uart loop test set ok\n");
        complete(&g_uart_loop_test_info->set_done);
    }
}

STATIC void bfgx_recv_device_chr(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
#ifdef CONFIG_HI1102_PLAT_HW_CHR
    chr_errno_with_arg_stru *pst_chr = (chr_errno_with_arg_stru *)(buf_ptr + 1);
#endif

    ps_print_info("rcv devcie chr report\n");

#ifdef CONFIG_HI1102_PLAT_HW_CHR
    if (pst_chr->errlen > CHR_ERR_DATA_MAX_LEN) {
        ps_print_err("bfgx recv chr wrong, errno:0x%x, errlen:%d\n", pst_chr->errno, pst_chr->errlen);
        return;
    }

    chr_dev_exception_callback((uint8 *)pst_chr, (pst_chr->errlen + OAL_SIZEOF(chr_errno_with_arg_stru)));
#else
    ps_print_info("host chr not support, ignore errno:0x%x\n", *(uint32 *)buf_ptr);
#endif
}

STATIC void gnss_trickle_request_sleep(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    struct pm_drv_data *pm_data;

    pm_data = ps_core_d->pm_data;
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return;
    }

    ps_print_info("gnss trickle sleep!\n");

    if (!timer_pending(&pm_data->bfg_timer)) {
        ps_print_suc("gnss low power request sleep!\n");

        if (queue_work(pm_data->wkup_dev_workqueue, &pm_data->send_allow_sleep_work) != true) {
            ps_print_info("queue_work send_allow_sleep_work not return true\n");
        }
    }

    /* set the flag to 1 means gnss request sleep */
    atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
}

STATIC void heartbeat_debug_print(struct ps_core_s *ps_core_d, uint8 syschar)
{
    struct tty_struct *tty = NULL;
    struct uart_state *state = NULL;
    struct pm_drv_data *pm_data = NULL;
    struct serial_icounter_struct uart_icount;

    // ���ð汾����ӡ
    if (hi110x_get_release_type() == HI110X_VTYPE_RELEASE) {
        return;
    }

    tty = ps_core_d->tty;
    state = tty->driver_data;
    pm_data = ps_core_d->pm_data;
#ifdef _PRE_PRODUCT_HI3751V811
    if (state->uart_port == NULL) {
        return;
    }
#endif
    if (tty->ops->get_icount(tty, &uart_icount) < 0) {
        memset_s(&uart_icount, sizeof(struct serial_icounter_struct),
                 0, sizeof(struct serial_icounter_struct));
    }

    ps_print_info("%ds,tty_tx=%d,tty_rx=%d,uart_tx=%d,uart_rx=%d,cts=%d, stopped=%d, hw_stopped=%d, chars_in_tty=%d\n",
                  syschar - SYS_INF_HEARTBEAT_CMD_BASE - 1,
                  oal_atomic_read(&(ps_core_d->tty_tx_cnt)),
                  oal_atomic_read(&(ps_core_d->tty_rx_cnt)),
                  uart_icount.tx,
                  uart_icount.rx,
                  uart_icount.cts,
                  tty->stopped,
                  state->uart_port->hw_stopped,
                  tty_chars_in_buffer(tty));
    ps_print_info("[%s]pkt_rx:BT=%lu,GNSS=%lu,FM=%lu,IR=%lu,SYS_MSG=%lu,OML=%lu,timer=%d,mod_cnt=%d,sleep_flag=%d\n",
                  index2name(pm_data->index),
                  ps_core_d->bfgx_info[BFGX_BT].rx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_GNSS].rx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_FM].rx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_IR].rx_pkt_num,
                  ps_core_d->rx_pkt_sys,
                  ps_core_d->rx_pkt_oml,
                  timer_pending(&pm_data->bfg_timer),
                  pm_data->bfg_timer_mod_cnt,
                  atomic_read(&pm_data->gnss_sleep_flag));
    ps_print_info("[%s]pkt_tx:BT=%lu, GNSS=%lu, FM=%lu, IR=%lu\n",
                  index2name(pm_data->index),
                  ps_core_d->bfgx_info[BFGX_BT].tx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_GNSS].tx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_FM].tx_pkt_num,
                  ps_core_d->bfgx_info[BFGX_IR].tx_pkt_num);
}

STATIC void bfgx_recv_heartbeat(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    struct st_exception_info *pst_exception_data = NULL;
    uint8 syschar;

    if (get_exception_info_reference(&pst_exception_data) == EXCEPTION_SUCCESS) {
        if (pst_exception_data->debug_beat_flag == 1) {
            atomic_set(&pst_exception_data->bfgx_beat_flag, BFGX_RECV_BEAT_INFO);
        }
    }

    syschar = *buf_ptr;
    heartbeat_debug_print(ps_core_d, syschar);
}

/*
 * Prototype    : ps_exe_sys_func
 * Description  : called by core when recive sys data event from device
 *              : decode rx sys packet function
 */
int32 ps_exe_sys_func(struct ps_core_s *ps_core_d, uint8 *buf_ptr)
{
    uint8 syschar;

    if (buf_ptr == NULL) {
        ps_print_err("buf_ptr is NULL\n");
        return -EINVAL;
    }

    ps_core_d->rx_pkt_sys++;
    syschar = *buf_ptr;

    if (syschar < SYS_INF_BUTT) {
        if (g_sys_msg_handle[syschar] != NULL) {
            g_sys_msg_handle[syschar](ps_core_d, buf_ptr);
        }
    } else if (syschar >= SYS_INF_HEARTBEAT_CMD_BASE) {
        bfgx_recv_heartbeat(ps_core_d, buf_ptr);
    } else {
        ps_print_warning("bfgx recv unsupport msg:0x%x\n", syschar);
    }

    return 0;
}
