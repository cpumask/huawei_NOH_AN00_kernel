/*
 * hi64xx_vad.c
 *
 * codec vad driver
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

#include <linux/hisi/hi64xx/hi64xx_vad.h>

#include <linux/mutex.h>
#include <sound/soc.h>
#include <linux/jiffies.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/hisi/hi64xx/hi64xx_irq.h>
#include <hi64xx_algo_interface.h>
#include <linux/hisi/hi64xx/hi64xx_regs.h>
#include <linux/hisi/hi64xx/hi64xx_mad.h>
#include <linux/hisi/audio_log.h>
#include "soundtrigger_event.h"

#define LOG_TAG "DA_combine_vad"

#define HI6402_DSP_VAD_CMD (BASE_ADDR_PAGE_CFG + 0x73)

struct hi64xx_vad_platform_data {
	struct snd_soc_codec *codec;
	struct hi64xx_irq *irq;
	struct wakeup_source soundtrigger_wake_lock;
	bool fast_mode_enable;
};

static struct hi64xx_vad_platform_data *g_vad_data;

static irqreturn_t hi64xx_sound_trigger_handler(int irq, void *data)
{
	struct hi64xx_vad_platform_data *pdata = data;

	__pm_wakeup_event(&pdata->soundtrigger_wake_lock, 1000);

	/* clr VAD INTR */
	snd_soc_write(pdata->codec, HI64XX_VAD_INT_SET, 0);
	snd_soc_write(pdata->codec, HI64XX_REG_IRQ_1, BIT(HI64XX_VAD_BIT));

	AUDIO_LOGI("soundtrigger event: %d, fast mode enable: %d",
		HW_SOUNDTRIGGER_HISI_TRIGGER, g_vad_data->fast_mode_enable);

	if (g_vad_data->fast_mode_enable == false)
		hw_soundtrigger_event_input(HW_SOUNDTRIGGER_HISI_TRIGGER);
	else
		hw_soundtrigger_event_uevent(HW_SOUNDTRIGGER_HISI_TRIGGER);


	return IRQ_HANDLED;
}

int hi64xx_fast_mode_set(bool enable)
{
	if (g_vad_data == NULL) {
		AUDIO_LOGE("vad data is null");
		return -EINVAL;
	}

	g_vad_data->fast_mode_enable = enable;

	return 0;
}

int hi64xx_vad_init(struct snd_soc_codec *codec, struct hi64xx_irq *irq)
{
	int ret;

	AUDIO_LOGI("enter");

	if (codec == NULL || codec->dev == NULL || codec->dev->of_node == NULL ||
		irq == NULL) {
		AUDIO_LOGE("param is NULL");
		return -EINVAL;
	}

	if (of_property_read_bool(codec->dev->of_node, "hisilicon,dsp_soundtrigger_disable")) {
		AUDIO_LOGI("dsp soundtrigger disable, not need init vad");
		return 0;
	}

	g_vad_data = kzalloc(sizeof(*g_vad_data), GFP_KERNEL);
	if (g_vad_data == NULL) {
		AUDIO_LOGE("cannot allocate hisi vad platform data");
		return -ENOMEM;
	}

	g_vad_data->codec = codec;
	g_vad_data->irq = irq;
	g_vad_data->fast_mode_enable = false;

	wakeup_source_init(&g_vad_data->soundtrigger_wake_lock, "hisi-6402-soundtrigger");

	ret = hi64xx_irq_request_irq(g_vad_data->irq, IRQ_VAD,
		hi64xx_sound_trigger_handler, "sound_triger", g_vad_data);
	if (ret < 0) {
		AUDIO_LOGE("request irq for soundtrigger err");
		wakeup_source_trash(&g_vad_data->soundtrigger_wake_lock);
		kfree(g_vad_data);
		g_vad_data = NULL;
	}

	AUDIO_LOGI("exit");

	return ret;
}

int hi64xx_vad_deinit(struct device_node *node)
{
	if (node == NULL || g_vad_data == NULL) {
		AUDIO_LOGE("param is NULL");
		return -EINVAL;
	}

	if (of_property_read_bool(node, "hisilicon,dsp_soundtrigger_disable")) {
		AUDIO_LOGI("dsp soundtrigger disable, not need deinit vad");
		return 0;
	}

	if (g_vad_data->irq != NULL)
		hi64xx_irq_free_irq(g_vad_data->irq, IRQ_VAD, g_vad_data);

	wakeup_source_trash(&g_vad_data->soundtrigger_wake_lock);

	kfree(g_vad_data);
	g_vad_data = NULL;

	return 0;
}

