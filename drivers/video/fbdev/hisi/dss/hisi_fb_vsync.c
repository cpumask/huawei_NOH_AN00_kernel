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
#pragma GCC diagnostic ignored "-Wformat"

#include "hisi_fb.h"

#if defined(CONFIG_SH_AOD_ENABLE) && !defined(CONFIG_HISI_FB_970) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_FINGERPRINT)
#include <huawei_platform/fingerprint_interface/fingerprint_interface.h>
#endif

/* /sys/class/graphics/fb0/vsync_event */
#define VSYNC_CTRL_EXPIRE_COUNT 4
#define MASKLAYER_BACKLIGHT_WAIT_VSYNC_COUNT 2

struct dss_vote_cmd g_vote_cmd;

void hisifb_masklayer_backlight_flag_config(struct hisi_fb_data_type *hisifd,
	bool masklayer_backlight_flag)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (masklayer_backlight_flag == true)
			hisifd->masklayer_maxbacklight_flag = true;
	}
}

static void hisifb_masklayer_backlight_notify(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (hisifd->masklayer_backlight_notify_wq)
			queue_work(hisifd->masklayer_backlight_notify_wq, &hisifd->masklayer_backlight_notify_work);
	}
}

static bool is_primary_panel_at_diff_soc(struct hisi_fb_data_type *hisifd)
{
	return ((hisifd->index == PRIMARY_PANEL_IDX) &&
			(g_dss_version_tag != FB_ACCEL_KIRIN970));
}

void hisifb_masklayer_backlight_notify_handler(struct work_struct *work)
{
	static uint32_t vsync_count;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!work) {
		HISI_FB_ERR("work is NULL.\n");
		return;
	}

	hisifd = container_of(work, struct hisi_fb_data_type, masklayer_backlight_notify_work);
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL.\n");
		return;
	}

	if (is_primary_panel_at_diff_soc(hisifd)) {
		if (g_debug_online_vsync)
			HISI_FB_DEBUG("flag=%d, vsync_count=%d.\n", hisifd->masklayer_maxbacklight_flag, vsync_count);

		if (hisifd->masklayer_maxbacklight_flag == true) {
			vsync_count += 1;
			if (vsync_count == MASKLAYER_BACKLIGHT_WAIT_VSYNC_COUNT) {
				HISI_FB_INFO("mask layer max backlight done notify.\n");
				vsync_count = 0;
				hisifd->masklayer_maxbacklight_flag = false;
#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V330) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V350) || defined(CONFIG_FINGERPRINT)
				ud_fp_on_hbm_completed();
#endif
			}
		} else {
			vsync_count = 0;
		}
	}
}

void hisifb_frame_updated(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (hisifd->vsync_ctrl.vsync_report_fnc)
		atomic_inc(&(hisifd->vsync_ctrl.buffer_updated));
}

void hisifb_vsync_isr_handler(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_vsync *vsync_ctrl = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	int buffer_updated = 0;
	ktime_t pre_vsync_timestamp;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	vsync_ctrl = &(hisifd->vsync_ctrl);
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	hisi_check_and_no_retval(!pdata, ERR, "pdata is NULL\n");

	pre_vsync_timestamp = vsync_ctrl->vsync_timestamp;
	vsync_ctrl->vsync_timestamp = ktime_get();

	hisifb_masklayer_backlight_notify(hisifd);

	if (hisifd->vsync_ctrl.vsync_enabled)
		wake_up_interruptible_all(&(vsync_ctrl->vsync_wait));

	if (hisifd->panel_info.vsync_ctrl_type != VSYNC_CTRL_NONE) {
		spin_lock(&vsync_ctrl->spin_lock);
		if (vsync_ctrl->vsync_ctrl_expire_count) {
			vsync_ctrl->vsync_ctrl_expire_count--;

			if (vsync_ctrl->vsync_ctrl_expire_count == 0)
				schedule_work(&vsync_ctrl->vsync_ctrl_work);
		}
		spin_unlock(&vsync_ctrl->spin_lock);
	}

	if (vsync_ctrl->vsync_report_fnc) {
		if (hisifd->vsync_ctrl.vsync_enabled)
			buffer_updated = atomic_dec_return(&(vsync_ctrl->buffer_updated));
		else
			buffer_updated = 1;

		if (buffer_updated < 0)
			atomic_cmpxchg(&(vsync_ctrl->buffer_updated), buffer_updated, 1);  /*lint !e571*/
		else if (is_mipi_video_panel(hisifd))
			vsync_ctrl->vsync_report_fnc(0);
		else
			vsync_ctrl->vsync_report_fnc(buffer_updated);
	}

	if (g_debug_online_vsync)
		HISI_FB_INFO("fb%d, VSYNC=%llu, time_diff=%llu.\n", hisifd->index,
			ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp),
			(ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp) - ktime_to_ns(pre_vsync_timestamp)));
}

