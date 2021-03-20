#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/mfd/hisi_pmic.h>
#include <linux/power/hisi/hisi_bci_battery.h>
#include <linux/power/hisi/coul/hisi_coul_drv.h>
#include <huawei_platform/power/wireless_transmitter.h>
#include <huawei_platform/power/wired_channel_switch.h>
#include <linux/power/hisi/hisi_bci_battery.h>
#include <linux/jiffies.h>
#include <../charging_core.h>
#include <huawei_platform/power/wireless_power_supply.h>
#include <huawei_platform/power/hardware_ic/charge_pump.h>
#include <huawei_platform/power/hw_accessory.h>

#define HWLOG_TAG wireless_tx
HWLOG_REGIST();

static struct wakeup_source wireless_tx_wakelock;
static struct wireless_tx_device_info *g_wireless_tx_di;
static struct wireless_tx_device_ops *g_wireless_tx_ops;
static struct wltx_logic_ops *g_wltx_logic_ops[WLTX_OPEN_TYPE_END];
static enum wltx_stage g_tx_stage = WL_TX_STAGE_DEFAULT;
static enum wireless_tx_status_type tx_status = WL_TX_STATUS_DEFAULT;
static struct wltx_stage_info {
	enum wltx_stage tx_stg;
	char tx_stage_name[WL_TX_STR_LEN_32];
} const g_tx_stg[WL_TX_STAGE_TOTAL] = {
	{WL_TX_STAGE_DEFAULT,"STAGE_DEFAULT"},       {WL_TX_STAGE_POWER_SUPPLY,"STAGE_POWER_SUPPLY"},
	{WL_TX_STAGE_CHIP_INIT,"STAGE_CHIP_INIT"},   {WL_TX_STAGE_PING_RX,"STAGE_PING_RX"},
	{WL_TX_STAGE_REGULATION,"STAGE_REGULATION"},
};

static unsigned int tx_iin_samples[WL_TX_IIN_SAMPLE_LEN];
static unsigned int tx_fop_samples[WL_TX_FOP_SAMPLE_LEN];
static bool tx_open_flag = false; // record the UI operation state
static int tx_iin_limit[WL_TX_CHARGER_TYPE_MAX] = {0};
static int g_init_tbatt;

/* each dev should done */
struct key_info g_wl_acc_info_tab[WL_TX_ACC_INFO_MAX] = {
	{ "DEVICENO", "0" },
	{ "DEVICESTATE", "UNKNOWN" },
	{ "DEVICEMAC", "FF:FF:FF:FF:FF:FF" },
	{ "DEVICEMODELID", "000000" },
	{ "DEVICESUBMODELID", "00" },
	{ "DEVICEVERSION", "00" },
	{ "DEVICEBUSINESS", "00" },
};

BLOCKING_NOTIFIER_HEAD(tx_event_nh);
int wireless_tx_ops_register(struct wireless_tx_device_ops *tx_ops)
{
	int ret = 0;

	if (tx_ops != NULL) {
		g_wireless_tx_ops = tx_ops;
	} else {
		hwlog_err("wireless tx ops register fail!\n");
		ret = -EPERM;
	}
	return ret;
}

int wireless_tx_logic_ops_register(struct wltx_logic_ops *ops)
{
	if (!ops || (ops->type < WLTX_OPEN_TYPE_BEGIN) ||
		(ops->type >= WLTX_OPEN_TYPE_END))
		return -1;

	hwlog_info("[%s] type = %d\n", __func__, ops->type);
	if (g_wltx_logic_ops[ops->type])
		return -1;

	g_wltx_logic_ops[ops->type] = ops;

	return 0;
}

struct wireless_tx_device_info *wltx_get_dev_info(void)
{
	return g_wireless_tx_di;
}

int wireless_tx_get_tx_status(void)
{
	return tx_status;
}

bool wireless_tx_get_tx_open_flag(void)
{
	return tx_open_flag;
}

bool wltx_is_tx_open_by_hall(void)
{
	if (!g_wireless_tx_di)
		return false;

	return g_wireless_tx_di->tx_open_type == WLTX_OPEN_BY_HALL;
}

bool wireless_is_in_tx_mode(void)
{
	int i;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di || !di->tx_ops || !di->tx_ops->in_tx_mode) {
		hwlog_err("%s: di null\n", __func__);
		return false;
	}
	/* try 3 times for fault tolerance */
	for (i = 0; i < WL_TX_MODE_ERR_CNT1; i++) {
		if ((g_tx_stage > WL_TX_STAGE_CHIP_INIT) &&
			di->tx_ops->in_tx_mode())
			return true;
	}

	return false;
}

static void wireless_tx_wake_lock(void)
{
	if (!wireless_tx_wakelock.active) {
		__pm_stay_awake(&wireless_tx_wakelock);
		hwlog_info("wireless_tx wake lock\n");
	}
}

static void wireless_tx_wake_unlock(void)
{
	if (wireless_tx_wakelock.active) {
		__pm_relax(&wireless_tx_wakelock);
		hwlog_info("wireless_tx wake unlock\n");
	}
}

int wltx_msleep(int sleep_ms)
{
	int i;
	int interval = 25;  /* sleep interval, ms */
	int cnt = sleep_ms / interval;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return -1;
	}

	for (i = 0; i < cnt; i++) {
		msleep(interval);
		if (!tx_open_flag || di->stop_reverse_charge) {
			hwlog_err("%s: TX mode has stopped\n", __func__ );
			return -1;
		}
	}

	return 0;
}

static int wltx_check_abnormal_vbatt(void)
{
	int i;
	int vbatt;

	/* vbatt_th=3450mV, 20ms*3=60ms timeout for vbatt debouncing */
	for (i = 0; i < 3; i++) {
		vbatt = hisi_battery_voltage();
		if (tx_open_flag && (vbatt < 3450)) {
			hwlog_err("abnormal vbatt=%d\n", vbatt);
			msleep(20);
		} else {
			break;
		}
	}
	if (i >= 3) {
		tx_open_flag = false;
		return -EINVAL;
	}

	return 0;
}

static int wltx_check_abnormal_ibatt(void)
{
	int i;
	int idischrg;
	int idischrg_max;
	u8 tx_cap_type;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di)
		return -EINVAL;

	tx_cap_type = wltx_get_tx_cap_type();
	if ((tx_cap_type == WIRELESS_FCP) ||
		(di->tx_vset.para[di->tx_vset.cur].ext_hdl == WLTX_EXT_HDL_BP2CP))
		idischrg_max = WLTX_BP2CP_ABNORMAL_IBAT;
	else
		idischrg_max = WLTX_ABNORMAL_IBAT;

	/* 20ms*3=60ms timeout for ibatt debouncing */
	for (i = 0; i < 3; i++) {
		idischrg = hisi_battery_current();
		if (tx_open_flag && (idischrg > idischrg_max)) {
			hwlog_err("abnormal idischrg=%d\n", idischrg);
			msleep(20);
		} else {
			break;
		}
	}
	if (i >= 3) {
		tx_open_flag = false;
		return -EINVAL;
	}

	return 0;
}

static void wltx_chk_abnormal_tx_vin(void)
{
	int i;
	u16 tx_vin = 0;
	const char *src = NULL;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di || !di->tx_ops || !di->tx_ops->get_tx_vin)
		return;

	src = wltx_get_pwr_src_name(di->cur_pwr_src);
	if (!strstr(src, "CP"))
		return;

	if (tx_status != WL_TX_STATUS_IN_CHARGING)
		return;

	/* tx_vin_th=7500mV, 20ms*3=60ms timeout for tx_vin debouncing */
	for (i = 0; i < 3; i++) {
		(void)di->tx_ops->get_tx_vin(&tx_vin);
		if (tx_open_flag && (tx_vin < 7500)) {
			hwlog_err("abnormal tx_vin=%u\n", tx_vin);
			msleep(20);
		} else {
			break;
		}
	}
	if (i >= 3) {
		wireless_tx_set_tx_status(WL_TX_STATUS_RX_DISCONNECT);
		di->stop_reverse_charge = true;
	}
}

static void wltx_check_abnormal_power(void)
{
	if (wltx_check_abnormal_vbatt())
		return;

	if (wltx_check_abnormal_ibatt())
		return;

	(void)wltx_chk_abnormal_tx_vin();
}

static void wireless_tx_set_tx_open_flag(bool enable)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (di && di->tx_ops && di->tx_ops->set_tx_open_flag)
		di->tx_ops->set_tx_open_flag(enable);
	tx_open_flag = enable;
	hwlog_info("[%s] set tx_open_flag = %d\n", __func__, tx_open_flag);

	if (!enable)
		direct_charge_set_disable_flags(DC_CLEAR_DISABLE_FLAGS,
			DC_DISABLE_WIRELESS_TX);
}

static void wireless_tx_set_stage(enum wltx_stage stage)
{
	g_tx_stage = stage;
	hwlog_info("[set_stage] %s\n", g_tx_stg[g_tx_stage].tx_stage_name);
}

enum wltx_stage wireless_tx_get_stage(void)
{
	return g_tx_stage;
}

void wireless_tx_set_tx_status(enum wireless_tx_status_type event)
{
	tx_status = event;
	hwlog_info("[%s] 0x%02x\n", __func__, tx_status);
}

int wltx_get_tx_iin(u16 *tx_iin)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!tx_iin || !di || !di->tx_ops || !di->tx_ops->get_tx_iin)
		return -EINVAL;

	return di->tx_ops->get_tx_iin(tx_iin);
}

int wltx_get_tx_ilimit(int charger_type, int ibuck)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di || !tx_open_flag ||
		(wireless_tx_get_stage() == WL_TX_STAGE_DEFAULT) ||
		(charger_type >= WL_TX_CHARGER_TYPE_MAX) || (charger_type < 0))
		return 0;

	if (di->cur_pwr_sw_scn == PWR_SW_BY_DC_DONE)
		return di->tx_dc_done_buck_ilim;

	return ibuck - tx_iin_limit[charger_type];
}

static void wltx_calc_tx_fop_avg(struct wireless_tx_device_info *di,
	unsigned int tx_fop)
{
	int i;
	int fop_sum = 0;
	static int index;

	tx_fop_samples[index] = tx_fop;
	index = (index + 1) % WL_TX_FOP_SAMPLE_LEN; /* 1:next sample */
	for (i = 0; i < WL_TX_FOP_SAMPLE_LEN; i++)
		fop_sum += tx_fop_samples[i];

	di->tx_fop_avg = fop_sum / WL_TX_FOP_SAMPLE_LEN;
}

static void wltx_reset_avg_tx_fop(struct wireless_tx_device_info *di)
{
	int i = 0;

	for (; i < WL_TX_FOP_SAMPLE_LEN; i++)
		tx_fop_samples[i] = 0;

	di->tx_fop_avg = 0;
}

static void wireless_tx_calc_tx_iin_avg(struct wireless_tx_device_info *di, unsigned int tx_iin)
{
	static int index = 0;
	int iin_sum = 0;
	int i;

	tx_iin_samples[index] = tx_iin;
	index = (index + 1) % WL_TX_IIN_SAMPLE_LEN;
	for (i = 0; i < WL_TX_IIN_SAMPLE_LEN; i++)
		iin_sum += tx_iin_samples[i];
	di->tx_iin_avg = iin_sum/WL_TX_IIN_SAMPLE_LEN;
}

