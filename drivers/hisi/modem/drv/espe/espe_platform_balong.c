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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/etherdevice.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/syscore_ops.h>

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include <securec.h>
#include <bsp_sysctrl.h>
#include "espe.h"
#include "espe_core.h"
#include "espe_port.h"
#include "espe_dbg.h"


#define SPE_MODULE_NAME "espe"

const char *g_spe_dfs_propname[] = {
    "spe_dfs_freq_grade0", "spe_dfs_freq_grade1", "spe_dfs_freq_grade2", "spe_dfs_freq_grade3", "spe_dfs_freq_grade4",
    "spe_dfs_freq_grade5", "spe_dfs_freq_grade6", "spe_dfs_freq_grade7", "spe_dfs_freq_grade8", "spe_dfs_freq_grade9",
};

void espe_enable(void)
{
    spe_en_t value;
    struct spe *spe = &g_espe;
    SPE_TRACE("enter\n");

    value.u32 = spe_readl(spe->regs, SPE_EN);
    value.bits.spe_en = 1;
    spe_writel(spe->regs, SPE_EN, value.u32);
    SPE_TRACE("exit\n");

    return;
}

void espe_disable(void)
{
    struct spe *spe = &g_espe;
    spe_en_t value;

    /* disable spe hw */
    value.u32 = spe_readl(spe->regs, SPE_EN);
    value.bits.spe_en = 0;
    spe_writel(spe->regs, SPE_EN, value.u32);

    return;
}

void entry_store(void)
{
    struct spe *spe = &g_espe;
    unsigned int *cur_buf = spe->entry_bak;
    int i;
    unsigned int entry_word_num = SPE_MAC_ENTRY_NUM + SPE_IPV4_ENTRY_NUM + SPE_IPV6_ENTRY_NUM + SPE_QOS_MAC_ENTRY_NUM;

    for (i = 0; i < entry_word_num; i++) {
        *cur_buf = spe_readl(spe->regs, SPE_MAC_TAB(i));
        cur_buf++;
    }
}

void entry_restore(void)
{
    int i;
    struct spe *spe = &g_espe;
    unsigned int *cur_buf = spe->entry_bak;
    unsigned int entry_word_num = SPE_MAC_ENTRY_NUM + SPE_IPV4_ENTRY_NUM + SPE_IPV6_ENTRY_NUM + SPE_QOS_MAC_ENTRY_NUM;

    for (i = 0; i < entry_word_num; i++) {
        spe_writel(spe->regs, SPE_MAC_TAB(i), *cur_buf);
        cur_buf++;
    }
}

#ifdef CONFIG_ESPE_PHONE_SOC

static int espe_clk_enable(struct device *dev)
{
    struct spe_pericrg *peri = &g_espe.peri;
    unsigned int reg;

    writel(ESPE_GT_CLK_MASK, peri->crg_addr + ESPE_GT_CLK);
    mdelay(10);

    writel(ESPE_CLK_DIV_DEFAULT, peri->crg_addr + ESPE_CLK_DIV);
    g_espe.clk_div = 1;
    mdelay(10);

    reg = readl(peri->crg_addr + ESPE_GT_CLK_STAT);
    SPE_ERR("espe clk state %x\n", reg);
    reg = readl(peri->crg_addr + ESPE_GT_CLK_FINIAL_STAT);
    SPE_ERR("espe clk state finial %x\n", reg);
    reg = readl(peri->crg_addr + ESPE_CLK_DIV);
    SPE_ERR("espe div finial %x\n", reg);

    return 0;
}

int espe_clk_disable(struct device *dev)
{
    return 0;
}

#else

static int espe_clk_enable(struct device *dev)
{
    struct clk *espe_clk = NULL;
    int ret = 0;

    espe_clk = clk_get(dev, "spe_clk");
    if (IS_ERR(espe_clk)) {
        SPE_ERR("clk:spe_clk is NULL, can't find it,please check!!\n");
        return ret;
    } else {
        ret = clk_prepare_enable(espe_clk);
        if (ret) {
            SPE_ERR("clk:spe_clk enable failed!!\n");
        }
    }
    return ret;
}

