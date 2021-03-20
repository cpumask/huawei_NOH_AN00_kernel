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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "hisi_overlay_utils.h"
#include "../hisi_display_effect.h"
#include "../hisi_dpe_utils.h"
#include "../hisi_ovl_online_wb.h"
#include "../hisi_mmbuf_manager.h"
#include "../hisi_spr_dsc.h"
#include "../hisi_frame_rate_ctrl.h"

struct prev_module_param {
	bool has_ovl;
	bool has_rot;
	int32_t ovl_idx;
	uint32_t chn_idx;
	uint32_t display_id;
};

static inline bool hisi_dss_is_sharpness_support(int32_t width, int32_t height)
{
	/* width porch and height porch, width and height resolution */
	return ((width >= 16) && (width <= 1600) && (height >= 4) && (height <= 2560));
}

static int hisi_dss_add_rch_cmdlist(struct hisi_fb_data_type *hisifd,  int chn_idx,
	uint32_t wb_type, bool enable_cmdlist)
{
	int ret = 0;
	uint32_t tmp = 0;
	union cmd_flag flag = {
		.bits.pending = 0,
		.bits.task_end = 0,
		.bits.last = 0,
	};

	if (enable_cmdlist) {
		hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_chnidx((uint32_t)chn_idx);
		tmp = (0x1 << (uint32_t)(hisifd->cmdlist_idx));

		/* FIXME:base, dim */
		/* add rch cmdlist */
		ret = hisi_cmdlist_add_new_node(hisifd, tmp, flag, 0);
		hisi_check_and_return((ret != 0), ret, ERR, "fb%d, hisi_cmdlist_add_new_node err:%d\n",
			hisifd->index, ret);
	}

	return ret;
}

static void hisi_dss_dma_ch_set_reg(struct hisi_fb_data_type *hisifd, int chn_idx,
	dss_module_reg_t *dss_module)
{
	if (chn_idx < DSS_WCHN_W0 || chn_idx == DSS_RCHN_V2)
		hisi_dss_rdma_set_reg(hisifd, dss_module->dma_base[chn_idx], &(dss_module->rdma[chn_idx]));
	else
		hisi_dss_wdma_set_reg(hisifd, dss_module->dma_base[chn_idx], &(dss_module->wdma[chn_idx]));

	if ((chn_idx == DSS_RCHN_V0) || (chn_idx == DSS_RCHN_V1) || (chn_idx == DSS_RCHN_V2)) {
		hisi_dss_rdma_u_set_reg(hisifd, dss_module->dma_base[chn_idx], &(dss_module->rdma[chn_idx]));
		hisi_dss_rdma_v_set_reg(hisifd, dss_module->dma_base[chn_idx], &(dss_module->rdma[chn_idx]));
	}
}

int hisi_dss_ch_module_set_regs(struct hisi_fb_data_type *hisifd, int32_t mctl_idx, int chn_idx,
	uint32_t wb_type, bool enable_cmdlist)
{
	int i;
	dss_module_reg_t *dss_module = NULL;

	hisi_check_and_return((hisifd == NULL), -1, DEBUG, "hisifd is NULL!\n");
	hisi_check_and_return(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), -1, DEBUG,
		"chn_idx is out of the range!\n");

	i = chn_idx;
	dss_module = &(hisifd->dss_module);

	if (hisi_dss_add_rch_cmdlist(hisifd, chn_idx, wb_type, enable_cmdlist) != 0)
		return -1;

	if (dss_module->mctl_ch_used[i] == 1)
		hisi_dss_mctl_ch_set_reg(hisifd, &(dss_module->mctl_ch_base[i]),
			&(dss_module->mctl_ch[i]), mctl_idx, i);

	if (dss_module->smmu_used == 1)
		hisi_dss_smmu_ch_set_reg(hisifd, dss_module->smmu_base, &(dss_module->smmu), i);

	if (dss_module->dma_used[i] == 1)
		hisi_dss_dma_ch_set_reg(hisifd, i, dss_module);

	if (dss_module->aif_ch_used[i] == 1)
		hisi_dss_aif_ch_set_reg(hisifd, dss_module->aif_ch_base[i], &(dss_module->aif[i]));

	if (dss_module->aif1_ch_used[i] == 1)
		hisi_dss_aif_ch_set_reg(hisifd, dss_module->aif1_ch_base[i], &(dss_module->aif1[i]));

	if (dss_module->mif_used[i] == 1)
		hisi_dss_mif_set_reg(hisifd, dss_module->mif_ch_base[i], &(dss_module->mif[i]), i);

