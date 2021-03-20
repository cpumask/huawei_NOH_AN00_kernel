/*
 * copyright: Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 *
 * file: bfr_core.h
 *
 * define the core external public enum/macros/interface for BFR
 * (Boot Fail Recovery)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef BFR_CORE_H
#define BFR_CORE_H

#include "chipset_common/bfmr/public/bfmr_public.h"
#include "chipset_common/bfmr/common/bfmr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RRECORD_HEAD_RESERVED   68
#define RRECORD_RESERVED        68
#define RRECORD_BOOTFAIL_DETAIL 16

enum bfr_method {
	/*
	 * in case that the boot fail has been processed in frmawork layer,
	 * the BFMR should set the suggested recovery method as BFR_DO_NOTHING
	 * when invoke function
	 * try_to_recovery
	 */
	FRM_DO_NOTHING = 0,

	/*
	 * this is the basic recovery method for the most boot fail
	 */
	FRM_REBOOT,

	/*
	 * if the phone has A/B system and the B system is valid, when A system
	 * can not boot successfully, the BFR will use B system to do bootup
	 */
	FRM_GOTO_B_SYSTEM,

	/*
	 * this recovery method is used for the native and framework normal boot
	 * fail when the reboot action can not recovery the boot fail.
	 */
	FRM_DEL_FILES_FOR_BF,

	/*
	 * this recovery method is used when the available space of data
	 * partition is below the threshold, which will resulting in boot fail
	 */
	FRM_DEL_FILES_FOR_NOSPC,

	/*
	 * if deleting files in data can not recovery the boot fail, the further
	 * recovery method is suggesting the user to do factory data reset.
	 */
	FRM_FACTORY_RST,

	/*
	 * this recovery method is used for the extreme case that the action of
	 * mounting data partirion will result in panic in kernel
	 */
	FRM_FORMAT_DATA_PART,

	/* download the latest OTA full package to do recovery */
	FRM_DOWNLOAD,

	/*
	 * download the latest OTA full package to do recovery
	 * and in the end delete the key files in data partition
	 */
	FRM_DOWNLOAD_AND_DEL_FILES,

	/*
	 * download the latest OTA full package to do recovery
	 * and in the end suggest the user to do factory reset
	 */
	FRM_DOWNLOAD_AND_FACTORY_RST,

	/*
	 * if the recovery can not bootup successfully for ever, if we should
	 * notify the user in time
	 */
	FRM_NOTIFY_USER_FAIL,
	FRM_LOWLEVEL_FORMAT_DATA,
	FRM_ENTER_SAFE_MODE,
	FRM_FACTORY_RST_AFTER_DOWNLOAD,
	FRM_BOPD,
	FRM_STORAGE_RDONLY_BOPD,
	FRM_HW_DEGRADE_BOPD,
	FRM_STORAGE_RDONLY_HW_DEGRADE_BOPD,
	FRM_BOPD_AFTER_DOWNLOAD,
	FRM_HW_DEGRADE_BOPD_AFTER_DOWNLOAD,
	FRM_RECOVERY_METHOD_MAX_COUNT
};

enum bfr_suggested_method {
	/*
	 * if you can not determine the recovery method for a boot fail,
	 * transfer NO_SUGGESTION to the BFR.
	 */
	NO_SUGGESTION = 0x0,
	DO_NOTHING = 0x1,
};

struct bfr_fixed_method_param {
	/* 0 - disable; 1 - enable */
	int enable_this_method;
	enum bfr_method recovery_method;
};

struct bfr_recovery_policy {
	enum bfm_errno_code boot_fail_no;

	/* if the boot fail no has fixed recovery method. 1 - has one, 0 - has none */
	int has_fixed_policy;
	struct bfr_fixed_method_param param[4];
};

enum bfr_enter_erecovery_reason {
	/* enter erecovery because of boot fail */
	EER_BOOT_FAIL_SOLUTION = 0,

	/*
	 * enter erecovery because of user pressing the VolumeUp key when power
	 * on the phone
	 */
	EER_PRESS_VOLUMEUP_KEY,

	/*
	 * enter erecovery because of user pressing the VolumeUp key when
	 * verify failed
	 */
	EER_VERIFY_BOOT,
	EER_MAX_COUNT
};

enum bfr_bf_stage {
	/* boot fail occurs on bootloader stage */
	BFS_BOOTLOADER = 0,

	/* boot fail occurs on kernel stage */
	BFS_KERNEL,

	/* boot fail occurs on native or framework stage */
	BFS_APP,
};

