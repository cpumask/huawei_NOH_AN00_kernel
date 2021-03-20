/* Copyright (c) 2014-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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

#include "overlay/hisi_overlay_utils.h"
#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#include "hisi_dp.h"
#endif
#if defined(CONFIG_DRMDRIVER)
#include <linux/hisi/hisi_drmdriver.h>
#endif
#if defined(CONFIG_TEE_TUI)
#include "tui.h"
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define DPTX_HDCP_MAX_AUTH_RETRY 10

int configure_hdcp_service_security(unsigned int master_op_type, unsigned int value)
{
	int ret = 0;

	HISI_FB_DEBUG("+\n");

	if (master_op_type >= (unsigned int)HDCP_OP_SECURITY_MAX) {
		HISI_FB_ERR("invalid master_op_type=%d", master_op_type);
		return -1;
	}
#if defined(CONFIG_DRMDRIVER)
	ret = atfd_hisi_service_access_register_smc(ACCESS_REGISTER_FN_MAIN_ID_HDCP,
		master_op_type, value, (u64)ACCESS_REGISTER_FN_SUB_ID_HDCP_CTRL);
#endif
	HISI_FB_DEBUG("-\n");

	return ret;
}
EXPORT_SYMBOL_GPL(configure_hdcp_service_security);

int configure_hdcp_reg_get_security(uint32_t addr)
{
	int ret = 0;

	HISI_FB_DEBUG("+\n");
#if defined(CONFIG_DRMDRIVER)
	ret = atfd_hisi_service_access_register_smc(ACCESS_REGISTER_FN_MAIN_ID_HDCP,
		addr, 0x00, (u64)ACCESS_REGISTER_FN_SUB_ID_HDCP_INT);
#endif
	HISI_FB_DEBUG("-\n");

	return ret;
}
EXPORT_SYMBOL_GPL(configure_hdcp_reg_get_security);

void hisi_hdcp13_enable(uint32_t en)
{
	configure_hdcp_service_security(DSS_HDCP13_ENABLE, en);
}

void hisi_hdcp13_encrypt_enable(uint32_t en)
{
	configure_hdcp_service_security(DSS_HDCP13_ENCRYPT_ENABLE, en);
}

void hisi_hdcp22_enable(uint32_t en)
{
	configure_hdcp_service_security(DSS_HDCP22_ENABLE, en);
}

void hisi_hdcp_dpc_sec_en(void)
{
	configure_hdcp_service_security(DSS_HDCP_DPC_SEC_EN, 1);
}

void hisi_hdcp_obs_set(uint32_t reg)
{
	configure_hdcp_service_security(DSS_HDCP_OBS_SET, reg);
}

void hisi_hdcp_int_clr(uint32_t reg)
{
	configure_hdcp_service_security(DSS_HDCP_INT_CLR, reg);
}

void hisi_hdcp_int_mask(uint32_t reg)
{
	configure_hdcp_service_security(DSS_HDCP_INT_MASK, reg);
}

void hisi_hdcp_cp_irq(void)
{
	configure_hdcp_service_security(DSS_HDCP_CP_IRQ, 1);
}

int hisi_hdcp_reg_get(uint32_t addr)
{
	return configure_hdcp_reg_get_security(addr);
}

void hisi_hdcp_enc_mode(uint32_t en)
{
	configure_hdcp_service_security(DSS_HDCP_ENC_MODE_EN, en);
}

#endif

static int hisifb_secure_check_backlight_status(struct hisi_fb_data_type *hisifd,
	struct hisifb_secure *secure_ctrl, bool is_ready)
{
	if (hisifd->panel_info.bl_set_type & BL_SET_BY_MIPI) {
		if (!is_ready && (secure_ctrl->secure_event == DSS_SEC_ENABLE)) {
#if defined(CONFIG_TEE_TUI)
			send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
#endif
			secure_ctrl->secure_event = DSS_SEC_DISABLE;
			return -1;
		}
	}

	return 0;
}

static int hisifb_secure_check_panel_power_status(struct hisi_fb_data_type *hisifd,
	struct hisifb_secure *secure_ctrl)
{
	if (!hisifd->panel_power_on && (secure_ctrl->secure_event == DSS_SEC_ENABLE)) {
#if defined(CONFIG_TEE_TUI)
		send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
#endif
		secure_ctrl->secure_event = DSS_SEC_DISABLE;
		return -1;
	}

	return 0;
}

static void hisifb_secure_notify_tui_status(struct hisi_fb_data_type *hisifd,
	struct hisifb_secure *secure_ctrl)
{
	if ((secure_ctrl->secure_status == DSS_SEC_RUNNING) &&
		(secure_ctrl->secure_event == DSS_SEC_ENABLE) &&
		(secure_ctrl->tui_need_switch)) {
		hisifb_activate_vsync(hisifd);
#if defined(CONFIG_TEE_TUI)
		if (hisifd->secure_ctrl.secure_blank_flag) {
			send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
			secure_ctrl->secure_event = DSS_SEC_DISABLE;
			HISI_FB_INFO("TUI blank switch to DSS_SEC_RUNNING failed !\n");
		} else {
			send_tui_msg_config(TUI_POLL_CFG_OK, 0, "DSS");
			HISI_FB_INFO("TUI switch to DSS_SEC_RUNNING succ !\n");
		}
#endif
		secure_ctrl->tui_need_switch = 0;
	}
}

static void hisifb_secure_ctrl_wq_handler(struct work_struct *work)
{
	bool is_readly = false;
	unsigned long dw_jiffies;
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_secure *secure_ctrl = NULL;
	int ret;

	secure_ctrl = container_of(work, typeof(*secure_ctrl), secure_ctrl_work);
	hisi_check_and_no_retval(!secure_ctrl, ERR, "secure_ctrl is NULL!\n");

	hisifd = secure_ctrl->hisifd;
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	HISI_FB_DEBUG(": secure_status = %d, secure_event = %d, frame_no = %d +++\n",
		secure_ctrl->secure_status, secure_ctrl->secure_event, hisifd->ov_req.frame_no);

	if (hisifd->panel_info.bl_set_type & BL_SET_BY_MIPI) {
		dw_jiffies = jiffies + HZ;
		do {
			if (hisifd->secure_ctrl.have_set_backlight) {
				is_readly = true;
				break;
			}
		} while (time_after(dw_jiffies, jiffies));
	}

	down(&hisifd->blank_sem);
	ret = hisifb_secure_check_backlight_status(hisifd, secure_ctrl, is_readly);
	if (ret) {
		HISI_FB_INFO("backlight isn't set!\n");
		up(&hisifd->blank_sem);
		return;
	}

	ret  = hisifb_secure_check_panel_power_status(hisifd, secure_ctrl);
	if (ret) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		up(&hisifd->blank_sem);
		return;
	}

	if ((secure_ctrl->secure_status == PRIMARY_PANEL_IDX) &&
		(secure_ctrl->secure_event == DSS_SEC_ENABLE))
		secure_ctrl->secure_status = DSS_SEC_RUNNING;

	hisifb_secure_notify_tui_status(hisifd, secure_ctrl);
	up(&hisifd->blank_sem);

	HISI_FB_DEBUG(": secure_status = %d, secure_event = %d, frame_no = %d ---\n",
		secure_ctrl->secure_status, secure_ctrl->secure_event, hisifd->ov_req.frame_no);
}

static int update_dss_tui_request(struct hisi_fb_data_type *hisifd, struct hisifb_secure *secure_ctrl,
	int tui_request, int secure)
{
	if (secure_ctrl->secure_status == tui_request) {
#if defined(CONFIG_TEE_TUI)
		if (secure) {
			send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
			secure_ctrl->secure_event = DSS_SEC_DISABLE;
		}
#endif
		HISI_FB_INFO("secure_status is not changed, secure_status = %d,---!\n", secure_ctrl->secure_status);
		return -1;
	}
	secure_ctrl->secure_event = tui_request;
	secure_ctrl->tui_need_switch = 1;
	secure_ctrl->tui_need_skip_report = 0;

	return 0;
}

static void hisi_fb_secure_aod_blank(struct hisi_fb_data_type *hisifd, struct hisifb_secure *secure_ctrl)
{
	bool sensorhub_aod_hwlock_succ = false;
	int ret;

	sensorhub_aod_hwlock_succ = hisi_fb_request_aod_hw_lock(hisifd);
	ret = hisi_fb_blank_sub(FB_BLANK_POWERDOWN, hisifd->fbi);
	if (ret != 0)
		HISI_FB_ERR("fb%d, blank_mode %d failed!\n", hisifd->index, FB_BLANK_POWERDOWN);

	(void)hisi_fb_aod_blank(hisifd, FB_BLANK_POWERDOWN);
	hisi_fb_release_aod_hw_lock(hisifd, sensorhub_aod_hwlock_succ);
	secure_ctrl->secure_blank_flag = 0;
}

static int check_secure_created(struct hisifb_secure *secure_ctrl, int secure)
{
	if (!secure_ctrl->secure_created) {
#if defined(CONFIG_TEE_TUI)
		if (secure) {
			send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
			secure_ctrl->secure_event = DSS_SEC_DISABLE;
		}
#endif
		return -1;
	}

	return 0;
}

/* receive switch tui request
 * 1: secure enable
 * 0: secure disable
 */
