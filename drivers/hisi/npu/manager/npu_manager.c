/*
 * npu_manager.c
 *
 * about npu manager
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#include "npu_manager.h"

#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/swap.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/atomic.h>
#include <dsm/dsm_pub.h>

#include "npu_proc_ctx.h"
#include "npu_manager_ioctl_services.h"
#include "npu_ioctl_services.h"
#include "npu_calc_channel.h"
#include "npu_calc_cq.h"
#include "npu_stream.h"
#include "npu_shm.h"
#include "npu_common.h"
#include "npu_devinit.h"
#include "npu_log.h"
#include "npu_pm_framework.h"
#include "npu_firmware.h"
#include "npu_recycle.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_mailbox_msg.h"
#include "npu_manager_common.h"
#include "npu_hwts.h"
#include "npu_svm.h"

static unsigned int g_npu_manager_major;
static struct class *g_npu_manager_class = NULL;
static struct devdrv_manager_info *g_dev_manager_info = NULL;

static int (*devdrv_npu_ioctl_call[DEVDRV_MAX_CMD])(struct devdrv_proc_ctx *proc_ctx, unsigned long arg) = {NULL};
static int (*npu_feature_call[DEVDRV_MAX_FEATURE_ID])(struct devdrv_proc_ctx *proc_ctx, uint32_t flag) = {NULL};

struct dsm_dev dev_davinci = {
	.name = "dsm_ai",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = 1024,
};
struct dsm_client *davinci_dsm_client = NULL;

static int devdrv_manager_open(struct inode *inode, struct file *filep)
{
	int ret;
	uint32_t pid;
	NPU_DRV_DEBUG("devdrv_manager_open start\n");

	ret = devdrv_insert_item_bypid(0, current->tgid, current->tgid);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_insert_item_bypid failed\n");
		return ret;
	}
	pid = (uint32_t)current->tgid;
	filep->private_data = (void *)(uintptr_t)pid;
	return 0;
}

static int devdrv_manager_release(struct inode *inode, struct file *filep)
{
	int ret;
	pid_t pid;
	pid = (pid_t)(uintptr_t)filep->private_data;
	NPU_DRV_DEBUG("devdrv_manager_release start\n");
	ret = devdrv_release_item_bypid(0, pid, pid);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_release_item_bypid failed\n");
		return ret;
	}

	return 0;
}

struct devdrv_manager_info *devdrv_get_manager_info(void)
{
	return g_dev_manager_info; /* dev_manager_info */
}
EXPORT_SYMBOL(devdrv_get_manager_info);

const struct file_operations devdrv_manager_fops = {
	.owner = THIS_MODULE,
	.open = devdrv_manager_open,
	.release = devdrv_manager_release,
	.unlocked_ioctl = devdrv_manager_ioctl,
};

static char* devdrv_manager_devnode(struct device *dev, umode_t *mode)
{
	if (mode != NULL)
		*mode = 0666;
	return NULL;
}

static int devdrv_register_manager_chrdev(dev_t npu_manager_dev, dev_t devno)
{
	struct device *i_device = NULL;
	int ret;

	ret = alloc_chrdev_region(&npu_manager_dev, 0, DEVDRV_MAX_DAVINCI_NUM, "npu_manager");
	if (ret != 0) {
		NPU_DRV_ERR("npu manager alloc_chrdev_region error\n");
		return -1;
	}

	g_dev_manager_info->cdev.owner = THIS_MODULE;
	g_npu_manager_major = MAJOR(npu_manager_dev);
	devno = MKDEV(g_npu_manager_major, 0);

	g_npu_manager_class = class_create(THIS_MODULE, "npu_manager");
	if (IS_ERR(g_npu_manager_class)) {
		NPU_DRV_ERR("npu_manager_class create error\n");
		ret = -EINVAL;
		goto class_fail;
	}

	g_npu_manager_class->devnode = devdrv_manager_devnode;

	cdev_init(&g_dev_manager_info->cdev, &devdrv_manager_fops);
	ret = cdev_add(&g_dev_manager_info->cdev, devno, DEVDRV_MAX_DAVINCI_NUM);
	if (ret != 0) {
		NPU_DRV_ERR("npu manager cdev_add error\n");
		goto cdev_setup_fail;
	}

	i_device = device_create(g_npu_manager_class, NULL, devno, NULL, "davinci_manager");
	if (i_device == NULL) {
		NPU_DRV_ERR("npu manager device_create error\n");
		ret = -ENODEV;
		goto device_create_fail;
	}

	return ret;
device_create_fail:
	cdev_del(&g_dev_manager_info->cdev);
cdev_setup_fail:
	class_destroy(g_npu_manager_class);
class_fail:
	unregister_chrdev_region(npu_manager_dev, DEVDRV_MAX_DAVINCI_NUM);
	return ret;
}

