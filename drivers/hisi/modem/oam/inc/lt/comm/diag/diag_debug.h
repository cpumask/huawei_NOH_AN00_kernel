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

#ifndef __DIAG_DEBUG_H__
#define __DIAG_DEBUG_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include <product_config.h>
#include <mdrv.h>
#include <vos.h>
#include <msp.h>
#include "diag_frame.h"
#include "diag_cfg_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

#define DIAG_TEST_MSG_ID 0x123
#define DIAG_TEST_MSG_DATA 0x1234567
#define DIAG_TEST_FLOW_DATA 0x5A5A5A5A
#define DIAG_MNTN_ONE_SEC_SLICES  32768

typedef enum {
    DIAG_DEBUG_API = 0,
    DIAG_DEBUG_API_AIR,
    DIAG_DEBUG_API_TRACE,
    DIAG_DEBUG_API_USERPLANE,
    DIAG_DEBUG_API_EVENT,
    DIAG_DEBUG_API_PRINT,
    DIAG_DEBUG_API_VOLTE,
    DIAG_DEBUG_API_TRANS,
    DIAG_DEBUG_API_DEFAULT
} DIAG_DEBUG_API_ENUM;

/* CBT ********************************************************************** */
#define DIAG_DEBUG_SDM_FUN(enType, ulRserved1, ulRserved2, ulRserved3) \
    diag_CBT(enType, ulRserved1, ulRserved2, ulRserved3)

typedef enum {
    EN_DIAG_CBT_MSGMSP_CMD_ENTER,
    EN_DIAG_CBT_MSGMSP_TRANS_ENTER,
    EN_DIAG_CBT_MSGMSP_SYS_ENTER,
    EN_DIAG_CBT_MSGMSP_DFLT_ENTER,

    EN_DIAG_CBT_MSGMSP_TRANS_REQ,
    EN_DIAG_CBT_MSGMSP_TRANS_CNF,

    EN_DIAG_CBT_API_PRINTFV_OK,
    EN_DIAG_CBT_API_PRINTFV_ERR,

    EN_DIAG_CBT_API_EVENT_OK,
    EN_DIAG_CBT_API_EVENT_ERR,

    EN_DIAG_CBT_API_AIR_OK,
    EN_DIAG_CBT_API_AIR_ERR,

    EN_DIAG_CBT_API_TRACE_OK,
    EN_DIAG_CBT_API_TRACE_ERR,

    EN_DIAG_API_MSG_LAYER_OK,
    EN_DIAG_API_MSG_LAYER_ERR,
    EN_DIAG_API_MSG_LAYER_MATCH,
    EN_DIAG_API_MSG_LAYER_NOTIFY,

    EN_DIAG_CBT_API_TRACE_FILTER,
    EN_DIAG_CBT_API_TRACE_MATCH,

    EN_DIAG_CBT_API_USER_OK,
    EN_DIAG_CBT_API_USER_ERR,

    EN_DIAG_CBT_API_PACKET_ERR_REQ,

    EN_DIAG_CBT_API_TRANS_OK,
    EN_DIAG_CBT_API_TRANS_ERR,

    EN_DIAG_CBT_API_VOLTE_ERR,
    EN_DIAG_CBT_API_VOLTE_OK,

    EN_DIAG_DEBUG_CODE_PACKET_START,
    EN_DIAG_DEBUG_CODE_PACKET_START_ERROR,

    EN_DIAG_DEBUG_GET_SRC_BUF_START,
    EN_DIAG_DEBUG_GET_SRC_BUF_START_ERROR,

    EN_SDM_DIAG_DOT,
    EN_SDM_DIAG_DOT_ERR,
    EN_SDM_DIAG_DOT_OK,

    EN_DIAG_DEBUG_MSG,
    EN_DIAG_DEBUG_MSG_ERR,
    EN_DIAG_DEBUG_CONN_CFG,
    EN_DIAG_DEBUG_DIS_CONN_CFG,
    EN_DIAG_DEBUG_LAYER_CFG,
    EN_DIAG_DEBUG_PRINT_CFG,
    EN_DIAG_DEBUG_EVENT_CFG,
    EN_DIAG_DEBUG_AIR_CFG,
    EN_DIAG_DEBUG_USEPLANE_CFG,

    EN_DIAG_DEBUG_AGENT_INIT,
    EN_DIAG_DEBUG_AGENT_INIT_ERROR,
    EN_DIAG_DEBUG_AGENT_DISPATCH_CMD,
    EN_DIAG_DEBUG_REG_RD,
    EN_DIAG_DEBUG_REG_WR,
    EN_DIAG_DEBUG_BBP_LOG,
    EN_DIAG_DEBUG_BBP_SAMPLE,
    EN_DIAG_DEBUG_LTE_DSP_CNF,
    EN_DIAG_DEBUG_TDS_DSP_CNF,

    EN_DIAG_DEBUG_BBP_AGENT_TIME_OUT_ENTRY,
    /* DIAG AGENT与LDSP邮箱交互 */
    EN_DIAG_AGENT_LDSP_MB_MSG,

    EN_DIAG_CBT_API_DT_ERR,
    EN_DIAG_CBT_API_DT_OK,

    EN_DIAG_DEBUG_INFO_MAX
} DIAG_CBT_ID_ENUM;

