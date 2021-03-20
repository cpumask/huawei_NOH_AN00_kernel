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
#include <asm/memory.h>
#include <bsp_shared_ddr.h>
#include <bsp_hardtimer.h>
#include <bsp_version.h>
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
#include <mdrv_memory_layout.h>
#include "mloader_comm.h"
#include "mloader_load_image.h"
#include "mloader_debug.h"
#include "mloader_msg.h"

#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#endif

#define THIS_MODU mod_mloader

DECLARE_COMPLETION(g_mloader_lr_complete);
DECLARE_COMPLETION(g_mloader_nr_complete);
DECLARE_COMPLETION(g_mloader_sysboot_start_complete);

mloader_info_s g_mloader_info;
mloader_img_s g_mloader_images;
mloader_addr_s g_mloader_addr;
struct semaphore g_mloader_load_img_sem;
// for static check
u32 g_mloader_task = 1;
long g_mloader_load_type;

mloader_addr_s *bsp_mloader_get_addr(void)
{
    return &g_mloader_addr;
}

mloader_info_s *bsp_mloader_get_info_st(void)
{
    return &g_mloader_info;
}

mloader_img_s *bsp_mloader_get_images_st(void)
{
    return &g_mloader_images;
}

static int mloader_load_get_flag(u32 flag)
{
    return g_mloader_images.load_flags & BIT(flag);
}

void mloader_load_set_flag(u32 flag)
{
    set_bit(flag, &g_mloader_images.load_flags);
}

static void mloader_load_clear_flag(u32 flag)
{
    clear_bit(flag, &g_mloader_images.load_flags);
}

void mloader_load_clear_outer_event(void)
{
    g_mloader_images.outer_event = MLOADER_LOAD_UNINITED;
    mloader_print_err("mloader clear out event.\n");
}

void mloader_trigger_load_task(u32 event)
{
    g_mloader_images.outer_event = event;
    mloader_print_err("mloader out event %d.\n", event);
    __pm_stay_awake(&g_mloader_images.wake_lock);
    osl_sem_up(&g_mloader_images.task_sem);
}

int mloader_load_notifier(mloader_load_notifier_s *load_notifier)
{
    if (load_notifier == NULL || load_notifier->func == NULL) {
        mloader_print_err("mloader_load_notifier_regitser input param error.\n");
        return -1;
    }
    list_add(&(load_notifier->list), &(g_mloader_images.load_notifier_list));
    return 0;
}

int bsp_mloader_load_notifier_register(mloader_load_notifier_s *load_notifier)
{
    /*params check*/
    if (load_notifier == NULL || load_notifier->func == NULL) {
        mloader_print_err("mloader_load_notifier_regitser input param error.\n");
        return -1;
    }
    return mloader_load_notifier(load_notifier);
}

void mloader_load_notifier_process(load_stage_e state)
{
    mloader_load_notifier_s *load_notifier = NULL;

    list_for_each_entry(load_notifier, &(g_mloader_images.load_notifier_list), list) {
        if (load_notifier->func) {
            (*(load_notifier->func))(state);
        }
    }
}

static void mloader_modules_init(void)
{
    int ret;
#if (FEATURE_ON == FEATURE_DELAY_MODEM_INIT)
    u64 jiffies_old = 0;
    u64 jiffies_new = 0;
    mloader_print_err("modem rfile init start\n");

    ret = modem_rfile_init();
    if (ret) {
        mloader_print_err("modem rfile init fail\n");
    }

    mloader_print_err("modem nv init start\n");
    ret = modem_nv_init();
    if (ret) {
        mloader_print_err("modem nv init fail\n");
    }

    mloader_print_err("VOS Module Init start\n");

    jiffies_old = get_jiffies_64();
    ret = VOS_ModuleInit();
    if (ret) {
        mloader_print_err("VOS Module Init fail\n");
    }
    jiffies_new = get_jiffies_64();
    mloader_print_err("VOS Module Init end tick 0x%llx\n", jiffies_new - jiffies_old);
#endif
    ret = modem_dir_init();
    if (ret) {
        mloader_print_err("modem_dir_init failed, ret %x\n", ret);
        return;
    }
    return;
}

