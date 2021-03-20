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
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <asm/atomic.h>
#include <linux/netdevice.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/gfp.h>
#include <linux/timer.h>
#include <asm/memory.h>
#include <linux/bitops.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_aximem.h>
#include <bsp_slice.h>
#include <bsp_dump.h>
#include <bsp_om_enum.h>
#include "bsp_maa.h"
#include "maa.h"
#include "mdrv_errno.h"
#include <asm-generic/bug.h>
#include <osl_types.h>
#include <bsp_sysctrl.h>
#include <bsp_reset.h>
#include <mdrv_sysboot.h>
#include <linux/syscore_ops.h>
#include <linux/pm_wakeup.h>
#include <linux/mm.h>
#include <mdrv_memory_layout.h>
#ifdef CONFIG_MAA_V2
#include "hi_maa_v2.h"
#else
#include "hi_maa.h"
#endif

#define THIS_MODU mod_maa
#define MAA_TABLE_MAX_SIZE (96 * 1024)
#define MAA_TABLE_4K_SIZE 13000
#define MAA_TABLE_MIN_SIZE (1 << 10)
#define MAA_TABLE_CP_LEVEL4_SIZE (16 * 1024)
#define MAA_TABLE_CP_LEVEL5_SIZE (16 * 1024)
#define MAA_TABLE_CP_LEVEL3_SIZE 512
#define MAA_MEM_LINE_DEAULT 100
#define MAA_CNT_CLK_EN_VAL 1
#define MAA_MAX_SCHE_LOOP 64
#define MAA_CURRETN_AVAIL_MEM ((unsigned long)si_mem_available() << (PAGE_SHIFT - 10))
#define MAA_TIMER_EXPIRES (msecs_to_jiffies(1000))
#define MAA_U32_BITLEN  32
#define MAA_U64_BITLEN  64
#define MAA_U16_BITLEN  16
#define MAA_TWO_BURST_RESERVE   (2 * MAA_BURST_LEN)
#define MAA_RELEASE_THRSH_LP   128
#define MAA_OPIPE_REG_START MAA_OPIPE_BASE_ADDR_L(0)
#define MAA_OPIPE_REG_END   MAA_OPIPE_CFG_DONE(5)
#define MAA_LOWPHY_ADDR(addr) ((addr) & 0xffffffff)
#define MAA_HIGHPHY_ADDR(addr) (((addr) >> 32) & 0xff)
#define MAA_MAKE_IPIPE_THRH(uthrl, dthrl) ((((uthrl) & 0x7fffu) << MAA_U16_BITLEN) | ((dthrl) & 0x7fffu))
#define MAA_RELEASE_OPIPE_DEPTH (4 * MAA_BURST_LEN)
#define MAA_RELEASE_OPIPE_UTHRL (MAA_BURST_LEN >> 1)
#define MAA_RELEASE_OPIPE_DTHRL (MAA_BURST_LEN >> 2)

#ifdef CONFIG_MAA_V2
#define MAA_OPIPE2_LP_IPIPE 16
/* MAA V100 timer clk 32KHZ, 0x2000 * 1 / 32.768 : 250ms */
#define MAA_TIMER_CNT 0x2000
#else
#define MAA_OPIPE2_LP_IPIPE 12
/* MAA V100 timer clk 19.2MHZ, 0x100000 * 1 / 1000 / 38.4 : 27.3ms */
#define MAA_TIMER_CNT 0x100000
#endif
#define MAA_LP_IPIPE_LEN (MAA_BURST_LEN - 1)
#define MAA_IPIPE_WPTR_PHY_SIZE 500
#define MAA_LP_IPIPE_WPTR_PHY_SIZE 100
#define MAA_TIMER_EN_VALUE 0x3f
#define MAA_TIMER_CP_OPIPE_EN_VALUE 0x38
#define MAA_TIMER_AP_OPIPE_EN_VALUE 0x7
#define MAA_RECLAIM_MIN_DEPTH 0x1000
#define MAA_RECLAIM_MAX_DIFF 0x40

#define MAA_OPIPE_NAME_MAX 64

#define MAA_RESET_MARK 0xACDCACDC

#define MAA_BYTE_ALIGN(addr) ALIGN(((u32)(addr)), MAA_SKB_RESERVE_SIZE)
struct maa *g_maa_ctx;
extern unsigned long totalram_pages;

unsigned int g_maa_once_flag = 0;

#define maa_err_once(fmt, ...) do { \
    if (!g_maa_once_flag) {          \
        bsp_err("Warning once \n");  \
        bsp_err(fmt, ##__VA_ARGS__); \
        g_maa_once_flag = 1;         \
    }                                \
} while (0)

#define maa_err(fmt, ...) do { \
    bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
} while (0)

#define maa_dbg(fmt, ...) do { \
    if (maa->dbg & MAA_DEBUG_MSG) {                   \
        bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
    }                                                 \
} while (0)

#define maa_bug_on() do { \
    dump_stack();                                                \
    if (maa->dbg & MAA_DEBUG_SYSTEM_ERR) {                       \
        system_error(DRV_ERRNO_MAA_ADDR_CHECK_FAIL, 0, 0, 0, 0); \
    }                                                            \
    if (maa->dbg & MAA_DEBUG_BUGON) {                            \
        BUG_ON(1);                                               \
    }                                                            \
} while (0)

static const int g_level_size[MAA_OPIPE_MAX] = {
    MAA_OPIPE_LEVEL_0_SIZE, MAA_OPIPE_LEVEL_1_SIZE, MAA_OPIPE_LEVEL_2_SIZE, MAA_OPIPE_LEVEL_3_SIZE,
    MAA_OPIPE_LEVEL_4_SIZE, MAA_OPIPE_LEVEL_5_SIZE, MAA_OPIPE_LEVEL_INVALID
};
int maa_mdmreset_cb(drv_reset_cb_moment_e eparam, int userdata);

void maa_set_debug(int enable)
{
    g_maa_ctx->dbg = enable;
}

void maa_set_intr_threhold(int threhold)
{
    g_maa_ctx->intr_threhold = threhold;
}

void maa_set_timer_out(unsigned int timeout)
{
    g_maa_ctx->timer_out = timeout;
}

static inline int maa_check_addr_crc(unsigned long long addr)
{
    unsigned int bitcnt;

    bitcnt = __sw_hweight64(addr);
    if (bitcnt & 1) {
        maa_err("crc check fail,maa 0x%llx\n", addr);
        return -1;
    }
    return 0;
}

static inline int maa_check_double_free(struct maa *maa, unsigned long long addr)
{
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid maa, maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        maa_bug_on();
        return -EFAULT;
    }

    map = &maa->opipes[level].map;
    tab = &map->tab[idx];
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();

        return -EFAULT;
    }

    if (unlikely(MAA_OWN != tab->magic || tab->maa_addr != addr)) {
        maa_err("magic %x, maa 0x%llx, oldmaa 0x%llx, manager 0x%llx\n",
            tab->magic, addr, tab->maa_addr,
            (unsigned long long)(uintptr_t)tab->manager);
        maa_bug_on();

        return -EFAULT;
    }

    return 0;
}

static int maa_check_addr(struct maa *maa, unsigned long long addr)
{
    int err;
    err = maa_check_addr_crc(addr);
    if (err) {
        return err;
    }
    err = maa_check_double_free(maa, addr);
    return err;
}

void maa_list_measure(unsigned int level)
{
    struct maa_map *map = &g_maa_ctx->opipes[level].map;
    struct maa_opipe *opipes = &g_maa_ctx->opipes[level];
    struct list_head *pos = NULL;
    unsigned int cnt = 0;
    unsigned long flags;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->free_list) {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("free_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->busy_list) {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("busy_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->reclaim_list) {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("reclaim_list cnt %u\n", cnt);
}
void maa_show_opipe_status(unsigned int level)
{
    struct maa *maa = g_maa_ctx;
    unsigned int i = level;

    bsp_err("map_invalid_level %u\n", maa->status.map_invalid_level);
    bsp_err("unmap_invalid_level %u\n", maa->status.unmap_invalid_level);
    bsp_err("opipe%u:skb_alloc_success  %u\n", i, maa->opipe_status[level].skb_alloc_success);
    bsp_err("opipe%u:skb_alloc_atomic  %u\n", i, maa->opipe_status[level].skb_alloc_atomic);
    bsp_err("opipe%u:skb_alloc_kernel  %u\n", i, maa->opipe_status[level].skb_alloc_kernel);
    bsp_err("opipe%u:skb_own_free  %u\n", i, maa->opipe_status[level].skb_own_free);
    bsp_err("opipe%u:map_own_success  %u\n", i, maa->opipe_status[level].map_own_success);
    bsp_err("opipe%u:map_own_fail  %u\n", i, maa->opipe_status[level].map_own_fail);
    bsp_err("opipe%u:alloc_map_own_fail  %u\n", i, maa->opipe_status[level].alloc_map_own_fail);
    bsp_err("opipe%u:free_list_empty  %u\n", i, maa->opipe_status[level].free_list_empty);
    bsp_err("opipe%u:skb_alloc_fail  %u\n", i, maa->opipe_status[level].skb_alloc_fail);
    bsp_err("opipe%u:unmap_own  %u\n", i, maa->opipe_status[level].unmap_own);
    bsp_err("opipe%u:opipe:map_fail_skb_free  %u\n", i, maa->opipe_status[level].map_fail_skb_free);
    bsp_err("opipe%u:map_tcp_ip_ok  %u\n", i, maa->opipe_status[level].map_tcp_ip_ok);
    bsp_err("opipe%u:map_tcp_ip_fail  %u\n", i, maa->opipe_status[level].map_tcp_ip_fail);
    bsp_err("opipe%u:unmap_tcp_ip  %u\n", i, maa->opipe_status[level].unmap_tcp_ip);
    bsp_err("opipe%u:unmap_test_tcp_ip  %u\n", i, maa->opipe_status[level].unmap_test_tcp_ip);
    bsp_err("opipe%u:opipe_full  %u\n", i, maa->opipe_status[level].opipe_full);
    bsp_err("opipe%u:opipe_dn  %u\n", i, maa->opipe_status[level].opipe_dn);
    bsp_err("opipe%u:ipipe_up  %u\n", i, maa->opipe_status[level].ipipe_up);
    bsp_err("opipe%u:delay_free_cnt %u\n", i, maa->opipes[i].delay_free_cnt);
    bsp_err("opipe%u:reclaim_cnt %u\n", i, maa->opipes[i].reclaim_cnt);
    bsp_err("opipe%u:reclaim_depth %u\n", i, maa->opipes[i].reclaim_depth);
    bsp_err("opipe%u:reclaim_clear %u\n", i, maa->opipes[i].reclaim_clear);
    bsp_err("opipe%u:skb_reclaim_free %u\n", i, maa->opipe_status[level].skb_reclaim_free);
    bsp_err("opipe%u:skb_tcp_ip_free %u\n", i, maa->opipe_status[level].skb_tcp_ip_free);
    bsp_err("opipe%u:busy %u\n", i, maa->opipes[i].map.busy);
    bsp_err("opipe%u:busy_depth %u\n", i, maa->opipes[i].busy_depth);
    bsp_err("opipe%u:max_alloc_cnt %u\n", i, maa->opipes[i].max_alloc_cnt);
    bsp_err("opipe%u:min_alloc_cnt %u\n", i, maa->opipes[i].min_alloc_cnt);
    bsp_err("opipe%u:free_success %u\n", i, maa->opipe_status[level].free);
    bsp_err("refill_succ: %u\n", maa->status.refill_succ);
    bsp_err("sche_cnt: %u\n", maa->status.sche_cnt);
    bsp_err("reclaim_succ: %u\n", maa->status.reclaim_succ);
    bsp_err("tab size: %u\n", maa->opipes[level].map.tab_max_size);
    maa_list_measure(level);
}

void maa_show_ipipe_status(void)
{
    struct maa *maa = g_maa_ctx;
    unsigned int i = 0;
    struct maa_ipipe *ipipe = NULL;

    if (maa == NULL) {
        return;
    }

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        ipipe = maa->cpu_ipipes + i;
        bsp_err("ipipe%u ipipe_empty %u\n", ipipe->idx, ipipe->debug.ipipe_empty);
        bsp_err("ipipe%u alloc_success %u\n", ipipe->idx, ipipe->debug.alloc);
    }
}

