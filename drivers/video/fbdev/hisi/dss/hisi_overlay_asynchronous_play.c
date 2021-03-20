/* Copyright (c) 2019-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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
#pragma GCC diagnostic ignored "-Winteger-overflow"

#include "overlay/hisi_overlay_utils.h"
#include "hisi_dpe_utils.h"
#include "hisi_mmbuf_manager.h"
#include "product/rgb_stats/hisi_fb_rgb_stats.h"

struct hisi_ov_asynch_play_param      {
	uint32_t cmdlist_pre_idxs;
	uint32_t cmdlist_idxs;
	int enable_cmdlist;
	int need_skip;
	uint32_t timediff;
	bool masklayer_maxbacklight_flag;
	struct timeval tv0;
	struct timeval tv1;
	struct timeval returntv0;
};

#if CONFIG_SH_AOD_ENABLE
extern bool hisi_aod_get_aod_status(void);
#endif

#define MAX_WAIT_ASYNC_TIMES 25


#define hisi_set_async_vactive0_start_flag(hisifd)

int hisi_get_release_and_retire_fence(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	int ret = 0;
	int times = 0;
	uint32_t timeout_interval = g_debug_wait_asy_vactive0_thr;
	struct dss_fence fence_fd;
	struct timeval tv0;
	struct timeval tv1;

	hisi_check_and_return(((!hisifd) || (!argp)), -EINVAL, ERR, "NULL Pointer!\n");

	hisi_check_and_return(!hisifd->panel_power_on, -EINVAL, ERR, "fb%d, panel is power off!\n", hisifd->index);

	fence_fd.release_fence = -1;
	fence_fd.retire_fence = -1;

	if (hisi_dss_is_async_play(hisifd)) {
		hisi_set_async_vactive0_start_flag(hisifd);

		hisifb_get_timestamp(&tv0);
		if (hisifd->asynchronous_vactive0_start_flag == 0) {
			while (1) {
				ret = wait_event_interruptible_timeout(hisifd->asynchronous_play_wq,
						hisifd->asynchronous_vactive0_start_flag,
						msecs_to_jiffies(timeout_interval));
				if ((ret == -ERESTARTSYS) && (times++ < MAX_WAIT_ASYNC_TIMES))
					mdelay(2);  /* ms */
				else
					break;
			}

			if (ret <= 0) {
				hisifb_get_timestamp(&tv1);
				/* resync fence with hwcomposer */
				hisifb_buf_sync_suspend(hisifd);
				ret = -EFAULT;
				goto err_return;
			}
		}
		hisifd->asynchronous_vactive0_start_flag = 0;
	}

	ret = hisifb_buf_sync_create_fence(hisifd, &fence_fd.release_fence, &fence_fd.retire_fence);
	if (ret != 0)
		HISI_FB_INFO("fb%d, hisi_create_fence failed!\n", hisifd->index);

	if (copy_to_user((struct dss_fence __user *)argp, &fence_fd, sizeof(struct dss_fence))) {
		HISI_FB_ERR("fb%d, copy_to_user failed.\n", hisifd->index);
		hisifb_buf_sync_close_fence(&fence_fd.release_fence, &fence_fd.retire_fence);
		ret = -EFAULT;
	}

	return ret;
err_return:
	if (is_mipi_cmd_panel(hisifd))
		hisifd->asynchronous_vactive0_start_flag = 1;

	return ret;
}

static void hisi_free_reserve_buffer(struct hisi_fb_data_type *hisifd,
	int free_buffer_refcount)
{
	struct hisi_fb_data_type *external_fb = hisifd_list[EXTERNAL_PANEL_IDX];

	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer!\n");
		return;
	}

	if ((hisifd->index == PRIMARY_PANEL_IDX) && (free_buffer_refcount > 1)) {
		if (!hisifd->fb_mem_free_flag) {
			hisifb_free_fb_buffer(hisifd);
			hisifd->fb_mem_free_flag = true;
		}

		if (g_logo_buffer_base && g_logo_buffer_size) {
			hisifb_free_logo_buffer(hisifd);
			HISI_FB_INFO("dss_free_buffer_refcount = %d!\n", free_buffer_refcount);
		}

		/* wakeup DP when android system has been startup */
		if (external_fb) {
			external_fb->dp.dptx_gate = true;
			if (external_fb->dp_wakeup)
				external_fb->dp_wakeup(external_fb);
		}
	}
}

