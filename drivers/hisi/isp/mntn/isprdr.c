/*
 * record the data to rdr. (RDR: kernel run data recorder.)
 * This file wraps the ring buffer.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/hisi/rdr_pub.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/rproc_share.h>
#include <linux/version.h>
#include <linux/of_irq.h>
#include <linux/platform_data/remoteproc_hisi.h>
#include <securec.h>
#include "soc_mid.h"
#include "isp_ddr_map.h"
#include "isprdr.h"
#include "rdr_print.h"
#include "rdr_inner.h"
#include "rdr_field.h"
#include <linux/hisi/hisi_noc_modid_para.h>

/* Watchdog Regs Offset */
#define ISP_WATCHDOG_WDG_LOCK_ADDR      (0x0C00)
#define ISP_WATCHDOG_WDG_CONTROL_ADDR   (0x0008)
#define ISP_RDR_MAX_SIZE        64
#define ISP_RDR_TMP_SIZE        0x1000
/* SCtrl Regs Offset */

#define SCTRL_SCBAKDATA10_ADDR      (0x434)

#define SOC_MID_MASK 0x3F
/* PCtrl Regs Offset */
#define PCTRL_PERI_CTRL27_ADDR      (0x070)

enum rdr_isp_system_error_type {
	ISP_MODID_START = HISI_BB_MOD_ISP_START,
	ISP_WDT_TIMEOUT = 0x81fffdfe,
	ISP_SYSTEM_STATES,
	ISP_MODID_END = HISI_BB_MOD_ISP_END,
	ISP_SYSTEM_INVALID,
} rdr_isp_sys_err_t;

struct rdr_err_type {
	struct list_head node;
	enum rdr_isp_system_error_type type;
};

struct rdr_sys_err_dump_info {
	struct list_head node;
	u32 modid;
	u64 coreid;
	pfn_cb_dump_done cb;
};

struct rdr_isp_device {
	void __iomem *sctrl_addr;
	void __iomem *wdt_addr;
	void __iomem *pctrl_addr;
	void __iomem *rdr_addr;
	struct workqueue_struct *wq;
	struct work_struct err_work;
	struct work_struct dump_work;
	struct list_head err_list;
	struct list_head dump_list;
	spinlock_t err_list_lock;
	spinlock_t dump_list_lock;
	int wdt_irq;
	bool wdt_enable_flag;
	unsigned int offline_rdrlevel;
	unsigned char irq[IRQ_NUM];
	int isprdr_initialized;
	u64 isprdr_addr;
	u64 core_id;
	struct rdr_register_module_result current_info;
} rdr_isp_dev;

static struct rdr_exception_info_s exc_info[] = {
	[0] = {
		.e_modid = ISP_WDT_TIMEOUT,
		.e_modid_end = ISP_WDT_TIMEOUT,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_AP | RDR_ISP,
		.e_reset_core_mask = RDR_AP,
		.e_reentrant = RDR_REENTRANT_DISALLOW,
		.e_exce_type = ISP_S_ISPWD,
		.e_from_core = RDR_ISP,
		.e_from_module = MODULE_NAME,
		.e_desc = "RDR ISP WDT TIMEOUT",
	},
	[1] = {
		.e_modid = ISP_SYSTEM_STATES,
		.e_modid_end = ISP_SYSTEM_STATES,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_AP | RDR_ISP,
		.e_reset_core_mask = RDR_AP,
		.e_reentrant = RDR_REENTRANT_DISALLOW,
		.e_exce_type = ISP_S_EXCEPTION,
		.e_from_core = RDR_ISP,
		.e_from_module = MODULE_NAME,
		.e_desc = "RDR ISP NOC",
	},
};

struct noc_err_para_s ispnoc_err[] = {
	[0] = {
		.masterid = SOC_ISPCORE_MID & SOC_MID_MASK,
		.targetflow = ISP_TARGETFLOW,
		.bus = NOC_ERRBUS_VIVO,
	},
	[1] = {
		.masterid = SOC_ISPCPU_MID & SOC_MID_MASK,
		.targetflow = ISP_TARGETFLOW,
		.bus = NOC_ERRBUS_VIVO,
	},
};

