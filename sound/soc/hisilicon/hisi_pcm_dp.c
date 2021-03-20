/*
 * hisi_pcm_dp.c -- ALSA SoC HISI PCM DP driver
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define LOG_TAG "hisi_pcm_dp"

#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/dmaengine_pcm.h>
#include <sound/soc.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/hisi/audio_log.h>

#include "hisi_pcm_dp.h"
#include "asp_hdmi_dma.h"
#include "asp_cfg.h"
#include "hifi_lpp.h"

#define HIFI_DP_SAMPLE_RATE_NUM	13

static struct mutex substream_mutex;
static struct snd_pcm_substream *hisi_pcm_dp_substream;

/*lint -e429 -e647 -e574 -e570*/
static const unsigned int freq[HIFI_DP_SAMPLE_RATE_NUM][2] = {
	{8000,   SAMPLE_RATE_NO_SUPPORT},
	{11025, SAMPLE_RATE_NO_SUPPORT},
	{12000, SAMPLE_RATE_NO_SUPPORT},
	{16000, SAMPLE_RATE_NO_SUPPORT},
	{22050, SAMPLE_RATE_NO_SUPPORT},
	{24000, SAMPLE_RATE_NO_SUPPORT},
	{32000, SAMPLE_RATE_NO_SUPPORT},
	{44100, SAMPLE_RATE_NO_SUPPORT},
	{48000, SAMPLE_RATE_48},
	{88200, SAMPLE_RATE_NO_SUPPORT},
	{96000, SAMPLE_RATE_96},
	{176400, SAMPLE_RATE_NO_SUPPORT},
	{192000, SAMPLE_RATE_192}
};

static const unsigned int div_clk[SAMPLE_RATE_MAX][2] = {
	{SAMPLE_RATE_32, HISI_DIV_CLK_NO_SUPPORT},
	{SAMPLE_RATE_44, HISI_DIV_CLK_NO_SUPPORT},
	{SAMPLE_RATE_48, HISI_DIV_CLK_48},
	{SAMPLE_RATE_88, HISI_DIV_CLK_NO_SUPPORT},
	{SAMPLE_RATE_96, HISI_DIV_CLK_96},
	{SAMPLE_RATE_176, HISI_DIV_CLK_NO_SUPPORT},
	{SAMPLE_RATE_192, HISI_DIV_CLK_192}
};

static struct snd_pcm_hardware hisi_pcm_dp_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S16_BE |
				  SNDRV_PCM_FMTBIT_S24_LE |
				  SNDRV_PCM_FMTBIT_S24_BE |
				  SNDRV_PCM_FMTBIT_S32_LE |
				  SNDRV_PCM_FMTBIT_S32_BE,
	.period_bytes_min	= 32,
	.period_bytes_max	= 256 * 1024,
	.periods_min		= 2,
	.periods_max		= 32,
	.buffer_bytes_max	= 256 * 1024,
};

static const struct of_device_id hisi_pcm_dp_match[] = {
	{
		.compatible = "hisilicon,hisi-pcm-dp",
	},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_pcm_dp_match);

/*****************************************************************************
  function declare
 *****************************************************************************/
extern int mailbox_get_timestamp(void);

static int set_tx3_parameters(struct snd_pcm_hw_params *params,
			struct tx3_config_parameters *tx3_parameters)
{
	unsigned int params_value = 0;

	/* CHECK SUPPORT CHANNELS : mono or stereo or 8ch */
	params_value = params_channels(params);
	AUDIO_LOGI("set channel num: %d", params_value);
	switch (params_value) {
	case 1:
		tx3_parameters->channel_num = CHANNEL_NUM_1;
		break;
	case 2:
		tx3_parameters->channel_num = CHANNEL_NUM_2;
		break;
	case 8:
		tx3_parameters->channel_num = CHANNEL_NUM_8;
		break;
	default:
		AUDIO_LOGE("DAC not support %d channels", params_value);
		return -EINVAL;
	}

