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

#include "hisi_mipi_dsi.h"
#include "product/attrs/hisi_mipi_attrs.h"

static int mipi_dsi_remove(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	ret = panel_next_remove(pdev);

	if (need_config_dsi0(hisifd)) {
		if (hisifd->dss_dphy0_ref_clk != NULL) {
			clk_put(hisifd->dss_dphy0_ref_clk);
			hisifd->dss_dphy0_ref_clk = NULL;
		}

		if (hisifd->dss_dphy0_cfg_clk != NULL) {
			clk_put(hisifd->dss_dphy0_cfg_clk);
			hisifd->dss_dphy0_cfg_clk = NULL;
		}
	}

	if (need_config_dsi1(hisifd)) {
		if (hisifd->dss_dphy1_ref_clk != NULL) {
			clk_put(hisifd->dss_dphy1_ref_clk);
			hisifd->dss_dphy1_ref_clk = NULL;
		}

		if (hisifd->dss_dphy1_cfg_clk != NULL) {
			clk_put(hisifd->dss_dphy1_cfg_clk);
			hisifd->dss_dphy1_cfg_clk = NULL;
		}
	}

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return ret;
}

static int mipi_dsi_set_backlight(struct platform_device *pdev, uint32_t bl_level)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	ret = panel_next_set_backlight(pdev, bl_level);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return ret;
}

static int mipi_dsi_vsync_ctrl(struct platform_device *pdev, int enable)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	ret = panel_next_vsync_ctrl(pdev, enable);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return ret;
}

static int mipi_dsi_lcd_fps_scence_handle(struct platform_device *pdev, uint32_t scence)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	ret = panel_next_lcd_fps_scence_handle(pdev, scence);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return ret;
}

static int mipi_dsi_lcd_fps_updt_handle(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +!\n", hisifd->index);

	ret = panel_next_lcd_fps_updt_handle(pdev);

	HISI_FB_DEBUG("fb%d, -!\n", hisifd->index);

	return ret;
}

static int mipi_dsi_esd_handle(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_panel_info *pinfo = NULL;
	char __iomem *mipi_dsi0_base = NULL;
	bool is_timeout = true;
	uint32_t cmp_stopstate_val = 0;
	uint32_t try_times;
	uint32_t temp;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	mipi_dsi0_base = hisifd->mipi_dsi0_base;
	pinfo = &(hisifd->panel_info);

	HISI_FB_DEBUG("fb%d, +!\n", hisifd->index);

	if (pinfo->esd_skip_mipi_check == 1)
		goto panel_check;

	if (hisifd->panel_info.mipi.lane_nums >= DSI_4_LANES)
		cmp_stopstate_val = (BIT(4) | BIT(7) | BIT(9) | BIT(11));
	else if (hisifd->panel_info.mipi.lane_nums >= DSI_3_LANES)
		cmp_stopstate_val = (BIT(4) | BIT(7) | BIT(9));
	else if (hisifd->panel_info.mipi.lane_nums >= DSI_2_LANES)
		cmp_stopstate_val = (BIT(4) | BIT(7));
	else
		cmp_stopstate_val = (BIT(4));

	/* check DPHY data and clock lane stopstate */
	try_times = 0;
	temp = inp32(mipi_dsi0_base + MIPIDSI_PHY_STATUS_OFFSET);
	while ((temp & cmp_stopstate_val) != cmp_stopstate_val) {
		udelay(10);  /* 10us */
		if (++try_times > 100) {  /* 1ms */
			is_timeout = false;
			break;
		}

		temp = inp32(mipi_dsi0_base + MIPIDSI_PHY_STATUS_OFFSET);
	}

	if (is_timeout) {
		HISI_FB_ERR("fb%d, check DPHY data lane status failed! MIPIDSI_PHY_STATUS=0x%x.\n",
			hisifd->index, temp);
		ret = -EINVAL;
		goto error;
	}

panel_check:
	/* disable generate High Speed clock
	 * check panel power status
	 */
	ret = panel_next_esd_handle(pdev);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return ret;

error:
	return ret;
}

static int mipi_dsi_set_display_region(struct platform_device *pdev, struct dss_rect *dirty)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return((!pdev || !dirty), -EINVAL, ERR, "pdev or dirty is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	HISI_FB_DEBUG("index=%d, enter!\n", hisifd->index);

	ret = panel_next_set_display_region(pdev, dirty);

	HISI_FB_DEBUG("index=%d, exit!\n", hisifd->index);

	return ret;
}