static int notify_dss_tui_request(void *data, int secure)
{
	int ret;
	int tui_request;
	struct hisifb_secure *secure_ctrl = NULL;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = (struct hisi_fb_data_type *)data; /* hisifd_list[PRIMARY_PANEL_IDX]; */
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");
	secure_ctrl = &(hisifd->secure_ctrl);

	ret = check_secure_created(secure_ctrl, secure);
	if (ret < 0) {
		HISI_FB_ERR("fb%d, secure is not created yet!\n", hisifd->index);
		return -1;
	}

	down(&hisifd->blank_sem);
	if (!hisifd->panel_power_on) {
#if defined(CONFIG_TEE_TUI)
		if (secure) {
			send_tui_msg_config(TUI_POLL_CFG_FAIL, 0, "DSS");
			secure_ctrl->secure_event = DSS_SEC_DISABLE;
		}
#endif
		HISI_FB_INFO("fb%d, panel is power off!", hisifd->index);
		up(&hisifd->blank_sem);
		return -1;
	}

	tui_request = secure ? DSS_SEC_ENABLE : DSS_SEC_DISABLE;
	HISI_FB_INFO(": secure_status = %d, secure_event = %d, frame_no = %d , tui_request = %d +++\n",
		secure_ctrl->secure_status, secure_ctrl->secure_event, hisifd->ov_req.frame_no, tui_request);

	ret = update_dss_tui_request(hisifd, secure_ctrl, tui_request, secure);
	if (ret) {
		up(&hisifd->blank_sem);
		return ret;
	}

	if (secure_ctrl->secure_event == DSS_SEC_ENABLE) {
		hisifb_activate_vsync(hisifd);
		wake_up_interruptible_all(&(hisifd->vsync_ctrl.vsync_wait));
		up(&hisifd->blank_sem);
	} else {
		secure_ctrl->secure_status = DSS_SEC_IDLE;
		up(&hisifd->blank_sem);
		HISI_FB_INFO("secure_blank_flag = %d, panel_power_on = %d\n",
			secure_ctrl->secure_blank_flag, hisifd->panel_power_on);

		if (secure_ctrl->secure_blank_flag)
			hisi_fb_secure_aod_blank(hisifd, secure_ctrl);
	}

	HISI_FB_INFO("secure_status = %d, secure_event = %d, frame_no = %d , tui_request = %d ---\n",
		secure_ctrl->secure_status, secure_ctrl->secure_event, hisifd->ov_req.frame_no, tui_request);

	return 0;
}

