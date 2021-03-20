/*
 * hi6405_switch_widget.h -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef _HI6405_SWITCH_WIDGET_H_
#define _HI6405_SWITCH_WIDGET_H_

#include <sound/soc.h>

int hi6405_add_switch_widgets(struct snd_soc_codec *codec, bool single_kcontrol);
int audioup_2mic_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);
int audioup_4mic_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);
int s2up_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);
int iv_2pa_switch_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);
int ec_switch_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);

#endif