#ifdef SUPPORT_RCH_CLD
	if (dss_module->cld_used == 1)
		hisi_dss_cld_set_reg(hisifd, dss_module->cld_base, &(dss_module->cld), i);
#endif

	if (dss_module->dfc_used[i] == 1)
		hisi_dss_dfc_set_reg(hisifd, dss_module->dfc_base[i], &(dss_module->dfc[i]));

	if (dss_module->scl_used[i] == 1)
		hisi_dss_scl_set_reg(hisifd, dss_module->scl_base[i], &(dss_module->scl[i]));

	if (hisifd->dss_module.post_clip_used[i])
		hisi_dss_post_clip_set_reg(hisifd, dss_module->post_clip_base[i], &(dss_module->post_clip[i]), i);

	if (dss_module->pcsc_used[i] == 1)
		hisi_dss_csc_set_reg(hisifd, dss_module->pcsc_base[i], &(dss_module->pcsc[i]));

	if (dss_module->arsr2p_used[i] == 1)
		hisi_dss_arsr2p_set_reg(hisifd, dss_module->arsr2p_base[i], &(dss_module->arsr2p[i]));

	if (dss_module->csc_used[i] == 1)
		hisi_dss_csc_set_reg(hisifd, dss_module->csc_base[i], &(dss_module->csc[i]));

	if (dss_module->mctl_ch_used[i] == 1)
		hisi_dss_mctl_sys_ch_set_reg(hisifd, &(dss_module->mctl_ch_base[i]),
			&(dss_module->mctl_ch[i]), i, true);

	return 0;
}

static void hisi_dss_set_idle_display(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, bool is_first_ov_block)
{
	if (is_first_ov_block && (pov_req != NULL))
		hisifd->is_idle_display = pov_req->video_idle_status;
}

bool hisi_dss_is_async_play(struct hisi_fb_data_type *hisifd)
{
	if (g_debug_enable_asynchronous_play == 0)
		return false;

	if (!is_mipi_cmd_panel(hisifd))
		return false;

	if (!is_lcd_dfr_support(hisifd))
		return false;

	return true;
}

int hisi_dss_ov_module_set_regs(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req, int ovl_idx,
	struct ov_module_set_regs_flag ov_module_flag)
{
	dss_module_reg_t *dss_module = NULL;
	int ret = 0;
	uint32_t tmp = 0;
	union cmd_flag flag = {
		.bits.pending = 0,
		.bits.task_end = ov_module_flag.task_end,
		.bits.last = ov_module_flag.last,
	};

	hisi_check_and_return((hisifd == NULL), -1, DEBUG, "hisifd is NULL!\n");

	/* chicago copybit no ovl */
	if ((pov_req != NULL) && pov_req->wb_layer_infos[0].chn_idx == DSS_WCHN_W2)
		return 0;

	dss_module = &(hisifd->dss_module);
	hisi_check_and_return(((ovl_idx < 0) || (ovl_idx >= DSS_OVL_IDX_MAX)), -1, DEBUG,
		"ovl_idx More than Array Upper Limit!\n");

	if (ov_module_flag.enable_cmdlist) {
		/* add ov cmdlist */
		hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_ovlidx(ovl_idx);
		tmp = (0x1 << (uint32_t)(hisifd->cmdlist_idx));

		ret = hisi_cmdlist_add_new_node(hisifd, tmp, flag, 0);
		hisi_check_and_return((ret != 0), ret, ERR, "fb%d, hisi_cmdlist_add_new_node err:%d\n",
			hisifd->index, ret);
	}

	hisi_dss_set_idle_display(hisifd, pov_req, ov_module_flag.is_first_ov_block);

	if (dss_module->mctl_used[ovl_idx] == 1)
		hisi_dss_mctl_ov_set_reg(hisifd, dss_module->mctl_base[ovl_idx],
			&(dss_module->mctl[ovl_idx]), ovl_idx, ov_module_flag.enable_cmdlist);

