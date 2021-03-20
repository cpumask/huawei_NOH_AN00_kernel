/*
 * hisi_vcodec_vdec_plat.c
 *
 * This is for vdec platform
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

#include "hisi_vcodec_vdec_plat.h"
#include <linux/sched/clock.h>
#include "hisi_vcodec_vdec_regulator.h"
#include "vfmw_osal.h"

enum {
	PCTRL_PERI_VDEC_IS_FPGA,
	PCTRL_PERI_VDEC_IS_ES,
	PCTRL_PERI_VDEC_STATE,
	PCTRL_PERI_VDEC_MASK,
	PCTRL_PERI_VDEC_VALUE,
	PCTRL_PERI_VDEC_MAX,
};

const char *g_device_state_name[PCTRL_PERI_VDEC_MAX] = {
	"vdec_fpga",
	"vdec_es",
	"pctrl_peri_state",
	"pctrl_peri_vdec_mask",
	"pctrl_peri_vdec_value"
};

hi_s32 vdec_device_probe(void *plt)
{
	hi_s32 i;
	hi_u32 peri_vdec_value;
	hi_s32 ret;
	hi_u32 *pctrl_peri_state_vaddr = HI_NULL;
	hi_u32 state_info[PCTRL_PERI_VDEC_MAX] = {0};
	struct platform_device *plt_dev = (struct platform_device *)plt;
	struct device *dev = &plt_dev->dev;

	for (i = 0; i < PCTRL_PERI_VDEC_MAX; i++) {
		ret = of_property_read_u32(dev->of_node, g_device_state_name[i], &state_info[i]);
		if (ret) {
			dprint(PRN_FATAL, "get %s failed\n", g_device_state_name[i]);
			return HI_FAILURE;
		}
	}

	if (!state_info[PCTRL_PERI_VDEC_IS_FPGA]) {
		dprint(PRN_ALWS, "current is %s not fpga platform\n",
			state_info[PCTRL_PERI_VDEC_IS_ES] ? "es" : "cs");
		return HI_SUCCESS;
	}

	pctrl_peri_state_vaddr = ioremap(state_info[PCTRL_PERI_VDEC_STATE], 4);
	if (!pctrl_peri_state_vaddr) {
		dprint(PRN_FATAL, "ioremap vdec state reg failed\n");
		return HI_FAILURE;
	}

	peri_vdec_value = readl(pctrl_peri_state_vaddr) & state_info[PCTRL_PERI_VDEC_MASK];
	iounmap(pctrl_peri_state_vaddr);

	if (peri_vdec_value == state_info[PCTRL_PERI_VDEC_VALUE]) {
		dprint(PRN_ALWS, "current is %s fpga platform\n",
			state_info[PCTRL_PERI_VDEC_IS_ES] ? "es" : "cs");
		ret = HI_SUCCESS;
	} else {
		dprint(PRN_FATAL, "vdec module is not exist");
		ret = HI_FAILURE;
	}

	return ret;
}

