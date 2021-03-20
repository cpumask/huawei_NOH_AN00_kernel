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

#include <osl_generic.h>
#include <osl_spinlock.h>
#include <osl_malloc.h>
#include <securec.h>
#include <bsp_nvim.h>
#include <bsp_ipc.h>
#include <bsp_slice.h>
#include <bsp_modem_log.h>
#include <bsp_pm_om.h>
#include <osl_module.h>
#include <nv_stru_drv.h>
#include "pm_om_platform.h"
#include "pm_om_debug.h"
#include <mdrv_memory_layout.h>

#undef THIS_MODU
#define THIS_MODU mod_pm_om

struct pm_om_ctrl g_pmom_ctrl;
u32 g_pm_om_magic_tbl[PM_OM_MOD_ID_ENUM_MAX] = {
    PM_OM_MAGIC_PMA,  /* 0-PMA  : PM ACORE  */
    PM_OM_MAGIC_PMC,  /* 1-PMC  : PM CCORE  */
    PM_OM_MAGIC_PMM,  /* 2-PMM  : PM MCORE  */
    PM_OM_MAGIC_DPM,  /* 3-DPM  : DPM */
    PM_OM_MAGIC_ABB,  /* 4-ABB  : ABB */
    PM_OM_MAGIC_BBP,  /* 5-BBP  : BBP */
    PM_OM_MAGIC_DSP,  /* 6-DSP  : DSP */
    PM_OM_MAGIC_PMU,  /* 7-PMU  : PMU */
    PM_OM_MAGIC_WAKE, /* 8-WAKE : WAKELOCK */
    PM_OM_MAGIC_CPUF, /* 9-CPUF : CPUFREQ */
    PM_OM_MAGIC_CCLK, /* 10-CCLK : CLK CCORE */
    PM_OM_MAGIC_REGU, /* 11-REG  : REGULATOR */
    PM_OM_MAGIC_ACLK, /* 12-ACLK : CLK ACORE */
    PM_OM_MAGIC_AIPC, /* 13-AIPC : IPC ACORE */
    PM_OM_MAGIC_CIPC, /* 14-CIPC : IPC CCORE */
    PM_OM_MAGIC_AIPF, /* 15-AIPF : IPF ACORE */
    PM_OM_MAGIC_CIPF, /* 16-CIPF : IPF CCORE */
    0,
    PM_OM_MAGIC_COSA,     /* 18-COSA : OSA CCORE */
    PM_OM_MAGIC_CMSP,     /* 19-CMSP : MSP CCORE */
    PM_OM_MAGIC_NVA,      /* 20-NVA : NV */
    PM_OM_MAGIC_NVC,      /* 21-NVC : NV */
    PM_OM_MAGIC_ADPPOWER, /* 22-ADPP: ADPPOWER */
    PM_OM_MAGIC_AICC,     /* 23-AICC: ICC ACORE */
    PM_OM_MAGIC_CICC,     /* 24-CICC: ICC CCORE  */
    PM_OM_MAGIC_ARFILE,   /* 25-FILE: RFILE ACORE */
    PM_OM_MAGIC_PMLG,     /* 26-PMLG: LOG FILE  */
    PM_OM_MAGIC_TCXO,     /* 27-TCXO: CCORE  */
    PM_OM_MAGIC_RFFE,     /* 28-RFFE: CCORE */
    PM_OM_MAGIC_CDSP,     /* 29-CDSP: CDSP */
    PM_OM_MAGIC_MDRV,     /* 30-MDRV: ALL DRV PM INFO */
    0,                    /* 31- NULL */
    0,                    /* 32- NULL */
    PM_OM_MAGIC_CASX,     /* 33-CASX : CAS 1X    */
    PM_OM_MAGIC_CPRX,     /* 34-CPRX : CPROC 1X  */
    PM_OM_MAGIC_CASE,     /* 35-CASE : CAS EVDO  */
    PM_OM_MAGIC_CPRE,     /* 36-CPRE : CPROC EVDO */
    PM_OM_MAGIC_TLPY,     /* 37-TLPH : TL PHY    */
    0,
    0,
    0,
    0,
    0,
    PM_OM_MAGIC_AOSA, /* 43-AOSA : OSA ACORE */
    0,
    0,
    0,
    0
};
u32 ring_buffer_in(struct ring_buffer *rb, const void *data, u32 len)
{
    /* 首先拷贝 rb->write到buffer末尾的一段数据 */
    int ret;
    u32 left = min(len, rb->size - rb->write);

    ret = memcpy_s((void *)(rb->buf + rb->write), rb->size, (void *)data, (unsigned long)left); /*lint !e124 */
    if (ret) {
        pmom_pr_err("rb->buf + rb->write memcpy ret = %d\n", ret);
    }
    /* 接着将剩余待写入数据 (如果还有的话)从buffer的开始位置拷贝 */
    ret = memcpy_s((void *)rb->buf, rb->size, (void *)(data + left), (unsigned long)(len - left)); /*lint !e124 */
    if (ret) {
        pmom_pr_err("rb->buf memcpy ret = %d\n", ret);
    }

    rb->write += len;
    rb->write %= (rb->size);

    return len;
}

