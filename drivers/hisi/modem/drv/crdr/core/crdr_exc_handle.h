/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __DUMP_EXC_TYPE_H__
#define __DUMP_EXC_TYPE_H__
#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#else
#include <bsp_rdr.h>
#endif
#include "bsp_dump.h"
/* modem cp异常的rdr id */
#define RDR_MODEM_CP_DRV_MOD_ID_START 0x00000000
#define RDR_MODEM_CP_DRV_MOD_ID_END 0x0fffffff
#define RDR_MODEM_CP_OSA_MOD_ID_START 0x10000000
#define RDR_MODEM_CP_OSA_MOD_ID_END 0x1fffffff
#define RDR_MODEM_CP_OAM_MOD_ID_START 0x20000000
#define RDR_MODEM_CP_OAM_MOD_ID_END 0x2fffffff
#define RDR_MODEM_CP_GUL2_MOD_ID_START 0x30000000
#define RDR_MODEM_CP_GUL2_MOD_ID_END 0x3effffff
#define RDR_MODEM_CP_CTTF_MOD_ID_START 0x3f000000
#define RDR_MODEM_CP_CTTF_MOD_ID_END 0x3fffffff
#define RDR_MODEM_CP_GUWAS_MOD_ID_START 0x40000000
#define RDR_MODEM_CP_GUWAS_MOD_ID_END 0x4dffffff
#define RDR_MODEM_CP_CAS_MOD_ID_START 0x4e000000
#define RDR_MODEM_CP_CAS_MOD_ID_END 0x4effffff
#define RDR_MODEM_CP_CPROC_MOD_ID_START 0x4f000000
#define RDR_MODEM_CP_CPROC_MOD_ID_END 0x4fffffff
#define RDR_MODEM_CP_GUGAS_MOD_ID_START 0x50000000
#define RDR_MODEM_CP_GUGAS_MOD_ID_END 0x5fffffff
#define RDR_MODEM_CP_GUCNAS_MOD_ID_START 0x60000000
#define RDR_MODEM_CP_GUCNAS_MOD_ID_END 0x6fffffff
#define RDR_MODEM_CP_GUDSP_MOD_ID_START 0x70000000
#define RDR_MODEM_CP_GUDSP_MOD_ID_END 0x75ffffff
#define RDR_MODEM_CP_EASYRF_MOD_ID_START 0x76000000
#define RDR_MODEM_CP_EASYRF_MOD_ID_END 0x7fffffff
#define RDR_MODEM_CP_LPS_MOD_ID_START 0xa0000000
#define RDR_MODEM_CP_LPS_MOD_ID_END 0xafffffff
#define RDR_MODEM_CP_MSP_MOD_ID_START 0xb0000000
#define RDR_MODEM_CP_MSP_MOD_ID_END 0xbfffffff
#define RDR_MODEM_CP_TLDSP_MOD_ID_START 0xc0000000
#define RDR_MODEM_CP_TLDSP_MOD_ID_END 0xc1ffffff
#define RDR_MODEM_CP_NRDSP_MOD_ID_START 0xc2000000
#define RDR_MODEM_CP_NRDSP_MOD_ID_END 0xcfffffff
#define RDR_MODEM_CP_CPHY_MOD_ID_START 0xd0000000
#define RDR_MODEM_CP_CPHY_MOD_ID_END 0xdfffffff
#define RDR_MODEM_CP_IMS_MOD_ID_START 0xe0000000
#define RDR_MODEM_CP_IMS_MOD_ID_END 0xefffffff

