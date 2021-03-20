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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#include <bsp_trans_report.h>

#include "espe.h"
#include "espe_dbg.h"
#include "espe_desc.h"
#include "espe_port.h"
#include "espe_interrupt.h"

#define ESPE_SMP_BIT_SCHED 0
#define ESPE_SMP_BIT_SCHED_REQ 1

void espe_intr_disable(void)
{
    return;
}

void espe_intr_enable(void)
{
    return;
}

static void espe_intr_bh_tasklet(unsigned long _params)
{
    struct spe *spe = &g_espe;
    struct spe_port_ctrl *ctrl = (struct spe_port_ctrl *)(uintptr_t)_params;

    if (unlikely(ctrl == NULL)) {
        espe_bug(spe);
        return;
    }

    if (ctrl->port_flags.enable && (ctrl->portno != spe->ipfport.portno)) {
        espe_td_complete_intr(ctrl->portno);
        espe_rd_complete_intr(ctrl->portno);
    }

    return;
}

static void espe_interrupt_bh_task(void *info)
{
    struct spe_port_ctrl *ctrl = (struct spe_port_ctrl *)info;
    tasklet_hi_schedule(&ctrl->smp.ini_bh_tasklet);  // call spe_smp_intr_bh
    clear_bit(ESPE_SMP_BIT_SCHED, &ctrl->smp.status);
    clear_bit(ESPE_SMP_BIT_SCHED_REQ, &ctrl->smp.status);
}

void espe_process_desc_tasklet(void *spe_ctx, unsigned int evt_rd_done, unsigned int evt_td_done)
{
    struct spe *spe = (struct spe *)spe_ctx;
    struct spe_port_ctrl *ctrl = NULL;
    unsigned int port_num;
    unsigned int port_cpu;
    unsigned int evt_done;
    int ret;
    evt_done = evt_rd_done || evt_td_done;

    for (port_num = 0; port_num < SPE_PORT_NUM; port_num++) {
        ctrl = &spe->ports[port_num].ctrl;
        if (ctrl->port_flags.enable) {
            if (0x1 & (evt_td_done >> port_num)) {
                spe->stat.evt_td_complt[port_num]++;
            }

            if (0x1 & (evt_rd_done >> port_num)) {
                spe->stat.evt_rd_complt[port_num]++;
            }
        }
        if ((0x1 & (evt_done >> port_num)) || test_bit(ESPE_SMP_BIT_SCHED_REQ, &ctrl->smp.status)) {
            port_cpu = ctrl->smp.def_cpu;
            if (!test_and_set_bit(ESPE_SMP_BIT_SCHED, &ctrl->smp.status)) {
                ret = smp_call_function_single_async(port_cpu, &ctrl->smp.csd); // call espe_interrupt_bh_task
                if (unlikely(ret)) {
                    espe_interrupt_bh_task(ctrl);
                    spe->smp.sch_fail++;
                }
            } else {
                set_bit(ESPE_SMP_BIT_SCHED_REQ, &ctrl->smp.status);
            }
        }
    }
}

void espe_init_tasklet(struct spe *spe, struct spe_port_ctrl *port_ctrl)
{
    port_ctrl->smp.csd.func = espe_interrupt_bh_task;
    port_ctrl->smp.csd.info = port_ctrl;
    port_ctrl->smp.csd.flags = 0;

    tasklet_init(&port_ctrl->smp.ini_bh_tasklet, espe_intr_bh_tasklet, (unsigned long)(uintptr_t)port_ctrl);

    spe->smp.sch_fail = 0;

    SPE_ERR("espe_init_tasklet success \n");
    return;
}

