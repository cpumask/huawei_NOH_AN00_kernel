

#ifndef __OAL_LINUX_SCHEDULE_H__
#define __OAL_LINUX_SCHEDULE_H__

/* ����ͷ�ļ����� */
/*lint -e322*/
#include <asm/atomic.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <asm/param.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))
#include <linux/stacktrace.h>
#endif
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION))
#include <linux/pm_wakeup.h>
#endif
#include "linux/time.h"
#include "linux/timex.h"
#include "linux/rtc.h"
#include "securec.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#include <uapi/linux/sched/types.h>
#endif

/*lint +e322*/
typedef atomic_t oal_atomic;

#define OAL_SPIN_LOCK_MAGIC_TAG 0xdead4ead
typedef struct _oal_spin_lock_stru_ {
#ifdef CONFIG_SPIN_LOCK_MAGIC_DEBUG
    uint32_t magic;
    uint32_t reserved;
#endif
    spinlock_t lock;
} oal_spin_lock_stru;

#ifdef CONFIG_SPIN_LOCK_MAGIC_DEBUG
#define oal_define_spinlock(x) oal_spin_lock_stru x = { \
    .magic = OAL_SPIN_LOCK_MAGIC_TAG,                   \
    .lock = __SPIN_LOCK_UNLOCKED(x)                     \
}
#else
#define oal_define_spinlock(x) oal_spin_lock_stru x = { \
    .lock = __SPIN_LOCK_UNLOCKED(x)                     \
}
#endif

/* ����ָ�룬����ָ����Ҫ�����������ĵĺ��� */
typedef uint32_t (*oal_irqlocked_func)(void *);

typedef rwlock_t oal_rwlock_stru;
typedef struct timer_list oal_timer_list_stru;

typedef struct tasklet_struct oal_tasklet_stru;
typedef void (*oal_defer_func)(oal_uint);

/* tasklet���� */
#define OAL_DECLARE_TASK DECLARE_TASKLET

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
typedef wait_queue_entry_t oal_wait_queue_stru;
#else
typedef wait_queue_t oal_wait_queue_stru;
#endif
typedef wait_queue_head_t oal_wait_queue_head_stru;

/*
 * wait_event_interruptible_timeout - sleep until a condition gets true or a timeout elapses
 * @_wq: the waitqueue to wait on
 * @_condition: a C expression for the event to wait for
 * @_timeout: timeout, in jiffies
 * The process is put to sleep (TASK_INTERRUPTIBLE) until the
 * @condition evaluates to true or a signal is received.
 * The @condition is checked each time the waitqueue @wq is woken up.
 * wake_up() has to be called after changing any variable that could
 * change the result of the wait condition.
 * The function returns 0 if the @timeout elapsed, -ERESTARTSYS if it
 * was interrupted by a signal, and the remaining jiffies otherwise
 * if the condition evaluated to true before the timeout elapsed.
 */
#define oal_wait_event_interruptible_timeout_m(_st_wq, _condition, _timeout) \
    wait_event_interruptible_timeout(_st_wq, _condition, _timeout)

#define oal_wait_event_timeout_m(_st_wq, _condition, _timeout) \
    wait_event_timeout(_st_wq, _condition, _timeout)

#define oal_wait_event_interruptible_m(_st_wq, _condition) \
    wait_event_interruptible(_st_wq, _condition)

#define oal_wait_queue_wake_up_interrupt(_pst_wq) wake_up_interruptible(_pst_wq)

#define oal_wait_queue_wake_up(_pst_wq) wake_up(_pst_wq)

#define oal_interruptible_sleep_on(_pst_wq) interruptible_sleep_on(_pst_wq)

#define oal_wait_queue_init_head(_pst_wq) init_waitqueue_head(_pst_wq)

/* ��ȡ���뼶ʱ��� */
#define oal_time_get_stamp_ms() jiffies_to_msecs(jiffies)

/* ��ȡ�߾��Ⱥ���ʱ���,����1ms */
#define oal_time_get_high_precision_ms() oal_get_time_stamp_from_timeval()

#define oal_enable_cycle_count()
#define oal_disable_cycle_count()
#define oal_get_cycle_count() 0