u32 ring_buffer_in_with_header(struct ring_buffer *rb, const void *head, u32 head_len, const void *data, u32 data_len)
{
    u32 len;

    len = ring_buffer_in(rb, head, head_len);
    if (data != NULL) {
        len += ring_buffer_in(rb, data, data_len);
    }
    return len;
}

int pm_om_log_out(u32 mod_id, u32 typeid, u32 data_len, const void *data)
{
    struct pm_om_log_header log_head;
    u32 head_len = sizeof(log_head);
    u32 left_size;
    int ret = PM_OM_OK;
    unsigned long flags;
    if (mod_id >= PM_OM_MOD_ID_ENUM_MAX || ((data == NULL) && (data_len != 0))) {
        pmom_pr_err("mod_id:(%d), data_len = 0x%x\n", mod_id, data_len);
        return PM_OM_ERR;
    }
    /* 记录log过长会影响DRX寻呼(栈上变量) */
    if (data_len > PM_OM_LOG_MAX_LEN) {
        return PM_OM_ERR_LEN_LIMIT;
    }

    if (g_pmom_ctrl.log.init_flag != PM_OM_INIT_MAGIC) {
        return PM_OM_ERR_INIT;
    }

    if (pm_om_log_mod_is_off(g_pmom_ctrl.log.smem->nv_cfg.mod_sw_bitmap, mod_id)) {
        return PM_OM_ERR_MOD_OFF;
    }

    log_head.typeid = typeid;
    log_head.length = data_len;
    log_head.timestamp = bsp_get_slice_value();
    log_head.magic = g_pm_om_magic_tbl[mod_id];
    pm_om_spin_lock(&g_pmom_ctrl.log.lock, flags);
    /* 操作ring buffer之前, 先将核间指针取到本核 */
    g_pmom_ctrl.log.rb.write = g_pmom_ctrl.log.smem->mem_info.write;
    g_pmom_ctrl.log.rb.read = g_pmom_ctrl.log.smem->mem_info.read;
    log_head.sn = (g_pmom_ctrl.log.smem->sn)++;

    /* 出现log覆盖: 抛弃新数据 */
    left_size = bsp_ring_buffer_writable_size(&g_pmom_ctrl.log.rb);
    if ((head_len + data_len) > left_size) {
        g_pmom_ctrl.log.buf_is_full = 1;
        goto out; /*lint !e801 */
    } else {
        g_pmom_ctrl.log.buf_is_full = 0;
    }

    (void)ring_buffer_in_with_header(&g_pmom_ctrl.log.rb, (void *)&log_head, head_len, data, data_len);

    /* 数据写入之后更新核间ring buffer 写指针 */
    g_pmom_ctrl.log.smem->mem_info.write = g_pmom_ctrl.log.rb.write;
    cache_sync();

out:
    /* bsp_ipc_spin_unlock可保证调用cache_sync冲出write buffer */
    pm_om_spin_unlock((&g_pmom_ctrl.log.lock), flags);

    /* 写入数据超过log水线,唤醒acore写文件 */
    if (bsp_ring_buffer_readable_size(&g_pmom_ctrl.log.rb) >= g_pmom_ctrl.log.threshold &&
        !g_pmom_ctrl.log.buf_is_full) {
        (void)pm_om_fwrite_trigger();
    }

    return ret;
}

struct pm_om_ctrl *pm_om_ctrl_get(void)
{
    return &g_pmom_ctrl;
}

int bsp_pm_log(u32 mod_id, u32 data_len, const void *data)
{
    return pm_om_log_out(mod_id, 0, data_len, data);
}

