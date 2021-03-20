/*
 * vfmw_osal.h
 *
 * This is for vfmw_osal interface.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
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

#ifndef VFMW_OSAL
#define VFMW_OSAL

#include "vfmw.h"
#include "linux_osal.h"
#include <linux/io.h>
#include <linux/printk.h>

#define OSAL_OK  0
#define OSAL_ERR 1

typedef hi_s32 os_sema;
typedef hi_s32 os_event;
typedef hi_s32 os_lock;

typedef irq_handler_t vfmw_irq_handler_t;
typedef hi_u32 (*fn_osal_get_time_in_ms)(void);
typedef hi_u32 (*fn_osal_get_time_in_us)(void);
typedef hi_s32 (*fn_osal_spin_lock_init)(hi_u32 *);
typedef void (*fn_osal_spin_lock_exit)(hi_u32);
typedef hi_s32 (*fn_osal_spin_lock)(hi_u32, hi_ulong *);
typedef hi_s32 (*fn_osal_spin_unlock)(hi_u32, hi_ulong *);
typedef hi_s32 (*fn_osal_sema_init)(hi_u32 *);
typedef void (*fn_osal_sema_exit)(hi_u32);
typedef hi_s32 (*fn_osal_sema_down)(hi_u32);
typedef hi_s32 (*fn_osal_sema_try)(hi_u32);
typedef void (*fn_osal_sema_up)(hi_u32);
typedef void (*fn_osal_mb)(void);
typedef void (*fn_osal_udelay)(hi_ulong);
typedef void (*fn_osal_msleep)(hi_u32);
typedef hi_s32 (*fn_osal_event_init)(hi_u32 *, hi_s32);
typedef void (*fn_osal_event_exit)(hi_u32);
typedef hi_s32 (*fn_osal_event_give)(hi_u32);
typedef hi_s32 (*fn_osal_event_wait)(hi_u32, hi_u64);
typedef hi_s8 *(*fn_osal_reg_map)(UADDR, hi_u32);
typedef void (*fn_osal_reg_unmap)(hi_u8 *);
typedef void *(*fn_osal_create_thread)(void *, void *, hi_s8 *);
typedef hi_s32 (*fn_osal_stop_task)(void *);
typedef hi_s32 (*fn_osal_kthread_should_stop)(void);
typedef hi_s32 (*fn_osal_request_irq)(hi_u32, vfmw_irq_handler_t, hi_ulong, const char *, void *);
typedef void (*fn_osal_free_irq)(hi_u32, void *);
typedef hi_s32 (*fn_osal_proc_init)(void);
typedef void (*fn_osal_proc_exit)(void);
typedef hi_s32 (*fn_osal_proc_create)(hi_u8 *, void *, void *);
typedef void (*fn_osal_proc_destroy)(hi_u8 *);
typedef void (*fn_osal_dump_proc)(void *, hi_s32, hi_s32 *, hi_s8, const hi_s8 *, ...);

typedef struct {
	hi_s32 check1;

	fn_osal_get_time_in_ms get_time_in_ms;
	fn_osal_get_time_in_us get_time_in_us;
	fn_osal_spin_lock_init os_spin_lock_init;
	fn_osal_spin_lock_exit os_spin_lock_exit;
	fn_osal_spin_lock os_spin_lock;
	fn_osal_spin_unlock os_spin_unlock;
	fn_osal_sema_init sema_init;
	fn_osal_sema_exit sema_exit;
	fn_osal_sema_down sema_down;
	fn_osal_sema_try sema_try;
	fn_osal_sema_up sema_up;
	fn_osal_mb os_mb;
	fn_osal_udelay usdelay;
	fn_osal_msleep msleep;
	fn_osal_event_init event_init;
	fn_osal_event_exit event_exit;
	fn_osal_event_give event_give;
	fn_osal_event_wait event_wait;
	fn_osal_reg_map reg_map;
	fn_osal_reg_unmap reg_unmap;
	fn_osal_create_thread create_thread;
	fn_osal_stop_task stop_thread;
	fn_osal_kthread_should_stop thread_should_stop;
	fn_osal_request_irq request_irq;
	fn_osal_free_irq free_irq;
	fn_osal_proc_init proc_init;
	fn_osal_proc_exit proc_exit;
	fn_osal_proc_create proc_create;
	fn_osal_proc_destroy proc_destroy;
	fn_osal_dump_proc dump_proc;

	hi_s32 check2;
} vfmw_osal_ops;

extern vfmw_osal_ops g_vfmw_osal_ops;

#define OS_MB                 g_vfmw_osal_ops.os_mb
#define OS_UDELAY             g_vfmw_osal_ops.usdelay
#define OS_MSLEEP             msleep
#define OS_EVENT_INIT         g_vfmw_osal_ops.event_init
#define OS_EVENT_EXIT         g_vfmw_osal_ops.event_exit
#define OS_EVENT_GIVE         g_vfmw_osal_ops.event_give
#define OS_EVENT_WAIT         g_vfmw_osal_ops.event_wait
#define OS_KMAP_REG           g_vfmw_osal_ops.reg_map
#define OS_KUNMAP_REG         g_vfmw_osal_ops.reg_unmap
#define OS_CREATE_THREAD      g_vfmw_osal_ops.create_thread
#define OS_STOP_THREAD        g_vfmw_osal_ops.stop_thread
#define OS_THREAD_SHOULD_STOP g_vfmw_osal_ops.thread_should_stop
#define OS_REQUEST_IRQ        g_vfmw_osal_ops.request_irq
#define OS_FREE_IRQ           g_vfmw_osal_ops.free_irq
#define OS_GET_TIME_MS        g_vfmw_osal_ops.get_time_in_ms
#define OS_GET_TIME_US        g_vfmw_osal_ops.get_time_in_us
#define OS_SPIN_LOCK_INIT     g_vfmw_osal_ops.os_spin_lock_init
#define OS_SPIN_LOCK_EXIT     g_vfmw_osal_ops.os_spin_lock_exit
#define OS_SPIN_LOCK          g_vfmw_osal_ops.os_spin_lock
#define OS_SPIN_UNLOCK        g_vfmw_osal_ops.os_spin_unlock
#define OS_SEMA_INIT          g_vfmw_osal_ops.sema_init
#define OS_SEMA_EXIT          g_vfmw_osal_ops.sema_exit
#define OS_SEMA_DOWN          g_vfmw_osal_ops.sema_down
#define OS_SEMA_TRY           g_vfmw_osal_ops.sema_try
#define OS_SEMA_UP            g_vfmw_osal_ops.sema_up
#define OS_PRINT              printk
#define OS_PROC_INIT          g_vfmw_osal_ops.proc_init
#define OS_PROC_EXIT          g_vfmw_osal_ops.proc_exit
#define OS_PROC_CREATE        g_vfmw_osal_ops.proc_create
#define OS_PROC_DESTROY       g_vfmw_osal_ops.proc_destroy
#define OS_DUMP_PROC          g_vfmw_osal_ops.dump_proc

void os_intf_init(void);
void os_intf_exit(void);
sec_mode get_current_sec_mode(void);

#endif
