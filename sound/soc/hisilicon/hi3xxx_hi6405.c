/*
 * hi3xxx_hi6405 is the machine driver of alsa which is used to registe sound card
 * Copyright (C) 2014	Hisilicon

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

#define  EXTERN_HIFI_CODEC_CODECLESS_NAME  "codecless"
#define  EXTERN_HIFI_CODEC_WITHOUT_HIFI    "codec_without_hifi"

#ifdef CONFIG_HISI_AUDIO_DEBUG
#define HI3XXX_HI6405_DRIVER_NAME      "hi3xxx_hi6405"
#define HI3XXX_HI6405_CODECLESS_NAME   "hi3xxx_hi6405_codecless_card"
#define HI3XXX_HI6405_CARD_NAME        "hi3xxx_hi6405_card"
#else
#define HI3XXX_HI6405_DRIVER_NAME      "hi3xxx_DA_combine_v5"
#define HI3XXX_HI6405_CODECLESS_NAME   "hi3xxx_DA_combine_v5_codecless_card"
#define HI3XXX_HI6405_CARD_NAME        "hi3xxx_DA_combine_v5_card"
#endif

/* The following definition is the same as that of hi6405.c */
#ifdef CONFIG_HISI_AUDIO_DEBUG
#define DAI_LINK_CODEC_NAME            "hi6405-codec"
#define DAI_LINK_CODEC_DAI_NAME_AUDIO  "hi6405-audio-dai"
#define DAI_LINK_CODEC_DAI_NAME_VOICE  "hi6405-voice-dai"
#define DAI_LINK_CODEC_DAI_NAME_FM     "hi6405-fm-dai"
#define DAI_LINK_CODEC_DAI_NAME_BT     "hi6405-bluetooth-dai"
#else
#define DAI_LINK_CODEC_NAME            "DA_combine_v5"
#define DAI_LINK_CODEC_DAI_NAME_AUDIO  "DA_combine_v5-audio-dai"
#define DAI_LINK_CODEC_DAI_NAME_VOICE  "DA_combine_v5-voice-dai"
#define DAI_LINK_CODEC_DAI_NAME_FM     "DA_combine_v5-fm-dai"
#define DAI_LINK_CODEC_DAI_NAME_BT     "DA_combine_v5-bluetooth-dai"
#endif

/*lint -e785*/
static struct snd_soc_dai_link hi3xxx_hi6405_dai_link[] = {
	{
		.name = "hi3xxx_hi6405_pb_normal",
		.stream_name = "hi3xxx_hi6405_pb_normal",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_voice",
		.stream_name = "hi3xxx_voice",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_VOICE,
		.platform_name = "snd-soc-dummy",
	},
	{
		.name = "hi3xxx_fm1",
		.stream_name = "hi3xxx_fm1",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_FM,
		.platform_name = "snd-soc-dummy",
	},
	{
		.name = "hi3xxx_hi6405_pb_dsp",
		.stream_name = "hi3xxx_hi6405_pb_dsp",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_hi6405_pb_direct",
		.stream_name = "hi3xxx_hi6405_pb_direct",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.platform_name = "hi6210-hifi",
	},
	{
		.name = "hi3xxx_hi6405_lowlatency",
		.stream_name = "hi3xxx_hi6405_lowlatency",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
#ifdef AUDIO_LOW_LATENCY_LEGACY
		.platform_name = "hi3xxx-pcm-asp-dma",
#else
		.platform_name = "hi6210-hifi",
#endif
	},
	{
		.name = "hi3xxx_hi6405_mmap",
		.stream_name = "hi3xxx_hi6405_mmap",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.platform_name = "hi6210-hifi",
	},
};

static struct snd_soc_dai_link codecless_dai_link[] = {
	/* RX for headset/headphone with audio mode */
	{
		 .name = "CODECLESS_OUTPUT",
		 .stream_name = "Playback",
		 .codec_name = "asp-codec-codecless",
		 .cpu_dai_name = "hifi-vir-dai",
		 .platform_name = "hi6210-hifi",
		 .codec_dai_name = "asp-codec-codecless-dai",
	},
};

static struct snd_soc_dai_link hi3xxx_hi6405_dai_link_without_hifi[] = {
	{
		.name = "hi3xxx_hi6405_pb_normal",
		.stream_name = "hi3xxx_hi6405_pb_normal",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.platform_name = "hisi-pcm-codec",
		.nonatomic = true,
	},
	{
		.name = "hi3xxx_hi6405_bluetooth",
		.stream_name = "hi3xxx_hi6405_bluetooth",
		.codec_name = DAI_LINK_CODEC_NAME,
		.cpu_dai_name = "slimbus-dai",
		.codec_dai_name = DAI_LINK_CODEC_DAI_NAME_BT,
		.platform_name = "hisi-pcm-codec",
		.nonatomic = true,
	},
};