int bsp_pm_log_type(u32 mod_id, u32 type, u32 data_len, const void *data)
{
    return pm_om_log_out(mod_id, type, data_len, data);
}

void *bsp_pm_log_addr_get(void)
{
    if (g_pmom_ctrl.log.init_flag != PM_OM_INIT_MAGIC) {
        pmom_pr_err("pm om not init\n");
        return NULL;
    }
    return (void *)g_pmom_ctrl.log.rb.buf;
}

void *bsp_pm_dump_get(u32 mod_id, u32 len)
{
    struct pm_om_head_entry *entry = NULL;
    void *dump_area = NULL;
    u32 reg_cnt = g_pmom_ctrl.dump.region_cnt;
    u32 i;
    unsigned long flags;

    len = OSL_ROUND_UP(len, 4); /*lint !e712: (Info -- Loss of precision (assignment) (long to unsigned int))*/

    if (g_pmom_ctrl.dump.init_flag != PM_OM_INIT_MAGIC) {
        pmom_pr_err("not init\n");
        return NULL;
    }

    entry = g_pmom_ctrl.dump.cfg ? g_pmom_ctrl.dump.cfg->entry_tbl : NULL;
    if (entry == NULL) {
        pmom_pr_err("no dump memory\n");
        return NULL;
    }

    for (i = 0; i < reg_cnt; i++) {
        if (g_pm_om_magic_tbl[mod_id] == entry[i].magic) {
            return (void *)(uintptr_t)entry[i].offset;
        }
    }

    if (len + (u32)(g_pmom_ctrl.dump.sub_region - g_pmom_ctrl.dump.base) > PM_OM_DUMP_SIZE) {
        pmom_pr_err("dump exhaust\n");
        return NULL;
    }

    if (reg_cnt >= PM_OM_MOD_ID_MAX) {
        pmom_pr_err("num:(%d)>max:(%d)", reg_cnt, PM_OM_MOD_ID_MAX);
        return NULL;
    }

    spin_lock_irqsave(&g_pmom_ctrl.dump.lock, flags);

    entry[reg_cnt].magic = g_pm_om_magic_tbl[mod_id];                     /*lint !e661 !e662 */
    entry[reg_cnt].len = len;                                             /*lint !e661 !e662 */
    entry[reg_cnt].offset = (long)(uintptr_t)g_pmom_ctrl.dump.sub_region; /*lint !e661 !e662 */

    dump_area = (void *)g_pmom_ctrl.dump.sub_region;
    g_pmom_ctrl.dump.sub_region += len;
    g_pmom_ctrl.dump.region_cnt++;

    spin_unlock_irqrestore(&g_pmom_ctrl.dump.lock, flags);

    return dump_area;
}

static int pm_om_log_smem_init(DRV_PM_OM_CFG_STRU *nv_cfg)
{
    unsigned long socp_addr;
    unsigned int socp_size = 0;
    u32 mem_size;
    void *temp = NULL;
    int ret;

    g_pmom_ctrl.log.smem = (struct pm_om_smem_cfg *)(uintptr_t)PM_OM_LOG_AREA;
    mem_size = SHM_SIZE_PM_OM;
    if (nv_cfg->mem_ctrl == PM_OM_USE_NORMAL_DDR) {
        socp_addr = mdrv_mem_region_get("socp_bbpds_ddr", &socp_size);
        if (socp_addr == 0 || socp_size == 0) {
            pmom_pr_err("mdrv_mem_region_get socp_bbpds_ddr failed\n");
            return PM_OM_ERR;
        }
        /* 使用socp的1M往后的内存 */
        temp = ioremap_wc((MMU_VA_T)socp_addr, (unsigned long)socp_size);
        g_pmom_ctrl.log.smem = (struct pm_om_smem_cfg *)((uintptr_t)temp + PM_OM_SOCP_OFFSET);
        mem_size = (socp_size - PM_OM_SOCP_OFFSET);
    }
    /* 文件头初始化 */
    g_pmom_ctrl.log.smem->file_head.magic = PM_OM_MAGIC_PMLG;
    g_pmom_ctrl.log.smem->file_head.len = mem_size;
    g_pmom_ctrl.log.smem->file_head.version = 0;
    g_pmom_ctrl.log.smem->file_head.reserved = PM_OM_PROT_MAGIC1;

    g_pmom_ctrl.log.smem->sn = 0;
    ret = memcpy_s(&(g_pmom_ctrl.log.smem->nv_cfg), sizeof(*nv_cfg), nv_cfg, sizeof(*nv_cfg));
    if (ret != EOK) {
        pmom_pr_err("smem nv_cfg memcpy_s fail ret = %d\n", ret);
        return PM_OM_ERR;
    }
    /* log: ring buffer信息结构初始化 */
    g_pmom_ctrl.log.smem->mem_info.magic = PM_OM_PROT_MAGIC1;
    g_pmom_ctrl.log.smem->mem_info.read = 0;
    g_pmom_ctrl.log.smem->mem_info.write = 0;
    g_pmom_ctrl.log.smem->mem_info.size = mem_size - sizeof(struct pm_om_smem_cfg);
    g_pmom_ctrl.log.smem->mem_info.app_is_active = 0;

    g_pmom_ctrl.log.rb.buf = (char *)((uintptr_t)g_pmom_ctrl.log.smem + sizeof(struct pm_om_smem_cfg));
    g_pmom_ctrl.log.rb.size = g_pmom_ctrl.log.smem->mem_info.size;
    g_pmom_ctrl.log.rb.read = 0;
    g_pmom_ctrl.log.rb.write = 0;
    g_pmom_ctrl.log.threshold = (g_pmom_ctrl.log.smem->nv_cfg.log_threshold[PM_OM_CPUID] * g_pmom_ctrl.log.rb.size) /
                                100;
    return PM_OM_OK;
}