static ssize_t hisifb_secure_event_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int val;
	struct fb_info *fbi = NULL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!dev) {
		HISI_FB_ERR("secure event store dev NULL Pointer\n");
		return -1;
	}

	fbi = dev_get_drvdata(dev);
	if (!fbi) {
		HISI_FB_ERR("secure_event_store fbi NULL Pointer\n");
		return -1;
	}

	hisifd = (struct hisi_fb_data_type *)fbi->par;
	if (!hisifd) {
		HISI_FB_ERR("secure_event_store hisifd NULL Pointer\n");
		return -1;
	}

	val = (int)simple_strtoul(buf, NULL, 0);

	notify_dss_tui_request(hisifd, val);

	return count;
}

static ssize_t hisifb_secure_event_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t ret;
	struct fb_info *fbi = NULL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!dev) {
		HISI_FB_ERR("secure_event_show dev NULL Pointer\n");
		return -1;
	}

	fbi = dev_get_drvdata(dev);
	if (!fbi) {
		HISI_FB_ERR("secure_event_show fbi NULL Pointer\n");
		return -1;
	}

	hisifd = (struct hisi_fb_data_type *)fbi->par;
	if (!hisifd) {
		HISI_FB_ERR("secure_event_show hisifd NULL Pointer\n");
		return -1;
	}

	snprintf(buf, PAGE_SIZE, "SECURE_EVENT=%d, SECURE_STATUS=%d\n",
		hisifd->secure_ctrl.secure_event, hisifd->secure_ctrl.secure_status);
	ret = strlen(buf) + 1;

	return ret;
}

