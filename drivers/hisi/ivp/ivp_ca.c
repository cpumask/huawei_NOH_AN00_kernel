/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  this file define interfaces for hisi-ivp secure ca
 * Author:  donghaixu
 * Create:  2020-04-01
 */
#include "ivp_ca.h"
#include <teek_client_api.h>
#include <teek_client_id.h>
#include "ivp_log.h"

#define TEE_SECIVP_SERVICE_ID      TEE_SERVICE_DRM_GRALLOC
#define TEE_SECIVP_PACKAGE_NAME    "/vendor/bin/CameraDaemon"
#define TEE_SECIVP_UID             1013

#define INVALID_IOVA               0
#define IVP_TA_TRUE                1
#define IVP_TA_FALSE               0

#define IOMMU_READ   (1 << 0)
#define IOMMU_WRITE  (1 << 1)
#define IOMMU_CACHE  (1 << 2)
#define IOMMU_NOEXEC (1 << 3)
#define IOMMU_DEVICE (1 << 7)
#define IOMMU_SEC    (1 << 8)
#define IOMMU_EXEC   (1 << 9)

enum {
	TEE_SECIVP_CMD_MAP_DRM_BUFFER = 0,
	TEE_SECIVP_CMD_CORE_MEM_CFG,
	TEE_SECIVP_CMD_CORE_MEM_UNCFG,
	TEE_SECIVP_CMD_SEC_SMMU_INIT,
	TEE_SECIVP_CMD_SEC_SMMU_DEINIT,
	TEE_SECIVP_CMD_ALGO_SEC_MAP,
	TEE_SECIVP_CMD_ALGO_SEC_UNMAP,
	TEE_SECIVP_CMD_ALGO_NONSEC_MAP,
	TEE_SECIVP_CMD_ALGO_NONSEC_UNMAP,
	TEE_SECIVP_CMD_CAMERA_BUFFER_CFG,
	TEE_SECIVP_CMD_CAMERA_BUFFER_UNCFG,
	TEE_SECIVP_CMD_IVP_CLEAR,
	TEE_SECIVP_CMD_MAX
};

enum {
	TEE_PARAM_INDEX0 = 0,
	TEE_PARAM_INDEX1,
	TEE_PARAM_INDEX2,
	TEE_PARAM_INDEX3
};

enum {
	SECURE = 0,
	NON_SECURE
};

typedef struct {
	int sharefd;           /* memory fd */
	unsigned int size;     /* memory size */
	unsigned int type;     /* memory type */
	unsigned int da;       /* device address */
	unsigned int prot;     /* operation right */
	unsigned int sec_flag; /* secure flag */
	unsigned long long pa; /* physical address */
} secivp_mem_info;

static TEEC_Session g_secivp_session;
static TEEC_Context g_secivp_context;
static bool g_secivp_creat_teec = false;

static int teek_secivp_open(void)
{
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SECIVP_SERVICE_ID; /* Tzdriver id for secivp */
	TEEC_Operation operation = {0};
	unsigned int root_id = TEE_SECIVP_UID;
	char package_name[] = TEE_SECIVP_PACKAGE_NAME;

	ivp_info("enter");
	result = TEEK_InitializeContext(NULL, &g_secivp_context);
	if (result != TEEC_SUCCESS) {
		ivp_err("initialize context failed, result = 0x%x", result);
		return -EPERM;
	}

	operation.started = IVP_TA_TRUE;
	operation.cancel_flag = IVP_TA_FALSE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_NONE,
		TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT);

	operation.params[TEE_PARAM_INDEX2].tmpref.buffer = (void *)(&root_id);
	operation.params[TEE_PARAM_INDEX2].tmpref.size   = sizeof(root_id);
	operation.params[TEE_PARAM_INDEX3].tmpref.buffer = (void *)(package_name);
	operation.params[TEE_PARAM_INDEX3].tmpref.size   = strlen(package_name) + 1;

	result = TEEK_OpenSession(
		&g_secivp_context,
		&g_secivp_session,
		&svc_uuid,
		TEEC_LOGIN_IDENTIFY,
		NULL,
		&operation,
		NULL);
	if (result != TEEC_SUCCESS) {
		ivp_err("open session failed, result = 0x%x", result);
		TEEK_FinalizeContext(&g_secivp_context);
		return -EPERM;
	}
	g_secivp_creat_teec = true;
	return 0;
}