void espe_interrupt_init_smp(struct spe *spe)
{
    int ret;
    if (spe->spe_feature.smp_sch == 0) {
        return;
    }

    ret = of_property_read_u32_array(spe->dev->of_node, "espe_intr_core_num", &spe->smp.intr_core, 1);
    if (ret) {
        SPE_ERR(" spe_smp get espe_intr_core_num fail \n");
        spe->spe_feature.smp_sch = 0;
        return;
    } else {
        SPE_ERR(" spe_smp get espe_intr_core_num %u \n", spe->smp.intr_core);
    }

    ret = of_property_read_u32_array(spe->dev->of_node, "espe_use_hp_cpu_thr", &spe->smp.use_hp_cpu_thr, 1);
    if (ret) {
        SPE_ERR(" spe_smp get use_hp_cpu_thr fail \n");
        spe->spe_feature.smp_sch = 0;
        return;
    } else {
        SPE_ERR(" spe_smp get use_hp_cpu_thr %u \n", spe->smp.use_hp_cpu_thr);
    }

    ret = of_property_read_u32_array(spe->dev->of_node, "espe_def_cpuport_core_num", &spe->smp.def_cpuport_core, 1);
    if (ret) {
        SPE_ERR(" spe_smp get espe_def_cpuport_core_num fail \n");
        spe->spe_feature.smp_sch = 0;
        return;
    } else {
        SPE_ERR(" spe_smp get espe_def_cpuport_core_num %u \n", spe->smp.def_cpuport_core);
    }

    ret = of_property_read_u32_array(spe->dev->of_node, "espe_hp_cpuport_core_num", &spe->smp.hp_cpuport_core, 1);
    if (ret) {
        SPE_ERR(" spe_smp get espe_hp_cpuport_core_num fail \n");
        spe->spe_feature.smp_sch = 0;
        return;
    } else {
        SPE_ERR(" spe_smp get espe_hp_cpuport_core_num %u \n", spe->smp.hp_cpuport_core);
    }

    spe->smp.cpuport_core = spe->smp.def_cpuport_core;

    return;
}

void espe_process_desc_intr(void *spe_ctx, unsigned int evt_rd_done, unsigned int evt_td_done)
{
    struct spe *spe = (struct spe *)spe_ctx;
    unsigned int port_num;

    for (port_num = 0; port_num < SPE_PORT_NUM; port_num++) {
        if (0x1 & (evt_rd_done >> port_num)) {
            spe->stat.evt_rd_complt[port_num]++;
            espe_rd_complete_intr(port_num);
        }

        if (0x1 & (evt_td_done >> port_num)) {
            spe->stat.evt_td_complt[port_num]++;
            espe_td_complete_intr(port_num);
        }
    }
}

static void espe_intr_bh_workqueue(struct work_struct *work)
{
    struct spe *spe = &g_espe;
    struct spe_port_ctrl *ctrl = NULL;
    unsigned int port_num;

    for (port_num = 0; port_num < SPE_PORT_NUM; port_num++) {
        ctrl = &spe->ports[port_num].ctrl;

        if (ctrl->port_flags.enable && (port_num != spe->ipfport.portno)) {
            espe_rd_complete_intr(port_num);
        }
    }
    return;
}

void espe_process_desc_workqueue(void *spe_ctx, unsigned int evt_rd_done, unsigned int evt_td_done)
{
    struct spe *spe = (struct spe *)spe_ctx;
    unsigned int port_num;
    struct spe_port_ctrl *ctrl = NULL;

    for (port_num = 0; port_num < SPE_PORT_NUM; port_num++) {
        ctrl = &spe->ports[port_num].ctrl;

        if (ctrl->port_flags.enable) {
            if (0x1 & (evt_td_done >> port_num)) {
                spe->stat.evt_td_complt[port_num]++;
                espe_td_complete_intr(port_num);
            }

            if (0x1 & (evt_rd_done >> port_num)) {
                spe->stat.evt_rd_complt[port_num]++;
            }
        }
    }

    queue_work(spe->smp.workqueue,  &spe->smp.work);
}

void espe_init_workqueue(struct spe *spe)
{
    INIT_WORK(&spe->smp.work, espe_intr_bh_workqueue);
    spe->smp.workqueue = alloc_ordered_workqueue("process_desc", 0);
    if (spe->smp.workqueue == NULL) {
        SPE_ERR("espe port workqueue alloc fail\n");
    }

    SPE_ERR("espe port workqueue allocd\n");
    return;
}


