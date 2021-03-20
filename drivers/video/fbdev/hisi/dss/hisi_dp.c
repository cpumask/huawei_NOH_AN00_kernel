/* Copyright (c) 2013-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/hisi/usb/tca.h>

#include "hisi_fb_def.h"
#include "hisi_fb.h"
#include "hisi_dp.h"

#include "dp/link/dp_link_layer_interface.h"
#include <linux/printk.h>
#include "hisi_dpe_utils.h"
#include "hdcp/hdcp_common.h"

#if defined(CONFIG_HISI_PERIDVFS)
#include "peri_volt_poll.h"
#endif

#define DTS_HISI_DP "hisilicon,hisi_dp_swing"
#ifdef CONFIG_HISI_FB_V600
#define DTS_COMP_SSC_PPM_VALUE "hisilicon,hisi_dp_ssc_ppm"
#endif
#define DTS_DP_AUX_SWITCH "huawei,dp_aux_switch"
#define PERI_VOLTAGE_LEVEL0_060V		(0) /* 0.60v */
#define PERI_VOLTAGE_LEVEL1_065V		(1) /* 0.65v */
#define PERI_VOLTAGE_LEVEL2_070V		(2) /* 0.70v */
#define PERI_VOLTAGE_LEVEL3_080V		(3) /* 0.80v */

struct platform_device *g_dp_pdev;
static bool bpress_powerkey;
static bool btrigger_timeout;

/*******************************************************************************
 *
 */
int hisi_dptx_get_spec(void *data, unsigned int size, unsigned int *ext_acount)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	struct edid_audio *audio_info = NULL;

	hisi_check_and_return((data == NULL || ext_acount == NULL), -EINVAL, ERR, "[DP] parameter is NULL!\n");
	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &hisifd->dp;
	audio_info = &dptx->edid_info.Audio;

	if (size < sizeof(struct edid_audio_info) * audio_info->extACount) {
		HISI_FB_ERR("[DP] size is error!size %d extACount %d\n", size, audio_info->extACount);
		return -EINVAL;
	}

	memcpy(data, audio_info->spec, sizeof(struct edid_audio_info) * audio_info->extACount);
	*ext_acount = audio_info->extACount;

	HISI_FB_INFO("[DP] get spec success\n");

	return 0;
}

int hisi_dptx_set_aparam(unsigned int channel_num, unsigned int data_width, unsigned int sample_rate)
{
	uint8_t orig_sample_freq = 0;
	uint8_t sample_freq = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	struct audio_params *aparams = NULL;

	if (channel_num > DPTX_CHANNEL_NUM_MAX || data_width > DPTX_DATA_WIDTH_MAX) {
		HISI_FB_ERR("[DP] input param is invalid. channel_num(%d) data_width(%d)\n", channel_num, data_width);
		return -EINVAL;
	}

	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &hisifd->dp;
	aparams = &dptx->aparams;

	HISI_FB_INFO("[DP] set aparam. channel_num(%d) data_width(%d) sample_rate(%d)\n",
		channel_num, data_width, sample_rate);

	switch (sample_rate) {
	case 32000:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_32K;
		sample_freq = IEC_SAMP_FREQ_32K;
		break;
	case 44100:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_44K;
		sample_freq = IEC_SAMP_FREQ_44K;
		break;
	case 48000:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_48K;
		sample_freq = IEC_SAMP_FREQ_48K;
		break;
	case 88200:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_88K;
		sample_freq = IEC_SAMP_FREQ_88K;
		break;
	case 96000:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_96K;
		sample_freq = IEC_SAMP_FREQ_96K;
		break;
	case 176400:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_176K;
		sample_freq = IEC_SAMP_FREQ_176K;
		break;
	case 192000:
		orig_sample_freq = IEC_ORIG_SAMP_FREQ_192K;
		sample_freq = IEC_SAMP_FREQ_192K;
		break;
	default:
		HISI_FB_INFO("[DP] invalid sample_rate\n");
		return -EINVAL;
	}

	aparams->iec_samp_freq = sample_freq;
	aparams->iec_orig_samp_freq = orig_sample_freq;
	aparams->num_channels = (uint8_t)channel_num;
	aparams->data_width = (uint8_t)data_width;

	if (dptx->dptx_audio_config)
		dptx->dptx_audio_config(dptx);

	HISI_FB_INFO("[DP] set aparam success\n");

	return 0;
}

int dptx_phy_rate_to_bw(uint8_t rate)
{
	switch (rate) {
	case DPTX_PHYIF_CTRL_RATE_RBR:
		return DP_LINK_BW_1_62;
	case DPTX_PHYIF_CTRL_RATE_HBR:
		return DP_LINK_BW_2_7;
	case DPTX_PHYIF_CTRL_RATE_HBR2:
		return DP_LINK_BW_5_4;
	case DPTX_PHYIF_CTRL_RATE_HBR3:
		return DP_LINK_BW_8_1;
	default:
		HISI_FB_ERR("[DP] Invalid rate 0x%x\n", rate);
		return -EINVAL;
	}
}

int dptx_bw_to_phy_rate(uint8_t bw)
{
	switch (bw) {
	case DP_LINK_BW_1_62:
		return DPTX_PHYIF_CTRL_RATE_RBR;
	case DP_LINK_BW_2_7:
		return DPTX_PHYIF_CTRL_RATE_HBR;
	case DP_LINK_BW_5_4:
		return DPTX_PHYIF_CTRL_RATE_HBR2;
	case DP_LINK_BW_8_1:
		return DPTX_PHYIF_CTRL_RATE_HBR3;
	default:
		HISI_FB_ERR("[DP] Invalid bw 0x%x\n", bw);
		return -EINVAL;
	}
}

/*
 * Audio/Video Parameters Reset
 */
void dptx_audio_params_reset(struct audio_params *params)
{
	hisi_check_and_no_retval((params == NULL), ERR, "[DP] NULL Pointer!\n");

	memset(params, 0x0, sizeof(struct audio_params));
	params->iec_channel_numcl0 = 8;
	params->iec_channel_numcr0 = 4;
	params->use_lut = 1;
	params->iec_samp_freq = 3;
	params->iec_word_length = 11;
	params->iec_orig_samp_freq = 12;
	params->data_width = 16;
	params->num_channels = 2;
	params->inf_type = 1;
	params->ats_ver = 17;
	params->mute = 0;
}

void dptx_video_params_reset(struct video_params *params)
{
	hisi_check_and_no_retval((params == NULL), ERR, "[DP] NULL Pointer!\n");

	memset(params, 0x0, sizeof(struct video_params));

	/* 6 bpc should be default - use 8 bpc for MST calculation */
	params->bpc = COLOR_DEPTH_8;
	params->dp_dsc_info.dsc_info.dsc_bpp = 8; /* DPTX_BITS_PER_PIXEL */
	params->pix_enc = RGB;
	params->mode = 1;
	params->colorimetry = ITU601;
	params->dynamic_range = CEA;
	params->aver_bytes_per_tu = 30;
	params->aver_bytes_per_tu_frac = 0;
	params->init_threshold = 15;
	params->pattern_mode = RAMP;
	params->refresh_rate = 60000;
	params->video_format = VCEA;
}

