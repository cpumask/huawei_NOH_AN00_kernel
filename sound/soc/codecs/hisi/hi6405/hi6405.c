/*
 * hi6405.c -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include "linux/hisi/hi64xx/hi6405.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/pcm_params.h>
#include <sound/tlv.h>
#include <sound/soc.h>

#ifdef CONFIG_HUAWEI_DSM
#include <dsm_audio/dsm_audio.h>
#endif
#include "linux/hisi/audio_log.h"
#include "slimbus.h"
#include "slimbus_6405.h"
#ifdef CONFIG_SND_SOC_HICODEC_DEBUG
#include "hi6405_debug.h"
#endif
#include "linux/hisi/hi64xx/hi64xx_compat.h"
#include "linux/hisi/hi64xx/hi64xx_resmgr.h"
#include "linux/hisi/hi64xx/hi6405_regs.h"
#include "linux/hisi/hi64xx/hi6405_type.h"
#include <linux/hisi/hi64xx_dsp/hi64xx_dsp_misc.h>
#include "hi6405_path_widget.h"
#include "hi6405_codec_probe.h"
#include "hi6405_utils.h"
#include "hi6405_codec_pm.h"


#define PLL_DATA_ALL_NUM     128
#define PLL_DATA_GROUP_NUM   8
#define PLL_DATA_BUF_SIZE    10

/* The following definition is the same as that of hi3xxx_hi6405.c */
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

static const struct of_device_id hi6405_platform_match[] = {
	{ .compatible = "hisilicon,hi6405-codec", },
	{ },
};

enum codec_type {
	CODEC_TYPE_DEFAULT = 0,
	CODEC_TYPE_PC = 1,
};

