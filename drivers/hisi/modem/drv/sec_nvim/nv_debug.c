﻿/*
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

#include <product_config.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/rtc.h>
#include <securec.h>
#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#else
#include <bsp_rdr.h>
#endif
#include <acore_nv_stru_drv.h>
#include <mdrv_rfile.h>
#include <bsp_dump.h>
#include <bsp_slice.h>
#include <bsp_nvim.h>
#include <bsp_version.h>
#include <bsp_rfile.h>
#include "nv_partition_img.h"
#include "nv_comm.h"
#include "nv_ctrl.h"
#include "nv_debug.h"
#include "nv_index.h"


#define THIS_MODU mod_nv

NV_DEBUG_CFG_STRU g_nv_debug_cfg;
struct nv_global_debug_stru g_nv_debug[NV_FUN_MAX_ID];
struct nv_debug_stru g_nv_debug_info = {0};
debug_table_t g_nv_write_debug_table[NV_DEBUG_BUTT - 1] = {
    { NV_DEBUG_WRITEEX_START, "write nv start" },
    { NV_DEBUG_WRITEEX_GET_IPC_START, "for check crc,start to get ipc sem" },
    { NV_DEBUG_WRITEEX_GET_IPC_END, "get ipc sem end" },
    { NV_DEBUG_WRITEEX_GIVE_IPC, "check crc end, release ipc sem" },
    { NV_DEBUG_WRITEEX_MEM_START, "write to mem start" },
    { NV_DEBUG_WRITEEX_FILE_START, "write to file start" },
    { NV_DEBUG_FLUSH_START, "flush nv list start" },
    { NV_DEBUG_FLUSH_END, "flush nv list end" },
    { NV_DEBUG_REQ_FLUSH_START, "req flush nv list start" },
    { NV_DEBUG_REQ_FLUSH_END, "req flush nv list end" },
    { NV_DEBUG_FLUSHEX_START, "flush nv to file start" },
    { NV_DEBUG_FLUSHEX_OPEN_START, "open nv file start" },
    { NV_DEBUG_FLUSHEX_OPEN_END, "open nv file end" },
    { NV_DEBUG_FLUSHEX_GET_PROTECT_SEM_START, "before write to file get ipc and sem start" },
    { NV_DEBUG_FLUSHEX_GET_PROTECT_SEM_END, "before write to file get ipc and sem end" },
    { NV_DEBUG_FLUSHEX_GIVE_IPC, "write to file release ipc" },
    { NV_DEBUG_FLUSHEX_GIVE_SEM, "release sem" },
    { NV_DEBUG_FLUSHEX_WRITE_FILE_START, "write to nv.bin start" },
    { NV_DEBUG_FLUSHEX_WRITE_FILE_END, "write to nv.bin end" },
    { NV_DEBUG_WRITEEX_END, "write nv end" },
    { NV_DEBUG_RECEIVE_ICC, "recive icc from ccore" },
    { NV_DEBUG_SEND_ICC, "cnf to ccore" },
    { NV_DEBUG_READ_ICC, "read icc from ccore" },
    { NV_DEBUG_REQ_ASYNC_WRITE, "send async write nv msg" },
    { NV_DEBUG_REQ_REMOTE_WRITE, "read icc from ccore" }
};

#define NV_DEBUG_CHECK_FILE_LEN 255
s8 g_debug_check_file[NV_DEBUG_CHECK_FILE_LEN] = {0};
u32 g_debug_check_flag = 0;

void nv_debug_QueueInit(nv_queue_t *Q, u32 elementNum);

void nv_debug_QueueInit(nv_queue_t *Q, u32 elementNum)
{
    int ret;
    Q->maxNum = elementNum;
    Q->front = 0;
    Q->rear = 0;
    Q->num = 0;

    ret = memset_s((void *)Q->data, (size_t)(elementNum * sizeof(nv_queue_elemnt)), 0,
                   (size_t)(elementNum * sizeof(nv_queue_elemnt)));
    if (ret != EOK) {
        nv_printf("memset err\n");
    }
}

static __inline__ s32 nv_debug_QueueIn(nv_queue_t *Q, nv_queue_elemnt element)
{
    if (Q->num == Q->maxNum) {
        return -1;
    }

    Q->data[Q->rear].state = element.state;
    Q->data[Q->rear].slice = element.slice;
    Q->rear = (Q->rear + 1) % Q->maxNum;
    Q->num++;

    return 0;
}

static s32 nv_debug_QueueLoopIn(nv_queue_t *Q, nv_queue_elemnt element)
{
    if (Q->num < Q->maxNum) {
        return nv_debug_QueueIn(Q, element);
    } else {
        Q->data[Q->rear].state = element.state;
        Q->data[Q->rear].slice = element.slice;
        Q->rear = (Q->rear + 1) % Q->maxNum;
        Q->front = (Q->front + 1) % Q->maxNum;
    }

    return 0;
}

u32 nv_debug_init(void)
{
    u32 ret;
    s32 fd;

    ret = bsp_nvm_read(NV_ID_DRV_NV_DEBUG_CFG, (u8 *)&(g_nv_debug_cfg), (u32)sizeof(NV_DEBUG_CFG_STRU));
    if (ret) {
        g_nv_debug_cfg.resume_bakup = 1;
        g_nv_debug_cfg.resume_img = 1;
        g_nv_debug_cfg.save_ddr = 0;
        g_nv_debug_cfg.save_image = 0;
        g_nv_debug_cfg.save_bakup = 0;
        g_nv_debug_cfg.reset = 0;
        g_nv_debug_cfg.product = NV_PRODUCT_PHONE;
        g_nv_debug_cfg.reserved = 0;
        nv_printf("read nv 0x%x fail,use default value! ret :0x%x\n", NV_ID_DRV_NV_DEBUG_CFG, ret);
    }

    /* init nv dump queue */
    g_nv_debug_info.write_debug_table = g_nv_write_debug_table;

    g_nv_debug_info.nv_dump_queue = (nv_queue_t *)(uintptr_t)bsp_dump_register_field(DUMP_MODEMAP_NV, "NVA",
                                                                                     NV_DUMP_SIZE, 0x0000);
    if (g_nv_debug_info.nv_dump_queue != NULL) {
        nv_debug_QueueInit(g_nv_debug_info.nv_dump_queue,
                           (u32)((NV_DUMP_SIZE - 4 * sizeof(u32)) / sizeof(nv_queue_elemnt)));
    } else {
        nv_printf("alloc dump buffer fail, field id = 0x%x\n", DUMP_MODEMAP_NV);
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    /* if or not print info when write nv */
    if (memset_s(&NV_DEBUG_CTRL_VALUE, sizeof(debug_ctrl_union_t), 0, sizeof(debug_ctrl_union_t))) {
        nv_printf("memset err\n");
    }
    ret = (u32)bsp_access(NV_DEBUG_SWICH_PATH, 0);
    if (ret) {
        nv_printf("[debug info]debug file %s not exist, not a error\n", NV_DEBUG_SWICH_PATH);
    } else {
        fd = bsp_open(NV_DEBUG_SWICH_PATH, RFILE_RDONLY, 0660);
        if (fd < 0) {
            nv_printf("open %s fail\n", NV_DEBUG_SWICH_PATH);
        } else {
            ret = (u32)bsp_read((u32)fd, (s8 *)&NV_DEBUG_CTRL_VALUE, (u32)sizeof(debug_ctrl_union_t));
            if (bsp_close((u32)fd)) {
                nv_printf("close debug file %s fail\n", NV_DEBUG_SWICH_PATH);
                return NV_OK;
            }
            if (sizeof(debug_ctrl_union_t) != ret) {
                nv_printf("read file fail, ret = 0x%x\n", ret);
                return NV_OK;
            }
        }
    }

    return NV_OK;
}

