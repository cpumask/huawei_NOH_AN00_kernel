/*
 * slimbus is a kernel driver which is used to manager slimbus devices
 * Copyright (C) 2014 Hisilicon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _SLIMBUS_UTILS_H_
#define _SLIMBUS_UTILS_H_

#include "soc_acpu_baseaddr_interface.h"
#include "slimbus_types.h"

#ifndef SOC_ACPU_AO_IOC_BASE_ADDR
#define IOC_BASE_ADDR SOC_ACPU_PAD_AO_IOMG_IOCG_BASE_ADDR
#else
#define IOC_BASE_ADDR SOC_ACPU_AO_IOC_BASE_ADDR
#endif
#define IOC_REG_SIZE 0x1000

#define IOC_SYS_IOMG_011 0x02c
#define IOC_SYS_IOMG_012 0x030
#define IOC_SYS_IOCG_013 0x834
#define IOC_SYS_IOCG_014 0x838

#define IOC_SYS_IOMG_014 0x038
#define IOC_SYS_IOMG_015 0x03c

#define IOC_SYS_IOCG_027 0x86C
#define IOC_SYS_IOCG_028 0x870

#define SLIMBUS_PORT_NUM 16
#define SLIMBUS_PORT_BASE_ADDR 0x100
#define SLIMBUS_PORT_OFFSET 0x8

#define SLIMBUS_PORT_ACTIVE 0x1

void slimbus_utils_init(void __iomem *addr, int32_t src_freq);
void slimbus_utils_deinit(void);
void slimbus_utils_freq_request(void);
void slimbus_utils_freq_release(void);
uint32_t slimbus_utils_port_state_get(const void __iomem *slimbus_base_addr);
uint32_t slimbus_utils_asp_state_get(const void __iomem *asppower_base_addr, uint32_t offset);
void slimbus_utils_module_enable(const struct slimbus_device_info *dev, bool enable);
void slimbus_utils_set_soc_div_freq_disable(bool div_disable);

#endif