/* �Ĵ�����תģ������ʱ����� */
#define oal_time_calc_runtime(_ul_start, _ul_end)                              \
    (uint32_t)((oal_div_u64((uint64_t)(OAL_TIME_US_MAX_LEN), HZ) * 1000) + \
                 (((OAL_TIME_US_MAX_LEN) % HZ) * (1000 / HZ)) - (_ul_start) + (_ul_end))

#define OAL_TIME_JIFFY jiffies

#define OAL_TIME_HZ HZ

#define oal_msecs_to_jiffies(_msecs) msecs_to_jiffies(_msecs)

#define oal_jiffies_to_msecs(_jiffies) jiffies_to_msecs(_jiffies)

#define oal_get_real_time_m(_pst_tm) oal_get_real_time(_pst_tm)

#define oal_init_completion(_my_completion) init_completion(_my_completion)

#define oal_complete(_my_completion) complete(_my_completion)

#define oal_in_interrupt() in_interrupt()

#define oal_in_atomic() in_atomic()
typedef void (*oal_timer_func)(oal_uint);

typedef uint32_t (*oal_module_func_t)(void);

#define oal_module_license(_license_name) MODULE_LICENSE(_license_name)

#define OAL_MODULE_PARAM        module_param
#define OAL_MODULE_PARAM_STRING module_param_string

#define OAL_S_IRUGO S_IRUGO

#ifdef _PRE_WLAN_AIO
#define oal_module_init(_module_name)
#define oal_module_exit(_module_name)
#define oal_module_symbol(_symbol)
#else
#define oal_module_init(_module_name) module_init(_module_name)
#define oal_module_exit(_module_name) module_exit(_module_name)
#define oal_module_symbol(_symbol)    EXPORT_SYMBOL(_symbol)
#endif
#define oal_module_device_table(_type, _name) MODULE_DEVICE_TABLE(_type, _name)

#define oal_smp_call_function_single(core, task, info, wait) smp_call_function_single(core, task, info, wait)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
#define oal_is_err_or_null(ptr) (!(ptr) || IS_ERR(ptr))
#else
static inline bool __must_check oal_is_err_or_null(__force const void *ptr)
{
    return !ptr || IS_ERR(ptr);
}
#endif

/* STRUCT���� */
typedef struct proc_dir_entry oal_proc_dir_entry_stru;

typedef struct mutex oal_mutex_stru;

typedef struct completion oal_completion;

typedef struct {
    signed long i_sec;
    signed long i_usec;
} oal_time_us_stru;

typedef ktime_t oal_time_t_stru;

typedef struct _oal_task_lock_stru_ {
    oal_wait_queue_head_stru wq;
    struct task_struct *claimer; /* task that has host claimed */
    oal_spin_lock_stru lock;     /* lock for claim and bus ops */
    uintptr_t claim_addr;
    uint32_t claimed;
    int32_t claim_cnt;
} oal_task_lock_stru;

// ���ں�struct rtc_time ����һ��
typedef struct _oal_time_stru {
    int32_t tm_sec;   /* seconds */
    int32_t tm_min;   /* minutes */
    int32_t tm_hour;  /* hours */
    int32_t tm_mday;  /* day of the month */
    int32_t tm_mon;   /* month */
    int32_t tm_year;  /* year */
    int32_t tm_wday;  /* day of the week */
    int32_t tm_yday;  /* day in the year */
    int32_t tm_isdst; /* daylight saving time */
} oal_time_stru;

/*
 * �� �� ��  : oal_spin_lock_init
 * ��������  : ��������ʼ����������������Ϊ1��δ��״̬����
 * �������  : pst_lock: ���ĵ�ַ
 */
OAL_STATIC OAL_INLINE void oal_spin_lock_init(oal_spin_lock_stru *pst_lock)
{
    spin_lock_init(&pst_lock->lock);
#ifdef CONFIG_SPIN_LOCK_MAGIC_DEBUG
    pst_lock->magic = OAL_SPIN_LOCK_MAGIC_TAG;
#endif
}

