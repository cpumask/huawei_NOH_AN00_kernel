/*
 * tc_client_driver.c
 *
 * function for proc open,close session and invoke
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
#include "tc_client_driver.h"
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
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
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/security.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/acpi.h>
#include "tc_client_sub_driver.h"
#include "smc_smp.h"
#include "teek_client_constants.h"
#include "agent.h"
#include "mem.h"
#include "gp_ops.h"
#include "tc_ns_log.h"
#include "tc_ns_client.h"
#include "mailbox_mempool.h"
#include "tz_spi_notify.h"
#include "static_ion_mem.h"
#include "dynamic_ion_mem.h"
#include "teec_daemon_auth.h"
#include "security_auth_enhance.h"
#include "client_hash_auth.h"
#include "tui.h"
#include "auth_base_impl.h"
#include "tlogger.h"
#include "tzdebug.h"
#include "tee_s4.h"

// global reference start
static dev_t g_tc_ns_client_devt;
static struct class *g_driver_class = NULL;
static struct cdev g_tc_ns_client_cdev;
static struct device_node *g_np = NULL;


// record device_file count
static unsigned int g_device_file_cnt = 1;
static DEFINE_MUTEX(g_device_file_cnt_lock);

// dev node list and itself has mutex to avoid race
struct tc_ns_dev_list g_tc_ns_dev_list;
DEFINE_MUTEX(g_service_list_lock);

static struct task_struct *g_teecd_task = NULL;

// record all service node and need mutex to avoid race
struct list_head g_service_list;

struct load_image_params {
	struct tc_ns_dev_file *dev_file;
	char *file_buffer;
	unsigned int file_size;
};

void get_session_struct(struct tc_ns_session *session)
{
	if (session != NULL)
		atomic_inc(&session->usage);
}

void put_session_struct(struct tc_ns_session *session)
{
	if (session == NULL || !atomic_dec_and_test(&session->usage))
		return;

#ifdef CONFIG_AUTH_ENHANCE
	if (session->teec_token.token_buffer != NULL) {
		if (memset_s(
			(void *)session->teec_token.token_buffer,
			session->teec_token.token_len, 0,
			session->teec_token.token_len) != EOK)
			tloge("Caution, memset failed!\n");
		kfree(session->teec_token.token_buffer);
		session->teec_token.token_buffer = NULL;
		(void)session->teec_token.token_buffer; /* avoid Codex warning */
	}
#endif
	if (memset_s((void *)session, sizeof(*session), 0, sizeof(*session)) != EOK)
		tloge("Caution, memset failed!\n");
	kfree(session);
}

struct tc_ns_dev_list *get_dev_list(void)
{
	return &g_tc_ns_dev_list;
}

void get_service_struct(struct tc_ns_service *service)
{
	if (service != NULL) {
		atomic_inc(&service->usage);
		tlogd("service->usage = %d\n", atomic_read(&service->usage));
	}
}

void put_service_struct(struct tc_ns_service *service)
{
	if (service != NULL) {
		tlogd("service->usage = %d\n", atomic_read(&service->usage));
		mutex_lock(&g_service_list_lock);
		if (atomic_dec_and_test(&service->usage)) {
			tlogd("del service [0x%x] from service list\n",
				*(uint32_t *)service->uuid);
			list_del(&service->head);
			kfree(service);
		}
		mutex_unlock(&g_service_list_lock);
	}
}

/* Modify the client context so params id 2 and 3 contain temp pointers to the
 * public key and package name for the open session. This is used for the
 * TEEC_LOGIN_IDENTIFY open session method
 */
static int set_login_information(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	/* The daemon has failed to get login information or not supplied */
	if (dev_file->pkg_name_len == 0)
		return -1;
	/* The 3rd parameter buffer points to the pkg name buffer in the
	 * device file pointer
	 * get package name len and package name
	 */
	context->params[3].memref.size_addr =
		(__u64)(uintptr_t)&dev_file->pkg_name_len;
	context->params[3].memref.buffer =
		(__u64)(uintptr_t)dev_file->pkg_name;
	/* Set public key len and public key */
	if (dev_file->pub_key_len != 0) {
		context->params[2].memref.size_addr =
			(__u64)(uintptr_t)&dev_file->pub_key_len;
		context->params[2].memref.buffer =
			(__u64)(uintptr_t)dev_file->pub_key;
	} else {
		/* If get public key failed, then get uid in kernel */
		uint32_t ca_uid = tc_ns_get_uid();

		if (ca_uid == (uint32_t)(-1)) {
			tloge("Failed to get uid of the task\n");
			goto error;
		}
		dev_file->pub_key_len = sizeof(ca_uid);
		context->params[2].memref.size_addr =
			(__u64)(uintptr_t)&dev_file->pub_key_len;
		if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, &ca_uid,
			dev_file->pub_key_len)) {
			tloge("Failed to copy pubkey, pub_key_len=%u\n",
			      dev_file->pub_key_len);
			goto error;
		}
		context->params[2].memref.buffer =
			(__u64)(uintptr_t)dev_file->pub_key;
	}
	/* Now we mark the 2 parameters as input temp buffers */
	context->param_types = teec_param_types(
		teec_param_type_get(context->param_types, 0),
		teec_param_type_get(context->param_types, 1),
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	return 0;
error:
	return -1;
}

static int check_process_and_alloc_params(struct tc_ns_dev_file *dev_file,
	uint8_t **cert_buffer, unsigned int *cert_buffer_size)
{
	int ret;
	ret = check_teecd_access();
	if (ret != EOK) {
		tloge(KERN_ERR "tc client login verification failed: 0x%x!\n", ret);
		return -EPERM;
	}

	mutex_lock(&dev_file->login_setup_lock);
	if (dev_file->login_setup) {
		tloge("Login information cannot be set twice!\n");
		mutex_unlock(&dev_file->login_setup_lock);
		return -EINVAL;
	}
	dev_file->login_setup = true;
	mutex_unlock(&dev_file->login_setup_lock);

	*cert_buffer_size = (unsigned int)(MAX_PACKAGE_NAME_LEN + MAX_PUBKEY_LEN +
		sizeof(dev_file->pkg_name_len) + sizeof(dev_file->pub_key_len));
	*cert_buffer = kmalloc(*cert_buffer_size, GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)(*cert_buffer))) {
		tloge("Failed to allocate login buffer!");
		return -EFAULT;
	}

	return 0;
}