static void wltx_set_rp_dm_timeout_val(struct wireless_tx_device_info *di)
{
	if (!di->tx_ops->set_rp_dm_timeout_val)
		return;

	di->tx_ops->set_rp_dm_timeout_val(WLTX_RP_DEMODULE_TIMEOUT_VAL);
}

static void wireless_tx_reset_avg_iout(struct wireless_tx_device_info *di)
{
	int i;

	for (i = 0; i < WL_TX_IIN_SAMPLE_LEN; i++)
		tx_iin_samples[i] = 0;
	di->tx_iin_avg = 0;
}

static void wltx_reset_rx_data(struct wireless_tx_device_info *di)
{
	di->rx_data.pmax = 0;
	di->rx_data.product_type = QI_RX_TYPE_UNKNOWN;
}

static int wltx_check_handshake(struct wireless_tx_device_info *di)
{
	if ((tx_status < WL_TX_STATUS_PING_SUCC) ||
		(tx_status >= WL_TX_STATUS_FAULT_BASE))
		return -1;

	return 0;
}

static int wireless_tx_chip_reset(struct wireless_tx_device_info *di)
{
	return di->tx_ops->chip_reset();
}
static void wireless_tx_set_max_fop(struct wireless_tx_device_info *di, u16 fop)
{
	di->tx_ops->set_tx_max_fop(fop);
}
static void wireless_tx_get_max_fop(struct wireless_tx_device_info *di, u16 *fop)
{
	di->tx_ops->get_tx_max_fop(fop);
}
static int wireless_tx_set_min_fop(struct wireless_tx_device_info *di, u16 fop)
{
	return di->tx_ops->set_tx_min_fop(fop);
}
static void wireless_tx_get_min_fop(struct wireless_tx_device_info *di, u16 *fop)
{
	di->tx_ops->get_tx_min_fop(fop);
}
static void wireless_tx_get_fop(struct wireless_tx_device_info *di, u16 *fop)
{
	di->tx_ops->get_tx_fop(fop);
}
static void wireless_tx_get_digital_ping_frequency(struct wireless_tx_device_info *di, u16 *dping_freq)
{
	di->tx_ops->get_tx_ping_frequency(dping_freq);
}
static void wireless_tx_set_digital_ping_frequency(struct wireless_tx_device_info *di, u16 dping_freq)
{
	di->tx_ops->set_tx_ping_frequency(dping_freq);
}
static void wireless_tx_get_digital_ping_interval(struct wireless_tx_device_info *di, u16 *dping_interval)
{
	di->tx_ops->get_tx_ping_interval(dping_interval);
}
static void wireless_tx_set_digital_ping_interval(struct wireless_tx_device_info *di, u16 dping_interval)
{
	di->tx_ops->set_tx_ping_interval(dping_interval);
}
static void wireless_tx_check_fwupdate(struct wireless_tx_device_info *di)
{
	di->tx_ops->check_fwupdate(WIRELESS_TX_MODE);
}
static void wireless_tx_dsm_dump(struct wireless_tx_device_info *di, char* dsm_buff)
{
	int ret, i;
	char buff[ERR_NO_STRING_SIZE] = {0};
	u16 tx_iin = 0;
	u16 tx_vin = 0;
	u16 tx_vrect = 0;
	u8 chip_temp = 0;
	int soc = hisi_battery_capacity();
	int tbatt = 0;
	int charger_vbus = charge_get_vbus();

	huawei_battery_temp(BAT_TEMP_MIXED, &tbatt);
	ret = di->tx_ops->get_tx_iin(&tx_iin);
	ret |= di->tx_ops->get_tx_vin(&tx_vin);
	ret |= di->tx_ops->get_tx_vrect(&tx_vrect);
	ret |= di->tx_ops->get_chip_temp(&chip_temp);
	if (ret)
		hwlog_err("%s: get tx vin/iin/vrect... fail", __func__);
	snprintf(buff, sizeof(buff),
		"soc = %d, tbatt = %d, init_tbatt = %d, charger_vbus = %dmV, tx_vrect = %umV, tx_vin = %umV, tx_iin = %umA, tx_iin_avg = %dmA, chip_temp = %u\n",
		soc, tbatt, g_init_tbatt, charger_vbus, tx_vrect, tx_vin, tx_iin, di->tx_iin_avg, chip_temp);
	strncat(dsm_buff, buff, strlen(buff));
	snprintf(buff, ERR_NO_STRING_SIZE, "tx_iin(mA): ");
	strncat(dsm_buff, buff, strlen(buff));
	for (i = 0; i < WL_TX_IIN_SAMPLE_LEN; i++) {
		snprintf(buff, ERR_NO_STRING_SIZE, "%d ", tx_iin_samples[i]);
		strncat(dsm_buff, buff, strlen(buff));
	}
}

static void wireless_tx_dsm_report(int err_no, char* dsm_buff)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;
	if (di) {
		wireless_tx_dsm_dump(di, dsm_buff);
		power_dsm_dmd_report(POWER_DSM_BATTERY, err_no, dsm_buff);
	}
}

static void wireless_tx_check_rx_disconnect(struct wireless_tx_device_info *di)
{
	bool rx_disconnect = di->tx_ops->check_rx_disconnect();
	if (rx_disconnect) {
		hwlog_info("[%s] rx disconnect!\n",__func__);
		wireless_tx_set_tx_status(WL_TX_STATUS_RX_DISCONNECT);
		di->stop_reverse_charge = true;
	}
}
static void wireless_tx_en_enable(struct wireless_tx_device_info *di, bool enable)
{
	di->tx_ops->rx_enable(enable);
}

static void wireless_tx_mode_enable(struct wireless_tx_device_info *di, bool enable)
{
	di->tx_ops->enable_tx_mode(enable);
}

static void wireless_tx_enable_tx_mode(struct wireless_tx_device_info *di, bool enable)
{
	if (WIRED_CHANNEL_OFF == wireless_charge_get_wired_channel_state()) {
		wireless_tx_mode_enable(di, enable);
		hwlog_info("[%s] enable = %d, wired channel state:off\n",__func__, enable);
	} else {
		/* If enable equals false in wired charging, chip should be closed */
		wireless_tx_en_enable(di, enable);
		if (enable)
			wireless_tx_mode_enable(di, enable);
		hwlog_info("[%s] enable = %d, wired channel state:on\n",__func__, enable);
	}
}

void wltx_reset_reverse_charging(void)
{
	if (tx_open_flag) {
		wireless_tx_set_tx_open_flag(false);
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		wltx_disable_pwr_supply();
		hisi_coul_charger_event_rcv(WIRELESS_TX_STATUS_CHANGED);
	}
}

static void wltx_ps_get_tx_vin(struct wireless_tx_device_info *di, u16 *vin)
{
	u16 tx_vin = 0;

	if ((di->cur_pwr_src == PWR_SRC_VBUS) ||
		(di->cur_pwr_src == PWR_SRC_OTG)) {
		tx_vin = charge_get_vbus();
	} else {
		wireless_tx_en_enable(di, true);
		if (di->tx_ops->get_tx_vin(&tx_vin)) {
			hwlog_err("ps_get_tx_vin: failed\n");
			tx_vin = 0;
		}
	}

	*vin = tx_vin;
}

static void wireless_tx_switch_off(void)
{
	wlps_control(WLPS_TX_SW, WLPS_CTRL_OFF);
	wlps_control(WLPS_RX_SW_AUX, WLPS_CTRL_OFF);
	charge_pump_chip_enable(CP_TYPE_AUX, false);
	wlps_control(WLPS_RX_SW, WLPS_CTRL_OFF);
	charge_pump_chip_enable(CP_TYPE_MAIN, false);
}

static int wireless_tx_power_supply(struct wireless_tx_device_info *di)
{
	int ret;
	int count = 0;
	u16 tx_vin = 0;
	int charger_vbus;
	char dsm_buff[POWER_DSM_BUF_SIZE_0512] = {0};

	wireless_tx_switch_off();
	ret = wltx_enable_pwr_supply();
	if (ret)
		return ret;

	wltx_set_tx_volt(WL_TX_STAGE_POWER_SUPPLY, di->tx_vset.v_ps, true);
	do {
		msleep(WL_TX_VIN_SLEEP_TIME);
		wltx_check_abnormal_power();
		wltx_ps_get_tx_vin(di, &tx_vin);
		charger_vbus = charge_get_vbus();
		if ((tx_vin >= di->tx_vset.para[di->tx_vset.cur].lth) &&
			(tx_vin <= di->tx_vset.para[di->tx_vset.cur].hth)) {
			hwlog_info("[%s] tx_vin = %dmV, charger_vbus = %dmV, "
				"power supply succ\n", __func__, tx_vin, charger_vbus);
			wireless_tx_en_enable(di, true);
			return WL_TX_SUCC;
		}
		if (!tx_open_flag || di->stop_reverse_charge) {
			hwlog_err("power_supply: stop, tx_open=%d, stop=%d\n",
				tx_open_flag, di->stop_reverse_charge);
			return WL_TX_FAIL;
		}
		count++;
		if (count == WL_TX_VIN_RETRY_CNT1) {
			wltx_enable_extra_pwr_supply();
			wltx_set_tx_volt(WL_TX_STAGE_POWER_SUPPLY, di->tx_vset.v_ps, true);
		}
		hwlog_info("[%s] tx_vin = %dmV, charger_vbus = %dmV, "
			"retry times = %d!\n", __func__, tx_vin, charger_vbus, count);
	} while (count < WL_TX_VIN_RETRY_CNT2);

	hwlog_err("%s: power supply for TX fail!\n", __func__);
	wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
	wireless_tx_set_tx_open_flag(false);
	wireless_tx_dsm_report(ERROR_WIRELESS_TX_POWER_SUPPLY_FAIL, dsm_buff);
	return WL_TX_FAIL;
}

static int wireless_tx_limit_iout(struct wireless_tx_device_info *di)
{
	int ret;
	u8 tx_cap_type;
	int tx_ilimit;

	if ((di->cur_pwr_sw_scn == PWR_SW_BY_OTG_ON)
		&& (di->cur_pwr_src == PWR_SRC_5VBST)) {
		ret = wireless_tx_set_min_fop(di, WL_TX_5VBST_MIN_FOP);
		if (ret) {
			hwlog_err("%s: set_min_fop fail\n", __func__);
			return -1;
		}
	}

	if ((di && di->logic_ops && di->logic_ops->need_reinit_tx_para &&
		di->logic_ops->need_reinit_tx_para()) ||
		(di->tx_open_type == WLTX_OPEN_BY_LIGHTSTRAP)) {
		wireless_tx_set_digital_ping_frequency(di, WL_TX_LIGHTSTRAP_PING_FOP);
		wireless_tx_set_min_fop(di, WL_TX_LIGHTSTRAP_FOP);
		wireless_tx_set_max_fop(di, WL_TX_LIGHTSTRAP_FOP);
	}

	tx_cap_type = wltx_get_tx_cap_type();
	if (tx_cap_type == WIRELESS_FCP)
		tx_ilimit = WLTX_BP2CP_PWR_ILIM;
	else
		tx_ilimit = WLTX_HI_PWR_ILIM;

	if (di->tx_ops && di->tx_ops->set_tx_ilimit) {
		ret = di->tx_ops->set_tx_ilimit(tx_ilimit);
		if (ret) {
			hwlog_err("%s: set tx_ilimit fail\n", __func__);
			return ret;
		}
	}

	return 0;
}