int bsp_mloader_load_reset(void)
{
    int timeout;

    /* halt load task */
    mloader_load_clear_flag(MLOADER_LOAD_IN_HALT);
    mloader_load_set_flag(MLOADER_LOAD_REQ_HALT);
    mloader_print_err("mloader send halt flag.\n");

    mloader_trigger_load_task(MLOADER_LOAD_MODEM_REQ_RESET);

    timeout = 300;
    while (!mloader_load_get_flag(MLOADER_LOAD_IN_HALT)) {
        msleep(10);
        timeout--;
        if (timeout < 0) {
            mloader_print_err("wait for mloader halt timeout\n");
            return -1;
        }
    }
    mloader_print_err("mloader is in halt.\n");
    return 0;
}
int bsp_load_modem_images(void)
{
    /* clear halt state */
    mloader_load_clear_flag(MLOADER_LOAD_REQ_HALT);
    mloader_print_err("mloader clear halt flag.\n");

    /* start load */
    mloader_load_set_flag(MLOADER_LOAD_LR);
    mloader_trigger_load_task(MLOADER_LOAD_MODEM_IN_RESET);
    return 0;
}

int mloader_images_task(void *data)
{
    const bsp_version_info_s *version_info = bsp_get_version_info();

    g_mloader_images.op_state = MLOADER_LOAD_IDLE;

    /* 系统启动后第一次加载lr */
    wait_for_completion(&g_mloader_sysboot_start_complete);

    if (!bsp_need_loadmodem()) {
        mloader_print_err("upload or charge mode, will not start modem.\n");
        return 0;
    }
    __pm_stay_awake(&g_mloader_images.wake_lock);
    mloader_modules_init();

    if (version_info != NULL &&
        version_info->product_type == PRODUCT_PHONE &&
        version_info->plat_type != PLAT_ASIC) {
        wait_for_completion(&g_mloader_sysboot_start_complete);
        bsp_rfile_get_load_mode(g_mloader_load_type);
    }
    mloader_load_lr_img();

    while (g_mloader_task) {
        osl_sem_down(&g_mloader_images.task_sem);
        /* 如果有单独复位，需要把当前加载的加载完，并将任务halt住，halt住之后，执行单独复位加载任务 */
        if (mloader_load_get_flag(MLOADER_LOAD_REQ_HALT)) {
            mloader_load_set_flag(MLOADER_LOAD_IN_HALT);
            mloader_print_err("mloader receive halt flag.\n");
            goto done;
        }

        g_mloader_images.op_state = MLOADER_LOAD_DOING;
        mloader_update_start_time_debug_info();
        mloader_update_op_state(MLOADER_LOAD_DOING);

        if (mloader_load_get_flag(MLOADER_LOAD_LR)) {
            // load lr

            mloader_update_ccore_start_time_debug_info();
            mloader_load_lr_img();
            mloader_update_ccore_end_time_debug_info();
            mloader_load_clear_flag(MLOADER_LOAD_LR);
            mloader_load_clear_outer_event();
        } else {
            /* solve icc msg from ccore and load img */
            mloader_load_ccore_imgs();
        }
        g_mloader_images.op_state = MLOADER_LOAD_IDLE;
done:
        mloader_update_op_state(MLOADER_LOAD_IDLE);
        mloader_print_info("mloader load ok.\n");
    }
    return 0;
}

static ssize_t modem_start_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int status = count;
    long modem_load = 0;

    if ((kstrtol(buf, 10, &modem_load) < 0)) {
        return -EINVAL;
    }

    if (modem_load >= MLOADER_BUTT) {
        bsp_info("modem load ccpu get value error! modem_load %ld\n", modem_load);
        return -EINVAL;
    }

    g_mloader_load_type = modem_load;

    complete(&g_mloader_sysboot_start_complete);

    mloader_print_info("modem_sysboot_start write ok \n");

    return status;
}

static DEVICE_ATTR(modem_start, S_IWUSR, NULL, modem_start_store);

static DEVICE_ATTR(modem_image_patch_status, 0660, modem_imag_patch_status_show, modem_imag_patch_status_store);

static struct attribute *g_modem_start_attributes[] = { &dev_attr_modem_start.attr,
                                                      &dev_attr_modem_image_patch_status.attr, NULL };

static const struct attribute_group g_mloader_group = {
    .attrs = g_modem_start_attributes,
};

