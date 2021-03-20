/*
 * adapter_protocol_fcp.c
 *
 * fcp protocol driver
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/delay.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/adapter_protocol.h>
#include <chipset_common/hwpower/adapter_protocol_fcp.h>
#include <chipset_common/hwpower/adapter_protocol_scp.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG fcp_protocol
HWLOG_REGIST();

static struct fcp_protocol_dev *g_fcp_protocol_dev;

static const struct adapter_protocol_device_data g_fcp_protocol_dev_data[] = {
	{ PROTOCOL_DEVICE_ID_FSA9685, "fsa9685" },
	{ PROTOCOL_DEVICE_ID_SCHARGER_V300, "scharger_v300" },
	{ PROTOCOL_DEVICE_ID_SCHARGER_V600, "scharger_v600" },
	{ PROTOCOL_DEVICE_ID_FUSB3601, "fusb3601" },
	{ PROTOCOL_DEVICE_ID_BQ2560X, "bq2560x" },
	{ PROTOCOL_DEVICE_ID_RT9466, "rt9466" },
};

static int fcp_protocol_get_device_id(const char *str)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_fcp_protocol_dev_data); i++) {
		if (!strncmp(str, g_fcp_protocol_dev_data[i].name,
			strlen(str)))
			return g_fcp_protocol_dev_data[i].id;
	}

	return -1;
}

static struct fcp_protocol_dev *fcp_protocol_get_dev(void)
{
	if (!g_fcp_protocol_dev) {
		hwlog_err("g_fcp_protocol_dev is null\n");
		return NULL;
	}

	return g_fcp_protocol_dev;
}

static struct fcp_protocol_ops *fcp_protocol_get_ops(void)
{
	if (!g_fcp_protocol_dev || !g_fcp_protocol_dev->p_ops) {
		hwlog_err("g_fcp_protocol_dev or p_ops is null\n");
		return NULL;
	}

	return g_fcp_protocol_dev->p_ops;
}

int fcp_protocol_ops_register(struct fcp_protocol_ops *ops)
{
	int dev_id;

	if (!g_fcp_protocol_dev || !ops || !ops->chip_name) {
		hwlog_err("g_fcp_protocol_dev or ops or chip_name is null\n");
		return -1;
	}

	dev_id = fcp_protocol_get_device_id(ops->chip_name);
	if (dev_id < 0) {
		hwlog_err("%s ops register fail\n", ops->chip_name);
		return -1;
	}

	g_fcp_protocol_dev->p_ops = ops;
	g_fcp_protocol_dev->dev_id = dev_id;

	hwlog_info("%d:%s ops register ok\n", dev_id, ops->chip_name);
	return 0;
}

static int fcp_protocol_check_trans_num(int num)
{
	/* num must be less equal than 16 */
	if ((num >= BYTE_ONE) && (num <= BYTE_SIXTEEN)) {
		/* num must be 1 or even numbers */
		if ((num > 1) && (num % 2 == 1))
			return -1;

		return 0;
	}

	return -1;
}

static int fcp_protocol_get_rw_error_flag(void)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	return l_dev->info.rw_error_flag;
}

static void fcp_protocol_set_rw_error_flag(int flag)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return;

	hwlog_info("set_rw_error_flag: %d\n", flag);
	l_dev->info.rw_error_flag = flag;
}

static int fcp_protocol_reg_read(int reg, int *val, int num)
{
	int ret;
	struct fcp_protocol_ops *l_ops = NULL;

	if (fcp_protocol_get_rw_error_flag())
		return -1;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->reg_read) {
		hwlog_err("reg_read is null\n");
		return -1;
	}

	if (fcp_protocol_check_trans_num(num)) {
		hwlog_err("invalid num=%d\n", num);
		return -1;
	}

	ret = l_ops->reg_read(reg, val, num);
	if (ret < 0) {
		if (reg != FCP_PROTOCOL_ADP_TYPE1)
			fcp_protocol_set_rw_error_flag(RW_ERROR_FLAG);

		hwlog_err("reg 0x%x read fail\n", reg);
		return -1;
	}

	return 0;
}

