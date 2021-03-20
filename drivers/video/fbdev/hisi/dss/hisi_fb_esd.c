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

#include "hisi_fb.h"

#if defined(CONFIG_LCD_KIT_DRIVER) && defined(CONFIG_HUAWEI_DSM)
#include "lcd_kit_common.h"
extern struct lcd_kit_esd_error_info g_esd_error_info;
#endif

#define HISI_ESD_RECOVER_MAX_COUNT 10
#define HISI_ESD_CHECK_MAX_COUNT 3
#define HISI_ESD_POWER_OFF_TIME 100

extern unsigned int g_esd_recover_disable;

#if defined ESD_RESTORE_DSS_VOLTAGE_CLK
static void hisifb_esd_get_vote_cmd(struct hisi_fb_data_type *hisifd, dss_vote_cmd_t *vote_cmd)
{
	down(&g_hisifb_dss_clk_vote_sem);
	vote_cmd->dss_voltage_level = hisifd->dss_vote_cmd.dss_voltage_level;
	vote_cmd->dss_pri_clk_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;
	vote_cmd->dss_mmbuf_rate = hisifd->dss_vote_cmd.dss_mmbuf_rate;
	up(&g_hisifb_dss_clk_vote_sem);
	HISI_FB_DEBUG("fb%d, dss_voltage_level=%d, dss_pri_clk_rate=%llu\n",
		hisifd->index, vote_cmd->dss_voltage_level, vote_cmd->dss_pri_clk_rate);
}
#endif

static void hisifb_esd_recover(struct hisi_fb_data_type *hisifd)
{
	int ret;
	uint32_t bl_level_cur;
	int vsync_enabled;
#ifdef ESD_RESTORE_DSS_VOLTAGE_CLK
	dss_vote_cmd_t vote_cmd = {0};
#endif

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");
	hisi_check_and_no_retval(g_esd_recover_disable, ERR, "g_esd_recover_disable is disable\n");

	down(&hisifd->blank_aod_sem);
	down(&hisifd->brightness_esd_sem);
	if (hisifd->panel_power_on == false) {
		up(&hisifd->brightness_esd_sem);
		up(&hisifd->blank_aod_sem);
		HISI_FB_INFO("panel is off and exit esd recover");
		return;
	}

	if (hisi_fb_get_aod_lock_status() == false) {
		HISI_FB_INFO("ap had released aod lock, stop esd recover!\n");
		up(&hisifd->brightness_esd_sem);
		up(&hisifd->blank_aod_sem);
		return;
	}

	bl_level_cur = hisifd->bl_level;
	hisifb_set_backlight(hisifd, 0, false);
	up(&hisifd->brightness_esd_sem);

#ifdef ESD_RESTORE_DSS_VOLTAGE_CLK
	hisifb_esd_get_vote_cmd(hisifd, &vote_cmd);
#endif

	vsync_enabled = hisifd->vsync_ctrl.vsync_enabled;
	/* lcd panel off */
	ret = hisi_fb_blank_sub(FB_BLANK_POWERDOWN, hisifd->fbi);
	msleep(HISI_ESD_POWER_OFF_TIME);
	if (ret != 0)
		HISI_FB_ERR("fb%d, blank_mode[%d] failed!\n", hisifd->index, FB_BLANK_POWERDOWN);

	/* lcd panel on */
	if (hisi_fb_blank_sub(FB_BLANK_UNBLANK, hisifd->fbi) != 0)
		HISI_FB_ERR("fb%d, blank_mode[%d] failed!\n", hisifd->index, FB_BLANK_UNBLANK);

	hisifd->vsync_ctrl.vsync_enabled = vsync_enabled;

	if (g_debug_online_vsync)
		HISI_FB_INFO("fb%d, vsync_enabled=%d!\n", hisifd->index, hisifd->vsync_ctrl.vsync_enabled);

#ifdef ESD_RESTORE_DSS_VOLTAGE_CLK
	(void)hisifb_restore_dss_voltage_clk_vote(hisifd, vote_cmd);
#endif

	hisi_fb_frame_refresh(hisifd, "esd");
	up(&hisifd->blank_aod_sem);

	/* backlight on */
	msleep(100);  /* sleep 100ms */
	down(&hisifd->brightness_esd_sem);
	hisifb_set_backlight(hisifd, bl_level_cur ? bl_level_cur : hisifd->bl_level, false);
	up(&hisifd->brightness_esd_sem);
}

