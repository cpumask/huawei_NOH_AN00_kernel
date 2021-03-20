/*
 * teek_client_api.h
 *
 * function declaration for libteec interface for kernel CA.
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

/* @defgroup TEEC_API �ͻ���(�ǰ�ȫ��)�ӿ�
 * @defgroup TEEC_BASIC_FUNC ͨ�ýӿ�
 * @ingroup TEEC_API
 */

#ifndef _TEEK_CLIENT_API_H_
#define _TEEK_CLIENT_API_H_
#include "teek_ns_client.h"
#include "teek_client_type.h"

/*
 * @ingroup TEEC_BASIC_FUNC
 * ���ڼ���ǰ�ȫ�����밲ȫ���紫�ݲ�������ֵ
 */
#define TEEC_PARAM_TYPES(param0_type, param1_type, param2_type, param3_type) \
		((param3_type) << 12 | (param2_type) << 8 | \
		 (param1_type) << 4 | (param0_type))

/*
 * @ingroup TEEC_BASIC_FUNC
 * ���ڼ���paramTypes���ֶ�index����ֵ
 */
#define TEEC_PARAM_TYPE_GET(param_types, index) \
		(((param_types) >> ((index) << 2)) & 0x0F)

/*
 * @ingroup TEEC_BASIC_FUNC
 * ����������Ϊ#teec_valueʱ�������Ա����a��bû�и���ֵ���踳���ֵ��
 * ��ʾû���õ��˳�Ա����
 */
#define TEEC_VALUE_UNDEF 0xFFFFFFFF

/*
 * Function:        teek_is_agent_alive
 * Description:   This function check if the special agent is launched.
 *                         Used For HDCP key.
 *                         e.g. If sfs agent is not alive,
 *                         you can not do HDCP key write to SRAM.
 * Parameters:   agent_id.
 * Return:          1:agent is alive
 *                        0:agent not exsit.
 */
int teek_is_agent_alive(unsigned int agent_id);
/*
 * @ingroup  TEEC_BASIC_FUNC
 * @brief ��ʼ��TEE����
 *
 * @par ����:
 * ��ʼ��·��Ϊname��TEE����������name����Ϊ�գ�
 * ��ʼ��TEE�����Ǵ򿪻Ự����������Ļ�����
 * ��ʼ���ɹ��󣬿ͻ���Ӧ����TEE����һ�����ӡ�
 *
 * @attention ��
 * @param name [IN] TEE����·��
 * @param context [IN/OUT] contextָ�룬��ȫ���绷�����
 *
 * @retval #TEEC_SUCCESS ��ʼ��TEE�����ɹ�
 * @retval #TEEC_ERROR_BAD_PARAMETERS ��������ȷ��name����ȷ��contextΪ��
 * @retval #TEEC_ERROR_GENERIC ϵͳ������Դ�����ԭ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_FinalizeContext
 * @since V100R002C00B301
 */
uint32_t teek_initialize_context(const char *name, struct teec_context *context);

/*
 * @ingroup  TEEC_BASIC_FUNC
 * @brief �ر�TEE����
 *
 * @par ����:
 * �ر�contextָ���TEE�������Ͽ��ͻ���Ӧ����TEE����������
 *
 * @attention ��
 * @param context [IN/OUT] ָ���ѳ�ʼ���ɹ���TEE����
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_InitializeContext
 * @since V100R002C00B301
 */
void teek_finalize_context(struct teec_context *context);

/*
 * @ingroup  TEEC_BASIC_FUNC
 * @brief �򿪻Ự
 *
 * @par ����:
 * ��ָ����TEE����context�£�Ϊ�ͻ���Ӧ����UUID
 * Ϊdestination�İ�ȫ������һ�����ӣ�
 * ���ӷ�ʽ��connectionMethod������������connectionData,
 * ���ݵ����ݰ�����opetation�
 * �򿪻Ự�ɹ����������session�ǶԸ����ӵ�һ��������
 * ����򿪻Ựʧ�ܣ��������returnOriginΪ������Դ��
 *
 * @attention ��
 * @param context [IN/OUT] ָ���ѳ�ʼ���ɹ���TEE����
 * @param session [OUT] ָ��Ự��ȡֵ����Ϊ��
 * @param destination [IN] ��ȫ�����UUID��һ����ȫ����ӵ��Ψһ��UUID
 * @param connectionMethod [IN] ���ӷ�ʽ��ȡֵ��ΧΪ#TEEC_LoginMethod
 * @param connectionData [IN] �����ӷ�ʽ���Ӧ���������ݣ�
 * ������ӷ�ʽΪ#TEEC_LOGIN_PUBLIC, #TEEC_LOGIN_USE,
 * #TEEC_LOGIN_USER_APPLICATION,
 * #TEEC_LOGIN_GROUP_APPLICATION, ��������ȡֵ����Ϊ�գ�
 * ������ӷ�ʽΪ#TEEC_LOGIN_GROUP��#TEEC_LOGIN_GROUP_APPLICATION��
 * �������ݱ���ָ������Ϊuint32_t�����ݣ������ݱ�ʾ�ͻ���Ӧ���������ӵ����û�
 * @param operation [IN/OUT] �ͻ���Ӧ���밲ȫ���񴫵ݵ�����
 * @param returnOrigin [IN/OUT] ������Դ��ȡֵ��ΧΪ#TEEC_ReturnCodeOrigin
 *
 * @retval #TEEC_SUCCESS �򿪻Ự�ɹ�
 * @retval #TEEC_ERROR_BAD_PARAMETERS ��������ȷ������contextΪ�ջ�sessionΪ�ջ�destinationΪ��
 * @retval #TEEC_ERROR_ACCESS_DENIED ϵͳ����Ȩ�޷���ʧ��
 * @retval #TEEC_ERROR_OUT_OF_MEMORY ϵͳ������Դ����
 * @retval #TEEC_ERROR_TRUSTED_APP_LOAD_ERROR ���ذ�ȫ����ʧ��
 * @retval ��������ֵ�ο� #TEEC_ReturnCode
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_CloseSession
 * @since V100R002C00B301
 */
