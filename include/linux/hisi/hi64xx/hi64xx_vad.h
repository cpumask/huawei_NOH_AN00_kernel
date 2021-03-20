/*
 * hi64xx_vad.h
 *
 * hi64xx_vad codec driver
 *
 * Copyright (c) 2014-2020 Huawei Technologies CO., Ltd.
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

#ifndef __HI64XX_VAD_H__
#define __HI64XX_VAD_H__

#include <sound/soc.h>
#include <linux/hisi/hi64xx/hi64xx_irq.h>

int hi64xx_fast_mode_set(bool enable);
int hi64xx_vad_init(struct snd_soc_codec *codec, struct hi64xx_irq *irq);
int hi64xx_vad_deinit(struct device_node *node);

#endif /* __HI64XX_VAD_H__ */

