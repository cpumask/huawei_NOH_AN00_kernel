/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/list.h>
#include <linux/uio.h>
#include <linux/init.h>
#include "product_config.h"

#include "mdrv.h"
#include <osl_thread.h>
#include <bsp_slice.h>
#include <bsp_dump.h>
#include <bsp_print.h>
#include <bsp_version.h>
#include <bsp_mloader.h>
#include "rfile_balong_user.h"
#include <securec.h>


#define THIS_MODU mod_rfile
#define RFILE_APP_NAME "rfile_user"
#define RFILE_APP_NAME_PHONE "rfile_user_phone"
struct bsp_rfile_main_stru_acore g_rfile_acore_main = {
    RFILE_STATE_UNINITED,
    APP_STATE_UNINTED,
};
int g_rfile_while = 1;
bsp_fs_status_e g_fs_is_suport = BSP_FS_NOT_OK;

void rfile_icc_send(void *data, u32 len);

bsp_fs_status_e bsp_fs_ok(void)
{
    if (g_rfile_acore_main.state == RFILE_STATE_UNINITED) {
        return BSP_FS_NOT_OK;
    }
    return BSP_FS_OK;
}

/* *****************************rfile加载模式判断************************************************** */
bsp_fs_status_e bsp_fs_tell_load_mode(void)
{
    const bsp_version_info_s *version_info = bsp_get_version_info();
    if (version_info != NULL && version_info->plat_type == PLAT_ASIC) {
        return BSP_FS_OK;
    }

    return g_fs_is_suport;
}

void bsp_rfile_get_load_mode(long type)
{
    if (type == MLOADER_FS_LOAD) {
        g_fs_is_suport = BSP_FS_OK;
    }
    return ;
}

__init int rfile_get_load_mode_mbb(char *cmdline)
{
    if (cmdline == NULL) {
        bsp_err("cmdline is null.\n");
        return -EINVAL;
    }
    bsp_err("bsp_blk_load_mode :%s\n", cmdline);

    if (strcmp(cmdline, "flash") == 0) {
        g_fs_is_suport = BSP_FS_OK;
    }

    return 0;
}

#ifndef BSP_CONFIG_PHONE_TYPE
early_param("bsp_blk_load_mode", rfile_get_load_mode_mbb);
#endif
/* *****************************rfile维测************************************************** */
#define RFILE_OM_DUMP_SIZE 0xC00
#define RFILE_OM_DUMP_ID DUMP_MODEMAP_RFILE
struct bsp_rfile_dbg_info *g_rfile_dbg;

void rfile_dump_inner_state_dbg(enum bsp_rfile_state state_type)
{
    struct bsp_rfile_inner_state_dbg *debug = NULL;
    struct rfile_state_trace *trace = NULL;
    u32 timestamp = bsp_get_slice_value();
    int i;

    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }
    debug = &(g_rfile_dbg->inner_state_trace);
    i = debug->trace_index % RFILE_MNTN_INNER_STATE_CNT;
    trace = &(debug->trace[i]);
    trace->slice = timestamp;
    trace->state_type = state_type;
    debug->trace_index++;
    return;
}

void rfile_dump_app_state_dbg(enum bsp_rfile_app_state state_type)
{
    struct bsp_rfile_app_state_dbg *debug = NULL;
    struct rfile_state_trace *trace = NULL;
    u32 timestamp = bsp_get_slice_value();
    int i;

    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }
    debug = &(g_rfile_dbg->app_state_trace);
    i = debug->trace_index % RFILE_MNTN_APP_STATE_CNT;
    trace = &(debug->trace[i]);
    trace->slice = timestamp;
    trace->state_type = state_type;
    debug->trace_index++;
    return;
}

void rfile_dump_handle_dbg(u32 op_type, unsigned int queue_cnt, u32 state_type, s32 ret)
{
    struct bsp_rfile_acore_handle_dbg *debug = NULL;
    struct rfile_handle_trace *trace = NULL;
    u32 timestamp = bsp_get_slice_value();
    int i;

    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }
    debug = &(g_rfile_dbg->acore_handle_trace);
    i = debug->trace_index % RFILE_MNTN_HANDLE_CNT;
    trace = &(debug->trace[i]);
    trace->slice = timestamp;
    trace->op_type = op_type;
    trace->queue_cnt = queue_cnt;
    trace->state_type = state_type;
    trace->ret = ret;
    debug->trace_index++;
    return;
}

void rfile_dump_event_dbg(enum bsp_rfile_event_type event_name, enum bsp_rfile_event_state state_type)
{
    struct bsp_rfile_event_dbg *debug = NULL;
    struct rfile_event_trace *trace = NULL;
    u32 timestamp = bsp_get_slice_value();
    int i;

    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }
    debug = &(g_rfile_dbg->event_trace);
    i = debug->trace_index % RFILE_MNTN_EVENT_STATE_CNT;
    trace = &(debug->trace[i]);
    trace->slice = timestamp;
    trace->state_type = state_type;
    trace->event_name = event_name;
    debug->trace_index++;
    return;
}

void rfile_dump_fd_list(void)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    int i = 0;
    int ret;
    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }

    list_for_each_safe(p, n, &g_rfile_acore_main.fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (i >= RFILE_MNTN_FD_CNT) {
            bsp_err("<%s> elements in fd_list > 20,dump can only save 20\n", __FUNCTION__);
            break;
        }
        g_rfile_dbg->fd_dump.debug[i].fd = elemt->fd;
        g_rfile_dbg->fd_dump.debug[i].offset = elemt->offset;
        g_rfile_dbg->fd_dump.debug[i].path[0] = '\0';
        if (elemt->name != NULL) {
            ret = strncat_s(g_rfile_dbg->fd_dump.debug[i].path, RFILE_MNTN_PATHLEN_MAX, elemt->name,
                            RFILE_MNTN_PATHLEN_MAX - 1);
            if (ret) {
                bsp_err("<%s> strncat_s err. ret = %d.\n", __FUNCTION__, ret);
            }
        }
        i++;
    }
    return;
}

void rfile_dump_queue_list(void)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_que_stru_acore *elemt = NULL;
    int i = 0;
    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> g_rfile_dbg is NULL!\n", __FUNCTION__);
        return;
    }
    osl_sem_down(&g_rfile_acore_main.sem_queuelist);
    list_for_each_safe(p, n, &g_rfile_acore_main.queue_list)
    {
        elemt = list_entry(p, struct bsp_rfile_que_stru_acore, stlist);
        if (i >= RFILE_MNTN_QUEUE_CNT) {
            bsp_err("<%s> elements in queue_list > 20,dump can only save 20\n", __FUNCTION__);
            break;
        }
        g_rfile_dbg->queue_dump.debug[i].op_type = elemt->op_type;
        g_rfile_dbg->queue_dump.debug[i].queue_cnt = elemt->queue_cnt;
        i++;
    }
    osl_sem_up(&g_rfile_acore_main.sem_queuelist);
    return;
}

void rfile_dump_hook(void)
{
    rfile_dump_queue_list();
    rfile_dump_fd_list();
    return;
}

