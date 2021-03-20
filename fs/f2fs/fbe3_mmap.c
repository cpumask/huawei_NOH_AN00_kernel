/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ECE files read/write control under screen locked
 * Author: LAI Xinyi
 * Create: 2020-03-25
 */

#include "fbe3_mmap.h"

#include <linux/fs.h>
#include <linux/fscrypt_common.h>

#define F2FS_XATTR_SDP_ECE_ENABLE_FLAG 0x01

static bool fs_ece_file_screen_lock_check(struct inode *inode)
{
	int screen_lock = 0;
	bool flag = false;

#if F2FS_FS_SDP_ENCRYPTION
	/* If ECE && Lock Screen return true, otherwise, return false */
	if (inode && inode->i_crypt_info) {
		if (inode->i_crypt_info->ci_hw_enc_flag ==
		    F2FS_XATTR_SDP_ECE_ENABLE_FLAG) {
			flag = true;
			screen_lock = ece_screen_lock_check(inode);
		}
	}

	if (flag && screen_lock)
		return true;
#endif

	return false;
}

int mmap_read_check(struct vm_fault *vmf)
{
	struct inode *inode = NULL;

	if (!vmf->vma->vm_file)
		return 0;
	inode = file_inode(vmf->vma->vm_file);

	return fs_ece_file_screen_lock_check(inode) ? VM_FAULT_SIGSEGV : 0;
}

int mmap_write_check(struct vm_fault *vmf)
{
	struct inode *inode = NULL;

	if (!vmf->vma->vm_file)
		return 0;
	inode = file_inode(vmf->vma->vm_file);
	return fs_ece_file_screen_lock_check(inode) ? VM_FAULT_SIGSEGV : 0;
}
