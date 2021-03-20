/*
 * tc_client_sub_driver.c
 *
 * function for preparing and organizing data for tc_client_driver
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
 */
#include "tc_client_sub_driver.h"
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_reserved_mem.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/pid.h>
#include <linux/security.h>
#include <linux/cred.h>
#include <linux/namei.h>
#include <linux/thread_info.h>
#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/path.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include "smc_smp.h"
#include "agent.h"
#include "mem.h"
#include "gp_ops.h"
#include "tc_ns_log.h"
#include "mailbox_mempool.h"
#include "tz_spi_notify.h"
#include "tc_client_driver.h"
#include "dynamic_ion_mem.h"
#include "security_auth_enhance.h"
#include "ko_adapt.h"

static DEFINE_MUTEX(g_load_app_lock);
#define MAX_REF_COUNT (255)

struct tc_ns_service *tc_ref_service_in_dev(struct tc_ns_dev_file *dev, const unsigned char *uuid,
	int uuid_size, bool *is_full)
{
	uint32_t i;

	if (dev == NULL || uuid == NULL || uuid_size != UUID_LEN ||
		is_full == NULL)
		return NULL;
	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] != NULL &&
			memcmp(dev->services[i]->uuid, uuid, UUID_LEN) == 0) {
			if (dev->service_ref[i] == MAX_REF_COUNT) {
				*is_full = true;
				return NULL;
			}
			dev->service_ref[i]++;
			return dev->services[i];
		}
	}
	return NULL;
}

struct tc_ns_service *tc_find_service_in_dev(struct tc_ns_dev_file *dev,
	const unsigned char *uuid, int uuid_size)
{
	uint32_t i;

	if (dev == NULL || uuid == NULL || uuid_size != UUID_LEN)
		return NULL;
	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] != NULL &&
			memcmp(dev->services[i]->uuid, uuid, UUID_LEN) == 0)
			return dev->services[i];
	}
	return NULL;
}

int add_service_to_dev(struct tc_ns_dev_file *dev, struct tc_ns_service *service)
{
	uint32_t i;

	if (dev == NULL || service == NULL)
		return -1;
	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] == NULL) {
			tlogd("add service %u to %u\n", i, dev->dev_file_id);
			dev->services[i] = service;
			dev->service_ref[i] = 1;
			return 0;
		}
	}
	return -1;
}

void del_service_from_dev(struct tc_ns_dev_file *dev, struct tc_ns_service *service)
{
	uint32_t i;

	if (dev == NULL || service == NULL)
		return;
	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] == service) {
			tlogd("dev->service_ref[%u] = %u\n", i, dev->service_ref[i]);
			if (dev->service_ref[i] == 0) {
				tloge("Caution! No service to be deleted!\n");
				break;
			}
			dev->service_ref[i]--;
			if (!dev->service_ref[i]) {
				tlogd("del service %u from %u\n", i, dev->dev_file_id);
				dev->services[i] = NULL;
				put_service_struct(service);
			}
			break;
		}
	}
}

struct tc_ns_session *tc_find_session_withowner(struct list_head *session_list,
	unsigned int session_id, struct tc_ns_dev_file *dev_file)
{
	struct tc_ns_session *session = NULL;
	bool check_value = false;

	check_value = (session_list == NULL || dev_file == NULL);
	if (check_value == true) {
		tloge("session_list or dev_file is Null.\n");
		return NULL;
	}
	list_for_each_entry(session, session_list, head) {
		check_value = (session->session_id == session_id &&
			session->owner == dev_file);
		if (check_value == true)
			return session;
	}
	return NULL;
}

static errno_t init_context(struct tc_ns_client_context *context, const unsigned char *uuid,
	const unsigned int uuid_len)
{
	errno_t sret;

	if (context == NULL || uuid == NULL || uuid_len != UUID_LEN)
		return -1;
	sret = memset_s(context, sizeof(*context), 0, sizeof(*context));
	if (sret != EOK)
		return -1;

	sret = memcpy_s(context->uuid, sizeof(context->uuid), uuid, uuid_len);
	if (sret != EOK)
		return -1;
	return 0;
}

int close_session(struct tc_ns_dev_file *dev, struct tc_ns_session *session,
	const unsigned char *uuid, unsigned int uuid_len, unsigned int session_id)
{
	struct tc_ns_client_context context;
	int ret;
	errno_t sret;
	bool check_value = false;

	check_value = (dev == NULL || session == NULL ||
		uuid == NULL || uuid_len != UUID_LEN);
	if (check_value == true)
		return TEEC_ERROR_GENERIC;
	sret = init_context(&context, uuid, uuid_len);
	if (sret != 0)
		return TEEC_ERROR_GENERIC;
	context.session_id = session_id;
	context.cmd_id = GLOBAL_CMD_ID_CLOSE_SESSION;
	ret = tc_client_call(&context, dev, session,
		TC_CALL_GLOBAL | TC_CALL_SYNC);
	if (ret)
		tloge("close session failed, ret=0x%x\n", ret);
	kill_ion_by_uuid((struct tc_uuid *)(context.uuid));
	return ret;
}