int rfile_dump_init(void)
{
    u32 rfile_dbg_size = sizeof(struct bsp_rfile_dbg_info);
    s32 ret;
    g_rfile_dbg = (struct bsp_rfile_dbg_info *)bsp_dump_register_field(RFILE_OM_DUMP_ID, "RFILE_ACORE",
                                                                       RFILE_OM_DUMP_SIZE, 1);
    if (g_rfile_dbg == NULL) {
        bsp_err("<%s> malloc from dump fail,malloc form ddr!", __FUNCTION__);
        g_rfile_dbg = (struct bsp_rfile_dbg_info *)kmalloc(rfile_dbg_size, GFP_KERNEL);
        if (g_rfile_dbg == NULL) {
            bsp_err("<%s> malloc from ddr fail,error!", __FUNCTION__);
            return -1;
        }
    }
    ret = memset_s(g_rfile_dbg, sizeof(struct bsp_rfile_dbg_info), 0, sizeof(struct bsp_rfile_dbg_info));
    if (ret != EOK) {
        bsp_err("<%s> memcpy_s err. ret = %d.\n", __FUNCTION__, ret);
        return -1;
    }

    (void)bsp_dump_register_hook("RFILE", rfile_dump_hook);

    g_rfile_dbg->dbg_start_byte = 0x23232323;  // special start byte for rfile dump index

    rfile_dump_inner_state_dbg(RFILE_STATE_UNINITED);
    rfile_dump_app_state_dbg(APP_STATE_UNINTED);
    return 0;
}
/* *****************************rfile维测结束************************************************** */
s32 rfile_get_fd_list(void)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    int empty_flag = 1;
    osl_sem_down(&g_rfile_acore_main.sem_fdlist);
    list_for_each_safe(p, n, &g_rfile_acore_main.fd_list)
    {
        empty_flag = 0;
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (elemt->open_type == RFILE_OPEN_FILE) {
            if (elemt->fd_outdate_flag) {
                bsp_err("<%s> opened file %s, flag 0x%x, mode 0x%x, fd_org %d, fd_new %d.\n", __FUNCTION__, elemt->name,
                        elemt->flags, elemt->mode, elemt->fd, elemt->fd_new);
            } else {
                bsp_err("<%s> opened file %s, flag 0x%x, mode 0x%x, fd %d.\n", __FUNCTION__, elemt->name, elemt->flags,
                        elemt->mode, elemt->fd);
            }
        } else {
            if (elemt->fd_outdate_flag) {
                bsp_err("<%s> opened dir %s, fd_org %d, fd_new %d.\n", __FUNCTION__, elemt->name, elemt->fd,
                        elemt->fd_new);
            } else {
                bsp_err("<%s> opened dir %s, fd %d.\n", __FUNCTION__, elemt->name, elemt->fd);
            }
        }
    }
    osl_sem_up(&g_rfile_acore_main.sem_fdlist);
    if (empty_flag) {
        bsp_err("<%s> fd_list is empty!\n", __FUNCTION__);
    }
    return 0;
}

s32 rfile_get_queue_list(void)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_que_stru_acore *elemt = NULL;
    int empty_flag = 1;
    list_for_each_safe(p, n, &g_rfile_acore_main.queue_list)
    {
        empty_flag = 0;
        elemt = list_entry(p, struct bsp_rfile_que_stru_acore, stlist);
        bsp_err("<%s> queue op_type = %d, queue_cnt = %d\n", __FUNCTION__, elemt->op_type, elemt->queue_cnt);
    }
    if (empty_flag) {
        bsp_err("<%s> queue_list is empty!\n", __FUNCTION__);
    }
    return 0;
}

int rfile_dump_show(void)
{
    u32 i;
    char *handle_info[] = {
        "OP_IN", "USER_SEND", "USER_RECV", "OP_OUT"
    };
    char *innerstate_info[] = {
        "IDLE", "DOING", "RESET", "UNINITED", "THREAD_IN"
    };
    char *appstate_info[] = {
        "UNINITED", "RELEASED", "INITING", "DONE"
    };
    char *eventstate_info[] = {
        "OFF", "ON", "RESET_BEFORE", "RESET_DONE"
    };
    char *event_name[] = {
        "SHUTDOWN", "RESET", "ICC_CB", "INIT_DONE", "HANDLE_DONE"
    };
    char *type_info[] = {
        "open",   "close",  "write", "write_sync",   "read",           "seek",     "tell",
        "remove", "mkdir",  "rmdir", "opendir",      "readdir",        "closedir", "stat",
        "access", "rename", "reset", "open_recover", "opendir_recover"
    };

    bsp_err("*************rfile queue dump*************\n");
    rfile_get_queue_list();

    bsp_err("*************rfile fd dump*************\n");
    rfile_get_fd_list();

    bsp_err("*************rfile innerstate dump*************\n");
    bsp_err("index now = %d\n", g_rfile_dbg->inner_state_trace.trace_index % RFILE_MNTN_INNER_STATE_CNT);
    for (i = 0; i < RFILE_MNTN_INNER_STATE_CNT; i++) {
        bsp_err("No.%d  timestamp:0x%x  state:%s\n", i, g_rfile_dbg->inner_state_trace.trace[i].slice,
                innerstate_info[g_rfile_dbg->inner_state_trace.trace[i].state_type]);
    }

    bsp_err("*************rfile appstate dump*************\n");
    bsp_err("index now = %d\n", g_rfile_dbg->app_state_trace.trace_index % RFILE_MNTN_APP_STATE_CNT);
    for (i = 0; i < RFILE_MNTN_APP_STATE_CNT; i++) {
        bsp_err("No.%d  timestamp:0x%x  state:%s\n", i, g_rfile_dbg->app_state_trace.trace[i].slice,
                appstate_info[g_rfile_dbg->app_state_trace.trace[i].state_type]);
    }

    bsp_err("*************rfile ophandle dump*************\n");
    bsp_err("index now = %d\n", g_rfile_dbg->acore_handle_trace.trace_index % RFILE_MNTN_HANDLE_CNT);
    for (i = 0; i < RFILE_MNTN_HANDLE_CNT; i++) {
        bsp_err("No.%d  timestamp:0x%x  op:%s  queue_cnt:%d  state:%s  ret:%d\n", i,
                g_rfile_dbg->acore_handle_trace.trace[i].slice,
                type_info[g_rfile_dbg->acore_handle_trace.trace[i].op_type],
                g_rfile_dbg->acore_handle_trace.trace[i].queue_cnt,
                handle_info[g_rfile_dbg->acore_handle_trace.trace[i].state_type],
                g_rfile_dbg->acore_handle_trace.trace[i].ret);
    }

    bsp_err("*************rfile event dump*************\n");
    bsp_err("index now = %d\n", g_rfile_dbg->event_trace.trace_index % RFILE_MNTN_EVENT_STATE_CNT);
    for (i = 0; i < RFILE_MNTN_EVENT_STATE_CNT; i++) {
        bsp_err("No.%d  timestamp:0x%x  event:%s state:%s\n", i, g_rfile_dbg->event_trace.trace[i].slice,
                event_name[g_rfile_dbg->event_trace.trace[i].event_name],
                eventstate_info[g_rfile_dbg->event_trace.trace[i].state_type]);
    }

    return 0;
}

/* *****************************rfile维测end************************************************** */
void rfile_set_innner_state(enum bsp_rfile_state state)
{
    g_rfile_acore_main.state = state;
    rfile_dump_inner_state_dbg(state);
    return;
}
void rfile_set_app_state(enum bsp_rfile_app_state state)
{
    g_rfile_acore_main.app_state = state;
    rfile_dump_app_state_dbg(state);
    return;
}

void rfile_set_event(u32 *event_type, enum bsp_rfile_event_state state, enum bsp_rfile_event_type event_name)
{
    *event_type = state;
    osl_sem_up(&g_rfile_acore_main.sem_taskmain);
    rfile_dump_event_dbg(event_name, state);
    return ;
}

void rfile_clear_event(u32 *event_type, enum bsp_rfile_event_type event_name)
{
    *event_type = EVENT_STATE_OFF;
    rfile_dump_event_dbg(event_name, EVENT_STATE_OFF);
    return ;
}

