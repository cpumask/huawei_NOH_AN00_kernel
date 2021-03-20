/*
 * npu_pm_config_smmutbu.c
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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

#include "npu_adapter.h"
#include "npu_platform.h"
#include "npu_proc_ctx.h"

int npu_smmu_tbu_powerup(int work_mode, uint32_t subip_set, void **para)
{
	int ret = 0;
	struct devdrv_platform_info *plat_info = NULL;
	UNUSED(subip_set);
	UNUSED(para);

	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info failed\n");

	if (work_mode != NPU_SEC)
		ret = npu_plat_powerup_smmu(DEVDRV_PLAT_GET_PDEV(plat_info));
	return ret;
}

int npu_smmu_tbu_powerdown(int work_mode, uint32_t subip_set, void *para)
{
	int ret = 0;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	UNUSED(work_mode);
	UNUSED(subip_set);

	proc_ctx = (struct devdrv_proc_ctx *)para;

	if (work_mode != NPU_SEC)
		ret = npu_plat_poweroff_smmu(proc_ctx->devid);
	return ret;
}
