/*
 * npu_hwts_plat.c
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

#include "npu_hwts_plat.h"
#include "npu_shm.h"

void write_reg32_readback(u64 addr, u32 value)
{
	u32 read_value = 0;
	write_uint32(value, addr);
	read_uint32(read_value, addr);
	NPU_DRV_INFO ("addr:0x%lx,write value:0x%x, read value:0x%x", addr, value, read_value);
}

void write_reg64_readback(u64 addr, u64 value)
{
	u64 read_value = 0;
	write_uint64(value, addr);
	read_uint64(read_value, addr);
	NPU_DRV_INFO ("addr:0x%lx,write value:0x%lx, read value:0x%lx", addr, value, read_value);
}

void update_reg32_readback(u64 addr, u32 value, u32 mask)
{
	u32 read_value = 0;
	update_reg32(addr, value, mask);
	read_uint32(read_value, addr);
	NPU_DRV_INFO ("addr:0x%lx,write value:0x%x, mask=0x%x, read value:0x%x",
		addr, value, mask, read_value);
}

void update_reg64_readback(u64 addr, u64 value, u64 mask)
{
	u64 read_value = 0;
	update_reg64(addr, value, mask);
	read_uint64(read_value, addr);
	NPU_DRV_INFO ("addr:0x%lx,write value:0x%lx, mask=0x%lx, read value:0x%lx",
		addr, value, mask, read_value);
}


void hwts_log_reg_init(u8 devid, u64 hwts_base)
{
	SOC_NPU_HWTS_DFX_LOG_CTRL_UNION hwts_dfx_log_ctrl = {0};
	SOC_NPU_HWTS_DFX_LOG_BASE_ADDR_UNION hwts_dfx_log_base = {0};
	SOC_NPU_HWTS_DFX_LOG_PTR_UNION hwts_dfx_log_ptr = {0};
	struct buff_ring_manager *ring_manager = NULL;
	struct devdrv_prof_info *profiling_info = NULL;
	u32 hwts_log_addr;
	u32 log_buf_length;

	profiling_info = devdrv_calc_profiling_info(devid);
	COND_RETURN_VOID(profiling_info == NULL, "profiling_info is null, channel = %d\n",
		PROF_HWTS_LOG_CHANNEL);

	ring_manager = &(profiling_info->head.manager.ring_buff[PROF_HWTS_LOG_CHANNEL]);
	COND_RETURN_VOID(ring_manager == NULL, "ring_manager is null, channel = %d\n",
		PROF_HWTS_LOG_CHANNEL);
	log_buf_length = ring_manager->length;
	hwts_log_addr = ring_manager->iova_addr;

	hwts_dfx_log_ctrl.reg.task_log_en = 1;
	hwts_dfx_log_ctrl.reg.log_buf_length = (u16)log_buf_length;
	hwts_dfx_log_ctrl.reg.log_af_threshold = (u16)log_buf_length;
	write_reg64_readback(SOC_NPU_HWTS_DFX_LOG_CTRL_ADDR(hwts_base),
		hwts_dfx_log_ctrl.value);

	hwts_dfx_log_base.reg.log_base_addr = hwts_log_addr;
	hwts_dfx_log_base.reg.log_base_addr_is_virtual = 1;
	write_reg64_readback(SOC_NPU_HWTS_DFX_LOG_BASE_ADDR_ADDR(hwts_base),
		hwts_dfx_log_base.value);

	hwts_dfx_log_ptr.reg.log_rptr = 0;
	write_reg64_readback(SOC_NPU_HWTS_DFX_LOG_PTR_ADDR(hwts_base),
		hwts_dfx_log_ptr.value);
}

void hwts_profiling_reg_init(u8 devid, u64 hwts_base)
{
	SOC_NPU_HWTS_DFX_PROFILE_CTRL_UNION hwts_dfx_profile_ctrl = {0};
	SOC_NPU_HWTS_DFX_PROFILE_BASE_ADDR_UNION hwts_dfx_profile_base = {0};
	SOC_NPU_HWTS_DFX_PROFILE_PTR_UNION hwts_dfx_profile_ptr = {0};
	struct buff_ring_manager *ring_manager = NULL;
	struct devdrv_prof_info *profiling_info = NULL;
	u32 hwts_profile_addr;
	u32 profile_buf_length;

	profiling_info = devdrv_calc_profiling_info(devid);
	COND_RETURN_VOID(profiling_info == NULL, "profiling_info is null, channel = %d\n",
		PROF_HWTS_PROFILING_CHANNEL);

	ring_manager = &(profiling_info->head.manager.ring_buff[PROF_HWTS_PROFILING_CHANNEL]);
	COND_RETURN_VOID(ring_manager == NULL, "ring_manager is null, channel = %d\n",
		PROF_HWTS_PROFILING_CHANNEL);

	profile_buf_length = ring_manager->length;
	hwts_profile_addr = ring_manager->iova_addr;

	hwts_dfx_profile_ctrl.reg.profile_en = 1;
	hwts_dfx_profile_ctrl.reg.profile_buf_length = (u16)profile_buf_length;
	hwts_dfx_profile_ctrl.reg.profile_af_threshold = (u16)profile_buf_length;
	write_reg64_readback(SOC_NPU_HWTS_DFX_PROFILE_CTRL_ADDR(hwts_base),
		hwts_dfx_profile_ctrl.value);

	hwts_dfx_profile_base.reg.profile_base_addr = hwts_profile_addr;
	hwts_dfx_profile_base.reg.profile_base_addr_is_virtual = 1;
	write_reg64_readback(SOC_NPU_HWTS_DFX_PROFILE_BASE_ADDR_ADDR(hwts_base),
		hwts_dfx_profile_base.value);

	hwts_dfx_profile_ptr.reg.profile_rptr = 0;
	write_reg64_readback(SOC_NPU_HWTS_DFX_PROFILE_PTR_ADDR(hwts_base),
		hwts_dfx_profile_ptr.value);
}

void profiling_clear_mem_ptr(u8 devid)
{
	int i;
	struct buff_ring_manager *ring_manager = NULL;
	struct devdrv_prof_info *profiling_info = NULL;

	for (i = 0; i < PROF_CHANNEL_MAX_NUM; i++) {
		profiling_info = devdrv_calc_profiling_info(devid);
		COND_RETURN_VOID(profiling_info == NULL, "profiling_info is null, channel = %d\n", i);

		ring_manager = &(profiling_info->head.manager.ring_buff[i]);
		COND_RETURN_VOID(ring_manager == NULL, "ring_manager is null, channel = %d\n", i);

		ring_manager->read = 0;
		ring_manager->write = 0;
	}
}

void hwts_profiling_init(u8 devid)
{
	struct devdrv_platform_info *plat_info = NULL;
	u64 hwts_addr = 0;
	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("get plat_ops failed.\n");
		return;
	}
	hwts_addr = (u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_HWTS_BASE);

	hwts_log_reg_init(devid, hwts_addr);
	hwts_profiling_reg_init(devid, hwts_addr);

	/* hwts clear wirte&read ptr when power up, */
	/* clear ptr in profiling info remem synchronously */
	profiling_clear_mem_ptr(devid);
}