/*
 * DTD reset
 */
void dwc_dptx_dtd_reset(struct dtd *mdtd)
{
	hisi_check_and_no_retval((mdtd == NULL), ERR, "[DP] NULL Pointer!\n");

	mdtd->pixel_repetition_input = 0;
	mdtd->pixel_clock  = 0;
	mdtd->h_active = 0;
	mdtd->h_blanking = 0;
	mdtd->h_sync_offset = 0;
	mdtd->h_sync_pulse_width = 0;
	mdtd->h_image_size = 0;
	mdtd->v_active = 0;
	mdtd->v_blanking = 0;
	mdtd->v_sync_offset = 0;
	mdtd->v_sync_pulse_width = 0;
	mdtd->v_image_size = 0;
	mdtd->interlaced = 0;
	mdtd->v_sync_polarity = 0;
	mdtd->h_sync_polarity = 0;
}

bool dptx_check_low_temperature(struct dp_ctrl *dptx)
{
	uint32_t perictrl4;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return((dptx == NULL), FALSE, ERR, "[DP] NULL Pointer!\n");

	hisifd = dptx->hisifd;
	hisi_check_and_return((hisifd == NULL), FALSE, ERR, "[DP] hisifd is NULL!\n");

	perictrl4 = inp32(hisifd->pmctrl_base + MIDIA_PERI_CTRL4);//lint !e529
	perictrl4 &= PMCTRL_PERI_CTRL4_TEMPERATURE_MASK;
	perictrl4 = (perictrl4 >> PMCTRL_PERI_CTRL4_TEMPERATURE_SHIFT);
	HISI_FB_INFO("[DP] Get current temperature: %d\n", perictrl4);

	if (perictrl4 != NORMAL_TEMPRATURE)
		return TRUE;
	else
		return FALSE;
}

void dptx_notify(struct dp_ctrl *dptx)
{
	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

	wake_up_interruptible(&dptx->waitq);
}

void dptx_notify_shutdown(struct dp_ctrl *dptx)
{
	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

	atomic_set(&dptx->shutdown, 1);
	wake_up_interruptible(&dptx->waitq);
}

int dp_ceil(uint64_t a, uint64_t b)
{
	if (b == 0)
		return -1;

	if (a % b != 0)
		return a / b + 1;

	return a / b;
}


uint64_t dp_get_pxl_clk_rate(struct dp_ctrl *dptx)
{
	hisi_check_and_return((dptx == NULL), 0, ERR, "[DP] NULL Pointer\n");

	return dptx->vparams.mdtd.pixel_clock * 1000;
}

int dp_pxl_ppll7_init(struct hisi_fb_data_type *hisifd, uint64_t pixel_clock)
{
	uint64_t refdiv, fbdiv, frac, postdiv1, postdiv2;
	uint64_t vco_min_freq_output = KIRIN_VCO_MIN_FREQ_OUPUT;
	uint64_t sys_clock_fref = KIRIN_SYS_FREQ;
	uint64_t ppll7_freq_divider;
	uint64_t vco_freq_output;
	uint64_t frac_range = 0x1000000; /*2^24*/
	uint64_t pixel_clock_cur;
	uint32_t ppll7ctrl0;
	uint32_t ppll7ctrl1;
	uint32_t ppll7ctrl0_val;
	uint32_t ppll7ctrl1_val;
	uint8_t i, j;
	int ret;
	int ceil_temp;
	int freq_divider_list[22] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		12, 14, 15, 16, 20, 21, 24,
		25, 30, 36, 42, 49
	};
	int postdiv1_list[22] = {
		1, 2, 3, 4, 5, 6, 7, 4, 3, 5, 4,
		7, 5, 4, 5, 7, 6, 5, 6, 6, 7, 7
	};
	int postdiv2_list[22] = {
		1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 3,
		2, 3, 4, 4, 3, 4, 5, 5, 6, 6, 7
	};
	ret = 0;
	j = 1;
	postdiv1 = 0;
	postdiv2 = 0;
	if (pixel_clock == 0)
		return -EINVAL;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	pixel_clock_cur = pixel_clock;

	pixel_clock_cur = pixel_clock_cur / 1000;
	/* Fractional PLL can not output the so small clock */
	if (pixel_clock_cur * freq_divider_list[21] < vco_min_freq_output) {
		j = 2;
		pixel_clock_cur *= j; //multiple frequency
	}

	ceil_temp = dp_ceil(vco_min_freq_output, pixel_clock_cur);

	if (ceil_temp < 0)
		return -EINVAL;

	ppll7_freq_divider = (uint64_t)ceil_temp;/*lint !e571 */

	for (i = 0; i < 22; i++) {
		if (freq_divider_list[i] >= ppll7_freq_divider) {/*lint !e574 */
			ppll7_freq_divider = freq_divider_list[i];
#ifdef CONFIG_HISI_FB_V600
			postdiv1 = postdiv1_list[i] - 1;
			postdiv2 = postdiv2_list[i] - 1;
#else
			postdiv1 = postdiv1_list[i];
			postdiv2 = postdiv2_list[i];
#endif
			HISI_FB_INFO("[DP] postdiv1=0x%llx, POSTDIV2=0x%llx\n", postdiv1, postdiv2);/*lint !e559 */
			break;
		}
	}

	vco_freq_output = ppll7_freq_divider * pixel_clock_cur;
	if (vco_freq_output == 0)
		return -EINVAL;

	HISI_FB_INFO("[DP] vco_freq_output=0x%llx\n", vco_freq_output);

	ceil_temp = dp_ceil(400000, vco_freq_output);

	if (ceil_temp < 0)
		return -EINVAL;

	refdiv = ((vco_freq_output*ceil_temp) >= 494000) ? 1 : 2;
	HISI_FB_INFO("[DP] refdiv=0x%llx\n", refdiv);/*lint !e559 */

	fbdiv = (vco_freq_output * ceil_temp) * refdiv / sys_clock_fref;
	HISI_FB_INFO("[DP] fbdiv=0x%llx\n", fbdiv);/*lint !e559 */

	frac = (uint64_t)(ceil_temp * vco_freq_output - sys_clock_fref / refdiv*fbdiv) * refdiv * frac_range;
	frac = (uint64_t)frac / sys_clock_fref;
	HISI_FB_INFO("[DP] frac=0x%llx\n", frac);/*lint !e559 */


	ppll7ctrl0 = inp32(hisifd->pmctrl_base + MIDIA_PPLL7_CTRL0);
	ppll7ctrl0 &= ~MIDIA_PPLL7_FREQ_DEVIDER_MASK;/*lint !e648 */

	ppll7ctrl0_val = 0x0;
	ppll7ctrl0_val |= (uint32_t)(postdiv2 << 23 | postdiv1 << 20 | fbdiv << 8 | refdiv << 2);
	ppll7ctrl0_val &= MIDIA_PPLL7_FREQ_DEVIDER_MASK;/*lint !e648 */
	ppll7ctrl0 |= ppll7ctrl0_val;