static int mloader_get_dts(void)
{
    u32 ret = 0;
    int id = 0;
    u32 ddr_addr;

    struct device_node *dev_node = NULL;
    struct device_node *child_node = NULL;
    struct device_node *dts_node = NULL;

    dev_node = of_find_compatible_node(NULL, NULL, "hisilicon,balong_mloader");
    if (dev_node != NULL) {
        g_mloader_info.image_num = of_get_child_count(dev_node);
        if (!g_mloader_info.image_num) {
            mloader_print_err("no ccpu image found in mloader dts\n");
            return MLOADER_PARAM_ERROR;
        }
        g_mloader_info.img = (ccpu_img_info_s *)kzalloc(sizeof(ccpu_img_info_s) * g_mloader_info.image_num, GFP_KERNEL);
        if (g_mloader_info.img == NULL) {
            mloader_print_err("kzalloc fail, no memory to init load ccpu, size: 0x%lX\n",
                              (unsigned long)sizeof(ccpu_img_info_s) * g_mloader_info.image_num);
            return MLOADER_NO_MEMORY;
        }

        ret = (u32)of_property_read_u32_index(dev_node, "sec_os_is_enable", 0, (u32 *)&g_mloader_info.sec_os_is_enable);
        if (ret) {
            mloader_print_err("get decomress_method fail form dts, ret = %d\n", (int)ret);
            return MLOADER_INTERFACE_ERROR;
        }

        for_each_child_of_node(dev_node, child_node)
        {
            if (of_device_is_available(child_node)) {
                g_mloader_info.img[id].image_available = IMAGE_AVAILABLE;
            }
            ret = (u32)of_property_read_string_index(child_node, "image_name", 0,
                                                     (const char **)&g_mloader_info.img[id].image_name);

            ret |= (u32)of_property_read_u32_index(child_node, "with_dts", 0, (u32 *)&g_mloader_info.img[id].with_dts);

            g_mloader_info.img[id].dev_node = child_node;
            if (g_mloader_info.sec_os_is_enable) {
                if (of_property_read_string_index(child_node, "ddr_name", 0,
                                                     (const char **)&g_mloader_info.img[id].ddr_name)) {
                    ret |= (u32)of_property_read_u32_index(child_node, "ddr_addr", 0, (u32 *)&ddr_addr);
                    g_mloader_info.img[id].ddr_addr = ddr_addr;
                    ret |= (u32)of_property_read_u32_index(child_node, "ddr_size", 0,
                                                       (u32 *)&g_mloader_info.img[id].ddr_size);
                } else {
                    g_mloader_info.img[id].ddr_addr = mdrv_mem_region_get(g_mloader_info.img[id].ddr_name, &(g_mloader_info.img[id].ddr_size));
                }
            }

            if (g_mloader_info.img[id].with_dts) {
                dts_node = of_get_child_by_name(child_node, "modem_dtb");
                if (dts_node != NULL) {
                    ret |= (u32)of_property_read_string_index(dts_node, "file_name", 0,
                                                              (const char **)&g_mloader_info.img[id].dtb.name);
                    if (of_property_read_string_index(child_node, "dtb_ddr_name", 0,
                                                     (const char **)&g_mloader_info.img[id].dtb.dtb_ddr_name)) {
                        ret |= (u32)of_property_read_u32_index(dts_node, "dtb_addr", 0,
                                                           (u32 *)&g_mloader_info.img[id].dtb.run_addr);

                        ret |= (u32)of_property_read_u32_index(dts_node, "dtb_size", 0,
                                                           (u32 *)&g_mloader_info.img[id].dtb.ddr_size);
                    } else {
                        g_mloader_info.img[id].dtb.run_addr = mdrv_mem_region_get(g_mloader_info.img[id].dtb.dtb_ddr_name, &(g_mloader_info.img[id].dtb.ddr_size));
                    }   
                    ret |= (u32)of_property_read_u32_index(dts_node, "core_id", 0,
                                                           (u32 *)&g_mloader_info.img[id].dtb.core_id);
                } else {
                    mloader_print_err("balong modem load get dtb fail, parent image is %s\n",
                                      g_mloader_info.img[id].image_name);
                }
            }
            id++;
        }
    }
    if (ret) {
        mloader_print_err("balong modem load get dts fail, ret =%d\n", (int)ret);
    }

    return (int)ret;
}