static void wireless_tx_para_init(struct wireless_tx_device_info *di)
{
	di->stop_reverse_charge = false;
	di->i2c_err_cnt = 0;
	di->tx_iin_low_cnt = 0;
	di->tx_mode_err_cnt = 0;
	di->standard_rx = false;
	di->tx_pd_flag = false;
	di->tx_cap.cur_id = WLTX_DFLT_CAP;
	di->hp_time_out = 0;
	di->irq_vset_time_out = 0;
	di->tx_rp_timeout_lim_volt = 0;
	di->monitor_interval = WL_TX_MONITOR_INTERVAL;
	wltx_reset_rx_data(di);
	wireless_tx_reset_avg_iout(di);
	wltx_reset_avg_tx_fop(di);
	wltx_reset_vset_para();
}

static int wireless_tx_chip_init(struct wireless_tx_device_info *di)
{
	int ret;

	if (!tx_open_flag || di->stop_reverse_charge) {
		hwlog_err("%s: Tx mode has already stop! tx_open_flag = %d, stop_reverse_charge_flag = %d\n",
				__func__ , tx_open_flag, di->stop_reverse_charge);
		return WL_TX_FAIL;
	}

	if (di->cur_pwr_src == PWR_SRC_SPBST)
		wlps_control(WLPS_TX_SW, WLPS_CTRL_ON);
	wireless_tx_check_fwupdate(di);
	ret = di->tx_ops->tx_chip_init(di->tx_open_type);
	if (ret) {
		hwlog_err("%s: TX chip init fail!\n", __func__);
		return WL_TX_FAIL;
	}
	wltx_set_tx_volt(WL_TX_STAGE_CHIP_INIT, di->tx_vset.v_ping, true);
	ret = wireless_tx_limit_iout(di);
	if (ret) {
		hwlog_err("%s: limit TX iout fail!\n", __func__);
		return WL_TX_FAIL;
	}
	wltx_set_rp_dm_timeout_val(di);
	hwlog_info("%s: TX chip init succ!\n", __func__);
	return WL_TX_SUCC;
}
static void wireless_tx_check_in_tx_mode(struct wireless_tx_device_info *di)
{
	int ret;
	bool in_tx_mode = di->tx_ops->in_tx_mode();
	if (!in_tx_mode) {
		if (++di->tx_mode_err_cnt >= WL_TX_MODE_ERR_CNT2) {
			hwlog_err("%s: not in tx mode, close TX!\n",__func__);
			wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
			di->stop_reverse_charge = true;
		} else if (di->tx_mode_err_cnt >= WL_TX_MODE_ERR_CNT) {
			hwlog_err("%s: not in tx mode, reinit TX!\n",__func__);
			ret = wireless_tx_chip_init(di);
			if (ret)
				hwlog_err("%s: chip_init fail\n", __func__);
			wireless_tx_enable_tx_mode(di, true);
		}
	} else {
		di->tx_mode_err_cnt = 0;
	}
}

static bool wireless_is_in_rx_mode(struct wireless_tx_device_info *di)
{
	if (di && di->tx_ops && di->tx_ops->in_rx_mode)
		return di->tx_ops->in_rx_mode();

	return false;
}

static int wireless_tx_ping_rx(struct wireless_tx_device_info *di)
{
	u16 tx_vin = 0;
	int ret;
	bool tx_vin_uvp_flag = false;
	int tx_vin_uvp_cnt = 0;
	int tx_vin_ovp_cnt = 0;
	char dsm_buff[POWER_DSM_BUF_SIZE_0256] = {0};
	struct timespec64 ts64_timeout;
	struct timespec64 ts64_interval;
	struct timespec64 ts64_now;
	ts64_now = current_kernel_time64();
	ts64_interval.tv_sec = di->ping_timeout;
	ts64_interval.tv_nsec = 0;

	if (di->ping_timeout == WL_TX_PING_TIMEOUT_2) {
		ret = wireless_tx_chip_reset(di);
		if (ret)
			hwlog_err("%s: chip_reset fail\n", __func__);
		msleep(150); /* only used here */
		ret = wireless_tx_chip_init(di);
		if (ret)
			hwlog_err("%s: chip_init fail\n", __func__);
	}

	wltx_set_tx_volt(WL_TX_STAGE_PING_RX, di->tx_vset.v_ping, true);
	wireless_tx_enable_tx_mode(di, true);

	ts64_timeout = timespec64_add_safe(ts64_now, ts64_interval);
	if (ts64_timeout.tv_sec == TIME_T_MAX) {
		hwlog_err("%s: time overflow happend, TX ping RX fail!\n", __func__);
		return WL_TX_FAIL;
	}

	wireless_tx_set_tx_status(WL_TX_STATUS_PING);
	hisi_coul_charger_event_rcv(WIRELESS_TX_STATUS_CHANGED);
	while (timespec64_compare(&ts64_now, &ts64_timeout) < 0) {
		/* wait for config packet interrupt */
		if (WL_TX_STATUS_PING_SUCC == wireless_tx_get_tx_status()) {
			return WL_TX_SUCC;
		}
		wltx_check_abnormal_power();
		ret = di->tx_ops->get_tx_vin(&tx_vin);
		if (ret) {
			hwlog_err("[%s] get tx_vin fail\n", __func__);
			wireless_tx_enable_tx_mode(di, true);
			tx_vin = 0;
		} else if (!tx_vin) {
			tx_vin = di->tx_vset.para[di->tx_vset.cur].vset;
		} else if ((tx_vin < di->tx_vset.para[di->tx_vset.cur].lth) ||
			(tx_vin >= di->tx_vset.para[di->tx_vset.cur].hth)) {
			hwlog_err("%s: tx_vin = %umV\n", __func__, tx_vin);
		}
		/*
		 * to solve the problem of tx reset when power_supply ocp/scp
		 * in case of putting tx on the metal or something like this
		 */
		if (tx_vin < di->tx_vset.para[di->tx_vset.cur].lth) {
			tx_vin_uvp_flag = true;
		} else if (tx_vin >= di->tx_vset.para[di->tx_vset.cur].lth) {
			if (tx_vin_uvp_flag && ++tx_vin_uvp_cnt <= WL_TX_PING_VIN_UVP_CNT) {
				hwlog_err("%s: tx vin uvp cnt = %d\n", __func__, tx_vin_uvp_cnt);
				ret = wireless_tx_chip_init(di);
				if (ret) {
					hwlog_err("%s: tx_chip_init fail\n", __func__);
					wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
					wireless_tx_set_tx_open_flag(false);
					return WL_TX_FAIL;
				}
				wireless_tx_enable_tx_mode(di, true);
			}
			tx_vin_uvp_flag = false;
		}
		if (tx_vin_uvp_cnt >= WL_TX_PING_VIN_UVP_CNT) {
			hwlog_err("%s: tx_vin_uvp over %d times, tx ping fail\n", __func__, WL_TX_PING_VIN_UVP_CNT);
			wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
			wireless_tx_set_tx_open_flag(false);
			return WL_TX_FAIL;
		}

		/* to enter RX mode when put TRX(phone) on TX in case of TRX TX mode on */
		if (wired_chsw_get_wired_channel(WIRED_CHANNEL_ALL) == WIRED_CHANNEL_CUTOFF &&
			tx_vin >= di->tx_vset.para[di->tx_vset.cur].hth) {
			if (++tx_vin_ovp_cnt >= WL_TX_PING_VIN_OVP_CNT) {
				hwlog_err("%s: tx_vin over %dmV for %dms, stop TX power supply to reset TRX chip\n",
					__func__, di->tx_vset.para[di->tx_vset.cur].hth,
					tx_vin_ovp_cnt * WL_TX_PING_CHECK_INTERVAL);
				wltx_disable_pwr_supply();
				wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
				wireless_tx_set_tx_open_flag(false);
				return WL_TX_FAIL;
			}
		} else {
			tx_vin_ovp_cnt = 0;
		}
		msleep(WL_TX_PING_CHECK_INTERVAL);
		if (wireless_charge_get_wired_channel_state() == WIRED_CHANNEL_OFF)
			ts64_now = current_kernel_time64();

		if (!tx_open_flag || di->stop_reverse_charge) {
			hwlog_err("%s: Tx mode has already stop! tx_open_flag = %d, stop_reverse_charge_flag = %d\n",
				__func__ , tx_open_flag, di->stop_reverse_charge);
			return WL_TX_FAIL;
		}
	}
	wireless_tx_set_tx_open_flag(false);
	wireless_tx_set_tx_status(WL_TX_STATUS_PING_TIMEOUT);
	hwlog_err("[%s] TX ping RX timeout!!\n",__func__);
	if (di->ping_timeout == WL_TX_PING_TIMEOUT_1) {
		snprintf(dsm_buff, sizeof(dsm_buff), "TX ping RX timeout\n");
		power_dsm_dmd_report(POWER_DSM_BATTERY, ERROR_WIRELESS_TX_STATISTICS, dsm_buff);
	}
	return WL_TX_FAIL;
}
static int wireless_tx_can_do_reverse_charging(void)
{
	int batt_temp = 0;
	char dsm_buff[POWER_DSM_BUF_SIZE_0512] = {0};
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	huawei_battery_temp(BAT_TEMP_MIXED, &batt_temp);
	if (batt_temp <= WL_TX_BATT_TEMP_MIN) {
		hwlog_info("[%s] battery temperature %d is too low th: %d\n",
			__func__, batt_temp, WL_TX_BATT_TEMP_MIN);
		wireless_tx_set_tx_status(WL_TX_STATUS_TBATT_LOW);
		return WL_TX_FAIL;
	}
	if (batt_temp >= WL_TX_BATT_TEMP_MAX) {
		hwlog_info("[%s] battery temperature %d is too high th: %d\n",
			__func__, batt_temp, WL_TX_BATT_TEMP_MAX);
		wireless_tx_set_tx_status(WL_TX_STATUS_TBATT_HIGH);
		if (batt_temp - g_init_tbatt > WL_TX_TBATT_DELTA_TH)
			wireless_tx_dsm_report(ERROR_WIRELESS_TX_BATTERY_OVERHEAT, dsm_buff);
		return WL_TX_FAIL;
	}
	if (wireless_is_in_rx_mode(di)) {
		hwlog_info("[%s] in wireless charging, can not enable tx mode\n",__func__);
		wireless_tx_set_tx_status(WL_TX_STATUS_IN_WL_CHARGING);
		return WL_TX_FAIL;
	} else {
		wlc_reset_wireless_charge();
	}

	if (di && di->logic_ops && di->logic_ops->can_rev_charge_check &&
		!di->logic_ops->can_rev_charge_check())
		return WL_TX_FAIL;

	return WL_TX_SUCC;
}

static int wltx_get_rx_pmax(struct wireless_tx_device_info *di, u16 *rx_pmax)
{
	if (di->rx_data.pmax > 0) {
		*rx_pmax = di->rx_data.pmax;
	} else if (di->rx_data.product_type == QI_RX_TYPE_EARPHONE_BOX) {
		*rx_pmax = QI_EARPHONE_BOX_MIN_PWR;
	} else {
		*rx_pmax = WLTX_DEFAULT_RX_PMAX;
		return -EINVAL;
	}

	return 0;
}

