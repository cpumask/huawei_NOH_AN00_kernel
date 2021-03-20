/*
 * linux_proc.h
 *
 * This is for linux_proc impl.
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

#ifndef LINUX_PROC
#define LINUX_PROC

#include <linux/uaccess.h>
#include "linux/seq_file.h"

#define NAME_SIZE 16

#define OS_VA_LIST va_list
#define OS_VA_START va_start
#define OS_VA_END va_end

#define OS_PROC_FILE_S struct seq_file
#define OS_PROC_PRINT seq_printf
#define OS_COPY_FROM_USER copy_from_user

#define PROC_FILE OS_PROC_FILE_S
#define PDT_VERSION VERSION_STRING

typedef hi_s32 (*proc_read_fn)(struct seq_file *file, void *);
typedef hi_s32 (*proc_write_fn)(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos);

typedef struct {
	hi_u8 proc_name[NAME_SIZE];
	proc_read_fn read;
	proc_write_fn write;
} proc_ops;

hi_s32 linux_proc_create(hi_u8 *proc_name, void *read, void *write);
void linux_proc_destroy(hi_u8 *proc_name);

hi_s32 linux_proc_init(void);
void linux_proc_exit(void);
void linux_proc_dump(void *page, hi_s32 page_count, hi_s32 *used_bytes,
	hi_s8 from_shr, const hi_s8 *format, ...);

#endif
