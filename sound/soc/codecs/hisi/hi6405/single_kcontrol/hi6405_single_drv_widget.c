/*
 * hi6405_single_drv_widget.c -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include "hi6405_single_drv_widget.h"

#include <sound/core.h>

#include "linux/hisi/audio_log.h"
#include "linux/hisi/hi64xx/hi6405.h"
#include "linux/hisi/hi64xx/hi6405_regs.h"
#include "linux/hisi/hi64xx/hi64xx_utils.h"
#include "linux/hisi/hi64xx/hi6405_type.h"
#include "hi6405_switch_widget.h"
#include "hi6405_switch_widget_utils.h"


static void audio_play_drv_enable(struct snd_soc_codec *codec)
{
	AUDIO_LOGD("begin");

	hi64xx_update_bits(codec, VIRTUAL_DOWN_REG,
		BIT(PLAY48K_BIT), BIT(PLAY48K_BIT));
	play_config_power_event(SAMPLE_RATE_INDEX_48K, codec, SND_SOC_DAPM_PRE_PMU);
	AUDIO_LOGI("AUDIO PLAY DRV Open!");

	AUDIO_LOGD("end");
}

void audio_play_drv_disable(struct snd_soc_codec *codec)
{
	AUDIO_LOGD("begin");

	hi64xx_update_bits(codec, VIRTUAL_DOWN_REG, BIT(PLAY48K_BIT), 0x0);
	play_config_power_event(SAMPLE_RATE_INDEX_48K, codec, SND_SOC_DAPM_POST_PMD);
	AUDIO_LOGI("AUDIO PLAY DRV Close!");

	AUDIO_LOGD("end");
}

static int audio_play_drv_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = snd_soc_dapm_to_codec(w->dapm);

	AUDIO_LOGD("begin");

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		audio_play_drv_enable(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
		audio_play_drv_disable(codec);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	AUDIO_LOGD("end");
	return 0;
}

static int audio_capture_drv_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("power event: %d", event);
	return audioup_4mic_power_event(w, kcontrol, event);
}

static int ec_drv_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = snd_soc_dapm_to_codec(w->dapm);

	AUDIO_LOGD("begin");

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		/* "U7_EC_SWITC" */
		ec_switch_enable(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
		/* "U7_EC_SWITC" */
		ec_switch_disable(codec);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	AUDIO_LOGD("end");

	return 0;
}

#define DRV_WIDGET \
	SND_SOC_DAPM_OUT_DRV_E("AUDIO_PLAY_DRV", \
		SND_SOC_NOPM, 0, 0, NULL, 0, \
		audio_play_drv_power_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_OUT_DRV_E("AUDIO_CAPTURE_DRV", \
		SND_SOC_NOPM, 0, 0, NULL, 0, \
		audio_capture_drv_power_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_OUT_DRV_E("EC_DRV", \
		SND_SOC_NOPM, 0, 0, NULL, 0, \
		ec_drv_power_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \

static const struct snd_soc_dapm_widget drv_widgets[] = {
	DRV_WIDGET
};

int hi6405_add_single_drv_widgets(struct snd_soc_codec *codec)
{
	struct snd_soc_dapm_context *dapm = NULL;

	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return -EINVAL;
	}

	dapm = snd_soc_codec_get_dapm(codec);
	return snd_soc_dapm_new_controls(dapm, drv_widgets,
		ARRAY_SIZE(drv_widgets));
}

