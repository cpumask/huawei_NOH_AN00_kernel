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

#ifndef __FCACORECRESET_H__
#define __FCACORECRESET_H__

#include "vos.h"
#include "PsTypeDef.h"
#include "mdrv.h"
#include "fc_intra_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* 底软C核复位模块等待FcACore完成回调事务超时时间 */
#define FC_ACORE_CRESET_TIMEOUT_LEN 5000

/* 注册给底软C核复位接口的回调函数的优先级 */
#define FC_ACORE_CRESET_CALLBACK_PRIOR 32

/* 指示C核复位前后的枚举 */
enum FC_ACORE_CresetMoment {
    FC_ACORE_CRESET_BEFORE_RESET = 0, /* 代表C核复位处理时 */
    FC_ACORE_CRESET_AFTER_RESET,      /* 代表C核复位成功后 */
    FC_ACORE_CRESET_BUTT
};
typedef VOS_UINT8 FC_ACORE_CresetMomentUint8;

extern VOS_SEM g_fcaCorecResetDoneSem;

extern VOS_UINT32 FC_AcoreCResetProc(FC_ACORE_CresetMomentUint8 cResetMoment);
extern VOS_INT FC_AcoreCResetCallback(drv_reset_cb_moment_e param, VOS_INT userdata);
extern VOS_VOID FC_AcoreCResetRcvStartRsp(VOS_VOID);
extern VOS_UINT32 FC_AcoreCResetSendNotify(FC_MsgTypeUint16 msgName);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
