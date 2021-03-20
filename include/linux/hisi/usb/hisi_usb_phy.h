// SPDX-License-Identifier: GPL-2.0
/*
 * Phy provider for USB 2.0 controller on HiSilicon May platform
 *
 * Copyright (C) 2017-2018 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#ifndef _HISI_USB_PHY_H_
#define _HISI_USB_PHY_H_

#include <linux/hisi/usb/hisi_usb.h>
#include <linux/hisi/usb/tca.h>
#include <linux/phy/phy.h>
#include <linux/types.h>

struct hisi_usb2_phy {
	struct phy *phy;
	enum hisi_charger_type (*detect_charger_type)(
			struct hisi_usb2_phy *usb2_phy);
	int (*set_dpdm_pulldown)(struct hisi_usb2_phy *usb2_phy, bool pulldown);
	int (*set_vdp_src)(struct hisi_usb2_phy *usb2_phy, bool enable);
};

struct hisi_combophy {
	struct phy *phy;
	int (*set_mode)(struct hisi_combophy *combophy,
			TCPC_MUX_CTRL_TYPE mode_type,
			TYPEC_PLUG_ORIEN_E typec_orien);
	ssize_t (*info_dump)(struct hisi_combophy *combophy, char *buf,
			     size_t size);
	int (*register_debugfs)(struct hisi_combophy *combophy,
				struct dentry *root);
	void (*regdump)(struct hisi_combophy *combophy);
};

#endif /* _HISI_USB_PHY_H_ */