#if defined(CONFIG_HISI_DISPLAY_DFR)
static void hisi_dss_wait_dfr_end(struct hisi_fb_data_type *hisifd)
{
	uint32_t timeout = 65;  /* the max of timeout time is from 32ms to 39ms */

	while ((hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_NEED_DOING ||
		hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_DOING) && (--timeout))
		usleep_range(500, 600);  /* sleep range from 500us to 600us */

	/*lint -e529*/
	if (timeout == 0)
		HISI_FB_INFO("wait_dfr_end timeout, drf_status=%d, vstate=0x%x\n",
			hisifd->panel_info.frm_rate_ctrl.status,
			inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE));

	HISI_FB_DEBUG("drf_status=%d, timeout=%d, vstate=0x%x\n",
		hisifd->panel_info.frm_rate_ctrl.status, timeout,
		inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE));
	/*lint +e529*/
}

#define TIME_TO_NEXT_VSYNC 4000000  /* 4ms */
#define DELAY_TIME_AFTER_VSYNC 1000000  /* 1ms */

static int hisi_dss_check_dfr_status(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	int ret = 0;
	uint64_t curr_time;
	uint64_t vsync_timediff;
	uint64_t last_vsync;

	if (!is_lcd_dfr_support(hisifd))
		return 0;

	vsync_timediff = hisi_dss_get_vsync_timediff(hisifd);
	if (vsync_timediff == 0) {
		HISI_FB_ERR("error vsync_timediff, maybe pinfo->fps is 0\n");
		return -1;
	}
	curr_time = ktime_to_ns(ktime_get());
	last_vsync = ktime_to_ns(hisifd->vsync_ctrl.vsync_timestamp);

	if (curr_time <= (last_vsync + DELAY_TIME_AFTER_VSYNC)) {
		if (hisifd->panel_info.frm_rate_ctrl.status != FRM_UPDT_DONE) {
			usleep_range(1000, 1200);  /* sleep range from 1000us to 1200us */
			hisi_dss_wait_dfr_end(hisifd);
		}
	} else if (curr_time > (last_vsync + DELAY_TIME_AFTER_VSYNC) &&
		(curr_time - last_vsync) < (vsync_timediff - TIME_TO_NEXT_VSYNC)) {
		hisi_dss_wait_dfr_end(hisifd);
	} else {
		ret = hisi_vactive0_start_config(hisifd, pov_req);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_vactive0_start_config failed! ret=%d\n", hisifd->index, ret);
			return -1;
		}
		if (hisifd->panel_info.frm_rate_ctrl.status != FRM_UPDT_DONE) {
			usleep_range(1000, 1200);  /* sleep range from 1000us to 1200us */
			hisi_dss_wait_dfr_end(hisifd);
		}
	}

	return 0;
}
#endif

#ifdef MIPI_RESOURCE_PROTECTION
static uint32_t get_current_mipi_task(struct hisi_fb_data_type *hisifd, bool except_dfr)
{
	uint32_t mipi_task = 0;

	if (hisifd->mipi_dsi_bit_clk_update == 1)
		mipi_task |= BIT(0);

	if (hisifd->underflow_flag == 1)
		mipi_task |= BIT(1);

	if ((hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_NEED_DOING ||
		hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_DOING) && !except_dfr)
		mipi_task |= BIT(2);

	return mipi_task;
}

int hisifb_wait_for_mipi_resource_available(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	int wait_count = 0;
	uint32_t task;
	uint32_t task_tmp = 0;
	unsigned long flags = 0;
	int ret = 0;
	bool except_dfr = false;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");
	hisi_check_and_return(!pov_req, -EINVAL, ERR, "pov_req is NULL\n");

#if defined(CONFIG_HISI_DISPLAY_DFR)
	if (is_mipi_video_panel(hisifd) && is_lcd_dfr_support(hisifd)) {
		ret = hisi_dss_check_dfr_status(hisifd, pov_req);
		if (ret < 0)
			return -EINVAL;

		except_dfr = true;
	}
#endif

	spin_lock_irqsave(&hisifd->mipi_resource_lock, flags);
	task = get_current_mipi_task(hisifd, except_dfr);
	spin_unlock_irqrestore(&hisifd->mipi_resource_lock, flags);

	if (task != 0) {
		task_tmp = task;
		while ((task_tmp != 0) && (wait_count < MIPI_CLK_UPDT_TIMEOUT)) {
			wait_count++;
			usleep_range(1000, 1100); /* sleep range from 1000us to 1100us */
			task_tmp = get_current_mipi_task(hisifd, except_dfr);
		}
		HISI_FB_INFO("wait task 0x%x, cost time %d ms", task, wait_count);
	}

	return 0;
}
#else
int hisifb_wait_for_mipi_resource_available(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	(void)hisifd;
	(void)pov_req;

	return 0;
}
#endif