static int hi6405_audio_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	int ret = 0;
	int rate;

	if (params == NULL) {
		AUDIO_LOGE("pcm params is null");
		return -EINVAL;
	}

	if (dai == NULL) {
		AUDIO_LOGE("soc dai is null");
		return -EINVAL;
	}

	rate = params_rate(params);
	switch (rate) {
	case 8000:
	case 11025:
	case 16000:
	case 22050:
	case 32000:
	case 44100:
	case 48000:
	case 88200:
	case 96000:
	case 176400:
	case 192000:
		break;
	case 384000:
		AUDIO_LOGE("rate: %d", rate);
		break;
	default:
		AUDIO_LOGE("unknown rate: %d", rate);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int hi6405_audio_hw_free(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	return 0;
}

struct snd_soc_dai_ops hi6405_audio_dai_ops = {
	.hw_params = hi6405_audio_hw_params,
	.hw_free = hi6405_audio_hw_free,
};

static int hi6405_voice_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	int ret = 0;
	int rate;

	if (params == NULL) {
		AUDIO_LOGE("pcm params is null");
		return -EINVAL;
	}

	if (dai == NULL) {
		AUDIO_LOGE("soc dai is null");
		return -EINVAL;
	}

	rate = params_rate(params);
	switch (rate) {
	case 8000:
	case 16000:
	case 32000:
		break;
	default:
		AUDIO_LOGE("unknown rate: %d", rate);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int hi6405_voice_hw_free(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	return 0;
}

struct snd_soc_dai_ops hi6405_voice_dai_ops = {
	.hw_params = hi6405_voice_hw_params,
	.hw_free = hi6405_voice_hw_free,
};

struct snd_soc_dai_driver hi6405_dai[] = {
	{
		.name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.playback = {
			.stream_name = "Playback",
			.channels_min = 2,
			.channels_max = 4,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.capture = {
			.stream_name = "Capture",
			.channels_min = 1,
			.channels_max = 13,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.ops = &hi6405_audio_dai_ops,
	},
	{
		.name = DAI_LINK_CODEC_DAI_NAME_VOICE,
		.playback = {
			.stream_name = "Down",
			.channels_min = 1,
			.channels_max = 2,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.capture = {
			.stream_name = "Up",
			.channels_min = 1,
			.channels_max = 6,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.ops = &hi6405_voice_dai_ops,
	},
	{
		.name = DAI_LINK_CODEC_DAI_NAME_FM,
		.playback = {
			.stream_name = "FM",
			.channels_min = 1,
			.channels_max = 2,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
	},
};


struct snd_soc_dai_driver hi6405_dai_pc[] = {
	{
		.name = DAI_LINK_CODEC_DAI_NAME_AUDIO,
		.playback = {
			.stream_name = "Playback",
			.channels_min = 2,
			.channels_max = 4,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.capture = {
			.stream_name = "Capture",
			.channels_min = 1,
			.channels_max = 6,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.ops = &hi6405_audio_dai_ops,
	},
	{
		.name = DAI_LINK_CODEC_DAI_NAME_BT,
		.playback = {
			.stream_name = "Down",
			.channels_min = 2,
			.channels_max = 2,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.capture = {
			.stream_name = "Up",
			.channels_min = 1,
			.channels_max = 2,
			.rates = HI6405_RATES,
			.formats = HI6405_FORMATS },
		.ops = &hi6405_audio_dai_ops,
	},
};


static struct snd_soc_codec_driver hi6405_codec_driver = {
	.probe = hi6405_codec_probe,
	.remove = hi6405_codec_remove,
	.read = hi6405_reg_read,
	.write = hi6405_reg_write,
};

static bool check_card_valid(struct hi6405_platform_data *platform_data)
{
	unsigned int val = hi_cdcctrl_reg_read(platform_data->cdc_ctrl,
		HI64XX_VERSION_REG);

	if (val != HI64XX_VERSION_CS) {
		AUDIO_LOGE("read version failed:0x%x", val);
		return false;
	}

	return true;
}

static void get_board_micnum(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	if (!of_property_read_u32(node, "hisilicon,mic_num", &val))
		board_cfg->mic_num = val;
	else
		board_cfg->mic_num = 2;
}

static void get_board_hpswitch(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	if (!of_property_read_u32(node, "hisilicon,classh_rcv_hp_switch", &val)) {
		if (val != 0)
			board_cfg->classh_rcv_hp_switch = true;
		else
			board_cfg->classh_rcv_hp_switch = false;
	} else {
		board_cfg->classh_rcv_hp_switch = false;
	}
}

static void get_board_micmap(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	int ret;
	unsigned int i;
	unsigned int *map = board_cfg->mic_map;

	ret = of_property_read_u32_array(node, "hisilicon,mic_map",
		map, TOTAL_MACHINE_CALIB_MIC);
	if (ret) {
		AUDIO_LOGE("mic map read error\n");
		goto mic_map_err;
	}
	/* check the validation */
	for (i = 0; i < TOTAL_MACHINE_CALIB_MIC; i++) {
		if (map[i] >= TOTAL_CODEC_CALIB_MIC) {
			AUDIO_LOGE("mic map values are not valid\n");
			goto mic_map_err;
		}
	}
	return;
mic_map_err:
	for (i = 0; i < TOTAL_MACHINE_CALIB_MIC; i++)
		*(map++) = i;
}

#ifdef CONFIG_HAC_SUPPORT
int hac_gpio_init(int hac_gpio)
{
	if (!gpio_is_valid(hac_gpio)) {
		AUDIO_LOGE("hac Value is not valid");
		return -1;
	}
	if (gpio_request(hac_gpio, "hac_en_gpio")) {
		AUDIO_LOGE("hac gpio request failed");
		return -1;
	}
	if (gpio_direction_output(hac_gpio, 0)) {
		AUDIO_LOGE("hac gpio set output failed");
		return -1;
	}

	return 0;
}

static void get_board_hac(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;
	int ret = 0;

	if (!of_property_read_u32(node, "hisilicon,hac_gpio", &val)) {
		board_cfg->hac_gpio = val;
		ret = hac_gpio_init(board_cfg->hac_gpio);
		if (ret != 0)
			AUDIO_LOGE("gpio resource init fail, ret = %d", ret);
	} else {
		board_cfg->hac_gpio = -1;
	}
}
#endif

static void get_board_wakeup_hisi_algo_support(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	board_cfg->wakeup_hisi_algo_support = false;
	if (!of_property_read_u32(node, "hisilicon,wakeup_hisi_algo_support", &val)) {
		if (val != 0)
			board_cfg->wakeup_hisi_algo_support = true;
	}
}

static void get_board_hp_res_detect(struct device_node *node,
			struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	board_cfg->hp_res_detect_enable = false;
	if (of_property_read_u32(node, "hisilicon,hp_res_detect_enable", &val))
		return;

	if (val != 0)
		board_cfg->hp_res_detect_enable = true;

}

static void get_board_mic_control_sc_frequency(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	board_cfg->mic_control_sc_freq_enable = false;
	if (of_property_read_u32(node, "mic_control_sc_frequency", &val))
		return;

	if (val != 0)
		board_cfg->mic_control_sc_freq_enable = true;
}

static void get_board_cap_1nf_enable(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->cap_1nf_enable = false;
	if (of_property_read_bool(node, "cap_1nf_enable"))
		board_cfg->cap_1nf_enable = true;
}

static void get_board_ultra_pll_sync_i2s_id(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	board_cfg->ultra_pll_sync_i2s_id = 1;
	if (!of_property_read_u32(node, "ultra_pll_sync_i2s_id", &val))
		board_cfg->ultra_pll_sync_i2s_id = val;
}

static void get_board_headphone_pop_on_delay(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int val = 0;

	board_cfg->headphone_pop_on_delay_enable = false;
	if (of_property_read_u32(node, "headphone_pop_on_delay", &val))
		return;

	if (val != 0)
		board_cfg->headphone_pop_on_delay_enable = true;
}

static void get_board_backmic_record_enable(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->backmic_record_enable = false;
	if (of_property_read_bool(node, "backmic_record_enable"))
		board_cfg->backmic_record_enable = true;
}

static void get_board_single_kcontrol_route(const struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->single_kcontrol_route_mode = false;
	if (of_property_read_bool(node, "hisilicon,single_kcontrol_route"))
		board_cfg->single_kcontrol_route_mode = true;
}

static void get_hs_capture_gain(const struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	unsigned int gain;

	/* 0x4 -> 8dB default gain */
	board_cfg->hs_capture_gain = 0x4;
	if (!of_property_read_u32(node, "hisilicon,hs_capture_gain", &gain))
		board_cfg->hs_capture_gain = gain;
}

static void get_codec_extra_info_cfg(const struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->codec_extra_info_enable = false;
	if (of_property_read_bool(node, "hisilicon,codec_extra_info_enable"))
		board_cfg->codec_extra_info_enable = true;
}

static void get_board_pllunlock_to_reset_dsp_enable(const struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->pllunlock_to_reset_dsp_enable = true;
	if (of_property_read_bool(node, "pllunlock_to_reset_dsp_disable"))
		board_cfg->pllunlock_to_reset_dsp_enable = false;
}

static void get_dsp_soundtrigger_cfg(const struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	board_cfg->dsp_soundtrigger_disable = false;
	if (of_property_read_bool(node, "hisilicon,dsp_soundtrigger_disable"))
		board_cfg->dsp_soundtrigger_disable = true;
}

static void get_board_cfg(struct device_node *node,
	struct hi6405_board_cfg *board_cfg)
{
	get_board_micnum(node, board_cfg);
	get_board_hpswitch(node, board_cfg);
	get_board_micmap(node, board_cfg);
#ifdef CONFIG_HAC_SUPPORT
	get_board_hac(node, board_cfg);
	AUDIO_LOGI("hac_gpio %d", board_cfg->hac_gpio);
#endif
	AUDIO_LOGI("mic_num %d", board_cfg->mic_num);
	get_board_wakeup_hisi_algo_support(node, board_cfg);
	AUDIO_LOGI("wakeup_hisi_algo_support %d",
		board_cfg->wakeup_hisi_algo_support);
	get_board_hp_res_detect(node, board_cfg);
	AUDIO_LOGI("hp_res_detect %d", board_cfg->hp_res_detect_enable);
	get_board_mic_control_sc_frequency(node, board_cfg);
	AUDIO_LOGI("mic control sc frequency %d",
		board_cfg->mic_control_sc_freq_enable);
	get_board_cap_1nf_enable(node, board_cfg);
	AUDIO_LOGI("cap 1nf enable %d",
		board_cfg->cap_1nf_enable);
	get_board_headphone_pop_on_delay(node, board_cfg);
	AUDIO_LOGI("headphone pop on delay %d",
		board_cfg->headphone_pop_on_delay_enable);
	get_board_backmic_record_enable(node, board_cfg);
	AUDIO_LOGI("backmic record enable %d",
		board_cfg->backmic_record_enable);
	get_board_single_kcontrol_route(node, board_cfg);
	AUDIO_LOGI("single kcontrol route %d",
		board_cfg->single_kcontrol_route_mode);
	get_hs_capture_gain(node, board_cfg);
	AUDIO_LOGI("headset capture gain: %d",
		board_cfg->hs_capture_gain);
	get_codec_extra_info_cfg(node, board_cfg);
	AUDIO_LOGI("codec extra info enable: %d",
		board_cfg->codec_extra_info_enable);
	get_board_pllunlock_to_reset_dsp_enable(node, board_cfg);
	AUDIO_LOGI("reset dsp enable: %d",
		board_cfg->pllunlock_to_reset_dsp_enable);
	get_dsp_soundtrigger_cfg(node, board_cfg);
	AUDIO_LOGI("dsp soundtrigger disable: %d",
		board_cfg->dsp_soundtrigger_disable);
	get_board_ultra_pll_sync_i2s_id(node, board_cfg);
	AUDIO_LOGI("ultra pll sync i2s id %d",
		board_cfg->ultra_pll_sync_i2s_id);
}

static int init_platform_data(struct platform_device *pdev,
	struct hi6405_platform_data *platform_data)
{
	const struct of_device_id *match = NULL;
	int i;

	platform_data->irqmgr = (struct hi64xx_irq *)dev_get_drvdata(pdev->dev.parent);
	platform_data->cdc_ctrl = (struct hi_cdc_ctrl *)dev_get_drvdata(pdev->dev.parent->parent);

	if (!check_card_valid(platform_data))
		return -ENODEV;

	match = of_match_device(hi6405_platform_match, &pdev->dev);
	if (match == NULL) {
		AUDIO_LOGE("get device info err");
		return -ENOENT;
	}

	platform_data->cdc_ctrl->ssi_check_enable = SSI_STATE_CHECK_DISABLE;
	platform_data->node = pdev->dev.of_node;
	get_board_cfg(platform_data->node, &platform_data->board_config);
	platform_data->voice_up_params.channels = 2;
	/* set channel as 4, when mic num exceeds 2 */
	if (platform_data->board_config.mic_num > 2)
		platform_data->voice_up_params.channels = 4;
	platform_data->voice_down_params.channels = 2;
	platform_data->capture_params.channels = 2;
	platform_data->capture_params.rate = SLIMBUS_SAMPLE_RATE_48K;
	platform_data->soundtrigger_params.rate = SLIMBUS_SAMPLE_RATE_16K;
	platform_data->soundtrigger_params.channels = 1;
	platform_data->voiceup_state = TRACK_FREE;
	platform_data->voiceup_v2_state = TRACK_FREE;
	platform_data->audioup_4mic_state = TRACK_FREE;
	platform_data->audioup_4mic_v2_state = TRACK_FREE;
	platform_data->audioup_5mic_state = TRACK_FREE;
	platform_data->play_params.channels = 2;
	platform_data->play_params.rate = SLIMBUS_SAMPLE_RATE_48K;
	platform_data->pa_iv_params.channels = 4;
	platform_data->pa_iv_params.rate = SLIMBUS_SAMPLE_RATE_48K;
	platform_data->ultrasonic_up_params.rate = SLIMBUS_SAMPLE_RATE_48K;
	platform_data->ultrasonic_up_params.channels = 1;
	for (i = 0; i < TOTAL_CODEC_CALIB_MIC; i++)
		platform_data->mic_calib_value[i] = MIC_CALIB_NONE_VALUE;

	spin_lock_init(&platform_data->v_rw_lock);
	mutex_init(&platform_data->impdet_dapm_mutex);

	platform_set_drvdata(pdev, platform_data);
	dev_set_name(&pdev->dev, DAI_LINK_CODEC_NAME);

	platform_data->is_madswitch_on = false;
	platform_data->is_callswitch_on = false;
	if (platform_data->board_config.headphone_pop_on_delay_enable)
		INIT_DELAYED_WORK(&(platform_data->headphone_pop_on_delay),
			headphone_pop_on_delay_work);

	return 0;
}

static inline void deinit_platform_data(struct hi6405_platform_data *platform_data)
{
	mutex_destroy(&platform_data->impdet_dapm_mutex);
}

static struct hi64xx_irq_map irqs = {
	{ IRQ_REG0_REG, IRQ_REG1_REG, IRQ_REG2_REG, IRQ_REG3_REG,
		IRQ_REG4_REG, IRQ_REG5_REG, IRQ_REG6_REG },
	{ IRQM_REG0_REG, IRQM_REG1_REG, IRQM_REG2_REG, IRQM_REG3_REG,
		IRQM_REG4_REG, IRQM_REG5_REG, IRQM_REG6_REG },
	IRQ_NUM,
};

static inline void dsm_report(int dsm_type, char *str_error)
{
#ifdef CONFIG_HUAWEI_DSM
	audio_dsm_report_info(AUDIO_CODEC, dsm_type, str_error);
#endif
}

static void dsp_power_down(struct snd_soc_codec *codec)
{
	hi64xx_update_bits(codec, SC_DSP_CTRL0_REG,
		0x1 << SC_DSP_SFT_RUNSTALL_OFFSET | 0x1 << SC_DSP_EN_OFFSET |
		0x1 << SC_HIFI_CLK_EN_OFFSET | 0x1 << SC_HIFI_ACLK_EN_OFFSET,
		0x1 << SC_DSP_SFT_RUNSTALL_OFFSET | 0x0 << SC_DSP_EN_OFFSET |
		0x0 << SC_HIFI_CLK_EN_OFFSET | 0x0 << SC_HIFI_ACLK_EN_OFFSET);
	hi64xx_update_bits(codec, APB_CLK_CFG_REG,
		0x1 << APB_PD_PCLK_EN_OFFSET, 0x0 << APB_PD_PCLK_EN_OFFSET);
	snd_soc_write(codec, DSP_IF_CLK_EN, 0x0);
	hi64xx_update_bits(codec, SW_RST_REQ_REG,
		0x1 << DSP_PD_SRST_REQ_OFFSET, 0x1 << DSP_PD_SRST_REQ_OFFSET);
	hi64xx_update_bits(codec, DSP_LP_CTRL0_REG,
		0x1 << DSP_TOP_ISO_CTRL_OFFSET | 0x1 << DSP_TOP_MTCMOS_CTRL_OFFSET,
		0x1 << DSP_TOP_ISO_CTRL_OFFSET | 0x1 << DSP_TOP_MTCMOS_CTRL_OFFSET);
}

#ifdef CONFIG_HUAWEI_DSM
static void irq_handler(char *irq_name, unsigned int reg,
	unsigned int reg_offset, int dsm_type, void *data)
{
	struct hi6405_platform_data *platform_data = (struct hi6405_platform_data *)(data);
	struct snd_soc_codec *codec = platform_data->codec;

	if (codec != NULL) {
		AUDIO_LOGW("%s irq receive", irq_name);
		snd_soc_write(codec, reg, 0x1 << reg_offset);
		dsm_report(dsm_type, irq_name);
	}
}
#else
static void irq_handler(const char *irq_name, unsigned int reg,
	unsigned int reg_offset, int dsm_type, void *data)
{

}
#endif

static irqreturn_t bunk1_ocp_handler(int irq, void *data)
{
	irq_handler("64xx codec bunk1_ocp", IRQ_REG5_REG,
		PMU_BUNK1_OCP_IRQ_OFFSET, DSM_CODEC_BUNK1_OCP, data);
	return IRQ_HANDLED;
}

static irqreturn_t bunk1_scp_handler(int irq, void *data)
{
	irq_handler("64xx codec bunk1_scp", IRQ_REG5_REG,
		PMU_BUNK1_SCP_IRQ_OFFSET, DSM_CODEC_BUNK1_SCP, data);
	return IRQ_HANDLED;
}

static irqreturn_t ldo_avdd18_ocp_handler(int irq, void *data)
{
	irq_handler("64xx codec ldo_avdd18_ocp", IRQ_REG5_REG,
		PMU_LDO_AVDD18_OCP_IRQ_OFFSET, DSM_CODEC_LDO_AVDD18_OCP, data);
	return IRQ_HANDLED;
}

static irqreturn_t ldop_ocp_handler(int irq, void *data)
{
	irq_handler("64xx codec ldop_ocp", IRQ_REG5_REG,
		PMU_LDOP_OCP_IRQ_OFFSET, DSM_CODEC_LDOP_OCP, data);
	return IRQ_HANDLED;
}

static irqreturn_t ldon_ocp_handler(int irq, void *data)
{
	irq_handler("64xx codec ldon_ocp", IRQ_REG5_REG,
		PMU_LDON_OCP_IRQ_OFFSET, DSM_CODEC_LDON_OCP, data);
	return IRQ_HANDLED;
}

static irqreturn_t cp1_short_handler(int irq, void *data)
{
	irq_handler("64xx codec cp1_short", IRQ_REG5_REG,
		PMU_CP1_SHORT_IRQ_OFFSET, DSM_CODEC_CP1_SHORT, data);
	return IRQ_HANDLED;
}

static irqreturn_t cp2_short_handler(int irq, void *data)
{
	irq_handler("64xx codec cp2_short", IRQ_REG5_REG,
		PMU_CP2_SHORT_IRQ_OFFSET, DSM_CODEC_CP2_SHORT, data);
	return IRQ_HANDLED;
}

static const struct reg_seq_config pll_unlock_regs[] = {
	{ { PLL_TEST_CTRL1_REG, 0x1 << PLL_FIFO_CLK_EN_OFFSET,
		0x1 << PLL_FIFO_CLK_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_REF_CLK_CG_EN_OFFSET,
		0x1 << CODEC_ANA_TEST_REF_CLK_CG_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_200, 0x1 << CODEC_ANA_TEST_SOFT_RST_N_OFFSET,
		0x1 << CODEC_ANA_TEST_SOFT_RST_N_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_PLL_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_DCO_OPEN_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_EN_OFFSET |
		0x7 << CODEC_ANA_TEST_PLL_CLOSE_CAPTUTE_MODE_OFFSET,
		0x0 << CODEC_ANA_TEST_PLL_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_DCO_OPEN_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_CAPTUTE_MODE_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_194, 0, 0x07, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_195, 0, 0x07, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_196, 0x1 << CODEC_ANA_PVT_SEL_OFFSET |
		0x7 << CODEC_ANA_TEST_LOOP_I_OFFSET |
		0xf << CODEC_ANA_TEST_FREQ_CALC_CNT_TIME_OFFSET,
		0x1 << CODEC_ANA_PVT_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_LOOP_I_OFFSET |
		0xf << CODEC_ANA_TEST_FREQ_CALC_CNT_TIME_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_197, 0, 0xff, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_198, 0, 0xf, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_199, 0, 0xa0, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_200, 0x3 << CODEC_ANA_TEST_SCAN_CNT_TIME_OFFSET,
		0x2 << CODEC_ANA_TEST_SCAN_CNT_TIME_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_201, 0, 0x0, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_202, 0, 0xff, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_167, 0x7f << CODEC_ANA_MAIN1_TEST_TUNE_FINE_OFFSET,
		0x40 << CODEC_ANA_TEST_SCAN_CNT_TIME_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_168, 0x3f << CODEC_ANA_MAIN1_TEST_TUNE_PVT_OFFSET,
		0x20 << CODEC_ANA_MAIN1_TEST_TUNE_PVT_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_190, 0x7f << CODEC_ANA_MAIN2_TEST_TUNE_FINE_OFFSET,
		0x40 << CODEC_ANA_MAIN2_TEST_TUNE_FINE_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_191, 0x3f << CODEC_ANA_MAIN2_TEST_TUNE_PVT_OFFSET,
		0x20 << CODEC_ANA_MAIN2_TEST_TUNE_PVT_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET,
		0x0 << CODEC_ANA_TEST_MODE_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET,
		0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET, true }, 0, 0 },
};

static const struct reg_seq_config pll44k1_unlock_regs[] = {
	{ { PLL_TEST_CTRL1_REG, 0x1 << PLL_FIFO_CLK_EN_OFFSET,
		0x1 << PLL_FIFO_CLK_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_REF_CLK_CG_EN_OFFSET,
		0x1 << CODEC_ANA_TEST_REF_CLK_CG_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_200, 0x1 << CODEC_ANA_TEST_SOFT_RST_N_OFFSET,
		0x1 << CODEC_ANA_TEST_SOFT_RST_N_OFFSET, 1, }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_PLL_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_DCO_OPEN_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_EN_OFFSET |
		0x7 << CODEC_ANA_TEST_PLL_CLOSE_CAPTUTE_MODE_OFFSET,
		0x1 << CODEC_ANA_TEST_PLL_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_DCO_OPEN_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_EN_OFFSET |
		0x1 << CODEC_ANA_TEST_PLL_CLOSE_CAPTUTE_MODE_OFFSET, 1, }, 0, 0 },
	{ { CODEC_ANA_RWREG_194, 0, 0x7, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_195, 0, 0x7, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_196, 0x1 << CODEC_ANA_PVT_SEL_OFFSET |
		0x7 << CODEC_ANA_TEST_LOOP_I_OFFSET |
		0xf << CODEC_ANA_TEST_FREQ_CALC_CNT_TIME_OFFSET,
		0x1 << CODEC_ANA_PVT_SEL_OFFSET |
		0x1 << CODEC_ANA_TEST_LOOP_I_OFFSET |
		0xf << CODEC_ANA_TEST_FREQ_CALC_CNT_TIME_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_197, 0, 0xff, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_198, 0, 0xf, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_199, 0, 0xa0, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_200,
		0x3 << CODEC_ANA_TEST_SCAN_CNT_TIME_OFFSET,
		0x2 << CODEC_ANA_TEST_SCAN_CNT_TIME_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_201, 0, 0x0, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_202, 0, 0xff, false }, 0, 0 },
	{ { CODEC_ANA_RWREG_167, 0x7f << CODEC_ANA_MAIN1_TEST_TUNE_FINE_OFFSET,
		0x40 << CODEC_ANA_MAIN1_TEST_TUNE_FINE_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_168, 0x3f << CODEC_ANA_MAIN1_TEST_TUNE_PVT_OFFSET,
		0x20 << CODEC_ANA_MAIN1_TEST_TUNE_PVT_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_190, 0x7f << CODEC_ANA_MAIN2_TEST_TUNE_FINE_OFFSET,
		0x40 << CODEC_ANA_MAIN2_TEST_TUNE_FINE_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_191, 0x3f << CODEC_ANA_MAIN2_TEST_TUNE_PVT_OFFSET,
		0x20 << CODEC_ANA_MAIN2_TEST_TUNE_PVT_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET,
		0x0 << CODEC_ANA_TEST_MODE_EN_OFFSET, true }, 0, 0 },
	{ { CODEC_ANA_RWREG_193, 0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET,
		0x1 << CODEC_ANA_TEST_MODE_EN_OFFSET, true }, 0, 0 },
};

static int config_pll_test_reg(struct snd_soc_codec *codec, enum hi64xx_pll_type type)
{
	if (type == PLL_HIGH) {
		write_reg_seq_array(codec, pll_unlock_regs, ARRAY_SIZE(pll_unlock_regs));
		return 0;
	}

	if (type == PLL_44_1) {
		write_reg_seq_array(codec, pll44k1_unlock_regs, ARRAY_SIZE(pll44k1_unlock_regs));
		return 0;
	}

	AUDIO_LOGE("no support pll type %d", type);
	return -EINVAL;
}

static void dump_pll_data(struct snd_soc_codec *codec)
{
	unsigned char output_str[PLL_DATA_ALL_NUM] = {0};
	unsigned char buf[PLL_DATA_BUF_SIZE] = {0};
	unsigned int i;
	unsigned int j;

	for (i = 0; i < PLL_DATA_ALL_NUM; i = i + PLL_DATA_GROUP_NUM) {
		memset(output_str, 0, sizeof(output_str));
		memset(buf, 0, sizeof(buf));
		for (j = 0; j < PLL_DATA_GROUP_NUM; ++j) {
			snprintf(buf, sizeof(buf), "%9x",
				snd_soc_read(codec, PLL_FIFO));
			strncat(output_str, buf, strlen(buf));
		}
		AUDIO_LOGI("PLL data:%s", output_str);
	}
}

static void reset_codec_dsp(struct snd_soc_codec *codec, enum hi64xx_pll_type type)
{
	struct hi6405_platform_data *platform_data = snd_soc_codec_get_drvdata(codec);

	if (platform_data == NULL) {
		AUDIO_LOGE("platform data is NULL");
		return;
	}

	if (!platform_data->board_config.pllunlock_to_reset_dsp_enable) {
		AUDIO_LOGI("reset dsp is disable when pll unlock");
		return;
	}

	if (config_pll_test_reg(codec, type) != 0)
		return;

	msleep(100);
	dsp_power_down(codec);
	dump_pll_data(codec);
	hi64xx_wtdog_send_event();
}

static irqreturn_t pll_unlock_handler(int irq, void *data)
{
	struct hi6405_platform_data *platform_data = (struct hi6405_platform_data *)(data);
	struct snd_soc_codec *codec = platform_data->codec;

	AUDIO_LOGW("pll unlock irq received");

	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return IRQ_HANDLED;
	}

	snd_soc_write(codec, IRQ_REG2_REG,
		0x1 << PLL_48K_UNLOCK_F_IRQ_OFFSET);
#ifdef CONFIG_HUAWEI_DSM
	dsm_report(DSM_HI6402_PLL_UNLOCK, "64xx codec pll_unlock\n");
#endif
	reset_codec_dsp(codec, PLL_HIGH);

	return IRQ_HANDLED;
}

static irqreturn_t pll44k1_unlock_handler(int irq, void *data)
{
	struct hi6405_platform_data *platform_data = (struct hi6405_platform_data *)(data);
	struct snd_soc_codec *codec = platform_data->codec;

	AUDIO_LOGW("pll44k1 unlock irq received");

	if (codec == NULL) {
		AUDIO_LOGE("codec is NULL");
		return IRQ_HANDLED;
	}

	snd_soc_write(codec, IRQ_REG4_REG,
		0x1 << PLL_44P1K_UNLOCK_F_IRQ_OFFSET);
#ifdef CONFIG_HUAWEI_DSM
	dsm_report(DSM_HI6402_PLL_UNLOCK, "64xx codec pll44k1_unlock\n");
#endif
	reset_codec_dsp(codec, PLL_44_1);

	return IRQ_HANDLED;
}

static irqreturn_t pllmad_unlock_handler(int irq, void *data)
{
	irq_handler("64xx codec pllmad_unlock", IRQ_REG4_REG,
		PLL_MAD_UNLOCK_F_IRQ_OFFSET, DSM_HI6402_PLL_UNLOCK, data);
	return IRQ_HANDLED;
}

static const struct irq_config codec_irqs[] = {
	{ HI6405_IRQ_BUNK1_OCP, bunk1_ocp_handler, "bunk1_ocp", 1 },
	{ HI6405_IRQ_BUNK1_SCP, bunk1_scp_handler, "bunk1_scp", 1 },
	{ HI6405_IRQ_CP1_SHORT, cp1_short_handler, "cp1_short", 1 },
	{ HI6405_IRQ_CP2_SHORT, cp2_short_handler, "cp2_short", 1 },
	{ HI6405_IRQ_LDO_AVDD18_OCP, ldo_avdd18_ocp_handler, "ldo_avdd18_ocp", 1 },
	{ HI6405_IRQ_LDOP_OCP, ldop_ocp_handler, "ldop_ocp", 1 },
	{ HI6405_IRQ_LDON_OCP, ldon_ocp_handler, "ldon_ocp", 1 },
	{ HI6405_IRQ_PLL_UNLOCK, pll_unlock_handler, "pll_unlock", 0 },
	{ HI6405_IRQ_PLL44K1_UNLOCK, pll44k1_unlock_handler, "pll44k1_unlock", 0 },
	{ HI6405_IRQ_PLLMAD_UNLOCK, pllmad_unlock_handler, "pllmad_unlock", 0 },
};

static void codec_free_irq(struct hi6405_platform_data *platform_data)
{
	size_t len = ARRAY_SIZE(codec_irqs);
	unsigned int i;

	for (i = 0; i < len; i++)
		hi64xx_irq_free_irq(platform_data->irqmgr,
			codec_irqs[i].type, platform_data);
}

static int codec_request_irq(struct hi6405_platform_data *platform_data)
{
	size_t len = ARRAY_SIZE(codec_irqs);
	int ret = 0;
	unsigned int i, j;

	for (i = 0; i < len; i++) {
		ret = hi64xx_irq_request_irq(platform_data->irqmgr,
			codec_irqs[i].type, codec_irqs[i].handler,
			codec_irqs[i].name, platform_data);
		if (ret != 0) {
			AUDIO_LOGE("request irq failed, irq type is %d, irq name is %s",
				codec_irqs[i].type, codec_irqs[i].name);
			for (j = 0; j < i; j++) {
				hi64xx_irq_free_irq(platform_data->irqmgr,
					codec_irqs[i - 1 - j].type,
					platform_data);
			}
			break;
		}

		if (!codec_irqs[i].enabled)
			hi64xx_irq_disable_irq(platform_data->irqmgr,
				codec_irqs[i].type);
	}

	return ret;
}

static int hi6405_platform_probe(struct platform_device *pdev)
{
	int ret;
	unsigned int temp;
	struct device *dev = &pdev->dev;
	struct hi6405_platform_data *platform_data = devm_kzalloc(dev, sizeof(*platform_data), GFP_KERNEL);

	if (platform_data == NULL) {
		AUDIO_LOGE("malloc platform data failed");
		return -ENOMEM;
	}

	ret = init_platform_data(pdev, platform_data);
	if (ret != 0) {
		AUDIO_LOGE("platform data initialization failed:0x%x", ret);
		goto free_platform_data;
	}

	ret = hi64xx_irq_init_irq(platform_data->irqmgr, &irqs);
	if (ret != 0) {
		AUDIO_LOGE("irq init failed:0x%x", ret);
		goto irq_init_err_exit;
	}

	ret = codec_request_irq(platform_data);
	if (ret != 0) {
		AUDIO_LOGE("request irq failed:0x%x", ret);
		goto irq_request_err_exit;
	}

	ret = hi64xx_compat_init(platform_data->cdc_ctrl, platform_data->irqmgr);
	if (ret != 0) {
		AUDIO_LOGE("compat init failed:0x%x", ret);
		goto compat_init_err_exit;
	}

	if (!of_property_read_u32(dev->of_node, "hisilicon,codec_type", &temp)) {
		if (temp == CODEC_TYPE_PC) {
			AUDIO_LOGI("pc type, audio and bt dai");
			ret = snd_soc_register_codec(dev, &hi6405_codec_driver, hi6405_dai_pc, ARRAY_SIZE(hi6405_dai_pc));
		} else {
			AUDIO_LOGI("unknown type, use default dai type");
			ret = snd_soc_register_codec(dev, &hi6405_codec_driver, hi6405_dai, ARRAY_SIZE(hi6405_dai));
		}
	}
	else {
		AUDIO_LOGI("default type");
		ret = snd_soc_register_codec(dev, &hi6405_codec_driver, hi6405_dai, ARRAY_SIZE(hi6405_dai));
	}


	if (ret != 0) {
		AUDIO_LOGE("registe driver failed:0x%x", ret);
		goto codec_register_err_exit;
	}

	return ret;

codec_register_err_exit:
	hi64xx_compat_deinit();
compat_init_err_exit:
	codec_free_irq(platform_data);
irq_request_err_exit:
	hi64xx_irq_deinit_irq(platform_data->irqmgr);
irq_init_err_exit:
	deinit_platform_data(platform_data);
free_platform_data:
	if (platform_data != NULL)
		devm_kfree(dev, platform_data);

	AUDIO_LOGE("platform probe init failed");

	return ret;
}

static int hi6405_platform_remove(struct platform_device *pdev)
{
	struct hi6405_platform_data *platform_data = platform_get_drvdata(pdev);

#ifdef CONFIG_HAC_SUPPORT
	if (gpio_is_valid(platform_data->board_config.hac_gpio))
		gpio_free(platform_data->board_config.hac_gpio);
#endif

	snd_soc_unregister_codec(&pdev->dev);

	hi64xx_compat_deinit();

	codec_free_irq(platform_data);

	hi64xx_irq_deinit_irq(platform_data->irqmgr);

	deinit_platform_data(platform_data);

	return 0;
}

static void hi6405_platform_shutdown(struct platform_device *pdev)
{
	struct hi6405_platform_data *platform_data = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = platform_data->codec;

	if (codec != NULL)
		hi6405_headphone_pop_off(codec);
}

const struct dev_pm_ops hi6405_codec_pm_ops = {
	.suspend = hi6405_codec_suspend,
	.resume = hi6405_codec_resume,
	.thaw = hi6405_codec_resume,
	.freeze = hi6405_codec_freeze,
	.restore = hi6405_codec_restore,
};

static struct platform_driver hi6405_platform_driver = {
	.probe = hi6405_platform_probe,
	.remove = hi6405_platform_remove,
	.shutdown = hi6405_platform_shutdown,
	.driver = {
		.owner = THIS_MODULE,
		.name = DAI_LINK_CODEC_NAME,
		.of_match_table = of_match_ptr(hi6405_platform_match),
		.pm = &hi6405_codec_pm_ops,
	},
};

static int __init da_combine_v5_platform_init(void)
{
	return platform_driver_register(&hi6405_platform_driver);
}
module_init(da_combine_v5_platform_init);

static void __exit da_combine_v5_platform_exit(void)
{
	platform_driver_unregister(&hi6405_platform_driver);
}
module_exit(da_combine_v5_platform_exit);

MODULE_DESCRIPTION("ASoC hi6405 codec driver");
MODULE_LICENSE("GPL");

