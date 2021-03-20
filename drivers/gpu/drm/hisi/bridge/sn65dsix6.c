/*
 * sn65dsix6.c
 *
 * operate function for sn65dsix6
 *
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "edp_bridge.h"
#include "sn65dsix6.h"

#define MAX_CHIP_PROBE_TIMES	3
int is_sn65dsix6_onboard(struct mipi2edp *pdata)
{
	int i, ret, reg;

	/* check whether sn65dsix6 chip is on board */
	ret = 0;
	reg = 0;
	for (i = 0; i < MAX_CHIP_PROBE_TIMES; i++) {
		ret = regmap_read(pdata->regmap, SN_DEVICE_REV_REG, &reg);
		if (ret)
			continue;

		HISI_DRM_INFO("sn65dsix6 is onboard! device_rev is 0x%x\n", reg);
		return ret;
	}

	HISI_DRM_ERR("sn65dsix6 isn't onboard!\n");
	return ret;
}

void sn65dsi86_pre_enable(struct mipi2edp *pdata)
{
	/* wait for sn65dsi86 power up! */
	mdelay(1);
	/* configure bridge ref_clk */
	regmap_write(pdata->regmap, SN_DPPLL_SRC_REG, 0x08);

	regmap_update_bits(pdata->regmap, SN_HPD_DISABLE_REG, HPD_DISABLE,
		   HPD_DISABLE);
	/* wait for LCD inital finish! */
	mdelay(100);
	/* aux */
	regmap_write(pdata->regmap, SN_AUX_WDATA_REG(0), 0x01);
	regmap_write(pdata->regmap, SN_AUX_ADDR_19_16_REG, 0x00);
	regmap_write(pdata->regmap, SN_AUX_ADDR_15_8_REG, 0x01);
	regmap_write(pdata->regmap, SN_AUX_ADDR_7_0_REG, 0x0A);
	regmap_write(pdata->regmap, SN_AUX_LENGTH_REG, 0x01);
	regmap_write(pdata->regmap, SN_AUX_CMD_REG, 0x81);
}

static void video_param_config(struct mipi2edp *pdata)
{
	regmap_write(pdata->regmap, 0x20, 0x70);
	regmap_write(pdata->regmap, 0x21, 0x08);
	regmap_write(pdata->regmap, 0x22, 0x00);
	regmap_write(pdata->regmap, 0x23, 0x00);
	regmap_write(pdata->regmap, 0x24, 0xA0);
	regmap_write(pdata->regmap, 0x25, 0x05);
	regmap_write(pdata->regmap, 0x2C, 0x20);
	regmap_write(pdata->regmap, 0x2D, 0x00);
	regmap_write(pdata->regmap, 0x30, 0x0A);
	regmap_write(pdata->regmap, 0x31, 0x00);
	regmap_write(pdata->regmap, 0x34, 0x50);
	regmap_write(pdata->regmap, 0x36, 0x1B);
	regmap_write(pdata->regmap, 0x38, 0x30);
	regmap_write(pdata->regmap, 0x3A, 0x03);
	regmap_write(pdata->regmap, 0x5B, 0x00);
	regmap_write(pdata->regmap, 0x3C, 0x00);
}

int sn65dsi86_enable(struct mipi2edp *pdata)
{
	int val = 0;
	int ret = 0;

	/* DSI_A lane config */
	regmap_update_bits(pdata->regmap, SN_DSI_LANES_REG,
		CHA_DSI_LANES_MASK, 0x26);
	/* EQ for improve signal quality! */
	regmap_write(pdata->regmap, 0x11, 0xff);

	/* DP lane config */
	regmap_update_bits(pdata->regmap, SN_SSC_CONFIG_REG,
		DP_NUM_LANES_MASK, 0x34);
	regmap_update_bits(pdata->regmap, SN_DATARATE_CONFIG_REG,
		DP_DATARATE_MASK, 0x20);

	/* set dsi/dp clk frequency value */
	regmap_write(pdata->regmap, SN_DSIA_CLK_FREQ_REG,
		0x7C); /* 0x96 */

	/* regmap_update_bits(pdata->regmap, SN_DATARATE_CONFIG_REG,
	 * DP_DATARATE_MASK, DP_DATARATE(i))
	 */

	/* enable DP PLL */
	regmap_write(pdata->regmap, SN_PLL_ENABLE_REG, 1);

	ret = regmap_read_poll_timeout(pdata->regmap, SN_DPPLL_SRC_REG,
		val, val & DPPLL_SRC_DP_PLL_LOCK, 1000, 100 * 1000);
	if (ret) {
		HISI_DRM_ERR("DP_PLL_LOCK polling failed, ret=%d!", ret);
		return ret;
	}

	/**
	 * The SN65DSI86 only supports ASSR Display Authentication method and
	 * this method is enabled by default. An eDP panel must support this
	 * authentication method. We need to enable this method in the eDP panel
	 * at DisplayPort address 0x0010A prior to link training.
	 */
	drm_dp_dpcd_writeb(&pdata->aux, DP_EDP_CONFIGURATION_SET, DP_ALTERNATE_SCRAMBLER_RESET_ENABLE);

	regmap_write(pdata->regmap, 0x95, 0x00);

	/* Semi auto link training mode */
	regmap_write(pdata->regmap, 0x5A, 0x05);
	regmap_write(pdata->regmap, 0x96, 0x02);
	regmap_write(pdata->regmap, 0x93, 0x34);

	regmap_write(pdata->regmap, 0x96, 0x0A);
	mdelay(10);
	regmap_write(pdata->regmap, 0x96, 0x0A);
	mdelay(10);

	ret = regmap_read_poll_timeout(pdata->regmap, SN_ML_TX_MODE_REG, val,
		val == ML_TX_MAIN_LINK_OFF || val == ML_TX_NORMAL_MODE,
		1000, 500 * 1000);
	if (ret) {
		HISI_DRM_ERR("Training complete polling failed, ret=%d!", ret);
		return ret;
	} else if (val == ML_TX_MAIN_LINK_OFF) {
		HISI_DRM_ERR("Link training failed, link is off, ret=%d!", ret);
		return ret;
	}

	HISI_DRM_INFO("Link training OK!");

	/* config video parameters */
	video_param_config(pdata);
	/* enable video stream */
	regmap_write(pdata->regmap, 0x5A, 0x0D);

	return ret;
}

void sn65dsi86_disable(struct mipi2edp *pdata)
{
	/* disable video stream */
	regmap_update_bits(pdata->regmap, SN_ENH_FRAME_REG, VSTREAM_ENABLE, 0);
	/* semi auto link training mode OFF */
	regmap_write(pdata->regmap, SN_ML_TX_MODE_REG, 0);
	/* disable DP PLL */
	regmap_write(pdata->regmap, SN_PLL_ENABLE_REG, 0);
}
