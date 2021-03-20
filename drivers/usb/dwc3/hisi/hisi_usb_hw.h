// SPDX-License-Identifier: GPL-2.0
/*
 * Head file of hardware config for Hisilicon USB
 *
 * Authors: Yu Chen <chenyu56@huawei.com>
 */

#ifndef _HISI_USB_HW_H_
#define _HISI_USB_HW_H_

#include <linux/phy/phy.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/hisi/usb/hisi_usb_phy.h>
#include "dwc3-hisi.h"

int hisi_usb_get_hw_res(struct hisi_dwc3_device *hisi_usb, struct device *dev);
void hisi_usb_put_hw_res(struct hisi_dwc3_device *hisi_usb);
int hisi_usb3_phy_init(struct phy *phy,
		       TCPC_MUX_CTRL_TYPE mode_type,
		       TYPEC_PLUG_ORIEN_E typec_orien);
int hisi_usb3_phy_exit(struct phy *phy);
int hisi_usb2_phy_init(struct phy *usb2_phy, bool host_mode);
int hisi_usb2_phy_exit(struct phy *usb2_phy);
enum hisi_charger_type hisi_usb_detect_charger_type(
		struct hisi_dwc3_device *hisi_dwc3);
void hisi_usb_dpdm_pulldown(struct hisi_dwc3_device *hisi_dwc3);
void hisi_usb_dpdm_pullup(struct hisi_dwc3_device *hisi_dwc3);
void hisi_usb_disable_vdp_src(struct hisi_dwc3_device *hisi_dwc3);
void hisi_usb_enable_vdp_src(struct hisi_dwc3_device *hisi_dwc3);
int hisi_usb_controller_init(struct hisi_dwc3_device *hisi_dwc3);
int hisi_usb_controller_exit(struct hisi_dwc3_device *hisi_dwc3);
int hisi_usb_register_hw_debugfs(struct hisi_dwc3_device *hisi_usb);

#endif /* _HISI_USB_HW_H_ */