	params_value = params_format(params);
	AUDIO_LOGI("set format: %d", params_value);
	switch (params_value) {
	case SNDRV_PCM_FORMAT_S16_LE:
		tx3_parameters->bit_width  = BIT_WIDTH_16;
		tx3_parameters->align_type = ALIGN_16;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		tx3_parameters->bit_width  = BIT_WIDTH_24;
		tx3_parameters->align_type = ALIGN_32;
		break;
	/* use align_32 to avoid Channel interchange problem*/
	case SNDRV_PCM_FORMAT_S32_LE:
		tx3_parameters->bit_width  = BIT_WIDTH_16;
		tx3_parameters->align_type = ALIGN_32;
		break;
	default:
		AUDIO_LOGE("format err: %d", params_value);
		return -EINVAL;
	}

	return 0;

}

static int set_sio_parameters(struct snd_pcm_hw_params *params,
			struct sio_config_parameters *sio_parameters)
{
	unsigned int params_value = 0;
	unsigned int infreq_index = 0;
	unsigned int clk_index = 0;

	params_value = params_rate(params);
	AUDIO_LOGI("set rate: %d", params_value);

	for (infreq_index = 0; infreq_index < HIFI_DP_SAMPLE_RATE_NUM; infreq_index++) {
		if (params_value == freq[infreq_index][0])
			break;
	}
	if (infreq_index >= HIFI_DP_SAMPLE_RATE_NUM) {
		AUDIO_LOGE("set rate: %d error", params_value);
		return -EINVAL;
	}

	sio_parameters->sample_rate = freq[infreq_index][1];
	AUDIO_LOGI("sample rate[%d]: %d", sio_parameters->sample_rate, freq[infreq_index][0]);

	for (clk_index = 0; clk_index < SAMPLE_RATE_MAX; clk_index++) {
		if (sio_parameters->sample_rate == div_clk[clk_index][0])
			break;
	}
	if (clk_index >= SAMPLE_RATE_MAX) {
		AUDIO_LOGE("set clk: %d error", sio_parameters->sample_rate);
		return -EINVAL;
	}

	sio_parameters->div_clk = div_clk[clk_index][1];
	AUDIO_LOGI("div clk: 0x%x", sio_parameters->div_clk);

	return 0;

}

static int dp_clk_enable(struct hisi_dp_data *prtd)
{
	struct clk *dp_audio_pll_clk = NULL;
	struct clk *asp_subsys_clk = NULL;
	int ret = -1;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is NULL Pointer");
		return -EINVAL;
	}

	asp_subsys_clk = prtd->asp_subsys_clk;
	if (asp_subsys_clk) {
		ret = clk_prepare_enable(asp_subsys_clk);
		if (ret) {
			AUDIO_LOGE("asp_subsys_clk enable fail, error: %d", ret);
			return -EFAULT;
		}
	} else {
		AUDIO_LOGE("asp_subsys_clk is null");
	}

	dp_audio_pll_clk = prtd->dp_audio_pll_clk;
	if (dp_audio_pll_clk) {
		// TODO:Fixed support 48k series, if support 44.1k series, need to be modified
		ret = clk_set_rate(dp_audio_pll_clk, HISI_PLL6_RATE_48);
		AUDIO_LOGI("set pll6 rate: %d", HISI_PLL6_RATE_48);
		if (ret) {
			AUDIO_LOGE("set pll6 rate failed. ret: %d", ret);
			clk_disable_unprepare(asp_subsys_clk);
			return ret;
		}

		ret = clk_prepare_enable(dp_audio_pll_clk);
		if (ret) {
			AUDIO_LOGE("clk_dp_audio_pll enable fail, error: %d", ret);
			clk_disable_unprepare(asp_subsys_clk);
			return -EINVAL;
		}
	} else {
		AUDIO_LOGE("dp_audio_pll_clk is null");
	}

	return 0;
}

static int dp_clk_disable(struct hisi_dp_data *prtd)
{
	struct clk *dp_audio_pll_clk = NULL;
	struct clk *asp_subsys_clk = NULL;
	int ret;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is NULL Pointer");
		return -EINVAL;
	}

	dp_audio_pll_clk = prtd->dp_audio_pll_clk;
	if (dp_audio_pll_clk) {
		ret = clk_set_rate(dp_audio_pll_clk, HISI_PLL6_RATE_DEFAULT);
		if (ret)
			AUDIO_LOGE("set pll6 rate to default value failed. ret: %d", ret);
		clk_disable_unprepare(dp_audio_pll_clk);
	}

	asp_subsys_clk = prtd->asp_subsys_clk;
	if (asp_subsys_clk)
		clk_disable_unprepare(asp_subsys_clk);

	return 0;
}

