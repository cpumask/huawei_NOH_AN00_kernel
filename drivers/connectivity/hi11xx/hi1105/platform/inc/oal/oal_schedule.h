

#ifndef __OAL_SCHEDULE_H__
#define __OAL_SCHEDULE_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_hardware.h"
#include "oal_mm.h"
#include "arch/oal_schedule.h"
#include "oal_list.h"
#include "securec.h"
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
#include <linux/pm_wakeup.h>
#endif
/* �궨�� */
#if (_PRE_CHIP_BITS_MIPS32 == _PRE_CHIP_BITS)
/* 32λ�Ĵ�����󳤶� */
#define OAL_TIME_US_MAX_LEN 0xFFFFFFFF

#elif (_PRE_CHIP_BITS_MIPS64 == _PRE_CHIP_BITS)
/* 64λ�Ĵ�����󳤶� */
#define OAL_TIME_US_MAX_LEN 0xFFFFFFFFFFFFFFFF

#endif

#define oal_sys_wdt_start(mode, msec)
#define oal_sys_wdt_start_int(msec)
#define oal_sys_wdt_start_noint(msec)
#define oal_sys_wdt_stop()
#define oal_sys_wdt_feed()

#define oal_sys_bus_idle()

#if defined(PLATFORM_DEBUG_ENABLE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#define oal_debug_module_param        module_param
#define oal_debug_module_param_string module_param_string
#define oal_debug_module_parm_desc    MODULE_PARM_DESC
#else
#define oal_debug_module_param(_symbol, _type, _name)
#define oal_debug_module_param_string(arg1, arg2, arg3, arg4)
#define oal_debug_module_parm_desc(arg1, arg2)
#endif

typedef enum {
    OAL_DFT_TRACE_OTHER = 0x0,
    OAL_DFT_TRACE_SUCC,
    OAL_DFT_TRACE_FAIL,  /* > fail while dump the key_info to kernel */
    OAL_DFT_TRACE_EXCEP, /* exception */
    OAL_DFT_TRACE_BUTT,
} oal_dft_type;

#ifdef _PRE_OAL_FEATURE_KEY_PROCESS_TRACE
#include <linux/rtc.h>
typedef struct _oal_dft_trace_item_ {
    oal_list_entry_stru list;
    char *name;
    uint16_t dft_type;
    uint16_t trace_flag; /* add the the list */
    uint32_t trace_count;
    struct timeval first_timestamp; /* first keyinfo timestamp */
    struct timeval last_timestamp;  /* last keyinfo timestamp */
} oal_dft_trace_item;

extern oal_spin_lock_stru g_dft_head_lock;
extern oal_list_head_stru g_dft_head;
extern int32_t oal_dft_init(void);
extern void oal_dft_exit(void);
extern void oal_dft_print_error_key_info(void);
extern void oal_dft_print_all_key_info(void);

OAL_STATIC OAL_INLINE void oal_dft_trace_key_info_func(oal_dft_trace_item *pst_dft_item,
                                                           char *dname, uint16_t dtype)
{
    unsigned long flags__;
    oal_spin_lock_irq_save(&g_dft_head_lock, &flags__);
    if (!pst_dft_item->trace_flag) {
        oal_list_add(&pst_dft_item->list, &g_dft_head);
        pst_dft_item->name = dname;
        pst_dft_item->dft_type = dtype;
        pst_dft_item->trace_flag = 1;
    }
    pst_dft_item->trace_count++;
    oal_spin_unlock_irq_restore(&g_dft_head_lock, &flags__);
    if ((!pst_dft_item->first_timestamp.tv_sec) && (!pst_dft_item->first_timestamp.tv_usec)) {
        do_gettimeofday(&pst_dft_item->first_timestamp);
    } else {
        do_gettimeofday(&pst_dft_item->last_timestamp);
    }
    if (oal_unlikely(dtype >= OAL_DFT_TRACE_FAIL)) {
        printk(KERN_ERR "[E]key_info:%s happened[%u]!\n", dname, pst_dft_item->trace_count);
    }
}

/* �ؼ����̷���ʱ����¼���м������������� */
#define declare_dft_trace_key_info(dname, dtype)                                            \
    do {                                                                                    \
        OAL_STATIC oal_dft_trace_item st_dft_item = {{0}, NULL, 0, 0, 0, {0}, {0}}; \
        oal_dft_trace_key_info_func(&st_dft_item, dname, dtype);                            \
    } while (0)
