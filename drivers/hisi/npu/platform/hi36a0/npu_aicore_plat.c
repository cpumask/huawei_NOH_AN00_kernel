/*
 * npu_aicore_plat.c
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
#include "npu_aicore_plat.h"

u64 npu_aicore_get_base_addr(u8 aic_id)
{
	u64 aicore_base;
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();

	COND_RETURN_ERROR(plat_info == NULL, 0ULL, "devdrv_plat_get_info failed\n");
	if (aic_id == 0)
		aicore_base = (u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(
			plat_info, DEVDRV_REG_AIC0_BASE);
	else
		aicore_base = (u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(
			plat_info, DEVDRV_REG_AIC1_BASE);
	NPU_DRV_DEBUG("aicore%u_base = 0x%016llx\n", aic_id, aicore_base);
	return aicore_base;
}

int npu_aicore_dbg_busy_wait(u64 addr, u64 *val)
{
	u32 busy_wait_loop = 0;

	while (busy_wait_loop < NPU_AICORE_DBG_WAIT_LOOP_MAX) {
		read_uint64(*val, addr);
		if (*val != 0)
			break;
		busy_wait_loop++;
	}

	if (*val == 0 && busy_wait_loop >= NPU_AICORE_DBG_WAIT_LOOP_MAX) {
		NPU_DRV_WARN("busy wait timed out\n");
		return -1;
	}

	NPU_DRV_DEBUG("busy wait successfully\n");
	return 0;
}

int npu_aicore_read_dbg_register(u8 aic_id, u64 debug_addr_id, u64 *read_buf)
{
	int ret;
	SOC_NPU_AICORE_CLK_GATE_MASK_UNION clk_gate_mask = {0};
	SOC_NPU_AICORE_DBG_ADDR_UNION dbg_addr = {0};
	SOC_NPU_AICORE_DBG_RW_CTRL_UNION dbg_rw_ctrl = {0};
	SOC_NPU_AICORE_DBG_STATUS_UNION dbg_status = {0};
	SOC_NPU_AICORE_DBG_DATA0_UNION dbg_data0 = {0};
	u64 base = npu_aicore_get_base_addr(aic_id);

	COND_RETURN_ERROR(base == 0ULL, -1, "aicore base is NULL\n");
	NPU_DRV_DEBUG("base = 0x%016llx\n", base);

	/* step 1 */
	read_uint64(clk_gate_mask.value, SOC_NPU_AICORE_CLK_GATE_MASK_ADDR(base));
	clk_gate_mask.reg.debug_mode_en = 0x1;
	write_uint64(clk_gate_mask.value, SOC_NPU_AICORE_CLK_GATE_MASK_ADDR(base));
	NPU_DRV_DEBUG("clk_gate_mask.value = 0x%016llx\n", clk_gate_mask.value);

	/* step 2 */
	dbg_addr.reg.dfx_en = 0x0;
	dbg_addr.reg.dbg_sel = 0x0;
	dbg_addr.reg.dbg_addr = debug_addr_id;
	write_uint64(dbg_addr.value, SOC_NPU_AICORE_DBG_ADDR_ADDR(base));
	NPU_DRV_DEBUG("dbg_addr.value = 0x%016llx\n", dbg_addr.value);

	/* step 3 */
	dbg_rw_ctrl.reg.dbg_ctrl_rd = 0x1;
	write_uint64(dbg_rw_ctrl.value, SOC_NPU_AICORE_DBG_RW_CTRL_ADDR(base));
	NPU_DRV_DEBUG("dbg_rw_ctrl.value = 0x%016llx\n", dbg_rw_ctrl.value);

	/* step 4 */
	ret = npu_aicore_dbg_busy_wait(
		SOC_NPU_AICORE_DBG_RW_CTRL_ADDR(base), &dbg_rw_ctrl.value);

	/* step 5 */
	clk_gate_mask.reg.debug_mode_en = 0x0;
	write_uint64(clk_gate_mask.value, SOC_NPU_AICORE_CLK_GATE_MASK_ADDR(base));
	NPU_DRV_DEBUG("clk_gate_mask.value = 0x%016llx\n", clk_gate_mask.value);

	/* step 6 */
	if (ret != 0) {
		read_uint64(dbg_status.value, SOC_NPU_AICORE_DBG_STATUS_ADDR(base));
		if (dbg_status.reg.dbg_read_err == 0x1)
			NPU_DRV_WARN("register read error\n");
		else
			NPU_DRV_WARN("read register timed out\n");
		return -1;
	}

	/* step 7 */
	read_uint64(dbg_data0.value, SOC_NPU_AICORE_DBG_DATA0_ADDR(base));
	*read_buf = dbg_data0.reg.dbg_data0;
	NPU_DRV_DEBUG("*read_buf = 0x%016llx\n", *read_buf);

	return 0;
}

