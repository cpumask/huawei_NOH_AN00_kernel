/*
 * edp_bridge.c
 *
 * i2c driver for mipi2edp bridge
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
/*lint -e548 -e574 -e578*/
#include <securec.h>
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_edid.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <linux/component.h>
#include <linux/pm_runtime.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/leds.h>
#include <linux/of_gpio.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/semaphore.h>
#include <linux/backlight.h>
#include "linux/mfd/hisi_pmic.h"
#include "edp_bridge.h"
#include "sn65dsix6.h"
#include "lt9711a.h"

#define  GPIO_ID_HIGH 0x01
#define  GPIO_ID_LOW  0x0
#define  EDID_LEN  128

static char g_edid[EDID_LEN];

static struct pinctrl_data pinctrl;
static struct gpio_config  gpio_set;

static struct pinctrl_cmd_desc bridge_pinctrl_init_cmds[] = {
	{DTYPE_PINCTRL_GET, &pinctrl, 0},
	{DTYPE_PINCTRL_STATE_GET, &pinctrl, DTYPE_PINCTRL_STATE_DEFAULT},
	{DTYPE_PINCTRL_STATE_GET, &pinctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc bridge_pinctrl_normal_cmds[] = {
	{DTYPE_PINCTRL_SET, &pinctrl, DTYPE_PINCTRL_STATE_DEFAULT},
};

static struct gpio_desc bridge_gpio_set_init_cmds[] = {
	/*init enable gpio, set high to enable bridge*/
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, "hw_enable_gpio", &gpio_set.hw_enable_gpio, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "hw_enable_gpio", &gpio_set.hw_enable_gpio, 1},
	/*init suspend gpio,set low to enable bridge*/
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, "hw_suspend_gpio", &gpio_set.hw_suspend_gpio, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "hw_suspend_gpio", &gpio_set.hw_suspend_gpio, 0},
	/*lcd vdd enable gpio, set high to enable vdd*/
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, "lcd_vdd_enable_gpio", &gpio_set.lcd_vdd_enable_gpio, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "lcd_vdd_enable_gpio", &gpio_set.lcd_vdd_enable_gpio, 1},
	/*bl vdd enable gpio, set high to enable bl*/
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, "bl_config_enable_gpio", &gpio_set.bl_config_enable_gpio, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "bl_config_enable_gpio", &gpio_set.bl_config_enable_gpio, 1},
};

static struct gpio_desc bridge_gpio_enable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "hw_enable_gpio", &gpio_set.hw_enable_gpio, 1},
};

static struct gpio_desc bridge_gpio_disable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "hw_enable_gpio", &gpio_set.hw_enable_gpio, 0},
};

static struct gpio_desc bridge_lcd_vdd_enable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "lcd_vdd_enable_gpio", &gpio_set.lcd_vdd_enable_gpio, 1},
};

static struct gpio_desc bridge_lcd_vdd_disable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "lcd_vdd_enable_gpio", &gpio_set.lcd_vdd_enable_gpio, 0},
};

static struct gpio_desc bridge_bl_config_enable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "bl_config_enable_gpio", &gpio_set.bl_config_enable_gpio, 1},
};

static struct gpio_desc bridge_bl_config_disable_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 0, "bl_config_enable_gpio", &gpio_set.bl_config_enable_gpio, 0},
};

static const struct drm_display_mode default_mode_lcd = {
	.type = DRM_MODE_TYPE_DEFAULT,
	.clock = 206016,
	.hdisplay = 2160,
	.hsync_start = 2160 + 48,
	.hsync_end = 2160 + 48 + 80,
	.htotal = 2160 + 48 + 80 + 32,
	.vdisplay = 1440,
	.vsync_start = 1440 + 3,
	.vsync_end = 1440 + 3 + 27,
	.vtotal = 1440 + 3 + 27 + 10,
	.vrefresh = 60,
};

static const struct drm_display_mode default_mode_vga = {
	.type = DRM_MODE_TYPE_DEFAULT,
	.clock = 148500,
	.hdisplay = 1920,
	.hsync_start = 1920 + 88,
	.hsync_end = 1920 + 88 + 148,
	.htotal = 1920 + 88 + 148 + 44,
	.vdisplay = 1080,
	.vsync_start = 1080 + 4,
	.vsync_end = 1080 + 4 + 36,
	.vtotal = 1080 + 4 + 36 + 5,
	.vrefresh = 60,
};

static struct mipi2edp *connector_to_ti_sn_bridge(struct drm_connector *connector)
{
	return container_of(connector, struct mipi2edp, connector);
}

/* Connector helper functions */
static int mipi2edp_connector_get_modes(
	struct drm_connector *connector)
{
	struct drm_display_mode *mode = NULL;

	HISI_DRM_INFO("+");

	if (!connector) {
		HISI_DRM_ERR("connector is nullptr!");
		return -EINVAL;
	}
	struct mipi2edp *pdata = connector_to_ti_sn_bridge(connector);
	if (!pdata) {
		HISI_DRM_ERR("pdata is nullptr!");
		return -EINVAL;
	}

