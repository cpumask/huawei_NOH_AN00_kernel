/*
 * direct_charger_debug.h
 *
 * debug for direct charger module
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

#ifndef _DIRECT_CHARGER_DEBUG_H_
#define _DIRECT_CHARGER_DEBUG_H_

#include <huawei_platform/power/direct_charger/direct_charger.h>

void sc_dbg_register(struct direct_charge_device *di);
void lvc_dbg_register(struct direct_charge_device *di);

#endif /* _DIRECT_CHARGER_DEBUG_H_ */
