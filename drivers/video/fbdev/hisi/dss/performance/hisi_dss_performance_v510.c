/* Copyright (c) 2019-2019, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "hisi_fb.h"

#if defined(CONFIG_HISI_PERIDVFS)
#include "peri_volt_poll.h"
#endif

#define PERI_VOLTAGE_LEVEL0_060V 0 // 0.60v
#define PERI_VOLTAGE_LEVEL1_065V 1 // 0.65v
#define PERI_VOLTAGE_LEVEL2_070V 2 // 0.70v
#define PERI_VOLTAGE_LEVEL3_080V 3 // 0.80v

#define LCD_4K_XRES 2160
#define LCD_4K_YRES 3840

bool is_vote_needed_for_low_temp(bool is_lowtemp, int volt_to_set)
{
	if (is_lowtemp && (volt_to_set == PERI_VOLTAGE_LEVEL3_080V)) {
		HISI_FB_INFO("is_lowtemp, vlotage cannot exceed 0.7v\n");
		return false;
	}
	return true;
}

int dpe_get_voltage_value(uint32_t dss_voltage_level)
{
	switch (dss_voltage_level) {
	case PERI_VOLTAGE_LEVEL0:
		return PERI_VOLTAGE_LEVEL0_060V;
	case PERI_VOLTAGE_LEVEL1:
		return PERI_VOLTAGE_LEVEL1_065V;
	case PERI_VOLTAGE_LEVEL2:
		return PERI_VOLTAGE_LEVEL2_070V;
	case PERI_VOLTAGE_LEVEL3:
		return PERI_VOLTAGE_LEVEL3_080V;
	default:
		HISI_FB_ERR("not support dss_voltage_level is %d\n",
			dss_voltage_level);
		return -1;
	}
}

int dpe_get_voltage_level(int votage_value)
{
	switch (votage_value) {
	case PERI_VOLTAGE_LEVEL0_060V:
		return PERI_VOLTAGE_LEVEL0;
	case PERI_VOLTAGE_LEVEL1_065V:
		return PERI_VOLTAGE_LEVEL1;
	case PERI_VOLTAGE_LEVEL2_070V:
		return PERI_VOLTAGE_LEVEL2;
	case PERI_VOLTAGE_LEVEL3_080V:
		return PERI_VOLTAGE_LEVEL3;
	default:
		HISI_FB_ERR("not support votage_value is %d\n", votage_value);
		return PERI_VOLTAGE_LEVEL0;
	}
}

static int get_mdc_clk_rate(dss_vote_cmd_t vote_cmd, uint64_t *clk_rate)
{
	switch (vote_cmd.dss_voltage_level) {
	case PERI_VOLTAGE_LEVEL0:
		*clk_rate = DEFAULT_MDC_CORE_CLK_RATE_L1;
		break;
	case PERI_VOLTAGE_LEVEL1:
		*clk_rate = DEFAULT_MDC_CORE_CLK_RATE_L2;
		break;
	case PERI_VOLTAGE_LEVEL2:
		*clk_rate = DEFAULT_MDC_CORE_CLK_RATE_L3;
		break;
	case PERI_VOLTAGE_LEVEL3:
		*clk_rate = DEFAULT_MDC_CORE_CLK_RATE_L4;
		break;
	default:
		HISI_FB_ERR("no support set dss_voltage_level %d!\n",
			vote_cmd.dss_voltage_level);
		return -1;
	}
	HISI_FB_DEBUG("get mdc clk rate: %llu\n", *clk_rate);
	return 0;
}

int set_mdc_core_clk(struct hisi_fb_data_type *hisifd, dss_vote_cmd_t vote_cmd)
{
	int ret;
	uint64_t clk_rate = 0;

	if (vote_cmd.dss_voltage_level == hisifd->dss_vote_cmd.dss_voltage_level)
		return 0;

	if (get_mdc_clk_rate(vote_cmd, &clk_rate)) {
		HISI_FB_ERR("get mdc clk rate failed!\n");
		return -1;
	}

	HISI_FB_INFO_INTERIM("Trying to set media_common_clk from %llu to %llu\n",
		clk_get_rate(hisifd->dss_clk_media_common_clk), clk_rate);

	ret = clk_set_rate(hisifd->dss_clk_media_common_clk, clk_rate);
	hisi_check_and_return((ret < 0), -1, ERR,
		"set dss_clk_media_common_clk(%llu) failed, error=%d!\n", clk_rate, ret);

	hisifd->dss_vote_cmd.dss_voltage_level = vote_cmd.dss_voltage_level;

	HISI_FB_INFO("set dss_clk_media_common_clk = %llu\n", clk_rate);

	return ret;
}

static uint32_t get_clk_vote_level(dss_vote_cmd_t vote_cmd)
{
	uint32_t level = PERI_VOLTAGE_LEVEL0;

	switch (vote_cmd.dss_pri_clk_rate) {
	case DEFAULT_DSS_CORE_CLK_RATE_L4:
		level = PERI_VOLTAGE_LEVEL3;
		break;
	case DEFAULT_DSS_CORE_CLK_RATE_L3:
		level = PERI_VOLTAGE_LEVEL2;
		break;
	case DEFAULT_DSS_CORE_CLK_RATE_L2:
		level = PERI_VOLTAGE_LEVEL1;
		break;
	case DEFAULT_DSS_CORE_CLK_RATE_L1:
		level = PERI_VOLTAGE_LEVEL0;
		break;
	default:
		HISI_FB_ERR("wrong pri clk rate %llu\n", vote_cmd.dss_pri_clk_rate);
		break;
	}

	return level;
}

static void hisifb_get_other_fb_clk_vote(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t *other_fb_clk_rate)
{
	struct hisi_fb_data_type *targetfd1 = NULL;
	struct hisi_fb_data_type *targetfd2 = NULL;

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		targetfd1 = hisifd_list[EXTERNAL_PANEL_IDX];
		targetfd2 = hisifd_list[AUXILIARY_PANEL_IDX];
	} else if (hisifd->index == EXTERNAL_PANEL_IDX) {
		targetfd1 = hisifd_list[PRIMARY_PANEL_IDX];
		targetfd2 = hisifd_list[AUXILIARY_PANEL_IDX];
	} else {
		targetfd1 = hisifd_list[PRIMARY_PANEL_IDX];
		targetfd2 = hisifd_list[EXTERNAL_PANEL_IDX];
	}

	if (targetfd1 && targetfd2) {
		other_fb_clk_rate->dss_pri_clk_rate = max(targetfd1->dss_vote_cmd.dss_pri_clk_rate,
			targetfd2->dss_vote_cmd.dss_pri_clk_rate);
		other_fb_clk_rate->dss_mmbuf_rate = max(targetfd1->dss_vote_cmd.dss_mmbuf_rate,
			 targetfd2->dss_vote_cmd.dss_mmbuf_rate);
	} else if (targetfd1) {
		other_fb_clk_rate->dss_pri_clk_rate = targetfd1->dss_vote_cmd.dss_pri_clk_rate;
		other_fb_clk_rate->dss_mmbuf_rate = targetfd1->dss_vote_cmd.dss_mmbuf_rate;
	} else if (targetfd2) {
		other_fb_clk_rate->dss_pri_clk_rate = targetfd2->dss_vote_cmd.dss_pri_clk_rate;
		other_fb_clk_rate->dss_mmbuf_rate = targetfd2->dss_vote_cmd.dss_mmbuf_rate;
	} else {
		return;
	}
}

static int get_dss_clock_value(uint32_t voltage_level, dss_vote_cmd_t *dss_vote_cmd)
{
	if (dss_vote_cmd == NULL) {
		HISI_FB_ERR("dss_vote_cmd is NULL point!\n");
		return -EINVAL;
	}

	switch (voltage_level) {
	case PERI_VOLTAGE_LEVEL3:
		dss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
		dss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L4;
		break;
	case PERI_VOLTAGE_LEVEL2:
		dss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L3;
		dss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L3;
		break;
	case PERI_VOLTAGE_LEVEL1:
		dss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L2;
		dss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L2;
		break;
	case PERI_VOLTAGE_LEVEL0:
		dss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		dss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
		break;
	default:
		HISI_FB_ERR("wrong voltage value %d\n", voltage_level);
		return -EINVAL;
	}

	return 0;
}

#ifdef SUPPORT_DCA_WITHOUT_VOLT
bool is_clk_voltage_matching(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t target_vote)
{
	(void)hisifd;
	(void)target_vote;

	return true;
}
#else
/*lint -emacro(529,*) */
bool is_clk_voltage_matching(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t target_vote)
{
	int current_peri_voltage = 0;
	struct dss_vote_cmd supportable_cmd;
	struct hisi_fb_data_type *fb0 = hisifd_list[PRIMARY_PANEL_IDX];
	struct hisi_fb_data_type *fb1 = hisifd_list[EXTERNAL_PANEL_IDX];
	struct hisi_fb_data_type *fb2 = hisifd_list[AUXILIARY_PANEL_IDX];

	memset(&supportable_cmd, 0, sizeof(supportable_cmd));

	(void)dss_get_peri_volt(&current_peri_voltage);
	if (get_dss_clock_value(dpe_get_voltage_level(current_peri_voltage),
		&supportable_cmd)) {
		return false;
	}
	if ((target_vote.dss_pri_clk_rate > supportable_cmd.dss_pri_clk_rate) ||
		(target_vote.dss_mmbuf_rate > supportable_cmd.dss_mmbuf_rate)) {
		HISI_FB_ERR("fb%d, current voltage %d does not match clk rate %llu, %llu\n",
			hisifd->index, current_peri_voltage,
			target_vote.dss_pri_clk_rate,
			target_vote.dss_mmbuf_rate);

		if ((fb0 != NULL) && fb0->panel_power_on)
			HISI_FB_INFO("CLKDIV2 0x%x, PERI_CTRL4 & CTRL5 = 0x%x & 0x%x\n",
				inp32(fb0->media_crg_base + MEDIA_CLKDIV1 + 0x4),
				inp32(fb0->pmctrl_base + PMCTRL_PERI_CTRL4),
				inp32(fb0->pmctrl_base + PMCTRL_PERI_CTRL5));

		HISI_FB_INFO("volt: %d, %d, %d, edcClk: %llu, %llu, %llu\n",
			(fb0 != NULL) ? fb0->dss_vote_cmd.dss_voltage_level : 0,
			(fb1 != NULL) ? fb1->dss_vote_cmd.dss_voltage_level : 0,
			(fb2 != NULL) ? fb2->dss_vote_cmd.dss_voltage_level : 0,
			(fb0 != NULL) ? fb0->dss_vote_cmd.dss_pri_clk_rate : 0,
			(fb1 != NULL) ? fb1->dss_vote_cmd.dss_pri_clk_rate : 0,
			(fb2 != NULL) ? fb2->dss_vote_cmd.dss_pri_clk_rate : 0);

		return false;
	}

	return true;
}
/*lint +emacro(529,*) */
#endif

