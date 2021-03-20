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
/*lint -save -e18 -e26 -e64 -e115 -e119 -e123 -e529 -e537 -e569 -e570 -e713 -e718 -e732 -e746 -e747 -e958 -e715 -e818*/
#if defined(CONFIG_MMC_MODEM_SD) && \
    (defined(CONFIG_MMC_DW_BALONG) || defined(CONFIG_MMC_DW_UPDATE_BALONG) || defined(CONFIG_MMC_SDHCI))
#include <osl_types.h>
#include <linux/scatterlist.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/ioctl.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/card.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/major.h>
#include <linux/uaccess.h>
#include "securec.h"
#include <bsp_print.h>


#define THIS_MODU mod_mmc

#define SDMMC_BLOCK_BYTE (512)
#define SDMMC_ONE_BLOCK (1)
enum mmc_at_process_status {
    SDMMC_PROCESS_OVER_OR_UNDO = 0,
    SDMMC_PROCESS_IN,
    SDMMC_PROCESS_ERROR,
};

enum mmc_at_process_error {
    SDMMC_NOT_IN = 0,
    SDMMC_INIT_FAIL,
    SDMMC_PARAMETER_ERROR,
    SDMMC_ADDR_ERROR,
    SDMMC_OTHER_ERROR,
    SDMMC_NO_ERROR_PARA
};
enum mmc_at_process_data {
    SDMMC_DATA0 = 0,
    SDMMC_DATA1,
    SDMMC_DATA2,
    SDMMC_DATA3,
    SDMMC_DATA_BUTT,
};
/* Command */
enum mmc_at_process_command {
    SDMMC_AT_FORMAT = 0,
    SDMMC_AT_ERASE_ALL,
    SDMMC_AT_ERASE_ADDR,
    SDMMC_AT_WRTIE_ADDR,
    SDMMC_AT_READ,
    SDMMC_AT_BUTT,
};

static unsigned int g_mmc_at_status;

int mmc_get_status(void)
{
    struct gendisk *disk;
    int partno = 0;
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    if (!disk) {
        return 0;
    }
    return 1;
}

int mmc_get_dev_idex(void)
{
    struct gendisk *disk;
    int partno = 0;
    int ret;
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    if (!disk) {
        return -ENOMEDIUM;
    }
    ret = disk->disk_name[6] - '0'; /*lint !e409*/
    return ret;
}
unsigned long mmc_get_capacity(void)
{
    struct gendisk *disk;
    int partno = 0;
    unsigned long ret;
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    if (!disk) {
        return 0;
    }
    ret = get_capacity(disk);
    return ret;
}

static unsigned long mmc_at_prepare(unsigned int tpye, unsigned int addr, char *buf, unsigned int *err,
                                    unsigned int *at_status)
{
    int status;
    unsigned int capacity;
    unsigned long ret = -1;

    if (!err || !buf) {
        goto out;
    }
    /* not support */
    if (g_mmc_at_status == SDMMC_PROCESS_IN) {
        *err = SDMMC_OTHER_ERROR;
        goto out;
    }
    *at_status = SDMMC_PROCESS_IN;
    if (tpye >= SDMMC_AT_BUTT) {
        *err = SDMMC_PARAMETER_ERROR;
        *at_status = SDMMC_PROCESS_ERROR;
        goto out;
    }
    status = mmc_get_status();
    /* sd card not present */
    if (!status) {
        *err = SDMMC_NOT_IN;
        *at_status = SDMMC_PROCESS_ERROR;
        goto out;
    }
    capacity = mmc_get_capacity();
    switch (tpye) {
        case SDMMC_AT_ERASE_ADDR:
            break;
        case SDMMC_AT_WRTIE_ADDR:
            break;
        case SDMMC_AT_READ:
            if ((addr + SDMMC_ONE_BLOCK) > capacity) {
                *err = SDMMC_ADDR_ERROR;
                *at_status = SDMMC_PROCESS_ERROR;
                goto out;
            }
            break;
        default:
            break;
    }
    return 0;
out:
    return ret;
}

static int mmc_at_erase(struct block_device *blk_dev, unsigned int from, unsigned int to)
{
    struct mmc_ioc_cmd cmd;
    int ret;

    ret = memset_s(&cmd, sizeof(struct mmc_ioc_cmd), 0, sizeof(struct mmc_ioc_cmd));
    if (ret != EOK) {
        bsp_err("[%s] memset_s err. ret =  %d.\n", __FUNCTION__, ret);
        goto out;
    }
    cmd.opcode = SD_ERASE_WR_BLK_START;
    cmd.arg = from;
    cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
    ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(uintptr_t)(&cmd));
    if (ret) {
        goto out;
    }
    cmd.opcode = SD_ERASE_WR_BLK_END;
    cmd.arg = to;
    cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
    ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(uintptr_t)(&cmd));
    if (ret) {
        goto out;
    }
    cmd.opcode = MMC_ERASE;
    cmd.arg = MMC_ERASE_ARG;
    cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
    ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(uintptr_t)(&cmd));
out:
    return ret;
}