	if(pdata->output == OUTPUT_VGA) {
		mode = drm_mode_duplicate(connector->dev, &default_mode_vga);
		if (mode == NULL) {
			HISI_DRM_ERR("failed to add mode %ux%ux@%u!",
				default_mode_vga.hdisplay, default_mode_vga.vdisplay,
				default_mode_vga.vrefresh);
			return -ENOMEM;
		}
	} else {
		mode = drm_mode_duplicate(connector->dev, &default_mode_lcd);
		if (mode == NULL) {
			HISI_DRM_ERR("failed to add mode %ux%ux@%u!",
				default_mode_lcd.hdisplay, default_mode_lcd.vdisplay,
				default_mode_lcd.vrefresh);
			return -ENOMEM;
		}
	}

	drm_mode_set_name(mode);

	drm_mode_probed_add(connector, mode);

	HISI_DRM_INFO("-");

	return 1;
}

static enum drm_mode_status mipi2edp_connector_mode_valid(
	struct drm_connector *connector,
	struct drm_display_mode *mode)
{
	UNUSED(connector);
	UNUSED(mode);

	HISI_DRM_INFO("+");
	HISI_DRM_INFO("-");

	return MODE_OK;
}

static struct drm_connector_helper_funcs mipi2edp_connector_helper_funcs = {
	.get_modes = mipi2edp_connector_get_modes,
	.mode_valid = mipi2edp_connector_mode_valid,
};

static enum drm_connector_status mipi2edp_connector_detect(
	struct drm_connector *connector,
	bool force)
{
	HISI_DRM_INFO("+");
	enum drm_connector_status status;

	if (!connector) {
		HISI_DRM_ERR("connector is nullptr!");
		return -EINVAL;
	}
	struct mipi2edp *pdata = connector_to_ti_sn_bridge(connector);
	if (!pdata) {
		HISI_DRM_ERR("pdata is nullptr!");
		return -EINVAL;
	}

	if (pdata->output == OUTPUT_LCD) {
		status = connector_status_connected;
	} else {
		if (pdata->patch) {
			pdata->patch = 0;
			status = connector_status_connected;
		} else {
			status = gpio_get_value(pdata->hw_hpd_gpio) ? connector_status_connected : connector_status_disconnected;
		}
	}
	HISI_DRM_INFO("-");
	return status;
}

const static struct drm_connector_funcs mipi2edp_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.detect = mipi2edp_connector_detect,
	.destroy = drm_connector_cleanup,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static void set_blpwm_for_bl_chip(struct mipi2edp *pdata,int enable)
{
	struct hisi_panel_info *pinfo = &(pdata->panel_info);

	HISI_DRM_INFO("+");
	if (enable) {
		/* enable blpwm for backlight chip */
		pinfo->blpwm_input_ena = 0;
		pinfo->bl_min = DEFAULT_MAX_BRIGHTNESS/90;
		pinfo->bl_max = DEFAULT_MAX_BRIGHTNESS;
		pinfo->bl_ic_ctrl_mode = 10;
		/* set pwm frequecy to 4KHz */
		pinfo->blpwm_out_div_value=0x37;
		hisi_blpwm_on(pinfo);
		hisi_blpwm_set_backlight(pinfo,pdata->bl->props.brightness);
	} else {
		/* enable blpwm for backlight chip */
		hisi_blpwm_off(pinfo);
	}
	HISI_DRM_INFO("-");
}

static void set_refclk_for_bridge(struct mipi2edp *pdata, int enable)
{
	int ret;

	if (pdata->type == SN65DSIX6) {
		if (enable) {
			/* enable 38M4 clock for bridge from pmu */
			ret = clk_prepare_enable(pdata->mipi2edp_clk);
			if (ret) {
				HISI_DRM_INFO("mipi2edp_clk enable failed !\n");
				return;
			}
		} else {
			/* disable 38M4 clock for bridge from pmu */
			clk_disable_unprepare(pdata->mipi2edp_clk);
		}
	} else {
		HISI_DRM_INFO("do nothing\n");
	}
}

/**********************************************************
 *  Function:       set_power_for_bridge
 *  Discription:    enable its regulator moudule
 *  Parameters:     struct mipi2edp *pdata,int enable
 *  return value:   0-sucess or others-fail
 **********************************************************/
static int set_power_for_bridge(struct mipi2edp *pdata, int enable)
{
	int error=0;

	if (enable) {
		error = regulator_enable(pdata->ec_1v2_en);
		if (error) {
			dev_err(&pdata->client->dev,
				"failed to enable ec_1v2_en regulator: %d\n",
				error);
			return -EINVAL;
		}
		mdelay(2);
		error = regulator_enable(pdata->ec_dsi_vccio_on);
		if (error) {
			dev_err(&pdata->client->dev,
				"failed to enable ec_dsi_vccio_on regulator: %d\n",
				error);
			return -EINVAL;
		}
		mdelay(2);
	} else {
		error = regulator_disable(pdata->ec_dsi_vccio_on);
		if (error) {
			dev_err(&pdata->client->dev,
				"failed to disable ec_dsi_vccio_on regulator: %d\n",
				error);
			return -EINVAL;
		}
		mdelay(20);
		error = regulator_disable(pdata->ec_1v2_en);
		if (error) {
			dev_err(&pdata->client->dev,
				"failed to disable ec_1v2_en regulator: %d\n",
				error);
			return -EINVAL;
		}
		mdelay(20);
	}
	return 0;
}

