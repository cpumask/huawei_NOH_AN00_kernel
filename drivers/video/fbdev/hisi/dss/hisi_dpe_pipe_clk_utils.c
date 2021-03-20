/* Copyright (c) 2018-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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

#include "hisi_dpe_pipe_clk_utils.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static int hisifb_change_pipe_clk_rate(struct hisi_fb_data_type *hisifd, uint64_t pipe_clk_rate)
{
	int ret = -1;

	if (hisifd->dss_pxl0_clk != NULL) {
		ret = clk_set_rate(hisifd->dss_pxl0_clk, pipe_clk_rate); /* ppll0 */
		if (ret < 0) {
			HISI_FB_ERR("set pipe_clk_rate[%llu] fail, reset to [%llu], ret[%d].\n",
				hisifd->pipe_clk_ctrl.pipe_clk_rate, hisifd->panel_info.pxl_clk_rate, ret);

			ret = clk_set_rate(hisifd->dss_pxl0_clk, hisifd->panel_info.pxl_clk_rate);
			if ((ret < 0) && (g_fpga_flag == 0)) {
				HISI_FB_ERR("fb%d dss_pxl0_clk clk_set_rate(%llu) failed, error=%d!\n",
				hisifd->index, hisifd->panel_info.pxl_clk_rate, ret);
				return -EINVAL;
			}
		}

		HISI_FB_INFO("dss_pxl0_clk:set[%llu], get[%llu].\n",
			pipe_clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pxl0_clk));
	}

	return ret;
}

static int hisifb_pipe_clk_updt_config(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	uint32_t *hporch = NULL;
	char __iomem *ldi_base = NULL;
	uint32_t pxl0_divxcfg;
	uint32_t mipi_idx;
	int ret = 0;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "hisifd is nullptr!\n");

	pinfo = &(hisifd->panel_info);
	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);
	hisi_check_and_return((pinfo == NULL), -EINVAL, ERR, "pinfo is nullptr!\n");

	hporch = pipe_clk_ctrl->pipe_clk_updt_hporch;

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return ret;

	ldi_base = hisifd->dss_base + DSS_LDI0_OFFSET;

	ret = hisifb_change_pipe_clk_rate(hisifd, pipe_clk_ctrl->pipe_clk_rate);
	hisi_check_and_return((ret < 0), ret, ERR, "fb%d, set clk_ldi0 to [%llu] failed, error=%d!\n",
		hisifd->index, pipe_clk_ctrl->pipe_clk_rate, ret);

	mipi_idx = is_dual_mipi_panel(hisifd) ? 1 : 0;
	pxl0_divxcfg = g_mipi_ifbc_division[mipi_idx][pinfo->ifbc_type].pxl0_divxcfg;
	pxl0_divxcfg = (pxl0_divxcfg + 1) * pipe_clk_ctrl->pipe_clk_rate_div;

	set_reg(ldi_base + LDI_PXL0_DIVXCFG, (pxl0_divxcfg - 1), 3, 0);
	if (pxl0_divxcfg > 1)
		set_reg(ldi_base + LDI_PXL0_DSI_GT_EN, 3, 2, 0);
	else
		set_reg(ldi_base + LDI_PXL0_DSI_GT_EN, 1, 2, 0);

	HISI_FB_INFO("step1. set pxl0_divxcfg[%d]\n", (pxl0_divxcfg - 1));

	if (is_mipi_video_panel(hisifd)) {
		/* pipe_clk_updt_hporch valve */
		outp32(ldi_base + LDI_DPI0_HRZ_CTRL0, hporch[1] | ((hporch[0] + DSS_WIDTH(hporch[2])) << 16));
	} else {
		outp32(ldi_base + LDI_DPI0_HRZ_CTRL0, hporch[1] | (hporch[0] << 16));
		outp32(ldi_base + LDI_DPI0_HRZ_CTRL1, DSS_WIDTH(hporch[2]));
	}

	if (is_dual_mipi_panel(hisifd)) {
		if (is_mipi_video_panel(hisifd)) {
			outp32(ldi_base + LDI_DPI1_HRZ_CTRL0, (hporch[0] + DSS_WIDTH(hporch[2])) << 16);
		} else {
			outp32(ldi_base + LDI_DPI1_HRZ_CTRL0, hporch[0] << 16);
			outp32(ldi_base + LDI_DPI1_HRZ_CTRL1, DSS_WIDTH(hporch[2]));
		}
	}

	HISI_FB_INFO("step2. updt hporch: hbp[%d], hfp[%d], hpw[%d].\n", hporch[0], hporch[1], hporch[2]);

	return ret;
}

