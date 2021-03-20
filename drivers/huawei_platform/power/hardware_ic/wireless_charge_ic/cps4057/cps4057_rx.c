/*
 * cps4057_rx.c
 *
 * cps4057 rx driver
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

#define HWLOG_TAG wireless_cps4057_rx
HWLOG_REGIST();

static const char * const g_cps4057_rx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "rx_power_on",
	[1]  = "rx_ldo_off",
	[2]  = "rx_ldo_on",
	[3]  = "rx_ready",
	[4]  = "rx_fsk_ack",
	[5]  = "rx_fsk_timeout",
	[6]  = "rx_fsk_pkt",
	[7]  = "rx_vrect_ovp",
	[8]  = "rx_otp",
	[9]  = "rx_ocp",
	[11] = "rx_scp",
	[12] = "rx_mldo_ovp",
};

static int cps4057_rx_get_temp(void)
{
	int ret;
	s16 temp = 0;

	ret = cps4057_read_word(CPS4057_RX_CHIP_TEMP_ADDR, (u16 *)&temp);
	if (ret)
		return ret;

	return temp;
}

static int cps4057_rx_get_fop(void)
{
	int ret;
	u16 fop = 0;

	ret = cps4057_read_word(CPS4057_RX_OP_FREQ_ADDR, &fop);
	if (ret)
		return ret;

	return (int)fop;
}

static int cps4057_rx_get_cep(void)
{
	int ret;
	s8 cep = 0;

	ret = cps4057_read_byte(CPS4057_RX_CE_VAL_ADDR, (u8 *)&cep);
	if (ret)
		return ret;

	return (int)cep;
}

static int cps4057_rx_get_vrect(void)
{
	int ret;
	u16 vrect = 0;

	ret = cps4057_read_word(CPS4057_RX_VRECT_ADDR, &vrect);
	if (ret)
		return ret;

	return (int)vrect;
}

static int cps4057_rx_get_vout(void)
{
	int ret;
	u16 vout = 0;

	ret = cps4057_read_word(CPS4057_RX_VOUT_ADDR, &vout);
	if (ret)
		return ret;

	return (int)vout;
}

static int cps4057_rx_get_iout(void)
{
	int ret;
	u16 iout = 0;

	ret = cps4057_read_word(CPS4057_RX_IOUT_ADDR, &iout);
	if (ret)
		return ret;

	return (int)iout;
}

static int cps4057_rx_get_rx_vout_reg(void)
{
	int ret;
	u16 vreg = 0;

	ret = cps4057_read_word(CPS4057_RX_VOUT_SET_ADDR, &vreg);
	if (ret) {
		hwlog_err("get_rx_vout_reg: failed\n");
		return ret;
	}

	return (int)vreg;
}

static int cps4057_rx_get_tx_vout_reg(void)
{
	int ret;
	u16 tx_vreg;

	ret = cps4057_read_word(CPS4057_RX_FC_VOLT_ADDR, &tx_vreg);
	if (ret) {
		hwlog_err("get_tx_vout_reg: failed\n");
		return ret;
	}

	return (int)tx_vreg;
}

static int cps4057_rx_get_def_imax(void)
{
	return CPS4057_DFT_IOUT_MAX;
}

static void cps4057_rx_show_irq(u16 intr)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_cps4057_rx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[rx_irq] %s\n", g_cps4057_rx_irq_name[i]);
	}
}

static int cps4057_rx_get_interrupt(u16 *intr)
{
	int ret;

	ret = cps4057_read_word(CPS4057_RX_IRQ_ADDR, intr);
	if (ret)
		return ret;

	hwlog_info("[get_interrupt] irq=0x%04x\n", *intr);
	cps4057_rx_show_irq(*intr);

	return 0;
}

static int cps4057_rx_clear_irq(u16 intr)
{
	int ret;

	ret = cps4057_write_word(CPS4057_RX_IRQ_CLR_ADDR, intr);
	if (ret) {
		hwlog_err("clear_irq: failed\n");
		return ret;
	}

	return 0;
}

static int cps4057_rx_chip_reset(void)
{
	int ret;

	ret = cps4057_write_byte_mask(CPS4057_RX_CMD_ADDR,
		CPS4057_RX_CMD_SYS_RST, CPS4057_RX_CMD_SYS_RST_SHIFT,
		CPS4057_RX_CMD_VAL);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		return ret;
	}

	hwlog_info("[chip_reset] succ\n");
	return 0;
}

static void cps4057_rx_ext_pwr_post_ctrl(int flag)
{
	u16 val;

	if (flag == WLPS_CTRL_ON)
		val = CPS4057_RX_FUNC_EN;
	else
		val = CPS4057_RX_FUNC_DIS;

	(void)cps4057_write_word_mask(CPS4057_RX_FUNC_EN_ADDR,
		CPS4057_RX_EXT_VCC_EN_MASK, CPS4057_RX_EXT_VCC_EN_SHIFT, val);
}

static void cps4057_ask_mode_cfg(u16 val, int polarity)
{
	int ret;

	ret = cps4057_write_word_mask(CPS4057_RX_FUNC_EN_ADDR,
		CPS4057_RX_CMALL_EN_MASK, CPS4057_RX_CMALL_EN_SHIFT, val);
	ret += cps4057_write_word_mask(CPS4057_RX_FUNC_EN_ADDR,
		CPS4057_RX_CM_POLARITY_MASK, CPS4057_RX_CM_POLARITY_SHIFT, polarity);
	if (ret)
		hwlog_err("ask_mode_cfg: write fail\n");
}

static void cps4057_set_mode_cfg(int vset)
{
	if (vset <= RX_HIGH_VOUT) {
		cps4057_ask_mode_cfg(CPS4057_RX_CMALL_EN_VAL,
			CPS4057_RX_CM_POSITIVE);
	} else {
		if (!power_cmdline_is_factory_mode() && (vset >= RX_HIGH_VOUT2))
			cps4057_ask_mode_cfg(CPS4057_RX_CMC_EN_VAL, CPS4057_RX_CM_NEGTIVE);
		else
			cps4057_ask_mode_cfg(CPS4057_RX_CMBC_EN_VAL, CPS4057_RX_CM_NEGTIVE);
	}
}

static int cps4057_rx_set_rx_vout(int vol)
{
	int ret;

	if ((vol < CPS4057_RX_VOUT_MIN) || (vol > CPS4057_RX_VOUT_MAX)) {
		hwlog_err("set_rx_vout: out of range\n");
		return -WLC_ERR_PARA_WRONG;
	}

	ret = cps4057_write_word(CPS4057_RX_VOUT_SET_ADDR, (u16)vol);
	if (ret) {
		hwlog_err("set_rx_vout: failed\n");
		return ret;
	}

	return 0;
}

static bool cps4057_rx_is_cp_open(void)
{
	int rx_ratio;
	int rx_vset;
	int rx_vout;
	int cp_vout;

	if (!charge_pump_is_cp_open(CP_TYPE_MAIN)) {
		hwlog_err("rx_is_cp_open: failed\n");
		return false;
	}

	rx_ratio = charge_pump_get_cp_ratio(CP_TYPE_MAIN);
	rx_vset = cps4057_rx_get_rx_vout_reg();
	rx_vout = cps4057_rx_get_vout();
	cp_vout = charge_pump_get_cp_vout(CP_TYPE_MAIN);
	cp_vout = (cp_vout > 0) ? cp_vout : wldc_get_ls_vbus();

	hwlog_info("[is_cp_open] [rx] ratio:%d vset:%d vout:%d [cp] vout:%d\n",
		rx_ratio, rx_vset, rx_vout, cp_vout);
	if ((cp_vout * rx_ratio) < (rx_vout - CPS4057_RX_FC_VOUT_ERR_LTH))
		return false;
	if ((cp_vout * rx_ratio) > (rx_vout + CPS4057_RX_FC_VOUT_ERR_UTH))
		return false;

	return true;
}

static int cps4057_rx_check_cp_mode(struct cps4057_dev_info *di)
{
	int i;
	int cnt;
	int ret;

	ret = charge_pump_set_cp_mode(CP_TYPE_MAIN);
	if (ret) {
		hwlog_err("check_cp_mode: set cp_mode failed\n");
		return ret;
	}
	cnt = CPS4057_RX_BPCP_TIMEOUT / CPS4057_RX_BPCP_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		msleep(CPS4057_RX_BPCP_SLEEP_TIME);
		if (cps4057_rx_is_cp_open()) {
			hwlog_info("[check_cp_mode] set cp_mode succ\n");
			return 0;
		}
		if (di->g_val.rx_stop_chrg_flag) {
			hwlog_err("check_cp_mode: wlc already stopped\n");
			return -WLC_ERR_STOP_CHRG;
		}
	}

	hwlog_err("check_cp_mode: set cp_mode timeout\n");
	return -WLC_ERR_MISMATCH;
}

static int cps4057_rx_send_fc_cmd(int vset)
{
	int ret;

	ret = cps4057_write_word(CPS4057_RX_FC_VOLT_ADDR, (u16)vset);
	if (ret) {
		hwlog_err("send_fc_cmd: set fc_reg failed\n");
		return ret;
	}
	ret = cps4057_write_byte_mask(CPS4057_RX_CMD_ADDR,
		CPS4057_RX_CMD_SEND_FC, CPS4057_RX_CMD_SEND_FC_SHIFT,
		CPS4057_RX_CMD_VAL);
	if (ret) {
		hwlog_err("send_fc_cmd: send fc_cmd failed\n");
		return ret;
	}

	return 0;
}

static bool cps4057_rx_is_fc_succ(struct cps4057_dev_info *di, int vset)
{
	int i;
	int cnt;
	int vout;

	cnt = CPS4057_RX_FC_VOUT_TIMEOUT / CPS4057_RX_FC_VOUT_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		if (di->g_val.rx_stop_chrg_flag &&
			(vset > CPS4057_RX_FC_VOUT_DEFAULT))
			return false;
		msleep(CPS4057_RX_FC_VOUT_SLEEP_TIME);
		vout = cps4057_rx_get_vout();
		if ((vout >= vset - CPS4057_RX_FC_VOUT_ERR_LTH) &&
			(vout <= vset + CPS4057_RX_FC_VOUT_ERR_UTH)) {
			hwlog_info("[is_fc_succ] succ, cost_time: %dms\n",
				(i + 1) * CPS4057_RX_FC_VOUT_SLEEP_TIME);
			(void)cps4057_rx_set_rx_vout(vset);
			return true;
		}
	}

	return false;
}

static int cps4057_rx_set_tx_vout(int vset)
{
	int ret;
	int i;
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("set_tx_vout: di null\n");
		return -WLC_ERR_PARA_NULL;
	}

	if (vset >= RX_HIGH_VOUT2) {
		ret = cps4057_rx_check_cp_mode(di);
		if (ret)
			return ret;
	}

	for (i = 0; i < CPS4057_RX_FC_VOUT_RETRY_CNT; i++) {
		if (di->g_val.rx_stop_chrg_flag &&
			(vset > CPS4057_RX_FC_VOUT_DEFAULT))
			return -WLC_ERR_STOP_CHRG;
		ret = cps4057_rx_send_fc_cmd(vset);
		if (ret) {
			hwlog_err("set_tx_vout: send fc_cmd failed\n");
			continue;
		}
		hwlog_info("[set_tx_vout] send fc_cmd, cnt: %d\n", i);
		if (cps4057_rx_is_fc_succ(di, vset)) {
			if (vset < RX_HIGH_VOUT2)
				(void)charge_pump_set_bp_mode(CP_TYPE_MAIN);
			cps4057_set_mode_cfg(vset);
			hwlog_info("[set_tx_vout] succ\n");
			return 0;
		}
	}

	return -WLC_ERR_MISMATCH;
}

static int cps4057_rx_send_ept(enum wireless_etp_type ept_type)
{
	int ret;

	switch (ept_type) {
	case WIRELESS_EPT_ERR_VRECT:
	case WIRELESS_EPT_ERR_VOUT:
		break;
	default:
		return -WLC_ERR_PARA_WRONG;
	}
	ret = cps4057_write_byte(CPS4057_RX_EPT_MSG_ADDR, (u8)ept_type);
	ret += cps4057_write_byte_mask(CPS4057_RX_CMD_ADDR,
		CPS4057_RX_CMD_SEND_EPT, CPS4057_RX_CMD_SEND_EPT_SHIFT,
		CPS4057_RX_CMD_VAL);
	if (ret) {
		hwlog_err("send_ept: failed, ept=0x%x\n", ept_type);
		return ret;
	}

	return 0;
}

bool cps4057_rx_check_tx_exist(void)
{
	int ret;
	u8 mode = 0;

	ret = cps4057_get_mode(&mode);
	if (ret) {
		hwlog_err("check_tx_exist: get rx mode failed\n");
		return false;
	}
	if (mode == CPS4057_OP_MODE_RX)
		return true;

	hwlog_info("[check_tx_exist] mode = %d\n", mode);
	return false;
}

static int cps4057_rx_kick_watchdog(void)
{
	int ret;

	ret = cps4057_write_word(CPS4057_RX_WDT_TIMEOUT_ADDR,
		CPS4057_RX_WDT_TIMEOUT);
	if (ret)
		return ret;

	return 0;
}

static int cps4057_rx_get_fod(char *fod_str, int len)
{
	int i;
	int ret;
	char tmp[CPS4057_RX_FOD_TMP_STR_LEN] = { 0 };
	u8 fod_arr[CPS4057_RX_FOD_LEN] = { 0 };

	if (!fod_str || (len != WLC_FOD_COEF_STR_LEN))
		return -WLC_ERR_PARA_WRONG;

	memset(fod_str, 0, len);
	ret = cps4057_read_block(CPS4057_RX_FOD_ADDR,
		fod_arr, CPS4057_RX_FOD_LEN);
	if (ret) {
		hwlog_err("rx_get_fod read fod failed\n");
		return ret;
	}

	for (i = 0; i < CPS4057_RX_FOD_LEN; i++) {
		snprintf(tmp, CPS4057_RX_FOD_TMP_STR_LEN, "%x ", fod_arr[i]);
		strncat(fod_str, tmp, strlen(tmp));
	}

	return 0;
}

static int cps4057_rx_set_fod(const char *fod_str)
{
	int ret;
	char *cur = (char *)fod_str;
	char *token = NULL;
	int i;
	u8 val = 0;
	const char *sep = " ,";
	u8 fod_arr[CPS4057_RX_FOD_LEN] = { 0 };

	if (!fod_str) {
		hwlog_err("set_fod: input fod_str null\n");
		return -WLC_ERR_PARA_NULL;
	}

	for (i = 0; i < CPS4057_RX_FOD_LEN; i++) {
		token = strsep(&cur, sep);
		if (!token) {
			hwlog_err("set_fod: input fod_str number err\n");
			return -WLC_ERR_PARA_WRONG;
		}
		ret = kstrtou8(token, POWER_BASE_DEC, &val);
		if (ret) {
			hwlog_err("set_fod: input fod_str type err\n");
			return -WLC_ERR_PARA_WRONG;
		}
		fod_arr[i] = val;
		hwlog_info("[set_fod] fod[%d]=0x%x\n", i, fod_arr[i]);
	}

	return cps4057_write_block(CPS4057_RX_FOD_ADDR,
		fod_arr, CPS4057_RX_FOD_LEN);
}

static int cps4057_rx_init_fod_coef(struct cps4057_dev_info *di)
{
	int tx_vset;
	int ret;
	u8 *rx_fod = NULL;

	tx_vset = cps4057_rx_get_tx_vout_reg();
	hwlog_info("[init_fod_coef] tx_vout_reg: %dmV\n", tx_vset);

	if (tx_vset < 9000) /* (0, 9)V, set 5v fod */
		rx_fod = di->rx_fod_5v;
	else if (tx_vset < 15000) /* [9, 15)V, set 9V fod */
		rx_fod = di->rx_fod_9v;
	else if (tx_vset < 18000) /* [15, 18)V, set 15V fod */
		rx_fod = di->rx_fod_15v;
	else
		return -WLC_ERR_MISMATCH;

	ret = cps4057_write_block(CPS4057_RX_FOD_ADDR,
		rx_fod, CPS4057_RX_FOD_LEN);
	if (ret) {
		hwlog_err("init_fod_coef: write fod failed\n");
		return ret;
	}

	return ret;
}

