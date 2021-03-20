/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, 
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

#ifndef PS_COMMON_DEF_H
#define PS_COMMON_DEF_H

#include "vos.h"  /*lint !e537*/
#include "v_timer.h" /*lint !e537*/

#ifdef _lint
/* PCLINT������뿪�أ�ֻ��PCLINTʱ�� */
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#endif

#ifdef __RECUR_TEST__
#include "RecurTest.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define PS_SUBMOD_NULL 0

#ifndef L2_LIBFUZZ_TEST
#define PS_PRINTF (VOS_VOID)vos_printf
#else
#define PS_PRINTF
#endif

/* CACHE TYPE, copy from BSP */
typedef enum { 
    OSAL_INSTRUCTION_CACHE,
    OSAL_DATA_CACHE
} OSAL_CACHE_TYPE;

extern int OSAL_CacheFlush(OSAL_CACHE_TYPE type, void* address, unsigned int bytes);

/* Э��ջ��־��ӡ�½ӿ� */
#ifndef L2_LIBFUZZ_TEST
#define PS_PRINTF_FATAL(fmt, ...) (mdrv_fatal(fmt, ##__VA_ARGS__))
#define PS_PRINTF_ERR(fmt, ...) (mdrv_err(fmt, ##__VA_ARGS__))
#define PS_PRINTF_WARNING(fmt, ...) (mdrv_wrn(fmt, ##__VA_ARGS__))
#define PS_PRINTF_INFO(fmt, ...) (mdrv_info(fmt, ##__VA_ARGS__))
#define PS_PRINTF_DEBUG(fmt, ...) (mdrv_debug(fmt, ##__VA_ARGS__))
#else
#define PS_PRINTF_FATAL(fmt, ...)
#define PS_PRINTF_ERR(fmt, ...)
#define PS_PRINTF_WARNING(fmt, ...)
#define PS_PRINTF_INFO(fmt, ...)
#define PS_PRINTF_DEBUG(fmt, ...)
#endif

/* �ڴ������װ */
#ifdef _lint

#define PS_MEM_CMP(destBuffer, srcBuffer, count) memcmp(destBuffer, srcBuffer, count)

/* ������Ϣ��,����ĳ��Ȱ�����Ϣ��ͷ���� */
/*lint -emacro({586}, PS_ALLOC_MSG_WITH_HEADER_LEN)*/
#define PS_ALLOC_MSG_WITH_HEADER_LEN(pid, len) malloc(len)

/* ������Ϣ��,����ĳ��Ȳ�������Ϣ��ͷ���� */
/*lint -emacro({586}, PS_ALLOC_MSG)*/
#define PS_ALLOC_MSG(pid, len) malloc((len) + VOS_MSG_HEAD_LENGTH)

/* ��Ϣ���� */
#ifdef LINUX_PC_LINT
#define PS_SEND_MSG(pid, msg) (free((VOS_VOID*)msg), pid)
#define PS_CHR_RPT_SEND_MSG(pid, msg) (free((VOS_VOID*)msg), pid)

#else
/*lint -emacro({58}, PS_SEND_MSG)*/
/*lint -emacro({64}, PS_SEND_MSG)*/
/*lint -emacro({144}, PS_SEND_MSG)*/
/*lint -emacro({586}, PS_SEND_MSG)*/
/*lint -emacro({516}, PS_SEND_MSG)*/
#define PS_SEND_MSG(pid, msg) free(msg)

/*lint -emacro({64}, PS_CHR_RPT_SEND_MSG)*/
/*lint -emacro({144}, PS_CHR_RPT_SEND_MSG)*/
#define PS_CHR_RPT_SEND_MSG(pid, msg) free(msg)
#endif

/* ��Ϣ����,����PS_POST_MSG֮��,��Ҫʹ������ʾ���ͷ���Ϣ��,������ﲻ����ת����Ϊfree */
#define PS_POST_MSG(pid, msg) VOS_PostMsg(pid, msg)

/* ��Ϣ���� */
#ifdef LINUX_PC_LINT
#define PS_SEND_URGENT_MSG(pid, msg) (free(msg), pid)

#else
/*lint -emacro({58}, PS_SEND_URGENT_MSG)*/
/*lint -emacro({64}, PS_SEND_URGENT_MSG)*/
/*lint -emacro({144}, PS_SEND_URGENT_MSG)*/
/*lint -emacro({586}, PS_SEND_URGENT_MSG)*/
#define PS_SEND_URGENT_MSG(pid, msg) free(msg)

#endif

/* ��Ϣ�ͷź궨�� */
#ifdef LINUX_PC_LINT
#define PS_FREE_MSG(pid, msg) (free(msg), pid)