/**********************************************************
 *  Function:       mipi2edp_bridge_pre_enable
 *  Discription:    pre_enable mopi2edp_bridge moudule
 *  Parameters:     struct drm_bridge *bridge
 *  return value:   none
 **********************************************************/
static void mipi2edp_bridge_pre_enable(struct drm_bridge *bridge)
{
	struct mipi2edp *pdata = to_mipi2edp(bridge);
	struct drm_encoder *encoder = bridge->encoder;

	HISI_DRM_INFO("+");

	if(pdata->output == OUTPUT_LCD)
		pm_runtime_get_sync(pdata->dev);

	set_refclk_for_bridge(pdata, 1);
	mdelay(15);
	gpio_cmds_tx(bridge_gpio_enable_cmds,
		ARRAY_SIZE(bridge_gpio_enable_cmds));

	if (pdata->chip.pre_enable)
		pdata->chip.pre_enable(pdata);

	encoder_to_dsi(encoder);

	HISI_DRM_INFO("-");
}



static void mipi2edp_bridge_enable(struct drm_bridge *bridge)
{
	struct mipi2edp *pdata = to_mipi2edp(bridge);
	int ret;

	HISI_DRM_INFO("+");

	if (pdata->chip.enable) {
		ret = pdata->chip.enable(pdata);
		if (ret)
			HISI_DRM_INFO("bridge chip enable failed !!!");
	}
	if(pdata->output == OUTPUT_LCD) {
		mdelay(2);
		gpio_cmds_tx(bridge_bl_config_enable_cmds,
			ARRAY_SIZE(bridge_bl_config_enable_cmds));
		mdelay(2);
		/* enable backlight */
		set_blpwm_for_bl_chip(pdata,1);
	}

	HISI_DRM_INFO("-");
}


static void mipi2edp_bridge_disable(struct drm_bridge *bridge)
{
	struct mipi2edp *pdata = to_mipi2edp(bridge);

	HISI_DRM_INFO("+");

	if(pdata->output == OUTPUT_LCD) {
		/* set backlight pwm 0 */
		hisi_blpwm_set_brightness(0);
		mdelay(2);
		gpio_cmds_tx(bridge_bl_config_disable_cmds,
			ARRAY_SIZE(bridge_bl_config_disable_cmds));
	}

	/* disable  stream */
	if (pdata->chip.disable)
		pdata->chip.disable(pdata);

	HISI_DRM_INFO("-");
}

static void mipi2edp_bridge_post_disable(
	struct drm_bridge *bridge)
{
	struct mipi2edp *pdata = to_mipi2edp(bridge);

	HISI_DRM_INFO("+");

	/* reset the bridge chip*/
	gpio_cmds_tx(bridge_gpio_disable_cmds,
		ARRAY_SIZE(bridge_gpio_disable_cmds));
	mdelay(2);
	set_refclk_for_bridge(pdata, 0);

	if(pdata->output == OUTPUT_LCD) {
		pm_runtime_put_sync(pdata->dev);
		set_blpwm_for_bl_chip(pdata,0);
		mdelay(500);
	}

	HISI_DRM_INFO("-");
}

static void mipi2edp_bridge_mode_set(
	struct drm_bridge *bridge,
	struct drm_display_mode *mode,
	struct drm_display_mode *adjusted_mode)
{
	UNUSED(bridge);
	UNUSED(mode);
	UNUSED(adjusted_mode);

	HISI_DRM_INFO("+");
	HISI_DRM_INFO("-");
}

static irqreturn_t hw_hpd_gpio_irq_thread(int irq, void *data)
{
	struct mipi2edp *pdata = (struct mipi2edp *)data;
	struct drm_device *drm_dev = pdata->connector.dev;

	if (irq == pdata->hw_hpd_irq) {
		drm_kms_helper_hotplug_event(drm_dev);
		HISI_DRM_INFO("handle irq %d!\n", irq);
	} else {
		HISI_DRM_ERR("invalid irq %d!\n", irq);
	}
	return IRQ_HANDLED;
}

static irqreturn_t hw_hpd_irq_handler(int irq, void *data)
{
	HISI_DRM_INFO("[mipi] hpd gpio irq\n");
	return IRQ_WAKE_THREAD;
}

static int mipi2edp_hpd_init(struct mipi2edp *pdata)
{
	int ret;
	if (gpio_is_valid(pdata->hw_hpd_gpio)) {
		ret = devm_gpio_request(pdata->dev, pdata->hw_hpd_gpio, "hw_hpd_gpio");
		if (ret < 0) {
			HISI_DRM_ERR("Fail request hw_hpd_gpio:%d", pdata->hw_hpd_gpio);
			return ret;
		}
		HISI_DRM_INFO("success request hw_hpd_gpio:%d", pdata->hw_hpd_gpio);

		ret = gpio_direction_input(pdata->hw_hpd_gpio);
		if (ret < 0) {
			HISI_DRM_ERR("Failed to set hw_hpd_gpio directoin!");
			return ret;
		}
		HISI_DRM_INFO("success to set hw_hpd_gpio directoin!");

		pdata->hw_hpd_irq = gpio_to_irq(pdata->hw_hpd_gpio);
		if (pdata->hw_hpd_irq < 0) {
			HISI_DRM_ERR("Failed to get hw_hpd_gpio irq!");
			return ret;
		}
		HISI_DRM_INFO("success to get hw_hpd_gpio irq!");

		ret = devm_request_threaded_irq(pdata->dev,
		pdata->hw_hpd_irq, hw_hpd_irq_handler, hw_hpd_gpio_irq_thread,
		IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
		"edp_bridge", pdata);
		if (ret) {
			HISI_DRM_ERR("Failed to request hw_hpd_gpio handler!");
			return ret;
		}
		HISI_DRM_INFO("success to request hw_hpd_gpio handler!");

	} else {
		HISI_DRM_ERR("gpio of hpd is not valid, check DTS!");
		return -EINVAL;
	}
	return 0;
}