void maa_show_push_cnt(void)
{
    unsigned int level;
    unsigned int idx;
    struct maa *maa = g_maa_ctx;

    if (maa == NULL) {
        return;
    }

    for (level = 0; level < MAA_OPIPE_MAX; level++) {
        bsp_err("opipe%u alloc_cnt %u\n", level, readl(maa->regs + MAA_ACORE_ALLOC_CNT(level)));
        bsp_err("opipe%u alloc_fifo_cnt %u\n", level, readl(maa->regs + MAA_ALLOC_FIFO_CNT(level)));
        bsp_err("opipe%u alloc_maa_cnt %u\n", level, readl(maa->regs + MAA_ALLOC_MAA_CNT(level)));
        bsp_err("opipe%u maa_rls_cnt %u\n", level, readl(maa->regs + MAA_RLS_CNT(level)));
        for (idx = 0; idx < MAA_MAX_IPIPE_ENDPOINT; idx++) {
            bsp_err("opipe%u to ipipe%u alloc_destn_cnt %u\n", level, level * MAA_OPIPE_MAX + idx,
                    readl(maa->regs + MAA_ALLOC_DESTN_CNT(idx, level)));
            bsp_err("opipe%u to ipipe%u destn_alloc_cnt %u\n", level, level * MAA_OPIPE_MAX + idx,
                    readl(maa->regs + MAA_DESTN_ALLOC_CNT(idx, level)));
        }
    }
}

void maa_help(void)
{
    bsp_err("maa_show_opipe_status \n");
    bsp_err("maa_show_ipipe_status \n");
    bsp_err("maa_show_push_cnt \n");
}

void maa_hds_transreport_hal(struct maa_debug_info *dbg_info)
{
    int level;
    struct maa *maa = g_maa_ctx;
    unsigned long flags;

    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        spin_unlock_irqrestore(&maa->reset_lock, flags);
        maa_err("maa is resetting\n");
        return;
    }

    dbg_info->irq_mask = readl(maa->regs + MAA_OPIPE_INT_MSK);
    dbg_info->irq_raw = readl(maa->regs + MAA_OPIPE_INT_RAW);
    dbg_info->timer_en = readl(maa->regs + MAA_TIMER_EN);
    dbg_info->rsl_rptr = readl(maa->regs + MAA_RLS_POOL_RPTR);
    dbg_info->rsl_wptr = readl(maa->regs + MAA_RLS_POOL_WPTR);

    for (level = 0; level < MAA_OPIPE_LEVEL_INVALID; level++) {
        dbg_info->opipe_dbg[level].wptr = readl(maa->regs + MAA_OPIPE_WPTR(level));
        dbg_info->opipe_dbg[level].rptr = readl(maa->regs + MAA_OPIPE_RPTR(level));
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);
}

int bsp_maa_hds_transreport(struct maa_debug_info *dbg_info)
{
    int i = 0;
    struct maa *maa = g_maa_ctx;

    if (maa == NULL || dbg_info == NULL) {
        return -1;
    }

    maa_hds_transreport_hal(dbg_info);
    dbg_info->cur_ts = bsp_get_slice_value();
    dbg_info->int_ts = maa->status.int_ts;
    dbg_info->rsl_full = maa->status.rls_full;
    dbg_info->max_sche_time = maa->status.max_sche_time;
    maa->status.max_sche_time = 0;
    dbg_info->min_sche_time = maa->status.min_sche_time;
    maa->status.min_sche_time = 0x8000;
    dbg_info->work_resetting = maa->status.work_resetting;
    dbg_info->work_start = maa->status.work_start;
    dbg_info->work_end = maa->status.work_end;
    dbg_info->refill_succ = maa->status.refill_succ;
    dbg_info->sche_cnt = maa->status.sche_cnt;
    dbg_info->mem_protect = maa->status.mem_protect;
    dbg_info->map_invalid_level = maa->status.map_invalid_level;
    dbg_info->unmap_invalid_level = maa->status.unmap_invalid_level;
    dbg_info->fifo_timer_en = maa->fifo_timer_en;

    for (i = 0; i < sizeof(dbg_info->fill_cnt) / sizeof(dbg_info->fill_cnt[0]); i++) {
        dbg_info->fill_cnt[i] = maa->status.fill_cnt[i];
    }

    for (i = 0; i < MAA_OPIPE_LEVEL_INVALID; i++) {
        dbg_info->opipe_dbg[i].skb_alloc_success = maa->opipe_status[i].skb_alloc_success;
        dbg_info->opipe_dbg[i].skb_own_free = maa->opipe_status[i].skb_own_free;
        dbg_info->opipe_dbg[i].map_own_success = maa->opipe_status[i].map_own_success;
        dbg_info->opipe_dbg[i].map_own_fail = maa->opipe_status[i].map_own_fail;
        dbg_info->opipe_dbg[i].alloc_map_own_fail = maa->opipe_status[i].alloc_map_own_fail;
        dbg_info->opipe_dbg[i].free_list_empty = maa->opipe_status[i].free_list_empty;
        dbg_info->opipe_dbg[i].skb_alloc_fail = maa->opipe_status[i].skb_alloc_fail;
        dbg_info->opipe_dbg[i].unmap_own = maa->opipe_status[i].unmap_own;
        dbg_info->opipe_dbg[i].map_fail_skb_free = maa->opipe_status[i].map_fail_skb_free;
        dbg_info->opipe_dbg[i].map_tcp_ip_ok = maa->opipe_status[i].map_tcp_ip_ok;
        dbg_info->opipe_dbg[i].map_tcp_ip_fail = maa->opipe_status[i].map_tcp_ip_fail;
        dbg_info->opipe_dbg[i].unmap_tcp_ip = maa->opipe_status[i].unmap_tcp_ip;
        dbg_info->opipe_dbg[i].unmap_test_tcp_ip = maa->opipe_status[i].unmap_test_tcp_ip;
        dbg_info->opipe_dbg[i].delay_free_cnt = maa->opipes[i].delay_free_cnt;
        dbg_info->opipe_dbg[i].reclaim_cnt = maa->opipes[i].reclaim_cnt;
        dbg_info->opipe_dbg[i].max_alloc_cnt = maa->opipes[i].max_alloc_cnt;
        dbg_info->opipe_dbg[i].min_alloc_cnt = maa->opipes[i].min_alloc_cnt;
        dbg_info->opipe_dbg[i].free = maa->opipe_status[i].free;
        dbg_info->opipe_dbg[i].skb_reclaim_free = maa->opipe_status[i].skb_reclaim_free;
        dbg_info->opipe_dbg[i].reclaim_depth = maa->opipes[i].reclaim_depth;
    }

    return 0;
}

static int maa_sys_mem_protect(struct maa *maa)
{
    long avail;
    if (maa->protect_thresh) {
        avail = MAA_CURRETN_AVAIL_MEM;
        if (avail < maa->protect_thresh) {
            maa->status.mem_protect++;
            return 0;
        }
    }
    return 1;
}
#define MAA_HEAD_ROM_SIZE (sizeof(struct maa_head_rom))

void maa_inter_set_mask(unsigned int level, unsigned int set)
{
    unsigned int mask;

    mask = readl_relaxed(g_maa_ctx->regs + MAA_OPIPE_INT_MSK);
    if (!!set) {
        mask |= (1 << level);
    } else {
        mask &= ~(1 << level);
    }
    writel_relaxed(mask, g_maa_ctx->regs + MAA_OPIPE_INT_MSK);
    return;
}

static int maa_ipipe_init(struct maa *maa, struct maa_ipipe *ipipe, unsigned int idx)
{
    struct maa_fifo *fifo = &ipipe->fifo;
    writel((unsigned int)MAA_LOWPHY_ADDR(fifo->phy), (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_L(idx)));
    writel((unsigned int)MAA_HIGHPHY_ADDR(fifo->phy), (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_H(idx)));
    writel(fifo->depth - 1, (void *)(maa->regs + MAA_IPIPE_DEPTH(idx)));

#ifdef CONFIG_MAA_V2
    writel(fifo->uthrl, (void *)(maa->regs + MAA_IPIPE_UP_THRH(idx)));
    writel(fifo->dthrl, (void *)(maa->regs + MAA_IPIPE_DN_THRH(idx)));
#else
    writel(MAA_MAKE_IPIPE_THRH(fifo->uthrl, fifo->dthrl),
           (void *)(maa->regs + MAA_IPIPE_THRH(idx)));
#endif

    writel((unsigned int)MAA_LOWPHY_ADDR(fifo->update_write_addr),
           (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_L(idx)));
    writel((unsigned int)MAA_HIGHPHY_ADDR(fifo->update_write_addr),
           (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_H(idx)));
    writel(0, (void *)(maa->regs + MAA_IPIPE_WPTR(idx)));
    writel(0, (void *)(maa->regs + MAA_IPIPE_RPTR(idx)));
    if (MAA_IPIPE_FOR_ACPU_FREE != idx) {
        writel(1, (void *)(maa->regs + MAA_IPIPE_CTRL(idx)));
    }
    return 0;
}