OAL_STATIC OAL_INLINE void oal_spin_lock_magic_bug(oal_spin_lock_stru *pst_lock)
{
#ifdef CONFIG_SPIN_LOCK_MAGIC_DEBUG
    if (oal_unlikely(pst_lock->magic != (uint32_t)OAL_SPIN_LOCK_MAGIC_TAG)) {
#ifdef CONFIG_PRINTK
        const int l_dump_len = 32;
        /* spinlock never init or memory overwrite? */
        printk(KERN_EMERG "[E]SPIN_LOCK_BUG: spinlock:%p on CPU#%d, %s,magic:%08x should be %08x\n", pst_lock,
               raw_smp_processor_id(), current->comm, pst_lock->magic, OAL_SPIN_LOCK_MAGIC_TAG);
        /* �ں˺����̶�����, rowsize = 16,groupsize = 1 */
        print_hex_dump(KERN_EMERG, "spinlock_magic: ", DUMP_PREFIX_ADDRESS, 16, 1,
                       (uint8_t *)((uintptr_t)pst_lock - l_dump_len),
                       l_dump_len + sizeof(oal_spin_lock_stru) + l_dump_len, true); /* ��Ϊ��ǰ��32�ֽ����ݶ�dump���� */
        printk(KERN_EMERG "\n");
#endif
        oal_warn_on(1);
    }
#endif
}


OAL_STATIC OAL_INLINE void oal_spin_lock(oal_spin_lock_stru *pst_lock)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_lock(&pst_lock->lock);
}

/*
 * �� �� ��  : oal_spin_unlock
 * ��������  : Spinlock���ں��̵߳Ⱥ���̬�����Ļ����µĽ���������
 * �������  : pst_lock:��������ַ
 */
OAL_STATIC OAL_INLINE void oal_spin_unlock(oal_spin_lock_stru *pst_lock)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_unlock(&pst_lock->lock);
}


OAL_STATIC OAL_INLINE void oal_spin_lock_bh(oal_spin_lock_stru *pst_lock)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_lock_bh(&pst_lock->lock);
}

/*
 * �� �� ��  : oal_spin_unlock_bh
 * ��������  : Spinlock�����ж��Լ��ں��̵߳Ⱥ���̬�����Ļ����µĽ���������
 */
OAL_STATIC OAL_INLINE void oal_spin_unlock_bh(oal_spin_lock_stru *pst_lock)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_unlock_bh(&pst_lock->lock);
}

/*
 * �� �� ��  : oal_spin_lock_irq_save
 * ��������  : �����������ͬʱ��ñ����־�Ĵ�����ֵ������ʧЧ�����жϡ�
 * �������  : pst_lock:��������ַ
 *             pui_flags:��־�Ĵ���
 */
OAL_STATIC OAL_INLINE void oal_spin_lock_irq_save(oal_spin_lock_stru *pst_lock, oal_uint *pui_flags)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_lock_irqsave(&pst_lock->lock, *pui_flags);
}

/*
 * �� �� ��  : oal_spin_unlock_irq_restore
 * ��������  : �ͷ���������ͬʱ���ָ���־�Ĵ�����ֵ���ָ������жϡ�����oal_sp-
 *             in_lock_irq���ʹ��
 * �������  : pst_lock:��������ַ
 *             pui_flags:��־�Ĵ���
 */
OAL_STATIC OAL_INLINE void oal_spin_unlock_irq_restore(oal_spin_lock_stru *pst_lock, oal_uint *pui_flags)
{
    oal_spin_lock_magic_bug(pst_lock);
    spin_unlock_irqrestore(&pst_lock->lock, *pui_flags);
}

/*
 * �� �� ��  : oal_spin_lock_irq_exec
 * ��������  : ��ȡ���������ر��жϣ�ִ��ĳ������������֮���ٴ��жϣ��ͷ���������
 * �������  : pst_lock:��������ַ
 *             func������ָ���ַ
 *             p_arg����������
 *             pui_flags: �жϱ�־�Ĵ���
 */
OAL_STATIC OAL_INLINE uint32_t oal_spin_lock_irq_exec(oal_spin_lock_stru *pst_lock, oal_irqlocked_func func,
                                                      void *p_arg, oal_uint *pui_flags)
{
    uint32_t ul_rslt;

    spin_lock_irqsave(&pst_lock->lock, *pui_flags);

    ul_rslt = func(p_arg);

    spin_unlock_irqrestore(&pst_lock->lock, *pui_flags);

    return ul_rslt;
}

/*
 * �� �� ��  : oal_rw_lock_init
 * ��������  : ��д����ʼ�����Ѷ�д������Ϊ1��δ��״̬����
 * �������  : pst_lock: ��д���ṹ���ַ
 */
