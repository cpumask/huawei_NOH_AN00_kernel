/*
 * wireless_tx_pwr_ctrl.c
 *
 * power control for wireless reverse charging
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

#include <linux/kernel.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/wireless_transmitter.h>

static struct wltx_pwr_ctrl_info g_pwr_ctrl_di;

#define HWLOG_TAG wireless_tx_pwr_ctrl
HWLOG_REGIST();

static struct {
	enum wltx_pwr_sw_scene scn;
	const char *name;
} const g_pwr_sw_scn[] = {
	{ PWR_SW_BY_VBUS_ON,  "PWR_SW_BY_VBUS_ON" },
	{ PWR_SW_BY_VBUS_OFF, "PWR_SW_BY_VBUS_OFF" },
	{ PWR_SW_BY_OTG_ON,   "PWR_SW_BY_OTG_ON" },
	{ PWR_SW_BY_OTG_OFF,  "PWR_SW_BY_OTG_OFF" },
	{ PWR_SW_BY_DC_DONE,  "PWR_SW_BY_DC_DONE" }
};

/* pwr_type: VBUS_OTG */
const struct wltx_pwr_attr g_attr1[] = {
	{ PWR_SW_BY_VBUS_ON, true, PWR_SRC_VBUS },
	{ PWR_SW_BY_VBUS_OFF, true, PWR_SRC_OTG },
	{ PWR_SW_BY_OTG_ON, false, PWR_SRC_OTG },
	{ PWR_SW_BY_OTG_OFF, false, PWR_SRC_OTG }
};

/* pwr_type: 5VBST_OTG */
const struct wltx_pwr_attr g_attr2[] = {
	{ PWR_SW_BY_VBUS_ON, true, PWR_SRC_5VBST },
	{ PWR_SW_BY_VBUS_OFF, true, PWR_SRC_OTG },
	{ PWR_SW_BY_OTG_ON, false, PWR_SRC_5VBST },
	{ PWR_SW_BY_OTG_OFF, true, PWR_SRC_OTG }
};

/* pwr_type: SP_BST */
const struct wltx_pwr_attr g_attr3[] = {
	{ PWR_SW_BY_VBUS_ON, false, PWR_SRC_SPBST },
	{ PWR_SW_BY_VBUS_OFF, false, PWR_SRC_SPBST },
	{ PWR_SW_BY_OTG_ON, false, PWR_SRC_SPBST },
	{ PWR_SW_BY_OTG_OFF, false, PWR_SRC_SPBST }
};

/* pwr_type: 5VBST */
const struct wltx_pwr_attr g_attr4[] = {
	{ PWR_SW_BY_VBUS_ON, false, PWR_SRC_5VBST },
	{ PWR_SW_BY_VBUS_OFF, false, PWR_SRC_5VBST },
	{ PWR_SW_BY_OTG_ON, false, PWR_SRC_5VBST },
	{ PWR_SW_BY_OTG_OFF, false, PWR_SRC_5VBST }
};

/* pwr_type: 5VBST_VBUS_OTG_CP */
const struct wltx_pwr_attr g_attr5[] = {
	{ PWR_SW_BY_VBUS_ON, true, PWR_SRC_5VBST },
	{ PWR_SW_BY_VBUS_OFF, true, PWR_SRC_OTG_CP },
	{ PWR_SW_BY_OTG_ON, false, PWR_SRC_5VBST },
	{ PWR_SW_BY_OTG_OFF, true, PWR_SRC_OTG_CP },
	{ PWR_SW_BY_DC_DONE, true, PWR_SRC_VBUS_CP }
};

static struct {
	enum wltx_pwr_type type;
	const char *name;
	const struct wltx_pwr_attr *attr;
	int size;
} const g_pwr_ctrl[] = {
	{ WL_TX_PWR_VBUS_OTG, "VBUS_OTG", g_attr1, ARRAY_SIZE(g_attr1) },
	{ WL_TX_PWR_5VBST_OTG, "5VBST_OTG", g_attr2, ARRAY_SIZE(g_attr2) },
	{ WL_TX_PWR_SPBST, "SP-BST", g_attr3, ARRAY_SIZE(g_attr3) },
	{ WL_TX_PWR_5VBST, "5VBST", g_attr4, ARRAY_SIZE(g_attr4) },
	{ WL_TX_PWR_5VBST_VBUS_OTG_CP, "5VBST_VBUS_OTG_CP",
		g_attr5, ARRAY_SIZE(g_attr5) }
};

