/*
 * Copyright (c) 2013, Hisilicon Tech. Co., Ltd. All rights reserved.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#ifndef __SOUNDTRIGGER_LOG_H__
#define __SOUNDTRIGGER_LOG_H__

#define DEBUG_LEVEL 1

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#ifndef INFO_LEVEL
#define INFO_LEVEL	1
#endif

#ifndef LOG_TAG
#define LOG_TAG "SoundTrigger"
#endif

#if DEBUG_LEVEL
#define logd(fmt, ...) pr_err(LOG_TAG"[D]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) /*lint !e683 */
#else
#define logd(fmt, ...)
#endif

#if INFO_LEVEL
#ifdef CONFIG_HISI_AUDIO_DEBUG
#define logi(fmt, ...) pr_info(LOG_TAG"[I]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) /*lint !e683 */
#else
#define logi(fmt, ...) pr_info(LOG_TAG"[I]:%d: "fmt, __LINE__, ##__VA_ARGS__) /*lint !e683 */
#endif
#else
#define logi(fmt, ...)
#endif

#ifdef CONFIG_HISI_AUDIO_DEBUG
#define logw(fmt, ...) pr_warn(LOG_TAG"[W]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define loge(fmt, ...) pr_err(LOG_TAG"[E]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); /*lint !e683 */
#else
#define logw(fmt, ...) pr_warn(LOG_TAG"[W]:%d: "fmt, __LINE__, ##__VA_ARGS__);
#define loge(fmt, ...) pr_err(LOG_TAG"[E]:%d: "fmt, __LINE__, ##__VA_ARGS__); /*lint !e683 */
#endif

#define IN_FUNCTION   logd("in\n");
#define OUT_FUNCTION  logd("out\n");

#endif
