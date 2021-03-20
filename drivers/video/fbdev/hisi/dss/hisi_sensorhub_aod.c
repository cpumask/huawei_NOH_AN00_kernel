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

DEFINE_SEMAPHORE(hw_lock_semaphore);
DEFINE_SEMAPHORE(hisi_sensorhub_aod_blank_sem);

static int sh_aod_blank_refcount;

#if CONFIG_SH_AOD_ENABLE
extern int get_aod_support(void);
extern int hisi_aod_set_blank_mode(int blank_mode);
extern bool hisi_aod_get_aod_lock_status(void);
#endif

extern struct fb_info *g_info_fb0;

bool hisi_sensorhub_aod_hw_lock(struct hisi_fb_data_type *hisifd)
{
	bool hw_lock_succ = false;
	bool is_timeout = false;
	uint32_t lock_status;
	uint32_t delay_count = 0;
	uint32_t timeout_count = 5000;  /* wait 5000 times */

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return hw_lock_succ;
	}

#if CONFIG_SH_AOD_ENABLE
	if (get_aod_support() != 1) {
		if (g_dump_sensorhub_aod_hwlock)
			HISI_FB_INFO("sensorhub aod no support!\n");

		return false;
	}
#endif
	down(&hw_lock_semaphore);

	while (1) {
		set_reg(hisifd->pctrl_base + PCTRL_RESOURCE3_LOCK, 0x30000000, 32, 0);
		lock_status = inp32(hisifd->pctrl_base + PCTRL_RESOURCE3_LOCK_STATUS);

		if (((lock_status & 0x70000000) == 0x30000000) || (delay_count > timeout_count)) {
			is_timeout = (delay_count > timeout_count) ? true : false;
			break;
		}

		mdelay(1);
		++delay_count;
	}

	if (!is_timeout)
		hw_lock_succ = true;
	else
		HISI_FB_ERR("fb%d AP hw_lock fail, lock_status = 0x%x, delay_count = %d!\n",
			hisifd->index, lock_status, delay_count);

	up(&hw_lock_semaphore);

	if (g_dump_sensorhub_aod_hwlock)
		HISI_FB_INFO("fb%d +, hw_lock=%d (0: fail; 1: succ), lock_status=0x%x!\n",
			hisifd->index, hw_lock_succ, lock_status);

	return hw_lock_succ;
}

bool hisi_sensorhub_aod_hw_unlock(struct hisi_fb_data_type *hisifd)
{
	bool hw_unlock_succ = false;
	uint32_t unlock_status;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return hw_unlock_succ;
	}

#if CONFIG_SH_AOD_ENABLE
	if (get_aod_support() != 1) {
		if (g_dump_sensorhub_aod_hwlock)
			HISI_FB_INFO("sensorhub aod no support!\n");

		return true;
	}
#endif

	down(&hw_lock_semaphore);
	set_reg(hisifd->pctrl_base + PCTRL_RESOURCE3_UNLOCK, 0x30000000, 32, 0);
	unlock_status = inp32(hisifd->pctrl_base + PCTRL_RESOURCE3_LOCK_STATUS);
	hw_unlock_succ = true;
	up(&hw_lock_semaphore);

	if (g_dump_sensorhub_aod_hwlock)
		HISI_FB_INFO("fb%d -, hw_unlock=%d (0: fail; 1: succ), unlock_status=0x%x!\n",
			hisifd->index, hw_unlock_succ, unlock_status);

	return hw_unlock_succ;
}

static int hisi_check_sh_aod_blank_refcount(struct hisi_fb_data_type *hisifd, uint32_t msg_no)
{
	sh_aod_blank_refcount++;
	HISI_FB_INFO("fb%d +, sh_aod_blank_refcount=%d!\n", hisifd->index, sh_aod_blank_refcount);
	HISI_FB_INFO("Power State Reg is 0x%x\n", inp32(hisifd->sctrl_base + SCBAKDATA0));

	/* high 16bit indicate msg no */
	set_reg(hisifd->sctrl_base + SCBAKDATA0, msg_no & 0xFFFF, 16, 16);

	if (sh_aod_blank_refcount != 1) {
		HISI_FB_ERR("fb%d, sh_aod_blank_refcount=%d is error\n", hisifd->index, sh_aod_blank_refcount);
		return -1;
	}

	return 0;
}

