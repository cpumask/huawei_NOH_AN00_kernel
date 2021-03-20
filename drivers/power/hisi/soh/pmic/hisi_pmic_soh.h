/*
 * hisi_pmic_soh.h
 *
 * pmic soh module
 *
 * Copyright (c) 2018-2020 Huawei Technologies Co., Ltd.
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
#ifndef _HISI_PMIC_SOH_H_
#define _HISI_PMIC_SOH_H_

#include <linux/mfd/hisi_pmic.h>
#include <pmic_interface.h>
#ifdef CONFIG_HISI_COUL_HI6421V700
#include "hisi_hi6421v700_soh.h"
#elif defined(CONFIG_HISI_COUL_HI6421V800)
#include "hisi_hi6421v800_soh.h"
#elif defined(CONFIG_HISI_COUL_HI6555V300)
#include "hisi_hi6555v300_soh.h"
#else
#include "hisi_hi6xxx_soh.h"
#endif

/* reg read and write interface macro */
#define hisi_soh_pmic_reg_read(regAddr)               hisi_pmic_reg_read(regAddr)
#define hisi_soh_pmic_reg_write(regAddr, regval)      hisi_pmic_reg_write((int)(regAddr), (int)(regval))
#define hisi_soh_pmic_regs_read(regAddr, buf, size)   hisi_pmic_array_read((int)(regAddr), (char *)(buf), (int)(size))
#define hisi_soh_pmic_regs_write(regAddr, buf, size)  hisi_pmic_array_write((int)(regAddr), (char *)(buf), (int)(size))
#define SOH_PMIC_INFO
#ifndef SOH_PMIC_INFO
/* pimc soh print interface */
#define hisi_pmic_soh_err(fmt, args...) do {} while (0)
#define hisi_pmic_soh_evt(fmt, args...) do {} while (0)
#define hisi_pmic_soh_inf(fmt, args...) do {} while (0)
#define hisi_pmic_soh_debug(fmt, args...) do {} while (0)
#else
/* pimc soh print interface */
#define hisi_pmic_soh_err(fmt, args...) do { printk(KERN_ERR    "[hisi_soh_pmic]" fmt, ## args); } while (0)
#define hisi_pmic_soh_evt(fmt, args...) do { printk(KERN_WARNING"[hisi_soh_pmic]" fmt, ## args); } while (0)
#define hisi_pmic_soh_inf(fmt, args...) do { printk(KERN_INFO   "[hisi_soh_pmic]" fmt, ## args); } while (0)
#define hisi_pmic_soh_debug(fmt, args...) do {} while (0)

#endif

/* acr mul sel */
enum acr_mul {
	ACR_MUL_35  = 0,
	ACR_MUL_70  = 1,
	ACR_MUL_140 = 2,
	ACR_MUL_280 = 3,
	ACR_MUL_MAX,
};

enum soh_thd_type {
	VOL_MV = 0,
	TEMP,
};

struct soh_pmic_device {
	int pmic_acr_support;
	int pmic_dcr_support;
	int pmic_pd_leak_support;
	int pmic_soh_ovp_support;
	int soh_ovh_irq;
	int soh_ovh_dis_irq;
	struct device *dev;
};

#endif