void nv_debug_set_reset(u32 reset)
{
    struct nv_global_ctrl_info_stru *nv_ctrl = nv_get_ctrl_info();
    if (reset) {
        g_nv_debug_cfg.reset = 1;
    } else {
        g_nv_debug_cfg.reset = 0;
        nv_ctrl->nv_self_ctrl.ulResumeMode = NV_MODE_FACTORY;
    }
}

#define BACK_DDR_DATA_PATH MODEM_LOG_ROOT"/drv/nv/ddr_nv.bin"
u32 nv_debug_store_ddr_data(void)
{
    u32 ret = 0;
    void *buf = NULL;
    FILE *dst_fp = NULL;
    u32 size;

    if (!nv_debug_is_save_ddr()) {
        return NV_OK;
    }

    dst_fp = mdrv_file_open(BACK_DDR_DATA_PATH, NV_FILE_WRITE);
    if (dst_fp == NULL) {
        nv_printf("open %s fail\n", BACK_DDR_DATA_PATH);
        ret = BSP_ERR_NV_OPEN_FILE_FAIL;
        goto out;
    }
    buf = (u8 *)NV_GLOBAL_CTRL_INFO_ADDR;
    size = SHM_MEM_NV_SIZE - NV_GLOBAL_INFO_SIZE;
    ret = (u32)mdrv_file_write(buf, 1, size, dst_fp);
    if (ret != size) {
        nv_printf("nv wite file error, ret = 0x%x, size = 0x%x\n", ret, size);
        ret = BSP_ERR_NV_WRITE_DATA_FAIL;
        goto out;
    }
    ret = NV_OK;
out:
    nv_printf("ret = 0x%x\n", ret);
    if (dst_fp != NULL) {
        mdrv_file_close(dst_fp);
    }
    return ret;
}