#else
#define declare_dft_trace_key_info(dname, dtype)
#endif

extern int32_t g_chip_err_block;
OAL_STATIC OAL_INLINE void oal_chip_error_task_block(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    int32_t status = g_chip_err_block;
    if (status != 0) {
        printk("chip error block here\n");
    }

    while (g_chip_err_block == 1) { /* 1 means block */
        if (msleep_interruptible(100)) { /* 100 ms */
            printk("chip error block interrupt\n");
            break;
        }
    }

    printk("chip error block out\n");

    if (g_chip_err_block == 2) { /* 2 means step chip error */
        g_chip_err_block = status;
        printk("chip error step\n");
    }
#endif
}

OAL_STATIC OAL_INLINE int32_t oal_is_chip_error_block(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (g_chip_err_block == 1) { /* 1 means block */
        return OAL_TRUE;
    }
#endif
    return OAL_FALSE;
}

#define PRINT_RATE_SECOND 1000
#define PRINT_RATE_MINUTE (60 * 1000)
#define PRINT_RATE_HOUR   (60 * PRINT_RATE_MINUTE)
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC OAL_INLINE int oal_print_rate_limit_func(int *first_print, unsigned long *begin,
                                                    int *missed, uint32_t timeout, int32_t linenum)
{
    int ret__;
    if ((!!(*first_print)) && ((timeout != 0) && (!time_is_before_jiffies((*begin) + msecs_to_jiffies(timeout))))) {
        ret__ = 0;
        (*missed)++;
    } else {
        if (*missed) {
            printk(KERN_INFO "[%d]logs dropped line %d[timeout=%u]\n", *missed, linenum, timeout);
        }
        ret__ = 1;
        *missed = 0;
        *begin = jiffies;
        *first_print = 1;
    }

    return ret__;
}

/*
 * ΢�볬ʱ����timeout=1000 ˵������ӡƵ�ʸ���1000msʱ����true
 * ����ѹ�ƴ�ӡ����false, ��timeout=0 �಻ѹ�ƴ�ӡ
 */
#define oal_print_rate_limit(timeout) ({                                                 \
    int ret__;                                                                           \
    static int first_print = 0;                                                          \
    static unsigned long begin = 0;                                                          \
    static int missed = 0;                                                               \
    ret__ = oal_print_rate_limit_func(&first_print, &begin, &missed, timeout, __LINE__); \
    (ret__);                                                                             \
})

#else
OAL_STATIC OAL_INLINE int32_t oal_print_rate_limit(unsigned long timeout)
{
    return 1;
}
#endif

typedef struct _oal_wakelock_stru_ {
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    struct wakeup_source st_wakelock;  // wakelock��
    oal_spin_lock_stru lock;           // wakelock������spinlock��
    oal_dlist_head_stru list;
#endif
    unsigned long lock_count;  // ����wakelock���Ĵ���
    uintptr_t locked_addr; /* the locked address */
    uint32_t debug;      // debug flag
} oal_wakelock_stru;

/* ��ȡ��_ul_start��_ul_end��ʱ��� */
#define oal_time_get_runtime(_ul_start, _ul_end) \
    (((_ul_start) > (_ul_end)) ? (oal_time_calc_runtime((_ul_start), (_ul_end))) : ((_ul_end) - (_ul_start)))

/* ȫ�ֱ������� */
extern oal_spin_lock_stru g_wakelock_lock;
extern oal_dlist_head_stru g_wakelock_head;

/* �������� */
#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
extern int32_t oal_softwdt_init(void);
extern void oal_softwdt_exit(void);
#else
OAL_STATIC OAL_INLINE int32_t oal_softwdt_init(void)
{
    return OAL_SUCC;
}
OAL_STATIC OAL_INLINE void oal_softwdt_exit(void)
{
    return;
}
#endif

int32_t oal_print_all_wakelock_buff(char *buf, int32_t buf_len);
int32_t oal_set_wakelock_debuglevel(const char *name, uint32_t level);

