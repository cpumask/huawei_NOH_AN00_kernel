/*
 * gp_op.c
 *
 * alloc global operation and pass params to TEE.
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
#include "gp_ops.h"
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/types.h>
#include <asm/memory.h>
#include <securec.h>

#if ((defined CONFIG_ION_HISI) || (defined CONFIG_ION_HISI_SECSG))
#include <linux/hisi/hisi_ion.h>
#endif

#include "teek_client_constants.h"
#include "tc_ns_client.h"
#include "agent.h"
#include "tc_ns_log.h"
#include "smc_smp.h"
#include "mem.h"
#include "mailbox_mempool.h"
#include "tc_client_sub_driver.h"
#include "tc_client_driver.h"
#include "dynamic_ion_mem.h"
#include "security_auth_enhance.h"
#include "tlogger.h"

struct alloc_params {
	struct tc_ns_dev_file *dev_file;
	struct tc_ns_client_context *client_context;
	struct tc_ns_session *session;
	struct tc_ns_operation *operation;
	struct tc_ns_temp_buf *local_temp_buffer;
	unsigned int tmp_buf_size;
	unsigned int *trans_paramtype_to_tee;
	unsigned int trans_paramtype_size;
};

struct update_params {
	struct tc_ns_dev_file *dev_file;
	struct tc_ns_client_context *client_context;
	struct tc_ns_operation *operation;
	struct tc_ns_temp_buf *local_temp_buffer;
	unsigned int tmp_buf_size;
	bool is_complete;
};

#define MAX_SHARED_SIZE 0x100000      /* 1 MiB */

static void free_operation(struct tc_call_params *params, struct tc_ns_operation *operation);

/* dir: 0-inclue input, 1-include output, 2-both */
static inline bool teec_value_type(unsigned int type, int dir)
{
	return (((dir == 0 || dir == 2) && type == TEEC_VALUE_INPUT) ||
		((dir == 1 || dir == 2) && type == TEEC_VALUE_OUTPUT) ||
		type == TEEC_VALUE_INOUT) ? true : false;
}

static inline bool teec_tmpmem_type(unsigned int type, int dir)
{
	return (((dir == 0 || dir == 2) && type == TEEC_MEMREF_TEMP_INPUT) ||
		((dir == 1 || dir == 2) && type == TEEC_MEMREF_TEMP_OUTPUT) ||
		type == TEEC_MEMREF_TEMP_INOUT) ? true : false;
}

static inline bool teec_memref_type(unsigned int type, int dir)
{
	return (((dir == 0 || dir == 2) && type == TEEC_MEMREF_PARTIAL_INPUT) ||
		((dir == 1 || dir == 2) && type == TEEC_MEMREF_PARTIAL_OUTPUT) ||
		type == TEEC_MEMREF_PARTIAL_INOUT) ? true : false;
}

static int check_user_param(const struct tc_ns_client_context *client_context,
	unsigned int index)
{
	if (client_context == NULL) {
		tloge("client_context is null.\n");
		return -EINVAL;
	}
	if (index >= PARAM_NUM) {
		tloge("index is invalid, index:%x.\n", index);
		return -EINVAL;
	}
	return 0;
}

static bool is_mem_param(unsigned int param_type)
{
	if (param_type == TEEC_MEMREF_TEMP_INPUT ||
		param_type == TEEC_MEMREF_TEMP_OUTPUT ||
		param_type == TEEC_MEMREF_TEMP_INOUT ||
		param_type == TEEC_MEMREF_PARTIAL_INPUT ||
		param_type == TEEC_MEMREF_PARTIAL_OUTPUT ||
		param_type == TEEC_MEMREF_PARTIAL_INOUT)
		return true;

	return false;
}

static bool is_val_param(unsigned int param_type)
{
	if (param_type == TEEC_VALUE_INPUT ||
		param_type == TEEC_VALUE_OUTPUT ||
		param_type == TEEC_VALUE_INOUT ||
		param_type == TEEC_ION_INPUT ||
		param_type == TEEC_ION_SGLIST_INPUT)
		return true;

	return false;
}

/* Check the size and buffer addresses  have valid userspace addresses */
static bool is_usr_refmem_valid(union tc_ns_client_param *client_param)
{
	uint32_t size;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
	if (!access_ok(VERIFY_READ,
		(void *)(uintptr_t)client_param->memref.size_addr,
		sizeof(uint32_t)))
#else
	if (!access_ok(
		(void *)(uintptr_t)client_param->memref.size_addr,
		sizeof(uint32_t)))
#endif
		return false;

	get_user(size, (uint32_t *)(uintptr_t)client_param->memref.size_addr);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
	if (!access_ok(VERIFY_READ,
		(void *)(uintptr_t)client_param->memref.buffer, size))
#else
	if (!access_ok((void *)(uintptr_t)client_param->memref.buffer, size))
#endif
		return false;

	return true;
}

static bool is_usr_valmem_valid(union tc_ns_client_param *client_param)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(4, 19, 0))
	if (!access_ok(VERIFY_READ,
		(void *)(uintptr_t)client_param->value.a_addr,
		sizeof(uint32_t)))
#else
	if (!access_ok(
		(void *)(uintptr_t)client_param->value.a_addr,
		sizeof(uint32_t)))
#endif
		return false;

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(4, 19, 0))
	if (!access_ok(VERIFY_READ,
		(void *)(uintptr_t)client_param->value.b_addr,
		sizeof(uint32_t)))
#else
	if (!access_ok(
		(void *)(uintptr_t)client_param->value.b_addr,
		sizeof(uint32_t)))
#endif
		return false;

	return true;
}

int tc_user_param_valid(struct tc_ns_client_context *client_context,
	unsigned int index)
{
	union tc_ns_client_param *client_param = NULL;
	unsigned int param_type;

	if (check_user_param(client_context, index) != 0)
		return -EINVAL;

	client_param = &(client_context->params[index]);
	param_type = teec_param_type_get(client_context->param_types, index);
	tlogd("Param %u type is %x\n", index, param_type);
	if (param_type == TEEC_NONE) {
		tlogd("param_type is TEEC_NONE.\n");
		return 0;
	}

	/* temp buffers we need to allocate/deallocate for every operation */
	if (is_mem_param(param_type)) {
		if (!is_usr_refmem_valid(client_param))
			return -EFAULT;
	} else if (is_val_param(param_type)) {
		if (!is_usr_valmem_valid(client_param))
			return -EFAULT;
	} else {
		tloge("param_types is not supported\n");
		return -EFAULT;
	}
	return 0;
}

