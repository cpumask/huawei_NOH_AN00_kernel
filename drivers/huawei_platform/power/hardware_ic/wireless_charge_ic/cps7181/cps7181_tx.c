/*
 * cps7181_tx.c
 *
 * cps7181 tx driver
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

#include "cps7181.h"

#define HWLOG_TAG wireless_cps7181_tx
HWLOG_REGIST();

static bool g_cps7181_tx_open_flag;

static const char * const g_cps7181_tx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "tx_start_ping",
	[1]  = "tx_ss_pkt_rcvd",
	[2]  = "tx_id_pkt_rcvd",
	[3]  = "tx_cfg_pkt_rcvd",
	[4]  = "tx_pp_pkt_rcvd",
	[5]  = "tx_ept",
	[6]  = "tx_rpp_timeout",
	[7]  = "tx_cep_timeout",
	[8]  = "tx_ac_detect",
	[9]  = "tx_tx_init",
	[11] = "tx_rpp_type_err",
};

static const char * const g_cps7181_tx_ept_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "tx_ept_src_wrong_pkt",
	[1]  = "tx_ept_src_ac_detect",
	[2]  = "tx_ept_src_ss",
	[3]  = "tx_ept_src_rx_ept",
	[4]  = "tx_ept_src_cep_timeout",
	[5]  = "tx_ept_src_rpp_timeout",
	[6]  = "tx_ept_src_ocp",
	[7]  = "tx_ept_src_ovp",
	[8]  = "tx_ept_src_uvp",
	[9]  = "tx_ept_src_fod",
	[10] = "tx_ept_src_otp",
	[11] = "tx_ept_src_ping_ocp",
};

static bool cps7181_tx_is_tx_mode(void)
{
	int ret;
	u8 mode = 0;

	ret = cps7181_read_byte(CPS7181_OP_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_tx_mode: get op_mode failed\n");
		return false;
	}

	if (mode == CPS7181_OP_MODE_TX)
		return true;
	else
		return false;
}

static bool cps7181_tx_is_rx_mode(void)
{
	int ret;
	u8 mode = 0;

	ret = cps7181_read_byte(CPS7181_OP_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_rx_mode: get rx mode failed\n");
		return false;
	}

	return mode == CPS7181_OP_MODE_RX;
}

static void cps7181_tx_set_tx_open_flag(bool enable)
{
	g_cps7181_tx_open_flag = enable;
}

static int cps7181_tx_chip_reset(void)
{
	int ret;

	ret = cps7181_fw_sram_i2c_init(CPS7181_BYTE_INC);
	if (ret) {
		hwlog_err("chip_reset: i2c init failed\n");
		return ret;
	}
	ret = cps7181_write_byte_mask(CPS7181_TX_CMD_ADDR,
		CPS7181_TX_CMD_SYS_RST, CPS7181_TX_CMD_SYS_RST_SHIFT,
		CPS7181_TX_CMD_VAL);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		return ret;
	}
	usleep_range(9500, 10500); /* delay 10ms for sys-reset */
	ret = cps7181_fw_sram_i2c_init(CPS7181_BYTE_INC);
	if (ret) {
		hwlog_err("chip_reset: i2c init failed\n");
		return ret;
	}

	hwlog_info("[chip_reset] succ\n");
	return 0;
}

static int cps7181_tx_mode_vset(int tx_vset)
{
	int ret;
	u8 gpio_val;

	if (tx_vset == CPS7181_TX_PS_VOLT_5V5)
		gpio_val = CPS7181_TX_PS_GPIO_PU;
	else if ((tx_vset == CPS7181_TX_PS_VOLT_6V8) ||
		(tx_vset == CPS7181_TX_PS_VOLT_6V))
		gpio_val = CPS7181_TX_PS_GPIO_OPEN;
	else if ((tx_vset == CPS7181_TX_PS_VOLT_10V) ||
		(tx_vset == CPS7181_TX_PS_VOLT_6V9))
		gpio_val = CPS7181_TX_PS_GPIO_PD;
	else
		return -WLC_ERR_PARA_WRONG;

	ret = cps7181_write_byte(CPS7181_TX_PS_CTRL_ADDR, gpio_val);
	if (ret) {
		hwlog_err("tx_mode_vset: write failed\n");
		return ret;
	}

	return 0;
}

