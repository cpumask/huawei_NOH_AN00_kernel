/*
 * hi64xx_resmgr.h
 *
 * codec resmgr driver
 *
 * Copyright (c) 2014-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __HI64XX_RESMGR_H__
#define __HI64XX_RESMGR_H__

#include <sound/soc.h>
#include <linux/notifier.h>
#include <linux/hisi/hi64xx/hi_cdc_ctrl.h>
#include <linux/hisi/hi64xx/hi64xx_irq.h>

enum hi64xx_pll_type {
	PLL_LOW,
	PLL_HIGH,
	PLL_44_1,
	PLL_MAX,
	PLL_NONE = -1
};

enum hi64xx_pll_sw_mode {
	MODE_SINGLE,
	MODE_MULTIPLE,
	MODE_MAX,
	MODE_NONE = -1
};

/* Codec driver should implement these functions for each of its PLL or PLL state */
struct pll_ctrl_func {
	/*  function to turn on this PLL or PLL state */
	int (*turn_on)(struct snd_soc_codec *);
	/*  function to turn off this PLL or PLL state */
	int (*turn_off)(struct snd_soc_codec *);
	/*  return ture if pll is locked */
	bool (*is_locked)(struct snd_soc_codec *);
};

struct resmgr_config {
	/* number of the PLL or PLL state the codec has */
	int pll_num;
	/* codec pll switch mode */
	enum hi64xx_pll_sw_mode pll_sw_mode;
	/* functions to control each PLL or PLL state */
	struct pll_ctrl_func pfn_pll_ctrls[PLL_MAX];
	/* function to get the PLL required for accessing the specified register */
	enum hi64xx_pll_type (*pll_for_reg_access)(struct snd_soc_codec *, unsigned int);
	/* functions to enable/disable micbias */
	int (*enable_micbias)(struct snd_soc_codec *);
	int (*disable_micbias)(struct snd_soc_codec *);
	/* functions to enable/disable ibias */
	int (*enable_ibias)(struct snd_soc_codec *);
	int (*disable_ibias)(struct snd_soc_codec *);
	int (*enable_supply)(struct snd_soc_codec *);
	int (*disable_supply)(struct snd_soc_codec *);
	void (*hi64xx_hs_high_resistance_enable)(struct snd_soc_codec *, bool);
};

struct hi64xx_resmgr {
	/* define datas that should be public */
};

int hi64xx_resmgr_init(struct snd_soc_codec *codec,
	struct hi_cdc_ctrl *cdc_ctrl,
	struct hi64xx_irq *irqmgr,
	const struct resmgr_config *config,
	struct hi64xx_resmgr **resmgr);

void hi64xx_resmgr_deinit(struct hi64xx_resmgr *resmgr);

int hi64xx_resmgr_request_reg_access(struct hi64xx_resmgr *resmgr,
	unsigned int reg_addr);

void hi64xx_resmgr_release_reg_access(struct hi64xx_resmgr *resmgr,
	unsigned int reg_addr);

int hi64xx_resmgr_request_pll(struct hi64xx_resmgr *resmgr,
	enum hi64xx_pll_type pll_type);

int hi64xx_resmgr_release_pll(struct hi64xx_resmgr *resmgr,
	enum hi64xx_pll_type pll_type);

void hi64xx_resmgr_request_micbias(struct hi64xx_resmgr *resmgr);

int hi64xx_resmgr_release_micbias(struct hi64xx_resmgr *resmgr);

void hi64xx_resmgr_pm_get_clk(void);

void hi64xx_resmgr_pm_put_clk(void);

int hi64xx_resmgr_force_release_micbias(struct hi64xx_resmgr *resmgr);

void hi64xx_resmgr_hs_high_resistence_enable(struct hi64xx_resmgr *resmgr, bool enable);

void hi64xx_resmgr_dump(struct hi64xx_resmgr *resmgr);

struct pll_switch_event {
	enum hi64xx_pll_type from;
	enum hi64xx_pll_type to;
};

enum hi64xx_resmgr_event {
	PRE_PLL_SWITCH,
	POST_PLL_SWITCH
};

int hi64xx_resmgr_register_notifier(struct hi64xx_resmgr *resmgr,
	struct notifier_block *nblock);

int hi64xx_resmgr_unregister_notifier(struct hi64xx_resmgr *resmgr,
	struct notifier_block *nblock);

#endif /* __HI64XX_RESMGR_H__ */

