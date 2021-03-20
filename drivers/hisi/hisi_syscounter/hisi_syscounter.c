/*
 * Copyright (C) Huawei Tech. Co. Ltd. 2016-2019. All rights reserved.
 * Description: Syscounter driver
 * Author: Huawei
 * Create: 2016-06-28
 */

#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/of.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/time64.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <asm/arch_timer.h>
#include <linux/hisi/hisi_syscounter.h>
#include <linux/syscore_ops.h>
#include <linux/workqueue.h>
#include <securec.h>

#define RECORD_NEED_SYNC_PERIOD  0

static struct syscnt_device *syscnt_dev;
/* lint -e580 */
int syscounter_to_timespec64(u64 syscnt, struct timespec64 *ts)
{
	struct syscnt_device *dev = syscnt_dev;
	unsigned long flags;
	time64_t sec_delta, r_tv_sec;
	long nsec_delta, r_tv_nsec;
	u64 cnt_delta, r_syscnt;

	if (ts == NULL)
		return -EINVAL;
	if (dev == NULL)
		return -ENXIO;

	spin_lock_irqsave(&dev->sync_lock, flags);
	r_syscnt = dev->record.syscnt;
	r_tv_sec = dev->record.ts.tv_sec;
	r_tv_nsec = dev->record.ts.tv_nsec;
	spin_unlock_irqrestore(&dev->sync_lock, flags);

	if (syscnt >= r_syscnt)
		cnt_delta = syscnt - r_syscnt;
	else
		cnt_delta = r_syscnt - syscnt;

	sec_delta = (time64_t)(cnt_delta / dev->clock_rate);
	nsec_delta = (long)((cnt_delta % dev->clock_rate) * NSEC_PER_SEC / dev->clock_rate);

	if (syscnt >= r_syscnt) {
		ts->tv_sec = r_tv_sec + sec_delta;
		ts->tv_nsec = r_tv_nsec + nsec_delta;
		if (ts->tv_nsec >= NSEC_PER_SEC) {
			ts->tv_sec++;
			ts->tv_nsec -= NSEC_PER_SEC;
		}
	} else {
		ts->tv_sec = r_tv_sec - sec_delta;
		if (r_tv_nsec >= nsec_delta) {
			ts->tv_nsec = r_tv_nsec - nsec_delta;
		} else {
			ts->tv_sec--;
			ts->tv_nsec = (NSEC_PER_SEC - nsec_delta) + r_tv_nsec;
		}
	}

	return 0;
}
EXPORT_SYMBOL(syscounter_to_timespec64);

u64 hisi_get_syscount(void)
{
	struct syscnt_device *dev = syscnt_dev;
	union syscnt_val syscnt;
	unsigned long flags;

	if (dev == NULL || dev->base == NULL)
		return 0;

	spin_lock_irqsave(&dev->r_lock, flags);
	syscnt.val_lh32.l32 = (u32)readl(dev->base + SYSCOUNTER_L32);
	syscnt.val_lh32.h32 = (u32)readl(dev->base + SYSCOUNTER_H32);
	spin_unlock_irqrestore(&dev->r_lock, flags);

	return syscnt.val;
}
EXPORT_SYMBOL(hisi_get_syscount);
/* lint +e580 -e592 */

#if RECORD_NEED_SYNC_PERIOD
static void hisi_syscounter_sync_work(struct work_struct *work)
{
	unsigned long flags;
	struct syscnt_device *d = syscnt_dev;

	if (d == NULL)
		return;

	spin_lock_irqsave(&d->sync_lock, flags);
	get_monotonic_boottime64(&d->record.ts);
	getnstimeofday(&d->record.utc);
	d->record.syscnt = hisi_get_syscount();
	spin_unlock_irqrestore(&d->sync_lock, flags);

	schedule_delayed_work(&d->sync_record_work, round_jiffies_relative(msecs_to_jiffies(d->sync_interval)));

	return;
}
#endif

static int hisi_syscounter_suspend(void)
{
	struct syscnt_device *d = syscnt_dev;

	pr_info("%s ++", __func__);

	if (d == NULL) {
		pr_err("%s syscnt device is NULL\n", __func__);
		return -EXDEV;
	}

#if RECORD_NEED_SYNC_PERIOD
	(void)cancel_delayed_work(&d->sync_record_work);
#endif

	pr_info("%s --", __func__);

	return 0;
}

