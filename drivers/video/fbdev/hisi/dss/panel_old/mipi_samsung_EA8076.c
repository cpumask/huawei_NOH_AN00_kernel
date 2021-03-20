/*
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#include "hisi_fb.h"
#include "../panel/mipi_lcd_utils.h"
#ifdef CONFIG_LCD_KIT_DRIVER
#include "lcd_kit_core.h"
#define CONFIG_TP_ERR_DEBUG
#endif
#define DTS_COMP_SAMSUNG_EA8076 "hisilicon,mipi_samsung_EA8076"
static int g_lcd_fpga_flag;

/*lint -save -e569, -e574, -e527, -e572*/
/*******************************************************************************
** Power ON/OFF Sequence(sleep mode to Normal mode) begin
*/

static char display_off[] = {
	0x28,
};
static char enter_sleep[] = {
	0x10,
};

static char cmd0[] = {
	0x11,
	0x00,
};

static char cmd1[] = {
	0xF0,
	0x5A, 0x5A,
};

static char cmd2[] = {
	0x35,
	0x00,
};

static char cmd3[] = {
	0xF0,
	0xA5, 0xA5,
};

static char cmd4[] = {
	0x2B,
	0x00, 0x00, 0x09, 0x23,
};

static char cmd5[] = {
	0xF0,
	0x5A, 0x5A,
};

static char cmd6[] = {
	0xE1,
	0x00, 0x00, 0x02, 0x02,
	0x42, 0x02,
};

static char cmd7[] = {
	0xB0,
	0x0C,
};

static char cmd8[] = {
	0xE1,
	0x08,
};

static char cmd9[] = {
	0xF0,
	0xA5, 0xA5,
};

static char cmd10[] = {
	0x55,
	0x00,
};

static char cmd11[] = {
	0xF0,
	0x5A, 0x5A,
};

static char cmd12[] = {
	0x53,
	0x20,
};

static char cmd13[] = {
	0xF0,
	0xA5, 0xA5,
};

static char cmd14[] = {
	0x29,
	0x00,
};

static struct dsi_cmd_desc display_on_cmds[] = {
	{DTYPE_DCS_WRITE, 0,10, WAIT_TYPE_MS,
		sizeof(cmd0), cmd0},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd1), cmd1},
	{DTYPE_DCS_WRITE1, 0,10, WAIT_TYPE_US,
		sizeof(cmd2), cmd2},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd3), cmd3},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd4), cmd4},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd5), cmd5},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd6), cmd6},
	{DTYPE_DCS_WRITE1, 0,10, WAIT_TYPE_US,
		sizeof(cmd7), cmd7},
	{DTYPE_DCS_WRITE1, 0,10, WAIT_TYPE_US,
		sizeof(cmd8), cmd8},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd9), cmd9},
	{DTYPE_DCS_WRITE1, 0,10, WAIT_TYPE_US,
		sizeof(cmd10), cmd10},
	{DTYPE_DCS_LWRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd11), cmd11},
	{DTYPE_DCS_WRITE1, 0,10, WAIT_TYPE_US,
		sizeof(cmd12), cmd12},
	{DTYPE_DCS_LWRITE, 0,110, WAIT_TYPE_MS,
		sizeof(cmd13), cmd13},

	{DTYPE_DCS_WRITE, 0,10, WAIT_TYPE_US,
		sizeof(cmd14), cmd14},
};

static struct dsi_cmd_desc lcd_display_off_cmd[] = {
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS, sizeof(display_off), display_off},
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS, sizeof(enter_sleep), enter_sleep},
};

/*******************************************************************************
** LCD VCC
*/
#define VCC_LCDIO_NAME  "lcdio-vcc"
#define VCC_LCDDIG_NAME "lcddig-vcc"

static struct regulator *vcc_lcdio;
static struct regulator *vcc_lcddig;

static struct vcc_desc lcd_vcc_init_cmds[] = {
	/* vcc get */
	{DTYPE_VCC_GET, VCC_LCDIO_NAME, &vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0},

	/* io set voltage */
	{DTYPE_VCC_SET_VOLTAGE, VCC_LCDIO_NAME, &vcc_lcdio, 1800000, 1800000, WAIT_TYPE_MS, 0},
};

static struct vcc_desc lcd_vcc_finit_cmds[] = {
	/* vcc put */
	{DTYPE_VCC_PUT, VCC_LCDIO_NAME, &vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0},
};

static struct vcc_desc lcd_vcc_enable_cmds[] = {
	/* vcc enable */
	{DTYPE_VCC_ENABLE, VCC_LCDIO_NAME, &vcc_lcdio, 0, 0, WAIT_TYPE_MS, 10},
};