/* rdr为CP分配的ID范围为0x82000000~0x82ffffff，定义在bsp_rdr.h中 */
#define RDR_MODEM_AP_MOD_ID 0x82000000                  /* AP异常，保存log，整机复位，phone和mbb都有 */
#define RDR_MODEM_CP_MOD_ID 0x82000001                  /* CP异常，保存log，phone和mbb都有 */
#define RDR_MODEM_CP_WDT_MOD_ID 0x82000003              /* CP看门狗异常，保存log，phone和mbb都有 */
#define RDR_MODEM_CP_RESET_SIM_SWITCH_MOD_ID 0x82000005 /* 切卡引起的CP单独复位，不保存log，phone独有 */
#define RDR_MODEM_CP_RESET_FAIL_MOD_ID 0x82000006       /* CP单独复位失败，保存log，phone独有 */
#define RDR_MODEM_CP_RESET_RILD_MOD_ID 0x82000007       /* RILD引起的CP单独复位，保存log，phone独有 */
#define RDR_MODEM_CP_RESET_3RD_MOD_ID 0x82000008        /* 3rd modem引起的CP单独复位，保存log，phone独有 */
#define RDR_MODEM_CP_RESET_REBOOT_REQ_MOD_ID 0x82000009 /* CP单独复位为桩时，整机复位，保存log，phone独有 */
#define RDR_MODEM_CP_RESET_USER_RESET_MOD_ID 0x82000010 /* CP单独复位,用户发起，不保存log,PHONE 独有 */
#define RDR_MODEM_CP_RESET_DLOCK_MOD_ID 0x82000011      /* modem总线挂死 */
#define RDR_MODEM_CP_NOC_MOD_ID 0x82000012              /* modem内部触发noc */
#define RDR_MODEM_AP_NOC_MOD_ID 0x82000013              /* modem访问AP触发noc */
#define RDR_MODEM_CP_RESET_FREQUENTLY_MOD_ID 0x82000014 /* c 核频繁单独复位，整机重启 */
#define RDR_MODEM_NR_MOD_ID 0x82000015                  /* NR异常，保存log，phone有 */
#define RDR_MODEM_NR_L2HAC_MOD_ID 0x82000016            /* L2HAC异常，保存log，phone有 */
#define RDR_MODEM_NR_CCPU_WDT 0x82000017                /* NRWDT异常，保存log，phone有 */

#define RDR_MODEM_NOC_MOD_ID 0x82000030  /* NOC异常, 保存log，phone独有, */
                                         /* 如果要修改者定义一定要知会AP侧同步修改，否则会导致NOC异常log存储不下来 */
#define RDR_MODEM_DMSS_MOD_ID 0x82000031 /* DMSS异常, 保存log，phone独有, */
                                         /* 如果要修改者定义一定要知会AP侧同步修改，否则会导致DMSS异常log存储不下来 */
#define RDR_PHONE_MDMAP_PANIC_MOD_ID 0x80000030 /* modemap 代码panic后，kirin传入的modid */

#define RDR_MODEM_CP_DRV_MOD_ID 0x82000041
#define RDR_MODEM_CP_OSA_MOD_ID 0x82000042
#define RDR_MODEM_CP_OAM_MOD_ID 0x82000043
#define RDR_MODEM_CP_GUL2_MOD_ID 0x82000044
#define RDR_MODEM_CP_CTTF_MOD_ID 0x82000045
#define RDR_MODEM_CP_GUWAS_MOD_ID 0x82000046
#define RDR_MODEM_CP_CAS_MOD_ID 0x82000047
#define RDR_MODEM_CP_CPROC_MOD_ID 0x82000048
#define RDR_MODEM_CP_GUGAS_MOD_ID 0x82000049
#define RDR_MODEM_CP_GUCNAS_MOD_ID 0x8200004a
#define RDR_MODEM_CP_GUDSP_MOD_ID 0x8200004b
#define RDR_MODEM_CP_LPS_MOD_ID 0x8200004c
#define RDR_MODEM_CP_LMSP_MOD_ID 0x8200004d
#define RDR_MODEM_CP_TLDSP_MOD_ID 0x8200004e
#define RDR_MODEM_CP_CPHY_MOD_ID 0x8200004f
#define RDR_MODEM_CP_IMS_MOD_ID 0x82000050
#define RDR_MODEM_AP_DRV_MOD_ID 0x82000051
#define RDR_MODEM_LPM3_MOD_ID 0x82000057
#define RDR_MODEM_CP_EASYRF_MOD_ID 0x82000060
#define RDR_MODEM_CP_NRDSP_MOD_ID 0x82000061

#define RDR_MODEM_CODE_PATCH_REVERT_MOD_ID 0x82000100
#define RDR_MODEM_DRV_BUTT_MOD_ID 0x82ffffff