static DEVICE_ATTR(dss_secure, 0644, hisifb_secure_event_show, hisifb_secure_event_store);

/* TUI is enable, mctl sys ov should sel sec rch in non-secure. */
void hisi_sec_mctl_set_regs(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_secure *secure_ctrl = NULL;
	char __iomem *module_base;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}
	secure_ctrl = &(hisifd->secure_ctrl);
	if (secure_ctrl->secure_event == DSS_SEC_ENABLE) {
		module_base = hisifd->dss_module.mctl_sys_base;
		hisifd->set_reg(hisifd, module_base + MCTL_RCH_OV0_SEL1, TUI_SEC_RCH, 4, 0);
	}
}

static void hisi_dss_invalid_smmu_ptw_cache(struct hisi_fb_data_type *hisifd)
{
	if (hisifd == NULL)
		return;

}

void hisi_drm_layer_online_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req_prev, dss_overlay_t *pov_req)
{
	int i;
	int m;
	int j = 0;
#if defined(CONFIG_DRMDRIVER)
	int compose_mode;
#endif
	int sec_chn[250] = {DSS_RCHN_NONE};  /* dss chn max index is 12, 250 buff len is ok */
	dss_layer_t *layer = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	secure_ctrl = &(hisifd->secure_ctrl);
#if defined(CONFIG_DRMDRIVER)
	compose_mode = (hisifd->index == PRIMARY_PANEL_IDX) ? ONLINE_COMPOSE_MODE : OVL1_ONLINE_COMPOSE_MODE;
#endif

	if (!pov_req) {
		hisi_drm_layer_online_clear(hisifd, pov_req_prev, NULL, 0);
		HISI_FB_DEBUG("pov_req is NULL!\n");
		return;
	}

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req->ov_block_infos_ptr);
	for (m = 0; m < (int)pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);

		for (i = 0; i < (int)pov_h_block->layer_nums; i++) {
			layer = &(pov_h_block->layer_infos[i]);
			if (layer->img.secure_mode == 1) {
				HISI_FB_DEBUG("chn_idx = %d, frame_no = %d, mmu_enable = %d\n",
					layer->chn_idx, pov_req->frame_no, layer->img.mmu_enable);
					sec_chn[j++] = layer->chn_idx;
				if (layer->img.mmu_enable) {
					hisifb_atf_config_security(DSS_CH_MMU_SEC_CONFIG,
						(uint32_t)layer->chn_idx, compose_mode);
					hisi_dss_invalid_smmu_ptw_cache(hisifd);
				} else {
					hisifb_atf_config_security(DSS_CH_SEC_CONFIG,
						(uint32_t)layer->chn_idx, compose_mode);
				}
			}
		}
	}

	hisi_check_and_no_retval(!pov_req_prev, INFO, "pov_req_prev is NULL\n");

	hisi_drm_layer_online_clear(hisifd, pov_req_prev, sec_chn, j);
}

