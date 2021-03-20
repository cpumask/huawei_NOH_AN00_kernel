/*
 * hi64xx_mbhc_rear_jack.h
 *
 * hi64xx mbhc rear jack driver
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _HI64XX_MBHC_REAR_JACK_H_
#define _HI64XX_MBHC_REAR_JACK_H_

#include <sound/soc.h>

void rear_jack_init(struct snd_soc_codec *codec);
void hi64xx_set_head_mic_switch_gpio_value(int value);
void hi64xx_set_headphone_switch_gpio_value(int value);
void hi64xx_rear_jack_plug_in_detect_wapper(void);

#endif