static int maa_opipe2_lp_ipipe_init(struct maa *maa)
{
    unsigned int idx = MAA_OPIPE2_LP_IPIPE;
    struct maa_fifo *fifo = NULL;
    struct maa_ipipe maa_lp_ipipe;
    int ret;

    ret = (int)memset_s(&maa_lp_ipipe, sizeof(struct maa_ipipe), 0, sizeof(struct maa_ipipe));
    if (ret) {
        maa_err("memset_s fail\n");
    }

    fifo = &maa_lp_ipipe.fifo;

    fifo->depth = MAA_LP_IPIPE_LEN;

    fifo->uthrl = fifo->depth * 3 / 4;
    fifo->dthrl = fifo->depth / 2;
    fifo->update_write_addr = maa->ipipe_write_ptr_phy + MAA_IPIPE_WPTR_PHY_SIZE;
    fifo->phy = maa->ipipe_write_ptr_phy + MAA_IPIPE_WPTR_PHY_SIZE + MAA_LP_IPIPE_WPTR_PHY_SIZE;
    fifo->read = 0;
    fifo->write = 0;
    maa_ipipe_init(maa, &maa_lp_ipipe, idx);

    return 0;
}
static int maa_cpu_ipipe_init(struct maa *maa)
{
    unsigned int idx;
    unsigned int level;
    unsigned long long phy = 0;
    unsigned long long virt;
    struct maa_fifo *fifo = NULL;
    unsigned int i;

    if (!maa->iparamter.cnt) {
        maa_err("maa->iparamter.cnt = 0\n");
        return 0;
    }
    maa->max_cpu_ipipes = maa->iparamter.cnt;
    maa->cpu_ipipes = (struct maa_ipipe *)kzalloc(maa->max_cpu_ipipes * sizeof(struct maa_ipipe), GFP_KERNEL);
    if (maa->cpu_ipipes == NULL) {
        maa_err("cpu_ipipes alloc\n ");
        return 0;
    }
    maa->ipipe_write_ptr_virt = (unsigned int *)dma_alloc_coherent(maa->dev,
                                MAA_IPIPE_WPTR_PHY_SIZE +
                                MAA_LP_IPIPE_WPTR_PHY_SIZE +
                                MAA_LP_IPIPE_LEN * sizeof(maa_buf_t),
                                &maa->ipipe_write_ptr_phy, GFP_KERNEL);
    if (maa->ipipe_write_ptr_virt == NULL) {
        maa_err("g_maa_ipipe_write_ptr = 0\n ");
        goto fail1;
    }

    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        maa->cpu_using_ipipe[i] = maa->max_cpu_ipipes;
    }
    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        idx = maa->iparamter.ipipeidx[i];
        if (MAA_IPIPE_FOR_ACPU_FREE == idx) {
            maa->free_ipipe = &maa->cpu_ipipes[i];
        }
        if (!maa->iparamter.depth[i]) {
            maa_err("ipipe %u depth is zero\n", maa->cpu_ipipes[i].idx);
            continue;
        }
        fifo = &maa->cpu_ipipes[i].fifo;
        spin_lock_init(&maa->cpu_ipipes[i].lock);
        if (!maa->iparamter.phy[i]) {
            virt = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev,
                    maa->iparamter.depth[i] * sizeof(maa_buf_t), &phy,
                    GFP_KERNEL);
            if (!virt) {
                maa_err("ipipe %u base is zero\n", idx);
                continue;
            }
            fifo->base = virt;
            fifo->phy = phy;
        } else {
            /* maa->iparamter.phy[i] = axi mem */
            fifo->base = (unsigned long long)(uintptr_t)ioremap(maa->iparamter.phy[i], maa->iparamter.depth[i]);
            fifo->phy = maa->iparamter.phy[i];
        }

        maa->cpu_ipipes[i].idx = idx;
        level = idx / MAA_MAX_IPIPE_ENDPOINT;
        maa->cpu_ipipes[i].level = level;
        fifo->depth = maa->iparamter.depth[i];
        fifo->size = g_level_size[level];
        if (MAA_OPIPE_LEVEL_INVALID != g_level_size[level]) {
            maa_err("ipipe %u,level %u,size %u,using\n", idx, level, fifo->size);
        }
        if (maa->cpu_using_ipipe[level] == maa->max_cpu_ipipes) {
            maa->cpu_using_ipipe[level] = i;
        }

        fifo->uthrl = fifo->depth * 3 / 4;
        fifo->dthrl = fifo->depth / 2;
        fifo->update_write_addr = maa->ipipe_write_ptr_phy + i * 4;
        fifo->update_write_virt = (unsigned long long)(uintptr_t)maa->ipipe_write_ptr_virt + i * 4;
        maa_ipipe_init(maa, &maa->cpu_ipipes[i], idx);
    }
    /* open ipipe12 for lowpower, clear opipe2 isr */
    maa_opipe2_lp_ipipe_init(maa);

    return 0;
fail1:

    kfree(maa->cpu_ipipes);
    return 0;
}
unsigned long long maa_ipipe_alloc(unsigned int size)
{
    unsigned int level;
    unsigned long flags;
    unsigned int ipipeidx;
    unsigned int cpu_ipipe_idx;
    unsigned int write;
    maa_buf_t buf;
    struct maa_ipipe *ipipe = NULL;
    struct maa *maa = g_maa_ctx;

    level = maa_len_to_levels(size);
    if (level == MAA_OPIPE_LEVEL_INVALID) {
        return 0;
    }
    cpu_ipipe_idx = maa->cpu_using_ipipe[level];

    if (cpu_ipipe_idx >= maa->max_cpu_ipipes) {
        return 0;
    }

    ipipe = maa->cpu_ipipes + cpu_ipipe_idx;
    if (!ipipe->fifo.update_write_virt || !ipipe->fifo.base) {
        return 0;
    }

    ipipeidx = ipipe->idx;

    spin_lock_irqsave(&ipipe->lock, flags);
    write = *(unsigned int *)((uintptr_t)ipipe->fifo.update_write_virt);
    ipipe->fifo.write = write;

    if (ipipe->fifo.write == ipipe->fifo.read) {
        spin_unlock_irqrestore(&ipipe->lock, flags);
        ipipe->debug.ipipe_empty++;
        return 0;
    }
    buf = *((maa_buf_t *)(uintptr_t)ipipe->fifo.base + ipipe->fifo.read);
    ipipe->fifo.read++;
    ipipe->debug.alloc++;
    if (ipipe->fifo.read == ipipe->fifo.depth) {
        ipipe->fifo.read = 0;
    }
    writel_relaxed(ipipe->fifo.read, (void *)(maa->regs + MAA_IPIPE_RPTR(ipipeidx)));
    spin_unlock_irqrestore(&ipipe->lock, flags);

    return buf.addr;
}

static int maa_prepare(struct device *dev)
{
    unsigned int i;
    unsigned int idx;

    if (g_maa_ctx == NULL) {
        return 0;
    }

    for (i = 0; i < g_maa_ctx->max_cpu_ipipes; i++) {
        idx = g_maa_ctx->cpu_ipipes[i].idx;
        writel(0, g_maa_ctx->regs + MAA_IPIPE_CTRL(idx));
    }
    return 0;
}

static void maa_complete(struct device *dev)
{
    unsigned int i;
    unsigned int idx;

    if (g_maa_ctx == NULL) {
        return;
    }

    for (i = 0; i < g_maa_ctx->max_cpu_ipipes; i++) {
        idx = g_maa_ctx->cpu_ipipes[i].idx;
        writel(1, g_maa_ctx->regs + MAA_IPIPE_CTRL(idx));
    }
}

static void maa_ipipe_parse_dt(struct maa *maa)
{
    int ret;
    struct device_node *np = maa->dev->of_node;

    ret = of_property_read_u32(np, "balong,maa-ipipe-max-count", &maa->iparamter.cnt);
    if (ret || !maa->iparamter.cnt) {
        maa_err("no balong,maa-ipipe-max-count,not support cpu alloc buf from maa\n");
        return;
    }

    ret = of_property_read_u32_array(np, "balong,maa-ipipe-levels-idx", maa->iparamter.ipipeidx, maa->iparamter.cnt);
    if (ret) {
        maa_err("no maa-ipipe-levels-idx\n");
        return;
    }

    ret = of_property_read_u32_array(np, "balong,maa-ipipe-levels-base", maa->iparamter.phy, maa->iparamter.cnt);
    if (ret) {
        maa_err("no maa-ipipe-levels-base, use dma_alloc_coherent\n");
    }

    ret = of_property_read_u32_array(np, "balong,maa-ipipe-levels-depth", maa->iparamter.depth, maa->iparamter.cnt);
    if (ret) {
        maa_err("no maa-ipipe-levels-depth, use default \n");
    }
    return;
}