static struct vcc_desc lcd_vcc_disable_cmds[] = {
	/* vcc disable */
	{DTYPE_VCC_DISABLE, VCC_LCDIO_NAME, &vcc_lcdio, 0, 0, WAIT_TYPE_MS, 10},
};

static struct vcc_desc lcd_vcc_init_dig_cmds[] = {
	/* vci1v2 get */
	{DTYPE_VCC_GET, VCC_LCDDIG_NAME, &vcc_lcddig, 0, 0, WAIT_TYPE_MS, 0},

	/* vci1v2 set voltage */
	{DTYPE_VCC_SET_VOLTAGE, VCC_LCDDIG_NAME, &vcc_lcddig, 1200000, 1200000, WAIT_TYPE_MS, 0},
};

static struct vcc_desc lcd_vcc_finit_dig_cmds[] = {
	/* vci1v2 put */
	{DTYPE_VCC_PUT, VCC_LCDDIG_NAME, &vcc_lcddig, 0, 0, WAIT_TYPE_MS, 0},
};

static struct vcc_desc lcd_vcc_enable_dig_cmds[] = {
	/* vci1v2 enable */
	{DTYPE_VCC_ENABLE, VCC_LCDDIG_NAME, &vcc_lcddig, 0, 0, WAIT_TYPE_MS, 10},
};

static struct vcc_desc lcd_vcc_disable_dig_cmds[] = {
	/* vci1v2 disable */
	{DTYPE_VCC_DISABLE, VCC_LCDDIG_NAME, &vcc_lcddig, 0, 0, WAIT_TYPE_MS, 10},
};

/*******************************************************************************
** LCD IOMUX
*/
static struct pinctrl_data pctrl;

static struct pinctrl_cmd_desc lcd_pinctrl_init_cmds[] = {
	{DTYPE_PINCTRL_GET, &pctrl, 0},
	{DTYPE_PINCTRL_STATE_GET, &pctrl, DTYPE_PINCTRL_STATE_DEFAULT},
	{DTYPE_PINCTRL_STATE_GET, &pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc lcd_pinctrl_normal_cmds[] = {
	{DTYPE_PINCTRL_SET, &pctrl, DTYPE_PINCTRL_STATE_DEFAULT},
};

static struct pinctrl_cmd_desc lcd_pinctrl_lowpower_cmds[] = {
	{DTYPE_PINCTRL_SET, &pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc lcd_pinctrl_finit_cmds[] = {
	{DTYPE_PINCTRL_PUT, &pctrl, 0},
};

/*******************************************************************************
** LCD GPIO
*/
#define GPIO_AMOLED_RESET_NAME	"gpio_amoled_reset"
#define GPIO_AMOLED_TE0_NAME	"gpio_amoled_te0"
#define GPIO_AMOLED_VCI3V1_NAME	"gpio_amoled_vci3v1"

static uint32_t gpio_amoled_reset;
static uint32_t gpio_amoled_te0;
static uint32_t gpio_amoled_vci3v1;

static struct gpio_desc fpga_lcd_gpio_request_cmds[] = {
	/* vcc3v1 */
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},
	/* reset */
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

static struct gpio_desc fpga_lcd_gpio_normal_cmds[] = {
	/* vcc3v1 enable */
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 15,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 1},
	/* reset */
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 15,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 1},
	/* backlight enable */
};

static struct gpio_desc lcd_gpio_off_cmds[] = {
	/* vcc3v1 disable */
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 15,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},
	/* reset */
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

static struct gpio_desc lcd_gpio_free_cmds[] = {
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},

	/* reset */
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

static struct gpio_desc asic_lcd_gpio_enable_analog_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 1},
};

static struct gpio_desc asic_lcd_gpio_disable_analog_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},
};

static struct gpio_desc asic_lcd_gpio_request_cmds[] = {
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

static struct gpio_desc asic_lcd_gpio_normal_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 20,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 1},
};

static struct gpio_desc asic_lcd_gpio_lowpower_cmds[] = {
	/* reset */
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

static struct gpio_desc asic_lcd_gpio_free_cmds[] = {
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_VCI3V1_NAME, &gpio_amoled_vci3v1, 0},

	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &gpio_amoled_reset, 0},
};

/*******************************************************************************
*/
static int mipi_samsung_EA8076_panel_set_fastboot(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	BUG_ON(pdev == NULL);
	hisifd = platform_get_drvdata(pdev);
	BUG_ON(hisifd == NULL);

	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);

	if (g_lcd_fpga_flag == 0) {
		pinctrl_cmds_tx(pdev, lcd_pinctrl_normal_cmds,
			ARRAY_SIZE(lcd_pinctrl_normal_cmds));
		gpio_cmds_tx(asic_lcd_gpio_request_cmds,
			ARRAY_SIZE(asic_lcd_gpio_request_cmds));
	} else {
		gpio_cmds_tx(fpga_lcd_gpio_request_cmds,
			ARRAY_SIZE(fpga_lcd_gpio_request_cmds));
	}
	hisi_lcd_backlight_on(pdev);

	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);

	return 0;
}