s32 rfile_find_fd_inlist(struct bsp_rfile_main_stru_acore *rfile_info, u32 *fd)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *list_elemt = NULL;
    if (rfile_info->release_cnt == 0) {
        return 0; /* 没被kill过不需要重新找fd */
    }
    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        list_elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (list_elemt->fd == *fd) {
            if (list_elemt->fd_outdate_flag) {
                *fd = list_elemt->fd_new;
            }
            osl_sem_up(&rfile_info->sem_fdlist);
            return 0;
        }
    }
    bsp_err("<%s> find fdlist to end but not found fd:%d\n", __FUNCTION__, *fd);
    osl_sem_up(&rfile_info->sem_fdlist);
    return -1;
}
s32 rfile_add_path_into_queue(struct bsp_rfile_comm_req *req_data, struct bsp_rfile_que_stru_acore *queue_data)
{
    s32 ret;
    queue_data->pathlen = req_data->datalen;
    queue_data->path = NULL;
    queue_data->path = kzalloc(queue_data->pathlen, GFP_KERNEL);
    if (queue_data->path == NULL) {
        bsp_err("<%s> kzalloc queue_data->path failed.\n", __FUNCTION__);
        return -1;
    }
    ret = memcpy_s(queue_data->path, queue_data->pathlen, req_data->data, req_data->datalen);
    if (ret) {
        bsp_err("<%s> memcpy_s err. ret = %d.\n", __FUNCTION__, ret);
        kfree(queue_data->path);
        queue_data->path = NULL;
        return -1;
    }
    return 0;

}
s32 rfile_fill_queue_data(struct bsp_rfile_main_stru_acore *rfile_info, struct bsp_rfile_comm_req *req_data,
                          struct bsp_rfile_que_stru_acore *queue_data)
{
    queue_data->op_type = req_data->op_type;
    queue_data->queue_cnt = req_data->queue_cnt;
    queue_data->data_send = req_data;
    queue_data->data_recv = NULL;
    queue_data->ret = -1;

    switch (queue_data->op_type) {
        case EN_RFILE_OP_OPEN:
        case EN_RFILE_OP_OPEN_RECOVER:
            queue_data->flags = req_data->flags;
            queue_data->mode = req_data->mode;
            if(rfile_add_path_into_queue(req_data, queue_data)) {
                return -1;
            }
            break;
        case EN_RFILE_OP_OPENDIR:
        case EN_RFILE_OP_OPENDIR_RECOVER:
        case EN_RFILE_OP_ACCESS:
        case EN_RFILE_OP_RMDIR:
        case EN_RFILE_OP_MKDIR:
        case EN_RFILE_OP_STAT:
            if(rfile_add_path_into_queue(req_data, queue_data)) {
                return -1;
            }
            break;
        case EN_RFILE_OP_CLOSE:
        case EN_RFILE_OP_CLOSEDIR:
        case EN_RFILE_OP_READ:
        case EN_RFILE_OP_WRITE:
        case EN_RFILE_OP_WRITE_SYNC:
        case EN_RFILE_OP_SEEK:
        case EN_RFILE_OP_TELL:
        case EN_RFILE_OP_READDIR:
            if (rfile_find_fd_inlist(rfile_info, &req_data->fd)) {
                bsp_err("<%s> cannot find fd in list.\n", __FUNCTION__);
                return -1;
            }
            queue_data->fd = req_data->fd;
            break;
        default:
            break;
    }
    queue_data->datalen_send = sizeof(struct bsp_rfile_comm_req) + req_data->datalen;
    return 0;
}
s32 rfile_add_queue_node(struct bsp_rfile_main_stru_acore *rfile_info, struct bsp_rfile_comm_req *req_data,
                         struct bsp_rfile_que_stru_acore *queue_data, enum bsp_rfile_add_queue_type type)
{
    s32 ret;
    rfile_clear_event(&rfile_info->event.handle_done, RFILE_EVENT_HANDLE_DONE);
    ret = rfile_fill_queue_data(rfile_info, req_data, queue_data);
    if (ret) {
        bsp_err("<%s> fail.\n", __FUNCTION__);
        return -1;
    }
    osl_sem_down(&rfile_info->sem_queuelist);
    if (type == RFILE_ADD_QUEUE_TAIL) {
        list_add_tail(&queue_data->stlist, &rfile_info->queue_list);
    } else {
        list_add(&queue_data->stlist, &rfile_info->queue_list);
    }
    osl_sem_up(&rfile_info->sem_queuelist);

    return 0;
}

void rfile_open(struct bsp_rfile_main_stru_acore *rfile_info, const char *path, s32 flags, s32 mode, s32 offset)
{
    s32 ret;
    u32 datalen;
    u32 req_len;
    struct bsp_rfile_comm_req *req_data = NULL;
    struct bsp_rfile_que_stru_acore *queue_data = NULL;

    if (path == NULL) {
        bsp_err("<%s> path null.\n", __FUNCTION__);
        return;
    }
    datalen = strlen((char *)path) + 1;
    req_len = sizeof(struct bsp_rfile_comm_req) + datalen;
    req_data = kzalloc(req_len, GFP_KERNEL);
    if (req_data == NULL) {
        bsp_err("<%s> kzalloc req_data failed.\n", __FUNCTION__);
        return;
    }
    req_data->op_type = EN_RFILE_OP_OPEN_RECOVER;
    req_data->datalen = datalen;
    req_data->mode = mode;
    req_data->flags = flags;
    req_data->offset = offset;

    ret = memcpy_s((void *)req_data->data, RFILE_NAME_MAX, path, datalen);
    if (ret != EOK) {
        bsp_err("<%s> memcpy_s err. ret = %d.\n", __FUNCTION__, ret);
        goto err;
    }
    queue_data = kzalloc(sizeof(struct bsp_rfile_que_stru_acore), GFP_KERNEL);
    if (queue_data == NULL) {
        bsp_err("<%s> kzalloc queue_data failed.\n", __FUNCTION__);
        goto err;
    }
    queue_data->mode = mode;
    queue_data->flags = flags;
    if (rfile_add_queue_node(rfile_info, req_data, queue_data, RFILE_ADD_QUEUE_HEAD)) {
        goto err;
    }
    return;
err:
    if (req_data != NULL) {
        kfree(req_data);
        req_data = NULL;
    }
    if (queue_data != NULL) {
        kfree(queue_data);
        queue_data = NULL;
    }
    return;
}

void rfile_opendir(struct bsp_rfile_main_stru_acore *rfile_info, const char *dir_name)
{
    s32 ret;
    u32 datalen;
    u32 req_len;
    struct bsp_rfile_comm_req *req_data = NULL;
    struct bsp_rfile_que_stru_acore *queue_data = NULL;

    if (dir_name == NULL) {
        bsp_err("<%s> path null.\n", __FUNCTION__);
        return;
    }
    datalen = strlen((char *)dir_name) + 1;
    req_len = sizeof(struct bsp_rfile_comm_req) + datalen;
    req_data = kzalloc(req_len, GFP_KERNEL);
    if (req_data == NULL) {
        bsp_err("<%s> kzalloc req_data failed.\n", __FUNCTION__);
        return;
    }
    req_data->op_type = EN_RFILE_OP_OPENDIR;
    req_data->datalen = datalen;

    ret = memcpy_s((void *)req_data->data, RFILE_NAME_MAX, dir_name, datalen);
    if (ret != EOK) {
        bsp_err("<%s> memcpy_s err. ret = %d.\n", __FUNCTION__, ret);
        goto err;
    }
    queue_data = kzalloc(sizeof(struct bsp_rfile_que_stru_acore), GFP_KERNEL);
    if (queue_data == NULL) {
        bsp_err("<%s> kzalloc queue_data failed.\n", __FUNCTION__);
        goto err;
    }
    if (rfile_add_queue_node(rfile_info, req_data, queue_data, RFILE_ADD_QUEUE_HEAD)) {
        goto err;
    }
    return;
err:
    if (req_data != NULL) {
        kfree(req_data);
        req_data = NULL;
    }
    if (queue_data != NULL) {
        kfree(queue_data);
        queue_data = NULL;
    }
    return;
}

