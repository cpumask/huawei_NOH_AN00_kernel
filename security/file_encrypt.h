/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: functions for ece files read/write operations when lock/unlock
 *              screen
 * Author: LAI Xinyi
 * Create: 2020.02.12
 */
#ifndef __FILE_ENCRYPT_H__
#define __FILE_ENCRYPT_H__

#include <linux/fscrypt_common.h>
#include <linux/types.h>
#include <crypto/kpp.h>

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V3
#ifndef F2FS_FS_SDP_ENCRYPTION
#define F2FS_FS_SDP_ENCRYPTION 1
#endif

void hisi_fbe3_lock_in(void);
void hisi_fbe3_unlock_in(void);
void rw_finish(int read_write, struct file *file);

#endif
#endif