static irqreturn_t dp_dma_irq_handler(int irq, void *data)
{
	struct snd_pcm *pcm = data;
	struct snd_pcm_substream *substream = NULL;
	struct snd_pcm_runtime *runtime = NULL;
	struct hisi_dp_runtime_data *prtd = NULL;
	struct hisi_dp_data *pdata = NULL;
	unsigned int rt_period_size = 0;
	unsigned int num_periods = 0;
	unsigned int irs = 0;
	snd_pcm_uframes_t avail = 0;
	uint32_t asp_int;
	unsigned int cur_time = 0;
	unsigned int delay_time = 0;

	asp_int = asp_cfg_get_irq_value();
	if (!(asp_int & 0x1))
		return IRQ_HANDLED;
	if (pcm == NULL) {
		AUDIO_LOGE("pcm is null");
		return IRQ_HANDLED;
	}
	substream = pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream;
	if (NULL == substream) {
		AUDIO_LOGE("substream is NULL");
		return IRQ_HANDLED;
	}

	runtime = substream->runtime;
	prtd = runtime->private_data;
	pdata = prtd->pdata;
	rt_period_size = runtime->period_size;
	num_periods = runtime->periods;

	cur_time = (unsigned int)mailbox_get_timestamp();
	delay_time = cur_time - pdata->pre_time;
	pdata->pre_time = cur_time;
	if (delay_time > (HISI_WORK_DELAY_1MS * 30)) {
		AUDIO_LOGE("dp irp timeout: %d", delay_time);
	}

	irs = asp_hdmi_reg_read_irsr();

	if (0 == irs) {
		AUDIO_LOGW("not hdmi dma irq");
		return IRQ_HANDLED;
	}

	if (STATUS_HDMI_STOP == prtd->status) {
		AUDIO_LOGD("stop dma, irs: %#x", irs);
		asp_hdmi_dma_clear_interrupt(irs);
		return IRQ_HANDLED;
	}

	if (0 != ((1 << HDMI_DMA_A) & irs) || 0 != ((1 << HDMI_DMA_B) & irs)) {
		asp_hdmi_dma_clear_interrupt(irs);
	} else {
		AUDIO_LOGE("IRQ_ERROR, irs: %#x", irs);
		asp_hdmi_dma_clear_interrupt(irs);
		return IRQ_HANDLED;
	}

	spin_lock(&prtd->lock);
	prtd->period_cur = (prtd->period_cur + 1) % num_periods;
	spin_unlock(&prtd->lock);

	snd_pcm_period_elapsed(substream);

	spin_lock(&prtd->lock);

	avail = snd_pcm_playback_hw_avail(runtime);

	if (avail < rt_period_size) {
		AUDIO_LOGI("There is no avail data avail(%d)< rt_period_size(%d)",
			(int)avail, rt_period_size);
	}

	asp_hdmi_dma_enable();

	spin_unlock(&prtd->lock);

	return IRQ_HANDLED;
}

int stop_pcm_dp_substream(void)
{
	int ret = 0;

	mutex_lock(&substream_mutex);
	if (hisi_pcm_dp_substream != NULL) {
		ret = snd_pcm_stop_xrun(hisi_pcm_dp_substream);
		AUDIO_LOGI("pcm dp substream stop: %d", ret);
	}
	mutex_unlock(&substream_mutex);

	return ret;
}

