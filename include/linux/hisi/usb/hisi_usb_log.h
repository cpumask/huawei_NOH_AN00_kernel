// SPDX-License-Identifier: GPL-2.0
/*
 * Log function of Hisilicon Kirin USB Module
 *
 * Copyright (C) 2020-2020 Hisilicon Tech. Co., Ltd.
 *		http://www.huawei.com
 */

#ifndef _HISI_USB_LOG_
#define _HISI_USB_LOG_
#include <linux/printk.h>

#ifdef CONFIG_HISI_USB_LOG
#define hiusb_pr_info(fmt, arg...) pr_info("[HIUSB][I]%s:"fmt, __func__, ##arg)
#define hiusb_pr_err(fmt, arg...) pr_err("[HIUSB][E]%s:"fmt, __func__, ##arg)
#define hiusb_dev_info(dev, fmt, arg...) \
	dev_info(dev, "[HIUSB][I]%s:"fmt, __func__, ##arg)
#define hiusb_dev_err(dev, fmt, arg...) \
	dev_err(dev, "[HIUSB][E]%s:"fmt, __func__, ##arg)
#define hiusb_pr_info_ratelimited(fmt, arg...) \
	pr_info_ratelimited("[HIUSB][I]%s:"fmt, __func__, ##arg)
#define hiusb_pr_err_ratelimited(fmt, arg...) \
	pr_err_ratelimited("[HIUSB][E]%s:"fmt, __func__, ##arg)
#else
#define hiusb_pr_info(fmt, arg...)
#define hiusb_pr_err(fmt, arg...)
#define hiusb_dev_info(dev, fmt, arg...)
#define hiusb_dev_err(dev, fmt, arg...)
#define hiusb_pr_info_ratelimited(fmt, arg...)
#define hiusb_pr_err_ratelimited(fmt, arg...)
#endif /* CONFIG_HISI_USB_LOG */

#endif /* _HISI_USB_LOG_ */
