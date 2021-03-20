/*
 * hisi_pcm_codec.c
 *
 * hisi pcm driver
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
#include "hisi_pcm_codec.h"

#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/regulator/consumer.h>
#include <linux/types.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <linux/hisi/audio_log.h>

#include "asp_dma.h"
#include "platform_io.h"
#include "format.h"
#include "om.h"

#ifdef LLT_TEST
#define static_t
#else
#define static_t static
#endif

#ifdef CONFIG_HISI_AUDIO_DEBUG
#define LOG_TAG "hisi_pcm_codec"
#else
#define LOG_TAG "hisi_pcm"
#endif

#define RECORD_MUTE_MAX_CNT 10
#define IRQ_PERIOD_LOW_THRESHOLD 4
#define IRQ_PERIOD_HIGH_THRESHOLD 2
#define DMA_LLI_ENABLE_BITS 2
#define DMA_LLI_NUM 2
#define MS2S_FACTOR 1000
#define US2S_FACTOR 1000000
#define MAX_STREAM_TYPES (SNDRV_PCM_STREAM_LAST + 1)
#define SHORT_BITS 16
#define INTERRUPT_INTERVAL 20

typedef void (*substream_op)(struct snd_pcm_substream *);

struct platform_dev_data {
	struct regulator *asp_regulator;
	enum io_type io_type_cfg;
};

struct pcm_runtime_data {
	struct platform_dev_data *pdata;
	bool is_dma_running;
	unsigned int sample_rate;
	unsigned int period_size;
	unsigned int period_cur;
	unsigned int frames;
	unsigned int channels;
	unsigned int dma_buf_next[PCM_PORTS_MAX];
	unsigned int irq_cnt;
	unsigned long pre_period_ts;
	unsigned int record_mute_cnt;
	struct pcm_ports ports;
	struct dma_config dma_cfg[PCM_PORTS_MAX];
	struct dma_lli_cfg *pdma_lli_cfg[PCM_PORTS_MAX];
	unsigned int lli_dma_addr[PCM_PORTS_MAX];
	unsigned int dma_period_size;
};

static_t struct cust_priv *priv_cust;
static_t struct snd_pcm_substream *pcm_streams[PCM_DEVICE_TOTAL][MAX_STREAM_TYPES];
static_t struct mutex exter_interface_mutex;
static_t struct mutex prtd_mutex[MAX_STREAM_TYPES];

static_t const struct of_device_id hisi_pcm_codec_of_match[] = {
	{
		.compatible = "hisilicon,hisi-pcm-codec",
	},
	{ },
};
MODULE_DEVICE_TABLE(of, hisi_pcm_codec_of_match);

static const struct snd_soc_component_driver hisi_pcm_codec_component = {
	.name   = "hi3xxx-pcm-asp",
};

void hisi_pcm_codec_register_cust_interface(struct cust_priv *cust)
{
	priv_cust = cust;
}

static_t const struct snd_pcm_hardware *_get_hw_config(int device, int stream)
{
	const struct snd_pcm_hardware *pcm_hardware_cfg = NULL;

	if (priv_cust != NULL && priv_cust->get_pcm_hw_config != NULL)
		pcm_hardware_cfg = priv_cust->get_pcm_hw_config(device, stream);

	return pcm_hardware_cfg;
}

static_t unsigned int _get_hw_buffer_bytes_max(void)
{
	int device, stream;
	unsigned int ret = 0;
	const struct snd_pcm_hardware *pcm_hardware_cfg = NULL;

	for (device = 0; device < PCM_DEVICE_TOTAL; device++) {
		for (stream = 0; stream < PCM_STREAM_MAX; stream++) {
			pcm_hardware_cfg = _get_hw_config(device, stream);
			if (pcm_hardware_cfg != NULL)
				ret = pcm_hardware_cfg->buffer_bytes_max > ret ?
					pcm_hardware_cfg->buffer_bytes_max : ret;
		}
	}

	return ret;
}

static_t void _enum_pcm_streams(substream_op operation)
{
	int stream, device;

	for (device = PCM_DEVICE_NORMAL; device < PCM_DEVICE_TOTAL; device++) {
		for (stream = SNDRV_PCM_STREAM_PLAYBACK; stream < MAX_STREAM_TYPES; stream++) {
			if (pcm_streams[device][stream] == NULL) {
				AUDIO_LOGI("device: %d, stream: %d, substream is NULL", device, stream);
				continue;
			}
			operation(pcm_streams[device][stream]);
		}
	}
}

static_t int _get_dma_cfg(struct pcm_runtime_data *prtd)
{
	unsigned int port_index;
	const struct pipe *pipe = get_pipe(prtd->ports.pipe_type_cfg,
		prtd->pdata->io_type_cfg);

	if (pipe == NULL) {
		prtd->ports.ports_cnt = 0;
		AUDIO_LOGE("get pipe failed");
		return -EINVAL;
	}

	for (port_index = 0; port_index < pipe->ports_cnt; port_index++) {
		prtd->dma_cfg[port_index].port = pipe->ports[port_index]->port;
		prtd->dma_cfg[port_index].config = pipe->ports[port_index]->config;
		prtd->dma_cfg[port_index].channel = pipe->ports[port_index]->channel;

		AUDIO_LOGI("dma port: 0x%x, configs: 0x%x, channels: %u",
			prtd->dma_cfg[port_index].port,
			prtd->dma_cfg[port_index].config,
			prtd->dma_cfg[port_index].channel);
	}

	return 0;
}

static_t unsigned int _calc_dma_acount(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct pcm_runtime_data *prtd = runtime->private_data;
	unsigned int bytes;

	if (prtd->pdata->io_type_cfg == IO_FIFO_INTERLACED)
		bytes = INTERLACED_DMA_BYTES;
	else
		bytes = SLIMBUS_DMA_BYTES;

	return runtime->rate * INTERRUPT_INTERVAL * bytes / MS2S_FACTOR;
}

static_t void _set_lli(struct snd_pcm_substream *substream, unsigned int port_index,
	unsigned int lli_index, unsigned int bit24_dma_size)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	struct dma_lli_cfg *plli = &prtd->pdma_lli_cfg[port_index][lli_index];
	unsigned int dma_addr_offset = prtd->dma_buf_next[port_index] * bit24_dma_size;
	struct pcm_dma_buf_config *dma_buf_cfg = NULL;

	dma_buf_cfg = get_dma_buf_cfg(substream->pcm->device, substream->stream);
	if (dma_buf_cfg == NULL) {
		AUDIO_LOGE("get dma buf cfg failed, device: %d, stream: %d",
			substream->pcm->device, substream->stream);
		return;
	}

	plli->lli = (unsigned int)(prtd->lli_dma_addr[port_index] +
		sizeof(struct dma_lli_cfg) * (lli_index + 1)) | DMA_LLI_ENABLE_BITS;
	plli->config = prtd->dma_cfg[port_index].config;
	plli->a_count = bit24_dma_size;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		plli->des_addr = prtd->dma_cfg[port_index].port;
		plli->src_addr = dma_buf_cfg->phy_addr + dma_addr_offset;
	} else {
		plli->des_addr = dma_buf_cfg->phy_addr + dma_addr_offset;
		plli->src_addr = prtd->dma_cfg[port_index].port;
	}
}

static_t void _dma_lli_cfg(struct snd_pcm_substream *substream)
{
	unsigned int port_index;
	unsigned int lli_index;
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	unsigned int ports_cnt = prtd->ports.ports_cnt;
	unsigned int bit24_dma_size = _calc_dma_acount(substream);

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		memset(prtd->pdma_lli_cfg[port_index], 0, DMA_LLI_NUM * sizeof(struct dma_lli_cfg));

		for (lli_index = 0; lli_index < DMA_LLI_NUM; lli_index++) {
			_set_lli(substream, port_index, lli_index, bit24_dma_size);

			prtd->dma_buf_next[port_index] = (prtd->dma_buf_next[port_index] + ports_cnt) %
				(substream->runtime->periods * ports_cnt);
		}

		prtd->pdma_lli_cfg[port_index][DMA_LLI_NUM - 1].lli = prtd->lli_dma_addr[port_index] | DMA_LLI_ENABLE_BITS;

		for (lli_index = 0; lli_index < DMA_LLI_NUM; lli_index++)
			AUDIO_LOGI("port index: %u, node index: %u, lli: 0x%x, count: %d, src: 0x%pK, des: 0x%pK, configs: 0x%x",
				port_index, lli_index,
				prtd->pdma_lli_cfg[port_index][lli_index].lli,
				prtd->pdma_lli_cfg[port_index][lli_index].a_count,
				(void *)(uintptr_t)(prtd->pdma_lli_cfg[port_index][lli_index].src_addr),
				(void *)(uintptr_t)(prtd->pdma_lli_cfg[port_index][lli_index].des_addr),
				prtd->pdma_lli_cfg[port_index][lli_index].config);
	}
}

static_t int _get_port_index(struct pcm_runtime_data *prtd, int channel)
{
	int port_index;
	int ports_cnt = prtd->ports.ports_cnt;

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		if (prtd->dma_cfg[port_index].channel == channel) {
			AUDIO_LOGD("dma port index: %d, channel: %d", port_index, channel);
			return port_index;
		}
	}
	AUDIO_LOGE("channel number %d is invalid", channel);
	return -EINVAL;
}

static_t unsigned int _get_idle_buffer_id(struct snd_pcm_substream *substream, int channel)
{
	unsigned int used_dma_addr;
	unsigned int dma_buf_addr;
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	int cur_channel_port_index = _get_port_index(prtd, channel);

	if (cur_channel_port_index == -EINVAL)
		return PCM_SWAP_BUFF_A;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		used_dma_addr = asp_dma_get_src(channel);
		dma_buf_addr = prtd->pdma_lli_cfg[cur_channel_port_index][PCM_SWAP_BUFF_A].src_addr;
	} else {
		used_dma_addr = asp_dma_get_des(channel);
		dma_buf_addr = prtd->pdma_lli_cfg[cur_channel_port_index][PCM_SWAP_BUFF_A].des_addr;
	}

	AUDIO_LOGD("stream type: %d, used dma address: 0x%pK, dma buf address: 0x%pK",
		substream->stream, used_dma_addr, dma_buf_addr);

	if (used_dma_addr == dma_buf_addr)
		return PCM_SWAP_BUFF_B;
	else
		return PCM_SWAP_BUFF_A;
}

static_t bool _is_irq_valid(struct snd_pcm_runtime *runtime, int int_type)
{
	struct pcm_runtime_data *prtd = runtime->private_data;

	if (int_type != ASP_DMA_INT_TYPE_TC1 && int_type != ASP_DMA_INT_TYPE_TC2) {
		AUDIO_LOGE("dma irq is invalid, interrupt type: %d", int_type);
		return false;
	}

	if (!prtd->is_dma_running) {
		AUDIO_LOGE("dma is not running");
		return false;
	}

	return true;
}

static_t bool _is_period_valid(struct snd_pcm_runtime *runtime)
{
	struct pcm_runtime_data *prtd = runtime->private_data;
	unsigned long curr_period_time_us;
	unsigned int period_interval_ms;
	unsigned int normal_interval;

	if (prtd->pre_period_ts == 0) {
		prtd->pre_period_ts = hisi_getcurtime();
		return true;
	}

	normal_interval = (runtime->period_size * MS2S_FACTOR) / prtd->sample_rate;
	curr_period_time_us = hisi_getcurtime();
	period_interval_ms = (curr_period_time_us - prtd->pre_period_ts) / US2S_FACTOR;
	prtd->pre_period_ts = curr_period_time_us;

	if (period_interval_ms < normal_interval / IRQ_PERIOD_LOW_THRESHOLD) {
		AUDIO_LOGE("interval is too short, interval: %u ms", period_interval_ms);
		return false;
	}
	if (period_interval_ms > normal_interval * IRQ_PERIOD_HIGH_THRESHOLD) {
		AUDIO_LOGE("interval is too long, interval: %u ms", period_interval_ms);
		return false;
	}

	return true;
}

static_t unsigned int _get_dma_buf_size(struct snd_pcm_substream *substream, int channel)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;

	return prtd->ports.ports_cnt * prtd->pdma_lli_cfg[prtd->ports.ports_cnt - 1][PCM_SWAP_BUFF_A].a_count;
}

static_t bool _is_substream_valid(struct snd_pcm_substream *substream)
{
	unsigned int port_cnt;
	struct pcm_runtime_data *prtd = NULL;

	if (substream == NULL) {
		AUDIO_LOGE("substream is NULL");
		return false;
	}

	if (substream->runtime == NULL) {
		AUDIO_LOGE("runtime is NULL");
		return false;
	}

	prtd = substream->runtime->private_data;
	if (prtd == NULL) {
		AUDIO_LOGE("prtd is NULL");
		return false;
	}

	for (port_cnt = 0; port_cnt < prtd->ports.ports_cnt; port_cnt++) {
		if (prtd->pdma_lli_cfg[port_cnt] == NULL) {
			AUDIO_LOGE("port config is NULL, port count: %d", port_cnt);
			return false;
		}
	}

	return true;
}

static_t u64 _get_ap_virtual_addr(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	u64 ap_virtual_addr = (u64)(uintptr_t)substream->dma_buffer.area;
	u64 period_size = prtd->period_size;

	return ap_virtual_addr + period_size * prtd->period_cur;
}

static_t u64 _get_dma_virtual_addr(struct snd_pcm_substream *substream,
	unsigned int dma_channel)
{
	int device = substream->pcm->device;
	unsigned int cur_dma_index = _get_idle_buffer_id(substream, dma_channel);
	unsigned int dma_buf_size = _get_dma_buf_size(substream, dma_channel);
	struct pcm_dma_buf_config *dma_buf_cfg = NULL;

	dma_buf_cfg = get_dma_buf_cfg(device, substream->stream);
	if (dma_buf_cfg == NULL) {
		AUDIO_LOGE("get dma buf cfg failed, device: %d, stream: %d",
			device, substream->stream);
		return 0;
	}

	return dma_buf_cfg->virtual_addr + (u64)dma_buf_size * cur_dma_index;
}

static_t void set_data_config(struct data_format_conversion_cfg *ap_cfg,
	struct data_format_conversion_cfg *dma_cfg,
	struct snd_pcm_substream *substream,
	unsigned int dma_channel)
{
	ap_cfg->addr = (void *)(uintptr_t)_get_ap_virtual_addr(substream);
	ap_cfg->pcm_format = PCM_FORMAT_S16_LE;
	ap_cfg->data_format = DATA_FORMAT_INTERLACED;

	dma_cfg->addr = (void *)(uintptr_t)_get_dma_virtual_addr(substream, dma_channel);
	dma_cfg->pcm_format = PCM_FORMAT_S24_LE_LA;
	dma_cfg->data_format = DATA_FORMAT_NONE_INTERLACED;
}

static_t void set_convert_info(struct data_convert_info *convert_info,
	struct snd_pcm_substream *substream,
	unsigned int dma_channel)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct pcm_runtime_data *prtd = runtime->private_data;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		set_data_config(&convert_info->src_cfg, &convert_info->dest_cfg, substream, dma_channel);
	else
		set_data_config(&convert_info->dest_cfg, &convert_info->src_cfg, substream, dma_channel);

	convert_info->period_size = runtime->period_size;
	convert_info->channels = runtime->channels;
	convert_info->merge_interlace_channel = convert_info->channels / prtd->ports.ports_cnt;
}

static_t void _clear_stream_ap_buf(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime  = NULL;
	unsigned int sample_bytes, period_bytes;

	runtime = substream->runtime;
	sample_bytes = get_bytes_per_sample(runtime->format);
	period_bytes = runtime->period_size * sample_bytes * runtime->channels;
	memset((void *)substream->dma_buffer.area, 0, period_bytes * runtime->periods);
	AUDIO_LOGI("device: %d, stream: %d clear ap buffer", substream->pcm->device, substream->stream);
}

void default_data_process(struct data_convert_info *convert_info)
{
	convert_format(convert_info);
}

static_t void _set_play_buf(struct snd_pcm_substream *substream,
	unsigned int dma_channel)
{
	struct snd_pcm_runtime *runtime = NULL;
	struct pcm_runtime_data *prtd = NULL;
	struct data_convert_info info;
	unsigned int sample_bytes;
	int period_data_bytes;

	mutex_lock(&exter_interface_mutex);

	if (!_is_substream_valid(substream)) {
		AUDIO_LOGE("check substream invalid");
		mutex_unlock(&exter_interface_mutex);
		return;
	}

	runtime = substream->runtime;
	sample_bytes = get_bytes_per_sample(runtime->format);
	prtd = substream->runtime->private_data;
	period_data_bytes = runtime->period_size * sample_bytes * runtime->channels;

	set_convert_info(&info, substream, dma_channel);

#ifdef AUDIO_HISI_PCM_CODEC_DEBUG
	hisi_pcm_codec_dump_data((char *)(info.src_cfg.addr), period_data_bytes, substream->pcm->device, PLAY_AP_DATA);
#endif
	if (priv_cust != NULL && priv_cust->data_process != NULL)
		priv_cust->data_process(&info, substream->pcm->device, substream->stream);
	else
		default_data_process(&info);

#ifdef AUDIO_HISI_PCM_CODEC_DEBUG
	hisi_pcm_codec_dump_data((char *)(info.dest_cfg.addr), period_data_bytes * 2, substream->pcm->device, PLAY_DMA_DATA);
#endif

	memset((void *)info.src_cfg.addr, 0, period_data_bytes);

	mutex_unlock(&exter_interface_mutex);
}

static_t void _set_record_buf(struct snd_pcm_substream *substream,
	unsigned int dma_channel)
{
	struct snd_pcm_runtime *runtime  = NULL;
	struct pcm_runtime_data *prtd  = NULL;
	struct data_convert_info info;
	unsigned int sample_bytes;
	int period_bytes;

	mutex_lock(&exter_interface_mutex);

	if (!_is_substream_valid(substream)) {
		AUDIO_LOGE("check substream invalid");
		mutex_unlock(&exter_interface_mutex);
		return;
	}

	runtime = substream->runtime;
	sample_bytes = get_bytes_per_sample(runtime->format);
	prtd = substream->runtime->private_data;
	period_bytes = runtime->period_size * sample_bytes * runtime->channels;

	if (prtd->record_mute_cnt) {
		AUDIO_LOGI("mute record, mute count: %u", prtd->record_mute_cnt);
		_clear_stream_ap_buf(substream);
		prtd->record_mute_cnt--;
		mutex_unlock(&exter_interface_mutex);
		return;
	}

	set_convert_info(&info, substream, dma_channel);

#ifdef AUDIO_HISI_PCM_CODEC_DEBUG
	hisi_pcm_codec_dump_data((char *)(info.src_cfg.addr), period_bytes, substream->pcm->device, CAPTURE_DMA_DATA);
#endif

	if (priv_cust != NULL && priv_cust->data_process != NULL)
		priv_cust->data_process(&info, substream->pcm->device, substream->stream);
	else
		default_data_process(&info);

#ifdef AUDIO_HISI_PCM_CODEC_DEBUG
	hisi_pcm_codec_dump_data((char *)(info.dest_cfg.addr), period_bytes, substream->pcm->device, CAPTURE_AP_DATA);
#endif

	mutex_unlock(&exter_interface_mutex);
}

static_t int _check_stream_irq(unsigned short int_type,
	struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = NULL;
	struct pcm_runtime_data *prtd = NULL;
	int ret = 0;

	mutex_lock(&exter_interface_mutex);

	if (!_is_substream_valid(substream)) {
		ret = -EINVAL;
		goto exit;
	}

	runtime = substream->runtime;
	prtd = runtime->private_data;

	if (!_is_irq_valid(runtime, int_type)) {
		ret = -EINVAL;
		goto exit;
	}

	prtd->irq_cnt++;
	if (prtd->irq_cnt < prtd->ports.ports_cnt) {
		ret = -EINVAL;
		goto exit;
	}
	prtd->irq_cnt = 0;

	if (!_is_period_valid(runtime)) {
		ret = -EFAULT;
		goto exit;
	}

	prtd->period_cur = (prtd->period_cur + 1) % runtime->periods;
exit:
	mutex_unlock(&exter_interface_mutex);

	return ret;
}

static_t int _intr_play_handle(unsigned short int_type,
	unsigned long para, unsigned int dma_channel)
{
	struct snd_pcm_substream *substream = (void *)(uintptr_t)para;
	struct snd_pcm_runtime *runtime = NULL;
	struct pcm_runtime_data *prtd = NULL;
	snd_pcm_uframes_t avail;
	int ret;

	mutex_lock(&prtd_mutex[SNDRV_PCM_STREAM_PLAYBACK]);

	ret = _check_stream_irq(int_type, substream);
	if (ret != 0) {
		mutex_unlock(&prtd_mutex[SNDRV_PCM_STREAM_PLAYBACK]);
		return ret;
	}

	runtime = substream->runtime;
	prtd = runtime->private_data;
	avail = snd_pcm_playback_hw_avail(runtime);
	if (avail < runtime->period_size)
		AUDIO_LOGW("there is no avail data, available: %lu, period size: %lu",
			avail, runtime->period_size);

	snd_pcm_period_elapsed(substream);

	_set_play_buf(substream, dma_channel);

	mutex_unlock(&prtd_mutex[SNDRV_PCM_STREAM_PLAYBACK]);

	return ret;
}

static_t int _intr_record_handle(unsigned short int_type,
	unsigned long para, unsigned int dma_channel)
{
	struct snd_pcm_substream *substream = (void *)(uintptr_t)para;
	struct snd_pcm_runtime *runtime = NULL;
	struct pcm_runtime_data *prtd = NULL;
	snd_pcm_uframes_t avail;
	int ret;

	mutex_lock(&prtd_mutex[SNDRV_PCM_STREAM_CAPTURE]);

	ret = _check_stream_irq(int_type, substream);
	if (ret != 0) {
		mutex_unlock(&prtd_mutex[SNDRV_PCM_STREAM_CAPTURE]);
		return ret;
	}

	runtime = substream->runtime;
	prtd = runtime->private_data;
	avail = snd_pcm_capture_hw_avail(runtime);
	if (avail < runtime->period_size)
		AUDIO_LOGW("there is no avail data, available: %lu, period size: %lu",
			avail, runtime->period_size);

	_set_record_buf(substream, dma_channel);

	snd_pcm_period_elapsed(substream);

	mutex_unlock(&prtd_mutex[SNDRV_PCM_STREAM_CAPTURE]);

	return ret;
}

static_t int _alloc_dma_buffer(struct snd_pcm *pcm)
{
	int stream;
	int device = pcm->device;
	const struct snd_pcm_hardware *pcm_hardware = NULL;
	struct snd_pcm_substream *substream = NULL;
	struct snd_dma_buffer *buf = NULL;
	struct pcm_dma_buf_config *dma_buf_cfg = NULL;

	for (stream = SNDRV_PCM_STREAM_PLAYBACK; stream  < MAX_STREAM_TYPES; stream++) {
		substream = pcm->streams[stream].substream;
		if (substream == NULL)
			continue;

		dma_buf_cfg = get_dma_buf_cfg(device, stream);
		if (dma_buf_cfg == NULL) {
			AUDIO_LOGE("get dma buf cfg failed, device: %d, stream: %d",
				device, stream);
			return -ENOMEM;
		}

		buf = &substream->dma_buffer;
		buf->dev.type = SNDRV_DMA_TYPE_DEV;
		buf->dev.dev = pcm->card->dev;
		buf->private_data = NULL;

		pcm_hardware = _get_hw_config(device, stream);
		if (pcm_hardware == NULL) {
			AUDIO_LOGE("get pcm hardware config failed");
			return -ENOMEM;
		}

		buf->bytes = pcm_hardware->buffer_bytes_max;
		buf->area = kzalloc(buf->bytes, GFP_KERNEL);
		if (!buf->area) {
			AUDIO_LOGE("kzalloc pcm buff failed");
			return -ENOMEM;
		}

		AUDIO_LOGI("virt address: 0x%pK, buf length: 0x%lx", buf->area, buf->bytes);

		dma_buf_cfg->virtual_addr =
			(u64)(uintptr_t)ioremap_wc(dma_buf_cfg->phy_addr, /*lint !e446 */
			dma_buf_cfg->len);
		if (dma_buf_cfg->virtual_addr == 0) {
			AUDIO_LOGE("dma buffer ioremap failed");
			return -ENOMEM;
		}

		memset((void *)(uintptr_t)dma_buf_cfg->virtual_addr, 0, dma_buf_cfg->len);

		AUDIO_LOGI("device: %d, dma addr: 0x%pK, virt addr: 0x%pK, buf length: %llu",
			device,
			(void *)(uintptr_t)dma_buf_cfg->phy_addr,
			(void *)(uintptr_t)dma_buf_cfg->virtual_addr,
			dma_buf_cfg->len);

	}
	return 0;
}