OAL_STATIC OAL_INLINE void oal_rw_lock_init(oal_rwlock_stru *pst_lock)
{
    rwlock_init(pst_lock);
}

/*
 * �� �� ��  : oal_rw_lock_read_lock
 * ��������  : ���ָ���Ķ���
 * �������  : pst_lock: ��д���ṹ���ַ
 */
OAL_STATIC OAL_INLINE void oal_rw_lock_read_lock(oal_rwlock_stru *pst_lock)
{
    read_lock(pst_lock);
}

/*
 * �� �� ��  : oal_rw_lock_read_unlock
 * ��������  : �ͷ�ָ���Ķ���
 * �������  : pst_lock: ��д���ṹ���ַ
 */
OAL_STATIC OAL_INLINE void oal_rw_lock_read_unlock(oal_rwlock_stru *pst_lock)
{
    read_unlock(pst_lock);
}

/*
 * �� �� ��  : oal_rw_lock_write_lock
 * ��������  : ���ָ����д��
 * �������  : pst_lock: ��д���ṹ���ַ
 */
OAL_STATIC OAL_INLINE void oal_rw_lock_write_lock(oal_rwlock_stru *pst_lock)
{
    write_lock(pst_lock);
}

/*
 * �� �� ��  : oal_rw_lock_write_unlock
 * ��������  : �ͷ�ָ����д��
 * �������  : pst_lock: ��д���ṹ���ַ
 */
OAL_STATIC OAL_INLINE void oal_rw_lock_write_unlock(oal_rwlock_stru *pst_lock)
{
    write_unlock(pst_lock);
}

/*
 * �� �� ��  : oal_task_init
 * ��������  : �����ʼ������ʼ����ɺ������ڹ���״̬��
 * �������  : pst_task: ����ṹ��ָ��
 *             p_func: ����������ڵ�ַ
 *             ui_args: ����д���ĺ�������ε�ַ
 */
OAL_STATIC OAL_INLINE void oal_task_init(oal_tasklet_stru *pst_task, oal_defer_func p_func, oal_uint ui_args)
{
    tasklet_init(pst_task, p_func, ui_args);
}

OAL_STATIC OAL_INLINE void oal_task_kill(oal_tasklet_stru *pst_task)
{
    return tasklet_kill(pst_task);
}

/*
 * �� �� ��  : oal_task_sched
 * ��������  : ������ȣ���������׼������״̬��������ִ������ֻص�����״̬��
 */
OAL_STATIC OAL_INLINE void oal_task_sched(oal_tasklet_stru *pst_task)
{
    tasklet_schedule(pst_task);
}

/*
 * �� �� ��  : oal_task_is_scheduled
 * ��������  : ���tasklet�Ƿ�ȴ�ִ��
 */
OAL_STATIC OAL_INLINE oal_uint oal_task_is_scheduled(oal_tasklet_stru *pst_task)
{
    return test_bit(TASKLET_STATE_SCHED, (unsigned long const volatile *)&pst_task->state);
}

/*
 * �� �� ��  : oal_atomic_read
 * ��������  : ��ȡԭ�ӱ�����ֵ
 * �������  : *p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 */
OAL_STATIC OAL_INLINE int32_t oal_atomic_read(oal_atomic *p_vector)
{
    return atomic_read(p_vector);
}

/*
 * �� �� ��  : oal_atomic_set
 * ��������  : ԭ�ӵ�����ԭ�ӱ���p_vectorֵΪul_val
 * �������  : p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 *             l_val  : ��Ҫ�����óɵ�ֵ
 */
OAL_STATIC OAL_INLINE void oal_atomic_set(oal_atomic *p_vector, int32_t l_val)
{
    atomic_set(p_vector, l_val);
}

/*
 * �� �� ��  : oal_atomic_dec
 * ��������  : ԭ�ӵĸ���μ�1��
 * �������  : *p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 */
OAL_STATIC OAL_INLINE void oal_atomic_dec(oal_atomic *p_vector)
{
    atomic_dec(p_vector);
}

/*
 * �� �� ��  : oal_atomic_inc
 * ��������  : ԭ�ӵĸ���μ�һ
 * �������  : *p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 */
OAL_STATIC OAL_INLINE void oal_atomic_inc(oal_atomic *p_vector)
{
    atomic_inc(p_vector);
}