#ifdef CONFIG_HISI_FB_V600
	outp32(hisifd->peri_crg_base + MIDIA_PPLL7_CTRL0, ppll7ctrl0);
#else
	outp32(hisifd->pmctrl_base + MIDIA_PPLL7_CTRL0, ppll7ctrl0);
#endif
	ppll7ctrl1 = inp32(hisifd->pmctrl_base + MIDIA_PPLL7_CTRL1);
	ppll7ctrl1 &= ~MIDIA_PPLL7_FRAC_MODE_MASK;

	ppll7ctrl1_val = 0x0;
	ppll7ctrl1_val |= (uint32_t)(1 << 25 | 0 << 24 | frac);
	ppll7ctrl1_val &= MIDIA_PPLL7_FRAC_MODE_MASK;
	ppll7ctrl1 |= ppll7ctrl1_val;
#ifdef CONFIG_HISI_FB_V600
	outp32(hisifd->peri_crg_base + MIDIA_PPLL7_CTRL1, ppll7ctrl1);
#else
	outp32(hisifd->pmctrl_base + MIDIA_PPLL7_CTRL1, ppll7ctrl1);
#endif
	/*comfirm ldi1 switch ppll7*/
	pixel_clock_cur = pixel_clock_cur * 1000;

	ret = hisifb_set_dss_external_vote_pre(hisifd, pixel_clock_cur/j);
	if (ret < 0) {
		HISI_FB_ERR("[DP] fb%d dss_pxl1_clk clk_set_rate(%llu) set vote failed, error=%d!\n",
			hisifd->index, pixel_clock_cur, ret);
	}

	ret = clk_set_rate(hisifd->dss_pxl1_clk, DEFAULT_MIDIA_PPLL7_CLOCK_FREQ / j);

	if (ret < 0) {
		HISI_FB_ERR("[DP] fb%d dss_pxl1_clk clk_set_rate(%llu) failed, error=%d!\n",
			hisifd->index, pixel_clock_cur, ret);
	}
	return ret;
}

static int dp_auxclk_enable(struct hisi_fb_data_type *hisifd)
{
	struct clk *clk_tmp = NULL;
	int ret;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	clk_tmp = hisifd->dss_auxclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		ret = clk_prepare(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_auxclk_dpctrl_clk clk_prepare failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}

		ret = clk_enable(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_auxclk_dpctrl_clk clk_enable failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}
	}

	return 0;
}

static int dp_pclk_enable(struct hisi_fb_data_type *hisifd)
{
	struct clk *clk_tmp = NULL;
	int ret;
#if defined(CONFIG_HISI_FB_V600)
	uint32_t reg;
	struct dp_ctrl *dptx = NULL;
#endif

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

#if defined(CONFIG_HISI_FB_V600)
	dptx = &(hisifd->dp);
	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	if (dptx->ctrl_type == HIDPC) {
		reg = inp32(hisifd->mmc0_crg + HIDPC_PCLK_HSDT1_GATE_OFFSET); /* 0xF8481000 + 0x10 */
		reg |= HIDPC_PCLK_HSDT1_GATE_MASK; /* set bit1:0 to 3 */
		outp32(hisifd->mmc0_crg + HIDPC_PCLK_HSDT1_GATE_OFFSET, reg);
	}
#endif

	clk_tmp = hisifd->dss_pclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		ret = clk_prepare(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_pclk_dpctrl_clk clk_prepare failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}

		ret = clk_enable(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_pclk_dpctrl_clk clk_enable failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}
	}

	return 0;
}

static int dp_aclk_enable(struct hisi_fb_data_type *hisifd)
{
	struct clk *clk_tmp = NULL;
	int ret;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	clk_tmp = hisifd->dss_aclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		ret = clk_prepare(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_aclk_dpctrl_clk clk_prepare failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}

		ret = clk_enable(clk_tmp);
		if (ret) {
			HISI_FB_ERR("[DP] fb%d dss_aclk_dpctrl_clk clk_enable failed, error=%d!\n",
				hisifd->index, ret);
			return -EINVAL;
		}
	}

	return 0;
}

static int dp_clk_enable(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	ret = dp_auxclk_enable(hisifd);
	if (ret)
		return -EINVAL;

	ret = dp_pclk_enable(hisifd);
	if (ret)
		return -EINVAL;

	ret = dp_aclk_enable(hisifd);
	if (ret)
		return -EINVAL;

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return 0;//lint !e438
} /*lint !e550*/

static int dp_clk_disable(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct clk *clk_tmp = NULL;
#if defined(CONFIG_HISI_FB_V600)
	uint32_t reg;
	struct dp_ctrl *dptx = NULL;
#endif

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

#if defined(CONFIG_HISI_FB_V600)
	dptx = &(hisifd->dp);
	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");
#endif

	clk_tmp = hisifd->dss_auxclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		clk_disable(clk_tmp);
		clk_unprepare(clk_tmp);
	}

#if defined(CONFIG_HISI_FB_V600)
	if (dptx->ctrl_type == HIDPC) {
		reg = inp32(hisifd->mmc0_crg + HIDPC_PCLK_HSDT1_GATE_CLOSE_OFFSET); /* 0xF8481000 + 0x14 */
		reg |= HIDPC_PCLK_HSDT1_GATE_MASK; /* set bit1:0 to 3 */
		outp32(hisifd->mmc0_crg + HIDPC_PCLK_HSDT1_GATE_CLOSE_OFFSET, reg);
	}
#endif

	clk_tmp = hisifd->dss_pclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		clk_disable(clk_tmp);
		clk_unprepare(clk_tmp);
	}

	clk_tmp = hisifd->dss_aclk_dpctrl_clk;
	if (clk_tmp != NULL) {
		clk_disable(clk_tmp);
		clk_unprepare(clk_tmp);
	}

	return 0;
}