int hisifb_pipe_clk_updt_handler(struct hisi_fb_data_type *primary_hisifd, bool primary_panel_pwr_on)
{
	struct hisi_panel_info *pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	int ret = 0;

	if (primary_hisifd == NULL) {
		HISI_FB_ERR("primary_hisifd is null.\n");
		return -EINVAL;
	}

	if (primary_hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_INFO("fb%d not support.\n", primary_hisifd->index);
		return ret;
	}

	pinfo = &(primary_hisifd->panel_info);

	pipe_clk_ctrl = &(primary_hisifd->pipe_clk_ctrl);

	HISI_FB_DEBUG("+, primary_panel_pwr_on[%d].\n", primary_panel_pwr_on);

	if (primary_panel_pwr_on) {
		if (hisifd_list[EXTERNAL_PANEL_IDX] && hisifd_list[EXTERNAL_PANEL_IDX]->panel_power_on &&
			(pipe_clk_ctrl->pipe_clk_rate > pinfo->pxl_clk_rate))
			ret = hisifb_pipe_clk_updt_config(primary_hisifd);
	} else {
		/* only for pipe_clk_updt in vactive_end */
		if (pipe_clk_ctrl->pipe_clk_rate >= pinfo->pxl_clk_rate)
			ret = hisifb_pipe_clk_updt_config(primary_hisifd);
	}

	HISI_FB_DEBUG("-.\n");
	return ret;
}

static void calc_hporch_paras(struct hisi_panel_info *pinfo, struct hisifb_pipe_clk *pipe_clk_ctrl,
		struct hporch_value hp_value, struct hporch_new_value *hp_new_val)
{
	uint64_t pxl_clk_rate_new;
	uint32_t pxl_clk_ratio = 1;
	uint32_t precision = 100;  /* with 2 decimal places */
	uint32_t millisecond = 1000; /* us */
	uint32_t h_size;
	uint32_t v_size;
	uint32_t fps = 0;
	uint32_t t_frame = 0;
	uint32_t t_hline = 0;
	uint32_t t_hbphpw = 0;

	/* pipe_clk_rate_div calc from get_para_for_pipe_clk_updt(), not 0 */
	pxl_clk_rate_new = pipe_clk_ctrl->pipe_clk_rate / pipe_clk_ctrl->pipe_clk_rate_div;

