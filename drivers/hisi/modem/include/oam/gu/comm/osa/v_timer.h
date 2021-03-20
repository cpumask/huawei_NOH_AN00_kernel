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

/*
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2012-2019
 * ��������: OSA��ʱ��ģ��ͷ�ļ�
 * ��������: 2006��10��3��
 */

#ifndef _VOS_TIMER_H
#define _VOS_TIMER_H

#include "v_typdef.h"
#include "vos_config.h"
#include "v_msg.h"
#include "v_int.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define VOS_32K_TIMER_ENABLE VOS_YES

/* errno definiens */
#define VOS_ERRNO_RELTM_CTRLBLK_INITFAIL 0x20060000
#define VOS_ERRNO_RELTM_TASK_INITFAIL 0x20060001
#define VOS_ERRNO_RELTM_START_MSGNOTINSTALL 0x20060004
#define VOS_ERRNO_RELTM_START_INPUTMODEINVALID 0x20060005
#define VOS_ERRNO_SYSTIME_VALIDTIME_INPUTISNULL 0x20060006

#define VOS_ERRNO_RELTM_STOP_INPUTISNULL 0x2006000A
#define VOS_ERRNO_RELTM_STOP_TIMERINVALID 0x2006000B
#define VOS_ERRNO_RELTM_STOP_TIMERNOTSTART 0x2006000C
#define VOS_ERRNO_RELTM_STOP_STATUSERROR 0x2006000D
#define VOS_ERRNO_RELTM_FREE_RECEPTION 0x2006000E

#define VOS_ERRNO_RELTM_RESTART_INPUTISNULL 0x20060010
#define VOS_ERRNO_RELTM_RESTART_TIMERINVALID 0x20060011

#define VOS_ERRNO_RELTM_QUERY_PTICKNULL 0x20060082
#define VOS_ERRNO_RELTM_QUERY_TIMERNULL 0x20060083
#define VOS_ERRNO_RELTM_QUERY_TIMERSTOP 0x20060084
#define VOS_ERRNO_RELTM_QUERY_TIMERDRVNULL 0x20060085
#define VOS_ERRNO_RELTM_QUERY_TIMERNOTEXIST_INZERO 0x20060086
#define VOS_ERRNO_RELTM_QUERY_TIMERNOTEXIST_INSORT 0x20060087

#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA1ISNULL 0x20600056
#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA2ISNULL 0x20600057
#define VOS_ERRNO_SYSTIME_CMPDATE_INPUTDATE1INVALID 0x20600058
#define VOS_ERRNO_SYSTIME_CMPDATE_INPUTDATE2INVALID 0x20600060
#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA3ISNULL 0x20600061
#define VOS_ERRNO_SYSTIME_CALSEC_DATAOVERFLOW 0x20600062
#define VOS_ERRNO_SYSTIME_CALSEC_CALDAYERROR 0x20600063
#define VOS_ERRNO_SYSTIME_CALSEC_INPUT1LESSTHAN2 0x20600064

#define VOS_ERRNO_SYSTIME_GETSYST_INPUTPARAISNULL 0x20160085

#define VOS_ERRNO_SYSTIME_TK2MS_MSECPERTICKISZERO 0x20600035
#define VOS_ERRNO_SYSTIME_TK2MS_INPUTTICKISTOOBIG 0x20600036

#define VOS_ERRNO_SYSTIME_S2TIME_INPUTPARA2ISNULL 0x20600093
#define VOS_ERRNO_SYSTIME_S2TIME_INPUTPARA3ISNULL 0x20600094

#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA1ISNULL 0x20600070
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA2ISNULL 0x20600071
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA4ISNULL 0x20600072
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA5ISNULL 0x20600073
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTDATEINVALID 0x20600074
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTTIMEINVALID 0x20600075
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTMILLSECSINVALID 0x20600076

#define VOS_ERRNO_DRXTIME_START_INPUTISNULL 0x2060f000
#define VOS_ERRNO_DRXTIME_STOP_INPUTISNULL 0x2060f001
#define VOS_ERRNO_DRXTIME_START_STOP_FAIL 0x2060f002
#define VOS_ERRNO_DRXTIME_START_MSGNOTINSTALL 0x2060f003
#define VOS_ERRNO_DRXTIME_ERROR_TIMERHANDLE 0x2060f004
#define VOS_ERRNO_DRXTIME_RESOURCE_INITFAIL 0x2060f005
#define VOS_ERRNO_DRXTIME_TASK_INITFAIL 0x2060f006