	if (ov_module_flag.is_first_ov_block)
		if (dss_module->dirty_region_updt_used == 1)
			hisi_dss_dirty_region_dbuf_set_reg(hisifd, hisifd->dss_base, &(dss_module->dirty_region_updt));

	if (dss_module->ov_used[ovl_idx] == 1)
		hisi_dss_ovl_set_reg(hisifd, dss_module->ov_base[ovl_idx], &(dss_module->ov[ovl_idx]), ovl_idx);

	if (dss_module->post_scf_used == 1)
		hisi_dss_post_scf_set_reg(hisifd, dss_module->post_scf_base, &(dss_module->post_scf));

	if (ov_module_flag.is_first_ov_block) {
		hisi_dss_dpp_acm_gm_set_reg(hisifd);
		hisi_dss_dpp_ace_set_reg(hisifd);
		hisi_dss_dpp_hiace_set_reg(hisifd);
		hisi_dss_effect_set_reg(hisifd);
#ifdef CONFIG_HISI_FB_ALSC
		hisi_alsc_set_reg(hisifd);
#endif
	}

	if (dss_module->mctl_sys_used == 1)
		hisi_dss_mctl_sys_set_reg(hisifd, dss_module->mctl_sys_base, &(dss_module->mctl_sys), ovl_idx);

	return 0;
}

static int hisi_dss_layer_set_regs(struct hisi_fb_data_type *hisifd, uint32_t chn_idx)
{
	dss_module_reg_t *dss_module = NULL;

	dss_module = &(hisifd->dss_module);

	/* RCH default */
	hisi_dss_chn_set_reg_default_value(hisifd, dss_module->dma_base[chn_idx]);

	/* remove smmu config by cmdlist, not for smmu without shadow register */
#ifndef CONFIG_DSS_SMMU_V3
	hisi_dss_smmu_ch_set_reg(hisifd, dss_module->smmu_base, &(dss_module->smmu), chn_idx);
#endif
	/* MIF */
	hisi_dss_mif_set_reg(hisifd, dss_module->mif_ch_base[chn_idx], &(dss_module->mif[chn_idx]), chn_idx);

	/* AIF */
	hisi_dss_aif_ch_set_reg(hisifd, dss_module->aif_ch_base[chn_idx], &(dss_module->aif[chn_idx]));

	/* MCTL */
	dss_module->mctl_ch[chn_idx].chn_mutex =
		set_bits32(dss_module->mctl_ch[chn_idx].chn_mutex, 1, 1, 0);
	dss_module->mctl_ch[chn_idx].chn_flush_en =
		set_bits32(dss_module->mctl_ch[chn_idx].chn_flush_en, 1, 1, 0);
	dss_module->mctl_ch[chn_idx].chn_ov_oen =
		set_bits32(dss_module->mctl_ch[chn_idx].chn_ov_oen, 0, 32, 0);
	dss_module->mctl_ch_used[chn_idx] = 1;

	hisi_dss_mctl_sys_ch_set_reg(hisifd, &(dss_module->mctl_ch_base[chn_idx]),
		&(dss_module->mctl_ch[chn_idx]), chn_idx, false);

	return 0;
}

static void get_primary_online_mmbuf(int32_t ovl_idx, uint32_t chn_idx, dss_layer_t *layer)
{
	if (ovl_idx == DSS_OVL0) {
		g_primary_online_mmbuf[chn_idx].mmbuf.addr = layer->img.mmbuf_base;
		g_primary_online_mmbuf[chn_idx].mmbuf.size = layer->img.mmbuf_size;
		g_primary_online_mmbuf[chn_idx].ov_idx = ovl_idx;
	}
}

static void get_external_online_mmbuf(int32_t ovl_idx, uint32_t chn_idx, const dss_layer_t *layer)
{
	if (ovl_idx == DSS_OVL1) {
		g_external_online_mmbuf[chn_idx].mmbuf.addr = layer->img.mmbuf_base;
		g_external_online_mmbuf[chn_idx].mmbuf.size = layer->img.mmbuf_size;
		g_external_online_mmbuf[chn_idx].ov_idx = ovl_idx;
	}
}