	if (pxl_clk_rate_new > pinfo->pxl_clk_rate) {
		if (pinfo->pxl_clk_rate_div > 1) {
			hp_value.hbp *= pinfo->pxl_clk_rate_div;
			hp_value.hfp *= pinfo->pxl_clk_rate_div;
			hp_value.hpw *= pinfo->pxl_clk_rate_div;
		}

		/* hporch paras calc */
		/* with 2 decimal places */
		pxl_clk_ratio = precision * (pxl_clk_rate_new / 100) / (pinfo->pxl_clk_rate / 100);
		hp_new_val->hbp_new = hp_value.hbp * pxl_clk_ratio;  /* with 2 decimal places */
		hp_new_val->hpw_new = hp_value.hpw * pxl_clk_ratio;  /* with 2 decimal places */

		/* h_size, v_size from pinfo, lcd init will set pinfo value, not 0 */
		h_size = pinfo->xres + hp_value.hbp + hp_value.hfp + hp_value.hpw;
		v_size = pinfo->yres + pinfo->ldi.v_back_porch + pinfo->ldi.v_front_porch + pinfo->ldi.v_pulse_width;
		fps = pinfo->pxl_clk_rate / h_size * precision / v_size;  /* us with 2 decimal places */
		t_frame = millisecond * millisecond * precision / fps; /* us */
		t_hline = precision * t_frame / v_size;  /* us with 2 decimal places */
		t_hbphpw = t_hline * (hp_value.hbp + hp_value.hpw) / h_size;  /* us with 2 decimal places */
		/* with 2 decimal places */
		hp_new_val->hfp_new = (hp_new_val->hbp_new + hp_new_val->hpw_new) * t_hline / t_hbphpw -
			(pinfo->xres * precision + hp_new_val->hbp_new + hp_new_val->hpw_new);
		hp_new_val->hbp_new = hp_new_val->hbp_new / precision + (hp_new_val->hbp_new % precision > 0 ? 1 : 0);
		hp_new_val->hfp_new = hp_new_val->hfp_new / precision + (hp_new_val->hfp_new % precision > 0 ? 1 : 0);
		hp_new_val->hpw_new = hp_new_val->hpw_new / precision + (hp_new_val->hpw_new % precision > 0 ? 1 : 0);

		if (pinfo->pxl_clk_rate_div > 1) {
			hp_new_val->hbp_new /= pinfo->pxl_clk_rate_div;
			hp_new_val->hfp_new /= pinfo->pxl_clk_rate_div;
			hp_new_val->hpw_new /= pinfo->pxl_clk_rate_div;
		}
	} else {
		hp_new_val->hbp_new = hp_value.hbp;
		hp_new_val->hfp_new = hp_value.hfp;
		hp_new_val->hpw_new = hp_value.hpw;
	}

	HISI_FB_DEBUG("hbp[%d], hfp[%d], hpw[%d], pxl_clk_ratio[%d],\n"
		"fps[%d], t_frame[%d], t_hline[%d], t_hbphpw[%d]\n"
		"hbp_new[%d], hfp_new[%d], hpw_new[%d].\n",
		hp_value.hbp, hp_value.hfp, hp_value.hpw, pxl_clk_ratio, fps,
		t_frame, t_hline, t_hbphpw, hp_new_val->hbp_new, hp_new_val->hfp_new, hp_new_val->hpw_new);
}

int hisifb_get_ldi_hporch_updt_para(struct hisi_fb_data_type *hisifd, bool fps_updt_use)
{
	struct hisi_panel_info *pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	struct hporch_value hp_value;
	struct hporch_new_value hp_new_val;

	pinfo = &(hisifd->panel_info);

	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);

	HISI_FB_DEBUG("+ ,fps_updt_use[%d].\n", fps_updt_use);

	if (fps_updt_use) {
		hp_value.hbp = pinfo->ldi_updt.h_back_porch;
		hp_value.hfp = pinfo->ldi_updt.h_front_porch;
		hp_value.hpw = pinfo->ldi_updt.h_pulse_width;
	} else {
		hp_value.hbp = pinfo->ldi.h_back_porch;
		hp_value.hfp = pinfo->ldi.h_front_porch;
		hp_value.hpw = pinfo->ldi.h_pulse_width;
	}

	calc_hporch_paras(pinfo, pipe_clk_ctrl, hp_value, &hp_new_val);

	if (fps_updt_use) {
		pipe_clk_ctrl->fps_updt_hporch[0] = hp_new_val.hbp_new;
		pipe_clk_ctrl->fps_updt_hporch[1] = hp_new_val.hfp_new;
		pipe_clk_ctrl->fps_updt_hporch[2] = hp_new_val.hpw_new;
	} else {
		pipe_clk_ctrl->pipe_clk_updt_hporch[0] = hp_new_val.hbp_new;
		pipe_clk_ctrl->pipe_clk_updt_hporch[1] = hp_new_val.hfp_new;
		pipe_clk_ctrl->pipe_clk_updt_hporch[2] = hp_new_val.hpw_new;
	}

	return 0;
}