int hisp_get_wdt_irq(void)
{
	struct device_node *np = NULL;
	char *name = NULL;
	int irq = 0;

	name = hisp_get_comp_name();

	np = of_find_compatible_node(NULL, NULL, name);
	if (!np) {
		pr_err("%s: of_find_compatible_node failed, %s\n",
				__func__, name);
		return -ENXIO;
	}

	irq = irq_of_parse_and_map(np, 0);
	if (!irq) {
		pr_err("%s: irq_of_parse_and_map failed, irq.%d\n",
				__func__, irq);
		return -ENXIO;
	}

	pr_info("%s: comp.%s, wdt irq.%d.\n", __func__, name, irq);
	return irq;
}

u64 get_isprdr_addr(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;

	return dev->isprdr_addr;
}

void __iomem *get_isprdr_va(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;

	return dev->rdr_addr;
}

static void rdr_system_err_dump_work(struct work_struct *work)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	struct rdr_sys_err_dump_info *entry = NULL, *tmp = NULL;
	unsigned int sync;
	int timeout = 20;

	list_for_each_entry_safe(entry, tmp, &dev->dump_list, node) {
		if (entry->modid == ISP_WDT_TIMEOUT) {
			/* check sync word */
			do {
				sync = readl(dev->rdr_addr + RDR_SYNC_WORD_OFF);
				msleep(100);
			} while (sync != RDR_ISP_SYNC && timeout-- > 0);
			pr_info("%s: sync_word = 0x%x, timeout = %d.\n",
					__func__, sync, timeout);
		} else if (entry->modid == ISP_SYSTEM_STATES) {
			pr_info("%s: isp noc arrived.\n", __func__);
		}

		entry->cb(entry->modid, entry->coreid);

		spin_lock(&dev->dump_list_lock);
		list_del(&entry->node);
		spin_unlock(&dev->dump_list_lock);

		kfree(entry);
	}
}
/*lint -save -e429*/
static void rdr_isp_dump(u32 modid, u32 etype, u64 coreid,
		char *pathname, pfn_cb_dump_done pfn_cb)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	struct rdr_sys_err_dump_info *dump_info = NULL;

	pr_info("%s: enter.\n", __func__);
	dump_info = kzalloc(sizeof(struct rdr_sys_err_dump_info), GFP_KERNEL);
	if (dump_info == NULL) {
		pr_err("%s: kzalloc failed.\n", __func__);
		return;
	}

	dump_info->modid = modid;
	dump_info->coreid = dev->core_id;
	dump_info->cb = pfn_cb;

	spin_lock(&dev->dump_list_lock);
	list_add_tail(&dump_info->node, &dev->dump_list);
	spin_unlock(&dev->dump_list_lock);

	queue_work(dev->wq, &dev->dump_work);
	pr_info("%s: exit.\n", __func__);
}
/*lint -restore */
static void rdr_isp_reset(u32 modid, u32 etype, u64 coreid)
{
	pr_info("%s: enter.\n", __func__);
}
/*lint -save -e429*/
void ap_send_fiq2ispcpu(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	u32 exc_flag = 0x0;
	u32 value = 0x0;
	bool wdt_flag = false;
	int ret = 0;

	exc_flag = get_share_exc_flag();
	if (exc_flag == ISP_MAX_NUM_MAGIC) {
		pr_err("%s: get_share_exc_flag failed, exc_flag.0x%x\n",
				__func__, exc_flag);
		return;
	}

	pr_info("%s: exc_flag.0x%x\n", __func__, exc_flag);
	if (wait_share_excflag_timeout(ISP_CPU_POWER_DOWN, 10) == 0x0) {
		if (dev->sctrl_addr == NULL) {
			pr_err("%s: sctrl_addr NULL\n", __func__);
			return;
		}
		value = readl(dev->sctrl_addr + SCTRL_SCBAKDATA10_ADDR);
		if (value & (1 << SC_ISP_WDT_BIT))
			wdt_flag = true;
		if ((wdt_flag) && (dev->sctrl_addr != NULL)) {
			ret = fiq2ispcpu();
			if (ret < 0) {
				pr_err("[%s] fiq2ispcpu.%d\n", __func__, ret);
				return;
			}
		}
		mdelay(50);
	}
}