struct wltx_pwr_ctrl_info *wltx_get_pwr_ctrl_info(void)
{
	return &g_pwr_ctrl_di;
}

const char *wltx_get_pwr_type_name(enum wltx_pwr_type type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_pwr_ctrl); i++) {
		if (type == g_pwr_ctrl[i].type)
			return g_pwr_ctrl[i].name;
	}
	return "NA";
}

const char *wltx_get_pwr_sw_scn_name(enum wltx_pwr_sw_scene scn)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_pwr_sw_scn); i++) {
		if (scn == g_pwr_sw_scn[i].scn)
			return g_pwr_sw_scn[i].name;
	}
	return "NA";
}

static const struct wltx_pwr_attr *wltx_get_pwr_attr(enum wltx_pwr_type type,
	enum wltx_pwr_sw_scene scn)
{
	int t_id, s_id;

	for (t_id = 0; t_id < ARRAY_SIZE(g_pwr_ctrl); t_id++) {
		if (type == g_pwr_ctrl[t_id].type)
			break;
	}
	if (t_id >= ARRAY_SIZE(g_pwr_ctrl))
		return NULL;

	for (s_id = 0; s_id < g_pwr_ctrl[t_id].size; s_id++) {
		if (scn == g_pwr_ctrl[t_id].attr[s_id].scn)
			return &g_pwr_ctrl[t_id].attr[s_id];
	}

	return NULL;
}

static bool wltx_need_specify_pwr_src(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di || !di->logic_ops || !di->logic_ops->need_specify_pwr_src)
		return false;

	return di->logic_ops->need_specify_pwr_src();
}

static enum wltx_pwr_src wltx_specify_pwr_src(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di || !di->logic_ops || !di->logic_ops->specify_pwr_src ||
		!wltx_need_specify_pwr_src())
		return PWR_SRC_NULL;

	return di->logic_ops->specify_pwr_src();
}

static enum wltx_pwr_src wltx_get_pwr_src_by_type_scn(
	enum wltx_pwr_type type, enum wltx_pwr_sw_scene scn)
{
	const struct wltx_pwr_attr *attr = wltx_get_pwr_attr(type, scn);
	enum wltx_pwr_src pwr_src = wltx_specify_pwr_src();

	if (!attr)
		return PWR_SRC_NA;

	if (pwr_src != PWR_SRC_NULL)
		return pwr_src;

	return attr->src;
}

bool wltx_need_switch_power(enum wltx_pwr_type type,
	enum wltx_pwr_sw_scene scn, enum wltx_pwr_src cur_src)
{
	const struct wltx_pwr_attr *attr = wltx_get_pwr_attr(type, scn);

	if (!attr || wltx_need_specify_pwr_src())
		return false;

	return attr->need_sw && (cur_src != attr->src);
}

bool wltx_need_disable_wired_dc(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return false;

	if (di->pwr_type != WL_TX_PWR_VBUS_OTG)
		return false;
	if (!wireless_tx_get_tx_open_flag())
		return false;

	return true;
}

static void wltx_pre_pwr_supply(struct wireless_tx_device_info *di, bool flag)
{
	if (!di || !di->tx_ops || !di->tx_ops->pre_ps)
		return;

	di->tx_ops->pre_ps(flag);
}

static void wltx_activate_tx_chip(struct wireless_tx_device_info *di)
{
	if (!di || !di->tx_ops || !di->tx_ops->activate_tx_chip)
		return;

	di->tx_ops->activate_tx_chip();
}