static void maa_opipe_parse_dt(struct maa *maa)
{
    struct device_node *np = maa->dev->of_node;
    int ret;
    unsigned long long virt;
    char opipe_name[MAA_OPIPE_NAME_MAX];
    unsigned int level;

    ret = of_property_read_u32(np, "balong,maa-opipe-max-count", &maa->max_opipes);
    if (ret || !maa->max_opipes || maa->max_opipes > MAA_OPIPE_ALLOC_LEVELS) {
        maa_err("no balong,maa-opipe-max-count,maa->max_opipes = %u \n", maa->max_opipes);
        return;
    }

    ret = of_property_read_u32_array(np, "balong,maa-opipe-levels-depth", maa->oparamter.depth, maa->max_opipes);
    if (ret) {
        maa_err("no maa-opipe-levels-depth, use default \n");
    }

    ret = of_property_read_u32_array(np, "balong,maa-opipe-levels-dthr", maa->oparamter.dthr, maa->max_opipes);
    if (ret) {
        maa->oparamter.dthr_config = 0;
        maa_err("no maa-opipe-levels-depth, use default \n");
    } else {
        maa->oparamter.dthr_config = 1;
    }

    ret = of_property_read_u32_array(np, "balong,maa-opipe-from", maa->oparamter.from, maa->max_opipes);
    if (ret) {
        maa_err("no maa-opipe-from\n");
    }
    ret = of_property_read_u32_array(np, "balong,maa-opipe-tab-size", maa->oparamter.tab_size, maa->max_opipes);
    if (ret) {
        maa_err("no maa-opipe-tab-size\n");
    }
    ret = of_property_read_u32_array(np, "balong,maa-opipe-disable_irq", maa->oparamter.disable_irq, maa->max_opipes);
    if (ret) {
        if (memset_s(maa->oparamter.disable_irq, sizeof(maa->oparamter.disable_irq),
            0, sizeof(unsigned int) * MAA_OPIPE_ALLOC_LEVELS)) {
            maa_err("memset_s failed\n");
        }
    }
    ret = of_property_read_u32_array(np, "balong,maa-use-opipe-free", &maa->opipe_for_free, 1);
    if (maa->opipe_for_free) {
        maa_err("maa-use-opipe-free");
    } else {
        maa_err("maa no use-opipe-free\n");
    }

    ret = of_property_read_u32_array(np, "balong,maa-opipe-for-free", &maa->opipe_free_level, 1);
    maa_err("maa-opipe-for-free level:%d", maa->opipe_free_level);

    ret = of_property_read_u32_array(np, "balong,maa-opipe-use-aximem", &maa->opipe_use_aximem, 1);

    if (maa->opipe_use_aximem) {
        maa_err("maa-opipe-use-aximem \n");
        for (level = 0; level < maa->max_opipes; level++) {
            snprintf_s(opipe_name, MAA_OPIPE_NAME_MAX, MAA_OPIPE_NAME_MAX - 1, "maa_opipe%u", level);

            maa->oparamter.base[level] = bsp_aximem_get(opipe_name, 0, maa->oparamter.depth[level] * MAA_ADDR_SIZE, 0,
                                         &virt);
        }
    }
    ret = of_property_read_u32_array(np, "balong,maa-release-poll-depth", &maa->release.fifo.depth, 1);
    if (ret) {
        maa_err("no maa-release-poll-depth, use default \n");
    }

    ret = of_property_read_u32_array(np, "balong,maa-mem-protect-threshold", &maa->protect_thresh, 1);
    if (ret) {
        maa->protect_thresh = 0;
        maa_err("no balong,maa-mem-protect-threshold, use default \n");
    }
}

static const struct of_device_id g_maa_match[] = {
    { .compatible = "balong,maa31303061" },
    {},
};
MODULE_DEVICE_TABLE(of, g_maa_match);

int maa_freelist_is_empty(struct maa *maa, unsigned int level)
{
    struct maa_map *map = NULL;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid level\n");
        maa->status.map_invalid_level++;
        return -1;
    }

    map = &maa->opipes[level].map;

    if (list_empty(&map->free_list)) {
        maa->opipe_status[level].free_list_empty++;
        return 1;
    }

    return 0;
}

unsigned long long maa_map_single(struct maa *maa, unsigned long long phy, void *manager, int from, unsigned int level)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    maa_buf_t buf;
    unsigned long flags;
    unsigned int bitcnt;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid level\n");
        maa->status.map_invalid_level++;
        return 0;
    }
    map = &maa->opipes[level].map;
    spin_lock_irqsave(&maa->opipes[level].lock, flags);
    if (unlikely(list_empty(&map->free_list))) {
        maa->opipe_status[level].free_list_empty++;
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        return 0;
    }
    buf.addr = 0;
    tab = list_first_entry(&map->free_list, struct maa_tab, list);
    map->free--;
    tab->manager = manager;
    tab->magic = MAA_OWN;
    buf.bits.phyaddr = phy & MAA_PHY_ADDR_MASK;
    buf.bits.idx = tab->idx;
    buf.bits.from = from;
    buf.bits.level = level;
    buf.bits.crc = 0;

    bitcnt = __sw_hweight64(buf.addr);
    if (bitcnt & 1) {
        buf.bits.crc = 1;
    }
    tab->maa_addr = buf.addr;
    map->busy++;
    list_move_tail(&tab->list, &map->busy_list);
    maa->opipes[level].busy_depth++;
    spin_unlock_irqrestore(&maa->opipes[level].lock, flags);

    return buf.addr;
}

struct sk_buff *maa_unmap_tcpip_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;
    unsigned long flags = 0;

    opipe = &maa->opipes[level];
    map = &opipe->map;
    spin_lock_irqsave(&opipe->lock, flags);

    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (unsigned long long)((uintptr_t)skb) + 1;

    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[level].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    spin_unlock_irqrestore(&opipe->lock, flags);

    maa->opipe_status[level].unmap_tcp_ip++;
    if (map->cache) {
        if (skb->len) {
            dma_unmap_single(maa->dev, (dma_addr_t)virt_to_phys(skb->data), skb->len, DMA_TO_DEVICE);
        } else {
            maa_err("skb->len is zero\n");
        }
    }

    return skb;
}
struct sk_buff *__maa_unmap_tcpip_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;
    opipe = &maa->opipes[level];
    map = &opipe->map;
    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (unsigned long long)((uintptr_t)skb) + 1;
    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[level].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    maa->opipe_status[level].unmap_tcp_ip++;
    if (map->cache) {
        if (skb->len) {
            dma_unmap_single(maa->dev, (dma_addr_t)virt_to_phys(skb->data), skb->len, DMA_TO_DEVICE);
        } else {
            maa_err("skb->len is zero\n");
        }
    }
    return skb;
}

static struct sk_buff *__maa_unmap_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid  level maa 0x%llx\n", buf.addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }
    opipe = &maa->opipes[level];
    map = &opipe->map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();
        return NULL;
    }

    tab = &map->tab[idx];

    if (unlikely(MAA_OWN != tab->magic || tab->maa_addr != buf.addr)) {
        maa->opipe_status[level].unmap_own_fail++;
        WARN_ON_ONCE(1);
        maa_err_once("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n", buf.addr, tab->maa_addr, level, idx);
        maa_bug_on();
        return NULL;
    }

    skb = (struct sk_buff *)tab->manager;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (uintptr_t)skb + 1;
    list_move_tail(&tab->list, &map->free_list);
    map->busy--;
    map->free++;
    map->curr = tab->idx;

    if (buf.bits.from && map->cache) {
        dma_unmap_single(g_maa_ctx->dev, (dma_addr_t)buf.bits.phyaddr - MAA_SKB_RESERVE_SIZE,
                         MAA_SKB_RESERVE_SIZE, DMA_FROM_DEVICE);
        rom = (struct maa_head_rom *)phys_to_virt(buf.bits.phyaddr - MAA_SKB_RESERVE_SIZE);
        rom->check = ~MAA_OWN;
        rom->dcheck = ~MAA_OWN;
        rom->maa_addr = 0;
        (void)dma_map_single(maa->dev, rom, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        if (rom->check == MAA_OWN) {
            maa_bug_on();
        }
        maa->opipe_status[level].unmap_cache_flush++;
    }
    maa->opipe_status[level].unmap_own++;
    return skb;
}

struct sk_buff *maa_unmap_skb(struct maa *maa, maa_buf_t buf, unsigned long long *orig)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    unsigned long flags;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;
    maa_buf_t true_addr;
    unsigned long long phyaddr_temp;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid  level maa 0x%llx\n", buf.addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }
    opipe = &maa->opipes[level];
    map = &opipe->map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();
        return NULL;
    }

    spin_lock_irqsave(&opipe->lock, flags);

    tab = &map->tab[idx];

    if (likely(tab->maa_addr > buf.addr)) {
        phyaddr_temp = buf.addr + MAA_SKB_RESERVE_SIZE;
    } else {
        phyaddr_temp = buf.addr;
    }

    if (unlikely(MAA_OWN != tab->magic ||
                 (orig == NULL && tab->maa_addr != buf.addr) ||
                 ((tab->maa_addr & MAA_SKB_RESERVE_MASK) != (phyaddr_temp & MAA_SKB_RESERVE_MASK)))) {
        maa->opipe_status[level].unmap_own_fail++;
        maa_err("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n", buf.addr, tab->maa_addr, level, idx);
        spin_unlock_irqrestore(&opipe->lock, flags);
        maa_bug_on();
        return NULL;
    }

    skb = (struct sk_buff *)tab->manager;
    true_addr.addr = tab->maa_addr;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (uintptr_t)skb + 1;
    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[level].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    spin_unlock_irqrestore(&opipe->lock, flags);

    if (true_addr.bits.from && map->cache) {
        dma_unmap_single(g_maa_ctx->dev, true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE,
                         MAA_SKB_RESERVE_SIZE, DMA_FROM_DEVICE);
        rom = (struct maa_head_rom *)phys_to_virt(true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE);
        rom->check = ~MAA_OWN;
        rom->dcheck = ~MAA_OWN;
        rom->maa_addr = 0;
        (void)dma_map_single(maa->dev, rom, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        if (rom->check == MAA_OWN) {
            maa_bug_on();
        }
        maa->opipe_status[level].unmap_cache_flush++;
    }
    maa->opipe_status[level].unmap_own++;
    if (orig) {
        *orig = true_addr.addr;
    }
    return skb;
}

unsigned long maa_unmap_page(struct maa *maa, maa_buf_t buf)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    unsigned long flags;
    unsigned long page;
    unsigned int idx;

    map = &maa->opipes[MAA_OPIPE_LEVEL_2].map;
    idx = buf.bits.idx;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();
        return 0;
    }
    spin_lock_irqsave(&maa->opipes[MAA_OPIPE_LEVEL_2].lock, flags);
    tab = &map->tab[idx];
    page = (unsigned long)(uintptr_t)tab->manager;
    if (unlikely(MAA_OWN != tab->magic || tab->maa_addr != buf.addr)) {
        maa->opipe_status[MAA_OPIPE_LEVEL_2].unmap_own_fail++;
        WARN_ON_ONCE(1);
        maa_err_once("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n", buf.addr, tab->maa_addr, buf.bits.level,
                     idx);
        spin_unlock_irqrestore(&maa->opipes[MAA_OPIPE_LEVEL_2].lock, flags);
        maa_bug_on();
        return 0;
    }
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (unsigned long long)page + 1;
    maa->opipe_status[MAA_OPIPE_LEVEL_2].unmap_own++;
    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[MAA_OPIPE_LEVEL_2].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    spin_unlock_irqrestore(&maa->opipes[MAA_OPIPE_LEVEL_2].lock, flags);

    if (map->cache) {
        (void)dma_unmap_single(maa->dev, (dma_addr_t)buf.bits.phyaddr, PAGE_SIZE, DMA_FROM_DEVICE);
    }

    return page;
}

int maa_mem_check(struct maa *maa)
{
    unsigned long cnt;
    cnt = global_zone_page_state(NR_FREE_PAGES);
    return cnt > (maa->totalram / maa->mem_line);
}