int bsp_pm_om_log_init(void)
{
    DRV_PM_OM_CFG_STRU nv_cfg = { 0 };
    int ret;

    ret = memset_s((void *)&nv_cfg, sizeof(nv_cfg), 0, sizeof(nv_cfg));
    if (ret != EOK) {
        pmom_pr_err("nv_cfg memset ret = %d\n", ret);
        return PM_OM_ERR;
    }
    if (bsp_nvm_read(NV_ID_DRV_PM_OM, (u8 *)&(nv_cfg), sizeof(nv_cfg)) != NV_OK) {
        pmom_pr_err("nv[0x%x] read fail\n", NV_ID_DRV_PM_OM);
        return PM_OM_ERR;
    }

    ret = memset_s((void *)&g_pmom_ctrl.log, sizeof(g_pmom_ctrl.log), 0, sizeof(g_pmom_ctrl.log));
    if (ret != EOK) {
        pmom_pr_err("g_pmom_ctrl.log memset ret = %d\n", ret);
        return PM_OM_ERR;
    }
    spin_lock_init(&g_pmom_ctrl.log.lock);
    ret = pm_om_log_smem_init(&nv_cfg);
    if (ret != PM_OM_OK) {
        return PM_OM_ERR;
    }

    /* 当前debug和platform主要和log强相关, dump比较独立 */
    g_pmom_ctrl.platform = NULL;
    g_pmom_ctrl.debug = NULL;
    (void)pm_om_platform_init();
    (void)pm_om_debug_init();

    return PM_OM_OK;
}

