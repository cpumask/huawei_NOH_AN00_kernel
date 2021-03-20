/*
 * hisi_vcodec_vdec_refulator.c
 *
 * This is for vdec regulator
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "hisi_vcodec_vdec_regulator.h"
#include "hisi_vcodec_vdec_plat.h"
#include <linux/device.h>
#include "securec.h"
#include "hisi_vcodec_vdec_dpm.h"
#include "vfmw_osal.h"

vdec_plat g_vdec_plat_entry;

#define VDEC_CLOCK_NAME "clk_vdec"
#define VDEC_CLK_RATE "clk_rate"
#define VDEC_TRANSITION_CLK_NAME "transi_clk_rate"
#define MEDIA2_DEFAULT_CLK_NAME "default_clk_rate"
#define VDEC_TRANSITION_CLK_VALUE 240000000
#define MEDIA2_DEFAULT_CLK_VALUE 104000000

const char *g_regulator_name[MAX_REGULATOR] = { "ldo_media", "ldo_vdec", "ldo_smmu_tcu" };

vdec_plat *vdec_plat_get_entry(void)
{
	return &g_vdec_plat_entry;
}

static hi_s32 vdec_plat_init_regulator(struct device *dev)
{
	hi_s32 i;
	hi_s32 ret;
	vdec_plat *plt = vdec_plat_get_entry();
	(void)memset_s(&plt->regulator_info, sizeof(vdec_regulator), 0, sizeof(vdec_regulator));

	for (i = 0; i < MAX_REGULATOR; i++) {
		plt->regulator_info.regulators[i] = devm_regulator_get(dev, g_regulator_name[i]);
		if (IS_ERR_OR_NULL(plt->regulator_info.regulators[i])) {
			dprint(PRN_FATAL, "get regulator: %s failed\n", g_regulator_name[i]);
			return HI_FAILURE;
		}
	}

	plt->regulator_info.clk_vdec = devm_clk_get(dev, VDEC_CLOCK_NAME);
	if (IS_ERR_OR_NULL(plt->regulator_info.clk_vdec)) {
		dprint(PRN_FATAL, "get clk failed\n");
		return HI_FAILURE;
	}

	for (i = CLK_LEVEL_FHD_60FPS; i < CLK_LEVEL_MAX; i++) {
		ret = of_property_read_u32_index(dev->of_node, VDEC_CLK_RATE, i,
			&plt->regulator_info.clk_values[i]);
		if (ret) {
			dprint(PRN_FATAL, "read %d clk failed\n", i);
			return HI_FAILURE;
		}
	}

	/* transi_clk_rate & default_clk_rate are used for clk config constraint */
	ret = of_property_read_u32(dev->of_node, VDEC_TRANSITION_CLK_NAME,
		&plt->regulator_info.transi_clk_rate);
	if (ret) {
		dprint(PRN_WARN, "read %s failed set to default\n",
			VDEC_TRANSITION_CLK_NAME);
		plt->regulator_info.transi_clk_rate = VDEC_TRANSITION_CLK_VALUE;
	}

	ret = of_property_read_u32(dev->of_node, MEDIA2_DEFAULT_CLK_NAME,
		&plt->regulator_info.default_clk_rate);
	if (ret) {
		plt->regulator_info.default_clk_rate = MEDIA2_DEFAULT_CLK_VALUE;
		dprint(PRN_WARN, "read %s failed set to default\n", MEDIA2_DEFAULT_CLK_NAME);
	}

	return HI_SUCCESS;
}

#ifdef USER_DISABLE_VDEC_PROC
static void vdec_plat_dump_info(vdec_plat *plt)
{
	hi_s32 i;

	dprint(PRN_ALWS, "irq_norm %u\n", plt->dts_info.irq_norm);
	dprint(PRN_ALWS, "irq_safe %u\n", plt->dts_info.irq_safe);
	dprint(PRN_ALWS, "is_fpga %u\n", plt->dts_info.is_fpga);
	dprint(PRN_ALWS, "is_es_plat %u\n", plt->dts_info.is_es_plat);
	dprint(PRN_ALWS, "transi_rate %u\n", plt->regulator_info.transi_clk_rate);
	dprint(PRN_ALWS, "default_rate %u\n", plt->regulator_info.default_clk_rate);

	for (i = 0; i < MAX_REGULATOR; i++)
		dprint(PRN_ALWS, "regulator addr %#x\n", plt->regulator_info.regulators[i]);

	dprint(PRN_ALWS, "cld_vdec addr %#x\n", plt->regulator_info.clk_vdec);

	for (i = 0; i < CLK_LEVEL_MAX; i++)
		dprint(PRN_ALWS, "clk_value %u\n", plt->regulator_info.clk_values[i]);

	for (i = 0; i < MAX_INNER_MODULE; i++) {
		dprint(PRN_ALWS, "module reg phy: %#x\n", plt->dts_info.module_reg[i].reg_phy_addr);
		dprint(PRN_ALWS, "module reg size: %#x\n", plt->dts_info.module_reg[i].reg_range);
	}
}
#endif

