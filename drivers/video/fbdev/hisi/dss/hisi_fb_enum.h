/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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
#ifndef HISI_FB_ENUM_H
#define HISI_FB_ENUM_H

#include <linux/console.h>
#include <linux/uaccess.h>
#include <linux/leds.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/fb.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/raid/pq.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/backlight.h>
#include <linux/pwm.h>
#include <linux/pm_runtime.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/memblock.h>
#include <linux/syscalls.h>

#include <linux/spi/spi.h>

#include <linux/ion.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/gpio.h>

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>

#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/pinctrl/consumer.h>
#include <linux/file.h>
#include <linux/dma-buf.h>
#include <linux/genalloc.h>
#include <linux/hisi-iommu.h>

#include <linux/atomic.h>

#include "hisi_dss_ion.h"
#include "hisi_dss_iommu.h"
#include "hisi_fb_panel.h"
#include "hisi_fb_debug.h"
#include "hisi_dss.h"
#include "hisi_mipi_dsi.h"
#include <soc_dss_interface.h>
#include "hisi_overlay_cmdlist_utils.h"
#include "hisi_hiace_single_mode.h"
#include "hisi_display_effect.h"
#include "hisi_fb_sysfs.h"
#include "overlay/hisi_overlay_utils_struct.h"

#if defined(CONFIG_HISI_FB_V501)
#include "hisi_overlay_utils_dssv501.h"
#include "hisi_dpe_pipe_clk_utils.h"
#elif defined(CONFIG_HISI_FB_V320)
#include "hisi_overlay_utils_dssv320.h"
#elif defined(CONFIG_HISI_FB_V510)
#include "overlay/hisi_overlay_utils_dssv510.h"
#elif defined(CONFIG_HISI_FB_V600)
#include "hisi_overlay_utils_dssv600.h"
#elif defined(CONFIG_HISI_FB_V330)
#include "hisi_overlay_utils_dssv330.h"
#elif defined(CONFIG_HISI_FB_V350)
#include "hisi_overlay_utils_dssv350.h"
#elif defined(CONFIG_HISI_FB_V345)
#include "hisi_overlay_utils_dssv345.h"
#elif defined(CONFIG_HISI_FB_V346)
#include "hisi_overlay_utils_dssv346.h"

#endif

#include "hisi_dpe_utils.h"
#include "performance/hisi_dss_performance.h"
#include "overlay/hisi_overlay_utils.h"
#include "hisi_fb_video_idle.h"
#include "hisi_ovl_online_wb.h"
#include "hisi_dss_sync.h"
#include "hisi_spr_dsc.h"
#ifdef CONFIG_HISI_DPP_CMDLIST
#include "hisi_dpp_cmdlist_utils.h"
#endif

#ifdef CONFIG_HISI_FB_MEDIACOMMON
#include "mediacommon/hisi_mdc_dev.h"
#endif

#ifdef CONFIG_HISI_FB_ALSC
#include "effect/display_effect_alsc.h"
#endif

#ifdef CONFIG_HISI_FB_AOD
#define CONFIG_SH_AOD_ENABLE 1
#else
#define CONFIG_SH_AOD_ENABLE 0
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define VIDEO_IDLE_GPU_COMPOSE_ENABLE 1
#else
#define VIDEO_IDLE_GPU_COMPOSE_ENABLE 0
#endif

#if defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
#define SUPPORT_RCH_CLD
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define SUPPORT_HEBC_ROT_CAP
#define SUPPORT_SPR_DSC
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
#define MIPI_RESOURCE_PROTECTION
/* mipi_dsi_bit_clk_update wait count, unit is ms */
#define MIPI_CLK_UPDT_TIMEOUT 30
/* esd check wait for mipi_resource_available count, unit is ms */
#define ESD_WAIT_MIPI_AVAILABLE_TIMEOUT 64
#endif

#if defined(CONFIG_HISI_FB_V330) || defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
#define SUPPORT_FPGA_SUSPEND_RESUME
#endif

#if defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define LOW_TEMP_VOLTAGE_LIMIT
#endif

#if defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
/* support dynamic clk adapter (DCA) without voltage vote */
#define SUPPORT_DCA_WITHOUT_VOLT
#endif

#if defined(CONFIG_HISI_FB_V350)
/* support copybit offline compose use wch scaler */
#define SUPPORT_COPYBIT_WCH_SCL
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
/* after 990, dsi instead ldi */
#define SUPPORT_DSI_VER_2_0

/* after 990, if dsc configure to single pipe, pic_width must
 * consistent with DDIC,when dsc configure to double pipe,pic_width
 * must be half the size of the panel
 */
#define SUPPORT_DSC_PIC_WIDTH

/* after 990, supports two dpp channels */
#define SUPPORT_DISP_CHANNEL
#endif

#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
#define SUPPORT_DSS_BUS_IDLE_CTRL
#endif

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
#define SUPPORT_FULL_CHN_10BIT_COLOR
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
/* after 990, supports copybit vote(core clock,voltage) */
#define SUPPORT_COPYBIT_VOTE_CTRL
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) ||  defined(CONFIG_HISI_FB_V346)
/* after 990, dump disp dbuf dsi info */
#define SUPPORT_DUMP_REGISTER_INFO
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define SUPPORT_DSS_HEBCE
#define SUPPORT_DSS_HEBCD
#endif

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V600)
#define SUPPORT_DSS_HFBCD
#endif

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
#define SUPPORT_DSS_HFBCE
#endif

