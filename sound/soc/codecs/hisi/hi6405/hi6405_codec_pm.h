/*
 * hi6405_codec_pm.h
 *
 * expose route pm api for hi6405 driver
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
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

#ifndef _HI6405_CODEC_PM_H_
#define _HI6405_CODEC_PM_H_

#include <sound/soc.h>
#include <linux/device.h>

void hi6405_codec_pm_init(struct snd_soc_codec *codec);
int hi6405_codec_suspend(struct device *dev);
int hi6405_codec_resume(struct device *dev);
int hi6405_codec_freeze(struct device *dev);
int hi6405_codec_restore(struct device *dev);

#endif
