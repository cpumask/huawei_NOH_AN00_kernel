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

#ifndef __BSP_SEC_CALL__
#define __BSP_SEC_CALL__

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

/**
 * 调用安全函数支持的命令ID
 */
typedef enum FUNC_SEC_CMD_ID {
    FUNC_CMD_ID_MIN = 0x800,
    FUNC_ICC_CHANNEL_RESET = FUNC_CMD_ID_MIN, /* ICC通道复位 */
    FUNC_ICC_MSG_SWITCH_ON,                   /* 允许ICC往modem发送消息 */
    FUNC_ICC_MSG_SWITCH_OFF,                  /* 禁止ICC往modem发送消息 */
    FUNC_SEC_DUMP_CHANNEL_ENABLE,             /* 异常场景通道使能传输 */
    FUNC_TRNG_SEED_REQUEST,                   /* 真随机数种子申请 */
    FUNC_DTS_LOAD_DTBO,                       /* DTS加载校验DTBO */
    FUNC_EFUSE_READ,                          /* Efuse Read控制命令 */
    FUNC_EFUSE_WRITE,                         /* Efuse Write控制命令 */
    FUNC_EFUSE_WRITE_WITH_DMPU,               /* Efuse Write with dmpu 控制命令 */
    FUNC_EFUSE_SEC_READ,                      /* Efuse Sec Read控制命令 */
    FUNC_EFUSE_SEC_WRITE,                     /* Efuse Sec Write控制命令 */
    FUNC_MDRV_EICC_CAOPTS,                    /* EICC 操作控制命令 */

    FUNC_CMD_ID_MIN_BUTTOM                    /* 请在此之前添加 */
} FUNC_CMD_ID;

#ifdef CONFIG_SEC_CALL
int bsp_sec_call(FUNC_CMD_ID func_cmd, unsigned int param);
int bsp_sec_call_ext(FUNC_CMD_ID func_cmd, unsigned int param, void *buffer, unsigned int size);
#else
static inline int bsp_sec_call(FUNC_CMD_ID func_cmd, unsigned int param){ return 0;}
static inline int bsp_sec_call_ext(FUNC_CMD_ID func_cmd, unsigned int param, void *buffer, unsigned int size){ return 0;}
#endif

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /* __BSP_SEC_CALL__ */