/* 发起主动复位时MSP传入的参数ID最小值为:0xB000000 */
#define DIAG_BASE_ERROR_NUMBER (0xB0000000)

enum MSP_SYSTEMERROR_MODID_ENUM {
    DIAG_ERROR_MODID_BASE = (0xB0000000),
    DIAG_CALLED_IN_IRQ = DIAG_ERROR_MODID_BASE,
    DIAG_ERROR_MODID_OVERFLOW,
    DIAG_ERROR_MODID_BUTT = (0xB0000010)
};

typedef struct {
    VOS_UINT32 ulCalledNum; /* 调用次数或者消息次数,或者表明该函数是否被调用 */
    VOS_UINT32 ulRserved1;
    VOS_UINT32 ulRserved2;
    VOS_UINT32 ulRserved3;
    VOS_UINT32 ulRtcTime;
} DIAG_CBT_INFO_TBL_STRU;

typedef struct {
    VOS_CHAR pucApiName[8];
    DIAG_CBT_ID_ENUM ulOk;
    DIAG_CBT_ID_ENUM ulErr;
    VOS_UINT32 ulSlice;
    VOS_UINT32 ulOkNum;
    VOS_UINT32 ulErrNum;
} DIAG_DEBUG_API_INFO_STRU;

VOS_VOID diag_CBT(DIAG_CBT_ID_ENUM ulType, VOS_UINT32 ulRserved1, VOS_UINT32 ulRserved2, VOS_UINT32 ulRserved3);

/* LNR ********************************************************************** */
#define DIAG_LNR_NUMBER (100)

typedef enum {
    EN_DIAG_LNR_CMDID = 0, /* 最后N条接收到的诊断命令 */

    EN_DIAG_LNR_LOG_LOST, /* 最后N条log丢失的记录 */

    EN_DIAG_LNR_PS_TRANS, /* 最后N条PS透传命令的记录 */

    EN_DIAG_LNR_CCORE_MSG, /* 最后N条C核从A核收到的消息ID的记录 */

    EN_DIAG_LNR_MESSAGE_RPT, /* 最后N条通过message模块report上报的cmdid的记录 */

    EN_DIAG_LNR_TRANS_IND, /* 最后N条透传上报的记录 */

    EN_DIAG_LNR_INFO_MAX
} DIAG_LNR_ID_ENUM;

typedef enum {
    EN_REQ_SWITCH_OFF = 0,
    EN_REQ_SWITCH_ON = 1,
} DIAG_MNTN_REQ_ENUM;

typedef struct {
    VOS_UINT32 ulCur;
    VOS_UINT32 ulRserved1[DIAG_LNR_NUMBER];
    VOS_UINT32 ulRserved2[DIAG_LNR_NUMBER];
} DIAG_LNR_INFO_TBL_STRU;

VOS_VOID diag_LNR(DIAG_LNR_ID_ENUM ulType, VOS_UINT32 ulRserved1, VOS_UINT32 ulRserved2);