static uint32_t mipi_read_ic_regs(char __iomem * mipi_dsi0_base, uint32_t rd_cmd)
{
	uint32_t status = 0;
	uint32_t try_times = 0;

	outp32(mipi_dsi0_base + MIPIDSI_GEN_HDR_OFFSET, rd_cmd);
	status = inp32(mipi_dsi0_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
	while (status & 0x10) {
		udelay(50);
		if (++try_times > 100) {
			HISI_FB_ERR("Read AMOLED reg:0x%x timeout!\n",rd_cmd>>8);
			break;
		}
		status = inp32(mipi_dsi0_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
	}
	status = inp32(mipi_dsi0_base + MIPIDSI_GEN_PLD_DATA_OFFSET);
	HISI_FB_INFO("Read AMOLED reg:0x%x = 0x%x.\n", rd_cmd>>8, status);

	return 0;
}

static int mipi_samsung_EA8076_panel_on(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_panel_info *pinfo = NULL;
	char __iomem *mipi_dsi0_base = NULL;
#ifdef CONFIG_TP_ERR_DEBUG
	struct ts_kit_ops *ts_ops = NULL;
#endif

	BUG_ON(pdev == NULL);
	hisifd = platform_get_drvdata(pdev);
	BUG_ON(hisifd == NULL);
	pinfo = &(hisifd->panel_info);

	HISI_FB_INFO("fb%d, +.\n", hisifd->index);

#ifdef CONFIG_TP_ERR_DEBUG
	ts_ops = ts_kit_get_ops();
#endif
	pinfo = &(hisifd->panel_info);
	mipi_dsi0_base = hisifd->mipi_dsi0_base;

	if (pinfo->lcd_init_step == LCD_INIT_POWER_ON) {
		if (g_lcd_fpga_flag == 0) {
			// lcd pinctrl normal
			pinctrl_cmds_tx(pdev, lcd_pinctrl_normal_cmds,
				ARRAY_SIZE(lcd_pinctrl_normal_cmds));

			// lcd gpio request
			gpio_cmds_tx(asic_lcd_gpio_request_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_request_cmds));

			// lcd vci1v8 enable
			vcc_cmds_tx(pdev, lcd_vcc_enable_cmds,
				ARRAY_SIZE(lcd_vcc_enable_cmds));

			// lcd vci3v1 enable
			gpio_cmds_tx(asic_lcd_gpio_enable_analog_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_enable_analog_cmds));

			// lcd vci1v2 enable
			vcc_cmds_tx(pdev, lcd_vcc_enable_dig_cmds,
				ARRAY_SIZE(lcd_vcc_enable_dig_cmds));

			// lcd reset gpio normal config
			gpio_cmds_tx(asic_lcd_gpio_normal_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_normal_cmds));
		} else {
			// lcd gpio request
			gpio_cmds_tx(fpga_lcd_gpio_request_cmds, \
				ARRAY_SIZE(fpga_lcd_gpio_request_cmds));

			// lcd gpio normal
			gpio_cmds_tx(fpga_lcd_gpio_normal_cmds, \
				ARRAY_SIZE(fpga_lcd_gpio_normal_cmds));
		}
		pinfo->lcd_init_step = LCD_INIT_MIPI_LP_SEND_SEQUENCE;
	} else if (pinfo->lcd_init_step == LCD_INIT_MIPI_LP_SEND_SEQUENCE) {
		mdelay(10);

		// lcd display on sequence
		mipi_dsi_cmds_tx(display_on_cmds, \
			ARRAY_SIZE(display_on_cmds), mipi_dsi0_base);
#ifdef CONFIG_TP_ERR_DEBUG
		if(ts_ops) {
			ts_ops->ts_power_notify(TS_RESUME_DEVICE, NO_SYNC);
		} else {
			HISI_FB_ERR("ts_ops is null,can't notify thp power_on\n");
		}
#endif
		mipi_read_ic_regs(mipi_dsi0_base, 0x0a06);

		pinfo->lcd_init_step = LCD_INIT_MIPI_HS_SEND_SEQUENCE;
	} else if (pinfo->lcd_init_step == LCD_INIT_MIPI_HS_SEND_SEQUENCE) {
#ifdef CONFIG_TP_ERR_DEBUG
		if (ts_ops)
			ts_ops->ts_power_notify(TS_AFTER_RESUME, NO_SYNC);
#endif
	} else {
		HISI_FB_ERR("failed to init lcd!\n");
	}
	hisi_lcd_backlight_on(pdev);
	HISI_FB_DEBUG("fb%d, -!\n", hisifd->index);

	return 0;
}