struct bfr_bf_no_range {
	unsigned int start;
	unsigned int end;
};

struct bfr_enter_erecovery_reason_map {
	struct bfr_bf_no_range range;

	/* this will be used by erecovery to communicate with BI server */
	unsigned int enter_erecovery_reason;
};

enum bfr_method_running_status {
	RMRSC_EXEC_COMPLETED = 0,
	RMRSC_ERECOVERY_BOOT_FAILED = 1,
	RMRSC_ERECOVERY_BOOT_SUCCESS,
	RMRSC_ERECOVERY_START_DEL_FILES,
	RMRSC_ERECOVERY_DEL_FILES_SUCCESS,
	RMRSC_ERECOVERY_DEL_FILES_FAILED,
	RMRSC_ERECOVERY_START_WIFI,
	RMRSC_ERECOVERY_START_WIFI_SUCCESS,
	RMRSC_ERECOVERY_START_WIFI_FAILED,
	RMRSC_ERECOVERY_SATRT_QUERY_PACKAGES,
	RMRSC_ERECOVERY_QUERY_PACKAGES_SUCCESS,
	RMRSC_ERECOVERY_QUERY_PACKAGES_FAILED,
	RMRSC_ERECOVERY_START_DOWNLOAD_PACKAGES,
	RMRSC_ERECOVERY_DOWNLOAD_PACKAGES_SUCCESS,
	RMRSC_ERECOVERY_DOWNLOAD_PACKAGES_FAILED,
	RMRSC_ERECOVERY_START_DOWNLOAD_UPDATE_AUTH_FILES,
	RMRSC_ERECOVERY_DOWNLOAD_UPDATE_AUTH_FILES_SUCCESS,
	RMRSC_ERECOVERY_DOWNLOAD_UPDATE_AUTH_FILES_FAILED,
	RMRSC_ERECOVERY_START_INSTALL_PACKAGES,
	RMRSC_ERECOVERY_INSTALL_PACKAGES_SUCCESS,
	RMRSC_ERECOVERY_INSTALL_PACKAGES_FAILED,
	RMRSC_ERECOVERY_MOUNT_USERDATA_FAILED,
	RMRSC_ERECOVERY_MOUNT_USERDATA_SUCCESS,
};

enum bfr_method_run_result {
	RMRR_SUCCESS = 0,
	RMRR_FAILED,
};

enum bfr_recovery_result {
	BF_RECOVERY_SUCCESS = ((unsigned int)0xA5A5A5A5),
	BF_RECOVERY_FAILURE = ((unsigned int)0x5A5A5A5A),
};

struct bfr_record_header {
	unsigned char sha256[BFMR_SHA256_HASH_LEN];
	unsigned int magic_number;
	int boot_fail_count;
	int last_record_idx;
	int next_record_idx;

	/* record total count can be saved in the rrecord partition */
	int record_count;

	/* this field shouid be set as 0 when boot success */
	int rec_cnt_before_boot_succ;

	/* if this fiels is 0x656e736d means safe mode has been enabled */
	unsigned int safe_mode_enable_flag;
	char reserved[RRECORD_HEAD_RESERVED];
} __attribute__((__packed__));

struct bfr_record {
	unsigned long long boot_fail_detected_time;
	unsigned int boot_fail_stage;
	unsigned int boot_fail_no;
	unsigned int recovery_method;
	enum bfr_method_running_status running_status_code;
	enum bfr_method_run_result method_run_result;
	enum bfr_recovery_result recovery_result;
	unsigned int factory_reset_flag;
	unsigned int recovery_method_original;
	unsigned int boot_fail_time; /* boot time when bootfail happened */
	char bootfail_detail[RRECORD_BOOTFAIL_DETAIL];
	char reserved[RRECORD_RESERVED];
} __attribute__((__packed__));

struct bfr_record_param {
	unsigned char *buf;
	unsigned int buf_size;
	unsigned int part_offset;
	int record_damaged; /* 0 - good, 1 - damaged */

	/* this field shouid be set as 0 when boot success */
	int rec_cnt_before_boot_succ;
};

struct bfr_real_rcv_info {
	unsigned int boot_fail_rtc_time[BFM_LOG_MAX_COUNT];
	unsigned int boot_fail_time[BFM_LOG_MAX_COUNT];
	unsigned int boot_fail_no[BFM_LOG_MAX_COUNT];
	unsigned int boot_fail_stage[BFM_LOG_MAX_COUNT];
	unsigned int recovery_method[BFM_LOG_MAX_COUNT];
	unsigned int recovery_method_original[BFM_LOG_MAX_COUNT];
	int record_count;
};

