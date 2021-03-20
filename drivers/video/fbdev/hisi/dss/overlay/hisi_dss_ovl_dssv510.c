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

#include "hisi_overlay_utils.h"
#include "hisi_mmbuf_manager.h"

/*lint -e529*/
void hisi_dss_ovl_init(const char __iomem *ovl_base, dss_ovl_t *s_ovl,
	int ovl_idx)
{
	int i;

	hisi_check_and_no_retval(!ovl_base, ERR, "ovl_base is NULL!\n");
	hisi_check_and_no_retval(!s_ovl, ERR, "s_ovl is NULL!\n");

	memset(s_ovl, 0, sizeof(dss_ovl_t));

	s_ovl->ovl_size = inp32(ovl_base + OV_SIZE);
	s_ovl->ovl_bg_color = inp32(ovl_base + OV_BG_COLOR_RGB);
	s_ovl->ovl_bg_color_alpha = inp32(ovl_base + OV_BG_COLOR_A);
	s_ovl->ovl_dst_startpos = inp32(ovl_base + OV_DST_STARTPOS);
	s_ovl->ovl_dst_endpos = inp32(ovl_base + OV_DST_ENDPOS);
	s_ovl->ovl_gcfg = inp32(ovl_base + OV_GCFG);

	if (ovl_idx == DSS_OVL3) {
		for (i = 0; i < OVL_2LAYER_NUM; i++) {
			s_ovl->ovl_layer[i].layer_pos = inp32(ovl_base + OV_LAYER0_POS + i * 0x60);
			s_ovl->ovl_layer[i].layer_size = inp32(ovl_base + OV_LAYER0_SIZE + i * 0x60);
			s_ovl->ovl_layer[i].layer_pattern = inp32(ovl_base + OV_LAYER0_PATTERN_RGB + i * 0x60);
			s_ovl->ovl_layer[i].layer_pattern_alpha = inp32(ovl_base + OV_LAYER0_PATTERN_A + i * 0x60);
			s_ovl->ovl_layer[i].layer_alpha = inp32(ovl_base + OV_LAYER0_ALPHA_MODE + i * 0x60);
			s_ovl->ovl_layer[i].layer_alpha_a = inp32(ovl_base + OV_LAYER0_ALPHA_A + i * 0x60);
			s_ovl->ovl_layer[i].layer_cfg = inp32(ovl_base + OVL_LAYER0_CFG + i * 0x60);
			s_ovl->ovl_layer_pos[i].layer_pspos = inp32(ovl_base + OVL_LAYER0_PSPOS + i * 0x60);
			s_ovl->ovl_layer_pos[i].layer_pepos = inp32(ovl_base + OVL_LAYER0_PEPOS + i * 0x60);
		}

		s_ovl->ovl_block_size = inp32(ovl_base + OV2_BLOCK_SIZE);
	} else {
		for (i = 0; i < OV_8LAYER_NUM; i++) {
			s_ovl->ovl_layer[i].layer_pos = inp32(ovl_base + OV_LAYER0_POS + i * 0x60);
			s_ovl->ovl_layer[i].layer_size = inp32(ovl_base + OV_LAYER0_SIZE + i * 0x60);
			s_ovl->ovl_layer[i].layer_pattern = inp32(ovl_base + OV_LAYER0_PATTERN_RGB + i * 0x60);
			s_ovl->ovl_layer[i].layer_pattern_alpha = inp32(ovl_base + OV_LAYER0_PATTERN_A + i * 0x60);
			s_ovl->ovl_layer[i].layer_alpha = inp32(ovl_base + OV_LAYER0_ALPHA_MODE + i * 0x60);
			s_ovl->ovl_layer[i].layer_alpha_a = inp32(ovl_base + OV_LAYER0_ALPHA_A + i * 0x60);
			s_ovl->ovl_layer[i].layer_cfg = inp32(ovl_base + OV_LAYER0_CFG + i * 0x60);
			s_ovl->ovl_layer_pos[i].layer_pspos = inp32(ovl_base + OV_LAYER0_PSPOS + i * 0x60);
			s_ovl->ovl_layer_pos[i].layer_pepos = inp32(ovl_base + OV_LAYER0_PEPOS + i * 0x60);
		}

		s_ovl->ovl_block_size = inp32(ovl_base + OV8_BLOCK_SIZE);
	}
}
/*lint +e529*/