static bool wltx_rx_disconnect_need_power_off(struct wireless_tx_device_info *di)
{
	u16 rx_pmax = 0;
	int tx_vset = di->tx_vset.para[di->tx_vset.cur].vset;

	if (tx_vset <= WLTX_RX_DISC_PWR_OFF_VTH) {
		di->rx_disc_pwr_on = true;
		hwlog_info("[rx_disc_need_power_off] tx_vset=%dmV\n", tx_vset);
		return false;
	}
	if (wltx_get_rx_pmax(di, &rx_pmax))
		return true;

	if (rx_pmax <= WLTX_RX_DISC_PWR_OFF_PTH) {
		di->rx_disc_pwr_on = true;
		hwlog_info("[rx_disc_need_power_off] rx_pmax=0x%x\n", rx_pmax);
		return false;
	}

	return true;
}

static void wltx_rx_disconnect_handler(struct wireless_tx_device_info *di)
{
	wltx_reset_exp_tx_cap_id();
	di->ping_timeout = WL_TX_PING_TIMEOUT_2;
	if (wltx_rx_disconnect_need_power_off(di)) {
		/* power off to reset tx,in case tx runs err, ocp */
		wltx_disable_pwr_supply();
		usleep_range(9500, 10500); /* wait 10ms for tx power down */
		wireless_tx_set_stage(WL_TX_STAGE_POWER_SUPPLY);
	} else {
		wireless_tx_set_stage(WL_TX_STAGE_PING_RX);
	}
	schedule_work(&di->wireless_tx_check_work);
}

static void wireless_tx_fault_event_handler(struct wireless_tx_device_info *di)
{
	wireless_tx_wake_lock();
	hisi_coul_charger_event_rcv(WIRELESS_TX_STATUS_CHANGED);
	hwlog_info("[%s] tx_status = 0x%02x\n", __func__, tx_status);

	switch(tx_status) {
	case WL_TX_STATUS_RX_DISCONNECT:
		wltx_rx_disconnect_handler(di);
		break;
	case WL_TX_STATUS_TX_CLOSE:
	case WL_TX_STATUS_SOC_ERROR:
	case WL_TX_STATUS_TBATT_HIGH:
	case WL_TX_STATUS_TBATT_LOW:
	case WL_TX_STATUS_CHARGE_DONE:
	case WL_TX_STATUS_IN_WL_CHARGING:
		wireless_tx_set_tx_open_flag(false);
		wireless_tx_enable_tx_mode(di, false);
		power_event_notify(POWER_EVENT_NE_WIRELESS_REVERSE_STOP, NULL);
		wltx_disable_pwr_supply();
		wireless_tx_wake_unlock();
		break;
	default:
		wireless_tx_wake_unlock();
		hwlog_err("%s: has no this tx_status %d\n", __func__, tx_status);
		break;
	}

	if (di->tx_pd_flag) {
		di->tx_pd_flag = false;
		if (tx_open_flag &&
			(tx_status != WL_TX_STATUS_RX_DISCONNECT)) {
			wireless_tx_set_stage(WL_TX_STAGE_POWER_SUPPLY);
			schedule_work(&di->wireless_tx_check_work);
		}
	}
}

void wltx_cancle_work_handler(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	hwlog_info("[cancle_work_handler] start\n");
	wireless_tx_limit_iout(di);
	di->stop_reverse_charge = true;
	cancel_work_sync(&di->wireless_tx_check_work);
	cancel_delayed_work_sync(&di->wireless_tx_monitor_work);
	wireless_tx_enable_tx_mode(di, false);
	wltx_disable_pwr_supply();
	hwlog_info("[cancle_work_handler] end\n");
}

void wltx_restart_work_handler(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	hwlog_info("[restart_work_handler] start\n");
	wireless_tx_limit_iout(di);
	wireless_tx_set_stage(WL_TX_STAGE_DEFAULT);
	schedule_work(&di->wireless_tx_check_work);
}

static void wireless_tx_iout_control(struct wireless_tx_device_info *di)
{
	int ret;
	u16 tx_iin = 0;
	u16 tx_vin = 0;
	u16 tx_vrect = 0;
	u16 tx_fop = 0;
	u8 chip_temp = 0;
	static int log_cnt = 0;

	ret = di->tx_ops->get_tx_iin(&tx_iin);
	ret |= di->tx_ops->get_tx_vin(&tx_vin);
	ret |= di->tx_ops->get_tx_vrect(&tx_vrect);
	ret |= di->tx_ops->get_chip_temp(&chip_temp);
	ret |= di->tx_ops->get_tx_fop(&tx_fop);
	if (ret) {
		di->i2c_err_cnt++;
		hwlog_err("%s: get tx vin/iin fail", __func__);
	}
	if (di->standard_rx == false && tx_iin <= WL_TX_IIN_LOW &&
		WIRED_CHANNEL_OFF == wireless_charge_get_wired_channel_state()) {
		if (++di->tx_iin_low_cnt >= WL_TX_IIN_LOW_CNT/di->monitor_interval) {
			di->tx_iin_low_cnt = WL_TX_IIN_LOW_CNT;
			wireless_tx_set_tx_status(WL_TX_STATUS_CHARGE_DONE);
			di->stop_reverse_charge = true;
			hwlog_info("[%s] tx_iin below for %ds, set tx_status to charge_done\n",
				__func__, WL_TX_IIN_LOW_CNT/WL_MSEC_PER_SEC);
		}
	} else {
		di->tx_iin_low_cnt = 0;
	}
	wltx_calc_tx_fop_avg(di, tx_fop);
	wireless_tx_calc_tx_iin_avg(di, tx_iin);
	if (log_cnt++ == WL_TX_MONITOR_LOG_INTERVAL/di->monitor_interval) {
		hwlog_info("[%s] tx_fop = %ukHZ, tx_fop_avg =%ukHZ\t"
			"tx_iin = %dmA, tx_iin_avg = %dmA\t"
			"tx_vin = %dmV, tx_vrect = %dmV, chip_temp = %d\n",
			__func__, tx_fop, di->tx_fop_avg, tx_iin,
			di->tx_iin_avg, tx_vin, tx_vrect, chip_temp);
		log_cnt = 0;
	}
}

static int wltx_update_vset_by_soc(struct wireless_tx_device_info *di)
{
	int soc = hisi_bci_show_capacity();

	if (soc <= WLTX_TX_VSET_SOC_TH1)
		return WLTX_TX_VSET_5V;
	if (soc >= WLTX_TX_VSET_SOC_TH2)
		return di->tx_vset.v_dflt;
	return di->tx_vset.para[di->tx_vset.cur].vset;
}

static int wltx_update_vset_by_tbat(struct wireless_tx_device_info *di)
{
	int tbatt = 0;

	huawei_battery_temp(BAT_TEMP_MIXED, &tbatt);
	if (tbatt >= di->tx_vset_tbat_high)
		return WLTX_TX_VSET_5V;
	if (tbatt <= di->tx_vset_tbat_low)
		return di->tx_vset.v_dflt;
	return di->tx_vset.para[di->tx_vset.cur].vset;
}

static int wltx_update_vset_by_fop(struct wireless_tx_device_info *di)
{
	if ((di->tx_fop_avg >= WLTX_TX_VSET_FOP_TH2) &&
		(di->tx_iin_avg <= WLTX_TX_VSET_IIN_TH1))
		return WLTX_TX_VSET_5V;

	if ((di->tx_fop_avg <= WLTX_TX_VSET_FOP_TH1) &&
		(di->tx_iin_avg >= WLTX_TX_VSET_IIN_TH2))
		return di->tx_vset.v_dflt;

	return di->tx_vset.para[di->tx_vset.cur].vset;
}

static void wltx_ps_tx_vset_check(struct wireless_tx_device_info *di)
{
	int tx_vset_by_fop;
	int tx_vset_by_soc;
	int tx_vset_by_tbat;
	int tx_vset = di->tx_vset.max_vset;
	u16 rx_pmax = 0;

	if (di->tx_cap.cur_id == WLTX_HIGH_PWR_CAP)
		return;

	if (!time_after(jiffies, di->irq_vset_time_out))
		return;

	tx_vset_by_fop = wltx_update_vset_by_fop(di);
	if (tx_vset > tx_vset_by_fop)
		tx_vset = tx_vset_by_fop;

	tx_vset_by_soc = wltx_update_vset_by_soc(di);
	if (tx_vset > tx_vset_by_soc)
		tx_vset = tx_vset_by_soc;

	tx_vset_by_tbat = wltx_update_vset_by_tbat(di);
	if (tx_vset > tx_vset_by_tbat)
		tx_vset = tx_vset_by_tbat;

	if ((di->tx_rp_timeout_lim_volt > 0) &&
		(tx_vset > di->tx_rp_timeout_lim_volt))
		tx_vset = di->tx_rp_timeout_lim_volt;

	(void)wltx_get_rx_pmax(di, &rx_pmax);
	if (rx_pmax < WLTX_FULL_BRIDGE_RX_PWR_TH)
		tx_vset = WIRELESS_ADAPTER_5V;

	wltx_set_tx_volt(WL_TX_STAGE_REGULATION, tx_vset, false);
}

static int wltx_get_full_bridge_ith(void)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di || !di->tx_ops || !di->tx_ops->get_full_bridge_ith)
		return WLTX_FULL_BRIDGE_ITH;

	return di->tx_ops->get_full_bridge_ith();
}

static void wltx_ps_tx_vout_check(struct wireless_tx_device_info *di)
{
	int ret;
	u16 tx_iin = 0;
	int tx_vout;
	int ith;
	int tbat = 0;
	u16 rx_pmax = 0;

	if (di->tx_vset.para[di->tx_vset.cur].vset < WLTX_HALF_BRIDGE_VTH)
		return;

	ret = di->tx_ops->get_tx_iin(&tx_iin);
	if (ret)
		return;

	ith = wltx_get_full_bridge_ith();
	(void)wltx_get_rx_pmax(di, &rx_pmax);
	huawei_battery_temp(BAT_TEMP_MIXED, &tbat);
	if ((tx_iin > ith) && (tbat <= di->tx_vset_tbat_low) &&
		(rx_pmax >= WLTX_FULL_BRIDGE_RX_PWR_TH)) {
		tx_vout = di->tx_vset.v_dflt;
		wltx_set_tx_volt(WL_TX_STAGE_REGULATION, tx_vout, false);
	}
	if ((tx_iin < WLTX_HALF_BRIDGE_ITH) &&
		(di->tx_iin_avg < WLTX_HALF_BRIDGE_ITH)) {
		tx_vout = WLTX_TX_VSET_5V;
		wltx_set_tx_volt(WL_TX_STAGE_REGULATION, tx_vout, false);
	}
	if (tbat > di->tx_vset_tbat_high) {
		tx_vout = WLTX_TX_VSET_5V;
		wltx_set_tx_volt(WL_TX_STAGE_REGULATION, tx_vout, false);
	}
}

static void wltx_ps_tx_volt_check(struct wireless_tx_device_info *di)
{
	if (!time_after(jiffies, di->hs_time_out))
		return;

	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return wltx_ps_tx_vset_check(di);

	return wltx_ps_tx_vout_check(di);
}

