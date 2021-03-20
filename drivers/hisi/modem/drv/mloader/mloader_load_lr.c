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
/*
 * File Name       : mloader_load_image.c
 * Description     : load modem image(ccore image),run in ccore
 * History         :
 */
#include <product_config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <asm/dma-mapping.h>
#include <asm/cacheflush.h>
#include <bsp_shared_ddr.h>
#include <bsp_hardtimer.h>
#include <bsp_version.h>
#include <bsp_nvim.h>
#include <bsp_slice.h>
#include <bsp_ipc.h>
#include <bsp_ddr.h>
#include <bsp_rfile.h>
#include <of.h>
#include <mdrv_sysboot.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
#include <uapi/linux/sched/types.h>
#endif

#include <securec.h>
#include <hi_mloader.h>
#include "mloader_comm.h"
#include "mloader_load_image.h"
#include "mloader_debug.h"

#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#endif

#define THIS_MODU mod_mloader

static DEFINE_MUTEX(load_proc_lock);

/*
 * 函 数 名: bsp_load_notify_ccpu_start
 * 功能描述: 通知lpm3 C核启动
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功返回OK,失败返回ERROR
 * 调用函数:
 * 被调函数:
 */

#ifndef CONFIG_LOAD_SEC_IMAGE
static inline int mloader_notify_mcu_load_ccpu(void)
{
    return bsp_ipc_int_send(IPC_CORE_MCORE, IPC_MCU_INT_SRC_CCPU_START);
}
#else
static inline int mloader_notify_mcu_load_ccpu(void)
{
    mloader_print_err("mloader_notify_mcu_load_ccpu stub!\n");
    return 0;
}
#endif

/*
 * 函 数 名: ccpu相关镜像加载接口
 * 功能描述: Modem相关镜像加载接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功返回OK,失败返回ERROR
 */
int mloader_load_ccpu(void)
{
    int ret;
    ccpu_img_info_s *image = NULL;
    const char *image_name = NULL;
    char file_name[MLOADER_FILE_NAME_LEN] = {0};

    mutex_lock(&load_proc_lock);

    image_name = MLOADER_LR_CCPU_IMG;

#if (((defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)) || \
     ((defined CONFIG_TRUSTZONE_HM) && (defined CONFIG_LOAD_SEC_IMAGE)))
    ret = ccpu_reset(MODEM);
    if (ret) {
        mloader_print_err("ccpu_reset failed, ret %#x\n", ret);
        goto error;
    }
    mloader_update_ccore_state_debug_info(SETED);
#endif

    ret = bsp_nvm_mreset_load();
    if (ret) {
        goto error;
    }

    image = mloader_get_image_info(image_name);
    if (image == NULL) {
        mloader_print_err("can't find image, %s\n", image_name);
        ret = MLOADER_INTERFACE_ERROR;
        goto error;
    }

    if (image->with_dts) {
        ret = mloader_load_and_verify_dtb_data(image);
        if (ret) {
            mloader_print_err("get dtb file fail, ret %d\n", ret);
            goto error;
        }
    }

    ret = mloader_get_file_name(file_name, image_name, 0);
    if (ret) {
        mloader_print_err("can't find image1, %s\n", image_name);
        goto error;
    }

#if (((defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)) || \
     ((defined CONFIG_TRUSTZONE_HM) && (defined CONFIG_LOAD_SEC_IMAGE)))

    // if have patch, need load and verify patch first, 不用增加宏控，如果不需要加载，patch直接打桩
    ret = mloader_load_patch(LR_PATCH, NULL, 0);
    if (ret) {
        mloader_print_err("load patch MODEM_COLD_PATCH fail, ret = 0x%x\n", ret);
        goto error;
    }

    ret = load_image(file_name, MODEM, image->ddr_addr, image->ddr_size, 0);
    if (ret) {
        mloader_print_err("load image MODEM fail, ret = 0x%x\n", ret);
        goto error;
    }
    mloader_update_ccore_state_debug_info(LOADED);

    ret = mloader_verify_soc_image(MODEM, 0);
    if (ret) {
        mloader_record_cold_patch_splicing_ret_val(LR_PATCH, ret);
        mloader_print_err("verify image MODEM fail, ret = 0x%x\n", ret);
        goto error;
    }
    mloader_update_ccore_state_debug_info(VERIFIED);

    mloader_update_modem_cold_patch_status(LR_PATCH);
#else
    ret = bsp_mloader_load_image_unsec(file_name, image->ddr_addr, NULL, image->ddr_size, 0);
    if (ret) {
        mloader_print_err("load image %s fail, ret = %d\n", image_name, ret);
        goto error;
    }
    mloader_update_ccore_state_debug_info(LOADED);

#endif
    mloader_print_err("load image %s success\n", image_name);
error:
    mutex_unlock(&load_proc_lock);
    return ret;
}

int mloader_ccpu_init(void)
{
    int ret;

    if (bsp_fs_tell_load_mode() == BSP_FS_OK) {
        ret = mloader_load_ccpu();
        if (ret) {
            mloader_print_err("%s his_load_image_start_up fail ,ret = %d", __func__, ret);
            return ret;
        }
    } else {
        mloader_print_err("fs not ok, no need to load ccpu from flash!\n");
    }

    ret = mloader_notify_mcu_load_ccpu();
    if (ret) {
        mloader_print_err("send ipc notify mcu ccpu started fail, ret=0x%x\n", ret);
    } else {
        mloader_print_err("send ipc notify mcu ccpu started!\n");
    }
    return ret;
}

void mloader_load_lr_img(void)
{
    int ret = 0;
    mloader_img_s *mloader_images = NULL;

    mloader_images = bsp_mloader_get_images_st();

    mloader_print_err("start loading ccpu\n");
    mloader_images->load_lr_flag = 0;
    ret = mloader_ccpu_init();
    if (!ret) {
        mloader_print_err("mloader load ccpu ok!\n");
    }
    __pm_relax(&(mloader_images->wake_lock));
    return;
}