int espe_clk_disable(struct device *dev)
{
    struct clk *espe_clk = NULL;
    int ret = 0;

    espe_clk = clk_get(dev, "spe_clk");
    if (IS_ERR(espe_clk)) {
        SPE_ERR("clk:spe_clk is NULL, can't find it,please check!!\n");
        return ret;
    } else {
        clk_disable_unprepare(espe_clk);
    }
    return ret;
}

#endif

void espe_sysctrl_init(const struct device_node *np, struct spe *spe)
{
    struct spe_pericrg *peri = &g_espe.peri;
    dma_addr_t percrg_addr;
    int ret;

    peri->complete_flag = 0;

    ret = of_property_read_u32_array(np, "spe_pericrg_addr", &peri->crg_phy_addr, 1);
    if (ret) {
        SPE_ERR("skip find of spe_pericrg_addr \n");
        return;
    }
    SPE_ERR("spe_pericrg_addr %x \n", peri->crg_phy_addr);

    ret = of_property_read_u32_array(np, "spe_pericrg_len", &peri->crg_len, 1);
    if (ret) {
        SPE_ERR("skip find of spe_pericrg_len\n");
        return;
    }
    SPE_ERR("spe_pericrg_len %u \n", peri->crg_len);

    percrg_addr = peri->crg_phy_addr;
    peri->crg_addr = ioremap(percrg_addr, peri->crg_len);
    if (peri->crg_addr == NULL) {
        SPE_ERR("espe get crg base addr fail \n");
        return;
    }
    SPE_ERR("espe get crg base addr %lx \n", (unsigned long)(uintptr_t)peri->crg_addr);

    ret = of_property_read_u32_array(np, "spe_peri_rst_en", &peri->crg_rst_en, 1);
    if (ret) {
        SPE_ERR("skip find of spe_peri_rst_en\n");
        return;
    }

    ret = of_property_read_u32_array(np, "spe_peri_rst_dis", &peri->crg_rst_dis, 1);
    if (ret) {
        SPE_ERR("skip find of spe_peri_rst_dis\n");
        return;
    }

    ret = of_property_read_u32_array(np, "spe_peri_rst_stat", &peri->crg_rst_stat, 1);
    if (ret) {
        SPE_ERR("skip find of spe_peri_rst_stat\n");
        return;
    }

    ret = of_property_read_u32_array(np, "spe_peri_rst_mask", &peri->crg_rst_mask, 1);
    if (ret) {
        SPE_ERR("skip find of spe_peri_rst_mask\n");
        return;
    }

    peri->complete_flag = 1;

    ret = of_property_read_u32_array(np, "clock_gate_en", &spe->clock_gate_en, 1);
    if (ret) {
        SPE_ERR("skip find of clock_gate_en\n");
        return;
    }

    return;
}

void espe_sysctrl_exit(void)
{
    return;
}

void espe_core_unreset(const struct device_node *np)
{
    struct spe_pericrg *peri = &g_espe.peri;
    unsigned int reg;

    if (peri->complete_flag != 1) {
        return;
    }

    writel(peri->crg_rst_mask, peri->crg_addr + peri->crg_rst_en);
    mdelay(10);
    writel(peri->crg_rst_mask, peri->crg_addr + peri->crg_rst_dis);
    mdelay(10);

    reg = readl(peri->crg_addr + peri->crg_rst_stat);
    SPE_ERR("espe unreset state %x\n", reg);

    return;
}

