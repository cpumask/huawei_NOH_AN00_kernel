/*
 * tee_s4.c
 *
 * function for proc open,close session and invoke
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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
#include "tee_s4.h"
#include <securec.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include "global_ddr_map.h"
#include "tc_ns_client.h"
#include "teek_ns_client.h"
#include "tc_ns_log.h"

#define S4_ADDR_4G              0xffffffff


int tc_s4_pm_suspend(struct device *dev)
{
	return 0;
}

int tc_s4_pm_resume(struct device *dev)
{
	return 0;
}