#define RDR_DUMP_FILE_PATH_LEN 128
#define RDR_DUMP_FILE_CP_PATH "cp_log/"
#define RDR_MODEM_CP_RESET_TIMEOUT (30000)

#define RDR_MODEM_NR_MOD_ID_START 0xb0000000
#define RDR_MODEM_NR_MOD_ID_END 0xbfffffff

#define NRRDR_MODEM_NR_CCPU_START (0xB1000000)
#define NRRDR_MODEM_NR_CCPU_END (0xB1FFFFFF)

#define NRRDR_MODEM_NR_L2HAC_START (0xB2000000)
#define NRRDR_MODEM_NR_L2HAC_END (0xB2FFFFFF)
#define NRRDR_MODEM_NR_HL1C_START (0xB3000000)
#define NRRDR_MODEM_NR_HL1C_END (0xB3FFFFFF)
#define NRRDR_MODEM_NR_CCPU_WDT (0xB4000000)

#define NRRDR_EXC_CCPU_DRV_MOD_ID (NRRDR_MODEM_NR_CCPU_START)
#define NRRDR_EXC_CCPU_OSA_MOD_ID (0xB1100000)
#define NRRDR_EXC_CCPU_OAM_MOD_ID (0xB1200000)
#define NRRDR_EXC_CCPU_GUL2_MOD_ID (0xB1300000)
#define NRRDR_EXC_CCPU_CTTF_MOD_ID (0xB1400000)
#define NRRDR_EXC_CCPU_GUWAS_MOD_ID (0xB1500000)
#define NRRDR_EXC_CCPU_CAS_MOD_ID (0xB1600000)
#define NRRDR_EXC_CCPU_CPROC_MOD_ID (0xB1700000)
#define NRRDR_EXC_CCPU_GUGAS_MOD_ID (0xB1800000)
#define NRRDR_EXC_CCPU_NRNAS_MOD_ID (0xB1900000)
#define NRRDR_EXC_CCPU_NRPHY_MOD_ID (0xB1a00000)
#define NRRDR_EXC_CCPU_NRPS_MOD_ID (0xB1b00000)
#define NRRDR_EXC_CCPU_NRDSP_MOD_ID (0xB1d00000)
#define NRRDR_EXC_CCPU_IMS_MOD_ID (0xB1f00000)
#define NRRDR_EXC_CCPU_END_ID (NRRDR_MODEM_NR_CCPU_END)

#define RDR_MODEM_REVERSE_MOD_ID_START 0xF0000000
#define RDR_MODEM_REVERSE_MOD_ID_END 0xffffffff

#define WAIT_EXCEPTION_HANDLE_TIME (45 * 1000 * 1000UL) /* 保存log和单独复位一共的时间目前设定为45s */
#define MODEM_EXC_DESC_SUM (64)
#define NRCCPU_EXCEPTION ("NRCCPU EXCEPTION")
#define NRL2HAC_EXCEPTION ("NRL2HAC EXCEPTION")

typedef struct {
    u32 modid;
    u32 product_type;
    u64 coreid;
    char log_path[RDR_DUMP_FILE_PATH_LEN];
    pfn_cb_dump_done dump_done;
} rdr_exc_info_s;

typedef enum {
    EXC_INFO_NORMAL = 0,
    EXC_INFO_NOC,
    EXC_INFO_DMSS,
    EXC_INFO_BUTT,
} EXC_INFO_INDEX;

typedef struct modid {
    u32 mdm_id_start;
    u32 mdm_id_end;
    u32 rdr_id;
} DUMP_MOD_ID;
typedef enum {
    RESET_NOT_SUPPORT = 0,
    RESET_NOT_SUCCES = 1,
    RESET_SUCCES = 2,
} dump_reset_result_e;

rdr_exc_info_s *crdr_get_rdr_exc_info_by_modid(u32 modid);
s32 crdr_register_modem_exc_info(void);
dump_exception_info_s *crdr_get_current_excpiton_info(u32 modid);
s32 crdr_register_exception(dump_exception_info_s *exception_info);
s32 crdr_exception_handler_init(void);
u32 crdr_get_exc_index(u32 modid);
u32 crdr_match_special_rdr_id(u32 modid);
void crdr_set_exc_state(u32 state);
u32 crdr_get_exc_state( void);
#endif