void rfile_del_ccore_fd(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        list_del(&elemt->stlist);
        kfree(elemt);
        elemt = NULL;
    }
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}
void rfile_del_ccore_queue(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_que_stru_acore *elemt = NULL;
    osl_sem_down(&rfile_info->sem_queuelist);
    list_for_each_safe(p, n, &rfile_info->queue_list)
    {
        elemt = list_entry(p, struct bsp_rfile_que_stru_acore, stlist);
        if (elemt->op_type == EN_RFILE_OP_RESET) {
            continue; /* 队列消息类型是reset的话说明是本次处理，此元素不删除 */
        }
        list_del(&elemt->stlist);
        kfree(elemt);
        elemt = NULL;
    }
    osl_sem_up(&rfile_info->sem_queuelist);
    return;
}

void rfile_do_handle_reset(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct bsp_rfile_comm_req *req_data = NULL;
    struct bsp_rfile_que_stru_acore *queue_data = NULL;
    if (rfile_info->app_state != APP_STATE_DONE) {
        rfile_del_ccore_fd(rfile_info);
        rfile_del_ccore_queue(rfile_info);
        osl_sem_up(&rfile_info->sem_reset);
        return;
    }
    req_data = kzalloc(sizeof(struct bsp_rfile_comm_req), GFP_KERNEL);
    if (req_data == NULL) {
        bsp_err("<%s> kzalloc req_data failed.\n", __FUNCTION__);
        return;
    }
    queue_data = kzalloc(sizeof(struct bsp_rfile_que_stru_acore), GFP_KERNEL);
    if (queue_data == NULL) {
        bsp_err("<%s> kzalloc queue_data failed.\n", __FUNCTION__);
        goto err;
    }

    req_data->op_type = EN_RFILE_OP_RESET;
    if (rfile_add_queue_node(rfile_info, req_data, queue_data, RFILE_ADD_QUEUE_HEAD)) {
        goto err;
    }
    osl_sem_up(&rfile_info->sem_taskpass);
    return;
err:
    if (req_data != NULL) {
        kfree(req_data);
        req_data = NULL;
    }
    if (queue_data != NULL) {
        kfree(queue_data);
        queue_data = NULL;
    }
    return;
}

u32 rfile_get_queue_cnt(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    u32 queue_cnt = 0;
    osl_sem_down(&rfile_info->sem_queuelist);
    list_for_each_safe(p, n, &rfile_info->queue_list)
    {
        queue_cnt++;
    }
    osl_sem_up(&rfile_info->sem_queuelist);

    return queue_cnt;
}

void rfile_handle_check_load_mode(struct bsp_rfile_comm_req *req_data)
{
    struct bsp_rfile_comm_cnf cnf;
    cnf.op_type = req_data->op_type;
    cnf.pstlist = req_data->pstlist;
    cnf.queue_cnt = req_data->queue_cnt;
    cnf.ret = (s32)g_fs_is_suport;
    bsp_err("<%s> ccore check load mode, return %d.\n", __FUNCTION__, g_fs_is_suport);
    rfile_icc_send(&cnf, sizeof(cnf));
    return ;
}

struct bsp_rfile_comm_req *rfile_read_icc(struct bsp_rfile_main_stru_acore *rfile_info)
{
    s32 ret;
    u32 queue_cnt = 0;
    struct bsp_rfile_comm_req *req_data = NULL;

    queue_cnt = rfile_get_queue_cnt(rfile_info);
    if (queue_cnt >= RFILE_QUEUE_STACK) {
        bsp_info("<%s> queue_cnt(%d) is over %d.\n", __FUNCTION__, queue_cnt, RFILE_QUEUE_STACK);
        msleep(1);
        osl_sem_up(&rfile_info->sem_taskmain); /* 队列满了就等一会再读icc，不然buffer不够用 */
        return NULL;
    }

    rfile_info->queue_curr = rfile_info->queue_curr >= RFILE_QUEUE_STACK? 0 : rfile_info->queue_curr;
    req_data = (struct bsp_rfile_comm_req *)rfile_info->data_recv_icc[rfile_info->queue_curr];
    ret = rfile_icc_common_recv((void *)req_data, RFILE_LEN_MAX);
    if (ret == 0) {
        return NULL;
    }
    if (((u32)ret > RFILE_LEN_MAX) || (ret < 0)) {
        bsp_err("<%s> icc_read failed, len = %d.\n", __FUNCTION__, ret);
        return NULL;
    }
    if (req_data->op_type >= EN_RFILE_OP_BUTT) {
        bsp_err("<%s> op_type %d is bigger than EN_RFILE_OP_BUTT.\n", __FUNCTION__, req_data->op_type);
        osl_sem_up(&rfile_info->sem_taskmain); /* 当前包异常，继续读icc下一包 */
        return NULL;
    }
    if (req_data->datalen > RFILE_LEN_MAX) {
        bsp_err("<%s> req_data->datalen %d err.\n", __FUNCTION__, req_data->datalen);
        osl_sem_up(&rfile_info->sem_taskmain); /* 当前包异常，继续读icc下一包 */
        return NULL;
    }
    if (req_data->op_type == EN_RFILE_OP_CHECK_LOAD_MODE) {
        rfile_handle_check_load_mode(req_data);
        osl_sem_up(&rfile_info->sem_taskmain); /* 如果是检查启动模式，直接返回，不参与用户态处理，继续读icc下一包 */
        return NULL;
    }
    rfile_info->queue_curr++;

    return (struct bsp_rfile_comm_req *)req_data;
}

void rfile_do_read_icc(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct bsp_rfile_comm_req *req_data = NULL;
    struct bsp_rfile_que_stru_acore *queue_data = NULL;
    if (rfile_info->app_state != APP_STATE_DONE) {
        msleep(10);
        osl_sem_up(&rfile_info->sem_taskmain); /* 用户态被kill中先不读icc */
        return;
    }
    req_data = rfile_read_icc(rfile_info);
    if(req_data == NULL) {
        return;
    }
    rfile_dump_handle_dbg(req_data->op_type, req_data->queue_cnt, RFILE_ACORE_OP_IN, 0);

    queue_data = kzalloc(sizeof(struct bsp_rfile_que_stru_acore), GFP_KERNEL);
    if (queue_data == NULL) {
        bsp_err("<%s> kzalloc queue_data failed.\n", __FUNCTION__);
        return;
    }

    if (rfile_add_queue_node(rfile_info, req_data, queue_data, RFILE_ADD_QUEUE_TAIL)) {
        kfree(queue_data);
        queue_data = NULL;
        return;
    }

    osl_sem_up(&rfile_info->sem_taskpass);
    osl_sem_up(&rfile_info->sem_taskmain); /* 处理结束后避免ICC通道中有缓存，再次启动读取 */
    return;
}