static int vsync_timestamp_changed(struct hisi_fb_data_type *hisifd,
	ktime_t prev_timestamp)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	return !(prev_timestamp == hisifd->vsync_ctrl.vsync_timestamp);
}

static ssize_t vsync_show_event(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret;
	int vsync_flag;
	bool report_flag = false;
	struct fb_info *fbi = NULL;
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_secure *secure_ctrl = NULL;
	ktime_t prev_timestamp;

	hisi_check_and_return((!dev), -1, ERR, "dev is NULL.\n");

	fbi = dev_get_drvdata(dev);
	hisi_check_and_return((!fbi), -1, ERR, "fbi is NULL.\n");

	hisifd = (struct hisi_fb_data_type *)fbi->par;
	hisi_check_and_return((!hisifd), -1, ERR, "hisifd is NULL.\n");

	hisi_check_and_return((!buf), -1, ERR, "buf is NULL.\n");

	secure_ctrl = &(hisifd->secure_ctrl);
	prev_timestamp = hisifd->vsync_ctrl.vsync_timestamp;

	/*lint -e666*/
	ret = wait_event_interruptible(hisifd->vsync_ctrl.vsync_wait,
		(vsync_timestamp_changed(hisifd, prev_timestamp) && hisifd->vsync_ctrl.vsync_enabled) ||
		(!!secure_ctrl->tui_need_switch));
	/*lint +e666*/

	vsync_flag = (vsync_timestamp_changed(hisifd, prev_timestamp) && hisifd->vsync_ctrl.vsync_enabled);

	report_flag = !!secure_ctrl->tui_need_switch;

	if (vsync_flag && report_flag) {
		HISI_FB_INFO("report (secure_event = %d) to hwc with vsync at (frame_no = %d).\n",
			secure_ctrl->secure_event, hisifd->ov_req.frame_no);

		ret = snprintf(buf, PAGE_SIZE, "VSYNC=%llu, SecureEvent=%d\n",
			ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp), secure_ctrl->secure_event);
		buf[strlen(buf) + 1] = '\0';
		if (secure_ctrl->secure_event == DSS_SEC_DISABLE)
			secure_ctrl->tui_need_switch = 0;

	} else if (vsync_flag) {
		ret = snprintf(buf, PAGE_SIZE, "VSYNC=%llu, xxxxxxEvent=x\n",
			ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp));
		buf[strlen(buf) + 1] = '\0';
		secure_ctrl->tui_need_skip_report = 0;

	} else if (report_flag && !secure_ctrl->tui_need_skip_report) {
		HISI_FB_INFO("report (secure_event = %d) to hwc at (frame_no = %d).\n",
			secure_ctrl->secure_event, hisifd->ov_req.frame_no);

		ret = snprintf(buf, PAGE_SIZE, "xxxxx=%llu, SecureEvent=%d\n",
			ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp), secure_ctrl->secure_event);
		buf[strlen(buf) + 1] = '\0';
		secure_ctrl->tui_need_skip_report = 1;
		if (secure_ctrl->secure_event == DSS_SEC_DISABLE)
			secure_ctrl->tui_need_switch = 0;

	} else {
		return -1;
	}

	return ret;
}

static ssize_t vsync_timestamp_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	ssize_t ret;
	struct fb_info *fbi = NULL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!dev) {
		HISI_FB_ERR("NULL Pointer.\n");
		return -1;
	}

	fbi = dev_get_drvdata(dev);
	if (!fbi) {
		HISI_FB_ERR("NULL Pointer.\n");
		return -1;
	}

	hisifd = (struct hisi_fb_data_type *)fbi->par;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer.\n");
		return -1;
	}

	if (!buf) {
		HISI_FB_ERR("NULL Pointer.\n");
		return -1;
	}

	ret = snprintf(buf, PAGE_SIZE, "%llu\n", ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp));
	buf[strlen(buf) + 1] = '\0';

	return ret;
}