static irqreturn_t espe_interrupt(int irq, void *dev_id)
{
    struct spe *spe = (struct spe *)dev_id;
    unsigned int evt_rd_full;
    unsigned int evt_rd_done;
    unsigned int evt_td_done;
    unsigned int evt_ad_empty;
    unsigned int port_num;

    if (unlikely(spe->dbg_level & SPE_DBG_HIDS_UPLOAD)) {
        bsp_trans_report_set_time(); /* set trans report timestamp */
    }

    evt_rd_full = spe_readl_relaxed(spe->regs, SPE_RDQ_FULL_INTA);
    evt_rd_done = spe_readl_relaxed(spe->regs, SPE_RD_DONE_INTA);
    evt_td_done = spe_readl_relaxed(spe->regs, SPE_TD_DONE_INTA);
    evt_ad_empty = spe_readl_relaxed(spe->regs, SPE_ADQ_EMPTY_INTA);

    spe_writel_relaxed(spe->regs, SPE_RDQ_FULL_INTA_STATUS, evt_rd_full);
    spe_writel_relaxed(spe->regs, SPE_RD_DONE_INTA_STATUS, evt_rd_done);
    spe_writel_relaxed(spe->regs, SPE_TD_DONE_INTA_STATUS, evt_td_done);
    spe_writel_relaxed(spe->regs, SPE_ADQ_EMPTY_INTA_STATUS, evt_ad_empty);

    if (spe->spe_version == ESPE_VER_V200 && spe->ipfport.alloced) {
        espe_td_complete_ipf_intr();
    }

    if (evt_rd_done || evt_td_done) {
        spe->smp.process_desc((void *)spe, evt_rd_done, evt_td_done);
    }

    if (unlikely(evt_rd_full)) {
        for (port_num = 0; port_num < SPE_PORT_NUM; port_num++) {
            if (unlikely(0x1 & (evt_rd_full >> port_num))) {
                spe->stat.evt_rd_full[port_num]++;
            }
        }
    }

    if (unlikely(evt_ad_empty)) {
        if (0x1 & evt_ad_empty) {
            spe->stat.evt_ad_empty[SPE_ADQ0]++;
        }
        if (0x2 & evt_ad_empty) {
            spe->stat.evt_ad_empty[SPE_ADQ1]++;
        }
    }

#ifndef CONFIG_ESPE_PHONE_SOC
    if (spe->spe_version == ESPE_VER_V200 || spe->spe_version == ESPE_VER_V300) {
        espe_ad_updata_wptr(spe);
    }
#endif

    return IRQ_HANDLED;
}

int espe_interrput_init(struct spe *spe)
{
    int ret;

    spe_writel_relaxed(spe->regs, SPE_RDQ_FULL_INTA_MASK, SPE_EVENT_ENABLE);
    spe_writel_relaxed(spe->regs, SPE_RD_DONE_INTA_MASK, SPE_EVENT_ENABLE);
    spe_writel_relaxed(spe->regs, SPE_TD_DONE_INTA_MASK, SPE_EVENT_ENABLE);
    spe_writel_relaxed(spe->regs, SPE_ADQ_EMPTY_INTA_MASK, SPE_EVENT_ENABLE);
    spe_writel_relaxed(spe->regs, SPE_BUS_ERR_INTA_MASK, SPE_EVENT_ENABLE);
    spe_writel_relaxed(spe->regs, SPE_INTA_INTERVAL, SPE_INTR_DEF_INTEVAL);

    // set to 1k as default
    spe_writel_relaxed(spe->regs, SPE_RDQ_FULL_LEVEL, SPE_EVT_RD_FULL);
    spe_writel(spe->regs, SPE_ADQ_EMPTY_LEVEL, SPE_EVT_AD_EMPTY);

    spe->smp.process_desc = espe_process_desc_workqueue;
    espe_init_workqueue(spe);

    ret = request_irq(spe->irq, espe_interrupt, IRQF_SHARED, "eSPE", spe);

    return ret;
}

