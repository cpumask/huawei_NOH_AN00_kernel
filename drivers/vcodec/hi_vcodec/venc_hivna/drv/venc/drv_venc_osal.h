/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: venc drv
 * Author: JiMuzhou     jimuzhou@huawei.com
 * Create: 2009/12/17
 */
#ifndef __DRV_VENC_OSAL_H__
#define __DRV_VENC_OSAL_H__

#include <linux/rtc.h>
#include <linux/kfifo.h>
#include <linux/timer.h>
#include "hi_type.h"

typedef struct hi_kernel_event {
	wait_queue_head_t   queue_head;
	HI_S32              flag;
} kernel_event_t;

typedef kernel_event_t  vedu_osal_event_t;
typedef unsigned long vedu_lock_flag_t;
typedef struct timer_list venc_timer_t;

#define TIME_PERIOD(begin, end) (((end) >= (begin)) ? ((end) - (begin)) : (~0LL - (begin) + (end)))
#define  MESCS_TO_JIFFIES(time)                         msecs_to_jiffies(time)
#define  HI_WAIT_EVENT(event, flag)                     wait_event_interruptible((event), (flag))
#define  HI_WAIT_EVENT_TIME_OUT(event, flag, ms_wait_time)  wait_event_interruptible_timeout((event), (flag), (ms_wait_time))
#define DEFAULT_PRINT_ENABLE 0xf

typedef enum {
	VENC_FATAL = 0,
	VENC_ERR,
	VENC_WARN,
	VENC_INFO,
	VENC_DBG,
	VENC_ALW,
	VENC_LOG_BUTT,
} venc_print_t;

static const char *psz_msg[(HI_U8)VENC_LOG_BUTT] = {
	"VENC_FATAL",
	"VENC_ERR",
	"VENC_WARN",
	"VENC_IFO",
	"VENC_DBG",
	"VENC_ALW"
}; /*lint !e785*/