static DEVICE_ATTR(vsync_event, 0444, vsync_show_event, NULL);
static DEVICE_ATTR(vsync_timestamp, 0444, vsync_timestamp_show, NULL);

#ifdef CONFIG_HISI_FB_V600
static bool check_low_temperature(struct hisi_fb_data_type *hisifd)
{
	uint32_t perictrl4;

	hisi_check_and_return(!hisifd, FALSE, ERR, "hisifd is NULL\n");

	perictrl4 = inp32(hisifd->pmctrl_base + MIDIA_PERI_CTRL4);
	perictrl4 &= PMCTRL_PERI_CTRL4_TEMPERATURE_MASK;
	perictrl4 = (perictrl4 >> PMCTRL_PERI_CTRL4_TEMPERATURE_SHIFT);
	HISI_FB_DEBUG("Get current temperature: %d\n", perictrl4);

	if (perictrl4 != NORMAL_TEMPRATURE) /* 0:normal temperature    1:low temperature */
		return true;
	else
		return false;
}

static void hisifb_low_temperature_clk_protect(struct hisi_fb_data_type *hisifd)
{
	struct dss_vote_cmd vote_cmd;
	int ret;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	if (!check_low_temperature(hisifd))
		return;

	down(&g_hisifb_dss_clk_vote_sem);

	vote_cmd = hisifd->dss_vote_cmd;
	/* clk_rate_l4@low_temperature enable would be fail,
	 * so clk config change to clk_rate_l3 before enter idle.
	 */
	if (vote_cmd.dss_pri_clk_rate == DEFAULT_DSS_CORE_CLK_RATE_L4) {
		g_vote_cmd = hisifd->dss_vote_cmd;
		vote_cmd.dss_pri_clk_rate = DEFAULT_DSS_CORE_CLK_RATE_L3;
		vote_cmd.dss_mmbuf_rate = DEFAULT_DSS_MMBUF_CLK_RATE_L3;
		ret = hisifb_set_edc_mmbuf_clk(hisifd, vote_cmd);
		if (ret < 0)
			HISI_FB_INFO("set edc mmbuf clk failed\n");
	}

	up(&g_hisifb_dss_clk_vote_sem);
}

static void hisifb_low_temperature_clk_restore(struct hisi_fb_data_type *hisifd)
{
	int ret;

	if (!g_vote_cmd.dss_pri_clk_rate)
		return;

	down(&g_hisifb_dss_clk_vote_sem);

	/* Recovery is required only if the two are equal.
	 * Otherwise, it means that there is already a process(HWC etc.) to vote clk.
	 */
	if (g_vote_cmd.dss_pri_clk_rate == hisifd->dss_vote_cmd.dss_pri_clk_rate) {
		ret = hisifb_set_edc_mmbuf_clk(hisifd, g_vote_cmd);
		if (ret < 0)
			HISI_FB_INFO("set edc mmbuf clk failed\n");
	}

	memset(&g_vote_cmd, 0, sizeof(g_vote_cmd));

	up(&g_hisifb_dss_clk_vote_sem);
}
#endif

static void hisifb_handle_vsync_ctrl(struct hisi_fb_data_type *hisifd,
	struct hisifb_vsync *vsync_ctrl, struct hisi_fb_panel_data *pdata)
{
	unsigned long flags = 0;

	if (vsync_ctrl->vsync_ctrl_disabled_set && !vsync_ctrl->vsync_disable_enter_idle &&
		(vsync_ctrl->vsync_ctrl_expire_count == 0) && vsync_ctrl->vsync_ctrl_enabled &&
		!vsync_ctrl->vsync_enabled && !vsync_ctrl->vsync_ctrl_offline_enabled) {

		HISI_FB_DEBUG("fb%d, dss clk off!\n", hisifd->index);

		spin_lock_irqsave(&(vsync_ctrl->spin_lock), flags);
		if (pdata->vsync_ctrl)
			pdata->vsync_ctrl(hisifd->pdev, 0);
		else
			HISI_FB_ERR("fb%d, vsync_ctrl not supported!\n", hisifd->index);
		vsync_ctrl->vsync_ctrl_enabled = 0;
		vsync_ctrl->vsync_ctrl_disabled_set = 0;
		spin_unlock_irqrestore(&(vsync_ctrl->spin_lock), flags);

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_MIPI_ULPS)
			mipi_dsi_ulps_cfg(hisifd, 0);

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_VCC_OFF) {
			if (hisifd->lp_fnc)
				hisifd->lp_fnc(hisifd, true);
		}

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_CLK_OFF) {
#ifdef CONFIG_DSS_SMMU_V3
			/* tbu must be disconnected before clk disable */
			hisi_dss_smmuv3_off(hisifd);
#endif

#ifdef CONFIG_HISI_FB_V600
			hisifb_low_temperature_clk_protect(hisifd);
#endif
			dpe_inner_clk_disable(hisifd);
			dpe_common_clk_disable(hisifd);
			mipi_dsi_clk_disable(hisifd);
		}

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_VCC_OFF)
			dpe_regulator_disable(hisifd);

		hisifd->enter_idle = true;
	}
}

