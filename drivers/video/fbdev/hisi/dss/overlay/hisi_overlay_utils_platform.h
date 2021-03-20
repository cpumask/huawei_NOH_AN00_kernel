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

#ifndef HISI_OVERLAY_UTILS_PLATFORM_H
#define HISI_OVERLAY_UTILS_PLATFORM_H

#include "../hisi_fb.h"

void hisifb_dss_disreset(struct hisi_fb_data_type *hisifd);
void hisi_dump_current_info(struct hisi_fb_data_type *hisifd);
void hisi_dss_qos_on(struct hisi_fb_data_type *hisifd);
int hisi_overlay_pan_display(struct hisi_fb_data_type *hisifd);
int hisi_ov_online_play(struct hisi_fb_data_type *hisifd, void __user *argp);
int hisi_ov_offline_play(struct hisi_fb_data_type *hisifd, const void __user *argp);
int hisi_ov_asynchronous_play(struct hisi_fb_data_type *hisifd, void __user *argp);
void hisi_dss_ovl_init(const char __iomem *ovl_base, dss_ovl_t *s_ovl, int ovl_idx);
void hisi_dss_ovl_set_reg(struct hisi_fb_data_type *hisifd, char __iomem *ovl_base, dss_ovl_t *s_ovl, int ovl_idx);
void hisi_dss_ov_set_reg_default_value(struct hisi_fb_data_type *hisifd,
	char __iomem *ovl_base, int ovl_idx);
int hisi_dss_check_pure_layer(struct hisi_fb_data_type *hisifd, dss_overlay_block_t *pov_h_block,
	void __user *argp);
int hisi_dss_check_userdata(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_overlay_block_t *pov_h_block_infos);
int hisi_dss_check_layer_par(struct hisi_fb_data_type *hisifd, dss_layer_t *layer);
int hisi_dss_aif_handler(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, dss_overlay_block_t *pov_h_block);
void hisi_dss_aif_init(const char __iomem *aif_ch_base,	dss_aif_t *s_aif);
void hisi_dss_aif_ch_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *aif_ch_base, dss_aif_t *s_aif);
int hisi_dss_aif_ch_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_layer_t *layer, dss_rect_t *wb_dst_rect, dss_wb_layer_t *wb_layer);
int hisi_dss_aif1_ch_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_layer_t *layer, dss_wb_layer_t *wb_layer, int ovl_idx);
int hisi_dss_hfbcd_config(struct hisi_fb_data_type *hisifd,
	int ovl_idx, dss_layer_t *layer, struct hisi_ov_compose_rect *ov_compose_rect,
	struct hisi_ov_compose_flag *ov_compose_flag);
void hisi_dss_hfbcd_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *dma_base, dss_rdma_t *s_dma);
int hisi_dss_hfbce_config(struct hisi_fb_data_type *hisifd,
	dss_wb_layer_t *layer, dss_rect_t aligned_rect, dss_rect_t *ov_block_rect, bool last_block);
void hisi_dss_hfbce_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *wdma_base, dss_wdma_t *s_wdma);
int hisi_dss_hebce_config(struct hisi_fb_data_type *hisifd,
	dss_wb_layer_t *layer, dss_rect_t aligned_rect, dss_rect_t *ov_block_rect, bool last_block);
void hisi_dss_hebce_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *wdma_base, dss_wdma_t *s_wdma);
int hisi_dss_hebcd_config(struct hisi_fb_data_type *hisifd, int ovl_idx, dss_layer_t *layer,
	struct hisi_ov_compose_rect *ov_compose_rect,
	struct hisi_ov_compose_flag *ov_compose_flag);
void hisi_dss_hebcd_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *dma_base, dss_rdma_t *s_dma);
int hisi_dss_wb_scl_config(struct hisi_fb_data_type *hisifd, dss_wb_layer_t *wb_layer);
int hisi_dss_post_scl_load_filter_coef(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *scl_lut_base, int coef_lut_idx);
int hisi_dss_post_scf_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisi_dss_csc_init(const char __iomem *csc_base, dss_csc_t *s_csc);
void hisi_dss_csc_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *csc_base, dss_csc_t *s_csc);
int hisi_dss_csc_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer,
	dss_wb_layer_t *wb_layer);