static int hisi_sensorhub_aod_set_pxl_clk(struct hisi_fb_data_type *hisifd)
{
	uint64_t pxl_clk_rate;
	int ret;

	pxl_clk_rate = hisifd->panel_info.pxl_clk_rate;
	ret = clk_set_rate(hisifd->dss_pxl0_clk, pxl_clk_rate);
	if (ret < 0) {
		HISI_FB_ERR("fb%d dss_pxl0_clk clk_set_rate[%llu] failed, error=%d!\n",
			hisifd->index, pxl_clk_rate, ret);
		return ret;
	}

	HISI_FB_INFO("dss_pxl0_clk:[%llu]->[%llu]\n", pxl_clk_rate, (uint64_t)clk_get_rate(hisifd->dss_pxl0_clk));

	return 0;
}

static int dpe_clk_enable(struct hisi_fb_data_type *hisifd)
{
	int ret;
	uint64_t default_dss_core_clk = DEFAULT_DSS_CORE_CLK_RATE_L1;

	ret = dpe_common_clk_enable(hisifd);
	if (ret) {
		HISI_FB_ERR("fb%d dpe_common_clk_enable, error=%d!\n", hisifd->index, ret);
		return -EINVAL;
	}

	ret = dpe_inner_clk_enable(hisifd);
	if (ret) {
		HISI_FB_ERR("fb%d dpe_inner_clk_enable, error=%d!\n", hisifd->index, ret);
		return -EINVAL;
	}

	if (hisifd->panel_info.update_core_clk_support)
		default_dss_core_clk = DEFAULT_DSS_CORE_CLK_RATE_L2;
	if (dss_get_current_pri_clk_rate(hisifd) < default_dss_core_clk) {
		ret = clk_set_rate(hisifd->dss_pri_clk, default_dss_core_clk);
		if (ret < 0) {
			HISI_FB_ERR("fb%d dss_pri_clk clk_set_rate failed, error=%d!\n", hisifd->index, ret);
			return -EINVAL;
		}
		hisifd->dss_vote_cmd.dss_pri_clk_rate = default_dss_core_clk;
		HISI_FB_INFO("dss_pri_clk:[%llu]\n", (uint64_t)clk_get_rate(hisifd->dss_pri_clk));
	}


	return 0;
}

static int hisi_aod_config_fold_panel(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	uint32_t panel_id;

	pinfo = &(hisifd->panel_info);
	panel_id = hisi_aod_get_panel_id();
	HISI_FB_INFO("[fold] panel id :%d\n", panel_id);

	if (panel_id >= DISPLAY_PANEL_ID_MAX) {
		HISI_FB_ERR("[fold] not supported panel id :%d\n", panel_id);
		return -1;
	}
	hisifd_get_panel_info(hisifd, pinfo, panel_id);

	return 0;
}

int hisi_sensorhub_aod_unblank(uint32_t msg_no)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = hisifd_list[PRIMARY_PANEL_IDX];

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL point!\n");

	HISI_FB_INFO("fb%d,msg_no is %d +\n", hisifd->index, msg_no);

	down(&hisi_sensorhub_aod_blank_sem);
	if (!hisi_sensorhub_aod_hw_lock(hisifd))
		goto up_blank_sem;

	if (hisi_check_sh_aod_blank_refcount(hisifd, msg_no))
		goto hw_unlock;

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		if (hisi_aod_config_fold_panel(hisifd))
			goto hw_unlock;
	}

	mediacrg_regulator_enable(hisifd);

	ret = dpe_clk_enable(hisifd);
	if (ret < 0)
		goto hw_unlock;

	ret = hisi_sensorhub_aod_set_pxl_clk(hisifd);
	if (ret < 0)
		goto hw_unlock;

	ret = dpe_regulator_enable(hisifd);
	if (ret) {
		HISI_FB_ERR("fb%d dpe_regulator_enable, error=%d!\n", hisifd->index, ret);
		goto hw_unlock;
	}
#ifdef CONFIG_ARM_SMMU_V3
	hisi_dss_smmuv3_on(hisifd);
