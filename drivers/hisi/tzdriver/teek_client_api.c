/*
 * teek_client_api.c
 *
 * function definition for libteec interface for kernel CA.
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
#include "teek_client_api.h"
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/kernel.h>
#include <securec.h>
#include "teek_client_id.h"
#include "tc_ns_log.h"
#include "tc_ns_client.h"

static void encde_for_part_mem(struct tc_ns_client_context *cli_context,
	const struct teec_operation *operation, uint32_t param_cnt, uint32_t *param_type)
{
	uint32_t diff = (uint32_t)TEEC_MEMREF_PARTIAL_INPUT -
		(uint32_t)TEEC_MEMREF_TEMP_INPUT;

	if (param_cnt < TEE_PARAM_NUM) {
		/* buffer offset len */
		if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {
			cli_context->params[param_cnt].memref.offset = 0;
			cli_context->params[param_cnt].memref.size_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].memref.parent->size;
		} else {
			cli_context->params[param_cnt].memref.offset =
				operation->params[param_cnt].memref.offset;
			cli_context->params[param_cnt].memref.size_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].memref.size;
		}
		if (operation->params[param_cnt].memref.parent->is_allocated) {
			cli_context->params[param_cnt].memref.buffer =
				(__u64)(uintptr_t)
				operation->params[param_cnt].memref.parent->buffer;
		} else {
			cli_context->params[param_cnt].memref.buffer =
				(__u64)(uintptr_t)
				operation->params[param_cnt].memref.parent->buffer +
				operation->params[param_cnt].memref.offset;
			cli_context->params[param_cnt].memref.offset = 0;
		}
		/* translate the paramType to know the driver */
		if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {
			switch (operation->params[param_cnt].memref.parent->flags) {
			case TEEC_MEM_INPUT:
				param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INPUT;
				break;
			case TEEC_MEM_OUTPUT:
				param_type[param_cnt] = TEEC_MEMREF_PARTIAL_OUTPUT;
				break;
			case TEEC_MEM_INOUT:
				param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
				break;
			default:
				param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
				break;
			}
		}
		/* if is not allocated, translate TEEC_MEMREF_PARTIAL_XXX to TEEC_MEMREF_TEMP_XXX */
		if (!operation->params[param_cnt].memref.parent->is_allocated)
			param_type[param_cnt] = param_type[param_cnt] - diff;
	}
	return;
}

static uint32_t proc_teek_encode(struct tc_ns_client_context *cli_context,
	const struct teec_operation *operation)
{
	bool check_value = false;
	bool check_temp_mem = false;
	bool check_part_mem = false;
	uint32_t param_type[TEE_PARAM_NUM];
	bool check_ion_value = false;
	uint32_t param_cnt;

	param_type[0] =
		teec_param_type_get(operation->paramtypes, 0);
	param_type[1] =
		teec_param_type_get(operation->paramtypes, 1);
	param_type[2] =
		teec_param_type_get(operation->paramtypes, 2);
	param_type[3] =
		teec_param_type_get(operation->paramtypes, 3);
	for (param_cnt = 0; param_cnt < TEE_PARAM_NUM; param_cnt++) {
		check_temp_mem = param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT ||
			param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT ||
			param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT;
		check_part_mem = param_type[param_cnt] == TEEC_MEMREF_WHOLE ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT;
		check_value = param_type[param_cnt] == TEEC_VALUE_INPUT ||
			param_type[param_cnt] == TEEC_VALUE_OUTPUT ||
			param_type[param_cnt] == TEEC_VALUE_INOUT;
		check_ion_value = param_type[param_cnt] == TEEC_ION_INPUT ||
			param_type[param_cnt] == TEEC_ION_SGLIST_INPUT;
		if (check_temp_mem == true) {
			cli_context->params[param_cnt].memref.buffer =
				(__u64)(uintptr_t)
				(operation->params[param_cnt].tmpref.buffer);
			cli_context->params[param_cnt].memref.size_addr =
				(__u64)(uintptr_t)
				(&operation->params[param_cnt].tmpref.size);
		} else if (check_part_mem == true) {
			encde_for_part_mem(cli_context, operation,
				param_cnt, param_type);
		} else if (check_value == true) {
			cli_context->params[param_cnt].value.a_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].value.a;
			cli_context->params[param_cnt].value.b_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].value.b;
		} else if (check_ion_value == true) {
			cli_context->params[param_cnt].value.a_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].ionref.ion_share_fd;
			cli_context->params[param_cnt].value.b_addr =
				(__u64)(uintptr_t)
				&operation->params[param_cnt].ionref.ion_size;
		} else if (param_type[param_cnt] == TEEC_NONE) {
			/* do nothing */
		} else {
			tloge("param_type[%u]=%u not correct\n", param_cnt,
				param_type[param_cnt]);
			return TEEC_ERROR_BAD_PARAMETERS;
		}
	}
	cli_context->param_types = teec_param_types(param_type[0],
		param_type[1], param_type[2],
		param_type[3]);
	return TEEC_SUCCESS;
}

