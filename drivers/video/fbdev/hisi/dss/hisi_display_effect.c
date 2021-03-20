/* Copyright (c) 2013-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/

#include "hisi_display_effect.h"
#include "overlay/hisi_overlay_utils.h"
#include "hisi_dpe_utils.h"
#include "product/hisi_displayengine_utils.h"
#include "product/hisi_displayengine_effect.h"
#include <linux/hisi/hw_cmdline_parse.h>

#include <linux/miscdevice.h>
#include <linux/of_reserved_mem.h>

#include <linux/dma-mapping.h>
#include <linux/memory.h>
#include <uapi/linux/sched/types.h>

#include "dbv_curve_map/dbv_map_lg_ea9151_6_39__cmd_tft_1440_x_3120.h"
#include "dbv_curve_map/dbv_map_lg_nt37280_lion_6_53__cmd_tft_1176_x_2400.h"

#include <huawei_platform/inputhub/sensor_feima_ext.h>

#define MMAP_DEVICE_NAME "display_sharemem_map"
#define DTS_COMP_SHAREMEM_NAME "hisilicon,hisisharemem"
#define DEV_NAME_SHARE_MEM "display_share_mem"
#define XCC_COEF_LENGTH 12
#define DEBUG_EFFECT_LOG HISI_FB_ERR
#define round(x, y)  (((x) + (y) - 1) / (y))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wframe-larger-than="

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

int g_factory_gamma_enable = 0;
struct mutex g_rgbw_lock;
uint8_t* share_mem_virt = NULL;
phys_addr_t share_mem_phy = 0;
const int JDI_TD4336_RT8555_RGBW_ID = 14;
const int SHARP_TD4336_RT8555_RGBW_ID = 15;
const int LG_NT36772A_RT8555_RGBW_ID = 16;
const int g_bl_timeinterval = 16670000;
static struct dss_display_effect_xcc last_xcc_param = {
	0,
	{
		0x0, 0x8000, 0x0, 0x0,
		0x0, 0x0, 0x8000, 0x0,
		0x0, 0x0, 0x0, 0x8000
	}
};
const int blc_xcc_buf_count = 17;
const int display_effect_flag_max = 4;
struct effect_bl_buf {
	uint32_t blc_enable;
	int delta;
	struct dss_display_effect_xcc xcc_param;
	uint32_t dc_enable;
	uint32_t dimming_enable;
};

/*lint -e838 -e778 -e845 -e712 -e527 -e30 -e142 -e715 -e655 -e550 +e559*/

void hisi_display_effect_hiace_trigger_wq(struct hisi_fb_data_type *hisifd, bool is_idle_display)
{
	// Check whether the system enters the idle state. If no, need queue work
	// if yes, clear the current hiace interrupt and no need queue work
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is null\n");
	if (!is_idle_display) {
		queue_work(hisifd->hiace_end_wq, &hisifd->hiace_end_work);
	} else {
		/* clear hiace interrupt */
		outp32(hisifd->dss_base + DSS_HI_ACE_OFFSET + HIACE_INT_STAT, 0x1);
	}
}

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
void hisi_effect_hiace_trigger_wq(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is null\n");
#ifdef HIACE_SINGLE_MODE_SUPPORT
	// check if reading fna in isr first, if yes, no need queue work
	if (!hisi_hiace_single_mode_handle_isr(hisifd)) {
		hisi_display_effect_hiace_trigger_wq(hisifd, hisifd->video_idle_ctrl.idle_frame_display);
	}
#else
	hisi_display_effect_hiace_trigger_wq(hisifd, hisifd->is_idle_display);
#endif

}
#endif

static void hisi_effect_module_support(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	struct dss_effect *effect_ctrl = NULL;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return ;
	}

	pinfo = &(hisifd->panel_info);
	effect_ctrl = &(hisifd->effect_ctl);

	memset(effect_ctrl, 0, sizeof(struct dss_effect));

	effect_ctrl->acm_support = (pinfo->acm_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_ACM) != 0));

	effect_ctrl->ace_support = (pinfo->acm_ce_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_ACE) != 0));

	effect_ctrl->dither_support = (pinfo->dither_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_DITHER) != 0));

	effect_ctrl->lcp_xcc_support = (pinfo->xcc_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_LCP_XCC) != 0));

	effect_ctrl->lcp_gmp_support = (pinfo->gmp_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_LCP_GMP) != 0));

	effect_ctrl->lcp_igm_support = (pinfo->gamma_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_LCP_IGM) != 0));

	effect_ctrl->gamma_support = (pinfo->gamma_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_GAMA) != 0));

	effect_ctrl->hiace_support = (pinfo->hiace_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_HIACE) != 0));

	effect_ctrl->arsr1p_sharp_support = (pinfo->arsr1p_sharpness_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_POST_SCF) != 0));

	effect_ctrl->arsr2p_sharp_support = (pinfo->prefix_sharpness2D_support);

	effect_ctrl->post_xcc_support = (pinfo->post_xcc_support
		&& (HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_POST_XCC) != 0));
	effect_ctrl->dss_ready = true;
}

static int hisifb_effect_module_init_handler(void __user *argp)
{
	int ret;
	struct hisi_fb_data_type *hisifd_primary = NULL;

	hisifd_primary = hisifd_list[PRIMARY_PANEL_IDX];
	if (NULL == hisifd_primary) {
		HISI_FB_ERR("fb0 is not existed, return!\n");
		ret = -ENODEV;
		goto err_out;
	}

	if (argp == NULL) {
		HISI_FB_ERR("argp is null pointer\n");
		return -1;
	}

	ret = copy_to_user(argp, &(hisifd_primary->effect_ctl), sizeof(struct dss_effect));
	HISI_FB_INFO("fb0 effect_ctl:dss_ready=%d, arsr2p=%d, arsr1p=%d, acm=%d,"
		" ace=%d, hiace=%d, igm=%d, gmp=%d, xcc=%d, gamma=%d, dither=%d,"
		" post_xcc=%d\n",
		hisifd_primary->effect_ctl.dss_ready,
		hisifd_primary->effect_ctl.arsr2p_sharp_support,
		hisifd_primary->effect_ctl.arsr1p_sharp_support,
		hisifd_primary->effect_ctl.acm_support,
		hisifd_primary->effect_ctl.ace_support,
		hisifd_primary->effect_ctl.hiace_support,
		hisifd_primary->effect_ctl.lcp_igm_support,
		hisifd_primary->effect_ctl.lcp_gmp_support,
		hisifd_primary->effect_ctl.lcp_xcc_support,
		hisifd_primary->effect_ctl.gamma_support,
		hisifd_primary->effect_ctl.dither_support,
		hisifd_primary->effect_ctl.post_xcc_support);
	if (ret) {
		HISI_FB_ERR("failed to copy result of ioctl to user space.\n");
		goto err_out;
	}

err_out:
	return ret;
}

static int hisifb_effect_module_deinit_handler(const void __user *argp)
{
	int ret;
	struct dss_effect init_status;

	if (argp == NULL) {
		HISI_FB_ERR("argp is null pointer\n");
		return -1;
	}

	ret = copy_from_user(&init_status, argp, sizeof(struct dss_effect));
	if (ret) {
		HISI_FB_ERR("failed to copy data to kernel space.\n");
		goto err_out;
	}

err_out:
	return ret;
}