#define VOS_ERRNO_BIT64TIME_START_STOP_FAIL 0x2060f011
#define VOS_ERRNO_BIT64TIME_START_MSGNOTINSTALL 0x2060f012
#define VOS_ERRNO_BIT64TIME_ERROR_TIMERNOUSED 0x2060f013
#define VOS_ERRNO_BIT64TIME_RESOURCE_INITFAIL 0x2060f014
#define VOS_ERRNO_BIT64TIME_TASK_INITFAIL 0x2060f015

#define VOS_ERRNO_SYSTIMER_FULL 0x2060FFFF

#define ELAPESD_TIME_INVAILD 0xFFFFFFFF

typedef struct {
    VOS_UINT32 startCount;             /* ������ӿ���ʱ������ */
    VOS_UINT32 startSlice;             /* ������ӿ���ʱ��ʱ�� */
    VOS_UINT32 stopCount;              /* ������ӿ�ͣ��ʱ������ */
    VOS_UINT32 expireCount;            /* �յ���ʱ���жϴ��� */
    VOS_UINT32 startErrCount;          /* ��������ʱ���ӿڷ��ش������ */
    VOS_UINT32 stopErrCount;           /* ������ͣ��ʱ���ӿڷ��ش������ */
    VOS_UINT32 elapsedErrCount;        /* �������ȡʣ��ʱ��ӿڴ��� */
    VOS_UINT32 elapsedContentErrCount; /* �������ȡʣ��ʱ��ӿڷ��ش������ */
    VOS_UINT32 elapsedContentErrSlice; /* �������ȡʣ��ʱ��ӿڷ��ش���ʱ�� */
    VOS_UINT32 expiredShortErrCount;   /* SOC Timer��ʱʱ��С������ʱ����� */
    VOS_UINT32 expiredShortErrSlice;   /* SOC Timer��ʱʱ��С������ʱ��ʱ�� */
    VOS_UINT32 expiredSendErrCount;    /* SOC Timer������Ϣʧ�ܴ��� */
    VOS_UINT32 expiredSendErrSlice;    /* SOC Timer������Ϣʧ��ʱ�� */
    VOS_UINT32 expiredLongErrCount;    /* SOC Timer��ʱʱ���������ʱ����� */
    VOS_UINT32 expiredLongErrSlice;    /* SOC Timer��ʱʱ���������ʱ��ʱ�� */
    VOS_UINT32 bit64TimerStartCount;
    VOS_UINT32 bit64TimerStopCount;
    VOS_UINT32 bit64TimerStartErrCount;
    VOS_UINT32 bit64TimerStopErrCount;
} VOS_TimerSocTimerInfo;

typedef struct {
    VOS_UINT32 action; /* the value of start;exp;0XFFFFFFFF->stop 0xFFFFFFFE->expire */
    VOS_UINT32 slice;
    VOS_UINT32 coreId;
} RTC_SocTimerDebugInfo;

/* the Max length of timer unit is ms -> 18 hours */
#define VOS_TIMER_MAX_LENGTH 64800000

/* ��ǰ�汾��ʱ�ӷ����ݲ���Ҫ����32.000Kʱ��״̬��Ĭ��ʹ��ԭ��32.768Kʱ������ */
#define RTC_CYCLE_LENGTH 32.768

#define VOS_RELTIMER_NOLOOP 0x0
#define VOS_RELTIMER_LOOP 1
#define VOS_RELTIMER_CALLBACK 2
#define VOS_DELTM_ON_TIMEOUTMSG 4

typedef VOS_VOID (*REL_TimerFunc)(VOS_UINT32 param, VOS_UINT32 timerName);

typedef int *HTIMER;

