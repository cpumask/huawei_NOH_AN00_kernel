/*
 * hi6405_codec_probe.h -- hi6405 codec driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef _HI6405_CODEC_PROBE_H_
#define _HI6405_CODEC_PROBE_H_

#include <sound/soc.h>

int hi6405_codec_probe(struct snd_soc_codec *codec);
int hi6405_codec_remove(struct snd_soc_codec *codec);

struct snd_soc_codec *hi6405_get_codec(void);

#endif