static int tc_ns_get_tee_version(const struct tc_ns_dev_file *dev_file, void __user *argp)
{
	unsigned int version;
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int smc_ret = 0;
	struct mb_cmd_pack *mb_pack = NULL;
	bool check_value = (g_teecd_task == current->group_leader) &&
		(!tc_ns_get_uid());

	if (argp == NULL) {
		tloge("error input parameter\n");
		return -1;
	}
	if (check_value == false) {
		tloge("ioctl is not from teecd and return\n");
		return -1;
	}
	mb_pack = mailbox_alloc_cmd_pack();
	if (mb_pack == NULL) {
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}

	mb_pack->operation.paramtypes = TEEC_VALUE_OUTPUT;
	smc_cmd.global_cmd = true;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_GET_TEE_VERSION;
	smc_cmd.dev_file_id = dev_file->dev_file_id;
	smc_cmd.operation_phys = virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys =
		virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;
	smc_ret = tc_ns_smc(&smc_cmd);
	tlogd("smc cmd ret %d\n", smc_ret);
	if (smc_ret != 0)
		tloge("smc_call returns error ret 0x%x\n", smc_ret);

	version = mb_pack->operation.params[0].value.a;
	if (copy_to_user(argp, &version, sizeof(unsigned int))) {
		if (smc_ret != 0)
			smc_ret = -EFAULT;
	}
	mailbox_free(mb_pack);
	return smc_ret;
}

#define MAX_BUF_LEN 4096
static int tc_ns_client_login_func(struct tc_ns_dev_file *dev_file,
	const void __user *buffer)
{
	int ret = -EINVAL;
	uint8_t *cert_buffer = NULL;
	uint8_t *temp_cert_buffer = NULL;
	errno_t sret;
	unsigned int cert_buffer_size = 0;

	if (buffer == NULL) {
		/* We accept no debug information because the daemon might  have failed */
		tlogd("No debug information\n");
		dev_file->pkg_name_len = 0;
		dev_file->pub_key_len = 0;
		return 0;
	}
	ret = check_process_and_alloc_params(dev_file, &cert_buffer,
		&cert_buffer_size);
	if (ret != 0)
		return ret;
	temp_cert_buffer = cert_buffer;
	/* GET PACKAGE NAME AND APP CERTIFICATE:
	 * The proc_info format is as follows:
	 * package_name_len(4 bytes) || package_name ||
	 * apk_cert_len(4 bytes) || apk_cert.
	 * or package_name_len(4 bytes) || package_name
	 * || exe_uid_len(4 bytes) || exe_uid.
	 * The apk certificate format is as follows:
	 * modulus_size(4bytes) ||modulus buffer
	 * || exponent size || exponent buffer
	 */
	if (cert_buffer_size > MAX_BUF_LEN) {
		tloge("cert buffer size is invalid!\n");
		ret = -EINVAL;
		goto error;
	}
	if (copy_from_user(cert_buffer, buffer, cert_buffer_size)) {
		tloge("Failed to get user login info!\n");
		ret = -EINVAL;
		goto error;
	}
	/* get package name len */
	ret = get_pack_name_len(dev_file, cert_buffer, cert_buffer_size);
	if (ret != 0)
		goto error;
	cert_buffer += sizeof(dev_file->pkg_name_len);

	/* get package name */
	sret = strncpy_s(dev_file->pkg_name, MAX_PACKAGE_NAME_LEN, cert_buffer,
		dev_file->pkg_name_len);
	if (sret != EOK) {
		ret = -ENOMEM;
		goto error;
	}
	tlogd("package name is %s\n", dev_file->pkg_name);
	cert_buffer += dev_file->pkg_name_len;

	/* get public key len */
	ret = get_public_key_len(dev_file, cert_buffer, cert_buffer_size);
	if (ret != 0)
		goto error;

	/* get public key */
	if (dev_file->pub_key_len != 0) {
		cert_buffer += sizeof(dev_file->pub_key_len);
		if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, cert_buffer,
			dev_file->pub_key_len)) {
			tloge("Failed to copy cert, pub_key_len=%u\n",
				dev_file->pub_key_len);
			ret = -EINVAL;
			goto error;
		}
		cert_buffer += dev_file->pub_key_len;
	}
	ret = 0;
error:
	kfree(temp_cert_buffer);
	return ret;
}

static int alloc_for_load_image(unsigned int *mb_load_size,
	unsigned int file_size, char **mb_load_mem,
	struct mb_cmd_pack **mb_pack, struct tc_uuid **uuid_return)
{
	/* we will try any possible to alloc mailbox mem to load TA */
	for (; *mb_load_size > 0; *mb_load_size >>= 1) {
		*mb_load_mem = mailbox_alloc(*mb_load_size, 0);
		if (*mb_load_mem != NULL)
			break;
		tlogw("alloc mem(size=%u) for TA load mem fail, will retry\n", *mb_load_size);
	}
	if (*mb_load_mem == NULL) {
		tloge("alloc TA load mem failed\n");
		return -ENOMEM;
	}
	*mb_pack = mailbox_alloc_cmd_pack();
	if (*mb_pack == NULL) {
		mailbox_free(*mb_load_mem);
		*mb_load_mem = NULL;
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}
	*uuid_return = mailbox_alloc(sizeof(struct tc_uuid), 0);
	if (*uuid_return == NULL) {
		mailbox_free(*mb_load_mem);
		*mb_load_mem = NULL;
		mailbox_free(*mb_pack);
		*mb_pack = NULL;
		tloge("alloc uuid failed\n");
		return -ENOMEM;
	}
	return 0;
}

static void pack_data_for_smc_cmd(uint32_t load_size,
	const char *mb_load_mem, struct mb_cmd_pack *mb_pack,
	struct tc_uuid *uuid_return, struct tc_ns_smc_cmd *smc_cmd)
{
	mb_pack->operation.params[0].memref.buffer =
		virt_to_phys((void *)mb_load_mem);
	mb_pack->operation.buffer_h_addr[0] =
		virt_to_phys((void *)mb_load_mem) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[0].memref.size = load_size + sizeof(int);
	mb_pack->operation.params[2].memref.buffer =
		virt_to_phys((void *)uuid_return);
	mb_pack->operation.buffer_h_addr[2] =
		virt_to_phys((void *)uuid_return) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[2].memref.size = sizeof(*uuid_return);
	mb_pack->operation.paramtypes = teec_param_types(TEEC_MEMREF_TEMP_INOUT,
		TEEC_VALUE_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INPUT);
	/* load image smc command */
	smc_cmd->global_cmd = true;
	smc_cmd->cmd_id = GLOBAL_CMD_ID_LOAD_SECURE_APP;
	smc_cmd->context_id = 0;
	smc_cmd->operation_phys = virt_to_phys(&mb_pack->operation);
	smc_cmd->operation_h_phys =
		virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;
}

#ifdef CONFIG_DYNAMIC_ION
static int load_image_for_ion(const struct mb_cmd_pack *mb_pack,
	const struct load_image_params *params_in, const struct tc_uuid *uuid_return)
{
	int ret = 0;
	/* check need to add ionmem  */
	uint32_t configid = mb_pack->operation.params[1].value.a;
	uint32_t ion_size = mb_pack->operation.params[1].value.b;
	int32_t check_result = (configid != 0 && ion_size != 0);

	tloge("check load by ION result %d : configid =%d,ion_size =%d,uuid=%x\n",
		check_result, configid, ion_size, uuid_return->time_low);
	if (check_result) {
		ret = load_app_use_configid(configid, params_in->dev_file->dev_file_id,
			uuid_return, ion_size);
		if (ret != 0) {
			tloge("load_app_use_configid failed ret =%d\n", ret);
			return -1;
		}
	}
	return ret;
}
#endif

