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

#ifndef _HISI_DPE_PIPE_CLK_UTILS_H_
#define _HISI_DPE_PIPE_CLK_UTILS_H_
#include "hisi_fb.h"
#define DEFAULT_PIPE_CLK_RATE 1000000UL

struct hporch_value {
	uint32_t hbp;
	uint32_t hfp;
	uint32_t hpw;
};

struct hporch_new_value {
	uint32_t hbp_new;
	uint32_t hfp_new;
	uint32_t hpw_new;
};

int hisifb_get_ldi_hporch_updt_para(struct hisi_fb_data_type *hisifd, bool fps_updt_use);
int hisifb_wait_pipe_clk_updt(struct hisi_fb_data_type *hisifd, bool dp_on);
int hisifb_pipe_clk_updt_handler(struct hisi_fb_data_type *primary_hisifd, bool primary_panel_pwr_on);
void hisifb_pipe_clk_updt_isr_handler(struct hisi_fb_data_type *hisifd);
void hisifb_pipe_clk_updt_work_init(struct hisi_fb_data_type *hisifd);
#endif