static uint64_t calc_pipe_clk_rate_by_ppll(uint64_t ppll_clk_rate, uint64_t dp_pxl_clk_rate)
{
	uint64_t pipe_clk_rate_ppll = 0;
	uint32_t div = 0;

	if (ppll_clk_rate < dp_pxl_clk_rate) {
		pipe_clk_rate_ppll = 0;
	} else if (ppll_clk_rate == dp_pxl_clk_rate) {
		pipe_clk_rate_ppll = ppll_clk_rate;
	} else {
		for (div = 1; div < 20; div++) {  /* clk division from 1 to 19 */
			if ((ppll_clk_rate / div) < dp_pxl_clk_rate)
				break;
		}
		div = div - 1;
		if (div > 0)
			pipe_clk_rate_ppll = (ppll_clk_rate % div) ? (ppll_clk_rate / div + 1) : (ppll_clk_rate / div);
		else
			HISI_FB_ERR("div = %u.\n", div);
	}

	HISI_FB_DEBUG("ppll_clk_rate[%llu], div[%u], pipe_clk_rate_ppll[%llu].\n",
		ppll_clk_rate, div, pipe_clk_rate_ppll);
	return pipe_clk_rate_ppll;
}

static int get_pxl_clk_div_by_ppll(uint64_t pxl_clk_rate, uint64_t ppll_clk_rate, int *out_div)
{
	uint32_t div = 1;
	int ret = 0;

	if (ppll_clk_rate != CRGPERI_PLL0_CLK_RATE
		&& ppll_clk_rate != CRGPERI_PLL2_CLK_RATE
		&& ppll_clk_rate != CRGPERI_PLL3_CLK_RATE) {
		HISI_FB_ERR("wrong ppll_clk_rate[%llu], which must be configed in panel init.\n", ppll_clk_rate);
		return -1;
	}

	if (ppll_clk_rate < pxl_clk_rate) {
		ret = -1;
	} else if (ppll_clk_rate == pxl_clk_rate) {
		div = 1;
	} else {
		for (div = 1; div < 20; div++) {  /* clk division from 1 to 19 */
			if ((ppll_clk_rate / div) <= pxl_clk_rate)
				break;
		}
	}

	if (ret)
		HISI_FB_ERR("ret=%d, pxl_clk_rate[%llu], div[%d].\n", ret, pxl_clk_rate, div);

	*out_div = div;

	return ret;
}

static int get_preset_para_for_pipe_clk_updt(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;

	pinfo = &(hisifd->panel_info);
	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);

	pipe_clk_ctrl->pipe_clk_rate = pinfo->ldi.pipe_clk_rate_pre_set * DEFAULT_PIPE_CLK_RATE;
	pipe_clk_ctrl->pipe_clk_rate_div = pinfo->ldi.div_pre_set;
	if (pinfo->ldi.hporch_pre_set[0] != 0) {
		pipe_clk_ctrl->pipe_clk_updt_hporch[0] = pinfo->ldi.hporch_pre_set[0];
		pipe_clk_ctrl->pipe_clk_updt_hporch[1] = pinfo->ldi.hporch_pre_set[1];
		pipe_clk_ctrl->pipe_clk_updt_hporch[2] = pinfo->ldi.hporch_pre_set[2];
	} else {
		pipe_clk_ctrl->pipe_clk_updt_hporch[0] = pinfo->ldi.h_back_porch;
		pipe_clk_ctrl->pipe_clk_updt_hporch[1] = pinfo->ldi.h_front_porch;
		pipe_clk_ctrl->pipe_clk_updt_hporch[2] = pinfo->ldi.h_pulse_width;
	}

	HISI_FB_INFO("self-adaption not support, use pre-set value\n");

	return 0;
}