uint32_t tc_ns_get_uid(void)
{
	struct task_struct *task = NULL;
	const struct cred *cred = NULL;
	uint32_t uid;

	rcu_read_lock();
	task = get_current();
	get_task_struct(task);
	rcu_read_unlock();
	cred = koadpt_get_task_cred(task);
	if (cred == NULL) {
		tloge("failed to get uid of the task\n");
		put_task_struct(task);
		return (uint32_t)(-1);
	}

	uid = cred->uid.val;
	put_cred(cred);
	put_task_struct(task);
	tlogd("current uid is %u\n", uid);
	return uid;
}

int get_pack_name_len(struct tc_ns_dev_file *dev_file, const uint8_t *cert_buffer,
	unsigned int cert_buffer_size)
{
	errno_t sret;

	if (dev_file == NULL || cert_buffer == NULL ||
		cert_buffer_size == 0)
		return -ENOMEM;
	sret = memcpy_s(&dev_file->pkg_name_len, sizeof(dev_file->pkg_name_len),
		cert_buffer, sizeof(dev_file->pkg_name_len));
	if (sret != EOK)
		return -ENOMEM;
	tlogd("package_name_len is %u\n", dev_file->pkg_name_len);
	if (dev_file->pkg_name_len == 0 ||
	    dev_file->pkg_name_len >= MAX_PACKAGE_NAME_LEN) {
		tloge("Invalid size of package name len login info!\n");
		return -EINVAL;
	}
	return 0;

}

int get_public_key_len(struct tc_ns_dev_file *dev_file, const uint8_t *cert_buffer,
	unsigned int cert_buffer_size)
{
	errno_t sret;

	if (dev_file == NULL || cert_buffer == NULL ||
		cert_buffer_size == 0)
		return -ENOMEM;
	sret = memcpy_s(&dev_file->pub_key_len, sizeof(dev_file->pub_key_len),
		cert_buffer, sizeof(dev_file->pub_key_len));
	if (sret != EOK)
		return -ENOMEM;
	tlogd("publick_key_len is %u\n", dev_file->pub_key_len);
	if (dev_file->pub_key_len > MAX_PUBKEY_LEN) {
		tloge("Invalid public key length in login info!\n");
		return -EINVAL;
	}
	return 0;
}

bool is_valid_ta_size(const char *file_buffer, unsigned int file_size)
{
	if (file_buffer == NULL || file_size == 0) {
		tloge("invalid load ta size\n");
		return false;
	}
	if (file_size > SZ_8M) {
		tloge("larger than 8M TA is not supportedi, size=%d\n", file_size);
		return false;
	}
	return true;
}

int tc_ns_need_load_image(unsigned int file_id, const unsigned char *uuid,
	unsigned int uuid_len)
{
	int ret = 0;
	int smc_ret;
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	struct mb_cmd_pack *mb_pack = NULL;
	char *mb_param = NULL;

	if (uuid == NULL || uuid_len != UUID_LEN) {
		tloge("invalid uuid\n");
		return -ENOMEM;
	}
	mb_pack = mailbox_alloc_cmd_pack();
	if (mb_pack == NULL) {
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}
	mb_param = mailbox_copy_alloc((void *)uuid, uuid_len);
	if (mb_param == NULL) {
		tloge("alloc mb param failed\n");
		ret = -ENOMEM;
		goto clean;
	}
	mb_pack->operation.paramtypes = TEEC_MEMREF_TEMP_INOUT;
	mb_pack->operation.params[0].memref.buffer = virt_to_phys((void *)mb_param);
	mb_pack->operation.buffer_h_addr[0] = virt_to_phys((void *)mb_param) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[0].memref.size = SZ_4K;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_NEED_LOAD_APP;
	smc_cmd.global_cmd = true;
	smc_cmd.dev_file_id = file_id;
	smc_cmd.context_id = 0;
	smc_cmd.operation_phys = virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys = virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;
	tlogd("secure app load smc command\n");
	smc_ret = tc_ns_smc(&smc_cmd);
	if (smc_ret != 0) {
		tloge("smc_call returns error ret 0x%x\n", smc_ret);
		ret = -1;
		goto clean;
	} else {
		ret = *(int *)mb_param;
	}
clean:
	if (mb_param != NULL)
		mailbox_free(mb_param);
	mailbox_free(mb_pack);

	return ret;
}

