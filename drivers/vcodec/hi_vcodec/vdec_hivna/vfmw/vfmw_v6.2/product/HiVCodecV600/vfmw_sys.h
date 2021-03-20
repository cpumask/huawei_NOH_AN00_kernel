/*
 * vfmw_sys.h
 *
 * This is for vfmw_sys interface.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
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

#ifndef VFMW_SYS_H
#define VFMW_SYS_H
#include "vfmw_osal.h"

#define SCD_NUM 2
#define VDH_NUM 2

#define MDMA_NUM 0
#define VDH_REG_NUM 1
#define VDH_MSG_NUM 1
#define VDH_PXP_NUM 1
#define MDMA_REG_NUM 1
#define MDMA_MSG_NUM 1
#define SCD_REG_LEN 256
#define VDH_SRST_REQ_REG_OFS 0xc
#define VDH_SRST_OK_REG_OFS  0x14

typedef struct {
	hi_u32 vdh_all_clken : 2;
	hi_u32 vdh_mfde_clken : 2;
	hi_u32 vdh_pxp_clken : 2;
	hi_u32 vdh_scd_clken : 2;
	hi_u32 vdh_bpd_clken : 2;
	hi_u32 vdh_bsp_clken : 2;
	hi_u32 vdh_ppfc_cken : 2;
	hi_u32 vdh_crc_cken : 2;
	hi_u32 reserved : 16;
} vcrg_vdh_clksel;

typedef struct {
	hi_u32 vdh_all_srst_req : 1;
	hi_u32 vdh_mfde_srst_req : 1;
	hi_u32 vdh_scd_srst_req : 1;
	hi_u32 vdh_bpd_srst_req : 1;
	hi_u32 reserved : 28;
} vcrg_vdh_srst_req;

typedef struct {
	hi_u32 vdh_all_srst_ok : 1;
	hi_u32 vdh_mfde_srst_ok : 1;
	hi_u32 vdh_scd_srst_ok : 1;
	hi_u32 vdh_bpd_srst_ok : 1;
	hi_u32 reserved : 28;
} vcrg_vdh_srst_ok;

typedef struct {
	UADDR crg_reg_phy_addr;
	hi_u8 *crg_reg_vaddr;
	hi_u32 crg_reg_size;
	hi_u32 is_fpga;
	void *dev;
} vfmw_global_info;

#endif