static void wltx_check_dc_done(void)
{
	int soc;

	soc = hisi_battery_capacity();
	if (soc >= WL_TX_HI_PWR_SOC_MIN) {
		g_pwr_ctrl_di.dc_done = true;
		direct_charge_set_disable_flags(DC_SET_DISABLE_FLAGS,
			DC_DISABLE_WIRELESS_TX);
	} else if (soc < WL_TX_HI_PWR_SOC_BACK) {
		g_pwr_ctrl_di.dc_done = false;
		direct_charge_set_disable_flags(DC_CLEAR_DISABLE_FLAGS,
			DC_DISABLE_WIRELESS_TX);
	}
}

static void wltx_update_pwr_scn(struct wireless_tx_device_info *di)
{
	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return;

	if (g_pwr_ctrl_di.charger_type != WLTX_SC_HI_PWR_CHARGER)
		return;

	wltx_check_dc_done();

	if (g_pwr_ctrl_di.dc_done)
		di->cur_pwr_sw_scn = PWR_SW_BY_DC_DONE;

	schedule_delayed_work(&g_pwr_ctrl_di.mon_dc_work,
		msecs_to_jiffies(WLTX_MON_DC_ADP_INTERVAL));
}

int wltx_enable_pwr_supply(void)
{
	int ret;
	int delay = 0;
	enum wltx_pwr_src src;
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return -EPERM;

	wltx_pre_pwr_supply(di, true);
	wltx_update_pwr_scn(di);
	if (di->tx_open_type != WLTX_OPEN_BY_LIGHTSTRAP)
		delay = 500; /* delay 500ms to avoid state disorder in case of quick open/close */

	ret = wltx_msleep(delay);
	if (ret) {
		wltx_pre_pwr_supply(di, false);
		return ret;
	}

	hwlog_info("[%s] before, pwr_sw_scn: %s pwr_src: %s\n",
		__func__, wltx_get_pwr_sw_scn_name(di->cur_pwr_sw_scn),
		wltx_get_pwr_src_name(di->cur_pwr_src));
	src = wltx_get_pwr_src_by_type_scn(di->pwr_type, di->cur_pwr_sw_scn);
	di->cur_pwr_src = wltx_set_pwr_src_output(true, src);
	hwlog_info("[%s] after, pwr_sw_scn: %s, pwr_src: %s\n",
		__func__, wltx_get_pwr_sw_scn_name(di->cur_pwr_sw_scn),
		wltx_get_pwr_src_name(di->cur_pwr_src));

	wltx_activate_tx_chip(di);
	wltx_pre_pwr_supply(di, false);

	return 0;
}

void wltx_disable_pwr_supply(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	hwlog_info("[%s] before, pwr_sw_scn: %s pwr_src: %s\n",
		__func__, wltx_get_pwr_sw_scn_name(di->cur_pwr_sw_scn),
		wltx_get_pwr_src_name(di->cur_pwr_src));

	di->cur_pwr_src = wltx_set_pwr_src_output(false, di->cur_pwr_src);

	hwlog_info("[%s] after, pwr_sw_scn: %s, pwr_src: %s\n",
		__func__, wltx_get_pwr_sw_scn_name(di->cur_pwr_sw_scn),
		wltx_get_pwr_src_name(di->cur_pwr_src));
}

static void wltx_5vbst_otg_extra_pwr(struct wireless_tx_device_info *di)
{
	enum wltx_pwr_src expected_src =
		wltx_get_pwr_src_by_type_scn(di->pwr_type, di->cur_pwr_sw_scn);

	if (expected_src == PWR_SRC_OTG) {
		di->cur_pwr_src = wltx_set_pwr_src_output(false, PWR_SRC_OTG);
		msleep(20); /* delay 20ms for otg pwr_off */
		di->cur_pwr_src = wltx_set_pwr_src_output(true, PWR_SRC_5VBST);
	}
}

static void wltx_5vbst_vbus_otg_cp_extra_pwr(struct wireless_tx_device_info *di)
{
}

