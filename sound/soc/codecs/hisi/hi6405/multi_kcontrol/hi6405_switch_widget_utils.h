/*
 * hi6405_switch_widget_utils.h -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef _HI6405_SWITCH_WIDGET_UTILS_H_
#define _HI6405_SWITCH_WIDGET_UTILS_H_

#include <sound/soc.h>

#include "linux/hisi/hi64xx/hi6405_type.h"


int play_config_power_event(unsigned int rate, struct snd_soc_codec *codec, int event);
int audioup_2mic_enable(struct snd_soc_codec *codec);
int audioup_2mic_disable(struct snd_soc_codec *codec);
int audioup_4mic_enable(struct snd_soc_codec *codec);
int audioup_4mic_disable(struct snd_soc_codec *codec);
void ec_switch_enable(struct snd_soc_codec *codec);
void ec_switch_disable(struct snd_soc_codec *codec);
void iv_2pa_switch_enable(struct snd_soc_codec *codec);
void iv_2pa_switch_disable(struct snd_soc_codec *codec);
void hp_high_level_enable(struct snd_soc_codec *codec);
void hp_high_level_disable(struct snd_soc_codec *codec);
void iv_dspif_switch_enable(struct snd_soc_codec *codec);
void iv_dspif_switch_disable(struct snd_soc_codec *codec);
int voice_slimbus_active(struct snd_soc_codec *codec,
	struct hi6405_platform_data *platform_data);
int voice_slimbus_deactive(struct snd_soc_codec *codec,
	struct hi6405_platform_data *platform_data);
void u12_select_pga(struct snd_soc_codec *codec);
void u12_select_dspif(struct snd_soc_codec *codec);
void u56_select_dspif(struct snd_soc_codec *codec);
void u56_select_pga(struct snd_soc_codec *codec);
void u10_select_mic_src(struct snd_soc_codec *codec);
void u10_select_dspif(struct snd_soc_codec *codec);
void up_src_pre_pmu(struct snd_soc_codec *codec, unsigned int channel,
	unsigned int sample_rate, unsigned int src_mode);
void up_src_post_pmu(struct snd_soc_codec *codec, unsigned int channel);
void up_src_pre_pmu_v2(struct snd_soc_codec *codec, unsigned int channel,
	unsigned int sample_rate, unsigned int src_mode);
void up_src_post_pmu_v2(struct snd_soc_codec *codec, unsigned int channel);
void update_callswitch_state(int event, struct hi6405_platform_data *platform_data);
int voice_v2_power_event(struct snd_soc_dapm_widget *w,
	 int event, uint32_t rate);
void u9_select_dspif(struct snd_soc_codec *codec);
void u9_select_pga(struct snd_soc_codec *codec);

#endif

