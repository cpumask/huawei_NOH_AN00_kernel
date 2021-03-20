

#ifndef __MSCP_ERRNO_H__
#define __MSCP_ERRNO_H__

#define OK              0x0
#define MSPC_OK         0x5A5A
#define MSPC_ERROR      0xA5A5

/* mspc kernel err prefix */
#define ERR_PREFIX      0xA7

#define MSPC_THIS_MODULE     MSPC_MODULE_KERNEL

#define ERR_MAKEUP(prefix, module, errcode) \
	(((prefix) << 24U) | (((module) & 0xff) << 16U) | ((errcode) & 0xffff))

#define MSPC_ERRCODE(errcode) \
	ERR_MAKEUP(ERR_PREFIX, MSPC_THIS_MODULE, (errcode))

enum mspc_module_name {
	MSPC_MODULE_KERNEL     = 0x00,
	/* Add other module in future. */
};

#define MSPC_SECLIB_ERROR                    (-1) /* must be -1 */

#define MSPC_GET_MSPC_LCS_ERROR              MSPC_ERRCODE(0x10)
#define MSPC_GET_LCS_VALUE_ERROR             MSPC_ERRCODE(0x11)
#define MSPC_IOCTL_NODE_CREATE_ERROR         MSPC_ERRCODE(0x12)
#define MSPC_POWER_NODE_CREATE_ERROR         MSPC_ERRCODE(0x13)
#define MSPC_CMA_DEVICE_INIT_ERROR           MSPC_ERRCODE(0x15)
#define MSPC_INVALID_PARAMS_ERROR            MSPC_ERRCODE(0x16)
#define MSPC_WAIT_READY_TIMEOUT_ERROR        MSPC_ERRCODE(0x17)
#define MSPC_FIRST_SMC_CMD_ERROR             MSPC_ERRCODE(0x18)
#define MSPC_SMC_CMD_TIMEOUT_ERROR           MSPC_ERRCODE(0x19)
#define MSPC_SMC_CMD_PROCESS_ERROR           MSPC_ERRCODE(0x1A)
#define MSPC_INVALID_PARAM_ERROR             MSPC_ERRCODE(0x1B)
#define MSPC_IS_UPGRADING_ERROR              MSPC_ERRCODE(0x1C)
#define MSPC_RPMB_MODULE_INIT_ERROR          MSPC_ERRCODE(0x1D)
#define MSPC_INVALID_FUNCTION_ERROR          MSPC_ERRCODE(0x1E)
#define MSPC_LCS_MODE_ERROR                  MSPC_ERRCODE(0x1F)
#define MSPC_FLASH_COS_ERROR                 MSPC_ERRCODE(0x20)
#define MSPC_ENCOS_FILES_CNT_ERROR           MSPC_ERRCODE(0x23)
#define MSPC_ENCOS_TOTAL_SIZE_ERROR          MSPC_ERRCODE(0x24)
#define MSPC_ENCOS_SIZE_ERROR                MSPC_ERRCODE(0x25)
#define MSPC_SECUREC_ERROR                   MSPC_ERRCODE(0x26)
#define MSPC_ENCOS_LSEEK_FILE_ERROR          MSPC_ERRCODE(0x27)
#define MSPC_ENCOS_WRITE_FILE_ERROR          MSPC_ERRCODE(0x28)
#define MSPC_NO_RESOURCES_ERROR              MSPC_ERRCODE(0x29)
#define MSPC_SYNC_FILE_ERROR                 MSPC_ERRCODE(0x2A)
#define MSPC_ENCOS_FIND_PTN_ERROR            MSPC_ERRCODE(0x2B)
#define MSPC_ENCOS_OPEN_FILE_ERROR           MSPC_ERRCODE(0x2C)
#define HISEE_OLD_COS_IMAGE_ERROR            MSPC_ERRCODE(0x2D)
#define MSPC_OLD_IMAGE_ERROR                 MSPC_ERRCODE(0x2E)
#define MSPC_RPMB_KEY_UNREADY_ERROR          MSPC_ERRCODE(0x2F)
#define MSPC_OPEN_FILE_ERROR                 MSPC_ERRCODE(0x30)
#define MSPC_LSEEK_FILE_ERROR                MSPC_ERRCODE(0x31)
#define MSPC_ACCESS_FILE_ERROR               MSPC_ERRCODE(0x32)
#define MSPC_GET_ACCESSING_INFO_ERROR        MSPC_ERRCODE(0x33)
#define MSPC_IMG_SUB_FILE_ABSENT_ERROR       MSPC_ERRCODE(0x34)
#define MSPC_IMG_HEADER_TIMESTAMP_ERROR      MSPC_ERRCODE(0x35)
#define MSPC_SUB_FILE_SIZE_ERROR             MSPC_ERRCODE(0x36)
#define MSPC_SUB_FILE_ID_ERROR               MSPC_ERRCODE(0x38)
#define MSPC_IMG_PARTITION_MAGIC_ERROR       MSPC_ERRCODE(0x39)
#define MSPC_IMG_PARTITION_FILES_ERROR       MSPC_ERRCODE(0x3A)
#define MSPC_THREAD_CREATE_ERROR             MSPC_ERRCODE(0x3B)
#define MSPC_RPMB_SWITCH_ERROR               MSPC_ERRCODE(0x3C)
#define MSPC_RPMB_SWITCH_TIMEOUT             MSPC_ERRCODE(0x3D)
#define MSPC_FS_FIND_PTN_ERROR               MSPC_ERRCODE(0x3E)
#define MSPC_FS_OPEN_FILE_ERROR              MSPC_ERRCODE(0x3F)
#define MSPC_FS_ACCESS_FILE_ERROR            MSPC_ERRCODE(0x40)
#define MSPC_FS_CHECK_ZERO_ERROR             MSPC_ERRCODE(0x41)
#define MSPC_CHECK_SECDEBUG_ERROR            MSPC_ERRCODE(0x42)
#define MSPC_BOOT_UPGRADE_CHK_ERROR          MSPC_ERRCODE(0x46)
#define MSPC_ENTER_FACTROY_MODE_ERROR        MSPC_ERRCODE(0x47)
#define MSPC_EXIT_FACTROY_MODE_ERROR         MSPC_ERRCODE(0x48)
#define MSPC_SECFLASH_DTS_FOUND_ERROR        MSPC_ERRCODE(0x49)