static void hisifb_vsync_ctrl_workqueue_handler(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;
	struct hisi_fb_panel_data *pdata = NULL;

	vsync_ctrl = container_of(work, typeof(*vsync_ctrl), vsync_ctrl_work);
	hisi_check_and_no_retval((!vsync_ctrl), ERR, "vsync_ctrl is NULL.\n");

	hisifd = vsync_ctrl->hisifd;
	hisi_check_and_no_retval((!hisifd), ERR, "hisifd is NULL.\n");

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	hisi_check_and_no_retval((!pdata), ERR, "pdata is NULL.\n");

	down(&(hisifd->blank_sem));
	down(&hisifd->blank_sem_effect_hiace);
	down(&hisifd->blank_sem_effect_gmp);

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		goto exit;
	}

	mutex_lock(&(vsync_ctrl->vsync_lock));
	hisifb_handle_vsync_ctrl(hisifd, vsync_ctrl, pdata);
	mutex_unlock(&(vsync_ctrl->vsync_lock));

	if (vsync_ctrl->vsync_report_fnc != NULL) {
		if (is_mipi_video_panel(hisifd))
			vsync_ctrl->vsync_report_fnc(0);
		else
			vsync_ctrl->vsync_report_fnc(1);
	}

exit:
	up(&hisifd->blank_sem_effect_gmp);
	up(&hisifd->blank_sem_effect_hiace);
	up(&(hisifd->blank_sem));
}

void hisifb_vsync_register(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL\n");
		return;
	}
	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL\n");
		return;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		dev_err(&pdev->dev, "vsync_ctrl is NULL\n");
		return;
	}

	if (vsync_ctrl->vsync_created)
		return;
	mutex_init(&(vsync_ctrl->vsync_lock));
	mutex_lock(&(vsync_ctrl->vsync_lock));
	vsync_ctrl->hisifd = hisifd;
	vsync_ctrl->vsync_infinite = 0;
	vsync_ctrl->vsync_enabled = 0;
	vsync_ctrl->vsync_ctrl_offline_enabled = 0;
	mutex_unlock(&(vsync_ctrl->vsync_lock));
	vsync_ctrl->vsync_timestamp = ktime_get();
	vsync_ctrl->vactive_timestamp = ktime_get();
	init_waitqueue_head(&(vsync_ctrl->vsync_wait));
	spin_lock_init(&(vsync_ctrl->spin_lock));
	INIT_WORK(&vsync_ctrl->vsync_ctrl_work, hisifb_vsync_ctrl_workqueue_handler);

	atomic_set(&(vsync_ctrl->buffer_updated), 1);
#ifdef CONFIG_REPORT_VSYNC
	vsync_ctrl->vsync_report_fnc = mali_kbase_pm_report_vsync;
#else
	vsync_ctrl->vsync_report_fnc = NULL;
#endif

	if (hisifd->sysfs_attrs_append_fnc) {
		hisifd->sysfs_attrs_append_fnc(hisifd, &dev_attr_vsync_event.attr);
		hisifd->sysfs_attrs_append_fnc(hisifd, &dev_attr_vsync_timestamp.attr);
	}

	vsync_ctrl->vsync_created = 1;
}

void hisifb_vsync_unregister(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL\n");
		return;
	}
	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL\n");
		return;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		dev_err(&pdev->dev, "vsync_ctrl is NULL\n");
		return;
	}

	if (!vsync_ctrl->vsync_created)
		return;

	vsync_ctrl->vsync_created = 0;
}

