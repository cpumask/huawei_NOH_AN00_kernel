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

#include "power_monitor.h"
#include <drv_comm.h>
#include <bsp_shared_ddr.h>
#include <bsp_print.h>
#include <bsp_ipc.h>
#include <securec.h>
#include <adrv.h>


#undef THIS_MODU
#define THIS_MODU mod_power_monitor

#define POWER_MONITOR_IP_NUM 13
#define POWER_MONITOR_NUM 1

static unsigned long long modem_power_monitor_update_power(void)
{
    unsigned long long *addr = (unsigned long long *)(SHM_BASE_ADDR + SHM_OFFSET_POWER_MONITOR);
    unsigned long long energy = 0;
    unsigned long ipc_flags;
    int i;
    bsp_ipc_spin_lock_irqsave(IPC_SEM_DPM, ipc_flags);
    for (i = 0; i < POWER_MONITOR_IP_NUM; i++) {
        energy += addr[i];
        addr[i] = 0;
    }
    bsp_ipc_spin_unlock_irqrestore(IPC_SEM_DPM, ipc_flags);

    return energy;
}

unsigned int power_monitor_get_num(void)
{
    return POWER_MONITOR_NUM;
}

int power_monitor_get_data(struct mdm_transmit_t *data, unsigned int len)
{
    int ret;
    unsigned int length = len * sizeof(struct mdm_energy);
    struct mdm_energy *dpm_ip = NULL;
    if (data == NULL || len == 0) {
        power_monitor_print_err("data or len invalid, len %u\n", len);
        return ERROR;
    }

    dpm_ip = (struct mdm_energy *)(data->data);
    ret = strcpy_s(dpm_ip->name, sizeof(dpm_ip->name), "MODEM");
    if (ret != EOK) {
        power_monitor_print_err("copy name error %d\n", ret);
        return ERROR;
    }

    data->length = length;
    dpm_ip->energy = modem_power_monitor_update_power();

    return OK;
}

int bsp_power_monitor_init(void)
{
    int ret = dpm_modem_register(power_monitor_get_num, power_monitor_get_data);
    if (ret != OK) {
        power_monitor_print_err("init error %d\n", ret);
        return ERROR;
    }
    power_monitor_print_err("init ok\n");

    return OK;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_power_monitor_init);
#endif


