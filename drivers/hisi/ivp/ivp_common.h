/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: this file provide initialization of the ivp dev
 * Author: liuqiqi
 * Create: 2020-03-05
 */

#ifndef _IVP_COMMON_H_
#define _IVP_COMMON_H_

#include <linux/fs.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include "ivp.h"

#define DMA_64BIT 64

#define LOGE_AND_RETURN_IF_COND(ret, cond, msg, ...) do { \
	if (cond) { \
		ivp_err(msg, ##__VA_ARGS__); \
		return (ret); \
	} \
} while (0)

struct ivp_common {
	struct clk *clk;
	unsigned int clk_rate;
	unsigned int clk_level;
	unsigned int clk_usrsetrate;
	unsigned int lowfrq_pd_clk_rate;
	unsigned int wdg_irq;
	atomic_t wdg_sleep;
	unsigned int dwaxi_dlock_irq;
	struct semaphore wdg_sem;
	int sect_count;
	struct ivp_sect_info *sects;
	struct ivp_sect_info *sec_sects;
	struct dentry *debugfs;
	struct miscdevice device;
	struct regulator *regulator;
	unsigned long smmu_pgd_base;
	struct ivp_smmu_dev *smmu_dev;
	atomic_t accessible;
	atomic_t poweron_access;
	atomic_t poweron_success;
	void *vaddr_memory;
	struct wakeup_source ivp_power_wakelock;
	struct mutex ivp_wake_lock_mutex;
	struct mutex ivp_load_image_mutex;
	struct mutex ivp_power_up_off_mutex;
	struct mutex ivp_open_release_mutex;
	int ivp_meminddr_len;
	unsigned int dynamic_mem_size;
	unsigned int dynamic_mem_section_size;
	unsigned int ivp_sec_support;
	unsigned int ivp_secmode;
	int ivp_sec_buff_fd;
	void *iram;
	struct platform_device *ivp_pdev;
};

#endif /* _IVP_COMMON_H_ */
