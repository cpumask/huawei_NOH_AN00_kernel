/*
 * vfmw_ext.h
 *
 * This is for vfmw.
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

#ifndef VFMW_EXT_H
#define VFMW_EXT_H

#include "hi_type.h"

typedef enum {
	MFDE_MODULE,
	SCD_MOUDLE,
	CRG_MOUDLE,
	MMU_SID_MOUDLE,
	DP_MONIT_MOUDLE,
	MMU_TBU_MODULE,
	MAX_INNER_MODULE,
} vfmw_inner_module;

typedef struct {
	UADDR reg_phy_addr;
	hi_u32 reg_range;
} vfmw_module_reg_info;

typedef struct {
	hi_u32 irq_norm;
	hi_u32 irq_safe;
	hi_u32 is_es_plat;
	hi_u32 is_fpga;
	vfmw_module_reg_info module_reg[MAX_INNER_MODULE];
	struct device *dev;
} vdec_dts;

typedef enum {
	MEM_SHR_ERR = 0,
	MEM_SHR_CTG,
	MEM_MMU_MMU,
	MEM_MMU_SEC,
} mem_mode;

typedef struct {
	hi_u64 fd;
	UADDR phy_addr;
	UADDR mmu_addr;
	hi_u64 vir_addr;
	hi_s32 length;
	mem_mode mode;
	hi_handle ssm_handle;
	hi_handle vdec_handle;
} vfmw_mem_desc;

typedef struct {
	hi_u8 int_mask;
	hi_u8 scd_start;
	hi_u8 scd_init_clr;
	hi_u8 scd_mmu_en;
	hi_u8 vdh_mmu_en;
	hi_u8 avs_flag;
	hi_u8 slice_check;
	hi_u8 safe_flag;
	hi_u8 scd_over;
	hi_s32 buffer_share_fd;
	UADDR buffer_first;
	UADDR buffer_last;
	UADDR buffer_init;
	UADDR roll_addr;
	UADDR next_addr;
	hi_s32 msg_share_fd;
	UADDR dn_msg_phy;
	UADDR up_msg_phy;
	hi_u32 up_len;
	hi_u32 up_step;
	hi_u32 std_type;
	hi_u32 pre_lsb;
	hi_u32 pre_msb;
	hi_u32 byte_valid;
	hi_u32 short_num;
	hi_u32 start_num;
	hi_u32 src_eaten;
	hi_u32 cfg_unid;
	hi_u32 int_unid;
	hi_s32 ret_errno;
} scd_reg;

typedef struct {
	hi_s32 scd_id;
	scd_reg reg;
} scd_reg_ioctl;

typedef enum {
	DEC_DEV_TYPE_NULL = 0,
	DEC_DEV_TYPE_VDH,
	DEC_DEV_TYPE_MDMA,
	DEC_DEV_TYPE_MAX,
} dec_dev_type;

typedef struct {
	hi_u32 dev_id;
	dec_dev_type type;
	hi_u16 reg_id;
} dev_cfg_ioctl;

typedef struct {
	dev_cfg_ioctl dev_cfg_info;
	hi_u16 cfg_id;
	hi_u8 vdh_mmu_en;
	hi_u8 vdh_in_ram;
	hi_u8 vdh_safe_flag;
	hi_u8 mask_mmu_irq;
	UADDR pub_msg_phy_addr;
	hi_s32 msg_shared_fd;
	hi_u32 is_slc_ldy;
	hi_u32 is_frm_cpy;
	hi_u16 list_num;
	hi_u32 std_cfg;
	hi_u32 cur_time;
	hi_s32 ret_errno;
} dec_dev_cfg;

typedef struct {
	hi_u32 int_reg;
	hi_u32 int_flag;
	hi_u32 int_unid;
	hi_u32 err_flag;
	hi_u32 rd_slc_msg;
	hi_u32 slice_num;
	hi_u32 ctb_num;
	hi_u32 bsp_cycle;
	hi_u32 pxp_cycle;
	hi_u32 is_need_wait;
	hi_s32 ret_errno;
} dec_back_up;

#endif