void hisi_dss_mctl_sys_init(const char __iomem *mctl_sys_base, dss_mctl_sys_t *s_mctl_sys);
int hisi_dss_sharpness_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer);
void hisi_dss_post_clip_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *post_clip_base, dss_post_clip_t *s_post_clip, int chn_idx);
int hisi_dss_ce_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer);
void hisi_dss_secure_layer_check_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisi_rch2_ce_end_handle_func(struct work_struct *work);
void hisi_rch4_ce_end_handle_func(struct work_struct *work);
void hisi_dss_dpp_acm_ce_end_handle_func(struct work_struct *work);
void hisi_sec_mctl_set_regs(struct hisi_fb_data_type *hisifd);
void hisi_drm_layer_online_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req_prev,
	dss_overlay_t *pov_req);
void hisi_drm_layer_online_clear(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req_prev,
	const int *seclist, int list_max);
void hisi_drm_layer_offline_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisi_drm_layer_offline_clear(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisi_dfr_notice_handle_func(struct work_struct *work);
void hisi_dss_mctl_ov_set_ctl_dbg_reg(struct hisi_fb_data_type *hisifd, char __iomem *mctl_base, bool enable_cmdlist);
uint32_t hisi_dss_mif_get_invalid_sel(dss_img_t *img, uint32_t transform, int v_scaling_factor,
	uint8_t is_tile, bool rdma_stretch_enable);
#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
int hisi_dss_arsr1p_write_lsc_gain(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *addr, const uint32_t **p, int row, int col);
int hisi_dss_arsr1p_write_coefs(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *addr, const int **p, int row, int col);
void hisi_dss_post_scf_init(const char __iomem *dss_base, const char __iomem *post_scf_base, dss_arsr1p_t *s_post_scf);
void hisi_dss_post_scf_set_reg(struct hisi_fb_data_type *hisifd, char __iomem *post_scf_base, dss_arsr1p_t *s_post_scf);
#endif

/* arsr2p interface */
void hisi_dss_arsr2p_init(const char __iomem *arsr2p_base, struct dss_arsr2p *s_arsr2p);  /* arsr2p init */
void hisi_dss_arsr2p_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *arsr2p_base, struct dss_arsr2p *s_arsr2p);   /* arsr2p set reg */
void hisi_dss_arsr2p_coef_on(struct hisi_fb_data_type *hisifd, bool enable_cmdlist);  /* lut coef */
int hisi_dss_arsr2p_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer,
	dss_rect_t *aligned_rect, bool rdma_stretch_enable);  /* arsr2p module config */
void hisi_remove_mctl_mutex(struct hisi_fb_data_type *hisifd, int mctl_idx, uint32_t cmdlist_idxs);
void hisi_dss_dpp_acm_gm_set_reg(struct hisi_fb_data_type *hisifd);

#if defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330)
void hisi_dss_post_scf_set_reg(struct hisi_fb_data_type *hisifd, char __iomem *post_scf_base, dss_scl_t *s_post_scf);
void hisi_dss_post_scf_init(char __iomem *dss_base, const char __iomem *post_scf_base, dss_scl_t *s_post_scf);
#endif
#ifndef CONFIG_HISI_FB_970
/* CONFIG_SH_AOD_ENABLE */
void clear_xcc_table(struct hisi_fb_data_type *hisifd);
void restore_xcc_table(struct hisi_fb_data_type *hisifd);
void hisifb_masklayer_backlight_notify_handler(struct work_struct *work);
#endif
#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
void hisi_dss_arsr_post_coef_on(struct hisi_fb_data_type *hisifd);
#endif

/* This api will pause send data to LCD which will freeze the display, be careful to use it */
int hisi_online_play_bypass(struct hisi_fb_data_type *hisifd, const void __user *argp);
bool hisi_online_play_bypass_set(struct hisi_fb_data_type *hisifd, int bypass);
bool hisi_online_play_bypass_check(struct hisi_fb_data_type *hisifd);
int hisi_get_release_and_retire_fence(struct hisi_fb_data_type *hisifd, void __user *argp);
void hisifb_mask_layer_backlight_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req_prev,
	dss_overlay_t *pov_req, bool *masklayer_maxbacklight_flag);
void hisifb_dc_backlight_config(struct hisi_fb_data_type *hisifd);
int hisifb_wait_for_mipi_resource_available(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);


#endif /* HISI_OVERLAY_UTILS_PLATFORM_H */
