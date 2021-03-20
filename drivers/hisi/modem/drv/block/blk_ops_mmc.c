#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/statfs.h>
#include <securec.h>
#include <product_config.h>
#include <bsp_print.h>
#include <bsp_blk.h>
#include <bsp_version.h>
#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#else
#include <bsp_partition.h>
#endif


#define THIS_MODU mod_blk
#define BLK_PATH_LEN_MAX 128

#ifdef BSP_CONFIG_PHONE_TYPE
int mmc_blk_get_cdromiso_blkname(char *blk_path, int len)
{
    return 0;
}
#else
int mmc_blk_get_cdromiso_blkname(char *blk_path, int len)
{
    int ret;
    if (blk_path == NULL || len < BLK_PATH_LEN_MAX) {
        bsp_err("%s para err.\n", __func__);
        return -1;
    }

    ret = flash_find_ptn_s(PTABLE_CDROMISO_NM, blk_path, BLK_PATH_LEN_MAX);
    if (ret < 0) {
        bsp_err("%s not found from partition table!\n", PTABLE_CDROMISO_NM);
        return -1;
    }

    return 0;
}
#endif

int mmc_flash_find_ptn_s(const char *part_name, char *blk_path, unsigned int blk_path_len)
{
    if ((part_name == NULL) || (blk_path == NULL) || (!strlen(part_name))) {
        bsp_err("%s para err.\n", __func__);
        return -1;
    }

    return sprintf_s(blk_path, blk_path_len, "/dev/block/platform/hi_mci.0/by-name/%s", part_name);
}


