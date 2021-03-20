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

#ifndef __ACORE_NV_ID_GUCTTF_H__
#define __ACORE_NV_ID_GUCTTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* GUCTTF���A�����NVID��Χ����,[4850-4949],100�� */
#define GUCTTF_ACORE_NV_ID_MIN 4850
#define GUCTTF_ACORE_NV_ID_MAX 4999

/* A��NV_ID���� */
enum GUCTTF_AcoreNvId {
    /* via:CPULOAD_Cfg */
    NV_ITEM_LINUX_CPU_MONITOR_TIMER_LEN = GUCTTF_ACORE_NV_ID_MIN + 1, /* 4851 */

    /* via:FC_CpuDrvAssemParaNv����̬�����������������λ��Ϣ */
    NV_ITEM_FC_ACPU_DRV_ASSEMBLE_PARA = GUCTTF_ACORE_NV_ID_MIN + 2, /* 4852 */

    NV_ITEM_NETFILTER_HOOK_MASK = GUCTTF_ACORE_NV_ID_MIN + 3, /* 4853 */

    /* via:FC_AcoreCfgNv */
    NV_ITEM_ACORE_FLOW_CRTL_CONFIG = GUCTTF_ACORE_NV_ID_MIN + 5, /* 4855 */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