/*
 * These function handle read from client. Because client here can be
 * kernel client or user space client, we must use the proper function
 */
int read_from_client(void *dest, size_t dest_size,
	const void __user *src, size_t size, uint8_t kernel_api)
{
	int ret;
	bool check_value = false;

	check_value = (dest == NULL || src == NULL);
	if (check_value) {
		tloge("src or dest is NULL input buffer\n");
		return -EINVAL;
	}
	/*  to be sure that size is <= dest's buffer size. */
	if (size > dest_size) {
		tloge("size is larger than dest_size or size is 0\n");
		return -EINVAL;
	}
	if (size == 0)
		return 0;

	if (kernel_api) {
		check_value = virt_addr_valid((unsigned long)(uintptr_t)src) ||
			vmalloc_addr_valid(src) || modules_addr_valid(src);
		if (!check_value) {
			tloge("invalid addr\n");
			return -EFAULT;
		}
		ret = memcpy_s(dest, dest_size, src, size);
		if (ret != EOK) {
			tloge("memcpy fail. line=%d, s_ret=%d\n",
				__LINE__, ret);
			return ret;
		}
		return ret;
	}
	/* buffer is in user space(CA call TEE API) */
	if (copy_from_user(dest, src, size)) {
		tloge("copy from user failed\n");
		return -EFAULT;
	}

	return 0;
}

int write_to_client(void __user *dest, size_t dest_size,
	const void *src, size_t size, uint8_t kernel_api)
{
	int ret;
	bool check_value = false;

	check_value = (dest == NULL) || (src == NULL);
	if (check_value == true) {
		tloge("src or dest is NULL input buffer\n");
		return -EINVAL;
	}
	/*  to be sure that size is <= dest's buffer size. */
	if (size > dest_size) {
		tloge("size is larger than dest_size\n");
		return -EINVAL;
	}
	if (size == 0)
		return 0;

	if (kernel_api) {
		check_value = virt_addr_valid((unsigned long)(uintptr_t)dest) ||
			vmalloc_addr_valid(dest) || modules_addr_valid(dest);
		if (!check_value) {
			tloge("invalid addr\n");
			return -EFAULT;
		}
		ret = memcpy_s(dest, dest_size, src, size);
		if (ret != EOK) {
			tloge("write to client fail. line=%d, ret=%d\n",
			      __LINE__, ret);
			return ret;
		}
		return ret;
	}
	/* buffer is in user space(CA call TEE API) */
	if (copy_to_user(dest, src, size)) {
		tloge("copy to user failed\n");
		return -EFAULT;
	}
	return 0;
}

static int check_params_for_alloc(const struct tc_call_params *params,
	const struct tc_ns_operation *operation)
{
	if (params->dev_file == NULL) {
		tloge("dev_file is null");
		return -EINVAL;
	}
	if (params->session == NULL) {
		tloge("session is null\n");
		return -EINVAL;
	}
	if (operation == NULL) {
		tloge("operation is null\n");
		return -EINVAL;
	}
	if (params->local_temp_buffer == NULL) {
		tloge("local_temp_buffer is null");
		return -EINVAL;
	}
	if (params->tmp_buf_size != (unsigned int)TEE_PARAM_NUM) {
		tloge("tmp_buf_size is wrong");
		return -EINVAL;
	}
	return 0;
}

static int check_context_for_alloc(const struct tc_ns_client_context *client_context)
{
	if (client_context == NULL) {
		tloge("client_context is null");
		return -EINVAL;
	}
	if (client_context->param_types == 0) {
		tloge("invalid param type\n");
		return -EINVAL;
	}
	return 0;
}

static void set_kernel_params_for_open_session(uint8_t flags,
	int index, uint8_t *kernel_params)
{
	/*
	 * Normally kernel_params = kernel_api
	 * But when TC_CALL_LOGIN, params 2/3 will
	 * be filled by kernel. so under this circumstance,
	 * params 2/3 has to be set to kernel mode; and
	 * param 0/1 will keep the same with kernel_api.
	 */
	bool check_value = (flags & TC_CALL_LOGIN) && (index >= 2);
	if (check_value)
		*kernel_params = TEE_REQ_FROM_KERNEL_MODE;
	return;
}

static int check_size_for_alloc(const struct alloc_params *params_in, unsigned int index)
{
	bool check_value = false;

	check_value = (params_in->trans_paramtype_size != TEE_PARAM_NUM ||
		params_in->tmp_buf_size != TEE_PARAM_NUM ||
		index >= TEE_PARAM_NUM);
	if (check_value) {
		tloge("buf size or params type or index is invalid.\n");
		return -EFAULT;
	}
	return 0;
}

static int alloc_for_tmp_mem(struct alloc_params *params_in,
	uint8_t kernel_params, unsigned int param_type, uint8_t flags,
	unsigned int index)
{
	struct tc_ns_client_context *client_context = params_in->client_context;
	struct tc_ns_temp_buf *local_temp_buffer = params_in->local_temp_buffer;
	struct tc_ns_operation *operation = params_in->operation;
	unsigned int *trans_paramtype_to_tee = params_in->trans_paramtype_to_tee;
	union tc_ns_client_param *client_param = NULL;
	void *temp_buf = NULL;
	uint32_t buffer_size = 0;
	bool check_value = false;
	int ret = 0;

	if (check_size_for_alloc(params_in, index) != 0)
		return -EFAULT;
	/* For interface compatibility sake we assume buffer size to be 32bits */
	client_param = &(client_context->params[index]);
	if (read_from_client(&buffer_size, sizeof(buffer_size),
		(uint32_t __user *)(uintptr_t)client_param->memref.size_addr,
		sizeof(uint32_t), kernel_params)) {
		tloge("copy memref.size_addr failed\n");
		return -EFAULT;
	}
	/* Don't allow unbounded malloc requests */
	if (buffer_size > MAX_SHARED_SIZE) {
		tloge("buffer_size %u from user is too large\n", buffer_size);
		return -EFAULT;
	}
	temp_buf = (void *)mailbox_alloc(buffer_size, MB_FLAG_ZERO);
	/* If buffer size is zero or malloc failed */
	if (temp_buf == NULL) {
		tloge("temp_buf malloc failed, i = %u.\n", index);
		return -ENOMEM;
	} else {
		tlogd("temp_buf malloc ok, i = %u.\n", index);
	}
	local_temp_buffer[index].temp_buffer = temp_buf;
	local_temp_buffer[index].size = buffer_size;
	check_value = (param_type == TEEC_MEMREF_TEMP_INPUT) ||
		(param_type == TEEC_MEMREF_TEMP_INOUT);
	if (check_value) {
		tlogv("client_param->memref.buffer=0x%llx\n",
			client_param->memref.buffer);
		/* Kernel side buffer */
		if (read_from_client(temp_buf, buffer_size,
			(void *)(uintptr_t)client_param->memref.buffer,
			buffer_size, kernel_params)) {
			tloge("copy memref.buffer failed\n");
			return -EFAULT;
		}
	}
#ifdef CONFIG_AUTH_ENHANCE
	if (is_opensession_by_index(flags, client_context->cmd_id, index)) {
		ret = encrypt_login_info(buffer_size,
			temp_buf, params_in->session->secure_info.crypto_info.key);
		if (ret != 0) {
			tloge("CONFIG_AUTH_ENHANCE:encry failed\n");
			return ret;
		}
	}
#endif
	operation->params[index].memref.buffer = virt_to_phys((void *)temp_buf);
	operation->buffer_h_addr[index] =
		virt_to_phys((void *)temp_buf) >> ADDR_TRANS_NUM;
	operation->params[index].memref.size = buffer_size;
	/* TEEC_MEMREF_TEMP_INPUT equal to TEE_PARAM_TYPE_MEMREF_INPUT */
	trans_paramtype_to_tee[index] = param_type;
	return ret;
}

