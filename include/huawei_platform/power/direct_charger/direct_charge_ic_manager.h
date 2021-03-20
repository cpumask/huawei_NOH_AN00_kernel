/*
 * direct_charge_ic_manager.h
 *
 * direct charge ic management interface
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

#ifndef _DIRECT_CHARGE_IC_MANAGER_H_
#define _DIRECT_CHARGE_IC_MANAGER_H_

#include <linux/init.h>
#include <linux/bitops.h>

#define DC_IC_ENABLE            1
#define DC_IC_DISABLE           0

#define SENSE_R_1_MOHM          10 /* 1 mohm */
#define SENSE_R_2_MOHM          20 /* 2 mohm */
#define SENSE_R_2P5_MOHM        25 /* 2.5 mohm */
#define SENSE_R_5_MOHM          50 /* 5 mohm */
#define SENSE_R_10_MOHM         100 /* 10 mohm */

#define CHARGE_IC_MAX_NUM       2
#define CHARGE_IC_MAIN          BIT(0)
#define CHARGE_IC_AUX           BIT(1)
#define CHARGE_MULTI_IC         (CHARGE_IC_MAIN | CHARGE_IC_AUX)

/* for current calibration */
#define C_OFFSET_MAX_LEN        20
#define C_OFFSET_A_MAX          1300000
#define C_OFFSET_A_MIN          650000

enum direct_charge_working_mode {
	UNDEFINED_MODE = 0x0,
	LVC_MODE = 0x1,
	SC_MODE = 0x2,
};

enum direct_charge_ic_type {
	CHARGE_IC_TYPE_MAIN = 0,
	CHARGE_IC_TYPE_AUX,
	CHARGE_IC_TYPE_MAX,
};

struct direct_charge_ic_ops {
	const char *dev_name;
	int (*ic_init)(void);
	int (*ic_exit)(void);
	int (*ic_enable)(int);
	int (*ic_adc_enable)(int);
	int (*ic_discharge)(int);
	int (*is_ic_close)(void);
	int (*ic_enable_prepare)(void);
	int (*config_ic_watchdog)(int);
	int (*kick_ic_watchdog)(void);
	int (*get_ic_id)(void);
	int (*get_ic_status)(void);
};

struct direct_charge_batinfo_ops {
	int (*init)(void);
	int (*exit)(void);
	int (*get_bat_btb_voltage)(void);
	int (*get_bat_package_voltage)(void);
	int (*get_vbus_voltage)(int *vol);
	int (*get_bat_current)(int *cur);
	int (*get_ic_ibus)(int *ibus);
	int (*get_ic_temp)(int *temp);
	int (*get_ic_vout)(int *vout);
	int (*get_ic_vusb)(int *vusb);
};

#ifdef CONFIG_DIRECT_CHARGER
int dc_sc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops);
int dc_lvc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops);
int dc_batinfo_ops_register(int mode, unsigned int type, struct direct_charge_batinfo_ops *ops);
int dc_ic_init(int mode, int type);
int dc_ic_exit(int mode, int type);
int dc_ic_enable(int mode, int type, int enable);
int dc_ic_adc_enable(int mode, int type, int enable);
int dc_ic_discharge(int mode, int type, int enable);
int dc_is_ic_close(int mode, int type);
bool dc_is_ic_support_prepare(int mode, int type);
int dc_ic_enable_prepare(int mode, int type);
int dc_config_ic_watchdog(int mode, int type, int time);
int dc_kick_ic_watchdog(int mode, int type);
int dc_get_ic_id(int mode, int type);
int dc_get_ic_status(int mode, int type);
const char *dc_get_ic_name(int mode, int type);
int dc_batinfo_init(int mode, int type);
int dc_batinfo_exit(int mode, int type);
int dc_get_bat_btb_voltage(int mode, int type);
int dc_get_bat_package_voltage(int mode, int type);
int dc_get_bat_btb_voltage_with_comp(int mode, int type, const int *vbat_comp);
int dc_get_vbus_voltage(int mode, int type, int *vbus);
int dc_get_bat_current(int mode, int type, int *ibat);
int dc_get_bat_current_with_comp(int mode, int type, int *ibat);
int dc_get_ic_ibus(int mode, int type, int *ibus);
int dc_get_ic_temp(int mode, int type, int *temp);
int dc_get_ic_vusb(int mode, int type, int *vusb);
int dc_get_ic_vout(int mode, int type, int *vout);
int dc_get_ic_max_temp(int mode, int *temp);
int dc_get_total_ibus(int mode, int *ibus);
int dc_get_total_bat_current(int mode, int *ibat);
int dc_get_bat_btb_avg_voltage(int mode, int *vbat);
int dc_get_bat_package_avg_voltage(int mode, int *vbat);
int dc_get_vbus_avg_voltage(int mode, int *vbus);
#else
static inline int dc_sc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops)
{
	return -1;
}

