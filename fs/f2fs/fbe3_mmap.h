/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Internal functions for fbe3 mmap read/write control
 * Author: LAI Xinyi
 * Create: 2020-03-25
 */

#ifndef __FBE3_MMAP_H__
#define __FBE3_MMAP_H__

#include <linux/mm.h>

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V3
int mmap_read_check(struct vm_fault *vmf);
int mmap_write_check(struct vm_fault *vmf);
#endif

#endif