static int hisi_dss_block_set_regs(struct hisi_fb_data_type *hisifd, dss_overlay_block_t *pov_h_block,
	dss_mmbuf_t *offline_mmbuf,
	uint32_t *cmdlist_idxs_temp, int32_t ovl_idx)
{
	dss_layer_t *layer = NULL;
	uint32_t chn_idx = 0;
	uint32_t i = 0;
	int j = 0;
	uint32_t tmp = 0;

	for (i = 0; i < pov_h_block->layer_nums; i++) {
		layer = &(pov_h_block->layer_infos[i]);
		chn_idx = layer->chn_idx;
		if (chn_idx >= DSS_CHN_MAX_DEFINE) {
			HISI_FB_ERR("chn_idx=%d, More than Array Upper Limit\n", chn_idx);
			return -EINVAL;
		}
		if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR))
			continue;

		if ((layer->need_cap & (CAP_AFBCD | CAP_HFBCD | CAP_HEBCD)) &&
			(layer->dst_rect.y >= pov_h_block->ov_block_rect.y)) {
			if (chn_idx < DSS_CHN_MAX_DEFINE) {
				get_primary_online_mmbuf(ovl_idx, chn_idx, layer);
				get_external_online_mmbuf(ovl_idx, chn_idx, layer);
			}
		}

		if ((ovl_idx == DSS_OVL2) || (ovl_idx == DSS_OVL3)) {
			if (layer->need_cap & (CAP_AFBCD | CAP_HFBCD | CAP_HEBCD)) {
				if (j < DSS_CHN_MAX_DEFINE) {
					offline_mmbuf[j].addr = layer->img.mmbuf_base;
					offline_mmbuf[j].size = layer->img.mmbuf_size;
					j++;
				}
			}
		}

		hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_chnidx(chn_idx);
		tmp = (0x1 << (uint32_t)(hisifd->cmdlist_idx));

		if ((*cmdlist_idxs_temp & tmp) != tmp)
			continue;
		*cmdlist_idxs_temp &= (~tmp);

		hisi_dss_layer_set_regs(hisifd, chn_idx);
	}

	return 0;
}

static int hisi_dss_mctl_sys_ov_set_reg(struct hisi_fb_data_type *hisifd, dss_module_reg_t *dss_module,
	int32_t ovl_idx)
{
	/* MCTL ov */
	dss_module->mctl_sys.chn_ov_sel_used[ovl_idx] = 1;
	dss_module->mctl_sys.wch_ov_sel_used[ovl_idx - DSS_OVL2] = 1;
	dss_module->mctl_sys.ov_flush_en_used[ovl_idx] = 1;
	dss_module->mctl_sys.ov_flush_en[ovl_idx] =
		set_bits32(dss_module->mctl_sys.ov_flush_en[ovl_idx], 0x1, 1, 0);
	hisi_dss_mctl_sys_set_reg(hisifd, dss_module->mctl_sys_base, &(dss_module->mctl_sys), ovl_idx);

	return 0;
}

static void hisi_dss_wb_layers_set_regs(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, struct prev_module_param *param)
{
	dss_wb_layer_t *wb_layer = NULL;
	uint32_t i;

	for (i = 0; i < pov_req->wb_layer_nums; i++) {
		wb_layer = &(pov_req->wb_layer_infos[i]);
		param->chn_idx = wb_layer->chn_idx;
		param->display_id = wb_layer->dst.display_id;
		if (wb_layer->transform & HISI_FB_TRANSFORM_ROT_90)
			param->has_rot = true;

		/* chicago copybit */
		hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_chnidx(param->chn_idx);

		hisi_dss_layer_set_regs(hisifd, param->chn_idx);
	}
}