static hi_s32 vdec_plat_init_dts(struct device *dev)
{
	hi_s32 ret;
	hi_s32 i;
	struct resource res;
	vdec_plat *plt_info = vdec_plat_get_entry();
	(void)memset_s(&plt_info->dts_info, sizeof(vdec_dts), 0, sizeof(vdec_dts));
	(void)memset_s(&res, sizeof(res), 0, sizeof(res));

	plt_info->dts_info.irq_norm = irq_of_parse_and_map(dev->of_node, 0);
	plt_info->dts_info.irq_safe = irq_of_parse_and_map(dev->of_node, 1);
	if (plt_info->dts_info.irq_norm == 0 || plt_info->dts_info.irq_safe == 0) {
		dprint(PRN_ERROR, "irq_of_parse_and_map failed\n");
		return HI_FAILURE;
	}

	for (i = 0; i < MAX_INNER_MODULE; i++) {
		ret = of_address_to_resource(dev->of_node, i, &res);
		if (ret) {
			dprint(PRN_FATAL, "read dec inner module reg %d info failed\n", i);
			return HI_FAILURE;
		}

		plt_info->dts_info.module_reg[i].reg_phy_addr = res.start;
		plt_info->dts_info.module_reg[i].reg_range = resource_size(&res);
	}

	ret = of_property_read_u32(dev->of_node, "vdec_fpga", &plt_info->dts_info.is_fpga);
	if (ret) {
		dprint(PRN_WARN, "get plat vdec_fpga flag failed\n");
		plt_info->dts_info.is_fpga = 0;
	}

	ret = of_property_read_u32(dev->of_node, "vdec_es", &plt_info->dts_info.is_es_plat);
	if (ret) {
		dprint(PRN_WARN, "get plat vdec_es flag failed\n");
		plt_info->dts_info.is_es_plat = 0;
	}

	return HI_SUCCESS;
}

static void vdec_plat_deinit_regulator(vdec_regulator *regulator_info)
{
	(void)memset_s(regulator_info, sizeof(vdec_regulator), 0, sizeof(vdec_regulator));
}

static void vdec_plat_deinit_dts(vdec_dts *dts_info)
{
	(void)memset_s(dts_info, sizeof(vdec_dts), 0, sizeof(vdec_dts));
}

hi_s32 vdec_plat_init(struct platform_device *plt_dev)
{
	hi_s32 ret;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_init_mutex(&plt->vdec_plat_mutex);

	ret = vdec_plat_init_regulator(&plt_dev->dev);
	if (ret) {
		dprint(PRN_FATAL, "regulator init failed\n");
		return HI_FAILURE;
	}

	ret = vdec_plat_init_dts(&plt_dev->dev);
	if (ret) {
		dprint(PRN_FATAL, "dts init failed\n");
		return HI_FAILURE;
	}

	plt->dev = &plt_dev->dev;
	plt->dts_info.dev = plt->dev;

	plt->plt_init = 1;
	plt->power_flag = 0;
	plt->resume_clk = VDEC_CLK_RATE_LOWER;

#ifdef USER_DISABLE_VDEC_PROC
	if (dcheck(PRN_DBG))
		vdec_plat_dump_info(plt);
#endif

	return HI_SUCCESS;
}

hi_s32 vdec_plat_deinit(void)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_plat_deinit_regulator(&plt->regulator_info);

	vdec_plat_deinit_dts(&plt->dts_info);

	plt->power_flag = 0;
	plt->plt_init = 0;

	return HI_SUCCESS;
}

hi_s32 vdec_plat_regulator_enable(void)
{
	hi_s32 ret;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_check_ret(plt->plt_init, HI_FAILURE);

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	if (plt->power_flag) {
		dprint(PRN_ALWS, "regulator already enable\n");
		vdec_mutex_unlock(&plt->vdec_plat_mutex);
		return HI_SUCCESS;
	}

	ret = regulator_enable(plt->regulator_info.regulators[MEDIA_REGULATOR]);
	if (ret) {
		dprint(PRN_FATAL, "enable media regulator failed\n");
		goto exit;
	}

	ret = clk_prepare_enable(plt->regulator_info.clk_vdec);
	if (ret) {
		dprint(PRN_FATAL, "clk enable failed\n");
		goto disable_media;
	}

	ret = regulator_enable(plt->regulator_info.regulators[VDEC_REGULATOR]);
	if (ret) {
		dprint(PRN_FATAL, "enable vdec regulator failed\n");
		goto unprepare_clk;
	}

	plt->power_flag = 1;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	dprint(PRN_CTRL, "success\n");
	return HI_SUCCESS;

unprepare_clk:
	clk_disable_unprepare(plt->regulator_info.clk_vdec);

disable_media:
	if (regulator_disable(plt->regulator_info.regulators[MEDIA_REGULATOR]))
		dprint(PRN_ALWS, "disable media regulator failed\n");
exit:
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	return HI_FAILURE;
}