static int get_para_for_pipe_clk_updt(struct hisi_fb_data_type *hisifd, uint64_t dp_pxl_clk_rate)
{
	struct hisi_panel_info *pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	uint64_t pipe_clk_rate;
	uint64_t pipe_clk_rate_ppll[3];  /* PLL0/PLL1/PLL2 clk rate */
	uint32_t pipe_clk_rate_div = 1;
	int count;
	int ret;
	uint64_t pxl_clk_rate_max_080v = 645000000UL;  /* 645MHz */

	pinfo = &(hisifd->panel_info);
	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);

	if (pinfo->ldi.pipe_clk_rate_pre_set)
		return get_preset_para_for_pipe_clk_updt(hisifd);

	pipe_clk_rate_ppll[0] = calc_pipe_clk_rate_by_ppll(CRGPERI_PLL0_CLK_RATE, dp_pxl_clk_rate);
	pipe_clk_rate_ppll[1] = calc_pipe_clk_rate_by_ppll(CRGPERI_PLL2_CLK_RATE, dp_pxl_clk_rate);
	pipe_clk_rate_ppll[2] = calc_pipe_clk_rate_by_ppll(CRGPERI_PLL3_CLK_RATE, dp_pxl_clk_rate);

	pipe_clk_rate = CRGPERI_PLL0_CLK_RATE;
	for (count = 0; count < 3; count++) {  /* PLL0/PLL1/PLL2 clk rate */
		if ((pipe_clk_rate_ppll[count] > 0) && (pipe_clk_rate > pipe_clk_rate_ppll[count]))
			pipe_clk_rate = pipe_clk_rate_ppll[count];
	}

	/* clk division from 1 to 19 */
	for (pipe_clk_rate_div = 1; pipe_clk_rate_div < 20; pipe_clk_rate_div++) {
		if ((pipe_clk_rate / pipe_clk_rate_div) < pinfo->pxl_clk_rate)
			break;
	}
	pipe_clk_rate_div--;

	if (pipe_clk_rate_div < 1) {
		ret = -2;
	} else if (((pipe_clk_rate / pipe_clk_rate_div) < pinfo->pxl_clk_rate) ||
		(pipe_clk_rate > pxl_clk_rate_max_080v)) {
		ret = -3;
	} else {
		hisifd->pipe_clk_ctrl.pipe_clk_rate = pipe_clk_rate;
		hisifd->pipe_clk_ctrl.pipe_clk_rate_div = pipe_clk_rate_div;
		ret = hisifb_get_ldi_hporch_updt_para(hisifd, false);
	}

	HISI_FB_DEBUG("ret[%d],pipe_clk_rate[%llu]->[%llu], pipe_clk_rate_div[%d]->[%d].\n",
		ret, pipe_clk_rate, hisifd->pipe_clk_ctrl.pipe_clk_rate, pipe_clk_rate_div,
		hisifd->pipe_clk_ctrl.pipe_clk_rate_div);

	return ret;
}

static int wait_pipe_clk_para_updt_end(void)
{
	struct hisi_fb_data_type *primary_hisifd = NULL;
	struct hisi_panel_info *primary_pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	uint32_t wait_time = 0;
	int ret = 0;

	primary_hisifd = hisifd_list[PRIMARY_PANEL_IDX];
	if (primary_hisifd == NULL) {
		HISI_FB_ERR("hisifd_primary is NULL .\n");
		return -1;
	}
	primary_pinfo = &(primary_hisifd->panel_info);

	pipe_clk_ctrl = &(primary_hisifd->pipe_clk_ctrl);

	while (pipe_clk_ctrl->pipe_clk_updt_state != PARA_UPDT_END) {
		if (!primary_hisifd->panel_power_on) {
			HISI_FB_INFO("primary_panel_power off, break.\n");
			break;
		}
		if ((wait_time++) > 200) {  /* wait time 1s */
			HISI_FB_ERR("wait PARA_UPDT_END time > 1 s.\n");
			ret = -1;
			break;
		}
		msleep(5);  /* sleep 5 ms */
	}

	if ((!primary_hisifd->panel_power_on) && (pipe_clk_ctrl->pipe_clk_updt_times == 0)) {
		HISI_FB_INFO("primary_panel_power_on[%d], only change pipeclkrate\n", primary_hisifd->panel_power_on);
		ret = hisifb_change_pipe_clk_rate(primary_hisifd, pipe_clk_ctrl->pipe_clk_rate);
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
		if (ret < 0) {
			HISI_FB_ERR("set pipe_clk rate fail. ret=%d.\n", ret);
			return ret;
		}
	} else if (pipe_clk_ctrl->pipe_clk_updt_times >= 5) {
		ret = -1;
	}

	return ret;
}