static int hisi_pcm_dp_open(struct snd_pcm_substream *substream)
{
	struct hisi_dp_runtime_data *prtd = NULL;
	struct snd_soc_pcm_runtime *rtd = NULL;
	struct hisi_dp_data *pdata = NULL;
	struct snd_pcm *pcm = NULL;
	int ret = 0;

	pcm =  substream->pcm;
	WARN_ON(NULL == pcm);

	AUDIO_LOGI("begin");

	prtd = kzalloc(sizeof(struct hisi_dp_runtime_data), GFP_KERNEL);
	if (NULL == prtd) {
		AUDIO_LOGE("kzalloc hisi_dp_runtime_data error");
		return -ENOMEM;
	}

	mutex_init(&prtd->mutex);
	spin_lock_init(&prtd->lock);

	rtd = (struct snd_soc_pcm_runtime *)substream->private_data;
	pdata = (struct hisi_dp_data *)snd_soc_platform_get_drvdata(rtd->platform);

	WARN_ON(NULL == pdata);

#ifdef CONFIG_PM_RUNTIME
	pm_runtime_get_sync(pdata->dev);
#else
	ret = regulator_bulk_enable(1, &pdata->regu);
	if (0 != ret) {
		AUDIO_LOGE("couldn't enable regulators %d", ret);
		goto err_bulk;
	}
#endif

	prtd->pdata = pdata;

	substream->runtime->private_data = prtd;

	ret = request_irq(pdata->irq, dp_dma_irq_handler,
		IRQF_TRIGGER_HIGH | IRQF_SHARED | IRQF_NO_SUSPEND, "asp_hdmi_dma", pcm);
	if (ret) {
		AUDIO_LOGE("request asp dma irq failed");
		goto err_irq;
	}

	/* enable dp clk */
	ret = dp_clk_enable(pdata);
	if (ret) {
		AUDIO_LOGE("dp_clk_enable failed");
		goto err_clk;
	}

	/* enable hdmi modile*/
	asp_cfg_hdmi_module_enable();

	/* selete clk_audio_pll */
	asp_cfg_hdmi_clk_sel(ASP_CFG_HDMI_CLK_SEL_I2S);

	/* enable_hdmi_interrupeter */
	asp_cfg_enable_hdmi_interrupeter();

	ret = snd_soc_set_runtime_hwparams(substream, &hisi_pcm_dp_hardware);
	if (ret) {
		AUDIO_LOGE("set hwparams failed");
		goto err;
	}

	mutex_lock(&substream_mutex);
	hisi_pcm_dp_substream = substream;
	mutex_unlock(&substream_mutex);

	return ret;

err:
	asp_cfg_disable_hdmi_interrupeter();
	asp_cfg_hdmi_module_disable();
	dp_clk_disable(prtd->pdata);
err_clk:
	free_irq(pdata->irq, pcm);
	mutex_destroy(&prtd->mutex);
err_irq:
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_mark_last_busy(pdata->dev);
	pm_runtime_put_autosuspend(pdata->dev);
#else
	if (regulator_bulk_disable(1, &prtd->pdata->regu))
		AUDIO_LOGE("fail to disable regulator");
err_bulk:
#endif
	kfree(prtd);
	return ret;
}

static int hisi_pcm_dp_close(struct snd_pcm_substream *substream)
{
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	AUDIO_LOGI("begin");

	if (prtd == NULL) {
		AUDIO_LOGE("prtd is NULL");
		return -ENOMEM;
	}

	if (prtd->pdata== NULL) {
		AUDIO_LOGE("pdata is NULL");
		return -ENOMEM;
	}

	free_irq(prtd->pdata->irq, substream->pcm);

	/* disable hdmi_interrupeter */
	asp_cfg_disable_hdmi_interrupeter();

	/* disable tx3 */
	asp_hdmi_tx3_disable();

	/* disable hdmi module*/
	asp_cfg_hdmi_module_disable();

	/* disable dp clk */
	dp_clk_disable(prtd->pdata);

#ifdef CONFIG_PM_RUNTIME
	pm_runtime_mark_last_busy(prtd->pdata->dev);
	pm_runtime_put_autosuspend(prtd->pdata->dev);
#else
	ret = regulator_bulk_disable(1, &prtd->pdata->regu);
	if (ret)
		AUDIO_LOGE("fail to disable regulator, ret: %d", ret);
#endif

	kfree(prtd);
	substream->runtime->private_data = NULL;

	mutex_lock(&substream_mutex);
	hisi_pcm_dp_substream = NULL;
	mutex_unlock(&substream_mutex);

	return 0;
}

