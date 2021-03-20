/*
 * hisi_vcodec_vdec_plat.h
 *
 * This is for vdec platform
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef HISI_VCODEC_VDEC_PLAT_H
#define HISI_VCODEC_VDEC_PLAT_H
#include "dbg.h"

#define vdec_check_ret(cond, ret) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "assert condition %s not match\n", #cond); \
			return ret; \
		} \
	} while (0)

#define vdec_init_mutex(lock) \
	do { \
		mutex_init(lock); \
	} while (0)

#define vdec_mutex_lock(lock) \
	do { \
		mutex_lock(lock); \
	} while (0)

#define vdec_mutex_lock_interruptible(lock) \
	({int mutex_lock_ret;\
		do { \
			mutex_lock_ret = mutex_lock_interruptible(lock); \
		} while (0); \
	mutex_lock_ret;})

#define vdec_mutex_unlock(lock) \
	do { \
		mutex_unlock(lock); \
	} while (0)

hi_s32 vdec_device_probe(void *plt);

#endif