static bool is_secure_layer(const int *seclist, int list_max, int32_t chn_idx)
{
	int k;

	if (!seclist)
		return false;

	for (k = 0; k < list_max; k++) {
		if (chn_idx == seclist[k])
			return true;
	}

	return false;
}

void hisi_drm_layer_online_clear(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req_prev,
	const int *seclist, int list_max)
{
	int i;
	int j;
	dss_layer_t *layer = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

#if defined(CONFIG_DRMDRIVER)
	int cmd_mode;

	int compose_mode = (hisifd->index == PRIMARY_PANEL_IDX) ? ONLINE_COMPOSE_MODE : OVL1_ONLINE_COMPOSE_MODE;
#endif

	if (!pov_req_prev) {
		HISI_FB_ERR("pov_req_prev is NULL!\n");
		return;
	}

	secure_ctrl = &(hisifd->secure_ctrl);
	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req_prev->ov_block_infos_ptr);

	for (i = 0; i < (int)pov_req_prev->ov_block_nums; i++) {
		pov_h_block = &(pov_h_block_infos[i]);
		for (j = 0; j < (int)pov_h_block->layer_nums; j++) {
			layer = &(pov_h_block->layer_infos[j]);

			if (layer->img.secure_mode != 1)
				continue;

			if (!is_secure_layer(seclist, list_max, layer->chn_idx)) {
				HISI_FB_DEBUG("chn_idx = %d, prev_frame_no = %d mmu_emable=%d\n",
					layer->chn_idx, pov_req_prev->frame_no, layer->img.mmu_enable);
				cmd_mode = layer->img.mmu_enable ? DSS_CH_MMU_SEC_DECONFIG : DSS_CH_SEC_DECONFIG;
				hisifb_atf_config_security(cmd_mode, (uint32_t)layer->chn_idx, compose_mode);
				hisi_dss_invalid_smmu_ptw_cache(hisifd);
			}
		}
	}
}

static void set_drm_offline_compose_mode(struct hisi_fb_data_type *hisifd, uint32_t secure_mode,
	int32_t chn_idx, uint32_t mmu_enable, int compose_mode)
{
	if (secure_mode == 1) {
		HISI_FB_DEBUG("chn_idx = %d, mmu_enable = %d\n", chn_idx, mmu_enable);
		if (mmu_enable) {
			hisifb_atf_config_security(DSS_CH_MMU_SEC_CONFIG, (uint32_t)chn_idx, compose_mode);
			hisi_dss_invalid_smmu_ptw_cache(hisifd);
		} else {
			hisifb_atf_config_security(DSS_CH_SEC_CONFIG, (uint32_t)chn_idx, compose_mode);
		}
	}
}

void hisi_drm_layer_offline_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	int m;
	int i;
#if defined(CONFIG_DRMDRIVER)
	int compose_mode;
#endif
	dss_wb_layer_t *wb_layer4block = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_layer_t *layer = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "ddrm layer offline config hisifd is NULL!\n");
	hisi_check_and_no_retval(!pov_req, ERR, "ddrm layer offline config pov_req is NULL!\n");

	secure_ctrl = &(hisifd_list[PRIMARY_PANEL_IDX]->secure_ctrl);