static int dp_clock_setup(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;
	uint32_t default_aclk_dpctrl_rate;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	hisifd->dss_auxclk_dpctrl_clk = devm_clk_get(&pdev->dev, hisifd->dss_auxclk_dpctrl_name);
	if (IS_ERR(hisifd->dss_auxclk_dpctrl_clk)) {
		ret = PTR_ERR(hisifd->dss_auxclk_dpctrl_clk);//lint !e712
		HISI_FB_ERR("[DP] fb%d %s get fail ret = %d.\n",
			hisifd->index, hisifd->dss_auxclk_dpctrl_name, ret);
		return ret;
	}

	ret = clk_set_rate(hisifd->dss_auxclk_dpctrl_clk, DEFAULT_AUXCLK_DPCTRL_RATE);
	if (ret < 0) {
		HISI_FB_ERR("[DP] fb%d dss_auxclk_dpctrl_clk clk_set_rate(%lu) failed, error=%d!\n",
			hisifd->index, DEFAULT_AUXCLK_DPCTRL_RATE, ret);
		return -EINVAL;
	}
	HISI_FB_INFO("[DP] dss_auxclk_dpctrl_clk:[%lu]->[%lu].\n",
		DEFAULT_AUXCLK_DPCTRL_RATE, clk_get_rate(hisifd->dss_auxclk_dpctrl_clk));

	default_aclk_dpctrl_rate = DEFAULT_ACLK_DPCTRL_RATE;

	hisifd->dss_aclk_dpctrl_clk = devm_clk_get(&pdev->dev, hisifd->dss_aclk_dpctrl_name);
	if (IS_ERR(hisifd->dss_aclk_dpctrl_clk)) {
		ret = PTR_ERR(hisifd->dss_aclk_dpctrl_clk);//lint !e712
		HISI_FB_ERR("[DP] fb%d dss_aclk_dpctrl_clk get fail ret = %d.\n",
			hisifd->index, ret);
		return ret;
	}

	ret = clk_set_rate(hisifd->dss_aclk_dpctrl_clk, default_aclk_dpctrl_rate);
	if (ret < 0) {
		HISI_FB_ERR("[DP] fb%d dss_aclk_dpctrl_clk clk_set_rate(%u) failed, error=%d!\n",
			hisifd->index, default_aclk_dpctrl_rate, ret);/*lint !e559 */
		return -EINVAL;
	}
	HISI_FB_INFO("[DP] dss_aclk_dpctrl_clk:[%u]->[%lu].\n",
		default_aclk_dpctrl_rate, clk_get_rate(hisifd->dss_aclk_dpctrl_clk));

	hisifd->dss_pclk_dpctrl_clk = devm_clk_get(&pdev->dev, hisifd->dss_pclk_dpctrl_name);
	if (IS_ERR(hisifd->dss_pclk_dpctrl_clk)) {
		ret = PTR_ERR(hisifd->dss_pclk_dpctrl_clk);//lint !e712
		HISI_FB_ERR("[DP] fb%d dss_pclk_dpctrl_clk get fail ret = %d.\n",
			hisifd->index, ret);
		return ret;
	}

	return 0;
}

static int dp_irq_setup(struct dp_ctrl *dptx)
{
	int ret;

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");
	hisi_check_and_return((dptx->dptx_irq == NULL), -EINVAL, ERR, "[DP] dptx->dptx_irq is NULL!\n");
	hisi_check_and_return((dptx->dptx_threaded_irq == NULL), -EINVAL, ERR,
		"[DP] dptx->dptx_threaded_irq is NULL!\n");

	ret = devm_request_threaded_irq(dptx->dev,
		dptx->irq, dptx->dptx_irq, dptx->dptx_threaded_irq,
		IRQF_SHARED | IRQ_LEVEL, "dwc_dptx", (void *)dptx->hisifd);//lint !e747
	if (ret) {
		HISI_FB_ERR("[DP] Request for irq %d failed!\n", dptx->irq);
		return -EINVAL;
	}
	disable_irq(dptx->irq);

	return 0;
}

#if defined(CONFIG_HISI_FB_V600)
static void dp_select_controller(struct dp_ctrl *dptx)
{
	uint32_t reg;
	struct hisi_fb_data_type *hisifd = NULL;

	if (g_fpga_flag == 1)
		return;

	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

	hisifd = dptx->hisifd;
	hisi_check_and_no_retval((hisifd == NULL), ERR, "[DP] hisifd is NULL!\n");

	reg = inp32(hisifd->usb_dp_ctrl_base + CTRL_TYPE_SELECT_OFFSET); /* 0xF8689000 + 0x54 */

	if (dptx->ctrl_type == HIDPTX)
		reg &= ~CTRL_TYPE_SELECT_BIT; /* set bit3 to 0 */
	else
		reg |= CTRL_TYPE_SELECT_BIT; /* set bit3 to 1 */

	outp32(hisifd->usb_dp_ctrl_base + CTRL_TYPE_SELECT_OFFSET, reg);
}
#endif

static void dp_on_params_init(struct dp_ctrl *dptx)
{
	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

	dptx->dptx_enable = true;
	dptx->detect_times = 0;
	dptx->current_link_rate = dptx->max_rate;
	dptx->current_link_lanes = dptx->max_lanes;
	bpress_powerkey = false;
	btrigger_timeout = false;
}

static int dp_on(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	int ret = 0;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);
	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	mutex_lock(&dptx->dptx_mutex);

	if (dptx->dptx_enable) {
		HISI_FB_INFO("[DP] dptx has already on\n");
		mutex_unlock(&dptx->dptx_mutex);
		return 0;
	}

#if defined(CONFIG_HISI_FB_V600)
	dp_select_controller(dptx);
#endif

	if (dptx->dp_dis_reset)
		ret = dptx->dp_dis_reset(hisifd, true);
	if (ret) {
		HISI_FB_ERR("[DP] DPTX dis reset failed !!!\n");
		ret = -ENODEV;
		goto err_out;
	}

	ret = dp_clk_enable(pdev);
	if (ret) {
		HISI_FB_ERR("[DP] DPTX dp clock enable failed !!!\n");
		ret = -ENODEV;
		goto err_out;
	}

	if (dptx->dptx_core_on)
		ret = dptx->dptx_core_on(dptx);
	if (ret) {
		ret = -ENODEV;
		goto err_out;
	}

	dp_on_params_init(dptx);

	ret = panel_next_on(pdev);
	if (ret)
		HISI_FB_ERR("[DP] panel_next_on failed!\n");

err_out:
	mutex_unlock(&dptx->dptx_mutex);

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}

static void dp_off_params_reset(struct dp_ctrl *dptx)
{
	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

	dptx->detect_times = 0;
	dptx->dptx_vr = false;
	dptx->dptx_enable = false;
	dptx->video_transfer_enable = false;
	dptx->dptx_plug_type = DP_PLUG_TYPE_NORMAL;
	bpress_powerkey = false;
	btrigger_timeout = false;
}

static int dp_off(struct platform_device *pdev)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");
	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	mutex_lock(&dptx->dptx_mutex);

	if (!dptx->dptx_enable) {
		HISI_FB_INFO("[DP] dptx has already off.\n");
		ret = 0;
		goto err_out;
	}

	if (dptx->video_transfer_enable) {
		if (dptx->handle_hotunplug)
			dptx->handle_hotunplug(hisifd);
		mdelay(10);
	}

	if (dptx->dptx_core_off)
		dptx->dptx_core_off(hisifd, dptx);

	dp_clk_disable(pdev);

	if (dptx->dp_dis_reset) {
		ret = dptx->dp_dis_reset(hisifd, false);
		if (ret) {
			HISI_FB_ERR("[DP] DPTX dis reset failed !!!\n");
			ret = -ENODEV;
		}
	}

	dp_off_params_reset(dptx);

	ret = panel_next_off(pdev);
	if (ret)
		HISI_FB_ERR("[DP] Panel DP next off error !!\n");

err_out:
	mutex_unlock(&dptx->dptx_mutex);

	HISI_FB_INFO("fb%d, -.\n", hisifd->index);
	return ret;
}

static int dp_resume(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	int ret;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);
	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	if (dptx->dptx_enable) {
		ret = dp_off(g_dp_pdev);
		if (ret)
			HISI_FB_ERR("[DP] dp_off failed!\n");
		msleep(10);
		ret = dp_on(g_dp_pdev);
		if (ret)
			HISI_FB_ERR("[DP] dp_on failed!\n");
	}

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return 0;
}

