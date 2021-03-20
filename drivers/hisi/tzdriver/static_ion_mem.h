/*
 * static_ion_mem.h
 *
 * memory init, register for mailbox pool.
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _STATIC_ION_MEM_H_
#define _STATIC_ION_MEM_H_

#ifdef CONFIG_STATIC_ION

int tc_ns_register_ion_mem(void);

#else

static int tc_ns_register_ion_mem(void)
{
	return 0;
}

#endif

#endif