typedef struct REL_TimerMsg {
    VOS_MSG_HEADER       /* Structure of message packet header, where senderPid equals to VOS_PID_TIMER. */
    VOS_UINT8  type;  /* Type of timing message packet, being VOS_RELATIVE_TIMER. */
    VOS_UINT8  valid; /* Whether the msg is valid, if invalid, The value is VOS_TM_INVALID_MSG */
    VOS_UINT8  reserved[2]; /* Reserved for align */
    VOS_UINT32 name;        /* Timer name, input when the timer is started. */
    VOS_UINT32 para;        /* Timer parameter, input when the timer is started. */
    /* When a timer belongs to a timer group, this parameter indicates its serial number in this timer group.  */
    HTIMER hTm; /* Which timer sends this message */

    /* Double link, linking messages sent by the same timer */
    struct REL_TimerMsg *next;
    struct REL_TimerMsg *prev;

} REL_TimerMsgBlock;

enum {
    VOS_TIMER_OM_START,
    VOS_TIMER_OM_RESTART,
    VOS_TIMER_OM_STOP,
    VOS_TIMER_OM_BUTT
};

#define VOS_MSG_TM_SECOND 0xFF
#define VOS_MSG_TM_MINUTE 0xFE
#define VOS_MSG_TM_CHECK_START 0xFD
#define VOS_MSG_TM_CHECK_STOP 0xFC
#define VOS_TM_INVALID_MSG 0xBD
#define VOS_TM_CHECK_START 2 /* Start checking system at 2:00... */
#define VOS_TM_CHECK_STOP 3  /* ...and stop it at 3:00           */

#define VOS_TM_TYPE_DOPRA 0
#define VOS_TM_TYPE_VRPS 1
#define VOS_TM_TYPE_MAX 2

#define VOS_TM_LINK_ZERO 0
#define VOS_TM_LINK_SORTED 1

#define APP_PID 0xffffffff
extern CPU_TickEx   g_vosCpuTick;
extern VOS_UINT32   g_vosTimerTaskId;
extern VOS_UINT32   g_rtcTimerTaskId;
extern VOS_SPINLOCK g_vosTimerSpinLock;

enum VOS_TimerPrecision {
    VOS_TIMER_PRECISION_0   = 0,
    VOS_TIMER_PRECISION_5   = 5,
    VOS_TIMER_PRECISION_10  = 10,
    VOS_TIMER_PRECISION_20  = 20,
    VOS_TIMER_PRECISION_30  = 30,
    VOS_TIMER_PRECISION_40  = 40,
    VOS_TIMER_PRECISION_50  = 50,
    VOS_TIMER_PRECISION_60  = 60,
    VOS_TIMER_PRECISION_70  = 70,
    VOS_TIMER_PRECISION_80  = 80,
    VOS_TIMER_PRECISION_90  = 90,
    VOS_TIMER_PRECISION_100 = 100,
    VOS_TIMER_NO_PRECISION,
    VOS_TIMER_PRECISION_BUTT
};

typedef VOS_UINT32 VOS_TimerPrecisionUint32;

typedef struct {
    VOS_UINT8                action;
    VOS_UINT8                mode;
    VOS_UINT8                result;
    VOS_UINT8                type;
    VOS_PID                  pid;
    VOS_UINT32               length;
    VOS_UINT32               name;
    VOS_UINT32               param;
    VOS_TimerPrecisionUint32 precision;
    HTIMER                   hTimerAddress;
    VOS_UINT32               rTCAction; /* ��¼RTCʱ�ӵ�Action */
    VOS_UINT32               rTCSlice;  /* ��¼RTCʱ�ӵ�Slice */
    VOS_UINT32               coreId;    /* ��¼RTCʱ�����е�CoreId */
} VOS_TimerOmEvent;

#if (VOS_OSA_CPU == OSA_CPU_CCPU)

enum DRX_TimerWakeSrcVoteType {
    DRX_TIMER_WAKE_SRC_VOTE_SET_ALL = 0,
    DRX_TIMER_WAKE_SRC_VOTE_SET     = 1,
    DRX_TIMER_WAKE_SRC_VOTE_CLEAR   = 2,
    DRX_TIMER_WAKE_SRC_VOTE_BUTT
};
typedef VOS_UINT32 DRX_TimerWakeSrcVoteUint32;

typedef struct {
    DRX_TimerWakeSrcVoteUint32 voteType;
    ModemIdUint16       voteModem;
    VOS_UINT8                  reserved[2];
    VOS_RatModeUint32    voteMode;
    VOS_UINT32                 voteValue;
    VOS_UINT32                 slice;
} DRX_TimerWakeSrcVote;