static int mipi_samsung_EA8076_panel_off(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;
#ifdef CONFIG_TP_ERR_DEBUG
	struct ts_kit_ops *ts_ops = NULL;
#endif
	struct hisi_panel_info *pinfo = NULL;
	char __iomem *mipi_dsi0_base = NULL;

	BUG_ON(pdev == NULL);
	hisifd = platform_get_drvdata(pdev);
	BUG_ON(hisifd == NULL);
	pinfo = &(hisifd->panel_info);
	mipi_dsi0_base = hisifd->mipi_dsi0_base;
	HISI_FB_INFO("fb%d, +.\n", hisifd->index);

	if (pinfo->lcd_uninit_step == LCD_UNINIT_MIPI_HS_SEND_SEQUENCE) {
	#ifdef CONFIG_TP_ERR_DEBUG
		ts_ops = ts_kit_get_ops();
		if (ts_ops) {
			ts_ops->ts_power_notify(TS_EARLY_SUSPEND, NO_SYNC);
		} else {
			HISI_FB_ERR("ts_ops is null,can't notify thp power_off\n");
		}
	#endif
		hisi_lcd_backlight_off(pdev);

		mipi_dsi_cmds_tx(lcd_display_off_cmd, \
				ARRAY_SIZE(lcd_display_off_cmd), mipi_dsi0_base);

		pinfo->lcd_uninit_step = LCD_UNINIT_MIPI_LP_SEND_SEQUENCE;
	#ifdef CONFIG_TP_ERR_DEBUG
		if(ts_ops) {
			ts_ops->ts_power_notify(TS_BEFORE_SUSPEND, NO_SYNC);
			ts_ops->ts_power_notify(TS_SUSPEND_DEVICE, NO_SYNC);
		}
	#endif
	} else if (pinfo->lcd_uninit_step == LCD_UNINIT_MIPI_LP_SEND_SEQUENCE) {
		pinfo->lcd_uninit_step = LCD_UNINIT_POWER_OFF;

	} else if (pinfo->lcd_uninit_step == LCD_UNINIT_POWER_OFF) {
		if (g_lcd_fpga_flag == 0) {
			// lcd gpio lowpower
			gpio_cmds_tx(asic_lcd_gpio_lowpower_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_lowpower_cmds));
			// lcd vci1v2 disable
			vcc_cmds_tx(pdev, lcd_vcc_disable_dig_cmds,
				ARRAY_SIZE(lcd_vcc_disable_dig_cmds));
			// lcd vci3v1 disable
			gpio_cmds_tx(asic_lcd_gpio_disable_analog_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_disable_analog_cmds));
			// lcd vci1v8 disable
			vcc_cmds_tx(pdev, lcd_vcc_disable_cmds,
				ARRAY_SIZE(lcd_vcc_disable_cmds));
			// lcd gpio free
			gpio_cmds_tx(asic_lcd_gpio_free_cmds, \
				ARRAY_SIZE(asic_lcd_gpio_free_cmds));
			// lcd pinctrl lowpower
			pinctrl_cmds_tx(pdev,lcd_pinctrl_lowpower_cmds,
				ARRAY_SIZE(lcd_pinctrl_lowpower_cmds));
		} else {
			// lcd gpio lowpower
			gpio_cmds_tx(lcd_gpio_off_cmds, \
				ARRAY_SIZE(lcd_gpio_off_cmds));
			// lcd gpio free
			gpio_cmds_tx(lcd_gpio_free_cmds, \
				ARRAY_SIZE(lcd_gpio_free_cmds));
		}
	} else {
		HISI_FB_ERR("failed to uninit lcd!\n");
	}

	HISI_FB_INFO("fb%d, -.\n", hisifd->index);

	return 0;
}

static int mipi_samsung_EA8076_panel_remove(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	BUG_ON(pdev == NULL);
	hisifd = platform_get_drvdata(pdev);
	if (!hisifd) {
		return 0;
	}
	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);

	if (g_lcd_fpga_flag == 0) {
		// lcd vcc finit
		vcc_cmds_tx(pdev, lcd_vcc_finit_cmds,
			ARRAY_SIZE(lcd_vcc_finit_cmds));

		// lcd vcc dig finit
		vcc_cmds_tx(pdev, lcd_vcc_finit_dig_cmds,
			ARRAY_SIZE(lcd_vcc_finit_dig_cmds));

		// lcd pinctrl finit
		pinctrl_cmds_tx(pdev, lcd_pinctrl_finit_cmds,
			ARRAY_SIZE(lcd_pinctrl_finit_cmds));
	}

	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);
	return 0;
}