static int fcp_protocol_reg_write(int reg, const int *val, int num)
{
	int ret;
	struct fcp_protocol_ops *l_ops = NULL;

	if (fcp_protocol_get_rw_error_flag())
		return -1;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->reg_write) {
		hwlog_err("reg_write is null\n");
		return -1;
	}

	if (fcp_protocol_check_trans_num(num)) {
		hwlog_err("invalid num=%d\n", num);
		return -1;
	}

	ret = l_ops->reg_write(reg, val, num);
	if (ret < 0) {
		if (reg != FCP_PROTOCOL_ADP_TYPE1)
			fcp_protocol_set_rw_error_flag(RW_ERROR_FLAG);

		hwlog_err("reg 0x%x write fail\n", reg);
		return -1;
	}

	return 0;
}

static int fcp_protocol_detect_adapter(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->detect_adapter) {
		hwlog_err("detect_adapter is null\n");
		return -1;
	}

	hwlog_info("detect_adapter\n");

	return l_ops->detect_adapter();
}

static int fcp_protocol_soft_reset_master(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->soft_reset_master) {
		hwlog_err("soft_reset_master is null\n");
		return -1;
	}

	hwlog_info("soft_reset_master\n");

	return l_ops->soft_reset_master();
}

static int fcp_protocol_soft_reset_slave(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->soft_reset_slave) {
		hwlog_err("soft_reset_slave is null\n");
		return -1;
	}

	hwlog_info("soft_reset_slave\n");

	return l_ops->soft_reset_slave();
}

static int fcp_protocol_get_master_status(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->get_master_status)
		return 0;

	hwlog_info("get_master_status\n");

	return l_ops->get_master_status();
}

static int fcp_protocol_stop_charging_config(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->stop_charging_config)
		return 0;

	hwlog_info("stop_charging_config\n");

	return l_ops->stop_charging_config();
}

static bool fcp_protocol_is_accp_charger_type(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return false;

	if (!l_ops->is_accp_charger_type)
		return false;

	hwlog_info("is_accp_charger_type\n");

	return (bool)l_ops->is_accp_charger_type();
}

#ifdef POWER_MODULE_DEBUG_FUNCTION
static int fcp_protocol_process_pre_init(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->pre_init)
		return 0;

	hwlog_info("process_pre_init\n");

	return l_ops->pre_init();
}

static int fcp_protocol_process_post_init(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->post_init)
		return 0;

	hwlog_info("process_post_init\n");

	return l_ops->post_init();
}
#endif /* POWER_MODULE_DEBUG_FUNCTION */

static int fcp_protocol_process_pre_exit(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->pre_exit)
		return 0;

	hwlog_info("process_pre_exit\n");

	return l_ops->pre_exit();
}

static int fcp_protocol_process_post_exit(void)
{
	struct fcp_protocol_ops *l_ops = NULL;

	l_ops = fcp_protocol_get_ops();
	if (!l_ops)
		return -1;

	if (!l_ops->post_exit)
		return 0;

	hwlog_info("process_post_exit\n");

	return l_ops->post_exit();
}

static int fcp_protocol_set_default_param(void)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	memset(&l_dev->info, 0, sizeof(l_dev->info));

	return 0;
}

static int fcp_protocol_get_vendor_id(int *id)
{
	int value[BYTE_ONE] = { 0 };
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev || !id)
		return -1;

	if (l_dev->info.vid_rd_flag == HAS_READ_FLAG) {
		*id = l_dev->info.vid;
		hwlog_info("get_vendor_id_a: 0x%x\n", *id);
		return 0;
	}

	/* reg: 0x04 */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_ID_OUT0,
		value, BYTE_ONE))
		return -1;

	*id = value[0];
	l_dev->info.vid_rd_flag = HAS_READ_FLAG;
	l_dev->info.vid = value[0];

	hwlog_info("get_vendor_id_f: 0x%x\n", *id);
	return 0;
}

