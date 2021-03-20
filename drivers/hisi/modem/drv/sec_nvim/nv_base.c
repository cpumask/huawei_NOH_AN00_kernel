﻿/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <product_config.h>
#include <securec.h>
#include <osl_thread.h>
#include <bsp_nvim.h>
#include <bsp_version.h>
#include <bsp_blk.h>
#include <bsp_dump.h>
#include <bsp_reset.h>
#include <bsp_slice.h>
#include "nv_comm.h"
#include "nv_file.h"
#include "nv_ctrl.h"
#include "nv_debug.h"
#include "nv_index.h"
#include "nv_msg.h"
#include "nv_proc.h"
#include "nv_partition_img.h"
#include "nv_partition_bakup.h"
#include "nv_partition_factory.h"
#include "nv_partition_upgrade.h"
#include "nv_factory_check.h"


#define THIS_MODU mod_nv

u32 g_flush_img_cnt = 0;
u32 g_flush_bak_cnt = 0;

unsigned long g_nv_global_addr = 0;
u32 g_nv_global_size = 0;
unsigned long g_nv_unsec_addr = 0;
u32 g_nv_unsec_size = 0;

struct nv_global_ctrl_info_stru g_nv_ctrl = { { 0 }, };

struct nv_global_ctrl_info_stru* nv_get_ctrl_info(void)
{
    return &g_nv_ctrl;
}

bool nv_check_esl_emu(void)
{
    u16 chip_type;
    u8 plat_type;

    if (bsp_get_version_info() == NULL) {
        nv_printf("bsp_get_version_info == NULL\n");
        return false;
    }

    plat_type = bsp_get_version_info()->plat_type;
    chip_type = bsp_get_version_info()->chip_type;

    return (bool)(((plat_type == PLAT_ESL) || (plat_type == PLAT_EMU) || (plat_type == PLAT_HYBRID)) &&
                  (chip_type == CHIP_V5010));
}

/************************************************************************
 函 数 名  : bsp_nvm_get_modem_num
 功能描述  : 获取所有NV支持的modem个数
*************************************************************************/
u32 bsp_nvm_get_modem_num(void)
{
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;
    return ctrl_info->modem_num;
}

