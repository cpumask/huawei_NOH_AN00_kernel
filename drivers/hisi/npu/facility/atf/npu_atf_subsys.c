/*
 * npu_atf_subsys.c
 *
 * about npu atf subsys
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
#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/hisi/hisi_npu_pm.h>
#include "npu_atf_subsys.h"
#include "npu_log.h"

noinline int atfd_hisi_service_npu_smc(u64 _service_id, u64 _cmd, u64 _arg1, u64 _arg2)
{
	register u64 service_id asm("x0") = _service_id;
	register u64 cmd asm("x1") = _cmd;
	register u64 arg1 asm("x2") = _arg1;
	register u64 arg2 asm("x3") = _arg2;
	asm volatile(
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")
		"smc    #0\n"
		: "+r" (service_id)
		: "r" (cmd), "r" (arg1), "r" (arg2));

	return (int)service_id;
}

int npuatf_change_slv_secmod(u64 cmd)
{
	int ret;
	NPU_DRV_DEBUG("start npuatf_change_slv_secmod\n");
	ret = atfd_hisi_service_npu_smc(NPU_SLV_SECMODE, cmd, 0, 0);
	return ret;
}

int npuatf_change_mst_secmod(u64 cmd)
{
	(void)atfd_hisi_service_npu_smc(NPU_MST_SECMODE, cmd, 0, 0);
	return 0;
}

int npuatf_start_secmod(u64 is_secure, u64 canary)
{
	int ret;
	ret = atfd_hisi_service_npu_smc(NPU_START_SECMODE, 0, is_secure, canary);
	return ret;
}

int npuatf_enable_secmod(u64 secure_mode)
{
	int ret;
	NPU_DRV_DEBUG("hisi_npu_power_on start\n");
	NPU_DRV_BOOT_TIME_TAG("start hisi_npu_power_on \n");  // based on hisi sw interface for npu subsys/npu bus poweron

	ret = hisi_npu_power_on();
	if (ret) {
		NPU_DRV_ERR("hisi_npu_power_on failed ,ret = %d \n", ret);
		return -1;
	}

	NPU_DRV_BOOT_TIME_TAG("start atfd_hisi_service_npu_smc \n");
	ret = atfd_hisi_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode, NPUIP_NPU_SUBSYS, NPU_FLAGS_POWER_ON);
	if (ret) {
		NPU_DRV_ERR("NPU_ENABLE_SECMODE failed ,ret = %d \n", ret);
		return -1;
	}

	return ret;
}

int npuatf_powerup_aicore(u64 secure_mode, u32 aic_flag)
{
	int ret;

	NPU_DRV_DEBUG("hisi_npu_power_on aicore start\n");
	NPU_DRV_BOOT_TIME_TAG("start atfd_hisi_service_npu_smc \n");
	ret = atfd_hisi_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode, NPUIP_AICORE, aic_flag);
	if (ret) {
		NPU_DRV_ERR("NPU_ENABLE_SECMODE failed ,ret = %d \n", ret);
		return -1;
	}

	return ret;
}

int devdrv_init_sdma_tbu(u64 secure_mode, u32 flag)
{
	int ret;

	NPU_DRV_BOOT_TIME_TAG("npu init sdma tbu start");
	ret = atfd_hisi_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode, NPUIP_SDMA, flag);
	if (ret) {
		NPU_DRV_ERR("NPU_ENABLE_SECMODE init sdma failed ,ret = %d \n", ret);
		return -1;
	}

	return ret;
}

int npuatf_power_down_ts_secreg(u32 is_secure)
{
	int ret;
	ret = atfd_hisi_service_npu_smc(NPU_POWER_DOWN_TS_SEC_REG, is_secure, 0, 0);
	return ret;
}

int npuatf_power_down(u32 mode)
{
	int ret;
	// power down npu cpu
	ret = atfd_hisi_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, mode, NPUIP_NPU_SUBSYS, NPU_FLAGS_POWER_OFF);
	if (ret != 0)
		NPU_DRV_ERR("npu_smc or power_off fail NPU_CPU_POWER_DOWN_SECMODE ,ret: %d\n", ret);

	return ret;
}
int npuatf_power_down_aicore(u64 secure_mode, u32 aic_flag)
{
	int ret;

	NPU_DRV_DEBUG("hisi_npu_power_down aicore start\n");
	ret = atfd_hisi_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, secure_mode, NPUIP_AICORE, aic_flag);
	if (ret) {
		NPU_DRV_ERR("NPU_CPU_POWER_DOWN_SECMODE failed ,ret = %d \n", ret);
		return -1;
	}

	return ret;
}

int npuatf_power_down_tcu()
{
	int ret;

	NPU_DRV_INFO("hisi_npu_power_down tcu start\n");
	ret = atfd_hisi_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, 0, NPUIP_TCU, 0);
	if (ret) {
		NPU_DRV_ERR("power down tcu failed ,ret = %d \n", ret);
		return -1;
	}

	return ret;
}

int atf_query_gic0_state(u64 cmd)
{
	int ret;
	ret = atfd_hisi_service_npu_smc(GIC_CFG_CHECK_SECMODE, cmd, 0, 0);
	return ret;
}

int acpu_gic0_online_ready(u64 cmd)
{
	int ret;
	ret = atfd_hisi_service_npu_smc(GIC_ONLINE_READY_SECMODE, cmd, 0, 0);
	return ret;
}

int npuatf_enable_tbu(u64 cmd)
{
	int ret;
	NPU_DRV_DEBUG("npuatf_enable_tbu start\n");

	ret = atfd_hisi_service_npu_smc(NPU_POWER_UP_SMMU_TBU, cmd, 0, 0);
	if (ret) {
		NPU_DRV_ERR("npuatf_enable_tbu failed ,ret = %d \n", ret);
		return -1;
	}
	return ret;
}

int npuatf_disable_tbu(u64 cmd, u32 aic_flag)
{
	int ret;
	NPU_DRV_DEBUG("npuatf_disable_tbu start\n");

	ret = atfd_hisi_service_npu_smc(NPU_POWER_DOWN_SMMU_TBU, cmd, 0, aic_flag);
	if (ret) {
		NPU_DRV_ERR("atfd_hisi_service_npu_smc failed ,ret = %d \n", ret);
		return -1;
	}
	return ret;
}

int npuatf_switch_hwts_aicore_pool(u64 aic_pool_switch_info)
{
	int ret;
	NPU_DRV_DEBUG("npuatf_switch_hwts_aicore_pool start\n");
	ret = atfd_hisi_service_npu_smc(NPU_SWITCH_HWTS_AICORE_POOL,
		0, NPUIP_NONE, aic_pool_switch_info);
	if (ret) {
		NPU_DRV_ERR("npu atf switch hwts aicore pool failed, ret = %d \n", ret);
		return -1;
	}
	return ret;
}

int npuatf_lowpower_ops(u64 lowpower_dev, u64 lowpower_flag)
{
	int ret;
	ret = atfd_hisi_service_npu_smc(NPU_LOWERPOWER_OPS, 0,
			lowpower_dev, lowpower_flag);
	if (ret != 0) {
			NPU_DRV_ERR("atfd_hisi_service_npu_smc failed ,ret = %d \n", ret);
			return -1;
		}
	return ret;
}