static int hisifb_effect_info_get_handler(void __user *argp)
{
	int ret = -EINVAL;
	struct dss_effect_info effect_info;
	struct hisi_fb_data_type *hisifd_primary = NULL;

	if (argp == NULL) {
		HISI_FB_ERR("argp is null pointer\n");
		return -1;
	}

	ret = copy_from_user(&effect_info, argp, sizeof(struct dss_effect_info));
	if (ret) {
		HISI_FB_ERR("failed to copy data from user.\n");
		goto err_out;
	}

	hisifd_primary = hisifd_list[PRIMARY_PANEL_IDX];
	if (hisifd_primary == NULL) {
		HISI_FB_ERR("fb0 is not existed, return!\n");
		ret = -ENODEV;
		goto err_out;
	}

	if (!hisifd_primary->panel_power_on) {
		HISI_FB_ERR("panel is power down, return!\n");
		ret = -EBADRQC;
		goto err_out;
	}

	if (!hisifd_primary->effect_ctl.dss_ready) {
		HISI_FB_ERR("dss is not ready\n");
		ret = -EBADRQC;
		goto err_out;
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_ARSR2P) {
		ret = hisi_effect_arsr2p_info_get(hisifd_primary, effect_info.arsr2p);
		if (ret) {
			HISI_FB_ERR("failed to get arsr2p info\n");
			goto err_out;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_ARSR1P) {
		ret = hisi_effect_arsr1p_info_get(hisifd_primary, effect_info.arsr1p);
		if (ret) {
			HISI_FB_ERR("failed to get arsr1p info\n");
			goto err_out;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_ACM) {
		ret = hisi_effect_acm_info_get(hisifd_primary, &effect_info.acm);
		if (ret) {
			HISI_FB_ERR("failed to get acm info\n");
			goto err_out;
		}
	}

	if (effect_info.modules & (DSS_EFFECT_MODULE_LCP_GMP | DSS_EFFECT_MODULE_LCP_IGM |
		DSS_EFFECT_MODULE_LCP_XCC | DSS_EFFECT_MODULE_POST_XCC)) {
		ret = hisi_effect_lcp_info_get(hisifd_primary, &effect_info.lcp);
		if (ret) {
			HISI_FB_ERR("failed to get lcp info\n");
			goto err_out;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_GAMMA) {
		ret = hisi_effect_gamma_info_get(hisifd_primary, &effect_info.gamma);
		if (ret) {
			HISI_FB_ERR("failed to get gamma info\n");
			goto err_out;
		}
	}

	ret = copy_to_user(argp, &effect_info, sizeof(struct dss_effect_info));
	if (ret) {
		HISI_FB_ERR("failed to copy result of ioctl to user space.\n");
		goto err_out;
	}

err_out:
	return ret;;
}

static int hisifb_effect_info_set_handler(const void __user *argp)
{
	int ret;
	struct dss_effect_info effect_info;
	struct hisi_fb_data_type *hisifd_primary = NULL;

	if (argp == NULL) {
		HISI_FB_ERR("argp is null pointer\n");
		return -1;
	}

	ret = copy_from_user(&effect_info, argp, sizeof(struct dss_effect_info));
	if (ret) {
		HISI_FB_ERR("failed to copy data to kernel space.\n");
		goto err_out;
	}

	if ((effect_info.disp_panel_id >= DISP_PANEL_NUM) ||
		(effect_info.disp_panel_id < 0)) {
		HISI_FB_ERR("disp_panel_id = %d is overflow.\n", effect_info.disp_panel_id);
		goto err_out;
	}
	hisifd_primary = hisifd_list[PRIMARY_PANEL_IDX];
	if (hisifd_primary == NULL) {
		HISI_FB_ERR("hisifd_primary is null or unexpected input fb\n");
		ret = -EBADRQC;
		goto err_out;
	}

	mutex_lock(&hisifd_primary->effect_lock);

	hisifd_primary->effect_info[effect_info.disp_panel_id].modules = effect_info.modules;

	if (effect_info.modules & DSS_EFFECT_MODULE_ARSR2P) {
		ret = hisi_effect_save_arsr2p_info(hisifd_primary, &effect_info);
		if (ret) {
			HISI_FB_ERR("failed to set arsr2p info\n");
			goto err_out_spin;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_ARSR1P) {
		ret = hisi_effect_save_arsr1p_info(hisifd_primary, &effect_info);
		if (ret) {
			HISI_FB_ERR("failed to set arsr1p info\n");
			goto err_out_spin;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_ACM) {
		ret = hisi_effect_save_acm_info(hisifd_primary, &effect_info);
		if (ret) {
			HISI_FB_ERR("failed to set acm info\n");
			goto err_out_spin;
		}
	}

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
	if (effect_info.modules & DSS_EFFECT_MODULE_POST_XCC) {
		ret = hisi_effect_save_post_xcc_info(hisifd_primary, &effect_info);
		if (ret) {
			HISI_FB_ERR("failed to set post_xcc info\n");
			goto err_out_spin;
		}
	}

	if ((effect_info.modules & DSS_EFFECT_MODULE_LCP_GMP) ||
		(effect_info.modules & DSS_EFFECT_MODULE_LCP_IGM) ||
		(effect_info.modules & DSS_EFFECT_MODULE_LCP_XCC) ||
		(effect_info.modules & DSS_EFFECT_MODULE_GAMMA)) {
		ret = hisi_effect_dpp_buf_info_set(hisifd_primary, &effect_info, false);
		if (ret) {
			HISI_FB_ERR("failed to set dpp info\n");
			goto err_out_spin;
		}
	}
#else
	if (effect_info.modules & DSS_EFFECT_MODULE_LCP_GMP) {
		ret = hisi_effect_gmp_info_set(hisifd_primary, &effect_info.lcp);
		if (ret) {
			HISI_FB_ERR("failed to set GMP info\n");
			goto err_out_spin;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_LCP_IGM) {
		ret = hisi_effect_igm_info_set(hisifd_primary, &effect_info.lcp);
		if (ret) {
			HISI_FB_ERR("failed to set IGM info\n");
			goto err_out_spin;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_LCP_XCC) {
		ret = hisi_effect_xcc_info_set(hisifd_primary, &effect_info.lcp);
		if (ret) {
			HISI_FB_ERR("failed to set XCC info\n");
			goto err_out_spin;
		}
	}

	if (effect_info.modules & DSS_EFFECT_MODULE_GAMMA) {
		ret = hisi_effect_gamma_info_set(hisifd_primary, &effect_info.gamma);
		if (ret) {
			HISI_FB_ERR("failed to set gama info\n");
			goto err_out_spin;
		}
	}
#endif
	/*the display effect is not allowed to set reg when the partical update*/
	if (hisifd_primary->display_effect_flag < 5)
		hisifd_primary->display_effect_flag = 4;

err_out_spin:
	mutex_unlock(&hisifd_primary->effect_lock);

	HISI_FB_DEBUG("fb%d, modules = 0x%x, -.\n", hisifd_primary->index, effect_info.modules);

err_out:
	return ret;;
}

static int hisi_display_effect_ioctl_handler(struct hisi_fb_data_type *hisifd, unsigned int cmd, void __user *argp)
{
	int ret = -EINVAL;

	if (argp == NULL || hisifd == NULL) {
		HISI_FB_ERR("[effect]NULL pointer of argp or hisifd\n");
		goto err_out;
	}

	HISI_FB_DEBUG("[effect]fb%d, +\n", hisifd->index);

	switch (cmd) {
	case HISIFB_EFFECT_MODULE_INIT:
		ret = hisifb_effect_module_init_handler(argp);
		break;
	case HISIFB_EFFECT_MODULE_DEINIT:
		ret = hisifb_effect_module_deinit_handler(argp);
		break;
	case HISIFB_EFFECT_INFO_GET:
		ret = hisifb_effect_info_get_handler(argp);
		break;
	case HISIFB_EFFECT_INFO_SET:
		ret = hisifb_effect_info_set_handler(argp);
		break;
	default:
		HISI_FB_ERR("[effect]unknown cmd id\n");
		ret = -ENOSYS;
		break;
	};

	HISI_FB_DEBUG("[effect]fb%d, -\n", hisifd->index);

err_out:
	return ret;
}

static int effect_thread_fn(void *data)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	HISI_FB_INFO("[effect] thread + \n");

	hisifd = (struct hisi_fb_data_type *)data;
	if (!hisifd) {
		HISI_FB_ERR("hisifd is null pointer\n");
		return -1;
	}

	while(!kthread_should_stop()) {
		ret = wait_event_interruptible(hisifd->effect_wq, hisifd->effect_update); //lint !e578
		if (ret) {
			HISI_FB_ERR("wait_event_interruptible wrong \n");
			continue;
		}
		hisifd->effect_update = false;
#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
		hisifb_effect_dpp_config(hisifd, false);
#endif
	}

	HISI_FB_INFO("[effect] thread - \n");

	return 0;
}

int hisi_display_effect_thread_init(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->effect_thread)
		return 0;

	struct sched_param param = { .sched_priority = 1 };
	hisifd->effect_thread = kthread_create(effect_thread_fn, hisifd, "effect_thread");

	if (IS_ERR(hisifd->effect_thread)) {
		HISI_FB_ERR("Unable to start kernel effect_thread.\n");
		hisifd->effect_thread = NULL;
		return -EINVAL;
	}

	init_waitqueue_head(&hisifd->effect_wq);
	sched_setscheduler_nocheck(hisifd->effect_thread, SCHED_FIFO, &param);
	wake_up_process(hisifd->effect_thread);
	return 0;
}

void hisi_display_effect_init(struct hisi_fb_data_type *hisifd)
{
	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return ;
	}

	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);

	if (PRIMARY_PANEL_IDX == hisifd->index) {
		if (hisifd->panel_info.p3_support)
			hisi_display_effect_thread_init(hisifd);

		hisifd->display_effect_ioctl_handler = NULL;
		memset(hisifd->effect_updated_flag, 0, sizeof(hisifd->effect_updated_flag));
		mutex_init(&hisifd->effect_lock);

		hisi_effect_module_support(hisifd);
#ifdef CONFIG_HISI_FB_ALSC
		hisi_alsc_init(hisifd);
#endif
	} else if (AUXILIARY_PANEL_IDX == hisifd->index) {
		hisifd->display_effect_ioctl_handler = hisi_display_effect_ioctl_handler;
	} else {
		hisifd->display_effect_ioctl_handler = NULL;
	}

	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);
}
void hisi_dss_effect_set_reg(struct hisi_fb_data_type *hisifd)
{
	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return ;
	}

	if (PRIMARY_PANEL_IDX != hisifd->index) {
		return;
	}

	if (mutex_trylock(&hisifd->effect_lock) != 0) {
		if (hisifd->panel_info.smart_color_mode_support == 0) {
			hisi_effect_acm_set_reg(hisifd);
			hisi_effect_lcp_set_reg(hisifd);
		}
#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
		hisi_effect_post_xcc_set_reg(hisifd);
#endif
		hisi_effect_gamma_set_reg(hisifd);
		mutex_unlock(&hisifd->effect_lock);
	} else {
		HISI_FB_DEBUG("dss effect param is being updated, delay set reg to next frame!\n");
	}
	return;
}

static inline struct hisi_fb_data_type *display_engine_check_input_and_get_hisifd(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (NULL == info) {
		HISI_FB_ERR("[effect] info is NULL\n");
		return NULL;
	}

	if (NULL == argp) {
		HISI_FB_ERR("[effect] argp is NULL\n");
		return NULL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return NULL;
	}

	return hisifd;
}

static void display_engine_amoled_mod_set(
	struct hisi_fb_data_type *hisifd,
	display_engine_param_t *de_param)
{
	uint32_t hisifd_modules = 0;

	mutex_lock(&hisifd->de_info.param_lock);
	hisifd_modules = hisifd->de_param.modules;
	hisifd->de_param = *de_param;
	hisifd_modules |= (*de_param).modules;
	hisifd->de_param.modules = hisifd_modules;
	mutex_unlock(&hisifd->de_info.param_lock);
}

static int display_engine_amoled_sync_param_set(
	struct hisi_fb_data_type *hisifd,
	display_engine_amoled_param_sync_t *param)
{
	int ret = 0;
	int num = 0;
	struct hisi_fb_panel_data *pdata = NULL;
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}
	if (hisifd->pdev == NULL) {
		HISI_FB_ERR("[effect] pdev is NULL Pointer\n");
		return -1;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (pdata == NULL) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	if (param->high_dbv_map == NULL ||
		param->map_size != sizeof(hisifd->de_info.dbv_map))
		goto OUT;

	ret = copy_from_user(hisifd->de_info.dbv_map,
		param->high_dbv_map, param->map_size);
	if (ret != 0) {
		HISI_FB_ERR("[effect] copy dbv map fail\n");
		goto OUT;
	}
	for (num = 1; num < MAP_LUMLUT_LENGTH; num++) {
		if (hisifd->de_info.dbv_map[num] == 0)
			goto OUT;
		if (hisifd->de_info.dbv_map[num - 1] >
			hisifd->de_info.dbv_map[num])
			HISI_FB_DEBUG("[effect] map is no monotonous\n");
		return ret;
	}
OUT:
	HISI_FB_INFO("[effect] use default dbv map\n");
	for (num = 0; num < MAP_LUMLUT_LENGTH; num++)
		hisifd->de_info.dbv_map[num] = num;
	return ret;
}

static int display_engine_ud_fp_backlight_param_set(
        struct hisi_fb_data_type *hisifd,
        display_engine_sync_ud_fingerprint_backlight_t *param)
{
	int ret = 0;
	struct hisi_fb_panel_data *pdata = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (hisifd->pdev == NULL) {
		HISI_FB_ERR("[effect] pdev is NULL Pointer\n");
		return -1;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (pdata == NULL) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	if ((int)param->current_level == ((int)param->hbm_level + 1)) {
		hisifd->hbm_need_to_open = true;
		hisifd->ud_fp_hbm_level = (int)param->hbm_level;
		HISI_FB_INFO("hisifd->hbm_need_to_open = true\n");
		return ret;
	}

	if ((param->hbm_level < hisifd->panel_info.bl_min) ||
		(param->hbm_level > hisifd->panel_info.bl_max) ||
		(param->current_level < hisifd->panel_info.bl_min) ||
		(param->current_level > hisifd->panel_info.bl_max)) {
		HISI_FB_ERR("invalid data, hbm_level(%d) current_level(%d)\n",
			param->hbm_level, param->current_level);
		return -EINVAL;
	}

	hisifd->ud_fp_scene = (int)param->scene_info;
	hisifd->ud_fp_hbm_level = (int)param->hbm_level;
	hisifd->ud_fp_current_level = (int)param->current_level;

	HISI_FB_INFO("[effect] scene:%d hbm_level:%d current_level:%d\n",
		hisifd->ud_fp_scene, hisifd->ud_fp_hbm_level,
		hisifd->ud_fp_current_level);
	return ret;
}

static int display_engine_blc_param_set(struct hisi_fb_data_type *hisifd, display_engine_blc_param_t *param)
{
	struct hisi_fb_panel_data *pdata = NULL;
	ktime_t current_bl_timestamp = ktime_get();
	int ret = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -EINVAL;
	}

	if (NULL == param) {
		HISI_FB_ERR("[effect] param is NULL\n");
		return -EINVAL;
	}

	hisifd->de_info.blc_enable = (param->enable == 1) ? true : false;
	hisifd->de_info.blc_delta = param->delta;

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("pdata is NULL");
		return -EINVAL;
	}
	down(&hisifd->blank_sem_effect);

	if (abs((int)(ktime_to_ns(hisifd->backlight.bl_timestamp) - ktime_to_ns(current_bl_timestamp))) > g_bl_timeinterval) {
		HISI_FB_DEBUG("[effect] delta:%d bl:%d enable:%d set\n", hisifd->de_info.blc_delta, hisifd->bl_level, hisifd->de_info.blc_enable);
		if (hisifd->bl_level > 0) {
			down(&hisifd->brightness_esd_sem);
			hisifb_set_backlight(hisifd, hisifd->bl_level, true);
			up(&hisifd->brightness_esd_sem);
		}
	} else {
		HISI_FB_DEBUG("[effect] delta:%d bl:%d enable:%d skip\n", hisifd->de_info.blc_delta, hisifd->bl_level, hisifd->de_info.blc_enable);
		ret = 2; // This delta has been skipped, and hal may resent the command.
	}

	up(&hisifd->blank_sem_effect);
	return ret;
}