static void teek_secivp_close(void)
{
	ivp_info("enter");
	TEEK_CloseSession(&g_secivp_session);
	TEEK_FinalizeContext(&g_secivp_context);
	g_secivp_creat_teec = false;
}

int teek_secivp_sec_cfg(int sharefd, unsigned int size)
{
	TEEC_Result result;
	TEEC_Operation operation;
	unsigned int origin = 0;

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	if (!g_secivp_creat_teec) {
		ivp_err("secivp teec not create");
		return -EPERM;
	}

	if (size == 0) {
		ivp_err("ISP buffer no need set sec cfg");
		return EOK;
	}

	if (sharefd < 0) {
		ivp_err("sharefd invalid %d", sharefd);
		return -EINVAL;
	}

	operation.started = IVP_TA_TRUE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_ION_INPUT,
		TEEC_NONE,
		TEEC_NONE,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].ionref.ion_share_fd = sharefd;
	operation.params[TEE_PARAM_INDEX0].ionref.ion_size = size;

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_CAMERA_BUFFER_CFG, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("sec cfg failed, result[0x%x] origin[0x%x]", result, origin);
		return -EPERM;
	}
	return EOK;
}

int teek_secivp_sec_uncfg(int sharefd, unsigned int size)
{
	TEEC_Result result;
	TEEC_Operation operation;
	u32 origin = 0;

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	if (!g_secivp_creat_teec) {
		ivp_err("secivp teec not create");
		return -EPERM;
	}

	if (size == 0) {
		ivp_err("ISP buffer no need set sec uncfg");
		return EOK;
	}

	if (sharefd < 0) {
		ivp_err("sharefd invalid %d", sharefd);
		return -EINVAL;
	}

	operation.started = IVP_TA_TRUE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_ION_INPUT,
		TEEC_NONE,
		TEEC_NONE,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].ionref.ion_share_fd = sharefd;
	operation.params[TEE_PARAM_INDEX0].ionref.ion_size = size;

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_CAMERA_BUFFER_UNCFG, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("sec uncfg failed, result[0x%x] origin[0x%x]", result, origin);
		return -EPERM;
	}
	return EOK;
}

unsigned int teek_secivp_secmem_map(unsigned int sharefd, unsigned int size)
{
	TEEC_Result result;
	TEEC_Operation operation;
	secivp_mem_info buffer;
	unsigned int origin = 0;

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	(void)memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));

	buffer.sharefd = sharefd;
	buffer.size = size;
	buffer.prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE | IOMMU_SEC;
	buffer.sec_flag = SECURE;

	operation.started = IVP_TA_TRUE;
	operation.cancel_flag = IVP_TA_FALSE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_OUTPUT,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].value.a = buffer.sharefd;
	operation.params[TEE_PARAM_INDEX0].value.b = buffer.size;
	operation.params[TEE_PARAM_INDEX1].tmpref.buffer = &buffer;
	operation.params[TEE_PARAM_INDEX1].tmpref.size = sizeof(buffer);

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_ALGO_SEC_MAP, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("ivp map failed, result[0x%x] origin[0x%x]", result, origin);
		return INVALID_IOVA;
	}

	return operation.params[TEE_PARAM_INDEX2].value.a;
}