static int cps4057_rx_chip_init(int init_type, int tx_type)
{
	int ret = 0;
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("chip_init: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	switch (init_type) {
	case WIRELESS_CHIP_INIT:
		hwlog_info("[chip_init] default chip init\n");
		di->g_val.rx_stop_chrg_flag = false;
		ret += cps4057_write_word_mask(CPS4057_RX_FUNC_EN_ADDR,
			CPS4057_RX_HV_WDT_EN_MASK, CPS4057_RX_HV_WDT_EN_SHIFT,
			CPS4057_RX_FUNC_EN);
		ret += cps4057_write_word(CPS4057_RX_WDT_TIMEOUT_ADDR,
			CPS4057_RX_WDT_TIMEOUT);
		/* fall through */
	case ADAPTER_5V * WL_MVOLT_PER_VOLT:
		hwlog_info("[chip_init] 5v chip init\n");
		ret += cps4057_write_block(CPS4057_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg_5v, CPS4057_RX_LDO_CFG_LEN);
		ret += cps4057_rx_init_fod_coef(di);
		cps4057_set_mode_cfg(init_type);
		break;
	case ADAPTER_9V * WL_MVOLT_PER_VOLT:
		hwlog_info("[chip_init] 9v chip init\n");
		ret += cps4057_write_block(CPS4057_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg_9v, CPS4057_RX_LDO_CFG_LEN);
		ret += cps4057_rx_init_fod_coef(di);
		break;
	case ADAPTER_12V * WL_MVOLT_PER_VOLT:
		hwlog_info("[chip_init] 12v chip init\n");
		ret += cps4057_write_block(CPS4057_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg_12v, CPS4057_RX_LDO_CFG_LEN);
		ret += cps4057_rx_init_fod_coef(di);
		break;
	case WILREESS_SC_CHIP_INIT:
		hwlog_info("[chip_init] sc chip init\n");
		ret += cps4057_write_block(CPS4057_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg_sc, CPS4057_RX_LDO_CFG_LEN);
		ret += cps4057_rx_init_fod_coef(di);
		break;
	default:
		hwlog_err("chip_init: input para invalid\n");
		break;
	}

	return ret;
}

