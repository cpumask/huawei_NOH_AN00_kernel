/*
 * hi6405_codec_pm.c
 *
 * hi6405 codec pm process
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
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

#include "hi6405_codec_pm.h"

#include <linux/hisi/audio_log.h>

#include <linux/hisi/hi64xx/hi6405.h>
#include <linux/hisi/hi64xx/hi64xx_mbhc_rear_jack.h>

#include "hi6405_codec_probe.h"
#include "download/download_image.h"

#define CODEC_PM_SWITCHES_MAX_COUNT 10

static void save_switch_value(struct snd_soc_codec *codec)
{
	struct snd_card *card = codec->component.card->snd_card;
	struct snd_kcontrol *kctl = NULL;
	struct snd_ctl_elem_value *ucontrol = NULL;
	struct hi6405_platform_data *priv = snd_soc_codec_get_drvdata(codec);
	int i, result;
	int count = priv->pm.codec_pm_switches_count;

	for (i = 0; i < count; ++i) {
		ucontrol = &priv->pm.codec_pm_switches[i];

		kctl = snd_ctl_find_id(card, &ucontrol->id);
		if (kctl == NULL || kctl->get == NULL) {
			AUDIO_LOGE("can't find %s, or switch callback is NULL", ucontrol->id.name);
			continue;
		}

		result = kctl->get(kctl, ucontrol);
		AUDIO_LOGI("kctl %s get value %ld result is %d", ucontrol->id.name, ucontrol->value.integer.value[0],
			result);
	}
}

static void clear_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol)
{
	int result;
	int value = ucontrol->value.integer.value[0];

	ucontrol->value.integer.value[0] = 0;
	result = kctl->put(kctl, ucontrol);
	AUDIO_LOGI("kctl %s put value %ld result is %d", ucontrol->id.name, ucontrol->value.integer.value[0], result);

	ucontrol->value.integer.value[0] = value;
}

static void set_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol)
{
	int result;
	int value = ucontrol->value.integer.value[0];

	if (value == 0)
		return;

	result = kctl->put(kctl, ucontrol);

	AUDIO_LOGI("kctl %s put value %d result is %d", ucontrol->id.name, value, result);
}

static void set_switch_value(struct snd_soc_codec *codec,
	void (*put)(struct snd_kcontrol *, struct snd_ctl_elem_value *))
{
	struct snd_card *card = codec->component.card->snd_card;
	struct snd_kcontrol *kctl = NULL;
	struct snd_ctl_elem_value *ucontrol = NULL;
	struct hi6405_platform_data *priv = snd_soc_codec_get_drvdata(codec);
	int i;

	int count = priv->pm.codec_pm_switches_count;

	for (i = 0; i < count; ++i) {
		ucontrol = &priv->pm.codec_pm_switches[i];

		kctl = snd_ctl_find_id(card, &ucontrol->id);
		if (kctl == NULL || kctl->put == NULL) {
			AUDIO_LOGE("can not find %s, or switch callback is NULL", ucontrol->id.name);
			continue;
		}

		put(kctl, ucontrol);
	}
}

static bool get_headset_plug_in_status(void)
{
	bool is_plug_in = hi64xx_check_headset_pluged_in();

	AUDIO_LOGI("headset plug in status: %d", is_plug_in);
	return is_plug_in;
}

static void codec_suspend(struct snd_soc_codec *codec)
{
	struct hi6405_platform_data *priv = snd_soc_codec_get_drvdata(codec);
	bool is_plug_in = get_headset_plug_in_status();
	AUDIO_LOGI("begin");

	save_switch_value(codec);

	if (is_plug_in)
		hi64xx_plug_out_wapper(priv->mbhc);

	set_switch_value(codec, clear_kctl_value);
	AUDIO_LOGI("end");
}

static void codec_resume(struct snd_soc_codec *codec)
{
	struct hi6405_platform_data *priv = snd_soc_codec_get_drvdata(codec);

	AUDIO_LOGI("begin");
	set_switch_value(codec, set_kctl_value);

	hi64xx_plug_in_detect_wapper(priv->mbhc);
	hi64xx_rear_jack_plug_in_detect_wapper();

	AUDIO_LOGI("end");
}

int hi6405_codec_suspend(struct device *dev)
{
	struct snd_soc_codec *codec = hi6405_get_codec();
	struct hi6405_platform_data *priv = NULL;
	bool support_irq_pm = hi64xx_support_hs_irq_pm();

	AUDIO_LOGI("begin");

	if (!support_irq_pm) {
		AUDIO_LOGI("not support headset irq pm");
		return 0;
	}
	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return 0;
	}

	priv = snd_soc_codec_get_drvdata(codec);

	codec_suspend(codec);
	hi64xx_disable_hs_irq(priv->irqmgr);

	AUDIO_LOGI("end");
	return 0;
}

int hi6405_codec_resume(struct device *dev)
{
	struct snd_soc_codec *codec = hi6405_get_codec();
	struct hi6405_platform_data *priv = NULL;
	bool support_irq_pm = hi64xx_support_hs_irq_pm();

	AUDIO_LOGI("begin");

	if (!support_irq_pm) {
		AUDIO_LOGI("not support headset irq pm");
		return 0;
	}
	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return 0;
	}

	priv = snd_soc_codec_get_drvdata(codec);

	hi64xx_enable_hs_irq(priv->irqmgr);
	codec_resume(codec);

	AUDIO_LOGI("end");
	return 0;
}

int hi6405_codec_freeze(struct device *dev)
{
	struct hi6405_platform_data *priv = NULL;
	struct snd_soc_codec *codec = hi6405_get_codec();

	AUDIO_LOGI("begin");

	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return 0;
	}

	priv = snd_soc_codec_get_drvdata(codec);
	if (!priv->pm.support_s4_pm) {
		AUDIO_LOGE("not support s4 pm");
		return 0;
	}

	codec_suspend(codec);

	AUDIO_LOGI("end");
	return 0;
}

int hi6405_codec_restore(struct device *dev)
{
	struct hi6405_platform_data *priv = NULL;
	struct snd_soc_codec *codec = hi6405_get_codec();
	struct snd_soc_card *card = NULL;

	AUDIO_LOGI("begin");
	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return 0;
	}

	card = codec->component.card;
	priv = snd_soc_codec_get_drvdata(codec);
	if (!priv->pm.support_s4_pm) {
		AUDIO_LOGE("not support s4 pm");
		return 0;
	}

	get_headset_plug_in_status();
	codec_resume(codec);
	hi64xx_fw_restore();

	AUDIO_LOGI("end");
	return 0;
}

static void get_codec_pm_switch(struct snd_soc_codec *codec)
{
	struct hi6405_platform_data *priv = snd_soc_codec_get_drvdata(codec);
	struct device *dev = codec->dev;
	struct snd_ctl_elem_value *switches = NULL;
	int switches_count, ret, len, i;
	const char *prop_name = "codec_pm_switches";
	const char *name = NULL;

	switches_count = of_property_count_strings(dev->of_node, prop_name);
	if ((switches_count <= 0) || (switches_count > CODEC_PM_SWITCHES_MAX_COUNT)) {
		AUDIO_LOGE("get properity %s's count failed with error %d", prop_name, switches_count);
		return;
	}

	switches = devm_kzalloc(dev, switches_count * sizeof(*switches), GFP_KERNEL);
	if (switches == NULL) {
		AUDIO_LOGE("switches is null");
		return;
	}

	priv->pm.codec_pm_switches = switches;
	len = sizeof(switches->id.name);

	for (i = 0; i < switches_count; ++i) {
		ret = of_property_read_string_index(dev->of_node, prop_name, i, &name);
		if (ret) {
			AUDIO_LOGE("property %s index %d read failed with error %d", prop_name, i, ret);
			return;
		}

		ret = strlcpy(switches[i].id.name, name, len);
		if (ret >= len)
			AUDIO_LOGW("switch '%s' is too long was truncated", name);

		switches[i].id.iface = SNDRV_CTL_ELEM_IFACE_MIXER;
		AUDIO_LOGI("switch '%s' support codec pm", name);
	}

	priv->pm.codec_pm_switches_count = switches_count;
}

void hi6405_codec_pm_init(struct snd_soc_codec *codec)
{
	struct hi6405_platform_data *priv = NULL;

	if (codec == NULL) {
		AUDIO_LOGE("codec parameter is null");
		return;
	}

	priv = snd_soc_codec_get_drvdata(codec);

	if (of_property_read_bool(priv->node, "hisilicon,support_s4_pm"))
		priv->pm.support_s4_pm = true;
	else
		priv->pm.support_s4_pm = false;

	AUDIO_LOGI("support s4 pm: %d", priv->pm.support_s4_pm);

	get_codec_pm_switch(codec);
}
