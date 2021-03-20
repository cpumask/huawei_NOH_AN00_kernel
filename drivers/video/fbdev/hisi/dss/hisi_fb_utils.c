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

#include "hisi_fb.h"
#include "hisi_display_effect.h"
#include "hisi_dpe_utils.h"
#include "hisi_frame_rate_ctrl.h"
#include "hisi_mipi_dsi.h"
#include "product/rgb_stats/hisi_fb_rgb_stats.h"
#include "sensor_feima.h"

#if defined(CONFIG_HISI_PERIDVFS)
#include "peri_volt_poll.h"
#endif
#if defined(CONFIG_TEE_TUI)
#include "tui.h"
#endif

hisi_fb_fix_var_screeninfo_t g_hisi_fb_fix_var_screeninfo[HISI_FB_PIXEL_FORMAT_MAX] = {
	{0}, {0}, {0}, {0}, {0}, {0}, {0},
	/* for HISI_FB_PIXEL_FORMAT_BGR_565 */
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 5, 11, 0, 5, 6, 5, 0, 0, 0, 0, 0, 2},
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 4, 8, 0, 4, 4, 4, 0, 0, 0, 0, 0, 2},
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 4, 8, 12, 4, 4, 4, 4, 0, 0, 0, 0, 2},
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 5, 10, 0, 5, 5, 5, 0, 0, 0, 0, 0, 2},
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 5, 10, 15, 5, 5, 5, 1, 0, 0, 0, 0, 2},
	{0},
	/* HISI_FB_PIXEL_FORMAT_BGRA_8888 */
	{FB_TYPE_PACKED_PIXELS, 1, 1, FB_VMODE_NONINTERLACED, 0, 8, 16, 24, 8, 8, 8, 8, 0, 0, 0, 0, 4},
	{FB_TYPE_INTERLEAVED_PLANES, 2, 1, FB_VMODE_NONINTERLACED, 0, 5, 11, 0, 5, 6, 5, 0, 0, 0, 0, 0, 2},
	{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
	{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}
};

static bool check_panel_status_without_vr(struct hisi_fb_data_type *hisifd)
{
	if (hisifd_list[EXTERNAL_PANEL_IDX] && !hisifd_list[EXTERNAL_PANEL_IDX]->dp.dptx_vr) {
		if (hisifd->index != AUXILIARY_PANEL_IDX && !hisifd->panel_power_on) {
			HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
			return false;
		}
	}

	return true;
}

void set_reg(char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs)
{
	uint32_t mask = (1UL << bw) - 1UL;
	uint32_t temp = 0;

	temp = inp32(addr);
	temp &= ~(mask << bs);

	outp32(addr, temp | ((val & mask) << bs));

	if (g_debug_set_reg_val)
		HISI_FB_INFO("writel: [%pK] = 0x%x\n", addr, temp | ((val & mask) << bs));
}

uint32_t set_bits32(uint32_t old_val, uint32_t val, uint8_t bw, uint8_t bs)
{
	uint32_t mask = (1UL << bw) - 1UL;
	uint32_t tmp;

	tmp = old_val;
	tmp &= ~(mask << bs);

	return (tmp | ((val & mask) << bs));
}

void hisifb_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs)
{
	set_reg(addr, val, bw, bs);
}

bool is_fastboot_display_enable(void)
{
	return ((g_fastboot_enable_flag == 1) ? true : false);
}

bool is_dss_idle_enable(void)
{
	return ((g_enable_dss_idle == 1) ? true : false);
}

uint32_t get_panel_xres(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return 0;
	}

	return ((hisifd->resolution_rect.w > 0) ? hisifd->resolution_rect.w : hisifd->panel_info.xres);
}

uint32_t get_panel_yres(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return 0;
	}

	return ((hisifd->resolution_rect.h > 0) ? hisifd->resolution_rect.h : hisifd->panel_info.yres);
}

uint32_t hisifb_line_length(int index, uint32_t xres, int bpp)
{
	return ALIGN_UP(xres * (uint32_t)bpp, DMA_STRIDE_ALIGN);
}

