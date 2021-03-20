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

#include "hisi_fb.h"
#include <linux/device.h>

#define DTS_COMP_HISI_MDC_PANEL	"hisilicon,mediacommon_panel"

static int hisi_mediacommon_panel_on(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is null!\n");
	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is null!\n");

	if (mdc_clk_regulator_enable(hisifd)) {
		HISI_FB_ERR("mdc clk regulator enable failed");
		return -EINVAL;
	}

	hisi_mdc_interrupt_config(hisifd);
	if (!hisifd->dss_module_resource_initialized) {
		hisi_mdc_module_default(hisifd);
		hisifd->dss_module_resource_initialized = true;
	}
	hisi_mdc_mif_on(hisifd);
	hisi_mdc_smmu_on(hisifd);
	hisi_mdc_scl_coef_on(hisifd, SCL_COEF_YUV_IDX);

	hisi_mdc_mctl_on(hisifd);

	return 0;
}

static int hisi_mediacommon_panel_off(struct platform_device *pdev)
{
	int ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is null!\n");
	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is null!\n");

#ifdef CONFIG_DSS_SMMU_V3
	ret = hisi_mdc_smmu_off(hisifd);
#endif
	mdc_clk_regulator_disable(hisifd);

	return ret;
}

static int hisi_mediacommon_panel_remove(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is null!\n");
	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is null!\n");

	return 0;
}

static struct hisi_panel_info hisi_mdc_panel_info = {0};
static struct hisi_fb_panel_data hisi_mdc_panel_data = {
	.panel_info = &hisi_mdc_panel_info,
	.on = hisi_mediacommon_panel_on,
	.off = hisi_mediacommon_panel_off,
	.remove = hisi_mediacommon_panel_remove,
};

static int hisi_mdc_probe(struct platform_device *pdev)
{
	int ret;
	struct hisi_panel_info *pinfo = NULL;
	struct platform_device *this_dev = NULL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (hisi_fb_device_probe_defer(PANEL_MEDIACOMMON, BL_SET_BY_NONE))
		goto err_probe_defer;

	if (!pdev)
		goto err_probe_defer;

	pdev->id = 1; /*lint !e613*/
	pinfo = hisi_mdc_panel_data.panel_info;
	pinfo->xres = g_primary_lcd_xres;
	pinfo->yres = g_primary_lcd_yres;
	pinfo->type = PANEL_MEDIACOMMON;
	pinfo->pxl_clk_rate = g_pxl_clk_rate;

	/* alloc panel device data */
	ret = platform_device_add_data(pdev, &hisi_mdc_panel_data,
		sizeof(struct hisi_fb_panel_data));
	if (ret) {
		dev_err(&pdev->dev, "platform_device_add_data failed!\n"); /*lint !e613*/
		goto err_device_put;
	}

	this_dev = hisi_fb_add_device(pdev);
	if (this_dev != NULL)
		hisifd = platform_get_drvdata(this_dev);

	if (hisifd) {
		hisifd->dss_aclk_media_common_clk = devm_clk_get(&pdev->dev, "aclk_media_common");
		if (IS_ERR(hisifd->dss_aclk_media_common_clk)) {
			ret = PTR_ERR(hisifd->dss_aclk_media_common_clk);
			HISI_FB_ERR("dss_aclk_media_common_clk error, ret=%d", ret);
		}

		hisifd->dss_clk_media_common_clk = devm_clk_get(&pdev->dev, "clk_media_common");
		if (IS_ERR(hisifd->dss_clk_media_common_clk)) {
			ret = PTR_ERR(hisifd->dss_clk_media_common_clk);
			HISI_FB_ERR("dss_clk_media_common_clk error, ret=%d", ret);
		}

		hisifd->mdc_regulator = devm_regulator_get(&pdev->dev, "regulator_mdc");
		if (IS_ERR(hisifd->mdc_regulator)) {
			ret = PTR_ERR(hisifd->mdc_regulator);
			HISI_FB_ERR("mdc_regulator error, ret=%d", ret);
		}
	}

	return 0;

err_device_put:
	platform_device_put(pdev);
	return ret;

err_probe_defer:
	return -EPROBE_DEFER;
}

static const struct of_device_id hisi_device_match_table[] = {
	{
		.compatible = DTS_COMP_HISI_MDC_PANEL,
		.data = NULL,
	},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_device_match_table);

static struct platform_driver this_driver = {
	.probe = hisi_mdc_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = "hisi_mediacommon_panel",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisi_device_match_table),
	},
};

static int __init hisi_mdc_panel_init(void)
{
	int ret;

	ret = platform_driver_register(&this_driver);
	if (ret)
		pr_err("register mediacommon driver fail\n");

	return ret;
}

module_init(hisi_mdc_panel_init);

