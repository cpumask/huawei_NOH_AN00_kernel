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

#ifndef __OSL_MALLOC_H
#define __OSL_MALLOC_H

#include "osl_common.h"


#ifdef __KERNEL__
#include <linux/slab.h>


static inline void* osl_malloc(u32 size)
{
    return kmalloc(size, GFP_KERNEL);
}

static inline void osl_free(const void *p)
{
    return kfree(p);
}

#elif  defined(__OS_RTOSCK__) || defined(__OS_RTOSCK_SMP__)
#include "sre_mem.h"
#include <mdrv_malloc.h>
#include <osl_printf.h>
#ifndef ROUND_UP
#define ROUND_UP(uwAddr, uwBoundary)  (((uwAddr) + (uwBoundary) - 1) & ~((uwBoundary) - 1))
#endif
#ifndef  ALIGN
#define  ALIGN(x, a)                  __ALIGN_KERNEL((x), (a))
#define  __ALIGN_KERNEL(x, a)         __ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define  __ALIGN_KERNEL_MASK(x, mask) (((x) + (mask)) & ~(mask))
#endif
#if defined(__OS_RTOSCK_SMP__) 
#define UNCACHE_MEM_ALIGN_BYTE MEM_ADDR_ALIGN_016
#else
#define UNCACHE_MEM_ALIGN_BYTE MEM_ADDR_ALIGN_4K
#endif
static inline void* osl_malloc(unsigned int nBytes)
{
#if defined(__OS_RTOSCK_SMP__)
    u32 cookis;
    __asm__ __volatile__("mov %0,pc":"=r"(cookis)::"memory");
    return SRE_MemAllocDebug(0,0,nBytes,cookis); /*lint !e530*/
#else
    return SRE_MemCacheAlloc(nBytes, (enum MemAlign)MEM_ADDR_ALIGN_004);
#endif
}

static inline void osl_free(void *objp)
{
    u32 ret = 0;
    ret = SRE_MemCacheFree(objp);
    if(SRE_OK == ret) {
        return;
    } else {
        (void)osl_printf("SRE_MemCacheFree failed,ret=0x%x\n", ret);
    }
}
static inline int osl_cachedma_free(void *ptr)
{
    return (int)SRE_MemUncacheFree(ptr);
}
static inline void *osl_cachedma_malloc (unsigned int nBytes)
{
#if defined(__OS_RTOSCK_SMP__)
    u32 cookis;
    __asm__ __volatile__("mov %0,pc":"=r"(cookis)::"memory");
    return SRE_MemAllocAlignDebug(0, 1, nBytes, (enum MemAlign)UNCACHE_MEM_ALIGN_BYTE, cookis); /*lint !e530*/
#else
    return SRE_MemUncacheAlloc(nBytes, (enum MemAlign)UNCACHE_MEM_ALIGN_BYTE);
#endif
}

static inline void *osl_cachedma_malloc_debug(unsigned int nBytes, u32 cookis)
{
#if defined(__OS_RTOSCK_SMP__) 
    return SRE_MemAllocAlignDebug(0, 1, nBytes, (enum MemAlign)UNCACHE_MEM_ALIGN_BYTE, cookis);
#else
    return SRE_MemUncacheAlloc(nBytes, (enum MemAlign)UNCACHE_MEM_ALIGN_BYTE);
#endif
}

static inline void *osl_cachedma_malloc_align(unsigned int align_size, unsigned int nBytes)
{
    int count = 2;
    unsigned int align = align_size >> 2;
#if defined(__OS_RTOSCK_SMP__)
    u32 cookis;
    __asm__ __volatile__("mov %0,pc":"=r"(cookis)::"memory");
#endif
    while((align >> 1) > 0) {
        count ++ ;
        align = align >> 1;
    }
    if(align > MEM_ADDR_ALIGN_16K) {
        return NULL;
    }
#if defined(__OS_RTOSCK_SMP__)
    return SRE_MemAllocAlignDebug(0, 1, nBytes, (enum MemAlign)count, cookis); /*lint !e530*/
#else
    return SRE_MemUncacheAlloc(nBytes, (enum MemAlign)count);
#endif
}
static inline void *osl_mem_align(unsigned int align_size, unsigned int nBytes)
{
    int count = 2;
    unsigned int align = align_size >> 2;
    #if defined(__OS_RTOSCK_SMP__)
    u32 cookis;
    __asm__ __volatile__("mov %0,pc":"=r"(cookis)::"memory");
    #endif
    while((align >> 1) > 0) {
        count ++ ;
        align = align >> 1;
    }
    if(align > MEM_ADDR_ALIGN_16K) {
        return NULL;
    }
#if defined(__OS_RTOSCK_SMP__)
    return SRE_MemAllocAlignDebug(0, 0, nBytes, (enum MemAlign)count, cookis); /*lint !e530*/
#else
    return SRE_MemCacheAlloc(nBytes, (enum MemAlign)count);
#endif
}

void *osl_reserved_mem_malloc(u32 size, enum malloc_reserved_req_id request_id);
void *osl_malloc_virt_to_phy(void *cpu_addr);

#elif defined(__CMSIS_RTOS) || defined(__M3_OS_RTOSCK__)
#include "osl_common.h"


static __inline__ void* osl_malloc(u32 size)
{
    if(!size) {
        return NULL;
    } else {
        return malloc(size);
    }
}

#define osl_free(objp)  free(objp)

#endif /* __KERNEL__ */

#endif