static int load_image_by_frame(const struct load_image_params *params_in,
	unsigned int mb_load_size, char *mb_load_mem,
	struct mb_cmd_pack *mb_pack, struct tc_uuid *uuid_return,
	unsigned int load_times)
{
	char *p = mb_load_mem;
	uint32_t load_size;
	int load_flag = 1; /* 0:it's last block, 1:not last block */
	uint32_t loaded_size = 0;
	unsigned int index;
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int smc_ret = 0;

	for (index = 0; index < load_times; index++) {
		if (index == (load_times - 1)) {
			load_flag = 0;
			load_size = params_in->file_size - loaded_size;
		} else {
			load_size = mb_load_size - sizeof(load_flag);
		}
		*(int *)p = load_flag;
		if (load_size > mb_load_size - sizeof(load_flag)) {
			tloge("invalid load size %u/%u\n", load_size,
				mb_load_size);
			return  -1;
		}
		if (copy_from_user(mb_load_mem + sizeof(load_flag),
			(void __user *)params_in->file_buffer +
			loaded_size, load_size)) {
			tloge("file buf get fail\n");
			return  -1;
		}
		pack_data_for_smc_cmd(load_size, mb_load_mem, mb_pack,
			uuid_return, &smc_cmd);
		mb_pack->operation.params[3].value.a = index;
		smc_cmd.dev_file_id = params_in->dev_file->dev_file_id;
		smc_ret = tc_ns_smc(&smc_cmd);
		tlogd("smc cmd ret %d\n", smc_ret);
		tlogd("configid=%u, ret=%d, load_flag=%d, index=%u\n",
			mb_pack->operation.params[1].value.a, smc_ret,
			load_flag, index);

		if (smc_ret != 0) {
			tloge("smc_call returns error ret 0x%x\n", smc_ret);
			return -1;
		}
#ifdef CONFIG_DYNAMIC_ION
		if (smc_ret == 0 && load_flag == 0) {
			if (load_image_for_ion(mb_pack, params_in,
				uuid_return) != 0)
				return -1;
		}
#endif
		loaded_size += load_size;
	}
	return 0;
}

int tc_ns_load_image(struct tc_ns_dev_file *dev_file, char *file_buffer,
	unsigned int file_size)
{
	int ret;
	struct mb_cmd_pack *mb_pack = NULL;
	unsigned int mb_load_size;
	char *mb_load_mem = NULL;
	struct tc_uuid *uuid_return = NULL;
	unsigned int load_times;
	struct load_image_params params_in = {dev_file, file_buffer, file_size};
	bool check_value = false;

	check_value = (dev_file == NULL || file_buffer == NULL);
	if (check_value) {
		tloge("dev_file or file_buffer is NULL!\n");
		return -EINVAL;
	}
	if (!is_valid_ta_size(file_buffer, file_size))
		return -EINVAL;
	mb_load_size = file_size > (SZ_1M - sizeof(int)) ?
		SZ_1M : ALIGN(file_size, SZ_4K);
	ret = alloc_for_load_image(&mb_load_size, file_size, &mb_load_mem,
		&mb_pack, &uuid_return);
	if (ret != 0)
		return ret;
	if (mb_load_size <= sizeof(int)) {
		tloge("mb_load_size is too small!\n");
		ret = -ENOMEM;
		goto free_mem;
	}
	load_times = file_size / (mb_load_size - sizeof(int));
	if (file_size % (mb_load_size - sizeof(int)))
		load_times += 1;
	ret = load_image_by_frame(&params_in, mb_load_size, mb_load_mem,
		mb_pack, uuid_return, load_times);
free_mem:
	mailbox_free(mb_load_mem);
	mailbox_free(mb_pack);
	mailbox_free(uuid_return);
	return ret;
}

static int check_login_method(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, uint8_t *flags)
{
	bool check_value = false;
	int ret = 0;

	if (dev_file == NULL || context == NULL || flags == NULL)
		return -EFAULT;
	if (context->login.method == TEEC_LOGIN_IDENTIFY) {
		tlogd("login method is IDENTIFY\n");
		/* Check if params 0 and 1 are valid */
		check_value = dev_file->kernel_api == TEE_REQ_FROM_USER_MODE &&
			(tc_user_param_valid(context, (unsigned int)0) ||
			tc_user_param_valid(context, (unsigned int)1));
		if (check_value == true)
			return -EFAULT;
		ret = set_login_information(dev_file, context);
		if (ret != 0) {
			tloge("set_login_information failed ret =%d\n", ret);
			return ret;
		}
		*flags |= TC_CALL_LOGIN;
	} else {
		tlogd("login method is not supported\n");
		return -EINVAL;
	}
	return 0;
}

static int tc_ns_service_init(const unsigned char *uuid, uint32_t uuid_len,
	struct tc_ns_service **new_service)
{
	int ret = 0;
	struct tc_ns_service *service = NULL;
	errno_t sret;
	bool check_value = false;

	check_value = (uuid == NULL || new_service == NULL ||
		uuid_len != UUID_LEN);
	if (check_value == true)
		return -ENOMEM;
	service = kzalloc(sizeof(*service), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)service)) {
		tloge("kzalloc failed\n");
		ret = -ENOMEM;
		return ret;
	}
	sret = memcpy_s(service->uuid, sizeof(service->uuid), uuid, uuid_len);
	if (sret != EOK) {
		kfree(service);
		return -ENOMEM;
	}
	INIT_LIST_HEAD(&service->session_list);
	mutex_init(&service->session_lock);
	list_add_tail(&service->head, &g_service_list);
	tlogd("add service [0x%x] to service list\n", *(uint32_t *)uuid);
	atomic_set(&service->usage, 1);
	mutex_init(&service->operation_lock);
	*new_service = service;
	return ret;
}

static struct tc_ns_service *tc_find_service_from_all(
	const unsigned char *uuid, uint32_t uuid_len)
{
	struct tc_ns_service *service = NULL;

	if (uuid == NULL || uuid_len != UUID_LEN)
		return NULL;
	list_for_each_entry(service, &g_service_list, head) {
		if (memcmp(service->uuid, uuid, sizeof(service->uuid)) == 0)
			return service;
	}
	return NULL;
}

static struct tc_ns_service *find_service(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context)
{
	int ret;
	struct tc_ns_service *service = NULL;
	bool is_full = false;