#endif

/* the Max timer number supported by VOS */
/* VOS_TIMER_MESSAGE_NUM must be equal VOS_MAX_TIMER_NUMBER */
#define VOS_MAX_TIMER_NUMBER 250

#define VOS_UPPER_TIMER_NUMBER (VOS_MAX_TIMER_NUMBER / 10)

/* the Max RTC timer number supported by VOS */
#define RTC_MAX_TIMER_NUMBER 200

#define RTC_UPPER_TIMER_NUMBER (RTC_MAX_TIMER_NUMBER / 10)

#define DRX_TIMER_NOT_USED_FLAG (1)
#define DRX_TIMER_USED_FLAG (2)

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
#define DRX_TIMER_MAX_NUMBER (13)
#else
#define DRX_TIMER_MAX_NUMBER (7)
#endif
#define DRX_TIMER_TIMEOUT_INTERVAL (10 * 32768)

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
#define BIT64_TIMER_MAX_NUMBER (64)
#else
#define BIT64_TIMER_MAX_NUMBER (32)
#endif

#define BIT64_TIMER_NOT_USED_FLAG (1)
#define BIT64_TIMER_USED_FLAG (2)

VOS_UINT32 VOS_TimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 VOS_TimerTaskCreat(VOS_VOID);

VOS_UINT32 VOS_CheckTimer(HTIMER *phTm, VOS_UINT32 *timerID, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 V_Start26MRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                              VOS_UINT8 mode, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 V_Start26MCallBackRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                                      VOS_UINT8 mode, REL_TimerFunc timeOutRoutine, VOS_UINT32 fileId,
                                      VOS_INT32 lineNo);

/*
 * ����˵��: ���벢����һ��RTCʵʱ��ʱ��
 *           VOS_PID Pid    �������PID
 *           VOS_UINT32 length    ��ʱ��ʱ��,���0xFFFFFFFF,�����СʱΪ1193Сʱ
 *           VOS_UINT32 name      ��ʱ������
 *           VOS_UINT32 ulParam     ��ʱ������
 *           VOS_UINT8 mode       ��ʱ��ѭ��ģʽ
 *           VOS_RELTIMER_LOOP  -- start periodically
 *           VOS_RELTIMER_NOLOO -- start once time
 *           VOS_TimerPrecisionUint32 precision    ��ʱ������Ҫ�󣬵�λ0 - 100->0%- 100%
 *           ����ʱ������18Сʱ�ĳ���������Ĭ��Ϊ0���ȶ�ʱ��
 *           VOS_UINT32 ulFileID    �����ļ���
 *           VOS_INT32 lLineNo      �����к�
 *           HTIMER *phTm timer���
 *           VOS_OK ��ʱ�������ɹ�����ERRNO��ʾ����ʧ��
 * 1. ��    ��: 2018��5��23��
 *    �޸�����: ����CCPU/NCPU����ζ�ʱ��ʱ������18Сʱ����64λ��ʱ������֧��
 */
VOS_UINT32 V_StartRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                           VOS_UINT8 mode, VOS_TimerPrecisionUint32 precision, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 V_Stop26MRelTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo, VOS_TimerOmEvent *event);

#if ((!defined DMT) || (defined DMT_TIMER_ON))
#define VOS_StartRelTimer(phTm, Pid, length, name, param, mode, precision) \
    V_StartRelTimer((phTm), (Pid), (length), (name), (param), (mode), (precision), VOS_FILE_ID, __LINE__)
#endif

REL_TimerMsgBlock *VOS_TimerPreAllocMsg(VOS_PID pid);

VOS_UINT32 V_StopRelTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo);

#if ((!defined DMT) || (defined DMT_TIMER_ON))
#define VOS_StopRelTimer(phTm) V_StopRelTimer((phTm), VOS_FILE_ID, __LINE__)
#endif

#if (VOS_WIN32 == VOS_OS_VER)

#define VOS_StartDrxTimer(phTm, Pid, length, name, param)                                                  \
    V_StartRelTimer((phTm), (Pid), (length), (name), (param), VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION, \
                    VOS_FILE_ID, __LINE__)