OAL_STATIC OAL_INLINE void oal_wake_lock_init(oal_wakelock_stru *pst_wakelock, char *name)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    unsigned long flags;
    memset_s((void *)pst_wakelock, sizeof(oal_wakelock_stru), 0, sizeof(oal_wakelock_stru));

    wakeup_source_init(&pst_wakelock->st_wakelock, name ? name : "wake_lock_null");
    oal_spin_lock_init(&pst_wakelock->lock);
    pst_wakelock->lock_count = 0;
    pst_wakelock->locked_addr = 0;
    /* add to local list */
    oal_spin_lock_irq_save(&g_wakelock_lock, &flags);
    oal_dlist_add_tail(&pst_wakelock->list, &g_wakelock_head);
    oal_spin_unlock_irq_restore(&g_wakelock_lock, &flags);
#endif
}

OAL_STATIC OAL_INLINE void oal_wake_lock_exit(oal_wakelock_stru *pst_wakelock)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    unsigned long flags;
    oal_spin_lock_irq_save(&g_wakelock_lock, &flags);
    oal_dlist_delete_entry(&pst_wakelock->list);
    oal_spin_unlock_irq_restore(&g_wakelock_lock, &flags);
    wakeup_source_trash(&pst_wakelock->st_wakelock);
#endif
}

OAL_STATIC OAL_INLINE void oal_wake_lock(oal_wakelock_stru *pst_wakelock)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))

    unsigned long ul_flags;

    if (oal_unlikely(pst_wakelock == NULL)) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_wakelock->lock, &ul_flags);
    if (!pst_wakelock->lock_count) {
        __pm_stay_awake(&pst_wakelock->st_wakelock);
        pst_wakelock->locked_addr = (uintptr_t)_RET_IP_;
    }
    pst_wakelock->lock_count++;
    if (oal_unlikely(pst_wakelock->debug)) {
        printk(KERN_INFO "wakelock[%s] lockcnt:%lu <==%pf\n",
               pst_wakelock->st_wakelock.name, pst_wakelock->lock_count, (void *)_RET_IP_);
    }
    oal_spin_unlock_irq_restore(&pst_wakelock->lock, &ul_flags);
#endif
}

OAL_STATIC OAL_INLINE void oal_wake_unlock(oal_wakelock_stru *pst_wakelock)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))

    unsigned long ul_flags;

    if (oal_unlikely(pst_wakelock == NULL)) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_wakelock->lock, &ul_flags);
    if (pst_wakelock->lock_count) {
        pst_wakelock->lock_count--;
        if (!pst_wakelock->lock_count) {
            __pm_relax(&pst_wakelock->st_wakelock);
            pst_wakelock->locked_addr = (uintptr_t)0x0;
        }

        if (oal_unlikely(pst_wakelock->debug)) {
            printk(KERN_INFO "wakeunlock[%s] lockcnt:%lu <==%pf\n",
                   pst_wakelock->st_wakelock.name, pst_wakelock->lock_count, (void *)(uintptr_t)_RET_IP_);
        }
    }
    oal_spin_unlock_irq_restore(&pst_wakelock->lock, &ul_flags);
#endif
}

OAL_STATIC OAL_INLINE int32_t oal_wakelock_active(oal_wakelock_stru *pst_wakelock)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
    if (oal_unlikely(pst_wakelock == NULL)) {
        oal_warn_on(1);
        return 0;
    }
    return pst_wakelock->st_wakelock.active;
#else
    return 0;
#endif
}

OAL_STATIC OAL_INLINE void oal_wake_lock_timeout(oal_wakelock_stru *pst_wakelock, uint32_t msec)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))

    unsigned long ul_flags;

    oal_spin_lock_irq_save(&pst_wakelock->lock, &ul_flags);

    __pm_wakeup_event(&pst_wakelock->st_wakelock, msec);
    pst_wakelock->locked_addr = _RET_IP_;

    oal_spin_unlock_irq_restore(&pst_wakelock->lock, &ul_flags);
#endif
}

OAL_STATIC OAL_INLINE void oal_wake_unlock_force(oal_wakelock_stru *pst_wakelock)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))

    unsigned long ul_flags;

    oal_spin_lock_irq_save(&pst_wakelock->lock, &ul_flags);

    __pm_relax(&pst_wakelock->st_wakelock);
    pst_wakelock->locked_addr = (uintptr_t)0x0;
    pst_wakelock->lock_count = 0;

    oal_spin_unlock_irq_restore(&pst_wakelock->lock, &ul_flags);
#endif
}

#endif /* end of oal_schedule.h */