void spe_get_dfs_arg(struct platform_device *pdev)
{
    struct spe *spe = &g_espe;
    struct device_node *node = pdev->dev.of_node;
    int ret;
    int i = 0;

    spe->spe_dfs_enable = 0;
    spe->spe_dfs_time_interval = SPE_DFS_INTERVAL_DEFAULT;
    ret = of_property_read_u32_array(node, "spe_fre_grade_max", &spe->spe_fre_grade_max, 1);
    if (ret) {
        SPE_ERR("failed to get spe_fre_grade_max from dts\n");
        return;
    } else if (spe->spe_fre_grade_max >= DFS_MAX_LEVEL) {
        SPE_ERR("spe_fre_grade_num from dts overflow!\n");
        return;
    }

    ret = of_property_read_u32_array(node, "spe_dfs_interval", &spe->spe_dfs_time_interval, 1);
    if (ret) {
        SPE_ERR("failed to get spe_dfs_time_interval from dts\n");
        return;
    }

    for (i = 0; i <= spe->spe_fre_grade_max; i++) {
        ret = of_property_read_u32_array(node, g_spe_dfs_propname[i], (u32 *)(spe->freq_grade_array + i), 3);
        if (ret) {
            SPE_ERR("failed to get spe_dfs_freq_grade from dts   i=%d\n", i);
            return;
        }
    }

    ret = of_property_read_u32_array(node, "spe_dfs_ddr_shifts", (u32 *)spe->ddr_profile, (spe->spe_fre_grade_max + 1));
    if (ret) {
        SPE_ERR(" spe ddr profile disabled \n");
        return;
    }

    ret = of_property_read_u32_array(node, "spe_dfs_div_shifts", (u32 *)spe->div_profile, (spe->spe_fre_grade_max + 1));
    if (ret) {
        SPE_ERR(" spe div profile disabled \n");
        spe->spe_dfs_div_enable = 0;
    } else {
        spe->spe_dfs_div_enable = 1;
    }

    spe->start_byte_limit = SPE_DEFAULT_START_LIMIT;
    spe->spe_dfs_enable = 1;

    return;
}

void espe_rwptr_show(void)
{
    struct spe *spe = &g_espe;
    unsigned int rd_rptr;
    unsigned int rd_wptr;
    unsigned int td_rptr;
    unsigned int td_wptr;
    unsigned int portno;

    for (portno = 0; portno < SPE_PORT_NUM; portno++) {
        if (test_bit(portno, &spe->portmap[SPE_PORTS_NOMARL]) || portno == SPE_CCORE_PPP_NDIS_CTRL_ID) {
            rd_rptr = spe_readl_relaxed(spe->regs, SPE_RDQX_RPTR(portno));
            rd_wptr = spe_readl_relaxed(spe->regs, SPE_RDQX_WPTR(portno));
            td_rptr = spe_readl_relaxed(spe->regs, SPE_TDQX_RPTR(portno));
            td_wptr = spe_readl_relaxed(spe->regs, SPE_TDQX_WPTR(portno));

            SPE_ERR("portno:%d, rd_rptr = 0x%x rd_wptr = 0x%x td_rptr = 0x%x td_wptr = 0x%x \n", portno, rd_rptr, rd_wptr, td_rptr, td_wptr);
        }
    }

    if (spe->ipfport.check_ipf_bd != NULL) {
        spe->ipfport.check_ipf_bd();
    }
    return;
}
int espe_suspend(void)
{
    struct spe *spe = &g_espe;
    unsigned long flags;
    unsigned int spe_idle;
    unsigned int spe_rd_status;

    spin_lock_irqsave(&spe->pm_lock, flags);
    // judge spe_idle
    spe_idle = spe_readl_relaxed(spe->regs, SPE_IDLE);
    spe_rd_status = spe_readl(spe->regs, SPE_RDQ_STATUS);
    if ((spe_idle != SPE_IDLE_DEF_VAL) || (spe_rd_status != SPE_RDQ_STATUS_EMPYT)) {
        spe->not_idle++;
        espe_rwptr_show();
        spin_unlock_irqrestore(&spe->pm_lock, flags);
        SPE_ERR("spe busy; spe_idle=%x spe_rd_status=%x  \n", spe_idle, spe_rd_status);
        return -EBUSY;
    }

    // close spe_en
    spe->flags &= (~SPE_FLAG_ENABLE);
    espe_disable();

    // open dbgen
    dbgen_en();

    // store entrys
    entry_store();

    // close dbgen
    dbgen_dis();

    spe->halt = 1;

    espe_enable();
    spe->flags |= SPE_FLAG_ENABLE;
    spe->flags |= SPE_FLAG_SUSPEND;
    spin_unlock_irqrestore(&spe->pm_lock, flags);

    spe->suspend_count++;

    return 0;
}