static int check_buffer_for_ref(uint32_t *buffer_size,
	const union tc_ns_client_param *client_param, uint8_t kernel_params)
{
	if (read_from_client(buffer_size, sizeof(*buffer_size),
		(uint32_t __user *)(uintptr_t)client_param->memref.size_addr,
		sizeof(uint32_t), kernel_params)) {
		tloge("copy memref.size_addr failed\n");
		return -EFAULT;
	}
	if (*buffer_size == 0) {
		tloge("buffer_size from user is 0\n");
		return -ENOMEM;
	}
	return 0;
}
static int alloc_for_ref_mem(struct alloc_params *params_in,
	uint8_t kernel_params, unsigned int param_type, int index)
{
	struct tc_ns_client_context *client_context = params_in->client_context;
	struct tc_ns_operation *operation = params_in->operation;
	unsigned int *trans_paramtype_to_tee = params_in->trans_paramtype_to_tee;
	struct tc_ns_dev_file *dev_file = params_in->dev_file;
	union tc_ns_client_param *client_param = NULL;
	struct tc_ns_shared_mem *shared_mem = NULL;
	uint32_t buffer_size = 0;
	bool check_value = false;
	int ret = 0;

	if (check_size_for_alloc(params_in, index) != 0)
		return -EFAULT;
	client_param = &(client_context->params[index]);
	ret = check_buffer_for_ref(&buffer_size, client_param, kernel_params);
	if (ret != 0)
		return ret;
	operation->params[index].memref.buffer = 0;
	/* find kernel addr refered to user addr */
	mutex_lock(&dev_file->shared_mem_lock);
	list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head) {
		if (shared_mem->user_addr ==
			(void *)(uintptr_t)client_param->memref.buffer) {
			/* arbitrary CA can control offset by ioctl, so in here
			 * offset must be checked, and avoid integer overflow.
			 */
			check_value = ((shared_mem->len -
				client_param->memref.offset) >= buffer_size) &&
				(shared_mem->len > client_param->memref.offset);
			if (check_value == true) {
				void *buffer_addr =
					(void *)(uintptr_t)(
					(uintptr_t)shared_mem->kernel_addr +
					client_param->memref.offset);
				buffer_addr =
					mailbox_copy_alloc(buffer_addr,
						buffer_size);
				if (buffer_addr == NULL) {
					tloge("alloc mailbox copy failed\n");
					ret = -ENOMEM;
					break;
				}
				operation->mb_buffer[index] = buffer_addr;
				operation->params[index].memref.buffer =
					virt_to_phys(buffer_addr);
				operation->buffer_h_addr[index] =
					virt_to_phys(buffer_addr) >> ADDR_TRANS_NUM;
				/* save shared_mem in operation
				 * so that we can use it while free_operation
				 */
				operation->sharemem[index] = shared_mem;
				get_sharemem_struct(shared_mem);
			} else {
				tloge("Unexpected size %u vs %u",
					shared_mem->len, buffer_size);
			}
			break;
		}
	}
	mutex_unlock(&dev_file->shared_mem_lock);
	/* for 8G physical memory device, there is a chance that
	 * operation->params[i].memref.buffer could be all 0,
	 * buffer_h_addr cannot be 0 in the same time.
	 */
	check_value = (!operation->params[index].memref.buffer) &&
		(!operation->buffer_h_addr[index]);
	if (check_value == true) {
		tloge("can not find shared buffer, exit\n");
		return -EINVAL;
	}
	operation->params[index].memref.size = buffer_size;
	/* Change TEEC_MEMREF_PARTIAL_XXXXX  to TEE_PARAM_TYPE_MEMREF_XXXXX */
	trans_paramtype_to_tee[index] = param_type -
		(TEEC_MEMREF_PARTIAL_INPUT - TEE_PARAM_TYPE_MEMREF_INPUT);
	return ret;
}

static int copy_for_value(struct alloc_params *params_in, uint8_t kernel_params,
	unsigned int param_type, int index)
{
	struct tc_ns_operation *operation = params_in->operation;
	unsigned int *trans_paramtype_to_tee = params_in->trans_paramtype_to_tee;
	struct tc_ns_client_context *client_context = params_in->client_context;
	int ret = 0;
	union tc_ns_client_param *client_param = NULL;

	if (check_size_for_alloc(params_in, index) != 0)
		return -EFAULT;
	client_param = &(client_context->params[index]);
	if (read_from_client(&operation->params[index].value.a,
		sizeof(operation->params[index].value.a),
		(void *)(uintptr_t)client_param->value.a_addr,
		sizeof(operation->params[index].value.a),
		kernel_params)) {
		tloge("copy value.a_addr failed\n");
		return -EFAULT;
	}
	if (read_from_client(&operation->params[index].value.b,
		sizeof(operation->params[index].value.b),
		(void *)(uintptr_t)client_param->value.b_addr,
		sizeof(operation->params[index].value.b),
		kernel_params)) {
		tloge("copy value.b_addr failed\n");
		return -EFAULT;
	}
	/* TEEC_VALUE_INPUT equal
	 * to TEE_PARAM_TYPE_VALUE_INPUT
	 */
	trans_paramtype_to_tee[index] = param_type;
	return ret;
}