u64 npu_hwts_get_base_addr(void)
{
	u64 hwts_base = 0ULL;
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();

	COND_RETURN_ERROR(plat_info == NULL, 0ULL, "devdrv_plat_get_info failed\n");
	hwts_base = (u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(plat_info,
		DEVDRV_REG_HWTS_BASE);
	NPU_DRV_DEBUG("hwts_base = 0x%llx\n", hwts_base);
	return hwts_base;
}

int npu_hwts_query_aicore_pool_status(
	u8 index, u8 pool_sec, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS0_NS_UNION aicore_pool_status0_ns = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS1_NS_UNION aicore_pool_status1_ns = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS2_NS_UNION aicore_pool_status2_ns = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS3_NS_UNION aicore_pool_status3_ns = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS0_S_UNION aicore_pool_status0_s = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS1_S_UNION aicore_pool_status1_s = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS2_S_UNION aicore_pool_status2_s = {0};
	SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS3_S_UNION aicore_pool_status3_s = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	if (pool_sec == 0) {
		if (index == 0) {
			read_uint64(aicore_pool_status0_ns.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS0_NS_ADDR(hwts_base));
			*reg_val = aicore_pool_status0_ns.reg.aic_enabled_status0_ns;
		} else if (index == 1) {
			read_uint64(aicore_pool_status1_ns.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS1_NS_ADDR(hwts_base));
			*reg_val = aicore_pool_status1_ns.reg.aic_enabled_status1_ns;
		} else if (index == 2) {
			read_uint64(aicore_pool_status2_ns.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS2_NS_ADDR(hwts_base));
			*reg_val = aicore_pool_status2_ns.reg.aic_enabled_status2_ns;
		} else {
			read_uint64(aicore_pool_status3_ns.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS3_NS_ADDR(hwts_base));
			*reg_val = aicore_pool_status3_ns.reg.aic_enabled_status3_ns;
		}
		if (do_print != 0)
			NPU_DRV_WARN(
				"HWTS_AICORE_POOL_STATUS%u_NS = 0x%x\n", index, *reg_val);
	} else {
		if (index == 0) {
			read_uint64(aicore_pool_status0_s.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS0_S_ADDR(hwts_base));
			*reg_val = aicore_pool_status0_s.reg.aic_enabled_status0_s;
		} else if (index == 1) {
			read_uint64(aicore_pool_status1_s.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS1_S_ADDR(hwts_base));
			*reg_val = aicore_pool_status1_s.reg.aic_enabled_status1_s;
		} else if (index == 2) {
			read_uint64(aicore_pool_status2_s.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS2_S_ADDR(hwts_base));
			*reg_val = aicore_pool_status2_s.reg.aic_enabled_status2_s;
		} else {
			read_uint64(aicore_pool_status3_s.value,
				SOC_NPU_HWTS_HWTS_AICORE_POOL_STATUS3_S_ADDR(hwts_base));
			*reg_val = aicore_pool_status3_s.reg.aic_enabled_status3_s;
		}
		if (do_print != 0)
			NPU_DRV_WARN("HWTS_AICORE_POOL_STATUS%u_S = 0x%x\n", index, *reg_val);
	}
	return 0;
}

int npu_hwts_query_sdma_pool_status(
	u8 index, u8 pool_sec, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS0_NS_UNION sdma_pool_status0_ns = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS1_NS_UNION sdma_pool_status1_ns = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS2_NS_UNION sdma_pool_status2_ns = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS3_NS_UNION sdma_pool_status3_ns = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS0_S_UNION sdma_pool_status0_s = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS1_S_UNION sdma_pool_status1_s = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS2_S_UNION sdma_pool_status2_s = {0};
	SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS3_S_UNION sdma_pool_status3_s = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	if (pool_sec == 0) {
		if (index == 0) {
			read_uint64(sdma_pool_status0_ns.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS0_NS_ADDR(hwts_base));
			*reg_val = sdma_pool_status0_ns.reg.sdma_enabled_status0_ns;
		} else if (index == 1) {
			read_uint64(sdma_pool_status1_ns.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS1_NS_ADDR(hwts_base));
			*reg_val = sdma_pool_status1_ns.reg.sdma_enabled_status1_ns;
		} else if (index == 2) {
			read_uint64(sdma_pool_status2_ns.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS2_NS_ADDR(hwts_base));
			*reg_val = sdma_pool_status2_ns.reg.sdma_enabled_status2_ns;
		} else {
			read_uint64(sdma_pool_status3_ns.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS3_NS_ADDR(hwts_base));
			*reg_val = sdma_pool_status3_ns.reg.sdma_enabled_status3_ns;
		}
		if (do_print != 0)
			NPU_DRV_WARN("HWTS_SDMA_POOL_STATUS%u_NS = 0x%x\n", index, *reg_val);
	} else {
		if (index == 0) {
			read_uint64(sdma_pool_status0_s.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS0_S_ADDR(hwts_base));
			*reg_val = sdma_pool_status0_s.reg.sdma_enabled_status0_s;
		} else if (index == 1) {
			read_uint64(sdma_pool_status1_s.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS1_S_ADDR(hwts_base));
			*reg_val = sdma_pool_status1_s.reg.sdma_enabled_status1_s;
		} else if (index == 2) {
			read_uint64(sdma_pool_status2_s.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS2_S_ADDR(hwts_base));
			*reg_val = sdma_pool_status2_s.reg.sdma_enabled_status2_s;
		} else {
			read_uint64(sdma_pool_status3_s.value,
				SOC_NPU_HWTS_HWTS_SDMA_POOL_STATUS3_S_ADDR(hwts_base));
			*reg_val = sdma_pool_status3_s.reg.sdma_enabled_status3_s;
		}
		if (do_print != 0)
			NPU_DRV_WARN("HWTS_SDMA_POOL_STATUS%u_S = 0x%x\n", index, *reg_val);
	}
	return 0;
}

int npu_hwts_query_bus_error_type(u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_BUS_ERR_INFO_UNION bus_err_info = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(bus_err_info.value,
		SOC_NPU_HWTS_HWTS_BUS_ERR_INFO_ADDR(hwts_base));
	*reg_val = bus_err_info.reg.bus_err_type;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_BUS_ERR_INFO.bus_err_type = 0x%x\n",
			bus_err_info.reg.bus_err_type);
	return 0;
}