void espe_resume(void)
{
    struct spe *spe = &g_espe;
    unsigned long flags;
    unsigned int portno;
    unsigned int intr_interval;

    spin_lock_irqsave(&spe->pm_lock, flags);
    spe->wakeup_flag = TRUE;
    spe->halt = 0;
    intr_interval = spe_readl(spe->regs, SPE_INTA_INTERVAL);
    if (intr_interval == SPE_INTR_MIN_INTERVAL) {
        espe_intr_set_interval(SPE_INTR_DEF_INTEVAL);
        for (portno = 0; portno < SPE_PORT_NUM; portno++) {
            spe->ports[portno].ctrl.td_free = 0;
            spe->ports[portno].ctrl.td_busy = 0;
            spe->ports[portno].ctrl.rd_free = 0;
            spe->ports[portno].ctrl.rd_busy = 0;
        }
    }

    spe->flags &= (~SPE_FLAG_ENABLE);
    espe_disable();

    // open dbgen
    dbgen_en();

    // restore entrys   timestamp?
    entry_restore();

    // close dbgen
    dbgen_dis();

    // open spe_en
    espe_enable();

    spe->flags |= SPE_FLAG_ENABLE;
    spe->flags &= (~SPE_FLAG_SUSPEND);
    spe->resume_count++;

    spin_unlock_irqrestore(&spe->pm_lock, flags);
    return;
}

static int spe_bakspace_init(struct spe *spe)
{
    int ret = 0;
    unsigned int entry_word_num = SPE_MAC_ENTRY_NUM + SPE_IPV4_ENTRY_NUM + SPE_IPV6_ENTRY_NUM + SPE_QOS_MAC_ENTRY_NUM;

    spe->porten_bak = 0;
    spe->suspend_count = 0;
    spe->not_idle = 0;

    spe->entry_bak = kmalloc((sizeof(unsigned int)) * entry_word_num, GFP_ATOMIC);
    if (spe->entry_bak == NULL) {
        ret = -ENOMEM;
    }

    return ret;
}

static void spe_bakspace_exit(struct spe *spe)
{
    if (spe->entry_bak != NULL) {
        kfree(spe->entry_bak);
        spe->entry_bak = NULL;
    }
}

static struct syscore_ops espe_syscore_ops = {
    .suspend = espe_suspend,
    .resume = espe_resume,
};


void spe_nv_init(struct spe *spe)
{
    int ret;
    ret = bsp_nvm_read(NV_ID_DRV_SPE_FEATURE, (u8 *)&spe->spe_feature, sizeof(spe->spe_feature));
    if (ret) {
        spe->spe_feature.smp_sch = 0;
        SPE_ERR("spe read nv fail\n");
    } else {
        spe->ipfport.extend_desc = spe->spe_feature.ipf_extdesc;
        spe->usbport.bypass_mode = 0;
        SPE_ERR("[init]spe smp_sch      %u \n", spe->spe_feature.smp_sch);
        SPE_ERR("[init]spe smp_hp       %u \n", spe->spe_feature.smp_hp);
        SPE_ERR("[init]spe ipf_extdesc  %u \n", spe->spe_feature.ipf_extdesc);
        SPE_ERR("[init]spe direct_fw     %u \n", spe->spe_feature.direct_fw);
    }
}