static_t void _free_dma_buffer(struct snd_pcm *pcm)
{
	int stream;
	struct snd_pcm_substream *substream = NULL;
	struct snd_dma_buffer *buf = NULL;
	int device = pcm->device;
	struct pcm_dma_buf_config *dma_buf_cfg = NULL;

	for (stream = SNDRV_PCM_STREAM_PLAYBACK;
		stream < MAX_STREAM_TYPES; stream++) {
		substream = pcm->streams[stream].substream;
		if (substream == NULL)
			continue;

		buf = &substream->dma_buffer;
		kfree(buf->area);
		buf->area = NULL;

		dma_buf_cfg = get_dma_buf_cfg(device, stream);
		if (dma_buf_cfg == NULL) {
			AUDIO_LOGE("get dma buf cfg failed, device: %d, stream: %d",
				device, stream);
			continue;
		}

		iounmap((void __iomem *)(uintptr_t)dma_buf_cfg->virtual_addr);
		dma_buf_cfg->virtual_addr = 0;

		AUDIO_LOGI("steam type: %d, free buffer success", stream);
	}
}

static_t int _config_dma(struct pcm_runtime_data *prtd,
	struct snd_pcm_substream *substream)
{
	unsigned int port_index;
	callback_t callback;
	unsigned int ports_cnt = prtd->ports.ports_cnt;
	int ret = 0;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		callback = _intr_play_handle;
	else
		callback = _intr_record_handle;

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		ret = asp_dma_config(prtd->dma_cfg[port_index].channel,
			prtd->pdma_lli_cfg[port_index],
			callback, (uintptr_t)substream);
		if (ret != 0) {
			AUDIO_LOGE("dma channel: %u config failed, port index: %u",
				prtd->dma_cfg[port_index].channel, port_index);
			return ret;
		}
	}

	return ret;
}