static int fcp_protocol_get_voltage_capabilities(int *cap)
{
	int value[BYTE_ONE] = { 0 };
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	if (l_dev->info.volt_cap_rd_flag == HAS_READ_FLAG) {
		*cap = l_dev->info.volt_cap;
		hwlog_info("get_voltage_capabilities_a: %d\n", *cap);
		return 0;
	}

	/* reg: 0x21 */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_DISCRETE_CAPABILOTIES0,
		value, BYTE_ONE))
		return -1;

	/* only support three out voltage cap(5v 9v 12v) */
	if (value[0] > FCP_PROTOCOL_CAPABILOTIES_5V_9V_12V) {
		hwlog_err("invalid voltage_capabilities=%d\n", value[0]);
		return -1;
	}

	*cap = value[0];
	l_dev->info.volt_cap_rd_flag = HAS_READ_FLAG;
	l_dev->info.volt_cap = value[0];

	hwlog_info("get_voltage_capabilities_f: %d\n", *cap);
	return 0;
}

static int fcp_protocol_get_max_voltage(int *volt)
{
	struct fcp_protocol_dev *l_dev = NULL;
	int value[BYTE_ONE] = { 0 };
	int cap;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev || !volt)
		return -1;

	if (l_dev->info.max_volt_rd_flag == HAS_READ_FLAG) {
		*volt = l_dev->info.max_volt;
		hwlog_info("get_max_voltage_a: %d\n", *volt);
		return 0;
	}

	if (fcp_protocol_get_voltage_capabilities(&cap))
		return -1;

	/* reg: 0x3n */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_OUTPUT_V_REG(cap),
		value, BYTE_ONE))
		return -1;

	*volt = (value[0] * FCP_PROTOCOL_OUTPUT_V_STEP);
	l_dev->info.max_volt_rd_flag = HAS_READ_FLAG;
	l_dev->info.max_volt = *volt;

	hwlog_info("get_max_voltage_f: %d\n", *volt);
	return 0;
}

static int fcp_protocol_get_max_power(int *power)
{
	int value[BYTE_ONE] = { 0 };
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	if (l_dev->info.max_pwr_rd_flag == HAS_READ_FLAG) {
		*power = l_dev->info.max_pwr;
		hwlog_info("get_max_power_a: %d\n", *power);
		return 0;
	}

	/* reg: 0x22 */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_MAX_PWR, value, BYTE_ONE))
		return -1;

	*power = (value[0] * FCP_PROTOCOL_MAX_PWR_STEP);
	l_dev->info.max_pwr_rd_flag = HAS_READ_FLAG;
	l_dev->info.max_pwr = *power;

	hwlog_info("get_max_power_f: %d\n", *power);
	return 0;
}

static int fcp_protocol_detect_adapter_support_mode_by_0x80(void)
{
	int value[BYTE_ONE] = { 0 };

	/* reg: 0x80 */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_ADP_TYPE1, value, BYTE_ONE)) {
		hwlog_err("read adp_type1(0x80) fail\n");
		return FCP_DETECT_SUCC;
	}

	hwlog_info("adp_type1[%x]=%x\n", FCP_PROTOCOL_ADP_TYPE1, value[0]);
	return FCP_DETECT_OTHER;
}

static int fcp_protocol_detect_adapter_support_mode(int *mode)
{
	int ret;
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev || !mode)
		return ADAPTER_DETECT_OTHER;

	/* set all parameter to default state */
	fcp_protocol_set_default_param();

	l_dev->info.support_mode = ADAPTER_SUPPORT_UNDEFINED;

	/*
	 * if 0x80 has read failed(-1) when detect scp adapter,
	 * we think it is a fcp adapter at this case.
	 * in order to reduce the detection time of fcp adapter.
	 */
	if (scp_protocol_get_reg80_rw_error_flag()) {
		hwlog_info("no need continue, reg80 already read fail\n");
		goto end_detect;
	}

	/* protocol handshark: detect fcp adapter */
	ret = fcp_protocol_detect_adapter();
	if (ret == FCP_DETECT_OTHER) {
		hwlog_err("fcp adapter detect other\n");
		return ADAPTER_DETECT_OTHER;
	}
	if (ret == FCP_DETECT_FAIL) {
		hwlog_err("fcp adapter detect fail\n");
		return ADAPTER_DETECT_FAIL;
	}