/************************************************************************
 函 数 名  : nv_readEx
 功能描述  : 读NV接口，作用范围:所有的NV
 输入参数  : modem_id: modem ID
             itemid: NV ID
             pdata: NV 数据源buff
             offset: NV在NV内存中相对NV基地址的偏移地址
             datalen: NV数据长度
*************************************************************************/
u32 nv_readEx(u32 modem_id, u32 itemid, u32 offset, u8 *pdata, u32 datalen)
{
    u32 ret;
    nv_rdwr_req rreq;

    nv_item_info_s *item_info = NULL;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;

    if ((pdata == NULL) || (datalen == 0)) {
        nv_record("parms is err! data_len=%d.\n", datalen);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    ret = nv_search_iteminfo(itemid, ctrl_info, &item_info);
    if (ret) {
        nv_record("can not find 0x%x !\n", itemid);
        return BSP_ERR_NV_NO_THIS_ID;
    }

    if (((offset + datalen) < offset) || ((offset + datalen) < datalen)) {
        nv_record("offset add datalen overflow, nvid=0x%x itemlen=0x%x rdlen=0x%x offset=0x%x!\n", itemid,
                  item_info->nv_len, datalen, offset);
        return BSP_ERR_NV_ITEM_LEN_ERR;
    }

    if ((offset + datalen) > item_info->nv_len) {
        nv_record("length overflow! all_len=%d, offset=%d, len=%d.\n", item_info->nv_len, offset, datalen);
        return BSP_ERR_NV_ITEM_LEN_ERR;
    }

    if ((modem_id == 0) || (modem_id > ctrl_info->modem_num)) {
        nv_record("modemid err!modemid=%d,modem=%d.\n", modem_id, ctrl_info->modem_num);
        return BSP_ERR_NV_INVALID_MDMID_ERR;
    }

    if (modem_id > item_info->modem_num) {
#if (FEATURE_OFF == FEATURE_MULTI_MODEM)
        nv_record("modemid err!mpdemid=%d.nvmodem=%d.\n", modem_id, item_info->modem_num);
        return BSP_ERR_NV_INVALID_MDMID_ERR;
#else
        modem_id = 1;
#endif
    }

    rreq.itemid = itemid;
    rreq.modemid = modem_id;
    rreq.offset = offset;
    rreq.pdata = pdata;
    rreq.size = datalen;

    (void)nv_read_from_mem(&rreq, item_info, ctrl_info);
    nv_debug_trace(pdata, datalen);

    return NV_OK;
}
/************************************************************************
 函 数 名  : nv_readEx
 功能描述  : 读NV接口，作用范围:所有的NV
 输入参数  : modem_id: modem ID
             itemid: NV ID
             pdata: NV 数据源buff
             offset: NV在NV内存中相对NV基地址的偏移地址
             datalen: NV数据长度
             path:分区路径
*************************************************************************/
u32 nv_readEx_fac_partition(u32 modem_id, u32 itemid, u32 offset, u8 *pdata, u32 datalen)
{
    u32 ret;

    if ((pdata == NULL) || (datalen == 0)) {
        nv_record("parms is err! data_len=%d.\n", datalen);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    if (((offset + datalen) < offset) || ((offset + datalen) < datalen)) {
        nv_record("offset add datalen overflow, nvid=0x%x rdlen=0x%x offset=0x%x!\n", itemid, datalen, offset);
        return BSP_ERR_NV_ITEM_LEN_ERR;
    }

    ret = nv_read_item_from_factory(modem_id, itemid, offset, pdata, datalen);
    nv_debug_trace(pdata, datalen);

    return ret;
}

u32 nv_readEx_factory(u32 modem_id, u32 itemid, u32 offset, u8 *pdata, u32 datalen)
{
    /* check init state */
    if (nv_read_right() == false) {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }
    return nv_readEx_fac_partition(modem_id, itemid, offset, pdata, datalen);
}

/************************************************************************
 函 数 名  : bsp_nvm_get_len
 功能描述  : 获取NV的数据长度
*************************************************************************/
u32 bsp_nvm_get_len(u32 itemid, u32 *len)
{
    u32 ret;
    nv_item_info_s *ref_info = NULL;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;

    if (len == NULL) {
        nv_printf("NULL!nvid=0x%x.\n", itemid);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    /* check init state */
    if (nv_read_right() == false) {
        nv_printf("init state err!");
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    ret = nv_search_iteminfo(itemid, ctrl_info, &ref_info);
    if (ret) {
        nv_printf("no find nv(0x%x)!\n", itemid);
        return ret;
    }

    *len = ref_info->nv_len;
    return NV_OK;
}

/************************************************************************
 函 数 名  : bsp_nvm_dcread
 功能描述  : 读取NV数据，偏移地址为0
 备注:如果传入的modemid超过了这个NV配置的modem个数，则会默认读取modem0的NV数据
*************************************************************************/
u32 bsp_nvm_dcread(u32 modem_id, u32 itemid, u8 *pdata, u32 datalen)
{
    /* check init state */
    if (nv_read_right() == false) {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_readEx(modem_id, itemid, 0, pdata, datalen);
}

/************************************************************************
 函 数 名  : bsp_nvm_dcread
 功能描述  : 读取NV数据，任意偏移地址
 备注:如果传入的modemid超过了这个NV配置的modem个数，则会默认读取modem0的NV数据
*************************************************************************/
u32 bsp_nvm_dcreadpart(u32 modem_id, u32 itemid, u32 offset, u8 *pdata, u32 datalen)
{
    /* check init state */
    if (nv_read_right() == false) {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_readEx(modem_id, itemid, offset, pdata, datalen);
}

void *bsp_nvm_get_addr(u32 modemid, u32 itemid)
{
    u32 ret;
    nv_ctrl_info_s *ctrl_info = (nv_ctrl_info_s *)NV_GLOBAL_CTRL_INFO_ADDR;
    nv_item_info_s *item_info = NULL;

    if ((modemid == 0) || (modemid > ctrl_info->modem_num)) {
        nv_printf("invalid modemid!\n");
        return NULL;
    }

    ret = nv_search_iteminfo(itemid, ctrl_info, &item_info);
    if (ret) {
        nv_printf("not find nv id(0x%x)!\n", itemid);
        return NULL;
    }

    if (modemid > item_info->modem_num) {
        modemid = 1;
    }

    return (nv_get_item_mdmbase(modemid, item_info, ctrl_info));
}

/* Function: bsp_nvm_update_default
 * detail  : flush nv to image, backup and factory partition
 */
u32 bsp_nvm_update_default(void)
{
    u32 ret;

    nv_printf("^INFORBU: factory bakup start!\n");

    ret = nv_fac_flush();
    if (ret) {
        nv_record("^INFORBU: flush factory nv faild! 0x%x\n", ret);
        return ret;
    }

    ret = nv_bak_flush();
    if (ret) {
        nv_record("^INFORBU: flush backup nv failed! 0x%x\n", ret);
        return ret;
    }

    nv_printf("^INFORBU: factory bakup end!\n");

    return NV_OK;
}
/* Function: nv_update_backup
 * detail  : flush nv to image and backup partition
 */
u32 nv_update_backup(void)
{
    u32 ret;

    nv_printf("^NVBACKUP: bakup start!\n");
    ret = nv_img_flush();
    if (ret) {
        nv_record("^NVBACKUP: flush image nv failed! ret :0x%x\n", ret);
        return ret;
    }
    ret = nv_bak_flush();
    if (ret) {
        nv_record("^NVBACKUP: flush backup nv failed! 0x%x\n", ret);
        return ret;
    }
    nv_printf("^NVBACKUP: bakup end!\n");
    nv_record("updt bak stamp:0x%x\n", bsp_get_slice_value());
    return NV_OK;
}
/* Function: nv_factory_reset
 * detail  : MBB reset nv to factory
 */
u32 nv_factory_reset(void)
{
    u32 ret;

    nv_printf("nv factory reset start\n");
    ret = nv_bak_flush();
    if (ret) {
        nv_record("nv factory reset flush backup failed, ret:0x%x\n", ret);
        return ret;
    }

    ret = nv_upgrade_set_flag((bool)true);
    if (ret) {
        nv_record("nv factory reset set upgrade flag failed, ret:0x%x\n", ret);
        return ret;
    }

    nv_printf("nv factory reset end\n");

    return NV_OK;
}
/* Function: nv_upgrade_flush
 * detail  : write data to flash and clear upgrade flag
 */
u32 nv_upgrade_flush(void)
{
    u32 ret;

    nv_printf("nv upgrade flush start\n");

    ret = nv_data_writeback();
    if (ret) {
        nv_record("nv upgrade write back failed, ret:0x%x\n", ret);
        return ret;
    }

    ret = nv_clear_upgrade_flag();
    if (ret) {
        nv_record("nv upgrade clear upgrade flag failed, ret:0x%x\n", ret);
        return ret;
    }

    nv_printf("nv upgrade flush end\n");
    nv_record("upgrade flush stamp:0x%x\n", bsp_get_slice_value());
    return NV_OK;
}

u32 nv_flush_rwfile(void)
{
    u32 ret;
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;

    ret = nv_img_rwbuf();
    if (ret) {
        nv_printf("flush nvm img fail!");
        return ret;
    }

    if (ddr_info->nminfo.is_need_flush_backup & RWNV_FILE_BIT) {
        ret = nv_bak_rwbuf();
        if (ret) {
            nv_printf("flush modemnvm_back fail!");
            return ret;
        }
    }

    return NV_OK;
}

/************************************************************************
 函 数 名  : nv_write2file_handle
 功能描述  : 刷新nv镜像到工作分区
*************************************************************************/
u32 nv_write2file_handle(void)
{
    u32 ret;

    if (nv_img_is_need_flush()) {
        ret = nv_img_flush();
        if (ret) {
            nv_printf("flush nvm img fail!");
            return ret;
        }
        g_flush_img_cnt++;
    }

    if (nv_bak_is_need_flush()) {
        ret = nv_bak_flush();
        if (ret) {
            nv_printf("flush modemnvm_back fail!");
            return ret;
        }
        g_flush_bak_cnt++;
    }

    return NV_OK;
}

/************************************************************************
 函 数 名  : bsp_nvm_icc_task
 功能描述  : 处理C核发送的NV消息任务
*************************************************************************/
int bsp_nvm_icc_task(void *parm)
{
    u32 ret = NV_ERROR;
    nv_cmd_req *msg = NULL;
    void *temp = NULL;
    const struct nv_path_info_stru *nv_path = nv_get_path_info();
    if (nv_path == NULL) {
        nv_printf("nv_path NULL\n");
        return NV_ERROR;
    }
    /* make warncheck happy */
    parm = temp;
    temp = parm;

    while (1) {
        osl_sem_down(&g_nv_ctrl.task_sem);

        msg = nv_get_cmd_req();
        if (msg == NULL) {
            g_nv_ctrl.opState = NV_IDLE_STATE;
            continue;
        }

        switch (msg->msg_type) {
            case NV_TASK_MSG_FLUSH:
                /* flush nv data to img and backup */
                ret = nv_write2file_handle();
                break;

            case NV_TASK_MSG_FLUSH_RWFILE:
                /* flush rw nv data to img */
                ret = nv_flush_rwfile();
                break;

            case NV_TASK_MSG_LOAD_BACKUP:
                /* now only load rw data from backup to memory */
                nv_img_remove_rw();
                ret = nv_bak_reload(NVM_S);
                break;

            case NV_TASK_MSG_LOAD_CARRIER:
                ret = nv_img_load_carrier((s8 *)nv_path->file_path[NV_MBN]);
                break;

            case NV_TASK_MSG_LOAD_CARRIER_COMM:
                ret = nv_img_load_carrier((s8 *)nv_path->file_path[NV_MBN_COMM]);
                break;
            case NV_TASK_MSG_UPDATE_DEFAULT:
                /* at^inforbu operation */
                ret = bsp_nvm_update_default();
                break;

            case NV_TASK_MSG_UPDATE_BACKUP:
                /* at^nvbackup operation, flush nv data to img and backup */
                ret = nv_update_backup();
                break;

            case NV_TASK_MSG_FACTORY_RESET:
                /* nv recovery */
                ret = nv_factory_reset();
                break;

            case NV_TASK_MSG_UPGRADE_FLUSH:
                /* finish upgrade operation: write back and clear upgrade flag */
                ret = nv_upgrade_flush();
                break;

            default:
                nv_printf("msg type invalid, msg type;0x%x\n", msg->msg_type);
                break;
        }

        nv_debug_printf("deal msg ok\n");
        if (ret) {
            nv_record("execute nv icc cmd fail, msg type:0x%x errno:0x%x\n", msg->msg_type, ret);
        }

        if (msg->nv_msg_callback != NULL) {
            msg->nv_msg_callback(ret, msg->sn);
        }

        nv_put_cmd_req(msg);
        g_nv_ctrl.task_proc_count++;
        g_nv_ctrl.opState = NV_IDLE_STATE;
    }

    return NV_OK;
}

/************************************************************************
 函 数 名  : bsp_nvm_upgrade
 功能描述  : NV升级启动过程，kernel阶段NV启动流程
*************************************************************************/
u32 bsp_nvm_upgrade(void)
{
#ifndef BSP_CONFIG_PHONE_TYPE
    u32 ret;
#endif
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;

    nv_debug(NV_FUN_UPGRADE_PROC, 0, 0, 0, 0);
    nv_record("Balong nv upgrade start!\n");

    /* 判断fastboot阶段升级包文件解析异常，若出现异常，则需要重新解析升级包文件 */
    if (ddr_info->nvdload_boot_state != NV_BOOT_UPGRADE_SUCC_STATE) {
        nv_record("fastboot upgrade fail!\n");
        goto upgrade_fail_out;
    }
#ifndef BSP_CONFIG_PHONE_TYPE
    if (ddr_info->mem_file_type == NV_MEM_DEFAULT) {
        nv_printf("factory recover,need reload nv_rdwr.bin!\n");
        ret = nv_img_reload();
        if (ret) {
            nv_record("reload fail!ret=0x%x.\n", ret);
        }
    }
#endif

    return NV_OK;

upgrade_fail_out:
    nv_record("\n<%s>\n", __func__);
    return NV_ERROR;
}

/************************************************************************
 函 数 名  : bsp_nvm_reload
 功能描述  : NV正常启动，kernel阶段NV启动流程
*************************************************************************/
u32 bsp_nvm_reload(void)
{
    u32 ret;

    ret = nv_img_reload();
    if (ret) {
        ret = nv_bak_reload(NVM);
        if (ret) {
            return NV_ERROR;
        }
    }

    return NV_OK;
}

/************************************************************************
 函 数 名  : bsp_nvm_reload
 功能描述  : modem 单独复位调用，NV数据需要重新从工作分区加载
*************************************************************************/
u32 bsp_nvm_mreset_load(void)
{
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;
    u32 mode = ddr_info->mem_file_type;
    if ((bsp_reset_ccore_is_reboot() != 0) && ((mode == NV_MEM_BACKUP) || (mode == NV_MEM_UPGRADE_SUCCESS))) {
        nv_printf("modem reset, need reload nv file from modemnvm_img!\n");
        return nv_img_mreset_load();
    } else {
        nv_printf("no need reload nv file form modemnvm_img!\n");
        return NV_OK;
    }
}

/*****************************************************************************
 函 数 名  : bsp_nvm_write_buf_init
 功能描述  : 初始化写入NV时使用的buf和信号量
*****************************************************************************/
u32 bsp_nvm_buf_init(void)
{
    /* create sem */
    osl_sem_init(1, &g_nv_ctrl.nv_list_sem);
    INIT_LIST_HEAD(&g_nv_ctrl.nv_list);

    return NV_OK;
}

/*****************************************************************************
 函 数 名  : bsp_nvm_core_init
 功能描述  : NV升级或者加载
*****************************************************************************/
u32 bsp_nvm_core_init(u32 mode)
{
    u32 ret;
    if ((mode == NV_MEM_DLOAD) || (mode == NV_MEM_DEFAULT) || (mode == NV_MEM_COLDPATCH)) {
        ret = bsp_nvm_upgrade();
        if (ret) {
            nv_record("upgrade faided! 0x%x\n", ret);
            return ret;
        } else {
            nv_record("upgrade success!\n");
        }

        /* 读取NV自管理配置 */
        ret = bsp_nvm_read(NV_ID_DRV_SELF_CTRL, (u8 *)(&(g_nv_ctrl.nv_self_ctrl)), (u32)sizeof(NV_SELF_CTRL_STRU));
        if (ret) {
            g_nv_ctrl.nv_self_ctrl.ulResumeMode = NV_MODE_USER;
            nv_printf("read 0x%x fail,use default value! ret :0x%x\n", NV_ID_DRV_SELF_CTRL, ret);
        }
    } else {
        /* 读取NV自管理配置 */
        ret = bsp_nvm_read(NV_ID_DRV_SELF_CTRL, (u8 *)(&(g_nv_ctrl.nv_self_ctrl)), (u32)sizeof(NV_SELF_CTRL_STRU));
        if (ret) {
            g_nv_ctrl.nv_self_ctrl.ulResumeMode = NV_MODE_USER;
            nv_printf("read 0x%x fail,use default value! ret :0x%x\n", NV_ID_DRV_SELF_CTRL, ret);
        }

        /* 重新加载最新数据 */
        ret = bsp_nvm_reload();
        if (ret) {
            nv_record("reload fail!ret=0x%x.\n", ret);
            return ret;
        }

        nv_printf("reload success!\n");
    }

    return NV_OK;
}


u32 nv_mem_init(void)
{

    return NV_OK;
}

/*****************************************************************************
 函 数 名  : bsp_nvm_core_init1
 功能描述  : NV模块一些资源初始化
*****************************************************************************/
u32 bsp_nvm_core_init1(void)
{
    u32 ret;

    ret = bsp_nvm_buf_init();
    if (ret) {
        nv_record("init nvm buff fail!ret=0x%x.\n", ret);
        return ret;
    }

    INIT_LIST_HEAD(&g_nv_ctrl.stList);
    ret = (u32)osl_task_init("drv_nv", NV_ICC_TASK_PRIORITY, NV_ICC_TASK_STACK_SIZE, (OSL_TASK_FUNC)bsp_nvm_icc_task,
                             NULL, (OSL_TASK_ID *)&g_nv_ctrl.task_id);
    if (ret) {
        nv_record("nv task init err!ret=0x%x.\n", ret);
        return ret;
    }

    ret = nv_msg_init();
    if (ret) {
        nv_record("init task msg fail!ret=0x%x.\n", ret);
        return ret;
    }

    ret = nv_cpmsg_chan_init();
    if (ret) {
        nv_record("init icc chan fail!ret=0x%x.\n", ret);
        return ret;
    }

    return NV_OK;
}

/*****************************************************************************
 函 数 名  : bsp_nvm_kernel_init
 功能描述  : NV模块初始化启动
*****************************************************************************/
s32 bsp_nvm_kernel_init(void)
{
    u32 ret;
    nv_global_info_s *ddr_info = NULL;
    const struct nv_path_info_stru *nv_path = NULL;
    nv_debug(NV_FUN_KERNEL_INIT, 0, 0, 0, 0);

    nv_record("Balong nv init  start!\n");
    ret = nv_mem_init();
    if (ret != NV_OK) {
        nv_record("nv mem init failed\n");
        return ret;
    }

    ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;
    /* sem & lock init */
    spin_lock_init(&g_nv_ctrl.spinlock);
    osl_sem_init(0, &g_nv_ctrl.task_sem);
    osl_sem_init(0, &g_nv_ctrl.suspend_sem);
    osl_sem_init(1, &g_nv_ctrl.rw_sem);
    wakeup_source_init(&g_nv_ctrl.wake_lock, "nv_wakelock");
    g_nv_ctrl.shared_addr = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;

    (void)nv_debug_init();

    if (nv_check_esl_emu()) {
        nv_printf("this is esl/emu config\n");
        goto esl_nv_init;
    }

    /* nv root dir init */
    ret = (u32)nv_img_file_init();
    if (ret) {
        nv_record("no nv root dir!.\n");
        goto out;
    }
    nv_path = nv_get_path_info();
    if (nv_path == NULL) {
        goto out;
    }
    /* check nv file */
    ret = (u32)nv_img_boot_check(nv_path->root_dir);
    if (ret) {
        nv_record("check white list fail!ret=0x%x.\n", ret);
        goto out;
    }

    /* file info init */
    ret = nv_file_init();
    if (ret) {
        nv_record("init file info fail!ret=0x%x.\n", ret);
        goto out;
    }

    if (ddr_info->acore_init_state != NV_BOOT_INIT_OK) {
        nv_record("fast boot nv init fail!\n");
        goto out;
    }

    nv_flush_cache((void *)NV_GLOBAL_INFO_ADDR, (u32)NV_GLOBAL_INFO_SIZE);

    ret = bsp_nvm_core_init(ddr_info->mem_file_type);
    if (ret) {
        goto out;
    }

esl_nv_init:
    ret = bsp_nvm_core_init1();
    if (ret) {
        goto out;
    }

    /* 置初始化状态为OK */
    ddr_info->acore_init_state = NV_INIT_OK;
    nv_flush_cache((void *)NV_GLOBAL_INFO_ADDR, (u32)NV_GLOBAL_INFO_SIZE);

    nv_record("fastboot nv status:0x%x, 0x%x, 0x%x, 0x%x!\n", ddr_info->fb_debug.line, ddr_info->fb_debug.ret,
              ddr_info->fb_debug.reseverd1, ddr_info->fb_debug.reseverd2);
    nv_record("Balong nv init ok!\n");
    return NV_OK;

out:
    ddr_info->acore_init_state = NV_INIT_FAIL;
    nv_show_fastboot_err();
    nv_show_ddr_info();
    return -1;
}

static void bsp_nvm_exit(void)
{
    return;
}

void modem_nv_delay(void)
{
    u32 blk_size = 0;
    char *blk_label = NULL;
    int i, ret;


    /* 最长等待时长10s */
    for (i = 0; i < 10; i++) {
        nv_printf("modem nv wait for nv block device %d s\n", i);

        blk_label = (char *)NV_BACK_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(NV_DELAY_TIME);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }
        nv_printf("%s block size is 0x%x.\n", blk_label, blk_size);
        blk_label = (char *)NV_DLOAD_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(NV_DELAY_TIME);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        blk_label = (char *)NV_DLOAD_CUST_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(NV_DELAY_TIME);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        blk_label = (char *)NV_DEF_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(NV_DELAY_TIME);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        return;
    }
}

/*lint -save -e715*/ /* 715表示入参dev未使用 */
static int modem_nv_probe(struct platform_device *dev)
{
    int ret, flag;

    g_nv_ctrl.pmState = NV_WAKEUP_STATE;
    g_nv_ctrl.opState = NV_IDLE_STATE;

    modem_nv_delay();

    if (mdrv_file_access(MODEM_LOG_ROOT"/drv/nv", 0))
        (void)mdrv_file_mkdir(MODEM_LOG_ROOT"/drv/nv");

    ret = bsp_nvm_kernel_init();

    flag = modemNv_ProcInit();
    if ((ret < 0) || (flag < 0)) {
        ret = -1;
    }

    return ret;
}

#define NV_SHUTDOWN_STATE NV_BOOT_INIT_OK
static void modem_nv_shutdown(struct platform_device *dev)
{
    nv_global_info_s *ddr_info = (nv_global_info_s *)NV_GLOBAL_INFO_ADDR;

    /* read only */
    ddr_info->acore_init_state = NV_SHUTDOWN_STATE;
    ddr_info->ccore_init_state = NV_SHUTDOWN_STATE;
    ddr_info->mcore_init_state = NV_SHUTDOWN_STATE;
}
/*lint -restore*/
/*lint -save -e785*/ /* 785表示对结构体初始化的不完全modem_nv_pm_ops和modem_nv_drv modem_nv_device */
#ifdef CONFIG_PM
/*lint -save -e715*/ /* 715表示入参dev未使用 */
static s32 modem_nv_suspend(struct device *dev)
{
    nv_printf("mod_nv suspend\n");
    return 0;
}

static s32 modem_nv_resume(struct device *dev)
{
    nv_printf("mod_nv resume\n");
    return 0;
}
/*lint -restore*/
static const struct dev_pm_ops modem_nv_pm_ops = {
    .suspend = modem_nv_suspend,
    .resume = modem_nv_resume,
};

#define MODEM_NV_PM_OPS (&modem_nv_pm_ops)
#else
#define MODEM_NV_PM_OPS NULL
#endif

static struct platform_driver modem_nv_drv = {
    .shutdown = modem_nv_shutdown,
    .driver = {
        .name = "modem_nv",
        .owner = (struct module *)(unsigned long)THIS_MODULE,
        .pm = MODEM_NV_PM_OPS,
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

static struct platform_device modem_nv_device = {
    .name = "modem_nv",
    .id = 0,
    .dev = {
        .init_name = "modem_nv",
    },
};
/*lint -restore*/
int modem_nv_init(void)
{
    struct platform_device *dev = NULL;
    int ret;
    if (g_nv_ctrl.initStatus == 0) {
        g_nv_ctrl.initStatus = 1;
    } else {
        show_stack(current, NULL);
    }

    ret = modem_nv_probe(dev);

    return ret;
}

/* 仅用于初始化nv设备 */
int nv_init_dev(void)
{
    u32 ret;
    ret = (u32)platform_device_register(&modem_nv_device);
    if (ret) {
        nv_printf("platform_device_register modem_nv_device fail !\n");
        return -1;
    }

    ret = (u32)platform_driver_register(&modem_nv_drv);
    if (ret) {
        nv_printf("platform_device_register modem_nv_drv fail !\n");
        platform_device_unregister(&modem_nv_device);
        return -1;
    }
    nv_printf("init modem nv dev ok\n");
    return NV_OK;
}
void modem_nv_exit(void)
{
    bsp_nvm_exit();
    platform_device_unregister(&modem_nv_device);
    platform_driver_unregister(&modem_nv_drv);
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
device_initcall(nv_init_dev);
#endif
#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
module_init(modem_nv_init);
module_exit(modem_nv_exit);
#endif

EXPORT_SYMBOL(bsp_nvm_dcread);
EXPORT_SYMBOL(bsp_nvm_kernel_init);
EXPORT_SYMBOL(bsp_nvm_dcreadpart);
EXPORT_SYMBOL(bsp_nvm_get_len);
EXPORT_SYMBOL(bsp_nvm_reload);
EXPORT_SYMBOL(bsp_nvm_update_default);
EXPORT_SYMBOL(bsp_nvm_get_modem_num);
EXPORT_SYMBOL(g_nv_global_addr);
EXPORT_SYMBOL(g_nv_unsec_addr);