int hisi_dss_prev_module_set_regs(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, uint32_t cmdlist_pre_idxs, bool enable_cmdlist, int *use_comm_mmbuf)
{
	dss_module_reg_t *dss_module = NULL;
	struct prev_module_param param = {true, false, 0, 0, 0};
	uint32_t i = 0;
	int ret;
	uint32_t cmdlist_idxs_temp;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_mmbuf_t offline_mmbuf[DSS_CHN_MAX_DEFINE] = { {0} };
	union cmd_flag flag = {
		.bits.pending = 0,
		.bits.task_end = 1,
		.bits.last = 1,
	};

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is nullptr!\n");
	hisi_check_and_return(!pov_req, -EINVAL, ERR, "pov_req is nullptr!\n");
	param.ovl_idx = pov_req->ovl_idx;
	dss_module = &(hisifd->dss_module);

	if (enable_cmdlist) {
		/* clear prev chn cmdlist reg default */
		if (pov_req->wb_enable) {
			ret = hisi_cmdlist_add_new_node(hisifd, cmdlist_pre_idxs, flag, 1);
		} else {
			flag.bits.task_end = 0;
			flag.bits.last = 0;
			ret = hisi_cmdlist_add_new_node(hisifd, cmdlist_pre_idxs, flag, 0);
		}
		hisi_check_and_return((ret != 0), ret, ERR, "fb%d, add_new_node err:%d\n", hisifd->index, ret);
	}
	if ((hisifd->index == MEDIACOMMON_PANEL_IDX) && (pov_req->wb_compose_type == DSS_WB_COMPOSE_COPYBIT))
		param.has_ovl = false;

	memset(offline_mmbuf, 0x0, sizeof(offline_mmbuf));
	cmdlist_idxs_temp = cmdlist_pre_idxs;
	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr;

	for (i = 0; i < pov_req->ov_block_nums; i++)
		hisi_dss_block_set_regs(hisifd, &(pov_h_block_infos[i]), offline_mmbuf,
			&cmdlist_idxs_temp, param.ovl_idx);

	if (pov_req->wb_enable && ((param.ovl_idx > DSS_OVL1) || (!param.has_ovl))) {
		if (param.has_ovl) {
			hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_ovlidx(param.ovl_idx);
			/* OV default */
			hisi_dss_ov_set_reg_default_value(hisifd, dss_module->ov_base[param.ovl_idx], param.ovl_idx);
		}

		hisi_dss_wb_layers_set_regs(hisifd, pov_req, &param);

		if (param.has_ovl) {  /* chicago copybit */
			hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_ovlidx(param.ovl_idx);

			hisi_dss_mctl_sys_ov_set_reg(hisifd, dss_module, param.ovl_idx);
		}
		hisi_dss_check_use_comm_mmbuf(param.display_id, use_comm_mmbuf, offline_mmbuf, param.has_rot);
	}

	return 0;
}

bool hisi_dss_check_reg_reload_status(struct hisi_fb_data_type *hisifd)
{
	if (g_fpga_flag == 1) {
		/* android reboot test may cause dss underflow and clear ack timeout in fpga */
		/* delay 1s to ensure dss status is stable when display off */
		mdelay(1000);  /* delay 1000ms */
		HISI_FB_INFO("need delay 1s in fpga!\n");
	} else {
		mdelay(50);  /* delay 50ms */
	}

	(void)hisifd;

	return true;
}

bool hisi_dss_check_crg_sctrl_status(struct hisi_fb_data_type *hisifd)
{
	uint32_t crg_state_check;
	uint32_t sctrl_mmbuf_dss_check;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL\n");
		return false;
	}

	crg_state_check = inp32(hisifd->peri_crg_base + PERCLKEN3);  //lint !e529
	if ((crg_state_check & 0x23000) != 0x23000) {  /* mask */
		HISI_FB_ERR("dss crg_clk_enable failed, crg_state_check = 0x%x\n", crg_state_check);
		return false;
	}

	crg_state_check = inp32(hisifd->peri_crg_base + PERRSTSTAT3);  //lint !e529
	if ((crg_state_check | 0xfffff3ff) != 0xfffff3ff) {  /* mask */
		HISI_FB_ERR("dss crg_reset failed, crg_state_check = 0x%x\n", crg_state_check);
		return false;
	}

	crg_state_check = inp32(hisifd->peri_crg_base + ISOSTAT);  //lint !e529
	if ((crg_state_check | 0xffffffbf) != 0xffffffbf) {  /* mask */
		HISI_FB_ERR("dss iso_disable failed, crg_state_check = 0x%x\n", crg_state_check);
		return false;
	}

	crg_state_check = inp32(hisifd->peri_crg_base + PERPWRSTAT);  //lint !e529
	if ((crg_state_check & 0x20) != 0x20) {  /* mask */
		HISI_FB_ERR("dss subsys regulator_enabel failed, crg_state_check = 0x%x\n", crg_state_check);
		return false;
	}

	sctrl_mmbuf_dss_check = inp32(hisifd->sctrl_base + SCPERCLKEN1);  //lint !e529
	if ((sctrl_mmbuf_dss_check & 0x1000000) != 0x1000000) {  /* mask */
		HISI_FB_ERR("dss subsys mmbuf_dss_clk_enabel failed, sctrl_mmbuf_dss_check = 0x%x\n",
			sctrl_mmbuf_dss_check);
		return false;
	}

	return true;
}

