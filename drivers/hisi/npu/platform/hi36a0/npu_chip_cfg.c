/*
 * npu_chip_cfg.c
 *
 * about chip config
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
 */

#include <linux/io.h>

#include "npu_log.h"
#include "npu_common.h"
#include "npu_platform.h"
#include "npu_atf_subsys.h"
#include "npu_adapter.h"

#define BALTIMORE_CS_LIMITED_SPEC_VALUE    6

static u32 s_aicore_disable_map = UINT8_MAX;
static u32 s_platform_specification = UINT8_MAX;

static int npu_plat_get_chip_cfg()
{
	int ret = 0;
	struct npu_chip_cfg *chip_cfg = NULL;
	struct devdrv_platform_info *plat_info = NULL;
	struct devdrv_mem_desc *chip_cfg_mem = NULL;

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info error\n");
		return -1;
	}

	chip_cfg_mem = DEVDRV_PLAT_GET_CHIP_CFG_BUF(plat_info);
	NPU_DRV_DEBUG("chip_cfg_mem.base = 0x%x, chip_cfg_mem.len = 0x%x\n",
		chip_cfg_mem->base, chip_cfg_mem->len);

	chip_cfg = (struct npu_chip_cfg *)ioremap_wc(chip_cfg_mem->base, chip_cfg_mem->len);
	COND_RETURN_ERROR(chip_cfg == NULL, -EINVAL,
		"ioremap chip_cfg error, size:0x%x\n", chip_cfg_mem->len);
	COND_GOTO_ERROR(chip_cfg->valid_magic != NPU_DDR_CONFIG_VALID_MAGIC, COMPLETE, ret, -EINVAL,
		"va_npu_config valid_magic:0x%x is not valid\n",
		chip_cfg->valid_magic);
	s_aicore_disable_map = chip_cfg->aicore_disable_bitmap;
	s_platform_specification = chip_cfg->platform_specification;
	NPU_DRV_WARN("s_aicore_disable_map : %u s_platform_specification : %u\n",
		s_aicore_disable_map, s_platform_specification);

COMPLETE:
	iounmap((void *)chip_cfg);
	return ret;
}

/*
 * return value : 1 disable core; 0 not disable core
 */
int npu_plat_aicore_get_disable_status(u32 core_id)
{
	int ret;
	int aicore_disable;

	if (s_aicore_disable_map == UINT8_MAX) {
		ret = npu_plat_get_chip_cfg();
		if (ret != 0)
			return 0;
	}

	aicore_disable = BITMAP_GET(s_aicore_disable_map, core_id);

	return aicore_disable;
}

/*
 * return value: 1 limited chip, 0 full specification chip
 */
static bool npu_plat_is_limited_chip(void)
{
	int ret;
	if (s_platform_specification == UINT8_MAX) {
		ret = npu_plat_get_chip_cfg();
		if (ret != 0)
			return false;
	}

	if (s_platform_specification == (u32)BALTIMORE_CS_LIMITED_SPEC_VALUE)
		return true;

	return false;
}

/*
 * return value : 1 support ispnn; 0 not support ispnn
 */
bool npu_plat_is_support_ispnn(void)
{
	return !npu_plat_is_limited_chip();
}

/*
 * return value : 1 support sc; 0 not support sc
 */
bool npu_plat_is_support_sc(void)
{
	return !npu_plat_is_limited_chip();
}