#define hi_print(type, fmt, arg...) \
do { \
	if (((type) == VENC_ALW) || ((DEFAULT_PRINT_ENABLE & (1LL << (type))) != 0)) \
		printk(KERN_ALERT "%s:<%d:%s>"fmt, psz_msg[type], (int)__LINE__, (char *)__func__, ##arg); \
} while (0)

#define HI_FATAL_VENC(fmt, arg...) hi_print(VENC_FATAL, fmt, ##arg)
#define HI_ERR_VENC(fmt, arg...)   hi_print(VENC_ERR, fmt, ##arg)
#define HI_WARN_VENC(fmt, arg...)  hi_print(VENC_WARN, fmt, ##arg)
#define HI_INFO_VENC(fmt, arg...)  hi_print(VENC_INFO, fmt, ##arg)
#define HI_DBG_VENC(fmt, arg...)   hi_print(VENC_DBG, fmt, ##arg)


#define OSAL_WAIT_EVENT_TIMEOUT(event, condtion, timeout_in_ms) \
({ \
	int _ret = timeout_in_ms; \
	HI_U64 start_time, cur_time; \
	start_time = osal_get_sys_time_in_ms(); \
	while (!(condtion) && (_ret != 0)) { \
		_ret = wait_event_interruptible_timeout(((event)->queue_head), (condtion), (msecs_to_jiffies(timeout_in_ms))); /*lint !e665 !e666 !e40 !e713 !e578*/ \
		if (_ret < 0) { \
			cur_time = osal_get_sys_time_in_ms(); \
			if (TIME_PERIOD(start_time, cur_time) > (HI_U64)(timeout_in_ms)) { \
				HI_FATAL_VENC("waitevent time out, time : %lld", \
					TIME_PERIOD(start_time, cur_time)); \
				_ret = 0; \
				break; \
			} \
		} \
	} \
	if (_ret == 0) \
		HI_WARN_VENC("waitevent timeout"); \
	if ((condtion)) { \
		_ret = HI_SUCCESS; \
	} else { \
		_ret = HI_FAILURE; \
	} \
	_ret; \
})

#define queue_is_empty(queue) kfifo_is_empty(&queue->fifo)
#define queue_is_null(queue) (queue->fifo.kfifo.data == NULL)

#define create_queue(type) \
({ \
	type *queue = NULL; \
	type *tmp = NULL; \
	tmp = (type *)kzalloc(sizeof(type), GFP_KERNEL); \
	if (!IS_ERR_OR_NULL(tmp)) { \
		if (venc_drv_osal_lock_create(&tmp->lock)) { \
			HI_FATAL_VENC("alloc lock failed"); \
			kfree(tmp); \
		} else { \
			venc_drv_osal_init_event(&tmp->event, 0); \
			queue = tmp; \
		} \
	} else { \
		HI_FATAL_VENC("alloc queue failed"); \
	} \
	queue; \
})

#define alloc_queue(queue, size) \
({ \
	HI_S32 _ret = HI_SUCCESS; \
	if (kfifo_alloc(&queue->fifo, size, GFP_KERNEL)) { \
		_ret = HI_FAILURE; \
		HI_FATAL_VENC("alloc kfifo failed"); \
	} \
	_ret; \
})

/* queue->fifo.kfifo.data is set to NULL in kfifo_free. */
#define free_queue(queue) \
({ \
	unsigned long _flag; \
	spin_lock_irqsave(queue->lock, _flag); \
	if (!queue_is_null(queue)) \
		kfifo_free(&queue->fifo); \
	spin_unlock_irqrestore(queue->lock, _flag); \
})

#define destory_queue(queue) \
({ \
	venc_drv_osal_lock_destroy(queue->lock); \
	kfree(queue); \
	queue = NULL; \
})

#define pop(queue, buf) \
({ \
	HI_S32 _len; \
	HI_S32 _ret = HI_FAILURE; \
	unsigned long _flag; \
	spin_lock_irqsave(queue->lock, _flag); \
	do { \
		if (queue_is_null(queue)) { \
			HI_ERR_VENC("pop data is failed"); \
			break; \
		} \
		_len = kfifo_out(&queue->fifo, buf, 1); \
		_ret = (_len == 1) ? HI_SUCCESS : HI_FAILURE; \
	} while(0); \
	spin_unlock_irqrestore(queue->lock, _flag); \
	_ret; \
})

#define push(queue, buf) \
({ \
	HI_S32 _len; \
	HI_S32 _ret = HI_FAILURE; \
	unsigned long _flag; \
	spin_lock_irqsave(queue->lock, _flag); \
	do { \
		if (queue_is_null(queue)) { \
			HI_ERR_VENC("push data is failed"); \
			break; \
		} \
		_len = kfifo_in(&queue->fifo, buf, 1); \
		_ret = (_len == 1) ? HI_SUCCESS : HI_FAILURE; \
	} while(0); \
	spin_unlock_irqrestore(queue->lock, _flag); \
	_ret; \
})

HI_U32      *hi_mmap(HI_U32 addr, HI_U32 range);
HI_VOID      hi_munmap(HI_U32 *mem_addr);
HI_S32       hi_strncmp(const HI_PCHAR str_name, const HI_PCHAR dst_name, HI_S32 size);
HI_VOID      hi_sleep_ms(HI_U32 millisec);
HI_VOID     *hi_mem_valloc(HI_U32 mem_size);
HI_VOID      hi_mem_vfree(const HI_VOID *mem_addr);
HI_VOID      hi_venc_init_sem(HI_VOID *sem);
HI_S32       hi_venc_down_interruptible(HI_VOID *sem);
HI_VOID      hi_venc_up_interruptible(HI_VOID *sem);

HI_S32 venc_drv_osal_irq_init(HI_U32 irq, irqreturn_t (*callback)(HI_S32, HI_VOID *));
HI_VOID  venc_drv_osal_irq_free(HI_U32 irq);
HI_S32   venc_drv_osal_lock_create(spinlock_t **phlock);
HI_VOID  venc_drv_osal_lock_destroy(spinlock_t *hlock);

HI_S32 venc_drv_osal_init_event(vedu_osal_event_t *event, HI_S32 initval);
HI_S32 venc_drv_osal_give_event(vedu_osal_event_t *event);
HI_S32 venc_drv_osal_wait_event(vedu_osal_event_t *event, HI_U32 ms_wait_time);

HI_VOID osal_init_timer(struct timer_list *timer,
		void (*function)(unsigned long),
		unsigned long data);
HI_VOID osal_add_timer(struct timer_list *timer, HI_U32 time_in_ms);
HI_S32 osal_del_timer(struct timer_list *timer, HI_BOOL is_sync);
HI_U64 osal_get_sys_time_in_ms(void);
HI_U64 osal_get_sys_time_in_us(void);
HI_U64 osal_get_during_time(HI_U64 start_time);

#endif // __DRV_VENC_OSAL_H__