static int cps4057_rx_stop_charging(void)
{
	int wired_channel_state;
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("stop_charging: para null\n");
		return -WLC_ERR_PARA_NULL;
	}

	di->g_val.rx_stop_chrg_flag = true;

	if (!di->g_val.irq_abnormal_flag)
		return 0;

	wired_channel_state = wireless_charge_get_wired_channel_state();
	if (wired_channel_state != WIRED_CHANNEL_ON) {
		hwlog_info("[stop_charging] irq_abnormal, keep rx_sw on\n");
		di->g_val.irq_abnormal_flag = true;
		charge_pump_chip_enable(CP_TYPE_MAIN, true);
		wlps_control(WLPS_RX_SW, WLPS_CTRL_ON);
	} else {
		di->irq_cnt = 0;
		di->g_val.irq_abnormal_flag = false;
		cps4057_enable_irq();
		hwlog_info("[stop_charging] wired channel on, enable irq\n");
	}

	return 0;
}

static int cps4057_rx_data_rcvd_handler(struct cps4057_dev_info *di)
{
	int ret;
	int i;
	u8 cmd;
	u8 buff[QI_PKT_LEN] = { 0 };

	ret = cps4057_read_block(CPS4057_RCVD_MSG_HEADER_ADDR,
		buff, QI_PKT_LEN);
	if (ret) {
		hwlog_err("data_received_handler: read received data failed\n");
		return ret;
	}

	cmd = buff[QI_PKT_CMD];
	hwlog_info("[data_received_handler] cmd: 0x%x\n", cmd);
	for (i = QI_PKT_DATA; i < QI_PKT_LEN; i++)
		hwlog_info("[data_received_handler] data: 0x%x\n", buff[i]);

	switch (cmd) {
	case QI_CMD_TX_ALARM:
	case QI_CMD_ACK_BST_ERR:
		di->irq_val &= ~CPS4057_RX_IRQ_FSK_PKT;
		if (di->g_val.qi_hdl &&
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt)
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt(buff, QI_PKT_LEN);
		break;
	default:
		break;
	}
	return 0;
}