void hisifb_get_timestamp(struct timeval *tv)
{
	struct timespec ts;

	ktime_get_ts(&ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / NSEC_PER_USEC;
}

uint32_t hisifb_timestamp_diff(struct timeval *lasttime, struct timeval *curtime)
{
	uint32_t ret;

	ret = (curtime->tv_usec >= lasttime->tv_usec) ?
		curtime->tv_usec - lasttime->tv_usec :
		1000000 - (lasttime->tv_usec - curtime->tv_usec);  /* 1s */

	return ret;
}

void hisifb_save_file(char *filename, const char *buf, uint32_t buf_len)
{
	ssize_t write_len = 0;
	struct file *fd = NULL;
	mm_segment_t old_fs;
	loff_t pos = 0;

	if (!filename) {
		HISI_FB_ERR("filename is NULL\n");
		return;
	}
	if (!buf) {
		HISI_FB_ERR("buf is NULL\n");
		return;
	}

	fd = filp_open(filename, O_CREAT | O_RDWR, 0644);
	if (IS_ERR(fd)) {
		HISI_FB_ERR("filp_open returned:filename %s, error %ld\n",
			filename, PTR_ERR(fd));
		return;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);  //lint !e501

	write_len = vfs_write(fd, (char __user *)buf, buf_len, &pos);

	pos = 0;
	set_fs(old_fs);
	filp_close(fd, NULL);
}

extern uint32_t g_fastboot_already_set;
int hisifb_ctrl_fastboot(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return -EINVAL;
	}

#if defined(CONFIG_HISI_DISPLAY_DFR)
	mipi_dsi_frm_rate_ctrl_init(hisifd);
#endif

	if (pdata->set_fastboot && !g_fastboot_already_set)
		ret = pdata->set_fastboot(hisifd->pdev);

	hisifb_vsync_resume(hisifd);

	hisi_overlay_on(hisifd, true);

	if (hisifd->panel_info.esd_enable)
		hrtimer_restart(&hisifd->esd_ctrl.esd_hrtimer);

	return ret;
}

int hisifb_ctrl_on(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);

	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return -EINVAL;
	}

#if defined(CONFIG_HISI_DISPLAY_DFR)
	mipi_dsi_frm_rate_ctrl_init(hisifd);
#endif

	if (pdata->on != NULL) {
		ret = pdata->on(hisifd->pdev);
		if (ret < 0) {
			HISI_FB_ERR("regulator/clk on fail\n");
			return ret;
		}
	}

	hisifb_vsync_resume(hisifd);


	hisi_overlay_on(hisifd, false);

	if (hisifd->panel_info.esd_enable)
		hrtimer_start(&hisifd->esd_ctrl.esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
			(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		hisifb_rgb_reg_enable(hisifd);
		hisifb_rgb_start_wq();
	}

	return ret;
}

int hisifb_ctrl_off(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return -EINVAL;
	}

	if (hisifd->panel_info.esd_enable)
		hrtimer_cancel(&hisifd->esd_ctrl.esd_hrtimer);

	hisifb_vsync_suspend(hisifd);

	hisi_overlay_off(hisifd);


	if (pdata->off != NULL)
		ret = pdata->off(hisifd->pdev);

	if (hisifd->index == PRIMARY_PANEL_IDX)
		hisifb_rgb_cancel_wq();

	return ret;
}

int hisifb_ctrl_lp(struct hisi_fb_data_type *hisifd, bool lp_enter)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return -EINVAL;
	}

	if (lp_enter) {
		hisi_overlay_off_lp(hisifd);

		if (pdata->lp_ctrl != NULL)
			ret = pdata->lp_ctrl(hisifd->pdev, lp_enter);
	} else {
		if (pdata->lp_ctrl != NULL)
			ret = pdata->lp_ctrl(hisifd->pdev, lp_enter);

		hisi_overlay_on_lp(hisifd);
	}

	return ret;
}

#define TIME_RANGE_TO_NEXT_VSYNC 3000000
#define DELAY_TIME_AFTER_TE 1000
#define DELAY_TIME_RANGE 500
static void hisi_esd_timing_ctrl(struct hisi_fb_data_type *hisifd)
{
	uint64_t vsync_period;
	ktime_t curr_time;
	uint64_t time_diff = 0;
	uint64_t delay_time = 0;
	uint32_t current_fps;

	if (!hisifd->panel_info.esd_timing_ctrl)
		return;

	current_fps = (hisifd->panel_info.fps == FPS_HIGH_60HZ) ? FPS_60HZ : hisifd->panel_info.fps;
	if (current_fps == 0) {
		HISI_FB_ERR("error fps %d\n", current_fps);
		return;
	}

	vsync_period = 1000000000UL / (uint64_t)current_fps;  /* convert to ns from s */
	curr_time = ktime_get();

	if (ktime_to_ns(curr_time) > ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp)) {
		time_diff = ktime_to_ns(curr_time) - ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp);
		if ((vsync_period > time_diff) &&
			((vsync_period - time_diff) < TIME_RANGE_TO_NEXT_VSYNC)) {
			delay_time = (vsync_period - time_diff) / 1000 +
				DELAY_TIME_AFTER_TE;  /* convert to us from ns */

			usleep_range(delay_time, delay_time + DELAY_TIME_RANGE);
			HISI_FB_INFO("vsync %llu ns, timediff %llu ns, delay %llu us",
				vsync_period, time_diff, delay_time);
		}
	}
}