#if defined(CONFIG_DRMDRIVER)
	compose_mode = (pov_req->ovl_idx == DSS_OVL2) ? OFFLINE_COMPOSE_MODE : OVL3_OFFLINE_COMPOSE_MODE;
#endif

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr;
	if (!pov_h_block_infos) {
		HISI_FB_ERR("fb%d, offline config invalid pov_h_block_infos!\n", hisifd->index);
		return;
	}

	wb_layer4block = &(pov_req->wb_layer_infos[0]);
	set_drm_offline_compose_mode(hisifd, wb_layer4block->dst.secure_mode, wb_layer4block->chn_idx,
		wb_layer4block->dst.mmu_enable, compose_mode);

	for (m = 0; m < (int)pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);
		for (i = 0; i < (int)pov_h_block->layer_nums; i++) {
			layer = &(pov_h_block->layer_infos[i]);
			set_drm_offline_compose_mode(hisifd, layer->img.secure_mode, layer->chn_idx,
				layer->img.mmu_enable, compose_mode);
		}
	}
}

void hisi_drm_layer_offline_clear(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	int m;
	int i;
#if defined(CONFIG_DRMDRIVER)
	int compose_mode;
	int cmd_mode;
#endif
	dss_wb_layer_t *wb_layer4block = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_layer_t *layer = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "drm layer offline clear hisifd is NULL!\n");
	hisi_check_and_no_retval(!pov_req, ERR, "drm layer offline clear pov_req is NULL!\n");

	secure_ctrl = &(hisifd_list[PRIMARY_PANEL_IDX]->secure_ctrl);
#if defined(CONFIG_DRMDRIVER)
	compose_mode = (pov_req->ovl_idx == DSS_OVL2) ? OFFLINE_COMPOSE_MODE : OVL3_OFFLINE_COMPOSE_MODE;
#endif

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr;
	if (!pov_h_block_infos) {
		HISI_FB_ERR("fb%d, drm layer offline clear invalid pov_h_block_infos!\n", hisifd->index);
		return;
	}

	wb_layer4block = &(pov_req->wb_layer_infos[0]);
	if (wb_layer4block->dst.secure_mode == 1) {
		HISI_FB_DEBUG("wb_layer4block->chn_idx = %d, mmu_enable = %d\n",
			wb_layer4block->chn_idx, wb_layer4block->dst.mmu_enable);
		cmd_mode = wb_layer4block->dst.mmu_enable ? DSS_CH_MMU_SEC_DECONFIG : DSS_CH_SEC_DECONFIG;
		hisifb_atf_config_security(cmd_mode, (uint32_t)wb_layer4block->chn_idx, compose_mode);
	}

	for (m = 0; m < (int)pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);
		for (i = 0; i < (int)pov_h_block->layer_nums; i++) {
			layer = &(pov_h_block->layer_infos[i]);
			if (layer->img.secure_mode == 1) {
				HISI_FB_DEBUG("chn_idx = %d, mmu_enable = %d\n", layer->chn_idx, layer->img.mmu_enable);
				cmd_mode = layer->img.mmu_enable ? DSS_CH_MMU_SEC_DECONFIG : DSS_CH_SEC_DECONFIG;
				hisifb_atf_config_security(cmd_mode, (uint32_t)layer->chn_idx, compose_mode);
				hisi_dss_invalid_smmu_ptw_cache(hisifd);
			}
		}
	}
}