int mloader_images_init(void)
{
    int ret;
    struct sched_param sch_para;

    sch_para.sched_priority = 14;
    mloader_print_err("balong load images init\n");
    ret = memset_s(&g_mloader_images, sizeof(g_mloader_images), 0x00, sizeof(mloader_img_s));
    if (ret) {
        mloader_print_err("<%s> memset_s error, ret = %d\n", __FUNCTION__, ret);
    }
#if (((defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)) || \
     ((defined CONFIG_TRUSTZONE_HM) && (defined CONFIG_LOAD_SEC_IMAGE)))
    ret = mloader_teek_init();
    if (ret) {
        mloader_print_err("<%s> teek init error, ret = %d\n", __FUNCTION__, ret);
    }
#endif
    osl_sem_init(0, &(g_mloader_images.task_sem));
    osl_sem_init(1, &(g_mloader_images.msg_sem));
    osl_sem_init(1, &g_mloader_load_img_sem);

    INIT_LIST_HEAD(&(g_mloader_images.load_notifier_list));

    wakeup_source_init(&g_mloader_images.wake_lock, "mloader_wakelock");
    mloader_load_clear_flag(MLOADER_LOAD_LR);

    g_mloader_images.taskid = kthread_run(mloader_images_task, BSP_NULL, "mloader");
    if (IS_ERR(g_mloader_images.taskid)) {
        mloader_print_err("kthread_run fail\n");
        return PTR_ERR(g_mloader_images.taskid);
    }

    ret = sched_setscheduler(g_mloader_images.taskid, SCHED_FIFO, &sch_para);
    if (ret) {
        mloader_print_err("sched_setscheduler fail\n");
        return ret;
    }
    ret = mloader_msg_init();
    if (ret) {
        mloader_print_err("mloader msg init fail, ret = %d\n", ret);
        return ret;
    }

    mloader_print_err("mloader init success\n");

    return BSP_OK;
}

int mloader_probe(struct platform_device *pdev)
{
    int ret;

    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
    g_mloader_images.dev = pdev->dev;
    ret = mloader_get_dts();
    if (ret) {
        mloader_print_err("get mloader dts fail, ret = %d\n", ret);
        return ret;
    }

    ret = mloader_debug_init();
    if (ret != 0) {
        mloader_print_err("modem debug init fail\n");
    }

    ret = mloader_images_init();
    if (ret != 0) {
        mloader_print_err("modem load images init fail\n");
        return -ENXIO;
    }

    g_mloader_addr.mloader_secboot_virt_addr_for_ccpu = kzalloc(PAGE_ALIGN(MLOADER_SECBOOT_BUFLEN), GFP_KERNEL);
    if (g_mloader_addr.mloader_secboot_virt_addr_for_ccpu == NULL) {
        mloader_print_err("fail to malloc secboot buffer for ccpu\n");
        return -1;
    }
    g_mloader_addr.mloader_secboot_phy_addr_for_ccpu = dma_map_single(&(pdev->dev),
                                                                      g_mloader_addr.mloader_secboot_virt_addr_for_ccpu,
                                                                      MLOADER_SECBOOT_BUFLEN, DMA_TO_DEVICE);
    if (dma_mapping_error(&(pdev->dev), (dma_addr_t)(uintptr_t)g_mloader_addr.mloader_secboot_virt_addr_for_ccpu)) {
        mloader_print_err("dma mapping error\n");
        return -1;
    }
    ret = sysfs_create_group(&pdev->dev.kobj, &g_mloader_group);
    if (ret != 0) {
        mloader_print_err("create his modem sys node fail\n");
        return -ENXIO;
    }

    return ret;
}

static const struct of_device_id g_mloader_match[] = {
    {
        .compatible = "hisilicon,balong_mloader",
    },
    {},
};
MODULE_DEVICE_TABLE(of, g_mloader_match);

struct platform_driver g_mloader_driver = {
    .probe      = mloader_probe,
    .driver     = {
        .name     = "balong_mloader",
        .owner    = THIS_MODULE,
        .of_match_table = g_mloader_match,
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

int mloader_driver_init(void)
{
    int ret;

    ret = platform_driver_register(&g_mloader_driver);
    if (ret) {
        mloader_print_err("register mloader driver failed!\n");
    }
    return ret;
}
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(mloader_driver_init);
#endif