static bool cps7181_tx_check_rx_disconnect(void)
{
	struct cps7181_dev_info *di = NULL;

	cps7181_get_dev_info(&di);
	if (!di) {
		hwlog_err("check_rx_disconnect: di null\n");
		return true;
	}

	if (di->ept_type & CPS7181_TX_EPT_SRC_CEP_TIMEOUT) {
		di->ept_type &= ~CPS7181_TX_EPT_SRC_CEP_TIMEOUT;
		hwlog_info("[check_rx_disconnect] rx disconnect\n");
		return true;
	}

	return false;
}

static int cps7181_tx_get_ping_interval(u16 *ping_interval)
{
	int ret;

	if (!ping_interval) {
		hwlog_err("get_ping_interval: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_PING_INTERVAL_ADDR, ping_interval);
	if (ret) {
		hwlog_err("get_ping_interval: read failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_set_ping_interval(u16 ping_interval)
{
	int ret;

	if ((ping_interval < CPS7181_TX_PING_INTERVAL_MIN) ||
		(ping_interval > CPS7181_TX_PING_INTERVAL_MAX)) {
		hwlog_err("set_ping_interval: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps7181_write_word(CPS7181_TX_PING_INTERVAL_ADDR, ping_interval);
	if (ret) {
		hwlog_err("set_ping_interval: write failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_ping_frequency(u16 *ping_freq)
{
	int ret;
	u8 data = 0;

	if (!ping_freq) {
		hwlog_err("get_ping_frequency: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_byte(CPS7181_TX_PING_FREQ_ADDR, &data);
	if (ret) {
		hwlog_err("get_ping_frequency: read failed\n");
		return ret;
	}
	*ping_freq = (u16)data;

	return 0;
}

static int cps7181_tx_set_ping_frequency(u16 ping_freq)
{
	int ret;

	if ((ping_freq < CPS7181_TX_PING_FREQ_MIN) ||
		(ping_freq > CPS7181_TX_PING_FREQ_MAX)) {
		hwlog_err("set_ping_frequency: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps7181_write_byte(CPS7181_TX_PING_FREQ_ADDR, (u8)ping_freq);
	if (ret) {
		hwlog_err("set_ping_frequency: write failed\n");
		return ret;
	}

	return ret;
}

static int cps7181_tx_get_min_fop(u16 *fop)
{
	int ret;
	u8 data = 0;

	if (!fop) {
		hwlog_err("get_min_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_byte(CPS7181_TX_MIN_FOP_ADDR, &data);
	if (ret) {
		hwlog_err("get_min_fop: read failed\n");
		return ret;
	}
	*fop = (u16)data;

	return 0;
}

static int cps7181_tx_set_min_fop(u16 fop)
{
	int ret;

	if ((fop < CPS7181_TX_MIN_FOP) || (fop > CPS7181_TX_MAX_FOP)) {
		hwlog_err("set_min_fop: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps7181_write_byte(CPS7181_TX_MIN_FOP_ADDR, (u8)fop);
	if (ret) {
		hwlog_err("set_min_fop: write failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_max_fop(u16 *fop)
{
	int ret;
	u8 data = 0;

	if (!fop) {
		hwlog_err("get_max_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_byte(CPS7181_TX_MAX_FOP_ADDR, &data);
	if (ret) {
		hwlog_err("get_max_fop: read failed\n");
		return ret;
	}
	*fop = (u16)data;

	return 0;
}

static int cps7181_tx_set_max_fop(u16 fop)
{
	int ret;

	if ((fop < CPS7181_TX_MIN_FOP) || (fop > CPS7181_TX_MAX_FOP)) {
		hwlog_err("set_max_fop: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps7181_write_byte(CPS7181_TX_MAX_FOP_ADDR, (u8)fop);
	if (ret) {
		hwlog_err("set_max_fop: write failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_fop(u16 *fop)
{
	int ret;

	if (!fop) {
		hwlog_err("get_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_OP_FREQ_ADDR, fop);
	if (ret) {
		hwlog_err("get_fop: failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_temp(u8 *chip_temp)
{
	int ret;
	u16 temp = 0;

	if (!chip_temp) {
		hwlog_err("get_temp: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_CHIP_TEMP_ADDR, &temp);
	if (ret) {
		hwlog_err("get_temp: read failed\n");
		return ret;
	}

	*chip_temp = (u8)temp;

	return 0;
}

static int cps7181_tx_get_vin(u16 *tx_vin)
{
	int ret;

	if (!tx_vin) {
		hwlog_err("get_vin: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_VIN_ADDR, tx_vin);
	if (ret) {
		hwlog_err("get_vin: read failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_vrect(u16 *tx_vrect)
{
	int ret;

	if (!tx_vrect) {
		hwlog_err("get_vrect: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_VRECT_ADDR, tx_vrect);
	if (ret) {
		hwlog_err("get_vrect: read failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_get_iin(u16 *tx_iin)
{
	int ret;

	if (!tx_iin) {
		hwlog_err("get_iin: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_IIN_ADDR, tx_iin);
	if (ret) {
		hwlog_err("get_iin: read failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_set_ilimit(int tx_ilim)
{
	int ret;

	if ((tx_ilim < CPS7181_TX_ILIM_MIN) ||
		(tx_ilim > CPS7181_TX_ILIM_MAX)) {
		hwlog_err("set_ilimit: out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps7181_write_word(CPS7181_TX_ILIM_ADDR, (u16)tx_ilim);
	if (ret) {
		hwlog_err("set_ilimit: failed\n");
		return ret;
	}

	return 0;
}

static int cps7181_tx_set_fod_coef(u32 pl_th, u8 pl_cnt)
{
	int ret;

	/* tx ploss threshold 0:disabled */
	ret = cps7181_write_word(CPS7181_TX_PLOSS_V0_TH, (u16)pl_th);
	ret += cps7181_write_word(CPS7181_TX_PLOSS_V1_TH, (u16)pl_th);
	ret += cps7181_write_word(CPS7181_TX_PLOSS_V2_TH, (u16)pl_th);
	/* tx ploss fod debounce count 0:no debounce */
	ret += cps7181_write_byte(CPS7181_TX_PLOSS_CNT_ADDR, pl_cnt);
	if (ret)
		hwlog_err("tx set fod coef failed\n");

	return ret;
}

static void cps7181_tx_set_rp_dm_timeout_val(u8 val)
{
}

static int cps7181_tx_stop_config(void)
{
	return 0;
}

static int cps7181_tx_chip_init(enum wltx_open_type type)
{
	int ret = 0;
	struct cps7181_dev_info *di = NULL;

	cps7181_get_dev_info(&di);
	if (!di) {
		hwlog_err("chip_init: di null\n");
		return -WLC_ERR_PARA_NULL;
	}

	di->irq_cnt = 0;
	di->g_val.irq_abnormal_flag = false;
	cps7181_enable_irq();

	ret += cps7181_write_byte_mask(CPS7181_TX_IRQ_EN_ADDR,
		CPS7181_TX_IRQ_EN_RPP_TIMEOUT, 6, 0);
	ret += cps7181_write_word(CPS7181_TX_OCP_TH_ADDR, CPS7181_TX_OCP_TH);
	ret += cps7181_write_word(CPS7181_TX_OVP_TH_ADDR, CPS7181_TX_OVP_TH);
	ret += cps7181_tx_set_ping_frequency(CPS7181_TX_PING_FREQ);
	ret += cps7181_tx_set_min_fop(CPS7181_TX_MIN_FOP);
	ret += cps7181_tx_set_max_fop(CPS7181_TX_MAX_FOP);
	ret += cps7181_tx_set_ping_interval(CPS7181_TX_PING_INTERVAL);
	if (ret) {
		hwlog_err("chip_init: write failed\n");
		return -WLC_ERR_I2C_W;
	}

	return 0;
}

static int cps7181_tx_enable_tx_mode(bool enable)
{
	int ret;

	if (enable)
		ret = cps7181_write_byte_mask(CPS7181_TX_CMD_ADDR,
			CPS7181_TX_CMD_EN_TX, CPS7181_TX_CMD_EN_TX_SHIFT,
			CPS7181_TX_CMD_VAL);
	else
		ret = cps7181_write_byte_mask(CPS7181_TX_CMD_ADDR,
			CPS7181_TX_CMD_DIS_TX, CPS7181_TX_CMD_DIS_TX_SHIFT,
			CPS7181_TX_CMD_VAL);

	if (ret) {
		hwlog_err("%s tx_mode failed\n", enable ? "enable" : "disable");
		return ret;
	}

	return 0;
}

static void cps7181_tx_show_ept_type(u16 ept)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_cps7181_tx_ept_name); i++) {
		if (ept & BIT(i))
			hwlog_info("[tx_ept] %s\n", g_cps7181_tx_ept_name[i]);
	}
}

static int cps7181_tx_get_ept_type(u16 *ept)
{
	int ret;
	u16 ept_value = 0;

	if (!ept) {
		hwlog_err("get_ept_type: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps7181_read_word(CPS7181_TX_EPT_SRC_ADDR, &ept_value);
	if (ret) {
		hwlog_err("get_ept_type: read failed\n");
		return ret;
	}
	*ept = ept_value;
	hwlog_info("[get_ept_type] type=0x%02x", *ept);
	cps7181_tx_show_ept_type(*ept);

	return 0;
}

static int cps7181_tx_clear_ept_src(void)
{
	int ret;

	ret = cps7181_write_word(CPS7181_TX_EPT_SRC_ADDR, CPS7181_TX_EPT_SRC_CLEAR);
	if (ret) {
		hwlog_err("clear_ept_src: failed\n");
		return ret;
	}

	hwlog_info("[clear_ept_src] success\n");
	return 0;
}

static void cps7181_tx_ept_handler(struct cps7181_dev_info *di)
{
	int ret;

	ret = cps7181_tx_get_ept_type(&di->ept_type);
	ret += cps7181_tx_clear_ept_src();
	if (ret)
		return;

	switch (di->ept_type) {
	case CPS7181_TX_EPT_SRC_CEP_TIMEOUT:
	case CPS7181_TX_EPT_SRC_RX_EPT:
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_CEP_TIMEOUT, NULL);
		break;
	case CPS7181_TX_EPT_SRC_POCP:
	case CPS7181_TX_EPT_SRC_FOD:
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_TX_FOD, NULL);
		break;
	case CPS7181_TX_EPT_SRC_OCP:
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_OCP, NULL);
		break;
	case CPS7181_TX_EPT_SRC_OVP:
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_OVP, NULL);
		break;
	default:
		break;
	}
}

static int cps7181_tx_clear_irq(u16 itr)
{
	int ret;

	ret = cps7181_write_word(CPS7181_TX_IRQ_CLR_ADDR, itr);
	if (ret) {
		hwlog_err("clear_irq: write failed\n");
		return ret;
	}

	return 0;
}

static void cps7181_tx_ask_pkt_handler(struct cps7181_dev_info *di)
{
	if (di->irq_val & CPS7181_TX_IRQ_SS_PKG_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_SS_PKG_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
	}

	if (di->irq_val & CPS7181_TX_IRQ_ID_PKT_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_ID_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
	}

	if (di->irq_val & CPS7181_TX_IRQ_CFG_PKT_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_CFG_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_GET_CFG, NULL);
	}

	if (di->irq_val & CPS7181_TX_IRQ_PP_PKT_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_PP_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_non_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_non_qi_ask_pkt();
	}
}

static void cps7181_tx_show_irq(u32 intr)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_cps7181_tx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[tx_irq] %s\n", g_cps7181_tx_irq_name[i]);
	}
}

static int cps7181_tx_get_interrupt(u16 *intr)
{
	int ret;

	ret = cps7181_read_word(CPS7181_TX_IRQ_ADDR, intr);
	if (ret)
		return ret;

	hwlog_info("[get_interrupt] irq=0x%04x\n", *intr);
	cps7181_tx_show_irq(*intr);

	return 0;
}

static void cps7181_tx_mode_irq_recheck(struct cps7181_dev_info *di)
{
	int ret;
	u16 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[tx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = cps7181_tx_get_interrupt(&irq_val);
	if (ret)
		return;

	cps7181_tx_clear_irq(CPS7181_TX_IRQ_CLR_ALL);
}

void cps7181_tx_mode_irq_handler(struct cps7181_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = cps7181_tx_get_interrupt(&di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: get irq failed, clear\n");
		cps7181_tx_clear_irq(CPS7181_TX_IRQ_CLR_ALL);
		goto rechk_irq;
	}

	cps7181_tx_clear_irq(di->irq_val);

	cps7181_tx_ask_pkt_handler(di);

	if (di->irq_val & CPS7181_TX_IRQ_START_PING) {
		di->irq_val &= ~CPS7181_TX_IRQ_START_PING;
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_PING_RX, NULL);
	}
	if (di->irq_val & CPS7181_TX_IRQ_EPT_PKT_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_EPT_PKT_RCVD;
		cps7181_tx_ept_handler(di);
	}
	if (di->irq_val & CPS7181_TX_IRQ_DPING_RCVD) {
		di->irq_val &= ~CPS7181_TX_IRQ_DPING_RCVD;
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_RCV_DPING, NULL);
	}
	if (di->irq_val & CPS7181_TX_IRQ_RPP_TIMEOUT) {
		di->irq_val &= ~CPS7181_TX_IRQ_RPP_TIMEOUT;
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_RP_DM_TIMEOUT, NULL);
	}

rechk_irq:
	cps7181_tx_mode_irq_recheck(di);
}


static void cps7181_tx_activate_chip(void)
{
	int ret;

	msleep(100); /* delay make sure i2c ready */
	ret = cps7181_fw_sram_i2c_init(CPS7181_BYTE_INC);
	if (ret)
		hwlog_err("sram i2c init failed\n");
}

static struct wireless_tx_device_ops g_cps7181_tx_ops = {
	.rx_enable              = cps7181_chip_enable,
	.rx_sleep_enable        = cps7181_sleep_enable,
	.chip_reset             = cps7181_tx_chip_reset,
	.enable_tx_mode         = cps7181_tx_enable_tx_mode,
	.tx_chip_init           = cps7181_tx_chip_init,
	.tx_stop_config         = cps7181_tx_stop_config,
	.check_fwupdate         = cps7181_fw_sram_update,
	.get_tx_iin             = cps7181_tx_get_iin,
	.get_tx_vrect           = cps7181_tx_get_vrect,
	.get_tx_vin             = cps7181_tx_get_vin,
	.get_chip_temp          = cps7181_tx_get_temp,
	.get_tx_fop             = cps7181_tx_get_fop,
	.set_tx_max_fop         = cps7181_tx_set_max_fop,
	.get_tx_max_fop         = cps7181_tx_get_max_fop,
	.set_tx_min_fop         = cps7181_tx_set_min_fop,
	.get_tx_min_fop         = cps7181_tx_get_min_fop,
	.set_tx_ping_frequency  = cps7181_tx_set_ping_frequency,
	.get_tx_ping_frequency  = cps7181_tx_get_ping_frequency,
	.set_tx_ping_interval   = cps7181_tx_set_ping_interval,
	.get_tx_ping_interval   = cps7181_tx_get_ping_interval,
	.check_rx_disconnect    = cps7181_tx_check_rx_disconnect,
	.in_tx_mode             = cps7181_tx_is_tx_mode,
	.in_rx_mode             = cps7181_tx_is_rx_mode,
	.set_tx_open_flag       = cps7181_tx_set_tx_open_flag,
	.set_tx_ilimit          = cps7181_tx_set_ilimit,
	.set_tx_fod_coef        = cps7181_tx_set_fod_coef,
	.set_rp_dm_timeout_val  = cps7181_tx_set_rp_dm_timeout_val,
	.activate_tx_chip       = cps7181_tx_activate_chip,
};

static struct wlps_tx_ops g_cps7181_txps_ops = {
	.tx_vset                = cps7181_tx_mode_vset,
};

int cps7181_tx_ops_register(void)
{
	return wireless_tx_ops_register(&g_cps7181_tx_ops);
}

int cps7181_tx_ps_ops_register(void)
{
	int ret;
	u32 tx_ps_ctrl_src = 0;

	ret = power_dts_read_u32_compatible(power_dts_tag(HWLOG_TAG),
		"huawei,wireless_ps", "tx_ps_ctrl_src", &tx_ps_ctrl_src, 0);
	if (ret)
		return ret;

	if (tx_ps_ctrl_src == WLPS_TX_SRC_TX_CHIP)
		return wlps_tx_ops_register(&g_cps7181_txps_ops);

	return 0;
}