void wltx_enable_extra_pwr_supply(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	switch (di->pwr_type) {
	case WL_TX_PWR_5VBST_OTG:
		wltx_5vbst_otg_extra_pwr(di);
		break;
	case WL_TX_PWR_5VBST_VBUS_OTG_CP:
		wltx_5vbst_vbus_otg_cp_extra_pwr(di);
		break;
	default:
		break;
	}
}

void wltx_disable_all_pwr(void)
{
	int i;
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	for (i = PWR_SW_SCN_BEGIN; i < PWR_SW_SCN_END; i++) {
		di->cur_pwr_src = wltx_get_pwr_src_by_type_scn(di->pwr_type, i);
		wltx_disable_pwr_supply();
	}
}

enum wltx_pwr_type wltx_get_pwr_type(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return WL_TX_PWR_END;

	return di->pwr_type;
}

void wireless_tx_cancel_work(enum wltx_pwr_sw_scene pwr_sw_scn)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	di->cur_pwr_sw_scn = pwr_sw_scn;
	if (pwr_sw_scn == PWR_SW_BY_VBUS_OFF) {
		g_pwr_ctrl_di.dc_done = false;
		g_pwr_ctrl_di.charger_type = WLTX_UNKOWN_CHARGER;
	}

	if (!wireless_tx_get_tx_open_flag())
		return;

	if (wltx_need_switch_power(di->pwr_type, pwr_sw_scn, di->cur_pwr_src))
		wltx_cancle_work_handler();
}

void wireless_tx_restart_check(enum wltx_pwr_sw_scene pwr_sw_scn)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	di->cur_pwr_sw_scn = pwr_sw_scn;
	if (!wireless_tx_get_tx_open_flag())
		return;

	if (wltx_need_switch_power(di->pwr_type, pwr_sw_scn, di->cur_pwr_src))
		wltx_restart_work_handler();
}

static void wltx_switch_pwr_src(enum wltx_pwr_sw_scene pwr_sw_scn)
{
	wireless_tx_cancel_work(pwr_sw_scn);
	wireless_tx_restart_check(pwr_sw_scn);
}

static void wltx_monitor_dc_work(struct work_struct *work)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	if (!wireless_tx_get_tx_open_flag() ||
		(g_pwr_ctrl_di.charger_type != WLTX_SC_HI_PWR_CHARGER))
		return;

	wltx_check_dc_done();
	if (g_pwr_ctrl_di.dc_done && (di->cur_pwr_sw_scn != PWR_SW_BY_DC_DONE))
		wltx_switch_pwr_src(PWR_SW_BY_DC_DONE);
	else if (!g_pwr_ctrl_di.dc_done &&
		(di->cur_pwr_sw_scn == PWR_SW_BY_DC_DONE))
		wltx_switch_pwr_src(PWR_SW_BY_VBUS_ON);

	schedule_delayed_work(&g_pwr_ctrl_di.mon_dc_work,
		msecs_to_jiffies(WLTX_MON_DC_ADP_INTERVAL));
}

void wltx_dc_adaptor_handler(void)
{
	int adp_imax;
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di || (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP))
		return;

	if (g_pwr_ctrl_di.charger_type != WLTX_UNKOWN_CHARGER)
		return;

	/* get adapter ability of current on 5V */
	adp_imax = direct_charge_get_adapter_max_current(5000);
	hwlog_info("[dc_adaptor_handler] scp charger, imax=%d\n", adp_imax);
	if (adp_imax < WLTX_HIGH_PWR_SC_MIN_IOUT) {
		g_pwr_ctrl_di.charger_type = WLTX_SC_LOW_PWR_CHARGER;
		return;
	}
	g_pwr_ctrl_di.charger_type = WLTX_SC_HI_PWR_CHARGER;

	if (wireless_tx_get_tx_open_flag())
		wltx_check_dc_done();

	schedule_delayed_work(&g_pwr_ctrl_di.mon_dc_work, msecs_to_jiffies(0));
}

int wltx_pwr_ctrl_init(void)
{
	INIT_DELAYED_WORK(&g_pwr_ctrl_di.mon_dc_work, wltx_monitor_dc_work);
	return 0;
}

void wltx_pwr_ctrl_exit(void)
{
}
