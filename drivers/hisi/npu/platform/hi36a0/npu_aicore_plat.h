/*
 * npu_aicore_plat.h
 *
 * about npu aicore plat
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
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
#ifndef __DEVDRV_NPU_AICORE_PLAT_H
#define __DEVDRV_NPU_AICORE_PLAT_H

#include "npu_platform_register.h"
#include "npu_adapter.h"
#include "npu_platform.h"

#define NPU_AICORE_DBG_WAIT_LOOP_MAX 1000

#define read_uint64(ullValue, Addr)                                            \
	((ullValue) = *((volatile u64 *)((uintptr_t)(Addr))))

#define write_uint64(ullValue, Addr)                                           \
	(*((volatile u64 *)((uintptr_t)(Addr))) = (ullValue))

struct aicore_exception_info {
	u64 ifu_start;
	u64 aic_error;
	u64 aic_error1;
	u64 aic_error2;
	u64 aic_error_mask;
	u64 aic_core_int;
};

struct aicore_exception_error_info {
	u64 biu_err_info;
	u64 ccu_err_info;
	u64 cube_err_info;
	u64 mte_err_info;
	u64 vec_err_info;
	u64 mte_ecc_1bit_err;
	u64 vec_cube_ecc_1bit_err;
};

struct aicore_exception_dbg_info {
	u64 ifu_current;
	u64 general_register[32];
};

int npu_aicore_query_exception_info(
	u8 aic_id, struct aicore_exception_info *aic_info, u8 do_print);

int npu_aicore_query_exception_error_info(
	u8 aic_id, struct aicore_exception_error_info *aic_info, u8 do_print);

int npu_aicore_query_exception_dbg_info(
	u8 aic_id, struct aicore_exception_dbg_info *aic_info, u8 do_print);

#endif
