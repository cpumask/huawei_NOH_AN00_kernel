/* Copyright (c) 2019-2019, Hisilicon Tech. Co., Ltd. All rights reserved.
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
#include "hisi_fb_rgb_stats.h"

static void hisifb_rgb_wq_handler(struct work_struct *work);
static DECLARE_DELAYED_WORK(rgb_work, hisifb_rgb_wq_handler);
static DEFINE_MUTEX(rgb_lock);

static atomic_t rgb_read_status;
bool g_rgb_enable = false;
bool g_rgb_init = false;
struct hisifb_rgb_data g_rgb_data;
struct hisifb_rgb_data g_rgb_data_last;
struct hisifb_rgb_data g_rgb_data_sum;

static void hisifb_rgb_sum_data()
{
	uint32_t i;

	mutex_lock(&rgb_lock);
	for (i = 0; i < DISP_GLB_REG_LENGTH; i++) {
		g_rgb_data_sum.red[i] += g_rgb_data_last.red[i];
		g_rgb_data_sum.green[i] += g_rgb_data_last.green[i];
		g_rgb_data_sum.blue[i] += g_rgb_data_last.blue[i];
	}
	mutex_unlock(&rgb_lock);
}

static bool hisifb_rgb_check_enable()
{
	return (g_debug_rgb_stats_enable == 1) && g_rgb_enable;
}

void hisifb_rgb_read_register(const struct hisi_fb_data_type *hisifd)
{
	uint32_t i;
	const uint32_t reg_length = 4;

	if (hisifd == NULL || hisifd->dss_base == NULL) {
		HISI_FB_ERR("hisifd or dss base is NULL");
		return;
	}

	if (!hisifb_rgb_check_enable() || (!atomic_read(&rgb_read_status)))
		return;

	if ((inp32(hisifd->dss_base + DSS_MIPI_DSI0_OFFSET + MIPI_LDI_VRT_CTRL2) & 0x3fff) ==
		(hisifd->panel_info.yres - 1)) {
		for (i = 0; i < DISP_GLB_REG_LENGTH; i++) {
			g_rgb_data.red[i] = inp32(hisifd->dss_base + DSS_DISP_GLB_OFFSET + R0_HIST + i * reg_length);
			g_rgb_data.green[i] = inp32(hisifd->dss_base + DSS_DISP_GLB_OFFSET + G0_HIST + i * reg_length);
			g_rgb_data.blue[i] = inp32(hisifd->dss_base + DSS_DISP_GLB_OFFSET + B0_HIST + i * reg_length);
		}
	}

	atomic_set(&rgb_read_status, 0);
}

static void hisifb_rgb_wq_handler(struct work_struct *work)
{
	if (work == NULL) {
		HISI_FB_ERR("rgb work is NULL");
		return;
	}

	if (!hisifb_rgb_check_enable()) {
		HISI_FB_ERR("rgb stats is disable");
		return;
	}

	if (!atomic_read(&rgb_read_status)) {
		memcpy(&g_rgb_data_last, &g_rgb_data, sizeof(struct hisifb_rgb_data));
		atomic_set(&rgb_read_status, 1);
	}

	hisifb_rgb_sum_data();

	schedule_delayed_work(&rgb_work, msecs_to_jiffies(DISP_GLB_TIME_PERIOD));
}

static void hisifb_rgb_stats_init(void)
{
	if (g_rgb_init) {
		HISI_FB_ERR("dubai rgb stats has been initialized");
		return;
	}

	g_rgb_init = true;
	atomic_set(&rgb_read_status, 1);
	memset(&g_rgb_data, 0, sizeof(struct hisifb_rgb_data));
	memset(&g_rgb_data_last, 0, sizeof(struct hisifb_rgb_data));
	memset(&g_rgb_data_sum, 0, sizeof(struct hisifb_rgb_data));

	schedule_delayed_work(&rgb_work, msecs_to_jiffies(DISP_GLB_TIME_PERIOD));
}

void hisifb_rgb_reg_enable(const struct hisi_fb_data_type *hisifd)
{
	if (hisifd == NULL || hisifd->dss_base == NULL) {
		HISI_FB_ERR("hisifd or dss base is NULL");
		return;
	}

	if (hisifb_rgb_check_enable())
		outp32(hisifd->dss_base + DSS_DISP_GLB_OFFSET + RGB_HIST_EN, 0x1);
}

void hisifb_rgb_start_wq(void)
{
	if (hisifb_rgb_check_enable())
		schedule_delayed_work(&rgb_work, msecs_to_jiffies(DISP_GLB_TIME_PERIOD));
}

void hisifb_rgb_cancel_wq(void)
{
	if (hisifb_rgb_check_enable())
		cancel_delayed_work_sync(&rgb_work);
}

uint32_t hisifb_rgb_get_data(struct hisifb_rgb_data *data)
{
	if (data == NULL) {
		HISI_FB_ERR("rgb data is NULL");
		return -1;
	}

	if (hisifb_rgb_check_enable()) {
		mutex_lock(&rgb_lock);
		memcpy(data, &g_rgb_data_sum, sizeof(struct hisifb_rgb_data));
		mutex_unlock(&rgb_lock);
		return 0;
	} else {
		HISI_FB_ERR("rgb stats is disable");
		return -1;
	}
}

bool hisifb_rgb_set_enable(bool enable)
{
	g_rgb_enable = enable;
	if (hisifb_rgb_check_enable()) {
		hisifb_rgb_stats_init();
		return true;
	} else {
		return false;
	}
}
