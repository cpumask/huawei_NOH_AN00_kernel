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

#ifndef _NVIM_MSG_H
#define _NVIM_MSG_H


#include <osl_types.h>
#include <osl_list.h>
#include <osl_sem.h>
#include <osl_spinlock.h>
#include <bsp_nvim.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define NV_MSG_LEN 0x100

/*icc msg type*/
enum _icc_msg_e_
{
    NV_ICC_REQ_FLUSH = 0x40,
    NV_ICC_REQ_FLUSH_RWNV,
    NV_ICC_REQ_LOAD_BACKUP,
    NV_ICC_REQ_LOAD_CARRIER_CUST,
    NV_ICC_REQ_LOAD_CARRIER_RESUM,
    NV_ICC_REQ_UPDATE_DEFAULT,
    NV_ICC_REQ_UPDATE_BACKUP,
    NV_ICC_REQ_FACTORY_RESET,
    NV_ICC_REQ_FLUSH_RDWR_ASYNC,
    NV_ICC_REQ_UPGRADE_FLUSH,
    NV_ICC_CNF                          = 127,      /*ACK*/
    NV_ICC_BUTT                         = 0xFF,     /*MAX MSG*/
};

extern struct nv_global_msg_info_stru g_msg_ctrl;

typedef u32 (*nv_cb_func)(u32 ret, u32 sn);

/* 描述单个Nv信息 */
typedef struct _nv_item_info_stru_ {
    u16 itemid;   /* NV ID */
    u16 modem_id; /* NV Length */
} nv_item_info_t;

/* AP Nv写任务节点结构 */
typedef struct _nv_cmd_req_ {
    struct list_head stList;
    u8 reserved[2];
    u8 priority;
    u8 msg_type; /* msg type */
    u32 sn;      /* 序列号 */
    nv_cb_func nv_msg_callback;
} nv_cmd_req;

/* 队列元素 */
typedef struct _nv_cmd_reply_ {
    struct list_head stList; /* 链表节点 */
    u32 sn;                  /* sn号唯一标识本次请求 */
    osl_sem_id sem_req;      /* 请求阻塞信号量 */
    u32 ret;                 /* 处理结果 */
} nv_cmd_reply;

typedef struct _msg_queue_stru_ {
    struct list_head list;
    spinlock_t lock; /* 用于保护list的信号量 */
} msg_queue_t;

/* ICC请求NV_ICC_REQ_RESUME NV_ICC_REQ_RESUME消息结构 */
typedef struct _nv_icc_msg_stru_ {
    u32 send_slice; /* 发送时间 */
    u32 msg_type;   /* 消息类型 */
    u32 sn;         /* 序列号 */
    u32 ret;        /* 发送消息中存储要写入的NV数量,接收消息中存储处理结果 */
} nv_icc_msg_t;

enum _task_msg_type_e_ {
    NV_TASK_MSG_FLUSH = 0x1, /* 写NV请求 */
    NV_TASK_MSG_FLUSH_RWFILE,
    NV_TASK_MSG_LOAD_CARRIER,      /* 加载运行商定制NV */
    NV_TASK_MSG_LOAD_CARRIER_COMM, /* 加载运行商定制恢复NV */
    NV_TASK_MSG_LOAD_BACKUP,       /* 重加载镜像 */
    NV_TASK_MSG_UPDATE_DEFAULT,    /* at^inforbu */
    NV_TASK_MSG_UPDATE_BACKUP,     /* at^nvbackup */
    NV_TASK_MSG_FACTORY_RESET,     /* AT^FTYRESET */
    NV_TASK_MSG_UPGRADE_FLUSH,
    NV_TASK_MSG_BUTT,
};

#define HIGH_PRIORITY_MSG_QUEUE 1
#define LOW_PRIORITY_MSG_QUEUE 2

struct nv_global_msg_info_stru {
    msg_queue_t req_list;       /* 等待消息回复链表 */
    msg_queue_t high_task_list; /* 高优先级NV命令操作链表 */
    msg_queue_t low_task_list;  /* 低优先级NV命令操作链表 */
    u32 req_sn;
#ifdef UNUSE_CODE
    osl_sem_id flush_sem;
    nv_flush_list *flush_info;
#endif
    int icc_cb_count;
    int icc_cb_reply;
    int sync_wr_count;
    int sync_wr_done_count;
    int async_wr_count;
    int wr_timeout_count;
    int remote_wr_count;
};

nv_cmd_req *nv_get_cmd_req(void);

int nv_put_cmd_req(nv_cmd_req *pNvCmdReq);

u32 nv_send_rmsg(u32 msg_type, u32 req_sn);

u32 bsp_nvm_ccore_msg_cb(u32 result, u32 sn);

u32 nv_cpmsg_chan_init(void);

u32 nv_msg_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