#if !defined(CONFIG_HISI_FB_V320) && !defined(CONFIG_HISI_FB_V330)
#define SUPPORT_DSS_AFBC_ENHANCE
#endif

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V346)
#define ESD_RESTORE_DSS_VOLTAGE_CLK
#endif

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V330) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
/* low power cmds trans under video mode */
#define MIPI_DSI_HOST_VID_LP_MODE
#endif

#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
/* after 501, mmbuf's clock is configurable */
#define SUPPORT_SET_MMBUF_CLK
#endif

#if !defined(CONFIG_HISI_FB_V320) && !defined(CONFIG_HISI_FB_V330)
/* after 970, no need to manually clean cmdlist */
#define SUPPORT_CLR_CMDLIST_BY_HARDWARE
#endif

#if defined(CONFIG_HISI_FB_V600) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V345) || defined(CONFIG_HISI_FB_V346)
/* after 350, 600, supports afbcd online rot */
#define SUPPORT_AFBCD_ROT
#endif

#if defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330) || \
	defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501)
#define PANEL_MODE_SWITCH_SUPPORT
#endif

#define CONFIG_HISI_FB_BACKLIGHT_DELAY

#define CONFIG_BACKLIGHT_2048

#define HISI_DSS_COMPOSER_HOLD_TIME (1000UL * 3600 * 24 * 5)

#define HISI_FB0_NUM 3
#define HISI_FB1_NUM 0
#define HISI_FB2_NUM 0
#define HISI_FB3_NUM 0

#define HISI_FB_SYSFS_ATTRS_NUM 64

#define HISI_FB_MAX_DEV_LIST 32
#define HISI_FB_MAX_FBI_LIST 32

#define HISI_DSS_OFFLINE_MAX_NUM 2
#define HISI_DSS_OFFLINE_MAX_LIST 128
#define ONLINE_PLAY_LOG_PRINTF 10
#define BACKLIGHT_LOG_PRINTF 16
#define ARSR_POST_COLUMN_PADDING_NUM 4
#define MAX_MDC_CHANNEL 3

#define round1(x, y)  ((x) / (y) + ((x) % (y)  ? 1 : 0))
#define dss_reduce(x) ((x) > 0 ? ((x) - 1) : (x))

#define _effect_debug_log(message, ...) \
	do { printk("%s: "message, __func__, ## __VA_ARGS__); } while (0)
#define effect_debug_log(feature, message, ...) \
	do { if (g_debug_effect & feature) \
		printk("%s: "message, __func__, ## __VA_ARGS__); } while (0)

/* esd check period-->5000ms */
#define ESD_CHECK_TIME_PERIOD 5000
/* emi protect period-->120000ms */
#define HISI_EMI_PROTECT_CHECK_MAX_COUNT (120000 / ESD_CHECK_TIME_PERIOD)

#define DSM_CREATE_FENCE_FAIL_EXPIRE_COUNT 6

/* UD fingerprint */
#define UD_FP_HBM_LEVEL 993
#define UD_FP_CURRENT_LEVEL 500

/*
 * If vsync delay is enable, the vsync_delay_count can not be set to 0.
 * It is recommended to set to a small value to avoid the abnormality of vsync_delay function
 */
#define VSYNC_DELAY_TIME_DEFAULT 0x01

enum mask_layer_state {
	MASK_LAYER_COMMON_STATE = 0x01, /* all the other scene except screen-on */
	MASK_LAYER_SCREENON_STATE = 0x02, /* when screen on, add mask layer and hbm */
	CIRCLE_LAYER_STATE = 0x04, /* circle layer */
};

enum mask_layer_change_status {
	MASK_LAYER_NO_CHANGE = 0x00, /* mask layer status is no change */
	MASK_LAYER_COMMON_ADDED = 0x01, /* when mask layer is added (not screenon) */
	MASK_LAYER_SCREENON_ADDED = 0x02, /* when mask layer is added (screen-on) */
	MASK_LAYER_REMOVED = 0x03, /* when mask layer is removed */
	CIRCLE_LAYER_ADDED = 0x04, /* when circle layer is added */
	CIRCLE_LAYER_REMOVED = 0x05, /* when circle layer is removed */
};

enum dss_sec_event {
	DSS_SEC_DISABLE = 0,
	DSS_SEC_ENABLE,
};

enum dss_sec_status {
	DSS_SEC_IDLE = 0,
	DSS_SEC_RUNNING,
};

enum bl_control_mode {
	REG_ONLY_MODE = 1,
	PWM_ONLY_MODE,
	MUTI_THEN_RAMP_MODE,
	RAMP_THEN_MUTI_MODE,
	I2C_ONLY_MODE = 6,
	BLPWM_AND_CABC_MODE,
	COMMON_IC_MODE = 8,
	AMOLED_NO_BL_IC_MODE = 9,
	BLPWM_MODE = 10,
};

enum ESD_RECOVER_STATE {
	ESD_RECOVER_STATE_NONE = 0,
	ESD_RECOVER_STATE_START = 1,
	ESD_RECOVER_STATE_COMPLETE = 2,
};

enum DPP_BUF_WORK_MODE {
	DPP_BUF_WORK_MODE_ROTATION = 0,
	DPP_BUF_WORK_MODE_BOTH_EFFECT = 1,
};

enum {
	PIPE_SWITCH_CONNECT_DSI0 = 0,
	PIPE_SWITCH_CONNECT_DSI1 = 1,
};

enum {
	FREE = 0,
	HWC_USED = 1,
	MDC_USED = 2,
};

#endif /* HISI_FB_ENUM_H */