u32 nv_debug_store_file(const char *src)
{
    FILE *src_fp = NULL;
    FILE *dst_fp = NULL;
    void *buf = NULL;
    char *dst = MODEM_LOG_ROOT"/drv/nv/nv_file.bin";
    u32 ret;
    u32 len;
    const struct nv_path_info_stru *nv_path = nv_get_path_info();

    if (strncmp(src, nv_path->file_path[NV_IMG], sizeof(nv_path->file_path[NV_IMG])) == 0) {
        if (!nv_debug_is_save_image()) {
            return NV_OK;
        }
    } else if (strncmp(src, NV_BACK_PATH, sizeof(NV_BACK_PATH)) == 0) {
        if (!nv_debug_is_save_bakup()) {
            return NV_OK;
        }
    } else {
        return NV_OK;
    }

    /* open src file */
    src_fp = nv_file_open(src, NV_FILE_READ);
    if (src_fp == NULL) {
        nv_printf("open %s fail\n", src);
        return BSP_ERR_NV_OPEN_FILE_FAIL;
    }
    /* get src file len */
    len = nv_get_file_len(src_fp);
    if (len == NV_ERROR) {
        nv_file_close(src_fp);
        nv_printf("get nv len error\n");
        return len;
    }
    /* open dst file */
    dst_fp = mdrv_file_open(dst, NV_FILE_WRITE);
    if (dst_fp == NULL) {
        nv_file_close(src_fp);
        nv_printf("open %s fail\n", dst);
        return BSP_ERR_NV_OPEN_FILE_FAIL;
    }
    /* write src file info */
    ret = (u32)mdrv_file_write((void *)src, 1, (u32)strlen(src), dst_fp);
    if (ret != strlen(src)) {
        nv_printf("write file info fail\n");
    }

    /* write src file data to dst file */
    buf = vmalloc((unsigned long)len);
    if (buf == NULL) {
        nv_file_close(src_fp);
        mdrv_file_close(dst_fp);
        nv_printf("alloc buff fail ,len = 0x%x\n", len);
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    ret = (u32)nv_file_seek(src_fp, 0, SEEK_SET);
    if (ret) {
        nv_printf("seek set file fail\n");
    }

    ret = (u32)nv_file_read(buf, 1, len, src_fp);
    if (ret != len) {
        nv_file_close(src_fp);
        mdrv_file_close(dst_fp);
        vfree(buf);
        nv_printf("read file len err, ret = 0x%x len = 0x%x\n", ret, len);
        return BSP_ERR_NV_READ_FILE_FAIL;
    }
    ret = (u32)mdrv_file_write(buf, 1, len, dst_fp);
    if (ret != len) {
        nv_file_close(src_fp);
        mdrv_file_close(dst_fp);
        vfree(buf);
        nv_printf("write file len err, ret = 0x%x len = 0x%x\n", ret, len);
        return BSP_ERR_NV_WRITE_DATA_FAIL;
    }
    nv_file_close(src_fp);
    mdrv_file_close(dst_fp);
    vfree(buf);
    return NV_OK;
}
/*****************************************************************************
 函 数 名  : nv_debug_switch
 功能描述  : nv debug功能开关
*****************************************************************************/
u32 nv_debug_switch(debug_ctrl_union_t debug_switch, u32 type)
{
    s32 fd;
    u32 ret;

    switch (type) {
        case NV_DEBUG_SWITCH_TYPE_FOREVER:
            fd = bsp_open(NV_DEBUG_SWICH_PATH, RFILE_RDWR | RFILE_CREAT, 0660);
            if (fd < 0) {
                nv_printf("create %s fail\n", NV_DEBUG_SWICH_PATH);
                return NV_ERROR;
            }
            ret = (u32)bsp_write_sync((u32)fd, (const s8 *)&debug_switch, (u32)sizeof(debug_ctrl_union_t));
            if (bsp_close((u32)fd)) {
                nv_printf("close %s fail\n", NV_DEBUG_SWICH_PATH);
                return NV_ERROR;
            }
            if (sizeof(debug_ctrl_union_t) != ret) {
                nv_printf("write debug switch fail\n");
                return BSP_ERR_NV_WRITE_DATA_FAIL;
            }
            break;
        case NV_DEBUG_SWITCH_TYPE_TEMPORARY:
            NV_DEBUG_CTRL_VALUE = debug_switch;
            break;

        default:
            nv_printf("para error, type:0x%x\n", type);
    }

    return NV_OK;
}
/*****************************************************************************
 函 数 名  : nv_debug_record
 功能描述  : 对读写操作过程记录打点时间
*****************************************************************************/
void nv_debug_record(u32 current_state)
{
    nv_queue_elemnt new_record = {0};
    u32 table_index;
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;
    bool need_print = ((ddr_info->acore_init_state != NV_INIT_OK) &&
                       ((current_state == NV_DEBUG_WRITEEX_GET_IPC_START) ||
                        (current_state == NV_DEBUG_WRITEEX_GET_IPC_END) ||
                        (current_state == NV_DEBUG_WRITEEX_GIVE_IPC))) ? false : true;

    if ((nv_debug_is_print_in_write()) && (need_print)) {
        for (table_index = 0; table_index < NV_DEBUG_BUTT; table_index++) {
            if (g_nv_debug_info.write_debug_table[table_index].state == (current_state & 0xFFFF)) {
                nv_printf_info("%s\n", g_nv_debug_info.write_debug_table[table_index].info);
                break;
            }
        }
        switch (current_state & 0xFFFF) {
            case NV_DEBUG_WRITEEX_START:
            case NV_DEBUG_WRITEEX_END:
                nv_printf_info("nv id: 0x%x\n", current_state >> 16);
                break;
            case NV_DEBUG_SEND_ICC:
            case NV_DEBUG_READ_ICC:
                nv_printf_info("msg type: 0x%x\n", current_state >> 16);
                break;
            default:
                break;
        }
    }

    if (g_nv_debug_info.nv_dump_queue == NULL) {
        return;
    }
    new_record.slice = bsp_get_slice_value();
    new_record.state = current_state;

    nv_debug_QueueLoopIn(g_nv_debug_info.nv_dump_queue, new_record);

    return;
}
/*****************************************************************************
 函 数 名  : nv_debug_print_dump_queue
 功能描述  : 打印dump队列中的信息
*****************************************************************************/
void nv_debug_print_dump_queue(void)
{
    u32 queue_index;
    u32 queue_num;
    u32 state_str_index;
    u32 current_state;
    char *state_str_info = NULL;

    if (g_nv_debug_info.nv_dump_queue == NULL) {
        nv_printf("dump queue is NULL\n");
        return;
    }

    queue_num = g_nv_debug_info.nv_dump_queue->num > g_nv_debug_info.nv_dump_queue->maxNum ?
        g_nv_debug_info.nv_dump_queue->maxNum : g_nv_debug_info.nv_dump_queue->num;

    for (queue_index = g_nv_debug_info.nv_dump_queue->front; queue_index < queue_num; queue_index++) {
        current_state = g_nv_debug_info.nv_dump_queue->data[queue_index].state;

        for (state_str_index = 0; state_str_index < NV_DEBUG_BUTT; state_str_index++) {
            if (g_nv_debug_info.write_debug_table[state_str_index].state == (current_state & 0xFFFF)) {
                state_str_info = g_nv_debug_info.write_debug_table[state_str_index].info;
                break;
            }
        }
        nv_printf_info("slice:0x%x state:0x%x %s\n", g_nv_debug_info.nv_dump_queue->data[queue_index].slice,
                       g_nv_debug_info.nv_dump_queue->data[queue_index].state, state_str_info);

        switch (current_state & 0xFFFF) {
            case NV_DEBUG_WRITEEX_START:
            case NV_DEBUG_WRITEEX_END:
                nv_printf_info("nv id: 0x%x\n", current_state >> 16);
                break;
            case NV_DEBUG_SEND_ICC:
            case NV_DEBUG_READ_ICC:
                nv_printf_info("msg type: 0x%x\n", current_state >> 16);
                break;
            default:
                break;
        }
    }
    return;
}
/*****************************************************************************
 函 数 名  : nv_debug_record_delta_time
 功能描述  : 记录操作最大时间
*****************************************************************************/
void nv_debug_record_delta_time(u32 type, u32 start, u32 end)
{
    u32 delta;

    delta = get_timer_slice_delta(start, end);
    if (delta > g_nv_debug_info.delta_time[type]) {
        g_nv_debug_info.delta_time[type] = delta;
    }
    return;
}
void nv_debug_print_delta_time(void)
{
    u32 type;
    for (type = 0; type < NV_DEBUG_DELTA_BUTT; type++) {
        nv_printf_info("type 0x%x max delta time 0x%x\n", type, g_nv_debug_info.delta_time[type]);
    }
    return;
}

void nv_show_ddr_info(void)
{
    u32 i;
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;
    nv_file_info_s *file_info = (nv_file_info_s *)(NV_GLOBAL_CTRL_INFO_ADDR + ctrl_info->file_offset);
    struct nv_global_ctrl_info_stru *nv_ctrl = nv_get_ctrl_info();

    nv_printf("current slice:0x%x\n", bsp_get_slice_value());

    nv_printf("global nv ddr size      :0x%x\n", SHM_MEM_NV_SIZE);
    nv_printf("global mem buf   ddr    :0x%lx\n", ddr_info->nminfo.mem_buf_addr);
    nv_printf("global mem buf   size   :0x%x\n", ddr_info->nminfo.buf_size);

    nv_printf("acore init state    : 0x%x\n", ddr_info->acore_init_state);
    nv_printf("ccore init state    : 0x%x\n", ddr_info->ccore_init_state);
    nv_printf("mcore init state    : 0x%x\n", ddr_info->mcore_init_state);
    nv_printf("crc status          : 0x%x\n", ctrl_info->crc_mark);
    nv_printf("mid priority        : 0x%x\n", ddr_info->priority);
    nv_printf("mem file type   : 0x%x\n", ddr_info->mem_file_type);
    nv_printf("total nv size   : 0x%x\n", ddr_info->file_len);
    nv_printf("nv resume mode  : 0x%x\n", nv_ctrl->nv_self_ctrl.ulResumeMode);

    if (ddr_info->acore_init_state <= NV_INIT_FAIL) {
        nv_printf("init fail!return\n");
        return;
    }

    nv_printf("ctrl file size    : 0x%x\n", ctrl_info->ctrl_size);
    nv_printf("file num          : 0x%x\n", ctrl_info->file_num);
    nv_printf("file list off     : 0x%x\n", ctrl_info->file_offset);
    nv_printf("file list size    : 0x%x\n", ctrl_info->file_size);
    nv_printf("ctrl file magic   : 0x%x\n", ctrl_info->magicnum);
    nv_printf("modem num         : 0x%x\n", ctrl_info->modem_num);
    nv_printf("nv count          : 0x%x\n", ctrl_info->ref_count);
    nv_printf("nv ref data off   : 0x%x\n", ctrl_info->ref_offset);
    nv_printf("nv ref data size  : 0x%x\n", ctrl_info->ref_size);

    for (i = 0; i < ctrl_info->file_num; i++) {
        nv_printf("file id       : 0x%x\n", file_info->file_id);
        nv_printf("file name     : %s\n", file_info->file_name);
        nv_printf("nv num        : 0x%x\n", file_info->file_nvnum);
        nv_printf("file offset   : 0x%x\n", file_info->file_size);
        nv_printf("file size     : 0x%x\n", file_info->file_size);
        file_info++;
        nv_printf("\n");
    }
}

void nv_show_ref_info(u32 arg1, u32 arg2)
{
    u32 i;
    u32 _max;
    u32 _min;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)(uintptr_t)NV_GLOBAL_CTRL_INFO_ADDR;
    nv_item_info_s *ref_info = (nv_item_info_s *)(uintptr_t)(NV_GLOBAL_CTRL_INFO_ADDR + NV_GLOBAL_CTRL_INFO_SIZE +
                                                  NV_GLOBAL_FILE_ELEMENT_SIZE * ctrl_info->file_num);

    _max = arg2 > ctrl_info->ref_count ? ctrl_info->ref_count : arg2;
    _min = arg1 > _max ? 0 : arg1;

    _max = (arg2 == 0) ? ctrl_info->ref_count : _max;

    ref_info = ref_info + _min;

    for (i = _min; i < _max; i++) {
        nv_printf("nvid   :0x%-8x, file id : 0x%-8x\n", ref_info->itemid, ref_info->file_id);
        nv_printf("nvlen  :0x%-8x, nv_off  : 0x%-8x, nv_pri 0x%-8x\n", ref_info->nv_len, ref_info->nv_off[i],
                  ref_info->priority);
        nv_printf("dsda   :0x%-8x\n", ref_info->modem_num);
        ref_info++;
    }
}