#else
/*lint -emacro({58}, PS_FREE_MSG)*/
/*lint -emacro({64}, PS_FREE_MSG)*/
/*lint -emacro({144}, PS_FREE_MSG)*/
/*lint -emacro({586}, PS_FREE_MSG)*/
#define PS_FREE_MSG(pid, msg) free(msg)

#endif

/* �ڴ�����궨�� */
/*lint -emacro({586}, PS_MEM_ALLOC)*/
#define PS_MEM_ALLOC(pid, size) malloc(size)

/* �ڴ��ͷź궨�� */
#ifdef LINUX_PC_LINT
#define PS_MEM_FREE(pid, addr) (free(addr), pid)

#else
/*lint -emacro({58}, PS_MEM_FREE)*/
/*lint -emacro({64}, PS_MEM_FREE)*/
/*lint -emacro({144}, PS_MEM_FREE)*/
/*lint -emacro({586}, PS_MEM_FREE)*/
#define PS_MEM_FREE(pid, addr) free(addr)

#endif

/* ���뾲̬�ڴ�궨�� */
/*lint -emacro({586}, PS_ALLOC_STATIC_MEM)*/
#define PS_ALLOC_STATIC_MEM(pid, size) malloc(size)

/* �ͷž�̬�ڴ�궨�� */
#ifdef LINUX_PC_LINT
#define PS_FREE_STATIC_MEM(pid, addr) (free(addr), pid)

#else
/*lint -emacro({58}, PS_FREE_STATIC_MEM)*/
/*lint -emacro({64}, PS_FREE_STATIC_MEM)*/
/*lint -emacro({144}, PS_FREE_STATIC_MEM)*/
/*lint -emacro({586}, PS_FREE_STATIC_MEM)*/
#define PS_FREE_STATIC_MEM(pid, addr) free(addr)

#endif

/* �ڴ����� */
#define PS_ALLOC_MSG_ALL_CHECK(pid, len) VOS_AllocMsg(pid, (len) - (VOS_MSG_HEAD_LENGTH))
#elif defined(PS_UT_SWITCH) || defined(_GAS_UT_SWITCH_)
#include "stdlib.h"

/*lint -emacro({586}, PS_ALLOC_MSG)*/
#define PS_ALLOC_MSG(pid, len) malloc((len) + VOS_MSG_HEAD_LENGTH)

/*lint -emacro({586}, PS_ALLOC_MSG_WITH_HEADER_LEN)*/
#define PS_ALLOC_MSG_WITH_HEADER_LEN(pid, len) malloc(len)
#define PS_POST_MSG(pid, msg) VOS_PostMsg(pid, msg)

/*lint -emacro({586}, PS_SEND_MSG)*/
/*lint -emacro({516}, PS_SEND_MSG)*/
#define PS_SEND_MSG(pid, msg) free(msg)
#define PS_CHR_RPT_SEND_MSG(pid, msg) free(msg)
/*lint -emacro({586}, PS_MEM_FREE)*/
#define PS_MEM_FREE(pid, addr) free(addr)
/*lint -emacro({586}, PS_MEM_ALLOC)*/
#define PS_MEM_ALLOC(pid, size) malloc(size)
/*lint -emacro({586}, PS_FREE_MSG)*/
#define PS_FREE_MSG(pid, msg) free(msg)
/*lint -emacro({586}, PS_FREE_STATIC_MEM)*/
#define PS_FREE_STATIC_MEM(pid, addr) free(addr)
#define PS_MEM_CMP(destBuffer, srcBuffer, count) memcmp(destBuffer, srcBuffer, count)
#else

#define PS_MEM_CMP(destBuffer, srcBuffer, bufferLen) VOS_MemCmp(destBuffer, srcBuffer, bufferLen)

/* ������Ϣ��,����ĳ��Ȱ�����Ϣ��ͷ���� */
/*lint -emacro({586}, PS_ALLOC_MSG_WITH_HEADER_LEN)*/
#define PS_ALLOC_MSG_WITH_HEADER_LEN(pid, len) VOS_AllocMsg(pid, (len) - (VOS_MSG_HEAD_LENGTH))

/* ������Ϣ��,����ĳ��Ȳ�������Ϣ��ͷ���� */
/*lint -emacro({586}, PS_ALLOC_MSG)*/
#define PS_ALLOC_MSG(pid, len) VOS_AllocMsg(pid, len)

/*lint -emacro({586}, PS_ALLOC_MSG_ALL_CHECK) */
#define PS_ALLOC_MSG_ALL_CHECK(pid, len) VOS_AllocMsg(pid, (len) - (VOS_MSG_HEAD_LENGTH))