int hisifb_pipe_clk_input_para_check(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_data_type *primary_hisifd = NULL;
	struct hisi_panel_info *primary_pinfo = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null\n");
		return -1;
	}

	primary_hisifd = hisifd_list[PRIMARY_PANEL_IDX];
	if (primary_hisifd == NULL) {
		HISI_FB_ERR("hisifd_primary is NULL .\n");
		return -1;
	}
	primary_pinfo = &(primary_hisifd->panel_info);

	return 0;
}

static int hisifb_pipe_clk_pre_process(struct hisi_fb_data_type *primary_hisifd)
{
	struct hisi_panel_info *primary_pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	int ret = 0;

	primary_pinfo = &(primary_hisifd->panel_info);
	pipe_clk_ctrl = &(primary_hisifd->pipe_clk_ctrl);

	if (primary_hisifd->panel_power_on) {
		if (pipe_clk_ctrl->pipe_clk_rate > primary_pinfo->pxl_clk_rate) {
			pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_NEED;
			hisi_fb_frame_refresh(primary_hisifd, "pipeclk");
		}
	} else {
		if (pipe_clk_ctrl->pipe_clk_rate > primary_pinfo->pxl_clk_rate) {
			ret = hisifb_change_pipe_clk_rate(primary_hisifd, pipe_clk_ctrl->pipe_clk_rate);
			if (ret < 0) {
				HISI_FB_ERR("set pipe_clk rate fail. ret=%d.\n", ret);
				return ret;
			}
		}
	}

	return ret;
}

static void hisifb_pipe_clk_updt_disable_dirty_region(bool disable)
{
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;

	if (hisifd_list[PRIMARY_PANEL_IDX] == NULL)
		return;

	pipe_clk_ctrl = &(hisifd_list[PRIMARY_PANEL_IDX]->pipe_clk_ctrl);

	if (disable)
		pipe_clk_ctrl->dirty_region_updt_disable = 1;
	else
		pipe_clk_ctrl->dirty_region_updt_disable = 0;
}

static int pipe_clk_updt_dpe_on(struct hisi_fb_data_type *primary_hisifd, struct hisi_panel_info *primary_pinfo,
	struct hisifb_pipe_clk *pipe_clk_ctrl, uint64_t dp_pxl_clk_rate)
{
	int ret;

	ret = get_para_for_pipe_clk_updt(primary_hisifd, dp_pxl_clk_rate);
	if (ret < 0) {
		HISI_FB_ERR("pipe_clk para calc error. ret=%d.\n", ret);
		return ret;
	}

	if (pipe_clk_ctrl->pipe_clk_rate > primary_pinfo->pxl_clk_rate)
		hisifb_pipe_clk_updt_disable_dirty_region(true);

	ret = hisifb_pipe_clk_pre_process(primary_hisifd);
	if (ret < 0) {
		HISI_FB_ERR("pipe_clk_pre_process fail, ret=%d .\n", ret);
		hisifb_pipe_clk_updt_disable_dirty_region(false);
		return ret;
	}

	if (pipe_clk_ctrl->pipe_clk_updt_state != PARA_UPDT_END) {
		ret = wait_pipe_clk_para_updt_end();
		if (ret < 0) {
			HISI_FB_ERR("dp_on, pipe_clk_updt failed .\n");
			pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
			hisifb_pipe_clk_updt_disable_dirty_region(false);
			return ret;
		}
	}

	return ret;
}

static void pipe_clk_updt_dpe_off(struct hisi_fb_data_type *primary_hisifd, struct hisi_panel_info *primary_pinfo,
		struct hisifb_pipe_clk *pipe_clk_ctrl)
{
	if (pipe_clk_ctrl->pipe_clk_rate > primary_pinfo->pxl_clk_rate) {
		pipe_clk_ctrl->pipe_clk_rate = primary_pinfo->pxl_clk_rate;
		pipe_clk_ctrl->pipe_clk_rate_div = 1;
		pipe_clk_ctrl->pipe_clk_updt_hporch[0] = primary_pinfo->ldi.h_back_porch;
		pipe_clk_ctrl->pipe_clk_updt_hporch[1] = primary_pinfo->ldi.h_front_porch;
		pipe_clk_ctrl->pipe_clk_updt_hporch[2] = primary_pinfo->ldi.h_pulse_width;
		if (primary_hisifd->panel_power_on) {
			pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_NEED;
			hisi_fb_frame_refresh(primary_hisifd, "pipeclk");
		}

		hisifb_pipe_clk_updt_disable_dirty_region(false);
	}
}