static void hisi_syscounter_resume(void)
{
	unsigned long flags;
	struct syscnt_device *d = syscnt_dev;

	pr_info("%s ++", __func__);

	if (d == NULL) {
		pr_err("%s syscnt device is NULL\n", __func__);
		return;
	}

	spin_lock_irqsave(&d->sync_lock, flags);
	get_monotonic_boottime64(&d->record.ts);
	getnstimeofday(&d->record.utc);
	d->record.syscnt = hisi_get_syscount();
	spin_unlock_irqrestore(&d->sync_lock, flags);

#if RECORD_NEED_SYNC_PERIOD
	schedule_delayed_work(&d->sync_record_work, round_jiffies_relative(msecs_to_jiffies(d->sync_interval)));
#endif

	pr_info("%s --", __func__);

	return;
}

/* sysfs resume/suspend bits for timekeeping */
static struct syscore_ops hisi_syscounter_syscore_ops = {
	.resume     = hisi_syscounter_resume,
	.suspend    = hisi_syscounter_suspend,
};
/* lint -e446 -e86 */
static int hisi_syscounter_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct syscnt_device *d = NULL;
	int ret;
	unsigned long flags;

	d = kzalloc(sizeof(struct syscnt_device), GFP_KERNEL);
	if (d == NULL) {
		dev_err(dev, "kzalloc mem error\n");
		return -ENOMEM;
	}
	syscnt_dev = d;

	ret = of_property_read_u64(dev->of_node, "clock-rate", &d->clock_rate);
	if (ret != 0) {
		dev_err(dev, "read clock-rate from dts error\n");
		goto err_probe;
	}

#if RECORD_NEED_SYNC_PERIOD
	ret = of_property_read_u32(dev->of_node, "sync-interval", &d->sync_interval);
	if (ret != 0) {
		dev_err(dev, "read sync-interval from dts error\n");
		goto err_probe;
	}
#endif

	d->res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (d->res == NULL) {
		dev_err(dev, "platform_get_resource error\n");
		ret = -ENOMEM;
		goto err_probe;
	}

	d->base = ioremap(d->res->start, resource_size(d->res));
	if (d->base == NULL) {
		dev_err(dev, "ioremap baseaddr error\n");
		ret = -ENOMEM;
		goto err_probe;
	}

	spin_lock_init(&d->r_lock);
	spin_lock_init(&d->sync_lock);

	spin_lock_irqsave(&d->sync_lock, flags);
	get_monotonic_boottime64(&d->record.ts);
	getnstimeofday(&d->record.utc);
	d->record.syscnt = hisi_get_syscount();
	spin_unlock_irqrestore(&d->sync_lock, flags);

	platform_set_drvdata(pdev, d);
	register_syscore_ops(&hisi_syscounter_syscore_ops);

#if RECORD_NEED_SYNC_PERIOD
	INIT_DELAYED_WORK(&d->sync_record_work, hisi_syscounter_sync_work);
	schedule_delayed_work(&d->sync_record_work, round_jiffies_relative(msecs_to_jiffies(d->sync_interval)));
#endif


	dev_info(dev, "%s success\n", __func__);

	return 0;

err_probe:
	kfree(d);
	syscnt_dev = NULL;
	return ret;
}
/* lint +e446 */
static int hisi_syscounter_remove(struct platform_device *pdev)
{
	struct syscnt_device *d = syscnt_dev;

	unregister_syscore_ops(&hisi_syscounter_syscore_ops);
	if (d != NULL) {
		iounmap(d->base);
		kfree(d);
		syscnt_dev = NULL;
	}
	return 0;
}

static struct of_device_id hisi_syscounter_of_match[] = {
	{ .compatible = "hisilicon,syscounter-driver" },
	{ }
};

static struct platform_driver hisi_syscounter_driver = {
	.probe  = hisi_syscounter_probe,
	.remove = hisi_syscounter_remove,
	.driver	= {
		.name =	"hisi-syscounter",
		.owner = THIS_MODULE,/*lint -e64*/
		.of_match_table = of_match_ptr(hisi_syscounter_of_match),
	},
};

static int __init hisi_syscounter_init(void)
{
	return platform_driver_register(&hisi_syscounter_driver);/*lint -e64*/
}

static void __exit hisi_syscounter_exit(void)
{
	platform_driver_unregister(&hisi_syscounter_driver);
}
/* lint +e86 */
module_init(hisi_syscounter_init);
module_exit(hisi_syscounter_exit);

MODULE_AUTHOR("Hisilicon Co. Ltd");
MODULE_DESCRIPTION("Hisi SysCounter Driver");
MODULE_LICENSE("GPL");