static int mipi2edp_bridge_attach(struct drm_bridge *bridge)
{
	int ret;
	struct mipi2edp *pdata = NULL;
	struct drm_encoder *encoder = NULL;
	struct hisi_dsi *dsi = NULL;
	u32 allowed_scalers = 0;

	HISI_DRM_INFO("+");

	if (!bridge) {
		HISI_DRM_ERR("bridge is nullptr!");
		return -EINVAL;
	}

	pdata = to_mipi2edp(bridge);
	if (!pdata) {
		HISI_DRM_ERR("pdata is nullptr!");
		return -EINVAL;
	}

	encoder = bridge->encoder;
	if (!encoder) {
		HISI_DRM_ERR("encoder is nullptr!");
		return -EINVAL;
	}

	ret = drm_connector_init(bridge->dev, &pdata->connector,
		&mipi2edp_connector_funcs, DRM_MODE_CONNECTOR_eDP);
	if (ret) {
		HISI_DRM_ERR("Failed to initialize connector with drm, ret=%d!", ret);
		return ret;
	}

	drm_connector_helper_add(&pdata->connector,
		&mipi2edp_connector_helper_funcs);
	drm_mode_connector_attach_encoder(&pdata->connector,
		bridge->encoder);

	allowed_scalers |= BIT(DRM_MODE_SCALE_ASPECT);
	allowed_scalers |= BIT(DRM_MODE_SCALE_FULLSCREEN);
	drm_connector_attach_scaling_mode_property(&pdata->connector,
		allowed_scalers);

	dsi = encoder_to_dsi(encoder);
	dsi->client.lanes = 4;
	dsi->client.format = MIPI_DSI_FMT_RGB888;
	dsi->client.mode_flags = MIPI_DSI_MODE_VIDEO|MIPI_DSI_CLOCK_NON_CONTINUOUS;
	dsi->client.phy_clock = 0;
	dsi->panel_info = &(pdata->panel_info);

	if(pdata->output == OUTPUT_VGA) {
		ret = mipi2edp_hpd_init(pdata);
		if (ret) {
			HISI_DRM_ERR("Failed to initialize hpd gpio, ret=%d!", ret);
			return ret;
		}
	}
	HISI_DRM_INFO("-");

	return ret;
}

static const struct drm_bridge_funcs mipi2edp_bridge_funcs = {
	.attach = mipi2edp_bridge_attach,
	.pre_enable = mipi2edp_bridge_pre_enable,
	.enable = mipi2edp_bridge_enable,
	.disable = mipi2edp_bridge_disable,
	.post_disable = mipi2edp_bridge_post_disable,
	.mode_set = mipi2edp_bridge_mode_set,
};


static int  mipi2edp_bridge_resume(struct device *dev)
{
	int ret = 0;
	int status;

	struct mipi2edp *pdata = dev_get_drvdata(dev);

	HISI_DRM_INFO("mipi2edp bridge resume start");

	/* enable lcdvdd */
	gpio_cmds_tx(bridge_lcd_vdd_enable_cmds,
		ARRAY_SIZE(bridge_lcd_vdd_enable_cmds));

	status = set_power_for_bridge(pdata, 1);
	if (status)
		pr_err("set eDP_bridge power up failed !\n");

	HISI_DRM_INFO("mipi2edp bridge resume end");

	return ret;
}

static int  mipi2edp_bridge_suspend(struct device *dev)
{

	int ret = 0;
	int status;

	struct mipi2edp *pdata = dev_get_drvdata(dev);

	HISI_DRM_INFO("mipi2edp bridge suspend start");

	/* disable lcdvdd */
	gpio_cmds_tx(bridge_lcd_vdd_disable_cmds,
		ARRAY_SIZE(bridge_lcd_vdd_disable_cmds));

	status = set_power_for_bridge(pdata, 0);

	if (status)
		pr_err("set eDP_bridge power down failed !\n");

	HISI_DRM_INFO("mipi2edp bridge suspend end");

	return ret;
}


static const struct dev_pm_ops mipi2edp_bridge_pm_ops = {
	SET_RUNTIME_PM_OPS(mipi2edp_bridge_suspend, mipi2edp_bridge_resume, NULL)
};

static int mipi2edp_get_brightness(struct backlight_device *bl)
{
	struct mipi2edp *pdata = bl_get_data(bl);

	return pdata->bl->props.brightness;
}

