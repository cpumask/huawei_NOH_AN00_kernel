/*
 * hisi_usb_phy_chip_efuse.c
 *
 * Hisilicon Hi6502 efuse driver
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
 */
#include <linux/types.h>
s32 get_upc_efuse_value(u32 start_bit, u32 *buf, u32 bit_cnt)
{
	return -1;
}

s32 set_upc_efuse_value(u32 start_bit, u32 *buf, u32 bit_cnt)
{
	return -1;
}
int hisi_usb_phy_chip_get_dieid(char *buf, unsigned int buf_len)
{
	return -1;
}