/*
 * Ŀǰ,Э��ջOSA����֧��WIN32����ϵͳ,��PC�����ϲ���ʱ����Ҫʹ��miniDOPRA,���
 * ���������ｫPS_SEND_MSG�ֱ���,����WIN32��VXWORKS�汾,����WIN32�汾,��ʹ��ԭ
 * Ps_SendMsg����,�Ա�����PC STUB׮ת����Ϣ.����OSA֧��WIN32�汾��,�����ͳһ����
 */
#if (VOS_OS_VER == VOS_WIN32)

/* ��Ϣ���� */
#ifdef __RECUR_TEST__

#define PS_SEND_MSG(pid, msg) RECRUTEST_PsSendMsg(pid, msg)

#elif defined(LIBFUZZER_LRRC)

#define PS_SEND_MSG(pid, msg) VOS_FreeMsg(pid, msg)

#elif defined(PS_ITT_PC_TEST_LTEV)

#define PS_SEND_MSG(pid, msg) VOS_SendMsg(pid, msg)

#elif defined(PS_ITT_PC_TEST)

#define PS_SEND_MSG(pid, msg) Ps_Itt_Stub_SendMsg(pid, msg)

#elif defined(PS_ITT_PC_TEST_L2)
#define PS_SEND_MSG(pid, msg) VOS_SendMsg(pid, msg)
#else

VOS_UINT32 Ps_SendMsg(VOS_INT8* fileName, VOS_UINT32 line, VOS_PID pid, VOS_VOID* msg);

#define PS_SEND_MSG(pid, msg) Ps_SendMsg((VOS_INT8*)__FILE__, __LINE__, pid, msg)

#define PS_CHR_RPT_SEND_MSG(pid, msg) Ps_SendMsg((VOS_INT8*)__FILE__, __LINE__, pid, msg)
#endif
#else
/* ��Ϣ���� */
/*lint --e(683)*/
#define PS_SEND_MSG(pid, msg) VOS_SendMsg(pid, msg)

#define PS_CHR_RPT_SEND_MSG(pid, msg) mdrv_chr_msgreport(msg)
#endif

/* ��Ϣ���� */
#define PS_POST_MSG(pid, msg) VOS_PostMsg(pid, msg)

/* ��Ϣ���� */
#define PS_SEND_URGENT_MSG(pid, msg) VOS_SendUrgentMsg(pid, msg)

/* ��Ϣ�ͷź궨�� */
#define PS_FREE_MSG(pid, msg) VOS_FreeMsg(pid, msg)

/* �ڴ�����궨�� */
#if (VOS_OS_VER != VOS_WIN32)

/*lint -emacro({586}, PS_MEM_ALLOC)*/
#define PS_MEM_ALLOC(pid, size) VOS_MemAlloc(pid, (DYNAMIC_MEM_PT), size)

#else /* WIN32 DOPRA 1.6.1�汾DYNAMIC_MEM_PT�ڴ�����㷨����BUG����˻���BLOCK_MEM_PT */
/*lint -emacro({586}, PS_MEM_ALLOC)*/
#define PS_MEM_ALLOC(pid, size) VOS_MemAlloc(pid, (BLOCK_MEM_PT), size)
#endif

/* �ڴ��ͷź궨�� */
/*lint -emacro({586}, PS_MEM_FREE)*/
#define PS_MEM_FREE(pid, addr) VOS_MemFree(pid, addr)

/* ���뾲̬�ڴ�궨�� */
/*lint -emacro({586}, PS_ALLOC_STATIC_MEM)*/
#define PS_ALLOC_STATIC_MEM(pid, size) VOS_MemAlloc(pid, (STATIC_MEM_PT), size)

/* �ͷž�̬�ڴ�궨�� */
/*lint -emacro({586}, PS_FREE_STATIC_MEM)*/
#define PS_FREE_STATIC_MEM(pid, addr) VOS_MemFree(pid, (addr))
#endif

/*
 * ������ʱ��
 * Input Parameters
 * Pid: The function module ID of application.
 * ulLength:The duration of relative timer, in milliseconds.
 * ulName:Timer name. When the timer times out, this parameter will be passed to the application.
 * ulParam:Additional parameter. When the timer times out,
 * this parameter will be passed to the application.
 * ucMode: Work mode of relative timer:
 * VOS_RELTIMER_LOOP (loop);
 * VOS_RELTIMER_NOLOOP (no loop).
 * Output Parameters
 * tm: The relative timer provided to the application by the system.
 * When tm is VOS_NULL_PTR, ucMode is not allowed to be VOS_RELTIMER_LOOP.
 */