int mmc_blk_size(const char *part_name, u32 *size)
{ /*lint --e{501}*/
    mm_segment_t fs;
    long ret_close, ret;
    long fd = -1;
    loff_t isize = 0;

    char blk_path[BLK_PATH_LEN_MAX] = "";

    /* check param */
    if (part_name == NULL || size == NULL) {
        bsp_err("%s:invalid param.\n", __func__);
        return -1;
    }
    ret = (long)flash_find_ptn_s(part_name, (char *)blk_path, BLK_PATH_LEN_MAX);
    if (ret < 0) {
        bsp_err("%s not found from partition table!\n", part_name);
        return -1;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    fd = sys_open(blk_path, O_RDONLY, 0600);
    if (fd < 0) {
        bsp_err("fail to open file %s, ret %ld!\n", blk_path, fd);
        goto open_err;
    }

    ret = sys_ioctl(fd, BLKGETSIZE64, (uintptr_t)&isize);
    if (ret < 0) {
        bsp_err("get %s size is failed, ret %ld!\n", blk_path, ret);
        goto ioctl_err;
    }
    *size = (u32)isize;
    ret = 0;
ioctl_err:
    ret_close = sys_close(fd);
    if (ret_close) {
        ret = -1;
        bsp_err("%s close failed??, ret %ld\n", blk_path, ret_close);
    }
    fd = -1;
open_err:
    set_fs(fs);

    return ret;
}

int mmc_check_file_len(long fd, loff_t part_offset, size_t data_len, const char* blk_path)
{
    long ret;
    loff_t size = 0;

    ret = sys_ioctl(fd, BLKGETSIZE64, (uintptr_t)&size);
    if (ret < 0) {
        bsp_err("get %s size is failed, ret %ld!\n", blk_path, ret);
        return -1;
    }

    if (part_offset > size || (part_offset + (loff_t)data_len > size)) {
        bsp_err("%s invalid offset %lld data_len %zu size %lld!\n", blk_path, part_offset, data_len, size);
        return -1;
    }

    ret = sys_lseek(fd, part_offset, SEEK_SET);
    if (ret < 0) {
        bsp_err("%s lseek %lld failed, ret %ld!\n", blk_path, part_offset, ret);
        return -1;
    }
    return 0;
}

int mmc_blk_read(const char *part_name, loff_t part_offset, void *data_buf, size_t data_len)
{ /*lint --e{838,501}*/
    mm_segment_t fs;
    long ret_close, ret, len;
    long fd = -1;
    char blk_path[BLK_PATH_LEN_MAX] = "";

    /* check param */
    if (part_name == NULL || data_buf == NULL) {
        bsp_err("%s:invalid param.\n", __func__);
        return -1;
    }

    ret = flash_find_ptn_s(part_name, (char *)blk_path, BLK_PATH_LEN_MAX);
    if (ret < 0) {
        bsp_err("%s not found from partition table!\n", part_name);
        return -1;
    }
    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    fd = sys_open(blk_path, O_RDONLY, 0600);
    if (fd < 0) {
        bsp_err("fail to open file %s, ret %ld!\n", blk_path, fd);
        goto open_err;
    }
    ret = mmc_check_file_len(fd, part_offset, data_len, (const char*)blk_path);
    if (ret) {
        goto ioctl_err;
    }
    len = sys_read(fd, data_buf, data_len);
    if (len != data_len) {
        ret = -1;
        bsp_err("%s read error, data_len %zu read_len %ld!\n", blk_path, data_len, len);
        goto ioctl_err;
    }
    ret = 0;

ioctl_err:
    ret_close = sys_close(fd);
    if (ret_close) {
        ret = -1;
        bsp_err("%s close failed, ret %ld\n", blk_path, ret_close);
    }
    fd = -1;
open_err:
    set_fs(fs);

    return ret;
}

int mmc_blk_write(const char *part_name, loff_t part_offset, const void *data_buf, size_t data_len)
{ /*lint --e{838, 501}*/
    mm_segment_t fs;
    long ret_close, ret, len;
    long fd = -1;
    char blk_path[BLK_PATH_LEN_MAX] = "";

    /* check param */
    if (part_name == NULL || data_buf == NULL) {
        bsp_err("%s:invalid param.\n", __func__);
        return -1;
    }

    ret = flash_find_ptn_s(part_name, (char *)blk_path, BLK_PATH_LEN_MAX);
    if (ret < 0) {
        bsp_err("%s not found from partition table!\n", part_name);
        return -1;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    fd = sys_open(blk_path, O_WRONLY | O_DSYNC, 0600);
    if (fd < 0) {
        bsp_err("fail to open file %s, ret %ld!\n", blk_path, fd);
        goto open_err;
    }

    ret = mmc_check_file_len(fd, part_offset, data_len, (const char*)blk_path);
    if (ret) {
        goto ioctl_err;
    }

    len = sys_write(fd, data_buf, data_len);
    if (len != data_len) {
        ret = -1;
        bsp_err("%s read error, data_len %zu read_len %ld!\n", blk_path, data_len, len);
        goto ioctl_err;
    }

    ret = sys_fsync(fd);
    if (ret < 0) {
        bsp_err("%s fsync failed, ret %ld!\n", blk_path, ret);
        goto ioctl_err;
    }
    ret = 0;

ioctl_err:
    ret_close = sys_close(fd);
    if (ret_close) {
        ret = -1;
        bsp_err("%s close failed, ret %ld\n", blk_path, ret_close);
    }
    fd = -1;
open_err:
    set_fs(fs);

    return ret;
}

struct block_ops g_block_ops_mmc = {
    .read = mmc_blk_read,
    .write = mmc_blk_write,
    .get_part_size = mmc_blk_size,
    .get_cdromiso_blkname = mmc_blk_get_cdromiso_blkname,
    .find_part = mmc_flash_find_ptn_s,
};

int mmc_init(void)
{
#ifndef BSP_CONFIG_PHONE_TYPE
    u32 boot_mode;
    if (bsp_blk_is_support() != BLOCK_BALONG_IS_SUPPORT) {
        bsp_err("<%s> flash is not needed\n", __func__);
        return 0;
    }
    boot_mode = bsp_boot_mode_get();
    if (boot_mode != BOOT_MODE_EMMC0) {
        return 0;
    }
#endif
    if (bsp_blk_register_ops(&g_block_ops_mmc)) {
        bsp_err("<%s> bsp_blk_register_ops failed\n", __func__);
        return -1;
    }

    return 0;
}


#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
fs_initcall(mmc_init);
#endif


