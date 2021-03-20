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

#ifndef __OSL_BIO_H
#define __OSL_BIO_H
#include <product_config.h>
#ifdef __KERNEL__
#ifndef __ASSEMBLY__
#include <linux/io.h>
#include <asm/barrier.h>
static __inline__  void cache_sync(void)
{
	mb();
}
#endif

#elif defined(__OS_RTOSCK__) ||defined(__OS_RTOSCK_SMP__) && !defined(__ASSEMBLY__)
#include <sre_cache.h>

#define isb() __asm__ __volatile__ ("isb" : : : "memory")
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory")
#ifdef CONFIG_BALONG_L2CACHE
#define mb()	do { dsb(); SRE_L2CacheWait(); } while (0)
#else
#define mb()	do { dsb(); } while (0)
#endif
#define write_sync() mb()
#define read_sync()

#define rmb()	dmb()
#define wmb()	mb()
static inline void WFI(void)
{
	__asm__ __volatile__ ( "dsb;" );/*lint !e123*/
    __asm__ __volatile__ ( "isb;");/*lint !e123*/
	__asm__ __volatile__ ( "wfi;");/*lint !e123*/
	__asm__ __volatile__ ( "nop;");/*lint !e123*/
	__asm__ __volatile__ ( "nop;");/*lint !e123*/
}
//lint -esym(528,*)
/*lint -save -e528*/
 static __inline__ void DWB(void) /* drain write buffer */
{
    __asm__ __volatile__ ( "dsb;" );
}
static __inline__  void cache_sync(void)
{
	mb();
}
static __inline__ void writel_relaxed(unsigned val, void* addr)
{
    (*(volatile unsigned *) (addr)) = (val);
}
static inline void writew_relaxed(unsigned val, void* addr)
{
    (*(volatile unsigned short *) (addr)) = (unsigned short)(val);
}

static __inline__ void writeb_relaxed(unsigned val, void* addr)
{
    (*(volatile unsigned char *) (addr)) = (unsigned char)(val);
}

static __inline__ void writel(unsigned val, void* addr)
{
    write_sync();
    writel_relaxed(val, addr);
}

static inline void writew(unsigned val, void* addr)
{
    write_sync();
    writew_relaxed(val, addr);
}

static __inline__ void writeb(unsigned val, void* addr)
{
    write_sync();
    writeb_relaxed(val, addr);
}

static __inline__ unsigned readl_relaxed(void* addr)
{
    return (*(volatile unsigned *) (addr));
}
static inline unsigned readw_relaxed(void* addr)
{
    return (*(volatile unsigned short*) (addr));
}

static __inline__ unsigned readb_relaxed(void* addr)
{
    return (*(volatile unsigned char*)(addr));
}

static __inline__ unsigned readl(const void* addr)
{
    read_sync();
    return (*(const volatile unsigned *) (addr));
}

static inline unsigned readw(const void* addr)
{
    read_sync();
    return (*(volatile unsigned short*) (addr));
}

static __inline__ unsigned readb(const void* addr)
{
    read_sync();
    return (*(volatile unsigned char*)(addr));
}
//lint +esym(528,*)
/*lint -restore */
#endif/*__KERNEL__*/


#if defined(__CMSIS_RTOS) ||defined(__M3_OS_RTOSCK__) 
#define isb() __asm__ __volatile__ ("isb" : : : "memory")
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory")

#define mb()	dsb()
#define rmb()	dmb()
#define wmb()	mb()

 /* drain write buffer */
static inline void DWB(void)
{
}

static inline void ISB(void) /* drain write buffer */
{
	__asm__ __volatile__ ( "isb;");
}

static inline void WFI(void)
{
	__asm__ __volatile__ ( "wfi;");
}

static inline void writel(unsigned val, void* addr)
{
    DWB();
    (*(volatile unsigned *) (addr)) = (val);
}

static inline void writeb(unsigned val, void* addr)
{
    DWB();
    (*(volatile unsigned char *) (addr)) = (val);
}

static inline unsigned readl(void* addr)
{
    return (*(volatile unsigned *) (addr));
}

static inline unsigned readb(void* addr)
{
	return (*(volatile unsigned char*)(addr));
}

static __inline__  void cache_sync(void)
{
	return;
}
#endif/*__CMSIS_RTOS*/


#if defined(__OS_RTOSCK__)

#ifndef __ASSEMBLY__
#include <sre_mmu.h>
#include <product_config.h>
enum MMU_MAP_E
{
	MMU_MAP_CCORE_TEXT = 0,
	MMU_MAP_CCORE_DATA,
	MMU_MAP_CCORE_DTS,
	MMU_MAP_GIC,
	MMU_MAP_UART,
#ifdef CONFIG_BALONG_L2CACHE
	MMU_MAP_L2CACHE,
#endif
	MMU_MAP_MAX
};

#define MMU_STATEMASK_ALL (OS_MMU_STATEMASK_ACCESS|OS_MMU_STATEMASK_ATTRIBUTES|OS_MMU_STATEMASK_SHARE)
#define MMU_STATE_RWX (OS_MMU_STATE_READ|OS_MMU_STATE_WRITE|OS_MMU_STATE_EXE)
#define MMU_STATE_RW (OS_MMU_STATE_READ | OS_MMU_STATE_WRITE)
#define MMU_STATE_ROX (OS_MMU_STATE_READ|OS_MMU_STATE_EXE)

#endif/*__ASSEMBLY__*/

#endif	/* __OS_RTOSCK__ */
#if defined(__OS_RTOSCK_SMP__) 
#ifndef MMU_VA_T
#define MMU_VA_T void*
#endif
#ifndef MMU_PA_T
#define MMU_PA_T void*
#endif

#endif

#if defined(__OS_RTOSCK_SMP__) ||defined(__OS_RTOSCK__)
/* device: strongly-ordered �Ĵ�����Ҫʹ�øýӿ� */
void *ioremap(const MMU_PA_T phy_addr, unsigned int len);

/* device: non-cacheable normal */
void *ioremap_wc(const MMU_PA_T phy_addr, unsigned int len);

/* device: non-cacheable normal exe */
void *ioremap_wcx(const MMU_VA_T phy_addr, unsigned int len);

/* memory: normal */
void *ioremap_memory(const MMU_PA_T phy_addr, unsigned int len);

int ioremap_wc_smp(void* phy_addr, unsigned int len,void* virt_addr);
int ioremap_memory_smp(void* phy_addr, unsigned int len,void* virt_addr);
int iounmap_smp(unsigned int region_id,void* virt_addr, unsigned int len);

#endif

#if defined (__KERNEL__)||defined(__CMSIS_RTOS)||defined(__M3_OS_RTOSCK__) || defined(__OS_RTOSCK__)||defined(__OS_RTOSCK_SMP__) 
static __inline__ unsigned osl_reg_get_bit(void *reg, unsigned bit_start, unsigned bit_end)
{
	unsigned tmp;
	tmp = readl(reg);
	return ((tmp >> bit_start)&((1U << (bit_end - bit_start + 1))-1));
}

static __inline__ void osl_reg_set_bit(void *reg, unsigned bit_start, unsigned bit_end, unsigned reg_val)
{
	unsigned tmp;
	tmp = readl(reg);
	tmp &= (~(((1U << (bit_end - bit_start + 1))-1) << bit_start));
	tmp |= (reg_val << bit_start);
	writel(tmp, reg);
}

#endif

#endif	/* __OSL_BIO_H */
