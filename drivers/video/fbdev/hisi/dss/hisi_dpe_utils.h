/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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

#ifndef HISI_DPE_UTILS_H
#define HISI_DPE_UTILS_H

#include "hisi_fb.h"

#define COMFORM_MAX 80
#define CHANGE_MAX 100
#define discount_coefficient(value) (CHANGE_MAX - (value))
#define CSC_VALUE_MAX 32768
#define CSC_VALUE_NUM 9

struct dpe_irq {
	const char *irq_name;
	irqreturn_t (*isr_fnc)(int irq, void *ptr);
	const char *dsi_irq_name;
	irqreturn_t (*dsi_isr_fnc)(int irq, void *ptr);
};

struct thd {
	uint32_t rqos_in;
	uint32_t rqos_out;
	uint32_t wqos_in;
	uint32_t wqos_out;
	uint32_t wr_wait;
	uint32_t cg_hold;
	uint32_t cg_in;
	uint32_t cg_out;
	uint32_t flux_req_befdfs_in;
	uint32_t flux_req_befdfs_out;
	uint32_t flux_req_aftdfs_in;
	uint32_t flux_req_aftdfs_out;
	uint32_t dfs_ok;
	uint32_t rqos_idle;
	uint32_t flux_req_sw_en;
};

struct ram_info {
	int dfs_ram;
	int sram_valid_num;
};
int hisifb_offline_vote_ctrl(struct hisi_fb_data_type *hisifd, bool offline_start);

void init_post_scf(struct hisi_fb_data_type *hisifd);
void init_dbuf(struct hisi_fb_data_type *hisifd);
void deinit_dbuf(struct hisi_fb_data_type *hisifd);
void init_dpp(struct hisi_fb_data_type *hisifd);
void init_acm(struct hisi_fb_data_type *hisifd);
void init_igm_gmp_xcc_gm(struct hisi_fb_data_type *hisifd);
void init_dither(struct hisi_fb_data_type *hisifd);
void init_ifbc(struct hisi_fb_data_type *hisifd);
void acm_set_lut(char __iomem *address, uint32_t table[], uint32_t size);
void acm_set_lut_hue(char __iomem *address, uint32_t table[], uint32_t size);

void hisifb_display_post_process_chn_init(struct hisi_fb_data_type *hisifd);
void init_ldi(struct hisi_fb_data_type *hisifd, bool fastboot_enable);
void deinit_ldi(struct hisi_fb_data_type *hisifd);
void enable_ldi(struct hisi_fb_data_type *hisifd);
void disable_ldi(struct hisi_fb_data_type *hisifd);
void ldi_frame_update(struct hisi_fb_data_type *hisifd, bool update);
void single_frame_update(struct hisi_fb_data_type *hisifd);
void ldi_data_gate(struct hisi_fb_data_type *hisifd, bool enble);
void init_dpp_csc(struct hisi_fb_data_type *hisifd);

void dpe_store_ct_csc_value(struct hisi_fb_data_type *hisifd, unsigned int csc_value[], unsigned int len);
int dpe_set_ct_csc_value(struct hisi_fb_data_type *hisifd);
ssize_t dpe_show_ct_csc_value(struct hisi_fb_data_type *hisifd, char *buf);
int dpe_set_xcc_csc_value(struct hisi_fb_data_type *hisifd);
/* isr */
irqreturn_t dss_dsi0_isr(int irq, void *ptr);
irqreturn_t dss_dsi1_isr(int irq, void *ptr);
irqreturn_t dss_sdp_isr_mipi_panel(int irq, void *ptr);
irqreturn_t dss_sdp_isr_dp(int irq, void *ptr);
irqreturn_t dss_pdp_isr(int irq, void *ptr);
irqreturn_t dss_sdp_isr(int irq, void *ptr);
irqreturn_t dss_adp_isr(int irq, void *ptr);
irqreturn_t dss_mdc_isr(int irq, void *ptr);

void dpe_interrupt_clear(struct hisi_fb_data_type *hisifd);
void dpe_interrupt_unmask(struct hisi_fb_data_type *hisifd);
void dpe_interrupt_mask(struct hisi_fb_data_type *hisifd);
int dpe_irq_enable(struct hisi_fb_data_type *hisifd);
int dpe_irq_disable(struct hisi_fb_data_type *hisifd);
int mediacrg_regulator_enable(struct hisi_fb_data_type *hisifd);
int mediacrg_regulator_disable(struct hisi_fb_data_type *hisifd);
int dpe_regulator_enable(struct hisi_fb_data_type *hisifd);
int dpe_regulator_disable(struct hisi_fb_data_type *hisifd);
int dpe_common_clk_enable(struct hisi_fb_data_type *hisifd);
int dpe_common_clk_enable_mmbuf_clk(struct hisi_fb_data_type *hisifd);
int dpe_inner_clk_enable(struct hisi_fb_data_type *hisifd);
int dpe_common_clk_disable(struct hisi_fb_data_type *hisifd);
int dpe_common_clk_disable_mmbuf_clk(struct hisi_fb_data_type *hisifd);
int dpe_inner_clk_disable(struct hisi_fb_data_type *hisifd);
void hisifb_pipe_clk_set_underflow_flag(struct hisi_fb_data_type *hisifd, bool underflow);
void dss_inner_clk_common_enable(struct hisi_fb_data_type *hisifd, bool fastboot_enable);

void dss_inner_clk_common_disable(struct hisi_fb_data_type *hisifd);

void dss_inner_clk_pdp_enable(struct hisi_fb_data_type *hisifd, bool fastboot_enable);
void dss_inner_clk_pdp_disable(struct hisi_fb_data_type *hisifd);

void dss_inner_clk_sdp_enable(struct hisi_fb_data_type *hisifd);
void dss_inner_clk_sdp_disable(struct hisi_fb_data_type *hisifd);

void dpe_update_g_comform_discount(unsigned int value);
int dpe_set_comform_ct_csc_value(struct hisi_fb_data_type *hisifd);
ssize_t dpe_show_comform_ct_csc_value(struct hisi_fb_data_type *hisifd, char *buf);

void dpe_init_led_rg_ct_csc_value(void);
void dpe_store_led_rg_ct_csc_value(unsigned int csc_value[], unsigned int len);
int dpe_set_led_rg_ct_csc_value(struct hisi_fb_data_type *hisifd);
ssize_t dpe_show_led_rg_ct_csc_value(char *buf);
ssize_t dpe_show_cinema_value(struct hisi_fb_data_type *hisifd, char *buf);
void dpe_set_cinema_acm(struct hisi_fb_data_type *hisifd, unsigned int value);
int dpe_set_cinema(struct hisi_fb_data_type *hisifd, unsigned int value);
void dpe_update_g_acm_state(unsigned int value);
void dpe_set_acm_state(struct hisi_fb_data_type *hisifd);
ssize_t dpe_show_acm_state(char *buf);
void dpe_update_g_gmp_state(unsigned int value);
void dpe_set_gmp_state(struct hisi_fb_data_type *hisifd);
ssize_t dpe_show_gmp_state(char *buf);
int dpe_irq_disable_nosync(struct hisi_fb_data_type *hisifd);

void no_memory_lp_ctrl(struct hisi_fb_data_type *hisifd);
void lp_first_level_clk_gate_ctrl(struct hisi_fb_data_type *hisifd);
void lp_second_level_clk_gate_ctrl(struct hisi_fb_data_type *hisifd);

#endif

