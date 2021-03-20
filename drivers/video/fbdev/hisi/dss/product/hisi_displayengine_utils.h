/* Copyright (c) 2020-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef HISI_DISPLAYENGINE_UTILS_H
#define HISI_DISPLAYENGINE_UTILS_H

#include "hisi_fb.h"

int display_engine_ddic_irc_set(struct hisi_fb_data_type *hisifd,
	display_engine_ddic_irc_param_t *param);

#endif  //HISI_DISPLAYENGINE_UTILS_H