static int dss_core_clk_enable(struct hisi_fb_data_type *hisifd)
{
	int ret;

	if (hisifd->dss_pri_clk != NULL) {
		ret = clk_prepare_enable(hisifd->dss_pri_clk);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d dss_pri_clk clk_enable failed, error=%d!\n", hisifd->index, ret);
	}

	if (hisifd->dss_axi_clk != NULL) {
		ret = clk_prepare_enable(hisifd->dss_axi_clk);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d dss_axi_clk enable failed, error=%d!\n", hisifd->index, ret);
	}

	if (hisifd->dss_mmbuf_clk != NULL) {
		ret = clk_prepare_enable(hisifd->dss_mmbuf_clk);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d dss_mmbuf_clk enable failed, error=%d!\n", hisifd->index, ret);
	}

	return 0;
}

static void dss_core_clk_disable(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->dss_pri_clk != NULL)
		clk_disable_unprepare(hisifd->dss_pri_clk);

	if (hisifd->dss_axi_clk != NULL)
		clk_disable_unprepare(hisifd->dss_axi_clk);

	if (hisifd->dss_mmbuf_clk != NULL)
		clk_disable_unprepare(hisifd->dss_mmbuf_clk);
}

int hisifb_set_edc_mmbuf_clk(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t target_vote)
{
	int ret = 0;

	if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_CLK_OFF) {
		ret = dss_core_clk_enable(hisifd);
		hisi_check_and_return((ret < 0), -1, ERR,
			"dss_core_clk_enable failed, error=%d\n", ret);
	}

	ret = clk_set_rate(hisifd->dss_pri_clk, target_vote.dss_pri_clk_rate);
	hisi_check_and_return((ret < 0), -1, ERR, "set dss_pri_clk_rate %llu failed, error=%d\n",
		target_vote.dss_pri_clk_rate, ret);

	HISI_FB_DEBUG("Trying to set dss_mmbuf_clk from %llu to %llu\n",
		clk_get_rate(hisifd->dss_mmbuf_clk), target_vote.dss_mmbuf_rate);
	ret = clk_set_rate(hisifd->dss_mmbuf_clk, target_vote.dss_mmbuf_rate);
	hisi_check_and_return((ret < 0), -1, ERR,
		"set dss_mmbuf_rate %llu failed, error=%d\n",
		target_vote.dss_mmbuf_rate, ret);

	if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_CLK_OFF)
		dss_core_clk_disable(hisifd);

	return ret;
}