static_t int _start_dma(struct pcm_runtime_data *prtd)
{
	unsigned int port_index;
	unsigned int ports_cnt = prtd->ports.ports_cnt;
	int ret = 0;

	prtd->irq_cnt = 0;
	prtd->pre_period_ts = 0;

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		ret = asp_dma_start(prtd->dma_cfg[port_index].channel,
			prtd->pdma_lli_cfg[port_index]);
		if (ret != 0) {
			AUDIO_LOGE("dma channel: %u, start failed",
				prtd->dma_cfg[port_index].channel);
			return ret;
		}
		AUDIO_LOGI("port index: %u, dma channel: %u, lli: 0x%x, start success",
			port_index, prtd->dma_cfg[port_index].channel,
			prtd->pdma_lli_cfg[port_index]->lli);
	}

	return ret;
}

static_t void _stop_dma(struct pcm_runtime_data *prtd)
{
	unsigned int port_index;
	unsigned int lli_index;
	unsigned int ports_cnt = prtd->ports.ports_cnt;

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		for (lli_index = 0; lli_index < DMA_LLI_NUM; lli_index++)
			prtd->pdma_lli_cfg[port_index][lli_index].lli = 0x0;

		asp_dma_stop(prtd->dma_cfg[port_index].channel);
	}
}