void nv_show_fastboot_err(void)
{
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;

    nv_printf("line       :%d\n", ddr_info->fb_debug.line);
    nv_printf("result     :0x%x\n", ddr_info->fb_debug.ret);
    nv_printf("reseved1   :0x%x\n", ddr_info->fb_debug.reseverd1);
    nv_printf("reseved2   :0x%x\n", ddr_info->fb_debug.reseverd2);
}

void nv_help(u32 type)
{
    u32 i;
    if (type == 63) {
        nv_printf("1   -------  read\n");
        nv_printf("4   -------  auth read\n");
        nv_printf("5   -------  write\n");
        nv_printf("6   -------  auth write\n");
        nv_printf("8   -------  get len\n");
        nv_printf("9   -------  auth get len\n");
        nv_printf("10  -------  flush\n");
        nv_printf("12  -------  backup\n");
        nv_printf("15  -------  import\n");
        nv_printf("16  -------  export\n");
        nv_printf("19  -------  kernel init\n");
        nv_printf("20  -------  remain init\n");
        nv_printf("21  -------  nvm init\n");
        nv_printf("22  -------  xml decode\n");
        nv_printf("24  -------  revert\n");
        nv_printf("25  -------  update default\n");
        return;
    }
    if (type == NV_FUN_MAX_ID) {
        for (i = 0; i < NV_FUN_MAX_ID; i++) {
            nv_printf("fun id %d\n", i);
            nv_printf("call num             : 0x%x\n", g_nv_debug[i].callnum);
            nv_printf("out branch (reseved1): 0x%x\n", g_nv_debug[i].reseved1);
            nv_printf("reseved2             : 0x%x\n", g_nv_debug[i].reseved2);
            nv_printf("reseved3             : 0x%x\n", g_nv_debug[i].reseved3);
            nv_printf("reseved4             : 0x%x\n", g_nv_debug[i].reseved4);
        }
        return;
    }

    i = type;
    nv_printf("fun id %d\n", i);
    nv_printf("call num             : 0x%x\n", g_nv_debug[i].callnum);
    nv_printf("out branch (reseved1): 0x%x\n", g_nv_debug[i].reseved1);
    nv_printf("reseved2             : 0x%x\n", g_nv_debug[i].reseved2);
    nv_printf("reseved3             : 0x%x\n", g_nv_debug[i].reseved3);
    nv_printf("reseved4             : 0x%x\n", g_nv_debug[i].reseved4);
}
u32 nv_show_item_info(u16 itemid)
{
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;
    u32 i;
    u32 ret;
    u32 nv_phy_addr;
    u8 *mdm_base = NULL;
    nv_item_info_s *ref_info = NULL;

    ret = nv_search_iteminfo((u32)itemid, ctrl_info, &ref_info);
    if (ret) {
        return NV_ERROR;
    }
    nv_printf("itemid   = 0x%x\n", ref_info->itemid);
    nv_printf("file_id  = 0x%x\n", ref_info->file_id);
    nv_printf("priority = 0x%x\n", ref_info->priority);
    nv_printf("resume   = 0x%x\n", ref_info->resume);
    nv_printf("modem_num = 0x%x\n", ref_info->modem_num);
    nv_printf("nv len = 0x%x\n", ref_info->nv_len);

    for (i = 0; i < ref_info->modem_num; i++) {
        mdm_base = nv_get_item_mdmbase(i + 1, ref_info, ctrl_info);
        nv_phy_addr = (u32)(uintptr_t)SHD_DDR_V2P((uintptr_t)mdm_base);

        nv_printf("nv_virt_addr = 0x%x\n", (u32)(uintptr_t)mdm_base);
        nv_printf("nv_phy_addr = 0x%x\n", nv_phy_addr);
    }

    return NV_OK;
}