static int hisifb_edc_mmbuf_clk_vote(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t dss_vote_cmd)
{
	dss_vote_cmd_t other_fb_clk_rate;
	dss_vote_cmd_t target_vote;

	memset(&other_fb_clk_rate, 0, sizeof(other_fb_clk_rate));
	memset(&target_vote, 0, sizeof(target_vote));

	hisifb_get_other_fb_clk_vote(hisifd, &other_fb_clk_rate);
	if (dss_vote_cmd.dss_pri_clk_rate >= other_fb_clk_rate.dss_pri_clk_rate) {
		target_vote.dss_pri_clk_rate = dss_vote_cmd.dss_pri_clk_rate;
		target_vote.dss_mmbuf_rate = dss_vote_cmd.dss_mmbuf_rate;
	} else {
		target_vote.dss_pri_clk_rate = other_fb_clk_rate.dss_pri_clk_rate;
		target_vote.dss_mmbuf_rate = other_fb_clk_rate.dss_mmbuf_rate;
	}

	if (!is_clk_voltage_matching(hisifd, target_vote))
		return -1;

	if (hisifb_set_edc_mmbuf_clk(hisifd, target_vote))
		return -1;

	HISI_FB_DEBUG("fb%d, pri clk vote %llu, set %llu, mmbuf clk vote %llu, set %llu, "
		"get coreClk = %llu mmbufClk = %llu\n",
		hisifd->index, dss_vote_cmd.dss_pri_clk_rate, target_vote.dss_pri_clk_rate,
		dss_vote_cmd.dss_mmbuf_rate, target_vote.dss_mmbuf_rate,
		(uint64_t)clk_get_rate(hisifd->dss_pri_clk),
		(uint64_t)clk_get_rate(hisifd->dss_mmbuf_clk));

	return 0;
}


static int check_pri_clk_rate(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t vote_cmd)
{
	if ((vote_cmd.dss_pri_clk_rate != DEFAULT_DSS_CORE_CLK_RATE_L1)
		&& (vote_cmd.dss_pri_clk_rate != DEFAULT_DSS_CORE_CLK_RATE_L2)
		&& (vote_cmd.dss_pri_clk_rate != DEFAULT_DSS_CORE_CLK_RATE_L3)
		&& (vote_cmd.dss_pri_clk_rate != DEFAULT_DSS_CORE_CLK_RATE_L4)) {
		HISI_FB_ERR("fb%d no support dss_pri_clk_rate %llu\n",
			hisifd->index, vote_cmd.dss_pri_clk_rate);
		return -EINVAL;
	}
	return 0;
}

static int check_mmbuf_clk_rate(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t vote_cmd)
{
	if ((vote_cmd.dss_mmbuf_rate != DEFAULT_DSS_MMBUF_CLK_RATE_L1)
		&& (vote_cmd.dss_mmbuf_rate != DEFAULT_DSS_MMBUF_CLK_RATE_L2)
		&& (vote_cmd.dss_mmbuf_rate != DEFAULT_DSS_MMBUF_CLK_RATE_L3)
		&& (vote_cmd.dss_mmbuf_rate != DEFAULT_DSS_MMBUF_CLK_RATE_L4)) {
		HISI_FB_ERR("fb%d no support dss_mmbuf_rate %llu\n",
			hisifd->index, vote_cmd.dss_pri_clk_rate);
		return -EINVAL;
	}
	return 0;
}

int set_dss_vote_cmd(struct hisi_fb_data_type *hisifd,
	dss_vote_cmd_t vote_cmd)
{
	int ret = 0;
	struct hisi_fb_data_type *fb0 = hisifd_list[PRIMARY_PANEL_IDX];
	struct hisi_fb_data_type *fb1 = hisifd_list[EXTERNAL_PANEL_IDX];
	struct hisi_fb_data_type *fb2 = hisifd_list[AUXILIARY_PANEL_IDX];

	hisi_check_and_return((hisifd == NULL), -1, ERR, "hisifd is NULL!\n");

	if (hisifd->index == MEDIACOMMON_PANEL_IDX)
		/* clk_media_common's voltage following the frequency */
		return set_mdc_core_clk(hisifd, vote_cmd);

	if (check_pri_clk_rate(hisifd, vote_cmd) != 0)
		return -EINVAL;

	if (check_mmbuf_clk_rate(hisifd, vote_cmd) != 0)
		return -EINVAL;

	if ((hisifd->index != AUXILIARY_PANEL_IDX) &&
		(vote_cmd.dss_pri_clk_rate == hisifd->dss_vote_cmd.dss_pri_clk_rate) &&
		(vote_cmd.dss_mmbuf_rate == hisifd->dss_vote_cmd.dss_mmbuf_rate)) {
		return ret;
	}

	if (hisifd_list[EXTERNAL_PANEL_IDX] && !hisifd_list[EXTERNAL_PANEL_IDX]->dp.dptx_vr) {
		if ((hisifd->index == AUXILIARY_PANEL_IDX) && (!hisifd_list[PRIMARY_PANEL_IDX]->panel_power_on)) {
			HISI_FB_INFO("fb%d, primary_pane is power off\n", hisifd->index);
			return -EINVAL;
		}
	}

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		hisifd->clk_vote_level = get_clk_vote_level(vote_cmd);
		HISI_FB_DEBUG("fb%d, get clk_vote_level = %d\n",
			hisifd->index, hisifd->clk_vote_level);
		return ret;
	}


	ret = hisifb_edc_mmbuf_clk_vote(hisifd, vote_cmd);
	if (ret == 0) {
		hisifd->dss_vote_cmd.dss_pri_clk_rate = vote_cmd.dss_pri_clk_rate;
		hisifd->dss_vote_cmd.dss_axi_clk_rate = vote_cmd.dss_axi_clk_rate;
		hisifd->dss_vote_cmd.dss_mmbuf_rate = vote_cmd.dss_mmbuf_rate;
		HISI_FB_DEBUG("edc: %llu, %llu, %llu, mmbuf: %llu, %llu, %llu\n",
			(fb0 != NULL) ? fb0->dss_vote_cmd.dss_pri_clk_rate : 0,
			(fb1 != NULL) ? fb1->dss_vote_cmd.dss_pri_clk_rate : 0,
			(fb2 != NULL) ? fb2->dss_vote_cmd.dss_pri_clk_rate : 0,
			(fb0 != NULL) ? fb0->dss_vote_cmd.dss_mmbuf_rate : 0,
			(fb1 != NULL) ? fb1->dss_vote_cmd.dss_mmbuf_rate : 0,
			(fb2 != NULL) ? fb2->dss_vote_cmd.dss_mmbuf_rate : 0);
	}

	return ret;
}