int hisifb_wait_pipe_clk_updt(struct hisi_fb_data_type *hisifd, bool dp_on)
{
	struct hisi_fb_data_type *primary_hisifd = NULL;
	struct hisi_panel_info *primary_pinfo = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;
	int ret;
	uint64_t dp_pxl_clk_rate;

	ret = hisifb_pipe_clk_input_para_check(hisifd);
	hisi_check_and_return((ret < 0), ret, ERR, "input para check fail.\n");

	hisi_check_and_return((!is_dp_panel(hisifd)), 0, DEBUG, "fb%d, is not dp panel\n", hisifd->index);

	dp_pxl_clk_rate = hisifd->panel_info.pxl_clk_rate;
	primary_hisifd = hisifd_list[PRIMARY_PANEL_IDX];
	primary_pinfo = &(primary_hisifd->panel_info);

	HISI_FB_INFO("+, dp_on[%d], dp_pxl_clk_rate = %llu\n", dp_on, dp_pxl_clk_rate);

	if (dp_pxl_clk_rate <= primary_pinfo->pxl_clk_rate) {
		HISI_FB_INFO("pxl_clk_rate[%llu] is enough, return.\n", primary_pinfo->pxl_clk_rate);
		return 0;
	}

	pipe_clk_ctrl = &(primary_hisifd->pipe_clk_ctrl);
	pipe_clk_ctrl->pipe_clk_updt_times = 0;  /* clean upda_times when dp in/out. */
	pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;

	HISI_FB_DEBUG("primary_pinfo:\n"
		"pxl_clk_rate[%llu], pxl_clk_rate_div[%d], xres[%d], yres[%d],\n"
		"hbp[%d], hfp[%d], hpw[%d], vbp[%d], vfp[%d], vpw[%d].\n",
		primary_pinfo->pxl_clk_rate, primary_pinfo->pxl_clk_rate_div,
		primary_pinfo->xres, primary_pinfo->yres,
		primary_pinfo->ldi.h_back_porch, primary_pinfo->ldi.h_front_porch, primary_pinfo->ldi.h_pulse_width,
		primary_pinfo->ldi.v_back_porch, primary_pinfo->ldi.v_front_porch, primary_pinfo->ldi.v_pulse_width);

	if (dp_on) {
		ret = pipe_clk_updt_dpe_on(primary_hisifd, primary_pinfo, pipe_clk_ctrl, dp_pxl_clk_rate);
		if (ret < 0) {
			HISI_FB_ERR("pipe_clk_updt_dpe_on fail. ret=%d.\n", ret);
			return ret;
		}

	} else {
		/* dp_off */
		pipe_clk_updt_dpe_off(primary_hisifd, primary_pinfo, pipe_clk_ctrl);
	}

	HISI_FB_DEBUG(" -, pipe_clk_updt_state[%d],\n"
		"pipe_clk_rate[%llu], pipe_clk_rate_div[%d],\n"
		"pipe_clk_updt:hbp[%d], hfp[%d], hpw[%d].\n",
		pipe_clk_ctrl->pipe_clk_updt_state,
		pipe_clk_ctrl->pipe_clk_rate,
		pipe_clk_ctrl->pipe_clk_rate_div,
		pipe_clk_ctrl->pipe_clk_updt_hporch[0],
		pipe_clk_ctrl->pipe_clk_updt_hporch[1],
		pipe_clk_ctrl->pipe_clk_updt_hporch[2]);

	return 0;
}