void cps4057_rx_abnormal_irq_handler(struct cps4057_dev_info *di)
{
	static struct timespec64 ts64_timeout;
	struct timespec64 ts64_interval;
	struct timespec64 ts64_now;

	ts64_now = current_kernel_time64();
	ts64_interval.tv_sec = 0;
	ts64_interval.tv_nsec = WIRELESS_INT_TIMEOUT_TH * NSEC_PER_MSEC;

	if (!di)
		return;

	hwlog_info("[handle_abnormal_irq] irq_cnt = %d\n", ++di->irq_cnt);
	/* power on irq occurs first time, so start monitor now */
	if (di->irq_cnt == 1) {
		ts64_timeout = timespec64_add_safe(ts64_now, ts64_interval);
		if (ts64_timeout.tv_sec == TIME_T_MAX) {
			di->irq_cnt = 0;
			hwlog_err("handle_abnormal_irq: time overflow\n");
			return;
		}
	}

	if (timespec64_compare(&ts64_now, &ts64_timeout) < 0)
		return;

	if (di->irq_cnt < WIRELESS_INT_CNT_TH) {
		di->irq_cnt = 0;
		return;
	}

	di->g_val.irq_abnormal_flag = true;
	charge_pump_chip_enable(CP_TYPE_MAIN, true);
	wlps_control(WLPS_RX_SW, WLPS_CTRL_ON);
	cps4057_disable_irq_nosync();
	gpio_set_value(di->gpio_sleep_en, RX_SLEEP_EN_DISABLE);
	hwlog_err("handle_abnormal_irq: more than %d irq in %ds, disable irq\n",
		WIRELESS_INT_CNT_TH, WIRELESS_INT_TIMEOUT_TH / WL_MSEC_PER_SEC);
}