static void hisi_ctrl_esd_exit(struct hisi_fb_data_type *hisifd)
{
	hisifd->esd_check_is_doing = 0;
	hisifb_vsync_disable_enter_idle(hisifd, false);
	hisifb_deactivate_vsync(hisifd);
}

int hisifb_ctrl_esd(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

#ifdef MIPI_RESOURCE_PROTECTION
	int wait_count = 0;
#endif

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	hisi_check_and_return(!pdata, -EINVAL, ERR, "pdata is NULL\n");

	down(&hisifd->power_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
		goto err_out;
	}

	if (!pdata->esd_handle) {
		HISI_FB_DEBUG("pdata->esd_handle is null\n");
		goto err_out;
	}

	hisifb_vsync_disable_enter_idle(hisifd, true);
	hisifb_activate_vsync(hisifd);

#ifdef MIPI_RESOURCE_PROTECTION
	if (is_lcd_dfr_support(hisifd) &&
		hisifd->panel_info.frm_rate_ctrl.status != FRM_UPDT_DONE) {
		HISI_FB_INFO("dfr status %d, try next time\n", hisifd->panel_info.frm_rate_ctrl.status);
		hisi_ctrl_esd_exit(hisifd);
		goto err_out;
	}
	hisifd->esd_check_is_doing = 1;

	/* wait when mipi_resource is available */
	while ((hisifd->mipi_dsi_bit_clk_update == 1) &&
		(wait_count < ESD_WAIT_MIPI_AVAILABLE_TIMEOUT)) {
		wait_count++;
		usleep_range(1000, 1500);
	}
	if (wait_count >= ESD_WAIT_MIPI_AVAILABLE_TIMEOUT) {
		HISI_FB_ERR("wait mipi_available timeout!\n");
		hisi_ctrl_esd_exit(hisifd);
		goto err_out;
	}

	HISI_FB_DEBUG("wait mipi_available %d times\n", wait_count);
#endif
	hisi_esd_timing_ctrl(hisifd);
	ret = pdata->esd_handle(hisifd->pdev);
	hisi_ctrl_esd_exit(hisifd);

err_out:
	up(&hisifd->power_sem);

	return ret;
}

int hisifb_fps_upt_isr_handler(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	int ret = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return -EINVAL;
	}

	if (!hisifd->panel_power_on) {
		HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
		goto err_out;
	}

	if (pdata->lcd_fps_updt_handle != NULL)
		ret = pdata->lcd_fps_updt_handle(hisifd->pdev);

	if (is_lcd_dfr_support(hisifd))
		send_lcd_freq_to_sensorhub(hisifd->panel_info.frm_rate_ctrl.target_frame_rate);

err_out:
	return ret;
}

