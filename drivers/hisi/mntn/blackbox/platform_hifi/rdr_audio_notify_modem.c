/*
 * hifi reset notify modem
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include "rdr_audio_notify_modem.h"

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

#include "rdr_print.h"

#define NOTIFY_MODME_NUM 3

struct hifi_reset_node_info {
	char* name;
	dev_t devt;
	struct cdev reset_cdev;
	struct class* class;
	unsigned int hifi_reset_state;
	unsigned int flag;
	wait_queue_head_t wait;
};

static const char* const hifi_state_str[] = {
	"HIFI_RESET_STATE_OFF\n",
	"HIFI_RESET_STATE_READY\n",
	"HIFI_RESET_STATE_INVALID\n",
};

struct hifi_reset_node_info hifi_reset_node[NOTIFY_MODME_NUM] = {
	[0] = {
		.name = "hifi_reset0",
		.hifi_reset_state = HIFI_RESET_STATE_READY,
		.flag = 0,
	},
	[1] = {
		.name = "hifi_reset1",
		.hifi_reset_state = HIFI_RESET_STATE_READY,
		.flag = 0,
	},
	[2] = {
		.name = "hifi_reset2",
		.hifi_reset_state = HIFI_RESET_STATE_READY,
		.flag = 0,
	},
};

static int hifi_reset_open(struct inode *inode, struct file *filp)
{
	unsigned int dev_num = 0;

	if(!inode)
		return -EIO;

	if(!filp)
		return -ENOENT;

	for (dev_num = 0; dev_num < NOTIFY_MODME_NUM; dev_num++) {
		if (MINOR(hifi_reset_node[dev_num].devt) == iminor(inode)) {
			filp->private_data = &hifi_reset_node[dev_num];
			BB_PRINT_PN("hifi reset open succ. devt %d, dev num %u\n",
					hifi_reset_node[dev_num].devt, dev_num);
			return 0;
		}
	}

	BB_PRINT_ERR("hifi reset no dev to open\n");

	return -EFAULT;
}

static int hifi_reset_release(struct inode *inode, struct file *filp)
{
	unsigned int dev_num = 0;

	if(!inode)
		return -EIO;

	if(!filp)
		return -ENOENT;

	for (dev_num = 0; dev_num < NOTIFY_MODME_NUM; dev_num++) {
		if (MINOR(hifi_reset_node[dev_num].devt) == iminor(inode)) {
			filp->private_data = NULL;
			BB_PRINT_PN("hifi reset release succ. devt %d, dev num %u\n",
					hifi_reset_node[dev_num].devt, dev_num);
			return 0;
		}
	}

	BB_PRINT_ERR("hifi reset no dev to release\n");

	return -EFAULT;
}

static ssize_t hifi_reset_read(struct file *filp,
                         char __user *buf, size_t count, loff_t *pos)
{
	ssize_t len = 0;
	struct hifi_reset_node_info *node = NULL;

	if (!filp)
		return -ENOENT;

	if (!buf)
		return -EINVAL;

	if (!pos)
		return -EFAULT;

	node = (struct hifi_reset_node_info *)filp->private_data;
	if (!node) {
		BB_PRINT_ERR("node is null\n");
		return -EIO;
	}

	if (node->hifi_reset_state >= HIFI_RESET_STATE_INVALID) {
		BB_PRINT_ERR("hifi reset state error. %d\n", node->hifi_reset_state);
		return -EFAULT;
	}

	if (wait_event_interruptible(node->wait, (node->flag == 1))) {
		BB_PRINT_PN("hifi reset event state not change\n");
		return -ERESTARTSYS;
	}

	node->flag = 0;
	len = strlen(hifi_state_str[node->hifi_reset_state]) + 1;
	BB_PRINT_PN("hifi reset pos %lld, count %zu, len %zd\n", *pos, count, len);
	if (count < len) {
		BB_PRINT_ERR("hifi reset usr count need larger, count %zu\n", count);
		return -EFAULT;
	}
	if (copy_to_user(buf, hifi_state_str[node->hifi_reset_state], len)) {
		BB_PRINT_ERR("hifi reset copy to user fail\n");
		return -EFAULT;
	}

	return len;
}

static struct file_operations hifi_reset_fops = {
	.owner = THIS_MODULE,
	.open = hifi_reset_open,
	.read = hifi_reset_read,
	.release = hifi_reset_release,
};

static int hifi_reset_dev_init_by_num(dev_t devt, unsigned int dev_num)
{
	int ret;
	struct hifi_reset_node_info *node = &hifi_reset_node[dev_num];

	BB_PRINT_PN("init %s\n", node->name);

	init_waitqueue_head(&node->wait);
	node->devt = MKDEV(MAJOR(devt), dev_num);
	cdev_init(&node->reset_cdev, &hifi_reset_fops);
	ret = cdev_add(&node->reset_cdev, node->devt, 1);
	if (ret) {
		BB_PRINT_ERR("cdev add fail %d\n", ret);
		return -EFAULT;
	}

	node->class = class_create(THIS_MODULE, node->name);
	if (!node->class) {
		BB_PRINT_ERR("class creat fail\n");
		goto class_create_error;
	}

	if (IS_ERR(device_create(node->class , NULL, node->devt, &node, node->name))) {
		BB_PRINT_ERR("device_create fail\n");
		goto device_create_error;
	}

	return 0;

device_create_error:
	class_destroy(node->class);
class_create_error:
	cdev_del(&node->reset_cdev);
	return -1;
}

static void hifi_reset_dev_exit_by_num(unsigned int dev_num)
{
	struct hifi_reset_node_info *node = &hifi_reset_node[dev_num];

	device_destroy(node->class, node->devt);
	class_destroy(node->class);
	cdev_del(&node->reset_cdev);
}

void hifi_reset_dev_exit(void)
{
	unsigned int dev_num;
	dev_t devt = hifi_reset_node[0].devt;

	for (dev_num = 0; dev_num < NOTIFY_MODME_NUM; dev_num++)
		hifi_reset_dev_exit_by_num(dev_num);

	unregister_chrdev_region(MKDEV(MAJOR(devt), 0), NOTIFY_MODME_NUM);
}

int hifi_reset_dev_init(void)
{
	int ret;
	unsigned int dev_num;
	dev_t devt;

	ret = alloc_chrdev_region(&devt, 0, NOTIFY_MODME_NUM, "hifi_reset");
	if (ret) {
		BB_PRINT_ERR("alloc chrdev region fail\n");
		return -EFAULT;
	}

	for (dev_num = 0; dev_num < NOTIFY_MODME_NUM; dev_num++) {
		ret =  hifi_reset_dev_init_by_num(devt, dev_num);
		if (ret) {
			BB_PRINT_ERR("init dev num %d fail. ret %d\n", dev_num, ret);
			goto dev_init_error;
		}
	}

	BB_PRINT_PN("hifi reset dev init succeed\n");

	return 0;

dev_init_error:
	while (dev_num > 0) {
		dev_num--;
		hifi_reset_dev_exit_by_num(dev_num);
	}
	unregister_chrdev_region(MKDEV(MAJOR(devt), 0), NOTIFY_MODME_NUM);

	return -1;
}

void hifi_reset_inform_modem(enum hifi_reset_state state)
{
	unsigned int dev_num;

	if (state >= HIFI_RESET_STATE_INVALID) {
		BB_PRINT_ERR("hifi reset state error. state %d\n", state);
		return;
	}

	for (dev_num = 0; dev_num < NOTIFY_MODME_NUM; dev_num++) {
		hifi_reset_node[dev_num].hifi_reset_state = state;
		hifi_reset_node[dev_num].flag = 1;
		wake_up_interruptible(&hifi_reset_node[dev_num].wait);
	}
}
