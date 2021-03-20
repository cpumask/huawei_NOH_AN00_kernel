/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description:
 * Author:
 * Create: 2018-12-22
 */
#include <linux/idr.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <asm/uaccess.h>
#include <linux/gfp.h>

#include "npu_hw_exp_irq.h"
#include "soc_npu_hw_exp_irq_interface.h"
#include "npu_log.h"
#include "npu_platform.h"


// get hw irq offset according the receving irq num
#define DEVDRV_SHIFT_LEFT_VAL(irq_offset)   (0x1 << (irq_offset))

#define read_uint32(uwValue, Addr)          ((uwValue) = *((volatile u32 *)((uintptr_t)(Addr))))
#define write_uint32(uwValue, Addr)         (*((volatile u32 *)((uintptr_t)(Addr))) = (uwValue))
static inline void update_reg32(u64 addr, u32 value, u32 mask)
{
	u32 reg_val;

	read_uint32(reg_val, addr);
	reg_val = (reg_val & (~mask)) | (value & mask);
	write_uint32(reg_val, addr);
}

static inline int devdrv_parse_gic_irq(u32 gic_irq)
{
	COND_RETURN_ERROR((gic_irq < DEVDRV_NPU2ACPU_HW_EXP_IRQ_0) || (gic_irq > DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_3),
		-1, "devdrv_plat_get_info is NULL\n");

	return (gic_irq >= DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0) ?
		gic_irq - DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0 : gic_irq - DEVDRV_NPU2ACPU_HW_EXP_IRQ_0;
}

void devdrv_clr_hw_exp_irq_int(u64 reg_base, u32 gic_irq)
{
	u32 setval;
	int irq_offset = devdrv_parse_gic_irq(gic_irq);

	if (irq_offset == -1) {
		NPU_DRV_ERR("invalide gic_irq:%d\n", gic_irq);
		return;
	}
	if (reg_base == 0) {
		NPU_DRV_ERR("invalide hwirq reg base\n");
		return;
	}

	setval = DEVDRV_SHIFT_LEFT_VAL((u32)irq_offset);
	update_reg32(SOC_NPU_HW_EXP_IRQ_CLR_ADDR(reg_base), setval, setval);
	return;
}
EXPORT_SYMBOL(devdrv_clr_hw_exp_irq_int);
