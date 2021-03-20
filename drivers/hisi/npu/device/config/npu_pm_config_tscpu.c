/*
 * npu_pm_config_tscpu.c
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
#include "npu_atf_subsys.h"
#include "npu_proc_ctx.h"

int npu_tscpu_powerup(int work_mode, uint32_t subip_set, void **para)
{
	int ret;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	UNUSED(subip_set);
	proc_ctx = (struct devdrv_proc_ctx *)para;

	ret = devdrv_plat_powerup_till_ts(work_mode, 0);
	if (ret)
		return ret;
	npu_proc_clear_sqcq_info(proc_ctx);
	/* sdma init */
	ret = npu_plat_init_sdma(work_mode);
	return ret;
}

int npu_tscpu_powerdown(int work_mode, uint32_t subip_set, void *para)
{
	struct devdrv_proc_ctx *proc_ctx = NULL;
	UNUSED(subip_set);
	proc_ctx = (struct devdrv_proc_ctx *)para;

	(void)devdrv_plat_powerdown_tbu(NPU_FLAGS_DISBALE_SYSDMA);
	npu_proc_clear_sqcq_info(proc_ctx);
	return npu_plat_powerdown_ts(0, work_mode);
}
