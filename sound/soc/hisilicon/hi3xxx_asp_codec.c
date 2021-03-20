/*
 * hi3xxx_asp_codec.c -- hi3xxx asp codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

#include "audio_log.h"

#ifdef CONFIG_HISI_AUDIO_DEBUG
#define LOG_TAG "hi3xxx_asp_codec"
#else
#define LOG_TAG "hi3xxx_Analog_less"
#endif

/*lint -e785*/
static struct snd_soc_dai_link hi3xxx_asp_codec_dai_link[] = {
	{
		.name = "hi3xxx_asp_codec_pb_normal",
		.stream_name = "hi3xxx_asp_codec_pb_normal",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-mm",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_voice",
		.stream_name = "hi3xxx_voice",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-modem",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "snd-soc-dummy",
	},
	{
		.name = "hi3xxx_fm1",
		.stream_name = "hi3xxx_fm1",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-fm",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "snd-soc-dummy",
	},
	{
		.name = "hi3xxx_asp_codec_pb_dsp",
		.stream_name = "hi3xxx_asp_codec_pb_dsp",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-lpp",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_asp_codec_pb_direct",
		.stream_name = "hi3xxx_asp_codec_pb_direct",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-direct",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_asp_codec_lowlatency",
		.stream_name = "hi3xxx_asp_codec_lowlatency",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hi6210-fast",
		.codec_dai_name = "asp-codec-dai",
#ifdef AUDIO_LOW_LATENCY_LEGACY
		.platform_name = "hi3xxx-pcm-asp-dma",
#else
		.platform_name = "hi6210-hifi",
#endif
	},
	{
		.name = "hi3xxx_asp_codec_mmap",
		.stream_name = "hi3xxx_asp_codec_mmap",
		.codec_name = "asp-codec-990",
		.cpu_dai_name = "hisi-pcm-mmap",
		.codec_dai_name = "asp-codec-dai",
		.platform_name = "hi6210-hifi",
	},
};

static struct snd_soc_card hi3xxx_asp_codec_card = {
	.name = "hi3xxx_asp_codec_card",
	.owner = THIS_MODULE,
	.dai_link = hi3xxx_asp_codec_dai_link,
	.num_links = ARRAY_SIZE(hi3xxx_asp_codec_dai_link),
};

static int hi3xxx_asp_codec_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct snd_soc_card *card = &hi3xxx_asp_codec_card;

	if (pdev == NULL) {
		AUDIO_LOGE("pdev is null, fail");
		return -EINVAL;
	}

	AUDIO_LOGI("hi3xxx asp codec probe");

	card->dev = &pdev->dev;

	ret = snd_soc_register_card(card);
	if (ret) {
		AUDIO_LOGE("sound card register failed %d", ret);
	}

	return ret;
}

static int hi3xxx_asp_codec_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	if (card != NULL)
		snd_soc_unregister_card(card);

	return 0;
}

static const struct of_device_id hi3xxx_asp_codec_of_match[] = {
	{.compatible = "hisilicon,hi3xxx-asp-codec", },
	{ },
};
MODULE_DEVICE_TABLE(of, hi3xxx_asp_codec_of_match);

static struct platform_driver hi3xxx_asp_codec_driver = {
	.driver = {
		.name = "hi3xxx_asp_codec",
		.owner = THIS_MODULE,
		.of_match_table = hi3xxx_asp_codec_of_match,
	},
	.probe = hi3xxx_asp_codec_probe,
	.remove = hi3xxx_asp_codec_remove,
};

static int __init hi3xxx_analog_less_init(void)
{
	AUDIO_LOGI("hi3xxx analog less init");
	return platform_driver_register(&hi3xxx_asp_codec_driver);
}

late_initcall(hi3xxx_analog_less_init);

static void __exit hi3xxx_analog_less_exit(void)
{
	platform_driver_unregister(&hi3xxx_asp_codec_driver);
}
module_exit(hi3xxx_analog_less_exit);

MODULE_DESCRIPTION("ALSA SoC for Hisilicon hi3xxx with asp codec");
MODULE_LICENSE("GPL");
MODULE_ALIAS("machine driver:hi3xxx-asp-codec");