#ifdef CONFIG_ION_HISI_SECSG
static int get_ion_sg_list_from_fd(uint32_t ion_shared_fd,
	uint32_t ion_alloc_size, phys_addr_t *sglist_table,
	size_t *ion_sglist_size)
{
	struct sg_table *ion_table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct sglist *tmp_sglist = NULL;
	uint64_t ion_id = 0;
	enum SEC_SVC ion_type = 0;
	uint32_t ion_list_num = 0;
	uint32_t sglist_size;
	uint32_t i;

	int ret = secmem_get_buffer(ion_shared_fd, &ion_table, &ion_id, &ion_type);
	if (ret != 0) {
		tloge("get ion table failed. \n");
		return -EFAULT;
	}

	if (ion_type != SEC_DRM_TEE) {
		bool stat = (ion_table->nents <= 0 || ion_table->nents > MAX_ION_NENTS);
		if (stat)
			return -EFAULT;
		ion_list_num = (uint32_t)(ion_table->nents & INT_MAX);
		for_each_sg(ion_table->sgl, sg, ion_list_num, i) {
			if (sg == NULL) {
				tloge("an error sg when get ion sglist \n");
				return -EFAULT;
			}
		}
	}
	// ion_list_num is less than 1024, so sglist_size won't flow
	sglist_size = sizeof(struct ion_page_info) * ion_list_num +
		sizeof(*tmp_sglist);
	tmp_sglist = (struct sglist *)mailbox_alloc(sglist_size, MB_FLAG_ZERO);
	if (tmp_sglist == NULL) {
		tloge("sglist mem alloc failed.\n");
		return -ENOMEM;
	}
	tmp_sglist->sglist_size = (uint64_t)sglist_size;
	tmp_sglist->ion_size = (uint64_t)ion_alloc_size;
	tmp_sglist->info_length = (uint64_t)ion_list_num;
	if (ion_type != SEC_DRM_TEE) {
		for_each_sg(ion_table->sgl, sg, ion_list_num, i) {
			page = sg_page(sg);
			tmp_sglist->page_info[i].phys_addr = page_to_phys(page);
			tmp_sglist->page_info[i].npages = sg->length / PAGE_SIZE;
		}
	} else
		tmp_sglist->ion_id = ion_id;

	*sglist_table = virt_to_phys((void *)tmp_sglist);
	*ion_sglist_size = sglist_size;
	return 0;
}

static int alloc_for_ion_sglist(struct alloc_params *params_in,
	uint8_t kernel_params, unsigned int param_type, int index)
{
	struct tc_ns_operation *operation = params_in->operation;
	struct tc_ns_temp_buf *local_temp_buffer = params_in->local_temp_buffer;
	unsigned int *param_type_to_tee = params_in->trans_paramtype_to_tee;
	struct tc_ns_client_context *client_context = params_in->client_context;
	size_t ion_sglist_size = 0;
	phys_addr_t ion_sglist_addr = 0x0;
	int ret = 0;
	union tc_ns_client_param *client_param = NULL;
	unsigned int ion_shared_fd = 0;
	unsigned int ion_alloc_size = 0;

	if (check_size_for_alloc(params_in, index) != 0)
		return -EFAULT;
	client_param = &(client_context->params[index]);
	if (read_from_client(&operation->params[index].value.a,
		sizeof(operation->params[index].value.a),
		(void *)(uintptr_t)client_param->value.a_addr,
		sizeof(operation->params[index].value.a), kernel_params)) {
		tloge("value.a_addr copy failed\n");
		return -EFAULT;
	}
	if (read_from_client(&operation->params[index].value.b,
		sizeof(operation->params[index].value.b),
		(void *)(uintptr_t)client_param->value.b_addr,
		sizeof(operation->params[index].value.b), kernel_params)) {
		tloge("value.b_addr copy failed\n");
		return -EFAULT;
	}
	if ((int)operation->params[index].value.a < 0) {
		tloge("drm ion handle invaild!\n");
		return -EFAULT;
	}
	ion_shared_fd = operation->params[index].value.a;
	ion_alloc_size = operation->params[index].value.b;

	ret = get_ion_sg_list_from_fd(ion_shared_fd, ion_alloc_size,
		&ion_sglist_addr, &ion_sglist_size);
	if (ret < 0) {
		tloge("get ion sglist failed, fd=%u\n", ion_shared_fd);
		return -EFAULT;
	}
	local_temp_buffer[index].temp_buffer = phys_to_virt(ion_sglist_addr);
	local_temp_buffer[index].size = ion_sglist_size;

	operation->params[index].memref.buffer = (unsigned int)ion_sglist_addr;
	operation->buffer_h_addr[index] =
		(unsigned int)(ion_sglist_addr >> ADDR_TRANS_NUM);
	operation->params[index].memref.size = (unsigned int)ion_sglist_size;
	param_type_to_tee[index] = param_type;
	return ret;
}
#else
static int alloc_for_ion_sglist(struct alloc_params *params_in,
	uint8_t kernel_params, unsigned int param_type, int index)
{
	tloge("not support hisi seg and releated feature!\n");
	return -1;
}
#endif

