/*
 * dec_reg.h
 *
 * This is for dec registers definition.
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

#ifndef DEC_REG_H
#define DEC_REG_H

#include "dec_base.h"

#define VREG_OFS_DOMN_CLK_CNT 0x0
#define VREG_OFS_DOMN_CLK_BSP 0x4
#define VREG_OFS_ROM_VRG_EMA  0x10
#define VREG_OFS_PXP_CORE_EN  0x14

#define VREG_OFS_BSP_INTR_STATE    0x00
#define VREG_OFS_BSP_INTR_MSKSTATE 0x04
#define VREG_OFS_BSP_INTR_MASK     0x08
#define VREG_OFS_BSP_CUR_REG       0x0C
#define VREG_OFS_PXP_INTR_STATE    0x10
#define VREG_OFS_PXP_INTR_MSKSTATE 0x14
#define VREG_OFS_PXP_INTR_MASK     0x18
#define VREG_OFS_PXP_CUR_REG       0x1C
#define VREG_OFS_VDH_COEFF         0x20
#define VREG_OFS_BSP_CYCLES        0x24
#define VREG_OFS_PXP_CYCLES        0x28

#define VREG_OFS_DEC_STORE         0x50

#define VREG_OFS_BETWEEN_REG 0x20

#define VREG_OFS_DEC_START     0x000
#define VREG_OFS_INFO_STATE    0x004
#define VREG_OFS_AVM_ADDR_CNT  0x008
#define VREG_OFS_PROCESS_STATE 0x00C
#define VREG_OFS_PXP_START     0x010

#define VREG_OFS_MDMA_START      (MDMA_BASE + 0x00)
#define VREG_OFS_MDMA_AVM_ADDR   (MDMA_BASE + 0x04)
#define VREG_OFS_MDMA_SAFE_MMU   (MDMA_BASE + 0x08)
#define VREG_OFS_MDMA_UNID       (MDMA_BASE + 0x0C)
#define VREG_OFS_MDMA_CANCEL     (MDMA_BASE + 0x10)
#define VREG_OFS_MDMA_INTR_MASK  (MDMA_BASE + 0x14)
#define VREG_OFS_MDMA_INTR_CLR   (MDMA_BASE + 0x18)
#define VREG_OFS_MDMA_INTR_STATE (MDMA_BASE + 0x1C)

typedef struct {
	hi_u32 frame_indicator : 16;
	hi_u32 reserved : 1;
	hi_u32 intrst_vdh_dec_over : 1;
	hi_u32 intrst_vdh_dec_err : 1;
	hi_u32 reserved1 : 13;
} vreg_bsp_intr_state;

typedef struct {
	hi_u32 frame_indicator : 16;
	hi_u32 reserved : 1;
	hi_u32 intrst_vdh_dec_over : 1;
	hi_u32 intrst_vdh_dec_err : 1;
	hi_u32 intrst_vdh_dec_part : 1;
	hi_u32 reserved1 : 4;
	hi_u32 pxp_cur_reg : 3;
	hi_u32 reserved2 : 5;
} vreg_pxp_intr_state;

typedef struct {
	hi_u32 reserved : 3;
	hi_u32 bsp_interrupt_en : 1;
	hi_u32 low_latency_err_rsten : 1;
	hi_u32 vp9av1_frame_parallel : 1;
	hi_u32 mfde_mmu_en : 1;
	hi_u32 reserved1 : 25;
} vreg_coeff;

typedef struct {
	hi_u32 vdh_safe_flag : 1;
	hi_u32 use_bsp_num : 3;
	hi_u32 low_latency_en : 1;
	hi_u32 vdh_mdma_sel : 1;
	hi_u32 int_report_en : 1;
	hi_u32 reserved2 : 1;
	hi_u32 bsp_internal_ram : 1;
	hi_u32 pxp_internal_ram : 1;
	hi_u32 reserved3 : 2;
	hi_u32 frame_indicator : 16;
	hi_u32 dec_cancel : 1;
	hi_u32 pxp_state : 1;
	hi_u32 bsp_state : 1;
	hi_u32 vdec_busy_state : 1;
} vreg_info_state;

typedef struct {
	hi_u32 proc_state : 2;
	hi_u32 reserved1 : 2;
	hi_u32 set_err : 1;
	hi_u32 reserved2 : 3;
	hi_u32 vid_std : 5;
	hi_u32 reserved3 : 19;
} vreg_proc_state;

typedef struct {
	hi_u32 mbamt_to_dec : 20;
	hi_u32 reserved : 2;
	hi_u32 marker_bit_detect_en : 1;
	hi_u32 ac_last_detect_en : 1;
	hi_u32 coef_idx_detect_en : 1;
	hi_u32 vop_type_detect_en : 1;
	hi_u32 work_mode : 2;
	hi_u32 luma_sum_en : 1;
	hi_u32 luma_histogram_en : 1;
	hi_u32 load_qmatrix_flag : 1;
	hi_u32 reserved3 : 1;
} vmsg_com_b0_d2;

typedef struct {
	hi_u32 video_standard : 5;
	hi_u32 allow_intrabc : 1;
	hi_u32 sec_cmp_en : 1;
	hi_u32 sec_10b_fmt : 2;
	hi_u32 sec_path_en : 1;
	hi_u32 water_mark_en : 1;
	hi_u32 vpss_info_en : 1;
	hi_u32 sec_linear_en : 1;
	hi_u32 uv_order_en : 1;
	hi_u32 fst_slc_grp : 1;
	hi_u32 mv_output_en : 1;
	hi_u32 max_slcgrp_num : 12;
	hi_u32 line_num_output_en : 1;
	hi_u32 vdh_2d_en : 1;
	hi_u32 compress_en : 1;
	hi_u32 emar_stop : 1;
} vmsg_com_b0_d3;

typedef struct {
	hi_u32 loss_cmp_en : 1;
	hi_u32 l2bit_dispose : 1;
	hi_u32 hebc_afbc_sel : 1;
	hi_u32 reserved : 1;
	hi_u32 ddr_il_mode1 : 2;
	hi_u32 ddr_il_mode2 : 2;
	hi_u32 line_exchg_en1 : 1;
	hi_u32 line_exchg_en2 : 1;
	hi_u32 reserved2 : 6;
	hi_u32 pic_structure : 2;
	hi_u32 reserved3 : 14;
} vmsg_com_b0_d17;

typedef struct {
	hi_u32 mbamt_to_dec : 20;
	hi_u32 memory_clock_gating_en : 1;
	hi_u32 module_clock_gating_en : 1;
	hi_u32 marker_bit_detect_en : 1;
	hi_u32 ac_last_detect_en : 1;
	hi_u32 coef_idx_detect_en : 1;
	hi_u32 vop_type_detect_en : 1;
	hi_u32 work_mode : 2;
	hi_u32 luma_sum_en : 1;
	hi_u32 luma_histogram_en : 1;
	hi_u32 load_qmatrix_flag : 1;
	hi_u32 vdh_safe_flag : 1;
} vreg_basic_cfg0;

typedef struct {
	hi_u32 video_standard : 4;
	hi_u32 reserved : 7;
	hi_u32 vpss_info_en : 1;
	hi_u32 mfd_mmu_en : 1;
	hi_u32 uv_order_en : 1;
	hi_u32 fst_slc_grp : 1;
	hi_u32 mv_output_en : 1;
	hi_u32 max_slcgrp_num : 12;
	hi_u32 line_num_output_en : 1;
	hi_u32 vdh_2d_en : 1;
	hi_u32 frm_cmp_en : 1;
	hi_u32 ppfd_en : 1;
} vreg_basic_cfg1;

#endif
