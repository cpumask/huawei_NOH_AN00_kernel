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
#include "hisi_fb_dts.h"
#include "hisi_mmbuf_manager.h"
#include <huawei_platform/log/log_jank.h>
#include "hisi_display_effect.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/device.h>
#include <linux/power/hisi_hi6521_charger_power.h>
#include "hisi_frame_rate_ctrl.h"
#include "hisi_mipi_dsi.h"

#if defined(CONFIG_TEE_TUI)
#include "tui.h"
#endif


#ifdef CONFIG_LCD_KIT_DRIVER
#include "lcd_kit_core.h"
#endif

#define set_flag_bits_if_cond(cond, flag, bits) \
	do { if (cond) (flag) |= (bits); } while (0)

uint8_t color_temp_cal_buf[32] = {0};

static int hisi_fb_resource_initialized;
static struct platform_device *pdev_list[HISI_FB_MAX_DEV_LIST] = {0};

static int pdev_list_cnt;
struct fb_info *fbi_list[HISI_FB_MAX_FBI_LIST] = {0};
static int fbi_list_index;

struct hisi_fb_data_type *hisifd_list[HISI_FB_MAX_FBI_LIST] = {0};
static int hisifd_list_index;
struct semaphore g_hisifb_dss_clk_vote_sem;

uint32_t g_dts_resouce_ready;
uint32_t g_fastboot_enable_flag;
uint32_t g_fake_lcd_flag;
uint32_t g_dss_version_tag;
uint32_t g_dss_module_resource_initialized;
uint32_t g_logo_buffer_base;
uint32_t g_logo_buffer_size;
uint32_t g_mipi_dphy_version;
uint32_t g_mipi_dphy_opt;
uint32_t g_chip_id;

uint32_t g_fastboot_already_set;

unsigned int g_esd_recover_disable;  //lint !e552
int g_fastboot_set_needed;

#define MAX_DPE_NUM 3
static struct regulator_bulk_data g_dpe_regulator[MAX_DPE_NUM];

/*lint -e552*/
int g_primary_lcd_xres;
int g_primary_lcd_yres;
uint64_t g_pxl_clk_rate;
uint8_t g_prefix_ce_support;
uint8_t g_prefix_sharpness1d_support;
uint8_t g_prefix_sharpness2d_support;
/*lint +e552*/

/* dc brightness dimming */
int delta_bl_delayed;
bool blc_enable_delayed;
bool dc_switch_xcc_updated;
int g_debug_enable_lcd_sleep_in;
uint32_t g_err_status;  //lint !e552

struct hisi_fb_data_type *g_hisifd_fb0;
struct fb_info *g_info_fb0;  //lint !e552

/******************************************************************************
 * FUNCTIONS PROTOTYPES
 */
static int hisi_fb_register(struct hisi_fb_data_type *hisifd);

static int hisi_fb_open(struct fb_info *info, int user);
static int hisi_fb_release(struct fb_info *info, int user);
static int hisi_fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
static int hisi_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info);
static int hisi_fb_set_par(struct fb_info *info);
static int hisi_fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg);
static int hisi_fb_suspend_sub(struct hisi_fb_data_type *hisifd);
static int hisi_fb_resume_sub(struct hisi_fb_data_type *hisifd);


/*******************************************************************************
 *
 */
static void hisifb_init_regulator(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->dpe_regulator = &(g_dpe_regulator[0]);
	hisifd->smmu_tcu_regulator = &(g_dpe_regulator[1]);
	hisifd->mediacrg_regulator = &(g_dpe_regulator[2]);
}

struct platform_device *hisi_fb_add_device(struct platform_device *pdev)
{
	struct hisi_fb_panel_data *pdata = NULL;
	struct platform_device *this_dev = NULL;
	struct fb_info *fbi = NULL;
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t type;
	uint32_t id;

	hisi_check_and_return(!pdev, NULL, ERR, "pdev is NULL\n");
	pdata = dev_get_platdata(&pdev->dev);
	hisi_check_and_return(!pdata, NULL, ERR, "pdata is NULL\n");

	hisi_check_and_return((fbi_list_index >= HISI_FB_MAX_FBI_LIST), NULL, ERR, "no more framebuffer info list!\n");

	id = pdev->id;
	type = pdata->panel_info->type;
	/* alloc panel device data */
	this_dev = hisi_fb_device_alloc(pdata, type, id);
	hisi_check_and_return(!this_dev, NULL, ERR, "failed to hisi_fb_device_alloc!\n");

	/* alloc framebuffer info + par data */
	fbi = framebuffer_alloc(sizeof(struct hisi_fb_data_type), NULL);
	if (!fbi) {
		HISI_FB_ERR("can't alloc framebuffer info data!\n");
		platform_device_put(this_dev);
		return NULL;
	}

	hisifd = (struct hisi_fb_data_type *)fbi->par;
	memset(hisifd, 0, sizeof(*hisifd));
	hisifd->fbi = fbi;
	hisifd->fb_imgType = HISI_FB_PIXEL_FORMAT_BGRA_8888;
	hisifd->index = fbi_list_index;

	hisifb_init_base_addr(hisifd);
	hisifb_init_clk_name(hisifd);
	if (hisifb_init_irq(hisifd, type)) {
		platform_device_put(this_dev);
		framebuffer_release(fbi);
		return NULL;
	}

	hisifb_init_regulator(hisifd);

	/* link to the latest pdev */
	hisifd->pdev = this_dev;

	hisifd_list[hisifd_list_index++] = hisifd;
	fbi_list[fbi_list_index++] = fbi;

	 /* get/set panel info */
	memcpy(&hisifd->panel_info, pdata->panel_info, sizeof(*pdata->panel_info));
	/* set driver data */
	platform_set_drvdata(this_dev, hisifd);

	if (platform_device_add(this_dev)) {
		HISI_FB_ERR("failed to platform_device_add!\n");
		framebuffer_release(fbi);
		platform_device_put(this_dev);
		hisifd_list_index--;
		fbi_list_index--;
		return NULL;
	}

	return this_dev;
}


static void hisi_fb_displayeffect_update(struct hisi_fb_data_type *hisifd)
{
	int disp_panel_id = hisifd->panel_info.disp_panel_id;

	if (hisifd->index == AUXILIARY_PANEL_IDX)
		return;

	HISI_FB_INFO("[effect]+\n");

#if   defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345)
	hisifd->effect_updated_flag[disp_panel_id].post_xcc_effect_updated = true;
	hisifd->effect_updated_flag[disp_panel_id].hiace_effect_updated = true;
	hisifd->hiace_info[hisifd->panel_info.disp_panel_id].algorithm_result = 0;
#endif
}

static void hisi_fb_down_effect_sem(struct hisi_fb_data_type *hisifd)
{
	down(&hisifd->blank_sem_effect);
	down(&hisifd->blank_sem_effect_hiace);
	down(&hisifd->blank_sem_effect_gmp);
}

static void hisi_fb_up_effect_sem(struct hisi_fb_data_type *hisifd)
{
	up(&hisifd->blank_sem_effect_gmp);
	up(&hisifd->blank_sem_effect_hiace);
	up(&hisifd->blank_sem_effect);
}

static void hisifb_config_panel_power_on_status(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->panel_info.disp_panel_id == DISPLAY_INNEL_PANEL_ID)
		hisifd->panel_power_status |= EN_INNER_PANEL_ON;

	if (hisifd->panel_info.disp_panel_id == DISPLAY_OUTER_PANEL_ID)
		hisifd->panel_power_status |= EN_OUTER_PANEL_ON;
}

static void hisifb_config_panel_power_off_status(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->panel_power_status == EN_INNER_OUTER_PANEL_ON) {
		if (hisifd->panel_info.disp_panel_id == DISPLAY_INNEL_PANEL_ID)
			hisifd->panel_power_status = EN_OUTER_PANEL_ON;
		if (hisifd->panel_info.disp_panel_id == DISPLAY_OUTER_PANEL_ID)
			hisifd->panel_power_status = EN_INNER_PANEL_ON;
	} else {
		hisifd->panel_power_status = EN_INNER_OUTER_PANEL_OFF;
	}
}

void hisifb_set_panel_power_status(struct hisi_fb_data_type *hisifd,
	bool power_on)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		if (power_on)
			hisifb_config_panel_power_on_status(hisifd);
		else
			hisifb_config_panel_power_off_status(hisifd);

		HISI_FB_INFO("[fold]disp_panel_id = %d, panel_power_status = %d\n",
			hisifd->panel_info.disp_panel_id, hisifd->panel_power_status);
	}
}