/* nv debug func,reseverd1 used to reg branch */
void nv_debug(u32 type, u32 reseverd1, u32 reserved2, u32 reserved3, u32 reserved4)
{
    if (reseverd1 == 0) {
        g_nv_debug[type].callnum++;
    }
    g_nv_debug[type].reseved1 = reseverd1;
    g_nv_debug[type].reseved2 = reserved2;
    g_nv_debug[type].reseved3 = reserved3;
    g_nv_debug[type].reseved4 = reserved4;
}

/* 系统启动log记录接口，保存到 NV_LOG_PATH 中，大小限定在 NV_LOG_MAX_SIZE */
void nv_record(char *fmt, ...)
{
    char buffer[NV_RECORD_BUFF_SIZE] = {0};
    va_list arglist;
    FILE *fp = NULL;
    u32 ret;
    s32 err;
    u32 file_len;
    u32 buffer_size;

    va_start(arglist, fmt);
    err = (u32)vsnprintf_s(buffer, (size_t)NV_RECORD_BUFF_SIZE, (size_t)NV_RECORD_BUFF_SIZE - 1, fmt, arglist);
    if (err < 0) {
        nv_printf("vsnprintf err");
    }
    va_end(arglist);

    nv_printf("%s", buffer);

    if (nv_check_esl_emu()) {
        return;
    }

    if (mdrv_file_access((char *)NV_LOG_PATH, 0)) {
        fp = mdrv_file_open((char *)NV_LOG_PATH, "w+");
        if (fp == NULL)
            return;
    } else {
        fp = mdrv_file_open((char *)NV_LOG_PATH, "r+");
        if (fp == NULL)
            return;
        if (mdrv_file_seek(fp, (long)0, SEEK_END)) {
            mdrv_file_close(fp);
            return;
        }
        file_len = (u32)mdrv_file_tell(fp);
        if ((file_len + strlen(buffer)) >= NV_LOG_MAX_SIZE) {
            mdrv_file_close(fp);
            fp = mdrv_file_open((char *)NV_LOG_PATH, "w+");
            if (fp == NULL)
                return;
        }
    }
    ret = (u32)mdrv_file_write(buffer, 1, (unsigned int)strlen(buffer), fp);
    if (ret != strlen(buffer)) {
        buffer_size = (u32)strlen(buffer);
        nv_printf("mdrv_file_write   nv   log err!  ret :0x%x buffer len :0x%x\n", ret, buffer_size);
    }
    mdrv_file_close(fp);
}