static void cps4057_rx_ready_handler(struct cps4057_dev_info *di)
{
	int wired_ch_state;

	wired_ch_state = wireless_charge_get_wired_channel_state();
	if (wired_ch_state == WIRED_CHANNEL_ON) {
		hwlog_err("rx_ready_handler: wired channel on, ignore\n");
		return;
	}

	hwlog_info("[rx_ready_handler] rx ready, goto wireless charging\n");
	di->g_val.rx_stop_chrg_flag = false;
	di->irq_cnt = 0;
	wired_chsw_set_wired_channel(WIRED_CHANNEL_ALL, WIRED_CHANNEL_CUTOFF);
	wlps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_ON);
	msleep(CHANNEL_SW_TIME);
	gpio_set_value(di->gpio_sleep_en, RX_SLEEP_EN_DISABLE);
	wlps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_OFF);
	wlc_rx_evt_notify(WLC_EVT_RX_READY, NULL);
}

static void cps4057_rx_power_on_handler(struct cps4057_dev_info *di)
{
	u8 rx_ss = 0; /* ss: signal strength */
	int pwr_flag = RX_PWR_ON_NOT_GOOD;
	int wired_ch_state;

	wired_ch_state = wireless_charge_get_wired_channel_state();
	if (wired_ch_state == WIRED_CHANNEL_ON) {
		hwlog_err("rx_power_on_handler: wired channel on, ignore\n");
		return;
	}

	cps4057_rx_abnormal_irq_handler(di);
	cps4057_read_byte(CPS4057_RX_SS_ADDR, &rx_ss);
	hwlog_info("[rx_power_on_handler] ss=%u\n", rx_ss);
	if ((rx_ss > di->rx_ss_good_lth) && (rx_ss <= CPS4057_RX_SS_MAX))
		pwr_flag = RX_PWR_ON_GOOD;
	wlc_rx_evt_notify(WLC_EVT_RX_PWR_ON, &pwr_flag);
}