int hisi_fb_blank_sub(int blank_mode, struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret = 0;

	hisi_check_and_return(!info, -EINVAL, ERR, "info is NULL\n");

	hisifd = (struct hisi_fb_data_type *)info->par;
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	down(&hisifd->blank_sem);
	down(&hisifd->blank_sem0);
	hisi_fb_down_effect_sem(hisifd);
	switch (blank_mode) {
	case FB_BLANK_UNBLANK:
		hisi_fb_displayeffect_update(hisifd);
		if (!hisifd->panel_power_on) {
			ret = hisi_fb_blank_panel_power_on(hisifd);
			hisifb_set_panel_power_status(hisifd, true);
		}
		break;

	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_NORMAL:
	case FB_BLANK_POWERDOWN:
	default:
		if (hisifd->panel_power_on) {
			if (hisi_fb_blank_update_tui_status(hisifd))
				break;

			ret = hisi_fb_blank_panel_power_off(hisifd);

			if (hisifd->buf_sync_suspend != NULL)
				hisifd->buf_sync_suspend(hisifd);

			/* reset online play bypass state, ensure normal state when next power on */
			(void) hisi_online_play_bypass_set(hisifd, false);
		}
		break;
	}
	hisi_fb_up_effect_sem(hisifd);
	up(&hisifd->blank_sem0);
	up(&hisifd->blank_sem);

	if (blank_mode == FB_BLANK_UNBLANK)
		dfr_power_on_notification(hisifd);

#ifdef CONFIG_HISI_FB_ALSC
	hisi_alsc_blank(hisifd, blank_mode);
#endif

	return ret;
}

static bool hisi_fb_set_fastboot_needed(struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("info is NULL");
		return false;
	}
	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return false;
	}

	if (g_fastboot_set_needed == 1) {
		hisifb_ctrl_fastboot(hisifd);

		hisifd->panel_power_on = true;
		if (info->screen_base && (info->fix.smem_len > 0))
			memset(info->screen_base, 0x0, info->fix.smem_len);

		g_fastboot_set_needed = 0;
		return true;
	}

	return false;
}

int hisi_fb_open_sub(struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret = 0;
	bool needed = false;

	if (!info) {
		HISI_FB_ERR("info is NULL");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}

	if (hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external) {
		if (hisi_cmdlist_init(hisifd)) {
			HISI_FB_ERR("fb%d hisi_cmdlist_init failed!\n", hisifd->index);
			return -EINVAL;
		}
	}

	if (hisifd->set_fastboot_fnc != NULL)
		needed = hisifd->set_fastboot_fnc(info);

	if (!needed) {
		ret = hisi_fb_blank_sub(FB_BLANK_UNBLANK, info);
		if (ret != 0) {
			if (hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external)
				hisi_cmdlist_deinit(hisifd);
			HISI_FB_ERR("can't turn on display!\n");
			return ret;
		}
	}

	return 0;
}

int hisi_fb_release_sub(struct fb_info *info)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("info is NULL\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	ret = hisi_fb_blank_sub(FB_BLANK_POWERDOWN, info);
	if (ret != 0) {
		HISI_FB_ERR("can't turn off display!\n");
		return ret;
	}

	if (hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external)
		hisi_cmdlist_deinit(hisifd);

	return 0;
}

#if CONFIG_SH_AOD_ENABLE
int hisi_aod_inc_atomic(struct hisi_fb_data_type *hisifd);
void hisi_aod_dec_atomic(struct hisi_fb_data_type *hisifd);
#endif

void hisi_aod_dec_atomic(struct hisi_fb_data_type *hisifd)
{
	int temp;

	if (!hisifd)
		return;

	temp = atomic_read(&(hisifd->atomic_v));
	HISI_FB_INFO("atomic_v = %d\n", temp);
	if (temp <= 0)
		return;

	atomic_dec(&(hisifd->atomic_v));
}

int hisi_aod_inc_atomic(struct hisi_fb_data_type *hisifd)
{
	int temp;

	if (!hisifd)
		return 0;

	temp = atomic_inc_return(&(hisifd->atomic_v));
	HISI_FB_INFO("atomic_v increased to %d\n", temp);
	if (temp == 1)
		return 1;

	HISI_FB_INFO("no need reget dss, atomic_v = %d\n", temp);
	hisi_aod_dec_atomic(hisifd);

	return 0;
}

void hisi_aod_schedule_wq(void)
{
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = g_hisifd_fb0;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return;
	}

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		if (hisifd->enable_fast_unblank_for_multi_panel == false) {
			HISI_FB_INFO("[fold] power up with fb blank!\n");
			hisifd->enable_fast_unblank = false;
			return;
		}
	}

	hisifd->enable_fast_unblank = TRUE;
	queue_work(hisifd->aod_ud_fast_unblank_workqueue, &hisifd->aod_ud_fast_unblank_work);
}

uint32_t hisi_get_panel_product_type(void)
{
	struct hisi_fb_data_type *hisifd = g_hisifd_fb0;

	return hisifd->panel_info.product_type;
}

void hisi_fb_frame_refresh(struct hisi_fb_data_type *hisifd, char *trigger)
{
	char *envp[2];  /* uevent num for environment pointer */
	char buf[64] = {0};  /* buf len for fb_frame_refresh flag */

	snprintf(buf, sizeof(buf), "Refresh=1");
	envp[0] = buf;
	envp[1] = NULL;

	if (!hisifd || !trigger) {
		HISI_FB_ERR("NULL Pointer\n");
		return;
	}

	kobject_uevent_env(&(hisifd->fbi->dev->kobj), KOBJ_CHANGE, envp);
	HISI_FB_INFO("fb%d, %s frame refresh\n", hisifd->index, trigger);
}

static void aod_wait_for_offline_blank(struct hisi_fb_data_type *hisifd, int blank_mode)
{
	int try_times = 0;

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	if (blank_mode != FB_BLANK_UNBLANK)
		return;

#if CONFIG_SH_AOD_ENABLE
#endif
	HISI_FB_DEBUG(" %d ms\n", try_times);
}

static void aod_wait_for_blank(struct hisi_fb_data_type *hisifd, int blank_mode)
{
	uint32_t try_times;

	aod_wait_for_offline_blank(hisifd, blank_mode);

	if (blank_mode == FB_BLANK_UNBLANK) {
		if ((hisifd->panel_power_on) && (hisifd->secure_ctrl.secure_blank_flag)) {
			/* wait for blank */
			HISI_FB_INFO("wait for tui blank!\n");
			while (hisifd->panel_power_on)
				mdelay(1);
		}
		while (hisifd->enable_fast_unblank)
			mdelay(1);  /* delay 1ms */
	}

	if ((blank_mode == FB_BLANK_POWERDOWN) && (hisifd->enable_fast_unblank)) {
		HISI_FB_INFO("need to wait for aod fast unblank wq end!\n");
		try_times = 0;
		while (hisifd->enable_fast_unblank) {
			mdelay(1);
			if (++try_times > 1000) {  /* wait times */
				HISI_FB_ERR("wait for aod fast unblank wq end timeout!\n");
				break;
			}
		}
	}

}

static void restore_fast_unblank_status(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	hisifd->enable_fast_unblank_for_multi_panel = false;
}

static bool hisi_fb_need_fast_unblank(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return false;

	HISI_FB_INFO("+\n");
	if (hisi_aod_need_fast_unblank()) {
		hisifd->enable_fast_unblank_for_multi_panel = true;
		return true;
	}

	HISI_FB_INFO("-\n");
	return false;
}

static int hisi_fb_blank(int blank_mode, struct fb_info *info)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	bool sensorhub_aod_hwlock_succ = false;

	hisi_check_and_return(!info, -EINVAL, ERR, "info is NULL\n");

	hisifd = (struct hisi_fb_data_type *)info->par;
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	hisi_check_and_return((hisifd->panel_info.fake_external && (hisifd->index == EXTERNAL_PANEL_IDX)),
		-EINVAL, INFO, "it is fake, blank it fail\n");

	if (hisifd->index == PRIMARY_PANEL_IDX)
		g_info_fb0 = info;

	sensorhub_aod_hwlock_succ = hisi_fb_request_aod_hw_lock(hisifd);

	hisi_log_enable_if((hisifd->index != AUXILIARY_PANEL_IDX), "fb%d, blank_mode[%d] +\n",
		hisifd->index, blank_mode);

	if (blank_mode == FB_BLANK_UNBLANK) {
		hisi_check_and_return((hisi_fb_need_fast_unblank(hisifd)), 0, INFO, "[aod] power up with fast unblank\n");
		wait_for_aod_stop(hisifd);
		ret = hisi_fb_aod_blank(hisifd, blank_mode);
		hisi_check_and_return((ret == -3), 0, INFO, "AOD in fast unlock mode\n");
	}

	aod_wait_for_blank(hisifd, blank_mode);

	if (blank_mode == FB_BLANK_POWERDOWN)
		down(&hisifd->blank_aod_sem);

	ret = hisi_fb_blank_device(hisifd, blank_mode, info);
	if (ret != 0)
		goto sensorhub_aod_hw_unlock;

	hisi_log_enable_if((hisifd->index != AUXILIARY_PANEL_IDX), "fb%d, blank_mode[%d] -\n",
		hisifd->index, blank_mode);

	g_err_status = 0;

	if (blank_mode == FB_BLANK_POWERDOWN) {
		(void)hisi_fb_aod_blank(hisifd, blank_mode);
		hisi_fb_release_aod_hw_lock(hisifd, sensorhub_aod_hwlock_succ);
		restore_fast_unblank_status(hisifd);
		up(&hisifd->blank_aod_sem);
	} else {
		hisi_fb_release_aod_hw_lock(hisifd, sensorhub_aod_hwlock_succ);
	}

	return 0;