static_t void _iounmap_dma_lli_cfg(struct pcm_runtime_data *prtd)
{
	unsigned int port_index;
	unsigned int ports_cnt = prtd->ports.ports_cnt;

	for (port_index = 0; port_index < ports_cnt; port_index++) {
		if (prtd->pdma_lli_cfg[port_index]) {
			iounmap(prtd->pdma_lli_cfg[port_index]);
			prtd->pdma_lli_cfg[port_index] = NULL;
		}
	}
}

static_t int _check_channels_param(int stream, unsigned int channels)
{
	if (channels == 0 || (stream == SNDRV_PCM_STREAM_PLAYBACK && channels > PLAYBACK_CHANNEL_CNT_MAX) ||
		(stream == SNDRV_PCM_STREAM_CAPTURE && channels > RECORD_EC_CHANNEL_CNT)) {
		AUDIO_LOGE("channel: %u is not support", channels);
		return -EPERM;
	}
	return 0;
}

static_t int _ioremap_dma_lli_cfg(struct pcm_runtime_data *prtd, int stream,
	int device, struct pcm_dma_buf_config *lli_buf_cfg)
{
	int ret = 0;
	unsigned int port_index;

	for (port_index = 0; port_index < prtd->ports.ports_cnt; port_index++) {
		prtd->pdma_lli_cfg[port_index] =
			(struct dma_lli_cfg *)ioremap(lli_buf_cfg->phy_addr + /*lint !e446 */
			(port_index * DMA_LLI_NUM * sizeof(struct dma_lli_cfg)),
			DMA_LLI_NUM * sizeof(struct dma_lli_cfg));
		if (prtd->pdma_lli_cfg[port_index] == NULL) {
			AUDIO_LOGE("lli config dma ioremap error");
			ret = -ENOMEM;
			break;
		}
	}
	return ret;
}