static void hisi_recovery_external_clk_rate(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->index == EXTERNAL_PANEL_IDX) {
		hisifd->dss_vote_cmd.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		hisifd->dss_vote_cmd.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
		hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL0;
		hisifb_edc_mmbuf_clk_vote(hisifd, hisifd->dss_vote_cmd);
	}
}

int dpe_set_pixel_clk_rate_on_pll0(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	uint64_t clk_rate;
#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
	struct peri_volt_poll *pvp = NULL;
#endif

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL Pointer!\n");
		return -EINVAL;
	}

	if (is_dp_panel(hisifd)) {
		hisi_recovery_external_clk_rate(hisifd);

		clk_rate = DEFAULT_DSS_PXL1_CLK_RATE_POWER_OFF;
		ret = clk_set_rate(hisifd->dss_pxl1_clk, clk_rate);
		if (ret < 0)
			HISI_FB_ERR("fb%d dss_pxl1_clk clk_set_rate(%llu) failed, error=%d!\n",
				hisifd->index, clk_rate, ret);

		HISI_FB_INFO("dss_pxl1_clk:[%llu]->[%llu].\n",
			clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pxl1_clk));

#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
		pvp = peri_volt_poll_get(DEV_LDI1_VOLTAGE_ID, NULL);
		hisi_check_and_return((pvp == NULL), -EINVAL, ERR, "get pvp failed!\n");

		ret = peri_set_volt(pvp, PERI_VOLTAGE_LEVEL0_060V);
		hisi_check_and_return((ret != 0), -EINVAL, ERR, "set voltage_value=0 failed!\n");
#endif
	}

	return ret;
}

static void set_dss_perf_level_debug(struct hisi_fb_data_type *hisifd)
{
	struct dss_vote_cmd *pdss_vote_cmd = &(hisifd->dss_vote_cmd);

	if (g_dss_perf_debug <= 0)
		return;

	hisifd->core_clk_upt_support = 0;

	switch (g_dss_perf_debug - 1) {
	case PERI_VOLTAGE_LEVEL0:
		pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
		break;
	case PERI_VOLTAGE_LEVEL1:
		pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L2;
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L2;
		break;
	case PERI_VOLTAGE_LEVEL2:
		pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L3;
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L3;
		break;
	case PERI_VOLTAGE_LEVEL3:
		pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L4;
		break;
	default:
		break;
	}

	HISI_FB_INFO("set edc %llu, mmbuf %llu",
		pdss_vote_cmd->dss_pri_clk_rate, pdss_vote_cmd->dss_mmbuf_rate);
}

static uint32_t get_external_performance_level(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	uint64_t pxl_clk_rate;
	uint32_t perf_level = EXTERNAL_PERFORMANCE_LEVEL0;

	pinfo = &(hisifd->panel_info);
	pxl_clk_rate = (is_dp_panel(hisifd) && hisifd->dp_get_pxl_clk_rate) ?
		hisifd->dp_get_pxl_clk_rate(&(hisifd->dp)) : pinfo->pxl_clk_rate * 2;

	if (pxl_clk_rate > DEFAULT_DSS_PXL1_CLK_RATE_L3)
		perf_level = EXTERNAL_PERFORMANCE_LEVEL3;
	else if (pxl_clk_rate > DEFAULT_DSS_PXL1_CLK_RATE_L2)
		perf_level = EXTERNAL_PERFORMANCE_LEVEL2;
	else if (pxl_clk_rate > DEFAULT_DSS_PXL1_CLK_RATE_L1)
		perf_level = EXTERNAL_PERFORMANCE_LEVEL1;
	else
		perf_level = EXTERNAL_PERFORMANCE_LEVEL0;

	/* dp 4k set max performance level */
	if (is_dp_panel(hisifd) &&
		(pinfo->xres * pinfo->yres) >= (LCD_4K_XRES * LCD_4K_YRES))
		perf_level = EXTERNAL_PERFORMANCE_LEVEL3;

	return perf_level;
}

static void get_mmbuf_clk_rate(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct dss_vote_cmd *pdss_vote_cmd = NULL;
	uint32_t perf_level;

	pinfo = &(hisifd->panel_info);
	pdss_vote_cmd = &(hisifd->dss_vote_cmd);

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (pdss_vote_cmd->dss_mmbuf_rate < DEFAULT_DSS_MMBUF_CLK_RATE_L1)
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	} else if (hisifd->index == EXTERNAL_PANEL_IDX) {
		perf_level = get_external_performance_level(hisifd);
		if (perf_level == EXTERNAL_PERFORMANCE_LEVEL3)
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L4;
		else if (perf_level == EXTERNAL_PERFORMANCE_LEVEL2)
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L3;
		else if (perf_level == EXTERNAL_PERFORMANCE_LEVEL1)
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L2;
		else
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	} else {
		if (pdss_vote_cmd->dss_mmbuf_rate == 0)
			pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	}
	set_dss_perf_level_debug(hisifd);
}

