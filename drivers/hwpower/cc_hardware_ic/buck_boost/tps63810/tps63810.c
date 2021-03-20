/*
 * tps63810.c
 *
 * tps63810 driver
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
 *
 */

#include "tps63810.h"
#include <chipset_common/hwpower/buck_boost.h>
#include <chipset_common/hwpower/power_i2c.h>
#include <chipset_common/hwpower/power_dts.h>
#include <chipset_common/hwpower/power_devices_info.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG tps63810
HWLOG_REGIST();

struct tps63810_device_info *g_tps63810_di;

static int tps63810_write_byte(u8 reg, u8 val)
{
	struct tps63810_device_info *di = g_tps63810_di;
	int i;

	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	for (i = 0; i < TPS_I2C_RETRY_CNT; i++) {
		if (!power_i2c_u8_write_byte(di->client, reg, val))
			return 0;
		usleep_range(5000, 5100); /* sleep 5ms */
	}

	return -EIO;
}

static int tps63810_read_byte(u8 reg, u8 *val)
{
	struct tps63810_device_info *di = g_tps63810_di;
	int i;

	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	for (i = 0; i < TPS_I2C_RETRY_CNT; i++) {
		if (!power_i2c_u8_read_byte(di->client, reg, val))
			return 0;
		usleep_range(5000, 5100); /* sleep 5ms */
	}

	return -EIO;
}

static int tps63810_read_mask(u8 reg, u8 mask, u8 shift, u8 *value)
{
	int ret;
	u8 val;

	ret = tps63810_read_byte(reg, &val);
	if (ret < 0)
		return ret;

	val &= mask;
	val >>= shift;
	*value = val;

	return 0;
}

static int tps63810_set_vout1(unsigned int vout)
{
	u8 vset;

	if ((vout < TPS63810_MIN_VOUT1) || (vout > TPS63810_MAX_VOUT1)) {
		hwlog_err("vout1 out of range [%d, %d]mV\n",
			TPS63810_MIN_VOUT1, TPS63810_MAX_VOUT1);
		return -EINVAL;
	}

	hwlog_info("set_vout1 is %dmV\n", vout);
	vset = (u8)((vout - TPS63810_MIN_VOUT1) / TPS63810_VOUT1_STEP);
	return tps63810_write_byte(TPS63810_VOUT1_ADDR, vset);
}

static int tps63810_set_vout2(unsigned int vout)
{
	u8 vset;

	if ((vout < TPS63810_MIN_VOUT2) || (vout > TPS63810_MAX_VOUT2)) {
		hwlog_err("vout2 out of range [%d, %d]mV\n",
			TPS63810_MIN_VOUT2, TPS63810_MAX_VOUT2);
		return -EINVAL;
	}

	hwlog_info("set_vout2 is %dmV\n", vout);
	vset = (u8)((vout - TPS63810_MIN_VOUT2) / TPS63810_VOUT2_STEP);
	return tps63810_write_byte(TPS63810_VOUT2_ADDR, vset);
}

static int tps63810_set_vout(unsigned int vout)
{
	struct tps63810_device_info *di = g_tps63810_di;

	if (!di) {
		hwlog_err("di is null\n");
		return -EINVAL;
	}

	if (!di->vsel_state)
		return tps63810_set_vout1(vout);

	return tps63810_set_vout2(vout);
}

static bool tps63810_power_good(void)
{
	int ret;
	u8 pg_val = 0;

	/* Bit[0] 0: power good; 1: power not good */
	ret = tps63810_read_mask(TPS63810_STATUS_ADDR, TPS63810_STATUS_PG_MASK,
		TPS63810_STATUS_PG_SHIFT, &pg_val);
	if (ret)
		return false;

	if (!pg_val)
		return true;

	return false;
}

static bool tps63810_device_check(void)
{
	return tps63810_power_good();
}

static void tps63810_parse_dts(struct device_node *np,
	struct tps63810_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"vsel_state", &di->vsel_state, 1);
}

static struct buck_boost_ops tps63810_ops = {
	.device_check = tps63810_device_check,
	.pwr_good = tps63810_power_good,
	.set_vout = tps63810_set_vout,
};

static int tps63810_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct device_node *np = NULL;
	struct tps63810_device_info *di = NULL;
	struct power_devices_info_data *power_dev_info = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_tps63810_di = di;
	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;
	i2c_set_clientdata(client, di);

	tps63810_parse_dts(np, di);
	ret = buck_boost_ops_register(&tps63810_ops);
	if (ret)
		return ret;

	power_dev_info = power_devices_info_register();
	if (power_dev_info) {
		power_dev_info->dev_name = di->dev->driver->name;
		power_dev_info->dev_id = 0;
		power_dev_info->ver_id = 0;
	}
	return 0;
}

static int tps63810_remove(struct i2c_client *client)
{
	struct tps63810_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	i2c_set_clientdata(client, NULL);
	g_tps63810_di = NULL;
	return 0;
}

MODULE_DEVICE_TABLE(i2c, bbst_tps63810);
static const struct of_device_id tps63810_of_match[] = {
	{
		.compatible = "huawei, buckboost_tps63810",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id tps63810_i2c_id[] = {
	{ "bbst_tps63810", 0 }, {}
};

static struct i2c_driver tps63810_driver = {
	.probe = tps63810_probe,
	.remove = tps63810_remove,
	.id_table = tps63810_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "bbst_tps63810",
		.of_match_table = of_match_ptr(tps63810_of_match),
	},
};

static int __init tps63810_init(void)
{
	return i2c_add_driver(&tps63810_driver);
}

static void __exit tps63810_exit(void)
{
	i2c_del_driver(&tps63810_driver);
}

fs_initcall_sync(tps63810_init);
module_exit(tps63810_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("tps63810 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
