/*
 * anc_hs_interface.h
 *
 * analog headset interface
 *
 * Copyright (c) 2014-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __ANC_HS_INTERFACE__
#define __ANC_HS_INTERFACE__

#include "huawei_platform/audio/ana_hs_common.h"

#define ANC_HS_ENABLE_CHARGE                       0
#define ANC_HS_DISABLE_CHARGE                      1

#define ANC_HS_ENABLE_5VOLTATE                     1
#define ANC_HS_DISABLE_5VOLTATE                    0

enum {
	NO_MAX14744,
	HANDSET_PLUG_IN,
	HANDSET_PLUG_OUT,
};

enum {
	ANC_HS_MIC_GND_DISCONNECT = 0,
	ANC_HS_MIC_GND_CONNECT = 1,
};

/* codec operate used by anc_hs driver */
struct anc_hs_codec_ops {
	bool (*check_headset_in)(void *);
	void (*btn_report)(int, int);
	void (*codec_resume_lock)(void *, bool);
	void (*plug_in_detect)(void *);
	void (*plug_out_detect)(void *);
	void (*check_bus_status)(void *);
};

struct anc_hs_dev {
	char *name;
	struct anc_hs_codec_ops ops;
};

struct anc_hs_ops {
	int (*anc_hs_dev_register)(struct anc_hs_dev *dev, void *codec_data);
	bool (*anc_hs_check_headset_pluged_in)(void);
	void (*anc_hs_start_charge)(void);
	bool (*anc_hs_charge_detect)(int saradc_value, int headset_type);
	void (*anc_hs_stop_charge)(void);
	void (*anc_hs_force_charge)(int disable);
	bool (*check_anc_hs_support)(void);
	void (*anc_hs_5v_control)(int enable);
	bool (*anc_hs_plug_enable)(void);
	void (*anc_hs_invert_hs_control)(int connect);
	void (*anc_hs_refresh_headset_type)(int headset_type);
};

int anc_hs_interface_dev_register(struct anc_hs_dev *dev, void *codec_data);

int anc_hs_interface_check_headset_pluged_in(void);

void anc_hs_interface_start_charge(void);

bool anc_hs_interface_charge_detect(int saradc_value, int headset_type);

void anc_hs_interface_stop_charge(void);

void anc_hs_interface_force_charge(int disable);

bool check_anc_hs_interface_support(void);

int anc_hs_ops_register(struct anc_hs_ops *ops);

void anc_hs_interface_5v_control(int enable);

bool anc_hs_interface_plug_enable(void);

void anc_hs_interface_invert_hs_control(int connect);

void anc_hs_interface_refresh_headset_type(int headset_type);

#endif /* __ANC_HS_INTERFACE__ */
