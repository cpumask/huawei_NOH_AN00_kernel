/*
 * hisi_bl31_exception.h
 *
 * Hisilicon BL31 exception driver
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __BL31_EXCEPTION_H
#define __BL31_EXCEPTION_H

#include <linux/types.h>
#include <linux/hisi/rdr_pub.h>

#define BL31_PANIC_MAGIC 0xdead
#define BL31_DEBUG_PANIC_VAL 0xCA000001
#define SCTRL_REG_SIZE 0x1000ULL

#ifdef CONFIG_HISI_BL31_MNTN
extern void bl31_panic_ipi_handle(void);
extern u32 get_bl31_exception_flag(void);
extern int rdr_exception_trace_bl31_cleartext_print(const char *dir_path, u64 log_addr, u32 log_len);
extern int rdr_exception_trace_bl31_init(u8 *phy_addr, u8 *virt_addr, u32 log_len);
extern int rdr_exception_analysis_bl31(u64 etime, u8 *addr, u32 len, struct rdr_exception_info_s *exception);
extern void rdr_init_sucess_notify_bl31(void);
#else
static inline void bl31_panic_ipi_handle(void)
{
}

static inline u32 get_bl31_exception_flag(void)
{
	return 0;
}

static inline int rdr_exception_trace_bl31_cleartext_print(const char *dir_path, u64 log_addr, u32 log_len)
{
	return 0;
}

static inline int rdr_exception_trace_bl31_init(u8 *phy_addr, u8 *virt_addr, u32 log_len)
{
	return 0;
}

static inline void rdr_init_sucess_notify_bl31(void)
{
}

static inline int rdr_exception_analysis_bl31(u64 etime, u8 *addr, u32 len, struct rdr_exception_info_s *exception)
{
	return 0;
}
#endif
#endif