static unsigned long maa_alloc_page(struct maa *maa, struct maa_opipe *opipe, maa_buf_t *p_addr, unsigned int level)
{
    unsigned long page;
    u64 phy;
    u64 addr;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    page = __get_free_page(GFP_KERNEL);
    if (unlikely(!page)) {
        maa->opipe_status[level].skb_alloc_fail++;
        maa->need_schedule |= BIT(level);
        return 0;
    }
    maa->opipe_status[level].skb_alloc_success++;

    phy = virt_to_phys((void *)(uintptr_t)page);
    addr = maa_map_single(maa, phy, (void *)(uintptr_t)page, MAA_SKB_FROM_OWN, level);
    p_addr->addr = addr;
    if (unlikely(!addr)) {
        free_page(page);
        maa->opipe_status[level].map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        return 0;
    }
    if (opipe->map.cache) {
        (void)dma_map_single(maa->dev, (void *)(uintptr_t)page, PAGE_SIZE, DMA_FROM_DEVICE);
    }
    maa->opipe_status[level].map_own_success++;
    return page;
}

static struct sk_buff *maa_alloc_skb(struct maa *maa, struct maa_opipe *opipe, maa_buf_t *p_addr, gfp_t gfp)
{
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    u64 addr;
    u64 phy;
    unsigned int len = g_level_size[opipe->level];
    unsigned int level = opipe->level;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    skb = alloc_skb(len + MAA_SKB_RESERVE_SIZE, gfp);
    if (unlikely(skb == NULL)) {
        maa->opipe_status[level].skb_alloc_fail++;
        return NULL;
    }
    maa->opipe_status[level].skb_alloc_success++;
    if (gfp == GFP_ATOMIC) {
        maa->opipe_status[level].skb_alloc_atomic++;
    }
    if (gfp == GFP_KERNEL) {
        maa->opipe_status[level].skb_alloc_kernel++;
    }
    phy = virt_to_phys(skb->data);
    addr = maa_map_single(maa, phy + MAA_SKB_RESERVE_SIZE, skb, MAA_SKB_FROM_OWN, level);
    p_addr->addr = addr;
    if (unlikely(!addr)) {
        dev_kfree_skb_any(skb);
        maa->opipe_status[level].alloc_map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        return NULL;
    }
    rom = (struct maa_head_rom *)(skb->data);
    rom->maa_addr = addr;
    rom->dcheck = MAA_OWN;
    rom->check = MAA_OWN;
    if (opipe->map.cache) {
        (void)dma_map_single(maa->dev, skb->data, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        (void)dma_map_single(maa->dev, skb->data, len + MAA_SKB_RESERVE_SIZE, DMA_FROM_DEVICE);
    }
    skb_reserve(skb, MAA_SKB_RESERVE_SIZE);
    maa->opipe_status[level].map_own_success++;
    return skb;
}

unsigned long long maa_alloc_reserve_mem(struct maa *maa, struct maa_opipe *opipe, unsigned int size,
        unsigned int level)
{
    unsigned long long addr;
    unsigned long flags = 0;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    spin_lock_irqsave(&maa->reserve_mem.lock, flags);
    maa->reserve_mem.cur = MAA_BYTE_ALIGN(maa->reserve_mem.cur);
    if (maa->reserve_mem.cur + size > maa->reserve_mem.end) {
        maa->opipe_status[level].skb_alloc_fail++;
        spin_unlock_irqrestore(&maa->reserve_mem.lock, flags);
        return 0;
    }
    addr = (unsigned long long)maa->reserve_mem.cur;
    maa->reserve_mem.cur += size;
    spin_unlock_irqrestore(&maa->reserve_mem.lock, flags);
    maa->opipe_status[level].skb_alloc_success++;
    addr = maa_map_single(maa, addr, (void *)(uintptr_t)addr, MAA_SKB_FROM_OWN, level);
    if (unlikely(!addr)) {
        maa->opipe_status[level].alloc_map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        return 0;
    }
    maa->opipe_status[level].map_own_success++;
    return addr;
}

int maa_free_buf(struct maa *maa, maa_buf_t buf)
{
    int err;
    unsigned int level;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned long flags;
    err = maa_check_addr(maa, buf.addr);
    if (err) {
        maa_err("maa check add error\n");
        maa_bug_on();
        return err;
    }
    level = buf.bits.level;
    map = &maa->opipes[level].map;
    tab = &map->tab[buf.bits.idx];
    if (buf.bits.from == MAA_SKB_FROM_OWN) {
        spin_lock_irqsave(&maa->opipes[level].lock, flags);
        list_move_tail(&tab->list, &map->reclaim_list);
        maa->opipes[level].busy_depth--;
        maa->opipes[level].reclaim_depth++;
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        maa->opipes[level].delay_free_cnt++;
    } else if (buf.bits.from == MAA_SKB_FROM_TCP_IP) {
        skb = maa_unmap_tcpip_skb(maa, buf);
        if (skb != NULL) {
            dev_kfree_skb_any(skb);
            maa->opipe_status[level].skb_tcp_ip_free++;
        }
    } else {
        maa_err("maa check add error, 0x%llx\n", buf.addr);
    }
    return err;
}

void maa_ipipe_free(struct maa *maa, unsigned long long ptr)
{
    maa_buf_t buf;
    buf.addr = ptr;
    maa_free_buf(maa, buf);
}

unsigned int __maa_fill_fifo(struct maa *maa, struct maa_fifo *fifo, unsigned int cnt)
{
    struct sk_buff *skb = NULL;
    void __iomem *base;
    unsigned long page;
    maa_buf_t first;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;

    unsigned int i = 0;
    unsigned long flag = 0;

    struct maa_opipe *opipe = container_of(fifo, struct maa_opipe, fifo);

    base = maa->regs + MAA_ALLOC_ADDR_L(opipe->level, 0);
    map = &opipe->map;
    if (!list_empty(&map->reclaim_list)) {
        spin_lock_irqsave(&opipe->lock, flag);
        for (i = 0; i < cnt; i++) {
            if (!list_empty(&map->reclaim_list)) {
                tab = list_first_entry(&map->reclaim_list, struct maa_tab, list);
                list_move_tail(&tab->list, &map->busy_list);
                opipe->busy_depth++;
                opipe->reclaim_depth--;
                *(unsigned long long *)base = tab->maa_addr;
                opipe->reclaim_cnt++;
            } else {
                break;
            }
        }
        spin_unlock_irqrestore(&opipe->lock, flag);
    }
    for (; i < cnt; i++) {
        if (opipe->from == MAA_CP_MEM) {
            first.addr = maa_alloc_reserve_mem(maa, opipe, fifo->size, opipe->level);
            if (first.addr == 0) {
                break;
            }
        } else {
            if (!maa_sys_mem_protect(maa)) {
                break;
            }
            if (MAA_OPIPE_LEVEL_2 != opipe->level) {
                skb = maa_alloc_skb(maa, opipe, &first, GFP_KERNEL);
                if (unlikely(skb == NULL)) {
                    break;
                }
            } else {
                page = maa_alloc_page(maa, opipe, &first, opipe->level);
                if (unlikely(!page)) {
                    break;
                }
            }
        }
        *(unsigned long long *)base = first.addr;
    }
    return i;
}

unsigned int maa_fill_fifo(struct maa *maa, struct maa_fifo *fifo, unsigned int cnt)
{
    struct sk_buff *skb = NULL;
    void __iomem *base;
    unsigned long page;
    maa_buf_t first;

    unsigned int i ;

    struct maa_opipe *opipe = container_of(fifo, struct maa_opipe, fifo);

    base = maa->regs + MAA_ALLOC_ADDR_L(opipe->level, 0);

    for (i = 0; i < cnt; i++) {
        if (MAA_OPIPE_LEVEL_2 != opipe->level) {
            if (opipe->from == MAA_CP_MEM) {
                first.addr = maa_alloc_reserve_mem(maa, opipe, fifo->size, opipe->level);
                if (first.addr == 0) {
                    break;
                }
            } else {
                skb = maa_alloc_skb(maa, opipe, &first, GFP_KERNEL);
                if (unlikely(skb == NULL)) {
                    break;
                }
            }
        } else {
            page = maa_alloc_page(maa, opipe, &first, opipe->level);
            if (unlikely(!page)) {
                break;
            }
        }
        *(unsigned long long *)base = first.addr;
    }
    return i;
}

extern void maa_release_func(struct maa *maa);

void maa_clear_reclaim(struct maa *maa, unsigned int level)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct maa_tab *tmp = NULL;
    struct maa_opipe *opipe = NULL;
    struct sk_buff *skb = NULL;

    opipe = &maa->opipes[level];
    map = &opipe->map;

    if (opipe->reclaim_depth > MAA_RECLAIM_MIN_DEPTH) {
        list_for_each_entry_safe(tab, tmp, &map->reclaim_list, list) {
            skb = __maa_unmap_skb(maa, (maa_buf_t)tab->maa_addr);
            if (skb != NULL) {
                dev_kfree_skb_any(skb);
                maa->opipe_status[level].skb_reclaim_free++;
            }
            opipe->reclaim_depth--;
            if (opipe->reclaim_depth <= MAA_RECLAIM_MIN_DEPTH) {
                break;
            }
        }
        opipe->reclaim_clear++;
    }
}

static void maa_bigpool_timer(unsigned long data)
{
    int i;
    unsigned long flags;
    struct maa *maa = (struct maa *)(uintptr_t)data;

    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        spin_unlock_irqrestore(&maa->reset_lock, flags);
        mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);
        maa->timer_cnt++;
        maa_err("maa is resetting\n");
        return;
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    for (i = 0; i < MAA_OPIPE_LEVEL_2; i++) {
        spin_lock_irqsave(&maa->opipes[i].lock, flags);
        if (maa->opipes[i].reclaim_depth == maa->opipes[i].last_reclaim_depth) {
            maa_clear_reclaim(maa, i);
        }
        maa->opipes[i].last_reclaim_depth = maa->opipes[i].reclaim_depth;
        spin_unlock_irqrestore(&maa->opipes[i].lock, flags);
    }

    mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);
    maa->timer_cnt++;

    return;
}

int maa_reclaim_fullfill(struct maa *maa)
{
    int ret;
    unsigned int idx;
    unsigned int count;
    ret = of_property_read_u32(maa->dev->of_node, "balong,maa-opipe-fullfill-idx", &idx);
    if (ret) {
        bsp_err("balong,maa-opipe-fullfill-idx not exist\n");
        return -1;
    }

    bsp_err("balong,maa-opipe-fullfill-idx:%d\n", idx);

    ret = of_property_read_u32(maa->dev->of_node, "balong,maa-opipe-fullfill-count", &count);
    if (ret) {
        bsp_err("balong,maa-opipe-fullfill-count not exist\n");
        return -1;
    }

    bsp_err("balong,maa-opipe-fullfill-count:%d\n", count);

    if (count) {
        ret = __maa_fill_fifo(maa, &maa->opipes[idx].fifo, count);
        bsp_err("fullfill:%d\n", ret);
    }

    maa_release_func(maa);

    return ret;
}