int hisifb_ctrl_dss_voltage_get(struct fb_info *info, void __user *argp)
{
	int voltage_value = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	dss_vote_cmd_t dss_vote_cmd;

#ifdef SUPPORT_DCA_WITHOUT_VOLT
	return 0;
#else
	if (!info) {
		HISI_FB_ERR("dss voltage get info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("dss voltage get hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("dss voltage get argp NULL Pointer!\n");
		return -EINVAL;
	}

	if (hisifd->index == EXTERNAL_PANEL_IDX) {
		HISI_FB_ERR("fb%d, dss voltage get not supported!\n", hisifd->index);
		return -EINVAL;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (hisifd->core_clk_upt_support == 0) {
			HISI_FB_DEBUG("no support core_clk_upt\n");
			return 0;
		}
	}
	memset(&dss_vote_cmd, 0, sizeof(dss_vote_cmd_t));

	dss_get_peri_volt(&voltage_value);

	dss_vote_cmd.dss_voltage_level = dpe_get_voltage_level(voltage_value);

	if (copy_to_user(argp, &dss_vote_cmd, sizeof(dss_vote_cmd_t))) {
		HISI_FB_ERR("copy to user fail\n");
		return -EFAULT;
	}
	HISI_FB_DEBUG("fb%d, current_peri_voltage_level = %d\n", hisifd->index, dss_vote_cmd.dss_voltage_level);

	return 0;
#endif
}

int hisifb_ctrl_dss_voltage_set(struct fb_info *info, void __user *argp)
{
	int ret = 0;
	dss_vote_cmd_t dss_vote_cmd;
	int current_peri_voltage = 0;
	struct hisi_fb_data_type *hisifd = NULL;

#ifdef SUPPORT_DCA_WITHOUT_VOLT
	return 0;
#else
	hisi_check_and_return(!info, -EINVAL, ERR, "dss voltage set info NULL Pointer!\n");

	hisifd = (struct hisi_fb_data_type *)info->par;
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "dss voltage set hisifd NULL Pointer!\n");

	hisi_check_and_return(!argp, -EINVAL, ERR, "dss voltage set argp NULL Pointer!\n");

	hisi_check_and_return((hisifd->index == EXTERNAL_PANEL_IDX), -EINVAL, ERR,
		"fb%d, dss voltage get not supported!\n", hisifd->index);

	hisi_check_and_return((hisifd->index == MEDIACOMMON_PANEL_IDX), -EINVAL, ERR,
		"fb%d, dss voltage get not supported!\n", hisifd->index);

	if ((hisifd->index == PRIMARY_PANEL_IDX) && (hisifd->core_clk_upt_support == 0)) {
		HISI_FB_DEBUG("no support core_clk_upt\n");
		return ret;
	}

	if (hisifd_list[EXTERNAL_PANEL_IDX] && !hisifd_list[EXTERNAL_PANEL_IDX]->dp.dptx_vr) {
		if ((hisifd->index == AUXILIARY_PANEL_IDX) && (!hisifd_list[PRIMARY_PANEL_IDX]->panel_power_on)) {
			HISI_FB_INFO("fb%d, primary_panel is power off!\n", hisifd->index);
			return -EINVAL;
		}
	}

	down(&g_hisifb_dss_clk_vote_sem);

	ret = copy_from_user(&dss_vote_cmd, argp, sizeof(dss_vote_cmd_t));
	if (ret) {
		HISI_FB_ERR("copy_from_user failed!ret=%d!\n", ret);
		goto volt_vote_out;
	}

	if (dss_vote_cmd.dss_voltage_level == hisifd->dss_vote_cmd.dss_voltage_level) {
		HISI_FB_DEBUG("fb%d same voltage level %d\n", hisifd->index, dss_vote_cmd.dss_voltage_level);
		goto volt_vote_out;
	}

	ret = hisifb_set_dss_vote_voltage(hisifd, dss_vote_cmd.dss_voltage_level, &current_peri_voltage);
	if (ret < 0)
		goto volt_vote_out;

	dss_vote_cmd.dss_voltage_level = dpe_get_voltage_level(current_peri_voltage);

	if (copy_to_user(argp, &dss_vote_cmd, sizeof(dss_vote_cmd_t))) {
		HISI_FB_ERR("copy to user fail\n");
		ret = -EFAULT;
		goto volt_vote_out;
	}

volt_vote_out:
	up(&g_hisifb_dss_clk_vote_sem);
	return ret;
#endif
}