static int get_lcd_frame_rate(struct hisi_panel_info *pinfo)
{
	uint32_t h_porch = pinfo->ldi.h_back_porch +
		pinfo->ldi.h_front_porch + pinfo->ldi.h_pulse_width;

	uint32_t v_porch = pinfo->ldi.v_back_porch +
		pinfo->ldi.v_front_porch + pinfo->ldi.v_pulse_width;

	return pinfo->pxl_clk_rate /
		(pinfo->xres + pinfo->pxl_clk_rate_div * h_porch) /
		(pinfo->yres + v_porch);
}

static uint32_t get_current_porch_ratio(struct hisi_fb_data_type *hisifd)
{
	uint32_t porch_ratio;
	uint32_t real_area;
	uint32_t total_area;
	struct mipi_panel_info *mipi = &(hisifd->panel_info.mipi);

	real_area = hisifd->panel_info.yres;
	total_area = hisifd->panel_info.yres + mipi->vsa + mipi->vbp + mipi->vfp;
	porch_ratio = (total_area - real_area) * 100 / total_area;
	return porch_ratio;
}

static uint64_t adjust_default_pri_clk_rate(struct hisi_fb_data_type *hisifd, uint64_t dss_pri_clk_rate)
{
	uint32_t porch_ratio;
	uint64_t single_layer_clk;

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return dss_pri_clk_rate;

	if (hisifd->panel_info.ifbc_type != IFBC_MODE_VESA3_75X_SINGLE)
		return dss_pri_clk_rate;

	porch_ratio = get_current_porch_ratio(hisifd);
	single_layer_clk = calc_core_rate(hisifd, porch_ratio, hisifd->panel_info.fps);

	if (single_layer_clk > DEFAULT_DSS_CORE_CLK_RATE_L3)
		single_layer_clk = DEFAULT_DSS_CORE_CLK_RATE_L4;
	else if (single_layer_clk > DEFAULT_DSS_CORE_CLK_RATE_L2)
		single_layer_clk = DEFAULT_DSS_CORE_CLK_RATE_L3;
	else if (single_layer_clk > DEFAULT_DSS_CORE_CLK_RATE_L1)
		single_layer_clk = DEFAULT_DSS_CORE_CLK_RATE_L2;
	else
		single_layer_clk = DEFAULT_DSS_CORE_CLK_RATE_L1;
	HISI_FB_INFO("adjust boot core rate to %llu, fps: %d, porch_ratio: %u, dss_pri_clk_rate: %llu\n",
		single_layer_clk, hisifd->panel_info.fps, porch_ratio, dss_pri_clk_rate);
	return max(single_layer_clk, dss_pri_clk_rate);
}

struct dss_vote_cmd *get_dss_vote_cmd(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct dss_vote_cmd *pdss_vote_cmd = NULL;
	int frame_rate;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null.\n");
		return NULL;
	}

	pinfo = &(hisifd->panel_info);
	pdss_vote_cmd = &(hisifd->dss_vote_cmd);
	frame_rate = get_lcd_frame_rate(pinfo);

	if (g_fpga_flag == 1) {
		if (pdss_vote_cmd->dss_pri_clk_rate == 0)
			pdss_vote_cmd->dss_pri_clk_rate = 40 * 1000000UL;

		return pdss_vote_cmd;
	}

	if (pdss_vote_cmd->dss_pri_clk_rate == 0) {
		if ((pinfo->xres * pinfo->yres) >= (RES_4K_PHONE)) {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
			hisifd->core_clk_upt_support = 0;
		} else if ((pinfo->xres * pinfo->yres) >= (RES_1440P)) {
			if (frame_rate >= 110) { // 110Hz is inherited param
				pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
				hisifd->core_clk_upt_support = 0;
			} else {
				pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
				hisifd->core_clk_upt_support = 1;
			}
		} else if ((pinfo->xres * pinfo->yres) >= (RES_1080P)) {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
			hisifd->core_clk_upt_support = 1;
		} else {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
			hisifd->core_clk_upt_support = 1;
		}
		pdss_vote_cmd->dss_pclk_pctrl_rate = DEFAULT_PCLK_PCTRL_RATE;
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
		pdss_vote_cmd->dss_pclk_dss_rate = DEFAULT_PCLK_DSS_RATE;
	}
	pdss_vote_cmd->dss_pri_clk_rate = adjust_default_pri_clk_rate(hisifd, pdss_vote_cmd->dss_pri_clk_rate);

	if (hisifd->index == EXTERNAL_PANEL_IDX)
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;

	set_dss_perf_level_debug(hisifd);

	return pdss_vote_cmd;
}

int hisifb_set_mmbuf_clk_rate(struct hisi_fb_data_type *hisifd)
{
	int ret;
	struct dss_vote_cmd *pdss_vote_cmd = NULL;
	struct hisi_panel_info *pinfo = NULL;
	struct dss_vote_cmd other_fb_clk_rate;
	struct dss_vote_cmd vote_cmd_tmp;

	memset(&vote_cmd_tmp, 0, sizeof(vote_cmd_tmp));
	memset(&other_fb_clk_rate, 0, sizeof(other_fb_clk_rate));

	get_mmbuf_clk_rate(hisifd);

	pinfo = &(hisifd->panel_info);

	if ((hisifd->index == AUXILIARY_PANEL_IDX) &&
		hisifd_list[PRIMARY_PANEL_IDX]->panel_power_on)
		return 0;

	down(&g_hisifb_dss_clk_vote_sem);
	pdss_vote_cmd = &(hisifd->dss_vote_cmd);
	vote_cmd_tmp.dss_mmbuf_rate = pdss_vote_cmd->dss_mmbuf_rate;

	hisifb_get_other_fb_clk_vote(hisifd, &other_fb_clk_rate);

	vote_cmd_tmp.dss_mmbuf_rate = max(vote_cmd_tmp.dss_mmbuf_rate,
		other_fb_clk_rate.dss_mmbuf_rate);

	if (!is_clk_voltage_matching(hisifd, vote_cmd_tmp)) {
		HISI_FB_ERR("clk voltage does not match\n");
		vote_cmd_tmp.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	}

	ret = clk_set_rate(hisifd->dss_mmbuf_clk, vote_cmd_tmp.dss_mmbuf_rate);
	if (ret < 0) {
		up(&g_hisifb_dss_clk_vote_sem);
		HISI_FB_ERR("fb%d dss_mmbuf clk_set_rate(%llu) failed, error=%d!\n",
			hisifd->index, vote_cmd_tmp.dss_mmbuf_rate, ret);
		return -EINVAL;
	}

	if ((hisifd->index == PRIMARY_PANEL_IDX) ||
		(hisifd->index == EXTERNAL_PANEL_IDX))
		HISI_FB_INFO("fb%d mmbuf clk rate[%llu], set[%llu],get[%llu].\n",
			hisifd->index, pdss_vote_cmd->dss_mmbuf_rate,
			vote_cmd_tmp.dss_mmbuf_rate,
			(uint64_t)clk_get_rate(hisifd->dss_mmbuf_clk));

	up(&g_hisifb_dss_clk_vote_sem);
	return 0;
}

