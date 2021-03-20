/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2025. All rights reserved.
 * Description: jpeg jpu
 * Author: Huawei Hisilicon
 * Create: 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef HISI_JPU_H
#define HISI_JPU_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/regulator/driver.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/module.h>

#include <linux/ion.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/hisi-iommu.h>
#ifdef CONFIG_HISI_FB_V600
#include <linux/hisi/hipp_common.h>
#else
#include <linux/hisi/hipp.h>
#endif

#include <soc_jpgdec_interface.h>

#include "hisi_jpu_def.h"
#include "hisi_jpu_common.h"
#include "hisi_jpu_iommu.h"
#include <securec.h>

#define DEV_ERR_IF_FAIL(cond, msg, ...) \
	do {\
		if (cond) {\
			dev_err(dev, msg, ##__VA_ARGS__);\
		} \
	} while (0)

#define JPU_ERR_IF_COND(cond, msg, ...) \
	do {\
		if (cond) {\
			HISI_JPU_ERR(msg, ##__VA_ARGS__);\
		} \
	} while (0)

#define DEV_NAME_JPU            "hisi_jpu"
#define DTS_COMP_JPU_NAME       "hisilicon,hisijpu"
#define REGULATOR_JPU_NAME      "regulator_jpu"

#define IRQ_JPU_DEC_DONE_NAME   "irq_jpu_dec_done"
#define IRQ_JPU_DEC_ERR_NAME    "irq_jpu_dec_err"
#define IRQ_JPU_DEC_OTHER_NAME  "irq_jpu_dec_other"
#define IRQ_JPU_DEC_MERGED_NAME "irq_jpu_dec_merged"

#define JPU_DEC_DONE_TIMEOUT_THRESHOLD_ASIC 400
#define JPU_DEC_DONE_TIMEOUT_THRESHOLD_FPGA (15 * 1000)

#define JPG_FUNC_CLK_DEFAULT_RATE   (554 * 1000000L)
#define JPG_FUNC_CLK_PD_RATE        (238 * 1000000L)
#define JPG_FUNC_CLK_PD_RATE_V510   (104 * 1000000L)
#define JPG_FUNC_CLK_LOW_TEMP       (277 * 1000000L)
#define JPG_FUNC_CLK_LOW_TEMP_V501  (300 * 1000000L)
#define JPG_FUNC_CLK_LOW_TEMP_V510  (415 * 1000000L)

#define JPG_CLK_ENABLE_DELAY  1
#define JPG_MEMORY_REMPAIR_DELAY  400

#ifndef CONFIG_ES_LOW_FREQ
#define JPG_FUNC_CLK_DEFAULT_RATE_V501 (600 * 1000000L)
#else
#define JPG_FUNC_CLK_DEFAULT_RATE_V501 (415 * 1000000L)
#endif

#define JPG_FUNC_CLK_DEFAULT_RATE_V600 (640 * 1000000L)

#define CONFIG_FB_DEBUG_USED

/*
** support platform
*/
typedef enum {
	HISI_KIRIN_970 = 1,
	HISI_DSS_V500,
	HISI_DSS_V501,
	HISI_DSS_V510,
	HISI_DSS_V510_CS,
	HISI_DSS_V600,
	UNSUPPORT_PLATFORM,
}jpeg_dec_platform;

struct hisi_jpu_data_type {
	uint32_t index;
	uint32_t ref_cnt;
	uint32_t fpga_flag;

	struct platform_device *pdev;
	uint32_t jpu_major;
	struct class *jpu_class;
	struct device *jpu_dev;

	char __iomem *jpu_cvdr_base;
	char __iomem *jpu_top_base;
	char __iomem *jpu_dec_base;
	char __iomem *jpu_smmu_base;
	char __iomem *media1_crg_base;
	char __iomem *peri_crg_base;
	char __iomem *pmctrl_base;
	char __iomem *sctrl_base;
#if defined(CONFIG_HISI_FB_V600)
	char __iomem *secadapt_base;
#endif

	uint32_t jpu_done_irq;
	uint32_t jpu_err_irq;
	uint32_t jpu_other_irq;
	uint32_t jpu_merged_irq;

	wait_queue_head_t jpu_dec_done_wq;
	uint32_t jpu_dec_done_flag;

	struct regulator *jpu_regulator;
	struct regulator *media1_regulator;

	const char *jpg_func_clk_name;
	struct clk *jpg_func_clk;

	const char *jpg_platform_name;
	jpeg_dec_platform jpu_support_platform;
	bool is_irq_merge;

	struct sg_table *lb_sg_table;
	uint32_t lb_buf_base;

	uint32_t lb_addr; /* line buffer addr */
	bool power_on;
	struct semaphore blank_sem;

	jpu_dec_reg_t jpu_dec_reg_default;
	jpu_dec_reg_t jpu_dec_reg;
	bool jpu_res_initialized;

	struct jpu_data_t jpu_req;

#if defined(CONFIG_HISI_FB_V501) || defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	struct hipp_common_s *jpgdDrv;
#endif

#if defined(CONFIG_HISI_FB_V600)
	jpgd_secadapt_prop_t jpgd_secadapt_prop;
#endif
};

/* ****************************************************************************
 * FUNCTIONS PROTOTYPES
 */
extern int g_debug_jpu_dec;
extern int g_debug_jpu_dec_job_timediff;

int hisi_jpu_register(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_unregister(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_on(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_off(struct hisi_jpu_data_type *hisijd);

void hisi_jpu_dec_normal_reset(struct hisi_jpu_data_type *hisijd);
void hisi_jpu_dec_error_reset(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_dec_done_config(struct hisi_jpu_data_type *hisijd,
					struct jpu_data_t *jpu_req);
int hisi_jpu_dec_err_config(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_dec_other_config(struct hisi_jpu_data_type *hisijd);

irqreturn_t hisi_jpu_dec_done_isr(int irq, void *ptr);
irqreturn_t hisi_jpu_dec_err_isr(int irq, void *ptr);
irqreturn_t hisi_jpu_dec_other_isr(int irq, void *ptr);
irqreturn_t hisi_jpu_dec_merged_isr(int irq, void *ptr);
void hisi_jpu_dec_interrupt_unmask(struct hisi_jpu_data_type *hisijd);
void hisi_jpu_dec_interrupt_mask(struct hisi_jpu_data_type *hisijd);
void hisi_jpu_dec_interrupt_clear(struct hisi_jpu_data_type *hisijd);

int hisijpu_job_exec(struct hisi_jpu_data_type *hisijd,
		const void __user *argp);

int hisijpu_create_buffer_client(struct hisi_jpu_data_type *hisijd);
void hisijpu_destroy_buffer_client(struct hisi_jpu_data_type *hisijd);
int hisijpu_enable_iommu(struct hisi_jpu_data_type *hisijd);
phys_addr_t hisi_jpu_domain_get_ttbr(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_lb_alloc(struct hisi_jpu_data_type *hisijd);
void hisi_jpu_lb_free(struct hisi_jpu_data_type *hisijd);
int hisi_jpu_check_inbuff_addr(struct hisi_jpu_data_type *hisijd,
			struct jpu_data_t *jpu_req);
int hisi_jpu_check_outbuff_addr(struct hisi_jpu_data_type *hisijd,
				struct jpu_data_t *jpu_req);

#endif /* HISI_JPU_H */