/* DIAG lost 保留为解析以前的结构保留****************************************** */
typedef struct {
    VOS_UINT32 ulModuleId;
    VOS_UINT32 ulNo; /* 序号 */

    VOS_UINT32 ulLostTotalNum; /* 丢包总次数 */

    VOS_UINT32 aulCurFailNum[6]; /* 当前时间段内丢包的各类次数 */

    VOS_UINT32 ulSrcChnSize; /* 编码源通道剩余字节数 */
    VOS_UINT32 ulDstChnSize; /* 编码目的通道剩余字节数 */
    VOS_UINT32 ulDSPChnSize; /* DSP编码源通道剩余字节数 */

    VOS_UINT32 ulThrputPhy; /* 物理通道吞吐率 */
    VOS_UINT32 ulThrputCb;  /* 回调吞吐率 */

    VOS_UINT32 ulTotalSocpDstLen; /* 从SOCP编码目的buffer中读取的总字节数 */

    VOS_UINT32 ulTotalUSBLen;     /* 向USB写入的总字节数 */
    VOS_UINT32 ulTotalUSBFreeLen; /* USB回调释放的总字节数 */
    VOS_UINT32 ulTotalVCOMLen;    /* 向VCOM写入的总字节数 */
    VOS_UINT32 ulTotalVCOMErrLen; /* 向VCOM写入失败的总字节数 */
    VOS_UINT32 ulVCOMMaxTimeLen;  /* 调用VCOM写接口话费的最大时长 */

    VOS_UINT32 ulTotalSOCKETLen; /* 向SOCKET写入的总字节数 */

    VOS_UINT32 aulRreserve[4]; /* 预留 */

    VOS_UINT32 aulToolreserve[4]; /* 给工具预留的16个字节，用于在工具上显示工具的维测信息 */
} DIAG_LOST_INFO_STRU;

typedef struct {
    VOS_UINT32 ulTraceNum; /* 层间消息成功上报次数 */
    VOS_UINT32 ulEventNum; /* event成功上报次数 */
    VOS_UINT32 ulLogNum;   /* Log成功上报次数 */
    VOS_UINT32 ulAirNum;   /* 空口成功上报次数 */
    VOS_UINT32 ulTransNum; /* 透传成功上报次数 */

    VOS_UINT32 ulThrputEnc; /* 编码源吞吐率 */

    VOS_UINT32 ulThrputPhy; /* 物理通道吞吐率 */
    VOS_UINT32 ulThrputCb;  /* 回调吞吐率 */

    VOS_UINT32 ulSrcChnSize; /* 编码源通道剩余字节数 */
    VOS_UINT32 ulDstChnSize; /* 编码目的通道剩余字节数 */

    VOS_UINT32 ulTotalLostTimes; /* 丢包次数 */

    VOS_UINT32 ulTotalUSBLen;     /* 向USB写入的总字节数 */
    VOS_UINT32 ulTotalUSBFreeLen; /* USB回调释放的总字节数 */
    VOS_UINT32 ulTotalVCOMLen;    /* 向VCOM写入的总字节数 */
    VOS_UINT32 ulTotalVCOMErrLen; /* 向VCOM写入失败的总字节数 */
    VOS_UINT32 ulVCOMMaxTimeLen;  /* 调用VCOM写接口话费的最大时长 */

    VOS_UINT32 ulTotalSOCKETLen; /* 向SOCKET写入的总字节数 */

    VOS_UINT32 aulReserve[4]; /* 预留 */

    VOS_UINT32 aulToolreserve[12]; /* 给工具预留的64个字节，用于在工具上显示工具的维测信息 */
} DIAG_MNTN_INFO_STRU;

/* DIAG MNTN ****************************************** */
#define DIAG_DEBUG_LOST_LEN (5 * 32768)

#define DIAG_DEBUG_LOST_MINUTE (60 * 32768)

#define DIAG_SLICE_DELTA(begin, end) (((end) > (begin)) ? ((end) - (begin)) : ((0xFFFFFFFF - (begin)) + (end)))

#define DIAG_RATE_INFO_LEN (0x40)

