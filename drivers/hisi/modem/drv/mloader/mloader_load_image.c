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
 * File Name       : mloader_load_image_unsec.c
 * Description     : load modem image(ccore image),run in ccore
 * History         :
 */
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/zlib.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <bsp_ddr.h>
#include <bsp_sysctrl.h>
#include <bsp_sec.h>
#include <bsp_efuse.h>
#include <bsp_mloader.h>
#include "mloader_load_image.h"
#include <param_cfg_to_sec.h>
#include "mloader_comm.h"
#include <hi_common.h>
#include <hi_mloader.h>
#include <securec.h>

#define THIS_MODU mod_mloader
extern struct semaphore g_mloader_load_img_sem;

int bsp_mloader_load_image_unsec(const char *file_name, unsigned long image_addr, u32 *image_size, u32 ddr_size,
                                 u32 image_offset)
{
    int ret = 0;
    int file_size;
    int readed_bytes;

    unsigned long paddr;
    void *vaddr = NULL;
    void *vaddr_load = NULL;
    unsigned int offset = 0;

    static u8 vrl_buffer[4096];

    file_size = mloader_get_file_size(file_name);
    mloader_print_info("file_size 0x%x\n", file_size);
    if ((file_size <= 0) || ((u32)file_size > ddr_size)) {
        mloader_print_err("error %s size < 0 or image larger than ddr size:  file_size 0x%x > ddr_size %#x !\n",
                          file_name, file_size, ddr_size);
        return file_size;
    }

    if (image_size != NULL) {
        *image_size = file_size;
    }

    paddr = image_addr;
    vaddr = ioremap_wc(paddr, ddr_size);
    if (vaddr == NULL) {
        mloader_print_err("ioremap_wc error\n");
        return -ENOMEM;
    }

    readed_bytes = mloader_read_file(file_name, 0, sizeof(vrl_buffer), (char *)&vrl_buffer[0]);
    if (readed_bytes != sizeof(vrl_buffer)) {
        mloader_print_err("get image head fail, ret = %d\n", readed_bytes);
        return readed_bytes;
    }
#ifdef CONFIG_COMPRESS_CCORE_IMAGE
    vaddr_load = vaddr + ddr_size - file_size;
#else
    vaddr_load = vaddr;
#endif

    offset = SECBOOT_VRL_TABLE_SIZE;
    file_size = file_size - SECBOOT_VRL_TABLE_SIZE;
    readed_bytes = mloader_read_file(file_name, offset, (u32)file_size, (s8 *)vaddr_load);
    if (readed_bytes != file_size) {
        ret = -EIO;
        mloader_print_err("read_file %s err: readed_bytes 0x%x\n", file_name, file_size);
        goto error_unmap;
    }

#ifdef CONFIG_COMPRESS_CCORE_IMAGE
    mloader_print_err("image %s is start decompress\n", file_name);
    ret = mloader_decompress(ddr_size, vaddr_load, vaddr, (u32)file_size);
    if (ret) {
        mloader_print_err("image %s is decompress fail\n", file_name);
        goto error_unmap;
    }

    mloader_print_err("image %s is decompress success\n", file_name);
#endif

    mloader_print_err("load image %s success\n", file_name);

error_unmap:
    iounmap(vaddr);

    return ret;
}

/*
 * Function:       load_data_to_secos
 * Description:    从指定偏移开始传送指定大小的镜像
 * Input:
 *             part_name   - 要发送镜像的名称
 *             offset    - 偏移地址
 *             sizeToRead  - 输入参数，要写入的镜像的bytes大小
 * Output:         none
 * Return:         SEC_OK: OK  SEC_ERROR: ERROR码
 */

#if (((defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)) || \
    ((defined CONFIG_TRUSTZONE_HM) && (defined CONFIG_LOAD_SEC_IMAGE)))