static int dpe_set_pxl_clk_rate(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo;
	int ret = 0;

	pinfo = &(hisifd->panel_info);
	if (is_dp_panel(hisifd)) {
		if (hisifd->dp_pxl_ppll7_init != NULL)
			ret = hisifd->dp_pxl_ppll7_init(hisifd, pinfo->pxl_clk_rate);
		else
			ret = clk_set_rate(hisifd->dss_pxl1_clk, pinfo->pxl_clk_rate);

		if (ret < 0) {
			HISI_FB_ERR("fb%d dss_pxl1_clk clk_set_rate(%llu) failed, error=%d!\n",
				hisifd->index, pinfo->pxl_clk_rate, ret);

			if (g_fpga_flag == 0)
				return -EINVAL;
		}
		HISI_FB_INFO("dss_pxl1_clk:[%llu]->[%llu].\n",
			pinfo->pxl_clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pxl1_clk));
	}

	return ret;
}

int dpe_set_clk_rate(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_panel_info *pinfo = NULL;
	struct dss_vote_cmd *pdss_vote_cmd = NULL;
	dss_vote_cmd_t tmp;
	struct dss_vote_cmd other_fb_clk_rate;
	int ret;

	if (pdev == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = platform_get_drvdata(pdev);
	if (hisifd == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

	pinfo = &(hisifd->panel_info);

	down(&g_hisifb_dss_clk_vote_sem);
	pdss_vote_cmd = get_dss_vote_cmd(hisifd);
	if (pdss_vote_cmd == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		ret = -EINVAL;
		goto ERR_OUT;
	}
	memset(&tmp, 0, sizeof(tmp));
	memset(&other_fb_clk_rate, 0, sizeof(other_fb_clk_rate));

	tmp.dss_pri_clk_rate = pdss_vote_cmd->dss_pri_clk_rate;

	/* dss_pri_clk_rate */
	hisifb_get_other_fb_clk_vote(hisifd, &other_fb_clk_rate);

	tmp.dss_pri_clk_rate = max(tmp.dss_pri_clk_rate, other_fb_clk_rate.dss_pri_clk_rate);
	if (!is_clk_voltage_matching(hisifd, tmp)) {
		HISI_FB_ERR("clk voltage does not match\n");
		tmp.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
	}

	ret = clk_set_rate(hisifd->dss_pri_clk, tmp.dss_pri_clk_rate);
	if (ret < 0) {
		HISI_FB_ERR("fb%d dss_pri_clk clk_set_rate(%llu) failed, error=%d!\n",
			hisifd->index, tmp.dss_pri_clk_rate, ret);
		ret = -EINVAL;
		goto ERR_OUT;
	}

	ret = dpe_set_pxl_clk_rate(hisifd);
	if (ret < 0) {
		HISI_FB_ERR("fb%d set pxl clk rate failed, error=%d!\n", hisifd->index, ret);
		ret = -EINVAL;
		goto ERR_OUT;
	}

	if ((hisifd->index == PRIMARY_PANEL_IDX) ||
		(hisifd->index == EXTERNAL_PANEL_IDX))
		HISI_FB_INFO("fb%d, dss_pri_clk set %llu get %llu\n", hisifd->index,
			tmp.dss_pri_clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pri_clk));

ERR_OUT:

	up(&g_hisifb_dss_clk_vote_sem);
	return ret;
}

static void hisifb_set_default_pri_clk_rate(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct dss_vote_cmd *pdss_vote_cmd = NULL;
	int frame_rate;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null.\n");
		return;
	}

	pinfo = &(hisifd->panel_info);
	pdss_vote_cmd = &(hisifd->dss_vote_cmd);
	frame_rate = get_lcd_frame_rate(pinfo);

	if (g_fpga_flag == 1) {
		pdss_vote_cmd->dss_pri_clk_rate = 40 * 1000000UL;
	} else {
		if ((pinfo->xres * pinfo->yres) >= (RES_4K_PHONE)) {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
		} else if ((pinfo->xres * pinfo->yres) >= (RES_1440P)) {
			if (frame_rate >= 110) // 110Hz is inherited param
				pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
			else
				pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		} else if ((pinfo->xres * pinfo->yres) >= (RES_1080P)) {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		} else {
			pdss_vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		}
		pdss_vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	}
}

