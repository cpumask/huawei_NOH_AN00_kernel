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

#ifndef __BSP_NOC_H__
#define __BSP_NOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "osl_types.h"
#include "osl_list.h"
#include "osl_sem.h"
#include "hi_noc_idle.h"

#define MST_NAME_MAX_LEN        (32)
#define NOC_BUS_NAME_MAX_LEN    (16)
#define NOC_ERR_PROBE_REG_SIZE       (64)
#define NOC_MAX_OFFSET   (0x3c)

#define NOC_RESET_GUC_MODID       0x70000009
#define NOC_RESET_NXP_MODID       0xcb000001
#define NOC_RESET_BBP_DMA0_MODID  0xcb000002
#define NOC_RESET_BBP_DMA1_MODID  0xcb000003
#define NOC_RESET_HARQ_MODID      0xcb000004
#define NOC_RESET_CPHY_MODID      0xd0000001
#define NOC_RESET_GUL2_MODID      0x31000000
#define NOC_RESET_MODID_ERR       0xffffffff
#define NOC_IDLE_TIMEOUT_CYCLE  (1000)

enum
{
    NOC_DISABLE = 0,
    NOC_ENABLE = 1
};

enum
{
    NOC_CP_RESET  = 0x0,
    NOC_AP_RESET  = 0x1
};


enum noc_test_modid_e
{
    NOC_IDLE_MODID        = 0x00,
    NOC_RSRACC_MODID      = 0x01,
    NOC_EDMA0_MODID       = 0x02,
    NOC_EDMA1_MODID       = 0x03,
    NOC_NXP_MODID         = 0x04,
    NOC_BBPDMA0_MODID     = 0x05,
    NOC_BBPDMA1_MODID     = 0x06,
    NOC_CBBE16_MODID      = 0x07,
    NOC_BBPMST0_MODID     = 0x08,
    NOC_BBPMST1_MODID     = 0x09,
    NOC_DMAC_MODID        = 0x0A,

    NOC_MODID_BUTT        = 0x0B
};


typedef struct
{
    u32 enable;
    u32 reset_enable;
}noc_err_probe_nv_s;

typedef struct
{
    u32    total          :16;
    u32    max_record_num :8;
    u32    offset         :8;
}noc_record_num_s;

typedef struct
{
    u32 magic_num;
    u32 noc_base_addr;
    noc_record_num_s record_num_info;
    u32 slice;
    u32 noc_reg[16];
}noc_dump_record_s;

typedef struct
{
    u32 modid;
    u32 errcode;
    u32 opc;
    u64 slv_addr;
    char master[16];
    u32 noc_err_probe_reg_val[16];
}noc_err_parse_info_s;

typedef int (*bsp_noc_hook_func)(noc_err_parse_info_s *parData);

typedef struct
{
    struct list_head    list;
    u32                 nocModId;
    bsp_noc_hook_func   pFunc;
    u32                 ulReserve;
}noc_hook_s;

typedef struct
{
    u32 rsv_addr;
    u32 exc_addr;
    u32 nor_addr;
    u32 ap_exc_addr;
    u32 ulReserve;
}noc_test_addr_s;

enum noc_test_mode_e
{
    NOC_MODE_OFF = 0,
    NOC_MODE_ON = 1,
    NOC_MODE_LLT = 2,
    NOC_MODE_ACT = 3,
    NOC_MODE_BUTT
};


typedef struct
{
    noc_test_addr_s test_addr;
    enum noc_test_modid_e modid;
    enum noc_test_mode_e test_mode;
    osl_sem_id noc_test_sem;
    bool noc_exc_flag;
}noc_err_test_s;

typedef struct
{
    enum noc_test_modid_e modid;
    char *mst_name;
}noc_test_modid_match_s;

typedef struct
{
    u32 modid;
    char *mst_name;
}noc_modid_match_s;

extern noc_err_test_s g_noc_err_test_cfg;
extern noc_err_probe_nv_s g_noc_err_probe_nv_cfg;


extern bsp_noc_hook_func g_NocTestFunc;


extern struct dsp_mainctrl *g_dsp_ctrl;

s32 bsp_noc_fault_handler(const char *bus_name, u32 *err_no, u32 buserr_reset_flag);
s32 bsp_noc_check_test_mode(void);
void bsp_noc_test_sem_up(void);
void bsp_noc_test_mode_start(u32 modid);
void bsp_noc_llt_mode_start(u32 modid);
void bsp_noc_test_start(u32 modid);
void bsp_noc_test_mode_stop(noc_err_parse_info_s *parse_info);
void bsp_noc_test_stop(noc_err_parse_info_s *parse_info);
u32 bsp_noc_get_rsv_addr(void);
u32 bsp_noc_get_exc_addr(void);
u32 bsp_noc_get_nor_addr(void);
u32 bsp_noc_get_exc_flag(void);
u32 bsp_noc_get_ap_exc_addr(void);
u32 bsp_noc_get_modid(void);
int bsp_noc_test_sem_down(void);
s32 bsp_noc_check_exc_flag(u32 modid, noc_err_parse_info_s *parse_info);
u32 noc_fault_dump_handler(void);

void bsp_noc_test_hook_register(bsp_noc_hook_func fn);
void bsp_noc_test_hook_unregister(void);
void bsp_noc_test_init(void);
u64 noc_get_addr_from_routeid(u32 initflow, u32 targetflow, u32 targetsubrange);
u64 noc_dmesg_get_addr_from_routeid(u32 initflow, u32 targetflow, u32 targetsubrange);

void bsp_noc_test_proc(void);
void bsp_noc_llt_test_proc(void);
void bsp_noc_reset_branch_test(void* base);
void noc_get_route_info(const void* base, u32 idx, int *initflow, int *targetflow);
u32 bsp_noc_get_mid(void);

#ifdef __cplusplus
}
#endif


#endif


