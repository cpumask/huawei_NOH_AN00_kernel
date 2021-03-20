// SPDX-License-Identifier: GPL-2.0
/*
 * ComboPHY Core Module on HiSilicon platform
 *
 * Copyright (C) 2020-2022 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#include <linux/hisi/usb/hisi_usb_phy.h>
#include <linux/phy/phy.h>

#define INVALID_REG_VALUE 0xFFFFFFFFu

int combophy_set_mode(struct phy *phy,
		      TCPC_MUX_CTRL_TYPE mode_type,
		      TYPEC_PLUG_ORIEN_E typec_orien)
{
	struct hisi_combophy *combophy = NULL;

	if (!phy)
		return -EINVAL;

	combophy = phy_get_drvdata(phy);
	if (combophy && combophy->set_mode)
		return combophy->set_mode(combophy, mode_type, typec_orien);

	return -EINVAL;
}

int combophy_init(struct phy *phy)
{
	return phy_init(phy);
}

int combophy_exit(struct phy *phy)
{
	return phy_exit(phy);
}

int combophy_register_debugfs(struct phy *phy, struct dentry *root)
{
	struct hisi_combophy *combophy = NULL;

	if (!phy)
		return -EINVAL;

	combophy = phy_get_drvdata(phy);
	if (combophy && combophy->register_debugfs)
		return combophy->register_debugfs(combophy, root);

	return 0;
}

void combophy_regdump(struct phy *phy)
{
	struct hisi_combophy *combophy = NULL;

	if (!phy)
		return;

	combophy = phy_get_drvdata(phy);
	if (combophy && combophy->regdump)
		combophy->regdump(combophy);
}