static int _allocate_dma_buff(struct snd_pcm_hw_params *params,
	struct snd_pcm_substream *substream, struct pcm_runtime_data *prtd,
	unsigned int channels, int stream)
{
	unsigned int ports_cnt, dma_buffer_size_param;
	int ret;

	ports_cnt = get_dma_port_count(channels, prtd->pdata->io_type_cfg,
		prtd->ports.pipe_type_cfg);
	if (ports_cnt == 0) {
		AUDIO_LOGE("ports count is invalid: %d", ports_cnt);
		return -EINVAL;
	}

	prtd->ports.ports_cnt = ports_cnt;
	dma_buffer_size_param = params_buffer_bytes(params);
	if (stream == SNDRV_PCM_STREAM_CAPTURE && channels == RECORD_EC_CHANNEL_CNT)
		dma_buffer_size_param = dma_buffer_size_param / channels * prtd->ports.ports_cnt;

	ret = snd_pcm_lib_malloc_pages(substream, dma_buffer_size_param);

	return ret;
}

static_t int hisi_pcm_codec_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	unsigned int port_index, channels;
	int stream, ret;
	int device = substream->pcm->device;
	struct pcm_dma_buf_config *lli_buf_cfg = NULL;

	if (prtd == NULL) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	channels = params_channels(params);
	prtd->channels = channels;
	prtd->frames = params_buffer_size(params);
	stream = substream->stream;

	lli_buf_cfg = get_lli_buf_cfg(device, stream);
	if (lli_buf_cfg == NULL) {
		AUDIO_LOGE("get lli buf cfg failed, device: %d, stream: %d", device, stream);
		ret = -EINVAL;
		goto exit;
	}

	ret = _check_channels_param(stream, channels);
	if (ret != 0)
		goto exit;

	ret = _allocate_dma_buff(params, substream, prtd, channels, stream);
	if (ret < 0) {
		AUDIO_LOGE("snd malloc pages failed, result: %d", ret);
		goto exit;
	}

	ret = _ioremap_dma_lli_cfg(prtd, stream, device, lli_buf_cfg);
	if (ret != 0)
		goto iomap_err_out;

	for (port_index = 0; port_index < prtd->ports.ports_cnt; port_index++)
		prtd->lli_dma_addr[port_index] = lli_buf_cfg->phy_addr +
			(port_index * DMA_LLI_NUM * sizeof(struct dma_lli_cfg)) -
			ASP_FAMA_PHY_ADDR_DIFF;

	prtd->period_size = params_period_bytes(params);
	if (stream == SNDRV_PCM_STREAM_CAPTURE && channels == RECORD_EC_CHANNEL_CNT)
		prtd->dma_period_size = prtd->period_size / channels * prtd->ports.ports_cnt;
	else
		prtd->dma_period_size = prtd->period_size;

	AUDIO_LOGI("ap period size: %u, dma period size: %u", prtd->period_size, prtd->dma_period_size);

	prtd->sample_rate = params_rate(params);

	AUDIO_LOGI("dma buffer bytes: %u, buffer size: %u, period size: %u, rate: %u, format: %d",
		params_buffer_bytes(params), prtd->frames, prtd->period_size,
		prtd->sample_rate, params_format(params));

	goto exit;