static int mipi_samsung_EA8076_panel_set_backlight(struct platform_device *pdev, uint32_t bl_level)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret = 0;
	static uint32_t last_bl_level = 0;

	char bl_level_adjust[3] = {
		0x51,
		0x00,
		0x00,
	};
	struct dsi_cmd_desc lcd_bl_level_adjust[] = {
		{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US,
			sizeof(bl_level_adjust), bl_level_adjust},
	};

	if (pdev == NULL) {
		return -1;
	}
	hisifd = platform_get_drvdata(pdev);
	if (hisifd == NULL) {
		return -1;
	}

	if (bl_level > hisifd->panel_info.bl_max)
		bl_level = hisifd->panel_info.bl_max;

	if (hisifd->panel_info.bl_set_type & BL_SET_BY_PWM) {
		ret = hisi_pwm_set_backlight(hisifd, bl_level);
	} else if (hisifd->panel_info.bl_set_type & BL_SET_BY_BLPWM) {
		ret = hisi_blpwm_set_backlight(hisifd, bl_level);
	} else if (hisifd->panel_info.bl_set_type & BL_SET_BY_SH_BLPWM) {
		ret = hisi_sh_blpwm_set_backlight(hisifd, bl_level);
	} else if (hisifd->panel_info.bl_set_type & BL_SET_BY_MIPI) {
		bl_level_adjust[1] = (bl_level >> 8) & 0xff;
		bl_level_adjust[2] = bl_level & 0x00ff;
		HISI_FB_DEBUG("bl_level is %d, bl_level_adjust[1]=0x%x, bl_level_adjust[2] = 0x%x\n",
			bl_level,bl_level_adjust[1],bl_level_adjust[2]);

		if (last_bl_level != bl_level) {
			last_bl_level = bl_level;
			mipi_dsi_cmds_tx(lcd_bl_level_adjust, \
				ARRAY_SIZE(lcd_bl_level_adjust), hisifd->mipi_dsi0_base);
		}
	} else {
		HISI_FB_ERR("fb%d, not support this bl_set_type=%d!\n",
			hisifd->index, hisifd->panel_info.bl_set_type);
	}

	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);
	return ret;
}

static ssize_t mipi_samsung_EA8076_lcd_model_show(struct platform_device *pdev,
	char *buf)
{
	struct hisi_fb_data_type *hisifd = NULL;
	ssize_t ret = 0;

	if (NULL == pdev) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	hisifd = platform_get_drvdata(pdev);
	if (NULL == hisifd) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}

	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);

	ret = snprintf(buf, PAGE_SIZE, "samsung_EA8076 CMD AMOLED PANEL\n");

	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);

	return ret;
}

static ssize_t mipi_samsung_EA8076_panel_lcd_check_reg_show(struct platform_device *pdev, char *buf)
{
	ssize_t ret = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	char __iomem *mipi_dsi0_base = NULL;
	uint32_t read_value[4] = {0};
	uint32_t expected_value[4] = {0x9c, 0x00, 0x00, 0x80};
	uint32_t read_mask[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	char* reg_name[4] = {"power mode", "MADCTR", "image mode", "dsi error"};
	char lcd_reg_0a[] = {0x0a};
	char lcd_reg_0b[] = {0x0b};
	char lcd_reg_0d[] = {0x0d};
	char lcd_reg_0e[] = {0x0e};

	struct dsi_cmd_desc lcd_check_reg[] = {
		{DTYPE_DCS_READ, 0, 10, WAIT_TYPE_US,
			sizeof(lcd_reg_0a), lcd_reg_0a},
		{DTYPE_DCS_READ, 0, 10, WAIT_TYPE_US,
			sizeof(lcd_reg_0b), lcd_reg_0b},
		{DTYPE_DCS_READ, 0, 10, WAIT_TYPE_US,
			sizeof(lcd_reg_0d), lcd_reg_0d},
		{DTYPE_DCS_READ, 0, 10, WAIT_TYPE_US,
			sizeof(lcd_reg_0e), lcd_reg_0e},
	};

	struct mipi_dsi_read_compare_data data = {
		.read_value = read_value,
		.expected_value = expected_value,
		.read_mask = read_mask,
		.reg_name = reg_name,
		.log_on = 1,
		.cmds = lcd_check_reg,
		.cnt = ARRAY_SIZE(lcd_check_reg),
	};

	if (NULL == pdev) {
		HISI_FB_ERR("pdev is NULL");
		return -EINVAL;
	}
	hisifd = platform_get_drvdata(pdev);
	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}

	mipi_dsi0_base = hisifd->mipi_dsi0_base;

	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);
	if (!mipi_dsi_read_compare(&data, mipi_dsi0_base)) {
		ret = snprintf(buf, PAGE_SIZE, "OK\n");
	} else {
		ret = snprintf(buf, PAGE_SIZE, "ERROR\n");
	}
	HISI_FB_DEBUG("fb%d, -.\n", hisifd->index);

	return ret;
}

