/*
 * npu_reg.c
 *
 * about npu reg
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
#include "npu_reg.h"

#include <linux/io.h>

#include "npu_adapter.h"
#include "npu_log.h"

#define DEVDRV_DUMP_REGION_NAME     "dump_region"

static devdrv_reg_info devdrv_regs[DEVDRV_REG_MAX_REG] = {
	[DEVDRV_REG_POWER_STATUS] = {"DEVDRV_REG_POWER_STATUS", DRV_NPU_POWER_STATUS_REG, NPU_POWER_STATUS_REG_LEN, NULL},
};

struct devdrv_mem_desc* devdrv_plat_get_reg_desc(u32 reg_idx)
{
	struct devdrv_platform_info *plat_info = NULL;

	NPU_DRV_DEBUG("get plat_info reg_idx = %u\n", reg_idx);
	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("get plat_info failed\n");
		return NULL;
	}

	return &DEVDRV_PLAT_GET_REG_DESC(plat_info, reg_idx);
}
EXPORT_SYMBOL(devdrv_plat_get_reg_desc);

u32* devdrv_plat_get_reg_vaddr(u32 reg_idx, u32 offset)
{
	struct devdrv_platform_info *plat_info = NULL;
	if (reg_idx >= DEVDRV_REG_MAX_RESOURCE) {
		NPU_DRV_ERR("reg_idx = %x is error\n", reg_idx);
		return NULL;
	}

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("get plat_info failed\n");
		return NULL;
	}
	return (u32 *)((u8*)DEVDRV_PLAT_GET_REG_VADDR(plat_info, reg_idx) + offset);
}
EXPORT_SYMBOL(devdrv_plat_get_reg_vaddr);

int devdrv_plat_unmap_reg(struct platform_device *pdev, struct devdrv_platform_info *plat_info)
{
	int i;
	int max_regs = DEVDRV_REG_MAX_RESOURCE;

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 0)
		max_regs = DEVDRV_REG_PERICRG_BASE + 1;

	for (i = 0; i < max_regs; i++) {
		if (DEVDRV_PLAT_GET_REG_VADDR(plat_info, i) != NULL) {
			if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
				devm_iounmap(&pdev->dev, DEVDRV_PLAT_GET_REG_VADDR(plat_info, i));
			} else {
				if (i == DEVDRV_REG_TS_SRAM) {
					devdrv_plat_sram_unmap(pdev,
						DEVDRV_PLAT_GET_REG_VADDR(plat_info, i));
				} else {
					devm_iounmap(&pdev->dev,
						DEVDRV_PLAT_GET_REG_VADDR(plat_info, i));
				}
			}
		}
	}
	return 0;
}
EXPORT_SYMBOL(devdrv_plat_unmap_reg);

int devdrv_plat_parse_dump_region_desc(const struct device_node *root, struct devdrv_platform_info *plat_info)
{
	int ret;
	struct devdrv_mem_desc *mem_desc = &DEVDRV_PLAT_GET_DUMP_REGION_DESC(plat_info, 0);
	if (mem_desc == NULL) {
		NPU_DRV_ERR("dump_region desc is NULL");
		return -1;
	}
	ret = of_property_read_u32_array(root, DEVDRV_DUMP_REGION_NAME, (u32 *)mem_desc,
		DEVDRV_DUMP_REGION_MAX * (sizeof(struct devdrv_mem_desc) / sizeof(u32)));
	if (ret < 0) {
		NPU_DRV_ERR("read dump_regs base failed\n");
		return ret;
	}
	NPU_DRV_DEBUG("dump region: base=0x%x len=0x%x", mem_desc->base, mem_desc->len);
	return 0;
}

int devdrv_plat_parse_reg_desc(struct platform_device *pdev, struct devdrv_platform_info *plat_info)
{
	int i;
	void __iomem *base = NULL;
	struct resource *info = NULL;
	struct devdrv_mem_desc* mem_desc = NULL;
	int max_regs = DEVDRV_REG_MAX_RESOURCE;

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 0)
		max_regs = DEVDRV_REG_PERICRG_BASE + 1;
	for (i = 0; i < max_regs; i++) {
		info = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (info == NULL) {
			NPU_DRV_ERR("platform_get_resource failed i = %d \n", i);
			return -1;
		}
		mem_desc = &DEVDRV_PLAT_GET_REG_DESC(plat_info, i);
		mem_desc->base = info->start;
		mem_desc->len = info->end - info->start;

		NPU_DRV_DEBUG("resource: base %pK\n", (void *)(uintptr_t)(u64)mem_desc->base);
		NPU_DRV_WARN("resource: mem_desc.len %x\n", mem_desc->len);

		if (mem_desc->base != 0 && mem_desc->len != 0) {
			if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
				base = devm_ioremap(&pdev->dev, mem_desc->base, mem_desc->len);
			} else {
				if (i == DEVDRV_REG_TS_SRAM) {
					base = devdrv_plat_sram_remap(pdev, mem_desc->base, mem_desc->len);
				} else {
					base = devm_ioremap(&pdev->dev, mem_desc->base, mem_desc->len);
				}
			}

			if (base == NULL) {
				NPU_DRV_ERR("platform_get_resource failed i = %d \n", i);
				goto map_platform_reg_fail;
			}
			DEVDRV_PLAT_GET_REG_VADDR(plat_info, i) = base;
		} else {
			DEVDRV_PLAT_GET_REG_VADDR(plat_info, i) = NULL;
		}
	}

	return 0;
map_platform_reg_fail:
	memset(plat_info->dts_info.reg_desc, 0, sizeof(plat_info->dts_info.reg_desc));
	return devdrv_plat_unmap_reg(pdev, plat_info);
}

int devdrv_plat_ioremap(devdrv_reg_type reg_type)
{
	devdrv_reg_info *reg_info = NULL;
	if (reg_type >= DEVDRV_REG_MAX_REG) {
		NPU_DRV_ERR("invalid reg_type = %d\n", reg_type);
		return -1;
	}

	reg_info = &devdrv_regs[reg_type];
	if (reg_info->base == 0 || reg_info->size == 0) {
		NPU_DRV_ERR("reg_type = %d is unused\n", reg_type);
		return -1;
	}
	if (reg_info->vaddr) {
		return 0;
	}
/*lint -e446 */
	reg_info->vaddr = ioremap_nocache(reg_info->base, reg_info->size);
/*lint +e446 */
	if (reg_info->vaddr == NULL) {
		NPU_DRV_ERR("ioremap failed\n");
		return -1;
	}
	return 0;
}

void devdrv_plat_iounmap(devdrv_reg_type reg_type)
{
	devdrv_reg_info *reg_info = NULL;
	if (reg_type >= DEVDRV_REG_MAX_REG) {
		NPU_DRV_ERR("invalid reg_type = %d\n", reg_type);
		return;
	}

	reg_info = &devdrv_regs[reg_type];
	if (reg_info->vaddr) {
		iounmap(reg_info->vaddr);
		reg_info->vaddr = NULL;
	}
}

void __iomem * devdrv_plat_get_vaddr(devdrv_reg_type reg_type)
{
	if (reg_type >= DEVDRV_REG_MAX_REG) {
		NPU_DRV_ERR("invalid reg_type = %d\n", reg_type);
		return NULL;
	}
	return devdrv_regs[reg_type].vaddr;
}

void devdrv_plat_set_npu_power_status(u32 status)
{
	u32 *addr = (u32 *)devdrv_plat_get_vaddr(DEVDRV_REG_POWER_STATUS);
	if (addr) {
		writel(status, addr);
	} else {
		NPU_DRV_WARN("invalid power status addr\n");
	}
}