iomap_err_out:
	AUDIO_LOGE("hw params error, result: %d", ret);
	_iounmap_dma_lli_cfg(prtd);

	snd_pcm_lib_free_pages(substream);
exit:
	mutex_unlock(&exter_interface_mutex);

	return ret;
}

static_t int hisi_pcm_codec_hw_free(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = NULL;
	int ret;

	AUDIO_LOGI("hardware free begin");

	prtd = substream->runtime->private_data;
	if (prtd == NULL) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	_iounmap_dma_lli_cfg(prtd);
	ret = snd_pcm_lib_free_pages(substream);

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGI("hardware free end");
	return ret;
}

static_t int hisi_pcm_codec_prepare(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	unsigned int ports_cnt, port_index;
	int ret;

	if (prtd == NULL) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	ports_cnt = prtd->ports.ports_cnt;
	prtd->is_dma_running = false;
	for (port_index = 0; port_index < ports_cnt; port_index++)
		prtd->dma_buf_next[port_index] = port_index;

	prtd->period_cur = 0;

	ret = _get_dma_cfg(prtd);
	if (ret != 0) {
		mutex_unlock(&exter_interface_mutex);
		return ret;
	}

	_dma_lli_cfg(substream);

	ret = _config_dma(prtd, substream);
	if (ret != 0) {
		mutex_unlock(&exter_interface_mutex);
		AUDIO_LOGE("config dma failed");
		return ret;
	}

	mutex_unlock(&exter_interface_mutex);

	return 0;
}

static_t int hisi_pcm_codec_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	if (prtd == NULL) {
		AUDIO_LOGE("prtd is null");
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	AUDIO_LOGI("stream type: %d, command: %d, ports count: %u",
		substream->stream, cmd, prtd->ports.ports_cnt);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
#ifdef LLT_TEST
			prtd->record_mute_cnt = 0;
#else
			prtd->record_mute_cnt = RECORD_MUTE_MAX_CNT;
#endif
			AUDIO_LOGI("record mute count: %u", prtd->record_mute_cnt);
		}
		ret = _start_dma(prtd);
		if (ret != 0) {
			AUDIO_LOGE("start dma failed");
			_stop_dma(prtd);
			mutex_unlock(&exter_interface_mutex);
			return ret;
		}

		prtd->is_dma_running = true;
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		prtd->is_dma_running = false;
		_stop_dma(prtd);
		break;

	default:
		AUDIO_LOGE("command: %d, is invalid", cmd);
		ret = -EINVAL;
		break;
	}

	mutex_unlock(&exter_interface_mutex);

	return ret;
}

static_t snd_pcm_uframes_t hisi_pcm_codec_pointer(
	struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct pcm_runtime_data *prtd = substream->runtime->private_data;
	unsigned int period_cur, period_size;
	snd_pcm_uframes_t offset;

	if (prtd == NULL) {
		AUDIO_LOGE("prtd is null");
		return 0;
	}

	mutex_lock(&exter_interface_mutex);

	period_cur = prtd->period_cur;
	period_size = prtd->period_size;

	offset = bytes_to_frames(runtime, period_cur * period_size);
	if (offset >= runtime->buffer_size)
		offset = 0;

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGD("period current: %u, frame: %ld, buffer size: %ld, period size: %u",
		prtd->period_cur, (long)offset,
		(long)(runtime->buffer_size), period_size);

	return offset;
}

static_t int _set_stream_runtime_hwparams(const struct snd_pcm_hardware *cfg,
	struct snd_pcm_substream *substream, struct pcm_runtime_data *prtd,
	enum pipe_type pipe_type_cfg, int stream)
{
	int ret;
	struct snd_pcm *pcm = substream->pcm;

	prtd->ports.pipe_type_cfg = pipe_type_cfg;
	if (is_device_valid(substream->pcm->device) &&
		is_stream_valid(substream->stream))
		pcm_streams[pcm->device][stream] = substream;

	ret = snd_soc_set_runtime_hwparams(substream, cfg);
	if (ret != 0) {
		AUDIO_LOGE("set stream type: %d runtime hwparams failed, result: %d", stream, ret);
		return -EINVAL;
	}

	AUDIO_LOGI("stream type: %d, pcm device: %d, pipe type: %u",
		stream, pcm->device, prtd->ports.pipe_type_cfg);

	return 0;
}


static_t int _set_runtime_hwparams(struct snd_pcm_substream *substream,
	struct pcm_runtime_data *prtd, enum io_type io_type_cfg)
{
	int device = substream->pcm->device;
	int stream = substream->stream;
	const struct snd_pcm_hardware *pcm_hardware_cfg = NULL;
	enum pipe_type pipe_type_cfg = get_pipe_type(device, stream, io_type_cfg);

	if (pipe_type_cfg >= PIPE_CNT) {
		AUDIO_LOGE("invaild pipe type: %d", pipe_type_cfg);
		return -EINVAL;
	}
	pcm_hardware_cfg = _get_hw_config(device, stream);

	if (pcm_hardware_cfg == NULL) {
		AUDIO_LOGE("get pcm hardware cfg failed");
		return -EINVAL;
	}
	return _set_stream_runtime_hwparams(pcm_hardware_cfg,
			substream, prtd, pipe_type_cfg, stream);
}