/*****************************************************************************
 Prototype       : nv_debug_is_bak_resume_nv
 Description     : 检测nv是否为备份恢复nv
*****************************************************************************/
void nv_debug_is_bak_resume_nv(u32 itemid)
{
    u32 ret;
    nv_item_info_s *item_info = NULL;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;

    ret = nv_search_iteminfo(itemid, ctrl_info, &item_info);
    if (ret) {
        nv_printf("can not find nvid 0x%x!\n", itemid);
        return;
    }

    if (item_info->resume == 1) {
        nv_printf("nvid 0x%x is resume nv!\n", itemid);
    } else {
        nv_printf("nvid 0x%x is not resume nv!\n", itemid);
    }
    return;
}

/*****************************************************************************
 函 数 名  : nv_check_item_len
 功能描述  : 检查nv id在start和end之间的各个nv项的长度
             并将非四字节对齐的nv id和该nv项的长度打印出来
 输入参数  : start:  nv id的起始值
             end  :  nv id的结束值
*****************************************************************************/
u32 nv_debug_check_item_len(u32 start, u32 end)
{
    u32 i;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)(uintptr_t)NV_GLOBAL_CTRL_INFO_ADDR;
    nv_item_info_s *ref_info = (nv_item_info_s *)(uintptr_t)(NV_GLOBAL_CTRL_INFO_ADDR + NV_GLOBAL_CTRL_INFO_SIZE +
        NV_GLOBAL_FILE_ELEMENT_SIZE * ctrl_info->file_num);
    u32 item_num = 0;

    for (i = 0; i < ctrl_info->ref_count; i++) {
        if (((ref_info->nv_len) % 4 != 0) && (start <= ref_info->itemid) && (end >= ref_info->itemid)) {
            item_num++;
            nv_printf("nv id = 0x%x nv len = 0x%x\n ", ref_info->itemid, ref_info->nv_len);
        }
        ref_info++;
    }

    return NV_OK;
}