static void hisi_preserve_ov_req(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	int enable_cmdlist, uint32_t cmdlist_idxs)
{
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer!\n");
		return;
	}

	if (g_debug_ovl_cmdlist && enable_cmdlist)
		hisi_cmdlist_dump_all_node(hisifd, NULL, cmdlist_idxs);

	if ((pov_req->ov_block_nums < 0) || (pov_req->ov_block_nums > HISI_DSS_OV_BLOCK_NUMS)) {
		HISI_FB_ERR("ov_block_nums[%d] is out of range!\n", pov_req->ov_block_nums);
		return;
	}

	hisifd->frame_count++;
	memcpy(&hisifd->ov_req_prev_prev, &hisifd->ov_req_prev, sizeof(dss_overlay_t));
	memcpy(hisifd->ov_block_infos_prev_prev, hisifd->ov_block_infos_prev,
		hisifd->ov_req_prev.ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(hisifd->ov_block_infos_prev_prev);

	memcpy(&hisifd->ov_req_prev, pov_req, sizeof(dss_overlay_t));
	memcpy(hisifd->ov_block_infos_prev, hisifd->ov_block_infos,
		pov_req->ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(hisifd->ov_block_infos_prev);
	hisifd->vsync_ctrl.vsync_timestamp_prev = hisifd->vsync_ctrl.vsync_timestamp;
}

static int ov_composer_first_block_process(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, bool *vsync_time_checked, bool first_block)
{
	int ret;

	ret = hisi_dss_dirty_region_dbuf_config(hisifd, pov_req);
	hisi_check_and_return((ret != 0), -EINVAL, ERR,
		"fb%d, dirty_region_dbuf_config failed! ret = %d\n", hisifd->index, ret);

	ret = hisi_dss_post_scf_config(hisifd, pov_req);
	hisi_check_and_return((ret != 0), -EINVAL, ERR,
		"fb%d, post_scf_config failed! ret = %d\n", hisifd->index, ret);

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
	ret = hisi_effect_arsr1p_config(hisifd, pov_req);
	hisi_check_and_return((ret != 0), -EINVAL, ERR,
		"fb%d, effect_arsr1p_config failed! ret = %d\n", hisifd->index, ret);

	ret = hisi_effect_hiace_config(hisifd);
	hisi_check_and_return((ret != 0), -EINVAL, ERR,
		"fb%d, effect_hiace_config failed! ret = %d\n", hisifd->index, ret);
#endif

	if (is_mipi_video_panel(hisifd) || is_dp_panel(hisifd)) {
		*vsync_time_checked = true;
		if (hisifb_wait_for_mipi_resource_available(hisifd, pov_req) < 0)
			return -EINVAL;
		if (hisi_dss_check_vsync_timediff(hisifd, pov_req) < 0)
			return -EINVAL;
	}

	return ret;
}

static int hisi_ov_layers_composer(struct hisi_fb_data_type *hisifd, dss_overlay_block_t *pov_h_block,
	struct hisi_ov_compose_rect *ov_compose_rect, struct hisi_ov_compose_flag *ov_compose_flag)
{
	uint32_t i;
	int ret;
	dss_layer_t *layer = NULL;

	for (i = 0; i < pov_h_block->layer_nums; i++) {
		layer = &(pov_h_block->layer_infos[i]);
		memset(ov_compose_rect->clip_rect, 0, sizeof(dss_rect_ltrb_t));
		memset(ov_compose_rect->aligned_rect, 0, sizeof(dss_rect_t));
		ov_compose_flag->rdma_stretch_enable = false;

		ret = hisi_ov_compose_handler(hisifd, pov_h_block, layer, ov_compose_rect, ov_compose_flag);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, ov_compose failed! ret = %d\n", hisifd->index, ret);
	}

	return 0;
}

static int hisi_set_ov_composer_info(struct hisi_fb_data_type *hisifd, int enable_cmdlist, bool *vsync_time_checked)
{
	uint32_t m;
	int ret;
	dss_rect_t aligned_rect;
	dss_rect_ltrb_t clip_rect;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_overlay_t *pov_req = &(hisifd->ov_req);
	struct hisi_ov_compose_rect ov_compose_rect = { NULL, NULL, &clip_rect, &aligned_rect };
	struct hisi_ov_compose_flag ov_compose_flag = { false, false, true, enable_cmdlist };
	struct ov_module_set_regs_flag ov_module_flag = { enable_cmdlist, false, 0, 0 };

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req->ov_block_infos_ptr);
	for (m = 0; m < pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);

		ret = hisi_dss_module_init(hisifd);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, module_init failed! ret = %d\n", hisifd->index, ret);

		hisi_dss_aif_handler(hisifd, pov_req, pov_h_block);
		ret = hisi_dss_ovl_base_config(hisifd, pov_req, pov_h_block, NULL, m);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, ovl_base_config failed! ret = %d\n", hisifd->index, ret);

#if VIDEO_IDLE_GPU_COMPOSE_ENABLE
		if (m == 0)
			(void)hisifb_video_panel_idle_display_ctrl(hisifd, pov_req->video_idle_status);
#endif

		/* Go through all layers */
		ret = hisi_ov_layers_composer(hisifd, pov_h_block, &ov_compose_rect, &ov_compose_flag);
		if (ret != 0)
			return -EINVAL;

		ret = hisi_dss_mctl_ov_config(hisifd, pov_req, pov_req->ovl_idx, ov_compose_flag.has_base, (m == 0));
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, mctl_config failed! ret = %d\n", hisifd->index, ret);

		ret = ov_composer_first_block_process(hisifd, pov_req, vsync_time_checked, (m == 0));
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, first_block_process failed! ret = %d\n", hisifd->index, ret);

		ov_module_flag.is_first_ov_block = (m == 0);
		ret = hisi_dss_ov_module_set_regs(hisifd, pov_req, pov_req->ovl_idx, ov_module_flag);
		hisi_check_and_return((ret != 0), -EINVAL, ERR,
			"fb%d, ov_module_set_regs failed! ret = %d\n", hisifd->index, ret);
	}

	return 0;
}

