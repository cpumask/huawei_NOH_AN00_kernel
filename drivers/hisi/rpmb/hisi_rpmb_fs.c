/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: storage read-write for common partition
 * Author: Peng Jiali
 * Create: 2020-02-14
 */

#include "hisi_rpmb_fs.h"
#include <asm/compiler.h>
#include <linux/errno.h>
#include <linux/compiler.h>
#include <linux/fd.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/hisi/kirin_partition.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <securec.h>
#include "hisi_rpmb.h"

#define STORAGE_FILESYS_DEFAULT_MODE       0640
#define STORAGE_PTN_PATH_BUF_SIZE 128

static int storage_read(const char *ptn_name, long ptn_offset, char *data_buf,
			unsigned int data_size)
{
	int fd = -1;
	int ret;
	ssize_t cnt;
	mm_segment_t old_fs;
	char fullpath[STORAGE_PTN_PATH_BUF_SIZE] = { 0 };

	if (!data_buf || !ptn_name) {
		pr_err("%s: data_buf or ptn_name maybe invalid\n", __func__);
		return -EINVAL;
	}

	/* 1. find the partition path name */
	ret = flash_find_ptn_s(ptn_name, fullpath, sizeof(fullpath));
	if (ret) {
		pr_err("%s: flash_find_ptn_s fail, ret = %d\n", __func__, ret);
		return ret;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	/* 2. open file */
	fd = (int)sys_open(fullpath, O_RDONLY, STORAGE_FILESYS_DEFAULT_MODE);
	if (fd < 0) {
		pr_err("%s: open %s failed, fd = %d\n", __func__, ptn_name, fd);
		ret = fd;
		goto out;
	}

	/* 3. read data */
	ret = (int)sys_lseek((unsigned int)fd, ptn_offset, SEEK_SET);
	if (ret < 0) {
		pr_err("%s: sys_lseek fail, ret = %d\n", __func__, ret);
		goto out;
	}

	cnt = sys_read((unsigned int)fd, (char __user *)data_buf,
		       (unsigned long)data_size);
	if (cnt < (ssize_t)data_size) {
		pr_err("%s: read %s failed, %ld completed\n", __func__,
		       ptn_name, cnt);
		ret = -EINVAL;
		goto out;
	}

	ret = 0;
out:
	if (fd >= 0)
		sys_close((unsigned int)fd);
	set_fs(old_fs);
	return ret;
}

static int storage_write(const char *ptn_name, long ptn_offset,
			 const char *data_buf, unsigned int data_size)
{
	int fd = -1;
	int ret;
	ssize_t cnt;
	mm_segment_t old_fs;
	char fullpath[STORAGE_PTN_PATH_BUF_SIZE] = { 0 };

	if (!data_buf || !ptn_name) {
		pr_err("%s: data_buf or ptn_name maybe invalid\n", __func__);
		return -EINVAL;
	}

	/* 1. find the partition path name */
	ret = flash_find_ptn_s(ptn_name, fullpath, sizeof(fullpath));
	if (ret) {
		pr_err("%s: flash_find_ptn_s fail, ret = %d\n", __func__, ret);
		return ret;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	/* 2. open file */
	fd = (int)sys_open(fullpath, O_WRONLY, STORAGE_FILESYS_DEFAULT_MODE);
	if (fd < 0) {
		pr_err("%s: open %s failed, fd = %d\n", __func__, ptn_name, fd);
		ret = fd;
		goto out;
	}

	/* 3. write data */
	ret = (int)sys_lseek((unsigned int)fd, ptn_offset, SEEK_SET);
	if (ret < 0) {
		pr_err("%s: sys_lseek fail, ret = %d\n", __func__, ret);
		goto out;
	}

	cnt = sys_write((unsigned int)fd, (char __user *)data_buf,
			(unsigned long)data_size);
	if (cnt < (ssize_t)data_size) {
		pr_err("%s: write %s failed, %ld completed\n", __func__,
		       ptn_name, cnt);
		ret = -EINVAL;
		goto out;
	}

	ret = sys_fsync((unsigned int)fd);
	if (ret < 0) {
		pr_err("%s: sys_fsync failed, ret = %d\n", __func__, ret);
		goto out;
	}

	ret = 0;
out:
	if (fd >= 0)
		sys_close((unsigned int)fd);
	set_fs(old_fs);
	return ret;
}

int storage_issue_work(struct storage_rw_packet *storage_request)
{
	int ret;

	switch (storage_request->req_type) {
	case READ_REQ:
		ret = storage_read(storage_request->ptn_name,
				   storage_request->ptn_offset,
				   storage_request->data_buf,
				   storage_request->data_size);
		break;
	case WRITE_REQ:
		ret = storage_write(storage_request->ptn_name,
				    storage_request->ptn_offset,
				    storage_request->data_buf,
				    storage_request->data_size);
		break;
	default:
		pr_err("[%s]: request type error, req_type = 0x%x\n", __func__,
		       storage_request->req_type);
		ret = -1;
		break;
	}

	return ret;
}