/*****************************************************************************
 Prototype       : nv_debug_chk_invalid_type
 Description     : 检查文件是否是测试桩定义的文件
*****************************************************************************/
u32 nv_debug_chk_invalid_type(const s8 *path, u32 invalid_type)
{
    if (g_debug_check_flag != 0) {
        if ((strncmp((const s8 *)path, (const s8 *)g_debug_check_file, strlen((s8 *)g_debug_check_file) + 1) == 0) &&
            (g_debug_check_flag == invalid_type)) {
            return g_debug_check_flag;
        }
    }

    return 0;
}

/*****************************************************************************
 Prototype       : nv_debug_set_invalid_type
 Description     : 设置测试桩定义的文件的名字和属性
*************************************************************************/
void nv_debug_set_invalid_type(const s8 *path, u32 invalid_type)
{
    if (strncpy_s((s8 *)g_debug_check_file, sizeof(g_debug_check_file), (s8 *)path, strlen(path))) {
        nv_printf("strncpy g_debug_check_file err\n");
    }
    g_debug_check_flag = invalid_type;
    return;
}

/*****************************************************************************
 Prototype       : nv_debug_set_invalid_type
 Description     : 清除测试桩定义的文件的名字和属性
*************************************************************************/
void nv_debug_clear_invalid_type(void)
{
    if (memset_s((s8 *)g_debug_check_file, sizeof(g_debug_check_file), 0, sizeof(g_debug_check_file))) {
        nv_printf("memset err\n");
    }
    g_debug_check_flag = 0;

    nv_img_clear_check_result();
    return;
}