static inline int dc_lvc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops)
{
	return -1;
}

static inline int dc_batinfo_ops_register(int mode, unsigned int type, struct direct_charge_batinfo_ops *ops)
{
	return -1;
}

static inline int dc_ic_init(int mode, int type)
{
	return -1;
}

static inline int dc_ic_exit(int mode, int type)
{
	return -1;
}

static inline int dc_ic_enable(int mode, int type, int enable)
{
	return -1;
}

static inline int dc_ic_adc_enable(int mode, int type, int enable)
{
	return -1;
}

static inline int dc_ic_discharge(int mode, int type, int enable)
{
	return -1;
}

static inline int dc_is_ic_close(int mode, int type)
{
	return -1;
}

static inline bool dc_is_ic_support_prepare(int mode, int type)
{
	return false;
}

static inline int dc_ic_enable_prepare(int mode, int type)
{
	return -1;
}

static inline int dc_config_ic_watchdog(int mode, int type, int time)
{
	return -1;
}

static inline int dc_kick_ic_watchdog(int mode, int type)
{
	return -1;
}

static inline int dc_get_ic_id(int mode, int type)
{
	return -1;
}

static inline int dc_get_ic_status(int mode, int type)
{
	return -1;
}

static inline const char *dc_get_ic_name(int mode, int type)
{
	return "invalid ic";
}

static inline int dc_batinfo_init(int mode, int type)
{
	return -1;
}

static inline int dc_batinfo_exit(int mode, int type)
{
	return -1;
}

static inline int dc_get_bat_btb_voltage(int mode, int type)
{
	return -1;
}

static inline int dc_get_bat_package_voltage(int mode, int type)
{
	return -1;
}

static inline int dc_get_bat_btb_voltage_with_comp(int mode, int type, const int *vbat_comp)
{
	return -1;
}

static inline int dc_get_vbus_voltage(int mode, int type, int *vbus)
{
	return -1;
}

static inline int dc_get_bat_current(int mode, int type, int *ibat)
{
	return -1;
}

static inline int dc_get_bat_current_with_comp(int mode, int type, int *ibat)
{
	return -1;
}

static inline int dc_get_ic_ibus(int mode, int type, int *ibus)
{
	return -1;
}

static inline int dc_get_ic_temp(int mode, int type, int *temp)
{
	return -1;
}

static inline int dc_get_ic_vusb(int mode, int type, int *vusb)
{
	return -1;
}

static inline int dc_get_ic_vout(int mode, int type, int *vout)
{
	return -1;
}

static inline int dc_get_ic_max_temp(int mode, int *temp)
{
	return -1;
}

static inline int dc_get_total_ibus(int mode, int *ibus)
{
	return -1;
}

static inline int dc_get_total_bat_current(int mode, int *ibat)
{
	return -1;
}

static inline int dc_get_bat_btb_avg_voltage(int mode, int *vbat)
{
	return -1;
}

static inline int dc_get_bat_package_avg_voltage(int mode, int *vbat)
{
	return -1;
}

static inline int dc_get_vbus_avg_voltage(int mode, int *vbus)
{
	return -1;
}
#endif /* CONFIG_DIRECT_CHARGER */

#endif /* _DIRECT_CHARGE_IC_MANAGER_H_ */