void dp_send_cable_notification(struct dp_ctrl *dptx, int val)
{
	int state = 0;
	struct dtd *mdtd = NULL;
	struct video_params *vparams = NULL;

	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

#if CONFIG_DP_ENABLE
	state = dptx->sdev.state;
	switch_set_state(&dptx->sdev, val);
	if (dptx->edid_info.Audio.basicAudio == 0x1) {
		if (val == HOT_PLUG_OUT || val == HOT_PLUG_OUT_VR)
			switch_set_state(&dptx->dp_switch, 0);
		else if (val == HOT_PLUG_IN || val == HOT_PLUG_IN_VR)
			switch_set_state(&dptx->dp_switch, 1);
	} else {
		HISI_FB_WARNING("[DP] basicAudio(%ud) no support!\n", dptx->edid_info.Audio.basicAudio);
	}

	vparams = &(dptx->vparams);
	mdtd = &(dptx->vparams.mdtd);

	huawei_dp_update_external_display_timming_info(mdtd->h_active, mdtd->v_active, vparams->m_fps);

	HISI_FB_INFO("[DP] cable state %s %d\n",
		dptx->sdev.state == state ? "is same" : "switched to", dptx->sdev.state);
#endif
}

int dp_device_srs(struct hisi_fb_data_type *hisifd, bool ublank)
{
	struct dp_ctrl *dptx = NULL;
	struct hisi_panel_info *pinfo = NULL;
	int retval = 0;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);
	pinfo = &(hisifd->panel_info);

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");
	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	if (dptx->dptx_power_handler)
		retval = dptx->dptx_power_handler(dptx, ublank, &bpress_powerkey);

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	if (retval)
		return retval;
	return 0;
}

int dp_get_color_bit_mode(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	struct dp_ctrl *dptx = NULL;
	int ret;

	hisi_check_and_return((argp == NULL), -EINVAL, ERR, "[DP] argp is NULL!\n");
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	ret = (int)copy_to_user(argp, &(dptx->vparams.bpc), sizeof(dptx->vparams.bpc));
	if (ret) {
		HISI_FB_ERR("[DP]  copy_to_user failed! ret=%d.\n", ret);
		return ret;
	}

	return ret;
}

int dp_get_source_mode(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	struct dp_ctrl *dptx = NULL;
	int ret;

	hisi_check_and_return((argp == NULL), -EINVAL, ERR, "[DP] argp is NULL!\n");
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

#if CONFIG_DP_ENABLE
	dptx->same_source = huawei_dp_get_current_dp_source_mode();
#endif
	ret = (int)copy_to_user(argp, &(dptx->same_source), sizeof(dptx->same_source));
	if (ret) {
		HISI_FB_ERR("[DP]  copy_to_user failed! ret=%d.\n", ret);
		return ret;
	}

	return ret;
}

int dp_wakeup(struct hisi_fb_data_type *hisifd)
{
	struct dp_ctrl *dptx = NULL;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");
#if defined(CONFIG_HISI_FB_V600)
	wake_up_interruptible(&dptx->dptxq);
#else
	if (dptx->dptx_enable)
		wake_up_interruptible(&dptx->dptxq);
#endif

	return 0;
}
/*******************************************************************************
 *
 */
int hisi_dptx_main_panel_blank(bool b_blank_on)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;

	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");
	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);
	ret = 0;

	mutex_lock(&dptx->dptx_mutex);

	if (!dptx->dptx_enable) {
		HISI_FB_ERR("[DP] dptx has already off.\n");
		ret = -EINVAL;
		goto fail;
	}

	if (!dptx->video_transfer_enable) {
		HISI_FB_ERR("[DP] dptx never transfer video.\n");
		ret = -EINVAL;
		goto fail;
	}

	if (b_blank_on) {
		if (btrigger_timeout) {
			dp_send_cable_notification(dptx, HOT_PLUG_MAINPANEL_UP);
			btrigger_timeout = false;
		} else {
			HISI_FB_ERR("[DP] dptx receive repeat CMD to power on the main panel !!!\n");
			ret = -1;
		}
	} else {
		if (!btrigger_timeout) {
			dp_send_cable_notification(dptx, HOT_PLUG_MAINPANEL_DOWN);
			btrigger_timeout = true;
		} else {
			HISI_FB_ERR("[DP] dptx receive repeat CMD to power down the main panel !!!\n");
			ret = -1;
		}
	}

fail:
	mutex_unlock(&dptx->dptx_mutex);
	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_main_panel_blank);

int hisi_dptx_switch_source(uint32_t user_mode, uint32_t user_format)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	struct hisi_panel_info *pinfo = NULL;

	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);

	mutex_lock(&dptx->dptx_mutex);

	if (!dptx->dptx_enable) {
		HISI_FB_ERR("[DP] dptx has already off\n");
		ret = -EINVAL;
		goto fail;
	}

	if (!dptx->video_transfer_enable) {
		HISI_FB_ERR("[DP] dptx never transfer video\n");
		ret = -EINVAL;
		goto fail;
	}

#if CONFIG_DP_ENABLE
	if ((dptx->same_source == huawei_dp_get_current_dp_source_mode()) && (!user_mode) && (!user_format)) {
		HISI_FB_ERR("[DP] dptx don't switch source when the dest mode is same as current!!!\n");
		ret = -EINVAL;
		goto fail;
	}

	dptx->user_mode = user_mode;
	dptx->user_mode_format = (enum video_format_type) user_format;
	dptx->same_source = huawei_dp_get_current_dp_source_mode();
#endif
	HISI_FB_INFO("[DP] dptx user switch: mode (%d); format (%d); same_source (%d).\n",
		dptx->user_mode, dptx->user_mode_format, dptx->same_source);

	pinfo = &(hisifd->panel_info);

	/*PC mode change to 1080p*/
	if (((pinfo->xres >= MAX_DIFF_SOURCE_X_SIZE) && (dptx->max_edid_timing_hactive > MAX_DIFF_SOURCE_X_SIZE))
		|| (dptx->user_mode != 0)) {
		/* DP plug out */
		if (dptx->handle_hotunplug)
			dptx->handle_hotunplug(hisifd);
		mutex_unlock(&dptx->dptx_mutex);

		msleep(10);

		mutex_lock(&dptx->dptx_mutex);
		if (dptx->handle_hotplug)
			ret = dptx->handle_hotplug(hisifd);
		huawei_dp_imonitor_set_param(DP_PARAM_HOTPLUG_RETVAL, &ret);
	}
fail:
	mutex_unlock(&dptx->dptx_mutex);

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_switch_source);

static int dp_get_lanes_mode(TCPC_MUX_CTRL_TYPE mode, uint8_t *dp_lanes)
{
	hisi_check_and_return((dp_lanes == NULL), -EINVAL, ERR, "[DP] dp_lanes is NULL!\n");

	switch (mode) {
	case TCPC_DP:
		*dp_lanes = 4;
		break;
	case TCPC_USB31_AND_DP_2LINE:
		*dp_lanes = 2;
		break;
	default:
		HISI_FB_ERR("[DP] not supported tcpc_mux_ctrl_type=%d.\n", mode);
		return -EINVAL;
	}

	return 0;
}