int hisifb_ctrl_dss_vote_cmd_set(struct fb_info *info, const void __user *argp)
{
	int ret = 0;
	bool status = true;
	struct hisi_fb_data_type *hisifd = NULL;
	dss_vote_cmd_t vote_cmd;

	if (!info) {
		HISI_FB_ERR("dss clk rate set info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("dss clk rate set hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("dss clk rate set argp NULL Pointer!\n");
		return -EINVAL;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (hisifd->core_clk_upt_support == 0) {
			HISI_FB_DEBUG("no support core_clk_upt\n");
			return ret;
		}
	}

	ret = copy_from_user(&vote_cmd, argp, sizeof(dss_vote_cmd_t));
	if (ret) {
		HISI_FB_ERR("copy_from_user failed!ret=%d", ret);
		return ret;
	}

	down(&hisifd->blank_sem);
#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	down(&hisifd->dp_vote_sem);
#endif

	down(&g_hisifb_dss_clk_vote_sem);

	status = check_panel_status_without_vr(hisifd);
	if (!status) {
		ret = -EPERM;
		goto err_out;
	}

	ret = set_dss_vote_cmd(hisifd, vote_cmd);

err_out:
	up(&g_hisifb_dss_clk_vote_sem);

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	up(&hisifd->dp_vote_sem);
#endif
	up(&hisifd->blank_sem);

	return ret;
}

int hisi_fb_blank_panel_power_on(struct hisi_fb_data_type *hisifd)
{
	int ret;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	ret = hisifd->on_fnc(hisifd);
	if (ret == 0)
		hisifd->panel_power_on = true;

	return ret;
}

int hisi_fb_blank_panel_power_off(struct hisi_fb_data_type *hisifd)
{
	int curr_pwr_state;
	int ret;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	curr_pwr_state = hisifd->panel_power_on;
	down(&hisifd->power_sem);
	hisifd->panel_power_on = false;
	hisifb_set_panel_power_status(hisifd, false);
	up(&hisifd->power_sem);

	hisifd->mask_layer_xcc_flag = 0;
	hisifd->hbm_is_opened = 0;

	if (hisifd->bl_cancel != NULL)
		hisifd->bl_cancel(hisifd);

	ret = hisifd->off_fnc(hisifd);
	if (ret)
		hisifd->panel_power_on = curr_pwr_state;

	return ret;

}

int hisi_fb_blank_update_tui_status(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	/* 1. if tui is running, dss should not powerdown,
	 *    because register will be writen in tui mode.
	 * 2. if tui is enable, but not running, then tui should not be ok,
	 *    send the msg that tui config fail.
	 */

	if (hisifd->fold_panel_display_change) {
		HISI_FB_INFO("It's in fold sence, don't update tui status");
		return 0;
	}

	if (hisifd->secure_ctrl.secure_status == DSS_SEC_RUNNING) {
		hisifd->secure_ctrl.secure_blank_flag = 1;
	#if defined(CONFIG_TEE_TUI)
		tui_poweroff_work_start();
	#endif
		HISI_FB_INFO("wait for tui quit\n");
		return -EINVAL;
	} else if (hisifd->secure_ctrl.secure_event == DSS_SEC_ENABLE) {
		hisifd->secure_ctrl.secure_event = DSS_SEC_DISABLE;
	#if defined(CONFIG_TEE_TUI)
		send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
	#endif
		HISI_FB_INFO("In power down, secure event will not be handled\n");
	}

	return 0;
}

int hisi_fb_blank_device(struct hisi_fb_data_type *hisifd, int blank_mode, struct fb_info *info)
{
	int ret = 0;

	hisi_check_and_return((!hisifd || !info), -EINVAL, ERR, "hisifd or info is NULL!\n");

	if (hisifd->dp_device_srs != NULL) {
		hisifd->dp_device_srs(hisifd, (blank_mode == FB_BLANK_UNBLANK) ? true : false);
	} else {
		ret = hisi_fb_blank_sub(blank_mode, info);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, blank_mode %d failed!\n", hisifd->index, blank_mode);
			return ret;
		}

		ret = hisifb_ce_service_blank(blank_mode, info);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, blank_mode %d hisifb_ce_service_blank() failed!\n",
				hisifd->index, blank_mode);
			return ret;
		}

		ret = hisifb_display_engine_blank(blank_mode, info);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, blank_mode %d hisifb_display_engine_blank() failed!\n",
				hisifd->index, blank_mode);
			return ret;
		}
	}

	return ret;
}

void hisi_fb_pm_runtime_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	hisifd->pm_runtime_register = NULL;
	hisifd->pm_runtime_unregister = NULL;
	hisifd->pm_runtime_get = NULL;
	hisifd->pm_runtime_put = NULL;
}