static void wltx_check_expect_power(struct wireless_tx_device_info *di)
{
	int soc;
	int tbatt = 0;
	int tbatt_max = WL_TX_HI_PWR_TBATT_MAX;

	if (di->tx_cap.cur_id != WLTX_HIGH_PWR_CAP)
		return;

	soc = hisi_battery_capacity();
	huawei_battery_temp(BAT_TEMP_MIXED, &tbatt);
	if (di->hp_time_out && time_after(jiffies, di->hp_time_out)) {
		hwlog_err("%s: high pwr timeout\n", __func__);
		goto sw_pwr;
	}
	if (soc < WL_TX_HI_PWR_SOC_BACK) {
		hwlog_err("%s: soc=%d low\n", __func__, soc);
		goto sw_pwr;
	}
	if (di->tx_vset.para[di->tx_vset.cur].ext_hdl == WLTX_EXT_HDL_BP2CP)
		tbatt_max = WL_TX_HI_PWR_BP2CP_TBATT_MAX;
	if (tbatt >= tbatt_max) {
		hwlog_err("%s: tbatt=%d high\n", __func__, tbatt);
		goto sw_pwr;
	}
	if (di->tx_rp_timeout_lim_volt > 0) {
		hwlog_err("%s: rpp timeout\n", __func__);
		goto sw_pwr;
	}

	return;

sw_pwr:
	wlps_control(WLPS_TX_SW, WLPS_CTRL_OFF);
	wltx_disable_pwr_supply();
	usleep_range(9500, 10500); /* wait 10ms for tx power down */
	di->stop_reverse_charge = true;
	di->tx_pd_flag = true;
	di->tx_cap.exp_id = WLTX_DFLT_CAP;
}

void wltx_open_tx(enum wltx_open_type type, bool enable)
{
	if (!g_wireless_tx_di || (type < WLTX_OPEN_TYPE_BEGIN) ||
		(type >= WLTX_OPEN_TYPE_END))
		return;

	if (tx_open_flag && enable) {
		hwlog_info("[%s] tx mode has already open, ignore\n", __func__);
		return;
	}

	hwlog_info("[%s] type:%d\n", __func__, type);
	g_wireless_tx_di->logic_ops = g_wltx_logic_ops[type];
	wireless_tx_set_tx_open_flag(enable);

	if (tx_open_flag) {
		wltx_reset_exp_tx_cap_id();
		g_wireless_tx_di->tx_open_type = type;
		g_wireless_tx_di->ping_timeout = WL_TX_PING_TIMEOUT_1;
		wireless_tx_set_stage(WL_TX_STAGE_DEFAULT);
		schedule_work(&g_wireless_tx_di->wireless_tx_check_work);
	}
	wireless_tx_set_tx_status(WL_TX_STATUS_DEFAULT);
}

static int wltx_set_acc_dev_state(struct wireless_tx_device_info *di,
	u8 dev_state)
{
	wireless_acc_set_tx_dev_state(WIRELESS_PROTOCOL_QI, dev_state);
	return 0;
}

static int wltx_get_acc_dev_info_cnt(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_info_cnt(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_info_cnt);
	if (ret) {
		hwlog_err("%s: get_tx_acc_dev_info_cnt fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] get wireless_tx_acc dev_info_cnt=%d\n",
		__func__, di->wireless_tx_acc->dev_info_cnt);
	return 0;
}

static int wltx_reset_acc_dev_info_cnt(struct wireless_tx_device_info *di,
	u8 dev_info_cnt)
{
	int ret;

	ret = wireless_acc_set_tx_dev_info_cnt(WIRELESS_PROTOCOL_QI,
		dev_info_cnt);
	if (ret) {
		hwlog_err("%s: reset_acc_dev_info_cnt fail\n", __func__);
		return ret;
	}
	return 0;
}

static int wltx_get_acc_dev_state(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_state(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_state);
	if (ret) {
		hwlog_err("%s: get_tx_acc_dev_state fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] get wireless_tx_acc dev_state=%d\n",
		__func__, di->wireless_tx_acc->dev_state);
	return 0;
}

static int wltx_get_acc_dev_no(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_no(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_no);
	if (ret) {
		hwlog_err("%s: get_tx_acc_dev_no fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] get wireless_tx_acc dev_no=%d\n",
		__func__, di->wireless_tx_acc->dev_no);
	return 0;
}

static int wltx_get_acc_dev_mac(struct wireless_tx_device_info *di)
{
	int i;
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_mac(WIRELESS_PROTOCOL_QI,
		di->wireless_tx_acc->dev_mac, QI_ACC_TX_DEV_MAC_LEN);
	if (ret) {
		hwlog_err("%s: get_tx_acc_dev_mac fail\n", __func__);
		return ret;
	}

	for (i = 0; i < WL_TX_ACC_DEV_MAC_LEN; i++)
		hwlog_info("[%s] get wireless_tx_acc dev_mac[%d]=0x%02x\n",
			__func__, i, di->wireless_tx_acc->dev_mac[i]);

	return 0;
}

static int wltx_get_acc_dev_model_id(struct wireless_tx_device_info *di)
{
	int i;
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_model_id(WIRELESS_PROTOCOL_QI,
		di->wireless_tx_acc->dev_model_id, QI_ACC_TX_DEV_MODELID_LEN);
	if (ret) {
		hwlog_err("%s: get_tx_acc_dev_model_id fail\n", __func__);
		return ret;
	}

	for (i = 0; i < WL_TX_ACC_DEV_MODELID_LEN; i++)
		hwlog_info("[%s] wireless_tx_acc dev_model_id[%d]=0x%02x\n",
			__func__, i, di->wireless_tx_acc->dev_model_id[i]);

	return 0;
}

static int wltx_get_acc_dev_submodel_id(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_submodel_id(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_submodel_id);
	if (ret) {
		hwlog_err("%s: TX get acc dev_submodeid fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] wireless_tx_acc dev_submodel_id=0x%02x\n",
		__func__, di->wireless_tx_acc->dev_submodel_id);

	return 0;
}

static int wltx_get_acc_dev_version(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_version(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_version);
	if (ret) {
		hwlog_err("%s: TX get acc dev_submodeid fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] wireless_tx_acc dev_version=0x%02x\n",
		__func__, di->wireless_tx_acc->dev_version);

	return 0;
}

static int wltx_get_acc_dev_business(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di->wireless_tx_acc) {
		hwlog_err("%s: wireless_tx_acc is null\n", __func__);
		return -1;
	}

	ret = wireless_acc_get_tx_dev_business(WIRELESS_PROTOCOL_QI,
		&di->wireless_tx_acc->dev_business);
	if (ret) {
		hwlog_err("%s: get acc dev_submodeid fail\n", __func__);
		return ret;
	}

	hwlog_info("[%s] wireless_tx_acc dev_business=0x%02x\n",
		__func__, di->wireless_tx_acc->dev_business);
	return 0;
}

static int wltx_get_acc_info(struct wireless_tx_device_info *di)
{
	int ret;

	ret = wltx_get_acc_dev_info_cnt(di);
	ret += wltx_get_acc_dev_state(di);
	ret += wltx_get_acc_dev_no(di);
	ret += wltx_get_acc_dev_mac(di);
	ret += wltx_get_acc_dev_model_id(di);
	ret += wltx_get_acc_dev_submodel_id(di);
	ret += wltx_get_acc_dev_version(di);
	ret += wltx_get_acc_dev_business(di);
	if (ret) {
		hwlog_err("%s: get acc info failed\n", __func__);
		return ret;
	}

	hwlog_info("[%s] get acc info succ\n", __func__);
	return 0;
}

void wltx_notify_android_uevent(struct qi_protocol_acc_device_info *di)
{
	if (!di || (di->dev_no < ACC_DEV_NO_BEGIN) ||
		(di->dev_no >= ACC_DEV_NO_MAX)) {
		hwlog_err("%s: input invaild, not notify\n", __func__);
		return;
	}

	switch (di->dev_state) {
	case ACC_DEVICE_ONLINE:
		snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_STATE].value,
			VALUE_MAX_LEN, "%s", ACC_CONNECTED_STR);
		break;
	case ACC_DEVICE_OFFLINE:
		snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_STATE].value,
			VALUE_MAX_LEN, "%s", ACC_DISCONNECTED_STR);
		break;
	default:
		snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_STATE].value,
			VALUE_MAX_LEN, "%s", ACC_UNKNOWN_STR);
		break;
	}

	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_NO].value,
		VALUE_MAX_LEN, "%d", di->dev_no);
	/* dev_mac[0 1 2 3 4 5] is BT MAC ADDR */
	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_MAC].value, VALUE_MAX_LEN,
		"%02x:%02x:%02x:%02x:%02x:%02x", di->dev_mac[0], di->dev_mac[1],
		di->dev_mac[2], di->dev_mac[3], di->dev_mac[4], di->dev_mac[5]);
	/* dev_model_id[0 1 2] is BT model id */
	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_MODEL_ID].value,
		VALUE_MAX_LEN, "%02x%02x%02x", di->dev_model_id[0],
		di->dev_model_id[1], di->dev_model_id[2]);
	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_SUBMODEL_ID].value,
		VALUE_MAX_LEN, "%02x", di->dev_submodel_id);
	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_VERSION].value,
		VALUE_MAX_LEN, "%02x", di->dev_version);
	snprintf(g_wl_acc_info_tab[WL_TX_ACC_INFO_BUSINESS].value,
		VALUE_MAX_LEN, "%02x", di->dev_business);
	accessory_notify_android_uevent(g_wl_acc_info_tab,
		WL_TX_ACC_INFO_MAX, di->dev_no);
	hwlog_info("%s: succ\n", __func__);
}

static int wltx_get_acc_dev_info_and_notify(struct wireless_tx_device_info *di)
{
	int ret;

	ret = wltx_get_acc_info(di);
	if (ret) {
		hwlog_err("%s: get_acc_info fail, no notify\n", __func__);
		return -1;
	}
	wltx_notify_android_uevent(di->wireless_tx_acc);
	wltx_reset_acc_dev_info_cnt(di, 0);

	return 0;
}

static void wltx_handle_tx_init_evt(struct wireless_tx_device_info *di)
{
}

static void wltx_handle_tx_ap_on_evt(struct wireless_tx_device_info *di)
{
	wltx_set_tx_volt(WL_TX_STAGE_PING_RX, di->tx_vset.v_ps, true);
}

static void wireless_tx_monitor_work(struct work_struct *work)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	if (wireless_tx_can_do_reverse_charging() == WL_TX_FAIL)
		goto FuncEnd;

	wireless_tx_check_rx_disconnect(di);
	wireless_tx_check_in_tx_mode(di);
	wltx_ps_tx_volt_check(di);
	wltx_check_expect_power(di);
	wltx_check_abnormal_power();

	if (di->stop_reverse_charge) {
		hwlog_info("[%s] stop monitor work\n",__func__);
		goto FuncEnd;
	}

	if (!tx_open_flag || (di->i2c_err_cnt > WL_TX_I2C_ERR_CNT)) {
		hwlog_err("%s: TX chip is fault or TX closed\n", __func__);
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		goto FuncEnd;
	}

	wireless_tx_iout_control(di);
	schedule_delayed_work(&di->wireless_tx_monitor_work,
		msecs_to_jiffies(di->monitor_interval));
	return;