static int mmc_at_rw(struct block_device *blk_dev, unsigned int from, unsigned char *buf, int write, int data)
{
    struct mmc_ioc_cmd cmd;
    int ret;
    unsigned char check_data[5] = { 0x00, 0x55, 0xAA, 0xFF, 0x00 };

    ret = memset_s(&cmd, sizeof(struct mmc_ioc_cmd), 0, sizeof(struct mmc_ioc_cmd));
    if (ret != EOK) {
        bsp_err("[%s] memset_s err. ret =  %d.\n", __FUNCTION__, ret);
        return ret;
    }
    if (write) {
        if (data > 4) {
            data = 4;
        }
        ret = memset_s(buf, SDMMC_BLOCK_BYTE + 1, check_data[data], SDMMC_BLOCK_BYTE);
        if (ret != EOK) {
            bsp_err("[%s] memset_s err. ret =  %d.\n", __FUNCTION__, ret);
            return ret;
        }
        cmd.write_flag = 1;
        cmd.opcode = MMC_WRITE_BLOCK;
    } else {
        cmd.write_flag = 0;
        cmd.opcode = MMC_READ_SINGLE_BLOCK;
    }
    cmd.blksz = SDMMC_BLOCK_BYTE;
    cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
    cmd.blocks = 1;
    cmd.arg = from;
    mmc_ioc_cmd_set_data(cmd, (uintptr_t)buf);

    ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(uintptr_t)(&cmd));
    return ret;
}

unsigned long mdrv_sd_get_opptstatus(void)
{
    return g_mmc_at_status;
}
unsigned int mmc_switch_type(unsigned int type, unsigned int addr, unsigned int data, unsigned char *buf,
                             struct block_device *blk_dev)
{
    unsigned int capacity = mmc_get_capacity();
    unsigned int from, to;
    unsigned int ret = 0;

    switch (type) {
        /* erase not yet suport */
        case SDMMC_AT_ERASE_ADDR:
            from = addr;
            to = from + 0x1;
            ret = mmc_at_erase(blk_dev, from, to);
            break;
        case SDMMC_AT_FORMAT:
        case SDMMC_AT_ERASE_ALL:
            from = 0x1;
            to = from + capacity - 1;
            ret = mmc_at_erase(blk_dev, from, to);
            break;
        case SDMMC_AT_WRTIE_ADDR:
            ret = mmc_at_rw(blk_dev, 0x1, buf, 0x1, data);
            break;
        case SDMMC_AT_READ:
            ret = mmc_at_rw(blk_dev, 0x1, buf, 0x0, 0x0);
            break;
        case SDMMC_AT_BUTT:
            break;
        default:
            break;
    }
    return ret;
}

/*
 * 功能描述: at^sd,SD卡操作，写，擦除，格式化操作
 * 返 回 值: 0 ：OK  非 0 ：Error
 *          具体的错误值填充在*pulErr中
 *          0 表示SD卡不在位
 *          1 表示SD卡初始化失败
 *          2 表示<opr>参数非法，对应操作不支持(该错误由AT使用,不需要底软使用)
 *          3 表示<address>地址非法，超过SD卡本身容量
 *          4 其他未知错误
 */
unsigned int mdrv_sd_at_process(unsigned int type, unsigned int addr, unsigned int data, unsigned char *buf,
                                unsigned int *err)
{
    unsigned int ret;
    int partno = 0;

    struct gendisk *disk = NULL;
    struct block_device *blk_dev = NULL;

    /* parameter check */
    ret = mmc_at_prepare(type, addr, buf, err, &g_mmc_at_status);
    if (ret) {
        goto out;
    }
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    if (disk == NULL) {
        goto out;
    }
    blk_dev = bdget_disk(disk, partno);
    if (blk_dev == NULL) {
        goto out;
    }
    /* 在进行下面的sd卡操作前，首先要打开该设备，建立block_device和gendisk的关联，防止为空造成单板重启 */
    ret = blkdev_get(blk_dev, FMODE_WRITE | FMODE_READ, NULL);
    if (ret) {
        goto out;
    }
    ret = mmc_switch_type(type, addr, data, buf, blk_dev);

out:
    if (ret) {
        *err = SDMMC_OTHER_ERROR;
        g_mmc_at_status = SDMMC_PROCESS_ERROR;
    } else {
        *err = SDMMC_NO_ERROR_PARA;
        g_mmc_at_status = SDMMC_PROCESS_OVER_OR_UNDO;
        /* 在at命令处理结束时，关闭该设备，恢复原始状态 */
        blkdev_put(blk_dev, FMODE_WRITE | FMODE_READ);
    }
    return ret;
}

unsigned int at_test(unsigned int type, unsigned int addr, unsigned int data)
{
    unsigned char buf[SDMMC_BLOCK_BYTE];
    unsigned int err;
    unsigned int ret;
    ret = mdrv_sd_at_process(type, addr, data, (unsigned char *)buf, &err);
    print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1, buf, SDMMC_BLOCK_BYTE, 0);
    return ret;
}
#else
/*
 * 功能描述: 卡容量查询
 * 返 回 值: 0 : 失败；>0: 卡容量
 */
unsigned int mdrv_sd_get_capacity(void)
{
    return 0;
}

/*
 * 功能描述: 卡在位查询
 * 返 回 值: 0 : 在位；-1: 不在位
 */
int mdrv_sd_get_status(void)
{
    return 0;
}

unsigned long mdrv_sd_get_opptstatus(void)
{
    return 0;
}

unsigned int mdrv_sd_at_process(unsigned int type, unsigned int addr, unsigned int data, unsigned char *buf,
                                unsigned int *err)
{
    return 0;
}

/*
 * 功能描述: SD多块数据传输sg list初始化
 * 返 回 值:  0 : 成功;  其它:失败
 * 其它说明: NA
 */
int mdrv_sd_sg_init_table(const void *buf, unsigned int buflen)
{
    return 0;
}

/*
 * 功能描述: SD多块数据传输
 * 返 回 值:  0 : 成功;  其它:失败
 * 其它说明: NA
 */
int mdrv_sd_multi_transfer(unsigned int dev_addr, unsigned int blocks, unsigned int blksz, int write)
{
    return 0;
}
#endif


