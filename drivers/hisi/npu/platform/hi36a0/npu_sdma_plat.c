/*
 * npu_sdma_plat.c
 *
 * about npu sdma plat
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
#include "npu_sdma_plat.h"

u64 npu_sdma_get_base_addr(void)
{
	u64 sdma_base;
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();

	COND_RETURN_ERROR(plat_info == NULL, 0ULL, "devdrv_plat_get_info failed\n");
	sdma_base = (u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(
		plat_info, DEVDRV_REG_SDMA_BASE);
	NPU_DRV_DEBUG("sdma_base = 0x%llx\n", sdma_base);
	return sdma_base;
}

int npu_sdma_query_exception_info(
	u8 sdma_channel, struct sdma_exception_info *sdma_info, u8 do_print)
{
	u64 addr_low = 0ULL;
	u64 addr_high = 0ULL;
	SOC_NPU_sysDMA_DEBUG_SQE1_UNION debug_sqe1 = {0};
	SOC_NPU_sysDMA_DEBUG_SQE4_UNION debug_sqe4 = {0};
	SOC_NPU_sysDMA_DEBUG_SQE5_UNION debug_sqe5 = {0};
	SOC_NPU_sysDMA_DEBUG_SQE6_UNION debug_sqe6 = {0};
	SOC_NPU_sysDMA_DEBUG_SQE7_UNION debug_sqe7 = {0};
	u64 base = npu_sdma_get_base_addr();

	COND_RETURN_ERROR(base == 0ULL, -EINVAL, "sdma base is NULL\n");
	COND_RETURN_ERROR(sdma_info == NULL, -EINVAL, "sdma_info is NULL\n");

	read_uint32(debug_sqe4.value, SOC_NPU_sysDMA_DEBUG_SQE4_ADDR(base));
	addr_low = debug_sqe4.reg.s_addr_l;
	read_uint32(debug_sqe5.value, SOC_NPU_sysDMA_DEBUG_SQE5_ADDR(base));
	addr_high = debug_sqe5.reg.s_addr_h;
	sdma_info->s_addr = (addr_high << 32) | addr_low;

	read_uint32(debug_sqe6.value, SOC_NPU_sysDMA_DEBUG_SQE6_ADDR(base));
	addr_low = debug_sqe6.reg.d_addr_l;
	read_uint32(debug_sqe7.value, SOC_NPU_sysDMA_DEBUG_SQE7_ADDR(base));
	addr_high = debug_sqe7.reg.d_addr_h;
	sdma_info->d_addr = (addr_high << 32) | addr_low;

	read_uint32(debug_sqe1.value, SOC_NPU_sysDMA_DEBUG_SQE1_ADDR(base));
	sdma_info->s_sid = debug_sqe1.reg.s_sid;
	sdma_info->s_subsid = debug_sqe1.reg.s_subsid;
	sdma_info->d_sid = debug_sqe1.reg.d_sid;
	sdma_info->d_subsid = debug_sqe1.reg.d_subsid;
	if (do_print != 0) {
		NPU_DRV_WARN("SRC_SID = 0x%02x, SRC_SSID = 0x%02x\n", sdma_info->s_sid,
			sdma_info->s_subsid);
		NPU_DRV_WARN("DST_SID = 0x%02x, DST_SSID = 0x%02x\n", sdma_info->s_sid,
			sdma_info->s_subsid);
	}
	return 0;
}