static void cps4057_rx_mode_irq_recheck(struct cps4057_dev_info *di)
{
	int ret;
	u16 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[rx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = cps4057_rx_get_interrupt(&irq_val);
	if (ret)
		return;

	if (irq_val & CPS4057_RX_IRQ_READY)
		cps4057_rx_ready_handler(di);

	cps4057_rx_clear_irq(CPS4057_RX_IRQ_CLR_ALL);
}

static void cps4057_rx_fault_irq_handler(struct cps4057_dev_info *di)
{
	if (di->irq_val & CPS4057_RX_IRQ_OCP) {
		di->irq_val &= ~CPS4057_RX_IRQ_OCP;
		wlc_rx_evt_notify(WLC_EVT_RX_OCP, NULL);
	}

	if (di->irq_val & CPS4057_RX_IRQ_VRECT_OVP) {
		di->irq_val &= ~CPS4057_RX_IRQ_VRECT_OVP;
		wlc_rx_evt_notify(WLC_EVT_RX_OVP, NULL);
	}

	if (di->irq_val & CPS4057_RX_IRQ_MLDO_OVP) {
		di->irq_val &= ~CPS4057_RX_IRQ_MLDO_OVP;
		wlc_rx_evt_notify(WLC_EVT_RX_OVP, NULL);
	}

	if (di->irq_val & CPS4057_RX_IRQ_OTP) {
		di->irq_val &= ~CPS4057_RX_IRQ_OTP;
		wlc_rx_evt_notify(WLC_EVT_RX_OTP, NULL);
	}
}

void cps4057_rx_mode_irq_handler(struct cps4057_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = cps4057_rx_get_interrupt(&di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: read irq failed, clear\n");
		cps4057_rx_clear_irq(CPS4057_RX_IRQ_CLR_ALL);
		cps4057_rx_abnormal_irq_handler(di);
		goto rechk_irq;
	}

	cps4057_rx_clear_irq(di->irq_val);

	if (di->irq_val & CPS4057_RX_IRQ_POWER_ON) {
		di->irq_val &= ~CPS4057_RX_IRQ_POWER_ON;
		cps4057_rx_power_on_handler(di);
	}
	if (di->irq_val & CPS4057_RX_IRQ_READY) {
		di->irq_val &= ~CPS4057_RX_IRQ_READY;
		cps4057_rx_ready_handler(di);
	}
	if (di->irq_val & CPS4057_RX_IRQ_FSK_PKT)
		cps4057_rx_data_rcvd_handler(di);

	cps4057_rx_fault_irq_handler(di);

rechk_irq:
	cps4057_rx_mode_irq_recheck(di);
}

static void cps4057_rx_pmic_vbus_handler(bool vbus_state)
{
	int ret;
	int wired_ch_state;
	u16 irq_val = 0;
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di) {
		hwlog_err("pmic_vbus_handler: di null\n");
		return;
	}

	if (!vbus_state || !di->g_val.irq_abnormal_flag)
		return;

	wired_ch_state = wireless_charge_get_wired_channel_state();
	if (wired_ch_state == WIRED_CHANNEL_ON)
		return;

	if (!cps4057_rx_check_tx_exist())
		return;

	ret = cps4057_rx_get_interrupt(&irq_val);
	if (ret) {
		hwlog_err("pmic_vbus_handler: read irq failed, clear\n");
		return;
	}
	hwlog_info("[pmic_vbus_handler] irq_val=0x%x\n", irq_val);
	if (irq_val & CPS4057_RX_IRQ_READY) {
		cps4057_rx_clear_irq(CPS4057_RX_IRQ_CLR_ALL);
		cps4057_rx_ready_handler(di);
		di->irq_cnt = 0;
		di->g_val.irq_abnormal_flag = false;
		cps4057_enable_irq();
	}
}