/*
 * �� �� ��  : oal_atomic_add
 * ��������  : ԭ�ӵ�����ԭ�ӱ���p_vectorֵ����ul_val
 * �������  : p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 *             l_val  : ��Ҫ���ϵ�ֵ
 */
OAL_STATIC OAL_INLINE void oal_atomic_add(oal_atomic *p_vector, int32_t l_val)
{
    atomic_add(l_val, p_vector);
}


/*
 * �� �� ��  : oal_atomic_inc_and_test
 * ��������  : ԭ�ӵ����������Ƿ�Ϊ0
 * �������  : *p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 */
OAL_STATIC OAL_INLINE int32_t oal_atomic_inc_and_test(oal_atomic *p_vector)
{
    return atomic_inc_and_test(p_vector);
}

/*
 * �� �� ��  : oal_atomic_dec_and_test
 * ��������  : ԭ�ӵݼ��������Ƿ�Ϊ0
 * �������  : *p_vector: ��Ҫ����ԭ�Ӳ�����ԭ�ӱ�����ַ
 */
OAL_STATIC OAL_INLINE int32_t oal_atomic_dec_and_test(oal_atomic *p_vector)
{
    return atomic_dec_and_test(p_vector);
}

/*
 * �� �� ��  : oal_time_get_stamp_us
 * ��������  : ��ȡ΢��ȼ���ʱ���
 * �������  : pst_usec: ʱ��ṹ��ָ��
 */
OAL_STATIC OAL_INLINE void oal_time_get_stamp_us(oal_time_us_stru *pst_usec)
{
    struct timespec ts = {0};

    getnstimeofday(&ts);

    pst_usec->i_sec = ts.tv_sec;

    pst_usec->i_usec = ts.tv_nsec / 1000; /* ��λת����1us=1000ns */
}

/*
 * �� �� ��  : oal_ktime_get
 * ��������  : �����ں˺�����ȡ��ǰʱ���
 */
OAL_STATIC OAL_INLINE oal_time_t_stru oal_ktime_get(void)
{
    return ktime_get();
}

/*
 * �� �� ��  : oal_ktime_sub
 * ��������  : �����ں˺�����ȡʱ���ֵ
 */
OAL_STATIC OAL_INLINE oal_time_t_stru oal_ktime_sub(const oal_time_t_stru lhs, const oal_time_t_stru rhs)
{
    return ktime_sub(lhs, rhs);
}

OAL_STATIC OAL_INLINE void oal_timer_init(oal_timer_list_stru *pst_timer, uint32_t ul_delay,
                                          oal_timer_func p_func, unsigned long arg)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0))
    init_timer(pst_timer);
    pst_timer->function = p_func;
    pst_timer->data = arg;
#else
    timer_setup(pst_timer, (void*)p_func, 0);
#endif
    pst_timer->expires = jiffies + msecs_to_jiffies(ul_delay);
}

OAL_STATIC OAL_INLINE int32_t oal_timer_delete(oal_timer_list_stru *pst_timer)
{
    return del_timer(pst_timer);
}

/*
 * �� �� ��  : oal_timer_delete_sync
 * ��������  : ͬ��ɾ����ʱ�������ڶ��
 * �������  : pst_timer: ��ʱ���ṹ��ָ��
 */
OAL_STATIC OAL_INLINE int32_t oal_timer_delete_sync(oal_timer_list_stru *pst_timer)
{
    return del_timer_sync(pst_timer);
}

/*
 * �� �� ��  : oal_timer_add
 * ��������  : ���ʱ��
 * �������  : pst_timer: ��ʱ���ṹ��ָ��
 */
OAL_STATIC OAL_INLINE void oal_timer_add(oal_timer_list_stru *pst_timer)
{
    add_timer(pst_timer);
}

/*
 * �� �� ��  : oal_timer_start
 * ��������  : ������ʱ��
 */
OAL_STATIC OAL_INLINE int32_t oal_timer_start(oal_timer_list_stru *pst_timer, oal_uint ui_delay)
{
    return mod_timer(pst_timer, (jiffies + msecs_to_jiffies(ui_delay)));
}

/*
 * �� �� ��  : oal_timer_start_on
 * ��������  : ָ��cpu,������ʱ��,����ʱtimerҪ���ڷǼ���״̬���߻�����
 */