static void dp_hpd_status_init(struct dp_ctrl *dptx, TYPEC_PLUG_ORIEN_E typec_orien)
{
	hisi_check_and_no_retval((dptx == NULL), ERR, "[DP] dptx is NULL!\n");

#if CONFIG_DP_ENABLE
	dptx->same_source = huawei_dp_get_current_dp_source_mode();
#endif
	dptx->user_mode = 0;
	dptx->dptx_plug_type = typec_orien;
	dptx->user_mode_format = VCEA;

	/* DP HPD event must be delayed when system is booting*/
	if (!dptx->dptx_gate)
		wait_event_interruptible_timeout(dptx->dptxq, dptx->dptx_gate, msecs_to_jiffies(20000)); /*lint !e666 !e578 */
}

#if defined(CONFIG_HISI_FB_V600)
int hisi_dptx_hpd_trigger(TCA_IRQ_TYPE_E irq_type, TCPC_MUX_CTRL_TYPE mode, TYPEC_PLUG_ORIEN_E typec_orien)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	uint8_t dp_lanes = 0;

	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +\n", hisifd->index);

	dptx = &(hisifd->dp);

	mutex_lock(&dptx->dptx_mutex);

	HISI_FB_INFO("[DP] DP HPD Type: %d, Mode: %d, Gate: %d\n", irq_type, mode, dptx->dptx_gate);

	dp_hpd_status_init(dptx, typec_orien);
	ret = dp_get_lanes_mode(mode, &dp_lanes);
	if (ret)
		goto fail;

	if (!dptx->dptx_enable && irq_type != TCA_IRQ_HPD_IN) {
		HISI_FB_INFO("[DP] dptx has already off.\n");
		ret = -EINVAL;
		goto fail;
	}

	if (dptx->dptx_enable && irq_type == TCA_IRQ_HPD_IN) {
		HISI_FB_INFO("[DP] dptx has already plug in.\n");
		ret = -EINVAL;
		goto fail;
	}

	switch (irq_type) {
	case TCA_IRQ_HPD_OUT:
		if (dptx->dptx_hpd_handler)
			dptx->dptx_hpd_handler(dptx, false, dp_lanes);

		mutex_unlock(&dptx->dptx_mutex);
		mdelay(1);
		ret = dp_off(g_dp_pdev);
		if (ret)
			HISI_FB_ERR("[DP] dp_off failed!\n");
		mutex_lock(&dptx->dptx_mutex);
		mdelay(1);

		break;
	case TCA_IRQ_HPD_IN:
		mutex_unlock(&dptx->dptx_mutex);
		mdelay(1);
		ret = dp_on(g_dp_pdev);
		mutex_lock(&dptx->dptx_mutex);
		mdelay(1);
		if (ret) {
			HISI_FB_ERR("[DP] dp_on failed!\n");
			goto fail;
		}

		if (dptx->dptx_hpd_handler)
			dptx->dptx_hpd_handler(dptx, true, dp_lanes);
		break;
	case TCA_IRQ_SHORT:
		if (dptx->dptx_hpd_irq_handler)
			dptx->dptx_hpd_irq_handler(dptx);
		break;
	default:
		HISI_FB_ERR("[DP] not supported tca_irq_type=%d\n", irq_type);
		ret = -EINVAL;
		goto fail;
	}

fail:
	mutex_unlock(&dptx->dptx_mutex);

	HISI_FB_INFO("[DP] fb%d, -\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_hpd_trigger);

int hisi_dptx_triger(bool enable)
{
	return 0;
}
EXPORT_SYMBOL_GPL(hisi_dptx_triger);

int hisi_dptx_notify_switch(void)
{
	int ret = 0;
	uint32_t reg;
	uint32_t dp_ctrl_mode;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_pdev == NULL) {
		HISI_FB_ERR("[DP] g_dp_pdev is NULL!\n");
		/* return 0 to ensure USB flow when DP is absent.*/
		return 0;
	}

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);

	HISI_FB_INFO("[DP] fb%d, + [DP] Status: %d.\n", hisifd->index, dptx->dptx_enable);

	reg = inp32(hisifd->mmc0_crg + DPTX_CTRL_STATUS_OFFSET); /* 0xF8481000 + 0x28 */
	if (dptx->ctrl_type == HIDPTX)
		dp_ctrl_mode = reg & HIDPTX_CTRL_RESET_STATUS_BIT;
	else
		dp_ctrl_mode = reg & HIDPC_CTRL_RESET_STATUS_BIT;

	if (dp_ctrl_mode != 0) /* dp ctrl is in reset mode, just return */
		return 0;

	ret = dp_off(g_dp_pdev);
	if (ret)
		HISI_FB_ERR("[DP] dp_off failed!\n");

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_notify_switch);

#else
int hisi_dptx_hpd_trigger(TCA_IRQ_TYPE_E irq_type, TCPC_MUX_CTRL_TYPE mode, TYPEC_PLUG_ORIEN_E typec_orien)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	uint8_t dp_lanes = 0;

	hisi_check_and_return((g_dp_pdev == NULL), -EINVAL, ERR, "[DP] g_dp_pdev is NULL!\n");

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);
	ret = 0;

	mutex_lock(&dptx->dptx_mutex);

	if (!dptx->dptx_enable) {
		HISI_FB_INFO("[DP] dptx has already off.\n");
		ret = -EINVAL;
		goto fail;
	}

	HISI_FB_INFO("[DP] DP HPD Type: %d, Mode: %d, Gate: %d\n", irq_type, mode, dptx->dptx_gate);

	dp_hpd_status_init(dptx, typec_orien);
	ret = dp_get_lanes_mode(mode, &dp_lanes);
	if (ret)
		goto fail;

	switch (irq_type) {
	case TCA_IRQ_HPD_OUT:
		if (dptx->dptx_hpd_handler)
			dptx->dptx_hpd_handler(dptx, false, dp_lanes);
		break;
	case TCA_IRQ_HPD_IN:
		if (dptx->dptx_hpd_handler)
			dptx->dptx_hpd_handler(dptx, true, dp_lanes);
		break;
	case TCA_IRQ_SHORT:
		if (dptx->dptx_hpd_irq_handler)
			dptx->dptx_hpd_irq_handler(dptx);
		break;
	default:
		HISI_FB_ERR("[DP] not supported tca_irq_type=%d.\n", irq_type);
		ret = -EINVAL;
		goto fail;
	}

fail:
	mutex_unlock(&dptx->dptx_mutex);

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_hpd_trigger);

int hisi_dptx_triger(bool enable)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	int ret;

	if (g_dp_pdev == NULL) {
		HISI_FB_ERR("[DP] g_dp_pdev is NULL!\n");
		/* return 0 to ensure USB flow when DP is absent.*/
		return 0;
	}

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +. DP Device %s\n", hisifd->index, enable ? "ON" : "OFF");

	dptx = &(hisifd->dp);

	if (enable) {
		ret = dp_on(g_dp_pdev);
		if (ret)
			HISI_FB_ERR("[DP] dp_on failed!\n");
	} else {
		ret = dp_off(g_dp_pdev);
		if (ret)
			HISI_FB_ERR("[DP] dp_off failed!\n");
	}

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}
EXPORT_SYMBOL_GPL(hisi_dptx_triger);