static int mipi_dsi_primary_clk_irq_setup(struct hisi_fb_data_type *hisifd, struct platform_device *pdev)
{
	int ret;

	hisifd->dss_dphy0_ref_clk = devm_clk_get(&pdev->dev, hisifd->dss_dphy0_ref_clk_name);
	if (IS_ERR(hisifd->dss_dphy0_ref_clk)) {
		ret = PTR_ERR(hisifd->dss_dphy0_ref_clk);
		return ret;
	}

	ret = clk_set_rate(hisifd->dss_dphy0_ref_clk, DEFAULT_MIPI_CLK_RATE);
	hisi_check_and_return((ret < 0), -EINVAL, ERR,
		"fb%d dss_dphy0_ref_clk clk_set_rate[%lu] failed, error=%d!\n",
		hisifd->index, DEFAULT_MIPI_CLK_RATE, ret);

	HISI_FB_INFO("dss_dphy0_ref_clk:[%lu]->[%lu]\n",
		DEFAULT_MIPI_CLK_RATE, clk_get_rate(hisifd->dss_dphy0_ref_clk));

	hisifd->dss_dphy0_cfg_clk = devm_clk_get(&pdev->dev, hisifd->dss_dphy0_cfg_clk_name);
	if (IS_ERR(hisifd->dss_dphy0_cfg_clk)) {
		ret = PTR_ERR(hisifd->dss_dphy0_cfg_clk);
		return ret;
	}

	ret = clk_set_rate(hisifd->dss_dphy0_cfg_clk, DEFAULT_MIPI_CLK_RATE);
	hisi_check_and_return((ret < 0), -EINVAL, ERR,
		"fb%d dss_dphy0_cfg_clk clk_set_rate[%lu] failed, error=%d!\n",
		hisifd->index, DEFAULT_MIPI_CLK_RATE, ret);

	HISI_FB_INFO("dss_dphy0_cfg_clk:[%lu]->[%lu]\n",
		DEFAULT_MIPI_CLK_RATE, clk_get_rate(hisifd->dss_dphy0_cfg_clk));

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V320) || \
	defined(CONFIG_HISI_FB_330) || defined(CONFIG_HISI_FB_V501)
	hisifd->dss_pclk_dsi0_clk = devm_clk_get(&pdev->dev, hisifd->dss_pclk_dsi0_name);
	if (IS_ERR(hisifd->dss_pclk_dsi0_clk)) {
		ret = PTR_ERR(hisifd->dss_pclk_dsi0_clk);
		return ret;
	}

	HISI_FB_INFO("dss_pclk_dsi0_clk:[%lu]->[%lu]\n",
		DEFAULT_PCLK_DSI_RATE, clk_get_rate(hisifd->dss_pclk_dsi0_clk));
#endif

	return 0;
}


static int mipi_dsi_external_clk_irq_setup(struct hisi_fb_data_type *hisifd, struct platform_device *pdev)
{
	int ret;

	hisifd->dss_dphy1_ref_clk = devm_clk_get(&pdev->dev, hisifd->dss_dphy1_ref_clk_name);
	if (IS_ERR(hisifd->dss_dphy1_ref_clk)) {
		ret = PTR_ERR(hisifd->dss_dphy1_ref_clk);
		return ret;
	}
	ret = clk_set_rate(hisifd->dss_dphy1_ref_clk, DEFAULT_MIPI_CLK_RATE);
	hisi_check_and_return((ret < 0), -EINVAL, ERR,
		"fb%d dss_dphy1_ref_clk clk_set_rate[%lu] failed, error=%d!\n",
		hisifd->index, DEFAULT_MIPI_CLK_RATE, ret);

	HISI_FB_INFO("dss_dphy1_ref_clk:[%lu]->[%lu]\n",
		DEFAULT_MIPI_CLK_RATE, clk_get_rate(hisifd->dss_dphy1_ref_clk));


	hisifd->dss_dphy1_cfg_clk = devm_clk_get(&pdev->dev, hisifd->dss_dphy1_cfg_clk_name);
	if (IS_ERR(hisifd->dss_dphy1_cfg_clk)) {
		ret = PTR_ERR(hisifd->dss_dphy1_cfg_clk);
		return ret;
	}
	ret = clk_set_rate(hisifd->dss_dphy1_cfg_clk, DEFAULT_MIPI_CLK_RATE);
	hisi_check_and_return((ret < 0), -EINVAL, ERR,
		"fb%d dss_dphy1_cfg_clk clk_set_rate[%lu] failed, error=%d!\n",
		hisifd->index, DEFAULT_MIPI_CLK_RATE, ret);

	HISI_FB_INFO("dss_dphy1_cfg_clk:[%lu]->[%lu]\n",
		DEFAULT_MIPI_CLK_RATE, clk_get_rate(hisifd->dss_dphy1_cfg_clk));

#if !defined(CONFIG_HISI_FB_V510) && !defined(CONFIG_HISI_FB_V600)
	hisifd->dss_pclk_dsi1_clk = devm_clk_get(&pdev->dev, hisifd->dss_pclk_dsi1_name);
	if (IS_ERR(hisifd->dss_pclk_dsi1_clk)) {
		ret = PTR_ERR(hisifd->dss_pclk_dsi1_clk);
		return ret;
	}

	HISI_FB_INFO("dss_pclk_dsi1_clk:[%lu]->[%lu]\n",
		DEFAULT_PCLK_DSI_RATE, clk_get_rate(hisifd->dss_pclk_dsi1_clk));
#endif

	return 0;
}