static void dsm_client_record_esd_err(uint32_t err_no)
{
#if defined(CONFIG_LCD_KIT_DRIVER) && defined(CONFIG_HUAWEI_DSM)
	int i = 0;

	if (lcd_dclient && !dsm_client_ocuppy(lcd_dclient)) {
		dsm_client_record(lcd_dclient, "lcd esd register status error:");
		for (i = 0; i < g_esd_error_info.esd_error_reg_num; i++)
			dsm_client_record(lcd_dclient, "read_reg_val[%d]=0x%x, expect_reg_val[%d]=0x%x",
				g_esd_error_info.esd_reg_index[i],
				g_esd_error_info.esd_error_reg_val[i],
				g_esd_error_info.esd_reg_index[i],
				g_esd_error_info.esd_expect_reg_val[i]);

		dsm_client_record(lcd_dclient, "\n");
		dsm_client_notify(lcd_dclient, err_no);
	}
#endif
}

static void hisifb_esd_check_and_recover(struct hisi_fb_data_type *hisifd, uint32_t *recover_count)
{
	uint32_t esd_check_count = 0;
	int ret = 0;

	while (*recover_count < hisifd->panel_info.esd_recovery_max_count) {
#ifdef CONFIG_LCD_KIT_DRIVER
		if (esd_check_count < hisifd->panel_info.esd_check_max_count) {
#else
		if (esd_check_count < HISI_ESD_CHECK_MAX_COUNT) {
#endif
			if ((hisifd->secure_ctrl.secure_status == DSS_SEC_RUNNING) ||
				((hisifd->secure_ctrl.secure_status == DSS_SEC_IDLE) &&
				(hisifd->secure_ctrl.secure_event == DSS_SEC_ENABLE)))
				break;

			ret = hisifb_ctrl_esd(hisifd);
			if (ret || (hisifd->esd_recover_state == ESD_RECOVER_STATE_START)) {
				esd_check_count++;
				hisifd->esd_happened = 1;
				HISI_FB_INFO("esd check abnormal, esd_check_count:%d!\n", esd_check_count);
			} else {
				hisifd->esd_happened = 0;
				break;
			}
		}

#ifdef CONFIG_LCD_KIT_DRIVER
		if ((esd_check_count >= hisifd->panel_info.esd_check_max_count) ||
			(hisifd->esd_recover_state == ESD_RECOVER_STATE_START)) {
#else
		if ((esd_check_count >= HISI_ESD_CHECK_MAX_COUNT) ||
			(hisifd->esd_recover_state == ESD_RECOVER_STATE_START)) {
#endif
			HISI_FB_ERR("esd recover panel, recover_count:%d!\n", *recover_count);
		#ifdef CONFIG_HUAWEI_DSM
			dsm_client_record_esd_err(DSM_LCD_ESD_STATUS_ERROR_NO);
		#endif
			down(&hisifd->esd_panel_change_sem);
			hisifb_esd_recover(hisifd);
			up(&hisifd->esd_panel_change_sem);
			hisifd->esd_recover_state = ESD_RECOVER_STATE_COMPLETE;
			esd_check_count = 0;
			(*recover_count)++;
		}
	}
}

static void hisifb_esd_check_wq_handler(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_esd *esd_ctrl = NULL;
	uint32_t recover_count = 0;

	esd_ctrl = container_of(work, struct hisifb_esd, esd_check_work);
	hisi_check_and_no_retval(!esd_ctrl, ERR, "esd_ctrl is NULL\n");

	hisifd = esd_ctrl->hisifd;
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	if (hisifd->panel_info.emi_protect_enable && hisifd->enter_idle) {
		hisifd->emi_protect_check_count++;
		if (hisifd->emi_protect_check_count >= HISI_EMI_PROTECT_CHECK_MAX_COUNT)
			hisi_fb_frame_refresh(hisifd, "emi");
	}

	if (!hisifd->panel_info.esd_enable || g_esd_recover_disable) {
		if (g_esd_recover_disable)
			HISI_FB_INFO("esd_enable=%d, g_esd_recover_disable=%d\n",
				hisifd->panel_info.esd_enable, g_esd_recover_disable);

		return;
	}

	hisifb_esd_check_and_recover(hisifd, &recover_count);


	/* recover count equate 5, we disable esd check function */
	if (recover_count >= hisifd->panel_info.esd_recovery_max_count) {
	#ifdef CONFIG_HUAWEI_DSM
		dsm_client_record_esd_err(DSM_LCD_POWER_ABNOMAL_ERROR_NO);
	#endif
		hrtimer_cancel(&esd_ctrl->esd_hrtimer);
		hisifd->panel_info.esd_enable = 0;
		HISI_FB_ERR("esd recover %d count, disable esd function\n", hisifd->panel_info.esd_recovery_max_count);
	}
}

static enum hrtimer_restart hisifb_esd_hrtimer_fnc(struct hrtimer *timer)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_esd *esd_ctrl = NULL;

	esd_ctrl = container_of(timer, struct hisifb_esd, esd_hrtimer);
	if (!esd_ctrl) {
		HISI_FB_ERR("esd_ctrl is NULL\n");
		return HRTIMER_NORESTART;
	}
	hisifd = esd_ctrl->hisifd;
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return HRTIMER_NORESTART;
	}

	if (hisifd->panel_info.esd_enable) {
		if (esd_ctrl->esd_check_wq != NULL)
			queue_work(esd_ctrl->esd_check_wq, &(esd_ctrl->esd_check_work));
	}

	hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
		(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);

	return HRTIMER_NORESTART;
}


