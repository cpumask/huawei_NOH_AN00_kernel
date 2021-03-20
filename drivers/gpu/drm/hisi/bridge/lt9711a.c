/*
 * lt9711a.c
 *
 * operate function for lt9711a
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

#include "edp_bridge.h"
#include "lt9711a.h"

#define MAX_CHIP_PROBE_TIMES 3

#define I2C_SLAVE_ADDR     0x41
#define I2C_CONTROLLER     4
#define I2C_TRANSFER_LEN   2
#define EDID_HEADER_LEN    8

static const u8 edid_header[EDID_HEADER_LEN] = {0x00, 0xff, 0xff, 0xff,
			   0xff, 0xff, 0xff, 0x00};

int is_lt9711a_onboard(struct mipi2edp *pdata)
{
	int i, ret, reg, chipid;

	HISI_DRM_INFO("check whether lt9711a chip is onboard\n");
	ret = 0;
	reg = 0;
	chipid = 0;
	for (i = 0; i < MAX_CHIP_PROBE_TIMES; i++) {
		ret = regmap_read(pdata->regmap, CHIP_ID_REG_0, &reg);
		if (ret)
			continue;
		else
			chipid = (reg&0xFF) << 8;

		ret = regmap_read(pdata->regmap, CHIP_ID_REG_1, &reg);
		if (ret)
			continue;
		else
			chipid |= (reg&0xFF);

		HISI_DRM_INFO("lt9711a is onboard,chipid is 0x%x\n", chipid);
		return ret;
	}

	HISI_DRM_ERR("lt9711a isn't onboard!\n");
	return ret;
}

static int lt9711a_i2c_send_byte(char devaddr, const char regoffset, const char value)
{
	struct i2c_adapter *adap;
	int ret;
	char data[I2C_TRANSFER_LEN];
	struct i2c_msg msg[1];
	adap = i2c_get_adapter(I2C_CONTROLLER);
	if (!adap) {
		HISI_DRM_ERR("lt9711a_i2c_send_byte i2c_get_adapter err");
		return -ENODEV;
	}

	msg->addr = devaddr;
	msg->flags = 0;
	msg->len = I2C_TRANSFER_LEN;
	msg->buf = data;
	data[0] = regoffset;
	data[1] = value;

	ret = i2c_transfer(adap, msg, 1);
	HISI_DRM_DEBUG("lt9711a_i2c_send_byte regoffset = %d value=%d ret = %d \n", regoffset, value, ret);
	i2c_put_adapter(adap);
	if (ret == 1) {
		return 0;
	}
	return ret;
}

static int lt9711a_i2c_recv_byte(char devaddr, const char regoffset, char *value)
{
	struct i2c_adapter *adap;
	int ret;
	struct i2c_msg msg[2];

	adap = i2c_get_adapter(I2C_CONTROLLER);
	if (!adap) {
		HISI_DRM_ERR("lt9711a_i2c_recv_byte i2c_get_adapter err");
		return -ENODEV;
	}

	msg[0].addr = devaddr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &regoffset;

	msg[1].addr = devaddr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = value;
	ret = i2c_transfer(adap, msg, 2);
	HISI_DRM_DEBUG("lt9711a_i2c_send_byte regoffset=%d value=%d ret = %d \n", regoffset,value[0],ret);
	i2c_put_adapter(adap);
	if (ret == 2) {
		return 0;
	}
	return ret;
}

int lt9711a_get_edid(char *edid, unsigned int len)
{
	char ret[2];
	int res;
	int top = 0;
	int i = 0;
	int j = 0;
	int base_address = 0;
	int data_register = 0xd8;
	int current_data = 0;

	if (!edid) {
		HISI_DRM_ERR("edid input err");
		return -1;
	}

	if (len < 128) {
		HISI_DRM_ERR("len input err");
		return -1;
	}

	res = lt9711a_i2c_recv_byte(I2C_SLAVE_ADDR, 0xd6, ret);
	if(res || ((ret[0] != 238) && (ret[0] != 17))) {
		HISI_DRM_ERR("lt9711a_i2c_transfer err");
		return -1;
	}

	res = lt9711a_i2c_send_byte(I2C_SLAVE_ADDR, 0xe0, 0x20);
	if (res) {
		HISI_DRM_ERR("lt9711a_i2c_transfer err");
		return -1;
	}

	for(i = 0; i < 16; i++) {
		res = lt9711a_i2c_send_byte(I2C_SLAVE_ADDR, 0xe1, base_address);
		if (res) {
			HISI_DRM_ERR("lt9711a_i2c_transfer err");
			return -1;
		}
		for(j = 0; j < 8; j++) {
			current_data = data_register + j;
			res = lt9711a_i2c_recv_byte(I2C_SLAVE_ADDR, current_data, ret);
			if (res) {
				HISI_DRM_ERR("lt9711a_i2c_transfer err");
				return -1;
			}
			edid[top++] = ret[0];
		}
		base_address = base_address + 8;
	}
	if (top != len) {
		HISI_DRM_ERR("edid len err");
		return -1;
	}
	res = lt9711a_i2c_send_byte(I2C_SLAVE_ADDR, 0xe0, 0x40);
	if (res) {
		HISI_DRM_WARN("lt9711a_i2c_transfer warn");
	}
	res = lt9711a_i2c_send_byte(I2C_SLAVE_ADDR, 0xe1, 0x00);
	if (res) {
		HISI_DRM_WARN("lt9711a_i2c_transfer warn");
	}
	return 0;
}

int check_edid(char *edid, unsigned int len)
{
	int i;
	if (!edid) {
		HISI_DRM_ERR("Edid The pointer is NULL.\n");
		return -1;
	}

	if (len < 128) {
		HISI_DRM_ERR("len input err");
		return -1;
	}

	/*Verify Header*/
	for (i = 0; i < EDID_HEADER_LEN; i++) {
		if (edid[i] != edid_header[i]) {
			HISI_DRM_ERR("Don't match EDID header\n");
			return -1;
		}
	}
	return 0;
}