static int mipi_dsi_clk_irq_setup(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret = 0;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	if (need_config_dsi0(hisifd) ||
		(hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE)) {
		ret = mipi_dsi_primary_clk_irq_setup(hisifd, pdev);
		if (ret)
			return ret;
	}


	if (need_config_dsi1(hisifd) ||
		(hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE)) {
		ret = mipi_dsi_external_clk_irq_setup(hisifd, pdev);
		if (ret)
			return ret;
	}

	return ret;
}

static void mipi_dsi_pdata_registe_cb(struct hisi_fb_panel_data *pdata)
{
	pdata->set_fastboot = mipi_dsi_set_fastboot;
	pdata->on = mipi_dsi_on;
	pdata->off = mipi_dsi_off;
	pdata->remove = mipi_dsi_remove;
	pdata->set_backlight = mipi_dsi_set_backlight;
	pdata->vsync_ctrl = mipi_dsi_vsync_ctrl;
	pdata->lcd_fps_scence_handle = mipi_dsi_lcd_fps_scence_handle;
	pdata->lcd_fps_updt_handle = mipi_dsi_lcd_fps_updt_handle;
	pdata->esd_handle = mipi_dsi_esd_handle;
	pdata->set_display_region = mipi_dsi_set_display_region;
	pdata->mipi_dsi_bit_clk_upt_store = mipi_dsi_bit_clock_upt_store;
	pdata->mipi_dsi_bit_clk_upt_show = mipi_dsi_bit_clock_upt_show;
	pdata->lcd_model_show = mipi_dsi_lcd_model_show;
	pdata->lcd_check_reg = mipi_dsi_lcd_check_reg_show;
	pdata->lcd_mipi_detect = mipi_dsi_lcd_mipi_detect_show;
	pdata->lcd_hkadc_debug_show = mipi_dsi_lcd_hkadc_debug_show;
	pdata->lcd_hkadc_debug_store = mipi_dsi_lcd_hkadc_debug_store;
	pdata->lcd_gram_check_show = mipi_dsi_lcd_gram_check_show;
	pdata->lcd_gram_check_store = mipi_dsi_lcd_gram_check_store;
	pdata->lcd_dynamic_sram_checksum_show = mipi_dsi_lcd_dynamic_sram_checksum_show;
	pdata->lcd_dynamic_sram_checksum_store = mipi_dsi_lcd_dynamic_sram_checksum_store;
	pdata->lcd_voltage_enable_store = mipi_dsi_lcd_voltage_enable_store;
	pdata->lcd_bist_check = mipi_dsi_lcd_bist_check;
	pdata->lcd_sleep_ctrl_show = mipi_dsi_lcd_sleep_ctrl_show;
	pdata->lcd_sleep_ctrl_store = mipi_dsi_lcd_sleep_ctrl_store;
	pdata->lcd_acl_ctrl_show = mipi_dsi_lcd_acl_ctrl_show;
	pdata->lcd_acl_ctrl_store = mipi_dsi_lcd_acl_ctrl_store;
	pdata->lcd_amoled_vr_mode_show = mipi_dsi_lcd_amoled_vr_mode_show;
	pdata->lcd_amoled_vr_mode_store = mipi_dsi_lcd_amoled_vr_mode_store;
	pdata->lcd_test_config_show = mipi_dsi_lcd_test_config_show;
	pdata->lcd_test_config_store = mipi_dsi_lcd_test_config_store;
	pdata->lcd_reg_read_show = mipi_dsi_lcd_reg_read_show;
	pdata->lcd_reg_read_store = mipi_dsi_lcd_reg_read_store;
	pdata->lcd_support_mode_show = mipi_dsi_lcd_support_mode_show;
	pdata->lcd_support_mode_store = mipi_dsi_lcd_support_mode_store;
	pdata->lcd_support_checkmode_show = mipi_dsi_lcd_support_checkmode_show;
	pdata->lcd_lp2hs_mipi_check_show = mipi_dsi_lcd_lp2hs_mipi_check_show;
	pdata->lcd_lp2hs_mipi_check_store = mipi_dsi_lcd_lp2hs_mipi_check_store;
	pdata->amoled_pcd_errflag_check = mipi_dsi_amoled_pcd_errflag_check;
	pdata->lcd_ic_color_enhancement_mode_show = mipi_dsi_lcd_ic_color_enhancement_mode_show;
	pdata->lcd_ic_color_enhancement_mode_store = mipi_dsi_lcd_ic_color_enhancement_mode_store;
	pdata->sharpness2d_table_store = mipi_dsi_sharpness2d_table_store;
	pdata->sharpness2d_table_show = mipi_dsi_sharpness2d_table_show;
	pdata->panel_info_show = mipi_dsi_panel_info_show;
	pdata->get_lcd_id = mipi_dsi_get_lcd_id;
	pdata->amoled_alpm_setting_store = mipi_dsi_alpm_setting;
	pdata->panel_switch = mipi_dsi_panel_switch;
	pdata->get_panel_info = mipi_dsi_get_panel_info;
}