struct sk_buff *_maa_to_skb(struct maa *maa, unsigned long long addr)
{
    struct sk_buff *skb = NULL;
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid level maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }

    map = &maa->opipes[level].map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa_err("invalid idx maa 0x%llx\n", addr);
        return NULL;
    }

    tab = &map->tab[idx];

    if (unlikely(MAA_OWN != tab->magic || tab->maa_addr != addr)) {
        maa_err("magic %x, addr 0x%llx, oldaddr 0x%llx, skb 0x%llx\n",
            tab->magic, addr, tab->maa_addr, (unsigned long long)(uintptr_t)tab->manager);
        return NULL;
    }
    skb = (struct sk_buff *)tab->manager;

    return skb;
}

void __maa_release_func(struct maa *maa, struct maa_release_poll *release, unsigned int cnt)
{
    maa_buf_t buf;
    unsigned int i;

    for (i = 0; i < cnt; i++) {
        buf = release->local_release[i];
        maa_free_buf(maa, buf);
    }
}

static inline unsigned int maa_get_release_busy_num(unsigned int w, unsigned r, unsigned int depth)
{
    return ((w >= r) ? (w - r) : (depth - (r - w)));
}

void maa_release_func(struct maa *maa)
{
    struct maa_release_poll *release = &maa->release;
    maa_buf_t *base = (maa_buf_t *)(uintptr_t)release->fifo.base;
    unsigned int max_free_cnt = 0;
    unsigned int idx;
    int ret;
    unsigned int rsl_busy_cnt;
    unsigned int cnt = 0;
    unsigned int tail_num;

    release->fifo.write = readl_relaxed(maa->regs + MAA_RLS_POOL_WPTR);

    while (release->fifo.write != release->fifo.read) {
        rsl_busy_cnt = maa_get_release_busy_num(release->fifo.write, release->fifo.read, release->fifo.depth);
        if (rsl_busy_cnt > MAA_BURST_LEN) {
            cnt = MAA_BURST_LEN;
        } else {
            cnt = rsl_busy_cnt;
        }
        if (release->fifo.read + cnt <= release->fifo.depth) {
            ret = memcpy_s(release->local_release, sizeof(release->local_release), base + release->fifo.read,
                           cnt * sizeof(maa_buf_t));
        } else {
            /* copy two space */
            tail_num = release->fifo.depth - release->fifo.read;
            ret = memcpy_s(release->local_release, sizeof(release->local_release), base + release->fifo.read,
                           tail_num * sizeof(maa_buf_t));
            ret = memcpy_s(release->local_release + tail_num,
                           sizeof(release->local_release) - tail_num * sizeof(maa_buf_t),
                           base, (cnt - tail_num) * sizeof(maa_buf_t));
        }
        if (ret) {
            release->mcp_fail++;
        }
        max_free_cnt += cnt;
        __maa_release_func(maa, release, cnt);
        release->fifo.read += cnt;
        if (release->fifo.read >= release->fifo.depth) {
            release->fifo.read -= release->fifo.depth;
        }
    }
    if (release->max_free_cnt < max_free_cnt) {
        release->max_free_cnt = max_free_cnt;
    }
    if (release->min_free_cnt > max_free_cnt) {
        release->min_free_cnt = max_free_cnt;
    }
    idx = maa->perf.release_perf_debug.idx;
    maa->perf.release_perf_debug.fifo[MAX_PERF_DEBUG_CNT_IDX][idx & (MAX_PERF_DEBUG_CNT - 1)] = max_free_cnt;

    if (maa->dbg & MAA_DEBUG_ALLOC_FREE_TIMESTAMP) {
        maa->perf.release_perf_debug.fifo[MAX_PERF_DEBUG_TIME_IDX][idx & (MAX_PERF_DEBUG_CNT - 1)] =
            bsp_get_slice_value();
    }
    maa->perf.release_perf_debug.idx++;

    writel_relaxed(release->fifo.read, maa->regs + MAA_RLS_POOL_RPTR);
}

static irqreturn_t maa_interrupt(int irq, void *dev_id)
{
    struct maa *maa = (struct maa *)dev_id;
    unsigned int status;
    unsigned int raw_value;
    unsigned int mask;

    mask = MAA_RLS_FULL_WR_INT_MASK | MAA_SAFE_CHK_INT_MASK;
    status = readl(maa->regs + MAA_OPIPE_INT_STAT);
    if (unlikely(status & MAA_SAFE_CHK_INT_MASK)) {
        /* clear raw */
        status &= ~MAA_SAFE_CHK_INT_MASK;
        raw_value = readl(maa->regs + MAA_OPIPE_INT_RAW);
        writel(raw_value | MAA_SAFE_CHK_INT_MASK, maa->regs + MAA_OPIPE_INT_RAW);
        maa->status.safe_check_err++;
        maa_err("erraddr_h: %u\n", readl(maa->regs + MAA_HAC_BP_INVALID_DATA_H));
        maa_err("erraddr_l: %u\n", readl(maa->regs + MAA_HAC_BP_INVALID_DATA_L));
    }
    if (unlikely(status & MAA_RLS_FULL_WR_INT_MASK)) {
        /* clear raw */
        status &= ~MAA_RLS_FULL_WR_INT_MASK;
        raw_value = readl(maa->regs + MAA_OPIPE_INT_RAW);
        writel(raw_value | MAA_RLS_FULL_WR_INT_MASK, maa->regs + MAA_OPIPE_INT_RAW);
        maa->status.rls_full++;
        maa_err("release full \n");
        maa_err("rls_wptr: %u\n", readl(maa->regs + MAA_RLS_POOL_WPTR));
        maa_err("rls_rptr: %u\n", readl(maa->regs + MAA_RLS_POOL_RPTR));
        maa_bug_on();
    }

    if (!maa->release_in_task) {
        if (status & MAA_DDR_RLS_INT_MASK) {
            maa_release_func(maa);
        }
        mask |= MAA_DDR_RLS_INT_MASK;
    }

    __pm_stay_awake(&(maa->wake_lock));
    writel(mask, maa->regs + MAA_OPIPE_INT_MSK);
    queue_work(g_maa_ctx->workqueue,  &g_maa_ctx->fill_work);

    maa->status.int_ts = bsp_get_slice_value();
    maa->status.irq_cnt++;
    return IRQ_HANDLED;
}

void maa_refill_bigpool_work(struct work_struct *work)
{
    unsigned int i;
    unsigned int total;
    unsigned int success;
    unsigned int  status;
    unsigned int  mask = 0;
    struct maa *maa = NULL;
    unsigned long flags = 0;

    maa = container_of(work, struct maa, fill_work);
    maa->status.work_start = bsp_get_slice_value();
    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        maa->status.work_resetting++;
        maa->status.work_end = bsp_get_slice_value();
        spin_unlock_irqrestore(&maa->reset_lock, flags);
        __pm_relax(&(maa->wake_lock));
        maa_err("maa is resetting\n");
        return;
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    status = readl(maa->regs + MAA_OPIPE_INT_RAW);
    status &= maa->int_mask;

    if (maa->release_in_task) {
        maa_release_func(maa);
    }
    if (status & MAA_DDR_RLS_INT_MASK) {
        mask |= MAA_DDR_RLS_INT_MASK;
    }

    for (i = 0; i < maa->max_opipes; i++) {
        if (status & BIT(i)) {
            total = readl(maa->regs + MAA_OPIPE_SPACE(i));
            success = __maa_fill_fifo(maa, &maa->opipes[i].fifo, total);
            if (total == success) {
                mask |= BIT(i);
            } else {
                maa->status.fill_cnt[i]++;
            }
        }
    }

    if (status == mask) {
        writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_MSK);
        __pm_relax(&(maa->wake_lock));
        maa->status.refill_succ++;
    } else {
        maa_dbg("status 0x%x, mask 0x%x\n", status, mask);
        queue_work(g_maa_ctx->workqueue,  &g_maa_ctx->fill_work);
        maa->status.sche_cnt++;
    }
    maa->status.work_end = bsp_get_slice_value();
}

void maa_fifo_init(struct maa *maa, struct maa_fifo *fifo, int level)
{
    writel(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_L(level));
    writel(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_H(level));
    writel(fifo->depth - 1, maa->regs + MAA_OPIPE_DEPTH(level));
    writel(0, maa->regs + MAA_OPIPE_WPTR(level));
    writel(0, maa->regs + MAA_OPIPE_RPTR(level));
    writel(fifo->uthrl, maa->regs + MAA_OPIPE_UP_THRH(level));
    writel(fifo->dthrl, maa->regs + MAA_OPIPE_DN_THRH(level));
    writel(1, maa->regs + MAA_OPIPE_CFG_DONE(level));

    maa_dbg("opipe %d,fifo init depth %u\n", level, fifo->depth);

    if (maa->opipe_for_free && level == maa->opipe_free_level) {
        writel_relaxed(fifo->depth - 5, maa->regs + MAA_OPIPE_WPTR(level));
    } else {
        maa_fill_fifo(maa, fifo, fifo->dthrl);
    }
}

static unsigned int maa_set_dthrl(struct maa *maa, int level)
{
    struct maa_fifo *fifo = &maa->opipes[level].fifo;

    if (maa->oparamter.dthr_config) {
        return maa->oparamter.dthr[level];
    }
    switch (level) {
        case MAA_OPIPE_LEVEL_0:
        case MAA_OPIPE_LEVEL_1:
            return 3 * (fifo->depth >> 2);
        case MAA_OPIPE_LEVEL_4:
        case MAA_OPIPE_LEVEL_5:
            return fifo->depth >> 2;
        default:
            return fifo->depth - MAA_TWO_BURST_RESERVE - 1;
    }
}

