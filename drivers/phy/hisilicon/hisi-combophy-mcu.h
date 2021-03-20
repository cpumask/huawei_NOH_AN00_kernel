// SPDX-License-Identifier: GPL-2.0
/*
 * Head file of MCU Driver for ComboPHY on HiSilicon platform
 *
 * Copyright (C) 2020-2022 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#ifndef __HISI_COMBOPHY_MCU_H__
#define __HISI_COMBOPHY_MCU_H__

extern int combophy_mcu_init(const void *firmware, size_t size);
extern void combophy_mcu_exit(void);
extern void combophy_mcu_register_debugfs(struct dentry *root);

#endif /* __HISI_COMBOPHY_MCU_H__ */
