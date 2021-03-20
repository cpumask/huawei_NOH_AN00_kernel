/*
 * vfmw.h
 *
 * This is for vfmw.
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

#ifndef VFMW_H
#define VFMW_H

#include "vfmw_ext.h"

#ifdef HI_TVP_SUPPORT
#define  TVP_CHAN_NUM 2
#else
#define  TVP_CHAN_NUM 0
#endif

#ifdef CFG_MAX_CHAN_NUM
#define VFMW_CHAN_NUM CFG_MAX_CHAN_NUM
#else
#define VFMW_CHAN_NUM 32
#endif

#define VDEC_OK 0
#define VDEC_ERR (-1)

#define uint64_ptr(ptr) ((void *)(uintptr_t)(ptr))
#define ptr_uint64(ptr) ((hi_u64)(uintptr_t)(ptr))

typedef struct {
	UADDR phy_addr;
	UADDR mmu_addr;
	hi_u64 vir_addr;
	hi_u32 length;
	mem_mode mode;
	hi_u8 asz_name[16];
	hi_bool tvp;
	hi_u32 h_handle;
} vfmw_mem;

typedef struct {
	hi_u64 fd;
	hi_u8 is_cached;
	hi_u8 *vir_addr;
	UADDR phy_addr;
	UADDR mmu_addr;
	hi_u32 length;
	mem_mode mode;
	hi_handle vdec_handle;
	hi_handle ssm_handle;
	hi_u32 protect_id;
	hi_u32 buff_id;
	hi_bool is_map_iova;
} mem_record;

typedef enum {
	NO_SEC_MODE,
	SEC_MODE
} sec_mode;

#endif

