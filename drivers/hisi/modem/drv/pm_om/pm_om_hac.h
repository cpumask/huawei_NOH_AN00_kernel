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

#ifndef __PM_OM_HAC_H__
#define __PM_OM_HAC_H__
#include <product_config.h>
#ifndef HAC_CORE_NUM
#define HAC_CORE_NUM 1
#endif
#define HAC_PRINT_BUF_SIZE (800)
/* defined in hac dpm_debug.h,should be keep same */
#define HAC_DPM_DEBUG_CHAR_NUM 8
struct hac_dpm_device_info {
    char device_name[DPM_DEBUG_CHAR_NUM];
    u32 device_addr;
    u32 fail_cnt;
    u32 max_s;
    u32 max_r;
};
#define HAC_PM_MAIN_CPU 0
/* defined in hac wakelock_debug.h,should be keep same */
#define HAC_WAKELOCK_NUMBER 32
struct hac_wakelock_dump_s {
    u32 total_time[HAC_WAKELOCK_NUMBER];
    u32 latest_lock_time;
    u32 latest_lockid;
    u32 latest_unlock_time;
    u32 latest_unlockid;
    u32 hac_wakelock_state;
};
/* defined in pm_debug.h,should be keep same */
#define HAC_DUMP_SLICE_SIZE 10
struct hac_dump {
    u32 wakeup_cnt[HAC_CORE_NUM];
    u32 sleep_cnt[HAC_CORE_NUM];
    u32 wakeup_slice[HAC_CORE_NUM][HAC_DUMP_SLICE_SIZE];
    u32 sleep_slice[HAC_CORE_NUM][HAC_DUMP_SLICE_SIZE];
};
struct hac_dump_info_s {
    struct hac_dump *hac_pm_dump_base; /* hac dump info base */
    struct hac_wakelock_dump_s *hac_wakelock_dump_base;
    struct hac_dpm_device_info *hac_dpm_dump_base;
    void *hac_dump_base; /* hac dump info base,include dpm,pm,wakelock */
    u32 hac_pm_dump_offset;
    u32 hac_pm_dump_size;
    u32 hac_dpm_dump_offset;
    u32 hac_dpm_dump_size;
    u32 hac_wakelock_dump_offset;
    u32 hac_wakelock_dump_size;
    u32 hac_logbuf_max_size;
};

void hac_dump_init(void);
void hacpm_suspend_prepare(void);
void print_hac_lowpower_info(void);

#endif
