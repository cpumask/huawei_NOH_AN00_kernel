/*
 * vfmw_intf.h
 *
 * This is for vfmw_intf.
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

#ifndef VFMW_INTF_H
#define VFMW_INTF_H

#include "vfmw_ext.h"
#ifdef HI_TVP_SUPPORT
#include "tvp_adapter.h"
#endif

typedef enum {
	VFMW_CID_STM_PROCESS,
	VFMW_CID_GET_ACTIVE_REG,
	VFMW_CID_DEC_PROCESS,
	VFMW_CID_MAX_PROCESS,
} vfmw_cmd_type;

typedef enum {
	VFMW_STM_RESET,
	VFMW_DEC_RESET,
	VFMW_ALL_RESET
} vfmw_reset_type;

#define vfmw_check_return(cond, ret) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "check error: condition %s not match.\n", #cond); \
			return ret; \
		} \
	} while (0)

hi_s32 vfmw_control(struct file *file, vfmw_cmd_type cmd, void *arg_in, void *arg_out);
hi_s32 vfmw_query_image(void *dev_cfg, void *backup_out);
hi_s32 vfmw_init(void *args);
void vfmw_deinit(void);
void vfmw_suspend(bool sec_instance_exist);
void vfmw_resume(bool sec_instance_exist);
hi_s32 vdec_device_proc(hi_u32 *device_stat);
hi_bool vfmw_scene_ident(void);

#endif