uint32_t teek_open_session(struct teec_context *context,
	struct teec_session *session,
	const struct teec_uuid *destination,
	uint32_t connection_method,
	const void *connection_data,
	const struct teec_operation *operation,
	uint32_t *returnOrigin);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief �رջỰ
 *
 * @par ����:
 * �ر�sessionָ��ĻỰ���Ͽ��ͻ���Ӧ���밲ȫ���������
 *
 * @attention ��
 * @param session [IN/OUT] ָ���ѳɹ��򿪵ĻỰ
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_OpenSession
 * @since V100R002C00B301
 */
void teek_close_session(struct teec_session *session);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief ��������
 *
 * @par ����:
 * ��ָ���ĻỰsession��ɿͻ���Ӧ����ȫ����������commandID��
 * ���͵�����Ϊoperation�������������ʧ�ܣ��������returnOriginΪ������Դ
 *
 * @attention ��
 * @param session [IN/OUT] ָ���Ѵ򿪳ɹ��ĻỰ
 * @param commandID [IN] ��ȫ����֧�ֵ�����ID���ɰ�ȫ������
 * @param operation [IN/OUT] �����˿ͻ���Ӧ����ȫ�����͵���������
 * @param returnOrigin [IN/OUT] ������Դ��ȡֵ��ΧΪ#TEEC_ReturnCodeOrigin
 *
 * @retval #TEEC_SUCCESS ��������ɹ�
 * @retval #TEEC_ERROR_BAD_PARAMETERS ��������ȷ������sessionΪ�ջ����operation��ʽ����ȷ
 * @retval #TEEC_ERROR_ACCESS_DENIED ϵͳ����Ȩ�޷���ʧ��
 * @retval #TEEC_ERROR_OUT_OF_MEMORY ϵͳ������Դ����
 * @retval ��������ֵ�ο� #TEEC_ReturnCode
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see ��
 * @since V100R002C00B301
 */
uint32_t teek_invoke_command(struct teec_session *session,
	uint32_t commandID,
	struct teec_operation *operation,
	uint32_t *returnOrigin);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief ע�Ṳ���ڴ�
 *
 * @par ����:
 * ��ָ����TEE����context��ע�Ṳ���ڴ�sharedMem��
 * ͨ��ע��ķ�ʽ��ȡ�����ڴ���ʵ���㿽����Ҫ����ϵͳ��֧�֣�
 * Ŀǰ��ʵ���У��÷�ʽ����ʵ���㿽��
 *
 * @attention ������sharedMem��size������Ϊ0�������᷵�سɹ������޷�ʹ�����
 * �����ڴ棬��Ϊ����ڴ�û�д�С
 * @param context [IN/OUT] �ѳ�ʼ���ɹ���TEE����
 * @param sharedMem [IN/OUT] �����ڴ�ָ�룬�����ڴ���ָ����ڴ治��Ϊ�ա���С����Ϊ��
 *
 * @retval #TEEC_SUCCESS ��������ɹ�
 * @retval #TEEC_ERROR_BAD_PARAMETERS ��������ȷ������contextΪ�ջ�sharedMemΪ�գ�
 * �����ڴ���ָ����ڴ�Ϊ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_AllocateSharedMemory
 * @since V100R002C00B301
 */