static irqreturn_t isp_wdt_irq_handler(int irq, void *data)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	struct rdr_err_type *err_info = NULL;
	int ret = 0;

	pr_info("%s:enter.\n", __func__);
	/* disable wdt */
	if (dev->wdt_addr == NULL) {
		pr_err("%s: wdt_addr NULL\n", __func__);
		return IRQ_NONE;
	}

	writel(WDT_UNLOCK, dev->wdt_addr + ISP_WATCHDOG_WDG_LOCK_ADDR);
	writel(0, dev->wdt_addr + ISP_WATCHDOG_WDG_CONTROL_ADDR);
	writel(WDT_LOCK, dev->wdt_addr + ISP_WATCHDOG_WDG_LOCK_ADDR);

	/* init sync work */
	writel(0, dev->rdr_addr + RDR_SYNC_WORD_OFF);
	ret = get_ispcpu_cfg_info();
	if (ret < 0)
		pr_err("%s: get_ispcpu_cfg_info failed, irq.0x%x\n",
				__func__, irq);

	err_info = kzalloc(sizeof(struct rdr_err_type), GFP_ATOMIC);
	if (err_info == NULL)
		return IRQ_NONE;

	err_info->type = ISP_WDT_TIMEOUT;

	spin_lock(&dev->err_list_lock);
	list_add_tail(&err_info->node, &dev->err_list);
	spin_unlock(&dev->err_list_lock);

	queue_work(dev->wq, &dev->err_work);
	pr_info("%s:exit.\n", __func__);
	return IRQ_HANDLED;
}
/*lint -restore */
static void rdr_system_err_work(struct work_struct *work)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	struct rdr_err_type *entry = NULL, *tmp = NULL;

	list_for_each_entry_safe(entry, tmp, &dev->err_list, node) {
		if (entry->type == ISP_WDT_TIMEOUT)
			rdr_system_error(entry->type, dev->wdt_irq, 0);
		else
			rdr_system_error(entry->type, 0, 0);

		spin_lock_irq(&dev->err_list_lock);
		list_del(&entry->node);
		spin_unlock_irq(&dev->err_list_lock);

		kfree(entry);
	}
}

static int rdr_isp_wdt_init(struct rdr_isp_device *dev)
{
	int ret = 0;
	int irq = 0;

	pr_info("%s: enter.\n", __func__);
	if (dev->wdt_enable_flag == 0) {
		pr_info("%s: isp wdt is disabled.\n", __func__);
		return 0;
	}

	irq = hisp_get_wdt_irq();
	if (irq <= 0) {
		pr_err("%s: hisp_get_wdt_irq failed, irq.0x%x\n",
			__func__, irq);
		return -EINVAL;
	}
	dev->wdt_irq = irq;

	ret = request_irq(dev->wdt_irq, isp_wdt_irq_handler,
			  0, "isp wtd hanler", NULL);
	if (ret != 0)
		pr_err("%s: request_irq failed, irq.%d, ret.%d.\n",
			__func__, dev->wdt_irq, ret);

	pr_info("%s: exit.\n", __func__);
	return 0;
}

static void rdr_isp_wdt_exit(struct rdr_isp_device *dev)
{
	pr_info("%s: enter.\n", __func__);
	free_irq(dev->wdt_irq, NULL);
	pr_info("%s: exit.\n", __func__);
}