#define VOS_StopDrxTimer(phTm) V_StopRelTimer((phTm), VOS_FILE_ID, __LINE__)

#else

/* Attention: DRX Timer is available just in CCPU. */
VOS_UINT32 V_StartDrxTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                           VOS_UINT32 fileId, VOS_INT32 lineNo);

#define VOS_StartDrxTimer(phTm, Pid, length, name, param) \
    V_StartDrxTimer((phTm), (Pid), (length), (name), (param), VOS_FILE_ID, __LINE__)

VOS_UINT32 V_StopDrxTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo);

#define VOS_StopDrxTimer(phTm) V_StopDrxTimer((phTm), VOS_FILE_ID, __LINE__)

#endif

VOS_UINT32 V_RestartRelTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo);

#define VOS_RestartRelTimer(phTm) V_RestartRelTimer((phTm), VOS_FILE_ID, __LINE__)

VOS_UINT32 V_GetRelTmRemainTime(HTIMER *phTm, VOS_UINT32 *tick, VOS_UINT32 fileId, VOS_INT32 lineNo);

#define VOS_GetRelTmRemainTime(phTm, pulTick) V_GetRelTmRemainTime((phTm), (pulTick), VOS_FILE_ID, __LINE__)

VOS_UINT32 VOS_GetTick(VOS_VOID);

/*
 * ����˵��: ����һ�����лص���RTC��ʱ��
 *           VOS_PID Pid    �������PID
 *           VOS_UINT32 length    ��ʱ��ʱ��,���VOS_TIMER_MAX_LENGTH(18Сʱ)��������󳤶�OSA����������λ
 *           VOS_UINT32 name      ��ʱ������
 *           VOS_UINT32 ulParam     ��ʱ������
 *           VOS_UINT8 mode       ��ʱ��ѭ��ģʽ
 *           VOS_RELTIMER_LOOP  -- start periodically
 *           VOS_RELTIMER_NOLOO -- start once time
 *           REL_TimerFunc timeOutRoutine �ص��ӿ�
 *           VOS_TimerPrecisionUint32 precision    ��ʱ������Ҫ�󣬵�λ0 - 100->0%- 100%
 *           VOS_UINT32 ulFileID    �����ļ���
 *           VOS_INT32 lLineNo      �����к�
 *           HTIMER *phTm timer���
 *           VOS_OK ��ʱ�������ɹ�����ERRNO��ʾ����ʧ��
 */
VOS_UINT32 V_StartCallBackRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                                   VOS_UINT8 mode, REL_TimerFunc timeOutRoutine, VOS_TimerPrecisionUint32 precision,
                                   VOS_UINT32 fileId, VOS_INT32 lineNo);

#define VOS_StartCallBackRelTimer(phTm, Pid, length, name, param, mode, timeOutRoutine, precision)           \
    V_StartCallBackRelTimer((phTm), (Pid), (length), (name), (param), (mode), (timeOutRoutine), (precision), \
                            VOS_FILE_ID, __LINE__)

VOS_UINT32 VOS_TmTickToMillSec(VOS_UINT32 ticks);

VOS_BOOL VOS_Is26MTimer(const HTIMER *phTm);

VOS_UINT32 VOS_Is26MTimerRunning(VOS_VOID);

/* RTC timer begin */
/* errno definiens */
#define VOS_RTC_ERRNO_RELTM_START_MSGNOTINSTALL 0x2fff0004
#define VOS_RTC_ERRNO_RELTM_START_INPUTMODEINVALID 0x2fff0005
#define VOS_RTC_ERRNO_SYSTIME_VALIDTIME_INPUTISNULL 0x2fff0006

#define VOS_RTC_ERRNO_RELTM_STOP_INPUTISNULL 0x2fff000A
#define VOS_RTC_ERRNO_RELTM_STOP_TIMERINVALID 0x2fff000B
#define VOS_RTC_ERRNO_RELTM_STOP_TIMERNOTSTART 0x2fff000C
#define VOS_RTC_ERRNO_RELTM_STOP_STATUSERROR 0x2fff000D
#define VOS_RTC_ERRNO_RELTM_FREE_RECEPTION 0x2fff000E

#define VOS_RTC_ERRNO_SYSTIMER_FULL 0x2060FFFF

