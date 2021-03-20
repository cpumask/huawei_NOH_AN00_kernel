// SPDX-License-Identifier: GPL-2.0
/*
 * Head file of ComboPHY Core Module on HiSilicon platform
 *
 * Copyright (C) 2020-2022 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#ifndef __HISI_COMBOPHY_CORE_H__
#define __HISI_COMBOPHY_CORE_H__

#include <linux/phy/phy.h>

#if IS_ENABLED(CONFIG_HISI_COMBOPHY)
extern int combophy_set_mode(struct phy *phy,
		      TCPC_MUX_CTRL_TYPE mode_type,
		      TYPEC_PLUG_ORIEN_E typec_orien);
extern int combophy_init(struct phy *phy);
extern int combophy_exit(struct phy *phy);
extern int combophy_register_debugfs(struct phy *phy, struct dentry *root);
extern void combophy_regdump(struct phy *phy);
#else
static inline int combophy_set_mode(struct phy *phy,
		      TCPC_MUX_CTRL_TYPE mode_type,
		      TYPEC_PLUG_ORIEN_E typec_orien)
{
	return 0;
}

static inline int combophy_init(struct phy *phy)
{
	return 0;
}

static inline int combophy_exit(struct phy *phy)
{
	return 0;
}

static inline int combophy_register_debugfs(struct phy *phy,
					    struct dentry *root)
{
	return 0;
}

static inline void combophy_regdump(struct phy *phy) {}
#endif /* CONFIG_HISI_COMBOPHY */

#endif /* __HISI_COMBOPHY_CORE_H__ */