int tc_ns_load_secfile(struct tc_ns_dev_file *dev_file,
	const void __user *argp)
{
	int ret;
	struct load_secfile_ioctl_struct ioctl_arg = { 0, {0}, 0, { NULL } };

	if (dev_file == NULL || argp == NULL) {
		tloge("Invalid params !\n");
		return -EINVAL;
	}
	if (copy_from_user(&ioctl_arg, argp, sizeof(ioctl_arg))) {
		tloge("copy from user failed\n");
		ret = -ENOMEM;
		return ret;
	}
	mutex_lock(&g_load_app_lock);
	if (ioctl_arg.secfile_type == LOAD_TA) {
		ret = tc_ns_need_load_image(dev_file->dev_file_id,
			ioctl_arg.uuid, (unsigned int)UUID_LEN);
		if (ret == 1) // 1 means we need to load image
			ret = tc_ns_load_image(dev_file, ioctl_arg.file_buffer,
				ioctl_arg.file_size);
	} else if (ioctl_arg.secfile_type == LOAD_LIB) {
		ret = tc_ns_load_image(dev_file,
			ioctl_arg.file_buffer, ioctl_arg.file_size);
	} else {
		tloge("invalid secfile type: %d!", ioctl_arg.secfile_type);
		ret = -EINVAL;
	}
	if (ret)
		tloge("load TA secfile: %d failed, ret = %x", ioctl_arg.secfile_type, ret);
	mutex_unlock(&g_load_app_lock);
	return ret;
}

int load_ta_image(struct tc_ns_dev_file *dev_file, struct tc_ns_client_context *context)
{
	int ret;

	if (dev_file == NULL || context == NULL)
		return -1;
	mutex_lock(&g_load_app_lock);
	ret = tc_ns_need_load_image(dev_file->dev_file_id, context->uuid,
		(unsigned int)UUID_LEN);
	if (ret == 1) {
		if (context->file_buffer == NULL) {
			tloge("context's file_buffer is NULL");
			mutex_unlock(&g_load_app_lock);
			return -1;
		}
		ret = tc_ns_load_image(dev_file, context->file_buffer,
			context->file_size);
		if (ret) {
			tloge("load image failed, ret=%x", ret);
			mutex_unlock(&g_load_app_lock);
			return ret;
		}
	}
	mutex_unlock(&g_load_app_lock);
	return ret;
}

void release_free_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_session *session)
{
	if (dev_file == NULL || context == NULL || session == NULL)
		return;
#ifdef CONFIG_AUTH_ENHANCE
	if (session != NULL &&
		session->teec_token.token_buffer != NULL) {
		if (memset_s((void *)session->teec_token.token_buffer,
			session->teec_token.token_len,
			0, session->teec_token.token_len) != EOK)
			tloge("Caution, memset failed!\n");
		kfree(session->teec_token.token_buffer);
		session->teec_token.token_buffer = NULL;
	}
#endif
}

void close_session_in_service_list(struct tc_ns_dev_file *dev, struct tc_ns_service *service,
	uint32_t index)
{
	struct tc_ns_session *tmp_session = NULL;
	struct tc_ns_session *session = NULL;
	int ret;

	if (dev == NULL || service == NULL || index >= SERVICES_MAX_COUNT)
		return;
	list_for_each_entry_safe(session, tmp_session,
		&dev->services[index]->session_list, head) {
		if (session->owner != dev)
			continue;
		ret = close_session(dev, session, service->uuid,
			(unsigned int)UUID_LEN, session->session_id);
		if (ret != TEEC_SUCCESS)
			tloge("close session smc(when close fd) failed!\n");
#ifdef CONFIG_AUTH_ENHANCE
		/* Clean session secure information */
		if (memset_s(&session->secure_info,
			sizeof(session->secure_info), 0,
			sizeof(session->secure_info)) != EOK)
			tloge("tc_ns_client_close memset error\n");
#endif
		mutex_lock(&service->session_lock);
		list_del(&session->head);
		mutex_unlock(&service->session_lock);
		put_session_struct(session); /* pair with open session */
	}
}

void close_unclosed_session(struct tc_ns_dev_file *dev, uint32_t index)
{
	struct tc_ns_service *service = NULL;

	if (dev == NULL || index >= SERVICES_MAX_COUNT)
		return;
	if (dev->services[index] != NULL &&
		!list_empty(&dev->services[index]->session_list)) {
		service = dev->services[index];

		mutex_lock(&service->operation_lock);
		close_session_in_service_list(dev, service, index);
		mutex_unlock(&service->operation_lock);

		put_service_struct(service); /* pair with open session */
	}
}