#ifdef CONFIG_ION_HISI
static int alloc_for_ion(struct alloc_params *params_in, uint8_t kernel_params,
	unsigned int param_type, int index)
{
	struct tc_ns_operation *operation = params_in->operation;
	unsigned int *trans_paramtype_to_tee = params_in->trans_paramtype_to_tee;
	struct tc_ns_client_context *client_context = params_in->client_context;
	size_t drm_ion_size = 0;
	phys_addr_t drm_ion_phys = 0x0;
	struct dma_buf *drm_dma_buf = NULL;
	int ret = 0;
	union tc_ns_client_param *client_param = NULL;
	unsigned int ion_shared_fd = 0;

	if (check_size_for_alloc(params_in, index) != 0)
		return -EFAULT;
	client_param = &(client_context->params[index]);
	if (read_from_client(&operation->params[index].value.a,
		sizeof(operation->params[index].value.a),
		(void *)(uintptr_t)client_param->value.a_addr,
		sizeof(operation->params[index].value.a), kernel_params)) {
		tloge("value.a_addr copy failed\n");
		return -EFAULT;
	}
	if (read_from_client(&operation->params[index].value.b,
		sizeof(operation->params[index].value.b),
		(void *)(uintptr_t)client_param->value.b_addr,
		sizeof(operation->params[index].value.b), kernel_params)) {
		tloge("value.b_addr copy failed\n");
		return -EFAULT;
	}
	if ((int)operation->params[index].value.a >= 0) {
		ion_shared_fd = operation->params[index].value.a;
		drm_dma_buf = dma_buf_get(ion_shared_fd);
		if (IS_ERR_OR_NULL(drm_dma_buf)) {
			tloge("in %s err:drm_dma_buf is ERR, ret=%d fd=%u\n",
				__func__, ret, ion_shared_fd);
			return -EFAULT;
		}

		ret = ion_secmem_get_phys(drm_dma_buf, &drm_ion_phys,
			&drm_ion_size);
		if (ret) {
			tloge("in %s err:ret=%d fd=%u\n", __func__,
				ret, ion_shared_fd);
			dma_buf_put(drm_dma_buf);
			return -EFAULT;
		}

		if (drm_ion_size > operation->params[index].value.b)
			drm_ion_size = operation->params[index].value.b;
		operation->params[index].memref.buffer = (unsigned int)drm_ion_phys;
		operation->params[index].memref.size = (unsigned int)drm_ion_size;
		trans_paramtype_to_tee[index] = param_type;
		dma_buf_put(drm_dma_buf);
	} else {
		tloge("in %s err: drm ion handle invaild!\n", __func__);
		return -EFAULT;
	}
	return ret;
}
#else
static inline int alloc_for_ion(struct alloc_params *params_in, uint8_t kernel_params,
	unsigned int param_type, int index)
{
	tloge("not support hisi ion and releated feature!\n");
	return -1;
}
#endif

static int alloc_operation(struct tc_call_params *params,
	struct tc_ns_operation *operation, uint8_t flags)
{
	int ret;
	unsigned int index;
	unsigned int trans_paramtype_to_tee[TEE_PARAM_NUM] = { TEE_PARAM_TYPE_NONE };
	uint8_t kernel_params;
	unsigned int param_type;
	union tc_ns_client_param *client_param = NULL;
	struct alloc_params params_in = {
		params->dev_file, params->client_context, params->session,
		operation, params->local_temp_buffer, TEE_PARAM_NUM,
		trans_paramtype_to_tee, TEE_PARAM_NUM,
	};
	ret = check_params_for_alloc(params, operation);
	if (ret != 0)
		return ret;
	ret = check_context_for_alloc(params->client_context);
	if (ret != 0)
		return ret;
	kernel_params = params->dev_file->kernel_api;
	tlogd("Allocating param types %08X\n",
		params->client_context->param_types);
	/* Get the 4 params from the client context */
	for (index = 0; index < TEE_PARAM_NUM; index++) {
		/*
		 * Normally kernel_params = kernel_api
		 * But when TC_CALL_LOGIN(open session), params 2/3 will
		 * be filled by kernel for authentication. so under this circumstance,
		 * params 2/3 has to be set to kernel mode for  authentication; and
		 * param 0/1 will keep the same with user_api.
		 */
		set_kernel_params_for_open_session(flags, index, &kernel_params);
		client_param = &(params->client_context->params[index]);
		param_type = teec_param_type_get(
			params->client_context->param_types, index);
		tlogd("Param %u type is %x\n", index, param_type);
		if (teec_tmpmem_type(param_type, 2))
			/* temp buffers we need to allocate/deallocate
			 * for every operation
			 */
			ret = alloc_for_tmp_mem(&params_in, kernel_params,
				param_type, flags, index);
		else if (teec_memref_type(param_type, 2))
			/* MEMREF_PARTIAL buffers are already allocated so we just
			 * need to search for the shared_mem ref;
			 * For interface compatibility we assume buffer size to be 32bits
			 */
			ret = alloc_for_ref_mem(&params_in, kernel_params,
				param_type, index);
		else if (teec_value_type(param_type, 2))
			ret = copy_for_value(&params_in, kernel_params,
				param_type, index);
		else if (param_type == TEEC_ION_INPUT)
			ret = alloc_for_ion(&params_in, kernel_params,
				param_type, index);
		else if (param_type == TEEC_ION_SGLIST_INPUT)
			ret = alloc_for_ion_sglist(&params_in, kernel_params,
				param_type, index);
		else
			tlogd("param_type = TEEC_NONE\n");

		if (ret != 0)
			break;
	}
	if (ret != 0) {
		free_operation(params, operation);
		return ret;
	}
	operation->paramtypes =
		teec_param_types(trans_paramtype_to_tee[0],
		trans_paramtype_to_tee[1],
		trans_paramtype_to_tee[2],
		trans_paramtype_to_tee[3]);
	return ret;
}

static int check_params_for_update(const struct tc_call_params *in_params)
{
	if (in_params->dev_file == NULL) {
		tloge("dev_file is null");
		return -EINVAL;
	}
	if (in_params->client_context == NULL) {
		tloge("client_context is null");
		return -EINVAL;
	}
	if (in_params->local_temp_buffer == NULL) {
		tloge("local_temp_buffer is null");
		return -EINVAL;
	}
	if (in_params->tmp_buf_size != TEE_PARAM_NUM) {
		tloge("tmp_buf_size is invalid");
		return -EINVAL;
	}
	return 0;
}