static int mipi2edp_backlight_update(struct backlight_device *bl)
{
	struct mipi2edp *pdata = bl_get_data(bl);
	int brightness = bl->props.brightness;

	HISI_DRM_INFO("brightness :%d.\n", brightness);
	hisi_blpwm_set_backlight(&(pdata->panel_info), (uint32_t)brightness);

	pdata->bl->props.brightness = brightness;
	HISI_DRM_INFO("+");
	return 0;
}

static const struct backlight_ops mipi2edp_backlight_ops = {
	.get_brightness = mipi2edp_get_brightness,
	.update_status = mipi2edp_backlight_update,
};

static void mipi2edp_get_edid(struct mipi2edp *pdata)
{
	int ret;

	if(pdata->output == OUTPUT_VGA) {
		ret = lt9711a_get_edid(g_edid, EDID_LEN);
		if (ret == -1)
			HISI_DRM_ERR("lt9711a_get_edid fail");
		else
			HISI_DRM_INFO("lt9711a_get_edid ret = %d", ret);
	}

}

static void mipi2edp_panel_info_init(struct mipi2edp *pdata)
{
	struct hisi_panel_info *pinfo = &(pdata->panel_info);
	int ret;

	ret = memset_s(pinfo, sizeof(struct hisi_panel_info), 0, sizeof(struct hisi_panel_info));
	drm_check_and_void_return(ret != EOK, "memset failed!");

	pinfo->type = PANEL_MIPI_VIDEO;
	pinfo->mipi.non_continue_en = 0;
	pinfo->mipi.lane_nums = DSI_4_LANES;
	pinfo->mipi.color_mode = DSI_24BITS_1;
	pinfo->mipi.vc = 0;
	pinfo->mipi.max_tx_esc_clk = 10 * 1000000;
	pinfo->mipi.phy_mode = DPHY_MODE;
	pinfo->mipi.dsi_version = DSI_1_1_VERSION;
	pinfo->mipi.dsi_timing_support = 1;
	pinfo->ifbc_type = IFBC_TYPE_NONE;
	pinfo->pxl_clk_rate_div = 1;

	if (pdata->output == OUTPUT_LCD) {
		pinfo->mipi.dsi_bit_clk = 624;
		if (pdata->type == SN65DSIX6) {
			HISI_DRM_INFO("bridge:TI sn65dsi86 ");
			pinfo->mipi.dsi_bit_clk = 624;
		} else if (pdata->type == LT9711A) {
			HISI_DRM_INFO("bridge: lt9711A ");
			pinfo->mipi.dsi_bit_clk = 600;
		} else {
			HISI_DRM_INFO("bridge: not found");
		}
		pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;
		pinfo->mipi.burst_mode = DSI_BURST_SYNC_PULSES_1;
		pinfo->mipi.hsa = 24;
		pinfo->mipi.hbp = 60;
		pinfo->mipi.hline_time = 1742;
		pinfo->mipi.vsa = 10;
		pinfo->mipi.vbp = 27;
		pinfo->mipi.vfp = 3;
		pinfo->mipi.dpi_hsize = 1622;
		pinfo->mipi.vactive_line = 1440;
		pinfo->blpwm_input_ena = 0;
		/**
		 * Brightness min value can't cause screen completely black!
		 * so the default min value set to 20 percent of max!
		 */
		pinfo->bl_min = DEFAULT_MAX_BRIGHTNESS/90;
		pinfo->bl_max = DEFAULT_MAX_BRIGHTNESS;
		pinfo->bl_ic_ctrl_mode = 10;
		/* set pwm frequecy to 4KHz */
		pinfo->blpwm_out_div_value = 0x37;
		hisi_blpwm_on(pinfo);
		hisi_blpwm_set_backlight(pinfo, pdata->bl->props.brightness);
	} else {
		pinfo->mipi.dsi_bit_clk = 448.4;
		pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;
		pinfo->mipi.burst_mode = DSI_NON_BURST_SYNC_PULSES;

		pinfo->mipi.hsa = 33;
		pinfo->mipi.hbp = 111;
		pinfo->mipi.hline_time = 1652;
		pinfo->mipi.vsa = 5;
		pinfo->mipi.vbp = 36;
		pinfo->mipi.vfp = 4;
		pinfo->mipi.dpi_hsize = 1442;
		pinfo->mipi.vactive_line = 1080;
	}
}

static struct mipi2edp *aux_to_ti_sn_bridge(struct drm_dp_aux *aux)
{
	return container_of(aux, struct mipi2edp, aux);
}

