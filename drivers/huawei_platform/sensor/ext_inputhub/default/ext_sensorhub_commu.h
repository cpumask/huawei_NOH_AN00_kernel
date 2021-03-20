/*
 * ext_sensorhub_commu.h
 *
 * head file for communication with external sensorhub
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
 *
 */

#ifndef EXT_SENSORHUB_COMMU_H
#define EXT_SENSORHUB_COMMU_H
#include <linux/types.h>

int commu_probe_init(void);

void commu_exit(void);

ssize_t commu_write(const unsigned char *buf, size_t len);

void commu_suspend(void);

void commu_resume(void);

#endif /* EXT_SENSORHUB_COMMU_H */
