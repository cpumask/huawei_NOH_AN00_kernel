/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This file is tracking mem stack system.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Author: HISI_DRV1
 * Create: 2019-5-26
 */

#ifndef __MEM_STACK_H
#define __MEM_STACK_H
#include <linux/sizes.h>
#include "linux/hisi/mem_trace.h"

static inline int hisi_vmalloc_track_on(char *name)
{
	return 0;
}
static inline int hisi_vmalloc_track_off(char *name)
{
	return 0;
}
int hisi_vmalloc_stack_open(int type);
int hisi_vmalloc_stack_close(void);
size_t hisi_vmalloc_stack_read(struct hisi_stack_info *buf,
				size_t len, int type);

int hisi_slub_track_on(char *name);
int hisi_slub_track_off(char *name);
static inline int hisi_slub_stack_open(int type)
{
	return 0;
}
static inline int hisi_slub_stack_close(void)
{
	return 0;
}
size_t hisi_slub_stack_read(struct hisi_stack_info *buf,
				size_t len, int type);

static inline int hisi_buddy_track_on(char *name)
{
	return 0;
}
static inline int hisi_buddy_track_off(char *name)
{
	return 0;
}
int hisi_buddy_stack_open(int type);
int hisi_buddy_stack_close(void);
size_t hisi_buddy_stack_read(struct hisi_stack_info *buf,
				size_t len, int type);

size_t vm_type_detail_get(int subtype);
size_t hisi_get_vmalloc_detail(void *buf, size_t len);
unsigned long get_buddy_caller(unsigned long pfn);
extern size_t hisi_get_ion_size_by_pid(pid_t pid);
extern size_t hisi_get_ion_detail(void *buf, size_t len);
#endif

