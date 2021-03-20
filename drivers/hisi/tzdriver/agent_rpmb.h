/*
 * agent_rpmb.h
 *
 * function declarment for rpmb
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _AGENT_RPMB_H
#define _AGENT_RPMB_H

#if defined(CONFIG_HISI_RPMB) || defined(CONFIG_HISI_MMC_SECURE_RPMB)
#include <linux/hisi/rpmb.h>

int hisi_rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation,
	struct storage_blk_ioc_rpmb_data *storage_data);
int rpmb_agent_register(void);

#else

static inline int rpmb_agent_register(void)
{
	return 0;
}

#endif

#endif
