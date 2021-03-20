/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: hisi bkops sysfs interface
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __HISI_BKOPS_SYSFS_INTERFACE_H__
#define __HISI_BKOPS_SYSFS_INTERFACE_H__
#include <linux/debugfs.h>
#include <linux/hisi-bkops-core.h>

extern int __cfi_hisi_bkops_stat_open(struct inode *inode, struct file *filp);
extern int hisi_bkops_stat_open(const struct inode *inode, struct file *filp);
extern ssize_t __cfi_hisi_bkops_stat_read(
	struct file *filp, char __user *ubuf, size_t cnt, loff_t *ppos);
extern ssize_t hisi_bkops_stat_read(
	const struct file *filp, char __user *ubuf, size_t cnt,
	const loff_t *ppos);
extern int __cfi_hisi_bkops_stat_release(
	struct inode *inode, struct file *file);
extern int hisi_bkops_stat_release(
	const struct inode *inode, struct file *file);
extern int __cfi_hisi_bkops_force_query_open(
	struct inode *inode, struct file *filp);
extern int hisi_bkops_force_query_open(
	const struct inode *inode, struct file *filp);
extern ssize_t __cfi_hisi_bkops_force_query_read(
	struct file *filp, char __user *ubuf, size_t cnt, loff_t *ppos);
extern ssize_t hisi_bkops_force_query_read(
	const struct file *filp, char __user *ubuf, size_t cnt,
	const loff_t *ppos);
extern int __cfi_hisi_bkops_force_query_release(
	struct inode *inode, struct file *file);
extern int hisi_bkops_force_query_release(
	const struct inode *inode, const struct file *file);
#endif /* __HISI_BKOPS_SYSFS_INTERFACE_H__ */