static int mipi_dsi_probe(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct platform_device *dpp_dev = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	int ret;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	dev_check_and_return(&pdev->dev, !hisifd, -EINVAL, err, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);
	mipi_transfer_lock_init();

	ret = mipi_dsi_clk_irq_setup(pdev);
	if (ret) {
		dev_err(&pdev->dev, "fb%d mipi_dsi_irq_clk_setup failed, error=%d!\n", hisifd->index, ret);
		goto err;
	}

	/* alloc device */
	dpp_dev = platform_device_alloc(DEV_NAME_DSS_DPE, pdev->id);
	if (!dpp_dev) {
		dev_err(&pdev->dev, "fb%d platform_device_alloc failed, error=%d!\n", hisifd->index, ret);
		ret = -ENOMEM;
		goto err_device_alloc;
	}

	/* link to the latest pdev */
	hisifd->pdev = dpp_dev;

	/* alloc panel device data */
	ret = platform_device_add_data(dpp_dev, dev_get_platdata(&pdev->dev),
		sizeof(struct hisi_fb_panel_data));
	if (ret) {
		dev_err(&pdev->dev, "fb%d platform_device_add_data failed error=%d!\n", hisifd->index, ret);
		goto err_device_put;
	}

	/* data chain */
	pdata = dev_get_platdata(&dpp_dev->dev);
	pdata->lp_ctrl = NULL;
	mipi_dsi_pdata_registe_cb(pdata);
	pdata->next = pdev;

	/* get/set panel info */
	memcpy(&hisifd->panel_info, pdata->panel_info, sizeof(hisifd->panel_info));

	/* set driver data */
	platform_set_drvdata(dpp_dev, hisifd);
	/* device add */
	ret = platform_device_add(dpp_dev);
	if (ret) {
		dev_err(&pdev->dev, "fb%d platform_device_add failed, error=%d!\n", hisifd->index, ret);
		goto err_device_put;
	}


	if (hisifd->panel_info.delayed_cmd_queue_support)
		mipi_dsi_init_delayed_cmd_queue();

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return 0;

err_device_put:
	platform_device_put(dpp_dev);
	hisifd->pdev = NULL;
err_device_alloc:
err:
	return ret;
}

static struct platform_driver this_driver = {
	.probe = mipi_dsi_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = DEV_NAME_MIPIDSI,
	},
};

static int __init mipi_dsi_driver_init(void)
{
	int ret;

	ret = platform_driver_register(&this_driver);
	if (ret) {
		HISI_FB_ERR("platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

module_init(mipi_dsi_driver_init);

