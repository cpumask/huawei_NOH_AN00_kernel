/*
 * lcd_kit_sysfs_mtk.c
 *
 * lcdkit sysfs function head file for lcd driver mtk platform
 *
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
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

#ifndef __LCD_KIT_SYSFS_HS_H_
#define __LCD_KIT_SYSFS_HS_H_
/* oem info */
#define OEM_INFO_SIZE_MAX 500
#define OEM_INFO_BLOCK_NUM 1
/* 2d barcode */
#define BARCODE_LENGTH    46
#define BARCODE_BLOCK_NUM 3
#define BARCODE_BLOCK_LEN 16
/* project id */
#define PROJECT_ID_LENGTH 10

#define INVALID_TYPE (-1)
#define DATA_INDEX 1

#define ORDER_DELAY 200

enum oem_type {
	PROJECT_ID_TYPE,
	BARCODE_2D_TYPE,
	BRIGHTNESS_TYPE,
};

/* oem info cmd */
struct oem_info_cmd {
	unsigned char type;
	int (*func)(char *oem_data);
};

/* oem info */
static int oem_info_type = INVALID_TYPE;
static int lcd_get_2d_barcode(char *oem_data);
static int lcd_get_project_id(char *oem_data);

static struct oem_info_cmd oem_read_cmds[] = {
	{ PROJECT_ID_TYPE, lcd_get_project_id },
	{ BARCODE_2D_TYPE, lcd_get_2d_barcode }
};

static struct oem_info_cmd oem_write_cmds[] = {
};

#endif