static_t int hisi_pcm_codec_open(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = NULL;
	struct snd_soc_pcm_runtime *rtd = NULL;
	struct platform_dev_data *pdata = NULL;
	int ret = 0;

	mutex_lock(&exter_interface_mutex);

	prtd = kzalloc(sizeof(struct pcm_runtime_data), GFP_KERNEL);
	if (prtd == NULL) {
		AUDIO_LOGE("kzalloc runtime data failed");
		ret = -ENOMEM;
		goto exit;
	}

	rtd = (struct snd_soc_pcm_runtime *)substream->private_data;
	pdata = (struct platform_dev_data *)snd_soc_platform_get_drvdata(rtd->platform);
	if (pdata == NULL) {
		AUDIO_LOGE("get drv data failed");
		ret = -EINVAL;
		goto runtime_data_free;
	}

	ret = regulator_enable(pdata->asp_regulator);
	if (ret != 0) {
		AUDIO_LOGE("enable asp regulator failed, result: %d", ret);
		goto runtime_data_free;
	}

	prtd->pdata = pdata;
	substream->runtime->private_data = prtd;

	ret = _set_runtime_hwparams(substream, prtd, pdata->io_type_cfg);
	if (ret != 0)
		goto set_hwparams_failed;

	goto exit;

set_hwparams_failed:
	if (regulator_disable(pdata->asp_regulator) != 0)
		AUDIO_LOGW("regulator disable failed");
runtime_data_free:
	kfree(prtd);
exit:
	mutex_unlock(&exter_interface_mutex);
	return ret;
}

static_t int hisi_pcm_codec_close(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = NULL;
	int ret;

	AUDIO_LOGI("close begin");

	mutex_lock(&prtd_mutex[substream->stream]);
	mutex_lock(&exter_interface_mutex);

	prtd = substream->runtime->private_data;

	if (prtd == NULL || prtd->pdata == NULL) {
		AUDIO_LOGE("prtd or pdata is NULL");
		mutex_unlock(&exter_interface_mutex);
		mutex_unlock(&prtd_mutex[substream->stream]);
		return -ENOMEM;
	}

	if (is_device_valid(substream->pcm->device) &&
		is_stream_valid(substream->stream))
		pcm_streams[substream->pcm->device][substream->stream] = NULL;

	ret = regulator_disable(prtd->pdata->asp_regulator);
	if (ret != 0)
		AUDIO_LOGW("regulator disable failed, ret: %d", ret);

	kfree(prtd);
	substream->runtime->private_data = NULL;

	mutex_unlock(&exter_interface_mutex);
	mutex_unlock(&prtd_mutex[substream->stream]);

	AUDIO_LOGI("close end");

	return 0;
}

static_t struct snd_pcm_ops hisi_pcm_codec_ops = {
	.open       = hisi_pcm_codec_open,
	.close      = hisi_pcm_codec_close,
	.ioctl      = snd_pcm_lib_ioctl,
	.hw_params  = hisi_pcm_codec_hw_params,
	.hw_free    = hisi_pcm_codec_hw_free,
	.prepare    = hisi_pcm_codec_prepare,
	.trigger    = hisi_pcm_codec_trigger,
	.pointer    = hisi_pcm_codec_pointer,
};

static_t int hisi_pcm_codec_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm *pcm = rtd->pcm;
	static unsigned long long dma_mask = DMA_BIT_MASK(32);
	int ret = 0;

	if (card == NULL || pcm == NULL) {
		AUDIO_LOGE("card or pcm is NULL");
		return -EINVAL;
	}

	if (pcm->device >= PCM_DEVICE_TOTAL) {
		AUDIO_LOGE("invalid argument, pcm device: %d", pcm->device);
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	if (card->dev->dma_mask == NULL)
		card->dev->dma_mask = &dma_mask;
	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = dma_mask;

	ret = _alloc_dma_buffer(pcm);
	if (ret != 0) {
		AUDIO_LOGE("alloc dma buffer failed, result: %d", ret);
		_free_dma_buffer(pcm);
		goto exit;
	}

	AUDIO_LOGI("pcm new ok");
exit:
	mutex_unlock(&exter_interface_mutex);

	return ret;
}

static_t void hisi_pcm_codec_free(struct snd_pcm *pcm)
{
	if (pcm == NULL) {
		AUDIO_LOGE("pcm is NULL");
		return;
	}

	mutex_lock(&exter_interface_mutex);

	_free_dma_buffer(pcm);

	mutex_unlock(&exter_interface_mutex);
}

static_t struct snd_soc_platform_driver hisi_pcm_codec_platform = {
	.ops      = &hisi_pcm_codec_ops,
	.pcm_new  = hisi_pcm_codec_new,
	.pcm_free = hisi_pcm_codec_free,
};

static_t int _register_dai_driver(struct device *dev)
{
	int ret = 0;

	if (priv_cust->pcm_dai != NULL && priv_cust->dai_num > 0) {
		ret = snd_soc_register_component(dev, &hisi_pcm_codec_component,
				priv_cust->pcm_dai, priv_cust->dai_num);
		if (ret != 0) {
			AUDIO_LOGE("snd_soc_register_dai return %d", ret);
			return -EINVAL;
		}
	}

	return ret;
}

static_t int _read_config(struct platform_device *pdev, struct platform_dev_data *pdata)
{
	struct device_node *np = pdev->dev.of_node;

	if (of_property_read_u32(np, "io_type", &pdata->io_type_cfg)) {
		AUDIO_LOGE("can not get io type");
		return -EINVAL;
	}

	if (pdata->io_type_cfg >= IO_CNT) {
		AUDIO_LOGE("invalid io type num");
		return -EINVAL;
	}

	return 0;
}