int display_engine_ddic_color_param_set(struct hisi_fb_data_type *hisifd, display_engine_ddic_color_param_t *param)
{
	int ret = 0;
	struct hisi_fb_panel_data *pdata = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (hisifd->pdev == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (pdata == NULL) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	hisifd->de_info.ddic_color_mode = param->ddic_color_mode;

	down(&hisifd->blank_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
		ret = -1;
		goto err_out;
	}

	if (pdata->lcd_ce_mode_store != NULL) {
		char buf[8];
		int count = 0;
		hisifb_activate_vsync(hisifd);
		count = snprintf(buf, sizeof(buf), "%d", param->ddic_color_mode);
		pdata->lcd_ce_mode_store(hisifd->pdev, buf, count);
		hisifb_deactivate_vsync(hisifd);
	}

err_out:
	up(&hisifd->blank_sem);

	return ret;
}

int display_engine_ddic_cabc_param_set(struct hisi_fb_data_type *hisifd, display_engine_ddic_cabc_param_t *param)
{
	int ret = 0;
	struct hisi_fb_panel_data *pdata = NULL;
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (hisifd->pdev == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	hisifd->de_info.ddic_cabc_mode = param->ddic_cabc_mode;
	down(&hisifd->blank_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
		ret = -1;
		goto err_out;
	}

	if (pdata->lcd_cabc_mode_store != NULL) {
		char buf[8];
		int count = 0;
		hisifb_activate_vsync(hisifd);
		count = snprintf(buf, sizeof(buf), "%d", param->ddic_cabc_mode);
		pdata->lcd_cabc_mode_store(hisifd->pdev, buf, count);
		hisifb_deactivate_vsync(hisifd);
	}

err_out:
	up(&hisifd->blank_sem);
	return ret;
}

int display_engine_ddic_rgbw_param_set(struct hisi_fb_data_type *hisifd, display_engine_ddic_rgbw_param_t *param)
{
	int ret = 0;
	struct hisi_fb_panel_data *pdata = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	mutex_lock(&g_rgbw_lock);
	hisifd->de_info.ddic_panel_id = param->ddic_panel_id;
	hisifd->de_info.ddic_rgbw_mode = param->ddic_rgbw_mode;
	hisifd->de_info.rgbw_saturation_control = param->rgbw_saturation_control;
	hisifd->de_info.frame_gain_speed = param->frame_gain_speed;
	hisifd->de_info.pixel_gain_speed = param->pixel_gain_speed;
	if((param->ddic_panel_id != LG_NT36772A_RGBW_ID) && (param->ddic_panel_id != LG_NT36772A_RGBW_ID_HMA)
		&& (param->ddic_panel_id != BOE_HX83112E_RGBW_ID_HMA) && (param->ddic_panel_id != JDI_TD4336_RT8555_RGBW_ID)
		&& (param->ddic_panel_id != SHARP_TD4336_RT8555_RGBW_ID) && (param->ddic_panel_id != LG_NT36772A_RT8555_RGBW_ID)) {
		hisifd->de_info.frame_gain_limit = param->frame_gain_limit;
		hisifd->de_info.pwm_duty_gain = param->pwm_duty_gain;
		hisifd->de_info.color_distortion_allowance = param->color_distortion_allowance;
		hisifd->de_info.pixel_gain_limit = param->pixel_gain_limit;
	}
	mutex_unlock(&g_rgbw_lock);
	return ret;
}

int display_engine_amoled_algo_param_set(struct hisi_fb_data_type *hisifd, display_engine_amoled_param_t *param)
{
	int ret = 0;

	if(NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}
	if(NULL == param) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	down(&hisifd->blank_sem);
	hisifd->de_info.amoled_param.HBMEnable = param->HBMEnable;
	hisifd->de_info.amoled_param.amoled_diming_enable = param->amoled_diming_enable;
	hisifd->de_info.amoled_param.HBM_Threshold_BackLight = param->HBM_Threshold_BackLight;
	hisifd->de_info.amoled_param.HBM_Min_BackLight = param->HBM_Min_BackLight;
	hisifd->de_info.amoled_param.HBM_Max_BackLight = param->HBM_Max_BackLight;
	hisifd->de_info.amoled_param.HBM_MinLum_Regvalue = param->HBM_MinLum_Regvalue;
	hisifd->de_info.amoled_param.HBM_MaxLum_Regvalue = param->HBM_MaxLum_Regvalue;
	hisifd->de_info.amoled_param.Hiac_DBVThres = param->Hiac_DBVThres;
	hisifd->de_info.amoled_param.Hiac_DBV_XCCThres = param->Hiac_DBV_XCCThres;
	hisifd->de_info.amoled_param.Hiac_DBV_XCC_MinThres = param->Hiac_DBV_XCC_MinThres;
	hisifd->de_info.amoled_param.Lowac_DBVThres = param->Lowac_DBVThres;
	hisifd->de_info.amoled_param.Lowac_DBV_XCCThres = param->Lowac_DBV_XCCThres;
	hisifd->de_info.amoled_param.Lowac_DBV_XCC_MinThres = param->Lowac_DBV_XCC_MinThres;
	hisifd->de_info.amoled_param.Lowac_Fixed_DBVThres= param->Lowac_Fixed_DBVThres;
	hisifd->de_info.amoled_param.dc_lowac_dbv_thre = param->dc_lowac_dbv_thre;
	hisifd->de_info.amoled_param.dc_lowac_fixed_dbv_thres = param->dc_lowac_fixed_dbv_thres;
	hisifd->de_info.amoled_param.dc_backlight_delay_us = param->dc_backlight_delay_us;
	hisifd->de_info.amoled_param.dc_lowac_dbv_thres_low =
		param->dc_lowac_dbv_thres_low;
	up(&hisifd->blank_sem);
	HISI_FB_DEBUG("[effect] first screen on ! HBM_Max_BackLight:%d Hiac_DBVThres: %d HBMEnable: %d amoled_diming_enable : %d\n",param->HBM_Max_BackLight,param->Hiac_DBVThres,param->HBMEnable,param->amoled_diming_enable);
	HISI_FB_DEBUG("[effect] first screen on ! Lowac_DBVThres:%d Lowac_DBV_XCCThres: %d Lowac_DBV_XCC_MinThres: %d Lowac_Fixed_DBVThres : %d\n",param->Lowac_DBVThres,param->Lowac_DBV_XCCThres,param->Lowac_DBV_XCC_MinThres,param->Lowac_Fixed_DBVThres);
	HISI_FB_DEBUG("[effect] dc_backlight_delay_us = %d", hisifd->de_info.amoled_param.dc_backlight_delay_us);
	HISI_FB_DEBUG("[effect] dc_lowac_dbv_thres_low = %d",
		hisifd->de_info.amoled_param.dc_lowac_dbv_thres_low);
	return ret;
}

int hisifb_display_engine_blank(int blank_mode, struct fb_info *info)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_fb_panel_data *pdata = NULL;

	if (NULL == info) {
		HISI_FB_ERR("info is NULL\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -EINVAL;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (blank_mode == FB_BLANK_UNBLANK) {
			if (pdata->lcd_cabc_mode_store && hisifd->de_info.ddic_cabc_mode) {
				char buf[8];
				int count = 0;
				hisifb_activate_vsync(hisifd);
				count = snprintf(buf, sizeof(buf), "%d", hisifd->de_info.ddic_cabc_mode);
				pdata->lcd_cabc_mode_store(hisifd->pdev, buf, count);
				hisifb_deactivate_vsync(hisifd);
			}
		}
	}
	return 0;
}

void hisifb_dbv_curve_mapped_init(struct hisi_fb_data_type *hisifd)
{
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return;
	}

	if (!hisifd->panel_info.dbv_curve_mapped_support) {
		HISI_FB_INFO("[effect] no need dbv_curve_mapped");
		return;
	}

	if (!hisifd->panel_info.panel_name)
		return;

	if (!strcmp(hisifd->panel_info.panel_name, "LG_EA9151 6.39' CMD TFT 1440 x 3120")) {
		if (!strcmp(hisifd->panel_info.lcd_panel_version, " VER:V4")) {
			hisifd->panel_info.dbv_map_index = 0;
			hisifd->panel_info.is_dbv_need_mapped = 1;
		} else if (!strcmp(hisifd->panel_info.lcd_panel_version, " VER:VN1")) {
			hisifd->panel_info.dbv_map_index = 1;
			hisifd->panel_info.is_dbv_need_mapped = 1;
		} else if (!strcmp(hisifd->panel_info.lcd_panel_version, " VER:VN2")) {
			hisifd->panel_info.dbv_map_index= 2;
			hisifd->panel_info.is_dbv_need_mapped = 1;
		}
		hisifd->panel_info.dbv_map_points_num = DBV_MAP_10BIT;
		hisifd->panel_info.dbv_map_curve_pointer =
			&dbv_curve_nolinear_to_linear_map_laya[0][0];
	}
	if (!strcmp(hisifd->panel_info.panel_name, "LG_NT37280_LION 6.53' CMD TFT 1176 x 2400")) {
		if (!strcmp(hisifd->panel_info.lcd_panel_version, " VER:VN1")) {
			hisifd->panel_info.dbv_map_index = 0;
			hisifd->panel_info.is_dbv_need_mapped = 1;
		} else if (!strcmp(hisifd->panel_info.lcd_panel_version, " VER:VN2")) {
			hisifd->panel_info.dbv_map_index= 1;
			hisifd->panel_info.is_dbv_need_mapped = 1;
		}
		hisifd->panel_info.dbv_map_points_num = DBV_MAP_12BIT;
		hisifd->panel_info.dbv_map_curve_pointer =
			&dbv_curve_nolinear_to_linear_map_lion[0][0];
	}
	HISI_FB_INFO("[effect] hisifd index is %d need_map = %d\n",
		hisifd->panel_info.dbv_map_index,
		hisifd->panel_info.is_dbv_need_mapped);
}