static int rdr_isp_module_register(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	struct rdr_module_ops_pub module_ops;
	struct rdr_register_module_result ret_info;
	int ret = 0;

	pr_info("%s: enter.\n", __func__);
	module_ops.ops_dump = rdr_isp_dump;
	module_ops.ops_reset = rdr_isp_reset;

	dev->core_id = RDR_ISP;
	ret = rdr_register_module_ops(dev->core_id, &module_ops, &ret_info);
	if (ret != 0) {
		pr_err("%s: rdr_register_module_ops failed! return %d\n",
				__func__, ret);
		return ret;
	}

	dev->current_info.log_addr = ret_info.log_addr;
	dev->current_info.log_len = ret_info.log_len;
	dev->current_info.nve = ret_info.nve;
	dev->isprdr_addr = ret_info.log_addr;
	dev->rdr_addr = hisi_bbox_map((phys_addr_t)dev->isprdr_addr,
			dev->current_info.log_len);
	if (dev->rdr_addr == NULL) {
		pr_err("%s: hisi_bbox_map rdr_addr failed.\n", __func__);
		goto rdr_module_ops_unregister;
	}

	dev->isprdr_initialized = 1;
	pr_info("%s: exit.\n", __func__);
	return 0;

rdr_module_ops_unregister:
	dev->isprdr_initialized = 0;
	ret = rdr_unregister_module_ops(dev->core_id);
	if (ret != 0)
		pr_err("%s: rdr_unregister_module_ops failed! return %d\n",
				__func__, ret);
	return -ENOMEM;
}

static void rdr_isp_module_unregister(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	int ret = 0;

	pr_info("%s: enter.\n", __func__);
	dev->isprdr_initialized = 0;

	hisi_bbox_unmap(dev->rdr_addr);
	dev->rdr_addr = NULL;

	dev->isprdr_addr = 0;

	ret = rdr_unregister_module_ops(dev->core_id);
	if (ret != 0)
		pr_err("%s: rdr_unregister_module_ops failed! return %d\n",
				__func__, ret);

	pr_info("%s: exit.\n", __func__);
}

static void rdr_isp_noc_register(void)
{
	int i = 0;

	if (ISP_NOC_ENABLE == 0)
		return;

	pr_info("%s: enter.\n", __func__);
	for (i = 0; i < ARRAY_SIZE(ispnoc_err); i++) {
		pr_debug("%s: register rdr exception, i = %d, masterid:%d",
				__func__, i, ispnoc_err[i].masterid);

		noc_modid_register(ispnoc_err[i], ISP_SYSTEM_STATES);
	}

	pr_info("%s: exit.\n", __func__);
}

static int rdr_isp_exception_register(struct rdr_isp_device *dev)
{
	int i, ret;

	pr_info("%s: enter.\n", __func__);
	for (i = 0; i < ARRAY_SIZE(exc_info); i++) {
		pr_info("%s: register rdr exception, i = %d, type:%d",
				__func__, i, exc_info[i].e_exce_type);

		if (exc_info[i].e_modid == ISP_WDT_TIMEOUT)
			if (!dev->wdt_enable_flag)
				continue;

		ret = rdr_register_exception(&exc_info[i]);
		if (ret != exc_info[i].e_modid_end) {
			pr_info("%s: rdr_register_exception failed, ret.%d.\n",
					__func__, ret);
			goto rdr_register_exception_fail;
		}
	}

	pr_info("%s: exit.\n", __func__);
	return 0;
rdr_register_exception_fail:
	for (; i >= 0; i--) {
		if (exc_info[i].e_modid == ISP_WDT_TIMEOUT)
			if (!dev->wdt_enable_flag)
				continue;

		if (rdr_unregister_exception(exc_info[i].e_modid) < 0)
			pr_err("%s: rdr_unregister_exception failed.\n",
					__func__);
	}

	return ret;
}

static void rdr_isp_exception_unregister(struct rdr_isp_device *dev)
{
	int i, ret;

	pr_info("%s: enter.\n", __func__);
	for (i = ARRAY_SIZE(exc_info) - 1; i >= 0; i--) {
		pr_info("%s: register rdr exception, i = %d, type:%d",
				__func__, i, exc_info[i].e_exce_type);

		if (exc_info[i].e_modid == ISP_WDT_TIMEOUT)
			if (!dev->wdt_enable_flag)
				continue;

		ret = rdr_unregister_exception(exc_info[i].e_modid);
		if (ret < 0)
			pr_info("%s: rdr_unregister_exception failed, ret.%d.\n",
					__func__, ret);
	}

	pr_info("%s: exit.\n", __func__);
}


