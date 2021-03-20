/*
 * hi64xx_utils.c
 *
 * hi64xx_utils codec driver
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

#include <linux/hisi/hi64xx/hi64xx_utils.h>

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/hisi/hi64xx/hi64xx_resmgr.h>
#include <linux/hisi/audio_log.h>

#ifdef CONFIG_SND_SOC_HI6405
#include <linux/hisi/hi64xx/hi6405.h>
#else
#define LOG_TAG "DA_combine_utils"
#endif

static struct utils_config *g_utils_config;
static unsigned int g_cdc_type = HI64XX_CODEC_TYPE_BUTT;
static struct snd_soc_codec *g_snd_codec;

int hi64xx_update_bits(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int mask, unsigned int value)
{
	int change;
	unsigned int old;
	unsigned int new;

	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return -EINVAL;
	}
	old = snd_soc_read(codec, reg);
	new = (old & ~mask) | (value & mask);
	change = (old != new);
	if (change != 0)
		snd_soc_write(codec, reg, new);

	return change;
}


int hi64xx_utils_init(struct snd_soc_codec *codec, struct hi_cdc_ctrl *cdc_ctrl,
	const struct utils_config *config, struct hi64xx_resmgr *resmgr,
	unsigned int codec_type)
{
	g_utils_config = kzalloc(sizeof(*g_utils_config), GFP_KERNEL);
	if (g_utils_config == NULL) {
		AUDIO_LOGE("Failed to kzalloc utils config");
		g_snd_codec = NULL;
		return -EFAULT;
	}
	memcpy(g_utils_config, config, sizeof(*g_utils_config));

	g_cdc_type = codec_type;
	g_snd_codec = codec;

	return 0;
}

void hi64xx_utils_deinit(void)
{
	if (g_utils_config != NULL) {
		kfree(g_utils_config);
		g_utils_config = NULL;
	}

	g_snd_codec = NULL;
}

#ifdef CONFIG_HISI_DIEID
int hisi_codec_get_dieid(char *dieid, unsigned int len)
{
#ifdef CONFIG_SND_SOC_HI6405
	if (g_cdc_type == HI64XX_CODEC_TYPE_6405)
		return hi6405_codec_get_dieid(dieid, len);
#endif
	return -1;
}
#endif

unsigned int hi64xx_utils_reg_read(unsigned int reg)
{
	if (g_snd_codec == NULL) {
		AUDIO_LOGE("parameter is NULL");
		return 0;
	}

	return snd_soc_read(g_snd_codec, reg);
}