int npu_hwts_query_bus_error_id(u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_BUS_ERR_INFO_UNION bus_err_info = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(bus_err_info.value,
		SOC_NPU_HWTS_HWTS_BUS_ERR_INFO_ADDR(hwts_base));
	*reg_val = bus_err_info.reg.bus_err_id;
	if (do_print != 0)
		NPU_DRV_WARN(
			"HWTS_BUS_ERR_INFO.bus_err_id = 0x%x\n", bus_err_info.reg.bus_err_id);
	return 0;
}

int npu_hwts_query_sw_status(u16 channel_id, u32 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_SQ_SW_STATUS_UNION sq_sw_status = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sq_sw_status.value,
		SOC_NPU_HWTS_HWTS_SQ_SW_STATUS_ADDR(hwts_base, channel_id));
	*reg_val = sq_sw_status.reg.sq_sw_status;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_BUS_ERR_INFO.sq_sw_status = 0x%08x\n",
			sq_sw_status.reg.sq_sw_status);
	return 0;
}

int npu_hwts_query_sq_head(u16 channel_id, u16 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_SQ_CFG0_UNION sq_cfg0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sq_cfg0.value,
		SOC_NPU_HWTS_HWTS_SQ_CFG0_ADDR(hwts_base, channel_id));
	*reg_val = sq_cfg0.reg.sq_head;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_SQ_CFG0.sq_head = 0x%04x\n", sq_cfg0.reg.sq_head);
	return 0;
}

