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
#include <linux/hisi-iommu.h>
#include <linux/hisi/hisi_svm.h>

#include "hisi_dss_smmuv3.h"
#include "overlay/hisi_overlay_utils.h"
#include "hisi_dss_iommu.h"

static bool g_smmuv3_inited = false;
static int g_tbu_sr_refcount;
static int g_tbu0_cnt_refcount;
static bool offline_tbu1_conected = false;
static struct mutex g_tbu_sr_lock;

typedef bool (*smmu_event_cmp)(uint32_t value, uint32_t expect_value);

bool hisi_smmu_connect_event_cmp(uint32_t value, uint32_t expect_value)
{
	if (((value & 0x3) != 0x3) || (((value >> 8) & 0x7f) < expect_value))
		return false;

	return true;
}

bool hisi_smmu_disconnect_event_cmp(uint32_t value, uint32_t expect_value)
{
	void_unused(expect_value);
	if ((value & 0x3) != 0x1)
		return false;

	return true;
}

static void hisi_smmu_event_request(char __iomem *smmu_base,
	smmu_event_cmp cmp_func, enum smmu_event event, uint32_t check_value)
{
	uint32_t smmu_tbu_crack;
	uint32_t delay_count = 0;

	hisi_check_and_no_retval(!smmu_base, ERR, "smmu_base is nullptr!\n");
	/* request event config */
	set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(smmu_base), event, 1, 0);

	/* check ack */
	do {
		smmu_tbu_crack = inp32(SOC_SMMUv3_TBU_SMMU_TBU_CRACK_ADDR(smmu_base));
		if (cmp_func(smmu_tbu_crack, check_value))
			break;

		udelay(1);
		delay_count++;
	} while (delay_count < SMMU_TIMEOUT);

	if (delay_count == SMMU_TIMEOUT) {
		hisi_smmu_dump_tbu_status(__hdss_get_dev());
		HISI_FB_ERR("smmu=0x%x event=%d smmu_tbu_crack=0x%x check_value=0x%x!\n", /*lint !e559*/
			smmu_base, event, smmu_tbu_crack, check_value);
	}
}

void hisi_dss_smmuv3_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *tbu_base = NULL;
	int ret;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	mutex_lock(&g_tbu_sr_lock);
	if (g_tbu_sr_refcount == 0) {
		ret = regulator_enable(hisifd->smmu_tcu_regulator->consumer);
		if (ret)
			HISI_FB_ERR("fb%u smmu tcu regulator_enable failed, error=%d!\n", hisifd->index, ret);

		/* cmdlist select tbu1 config stream bypass */
		set_reg(hisifd->dss_base + DSS_VBIF0_AIF + AIF_CH_CTL_CMD, 0x1, 1, 0);
		set_reg(hisifd->dss_base + DSS_VBIF0_AIF + MMU_ID_ATTR_NS_13, 0x3F, 32, 0);
		set_reg(hisifd->dss_base + VBIF0_MIF_OFFSET + AIF_CMD_RELOAD, 0x1, 1, 0);
	}

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		if (g_tbu0_cnt_refcount == 0) {
			hisifb_atf_config_security(DSS_SMMU_INIT, 0, OFFLINE_COMPOSE_MODE);
			tbu_base = hisifd->dss_base + DSS_SMMU1_OFFSET;
			hisi_smmu_event_request(tbu_base, hisi_smmu_connect_event_cmp,
				TBU_CONNECT, DSS_TBU1_DTI_NUMS);
		}
		offline_tbu1_conected = true;
	} else {
		if (g_tbu0_cnt_refcount == 0) {
			hisifb_atf_config_security(DSS_SMMU_INIT, 0, ONLINE_COMPOSE_MODE);
			tbu_base = hisifd->dss_base + DSS_SMMU_OFFSET;
			hisi_smmu_event_request(tbu_base, hisi_smmu_connect_event_cmp,
				TBU_CONNECT, DSS_TBU0_DTI_NUMS);

			hisifb_atf_config_security(DSS_SMMU_INIT, 0, OFFLINE_COMPOSE_MODE);
			tbu_base = hisifd->dss_base + DSS_SMMU1_OFFSET;
			hisi_smmu_event_request(tbu_base, hisi_smmu_connect_event_cmp,
				TBU_CONNECT, DSS_TBU1_DTI_NUMS);
		}
		g_tbu0_cnt_refcount++;
	}

	g_tbu_sr_refcount++;

	HISI_FB_DEBUG("fb%u tbu_sr_refcount=%d, tbu0_cnt_refcount=%d, offline_tbu1_conected=%d",
		hisifd->index, g_tbu_sr_refcount, g_tbu0_cnt_refcount, offline_tbu1_conected);

	mutex_unlock(&g_tbu_sr_lock);
}