#if (VOS_OS_VER != VOS_WIN32)

#define PS_START_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_PRECISION_0)

#define PS_START_NO_PRECISION_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_NO_PRECISION)

#else

#ifndef DMT
#if defined(__PS_WIN32_RECUR__) || defined(__UT_CENTER__)
#define PS_START_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_PRECISION_0)
#define PS_START_NO_PRECISION_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_NO_PRECISION)
#else
#define PS_START_REL_TIMER(tm, pid, length, name, param, mode) VOS_StartRelTimer(tm, pid, length, name, param, mode)
#define PS_START_NO_PRECISION_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode)
#endif
#else
#define PS_START_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_PRECISION_0)
#define PS_START_NO_PRECISION_REL_TIMER(tm, pid, length, name, param, mode) \
    VOS_StartRelTimer(tm, pid, length, name, param, mode, VOS_TIMER_NO_PRECISION)
#endif

#endif

#define PS_START_CALLBACK_REL_TIMER(tm, pid, length, name, param, mode, timeOutRoutine, precision) \
    VOS_StartCallBackRelTimer(tm, pid, length, name, param, mode, timeOutRoutine, precision)

#define PS_STOP_REL_TIMER(tm) VOS_StopRelTimer(tm)

#define PS_GET_REL_TIMER_NAME(msg) (((REL_TimerMsgBlock*)msg)->name)

#define PS_GET_REL_TIMER_PARA(msg) (((REL_TimerMsgBlock*)msg)->para)

#define PS_RESTART_REL_TIMER(tm) VOS_RestartRelTimer(tm)

/* ��װ���Ӻ���ע�ắ�� */
#define PS_REGISTER_MSG_GET_HOOK(pfnMsgHook) VOS_RegisterMsgGetHook(pfnMsgHook)

#if (VOS_WIN32 != VOS_OS_VER)
#ifdef _lint
#define PS_SND_MSG_ALL_CHECK(pid, msg) PS_SEND_MSG(pid, msg)
#else

#define PS_SND_MSG_ALL_CHECK(pid, msg) PS_OM_SendMsg(pid, msg)
#endif
#else
#define PS_SND_MSG_ALL_CHECK(pid, msg) PS_SEND_MSG(pid, msg)
#endif

/* ���ͽ�����Ϣ */
#define PS_SND_URGENT_MSG(pid, msg) VOS_SendUrgentMsg(pid, msg)

#define PS_MEM_ALLOC_All_CHECK(pid, size) PS_MEM_ALLOC(pid, size)

/* �ڴ��ͷź궨�� */
#define PS_MEM_FREE_ALL_CHECK(pid, addr) PS_MEM_FREE(pid, addr)

#define PS_FREE_MSG_ALL_CHECK(pid, msg) PS_FREE_MSG(pid, msg)

/* flush��clear�ײ�ʵ��һ�� */
#if (VOS_WIN32 != VOS_OS_VER)
#if (VOS_VXWORKS == VOS_OS_VER)
#define LPS_CacheClear(dataAddr, dataLen) (VOS_VOID)cacheClear(DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#define LPS_CacheFlush(dataAddr, dataLen) (VOS_VOID)cacheFlush(DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#define LPS_CacheInvalidate(dataAddr, dataLen) (VOS_VOID)cacheInvalidate(DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#else
#define LPS_CacheClear(dataAddr, dataLen) (VOS_VOID)OSAL_CacheFlush(OSAL_DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#define LPS_CacheFlush(dataAddr, dataLen) (VOS_VOID)OSAL_CacheFlush(OSAL_DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#define LPS_CacheInvalidate(dataAddr, dataLen) \
    (VOS_VOID)OSAL_CacheInvalid(OSAL_DATA_CACHE, (VOS_VOID*)(dataAddr), (dataLen))
#endif

/* ���Ӻ������: �ֽڶ��䷽ʽenAlignPow */
#if (VOS_OS_VER == VOS_RTOSCK)
#define LPS_CacheDmaMalloc(size, alignPow, dataAddr, cookie) VOS_UnCacheMemAllocDebug(size, alignPow, dataAddr, cookie)
#define LPS_CacheDmaFree(buf, size) VOS_UnCacheMemFree(buf, buf, size)
#else
#define LPS_CacheDmaMalloc(size, alignPow, dataAddr, cookie) VOS_UnCacheMemAllocDebug(size, alignPow, dataAddr, cookie)
#define LPS_CacheDmaFree(buf, size) VOS_UnCacheMemFree(buf, buf, size)
#endif

#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#if (VOS_OS_VER == VOS_WIN32)
#define STATIC
#else
#define STATIC static
#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