#endif
	/* dsi apb clk must be enabled after dsi reset, so now disable dsi apb clk */
	inheritor_dss_wr_reg(hisifd->dss_base + GLB_MODULE_CLK_SEL, 0xFE7FFFFF);
	inheritor_dss_wr_reg(hisifd->dss_base + GLB_MODULE_CLK_EN, 0xFE7FFFFF);

	if (need_config_dsi1(hisifd))
		outp32(hisifd->peri_crg_base + PERRSTDIS3, 0x30000000);
	else
		outp32(hisifd->peri_crg_base + PERRSTDIS3, 0x10000000);

	ret = mipi_dsi_clk_enable(hisifd);
	if (ret) {
		HISI_FB_ERR("fb%d mipi_dsi_clk_enable, error=%d!\n", hisifd->index, ret);
		goto hw_unlock;
	} else {
		set_reg(hisifd->sctrl_base + SCBAKDATA0, 0x1, 2, 6);
		HISI_FB_INFO("Power State Reg is 0x%x\n", inp32(hisifd->sctrl_base + SCBAKDATA0));
		goto up_blank_sem;
	}

hw_unlock:
	set_reg(hisifd->sctrl_base + SCBAKDATA0, 0x3, 2, 6);
	if (!hisi_sensorhub_aod_hw_unlock(hisifd))
		HISI_FB_ERR("aod_hw_unlock fail!\n");

up_blank_sem:
	up(&hisi_sensorhub_aod_blank_sem);

	HISI_FB_INFO("fb%d, -\n", hisifd->index);
	return ret;
}

int hisi_sensorhub_aod_blank(uint32_t msg_no)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = hisifd_list[PRIMARY_PANEL_IDX];

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return -EINVAL;
	}

	HISI_FB_INFO("fb%d,msg_no is %d +\n", hisifd->index, msg_no);

	down(&hisi_sensorhub_aod_blank_sem);

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		if (hisi_aod_config_fold_panel(hisifd))
			goto up_blank_sem;
	}

	sh_aod_blank_refcount--;
	HISI_FB_INFO("fb%d +, sh_aod_blank_refcount=%d!\n", hisifd->index, sh_aod_blank_refcount);
	HISI_FB_INFO("Power State Reg is 0x%x\n", inp32(hisifd->sctrl_base + SCBAKDATA0));

	/* high 16bit indicate msg no */
	set_reg(hisifd->sctrl_base + SCBAKDATA0, msg_no & 0xFFFF, 16, 16);

	if (sh_aod_blank_refcount != 0) {
		set_reg(hisifd->sctrl_base + SCBAKDATA0, 0x3, 2, 6);
		HISI_FB_ERR("fb%d, sh_aod_blank_refcount=%d is error\n", hisifd->index, sh_aod_blank_refcount);
		goto up_blank_sem;
	}

	/* reset DSI */
	if (need_config_dsi1(hisifd))
		outp32(hisifd->peri_crg_base + PERRSTEN3, 0x30000000);
	else
		outp32(hisifd->peri_crg_base + PERRSTEN3, 0x10000000);
	mipi_dsi_clk_disable(hisifd);

#ifdef CONFIG_ARM_SMMU_V3
	hisi_dss_smmuv3_off(hisifd);
#endif
	dpe_regulator_disable(hisifd);
	dpe_inner_clk_disable(hisifd);
	dpe_common_clk_disable(hisifd);
	mediacrg_regulator_disable(hisifd);

	if (!hisi_sensorhub_aod_hw_unlock(hisifd))
		HISI_FB_ERR("aod_hw_unlock fail!\n");

	HISI_FB_INFO("Power State Reg is 0x%x\n", inp32(hisifd->sctrl_base + SCBAKDATA0));
	set_reg(hisifd->sctrl_base + SCBAKDATA0, 0x2, 2, 6);
up_blank_sem:
	up(&hisi_sensorhub_aod_blank_sem);

	HISI_FB_INFO("fb%d, -\n", hisifd->index);
	return ret;
}

#if CONFIG_SH_AOD_ENABLE
bool hisi_fb_request_aod_hw_lock(struct hisi_fb_data_type *hisifd)
{
	return hisi_sensorhub_aod_hw_lock(hisifd);
}

void hisi_fb_release_aod_hw_lock(struct hisi_fb_data_type *hisifd, bool locked)
{
	if (locked) {
		if (!hisi_sensorhub_aod_hw_unlock(hisifd))
			HISI_FB_ERR("aod_hw_unlock fail!\n");
	}
}