static int hisi_wait_vactive0_start_flag(struct hisi_fb_data_type *hisifd,
	const void __user *argp, int free_buffer_refcount, dss_overlay_t *pov_req, int *need_skip)
{
	uint32_t timediff;
	struct timeval tv1;
	struct timeval tv2;
	int ret;
	int wait_1vsync = 0;
	unsigned long flags = 0;

	if (g_debug_ovl_online_composer_timediff & 0x4)
		hisifb_get_timestamp(&tv1);

	ret = hisi_dss_get_ov_data_from_user(hisifd, pov_req, argp);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_get_ov_data_from_user failed! ret=%d\n", hisifd->index, ret);
		*need_skip = 1;
		return -EINVAL;
	}

	if ((is_mipi_video_panel(hisifd)) && (hisifd->online_play_count == 1)) {
		wait_1vsync = 1;
		HISI_FB_INFO("video panel wait a vsync when first frame displayed!\n");
	}

	if (is_mipi_cmd_panel(hisifd) || (free_buffer_refcount < 2) || wait_1vsync) {
		ret = hisi_vactive0_start_config(hisifd, pov_req);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_vactive0_start_config failed! ret=%d\n", hisifd->index, ret);
			*need_skip = 1;
			return -EINVAL;
		}

		if (is_mipi_cmd_panel(hisifd)) {
			spin_lock_irqsave(&hisifd->buf_sync_ctrl.refresh_lock, flags);
			hisifd->buf_sync_ctrl.refresh++;
			spin_unlock_irqrestore(&hisifd->buf_sync_ctrl.refresh_lock, flags);
		}
	}

	if (g_debug_ovl_online_composer_timediff & 0x4) {
		hisifb_get_timestamp(&tv2);
		timediff = hisifb_timestamp_diff(&tv1, &tv2);
		if (timediff >= g_debug_ovl_online_composer_time_threshold)
			HISI_FB_INFO("ONLINE_VACTIVE_TIMEDIFF is %u us!\n", timediff);
	}

	return 0;
}

