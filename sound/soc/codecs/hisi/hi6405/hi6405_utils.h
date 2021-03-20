/*
 * hi6405_utils.h -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef _HI6405_UTILS_H_
#define _HI6405_UTILS_H_

#include <sound/soc.h>

unsigned int hi6405_reg_read(struct snd_soc_codec *codec,
	unsigned int reg);
int hi6405_reg_write(struct snd_soc_codec *codec,
	unsigned int reg, unsigned int value);

#endif