static int write_regmap(struct drm_dp_aux_msg *msg, u32 request, struct mipi2edp *pdata, u8 *buf)
{
	u32 request_val = AUX_CMD_REQ(msg->request);
	int i;

	switch (request) {
	case DP_AUX_NATIVE_WRITE:
	case DP_AUX_I2C_WRITE:
	case DP_AUX_NATIVE_READ:
	case DP_AUX_I2C_READ:
		regmap_write(pdata->regmap, SN_AUX_CMD_REG, request_val);
		break;
	default:
		return -EINVAL;
	}

	regmap_write(pdata->regmap, SN_AUX_ADDR_19_16_REG,
		     (msg->address >> 16) & 0xF);
	regmap_write(pdata->regmap, SN_AUX_ADDR_15_8_REG,
		     (msg->address >> 8) & 0xFF);
	regmap_write(pdata->regmap, SN_AUX_ADDR_7_0_REG, msg->address & 0xFF);

	regmap_write(pdata->regmap, SN_AUX_LENGTH_REG, msg->size);

	if (request == DP_AUX_NATIVE_WRITE || request == DP_AUX_I2C_WRITE) {
		for (i = 0; i < msg->size; i++)
			regmap_write(pdata->regmap, SN_AUX_WDATA_REG(i),
				     buf[i]);
	}

	regmap_write(pdata->regmap, SN_AUX_CMD_REG, request_val | AUX_CMD_SEND);
	return 0;
}

static ssize_t ti_sn_aux_transfer(struct drm_dp_aux *aux,
				  struct drm_dp_aux_msg *msg)
{
	struct mipi2edp *pdata = aux_to_ti_sn_bridge(aux);
	u32 request = msg->request & ~DP_AUX_I2C_MOT;
	u8 *buf = (u8 *)msg->buffer;
	unsigned int val;
	int ret, i;

	if (msg->size > SN_AUX_MAX_PAYLOAD_BYTES)
		return -EINVAL;

	ret = write_regmap(msg, request, pdata, buf);
	if (ret)
		return ret;

	ret = regmap_read_poll_timeout(pdata->regmap, SN_AUX_CMD_REG, val,
				       !(val & AUX_CMD_SEND), 200,
				       50 * 1000);
	if (ret)
		return ret;

	ret = regmap_read(pdata->regmap, SN_AUX_CMD_STATUS_REG, &val);
	if (ret)
		return ret;
	else if ((val & AUX_IRQ_STATUS_NAT_I2C_FAIL)
		 || (val & AUX_IRQ_STATUS_AUX_RPLY_TOUT)
		 || (val & AUX_IRQ_STATUS_AUX_SHORT))
		return -ENXIO;

	if (request == DP_AUX_NATIVE_WRITE || request == DP_AUX_I2C_WRITE)
		return msg->size;

	for (i = 0; i < msg->size; i++) {
		unsigned int val;

		ret = regmap_read(pdata->regmap, SN_AUX_RDATA_REG(i),
				  &val);
		if (ret)
			return ret;

		WARN_ON(val & ~0xFF);
		buf[i] = (u8)(val & 0xFF);
	}

	return msg->size;
}

static int mipi2edp_probe_init(struct i2c_client *client, struct device **dev,
	struct mipi2edp **pdata)
{
	struct i2c_adapter *adapter = NULL;

	adapter = client->adapter;
	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C)) {
		HISI_DRM_ERR("failed to i2c_check_functionality!");
		return -EOPNOTSUPP;
	}
	*dev = &client->dev;
	*pdata = devm_kzalloc(*dev, sizeof(struct mipi2edp), GFP_KERNEL);
	if (!(*pdata)) {
		HISI_DRM_ERR("failed to alloc i2c!");
		return -ENOMEM;
	}

	return 0;
}


static int get_func_ptr_for_chip(struct i2c_client *client, struct mipi2edp *pdata)
{
	if (client->addr == SN65DSIX6_I2C_ADDR) {
		pdata->type = SN65DSIX6;
		pdata->regmap_config = &sn65dsix6_regmap;
		pdata->chip.is_chip_onboard = is_sn65dsix6_onboard;
		pdata->chip.pre_enable = sn65dsi86_pre_enable;
		pdata->chip.enable = sn65dsi86_enable;
		pdata->chip.disable = sn65dsi86_disable;
		pdata->chip.post_disable = NULL;
	} else if (client->addr == LT9711A_I2C_ADDR) {
		pdata->type = LT9711A;
		pdata->regmap_config = &lt9711a_regmap;
		pdata->chip.is_chip_onboard = is_lt9711a_onboard;
		pdata->chip.pre_enable = NULL;
		pdata->chip.enable = NULL;
		pdata->chip.disable = NULL;
		pdata->chip.post_disable = NULL;
	} else {
		HISI_DRM_ERR("i2c addr 0x%x is invalid!", client->addr);
		return -1;
	}
	return 0;
}

static int mipi2edp_regmap_init(struct i2c_client *client, struct mipi2edp *pdata)
{
	int ret = 0;

	pdata->regmap = devm_regmap_init_i2c(client, pdata->regmap_config);
	if (IS_ERR(pdata->regmap)) {
		HISI_DRM_ERR("failed to regmap iit i2c!");
		ret = -ENOMEM;
	}
	return ret;
}

static int check_mipi2edp_onboard(struct mipi2edp *pdata)
{
	/* assume the chip is enaable in UEFI.
	 * check whether the mipi2edp bridge chip is onboard
	 */
	int ret = 0;

	if (pdata->chip.is_chip_onboard) {
		ret = pdata->chip.is_chip_onboard(pdata);
		if (ret < 0)
			HISI_DRM_ERR("failed to find the bridge chip!");
	}
	return ret;
}