static int hisi_ov_async_play_early_check(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	hisi_check_and_return(!argp, -EINVAL, ERR, "argp is NULL\n");

#if CONFIG_SH_AOD_ENABLE
	if ((hisifd->index == EXTERNAL_PANEL_IDX) && hisi_aod_get_aod_status()) {
		HISI_FB_INFO("In aod mode!\n");
		return 0;
	}
#endif

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		hisifd->backlight.bl_updated = 0;
		return 0;
	}

	if (g_debug_ldi_underflow) {
		if (g_err_status & (DSS_PDP_LDI_UNDERFLOW | DSS_SDP_LDI_UNDERFLOW)) {
			mdelay(HISI_DSS_COMPOSER_HOLD_TIME);
			return 0;
		}
	}

	if (g_debug_ovl_online_composer_return)
		return 0;

	return 1;
}

static int hisi_ov_async_play_early_prepare(struct hisi_fb_data_type *hisifd, void __user *argp,
	struct list_head *lock_list, struct hisi_ov_asynch_play_param *asynch_play_param)
{
	dss_overlay_t *pov_req = &(hisifd->ov_req);
	dss_overlay_t *pov_req_prev = &(hisifd->ov_req_prev);
	int ret;

	ret = hisifb_layerbuf_lock(hisifd, pov_req, lock_list);
	hisi_check_and_return(ret, ret, ERR, "fb%d, hisifb_layerbuf_lock failed! ret=%d\n", hisifd->index, ret);

	hisi_dss_handle_cur_ovl_req(hisifd, pov_req);

	ret = hisi_dss_module_init(hisifd);
	hisi_check_and_return(ret, ret, ERR, "fb%d, hisi_dss_module_init failed! ret=%d\n", hisifd->index, ret);

	hisi_mmbuf_info_get_online(hisifd);

	if (asynch_play_param->enable_cmdlist) {
		hisifd->set_reg = hisi_cmdlist_set_reg;

		hisi_cmdlist_data_get_online(hisifd);

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req_prev, &asynch_play_param->cmdlist_pre_idxs, NULL);
		hisi_check_and_return(ret, ret, ERR,
			"fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req_prev failed! ret=%d\n", hisifd->index, ret);

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req, &asynch_play_param->cmdlist_pre_idxs,
			&asynch_play_param->cmdlist_idxs);
		hisi_check_and_return(ret, ret, ERR, "fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req failed! ret=%d\n",
			hisifd->index, ret);

		hisi_cmdlist_add_nop_node(hisifd, asynch_play_param->cmdlist_pre_idxs, 0, 0);
		hisi_cmdlist_add_nop_node(hisifd, asynch_play_param->cmdlist_idxs, 0, 0);
	} else {
		hisifd->set_reg = hisifb_set_reg;
		hisi_dss_mctl_mutex_lock(hisifd, pov_req->ovl_idx);
		asynch_play_param->cmdlist_pre_idxs = ~0;
	}

	return 0;
}