#ifdef CONFIG_DIRECT_CHARGER
	/*
	 * if the product does not support scp,
	 * we think it is a fcp adapter at this case.
	 */
	if (scp_protocol_get_protocol_register_state()) {
		hwlog_info("no need continue, scp protocol not support\n");
		goto end_detect;
	}

	/* adapter type detect: 0x80 */
	ret = fcp_protocol_detect_adapter_support_mode_by_0x80();
	if (ret == FCP_DETECT_OTHER) {
		hwlog_info("fcp adapter type_b detect other(judge by 0x80)\n");
		return ADAPTER_DETECT_OTHER;
	}
#endif /* CONFIG_DIRECT_CHARGER */

end_detect:
	*mode = ADAPTER_SUPPORT_FCP;
	l_dev->info.support_mode = ADAPTER_SUPPORT_FCP;
	hwlog_info("detect_adapter_type success\n");
	return ADAPTER_DETECT_SUCC;
}

static int fcp_protocol_get_support_mode(int *mode)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev || !mode)
		return -1;

	*mode = l_dev->info.support_mode;

	hwlog_info("get_support_mode\n");
	return 0;
}

static int fcp_protocol_get_device_info(struct adapter_device_info *info)
{
	if (!info)
		return -1;

	if (fcp_protocol_get_vendor_id(&info->vendor_id))
		return -1;

	if (fcp_protocol_get_max_voltage(&info->max_volt))
		return -1;

	if (fcp_protocol_get_voltage_capabilities(&info->volt_cap))
		return -1;

	if (fcp_protocol_get_max_power(&info->max_pwr))
		return -1;

	hwlog_info("get_device_info\n");
	return 0;
}

static int fcp_protocol_set_output_voltage(int volt)
{
	int value1[BYTE_ONE] = { 0 };
	int value2[BYTE_ONE] = { 0 };
	int value3[BYTE_ONE] = { 0 };
	int vendor_id = 0;
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	/* read vid for identify huawei adapter */
	if (fcp_protocol_get_vendor_id(&vendor_id))
		return -1;

	value1[0] = (volt / FCP_PROTOCOL_VOUT_STEP);
	value3[0] = FCP_PROTOCOL_VOUT_CONFIG_ENABLE;

	/* reg: 0x2c */
	if (fcp_protocol_reg_write(FCP_PROTOCOL_VOUT_CONFIG, value1, BYTE_ONE))
		return -1;

	/* reg: 0x2c */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_VOUT_CONFIG, value2, BYTE_ONE))
		return -1;

	/* bq2560x & rt9466 should ignore this case */
	if ((l_dev->dev_id != PROTOCOL_DEVICE_ID_BQ2560X) &&
		(l_dev->dev_id != PROTOCOL_DEVICE_ID_RT9466)) {
		if (value1[0] != value2[0]) {
			hwlog_err("output voltage config fail, reg[0x%x]=%d\n",
				FCP_PROTOCOL_VOUT_CONFIG, value2[0]);
			return -1;
		}
	}

	/* reg: 0x2b */
	if (fcp_protocol_reg_write(FCP_PROTOCOL_OUTPUT_CONTROL,
		value3, BYTE_ONE))
		return -1;

	hwlog_info("set_output_voltage: %d\n", volt);
	return 0;
}

static int fcp_protocol_get_output_current(int *cur)
{
	int volt = 1;
	int max_power = 0;

	if (!cur)
		return -1;

	if (fcp_protocol_get_max_voltage(&volt))
		return -1;

	if (fcp_protocol_get_max_power(&max_power))
		return -1;

	if (volt == 0) {
		hwlog_err("volt is zero\n");
		return -1;
	}

	/* mw = 1000ma * mv */
	*cur = (max_power / volt) * 1000;

	hwlog_info("get_output_current: %d\n", *cur);
	return 0;
}