static void devdrv_unregister_manager_chrdev(dev_t npu_manager_dev, dev_t devno)
{
	device_destroy(g_npu_manager_class, devno);
	cdev_del(&g_dev_manager_info->cdev);
	class_destroy(g_npu_manager_class);
	unregister_chrdev_region(npu_manager_dev, DEVDRV_MAX_DAVINCI_NUM);
}

static void devdrv_npu_ioctl_call_init(void)
{
	int i;
	for (i = 0; i < DEVDRV_MAX_CMD; i++)
		devdrv_npu_ioctl_call[i] = NULL;

	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ALLOC_STREAM_ID)] = devdrv_ioctl_alloc_stream;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_FREE_STREAM_ID)] = devdrv_ioctl_free_stream;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_GET_OCCUPY_STREAM_ID)] = devdrv_ioctl_get_occupy_stream_id;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ALLOC_EVENT_ID)] = devdrv_ioctl_alloc_event;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_FREE_EVENT_ID)] = devdrv_ioctl_free_event;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ALLOC_MODEL_ID)] = devdrv_ioctl_alloc_model;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_REPORT_SYNC_WAIT)] = devdrv_ioctl_report_sync_wait;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_FREE_MODEL_ID)] = devdrv_ioctl_free_model;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_REQUEST_SEND)] = devdrv_ioctl_send_request;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_RESPONSE_RECEIVE)] = devdrv_ioctl_receive_response;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ALLOC_TASK_ID)] = devdrv_ioctl_alloc_task;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_FREE_TASK_ID)] = devdrv_ioctl_free_task;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_REPORT_WAIT)] = devdrv_ioctl_report_wait;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_GET_TS_TIMEOUT_ID)] = devdrv_ioctl_get_ts_timeout;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ATTACH_SYSCACHE)] = devdrv_ioctl_attach_syscache;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_SVM_BIND_PID)] = devdrv_ioctl_svm_bind_pid;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_SVM_UNBIND_PID)] = devdrv_ioctl_svm_unbind_pid;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_CUSTOM_IOCTL)] = devdrv_ioctl_custom;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ENTER_WORKMODE)] = devdrv_ioctl_enter_workwode;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_EXIT_WORKMODE)] = devdrv_ioctl_exit_workwode;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_SET_LIMIT)] = devdrv_ioctl_set_limit;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_ENABLE_FEATURE)] = npu_ioctl_enable_feature;
	devdrv_npu_ioctl_call[_IOC_NR(DEVDRV_DISABLE_FEATURE)] = npu_ioctl_disable_feature;
}

int devdrv_proc_npu_ioctl_call(struct devdrv_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg)
{
	int ret;

	if (cmd < _IO(DEVDRV_ID_MAGIC, 1) || cmd >= _IO(DEVDRV_ID_MAGIC, DEVDRV_MAX_CMD)) {
		NPU_DRV_ERR("parameter,arg = 0x%lx, cmd = %d\n", arg, cmd);
		return -EINVAL;
	}

	NPU_DRV_DEBUG("IOC_NR = %d  cmd = %d\n", _IOC_NR(cmd), cmd);

	if (devdrv_npu_ioctl_call[_IOC_NR(cmd)] == NULL) {
		NPU_DRV_ERR("devdrv_proc_npu_ioctl_call invalid cmd = %d\n", cmd);
		return -EINVAL;
	}

	// porcess ioctl
	ret = devdrv_npu_ioctl_call[_IOC_NR(cmd)](proc_ctx, arg);
	if (ret != 0) {
		NPU_DRV_ERR("process failed,arg = %d\n", cmd);
		return -EINVAL;
	}

	return ret;
}

int npu_feature_enable(struct devdrv_proc_ctx *proc_ctx, uint32_t feature_id, uint32_t enable)
{
	int ret = 0;

	if(feature_id >= DEVDRV_MAX_FEATURE_ID) {
		NPU_DRV_ERR("feature id %u is invalid\n", feature_id);
		return -EINVAL;
	}
	if (npu_feature_call[feature_id] == NULL) {
		NPU_DRV_ERR("npu feature call invalid feature id = %d\n", feature_id);
		return -EINVAL;
	}
	ret = npu_feature_call[feature_id](proc_ctx, enable);
	if (ret != 0) {
		NPU_DRV_ERR("feature id = %u, enable = %u failed\n", feature_id, enable);
		return -EINVAL;
	}

	return ret;
}

