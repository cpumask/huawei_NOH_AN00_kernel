
/*
 * ufs-hisi-dump.h
 *
 * ufs dump for hisilicon
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __UFS_HISI_DUMP_H__
#define __UFS_HISI_DUMP_H__

#include "ufshcd.h"

#ifdef CONFIG_HISI_UFS_HC
void hisi_ufs_key_reg_dump(struct ufs_hba *hba);
#else
static inline void hisi_ufs_key_reg_dump(struct ufs_hba *hba)
{
}

#endif
#endif