FuncEnd:
	hwlog_info("[%s] stop monitor work\n", __func__);
	wireless_tx_set_stage(WL_TX_STAGE_DEFAULT);
	wireless_tx_fault_event_handler(di);
}

static void wireless_tx_start_check_work(struct work_struct *work)
{
	int ret;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	wireless_tx_wake_lock();
	wireless_tx_para_init(di);

	if (WL_TX_STAGE_DEFAULT == wireless_tx_get_stage()) {
		ret = wireless_tx_can_do_reverse_charging();
		if (ret) {
			goto FuncEnd;
		}
		wireless_tx_set_stage(WL_TX_STAGE_POWER_SUPPLY);
	}
	if (WL_TX_STAGE_POWER_SUPPLY == wireless_tx_get_stage()) {
		ret = wireless_tx_power_supply(di);
		if (ret) {
			goto FuncEnd;
		}
		wireless_tx_set_stage(WL_TX_STAGE_CHIP_INIT);
	}
	if (WL_TX_STAGE_CHIP_INIT == wireless_tx_get_stage()) {
		ret = wireless_tx_chip_init(di);
		if (ret)
			hwlog_err("%s: TX chip init fail, go on\n", __func__);
		wireless_tx_set_stage(WL_TX_STAGE_PING_RX);
	}
	if (WL_TX_STAGE_PING_RX == wireless_tx_get_stage()) {
		ret = wireless_tx_ping_rx(di);
		if (ret) {
			goto FuncEnd;
		}
		wireless_tx_set_stage(WL_TX_STAGE_REGULATION);
	}

	g_init_tbatt = 0;
	huawei_battery_temp(BAT_TEMP_MIXED, &g_init_tbatt);
	hwlog_info("[%s] start wireless reverse charging!\n",__func__);
	wireless_tx_set_tx_status(WL_TX_STATUS_IN_CHARGING);
	power_event_notify(POWER_EVENT_NE_WIRELESS_REVERSE_START, NULL);
	hisi_coul_charger_event_rcv(WIRELESS_TX_STATUS_CHANGED);
	mod_delayed_work(system_wq, &di->wireless_tx_monitor_work, msecs_to_jiffies(0));
	return;

FuncEnd:
	hisi_coul_charger_event_rcv(WIRELESS_TX_STATUS_CHANGED);
	if (wireless_tx_get_stage() == WL_TX_STAGE_DEFAULT)
		wireless_tx_set_tx_open_flag(false);
	if (wireless_tx_get_stage() >= WL_TX_STAGE_PING_RX)
		wireless_tx_enable_tx_mode(di, false);
	if (wireless_tx_get_stage() >= WL_TX_STAGE_POWER_SUPPLY)
		wltx_disable_pwr_supply();

	wireless_tx_set_stage(WL_TX_STAGE_DEFAULT);
	power_event_notify(POWER_EVENT_NE_WIRELESS_REVERSE_STOP, NULL);
	wireless_tx_wake_unlock();
}

static void wireless_tx_handle_ping_event(void)
{
	struct wireless_tx_device_info *di = g_wireless_tx_di;
	static int abnormal_ping_cnt;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	if (tx_open_flag) {
		abnormal_ping_cnt = 0;
		return;
	}
	if (++abnormal_ping_cnt < 30) /* about 15s */
		return;

	wireless_tx_enable_tx_mode(di, false);
	wltx_disable_all_pwr();
	wireless_tx_wake_unlock();
}

static void wltx_handle_rx_vset_evt(struct wireless_tx_device_info *di)
{
	int vset;

	if ((di->tx_cap.exp_id < 0) || (di->tx_cap.exp_id >= WLTX_TOTAL_CAP))
		return;

	if (di->tx_cap.exp_id != WLTX_HIGH_PWR_CAP)
		return;

	vset = di->tx_event_data;
	hwlog_info("[%s] vset:%dmV\n", __func__, vset);
	if (vset < WLTX_RX_HIGH_VOUT)
		return;

	wltx_set_tx_volt(WL_TX_STAGE_REGULATION, vset, true);
	di->tx_cap.cur_id = di->tx_cap.exp_id;
	di->hp_time_out = jiffies + msecs_to_jiffies(WLTX_HI_PWR_TIME);
}

static void wltx_handle_irq_set_vtx_evt(struct wireless_tx_device_info *di)
{
	u16 rx_pmax = 0;

	/* during WLTX_IRQ_VSET_TIME, tx vset is invariable */
	di->irq_vset_time_out = jiffies + msecs_to_jiffies(WLTX_IRQ_VSET_TIME);

	if (!time_after(jiffies, di->hs_time_out))
		return;

	(void)wltx_get_rx_pmax(di, &rx_pmax);
	if (rx_pmax < WLTX_FULL_BRIDGE_RX_PWR_TH) {
		hwlog_info("%s: rx pmax=%u,set volt 5v\n", __func__, rx_pmax);
		wltx_set_tx_volt(WL_TX_STAGE_REGULATION, WIRELESS_ADAPTER_5V, true);
		return;
	}

	wltx_set_tx_volt(WL_TX_STAGE_REGULATION, di->tx_event_data, true);
}

static void wltx_handle_fault_event(struct wireless_tx_device_info *di)
{
	switch (di->tx_event_type) {
	case WLTX_EVT_RP_DM_TIMEOUT:
		di->tx_rp_timeout_lim_volt = WLTX_TX_VSET_5V;
		break;
	case WLTX_EVT_IRQ_SET_VTX:
		wltx_handle_irq_set_vtx_evt(di);
		break;
	default:
		break;
	}
}

static void wltx_handle_rx_data(struct wireless_tx_device_info *di)
{
	switch (di->tx_event_type) {
	case WLTX_EVT_GET_RX_PRODUCT_TYPE:
		di->rx_data.product_type = (u8)di->tx_event_data;
		hwlog_info("get_rx_type=%u\n", di->rx_data.product_type);
		break;
	case WLTX_EVT_GET_RX_MAX_POWER:
		di->rx_data.pmax = di->tx_event_data;
		hwlog_info("get_rx_pmax=%u\n", di->rx_data.pmax);
		break;
	default:
		break;
	}
}

static void wireless_tx_event_work(struct work_struct *work)
{
	int wired_ch_state;
	struct wireless_tx_device_info *di = g_wireless_tx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	switch (di->tx_event_type) {
	case WL_TX_EVENT_GET_CFG:
		/* get configure packet, ping succ */
		wireless_tx_set_tx_status(WL_TX_STATUS_PING_SUCC);
		di->hs_time_out = jiffies + msecs_to_jiffies(di->hs_timeout_offset);
		wltx_set_tx_volt(WL_TX_STAGE_REGULATION, di->tx_vset.v_hs, true);
		break;
	case WL_TX_EVENT_HANDSHAKE_SUCC:
		/* 0x8866 handshake, security authentic succ */
		di->standard_rx = true;
		break;
	case WL_TX_EVENT_CHARGEDONE:
		wired_ch_state = wireless_charge_get_wired_channel_state();
		if ((wired_ch_state == WIRED_CHANNEL_OFF) ||
			di->is_keyboard_online) {
			wireless_tx_set_tx_status(WL_TX_STATUS_CHARGE_DONE);
			di->stop_reverse_charge = true;
		}
		break;
	case WL_TX_EVENT_CEP_TIMEOUT:
		wireless_tx_set_tx_status(WL_TX_STATUS_RX_DISCONNECT);
		di->stop_reverse_charge = true;
		wireless_tx_mode_enable(di, false);
		break;
	case WL_TX_EVENT_EPT_CMD:
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		di->stop_reverse_charge = true;
		break;
	case WL_TX_EVENT_OVP:
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		di->stop_reverse_charge = true;
		break;
	case WL_TX_EVENT_OCP:
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		di->stop_reverse_charge = true;
		break;
	case WL_TX_EVENT_PING_RX:
		wireless_tx_handle_ping_event();
		break;
	case WLTX_EVT_ASK_SET_VTX:
		wltx_handle_rx_vset_evt(di);
		break;
	case WLTX_EVT_GET_TX_CAP:
		wltx_send_tx_cap();
		break;
	case WLTX_EVT_RCV_DPING:
		wltx_reset_reverse_charging();
		break;
	case WL_TX_EVENT_HALL_APPROACH:
		hwlog_info("[%s] WL_TX_EVENT_HALL_APPROACH\n", __func__);
		di->is_keyboard_online = true;
		wltx_open_tx(WLTX_OPEN_BY_HALL, true);
		break;
	case WL_TX_EVENT_HALL_AWAY_FROM:
		hwlog_info("[%s] WL_TX_EVENT_HALL_AWAY_FROM\n", __func__);
		di->tx_open_type = 0;
		di->is_keyboard_online = false;
		wltx_set_acc_dev_state(di, WL_ACC_DEV_STATE_OFFLINE);
		wltx_get_acc_dev_info_and_notify(di);
		break;
	case WL_TX_EVENT_ACC_DEV_CONNECTD:
		hwlog_info("[%s] WL_TX_EVENT_ACC_DEV_CONNECTD\n", __func__);
		wltx_get_acc_dev_info_and_notify(di);
		break;
	case WLTX_EVT_TX_FOD:
		wltx_reset_reverse_charging();
		break;
	case WLTX_EVT_TX_INIT:
		wltx_handle_tx_init_evt(di);
		break;
	case WLTX_EVT_TX_AP_ON:
		wltx_handle_tx_ap_on_evt(di);
		break;
	case WLTX_EVT_RP_DM_TIMEOUT:
	case WLTX_EVT_IRQ_SET_VTX:
		wltx_handle_fault_event(di);
		break;
	case WLTX_EVT_GET_RX_PRODUCT_TYPE:
	case WLTX_EVT_GET_RX_MAX_POWER:
		wltx_handle_rx_data(di);
		break;
	default:
		hwlog_err("invalid tx_event_type\n");
		break;
	}
}

static int wireless_tx_event_notifier_call(struct notifier_block *tx_event_nb, unsigned long event, void *data)
{
	struct wireless_tx_device_info *di =
	    container_of(tx_event_nb, struct wireless_tx_device_info, tx_event_nb);
	u16 *tx_notify_data = NULL;

	if (!di) {
		hwlog_err("%s: di NULL\n", __func__);
		return NOTIFY_OK;
	}

	di->tx_event_data = 0;
	if (data) {
		tx_notify_data = (u16 *)data;
		di->tx_event_data = *tx_notify_data;
	}
	di->tx_event_type = (enum wireless_tx_status_type)event;
	schedule_work(&di->wireless_tx_evt_work);
	return NOTIFY_OK;
}

void wireless_tx_event_notify(unsigned long e, void *v)
{
	blocking_notifier_call_chain(&tx_event_nh, e, v);
}

static void wltx_open_val_check(long val)
{
	bool status = false;
	enum wltx_open_type type;

	if ((val < 0) || (val >= (WLTX_OPEN_TYPE_END * WL_TX_OPEN_STATUS))) {
		hwlog_info("%s: input val is out of range\n", __func__);
		return;
	}

	type = val / WL_TX_OPEN_STATUS;
	status = (bool)(val % WL_TX_OPEN_STATUS);

	wltx_open_tx(type, status);
}