sensorhub_aod_hw_unlock:

	hisi_fb_release_aod_hw_lock(hisifd, sensorhub_aod_hwlock_succ);
	if (blank_mode == FB_BLANK_POWERDOWN)
		up(&hisifd->blank_aod_sem);

	return ret;
}

static int hisi_fb_open(struct fb_info *info, int user)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	if (hisifd->panel_info.fake_external && (hisifd->index == EXTERNAL_PANEL_IDX)) {
		HISI_FB_INFO("fb%d, is fake, open it fail\n", hisifd->index);
		return -EINVAL;
	}

	if (!hisifd->ref_cnt) {
		HISI_FB_DEBUG("fb%d, +!\n", hisifd->index);
		if (hisifd->open_sub_fnc != NULL) {
			LOG_JANK_D(JLID_KERNEL_LCD_OPEN, "%s", "JL_KERNEL_LCD_OPEN 3650");
			ret = hisifd->open_sub_fnc(info);
		}
		HISI_FB_DEBUG("fb%d, -! ret = %d\n", hisifd->index, ret);
	}

	hisifd->ref_cnt++;

	return ret;
}

static int hisi_fb_release(struct fb_info *info, int user)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	if (hisifd->panel_info.fake_external && (hisifd->index == EXTERNAL_PANEL_IDX)) {
		HISI_FB_INFO("fb%d, is fake, release it fail\n", hisifd->index);
		return -EINVAL;
	}

	if (!hisifd->ref_cnt) {
		HISI_FB_INFO("try to close unopened fb%d!\n", hisifd->index);
		return -EINVAL;
	}

	hisifd->ref_cnt--;

	if (!hisifd->ref_cnt) {
		HISI_FB_DEBUG("fb%d, +\n", hisifd->index);
		if (hisifd->release_sub_fnc != NULL)
			ret = hisifd->release_sub_fnc(info);

		HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

		if (hisifd->index == PRIMARY_PANEL_IDX) {
			if (!hisifd->fb_mem_free_flag) {
				hisifb_free_fb_buffer(hisifd);
				hisifd->fb_mem_free_flag = true;
			}
#if defined(CONFIG_HUAWEI_DSM)
			if (lcd_dclient && !dsm_client_ocuppy(lcd_dclient)) {
				HISI_FB_INFO("fb%d, ref_cnt = %d\n", hisifd->index, hisifd->ref_cnt);
				dsm_client_record(lcd_dclient, "No fb0 device can use\n");
				dsm_client_notify(lcd_dclient, DSM_LCD_FB0_CLOSE_ERROR_NO);
			}
#endif
		}
	}

	return ret;
}

static int hisi_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	if (!var) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	if (var->rotate != FB_ROTATE_UR) {
		HISI_FB_ERR("error rotate %d!\n", var->rotate);
		return -EINVAL;
	}

	if (var->grayscale != info->var.grayscale) {
		HISI_FB_DEBUG("error grayscale %d!\n", var->grayscale);
		return -EINVAL;
	}

	if ((var->xres_virtual <= 0) || (var->yres_virtual <= 0)) {
		HISI_FB_ERR("xres_virtual=%d yres_virtual=%d out of range!\n",
			var->xres_virtual, var->yres_virtual);
		return -EINVAL;
	}

	if ((var->xres == 0) || (var->yres == 0)) {
		HISI_FB_ERR("xres=%d, yres=%d is invalid!\n", var->xres, var->yres);
		return -EINVAL;
	}

	if (var->xoffset > (var->xres_virtual - var->xres)) {
		HISI_FB_ERR("xoffset=%d(xres_virtual=%d, xres=%d) out of range!\n",
			var->xoffset, var->xres_virtual, var->xres);
		return -EINVAL;
	}

	if (var->yoffset > (var->yres_virtual - var->yres)) {
		HISI_FB_ERR("yoffset=%d(yres_virtual=%d, yres=%d) out of range!\n",
			var->yoffset, var->yres_virtual, var->yres);
		return -EINVAL;
	}

	return 0;
}

static int hisi_fb_set_par(struct fb_info *info)
{
	uint32_t xres;
	struct hisi_fb_data_type *hisifd = NULL;
	struct fb_var_screeninfo *var = NULL;

	if (!info) {
		HISI_FB_ERR("set par info NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("set par hisifd NULL Pointer\n");
		return -EINVAL;
	}

	var = &info->var;

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE)
		xres = hisifd->panel_info.fb_xres;
	else
		xres = var->xres_virtual;

	hisifd->fbi->fix.line_length = hisifb_line_length(hisifd->index, xres,
		var->bits_per_pixel >> 3);

	return 0;
}