static int pdata_config(struct i2c_client *client, struct mipi2edp *pdata)
{
	int ret;

	ret = get_func_ptr_for_chip(client, pdata);
	if (ret)
		return ret;

	ret = mipi2edp_regmap_init(client, pdata);
	if (ret)
		return ret;

	ret = check_mipi2edp_onboard(pdata);
	if (ret)
		return ret;

	return 0;

}

static int bridge_pinctrl_init(struct i2c_client *client)
{
	int ret = 0;

	/* bridge pinctrl init */
	ret = pinctrl_cmds_tx(&client->dev, bridge_pinctrl_init_cmds,
		ARRAY_SIZE(bridge_pinctrl_init_cmds));
	if (ret != 0)
		HISI_DRM_ERR("Init bridge pinctrl failed, ret=%d!", ret);
	return ret;
}

static int gpio_set_config(struct mipi2edp *pdata, struct i2c_client *client)
{
	struct device_node *np = NULL;
	int ret = 0;

	np = client->dev.of_node;

	ret = of_property_read_u32(np, "output_mode", &pdata->output);
	if (ret < 0)
		HISI_DRM_WARN("get output_mode dts config failed!");

	if (pdata->output == OUTPUT_VGA) {
		pdata->patch = 1;
	}

	HISI_DRM_INFO("get output_mode = %d !", pdata->output);

	ret = of_property_read_u32(np, "hw_hpd_gpio", &pdata->hw_hpd_gpio);
	if (ret < 0)
		HISI_DRM_WARN("get hw_hpd_gpio config failed!");

	HISI_DRM_INFO("get hw_hpd_gpio = %d !", pdata->hw_hpd_gpio);

	ret = of_property_read_u32(np, "hw_enable_gpio",
		&pdata->gpio_set->hw_enable_gpio);
	if (ret < 0) {
		HISI_DRM_ERR("get hw_enable_gpio dts config failed!");
		return ret;
	}

	ret = of_property_read_u32(np, "hw_suspend_gpio",
		&pdata->gpio_set->hw_suspend_gpio);
	if (ret < 0) {
		HISI_DRM_ERR("get hw_suspend_gpio dts config failed!");
		return ret;
	}

	ret = of_property_read_u32(np, "lcd_vdd_enable_gpio",
		&pdata->gpio_set->lcd_vdd_enable_gpio);
	if (ret < 0) {
		HISI_DRM_ERR("get lcd_vdd_enable_gpio dts config failed!");
		return ret;
	}

	ret = of_property_read_u32(np, "bl_config_enable_gpio",
		&pdata->gpio_set->bl_config_enable_gpio);
	if (ret < 0) {
		HISI_DRM_ERR("get bl_config_enable_gpio dts config failed!");
		return ret;
	}
	return ret;
}


static int get_mipi2edp_clk(struct mipi2edp *pdata, struct device *dev)
{
	/* get mipi2edp clk resource */
	int ret = 0;

	pdata->mipi2edp_clk = devm_clk_get(dev, "clk_nfc");
	if (IS_ERR(pdata->mipi2edp_clk)) {
		HISI_DRM_ERR("mipi2edp_clk not found!");
		ret = -ENXIO;
	}
	return ret;
}


static void get_cmds_for_bridge_gpio(struct i2c_client *client)
{
	int ret;

	ret = pinctrl_cmds_tx(&client->dev, bridge_pinctrl_normal_cmds,
		ARRAY_SIZE(bridge_pinctrl_normal_cmds));
	if (ret)
		HISI_DRM_ERR("failed to pinctrl_cmds_tx, ret=%d!", ret);
	gpio_cmds_tx(bridge_gpio_set_init_cmds, ARRAY_SIZE(bridge_gpio_set_init_cmds));

	/* enable LCD_VDD */
	gpio_cmds_tx(bridge_lcd_vdd_enable_cmds,
	       ARRAY_SIZE(bridge_lcd_vdd_enable_cmds));
}


static int set_bl_props(struct mipi2edp *pdata, struct device *dev)
{
	int ret;

	pdata->bl = backlight_device_register("mipi2edp-backlight",
			dev, pdata, &mipi2edp_backlight_ops,
			NULL);
	if (IS_ERR(pdata->bl)) {
		HISI_DRM_ERR("failed to register backlight\n");
		ret = PTR_ERR(pdata->bl);
		pdata->bl = NULL;
		return -1;
	}
	pdata->bl->props.max_brightness = DEFAULT_MAX_BRIGHTNESS;
	pdata->bl->props.brightness = DEFAULT_MAX_BRIGHTNESS/2;

	return 0;
}

static void check_mipi2edp_type(struct mipi2edp *pdata)
{
	if (pdata->type == SN65DSIX6) {
		pdata->aux.name = "ti-sn65dsi86-aux";
		pdata->aux.dev = pdata->dev;
		pdata->aux.transfer = ti_sn_aux_transfer;
		drm_dp_aux_register(&pdata->aux);
	}
}


static int get_func_for_bridge(struct mipi2edp *pdata, struct device *dev)
{
	int ret;

	pdata->bridge.funcs = &mipi2edp_bridge_funcs;
	pdata->bridge.of_node = dev->of_node;
	ret = drm_bridge_add(&pdata->bridge);
	if (ret) {
		HISI_DRM_ERR("failed to add sn65dsix6 bridge!");
		return ret;
	}
	return 0;
}