#define MSPC_AT_CMD_UNSUPPORT_ERROR          MSPC_ERRCODE(0x0100)
#define MSPC_AT_CMD_NOTRUNNING_ERROR         MSPC_ERRCODE(0x0101)
#define MSPC_AT_PARAM_INVALID_ERROR          MSPC_ERRCODE(0x0102)
#define MSPC_AT_SET_RESP_DATA_ERROR          MSPC_ERRCODE(0x0103)
#define MSPC_AT_CTRL_NODE_CREATE_ERROR       MSPC_ERRCODE(0x0104)
#define MSPC_AT_RSP_CALLBACK_NULL_ERROR      MSPC_ERRCODE(0x0105)
#define MSPC_AT_INSTALL_STATE_ERROR          MSPC_ERRCODE(0x0106)
#define MSPC_AT_SOCSLT_STATE_ERROR           MSPC_ERRCODE(0x0107)
#define MSPC_AT_SOCSLT_RUNNING_ERROR         MSPC_ERRCODE(0x0108)
#define MSPC_AT_SMX_ENABLE_ERROR             MSPC_ERRCODE(0x0109)
#define MSPC_AT_TEST_ERROR                   MSPC_ERRCODE(0x010A)

#define MSPC_CHANNEL_TEST_CMD_ERROR            MSPC_ERRCODE(0x0600)
#define MSPC_CHANNEL_TEST_RESULT_MALLOC_ERROR  MSPC_ERRCODE(0x0601)
#define MSPC_CHANNEL_TEST_PATH_ABSENT_ERROR    MSPC_ERRCODE(0x0602)
#define MSPC_CHANNEL_TEST_WRITE_RESULT_ERROR   MSPC_ERRCODE(0x0603)

#endif /* __MSCP_ERRNO_H__ */