static int hisi_fb_pan_display(struct fb_var_screeninfo *var,
	struct fb_info *info)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!var || !info) {
		HISI_FB_ERR("pan display var or info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("pan display hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	down(&hisifd->blank_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel power off!\n", hisifd->index);
		goto err_out;
	}

	if (var->xoffset > (info->var.xres_virtual - info->var.xres))
		goto err_out;

	if (var->yoffset > (info->var.yres_virtual - info->var.yres))
		goto err_out;

	if (info->fix.xpanstep)
		info->var.xoffset =
		(var->xoffset / info->fix.xpanstep) * info->fix.xpanstep;

	if (info->fix.ypanstep)
		info->var.yoffset =
		(var->yoffset / info->fix.ypanstep) * info->fix.ypanstep;

	if (hisifd->pan_display_fnc != NULL)
		hisifd->pan_display_fnc(hisifd);
	else
		HISI_FB_ERR("fb%d pan_display_fnc not set!\n", hisifd->index);

	up(&hisifd->blank_sem);

	if (hisifd->bl_update != NULL)
		hisifd->bl_update(hisifd);

	return ret;

err_out:
	up(&hisifd->blank_sem);
	return 0;
}

static int hisifb_lcd_dirty_region_info_get(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("dirty region info get info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("dirty region info get hisifdNULL Pointer!\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("dirty region info get argp NULL Pointer!\n");
		return -EINVAL;
	}

	if (copy_to_user(argp, &(hisifd->panel_info.dirty_region_info),
		sizeof(struct lcd_dirty_region_info))) {
		HISI_FB_ERR("copy to user fail");
		return -EFAULT;
	}

	return 0;
}

static int hisifb_alsc_enable_info_get(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct alsc_enable_info enable_info;

	if (!info) {
		HISI_FB_ERR("info is NULL\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("argp is NULL\n");
		return -EINVAL;
	}

	memset(&enable_info, 0, sizeof(enable_info));
#ifdef CONFIG_HISI_FB_ALSC
	enable_info.alsc_en = hisifd->alsc.alsc_en;
	enable_info.alsc_en &= (uint32_t)g_enable_alsc;
	enable_info.alsc_addr = hisifd->panel_info.dirty_region_info.alsc.alsc_addr;
	enable_info.alsc_size = hisifd->panel_info.dirty_region_info.alsc.alsc_size;
	enable_info.bl_update = hisifd->alsc.bl_update_to_hwc;
	HISI_FB_DEBUG("[ALSC]en addr size bl_update=[%u %u %u %u]\n",
		enable_info.alsc_en, enable_info.alsc_addr, enable_info.alsc_size, enable_info.bl_update);
	hisifd->alsc.bl_update_to_hwc = 0;
#endif

	if (copy_to_user(argp, &enable_info, sizeof(enable_info))) {
		HISI_FB_ERR("copy to user fail\n");
		return -EFAULT;
	}

	return 0;
}

static int hisifb_dirty_updt_support_get(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t flag = 0;

	if (!info) {
		HISI_FB_ERR("dirty region updt set info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("dirty region updt set hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_ERR("fb%d, not supported!\n", hisifd->index);
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("dirty region updt set argp NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd->dirty_region_updt_enable = 0;

	/* check partial update diable reason and set bits */
	set_flag_bits_if_cond(!g_enable_dirty_region_updt, flag, DEBUG_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(!hisifd->panel_info.dirty_region_updt_support, flag, LCD_CTRL_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->color_temperature_flag, flag, COLOR_TEMPERATURE_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->display_effect_flag, flag, DISPLAY_EFFECT_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->esd_happened, flag, ESD_HAPPENED_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->secure_ctrl.secure_event != DSS_SEC_DISABLE, flag,
		SECURE_ENABLED_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->aod_mode, flag, AOD_ENABLED_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifd->vr_mode, flag, VR_ENABLED_DIRTY_UPDT_DISABLE);
	set_flag_bits_if_cond(hisifb_display_effect_is_need_ace(hisifd), flag, ACE_ENABLED_DIRTY_UPDT_DISABLE);

	if (is_lcd_dfr_support(hisifd) &&
		(hisifd->panel_info.fps_updt != hisifd->panel_info.fps ||
		hisifd->panel_info.frm_rate_ctrl.status != FRM_UPDT_DONE))
		flag = flag | BIT(10);

	if (flag == 0)
		hisifd->dirty_region_updt_enable = 1;

	if (copy_to_user(argp, &flag, sizeof(flag))) {
		HISI_FB_ERR("copy to user fail");
		return -EFAULT;
	}

	return 0;
}

static int hisifb_get_video_idle_mode(struct fb_info *info, void __user *argp)
{
	int is_video_idle = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info)
		return -EINVAL;

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd)
		return -EINVAL;

	if (!argp)
		return -EINVAL;

	is_video_idle = 0;

	if (g_enable_video_idle_l3cache && is_video_idle_ctrl_mode(hisifd)) {
#if VIDEO_IDLE_GPU_COMPOSE_ENABLE
		/* last idle frame use gpu compose */
		is_video_idle =
			hisifd->video_idle_ctrl.gpu_compose_idle_frame ? 0 : 1;
#else
		is_video_idle = 1;
#endif
	}

	if (copy_to_user(argp, &is_video_idle, sizeof(is_video_idle))) {
		HISI_FB_ERR("copy to user fail");
		return -EFAULT;
	}

	return 0;
}

static int hisifb_idle_is_allowed(struct fb_info *info, void __user *argp)
{
	int is_allowed = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!info) {
		HISI_FB_ERR("idle is allowed info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("idle is allowed hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("idle is allowed argp NULL Pointer!\n");
		return -EINVAL;
	}

	is_allowed = (hisifd->frame_update_flag == 1) ? 0 : 1;

	if (copy_to_user(argp, &is_allowed, sizeof(is_allowed))) {
		HISI_FB_ERR("copy to user fail");
		return -EFAULT;
	}

	return 0;
}

static int hisifb_debug_check_fence_timeline(struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_buf_sync *buf_sync_ctrl = NULL;
	unsigned long flags;
	int val = 0;

	if (!info) {
		HISI_FB_ERR("timeline info NULL Pointer!\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (!hisifd) {
		HISI_FB_ERR("timeline hisifd NULL Pointer!\n");
		return -EINVAL;
	}

	buf_sync_ctrl = &hisifd->buf_sync_ctrl;
	if (buf_sync_ctrl->timeline == NULL) {
		HISI_FB_ERR("timeline NULL Pointer!\n");
		return -EINVAL;
	}

	if (hisifd->ov_req.frame_no != 0)
		HISI_FB_INFO("fb%d frame_no[%d] timeline_max[%d], TL(Nxt %d , Crnt %d)!\n",
			hisifd->index, hisifd->ov_req.frame_no, buf_sync_ctrl->timeline_max,
			buf_sync_ctrl->timeline->next_value, buf_sync_ctrl->timeline->value);

	spin_lock_irqsave(&buf_sync_ctrl->refresh_lock, flags);

	if ((buf_sync_ctrl->timeline->next_value - buf_sync_ctrl->timeline->value) > 0)
		val = buf_sync_ctrl->timeline->next_value - buf_sync_ctrl->timeline->value;

	hisi_dss_resync_timeline(buf_sync_ctrl->timeline);
	hisi_dss_resync_timeline(buf_sync_ctrl->timeline_retire);

	buf_sync_ctrl->timeline_max = buf_sync_ctrl->timeline->next_value + 1;
	buf_sync_ctrl->refresh = 0;

	spin_unlock_irqrestore(&buf_sync_ctrl->refresh_lock, flags);

	if (hisifd->ov_req.frame_no != 0)
		HISI_FB_INFO("fb%d frame_no[%d] timeline_max[%d], TL(Nxt %d , Crnt %d)!\n",
			hisifd->index, hisifd->ov_req.frame_no, buf_sync_ctrl->timeline_max,
			buf_sync_ctrl->timeline->next_value, buf_sync_ctrl->timeline->value);

	return 0;
}

static int hisifb_dss_get_platform_type(struct fb_info *info, void __user *argp)
{
	int type;
	int ret;

	type = HISIFB_DSS_PLATFORM_TYPE;
	if (g_fpga_flag == 1)
		type = HISIFB_DSS_PLATFORM_TYPE | FB_ACCEL_PLATFORM_TYPE_FPGA;

	if (!argp) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}
	ret = copy_to_user(argp, &type, sizeof(type));
	if (ret) {
		HISI_FB_ERR("copy to user failed! ret=%d\n", ret);
		ret = -EFAULT;
	}

	return ret;
}

static int hisifb_dss_get_platform_product_info(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	struct platform_product_info get_platform_product_info;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	if (!argp) {
		HISI_FB_ERR("argp is NULL\n");
		return -EINVAL;
	}

	memset(&get_platform_product_info, 0, sizeof(struct platform_product_info));
	get_platform_product_info.max_hwc_mmbuf_size = mmbuf_max_size;
	get_platform_product_info.max_mdc_mmbuf_size = dss_mmbuf_reserved_info[SERVICE_MDC].size;
	get_platform_product_info.fold_display_support = hisifd->panel_info.cascadeic_support;
	get_platform_product_info.dummy_pixel_num = hisifd->panel_info.dummy_pixel_num;
	get_platform_product_info.dfr_support_value = hisifd->panel_info.dfr_support_value;
	get_platform_product_info.ifbc_type = hisifd->panel_info.ifbc_type;
	get_platform_product_info.p3_support = hisifd->panel_info.p3_support;
	get_platform_product_info.hdr_flw_support = hisifd->panel_info.hdr_flw_support;
	get_platform_product_info.support_ddr_bw_adjust =  hisifd->panel_info.support_ddr_bw_adjust;

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		get_platform_product_info.need_two_panel_display = 1;
		get_platform_product_info.fold_display_support = 1;
		get_platform_product_info.virtual_fb_xres = hisifd->panel_info.fb_xres;
		get_platform_product_info.virtual_fb_yres = hisifd->panel_info.fb_yres;
		hisifb_get_lcd_panel_info(hisifd, &get_platform_product_info);
	}

	if (copy_to_user(argp, &get_platform_product_info, sizeof(struct platform_product_info))) {
		HISI_FB_ERR("copy to user fail!\n");
		return -EINVAL;
	}

	HISI_FB_INFO("max_hwc_mmbuf_size=%d, max_mdc_mmbuf_size=%d, fold_display_support=%d,"
		"dummy_pixel_num=%d, dfr_support_value=%d, p3_support = %d, hdr_flw_support = %d,"
		"ifbc_type = %d, support_ddr_bw_adjust = %d\n",
		get_platform_product_info.max_hwc_mmbuf_size, get_platform_product_info.max_mdc_mmbuf_size,
		get_platform_product_info.fold_display_support, get_platform_product_info.dummy_pixel_num,
		get_platform_product_info.dfr_support_value, get_platform_product_info.p3_support,
		get_platform_product_info.hdr_flw_support, get_platform_product_info.ifbc_type,
		get_platform_product_info.support_ddr_bw_adjust);

	return 0;
}

static int hisi_fb_resource_ioctl(struct fb_info *info, struct hisi_fb_data_type *hisifd, unsigned int cmd,
	void __user *argp)
{
	int ret = -ENOSYS;

	switch (cmd) {
	case HISIFB_DSS_VOLTAGE_GET:
		ret = hisifb_ctrl_dss_voltage_get(info, argp);
		break;
	case HISIFB_DSS_VOLTAGE_SET:
		ret = hisifb_ctrl_dss_voltage_set(info, argp);
		break;
	case HISIFB_DSS_VOTE_CMD_SET:
		ret = hisifb_ctrl_dss_vote_cmd_set(info, argp);
		break;
	case HISIFB_DEBUG_CHECK_FENCE_TIMELINE:
		ret = hisifb_debug_check_fence_timeline(info);
		break;
	case HISIFB_IDLE_IS_ALLOWED:
		ret = hisifb_idle_is_allowed(info, argp);
		break;
	case HISIFB_LCD_DIRTY_REGION_INFO_GET:
		ret = hisifb_lcd_dirty_region_info_get(info, argp);
		break;
	case HISIFB_ALSC_ENABLE_INFO_GET:
		ret = hisifb_alsc_enable_info_get(info, argp);
		break;
	case HISIFB_DIRTY_UPDT_SUPPORT_GET:
		ret = hisifb_dirty_updt_support_get(info, argp);
		break;
	case HISIFB_VIDEO_IDLE_CTRL:
		ret = hisifb_get_video_idle_mode(info, argp);
		break;
	case HISIFB_PLATFORM_TYPE_GET:
		ret = hisifb_dss_get_platform_type(info, argp);
		break;
	case HISIFB_PLATFORM_PRODUCT_INFO_GET:
		ret = hisifb_dss_get_platform_product_info(hisifd, argp);
		break;
	case HISIFB_DPTX_GET_COLOR_BIT_MODE:
		if (hisifd->dp_get_color_bit_mode != NULL)
			ret = hisifd->dp_get_color_bit_mode(hisifd, argp);
		break;
	case HISIFB_DPTX_GET_SOURCE_MODE:
		if (hisifd->dp_get_source_mode != NULL)
			ret = hisifd->dp_get_source_mode(hisifd, argp);
		break;
	case HISIFB_PANEL_REGION_NOTIFY:
		if (hisifd->panel_set_display_region != NULL)
			ret = hisifd->panel_set_display_region(hisifd, argp);
		break;
	case HISIFB_DISPLAY_PANEL_FOLD_STATUS_NOTIFY:
		if (hisifd->set_display_panel_status != NULL)
			ret = hisifd->set_display_panel_status(hisifd, argp);
		break;
	default:
		break;
	}

	return ret;
}

static int hisi_fb_effect_ioctl(struct fb_info *info, struct hisi_fb_data_type *hisifd, unsigned int cmd,
	void __user *argp)
{
	int ret = -ENOSYS;

	switch (cmd) {
	case HISIFB_CE_ENABLE:
		ret = hisifb_ce_service_enable_hiace(info, argp);
		break;
	case HISIFB_CE_SUPPORT_GET:
		ret = hisifb_ce_service_get_support(info, argp);
		break;
	case HISIFB_CE_SERVICE_LIMIT_GET:
		ret = hisifb_ce_service_get_limit(info, argp);
		break;
	case HISIFB_CE_PARAM_GET:
		ret = hisifb_ce_service_get_param(info, argp);
		break;
	case HISIFB_HIACE_PARAM_GET:
		ret = hisifb_ce_service_get_hiace_param(info, argp);
		break;
	case HISIFB_CE_PARAM_SET:
		ret = hisifb_ce_service_set_param(info, argp);
		break;
	case HISIFB_GET_REG_VAL:
		ret = hisifb_get_reg_val(info, argp);
		break;
	case HISIFB_CE_HIST_GET:
		ret = hisifb_ce_service_get_hist(info, argp);
		break;
	case HISIFB_CE_LUT_SET:
		ret = hisifb_ce_service_set_lut(info, argp);
		break;
#ifdef HIACE_SINGLE_MODE_SUPPORT
	case HISIFB_HIACE_SINGLE_MODE_TRIGGER:
		ret = hisifb_hiace_single_mode_trigger(info, argp);
		break;
	case HISIFB_HIACE_BLOCK_ONCE_SET:
		ret = hisifb_hiace_single_mode_block_once_set(info, argp);
		break;
	case HISIFB_HIACE_HIST_GET:
		ret = hisifb_hiace_hist_get(info, argp);
		break;
	case HISIFB_HIACE_FNA_DATA_GET:
		ret = hisifb_hiace_fna_get(info, argp);
		break;
#endif
#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	case HISIFB_GET_HIACE_ENABLE:
		ret = hisifb_get_hiace_enable(info, argp);
		break;
	case HISIFB_HIACE_ROI_GET:
		ret = hisifb_get_hiace_roi(info, argp);
		break;
#endif
	case HISIFB_DISPLAY_ENGINE_INIT:
		ret = hisifb_display_engine_init(info, argp);
		break;
	case HISIFB_DISPLAY_ENGINE_DEINIT:
		ret = hisifb_display_engine_deinit(info, argp);
		break;
	case HISIFB_DISPLAY_ENGINE_PARAM_GET:
		ret = hisifb_display_engine_param_get(info, argp);
		break;
	case HISIFB_DISPLAY_ENGINE_PARAM_SET:
		ret = hisifb_display_engine_param_set(info, argp);
		break;
	case HISIFB_EFFECT_MODULE_INIT:
	case HISIFB_EFFECT_MODULE_DEINIT:
	case HISIFB_EFFECT_INFO_GET:
	case HISIFB_EFFECT_INFO_SET:
		if (hisifd->display_effect_ioctl_handler != NULL)
			ret = hisifd->display_effect_ioctl_handler(hisifd, cmd, argp);
		break;
	default:
		break;
	}

	return ret;
}

static int hisi_fb_display_ioctl(struct fb_info *info, struct hisi_fb_data_type *hisifd, unsigned int cmd,
	void __user *argp)
{
	int ret = -ENOSYS;

	switch (cmd) {
	case HISIFB_VSYNC_CTRL:
		if (hisifd->vsync_ctrl_fnc != NULL)
			ret = hisifd->vsync_ctrl_fnc(info, argp);
		break;
	case HISIFB_GRALLOC_MAP_IOVA:
		ret = hisi_dss_buffer_map_iova(info, argp);
		break;
	case HISIFB_GRALLOC_UNMAP_IOVA:
		ret = hisi_dss_buffer_unmap_iova(info, argp);
		break;
	case HISIFB_MMBUF_SIZE_QUERY:
		ret = hisi_mmbuf_reserved_size_query(info, argp);
		break;
	case HISIFB_DSS_MMBUF_ALLOC:
		ret = hisi_mmbuf_request(info, argp);
		break;
	case HISIFB_DSS_MMBUF_FREE:
		ret = hisi_mmbuf_release(info, argp);
		break;
	case HISIFB_DSS_MMBUF_FREE_ALL:
		ret = hisi_mmbuf_free_all(info, argp);
		break;
	case HISIFB_MDC_CHANNEL_INFO_REQUEST:
		ret = hisi_mdc_chn_request(info, argp);
		break;
	case HISIFB_MDC_CHANNEL_INFO_RELEASE:
		ret = hisi_mdc_chn_release(info, argp);
		break;
	case HISIFB_DPTX_SEND_HDR_METADATA:
		if (hisifd->dptx_hdr_infoframe_sdp_send)
			ret = hisifd->dptx_hdr_infoframe_sdp_send(&(hisifd->dp), argp);
		break;
	case HISIFB_CONFIG_PANEL_ESD_STATUS:
		ret = hisifb_config_panel_esd_status(hisifd, argp);
		break;
	default:
		if (hisifd->ov_ioctl_handler != NULL)
			ret = hisifd->ov_ioctl_handler(hisifd, cmd, argp);
		break;
	}

	return ret;
}

static int hisi_fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	void __user *argp = (void __user *)(uintptr_t)arg;

	hisi_check_and_return(!info, -EINVAL, ERR, "info is NULL!\n");

	hisifd = (struct hisi_fb_data_type *)info->par;
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

#ifdef CONFIG_HISI_FB_MEDIACOMMON
	if (hisifd->index == MEDIACOMMON_PANEL_IDX)
		return hisi_mediacommon_ioctl_handler(hisifd, cmd, argp);
#endif

	ret = hisi_fb_resource_ioctl(info, hisifd, cmd, argp);
	if (ret != -ENOSYS)
		return ret;

	ret = hisi_fb_effect_ioctl(info, hisifd, cmd, argp);
	if (ret != -ENOSYS)
		return ret;

	ret = hisi_fb_display_ioctl(info, hisifd, cmd, argp);
	if (ret != -ENOSYS)
		return ret;
	else
		HISI_FB_ERR("unsupported ioctl [%x]\n", cmd);

	return ret;
}

static ssize_t hisi_fb_read(struct fb_info *info, char __user *buf,
	size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t hisi_fb_write(struct fb_info *info, const char __user *buf,
	size_t count, loff_t *ppos)
{
	int err;

	if (!info)
		return -ENODEV;

	if (!lock_fb_info(info))
		return -ENODEV;

	if (!info->screen_base) {
		unlock_fb_info(info);
		return -ENODEV;
	}

	err = fb_sys_write(info, buf, count, ppos);

	unlock_fb_info(info);

	return err;
}

/*******************************************************************************
 *
 */
static struct fb_ops hisi_fb_ops = {
	.owner = THIS_MODULE,
	.fb_open = hisi_fb_open,
	.fb_release = hisi_fb_release,
	.fb_read = hisi_fb_read,
	.fb_write = hisi_fb_write,
	.fb_cursor = NULL,
	.fb_check_var = hisi_fb_check_var,
	.fb_set_par = hisi_fb_set_par,
	.fb_setcolreg = NULL,
	.fb_blank = hisi_fb_blank,
	.fb_pan_display = hisi_fb_pan_display,
	.fb_fillrect = NULL,
	.fb_copyarea = NULL,
	.fb_imageblit = NULL,
	.fb_sync = NULL,
	.fb_ioctl = hisi_fb_ioctl,
	.fb_compat_ioctl = hisi_fb_ioctl,
	.fb_mmap = hisi_fb_mmap,
};


int hisifb_esd_recover_disable(int value)
{
	struct hisi_fb_panel_data *pdata = NULL;
	/* primary panel */
	struct hisi_fb_data_type *hisifd = hisifd_list[PRIMARY_PANEL_IDX];

	if (!hisifd) {
		HISI_FB_INFO("esd_recover_disable fail\n");
		return 0;
	}

	pdata = (struct hisi_fb_panel_data *)hisifd->pdev->dev.platform_data;
	if (pdata != NULL && pdata->panel_info) {
		if (pdata->panel_info->esd_enable) {
			HISI_FB_INFO("esd_recover_disable=%d\n", value);
			g_esd_recover_disable = value;
		}
	}

	return 0;
}
EXPORT_SYMBOL(hisifb_esd_recover_disable);

int hisifb_check_ldi_porch(struct hisi_panel_info *pinfo)
{
	int vertical_porch_min_time;
	int pxl_clk_rate_div;
	int ifbc_v_porch_div = 1;

	hisi_check_and_return(!pinfo, -EINVAL, ERR, "pinfo is NULL\n");

	if (pinfo->mipi.dsi_timing_support)
		return 0;

	pxl_clk_rate_div = (pinfo->pxl_clk_rate_div == 0 ? 1 : pinfo->pxl_clk_rate_div);
	if (pinfo->ifbc_type == IFBC_TYPE_RSP3X) {
		pxl_clk_rate_div *= 3;  /* IFBC_TYPE_RSP3X */
		pxl_clk_rate_div /= 2;  /* half clk_rate_division */
		ifbc_v_porch_div = 2;  /* half ifbv v_porch_division */
	}

	if (g_fpga_flag == 1)
		return 0;  /* do not check ldi porch in fpga version */

	/* hbp+hfp+hsw time should be longer than 30 pixel clock cycel */
	if (pxl_clk_rate_div * (pinfo->ldi.h_back_porch + pinfo->ldi.h_front_porch
		+ pinfo->ldi.h_pulse_width) <= 30) {
		HISI_FB_ERR("ldi hbp+hfp+hsw is not larger than 30, return!\n");
		return -1;
	}

	/* check vbp+vsw */
	if (pinfo->xres * pinfo->yres >= RES_4K_PAD)
		vertical_porch_min_time = 50;
	else if (pinfo->xres * pinfo->yres >= RES_1600P)
		vertical_porch_min_time = 45;
	else if (pinfo->xres * pinfo->yres >= RES_1440P)
		vertical_porch_min_time = 40;
	else if (pinfo->xres * pinfo->yres >= RES_1200P)
		vertical_porch_min_time = 45;
	else
		vertical_porch_min_time = 35;

	if ((uint32_t)ifbc_v_porch_div * (pinfo->ldi.v_back_porch + pinfo->ldi.v_pulse_width) <
		(uint32_t)vertical_porch_min_time) {
		if ((uint32_t)pinfo->non_check_ldi_porch == 1) {
			HISI_FB_INFO("panel IC specification do not match this rule(vbp+vsw >= %d),"
				"we must skip it, Otherwise it will display unnormal!\n", vertical_porch_min_time);
		} else {
			HISI_FB_ERR("ldi vbp+vsw is less than %d, return!\n", vertical_porch_min_time);
			return -1;
		}
	}

	return 0;
}

static void hisifb_check_dummy_pixel_num(struct hisi_panel_info *pinfo)
{
	if (pinfo->dummy_pixel_num % 2 != 0) {
		HISI_FB_ERR("dummy_pixel_num should be even, so plus 1 !\n");
		pinfo->dummy_pixel_num += 1;
	}

	if (pinfo->dummy_pixel_num >= pinfo->xres) {
		HISI_FB_ERR("dummy_pixel_num is invalid, force set it to 0 !\n");
		pinfo->dummy_pixel_num = 0;
	}
}

static uint32_t get_initial_fps(struct hisi_fb_data_type *hisifd, struct fb_var_screeninfo *var)
{
	uint64_t lane_byte_clock;
	uint32_t hsize;
	uint32_t vsize;
	uint32_t fps;

	if (hisifd->panel_info.mipi.dsi_timing_support) {
		lane_byte_clock = (hisifd->panel_info.mipi.phy_mode == DPHY_MODE) ?
				(uint64_t)(hisifd->panel_info.mipi.dsi_bit_clk * 2 / 8) : /* lane byte clock */
				(uint64_t)(hisifd->panel_info.mipi.dsi_bit_clk / 7);
		lane_byte_clock = lane_byte_clock * 1000000UL;  /* 1MHz */
		hsize = hisifd->panel_info.mipi.hline_time;
		vsize = hisifd->panel_info.yres + hisifd->panel_info.mipi.vsa +
			hisifd->panel_info.mipi.vfp + hisifd->panel_info.mipi.vbp;

		fps = (uint32_t)(lane_byte_clock / hsize / vsize);
	} else {
		hsize = var->upper_margin + var->lower_margin + var->vsync_len + hisifd->panel_info.yres;
		vsize = var->left_margin + var->right_margin + var->hsync_len + hisifd->panel_info.xres;
		fps = (uint32_t)(var->pixclock / hsize / vsize);
	}

	return fps;
}

void hisi_fb_pdp_fnc_register(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->fb_mem_free_flag = false;

	if (g_fastboot_enable_flag == 1) {
		hisifd->set_fastboot_fnc = hisi_fb_set_fastboot_needed;
		g_fastboot_set_needed = 1;
	} else {
		hisifd->set_fastboot_fnc = NULL;
	}

	hisifd->open_sub_fnc = hisi_fb_open_sub;
	hisifd->release_sub_fnc = hisi_fb_release_sub;
	hisifd->hpd_open_sub_fnc = NULL;
	hisifd->hpd_release_sub_fnc = NULL;
	hisifd->lp_fnc = hisifb_ctrl_lp;
	hisifd->esd_fnc = hisifb_ctrl_esd;
	hisifd->sbl_ctrl_fnc = NULL;
	hisifd->fps_upt_isr_handler = hisifb_fps_upt_isr_handler;
	hisifd->mipi_dsi_bit_clk_upt_isr_handler = mipi_dsi_bit_clk_upt_isr_handler;
	hisifd->panel_mode_switch_isr_handler = panel_mode_switch_isr_handler;
	hisifd->sysfs_attrs_add_fnc = hisifb_sysfs_attrs_add;
	hisifd->sysfs_attrs_append_fnc = hisifb_sysfs_attrs_append;
	hisifd->sysfs_create_fnc = hisifb_sysfs_create;
	hisifd->sysfs_remove_fnc = hisifb_sysfs_remove;

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
	hisifd->esd_register = hisifb_esd_register;
	hisifd->esd_unregister = hisifb_esd_unregister;
	hisifd->debug_register = hisifb_debug_register;
	hisifd->debug_unregister = hisifb_debug_unregister;
	hisifd->cabc_update = update_cabc_pwm;
	hisifd->panel_set_display_region = panel_set_display_region;
	hisifd->set_display_panel_status = set_display_panel_status;

	hisi_fb_pm_runtime_register(hisifd);

#if   defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V600)
	hisifd->video_idle_ctrl_register = hisifb_video_idle_ctrl_register;
	hisifd->video_idle_ctrl_unregister = hisifb_video_idle_ctrl_unregister;
#else
	hisifd->video_idle_ctrl_register = NULL;
	hisifd->video_idle_ctrl_unregister = NULL;
#endif

	hisifd->pipe_clk_updt_isr_handler = NULL;
	hisifd->overlay_online_wb_register = NULL;
	hisifd->overlay_online_wb_unregister = NULL;

	hisifd->dumpDss = hisifb_alloc_dumpdss();
}

static void hisi_fbi_data_init(struct fb_info *fbi)
{
	fbi->screen_base = 0;
	fbi->fbops = &hisi_fb_ops;
	fbi->flags = FBINFO_FLAG_DEFAULT;
	fbi->pseudo_palette = NULL;
}

void hisi_fb_offlinecomposer_init(struct fb_var_screeninfo *var, struct hisi_panel_info *panel_info)
{
	hisi_check_and_no_retval((!var || !panel_info), ERR, "panel_info is NULL\n");

	/* for offline composer */
	g_primary_lcd_xres = var->xres;
	g_primary_lcd_yres = var->yres;
	g_pxl_clk_rate = panel_info->pxl_clk_rate;
	g_prefix_ce_support = panel_info->prefix_ce_support;
	g_prefix_sharpness1d_support = panel_info->prefix_sharpness1D_support;
	g_prefix_sharpness2d_support = panel_info->prefix_sharpness2D_support;
}

static int hisi_fbi_info_init(struct hisi_fb_data_type *hisifd, struct fb_info *fbi)
{
	int bpp = 0;
	struct fb_fix_screeninfo *fix = &fbi->fix;
	struct fb_var_screeninfo *var = &fbi->var;
	struct hisi_panel_info *panel_info = &hisifd->panel_info;

	hisi_fb_init_screeninfo_base(fix, var);

	if (!hisi_fb_img_type_valid(hisifd->fb_imgType)) {
		HISI_FB_ERR("fb%d, unkown image type!\n", hisifd->index);
		return -EINVAL;
	}

	hisi_fb_init_sreeninfo_by_img_type(fix, var, hisifd->fb_imgType, &bpp);

	/* for resolution update */
	memset(&(hisifd->resolution_rect), 0, sizeof(dss_rect_t));

	hisi_fb_init_sreeninfo_by_panel_info(var, panel_info, hisifd->fb_num, bpp);

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		panel_info->fps = get_initial_fps(hisifd, var);
		var->reserved[0] = panel_info->fps;
	}
	var->reserved[1] = panel_info->split_support;
	var->reserved[1] = var->reserved[1] | (panel_info->split_logical1_ratio << 8);
	HISI_FB_INFO("fb%d, dsi_timing_support %d, fps = reserved[0] = %d, split_support = 0x%x\n",
		hisifd->index, panel_info->mipi.dsi_timing_support, var->reserved[0], var->reserved[1]);
	snprintf(fix->id, sizeof(fix->id), "hisifb%d", hisifd->index);
	fix->line_length = hisifb_line_length(hisifd->index, var->xres_virtual, bpp);
	fix->smem_len = roundup(fix->line_length * var->yres_virtual, PAGE_SIZE);
	fix->smem_start = 0;

	hisi_fbi_data_init(fbi);

	fix->reserved[0] = is_mipi_cmd_panel(hisifd) ? 1 : 0;

	return 0;
}

static int hisi_fb_register(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *panel_info = NULL;
	struct fb_info *fbi = NULL;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	fbi = hisifd->fbi;
	panel_info = &hisifd->panel_info;

	hisifb_check_dummy_pixel_num(panel_info);

	if (hisi_fbi_info_init(hisifd, fbi) < 0)
		return -EINVAL;

	hisi_check_and_return(hisifb_create_buffer_client(hisifd), -ENOMEM, ERR, "create buffer client failed!\n");

	if (fbi->fix.smem_len > 0)
		hisi_check_and_return((!hisifb_alloc_fb_buffer(hisifd)), -ENOMEM, ERR, "hisifb_alloc_buffer fail!\n");

	hisi_fb_data_init(hisifd);

	atomic_set(&(hisifd->atomic_v), 0);

	hisi_fb_init_sema(hisifd);

	hisi_fb_init_spin_lock(hisifd);

	hisifb_sysfs_init(hisifd);

	if (hisi_fb_registe_callback(hisifd, &fbi->var, panel_info) < 0)
		return -EINVAL;


	if (hisi_overlay_init(hisifd))
		return -EPERM;

	hisi_display_effect_init(hisifd);
	hisifb_display_engine_register(hisifd);

#if defined(CONFIG_HISI_DISPLAY_DFR)
	mipi_dsi_frm_rate_ctrl_init(hisifd);
#endif

	if (register_framebuffer(fbi) < 0) {
		HISI_FB_ERR("fb%d failed to register_framebuffer!\n", hisifd->index);
		return -EPERM;
	}

	if (hisifd->sysfs_attrs_add_fnc != NULL)
		hisifd->sysfs_attrs_add_fnc(hisifd);

	hisi_fb_common_register(hisifd);

	HISI_FB_INFO("FrameBuffer[%d] %dx%d size=%d bytes is registered successfully!\n",
		hisifd->index, fbi->var.xres, fbi->var.yres, fbi->fix.smem_len);

	return 0;
}

/*******************************************************************************
 *
 */
static void hisi_create_aod_wq(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd NULL Pointer!\n");
		return;
	}
	/* creat aod workqueue */
	if (hisifd->index == PRIMARY_PANEL_IDX) {
		hisifd->aod_ud_fast_unblank_workqueue = alloc_ordered_workqueue("aod_ud_fast_unblank", WQ_HIGHPRI | WQ_MEM_RECLAIM);
		if (hisifd->aod_ud_fast_unblank_workqueue == NULL) {
			HISI_FB_ERR("creat aod work queue failed!\n");
			return;
		}
		INIT_WORK(&hisifd->aod_ud_fast_unblank_work, hisi_fb_unblank_wq_handle);

		g_hisifd_fb0 = hisifd;
	}
}

static int hisi_fb_get_regulator_resource(struct platform_device *pdev)
{
	int ret;

	g_dpe_regulator[0].supply = REGULATOR_PDP_NAME;
	g_dpe_regulator[1].supply = REGULATOR_SMMU_TCU_NAME;
	g_dpe_regulator[2].supply = REGULATOR_MEDIA_NAME;
	ret = devm_regulator_bulk_get(&(pdev->dev),
		ARRAY_SIZE(g_dpe_regulator), g_dpe_regulator);

	return ret;
}

static int hisi_fb_init_resource(struct platform_device *pdev)
{
	int ret;
	struct device_node *np = NULL;
	struct device *dev = NULL;

	dev = &pdev->dev;

	dev_dbg(dev, "initialized=%d, +\n", hisi_fb_resource_initialized);

	hisi_mmbuf_info_init();

	pdev->id = 0;

	np = of_find_compatible_node(NULL, NULL, DTS_COMP_FB_NAME);
	hisi_check_and_return(!np, -ENXIO, ERR, "NOT FOUND device node %s!\n", DTS_COMP_FB_NAME);

	ret = hisi_fb_read_property_from_dts(np, dev);
	dev_check_and_return(dev, ret, ret, err, "fail to read property from dts!\n");

	ret = hisi_fb_get_irq_no_from_dts(np, dev);
	dev_check_and_return(dev, ret, ret, err, "fail to get irq from dts!\n");

	ret = hisi_fb_get_baseaddr_from_dts(np, dev);
	dev_check_and_return(dev, ret, ret, err, "fail to get base addr from dts!\n");

	ret = hisi_fb_get_regulator_resource(pdev);
	dev_check_and_return(dev, ret, -ENXIO, err, "failed to get regulator resource! ret = %d\n", ret);

	ret = hisi_fb_get_clk_name_from_dts(np, dev);
	dev_check_and_return(dev, ret, ret, err, "fail to get clk name from dts!\n");

	ret = hisi_dss_iommu_enable(pdev);
	dev_check_and_return(dev, ret, -ENXIO, err, "failed to hisi_dss_iommu_enable! ret = %d\n", ret);

	/* find and get logo-buffer base */
	np = of_find_node_by_path(DTS_PATH_LOGO_BUFFER);
	if (!np)
		dev_err(dev, "NOT FOUND dts path: %s!\n", DTS_PATH_LOGO_BUFFER);

	if (g_fastboot_enable_flag == 1)
		hisi_fb_read_logo_buffer_from_dts(np, dev);

	sema_init(&g_hisifb_dss_clk_vote_sem, 1);

	hisi_fb_resource_initialized = 1;

	hisi_fb_device_set_status0(DTS_FB_RESOURCE_INIT_READY);

	dev_dbg(dev, "initialized=%d, -\n", hisi_fb_resource_initialized);

	return ret;
}

static int hisi_fb_probe(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	struct device *dev = NULL;

	hisi_check_and_return(!pdev, -1, ERR, "pdev is NULL!\n");
	dev = &pdev->dev;

	if (!hisi_fb_resource_initialized) {
		ret = hisi_fb_init_resource(pdev);
		return ret;
	}

	if (pdev->id < 0) {
		dev_err(dev, "WARNING: id=%d, name=%s!\n", pdev->id, pdev->name);
		return 0;
	}

	if (!hisi_fb_resource_initialized) {
		dev_err(dev, "fb resource not initialized!\n");
		return -EPERM;
	}

	if (pdev_list_cnt >= HISI_FB_MAX_DEV_LIST) {
		dev_err(dev, "too many fb devices, num=%d!\n", pdev_list_cnt);
		return -ENOMEM;
	}

	hisifd = platform_get_drvdata(pdev);
	dev_check_and_return(dev, !hisifd, -EINVAL, err, "hisifd is NULL\n");

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	ret = hisi_fb_register(hisifd);
	if (ret) {
		dev_err(dev, "fb%d hisi_fb_register failed, error=%d!\n", hisifd->index, ret);
		return ret;
	}

	/* config earlysuspend */

	pdev_list[pdev_list_cnt++] = pdev;

	/* set device probe status */
	hisi_fb_device_set_status1(hisifd);

	hisi_create_aod_wq(hisifd);

#if CONFIG_SH_AOD_ENABLE
	/* clear SCBAKDATA0 status */
	outp32(hisifd->sctrl_base + SCBAKDATA0, 0x0);
#endif

#ifdef CONFIG_HUAWEI_DUBAI_RGB_STATS
	/* enable rgb stats register */
	if (hisifd->index == PRIMARY_PANEL_IDX)
		outp32(hisifd->dss_base + DSS_DISP_GLB_OFFSET + RGB_HIST_EN, 0x1);
#endif

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return 0;
}


static int hisi_fb_remove(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (!pdev) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	HISI_FB_DEBUG("fb%d, +\n", hisifd->index);

	/* pm_runtime unregister */
	if (hisifd->pm_runtime_unregister != NULL)
		hisifd->pm_runtime_unregister(pdev);

	/* stop the device */
	if (hisi_fb_suspend_sub(hisifd) != 0)
		HISI_FB_ERR("fb%d hisi_fb_suspend_sub failed!\n", hisifd->index);

	hisifb_display_engine_unregister(hisifd);

	/* overlay destroy */
	hisi_overlay_deinit(hisifd);

	/* free framebuffer */
	hisifb_free_fb_buffer(hisifd);

	hisifb_destroy_buffer_client(hisifd);

	/* remove /dev/fb* */
	unregister_framebuffer(hisifd->fbi);

	hisifb_free_dumpdss(hisifd);

	/* unregister buf_sync */
	if (hisifd->buf_sync_unregister != NULL)
		hisifd->buf_sync_unregister(pdev);
	/* unregister vsync */
	if (hisifd->vsync_unregister != NULL)
		hisifd->vsync_unregister(pdev);
	/* unregister backlight */
	if (hisifd->bl_unregister != NULL)
		hisifd->bl_unregister(pdev);
	/* fb sysfs remove */
	if (hisifd->sysfs_remove_fnc != NULL)
		hisifd->sysfs_remove_fnc(hisifd->pdev);
	/* lcd check esd remove */
	if (hisifd->esd_unregister != NULL)
		hisifd->esd_unregister(hisifd->pdev);
	/* unregister debug */
	if (hisifd->debug_unregister != NULL)
		hisifd->debug_unregister(hisifd->pdev);
	/* remove video idle ctrl */
	if (hisifd->video_idle_ctrl_unregister != NULL)
		hisifd->video_idle_ctrl_unregister(hisifd->pdev);
	/* remove overlay online wirteback */
	if (hisifd->overlay_online_wb_unregister != NULL)
		hisifd->overlay_online_wb_unregister(hisifd->pdev);

	HISI_FB_DEBUG("fb%d, -\n", hisifd->index);

	return 0;
}

static int hisi_fb_suspend_sub(struct hisi_fb_data_type *hisifd)
{
	int ret;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}

	ret = hisi_fb_blank_sub(FB_BLANK_POWERDOWN, hisifd->fbi);
	if (ret) {
		HISI_FB_ERR("fb%d can't turn off display, error=%d!\n", hisifd->index, ret);
		return ret;
	}

	return 0;
}

