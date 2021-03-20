/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
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

#ifndef _AT_MBB_CMD_H_
#define _AT_MBB_CMD_H_


#include "vos.h"
#include "AtParse.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

#define AT_TRUSTLIST_NUM_PARA_MAX_LEN 24
#define AT_TRUSTLIST_NUM_MIN_LEN 1
#define AT_TRUSTLIST_NUM_PREFIX_MAX_LEN 2
#define AT_SET_SMS_TRUST_NUM_MAX_PARA_NUM 2
#define AT_SET_TRUST_NUM_CTRL_MAX_PARA_NUM 3

VOS_UINT32 AT_RegisterMbbCmdTable(VOS_VOID);
VOS_VOID AT_InitMtTrustListCtrl(VOS_VOID);
VOS_UINT32 AT_SetTrustNumCtrlPara(VOS_UINT8 index);
VOS_UINT32 AT_QryTrustNumCtrlPara(VOS_UINT8 index);
VOS_UINT32 AT_TestTrustNumCtrlPara(VOS_UINT8 index);
VOS_UINT32 AT_SetTrustCallNumPara(VOS_UINT8 index);
VOS_UINT32 AT_QryTrustCallNumPara(VOS_UINT8 index);
VOS_UINT32 AT_TestCallTrustNumPara(VOS_UINT8 index);
VOS_UINT32 AT_SetSmsTrustNumPara(VOS_UINT8 index);
VOS_UINT32 AT_QrySmsTrustNumPara(VOS_UINT8 index);
VOS_UINT32 AT_TestSmsTrustNumPara(VOS_UINT8 index);

#if (FEATURE_MBB_CUST == FEATURE_ON)
/* HSMF����������� */
VOS_UINT32 AT_SetHsmfPara(VOS_UINT8 index);
/* HSMF��ѯ������� */
VOS_UINT32 AT_QryHsmfPara(VOS_UINT8 index);
#endif


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