void hisi_fb_fnc_register_base(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	hisifd->set_fastboot_fnc = NULL;
	hisifd->open_sub_fnc = NULL;
	hisifd->release_sub_fnc = NULL;
	hisifd->hpd_open_sub_fnc = NULL;
	hisifd->hpd_release_sub_fnc = NULL;
	hisifd->lp_fnc = NULL;
	hisifd->esd_fnc = NULL;
	hisifd->sbl_ctrl_fnc = NULL;
	hisifd->fps_upt_isr_handler = NULL;
	hisifd->mipi_dsi_bit_clk_upt_isr_handler = NULL;
	hisifd->sysfs_attrs_add_fnc = NULL;
	hisifd->sysfs_attrs_append_fnc = NULL;
	hisifd->sysfs_create_fnc = NULL;
	hisifd->sysfs_remove_fnc = NULL;

	hisifd->pm_runtime_register = NULL;
	hisifd->pm_runtime_unregister = NULL;
	hisifd->pm_runtime_get = NULL;
	hisifd->pm_runtime_put = NULL;
	hisifd->bl_register = NULL;
	hisifd->bl_unregister = NULL;
	hisifd->bl_update = NULL;
	hisifd->bl_cancel = NULL;
	hisifd->vsync_register = NULL;
	hisifd->vsync_unregister = NULL;
	hisifd->vsync_ctrl_fnc = NULL;
	hisifd->vsync_isr_handler = NULL;
	hisifd->buf_sync_register = NULL;
	hisifd->buf_sync_unregister = NULL;
	hisifd->buf_sync_signal = NULL;
	hisifd->buf_sync_suspend = NULL;
	hisifd->secure_register = NULL;
	hisifd->secure_unregister = NULL;
	hisifd->esd_register = NULL;
	hisifd->esd_unregister = NULL;
	hisifd->debug_register = NULL;
	hisifd->debug_unregister = NULL;
	hisifd->video_idle_ctrl_register = NULL;
	hisifd->video_idle_ctrl_unregister = NULL;
	hisifd->pipe_clk_updt_isr_handler = NULL;
	hisifd->overlay_online_wb_register = NULL;
	hisifd->overlay_online_wb_unregister = NULL;
}

void hisi_fb_sdp_fnc_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->fb_mem_free_flag = true;

	hisifd->set_fastboot_fnc = NULL;
	hisifd->open_sub_fnc = NULL;
	hisifd->release_sub_fnc = NULL;
	hisifd->hpd_open_sub_fnc = hisi_fb_open_sub;
	hisifd->hpd_release_sub_fnc = hisi_fb_release_sub;
	hisifd->lp_fnc = NULL;
	hisifd->esd_fnc = NULL;
	hisifd->sbl_ctrl_fnc = NULL;
	hisifd->fps_upt_isr_handler = NULL;
	hisifd->mipi_dsi_bit_clk_upt_isr_handler = NULL;
	hisifd->sysfs_attrs_add_fnc = NULL;
	hisifd->sysfs_attrs_append_fnc = hisifb_sysfs_attrs_append;
	hisifd->sysfs_create_fnc = hisifb_sysfs_create;
	hisifd->sysfs_remove_fnc = hisifb_sysfs_remove;

	hisi_fb_pm_runtime_register(hisifd);

	hisifd->bl_register = hisifb_backlight_register;
	hisifd->bl_unregister = hisifb_backlight_unregister;
	hisifd->bl_update = hisifb_backlight_update;
	hisifd->bl_cancel = hisifb_backlight_cancel;
	hisifd->vsync_register = hisifb_vsync_register;
	hisifd->vsync_unregister = hisifb_vsync_unregister;
	hisifd->vsync_ctrl_fnc = hisifb_vsync_ctrl;
	hisifd->vsync_isr_handler = hisifb_vsync_isr_handler;
	hisifd->buf_sync_register = hisifb_buf_sync_register;
	hisifd->buf_sync_unregister = hisifb_buf_sync_unregister;
	hisifd->buf_sync_signal = hisifb_buf_sync_signal;
	hisifd->buf_sync_suspend = hisifb_buf_sync_suspend;
	hisifd->secure_register = hisifb_secure_register;
	hisifd->secure_unregister = hisifb_secure_unregister;
	hisifd->overlay_online_wb_register = NULL;
	hisifd->overlay_online_wb_unregister = NULL;
	hisifd->video_idle_ctrl_register = NULL;
	hisifd->video_idle_ctrl_unregister = NULL;
	hisifd->pipe_clk_updt_isr_handler = NULL;
	hisifd->esd_register = NULL;
	hisifd->esd_unregister = NULL;
	hisifd->debug_register = hisifb_debug_register;
	hisifd->debug_unregister = NULL;
}

void hisi_fb_mdc_fnc_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	mutex_init(&hisifd->work_lock);
	sema_init(&hisifd->media_common_sr_sem, 1);
	hisifd->media_common_composer_sr_refcount = 0;

	hisifd->fb_mem_free_flag = true;
	hisi_fb_fnc_register_base(hisifd);
	hisifd->buf_sync_register = hisifb_buf_sync_register;
	hisifd->buf_sync_unregister = hisifb_buf_sync_unregister;
}

