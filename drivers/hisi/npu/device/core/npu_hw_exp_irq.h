/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Macro
 */
#ifndef __NPU_HW_EXP_IRQ_H
#define __NPU_HW_EXP_IRQ_H


#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_0        67
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_1        (DEVDRV_NPU2ACPU_HW_EXP_IRQ_0 + 1)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_2        (DEVDRV_NPU2ACPU_HW_EXP_IRQ_0 + 2)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_3        (DEVDRV_NPU2ACPU_HW_EXP_IRQ_0 + 3)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0     (DEVDRV_NPU2ACPU_HW_EXP_IRQ_0 + 4)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_1     (DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0 + 1)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_2     (DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0 + 2)
#define DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_3     (DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0 + 3)

void devdrv_clr_hw_exp_irq_int(u64 reg_base, u32 gic_irq);

#endif