int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_SQCQ_FSM_MISC_STATE_UNION sqcq_fsm_misc_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sqcq_fsm_misc_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_MISC_STATE_ADDR(hwts_base, channel_id));
	*reg_val = sqcq_fsm_misc_state.reg.sqe_type;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_MISC_STATE.sqe_type = 0x%x\n",
			sqcq_fsm_misc_state.reg.sqe_type);
	return 0;
}

int npu_hwts_query_aic_own_bitmap(u16 channel_id, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_SQCQ_FSM_AIC_OWN_STATE_UNION sqcq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sqcq_fsm_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_AIC_OWN_STATE_ADDR(hwts_base, channel_id));
	*reg_val = sqcq_fsm_state.reg.aic_own_bitmap;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_AIC_OWN_STATE.aic_own_bitmap = 0x%x\n",
			sqcq_fsm_state.reg.aic_own_bitmap);
	return 0;
}

int npu_hwts_query_aic_exception_bitmap(u16 channel_id, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_SQCQ_FSM_AIC_EXCEPTION_STATE_UNION sqcq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sqcq_fsm_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_AIC_EXCEPTION_STATE_ADDR(hwts_base, channel_id));
	*reg_val = sqcq_fsm_state.reg.aic_exception_bitmap;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_AIC_EXCEPTION_STATE.aic_exception_bitmap = 0x%x\n",
			sqcq_fsm_state.reg.aic_exception_bitmap);
	return 0;
}