static char lcd_disp_x[] = {
	0x2A,
	0x00, 0x00, 0x04, 0x37
};

static char lcd_disp_y[] = {
	0x2B,
	0x00, 0x00, 0x08, 0xBF
};

static struct dsi_cmd_desc set_display_address[] = {
	{DTYPE_DCS_LWRITE, 0, 5, WAIT_TYPE_US,
		sizeof(lcd_disp_x), lcd_disp_x},
	{DTYPE_DCS_LWRITE, 0, 5, WAIT_TYPE_US,
		sizeof(lcd_disp_y), lcd_disp_y},
};

static int mipi_panel_set_display_region(struct platform_device *pdev, struct dss_rect *dirty)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;

	if (pdev == NULL || dirty == NULL)
		return -1;
	hisifd = platform_get_drvdata(pdev);
	if (hisifd == NULL)
		return -1;

	x = (uint32_t)dirty->x;
	y = (uint32_t)dirty->y;
	w = (uint32_t)dirty->w;
	h = (uint32_t)dirty->h;

	lcd_disp_x[1] = (x >> 8) & 0xff;
	lcd_disp_x[2] = x & 0xff;
	lcd_disp_x[3] = ((x + w - 1) >> 8) & 0xff;
	lcd_disp_x[4] = (x + w - 1) & 0xff;
	lcd_disp_y[1] = (y >> 8) & 0xff;
	lcd_disp_y[2] = y & 0xff;
	lcd_disp_y[3] = ((y + h - 1) >> 8) & 0xff;
	lcd_disp_y[4] = (y + h - 1) & 0xff;

	HISI_FB_DEBUG("x[1] = 0x%2x, x[2] = 0x%2x, x[3] = 0x%2x, x[4] = 0x%2x\n",
			lcd_disp_x[1], lcd_disp_x[2], lcd_disp_x[3], lcd_disp_x[4]);
	HISI_FB_DEBUG("y[1] = 0x%2x, y[2] = 0x%2x, y[3] = 0x%2x, y[4] = 0x%2x\n",
			lcd_disp_y[1], lcd_disp_y[2], lcd_disp_y[3], lcd_disp_y[4]);


	mipi_dsi_cmds_tx(set_display_address, \
			ARRAY_SIZE(set_display_address), hisifd->mipi_dsi0_base);

	return 0;
}

/*******************************************************************************
**
*/
static struct hisi_panel_info g_panel_info = {0};
static struct hisi_fb_panel_data g_panel_data = {
	.panel_info = &g_panel_info,
	.set_fastboot = mipi_samsung_EA8076_panel_set_fastboot,
	.on = mipi_samsung_EA8076_panel_on,
	.off = mipi_samsung_EA8076_panel_off,
	.remove = mipi_samsung_EA8076_panel_remove,
	.set_backlight = mipi_samsung_EA8076_panel_set_backlight,
	.lcd_model_show = mipi_samsung_EA8076_lcd_model_show,
	.lcd_check_reg = mipi_samsung_EA8076_panel_lcd_check_reg_show,
	.set_display_region = mipi_panel_set_display_region,
};

/*******************************************************************************
**
*/
static int lcd_voltage_relevant_init(struct platform_device *pdev)
{
	int ret = 0;

	if (g_lcd_fpga_flag == 0) {
		ret = vcc_cmds_tx(pdev, lcd_vcc_init_cmds,
			ARRAY_SIZE(lcd_vcc_init_cmds));
		if (ret != 0) {
			HISI_FB_ERR("LCD vcc init failed!\n");
			return ret;
		}

		ret = vcc_cmds_tx(pdev, lcd_vcc_init_dig_cmds,
			ARRAY_SIZE(lcd_vcc_init_dig_cmds));
		if (ret != 0) {
			HISI_FB_ERR("LCD vcc dig init failed!\n");
			return ret;
		}

		ret = pinctrl_cmds_tx(pdev, lcd_pinctrl_init_cmds,
			ARRAY_SIZE(lcd_pinctrl_init_cmds));
		if (ret != 0) {
			HISI_FB_ERR("Init pinctrl failed, defer\n");
			return ret;
		}

		if (is_fastboot_display_enable()) {
			vcc_cmds_tx(pdev, lcd_vcc_enable_cmds,
				ARRAY_SIZE(lcd_vcc_enable_cmds));
			vcc_cmds_tx(pdev, lcd_vcc_enable_dig_cmds,
				ARRAY_SIZE(lcd_vcc_enable_dig_cmds));
		}
	}
	return ret;
}

