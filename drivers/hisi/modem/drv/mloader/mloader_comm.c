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
 * File Name       : mloader_comm.c
 * Description     : load modem image(ccore image),run in ccore
 * History         :
 */
#include "product_config.h"
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/statfs.h>
#include <linux/slab.h>
#include <linux/zlib.h>
#include <bsp_slice.h>
#include "mloader_comm.h"
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/stat.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/statfs.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <bsp_print.h>
#include <hi_mloader.h>
#include <securec.h>
#include <bsp_cold_patch.h>
#include <bsp_rfile.h>
#include "mloader_debug.h"

#define THIS_MODU mod_mloader

extern mloader_info_s g_mloader_info;

#define MODEM_IMAGE_PATH_ROOT "/modem_fw/"
static char *g_modem_fw_path = MODEM_IMAGE_PATH;

int modem_dir_init(void)
{
    if (!bsp_access((s8 *)MODEM_IMAGE_PATH, 0)) {
        g_modem_fw_path = MODEM_IMAGE_PATH;
    } else if (!bsp_access((s8 *)MODEM_IMAGE_PATH_ROOT, 0)) {
        g_modem_fw_path = MODEM_IMAGE_PATH_ROOT;
    } else {
        mloader_print_err("error: path /vendor/modem/modem_fw/  and path /modem_fw/ can't access, return\n");
        return -EACCES;
    }

    mloader_print_err("info: path %s   can access\n", g_modem_fw_path);
    return 0;
}

ccpu_img_info_s *mloader_get_image_info(const char *image_name)
{
    int id;
    for (id = 0; id < g_mloader_info.image_num; id++) {
        if (!strcmp(g_mloader_info.img[id].image_name, image_name)) {
            return &g_mloader_info.img[id];
        }
    }

    return NULL;
}

int mloader_get_file_size(const char *filename)
{
    int ret = 0;
    struct kstat kstattmp = {0};
    unsigned long old_fs;
    int len;
    int fail_time;

    len = strnlen(filename, MLOADER_FILE_NAME_LEN);
    /* 如果是patch镜像，查询镜像的加载失败次数，如果失败次数达到3次，不再加载patch镜像 */
    if ((filename[len - 1] == 'p') && (filename[len - 2] == '.')) {
        fail_time = mloader_patch_get_load_fail_time();
        if (fail_time >= 3) {
            return 0;
        }
    }
    if (!bsp_access((s8 *)filename, RFILE_RDONLY)) {
        old_fs = get_fs();
        set_fs((mm_segment_t)KERNEL_DS);
        ret = vfs_stat(filename, &kstattmp);
        set_fs(old_fs);
        if (ret) {
            mloader_print_err("error: get file %s size failed, ret = %d\n", filename, ret);
            return MLOADER_INTERFACE_ERROR;
        }
        return kstattmp.size;
    }
    return ret;
}

int mloader_get_file_name(char *file_name, const char *image_name, u32 is_sec)
{
    u32 ret;
    int len;
    file_name[0] = '\0';

    len = strnlen(image_name, MLOADER_FILE_NAME_LEN);
    /* 如果是patch镜像，则补全patch镜像的路径，否则补全各核镜像的路径 */
    if ((image_name[len - 1] == 'p') && (image_name[len - 2] == '.')) {
        ret = (u32)strncat_s(file_name, MLOADER_FILE_NAME_LEN, MLOADER_COLD_PATCH_PATH,
                             strnlen(MLOADER_COLD_PATCH_PATH, MLOADER_FILE_NAME_LEN));
    } else {
        file_name[0] = '\0';
        ret = (u32)strncat_s(file_name, MLOADER_FILE_NAME_LEN, g_modem_fw_path,
                             strnlen(g_modem_fw_path, MLOADER_FILE_NAME_LEN));
    }
    ret |= (u32)strncat_s(file_name, MLOADER_FILE_NAME_LEN, image_name, strnlen(image_name, MLOADER_FILE_NAME_LEN));
    if (ret != EOK) {
        mloader_print_err("mloader_get_file_name:strncat_s failed1,ret=0x%x!\n", ret);
        return ret;
    }

    return 0;
}

int mloader_read_file(const char *file_name, unsigned int offset, unsigned int length, char *buffer)
{
    struct file *fp = NULL;
    int retval;
    loff_t offset_adp = (loff_t)offset;
    fp = filp_open(file_name, O_RDONLY, 0600);
    if ((!fp) || (IS_ERR(fp))) {
        retval = (int)PTR_ERR(fp);
        mloader_print_err("filp_open(%s) failed, ret:%d", file_name, retval);
        return retval;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
    retval = kernel_read(fp, buffer, (unsigned long)length, (loff_t *)&offset_adp);
#else
    retval = kernel_read(fp, offset_adp, buffer, (unsigned long)length);
#endif

    if (retval != (int)length) {
        mloader_print_err("kernel_read(%s) failed, retval %d, require len %u\n", file_name, retval, length);
        if (retval >= 0) {
            retval = -EIO;
        }
    }

    filp_close(fp, NULL);
    return retval;
}

int gzip_header_check(const unsigned char *zbuf)
{
    if (zbuf[0] != 0x1f || zbuf[1] != 0x8b || zbuf[2] != 0x08) {
        return 0;
    } else {
        return 1;
    }
}

int mloader_decompress_zlib(u32 ddr_size, void *vaddr_load, void *vaddr, u32 file_size)
{
    char *zlib_next_in = NULL;
    unsigned int zlib_avail_in;
    int ret;
    u32 start, end;

    zlib_next_in = (char *)vaddr_load;

    /* skip over asciz filename */
    if ((unsigned char)zlib_next_in[3] & 0x8) {
        /*
         * skip over gzip header (1f,8b,08... 10 bytes total +
         * possible asciz filename)
         */
        zlib_next_in = zlib_next_in + 10;
        zlib_avail_in = (unsigned)((file_size - 10) - 8);
        do {
            /*
             * If the filename doesn't fit into the buffer,
             * the file is very probably corrupt. Don't try
             * to read more data.
             */
            if (zlib_avail_in == 0) {
                mloader_print_err("gzip header error");
                return -EIO;
            }
            --zlib_avail_in;
        } while (*zlib_next_in++);
    } else {
        /*
         * skip over gzip header (1f,8b,08... 10 bytes total +
         * possible asciz filename)
         */
        zlib_next_in = zlib_next_in + 10;
        zlib_avail_in = (unsigned)((file_size - 10) - 8);
    }
    start = bsp_get_slice_value();
    ret = zlib_inflate_blob(vaddr, ddr_size, (void *)zlib_next_in, zlib_avail_in);
    end = bsp_get_slice_value();
    mloader_print_err("zlib inflate time 0x%x.\n", end - start);
    if (ret < 0) {
        mloader_print_err("fail to decompress image , error code %d\n", ret);
        return ret;
    } else {
        mloader_print_err("decompress image success. file length = 0x%x\n", (unsigned)ret);
        return 0;
    }
}
int mloader_decompress(u32 ddr_size, void *vaddr_load, void *vaddr, u32 file_size)
{
    if (gzip_header_check(vaddr_load)) {
        return mloader_decompress_zlib(ddr_size, vaddr_load, vaddr, file_size);
    } else {
        mloader_print_err("unknown head info, please check.\n");
        return MLOADER_PARAM_ERROR;
    }
}