	mutex_lock(&dev_file->service_lock);
	service = tc_ref_service_in_dev(dev_file, context->uuid,
		UUID_LEN, &is_full);
	/* if service has been opened in this dev or ref cnt is full */
	if (service != NULL || is_full == true) {
		/* If service has been reference by this dev, tc_find_service_in_dev
		 * will increase a ref count to declaim there's how many callers to
		 * this service from the dev_file, instead of increase service->usage.
		 * While close session, dev->service_ref[i] will decrease and till
		 * it get to 0 put_service_struct will be called.
		 */
		mutex_unlock(&dev_file->service_lock);
		return service;
	}
	mutex_lock(&g_service_list_lock);
	service = tc_find_service_from_all(context->uuid, UUID_LEN);
	/* if service has been opened in other dev */
	if (service != NULL) {
		get_service_struct(service);
		mutex_unlock(&g_service_list_lock);
		goto add_service;
	}
	/* Create a new service if we couldn't find it in list */
	ret = tc_ns_service_init(context->uuid, UUID_LEN, &service);
	/* Put unlock after init to make sure tc_find_service_from_all is correct */
	mutex_unlock(&g_service_list_lock);
	if (ret) {
		tloge("service init failed");
		mutex_unlock(&dev_file->service_lock);
		return NULL;
	}
add_service:
	ret = add_service_to_dev(dev_file, service);
	mutex_unlock(&dev_file->service_lock);
	if (ret) {
		/*
		 * for new srvc, match init usage to 1;
		 * for srvc already exist, match get;
		 */
		put_service_struct(service);
		service = NULL;
		tloge("fail to add service to dev\n");
		return NULL;
	}
	return service;
}

static void proc_after_smc_cmd(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context, struct tc_ns_service *service,
	struct tc_ns_session *session)
{
	session->session_id = context->session_id;
	atomic_set(&session->usage, 1);
	session->owner = dev_file;

	session->wait_data.send_wait_flag = 0;
	init_waitqueue_head(&session->wait_data.send_cmd_wq);

	mutex_lock(&service->session_lock);
	list_add_tail(&session->head, &service->session_list);
	mutex_unlock(&service->session_lock);
}


static int proc_open_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_service *service,
	struct tc_ns_session *session, uint8_t flags)
{
	int ret;

	mutex_lock(&service->operation_lock);
	ret = load_ta_image(dev_file, context);
	if (ret != 0) {
		tloge("load ta image failed\n");
		mutex_unlock(&service->operation_lock);
		return ret;
	}

	ret = get_session_secure_params(dev_file, context, session);
	if (ret) {
		tloge("Get session secure parameters failed, ret = %d.\n", ret);
		/* Clean this session secure information */
		clean_session_secure_information(session);
		mutex_unlock(&service->operation_lock);
		return ret;
	}
#ifdef CONFIG_AUTH_ENHANCE
	session->teec_token.token_buffer =
		kzalloc(TOKEN_BUFFER_LEN, GFP_KERNEL);
	session->teec_token.token_len = TOKEN_BUFFER_LEN;
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)
		session->teec_token.token_buffer)) {
		tloge("kzalloc %d bytes token failed.\n", TOKEN_BUFFER_LEN);
		/* Clean this session secure information */
		clean_session_secure_information(session);
		mutex_unlock(&service->operation_lock);
		return -ENOMEM;
	}
#endif
	ret = tc_client_call(context, dev_file, session, flags);
	if (ret != 0) {
		/* Clean this session secure information */
		clean_session_secure_information(session);
		kill_ion_by_uuid((struct tc_uuid *)context->uuid);
		mutex_unlock(&service->operation_lock);
		tloge("smc_call returns error, ret=0x%x\n", ret);
		return ret;
	}
	proc_after_smc_cmd(dev_file, context, service, session);
	/* session_id in tee is unique, but in concurrency scene
	 * same session_id may appear in tzdriver, put session_list
	 * add/del in service->operation_lock can avoid it.
	 */
	mutex_unlock(&service->operation_lock);
	return ret;
}

static void proc_error_situation(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_service *service,
	struct tc_ns_session *session)
{
	release_free_session(dev_file, context, session);
	mutex_lock(&dev_file->service_lock);
	del_service_from_dev(dev_file, service);
	mutex_unlock(&dev_file->service_lock);
	kfree(session);
	return;
}

int tc_ns_open_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;
	uint8_t flags = TC_CALL_GLOBAL;
	bool check_value = (dev_file == NULL || context == NULL);

	if (check_value == true) {
		tloge("invalid dev_file or context\n");
		return -EINVAL;
	}

	ret = check_login_method(dev_file, context, &flags);
	if (ret != 0)
		return ret;
	context->cmd_id = GLOBAL_CMD_ID_OPEN_SESSION;

	service = find_service(dev_file, context);
	if (service == NULL) {
		tloge("find service failed\n");
		return -ENOMEM;
	}

	session = kzalloc(sizeof(*session), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)session)) {
		tloge("kzalloc failed\n");
		mutex_lock(&dev_file->service_lock);
		del_service_from_dev(dev_file, service);
		mutex_unlock(&dev_file->service_lock);
		return -ENOMEM;
	}
	mutex_init(&session->ta_session_lock);

	ret = calc_client_auth_hash(dev_file, context, session);
	if (ret != 0) {
		tloge("calc client auth hash failed\n");
		goto error;
	}

	ret = proc_open_session(dev_file, context, service, session, flags);
	if (ret == 0)
		return ret;
error:
	proc_error_situation(dev_file, context, service, session);
	return ret;
}

static struct tc_ns_service *get_service(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context)
{
	struct tc_ns_service *service = NULL;

	mutex_lock(&dev_file->service_lock);
	service = tc_find_service_in_dev(dev_file,
		context->uuid, UUID_LEN);
	get_service_struct(service);
	mutex_unlock(&dev_file->service_lock);
	return service;
}

static struct tc_ns_session *get_session(struct tc_ns_service *service,
	struct tc_ns_dev_file *dev_file, const struct tc_ns_client_context *context)
{
	struct tc_ns_session *session = NULL;

	mutex_lock(&service->session_lock);
	session = tc_find_session_withowner(&service->session_list,
		context->session_id, dev_file);
	get_session_struct(session);
	mutex_unlock(&service->session_lock);
	return session;
}

int tc_ns_close_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret = -EINVAL;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;

	if (dev_file == NULL || context == NULL) {
		tloge("invalid dev_file or context\n");
		return ret;
	}
	service = get_service(dev_file, context);
	if (service == NULL) {
		tloge("invalid service\n");
		return ret;
	}
	/*
	 * session_id in tee is unique, but in concurrency scene
	 * same session_id may appear in tzdriver, put session_list
	 * add/del in service->operation_lock can avoid it.
	 */
	mutex_lock(&service->operation_lock);
	session = get_session(service, dev_file, context);
	if (session != NULL) {
		int ret2;
		mutex_lock(&session->ta_session_lock);
		ret2 = close_session(dev_file, session, context->uuid,
			(unsigned int)UUID_LEN, context->session_id);
		mutex_unlock(&session->ta_session_lock);
		if (ret2 != TEEC_SUCCESS)
			tloge("close session smc failed!\n");
#ifdef CONFIG_AUTH_ENHANCE
		/* Clean this session secure information */
		if (memset_s((void *)&session->secure_info,
			sizeof(session->secure_info),
			0, sizeof(session->secure_info)) != EOK)
			tloge("close session memset error\n");
#endif
		mutex_lock(&service->session_lock);
		list_del(&session->head);
		mutex_unlock(&service->session_lock);

		put_session_struct(session);
		put_session_struct(session); /* pair with open session */

		ret = TEEC_SUCCESS;
		mutex_lock(&dev_file->service_lock);
		del_service_from_dev(dev_file, service);
		mutex_unlock(&dev_file->service_lock);
	} else {
		tloge("invalid session\n");
	}
	mutex_unlock(&service->operation_lock);
	put_service_struct(service);
	return ret;
}