int dpe_set_common_clk_rate_on_pll0(struct hisi_fb_data_type *hisifd)
{
	int ret;
	uint64_t clk_rate;
#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
	struct peri_volt_poll *pvp = NULL;
#endif

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "hisifd is NULL Pointer!\n");

	if (g_fpga_flag == 1)
		return 0;

	down(&g_hisifb_dss_clk_vote_sem);
	clk_rate = DEFAULT_DSS_MMBUF_CLK_RATE_POWER_OFF;
	ret = clk_set_rate(hisifd->dss_mmbuf_clk, clk_rate);
	if (ret < 0) {
		HISI_FB_ERR("fb%d dss_mmbuf clk_set_rate(%llu) failed, error=%d!\n",
				hisifd->index, clk_rate, ret);
		ret = -EINVAL;
		goto ERR_OUT;
	}

	HISI_FB_INFO("dss_mmbuf_clk:[%llu]->[%llu]\n",
		clk_rate, (uint64_t)clk_get_rate(hisifd->dss_mmbuf_clk));

	clk_rate = DEFAULT_DSS_CORE_CLK_RATE_POWER_OFF;
	ret = clk_set_rate(hisifd->dss_pri_clk, clk_rate);
	if (ret < 0) {
		HISI_FB_ERR("fb%d dss_pri_clk clk_set_rate(%llu) failed, error=%d!\n",
			hisifd->index, clk_rate, ret);
		ret = -EINVAL;
		goto ERR_OUT;
	}

	HISI_FB_INFO("dss_pri_clk:[%llu]->[%llu]\n",
		clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pri_clk));
	hisifb_set_default_pri_clk_rate(hisifd_list[PRIMARY_PANEL_IDX]);
	hisifd_list[AUXILIARY_PANEL_IDX]->dss_vote_cmd.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
	hisifd_list[AUXILIARY_PANEL_IDX]->dss_vote_cmd.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	hisifd_list[AUXILIARY_PANEL_IDX]->clk_vote_level = PERI_VOLTAGE_LEVEL0;

#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
	pvp = peri_volt_poll_get(DEV_DSS_VOLTAGE_ID, NULL);
	if (pvp == NULL) {
		HISI_FB_ERR("get pvp failed!\n");
		ret = -EINVAL;
		goto ERR_OUT;
	}

	ret = peri_set_volt(pvp, PERI_VOLTAGE_LEVEL0_060V);
	if (ret != 0) {
		HISI_FB_ERR("set voltage_value=0 failed!\n");
		ret = -EINVAL;
		goto ERR_OUT;
	}

	hisifd_list[PRIMARY_PANEL_IDX]->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL0;
	hisifd_list[AUXILIARY_PANEL_IDX]->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL0;

	HISI_FB_INFO("set dss_voltage_level=0!\n");
#endif

ERR_OUT:

	up(&g_hisifb_dss_clk_vote_sem);
	return ret;
}

int hisifb_restore_dss_voltage_clk_vote(
	struct hisi_fb_data_type *hisifd, dss_vote_cmd_t vote_cmd)
{
	int ret = 0;
#ifndef SUPPORT_DCA_WITHOUT_VOLT
	int volt_to_set;
	int timeout = 10;
	int current_peri_voltage = 0;
#endif

	if (hisifd == NULL || hisifd->index != PRIMARY_PANEL_IDX)
		return 0;

	down(&g_hisifb_dss_clk_vote_sem);
	if ((hisifd->dss_vote_cmd.dss_voltage_level != 0) ||
		(hisifd->dss_vote_cmd.dss_pri_clk_rate != DEFAULT_DSS_CORE_CLK_RATE_L1))
		HISI_FB_ERR("break restore, expect: %d, %llu, now: %d, %llu\n",
			vote_cmd.dss_voltage_level, vote_cmd.dss_pri_clk_rate,
			hisifd->dss_vote_cmd.dss_voltage_level, hisifd->dss_vote_cmd.dss_pri_clk_rate);

#ifndef SUPPORT_DCA_WITHOUT_VOLT
	if (vote_cmd.dss_voltage_level == 0)
		goto vote_out;

	volt_to_set = dpe_get_voltage_value(vote_cmd.dss_voltage_level);
	ret = hisifb_set_dss_vote_voltage(hisifd, vote_cmd.dss_voltage_level, &current_peri_voltage);
	if (ret < 0) {
		HISI_FB_ERR("set volt %d fail\n", vote_cmd.dss_voltage_level);
		goto vote_out;
	}

	while ((current_peri_voltage < volt_to_set) && (timeout > 0)) {
		msleep(1);
		(void)dss_get_peri_volt(&current_peri_voltage);
		timeout--;
	}

	if (timeout > 0) {
		ret = set_dss_vote_cmd(hisifd, vote_cmd);
		if (ret < 0) {
			HISI_FB_ERR("set clk %llu, %llu fail\n", vote_cmd.dss_pri_clk_rate, vote_cmd.dss_mmbuf_rate);
			goto vote_out;
		}
	} else {
		HISI_FB_ERR("timeout, current_peri_voltage %d, less than expect %d\n", current_peri_voltage, volt_to_set);
		goto vote_out;
	}
#else
	ret = set_dss_vote_cmd(hisifd, vote_cmd);
	if (ret < 0) {
		HISI_FB_ERR("set clk %llu, %llu fail\n", vote_cmd.dss_pri_clk_rate, vote_cmd.dss_mmbuf_rate);
		goto vote_out;
	}
#endif
	HISI_FB_INFO("restore vote: %d, %llu, %llu succ\n",hisifd->dss_vote_cmd.dss_voltage_level,
		hisifd->dss_vote_cmd.dss_pri_clk_rate, hisifd->dss_vote_cmd.dss_mmbuf_rate);

vote_out:
	up(&g_hisifb_dss_clk_vote_sem);
	return ret;
}

static void set_external_performance(struct hisi_fb_data_type *hisifd,
	uint64_t pixel_clock, uint32_t *voltage, dss_vote_cmd_t *vote_cmd)
{
	uint32_t perf_level;

	perf_level = get_external_performance_level(hisifd);

	if (perf_level == EXTERNAL_PERFORMANCE_LEVEL3) {
		hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL3;
		*voltage = PERI_VOLTAGE_LEVEL3_080V;
		vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L4;
		vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L4;
		vote_cmd->dss_voltage_level = PERI_VOLTAGE_LEVEL3;
	} else if (perf_level == EXTERNAL_PERFORMANCE_LEVEL2) {
		hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL2;
		*voltage = PERI_VOLTAGE_LEVEL2_070V;
		vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L3;
		vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L3;
		vote_cmd->dss_voltage_level = PERI_VOLTAGE_LEVEL2;
	} else if (perf_level == EXTERNAL_PERFORMANCE_LEVEL1) {
		hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL1;
		*voltage = PERI_VOLTAGE_LEVEL1_065V;
		vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L2;
		vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L2;
		vote_cmd->dss_voltage_level = PERI_VOLTAGE_LEVEL1;
	} else {
		hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL0;
		*voltage = PERI_VOLTAGE_LEVEL0_060V;
		vote_cmd->dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		vote_cmd->dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
		vote_cmd->dss_voltage_level = PERI_VOLTAGE_LEVEL0;
	}
}