int load_data_to_secos(const char *file_name, u32 offset, u32 size, enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr,
                       u32 ddr_size, u32 index)
{
    int ret = 0;
    int read_bytes;
    int readed_bytes;
    int remain_bytes;
    u32 file_offset = 0;
    u32 skip_offset = 0;
    u32 load_position_offset = 0;
    mloader_addr_s *mloader_addr;

    mloader_addr = bsp_mloader_get_addr();
    /* 读取指定偏移的指定大小 */
    if ((0 != offset) || (0 != size)) {
        skip_offset = offset;
        remain_bytes = (int)size;
    } else { /* 读取整个文件 */
        remain_bytes = mloader_get_file_size(file_name);
        if (remain_bytes <= 0) {
            mloader_print_err("error file_size 0x%x\n", remain_bytes);
            return remain_bytes;
        }

        if (remain_bytes <= SECBOOT_VRL_TABLE_SIZE) {
            mloader_print_err("error file_size (0x%x) less than VRL_SIZE\n", remain_bytes);
            return -EIO;
        }
        remain_bytes -= SECBOOT_VRL_TABLE_SIZE;
        skip_offset = SECBOOT_VRL_TABLE_SIZE;
    }

    /* 检查读取的大小是否超过ddr分区大小 */
    if ((u32)remain_bytes > ddr_size) {
        /* cov_verified_start */
        mloader_print_err("remain_bytes larger than ddr size:  remain_bytes 0x%x > ddr_size 0x%x!\n", remain_bytes,
                          ddr_size);
        return -ENOMEM;
        /* cov_verified_stop */
    }

    /* split the size to be read to each SECBOOT_BUFLEN bytes. */
    while (remain_bytes) {
        if (remain_bytes > MLOADER_SECBOOT_BUFLEN)
            read_bytes = MLOADER_SECBOOT_BUFLEN;
        else
            read_bytes = remain_bytes;

        readed_bytes = mloader_read_file(file_name, skip_offset + file_offset, (u32)read_bytes,
                                         (s8 *)mloader_addr->mloader_secboot_virt_addr_for_ccpu);
        if (readed_bytes < 0 || readed_bytes != read_bytes) {
            mloader_print_err("read_file %s err: readed_bytes 0x%x\n", file_name, readed_bytes);
            return readed_bytes;
        }

        if (!load_position_offset) {
            load_position_offset = mloader_patch_get_load_position_offset(ecoretype, ddr_size, remain_bytes);
        }

#ifdef CONFIG_COMPRESS_CCORE_IMAGE
        if ((!load_position_offset) && (ecoretype == MODEM))
            /* 将整个gzip格式的压缩镜像放在DDR空间结束位置 */
            load_position_offset = (u32)((ddr_size - (u32)remain_bytes) & ~0x3F);
#endif

        ret = mloader_trans_data_to_os(ecoretype, run_addr,
                                       (u64)(uintptr_t)(mloader_addr->mloader_secboot_phy_addr_for_ccpu),
                                       load_position_offset + file_offset, (u32)read_bytes);
        mloader_print_info(
            "trans data ot os: etype 0x%x ,load_offset 0x%x, to secos file_offset 0x%x, bytes 0x%x success\n",
            ecoretype, load_position_offset + file_offset, file_offset, read_bytes);

        if (ret) {
            /* cov_verified_start */
            mloader_print_err("modem image trans to os is failed, error code 0x%x\n", ret);
            return ret;
            /* cov_verified_stop */
        }

        remain_bytes -= read_bytes;
        file_offset += (u32)read_bytes;
    }

    return 0;
}
#else
int load_data_to_secos(const char *file_name, u32 offset, u32 size, enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr,
                       u32 ddr_size, u32 index)
{
    return 0;
}
#endif


#if (((defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)) || \
    ((defined CONFIG_TRUSTZONE_HM) && (defined CONFIG_LOAD_SEC_IMAGE)))