int hisi_dptx_notify_switch(void)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	bool lanes_status_change = false;

	if (g_dp_pdev == NULL) {
		HISI_FB_ERR("[DP] g_dp_pdev is NULL!\n");
		/* return 0 to ensure USB flow when DP is absent.*/
		return 0;
	}

	hisifd = platform_get_drvdata(g_dp_pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	dptx = &(hisifd->dp);

	HISI_FB_INFO("[DP] fb%d, + [DP] Status: %d.\n", hisifd->index, dptx->dptx_enable);
	mutex_lock(&dptx->dptx_mutex);

	if (dptx->dptx_enable) {
		lanes_status_change = true;
		if (dptx->dptx_free_lanes)
			dptx->dptx_free_lanes(dptx);
	}

	mutex_unlock(&dptx->dptx_mutex);

	if (!lanes_status_change) {
		ret = dp_on(g_dp_pdev);
		if (ret) {
			HISI_FB_ERR("[DP] DP on failed!\n");
			return -EINVAL;
		}
	}

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return 0;
}
EXPORT_SYMBOL_GPL(hisi_dptx_notify_switch);
#endif

bool hisi_dptx_ready(void)
{
	return (g_dp_pdev != NULL);
}
EXPORT_SYMBOL_GPL(hisi_dptx_ready);

/*******************************************************************************
 **
 */
static int dp_parse_dts_params(struct dp_ctrl *dptx)
{
	struct device_node *np = NULL;
	int i;
	int ret;

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

#if CONFIG_DP_ENABLE
	/* parse pre swing configure values */
	np = of_find_compatible_node(NULL, NULL, DTS_HISI_DP);
	if (np == NULL) {
		HISI_FB_ERR("[DP] NOT FOUND device node %s!\n", DTS_HISI_DP);
		return -EINVAL;
	}
	for (i = 0; i < DPTX_COMBOPHY_PARAM_NUM; i++) {
		ret = of_property_read_u32_index(np, "preemphasis_swing", i, &(dptx->combophy_pree_swing[i]));
		if (ret) {
			HISI_FB_ERR("[DP] preemphasis_swing[%d] is got fail\n", i);
			return -EINVAL;
		}
	}
#ifdef CONFIG_HISI_FB_V600
	/* parse ssc ppm configure values */
	for (i = 0; i < DPTX_COMBOPHY_SSC_PPM_PARAM_NUM; i++) {
		ret = of_property_read_u32_index(np, "ssc_ppm", i, &(dptx->combophy_ssc_ppm[i]));
		if (ret) {
			HISI_FB_ERR("[DP] combophy_ssc_ppm[%d] is got fail\n", i);
			return -EINVAL;
		}
	}
#endif
#endif

	np = of_find_compatible_node(NULL, NULL, DTS_DP_AUX_SWITCH);
	if (np == NULL) {
		dptx->edid_try_count = MAX_AUX_RETRY_COUNT;
		dptx->edid_try_delay = AUX_RETRY_DELAY_TIME;
	} else {
		ret = of_property_read_u32(np, "edid_try_count", &dptx->edid_try_count);
		if (ret < 0)
			dptx->edid_try_count = MAX_AUX_RETRY_COUNT;

		ret = of_property_read_u32(np, "edid_try_delay", &dptx->edid_try_delay);
		if (ret < 0)
			dptx->edid_try_delay = AUX_RETRY_DELAY_TIME;
	}
	HISI_FB_INFO("[DP] edid try count=%d, delay=%d ms.\n", dptx->edid_try_count, dptx->edid_try_delay);

	return 0;
}

static int dp_device_params_init(struct dp_ctrl *dptx)
{
	int ret;

	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	mutex_init(&dptx->dptx_mutex);
	init_waitqueue_head(&dptx->dptxq);
	init_waitqueue_head(&dptx->waitq);
	atomic_set(&(dptx->sink_request), 0);
	atomic_set(&(dptx->shutdown), 0);
	atomic_set(&(dptx->c_connect), 0);

#if defined(CONFIG_HISI_FB_V600)
	dptx->ctrl_type = HIDPTX;
#else
	dptx->ctrl_type = HIDPC;
#endif

	dptx->dummy_dtds_present = false;
	dptx->selected_est_timing = NONE;
	dptx->dptx_vr = false;
	dptx->dptx_gate = false;
	dptx->dptx_enable = false;
	dptx->video_transfer_enable = false;
	dptx->dptx_detect_inited = false;
	dptx->user_mode = 0;
	dptx->detect_times = 0;
	dptx->dptx_plug_type = DP_PLUG_TYPE_NORMAL;
	dptx->user_mode_format = VCEA;
	dptx->dsc_decoders = DSC_DEFAULT_DECODER;
#ifdef CONFIG_HISI_FB_V600
	if (g_fpga_flag == 1)
		dptx->dsc_ifbc_type = IFBC_TYPE_VESA2X_DUAL;
	else
		dptx->dsc_ifbc_type = IFBC_TYPE_VESA3X_DUAL;
#else
	dptx->dsc_ifbc_type = IFBC_TYPE_VESA3X_DUAL;
#endif

#if CONFIG_DP_ENABLE
	dptx->same_source = huawei_dp_get_current_dp_source_mode();
#endif
	dptx->max_edid_timing_hactive = 0;
	dptx->bstatus = 0;

	ret = dp_parse_dts_params(dptx);
	if (ret)
		ret = -ENOMEM;

#if CONFIG_DP_ENABLE
	huawei_dp_debug_init_combophy_pree_swing(dptx->combophy_pree_swing, DPTX_COMBOPHY_PARAM_NUM);
#endif

	return ret;
}

static int dp_device_buf_alloc(struct dp_ctrl *dptx)
{
	hisi_check_and_return((dptx == NULL), -EINVAL, ERR, "[DP] dptx is NULL!\n");

	dptx->edid_second = kzalloc(DPTX_DEFAULT_EDID_BUFLEN, GFP_KERNEL);
	if (dptx->edid_second == NULL) {
		HISI_FB_ERR("[DP] dptx base is NULL!\n");
		return -ENOMEM;
	}
	memset(&(dptx->edid_info), 0, sizeof(struct edid_information));

	dptx->edid = kzalloc(DPTX_DEFAULT_EDID_BUFLEN, GFP_KERNEL);
	if (dptx->edid == NULL) {
		HISI_FB_ERR("[DP] dptx base is NULL!\n");
		return -ENOMEM;
	}
	memset(dptx->edid, 0, DPTX_DEFAULT_EDID_BUFLEN);

	return 0;
}