static int hisi_pcm_dp_hw_params(struct snd_pcm_substream *substream,
					 struct snd_pcm_hw_params *params)
{
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;
	unsigned long bytes = params_buffer_bytes(params);
	int ret = 0;
	struct tx3_config_parameters tx3_parameters = {CHANNEL_NUM_2, BIT_WIDTH_16, ALIGN_16};
	struct sio_config_parameters sio_parameters = {&tx3_parameters,  SAMPLE_RATE_48, HISI_DIV_CLK_48, false};

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	AUDIO_LOGI("begin");

	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		ret = snd_pcm_lib_malloc_pages(substream, bytes);
		if (0 > ret) {
			AUDIO_LOGE("malloc pages failed. ret: %d", ret);
			return ret;
		}

		ret = set_tx3_parameters(params, &tx3_parameters);
		if (ret) {
			AUDIO_LOGE("set tx3 parameters failed. ret: %d", ret);
			goto err_out;
		}

		ret = set_sio_parameters(params, &sio_parameters);
		if (ret) {
			AUDIO_LOGE("set sio parameters failed. ret: %d", ret);
			goto err_out;
		}

		if (sio_parameters.div_clk != HISI_DIV_CLK_NO_SUPPORT) {
			asp_cfg_div_clk(sio_parameters.div_clk);
		} else {
			AUDIO_LOGE("div_clk(%d) is no support,and will use defaulte div_clk config",
				sio_parameters.div_clk);
		}

		mutex_lock(&prtd->mutex);
		prtd->period_size = params_period_bytes(params);
		mutex_unlock(&prtd->mutex);

		ret = asp_hdmi_tx3_config(tx3_parameters);
		if (ret) {
			AUDIO_LOGE("tx3 config failed. ret: %d", ret);
			goto err_out;
		}

		asp_hdmi_io_config(sio_parameters);

		return 0;
err_out:
		AUDIO_LOGE("hw params error, ret: %d", ret);
		snd_pcm_lib_free_pages(substream);
	}

	return ret;

}

static int hisi_pcm_dp_hw_free(struct snd_pcm_substream *substream)
{
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		/*clear interrupt*/
		asp_hdmi_dma_clear_interrupt(HDMI_IRQ_MASK);

		ret = snd_pcm_lib_free_pages(substream);
	}

	return ret;
}

static int hisi_pcm_dp_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}
	AUDIO_LOGI("mode %d hisi_pcm_dp_prepare", substream->stream);

	spin_lock(&prtd->lock);
	prtd->status = STATUS_HDMI_STOP;
	prtd->period_next = 0;
	prtd->period_cur = 0;
	prtd->dma_addr = (unsigned int)((unsigned long)runtime->dma_addr);
	spin_unlock(&prtd->lock);

	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		/* config DMA CH_A   CH_B*/
		asp_hdmi_dma_config(prtd->dma_addr, prtd->period_size);

		/*clear all interrupt*/
		asp_hdmi_dma_clear_interrupt(HDMI_IRQ_MASK);
	}

	return 0;
}

