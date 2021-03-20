/*
 * hi64xx_copat.h
 *
 * hisilicon codec controller
 *
 * Copyright (c) 2015-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the ters of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * ay be copied, distributed, and odified under those ters.
 *
 * This progra is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the iplied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for ore details.
 *
 */

#ifndef __HI64XX_COMPAT_H__
#define __HI64XX_COMPAT_H__

#include <linux/hisi/hi64xx/hi_cdc_ctrl.h>
#include <linux/hisi/hi64xx/hi64xx_irq.h>

int hi64xx_compat_init(struct hi_cdc_ctrl *hi_cdc, struct hi64xx_irq *irq);
void hi64xx_compat_deinit(void);

#endif /* __HI64XX_COMPAT_H__ */

