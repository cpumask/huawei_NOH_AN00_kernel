

#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
#define HISI_LOG_TAG "[SOFT_WDT]"
#include "oal_util.h"
#include "oal_net.h"

#include "oal_schedule.h"
#include "oal_workqueue.h"
#include "securec.h"

/* 4s timeout,8s bug_on */
#define OAL_SOFTWDT_DEFAULT_TIMEOUT 4000 /* 4s */

typedef struct _hisi_conn_softwdt_ {
    uint32_t kick_time;
    oal_delayed_work wdt_delayed_work;
    oal_workqueue_stru *wdt_wq;
    oal_timer_list_stru wdt_timer;
    uint32_t wdt_timeout_count;
} hisi_conn_softwdt;

OAL_STATIC hisi_conn_softwdt g_hisi_softwdt;

#ifdef PLATFORM_SSI_FULL_LOG
OAL_STATIC int32_t g_disable_wdt_flag = 0;
#else
OAL_STATIC int32_t g_disable_wdt_flag = 1;
#endif
oal_debug_module_param(g_disable_wdt_flag, int, S_IRUGO | S_IWUSR);

OAL_STATIC void oal_softwdt_kick(void)
{
    oal_timer_start(&g_hisi_softwdt.wdt_timer, OAL_SOFTWDT_DEFAULT_TIMEOUT);
}

OAL_STATIC void oal_softwdt_timeout(unsigned long data)
{
    oal_reference(data);
    g_hisi_softwdt.wdt_timeout_count++;

    if (g_hisi_softwdt.wdt_timeout_count == 1) {
        /* ��һ�γ�ʱ */
#ifdef CONFIG_PRINTK
        printk(KERN_WARNING "hisi softwdt timeout first time,keep try...\n");
#else
        oal_io_print("hisi softwdt timeout first time,keep try...\n");
#endif
        oal_softwdt_kick();
    }

    if (g_hisi_softwdt.wdt_timeout_count >= 2) {
        /* �ڶ��μ����ϴγ�ʱ */
#ifdef CONFIG_PRINTK
        printk(KERN_EMERG "[E]hisi softwdt timeout second time, dump system stack\n");
#else
        oal_io_print("hisi softwdt timeout second time, dump system stack!\n");
#endif
        declare_dft_trace_key_info("oal_softwdt_timeout", OAL_DFT_TRACE_EXCEP);
        oal_warn_on(1);
        g_hisi_softwdt.wdt_timeout_count = 0;
    }

    return;
}

OAL_STATIC void oal_softwdt_feed_task(oal_work_stru *pst_work)
{
    oal_softwdt_kick();
    oal_queue_delayed_work_on(0, g_hisi_softwdt.wdt_wq, &g_hisi_softwdt.wdt_delayed_work,
                              oal_msecs_to_jiffies(g_hisi_softwdt.kick_time));
    return;
}

int32_t oal_softwdt_init(void)
{
    if (g_disable_wdt_flag) {
        return OAL_SUCC;
    }

    memset_s((void *)&g_hisi_softwdt, OAL_SIZEOF(g_hisi_softwdt), 0, OAL_SIZEOF(g_hisi_softwdt));
    oal_timer_init(&g_hisi_softwdt.wdt_timer, OAL_SOFTWDT_DEFAULT_TIMEOUT, oal_softwdt_timeout, 0);
    g_hisi_softwdt.kick_time = OAL_SOFTWDT_DEFAULT_TIMEOUT / 2;  // 2s
    oal_init_delayed_work(&g_hisi_softwdt.wdt_delayed_work, oal_softwdt_feed_task);
    g_hisi_softwdt.wdt_wq = oal_create_workqueue("softwdt");
    if (oal_unlikely(g_hisi_softwdt.wdt_wq == NULL)) {
        oal_io_print("hisi soft wdt create workqueue failed!\n");
        return -OAL_ENOMEM;
    }

    /* start wdt */
    oal_queue_delayed_work_on(0, g_hisi_softwdt.wdt_wq, &g_hisi_softwdt.wdt_delayed_work, 0);
    return OAL_SUCC;
}
oal_module_symbol(oal_softwdt_init);

void oal_softwdt_exit(void)
{
    if (g_disable_wdt_flag) {
        return;
    }

    oal_timer_delete_sync(&g_hisi_softwdt.wdt_timer);
    oal_cancel_delayed_work_sync(&g_hisi_softwdt.wdt_delayed_work);
    oal_destroy_workqueue(g_hisi_softwdt.wdt_wq);
}
oal_module_symbol(oal_softwdt_exit);
#endif