enum bfr_misc_cmd {
	BFR_MISC_CMD_ERECOVERY = 0,
	BFR_MISC_CMD_RECOVERY,
	BFR_MISC_CMD_MAX_COUNT,
};

enum bfmr_selfheal_code {
	SH_BL1_REBOOT = 400000000,
	SH_BL1_FACTORY_RESET = 400000002,
	SH_BL1_SAFE_MODE = 400000003,
	SH_BL1_DOWNLOAD_VERSION = 400000004,
	SH_BL1_SELFHEAL = 400000019,
	SH_BL2_REBOOT = 400001000,
	SH_BL2_FACTORY_RESET = 400001002,
	SH_BL2_SAFE_MODE = 400001003,
	SH_BL2_DOWNLOAD_VERSION = 400001004,
	SH_BL2_SELFHEAL = 400001019,
	SH_KERNEL_REBOOT = 400002000,
	SH_KERNEL_FACTORY_RESET = 400002002,
	SH_KERNEL_SAFE_MODE = 400002002,
	SH_KERNEL_DOWNLOAD_VERSION = 400002004,
	SH_KERNEL_BOPD = 400002014,
	SH_KERNEL_STORAGE_RDONLY_BOPD = 400002015,
	SH_KERNEL_HARDWARE_DEGRADE_BOPD = 400002016,
	SH_KERNEL_BOPD_AFTER_DOWNLOAD = 400002017,
	SH_KERNEL_HARDWARE_DEGRADE_BOPD_AFTER_DOWNLOAD = 400002018,
	SH_KERNEL_SELFHEAL = 400002019,
	SH_NATIVE_REBOOT = 400003000,
	SH_NATIVE_FACTORY_RESET = 400003002,
	SH_NATIVE_SAFE_MODE = 400003003,
	SH_NATIVE_DOWNLOAD_VERSION = 400003004,
	SH_NATIVE_BOPD = 400003014,
	SH_NATIVE_STORAGE_RDONLY_BOPD = 400003015,
	SH_NATIVE_HARDWARE_DEGRADE_BOPD = 400003016,
	SH_NATIVE_BOPD_AFTER_DOWNLOAD = 400003017,
	SH_NATIVE_HARDWARE_DEGRADE_BOPD_AFTER_DOWNLOAD = 400003018,
	SH_NATIVE_SELFHEAL = 400003019,
	SH_FRAMEWORK_REBOOT = 400004000,
	SH_FRAMEWORK_FACTORY_RESET = 400004002,
	SH_FRAMEWORK_SAFE_MODE = 400004003,
	SH_FRAMEWORK_DOWNLOAD_VERSION = 400004004,
	SH_FRAMEWORK_BOPD = 400004014,
	SH_FRAMEWORK_STORAGE_RDONLY_BOPD = 400004015,
	SH_FRAMEWORK_HARDWARE_DEGRADE_BOPD = 400004016,
	SH_FRAMEWORK_BOPD_AFTER_DOWNLOAD = 400004017,
	SH_FRAMEWORK_HARDWARE_DEGRADE_BOPD_AFTER_DOWNLOAD = 400004018,
	SH_FRAMEWORK_SELFHEAL = 400004019,
	SH_UNKNOW = 400004999,
};

#define BFR_ENTER_ERECOVERY_CMD "boot-erecovery"
#define BFR_ENTER_RECOVERY_CMD  "boot-recovery"
#define BFR_ENTER_SAFE_MODE_CMD "boot-safemode"
#define BFR_ENTER_BOPD_MODE_CMD "boot-bopd"
#define BFR_MISC_PART_NAME "misc"
#define BFR_VALID_LONG_PRESS_LOG "valid_long_press"

/* 0x72656364-'r' 'e' 'c' 'd' */
#define BFR_RRECORD_MAGIC_NUMBER ((unsigned int)0x72656364)
#define BFR_ENTER_ERECOVERY_REASON_SIZE (sizeof(struct bfmr_rrecord_misc_param))
#define BFR_EACH_RECORD_PART_SIZE ((unsigned int)0x100000)
#define BFR_ENTER_ERECOVERY_REASON_OFFSET (BFR_MISC_PART_OFFSET)
#define BFR_RRECORD_1ST_PART_OFS (BFR_ENTER_ERECOVERY_REASON_SIZE)
#define BFR_RRECORD_2ND_PART_OFS (BFR_RRECORD_1ST_PART_OFS +\
	BFR_EACH_RECORD_PART_SIZE)