int hisi_dss_smmuv3_off(struct hisi_fb_data_type *hisifd)
{
	char __iomem *tbu_base = NULL;
	int ret = 0;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is nullptr\n");
	mutex_lock(&g_tbu_sr_lock);
	g_tbu_sr_refcount--;

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		if (g_tbu0_cnt_refcount == 0) {
			tbu_base = hisifd->dss_base + DSS_SMMU1_OFFSET;
			hisi_smmu_event_request(tbu_base, hisi_smmu_disconnect_event_cmp, TBU_DISCONNECT, 0);
		}
		offline_tbu1_conected = false;
	} else {
		g_tbu0_cnt_refcount--;
		if (g_tbu0_cnt_refcount == 0) {
			/* keep the connection if tbu0 is used by fb2 cmdlist */
			tbu_base = hisifd->dss_base + DSS_SMMU_OFFSET;
			hisi_smmu_event_request(tbu_base, hisi_smmu_disconnect_event_cmp, TBU_DISCONNECT, 0);
			if (!offline_tbu1_conected) {
				tbu_base = hisifd->dss_base + DSS_SMMU1_OFFSET;
				hisi_smmu_event_request(tbu_base, hisi_smmu_disconnect_event_cmp, TBU_DISCONNECT, 0);
			}
		}
	}

	if (g_tbu_sr_refcount == 0) {
		ret = regulator_disable(hisifd->smmu_tcu_regulator->consumer);
		if (ret)
			HISI_FB_ERR("fb%u smmu tcu regulator_disable failed, error=%d!\n", hisifd->index, ret);
	}
	HISI_FB_DEBUG("fb%u tbu_sr_refcount=%d, tbu0_cnt_refcount=%d, offline_tbu1_conected=%d",
		hisifd->index, g_tbu_sr_refcount, g_tbu0_cnt_refcount, offline_tbu1_conected);

	mutex_unlock(&g_tbu_sr_lock);

	return ret;
}

void hisi_mdc_smmu_on(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int i = 0;
	char __iomem *addr_base = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	ret = regulator_enable(hisifd->smmu_tcu_regulator->consumer);
	if (ret)
		HISI_FB_ERR("smmu tcu regulator_enable failed, error=%d!\n", ret);

	addr_base = hisifd->media_common_base + VBIF0_SMMU_OFFSET;
	set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(addr_base), 0x10100F03, 32, 0);
	hisi_smmu_event_request(addr_base, hisi_smmu_connect_event_cmp,
		TBU_CONNECT, DSS_TBU1_DTI_NUMS);

	/* enable mdc tbu pref, num=33 was reference chip manual */
	for (; i < MDC_SWID_NUMS; i++)
		set_reg(SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_ADDR(addr_base, i), 1, 1, 31);
}

int hisi_mdc_smmu_off(struct hisi_fb_data_type *hisifd)
{
	int ret;
	char __iomem *addr_base = NULL;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is null!\n");

	addr_base = hisifd->media_common_base + VBIF0_SMMU_OFFSET;
	hisi_smmu_event_request(addr_base, hisi_smmu_disconnect_event_cmp, TBU_DISCONNECT, 0);

	ret = regulator_disable(hisifd->smmu_tcu_regulator->consumer);
	if (ret)
		HISI_FB_ERR("smmu tcu regulator_disable failed, error=%d!\n", ret);

	return ret;
}