void cps4057_rx_probe_check_tx_exist(void)
{
	struct cps4057_dev_info *di = NULL;

	cps4057_get_dev_info(&di);
	if (!di)
		return;

	if (cps4057_rx_check_tx_exist()) {
		cps4057_rx_clear_irq(CPS4057_RX_IRQ_CLR_ALL);
		hwlog_info("[rx_probe_check_tx_exist] rx exsit\n");
		cps4057_rx_ready_handler(di);
	} else {
		cps4057_sleep_enable(RX_SLEEP_EN_ENABLE);
	}
}

void cps4057_rx_shutdown_handler(void)
{
	wlps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_OFF);
	msleep(50); /* dalay 50ms for power off */
	(void)cps4057_rx_set_tx_vout(ADAPTER_5V * WL_MVOLT_PER_VOLT);
	(void)cps4057_rx_set_rx_vout(ADAPTER_5V * WL_MVOLT_PER_VOLT);
	cps4057_chip_enable(RX_EN_DISABLE);
	msleep(CPS4057_SHUTDOWN_SLEEP_TIME);
	cps4057_chip_enable(RX_EN_ENABLE);
}

static struct wireless_charge_device_ops g_cps4057_rx_ops = {
	.dev_node               = cps4057_dts_dev_node,
	.chip_init              = cps4057_rx_chip_init,
	.chip_reset             = cps4057_rx_chip_reset,
	.rx_enable              = cps4057_chip_enable,
	.rx_sleep_enable        = cps4057_sleep_enable,
	.ext_pwr_post_ctrl      = cps4057_rx_ext_pwr_post_ctrl,
	.get_chip_info          = cps4057_get_chip_info_str,
	.get_rx_chip_id         = cps4057_get_chip_id,
	.get_rx_def_imax        = cps4057_rx_get_def_imax,
	.get_rx_vrect           = cps4057_rx_get_vrect,
	.get_rx_vout            = cps4057_rx_get_vout,
	.get_rx_iout            = cps4057_rx_get_iout,
	.get_rx_vout_reg        = cps4057_rx_get_rx_vout_reg,
	.get_tx_vout_reg        = cps4057_rx_get_tx_vout_reg,
	.set_tx_vout            = cps4057_rx_set_tx_vout,
	.set_rx_vout            = cps4057_rx_set_rx_vout,
	.get_rx_fop             = cps4057_rx_get_fop,
	.get_rx_cep             = cps4057_rx_get_cep,
	.get_rx_temp            = cps4057_rx_get_temp,
	.get_rx_fod_coef        = cps4057_rx_get_fod,
	.set_rx_fod_coef        = cps4057_rx_set_fod,
	.check_tx_exist         = cps4057_rx_check_tx_exist,
	.kick_watchdog          = cps4057_rx_kick_watchdog,
	.send_ept               = cps4057_rx_send_ept,
	.stop_charging          = cps4057_rx_stop_charging,
	.pmic_vbus_handler      = cps4057_rx_pmic_vbus_handler,
	.check_fwupdate         = cps4057_fw_sram_update,
	.need_chk_pu_shell      = NULL,
	.set_pu_shell_flag      = NULL,
};

int cps4057_rx_ops_register(void)
{
	return wireless_charge_ops_register(&g_cps4057_rx_ops);
}