void hisifb_set_vsync_activate_state(struct hisi_fb_data_type *hisifd, bool infinite)
{
	struct hisifb_vsync *vsync_ctrl = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		HISI_FB_ERR("vsync_ctrl is NULL\n");
		return;
	}

	if (hisifd->panel_info.vsync_ctrl_type == VSYNC_CTRL_NONE)
		return;

	mutex_lock(&(vsync_ctrl->vsync_lock));

	if (infinite)
		vsync_ctrl->vsync_infinite_count += 1;
	else
		vsync_ctrl->vsync_infinite_count -= 1;

	if (vsync_ctrl->vsync_infinite_count >= 1)
		vsync_ctrl->vsync_infinite = 1;

	if (vsync_ctrl->vsync_infinite_count == 0)
		vsync_ctrl->vsync_infinite = 0;

	mutex_unlock(&(vsync_ctrl->vsync_lock));
}

static bool hisifb_vsync_ctrl_enabled(struct hisi_fb_data_type *hisifd, struct hisifb_vsync *vsync_ctrl)
{
	if (vsync_ctrl->vsync_ctrl_enabled == 0) {
		HISI_FB_DEBUG("fb%d, dss clk on!\n", hisifd->index);

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_VCC_OFF)
			dpe_regulator_enable(hisifd);

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_CLK_OFF) {
			mipi_dsi_clk_enable(hisifd);
			(void)dpe_common_clk_enable(hisifd);
			(void)dpe_inner_clk_enable(hisifd);
#ifdef CONFIG_HISI_FB_V600
			hisifb_low_temperature_clk_restore(hisifd);
#endif
#ifdef CONFIG_DSS_SMMU_V3
			hisi_dss_smmuv3_on(hisifd);
#endif
		}

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_VCC_OFF) {
			if (hisifd->lp_fnc != NULL)
				hisifd->lp_fnc(hisifd, false);
		}

		if (hisifd->panel_info.vsync_ctrl_type & VSYNC_CTRL_MIPI_ULPS)
			mipi_dsi_ulps_cfg(hisifd, 1);

		vsync_ctrl->vsync_ctrl_enabled = 1;
		return true;
	} else if (vsync_ctrl->vsync_ctrl_isr_enabled) {
		vsync_ctrl->vsync_ctrl_isr_enabled = 0;
		return true;
	} else {
		return false;
	}
}

void hisifb_activate_vsync(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;
	unsigned long flags = 0;
	bool clk_enabled = false;

	hisi_check_and_no_retval((!hisifd), ERR, "hisifd is NULL.\n");

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	hisi_check_and_no_retval((!pdata), ERR, "pdata is NULL.\n");

	vsync_ctrl = &(hisifd->vsync_ctrl);
	hisi_check_and_no_retval((!vsync_ctrl), ERR, "vsync_ctrl is NULL.\n");

	if (hisifd->panel_info.vsync_ctrl_type == VSYNC_CTRL_NONE)
		return;

	mutex_lock(&(vsync_ctrl->vsync_lock));
	clk_enabled = hisifb_vsync_ctrl_enabled(hisifd, vsync_ctrl);

	spin_lock_irqsave(&(vsync_ctrl->spin_lock), flags);
	vsync_ctrl->vsync_ctrl_disabled_set = 0;
	vsync_ctrl->vsync_ctrl_expire_count = 0;

	if (clk_enabled) {
		if (pdata->vsync_ctrl)
			pdata->vsync_ctrl(hisifd->pdev, 1);
		else
			HISI_FB_ERR("fb%d, vsync_ctrl not supported!\n", hisifd->index);
	}
	spin_unlock_irqrestore(&(vsync_ctrl->spin_lock), flags);

	mutex_unlock(&(vsync_ctrl->vsync_lock));
}

void hisifb_deactivate_vsync(struct hisi_fb_data_type *hisifd)
{
	struct hisi_fb_panel_data *pdata = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;
	unsigned long flags = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		HISI_FB_ERR("pdata is NULL\n");
		return;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		HISI_FB_ERR("vsync_ctrl is NULL\n");
		return;
	}

	if (hisifd->panel_info.vsync_ctrl_type == VSYNC_CTRL_NONE)
		return;

	if (hisifd->secure_ctrl.secure_event == DSS_SEC_ENABLE)
		return;

	mutex_lock(&(vsync_ctrl->vsync_lock));

	spin_lock_irqsave(&(vsync_ctrl->spin_lock), flags);
	if (vsync_ctrl->vsync_infinite == 0)
		vsync_ctrl->vsync_ctrl_disabled_set = 1;

	if (vsync_ctrl->vsync_ctrl_enabled)
		vsync_ctrl->vsync_ctrl_expire_count = VSYNC_CTRL_EXPIRE_COUNT;
	spin_unlock_irqrestore(&(vsync_ctrl->spin_lock), flags);

	mutex_unlock(&(vsync_ctrl->vsync_lock));
}