u32 nvm_read_rand(u32 nvid)
{
    u32 ret;
    u8 *tempdata = NULL;
    u32 i;
    nv_item_info_s *ref_info = NULL;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;

    ret = nv_search_iteminfo(nvid, ctrl_info, &ref_info);
    if (ret != NV_OK) {
        return ret;
    }
    nv_printf("[0x%x]:len 0x%x,off 0x%x,file id %d\n", nvid, ref_info->nv_len, ref_info->nv_off[0], ref_info->file_id);

    tempdata = (u8 *)nv_malloc((unsigned long)(ref_info->nv_len) + 1);
    if (tempdata == NULL) {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    if (memset_s((void *)tempdata, ((unsigned long)(ref_info->nv_len) + 1), 0,
                 ((unsigned long)(ref_info->nv_len) + 1))) {
        nv_printf("memset err\n");
    }

    ret = bsp_nvm_read(nvid, tempdata, ref_info->nv_len);
    if (ret != NV_OK) {
        nv_free(tempdata);
        return BSP_ERR_NV_READ_DATA_FAIL;
    }
    for (i = 0; i < ref_info->nv_len; i++) {
        if ((i % 32) == 0) {
            nv_printf("\n");
        }
        nv_printf("%02x ", (u8)(*(tempdata + i)));
    }
    nv_free(tempdata);
    nv_printf("\n\n");
    return 0;
}

u32 nvm_read_randex(u32 nvid, u32 modem_id)
{
    u32 ret;
    u8 *tempdata = NULL;
    u32 i;
    nv_item_info_s *ref_info = NULL;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;

    ret = nv_search_iteminfo(nvid, ctrl_info, &ref_info);
    if (ret != NV_OK) {
        return ret;
    }
    if (ref_info->nv_len == 0) {
        return NV_ERROR;
    }

    nv_printf("[0x%x]:len 0x%x,off 0x%x,file id %d\n", nvid, ref_info->nv_len, ref_info->nv_off[0], ref_info->file_id);
    nv_printf("[0x%x]:dsda 0x%x\n", nvid, ref_info->modem_num);

    tempdata = (u8 *)nv_malloc((unsigned long)(ref_info->nv_len) + 1);
    if (tempdata == NULL) {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    if (memset_s((void *)tempdata, ((unsigned long)(ref_info->nv_len) + 1), 0,
                 ((unsigned long)(ref_info->nv_len) + 1))) {
        nv_printf("memset err\n");
    }

    ret = bsp_nvm_dcread(modem_id, nvid, tempdata, ref_info->nv_len);
    if (ret != NV_OK) {
        nv_free(tempdata);
        return BSP_ERR_NV_READ_DATA_FAIL;
    }
    for (i = 0; i < ref_info->nv_len; i++) {
        if ((i % 32) == 0) {
            nv_printf("\n");
        }
        nv_printf("%02x ", (u8)(*(tempdata + i)));
    }

    nv_printf("\n\n");
    nv_free(tempdata);

    return 0;
}

void nv_show_all_nv(void)
{
    int i;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)(uintptr_t)NV_GLOBAL_CTRL_INFO_ADDR;
    nv_file_info_s *file_info = (nv_file_info_s *)(uintptr_t)(NV_GLOBAL_CTRL_INFO_ADDR + NV_GLOBAL_CTRL_INFO_SIZE);
    nv_item_info_s *item_info =
        (nv_item_info_s *)(uintptr_t)(NV_GLOBAL_CTRL_INFO_ADDR + NV_GLOBAL_CTRL_INFO_SIZE + ctrl_info->file_size);

    for (i = 0; i < ctrl_info->ref_count; i++)
        nv_printf("nv:%d len:%d file_id=%d: file_offset=%d:\n", item_info[i].itemid, item_info[i].nv_len,
                  item_info[i].file_id, file_info[item_info[i].file_id - 1].file_offset);

    return;
}

EXPORT_SYMBOL(g_nv_debug_cfg);
EXPORT_SYMBOL(nv_debug_check_item_len);
EXPORT_SYMBOL(nv_debug_store_ddr_data);
EXPORT_SYMBOL(nv_debug_print_dump_queue);
EXPORT_SYMBOL(nv_debug_print_delta_time);
EXPORT_SYMBOL(nv_debug_store_file);
EXPORT_SYMBOL(nv_debug_switch);
EXPORT_SYMBOL(nv_show_ref_info);
EXPORT_SYMBOL(nv_help);
EXPORT_SYMBOL(nv_show_fastboot_err);
EXPORT_SYMBOL(nv_show_item_info);
EXPORT_SYMBOL(nv_show_ddr_info);
EXPORT_SYMBOL(nv_debug_set_reset);
EXPORT_SYMBOL(nv_debug_is_bak_resume_nv);
EXPORT_SYMBOL(nv_debug_set_invalid_type);
EXPORT_SYMBOL(nv_debug_chk_invalid_type);
EXPORT_SYMBOL(nv_debug_clear_invalid_type);
EXPORT_SYMBOL(nvm_read_rand);
EXPORT_SYMBOL(nvm_read_randex);
