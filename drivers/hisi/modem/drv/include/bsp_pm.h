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
#ifndef __BSP_PM_H__
#define __BSP_PM_H__

#include <product_config.h>
#include <osl_types.h>
#include <mdrv_pm.h>

/*
动态设置唤醒源
参数 int_id  对应 m3 上的中断号
*/
#define CPU_NOTIFIER_SUSPEND   0
#define CPU_NOTIFIER_RESUME     1

#if (defined(CONFIG_BALONG_PM_SUSPEND) && defined(__KERNEL__))

void pm_enable_wake_src(enum pm_wake_src wake_src);
void pm_disable_wake_src(enum pm_wake_src wake_src);
u32 pm_get_acore_sleep_times(void);
#else
static inline u32 bsp_get_pm_wakeup_slice(void){return 0;}
static inline void pm_enable_wake_src(enum pm_wake_src wake_src){}
static inline void pm_disable_wake_src(enum pm_wake_src wake_src){}
static inline u32 pm_get_acore_sleep_times(void){return 0;}
#endif

enum debug_wake_type{
	PM_WAKEUP,
	PM_WAKEUP_THEN_SLEEP,
};
struct debug_pm_s{
	u32 wake_type;
};
enum PM_LOG_TYPE{
	PM_LOG_NORMAL_RECORD,
	PM_LOG_IDLE_RECORD,
	PM_LOG_SET_WAKE_SRC_RECORD,
};
enum PM_SLEEP_MODE_TYPE{
	PM_DEEP_SLEEP_MODE,
	PM_LIGHT_SLEEP_MODE,
};
void bsp_pm_set_sleep_mode(enum PM_SLEEP_MODE_TYPE sleep_mode);

#endif/*__BSP_PM_H__*/