/*
 * ����˵��: ͨ�����ӿ��ϱ���RTC��Ϣ�ṹ��
 * 1. ��    ��: 2017��5��10��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT8  value[4];
} VOS_RtcOmInfo;

#ifndef FLOAT_SUPPORT
VOS_UINT32 RTC_MUL_32_DOT_768(VOS_UINT32 value, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 RTC_DIV_32_DOT_768(VOS_UINT32 value, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 RTC_MUL_DOT_32768(VOS_UINT32 value, VOS_UINT32 fileId, VOS_INT32 lineNo);
#endif

VOS_UINT32 RTC_TimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 RTC_TimerTaskCreat(VOS_VOID);

VOS_UINT32 R_Stop32KTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo, VOS_TimerOmEvent *event);

#define RTC_StopTimer(phTm) V_StopRelTimer((phTm), VOS_FILE_ID, __LINE__)

VOS_UINT32 R_Get32KRelTmRemainTime(HTIMER *phTm, VOS_UINT32 *vosTime, VOS_UINT32 fileId, VOS_INT32 lineNo);

#define RTC_GetRelTmRemainTime(phTm, pulTime) R_Get32KRelTmRemainTime((phTm), (pulTime), VOS_FILE_ID, __LINE__)

VOS_UINT32 V_Start32KCallBackRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                                      VOS_UINT8 mode, REL_TimerFunc timeOutRoutine, VOS_UINT32 precision,
                                      VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 V_Start32KRelTimer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                              VOS_UINT8 mode, VOS_UINT32 precision, VOS_UINT32 fileId, VOS_INT32 lineNo);

#define RTC_StartTimer(phTm, Pid, length, name, ulParam, mode, timeOutRoutine)                                 \
    V_StartCallBackRelTimer((phTm), (Pid), (length), (name), (ulParam), (mode), (timeOutRoutine), VOS_FILE_ID, \
                            __LINE__)

VOS_UINT32 RTC_timer_running(VOS_VOID);

VOS_UINT32 RTC_CheckTimer(HTIMER *phTm, VOS_UINT32 *timerID, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 R_Restart32KRelTimer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo);

VOS_UINT32 VOS_GetFirst32KTimer(VOS_VOID);

VOS_UINT32 VOS_GetSlice(VOS_VOID);

VOS_UINT64 VOS_Get64BitSlice(VOS_VOID);

VOS_UINT32 VOS_GetSliceUnit(VOS_VOID);

VOS_BOOL VOS_CalcTimerInfo(VOS_VOID);

VOS_BOOL RTC_CalcTimerInfo(VOS_VOID);

#if (VOS_WIN32 == VOS_OS_VER)
VOS_VOID VOS_MagnifyTimerLength(VOS_UINT32 ratio);
#endif

VOS_VOID RTC_GetDebugSocInfo(VOS_UINT32 *action, VOS_UINT32 *slice, VOS_UINT32 *coreId);

VOS_VOID RTC_ReportOmInfo(VOS_VOID);

VOS_UINT32 VOS_DrxTimerCtrlBlkInit(VOS_VOID);
VOS_UINT32 VOS_DrxTimerTaskCreat(VOS_VOID);

#if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (OSA_CPU_NRCPU == VOS_OSA_CPU)) && (FEATURE_ON == FEATURE_VOS_18H_TIMER)
VOS_UINT32 VOS_Bit64RtcTimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 VOS_Bit64TimerTaskCreat(VOS_VOID);

VOS_UINT32 VOS_StartBit64Timer(HTIMER *phTm, VOS_PID pid, VOS_UINT32 length, VOS_UINT32 name, VOS_UINT32 param,
                               VOS_UINT32 fileId, VOS_INT32 lineNo);
VOS_UINT32 VOS_StopBit64Timer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo, VOS_TimerOmEvent *event);
VOS_UINT32 VOS_Bit64TimerCheckTimer(const HTIMER *phTm);
VOS_UINT32 VOS_RestartBit64Timer(HTIMER *phTm, VOS_UINT32 fileId, VOS_INT32 lineNo);
VOS_UINT32 VOS_GetBit64RemainTime(HTIMER *phTm, VOS_UINT32 *vosTime, VOS_UINT32 fileId, VOS_INT32 lineNo);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _VOS_TIMER_H */