static uint32_t teek_encode(struct tc_ns_client_context *cli_context,
	struct teec_uuid service_id, uint32_t session_id, uint32_t cmd_id,
	const struct tc_ns_client_login *cli_login, const struct teec_operation *operation)
{
	uint32_t diff;
	uint32_t ret;
	errno_t sret;

	if (cli_context == NULL || cli_login == NULL) {
		tloge("cli_context or cli_login is null.\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	diff = (uint32_t)TEEC_MEMREF_PARTIAL_INPUT -
		(uint32_t)TEEC_MEMREF_TEMP_INPUT;
	sret = memset_s(cli_context, sizeof(*cli_context),
		0x00, sizeof(*cli_context));
	if (sret != EOK) {
		tloge("memset error sret is %d.\n", sret);
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	sret = memcpy_s(cli_context->uuid, sizeof(cli_context->uuid),
		(uint8_t *)&service_id, sizeof(service_id));
	if (sret != EOK) {
		tloge("memcpy error sret is %d.\n", sret);
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	cli_context->session_id = session_id;
	cli_context->cmd_id = cmd_id;
	cli_context->returns.code = 0;
	cli_context->returns.origin = 0;
	cli_context->login.method = cli_login->method;
	cli_context->login.mdata = cli_login->mdata;
	/* support when operation is null */
	if (operation == NULL)
		return TEEC_SUCCESS;
	cli_context->started = operation->cancel_flag;
	ret = proc_teek_encode(cli_context, operation);
	tlogv("cli param type %u\n", cli_context->param_types);
	return ret;
}

static uint32_t teek_check_tmp_ref(struct teec_tempmemory_reference tmpref)
{
	uint32_t ret;
	bool check_value = (tmpref.buffer == NULL) || (tmpref.size == 0);

	if (check_value == true) {
		tloge("tmpref buffer is null, or size is zero\n");
		ret = TEEC_ERROR_BAD_PARAMETERS;
	} else {
		ret = TEEC_SUCCESS;
	}
	return ret;
}

static uint32_t teek_check_mem_ref(struct teec_registeredmemory_reference memref,
	uint32_t param_type)
{
	bool check_value = (memref.parent == NULL) || (memref.parent->buffer == NULL);
	bool check_offset = false;

	if (check_value == true) {
		tloge("parent of memref is null, or the buffer is zero\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (param_type == TEEC_MEMREF_PARTIAL_INPUT) {
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			return TEEC_ERROR_BAD_PARAMETERS;
	} else if (param_type == TEEC_MEMREF_PARTIAL_OUTPUT) {
		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			return TEEC_ERROR_BAD_PARAMETERS;
	} else if (param_type == TEEC_MEMREF_PARTIAL_INOUT) {
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			return TEEC_ERROR_BAD_PARAMETERS;
		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			return TEEC_ERROR_BAD_PARAMETERS;
	} else if (param_type == TEEC_MEMREF_WHOLE) {
		/* if type is TEEC_MEMREF_WHOLE, ignore it */
	} else {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	check_value = (param_type == TEEC_MEMREF_PARTIAL_INPUT) ||
		(param_type == TEEC_MEMREF_PARTIAL_OUTPUT) ||
		(param_type == TEEC_MEMREF_PARTIAL_INOUT);
	if (check_value == true) {
		check_offset = (memref.offset + memref.size) > memref.parent->size ||
			(memref.offset + memref.size) < memref.offset ||
			(memref.offset + memref.size) < memref.size;
		if (check_offset == true) {
			tloge("offset + size exceed the parent size\n");
			return TEEC_ERROR_BAD_PARAMETERS;
		}
	}
	return TEEC_SUCCESS;
}

/*
 * This function checks a operation is valid or not.
 */
uint32_t teek_check_operation(const struct teec_operation *operation)
{
	uint32_t param_type[TEE_PARAM_NUM] = {0};
	uint32_t param_cnt;
	uint32_t ret = TEEC_SUCCESS;
	bool check_value = false;
	bool check_temp_mem = false;
	bool check_part_mem = false;
	bool check_ion_value = false;

	/* GP Support operation is NULL
	 * operation: a pointer to a Client Application initialized struct teec_operation structure,
	 * or NULL if there is no payload to send or if the Command does not need to support
	 * cancellation.
	 */
	if (operation == NULL)
		return TEEC_SUCCESS;
	if (!operation->started) {
		tloge("sorry, cancellation not support\n");
		return TEEC_ERROR_NOT_IMPLEMENTED;
	}
	param_type[0] =
		teec_param_type_get(operation->paramtypes, 0);
	param_type[1] =
		teec_param_type_get(operation->paramtypes, 1);
	param_type[2] =
		teec_param_type_get(operation->paramtypes, 2);
	param_type[3] =
		teec_param_type_get(operation->paramtypes, 3);
	for (param_cnt = 0; param_cnt < TEE_PARAM_NUM; param_cnt++) {
		check_temp_mem = param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT ||
			param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT ||
			param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT;
		check_part_mem = param_type[param_cnt] == TEEC_MEMREF_WHOLE ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT ||
			param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT;
		check_value = param_type[param_cnt] == TEEC_VALUE_INPUT ||
			param_type[param_cnt] == TEEC_VALUE_OUTPUT ||
			param_type[param_cnt] == TEEC_VALUE_INOUT;
		check_ion_value = param_type[param_cnt] == TEEC_ION_INPUT ||
			param_type[param_cnt] == TEEC_ION_SGLIST_INPUT;
		if (check_temp_mem == true) {
			ret = teek_check_tmp_ref(
				operation->params[param_cnt].tmpref);
			if (ret != TEEC_SUCCESS)
				break;
		} else if (check_part_mem == true) {
			ret = teek_check_mem_ref(
				operation->params[param_cnt].memref,
				param_type[param_cnt]);
			if (ret != TEEC_SUCCESS)
				break;
		} else if (check_value == true) {
			/* if type is value, ignore it */
		} else if (param_type[param_cnt] == TEEC_NONE) {
			/* if type is none, ignore it */
		} else if (check_ion_value == true) {
			if (operation->params[param_cnt].ionref.ion_share_fd < 0) {
				tloge("operation check failed: ion_handle is invalid!\n");
				ret = TEEC_ERROR_BAD_PARAMETERS;
				break;
			}
		} else {
			tloge("paramType[%u]=%x is not support\n", param_cnt,
				param_type[param_cnt]);
			ret = TEEC_ERROR_BAD_PARAMETERS;
			break;
		}
	}
	return ret;
}

/*
 * This function check if the special agent is launched.Used For HDCP key.
 * e.g. If sfs agent is not alive, you can not do HDCP key write to SRAM.
 */
int teek_is_agent_alive(unsigned int agent_id)
{
	return is_agent_alive(agent_id);
}

/*
 * This function initializes a new TEE Context, forming a connection between this Client Application
 * and the TEE identified by the string identifier name.
 */
uint32_t teek_initialize_context(const char *name, struct teec_context *context)
{
	int32_t ret;

	/* name current not used */
	(void)(name);
	tlogd("teek_initialize_context Started:\n");
	/* First, check parameters is valid or not */
	if (context == NULL) {
		tloge("context is null, not correct\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	context->dev = NULL;
	/* Paramters right, start execution */
	ret = tc_ns_client_open((struct tc_ns_dev_file **)&context->dev,
		TEE_REQ_FROM_KERNEL_MODE);
	if (ret != TEEC_SUCCESS) {
		tloge("open device failed\n");
		return TEEC_ERROR_GENERIC;
	}
	tlogd("open device success\n");
	INIT_LIST_HEAD((struct list_head *)&context->session_list);
	INIT_LIST_HEAD((struct list_head *)&context->shrd_mem_list);
	return TEEC_SUCCESS;
}

/*
 * This function finalizes an initialized TEE Context.
 */
void teek_finalize_context(struct teec_context *context)
{
	struct list_node *ptr = NULL;
	struct teec_session *session = NULL;
	/* struct teec_sharedmemory* shrdmem */
	tlogd("teek_finalize_context started\n");
	/* First, check parameters is valid or not */
	if (context == NULL || context->dev == NULL) {
		tloge("context or dev is null, not correct\n");
		return;
	}
	/* Paramters right, start execution */
	if (!LIST_EMPTY(&context->session_list)) {
		tlogi("context still has sessions opened, close it\n");
		list_for_each(ptr, &context->session_list) {
			session = list_entry(ptr, struct teec_session, head);
			teek_close_session(session);
		}
	}
	tlogd("close device\n");
	tc_ns_client_close(context->dev);
	context->dev = NULL;
}


static uint32_t check_params_for_open_session(struct teec_context *context,
	const struct teec_operation *operation, struct tc_ns_client_login *cli_login)
{
	bool check_value = false;
	struct tc_ns_dev_file *dev_file = NULL;
	uint32_t teec_ret;
	errno_t sret;
	uint32_t param_type[TEE_PARAM_NUM] = {0};

	param_type[3] =
		teec_param_type_get(operation->paramtypes, 3);
	param_type[2] =
		teec_param_type_get(operation->paramtypes, 2);
	check_value = param_type[3] != TEEC_MEMREF_TEMP_INPUT ||
		param_type[2] != TEEC_MEMREF_TEMP_INPUT;
	if (check_value == true) {
		tloge("invalid param type 0x%x\n", operation->paramtypes);
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	check_value = operation->params[3].tmpref.buffer == NULL ||
		operation->params[2].tmpref.buffer == NULL ||
		operation->params[3].tmpref.size == 0 ||
		operation->params[2].tmpref.size == 0;
	if (check_value == true) {
		tloge("invalid operation params(NULL)\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	cli_login->method = TEEC_LOGIN_IDENTIFY;
	dev_file = (struct tc_ns_dev_file *)(context->dev);
	if (dev_file == NULL) {
		tloge("invalid context->dev (NULL)\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	dev_file->pkg_name_len = operation->params[3].tmpref.size;
	if (operation->params[3].tmpref.size >
		(MAX_PACKAGE_NAME_LEN - 1)) {
		return TEEC_ERROR_BAD_PARAMETERS;
	} else {
		sret = memset_s(dev_file->pkg_name, sizeof(dev_file->pkg_name),
			0, MAX_PACKAGE_NAME_LEN);
		if (sret != EOK) {
			tloge("memset error sret is %d.\n", sret);
			return TEEC_ERROR_BAD_PARAMETERS;
		}
		sret = memcpy_s(dev_file->pkg_name, sizeof(dev_file->pkg_name),
			operation->params[3].tmpref.buffer,
			operation->params[3].tmpref.size);
		if (sret != EOK) {
			tloge("memcpy error sret is %d.\n", sret);
			return TEEC_ERROR_BAD_PARAMETERS;
		}
	}
	dev_file->pub_key_len = 0;
	dev_file->login_setup = 1;
	teec_ret = teek_check_operation(operation);
	if (teec_ret != TEEC_SUCCESS) {
		tloge("operation is invalid\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	return teec_ret;
}

static uint32_t open_session_and_switch_ret(struct teec_session *session,
	struct teec_context *context, const struct teec_uuid *destination,
	struct tc_ns_client_context *cli_context, uint32_t *origin)
{
	int32_t ret;
	uint32_t teec_ret;

	ret = tc_ns_open_session(context->dev, cli_context);
	if (ret == 0) {
		tlogd("open session success\n");
		session->session_id = cli_context->session_id;
		session->service_id = *destination;
		session->ops_cnt = 0;
		INIT_LIST_HEAD((struct list_head *)&session->head);
		list_insert_tail(&context->session_list, &session->head);
		session->context = context;
		return TEEC_SUCCESS;
	} else if (ret < 0) {
		tloge("open session failed, ioctl errno = %u\n", ret);
		if (ret == -EFAULT)
			teec_ret = TEEC_ERROR_ACCESS_DENIED;
		else if (ret == -ENOMEM)
			teec_ret = TEEC_ERROR_OUT_OF_MEMORY;
		else if (ret == -EINVAL)
			teec_ret = TEEC_ERROR_BAD_PARAMETERS;
		else if (ret == -ERESTARTSYS)
			teec_ret = TEEC_CLIENT_INTR;
		else
			teec_ret = TEEC_ERROR_GENERIC;
		*origin = TEEC_ORIGIN_COMMS;
		return teec_ret;
	} else {
		tloge("open session failed, code=0x%x, origin=%u\n", cli_context->returns.code,
		      cli_context->returns.origin);
		teec_ret = (uint32_t)cli_context->returns.code;
		*origin = cli_context->returns.origin;
	}
	return teec_ret;
}

/*
 * Function:	   TEEC_OpenSession
 * Description:   This function opens a new Session
 * Parameters:   context: a pointer to an initialized TEE Context.
 * session: a pointer to a Session structure to open.
 * destination: a pointer to a UUID structure.
 * connectionMethod: the method of connection to use.
 * connectionData: any necessary data required to support the connection method chosen.
 * operation: a pointer to an Operation containing a set of Parameters.
 * returnOrigin: a pointer to a variable which will contain the return origin.
 * Return: TEEC_SUCCESS: success other: failure
 */
static uint32_t proc_teek_open_session(struct teec_context *context,
	struct teec_session *session, const struct teec_uuid *destination,
	uint32_t connection_method, const void *connection_data,
	const struct teec_operation *operation, uint32_t *return_origin)
{
	uint32_t teec_ret;
	uint32_t origin = TEEC_ORIGIN_API;
	struct tc_ns_client_context cli_context;
	struct tc_ns_client_login cli_login = { 0, 0 };
	bool check_value = false;

	tlogd("teek_open_session Started:\n");
	/* connectionData current not used */
	(void)(connection_data);
	/* returnOrigin maybe null, so only when it is valid, we set
	 * origin(error come from which module)
	 */
	if (return_origin != NULL)
		*return_origin = origin;
	/* First, check parameters is valid or not */
	check_value = (context == NULL || operation == NULL ||
		connection_method != TEEC_LOGIN_IDENTIFY);
	if (check_value == true || destination == NULL || session == NULL) {
		tloge("invalid input params\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	teec_ret = check_params_for_open_session(context, operation, &cli_login);
	if (teec_ret != TEEC_SUCCESS)
		goto ret_fail;
	/* Paramters right, start execution
	 * note:before open session success,
	 * we should send session=0 as initial state.
	 */
	teec_ret = teek_encode(&cli_context, *destination, 0,
		GLOBAL_CMD_ID_OPEN_SESSION, &cli_login, operation);
	if (teec_ret != TEEC_SUCCESS) {
		tloge("encode failed\n");
		goto ret_fail;
	}
#ifdef CONFIG_AUTH_ENHANCE
	cli_context.teec_token = session->teec_token;
	cli_context.token_len = sizeof(session->teec_token);
#endif
	teec_ret = open_session_and_switch_ret(session, context,
		destination, &cli_context, &origin);
	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL, set returnOrigin */
	if (teec_ret != TEEC_SUCCESS && return_origin != NULL)
		*return_origin = origin;
ret_fail:
	return teec_ret;
}

#define RETRY_TIMES 5
uint32_t teek_open_session(struct teec_context *context,
	struct teec_session *session, const struct teec_uuid *destination,
	uint32_t connection_method, const void *connection_data,
	const struct teec_operation *operation, uint32_t *return_origin)
{
	int i;
	uint32_t ret;

	for (i = 0; i < RETRY_TIMES; i++) {
		ret = proc_teek_open_session(context, session,
			destination, connection_method, connection_data,
			operation, return_origin);
		if (ret != (uint32_t)TEEC_CLIENT_INTR)
			return ret;
	}
	return ret;
}

/*
 * This function closes an opened Session.
 */
void teek_close_session(struct teec_session *session)
{
	int32_t ret;
	struct tc_ns_client_context cli_context;
	struct tc_ns_client_login cli_login = { 0, 0 };
	struct list_node *ptr = NULL;
	struct teec_session *temp_sess = NULL;
	bool found = false;
	bool check_value = false;
	errno_t sret;

	tlogd("teek_close_session started\n");
	/* First, check parameters is valid or not */
	check_value = session == NULL || session->context == NULL;
	if (check_value || session->context->dev == NULL) {
		tloge("input invalid session or session->context is null\n");
		return;
	}
	list_for_each(ptr, &session->context->session_list) {
		temp_sess = list_entry(ptr, struct teec_session, head);
		if (temp_sess == session) {
			found = true;
			break;
		}
	}
	if (!found) {
		tloge("session is not in the context list\n");
		return;
	}
	/* Paramters all right, start execution */
	if (session->ops_cnt)
		tloge("session still has commands running\n");
	if (teek_encode(&cli_context, session->service_id,
		session->session_id, GLOBAL_CMD_ID_CLOSE_SESSION,
		&cli_login, NULL) != TEEC_SUCCESS) {
		tloge("encode failed, just return\n");
		return;
	}
#ifdef CONFIG_AUTH_ENHANCE
	cli_context.teec_token = session->teec_token;
	cli_context.token_len = sizeof(session->teec_token);
#endif
	ret = tc_ns_close_session(session->context->dev, &cli_context);
	if (ret == 0) {
		tlogd("close session success\n");
		session->session_id = 0;
		sret = memset_s((uint8_t *)(&session->service_id),
			sizeof(session->service_id), 0x00, UUID_LEN);
		/* teek_close_session is void so go on execute */
		if (sret != EOK)
			tloge("memset error sret is %d.\n", sret);
#ifdef CONFIG_AUTH_ENHANCE
		sret = memset_s(session->teec_token,
			TOKEN_SAVE_LEN, 0x00, TOKEN_SAVE_LEN);
		if (sret != EOK)
			tloge("memset session's member error ret value is %d.\n", sret);
#endif
		session->ops_cnt = 0;
		list_remove(&session->head);
		session->context = NULL;
	} else {
		tloge("close session failed\n");
	}
}

static uint32_t invoke_cmd_and_switch_ret(struct teec_session *session,
	struct tc_ns_client_context *cli_context, uint32_t *origin)
{
	int32_t ret;
	uint32_t teec_ret;

	ret = tc_ns_send_cmd(session->context->dev, cli_context);
	if (ret == 0) {
		tlogd("invoke cmd success\n");
		teec_ret = TEEC_SUCCESS;
	} else if (ret < 0) {
		tloge("invoke cmd failed, ioctl errno = %d\n", ret);
		if (ret == -EFAULT)
			teec_ret = TEEC_ERROR_ACCESS_DENIED;
		else if (ret == -ENOMEM)
			teec_ret = TEEC_ERROR_OUT_OF_MEMORY;
		else if (ret == -EINVAL)
			teec_ret = TEEC_ERROR_BAD_PARAMETERS;
		else
			teec_ret = TEEC_ERROR_GENERIC;
		*origin = TEEC_ORIGIN_COMMS;
	} else {
		tloge("invoke cmd failed, code=0x%x, origin=%d\n",
			cli_context->returns.code, cli_context->returns.origin);
		teec_ret = (uint32_t)cli_context->returns.code;
		*origin = cli_context->returns.origin;
	}
	return teec_ret;
}

/* This function invokes a Command within the specified Session. */
uint32_t teek_invoke_command(struct teec_session *session, uint32_t command_id,
	struct teec_operation *operation, uint32_t *return_origin)
{
	uint32_t teec_ret = TEEC_ERROR_BAD_PARAMETERS;
	uint32_t origin = TEEC_ORIGIN_API;
	struct tc_ns_client_context cli_context;
	struct tc_ns_client_login cli_login = { 0, 0 };


	/* First, check parameters is valid or not */
	if (session == NULL || session->context == NULL) {
		tloge("input invalid session or session->context is null\n");
		if (return_origin != NULL)
			*return_origin = origin;
		return teec_ret;
	}
	teec_ret = teek_check_operation(operation);
	if (teec_ret != TEEC_SUCCESS) {
		tloge("operation is invalid\n");
		if (return_origin != NULL)
			*return_origin = origin;
		return teec_ret;
	}

	/* Paramters all right, start execution */
	session->ops_cnt++;
	teec_ret = teek_encode(&cli_context, session->service_id,
		session->session_id, command_id, &cli_login, operation);
	if (teec_ret != TEEC_SUCCESS) {
		tloge("encode failed\n");
		session->ops_cnt--;
		if (return_origin != NULL)
			*return_origin = origin;
		return teec_ret;
	}
#ifdef CONFIG_AUTH_ENHANCE
	cli_context.teec_token = session->teec_token;
	cli_context.token_len = sizeof(session->teec_token);
#endif
	teec_ret = invoke_cmd_and_switch_ret(session, &cli_context, &origin);
	session->ops_cnt--;
	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL, set *returnOrigin */
	if ((teec_ret != TEEC_SUCCESS) && (return_origin != NULL))
		*return_origin = origin;
	return teec_ret;
}

/*
 * This function registers a block of existing Client Application memory
 * as a block of Shared Memory within the scope of the specified TEE Context.
 */
uint32_t teek_register_shared_memory(struct teec_context *context,
	struct teec_sharedmemory *shared_mem)
{
	tloge("teek_register_shared_memory not supported\n");
	return TEEC_ERROR_NOT_SUPPORTED;
}

/*
 * This function allocates a new block of memory as a block of
 *  Shared Memory within the scope of the specified TEE Context.
 */
uint32_t teek_allocate_shared_memory(struct teec_context *context,
	struct teec_sharedmemory *shared_mem)
{
	tloge("teek_allocate_shared_memory not supported\n");
	return TEEC_ERROR_NOT_SUPPORTED;
}

/*
 * This function deregisters or deallocates
 * a previously initialized block of Shared Memory..
 */
void teek_release_shared_memory(struct teec_sharedmemory *shared_mem)
{
	tloge("teek_release_shared_memory not supported\n");
}

/*
 * This function requests the cancellation of a pending open Session operation or
 * a Command invocation operation.
 */
void teek_request_cancellation(struct teec_operation *operation)
{
	tloge("teek_request_cancellation not supported\n");
}

/* begin: for KERNEL-HAL out interface */
int TEEK_IsAgentAlive(unsigned int agent_id)
{
	return teek_is_agent_alive(agent_id);
}
EXPORT_SYMBOL(TEEK_IsAgentAlive);

TEEC_Result TEEK_InitializeContext(const char *name, TEEC_Context *context)
{
	return (TEEC_Result)teek_initialize_context(name, (struct teec_context *)context);
}
EXPORT_SYMBOL(TEEK_InitializeContext);

void TEEK_FinalizeContext(TEEC_Context *context)
{
	teek_finalize_context((struct teec_context *)context);
}
EXPORT_SYMBOL(TEEK_FinalizeContext);

TEEC_Result TEEK_OpenSession(TEEC_Context *context, TEEC_Session *session,
	const TEEC_UUID *destination, uint32_t connectionMethod,
	const void *connectionData, TEEC_Operation *operation,
	uint32_t *returnOrigin)
{
	return (TEEC_Result)teek_open_session(
		(struct teec_context *)context, (struct teec_session *)session,
		(struct teec_uuid *)destination, connectionMethod, connectionData,
		(struct teec_operation *)operation, returnOrigin);
}
EXPORT_SYMBOL(TEEK_OpenSession);

void TEEK_CloseSession(TEEC_Session *session)
{
	teek_close_session((struct teec_session *)session);
}
EXPORT_SYMBOL(TEEK_CloseSession);

TEEC_Result TEEK_InvokeCommand(TEEC_Session *session, uint32_t commandID,
	TEEC_Operation *operation, uint32_t *returnOrigin)
{
	return (TEEC_Result)teek_invoke_command(
		(struct teec_session *)session, commandID,
		(struct teec_operation *)operation, returnOrigin);
}
EXPORT_SYMBOL(TEEK_InvokeCommand);

TEEC_Result TEEK_RegisterSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem)
{
	return (TEEC_Result)teek_register_shared_memory(
		(struct teec_context *)context, (struct teec_sharedmemory *)sharedMem);
}
EXPORT_SYMBOL(TEEK_RegisterSharedMemory);

TEEC_Result TEEK_AllocateSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem)
{
	return (TEEC_Result)teek_allocate_shared_memory((struct teec_context *)context,
		(struct teec_sharedmemory *)sharedMem);
}
EXPORT_SYMBOL(TEEK_AllocateSharedMemory);

void TEEK_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem)
{
	teek_release_shared_memory((struct teec_sharedmemory *)sharedMem);
}
EXPORT_SYMBOL(TEEK_ReleaseSharedMemory);

void TEEK_RequestCancellation(TEEC_Operation *operation)
{
	teek_request_cancellation((struct teec_operation *)operation);
}

/* end: for KERNEL-HAL out interface */