int maa_opipe_init(struct maa *maa, int level)
{
    struct maa_map *map = &maa->opipes[level].map;
    struct maa_fifo *fifo = &maa->opipes[level].fifo;
    struct maa_tab *tab = NULL;
    struct maa_tab *tmp = NULL;
    int j;

    INIT_LIST_HEAD(&map->free_list);
    INIT_LIST_HEAD(&map->busy_list);
    INIT_LIST_HEAD(&map->reclaim_list);
    skb_queue_head_init(&maa->opipes[level].free_queue);
    spin_lock_init(&maa->opipes[level].lock);
    spin_lock_init(&maa->opipes[level].refill_lock);

    map->free = 0;
    map->busy = 0;
    map->total = 0;
    fifo->depth = maa->oparamter.depth[level];
    maa->opipes[level].from = maa->oparamter.from[level];

    map->tab_max_size = maa->oparamter.tab_size[level];
    maa_err("level:%d, tab_size:%d\n", level, map->tab_max_size);

    tab = (struct maa_tab *)kvzalloc(sizeof(struct maa_tab) * map->tab_max_size, GFP_KERNEL);
    if (tab == NULL) {
        maa_err("no map tab alloc\n");
        return -ENOMEM;
    }

    map->tab = tab;
    map->cache = 1;
    if (!maa->oparamter.base[level]) {
        fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t),
                     &fifo->phy, GFP_KERNEL);
        if (!fifo->base) {
            maa_err("no map tab alloc\n");
            goto no_mem;
        }
    } else {
        fifo->base = maa->oparamter.base[level];
        fifo->phy = maa->oparamter.base[level];
    }

    fifo->size = g_level_size[level];
    fifo->uthrl = fifo->depth - MAA_BURST_LEN - 1;
    fifo->dthrl = maa_set_dthrl(maa, level);
    fifo->free = fifo->depth - fifo->dthrl;

    for (j = MAA_TABEL_START; j < map->tab_max_size; j++) {
        tmp = tab + j;
        tmp->idx = j;
        map->total++;
        map->free++;
        list_add_tail(&tmp->list, &map->free_list);
    }
    maa->opipes[level].status = MAA_OPIPE_STATUS_INIT;
    maa->opipes[level].level = level;
    maa_fifo_init(maa, fifo, level);
    if (!maa->oparamter.disable_irq[level]) {
        maa->int_mask |= 1 << (unsigned int)level;
    }
    return 0;

no_mem:
    if (map->tab != NULL) {
        kvfree(map->tab);
    }
    if (!maa->oparamter.base[level] && fifo->base) {
        dma_free_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t), (void *)(uintptr_t)fifo->base, fifo->phy);
    }
    return -ENOMEM;
}

struct sk_buff *maa_to_skb(struct maa *maa, unsigned long long addr, unsigned long long *orig)
{
    struct sk_buff *skb = NULL;
    unsigned long flags;
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(level > (maa->max_opipes - 1))) {
        maa_err("invalid level maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }

    map = &maa->opipes[level].map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", addr);
        return NULL;
    }
    spin_lock_irqsave(&maa->opipes[level].lock, flags);
    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    *orig = tab->maa_addr;
    if (unlikely(MAA_OWN != tab->magic || !tab->maa_addr)) {
        maa->opipe_status[level].unmap_own_fail++;
        WARN_ON_ONCE(1);
        maa_err_once("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n", buf.addr, tab->maa_addr, level, idx);
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        return NULL;
    }
    spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
    return skb;
}

int maa_release_opipe_init(struct maa *maa)
{
    unsigned int level = maa->opipe_free_level;
    struct maa_fifo *fifo = &maa->opipes[level].fifo;

    fifo->depth = MAA_RELEASE_OPIPE_DEPTH;
    fifo->uthrl = MAA_RELEASE_OPIPE_UTHRL; /* up th */
    fifo->dthrl = MAA_RELEASE_OPIPE_DTHRL; /* up th */
    fifo->read = 0;
    fifo->write = 0;
    /* should use aximem */
    if (!fifo->base) {
        fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t),
                     &fifo->phy, GFP_KERNEL);
    }

    if (!fifo->base) {
        maa_err("release opipe is null \n");
        return -ENOMEM;
    }
    maa_fifo_init(maa, fifo, level);

    return 0;
}

void __maa_release_pool_init(struct maa *maa)
{
    struct maa_fifo *fifo = &maa->release.fifo;
    writel(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_RLS_POOL_ADDR_L);
    writel(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_RLS_POOL_ADDR_H);
    writel(fifo->depth - 1, maa->regs + MAA_RLS_POOL_DEPTH);
    fifo->uthrl = 1;
    writel(fifo->uthrl, maa->regs + MAA_RLS_POOL_UP_THRH);
    writel(0, maa->regs + MAA_RLS_POOL_WPTR);
    writel(0, maa->regs + MAA_RLS_POOL_RPTR);
}

int maa_release_pool_init(struct maa *maa)
{
    struct maa_fifo *fifo = &maa->release.fifo;

    fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev,
        (fifo->depth + MAA_TWO_BURST_RESERVE) * sizeof(maa_buf_t),
        &fifo->phy, GFP_KERNEL);
    if (!fifo->base) {
        maa_err("release fifo is null \n");
        return -ENOMEM;
    }

    __maa_release_pool_init(maa);

    return 0;
}

void maa_global_init(struct maa *maa)
{
    struct device_node *np = maa->dev->of_node;
    int ret;

    maa_opipe_parse_dt(maa);
    maa_ipipe_parse_dt(maa);

    ret = of_property_read_u32(np, "balong,maa-mem-line", &maa->mem_line);
    if (ret || (maa->mem_line == 0)) {
        maa_err("no balong,maa-mem-line");
        maa->mem_line = MAA_MEM_LINE_DEAULT;
    }

    ret = of_property_read_u32(np, "balong,maa-release-in-task", &maa->release_in_task);
    if (ret) {
        maa->release_in_task = 0;
    }

    maa->fifo_timer_en = MAA_TIMER_AP_OPIPE_EN_VALUE | BIT(maa->opipe_free_level);
    writel(maa->fifo_timer_en, maa->regs + MAA_TIMER_EN);
    maa_err("timer_en: %d, MAA_TIMER_EN: %d", maa->fifo_timer_en,
        readl(maa->regs + MAA_TIMER_EN));
    writel(MAA_TIMER_CNT, maa->regs + MAA_TIMER_WATCHCNT);
    writel(MAA_CNT_CLK_EN_VAL, maa->regs + MAA_CNT_CLK_EN);
    maa->int_mask = MAA_RLS_FULL_WR_INT_MASK | MAA_SAFE_CHK_INT_MASK | MAA_DDR_RLS_INT_MASK;
}

void maa_interupt_init(struct maa *maa)
{
    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_RAW);
    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_MSK);
    writel_relaxed(0, maa->regs + MAA_IPIPE_INT_MSK0);
    return;
}

static int maa_suspend(struct device *dev)
{
    unsigned int i = 0;

    maa_err("maa_suspend+\n");
    if (g_maa_ctx == NULL) {
        return 0;
    }
    /* close timer_en and turn down opipe down waterline */
    writel(0, g_maa_ctx->regs + MAA_TIMER_EN);
    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        writel(g_maa_ctx->opipes[i].fifo.dthrl / 4, g_maa_ctx->regs + MAA_OPIPE_DN_THRH(i));
    }
    writel(MAA_RELEASE_THRSH_LP, g_maa_ctx->regs + MAA_RLS_POOL_UP_THRH);

    writel_relaxed(g_maa_ctx->int_mask, g_maa_ctx->regs + MAA_OPIPE_INT_MSK);
    maa_err("maa_suspend-\n");
    return 0;
}

static int maa_resume(struct device *dev)
{
    unsigned int i = 0;
    unsigned int status;
    maa_err("maa_resume+\n");
    status = readl(g_maa_ctx->regs + MAA_OPIPE_INT_STAT);
    if (status & g_maa_ctx->int_mask) {
        maa_err("maa resumed with irq(%x) pending\n", status);
    }

    /* restore maa timer_en and opipe down waterline */
    writel(g_maa_ctx->fifo_timer_en, g_maa_ctx->regs + MAA_TIMER_EN);
    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        writel(g_maa_ctx->opipes[i].fifo.dthrl, g_maa_ctx->regs + MAA_OPIPE_DN_THRH(i));
    }
    writel(g_maa_ctx->release.fifo.uthrl, g_maa_ctx->regs + MAA_RLS_POOL_UP_THRH);
    maa_err("maa_resume-\n");
    return 0;
}

void maa_reserve_mem_init(struct maa *maa)
{
    int ret;
    struct device_node *np = maa->dev->of_node;
#ifdef DDR_MAA_MDM_ADDR
    unsigned long maa_addr;
    unsigned int maa_size;
#endif
    ret = of_property_read_u32_array(np, "balong,maa-reserve-mem-size", &maa->reserve_mem.size, 1);
    if (ret) {
        maa_err("no maa-reserve-mem\n");
    }
    spin_lock_init(&maa->reserve_mem.lock);
#ifdef DDR_MAA_MDM_ADDR
    maa_addr = mdrv_mem_region_get("mdm_maa_ddr", &maa_size);
    maa->reserve_mem.start = maa_addr;
#else
    maa->reserve_mem.start = 0;
#endif
    maa->reserve_mem.cur = maa->reserve_mem.start;
#ifdef DDR_MAA_MDM_SIZE
    maa->reserve_mem.size = maa_size;
#else
    maa->reserve_mem.size = 0;
#endif
    maa->reserve_mem.end = maa->reserve_mem.start + maa->reserve_mem.size;
}

int maa_core_probe(struct maa *maa)
{
    int ret = 0;
    int level;

    maa->clk = devm_clk_get(maa->dev, "maa_clk");
    if (IS_ERR_OR_NULL(maa->clk)) {
        maa_err("maa clock not available\n");
    } else {
        ret = clk_prepare_enable(maa->clk);
        if (ret) {
            maa_err("failed to enable maa clock\n");
        }
    }
    if (SKB_DATA_ALIGN(MAA_HEAD_ROM_SIZE) > NET_SKB_PAD) {
        maa_err("head rom size must litter than NET_SKB_PAD(%d)\n", NET_SKB_PAD);
        return -ENOMEM;
    }

    maa_global_init(maa);

    wakeup_source_init(&maa->wake_lock, "modem_maa_wake");

    INIT_WORK(&maa->fill_work, maa_refill_bigpool_work);
    maa->workqueue = create_singlethread_workqueue("maa_refill");
    if (maa->workqueue == NULL) {
        maa_err("maa workqueue alloc\n");
        goto err_dmaunmap;
    }

    maa_reserve_mem_init(maa);

    maa->timeout_jiffies = MAA_TIMER_EXPIRES;

    ret = maa_release_pool_init(maa);
    if (ret) {
        goto err_dmaunmap;
    }

    for (level = 0; level < maa->max_opipes; level++) {
        ret = maa_opipe_init(maa, level);
        if (ret) {
            goto err_dmaunmap;
        }
    }

    maa_cpu_ipipe_init(maa);
    /* use level5 for directly free up packet */
    if (maa->opipe_for_free) {
        maa_release_opipe_init(maa);
    }

    init_timer(&maa->timer);
    maa->timer.function = maa_bigpool_timer;
    maa->timer.data = (uintptr_t)maa;
    maa->timer.expires = jiffies + MAA_TIMER_EXPIRES;

    ret = devm_request_irq(maa->dev, maa->irq, maa_interrupt, maa->irq_flags, "maa", maa);
    if (ret) {
        maa_err("irq=%u  request fail\n", maa->irq);
        goto err_dmaunmap;
    }

    spin_lock_init(&maa->reset_lock);

    maa_reclaim_fullfill(maa);
    maa_interupt_init(maa);
    add_timer(&maa->timer);

    bsp_err("bsp_reset_cb_func_register\n");
    if (bsp_reset_cb_func_register("MAA_BALONG", maa_mdmreset_cb, 0, DRV_RESET_CB_PIOR_MAA)) {
        bsp_err("set modem reset call back func failed\n");
    }
    maa_err("[init] ok\n");

    return 0;

err_dmaunmap:

    if (maa->clk != NULL) {
        clk_disable_unprepare(maa->clk);
    }

    return ret;
}

