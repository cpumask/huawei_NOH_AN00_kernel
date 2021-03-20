/*
 * linux_osal.c
 *
 * This is for linux_osal proc.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "vfmw_osal.h"
#include "linux_osal.h"
#include "linux_proc.h"
#include "dbg.h"
#include <linux/hisi/hisi_ion.h>
#include <linux/sched/clock.h>

#define OS_MAX_CHAN VFMW_CHAN_NUM
#define OS_MAX_SEMA (OS_MAX_CHAN * 4 * 4)
#define OS_MAX_EVENT (OS_MAX_CHAN * 4 * 1)
#define OS_MAX_SPIN_LOCK (OS_MAX_CHAN * 4 * 1)

static osal_irq_spin_lock g_os_lock;
static osal_sema g_os_sema[OS_MAX_SEMA];
static osal_event g_os_event[OS_MAX_EVENT];
static osal_irq_spin_lock g_os_spin_lock[OS_MAX_SPIN_LOCK];

static hi_u16 g_sema_used[OS_MAX_SEMA] = {0};
static hi_u16 g_event_used[OS_MAX_EVENT] = {0};
static hi_u16 g_spin_lock_used[OS_MAX_SPIN_LOCK] = {0};

inline hi_s32 linux_spin_lock_irq(osal_irq_spin_lock *lock, hi_ulong *lock_flag)
{
	if (lock->is_init == 0) {
		spin_lock_init(&lock->irq_lock);
		lock->is_init = 1;
	}
	spin_lock_irqsave(&lock->irq_lock, *lock_flag);

	return OSAL_OK;
}

inline hi_s32 linux_spin_unlock_irq(osal_irq_spin_lock *lock, hi_ulong *lock_flag)
{
	spin_unlock_irqrestore(&lock->irq_lock, *lock_flag);

	return OSAL_OK;
}

/* get kernel idle obj id for obj set */
hi_s32 linux_get_idle_id(hi_u16 used[], hi_s32 num, hi_u32 *idle_id)
{
	hi_s32 id = 0;
	static hi_u16 unid = 1;
	hi_ulong lock_flag;

	linux_spin_lock_irq(&g_os_lock, &lock_flag);

	for (id = 0; id < num; id++) {
		if (used[id] == 0) {
			if (unid >= 0x7000)
				unid = 1;

			used[id] = unid++;
			break;
		}
	}

	linux_spin_unlock_irq(&g_os_lock, &lock_flag);

	if (id < num) {
		*idle_id = id;
		return OSAL_OK;
	} else {
		return OSAL_ERR;
	}
}

hi_u32 linux_get_time_in_ms(void)
{
	hi_u64 sys_time;

	sys_time = sched_clock();
	do_div(sys_time, 1000000);

	return (hi_u32)sys_time;
}

hi_u32 linux_get_time_in_us(void)
{
	hi_u64 sys_time;

	sys_time = sched_clock();
	do_div(sys_time, 1000);

	return (hi_u32)sys_time;
}

void *linux_create_thread(void *task_func, void *param, hi_s8 *task_name)
{
	void *task = NULL;

	task = kthread_create(task_func, param, task_name);
	if (IS_ERR(task)) {
		dprint(PRN_ERROR, "can not create thread!\n");
		return NULL;
	}

	wake_up_process(task);

	return task;
}

hi_s32 linux_stop_task(void *param)
{
	osal_task *task = (osal_task *)param;

	if (!task) {
		dprint(PRN_FATAL, "param is invalid\n");
		return OSAL_ERR;
	}

	kthread_stop(task);

	return OSAL_OK;
}

hi_s32 linux_kthread_should_stop(void)
{
	return kthread_should_stop();
}

hi_s32 linux_request_irq(hi_u32 irq, vfmw_irq_handler_t handler,
	unsigned long flags, const char *name, void *dev)
{
	if (request_irq(irq, (irq_handler_t)handler, flags, name, dev) != 0) {
		dprint(PRN_ERROR, "%s request irq failed !\n", name);
		return OSAL_ERR;
	}

	return OSAL_OK;
}

