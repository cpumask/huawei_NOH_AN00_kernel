/*
 * hisi_eis_core_sysfs.h
 *
 * eis core sysfs interface header
 *
 * Copyright (c) 2018-2020 Huawei Technologies Co., Ltd.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _HISI_EIS_CORE_SYSFS_H_
#define _HISI_EIS_CORE_SYSFS_H_

#include "hisi_eis_core_freq.h"

int eis_create_sysfs_file(struct hisi_eis_device *di);

#endif /* _HISI_EIS_CORE_SYSFS_H_ */