int hisi_fb_aod_blank(struct hisi_fb_data_type *hisifd, int blank_mode)
{
	int ret = 0;

	if (!hisifd)
		return ret;

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return ret;

	if (blank_mode == FB_BLANK_UNBLANK) {
		HISI_FB_INFO("+, aod blank_mode %d\n", blank_mode);
		ret = hisi_aod_set_blank_mode(blank_mode);

	} else if (blank_mode == FB_BLANK_POWERDOWN) {
		if ((!hisifd->panel_power_on) &&
			(hisifd_list[EXTERNAL_PANEL_IDX] != NULL) &&
			(!hisifd_list[EXTERNAL_PANEL_IDX]->panel_power_on)) {
			HISI_FB_INFO("+, aod blank_mode %d\n", blank_mode);
			ret = hisi_aod_set_blank_mode(blank_mode);
		}
	}

	return ret;
}

bool hisi_fb_get_aod_lock_status(void)
{
	return hisi_aod_get_aod_lock_status();
}
#endif


static int hisifb_unblank_refresh(struct hisi_fb_data_type *hisifd, struct fb_info *info)
{
	int ret;

	ret = hisi_fb_blank_sub(FB_BLANK_UNBLANK, info);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, blank_mode %d failed!\n", hisifd->index, FB_BLANK_UNBLANK);
		return -1;
	}

	hisi_fb_frame_refresh(hisifd, "aod");

	ret = hisifb_ce_service_blank(FB_BLANK_UNBLANK, info);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, blank_mode %d hisifb_ce_service_blank() failed!\n", hisifd->index, FB_BLANK_UNBLANK);
		return -1;
	}

	ret = hisifb_display_engine_blank(FB_BLANK_UNBLANK, info);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, blank_mode %d hisifb_display_engine_blank() failed!\n",
			hisifd->index, FB_BLANK_UNBLANK);
		return -1;
	}

	return 0;
}

void hisi_fb_unblank_wq_handle(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct fb_info *info = g_info_fb0;

	hisi_check_and_no_retval(!info, ERR, "info is NULL\n");

	hisifd = container_of(work, struct hisi_fb_data_type, aod_ud_fast_unblank_work);
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	if (hisifd->panel_info.fake_external && (hisifd->index == EXTERNAL_PANEL_IDX)) {
		hisifd->enable_fast_unblank = FALSE;
		HISI_FB_INFO("it is fake, blank it fail\n");
		return;
	}

	HISI_FB_INFO("fb%d +\n", hisifd->index);

	down(&hisifd_list[AUXILIARY_PANEL_IDX]->blank_sem);
	down(&hisifd->fast_unblank_sem);
	if ((hisifd->panel_power_on) && (hisifd->secure_ctrl.secure_blank_flag)) {
		/* wait for blank */
		HISI_FB_INFO("wait for tui blank!\n");
		while (hisifd->panel_power_on)
			mdelay(1);
	}


	down(&hisifd->blank_aod_sem);
	if (hisi_fb_get_aod_lock_status() == false) {
		HISI_FB_INFO("ap had released aod lock, stop unblank wq handle!\n");
		goto fast_unblank_fail;
	}

	if (hisifd->dp_device_srs) {
		hisifd->dp_device_srs(hisifd, true);
	} else {
		if (hisifb_unblank_refresh(hisifd, info) < 0)
			goto fast_unblank_fail;
	}

	HISI_FB_INFO("fb%d -\n", hisifd->index);

fast_unblank_fail:
	hisifd->enable_fast_unblank = FALSE;
	up(&hisifd->blank_aod_sem);
	up(&hisifd->fast_unblank_sem);
	up(&hisifd_list[AUXILIARY_PANEL_IDX]->blank_sem);
}

void wait_for_aod_stop(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;
	if ((hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) == 0)
		return;

	HISI_FB_INFO("+\n");
	hisi_aod_wait_stop_nolock();
	HISI_FB_INFO("-\n");
}

void wait_for_fast_unblank_end(struct hisi_fb_data_type *hisifd)
{
	int try_times = 0;

	HISI_FB_INFO("+\n");

	while (hisifd->enable_fast_unblank) {
		mdelay(1);
		if (++try_times > 1000) { /* wait times */
			HISI_FB_ERR("[fold] wait for fast unblank end timeout!\n");
			break;
		}
	}
}