int tc_ns_send_cmd(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret = -EINVAL;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;
	bool check_value = (dev_file == NULL || context == NULL);

	if (check_value) {
		tloge("invalid dev_file or context\n");
		return ret;
	}
	service = get_service(dev_file, context);
	/* check session id is validated or not */
	if (service != NULL) {
		session = get_session(service, dev_file, context);
		put_service_struct(service);
		if (session != NULL) {
			tlogd("send cmd find session id %x\n",
				context->session_id);
			goto find_session;
		}
	} else {
		tloge("can't find service\n");
	}
	return ret;
find_session:
	/* send smc */
	mutex_lock(&session->ta_session_lock);
	ret = tc_client_call(context, dev_file, session, 0);
	mutex_unlock(&session->ta_session_lock);
	put_session_struct(session);
	if (ret != 0)
		tloge("smc_call returns error, ret=0x%x\n", ret);
	return ret;
}

int tc_ns_client_open(struct tc_ns_dev_file **dev_file, uint8_t kernel_api)
{
	int ret = TEEC_ERROR_GENERIC;
	struct tc_ns_dev_file *dev = NULL;

	tlogd("tc_client_open\n");
	if (dev_file == NULL) {
		tloge("dev_file is NULL\n");
		return -EFAULT;
	}
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)dev)) {
		tloge("dev malloc failed\n");
		return ret;
	}
	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	list_add_tail(&dev->head, &g_tc_ns_dev_list.dev_file_list);
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	mutex_lock(&g_device_file_cnt_lock);
	dev->dev_file_id = g_device_file_cnt;
	g_device_file_cnt++;
	mutex_unlock(&g_device_file_cnt_lock);
	INIT_LIST_HEAD(&dev->shared_mem_list);
	dev->login_setup = 0;
	dev->kernel_api = kernel_api;
	dev->load_app_flag = 0;
	mutex_init(&dev->service_lock);
	mutex_init(&dev->shared_mem_lock);
	mutex_init(&dev->login_setup_lock);
	*dev_file = dev;
	ret = TEEC_SUCCESS;
	return ret;
}

static void del_dev_node(struct tc_ns_dev_file *dev)
{
	if (dev == NULL)
		return;
	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	/* del dev from the list */
	list_del(&dev->head);
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
}

void free_dev(struct tc_ns_dev_file *dev)
{
	del_dev_node(dev);
	tee_agent_clear_dev_owner(dev);
	if (memset_s((void *)dev, sizeof(*dev), 0, sizeof(*dev)) != EOK)
		tloge("Caution, memset dev fail!\n");
	kfree(dev);
}

int tc_ns_client_close(struct tc_ns_dev_file *dev)
{
	int ret = TEEC_ERROR_GENERIC;
	uint32_t index = 0;

	if (dev == NULL) {
		tloge("invalid dev(null)\n");
		return ret;
	}

	/* close unclosed session */
	for (index = 0; index < SERVICES_MAX_COUNT; index++)
		close_unclosed_session(dev, index);

#ifdef CONFIG_TEE_TUI
	if (dev->dev_file_id == tui_attach_device())
		free_tui_caller_info();
#endif

	kill_ion_by_cafd(dev->dev_file_id);
	// for thirdparty agent, code runs here only when agent crashed
	send_crashed_event_response_all(dev);
	ret = TEEC_SUCCESS;
	free_dev(dev);
	return ret;
}

void shared_vma_open(struct vm_area_struct *vma)
{
	(void)vma;
}

static void release_vma_shared_mem(struct tc_ns_dev_file *dev_file,
	const struct vm_area_struct *vma)
{
	struct tc_ns_shared_mem *shared_mem = NULL;
	struct tc_ns_shared_mem *shared_mem_temp = NULL;
	bool find = false;

	mutex_lock(&dev_file->shared_mem_lock);
	list_for_each_entry_safe(shared_mem, shared_mem_temp,
			&dev_file->shared_mem_list, head) {
		if (shared_mem != NULL) {
			if (shared_mem->user_addr ==
				(void *)(uintptr_t)vma->vm_start) {
				shared_mem->user_addr = NULL;
				find = true;
			} else if (shared_mem->user_addr_ca ==
				(void *)(uintptr_t)vma->vm_start) {
				shared_mem->user_addr_ca = NULL;
				find = true;
			}

			if ((shared_mem->user_addr == NULL) &&
				(shared_mem->user_addr_ca == NULL))
				list_del(&shared_mem->head);

			/* pair with tc_client_mmap */
			if (find == true) {
				put_sharemem_struct(shared_mem);
				break;
			}
		}
	}
	mutex_unlock(&dev_file->shared_mem_lock);
}

void shared_vma_close(struct vm_area_struct *vma)
{
	struct tc_ns_dev_file *dev_file = NULL;
	bool check_value = false;
	if (vma == NULL) {
		tloge("vma is null\n");
		return;
	}
	dev_file = vma->vm_private_data;
	if (dev_file == NULL) {
		tloge("vma->vm_private_data is null\n");
		return;
	}
	check_value = (g_teecd_task == current->group_leader) && (!tc_ns_get_uid()) &&
		((g_teecd_task->flags & PF_EXITING) || (current->flags & PF_EXITING));
	if (check_value == true) {
		tlogd("teecd is killed, just return in vma close\n");
		return;
	}
	release_vma_shared_mem(dev_file, vma);
}

static struct vm_operations_struct g_shared_remap_vm_ops = {
	.open = shared_vma_open,
	.close = shared_vma_close,
};

static struct tc_ns_shared_mem *alloc_and_find_shared_mem(const struct vm_area_struct *vma,
	struct tc_ns_dev_file *dev_file, bool *only_remap)
{
	struct tc_ns_shared_mem *shm_tmp = NULL;
	unsigned long len = vma->vm_end - vma->vm_start;

	/* using vma->vm_pgoff as share_mem index */
	/* check if aready allocated */
	list_for_each_entry(shm_tmp, &dev_file->shared_mem_list, head) {
		if (atomic_read(&shm_tmp->offset) == vma->vm_pgoff) {
			tlogd("share_mem already allocated, shm_tmp->offset=%d\n",
				atomic_read(&shm_tmp->offset));
			/* args check:
			 * 1. this shared mem is already mapped ?
			 * 2. remap a different size shared_mem ?
			 */
			if (shm_tmp->user_addr_ca != NULL ||
				vma->vm_end - vma->vm_start != shm_tmp->len) {
				tloge("already remap once!\n");
				return NULL;
			}
			/* return the same sharedmem specified by vma->vm_pgoff */
			*only_remap = true;
			get_sharemem_struct(shm_tmp);
			return shm_tmp;
		}
	}

	/* if not find, alloc a new sharemem */
	return tc_mem_allocate(len);
}

static int remap_shared_mem(struct vm_area_struct *vma,
	struct tc_ns_shared_mem *shared_mem)
{
	int ret;