static bool check_tui_layer_chn_cfg_ok(struct hisi_fb_data_type *hisifd)
{
	dss_layer_t *layer = NULL;
	dss_overlay_t *pov_req_prev = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;

	pov_req_prev = &(hisifd->ov_req_prev);
	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req_prev->ov_block_infos_ptr);

	/* gpu compose only one block */
	if (pov_req_prev->ov_block_nums > 1)
		return false;

	if (!pov_h_block_infos) {
		HISI_FB_INFO("pov_h_block_infos is NULL!\n");
		return false;
	}

	/* gpu compose only one layer */
	pov_h_block = &(pov_h_block_infos[0]);
	if (pov_h_block->layer_nums > 1)
		return false;

	/* use chn V0 for TUI compose except dallas */
	layer = &(pov_h_block->layer_infos[0]);
	if (layer->chn_idx == TUI_SEC_RCH)
		return false;

	return true;
}

static void hisifd_notify_secure_switch(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_secure *secure_ctrl = NULL;

	if (!hisifd) {
		HISI_FB_INFO("hisifd is NULL!\n");
		return;
	}
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	secure_ctrl = &(hisifd->secure_ctrl);
	if ((hisifd->ov_req.sec_enable_status == DSS_SEC_ENABLE) &&
		(check_tui_layer_chn_cfg_ok(hisifd)) &&
		(secure_ctrl->secure_status != secure_ctrl->secure_event))
		schedule_work(&secure_ctrl->secure_ctrl_work);
}

void hisifb_atf_config_security(uint32_t master_op_type, uint32_t channel, uint32_t mode)
{
#if defined(CONFIG_DRMDRIVER)
	configure_dss_service_security(master_op_type, channel, mode);
#endif
}

void hisifb_secure_register(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL\n");
		return;
	}
	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL");
		return;
	}
	secure_ctrl = &(hisifd->secure_ctrl);

	if (secure_ctrl->secure_created)
		return;

	secure_ctrl->secure_status = DSS_SEC_IDLE;
	secure_ctrl->secure_event  = DSS_SEC_DISABLE;
	secure_ctrl->secure_blank_flag = 0;
	secure_ctrl->tui_need_switch = 0;
	secure_ctrl->have_set_backlight = false;

	INIT_WORK(&secure_ctrl->secure_ctrl_work, hisifb_secure_ctrl_wq_handler);

#if defined(CONFIG_TEE_TUI)
	if (hisifd->index == PRIMARY_PANEL_IDX)
		/* register dss tui process function to sw */
		register_tui_driver(notify_dss_tui_request, "DSS", hisifd);
#endif
	secure_ctrl->notify_secure_switch = hisifd_notify_secure_switch;

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	secure_ctrl->hdcp13_encrypt_enable = hisi_hdcp13_encrypt_enable;
	secure_ctrl->hdcp13_enable = hisi_hdcp13_enable;
	secure_ctrl->hdcp22_enable = hisi_hdcp22_enable;
	secure_ctrl->hdcp_dpc_sec_en = hisi_hdcp_dpc_sec_en;
	secure_ctrl->hdcp_obs_set = hisi_hdcp_obs_set;
	secure_ctrl->hdcp_int_clr = hisi_hdcp_int_clr;
	secure_ctrl->hdcp_int_mask = hisi_hdcp_int_mask;
	secure_ctrl->hdcp_cp_irq = hisi_hdcp_cp_irq;
	secure_ctrl->hdcp_reg_get = hisi_hdcp_reg_get;
	secure_ctrl->hdcp_enc_mode = hisi_hdcp_enc_mode;
#endif

	secure_ctrl->hisifd = hisifd;

	secure_ctrl->secure_created = 1;

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (hisifd->sysfs_attrs_append_fnc != NULL)
			hisifd->sysfs_attrs_append_fnc(hisifd, &dev_attr_dss_secure.attr);
	}
}

void hisifb_secure_unregister(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_secure *secure_ctrl = NULL;

	if (!pdev) {
		HISI_FB_ERR("pdev is NULL");
		return;
	}
	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		dev_err(&pdev->dev, "hisifd is NULL");
		return;
	}
	secure_ctrl = &(hisifd->secure_ctrl);

	if (!secure_ctrl->secure_created)
		return;

	secure_ctrl->secure_created = 0;
}

#pragma GCC diagnostic pop