static struct snd_soc_dai_link hi3xxx_hi6405_codecless_dai_links[
				ARRAY_SIZE(hi3xxx_hi6405_dai_link) +
				ARRAY_SIZE(codecless_dai_link)];

static struct snd_soc_card hi3xxx_hi6405_card = {
	.name = HI3XXX_HI6405_CARD_NAME,
	.owner = THIS_MODULE,
	.dai_link = hi3xxx_hi6405_dai_link,
	.num_links = ARRAY_SIZE(hi3xxx_hi6405_dai_link),
};

struct snd_soc_card hi3xxx_hi6405_codecless_card = {
	.name = HI3XXX_HI6405_CODECLESS_NAME,
	.owner = THIS_MODULE,
	.dai_link = hi3xxx_hi6405_codecless_dai_links,
	.num_links = ARRAY_SIZE(hi3xxx_hi6405_codecless_dai_links),
};

static struct snd_soc_card hi3xxx_hi6405_card_without_hifi = {
	.name = HI3XXX_HI6405_CARD_NAME,
	.owner = THIS_MODULE,
	.dai_link = hi3xxx_hi6405_dai_link_without_hifi,
	.num_links = ARRAY_SIZE(hi3xxx_hi6405_dai_link_without_hifi),
};


static int populate_extern_snd_card_dailinks(struct platform_device *pdev)
{
	pr_info("%s:Audio: probe\n", __func__);

	memcpy(hi3xxx_hi6405_codecless_dai_links, hi3xxx_hi6405_dai_link,
				sizeof(hi3xxx_hi6405_dai_link)); /* unsafe_function_ignore: memcpy */
	memcpy(hi3xxx_hi6405_codecless_dai_links + ARRAY_SIZE(hi3xxx_hi6405_dai_link),
			codecless_dai_link, sizeof(codecless_dai_link)); /* unsafe_function_ignore: memcpy */

	return 0;
}

static int hi3xxx_hi6405_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct snd_soc_card *card = &hi3xxx_hi6405_card;
	const char *extern_codec_type = "huawei,extern_codec_type";
	const char *ptr = NULL;

	if (NULL == pdev) {
		pr_err("probe: pdev is null, fail\n");
		return -EINVAL;
	}

	pr_info("Audio: probe\n");

	ret = of_property_read_string(pdev->dev.of_node, extern_codec_type, &ptr);
	if (!ret) {
		pr_info("extern type:%s in dt node\n", extern_codec_type);
		if (!strncmp(ptr, EXTERN_HIFI_CODEC_CODECLESS_NAME, strlen(EXTERN_HIFI_CODEC_CODECLESS_NAME))) {
			populate_extern_snd_card_dailinks(pdev);
			card = &hi3xxx_hi6405_codecless_card;
		} else if (!strncmp(ptr, EXTERN_HIFI_CODEC_WITHOUT_HIFI, strlen(EXTERN_HIFI_CODEC_WITHOUT_HIFI))) {
			card = &hi3xxx_hi6405_card_without_hifi;
		} else {
			card = &hi3xxx_hi6405_card;
		}
	} else {
		card = &hi3xxx_hi6405_card;
	}

	card->dev = &pdev->dev;

	ret = snd_soc_register_card(card);
	if (ret) {
		pr_err("probe: sound card register failed %d\n", ret);
	}

	return ret;
}

static int hi3xxx_hi6405_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	if (NULL != card)
		snd_soc_unregister_card(card);

	return 0;
}

static const struct of_device_id hi3xxx_hi6405_of_match[] = {
	{.compatible = "hisilicon,hi3xxx-hi6405", },
	{ },
};
MODULE_DEVICE_TABLE(of, hi3xxx_hi6405_of_match);

static struct platform_driver hi3xxx_hi6405_driver = {
	.driver	= {
		.name = HI3XXX_HI6405_DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = hi3xxx_hi6405_of_match,
	},
	.probe	= hi3xxx_hi6405_probe,
	.remove	= hi3xxx_hi6405_remove,
};

static int __init hi3xxx_da_combine_v5_init(void)
{
	pr_info("Audio: init\n");
	return platform_driver_register(&hi3xxx_hi6405_driver);
}

late_initcall(hi3xxx_da_combine_v5_init);

static void __exit hi3xxx_da_combine_v5_exit(void)
{
	platform_driver_unregister(&hi3xxx_hi6405_driver);
}
module_exit(hi3xxx_da_combine_v5_exit);

MODULE_DESCRIPTION("ALSA SoC for Hisilicon hi3xxx with hi6405 codec");
MODULE_LICENSE("GPL");
MODULE_ALIAS("machine driver:hi3xxx-hi6405");