int teek_secivp_secmem_unmap(unsigned int sharefd, unsigned int size)
{
	TEEC_Result result;
	TEEC_Operation operation;
	secivp_mem_info buffer;
	unsigned int origin = 0;

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	(void)memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));

	buffer.sharefd = sharefd;
	buffer.size = size;
	buffer.prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE | IOMMU_SEC;
	buffer.sec_flag = SECURE;

	operation.started = IVP_TA_TRUE;
	operation.cancel_flag = IVP_TA_FALSE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_NONE,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].value.a = buffer.sharefd;
	operation.params[TEE_PARAM_INDEX0].value.b = buffer.size;
	operation.params[TEE_PARAM_INDEX1].tmpref.buffer = &buffer;
	operation.params[TEE_PARAM_INDEX1].tmpref.size = sizeof(buffer);

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_ALGO_SEC_UNMAP, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("ivp ummap failed, result[0x%x] origin[0x%x]", result, origin);
		return -EPERM;
	}
	return EOK;
}

unsigned int teek_secivp_nonsecmem_map(int sharefd, unsigned int size,
	struct sglist *sgl)
{
	TEEC_Result result;
	TEEC_Operation operation;
	secivp_mem_info buffer;
	unsigned int origin = 0;

	if (sharefd < 0 || !sgl) {
		ivp_err("invalid input");
		return INVALID_IOVA;
	}
	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	(void)memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));

	buffer.sharefd = sharefd;
	buffer.size = size;
	buffer.prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE;
	buffer.sec_flag = NON_SECURE;
	sgl->ion_size = size;

	operation.started = IVP_TA_TRUE;
	operation.cancel_flag = IVP_TA_FALSE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_OUTPUT,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].tmpref.buffer = (void *)sgl;
	operation.params[TEE_PARAM_INDEX0].tmpref.size = sgl->sglist_size;
	operation.params[TEE_PARAM_INDEX1].tmpref.buffer = &buffer;
	operation.params[TEE_PARAM_INDEX1].tmpref.size = sizeof(buffer);

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_ALGO_NONSEC_MAP, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("ivp map failed, result[0x%x] origin[0x%x]", result, origin);
		return INVALID_IOVA;
	}
	return operation.params[TEE_PARAM_INDEX2].value.a;
}

int teek_secivp_nonsecmem_unmap(int sharefd, unsigned int size)
{
	TEEC_Result result;
	TEEC_Operation operation;
	secivp_mem_info buffer;
	unsigned int origin = 0;

	if (sharefd < 0) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	(void)memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));

	buffer.sharefd = sharefd;
	buffer.size = size;
	buffer.prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE;
	buffer.sec_flag = NON_SECURE;

	operation.started = IVP_TA_TRUE;
	operation.cancel_flag = IVP_TA_FALSE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_NONE,
		TEEC_NONE,
		TEEC_NONE);
	operation.params[TEE_PARAM_INDEX0].tmpref.buffer = &buffer;
	operation.params[TEE_PARAM_INDEX0].tmpref.size = sizeof(buffer);

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_ALGO_NONSEC_UNMAP, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		ivp_err("ivp ummap failed, result[0x%x] origin[0x%x]", result, origin);
		return -EPERM;
	}

	return EOK;
}

void teek_secivp_clear(void)
{
	TEEC_Result result;
	TEEC_Operation operation;
	unsigned int origin = 0;

	(void)memset_s(&operation, sizeof(operation), 0, sizeof(operation));
	if (!g_secivp_creat_teec) {
		ivp_err("secivp teec not create");
		return;
	}

	operation.started = IVP_TA_TRUE;
	operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_NONE,
		TEEC_NONE,
		TEEC_NONE,
		TEEC_NONE);

	result = TEEK_InvokeCommand(&g_secivp_session,
		TEE_SECIVP_CMD_IVP_CLEAR, &operation, &origin);
	if (result != TEEC_SUCCESS)
		ivp_err("clear failed, result[0x%x] origin[0x%x]", result, origin);
	return;
}

int teek_secivp_ca_probe(void)
{
	if (g_secivp_creat_teec) {
		ivp_warn("ta has opened");
		return EOK;
	}
	return teek_secivp_open();
}

void teek_secivp_ca_remove(void)
{
	if (!g_secivp_creat_teec) {
		ivp_warn("ta has closed");
		return;
	}
	teek_secivp_close();
}