static int hisi_fb_resume_sub(struct hisi_fb_data_type *hisifd)
{
	int ret;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}

	ret = hisi_fb_blank_sub(FB_BLANK_UNBLANK, hisifd->fbi);
	if (ret)
		HISI_FB_ERR("fb%d can't turn on display, error=%d!\n", hisifd->index, ret);

	return ret;
}


#if defined(CONFIG_PM) && !defined(CONFIG_HAS_EARLYSUSPEND)
static int hisi_fb_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!pdev) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	HISI_FB_INFO("fb%d, +\n", hisifd->index);

	console_lock();
	fb_set_suspend(hisifd->fbi, FBINFO_STATE_SUSPENDED);
	ret = hisi_fb_suspend_sub(hisifd);
	if (ret != 0) {
		HISI_FB_ERR("fb%d hisi_fb_suspend_sub failed, error=%d!\n", hisifd->index, ret);
		fb_set_suspend(hisifd->fbi, FBINFO_STATE_RUNNING);
	} else {
		pdev->dev.power.power_state = state;
	}
	console_unlock();

	HISI_FB_INFO("fb%d, -\n", hisifd->index);

	return ret;
}

static int hisi_fb_resume(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!pdev) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	HISI_FB_INFO("fb%d, +\n", hisifd->index);

	console_lock();
	ret = hisi_fb_resume_sub(hisifd);
	pdev->dev.power.power_state = PMSG_ON;
	fb_set_suspend(hisifd->fbi, FBINFO_STATE_RUNNING);
	console_unlock();

	HISI_FB_INFO("fb%d, -\n", hisifd->index);

	return ret;
}
#else
#define hisi_fb_suspend NULL
#define hisi_fb_resume NULL
#endif