static int update_for_tmp_mem(struct update_params *params_in, int index)
{
	union tc_ns_client_param *client_param = NULL;
	uint32_t buffer_size;
	struct tc_ns_dev_file *dev_file = params_in->dev_file;
	struct tc_ns_client_context *client_context = params_in->client_context;
	struct tc_ns_operation *operation = params_in->operation;
	struct tc_ns_temp_buf *local_temp_buffer = params_in->local_temp_buffer;
	bool is_complete = params_in->is_complete;
	bool check_value = params_in->tmp_buf_size != TEE_PARAM_NUM ||
		index >= TEE_PARAM_NUM;

	if (check_value == true) {
		tloge("tmp_buf_size or index is invalid\n");
		return -EFAULT;
	}
	/* temp buffer */
	buffer_size = operation->params[index].memref.size;
	client_param = &(client_context->params[index]);
	/* Size is updated all the time */
	if (write_to_client((void *)(uintptr_t)client_param->memref.size_addr,
		sizeof(buffer_size),
		&buffer_size, sizeof(buffer_size),
		dev_file->kernel_api)) {
		tloge("copy tempbuf size failed\n");
		return -EFAULT;
	}
	if (buffer_size > local_temp_buffer[index].size) {
		/* incomplete case, when the buffer size is invalid see next param */
		if (!is_complete)
			return 0;
		/* complete case, operation is allocated from mailbox
		 *  and share with gtask, so it's possible to be changed
		 */
		tloge("client_param->memref.size has been changed larger than the initial\n");
		return -EFAULT;
	}
	/* Only update the buffer when the buffer size is valid in complete case */
	if (write_to_client((void *)(uintptr_t)client_param->memref.buffer,
		operation->params[index].memref.size,
		local_temp_buffer[index].temp_buffer,
		operation->params[index].memref.size, dev_file->kernel_api)) {
		tloge("copy tempbuf failed\n");
		return -ENOMEM;
	}
	return 0;
}

static int update_for_ref_mem(struct update_params *params_in, unsigned int index)
{
	union tc_ns_client_param *client_param = NULL;
	uint32_t buffer_size;
	bool check_value = false;
	unsigned int orig_size = 0;
	struct tc_ns_dev_file *dev_file = params_in->dev_file;
	struct tc_ns_client_context *client_context = params_in->client_context;
	struct tc_ns_operation *operation = params_in->operation;

	if (index >= TEE_PARAM_NUM) {
		tloge("index is invalid\n");
		return -EFAULT;
	}
	/* update size */
	buffer_size = operation->params[index].memref.size;
	client_param = &(client_context->params[index]);

	if (read_from_client(&orig_size,
		sizeof(orig_size),
		(uint32_t __user *)(uintptr_t)client_param->memref.size_addr,
		sizeof(orig_size), dev_file->kernel_api)) {
		tloge("copy orig memref.size_addr failed\n");
		return -EFAULT;
	}
	if (write_to_client((void *)(uintptr_t)client_param->memref.size_addr,
		sizeof(buffer_size),
		&buffer_size, sizeof(buffer_size), dev_file->kernel_api)) {
		tloge("copy buf size failed\n");
		return -EFAULT;
	}
	/* copy from mb_buffer to sharemem */
	check_value = operation->mb_buffer[index] && orig_size >= buffer_size;
	if (check_value == true) {
		void *buffer_addr =
			(void *)(uintptr_t)((uintptr_t)
			operation->sharemem[index]->kernel_addr +
			client_param->memref.offset);
		if (memcpy_s(buffer_addr,
			operation->sharemem[index]->len -
			client_param->memref.offset,
			operation->mb_buffer[index], buffer_size)) {
			tloge("copy to sharemem failed\n");
			return -EFAULT;
		}
	}
	return 0;
}

static int update_for_value(struct update_params *params_in, unsigned int index)
{
	union tc_ns_client_param *client_param = NULL;
	struct tc_ns_dev_file *dev_file = params_in->dev_file;
	struct tc_ns_client_context *client_context = params_in->client_context;
	struct tc_ns_operation *operation = params_in->operation;

	if (index >= TEE_PARAM_NUM) {
		tloge("index is invalid\n");
		return -EFAULT;
	}
	client_param = &(client_context->params[index]);
	if (write_to_client((void *)(uintptr_t)client_param->value.a_addr,
		sizeof(operation->params[index].value.a),
		&operation->params[index].value.a,
		sizeof(operation->params[index].value.a),
		dev_file->kernel_api)) {
		tloge("inc copy value.a_addr failed\n");
		return -EFAULT;
	}
	if (write_to_client((void *)(uintptr_t)client_param->value.b_addr,
		sizeof(operation->params[index].value.b),
		&operation->params[index].value.b,
		sizeof(operation->params[index].value.b),
		dev_file->kernel_api)) {
		tloge("inc copy value.b_addr failed\n");
		return -EFAULT;
	}
	return 0;
}

static int update_client_operation(struct tc_call_params *params,
	struct tc_ns_operation *operation, bool is_complete, bool is_oper_init)
{
	union tc_ns_client_param *client_param = NULL;
	int ret;
	unsigned int param_type;
	unsigned int index;
	struct update_params params_in = { params->dev_file, params->client_context,
		operation, params->local_temp_buffer, TEE_PARAM_NUM, is_complete
	};

	/* update size */
	if (!is_oper_init)
		return 0;

	ret = check_params_for_update(params);
	if (ret != 0)
		return -EINVAL;
	/* if paramTypes is NULL, no need to update */
	if (params->client_context->param_types == 0)
		return 0;
	for (index = 0; index < TEE_PARAM_NUM; index++) {
		client_param = &(params->client_context->params[index]);
		param_type = teec_param_type_get(
			params->client_context->param_types, index);
		if (teec_tmpmem_type(param_type, 1))
			ret = update_for_tmp_mem(&params_in, index);
		else if (teec_memref_type(param_type, 1))
			ret = update_for_ref_mem(&params_in, index);
		else if (is_complete && teec_value_type(param_type, 1))
			ret = update_for_value(&params_in, index);
		else
			tlogd("param_type:%u don't need to update.\n",
				param_type);
		if (ret != 0)
			break;
	}
	return ret;
}

static void free_operation(struct tc_call_params *params, struct tc_ns_operation *operation)
{
	unsigned int param_type;
	unsigned int index;
	void *temp_buf = NULL;
	bool check_temp_mem = false;
	bool check_part_mem = false;
	struct tc_ns_temp_buf *local_temp_buffer = params->local_temp_buffer;
	struct tc_ns_client_context *client_context = params->client_context;

	if (params->tmp_buf_size != TEE_PARAM_NUM)
		tloge("tmp_buf_size is invalid %x.\n", params->tmp_buf_size);
	for (index = 0; index < TEE_PARAM_NUM; index++) {
		param_type = teec_param_type_get(
			client_context->param_types, index);
		check_temp_mem = param_type == TEEC_MEMREF_TEMP_INPUT ||
			param_type == TEEC_MEMREF_TEMP_OUTPUT ||
			param_type == TEEC_MEMREF_TEMP_INOUT;
		check_part_mem = param_type == TEEC_MEMREF_PARTIAL_INPUT ||
			param_type == TEEC_MEMREF_PARTIAL_OUTPUT ||
			param_type == TEEC_MEMREF_PARTIAL_INOUT;
		if (check_temp_mem == true) {
			/* free temp buffer */
			temp_buf = local_temp_buffer[index].temp_buffer;
			tlogd("Free temp buf, i = %u\n", index);
			if (virt_addr_valid((unsigned long)(uintptr_t)temp_buf) &&
				!ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)temp_buf)) {
				mailbox_free(temp_buf);
				temp_buf = NULL;
			}
		} else if (check_part_mem == true) {
			put_sharemem_struct(operation->sharemem[index]);
			if (operation->mb_buffer[index])
				mailbox_free(operation->mb_buffer[index]);
		} else if (param_type == TEEC_ION_SGLIST_INPUT) {
			temp_buf = local_temp_buffer[index].temp_buffer;
			tlogd("Free ion sglist buf, i = %u\n", index);
			if (virt_addr_valid((uint64_t)(uintptr_t)temp_buf) &&
				!ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)temp_buf)) {
				mailbox_free(temp_buf);
				temp_buf = NULL;
			}
		}
	}
}

