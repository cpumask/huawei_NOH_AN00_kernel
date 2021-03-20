/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mipi switch between AP and MCU
 * Author: lijiawen
 * Create: 2020-04-20
 */
#include "lcdkit_mipi_switch.h"
#include "lcdkit_panel.h"
#include "lcdkit_dbg.h"
#include "hisi_mipi_dsi.h"
#include "securec.h"

#define TIMEOUT 60

struct display_switch {
	struct platform_device *pdev;
	int mipi_request_gpio;
	int mipi_switch_gpio;
	int mipi_switch_irq;
	struct work_struct sw;
};
struct display_switch *g_display_switch = NULL;
bool g_mipi_ap = false;

void request_mipi_gpio_operate(bool pull_up)
{
	if (pull_up) {
		gpio_direction_output(g_display_switch->mipi_request_gpio, 1);
		LCDKIT_DEBUG("mipi_request_gpio power_on\n");
	} else {
		gpio_direction_output(g_display_switch->mipi_request_gpio, 0);
		LCDKIT_DEBUG("mipi_request_gpio power_off\n");
	}
}

static int request_common_gpio(
	int *gpio, const char *compat, const char *name)
{
	struct device_node *np = NULL;
	np = of_find_compatible_node(NULL, NULL, compat);
	if (np == NULL) {
		LCDKIT_ERR("%s: node not found\n", __func__);
		return -ENODEV;
	}

	*gpio = of_get_named_gpio(np, name, 0);
	if (*gpio < 0) {
		LCDKIT_ERR("%s:%d.\n", name, *gpio);
			return -ENODEV;
	}

	if (gpio_request(*gpio, name) < 0) {
		LCDKIT_ERR("Failed to request gpio %d for %s\n", *gpio, name);
		return -ENODEV;
	}
	return 0;
}


static irqreturn_t display_switch_handler(int irq, void *arg)
{
	schedule_work(&g_display_switch->sw);
	return IRQ_HANDLED;
}

static void do_display_switch(struct work_struct *work)
{
	int req_display = gpio_get_value(g_display_switch->mipi_request_gpio);
	int irq_value = gpio_get_value(g_display_switch->mipi_switch_gpio);

	HISI_FB_INFO("mipi  recieve irq! req_display=%d, irq_value=%d\n", req_display, irq_value);
	if (!req_display) {
		LCDKIT_DEBUG("mipi off recieve irq!\n");
		g_mipi_ap = false;
	} else {
		LCDKIT_DEBUG("mipi on recieve irq!\n");
		g_mipi_ap = true;
	}
}


static int init_gpio(struct platform_device *pdev)
{
	int status;
	int retval;
	int irq_value;
	LCDKIT_ERR("+. \n");
	if (pdev == NULL) {
		HISI_FB_ERR("pdev is NULL");
		return -EINVAL;
	}

	status = request_common_gpio(&g_display_switch->mipi_switch_gpio,
		"mipi,switchselect", "mipi_switch");
	if (status != 0) {
		LCDKIT_ERR("init_gpio status = %d", status);
		return -EINVAL;
	}
	if (gpio_direction_input(g_display_switch->mipi_switch_gpio) < 0) {
		HISI_FB_ERR("Failed to set dir %d for mipi_switch_gpio\n",
			g_display_switch->mipi_switch_gpio);
		return -EINVAL;
	}
	irq_value = gpio_get_value(g_display_switch->mipi_switch_gpio);
	HISI_FB_INFO("init_gpio irq_value=%d\n", irq_value);
	g_display_switch->mipi_switch_irq = gpio_to_irq(g_display_switch->mipi_switch_gpio);
	LCDKIT_DEBUG("init_gpio mipi_switch_irq = %u",
		g_display_switch->mipi_switch_irq);
	retval = request_irq(g_display_switch->mipi_switch_irq, display_switch_handler,
		IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND,
		"mipi sw irq", g_display_switch->pdev);
	if (retval < 0) {
		LCDKIT_ERR("Couldn't acquire MCU HOST WAKE UP IRQ reval = %d\n", retval);
		return -EINVAL;
	}

	status = request_common_gpio(&g_display_switch->mipi_request_gpio,
		"mipi,requestdisplay", "mipi_request");
	INIT_WORK(&g_display_switch->sw, do_display_switch);
	LCDKIT_ERR("-. \n");
	return status;
}

void mipi_to_ap(struct hisi_fb_data_type *hisifd)
{
	int try_times = 0;

	HISI_FB_INFO("set request display gpio up!\n");
	request_mipi_gpio_operate(1);
	while (!g_mipi_ap) {
		mdelay(1);
		if (++try_times > TIMEOUT) {
			HISI_FB_ERR("wait for irq_mipi_on timeout!\n");
			break;
		}
	}
}

void mipi_to_mcu(struct hisi_fb_data_type *hisifd)
{
	int try_times = 0;

	HISI_FB_INFO("set request display gpio down!\n");
	request_mipi_gpio_operate(0);
	while (g_mipi_ap) {
		mdelay(1);
		if (++try_times > TIMEOUT) {
			HISI_FB_ERR("wait for irq_mipi_off timeout!\n");
			break;
		}
	}
}

void mipi_switch_release(struct platform_device* pdev)
{
	if (g_display_switch == NULL) {
		LCDKIT_ERR("g_display_switch is null pointer\n");
		return;
	}
	if (g_display_switch->mipi_request_gpio != 0)
		gpio_free(g_display_switch->mipi_request_gpio);
	if (g_display_switch->mipi_switch_gpio != 0)
		gpio_free(g_display_switch->mipi_switch_gpio);
	if (g_display_switch->mipi_switch_irq != 0)
		free_irq(g_display_switch->mipi_switch_irq, pdev);
	if (g_display_switch != NULL)
		kfree(g_display_switch);
	g_display_switch = NULL;
}

void mipi_switch_init(struct platform_device* pdev)
{
	g_display_switch = kzalloc(sizeof(struct display_switch), GFP_KERNEL);
	g_display_switch->pdev = pdev;
	memcpy_s(g_display_switch->pdev, sizeof(struct platform_device) + 1,
		pdev, sizeof(struct platform_device));
	init_gpio(pdev);
}