int hisifb_display_engine_init(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	display_engine_t de;
	int ret = 0;

	if (NULL == info) {
		HISI_FB_ERR("[effect] info is NULL\n");
		return -EINVAL;
	}

	if (NULL == argp) {
		HISI_FB_ERR("[effect] argp is NULL\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -EINVAL;
	}

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	if (!hisifd->de_info.is_ready) {
		hisifd->de_info.is_ready = true;
	}

	hisifb_dbv_curve_mapped_init(hisifd);

	de.blc_support = 1;
	de.ddic_cabc_support = pdata->lcd_cabc_mode_store == NULL ? 0 : 1;
	de.ddic_rgbw_support = pdata->lcd_rgbw_set_func == NULL ? 0 : 1;
	de.ddic_hbm_support = pdata->lcd_hbm_set_func == NULL ? 0 : 1;
	de.ddic_color_support = pdata->lcd_ce_mode_store == NULL ? 0 : 1;
	de.ddic_color_rectify_support = pdata->lcd_color_param_get_func == NULL ? 0 : 1;

	ret = (int)copy_to_user(argp, &de, sizeof(display_engine_t));
	if (ret) {
		HISI_FB_ERR("[effect] copy_to_user(display_engine_t) failed! ret=%d.\n", ret);
		goto ERR_OUT;
	}

ERR_OUT:
	return ret;
}

int hisifb_display_engine_deinit(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = display_engine_check_input_and_get_hisifd(info, argp);
	if (NULL == hisifd) {
		return -EINVAL;
	}

	if (hisifd->de_info.is_ready) {
		hisifd->de_info.is_ready = false;
	}

	return 0;
}

int hisifb_display_engine_param_get(struct fb_info *info, void __user *argp)
{
	return display_engine_param_get(info, argp);
}

int hisifb_param_check(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = display_engine_check_input_and_get_hisifd(info, argp);
	if (NULL == hisifd) {
		return -EINVAL;
	}

	if (!hisifd->de_info.is_ready) {
		HISI_FB_ERR("[effect] display engine has not been initialized!\n");
		return -EINVAL;
	}

	return 0;
}

int hisifb_runmode_check(display_engine_param_t* de_param)
{
	if(de_param == NULL){
		HISI_FB_ERR("de_param is null pointer\n");
		return -1;
	}
	if (de_param->modules & DISPLAY_ENGINE_DDIC_RGBW ||
		de_param->modules & DISPLAY_ENGINE_HBM||
		de_param->modules & DISPLAY_ENGINE_BLC) {
		return 0;
	} else if (runmode_is_factory()) {
		return -EINVAL;
	}

	return 0;
}

int hisifb_display_engine_param_set(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;
	display_engine_param_t de_param;
	int ret = 0;

	if(argp == NULL){
		HISI_FB_ERR("argp is null pointer\n");
		return -EINVAL;
	}
	if (hisifb_param_check(info, argp)) {
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if(hisifd == NULL){
		HISI_FB_ERR("hisifd is null pointer\n");
		return -1;
	}

	ret = (int)copy_from_user(&de_param, argp, sizeof(display_engine_param_t));
	if (ret) {
		HISI_FB_ERR("[effect] copy_from_user(param) failed! ret=%d.\n", ret);
		return ret;
	}

	if (hisifb_runmode_check(&de_param)) {
		return -EINVAL;
	}
	display_engine_amoled_mod_set(hisifd, &de_param);
	if (de_param.modules & DISPLAY_ENGINE_AMOLED_SYNC) {
		ret = display_engine_amoled_sync_param_set(hisifd,
			&de_param.amoled_param_sync);
		if (ret < 0) {
			HISI_FB_ERR("AMOLED_SYNC set fail");
			return ret;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_UD_FINGERPRINT_BACKLIGHT) {
		ret = display_engine_ud_fp_backlight_param_set(hisifd,
                        &de_param.ud_fingerprint_backlight);
		if (ret < 0) {
			HISI_FB_ERR("UD_FP_BACKLIGHT failed, ret=%d\n", ret);
			return ret;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_MAINTENANCE)
		hisifd->de_info.is_maintaining = de_param.is_maintaining;

	if (hisifd->display_engine_wq != NULL) {
		queue_work(hisifd->display_engine_wq, &hisifd->display_engine_work);
	}

	return ret;
}

void hisifb_display_engine_workqueue_handler(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	display_engine_param_t de_param;
	int ret = 0;

	if (NULL == work) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return;
	}

	hisifd = container_of(work, struct hisi_fb_data_type, display_engine_work);
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return;
	}

	mutex_lock(&hisifd->de_info.param_lock);
	de_param = hisifd->de_param;
	hisifd->de_param.modules = 0;
	mutex_unlock(&hisifd->de_info.param_lock);

	if (de_param.modules & DISPLAY_ENGINE_BLC) {
		ret = display_engine_blc_param_set(hisifd, &de_param.blc);
		if (ret < 0) {
			HISI_FB_ERR("[effect] failed to set BLC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_DDIC_CABC) {
		ret = display_engine_ddic_cabc_param_set(hisifd, &de_param.ddic_cabc);
		if (ret) {
			HISI_FB_ERR("[effect] failed to set DDIC_CABC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_DDIC_RGBW) {
		ret = display_engine_ddic_rgbw_param_set(hisifd, &de_param.ddic_rgbw);
		if (ret) {
			HISI_FB_ERR("[effect] failed to set DDIC_RGBW, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_DDIC_COLOR) {
		ret = display_engine_ddic_color_param_set(hisifd, &de_param.ddic_color);
		if (ret) {
			HISI_FB_ERR("[effect] failed to set DDIC_COLOR, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_HBM) {
		ret = display_engine_hbm_param_set(hisifd, &de_param.hbm);
		if (ret) {
			HISI_FB_INFO("[effect] failed to set HBM, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_AMOLED_ALGO) {
		ret = display_engine_amoled_algo_param_set(hisifd, &de_param.amoled_param);
		if (ret) {
			HISI_FB_ERR("[effect] failed to set Amoled, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param.modules & DISPLAY_ENGINE_FLICKER_DETECTOR) {
		bl_flicker_detector_init(de_param.flicker_detector_config);
	}

	if (de_param.modules & DISPLAY_ENGINE_DDIC_IRC) {
		ret = display_engine_ddic_irc_set(hisifd, &de_param.ddic_irc);
		if (ret) {
			HISI_FB_INFO("[effect] failed to set IRC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

ERR_OUT:
	return;
}

#ifndef DISPLAY_ENGINE_DEBUG
#define DISPLAY_ENGINE_DEBUG(product_type, msg)
#endif

int hisifb_display_effect_resolve_bl_buf(const char *buf,
	struct effect_bl_buf *resolved_buf)
{
	int ret_count;
	ret_count = sscanf(buf,
		"%u:%d:%u:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%u:%u",
		&(resolved_buf->blc_enable),
		&(resolved_buf->delta),
		&(resolved_buf->xcc_param.xcc_enable),
		&(resolved_buf->xcc_param.xcc_table[0]),
		&(resolved_buf->xcc_param.xcc_table[1]),
		&(resolved_buf->xcc_param.xcc_table[2]),
		&(resolved_buf->xcc_param.xcc_table[3]),
		&(resolved_buf->xcc_param.xcc_table[4]),
		&(resolved_buf->xcc_param.xcc_table[5]),
		&(resolved_buf->xcc_param.xcc_table[6]),
		&(resolved_buf->xcc_param.xcc_table[7]),
		&(resolved_buf->xcc_param.xcc_table[8]),
		&(resolved_buf->xcc_param.xcc_table[9]),
		&(resolved_buf->xcc_param.xcc_table[10]),
		&(resolved_buf->xcc_param.xcc_table[11]),
		&(resolved_buf->dc_enable),
		&(resolved_buf->dimming_enable));
	HISI_FB_DEBUG("[effect] ret_count = %d\n", ret_count);
	return ret_count;
}

static void hisifb_display_effect_handle_xcc(struct hisi_fb_data_type *hisifd,
	struct effect_bl_buf *resolved_buf, const bool dc_enable_changed)
{
	if ((memcmp(&last_xcc_param, &(resolved_buf->xcc_param),
		sizeof(last_xcc_param)) == 0) && !dc_enable_changed)
		return;
#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V330) || \
	defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
	DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
		"[DE] xcc_info_set_kernel +\n");
	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
		int panel_id = hisifd->panel_info.disp_panel_id;
		HISI_FB_DEBUG("[effect] enable=%d xcc[1]=%d [6]=%d [11]=%d\n", resolved_buf->xcc_param.xcc_enable,
			resolved_buf->xcc_param.xcc_table[1],
			resolved_buf->xcc_param.xcc_table[6],
			resolved_buf->xcc_param.xcc_table[11]); /* Print R,G,B value of xcc with index 1,6,11 */
		if (hisi_effect_xcc_info_set_kernel_multiple(hisifd, &(resolved_buf->xcc_param), panel_id) != 0) {
			hisifd->de_info.amoled_param.dc_brightness_dimming_enable = 0;
			hisifd->de_info.amoled_param.amoled_enable_from_hal = 0;
			HISI_FB_ERR("[effect] hisi_effect_xcc_info_set error\n");
		}
		panel_id = (panel_id == DISPLAY_INNEL_PANEL_ID) ? DISPLAY_OUTER_PANEL_ID : DISPLAY_INNEL_PANEL_ID;
		if (hisi_effect_xcc_info_set_kernel_multiple(hisifd, &(resolved_buf->xcc_param), panel_id) != 0)
			HISI_FB_ERR("[effect] hisi_effect_xcc_info_set_kernel(panel_id=%d) error", panel_id);
#endif
	} else {
		if (hisi_effect_xcc_info_set_kernel(hisifd,
			&(resolved_buf->xcc_param)) != 0) {
			hisifd->de_info.amoled_param.dc_brightness_dimming_enable = 0;
			hisifd->de_info.amoled_param.amoled_enable_from_hal = 0;
			HISI_FB_ERR("[effect] hisi_effect_xcc_info_set error\n");
		}
	}
	DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
		"[DE] xcc_info_set_kernel -\n");
#endif
	(void)memcpy(&last_xcc_param, &(resolved_buf->xcc_param),
		sizeof(last_xcc_param));
}

static void hisifb_display_effect_handle_dc_sync(
	struct hisi_fb_data_type *hisifd,
	struct effect_bl_buf *resolved_buf, const bool dc_enable_changed)
{
	struct hisi_fb_data_type *hisifd_primary =
		hisifd_list[PRIMARY_PANEL_IDX];
	if (hisifd_primary == NULL) {
		HISI_FB_ERR("hisifd_primary is NULL pointer, return!\n");
		return;
	}

	if (dc_enable_changed) {
		hisifd_primary->display_effect_flag = display_effect_flag_max;
		hisifd->de_info.amoled_param.dc_brightness_dimming_enable =
			resolved_buf->dc_enable;
		hisifd->de_info.amoled_param.amoled_enable_from_hal =
			resolved_buf->dimming_enable;
		if (hisifd->bl_level > 0 || resolved_buf->dc_enable > 0 ||
			resolved_buf->dimming_enable > 0) {
			delta_bl_delayed = resolved_buf->delta;
			blc_enable_delayed = resolved_buf->blc_enable;
		} else {
			hisifd->de_info.amoled_param.
				dc_brightness_dimming_enable_real =
				resolved_buf->dc_enable;
			hisifd->de_info.amoled_param.amoled_diming_enable =
				resolved_buf->dimming_enable;
			hisifd->de_info.blc_enable =
				(bool)resolved_buf->blc_enable;
			hisifd->de_info.blc_delta = resolved_buf->delta;
		}
	} else {
		hisifd->de_info.blc_enable = (bool)resolved_buf->blc_enable;
		hisifd->de_info.blc_delta = resolved_buf->delta;
	}
}

static ssize_t display_engine_effect_set_delta(struct hisi_fb_data_type *hisifd,
	ssize_t ret, bool dc_enable_changed, bool delta_changed)
{
	static bool need_trigger_next_time = false;
	if (abs((int)(ktime_to_ns(hisifd->backlight.bl_timestamp) -
		ktime_to_ns(ktime_get()))) > g_bl_timeinterval) {
		HISI_FB_DEBUG("[effect] delta:%d bl:%d enable:%d, return %d\n",
			hisifd->de_info.blc_delta, hisifd->bl_level,
			hisifd->de_info.blc_enable, (uint32_t)ret);
		if (hisifd->bl_level > 0 && !dc_enable_changed &&
			(delta_changed || need_trigger_next_time)) {
			need_trigger_next_time = false;
			DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
				"[DE] brightness_sem +\n");
			down(&hisifd->brightness_esd_sem);
			DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type
				, "[DE] brightness_sem -\n");
			hisifb_set_backlight(hisifd, hisifd->bl_level, true);
			DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
				"[DE] set_backlight -\n");
			up(&hisifd->brightness_esd_sem);
		}
	} else {
		HISI_FB_DEBUG("[effect] delta:%d bl:%d skip,return 0\n",
			hisifd->de_info.blc_delta, hisifd->bl_level);
		need_trigger_next_time = true;
		ret = 0;
		// This delta has been skipped, and hal may resent the command.
	}
	return ret;
}

static ssize_t display_engine_effect_handle_delta(
	struct hisi_fb_data_type *hisifd, ssize_t ret_count,
	struct effect_bl_buf *resolved_buf, ssize_t ret)
{
	bool delta_changed = hisifd->de_info.blc_delta != resolved_buf->delta;
	bool dc_enable_changed = false;
	if (ret_count == blc_xcc_buf_count) {
		dc_enable_changed = (resolved_buf->dc_enable !=
			hisifd->de_info.amoled_param.
			dc_brightness_dimming_enable_real) ||
			(resolved_buf->dimming_enable !=
			hisifd->de_info.amoled_param.
			amoled_diming_enable);
		hisifb_display_effect_handle_xcc(hisifd,
			resolved_buf, dc_enable_changed);
		hisifb_display_effect_handle_dc_sync(hisifd,
			resolved_buf, dc_enable_changed);
	} else {
		hisifd->de_info.blc_enable = (bool)resolved_buf->blc_enable;
		hisifd->de_info.blc_delta = resolved_buf->delta;
		hisifd->de_info.amoled_param.dc_brightness_dimming_enable = 0;
	}
	HISI_FB_DEBUG("[effect] blc_enable = %d, blc_delta = %d",
		hisifd->de_info.blc_enable, hisifd->de_info.blc_delta);
	return display_engine_effect_set_delta(hisifd, ret, dc_enable_changed,
		delta_changed);
}

ssize_t hisifb_display_effect_bl_ctrl_store(struct fb_info *info,
	const char *buf, size_t count) {
	struct hisi_fb_data_type *hisifd = NULL;
	ssize_t ret = (ssize_t)count;
	uint32_t len = 0;
	ssize_t ret_count = 0;
	struct effect_bl_buf resolved_buf = { 0 };

	if (NULL == info) {
		HISI_FB_ERR("[effect] info is NULL\n");
		return -1;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -1;
	}

	if (NULL == buf) {
		HISI_FB_ERR("[effect] buf is NULL\n");
		return -1;
	}

	len = (uint32_t)strlen(buf);
	if (len < 3) {
		HISI_FB_ERR("[effect] invalid input buf, length=%d\n", len);
		return -1;
	}
	if (buf[1] != ':') {
		HISI_FB_ERR("[effect] invalid input buf:%s\n", buf);
		return -1;
	}

	DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
		"[DE] blank_sem_effect +\n");
	down(&hisifd->blank_sem_effect);
	DISPLAY_ENGINE_DEBUG(hisifd->panel_info.product_type,
		"[DE] blank_sem_effect -\n");

	ret_count = hisifb_display_effect_resolve_bl_buf(buf, &resolved_buf);
	HISI_FB_DEBUG("[effect]buf is %s, ret_count is %d\n",
		buf, (int)ret_count);
	ret = display_engine_effect_handle_delta(hisifd, ret_count,
		&resolved_buf, ret);
	up(&hisifd->blank_sem_effect);
	return ret;
}

uint32_t get_fixed_point_offset(uint32_t half_block_size)
{
	uint32_t num = 2;
	uint32_t len = 2;

	while (len < half_block_size) {
		num++;
		len <<= 1;
	}
	return num;
}

#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
int hisifb_hiace_roi_info_init(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req)
{
	struct dss_rect *hiace_roi = NULL;
	struct hiace_roi_info *hiace_roi_param = NULL;

	if (pov_req == NULL) {
		HISI_FB_ERR("pov_req is NULL!\n");
		return -1;
	}
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return -1;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_DEBUG("[effect] fb%d, not support!", hisifd->index);
		return -EINVAL;
	}

	pov_req = &(hisifd->ov_req);
	hiace_roi_param = &(hisifd->auto_hiace_roi_info);

	if (hisifd->panel_info.hiace_support) {
		hiace_roi = &(pov_req->hiace_roi_rect);
		if (pov_req->hiace_roi_support) {
			if (pov_req->hiace_roi_enable) {
				hiace_roi_param->roi_top = hiace_roi->y;
				hiace_roi_param->roi_left = hiace_roi->x;
				hiace_roi_param->roi_bot = hiace_roi->y + hiace_roi->h;
				hiace_roi_param->roi_right = hiace_roi->x + hiace_roi->w;
			} else {
				hiace_roi_param->roi_top = 0;
				hiace_roi_param->roi_left = 0;
				hiace_roi_param->roi_bot = 0;
				hiace_roi_param->roi_right = 0;
			}
			hiace_roi_param->roi_enable = pov_req->hiace_roi_enable;
		}
	} else {
		hiace_roi_param->roi_top =
			((hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_start_point >> 16) & 0x1fff);
		hiace_roi_param->roi_left =
			hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_start_point & 0x1fff;
		hiace_roi_param->roi_bot = hiace_roi_param->roi_top +
			((hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_width_high >> 16) & 0x1fff);
		hiace_roi_param->roi_right = hiace_roi_param->roi_left +
			(hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_width_high & 0x1fff);
		hiace_roi_param->roi_enable = hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_mode_ctrl;
	}
	return 0;
}

int hisifb_get_hiace_enable(struct fb_info *info, void __user *argp)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	int hiace_enabled;

	if (info == NULL) {
		HISI_FB_ERR("[effect] info is NULL\n");
		return -EINVAL;
	}

	if (argp == NULL) {
		HISI_FB_ERR("[effect] argp is NULL\n");
		return -EINVAL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -EINVAL;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_ERR("[effect] hisifd index error: fd = %d\n", hisifd->index);
		return -EINVAL;
	}

	if (hisifd->panel_info.hiace_support == 0) {
		if (g_debug_effect & DEBUG_EFFECT_ENTRY)
			DEBUG_EFFECT_LOG("[effect] Don't support HIACE\n");
		return -EINVAL;
	}

	hiace_enabled = hisifd->hiace_info[hisifd->panel_info.disp_panel_id].hiace_enable;

	ret = copy_to_user(argp, &hiace_enabled, sizeof(int));
	if (ret) {
		HISI_FB_ERR("failed to copy result of ioctl to user space\n");
		return -EINVAL;
	}

	return 0;
}

void hisifb_hiace_roi_reg_set(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req)
{
	char __iomem *hiace_base = NULL;
	dss_rect_t *rect = NULL;
	uint32_t start_point, width_high;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}
	if (pov_req == NULL) {
		HISI_FB_ERR("pov_req is NULL!\n");
		return;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_DEBUG("[effect] fb%d, not support!", hisifd->index);
		return;
	}

	hiace_base = hisifd->dss_base + DSS_HI_ACE_OFFSET;

	if (hisifd->fb_shutdown == true || hisifd->panel_power_on == false) {
		HISI_FB_ERR("[effect] fb_shutdown or panel power down");
		return;
	}

	if (pov_req->hiace_roi_support) {
		rect = &(pov_req->hiace_roi_rect);
		start_point = ((uint32_t)rect->y << 16) | ((uint32_t)rect->x +
			hisifd->panel_info.dummy_pixel_x_left);
		width_high = ((uint32_t)rect->h << 16) | ((uint32_t)rect->w);
		set_reg(hiace_base + DPE_ROI_START_POINT, start_point, 32, 0);
		set_reg(hiace_base + DPE_ROI_WIDTH_HIGH, width_high, 32, 0);
		set_reg(hiace_base + DPE_ROI_MODE_CTRL,
			pov_req->hiace_roi_enable, 32, 0);
		HISI_FB_DEBUG("start_point:0x%x, width_high:0x%x, roi_enable=%d\n",
			start_point, width_high, pov_req->hiace_roi_enable);
	} else {
		set_reg(hiace_base + DPE_ROI_START_POINT,
			hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_start_point, 32, 0);
		set_reg(hiace_base + DPE_ROI_WIDTH_HIGH,
			hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_width_high, 32, 0);
		set_reg(hiace_base + DPE_ROI_MODE_CTRL,
			hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.roi_mode_ctrl, 32, 0);
	}
}

void hiace_size_config(struct hisi_fb_data_type *hisifd, uint32_t width, uint32_t height)
{
	char __iomem *hiace_base = NULL;
	uint32_t half_block_w;
	uint32_t half_block_h;
	uint32_t fixbit_x;
	uint32_t fixbit_y;
	uint32_t reciprocal_x;
	uint32_t reciprocal_y;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return;
	}

	if (hisifd->panel_info.hiace_support == 0) {
		if (g_debug_effect & DEBUG_EFFECT_ENTRY)
			DEBUG_EFFECT_LOG("[effect] HIACE is not supported!\n");
		return;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_ERR("[effect] fb%d, not support!", hisifd->index);
		return;
	}

	hiace_base = hisifd->dss_base + DSS_HI_ACE_OFFSET;

	/* parameters */
	set_reg(hiace_base + DPE_IMAGE_INFO, (height << 16) | width, 32, 0);
	hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.image_info = (height << 16) | width;

	half_block_w = (width / (XBLOCKNUM * 2)) & 0x1ff;
	half_block_h = round(height, (YBLOCKNUM * 2)) & 0x1ff;
	set_reg(hiace_base + DPE_HALF_BLOCK_INFO,
		(half_block_h << 16) | half_block_w, 32, 0);

	hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.half_block_info = (half_block_h << 16) | half_block_w;

	fixbit_x = get_fixed_point_offset(half_block_w) & 0x1f;
	fixbit_y = get_fixed_point_offset(half_block_h) & 0x1f;
	reciprocal_x = (1U << (fixbit_x + 8)) / (2 * MAX(half_block_w, 1)) & 0x3ff;
	reciprocal_y = (1U << (fixbit_y + 8)) / (2 * MAX(half_block_h, 1)) & 0x3ff;
	set_reg(hiace_base + DPE_XYWEIGHT, (fixbit_y << 26) | (reciprocal_y << 16) |
		(fixbit_x << 10) | reciprocal_x, 32, 0);

	hisifd->effect_info[hisifd->panel_info.disp_panel_id].hiace.xyweight = (fixbit_y << 26) | (reciprocal_y << 16) |
		(fixbit_x << 10) | reciprocal_x;

	if (g_debug_effect & DEBUG_EFFECT_ENTRY)
		DEBUG_EFFECT_LOG("[effect] half_block_w:%d,half_block_h:%d,fixbit_x:%d,"
			"fixbit_y:%d, reciprocal_x:%d, reciprocal_y:%d\n",
			half_block_w, half_block_h, fixbit_x,
			fixbit_y, reciprocal_x, reciprocal_y);
}
#endif

static int share_mmap_map(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long start = 0;
	unsigned long size = 0;

	if (vma == NULL) {
		HISI_FB_ERR("[shmmap] vma is null!\n");
		return -1;
	}

	if (share_mem_virt == NULL || share_mem_phy == 0) {
		HISI_FB_ERR("[shmmap] share memory is not alloced!\n");
		return 0;
	}

	start = (unsigned long)vma->vm_start;
	size = (unsigned long)(vma->vm_end - vma->vm_start);
	if (size > SHARE_MEMORY_SIZE) {
		return -1;
	}
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	HISI_FB_INFO("[shmmap] my_map size = 0x%lx\n", size);
	if(remap_pfn_range(vma, start, __phys_to_pfn(share_mem_phy), size, vma->vm_page_prot)) {
		HISI_FB_ERR("[shmmap] remap_pfn_range error!\n");
		return -1;
	}
	return 0;
}


static struct file_operations mmap_dev_fops = {
	.owner = THIS_MODULE,
	.mmap = share_mmap_map,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MMAP_DEVICE_NAME,
	.fops = &mmap_dev_fops,
};

static int __init mmap_dev_init(void)
{
	int ret = 0;
	HISI_FB_INFO("[shmmap] dev_init \n");
	ret = misc_register(&misc);
	if (ret) {
		HISI_FB_ERR("[shmmap] misc_register ret = %d \n", ret);
		return ret;
	}
	return ret;
}

//static void __exit mmap_dev_exit(void) {
//	misc_deregister(&misc);
//	HISI_FB_DEBUG("[shmmap] dev_exit \n");
//}

//module_exit(mmap_dev_exit);

static int hisi_share_mem_probe(struct platform_device *pdev)
{
	HISI_FB_DEBUG("[shmmap] hisi_share_mem_probe \n");

	if (NULL == pdev) {
		HISI_FB_ERR("[shmmap] pdev is NULL");
		return -EINVAL;
	}

	share_mem_virt = (void *)dma_alloc_coherent(&pdev->dev, SHARE_MEMORY_SIZE, &share_mem_phy, GFP_KERNEL);
	if (share_mem_virt == NULL || share_mem_phy == 0) {
		HISI_FB_ERR("[shmmap] dma_alloc_coherent error! ");
		return -EINVAL;
	}

	mmap_dev_init();
	return 0;
}

static int hisi_share_mem_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id hisi_share_match_table[] = {
	{
		.compatible = DTS_COMP_SHAREMEM_NAME,
		.data = NULL,
	},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_share_match_table);

static struct platform_driver this_driver = {
	.probe = hisi_share_mem_probe,
	.remove = hisi_share_mem_remove,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = DEV_NAME_SHARE_MEM,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisi_share_match_table),
	},
};

static int __init hisi_share_mem_init(void)
{
	int ret = 0;
	HISI_FB_INFO("[shmmap] hisi_share_mem_init\n");
	ret = platform_driver_register(&this_driver);
	if (ret) {
		HISI_FB_ERR("[shmmap] platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

module_init(hisi_share_mem_init);

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
/*lint +e838 +e778 +e845 +e712 +e527 +e30 +e142 +e715 +e655 +e550 +e559*/