void hisi_fb_aux_fnc_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	sema_init(&hisifd->offline_composer_sr_sem, 1);
	hisifd->offline_composer_sr_refcount = 0;

	hisifd->fb_mem_free_flag = true;

	hisi_fb_fnc_register_base(hisifd);
	hisifd->buf_sync_register = hisifb_buf_sync_register;
	hisifd->buf_sync_unregister = hisifb_buf_sync_unregister;
}

void hisi_fb_common_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->on_fnc = hisifb_ctrl_on;
	hisifd->off_fnc = hisifb_ctrl_off;

	/* debug register */
	if (hisifd->debug_register != NULL)
		hisifd->debug_register(hisifd->pdev);
	/* backlight register */
	if (hisifd->bl_register != NULL)
		hisifd->bl_register(hisifd->pdev);
	/* vsync register */
	if (hisifd->vsync_register != NULL)
		hisifd->vsync_register(hisifd->pdev);
	/* secure register */
	if (hisifd->secure_register != NULL)
		hisifd->secure_register(hisifd->pdev);
	/* buf_sync register */
	if (hisifd->buf_sync_register != NULL)
		hisifd->buf_sync_register(hisifd->pdev);
	/* pm runtime register */
	if (hisifd->pm_runtime_register != NULL)
		hisifd->pm_runtime_register(hisifd->pdev);
	/* fb sysfs create */
	if (hisifd->sysfs_create_fnc != NULL)
		hisifd->sysfs_create_fnc(hisifd->pdev);
	/* lcd check esd register */
	if (hisifd->esd_register != NULL)
		hisifd->esd_register(hisifd->pdev);
	/* register video idle crtl */
	if (hisifd->video_idle_ctrl_register != NULL)
		hisifd->video_idle_ctrl_register(hisifd->pdev);
	if (hisifd->overlay_online_wb_register != NULL)
		hisifd->overlay_online_wb_register(hisifd->pdev);
}

void hisi_fb_init_screeninfo_base(struct fb_fix_screeninfo *fix, struct fb_var_screeninfo *var)
{
	hisi_check_and_no_retval((!fix || !var), ERR, "fix or var is NULL\n");

	fix->type_aux = 0;
	fix->visual = FB_VISUAL_TRUECOLOR;
	fix->ywrapstep = 0;
	fix->mmio_start = 0;
	fix->mmio_len = 0;
	fix->accel = FB_ACCEL_NONE;

	var->xoffset = 0;
	var->yoffset = 0;
	var->grayscale = 0;
	var->nonstd = 0;
	var->activate = FB_ACTIVATE_VBL;
	var->accel_flags = 0;
	var->sync = 0;
	var->rotate = 0;
}

bool hisi_fb_img_type_valid(uint32_t fb_img_type)
{
	if ((fb_img_type == HISI_FB_PIXEL_FORMAT_BGR_565) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_BGRX_4444) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_BGRA_4444) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_BGRX_5551) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_BGRA_5551) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_BGRA_8888) ||
		(fb_img_type == HISI_FB_PIXEL_FORMAT_YUV_422_I))
		return true;

	return false;
}

void hisi_fb_init_sreeninfo_by_img_type(struct fb_fix_screeninfo *fix, struct fb_var_screeninfo *var,
	uint32_t fb_img_type, int *bpp)
{
	hisi_check_and_no_retval((!fix || !var), ERR, "fix or var is NULL\n");

	fix->type = g_hisi_fb_fix_var_screeninfo[fb_img_type].fix_type;
	fix->xpanstep = g_hisi_fb_fix_var_screeninfo[fb_img_type].fix_xpanstep;
	fix->ypanstep = g_hisi_fb_fix_var_screeninfo[fb_img_type].fix_ypanstep;
	var->vmode = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_vmode;

	var->blue.offset = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_blue_offset;
	var->green.offset = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_green_offset;
	var->red.offset = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_red_offset;
	var->transp.offset = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_transp_offset;

	var->blue.length = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_blue_length;
	var->green.length = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_green_length;
	var->red.length = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_red_length;
	var->transp.length = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_transp_length;

	var->blue.msb_right = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_blue_msb_right;
	var->green.msb_right = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_green_msb_right;
	var->red.msb_right = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_red_msb_right;
	var->transp.msb_right = g_hisi_fb_fix_var_screeninfo[fb_img_type].var_transp_msb_right;
	*bpp = g_hisi_fb_fix_var_screeninfo[fb_img_type].bpp;
}

