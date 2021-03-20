/*
 * tvp_adapter.c
 *
 * This is for vdec tvp adapter.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "tvp_adapter.h"
#include <teek_client_api.h>
#include "vfmw.h"

static hi_u32 g_sec_pint_enable = 0x3;
static hi_s32 g_sec_init;

#define HI_VCODEC_INVOKE_CODE_A 0x6728661c
#define HI_VCODEC_INVOKE_CODE_B 0x5b9c660c
#define set_invoke_code(value) \
	do { \
		(value).a = HI_VCODEC_INVOKE_CODE_A; \
		(value).b = HI_VCODEC_INVOKE_CODE_B; \
	} while (0)

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define sec_print(type, fmt, arg...) \
	do { \
	} while (0)
#else
#define sec_print(type, fmt, arg...) \
	do { \
		if (type == PRN_ALWS || (g_sec_pint_enable & (1 << type)) != 0) { \
			dprint(PRN_ALWS, fmt, ##arg); \
		} \
	} while (0)
#endif

typedef enum {
	HIVCODEC_CMD_ID_INVALID = 0x0,
	HIVCODEC_CMD_ID_INIT,
	HIVCODEC_CMD_ID_EXIT,
	HIVCODEC_CMD_ID_SUSPEND,
	HIVCODEC_CMD_ID_RESUME,
	HIVCODEC_CMD_ID_CONTROL,
	HIVCODEC_CMD_ID_RUN_PROCESS,
	HIVCODEC_CMD_ID_GET_IMAGE,
	HIVCODEC_CMD_ID_RELEASE_IMAGE,
	HIVCODEC_CMD_ID_READ_PROC,
	HIVCODEC_CMD_ID_WRITE_PROC,
} tee_hivcodec_cmd_id;

static hi_u8 g_packet_id;

static TEEC_Context g_tee_context;
static TEEC_Session g_tee_session;

hi_s32 tvp_vdec_suspend(void)
{
	hi_s32 ret;
	TEEC_Result result;
	TEEC_Operation operation;

	ret = memset_s(&operation, sizeof(TEEC_Operation), 0, sizeof(TEEC_Operation));
	if (ret != EOK) {
		dprint(PRN_FATAL, "memset_s err in function\n");
		return VDEC_ERR;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].value.a = 0;
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	set_invoke_code(operation.params[3].value);

	result = TEEK_InvokeCommand(&g_tee_session, HIVCODEC_CMD_ID_SUSPEND,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand HIVCODEC_CMD_ID_SUSPEND Failed\n");
		ret = VDEC_ERR;
	} else {
		ret = operation.params[0].value.a;
	}

	return ret;
}

hi_s32 tvp_vdec_resume(void)
{
	hi_s32 ret;
	TEEC_Result result;
	TEEC_Operation operation;

	ret = memset_s(&operation, sizeof(TEEC_Operation), 0, sizeof(TEEC_Operation));
	if (ret != EOK) {
		dprint(PRN_FATAL, "memset_s err in function\n");
		return VDEC_ERR;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].value.a = 0;
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	set_invoke_code(operation.params[3].value);

	result = TEEK_InvokeCommand(&g_tee_session, HIVCODEC_CMD_ID_RESUME,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand HIVCODEC_CMD_ID_RESUME Failed\n");
		ret = VDEC_ERR;
	} else {
		ret = operation.params[0].value.a;
	}

	return ret;
}

hi_s32 tvp_vdec_secure_init(void)
{
	TEEC_Result result;
	TEEC_Operation operation;
	hi_s32 ret;

	TEEC_UUID svc_id = { 0xf92dbe38, 0x4d09, 0x4422, { 0xaa, 0x34, 0x14, 0x99, 0x2e, 0x2a, 0x0b, 0xf2 } };

	const hi_u8 *package_name_0 =
		"/vendor/bin/hw/android.hardware.media.omx@1.0-service";
	const hi_u8 *package_name_1 = "sample_omxvdec";
	hi_u8 package_name[TVP_PACKAGE_NAME_MAX_LENGTH] = {0};
	hi_u32 uid = 0;

	if (g_sec_init != 0) {
		sec_print(PRN_FATAL, "init already\n");
		return VDEC_OK;
	}

	BUG_ON(strlen(package_name_0) >= sizeof(package_name));
	BUG_ON(strlen(package_name_1) >= sizeof(package_name));

	if (g_packet_id == 0) {
		ret = strncpy_s(package_name, TVP_PACKAGE_NAME_MAX_LENGTH,
			package_name_0, (sizeof(package_name) - 1));
		if (ret != EOK) {
			dprint(PRN_FATAL, "strncpy_s err in function\n");
			return VDEC_ERR;
		}
	} else {
		ret = strncpy_s(package_name, TVP_PACKAGE_NAME_MAX_LENGTH,
			package_name_1, (sizeof(package_name) - 1));
		if (ret != EOK) {
			dprint(PRN_FATAL, "strncpy_s err in function\n");
			return VDEC_ERR;
		}
	}

	result = TEEK_InitializeContext(NULL, &g_tee_context);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "TEEC_InitializeContext Failed\n");
		return VDEC_ERR;
	}

	ret = memset_s(&operation, sizeof(TEEC_Operation), 0,
		sizeof(TEEC_Operation));
	if (ret != EOK) {
		dprint(PRN_FATAL, "memset_s err in function\n");
		return VDEC_ERR;
	}
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	operation.params[TEEC_OPERATION_PARA_INDEX_SECOND].tmpref.buffer = (void *)(&uid);
	operation.params[TEEC_OPERATION_PARA_INDEX_SECOND].tmpref.size = sizeof(uid);
	operation.params[TEEC_OPERATION_PARA_INDEX_THIRD].tmpref.buffer = (void *)(package_name);
	operation.params[TEEC_OPERATION_PARA_INDEX_THIRD].tmpref.size = strlen(package_name) + 1;

	result = TEEK_OpenSession(&g_tee_context, &g_tee_session,
		&svc_id, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "TEEK_OpenSession Failed\n");
		goto InitWithfree_0;
	}

	g_sec_init = 1;
	return VDEC_OK;

InitWithfree_0:
	TEEK_FinalizeContext(&g_tee_context);
	return VDEC_ERR;
}

hi_s32 tvp_vdec_secure_exit(void)
{
	hi_s32 ret = VDEC_OK;

	sec_print(PRN_ERROR, "Enter\n");
	if (!g_sec_init) {
		sec_print(PRN_FATAL, "FATAL, init first\n");
		return VDEC_ERR;
	}

	TEEK_CloseSession(&g_tee_session);
	TEEK_FinalizeContext(&g_tee_context);

	sec_print(PRN_FATAL, "Success\n");
	g_sec_init = 0;

	return ret;
}