void linux_free_irq(hi_u32 irq, void *dev)
{
	free_irq(irq, dev);
}

hi_s32 linux_init_event(hi_u32 *handle, hi_s32 init_val)
{
	hi_u32 id;
	hi_s32 ret;
	osal_event *event = HI_NULL;

	ret = linux_get_idle_id(g_event_used, OS_MAX_EVENT, &id);
	if (ret != OSAL_OK)
		return OSAL_ERR;

	event = &g_os_event[id];
	event->flag = init_val;
	init_waitqueue_head(&(event->queue_head));

	*handle = id + (g_event_used[id] << 16);

	return OSAL_OK;
}

void linux_exit_event(hi_u32 handle)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (handle == 0 || id >= OS_MAX_EVENT || g_event_used[id] != unid) {
		dprint(PRN_ERROR, "handle = %x error\n", handle);
		dump_stack();
		return;
	}

	g_event_used[id] = 0;
}

hi_s32 linux_give_event(hi_u32 handle)
{
	osal_event *event = NULL;
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_EVENT || g_event_used[id] != unid) {
		dprint(PRN_ERROR, "handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	event = &g_os_event[id];
	event->flag = 1;
	wake_up(&(event->queue_head));

	return OSAL_ERR;
}

hi_s32 linux_wait_event(hi_u32 handle, hi_u64 ms_wait_time)
{
	hi_s32 l_ret;
	osal_event *event = NULL;
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_EVENT || g_event_used[id] != unid) {
		dprint(PRN_ERROR, "Handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	event = &g_os_event[id];
	l_ret = wait_event_timeout(event->queue_head, (event->flag != 0),
		msecs_to_jiffies(ms_wait_time));
	if (event->flag != 0) {
		event->flag = 0;
		return OSAL_OK;
	}
	event->flag = 0;

	return (l_ret > 0) ? OSAL_OK : OSAL_ERR;
}

hi_s8 *linux_ioremap(UADDR phy_addr, hi_u32 len)
{
	return (hi_s8 *)ioremap_nocache(phy_addr, len);
}

void linux_iounmap(hi_u8 *vir_addr)
{
	iounmap(vir_addr);
}

hi_s32 linux_sema_init(hi_u32 *handle)
{
	hi_u32 id;
	hi_s32 ret;

	ret = linux_get_idle_id(g_sema_used, OS_MAX_SEMA, &id);
	if (ret != OSAL_OK)
		return OSAL_ERR;

	sema_init(&g_os_sema[id], 1);
	*handle = id + (g_sema_used[id] << 16);

	return OSAL_OK;
}

void linux_sema_exit(hi_u32 handle)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (handle == 0 || id >= OS_MAX_SEMA || g_sema_used[id] != unid) {
		dprint(PRN_ERROR, "handle = %x error\n", handle);
		dump_stack();
		return;
	}

	g_sema_used[id] = 0;
}

hi_s32 linux_sema_down(hi_u32 handle)
{
	hi_s32 ret;
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_SEMA || g_sema_used[id] != unid) {
		dprint(PRN_ERROR, "handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	ret = down_interruptible(&g_os_sema[id]);

	return ret;
}

hi_s32 linux_sema_try(hi_u32 handle)
{
	hi_s32 ret;
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_SEMA || g_sema_used[id] != unid) {
		dprint(PRN_ERROR, "Handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	ret = down_trylock(&g_os_sema[id]);

	return ret == 0 ? OSAL_OK : OSAL_ERR;
}

void linux_sema_up(hi_u32 handle)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_SEMA || g_sema_used[id] != unid) {
		dprint(PRN_ERROR, "handle = %x error\n", handle);
		dump_stack();
		return;
	}

	up(&g_os_sema[id]);
}

inline void linux_spin_lock_irq_init(osal_irq_spin_lock *lock)
{
	if (!lock)
		return;

	spin_lock_init(&lock->irq_lock);
	lock->is_init = 1;
}

hi_s32 linux_spin_lock_init(hi_u32 *handle)
{
	hi_u32 id;
	hi_s32 ret;

	ret = linux_get_idle_id(g_spin_lock_used, OS_MAX_SPIN_LOCK, &id);
	if (ret != OSAL_OK)
		return OSAL_ERR;

	linux_spin_lock_irq_init(&g_os_spin_lock[id]);

	*handle = id + (g_spin_lock_used[id] << 16);

	return OSAL_OK;
}

void linux_spin_lock_exit(hi_u32 handle)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (handle == 0 || id >= OS_MAX_SPIN_LOCK || g_spin_lock_used[id] != unid) {
		dprint(PRN_ERROR, "Handle = %x error\n", handle);
		dump_stack();
		return;
	}

	g_spin_lock_used[id] = 0;
}