static int hisi_pcm_dp_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	AUDIO_LOGI("mode %d trigger %d", substream->stream, cmd);
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
			prtd->status = STATUS_HDMI_RUNNING;
			prtd->pdata->pre_time = (unsigned int)mailbox_get_timestamp();
			/*enable dp*/
			asp_cfg_dp_module_enable();
			/*tx3 enable*/
			asp_hdmi_tx3_enable();
			/*dma start*/
			asp_hdmi_dma_start();
		}
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
			prtd->status = STATUS_HDMI_STOP;
			/*dma stop*/
			asp_hdmi_dma_stop();
			/*tx3 disable*/
			asp_hdmi_tx3_disable();
			/*disable dp*/
			asp_cfg_dp_module_disable();
		}
		break;

	default:
		AUDIO_LOGE("cmd is error and trigger cmd: %d", cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static snd_pcm_uframes_t hisi_pcm_dp_pointer(struct snd_pcm_substream *substream)
{
	long frame = 0L;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct hisi_dp_runtime_data *prtd = substream->runtime->private_data;

	unsigned int period_cur = 0;
	unsigned int period_size = 0;

	if (NULL == prtd) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	spin_lock(&prtd->lock);
	period_cur = prtd->period_cur;
	period_size = prtd->period_size;
	spin_unlock(&prtd->lock);

	frame = bytes_to_frames(runtime, period_cur * period_size);
	if (frame >= runtime->buffer_size)
		frame = 0;

	return (snd_pcm_uframes_t)frame;
}

static int hisi_dp_normal_mmap(struct snd_pcm_substream *substream,
				 struct vm_area_struct *vma)
{
	int ret = 0;
	struct snd_pcm_runtime *runtime = NULL;

	runtime = substream->runtime;
	if (NULL == runtime) {
		AUDIO_LOGE("runtime is invalid");
		return -ENOMEM;
	}

	ret = dma_mmap_writecombine(substream->pcm->card->dev,
				vma,
				runtime->dma_area,
				runtime->dma_addr,
				runtime->dma_bytes);

	return ret;
}

int hisi_pcm_preallocate_dma_buffer(struct snd_pcm *pcm, int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = NULL;
	size_t size = hisi_pcm_dp_hardware.buffer_bytes_max;

	if (NULL == substream) {
		AUDIO_LOGE("substream is NULL");
		return  -ENOMEM;
	}

	buf = &substream->dma_buffer;

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;
	buf->addr = HISI_DP_AUDIO_DATA_LOCATION;

	buf->area = ioremap_wc(buf->addr, size);

	if (!buf->area) {
		AUDIO_LOGE("dma_alloc_combine error");
		return -ENOMEM;
	}

	buf->bytes = size;

	return 0;
}

void hisi_pcm_free_dma_buffers(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream = NULL;
	struct snd_dma_buffer *buf = NULL;
	int stream;

	for (stream = 0; stream < 2; stream++) {
		substream = pcm->streams[stream].substream;
		if (!substream)
			continue;

		buf = &substream->dma_buffer;
		if (!buf->area)
			continue;

		iounmap(buf->area);

		buf->area = NULL;
		buf->addr = 0;
	}
}

static struct snd_pcm_ops hisi_pcm_dp_ops = {
	.open		= hisi_pcm_dp_open,
	.close		= hisi_pcm_dp_close,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= hisi_pcm_dp_hw_params,
	.hw_free	= hisi_pcm_dp_hw_free,
	.prepare	= hisi_pcm_dp_prepare,
	.trigger	= hisi_pcm_dp_trigger,
	.pointer	= hisi_pcm_dp_pointer,
	.mmap	= hisi_dp_normal_mmap,
};

static unsigned long long hisi_pcm_dmamask = DMA_BIT_MASK(32);

static int hisi_pcm_dp_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm  *pcm  = rtd->pcm;
	int ret = 0;

	if (pcm == NULL) {
		AUDIO_LOGE("pcm is null");
		return -EINVAL;
	}

	if(!card || !card->dev)
	{
		AUDIO_LOGE("pcm_runtime has no card or card->dev is null");
		return -ENOENT;
	}

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &hisi_pcm_dmamask;
	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
		ret = hisi_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_PLAYBACK);
		if (ret) {
			AUDIO_LOGE("preallocate dma buffer error, error No.: %d", ret);
			return ret;
		}
	}

	return ret;
}

static void hisi_pcm_dp_free(struct snd_pcm *pcm)
{
	if (pcm == NULL) {
		AUDIO_LOGE("pcm is null");
		return;
	}

	hisi_pcm_free_dma_buffers(pcm);
}

static struct snd_soc_platform_driver hisi_pcm_dp_platform = {
	.ops		= &hisi_pcm_dp_ops,
	.pcm_new	= hisi_pcm_dp_new,
	.pcm_free	= hisi_pcm_dp_free,
};