uint32_t teek_register_shared_memory(struct teec_context *context,
	struct teec_sharedmemory *shared_mem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief ���빲���ڴ�
 *
 * @par ����:
 * ��ָ����TEE����context�����빲���ڴ�sharedMem��
 * ͨ�������ڴ����ʵ�ַǰ�ȫ�����밲ȫ���紫������ʱ���㿽��
 *
 * @attention ������sharedMem��size������Ϊ0�������᷵�سɹ������޷�ʹ�����
 * �����ڴ棬��Ϊ����ڴ��û�е�ַҲû�д�С
 * @param context [IN/OUT] �ѳ�ʼ���ɹ���TEE����
 * @param sharedMem [IN/OUT] �����ڴ�ָ�룬�����ڴ�Ĵ�С����Ϊ��
 *
 * @retval #TEEC_SUCCESS ��������ɹ�
 * @retval #TEEC_ERROR_BAD_PARAMETERS ��������ȷ������contextΪ�ջ�sharedMemΪ��
 * @retval #TEEC_ERROR_OUT_OF_MEMORY ϵͳ������Դ���㣬����ʧ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_RegisterSharedMemory
 * @since V100R002C00B301
 */
uint32_t teek_allocate_shared_memory(struct teec_context *context,
	struct teec_sharedmemory *shared_mem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief �ͷŹ����ڴ�
 *
 * @par ����:
 * �ͷ���ע��ɹ��ĵĻ�������ɹ��Ĺ����ڴ�sharedMem
 *
 * @attention �����ͨ��#TEEC_AllocateSharedMemory��ʽ��ȡ�Ĺ����ڴ棬
 * �ͷ�ʱ���������ڴ棻�����ͨ��#TEEC_RegisterSharedMemory��ʽ
 * ��ȡ�Ĺ����ڴ棬�ͷ�ʱ������չ����ڴ���ָ��ı����ڴ�
 * @param sharedMem [IN/OUT] ָ����ע��ɹ�������ɹ��Ĺ����ڴ�
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see TEEC_RegisterSharedMemory | TEEC_AllocateSharedMemory
 * @since V100R002C00B301
 */
void teek_release_shared_memory(struct teec_sharedmemory *shared_mem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief cancel API
 *
 * @par ����:
 * ȡ����һ���������е�open Session������һ��invoke command
 * ����һ��cancel��signal����������
 *
 * @attention �˲��������Ƿ���һ��cancel����Ϣ���Ƿ����cancel������ǰ���TEE �� TA����
 * @param operation [IN/OUT] �����˿ͻ���Ӧ����ȫ�����͵���������
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_api.h���ýӿ���������ͷ�ļ�
 * @see ��
 * @since V100R002C00B309
 */
void teek_request_cancellation(struct teec_operation *operation);

/* begin: for KERNEL-HAL out interface */

#ifdef CONFIG_KERNEL_CLIENT

int TEEK_IsAgentAlive(unsigned int agent_id);

TEEC_Result TEEK_InitializeContext(const char *name, TEEC_Context *context);

void TEEK_FinalizeContext(TEEC_Context *context);

TEEC_Result TEEK_OpenSession(TEEC_Context *context,
	TEEC_Session *session,
	const TEEC_UUID *destination,
	uint32_t connectionMethod,
	const void *connectionData,
	TEEC_Operation *operation,
	uint32_t *returnOrigin);

void TEEK_CloseSession(TEEC_Session *session);

TEEC_Result TEEK_InvokeCommand(TEEC_Session *session,
	uint32_t commandID,
	TEEC_Operation *operation,
	uint32_t *returnOrigin);

TEEC_Result TEEK_RegisterSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem);

TEEC_Result TEEK_AllocateSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem);

void TEEK_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem);

void TEEK_RequestCancellation(TEEC_Operation *operation);

#else

static inline int TEEK_IsAgentAlive(unsigned int agent_id)
{
	(void)agent_id;
	return TEEC_SUCCESS;
}

static inline TEEC_Result TEEK_InitializeContext(const char *name, TEEC_Context *context)
{
	(void)name;
	(void)context;
	return TEEC_SUCCESS;
}

static inline void TEEK_FinalizeContext(TEEC_Context *context)
{
	(void)context;
}

static inline TEEC_Result TEEK_OpenSession(TEEC_Context *context, TEEC_Session *session,
	const TEEC_UUID *destination, uint32_t connectionMethod,
	const void *connectionData, TEEC_Operation *operation,
	uint32_t *returnOrigin)
{
	(void)context;
	(void)session;
	(void)destination;
	(void)connectionMethod;
	(void)connectionData;
	(void)operation;
	(void)returnOrigin;
	return TEEC_SUCCESS;
}

static inline void TEEK_CloseSession(TEEC_Session *session)
{
	(void)session;
}

static inline TEEC_Result TEEK_InvokeCommand(TEEC_Session *session, uint32_t commandID,
	TEEC_Operation *operation, uint32_t *returnOrigin)
{
	(void)session;
	(void)commandID;
	(void)operation;
	(void)returnOrigin;
	return TEEC_SUCCESS;
}

static inline TEEC_Result TEEK_RegisterSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem)
{
	(void)context;
	(void)sharedMem;
	return TEEC_SUCCESS;
}

static inline TEEC_Result TEEK_AllocateSharedMemory(TEEC_Context *context,
	TEEC_SharedMemory *sharedMem)
{
	(void)context;
	(void)sharedMem;
	return TEEC_SUCCESS;
}

static inline void TEEK_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem)
{
	(void)sharedMem;
}

static inline void TEEK_RequestCancellation(TEEC_Operation *operation)
{
	(void)operation;
}

#endif

/* end: for KERNEL-HAL out interface */

#endif