int npu_aicore_query_exception_dbg_info(
	u8 aic_id, struct aicore_exception_dbg_info *aic_info, u8 do_print)
{
	u64 read_buf = 0;
	int ret;
	u32 reg_id;
	const u32 x_reg_base = 64;

	COND_RETURN_ERROR(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	/* ifu current */
	ret = npu_aicore_read_dbg_register(aic_id, 0x0, &read_buf);
	if (ret == 0) {
		aic_info->ifu_current = read_buf;
		if (do_print != 0)
			NPU_DRV_WARN("ifu current is at 0x%016llx\n", read_buf);
	}

	/* x0-x31 */
	for (reg_id = 0; reg_id < 32; reg_id++) {
		ret = npu_aicore_read_dbg_register(
			aic_id, x_reg_base + reg_id, &read_buf);
		if (ret == 0) {
			aic_info->general_register[reg_id] = read_buf;
			if (do_print != 0)
				NPU_DRV_WARN("general purpose register x%u = 0x%016llx\n",
					reg_id, read_buf);
		}
	}

	return 0;
}

int npu_aicore_query_exception_info(
	u8 aic_id, struct aicore_exception_info *aic_info, u8 do_print)
{
	SOC_NPU_AICORE_PC_START_UNION ifu_start = {0};
	SOC_NPU_AICORE_AIC_ERROR_UNION aic_error = {0};
	SOC_NPU_AICORE_AIC_ERROR1_UNION aic_error1 = {0};
	SOC_NPU_AICORE_AIC_ERROR2_UNION aic_error2 = {0};
	SOC_NPU_AICORE_AIC_ERROR_MASK_UNION aic_error_mask = {0};
	SOC_NPU_AICORE_AI_CORE_INT_UNION aic_core_int = {0};
	u64 base = npu_aicore_get_base_addr(aic_id);

	COND_RETURN_ERROR(base == 0ULL, -EINVAL, "aicore base is NULL\n");
	COND_RETURN_ERROR(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	read_uint64(ifu_start.value, SOC_NPU_AICORE_PC_START_ADDR(base));
	aic_info->ifu_start = ifu_start.reg.va;
	if (do_print != 0)
		NPU_DRV_WARN("ifu start = %016llx\n", ifu_start.reg.va);

	read_uint64(aic_error.value, SOC_NPU_AICORE_AIC_ERROR_ADDR(base));
	aic_info->aic_error = aic_error.value;
	if (do_print != 0)
		NPU_DRV_WARN("AIC_ERROR.value = 0x%016llx\n", aic_error.value);

	read_uint64(aic_error1.value, SOC_NPU_AICORE_AIC_ERROR1_ADDR(base));
	aic_info->aic_error1 = aic_error1.value;
	if (do_print != 0)
		NPU_DRV_WARN("AIC_ERROR1.value = 0x%016llx\n", aic_error1.value);

	read_uint64(aic_error2.value, SOC_NPU_AICORE_AIC_ERROR2_ADDR(base));
	aic_info->aic_error2 = aic_error2.value;
	if (do_print != 0)
		NPU_DRV_WARN("AIC_ERROR2.value = 0x%016llx\n", aic_error2.value);

	read_uint64(aic_error_mask.value, SOC_NPU_AICORE_AIC_ERROR_MASK_ADDR(base));
	aic_info->aic_error_mask = aic_error_mask.value;
	if (do_print != 0)
		NPU_DRV_WARN(
			"AIC_ERROR_MASK.value = 0x%016llx\n", aic_error_mask.value);

	read_uint64(aic_core_int.value, SOC_NPU_AICORE_AI_CORE_INT_ADDR(base));
	aic_info->aic_core_int = aic_core_int.value;
	if (do_print != 0)
		NPU_DRV_WARN("AI_CORE_INT.value = 0x%016llx\n", aic_core_int.value);

	return 0;
}

int npu_aicore_query_exception_error_info(
	u8 aic_id, struct aicore_exception_error_info *aic_info, u8 do_print)
{
	SOC_NPU_AICORE_BIU_ERR_INFO_UNION biu_err_info = {0};
	SOC_NPU_AICORE_CCU_ERR_INFO_UNION ccu_err_info = {0};
	SOC_NPU_AICORE_CUBE_ERR_INFO_UNION cube_err_info = {0};
	SOC_NPU_AICORE_MTE_ERR_INFO_UNION mte_err_info = {0};
	SOC_NPU_AICORE_VEC_ERR_INFO_UNION vec_err_info = {0};
	SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_UNION mte_ecc_1bit_err = {0};
	SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_UNION vec_cube_ecc_1bit_err = {0};
	u64 base = npu_aicore_get_base_addr(aic_id);

	COND_RETURN_ERROR(base == 0ULL, -EINVAL, "aicore base is NULL\n");
	COND_RETURN_ERROR(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	read_uint64(biu_err_info.value, SOC_NPU_AICORE_BIU_ERR_INFO_ADDR(base));
	aic_info->biu_err_info = biu_err_info.value;
	if (do_print != 0)
		NPU_DRV_WARN("BIU_ERR_INFO.value = 0x%016llx\n", biu_err_info.value);

	read_uint64(ccu_err_info.value, SOC_NPU_AICORE_CCU_ERR_INFO_ADDR(base));
	aic_info->ccu_err_info = ccu_err_info.value;
	if (do_print != 0)
		NPU_DRV_WARN("CCU_ERR_INFO.value = 0x%016llx\n", ccu_err_info.value);

	read_uint64(cube_err_info.value, SOC_NPU_AICORE_CUBE_ERR_INFO_ADDR(base));
	aic_info->cube_err_info = cube_err_info.value;
	if (do_print != 0)
		NPU_DRV_WARN("CUBE_ERR_INFO.value = 0x%016llx\n", cube_err_info.value);

	read_uint64(mte_err_info.value, SOC_NPU_AICORE_MTE_ERR_INFO_ADDR(base));
	aic_info->mte_err_info = mte_err_info.value;
	if (do_print != 0)
		NPU_DRV_WARN("MTE_ERR_INFO.value = 0x%016llx\n", mte_err_info.value);

	read_uint64(vec_err_info.value, SOC_NPU_AICORE_VEC_ERR_INFO_ADDR(base));
	aic_info->vec_err_info = vec_err_info.value;
	if (do_print != 0)
		NPU_DRV_WARN("VEC_ERR_INFO.value = 0x%016llx\n", vec_err_info.value);

	read_uint64(
		mte_ecc_1bit_err.value, SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ADDR(base));
	aic_info->mte_ecc_1bit_err = mte_ecc_1bit_err.value;
	if (do_print != 0)
		NPU_DRV_WARN(
			"MTE_ECC_1BIT_ERR.value = 0x%016llx\n", mte_ecc_1bit_err.value);

	read_uint64(vec_cube_ecc_1bit_err.value,
		SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ADDR(base));
	aic_info->vec_cube_ecc_1bit_err = vec_cube_ecc_1bit_err.value;
	if (do_print != 0)
		NPU_DRV_WARN("VEC_CUBE_ECC_1BIT_ERR.value = 0x%016llx\n",
			vec_cube_ecc_1bit_err.value);

	return 0;
}