s32 rfile_process_thread(void *data)
{
    struct bsp_rfile_main_stru_acore *rfile_info = (struct bsp_rfile_main_stru_acore *)data;
    bsp_info("<%s> entry.\n", __FUNCTION__);

    while (g_rfile_while) {
        rfile_dump_inner_state_dbg(RFILE_THREAD_IN);
        osl_sem_down(&rfile_info->sem_taskmain);
        rfile_dump_inner_state_dbg(rfile_info->state);
        switch (rfile_info->state) {
            case RFILE_STATE_UNINITED:
                if (rfile_info->event.init_done == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.init_done, RFILE_EVENT_INIT_DONE);
                    rfile_set_innner_state(RFILE_STATE_IDLE);
                }
                break;
            case RFILE_STATE_IDLE:
                if (rfile_info->event.shutdown == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.shutdown, RFILE_EVENT_SHUTDOWN);
                    rfile_set_innner_state(RFILE_STATE_UNINITED);
                } else if (rfile_info->event.reset == EVENT_STATE_RESET_BEFORE) {
                    rfile_clear_event(&rfile_info->event.reset, RFILE_EVENT_RESET);
                    rfile_set_innner_state(RFILE_STATE_RESET);
                    rfile_do_handle_reset(rfile_info);
                } else if (rfile_info->event.icc_cb == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.icc_cb, RFILE_EVENT_ICC_CB);
                    rfile_set_innner_state(RFILE_STATE_DOING);
                    rfile_do_read_icc(rfile_info);
                } else {
                    rfile_do_read_icc(rfile_info);
                }
                break;
            case RFILE_STATE_DOING:
                if (rfile_info->event.shutdown == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.shutdown, RFILE_EVENT_SHUTDOWN);
                    rfile_set_innner_state(RFILE_STATE_UNINITED);
                } else if (rfile_info->event.reset == EVENT_STATE_RESET_BEFORE) {
                    rfile_clear_event(&rfile_info->event.reset, RFILE_EVENT_RESET);
                    rfile_set_innner_state(RFILE_STATE_RESET);
                    rfile_do_handle_reset(rfile_info);
                } else if (rfile_info->event.icc_cb == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.icc_cb, RFILE_EVENT_ICC_CB);
                    rfile_do_read_icc(rfile_info);
                } else if (rfile_info->event.handle_done == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.handle_done, RFILE_EVENT_HANDLE_DONE);
                    rfile_set_innner_state(RFILE_STATE_IDLE);
                } else {
                    rfile_do_read_icc(rfile_info);
                }
                break;
            case RFILE_STATE_RESET:
                if (rfile_info->event.shutdown == EVENT_STATE_ON) {
                    rfile_clear_event(&rfile_info->event.shutdown, RFILE_EVENT_SHUTDOWN);
                    rfile_set_innner_state(RFILE_STATE_UNINITED);
                } else if (rfile_info->event.reset == EVENT_STATE_RESET_DONE) {
                    rfile_clear_event(&rfile_info->event.reset, RFILE_EVENT_RESET);
                    rfile_set_innner_state(RFILE_STATE_DOING);
                    rfile_do_read_icc(rfile_info);
                }
                break;
            default:
                bsp_err("<%s> rfile state(%d) err.\n", __FUNCTION__, rfile_info->state);
        }
    }
    return 0;
}

void rfile_icc_send(void *data, u32 len)
{
    s32 ret, i;
    for (i = 0; i < RFILE_MAX_SEND_TIMES; i++) {
        ret = rfile_icc_common_send(data, len);
        if (ret == RFILE_CHANNAL_FULL) {
            /* buffer满，延时后重发 */
            bsp_info("<%s> icc buffer is full, send again.\n", __FUNCTION__);
            msleep(50);
            continue;
        } else if (ret == BSP_ERR_ICC_CCORE_RESETTING) {
            bsp_err("<%s> icc cannot use.\n", __FUNCTION__);
        } else if (len != (u32)ret) {
            bsp_err("<%s> icc_send failed, ret = 0x%x, len = 0x%x.\n", __FUNCTION__, (u32)ret, len);
            return;
        } else {
            return;
        }
    }
    return;
}

void rfile_icc_common_cb(void)
{
    if (g_rfile_acore_main.state != RFILE_STATE_IDLE && g_rfile_acore_main.state != RFILE_STATE_DOING) {
        /* 如果rfile未初始化则利用icc的缓存机制保存数据 */
        bsp_wrn("<%s> not responce, rfile state %d.\n", __FUNCTION__, g_rfile_acore_main.state);
        return ;
    }
    rfile_set_event(&g_rfile_acore_main.event.icc_cb, EVENT_STATE_ON, RFILE_EVENT_ICC_CB);
    return ;
}

void rfile_add_fd_node(struct bsp_rfile_main_stru_acore *rfile_info, enum bsp_rfile_open_type open_type)
{
    int ret;
    struct bsp_rfile_fd_list *elemt = NULL;
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    struct bsp_rfile_comm_cnf *data_recv = rfile_info->queue_data_now->data_recv;
    elemt = kzalloc(sizeof(struct bsp_rfile_fd_list), GFP_KERNEL);
    if (elemt == NULL) {
        bsp_err("<%s> kzalloc req_data failed.\n", __FUNCTION__);
        return;
    }
    elemt->fd = data_recv->ret;
    elemt->open_type = open_type;
    elemt->offset = 0;
    elemt->fd_outdate_flag = 0;
    if (open_type == RFILE_OPEN_FILE) {
        elemt->mode = queue_data_now->mode;
        elemt->flags = queue_data_now->flags;
    }
    elemt->name[0] = '\0';
    ret = strncat_s(elemt->name, sizeof(elemt->name), queue_data_now->path, (unsigned long)(queue_data_now->pathlen));
    if (ret != EOK) {
        bsp_err("<%s> strncat_s err. ret =  %d.\n", __FUNCTION__, ret);
        kfree(elemt);
        return;
    }
    osl_sem_down(&rfile_info->sem_fdlist);
    list_add(&elemt->stlist, &rfile_info->fd_list);
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}
void rfile_replace_fd_node(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    struct bsp_rfile_comm_cnf *data_recv = rfile_info->queue_data_now->data_recv;

    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (!strcmp(elemt->name, queue_data_now->path)) {
            elemt->fd_new = data_recv->ret;
            osl_sem_up(&rfile_info->sem_fdlist);
            return;
        }
    }
    bsp_err("<%s> find fdlist to end, but not found path %s\n", __FUNCTION__, queue_data_now->path);
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}

void rfile_del_fd_node(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (elemt->fd == queue_data_now->fd) {
            list_del(&elemt->stlist);
            kfree(elemt);
            elemt = NULL;
            osl_sem_up(&rfile_info->sem_fdlist);
            return;
        }
    }
    bsp_err("<%s> find fdlist to end, but not found fd %d\n", __FUNCTION__, queue_data_now->fd);
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}

void rfile_offset_manage(struct bsp_rfile_main_stru_acore *rfile_info, enum bsp_rfile_offset_manage_type offset_type)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    struct bsp_rfile_comm_cnf *data_recv = rfile_info->queue_data_now->data_recv;

    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        if (elemt->fd == queue_data_now->fd) {
            if (offset_type == RFILE_MANAGE_READ_WRITE) {
                elemt->offset += data_recv->ret;
            } else {
                elemt->offset = data_recv->ret;
            }
            osl_sem_up(&rfile_info->sem_fdlist);
            return;
        }
    }
    bsp_err("<%s> find fd_list to end but not found, fd = %d.\n", __FUNCTION__, queue_data_now->fd);
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}
void rfile_fdlist_manage(struct bsp_rfile_main_stru_acore *rfile_info)
{
    switch (rfile_info->queue_data_now->op_type) {
        case EN_RFILE_OP_OPEN:
            rfile_add_fd_node(rfile_info, RFILE_OPEN_FILE);
            break;
        case EN_RFILE_OP_OPENDIR:
            rfile_add_fd_node(rfile_info, RFILE_OPEN_DIR);
            break;
        case EN_RFILE_OP_OPEN_RECOVER:
        case EN_RFILE_OP_OPENDIR_RECOVER:
            rfile_replace_fd_node(rfile_info);
            break;
        case EN_RFILE_OP_READ:
        case EN_RFILE_OP_WRITE:
        case EN_RFILE_OP_WRITE_SYNC:
            rfile_offset_manage(rfile_info, RFILE_MANAGE_READ_WRITE);
            break;
        case EN_RFILE_OP_SEEK:
            rfile_offset_manage(rfile_info, RFILE_MANAGE_SEEK);
            break;
        case EN_RFILE_OP_CLOSE:
        case EN_RFILE_OP_CLOSEDIR:
            rfile_del_fd_node(rfile_info);
            break;
        default:
            break;
    }

    return;
}
void rfile_pass_handle_request(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    struct bsp_rfile_comm_cnf *data_recv = NULL;

    osl_sem_up(&rfile_info->sem_read);
    if (osl_sem_downtimeout(&rfile_info->sem_write, msecs_to_jiffies(RFILE_DOWNTIME))) {
        bsp_err("<%s> request handle timeout.\n", __FUNCTION__);
        return;
    }
    data_recv = (struct bsp_rfile_comm_cnf *)queue_data_now->data_recv;
    if (data_recv == NULL) {
        bsp_err("<%s> request handle failed.\n", __FUNCTION__);
        return;
    }
    /* 只有当前操作处理成功后才对fd信息进行维护 */
    if (data_recv->ret >= 0) {
        rfile_fdlist_manage(rfile_info);
    } else {
        if (rfile_info->queue_data_now->path != NULL) {
            if (data_recv->ret == -ENOENT) {
                bsp_err("op:%d, no such file, queue_cnt = %d, path = %s\n",
                    queue_data_now->op_type, data_recv->queue_cnt, rfile_info->queue_data_now->path);
            } else {
                bsp_err("op:%d handle failed, ret = %d, queue_cnt = %d, path = %s\n",
                    queue_data_now->op_type, data_recv->ret, data_recv->queue_cnt, rfile_info->queue_data_now->path);
            }
        } else {
            bsp_err("op:%d handle failed, ret = %d, queue_cnt = %d\n",
                queue_data_now->op_type, data_recv->ret, data_recv->queue_cnt);
        }
    }
    if (queue_data_now->op_type != EN_RFILE_OP_OPEN_RECOVER && queue_data_now->op_type != EN_RFILE_OP_OPENDIR_RECOVER) {
        rfile_icc_send(queue_data_now->data_recv, queue_data_now->datalen_recv);
    }

    rfile_dump_handle_dbg(data_recv->op_type, data_recv->queue_cnt, RFILE_ACORE_OP_OUT, data_recv->ret);
    return;
}

