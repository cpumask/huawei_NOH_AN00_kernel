
#include "drv_venc_osal.h"
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/sched/clock.h>

/*lint -e747 -e712 -e732 -e715 -e774 -e845 -e438 -e838*/

HI_S32 venc_drv_osal_irq_init(HI_U32 irq, irqreturn_t (*callback)(HI_S32, HI_VOID *))
{
	HI_S32 ret = 0;

	if (irq == 0) {
		HI_FATAL_VENC("params is invaild\n");
		return HI_FAILURE;
	}

	ret = request_irq(irq, callback, 0, "DT_device", NULL);
	if (ret) {
		HI_FATAL_VENC("request irq failed\n");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_VOID venc_drv_osal_irq_free(HI_U32 irq)
{
	free_irq(irq, NULL);
}

HI_S32 venc_drv_osal_lock_create(spinlock_t **phlock)
{
	spinlock_t *plock = NULL;

	plock = vmalloc(sizeof(spinlock_t));
	if (!plock) {
		HI_FATAL_VENC("vmalloc failed\n");
		return HI_FAILURE;
	}

	spin_lock_init(plock);
	*phlock = plock;

	return HI_SUCCESS;
}

HI_VOID venc_drv_osal_lock_destroy(spinlock_t *hlock)
{
	if (hlock)
		vfree((HI_VOID *)hlock);
}

HI_S32 venc_drv_osal_init_event(vedu_osal_event_t *event, HI_S32 initval)
{
	event->flag = initval;
	init_waitqueue_head(&(event->queue_head));
	return HI_SUCCESS;
}

HI_S32 venc_drv_osal_give_event(vedu_osal_event_t *event)
{
	event->flag = 1;
	wake_up_all(&(event->queue_head));
	return HI_SUCCESS;
}

HI_U64 osal_get_sys_time_in_ms(void)
{
	HI_U64 sys_time;

	sys_time = sched_clock();
	do_div(sys_time, 1000000);

	return sys_time;
}

HI_U64 osal_get_sys_time_in_us(void)
{
	HI_U64 sys_time;

	sys_time = sched_clock();
	do_div(sys_time, 1000);

	return sys_time;
}

HI_U64 osal_get_during_time(HI_U64 start_time)
{
	HI_U64 end_time = osal_get_sys_time_in_us();

	return TIME_PERIOD(start_time, end_time);
}

HI_VOID hi_sleep_ms(HI_U32 millisec)
{
	msleep(millisec);
}

HI_U32 *hi_mmap(HI_U32 addr, HI_U32 range)
{
	HI_U32 *res_addr = NULL;

	res_addr = (HI_U32 *)ioremap(addr, range);
	return res_addr;
}

HI_VOID hi_munmap(HI_U32 *mem_addr)
{
	if (!mem_addr) {
		HI_FATAL_VENC("params is invaild\n");
		return;
	}

	iounmap(mem_addr);
}

HI_S32 hi_strncmp(const HI_PCHAR str_name, const HI_PCHAR dst_name, HI_S32 size)
{
	HI_S32 ret = 0;

	if (str_name && dst_name) {
		ret = strncmp(str_name, dst_name, size);
		return ret;
	}

	return HI_FAILURE;
}

HI_VOID *hi_mem_valloc(HI_U32 mem_size)
{
	HI_VOID *mem_addr = NULL;

	if (mem_size)
		mem_addr = vmalloc(mem_size);

	return mem_addr;
}

HI_VOID hi_mem_vfree(const HI_VOID *mem_addr)
{
	if (mem_addr)
		vfree(mem_addr);
}

HI_VOID hi_venc_init_sem(HI_VOID *sem)
{
	if (sem)
		sema_init((struct semaphore *)sem, 1);
}

HI_S32 hi_venc_down_interruptible(HI_VOID *sem)
{
	HI_S32 ret = -1;

	if (sem)
		ret = down_interruptible((struct semaphore *)sem);

	if (ret)
		HI_FATAL_VENC("ret %d down interruptible fail\n", ret);

	return  ret;
}

HI_VOID  hi_venc_up_interruptible(HI_VOID *sem)
{
	if (sem)
		up((struct semaphore *)sem);
}

HI_VOID osal_init_timer(struct timer_list *timer,
		void (*function)(unsigned long),
		unsigned long data)
{
	if (timer == NULL) {
		HI_FATAL_VENC("input timer is availed\n");
		return;
	}

	if (function == NULL) {
		HI_FATAL_VENC("input callback function is availed\n");
		return;
	}

	setup_timer(timer, function, data);
}

HI_VOID osal_add_timer(struct timer_list *timer, HI_U32 time_in_ms)
{
	if (timer == NULL) {
		HI_FATAL_VENC("input timer is availed\n");
		return;
	}

	mod_timer(timer, jiffies + msecs_to_jiffies(time_in_ms));
}

HI_S32 osal_del_timer(struct timer_list *timer, HI_BOOL is_sync)
{
	if (timer == NULL) {
		HI_FATAL_VENC("input timer is availed\n");
		return HI_FAILURE;
	}

	if (!timer_pending(timer))
		return HI_FAILURE;

	if (is_sync)
		del_timer_sync(timer);
	else
		del_timer(timer);

	return HI_SUCCESS;
}