/*******************************************************************************
 * pm_runtime
 */

#ifdef CONFIG_PM_SLEEP
static int hisi_fb_pm_suspend(struct device *dev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	if (!dev) {
		HISI_FB_ERR("NULL Poniter\n");
		return 0;
	}

	hisifd = dev_get_drvdata(dev);
	if (!hisifd)
		return 0;

	if (hisifd->index == EXTERNAL_PANEL_IDX || hisifd->index == AUXILIARY_PANEL_IDX)
		return 0;

	HISI_FB_INFO("fb%d, +\n", hisifd->index);

	ret = hisi_fb_suspend_sub(hisifd);
	if (ret != 0)
		HISI_FB_ERR("fb%d, failed to hisi_fb_suspend_sub! ret=%d\n", hisifd->index, ret);

	hisifd->media_common_composer_sr_refcount = 0;
	HISI_FB_INFO("fb%d, -\n", hisifd->index);

	return 0;
}

#endif

static void hisi_fb_shutdown(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev NULL Pointer\n");
		return;
	}

	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		if (pdev->id)
			HISI_FB_ERR("hisifd NULL Pointer,pdev->id=%d\n", pdev->id);

		return;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_DEBUG("fb%d do not shutdown\n", hisifd->index);
		return;
	}

	HISI_FB_INFO("fb%d shutdown +\n", hisifd->index);
	hisifd->fb_shutdown = true;

	ret = hisi_fb_blank_sub(FB_BLANK_POWERDOWN, hisifd->fbi);
	if (ret)
		HISI_FB_ERR("fb%d can't turn off display, error=%d!\n", hisifd->index, ret);

	HISI_FB_INFO("fb%d shutdown -\n", hisifd->index);
}


/*******************************************************************************
 *
 */
static const struct dev_pm_ops hisi_fb_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = hisi_fb_pm_suspend,
	.resume = NULL,
#endif
};

static const struct of_device_id hisi_fb_match_table[] = {
	{
		.compatible = DTS_COMP_FB_NAME,
		.data = NULL,
	},
	{},
};
MODULE_DEVICE_TABLE(of, hisi_fb_match_table);

static struct platform_driver hisi_fb_driver = {
	.probe = hisi_fb_probe,
	.remove = hisi_fb_remove,
	.suspend = hisi_fb_suspend,
	.resume = hisi_fb_resume,
	.shutdown = hisi_fb_shutdown,
	.driver = {
		.name = DEV_NAME_FB,
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(hisi_fb_match_table),
		.pm = &hisi_fb_dev_pm_ops,
	},
};

static int __init hisi_fb_init(void)
{
	int ret;

	ret = platform_driver_register(&hisi_fb_driver);
	if (ret) {
		HISI_FB_ERR("platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

module_init(hisi_fb_init);

MODULE_DESCRIPTION("Hisilicon Framebuffer Driver");
MODULE_LICENSE("GPL v2");

