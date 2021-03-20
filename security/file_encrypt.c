/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: This file is for controlling the read/write operations of ece
 *              files when screen is locked/unlocked.
 * Author: laixinyi
 * Create: 2020-02-12
 */

#include "file_encrypt.h"

#include <linux/capability.h>
#include <linux/audit.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/ptrace.h>
#include <linux/hugetlb.h>
#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/prctl.h>
#include <linux/securebits.h>
#include <linux/user_namespace.h>
#include <linux/binfmts.h>
#include <linux/personality.h>
#include <linux/syscalls.h>
#include <linux/module.h>

#include <linux/hisi/hisi_fbe_ctrl.h>

#define F2FS_XATTR_SDP_ECE_ENABLE_FLAG 0x01

static spinlock_t fe_rw_check_lock;
static int fe_key_file_rw_count;
static bool fe_lock_notify;
static struct work_struct fe_key_protect_work;

static bool fe_is_ece_file(struct inode *inode)
{
	if (inode && inode->i_crypt_info) {
		if (inode->i_crypt_info->ci_hw_enc_flag ==
		    F2FS_XATTR_SDP_ECE_ENABLE_FLAG)
			return true;
	}

	return false;
}

int ece_screen_lock_check(struct inode *inode)
{
	struct fscrypt_info *ci_info = inode->i_crypt_info;

	if (!ci_info) {
		pr_err("[FBE3]%s:no crypt info for inode %llu\n", __func__,
		       inode->i_ino);
		return -EINVAL;
	}

	return f2fs_inode_check_sdp_keyring(ci_info->ci_master_key, 0);
}

static void fe_do_key_protect(struct work_struct *work)
{
	bool clear_key = false;

	sys_sync();
	spin_lock(&fe_rw_check_lock);
	clear_key = fe_lock_notify && (!fe_key_file_rw_count);
	spin_unlock(&fe_rw_check_lock);
	if (clear_key) {
		pr_err("%s: rw count zero, clear keyslot!\n", __func__);
		hisi_fbe3_flushcache_done();
	}
}

static void fe_active_key_protect(void)
{
	schedule_work(&fe_key_protect_work);
}

void hisi_fbe3_lock_in(void)
{
	spin_lock(&fe_rw_check_lock);
	fe_lock_notify = true;
	pr_err("%s!\n", __func__);
	if (!fe_key_file_rw_count)
		fe_active_key_protect();
	spin_unlock(&fe_rw_check_lock);
}

void hisi_fbe3_unlock_in(void)
{
	spin_lock(&fe_rw_check_lock);
	fe_lock_notify = false;
	pr_err("%s!\n", __func__);
	spin_unlock(&fe_rw_check_lock);
}

int rw_begin(struct file *file)
{
	struct inode *inode = NULL;
	int ret = 0;

	if (file->f_mapping)
		inode = file->f_mapping->host;
	if (inode && fe_is_ece_file(inode)) {
		int lock_check = ece_screen_lock_check(inode);
		spin_lock(&fe_rw_check_lock);
		if (lock_check) {
			pr_err("[FBE3]%s: rw for %lu is blocked when screen locked\n",
			       __func__, inode->i_ino);
			ret = -EKEYREVOKED;
		} else {
			fe_key_file_rw_count++;
		}
		spin_unlock(&fe_rw_check_lock);
	}

	return ret;
}

void rw_finish(int read_write, struct file *file)
{
	struct inode *inode = NULL;

	if (file->f_mapping)
		inode = file->f_mapping->host;
	if (inode && fe_is_ece_file(inode)) {
		spin_lock(&fe_rw_check_lock);
		if (!fe_key_file_rw_count) {
			pr_err("[FBE3]%s: no match of rw count\n", __func__);
#ifdef CONFIG_HISI_DEBUG_FS
			BUG();
#endif
		}

		if (!(--fe_key_file_rw_count) && fe_lock_notify)
			fe_active_key_protect();
		spin_unlock(&fe_rw_check_lock);
	}
}

static int __init fe_init(void)
{
	spin_lock_init(&fe_rw_check_lock);
	INIT_WORK(&fe_key_protect_work, fe_do_key_protect);
	return 0;
}
fs_initcall(fe_init);
