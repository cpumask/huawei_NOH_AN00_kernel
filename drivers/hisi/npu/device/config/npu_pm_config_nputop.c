/*
 * npu_pm_config_nputop.c
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

#include "npu_common.h"
#include <linux/hisi/hisi_npu_pm.h>

int npu_top_powerup(int work_mode, uint32_t subip_set, void **para)
{
	UNUSED(para);
	UNUSED(work_mode);
	UNUSED(subip_set);
	return 0; /* hisi_npu_power_on() call in npu_npusubsys_powerup */
}

int npu_top_powerdown(int work_mode, uint32_t subip_set, void *para)
{
	UNUSED(para);
	UNUSED(work_mode);
	UNUSED(subip_set);
	return hisi_npu_power_off();
}