int hisifb_vsync_ctrl(struct fb_info *info, const void __user *argp)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	struct hisifb_vsync *vsync_ctrl = NULL;
	int enable = 0;

	hisi_check_and_return((!info), -EINVAL, ERR, "vsync ctrl info NULL Pointer!\n");

	hisifd = (struct hisi_fb_data_type *)info->par;
	hisi_check_and_return((!hisifd), -EINVAL, ERR, "vsync ctrl hisifd NULL Pointer!\n");

	hisi_check_and_return(((hisifd->index != PRIMARY_PANEL_IDX) && (hisifd->index != EXTERNAL_PANEL_IDX)),
		-EINVAL, ERR, "fb%d, vsync ctrl not supported!\n", hisifd->index);

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	hisi_check_and_return((!pdata), -EINVAL, ERR, "pdata NULL Pointer!\n");

	vsync_ctrl = &(hisifd->vsync_ctrl);
	hisi_check_and_return((!vsync_ctrl), -EINVAL, ERR, "vsync_ctrl NULL Pointer!\n");

	hisi_check_and_return((!argp), -EINVAL, ERR, "argp NULL Pointer!\n");

	ret = copy_from_user(&enable, argp, sizeof(enable));
	hisi_check_and_return(ret, ret, ERR, "vsync ctrl ioctl failed!\n");

	enable = (enable) ? 1 : 0;

	mutex_lock(&(vsync_ctrl->vsync_lock));

	if (vsync_ctrl->vsync_enabled == enable) {
		mutex_unlock(&(vsync_ctrl->vsync_lock));
		return 0;
	}

	if (g_debug_online_vsync)
		HISI_FB_INFO("fb%d, enable=%d!\n", hisifd->index, enable);

	vsync_ctrl->vsync_enabled = enable;

	mutex_unlock(&(vsync_ctrl->vsync_lock));

	down(&hisifd->blank_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!", hisifd->index);
		up(&hisifd->blank_sem);
		return 0;
	}

	if (enable)
		hisifb_activate_vsync(hisifd);
	else
		hisifb_deactivate_vsync(hisifd);

	up(&hisifd->blank_sem);

	return 0;
}

void hisifb_vsync_disable_enter_idle(struct hisi_fb_data_type *hisifd, bool disable)
{
	struct hisifb_vsync *vsync_ctrl = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		HISI_FB_ERR("vsync_ctrl is NULL\n");
		return;
	}

	if (hisifd->panel_info.vsync_ctrl_type == VSYNC_CTRL_NONE)
		return;

	mutex_lock(&(vsync_ctrl->vsync_lock));
	if (disable)
		vsync_ctrl->vsync_disable_enter_idle = 1;
	else
		vsync_ctrl->vsync_disable_enter_idle = 0;
	mutex_unlock(&(vsync_ctrl->vsync_lock));
}

int hisifb_vsync_resume(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_vsync *vsync_ctrl = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	vsync_ctrl = &(hisifd->vsync_ctrl);
	if (!vsync_ctrl) {
		HISI_FB_ERR("vsync_ctrl is NULL\n");
		return -EINVAL;
	}

	mutex_lock(&(vsync_ctrl->vsync_lock));
	vsync_ctrl->vsync_ctrl_expire_count = 0;
	vsync_ctrl->vsync_ctrl_disabled_set = 0;
	vsync_ctrl->vsync_ctrl_enabled = 1;
	vsync_ctrl->vsync_ctrl_isr_enabled = 1;
	mutex_unlock(&(vsync_ctrl->vsync_lock));

	atomic_set(&(vsync_ctrl->buffer_updated), 1);

	return 0;
}

int hisifb_vsync_suspend(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	hisifd->vsync_ctrl.vsync_enabled = 0;

	return 0;
}
#pragma GCC diagnostic pop