#if defined(__KERNEL__)
#define DIAG_SRC_NAME "ap_diag"
#elif defined(__OS_LRCCPU__)
#define DIAG_SRC_NAME "lr_diag"
#elif defined(__OS_NRCCPU__)
#define DIAG_SRC_NAME "nr_diag"
#endif

typedef struct {
    VOS_UINT32 ulModuleId;
    VOS_UINT32 ulNo; /* 序号 */
} DIAG_MNTN_HEAD_STRU;

typedef struct {
    DIAGLOG_MNTN_SRC_INFO
    /* acore diag mntn info */
    VOS_UINT32 ulTraceNum;      /* 层间消息成功上报次数 */
    VOS_UINT32 ulLayerNum;      /* OSA勾包层间消息成功上报次数 */
    VOS_UINT32 ulEventNum;      /* event成功上报次数 */
    VOS_UINT32 ulLogNum;        /* Log成功上报次数 */
    VOS_UINT32 ulAirNum;        /* 空口成功上报次数 */
    VOS_UINT32 ulTransNum;      /* 透传成功上报次数 */
    VOS_UINT32 ulVolteNum;      /* Volte消息成功上报次数 */
    VOS_UINT32 ulUserNum;       /* Volte消息成功上报次数 */
    VOS_UINT32 ulLayerMatchNum; /* 由于脱敏层间消息被过滤次数 */
} DIAG_MNTN_SRC_INFO_STRU;

typedef struct {
    VOS_UINT32 ulTraceNum;      /* 层间消息成功上报次数 */
    VOS_UINT32 ulLayerNum;      /* OSA勾包层间消息成功上报次数 */
    VOS_UINT32 ulEventNum;      /* event成功上报次数 */
    VOS_UINT32 ulLogNum;        /* Log成功上报次数 */
    VOS_UINT32 ulAirNum;        /* 空口成功上报次数 */
    VOS_UINT32 ulTransNum;      /* 透传成功上报次数 */
    VOS_UINT32 ulVolteNum;      /* Volte消息成功上报次数 */
    VOS_UINT32 ulUserNum;       /* Volte消息成功上报次数 */
    VOS_UINT32 ulLayerMatchNum; /* 由于脱敏层间消息被过滤次数 */
} DIAG_MNTN_API_OK_STRU;

typedef struct {
    mdrv_diag_trans_ind_s pstMntnHead;
    DIAG_MNTN_SRC_INFO_STRU pstMntnInfo;
} DIAG_DEBUG_SRC_MNTN_STRU;

#if (VOS_OS_VER == VOS_LINUX)

typedef struct {
    mdrv_diag_trans_ind_s pstMntnHead;
    diag_mntn_dst_info_s pstMntnInfo;
} DIAG_DEBUG_DST_LOST_STRU;
#endif
/* DIAG_TRACE ****************************************** */
#define DIAG_DEBUG_TRACE_DELAY (1 * 32768)
#define DIAG_DEBUG_TXT_LOG_DELAY (10 * 32768)
#define DIAG_DEBUG_TXT_LOG_LENGTH 256
typedef struct {
    VOS_UINT32 ulModuleId;
    VOS_UINT32 ulLevel;
    VOS_UINT32 ulSn;
    VOS_CHAR pucData[DIAG_DEBUG_TXT_LOG_LENGTH];
} DIAG_DEBUG_INFO_STRU;

typedef struct {
    msp_diag_frame_info_s pstFrameInfo;
    DIAG_DEBUG_INFO_STRU pstInfo;
} DIAG_DEBUG_FRAME_STRU;

/* A核向C核发送的debug消息 ****************************************** */
#define DIAG_DEBUG_DFR_BIT 0x00000001 /* 保存码流 */
#define DIAG_DEBUG_NIL_BIT 0x00000002 /* 保存log不上报的定位信息 */

typedef struct {
    VOS_UINT32 ulSenderCpuId;
    VOS_UINT32 ulSenderPid;
    VOS_UINT32 ulReceiverCpuid;
    VOS_UINT32 ulReceiverPid;
    VOS_UINT32 ulLength;
    VOS_UINT32 ulMsgId;

    /* bit0 : DFR */
    /* bit1 : no ind log */
    VOS_UINT32 ulFlag;
} DIAG_A_DEBUG_C_REQ_STRU;

