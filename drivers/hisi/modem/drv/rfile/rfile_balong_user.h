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

#ifndef __RFILE_USER_BALONG_H__
#define __RFILE_USER_BALONG_H__



/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/stat.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/statfs.h>
#include <linux/rcupdate.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/cdev.h>

#include "drv_comm.h"
#include "osl_types.h"
#include "osl_sem.h"
#include "osl_list.h"
#include "mdrv_memory.h"
#include "bsp_icc.h"
#include "bsp_rfile.h"
#include "bsp_reset.h"

#ifndef CONFIG_ICC
#include "mdrv_eicc.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define DELAY_TIME 1
#define RFILE_PATHLEN_MAX 255

#define RFILE_MALLOC(size) kmalloc(size, GFP_KERNEL)
#define RFILE_FREE(ptr) kfree(ptr)
#define RFILE_DOWNTIME 30000
#define RFILE_QUEUE_STACK 10
#define RFILE_STACK_MAX 12

#define RFILE_MAX_SEND_TIMES (10) /* 发送失败时，最大尝试次数 */

#ifdef CONFIG_ICC
#define RFILE_CHANNAL_FULL ICC_INVALID_NO_FIFO_SPACE
#define RFILE_LEN_MAX (ICC_RFILE_SIZE - ICC_CHANNEL_PAYLOAD)
#else
#define RFILE_EICC_PAYLOAD 0x80
#define RFILE_CHANNAL_FULL EICC_ERR_EAGAIN
#define RFILE_LEN_MAX (64*1024 - 128 - RFILE_EICC_PAYLOAD)
#endif

/* RFILE模块写操作依赖于ICC的最大长度 */
#define RFILE_WR_LEN_MAX (RFILE_LEN_MAX - sizeof(struct bsp_rfile_comm_req))

/* RFILE模块读操作依赖于ICC的最大长度 */
#define RFILE_RD_LEN_MAX (RFILE_LEN_MAX - sizeof(struct bsp_rfile_comm_cnf))


typedef osl_sem_id rfile_sem_id;

#define RFILE_DEV_NAME "rfile"

#define RFILE_DEV_CLASS "rfile_class"
#define RFILE_DEV_NUM 1

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/* 操作类型 */
enum bsp_rfile_op_type {
    EN_RFILE_OP_OPEN = 0,
    EN_RFILE_OP_CLOSE,
    EN_RFILE_OP_WRITE,
    EN_RFILE_OP_WRITE_SYNC,
    EN_RFILE_OP_READ,
    EN_RFILE_OP_SEEK,
    EN_RFILE_OP_TELL,
    EN_RFILE_OP_REMOVE,
    EN_RFILE_OP_MKDIR,
    EN_RFILE_OP_RMDIR,
    EN_RFILE_OP_OPENDIR,
    EN_RFILE_OP_READDIR,
    EN_RFILE_OP_CLOSEDIR,
    EN_RFILE_OP_STAT,
    EN_RFILE_OP_ACCESS,
    EN_RFILE_OP_RENAME,
    EN_RFILE_OP_CHECK_LOAD_MODE,
    EN_RFILE_OP_RESET,
    EN_RFILE_OP_OPEN_RECOVER,
    EN_RFILE_OP_OPENDIR_RECOVER,
    EN_RFILE_OP_BUTT
};

/* 处理状态 */
enum bsp_rfile_state {
    RFILE_STATE_IDLE = 0,
    RFILE_STATE_DOING,
    RFILE_STATE_RESET,
    RFILE_STATE_UNINITED,
    RFILE_THREAD_IN
};

enum bsp_rfile_event_state {
    EVENT_STATE_OFF = 0,
    EVENT_STATE_ON,
    EVENT_STATE_RESET_BEFORE,
    EVENT_STATE_RESET_DONE
};

enum bsp_rfile_event_type {
    RFILE_EVENT_SHUTDOWN = 0,
    RFILE_EVENT_RESET,
    RFILE_EVENT_ICC_CB,
    RFILE_EVENT_INIT_DONE,
    RFILE_EVENT_HANDLE_DONE
};

enum bsp_rfile_app_state {
    APP_STATE_UNINTED = 0,
    APP_STATE_REALEASED,
    APP_STATE_INTING,
    APP_STATE_DONE
};

enum bsp_rfile_open_type {
    RFILE_OPEN_FILE = 0,
    RFILE_OPEN_DIR = 1
};

enum bsp_rfile_add_queue_type {
    RFILE_ADD_QUEUE_HEAD = 0,
    RFILE_ADD_QUEUE_TAIL = 1,
};

enum bsp_rfile_offset_manage_type {
    RFILE_MANAGE_READ_WRITE = 0,
    RFILE_MANAGE_SEEK = 1
};


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
struct bsp_rfile_fd_list {
    struct list_head stlist;
    u32 open_type;
    u32 fd;
    u32 offset;
    u32 mode;
    u32 flags;
    u32 fd_new;
    u32 fd_outdate_flag;
    s8 name[RFILE_NAME_MAX + 1];
};

struct bsp_rfile_event {
    u32 shutdown;
    u32 reset;
    u32 icc_cb;
    u32 init_done;
    u32 handle_done;
};

struct bsp_rfile_dev_info {
    struct cdev cdev;
    dev_t dev_no;
    struct class *rfile_class;
    struct device *device;
};