void rfile_pass_handle_reset(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct bsp_rfile_que_stru_acore *queue_data_now = rfile_info->queue_data_now;
    struct bsp_rfile_comm_cnf *data_recv = NULL;

    osl_sem_up(&rfile_info->sem_read);
    if (osl_sem_downtimeout(&rfile_info->sem_write, msecs_to_jiffies(RFILE_DOWNTIME))) {
        bsp_err("<%s> reset handle timeout.\n", __FUNCTION__);
        return;
    }
    if (queue_data_now->data_send != NULL) {
        kfree(queue_data_now->data_send);
        queue_data_now->data_send = NULL;
    }
    data_recv = (struct bsp_rfile_comm_cnf *)queue_data_now->data_recv;
    if (data_recv == NULL) {
        bsp_err("<%s> reset handle failed.\n", __FUNCTION__);
        return;
    }
    if (data_recv->ret < 0) {
        bsp_err("<%s> reset handle failed.\n", __FUNCTION__);
        return;
    }
    rfile_del_ccore_fd(rfile_info);
    rfile_del_ccore_queue(rfile_info);
    osl_sem_up(&rfile_info->sem_reset);
    return;
}

void rfile_del_queue_node(struct bsp_rfile_main_stru_acore *rfile_info)
{
    osl_sem_down(&rfile_info->sem_queuelist);
    list_del(&rfile_info->queue_data_now->stlist);
    if (rfile_info->queue_data_now->path != NULL) {
        kfree(rfile_info->queue_data_now->path);
        rfile_info->queue_data_now->path = NULL;
    }

    kfree(rfile_info->queue_data_now);
    rfile_info->queue_data_now = NULL;
    if (list_empty_careful(&rfile_info->queue_list)) {
        rfile_set_event(&g_rfile_acore_main.event.handle_done, EVENT_STATE_ON, RFILE_EVENT_HANDLE_DONE);
    }
    osl_sem_up(&rfile_info->sem_queuelist);
    return;
}

int rfile_get_queue_node(struct bsp_rfile_main_stru_acore *rfile_info)
{
    osl_sem_down(&rfile_info->sem_queuelist);
    if (list_empty_careful(&rfile_info->queue_list)) {
        bsp_err("<%s> queue is empty.\n", __FUNCTION__);
        osl_sem_up(&rfile_info->sem_queuelist);
        return -1;
    }
    rfile_info->queue_data_now = list_entry(rfile_info->queue_list.next, struct bsp_rfile_que_stru_acore, stlist);
    osl_sem_up(&rfile_info->sem_queuelist);
    return 0;
}

s32 rfile_pass_thread(void *data)
{
    struct bsp_rfile_main_stru_acore *rfile_info = (struct bsp_rfile_main_stru_acore *)data;
    bsp_info("<%s> entry.\n", __FUNCTION__);
    while (g_rfile_while) {
        osl_sem_down(&rfile_info->sem_taskpass);
        if (rfile_info->app_state != APP_STATE_DONE) {
            msleep(10);
            osl_sem_up(&rfile_info->sem_taskpass);
            continue;
        }
        if (rfile_get_queue_node(rfile_info)) {
            continue;
        }
        __pm_stay_awake(&rfile_info->wake_lock);

        if (rfile_info->queue_data_now->op_type == EN_RFILE_OP_RESET) {
            rfile_pass_handle_reset(rfile_info);
        } else {
            rfile_pass_handle_request(rfile_info);
        }
        __pm_relax(&rfile_info->wake_lock);
        rfile_del_queue_node(rfile_info);
    }
    return 0;
}

s32 bsp_rfile_reset_cb(drv_reset_cb_moment_e eparam, s32 userdata)
{
    if (eparam == MDRV_RESET_CB_BEFORE) {
        if (g_rfile_acore_main.state == RFILE_STATE_IDLE || g_rfile_acore_main.state == RFILE_STATE_DOING) {
            rfile_set_event(&g_rfile_acore_main.event.reset, EVENT_STATE_RESET_BEFORE, RFILE_EVENT_RESET);
            rfile_dump_handle_dbg(EN_RFILE_OP_RESET, 0, RFILE_ACORE_OP_IN, 0);

            if (osl_sem_downtimeout(&g_rfile_acore_main.sem_reset, msecs_to_jiffies(RFILE_DOWNTIME))) {
                bsp_err("<%s> reset handle timeout.\n", __FUNCTION__);
                rfile_set_event(&g_rfile_acore_main.event.reset, EVENT_STATE_RESET_DONE, RFILE_EVENT_RESET);
                rfile_dump_handle_dbg(EN_RFILE_OP_RESET, 0, RFILE_ACORE_OP_OUT, -1);
                return -1;
            }
            rfile_dump_handle_dbg(EN_RFILE_OP_RESET, 0, RFILE_ACORE_OP_OUT, 0);
        }
    } else if (eparam == MDRV_RESET_RESETTING) {
        if (g_rfile_acore_main.state == RFILE_STATE_RESET) {
            rfile_set_event(&g_rfile_acore_main.event.reset, EVENT_STATE_RESET_DONE, RFILE_EVENT_RESET);
        }
    }
    bsp_info("<%s> success!!!!!.\n", __FUNCTION__);
    (void)userdata;
    return 0;
}

void rfile_user_recover(struct bsp_rfile_main_stru_acore *rfile_info)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    struct bsp_rfile_fd_list *elemt = NULL;
    if (rfile_info->state == RFILE_STATE_RESET || rfile_info->state == RFILE_STATE_UNINITED) {
        return;
    }
    osl_sem_down(&rfile_info->sem_fdlist);
    list_for_each_safe(p, n, &rfile_info->fd_list)
    {
        elemt = list_entry(p, struct bsp_rfile_fd_list, stlist);
        elemt->fd_outdate_flag = 1;
        if (elemt->open_type == RFILE_OPEN_FILE) {
            rfile_open(rfile_info, (char *)elemt->name, elemt->flags, elemt->mode, elemt->offset);
        } else {
            rfile_opendir(rfile_info, (char *)elemt->name);
        }
        osl_sem_up(&rfile_info->sem_taskpass);
    }
    osl_sem_up(&rfile_info->sem_fdlist);
    return;
}

