/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef HISI_DSS_CLIP_H
#define HISI_DSS_CLIP_H

#include "../hisi_fb.h"

void hisi_dss_post_clip_init(char __iomem *post_clip_base,
	dss_post_clip_t *s_post_clip);
int hisi_dss_post_clip_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer);

#endif /* HISI_DSS_CLIP_H */