static int hisi_pcm_dp_probe(struct platform_device *pdev)
{
	int ret = -1;
	struct device *dev = &pdev->dev;
	struct hisi_dp_data *pdata = NULL;
	struct device_node *node = pdev->dev.of_node;

	if (!dev) {
		AUDIO_LOGE("platform_device has no device");
		return -ENOENT;
	}

	AUDIO_LOGI("probe begin");

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		AUDIO_LOGE("cannot allocate hisi pcm dp platform data");
		return -ENOMEM;
	}

	pdata->regu.supply = "dp-pcm";
	ret = devm_regulator_bulk_get(dev, 1, &(pdata->regu));
	if (0 != ret) {
		AUDIO_LOGE("couldn't get regulators %d", ret);
		return -ENOMEM;
	}

	pdata->dev = dev;

	pdata->dp_audio_pll_clk = devm_clk_get(dev, "clk_dp_audio_pll");
	if (IS_ERR_OR_NULL(pdata->dp_audio_pll_clk)) {
		AUDIO_LOGE("devm_clk_get: dp_audio_pll_clk not found");
		return -EINVAL;
	}

	pdata->asp_subsys_clk = devm_clk_get(dev, "clk_asp_subsys");
	if (IS_ERR_OR_NULL(pdata->asp_subsys_clk)) {
		AUDIO_LOGE("devm_clk_get: clk_asp_subsys not found");
		return -EFAULT;
	}

	pdata->irq = platform_get_irq_byname(pdev, "asp_hdmi_dma");
	if (0 > pdata->irq) {
		AUDIO_LOGE("cannot get irq");
		return -ENOENT;
	}

	if (of_property_read_bool(node, "no_support_mmap"))
		hisi_pcm_dp_hardware.info = SNDRV_PCM_INFO_INTERLEAVED;

	AUDIO_LOGI("pcm dp hardware info: %u", hisi_pcm_dp_hardware.info);

#ifdef CONFIG_PM_RUNTIME
	pm_runtime_set_autosuspend_delay(dev, 100); /* 100ms*/
	pm_runtime_use_autosuspend(dev);

	pm_runtime_enable(dev);
#endif

	platform_set_drvdata(pdev, pdata);

	dev_set_name(dev, "hisi-pcm-dp");

	ret = snd_soc_register_platform(dev, &hisi_pcm_dp_platform);
	if (ret) {
		AUDIO_LOGE("snd_soc_register_platform return %d", ret);
		goto reg_platform_failed;
	}

	mutex_init(&substream_mutex);

	AUDIO_LOGI("probe end");

	return 0;

reg_platform_failed:
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_disable(&pdev->dev);
#endif

	return ret;
}

static int hisi_pcm_dp_remove(struct platform_device *pdev)
{
	mutex_destroy(&substream_mutex);

	snd_soc_unregister_platform(&pdev->dev);
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_disable(&pdev->dev);
#endif

	return 0;
}

#ifdef CONFIG_PM_RUNTIME
int hisi_dp_runtime_suspend(struct device *dev)
{
	struct hisi_dp_data *pdata = dev_get_drvdata(dev);
	int ret = 0;

	WARN_ON(NULL == pdata);

	AUDIO_LOGI("begin");

	ret = regulator_bulk_disable(1, &pdata->regu);
	if (ret)
		AUDIO_LOGE("fail to disable regulator, ret: %d", ret);

	AUDIO_LOGI("end");

	return 0;
}

int hisi_dp_runtime_resume(struct device *dev)
{
	struct hisi_dp_data *pdata = dev_get_drvdata(dev);
	int ret;

	WARN_ON(NULL == pdata);

	AUDIO_LOGI("begin");

	ret = regulator_bulk_enable(1, &pdata->regu);
	if (0 != ret)
		AUDIO_LOGE("couldn't enable regulators %d", ret);

	AUDIO_LOGI("end");

	return ret;
}

const struct dev_pm_ops hisi_dp_pm_ops = {
	.runtime_suspend		= hisi_dp_runtime_suspend,
	.runtime_resume		= hisi_dp_runtime_resume,
};
#endif

static struct platform_driver hisi_pcm_dp_driver = {
	.probe	= hisi_pcm_dp_probe,
	.remove	= hisi_pcm_dp_remove,
	.driver	= {
		.owner	= THIS_MODULE,
		.name	= "hisi-pcm-dp",
		.of_match_table = of_match_ptr(hisi_pcm_dp_match),
#ifdef CONFIG_PM_RUNTIME
		.pm	= &hisi_dp_pm_ops,
#endif
	},
};

module_platform_driver(hisi_pcm_dp_driver);

MODULE_DESCRIPTION("ASoC hisi pcm dp driver");
MODULE_AUTHOR("wushengyang <wushengyang1@hisilicon.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:hisi_pcm_dp");