static int check_params_for_client_call(const struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *client_context)
{
	if (dev_file == NULL) {
		tloge("dev_file is null");
		return -EINVAL;
	}
	if (client_context == NULL) {
		tloge("client_context is null");
		return -EINVAL;
	}
	return 0;
}

static int alloc_for_client_call(struct tc_ns_smc_cmd **smc_cmd,
	struct mb_cmd_pack **mb_pack)
{
	*smc_cmd = kzalloc(sizeof(**smc_cmd), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)(*smc_cmd))) {
		tloge("smc_cmd malloc failed.\n");
		return -ENOMEM;
	}
	*mb_pack = mailbox_alloc_cmd_pack();
	if (*mb_pack == NULL) {
		kfree(*smc_cmd);
		*smc_cmd = NULL;
		return -ENOMEM;
	}
	return 0;
}

static int init_smc_cmd(const struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *client_context, struct tc_ns_smc_cmd *smc_cmd,
	const struct mb_cmd_pack *mb_pack, bool global)
{
	smc_cmd->global_cmd = global;
	if (memcpy_s(smc_cmd->uuid, sizeof(smc_cmd->uuid),
		client_context->uuid, UUID_LEN)) {
		tloge("memcpy_s uuid error.\n");
		return -EFAULT;
	}
	smc_cmd->cmd_id = client_context->cmd_id;
	smc_cmd->dev_file_id = dev_file->dev_file_id;
	smc_cmd->context_id = client_context->session_id;
	smc_cmd->err_origin = client_context->returns.origin;
	smc_cmd->started = client_context->started;

	if (tzmp2_uid(client_context, smc_cmd, global) != EOK)
		tloge("caution! tzmp uid failed !\n\n");

	tlogv("current uid is %u\n", smc_cmd->uid);
	if (client_context->param_types != 0) {
		smc_cmd->operation_phys = virt_to_phys(&mb_pack->operation);
		smc_cmd->operation_h_phys =
			virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;
	} else {
		smc_cmd->operation_phys = 0;
		smc_cmd->operation_h_phys = 0;
	}
	smc_cmd->login_method = client_context->login.method;
	return 0;
}

static int check_login_for_encrypt(struct tc_ns_client_context *client_context,
	struct tc_ns_session *session, struct tc_ns_smc_cmd *smc_cmd,
	struct mb_cmd_pack *mb_pack, int need_check_login)
{
	if (need_check_login && session != NULL) {
#ifdef CONFIG_AUTH_ENHANCE
		int ret = do_encryption(session->auth_hash_buf,
			sizeof(session->auth_hash_buf),
			MAX_SHA_256_SZ * (NUM_OF_SO + 1),
			session->secure_info.crypto_info.key);
		if (ret) {
			tloge("hash encryption failed ret=%d\n", ret);
			return ret;
		}
#endif
		if (memcpy_s(mb_pack->login_data, sizeof(mb_pack->login_data),
			session->auth_hash_buf,
			sizeof(session->auth_hash_buf))) {
			tloge("copy login data failed\n");
			return -EFAULT;
		}
		smc_cmd->login_data_phy = virt_to_phys(mb_pack->login_data);
		smc_cmd->login_data_h_addr =
			virt_to_phys(mb_pack->login_data) >> ADDR_TRANS_NUM;
		smc_cmd->login_data_len = MAX_SHA_256_SZ * (NUM_OF_SO + 1);
	} else {
		smc_cmd->login_data_phy = 0;
		smc_cmd->login_data_h_addr = 0;
		smc_cmd->login_data_len = 0;
	}
	return 0;
}

static void get_uid_for_cmd(uint32_t *uid)
{
	kuid_t kuid;

	kuid.val = 0;
	kuid = current_uid();
	*uid = kuid.val;
}

static int proc_check_login_for_open_session(
	struct tc_call_params *params, struct mb_cmd_pack *mb_pack,
	bool global, struct tc_ns_smc_cmd *smc_cmd)
{
	int ret;
	int need_check_login;
	struct tc_ns_dev_file *dev_file = params->dev_file;
	struct tc_ns_client_context *client_context = params->client_context;
	struct tc_ns_session *session = params->session;

	ret = init_smc_cmd(dev_file, client_context,
		smc_cmd, mb_pack, global);
	if (ret != 0)
		return ret;
	need_check_login = dev_file->pub_key_len == sizeof(uint32_t) &&
		smc_cmd->cmd_id == GLOBAL_CMD_ID_OPEN_SESSION &&
		(current->mm != NULL) && global;
	ret = check_login_for_encrypt(client_context, session,
		smc_cmd, mb_pack, need_check_login);
	if (ret != 0)
		return ret;

	smc_cmd->ca_pid = current->pid;

	return ret;
}

static void reset_session_id(struct tc_ns_client_context *client_context,
	bool global, const struct tc_ns_smc_cmd *smc_cmd, int tee_ret)
{
	bool need_reset = false;

	client_context->session_id = smc_cmd->context_id;
	// if tee_ret error except TEEC_PENDING,but context_id is seted,need to reset to 0.
	need_reset = (global &&
		client_context->cmd_id == GLOBAL_CMD_ID_OPEN_SESSION &&
		tee_ret != 0 && tee_ret != TEEC_PENDING);
	if (need_reset)
		client_context->session_id = 0;
	return;
}

