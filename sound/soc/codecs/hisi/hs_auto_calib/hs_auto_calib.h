/*
 * hs_auto_calib.h
 *
 * hs_auto_calib driver
 *
 * Copyright (c) 2016-2020 Huawei Technologies Co., Ltd.
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

#ifndef __HS_AUTO_CALIB_H__
#define __HS_AUTO_CALIB_H__

#include <linux/of.h>

enum adjust_state {
	DEFAULT = 0,
	REC_JUDGE = DEFAULT,
	ERROR_JUDGE,
	VOL_INTERZONE_READJUST,
	BTN_REPORT,
	BTN_RECORD,
	UNEXCEPTION_HANDLE
};

void headset_auto_calib_init(struct device_node *np);
void headset_auto_calib_reset_interzone(void);
void startup_fsm(enum adjust_state state, unsigned int hkadc_value, int *pr_btn_type);

/* Debug info */
#define ERROR_LEVEL     1
#define INFO_LEVEL      1
#define DEBUG_LEVEL     0

#if INFO_LEVEL
#ifdef CONFIG_HISI_AUDIO_DEBUG
#define logi(fmt, ...) \
	pr_info(LOG_TAG"[I]:%s:%d: " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define logi(fmt, ...) \
	pr_info(LOG_TAG"[I]:%d: " fmt, __LINE__, ##__VA_ARGS__)
#endif
#else
#define logi(fmt, ...)
#endif

#if DEBUG_LEVEL
#define logd(fmt, ...) \
	pr_info(LOG_TAG"[D]:%s:%d: " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define logd(fmt, ...)
#endif

#if ERROR_LEVEL
#ifdef CONFIG_HISI_AUDIO_DEBUG
#define loge(fmt, ...) \
	pr_err(LOG_TAG"[E]:%s:%d: " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define loge(fmt, ...) \
	pr_err(LOG_TAG"[E]:%d: " fmt, __LINE__, ##__VA_ARGS__)
#endif
#else
#define loge(fmt, ...)
#endif

#endif /* __HS_AUTO_CALIB_H__ */

