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

#ifndef HISI_DSS_DUMP_H
#define HISI_DSS_DUMP_H

#include "../hisi_fb.h"

#define DUMP_BUF_SIZE SZ_256K

struct dss_dump_data_type {
	char *dss_buf;
	uint32_t dss_buf_len;
	char dss_filename[256];

	char *scene_buf;  /* unused currently */
	uint32_t scene_buf_len;
	char scene_filename[256];

	char image_bin_filename[OVL_LAYER_NUM_MAX][256];
};

struct dss_dump_data_type *hisifb_alloc_dumpdss(void);
void hisifb_free_dumpdss(struct hisi_fb_data_type *hisifd);
void dump_dss_overlay(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisi_dss_debug_func(struct work_struct *work);


#endif /* HISI_DSS_DUMP_H */