static int maa_remove(struct platform_device *pdev)
{
    return 0;
}
void __maa_set_backup_regs(struct maa *maa, unsigned int idx, unsigned int start, unsigned int end)
{
    unsigned int num;
    if (maa == NULL) {
        maa_err("maa is null\n");
        return;
    }

    num = maa->back_regs_num;

    if (idx < MAA_BACK_REGS_NUM && (maa != NULL)) {
        num += maa->back[idx].num;
        if (num < MAA_DUMP_REG_NUM) {
            maa->back[idx].start = (char *)maa->regs + start;
            maa->back[idx].num = (end - start) / sizeof(unsigned int) + 1;
            maa->back_regs_num = num;
        }
    }
}

void maa_set_backup_regs(struct maa *maa)
{
    __maa_set_backup_regs(maa, 0, MAA_GLB_CTRL, MAA_HAC_BP_INVALID_DATA_H);
    __maa_set_backup_regs(maa, 1, MAA_OPIPE_REG_START, MAA_OPIPE_REG_END);
}

void maa_dump_hook(void)
{
    unsigned int *reg = NULL;
    unsigned int i, j;
    unsigned int num = 0;
    struct maa *maa = g_maa_ctx;

    if (maa == NULL) {
        return;
    }

    for (i = 0; i < MAA_BACK_REGS_NUM && (maa != NULL); i++) {
        for (j = 0; j < maa->back[i].num; j++) {
            reg = maa->dump_base + num + j;
            *reg = readl((void *)(maa->back[i].start + j * sizeof(unsigned int)));
        }
        num += maa->back[i].num;
    }
}

static void maa_dump_init(struct maa *maa)
{
    int ret;

    maa->dump_base = (unsigned int *)bsp_dump_register_field(DUMP_MODEMAP_MAA, "MAA", MAA_DUMP_SIZE, 0);
    if (NULL == maa->dump_base) {
        maa_err("dump mem alloc fail\n");
        return;
    }
    ret = (int)memset_s(maa->dump_base, MAA_DUMP_SIZE, 0, MAA_DUMP_SIZE);
    if (ret) {
        maa_err("dump memset_s fail\n");
    }

    ret = bsp_dump_register_hook("maa", maa_dump_hook);
    if (ret == BSP_ERROR) {
        maa_err("register om fail\n");
    }
}
static int maa_probe(struct platform_device *pdev)
{
    struct maa *maa = NULL;
    struct resource *regs = NULL;

    maa_err("[init] start\n");
    maa = devm_kzalloc(&pdev->dev, sizeof(*maa), GFP_KERNEL);
    if (maa == NULL) {
        maa_err("no map tab alloc\n");
        return -ENOMEM;
    }
    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(MAA_U64_BITLEN));
    maa->irq = platform_get_irq_byname(pdev, "opipe_dnirq");
    if (maa->irq < 0) {
        maa_err("opipe_dnirq get fail\n");
        return maa->irq;
    }

    maa->dev = &pdev->dev;
    maa->irq_flags = 0;

    regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (regs == NULL) {
        maa_err("regs is err\n");
        return -ENOMEM;
    }
    maa->regs = devm_ioremap_resource(&pdev->dev, regs);
    if (IS_ERR(maa->regs)) {
        maa_err("maa->regs is err\n");
        return PTR_ERR(maa->regs);
    }
    maa_dump_init(maa);

    maa->phy_regs = regs->start;
    g_maa_ctx = maa;
    maa_set_backup_regs(maa);
    maa->dbg = (MAA_DEBUG_ALLOC_FREE_TIMESTAMP | MAA_DEBUG_TIMER_EN);
    maa->totalram = totalram_pages;

    return maa_core_probe(maa);
}

static const struct dev_pm_ops g_maa_pmops = {
    .suspend = maa_suspend,
    .resume = maa_resume,
    .prepare = maa_prepare,
    .complete = maa_complete,
};

static struct platform_driver g_maa_pltfm_driver = {
    .probe		= maa_probe,
    .remove		= maa_remove,
    .driver		= {
        .name		= "balong_maa",
        .of_match_table	= g_maa_match,
        .pm		= &g_maa_pmops,
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

int maa_init(void)
{
    return platform_driver_register(&g_maa_pltfm_driver);
}

void maa_detach(struct maa *maa)
{
    int i, idx;

    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_SPE_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_SPE_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_SPE_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_SPE_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_IPF_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_IPF_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_IPF_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_IPF_2K));

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        idx = maa->cpu_ipipes[i].idx;
        writel(0, maa->regs + MAA_IPIPE_CTRL(idx));
    }
}

void maa_fifo_reinit(struct maa *maa, struct maa_fifo *fifo, int level)
{
    writel_relaxed(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_L(level));
    writel_relaxed(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_H(level));
    writel_relaxed(fifo->depth - 1, maa->regs + MAA_OPIPE_DEPTH(level));
    writel_relaxed(0, maa->regs + MAA_OPIPE_WPTR(level));
    writel_relaxed(0, maa->regs + MAA_OPIPE_RPTR(level));
    writel_relaxed(fifo->uthrl, maa->regs + MAA_OPIPE_UP_THRH(level));
    writel_relaxed(fifo->dthrl, maa->regs + MAA_OPIPE_DN_THRH(level));
    writel(1, maa->regs + MAA_OPIPE_CFG_DONE(level));
    maa_dbg("opipe %d,fifo init depth %u\n", level, fifo->depth);
    fifo->read = 0;
    fifo->write = 0;
    if (level != MAA_OPIPE_ALLOC_LEVELS) {
        __maa_fill_fifo(maa, fifo, fifo->dthrl);
    } else {
        if (maa->opipe_for_free) {
            writel_relaxed(fifo->uthrl - 1, maa->regs + MAA_OPIPE_WPTR(level));
        }
    }
}
int maa_deinit(void)
{
    struct maa *maa = g_maa_ctx;
    unsigned long flags = 0;

    maa_detach(maa);
    disable_irq(maa->irq);

    spin_lock_irqsave(&maa->reset_lock, flags);
    maa->resetting = 1;
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    flush_workqueue(maa->workqueue);
    mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);

    maa->deinit_cnt++;
    return 0;
}

int maa_interrupt_reinit(void)
{
    struct maa *maa = g_maa_ctx;
    maa_interupt_init(maa);
    enable_irq(maa->irq);
    return 0;
}
int bsp_maa_reinit(void)
{
    int level;
    int i;
    struct maa *maa = g_maa_ctx;
    unsigned long flags = 0;
    struct maa_tab *pos = NULL;
    struct maa_tab *n = NULL;
    struct sk_buff *skb = NULL;
    maa_buf_t buf;

    for (level = 0; level < maa->max_opipes; level++) {
        spin_lock_irqsave(&maa->opipes[level].lock, flags);
        list_splice_init(&maa->opipes[level].map.busy_list,
                         &maa->opipes[level].map.reclaim_list);
        maa->opipes[level].reclaim_depth += maa->opipes[level].busy_depth;
        maa->opipes[level].delay_free_cnt += maa->opipes[level].busy_depth;
        maa->opipes[level].busy_depth = 0;
        list_for_each_entry_safe(pos, n, &maa->opipes[level].map.reclaim_list, list) {
            buf.addr = pos->maa_addr;
            if (buf.bits.from == MAA_SKB_FROM_TCP_IP) {
                skb = __maa_unmap_tcpip_skb(maa, buf);
                dev_kfree_skb_any(skb);
                maa->opipes[level].reclaim_depth--;
            }
        }
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
    }
    for (level = 0; level < maa->max_opipes; level++) {
        maa_fifo_reinit(maa, &maa->opipes[level].fifo, level);
    }
    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        if (!maa->iparamter.depth[i]) {
            maa_err("ipipe %u depth is zero\n", maa->cpu_ipipes[i].idx);
            continue;
        }
        maa_ipipe_init(maa, &maa->cpu_ipipes[i], maa->iparamter.ipipeidx[i]);
    }
    maa_opipe2_lp_ipipe_init(maa);
    if (maa->opipe_for_free) {
        maa_release_opipe_init(maa);
    }
    __maa_release_pool_init(maa);
    maa->release.fifo.read = 0;
    maa->release.fifo.write = 0;
    spin_lock_irqsave(&maa->reset_lock, flags);
    maa->resetting = 0;
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    maa_interrupt_reinit();

    maa->reinit_cnt++;
    return 0;
}

int maa_mdmreset_cb(drv_reset_cb_moment_e eparam, int userdata)
{
    if (MDRV_RESET_CB_BEFORE == eparam) {
        maa_deinit();
    }
    return 0;
}

void maa_mdmreset_status(void)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("timer cnt:%d\n", maa->timer_cnt);
    bsp_err("resetting:%d\n", maa->resetting);
    bsp_err("deinit_cnt:%d\n", maa->deinit_cnt);
    bsp_err("reinit_cnt:%d\n", maa->reinit_cnt);
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
core_initcall(maa_init);
#endif
void maa_exit(void)
{
    /*
     * don't need kfree platform_data in exit process,
     * platform_driver_unregister will do it.
     */
    platform_driver_unregister(&g_maa_pltfm_driver);

    return;
}

MODULE_DESCRIPTION("Balong Specific MAA Driver");
MODULE_LICENSE("GPL v2");
