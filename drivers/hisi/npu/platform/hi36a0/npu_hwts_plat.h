/*
 * npu_hwts_plat.h
 *
 * about npu hwts plat
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __DEVDRV_NPU_HWTS_PLAT_H
#define __DEVDRV_NPU_HWTS_PLAT_H

#include "npu_platform_register.h"
#include "npu_adapter.h"
#include "npu_platform.h"

#define TEST_AICORE_SHIFT       0x14  // aicore 1 base addr: aicore 0 + 1 << 0x14

#define read_uint32(uwValue, Addr)	  ((uwValue) = *((volatile u32 *)((uintptr_t)(Addr))))

#define read_uint64(ullValue, Addr)	 ((ullValue) = *((volatile u64 *)((uintptr_t)(Addr))))

#define write_uint32(uwValue, Addr)	 (*((volatile u32 *)((uintptr_t)(Addr))) = (uwValue))

#define write_uint64(ullValue, Addr)	(*((volatile u64 *)((uintptr_t)(Addr))) = (ullValue))

static inline void update_reg32(u64 addr, u32 value, u32 mask)
{
	u32 reg_val;
	read_uint32(reg_val, addr);
	reg_val = (reg_val & (~mask)) | (value & mask);
	write_uint32(reg_val, addr);
}

static inline void update_reg64(u64 addr, u64 value, u64 mask)
{
	u64 reg_val;
	read_uint64(reg_val, addr);
	reg_val = (reg_val & (~mask)) | (value & mask);
	write_uint64(reg_val, addr);
}

void write_reg32_readback(u64 addr, u32 value);

void write_reg64_readback(u64 addr, u64 value);

void update_reg32_readback(u64 adr, u32 value, u32 mask);

void update_reg64_readback(u64 addr, u64 value, u64 mask);







void hwts_profiling_init(u8 devid);

struct hwts_interrupt_info {
	u8 aic_batch_mode_timeout_man;
	u8 aic_task_runtime_limit_exp;
	u8 wait_task_limit_exp;
	u64 l1_normal_ns_interrupt;
	u64 l1_debug_ns_interrupt;
	u64 l1_error_ns_interrupt;
	u64 l1_dfx_interrupt;
};

struct sq_exception_info {
	u16 sq_head;
	u16 sq_length;
	u16 sq_tail;
	u8 aic_own_bitmap;
	u64 sqcq_fsm_state;
};

u64 npu_hwts_get_base_addr(void);

int npu_hwts_query_aicore_pool_status(
	u8 index, u8 pool_sec, u8 *reg_val, u8 do_print);

int npu_hwts_query_sdma_pool_status(
	u8 index, u8 pool_sec, u8 *reg_val, u8 do_print);

int npu_hwts_query_bus_error_type(u8 *reg_val, u8 do_print);

int npu_hwts_query_bus_error_id(u8 *reg_val, u8 do_print);

int npu_hwts_query_sw_status(u16 channel_id, u32 *reg_val, u8 do_print);

int npu_hwts_query_sq_head(u16 channel_id, u16 *reg_val, u8 do_print);

int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val, u8 do_print);

int npu_hwts_query_aic_own_bitmap(u16 channel_id, u8 *reg_val, u8 do_print);

int npu_hwts_query_aic_exception_bitmap(u16 channel_id, u8 *reg_val, u8 do_print);

int npu_hwts_query_aic_trap_bitmap(u16 channel_id, u8 *reg_val, u8 do_print);

int npu_hwts_query_sdma_own_state(u16 channel_id, u8 *reg_val, u8 do_print);

int npu_hwts_query_interrupt_info(
	struct hwts_interrupt_info *interrupt_info, u8 do_print);

int npu_hwts_query_sq_info(
	u16 channel_id, struct sq_exception_info *sq_info, u8 do_print);

#endif