static int mipi_samsung_EA8076_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct hisi_panel_info *pinfo = NULL;
	struct device_node *np = NULL;
	uint32_t bl_type = 0;
	uint32_t lcd_display_type = 0;
	uint32_t lcd_ifbc_type = 0;

	HISI_FB_INFO("+\n");
	np = of_find_compatible_node(NULL, NULL, DTS_COMP_SAMSUNG_EA8076);
	if (!np) {
		HISI_FB_ERR("not found device node %s!\n", DTS_COMP_SAMSUNG_EA8076);
		goto err_return;
	}

	ret = of_property_read_u32(np, LCD_DISPLAY_TYPE_NAME, &lcd_display_type);
	if (ret) {
		HISI_FB_ERR("get lcd_display_type failed!\n");
		lcd_display_type = PANEL_MIPI_CMD;
	}

	ret = of_property_read_u32(np, LCD_IFBC_TYPE_NAME, &lcd_ifbc_type);
	if (ret) {
		HISI_FB_ERR("get ifbc_type failed!\n");
		lcd_ifbc_type = IFBC_TYPE_NONE;
	}

	ret = of_property_read_u32(np, LCD_BL_TYPE_NAME, &bl_type);
	if (ret) {
		HISI_FB_ERR("get lcd_bl_type failed!\n");
		bl_type = BL_SET_BY_MIPI;
	}
	HISI_FB_INFO("bl_type=0x%x.\n", bl_type);

	if (hisi_fb_device_probe_defer(lcd_display_type, bl_type))
		goto err_probe_defer;

	ret = of_property_read_u32(np, FPGA_FLAG_NAME, &g_lcd_fpga_flag); // lint !e64
	if (ret) {
		HISI_FB_WARNING("need to get g_lcd_fpga_flag resource in fpga, not needed in asic!\n");
	}
	HISI_FB_INFO("g_lcd_fpga_flag=%d.\n", g_lcd_fpga_flag);

	if (g_lcd_fpga_flag == 1) {
		// 143 3 174
		gpio_amoled_reset = of_get_named_gpio(np, "gpios", 0);
		gpio_amoled_te0 = of_get_named_gpio(np, "gpios", 1);
		gpio_amoled_vci3v1 = of_get_named_gpio(np, "gpios", 2);
	} else {
		// 60, 3, 190
		gpio_amoled_reset = of_get_named_gpio(np, "gpios", 0);
		gpio_amoled_te0 = of_get_named_gpio(np, "gpios", 1);
		gpio_amoled_vci3v1 = of_get_named_gpio(np, "gpios", 2);
	}
	HISI_FB_INFO("gpio_amoled_reset=%d, gpio_amoled_te0=%d, gpio_amoled_vci3v1=%d\n",
		gpio_amoled_reset, gpio_amoled_te0, gpio_amoled_vci3v1);

	pdev->id = 1;
	pinfo = g_panel_data.panel_info;
	memset(pinfo, 0, sizeof(struct hisi_panel_info));
	pinfo->xres = 1080;
	pinfo->yres = 2340;
	pinfo->width = 65;
	pinfo->height = 141;
	pinfo->orientation = LCD_PORTRAIT;
	pinfo->bpp = LCD_RGB888;
	pinfo->bgr_fmt = LCD_RGB;
	pinfo->bl_set_type = bl_type;
	pinfo->bl_min = 12;
	pinfo->bl_max = 1023;
	pinfo->bl_default = 1000;

	pinfo->frc_enable = 0;
	pinfo->esd_enable = 0;
	pinfo->esd_skip_mipi_check = 0;
	pinfo->lcd_uninit_step_support = 1;

	pinfo->current_mode = MODE_8BIT;
	pinfo->type = lcd_display_type; // PANEL_MIPI_CMD;

	if (g_lcd_fpga_flag == 1) {
		pinfo->ldi.h_back_porch = 23;
		pinfo->ldi.h_front_porch = 50;
		pinfo->ldi.h_pulse_width = 20;
		pinfo->ldi.v_back_porch = 12;
		pinfo->ldi.v_front_porch = 14;
		pinfo->ldi.v_pulse_width = 4;

		pinfo->mipi.dsi_bit_clk = 120;
		pinfo->mipi.dsi_bit_clk_val1 = 110;
		pinfo->mipi.dsi_bit_clk_val2 = 130;
		pinfo->mipi.dsi_bit_clk_val3 = 140;
		pinfo->mipi.dsi_bit_clk_val4 = 150;
		pinfo->dsi_bit_clk_upt_support = 0;
		pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;
		pinfo->mipi.non_continue_en = 0;

		pinfo->pxl_clk_rate = 20 * 1000000UL; // 40 * 1000000;
	} else {
		if (is_mipi_cmd_panel_ext(pinfo)) {
			pinfo->ldi.h_back_porch = 4; // 136;
			pinfo->ldi.h_front_porch = 48; // 188;
			pinfo->ldi.h_pulse_width = 12;
			pinfo->ldi.v_back_porch = 57; // 16;
			pinfo->ldi.v_front_porch = 4; // 18;
			pinfo->ldi.v_pulse_width = 10;

			pinfo->pxl_clk_rate = 175 * 1000000UL;
		} else {
			pinfo->ldi.h_back_porch = 96; // 136;
			pinfo->ldi.h_front_porch = 108; // 188;
			pinfo->ldi.h_pulse_width = 48;
			pinfo->ldi.v_back_porch = 12; // 16;
			pinfo->ldi.v_front_porch = 14; // 18;
			pinfo->ldi.v_pulse_width = 4;

			pinfo->pxl_clk_rate = 288 * 1000000;
		}
		pinfo->mipi.dsi_bit_clk = 500;
		pinfo->mipi.dsi_bit_clk_val1 = 471;
		pinfo->mipi.dsi_bit_clk_val2 = 480;
		pinfo->mipi.dsi_bit_clk_val3 = 490;
		pinfo->mipi.dsi_bit_clk_val4 = 500;
		pinfo->dsi_bit_clk_upt_support = 1;
		pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;
		pinfo->mipi.non_continue_en = 1;
	}
	pinfo->mipi.lane_nums = DSI_4_LANES;
	pinfo->mipi.clk_post_adjust = 16;
	pinfo->mipi.color_mode = DSI_24BITS_1;
	pinfo->mipi.vc = 0;
	pinfo->mipi.max_tx_esc_clk = 10 * 1000000;
	pinfo->mipi.burst_mode = DSI_BURST_SYNC_PULSES_1;
	pinfo->mipi.phy_mode = DPHY_MODE;
	pinfo->mipi.dsi_version = DSI_1_1_VERSION;

	pinfo->ifbc_type = IFBC_TYPE_NONE;
	pinfo->pxl_clk_rate_div = 1;

	if (pinfo->type == PANEL_MIPI_CMD) {
		pinfo->vsync_ctrl_type = VSYNC_CTRL_ISR_OFF | VSYNC_CTRL_MIPI_ULPS | VSYNC_CTRL_CLK_OFF;
		pinfo->dirty_region_updt_support = 1;
		/* parameters below are set according to ddic constraints */
		pinfo->dirty_region_info.left_align = -1;
		pinfo->dirty_region_info.right_align = -1;
		pinfo->dirty_region_info.top_align = 8;
		pinfo->dirty_region_info.bottom_align = -1;
		pinfo->dirty_region_info.w_align = -1;
		pinfo->dirty_region_info.h_align = -1;
		pinfo->dirty_region_info.w_min = 1080;
		pinfo->dirty_region_info.h_min = 8;
		pinfo->dirty_region_info.top_start = -1;
		pinfo->dirty_region_info.bottom_start = -1;
		pinfo->dirty_region_info.alsc.alsc_en = 1;
		pinfo->dirty_region_info.alsc.alsc_addr = 0x3E81F4;
		pinfo->dirty_region_info.alsc.alsc_size = 0x5959;
		/* xres and yres must no less than 1, and it must be satisfied */
		pinfo->dirty_region_info.alsc.pic_size =
			((pinfo->xres - 1) << ALSC_PIC_WIDTH_SHIFT) | (pinfo->yres - 1);
	}

	ret = lcd_voltage_relevant_init(pdev);
	if (ret != 0) {
		goto err_return;
	}

	ret = platform_device_add_data(pdev, &g_panel_data,
		sizeof(struct hisi_fb_panel_data));
	if (ret) {
		HISI_FB_ERR("platform_device_add_data failed!\n");
		goto err_device_put;
	}

	hisi_fb_add_device(pdev);

	HISI_FB_DEBUG("-.\n");

	return 0;

err_device_put:
	platform_device_put(pdev);
err_return:
	return ret;
err_probe_defer:
	return -EPROBE_DEFER;
}

static const struct of_device_id hisi_panel_match_table[] = {
	{
		.compatible = DTS_COMP_SAMSUNG_EA8076,
		.data = NULL,
	},
	{},
};
MODULE_DEVICE_TABLE(of, hisi_panel_match_table);

static struct platform_driver this_driver = {
	.probe = mipi_samsung_EA8076_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = "mipi_samsung_EA8076",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisi_panel_match_table),
	},
};

static int __init mipi_panel_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&this_driver);
	if (ret) {
		HISI_FB_ERR("platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

/*lint -restore*/
module_init(mipi_panel_init);