	vma->vm_flags |= VM_USERMAP;
	ret = remap_vmalloc_range(vma, shared_mem->kernel_addr, 0);
	if (ret) {
		tloge("can't remap to user, ret = %d\n", ret);
		return -1;
	}
	return ret;
}

/* in this func, we need to deal with follow cases:
 * vendor CA alloc sharedmem (alloc and remap);
 * HIDL alloc sharedmem (alloc and remap);
 * system CA alloc sharedmem (only just remap);
 */
static int tc_client_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	struct tc_ns_dev_file *dev_file = NULL;
	struct tc_ns_shared_mem *shared_mem = NULL;
	bool only_remap = false;
	bool check_value = false;

	check_value = (filp == NULL || vma == NULL || filp->private_data == NULL);
	if (check_value == true) {
		tloge("invalid args for tc mmap\n");
		return -EINVAL;
	}
	dev_file = filp->private_data;

	mutex_lock(&dev_file->shared_mem_lock);
	shared_mem = alloc_and_find_shared_mem(vma, dev_file, &only_remap);
	if (IS_ERR_OR_NULL(shared_mem)) {
		tloge("alloc shared mem failed\n");
		mutex_unlock(&dev_file->shared_mem_lock);
		return -ENOMEM;
	}

	ret = remap_shared_mem(vma, shared_mem);
	if (ret != 0) {
		if (only_remap)
			put_sharemem_struct(shared_mem);
		else
			tc_mem_free(shared_mem);
		mutex_unlock(&dev_file->shared_mem_lock);
		return ret;
	}

	vma->vm_flags |= VM_DONTCOPY;
	vma->vm_ops = &g_shared_remap_vm_ops;
	shared_vma_open(vma);
	vma->vm_private_data = (void *)dev_file;

	if (only_remap) {
		shared_mem->user_addr_ca = (void *)(uintptr_t)vma->vm_start;
		mutex_unlock(&dev_file->shared_mem_lock);
		return ret;
	}
	shared_mem->user_addr = (void *)(uintptr_t)vma->vm_start;
	atomic_set(&shared_mem->offset, vma->vm_pgoff);
	get_sharemem_struct(shared_mem);
	list_add_tail(&shared_mem->head, &dev_file->shared_mem_list);
	mutex_unlock(&dev_file->shared_mem_lock);
	return ret;
}

static int ioctl_session_send_cmd(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, void *argp)
{
	int ret;

	ret = tc_ns_send_cmd(dev_file, context);
	if (ret)
		tloge("tc_ns_send_cmd Failed ret is %d\n", ret);
	if (copy_to_user(argp, context, sizeof(*context))) {
		if (ret == 0)
			ret = -EFAULT;
	}
	return ret;
}

static int tc_client_session_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	void *argp = (void __user *)(uintptr_t)arg;
	struct tc_ns_dev_file *dev_file = file->private_data;
	struct tc_ns_client_context context;

	if (argp == NULL) {
		tloge("argp is NULL input buffer\n");
		ret = -EINVAL;
		return ret;
	}
	if (copy_from_user(&context, argp, sizeof(context))) {
		tloge("copy from user failed\n");
		return -EFAULT;
	}
	context.returns.origin = TEEC_ORIGIN_COMMS;
	switch (cmd) {
	case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ: {
		ret = tc_ns_open_session(dev_file, &context);
		if (ret)
			tloge("tc_ns_open_session Failed ret is %d\n", ret);
		if (copy_to_user(argp, &context, sizeof(context)) && ret == 0)
			ret = -EFAULT;
		break;
	}
	case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ: {
		ret = tc_ns_close_session(dev_file, &context);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ: {
		ret = ioctl_session_send_cmd(dev_file, &context, argp);
		break;
	}
	default:
		tloge("invalid cmd:0x%x!\n", cmd);
		return ret;
	}
	/*
	 * Don't leak ERESTARTSYS to user space.
	 *
	 * CloseSession is not reentrant, so convert to -EINTR.
	 * In other case, restart_syscall().
	 *
	 * It is better to call it right after the error code
	 * is generated (in tc_client_call), but kernel CAs are
	 * still exist when these words are written. Setting TIF
	 * flags for callers of those CAs is very hard to analysis.
	 *
	 * For kernel CA, when ERESTARTSYS is seen, loop in kernel
	 * instead of notifying user.
	 *
	 * P.S. ret code in this function is in mixed naming space.
	 * See the definition of ret. However, this function never
	 * return its default value, so using -EXXX is safe.
	 */
	if (ret == -ERESTARTSYS) {
		if (cmd == TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ)
			ret = -EINTR;
		else
			return restart_syscall();
	}
	return ret;
}

static int ioctl_register_agent(struct tc_ns_dev_file *dev_file, unsigned long arg)
{
	int ret;
	struct agent_ioctl_args args;

	if (copy_from_user(&args, (void *)(uintptr_t)arg, sizeof(args))) {
		tloge("copy agent args failed\n");
		return -EFAULT;
	}

	ret = tc_ns_register_agent(dev_file, args.id, args.buffer_size,
		&args.buffer, true);
	if (ret == 0) {
		if (copy_to_user((void *)(uintptr_t)arg, &args, sizeof(args)))
			tloge("copy agent user addr failed\n");
	}

	return ret;
}

static int ioctl_unregister_agent(const struct tc_ns_dev_file *dev_file, unsigned long arg)
{
	int ret;
	struct smc_event_data *event_data = NULL;

	event_data = find_event_control((unsigned int)arg);
	if (event_data == NULL) {
		tloge("invalid agent id\n");
		return TEEC_ERROR_GENERIC;
	}
	if (event_data->owner != dev_file) {
		tloge("invalid unregister request\n");
		put_agent_event(event_data);
		return TEEC_ERROR_GENERIC;
	}
	put_agent_event(event_data);
	ret = tc_ns_unregister_agent((unsigned int)arg);
	return ret;
}

