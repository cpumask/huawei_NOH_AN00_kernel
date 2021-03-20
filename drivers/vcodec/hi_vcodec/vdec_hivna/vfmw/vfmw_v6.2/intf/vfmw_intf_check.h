/*
 * vfmw_intf_check.h
 *
 * This is for vfmw_intf_check.
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

#ifndef VFMW_INTF_CHECK_H
#define VFMW_INTF_CHECK_H

#include "vfmw_ext.h"

#define ONE_DMSG_SIZE        (4 * sizeof(hi_u32))
#define SCD_DMSG_SIZE        (SCD_DMSG_NUM * ONE_DMSG_SIZE)
#define SEG_TOP_BLANK_SIZE   (4 * 1024)
#define SEG_BTM_BLANK_SIZE   (1 * 1024)
#define MAX_U32              0xffffffff
#define MAX_U16              0xffff
#define align_up(val, align) (((val) + ((align) - 1)) & ~((align) - 1))

/* standard type for vfmw common use */
enum vfmw_vid_std {
	VFMW_START_RESERVED = 0,
	VFMW_H264,
	VFMW_VC1,
	VFMW_MPEG4,
	VFMW_MPEG2,
	VFMW_H263,
	VFMW_DIVX3,
	VFMW_AVS,
	VFMW_JPEG,
	VFMW_REAL8,
	VFMW_REAL9,
	VFMW_VP6,
	VFMW_VP6F,
	VFMW_VP6A,
	VFMW_VP8,
	VFMW_SORENSON,
	VFMW_MVC,
	VFMW_HEVC,
	VFMW_VP9,
	VFMW_AVS2,
	VFMW_AV1,
	VFMW_AVS3,
	VFMW_MJPEG,
	VFMW_WMV1,
	VFMW_WMV2,
	VFMW_WMV3,
	VFMW_STD_MAX
};

/* standard type for stm only */
enum vfmw_stm_vid {
	VFMW_STM_AVC_HEVC = 0,
	VFMW_STM_MPEG4 = 2,
	VFMW_STM_MPEG2 = 3,
	VFMW_STM_VP9 = 18,
	VFMW_STM_VID_MAX
};

struct vdec_seg_buffer_addr {
	struct file  *file;
	UADDR seg_buffer_first;
	UADDR seg_buffer_last;
	hi_u32 pre_lsb;
	hi_u32 pre_msb;
};

struct vdec_address_info {
	struct mutex vdec_check_mutex;
	UADDR dec_pub_msg_phy_addr;
	UADDR scd_dn_msg_phy;
	UADDR scd_up_msg_phy;
	struct vdec_seg_buffer_addr seg_buffer_addr[MAX_OPEN_COUNT];
};

hi_s32 vfmw_reset_vdh(void *dev_cfg);
hi_s32 vfmw_dec_check_reg_id(void *dev_cfg);
void vfmw_init_seg_buffer_addr(struct file *fd);
hi_s32 vfmw_dec_param_check(void *dev_cfg);
hi_s32 vfmw_stm_param_check(void *fd, void *stm_cfg);
void vfmw_vdec_addr_init(void);
void vfmw_vdec_addr_clear(void);
hi_s32 vfmw_update_pre_sb(void *fd, void *scd_state_reg);

#endif