s32 load_image(char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr, u32 ddr_size, u32 index)
{
    s32 ret;
    int readed_bytes;
    mloader_addr_s *mloader_addr;

    mloader_addr = bsp_mloader_get_addr();

    /* load vrl data to sec os */
    readed_bytes = mloader_read_file(file_name, 0, SECBOOT_VRL_TABLE_SIZE,
                                     (char *)(mloader_addr->mloader_secboot_virt_addr_for_ccpu));
    if (readed_bytes < 0 || readed_bytes != SECBOOT_VRL_TABLE_SIZE) {
        mloader_print_err("read_file %s error, readed_bytes 0x%x!\n", file_name, readed_bytes);
        ret = readed_bytes;
        return ret;
    }

    ret = mloader_trans_vrl_to_os(ecoretype, (void *)(mloader_addr->mloader_secboot_virt_addr_for_ccpu),
                                  SECBOOT_VRL_TABLE_SIZE);
    if (ret) {
        /* cov_verified_start */
        mloader_print_err("trans_vrl_to_os error, ret 0x%x!\n", ret);
        return ret;
        /* cov_verified_stop */
    }
    mloader_print_err("trans vrl to secos success\n");

    /* load image data to sec os */
    ret = load_data_to_secos(file_name, 0, 0, ecoretype, run_addr, ddr_size, index);
    if (ret) {
        /* cov_verified_start */
        mloader_print_err("load image %s to secos failed, ret = 0x%x\n", file_name, ret);
        return ret;
        /* cov_verified_stop */
    }
    mloader_print_err("load image %s to secos success\n", file_name);
    return ret;
}

int bsp_mloader_load_verify_single_image(char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr,
                                         u32 ddr_size)
{
    int ret;
    osl_sem_down(&g_mloader_load_img_sem);
    ret = load_image(file_name, ecoretype, run_addr, ddr_size, 0);
    osl_sem_up(&g_mloader_load_img_sem);
    if (ret) {
        mloader_print_err("load image %s fail, ret = %d\n", file_name, ret);
        return ret;
    }
    mloader_print_err("load image %s succeed\n", file_name);

    return ret;
}

int bsp_mloader_load_single_image(const char *file_name, u32 offset, u32 size, enum SVC_SECBOOT_IMG_TYPE ecoretype,
                                  u32 run_addr, u32 ddr_size)
{
    int ret;

    osl_sem_down(&g_mloader_load_img_sem);
    ret = load_data_to_secos(file_name, offset, size, ecoretype, run_addr, ddr_size, 0);
    osl_sem_up(&g_mloader_load_img_sem);
    if (ret) {
        mloader_print_err("load image %s fail, ret = %d\n", file_name, ret);
        return ret;
    }
    mloader_print_err("load vrl and image %s succeed\n", file_name);
    return ret;
}

int bsp_mloader_load_single_vrl(const char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype)
{
    int ret;
    mloader_addr_s *mloader_addr;

    mloader_addr = bsp_mloader_get_addr();

    osl_sem_down(&g_mloader_load_img_sem);
    ret = mloader_trans_vrl_to_os(ecoretype, (void *)(mloader_addr->mloader_secboot_virt_addr_for_ccpu),
                                  SECBOOT_VRL_TABLE_SIZE);
    osl_sem_up(&g_mloader_load_img_sem);
    if (ret) {
        mloader_print_err("trans %s(%d) 's vrl to os failed, ret = %d\n", file_name, ecoretype, ret);
        return ret;
    }
    mloader_print_err("trans %s(%d) 's vrl to os succeed\n", file_name, ecoretype);
    return ret;
}

int bsp_mloader_verify_single_image(const char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype)
{
    int ret;

    ret = mloader_verify_soc_image(ecoretype, 0);
    if (ret) {
        mloader_print_err("verify image(%s) failed,image type is %d, ret = 0x%x\n", file_name, ecoretype, ret);
        return ret;
    }
    mloader_print_err("verify image(%s) succeed,image type is %d\n", file_name, ecoretype);
    return ret;
}
#else
int bsp_mloader_load_verify_single_image(char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr,
                                         u32 ddr_size)
{
    return 0;
}

int bsp_mloader_load_single_image(const char *file_name, u32 offset, u32 size, enum SVC_SECBOOT_IMG_TYPE ecoretype,
                                  u32 run_addr, u32 ddr_size)
{
    return 0;
}

int bsp_mloader_load_single_vrl(const char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype)
{
    return 0;
}

int bsp_mloader_verify_single_image(const char *file_name, enum SVC_SECBOOT_IMG_TYPE ecoretype)
{
    return 0;
}
#endif