int bsp_pm_om_dump_init(void)
{
    unsigned long base_addr = 0;
    int ret;
    /* add pm info collect */
    ret = memset_s((void *)&g_pmom_ctrl.pm_info, sizeof(g_pmom_ctrl.pm_info), 0, sizeof(g_pmom_ctrl.pm_info));
    if (ret) {
        pmom_pr_err("g_pmom_ctrl.pm_info memset ret = %d\n", ret);
    }
    spin_lock_init(&g_pmom_ctrl.pm_info.lock);
    INIT_LIST_HEAD(&g_pmom_ctrl.pm_info.list);
    g_pmom_ctrl.pm_info.init_flag = 1;
    ret = memset_s((void *)&g_pmom_ctrl.dump, sizeof(g_pmom_ctrl.dump), 0, sizeof(g_pmom_ctrl.dump));
    if (ret) {
        pmom_pr_err("g_pmom_ctrl.dump memset ret = %d\n", ret);
    }
    spin_lock_init(&g_pmom_ctrl.dump.lock);

    g_pmom_ctrl.dump.base = (void *)bsp_dump_register_field(PM_OM_DUMP_ID, "PM_OM", PM_OM_DUMP_SIZE, 0);
    if (g_pmom_ctrl.dump.base == NULL) {
        goto err_ret; /*lint !e801 */
    }
    g_pmom_ctrl.dump.sub_region = g_pmom_ctrl.dump.base;
    g_pmom_ctrl.dump.cfg = (struct pm_om_dump_cfg *)g_pmom_ctrl.dump.base; /* lint !e826: (Info -- Suspicious
                                                                              pointer-to-pointer conversion (area too
                                                                              small)) */
    g_pmom_ctrl.dump.cfg->file_head.magic = (u32)PM_OM_MAGIC_PMDP;
    g_pmom_ctrl.dump.cfg->file_head.len = PM_OM_DUMP_SIZE;
    g_pmom_ctrl.dump.cfg->file_head.version = 1;
    g_pmom_ctrl.dump.cfg->file_head.reserved = PM_OM_PROT_MAGIC2;
    g_pmom_ctrl.dump.sub_region += sizeof(*(g_pmom_ctrl.dump.cfg));
    base_addr = (uintptr_t)g_pmom_ctrl.dump.sub_region;
    g_pmom_ctrl.dump.sub_region = (char *)(uintptr_t)OSL_ROUND_UP(base_addr, 4);

    ret = memset_s((void *)g_pmom_ctrl.dump.cfg->entry_tbl, sizeof(g_pmom_ctrl.dump.cfg->entry_tbl), 0,
                   sizeof(g_pmom_ctrl.dump.cfg->entry_tbl));
    if (ret) {
        pmom_pr_err("g_pmom_ctrl.dump.cfg->entry_tbl memset ret = %d\n", ret);
    }

    g_pmom_ctrl.dump.init_flag = PM_OM_INIT_MAGIC;

    return PM_OM_OK;

err_ret:
    g_pmom_ctrl.dump.cfg = NULL;
    pmom_pr_err("alloc mem fail\n");
    return PM_OM_ERR;
}

int bsp_pm_info_stat_register(pm_info_cbfun pcbfun, struct pm_info_usr_data *usr_data)
{
    struct pm_info_list *pm_info = NULL; /*lint !e830*/
    unsigned long flags;
    int ret;

    if (pcbfun == NULL || usr_data == NULL) {
        return PM_OM_ERR;
    }

    list_for_each_entry(pm_info, &(g_pmom_ctrl.pm_info.list), entry) {
        if (pcbfun == pm_info->cb_func && usr_data->mod_id == pm_info->usr_data.mod_id) {
            pmom_pr_err("(id:%d)has registered\n", usr_data->mod_id);
            return PM_OM_OK;
        }
    }

    pm_info = (struct pm_info_list *)osl_malloc((unsigned int)sizeof(struct pm_info_list));
    if (pm_info == NULL) {
        pmom_pr_err("malloc failed!\n");
        return PM_OM_ERR;
    }

    ret = memset_s(pm_info, sizeof(struct pm_info_list), 0, sizeof(struct pm_info_list));
    if (ret) {
        pmom_pr_err("pm_info memset ret = %d\n", ret);
        osl_free(pm_info);
        return PM_OM_ERR;
    }

    pm_info->cb_func = pcbfun;
    ret = memcpy_s((void *)&pm_info->usr_data, sizeof(pm_info->usr_data), usr_data, sizeof(struct pm_info_usr_data));
    if (ret) {
        pmom_pr_err("pm_info->usr_data memcpy ret = %d\n", ret);
        osl_free(pm_info);
        return PM_OM_ERR;
    }

    spin_lock_irqsave(&g_pmom_ctrl.pm_info.lock, flags);
    g_pm_om_magic_tbl[pm_info->usr_data.mod_id] = pm_info->usr_data.magic;
    list_add_tail(&pm_info->entry, &g_pmom_ctrl.pm_info.list);
    spin_unlock_irqrestore(&g_pmom_ctrl.pm_info.lock, flags);

    return PM_OM_OK; /*lint !e429*/
}

/*lint --e{528}*/
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_pm_om_log_init);         /*lint !e19 */
arch_initcall_sync(bsp_pm_om_dump_init); /*lint !e19 */
#endif
EXPORT_SYMBOL(bsp_pm_log_type);     /*lint !e19 */
EXPORT_SYMBOL(bsp_pm_log);          /*lint !e19 */
EXPORT_SYMBOL(pm_om_ctrl_get);      /*lint !e19 */
EXPORT_SYMBOL(bsp_pm_log_addr_get); /*lint !e19 */
EXPORT_SYMBOL(bsp_pm_dump_get);     /*lint !e19 */
EXPORT_SYMBOL(g_pmom_ctrl);         /*lint !e19 */