static void wait_ldi_vstate_idle_time(struct hisi_fb_data_type *hisifd)
{
	int delay_count = 0;

	while ((((uint32_t)inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_VSTATE)) & 0x7FF) != 0x1) {
		if (++delay_count > 16) { /* wait times 16ms */
			HISI_FB_ERR("wait ldi vstate idle timeout.\n");
			break;
		}
		usleep_range(1000, 1000); /* delay 1ms */
	}
	HISI_FB_INFO("wait ldi vstate idle %d ms.\n", delay_count);
}

static void hisi_pipe_clk_updt_work_handler(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;

	pipe_clk_ctrl = container_of(work, struct hisifb_pipe_clk, pipe_clk_handle_work);
	hisifd = pipe_clk_ctrl->hisifd;
	hisi_check_and_no_retval((hisifd == NULL), ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval((hisifd->index != PRIMARY_PANEL_IDX), ERR,
		"fb%d pipe_clk_updt not support.\n", hisifd->index);

	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);

	down(&hisifd->blank_sem0);

	if (pipe_clk_ctrl->pipe_clk_updt_state == PARA_UPDT_END) {
		HISI_FB_INFO("fb%d, no need to updt pipe_clk, state[%d] .\n",
			hisifd->index, pipe_clk_ctrl->pipe_clk_updt_state);
		up(&hisifd->blank_sem0);
		return;
	}

	if (hisifd->pipe_clk_ctrl.underflow_int) {
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_NEED;
		up(&hisifd->blank_sem0);
		HISI_FB_INFO("fb%d, underflow hasn't been cleaded up .\n", hisifd->index);
		return;
	}

	if (!hisifd->panel_power_on) {
		HISI_FB_ERR("fb%d, panel_power_off .\n", hisifd->index);
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
		up(&hisifd->blank_sem0);
		return;
	}

	hisifb_activate_vsync(hisifd);
	disable_ldi(hisifd);
	wait_ldi_vstate_idle_time(hisifd);

	if (hisifb_pipe_clk_updt_handler(hisifd, false) < 0) {
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_NEED;
		pipe_clk_ctrl->pipe_clk_updt_times++;
		if (pipe_clk_ctrl->pipe_clk_updt_times >= 5) { /* 5 is try times */
			HISI_FB_ERR("pipe_clk_updt fail, total times > 5.\n");
			pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
		}
	} else {
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
	}

	enable_ldi(hisifd);
	hisifb_deactivate_vsync(hisifd);

	up(&hisifd->blank_sem0);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);
}

void hisifb_pipe_clk_updt_isr_handler(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_pipe_clk *pipe_clk_ctrl;

	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	if (pipe_clk_ctrl == NULL) {
		HISI_FB_ERR("pipe_clk_ctrl is NULL\n");
		return;
	}

	if (!pipe_clk_ctrl->inited) {
		HISI_FB_ERR("pipe_clk_ctrl has not been inited.\n");
		pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
		return;
	}

	queue_work(pipe_clk_ctrl->pipe_clk_handle_wq, &(pipe_clk_ctrl->pipe_clk_handle_work));
}

void hisifb_pipe_clk_updt_work_init(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_pipe_clk *pipe_clk_ctrl = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	pipe_clk_ctrl = &(hisifd->pipe_clk_ctrl);

	if (pipe_clk_ctrl->inited) {
		HISI_FB_INFO("pipe_clk_ctrl has been inited\n");
		return;
	}
	pipe_clk_ctrl->hisifd = hisifd;
	pipe_clk_ctrl->pipe_clk_rate = 0;
	pipe_clk_ctrl->underflow_int = 0;
	pipe_clk_ctrl->pipe_clk_updt_state = PARA_UPDT_END;
	pipe_clk_ctrl->dirty_region_updt_disable = 0;

	pipe_clk_ctrl->pipe_clk_handle_wq = create_singlethread_workqueue("pipe_clk_updt_work");
	if (pipe_clk_ctrl->pipe_clk_handle_wq == NULL) {
		HISI_FB_ERR("fb%d, create pipeclk_handle workqueue failed!\n", hisifd->index);
		return;
	}

	INIT_WORK(&(pipe_clk_ctrl->pipe_clk_handle_work), hisi_pipe_clk_updt_work_handler);
	pipe_clk_ctrl->inited = 1;
}
#pragma GCC diagnostic pop