static void pend_ca_thread(struct tc_ns_session *session, const struct tc_ns_smc_cmd *smc_cmd)
{
	struct tc_wait_data *wq = NULL;

	if (session != NULL)
		wq = &session->wait_data;
	if (wq != NULL) {
		tlogv("before wait event\n");
		/* use wait_event instead of wait_event_interruptible so
		 * that ap suspend will not wake up the TEE wait call
		 */
		wait_event(wq->send_cmd_wq, wq->send_wait_flag);
		wq->send_wait_flag = 0;
	}
	tlogv("operation start is :%d\n", smc_cmd->started);
	return;
}


static void proc_free_src(struct tc_call_params *params,
	struct mb_cmd_pack *mb_pack, const struct tc_ns_smc_cmd *smc_cmd,
	int tee_ret, bool is_oper_init)
{
	/* kfree(NULL) is safe and this check is probably not required */
	params->client_context->returns.code = tee_ret;
	params->client_context->returns.origin = smc_cmd->err_origin;

	/* when CA invoke command and crash,
	 * Gtask happen to release service node ,tzdriver need to kill ion;
	 * ta crash ,tzdriver also need to kill ion
	 */
	if (tee_ret == TEE_ERROR_TAGET_DEAD || tee_ret == TEEC_ERROR_GENERIC) {
		tloge("ta_crash or ca is killed or some error happen\n");
		kill_ion_by_uuid((struct tc_uuid *)smc_cmd->uuid);
	}

	if (is_oper_init && mb_pack != NULL)
		free_operation(params, &mb_pack->operation);

	kfree(smc_cmd);
	mailbox_free(mb_pack);
	return;
}

static void proc_short_buffer(struct tc_call_params *params, struct tc_ns_operation *operation,
	struct tc_ns_smc_cmd *smc_cmd, int tee_ret, bool is_oper_init)
{
	if (tee_ret != TEEC_ERROR_SHORT_BUFFER)
		return;

	if (update_client_operation(params, operation, false, is_oper_init) != 0)
		smc_cmd->err_origin = TEEC_ORIGIN_COMMS;
}

int tc_client_call(struct tc_ns_client_context *client_context,
	struct tc_ns_dev_file *dev_file, struct tc_ns_session *session, uint8_t flags)
{
	int ret;
	int tee_ret = 0;
	struct tc_ns_smc_cmd *smc_cmd = NULL;
	uint32_t uid = 0;
	struct mb_cmd_pack *mb_pack = NULL;
	bool is_oper_init = false;
	struct tc_ns_temp_buf local_temp_buffer[TEE_PARAM_NUM] = {
		{ NULL, 0 }, { NULL, 0 }, { NULL, 0 }, { NULL, 0 }
	};
	struct tc_call_params in_params = { dev_file, client_context, session,
		local_temp_buffer, TEE_PARAM_NUM
	};

	get_uid_for_cmd(&uid);
	ret = check_params_for_client_call(dev_file, client_context);
	if (ret != 0)
		return ret;

	ret = alloc_for_client_call(&smc_cmd, &mb_pack);
	if (ret != 0)
		return ret;

	smc_cmd->uid = uid;
	if (client_context->param_types != 0) {
		ret = alloc_operation(&in_params, &mb_pack->operation, flags);
		if (ret != 0) {
			tloge("alloc_operation malloc failed");
			goto free_src;
		}
		is_oper_init = true;
	}

	ret = proc_check_login_for_open_session(&in_params, mb_pack,
		(bool)(flags & TC_CALL_GLOBAL), smc_cmd);
	if (ret != 0)
		goto free_src;

	ret = load_security_enhance_info(&in_params, smc_cmd,
		mb_pack, (bool)(flags & TC_CALL_GLOBAL));
	if (ret != 0) {
		tloge("load_security_enhance_info failed.\n");
		goto free_src;
	}

	/* send smc to secure world */
	tee_ret = tc_ns_smc(smc_cmd);
	reset_session_id(client_context, (bool)(flags & TC_CALL_GLOBAL), smc_cmd, tee_ret);
#ifdef CONFIG_AUTH_ENHANCE
	ret = post_process_token(&in_params, smc_cmd, mb_pack->token,
		sizeof(mb_pack->token), (bool)(flags & TC_CALL_GLOBAL));
	if (ret != EOK) {
		tloge("post_process_token failed.\n");
		smc_cmd->err_origin = TEEC_ORIGIN_COMMS;
		goto free_src;
	}
#endif
	if (tee_ret != 0) {
		while (tee_ret == TEEC_PENDING) {
			pend_ca_thread(session, smc_cmd);
#ifdef CONFIG_AUTH_ENHANCE
			ret = append_teec_token(&in_params, smc_cmd,
				(bool)(flags & TC_CALL_GLOBAL), mb_pack->token,
				sizeof(mb_pack->token));
			if (ret != 0) {
				tloge("append teec's member failed\n");
				goto free_src;
			}
#endif
			tee_ret = tc_ns_smc_with_no_nr(smc_cmd);
#ifdef CONFIG_AUTH_ENHANCE
			ret = post_process_token(&in_params, smc_cmd,
				mb_pack->token, sizeof(mb_pack->token),
				(bool)(flags & TC_CALL_GLOBAL));
			if (ret != 0) {
				tloge("no nr smc post_process_token failed.\n");
				goto free_src;
			}
#endif
		}
		/* Client was interrupted, return and let it handle it's own signals first then retry */
		if (tee_ret == TEEC_CLIENT_INTR) {
			ret = -ERESTARTSYS;
			goto free_src;
		} else if (tee_ret) {
			tloge("smc_call returns error ret 0x%x\n", tee_ret);
			tloge("smc_call smc cmd ret 0x%x\n", smc_cmd->ret_val);
			/*
			 * comptible with libteec_vendor.so
			 * err from TEE, ret is positive,
			 * err frmo tzdriver, ret is positive
			 */
			ret = EFAULT;
			goto proc_short_buf;
		}
		client_context->session_id = smc_cmd->context_id;
	}
	/* wake_up tee log reader */
	tz_log_write();
	ret = update_client_operation(&in_params, &mb_pack->operation,
		true, is_oper_init);
	if (ret != 0)
		smc_cmd->err_origin = TEEC_ORIGIN_COMMS;
	goto free_src;
proc_short_buf:
	proc_short_buffer(&in_params, &mb_pack->operation, smc_cmd,
		tee_ret, is_oper_init);
free_src:
	proc_free_src(&in_params, mb_pack, smc_cmd, tee_ret, is_oper_init);
	return ret;
}
