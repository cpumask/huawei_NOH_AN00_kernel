/*
 * direct_charger_adapter.h
 *
 * adapter operate for direct charger
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

#ifndef _DIRECT_CHARGER_ADAPTER_H_
#define _DIRECT_CHARGER_ADAPTER_H_

#ifdef CONFIG_DIRECT_CHARGER
int direct_charge_init_adapter(void);
int direct_charge_reset_operate(int type);
int direct_charge_get_adapter_type(void);
int direct_charge_get_adapter_support_mode(void);
int direct_charge_get_adapter_port_leakage_current(void);
int direct_charge_get_adapter_voltage(int *value);
int direct_charge_get_adapter_max_voltage(int *value);
int direct_charge_get_adapter_min_voltage(int *value);
int direct_charge_get_adapter_current(int *value);
int direct_charge_get_adapter_current_set(int *value);
int direct_charge_get_power_drop_current(int *value);
void direct_charge_reset_adapter_power_curve(void *p);
int direct_charge_get_adapter_power_curve(void);
int direct_charge_get_adapter_max_current(int value);
int direct_charge_get_adapter_temp(int *value);
int direct_charge_get_protocol_register_state(void);
int direct_charge_set_adapter_voltage(int value);
int direct_charge_set_adapter_current(int value);
int direct_charge_set_adapter_output_enable(int enable);
void direct_charge_set_adapter_default_param(void);
int direct_charge_set_adapter_default(void);
int direct_charge_update_adapter_info(void);
unsigned int direct_charge_update_adapter_support_mode(void);
int direct_charge_check_adapter_support_mode(int *mode);
int direct_charge_check_adapter_antifake(void);
#else
static inline int direct_charge_init_adapter(void)
{
	return -1;
}

static inline int direct_charge_reset_operate(int type)
{
	return -1;
}

static inline int direct_charge_get_adapter_type(void)
{
	return 0;
}

static inline int direct_charge_get_adapter_support_mode(void)
{
	return 0;
}

static inline int direct_charge_get_adapter_port_leakage_current(void)
{
	return -1;
}

static inline int direct_charge_get_adapter_voltage(int *value)
{
	return -1;
}

static inline int direct_charge_get_adapter_max_voltage(int *value)
{
	return -1;
}

static inline int direct_charge_get_adapter_min_voltage(int *value)
{
	return -1;
}

static inline int direct_charge_get_adapter_current(int *value)
{
	return -1;
}

static inline int direct_charge_get_adapter_current_set(int *value)
{
	return -1;
}

static inline int direct_charge_get_power_drop_current(int *value)
{
	return -1;
}

static inline void direct_charge_reset_adapter_power_curve(void *p)
{
}

static inline int direct_charge_get_adapter_power_curve(void)
{
	return -1;
}

static inline int direct_charge_get_adapter_max_current(int value)
{
	return 0;
}

static inline int direct_charge_get_adapter_temp(int *value)
{
	return -1;
}

static inline int direct_charge_get_protocol_register_state(void)
{
	return -1;
}

static inline int direct_charge_set_adapter_voltage(int value)
{
	return -1;
}

static inline int direct_charge_set_adapter_current(int value)
{
	return -1;
}

static inline int direct_charge_set_adapter_output_enable(int enable)
{
	return -1;
}

static inline void direct_charge_set_adapter_default_param(void)
{
}

static inline int direct_charge_set_adapter_default(void)
{
	return -1;
}

static inline int direct_charge_update_adapter_info(void)
{
	return -1;
}

static inline unsigned int direct_charge_update_adapter_support_mode(void)
{
	return 0;
}

static inline int direct_charge_check_adapter_support_mode(int *mode)
{
	return -1;
}

static inline int direct_charge_check_adapter_antifake(void)
{
	return -1;
}
#endif /* CONFIG_DIRECT_CHARGER */

#endif /* _DIRECT_CHARGER_ADAPTER_H_ */