void hisi_fb_init_sreeninfo_by_panel_info(struct fb_var_screeninfo *var, struct hisi_panel_info *panel_info,
	uint32_t fb_num, int bpp)
{
	uint32_t xres;
	uint32_t yres;
	hisi_check_and_no_retval((!var || !panel_info), ERR, "var or panel_info is NULL\n");

	if (panel_info->cascadeic_support)
		panel_info->dummy_pixel_x_left = panel_info->dummy_pixel_num / 2;

	var->xres = panel_info->xres - panel_info->dummy_pixel_num;
	var->yres = panel_info->yres;

	if (panel_info->product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		xres = panel_info->fb_xres;
		yres = panel_info->fb_yres;
	} else {
		xres = var->xres;
		yres = var->yres;
	}

	var->xres_virtual = xres;
	var->yres_virtual = yres * fb_num;
	var->bits_per_pixel = bpp * 8;

	var->pixclock = panel_info->pxl_clk_rate;
	var->left_margin = panel_info->ldi.h_back_porch;
	var->right_margin = panel_info->ldi.h_front_porch;
	var->upper_margin = panel_info->ldi.v_back_porch;
	var->lower_margin = panel_info->ldi.v_front_porch;
	var->hsync_len = panel_info->ldi.h_pulse_width;
	var->vsync_len = panel_info->ldi.v_pulse_width;
	var->height = panel_info->height;
	var->width = panel_info->width;

}

void hisi_fb_data_init(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->ref_cnt = 0;
	hisifd->panel_power_on = false;
	hisifd->aod_function = 0;
	hisifd->aod_mode = 0;
	hisifd->vr_mode = 0;
	hisifd->mask_layer_xcc_flag = 0;
	hisifd->ud_fp_scene = 0;
	hisifd->ud_fp_hbm_level = UD_FP_HBM_LEVEL;
	hisifd->ud_fp_current_level = UD_FP_CURRENT_LEVEL;
	hisifd->enable_fast_unblank = false;
	hisifd->fb_pan_display = false;
	hisifd->is_idle_display = false;
	hisifd->fb2_irq_on_flag = false;
	hisifd->fb2_irq_force_on_flag = false;
	hisifd->dfr_create_singlethread = false;
}

void hisi_fb_init_sema(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	sema_init(&hisifd->blank_sem, 1);
	sema_init(&hisifd->blank_sem0, 1);
	sema_init(&hisifd->blank_sem_effect, 1);
	sema_init(&hisifd->blank_sem_effect_hiace, 1);
	sema_init(&hisifd->blank_sem_effect_gmp, 1);
	sema_init(&hisifd->effect_gmp_sem, 1);
	sema_init(&hisifd->brightness_esd_sem, 1);
	sema_init(&hisifd->blank_aod_sem, 1);
	sema_init(&hisifd->power_sem, 1);
	sema_init(&hisifd->fast_unblank_sem, 1);
	sema_init(&hisifd->hiace_hist_lock_sem, 1);
	sema_init(&hisifd->dp_vote_sem, 1);
	sema_init(&hisifd->esd_panel_change_sem, 1);
}

void hisi_fb_init_spin_lock(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

#ifdef MIPI_RESOURCE_PROTECTION
	spin_lock_init(&hisifd->mipi_resource_lock);
#endif
	spin_lock_init(&hisifd->underflow_lock);
}

int hisi_fb_registe_callback(struct hisi_fb_data_type *hisifd, struct fb_var_screeninfo *var,
	struct hisi_panel_info *panel_info)
{
	hisi_check_and_return((!hisifd || !var || !panel_info), -EINVAL,
		ERR, "hisifd, var or panel_info is NULL\n");

	hisifd->dumpDss = NULL;

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		hisi_fb_offlinecomposer_init(var, panel_info);
		hisi_fb_pdp_fnc_register(hisifd);
		if (hisifb_check_ldi_porch(panel_info)) {
			HISI_FB_ERR("check ldi porch failed, return!\n");
			return -EINVAL;
		}

		if (panel_info->product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
			hisifd->panel_power_status = EN_INNER_OUTER_PANEL_ON;
			HISI_FB_INFO("[fold]panel_power_status = %d\n", hisifd->panel_power_status);
		}
	} else if (hisifd->index == EXTERNAL_PANEL_IDX) {
		hisi_fb_sdp_fnc_register(hisifd);
	} else if (hisifd->index == MEDIACOMMON_PANEL_IDX) {
		hisi_fb_mdc_fnc_register(hisifd);
	} else {
		hisi_fb_aux_fnc_register(hisifd);
	}

	return 0;
}

#pragma GCC diagnostic pop
