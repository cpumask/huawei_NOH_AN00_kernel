/*
 * hisi_64xx_irq.h
 *
 * Interrupt controller support for Hisilicon HI64XX
 *
 * Copyright (c) 2015-2020 Huawei Technologies CO., Ltd.
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

#ifndef __HI64XX_IRQ_H__
#define __HI64XX_IRQ_H__

#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/hisi/hi64xx/hi_cdc_ctrl.h>

enum hi64xx_irq_type {
	IRQ_BTNUP_COMP1 = 0,
	IRQ_BTNDOWN_COMP1 = 1,
	IRQ_BTNDOWN_COMP2 = 2,
	IRQ_BTNUP_COMP2 = 3,
	IRQ_BTNUP_ECO = 4,
	IRQ_BTNDOWN_ECO = 5,
	IRQ_PLUGIN = 6,
	IRQ_PLUGOUT = 7,
	IRQ_VAD = 12,
	IRQ_WTD = 13,
	IRQ_MAD = 14,
	IRQ_CMD_VALID = 16,
	IRQ_PLL_UNLOCK = 20,
	IRQ_PLL44K1_UNLOCK = 34,
	IRQ_PLLMAD_UNLOCK = 36,
	IRQ_BUNK1_OCP = 40,
	IRQ_BUNK1_SCP = 41,
	IRQ_BUNK2_OCP = 42,
	IRQ_BUNK2_SCP = 43,
	IRQ_CP1_SHORT = 44,
	IRQ_CP2_SHORT = 45,
	IRQ_MAX
};

#define HI64XX_MAX_IRQS_NUM 64
#define HI64XX_IRQ_REG_BITS_NUM 8
#define HI64XX_MAX_IRQ_REGS_NUM (HI64XX_MAX_IRQS_NUM / HI64XX_IRQ_REG_BITS_NUM)

struct hi64xx_irq_map {
	/* irq registers in codec */
	unsigned int irq_regs[HI64XX_MAX_IRQ_REGS_NUM];
	/* irq mask register in codec */
	unsigned int irq_mask_regs[HI64XX_MAX_IRQ_REGS_NUM];
	int irq_num;
};

struct hi64xx_irq {
	struct device *dev;
};

int hi64xx_irq_init_irq(struct hi64xx_irq *irq_data,
	const struct hi64xx_irq_map *irq_map);
void hi64xx_irq_deinit_irq(struct hi64xx_irq *irq_data);
int hi64xx_irq_request_irq(struct hi64xx_irq *irq_data, int phy_irq,
	irq_handler_t handler, const char *name, void *priv);
void hi64xx_irq_free_irq(struct hi64xx_irq *irq_data, int phy_irq, void *priv);
int hi64xx_irq_enable_irq(struct hi64xx_irq *irq_data, int phy_irq);
int hi64xx_irq_disable_irq(struct hi64xx_irq *irq_data, int phy_irq);
int hi64xx_irq_enable_irqs(struct hi64xx_irq *irq_data,
	int irq_num, const int *phy_irqs);
int hi64xx_irq_disable_irqs(struct hi64xx_irq *irq_data,
	int irq_num, const int *phy_irqs);
void hi64xx_irq_resume_wait(struct hi64xx_irq *irq_data);
#endif /* __HI64XX_IRQ_H__ */

