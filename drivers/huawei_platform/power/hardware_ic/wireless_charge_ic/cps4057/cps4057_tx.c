/*
 * cps4057_tx.c
 *
 * cps4057 tx driver
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

#include "cps4057.h"

#define HWLOG_TAG wireless_cps4057_tx
HWLOG_REGIST();

static bool g_cps4057_tx_open_flag;

static const char * const g_cps4057_tx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0] = "tx_start_ping",
	[1] = "tx_ss_pkt_rcvd",
	[2] = "tx_id_pkt_rcvd",
	[3] = "tx_cfg_pkt_rcvd",
	[4] = "tx_ask_pkt_rcvd",
	[5] = "tx_ept",
	[6] = "tx_rpp_timeout",
	[7] = "tx_cep_timeout",
	[8] = "tx_ac_detect",
	[9] = "tx_init",
	[11] = "rpp_type_err",
};

static const char * const g_cps4057_tx_ept_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "tx_ept_src_wrong_pkt",
	[1]  = "tx_ept_src_ac_detect",
	[2]  = "tx_ept_src_ss",
	[3]  = "tx_ept_src_rx_ept",
	[4]  = "tx_ept_src_cep_timeout",
	[6]  = "tx_ept_src_ocp",
	[7]  = "tx_ept_src_ovp",
	[8]  = "tx_ept_src_uvp",
	[9]  = "tx_ept_src_fod",
	[10] = "tx_ept_src_otp",
	[11] = "tx_ept_src_ping_ocp",
};

static bool cps4057_tx_is_tx_mode(void)
{
	int ret;
	u8 mode = 0;

	ret = cps4057_read_byte(CPS4057_OP_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_tx_mode: get op_mode failed\n");
		return false;
	}

	if (mode == CPS4057_OP_MODE_TX)
		return true;
	else
		return false;
}

static bool cps4057_tx_is_rx_mode(void)
{
	int ret;
	u8 mode = 0;

	ret = cps4057_read_byte(CPS4057_OP_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_rx_mode: get op_mode failed\n");
		return false;
	}

	return mode == CPS4057_OP_MODE_RX;
}

static void cps4057_tx_set_tx_open_flag(bool enable)
{
	g_cps4057_tx_open_flag = enable;
}

static int cps4057_tx_chip_reset(void)
{
	int ret;

	ret = cps4057_write_byte_mask(CPS4057_TX_CMD_ADDR,
		CPS4057_TX_CMD_SYS_RST, CPS4057_TX_CMD_SYS_RST_SHIFT,
		CPS4057_TX_CMD_VAL);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		return ret;
	}

	hwlog_info("[chip_reset] succ\n");
	return 0;
}

static void cps4057_tx_pre_pwr_supply(bool flag)
{
}

static int cps4057_tx_mode_vset(int tx_vset)
{
	return 0;
}

static int cps4057_tx_get_full_bridge_ith(void)
{
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("get_full_bridge_ith: di null\n");
		return CPS4057_FULL_BRIDGE_ITH;
	}

	return di->full_bridge_ith;
}

static int cps4057_tx_set_pt_full_bridge(bool flag)
{
	int ret;
	u16 wr;

	if (flag)
		wr = CPS4057_TX_FUNC_EN;
	else
		wr = CPS4057_TX_FUNC_DIS;

	ret = cps4057_write_word_mask(CPS4057_TX_FUNC_EN_ADDR,
		CPS4057_FULL_BRIDGE_EN_MASK, CPS4057_FULL_BRIDGE_EN_SHIFT, wr);
	if (ret) {
		hwlog_err("set_pt_bridge: write failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_set_bridge(enum wltx_bridge_type type)
{
	if ((type == WLTX_PING_HALF_BRIDGE) || (type == WLTX_PING_FULL_BRIDGE))
		return 0;
	if (type == WLTX_PT_HALF_BRIDGE)
		return cps4057_tx_set_pt_full_bridge(false);
	else if (type == WLTX_PT_FULL_BRIDGE)
		return cps4057_tx_set_pt_full_bridge(true);
	else
		return -EINVAL;
}

static bool cps4057_tx_check_rx_disconnect(void)
{
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("check_rx_disconnect: di null\n");
		return true;
	}

	if (di->ept_type & CPS4057_TX_EPT_SRC_CEP_TIMEOUT) {
		di->ept_type &= ~CPS4057_TX_EPT_SRC_CEP_TIMEOUT;
		hwlog_info("[check_rx_disconnect] rx disconnect\n");
		return true;
	}

	return false;
}

static int cps4057_tx_get_ping_interval(u16 *ping_interval)
{
	int ret;

	if (!ping_interval) {
		hwlog_err("get_ping_interval: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_PING_INTERVAL_ADDR, ping_interval);
	if (ret) {
		hwlog_err("get_ping_interval: read failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_set_ping_interval(u16 ping_interval)
{
	int ret;

	if ((ping_interval < CPS4057_TX_PING_INTERVAL_MIN) ||
		(ping_interval > CPS4057_TX_PING_INTERVAL_MAX)) {
		hwlog_err("set_ping_interval: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_word(CPS4057_TX_PING_INTERVAL_ADDR, ping_interval);
	if (ret) {
		hwlog_err("set_ping_interval: write failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_ping_frequency(u16 *ping_freq)
{
	int ret;
	u8 data = 0;

	if (!ping_freq) {
		hwlog_err("get_ping_frequency: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_byte(CPS4057_TX_PING_FREQ_ADDR, &data);
	if (ret) {
		hwlog_err("get_ping_frequency: read failed\n");
		return ret;
	}
	*ping_freq = (u16)data;

	return 0;
}

static int cps4057_tx_set_ping_frequency(u16 ping_freq)
{
	int ret;

	if ((ping_freq < CPS4057_TX_PING_FREQ_MIN) ||
		(ping_freq > CPS4057_TX_PING_FREQ_MAX)) {
		hwlog_err("set_ping_frequency: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_byte(CPS4057_TX_PING_FREQ_ADDR, (u8)ping_freq);
	if (ret) {
		hwlog_err("set_ping_frequency: write failed\n");
		return ret;
	}

	return ret;
}

static int cps4057_tx_get_min_fop(u16 *fop)
{
	int ret;
	u8 data = 0;

	if (!fop) {
		hwlog_err("get_min_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_byte(CPS4057_TX_MIN_FOP_ADDR, &data);
	if (ret) {
		hwlog_err("get_min_fop: read failed\n");
		return ret;
	}
	*fop = (u16)data;

	return 0;
}

static int cps4057_tx_set_min_fop(u16 fop)
{
	int ret;

	if ((fop < CPS4057_TX_MIN_FOP) || (fop > CPS4057_TX_MAX_FOP)) {
		hwlog_err("set_min_fop: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_byte(CPS4057_TX_MIN_FOP_ADDR, (u8)fop);
	if (ret) {
		hwlog_err("set_min_fop: write failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_max_fop(u16 *fop)
{
	int ret;
	u8 data = 0;

	if (!fop) {
		hwlog_err("get_max_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_byte(CPS4057_TX_MAX_FOP_ADDR, &data);
	if (ret) {
		hwlog_err("get_max_fop: read failed\n");
		return ret;
	}
	*fop = (u16)data;

	return 0;
}

static int cps4057_tx_set_max_fop(u16 fop)
{
	int ret;

	if ((fop < CPS4057_TX_MIN_FOP) || (fop > CPS4057_TX_MAX_FOP)) {
		hwlog_err("set_max_fop: para out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_byte(CPS4057_TX_MAX_FOP_ADDR, (u8)fop);
	if (ret) {
		hwlog_err("set_max_fop: write failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_fop(u16 *fop)
{
	int ret;

	if (!fop) {
		hwlog_err("get_fop: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_OP_FREQ_ADDR, fop);
	if (ret) {
		hwlog_err("get_fop: failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_temp(u8 *chip_temp)
{
	int ret;
	u16 temp = 0;

	if (!chip_temp) {
		hwlog_err("get_temp: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_CHIP_TEMP_ADDR, &temp);
	if (ret) {
		hwlog_err("get_temp: read failed\n");
		return ret;
	}

	*chip_temp = (u8)temp;

	return 0;
}

static int cps4057_tx_get_vin(u16 *tx_vin)
{
	int ret;

	if (!tx_vin) {
		hwlog_err("get_vin: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_VIN_ADDR, tx_vin);
	if (ret) {
		hwlog_err("get_vin: read failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_vrect(u16 *tx_vrect)
{
	int ret;

	if (!tx_vrect) {
		hwlog_err("get_vrect: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_VRECT_ADDR, tx_vrect);
	if (ret) {
		hwlog_err("get_vrect: read failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_get_iin(u16 *tx_iin)
{
	int ret;

	if (!tx_iin) {
		hwlog_err("get_iin: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_IIN_ADDR, tx_iin);
	if (ret) {
		hwlog_err("get_iin: read failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_set_ilimit(int tx_ilim)
{
	int ret;

	if ((tx_ilim < CPS4057_TX_ILIM_MIN) ||
		(tx_ilim > CPS4057_TX_ILIM_MAX)) {
		hwlog_err("set_ilimit: out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_word(CPS4057_TX_ILIM_ADDR, (u16)tx_ilim);
	if (ret) {
		hwlog_err("set_ilimit: failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_tx_set_fod_coef(u32 pl_th, u8 pl_cnt)
{
	return 0;
}

static int cps4057_tx_init_fod_coef(struct cps4057_dev_info *di)
{
	int ret;

	ret = cps4057_write_word(CPS4057_TX_PLOSS_TH0_ADDR, di->tx_ploss_th0);
	ret += cps4057_write_word(CPS4057_TX_PLOSS_TH1_ADDR, di->tx_ploss_th1);
	ret += cps4057_write_word(CPS4057_TX_PLOSS_TH2_ADDR, di->tx_ploss_th2);
	ret += cps4057_write_word(CPS4057_TX_PLOSS_TH3_ADDR, di->tx_ploss_th3);
	ret += cps4057_write_byte(CPS4057_TX_PLOSS_CNT_ADDR, di->tx_ploss_cnt);
	if (ret) {
		hwlog_err("init_fod_coef: failed\n");
		return ret;
	}

	return 0;
}

static void cps4057_tx_set_rp_dm_timeout_val(u8 val)
{
}

static int cps4057_tx_stop_config(void)
{
	return 0;
}

static int cps4057_sw2tx(void)
{
	int ret;
	int i;
	u8 mode = 0;
	int cnt = CPS4057_SW2TX_RETRY_TIME / CPS4057_SW2TX_SLEEP_TIME;

	for (i = 0; i < cnt; i++) {
		msleep(CPS4057_SW2TX_SLEEP_TIME);
		ret = cps4057_get_mode(&mode);
		if (ret) {
			hwlog_err("sw2tx: get mode failed\n");
			continue;
		}
		if (mode == CPS4057_OP_MODE_TX) {
			hwlog_info("sw2tx: succ, cnt=%d\n", i);
			msleep(CPS4057_SW2TX_SLEEP_TIME);
			return 0;
		}
		ret = cps4057_write_byte_mask(CPS4057_TX_CMD_ADDR,
			CPS4057_TX_CMD_EN_TX, CPS4057_TX_CMD_EN_TX_SHIFT,
			CPS4057_TX_CMD_VAL);
		if (ret)
			hwlog_err("sw2tx: write cmd(sw2tx) failed\n");
	}
	hwlog_err("sw2tx: failed, cnt=%d\n", i);
	return -WLC_ERR_I2C_WR;
}

static void cps4057_tx_select_init_para(struct cps4057_dev_info *di,
	enum wltx_open_type type)
{
	switch (type) {
	case WLTX_OPEN_BY_CLIENT:
		di->tx_init_para.ping_freq = CPS4057_TX_PING_FREQ;
		di->tx_init_para.ping_interval = CPS4057_TX_PING_INTERVAL;
		break;
	case WLTX_OPEN_BY_COIL_TEST:
		di->tx_init_para.ping_freq = CPS4057_COIL_TEST_PING_FREQ;
		di->tx_init_para.ping_interval = CPS4057_COIL_TEST_PING_INTERVAL;
		break;
	default:
		di->tx_init_para.ping_freq = CPS4057_TX_PING_FREQ;
		di->tx_init_para.ping_interval = CPS4057_TX_PING_INTERVAL;
		break;
	}
}

static int cps4057_tx_set_init_para(struct cps4057_dev_info *di)
{
	int ret;

	ret = cps4057_sw2tx();
	if (ret) {
		hwlog_err("set_init_para: sw2tx failed\n");
		return ret;
	}

	ret = cps4057_write_word(CPS4057_TX_OCP_TH_ADDR, CPS4057_TX_OCP_TH);
	ret += cps4057_write_word(CPS4057_TX_OVP_TH_ADDR, CPS4057_TX_OVP_TH);
	ret += cps4057_write_word(CPS4057_TX_IRQ_EN_ADDR, CPS4057_TX_IRQ_VAL);
	ret += cps4057_write_word(CPS4057_TX_PING_OCP_ADDR, CPS4057_TX_PING_OCP_TH);
	ret += cps4057_tx_init_fod_coef(di);
	ret += cps4057_tx_set_ping_frequency(di->tx_init_para.ping_freq);
	ret += cps4057_tx_set_min_fop(CPS4057_TX_MIN_FOP);
	ret += cps4057_tx_set_max_fop(CPS4057_TX_MAX_FOP);
	ret += cps4057_tx_set_ping_interval(di->tx_init_para.ping_interval);
	if (ret) {
		hwlog_err("set_init_para: write failed\n");
		return -WLC_ERR_I2C_W;
	}

	return 0;
}

static int cps4057_tx_chip_init(enum wltx_open_type type)
{
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("chip_init: di null\n");
		return -WLC_ERR_PARA_NULL;
	}

	di->irq_cnt = 0;
	di->g_val.irq_abnormal_flag = false;
	cps4057_enable_irq();

	cps4057_tx_select_init_para(di, type);
	return cps4057_tx_set_init_para(di);
}

static int cps4057_tx_enable_tx_mode(bool enable)
{
	int ret;

	if (enable)
		ret = cps4057_write_byte_mask(CPS4057_TX_FUNC_EN_ADDR,
			CPS4057_TX_FUNC_EN_TX_MASK, CPS4057_TX_FUNC_EN_SHIFT,
			CPS4057_TX_FUNC_EN);
	else
		ret = cps4057_write_byte_mask(CPS4057_TX_FUNC_EN_ADDR,
			CPS4057_TX_FUNC_EN_TX_MASK, CPS4057_TX_FUNC_EN_SHIFT,
			CPS4057_TX_FUNC_DIS);

	if (ret) {
		hwlog_err("%s tx_mode failed\n", enable ? "enable" : "disable");
		return ret;
	}

	return 0;
}

static void cps4057_tx_show_ept_type(u16 ept)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_cps4057_tx_ept_name); i++) {
		if (ept & BIT(i))
			hwlog_info("[tx_ept] %s\n", g_cps4057_tx_ept_name[i]);
	}
}

static int cps4057_tx_get_ept_type(u16 *ept)
{
	int ret;
	u16 ept_value = 0;

	if (!ept) {
		hwlog_err("get_ept_type: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	ret = cps4057_read_word(CPS4057_TX_EPT_SRC_ADDR, &ept_value);
	if (ret) {
		hwlog_err("get_ept_type: read failed\n");
		return ret;
	}
	*ept = ept_value;
	hwlog_info("[get_ept_type] type=0x%02x", *ept);
	cps4057_tx_show_ept_type(*ept);

	return 0;
}

static int cps4057_tx_clear_ept_src(void)
{
	int ret;

	ret = cps4057_write_word(CPS4057_TX_EPT_SRC_ADDR, CPS4057_TX_EPT_SRC_CLEAR);
	if (ret) {
		hwlog_err("clear_ept_src: failed\n");
		return ret;
	}

	hwlog_info("[clear_ept_src] success\n");
	return 0;
}

static void cps4057_tx_ept_handler(struct cps4057_dev_info *di)
{
	int ret;

	ret = cps4057_tx_get_ept_type(&di->ept_type);
	ret += cps4057_tx_clear_ept_src();
	if (ret)
		return;

	switch (di->ept_type) {
	case CPS4057_TX_EPT_SRC_RX_EPT:
	case CPS4057_TX_EPT_SRC_SSP:
	case CPS4057_TX_EPT_SRC_CEP_TIMEOUT:
		di->ept_type &= ~CPS4057_TX_EPT_SRC_CEP_TIMEOUT;
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_CEP_TIMEOUT, NULL);
		break;
	case CPS4057_TX_EPT_SRC_FOD:
	case CPS4057_TX_EPT_SRC_POCP:
		di->ept_type &= ~CPS4057_TX_EPT_SRC_FOD;
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_TX_FOD, NULL);
		break;
	default:
		break;
	}
}

static int cps4057_tx_clear_irq(u16 itr)
{
	int ret;

	ret = cps4057_write_word(CPS4057_TX_IRQ_CLR_ADDR, itr);
	if (ret) {
		hwlog_err("clear_irq: write failed\n");
		return ret;
	}

	return 0;
}

static void cps4057_tx_ask_pkt_handler(struct cps4057_dev_info *di)
{
	if (di->irq_val & CPS4057_TX_IRQ_SS_PKG_RCVD) {
		di->irq_val &= ~CPS4057_TX_IRQ_SS_PKG_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
	}

	if (di->irq_val & CPS4057_TX_IRQ_ID_PKT_RCVD) {
		di->irq_val &= ~CPS4057_TX_IRQ_ID_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
	}

	if (di->irq_val & CPS4057_TX_IRQ_CFG_PKT_RCVD) {
		di->irq_val &= ~CPS4057_TX_IRQ_CFG_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt();
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_GET_CFG, NULL);
	}

	if (di->irq_val & CPS4057_TX_IRQ_ASK_PKT_RCVD) {
		di->irq_val &= ~CPS4057_TX_IRQ_ASK_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_non_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_non_qi_ask_pkt();
	}
}

static void cps4057_tx_show_irq(u32 intr)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_cps4057_tx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[tx_irq] %s\n", g_cps4057_tx_irq_name[i]);
	}
}

static int cps4057_tx_get_interrupt(u16 *intr)
{
	int ret;

	ret = cps4057_read_word(CPS4057_TX_IRQ_ADDR, intr);
	if (ret)
		return ret;

	hwlog_info("[get_interrupt] irq=0x%04x\n", *intr);
	cps4057_tx_show_irq(*intr);

	return 0;
}

static void cps4057_tx_mode_irq_recheck(struct cps4057_dev_info *di)
{
	int ret;
	u16 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[tx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = cps4057_tx_get_interrupt(&irq_val);
	if (ret)
		return;

	cps4057_tx_clear_irq(CPS4057_TX_IRQ_CLR_ALL);
}

void cps4057_tx_mode_irq_handler(struct cps4057_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = cps4057_tx_get_interrupt(&di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: get irq failed, clear\n");
		cps4057_tx_clear_irq(CPS4057_TX_IRQ_CLR_ALL);
		goto rechk_irq;
	}

	cps4057_tx_clear_irq(di->irq_val);

	cps4057_tx_ask_pkt_handler(di);

	if (di->irq_val & CPS4057_TX_IRQ_START_PING) {
		di->irq_val &= ~CPS4057_TX_IRQ_START_PING;
		blocking_notifier_call_chain(&tx_event_nh,
			WL_TX_EVENT_PING_RX, NULL);
	}
	if (di->irq_val & CPS4057_TX_IRQ_EPT_PKT_RCVD) {
		di->irq_val &= ~CPS4057_TX_IRQ_EPT_PKT_RCVD;
		cps4057_tx_ept_handler(di);
	}
	if (di->irq_val & CPS4057_TX_IRQ_AC_DET) {
		di->irq_val &= ~CPS4057_TX_IRQ_AC_DET;
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_RCV_DPING, NULL);
	}
	if (di->irq_val & CPS4057_TX_IRQ_RPP_TIMEOUT) {
		di->irq_val &= ~CPS4057_TX_IRQ_RPP_TIMEOUT;
		blocking_notifier_call_chain(&tx_event_nh,
			WLTX_EVT_RP_DM_TIMEOUT, NULL);
	}

rechk_irq:
	cps4057_tx_mode_irq_recheck(di);
}

static struct wireless_tx_device_ops g_cps4057_tx_ops = {
	.rx_enable              = cps4057_chip_enable,
	.rx_sleep_enable        = cps4057_sleep_enable,
	.chip_reset             = cps4057_tx_chip_reset,
	.pre_ps                 = cps4057_tx_pre_pwr_supply,
	.enable_tx_mode         = cps4057_tx_enable_tx_mode,
	.tx_chip_init           = cps4057_tx_chip_init,
	.tx_stop_config         = cps4057_tx_stop_config,
	.check_fwupdate         = cps4057_fw_sram_update,
	.get_tx_iin             = cps4057_tx_get_iin,
	.get_tx_vrect           = cps4057_tx_get_vrect,
	.get_tx_vin             = cps4057_tx_get_vin,
	.get_chip_temp          = cps4057_tx_get_temp,
	.get_tx_fop             = cps4057_tx_get_fop,
	.set_tx_max_fop         = cps4057_tx_set_max_fop,
	.get_tx_max_fop         = cps4057_tx_get_max_fop,
	.set_tx_min_fop         = cps4057_tx_set_min_fop,
	.get_tx_min_fop         = cps4057_tx_get_min_fop,
	.set_tx_ping_frequency  = cps4057_tx_set_ping_frequency,
	.get_tx_ping_frequency  = cps4057_tx_get_ping_frequency,
	.set_tx_ping_interval   = cps4057_tx_set_ping_interval,
	.get_tx_ping_interval   = cps4057_tx_get_ping_interval,
	.check_rx_disconnect    = cps4057_tx_check_rx_disconnect,
	.in_tx_mode             = cps4057_tx_is_tx_mode,
	.in_rx_mode             = cps4057_tx_is_rx_mode,
	.set_tx_open_flag       = cps4057_tx_set_tx_open_flag,
	.set_tx_ilimit          = cps4057_tx_set_ilimit,
	.set_tx_fod_coef        = cps4057_tx_set_fod_coef,
	.set_rp_dm_timeout_val  = cps4057_tx_set_rp_dm_timeout_val,
	.set_bridge             = cps4057_tx_set_bridge,
	.get_full_bridge_ith    = cps4057_tx_get_full_bridge_ith,
};

static struct wlps_tx_ops g_cps4057_txps_ops = {
	.tx_vset                = cps4057_tx_mode_vset,
};

int cps4057_tx_ops_register(void)
{
	return wireless_tx_ops_register(&g_cps4057_tx_ops);
}

int cps4057_tx_ps_ops_register(void)
{
	int ret;
	u32 tx_ps_ctrl_src = 0;

	ret = power_dts_read_u32_compatible(power_dts_tag(HWLOG_TAG),
		"huawei,wireless_ps", "tx_ps_ctrl_src", &tx_ps_ctrl_src, 0);
	if (ret)
		return ret;

	if (tx_ps_ctrl_src == WLPS_TX_SRC_TX_CHIP)
		return wlps_tx_ops_register(&g_cps4057_txps_ops);

	return 0;
}