hi_s32 linux_spin_lock(hi_u32 handle, hi_ulong *lock_flag)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_SPIN_LOCK || g_spin_lock_used[id] != unid) {
		dprint(PRN_ERROR, "Handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	return linux_spin_lock_irq(&g_os_spin_lock[id], lock_flag);
}

hi_s32 linux_spin_unlock(hi_u32 handle, hi_ulong *lock_flag)
{
	hi_s32 id = handle & 0xffff;
	hi_u32 unid = handle >> 16;

	if (id >= OS_MAX_SPIN_LOCK || g_spin_lock_used[id] != unid) {
		dprint(PRN_ERROR, "Handle = %x error\n", handle);
		dump_stack();
		return OSAL_ERR;
	}

	return linux_spin_unlock_irq(&g_os_spin_lock[id], lock_flag);
}

void linux_mb(void)
{
	mb();
}

void linux_udelay(unsigned long usecs)
{
	udelay(usecs);
}

sec_mode linux_get_cur_sec_mode(void)
{
	return NO_SEC_MODE;
}

extern vfmw_osal_ops g_vfmw_osal_ops;

void osal_intf_init(void)
{
	vfmw_osal_ops *ops = &g_vfmw_osal_ops;

	(void)memset_s(ops, sizeof(vfmw_osal_ops), 0, sizeof(vfmw_osal_ops));

	linux_spin_lock_irq_init(&g_os_lock);

	ops->get_time_in_ms = linux_get_time_in_ms;
	ops->get_time_in_us = linux_get_time_in_us;

	ops->os_spin_lock_init = linux_spin_lock_init;
	ops->os_spin_lock_exit = linux_spin_lock_exit;
	ops->os_spin_lock = linux_spin_lock;
	ops->os_spin_unlock = linux_spin_unlock;
	ops->sema_init = linux_sema_init;
	ops->sema_exit = linux_sema_exit;
	ops->sema_down = linux_sema_down;
	ops->sema_try = linux_sema_try;
	ops->sema_up = linux_sema_up;

	ops->os_mb = linux_mb;
	ops->usdelay = linux_udelay;

	ops->event_init = linux_init_event;
	ops->event_exit = linux_exit_event;
	ops->event_give = linux_give_event;
	ops->event_wait = linux_wait_event;

	ops->reg_map = linux_ioremap;
	ops->reg_unmap = linux_iounmap;

	ops->create_thread = linux_create_thread;
	ops->stop_thread = linux_stop_task;
	ops->thread_should_stop = linux_kthread_should_stop;

	ops->request_irq = linux_request_irq;
	ops->free_irq  = linux_free_irq;
#ifdef VFMW_PROC_SUPPORT
	ops->proc_init = (fn_osal_proc_init)linux_proc_init;
	ops->proc_exit = (fn_osal_proc_exit)linux_proc_exit;
	ops->proc_create = (fn_osal_proc_create)linux_proc_create;
	ops->proc_destroy = (fn_osal_proc_destroy)linux_proc_destroy;
#endif
	ops->dump_proc = (fn_osal_dump_proc)linux_proc_dump;
}

void osal_intf_exit(void)
{
	(void)memset_s(&g_vfmw_osal_ops, sizeof(g_vfmw_osal_ops),
		0, sizeof(vfmw_osal_ops));
}