static long tc_agent_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	struct tc_ns_dev_file *dev_file = file->private_data;

	if (dev_file == NULL) {
		tloge("invalid params\n");
		return ret;
	}
	switch (cmd) {
	case TC_NS_CLIENT_IOCTL_WAIT_EVENT: {
		ret = tc_ns_wait_event((unsigned int)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE: {
		ret = tc_ns_send_event_response((unsigned int)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_REGISTER_AGENT: {
		ret = ioctl_register_agent(dev_file, arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT: {
		ret = ioctl_unregister_agent(dev_file, arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SYC_SYS_TIME: {
		ret = tc_ns_sync_sys_time((struct tc_ns_client_time *)(uintptr_t)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SET_NATIVECA_IDENTITY: {
		ret = tc_ns_set_native_hash(arg, GLOBAL_CMD_ID_SET_CA_HASH);
		break;
	}
	case TC_NS_CLIENT_IOCTL_LATEINIT: {
		ret = tc_ns_late_init(arg);
		break;
	}
	default:
		tloge("invalid cmd!");
		return ret;
	}
	tlogd("TC_NS_ClientIoctl ret = 0x%x\n", ret);
	return ret;
}

#ifdef CONFIG_TEE_TUI
static int tc_ns_tui_event(struct tc_ns_dev_file *dev_file, void *argp)
{
	int ret;
	struct teec_tui_parameter tui_param = {0};
	bool check_value = false;

	if (dev_file == NULL || argp == NULL) {
		tloge("argp or dev is NULL\n");
		return -EINVAL;
	}

	if (copy_from_user(&tui_param, argp, sizeof(struct teec_tui_parameter))) {
		tloge("copy from user failed\n");
		ret = -ENOMEM;
		return ret;
	}
	check_value = (tui_param.event_type == TUI_POLL_CANCEL ||
		tui_param.event_type == TUI_POLL_NOTCH ||
		tui_param.event_type == TUI_POLL_FOLD);
	if (check_value) {
		ret = tui_send_event(tui_param.event_type, &tui_param);
	} else {
		tloge("no permission to send event\n");
		ret = -1;
	}
	return ret;
}
#endif

static long tc_client_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	void *argp = (void __user *)(uintptr_t)arg;
	struct tc_ns_dev_file *dev_file = file->private_data;
	struct tc_ns_client_context client_context = {{0}};

	switch (cmd) {
	/* IOCTLs for the CAs */
	case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ:
		/* Upvote for peripheral zone votage, needed by Coresight.
		 * Downvote will be processed inside CFC_RETURN_PMCLK_ON_COND
		 */
	/* Fall through */
	case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ:
	case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ:
		ret = tc_client_session_ioctl(file, cmd, arg);
		break;
	case TC_NS_CLIENT_IOCTL_LOAD_APP_REQ:
		ret = tc_ns_load_secfile(dev_file, argp);
		break;
	case TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ:
		if (argp == NULL) {
			tloge("argp is NULL input buffer\n");
			ret = -EINVAL;
			break;
		}
		if (copy_from_user(&client_context, argp,
			sizeof(client_context))) {
			tloge("copy from user failed\n");
			ret = -ENOMEM;
			break;
		}
		ret = tc_ns_send_cmd(dev_file, &client_context);
		break;
	/* This is the login information
	 * and is set teecd when client opens a new session
	 */
	case TC_NS_CLIENT_IOCTL_LOGIN: {
		ret = tc_ns_client_login_func(dev_file, argp);
		break;
	}
	/* IOCTLs for the secure storage daemon */
	case TC_NS_CLIENT_IOCTL_WAIT_EVENT:
	case TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE:
	case TC_NS_CLIENT_IOCTL_REGISTER_AGENT:
	case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT:
	case TC_NS_CLIENT_IOCTL_SYC_SYS_TIME:
	case TC_NS_CLIENT_IOCTL_SET_NATIVECA_IDENTITY:
	case TC_NS_CLIENT_IOCTL_LATEINIT:
		ret = tc_agent_ioctl(file, cmd, arg);
		break;
#ifdef CONFIG_TEE_TUI
	/* for tui service inform TUI TA  event type */
	case TC_NS_CLIENT_IOCTL_TUI_EVENT: {
		ret = tc_ns_tui_event(dev_file, argp);
		break;
	}
#endif
	case TC_NS_CLIENT_IOCTL_GET_TEE_VERSION: {
		ret = tc_ns_get_tee_version(dev_file, argp);
		break;
	}

	default:
		tloge("invalid cmd 0x%x!", cmd);
		break;
	}

	tlogd("tc client ioctl ret = 0x%x\n", ret);
	return ret;
}

static int tc_client_open(struct inode *inode, struct file *file)
{
	int ret;
	struct tc_ns_dev_file *dev = NULL;

	ret = check_teecd_access();
	if (ret != EOK) {
		tloge(KERN_ERR "teecd service may be exploited 0x%x\n", ret);
		return -EPERM;
	}

	g_teecd_task = current->group_leader;
	file->private_data = NULL;
	ret = tc_ns_client_open(&dev, TEE_REQ_FROM_USER_MODE);
	if (ret == TEEC_SUCCESS)
		file->private_data = dev;
	return ret;
}

static int teec_daemon_close(struct tc_ns_dev_file *dev)
{
	if (dev == NULL) {
		tloge("invalid dev(null)\n");
		return TEEC_ERROR_GENERIC;
	}
	del_dev_node(dev);
	kfree(dev);
	return TEEC_SUCCESS;
}

static int tc_client_close(struct inode *inode, struct file *file)
{
	int ret = TEEC_ERROR_GENERIC;
	struct tc_ns_dev_file *dev = file->private_data;
	bool check_value = false;
#ifdef CONFIG_TEE_TUI
	/* release tui resource */
	struct teec_tui_parameter tui_param = {0};
	if (dev->dev_file_id == tui_attach_device())
		tui_send_event(TUI_POLL_CANCEL, &tui_param);
#endif
	check_value = (g_teecd_task == current->group_leader) &&
		(!tc_ns_get_uid());
	if (check_value) {
		/* for teecd close fd */
		check_value = (g_teecd_task->flags & PF_EXITING) ||
			(current->flags & PF_EXITING);
		if (check_value == true) {
			/* when teecd is be killed or crash */
			tloge("teecd is killed, something bad must be happened!!!\n");
			if (is_system_agent(dev)) {
				/* for teecd agent close fd */
				send_event_response_single(dev);
				free_dev(dev);
			} else {
				/* for ca damon close fd */
				ret = teec_daemon_close(dev);
			}
		} else {
			/* for ca damon close fd when ca damon close fd
			 *  later than HIDL thread
			 */
			ret = tc_ns_client_close(dev);
		}
	} else {
		/* for CA(HIDL thread) close fd */
		ret = tc_ns_client_close(dev);
	}
	file->private_data = NULL;
	return ret;
}

void dump_services_status(const char *param)
{
	struct tc_ns_service *service = NULL;

	(void)param;
	mutex_lock(&g_service_list_lock);
	tlogi("show service list:\n");
	list_for_each_entry(service, &g_service_list, head) {
		tlogi("uuid-%x, usage=%d\n", *(uint32_t *)service->uuid,
			atomic_read(&service->usage));
	}
	mutex_unlock(&g_service_list_lock);
}

static struct tc_ns_dev_file *tc_find_dev_file(unsigned int dev_file_id)
{
	struct tc_ns_dev_file *dev_file = NULL;

	list_for_each_entry(dev_file, &g_tc_ns_dev_list.dev_file_list, head) {
		if (dev_file->dev_file_id == dev_file_id)
			return dev_file;
	}
	return NULL;
}

struct tc_ns_session *tc_find_session_by_uuid(unsigned int dev_file_id,
	const struct tc_ns_smc_cmd *cmd)
{
	struct tc_ns_dev_file *dev_file = NULL;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;

	if (cmd == NULL) {
		tloge("Parameter is null pointer!\n");
		return NULL;
	}
	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	dev_file = tc_find_dev_file(dev_file_id);
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	if (dev_file == NULL) {
		tloge("Can't find dev file!\n");
		return NULL;
	}
	mutex_lock(&dev_file->service_lock);
	service = tc_find_service_in_dev(dev_file, cmd->uuid, UUID_LEN);
	get_service_struct(service);
	mutex_unlock(&dev_file->service_lock);
	if (service == NULL) {
		tloge(" Can't find service!\n");
		return NULL;
	}
	mutex_lock(&service->session_lock);
	session = tc_find_session_withowner(&service->session_list,
		cmd->context_id, dev_file);
	get_session_struct(session);
	mutex_unlock(&service->session_lock);
	put_service_struct(service);
	if (session == NULL) {
		tloge("can't find session[0x%x]!\n", cmd->context_id);
		return NULL;
	}
	return session;
}

#ifdef CONFIG_COMPAT
long tc_compat_client_ioctl(struct file *flie, unsigned int cmd,
	unsigned long arg)
{
	long ret;

	if (flie == NULL)
		return -EINVAL;
	arg = (unsigned long)(uintptr_t)compat_ptr(arg);
	ret = tc_client_ioctl(flie, cmd, arg);
	return ret;
}
#endif

static const struct file_operations g_tc_ns_client_fops = {
	.owner = THIS_MODULE,
	.open = tc_client_open,
	.release = tc_client_close,
	.unlocked_ioctl = tc_client_ioctl,
	.mmap = tc_client_mmap,
#ifdef CONFIG_COMPAT
	.compat_ioctl = tc_compat_client_ioctl,
#endif
};


static int tzdriver_probe(struct platform_device *pdev)
{
	return 0;
}

struct of_device_id g_tzdriver_platform_match[] = {
	{ .compatible = "trusted_core" },
	{},
};

MODULE_DEVICE_TABLE(of, g_tzdriver_platform_match);


const struct dev_pm_ops g_tzdriver_pm_ops = {
	.freeze = tc_s4_pm_suspend,
	.restore = tc_s4_pm_resume,
};

static struct platform_driver g_tz_platform_driver = {
	.driver = {
		.name             = "trusted_core",
		.owner            = THIS_MODULE,
		.of_match_table = of_match_ptr(g_tzdriver_platform_match),
		.pm = &g_tzdriver_pm_ops,
	},
	.probe = tzdriver_probe,
};

static int tc_ns_client_init(struct device **class_dev)
{
	int ret;

	tlogd("tc_ns_client_init");
	g_np = of_find_compatible_node(NULL, NULL, "trusted_core");

	if (g_np == NULL) {
		tloge("No trusted_core compatible node found.\n");
		return -ENODEV;
	}

	ret = platform_driver_register(&g_tz_platform_driver);
	if (ret) {
		tloge("platform register driver failed\n");
		return -EFAULT;
	}

	ret = alloc_chrdev_region(&g_tc_ns_client_devt, 0, 1, TC_NS_CLIENT_DEV);
	if (ret < 0) {
		tloge("alloc chrdev region failed %d", ret);
		return -EFAULT;
	}
	g_driver_class = class_create(THIS_MODULE, TC_NS_CLIENT_DEV);
	if (IS_ERR_OR_NULL(g_driver_class)) {
		ret = -ENOMEM;
		tloge("class create failed %d", ret);
		goto chrdev_region_unregister;
	}
	*class_dev = device_create(g_driver_class, NULL, g_tc_ns_client_devt,
		NULL, TC_NS_CLIENT_DEV);
	if (IS_ERR_OR_NULL(*class_dev)) {
		tloge("class device create failed");
		ret = -ENOMEM;
		goto destroy_class;
	}

	(*class_dev)->of_node = g_np;
	cdev_init(&g_tc_ns_client_cdev, &g_tc_ns_client_fops);
	g_tc_ns_client_cdev.owner = THIS_MODULE;
	ret = cdev_add(&g_tc_ns_client_cdev,
		MKDEV(MAJOR(g_tc_ns_client_devt), 0), 1);
	if (ret < 0) {
		tloge("cdev_add failed %d", ret);
		goto class_device_destroy;
	}
	ret = memset_s(&g_tc_ns_dev_list, sizeof(g_tc_ns_dev_list), 0,
		sizeof(g_tc_ns_dev_list));
	if (ret != EOK)
		goto class_device_destroy;
	INIT_LIST_HEAD(&g_tc_ns_dev_list.dev_file_list);
	mutex_init(&g_tc_ns_dev_list.dev_lock);
	init_crypto_hash_lock();
	INIT_LIST_HEAD(&g_service_list);
	return ret;

class_device_destroy:
	device_destroy(g_driver_class, g_tc_ns_client_devt);
destroy_class:
	class_destroy(g_driver_class);
chrdev_region_unregister:
	unregister_chrdev_region(g_tc_ns_client_devt, 1);
	return ret;
}

static int tc_teeos_init(struct device *class_dev)
{
	int ret;

	ret = smc_init_data(class_dev);
	if (ret)
		return ret;

	ret = mailbox_mempool_init();
	if (ret) {
		tloge("tz mailbox init failed\n");
		goto smc_data_free;
	}

	ret = tz_spi_init(class_dev, g_np);
	if (ret) {
		tloge("tz spi init failed\n");
		goto release_mailbox;
	}
	return 0;
release_mailbox:
	mailbox_mempool_destroy();
smc_data_free:
	smc_free_data();
	return ret;
}

static void tc_re_init(struct device *class_dev)
{
	int ret = 0;

	// following failure don't block tzdriver init proc;
	agent_init();
	ret = tc_ns_register_ion_mem();
	if (ret)
		tloge("Failed to register ion mem in tee\n");

	ret = init_tui(class_dev);
	if (ret)
		tloge("init_tui failed 0x%x\n", ret);

	if (init_smc_svc_thread()) {
		tloge("init svc thread\n");
		ret = -EFAULT;
	}

	if (init_dynamic_mem()) {
		tloge("init dynamic mem Failed\n");
		ret = -EFAULT;
	}

	if (ret)
		tloge("Caution! Running environment init failed!\n");
}

static __init int tc_init(void)
{
	struct device *class_dev = NULL;
	int ret = 0;

	ret = tc_ns_client_init(&class_dev);
	if (ret != 0)
		return ret;

	ret = tc_teeos_init(class_dev);
	if (ret != 0)
		goto class_device_destroy;
	// run-time environment init failure don't block tzdriver init proc;
	tc_re_init(class_dev);
	return 0;

class_device_destroy:
	device_destroy(g_driver_class, g_tc_ns_client_devt);
	class_destroy(g_driver_class);
	unregister_chrdev_region(g_tc_ns_client_devt, 1);

	return ret;
}

static void tc_exit(void)
{
	tlogd("tz client exit");
	tui_exit();
	tz_spi_exit();
	/* run-time environment exit should before teeos exit */
	device_destroy(g_driver_class, g_tc_ns_client_devt);
	class_destroy(g_driver_class);
	unregister_chrdev_region(g_tc_ns_client_devt, 1);
	smc_free_data();
	agent_exit();
	mailbox_mempool_destroy();
	tee_exit_shash_handle();
}

MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("TrustCore ns-client driver");
MODULE_VERSION("1.10");

fs_initcall_sync(tc_init);
module_exit(tc_exit);
MODULE_LICENSE("GPL");