static_t int hisi_pcm_codec_probe(struct platform_device *pdev)
{
	int ret;
	int i;
	struct device *dev = &pdev->dev;
	struct platform_dev_data *pdata = NULL;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (pdata == NULL) {
		AUDIO_LOGE("privite data malloc failed");
		return -ENOMEM;
	}

	ret = hisi_pcm_format_init(dev, _get_hw_buffer_bytes_max());
	if (ret != 0) {
		AUDIO_LOGE("format init failed, result: %d", ret);
		return -EFAULT;
	}

	if (_read_config(pdev, pdata)) {
		AUDIO_LOGE("config error");
		return -EINVAL;
	}

	pdata->asp_regulator = devm_regulator_get(dev, "asp-dmac");
	if (IS_ERR(pdata->asp_regulator)) {
		AUDIO_LOGE("get asp regulator failed");
		return -ENOENT;
	}

	platform_set_drvdata(pdev, pdata);

	ret = platform_io_init();
	if (ret != 0) {
		AUDIO_LOGE("platform io init failed");
		return -EINVAL;
	}

	ret = _register_dai_driver(&pdev->dev);
	if (ret != 0) {
		AUDIO_LOGE("register dai driver failed");
		return ret;
	}

	dev_set_name(dev, "hisi-pcm-codec");
	memset(pcm_streams, 0, sizeof(pcm_streams));

	mutex_init(&exter_interface_mutex);
	for (i = 0; i < MAX_STREAM_TYPES; i++)
		mutex_init(&prtd_mutex[i]);

	ret = snd_soc_register_platform(dev, &hisi_pcm_codec_platform);
	if (ret != 0) {
		AUDIO_LOGE("snd register platform driver failed, result: %d", ret);
		mutex_destroy(&exter_interface_mutex);
		for (i = 0; i < MAX_STREAM_TYPES; i++)
			mutex_destroy(&prtd_mutex[i]);
		snd_soc_unregister_component(&pdev->dev);
		return -ENODEV;
	}

	return 0;
}

static_t void _pcm_codec_suspend(struct snd_pcm_substream *substream)
{
	unsigned int ports_cnt;
	unsigned int port_index;
	unsigned int stream = substream->stream;
	struct pcm_runtime_data *prtd = NULL;

	prtd = substream->runtime->private_data;

	if (!prtd->is_dma_running) {
		AUDIO_LOGI("stream type: %u, is not running", stream);
		return;
	}

	ports_cnt = prtd->ports.ports_cnt;
	for (port_index = 0; port_index < ports_cnt; port_index++) {
		asp_dma_suspend(prtd->dma_cfg[port_index].channel);
		AUDIO_LOGI("suspend dma channel: %u", prtd->dma_cfg[port_index].channel);
	}

	return;
}

static_t int hisi_pcm_codec_suspend(struct device *pdev)
{
	AUDIO_LOGI("begin");

	mutex_lock(&exter_interface_mutex);

	_enum_pcm_streams(_pcm_codec_suspend);

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGI("end");
	return 0;
}

static_t void _reconfig_dma_lli(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = NULL;

	prtd = substream->runtime->private_data;
	if (!prtd->is_dma_running) {
		AUDIO_LOGI("stream type: %u is not running", substream->stream);
		return;
	}

	_dma_lli_cfg(substream);
}

static_t void _pcm_codec_resume(struct snd_pcm_substream *substream)
{
	struct pcm_runtime_data *prtd = NULL;
	unsigned int stream = substream->stream;
	int ret;

	prtd = substream->runtime->private_data;
	if (!prtd->is_dma_running) {
		AUDIO_LOGI("stream type: %u is not running", stream);
		return;
	}

	AUDIO_LOGI("stream type: %u, irq count: %u, pre period time ns: %lu",
		stream, prtd->irq_cnt, prtd->pre_period_ts);

	ret = _config_dma(prtd, substream);
	if (ret != 0) {
		AUDIO_LOGW("config dma failed");
		return;
	}

	ret = _start_dma(prtd);
	if (ret != 0) {
		_stop_dma(prtd);
		AUDIO_LOGW("start dma failed");
		return;
	}
}

static_t int hisi_pcm_codec_resume(struct device *pdev)
{
	AUDIO_LOGI("begin");

	mutex_lock(&exter_interface_mutex);

	_enum_pcm_streams(_pcm_codec_resume);

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGI("end");
	return 0;
}

static_t int _restore_asp_regulator_status(struct device *pdev)
{
	struct platform_dev_data *pdata = (struct platform_dev_data *)dev_get_drvdata(pdev);
	unsigned int stream;
	unsigned int device;
	int ret;

	if (pdata == NULL) {
		AUDIO_LOGE("pdata is NULL");
		return -EINVAL;
	}

	for (device = PCM_DEVICE_NORMAL; device < PCM_DEVICE_TOTAL; device++) {
		for (stream = SNDRV_PCM_STREAM_PLAYBACK; stream < MAX_STREAM_TYPES; stream++) {
			if (pcm_streams[device][stream] != NULL) {
				ret = regulator_enable(pdata->asp_regulator);
				if (ret != 0) {
					AUDIO_LOGE("regulator enable for stream %d failed", stream);
					return ret;
				}
				AUDIO_LOGI("stream %d asp regulator enable", stream);
			}
		}
	}
	return 0;
}

static_t int hisi_pcm_codec_freeze(struct device *pdev)
{
	AUDIO_LOGI("begin");

	mutex_lock(&exter_interface_mutex);

	_enum_pcm_streams(_pcm_codec_suspend);

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGI("end");
	return 0;
}

static_t int hisi_pcm_codec_restore(struct device *pdev)
{
	int ret;
	AUDIO_LOGI("begin");

	if (pdev == NULL) {
		AUDIO_LOGE("pdev is NULL");
		return -EINVAL;
	}

	mutex_lock(&exter_interface_mutex);

	ret = _restore_asp_regulator_status(pdev);
	if (ret != 0) {
		mutex_unlock(&exter_interface_mutex);
		return ret;
	}

	_enum_pcm_streams(_clear_stream_ap_buf);
	_enum_pcm_streams(_reconfig_dma_lli);
	_enum_pcm_streams(_pcm_codec_resume);

	mutex_unlock(&exter_interface_mutex);

	AUDIO_LOGI("end");
	return ret;
}

const struct dev_pm_ops hisi_pcm_codec_pm_ops = {
	.suspend = hisi_pcm_codec_suspend,
	.resume = hisi_pcm_codec_resume,
	.freeze = hisi_pcm_codec_freeze,
	.thaw = hisi_pcm_codec_resume,
	.restore = hisi_pcm_codec_restore,
};

static_t struct platform_driver hisi_pcm_codec_driver = {
	.driver = {
		.name           = "hisi-pcm-codec",
		.owner          = THIS_MODULE,
		.of_match_table = hisi_pcm_codec_of_match,
		.pm             = &hisi_pcm_codec_pm_ops,
	},
	.probe = hisi_pcm_codec_probe,
};
module_platform_driver(hisi_pcm_codec_driver);

MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("Hisi Pcm Codec Platform Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:hisi_pcm_codec");

