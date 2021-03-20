/*
 * hi64xx_utils.h
 *
 * hi64xx_utils codec driver
 *
 * Copyright (c) 2014-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __HI64XX_UTILS_H__
#define __HI64XX_UTILS_H__

#include <sound/soc.h>
#include <linux/hisi/hi64xx/hi_cdc_ctrl.h>

struct utils_config {
	/* functions to dump codec registers */
	void (*hi64xx_dump_reg)(char *, unsigned int);
};

struct hi64xx_resmgr;

int hi64xx_update_bits(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int mask, unsigned int value);
int hi64xx_utils_init(struct snd_soc_codec *codec, struct hi_cdc_ctrl *cdc_ctrl,
	const struct utils_config *config, struct hi64xx_resmgr *resmgr,
	unsigned int codec_type);

void hi64xx_dump_debug_info(void);

void hi64xx_utils_deinit(void);

#ifdef CONFIG_HISI_DIEID
int hisi_codec_get_dieid(char *dieid, unsigned int len);
#endif

unsigned int hi64xx_utils_reg_read(unsigned int reg);

#endif /* __HI64XX_UTILS_H__ */