/* 对外函数接口 ****************************************** */
DIAG_CBT_INFO_TBL_STRU *diag_DebugGetInfo(VOS_VOID);
VOS_VOID DIAG_ShowLNR(DIAG_LNR_ID_ENUM ulType, VOS_UINT32 n);
VOS_VOID DIAG_ShowLogCfg(VOS_UINT32 ulModuleId);
VOS_VOID DIAG_ShowEventCfg(VOS_UINT32 ulpid);
VOS_VOID DIAG_ShowAirCfg(VOS_VOID);
VOS_VOID DIAG_ShowLayerCfg(VOS_UINT32 ulModuleId, VOS_UINT32 ulSrcDst);
VOS_VOID DIAG_ShowUsrCfg(VOS_VOID);
VOS_VOID DIAG_ShowLost(VOS_VOID);
VOS_VOID DIAG_ShowTrans(VOS_UINT32 n);
VOS_VOID DIAG_ShowPsTransCmd(VOS_UINT32 n);
VOS_UINT32 diag_DebugMsgProc(MsgBlock *pMsgBlock);
VOS_VOID DIAG_DebugEventReport(VOS_UINT32 ulpid);
VOS_VOID DIAG_DebugLayerReport(VOS_UINT32 ulsndpid, VOS_UINT32 ulrcvpid, VOS_UINT32 ulMsg);
VOS_VOID DIAG_DebugVosLayerReport(VOS_UINT32 ulsndpid, VOS_UINT32 ulrcvpid, VOS_UINT32 ulMsg);
VOS_VOID DIAG_DebugLogReport(VOS_UINT32 ulpid, VOS_UINT32 level);
VOS_VOID DIAG_DebugTransReport(VOS_UINT32 ulpid);
VOS_VOID DIAG_DebugLayerCfg(VOS_UINT32 ulModuleId, VOS_UINT8 ucFlag);
VOS_VOID diag_ReportMntn(VOS_VOID);
void diag_debug_ind_src_lost(VOS_UINT32 len);
void diag_reset_src_mntn_info(void);
VOS_VOID diag_ReportSrcMntn(VOS_VOID);
VOS_VOID diag_ReportDstMntn(VOS_VOID);
VOS_UINT32 DIAG_ApiTest(VOS_UINT8 *pstReq);
VOS_UINT32 diag_debug_conn_msg(VOS_UINT8 *preq);
VOS_UINT32 diag_MntnCfgProc(VOS_UINT8 *pstReq);
VOS_UINT32 diag_RateInfoCfgProc(VOS_UINT8 *pstReq);
VOS_VOID diag_StopMntnTimer(VOS_VOID);
VOS_VOID diag_StartMntnTimer(VOS_UINT32 ulMntnReportTime);
VOS_VOID diag_buffer_overflow_record(VOS_UINT32 package_len);
VOS_VOID diag_NotifyOthersMntnInfo(DIAG_CMD_LOG_DIAG_MNTN_REQ_STRU *pstDebugReq);
VOS_VOID diag_NotifyOthersMntnInfo_Ex(DIAG_MNTN_REQ_STRU *pstDebugInfo);
VOS_VOID DIAG_DebugDtReport(VOS_UINT32 ulpid);
VOS_UINT32 DIAG_FlowTest(VOS_UINT8 *pstReq);
VOS_VOID DIAG_ACoreFlowPress(VOS_VOID);
VOS_VOID DIAG_CCoreFlowPress(VOS_VOID);
VOS_UINT32 diag_debug_send_conn_msg(void);
VOS_VOID diag_StartRateInfoTimer(VOS_UINT32 rate_info_timer_len);
VOS_VOID diag_StopRateInfoTimer(VOS_VOID);
VOS_VOID diag_ReportRateInfo(VOS_VOID);
VOS_UINT32 diag_RateInfoCfgProc(VOS_UINT8 *pstReq);

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

#endif /* end of diag_Debug.h */
