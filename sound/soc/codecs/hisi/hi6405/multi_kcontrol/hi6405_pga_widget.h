/*
 * hi6405_pga_widget.h -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef _HI6405_PGA_WIDGET_H_
#define _HI6405_PGA_WIDGET_H_

#include "linux/hisi/hi64xx/hi6405_type.h"

int hi6405_add_pga_widgets(struct snd_soc_codec *codec, bool single_kcontrol);

#endif