int hisi_dss_smmu_ch_config(struct hisi_fb_data_type *hisifd,
	dss_layer_t *layer, dss_wb_layer_t *wb_layer)
{
	dss_img_t *img = NULL;
	dss_smmu_t *smmu = NULL;
	int chn_idx;
	uint32_t ssid;
	soc_dss_mmu_id_attr taget;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is null!\n");

	if (wb_layer) {
		img = &(wb_layer->dst);
		chn_idx = wb_layer->chn_idx;
	} else if (layer) {
		img = &(layer->img);
		chn_idx = layer->chn_idx;
	} else {
		HISI_FB_ERR("layer or wb_layer is NULL");
		return -EINVAL;
	}

	smmu = &(hisifd->dss_module.smmu);
	hisifd->dss_module.smmu_used = 1;

	taget.value = 0;
	taget.reg.ch_sid = img->mmu_enable ? 0x1 : 0x3F;
	taget.reg.ch_ssidv = 1;

	ssid = hisi_dss_get_ssid(hisifd->ov_req.frame_no);
	if (hisifd->index == PRIMARY_PANEL_IDX)
		taget.reg.ch_ssid = ssid + HISI_PRIMARY_DSS_SSID_OFFSET;
	else if (hisifd->index == EXTERNAL_PANEL_IDX)
		taget.reg.ch_ssid = ssid + HISI_EXTERNAL_DSS_SSID_OFFSET;
	else
		taget.reg.ch_ssid = HISI_OFFLINE_SSID;

	smmu->ch_mmu_attr[chn_idx].value = taget.value;
	HISI_FB_DEBUG("config ch_mmu_attr[%d].value=0x%x, ssid=%u\n",
		chn_idx, smmu->ch_mmu_attr[chn_idx].value, taget.reg.ch_ssid);

	return 0;
}

void hisi_dss_smmu_ch_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *smmu_base, dss_smmu_t *s_smmu, int chn_idx)
{
	char __iomem *addr = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	if (chn_idx == DSS_RCHN_V2)
		addr = hisifd->dss_base + DSS_VBIF0_AIF + MMU_ID_ATTR_NS_11;
	else
		addr = hisifd->dss_base + DSS_VBIF0_AIF + MMU_ID_ATTR_NS_0 + chn_idx * 0x4;

	if (hisifd->index == MEDIACOMMON_PANEL_IDX)
		addr = hisifd->media_common_base + DSS_VBIF0_AIF + MMU_ID_ATTR_NS_0 + chn_idx * 0x4;

	hisifd->set_reg(hisifd, addr, s_smmu->ch_mmu_attr[chn_idx].value, 32, 0);

	HISI_FB_DEBUG("config ch_mmu_attr[%d].value=0x%x, ssid=%u\n",
		chn_idx, s_smmu->ch_mmu_attr[chn_idx].value,
		s_smmu->ch_mmu_attr[chn_idx].reg.ch_ssid);
}

/* for dssv600 es chip bugfix pref lock clean fail */
void hisi_dss_mmu_lock_clear(struct hisi_fb_data_type *hisifd)
{
	char __iomem *addr = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	if (hisifd->index != AUXILIARY_PANEL_IDX) {
		/* set tbu0 lock clear */
		addr = hisifd->dss_base + DSS_SMMU_OFFSET;
		set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(addr), 0x3, 2, 1);
		outp32(SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_ADDR(addr), 0x1);
		set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(addr), 0x1, 2, 1);
	}

	/* set tbu1 lock clear */
	addr = hisifd->dss_base + DSS_SMMU1_OFFSET;
	set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(addr), 0x3, 2, 1);
	outp32(SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_ADDR(addr), 0x1);
	set_reg(SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(addr), 0x1, 2, 1);
}

void hisi_dss_mmu_tlb_flush(struct hisi_fb_data_type *hisifd, uint32_t frame_no)
{
	uint32_t ssid;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");
	ssid = hisi_dss_get_ssid(frame_no);
	if (hisifd->index == PRIMARY_PANEL_IDX)
		ssid = ssid + HISI_PRIMARY_DSS_SSID_OFFSET;
	else if (hisifd->index == EXTERNAL_PANEL_IDX)
		ssid = ssid + HISI_EXTERNAL_DSS_SSID_OFFSET;
	else
		ssid = HISI_OFFLINE_SSID;

	hisi_iommu_dev_flush_tlb(__hdss_get_dev(), ssid);
	HISI_FB_DEBUG("hisi_iommu_dev_flush_tlb ssid=%u\n", ssid);
}

void hisi_dss_smmuv3_deinit(void)
{
	if (g_smmuv3_inited) {
		g_smmuv3_inited = false;
		mutex_destroy(&g_tbu_sr_lock);
	}
}

void hisi_dss_smmuv3_init(void)
{
	if (!g_smmuv3_inited) {
		mutex_init(&g_tbu_sr_lock);
		g_smmuv3_inited = true;
	}
}