static uint32_t hisi_ldi_get_cmdlist_idxs(struct hisi_fb_data_type *hisifd)
{
	int ret;
	dss_overlay_t *pov_req_prev = NULL;
	dss_overlay_t *pov_req_prev_prev = NULL;
	uint32_t cmdlist_idxs_prev = 0;
	uint32_t cmdlist_idxs_prev_prev = 0;

	pov_req_prev = &(hisifd->ov_req_prev);
	pov_req_prev_prev = &(hisifd->ov_req_prev_prev);

	ret = hisi_cmdlist_get_cmdlist_idxs(pov_req_prev, &cmdlist_idxs_prev, NULL);
	if (ret != 0)
		HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req_prev failed! ret = %d\n", hisifd->index, ret);

	ret = hisi_cmdlist_get_cmdlist_idxs(pov_req_prev_prev, &cmdlist_idxs_prev_prev, NULL);
	if (ret != 0)
		HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req_prev_prev failed! ret = %d\n",
			hisifd->index, ret);

	HISI_FB_INFO("fb%d, -. cmdlist_idxs_prev = 0x%x, cmdlist_idxs_prev_prev = 0x%x\n",
		hisifd->index, cmdlist_idxs_prev, cmdlist_idxs_prev_prev);

	return (cmdlist_idxs_prev | cmdlist_idxs_prev_prev);
}

void hisi_ldi_underflow_handle_func(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t cmdlist_idxs;

	hisifd = container_of(work, struct hisi_fb_data_type, ldi_underflow_work);
	hisi_check_and_no_retval((hisifd == NULL), ERR, "hisifd is NULL point!\n");
	HISI_FB_INFO("fb%d, +.\n", hisifd->index);

	down(&hisifd->blank_sem0);
	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		up(&hisifd->blank_sem0);
		return;
	}

	cmdlist_idxs = hisi_ldi_get_cmdlist_idxs(hisifd);
	hisifb_activate_vsync(hisifd);
	hisi_cmdlist_config_reset(hisifd, &(hisifd->ov_req_prev), cmdlist_idxs);
	hisifb_deactivate_vsync(hisifd);
	hisifb_pipe_clk_set_underflow_flag(hisifd, false);
	up(&hisifd->blank_sem0);
	HISI_FB_INFO("fb%d, -.\n", hisifd->index);
}