static int rdr_isp_dev_map(struct rdr_isp_device *dev)
{
	unsigned int value;
	bool wdt_flag = false;

	pr_info("%s: enter.\n", __func__);

	if (dev == NULL) {
		pr_err("%s: rdr_isp_device is NULL.\n", __func__);
		return -EINVAL;
	}
	dev->wdt_enable_flag = wdt_flag;

	dev->sctrl_addr = get_regaddr_by_pa(SCTRL);
	if (dev->sctrl_addr == NULL) {
		pr_err("%s: ioremp sctrl failed.\n", __func__);
		return -ENOMEM;
	}

	value = readl(dev->sctrl_addr +	SCTRL_SCBAKDATA10_ADDR);

	if (value & (1 << SC_ISP_WDT_BIT))
		wdt_flag = true;

	if (wdt_flag) {
		dev->wdt_addr = get_regaddr_by_pa(WDT);
		if (!dev->wdt_addr) {
			pr_err("%s: ioremp wdt failed.\n", __func__);
			goto err;
		}

		dev->pctrl_addr = get_regaddr_by_pa(PCTRL);
		if (!dev->pctrl_addr) {
			pr_err("%s: ioremp pctrl failed.\n", __func__);
			goto err;
		}
	}

	dev->wdt_enable_flag = wdt_flag;
	pr_info("%s: exit.\n", __func__);
	return 0;

err:
	pr_info("%s: error, exit.\n", __func__);
	return -ENOMEM;
}

int rdr_isp_init(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;
	int ret = 0;
	int i;

	ret = rdr_isp_dev_map(dev);
	if (ret != 0) {
		pr_err("%s: rdr_isp_dev_map failed.\n", __func__);
		return ret;
	}

	ret = rdr_isp_wdt_init(dev);
	if (ret != 0) {
		pr_err("%s: rdr_isp_wdt_init failed.\n", __func__);
		return ret;
	}

	ret = rdr_isp_module_register();
	if (ret != 0) {
		pr_err("%s: rdr_isp_module_register failed.\n", __func__);
		goto rdr_isp_module_register_fail;
	}

	rdr_isp_noc_register();

	ret = rdr_isp_exception_register(dev);
	if (ret != 0) {
		pr_err("%s: rdr_isp_exception_register failed.\n", __func__);
		goto rdr_register_exception_fail;
	}

	dev->wq = create_singlethread_workqueue(MODULE_NAME);
	if (dev->wq == NULL) {
		pr_err("%s: create_singlethread_workqueue failed.\n", __func__);
		ret = -1;
		goto create_singlethread_workqueue_fail;
	}

	INIT_WORK(&dev->dump_work, rdr_system_err_dump_work);
	INIT_WORK(&dev->err_work, rdr_system_err_work);
	INIT_LIST_HEAD(&dev->err_list);
	INIT_LIST_HEAD(&dev->dump_list);

	spin_lock_init(&dev->err_list_lock);
	spin_lock_init(&dev->dump_list_lock);
	dev->offline_rdrlevel = ISPCPU_DEFAULT_RDR_LEVEL;
	for (i = 0; i < IRQ_NUM; i++)
		dev->irq[i] = 0;

	pr_info("[%s] -\n", __func__);

	return 0;

create_singlethread_workqueue_fail:
	rdr_isp_exception_unregister(dev);

rdr_register_exception_fail:
	rdr_isp_module_unregister();

rdr_isp_module_register_fail:
	rdr_isp_wdt_exit(dev);

	return ret;
}

void rdr_isp_exit(void)
{
	struct rdr_isp_device *dev = &rdr_isp_dev;

	destroy_workqueue(dev->wq);
	rdr_isp_exception_unregister(dev);
	rdr_isp_module_unregister();
	rdr_isp_wdt_exit(dev);
}

MODULE_LICENSE("GPL v2");