static int mipi2edp_i2c_probe(
	struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct mipi2edp *pdata = NULL;
	struct device *dev = NULL;
	int ret = -1;
	int error = 0;

	HISI_DRM_INFO("+");

	ret = mipi2edp_probe_init(client, &dev, &pdata);
	if (ret)
		return ret;

	ret = pdata_config(client, pdata);
	if (ret)
		goto err_out;

	/* get ec_1v2_en regulator*/
	pdata->ec_1v2_en = devm_regulator_get_optional(&client->dev, "EC_1V2_EN");
	if (IS_ERR(pdata->ec_1v2_en)) {
		error = PTR_ERR(pdata->ec_1v2_en);
		if (error == -EPROBE_DEFER)
			ret = -EPROBE_DEFER;
		else
			ret = -EINVAL;
		HISI_DRM_ERR("Failed to get 'ec_1v2_en' regulator: %d\n",error);
		goto err_out;
	}

	/* get ec_dsi_vccio_on regulator*/
	pdata->ec_dsi_vccio_on = devm_regulator_get_optional(&client->dev, "EC_DSI_VCCIO_ON");
	if (IS_ERR(pdata->ec_dsi_vccio_on)) {
		error = PTR_ERR(pdata->ec_dsi_vccio_on);
		if (error == -EPROBE_DEFER)
			ret = -EPROBE_DEFER;
		else
			ret = -EINVAL;
		HISI_DRM_ERR("Failed to get 'vccio' regulator: %d\n",error);
		goto err_out;
	}

	dev_set_drvdata(dev, pdata);
	pdata->dev = dev;

	pdata->gpio_set = (struct gpio_config *)&gpio_set;
	ret = memset_s((void *)pdata->gpio_set, sizeof(struct gpio_config), 0, sizeof(struct gpio_config));
	if (ret) {
		HISI_DRM_ERR("memset for gpio_set failed!");
		return -1;
	}

	pdata->pinctrl = (struct pinctrl_data *)&pinctrl;
	ret = memset_s((void *)pdata->pinctrl, sizeof(struct pinctrl_data), 0, sizeof(struct pinctrl_data));
	if (ret) {
		HISI_DRM_ERR("memset for pinctrl failed!");
		return -1;
	}

	pdata->client = client;
	i2c_set_clientdata(client, pdata);

	ret = bridge_pinctrl_init(client);
	if (ret)
		goto err_out;

	ret = gpio_set_config(pdata, client);
	if (ret < 0)
		goto err_out;


	ret = get_mipi2edp_clk(pdata, dev);
	if (ret)
		goto err_out;

	HISI_DRM_INFO("mipi2edp_clk:[%lu]\n", clk_get_rate(pdata->mipi2edp_clk));

	get_cmds_for_bridge_gpio(client);

	ret = set_bl_props(pdata, dev);
	if (ret)
		goto err_out;

	check_mipi2edp_type(pdata);

	mipi2edp_panel_info_init(pdata);

	mipi2edp_get_edid(pdata);

	ret = get_func_for_bridge(pdata, dev);
	if (ret)
		goto err_out;

	pm_runtime_enable(pdata->dev);

	HISI_DRM_INFO("-");

	return ret;

err_out:
	devm_kfree(&client->dev, pdata);

	return ret;
}

static int mipi2edp_i2c_remove(struct i2c_client *client)
{
	HISI_DRM_INFO("+");

	if (!client) {
		HISI_DRM_ERR("client is nullptr!");
		return -EINVAL;
	}

	HISI_DRM_INFO("-");

	return 0;
}

static const struct i2c_device_id mipi2edp_i2c_ids[] = {
	{ "ti,sn65dsix6", 0 },
	{ "lt,lt9711a", 0 },
	{}
};

static const struct of_device_id mipi2edp_i2c_of_ids[] = {
	{.compatible = "ti,sn65dsix6"},
	{.compatible = "lt,lt9711a"},
	{}
};


static struct i2c_driver mipi2edp_i2c_driver = {
	.driver = {
		.name = "mipi2edp",
		.of_match_table = mipi2edp_i2c_of_ids,
		.pm = &mipi2edp_bridge_pm_ops,
	},
	.id_table = mipi2edp_i2c_ids,
	.probe = mipi2edp_i2c_probe,
	.remove = mipi2edp_i2c_remove,
};

static int __init mipi2edp_init(void)
{
	int ret = 0;

	HISI_DRM_INFO("+.");

	ret = i2c_add_driver(&mipi2edp_i2c_driver);
	if (ret) {
		HISI_DRM_ERR("failed to i2c_add_driver, ret=%d!", ret);
		return ret;
	}

	HISI_DRM_INFO("-.");

	return ret;
}
module_init(mipi2edp_init);

static void __exit mipi2edp_exit(void)
{
	HISI_DRM_INFO("+.");
	i2c_del_driver(&mipi2edp_i2c_driver);
	HISI_DRM_INFO("-.");
}
module_exit(mipi2edp_exit);

MODULE_ALIAS("huawei mipi2edp bridge module");
MODULE_DESCRIPTION("huawei mipi2edp bridge driver");
MODULE_LICENSE("GPL");

/*lint +e548 +e574 +e578*/