static int hisi_ov_async_play_set_reg(struct hisi_fb_data_type *hisifd,
	struct hisi_ov_asynch_play_param *asynch_play_param)
{
	dss_overlay_t *pov_req = &(hisifd->ov_req);
	dss_overlay_t *pov_req_prev = &(hisifd->ov_req_prev);
	bool vsync_time_checked = false;
	unsigned long flags = 0;
	int ret;

	hisi_dss_prev_module_set_regs(hisifd, pov_req_prev, asynch_play_param->cmdlist_pre_idxs,
		asynch_play_param->enable_cmdlist, NULL);

	ret = hisi_set_ov_composer_info(hisifd, asynch_play_param->enable_cmdlist, &vsync_time_checked);
	if (ret)
		return ret;

	hisi_sec_mctl_set_regs(hisifd);

	if (asynch_play_param->enable_cmdlist) {
		g_online_cmdlist_idxs |= asynch_play_param->cmdlist_idxs;
		/* add taskend for share channel */
		hisi_cmdlist_add_nop_node(hisifd, asynch_play_param->cmdlist_idxs, 0, 0);

		/* remove ch cmdlist */
		hisi_cmdlist_config_stop(hisifd, asynch_play_param->cmdlist_pre_idxs);

		asynch_play_param->cmdlist_idxs |= asynch_play_param->cmdlist_pre_idxs;
		hisi_cmdlist_flush_cache(hisifd, asynch_play_param->cmdlist_idxs);
	}

	if (!g_fake_lcd_flag)
		hisi_dss_unflow_handler(hisifd, pov_req, true);

	if (!vsync_time_checked) {
		ret = hisi_dss_check_vsync_timediff(hisifd, pov_req);
		if (ret)
			return ret;
	}

	if (!is_mipi_cmd_panel(hisifd)) {
		spin_lock_irqsave(&hisifd->buf_sync_ctrl.refresh_lock, flags);
		hisifd->buf_sync_ctrl.refresh++;
		spin_unlock_irqrestore(&hisifd->buf_sync_ctrl.refresh_lock, flags);
	}

	if (is_mipi_cmd_panel(hisifd))
		(void)hisifb_wait_for_mipi_resource_available(hisifd, pov_req);

	hisifb_mask_layer_backlight_config(hisifd, pov_req_prev, pov_req,
		&asynch_play_param->masklayer_maxbacklight_flag);

	if (asynch_play_param->enable_cmdlist) {
		ret = hisi_cmdlist_config_start(hisifd, pov_req->ovl_idx, asynch_play_param->cmdlist_idxs, 0);
		if (ret)
			return ret;
	} else {
		hisi_dss_mctl_mutex_unlock(hisifd, pov_req->ovl_idx);
	}

	if (hisifd->panel_info.dirty_region_updt_support)
		hisi_dss_dirty_region_updt_config(hisifd, pov_req);

	/* cpu config drm layer */
	hisi_drm_layer_online_config(hisifd, pov_req_prev, pov_req);

	hisifb_dc_backlight_config(hisifd);

	return 0;
}