static int fcp_protocol_get_slave_status(void)
{
	int value[BYTE_ONE] = { 0 };

	/* reg: 0x28 */
	if (fcp_protocol_reg_read(FCP_PROTOCOL_ADAPTER_STATUS, value, BYTE_ONE))
		return -1;

	hwlog_info("get_slave_status: %d\n", value[0]);

	if ((value[0] & FCP_PROTOCOL_UVP_MASK) == FCP_PROTOCOL_UVP_MASK)
		return ADAPTER_OUTPUT_UVP;

	if ((value[0] & FCP_PROTOCOL_OVP_MASK) == FCP_PROTOCOL_OVP_MASK)
		return ADAPTER_OUTPUT_OVP;

	if ((value[0] & FCP_PROTOCOL_OCP_MASK) == FCP_PROTOCOL_OCP_MASK)
		return ADAPTER_OUTPUT_OCP;

	if ((value[0] & FCP_PROTOCOL_OTP_MASK) == FCP_PROTOCOL_OTP_MASK)
		return ADAPTER_OUTPUT_OTP;

	return ADAPTER_OUTPUT_NORMAL;
}

static int fcp_protocol_set_default_state(void)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	/* process non protocol flow */
	if (fcp_protocol_process_pre_exit())
		return -1;

	/* process non protocol flow */
	if (fcp_protocol_process_post_exit())
		return -1;

	hwlog_info("set_default_state ok\n");
	return 0;
}

static int fcp_protocol_get_protocol_register_state(void)
{
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = fcp_protocol_get_dev();
	if (!l_dev)
		return -1;

	if ((l_dev->dev_id >= PROTOCOL_DEVICE_ID_BEGIN) &&
		(l_dev->dev_id < PROTOCOL_DEVICE_ID_END))
		return 0;

	hwlog_info("get_protocol_register_state fail\n");
	return -1;
}

static struct adapter_protocol_ops adapter_protocol_fcp_ops = {
	.type_name = "hw_fcp",
	.soft_reset_master = fcp_protocol_soft_reset_master,
	.soft_reset_slave = fcp_protocol_soft_reset_slave,
	.get_master_status = fcp_protocol_get_master_status,
	.stop_charging_config = fcp_protocol_stop_charging_config,
	.detect_adapter_support_mode = fcp_protocol_detect_adapter_support_mode,
	.get_support_mode = fcp_protocol_get_support_mode,
	.get_device_info = fcp_protocol_get_device_info,
	.set_output_voltage = fcp_protocol_set_output_voltage,
	.get_output_current = fcp_protocol_get_output_current,
	.get_max_voltage = fcp_protocol_get_max_voltage,
	.get_slave_status = fcp_protocol_get_slave_status,
	.set_default_state = fcp_protocol_set_default_state,
	.set_default_param = fcp_protocol_set_default_param,
	.get_protocol_register_state = fcp_protocol_get_protocol_register_state,
	.is_accp_charger_type = fcp_protocol_is_accp_charger_type,
};

static int __init fcp_protocol_init(void)
{
	int ret;
	struct fcp_protocol_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_fcp_protocol_dev = l_dev;
	l_dev->dev_id = PROTOCOL_DEVICE_ID_END;

	ret = adapter_protocol_ops_register(&adapter_protocol_fcp_ops);
	if (ret)
		goto fail_register_ops;

	return 0;

fail_register_ops:
	kfree(l_dev);
	g_fcp_protocol_dev = NULL;
	return ret;
}

static void __exit fcp_protocol_exit(void)
{
	if (!g_fcp_protocol_dev)
		return;

	kfree(g_fcp_protocol_dev);
	g_fcp_protocol_dev = NULL;
}

subsys_initcall_sync(fcp_protocol_init);
module_exit(fcp_protocol_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fcp protocol driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