OAL_STATIC OAL_INLINE void oal_timer_start_on(oal_timer_list_stru *pst_timer, oal_uint ui_delay, int32_t cpu)
{
    pst_timer->expires = jiffies + msecs_to_jiffies(ui_delay);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    add_timer_on(pst_timer, cpu);
#else
    /* �Ͱ汾�ں�add_timer_on����δ���� */
    add_timer(pst_timer);
#endif
}

/*
 * �� �� ��  : oal_copy_from_user
 * ��������  : ���û�̬���ݿ������ں�̬
 * �������  : p_to: Ŀ�ĵ�ַ
 *             p_from : Դ��ַ
 *             ul_size : ��Ҫ�����ĳ���
 */
OAL_STATIC OAL_INLINE uint32_t oal_copy_from_user(void *p_to, const void *p_from, uint32_t ul_size)
{
    return (uint32_t)copy_from_user(p_to, p_from, (oal_uint)ul_size);
}

/*
 * �� �� ��  : oal_copy_to_user
 * ��������  : ���ں�̬���ݿ������û�̬
 * �������  : p_to: Ŀ�ĵ�ַ
 *             p_from : Դ��ַ
 *             ul_size : ��Ҫ�����ĳ���
 */
OAL_STATIC OAL_INLINE uint32_t oal_copy_to_user(void *p_to, const void *p_from, uint32_t ul_size)
{
    return (uint32_t)copy_to_user(p_to, p_from, (oal_uint)ul_size);
}

/*
 * �� �� ��  : oal_create_proc_entry
 * ��������  : ����g_proc_entry�ṹ��
 * �������  : pc_name: ������proc_entry������
 *             us_mode: ����ģʽ
 *             pst_parent: ĸproc_entry�ṹ�壬�̳�����
 */
OAL_STATIC OAL_INLINE oal_proc_dir_entry_stru *oal_create_proc_entry(const char *pc_name, uint16_t us_mode,
                                                                     oal_proc_dir_entry_stru *pst_parent)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    return NULL;
#else
    return create_proc_entry(pc_name, us_mode, pst_parent);
#endif
}

/*
 * �� �� ��  : oal_remove_proc_entry
 * ��������  : ����g_proc_entry�ṹ��
 * �������  : pc_name: ������proc_entry������
 *             pst_parent: ĸproc_entry�ṹ�壬�̳�����
 */
OAL_STATIC OAL_INLINE void oal_remove_proc_entry(const char *pc_name, oal_proc_dir_entry_stru *pst_parent)
{
    return remove_proc_entry(pc_name, pst_parent);
}

/*
 * �� �� ��  : oal_time_is_before
 * ��������  : �ж�ul_time�Ƿ�ȵ�ǰʱ����
 *             ���磬��ʾ��ʱʱ���ѹ��������磬������δ��ʱ
 */
OAL_STATIC OAL_INLINE uint32_t oal_time_is_before(oal_uint ui_time)
{
    return (uint32_t)time_is_before_jiffies(ui_time);
}

/*
 * �� �� ��  : oal_time_after
 * ��������  : �ж�ʱ���ul_time_a�Ƿ���ul_time_b֮��:
 * �� �� ֵ  : Return: 1 ul_time_a��ul_time_b֮��; ���� Return: 0
 */
OAL_STATIC OAL_INLINE uint32_t oal_time_after(unsigned long ul_time_a, unsigned long ul_time_b)
{
    return (uint32_t)time_after(ul_time_a, ul_time_b);
}

/*
 * �� �� ��  : oal_time_after
 * ��������  : �ж�ʱ���ul_time_a�Ƿ���ul_time_b֮��:
 * �� �� ֵ  : Return: 1 ul_time_a��ul_time_b֮��; ���� Return: 0
 * ��������  : �����߼���ֵ���ܴ���ox7FFFFFFF
 */
OAL_STATIC OAL_INLINE uint32_t oal_time_after32(uint32_t ul_time_a, uint32_t ul_time_b)
{
    return (uint32_t)((int32_t)((ul_time_b) - (ul_time_a)) < 0);
}