static int dp_device_init(struct platform_device *pdev)
{
	struct dp_ctrl *dptx = NULL;
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d +.\n", hisifd->index);

	ret = dp_clock_setup(pdev);
	if (ret) {
		HISI_FB_ERR("[DP] DP clock setup failed!\n");
		return ret;
	}

	dptx = &(hisifd->dp);

	dptx->dev = &pdev->dev;
	dptx->base = hisifd->dp_base;
	dptx->hisifd = hisifd;
	if (IS_ERR(dptx->base)) {
		HISI_FB_ERR("[DP] dptx base is NULL!\n");
		return -EINVAL;
	}

	dptx->irq = hisifd->dp_irq;
	if (!dptx->irq) {
		HISI_FB_ERR("[DP] dptx irq is NULL!\n");
		return -EINVAL;
	}

	ret = dp_device_params_init(dptx);
	if (ret)
		return ret;

	dptx_link_layer_init(dptx);
	if (dptx->dptx_default_params_from_core)
		dptx->dptx_default_params_from_core(dptx);

	ret = dp_device_buf_alloc(dptx);
	if (ret)
		goto err_edid_alloc;

#if CONFIG_DP_ENABLE
	dptx->sdev.name = "hisi-dp";
	if (switch_dev_register(&dptx->sdev) < 0) {
		HISI_FB_ERR("[DP] dp switch registration failed!\n");
		ret = -ENODEV;
		goto err_edid_alloc;
	}

	dptx->dp_switch.name = "hdmi_audio";
	ret = switch_dev_register(&dptx->dp_switch);
	if (ret < 0) {
		HISI_FB_ERR("[DP] hdmi_audio switch device register error %d\n", ret);
		goto err_sdev_register;
	}
#endif

	ret = dp_irq_setup(dptx);
	if (ret)
		goto err_sdev_hdmi_register;

	hisifd->dp_device_srs = dp_device_srs;
	hisifd->dp_get_color_bit_mode = dp_get_color_bit_mode;
	hisifd->dp_get_source_mode = dp_get_source_mode;
	hisifd->dp_pxl_ppll7_init = dp_pxl_ppll7_init;
	hisifd->dp_get_pxl_clk_rate = dp_get_pxl_clk_rate;
	hisifd->dp_wakeup = dp_wakeup;

	hdcp_register();
	HISI_FB_INFO("[DP] fb%d -.\n", hisifd->index);

	return 0;
err_sdev_hdmi_register: /*lint !e563 */
#if CONFIG_DP_ENABLE
	switch_dev_unregister(&dptx->dp_switch);
#endif

err_sdev_register: /*lint !e563 */
#if CONFIG_DP_ENABLE
	switch_dev_unregister(&dptx->sdev);
#endif

err_edid_alloc: /*lint !e563 */
	if (dptx->edid != NULL) {
		kfree(dptx->edid);
		dptx->edid = NULL;
	}

	if (dptx->edid_second != NULL) {
		kfree(dptx->edid_second);
		dptx->edid_second = NULL;
	}
	return ret;
}

static int dp_remove(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] pdev is NULL!\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] hisifd is NULL!\n");

	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	dptx = &(hisifd->dp);
#if CONFIG_DP_ENABLE
	switch_dev_unregister(&dptx->sdev);
#endif

	if (dptx->edid != NULL) {
		kfree(dptx->edid);
		dptx->edid = NULL;
	}

	if (dptx->edid_second != NULL) {
		kfree(dptx->edid_second);
		dptx->edid_second = NULL;
	}

	dptx->dptx_vr = false;
	dptx->dptx_enable = false;
	dptx->video_transfer_enable = false;

	if (dptx->dptx_core_remove)
		dptx->dptx_core_remove(dptx);

	dptx_notify_shutdown(dptx);
	mdelay(15); //lint !e778 !e747 !e774 !e845

	ret = panel_next_remove(pdev);

	g_dp_pdev = NULL;

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	return ret;
}

static int dp_probe(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct platform_device *dpp_dev = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	int ret;

	hisi_check_and_return((pdev == NULL), -EINVAL, ERR, "[DP] NULL Pointer\n");

	hisifd = platform_get_drvdata(pdev);

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "[DP] NULL Pointer\n");
	HISI_FB_INFO("[DP] fb%d, +.\n", hisifd->index);

	ret = dp_device_init(pdev);
	if (ret) {
		HISI_FB_INFO("[DP] fb%d mipi_dsi_irq_clk_setup failed, error=%d!\n", hisifd->index, ret);
		return -EINVAL;
	}

	/* alloc device */
	dpp_dev = platform_device_alloc(DEV_NAME_DSS_DPE, pdev->id);

	if (dpp_dev == NULL) {
		HISI_FB_INFO("[DP] fb%d platform_device_alloc failed, error=%d!\n", hisifd->index, ret);
		ret = -ENOMEM;
		goto err_device_alloc;
	}

	/* link to the latest pdev */
	hisifd->pdev = dpp_dev;

	/* alloc panel device data */
	ret = platform_device_add_data(dpp_dev, dev_get_platdata(&pdev->dev),
		sizeof(struct hisi_fb_panel_data));
	if (ret) {
		HISI_FB_INFO("[DP] fb%d platform_device_add_data failed error=%d!\n", hisifd->index, ret);
		ret = -EINVAL;
		goto err_device_put;
	}

	/* data chain */
	pdata = dev_get_platdata(&dpp_dev->dev);
	pdata->on = NULL;
	pdata->off = NULL;
	pdata->remove = dp_remove;
	pdata->next = pdev;

	/* get/set panel info */
	memcpy(&hisifd->panel_info, pdata->panel_info, sizeof(struct hisi_panel_info));

	/* set driver data */
	platform_set_drvdata(dpp_dev, hisifd);
	/* device add */
	ret = platform_device_add(dpp_dev);
	if (ret) {
		dev_err(&pdev->dev, "fb%d platform_device_add failed, error=%d!\n", hisifd->index, ret);
		ret = -EINVAL;
		goto err_device_put;
	}

	g_dp_pdev = pdev;

	HISI_FB_INFO("[DP] fb%d, -.\n", hisifd->index);

	if (g_fpga_flag == 1)
		dp_on(pdev);

	return 0;

err_device_put:
	platform_device_put(dpp_dev);
err_device_alloc:
	return ret;
}

#if defined(CONFIG_HISI_FB_V600)
static void dptx_dp_shutdown(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct dp_ctrl *dptx = NULL;
	int ret;

	HISI_FB_INFO("[DP] +\n");
	hisifd = platform_get_drvdata(pdev);
	dptx = &(hisifd->dp);

	ret = dp_off(pdev);
	if (ret)
		HISI_FB_ERR("[DP] dp_off failed!\n");

	HISI_FB_INFO("[DP] -\n");
}
#endif

/*lint -save -e* */
static struct platform_driver this_driver = {
	.probe = dp_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = dp_resume,
#if defined(CONFIG_HISI_FB_V600)
	.shutdown = dptx_dp_shutdown,
#else
	.shutdown = NULL,
#endif
	.driver = {
		.name = DEV_NAME_DP,
	},
};
/*lint -restore*/

static int __init dp_driver_init(void)
{
	int ret;

	ret = platform_driver_register(&this_driver);//lint !e64

	if (ret) {
		HISI_FB_ERR("[DP] platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

/*lint -e528 -esym(528,*)*/
module_init(dp_driver_init);
/*lint -e528 +esym(528,*)*/
