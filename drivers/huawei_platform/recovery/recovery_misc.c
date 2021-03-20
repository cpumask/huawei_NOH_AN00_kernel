/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: recovery misc
 * Author: jimingquan
 * Create: 2015-05-19
 */

#include "recovery_misc.h"

#include <linux/input.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/syscalls.h>
#include <linux/types.h>

#include <huawei_platform/log/hw_log.h>
#include <securec.h>

#define HWLOG_TAG MISC_RECOVERY
HWLOG_REGIST();
#define WR 1
#define RE 0

int operate_node(const char *path, char *data, unsigned int len,
	unsigned int write_read)
{
	struct file *fp = NULL;
	unsigned long old_fs;

	if (path == NULL || data == NULL) {
		MISC_LOG_ERR("NULL input\n");
		return -1;
	}
	fp = filp_open(path, O_RDWR, 0644);
	if (IS_ERR(fp)) {
		MISC_LOG_INFO("open misc node fail\n");
		return -1;
	}
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	if (write_read == WR) {
		vfs_write(fp, data, len, &(fp->f_pos));
		vfs_fsync(fp, 0);
	} else {
		vfs_read(fp, data, len, &(fp->f_pos));
	}
	MISC_LOG_INFO("writing has been done\n");
	filp_close(fp, NULL);
	set_fs(old_fs);
	return 0;
}

static int reboot_recovery_misc_write(void)
{
	struct misc_message_type misc_message;
	(void)memset_s(&misc_message, sizeof(misc_message), 0,
		sizeof(misc_message));
	if (operate_node(MISC_NODE_PATH, (char *)&misc_message,
		sizeof(misc_message), RE) == 0)
		MISC_LOG_INFO("read misc info success\n");
	else
		MISC_LOG_INFO("read misc failed\n");
	if (strncpy_s(misc_message.command, sizeof(misc_message.command),
		MISC_BOOT_RECOVERY_STR, strlen(MISC_BOOT_RECOVERY_STR)) != EOK)
		MISC_LOG_INFO("strncpy_s fail\n");
	if (operate_node(MISC_NODE_PATH, (char *)&misc_message,
		sizeof(misc_message), WR) == 0) {
		MISC_LOG_INFO("write boot-recovery into misc success\n");
		return 0;
	}
	MISC_LOG_ERR("write boot-recovery into misc fail\n");
	return -1;
}

static int recovery_notify(struct notifier_block *this, unsigned long code,
	void *buf)
{
	if (buf == NULL) {
		MISC_LOG_INFO("no need to handle\n");
		return 0;
	}
	if (strncmp("recovery", buf, strlen("recovery")) == EOK) {
		MISC_LOG_INFO("reboot recovery detected\n");
		if (reboot_recovery_misc_write() == 0)
			MISC_LOG_INFO("write to misc success\n");
	} else {
		MISC_LOG_INFO("no need to write boot-recovery into misc\n");
	}
	return 0;
}

static struct notifier_block recovery_notifier = {
	.notifier_call = recovery_notify,
};

static int __init misc_recovery_init(void)
{
	int ret = register_reboot_notifier(&recovery_notifier);

	if (ret)
		MISC_LOG_ERR("can't register reboot notifier (err=%d)\n", ret);
	return 0;
}

module_init(misc_recovery_init);

