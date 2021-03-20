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
#ifndef __MODEM_LOG_LINUX_H__
#define __MODEM_LOG_LINUX_H__
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>

#include <bsp_print.h>
#include <bsp_modem_log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define modem_log_pr_err(fmt, ...) bsp_err(fmt, ##__VA_ARGS__)
#define modem_log_pr_err_once(fmt, ...) bsp_err(fmt, ##__VA_ARGS__)
#define modem_log_pr_debug(fmt, ...)  // pr_err("[modem log]: " fmt, ##__VA_ARGS__)

enum MODEM_LOG_FLAG {
    MODEM_DATA_OK = 1,
    MODEM_NR_DATA_OK = 2,
    MODEM_ICC_CHN_READY = 4,
    MODEM_NRICC_CHN_READY = 8,
};

/* 模块返回错误码 */
enum MODEM_LOG_ERR_TYPE {
    MODEM_LOG_NO_MEM = -ENOMEM,
    MODEM_LOG_NO_USR_INFO = -35,
    MODEM_LOG_NO_IPC_SRC = -36,
};

struct logger_log {
    struct log_usr_info *usr_info;
    wait_queue_head_t wq;   /* The wait queue for reader */
    struct miscdevice misc; /* The "misc" device representing the log */
    struct mutex mutex;     /* The mutex that protects the @buffer */
    struct list_head logs;  /* The list of log channels */
    u32 fopen_cnt;
};

struct modem_log {
    struct wakeup_source wake_lock;
    struct notifier_block pm_notify;
    u32 init_flag;
};

void modem_log_wakeup_all(void);

#ifdef __cplusplus
}
#endif

#endif