static int wltx_parse_tx_vset_para(struct device_node *np,
	struct wireless_tx_device_info *di)
{
	int i;
	int ret;
	int array_len;
	u32 tmp_para[WLTX_TX_VSET_TOTAL * WLTX_TX_VSET_TYPE_MAX] = { 0 };

	array_len = of_property_count_u32_elems(np, "tx_vset_para");
	if ((array_len <= 0) || (array_len % WLTX_TX_VSET_TOTAL) ||
		array_len > WLTX_TX_VSET_TOTAL * WLTX_TX_VSET_TYPE_MAX) {
		hwlog_err("%s: tx_vset_para is invalid\n", __func__);
		return -EINVAL;
	}
	di->tx_vset.total = array_len / WLTX_TX_VSET_TOTAL;
	ret = of_property_read_u32_array(np, "tx_vset_para",
		tmp_para, array_len);
	if (ret) {
		hwlog_err("%s: get tx_vset_para fail\n", __func__);
		return -EINVAL;
	}
	for (i = 0; i < di->tx_vset.total; i++) {
		di->tx_vset.para[i].rx_vmin =
			tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_RX_VSET_MIN];
		di->tx_vset.para[i].rx_vmax =
			tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_RX_VSET_MAX];
		di->tx_vset.para[i].vset =
			(int)tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_VSET];
		di->tx_vset.para[i].lth =
			tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_VSET_LTH];
		di->tx_vset.para[i].hth =
			tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_VSET_HTH];
		di->tx_vset.para[i].pl_th =
			tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_PLOSS_TH];
		di->tx_vset.para[i].pl_cnt =
			(u8)tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_PLOSS_CNT];
		di->tx_vset.para[i].ext_hdl =
			(u8)tmp_para[WLTX_TX_VSET_TOTAL * i + WLTX_TX_EXT_HDL];

		if (di->tx_vset.max_vset < di->tx_vset.para[i].vset)
			di->tx_vset.max_vset = di->tx_vset.para[i].vset;
		hwlog_info("[%s][%d] rx_min:%dmV rx_max:%dmV vset:%dmV\t"
			"lth:%dmV hth:%dmV pl_th:%dmW pl_cnt:%d ext_hdl:%d\n",
			__func__, i, di->tx_vset.para[i].rx_vmin,
			di->tx_vset.para[i].rx_vmax, di->tx_vset.para[i].vset,
			di->tx_vset.para[i].lth, di->tx_vset.para[i].hth,
			di->tx_vset.para[i].pl_th, di->tx_vset.para[i].pl_cnt,
			di->tx_vset.para[i].ext_hdl);
	}

	return 0;
}

static void wltx_set_default_tx_vset_para(struct wireless_tx_device_info *di)
{
	di->tx_vset.total = 1; /* only one level */
	di->tx_vset.max_vset = 5000; /* mV */
	di->tx_vset.para[0].rx_vmin = 4400; /* mV */
	di->tx_vset.para[0].rx_vmax = 5900; /* mV */
	di->tx_vset.para[0].vset = 5000; /* mV */
	di->tx_vset.para[0].lth = 4500; /* mV */
	di->tx_vset.para[0].hth = 5800; /* mV */
}

static void wltx_parse_tx_stage_vset_para(struct device_node *np,
	struct wireless_tx_device_info *di)
{
	int ret;
	int array_len;
	u32 temp_arr[WLTX_TX_STAGE_VTOTAL] = { 0 };

	array_len = of_property_count_u32_elems(np, "tx_stage_vset");
	if ((array_len <= 0) || (array_len % WLTX_TOTAL_CAP))
		goto parse_err;

	ret = of_property_read_u32_array(np, "tx_stage_vset",
		temp_arr, array_len);
	if (ret)
		goto parse_err;

	di->tx_vset.v_ps = (int)temp_arr[WLTX_TX_STAGE_VPS];
	di->tx_vset.v_ping = (int)temp_arr[WLTX_TX_STAGE_VPING];
	di->tx_vset.v_hs = (int)temp_arr[WLTX_TX_STAGE_VHS];
	di->tx_vset.v_dflt = (int)temp_arr[WLTX_TX_STAGE_VDFLT];

	goto print_para;

parse_err:
	di->tx_vset.v_ps = 5000; /* mV */
	di->tx_vset.v_ping = 5000; /* mV */
	di->tx_vset.v_hs = 5000; /* mV */
	di->tx_vset.v_dflt = 5000; /* mV */

print_para:
	hwlog_info("[%s] pwr_supply:%d ping:%d handshake:%d default:%d\n",
		__func__, di->tx_vset.v_ps, di->tx_vset.v_ping,
		di->tx_vset.v_hs, di->tx_vset.v_dflt);
}

static void wltx_parse_tx_cap_para(struct device_node *np,
	struct wireless_tx_device_info *di)
{
	int ret;
	int i;
	int arr_len;
	u32 temp[WLTX_TOTAL_CAP * WLC_TX_CAP_TOTAL] = { 0 };

	arr_len = of_property_count_u32_elems(np, "tx_cap");
	if ((arr_len <= 0) || (arr_len % WLC_TX_CAP_TOTAL)) {
		hwlog_err("%s: para invaild\n", __func__);
		goto parse_err;
	}
	if (arr_len > WLTX_TOTAL_CAP * WLC_TX_CAP_TOTAL) {
		hwlog_err("%s: para too long\n", __func__);
		goto parse_err;
	}
	ret = of_property_read_u32_array(np, "tx_cap", temp, arr_len);
	if (ret) {
		hwlog_err("%s: get para fail\n", __func__);
		goto parse_err;
	}

	di->tx_cap.cap_level = arr_len / WLC_TX_CAP_TOTAL;
	for (i = 0; i < di->tx_cap.cap_level; i++) {
		di->tx_cap.cap_para[i].type =
			(u8)temp[WLC_TX_CAP_TYPE + WLC_TX_CAP_TOTAL * i];
		di->tx_cap.cap_para[i].vout =
			(int)temp[WLC_TX_CAP_VMAX + WLC_TX_CAP_TOTAL * i];
		di->tx_cap.cap_para[i].iout =
			(int)temp[WLC_TX_CAP_IMAX + WLC_TX_CAP_TOTAL * i];
		di->tx_cap.cap_para[i].attr =
			(u8)temp[WLC_TX_CAP_ATTR + WLC_TX_CAP_TOTAL * i];
	}

	goto print_para;

parse_err:
	di->tx_cap.cap_level = 1;
	di->tx_cap.cap_para[0].vout = 5000; /* 5V */
	di->tx_cap.cap_para[0].iout = 500; /* 500mA */
	di->tx_cap.cap_para[0].attr = 0; /* no attr */
print_para:
	for (i = 0; i < di->tx_cap.cap_level; i++)
		hwlog_info("[%s][%d] type:%d vmax:%d imax:%d attr:0x%x\n",
			__func__, i, di->tx_cap.cap_para[i].type,
			di->tx_cap.cap_para[i].vout,
			di->tx_cap.cap_para[i].iout,
			di->tx_cap.cap_para[i].attr);
}

static void wireless_tx_parse_dts(struct device_node *np, struct wireless_tx_device_info *di)
{
	int ret, i;

	ret = of_property_read_u32_array(np, "tx_iin_limit", tx_iin_limit, WL_TX_CHARGER_TYPE_MAX);
	if (ret)
		hwlog_err("%s: get tx_iin_limit para fail!\n", __func__);
	for (i = 0; i < WL_TX_CHARGER_TYPE_MAX; i++)
		hwlog_info("[%s] tx_iin_limit[%d] = %d\n", __func__, i, tx_iin_limit[i]);
	ret = of_property_read_u32(np, "pwr_type", &di->pwr_type);
	if (ret) {
		hwlog_err("%s: get pwr_type failed\n", __func__);
		di->pwr_type = WL_TX_PWR_VBUS_OTG;
	} else if (di->pwr_type >= WL_TX_PWR_END) {
		di->pwr_type = WL_TX_PWR_VBUS_OTG;
		hwlog_err("%s: pwr_type %d >= %d, set to %s type\n",
			__func__, di->pwr_type, WL_TX_PWR_END,
			wltx_get_pwr_type_name(di->pwr_type));
	}
	hwlog_info("[%s] pwr_type = %s\n", __func__,
		wltx_get_pwr_type_name(di->pwr_type));

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_vset_tbat_high", &di->tx_vset_tbat_high,
		WLTX_TX_VSET_TBAT_TH1);

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_vset_tbat_low", &di->tx_vset_tbat_low,
		WLTX_TX_VSET_TBAT_TH2);

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_dc_done_buck_ilim", &di->tx_dc_done_buck_ilim,
		WLTX_DC_DONE_BUCK_ILIM);

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ps_need_ext_pwr", &di->ps_need_ext_pwr, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"hs_timeout_offset", &di->hs_timeout_offset, WLTX_HI_PWR_HS_TIME);

	ret = wltx_parse_tx_vset_para(np, di);
	if (ret) {
		hwlog_err("%s: use default tx_vset para\n", __func__);
		wltx_set_default_tx_vset_para(di);
	}

	wltx_parse_tx_stage_vset_para(np, di);
	wltx_parse_tx_cap_para(np, di);
}

static int wireless_tx_ops_check(struct wireless_tx_device_info *di)
{
	int ret = 0;
	if ((di == NULL) || (di->tx_ops == NULL)
		|| (di->tx_ops->chip_reset == NULL)
		|| (di->tx_ops->rx_enable == NULL)
		|| (di->tx_ops->rx_sleep_enable == NULL)
		|| (di->tx_ops->enable_tx_mode == NULL)
		|| (di->tx_ops->tx_chip_init == NULL)
		|| (di->tx_ops->tx_stop_config == NULL)
		|| (di->tx_ops->check_fwupdate == NULL)
		|| (di->tx_ops->get_tx_iin == NULL)
		|| (di->tx_ops->get_tx_vrect == NULL)
		|| (di->tx_ops->get_tx_vin == NULL)
		|| (di->tx_ops->get_chip_temp == NULL)
		|| (di->tx_ops->get_tx_fop == NULL)
		|| (di->tx_ops->set_tx_max_fop == NULL)
		|| (di->tx_ops->get_tx_max_fop == NULL)
		|| (di->tx_ops->set_tx_min_fop == NULL)
		|| (di->tx_ops->get_tx_min_fop == NULL)
		|| (di->tx_ops->set_tx_ping_frequency == NULL)
		|| (di->tx_ops->get_tx_ping_frequency == NULL)
		|| (di->tx_ops->set_tx_ping_interval == NULL)
		|| (di->tx_ops->get_tx_ping_interval == NULL)
		|| (di->tx_ops->check_rx_disconnect == NULL)
		|| (di->tx_ops->in_tx_mode == NULL))
	{
		hwlog_err("wireless_tx ops is NULL!\n");
		ret = -EINVAL;
	}
	return ret;
}

#ifdef CONFIG_SYSFS
static ssize_t wireless_tx_sysfs_show(struct device *dev,
				struct device_attribute *attr, char *buf);
static ssize_t wireless_tx_sysfs_store(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count);