static int hisi_ov_async_play_finish(struct hisi_fb_data_type *hisifd, void __user *argp,
	int *free_buffer_refcount, struct list_head *lock_list, struct hisi_ov_asynch_play_param *asynch_play_param)
{
	uint64_t ov_block_infos_ptr;
	dss_overlay_t *pov_req = &(hisifd->ov_req);
	struct timeval returntv1;

	if ((hisifd->index == PRIMARY_PANEL_IDX) && (hisifd->online_play_count > 1))
		hisifb_rgb_read_register(hisifd);

	hisi_vactive0_end_isr_handler(hisifd);

	single_frame_update(hisifd);

	hisifb_frame_updated(hisifd);

	ov_block_infos_ptr = pov_req->ov_block_infos_ptr;
	pov_req->ov_block_infos_ptr = (uint64_t)0;  /* clear ov_block_infos_ptr */

	hisifb_get_timestamp(&returntv1);
	pov_req->online_wait_timediff = hisifb_timestamp_diff(&asynch_play_param->returntv0, &returntv1);

	if (copy_to_user((struct dss_overlay_t __user *)argp, pov_req, sizeof(dss_overlay_t))) {
		HISI_FB_ERR("fb%d, copy_to_user failed.\n", hisifd->index);
		return -EFAULT;
	}

	if (hisifd->ov_req_prev.ov_block_nums > HISI_DSS_OV_BLOCK_NUMS) {
		HISI_FB_ERR("ov_block_nums %d is out of range!\n", hisifd->ov_req_prev.ov_block_nums);
		return -EFAULT;
	}

	pov_req->ov_block_infos_ptr = ov_block_infos_ptr;

	hisifd->enter_idle = false;
	hisifd->emi_protect_check_count = 0;
	hisifb_deactivate_vsync(hisifd);

	hisifb_layerbuf_flush(hisifd, lock_list);

	hisifb_masklayer_backlight_flag_config(hisifd, asynch_play_param->masklayer_maxbacklight_flag);

	hisi_free_reserve_buffer(hisifd, *free_buffer_refcount);
	(*free_buffer_refcount)++;

	hisi_preserve_ov_req(hisifd, pov_req, asynch_play_param->enable_cmdlist, asynch_play_param->cmdlist_idxs);

	return 0;
}

int hisi_ov_asynchronous_play(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	static int free_buffer_refcount;
	dss_overlay_t *pov_req = NULL;
	int ret;
	struct list_head lock_list;
	struct hisi_ov_asynch_play_param asynch_play_param = {0};

	ret = hisi_ov_async_play_early_check(hisifd, argp);
	if (ret != 1)
		return ret;

	pov_req = &(hisifd->ov_req);
	INIT_LIST_HEAD(&lock_list);

	if (g_debug_ovl_online_composer_timediff & 0x2)
		hisifb_get_timestamp(&asynch_play_param.tv0);

	hisifb_get_timestamp(&asynch_play_param.returntv0);

	asynch_play_param.enable_cmdlist = g_enable_ovl_cmdlist_online;
	if ((hisifd->index == EXTERNAL_PANEL_IDX) && hisifd->panel_info.fake_external)
		asynch_play_param.enable_cmdlist = 0;

	hisifb_activate_vsync(hisifd);

	hisifb_snd_cmd_before_frame(hisifd);

	if (hisi_wait_vactive0_start_flag(hisifd, argp, free_buffer_refcount, pov_req, &asynch_play_param.need_skip))
		goto err_return;

	down(&hisifd->blank_sem0);

	if (hisi_ov_async_play_early_prepare(hisifd, argp, &lock_list, &asynch_play_param))
		goto err_return;

	if (hisi_ov_async_play_set_reg(hisifd, &asynch_play_param))
		goto err_return;

	if (hisi_ov_async_play_finish(hisifd, argp, &free_buffer_refcount, &lock_list, &asynch_play_param))
		goto err_return;

	if (g_debug_ovl_online_composer_timediff & 0x2) {
		hisifb_get_timestamp(&asynch_play_param.tv1);
		asynch_play_param.timediff = hisifb_timestamp_diff(&asynch_play_param.tv0, &asynch_play_param.tv1);
		if (asynch_play_param.timediff >= g_debug_ovl_online_composer_time_threshold)
			HISI_FB_INFO("ONLINE_TIMEDIFF is %u us!\n", asynch_play_param.timediff);
	}
	up(&hisifd->blank_sem0);

	return 0;

err_return:
	if (is_mipi_cmd_panel(hisifd))
		hisifd->vactive0_start_flag = 1;

	hisifb_layerbuf_lock_exception(hisifd, &lock_list);
	hisifb_deactivate_vsync(hisifd);
	if (!asynch_play_param.need_skip)
		up(&hisifd->blank_sem0);

	return ret;
}

#pragma GCC diagnostic pop