static void ovl_layer_used_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *ovl_base, dss_ovl_t *s_ovl, int i)
{
	if (s_ovl->ovl_layer_used[i] == 1) {
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_POS + i * 0x60,
			s_ovl->ovl_layer[i].layer_pos, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_SIZE + i * 0x60,
			s_ovl->ovl_layer[i].layer_size, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_PATTERN_RGB + i * 0x60,
			s_ovl->ovl_layer[i].layer_pattern, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_PATTERN_A + i * 0x60,
			s_ovl->ovl_layer[i].layer_pattern_alpha, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_ALPHA_MODE + i * 0x60,
			s_ovl->ovl_layer[i].layer_alpha, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_ALPHA_A + i * 0x60,
			s_ovl->ovl_layer[i].layer_alpha_a, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_CFG + i * 0x60,
			s_ovl->ovl_layer[i].layer_cfg, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_PSPOS + i * 0x60,
			s_ovl->ovl_layer_pos[i].layer_pspos, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_PEPOS + i * 0x60,
			s_ovl->ovl_layer_pos[i].layer_pepos, 32, 0);
	} else {
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_POS + i * 0x60,
			s_ovl->ovl_layer[i].layer_pos, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_SIZE + i * 0x60,
			s_ovl->ovl_layer[i].layer_size, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV_LAYER0_CFG + i * 0x60,
			s_ovl->ovl_layer[i].layer_cfg, 32, 0);
	}
}

void hisi_dss_ovl_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *ovl_base, dss_ovl_t *s_ovl, int ovl_idx)
{
	int i;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!ovl_base, ERR, "ovl_base is NULL!\n");
	hisi_check_and_no_retval(!s_ovl, ERR, "s_ovl is NULL!\n");

	if (ovl_idx == DSS_OVL3) {
		hisifd->set_reg(hisifd, ovl_base + OV2_REG_DEFAULT, 0x1, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV2_REG_DEFAULT, 0x0, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV2_BLOCK_DBG, 0x4, 32, 0);
	} else {
		hisifd->set_reg(hisifd, ovl_base + OV8_REG_DEFAULT, 0x1, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV8_REG_DEFAULT, 0x0, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV8_BLOCK_DBG, 0x4, 32, 0);
	}

	hisifd->set_reg(hisifd, ovl_base + OV_SIZE, s_ovl->ovl_size, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OV_BG_COLOR_RGB, s_ovl->ovl_bg_color, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OV_BG_COLOR_A, s_ovl->ovl_bg_color_alpha, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OV_DST_STARTPOS, s_ovl->ovl_dst_startpos, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OV_DST_ENDPOS, s_ovl->ovl_dst_endpos, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OV_GCFG, s_ovl->ovl_gcfg, 32, 0);

	if (ovl_idx == DSS_OVL3) {
		for (i = 0; i < OVL_2LAYER_NUM; i++)
			ovl_layer_used_set_reg(hisifd, ovl_base, s_ovl, i);

		hisifd->set_reg(hisifd, ovl_base + OV2_BLOCK_SIZE, s_ovl->ovl_block_size, 32, 0);
	} else {
		for (i = 0; i < OV_8LAYER_NUM; i++)
			ovl_layer_used_set_reg(hisifd, ovl_base, s_ovl, i);

		hisifd->set_reg(hisifd, ovl_base + OV8_BLOCK_SIZE, s_ovl->ovl_block_size, 32, 0);
	}
}

void hisi_dss_ov_set_reg_default_value(
	struct hisi_fb_data_type *hisifd, char __iomem *ovl_base, int ovl_idx)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is null\n");
		return;
	}

	if (!ovl_base) {
		HISI_FB_ERR("ovl_base is null\n");
		return;
	}

	if (ovl_idx == DSS_OVL3) {
		hisifd->set_reg(hisifd, ovl_base + OV2_REG_DEFAULT, 0x1, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV2_REG_DEFAULT, 0x0, 32, 0);
	} else {
		hisifd->set_reg(hisifd, ovl_base + OV8_REG_DEFAULT, 0x1, 32, 0);
		hisifd->set_reg(hisifd, ovl_base + OV8_REG_DEFAULT, 0x0, 32, 0);
	}
}