int npu_hwts_query_aic_trap_bitmap(u16 channel_id, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_SQCQ_FSM_AIC_TRAP_STATE_UNION sqcq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sqcq_fsm_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_AIC_TRAP_STATE_ADDR(hwts_base, channel_id));
	*reg_val = sqcq_fsm_state.reg.aic_trap_bitmap;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_AIC_TRAP_STATE.aic_trap_bitmap = 0x%x\n",
			sqcq_fsm_state.reg.aic_trap_bitmap);
	return 0;
}

int npu_hwts_query_sdma_own_state(u16 channel_id, u8 *reg_val, u8 do_print)
{
	SOC_NPU_HWTS_SQCQ_FSM_SDMA_OWN_STATE_UNION sqcq_fsm_sdma_own_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	read_uint64(sqcq_fsm_sdma_own_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_SDMA_OWN_STATE_ADDR(hwts_base, channel_id));
	*reg_val = sqcq_fsm_sdma_own_state.reg.owned_sdma_ch_bitmap;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_SDMA_OWN_STATE.owned_sdma_ch_bitmap = 0x%x\n",
			sqcq_fsm_sdma_own_state.reg.owned_sdma_ch_bitmap);
	return 0;
}

int npu_hwts_query_interrupt_info(
	struct hwts_interrupt_info *interrupt_info, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_GLB_CTRL1_UNION glb_ctrl1 = {0};
	SOC_NPU_HWTS_HWTS_L1_NORMAL_NS_INTERRUPT_UNION l1_normal_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_DEBUG_NS_INTERRUPT_UNION l1_debug_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_ERROR_NS_INTERRUPT_UNION l1_error_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_DFX_INTERRUPT_UNION l1_dfx_interrupt = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(interrupt_info == NULL,
		-EINVAL, "interrupt_info is NULL\n");

	read_uint64(glb_ctrl1.value, SOC_NPU_HWTS_HWTS_GLB_CTRL1_ADDR(hwts_base));
	interrupt_info->aic_batch_mode_timeout_man =
		glb_ctrl1.reg.aic_batch_mode_timeout_man;
	interrupt_info->aic_task_runtime_limit_exp =
		glb_ctrl1.reg.aic_task_runtime_limit_exp;
	interrupt_info->wait_task_limit_exp = glb_ctrl1.reg.wait_task_limit_exp;
	if (do_print != 0) {
		NPU_DRV_WARN("HWTS_GLB_CTRL1.aic_batch_mode_timeout_man = 0x%x\n",
			glb_ctrl1.reg.aic_batch_mode_timeout_man);
		NPU_DRV_WARN("HWTS_GLB_CTRL1.aic_task_runtime_limit_exp = 0x%x\n",
			glb_ctrl1.reg.aic_task_runtime_limit_exp);
		NPU_DRV_WARN("HWTS_GLB_CTRL1.wait_task_limit_exp = 0x%x\n",
			glb_ctrl1.reg.wait_task_limit_exp);
	}

	read_uint64(l1_normal_ns_interrupt.value,
		SOC_NPU_HWTS_HWTS_L1_NORMAL_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_normal_ns_interrupt = l1_normal_ns_interrupt.value;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_L1_NORMAL_NS_INTERRUPT.value = 0x%016llx\n",
			l1_normal_ns_interrupt.value);

	read_uint64(l1_debug_ns_interrupt.value,
		SOC_NPU_HWTS_HWTS_L1_DEBUG_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_debug_ns_interrupt = l1_debug_ns_interrupt.value;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_L1_DEBUG_NS_INTERRUPT.value = 0x%016llx\n",
			l1_debug_ns_interrupt.value);

	read_uint64(l1_error_ns_interrupt.value,
		SOC_NPU_HWTS_HWTS_L1_ERROR_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_error_ns_interrupt = l1_error_ns_interrupt.value;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_L1_ERROR_NS_INTERRUPT.value = 0x%016llx\n",
			l1_error_ns_interrupt.value);

	read_uint64(l1_dfx_interrupt.value,
		SOC_NPU_HWTS_HWTS_L1_DFX_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_dfx_interrupt = l1_dfx_interrupt.value;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_L1_DFX_INTERRUPT.value = 0x%016llx\n",
			l1_dfx_interrupt.value);

	return 0;
}