/*lint -emacro(1564,*) */
int hisifb_set_dss_external_vote_pre(struct hisi_fb_data_type *hisifd,
	uint64_t pixel_clock)
{
	int ret;
	uint32_t voltage = 0;
	dss_vote_cmd_t vote_cmd;
#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
	int voltage_timeout = 15;
	struct peri_volt_poll *pvp = NULL;
#endif

	hisi_check_and_return((hisifd == NULL), -1, ERR, "NULL Pointer!\n");

	hisi_check_and_return((pixel_clock > (ULLONG_MAX / 2)), -1, ERR,
		"pixel_clock will overflow!\n");

	if (hisifd->index != EXTERNAL_PANEL_IDX)
		return 0;

	memset(&vote_cmd, 0, sizeof(vote_cmd));

	set_external_performance(hisifd, pixel_clock, &voltage, &vote_cmd);

#if defined(CONFIG_HISI_PERIDVFS) && !defined(SUPPORT_DCA_WITHOUT_VOLT)
	pvp = peri_volt_poll_get(DEV_LDI1_VOLTAGE_ID, NULL);
	hisi_check_and_return((pvp == NULL), -EINVAL, ERR, "[DP] get pvp failed!\n");

	ret = peri_set_volt(pvp, voltage);
	hisi_check_and_return((ret != 0), -EINVAL, ERR, "[DP] set voltage_value failed!\n");

	while (voltage_timeout > 0) {
		mdelay(10);
		if (voltage <= peri_get_volt(pvp))
			break;
		voltage_timeout--;
	}

	hisi_check_and_return((voltage > peri_get_volt(pvp)), -EINVAL, ERR,
		"[DP] set voltage_value time out!! request volt %d, cur volt %d\n",
		voltage, peri_get_volt(pvp));
#endif
	/* pixel clock/2 need more than DSS core clk. In double pixel mode,
	 * pixel clock is half of previous.
	 * So it only need pixel clock more than DSS core clk.
	 */
	if ((voltage > PERI_VOLTAGE_LEVEL0_060V) &&
		((vote_cmd.dss_pri_clk_rate) > DEFAULT_DSS_CORE_CLK_RATE_L1)) {
		down(&hisifd_list[PRIMARY_PANEL_IDX]->dp_vote_sem);
		ret = set_dss_vote_cmd(hisifd, vote_cmd);
		if (ret < 0) {
			HISI_FB_ERR("[DP] DSS core clk set failed!!\n");
			up(&hisifd_list[PRIMARY_PANEL_IDX]->dp_vote_sem);
			return ret;
		}
		up(&hisifd_list[PRIMARY_PANEL_IDX]->dp_vote_sem);
	} else {
		hisifd->dss_vote_cmd.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		hisifd->dss_vote_cmd.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	}
	return 0;
}
/*lint +emacro(1564,*) */

int hisifb_offline_vote_ctrl(struct hisi_fb_data_type *hisifd,
	bool offline_start)
{
	int ret = 0;
	struct dss_vote_cmd other_fb_clk_rate, copybit_clk_rate;

	if (hisifd->index != AUXILIARY_PANEL_IDX)
		return -1;

	memset(&other_fb_clk_rate, 0, sizeof(other_fb_clk_rate));
	memset(&copybit_clk_rate, 0, sizeof(copybit_clk_rate));

	down(&g_hisifb_dss_clk_vote_sem);
	hisifb_get_other_fb_clk_vote(hisifd, &other_fb_clk_rate);

	if (offline_start) {
		ret = get_dss_clock_value(hisifd->clk_vote_level, &copybit_clk_rate);
		if (ret < 0) {
			HISI_FB_ERR("get_dss_clock_value fail, level = %d\n", hisifd->clk_vote_level);
			goto offline_vote_out;
		}

		if (copybit_clk_rate.dss_pri_clk_rate > other_fb_clk_rate.dss_pri_clk_rate) {
			if (!is_clk_voltage_matching(hisifd, copybit_clk_rate)) {
				ret = -1;
				HISI_FB_ERR("offline play vote mismatch\n");
				goto offline_vote_out;
			}

			ret = hisifb_set_edc_mmbuf_clk(hisifd, copybit_clk_rate);
			if (ret < 0) {
				HISI_FB_ERR("fb%d offline start set dss_pri_clk_rate %llu fail\n",
					hisifd->index, copybit_clk_rate.dss_pri_clk_rate);
				ret = -1;
				goto offline_vote_out;
			}
		}

		hisifd->dss_vote_cmd.dss_pri_clk_rate = copybit_clk_rate.dss_pri_clk_rate;
		hisifd->dss_vote_cmd.dss_mmbuf_rate = copybit_clk_rate.dss_mmbuf_rate;
		HISI_FB_DEBUG("fb%d offline start set dss_pri_clk_rate %llu\n",
			hisifd->index, hisifd->dss_vote_cmd.dss_pri_clk_rate);
	} else {
		/* offline play end, recover fb2 voltage&CoreClk to max of other fb */
		if (hisifd->dss_vote_cmd.dss_pri_clk_rate > other_fb_clk_rate.dss_pri_clk_rate) {
			ret = hisifb_set_edc_mmbuf_clk(hisifd, other_fb_clk_rate);
			if (ret < 0)
				HISI_FB_ERR("offline play end, set pri clk %llu fail\n",
					other_fb_clk_rate.dss_pri_clk_rate);
			else
				HISI_FB_DEBUG("fb%d offline end set dss_pri_clk_rate %llu\n",
					hisifd->index, other_fb_clk_rate.dss_pri_clk_rate);
		}

		hisifd->dss_vote_cmd.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L1;
		hisifd->dss_vote_cmd.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L1;
	}

offline_vote_out:
	up(&g_hisifb_dss_clk_vote_sem);
	return ret;
}