int hisi_dss_get_ov_data_from_user(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, const void __user *argp)
{
	dss_overlay_block_t *pov_h_block_infos = NULL;
	int ret;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");
	hisi_check_and_return(!pov_req, -EINVAL, ERR, "pov_req is NULL!\n");
	hisi_check_and_return(!argp, -EINVAL, ERR, "user data is invalid!\n");

	pov_h_block_infos = hisifd->ov_block_infos;

	ret = copy_from_user(pov_req, argp, sizeof(dss_overlay_t));
	if (ret) {
		HISI_FB_ERR("fb%d, copy_from_user failed!\n", hisifd->index);
		return -EINVAL;
	}

	pov_req->release_fence = -1;
	pov_req->retire_fence = -1;

	if ((pov_req->ov_block_nums <= 0) ||
		(pov_req->ov_block_nums > HISI_DSS_OV_BLOCK_NUMS)) {
		HISI_FB_ERR("fb%d, ov_block_nums(%d) is out of range!\n",
			hisifd->index, pov_req->ov_block_nums);
		return -EINVAL;
	}

	if ((dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr == NULL) {
		HISI_FB_ERR("pov_req->ov_block_infos_ptr is NULL\n");
		return -EINVAL;
	}
	ret = copy_from_user(pov_h_block_infos, (dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr,
		pov_req->ov_block_nums * sizeof(dss_overlay_block_t));
	if (ret) {
		HISI_FB_ERR("fb%d, dss_overlay_block_t copy_from_user failed!\n",
			hisifd->index);
		return -EINVAL;
	}

	ret = hisi_dss_check_userdata(hisifd, pov_req, pov_h_block_infos);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_check_userdata failed!\n", hisifd->index);
		return -EINVAL;
	}
	pov_req->ov_block_infos_ptr = (uint64_t)(uintptr_t)pov_h_block_infos;

	return ret;
}

uint64_t hisi_dss_get_vsync_timediff(struct hisi_fb_data_type *hisifd)
{
	uint64_t vsync_timediff = 0;
	struct hisi_panel_info *pinfo = NULL;
	uint32_t current_fps = 0;

	hisi_check_and_return((hisifd == NULL), 0, ERR, "hisifd is NULL\n");

	pinfo = &(hisifd->panel_info);

	if (pinfo->mipi.dsi_timing_support) {
		current_fps = (pinfo->fps == FPS_HIGH_60HZ) ? FPS_60HZ : pinfo->fps;
		if (current_fps != 0)
			vsync_timediff = 1000000000UL / (uint64_t)current_fps;
	} else {
		vsync_timediff = (uint64_t)(hisifd->panel_info.xres + hisifd->panel_info.ldi.h_back_porch +
			hisifd->panel_info.ldi.h_front_porch + hisifd->panel_info.ldi.h_pulse_width) *
			(hisifd->panel_info.yres + hisifd->panel_info.ldi.v_back_porch +
			hisifd->panel_info.ldi.v_front_porch + hisifd->panel_info.ldi.v_pulse_width) *
			1000000000UL / hisifd->panel_info.pxl_clk_rate;
	}

	HISI_FB_DEBUG("dsi_timing_support=%d, vsync_timediff=%llu, current_fps=%d, pinfo_fps=%d\n",
		pinfo->mipi.dsi_timing_support, vsync_timediff, current_fps, pinfo->fps);

	return vsync_timediff;
}

int hisi_dss_check_vsync_timediff(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	ktime_t prepare_timestamp;
	uint64_t vsync_timediff;
	uint64_t timestamp = 4000000;  /* 4ms */
	int ret;

	hisi_check_and_return((hisifd == NULL), -EINVAL, ERR, "hisifd is NULL\n");
	hisi_check_and_return((pov_req == NULL), -EINVAL, ERR, "pov_req is NULL\n");

	ret = hisifb_buf_sync_handle(hisifd, pov_req);
	hisi_check_and_return((ret < 0), -1, ERR, "fb%d, hisifb_buf_sync_handle failed! ret=%d\n", hisifd->index, ret);

	if (is_mipi_video_panel(hisifd) || is_dp_panel(hisifd)) {
		vsync_timediff = hisi_dss_get_vsync_timediff(hisifd);
		if (vsync_timediff == 0) {
			HISI_FB_ERR("error vsync_timediff, maybe pinfo->fps is 0\n");
			return -1;
		}

		prepare_timestamp = ktime_get();
		if ((ktime_to_ns(prepare_timestamp) > ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp)) &&
			((ktime_to_ns(prepare_timestamp) - ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp)) <
			(int64_t)(vsync_timediff - timestamp)) &&
			(ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp_prev) !=
			ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp))) {
			HISI_FB_DEBUG("fb%d, vsync_timediff=%llu, timestamp_diff=%llu.\n",
				hisifd->index, vsync_timediff,
				ktime_to_ns(prepare_timestamp) - ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp));
		} else {
			HISI_FB_DEBUG("fb%d, vsync_timediff=%llu.\n", hisifd->index, vsync_timediff);

			ret = hisi_vactive0_start_config(hisifd, pov_req);
			hisi_check_and_return((ret != 0), -1, ERR, "fb%d, hisi_vactive0_start_config fail ret=%d\n",
				hisifd->index, ret);
		}
	}

	return 0;
}

#pragma GCC diagnostic pop