/* 队列元素 */
struct bsp_rfile_que_stru_acore {
    struct list_head stlist; /* 链表节点 */
    u32 op_type;
    u32 queue_cnt;
    u32 fd;     // close,closedir,read,write,tell,seek
    u32 mode;   // open
    u32 flags;  // open
    u32 ret;    /* bsp_rfile_comm_cnf中返回值 */
    union {
        void *ptr;   /* 返回数据需要拷贝到的目的地址,read readdir stat */
        char *path;  // open,remove,opendir,rmdir,mkdir,stat,rename,access
    };
    union {
        u32 ptr_size;
        u32 pathlen; /* 包含\0 */
    };
    u32 datalen_send; /* data request包总长度 */
    u32 datalen_recv; /* data request包总长度 */
    void *data_send;  /* 发送用户态的数据 */
    void *data_recv;  /* 接收用户态的数据 */
};

/* 主控信息 */
struct bsp_rfile_main_stru_acore {
    u32 state;
    volatile u32 app_state;
    u32 release_cnt;
    struct bsp_rfile_event event;
    struct bsp_rfile_dev_info dev;
    struct task_struct *taskid_process;
    struct task_struct *taskid_user;
    rfile_sem_id sem_taskmain;
    rfile_sem_id sem_taskpass;
    rfile_sem_id sem_read;
    rfile_sem_id sem_write;
    rfile_sem_id sem_reset;
    rfile_sem_id sem_queuelist;
    rfile_sem_id sem_fdlist;
    struct list_head fd_list;                        /* C核打开文件&目录链表头 */
    struct list_head queue_list;                     /* 消息队列链表头 */
    struct bsp_rfile_que_stru_acore *queue_data_now; /* 当前处理的消息数据 */
    void *data_recv_user;
    struct wakeup_source wake_lock;
    void *data_recv_icc[RFILE_QUEUE_STACK];
    u32 queue_curr;
};

struct bsp_rfile_comm_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 queue_cnt;
    u32 fd; /* file:seek,read,write,close,tell;dir:readdir,closedir */
    union {
        s32 mode;      // open,accsess
        s32 offset;    // seek
        u32 readsize;  // read
        u32 count;     // readdir
    };
    union {
        s32 flags;   // open
        s32 whence;  // seek
    };
    u32 datalen;
    u8 data[0]; /* path:open,remove,mkdir,opendir,removedir,stat,access,rename, path包含‘\0’ */
};

struct bsp_rfile_comm_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 queue_cnt;
    s32 ret;
    u8 data[0];  // read, stat
};

/* ****************************************维测相关****************************************** */
#define RFILE_MNTN_INNER_STATE_CNT 40
#define RFILE_MNTN_APP_STATE_CNT 10
#define RFILE_MNTN_EVENT_STATE_CNT 20
#define RFILE_MNTN_FD_CNT 20
#define RFILE_MNTN_QUEUE_CNT 20
#define RFILE_MNTN_HANDLE_CNT 40
#define RFILE_MNTN_PATHLEN_MAX 64

struct rfile_state_trace {
    u32 slice;
    u32 state_type;
};
struct bsp_rfile_inner_state_dbg {
    struct rfile_state_trace trace[RFILE_MNTN_INNER_STATE_CNT];
    u32 trace_index;
};

struct bsp_rfile_app_state_dbg {
    struct rfile_state_trace trace[RFILE_MNTN_APP_STATE_CNT];
    u32 trace_index;
};

struct rfile_event_trace {
    u32 slice;
    u32 event_name;
    u32 state_type;
};

struct bsp_rfile_event_dbg {
    struct rfile_event_trace trace[RFILE_MNTN_EVENT_STATE_CNT];
    u32 trace_index;
};

enum bsp_rfile_acore_handle_state {
    RFILE_ACORE_OP_IN = 0,
    RFILE_ACORE_USER_SEND,
    RFILE_ACORE_USER_RECV,
    RFILE_ACORE_OP_OUT
};

struct rfile_handle_trace {
    u32 slice;
    u32 op_type;
    u32 queue_cnt;
    u32 state_type;
    s32 ret;
};

struct bsp_rfile_acore_handle_dbg {
    struct rfile_handle_trace trace[RFILE_MNTN_HANDLE_CNT];
    u32 trace_index;
};

struct rfile_queue_dump {
    u32 op_type;
    u32 queue_cnt;
};

struct bsp_rfile_queue_dbg {
    struct rfile_queue_dump debug[RFILE_MNTN_QUEUE_CNT];
};

struct rfile_fd_dump {
    u32 fd;
    u32 offset;
    char path[RFILE_MNTN_PATHLEN_MAX];
};

struct bsp_rfile_fd_dbg {
    struct rfile_fd_dump debug[RFILE_MNTN_FD_CNT];
};

struct bsp_rfile_dbg_info {
    u64 dbg_start_byte;
    struct bsp_rfile_inner_state_dbg inner_state_trace;
    struct bsp_rfile_app_state_dbg app_state_trace;
    struct bsp_rfile_acore_handle_dbg acore_handle_trace;
    struct bsp_rfile_event_dbg event_trace;
    struct bsp_rfile_queue_dbg queue_dump;
    struct bsp_rfile_fd_dbg fd_dump;
};

void rfile_icc_common_cb(void);
int rfile_icc_common_init(void);
int rfile_icc_common_recv(void* databuf, u32 datalen);
int rfile_icc_common_send(void* databuf, u32 datalen);


#pragma pack(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif
#endif /* end of rfile_balong.h */