static int spe_probe(struct platform_device *pdev)
{
    struct resource *res = NULL;
    struct device *dev = &pdev->dev;
    struct spe *spe = &g_espe;
    int ret;
    int irq;
    void __iomem *regs;

    SPE_ERR("[Init]SPE probe\n");

    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));

    ret = memset_s(spe, sizeof(*spe), 0, sizeof(struct spe));
    if (ret) {
        SPE_ERR("[Init]SPE memset_s err.\n");
        WARN_ON_ONCE(1);
    }

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL) {
        SPE_ERR("missing resource\n");
        ret = -ENODEV;
        goto spe_probe_err;
    }

    regs = devm_ioremap_resource(dev, res);
    if (IS_ERR(regs)) {
        SPE_ERR("ioremap failed\n");
        ret = -ENOMEM;
        goto spe_probe_err;
    }
    spe->res = res;
    spe->regs = regs;

    /* sysctrl initialization */
    espe_sysctrl_init(dev->of_node, spe);

    /* sysctrl initialization */
    espe_clk_enable(dev);

    espe_core_unreset(dev->of_node);

    irq = platform_get_irq_byname(pdev, "spe_irq");
    if (irq == -ENXIO) {
        SPE_ERR("SPE IRQ configuration information not found\n");
        ret = -ENXIO;
        goto spe_probe_err;
    }

    platform_set_drvdata(pdev, spe);

    spe->dev = dev;
    spe->irq = irq;

    spe_nv_init(spe);

    spe->spe_version = spe_readl(spe->regs, SPE_VER);
    spe->msg_level = SPE_MSG_ERR | SPE_MSG_TRACE;
    spe->dbg_level = SPE_DBG_TD_RESULT | SPE_DBG_RD_RESULT;
    spe->halt = 0;

    ret = spe_core_init(spe);
    if (ret) {
        SPE_ERR("spe_init fail, ret = %d\n", ret);
        goto spe_probe_err;
    }

    spe_get_dfs_arg(pdev);
    spe_dfs_init(spe);

    /* register acore dpm funs */
    register_syscore_ops(&espe_syscore_ops);
    ret = spe_bakspace_init(spe);
    if (ret) {
        SPE_ERR("failed to alloc memory for backup space\n");
        ret = -EIO;
        goto spe_probe_err_core_exit;
    }
    spin_lock_init(&spe->pm_lock);

    spe_hook_register();

    espe_enable();

    spe->flags |= SPE_FLAG_ENABLE;
    SPE_ERR("[Init]SPE probe done\n");

    return 0;

spe_probe_err_core_exit:
    spe_core_exit(spe);
    spe_bakspace_exit(spe);

spe_probe_err:
    spe_hook_unregister();
    espe_sysctrl_exit();
    return ret;
}

static int spe_remove(struct platform_device *pdev)
{
    struct spe *spe = platform_get_drvdata(pdev);

    if (spe == NULL) {
        SPE_ERR("no spe exist!\n");
        return -EFAULT;
    }

    espe_disable();

    spe->flags &= (~SPE_FLAG_ENABLE);

    spe_core_exit(spe);

    iounmap(spe->regs);

    espe_sysctrl_exit();

    spe_hook_unregister();

    return 0;
}

static const struct of_device_id spe_dt_ids[] = {
    { .compatible = "hisilicon,spe" },
    { /* sentinel */ }
};

static struct platform_driver spe_driver = {
    .probe = spe_probe,
    .remove = spe_remove,
    .driver =
    {
        .name = SPE_MODULE_NAME,
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(spe_dt_ids),
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

static int __init espe_balong_init(void)
{
    return platform_driver_register(&spe_driver);
}

int bsp_espe_module_init(void)
{
    return platform_driver_register(&spe_driver);
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(espe_balong_init);
#endif

void espe_balong_exit(void)
{
    /*
     * don't need kfree platform_data in exit process,
     * platform_driver_unregister will do it.
     */
    platform_driver_unregister(&spe_driver);

    return;
}

module_exit(espe_balong_exit);

MODULE_DEVICE_TABLE(of, spe_dt_ids);
MODULE_AUTHOR("hisi bsp4 network");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("smart packet engine(spe) driver");

