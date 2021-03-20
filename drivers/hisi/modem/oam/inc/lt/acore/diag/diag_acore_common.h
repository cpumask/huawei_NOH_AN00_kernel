﻿/*
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

#ifndef __DIAG_ACORE_COMMON_H__
#define __DIAG_ACORE_COMMON_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/list.h>
#include <vos.h>
#include "app_rrc_interface.h"
#include "msp_diag_comm.h"
#include "diag_message.h"
#include "diag_frame.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/
#define DIAG_DEBUG_TRANS (0x00000001)
#define DIAG_TRANS_CNF_TIMEOUT(CurTime, StartTime) \
    (CurTime>StartTime?((CurTime)-(StartTime)):((0xfffffff-(StartTime))+(CurTime)))

typedef struct {
    struct list_head    TransHead; /* 透传命令链表的头结点 */
    VOS_SEM             TransSem;  /* 透传命令链表互斥信号量 */
} DIAG_TRANS_HEADER_STRU;

typedef struct {
    VOS_UINT32      ulMagicNum;     /* 有效值为TRANS的前4个字母的ASCII码 0x5452414E */
    VOS_UINT32      StartTime;      /* 节点创建的时间戳 */
    struct list_head DiagList;      /* 指向下一个缓存节点命令数据 */
    VOS_UINT8       ucRcvData[0];   /* 工具下发的完整SOCP包数据 */
} DIAG_TRANS_NODE_STRU;

/* APP_OM_MSG_STRU由协议栈定义 */
typedef APP_OM_Msg DIAG_TRANS_MSG_STRU;

/*****************************************************************************
描述 : 透传命令的参数结构
REQ : DIAG_PS_MSG_STRU
CNF : DIAG_TRANS_CNF_STRU
*****************************************************************************/
/* 透传命令的应答结构 */
typedef struct {
    VOS_UINT32 ulAuid;    /* 原AUID */
    VOS_UINT32 ulSn;      /* HSO分发，插件命令管理 */
    VOS_UINT8 aucPara[0]; /* 参数内容 */
} DIAG_TRANS_CNF_STRU;

/* 自旋锁，用来作编码源buff的临界资源保护 */
extern VOS_SPINLOCK g_stScmIndSrcBuffSpinLock;
extern VOS_SPINLOCK g_stScmCnfSrcBuffSpinLock;

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
VOS_VOID diag_TransTimeoutProc(REL_TimerMsgBlock *pTimer);
VOS_UINT32 diag_TransCnfProc(VOS_UINT8 *pstCnf, VOS_UINT32 ulLen, unsigned int ulGroupId,
                             DIAG_TRANS_HEADER_STRU *pstHead);
DIAG_TRANS_NODE_STRU *diag_AddTransInfoToList(VOS_UINT8 *pstReq, VOS_UINT32 ulRcvlen, DIAG_TRANS_HEADER_STRU *pstHead);

DIAG_TRANS_NODE_STRU *diag_IsTransCnf(DIAG_TRANS_MSG_STRU *pstPsCnf, DIAG_TRANS_HEADER_STRU *pstHead);

VOS_VOID diag_GetTransInfo(MSP_DIAG_CNF_INFO_STRU *pstInfo, DIAG_TRANS_CNF_STRU *pstDiagCnf,
                           DIAG_TRANS_MSG_STRU *pstPsCnf, DIAG_TRANS_NODE_STRU *pNode);

VOS_VOID diag_DelTransCmdNode(DIAG_TRANS_NODE_STRU *pTempNode);

VOS_UINT32 diag_TransReqProcEntry(msp_diag_frame_info_s *pstReq, DIAG_TRANS_HEADER_STRU *pstHead);
VOS_VOID OM_AcpuCltInfoCnfMsgProc(MsgBlock *pMsg);
VOS_VOID OM_AcpuCltInfoCnfNotNeedProcessSetFlag(VOS_VOID);
VOS_VOID OM_AcpuRcvCltInfoFinish(VOS_VOID);
VOS_VOID ppm_socket_diag_srv_task(VOS_VOID);
VOS_VOID ppm_socket_at_srv_task(VOS_VOID);
VOS_UINT32 diag_DisconnectTLPort(void);
VOS_VOID diag_ApAgentMsgProc(MsgBlock *pMsgBlock);
VOS_VOID diag_TimerMsgProc(MsgBlock *pMsgBlock);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of msp_acore_common.h */