static void devdrv_npu_feature_call_init(void)
{
	int i;
	for (i = 0; i < DEVDRV_MAX_FEATURE_ID; i++)
		npu_feature_call[i] = NULL;

	npu_feature_call[DEVDRV_INTERFRAME_FEATURE_ID] = npu_interframe_enable;
}

static int __init devdrv_manager_init(void)
{
	const dev_t npu_manager_dev = 0;
	const dev_t npu_manager_devno = 0;
	const u8 dev_id = 0;
	int ret;
	struct devdrv_platform_info *plat_info = NULL;

	NPU_DRV_DEBUG("npu dev %d drv_manager_init start \n", dev_id);

	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -ENODEV, "devdrv_plat_get_info failed\n");

	/* bbox black box init */
	ret = npu_black_box_init();
	COND_RETURN_ERROR(ret != 0, ret, "Failed npu_black_box_init ! ret = %d\n", ret);

	ret = npu_noc_register();
	if (ret != 0) {
		NPU_DRV_ERR("Failed npu_noc_register ! ret = %d\n", ret);
	}

	g_dev_manager_info = kzalloc(sizeof(struct devdrv_manager_info), GFP_KERNEL);
	COND_RETURN_ERROR(g_dev_manager_info == NULL, -ENOMEM, "kzalloc npu g_dev_manager_info failed\n");

	g_dev_manager_info->plat_info = DEVDRV_MANAGER_DEVICE_ENV;
	ret = devdrv_register_manager_chrdev(npu_manager_dev, npu_manager_devno);
	COND_GOTO_ERROR(ret != 0, register_manager_chrdev_failed, ret, -ENODEV,
		"register npu manager chrdev failed ret = %d \n", ret);

	devdrv_npu_ioctl_call_init();
	devdrv_npu_feature_call_init();

	// init npu powerup or powerdown register info
	devdrv_register_callbak_info_init();

	if (davinci_dsm_client == NULL) {
		davinci_dsm_client = dsm_register_client(&dev_davinci);
		if (davinci_dsm_client == NULL) {
			NPU_DRV_ERR("dsm_register_client register fail\n");
		}
	}
	// npu device resoure init
	ret = devdrv_devinit(THIS_MODULE, dev_id);
	COND_GOTO_ERROR(ret != 0, npu_devinit_failed, ret, -ENODEV, "npu dev %d devdrv_devinit failed\n", dev_id);

	ret = devdrv_request_cq_report_irq_bh();
	COND_GOTO_ERROR(ret != 0, request_cq_report_irq_failed, ret, ret, "devdrv_request_cq_report_irq_bh failed \n");

	NPU_DRV_DEBUG("devdrv_manager_init success\n");
	return ret;

request_cq_report_irq_failed:
npu_devinit_failed:
	devdrv_unregister_manager_chrdev(npu_manager_dev, npu_manager_devno);
register_manager_chrdev_failed:
	kfree(g_dev_manager_info);
	g_dev_manager_info = NULL;
	NPU_DRV_ERR("npu devdrv_manager_init failed\n");
	return ret;
}

module_init(devdrv_manager_init);

static void __exit devdrv_manager_exit(void)
{
	dev_t npu_manager_dev = 0;
	dev_t npu_manager_devno = 0;
	const u8 dev_id = 0; // default npu 0 on lite plat

	if (davinci_dsm_client != NULL) {
		dsm_unregister_client(davinci_dsm_client, &dev_davinci);
		davinci_dsm_client = NULL;
	}
	// free irq and kill tasklet
	(void)devdrv_free_cq_report_irq_bh();
	// dev resource unregister
	(void)devdrv_devexit(dev_id);
	// unregister npu manager char dev
	devdrv_unregister_manager_chrdev(npu_manager_dev, npu_manager_devno);
	// free mem
	if (g_dev_manager_info != NULL) {
		kfree(g_dev_manager_info);
		g_dev_manager_info = NULL;
	}

	/* bbox black box uninit */
	(void)npu_black_box_exit();
}

module_exit(devdrv_manager_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd");
MODULE_DESCRIPTION("DAVINCI driver");
MODULE_VERSION("V1.0");