#define BFR_RRECORD_3RD_PART_OFS (BFR_RRECORD_2ND_PART_OFS +\
	BFR_EACH_RECORD_PART_SIZE)
#define BFR_SAFE_MODE_ENABLE_FLAG ((unsigned int)0x656e736d) /* ensm */
#define BFR_FACTORY_RESET_DONE ((unsigned int)0x6672646e) /* frdn */
#define BFR_FSCK_BEFORE_FORMAT_DATA ((unsigned int)0x6673666d) /* fsfm */
#define BFR_FORMAT_DATA_WITHOUT_FSCK ((unsigned int)0x666d6474) /* fmdt */
#define bfr_is_download_recovery_method(recovery_method) \
	((recovery_method == (unsigned int)FRM_DOWNLOAD) ||\
	(recovery_method == (unsigned int)FRM_DOWNLOAD_AND_DEL_FILES) ||\
	(recovery_method == (unsigned int)FRM_DOWNLOAD_AND_FACTORY_RST))

#define BOPD_BEFORE_DOWNLOAD ((unsigned long long)0x2)
#define BOPD_STORAGE_RDONLY_BEFORE_DOWNLOAD ((unsigned long long)0x3)
#define BOPD_HW_DEGRADE_BEFORE_DOWNLOAD ((unsigned long long)0x6)
#define BOPD_STORAGE_RDONLY_HW_DEGRADE_BEFORE_DOWNLOAD ((unsigned long long)0x7)
#define BOPD_AFTER_DOWNLOAD ((unsigned long long)0xa)
#define BOPD_HW_DEGRADE_AFTER_DOWNLOAD ((unsigned long long)0xe)
#define bfr_is_bopd_mode(bopd_mode_value) \
	((bopd_mode_value == BOPD_BEFORE_DOWNLOAD) ||\
	(bopd_mode_value == BOPD_STORAGE_RDONLY_BEFORE_DOWNLOAD) ||\
	(bopd_mode_value == BOPD_HW_DEGRADE_BEFORE_DOWNLOAD) ||\
	(bopd_mode_value == BOPD_STORAGE_RDONLY_HW_DEGRADE_BEFORE_DOWNLOAD) ||\
	(bopd_mode_value == BOPD_AFTER_DOWNLOAD) ||\
	(bopd_mode_value == BOPD_HW_DEGRADE_AFTER_DOWNLOAD))

#ifdef CONFIG_USE_BOOTFAIL_RECOVERY_SOLUTION
void release_rrecord_param(void);
int bfr_get_hardware_fault_times(
	struct bfmr_hw_fault_info_param *pfault_info_param);
int bfr_get_real_recovery_info(struct bfr_real_rcv_info *preal_recovery_info);
char *bfr_get_recovery_method_desc(int recovery_method);

/*
 * @brief: when the system bootup successfully, the BFM must call this
 * function to notify the BFR, and the BFM was notified by the BFD.
 */
void boot_status_notify(int boot_success);

/*
 * @brief: do recovery for the boot fail.
 *
 * @param: boot_fail_detected_time [in], rtc time when boot fail was detected.
 * @param: boot_fail_no [in], boot fail errno.
 * @param: boot_fail_stage [in], the stage when boot fail happened.
 * @param: suggested_recovery_method [in], suggested recovery method
 *         transferred by the BFD(Boot Fail Detection).
 * @param: args [in], extra parametrs for recovery.
 *
 * @return: the recovery method selected by the BFR.
 */
enum bfr_method try_to_recovery(
	unsigned long long boot_fail_detected_time,
	enum bfm_errno_code boot_fail_no,
	enum bfmr_detail_stage boot_fail_stage,
	enum bfr_suggested_method suggested_recovery_method,
	char *args);

int bfr_init(void);
#else
static inline int bfr_get_hardware_fault_times(struct bfmr_hw_fault_info_param
	*pfault_info_param)
{
	return -1;
}

static inline int bfr_get_real_recovery_info(struct bfr_real_rcv_info
	*preal_recovery_info)
{
	return 0;
}

static inline char *bfr_get_recovery_method_desc(int recovery_method)
{
	return "";
}

static inline void boot_status_notify(int boot_success)
{
	return;
}

static inline enum bfr_method try_to_recovery(
	unsigned long long boot_fail_detected_time,
	enum bfm_errno_code boot_fail_no,
	enum bfmr_detail_stage boot_fail_stage,
	enum bfr_suggested_method suggested_recovery_method,
	char *args)
{
	return FRM_DO_NOTHING;
}

static inline int bfr_init(void)
{
	return 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* BFR_CORE_H */