int npu_hwts_query_sq_info(
	u16 channel_id, struct sq_exception_info *sq_info, u8 do_print)
{
	SOC_NPU_HWTS_HWTS_SQ_DB_UNION sq_db = {0};
	SOC_NPU_HWTS_HWTS_SQ_CFG0_UNION sq_cfg0 = {0};
	SOC_NPU_HWTS_SQCQ_FSM_STATE_UNION sqcq_fsm_state = {0};
	SOC_NPU_HWTS_SQCQ_FSM_AIC_OWN_STATE_UNION sqcq_fsm_aic_own_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	COND_RETURN_ERROR(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	COND_RETURN_ERROR(sq_info == NULL, -EINVAL, "interrupt_info is NULL\n");

	read_uint64(sq_db.value,
		SOC_NPU_HWTS_HWTS_SQ_DB_ADDR(hwts_base, channel_id));
	sq_info->sq_tail = sq_db.reg.sq_tail;
	if (do_print != 0)
		NPU_DRV_WARN("HWTS_SQ_DB.sq_tail = 0x%04x\n", sq_db.reg.sq_tail);

	read_uint64(sq_cfg0.value,
		SOC_NPU_HWTS_HWTS_SQ_CFG0_ADDR(hwts_base, channel_id));
	sq_info->sq_head = sq_cfg0.reg.sq_head;
	sq_info->sq_length = sq_cfg0.reg.sq_length;
	if (do_print != 0) {
		NPU_DRV_WARN("HWTS_SQ_CFG0.sq_head = 0x%04x\n", sq_cfg0.reg.sq_head);
		NPU_DRV_WARN("HWTS_SQ_CFG0.sq_length = 0x%04x\n", sq_cfg0.reg.sq_length);
	}

	read_uint64(sqcq_fsm_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_STATE_ADDR(hwts_base, channel_id));
	sq_info->sqcq_fsm_state = sqcq_fsm_state.value;
	if (do_print != 0)
		NPU_DRV_WARN(
			"SQCQ_FSM_STATE.sqcq_fsm_state = 0x%016llx\n", sqcq_fsm_state.value);

	read_uint64(sqcq_fsm_aic_own_state.value,
		SOC_NPU_HWTS_SQCQ_FSM_AIC_OWN_STATE_ADDR(hwts_base, channel_id));
	sq_info->aic_own_bitmap = sqcq_fsm_aic_own_state.reg.aic_own_bitmap;
	if (do_print != 0)
		NPU_DRV_WARN("SQCQ_FSM_AIC_OWN_STATE.aic_own_bitmap = 0x%x\n",
			sqcq_fsm_aic_own_state.reg.aic_own_bitmap);

	return 0;
}