OAL_INLINE static char *oal_get_current_task_name(void)
{
    return current->comm;
}
/*
 * �� �� ��  : oal_wait_for_completion_timeout
 * ��������  : ͬ�����ȴ���ʱ���
 * �� �� ֵ  : Return: 0 if timed out, and positive (at least 1, or number of jiffies left till timeout) if completed.
 */
OAL_STATIC OAL_INLINE uint32_t oal_wait_for_completion_timeout(oal_completion *pst_completion, uint32_t ul_timeout)
{
    return (uint32_t)wait_for_completion_timeout(pst_completion, ul_timeout);
}


OAL_STATIC OAL_INLINE long oal_wait_for_completion_interruptible_timeout(oal_completion *pst_completion,
                                                                         unsigned long ul_timeout)
{
    return (long)wait_for_completion_interruptible_timeout(pst_completion, ul_timeout);
}

OAL_STATIC OAL_INLINE long oal_wait_for_completion_killable_timeout(oal_completion *pst_completion,
                                                                    unsigned long ul_timeout)
{
    return (long)wait_for_completion_killable_timeout(pst_completion, ul_timeout);
}

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
/*
 * �� �� ��  : oal_smp_task_lock
 * ��������  : lock the task, the lock can be double locked by the same process
 */
extern void _oal_smp_task_lock_(oal_task_lock_stru *pst_lock, uintptr_t claim_addr);
#define oal_smp_task_lock(lock) _oal_smp_task_lock_(lock, (uintptr_t)_THIS_IP_)

/*
 * �� �� ��  : oal_smp_task_unlock
 * ��������  : unlock the task
 */
OAL_STATIC OAL_INLINE void oal_smp_task_unlock(oal_task_lock_stru *pst_lock)
{
    unsigned long flags;

    if (oal_warn_on(in_interrupt() || in_atomic())) {
        return;
    }

    if (oal_unlikely(!pst_lock->claimed)) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_lock->lock, &flags);
    if (--pst_lock->claim_cnt) {
        oal_spin_unlock_irq_restore(&pst_lock->lock, &flags);
    } else {
        pst_lock->claimed = 0;
        pst_lock->claimer = NULL;
        oal_spin_unlock_irq_restore(&pst_lock->lock, &flags);
        wake_up(&pst_lock->wq);
    }
}

OAL_STATIC OAL_INLINE void oal_smp_task_lock_init(oal_task_lock_stru *pst_lock)
{
    memset_s((void *)pst_lock, sizeof(oal_task_lock_stru), 0, sizeof(oal_task_lock_stru));

    oal_spin_lock_init(&pst_lock->lock);
    oal_wait_queue_init_head(&pst_lock->wq);
    pst_lock->claimed = 0;
    pst_lock->claim_cnt = 0;
}
#endif

/*
 * �� �� ��  : oal_get_time_stamp_from_timeval
 * ��������  : ��ȡʱ�侫��
 */
OAL_STATIC OAL_INLINE uint64_t oal_get_time_stamp_from_timeval(void)
{
    struct timeval tv;
    uint64_t curr_time;

    do_gettimeofday(&tv);
    curr_time = tv.tv_usec;
    do_div(curr_time, 1000); // 1msΪ1000us
    curr_time = curr_time + tv.tv_sec * 1000; /* 1s��1000ms */

    return curr_time;
}

OAL_STATIC OAL_INLINE void oal_get_real_time(oal_time_stru *pst_tm)
{
    struct timex txc;
    struct rtc_time tm = {0};

    /* ��ȡ��ǰUTCʱ�� */
    do_gettimeofday(&(txc.time));

    /* ��UTCʱ���������ʱ�� */
    txc.time.tv_sec -= sys_tz.tz_minuteswest * 60; /* 1minute��60sec */
    /* ���ʱ���е������յ���ֵ��tm�� */
    rtc_time_to_tm(txc.time.tv_sec, &tm);

    memcpy_s(pst_tm, OAL_SIZEOF(oal_time_stru), &tm, OAL_SIZEOF(oal_time_stru));
}

OAL_STATIC OAL_INLINE unsigned long oal_access_write_ok(const void __user *addr, unsigned long size)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0))
    return access_ok(VERIFY_WRITE, (uintptr_t)addr, (int)size);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0))
    return access_ok(VERIFY_WRITE, addr, size);
#else
    return access_ok(addr, size);
#endif
}

#endif /* end of oal_schedule.h */