void vdec_plat_regulator_disable(void)
{
	vdec_plat *plt = vdec_plat_get_entry();

	if (!(plt->plt_init)) {
		dprint(PRN_ERROR, "assert condition not match\n");
		return;
	}
	vdec_mutex_lock(&plt->vdec_plat_mutex);

	if (!plt->power_flag) {
		dprint(PRN_ALWS, "regulator already disable\n");
		vdec_mutex_unlock(&plt->vdec_plat_mutex);
		return;
	}

	(void)regulator_disable(plt->regulator_info.regulators[VDEC_REGULATOR]);

	(void)clk_set_rate(plt->regulator_info.clk_vdec, plt->regulator_info.default_clk_rate);

	clk_disable_unprepare(plt->regulator_info.clk_vdec);

	(void)regulator_disable(plt->regulator_info.regulators[MEDIA_REGULATOR]);

	plt->power_flag = 0;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	dprint(PRN_CTRL, "success\n");
}

void vdec_plat_regulator_get_clk_rate(clk_rate_e *clk_rate)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	*clk_rate = plt->resume_clk;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

static void vdec_plat_set_transition_clk(void)
{
	hi_s32 ret;
	hi_u32 current_clk_rate;
	vdec_plat *plt = vdec_plat_get_entry();

	current_clk_rate = (hi_u32)clk_get_rate(plt->regulator_info.clk_vdec);
	/* set a transit frequence to avoid high frequence low voltage problem */
	if (current_clk_rate == plt->regulator_info.clk_values[CLK_LEVEL_EXTREME] ||
		current_clk_rate == plt->regulator_info.clk_values[CLK_LEVEL_UHD_60FPS]) {
		dprint(PRN_ALWS, "current_clk_rate is %u need set transi_clk_rate %u\n",
			current_clk_rate, plt->regulator_info.transi_clk_rate);
		ret = clk_set_rate(plt->regulator_info.clk_vdec, plt->regulator_info.transi_clk_rate);
		if (ret)
			dprint(PRN_ERROR, "failed set transi_clk_rate to %u Hz,fail code is %d\n",
				plt->regulator_info.transi_clk_rate, ret);
	}
}

static hi_s32 vdec_plat_switch_clk(clk_rate_e dst_clk_type, hi_u32 dst_clk_rate)
{
	hi_s32 ret;
	hi_u32 current_clk_rate;
	clk_rate_e actual_clk_type;
	hi_u32 actual_clk_rate;
	vdec_plat *plt = vdec_plat_get_entry();

	current_clk_rate = (hi_u32)clk_get_rate(plt->regulator_info.clk_vdec);
	if (dst_clk_rate == current_clk_rate)
		return HI_SUCCESS;

	vdec_plat_set_transition_clk();

	ret = clk_set_rate(plt->regulator_info.clk_vdec, dst_clk_rate);
	actual_clk_type = dst_clk_type;
	if (ret && dst_clk_rate == plt->regulator_info.clk_values[CLK_LEVEL_EXTREME]) {
		actual_clk_rate = plt->regulator_info.clk_values[CLK_LEVEL_UHD_60FPS];
		actual_clk_type = VDEC_CLK_RATE_NORMAL;
		ret = clk_set_rate(plt->regulator_info.clk_vdec, actual_clk_rate);
		dprint(PRN_ALWS, "low temperature state: %u\n", actual_clk_rate);
		if (ret) {
			dprint(PRN_ERROR, "failed set clk to %u Hz,fail code is %d\n",
				dst_clk_rate, ret);
			return HI_FAILURE;
		}
	}

	plt->resume_clk = actual_clk_type;

	return HI_SUCCESS;
}

hi_s32 vdec_plat_regulator_set_clk_rate(clk_rate_e dst_clk_type)
{
	hi_u32 dst_clk_rate;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_check_ret(plt->plt_init, HI_FAILURE);

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	switch (dst_clk_type) {
	case VDEC_CLK_RATE_LOWER:
		dst_clk_rate = plt->regulator_info.clk_values[CLK_LEVEL_FHD_60FPS];
		break;
	case VDEC_CLK_RATE_LOW:
		dst_clk_rate = plt->regulator_info.clk_values[CLK_LEVEL_UHD_30FPS];
		break;
	case VDEC_CLK_RATE_NORMAL:
		dst_clk_rate = plt->regulator_info.clk_values[CLK_LEVEL_UHD_60FPS];
		break;
	case VDEC_CLK_RATE_HIGH:
	case VDEC_CLK_RATE_HEIF:
		dst_clk_rate = plt->regulator_info.clk_values[CLK_LEVEL_EXTREME];
		break;
	default:
		dprint(PRN_ERROR, "unsupport clk rate type %d\n", dst_clk_type);
		goto error_exit;
	}

	if (vdec_plat_switch_clk(dst_clk_type, dst_clk_rate) != HI_SUCCESS)
		goto error_exit;

#ifdef VDEC_DPM_ENABLE
	vdec_dpm_freq_select(dst_clk_type > VDEC_CLK_RATE_NORMAL ? VDEC_CLK_RATE_HIGH : dst_clk_type);
#endif
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	dprint(PRN_CTRL, "set clk_rate value: %u\n", dst_clk_rate);
	return HI_SUCCESS;

error_exit:
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	return HI_FAILURE;
}