int rfile_dev_open(struct inode *inode, struct file *file)
{
    if (strcmp(current->comm, RFILE_APP_NAME) && strncmp(current->comm, RFILE_APP_NAME_PHONE, strlen(current->comm))) {
        bsp_err("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }
    switch (g_rfile_acore_main.app_state) {
        case APP_STATE_UNINTED:
            rfile_set_app_state(APP_STATE_DONE);
            bsp_err("<%s> open dev/rfile.\n", __FUNCTION__);
            break;
        case APP_STATE_DONE:
        case APP_STATE_REALEASED:
            rfile_set_app_state(APP_STATE_INTING);
            bsp_err("<%s> rfile_user app is restarted\n", __FUNCTION__);
            if (g_rfile_acore_main.state == RFILE_STATE_DOING || g_rfile_acore_main.state == RFILE_STATE_IDLE) {
                bsp_err("<%s> need recover.\n", __FUNCTION__);
                rfile_user_recover(&g_rfile_acore_main);
            }
            rfile_set_app_state(APP_STATE_DONE);
            break;
        default:
            bsp_err("<%s> rfile_user app is being recovered.\n", __FUNCTION__);
            break;
    }

    (void)inode;
    (void)file;
    return 0;
}

ssize_t rfile_dev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    struct bsp_rfile_que_stru_acore *queue_data_now = NULL;
    struct bsp_rfile_comm_req *req_data = NULL;
    if (strcmp(current->comm, RFILE_APP_NAME) && strncmp(current->comm, RFILE_APP_NAME_PHONE, strlen(current->comm))) {
        bsp_err("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }
    if (buf == NULL) {
        bsp_err("<%s> usr buf is null.\n", __FUNCTION__);
        return -1;
    }
    ret = down_interruptible(&g_rfile_acore_main.sem_read);
    if (ret) {
        bsp_info("<%s>read wakeup by softirq, ret = %d.\n", __FUNCTION__, ret);
        return -1;
    }
    queue_data_now = g_rfile_acore_main.queue_data_now;
    if (queue_data_now == NULL) {
        bsp_err("<%s> queue_data_now is null.\n", __FUNCTION__);
        return -1;
    }
    if (queue_data_now->data_send == NULL) {
        bsp_err("<%s> data_send is null.\n", __FUNCTION__);
        return -1;
    }
    if (count < queue_data_now->datalen_send) {
        bsp_err("<%s> usr count is smaller, kernel datasize is 0x%x, user count is 0x%x\n",
                __FUNCTION__, queue_data_now->datalen_send, (unsigned int)count);
        return -1;
    }

    ret = copy_to_user(buf, queue_data_now->data_send, queue_data_now->datalen_send);
    if (ret) {
        bsp_err("<%s> copy to user fail, ret = %d\n", __FUNCTION__, ret);
        return -1;
    }
    req_data = queue_data_now->data_send;
    rfile_dump_handle_dbg(req_data->op_type, req_data->queue_cnt, RFILE_ACORE_USER_SEND, 0);

    return queue_data_now->datalen_send;
}

ssize_t rfile_dev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    struct bsp_rfile_comm_cnf *data_recv = NULL;
    struct bsp_rfile_que_stru_acore *queue_data_now = g_rfile_acore_main.queue_data_now;
    if (queue_data_now == NULL) {
        bsp_err("<%s> queue_data_now is null.\n", __FUNCTION__);
        return -1;
    }
    queue_data_now->data_recv = NULL;
    if (strcmp(current->comm, RFILE_APP_NAME) && strncmp(current->comm, RFILE_APP_NAME_PHONE, strlen(current->comm))) {
        bsp_err("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }
    if (buf == NULL) {
        bsp_err("<%s> usr buf is null.\n", __FUNCTION__);
        return -1;
    }
    if (g_rfile_acore_main.data_recv_user == NULL) {
        bsp_err("<%s> data_recv buf is null.\n", __FUNCTION__);
        return -1;
    }
    if (count > RFILE_LEN_MAX || count < sizeof(struct bsp_rfile_comm_cnf)) {
        bsp_err("<%s> usr count err, count is 0x%x\n", __FUNCTION__, (unsigned int)count);
        return -1;
    }

    ret = copy_from_user(g_rfile_acore_main.data_recv_user, buf, count);
    if (ret) {
        bsp_err("<%s> copy from user fail, ret = %d\n", __FUNCTION__, ret);
        return -1;
    }
    /* 如果在pass_thread中down等待超时，当前队列request包会返回失败，但是如果之后又有写节点返回，而且和当前处理的消息对不上，则不处理 */
    data_recv = g_rfile_acore_main.data_recv_user;
    rfile_dump_handle_dbg(data_recv->op_type, data_recv->queue_cnt, RFILE_ACORE_USER_RECV, data_recv->ret);
    if (data_recv->queue_cnt != queue_data_now->queue_cnt) {
        bsp_err("<%s> queue_cnt diff! queue queue_cnt = %d, recv queue_cnt = %d\n", __FUNCTION__, queue_data_now->queue_cnt,
                data_recv->queue_cnt);
        return 0; /* queue_cnt对不上则不处理本次返回消息，可能是之前超时的请求的返回数据 */
    }
    queue_data_now->data_recv = g_rfile_acore_main.data_recv_user;
    queue_data_now->datalen_recv = count;

    osl_sem_up(&g_rfile_acore_main.sem_write);

    return count;
}

int rfile_dev_release(struct inode *inode, struct file *file)
{
    if (strcmp(current->comm, RFILE_APP_NAME) && strncmp(current->comm, RFILE_APP_NAME_PHONE, strlen(current->comm))) {
        bsp_err("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }

    if (g_rfile_acore_main.state == RFILE_STATE_RESET) {
        rfile_del_ccore_fd(&g_rfile_acore_main);
        rfile_del_ccore_queue(&g_rfile_acore_main);
        osl_sem_up(&g_rfile_acore_main.sem_reset);
        bsp_err("<%s> don't need to recover\n", __FUNCTION__);
    }
    rfile_set_app_state(APP_STATE_REALEASED);
    g_rfile_acore_main.release_cnt++;
    bsp_info("<%s> !!!!!!!!\n", __FUNCTION__);
    return 0;
}

static struct file_operations g_rfile_dev_fops = {
    .open = rfile_dev_open,
    .owner = THIS_MODULE,
    .read = rfile_dev_read,
    .write = rfile_dev_write,
    .release = rfile_dev_release,
};

struct device *rfile_dev_setup(struct bsp_rfile_dev_info *dev)
{
    int ret;
    char dev_name[10];
    char dev_class[20];
    struct file_operations *fops = NULL;
    struct cdev *cdev = &dev->cdev;

    fops = &g_rfile_dev_fops;
    ret = strcpy_s((char *)dev_name, sizeof(dev_name), RFILE_DEV_NAME);
    if (ret) {
        bsp_err("<%s> line %d strcpy_s err. ret = %d.\n", __FUNCTION__, __LINE__, ret);
        return NULL;
    }
    ret = strcpy_s((char *)dev_class, sizeof(dev_class), RFILE_DEV_CLASS);
    if (ret) {
        bsp_err("<%s> line %d strcpy_s err. ret = %d.\n", __FUNCTION__, __LINE__, ret);
        return NULL;
    }

    ret = alloc_chrdev_region(&dev->dev_no, 0, RFILE_DEV_NUM, dev_name);
    if (ret) {
        bsp_err("<%s> alloc_chrdev_region failed, dev_name = %s, ret = %d.\n", __FUNCTION__, dev_name, ret);
    }
    cdev_init(cdev, fops);
    cdev->owner = THIS_MODULE;
    cdev->ops = fops;
    ret = cdev_add(cdev, dev->dev_no, RFILE_DEV_NUM);
    if (ret) {
        bsp_err("<%s> cdev_add failed, dev_name = %s, ret = %d.\n", __FUNCTION__, dev_name, ret);
        return NULL;
    }
    dev->rfile_class = class_create(THIS_MODULE, (char *)dev_class);
    if (IS_ERR(dev->rfile_class)) {
        bsp_err("<%s> class_create failed, class_name = %s.\n", __FUNCTION__, dev_class);
        cdev_del(cdev);
        return NULL;
    }
    return device_create(dev->rfile_class, NULL, dev->dev_no, NULL, (char *)dev_name);
}
void rfile_init_prepare(void)
{
    osl_sem_init(0, &(g_rfile_acore_main.sem_taskmain));
    osl_sem_init(0, &(g_rfile_acore_main.sem_taskpass));
    osl_sem_init(0, &(g_rfile_acore_main.sem_read));
    osl_sem_init(0, &(g_rfile_acore_main.sem_write));
    osl_sem_init(0, &(g_rfile_acore_main.sem_reset));
    osl_sem_init(1, &(g_rfile_acore_main.sem_queuelist));
    osl_sem_init(1, &(g_rfile_acore_main.sem_fdlist));
    wakeup_source_init(&g_rfile_acore_main.wake_lock, "rfile_wakelock");

    INIT_LIST_HEAD(&g_rfile_acore_main.fd_list);
    INIT_LIST_HEAD(&g_rfile_acore_main.queue_list);

    if (rfile_dump_init()) {
        bsp_err("[init]: <%s> dump init failed.\n", __FUNCTION__);
    }
    return;
}

s32 rfile_icc_buffer_init(void)
{
    int i;
    g_rfile_acore_main.data_recv_icc[0] = vmalloc(RFILE_LEN_MAX * RFILE_QUEUE_STACK);
    if (g_rfile_acore_main.data_recv_icc[0] == NULL) {
        bsp_err("<%s> vmalloc failed.\n", __FUNCTION__);
        return -1;
    }
    (void)memset_s(g_rfile_acore_main.data_recv_icc[0], RFILE_LEN_MAX * RFILE_QUEUE_STACK, 0, RFILE_LEN_MAX * RFILE_QUEUE_STACK);
    for (i = 1; i < RFILE_QUEUE_STACK; i++) {
        g_rfile_acore_main.data_recv_icc[i] = (char *)g_rfile_acore_main.data_recv_icc[i - 1] + RFILE_LEN_MAX;
    }
    g_rfile_acore_main.queue_curr = 0;
    return 0;
}

s32 bsp_rfile_init(void)
{
    struct sched_param sch_para;
    sch_para.sched_priority = 15;

    bsp_err("[init]start.\n");
    rfile_init_prepare();

    g_rfile_acore_main.taskid_process = kthread_run(rfile_process_thread, &g_rfile_acore_main, "rfile_process");
    if (IS_ERR(g_rfile_acore_main.taskid_process)) {
        bsp_err("[init]: <%s> kthread_run rfile_process failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }
    if (sched_setscheduler(g_rfile_acore_main.taskid_process, SCHED_FIFO, &sch_para)) {
        bsp_err("[init]: <%s> sched_setscheduler rfile_process failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    g_rfile_acore_main.taskid_user = kthread_run(rfile_pass_thread, &g_rfile_acore_main, "rfile_pass");
    if (IS_ERR(g_rfile_acore_main.taskid_user)) {
        bsp_err("[init]: <%s> kthread_run rfile_user failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }
    if (sched_setscheduler(g_rfile_acore_main.taskid_user, SCHED_FIFO, &sch_para)) {
        bsp_err("[init]: <%s> sched_setscheduler rfile_user failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    adp_rfile_init();
    g_rfile_acore_main.dev.device = rfile_dev_setup(&g_rfile_acore_main.dev);
    if (g_rfile_acore_main.dev.device == NULL) {
        return BSP_ERROR;
    }
    g_rfile_acore_main.data_recv_user = NULL;
    g_rfile_acore_main.data_recv_user = kzalloc(RFILE_LEN_MAX, GFP_KERNEL);
    if (g_rfile_acore_main.data_recv_user == NULL) {
        bsp_err("<%s> kzalloc failed.\n", __FUNCTION__);
        goto err;
    }
    if (rfile_icc_common_init()) {
        goto err;
    }
    if (rfile_icc_buffer_init()) {
        goto err;
    }
    rfile_set_event(&g_rfile_acore_main.event.init_done, EVENT_STATE_ON, RFILE_EVENT_INIT_DONE);

    bsp_err("[init]ok.\n");
    return BSP_OK;
err:
    cdev_del(&g_rfile_acore_main.dev.cdev);
    if (g_rfile_acore_main.data_recv_user != NULL) {
        kfree(g_rfile_acore_main.data_recv_user);
        g_rfile_acore_main.data_recv_user = NULL;
    }
    return BSP_ERROR;
}

static int modem_rfile_probe(struct platform_device *dev)
{
    return bsp_rfile_init();
}

static void modem_rfile_shutdown(struct platform_device *dev)
{
    bsp_info("%s shutdown start \n", __func__);
    rfile_set_event(&g_rfile_acore_main.event.shutdown, EVENT_STATE_ON, RFILE_EVENT_SHUTDOWN);
    return ;
}
#ifdef CONFIG_PM
static s32 modem_rfile_suspend(struct device *dev)
{
    static s32 count = 0;
    osl_sem_down(&g_rfile_acore_main.sem_queuelist);
    if (!list_empty_careful(&g_rfile_acore_main.queue_list) || g_rfile_acore_main.state != RFILE_STATE_IDLE) {
        bsp_err(" <%s> modem rfile is in doing, state = %d!\n", __FUNCTION__, g_rfile_acore_main.state);
        (void)rfile_get_queue_list();
        osl_sem_up(&g_rfile_acore_main.sem_queuelist);
        return -1;
    }
    osl_sem_up(&g_rfile_acore_main.sem_queuelist);

    count++;
    bsp_err(" <%s> modem rfile enter suspend! %d times \n", __FUNCTION__, count);
    return 0;
}

static const struct dev_pm_ops g_modem_rfile_pm_ops = {
    .suspend = modem_rfile_suspend,
};

#define BALONG_RFILE_PM_OPS (&g_modem_rfile_pm_ops)
#else
#define BALONG_RFILE_PM_OPS NULL
#endif
static struct platform_driver g_modem_rfile_drv = {
    .probe      = modem_rfile_probe,
    .shutdown   = modem_rfile_shutdown,
    .driver     = {
        .name     = "modem_rfile",
        .owner    = THIS_MODULE,
        .pm       = BALONG_RFILE_PM_OPS,
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

static struct platform_device g_modem_rfile_device = {
    .name = "modem_rfile",
    .id = 0,
    .dev = {
    .init_name = "modem_rfile",
    },
};
int modem_rfile_init(void)
{
    int ret;
    ret = platform_device_register(&g_modem_rfile_device);
    if (ret) {
        bsp_err("[init]platform_device_register g_modem_rfile_device fail !\n");
        return -1;
    }
    ret = platform_driver_register(&g_modem_rfile_drv);
    if (ret) {
        bsp_err("[init]platform_device_register g_modem_rfile_drv fail !\n");
        platform_device_unregister(&g_modem_rfile_device);
        return -1;
    }
    return 0;
}

void modem_rfile_exit(void)
{
    platform_device_unregister(&g_modem_rfile_device);
    platform_driver_unregister(&g_modem_rfile_drv);
}

#if (FEATURE_DELAY_MODEM_INIT == FEATURE_OFF)
module_init(modem_rfile_init);
#endif