void hisifb_esd_register(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_esd *esd_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL\n");
		return;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL\n");
		return;
	}

	esd_ctrl = &(hisifd->esd_ctrl);
	if (!esd_ctrl) {
		dev_err(&pdev->dev, "esd_ctrl is NULL\n");
		return;
	}

	if (esd_ctrl->esd_inited)
		return;

	if (hisifd->panel_info.esd_enable) {
		hisifd->esd_happened = 0;
		hisifd->esd_recover_state = ESD_RECOVER_STATE_NONE;
		esd_ctrl->hisifd = hisifd;

		esd_ctrl->esd_check_wq = create_singlethread_workqueue("esd_check");
		if (esd_ctrl->esd_check_wq == NULL)
			dev_err(&pdev->dev, "create esd_check_wq failed\n");

		INIT_WORK(&esd_ctrl->esd_check_work, hisifb_esd_check_wq_handler);

		/* hrtimer for ESD timing */
		hrtimer_init(&esd_ctrl->esd_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		esd_ctrl->esd_hrtimer.function = hisifb_esd_hrtimer_fnc;
		hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
			(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);

		esd_ctrl->esd_inited = 1;
	}
}

void hisifb_esd_unregister(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_esd *esd_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL\n");
		return;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL\n");
		return;
	}

	esd_ctrl = &(hisifd->esd_ctrl);
	if (!esd_ctrl) {
		dev_err(&pdev->dev, "esd_ctrl is NULL\n");
		return;
	}

	if (!esd_ctrl->esd_inited)
		return;

	if (hisifd->panel_info.esd_enable)
		hrtimer_cancel(&esd_ctrl->esd_hrtimer);

	esd_ctrl->esd_inited = 0;
}

