/*
 * hi64xx_mad.h
 *
 * hi64xx mad driver
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#ifndef __HI64XX_MAD_H__
#define __HI64XX_MAD_H__

#include <linux/hisi/hi64xx/hi64xx_irq.h>

void hi64xx_hook_pcm_handle(void);
int hi64xx_mad_init(struct hi64xx_irq *irq);
void hi64xx_mad_request_irq(void);
void hi64xx_mad_free_irq(void);
void hi64xx_mad_deinit(void);

#endif /* __HI64XX_MAD_H__ */