static struct power_sysfs_attr_info wireless_tx_sysfs_field_tbl[] = {
	power_sysfs_attr_rw(wireless_tx, 0644, WL_TX_SYSFS_TX_OPEN, tx_open),
	power_sysfs_attr_ro(wireless_tx, 0444, WL_TX_SYSFS_TX_STATUS, tx_status),
	power_sysfs_attr_ro(wireless_tx, 0444, WL_TX_SYSFS_TX_IIN_AVG, tx_iin_avg),
	power_sysfs_attr_rw(wireless_tx, 0644, WL_TX_SYSFS_DPING_FREQ, dping_freq),
	power_sysfs_attr_rw(wireless_tx, 0644, WL_TX_SYSFS_DPING_INTERVAL, dping_interval),
	power_sysfs_attr_rw(wireless_tx, 0644, WL_TX_SYSFS_MAX_FOP, max_fop),
	power_sysfs_attr_rw(wireless_tx, 0644, WL_TX_SYSFS_MIN_FOP, min_fop),
	power_sysfs_attr_ro(wireless_tx, 0444, WL_TX_SYSFS_TX_FOP, tx_fop),
	power_sysfs_attr_ro(wireless_tx, 0444, WL_TX_SYSFS_HANDSHAKE, tx_handshake),
};
static struct attribute *wireless_tx_sysfs_attrs[ARRAY_SIZE(wireless_tx_sysfs_field_tbl) + 1];
static const struct attribute_group wireless_tx_sysfs_attr_group = {
	.attrs = wireless_tx_sysfs_attrs,
};

static void wireless_tx_sysfs_create_group(struct device *dev)
{
	power_sysfs_init_attrs(wireless_tx_sysfs_attrs,
		wireless_tx_sysfs_field_tbl, ARRAY_SIZE(wireless_tx_sysfs_field_tbl));
	power_sysfs_create_link_group("hw_power", "charger", "wireless_tx",
		dev, &wireless_tx_sysfs_attr_group);
}

static void wireless_tx_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "charger", "wireless_tx",
		dev, &wireless_tx_sysfs_attr_group);
}
#else
static inline void wireless_tx_sysfs_create_group(struct device *dev)
{
}

static inline void wireless_tx_sysfs_remove_group(struct device *dev)
{
}
#endif

/**********************************************************
*  Function:       wireless_tx_sysfs_show
*  Discription:    show the value for all wireless tx nodes
*  Parameters:   dev:device
*                      attr:device_attribute
*                      buf:string of node value
*  return value:  0-sucess or others-fail
**********************************************************/
static ssize_t wireless_tx_sysfs_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct wireless_tx_device_info *di = g_wireless_tx_di;
	u16 dping_freq = 0;
	u16 dping_interval = 0;
	u16 max_fop = 0;
	u16 min_fop = 0;
	u16 tx_fop = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		wireless_tx_sysfs_field_tbl, ARRAY_SIZE(wireless_tx_sysfs_field_tbl));
	if (!info || !di)
		return -EINVAL;

	switch (info->name) {
	case WL_TX_SYSFS_TX_OPEN:
		return snprintf(buf, PAGE_SIZE, "%d\n", tx_open_flag);
	case WL_TX_SYSFS_TX_STATUS:
		return snprintf(buf, PAGE_SIZE, "%d\n", tx_status);
	case WL_TX_SYSFS_TX_IIN_AVG:
		return snprintf(buf, PAGE_SIZE, "%d\n", di->tx_iin_avg);
	case WL_TX_SYSFS_DPING_FREQ:
		wireless_tx_get_digital_ping_frequency(di, &dping_freq);
		return snprintf(buf, PAGE_SIZE, "%d\n", dping_freq);
	case WL_TX_SYSFS_DPING_INTERVAL:
		wireless_tx_get_digital_ping_interval(di, &dping_interval);
		return snprintf(buf, PAGE_SIZE, "%d\n", dping_interval);
	case WL_TX_SYSFS_MAX_FOP:
		wireless_tx_get_max_fop(di, &max_fop);
		return snprintf(buf, PAGE_SIZE, "%d\n", max_fop);
	case WL_TX_SYSFS_MIN_FOP:
		wireless_tx_get_min_fop(di, &min_fop);
		return snprintf(buf, PAGE_SIZE, "%d\n", min_fop);
	case WL_TX_SYSFS_TX_FOP:
		wireless_tx_get_fop(di, &tx_fop);
		return snprintf(buf, PAGE_SIZE, "%d\n", tx_fop);
	case WL_TX_SYSFS_HANDSHAKE:
		return snprintf(buf, PAGE_SIZE, "%d\n",
			wltx_check_handshake(di));
	default:
		break;
	}
	return 0;
}
/**********************************************************
*  Function:       wireless_tx_sysfs_store
*  Discription:    set the value for all wireless tx nodes
*  Parameters:   dev:device
*                      attr:device_attribute
*                      buf:string of node value
*                      count:unused
*  return value:  0-sucess or others-fail
**********************************************************/
static ssize_t wireless_tx_sysfs_store(struct device *dev,
				   struct device_attribute *attr, const char *buf, size_t count)
{
	struct power_sysfs_attr_info *info = NULL;
	struct wireless_tx_device_info *di = g_wireless_tx_di;
	long val = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		wireless_tx_sysfs_field_tbl, ARRAY_SIZE(wireless_tx_sysfs_field_tbl));
	if (!info ||!di)
		return -EINVAL;

	switch (info->name) {
	case WL_TX_SYSFS_TX_OPEN:
		if (kstrtol(buf, POWER_BASE_DEC, &val) < 0)
			return -EINVAL;
		wltx_open_val_check(val);
		break;
	case WL_TX_SYSFS_DPING_FREQ:
		if (strict_strtol(buf, POWER_BASE_DEC, &val) < 0) {
			hwlog_info("%s: val is not valid!\n", __func__);
			return -EINVAL;
		}
		wireless_tx_set_digital_ping_frequency(di, val);
		break;
	case WL_TX_SYSFS_DPING_INTERVAL:
		if (strict_strtol(buf, POWER_BASE_DEC, &val) < 0) {
			hwlog_info("%s: val is not valid!\n", __func__);
			return -EINVAL;
		}
		wireless_tx_set_digital_ping_interval(di, val);
		break;
	case WL_TX_SYSFS_MAX_FOP:
		if (strict_strtol(buf, POWER_BASE_DEC, &val) < 0) {
			hwlog_info("%s: val is not valid!\n", __func__);
			return -EINVAL;
		}
		wireless_tx_set_max_fop(di, val);
		break;
	case WL_TX_SYSFS_MIN_FOP:
		if (strict_strtol(buf, POWER_BASE_DEC, &val) < 0) {
			hwlog_info("%s: val is not valid!\n", __func__);
			return -EINVAL;
		}
		wireless_tx_set_min_fop(di, val);
		break;
	default:
		break;
	}
	return count;
}

static struct wireless_tx_device_info *wireless_tx_device_info_alloc(void)
{
	static struct wireless_tx_device_info *di;
	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		hwlog_err("%s: di alloc failed\n", __func__);
	return di;
}
static void wireless_tx_shutdown(struct platform_device *pdev)
{
	struct wireless_tx_device_info *di = platform_get_drvdata(pdev);
	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}
	hwlog_info("[%s]\n", __func__);
	cancel_delayed_work(&di->wireless_tx_monitor_work);
}
static int wireless_tx_remove(struct platform_device *pdev)
{
	struct wireless_tx_device_info *di = platform_get_drvdata(pdev);
	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return 0;
	}

	wireless_tx_sysfs_remove_group(di->dev);
	wakeup_source_trash(&wireless_tx_wakelock);

	hwlog_info("[%s]\n", __func__);
	return 0;
}
static int wireless_tx_probe(struct platform_device *pdev)
{
	int ret;
	struct wireless_tx_device_info *di = NULL;
	struct qi_protocol_acc_device_info *tx_acc_dev_di = NULL;
	struct device_node *np = NULL;

	di = wireless_tx_device_info_alloc();
	if (!di) {
		hwlog_err("%s:di alloc failed\n",__func__);
		return -ENOMEM;
	}

	g_wireless_tx_di = di;
	di->dev = &pdev->dev;
	np = di->dev->of_node;
	di->tx_ops = g_wireless_tx_ops;

	wakeup_source_init(&wireless_tx_wakelock, "wireless_tx_wakelock");

	ret = wireless_tx_ops_check(di);
	if (ret) {
		hwlog_err("%s: tx ops null\n", __func__);
		goto tx_ops_check_fail;
	}
	wireless_tx_parse_dts(np, di);
	if (wltx_pwr_ctrl_init())
		goto tx_ops_check_fail;

	INIT_WORK(&di->wireless_tx_check_work, wireless_tx_start_check_work);
	INIT_WORK(&di->wireless_tx_evt_work, wireless_tx_event_work);
	INIT_DELAYED_WORK(&di->wireless_tx_monitor_work, wireless_tx_monitor_work);

	di->tx_event_nb.notifier_call = wireless_tx_event_notifier_call;
	ret = blocking_notifier_chain_register(&tx_event_nh, &di->tx_event_nb);
	if (ret < 0) {
		hwlog_err("register rx_connect notifier failed\n");
		goto notifier_regist_fail;
	}

	tx_acc_dev_di = kzalloc(sizeof(*tx_acc_dev_di), GFP_KERNEL);
	if (!tx_acc_dev_di) {
		hwlog_err("%s:acc di alloc failed\n", __func__);
		goto alloc_acc_dev_fail;
	}
	di->wireless_tx_acc = tx_acc_dev_di;

	wireless_tx_sysfs_create_group(di->dev);

	hwlog_info("wireless_tx probe ok\n");
	return 0;

alloc_acc_dev_fail:
	blocking_notifier_chain_unregister(&tx_event_nh, &di->tx_event_nb);
notifier_regist_fail:
	wltx_pwr_ctrl_exit();
tx_ops_check_fail:
	wakeup_source_trash(&wireless_tx_wakelock);
	di->tx_ops = NULL;
	kfree(di);
	di = NULL;
	g_wireless_tx_di = NULL;
	np = NULL;
	platform_set_drvdata(pdev, NULL);
	hwlog_info("wireless_tx probe failed\n");
	return ret;
}

static struct of_device_id wireless_tx_match_table[] = {
	{
	 .compatible = "huawei,wireless_tx",
	 .data = NULL,
	},
	{},
};

static struct platform_driver wireless_tx_driver = {
	.probe = wireless_tx_probe,
	.remove = wireless_tx_remove,
	.shutdown = wireless_tx_shutdown,
	.driver = {
		.name = "huawei,wireless_tx",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(wireless_tx_match_table),
	},
};
/**********************************************************
*  Function:       wireless_tx_init
*  Description:    wireless tx module initialization
*  Parameters:   NULL
*  return value:  0-sucess or others-fail
**********************************************************/
static int __init wireless_tx_init(void)
{
	hwlog_info("wireless_tx init ok.\n");

	return platform_driver_register(&wireless_tx_driver);
}
/**********************************************************
*  Function:       wireless_tx_exit
*  Description:    wireless tx module exit
*  Parameters:   NULL
*  return value:  NULL
**********************************************************/
static void __exit wireless_tx_exit(void)
{
	platform_driver_unregister(&wireless_tx_driver);
}

device_initcall_sync(wireless_tx_init);
module_exit(wireless_tx_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("wireless tx module driver");
MODULE_AUTHOR("HUAWEI Inc");
